#include "StdAfx.h"
#include "Camera.h"

extern CRITICAL_SECTION g_csLog;
extern CRITICAL_SECTION g_csOperation;
void WriteFileName(const char* chFileName);

DWORD WINAPI RecoThread(LPVOID lpParam);

CCamera::CCamera()
{
	m_hThread = NULL;
	m_hHandleThread = NULL;

	m_HvResult = NULL;
	m_iLogEnable = 0;
	m_iImgProcessEnable = 1;

	m_iFontSize = 25;
	m_iImgMode = 0;
	m_dwFontColor = 0;
	m_dwFontEdgeColor = 0;
	m_iBigImgWidth = 1600;
	m_iBigImgHeight = 1200;
	m_iCompressEnable = 0;
	m_iCompressQuality = 100;
	m_iCompressSubQuality = 5;
	m_iCompressSize = 300 * 1024;
	m_iFeatureEnable = 1;
	m_iFeatureWidth = 400;
	m_iFeatureHeigh = 300;

	m_iSyncTimeCount = 0;

	m_hHv = NULL;
	m_dwConnectStatus = 0;
	m_bExit = true;
	m_iDevType = 0;
	m_iPort = 0;

	InitializeCriticalSection( &m_csResult );
	
	m_Common.GetEncoderClsid(L"image/jpeg", &m_jpgClsid);
}

CCamera::~CCamera(void)
{
	DeleteCriticalSection( &m_csResult );
}

// ����ӿ�::��
int CCamera::Open(CString strDevSN, CString strDir)
{
	m_strDevSN = strDevSN;
	m_strDir = strDir;

	ReadIniFlie();
	if (-1 == Connect())
		return -1;

	m_bExit = false;
	// ״̬����߳�
	m_hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)RecoThread, this, 0, NULL);
	// ��������߳�
	m_hHandleThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)HandleResultThread, this, 0, NULL);
	return 0;
}

// ����ӿ�::�ر�
int CCamera::Close()
{
	m_bExit = true;
	if (WaitForSingleObject(m_hThread, 2000) == WAIT_OBJECT_0)
	{
	}
	if (m_hThread != NULL)
	{
		CloseHandle(m_hThread);
		m_hThread = NULL;
	}
	DisConnect();
	WriteIniFlie();
	if (WaitForSingleObject(m_hHandleThread, 2000) == WAIT_OBJECT_0)
	{
	}
	if (m_hHandleThread != NULL)
	{
		CloseHandle(m_hHandleThread);
		m_hHandleThread = NULL;
	}
	return 0;
}

// ����ӿ�::ʱ��ͬ��
int CCamera::SyncTime()
{
	if (300 == m_iSyncTimeCount || 0 == m_iSyncTimeCount)
	{
		time_t t = time(NULL);
		struct tm *Tm = localtime(&t);
		char chTemp[1024] = {0};
		sprintf(chTemp, "SetTime,Date[%d.%02d.%02d],Time[%02d:%02d:%02d]", Tm->tm_year+1900, Tm->tm_mon+1, Tm->tm_mday, Tm->tm_hour, Tm->tm_min, Tm->tm_sec);

		char szRetBuf[1024] = {0};
		int nRetLen = 0;
		if (m_hHv != NULL)
		{
			if (HVAPI_ExecCmdEx(m_hHv, chTemp, szRetBuf, 1024, &nRetLen) != S_OK)
				return -1;
		}
		
		m_iSyncTimeCount = 1;
	}

	m_iSyncTimeCount++;
	if (m_iSyncTimeCount > 300)
	{	
		m_iSyncTimeCount = 1;
	}

	return 0;
}

// ����ӿ�::��ȡ״̬
DWORD CCamera::GetStatus()
{
	return m_dwConnectStatus;
}

// �豸����
int CCamera::Connect()
{
	if (m_strDevSN.IsEmpty())
	{
		return -1;
	}

	DisConnect();

	// �����豸
	m_hHv = HVAPI_OpenAutoLink(m_strDevSN, NULL);
	if (NULL == m_hHv)
		return -1;

	// ��ȡ�豸IP
	char chDevSN[256] = {0};
	sprintf(chDevSN, "%s", m_strDevSN.GetBuffer(m_strDevSN.GetLength()));
	m_strDevSN.ReleaseBuffer();
	HVAPI_GetDeviceInfoAutoLink(chDevSN, NULL, m_strDevIP.GetBuffer(64), 64, m_iPort);
	m_strDevIP.ReleaseBuffer();

	// ��ȡģʽ
	PROTOCOL_VERSION emProtocolVersion;
	HVAPI_GetXmlVersionEx(m_hHv, &emProtocolVersion);

	char szRetBuf[512] = {0};
	int nRetLen = 0;
	if ( HVAPI_ExecCmdEx(m_hHv, "OptWorkMode", szRetBuf, 512, &nRetLen) == S_OK)
	{
		char szInfoValue[256] = {0};
		if ( HVAPIUTILS_GetExeCmdRetInfoEx(true, szRetBuf, "OptWorkMode", "WorkMode", szInfoValue) == S_OK )
			m_iDevType = atoi(szInfoValue);

		char cmd[256] = {0};
		sprintf(cmd, "GetWorkModeIndex,WorkModeIndex[%d]", m_iDevType);
		nRetLen = 0;
		memset(szRetBuf, 0, 512);
		memset(szInfoValue, 0, 256);
		if ( HVAPI_ExecCmdEx(m_hHv, cmd, szRetBuf, 512, &nRetLen) == S_OK)
		{
			HVAPIUTILS_GetExeCmdRetInfoEx( (emProtocolVersion == PROTOCOL_VERSION_1 ? false : true ), szRetBuf, "GetWorkModeIndex", "WorkModeName", szInfoValue);
			if ( strstr(szInfoValue, "���Ӿ���") != NULL )
			{	
				WriteLog("���Ӿ���");
				m_iDevType = 1;
			}
			else
			{
				WriteLog("����");
				m_iDevType = 0;
			}
		}
	}
	
	char szCommand[512];
	sprintf(szCommand, "DownloadRecord,Enable[%d],BeginTime[%s],Index[%d],EndTime[%s],DataInfo[%d]",
		m_safeModeInfo.iEanbleSafeMode, m_safeModeInfo.szBeginTime, m_safeModeInfo.index, m_safeModeInfo.szEndTime, m_safeModeInfo.DataInfo);
	WriteLog(szCommand);

	if ((HVAPI_SetCallBackEx(m_hHv, RecordInfoBeginCallBack, this, 0, CALLBACK_TYPE_RECORD_INFOBEGIN, NULL) != S_OK)
		|| (HVAPI_SetCallBackEx(m_hHv, RecordInfoEndCallBack, this, 0, CALLBACK_TYPE_RECORD_INFOEND, NULL) != S_OK)
		|| (HVAPI_SetCallBackEx(m_hHv, GetBigImgCallBack, this, 0, CALLBACK_TYPE_RECORD_BIGIMAGE, NULL) != S_OK)
		|| (HVAPI_SetCallBackEx(m_hHv, GetSmallImgCallBack, this, 0, CALLBACK_TYPE_RECORD_SMALLIMAGE, NULL) != S_OK)
		|| (HVAPI_SetCallBackEx(m_hHv, GetBinImgCallBack, this, 0, CALLBACK_TYPE_RECORD_BINARYIMAGE, NULL) != S_OK)
		|| (HVAPI_SetCallBackEx(m_hHv, GetPlateCallBack, this, 0, CALLBACK_TYPE_RECORD_PLATE, szCommand) != S_OK)
		)
	{
		WriteLog("���ûص�ʧ�ܣ�\n");
		DisConnect();
		return -1;
	}
	WriteLog("���ӳɹ���");
	
	return 0;
}

