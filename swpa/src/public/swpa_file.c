/**
* @file swpa_file.c
* @brief �ļ�����ʵ���ļ�
* 
* 
* @copyright Signalway All Rights Reserved
* @author zhouy
* @date 2013-02-27
* @version 1.0
*/



#include "swpa_file_private.h"
#include "swpa_eeprom_file.h"
#include "swpa_flash_file.h"
#include "swpa_normal_file.h"
#include "swpa_dsplink_file.h"
#include "swpa_comm_file.h"
#include "swpa_fifo_file.h"




/**
* @brief ���ļ�������
*
* �����ļ���������ò�ͬ�豸�Ĵ��ļ��ĺ���
* 
* @param [in] filename �ļ����� Ŀǰ֧�� ��ͨ�ļ��������ܵ��ļ��������ļ���EEPROM��FLASH��DSPLINK���涨�ļ������£�
* - �ļ���Ϊ��EEPROM/�豸id/�豸���������Ƿ���EEPROM�豸
* - �ļ���Ϊ��FLASH/�豸id/�豸���������Ƿ���FLASH�豸
* - �ļ���Ϊ��DSPLINK/�豸id��Ϊ����DSPLINK�豸
* - �ļ���Ϊ��COMM/�豸id��Ϊ���ʴ����豸
* - �ļ���Ϊ��FIFO/�ܵ�����Ϊ����FIFO�ļ�
* - �ļ���Ϊ��BLOCK/�ļ�����Ϊ������ͨ�����ļ�
* - �����ļ�����Ϊ��ͨ�Ĵ����ļ�
* @param [in] mode �ļ���ģʽ,�����¼���ģʽ:
* - "r"  : ֻ����ʽ��
* - "r+" : ��д��ʽ��
* - "w" :  ֻд��ʽ�򿪣��ļ����������ȴ���
* - "w+" :  ��д��ʽ�򿪣��ļ����������ȴ���
* - "a" :  ׷�ӷ�ʽ��
* - "a+" : ׷�ӷ�ʽ��,�ɶ�
* @retval �ļ�������: �ɹ�(ʵ���Ͼ���SWPA_FILEHEADER�ṹ��ָ��)
* @retval SWPAR_FAIL : ��ʧ��
* @retval SWPAR_INVALIDARG : �����Ƿ�
*/
int swpa_file_open(
	const char * filename, 
	const char * mode
)
{
	//������Ч�Լ��
	SWPA_FILE_CHECK(NULL != filename);    	//filenameָ��ǿ�
	SWPA_FILE_CHECK(0 != swpa_strcmp(filename, ""));//filename�ǿ��ַ���
	//SWPA_FILE_CHECK(NULL != mode);			//modeָ��ǿ�
	//SWPA_FILE_CHECK(0 != swpa_strcmp(mode, ""));	//mode�ǿ��ַ���

	//��ӡ����
	SWPA_FILE_PRINT("filename=%s\n", filename);
	SWPA_FILE_PRINT("mode=%s\n", mode);


	//����filename�򿪲�ͬ���ļ�	
	if (filename == swpa_strstr(filename, "EEPROM/"))
	{
		return swpa_eeprom_file_open(filename + swpa_strlen("EEPROM/"), mode);
	}
	else if (filename == swpa_strstr(filename, "FLASH/"))
	{
		return swpa_flash_file_open(filename + swpa_strlen("FLASH/"), mode);
	}
	else if (filename == swpa_strstr(filename, "FIFO/"))
	{
		return swpa_fifo_file_open(filename + swpa_strlen("FIFO/"), mode);
	}
	else if (filename == swpa_strstr(filename, "COMM/"))
	{
		return swpa_comm_file_open(filename + swpa_strlen("COMM/"), mode);
	}
	else if (filename == swpa_strstr(filename, "DSPLINK/"))
	{
		return swpa_dsplink_file_open(filename + swpa_strlen("DSPLINK/"), mode);
	}
	else if (filename == swpa_strstr(filename, "BLOCK/"))	//���̿��ļ�������ʡ�Բ�д
	{
		return swpa_normal_file_open(filename + swpa_strlen("BLOCK/"), mode);
	}
	else	//Ĭ�϶������̿��ļ���
	{
		return swpa_normal_file_open(filename, mode);
	}

	return SWPAR_FAIL;
}



