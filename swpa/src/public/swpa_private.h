/**
* @file swpa_private.h 
* @brief PlatformAPI��˽�е�ͨ�����ݺͺ�������
* 
* - ������PlatformAPI�㹲�õ�debug��:__SWPA_DEBUG
* - ������PlatformAPI�㹲�õ���־��ӡ�ӿ�:SWPA_PRINT()
* 
* @note ���ļ�ֻ��PlatformAPI���ڲ�ʹ�ã������⹫��!
* @copyright Signalway All Rights Reserved
* @author zhouy
* @date 2013-02-27
* @version 1.0
*/


#ifndef _SWPA_PRIVATE_H_
#define _SWPA_PRIVATE_H_ 

#include "swpa.h"
#include <stdio.h>

//�����ӡ����
#define SWPA_PRINT(fmt, ...) printf(fmt, ##__VA_ARGS__)

#endif //_SWPA_PRIVATE_H_


