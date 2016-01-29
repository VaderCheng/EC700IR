//���ߺ���ͷ�ļ�
#ifndef _TOOLSFUNCTION_H_
#define _TOOLSFUNCTION_H_
#include "stdafx.h"
#include "tinyxml\tinyxml.h"
#include "atltime.h"

const int BIN_WIDTH = 112;
const int BIN_HEIGHT = 20;
const int BIN_BIT_COUNT = 8;

//��ȡ���Ƹ�����Ϣ
HRESULT GetPlateNumByXmlExtInfo(const char* szXmlExtInfo, char* szPlateNum, DWORD32* pdwTimeLow, DWORD32* pdwTimeHigh, DWORD32* pdwCarID, char* IllegalInfo);

//�����ͼ
HRESULT SaveStreamFile(IStream* pStmData, int iSize, const char* pszFilePath);

inline int SetInt32ToByteArray(BYTE *pbDest, INT32 i32Src)
{
	memcpy( pbDest, &i32Src, sizeof( i32Src ) );
	return sizeof( i32Src );
}

inline int SetInt16ToByteArray(BYTE *pbDest, SHORT i16Src)
{
	memcpy( pbDest, &i16Src, sizeof( i16Src ) );
	return sizeof( i16Src );
}

//��ֵ������ת��ΪBMPͼƬ
BOOL BinImage2BMP(int iByteBinImageLen, BYTE* pByteBinImage, const char * pchBMPFileName);
//����ʱ���ж�һ�δ���ʣ������������200M���򷵻�true
BOOL DiskStatus(LPCTSTR filename);

#endif