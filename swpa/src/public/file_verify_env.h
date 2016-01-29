/**
* @file file_verify_env.h 
* @brief file ��ʱ��֤��ͷ�ļ�
*
*  �������ļ������Ļ�������������Ĳ���������ȥʵ��,ͨ���ļ������жϷ����ĸ��豸\n

* @copyright Signalway All Rights Reserved
* @author zhouy
* @date 2013-03-11
* @version 1.0
*/


#ifndef _SWPA_FILE_VERIFY_ENV_H_
#define _SWPA_FILE_VERIFY_ENV_H_

#include <memory.h>
#include <stdlib.h>
#include <string.h>


#ifdef __cplusplus
extern "C"
{
#endif

//memory
//#define swpa_mem_alloc 	malloc
//#define swpa_mem_free	free


//string
#define swpa_strcmp		strcmp
#define swpa_strncmp	strncmp
#define swpa_strstr		strstr
#define swpa_strcpy		strcpy
#define swpa_strlen		strlen
#define swpa_strcat		strcat
#define swpa_strncpy	strncpy


//io
#define swpa_sscanf		sscanf
#define swpa_sprintf	sprintf
#define swpa_snprintf	snprintf




#ifdef __cplusplus
}
#endif


#endif //_SWPA_FILE_VERIFY_ENV_H_