int CCamera::DisConnect()
{
	if (NULL == m_hHv)
		return -1;

	Result* pHvResult = NULL;
	EnterCriticalSection(&m_csResult);
	while(true)
	{
		if (!m_HvResultList.IsEmpty())
		{
			pHvResult = m_HvResultList.RemoveHead();
			if (pHvResult != NULL)
			{
				delete pHvResult;
				pHvResult = NULL;
			}
		}

		if (m_HvResultList.IsEmpty())
			break;
	}
	LeaveCriticalSection(&m_csResult);

	HVAPI_CloseEx(m_hHv);
	m_hHv = NULL;
	WriteLog("�Ͽ��ɹ���");
	return 0;
}

// ״̬����߳�
int CCamera::RecoConnect()
{
	// ���������޷�ʹ�ñ����������ӿ�
	//DWORD dwStatus = -1;
	//while(!m_bExit)
	//{
	//	if ( HVAPI_GetConnStatusEx(m_hHv, CONN_TYPE_RECORD, &dwStatus) == S_OK )
	//	{
	//		if (dwStatus == CONN_STATUS_NORMAL 
	//			|| dwStatus == CONN_STATUS_RECVDONE)
	//		{
	//			SyncTime(); // ʱ��ͬ��
	//			m_dwConnectStatus = dwStatus;
	//		}
	//		else if (dwStatus == CONN_STATUS_RECONN)
	//		{
	//			m_dwConnectStatus = dwStatus;
	//		}
	//		else
	//		{
	//			m_dwConnectStatus = dwStatus;
	//			//Connect();
	//		}
	//	}
	//	else
	//	{
	//		m_dwConnectStatus = CONN_STATUS_UNKNOWN;
	//		WriteLog("��ȡ״̬ʧ�ܣ�");
	//		//Connect();
	//	}

	//	Sleep(1000);
	//}
	return 0;
}

// ��������߳�
int CCamera::HandleResult()
{
	Result* pHvResult = NULL;
	while(!m_bExit)
	{
		EnterCriticalSection(&m_csResult);
		if (!m_HvResultList.IsEmpty())
		{
			pHvResult = m_HvResultList.RemoveHead();
		}
		else
		{
			Sleep(500);
		}
		LeaveCriticalSection(&m_csResult);
		
		if (pHvResult == NULL)
			continue;

		SaveResult(pHvResult);
		if (pHvResult != NULL)
		{
			delete pHvResult;
			pHvResult = NULL;
		}
	}
	return 0;
}

// ��ȡini�ļ�
int CCamera::ReadIniFlie()
{
	TCHAR fileName[MAX_PATH];
	GetModuleFileName(NULL, fileName, MAX_PATH-1);
	PathRemoveFileSpec(fileName);
	TCHAR chIniFileName[MAX_PATH] = { 0 };
	strcpy(chIniFileName, fileName);
	strcat(chIniFileName, _T("\\Config.ini"));

	ReadHistoryIniFile();

	EnterCriticalSection(&g_csLog);

	// ����·��
	GetPrivateProfileString("DIR_SET", "IllegalDir", "$(DeviceIP)\\$(IllegalInfo)Υ��\\$(StreetName)\\$(Year)$(Month)$(Day)$(Hour)$(Minute)$(Second)\\", m_chIllegalDir, 1024, chIniFileName);
	GetPrivateProfileString("DIR_SET", "NormalDir", "$(DeviceIP)\\����\\$(StreetName)\\$(Year)$(Month)$(Day)\\$(Hour)\\", m_chNormalDir, 1024, chIniFileName);

	// �ļ�����
	m_FileNameInfo.iBigImgEnable = GetPrivateProfileInt("FILENAME_SET", "BigImgEnable", 1, chIniFileName);
	m_FileNameInfo.iSmallImgEnable = GetPrivateProfileInt("FILENAME_SET", "SmallImgEnable", 1, chIniFileName);
	m_FileNameInfo.iBinEnable = GetPrivateProfileInt("FILENAME_SET", "BinEnable", 1, chIniFileName);
	m_FileNameInfo.iInfoEnable = GetPrivateProfileInt("FILENAME_SET", "InfoEnable", 1, chIniFileName);
	m_FileNameInfo.iPlateEnable = GetPrivateProfileInt("FILENAME_SET", "PlateEnable", 1, chIniFileName);
	m_FileNameInfo.iPlateNoEnable = GetPrivateProfileInt("FILENAME_SET", "PlateNoEnable", 0, chIniFileName);

	GetPrivateProfileString("FILENAME_SET", "BigImgFileName", "$(Year)$(Month)$(Day)$(Hour)$(Minute)$(Second)$(MilliSecond)_$(BigImageID_Letter).jpg", m_FileNameInfo.chBigImgFileName, 1024, chIniFileName);
	GetPrivateProfileString("FILENAME_SET", "SmallImgName", "$(Year)$(Month)$(Day)$(Hour)$(Minute)$(Second)$(MilliSecond).bmp", m_FileNameInfo.chSmallImgFileName, 1024, chIniFileName);
	GetPrivateProfileString("FILENAME_SET", "BinFileName", "$(Year)$(Month)$(Day)$(Hour)$(Minute)$(Second)$(MilliSecond).bin", m_FileNameInfo.chBinFileName, 1024, chIniFileName);
	GetPrivateProfileString("FILENAME_SET", "InfoFileName", "$(Year)$(Month)$(Day)$(Hour)$(Minute)$(Second)$(MilliSecond).inf", m_FileNameInfo.chInfoFileName, 1024, chIniFileName);
	GetPrivateProfileString("FILENAME_SET", "PlateFileName", "$(Year)$(Month)$(Day)$(Hour)$(Minute)$(Second)$(MilliSecond).txt", m_FileNameInfo.chPlateFileName, 1024, chIniFileName);
	GetPrivateProfileString("FILENAME_SET", "PlateNoFileName", "PlateNo.txt", m_FileNameInfo.chPlateNoFileName, 1024, chIniFileName);

	// ��־����
	m_iLogEnable = GetPrivateProfileInt("LOG_SET", "LogEnable", 0, chIniFileName);

	// ѹ��ѡ��
	m_iCompressEnable = GetPrivateProfileInt("COMPRESS_SET", "CompressEnable", 0, chIniFileName);
	m_iCompressQuality = GetPrivateProfileInt("COMPRESS_SET", "CompressQuality", 100, chIniFileName);
	m_iCompressSubQuality = GetPrivateProfileInt("COMPRESS_SET", "CompressSubQuality", 5, chIniFileName);
	m_iCompressSize = GetPrivateProfileInt("COMPRESS_SET", "CompressSize", 300, chIniFileName);
	if (m_iCompressSize <= 0)
		m_iCompressSize = 300 * 1024;
	else
		m_iCompressSize *= 1024;

	// ͼ������
	m_iImgProcessEnable = GetPrivateProfileInt("JPG_SET", "ImgProcessEnable", 1, chIniFileName);

	// ͼƬ����ģʽ 0Ϊ��ԭͼ�ֱ��ʱ���  1Ϊ�����÷ֱ��ʱ���
	m_iImgMode = GetPrivateProfileInt("JPG_SET", "ImgMode", 0, chIniFileName);

	// ͼƬ��С
	m_iBigImgWidth = GetPrivateProfileInt("JPG_SET", "BigImgWidth", 1600, chIniFileName);
	if (m_iBigImgWidth <= 0)
		m_iBigImgWidth = 1600;
	m_iBigImgHeight = GetPrivateProfileInt("JPG_SET", "BigImgHeight", 1200, chIniFileName);
	if (m_iBigImgWidth <= 0)
		m_iBigImgWidth = 1200;

	// �����С
	m_iFontSize = GetPrivateProfileInt("JPG_SET", "FontSize", 25, chIniFileName);

	char chTemp[256] = {0};
	int r, g, b;
	// ������ɫ
	GetPrivateProfileString("JPG_SET", "FontColor", "255,0,0", chTemp, 256, chIniFileName);
	sscanf(chTemp, "%d,%d,%d", &r, &g, &b);
	m_dwFontColor = RGB(r, g, b);

	// ˮӡ��ɫ
	GetPrivateProfileString("JPG_SET", "FontEdgeColor", "0,0,0", chTemp, 256, chIniFileName);
	sscanf(chTemp, "%d,%d,%d", &r, &g, &b);
	m_dwFontEdgeColor = RGB(r, g, b);

	// �����ͼѡ��
	m_iFeatureEnable = GetPrivateProfileInt("FeatureInfo", "FeatureEnable", 1, chIniFileName);
	m_iFeatureWidth = GetPrivateProfileInt("FeatureInfo", "FeatureWidth", 600, chIniFileName);
	m_iFeatureHeigh = GetPrivateProfileInt("FeatureInfo", "FeatureHeigh", 450, chIniFileName);

	LeaveCriticalSection(&g_csLog);

	return 0;
}

