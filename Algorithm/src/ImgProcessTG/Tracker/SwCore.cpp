#include "SwCore.h"

#include "DspLinkCmd.h"
#include "hvmodelstore.h"
#include "tracker.h"
#include "trackerdef.h"

//<DSP����ʹ��
char* g_pDspDebugInfo = NULL;  // ֱ��ָ�����ڴ�
int g_nDspDebugInfoLen = 0;    // Log����

int Dsp_DebugInfo(char* szfmt, ...)		//�ú�����ProcessOneFrame�е��ò������
{
//	if ( !IsDebugDeviceEnabled(iDeviceID) ) return 0;

	static char s_szbuf[1024];
	static char* pReWritePtr = NULL;
	 
	va_list arglist;
	int iRetVal = 0;

	s_szbuf[0] = 0;

	va_start( arglist, szfmt );
	iRetVal = std::vsnprintf(s_szbuf, sizeof(s_szbuf) - 1, szfmt, arglist);
	va_end( arglist );
	
	int nAddLen = strlen(s_szbuf);
	if (pReWritePtr != NULL && s_szbuf[0] == '@')
	{
		int nLastLen = strlen(pReWritePtr);
		g_nDspDebugInfoLen -= nLastLen;
	}
	if (s_szbuf[0] == '@')
		pReWritePtr = g_pDspDebugInfo + g_nDspDebugInfoLen;	
	else pReWritePtr = NULL;
	if (nAddLen + g_nDspDebugInfoLen < 3*1024-1)
	{
		memcpy(g_pDspDebugInfo + g_nDspDebugInfoLen, s_szbuf, nAddLen + 1);	  
		g_nDspDebugInfoLen += nAddLen;	
	}

	return iRetVal;
}


// ʶ��ӿ�
ITracker* g_pTrackerVideo = NULL;
static CHvModelStore g_modelStore;

// ƽ̨���ӿ�
extern PLATFORM_FUCTION_LIST* g_pFuction;

void SetFuctionCallBack(PLATFORM_FUCTION_LIST* pcList)
{
	if( g_pFuction == NULL )
	{
		g_pFuction = new PLATFORM_FUCTION_LIST;
		memset(g_pFuction, 0, sizeof(PLATFORM_FUCTION_LIST));
		g_pFuction->pfDebugOutPut = pcList->pfDebugOutPut;
		g_pFuction->pfCacheWbInv = pcList->pfCacheWbInv;
		g_pFuction->pfCacheInv = pcList->pfCacheInv;
		g_pFuction->pfGetSystemTick = pcList->pfGetSystemTick;

		g_pFuction->pfDMAOpen = pcList->pfDMAOpen;
		g_pFuction->pfDMACopy = pcList->pfDMACopy;
		g_pFuction->pfDMAWait = pcList->pfDMAWait;
		g_pFuction->pfDMAClose = pcList->pfDMAClose;
	}
}

// ͼƬ���ýӿ�
void ImageAddRef(IMG_MEM_OPER_LOG* pLog, IReferenceComponentImage* pRefImage)
{
	if ( pLog && pRefImage )
	{
		pLog->rgOperInfo[pLog->nCount].pMemAddr = pRefImage;
		pLog->rgOperInfo[pLog->nCount++].nOperFlag = IOF_ADD_REF;
	}
}

void ImageRelease(IMG_MEM_OPER_LOG* pLog, IReferenceComponentImage* pRefImage)
{
	if ( pLog && pRefImage )
	{
		pLog->rgOperInfo[pLog->nCount].pMemAddr = pRefImage;
		pLog->rgOperInfo[pLog->nCount++].nOperFlag = IOF_RELEASE;
	}
}

