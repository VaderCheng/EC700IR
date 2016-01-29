#pragma once

#define MAX_BIG_COUNT 5
#define MAX_LIST_COUNT 50

// ʶ�����ṹ��
typedef struct _Result
{
	DWORD dwCarID;				    // ����ID
	DWORD64 dwTime;              // ����ץ��ʱ��
	CHAR chPlate[32];                // ���ƺ���
	CHAR chPlateColor[8];         // ������ɫ

	PBYTE pbBinImage;		   // ���ƶ�ֵͼ
	DWORD dwBinSize;         // ���ƶ�ֵͼ��С

	//����Сͼ
	WORD wSmallImgWidth;			//Сͼ���
	WORD wSmallImgHeight;			//Сͼ�߶�
	PBYTE pbSmallImg;				//����JPEGͼ
	DWORD dwSmallImgSize;			//����JPEGͼ�Ĵ�С
	DWORD64 dwSmallImgTime;    //����Сͼʱ��
	
	//������ͼ
	DWORD BigCarID;
	IStream* BigImgStream[MAX_BIG_COUNT];      //������ͼ
	DWORD dwBigImgSize[MAX_BIG_COUNT];		//������ͼ��С
	RECT rcPlatePos[MAX_BIG_COUNT];                 //��������
	DWORD64 dwImageTimeMs[MAX_BIG_COUNT];          // ������ͼʱ��
	int iImageCount;   // ��ͼ����
	
	int iSpeed;
	int iLimitSpeed;
	char chRoadName[128];
	char chRoadDirection[128];
	int iLand;
	char chCarColor[8];
	char chIllegalInfo[24];
	int iIllegalType;
	char chCarType[16];
	char chAppendInfo[2048];
	int iRedTimeMS;
	float fOverSpeedRate;
	
	// ���ʱ��
	int iH;
	int iM;
	int iS;
	int iSS;
	
	_Result()
	{
		dwCarID = 0;
		dwTime = 0;
		pbBinImage = NULL;
		dwBinSize = 0;
		wSmallImgWidth = 0;
		wSmallImgHeight = 0;
		pbSmallImg = NULL;
		dwSmallImgSize = 0;
		BigCarID = 0;
		iImageCount = 0;
		iSpeed = 0;
		iLimitSpeed = 0;
		iLand = 0;
		iIllegalType = 0;
		iRedTimeMS= 0;
		fOverSpeedRate = 0;

		for (int i = 0; i < MAX_BIG_COUNT; i++)
		{
			BigImgStream[i] = NULL; 
			dwBigImgSize[i] = 0;
			dwImageTimeMs[i] = 0;
		}

		ZeroMemory(chAppendInfo, 2048);
	}

	~_Result()
	{
		if (pbBinImage != NULL)
		{
			delete[] pbBinImage;
			pbBinImage = NULL;
		}
		if (pbSmallImg != NULL)
		{
			delete[] pbSmallImg;
			pbSmallImg = NULL;
		}
		for (int i = 0; i < MAX_BIG_COUNT; i++)
		{
			if (BigImgStream[i] != NULL)
			{
				BigImgStream[i]->Release();
				BigImgStream[i] = NULL;
			}
		}
	}
}Result;

class CCamera
{
public:
	CCamera();
	~CCamera(void);

private:
	CLSID m_jpgClsid;
	HANDLE m_hThread;  // ���״̬�߳�
	HANDLE m_hHandleThread; // ��������߳�
	
	Result* m_HvResult;   // �����Ϣ
	SAFE_MODE_INFO m_safeModeInfo;  // ��ʷ�����Ϣ
	int m_iBreakCount;  // ��ʷ���ѭ���ڵ�
	//int m_iDevType;  // �豸����
	int m_iSyncTimeCount; // ͬ��ʱ����
	
    DEV_FILENAME m_FileNameInfo; // �ļ�������Ϣ
	CCommon m_Common; // �ۺϹ���
	
	int m_iLogEnable; // ��־����
	int m_iImgProcessEnable; // ͼ������

	int m_iFontSize; // �����С
	int m_iImgMode; // ͼƬ����ģʽ
	DWORD m_dwFontColor; // ������ɫ
	DWORD m_dwFontEdgeColor; // ���屳������ɫ

	// ͼƬ��С
	int m_iBigImgWidth;
	int m_iBigImgHeight;

	// ѹ��ѡ��
	int m_iCompressEnable;
	int m_iCompressQuality;  // ѹ������
	int m_iCompressSubQuality; // ѹ����������
	int m_iCompressSize;   // ����ѹ���ߴ�

	// �����ͼѡ��
	int m_iFeatureEnable;
	int m_iFeatureWidth;
	int m_iFeatureHeigh;

	char m_chIllegalDir[1024];   // Υ��·��
	char m_chNormalDir[1024]; // ��Υ��·��

	int Connect();
	int DisConnect();
	int SaveResult(Result* pHvResult);
	int ResolveXmlData(LPCSTR szResultInfo);
	int ImgProcess(Result* pHvResult, IStream* pStream, int iImgIndex, CString strText, IStream* pStreamOut);

	float CCamera::GetRedTime(int iRedTime, DWORD64 dwCarTime);

	int InflateCropRect(RECT& rectDst,
							const RECT &cPlatePos,
							const int& iImgWidth,
							const int& iImgHeight,
							const int& iCropWidth,
							const int& iCropHeight
						); // �����ͼλ�ô�С

