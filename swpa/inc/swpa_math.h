/**
* @file swpa_string.h 
* @brief �����ַ���������غ���
*
* @copyright Signalway All Rights Reserved
* @author zhouy
* @date 2013-02-27
* @version 1.0
*/

#ifndef _SWPA_MATH_H_
#define _SWPA_MATH_H_

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#ifdef __cplusplus
extern "C"
{
#endif


///�����͵ľ���ֵ
#define swpa_abs		abs


///��ʵ�͵ľ���ֵ
#define swpa_fabs	  fabs


///ȡ����
#define swpa_floor		floor


///ȡ����
#define swpa_ceil		ceil


///������eΪ�׵Ķ���
#define swpa_log 	log 


///������10Ϊ�׵Ķ���
#define swpa_log10		log10


///������
#define swpa_pow		pow


///��ƽ��
#define swpa_sqrt		sqrt

#define swpa_exp    exp

///���������Сֵ��
#define swpa_min(a, b) ((a) < (b) ? (a) : (b))
#define swpa_max(a, b) ((a) > (b) ? (a) : (b))

#ifdef __cplusplus
}
#endif


#endif //_SWPA_MATH_H_


