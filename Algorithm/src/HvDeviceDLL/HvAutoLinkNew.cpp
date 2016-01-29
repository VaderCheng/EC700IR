

#include "HvAutoLinkNew.h"
#include <atltime.h>

//#include "HvAutoLinkDeviceNew.h"

//#include "HvDeviceBaseType.h"
//#include "HvDeviceCommDef.h"
//#include "HvDeviceUtils.h"

//#include "HvCameraLinkOpt.h"
//#include "HvCameraLink.h"

//#include "..\Inc\HvCameraLink.h"
//#include "..\Inc\HvCameraLinkOpt.h"

using namespace HiVideo;

HRESULT ReleaseDevContext(_HVAPI_AL_CONTEXT *pContext);

NetServer *g_NetServer = NULL;

bool g_fServerStop = false;

const int MAX_ATUO_CONTEXT = 1000;

//�豸������֪ͨ�߳�
static DWORD WINAPI HvEventNteThreadFunc(LPVOID lpParameter)
{
	if ( lpParameter == NULL )
	{
		g_fServerStop = true;
		return 0;
	}

	NetServer *pNetServer = (NetServer *)lpParameter;

	while( !g_fServerStop )
	{
		//�ص��Ѿ��Ͽ�������
		
		for(int i=0; i<MAX_ATUO_CONTEXT; i++)
		{
			
			if ( pNetServer->pAutoContext[i] != NULL && pNetServer->pAutoContext[i]->fCmdThExit &&  pNetServer->pAutoContext[i]->eNotieType == ONLINE  /*( pNetServer->pAutoContext[i]->eNotieType == ONLINE || pNetServer->pAutoContext[i]->dwRef >0 ) */)  //�����̱߳�־�ѱ�־�˳�������ǰ������֪ͨ�������跢������֪ͨ   Ӧ�ü�����Ϊ0�� ˵��������ʹ�ã�֪ͨ
			{
				if ( pNetServer->noticeFunc.pOnOffLine != NULL )
				{
					pNetServer->noticeFunc.pOnOffLine(pNetServer->noticeFunc.pOnOffLineParam, pNetServer->pAutoContext[i], pNetServer->pAutoContext[i]->szDeviceNo);
				}

				pNetServer->pAutoContext[i]->eNotieType = OFFLINE;  //�豸������֪ͨ
			}

			//�ص����ߵ�����
			if (  pNetServer->pAutoContext[i] != NULL && !pNetServer->pAutoContext[i]->fCmdThExit &&  pNetServer->pAutoContext[i]->eNotieType == NONOTICE /*( pNetServer->pAutoContext[i]->eNotieType == NONOTICE || pNetServer->pAutoContext[i]->dwRef<=0 ) */)  //����ͻ�������ξ������û��һ��ʱ������һ��
			{
				if ( pNetServer->noticeFunc.pOnOnLine != NULL  )
				{
					pNetServer->noticeFunc.pOnOnLine(pNetServer->noticeFunc.pOnOnLineParam, pNetServer->pAutoContext[i], pNetServer->pAutoContext[i]->szDeviceNo,pNetServer->pAutoContext[i]->szAddr, pNetServer->pAutoContext[i]->iPort,pNetServer->pAutoContext[i]->szDeviceInfo);

				}
				pNetServer->pAutoContext[i]->eNotieType =  ONLINE;
			}

			if ( pNetServer->pAutoContext[i] != NULL  && pNetServer->pAutoContext[i]->fCmdThExit && pNetServer->pAutoContext[i]->dwRef == 0 )
			{
				CAutoLock sLock(&pNetServer->csContext);  

				ReleaseDevContext(pNetServer->pAutoContext[i]);
				delete pNetServer->pAutoContext[i];
				pNetServer->pAutoContext[i] = NULL;
			}

		}

		Sleep(2000);
	}

	return 0;
}



//��ʼ��ģ��
HV_API_AL HRESULT CDECL HVAPI_AL_InitActiveModule()
{
	if ( g_NetServer != NULL )
		return S_OK;

	g_NetServer = new NetServer();
	if (g_NetServer != NULL )
	{
		g_fServerStop = false;
		g_NetServer->pAutoContext = new _HVAPI_AL_CONTEXT*[MAX_ATUO_CONTEXT];

		for(int i=0; i<MAX_ATUO_CONTEXT; i++)
			g_NetServer->pAutoContext[i] = NULL;

		// ͬʱ���������������֪ͨ����
		g_NetServer->hNteThread = CreateThread(NULL, 0, HvEventNteThreadFunc, g_NetServer, 0, NULL);
		if ( NULL ==g_NetServer->hNteThread )
		{
			g_fServerStop = true;
			delete g_NetServer->pAutoContext;
			g_NetServer->pAutoContext = NULL;

			delete g_NetServer;
			
			return E_FAIL;
		}

	}
	return S_OK;
}

//�����Ƿ��п���λ�������µľ��
int FindEmptyAutoContext()
{
	g_fServerStop = false;

	if ( g_NetServer == NULL )
		return -1;

	int index = -1;

	for(int i=0;i<MAX_ATUO_CONTEXT; i++)
	{
		if (g_NetServer->pAutoContext[i] == NULL)
		{
			index = i;
			break;
		}
	}
	return index;
}


//�����豸��Ų��Ҳ�������
int FindExistAutoContext(char *szDeviceNo)
{
	if ( szDeviceNo == NULL )
		return -1;

	//CAutoLock sLock(&g_NetServer->csContext);
	int index = -1;

	for(int i=0;i<MAX_ATUO_CONTEXT; i++)
	{
		if ( g_NetServer->pAutoContext[i] == NULL )
			continue;

		if ( strcmp(g_NetServer->pAutoContext[i]->szDeviceNo, szDeviceNo) == 0 )
		{
			index = i;
			break;
		}
	}

	return index;
}


/// ����һ���������Դ
HRESULT ReleaseDevContext(_HVAPI_AL_CONTEXT *pContext)
{
	if ( pContext == NULL )
		return S_OK;

	pContext->fCmdThExit = true;
	pContext->videoH264.fExit = true;
	pContext->videoJpeg.fExit = true;
	HvSafeCloseThread(pContext->hThreadCmd);
	ForceCloseSocket(pContext->sktCmd);

	
	HvSafeCloseThread(pContext->videoH264.hThreadServer);
	ForceCloseSocket(pContext->videoH264.sktVideo);

	
	HvSafeCloseThread(pContext->videoJpeg.hThreadServer);
	ForceCloseSocket(pContext->videoJpeg.sktVideo);

	return S_OK;
}


//ж��ģ��
HV_API_AL HRESULT CDECL  HVAPI_AL_UnInitActiveModule()
{
	if (g_NetServer == NULL )
	{
		return S_OK;
		
	}


	g_fServerStop = true;
	HvSafeCloseThread(g_NetServer->hNteThread);


	for(int i=0; i<MAX_ATUO_CONTEXT;i++)
	{
		if (g_NetServer->pAutoContext[i] != NULL)
		{
			g_NetServer->pAutoContext[i]->fCmdThExit = true;
		}
	}



	if ( g_NetServer->pAutoContext != NULL)
	{
		for(int i=0; i<MAX_ATUO_CONTEXT;i++)
		{
			if (g_NetServer->pAutoContext[i] != NULL)
			{
				ReleaseDevContext(g_NetServer->pAutoContext[i]);

				delete g_NetServer->pAutoContext[i];
				g_NetServer->pAutoContext[i] = NULL;
			}
		}

		delete[] g_NetServer->pAutoContext;
		g_NetServer->pAutoContext = NULL;
	}

	delete g_NetServer;
	g_NetServer = NULL;


	return S_OK;
}


//ע���豸���߻ص�
HV_API_AL HRESULT HVAPI_AL_RegDevOnLineNotice(HVAPI_AL_ONLINE_NOTICE onLineNotice, void* pUserData)
{
	if ( g_NetServer == NULL )
		return E_FAIL;

	g_NetServer->noticeFunc.pOnOnLine = onLineNotice;
	g_NetServer->noticeFunc.pOnOnLineParam = pUserData;

	return S_OK;
}


//ע���豸���߻ص�
HV_API_AL HRESULT HVAPI_AL_RegDevOffLineNotice(HVAPI_AL_OFFLINE_NOTICE onoffLineNotice, void* pUserData)
{
	if ( g_NetServer == NULL )
		return E_FAIL;

	g_NetServer->noticeFunc.pOnOffLine = onoffLineNotice;
	g_NetServer->noticeFunc.pOnOffLineParam = pUserData;

	return S_OK;

}

//������������ά���߳�
static DWORD WINAPI HvCmdClientThreadFunc(LPVOID lpParameter)
{
	if ( lpParameter == NULL )
		return 0;

	_HVAPI_AL_CONTEXT *pAutoContext = (_HVAPI_AL_CONTEXT*)lpParameter;

	CAMERA_CMD_HEADER cInfoHeader;
	CAMERA_CMD_HEADER cInfoHeaderResponse;
	cInfoHeaderResponse.dwID = CAMERA_CMD_THROB;
	cInfoHeaderResponse.dwInfoSize = 0;
	

	int iStayCount = 4;
	while( !pAutoContext->fCmdThExit && !g_NetServer->fCmdSvrStop && !g_fServerStop)
	{
		
		DWORD dwTick = GetTickCount();
		

		//if (!pAutoContext->fBusySktCmd  && dwTick - pAutoContext->dwCmdTick >2000)

		if (!pAutoContext->fBusySktCmd  && iStayCount>=4)
		{
			iStayCount=0;

			CAutoLock sLock(&pAutoContext->csSktCmd);
			pAutoContext->fBusySktCmd = true;

			//����H264 ��Ƶ����
			if ( pAutoContext->videoH264.fExit &&  ( pAutoContext->CallBackSet.pOnRecoreH264Video != NULL  ||pAutoContext->CallBackSet.pOnRecoreH264HistoryVideo != NULL) )
			{
				// ִ������
				bool fRet = false;
				//��װXML ����
				char szCmd[32] = {"StartAutoH264"};
				char szClass[128] = {"SETTER"};
				char szValue[1024] = {"defaule"};
				char szXmlBuff[2048] = { 0 };

				int iBuffLen = HvGetXmlOfStringType(szCmd,szClass, szValue, szXmlBuff);

				int iRetLen = 1024;
				char szRetBuf[1024] = {0 };
				fRet = ExecXmlExtCmdMercury(NULL, (char*)szXmlBuff, (char*)szRetBuf, iRetLen, pAutoContext->sktCmd);

				if ( !fRet )
					pAutoContext->fCmdThExit = true;

			}
			else if (pAutoContext->videoJpeg.fExit &&  pAutoContext->CallBackSet.pOnMJPEG != NULL  )   //����JPEG
			{
				//��װXML ����
				bool fRet = false;
				char szCmd[32] = {"StartAutoJPEG"};
				char szClass[128] = {"SETTER"};
				char szValue[1024] = {"defaule"};
				char szXmlBuff[2048] = { 0 };

				int iBuffLen = HvGetXmlOfStringType(szCmd,szClass, szValue, szXmlBuff);
				int iRetLen = 1024;
				char szRetBuf[1024] = {0 };

				fRet = ExecXmlExtCmdMercury(NULL, (char*)szXmlBuff, (char*)szRetBuf, iRetLen, pAutoContext->sktCmd);

				if ( !fRet )
					pAutoContext->fCmdThExit = true;

			}
			else
			{
				//����������
				if( send(pAutoContext->sktCmd, (char*)&cInfoHeaderResponse, sizeof(cInfoHeaderResponse), 0) == sizeof(cInfoHeaderResponse))
				{
					if (RecvAll(pAutoContext->sktCmd, (char*)&cInfoHeader, sizeof(cInfoHeader)) != sizeof(cInfoHeader))
					{
						pAutoContext->fCmdThExit = true;     //�޷����յ������������������˳�
					}
				}
				else
				{
					pAutoContext->fCmdThExit = true;  //�޷����������������˳�
				}
			}
			pAutoContext->dwCmdTick = GetTickCount();
			pAutoContext->fBusySktCmd = false;
		}
		else
		{
			Sleep(1000);
			iStayCount++;
		}

	}

	pAutoContext->fCmdThExit = true;
	return 0;
}

