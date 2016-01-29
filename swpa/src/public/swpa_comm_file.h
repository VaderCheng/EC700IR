/**
* @file swpa_comm_file.h 
* @brief ��������COMM����غ�������ͷ�ļ������ⷢ������Ϊ�ڲ�ʹ��
* 
* @copyright Signalway All Rights Reserved
* @author zhouy
* @date 2013-03-11
* @version 1.0
*/


#ifndef _SWPA_COMM_FILE_H_
#define _SWPA_COMM_FILE_H_


#ifdef __cplusplus
extern "C"
{
#endif




/**
* @brief ��comm�ļ�,��Ҫ�ǳ�ʼ��SWPA_FILEHEADER�ṹ��
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
extern int swpa_comm_file_open(
	const char * filename, 
	const char * mode
);



/**
* @brief �ر�comm�豸�ļ�
*
* 
* @param [in] fd �ļ�������
* @retval SWPAR_OK :  �ɹ�
* @retval SWPAR_FAIL :  ʧ��
*/
extern int swpa_comm_file_close(
	int fd
);





/**
* @brief ��COMM�ļ�����
*
* 
* @param [in] fd �ļ�������
* @param [in] buf �����ݻ�����������ǿ�
* @param [in] size ��������С���������0����λΪ�ֽ�
* @param [out] ret_size ���ض��������ݴ�С���������ĸ���ֵ���ɴ�NULL
* @retval SWPAR_OK : �ɹ�
* @retval SWPAR_FAIL : ʧ��
*/
int swpa_comm_file_read(
	int fd, 
	void* buf, 
	int size, 
	int* ret_size
);



/**
* @brief дcomm�ļ�����
*
* 
* @param [in] fd �ļ�������
* @param [in] buf д���ݻ�����
* @param [in] size ��������С
* @param [out] ret_size ����д������ݴ�С
* @retval SWPAR_OK :  �ɹ�
* @retval SWPAR_FAIL :  ʧ��
*/
extern int swpa_comm_file_write(
	int fd, 
	void* buf, 
	int size, 
	int* ret_size
);




/**
* @brief ���ļ��������Ŀ���
*
*
* Ŀǰ��֧��д�ĳ�ʱʱ������
* 
* @param [in] fd �ļ�������
* @param [in] cmd �����ʶ��Ŀǰ�Ѷ����������
* - SWPA_FILE_IOCTL_COMM_SET_TIMEOUT : д�ĳ�ʱʱ������
* @param [in] args �����־����
* @retval SWPAR_OK : �ɹ�
* @retval SWPAR_FAIL : ʧ��
*/
extern int swpa_comm_file_ioctl(
	int fd, 
	int cmd, 
	void* args
);




#ifdef __cplusplus
}
#endif

#endif //_SWPA_COMM_FILE_H_