// ��ȡ��ʷ���ini�ļ�
int CCamera::ReadHistoryIniFile()
{
	TCHAR fileName[MAX_PATH];
	GetModuleFileName(NULL, fileName, MAX_PATH-1);
	PathRemoveFileSpec(fileName);
	TCHAR chIniFileName[MAX_PATH] = { 0 };
	strcpy(chIniFileName, fileName);
	strcat(chIniFileName, _T("\\SafeModeConfig.ini"));

	SYSTEMTIME systemTime;
	GetSystemTime(& systemTime);
	char szDefaultTime[64];
	ZeroMemory(szDefaultTime, sizeof(szDefaultTime));
	sprintf(szDefaultTime , "%4d.%2d.%2d_%2d", systemTime.wYear ,systemTime.wMonth , systemTime.wDay ,systemTime.wHour );
	CString strDefaultTime(szDefaultTime);
	strDefaultTime.Replace(' ','0');

	EnterCriticalSection(&g_csLog);

	m_safeModeInfo.iEanbleSafeMode = GetPrivateProfileInt(m_strDevSN, "SafeModeEnable", 0, chIniFileName);
	GetPrivateProfileString(m_strDevSN, "BeginTime", strDefaultTime, m_safeModeInfo.szBeginTime, 256, chIniFileName);
	GetPrivateProfileString(m_strDevSN, "EndTime", "0", m_safeModeInfo.szEndTime, 256, chIniFileName);
	m_safeModeInfo.index = GetPrivateProfileInt(m_strDevSN, "Index", 0, chIniFileName);
	m_safeModeInfo.DataInfo = GetPrivateProfileInt(m_strDevSN, "DataType", 0, chIniFileName);

	LeaveCriticalSection(&g_csLog);

	return 0;
}

// дini�ļ�
int CCamera::WriteIniFlie()
{
	TCHAR chFileName[MAX_PATH];
	GetModuleFileName(NULL, chFileName, MAX_PATH-1);
	PathRemoveFileSpec(chFileName);
	TCHAR chIniFileName[MAX_PATH] = { 0 };
	strcpy(chIniFileName, chFileName);
	strcat(chIniFileName, "\\Config.ini");

	EnterCriticalSection(&g_csLog);

	char chTemp[256] = {0};

	// ·����Ϣ
	WritePrivateProfileString("DIR_SET", "IllegalDir", m_chIllegalDir, chIniFileName);
	WritePrivateProfileString("DIR_SET", "NormalDir", m_chNormalDir, chIniFileName);

	// �ļ�����Ϣ
	sprintf(chTemp, "%d", m_FileNameInfo.iBigImgEnable);
	WritePrivateProfileString("FILENAME_SET", "BigImgEnable", chTemp, chIniFileName);
	sprintf(chTemp, "%d", m_FileNameInfo.iSmallImgEnable);
	WritePrivateProfileString("FILENAME_SET", "SmallImgEnable", chTemp, chIniFileName);
	sprintf(chTemp, "%d", m_FileNameInfo.iBinEnable);
	WritePrivateProfileString("FILENAME_SET", "BinEnable", chTemp, chIniFileName);
	sprintf(chTemp, "%d", m_FileNameInfo.iInfoEnable);
	WritePrivateProfileString("FILENAME_SET", "InfoEnable", chTemp, chIniFileName);
	sprintf(chTemp, "%d", m_FileNameInfo.iPlateEnable);
	WritePrivateProfileString("FILENAME_SET", "PlateEnable", chTemp, chIniFileName);
	sprintf(chTemp, "%d", m_FileNameInfo.iPlateNoEnable);
	WritePrivateProfileString("FILENAME_SET", "PlateNoEnable", chTemp, chIniFileName);

	WritePrivateProfileString("FILENAME_SET", "BigImgFileName", m_FileNameInfo.chBigImgFileName, chIniFileName);
	WritePrivateProfileString("FILENAME_SET", "SmallImgName", m_FileNameInfo.chSmallImgFileName, chIniFileName);
	WritePrivateProfileString("FILENAME_SET", "BinFileName", m_FileNameInfo.chBinFileName, chIniFileName);
	WritePrivateProfileString("FILENAME_SET", "InfoFileName", m_FileNameInfo.chInfoFileName, chIniFileName);
	WritePrivateProfileString("FILENAME_SET", "PlateFileName", m_FileNameInfo.chPlateFileName, chIniFileName);
	WritePrivateProfileString("FILENAME_SET", "PlateNoFileName", m_FileNameInfo.chPlateNoFileName, chIniFileName);

	// ��־����
	sprintf(chTemp, "%d", m_iLogEnable);
	WritePrivateProfileString("LOG_SET", "LogEnable", chTemp, chIniFileName);

	// ѹ��ѡ��
	sprintf(chTemp, "%d", m_iCompressEnable);
	WritePrivateProfileString("COMPRESS_SET", "CompressEnable", chTemp, chIniFileName);
	sprintf(chTemp, "%d", m_iCompressQuality);
	WritePrivateProfileString("COMPRESS_SET", "CompressQuality", chTemp, chIniFileName);
	sprintf(chTemp, "%d", m_iCompressSubQuality);
	WritePrivateProfileString("COMPRESS_SET", "CompressSubQuality", chTemp, chIniFileName);
	sprintf(chTemp, "%d", m_iCompressSize/1024);
	WritePrivateProfileString("COMPRESS_SET", "CompressSize", chTemp, chIniFileName);

	// ͼ������
	sprintf(chTemp, "%d", m_iImgProcessEnable);
	WritePrivateProfileString("JPG_SET", "ImgProcessEnable", chTemp, chIniFileName);
	
	// ͼƬ����ģʽ 0Ϊ��ԭͼ�ֱ��ʱ���  1Ϊ�����÷ֱ��ʱ���
	sprintf(chTemp, "%d", m_iImgMode);
	WritePrivateProfileString("JPG_SET", "ImgMode", chTemp, chIniFileName);

	// ͼƬ��С
	sprintf(chTemp, "%d", m_iBigImgWidth);
	WritePrivateProfileString("JPG_SET", "BigImgWidth", chTemp, chIniFileName);
	sprintf(chTemp, "%d", m_iBigImgHeight);
	WritePrivateProfileString("JPG_SET", "BigImgHeight", chTemp, chIniFileName);

	// �����С
	sprintf(chTemp, "%d", m_iFontSize);
	WritePrivateProfileString("JPG_SET", "FontSize", chTemp, chIniFileName);

	int r, g, b;
	// ������ɫ
	r = (BYTE)(m_dwFontColor);
	g = (BYTE)(m_dwFontColor>> 8 & 0xFF);
	b = (BYTE)(m_dwFontColor>> 16 & 0xFF);
	sprintf(chTemp, "%d,%d,%d", (int)r, (int)g, (int)b);
	WritePrivateProfileString("JPG_SET", "FontColor", chTemp, chIniFileName);
	// ������ɫ
	r = (BYTE)(m_dwFontEdgeColor);
	g = (BYTE)(m_dwFontEdgeColor>> 8 & 0xFF);
	b = (BYTE)(m_dwFontEdgeColor>> 16 & 0xFF);
	sprintf(chTemp, "%d,%d,%d", (int)r, (int)g, (int)b);
	WritePrivateProfileString("JPG_SET", "FontEdgeColor", chTemp, chIniFileName);

	// �����ͼѡ��
	sprintf(chTemp, "%d", m_iFeatureEnable);
	WritePrivateProfileString("FeatureInfo", "FeatureEnable", chTemp, chIniFileName);
	sprintf(chTemp, "%d", m_iFeatureWidth);
	WritePrivateProfileString("FeatureInfo", "FeatureWidth", chTemp, chIniFileName);
	sprintf(chTemp, "%d", m_iFeatureHeigh);
	WritePrivateProfileString("FeatureInfo", "FeatureHeigh", chTemp, chIniFileName);

	LeaveCriticalSection(&g_csLog);

	return 0;
}