// ��Ƶ����ʼ���ӿ�
int InitDspCmdLib_Video(int nPlateRecogParamIndex, int nLastLightType, int nLastPulseLevel,int nLastCplStatus, void* pvParam)
{
	DEBUG_OUTPUT pfDebugOutPut = NULL;
	if( NULL != g_pFuction )
	{
		pfDebugOutPut = g_pFuction->pfDebugOutPut;
	}
	HRESULT hr = S_OK;
	// ����ȫ�ֲ���
	char szMsg[256];

	LoadPlateRecogParam(nPlateRecogParamIndex);
	if (NULL == g_pTrackerVideo)
	{
		CreateTracker_Video(&g_pTrackerVideo);
		if (g_pTrackerVideo)
		{
			using namespace HvCore;
			IHvModel* pHvModel;
			//g_modelStore.QueryInterface(IID_IHvModel, (void**)&pHvModel);
			//g_pTrackerVideo->SetHvModel(&g_modelStore);
			hr = g_pTrackerVideo->InitTracker((TRACKER_CFG_PARAM*)pvParam);
			g_pTrackerVideo->SetFirstLightType((LIGHT_TYPE)nLastLightType);
			g_pTrackerVideo->SetFirstPulseLevel(nLastPulseLevel);
			g_pTrackerVideo->SetFirstCplStatus(nLastCplStatus);
		}
		if( g_pTrackerVideo == NULL ) return -1;
		if( S_OK != hr ) return -2;
		return 0;
	}
	else
	{
		return 1;
	}
}

int ModifyVideoParam(void* pvParam, int iSize, void* pvRespond, int iRespondSize)
{
	if( NULL == g_pTrackerVideo )
	{
		return 0;
	}

	DEBUG_OUTPUT pfDebugOutPut = NULL;
	if( NULL != g_pFuction )
	{
		pfDebugOutPut = g_pFuction->pfDebugOutPut;
	}

	// ���ֽڵĹ����ڴ��ַ��Ŀǰ������ˮ��һ�¡�
	char szMsg[256];

	if(iSize != 4 || NULL == pvParam )
	{
		return -1;
	}

	// �����������
	DSPLinkBuffer *input = NULL;
	DSPLinkBuffer *output = NULL;
	DSPLinkBuffer *msg = (DSPLinkBuffer *)(*((int*)pvParam));
	input  = &msg[0];
	output = &msg[1];

	if( NULL != pfDebugOutPut )
	{
		sprintf(szMsg, "<ModifyvideoParam>input:%08x,%08x, %d. output:%08x,%08x, %d.\n",
				input->addr, input->phys, input->len, output->addr, output->phys, output->len);
		pfDebugOutPut(szMsg);
	}


	unsigned char* pbInbuf = (unsigned char*)input->phys;
	unsigned char* pbOutbuf = (unsigned char*)output->phys;
	int iOutputSize = output->len;

	// ��������
	DSP_CMD_ID nCmdId = DSP_CMD_ID(*((DWORD32*)pbInbuf));
	if (sizeof(DSP_RESPOND_HEAD) > iOutputSize)
	{
		return -2;
	}

	// ֻ����ָ������
	if( DCI_MODIFY_VIDEO_PARAM != nCmdId )
	{
		return -3;
	}

	DSP_RESPOND_HEAD* pRespondHead = (DSP_RESPOND_HEAD*)pbOutbuf;
	pRespondHead->dwCmdId = nCmdId;
	pRespondHead->iResult = -1;
	pRespondHead->iAppendDataSize = 0;

	// ����ʶ���ʼ���ӿ�
	unsigned char* pbPos = pbInbuf + 4;
	int* pnParamSize = (int*)pbPos;

	if( NULL != pfDebugOutPut )
	{
		sprintf(szMsg, "<ModifyvideoParam>RECOGER_PARAM size:%d %d.\n",
				*pnParamSize, sizeof(TRACKER_CFG_PARAM));
		pfDebugOutPut(szMsg);
	}

	int iRet = 0;
	// �жϲ��������Ƿ���ȷ
	if (*pnParamSize == sizeof(TRACKER_CFG_PARAM))
	{
		pbPos += 4;
		TRACKER_CFG_PARAM* pParam = (TRACKER_CFG_PARAM*)pbPos;
		// todo.
		// �Ƿ���Ҫ����ʱ���У��
		//RectifySystemTick(pParam->dwArmTick);
		if( g_pTrackerVideo == NULL )
		{
			pRespondHead->iResult = S_OK;
		}
		else
		{
			pRespondHead->iResult = g_pTrackerVideo->ModifyTracker(pParam);
		}


		if( NULL != pfDebugOutPut )
		{
			sprintf(szMsg, "<ModifyvideoParam>ModifyTracker ret:%08x.\n",
					pRespondHead->iResult);
			pfDebugOutPut(szMsg);
		}

		if (pRespondHead->iResult == S_OK)
		{
			strcat(
				pRespondHead->szErrorInfo,
				"\nModify video recoger ok.\n"
				);
		}
		else
		{
			// ���������Ϣ
			sprintf(
				pRespondHead->szErrorInfo,
				"Modify is failed!!!\n"
				);
			iRet = -4;
		}
	}
	else
	{
		// ���������Ϣ
		sprintf(
			pRespondHead->szErrorInfo,
			"Modify video recoger failed, the size of param error!!!\n"
			);
		iRet = -5;
	}


	if( NULL != g_pFuction && g_pFuction->pfCacheWbInv != NULL )
	{
		g_pFuction->pfCacheWbInv(pbOutbuf, iOutputSize, CACHE_YTPE_ALL, 1);
	}

	return iRet;
}