HRESULT CloseDateLinkServer()
{
	if ( g_NetServer == NULL )
		return S_OK;

	g_NetServer->fDateSvrStop = true;
	
	HvSafeCloseThread(g_NetServer->hThreadDatedSvr);
	ForceCloseSocket(g_NetServer->sktDateSvr);

	return S_OK;
}


HRESULT GetParamString(
							  const TiXmlElement* pCmdArgElement,
							  const char *pszParamName,
							  char *pszParamValue,
							  int nParamValueSize
							  )
{
	if ( pCmdArgElement == NULL
		|| pszParamName == NULL
		|| pszParamValue == NULL
		|| nParamValueSize <= 0 )
	{
		return E_POINTER;
	}

	bool fFind = false;
	const TiXmlElement* pValueElement = pCmdArgElement->FirstChildElement("Value");
	while ( pValueElement )
	{
		if ( 0 == strcmp(pszParamName, pValueElement->Attribute("id")) )
		{
			if ( strlen(pValueElement->GetText()) < (size_t)nParamValueSize )
			{
				fFind = true;
				strcpy(pszParamValue, pValueElement->GetText());
				break;
			}
		}
		pValueElement = pValueElement->NextSiblingElement();
	}

	return fFind ? S_OK : E_FAIL;
}




//����H264 ����
static DWORD WINAPI HvH264ServThreadFunc(LPVOID lpParameter)
{ 
	if ( lpParameter == NULL )
	{
		return 0;
	}

	///

	_HVAPI_AL_CONTEXT *pContext = (_HVAPI_AL_CONTEXT*)lpParameter;

	//������ʷ���ս�����ַ���

	CTime cBeginTime(pContext->videoH264.dwBeginTime);
	CTime cEndTime(pContext->videoH264.dwEndTime);
	CHAR szCmd[256];

	DWORD64 dw64BeginTimeMS = pContext->videoH264.dwBeginTime;
	DWORD64 dw64EndTimeMS = pContext->videoH264.dwEndTime;

	if ( !pContext->videoH264.fHistoryEnable )
	{
		sprintf( szCmd ,"DownloadVideo,BeginTime[0],EndTime[0],Enable[0]");
	}
	else
	{
		if ( 0 != dw64BeginTimeMS && 0 == dw64EndTimeMS )
		{
			sprintf( szCmd ,"DownloadVideo,BeginTime[%s],EndTime[0],Enable[1]",
				cBeginTime.Format("%Y.%m.%d_%H:%M:%S") );
		}
		else if ( 0 != dw64BeginTimeMS && 0 != dw64EndTimeMS && dw64BeginTimeMS < dw64EndTimeMS )
		{
			sprintf( szCmd ,"DownloadVideo,BeginTime[%s],EndTime[%s],Enable[1]",
				cBeginTime.Format("%Y.%m.%d_%H:%M:%S") , cEndTime.Format("%Y.%m.%d_%H:%M:%S") );
		}
		else
		{
			sprintf( szCmd ,"DownloadVideo,BeginTime[0],EndTime[0],Enable[0]");
		}
	}


	char szRetBuf[1024] = { 0 };
	if( HvSendXmlCmd(szCmd, szRetBuf, sizeof(szRetBuf),NULL, 0, pContext->videoH264.sktVideo) != S_OK )
	{
		pContext->videoH264.fExit = true;
		return 0;
	}

	INFO_HEADER cInfoHeader;
	INFO_HEADER cInfoHeaderResponse;


	unsigned char* pbInfo = NULL;
	unsigned char* pbData = NULL;

	while ( !pContext->videoH264.fExit && !pContext->fCmdThExit && !g_NetServer->fCmdSvrStop && !g_fServerStop)
	{
		if( RecvAll(pContext->videoH264.sktVideo, (char*)&cInfoHeader, sizeof(cInfoHeader)) != sizeof(cInfoHeader) )
			break;

		SAFE_DELETE_ARG(pbInfo);
		SAFE_DELETE_ARG(pbData);

		if(cInfoHeader.dwType == CAMERA_THROB)
		{
			if(send(pContext->videoH264.sktVideo, (char*)&cInfoHeaderResponse, sizeof(cInfoHeaderResponse), 0) != sizeof(cInfoHeaderResponse))
			{
				break;
			}
			continue;
		}//if cInfo

		DWORD32 dwBuffLen = 10 * 1024 * 1024;
		if (cInfoHeader.dwInfoLen > dwBuffLen || cInfoHeader.dwDataLen > dwBuffLen)
		{
			break;
		}

		pbInfo = new unsigned char[cInfoHeader.dwInfoLen + 1];
		pbData = new unsigned char[cInfoHeader.dwDataLen + 1];
		ZeroMemory(pbInfo, cInfoHeader.dwInfoLen + 1);
		ZeroMemory(pbData, cInfoHeader.dwDataLen + 1);

		if(cInfoHeader.dwInfoLen > 0)
		{
			if(RecvAll(pContext->videoH264.sktVideo, (char*)pbInfo, cInfoHeader.dwInfoLen)!= cInfoHeader.dwInfoLen)
				break;
		}

		if(cInfoHeader.dwDataLen > 0)
		{
			if(RecvAll(pContext->videoH264.sktVideo, (char*)pbData, cInfoHeader.dwDataLen) != cInfoHeader.dwDataLen)
			{
				break;
			}
		}


		if( cInfoHeader.dwType == CAMERA_VIDEO )  //H264 ��Ƶ
		{
			// ������������Ϣ����
			DWORD32 dwVideoType = 0;
			DWORD32 dwFrameType = 0;
			DWORD64 dw64VideoTime = 0;
			VideoWidth cVideoWidth = {0};
			VideoHeight cVideoHeight = {0};
			VideoExtInfo cVideoExtInfo = {0};

			//����
			TiXmlDocument cXmlDoc;
			if (!cXmlDoc.Parse((const char*)pbInfo))
				continue;

			const TiXmlElement* pRootElement = cXmlDoc.RootElement();

			if (NULL == pRootElement)
				continue;
			
			const TiXmlElement* pImageElement = pRootElement->FirstChildElement("Video");
			if (NULL == pImageElement)
				continue;
			const char* szType = pImageElement->Attribute("Type");
			const char* szFrameType = pImageElement->Attribute("FrameType");
			const char* szWidth = pImageElement->Attribute("Width");
			const char* szHeight = pImageElement->Attribute("Height");
			const char* szTime = pImageElement->Attribute("Time");
			const char* szTimeHigh = pImageElement->Attribute("TimeHigh");
			const char* szTimeLow = pImageElement->Attribute("TimeLow");
			if (NULL == szType ||NULL == szFrameType ||NULL == szWidth ||NULL == szHeight )
				continue;

			const char* szShutter = pImageElement->Attribute("Shutter");
			const char* szGain = pImageElement->Attribute("Gain");
			const char* szRGain = pImageElement->Attribute("r_Gain");
			const char* szGGain = pImageElement->Attribute("g_Gain");
			const char* szBGain = pImageElement->Attribute("b_Gain");
			
			cVideoExtInfo.iShutter = atoi(szShutter?szShutter:"0");
			cVideoExtInfo.iGain = atoi(szGain?szGain:"0");
			cVideoExtInfo.iGainR = atoi(szRGain?szRGain:"0");
			cVideoExtInfo.iGainG = atoi(szGGain?szGGain:"0");
			cVideoExtInfo.iGainB = atoi(szBGain?szBGain:"0");

			if(strcmp("H264",szType ) == 0)
			{
				dwVideoType = CAMERA_VIDEO_H264;
			}
			if(strcmp("IFrame",szFrameType ) == 0)
			{
				dwFrameType = CAMERA_FRAME_I;
			}
			else if(strcmp("PFrame",szFrameType ) == 0)
			{
				dwFrameType = CAMERA_FRAME_P;
			}
			else if(strcmp("IPFrame",szFrameType ) == 0)
			{
				dwFrameType = CAMERA_FRAME_IP_ONE_SECOND;
			}
			
			cVideoWidth.iWidth = atoi(szWidth ? szWidth:"0");
			cVideoHeight.iHight = atoi(szHeight?szHeight:"0");
			DWORD dwTimeHigh = atoi(szTimeHigh ? szTimeHigh:"0");
			DWORD dwTimeLow = atoi(szTimeLow ? szTimeLow:"0");
			dw64VideoTime = (((DWORD64)dwTimeHigh)<<32) | dwTimeLow;

			if ( dwVideoType == CAMERA_VIDEO_H264 )
			{
				DWORD dwType = VIDEO_TYPE_UNKNOWN;
				if ( CAMERA_FRAME_I == dwFrameType )
				{
					dwType = VIDEO_TYPE_H264_NORMAL_I;
				}
				else if ( CAMERA_FRAME_P == dwFrameType )
				{
					dwType = VIDEO_TYPE_H264_NORMAL_P;
				}
				else if ( CAMERA_FRAME_IP_ONE_SECOND == dwFrameType )
				{
					continue;	
				}
				
				char* pszVideoExtInfo = new char[128];
				if(pszVideoExtInfo)
				{
					sprintf(pszVideoExtInfo, "Shutter:%d,Gain:%d,R:%d,G:%d,B:%d,FrameTime:%I64u,Width:%d,Height:%d",cVideoExtInfo.iShutter, cVideoExtInfo.iGain, cVideoExtInfo.iGainR,
								cVideoExtInfo.iGainG, cVideoExtInfo.iGainB, dw64VideoTime, cVideoWidth.iWidth, cVideoHeight.iHight);
				}

				if ( true )
				{
					CAutoLock sLock(& pContext->csH264Call); 
					if ( pContext->CallBackSet.pOnRecoreH264Video != NULL )
					{
						 pContext->CallBackSet.pOnRecoreH264Video(pContext->CallBackSet.pOnH264Param
									, 0
									, dwType
									, cVideoWidth.iWidth
									, cVideoHeight.iHight
									, dw64VideoTime
									, pbData
									, cInfoHeader.dwDataLen
									,  pszVideoExtInfo
									);
					}
				}

				SAFE_DELETE_ARG(pszVideoExtInfo);
			}

						
		}//cInfoHeader.dwType == CAMERA_VIDEO




	}//while end

	SAFE_DELETE_ARG(pbInfo);
	SAFE_DELETE_ARG(pbData);


	pContext->videoH264.fExit = true;
	return 0;
}



