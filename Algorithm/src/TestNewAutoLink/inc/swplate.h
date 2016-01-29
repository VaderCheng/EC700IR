#ifndef _SWPLATE_INCLUDED__
#define _SWPLATE_INCLUDED__

#include "swBaseType.h"
#include "swWinError.h"
#include "swplatetype.h"
#include <string.h>
#include <stdio.h>

#ifdef __cplusplus

const int g_cnChars = 116;				// �ַ����ϳ���
extern const WORD16 g_wszCharSet[g_cnChars];			// �ַ�����

// �ж��ַ��Ƿ��Ǿ�������
inline bool IsMilitaryChn(BYTE8 nID)
{
	// 103~113 �Ǿ��ƺ���
	if ((nID >= 103) && (nID <= 113))
	{
		return true;
	}
	return false;
}

// �ж��ַ�ʱ��������
inline bool IsDigitChar(BYTE8 nID)
{
	// 1~10������ 
	if ((nID >= 1) && (nID <= 10))
	{
		return true;
	}
	return false;
}

inline int MapChar2Id(const WORD16 ch)
{
	int i;
	for (i=0; i<g_cnChars; i++)
	{
		if(g_wszCharSet[i]==ch)	
		{
			break;
		}
	}
	if(i == g_cnChars)
	{
		i = 0;
	}
	return i;
}

inline WORD16 MapId2Char(int id) 
{
	if (id<g_cnChars)
	{
		return g_wszCharSet[id];
	}
	else
	{
		return g_wszCharSet[0];
	}
}

#endif

extern WORD16 NO_PLATE_UNICODE[];

extern char NO_PLATE[];

//�ڲ�����ʹ��
HRESULT GetPlateNameUnicode(
	WORD16 *pwszPlateName, 
	const PLATE_TYPE nPlateType, 
	const PLATE_COLOR nPlateColor, 
	const BYTE8 * pbContent
	 );

/*����ֲ�ĺ���*/
#ifdef __cplusplus
extern "C"
#endif

int GetPlateNameAlpha(
	char *pszPlateName, 
	const PLATE_TYPE nPlateType, 
	const PLATE_COLOR nPlateColor, 
	const unsigned char* pbContent
	);

#endif		// _SWPLATE_INCLUDED__
