/**
* @file swpa_dsplink_file.h 
* @brief ��������DSPLINK����غ�������ͷ�ļ������ⷢ������Ϊ�ڲ�ʹ��
*
* 
* @copyright Signalway All Rights Reserved
* @author zhouy
* @date 2013-03-11
* @version 1.0
*/


#ifndef _SWPA_DSPLINK_FILE_H_
#define _SWPA_DSPLINK_FILE_H_


#ifdef __cplusplus
extern "C"
{
#endif





/**
* @brief ��dsplink�ļ�,��Ҫ�ǳ�ʼ��SWPA_FILEHEADER�ṹ��
*
* 
* @param [in] filename �ļ���
* @param [in] mode �ļ���ģʽ
* - "w"  :д��ʽ��
* @retval �ļ�������(int��) :  �ɹ���ʵ���Ͼ���SWPA_FILEHEADER�ṹ��ָ��
* @retval SWPAR_FAIL : ��ʧ��
* @retval SWPAR_INVALIDARG : �����Ƿ�
* @retval SWPAR_OUTOFMEMORY : �����ڴ治��
*/
extern int swpa_dsplink_file_open(
	const char * filename, 
	const char * mode
);



/**
* @brief �ر�dsplink�豸�ļ�
*
* 
* @param [in] fd �ļ�������
* @retval SWPAR_OK :  �ɹ�
* @retval SWPAR_FAIL :  ʧ��
*/
extern int swpa_dsplink_file_close(
	int fd
);





/**
* @brief дdsplink�ļ�����
*
* 
* @param [in] fd �ļ�������
* @param [in] buf д���ݻ�����
* @param [in] size ��������С
* @param [out] ret_size ����д������ݴ�С
* @retval SWPAR_OK :  �ɹ�
* @retval SWPAR_FAIL :  ʧ��
*/
extern int swpa_dsplink_file_write(
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
* - SWPA_FILE_IOCTL_DSPLINK_SET_TIMEOUT : д�ĳ�ʱʱ������
* @param [in] args �����־����
* @retval SWPAR_OK : �ɹ�
* @retval SWPAR_FAIL : ʧ��
*/
extern int swpa_dsplink_file_ioctl(
	int fd, 
	int cmd, 
	void* args
);




#ifdef __cplusplus
}
#endif

#endif //_SWPA_DSPLINK_FILE_H_