// д��ʷ���ini�ļ�
int CCamera::WriteHistoryIniFile()
{
	TCHAR fileName[MAX_PATH];
	GetModuleFileName(NULL, fileName, MAX_PATH-1);
	PathRemoveFileSpec(fileName);
	TCHAR chIniFileName[MAX_PATH] = { 0 };
	strcpy(chIniFileName, fileName);
	strcat(chIniFileName, _T("\\SafeModeConfig.ini"));

	TCHAR chTemp[256] = {0};
	sprintf(chTemp, "%d", m_safeModeInfo.iEanbleSafeMode);
	if (m_safeModeInfo.iEanbleSafeMode == 0 )
	{
		CTime tm = CTime::GetCurrentTime();
		sprintf(m_safeModeInfo.szBeginTime, "%s", tm.Format("%Y.%m.%d_%H"));
	}

	EnterCriticalSection(&g_csLog);

	WritePrivateProfileString(m_strDevSN, "SafeModeEnable", chTemp, chIniFileName);
	WritePrivateProfileString(m_strDevSN, "BeginTime", m_safeModeInfo.szBeginTime, chIniFileName);
	WritePrivateProfileString(m_strDevSN, "EndTime", m_safeModeInfo.szEndTime, chIniFileName);
	sprintf(chTemp, "%d", m_safeModeInfo.index);
	WritePrivateProfileString(m_strDevSN, "Index", chTemp, chIniFileName);
	sprintf(chTemp, "%d", m_safeModeInfo.DataInfo);
	WritePrivateProfileString(m_strDevSN, "DataType", chTemp, chIniFileName);

	LeaveCriticalSection(&g_csLog);

	return 0;
}

// ʶ������ʼ�ص�����
int CCamera::RecordInfoBegin(DWORD dwCarID)
{
	if ( m_HvResult != NULL )
	{
		delete m_HvResult;
		m_HvResult = NULL;
	}

	m_HvResult = new Result();

	return 0;
}

// ʶ���������ص�����
int CCamera::RecordInfoEnd(DWORD dwCarID)
{
	if (m_HvResult == NULL)
		return -1;

	if ( m_safeModeInfo.iEanbleSafeMode == 1 )
	{
		m_safeModeInfo.index = m_HvResult->dwCarID;
		CTime tm(m_HvResult->dwTime/1000);
		sprintf(m_safeModeInfo.szBeginTime, "%s", tm.Format("%Y.%m.%d_%H"));

		m_iBreakCount++;

		if (m_iBreakCount > 3)
		{
			TCHAR chFileName[MAX_PATH];
			GetModuleFileName(NULL, chFileName, MAX_PATH-1);
			PathRemoveFileSpec(chFileName);
			TCHAR chIniFileName[MAX_PATH] = { 0 };
			strcpy(chIniFileName, chFileName);
			strcat(chIniFileName,  _T("\\SafeModeConfig.ini"));

			// ������ʷ��Ϣ
			EnterCriticalSection(&g_csLog);

			char chTemp[256] = {0};
			WritePrivateProfileString(m_strDevSN, "BeginTime", m_safeModeInfo.szBeginTime, chIniFileName);
			sprintf(chTemp, "%d", m_safeModeInfo.index);
			WritePrivateProfileString(m_strDevSN, "Index", chTemp, chIniFileName);

			LeaveCriticalSection(&g_csLog);

			m_iBreakCount = 0;
		}
	}


	// ��ӽ����������
	EnterCriticalSection(&m_csResult);
	if (m_HvResultList.GetCount() < MAX_LIST_COUNT)
	{
		m_HvResultList.AddTail(m_HvResult);
		m_HvResult = NULL;
	}
	LeaveCriticalSection(&m_csResult);

	return 0;
}

// ������Ϣ�ص�
int CCamera::GetPlate(DWORD dwCarID, 
						LPCSTR pcPlateNo, 
						LPCSTR pcAppendInfo,
						DWORD dwRecordType,
						DWORD64 dw64TimeMS)
{
	if (  m_HvResult == NULL )
		return -1;

	CString szPlate = pcPlateNo;
	if ( szPlate.Find("�޳���") == -1 )
	{
		strcpy(m_HvResult->chPlate, szPlate.Mid(2));
		strcpy(m_HvResult->chPlateColor, szPlate.Left(2));
	}
	else
	{
		strcpy(m_HvResult->chPlate, "�޳���");
		strcpy(m_HvResult->chPlateColor, "����");
	}

	if ( strstr(pcPlateNo, "�޳���") != NULL)
		strcpy(m_HvResult->chPlate, "��ʶ����");

	m_HvResult->dwCarID = dwCarID;
	m_HvResult->dwTime = dw64TimeMS;

	// ����������Ϣ
	ResolveXmlData(pcAppendInfo);

	// ��������ͨinf�ļ�
	if (pcAppendInfo != NULL)
	{
		HVAPIUTILS_ParsePlateXmlStringEx(pcAppendInfo, m_HvResult->chAppendInfo, 2048);
	}

	return 0;
}

// ��ͼ�ص�
int CCamera::GetBigImg(DWORD dwCarID, 
						WORD  wImgType,
						WORD  wWidth,
						WORD  wHeight,
						PBYTE pbPicData,
						DWORD dwImgDataLen,
						DWORD dwRecordType,
						DWORD64 dw64TimeMS)
{
	if (  m_HvResult == NULL )
		return -1;

	if (wImgType <= 0)
		wImgType = 1;
	if (wImgType > MAX_BIG_COUNT)
		wImgType = MAX_BIG_COUNT;

	LARGE_INTEGER liTemp = { 0 };
	ULARGE_INTEGER uliTemp = { 0 };

	int index = wImgType -1;

	if (m_HvResult->BigImgStream[index] == NULL)
	{
		CreateStreamOnHGlobal(NULL, TRUE, &m_HvResult->BigImgStream[index]);
	}

	if ( m_HvResult->BigImgStream[index] != NULL )
	{
		m_HvResult->BigImgStream[index]->SetSize(uliTemp);
		m_HvResult->BigImgStream[index]->Seek(liTemp, STREAM_SEEK_SET, NULL);
		m_HvResult->BigImgStream[index]->Write(pbPicData, dwImgDataLen, NULL);
		m_HvResult->dwBigImgSize[index] = dwImgDataLen;
		m_HvResult->dwImageTimeMs[index] = dw64TimeMS;
		m_HvResult->iImageCount++;
	}

	m_HvResult->BigCarID = dwCarID;

	// ��ȡ��������
	// ����a[0]Ϊ���Ͻ�y����a[1]Ϊ���Ͻ�x����
	// ����a[2]Ϊ���½�y����a[3]Ϊ���½�x����
	int a[4] = {0};
	int iBuffLen = sizeof(a);
	HVAPI_GetExtensionInfoEx(m_hHv, wImgType, a, &iBuffLen);

	// �����ص������Ǵ��ʱȡ�Զ�������
	if (a[0]>100 || a[1]>100 || a[2]>100 || a[3]>100
		|| 0 == a[0] || 0 == a[1] || 0 == a[2] || 0 == a[3])
	{
		a[0] = 50;
		a[1] = 50;
		a[2] = 50;
		a[3] = 50;
	}

	m_HvResult->rcPlatePos[index].left  = (long)(a[1] * wWidth) / 100;  //x
	m_HvResult->rcPlatePos[index].right = (long)(a[3] * wWidth) / 100; //x

	m_HvResult->rcPlatePos[index].top = (long)(a[0] * wHeight) / 100; //y
	m_HvResult->rcPlatePos[index].bottom = (long)(a[2] * wHeight) / 100; //y

	return 0;
}