	int WriteIniFlie();
	int WriteHistoryIniFile();
	int ReadIniFlie();
	int ReadHistoryIniFile();

public:
	HVAPI_HANDLE_EX m_hHv;  // �豸���Ӿ��
	bool m_bExit; // �߳��˳����
	DWORD m_dwConnectStatus;   // �豸����״̬
	CRITICAL_SECTION m_csResult;
	CString m_strDevSN; // �豸���
	CString m_strDevIP; // IP
	CString m_strDir;   // Ŀ¼
	int m_iDevType; // �豸����
	int m_iPort;
	CList<Result*, Result*> m_HvResultList;

	int Open(CString strDevSN, CString strDir);
	int Close();
	DWORD GetStatus();
	int SyncTime();
	int HandleResult();
	int RecoConnect();
	int WriteLog(char* chText);

public:
	// ��������߳�
	static DWORD HandleResultThread(LPVOID param)
	{
		if ( param == NULL)
			return  0;
		CCamera *pThis = (CCamera*)param;
		return pThis->HandleResult();
	}
	// ��ȡ״̬�߳�
	static DWORD RecoThread(LPVOID param)
	{
		if ( param == NULL)
			return  0;
		CCamera *pThis = (CCamera*)param;
		return pThis->RecoConnect();
	}
	// ==================�����ǻص�����========================
	// ʶ������ʼ�ص�����
	static int _cdecl RecordInfoBeginCallBack(PVOID pUserData, DWORD dwCarID)
	{
		if ( pUserData == NULL )
			return 0;

		CCamera* pThis = (CCamera*)pUserData;
		return pThis->RecordInfoBegin(dwCarID);
	}
	int RecordInfoBegin(DWORD dwCarID);

	// ʶ���������ص�����
	static int _cdecl RecordInfoEndCallBack(PVOID pUserData, DWORD dwCarID)
	{
		if ( pUserData == NULL )
			return 0;

		CCamera* pThis = (CCamera*)pUserData;
		return pThis->RecordInfoEnd(dwCarID);
	}
	int RecordInfoEnd(DWORD dwCarID);

	// ������Ϣ�ص�
	static int __cdecl GetPlateCallBack(PVOID pUserData, 
														DWORD dwCarID, 
														LPCSTR pcPlateNo, 
														LPCSTR pcAppendInfo, 
														DWORD dwRecordType,
														DWORD64 dw64TimeMS)
	{
		if (pUserData == NULL)
			return 0;

		CCamera* pThis = (CCamera*)pUserData;
		return pThis->GetPlate(dwCarID, pcPlateNo, pcAppendInfo, dwRecordType, dw64TimeMS);
	}
	int GetPlate(DWORD dwCarID, 
						LPCSTR pcPlateNo, 
						LPCSTR pcAppendInfo, 
						DWORD dwRecordType,
						DWORD64 dw64TimeMS);

	// ��ͼ�ص�
	static int __cdecl GetBigImgCallBack(PVOID pUserData,
							DWORD dwCarID, 
							WORD  wImgType,
							WORD  wWidth,
							WORD  wHeight,
							PBYTE pbPicData,
							DWORD dwImgDataLen,
							DWORD dwRecordType,
							DWORD64 dw64TimeMS)
	{
		if (pUserData == NULL)
			return 0;

		CCamera* pThis = (CCamera*)pUserData;
		return pThis->GetBigImg(dwCarID, wImgType, wWidth, wHeight, pbPicData, dwImgDataLen, dwRecordType, dw64TimeMS);
	}
	int GetBigImg(DWORD dwCarID, 
						WORD  wImgType,
						WORD  wWidth,
						WORD  wHeight,
						PBYTE pbPicData,
						DWORD dwImgDataLen,
						DWORD dwRecordType,
						DWORD64 dw64TimeMS);

	// ����Сͼ�ص�
	static int __cdecl GetSmallImgCallBack(PVOID pUserData,
													  DWORD dwCarID,
													  WORD wWidth,
													  WORD wHeight,
													  PBYTE pbPicData,
													  DWORD dwImgDataLen,
													  DWORD dwRecordType,
													  DWORD64 dw64TimeMS)
	{
		if (pUserData == NULL)
			return 0;

		CCamera* pThis = (CCamera*)pUserData;
		return pThis->GetSmallImg(dwCarID, wWidth, wHeight, pbPicData, dwImgDataLen, dwRecordType, dw64TimeMS);
	}
	int GetSmallImg(DWORD dwCarID,
							WORD wWidth,
							WORD wHeight,
							PBYTE pbPicData,
							DWORD dwImgDataLen,
							DWORD dwRecordType,
							DWORD64 dw64TimeMS);

	// ���ƶ�ֵͼ�ص�
	static int __cdecl GetBinImgCallBack(PVOID pUserData,
													   DWORD dwCarID,
													   WORD wWidth,
													   WORD wHeight,
													   PBYTE pbPicData,
													   DWORD dwImgDataLen,
													   DWORD dwRecordType,
													   DWORD64 dw64TimeMS)
	{
		if (pUserData == NULL)
			return 0;

		CCamera* pThis = (CCamera*)pUserData;
		return pThis->GetBinImg(dwCarID, wWidth, wHeight, pbPicData, dwImgDataLen, dwRecordType, dw64TimeMS);
	}
	int GetBinImg(DWORD dwCarID,
						WORD wWidth,
						WORD wHeight,
						PBYTE pbPicData,
						DWORD dwImgDataLen,
						DWORD dwRecordType,
						DWORD64 dw64TimeMS);

};
