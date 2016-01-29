/**
* @file swpa_normal_file.h 
*
* @brief ��������ͨ�����ļ������Ļ�����������ͷ�ļ������ⷢ������Ϊ�ڲ�ʹ��
* 
* @copyright Signalway All Rights Reserved
* @author zhouy
* @date 2013-02-27
* @version 1.0
*/

#ifndef _SWPA_NORMAL_FILE_H_
#define _SWPA_NORMAL_FILE_H_


#ifdef __cplusplus
extern "C"
{
#endif



/**
* @brief �򿪴����ļ�
* 
* @param [in] filename �ļ���
* @param [in] mode �ļ���ģʽ,�����¼���ģʽ:
* - "r"��"br"  : ����ʽ��
* - "r+"��"br+"  : ��д��ʽ��
* - "w"��"bw"  :   д��ʽ��
* - "w+"��"bw+"  : ��д��ʽ�򿪣�����ļ����������ȴ����ļ�
* - "a"��"ba"  :   ׷�ӷ�ʽ��
* - "a+"��"ba+"  : ׷�ӷ�ʽ�򿪣��ɶ���д
* @retval �ļ�������: �ɹ���(ʵ���Ͼ���SWPA_FILEHEADER�ṹ��ָ��)
* @retval SWPAR_FAIL : ��ʧ��
* @retval SWPAR_INVALIDARG : �����Ƿ�
*/
extern int swpa_normal_file_open(
	const char * filename, 
	const char * mode
);




/**
* @brief �رմ����ļ�
*
* 
* @param [in] fd �ļ�������
* @retval SWPAR_OK : �ɹ�
* @retval SWPAR_FAIL : ʧ��
*/
extern int swpa_normal_file_close(
	int fd
);





/**
* @brief �ж��Ƿ����ļ�β��
* @param [in] fd �ļ�������
* @retval 0 : û�е��ļ�β����
* @retval -1 : �����ļ�β��
* @attention ��������ķ���ֵ�Ƚ����⣬������Դ�
*/
extern int swpa_normal_file_eof(
	int fd
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
extern int swpa_normal_file_seek(
	int fd, 
	int offset, 
	int pos
);





/**
* @brief ��õ�ǰ�ļ�ָ��λ��
*
* 
* @param [in] fd �ļ�������
* @retval �ļ�ָ��λ��: ִ�гɹ�
* @retval SWPAR_FAIL : ʧ��
* @attention ��������ķ���ֵ�Ƚ����⣬������Դ�
*/
extern int swpa_normal_file_tell(
	int fd
);






/**
* @brief ���ļ��������Ŀ���
*
* 
* @param [in] fd �ļ�������
* @param [in] cmd �����ʶ
* @param [in] args �����־����
* @retval SWPAR_OK : �ɹ�
* @retval SWPAR_FAIL : ʧ��
* @retval SWPAR_NOTIMPL : û��ʵ��
*/
extern int swpa_normal_file_ioctl(
	int fd, 
	int cmd, 
	void* args
);






/**
* @brief �������ļ�����
*
* 
* @param [in] fd �ļ�������
* @param [in] buf �����ݻ�����������ǿ�
* @param [in] size ��������С���������0����λΪ�ֽ�
* @param [out] ret_size ���ض��������ݴ�С���������ĸ���ֵ���ɴ�NULL
* @retval SWPAR_OK : �ɹ�
* @retval SWPAR_FAIL : ʧ��
*/
extern int swpa_normal_file_read(
	int fd, 
	void * buf, 
	int size, 
	int * ret_size
);



/**
* @brief д�����ļ�����
*
* 
* @param [in] fd �ļ�������
* @param [in] buf  д���ݻ�����
* @param [in] size ��������С
* @param [out] ret_size ����д������ݴ�С
* @retval SWPAR_OK : �ɹ�
* @retval SWPAR_FAIL : ʧ��
*/
extern int swpa_normal_file_write(
	int fd, 
	void* buf, 
	int size, 
	int* ret_size
);


#ifdef __cplusplus
}
#endif


#endif //_SWPA_NORMAL_FILE_H_