// ����Сͼ�ص�
int CCamera::GetSmallImg(DWORD dwCarID,
							WORD wWidth,
							WORD wHeight,
							PBYTE pbPicData,
							DWORD dwImgDataLen,
							DWORD dwRecordType,
							DWORD64 dw64TimeMS)
{
	if ( m_HvResult == NULL )
		return -1;

	if ( m_HvResult->pbSmallImg != NULL )
	{
		delete[] m_HvResult->pbSmallImg;
		m_HvResult->pbSmallImg = NULL;
	}

	int iSmallLen = 100 * 1024;
	m_HvResult->pbSmallImg = new BYTE[iSmallLen];

	if ( m_HvResult->pbSmallImg != NULL  )
	{
		int iSmallBmpLen = iSmallLen;
		memset(m_HvResult->pbSmallImg, 0, iSmallLen);
		if ( HVAPIUTILS_SmallImageToBitmapEx(pbPicData, wWidth, wHeight, m_HvResult->pbSmallImg, &iSmallBmpLen) != S_OK )
		{
			WriteLog("ת��Сͼʧ�ܣ�");
		}
		m_HvResult->dwSmallImgSize = iSmallBmpLen;
		m_HvResult->dwSmallImgTime = dw64TimeMS;
	}
	return 0;
}

// ���ƶ�ֵͼ�ص�
int CCamera::GetBinImg(DWORD dwCarID,
						WORD wWidth,
						WORD wHeight,
						PBYTE pbPicData,
						DWORD dwImgDataLen,
						DWORD dwRecordType,
						DWORD64 dw64TimeMS)
{
	if (m_HvResult->pbBinImage != NULL)
	{
		delete[] m_HvResult->pbBinImage;
		m_HvResult->pbBinImage = NULL;
	}
	m_HvResult->pbBinImage = new BYTE[dwImgDataLen];
	m_HvResult->dwBinSize = 0;
	if (m_HvResult->pbBinImage != NULL)
	{
		memcpy(m_HvResult->pbBinImage, pbPicData, dwImgDataLen);
		m_HvResult->dwBinSize = dwImgDataLen;
	}

	return 0;
}

// д��־
int CCamera::WriteLog(char* chText)
{
	if (chText == NULL)
		return -1;

	FILE *file = NULL;
	CTime tm = CTime::GetCurrentTime();
	CString strText;
	strText.Format("%s [%s]:%s\n", tm.Format("%Y-%m-%d %H:%M:%S"), m_strDevSN, chText);

	EnterCriticalSection(&g_csLog);

	if (m_iLogEnable != 0)
	{
		file = fopen("Log.txt", "a+");
		if (file != NULL)
		{
			fprintf(file, "%s", strText.GetBuffer(strText.GetLength()));
			strText.ReleaseBuffer();
			fclose(file);
		}
		file = NULL;
	}

	LeaveCriticalSection(&g_csLog);
	
	return 0;
}

//��XML�ĸ�����Ϣ���н���
int CCamera::ResolveXmlData(LPCSTR szResultInfo)
{
	if (szResultInfo == NULL )
		return -1;

	// ���������ĵ�
	TiXmlDocument myDocument;
	if ( !myDocument.Parse(szResultInfo))
	{
		OutputDebugString( " !myDocument.Parse(szResultInfo) \n");
		return -1;
	}

	// �ĵ��ڵ�
	TiXmlElement* pRoot = myDocument.RootElement();
	if (pRoot ==  NULL)
		return -1;

	// �����
	TiXmlElement* pResultSet = pRoot->FirstChildElement();
	if ( pResultSet == NULL)
		return -1;

	// ���
	TiXmlElement* pResultElement = pResultSet->FirstChildElement();
	if ( pResultElement == NULL )
		return -1;

	// �������
	TiXmlElement* pEle = NULL;
	CString strTemp = _T("");
	const char *chElement = NULL;

	for (pEle = pResultElement->FirstChildElement(); pEle; pEle = pEle->NextSiblingElement())
	{
		if ( strcmp(pEle->Value(), "VideoScaleSpeed") == 0
			|| strcmp(pEle->Value(), "RadarScaleSpeed") == 0
			|| strcmp(pEle->Value(), "CoilScaleSpeed") == 0) // ��Ƶ���״��Ȧ����
		{
			chElement = pEle->Attribute("value");
			strTemp = chElement;
			strTemp.Replace("Km/h", "");
			m_HvResult->iSpeed = atoi(strTemp);
		}
		else if ( strcmp(pEle->Value(), "SpeedLimit") == 0 ) // ����ֵ
		{
			chElement = pEle->Attribute("value");
			strTemp = chElement;
			strTemp.Replace("Km/h", "");
			m_HvResult->iLimitSpeed = atoi(strTemp);
		}
		else if ( strcmp(pEle->Value(), "StreetName") == 0 )  // ·������
		{
			chElement = pEle->Attribute("value");
			strTemp = chElement;
			strcpy(m_HvResult->chRoadName, strTemp);
		}
		else if ( strcmp(pEle->Value(), "StreetDirection") == 0 ) // ·�ڷ���
		{
			chElement = pEle->Attribute("value");
			strTemp = chElement;
			strcpy(m_HvResult->chRoadDirection, strTemp);
		}
		else if ( strcmp(pEle->Value(), "RoadNumber") == 0 ) // ������
		{
			chElement = pEle->Attribute("value");
			strTemp = chElement;
			m_HvResult->iLand = atoi(strTemp);
		}
		else if ( strcmp(pEle->Value(), "CarType")==0 ) // ��������
		{
			chElement = pEle->Attribute("value");
			strTemp = chElement;
			strcpy(m_HvResult->chCarType, strTemp);
		}
		else if ( strcmp(pEle->Value(), "EventCheck") == 0 ) // �¼����
		{
			chElement = pEle->Attribute("value");
			strTemp = chElement;

			// ����Ϣ���н���
			BOOL fIsPeccancy = FALSE;
			//BOOL m_fIsRetrograde = FALSE;
			const char *pPeccancyType = strstr(strTemp, "Υ��:��");
			char szValue[256] = { 0 };
			CString strPeccancyType = _T("");

			if(pPeccancyType != NULL)
			{
				fIsPeccancy = TRUE;
				sscanf(pPeccancyType, "Υ��:��<%s>\n", szValue);
				strPeccancyType.Format("%s", szValue);
				// �ж����һ���ַ��Ƿ���">"�����Ǳ�ȥ��
				CString strTemp = strPeccancyType.Mid(strPeccancyType.GetLength() - 1, 1);
				if (strcmp(strTemp, ">") == 0)
				{
					strTemp = strPeccancyType.Mid(0, strPeccancyType.GetLength() - 1);
					strPeccancyType = strTemp;
				}
				// �ж����һ���ַ��Ƿ���","�����Ǳ�ȥ��
				strTemp = strPeccancyType.Mid(strPeccancyType.GetLength() - 1, 1);
				if (strcmp(strTemp, ",") == 0)
				{
					strTemp = strPeccancyType.Mid(0, strPeccancyType.GetLength() - 1);
					strPeccancyType = strTemp;
				}
				//strPeccancyType.Replace(">","");
			}
			
			// ��Υ��
			m_HvResult->iIllegalType = 0;
			strcpy(m_HvResult->chIllegalInfo, "����");

			// Υ��
			if (fIsPeccancy)
			{
				if (strPeccancyType.Find("�����") != -1)
				{
					strcpy(m_HvResult->chIllegalInfo, "�����");
					m_HvResult->iIllegalType = 3;
					// ��ƿ�ʼʱ��
					char szReadTemp[256] = { 0 };
					char *pRedTime = (char*)strstr((const char*)strTemp, "���ʱ��:");
					if (pRedTime)
					{
						sscanf(pRedTime, "���ʱ��:%d:%d:%d:%d\n", &m_HvResult->iH, &m_HvResult->iM, &m_HvResult->iS, &m_HvResult->iSS);
						m_HvResult->iRedTimeMS = m_HvResult->iH*3600000 + m_HvResult->iM*60000 + m_HvResult->iS*1000 + m_HvResult->iSS;
					}
				}
				else if (strPeccancyType.Find("����") != -1)
				{
					strcpy(m_HvResult->chIllegalInfo, "����");
					m_HvResult->iIllegalType = 5;
				}
				else if (strPeccancyType.Find("����") != -1)
				{
					strcpy(m_HvResult->chIllegalInfo, "����");
					m_HvResult->iIllegalType = 2;
				}
				else if (strPeccancyType.Find("����������ʻ") != -1)
				{
					strcpy(m_HvResult->chIllegalInfo, "����������ʻ");
					m_HvResult->iIllegalType = 4;

				}
				else if ( strPeccancyType.Find("ѹ��") != -1 )
				{
					strcpy(m_HvResult->chIllegalInfo, "ѹ��");
					m_HvResult->iIllegalType = 6;
				}
				else if (strPeccancyType.Find("����") != -1)  //����Υ��
				{
					strcpy(m_HvResult->chIllegalInfo, "����");
					m_HvResult->iIllegalType = 7;
				}
				else if (strPeccancyType.Find("�ǻ�������") != -1)
				{
					strcpy(m_HvResult->chIllegalInfo, "�ǻ�������");
					m_HvResult->iIllegalType = 8;
				}
				else
				{
					strcpy(m_HvResult->chIllegalInfo, "����");
					m_HvResult->iIllegalType = 9;
				}
			}

			// ���㳬�ٱ�
			if (m_HvResult->iSpeed > m_HvResult->iLimitSpeed)
			{
				m_HvResult->fOverSpeedRate = (float)(m_HvResult->iSpeed - m_HvResult->iLimitSpeed) * 100 / m_HvResult->iLimitSpeed;
			}
			else
			{
				m_HvResult->fOverSpeedRate = 0;
			}

		}
	}
	return 0;
}

