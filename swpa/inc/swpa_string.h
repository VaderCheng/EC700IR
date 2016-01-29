/**
* @file swpa_string.h 
* @brief �����ַ���������غ���
*
* @copyright Signalway All Rights Reserved
* @author zhouy
* @date 2013-02-27
* @version 1.0
*/

#ifndef _SWPA_STRING_H_
#define _SWPA_STRING_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#ifdef __cplusplus
extern "C"
{
#endif


///�ַ����ȽϺ���
#define swpa_strcmp		strcmp


///�޶����ȵ��ַ����ȽϺ���
#define swpa_strncmp	strncmp


///�����ַ��Ӵ�
#define swpa_strstr		strstr


///�ַ�������
#define swpa_strcpy		strcpy


///�����ַ�������
#define swpa_strncpy	strncpy


///��ȡ�ַ�������
#define swpa_strlen		strlen


///�ַ���ƴ��
#define swpa_strcat		strcat


///��һ�����в��Ҹ����ַ��ĵ�һ��ƥ��֮��
#define swpa_strchr		strchr


///��һ�����е�һ��������һ�����Ƚ�, ���ܴ�Сд
#define swpa_strnicmp	strncasecmp


///�Դ�Сд�����з�ʽ�Ƚ�������
#define swpa_stricmp	strcasecmp


///��һ�����е������ַ�����Ϊָ���ַ�
#define swpa_strset		strset


///��һ�����е�ǰn���ַ�����Ϊָ���ַ�
#define swpa_strnset	strnset




///���ַ�����ȡ����
#define swpa_sscanf		sscanf


///��ӡ���ݵ��ַ���
#define swpa_sprintf	sprintf
#define swpa_vsprintf vsprintf


///��ӡ���ݵ������ַ���
#define swpa_snprintf	snprintf




///�ַ���ת����
#define swpa_atoi	atoi


///�ַ���ת������
#define swpa_atol	atol


///�ַ���ת������
#define swpa_atof	atof

#define swpa_va_list  va_list
#define swpa_va_start va_start
#define swpa_va_end   va_end
#define swpa_va_arg   va_arg

/* BEGIN: Modified by huangdch, 2015/8/20 
*�ַ�������*/
#define swpa_strtok_r  strtok_r

#ifdef __cplusplus
}
#endif


#endif //_SWPA_STRING_H_


