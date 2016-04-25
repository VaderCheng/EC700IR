/**
* @file swpa_file.h 
* @brief �ļ�����ͷ�ļ�
*
* �������ļ������Ļ�������������Ĳ���������ȥʵ��,ͨ���ļ������жϷ����ĸ��豸\n
*
* @copyright Signalway All Rights Reserved
* @author zhouy
* @date 2013-02-27
* @version 1.0
*/

#ifndef _SWPA_FILE_H_
#define _SWPA_FILE_H_

#include "swpa.h"

#ifdef __cplusplus
extern "C"
{
#endif



enum 
{
	SWPA_SEEK_SET = 0,
	SWPA_SEEK_CUR,
	SWPA_SEEK_END,

	SWPA_SEEK_MAX
};


enum 
{
	SWPA_FILE_SET_READ_TIMEOUT,	    //���ö���ʱʱ��
	SWPA_FILE_SET_WRITE_TIMEOUT,    //����д��ʱʱ��
	SWPA_FILE_IOCTL_COMM_SET_ATTR,  //���ô��ڵ�����
	SWPA_FILE_IOCTL_COMM_IS_RS485,  //���ô���Э��RS232����RS485
	SWPA_FILE_IOCTL_GET_MAX_SIZE,	//����ļ�����С
	SWPA_FILE_IOCTL_TRANCATE,	  	//��ȡ�ļ���ָ����С
	SWPA_FILE_IOCTL_COMM_CLEAR,
	SWPA_FILE_IOCTL_MAX
};




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
extern int swpa_file_open(
	const char * filename, 
	const char * mode
);




/**
* @brief �ر��ļ�
*
* 
* @param [in] fd �ļ�������
* @retval SWPAR_OK : �ɹ�
* @retval SWPAR_FAIL : ʧ��
* @retval SWPAR_NOTIMPL : δʵ��
*/
extern int swpa_file_close(
	int fd
);





/**
* @brief �ж��Ƿ����ļ�β��
* @param [in] fd �ļ�������
* @retval 0 : û�е��ļ�β����
* @retval -1 : �����ļ�β��
* @attention ��������ķ���ֵ�Ƚ����⣬������Դ�
*/
extern int swpa_file_eof(
	int fd
);




/**
* @brief �ı��ļ�ָ��
*
* 
* @param [in] fd �ļ�������
* @param [in] offset ָ���ƫ����
* @param [in] pos ָ��ƫ��������ʼλ�ã�ȡֵ��Χ��: 
* - SWPA_SEEK_SET : �ļ�ͷ
* - SWPA_SEEK_CUR : ��ǰλ��
* - SWPA_SEEK_END : �ļ�β
* @retval SWPAR_OK : �ɹ�
* @retval SWPAR_FAIL : ʧ��
* @retval SWPAR_NOTIMPL : δʵ��
*/
extern int swpa_file_seek(
	int fd, 
	int offset, 
	int pos
);





/**
* @brief ��õ�ǰ�ļ�ָ��λ��
*
* 
* @param [in] fd �ļ�������
* @retval �ļ�ָ��λ��: �ɹ�
* @retval SWPAR_FAIL : ʧ��
* @retval SWPAR_INVALIDARG : �����Ƿ�
* @retval SWPAR_NOTIMPL : δʵ��
* @attention ��������ķ���ֵ�Ƚ����⣬������Դ�
*/
extern int swpa_file_tell(
	int fd
);






/**
* @brief ���ļ��������Ŀ���
*
*
* ���紮�ڲ����ʡ���д�ĳ�ʱʱ�����õȵ�
* 
* @param [in] fd �ļ�������
* @param [in] cmd �����ʶ
* @param [in] args �����־����
* @retval SWPAR_OK : �ɹ�
* @retval SWPAR_FAIL : ʧ��
* @retval SWPAR_NOTIMPL : δʵ��
*/
extern int swpa_file_ioctl(
	int fd, 
	int cmd, 
	void* args
);






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
extern int swpa_file_read(
	int fd, 
	void* buf, 
	int size, 
	int* ret_size
);



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
extern int swpa_file_write(
	int fd, 
	void* buf, 
	int size, 
	int* ret_size
);


#ifdef __cplusplus
}
#endif


#endif //_SWPA_FILE_H_