float CCamera::GetRedTime(int iRedTime, DWORD64 dwCarTime)
{

	CTime tm(dwCarTime/1000);
	int iMS = (int)dwCarTime%1000;
	int iH, iM, iS;
	sscanf(tm.Format("%H:%M:%S"), "%d:%d:%d", &iH, &iM, &iS);

	int iCarTimeTemp = iH*3600000 + iM*60000 + iS*1000 + iMS;

	if (iCarTimeTemp < iRedTime)
		return 0;

	int iTime = iCarTimeTemp - iRedTime;

	return (float)iTime / 1000;
}

// �����ͼλ�ô�С
int CCamera::InflateCropRect(RECT& rectDst,
	const RECT &cPlatePos,
	const int& iImgWidth,
	const int& iImgHeight,
	const int& iCropWidth,
	const int& iCropHeight
	)
{
	int iPosWidth = cPlatePos.right - cPlatePos.left;
	int iPosHeigh  = cPlatePos.bottom - cPlatePos.top;

	int iTemp = iCropWidth - iPosWidth;
	iTemp >>= 1;

	int iLeftDst = cPlatePos.left - iTemp;
	int iRightDst = cPlatePos.right + iTemp;
	iRightDst = iRightDst + (iCropWidth - (iRightDst - iLeftDst) );

	if( iCropWidth >= iImgWidth )
	{
		rectDst.left = 0;
		rectDst.right = iImgWidth;
	}
	else
	{
		if( iLeftDst < 0 )
		{
			iRightDst -= iLeftDst;
			iLeftDst = 0;
		}
		if( iRightDst > iImgWidth )
		{
			iLeftDst -= (iRightDst - iImgWidth);
			iRightDst = iImgWidth;
		}
		if( (iLeftDst & 1) == 1 )
		{
			iLeftDst -= 1;
			iRightDst -= 1;
		}
		rectDst.left = iLeftDst;
		rectDst.right = iRightDst;
	}

	iTemp = iCropHeight - iPosHeigh;
	iTemp >>= 2;
	int iTopDst = cPlatePos.top - (iTemp * 3);
	int iBottomDst = cPlatePos.bottom + iTemp;
	iBottomDst =  iBottomDst + (iCropHeight - (iBottomDst - iTopDst));

	if( iCropHeight >= iImgHeight )
	{
		rectDst.top = 0;
		rectDst.bottom = iImgHeight;
	}
	else
	{
		if( iTopDst < 0 )
		{
			iBottomDst -= iTopDst;
			iTopDst = 0;
		}
		if( iBottomDst > iImgHeight )
		{
			iTopDst -= (iBottomDst - iImgHeight);
			iBottomDst = iImgHeight;
		}
		rectDst.top = iTopDst;
		rectDst.bottom = iBottomDst;
	}
	return 0;
}