//����JPEG �����߳�
static DWORD WINAPI HvJpegServThreadFunc(LPVOID lpParameter)
{
	if ( lpParameter == NULL )
	{
		return 0;
	}


	////
	_HVAPI_AL_CONTEXT *pContext = (_HVAPI_AL_CONTEXT*)lpParameter;

	BOOL fEnableRecogVideo = (pContext->videoJpeg.dwRecvType == MJPEG_RECV_FLAG_DEBUG);
	CHAR szConnCmd[128];

	CHAR szCmd[256];
	sprintf( szCmd ,"DownloadImage, Enable[0]");

	char szRetBuf[1024] = { 0 };
	if( HvSendXmlCmd(szCmd, szRetBuf, sizeof(szRetBuf),NULL, 0, pContext->videoJpeg.sktVideo) != S_OK )
	{
		pContext->videoJpeg.fExit = true;
		return 0;
	}



	/*
	sprintf(szConnCmd , "SetImgType,EnableRecogVideo[%d]", fEnableRecogVideo );
	
	char szRetBuf[256] = {0};
	char szTmpConnCmd[128] = {0};
	memcpy(szTmpConnCmd, (char*)szConnCmd, 128);
	char szIP[32] = {"10.10.10.10"};
	
	if(HvSendXmlCmd(szIP, szTmpConnCmd, szRetBuf, sizeof(szRetBuf),NULL,pContext->videoJpeg.sktVideo) != S_OK)
	{
		pContext->videoJpeg.fExit = true;
		return E_FAIL;
	}*/
	

	INFO_HEADER cInfoHeader;
	INFO_HEADER cInfoHeaderResponse;
	BLOCK_HEADER cBlockHeader;
	unsigned char* pbInfo = NULL;
	unsigned char* pbData = NULL;
	cInfoHeaderResponse.dwType = CAMERA_THROB_RESPONSE;
	cInfoHeaderResponse.dwInfoLen = 0;
	cInfoHeaderResponse.dwDataLen = 0;

	DWORD32 dwBuffLen = 10 * 1024 * 1024;

	while ( !pContext->videoJpeg.fExit && !pContext->fCmdThExit && !g_NetServer->fCmdSvrStop && !g_fServerStop )
	{
		if( RecvAll(pContext->videoJpeg.sktVideo, (char*)&cInfoHeader, sizeof(cInfoHeader)) != sizeof(cInfoHeader) )
			break;

		SAFE_DELETE_ARG(pbInfo);
		SAFE_DELETE_ARG(pbData);

		if(cInfoHeader.dwType == CAMERA_THROB)
		{
			if(send(pContext->videoJpeg.sktVideo, (char*)&cInfoHeaderResponse, sizeof(cInfoHeaderResponse), 0) != sizeof(cInfoHeaderResponse))
			{
				break;
			}
			continue;
		}//if cInfo

		
		if (cInfoHeader.dwInfoLen > dwBuffLen || cInfoHeader.dwDataLen > dwBuffLen)
		{
			break;
		}

		pbInfo = new unsigned char[cInfoHeader.dwInfoLen + 1];
		pbData = new unsigned char[cInfoHeader.dwDataLen + 1];
		ZeroMemory(pbInfo, cInfoHeader.dwInfoLen + 1);
		ZeroMemory(pbData, cInfoHeader.dwDataLen + 1);

		if(cInfoHeader.dwInfoLen > 0)
		{
			if(RecvAll(pContext->videoJpeg.sktVideo, (char*)pbInfo, cInfoHeader.dwInfoLen)!= cInfoHeader.dwInfoLen)
				break;
		}

		if(cInfoHeader.dwDataLen > 0)
		{
			if(RecvAll(pContext->videoJpeg.sktVideo, (char*)pbData, cInfoHeader.dwDataLen) != cInfoHeader.dwDataLen)
			{
				break;
			}
		}

		//��ͼƬ
		if(cInfoHeader.dwType == CAMERA_IMAGE)
		{
			DWORD32 dwImageType = 0;
			DWORD32 dwImageWidth = 0;
			DWORD32 dwImageHeight = 0;
			DWORD64 dw64ImageTime = 0;
			DWORD32 dwImageOffset = 0;
			DWORD32 dwDebugInfoLen = 0;
			ImageExtInfo cImageExtInfo = {0};
			ImageExtInfo cImageMERCURYExtInfo = {0};  //ˮ��ͼƬ������Ϣ
			unsigned char* pbTemp = pbInfo;
			char* pszMercuryExtInfo = new char[128*1024];
			memset(pszMercuryExtInfo, 0, 128*1024);
			
			DWORD32 dwXmlLen = strlen((const char*)pbInfo);
			unsigned char* pbXml = new unsigned char[dwXmlLen];
			if (pbXml)
				memcpy(pbXml, pbInfo, dwXmlLen);
			else
			{
				
				SAFE_DELETE_ARG(pszMercuryExtInfo);
				continue;
			}

			 TiXmlDocument cXmlDoc;
			 if (!cXmlDoc.Parse((const char*)pbXml))
			 {
				 SAFE_DELETE_ARG(pbXml);
				 SAFE_DELETE_ARG(pszMercuryExtInfo);
				 continue;
			 }


			//
			SAFE_DELETE_ARG(pbXml);
			const TiXmlElement* pRootElement = cXmlDoc.RootElement();
			if (NULL == pRootElement)
				continue;
			
			const TiXmlElement* pImageElement = pRootElement->FirstChildElement("Image");
			if (NULL == pImageElement)
				continue;
			
			const char* szType = pImageElement->Attribute("Type");
			const char* szWidth = pImageElement->Attribute("Width");
			const char* szHeight = pImageElement->Attribute("Height");
			const char* szTime = pImageElement->Attribute("Time");
			const char* szTimeHigh = pImageElement->Attribute("TimeHigh");
			const char* szTimeLow = pImageElement->Attribute("TimeLow");
			if (NULL == szType ||NULL == szWidth ||NULL == szHeight)
			{
				continue;
			}
			
			const char* szShutter = pImageElement->Attribute("Shutter");
			const char* szGain = pImageElement->Attribute("Gain");
			const char* szRGain = pImageElement->Attribute("r_Gain");
			const char* szGGain = pImageElement->Attribute("g_Gain");
			const char* szBGain = pImageElement->Attribute("b_Gain");

			cImageMERCURYExtInfo.iShutter = atoi(szShutter?szShutter:"0");
			cImageMERCURYExtInfo.iGain = atoi(szGain?szGain:"0");
			cImageMERCURYExtInfo.iGainR = atoi(szRGain?szRGain:"0");
			cImageMERCURYExtInfo.iGainG = atoi(szGGain?szGGain:"0");
			cImageMERCURYExtInfo.iGainB = atoi(szBGain?szBGain:"0");
			
			// ���˿�����Ϣ����
			const TiXmlElement* pDebugElement = pImageElement->FirstChildElement("DebugInfo");
			if (pDebugElement)
			{
				const char* szDebugInfoSize = pDebugElement->Attribute("DebugInfoSize");
				dwDebugInfoLen = atoi(szDebugInfoSize?szDebugInfoSize:"0");
			}

            char* pMercuryExtInfo = pszMercuryExtInfo+4; // ����4�����ֽ���Ϊ���λ?Ϊʲô��
			const TiXmlElement* pRectInfoElement = pImageElement->FirstChildElement("RectInfo");
			if( NULL != pRectInfoElement )
			{
				
				const char* szRectCount = pRectInfoElement->Attribute("Count");
				DWORD32 dwRectCount = atoi(szRectCount?szRectCount:"0");
				if( dwRectCount > 0 )
				{
					
					memcpy( pMercuryExtInfo ,&dwRectCount , sizeof(DWORD32));
					pMercuryExtInfo += sizeof(DWORD32);
					
					RECT cRectTemp;
					char szRectName[16];
                     for (int i = 0; i < dwRectCount; i++)
                      {
						  sprintf(szRectName, "Rect%d", i);
						  const TiXmlElement* pRectElement = pRectInfoElement->FirstChildElement(szRectName);
						  if (NULL == pRectElement)
                          {
							   break;
                           }
						  
						  const char* pBottom = pRectElement->Attribute("Bottom");
						  const char* pLeft = pRectElement->Attribute("Left");
						  const char* pRight = pRectElement->Attribute("Right");
						  const char* pTop = pRectElement->Attribute("Top");
						  
						  cRectTemp.bottom = atoi(pBottom?pBottom:"0");
						  cRectTemp.left = atoi(pLeft?pLeft:"0");
						  cRectTemp.right = atoi(pRight?pRight:"0");
						  cRectTemp.top = atoi(pTop?pTop:"0");
						  memcpy(pMercuryExtInfo ,&cRectTemp ,sizeof(RECT) );
						  pMercuryExtInfo+= sizeof(RECT);

                    }
                 }
				else if (0 == dwRectCount && dwDebugInfoLen > 0) // ���˿�����Ϣ����
                 {
                                    dwRectCount = 12345; // �������˿��ڱ�־
                                    memcpy( pMercuryExtInfo ,&dwRectCount , sizeof(DWORD32));
                                    pMercuryExtInfo += sizeof(DWORD32);
                                    memcpy( pMercuryExtInfo ,&dwDebugInfoLen , sizeof(DWORD32));
                                    pMercuryExtInfo += sizeof(DWORD32);
                                    memcpy( pMercuryExtInfo, pbInfo+dwXmlLen+1, dwDebugInfoLen);
                   }
			}
			
			if (strcmp(szType, "JPEG") == 0)
			{
								dwImageType = CAMERA_IMAGE_JPEG;
			}
			else if (strcmp(szType, "JPEG_CAPTURE") == 0) // TODO: ���豸Լ����
			{
								dwImageType = CAMERA_IMAGE_JPEG_CAPTURE;
			}
			else if (strcmp(szType, "JPEG_SLAVE") == 0)
			{
								dwImageType = CAMERA_IMAGE_JPEG_SLAVE;
			}
			else
			{
				continue;
			}
			
			dwImageWidth = atoi(szWidth);
			if (0 == dwImageWidth)
			{
								continue;
			}
			
			dwImageHeight = atoi(szHeight);
			if (0 == dwImageHeight)
			{
				continue;
			}
			
			DWORD dwTimeHigh = atoi(szTimeHigh ? szTimeHigh:"0");
			DWORD dwTimeLow = atoi(szTimeLow ? szTimeLow:"0");
			dw64ImageTime = (((DWORD64)dwTimeHigh)<<32) | dwTimeLow;
			CTime ctime(dw64ImageTime/1000);
			CString strImageTime;
			strImageTime.Format("%4d-%2d-%2d %2d:%2d:%2d", 
								ctime.GetYear(),
								ctime.GetMonth(),
								ctime.GetDay(),
								ctime.GetHour(),
								ctime.GetMinute(),
								ctime.GetSecond());


			DWORD dwType = IMAGE_TYPE_UNKNOWN;
			if ( CAMERA_IMAGE_JPEG == dwImageType )
			{
				dwType = IMAGE_TYPE_JPEG_NORMAL;
			}
			else if ( CAMERA_IMAGE_JPEG_CAPTURE == dwImageType )
			{
				dwType = IMAGE_TYPE_JPEG_CAPTURE;
			}
			else if ( CAMERA_IMAGE_JPEG_SLAVE == dwImageType )
			{
				dwType = IMAGE_TYPE_JPEG_LPR;
				char* pszImageExtInfo = new char[128*1024];
				sprintf(pszImageExtInfo, "Shutter:%d,Gain:%d,R:%d,G:%d,B:%d,FrameTime:%I64u",
									cImageMERCURYExtInfo.iShutter, cImageMERCURYExtInfo.iGain, cImageMERCURYExtInfo.iGainR,
									cImageMERCURYExtInfo.iGainG, cImageMERCURYExtInfo.iGainB, dw64ImageTime);
				//ProcLprImageFrame(pHHC, pbData, cInfoHeader.dwDataLen, dwImageOffset, pszMercuryExtInfo, pszImageExtInfo,  dw64ImageTime, dwImageWidth,  dwImageHeight);

				char* pszTemp = pszMercuryExtInfo;
				if ( pszTemp == NULL )
				{
					SAFE_DELETE_ARG(pszImageExtInfo);
					SAFE_DELETE_ARG(pszMercuryExtInfo);
				}

				int iPlateCount = 0;
				 pszTemp = pszTemp + 4; // ȥ��ǰ�����ڱ�ǵ�4�����ֽ�
				 memcpy(&iPlateCount, pszTemp, sizeof(DWORD32));

					// ��Ϊ���˿���
				if (12345 == iPlateCount)
				{
						pszTemp = pszTemp + 4; // ȥ����������(����)
						DWORD32 dwDebugInfoLen = 0;
						memcpy(&dwDebugInfoLen, pszTemp, 4);

						strcat(pszImageExtInfo, ",DebugInfo:");
						char* pTmpInfo = pszImageExtInfo + strlen(pszImageExtInfo);
						memcpy(pTmpInfo, pszTemp, dwDebugInfoLen+sizeof(DWORD32));
					
						if ( true)
						{
							CAutoLock sLock(& pContext->csJpegCall);
							if ( pContext->CallBackSet.pOnMJPEG != NULL  )
							{
								pContext->CallBackSet.pOnMJPEG(pContext->CallBackSet.pOnJpegParam
								,0 
								,IMAGE_TYPE_JPEG_LPR
								,dwImageWidth
								,dwImageHeight
								,dw64ImageTime
								,pbData+dwImageOffset
								,cInfoHeader.dwDataLen-dwImageOffset
								,pszImageExtInfo);
							}


						}		
				}


				SAFE_DELETE_ARG(pszImageExtInfo);
				SAFE_DELETE_ARG(pszMercuryExtInfo);
				continue;
			}

			char* pszImageExtInfo = new char[128*1024];
			if(pszImageExtInfo)
			{
				
				sprintf(pszImageExtInfo, "Shutter:%d,Gain:%d,R:%d,G:%d,B:%d,FrameTime:%I64u",
								cImageMERCURYExtInfo.iShutter, cImageMERCURYExtInfo.iGain, cImageMERCURYExtInfo.iGainR,
								cImageMERCURYExtInfo.iGainG, cImageMERCURYExtInfo.iGainB, dw64ImageTime);		
			}

			if ( true)
			{
				CAutoLock sLock(& pContext->csJpegCall); 

				if ( pContext->CallBackSet.pOnMJPEG != NULL  )
				{
					pContext->CallBackSet.pOnMJPEG(pContext->CallBackSet.pOnJpegParam
								,0 
								,dwType
								,dwImageWidth
								,dwImageHeight
								,dw64ImageTime
								,pbData+dwImageOffset
								,cInfoHeader.dwDataLen-dwImageOffset
								,pszImageExtInfo);
				}

			}
				
			SAFE_DELETE_ARG(pszImageExtInfo);
            SAFE_DELETE_ARG(pszMercuryExtInfo);

		}// if Image






	}//while end

	pContext->videoJpeg.fExit = true;
	return 0;
}


