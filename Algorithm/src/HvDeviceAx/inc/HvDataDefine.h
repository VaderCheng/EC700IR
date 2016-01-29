#ifndef _HVDATADEFINE_H_
#define _HVDATADEFINE_H_
#include "stdafx.h"
//���ݶ���ͷ�ļ�

typedef enum _RESULT_TYPE
{
	IMG_BIG_A = 0,
	IMG_BIG_B,
	IMG_SMALL,
	IMG_BIN,
	TXT_PLATE,
	MAX_RESULT_TYPE
}RESULT_TYPE;

typedef enum _BIG_IMG_TYPE
{
	BEST_SNAP = 0,
	LAST_SNAP,
	BEGIN_CAP,
	BEST_CAP,
	LAST_CAP,
	MAX_BIG_IMG_TYPE
}BIG_IMG_TYPE;

// ���еĽ���ļ�
typedef struct _RESULT_FILE
{
	IStream*		pastmBigImg[MAX_BIG_IMG_TYPE];
	IStream*		pastmSmallImg;
	PBYTE		pbBinImg;
	DWORD32		dwBinImgSize;
	CString		strPlateNo;
	CString		strAppendInfo;

	_RESULT_FILE()
	{
		for (int i = 0; i < MAX_BIG_IMG_TYPE; i++)
		{
			pastmBigImg[i] = NULL;
		}
		pastmSmallImg = NULL;
		pbBinImg = NULL;
	}
}RESULT_FILE;

// ���������Ҫ����Ϣ
typedef struct _RESULT_INFO
{
	DWORD64		dw64Time;
	CString		strNodeName;
	CString		strNodeIP;
	CString		strFolder;
	DWORD32		dwCarID;
	int			rgBigImgSize[MAX_BIG_IMG_TYPE];
	int			iSmallImgSize;

	DWORD32     dwImageID;
	DWORD32     dwTriggerID;
	DWORD32     dwSpeed;
	CString     strSectionName;
	int iVideoID;

	_RESULT_INFO()
	{
		iVideoID = 0;
	}
}RESULT_FILE_INFO;

// ����ṹ��
typedef struct _RESULT_DATA
{
	RESULT_FILE_INFO	resultInfo;
	RESULT_FILE			resultFile;
}RESULT_DATA;

#endif