// ���泵����Ϣ
int CCamera::SaveResult(Result* pHvResult)
{
	if (NULL == pHvResult)
	{
		return -1;
	}
	if (m_safeModeInfo.iEanbleSafeMode == 1 && m_safeModeInfo.DataInfo == 1)
	{
		if (pHvResult->iIllegalType == 0)
			return 0;
	}

	// �ַ��滻Ԥ��
	ReplaceInfo replaceInfo;
	replaceInfo.dwCarIndex = pHvResult->dwCarID;
	replaceInfo.dwTimeMs = pHvResult->dwTime;
	replaceInfo.iLimitSpeed = pHvResult->iLimitSpeed;
	replaceInfo.iSpeed = pHvResult->iSpeed;
	replaceInfo.iRoadNumber = pHvResult->iLand;
	replaceInfo.strPlateNo = pHvResult->chPlate;
	replaceInfo.strDeviceIP = m_strDevIP;
	replaceInfo.strDeviceNo = m_strDevSN;
	replaceInfo.strIllegalInfo = pHvResult->chIllegalInfo;
	replaceInfo.strPlateColor = pHvResult->chPlateColor;
	replaceInfo.strCarType = pHvResult->chCarType;
	replaceInfo.strStreetName = pHvResult->chRoadName;
	replaceInfo.strStreetDirection = pHvResult->chRoadDirection;

	// ·���滻
	CString strPath = _T("");
	CString strTempPath = _T("");
	if (pHvResult->iIllegalType != 0)
	{
		strTempPath.Format("%s%s", m_strDir, m_chIllegalDir);
	}
	else
	{
		strTempPath.Format("%s%s", m_strDir, m_chNormalDir);
	}
	strPath = m_Common.GetTrueString(strTempPath, replaceInfo);

	// �����ͼ
	if (m_FileNameInfo.iBigImgEnable != 0)
	{
		CString strFile = _T("");
		CString strTempFile = _T("");
		strTempFile.Format("%s%s", strPath, m_FileNameInfo.chBigImgFileName);
		
		int iImageIndex[MAX_BIG_COUNT] = {-1, -1, -1, -1, -1};
		int iPos = 2;
		int iImageCount = 0;

		for (int i=MAX_BIG_COUNT-1; i>=0; i--)
		{
			if ( iImageCount == 3 )
				break ;

			if (pHvResult->dwBigImgSize[i] != 0)
			{
				iImageCount++;
				iImageIndex[iPos] = i;
				if ( iPos == 0 )
					break ;
				iPos--;
			}
		}

		if (pHvResult->iIllegalType != 0 )
		{
			if (1 == m_iDevType && iImageCount < 3)
			{
				WriteLog("�羯��ͼƬ�����������ţ�");
				return -1;
			}
			else if (0 == m_iDevType && iImageCount < 2)
			{
				WriteLog("���ڣ�ͼƬ�����������ţ�");
				return -1;
			}
		}

		replaceInfo.iBigImageCount = iImageCount;

		// ѭ��ÿ��ͼ
		for (int i = 0; i < MAX_BIG_COUNT; i++)
		{
			if (iImageIndex[i] == -1)
			{
				continue;
			}
			int iIndex = 0;
			iIndex = iImageIndex[i];
			if (pHvResult->BigImgStream[iIndex] == NULL)
			{
				continue;
			}

			IStream* pStreamOut = NULL;
			if (m_iImgProcessEnable != 0)
			{
				if (CreateStreamOnHGlobal(NULL, TRUE, &pStreamOut) != S_OK)
				{
					WriteLog("�����ڴ�ʧ�ܣ�");
					break;
				}
				CTime tm(pHvResult->dwImageTimeMs[iIndex]/1000);
				int iTemp = (int)(pHvResult->dwImageTimeMs[iIndex] % 1000);
				CString strText = _T("");
				if (pHvResult->iIllegalType == 3) // �����
				{
					strText.Format("ʱ��:%s:%03d  �ص�:%s  ���㷽��:%s  ��ƺ�%.3f��", tm.Format("%Y-%m-%d %H:%M:%S"), iTemp,
						pHvResult->chRoadName, pHvResult->chRoadDirection, GetRedTime(pHvResult->iRedTimeMS, pHvResult->dwImageTimeMs[iIndex]));
				}
				else if (pHvResult->iIllegalType == 2) // ����
				{
					strText.Format("ʱ��:%s:%03d  �ص�:%s  ���㷽��:%s  ����:%d Km/h  �ٶ�:%d Km/h ���ٱ�:%.2f%%", tm.Format("%Y-%m-%d %H:%M:%S"), iTemp,
						pHvResult->chRoadName, pHvResult->chRoadDirection, pHvResult->iLimitSpeed, pHvResult->iSpeed, pHvResult->fOverSpeedRate);
				}
				else
				{
					strText.Format("ʱ��:%s:%03d  �ص�:%s  ���㷽��:%s", tm.Format("%Y-%m-%d %H:%M:%S"), iTemp,
						pHvResult->chRoadName, pHvResult->chRoadDirection);
				}
				ImgProcess(pHvResult, pHvResult->BigImgStream[iIndex], iIndex, strText, pStreamOut); // ͼƬ����
			}
			else
			{
				pStreamOut = pHvResult->BigImgStream[iIndex];
				pHvResult->BigImgStream[iIndex] = NULL; // ���ⱻ����ͷ�
			}

			int iLastSize = 0;
			ULARGE_INTEGER uiLength;
			if (m_Common.GetStreamLength(pStreamOut, &uiLength))
			{
				iLastSize = (int)uiLength.QuadPart;
			}

			BYTE* pByte = new BYTE[iLastSize+1];
			if (NULL == pByte)
			{
				WriteLog("�ڴ�����ʧ�ܣ�");
				return -1;
			}
			m_Common.IStreamToByteArray(pStreamOut, pByte, iLastSize);

			if (m_iFeatureEnable != 0) // ������ͨ������ʽ���ڶ���ץ��ͼΪ����
			{
				switch(iIndex)
				{
				case 0:
					replaceInfo.strBigImageID = "a";
					replaceInfo.iBigImageID = 1;
					break;
				case 1:
					replaceInfo.strBigImageID = "b";
					replaceInfo.iBigImageID = 2;
					break;
				case 2:
					replaceInfo.strBigImageID = "b";
					replaceInfo.iBigImageID = 2;
					break;
				case 3:
					replaceInfo.strBigImageID = "a";
					replaceInfo.iBigImageID = 1;
					break;
				case 4:
					replaceInfo.strBigImageID = "c";
					replaceInfo.iBigImageID = 3;
					break;
				default:
					replaceInfo.strBigImageID = "a";
					replaceInfo.iBigImageID = 1;
					break;
				}
			}
			else // ��ͨģʽ
			{
				switch(i)
				{
				case 0:
					replaceInfo.strBigImageID = "a";
					replaceInfo.iBigImageID = 1;
					break;
				case 1:
					replaceInfo.strBigImageID = "b";
					replaceInfo.iBigImageID = 2;
					break;
				case 2:
					replaceInfo.strBigImageID = "a";
					replaceInfo.iBigImageID = 1;
					break;
				case 3:
					replaceInfo.strBigImageID = "b";
					replaceInfo.iBigImageID = 2;
					break;
				case 4:
					replaceInfo.strBigImageID = "c";
					replaceInfo.iBigImageID = 3;
					break;
				default:
					replaceInfo.strBigImageID = "a";
					replaceInfo.iBigImageID = 1;
					break;
				}
			}
			strFile = m_Common.GetTrueString(strTempFile, replaceInfo);

			if( !strFile.IsEmpty() && pByte != NULL)
			{
				// ����Ŀ¼
				if ( !PathFileExists(strPath) )
				{
					MakeSureDirectoryPathExists(strPath);
				}
				FILE* pfile = fopen(strFile, "wb");
				if( pfile != NULL )
				{
					fwrite(pByte, 1, iLastSize, pfile);
					fclose(pfile);
					pfile = NULL;
					WriteFileName(strFile);
				}
			}

			if (pByte != NULL)
			{
				delete[] pByte;
				pByte = NULL;
			}
			if (pStreamOut != NULL)
			{
				pStreamOut->Release();
				pStreamOut = NULL;
			}
		}
	}

	// ����Сͼ
	if (m_FileNameInfo.iSmallImgEnable != 0)
	{
		if ( !PathFileExists(strPath) )
		{
			MakeSureDirectoryPathExists(strPath);
		}

		CString strFile = _T("");
		CString strTempFile = _T("");
		strTempFile.Format("%s%s", strPath, m_FileNameInfo.chSmallImgFileName);
		strFile = m_Common.GetTrueString(strTempFile, replaceInfo);

		if( !strFile.IsEmpty() && pHvResult->pbSmallImg != NULL )
		{
			FILE* pfile = fopen(strFile, "wb");
			if( pfile != NULL )
			{
				fwrite(pHvResult->pbSmallImg, 1, pHvResult->dwSmallImgSize, pfile);
				fclose(pfile);
				pfile = NULL;
				WriteFileName(strFile);
			}
		}
	}

	// �����ֵͼ
	if (m_FileNameInfo.iBinEnable != 0)
	{
		if ( !PathFileExists(strPath) )
		{
			MakeSureDirectoryPathExists(strPath);
		}

		CString strFile = _T("");
		CString strTempFile = _T("");
		strTempFile.Format("%s%s", strPath, m_FileNameInfo.chBinFileName);
		strFile = m_Common.GetTrueString(strTempFile, replaceInfo);

		if( !strFile.IsEmpty() && pHvResult->pbBinImage != NULL )
		{
			FILE* pfile = fopen(strFile, "wb");
			if( pfile != NULL )
			{
				fwrite(pHvResult->pbBinImage, 1, pHvResult->dwBinSize, pfile);
				fclose(pfile);
				pfile = NULL;
				WriteFileName(strFile);
			}
		}
	}

	// ���渽����Ϣ
	if (m_FileNameInfo.iInfoEnable != 0)
	{
		if ( !PathFileExists(strPath) )
		{
			MakeSureDirectoryPathExists(strPath);
		}

		CString strFile = _T("");
		CString strTempFile = _T("");
		strTempFile.Format("%s%s", strPath, m_FileNameInfo.chInfoFileName);
		strFile = m_Common.GetTrueString(strTempFile, replaceInfo);

		if( !strFile.IsEmpty() )
		{
			FILE* pfile = fopen(strFile, "a+");
			if( pfile != NULL )
			{
				fprintf(pfile, "%s\n", pHvResult->chAppendInfo);
				fclose(pfile);
				pfile = NULL;
				WriteFileName(strFile);
			}
		}
	}

	// ���泵����Ϣ
	if (m_FileNameInfo.iPlateEnable != 0)
	{
		if ( !PathFileExists(strPath) )
		{
			MakeSureDirectoryPathExists(strPath);
		}

		CString strFile = _T("");
		CString strTempFile = _T("");
		strTempFile.Format("%s%s", strPath, m_FileNameInfo.chPlateFileName);
		strFile = m_Common.GetTrueString(strTempFile, replaceInfo);

		if( !strFile.IsEmpty() )
		{
			FILE* pfile = fopen(strFile, "a+");
			if( pfile != NULL )
			{
				fprintf(pfile, "%s\n", pHvResult->chPlate);
				fclose(pfile);
				pfile = NULL;
				WriteFileName(strFile);
			}
		}
	}

	// ���泵�ƻ���
	if (m_FileNameInfo.iPlateNoEnable != 0)
	{
		if ( !PathFileExists(strPath) )
		{
			MakeSureDirectoryPathExists(strPath);
		}

		CString strFile = _T("");
		CString strTempFile = _T("");
		strTempFile.Format("%s%s", strPath, m_FileNameInfo.chPlateNoFileName);
		strFile = m_Common.GetTrueString(strTempFile, replaceInfo);

		if( !strFile.IsEmpty() )
		{
			FILE* pfile = fopen(strFile, "a+");
			if( pfile != NULL )
			{
				fprintf(pfile, "%s\n", pHvResult->chPlate);
				fclose(pfile);
				pfile = NULL;
				WriteFileName(strFile);
			}
		}
	}

	return 0;
}

