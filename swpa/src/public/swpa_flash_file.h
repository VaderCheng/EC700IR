/**
* @file swpa_flash_file.h 
* @brief ��������FLASH����غ�������ͷ�ļ������ⷢ������Ϊ�ڲ�ʹ��
*
* 
* @copyright Signalway All Rights Reserved
* @author zhouy
* @date 2013-03-11
* @version 1.0
*/


#ifndef _SWPA_FLASH_FILE_H_
#define _SWPA_FLASH_FILE_H_


#ifdef __cplusplus
extern "C"
{
#endif




/**
* @brief ��FLASH�ļ�,��Ҫ�ǳ�ʼ��SWPA_FILEHEADER�ṹ��
*
* 
* @param [in] filename �ļ���
* @param [in] mode �ļ���ģʽ
* - "r"  ����ʽ��
* - "w"  д��ʽ��
* - "r+"  ��д��ʽ��
* - "w+"  ��д��ʽ��
* @retval �ļ�������(int��) :  �ɹ���ʵ���Ͼ���SWPA_FILEHEADER�ṹ��ָ��
* @retval SWPAR_FAIL : ��ʧ��
* @retval SWPAR_INVALIDARG : �����Ƿ�
* @retval SWPAR_OUTOFMEMORY : �����ڴ治��
*/
extern int swpa_flash_file_open(
	const char *filename, 
	const char *mode
);



/**
* @brief �ر�flash�豸�ļ�
*
* 
* @param [in] fd �ļ�������
* @retval SWPAR_OK :  �ɹ�
* @retval SWPAR_FAIL :  ʧ��
*/
extern int swpa_flash_file_close(
	int fd
);




/**
* @brief ��flash�ļ�����
*
* 
* @param [in] fd �ļ�������
* @param [in] buf �����ݻ�����
* @param [in] size ��������С
* @param [out] ret_size ���ض��������ݴ�С
* @retval SWPAR_OK :  �ɹ�
* @retval SWPAR_FAIL :  ʧ��
*/
extern int swpa_flash_file_read(
	int fd, 
	void* buf, 
	int size, 
	int* ret_size
);



/**
* @brief дflash�ļ�����
*
* 
* @param [in] fd �ļ�������
* @param [in] buf д���ݻ�����
* @param [in] size ��������С
* @param [out] ret_size ����д������ݴ�С
* @retval SWPAR_OK :  �ɹ�
* @retval SWPAR_FAIL :  ʧ��
*/
extern int swpa_flash_file_write(
	int fd, 
	void* buf, 
	int size, 
	int* ret_size
);



/**
* @brief �ı��ļ�ָ��
*
* 
* @param [in] fd �ļ�������
* @param [in] offset ָ���ƫ����
* @param [in] pos ָ��ƫ��������ʼλ�ã�ȡֵ��Χ��: 
* - �ļ�ͷ0(SWPA_SEEK_SET)
* - ��ǰλ��1(SWPA_SEEK_CUR)
* - �ļ�β2(SWPA_SEEK_END)
* @retval SWPAR_OK : �ɹ�
* @retval SWPAR_FAIL : ʧ��
*/
extern int swpa_flash_file_seek(
	int fd, 
	int offset, 
	int pos
);



/**
* @brief �ж��Ƿ����ļ�β��
* @param [in] fd �ļ�������
* @retval 0 : û�е��ļ�β����
* @retval -1 : �����ļ�β��
* @attention ��������ķ���ֵ�Ƚ����⣬������Դ�
*/
int swpa_flash_file_eof(
	int fd
);






/**
* @brief ��õ�ǰ�ļ�ָ��λ��
*
* 
* @param [in] fd �ļ�������
* @retval �ļ�ָ��λ��: �ɹ�
* @retval SWPAR_INVALIDARG : �����Ƿ�
* @retval SWPAR_FAIL : ʧ��
* @attention ��������ķ���ֵ�Ƚ����⣬������Դ�
*/
extern int swpa_flash_file_tell(
	int fd
);



/**
* @brief ���ļ��������Ŀ���
*
*
* Ŀǰ��֧�ֻ�ȡ�ļ����size
* 
* @param [in] fd �ļ�������
* @param [in] cmd �����ʶ��Ŀǰ�Ѷ����������
* - SWPA_FILE_IOCTL_FLASH_GET_MAX_SIZE : ��ȡ�ļ����size
* @param [in] args �����־����
* @retval SWPAR_OK : �ɹ�
* @retval SWPAR_FAIL : ʧ��
*/
int swpa_flash_file_ioctl(
	int fd, 
	int cmd, 
	void* args
);



#ifdef __cplusplus
}
#endif

#endif //_SWPA_FLASH_FILE_H_

