/*
 *	(C)��Ȩ���� 2010 ������·���Ƽ���չ���޹�˾
 */

/**
* @file		HvTime.h
* @version	1.0
* @brief	�й�ʱ��Ĳ�������
* @author	Shaorg
* @date		2010-8-20
*/

#ifndef _HVTIME_H_
#define _HVTIME_H_

#include "hvutils.h"

//��ͨ���ÿ�µ�����
const BYTE8 baMDays[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

//�������ÿ�µ�����
const BYTE8 baLMDays[12] = { 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

typedef struct _REAL_TIME_STRUCT
{
    WORD16				wYear;			//��.
    WORD16				wMonth;			//��.
    WORD16				wDay;			//��.
    WORD16				wWeekNum;		//��ǰ���ڵ�������. /* 0Ϊ���գ�1Ϊ��һ���Դ�����*/
    WORD16				wHour;			//ʱ, 24Сʱ��.
    WORD16				wMinute;		//��.
    WORD16				wSecond;		//��.
    WORD16				wMSecond;		//����.
} REAL_TIME_STRUCT;

#define ConvertTickToSystemTime(tick, stlow, sthigh) \
{ \
	int iTickDiff = GetSystemTick() - (tick); \
	if (iTickDiff < 0) \
	{\
		iTickDiff = 0;\
	}\
	DWORD32 dwTickDiff = iTickDiff; \
	GetSystemTime(&(stlow), &(sthigh)); \
	if (dwTickDiff <= (stlow)) \
	{ \
		(stlow) -= dwTickDiff; \
	} \
	else \
	{ \
		(sthigh)--; \
		(stlow) += (0xFFFFFFFF - dwTickDiff) ; \
	} \
}

int ChangeSystemTime(
    DWORD32 dwTimeMsLow,
    DWORD32 dwTimeMsHigh
);

int SetSystemTime(
    DWORD32 dwTimeMsLow,
    DWORD32 dwTimeMsHigh
);

int SetSystemTime(
    REAL_TIME_STRUCT tTime
);

int GetSystemTime(
    DWORD32* pdwTimeMsLow,
    DWORD32* pdwTimeMsHigh
);

int GetNowDateTime(
    int* piYear,
    int* piMon,
    int* piDay,
    int* piHour,
    int* piMin,
    int* piSec
);

void ConvertMsToTime(
    DWORD32 dwMSCountLow,
    DWORD32 dwMSCountHigh,
    REAL_TIME_STRUCT *pRealTime
);

void ConvertTimeToMs(
    const REAL_TIME_STRUCT *pRealTime,
    DWORD32 *pdwMSCountLow,
    DWORD32 *pdwMSCountHigh
);

#endif