int InitVideoParam(void* pvParam, int iSize, void* pvRespond, int iRespondSize)
{
	DEBUG_OUTPUT pfDebugOutPut = NULL;
	if( NULL != g_pFuction )
	{
		pfDebugOutPut = g_pFuction->pfDebugOutPut;
	}

	// ���ֽڵĹ����ڴ��ַ��Ŀǰ������ˮ��һ�¡�
	char szMsg[256];

	if(iSize != 4 || NULL == pvParam )
	{
		return -1;
	}

	// �����������
	DSPLinkBuffer *input = NULL;
	DSPLinkBuffer *output = NULL;
	DSPLinkBuffer *msg = (DSPLinkBuffer *)(*((int*)pvParam));
	input  = &msg[0];
	output = &msg[1];

	if( NULL != pfDebugOutPut )
	{
		sprintf(szMsg, "<InitvideoParam>input:%08x,%08x, %d. output:%08x,%08x, %d.\n",
				input->addr, input->phys, input->len, output->addr, output->phys, output->len);
		pfDebugOutPut(szMsg);
	}

	F_CACHE_INV pfCacheInv = NULL;
	if( NULL != g_pFuction )
	{
		pfCacheInv = g_pFuction->pfCacheInv;
	}

	unsigned char* pbInbuf = (unsigned char*)input->phys;
	unsigned char* pbOutbuf = (unsigned char*)output->phys;

	int iInputSize = input->len;
	// ��֮ǰ����CACHE��Ч
	if(NULL != pfCacheInv)
	{
		pfCacheInv(pbInbuf, iInputSize, CACHE_YTPE_ALL, 1);
	}

	int iOutputSize = output->len;

	// ��������
	DSP_CMD_ID nCmdId = DSP_CMD_ID(*((DWORD32*)pbInbuf));
	if (sizeof(DSP_RESPOND_HEAD) > iOutputSize)
	{
		return -2;
	}

	// ֻ����ָ������
	// todo.
	// �޸����ʼ������ͬһ����
	if( DCI_INIT_VIDEO_RECOGER != nCmdId
			&& DCI_MODIFY_VIDEO_PARAM != nCmdId )
	{
		return -3;
	}

	// �������ڴ�
	memset(pbOutbuf, 0, sizeof(DSP_RESPOND_HEAD));
	DSP_RESPOND_HEAD* pRespondHead = (DSP_RESPOND_HEAD*)pbOutbuf;
	pRespondHead->dwCmdId = nCmdId;
	pRespondHead->iResult = -1;
	pRespondHead->iAppendDataSize = 0;

	// ������޸Ĳ�������
	if( DCI_MODIFY_VIDEO_PARAM == nCmdId )
	{
		pRespondHead->iResult = ModifyVideoParam(pvParam, iSize, pvRespond, iRespondSize);
		if( NULL != g_pFuction && g_pFuction->pfCacheWbInv != NULL )
		{
			g_pFuction->pfCacheWbInv(pbOutbuf, iOutputSize, CACHE_YTPE_ALL, 1);
		}
		return pRespondHead->iResult;
	}

	// ����ʶ���ʼ���ӿ�
	unsigned char* pbPos = pbInbuf + 4;
	int* pnParamSize = (int*)pbPos;

	if( NULL != pfDebugOutPut )
	{
		sprintf(szMsg, "<InitvideoParam>RECOGER_PARAM size:%d %d.\n",
				*pnParamSize, sizeof(INIT_VIDEO_RECOGER_PARAM));
		pfDebugOutPut(szMsg);
	}

	int iRet = 0;
	// �жϲ��������Ƿ���ȷ
	if (*pnParamSize == sizeof(INIT_VIDEO_RECOGER_PARAM))
	{
		pbPos += 4;
		INIT_VIDEO_RECOGER_PARAM* pParam = (INIT_VIDEO_RECOGER_PARAM*)pbPos;
		// todo.
		// �Ƿ���Ҫ����ʱ���У��
		//RectifySystemTick(pParam->dwArmTick);
		g_pDspDebugInfo = pRespondHead->szErrorInfo;
		pRespondHead->iResult = InitDspCmdLib_Video(pParam->nPlateRecogParamIndex,
			pParam->nLastLightType,
			pParam->nLastPulseLevel,
			pParam->nLastCplStatus,
			&(pParam->cTrackerCfgParam)
			);

		if( NULL != pfDebugOutPut )
		{
			sprintf(szMsg, "<InitvideoParam>InitDspCmdLib_Video ret:%08x.\n",
					pRespondHead->iResult);
			pfDebugOutPut(szMsg);
		}

		if (pRespondHead->iResult == S_OK)
		{
			sprintf(
				pRespondHead->szErrorInfo,
				"Init video recoger ok.\n"
				);
			RectifySystemTick(pParam->dwArmTick);
		}
		else
		{
			// ���������Ϣ
			sprintf(
				pRespondHead->szErrorInfo,
				"InitDspCmdLib_Video is failed!!!\n"
				);
			iRet = -4;
		}
	}
	else
	{
		// ���������Ϣ
		sprintf(
			pRespondHead->szErrorInfo,
			"Init video recoger failed, the size of param error!!!\n"
			);
		iRet = -5;
	}


	if( NULL != g_pFuction && g_pFuction->pfCacheWbInv != NULL )
	{
		g_pFuction->pfCacheWbInv(pbOutbuf, iOutputSize, CACHE_YTPE_ALL, 1);
	}

	return iRet;
}