//�����µ����ݷ�����(H264 �� JPEG)
HRESULT SetNewDateServerClent(NetServer *pServer, SOCKET &clentSocket)
{
	if ( pServer == NULL )
		return E_FAIL;

	// �����ͷ
	CAMERA_CMD_HEADER  cCmdHeader;
	 //�豸�ͷ�����������Ϣ
	 if (RecvAll(clentSocket, (char*)&cCmdHeader, sizeof(cCmdHeader)) != sizeof(cCmdHeader))
	{
		return E_FAIL;
	}

	char *szInfo = new char[cCmdHeader.dwInfoSize+1];
	memset(szInfo, 0, cCmdHeader.dwInfoSize+1);
	if (RecvAll(clentSocket, szInfo, cCmdHeader.dwInfoSize) != cCmdHeader.dwInfoSize)
	{
		return E_FAIL;
	}

	//����XML �ĵ�
	char szDevSN[256] = { 0 };
	char szDateType[32] = { 0 };
	TiXmlDocument cXmlDoc;
	if ( cXmlDoc.Parse(szInfo))
	{
		const TiXmlElement* pRootElement = cXmlDoc.RootElement();
		if ( pRootElement != NULL && 0 == strcmp("1.0", pRootElement->Attribute("ver")) )
		{
			if ( 0 == strcmp("HvCmd", pRootElement->Value()) )
			{
					if(0 == strcmp("ConnectionRequest" ,  pRootElement->FirstChildElement("CmdName")->GetText()))
					{
						const TiXmlElement* pCmdArgElement = pRootElement->FirstChildElement("CmdArg");

						if ( pCmdArgElement != NULL)
						{
							GetParamString(pCmdArgElement , "SN" , szDevSN , 128);
							GetParamString(pCmdArgElement , "DateType" , szDateType , 128);
						}
					}
				}
		}
	}

	CAMERA_CMD_RESPOND cmdRespone;
	cmdRespone.dwID = CAMERA_XML_EXT_CMD;
	cmdRespone.dwInfoSize = 0;
	//cmdRespone.dwResult = S_OK == hr?0:-1;
	cmdRespone.dwResult = 0;

	int iSendLen = send(clentSocket , (char*)&cmdRespone , sizeof(cmdRespone) , 0);

	 HRESULT hr = iSendLen != sizeof(cmdRespone)?E_FAIL:S_OK;

	if (NULL !=szInfo)
	{
		delete[] szInfo;
		szInfo = NULL;
	}

	if ( hr != S_OK )
	{
		return E_FAIL;
	}

	
	CAutoLock sLock(&g_NetServer->csContext);   //�����豸����� ��ռ���ٽ�������ʹ��

	int index = FindExistAutoContext(szDevSN);
	if (index == -1)
		return E_FAIL;
	


	

	if ( strcmp(szDateType, "H264") == 0 )
	{
		if ( g_NetServer->pAutoContext[index]->videoH264.hThreadServer != NULL  &&  !g_NetServer->pAutoContext[index]->videoH264.fExit )
			return E_FAIL;

		ForceCloseSocket(g_NetServer->pAutoContext[index]->videoH264.sktVideo);
		HvSafeCloseThread(g_NetServer->pAutoContext[index]->videoH264.hThreadServer);

		g_NetServer->pAutoContext[index]->videoH264.sktVideo = clentSocket;
		g_NetServer->pAutoContext[index]->videoH264.fExit = false;
		g_NetServer->pAutoContext[index]->videoH264.hThreadServer= CreateThread(NULL, 0, HvH264ServThreadFunc, g_NetServer->pAutoContext[index], 0, NULL);
		if ( g_NetServer->pAutoContext[index]->videoH264.hThreadServer == NULL )
		{
			g_NetServer->pAutoContext[index]->videoH264.fExit = true;
			return E_FAIL;
		}

	}
	else
	{
		if ( g_NetServer->pAutoContext[index]->videoJpeg.hThreadServer != NULL  &&  !g_NetServer->pAutoContext[index]->videoJpeg.fExit )
			return E_FAIL;

		ForceCloseSocket(g_NetServer->pAutoContext[index]->videoJpeg.sktVideo);
		HvSafeCloseThread(g_NetServer->pAutoContext[index]->videoJpeg.hThreadServer);

		g_NetServer->pAutoContext[index]->videoJpeg.sktVideo = clentSocket;
		g_NetServer->pAutoContext[index]->videoJpeg.fExit = false;
		g_NetServer->pAutoContext[index]->videoJpeg.hThreadServer = CreateThread(NULL, 0, HvJpegServThreadFunc, g_NetServer->pAutoContext[index], 0, NULL);
		if ( g_NetServer->pAutoContext[index]->videoJpeg.hThreadServer == NULL )
		{
			g_NetServer->pAutoContext[index]->videoJpeg.fExit = true;
			return E_FAIL;
		}
	}

	return S_OK;
}


