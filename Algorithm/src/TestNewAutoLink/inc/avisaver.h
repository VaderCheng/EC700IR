#ifndef _INCLUDE_AVISAVER_H_
#define _INCLUDE_AVISAVER_H_

#include "resultsend.h"

////////////////////////////////////////////////////////////
//AVI��ȫ����ӿ�(��ά��),ȷ�����ݵ�����
////////////////////////////////////////////////////////////


class CAviSaverData
{
public:
	FILE *pFile;
	//char szFileName[MAX_PATH];
	long nFileLen;
	int nBlocks;				// �ܿ���
	int nCurPos;				// ��ǰ�鷢��λ��
	bool fBegin;				// �Ѿ���ʼ����
	unsigned short m_wYear;					// ��
	unsigned short m_wMon;					// ��
	unsigned short m_wDay;					// ��
	unsigned short m_wHour;					// ʱ
	unsigned short m_wMin;					// ��
	unsigned short m_wSec;					// ��
	unsigned int m_dwAviLen;				// ����
	DWORD64 dwAviTime;						// ʱ�䳤��

	CAviSaverData()
		: pFile(NULL)
	{
		//memset( pAvi, 0, sizeof(HVIO_Video));
	}

	~CAviSaverData()
	{
		//if (pFile != NULL)
		//{
		//	fclose(pFile);
		//	pFile = NULL;
		//}
	}

	CAviSaverData &operator =(const CAviSaverData &data)
	{
		pFile = data.pFile;
		nFileLen = data.nFileLen;

		m_wYear = data.m_wYear;					// ��
		m_wMon = data.m_wMon;					// ��
		m_wDay = data.m_wDay;					// ��
		m_wHour = data.m_wHour;					// ʱ
		m_wMin = data.m_wMin;					// ��
		m_wSec = data.m_wSec;					// ��
		m_dwAviLen = data.m_dwAviLen;			// Aviʱ��
		dwAviTime = data.dwAviTime;

		//if( strlen(data.szFileName) > 0 )
		//{
		//	HV_memcpy(szFileName, data.szFileName, strlen(data.szFileName) + 1);
		//}

		return *this;
	}
};

class IAviSaver
{
public:
	virtual bool Init(const char* pszIniFile) = 0;
	virtual char* GetSavePath(__time64_t lTime, int nLen) = 0;						// ����ʱ��ȡ����·��
	virtual HRESULT GetAviFileByTime(__time64_t lTime, CAviSaverData* pData) = 0;

	virtual HRESULT SetCurWriteFile(char *pszWriteFile) = 0;						// ���õ�ǰ����д���ļ�
	virtual HRESULT ClearCurWriteFile() = 0;										// �����ǰ����д���ļ�

	virtual HRESULT GetDiskStatus() = 0;
	virtual bool ThreadIsOk() = 0;

	//virtual HRESULT Init(LPCSTR lpszIniFile) = 0;
	//virtual HRESULT SaveData(CAviSaverData* pData) = 0;
	//virtual HRESULT GetPlateRecord(DWORD32 dwTimeLow, DWORD32 dwTimeHigh, int& index,  int iType, CAviSaverData* pData) = 0;
	//virtual HRESULT GetDiskStatus() = 0;
	//virtual bool ThreadIsOk() = 0;
};

extern HRESULT GetAviSaver( IAviSaver** ppAviSaver );

#endif