/**
* @brief �ر��ļ�
*
* 
* @param [in] fd �ļ�������
* @retval SWPAR_OK : �ɹ�
* @retval SWPAR_FAIL : ʧ��
* @retval SWPAR_NOTIMPL : δʵ��
*/
int swpa_file_close(
	int fd
)
{
	SWPA_FILEHEADER * pfile = (SWPA_FILEHEADER*)fd;

	SWPA_FILE_CHECK(0 != fd);    	// 0 != fd
	
	SWPA_FILE_PRINT("fd=%d\n", fd);
	
	if (0 != pfile->rwlock)
	{
		swpa_mutex_delete(&pfile->rwlock);
		pfile->rwlock = 0;
	}

	return (NULL != pfile->file_close) ? (pfile->file_close(fd)) : SWPAR_NOTIMPL;
}



/**
* @brief �ж��Ƿ����ļ�β��
* @param [in] fd �ļ�������
* @retval 0 : û�е��ļ�β����
* @retval -1 : �����ļ�β��
*/
int swpa_file_eof(
	int fd
)
{
	int ret = 0;

	SWPA_FILEHEADER * pfile = (SWPA_FILEHEADER*)fd;

	SWPA_FILE_CHECK(0 != fd);		// 0 != fd
	
	SWPA_FILE_PRINT("fd=%d\n", fd);

	
	swpa_mutex_lock(&pfile->rwlock, -1);
	
	ret = (NULL != pfile->file_eof) ? (pfile->file_eof(fd)) : SWPAR_NOTIMPL;	
	
	swpa_mutex_unlock(&pfile->rwlock);

	return ret;
}





/**
* @brief �ı��ļ�ָ��
*
* 
* @param [in] fd �ļ�������
* @param [in] offset ָ���ƫ����
* @param [in] pos ָ��ƫ��������ʼλ��
* @retval SWPAR_OK : �ɹ�
* @retval SWPAR_FAIL : ʧ��
* @retval SWPAR_NOTIMPL : δʵ��
*/
int swpa_file_seek(
	int fd, 
	int offset, 
	int pos
)
{
	int ret = 0;

	SWPA_FILEHEADER * pfile = (SWPA_FILEHEADER*)fd;

	SWPA_FILE_CHECK(0 != fd);		// 0 != fd
	
	SWPA_FILE_PRINT("fd=%d\n", fd);
	
	swpa_mutex_lock(&pfile->rwlock, -1);

	ret = (NULL != pfile->file_seek) ? (pfile->file_seek(fd, offset, pos)) : SWPAR_NOTIMPL;

	swpa_mutex_unlock(&pfile->rwlock);

	return ret;
}




/**
* @brief ��õ�ǰ�ļ�ָ��λ��
*
* 
* @param [in] fd �ļ�������
* @retval �ɹ����ص�ǰ�ļ�ָ��λ�ã�
* @retval SWPAR_FAIL : ʧ��
* @retval SWPAR_NOTIMPL : δʵ��
* @attention ��������ķ���ֵ�Ƚ����⣬������Դ�
*/
int swpa_file_tell(
	int fd
)
{
	int ret = 0;
		
	SWPA_FILEHEADER * pfile = (SWPA_FILEHEADER*)fd;

	SWPA_FILE_CHECK(0 != fd);		// 0 != fd

	SWPA_FILE_PRINT("fd=%d\n", fd);
	
	swpa_mutex_lock(&pfile->rwlock, -1);

	ret = (NULL != pfile->file_tell) ? (pfile->file_tell(fd)) : SWPAR_NOTIMPL;

	swpa_mutex_unlock(&pfile->rwlock);

	return ret;
}