// ���������ļ����������ӵļ����߳�
static DWORD WINAPI HvDateServerThreadFunc(LPVOID lpParameter)
{
	if (lpParameter == NULL)
	{
		return 0;
	}


	NetServer *pServer = (NetServer*)lpParameter;

	int nWaitMs = 3000;
	int nTimeOutMs = 4000;

	SOCKET& hSocket = pServer->sktDateSvr;  //������·�׽���
	SOCKET hNewSocket = INVALID_SOCKET;

	while ( !pServer->fDateSvrStop && !g_fServerStop )  
	{
		// ����
		//Sleep(2000);
		//continue;

		hNewSocket = INVALID_SOCKET;
		HRESULT hr = HvAccept(hSocket, hNewSocket, nWaitMs);

		if (S_OK == hr)
		{
			// �����µ�����
			setsockopt(hNewSocket,SOL_SOCKET,SO_RCVTIMEO,(char *)&nTimeOutMs,sizeof(nTimeOutMs));
			setsockopt(hNewSocket,SOL_SOCKET,SO_SNDTIMEO,(char *)&nTimeOutMs,sizeof(nTimeOutMs));

			pServer->iDateLinkCnt++;

			if ( SetNewDateServerClent(pServer, hNewSocket) == S_OK )
			{
			}
			else
			{
				ForceCloseSocket(hNewSocket);
			}



		}

	}

	return 0;
}




HRESULT OpenDateLinkServer()
{
	if ( g_NetServer == NULL )
		return E_FAIL;

	SOCKET hSocket = HvCreateSocket();
	
	if ( INVALID_SOCKET == hSocket )
	 return E_FAIL;

	int nTimeOutMs = 2000;
	setsockopt(hSocket,SOL_SOCKET,SO_RCVTIMEO,(char *)&nTimeOutMs,sizeof(nTimeOutMs));

	if ( HvListen( hSocket, g_NetServer->iDatePort, 1 ) != S_OK )
	{
		ForceCloseSocket(hSocket);
		return E_FAIL;
	}

	//���������߳�
	g_NetServer->sktDateSvr = hSocket;
	g_NetServer->fDateSvrStop = false;

	g_NetServer->hThreadDatedSvr = CreateThread(NULL, 0, HvDateServerThreadFunc, g_NetServer, 0, NULL);
	if ( NULL == g_NetServer->hThreadDatedSvr )
	{
		g_NetServer->fDateSvrStop = true;
		CloseDateLinkServer();
		ForceCloseSocket(g_NetServer->sktDateSvr);
		return E_FAIL;
	}

	return S_OK;
}



HRESULT SetNewServerClent(NetServer *pServer, SOCKET &clentSocket)
{
	if ( pServer == NULL )
		return E_FAIL;


	// �����ͷ
	CAMERA_CMD_HEADER  cCmdHeader;
	 //�豸�ͷ�����������Ϣ
	 if (RecvAll(clentSocket, (char*)&cCmdHeader, sizeof(cCmdHeader)) != sizeof(cCmdHeader))
	{
		return E_FAIL;
	}

	char *szInfo = new char[cCmdHeader.dwInfoSize+1];
	memset(szInfo, 0, cCmdHeader.dwInfoSize+1);
	if (RecvAll(clentSocket, szInfo, cCmdHeader.dwInfoSize) != cCmdHeader.dwInfoSize)
	{
		return E_FAIL;
	}

	char szMac[32] = {"aa"};

	CAMERA_CMD_RESPOND cCmdRespond;
	cCmdRespond.dwID = CAMERA_CMD_DEVICEINFO;
	cCmdRespond.dwInfoSize = strlen(szMac);
	cCmdRespond.dwResult = 0;

	if( send(clentSocket, (char*)&cCmdRespond, sizeof(cCmdRespond), 0) != sizeof(cCmdRespond))
	{
		return E_FAIL;
	}

	if( send(clentSocket, szMac, strlen(szMac), 0) != strlen(szMac))
	{
		return E_FAIL;
	}

	//��ȡ�ͻ���IP ��ַ
	struct sockaddr_in sa;
	int len = sizeof(sa);
	int iPort = 0;
	char szAddr[32] = { 0 };
	if(!getpeername(clentSocket, (struct sockaddr *)&sa, &len)) 
	{
		iPort = (int)ntohs(sa.sin_port);
		sprintf(szAddr ,"%s", inet_ntoa(sa.sin_addr));
	}

	char szSN[1024] = { 0 };
	//�������յ�XML ��Ϣ
	TiXmlDocument cXmlDoc;
	if(cXmlDoc.Parse(szInfo) )
	{ 
		// �µ�����Э�飬��Ŀǰ������ˮ�ǡ�����
		const TiXmlElement* pRootElement = cXmlDoc.RootElement();
		if (NULL != pRootElement)
		{
			const TiXmlElement* pCmdArgElement = pRootElement->FirstChildElement("CmdName");
			if (NULL != pCmdArgElement)
			{
				//const char* szIP = pCmdArgElement->Attribute("IP");
				//const char* szMask = pCmdArgElement->Attribute("Mask");
				//const char* szGateway = pCmdArgElement->Attribute("Gateway");
				//const char* szMac = pCmdArgElement->Attribute("MAC");
				const char* szTempSN = pCmdArgElement->Attribute("SN");
				if ( szTempSN != NULL )
				{
					strcpy(szSN,szTempSN );
				}
				//const char* szMode = pCmdArgElement->Attribute("Mode");
					
			}
			
		}
				
	}

	//�����Ƿ��Ѿ����ڴ��豸�ľ���Ҳ������ˣ���ν�����ã���ָ�Ѿ��ص�����֪ͨ�ľ����
	//�����豸��Ų��Ҳ�������
	int index = -1;
	bool bfUsed = false;
	_HVAPI_AL_CONTEXT *pContext = NULL;
	if ( true )   //�����Ѵ��ڵĶ���������Ҫ�������
	{
		CAutoLock sLock(&pServer->csContext);
		
		index = FindExistAutoContext(szSN);

		if ( index != -1 )
		{
			pContext = g_NetServer->pAutoContext[index];

			EnterCriticalSection(&pContext->csSktCmd);

			if ( pContext->fCmdThExit && pContext->dwRef > 0 && ( pContext->eNotieType == OFFLINE || pContext->eNotieType == NONOTICE ))  //����ά���߳��Ѿ��˳��� && ���ò�Ϊ0 �����Ѿ������ߵ� ��˵����λ��ϣ��ʹ��ԭ�豸��ԭ������������豸��ʹ��ԭ���
			{
				//ɾ��ԭ�׽��ֺͶ˿�
				ReleaseDevContext(pContext); 
				bfUsed = true;

			}
			else
			{
				LeaveCriticalSection(&pContext->csSktCmd);
				return E_FAIL;
			}
			
		}
		else
		{
			//���ͷ�������Ϣ
			index = FindEmptyAutoContext();
		}	
	}

 
	if ( bfUsed )   //ʹ�������ľ��
	{
		pContext->sktCmd = clentSocket;
		pContext->fCmdThExit = false;
		strcpy(pContext->szAddr, szAddr);
		strcpy(pContext->szDeviceInfo, szInfo);
		pContext->iPort = iPort;
		pContext->eNotieType = NONOTICE; //��״̬����Ϊ����δ֪ͨ

		HANDLE	hThreadCmd = NULL;
		hThreadCmd = CreateThread(NULL, 0, HvCmdClientThreadFunc, pContext, 0, NULL);
		if ( hThreadCmd == NULL )
		{ 
			pContext->fCmdThExit = true;
		}

		pContext->hThreadCmd = hThreadCmd;

		LeaveCriticalSection(&pContext->csSktCmd);  

	
	}
	else  //�����µľ��
	{
		
		pContext = new  _HVAPI_AL_CONTEXT();

		pContext->sktCmd = clentSocket;
		pContext->fCmdThExit = false;

		strcpy(pContext->szAddr, szAddr);
		strcpy(pContext->szDeviceNo, szSN);
		strcpy(pContext->szDeviceInfo, szInfo);
		pContext->iPort = iPort;

		HANDLE	hThreadCmd = NULL;
		hThreadCmd = CreateThread(NULL, 0, HvCmdClientThreadFunc, pContext, 0, NULL);
		if ( hThreadCmd == NULL )
		{ 
			pContext->fCmdThExit = true;
			delete pContext;
			pContext = NULL;
			return E_FAIL;
		}

		pContext->hThreadCmd = hThreadCmd;
		g_NetServer->pAutoContext[index] = pContext;
	}

	return S_OK;
}



static DWORD WINAPI HvCmdServerThreadFunc(LPVOID lpParameter)
{
	if (lpParameter == NULL)
	{
		return 0;
	}

	NetServer *pServer = (NetServer*)lpParameter;

	int nWaitMs = 3000;
	int nTimeOutMs = 4000;

	SOCKET& hSocket = pServer->sktCmdSvr;
	SOCKET hNewSocket = INVALID_SOCKET;

	while ( !pServer->fCmdSvrStop && !g_fServerStop )
	{
		// ����
		hNewSocket = INVALID_SOCKET;
		HRESULT hr = HvAccept(hSocket, hNewSocket, nWaitMs);

		if (S_OK == hr)
		{
			//ForceCloseSocket(hNewSocket);
			//continue;
			 
			// �����µ�����
			setsockopt(hNewSocket,SOL_SOCKET,SO_RCVTIMEO,(char *)&nTimeOutMs,sizeof(nTimeOutMs));
			setsockopt(hNewSocket,SOL_SOCKET,SO_SNDTIMEO,(char *)&nTimeOutMs,sizeof(nTimeOutMs));

			pServer->iLinkCnt++;


			
			if ( SetNewServerClent(pServer, hNewSocket) == S_OK )
			{
			}
			else
			{
				ForceCloseSocket(hNewSocket);
			}



		}

	}

	return 0;
}