// ͼƬ����
int CCamera::ImgProcess(Result* pHvResult, IStream* pStream, int iImgIndex, CString strText, IStream* pStreamOut)
{
	int iRet = -1;
	if (pStream == NULL)
	{
		return iRet;
	}
	Bitmap bmp(pStream);
	///==============================�ַ���������*��ʼ===========================================
	StringFormat fmtString;
	fmtString.SetAlignment(StringAlignmentNear);
	fmtString.SetLineAlignment(StringAlignmentNear); 

	FontFamily fntFamily(L"����");
	PointF point(0, 0);
	int iSingleFontSize = m_iFontSize;
	
	// ������ɫ
	BYTE r, g, b;
	r = (BYTE)(m_dwFontColor);
	g = (BYTE)(m_dwFontColor>> 8 & 0xFF);
	b = (BYTE)(m_dwFontColor>> 16 & 0xFF);
	SolidBrush solidBrush(Color(255, r, g, b));

	// ������ɫ
	r = (BYTE)(m_dwFontEdgeColor);
	g = (BYTE)(m_dwFontEdgeColor>> 8 & 0xFF);
	b = (BYTE)(m_dwFontEdgeColor>> 16 & 0xFF);
	SolidBrush solidBrushBack(Color(255, r, g, b));

	int iImageWidth = bmp.GetWidth();
	int iImageHeight = bmp.GetHeight();
	if (m_iImgMode != 0)
	{
		iImageWidth = m_iBigImgWidth;
		iImageHeight = m_iBigImgHeight;
	}

	// ��ͼ����
	RECT rectFeature = {0};
	if (1 == m_iFeatureEnable && 3 == iImgIndex)  // ������ͨ�ڶ���ץ��ͼ��ȡ�ɳ���ͼ
	{
        if (0 == pHvResult->rcPlatePos[iImgIndex].left) // δ��ȡ������
        {
			rectFeature.left = 0;
            rectFeature.top = 0;
            rectFeature.right = bmp.GetWidth();
            rectFeature.bottom = bmp.GetHeight();
        }
        else
        {
			InflateCropRect(rectFeature, pHvResult->rcPlatePos[iImgIndex], bmp.GetWidth(), bmp.GetHeight(),
                    m_iFeatureWidth, m_iFeatureHeigh);
        }
		iImageWidth = rectFeature.right - rectFeature.left;
		iImageHeight = rectFeature.bottom - rectFeature.top;
	}
	RectF rfFeature(0, 0, (REAL)iImageWidth, (REAL)iImageHeight);

	// ����ͼƬ��С�Զ��ı������С���������ȡ�������С
	CStringW wStr(strText);
	Graphics gr1(&bmp);
	int iFontWidth = 0;
	int iFontHeight = 0;
	while (true)
	{
		Gdiplus::Font fontTmp(&fntFamily, (REAL)iSingleFontSize, FontStyleBold);
		RectF outrect;
		gr1.MeasureString(wStr, -1, &fontTmp, point, &outrect); // ���������ߣ��ó�������
		if (outrect.Width > iImageWidth-1 && iSingleFontSize > 8 )
		{
			iSingleFontSize = iSingleFontSize - 2;
			continue;
		}

		iFontWidth = (int)outrect.Width;
		iFontHeight = (int)outrect.Height;
		break;
	}
	// ˢ��������
	Gdiplus::Font fontTmp(&fntFamily, (REAL)iSingleFontSize, FontStyleBold);
	// ���ñ������λ�ô�С
	RectF rfBack(0, (REAL)iImageHeight, (REAL)iImageWidth, (REAL)iFontHeight+10);
	// ��������λ�þ��δ�С
	RectF rfDestMString(0, (REAL)iImageHeight+5, (REAL)iImageWidth, (REAL)iFontHeight+10);
	///==============================�ַ���������*����===========================================

	///===============================�����ַ�*��ʼ======================================
	Bitmap *pDest = NULL;
	pDest = new Bitmap(iImageWidth, iImageHeight + iFontHeight+ 10, bmp.GetPixelFormat());
	if (pDest == NULL)
	{
		return iRet ;
	}
	Graphics grMain(pDest);

	if (1 == m_iFeatureEnable && 3 == iImgIndex)  // ������ͨ�ڶ���ץ��ͼ��ȡ�ɳ���ͼ
    {
	    grMain.DrawImage(&bmp, rfFeature, (REAL)rectFeature.left, (REAL)rectFeature.top,
               (REAL)(rectFeature.right - rectFeature.left), (REAL)(rectFeature.bottom - rectFeature.top), UnitPixel);
	}
	else
	{
		grMain.DrawImage(&bmp, 0, 0, iImageWidth, iImageHeight);
	}
	// ��ˢ����
	grMain.FillRectangle(&solidBrushBack, rfBack);
	// ��������
	grMain.DrawString(wStr, -1, &fontTmp, rfDestMString, &fmtString, &solidBrush);
	///==============================�ַ���������*����===========================================

	LARGE_INTEGER liTemp = { 0 };
	ULARGE_INTEGER uliZero = { 0 };
	pStreamOut->SetSize(uliZero);
	pStreamOut->Seek(liTemp, STREAM_SEEK_SET, NULL );

	int iLastSize = pHvResult->dwBigImgSize[iImgIndex];

	pDest->Save(pStreamOut, &m_jpgClsid, 0);
	ULARGE_INTEGER uiLength;
	if (m_Common.GetStreamLength(pStreamOut, &uiLength))
	{
		iLastSize = (int)uiLength.QuadPart;
	}

	// ѹ��
	if (1 == m_iCompressEnable)
	{
		// �����������б�
		EncoderParameters encoderParameters;
		// ������ֻ����һ��EncoderParameter����    
		encoderParameters.Count = 1;    
		encoderParameters.Parameter[0].Guid = EncoderQuality;    
		// ��������ΪLONG
		encoderParameters.Parameter[0].Type = EncoderParameterValueTypeLong;    
		// ֻ����һ������
		encoderParameters.Parameter[0].NumberOfValues = 1;  
		// ѹ������ 
		ULONG quality = m_iCompressQuality;  
		encoderParameters.Parameter[0].Value = &quality;

		while (iLastSize >= m_iCompressSize && quality > 0)
		{
			int iSub  = (int)(quality - m_iCompressSubQuality);
			if (iSub <= 0)
				quality = 0;
			else
				quality = iSub;

			encoderParameters.Parameter[0].Value = &quality;
			pStreamOut->Seek(liTemp, STREAM_SEEK_SET, NULL);
			pStreamOut->SetSize(uliZero);

			if (pDest->Save(pStreamOut, &m_jpgClsid, &encoderParameters) == Ok )
			{
				ULARGE_INTEGER uiLength;
				if (m_Common.GetStreamLength(pStreamOut, &uiLength))
				{
					iLastSize = (int)uiLength.QuadPart;
				}
			}
		}
	}

	pStreamOut->Seek(liTemp, STREAM_SEEK_SET, NULL);

	if (pDest != NULL)
	{
		delete pDest;
		pDest = NULL;
	}

	return 0;
}