int ProcessOneFrame(void* pvParam, int iSize, void* pvRespond, int iRespondSize)
{
	DEBUG_OUTPUT pfDebugOutPut = NULL;
	if( NULL != g_pFuction )
	{
		pfDebugOutPut = g_pFuction->pfDebugOutPut;
	}
	F_CACHE_INV pfCacheInv = NULL;
	if( NULL != g_pFuction )
	{
		pfCacheInv = g_pFuction->pfCacheInv;
	}
	F_CACHE_INV pfCacheWbInv = NULL;
	if( NULL != g_pFuction )
	{
		pfCacheWbInv = g_pFuction->pfCacheWbInv;
	}

	char szMsg[256];

	if( NULL != pfDebugOutPut )
	{
		//sprintf(szMsg, "<123processoneframe>do process.");
		//pfDebugOutPut(szMsg);
	}


	// ���ֽڵĹ����ڴ��ַ��Ŀǰ������ˮ��һ�¡�
	if(iSize != 4 || NULL == pvParam)
	{
		if( NULL != pfDebugOutPut )
		{
			sprintf(szMsg, "<123processoneframe>iSize != 4 || NULL == pvParam.");
			pfDebugOutPut(szMsg);
		}
		return -1;
	}

	// �����������
	DSPLinkBuffer *input = NULL;
	DSPLinkBuffer *output = NULL;
	DSPLinkBuffer *msg = (DSPLinkBuffer *)(*((int*)pvParam));
	input  = &msg[0];
	output = &msg[1];

	unsigned char* pbInbuf = (unsigned char*)input->phys;
	unsigned char* pbOutbuf = (unsigned char*)output->phys;
	int iInputSize = input->len;
	int iOutputSize = output->len;


	if( NULL != pfDebugOutPut )
	{
		//sprintf(szMsg, "<123processoneframe>param:%08x, input:%08x,%08x, %d. output:%08x,%08x, %d.\n",
		//		*((int*)pvParam), input->addr, input->phys, input->len, output->addr, output->phys, output->len);
		//pfDebugOutPut(szMsg);
	}

	// ��֮ǰ����CACHE��Ч
	if(NULL != pfCacheInv)
	{
		pfCacheInv(pbInbuf, iInputSize, CACHE_YTPE_ALL, 1);
	}


	// ��������
	DSP_CMD_ID nCmdId = DSP_CMD_ID(*((DWORD32*)pbInbuf));

	if (sizeof(DSP_RESPOND_HEAD) > iOutputSize)
	{
		if( NULL != pfDebugOutPut )
		{
			sprintf(szMsg, "<123processoneframe>sizeof(DSP_RESPOND_HEAD) > iOutputSize.");
			pfDebugOutPut(szMsg);
		}
		return -2;
	}
	// �������ڴ�
	memset(pbOutbuf, 0, sizeof(DSP_RESPOND_HEAD));
	DSP_RESPOND_HEAD* pRespondHead = (DSP_RESPOND_HEAD*)pbOutbuf;
	pRespondHead->dwCmdId = nCmdId;
	pRespondHead->iResult = -1;
	pRespondHead->iAppendDataSize = 0;
	g_pDspDebugInfo = pRespondHead->szErrorInfo;	//log�ڴ�ָ��
	g_nDspDebugInfoLen = 0;

	int iRet = 0;
	unsigned char* pbPos = pbInbuf + 4;
	int* pnParamSize = (int*)pbPos;
	pbPos += 4;
	// �жϲ��������Ƿ���ȷ
	PROCESS_ONE_FRAME_PARAM* pParam = (PROCESS_ONE_FRAME_PARAM*)pbPos;

	RectifySystemTick(pParam->dwCurTick);

	pbPos += *pnParamSize;
	int* pnDataSize = (int*)pbPos;
	pbPos += 4;
	PROCESS_ONE_FRAME_DATA* pData = (PROCESS_ONE_FRAME_DATA*)pbPos;

	// ֻ����ָ������
	if( DCI_PROCESS_ONE_FRAME != nCmdId )
	{
		if( NULL != pfDebugOutPut )
		{
			sprintf(szMsg, "<123processoneframe>DCI_PROCESS_ONE_FRAME != nCmdId");
			pfDebugOutPut(szMsg);
		}

		strcpy(pRespondHead->szErrorInfo, "<1234>DCI_PROCESS_ONE_FRAME != nCmdId.");

		// д���ҪWB����CACHE��Ч��
		if( pfCacheWbInv != NULL )
		{
			pfCacheWbInv(pbOutbuf, iOutputSize, CACHE_YTPE_ALL, 1);
		}

		return -3;
	}

	// ��֮ǰ����CACHE��Ч
	if(NULL != pfCacheInv && pData->cSyncDetData.nLen != 0)
	{
		pfCacheInv(pData->cSyncDetData.pbData, pData->cSyncDetData.nLen, CACHE_YTPE_ALL, 1);
	}

	if (*pnParamSize == sizeof(PROCESS_ONE_FRAME_PARAM) && *pnDataSize == sizeof(PROCESS_ONE_FRAME_DATA))
	{
		PROCESS_ONE_FRAME_RESPOND* pProcessRespond = (PROCESS_ONE_FRAME_RESPOND*)(pbOutbuf + sizeof(DSP_RESPOND_HEAD));
		memset(pProcessRespond, 0, sizeof(PROCESS_ONE_FRAME_RESPOND));
		pRespondHead->iAppendDataSize = sizeof(PROCESS_ONE_FRAME_RESPOND);
		pRespondHead->iResult = E_FAIL;

		// zhaopy
		// todo.
		// ǿ����� HV_IMAGE_YUV_420
		pData->hvImageYuv.nImgType = HV_IMAGE_YUV_420;

		if (pData->hvImageYuv.nImgType == HV_IMAGE_JPEG)
		{
			//��֧��JPEGʶ��
			if( NULL != pfDebugOutPut )
			{
				sprintf(szMsg, "<123processoneframe>pData->hvImageYuv.nImgType == HV_IMAGE_JPEG");
				pfDebugOutPut(szMsg);
			}
			iRet = -4;
		}
		else if (pData->hvImageYuv.nImgType == HV_IMAGE_BT1120
				|| pData->hvImageYuv.nImgType == HV_IMAGE_YUV_420)
		{
				// ͼƬ�鳡
			// 600W �Ѿ��鳡��
			// todo. ��Ҫ����ͼƬ����
			if( pData->hvImageYuv.iWidth < 3000
				|| pData->hvImageYuv.iWidth >= 3300) // 700w
			{
				pData->hvImageYuv.iStrideWidth[0] = (pData->hvImageYuv.iStrideWidth[0] << 1);
				pData->hvImageYuv.iStrideWidth[1] = (pData->hvImageYuv.iStrideWidth[1] << 1);
				pData->hvImageYuv.iHeight >>= 1;
			}
			pRespondHead->iResult = S_OK;
		}
		else
		{
			if( NULL != pfDebugOutPut )
			{
				sprintf(szMsg, "<123processoneframe>iRet = -4");
				pfDebugOutPut(szMsg);
			}
			pRespondHead->iResult = S_FALSE;
			iRet = -4;
		}

		if (pRespondHead->iResult == S_OK && g_pTrackerVideo)
		{
			pRespondHead->iResult = g_pTrackerVideo->ProcessOneFrame(pParam, pData, pProcessRespond);
		}

		// ���������Ϣ
		//memcpy(pRespondHead->szErrorInfo, g_szDebugInfo, 256);

		// ����Сͼ���ڴ�Ҫ����cache
		for(int i = 0; i < pProcessRespond->cTrigEvent.iCarLeftCount; ++i)
		{
			for(int j = 0; j < 3; ++j)
			{
				if( 0 != pData->rghvImageSmall[i].rgImageData[j].phys )
				{
					pfCacheWbInv((void*)pData->rghvImageSmall[i].rgImageData[j].phys,
						pData->rghvImageSmall[i].rgImageData[j].len,  CACHE_YTPE_ALL, 1);
				}
			}
			for(int j = 0; j < 3; ++j)
			{
				if( 0 != pData->rghvImageBin[i].rgImageData[j].phys )
				{
					pfCacheWbInv((void*)pData->rghvImageBin[i].rgImageData[j].phys,
						pData->rghvImageBin[i].rgImageData[j].len,  CACHE_YTPE_ALL, 1);
				}
			}
		}

	}
	else
	{
		sprintf(pRespondHead->szErrorInfo, "\nDSPLink protocol not match!\n");
	}

	pRespondHead->szErrorInfo[sizeof(pRespondHead->szErrorInfo)-1] = 0;

	// д���ҪWB����CACHE��Ч��
	if( pfCacheWbInv != NULL )
	{
		pfCacheWbInv(pbOutbuf, iOutputSize, CACHE_YTPE_ALL, 1);
	}

	if( NULL != pfDebugOutPut )
	{
		//sprintf(szMsg, "<123processoneframe>do process end.ret:%08x.", pRespondHead->iResult);
		//pfDebugOutPut(szMsg);
	}


	return iRet;
}