HV_API_AL HRESULT HVAPI_AL_OpenDevCtrlServer(int iPort, int iMaxCount)
{
	if ( g_NetServer == NULL )
		return E_FAIL;

	HVAPI_AL_CloseDevCtrlServer();

	//�ȴ�������·����
	//��������·�����Ƿ����������������ܿ�����·�Ŀ��ƣ�������Ʒ���һ����
	g_NetServer->iMaxDateLinkCnt = iMaxCount;
	g_NetServer->iDatePort = iPort-1;

	if ( OpenDateLinkServer() != S_OK )
	{
		return E_FAIL;
	}

	SOCKET hSocket = HvCreateSocket();
	
	if ( INVALID_SOCKET == hSocket )
	 return E_FAIL;

	int nTimeOutMs = 2000;
	setsockopt(hSocket,SOL_SOCKET,SO_RCVTIMEO,(char *)&nTimeOutMs,sizeof(nTimeOutMs));

	if ( HvListen( hSocket, iPort, 1 ) != S_OK )
	{
		ForceCloseSocket(hSocket);
		return E_FAIL;
	}

	//���������߳�
	g_NetServer->iMaxCnt = iMaxCount;
	g_NetServer->iCmdPort = iPort;
	g_NetServer->sktCmdSvr = hSocket;
	g_NetServer->fCmdSvrStop = false;

	g_NetServer->hThreadCmdSvr = CreateThread(NULL, 0, HvCmdServerThreadFunc, g_NetServer, 0, NULL);
	if ( NULL ==g_NetServer->hThreadCmdSvr )
	{
		CloseDateLinkServer();
		g_NetServer->fCmdSvrStop = true;
		ForceCloseSocket(g_NetServer->sktCmdSvr);

		return E_FAIL;
	}

	return S_OK;
}


HV_API_AL HRESULT HVAPI_AL_CloseDevCtrlServer()
{
	if ( g_NetServer == NULL )
		return S_OK;

	g_NetServer->fCmdSvrStop = true;
	g_NetServer->fDateSvrStop = true;

	CloseDateLinkServer();
	
	HvSafeCloseThread(g_NetServer->hThreadCmdSvr);
	ForceCloseSocket(g_NetServer->sktCmdSvr);

	//�Ͽ������豸��

	//�ص��Ѿ��Ͽ�������
	
	for(int i=0; i<MAX_ATUO_CONTEXT; i++)
	{
		if ( g_NetServer->pAutoContext[i] != NULL )
		{
			g_NetServer->pAutoContext[i]->CallBackSet.pOnH264Param = NULL;
			g_NetServer->pAutoContext[i]->CallBackSet.pOnHistroyH264Param = NULL;
			g_NetServer->pAutoContext[i]->CallBackSet.pOnRecoreH264HistoryVideo = NULL;
			g_NetServer->pAutoContext[i]->CallBackSet.pOnRecoreH264Video = NULL;
			g_NetServer->pAutoContext[i]->CallBackSet.pOnJpegParam = NULL;
			g_NetServer->pAutoContext[i]->CallBackSet.pOnMJPEG = NULL;

		}
	}

	return S_OK;
}


HV_API_AL HRESULT HVAPI_AL_GetDevConfigInfo(HVAPI_OPERATE_HANDLE hHandle, LPSTR szXmlParam, INT nBufLen, INT* pnRetLen)
{

	if ( NULL == hHandle || NULL == szXmlParam )
	{
		return E_FAIL;
	}


	_HVAPI_AL_CONTEXT* pAutoContext = (_HVAPI_AL_CONTEXT*)hHandle;


	const int iBufLen = (1024 << 10);
	char* rgchXmlParamBuf = new char[iBufLen];  // 1MB
	if(rgchXmlParamBuf == NULL)
	{
		return E_FAIL;
	}
	memset(rgchXmlParamBuf, 0, sizeof(rgchXmlParamBuf));


	CAMERA_CMD_HEADER cCmdHeader;
	CAMERA_CMD_RESPOND cCmdRespond;
	
	cCmdHeader.dwID = CAMERA_GET_PARAM_CMD;
	cCmdHeader.dwInfoSize = 0;

	int iTryTimes = 0;
	while(pAutoContext->fBusySktCmd && iTryTimes> 10 )
	{
		Sleep(200);
	}


	CAutoLock sLock(&pAutoContext->csSktCmd);
	pAutoContext->fBusySktCmd = true;

	if ( sizeof(cCmdHeader) != send( pAutoContext->sktCmd, (const char*)&cCmdHeader, sizeof(cCmdHeader), 0) )
	{
		SAFE_DELETE_ARG(rgchXmlParamBuf);

		pAutoContext->fBusySktCmd = false;

		return E_FAIL;
	}

	if (  recv(pAutoContext->sktCmd, (char*)&cCmdRespond, sizeof(cCmdRespond), 0) !=  sizeof(cCmdRespond))
	{
		pAutoContext->fBusySktCmd = false;
		SAFE_DELETE_ARG(rgchXmlParamBuf);
		return E_FAIL;
	}

	if ( CAMERA_GET_PARAM_CMD != cCmdRespond.dwID  ||  0 != cCmdRespond.dwResult )
	{
		pAutoContext->fBusySktCmd = false;
		SAFE_DELETE_ARG(rgchXmlParamBuf);
		return E_FAIL;
	}
	
	if ( cCmdRespond.dwInfoSize == RecvAll(pAutoContext->sktCmd, rgchXmlParamBuf, cCmdRespond.dwInfoSize) )
	{
		pAutoContext->dwCmdTick = GetTickCount();
		pAutoContext->fBusySktCmd = false;

		if ( nBufLen > (int)cCmdRespond.dwInfoSize )
		{
			if ( pnRetLen )
				*pnRetLen = cCmdRespond.dwInfoSize;

			memcpy(szXmlParam, rgchXmlParamBuf, cCmdRespond.dwInfoSize);
			SAFE_DELETE_ARG(rgchXmlParamBuf);
			return S_OK;
		}
		else
		{
			if ( pnRetLen )
				*pnRetLen = cCmdRespond.dwInfoSize;
			SAFE_DELETE_ARG(rgchXmlParamBuf);
			
			return E_FAIL;
		}
	}

	SAFE_DELETE_ARG(rgchXmlParamBuf);
	return E_FAIL;
}

HV_API_AL HRESULT HVAPI_AL_UploadDevConfigInfo(HVAPI_OPERATE_HANDLE hHandle, LPCSTR szParamDoc)
{
	if ( NULL == hHandle || NULL == szParamDoc )
	{
		return E_FAIL;
	}


	_HVAPI_AL_CONTEXT* pAutoContext = (_HVAPI_AL_CONTEXT*)hHandle;

	int iTryTimes = 0;
	while(pAutoContext->fBusySktCmd && iTryTimes> 10 )
	{
		Sleep(200);
	}


	CAutoLock sLock(&pAutoContext->csSktCmd);
	pAutoContext->fBusySktCmd = true;


	CAMERA_CMD_HEADER cCmdHeader;
	CAMERA_CMD_RESPOND cCmdRespond;
	
	
	cCmdHeader.dwID = CAMERA_SET_PARAM_CMD;
	cCmdHeader.dwInfoSize = (int)strlen(szParamDoc)+1;
	int iReciveTimeOutMS = 60000;
	setsockopt(pAutoContext->sktCmd, SOL_SOCKET, SO_RCVTIMEO, (char*)&iReciveTimeOutMS, sizeof(iReciveTimeOutMS));

	if ( sizeof(cCmdHeader) != send(pAutoContext->sktCmd, (const char*)&cCmdHeader, sizeof(cCmdHeader), 0) )
	{
		pAutoContext->fBusySktCmd = false;
		return E_FAIL;
	}
	
	if ( cCmdHeader.dwInfoSize != send(pAutoContext->sktCmd, (const char*)szParamDoc, cCmdHeader.dwInfoSize, 0) )
	{
		pAutoContext->fBusySktCmd = false;
		return E_FAIL;
	}
	
	if ( sizeof(cCmdRespond) == recv(pAutoContext->sktCmd, (char*)&cCmdRespond, sizeof(cCmdRespond), 0) )
	{
		if ( CAMERA_SET_PARAM_CMD != cCmdRespond.dwID && 0 != cCmdRespond.dwResult )
		{
			pAutoContext->fBusySktCmd = false;
			return E_FAIL;
		}
	}

	iReciveTimeOutMS = 4000;
	setsockopt(pAutoContext->sktCmd, SOL_SOCKET, SO_RCVTIMEO, (char*)&iReciveTimeOutMS, sizeof(iReciveTimeOutMS));

	pAutoContext->dwCmdTick = GetTickCount();
	pAutoContext->fBusySktCmd = false;

	return S_OK;

	
}

HV_API_AL HRESULT HVAPI_AL_DisConnect(HVAPI_OPERATE_HANDLE hHandle)
{
	if(hHandle == NULL)
	{
		return E_FAIL;
	}

	_HVAPI_AL_CONTEXT* pAutoContext = (_HVAPI_AL_CONTEXT*)hHandle;
	pAutoContext->fCmdThExit = true;
	return S_OK;
}






