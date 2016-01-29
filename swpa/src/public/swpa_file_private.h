/**
* @file swpa_file_private.h 
* @brief �ļ�ģ���ڲ�ͨ�ñ�������
* 
* �������ļ�ģ���ڲ�ʵ����Ҫ�����ͨ����Ϣ
* 
* @copyright Signalway All Rights Reserved
* @author zhouy
* @date 2013-02-27
* @version 1.0
*/

#ifndef _SWPA_FILE_PRIVATE_H_
#define _SWPA_FILE_PRIVATE_H_

#include "swpa_private.h"
#include "swpa_file.h"
#include "swpa_string.h"
#include "swpa_sem.h"

#ifdef __cplusplus
extern "C"
{
#endif


//#define __SWPA_FILE_VERIFY


/**
* @name SWPA_FILEHEADER
* @brief �ļ������ṹ�壬�������豸���������򿪡��رա���д�����Ƶĺ���ָ��
* 
*/
typedef struct _SWPA_FILEHEADER
{
	int device_param; /**< �豸������ */
	int mode; /**< ��дģʽ����0λ����������1λ����д ����3λ����׷��*/
	int rwlock; /**< �ļ���д�� */
	int (*file_open)(const char* filename, const char *mode);/**< ���ļ�����ָ�� */
	int (*file_close)(int fd);/**< �ر��ļ�����ָ�� */
	int (*file_seek)(int fd, int offset, int pos);/**< �ļ���������дλ�ú���ָ�� */
	int (*file_eof)(int fd);/**< �ж��ļ�ָ���Ƿ���β���ĺ���ָ�� */
	int (*file_tell)(int fd);/**< �ļ�ָ������λ�ú���ָ�� */
	int (*file_ioctl)(int fd, int cmd, void* args);/**< �ļ����ƺ���ָ�� */
	int (*file_read)(int fd, void* buf, int size, int* ret_size);/**< ���ļ�����ָ�� */
	int (*file_write)(int fd, void* buf, int size, int* ret_size);	/**< ���ļ�����ָ�� */
}SWPA_FILEHEADER;



#define SWPA_FILE_SET_MODE(pheader, md) (pheader)->mode |= (md)

#define SWPA_FILE_RESET_MODE(pheader, md) (pheader)->mode &= ~(md)



#define SWPA_FILE_READ_MODE 		(0x01<<0)
#define SWPA_FILE_WRITE_MODE 		(0x01<<1)
#define SWPA_FILE_APPEND_MODE 		(0x01<<2)
#define SWPA_FILE_CREATE_MODE 		(0x01<<3)


#define SWPA_FILE_IS_READ_MODE(pheader) 	(pheader->mode & SWPA_FILE_READ_MODE)

#define SWPA_FILE_IS_WRITE_MODE(pheader) 	(pheader->mode & SWPA_FILE_WRITE_MODE)

#define SWPA_FILE_IS_APPEND_MODE(pheader) 	(pheader->mode & SWPA_FILE_APPEND_MODE)

#define SWPA_FILE_IS_CREATE_MODE(pheader) 	(pheader->mode & SWPA_FILE_CREATE_MODE)



#ifdef SWPA_FILE
#define SWPA_FILE_PRINT(fmt, ...) SWPA_PRINT("[%s:%d]"fmt, __FILE__, __LINE__, ##__VA_ARGS__)
#define SWPA_FILE_CHECK(arg)      {if (!(arg)){SWPA_PRINT("[%s:%d]Check failed : %s [%d]\n", __FILE__, __LINE__, #arg, SWPAR_INVALIDARG);return SWPAR_INVALIDARG;}}
#else
#define SWPA_FILE_PRINT(fmt, ...)
#define SWPA_FILE_CHECK(arg)
#endif


#define SWPA_FILE_CHECK_RET(ret, ret_on_err) 							\
if ( SWPAR_OK != (ret) ) 												\
{ 																		\
	SWPA_FILE_PRINT("Err : %s = %d [%d]\n", #ret, ret, ret_on_err); 		\
	return ret_on_err;													\
}	\



/**
* @brief �����ļ���д��
*
* 
* @param [inout] pheader �ļ�ͷ�ṹ��ָ��
* @param [in] filename  �ļ���
* @retval SWPAR_OK : �ɹ�
* @retval SWPAR_INVALIDARG : �����Ƿ�
* @retval SWPAR_FAIL : ʧ��
*/
int swpa_file_create_rwlock(
	SWPA_FILEHEADER * pheader, 
	const char * filename
);



/**
* @brief �����������ļ�����ģʽ
*
* 
* @param [inout] pheader �ļ�ͷ�ṹ��ָ��
* @param [in] mode ����ģʽ�ַ���
* @retval SWPAR_OK : �ɹ�
* @retval SWPAR_INVALIDARG : �����Ƿ�
* @retval SWPAR_FAIL : ʧ��
*/
extern int swpa_file_parse_mode(
	SWPA_FILEHEADER * pheader, 
	const char * mode
);


#ifdef __cplusplus
}
#endif


#endif //_SWPA_FILE_PRIVATE_H_