/**
* @brief ���ļ��������Ŀ��ƣ����紮�ڲ����ʡ���д�ĳ�ʱʱ�����õȵ�
*
* 
* @param [in] fd �ļ�������
* @param [in] cmd �����ʶ
* @param [in] args �����־����
* @retval SWPAR_OK : �ɹ�
* @retval SWPAR_FAIL : ʧ��
* @retval SWPAR_NOTIMPL : δʵ��
*/
int swpa_file_ioctl(
	int fd, 
	int cmd, 
	void* args
)
{
	int ret = 0;

	SWPA_FILEHEADER * pfile = (SWPA_FILEHEADER*)fd;

	//�������
	SWPA_FILE_CHECK(0 != fd);		// 0 != fd
	//SWPA_FILE_CHECK(0 != cmd);			//Note: cmd����������ֵ����ע�͵�
	//SWPA_FILE_CHECK(NULL != args);		//Note: args������NULL����ע�͵�

	//��ӡ����
	SWPA_FILE_PRINT("fd=%d\n", fd);
	SWPA_FILE_PRINT("cmd=%d\n", cmd);
	SWPA_FILE_PRINT("args=0x%08x\n", (unsigned int)args);

	
	swpa_mutex_lock(&pfile->rwlock, -1);

	ret = (NULL != pfile->file_ioctl) ? (pfile->file_ioctl(fd, cmd, args)) : SWPAR_NOTIMPL;
	
	swpa_mutex_unlock(&pfile->rwlock);

	return ret;
		
}





/**
* @brief ���ļ�����
*
* 
* @param [in] fd �ļ�������
* @param [in] buf �����ݻ�����������ǿ�
* @param [in] size ��������С���������0����λΪ�ֽ�
* @param [out] ret_size ���ض��������ݴ�С���������ĸ���ֵ���ɴ�NULL
* @retval SWPAR_OK : �ɹ�
* @retval SWPAR_FAIL : ʧ��
* @retval SWPAR_NOTIMPL : δʵ��
*/
int swpa_file_read(
	int fd, 
	void* buf, 
	int size, 
	int* ret_size
)
{
	int ret = 0;

	SWPA_FILEHEADER * pfile = (SWPA_FILEHEADER*)fd;

	//�������
	SWPA_FILE_CHECK(0 != fd);		// 0 != fd
	SWPA_FILE_CHECK(NULL != buf);		// NULL != buf
	SWPA_FILE_CHECK(0 < size);		
	//SWPA_FILE_CHECK(NULL != ret_size);	//Note: ret_size������NULL����ע�͵�

	//��ӡ����
	SWPA_FILE_PRINT("fd=%d\n", fd);
	SWPA_FILE_PRINT("buf=0x%08x\n", (unsigned int)buf);
	SWPA_FILE_PRINT("size=%d\n", size);
	SWPA_FILE_PRINT("ret_size=0x%08x\n", (unsigned int)ret_size);

	
	swpa_mutex_lock(&pfile->rwlock, -1);

	ret = (NULL != pfile->file_read) ? (pfile->file_read(fd, buf, size, ret_size)) : SWPAR_NOTIMPL;

	swpa_mutex_unlock(&pfile->rwlock);

	return ret;
}




/**
* @brief д�ļ�����
*
* 
* @param [in] fd �ļ�������
* @param [in] buf д���ݻ�����
* @param [in] size ��������С
* @param [out] ret_size ����д������ݴ�С
* @retval SWPAR_OK : �ɹ�
* @retval SWPAR_FAIL : ʧ��
* @retval SWPAR_NOTIMPL : δʵ��
*/
int swpa_file_write(
	int fd, 
	void* buf, 
	int size, 
	int* ret_size
)
{
	int ret = 0;

	SWPA_FILEHEADER * pfile = (SWPA_FILEHEADER*)fd;

	//�������
	SWPA_FILE_CHECK(0 != fd);		// 0 != fd
	SWPA_FILE_CHECK(NULL != buf);		
	SWPA_FILE_CHECK(0 < size);		
	//SWPA_FILE_CHECK(NULL != ret_size);	//Note: ret_size������NULL����ע�͵�

	//��ӡ����
	SWPA_FILE_PRINT("fd=%d\n", fd);
	SWPA_FILE_PRINT("buf=0x%08x\n", (unsigned int)buf);
	SWPA_FILE_PRINT("size=%d\n", size);
	SWPA_FILE_PRINT("ret_size=0x%08x\n", (unsigned int)ret_size);

	
	swpa_mutex_lock(&pfile->rwlock, -1);

	ret = (NULL != pfile->file_write) ? (pfile->file_write(fd, buf, size, ret_size)) : SWPAR_NOTIMPL;
	
	swpa_mutex_unlock(&pfile->rwlock);

	return ret;
}