HV_API_AL HRESULT CDECL HVAPI_AL_ExecCmdOnDev(HVAPI_OPERATE_HANDLE hHandle,LPCSTR szCmd,LPSTR szRetBuf,INT nBufLen,INT* pnRetLen)
{
	if(hHandle == NULL || szCmd == NULL
		|| szRetBuf == NULL || nBufLen <= 0)
	{
		return E_FAIL;
	}

	_HVAPI_AL_CONTEXT* pAutoContext = (_HVAPI_AL_CONTEXT*)hHandle;

	const int iBufLen = (1024 << 4);
	char* pszXmlBuf = new char[iBufLen];
	if(pszXmlBuf == NULL)
	{
		return E_FAIL;
	}
	memset(pszXmlBuf, 0, iBufLen);

	int iXmlBufLen = iBufLen;

	// ��szCmd��֯XML����
	TiXmlDocument cXmlDoc;
	HRESULT hr = S_OK;
	bool fIsXml = cXmlDoc.Parse(szCmd);

	if (!fIsXml)
	{
		hr = HvMakeXmlCmdByString3( szCmd, (int)strlen(szCmd), pszXmlBuf, iXmlBufLen);
	}
			
	if ( S_OK != hr )
	{
		SAFE_DELETE_ARG(pszXmlBuf);
		return E_FAIL;
	}

	if (!fIsXml)
	{
		szCmd = (char*)pszXmlBuf;
	}

	int iTryTimes = 0;
	while(pAutoContext->fBusySktCmd && iTryTimes> 10 )
	{
		Sleep(200);
	}


	CAutoLock sLock(&pAutoContext->csSktCmd);
	pAutoContext->fBusySktCmd = true;
	
	/*
	bool fIsOk = false;
	cCmdHeader.dwID = CAMERA_XML_EXT_CMD;
	cCmdHeader.dwInfoSize = (int)strlen(szXmlCmd)+1;
    // ����֪ͨ��Ϣ
	if ( sizeof(cCmdHeader) != send(pAutoContext->sktCmd, (const char*)&cCmdHeader, sizeof(cCmdHeader), 0) )
	{
		fIsOk = false;
		pAutoContext->fBusySktCmd = false;
		SAFE_DELETE_ARG(pszXmlBuf);
		return E_FAIL;
	}
    // ��������
	if ( cCmdHeader.dwInfoSize != send(pAutoContext->sktCmd, (const char*)szXmlCmd, cCmdHeader.dwInfoSize, 0) )
	{
		SAFE_DELETE_ARG(pszXmlBuf);
		pAutoContext->fBusySktCmd = false;
		fIsOk = false;
		return E_FAIL;
		
	}
    // ����
	if ( sizeof(cCmdRespond) == recv(pAutoContext->sktCmd, (char*)&cCmdRespond, sizeof(cCmdRespond), 0) )
	{
		if ( CAMERA_XML_EXT_CMD == cCmdRespond.dwID&& 0 == cCmdRespond.dwResult && 0 < cCmdRespond.dwInfoSize )
		{
			int iRecvLen = RecvAll(hSocketCmd, pszTmpXmlBuf, cCmdRespond.dwInfoSize);
			if ( cCmdRespond.dwInfoSize ==  iRecvLen)
			{
				if ( iBufLen > (int)cCmdRespond.dwInfoSize )
				{
					fIsOk = true;
				}
				else
				{
					iBufLen = cCmdRespond.dwInfoSize;
					fIsOk = false;
				}
			}
			else
			{
				iBufLen = 0;
				fIsOk = false;
			}
		}
		fIsOk = true;
	}


	if ( fIsOK )
	*/
	




	// ִ������
	bool fRet = false;
	fRet = ExecXmlExtCmdMercury(NULL, (char*)szCmd, (char*)szRetBuf, nBufLen, pAutoContext->sktCmd);
	
	if ( NULL != pnRetLen )
	{
		*pnRetLen = nBufLen;
	}

	pAutoContext->dwCmdTick = GetTickCount();
	pAutoContext->fBusySktCmd = false;



	SAFE_DELETE_ARG(pszXmlBuf);

	return (true == fRet) ? S_OK : E_FAIL;

}

HV_API_AL HRESULT CDECL HVAPI_AL_GetPCSHumanTrafficInfo(HVAPI_OPERATE_HANDLE hHandle, DWORD64 dw64StartTime, DWORD64 dw64EndTime, CHAR* szRetInfo, INT* iLen)
{
	if (hHandle == NULL || NULL == szRetInfo)
        return E_FAIL;

    _HVAPI_AL_CONTEXT* pAutoContext = (_HVAPI_AL_CONTEXT*)hHandle;

	DWORD64 dw64TimeTemp = dw64EndTime - dw64StartTime;
	if (dw64StartTime > dw64EndTime || dw64TimeTemp < 0 || dw64TimeTemp > (1000 * 60 * 60 *24 * 31))
	{
		*iLen = 0; 
		return S_FALSE;
	}

    time_t dwStartTime = (time_t)(dw64StartTime / 1000);
    struct tm *tmStart = localtime(&dwStartTime);
	// ���ε���localtime�����������Ż���ֻ��һ���ڴ�ռ䣬��һ�εĻᱻ��һ�θ�ˢ��
	struct tm tmTemp;
	tmTemp.tm_year = tmStart->tm_year;
	tmTemp.tm_mon = tmStart->tm_mon;
	tmTemp.tm_mday = tmStart->tm_mday;
	tmTemp.tm_hour = tmStart->tm_hour;
	tmTemp.tm_min = tmStart->tm_min;

    time_t dwEndTime = (time_t)(dw64EndTime / 1000);
    struct tm *tmEnd = localtime(&dwEndTime);

	char chTime[512] = {0};
	sprintf(chTime, "StartTime:[%04d-%02d-%02d %02d:%02d];EndTime:[%04d-%02d-%02d %02d:%02d]",
		tmTemp.tm_year + 1900, tmTemp.tm_mon + 1, tmTemp.tm_mday, tmTemp.tm_hour, tmTemp.tm_min,
		tmEnd->tm_year + 1900, tmEnd->tm_mon + 1, tmEnd->tm_mday, tmEnd->tm_hour, tmEnd->tm_min);

    char szCmd[32] = {"GetPCSFlow"};
    char szCmdXml[1024] = { 0 };

    if (HvGetXmlOfStringType(szCmd, "GETTER", chTime, szCmdXml) == 0)
    {
        return E_FAIL;
    }

    int iRetXmlLen = (1024<<10)*2 + 1024;
    char *szRetXml = new char[iRetXmlLen];
    if (szRetXml == NULL)
    {
        return E_FAIL;
    }

    ExecXmlExtCmdMercury(NULL, szCmdXml, szRetXml, iRetXmlLen, pAutoContext->sktCmd);

    // ���������
    INT nRetCode = -1;
    CXmlParseInfo rgRetInfo[1];
    strcpy( rgRetInfo[0].szKeyName, "RetCode");

    if ( S_OK != HvXmlParseMercury(szCmd, szRetXml, iRetXmlLen, rgRetInfo, 1 ) )
    {
       // WrightLogEx(pHHC ->szIP , "GetPCSFlow  HvXmlParse Fail !\n");
        delete[] szRetXml;
        szRetXml = NULL;
        return E_FAIL ;
    }
    nRetCode = atoi(rgRetInfo[0].szKeyValue);

	int iRet = -1;
    if (nRetCode != 0)
    {
        delete[] szRetXml;
        szRetXml = NULL;
        return E_FAIL;
    }
    else
    {
        int iTempLen = (1024<<10) * 2;
        char *szRetTemp = new char[iTempLen];
        if ( szRetTemp == NULL )
        {
            delete[] szRetXml;
            szRetXml = NULL;
            return E_FAIL;
        }
        HvGetRetMsgFromXml(szRetXml, szRetTemp);

        int iRetLen = (int)strlen(szRetTemp);
        if (iRetLen <= *iLen)
        {
            strncpy(szRetInfo, szRetTemp, iRetLen);
            *iLen = iRetLen;
			iRet = 0;
        }
		else 
		{
			*iLen = iRetLen;
			iRet = -1;
		}

		if (szRetTemp != NULL)
		{
			delete[] szRetTemp;
			szRetTemp = NULL;
		}
    }

    if (szRetXml != NULL)
    {
        delete[] szRetXml;
        szRetXml = NULL;
    }

	if (-1 == iRet)
	{
		return S_FALSE;
	}
	else
	{
		return S_OK;
	}
}



HV_API_AL HRESULT CDECL HVAPI_AL_GetCaptureImage(HVAPI_OPERATE_HANDLE hHandle,int nTimeM, BYTE *pImageBuffLen, int nBuffLen, int *iImageLen,DWORD64 *dwTime,  DWORD *dwImageWidth, DWORD *dwImageHeigh)
{
	if ( NULL == hHandle || NULL == pImageBuffLen )
	{
		return E_FAIL;
	}
	
	if ( nTimeM < 0 || nTimeM>60)
		return E_FAIL;



	_HVAPI_AL_CONTEXT* pAutoContext = (_HVAPI_AL_CONTEXT*)hHandle;


	CAMERA_CMD_HEADER cCmdHeader;
	CAMERA_CMD_RESPOND cCmdRespond;
	
	cCmdHeader.dwID = CAMERA_CAPTURE_IMAGE;
	cCmdHeader.dwInfoSize = 4;

	int iTryTimes = 0;
	while(pAutoContext->fBusySktCmd && iTryTimes> 10 )
	{
		Sleep(200);
	}


	CAutoLock sLock(&pAutoContext->csSktCmd);

	pAutoContext->fBusySktCmd = true;

	if ( sizeof(cCmdHeader) != send( pAutoContext->sktCmd, (const char*)&cCmdHeader, sizeof(cCmdHeader), 0) )
	{
		pAutoContext->fBusySktCmd = false;
		return E_FAIL;
	}

	if ( sizeof(int) != send( pAutoContext->sktCmd, (const char*)&nTimeM, sizeof(int), 0) )
	{
		pAutoContext->fBusySktCmd = false;
		return E_FAIL;
	}


	if (  recv(pAutoContext->sktCmd, (char*)&cCmdRespond, sizeof(cCmdRespond), 0) !=  sizeof(cCmdRespond))
	{
		pAutoContext->fBusySktCmd = false;
		return E_FAIL;
	}

	if ( CAMERA_CAPTURE_IMAGE != cCmdRespond.dwID  ||  0 != cCmdRespond.dwResult )
	{
		pAutoContext->fBusySktCmd = false;
		
		return E_FAIL;
	}
	
	BYTE *pBuffTemp = new BYTE[cCmdRespond.dwInfoSize+1];
	memset(pBuffTemp, 0, cCmdRespond.dwInfoSize+1);


	struct ImgeInfo
	{
		int iSize;
		DWORD dwHighTime;
		DWORD dwLowTime;
		DWORD dwWidth;
		DWORD dwHeigh;
	}srtImageInfo;

	memset(&srtImageInfo, 0,sizeof(srtImageInfo));

	if ( cCmdRespond.dwInfoSize == RecvAll(pAutoContext->sktCmd, (char*)pBuffTemp, cCmdRespond.dwInfoSize) )
	{
		pAutoContext->dwCmdTick = GetTickCount();
		pAutoContext->fBusySktCmd = false;

		if ( nBuffLen > (int)cCmdRespond.dwInfoSize-20)
		{
		
			if ( iImageLen )
				*iImageLen = cCmdRespond.dwInfoSize-20;

			if ( *iImageLen <=0  )
			{
				SAFE_DELETE_ARG(pBuffTemp);
				return E_FAIL;
			}
			
			memcpy(&srtImageInfo, pBuffTemp, sizeof(srtImageInfo));

			if ( dwTime != NULL  )
			{
				DWORD64 dwTempTime = srtImageInfo.dwHighTime;
				dwTempTime = (dwTempTime <<32) | srtImageInfo.dwLowTime;
				*dwTime = dwTempTime;
			}

			if ( dwImageWidth != NULL )
			{
				*dwImageWidth = srtImageInfo.dwWidth;
			}
			if ( dwImageHeigh != NULL )
				*dwImageHeigh = srtImageInfo.dwHeigh;

			memcpy(pImageBuffLen, pBuffTemp+20, cCmdRespond.dwInfoSize-20);
			SAFE_DELETE_ARG(pBuffTemp);
			return S_OK;
		}
		else
		{
			if ( iImageLen )
				*iImageLen = cCmdRespond.dwInfoSize-12;

			SAFE_DELETE_ARG(pBuffTemp);
			
			return E_FAIL;
		}
	}
	else
	{
		pAutoContext->fBusySktCmd = false;
		pAutoContext->fCmdThExit = true; 
	}

	SAFE_DELETE_ARG(pBuffTemp);
	return E_FAIL;
}



HV_API_AL HRESULT CDECL HVAPI_AL_StartRecvH264Video(
	HVAPI_OPERATE_HANDLE hHandle,
	PVOID pFunc,
	PVOID pUserData,
	INT iVideoID ,       
	DWORD64 dw64BeginTimeMS,
	DWORD64 dw64EndTimeMS,
	DWORD dwRecvFlag
)
{
	
	if ( hHandle == NULL || pFunc == NULL  )
	{
		return E_FAIL;

	}
	
	_HVAPI_AL_CONTEXT* pAutoContext = (_HVAPI_AL_CONTEXT*)hHandle;

	if (  true )
	{
		CAutoLock sLock(& pAutoContext->csH264Call); 
		pAutoContext->videoH264.dwBeginTime = dw64BeginTimeMS;
		pAutoContext->videoH264.dwEndTime = dw64EndTimeMS;
		if ( H264_RECV_FLAG_REALTIME ==  dwRecvFlag )
		{
			pAutoContext->videoH264.fHistoryEnable = false;
			pAutoContext->CallBackSet.pOnRecoreH264Video = ( HVAPI_AL_CALLBACK_H264)pFunc;
			pAutoContext->CallBackSet.pOnH264Param = pUserData;
		}
		else if (H264_RECV_FLAG_HISTORY == dwRecvFlag )
		{
			pAutoContext->videoH264.fHistoryEnable = true;
			pAutoContext->CallBackSet.pOnHistroyH264Param =pUserData;
			pAutoContext->CallBackSet.pOnRecoreH264HistoryVideo = (HVAPI_AL_CALLBACK_H264)pFunc;
		}
		else
		{
			pAutoContext->videoH264.fHistoryEnable = false;
			pAutoContext->CallBackSet.pOnRecoreH264Video = (HVAPI_AL_CALLBACK_H264)pFunc;
			pAutoContext->CallBackSet.pOnH264Param = pUserData;
		}
		pAutoContext->videoH264.fExit = true;

	}


	//��װXML ����
	char szCmd[32] = {"StartAutoH264"};
	char szClass[128] = {"SETTER"};
	char szValue[1024] = {"111"};
	char szXmlBuff[2048] = { 0 };

	int iBuffLen = HvGetXmlOfStringType(szCmd,szClass, szValue, szXmlBuff);

	int iRetLen = 1024;
	char szRetBuff[1024] = {0 };

	if ( HVAPI_AL_ExecCmdOnDev(hHandle,szXmlBuff,szRetBuff,iRetLen,&iRetLen) != S_OK )
	{
		return E_FAIL;
	}

	return S_OK;

}


HV_API_AL HRESULT CDECL  HVAPI_AL_StopRecvH264Video(HVAPI_OPERATE_HANDLE hHandle)
{
	if ( hHandle == NULL )
		return E_FAIL;

	
	_HVAPI_AL_CONTEXT* pAutoContext = (_HVAPI_AL_CONTEXT*)hHandle;

	//��װXML ����
	char szCmd[32] = {"StopAutoH264"};
	char szClass[128] = {"SETTER"};
	char szValue[1024] = {"111"};
	char szXmlBuff[2048] = { 0 };

	int iBuffLen = HvGetXmlOfStringType(szCmd,szClass, szValue, szXmlBuff);

	int iRetLen = 1024;
	char szRetBuff[1024] = {0 };
	if ( HVAPI_AL_ExecCmdOnDev(hHandle,szXmlBuff,szRetBuff,iRetLen,&iRetLen) != S_OK )
	{
		return E_FAIL;
	}
	
	//�˳������̣߳� ͬʱ���ûص�����Ϊ��
	pAutoContext->videoH264.fExit = true;
	CAutoLock sLock(& pAutoContext->csH264Call);   //�����豸����� ��ռ���ٽ�������ʹ��
	pAutoContext->CallBackSet.pOnRecoreH264Video = NULL;
	pAutoContext->CallBackSet.pOnH264Param = NULL;
	pAutoContext->CallBackSet.pOnHistroyH264Param = NULL;
	pAutoContext->CallBackSet.pOnRecoreH264HistoryVideo = NULL;

	return S_OK;

}



HV_API_AL HRESULT CDECL HVAPI_AL_StartRecvMJPEG(
					 HVAPI_OPERATE_HANDLE hHandle,
					 PVOID pFunc,
					 PVOID pUserData,
					 INT iVideoID ,
					 DWORD dwRecvFlag
					 )
{

	if ( hHandle == NULL || pFunc == NULL  )
	{
		return E_FAIL;

	}
	
	_HVAPI_AL_CONTEXT* pAutoContext = (_HVAPI_AL_CONTEXT*)hHandle;

	if (  true )
	{

		CAutoLock sLock(& pAutoContext->csJpegCall); 
		pAutoContext->videoJpeg.fExit = true;
		pAutoContext->videoJpeg.dwRecvType = dwRecvFlag;
		pAutoContext->CallBackSet.pOnMJPEG = ( HVAPI_AL_CALLBACK_H264)pFunc;
		pAutoContext->CallBackSet.pOnJpegParam = pUserData;
	}

	//��װXML ����
	char szCmd[32] = {"StartAutoJPEG"};
	char szClass[128] = {"SETTER"};
	char szValue[1024] = {"111"};
	char szXmlBuff[2048] = { 0 };

	int iBuffLen = HvGetXmlOfStringType(szCmd,szClass, szValue, szXmlBuff);

	int iRetLen = 1024;
	char szRetBuff[1024] = {0 };

	if ( HVAPI_AL_ExecCmdOnDev(hHandle,szXmlBuff,szRetBuff,iRetLen,&iRetLen) != S_OK )
	{
		return E_FAIL;
	}

	return S_OK;

}



HV_API_AL HRESULT CDECL  HVAPI_AL_StopRecvMJPEG(HVAPI_OPERATE_HANDLE hHandle)
{
	if ( hHandle == NULL )
		return E_FAIL;

	_HVAPI_AL_CONTEXT* pAutoContext = (_HVAPI_AL_CONTEXT*)hHandle;

	//��װXML ����
	char szCmd[32] = {"StopAutoJPEG"};
	char szClass[128] = {"SETTER"};
	char szValue[1024] = {"111"};
	char szXmlBuff[2048] = { 0 };

	int iBuffLen = HvGetXmlOfStringType(szCmd,szClass, szValue, szXmlBuff);

	int iRetLen = 1024;
	char szRetBuff[1024] = {0 };
	if ( HVAPI_AL_ExecCmdOnDev(hHandle,szXmlBuff,szRetBuff,iRetLen,&iRetLen) != S_OK )
	{
		return E_FAIL;
	}
	
	//�˳������̣߳� ͬʱ���ûص�����Ϊ��

	pAutoContext->videoJpeg.fExit = true;
	CAutoLock sLock(&pAutoContext->csJpegCall);   //�����豸����� ��ռ���ٽ�������ʹ��
	pAutoContext->CallBackSet.pOnMJPEG = NULL;
	pAutoContext->CallBackSet.pOnJpegParam = NULL;

	return S_OK;
}

HV_API_AL HRESULT HVAPI_AL_AddRef(HVAPI_OPERATE_HANDLE handle)
{
	if(handle == NULL)
	{
		return E_FAIL;
	}
	_HVAPI_AL_CONTEXT* pAutoContext = (_HVAPI_AL_CONTEXT*)handle;

	CAutoLock sLock(&pAutoContext->csRef);   //�������ã� ��ռ���ٽ�������ʹ��
	pAutoContext->dwRef++;

	return S_OK;
}


HV_API_AL HRESULT HVAPI_AL_ReleaseRef(HVAPI_OPERATE_HANDLE handle)
{
	if(handle == NULL)
	{
		return E_FAIL;
	}



	_HVAPI_AL_CONTEXT* pAutoContext = (_HVAPI_AL_CONTEXT*)handle;

	CAutoLock sLock(&pAutoContext->csRef);   //�������ã� ��ռ���ٽ�������ʹ��

	pAutoContext->dwRef--;

	if ( pAutoContext->dwRef < 0 )
		pAutoContext->dwRef = 0;

	return S_OK;
		
}


//��ȡ��·״̬
HV_API_AL HRESULT CDECL HVAPI_AL_GetDevLinkStatus(HVAPI_OPERATE_HANDLE handle, DWORD dwLinkType, DWORD *dwStatus)
{
	if(handle == NULL && dwStatus == NULL )
	{
		return E_FAIL;
	}

	_HVAPI_AL_CONTEXT* pAutoContext = (_HVAPI_AL_CONTEXT*)handle;

	if ( AL_LINK_TYPE_JPEG_VIDEO == dwLinkType)
	{
		if ( pAutoContext->CallBackSet.pOnMJPEG != NULL)   //���غ�����Ϊ�գ�˵���ϲ������������Ƶ����
		{
			if (  pAutoContext->videoJpeg.fExit )  // �����������̬�⸺���������ʷ�������״̬���豸
			{
				*dwStatus = AL_LINK_STATE_RECONN;  // ����״̬
			}
			else
			{
				*dwStatus = AL_LINK_STATE_NORMAL;  //����״̬
			}
		}
		else
		{
			*dwStatus = AL_LINK_STATE_DISCONN;
		}


	}
	else if ( AL_LINK_TYPE_H264_VIDEO ==dwLinkType )
	{
		if (  (pAutoContext->CallBackSet.pOnRecoreH264Video != NULL ) || (pAutoContext->CallBackSet.pOnH264Param ))   //���غ�����Ϊ�գ�˵���ϲ������������Ƶ����
		{
			if (  pAutoContext->videoH264.fExit )  // �����������̬�⸺���������ʷ�������״̬���豸
			{
				*dwStatus = AL_LINK_STATE_RECONN;  // ����״̬
			}
			else
			{
				*dwStatus = AL_LINK_STATE_NORMAL;  //����״̬
			}
		}
		else
		{
			*dwStatus = AL_LINK_STATE_DISCONN;
		}
	}
	else
	{
		*dwStatus = AL_LINK_STATE_UNKNOWN;
	}

	return S_OK;

}












