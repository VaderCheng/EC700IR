#include "EPAppTracker.h"
#include "hvutils.h"
#include "TrackInfoHigh.h"
#include "EPAppUtils.h" 
#include "EPDetModelData.h"
#include "VirtualRefImage.h"
#include "EPCheckPeccancy.h"
#include "EPAppUtils.h"
extern void Venus_OutputDebug(char* szMsg);
extern void Venus_OutputDebugEx(char* szMsg);
extern void Venus_CacheWbInv(void* blockPtr, unsigned int byteCnt);

#define DBG_OPEN_EP 1

#include "svEPDetResult.h"

#if RUN_PLATFORM == PLATFORM_WINDOWS
#include "ExLib.h"
#include "DebugConsole.h"
#endif

extern char g_szDebugInfo[];
extern int intHeap;  
extern int dmaHeap;
extern int extHeap;

char* CapStr(CEPTrackInfo* pTrackInfo)
{
    static char buf[10];
    sprintf(buf, "[%d %d %d]", pTrackInfo->m_pimgBeginCapture != NULL, 
        pTrackInfo->m_pimgBestCapture != NULL, 
        pTrackInfo->m_pimgLastCapture != NULL
        );
    return buf;
}

#if RUN_PLATFORM == PLATFORM_WINDOWS
void ShowImgTrack(HV_COMPONENT_IMAGE* pimg, CEPTrackInfo* rgTrackInfo, int iInfoCnt, int iMs, char* szName = NULL)
{
    int* rgiInfoID = new int[iInfoCnt*2];
    HV_RECT* rgrcInfo = new HV_RECT[iInfoCnt*2];
    int iInfo1 = 0;
    int iInfo2 = 0;
    int* rgiInfoID2 = new int[iInfoCnt*2];
    HV_RECT* rgrcInfo2 = new HV_RECT[iInfoCnt*2];

    for (int i=0; i<iInfoCnt; ++i)
    {
        svEPApi::EP_PLATE_INFO cPlateInfo;
        if (rgTrackInfo[i].GetPlate(&cPlateInfo))
        {
            rgrcInfo2[iInfo2] = rgTrackInfo[i].GetLastPos();//rgTrackInfo[i].GetLastPlatePos();
            rgiInfoID2[iInfo2] = rgTrackInfo[i].GetID();
            iInfo2++;

            if (rgTrackInfo[i].GetLastPlateDetPosCount() == rgTrackInfo[i].GetPosCount())
            {
                rgrcInfo2[iInfo2] = rgTrackInfo[i].GetLastPlateDetPos();
                rgiInfoID2[iInfo2] = rgTrackInfo[i].GetID();
                iInfo2++;
            }
        }
        else if (rgTrackInfo[i].GetPosCount() != 1 
            || rgTrackInfo[i].GetState() != CEPTrackInfo::TS_RESET) 
        {
            rgrcInfo[iInfo1] = rgTrackInfo[i].GetLastPos();
            rgiInfoID[iInfo1] = rgTrackInfo[i].GetID();
            iInfo1++;
        }
    }

    ShowRectIntx2Img(pimg, rgrcInfo, rgiInfoID, iInfo1, rgrcInfo2, rgiInfoID2, iInfo2, iMs, szName);
    delete [] rgiInfoID;
    delete [] rgrcInfo;
    delete [] rgiInfoID2;
    delete [] rgrcInfo2;
}
#endif

CAR_TYPE TrackInfoType2AppType(int nInfoType)
{
    CAR_TYPE ttRet;
    switch(nInfoType)
    {
    case CEPTrackInfo::TT_LARGE_CAR:
        ttRet = CT_LARGE;
        break;
    case CEPTrackInfo::TT_MID_CAR:
        ttRet = CT_MID;
        break;
    case CEPTrackInfo::TT_SMALL_CAR:
        ttRet = CT_SMALL;
        break;
    case CEPTrackInfo::TT_BIKE:
        ttRet = CT_BIKE;
        break;
    case CEPTrackInfo::TT_WALK_MAN:
        ttRet = CT_WALKMAN;
        break;
    default:
        ttRet = CT_UNKNOWN;
    }
    return ttRet;
}

CROSS_OVER_LINE_TYPE RoadNum2CrossOverLineType(int nRoadNum)
{
    CROSS_OVER_LINE_TYPE colt;
    switch(nRoadNum)
    {
    case -1 :
        colt = COLT_NO;
        break;
    case 0:
        colt = COLT_LINE0;
        break;
    case 1:
        colt = COLT_LINE1;
        break;
    case 2:
        colt = COLT_LINE2;
        break;
    case 3:
        colt = COLT_LINE3;
        break;
    case 4:
        colt = COLT_LINE4;
        break;
    default:
        colt = COLT_INVALID;
    }
    return colt;
}

bool IsOverlapWithBigCar(CEPTrackInfo& objSmall, CEPTrackInfo* rgDjObj, int iObjCnt)
{
    HV_RECT rcSmall = objSmall.GetLastPos();
    int iSmallCH = (rcSmall.top + rcSmall.bottom) >> 1;
    for (int j=0; j<iObjCnt; ++j)
    {
        CEPTrackInfo& objBig = rgDjObj[j];

        if (objBig.GetPosCount() < 6)  continue;  

        svEPApi::EP_PLATE_INFO cPlateInfo;
        int iTypeJ = objBig.GetType();
        if ( !( (iTypeJ == CT_LARGE) 
                || (iTypeJ == CT_MID) 
                || (objBig.GetPlate(&cPlateInfo) && cPlateInfo.color == PC_YELLOW) )  )
        {
            continue;
        }
        HV_RECT rcBig = objBig.GetLastPos();
        if (rcBig.bottom < iSmallCH)  continue;

        // ���ཻ
        int L = MAX(rcBig.left, rcSmall.left);
        int R = MIN(rcBig.right, rcSmall.right);
        int T = MAX(rcBig.top, rcSmall.top);
        int B = MIN(rcBig.bottom, rcSmall.bottom);

        if ((R < L)|| (B < T)) continue;;
        if ( (R-L) * 100 / (rcSmall.right - rcSmall.left) > 70 )    
        {
            return true;
        }
    }
    return false;
}

// namespace sv
// {
// class CNoDataRefImage : public IReferenceSvImage
// {
// private:
//     SV_IMAGE *m_pPersistentImage;
//     int m_cRef;					// ���ü���
// 
//     DWORD32 m_dwFrameNo;		// ֡���
//     DWORD32 m_dwRefTime;			// ͼ��ȡ��ʱ��ϵͳʱ��.
//     DWORD32 m_dwFlag;				// ͼ��ȡ��ʱ��������־,��������岻��.
//     char m_szFrameName[260];		//֡�ļ�����
// 
//     IReferenceSvImage *m_pLast;
//     IReferenceSvImage *m_pNext;
// public:
//     // ���캯��
//     CNoDataRefImage(
//         SV_IMAGE img,
//         DWORD32 dwFrameNo,
//         DWORD32 dwRefTime,
//         LPSTR lpszFrameName
//         )
//         : m_cRef(1)
//         , m_pPersistentImage(NULL)
//         , m_dwFrameNo(dwFrameNo)
//         , m_dwRefTime(dwRefTime)
//         , m_pLast(NULL)
//         , m_pNext(NULL)
//     {
//         if (lpszFrameName != NULL)
//         {
//             strncpy(m_szFrameName, lpszFrameName, sizeof(m_szFrameName) - 1);
//         }
//         else
//         {
//             m_szFrameName[0] = 0;
//         }
//         m_pPersistentImage = new SV_IMAGE();
//         if (NULL != m_pPersistentImage)
//         {	
//             *m_pPersistentImage = img;
//         }
//     }
//     // ��������
//     ~CNoDataRefImage()
//     {
//         if (m_pPersistentImage)
//         {
//             delete m_pPersistentImage;
//         }
//         if (m_pLast != NULL)
//         {
//             m_pLast->Release();
//         }
//         if (m_pNext != NULL)
//         {
//             m_pNext->Release();
//         }
//     }
// 
//     HRESULT GetImage(SV_IMAGE *pImage)
//     {
//         if (m_pPersistentImage)
//         {
//             *pImage = *m_pPersistentImage;
//             return S_OK;
//         }
//         else
//         {
//             return E_POINTER;
//         }
//     }
// 
//     HRESULT Copy(const SV_IMAGE &imgSrc)
//     {
//         return E_FAIL;
//     }
// 
//     void AddRef() 
//     {
//         m_cRef++;
//     }
// 
//     void Release()
//     {
//         m_cRef--;
//         if (m_cRef == 0)
//         {
//             delete this;
//         }
//     }
// 
//     DWORD32 GetFrameNo() {return m_dwFrameNo;};
//     DWORD32 GetRefTime() {return m_dwRefTime;};
//     DWORD32 GetFlag() {return m_dwFlag;};
//     char *GetFrameName()
//     {
//         if (m_szFrameName[0] == 0)
//         {
//             return NULL;
//         }
//         else
//         {
//             return m_szFrameName;
//         }
//     };
// 
//     IReferenceSvImage *GetLastRefImage() {return m_pLast;};
//     IReferenceSvImage *GetNextRefImage() {return m_pNext;};
// 
//     void SetFrameNo(DWORD32 dwFrameNo) {m_dwFrameNo = dwFrameNo;};
//     void SetRefTime(DWORD32 dwRefTime) {m_dwRefTime = dwRefTime;};
//     void SetFlag(DWORD32 dwFlag) {m_dwFlag = dwFlag;};
//     void SetFrameName(const char *lpszFrameName)
//     {
//         if (lpszFrameName != NULL)
//         {
//             strncpy(m_szFrameName, lpszFrameName, sizeof(m_szFrameName) - 1);
//         }
//         else
//         {
//             m_szFrameName[0] = 0;
//         }
//     };
//     void SetLastRefImage(IReferenceSvImage *pLast) {m_pLast = pLast;};
//     void SetNextRefImage(IReferenceSvImage *pNext) {m_pNext = pNext;};
// 
//     HRESULT SetImageContent(SV_IMAGE& img)
//     {
//         return E_FAIL;
//     }
// 
//     HRESULT SetImageSize(const SV_IMAGE& img)
//     {
//         return S_FALSE;
//     }
// };
// 
//     HRESULT svCreateNoDataRefImage(
//         IReferenceSvImage** ppReferenceImage,
// 		SV_IMAGE img,
//         DWORD32 dwFrameNo,
//         DWORD32 dwRefTime,
//         LPSTR lpszFrameName
//         )
//     {
//         if (ppReferenceImage == NULL)
//         {
//             return E_POINTER;
//         }
// 
//         // ���ù��캯��
//         *ppReferenceImage = new CNoDataRefImage(
//             img,
//             dwFrameNo, dwRefTime,
//             lpszFrameName
//             );
// 
//         if (*ppReferenceImage == NULL)
//         {
//             return E_OUTOFMEMORY;
//         }
// 
//         SV_IMAGE image;
//         if (FAILED((*ppReferenceImage)->GetImage(&image)))
//         {
//             delete *ppReferenceImage;
//             return E_OUTOFMEMORY;
//         }
// 
//         return S_OK;
//     }
// 
// } // sv

CEPAppTracker* CEPAppTracker::m_pInstance = NULL;  // ���ڵ���DoCapture

CEPAppTracker::CEPAppTracker(void)
    :
    m_pEPCtrl(NULL),
    m_iTrackInfoCnt(0),
    m_pCallback(NULL),
    m_pCurIRefImage(NULL)
{
    // zhaopy
    m_nFirstLightType = LIGHT_TYPE_COUNT;
    m_dwLastCarLeftTime = 0;
    m_LightType = DAY;
    m_iMaxPlateBrightness = 80;
    m_iMinPlateBrightness = 120;
    m_iPlateLightCheckCount = 5;
    m_iNightThreshold = 55;

	for (int i = 0; i < MAX_ROADLINE_NUM; i++)
	{
		CTrackInfo::m_ActionDetectParam.iIsCrossLine[i] = 0;
		CTrackInfo::m_ActionDetectParam.iIsYellowLine[i] = 0;
	}

	m_nPlateLightType = LIGHT_TYPE((int)LIGHT_TYPE_COUNT / 2);

	m_pScaleSpeed = NULL;
	m_fEnableScaleSpeed = false;
	m_fFilterNoPlatePeccancy = false;
	m_iBlockTwinsTimeout = 120;
	m_iPlateInfoCount = 0;
	memset(m_rgPlateInfo, 0, sizeof(PLATE_INFO) * MAX_PLATE_INFO_COUNT);

	m_iLastPlateWidth = 0;
	m_iLimitSpeed = 0;
}

CEPAppTracker::~CEPAppTracker(void)
{

}

HRESULT CEPAppTracker::Init(
    TRACKER_CFG_PARAM* pCfgParam,
    PlateRecogParam* pRecogParam,
    HvCore::IHvModel* pHvModel, 
    int iFrameWidth, 
    int iFrameHeight,
    IScaleSpeed *pScaleSpeed
    )
{
    if (pCfgParam == NULL || pRecogParam == NULL || pHvModel == NULL)
    {
    	 sv::utTrace("=====pCfgParam == %08x || pRecogParam == %08x || pHvModel == %08x========\n",
    			 pCfgParam, pRecogParam, pHvModel);
        return E_INVALIDARG;
    }

    m_pScaleSpeed = pScaleSpeed;
    m_fEnableScaleSpeed = pCfgParam->cScaleSpeed.fEnable;
    m_fFilterNoPlatePeccancy = pCfgParam->cTrafficLight.fFilterNoPlatePeccancy;
    m_iBlockTwinsTimeout = pCfgParam->nBlockTwinsTimeout;
    m_iLimitSpeed = pCfgParam->iSpeedLimit;

    m_pInstance = this;

    SetSVCallBack();
    sv::utTrace("=================SVEPVER:%s================\n", svEPApi::GetRevInfo());

    m_cRecogParam = *pRecogParam;

    if (m_pEPCtrl != NULL)
    {
        Uninit();
    }

    // init var
    m_iFrameWidth = iFrameWidth;
    m_iFrameHeight = iFrameHeight;

    for (int i=0; i<m_iTrackInfoCnt; ++i)
    {
        m_rgTrackInfo[i].Free();
    }
    m_iTrackInfoCnt = 0;
    m_nEnvLightType = svEPApi::EP_RESULT_INFO::LT_DAY;
    m_nEnvLight = 160;

    m_iCurLightStatus = -1;
    m_iLastLightStatus = -1;

    // ���ֿ��س�ֵ
    m_iFilterRushPeccancy = 0;
    m_iBestLightMode = 1;
    m_iAverageConfidenceQuan = 0;
    m_iFirstConfidenceQuan = 0;
    m_fltOverLineSensitivity = 1.75f;

    //sv::memInitSegID(extHeap, intHeap);
    //sv::svDmaInit(dmaHeap, extHeap);
    
    // zhaopy
	m_iNightThreshold = pCfgParam->nNightThreshold;
	m_iPlateLightCheckCount = pCfgParam->nPlateLightCheckCount;
	m_iMinPlateBrightness = pCfgParam->nMinPlateBrightness;
	m_iMaxPlateBrightness = pCfgParam->nMaxPlateBrightness;

	m_iLastPlateWidth = 0;

#if DBG_OPEN_EP
    // create ctrl
    if (m_pEPCtrl != NULL)
    {
        svEPApi::FreeEPCtrl(m_pEPCtrl);
        m_pEPCtrl = NULL;
    }
    m_pEPCtrl = svEPApi::CreateEPCtrl();

    if (m_pEPCtrl == NULL)
    {
        return E_OUTOFMEMORY;
    }
    m_pEPCtrl->SetCaptureCallBack(CEPAppTracker::Capture_CallBack);

    // ��ʼ�����̵�
    RTN_HR_IF_FAILED(InitTrafficLight(pCfgParam));

    // ���ñ���Ĳ���
    SetMiscParam(pCfgParam);
    m_pEPCtrl->SetHvModel(pHvModel);
    
    RTN_HR_IF_FAILED(SetObjDetCfgParam(pCfgParam, iFrameWidth, iFrameHeight));
    RTN_HR_IF_FAILED(SetPlateRecogCfgParam(pCfgParam, pRecogParam));

    // load det model
    for (int i=0; i<MAX_MOD_DET_INFO; ++i)
    {
        RTN_HR_IF_SVFAILED(m_pEPCtrl->LoadDetModel(&m_rgModelParam[i]));
    }

    HRESULT hr = Svresult2Hresult(m_pEPCtrl->Init(&m_cEPParam));
    return hr;
#else
    return S_OK;
#endif
}

HRESULT CEPAppTracker::Uninit()
{
#if DBG_OPEN_EP
    if (NULL != m_pEPCtrl)
    {
        m_pEPCtrl->ReleaseDetModel();
        svEPApi::FreeEPCtrl(m_pEPCtrl);
        m_pEPCtrl = NULL;
    }
#endif

    return S_OK;
}

HRESULT CEPAppTracker::Process(
    PROCESS_ONE_FRAME_PARAM* pProcParam,
    PROCESS_ONE_FRAME_DATA* pProcessData,
    PROCESS_ONE_FRAME_RESPOND* pProcessRespond
    )
{

#if DBG_OPEN_EP
    if (m_pEPCtrl == NULL || pProcParam == NULL)  return E_FAIL;
#endif

    RTN_HR_IF_FAILED(PreProcess(pProcParam, pProcessData, pProcessRespond));

    DWORD32 dwFrameTimeMs = pProcParam->dwImageTime;

    // ����ʱ�ꡣ
    CEPTrackInfo::s_iCurImageTick = dwFrameTimeMs;

    HV_COMPONENT_IMAGE hvImgFrame = pProcessData->hvImageYuv;

    if( m_nLightCount > 0 )
    {
        CheckTrafficLight(&hvImgFrame);
    }

    // convert to svimage
    sv::SV_IMAGE svImgFrame;
    svImgFrame.m_nWidth = hvImgFrame.iWidth;
    svImgFrame.m_nHeight = hvImgFrame.iHeight;
    svImgFrame.m_nType = HvimgType2SvimgType(hvImgFrame.nImgType);
    svImgFrame.m_pData[0] = (sv::SV_UINT8*)GetHvImageData(&hvImgFrame, 0);
    svImgFrame.m_pData[1] = (sv::SV_UINT8*)GetHvImageData(&hvImgFrame, 1);
    svImgFrame.m_pData[2] = (sv::SV_UINT8*)GetHvImageData(&hvImgFrame, 2);
    svImgFrame.m_rgStrideWidth[0] = hvImgFrame.iStrideWidth[0];
    svImgFrame.m_rgStrideWidth[1] = hvImgFrame.iStrideWidth[1];
    svImgFrame.m_rgStrideWidth[2] = hvImgFrame.iStrideWidth[2];

    //< dbg time
    static int s_nFrameCnt = 0;
    static int s_nTimeAll = 0;
    if (++s_nFrameCnt > 10000) {
    	s_nFrameCnt = 1;
    	s_nTimeAll = 0;
    }
    int _t_begin = sv::utGetSystemTick();
    //>

    HRESULT hr = S_OK;
    const int MAX_DJ_DET_RES = 30;
    svEPApi::IEPTrack* rgpDjDetRes[MAX_DJ_DET_RES];
    int iDjDetResCnt = 0;
    svEPApi::EP_RESULT_INFO cEPResultInfo;
#if DBG_OPEN_EP
    // ���ʶ��
    //Venus_OutputDebug("m_pEPCtrl->Process before\n");
#if 1
    // �����첽����
    unsigned int nFrameTime;
    svEPDetApi::DET_ROI* pDetROI = NULL;
    int nDetROICnt = 0;
    svEPDetApi::SeparateResultRefPoint(pProcessData->cSyncDetData.pbData, 
        pProcessData->cSyncDetData.nLen, 
        &nFrameTime, &pDetROI, &nDetROICnt, NULL, NULL); 
    //sv::utTrace("DSP get ROI:%d\n", nDetROICnt);
    // ������ʶ��

    //dbg
    sv::SV_UINT32 nCurTime = sv::utGetSystemTick();
    static sv::SV_UINT32 s_nLastInTime = 0;

    static int s_nDelayCount = 0;
    static int s_MaxTime = 0;
    int _nUseTime = nCurTime - s_nLastInTime;
    if (s_nLastInTime != 0 && _nUseTime > s_MaxTime)
    {
        s_MaxTime = _nUseTime;
    }
    if (++s_nDelayCount > 20)
    {
        s_nDelayCount = 0;
        s_MaxTime = 0;
    }

    if (s_MaxTime > 40)
    {
    	char szMsg[256];
    	sprintf(szMsg, "@EPCtrlImpl OUT2, MAX JUMP:%d\n", s_MaxTime);
    	Venus_OutputDebug(szMsg);
    }
    //dbg

    sv::SV_RESULT svRet = m_pEPCtrl->ProcessTrack(svImgFrame, 
        dwFrameTimeMs, 
        (svEPApi::DET_ROI*)pDetROI, nDetROICnt,
        NULL, 0,
        rgpDjDetRes, 
        MAX_DJ_DET_RES, 
        &iDjDetResCnt, 
        &cEPResultInfo
        );

    //dbg
    s_nLastInTime = sv::utGetSystemTick();

#else
    sv::SV_RESULT svRet = m_pEPCtrl->Process(svImgFrame, 
        dwFrameTimeMs, 
        rgpDjDetRes, 
        MAX_DJ_DET_RES, 
        &iDjDetResCnt, 
        &cEPResultInfo
        ); 
#endif

    //< dbg time
    {
    int nUseTime = sv::utGetSystemTick() - _t_begin;
    s_nTimeAll += nUseTime;
    char szMsg[256];
    sprintf(szMsg, "EP dsp t:%d, avg:%d\n", nUseTime, s_nTimeAll/s_nFrameCnt);
    Venus_OutputDebug(szMsg);
    }
    //>


    if (svRet != sv::RS_S_OK)  {Venus_OutputDebug("m_pEPCtrl->Process Err\n");} //sprintf(g_szDebugInfo+strlen(g_szDebugInfo), "EP Process %08X. imgtype %d\n", svRet, hvImgFrame.nImgType);
    RTN_HR_IF_SVFAILED( svRet );

    // ȡ�������ȵȽ����Ϣ
    m_nEnvLightType = cEPResultInfo.nLightType;
    m_nEnvLight = cEPResultInfo.nAvgBrightness;
#endif

    // �������ת����CEPTrackInfo
    RTN_HR_IF_FAILED( CEPTrackInfo::UpdateAll(m_rgTrackInfo, MAX_DJ_TRACK_INFO, &m_iTrackInfoCnt, rgpDjDetRes, iDjDetResCnt) );
   
    //sprintf(g_szDebugInfo+strlen(g_szDebugInfo), "APP Tracker get det %d. up %d\n", iDjDetResCnt, m_iTrackInfoCnt);

#if RUN_PLATFORM == PLATFORM_WINDOWS
    ShowImgTrack(&hvImgFrame, m_rgTrackInfo, m_iTrackInfoCnt, 1, "out");
#endif

    // zhaopy
    // �жϳ���
    m_fIsCheckLightType = pProcParam->fIsCheckLightType;
	if(m_fIsCheckLightType)
	{
		Venus_OutputDebug("<123dspex0> m_fIsCheckLightType is true.");
		CheckLight();
	}

    ProcessTrackState(&hvImgFrame, pProcessRespond);

    // ������Ҫ��һЩ�������Ĵ�����
    PostProcess();
    
    return hr;
}

HRESULT CEPAppTracker::SetFirstLightType(LIGHT_TYPE nLightType)
{
	m_nFirstLightType = nLightType;
	m_dwLastCarLeftTime = sv::utGetSystemTick();
	return S_OK;
}

HRESULT CEPAppTracker::CheckLight()
{
	const int FIRST_CHECK_COUNT = 5;
	const int FIRST_COUNT_THRESHOLD = 3;
	const int MAX_CHECK_COUNT = 1000;
	const int COUNT_THRESHOLD = 900;

	static bool fFirstTime = true;
	static int iFrameCount = 0;
	static int iPositiveDayCount = 0;
	static int iPositiveNightCount = 0;
	static int iPositiveLightOff = 0;
	static int iPositiveLightOn = 0;
	static int iSuccessCount = 0;

	iFrameCount++;
	if (fFirstTime)
	{
		if (iSuccessCount < 2)
		{
			iFrameCount = 0;
			if (m_nFirstLightType >= 0 && m_nFirstLightType < LIGHT_TYPE_COUNT)
			{
				m_nPlateLightType = m_nFirstLightType;
				iSuccessCount = 2;
			}
			else
			{
 	    		char szMsg[256];
 	    		sprintf(szMsg, "<123exdo>CheckLightType iSuccessCount:%d,fFirstTime:%d.",
 	    				iSuccessCount, fFirstTime);
 	    		//Venus_OutputDebugEx(szMsg);

				if (CheckLightType(m_nEnvLight, true) == S_OK)
				{
					iSuccessCount++;
				}
			}
		}
		else
		{
			if ( iFrameCount >= FIRST_CHECK_COUNT )
			{
				fFirstTime = false;
				if (iPositiveNightCount >= FIRST_COUNT_THRESHOLD)
				{
					m_LightType = NIGHT;
				}
				else
				{
					m_LightType = DAY;
				}
			}
			else
			{
				if (m_nEnvLight <= m_iNightThreshold)
				{
					iPositiveNightCount++;
				}
				else
				{
					iPositiveDayCount++;
				}
			}
		}
	}
	else
	{
		// 5������û�������ͨ�����������ж�
		if (sv::utGetSystemTick() > m_dwLastCarLeftTime
				&& (sv::utGetSystemTick() - m_dwLastCarLeftTime) > 300000)
		{
	    		char szMsg[256];
	    		sprintf(szMsg, "<123exdo>CheckLightType tick:%d,m_dwLastCarLeftTime:%d.",
	    				sv::utGetSystemTick(), m_dwLastCarLeftTime);
	    		//Venus_OutputDebugEx(szMsg);
			CheckLightType(m_nEnvLight, true);
		}

		if ( m_nEnvLight > m_iNightThreshold )
		{
			iPositiveDayCount++;
		}
		else
		{
			iPositiveNightCount++;
		}

		if ( iFrameCount >= MAX_CHECK_COUNT )
		{
			if ( iPositiveDayCount >= COUNT_THRESHOLD )
			{
				m_LightType = DAY;
			}
			if ( iPositiveNightCount >= COUNT_THRESHOLD )
			{
				m_LightType = NIGHT;
			}

			iFrameCount = 0;
			iPositiveDayCount = 0;
			iPositiveNightCount = 0;
			iPositiveLightOn = 0;
			iPositiveLightOff = 0;
		}
	}

	return S_OK;
}

//
HRESULT CEPAppTracker::CheckLightType(int iCarY, bool fIsAvgBrightness)
{
	HRESULT hr = S_FALSE;
	static int iFrameCount = 0;
	static int iSumBrightness = 0;
	static int s_iMinValue = 255;
	static int s_iMaxValue = 0;
	static int s_iSucessCount = 0;
	static int s_iFirstSetLightType = 0;
	if((m_nFirstLightType >= 0 && m_nFirstLightType < LIGHT_TYPE_COUNT) || s_iSucessCount >= 2)
	{
		if(s_iFirstSetLightType != 1)
		{
			s_iFirstSetLightType = 1;
		}
		s_iSucessCount = 0;
	}

	// ������������������Ҫ��Ӧʱ䣬�ʵȴ?S��ż����һ֡����
	static DWORD32 s_dwLastSetParamTick = sv::utGetSystemTick();
	DWORD32 dwTimeInterval = 6000;
	if (sv::utGetSystemTick() - s_dwLastSetParamTick < dwTimeInterval && fIsAvgBrightness)
	{
		return S_FALSE;
	}

	char szMsg[256];
	sprintf(szMsg, "<123ex>CheckLightType y:%d, flag:%d. count:%d. cur:%d",
			iCarY, fIsAvgBrightness, m_iPlateLightCheckCount, iFrameCount + 1);
	//Venus_OutputDebugEx(szMsg);

	iFrameCount++;
	iSumBrightness += iCarY;

	int iMaxBrightness = 0;
	int iMinBrightness = 0;

	iMaxBrightness = m_iMaxPlateBrightness;
	iMinBrightness = m_iMinPlateBrightness;

	if (iCarY > s_iMaxValue)
	{
		s_iMaxValue = iCarY;
	}
	if (iCarY < s_iMinValue)
	{
		s_iMinValue = iCarY;
	}

	if (iFrameCount >= m_iPlateLightCheckCount && iFrameCount > 2)
	{
		int iAvgBrightness = (iSumBrightness - s_iMaxValue - s_iMinValue) / (iFrameCount - 2);
		if (iAvgBrightness < iMinBrightness && m_nPlateLightType < LIGHT_TYPE_COUNT)
		{
			if (m_nPlateLightType < LIGHT_TYPE_COUNT - 1)
			{
				sprintf(szMsg, "<123ex>CheckLightType --m_nPlateLightType:%d. iAvgBrightness:%d.",
						m_nPlateLightType, iAvgBrightness);
				//Venus_OutputDebugEx(szMsg);
				m_nPlateLightType = (LIGHT_TYPE)((int)m_nPlateLightType + 1);
			}
			else
			{
				hr = S_OK;
			}
		}
		else if (iAvgBrightness > iMaxBrightness )
		{

			if( m_nPlateLightType > 0 )
			{
				sprintf(szMsg, "<123ex>CheckLightType ++m_nPlateLightType:%d. iAvgBrightness:%d.",
						m_nPlateLightType, iAvgBrightness);
				//Venus_OutputDebugEx(szMsg);

				m_nPlateLightType = (LIGHT_TYPE)((int)m_nPlateLightType - 1);
			}
			else
			{
				hr = S_OK;
			}
		}
		else
		{
			hr = S_OK;
		}

		s_dwLastSetParamTick = sv::utGetSystemTick();
		iFrameCount = 0;
		iSumBrightness = 0;
		s_iMinValue = 255;
		s_iMaxValue = 0;
	}
	if(hr == S_OK)
	{
		s_iSucessCount++;
	}
	return hr;
}

HRESULT CEPAppTracker::PreProcess(
    PROCESS_ONE_FRAME_PARAM* pProcParam,
    PROCESS_ONE_FRAME_DATA* pProcessData,
    PROCESS_ONE_FRAME_RESPOND* pProcessRespond
    )
{
    m_pProcessData = pProcessData;
    m_pProcessRespond = pProcessRespond;

    // ��ʼ�������ڴ�����ָ��
    IVirtualRefImage::SetImgMemOperLog(&pProcessRespond->cImgMemOperLog);

    // ת��Ϊ��������ͼ��
    if (m_pCurIRefImage != NULL)    m_pCurIRefImage->Release();   
    RTN_HR_IF_FAILED(CeaeteIVirtualRefImage(&m_pCurIRefImage, 
        pProcParam->pCurFrame, pProcParam->dwFrameNo, pProcParam->dwImageTime));
    
    return S_OK;
}

HRESULT CEPAppTracker::PostProcess()
{
//  TODO ��Ӧ�ò�����
	m_pProcessRespond->cLightType = m_nPlateLightType;
// 	pProcessRespond->iCplStatus = m_iCplStatus;
// 	pProcessRespond->iPulseLevel = m_iPulseLevel;
//   
//     //TODO ���ڽṹ�����ӱ����ᵼ�¸�λ��������ʱʹ�ýṹ�����е�û�õ��ı���
// 	pProcessRespond->cTrigEvent.rgCarLeftCoreInfo[0].nVoteCondition = (m_LightType == NIGHT) ? 1 : 0; 
// 	if(m_LightType == NIGHT && pProcessRespond->iPulseLevel < 1)
// 	{
// 		pProcessRespond->iPulseLevel = 1;
// 	}
// 	if(m_iForceLightOffAtDay)
// 	{
// 		if(m_LightType == DAY && m_iPulseLevel > 0 
// 			&& m_iForceLightOffFlag == 1)
// 		{
// 			m_iPulseLevel = 0;
// 		}
// 	}

	// ���ٿ���Ϣ
	int iMaxRect = sizeof(m_pProcessRespond->cTrackRectInfo.rgTrackRect) / sizeof(m_pProcessRespond->cTrackRectInfo.rgTrackRect[0]);
	m_pProcessRespond->fIsNight = (m_nEnvLightType == svEPApi::EP_RESULT_INFO::LT_DAY) ? 0 : 1;
	sv::utTrace("is night %d\n", m_pProcessRespond->fIsNight);
	m_pProcessRespond->cTrackRectInfo.dwTrackCount = MIN(iMaxRect, m_iTrackInfoCnt);
    // һ������ֻ���һ����
	for (int i=0; i<(int)m_pProcessRespond->cTrackRectInfo.dwTrackCount; ++i)
	{
        m_pProcessRespond->cTrackRectInfo.rgTrackRect[i] = m_rgTrackInfo[i].GetLastPos();
	}

	int indexRed = 0;
	for(int i = m_pProcessRespond->cTrackRectInfo.dwTrackCount; i < iMaxRect; i++)
	{
		if( indexRed >= m_nRedLightCount ) break;
		m_pProcessRespond->cTrackRectInfo.rgTrackRect[m_pProcessRespond->cTrackRectInfo.dwTrackCount].left = m_rgRedLightRect[indexRed].left;
		m_pProcessRespond->cTrackRectInfo.rgTrackRect[m_pProcessRespond->cTrackRectInfo.dwTrackCount].top = m_rgRedLightRect[indexRed].top;
		m_pProcessRespond->cTrackRectInfo.rgTrackRect[m_pProcessRespond->cTrackRectInfo.dwTrackCount].right = m_rgRedLightRect[indexRed].right;
		m_pProcessRespond->cTrackRectInfo.rgTrackRect[m_pProcessRespond->cTrackRectInfo.dwTrackCount].bottom = m_rgRedLightRect[indexRed].bottom;
		m_pProcessRespond->cTrackRectInfo.dwTrackCount++;
		indexRed++;
	}
	//�������
	memset(m_pProcessRespond->rcRedLight, 0, sizeof(m_pProcessRespond->rcRedLight));
	for (int i = 0; i < m_nRedLightCount; i++)
	{
		m_pProcessRespond->rcRedLight[i].left =  m_rgRedLightRect[i].left;		
		m_pProcessRespond->rcRedLight[i].top =  m_rgRedLightRect[i].top * 2;
		m_pProcessRespond->rcRedLight[i].right =  m_rgRedLightRect[i].right;
		m_pProcessRespond->rcRedLight[i].bottom =  m_rgRedLightRect[i].bottom * 2;
	}
    return S_OK;
}

HRESULT CEPAppTracker::SetCallBackFunc(ITrackerCallback* pCallback)
{
#if RUN_PLATFORM == PLATFORM_WINDOWS
    m_pCallback = pCallback;
    return m_cTrafficLight.SetTrafficLightCallback(pCallback);
#endif
	return S_OK;
}

HRESULT CEPAppTracker::ProcessTrackState(
    HV_COMPONENT_IMAGE *pImage,
    PROCESS_ONE_FRAME_RESPOND* pProcessRespond
    )
{     
    // TODO PlateLightType
    // m_nPlateLightType = xx;

    for (int i=0; i<m_iTrackInfoCnt; ++i)
    {
        CheckOneState(&m_rgTrackInfo[i], pImage);
    }

    // �ж�ץ�ģ�����ץ�ı�־
    CheckCapture(pImage);

    CheckPeccancy(pImage);

    // ���ٽ�������
    for (int i=0; i<m_iTrackInfoCnt; ++i)
    {
        if (m_rgTrackInfo[i].GetState() == CEPTrackInfo::TS_RESET)
        {
            OnTrackerEnd(&m_rgTrackInfo[i]);
        }
    }

    return S_OK;
}


HRESULT CEPAppTracker::OnTrackerFirstDet(
    CEPTrackInfo* pTrackInfo
    )
{
    CEPTrackInfo& TrackInfo = *pTrackInfo; 
    TrackInfo.m_nStartFrameNo = m_pCurIRefImage->GetFrameNo();
    TrackInfo.m_dwFirstFrameTime = m_pCurIRefImage->GetRefTime();

    // ������������ץһ��ͼ��˵
    CRect rcPos = TrackInfo.GetLastPos(); 


//     if (TrackInfo.m_pimgLastSnapShot != NULL) 
//     {
//         if (pTrackInfo->GetState() == CEPTrackInfo::TS_RESET)
//         {
//             ReleaseIReferenceComponentImage(TrackInfo.m_pimgLastSnapShot);
//         }
// #if RUN_PLATFORM == PLATFORM_WINDOWS
//         else
//         {
//             sv::utTrace("Err. [%d] Should not have m_pimgLastSnapShot value\n", 
//                 TrackInfo.GetID());
//         }
// #endif
//     }
// 
//     
//     TrackInfo.m_pimgLastSnapShot = m_pCurIRefImage;
//     TrackInfo.m_pimgLastSnapShot->AddRef();
    TrackInfo.m_rgRect[1] = rcPos;
    TrackInfo.m_rcLastPos = rcPos;

    return S_OK;
}

HRESULT CEPAppTracker::OnTrackerEnd(
    CEPTrackInfo* pTrackInfo
    )
{
    pTrackInfo->m_nEndFrameNo = m_pCurIRefImage->GetFrameNo();

    if (IsTrackerCanOutput(pTrackInfo))
    {
    	 sv::utTrace("<123dspex> check press road line.\n");
        // ������ʻ����
        CheckRunType(pTrackInfo, &m_cRoadInfo, &m_cEPParam);

		int nRoadNum = -1;
		// Խ��
		nRoadNum = -1;
		if (CheckCrossLine(pTrackInfo, &m_cRoadInfo, &m_cEPParam, &nRoadNum, &m_ActionDetectParam))
		{
			pTrackInfo->m_nCrossRoadLineNum = nRoadNum;
		}

		// ѹ��
		nRoadNum = -1;
		if (CheckPressRoadLine(pTrackInfo, &m_cRoadInfo, m_fltOverLineSensitivity,
			m_cEPParam.iStopLinePos * m_iFrameHeight / 100, &nRoadNum, &m_ActionDetectParam))
		{
			pTrackInfo->m_nPressRoadLineNum = nRoadNum;
		}
        FireCarLeftEvent(pTrackInfo);
    }

    return S_OK;
}

bool CEPAppTracker::IsTrackerCanOutput(CEPTrackInfo* pTrackInfo)
{
    // ִ�е����˵�����Ѿ�����

    CEPTrackInfo& TrackInfo = *pTrackInfo;
    if (!pTrackInfo->m_fTrackReliable)   return false;

    bool fCanOutput = false;

    // ���������ƴ���
    svEPApi::EP_PLATE_INFO cPlateInfo;
    if (TrackInfo.GetPlate(&cPlateInfo))
    {
        bool fGoodTracking = false;
        // ץ��ͼ��켣�������������
        if (TrackInfo.m_pimgBeginCapture != NULL
            && TrackInfo.m_pimgBestCapture != NULL
            && TrackInfo.m_pimgLastCapture != NULL
            && TrackInfo.GetPosCount() > 10)
        {
            fGoodTracking = true;
        }

        // ��Ч�����
        int nValidDetCount = TrackInfo.GetValidDetCount();
        int nPlateRecogCount = TrackInfo.GetPlateRecogCount();

        sv::utTrace("===[%d] %.2f>%d %.2f>%d || %d>=%d||%.2f>0.3f||%d>10||%d>5||g%d v%d\n",
            TrackInfo.GetID(),
            TrackInfo.GetPlateAvgConf() * 100, m_iAverageConfidenceQuan,
            TrackInfo.GetPlateAvgFirstConf() * 100, 50,//m_iFirstConfidenceQuan, ||
            TrackInfo.GetPlateSimilarityCount(), m_cEPParam.g_nContFrames_EstablishTrack, //||
            cPlateInfo.fltConf,    // %.2f>0.3f
            nValidDetCount,  // %d>10
            nPlateRecogCount,    // %d>5
            fGoodTracking,
            TrackInfo.nRecogInValidCount
            );

        // ȥ���
        if(//m_fEnableDelPlate && GetCurrentParam()->g_PlateRcogMode == PRM_ELECTRONIC_POLICE &&
             nPlateRecogCount <= 6 && m_nEnvLightType == svEPApi::EP_RESULT_INFO::LT_NIGHT &&
             cPlateInfo.nAvgY > 180 && cPlateInfo.color == PC_LIGHTBLUE
             || TrackInfo.GetPlateAvgConf() < 0.05f)
        {
            return false;	
        }

        // ʶ��ȫ��Ч��ȥ��
        if (nPlateRecogCount <= 3
            && nValidDetCount < 7
            && nPlateRecogCount == TrackInfo.nRecogInValidCount)
        {
            return false;
        }

        // ���ƴ���
        if (TrackInfo.GetPlateAvgConf() * 100 > m_iAverageConfidenceQuan 
            && ( TrackInfo.GetPlateAvgFirstConf() * 100 > 50
            || TrackInfo.GetPlateSimilarityCount() >= m_cEPParam.g_nContFrames_EstablishTrack
            || (cPlateInfo.fltConf > 0.3f/*0.5f*/&& TrackInfo.GetPlateAvgFirstConf() > 0.05f))
            || (nValidDetCount > 10)
            || (nPlateRecogCount > 5)
            || (cPlateInfo.fltConf > 0.3f
            && nValidDetCount > 5
            && nPlateRecogCount > 4)
            || (fGoodTracking
            && nPlateRecogCount >= 2
            && TrackInfo.GetPlateSimilarityCount() > 0)
            ||
            (cPlateInfo.color == PC_YELLOW       //���ƴ󳵽��ͳ�������
            && cPlateInfo.fltConf > 0.16f
            && cPlateInfo.fltFirstConf > 0.15f
            && nValidDetCount > 3
            && TrackInfo.m_pimgBestCapture != NULL)
            )
        {
            fCanOutput = true;
        }
        else
        {
//             // ����������ǿ�Ƴ�
//             if (cPlateInfo.nPlateType == PC_WHITE)
//             {
//                 fCanOutput = true;
//             }
            sv::utTrace("DROP[%d]...\n", TrackInfo.GetID());
            //system("pause");
        }
        if (fCanOutput)
        {       
        	HV_RECT rcLast = TrackInfo.GetLastPos();
        	// ���²�
            HV_POINT ptBCLast = {(rcLast.left + rcLast.right)>>1,
                rcLast.bottom - ((rcLast.bottom - rcLast.top)>>1)};
            int nRoadNum = m_cRoadInfo.GetRoadNum(ptBCLast);

            // �ﵽ�������������ֻ�е�һ��ͼ���п������ڵ����²����������
            // ����һ��ʱ�䣬���8֡�ڼ첻���Ƶģ�����Ϊ�Ǹ���
            if (TrackInfo.m_pimgBeginCapture != NULL
                && TrackInfo.m_pimgBestCapture == NULL 
                && TrackInfo.m_pimgLastCapture == NULL
                && TrackInfo.GetLastPlateRecogPosCount() + 20 < TrackInfo.GetPosCount()
                && nRoadNum != -1)
            {
                TrackInfo.m_pimgBestCapture = TrackInfo.m_pimgBeginCapture;
                TrackInfo.m_pimgBestCapture->AddRef();
                TrackInfo.m_pimgLastCapture = m_pCurIRefImage;
                TrackInfo.m_pimgLastCapture->AddRef();

                TrackInfo.m_rgRect[3] = TrackInfo.m_rgRect[2];
                TrackInfo.m_rgRect[4] = TrackInfo.GetLastPos();
            }
            
            // ���ץ��ͼ������ץ��ͼ
            if ( TrackInfo.m_pimgBestCapture != NULL 
                && TrackInfo.m_pimgLastCapture == NULL 
                && TrackInfo.m_pimgBeginCapture != NULL )
            {
                TrackInfo.m_pimgLastCapture = TrackInfo.m_pimgBestCapture;
                TrackInfo.m_pimgLastCapture->AddRef();
                TrackInfo.m_rgRect[4] = TrackInfo.m_rgRect[3];
            }

            if (TrackInfo.m_pimgBeginCapture == NULL
                || TrackInfo.m_pimgBestCapture == NULL
                || TrackInfo.m_pimgLastCapture == NULL)
            {
                // û������ͼ����û�и��پ�����̣�һ��Ϊ��첻�ܳ����
                fCanOutput = false;
                //HV_RECT rcLast = TrackInfo.GetLastPos();
                //HV_POINT ptBCLast = {(rcLast.left + rcLast.right)>>1, // ���²�
                //    rcLast.bottom - ((rcLast.bottom - rcLast.top)>>1)};
                // ��Ƕ���ת֧��
                if (TrackInfo.GetPlateSimilarityCount() > 5
                    && TrackInfo.m_pimgBeginCapture != NULL
                    && TrackInfo.GetPosCount() - TrackInfo.GetLastPlateDetPosCount() < 15
                    && nRoadNum == -1)
                {
                    TrackInfo.m_pimgBestCapture = m_pCurIRefImage;
                    TrackInfo.m_pimgBestCapture->AddRef();
                    TrackInfo.m_pimgLastCapture = m_pCurIRefImage;
                    TrackInfo.m_pimgLastCapture->AddRef();

                    TrackInfo.m_rgRect[3] = TrackInfo.GetLastPos();
                    TrackInfo.m_rgRect[4] = TrackInfo.m_rgRect[3];
                    fCanOutput = true;
                }
                else 
                {
                    sv::utTrace("DROP [%d]. Less Cap Image %08X %08X %08X\n", TrackInfo.GetID(), 
                        TrackInfo.m_pimgBeginCapture, TrackInfo.m_pimgBestCapture, TrackInfo.m_pimgLastCapture);
                }
            }
        }
    }
    else
    {
        // ���ƴ���
        if (TrackInfo.m_pimgBeginCapture != NULL
            && TrackInfo.m_pimgBestCapture != NULL
            && TrackInfo.m_pimgLastCapture != NULL
            && TrackInfo.GetPosCount() > 5)
        {
            fCanOutput = true;
        }
        
    }

    return fCanOutput;
}

HRESULT CEPAppTracker::FireCarLeftEvent(CEPTrackInfo* pTrackInfo)
{

//#if RUN_PLATFORM == PLATFORM_DSP_BIOS

	if( pTrackInfo != NULL && pTrackInfo->m_nCarArriveTime == 0 )
	{
		if( pTrackInfo->m_pimgBestCapture != NULL )
		{
			pTrackInfo->m_nCarArriveTime = pTrackInfo->m_pimgBestCapture->GetRefTime();
		}
		else
		{
			pTrackInfo->m_nCarArriveTime = m_pCurIRefImage->GetRefTime();
		}
	}

    svEPApi::EP_PLATE_INFO cPlateInfoT;
    BOOL fHavePlateT = pTrackInfo->GetPlate(&cPlateInfoT);
    if (fHavePlateT)
    {
        sv::utTrace("[FIRE] id:[%d], %s, r:%d, t%d, c%d p%d\n", pTrackInfo->GetID(), cPlateInfoT.szPlate, pTrackInfo->GetRoadNum(),
            cPlateInfoT.nPlateType,
            pTrackInfo->m_nCrossRoadLineNum, pTrackInfo->m_nPressRoadLineNum);

        //  ������ͬ����
        for(int i = 0; i < MAX_PLATE_INFO_COUNT; ++i)
        {
        	if( m_rgPlateInfo[i].iPlateTick > 0
        			&& m_rgPlateInfo[i].iPlateTick < pTrackInfo->m_nCarArriveTime
        			&& abs((long long)pTrackInfo->m_nCarArriveTime - (long long)m_rgPlateInfo[i].iPlateTick) < (m_iBlockTwinsTimeout * 1000)
        			&& strcmp(cPlateInfoT.szPlate, m_rgPlateInfo[i].szPlate) == 0 )
        	{
        		return S_FALSE;
        	}
        }
        // ���µ����ƶ��С�
        int index = m_iPlateInfoCount;
        strcpy( m_rgPlateInfo[index].szPlate, cPlateInfoT.szPlate);
        m_rgPlateInfo[index].iPlateTick = pTrackInfo->m_nCarArriveTime;
        m_iPlateInfoCount++;
        m_iPlateInfoCount = (m_iPlateInfoCount % MAX_PLATE_INFO_COUNT);
    }
    else
    {
        sv::utTrace("[FIRE] id:[%d], NULL, r:%d t%d, c%d p%d\n", pTrackInfo->GetID(), pTrackInfo->GetRoadNum(),
            cPlateInfoT.nPlateType,
            pTrackInfo->m_nCrossRoadLineNum, pTrackInfo->m_nPressRoadLineNum);
    }
	//return S_OK;
//#endif 

    if (m_pProcessRespond == NULL || m_pProcessData == NULL)    return S_OK;

    // ���ڳ����ڵĳ����ˡ�
    if( pTrackInfo->GetRoadNum() == -1 ) return S_OK;

	//��Ҫ�����жϳ��Ƶĸ����Ƿ�������ĸ���������ֱ�ӷ���false
	if(m_pProcessRespond->cTrigEvent.iCarLeftCount >= MAX_EVENT_COUNT)
		return S_FALSE;

    // CheckLightType

 	if(m_fIsCheckLightType)
 	{
 	    svEPApi::EP_PLATE_INFO cPlateInfoTemp;
 	    BOOL fHavePlateTemp = pTrackInfo->GetPlate(&cPlateInfoTemp);
 	    if( fHavePlateTemp )
 	    {
 	    	m_dwLastCarLeftTime = sv::utGetSystemTick();
 	    	//  �������Ƴ�
 	    	if( cPlateInfoTemp.nAvgY > 0 )
 	    	{
 	    		char szMsg[256];
 	    		sprintf(szMsg, "<123exdo>CheckLightType plate.");
 	    	//	Venus_OutputDebugEx(szMsg);

 	    		CheckLightType((int)cPlateInfoTemp.nAvgY, false);
 	    	}
 	    }
 	}

//  TODO ɾ����Σ�����API��
// 	//��̬������СVariance���羯��Ϊʶ���ʵ�, �����ö�̬��ʽ
// 	if (m_pParam->g_PlateRecogSpeed == PR_SPEED_NORMAL 
// 		|| m_LightType == NIGHT
// 		|| GetCurrentParam()->g_PlateRcogMode == PRM_ELECTRONIC_POLICE)
// 	{
// 		m_pParam->g_nDetMinStdVar = m_iMinVariance;
// 	}
// 	else
// 	{
// 		int iCurrentStdVar = TrackInfo.LastInfo().nVariance - 40;
// 		if (iCurrentStdVar < 1)
// 		{
// 			iCurrentStdVar = 1;
// 		}
// 		m_pParam->g_nDetMinStdVar = (int)(m_pParam->g_nDetMinStdVar * 0.95 +
// 			iCurrentStdVar * iCurrentStdVar * 0.05);
// 	}
	PROCESS_IMAGE_CORE_RESULT* pCurResult = &(m_pProcessRespond->cTrigEvent.rgCarLeftCoreInfo[m_pProcessRespond->cTrigEvent.iCarLeftCount]);
	
    CEPTrackInfo& TrackInfo = *pTrackInfo;
	pCurResult->iCapSpeed = TrackInfo.GetID();//TrackInfo.m_nID;  //TODO what's this? ID = speead?

//  TODO ��������ʱ��
// 	pCurResult->dwLastSnapShotRefTime = TrackInfo.m_dwLastSnapShotRefTime;
// 	pCurResult->dwBestSnapShotRefTime = TrackInfo.m_dwBestSnapShotRefTime;
// 	pCurResult->dwBeginCaptureRefTime = TrackInfo.m_dwBeginCaptureRefTime;
// 	pCurResult->dwBestCaptureRefTime = TrackInfo.m_dwBestCaptureRefTime;
// 	pCurResult->dwLastCaptureRefTime = TrackInfo.m_dwLastCaptureRefTime;


//     TODO dwTriggerTime 
//     pCurResult->dwTriggerTime = 0;
//     if( TrackInfo.m_fHasTrigger && TrackInfo.m_dwTriggerTimeMs != 0 )
//     {
//         pCurResult->dwTriggerTime = TrackInfo.m_dwTriggerTimeMs;
//     }
// 
//     else if( m_fEnableCarArriveTrigger && TrackInfo.m_fIsTrigger && TrackInfo.m_nCarArriveTime != 0 )
//     {
//         pCurResult->dwTriggerTime = TrackInfo.m_nCarArriveTime;
//     }

//  ���̵Ʋ������ü����
// 	//���Ӻ����Ϣ
 	if(m_nRedLightCount < 20)
 	{
 		pCurResult->rcRedLightCount = m_nRedLightCount;
 		for(int i = 0; i < m_nRedLightCount; i++)
 		{
 			pCurResult->rcRedLightPos[i].left   = m_rgRedLightRect[i].left;
 			pCurResult->rcRedLightPos[i].top    = m_rgRedLightRect[i].top * 2;
 			pCurResult->rcRedLightPos[i].right  = m_rgRedLightRect[i].right;
 			pCurResult->rcRedLightPos[i].bottom = m_rgRedLightRect[i].bottom * 2;
 		}
 	}

	RESULT_IMAGE_STRUCT* pResultImage = &(pCurResult->cResultImg);

	// �羯�����ǰ�����Ŵ�ͼ��ֻ����������Ŵ�ͼ��
	//pResultImage->pimgBestSnapShot = (TrackInfo.m_pimgBestSnapShot != NULL)
   //     ? TrackInfo.m_pimgBestSnapShot->GetImage() : NULL;
	//pResultImage->pimgLastSnapShot = TrackInfo.m_pimgLastSnapShot->GetImage();
	// zhaopy
	if(1/*GetCurrentParam()->g_PlateRcogMode == PRM_ELECTRONIC_POLICE*/)
	{
        pResultImage->pimgBeginCapture = TrackInfo.m_pimgBeginCapture->GetImage();
        pResultImage->pimgBestCapture = TrackInfo.m_pimgBestCapture->GetImage();
        pResultImage->pimgLastCapture = TrackInfo.m_pimgLastCapture->GetImage();

//         pResultImage->pimgBeginCapture->AddRef();
//         pResultImage->pimgBestCapture->AddRef();
//         pResultImage->pimgLastCapture->AddRef();
// 
//         if (pResultImage->pimgBestSnapShot != NULL)
//             pResultImage->pimgBestSnapShot->AddRef();
//         if (pResultImage->pimgLastSnapShot != NULL)
//             pResultImage->pimgLastSnapShot->AddRef();
	}

	// ����ʱͼƬʱ���λ�õ�һ�͵����ŶԻ���
	if( pTrackInfo->m_pimgBeginCapture->GetRefTime() > pTrackInfo->m_pimgLastCapture->GetRefTime() )
	{
        pResultImage->pimgBeginCapture = TrackInfo.m_pimgLastCapture->GetImage();
        pResultImage->pimgBestCapture = TrackInfo.m_pimgBestCapture->GetImage();
        pResultImage->pimgLastCapture = TrackInfo.m_pimgBeginCapture->GetImage();

        pCurResult->dwLastSnapShotRefTime = 0;
        pCurResult->dwBestSnapShotRefTime = 0;
        pCurResult->dwBeginCaptureRefTime = pTrackInfo->m_pimgLastCapture->GetRefTime();
        pCurResult->dwBestCaptureRefTime = pTrackInfo->m_pimgBestCapture->GetRefTime();
        pCurResult->dwLastCaptureRefTime = pTrackInfo->m_pimgBeginCapture->GetRefTime();

    	pCurResult->rcBestPlatePos = TrackInfo.m_rcBestPos;
    	pCurResult->rcLastPlatePos = TrackInfo.m_rcLastPos;
    	pCurResult->rcFirstPos = TrackInfo.m_rgRect[4];
    	pCurResult->rcSecondPos = TrackInfo.m_rgRect[3];
    	pCurResult->rcThirdPos = TrackInfo.m_rgRect[2];
	}
	else
	{
        pResultImage->pimgBeginCapture = TrackInfo.m_pimgBeginCapture->GetImage();
        pResultImage->pimgBestCapture = TrackInfo.m_pimgBestCapture->GetImage();
        pResultImage->pimgLastCapture = TrackInfo.m_pimgLastCapture->GetImage();

        pCurResult->dwLastSnapShotRefTime = 0;
        pCurResult->dwBestSnapShotRefTime = 0;
        pCurResult->dwBeginCaptureRefTime = pTrackInfo->m_pimgBeginCapture->GetRefTime();
        pCurResult->dwBestCaptureRefTime = pTrackInfo->m_pimgBestCapture->GetRefTime();
        pCurResult->dwLastCaptureRefTime = pTrackInfo->m_pimgLastCapture->GetRefTime();

    	pCurResult->rcBestPlatePos = TrackInfo.m_rcBestPos;
    	pCurResult->rcLastPlatePos = TrackInfo.m_rcLastPos;
    	pCurResult->rcFirstPos = TrackInfo.m_rgRect[2];
    	pCurResult->rcSecondPos = TrackInfo.m_rgRect[3];
    	pCurResult->rcThirdPos = TrackInfo.m_rgRect[4];
	}

    svEPApi::EP_PLATE_INFO cPlateInfo;
    BOOL fHavePlate = pTrackInfo->GetPlate(&cPlateInfo);

    // �ж��Ƿ�Ϊ���Ƴ�������
    if (fHavePlate)
    {
        if ((pTrackInfo->GetPlateRecogCount() <= 2
            && cPlateInfo.fltFirstConf < 0.15f
            && cPlateInfo.color != PC_YELLOW)
            || 
            cPlateInfo.rgbContent[0] == 0)
        {    
            fHavePlate = FALSE;
            // �������
            memset(cPlateInfo.rgbContent, 0, 8);
            cPlateInfo.szPlate[0] = 0;
        }
    }

    if (fHavePlate)
    {
        int nVoteType = cPlateInfo.nPlateType;	
        IReferenceComponentImage* pimgPlate = TrackInfo.m_rgBestPlateInfo[nVoteType].pimgPlate;	// Сͼ
        bool fUseDB = false;

        if ((pimgPlate == NULL) && (PLATE_DOUBLE_MOTO == nVoteType))
        {
            fUseDB = true;				// ʹ��˫�ƴ���Ħ��
            pimgPlate = TrackInfo.m_rgBestPlateInfo[PLATE_DOUBLE_YELLOW].pimgPlate;	// Сͼ
        }

        if (pimgPlate != NULL)
        {
            // CopyСͼ�������ڴ�
            HV_COMPONENT_IMAGE imgPlate;
            if (FAILED(pimgPlate->GetImage(&imgPlate)) ||
                imgPlate.iWidth == 0 ||
                imgPlate.iHeight == 0)
            {
                pimgPlate = NULL;
            }
            else
            {
                if( cPlateInfo.nPlateType == PLATE_NORMAL)
                {
                	m_iLastPlateWidth = imgPlate.iWidth;
                }

                CPersistentComponentImage imgTemp;
                if(SUCCEEDED(imgTemp.Create(HV_IMAGE_YUV_422, imgPlate.iWidth, 2*imgPlate.iHeight)))
                {
                    //PlateResize(imgPlate, imgTemp);
                    for (int i = 0; i < 3; i++)
					{
						PBYTE8 pSrc = GetHvImageData(&imgPlate, i);
						PBYTE8 pDes = GetHvImageData(&imgTemp, i);

						if (pSrc && pDes)
						{
							if( 0 == i )
							{
								for( int j = 0; j < imgPlate.iHeight; ++j )
								{
									memcpy(pDes, pSrc, imgPlate.iWidth);
									pDes += imgTemp.iStrideWidth[i];
									memcpy(pDes, pSrc, imgPlate.iWidth);
									pDes += imgTemp.iStrideWidth[i];
									pSrc += imgPlate.iStrideWidth[i];
								}
							}
							else
							{
								int iCopySize = imgPlate.iWidth / 2;
								for( int j = 0; j < imgPlate.iHeight; ++j )
								{
									memcpy(pDes, pSrc, iCopySize);
									pDes += imgTemp.iStrideWidth[i];
									memcpy(pDes, pSrc, iCopySize);
									pDes += imgTemp.iStrideWidth[i];
									pSrc += imgPlate.iStrideWidth[i];
								}
							}

						}
					}
                    imgPlate = imgTemp;
                }

                for(int i = 0; i < 3; i++)
                {
                    PBYTE8 pSrc = GetHvImageData(&imgPlate, i);
                    PBYTE8 pDes = GetHvImageData(&m_pProcessData->rghvImageSmall[m_pProcessRespond->cTrigEvent.iCarLeftCount], i);
                    if (pSrc && pDes)
                    {
                        memcpy(pDes, pSrc, imgPlate.iStrideWidth[i] * imgPlate.iHeight);
                    }

                }
                // ��CACHE
                Venus_CacheWbInv(GetHvImageData(&m_pProcessData->rghvImageSmall[m_pProcessRespond->cTrigEvent.iCarLeftCount], 0),
                		imgPlate.iStrideWidth[0] * imgPlate.iHeight * 2 );

                pCurResult->nPlateWidth = imgPlate.iWidth;
                pCurResult->nPlateHeight = imgPlate.iHeight;
            }
        }

        if (pimgPlate != NULL)
        {
            HV_COMPONENT_IMAGE imgPlate;

            if (pimgPlate->GetImage(&imgPlate) == S_OK
                && GetHvImageData(&imgPlate, 0) != NULL)
            {
                BYTE8 rgbResizeImg[BINARY_IMAGE_WIDTH * BINARY_IMAGE_HEIGHT];

                int iWidth = BINARY_IMAGE_WIDTH;
                int iHeight = BINARY_IMAGE_HEIGHT;
                int iStride = BINARY_IMAGE_WIDTH;

                // ֻ������Ĳ��ֶ�ֵ��ͼ
                ScaleGrey(rgbResizeImg, iWidth, iHeight, iStride,
                    GetHvImageData(&imgPlate, 0),
                    imgPlate.iWidth,
                    imgPlate.iHeight,
                    imgPlate.iStrideWidth[0]
                );

                // �����ֵ����ֵ
                HiVideo::CRect rcArea(0, 0, iWidth, iHeight);
                int iThre = CalcBinaryThreByCenter(
                    rgbResizeImg,
                    iWidth,
                    iHeight,
                    iStride,
                    &rcArea);

                int nVotedColor = cPlateInfo.color;	
                // ���ݳ�����ɫ���ö�ֵ������
                BOOL fIsWhiteChar = FALSE;
                if ((nVotedColor == PC_BLUE) ||
                    (nVotedColor == PC_BLACK)	||
                    (nVotedColor == PC_GREEN))
                {
                    fIsWhiteChar = TRUE;
                }
                GrayToBinByThre(
                    fIsWhiteChar,
                    rgbResizeImg,
                    iWidth,
                    iHeight,
                    iStride,
                    iThre,
                    GetHvImageData(&m_pProcessData->rghvImageBin[m_pProcessRespond->cTrigEvent.iCarLeftCount], 0)
                    );
            }
        }
    }
    else
    {
        m_pProcessData->rghvImageSmall[m_pProcessRespond->cTrigEvent.iCarLeftCount].iWidth = 0;
        m_pProcessData->rghvImageBin[m_pProcessRespond->cTrigEvent.iCarLeftCount].iWidth = 0;

    }

	pCurResult->nDetectCrossLineEnable = m_cRecogParam.m_iDetectCrossLineEnable;
	pCurResult->nDetectOverYellowLineEnable = m_cRecogParam.m_iDetectOverYellowLineEnable;

	// ��������
	pCurResult->nCarType = TrackInfoType2AppType(TrackInfo.GetType());			// �������

	// ����ṹ��ֵ
	memcpy(pCurResult->rgbContent, cPlateInfo.rgbContent, 8);
    strcpy(pCurResult->szPlate, cPlateInfo.szPlate);

    // todo.
    // Ħ�г��Ƶ��жϡ�
    // �ڴ˴������ʣ�Ӧ���Ǽ��ʶ���������
    pCurResult->nType = cPlateInfo.nPlateType;
    if(cPlateInfo.nPlateType == PLATE_DOUBLE_YELLOW && m_iLastPlateWidth > 0 )
    {
    	int iPlateWidthMin = (m_iLastPlateWidth * 70 / 100);
    	if( pCurResult->nPlateWidth < iPlateWidthMin )
    	{
    		pCurResult->nType = PLATE_DOUBLE_MOTO;
    	}
    }

	pCurResult->nColor = cPlateInfo.color;

	if( fHavePlate )
	{
		pCurResult->fltAverageConfidence = TrackInfo.GetPlateAvgConf();
		pCurResult->fltFirstAverageConfidence = TrackInfo.GetPlateAvgFirstConf();
		pCurResult->iObservedFrames = TrackInfo.GetPlateRecogCount(); // TODO �������ָ��Ч֡����������ô�㣿
		pCurResult->iCarAvgY = (int)cPlateInfo.nAvgY;
	}
	else
	{
		pCurResult->fltAverageConfidence = 0;
		pCurResult->fltFirstAverageConfidence = 0;
		pCurResult->iObservedFrames = 0;
		pCurResult->iCarAvgY = 0;
	}

	pCurResult->nStartFrameNo = TrackInfo.m_nStartFrameNo;
	pCurResult->nEndFrameNo = TrackInfo.m_nEndFrameNo;
	pCurResult->nFirstFrameTime = TrackInfo.m_dwFirstFrameTime;
	pCurResult->iVotedObservedFrames = TrackInfo.GetVotedPlateTypeCount();;
	pCurResult->nCarColor = TrackInfo.GetColor();
	pCurResult->fIsNight = (m_nEnvLightType == svEPApi::EP_RESULT_INFO::LT_NIGHT);
//	pCurResult->fOutputCarColor = CTrackInfo::m_fEnableRecgCarColor;

	if(m_cRecogParam.m_iDetectReverseEnable)
	{
		pCurResult->fReverseRun = (TrackInfo.m_nReverseRunCount >= 3);
	}
	else
	{
		pCurResult->fReverseRun = false;
	}
	
	pCurResult->nVoteCondition = (int)TRACK_MISS;//TrackInfo.m_nVoteCondition;   // TODO ������÷�
	pCurResult->iAvgY = m_nEnvLight;

	pCurResult->iCarVariance = (int)cPlateInfo.nVariance;

	pCurResult->nRoadNo = TrackInfo.GetRoadNum();			// ��Ƶ����������������Ϊ0(�������,��Ϊ-1)

	//�¼����
	pCurResult->coltIsOverYellowLine	= COLT_NO;
	pCurResult->coltIsCrossLine		= COLT_NO;

	//ѹ����
	pCurResult->coltIsOverYellowLine = RoadNum2CrossOverLineType(TrackInfo.m_nPressRoadLineNum);//IsOverYellowLine(TrackInfo);
	//�ж���ѹʵ�߻��ǻ���
	if(CTrackInfo::m_ActionDetectParam.iIsYellowLine[pCurResult->coltIsOverYellowLine] == 2)
	{
		pCurResult->fIsDoubleYellowLine = true;
	}
	else
	{
		pCurResult->fIsDoubleYellowLine = false;
	}
	//Խ��
	pCurResult->coltIsCrossLine = //IsCrossLine(TrackInfo);
        RoadNum2CrossOverLineType(TrackInfo.m_nCrossRoadLineNum);

	if( 1 == m_iRoadNumberBegin )
	{
		pCurResult->nRoadNo = (m_cEPParam.iRoadInfoCount - 2 - pCurResult->nRoadNo);
		if( pCurResult->coltIsOverYellowLine >= 0 )
		{
			pCurResult->coltIsOverYellowLine =
					(CROSS_OVER_LINE_TYPE)(m_cEPParam.iRoadInfoCount - 1 - pCurResult->coltIsOverYellowLine);
		}
		if( pCurResult->coltIsCrossLine >= 0 )
		{
			pCurResult->coltIsCrossLine =
					(CROSS_OVER_LINE_TYPE)(m_cEPParam.iRoadInfoCount - 1 - pCurResult->coltIsCrossLine);
		}
	}
	
	// zhaopy
	// ���˲�����ѹ�ߡ�Խ���жϡ�
	if( pCurResult->coltIsOverYellowLine >= 0 )
	{
		if( m_ActionDetectParam.iIsYellowLine[pCurResult->coltIsOverYellowLine] != 1
				&&  m_ActionDetectParam.iIsYellowLine[pCurResult->coltIsOverYellowLine] != 2 )
		{
			pCurResult->coltIsOverYellowLine = COLT_NO;
		}
	}
	if( pCurResult->coltIsCrossLine >= 0 )
	{
		if( m_ActionDetectParam.iIsCrossLine[pCurResult->coltIsCrossLine] != 1 )
		{
			pCurResult->coltIsCrossLine = COLT_NO;
		}
	}

	// todo.
	// ���Ʋ����ڳ����м䲻�ж�ѹ��
	if(fHavePlate && abs(cPlateInfo.nPlatePosXOffset) > 60)
	{
		pCurResult->coltIsOverYellowLine = COLT_NO;
	}

	pCurResult->nCarLeftCount = 0;

    //TODO ���㳵��
    float fltCarSpeed(0.0f);
    float fltScaleOfDistance(1.0f);
    if(m_fEnableScaleSpeed)
    {
    	CalcCarSpeed(fltCarSpeed, fltScaleOfDistance, &TrackInfo, m_pScaleSpeed);
    	// ���������
    	// ����������ֵ��
    	if( fltCarSpeed > 200 )
    	{
    		fltCarSpeed = 22;
    		fltScaleOfDistance = 1.9;
    	}
    }
	pCurResult->fltCarspeed = fltCarSpeed;
	pCurResult->fltScaleOfDistance = fltScaleOfDistance;

	pCurResult->nPlateLightType = m_nPlateLightType;

    // TODO m_iCplStatus��m_iPulseLevel���ü����

	   

	//if( TrackInfo.m_nCarArriveTime > 0 )
	//{
	//	pCurResult->nCarArriveTime = TrackInfo.m_nCarArriveTime;   // TODO �����Է�
	//}
	//else
	// cararriveʱ��ʹ�õڶ�ץ��λ�õ�ʱ�䡣
	{
		pCurResult->nCarArriveTime = pCurResult->dwBestCaptureRefTime;
	}

#if RUN_PLATFORM == PLATFORM_WINDOWS
	sv::utTrace( 
		//"flag:%d,arrive:%d,best:0x%08x,last:0x%08x,begin:0x%08x,best:0x%08x,last:0x%08x", 
		"Out[%d],p:%d(%d,best:0x%08x,last:0x%08x,begin:0x%08x,best:0x%08x,last:0x%08x) r%d\n",
		//TrackInfo.m_fHasTrigger,
		pTrackInfo->GetID(),
        pTrackInfo->GetPosCount(),
        pTrackInfo->GetPlateRecogCount(),
		pResultImage->pimgBestSnapShot, 
		pResultImage->pimgLastSnapShot,
		pResultImage->pimgBeginCapture,
		pResultImage->pimgBestCapture,
		pResultImage->pimgLastCapture,
        m_pProcessRespond->cTrigEvent.iCarLeftCount
		);
#else
//     sv::utTrace( 
//         "#[Out] "
//         "c0:%s;rc0:%d,%d,%d,%d;"
//         "c1:%s;rc1:%d,%d,%d,%d;"
//         "c2:%s;rc2:%d,%d,%d,%d;"
//         "p:%s;\n",
//         pTrackInfo->m_pimgBeginCapture->GetFrameName(),
//         TrackInfo.m_rgRect[2].left, TrackInfo.m_rgRect[2].top, TrackInfo.m_rgRect[2].right, TrackInfo.m_rgRect[2].bottom, 
//         pTrackInfo->m_pimgBestCapture->GetFrameName(),
//         TrackInfo.m_rgRect[3].left, TrackInfo.m_rgRect[3].top, TrackInfo.m_rgRect[3].right, TrackInfo.m_rgRect[3].bottom, 
//         pTrackInfo->m_pimgLastCapture->GetFrameName(),
//         TrackInfo.m_rgRect[4].left, TrackInfo.m_rgRect[4].top, TrackInfo.m_rgRect[4].right, TrackInfo.m_rgRect[4].bottom,
//         (cPlateInfo.szPlate[0] == 0) ? "NULL" : cPlateInfo.szPlate
//         );
#endif
    //sv::utTrace("[OUT] id:%d, NULL, r:%d\n", pTrackInfo->GetID(), pTrackInfo->GetRoadNum());



#if RUN_PLATFORM == PLATFORM_WINDOWS
	// check vaild
    pTrackInfo->m_pimgBeginCapture->GetImage()->GetFrameName();
    pTrackInfo->m_pimgBestCapture->GetImage()->GetFrameName();
    pTrackInfo->m_pimgLastCapture->GetImage()->GetFrameName();
#endif
    // TODO ����б�Ҫ��
	pCurResult->fCarIsStop = false; //TrackInfo.m_fCarIsStop;
	pCurResult->iCarStopPassTime = 0; //TrackInfo.m_iCarStopPassTime;



	pCurResult->iUnSurePeccancy = 0;

    //��ʻ����
    RUN_TYPE rt = RT_UNSURE;
    //���������
    if( pCurResult->fReverseRun )
    {
        rt = RT_CONVERSE;
    }
    else
    {
        rt = TrackInfo.GetRunType();//CheckRunType(TrackInfo.m_rgRect[2], TrackInfo.m_rectLastImage);
    }

    //���˳�����ص���ʻ����
    int iFilter = CTrackInfo::m_roadInfo[pCurResult->nRoadNo].iFilterRunType;
    if( (iFilter & RRT_FORWARD) && rt == RT_FORWARD )
    {
        rt = RT_UNSURE;
    }
    else if( (iFilter & RRT_LEFT) && rt == RT_LEFT )
    {
        rt = RT_UNSURE;
    }
    else if( (iFilter & RRT_RIGHT) && rt == RT_RIGHT )
    {
        rt = RT_UNSURE;
    }
    else if( (iFilter & RRT_TURN) && rt == RT_TURN )
    {
        rt = RT_UNSURE;
    }

    pCurResult->rtType = rt;

    int iRoadType = (RRT_FORWARD | RRT_LEFT | RRT_RIGHT | RRT_TURN);
    if(pCurResult->nRoadNo != -1)
    {
    	iRoadType = m_cRoadInfo.GetRoadType(pCurResult->nRoadNo);
    }

	if( m_cRecogParam.m_fUsedTrafficLight && pCurResult->nRoadNo != -1)
	{
 		int iOnePosLightScene = TrackInfo.m_iOnePosLightScene;
 		int iTwoPosLightScene = TrackInfo.m_iPassStopLightScene;
 		int iThreePosLightScene = TrackInfo.m_iThreePosLightScene;
 
 		if( iRoadType & RRT_LEFT )
 		{
 			iTwoPosLightScene = TrackInfo.m_iPassLeftStopLightScene;
 		}

 		//GetLightScene(iOnePosLightScene, &(pCurResult->tsOnePosScene));
 		//GetLightScene(iTwoPosLightScene, &(pCurResult->tsTwoPosScene));
 		m_cTrafficLight.GetLightScene(iOnePosLightScene, &pCurResult->tsOnePosScene);
 		m_cTrafficLight.GetLightScene(iTwoPosLightScene, &pCurResult->tsTwoPosScene);
 
 		int pt;
 		//ֻ�����Ӧ�����ĺ�ƪʼʱ�?
 		//pCurResult->lrtRedStart = m_redrealtime;
 		m_cTrafficLight.GetRedLightStartTime(&pCurResult->lrtRedStart);
 		if( (iRoadType & RRT_LEFT) == 0 ) 
 		{
 			pCurResult->lrtRedStart.dwLeftL = pCurResult->lrtRedStart.dwLeftH = 0;
 		}
 		if( (iRoadType & RRT_FORWARD) == 0 )
 		{
 			pCurResult->lrtRedStart.dwForwardL = pCurResult->lrtRedStart.dwForwardH = 0;
 		}
 		if( (iRoadType & RRT_RIGHT) == 0 ) 
 		{
 			pCurResult->lrtRedStart.dwRightL = pCurResult->lrtRedStart.dwRightH = 0;
 		}
 		if( (iRoadType & RRT_TURN) == 0 ) 
 		{
 			pCurResult->lrtRedStart.dwTurnL = pCurResult->lrtRedStart.dwTurnH = 0;
 		}
 		//����л������δ֪�����ڵĳ���ֻ��һ�ַ���ģ���������ʻ����Ϊ�������?
 		if( pCurResult->rtType == RT_UNSURE && CTrackInfo::m_iRunTypeEnable == 1 )
 		{
 			if( iRoadType == RRT_LEFT )
 			{
 				pCurResult->rtType = RT_LEFT;
 			}
 			else if( iRoadType == RRT_FORWARD )
 			{
 				pCurResult->rtType = RT_FORWARD;
 			}
 			else if( iRoadType == RRT_RIGHT )
 			{
 				pCurResult->rtType = RT_RIGHT;
 			}
 			else if( iRoadType == RRT_TURN )
 			{
 				pCurResult->rtType = RT_TURN;
 			}
 		}
 	
 		bool fOverLine = false;
 		if( (pCurResult->coltIsOverYellowLine != COLT_INVALID &&  pCurResult->coltIsOverYellowLine != COLT_NO)
 			|| (pCurResult->coltIsCrossLine != COLT_INVALID && pCurResult->coltIsCrossLine != COLT_NO) )
 		{
 			fOverLine = true;
 		}
 
        DWORD32 dwCaptureTime = TrackInfo.m_pimgBestCapture->GetRefTime();
 
 		if( S_OK != CheckPeccancyType(iOnePosLightScene, iTwoPosLightScene, iThreePosLightScene, rt, iRoadType, dwCaptureTime, &pt) )
 		{
 			pt = PT_NORMAL;
 		}
 
 		//�Ƿ�ѹ�ߣ����Ƴ���������ж�.�ƹ���ע�͵���ֻҪ��ѹ�߶�Ҫ����������Υ������,2010-10-12
 		if( fOverLine )
 		{
 			pt |= PT_OVERLINE;
 		}

 		// �Ƿ���
 		if( m_iLimitSpeed > 0
 				&& pCurResult->fltCarspeed > 0.0f
 				&& fHavePlate
 				&& (int)pCurResult->fltCarspeed > m_iLimitSpeed )
 		{
 			pt |= PT_OVERSPEED;
 		}

 		//�ƹ����޸ģ�ͨ����÷�ʽ��������Υ��,2010-10-12
 		pCurResult->ptType |= pt;
 
 		if( pCurResult->dwBeginCaptureRefTime == pCurResult->dwBestCaptureRefTime
 			|| pCurResult->dwBeginCaptureRefTime == pCurResult->dwLastCaptureRefTime
 			|| pCurResult->dwBestCaptureRefTime == pCurResult->dwLastCaptureRefTime 
 			)
 		{
 			// �������С�ѹ�ߡ��ǻ��������޴�Ҫ��
 			if( (pCurResult->ptType & PT_CONVERSE) == PT_CONVERSE
 					|| (pCurResult->ptType & PT_ESTOP) == PT_ESTOP
 					|| (pCurResult->ptType & PT_OVERLINE) == PT_OVERLINE )
 			{

 			}
 			else
 			{
 				//ͼƬ����Υ�¹涨
 				pCurResult->iUnSurePeccancy = 1;
 			}
 		}
 
 		//���˵�����������Υ��
 		if( (pCurResult->ptType & PT_CONVERSE) == PT_CONVERSE 
 			&& ( (iFilter & RRT_CONVERSE) && rt == RT_CONVERSE) )
 		{
 			pCurResult->ptType &= ~PT_CONVERSE;
 		}
		
		//ץ�Ĳ���ץ��ʱ��
        // TODO �����ɷ�
		pCurResult->dwFirstTime = pCurResult->dwBeginCaptureRefTime;//TrackInfo.m_dwFirstTime;
		pCurResult->dwSecondTime = pCurResult->dwBestCaptureRefTime;//TrackInfo.m_dwSecondTime;
		pCurResult->dwThirdTime = pCurResult->dwLastCaptureRefTime;//TrackInfo.m_dwThirdTime;
	}
	else
	{
		pCurResult->ptType = PT_NORMAL;
	}

	// �������Ƴ�Υ��
	if (!fHavePlate && m_fFilterNoPlatePeccancy)
	{
		pCurResult->ptType = PT_NORMAL;
	}



	//У������
	pCurResult->rcBestPlatePos.left   = pCurResult->rcBestPlatePos.left  * 100 / m_iFrameWidth;
	pCurResult->rcBestPlatePos.top    = pCurResult->rcBestPlatePos.top   * 100 / m_iFrameHeight;
	pCurResult->rcBestPlatePos.right  = pCurResult->rcBestPlatePos.right  * 100 / m_iFrameWidth;
	pCurResult->rcBestPlatePos.bottom = pCurResult->rcBestPlatePos.bottom * 100 / m_iFrameHeight;

	pCurResult->rcLastPlatePos.left   = pCurResult->rcLastPlatePos.left  * 100 / m_iFrameWidth;
	pCurResult->rcLastPlatePos.top    = pCurResult->rcLastPlatePos.top   * 100 / m_iFrameHeight;
	pCurResult->rcLastPlatePos.right  = pCurResult->rcLastPlatePos.right  * 100 / m_iFrameWidth;
	pCurResult->rcLastPlatePos.bottom = pCurResult->rcLastPlatePos.bottom * 100 / m_iFrameHeight;

	pCurResult->rcFirstPos.left   = pCurResult->rcFirstPos.left  * 100 / m_iFrameWidth;
	pCurResult->rcFirstPos.top    = pCurResult->rcFirstPos.top   * 100 / m_iFrameHeight;
	pCurResult->rcFirstPos.right  = pCurResult->rcFirstPos.right  * 100 / m_iFrameWidth;
	pCurResult->rcFirstPos.bottom = pCurResult->rcFirstPos.bottom * 100 / m_iFrameHeight;

	pCurResult->rcSecondPos.left   = pCurResult->rcSecondPos.left  * 100 / m_iFrameWidth;
	pCurResult->rcSecondPos.top    = pCurResult->rcSecondPos.top   * 100 / m_iFrameHeight;
	pCurResult->rcSecondPos.right  = pCurResult->rcSecondPos.right  * 100 / m_iFrameWidth;
	pCurResult->rcSecondPos.bottom = pCurResult->rcSecondPos.bottom * 100 / m_iFrameHeight;

	pCurResult->rcThirdPos.left   = pCurResult->rcThirdPos.left  * 100 / m_iFrameWidth;
	pCurResult->rcThirdPos.top    = pCurResult->rcThirdPos.top   * 100 / m_iFrameHeight;
	pCurResult->rcThirdPos.right  = pCurResult->rcThirdPos.right  * 100 / m_iFrameWidth;
	pCurResult->rcThirdPos.bottom = pCurResult->rcThirdPos.bottom * 100 / m_iFrameHeight;

//  TODO ����ɶ
// 	if (TrackInfo.m_fIsTrigger)
// 	{
// 		pCurResult->dwTriggerIndex = TrackInfo.m_dwTriggerIndex;
// 		pCurResult->iCapCount = TrackInfo.m_iCapCount;
// 		//pCurResult->iCapSpeed = TrackInfo.m_iCapSpeed;
// 	}

//  TODO ��û��������Ŀ�ɣ�
// 	// ������Ŀ�޸ĳ�������
// 	if (m_iOutPutType == 1)
// 	{
// 		if ((pCurResult->nCarType == CT_SMALL)
// 			|| (pCurResult->nCarType == CT_MID)
// 			|| (pCurResult->nCarType == CT_LARGE))
// 		{
// 			pCurResult->nCarType = CT_VEHICLE;
// 		}
// 	}

//	RTN_HR_IF_FAILED( m_pCallback->CarLeft(
//		&stCarLeftInfo,
//		TrackInfo.m_pimgBestSnapShot->GetFrameName() ));

	m_pProcessRespond->cTrigEvent.dwEventId |= EVENT_CARLEFT;
	m_pProcessRespond->cTrigEvent.iCarLeftCount++;

    // ���ƣ�����ƣ����˶����Ʋ�ʹ��m_leftCar�����ﲻ��
// 	m_leftCar[pCurResult->nRoadNo].car_plate = (PLATE_COLOR)(pCurResult->nColor);
// 	m_leftCar[pCurResult->nRoadNo].frame_no = pParam->dwFrameNo;

	return S_OK;
}

// HRESULT CEPAppTracker::FireCarLeftEvent(CEPTrackInfo* pTrackInfo, 
//                                         PROCESS_ONE_FRAME_RESPOND* pProcessRespond)
// {
//     CEPTrackInfo& TrackInfo = *pTrackInfo;
// 
//     if (!m_pCallback)
//     {
//         return S_FALSE;
//     }
// 
//     svEPApi::EP_PLATE_INFO cPlateInfo;
//     BOOL fHavePlate = pTrackInfo->GetPlate(&cPlateInfo);
// 
//     RESULT_IMAGE_STRUCT ResultImage;
// 
//     if (fHavePlate)
//     {
//         int nPlateType = cPlateInfo.nPlateType;	
//         IReferenceComponentImage* pimgPlate = TrackInfo.m_rgBestPlateInfo[nPlateType].pimgPlate;
// 
//         if (pimgPlate == NULL)
//         {
//             sv::utTrace("BEST PLATE NULL!\n");
//             system("pause");
//         }
//         ResultImage.pimgPlate = pimgPlate; // Сͼ
// 
//         if ((ResultImage.pimgPlate == NULL) && (PLATE_DOUBLE_MOTO == nPlateType))
//         {
//             ResultImage.pimgPlate = TrackInfo.m_rgBestPlateInfo[PLATE_DOUBLE_YELLOW].pimgPlate;	// Сͼ
//         }
// 
//         if (ResultImage.pimgPlate != NULL)
//         {
//             HV_COMPONENT_IMAGE imgPlate;
//             if (FAILED(ResultImage.pimgPlate->GetImage(&imgPlate)) ||
//                 imgPlate.iWidth == 0 ||
//                 imgPlate.iHeight == 0)
//             {
//                 ResultImage.pimgPlate = NULL;
//             }          
//         }
// 
//         ResultImage.pimgBeginCapture = TrackInfo.m_pimgBeginCapture;
//         ResultImage.pimgBestCapture = TrackInfo.m_pimgBestCapture;
//         ResultImage.pimgLastCapture = TrackInfo.m_pimgLastCapture;
// 
// //         if( 1/*m_pParam->m_fUsedTrafficLight*/ && TrackInfo.m_pimgBestCaptureTemp != NULL )
// //         {
// //             int iOffsetSrc = HV_ABS(TrackInfo.m_rgRect[4].CenterPoint().y - TrackInfo.m_rgRect[3].CenterPoint().y);
// //             int iOffsetTemp = HV_ABS(TrackInfo.m_rgRect[4].CenterPoint().y - TrackInfo.m_rectBestCaptureTemp.CenterPoint().y);
// //             int iSrcHeight = TrackInfo.m_rgRect[0].Height();
// //             if( iOffsetSrc < iSrcHeight && iOffsetTemp > iOffsetSrc )
// //             {
// //                 ResultImage.pimgBestCapture = TrackInfo.m_pimgBestCaptureTemp;
// //                 TrackInfo.m_rgRect[3] = TrackInfo.m_rectBestCaptureTemp;
// //             }
// //         }
// 
//         // ResultImage.pimgPlateBin ����
//         //if (TrackInfo.m_bestPlatetoBinImage.pimgPlate != NULL)
//         {
//             HV_COMPONENT_IMAGE imgPlate;
//             //HRESULT hr = TrackInfo.m_bestPlatetoBinImage.pimgPlate->GetImage(&imgPlate);
//             HRESULT hr = pimgPlate->GetImage(&imgPlate);
//             if (SUCCEEDED(hr) && imgPlate.pbData[0] != NULL)
//             {
//                 // ����Bin�ĳ־û�����
//                 RTN_HR_IF_FAILED(CreateReferenceComponentImage(&ResultImage.pimgPlateBin,
//                     HV_IMAGE_BIN,
//                     BINARY_IMAGE_WIDTH,
//                     BINARY_IMAGE_HEIGHT
//                     ));
// 
//                 BYTE8 rgbResizeImg[BINARY_IMAGE_WIDTH * BINARY_IMAGE_HEIGHT];
// 
//                 int iWidth = BINARY_IMAGE_WIDTH;
//                 int iHeight = BINARY_IMAGE_HEIGHT;
//                 int iStride = BINARY_IMAGE_WIDTH;
// 
//                 // ֻ������Ĳ��ֶ�ֵ��ͼ
//                 ScaleGrey(rgbResizeImg, iWidth, iHeight, iStride,
//                     imgPlate.pbData[0],
//                     imgPlate.iWidth,
//                     imgPlate.iHeight,
//                     imgPlate.iStrideWidth[0]
//                 );
// 
//                 // �����ֵ����ֵ
//                 HiVideo::CRect rcArea(0, 0, iWidth, iHeight);
//                 int iThre = CalcBinaryThreByCenter(
//                     rgbResizeImg,
//                     iWidth,
//                     iHeight,
//                     iStride,
//                     &rcArea);
// 
//                 // �Ҷ�ת��ֵ��
//                 HV_COMPONENT_IMAGE binImage;
//                 RTN_HR_IF_FAILED(ResultImage.pimgPlateBin->GetImage(&binImage));
// 
//                 // ���ݳ�����ɫ���ö�ֵ������
//                 int nVotedColor = cPlateInfo.color;	
//                 BOOL fIsWhiteChar = FALSE;
//                 if ((nVotedColor == PC_BLUE) ||
//                     (nVotedColor == PC_BLACK)	||
//                     (nVotedColor == PC_GREEN))
//                 {
//                     fIsWhiteChar = TRUE;
//                 }
//                 GrayToBinByThre(
//                     fIsWhiteChar,
//                     rgbResizeImg,
//                     iWidth,
//                     iHeight,
//                     iStride,
//                     iThre,
//                     binImage.pbData[0]
//                 );
//             }
//         }
//     } // fHavePlate
//     else
//     {
//         ResultImage.pimgPlate = NULL;
//         ResultImage.pimgPlateBin = NULL;
// 
//         ResultImage.pimgBeginCapture = TrackInfo.m_pimgBeginCapture;
//         ResultImage.pimgBestCapture = TrackInfo.m_pimgBestCapture;
//         ResultImage.pimgLastCapture = TrackInfo.m_pimgLastCapture;
//     }
//     
//     ResultImage.pimgBestSnapShot = TrackInfo.m_pimgBestSnapShot;
//     ResultImage.pimgLastSnapShot = TrackInfo.m_pimgLastSnapShot;
// 
//     CARLEFT_INFO_STRUCT stCarLeftInfo;
// 
//     //���㳵�� TODO
// //     float fltCarSpeed(0.0f);
// //     float fltScaleOfDistance(1.0f);
// //     CalcCarSpeed(fltCarSpeed, fltScaleOfDistance, TrackInfo);
// 
//     // �������� 
//     stCarLeftInfo.nCarType = TrackInfoType2AppType(TrackInfo.GetType());//TrackInfo.m_nVotedCarType;			// �������
// 
//     // �����ߴ� TODO
// //     if (m_fOutputCarSize == TRUE)
// //     {
// //         stCarLeftInfo.stCarSize.iCarWidth = TrackInfo.m_stCarInfo.fltCarW / 100;
// //         stCarLeftInfo.stCarSize.iCarHeight = TrackInfo.m_stCarInfo.fltCarH / 100;
// // 
// //         if (m_fEnableScaleSpeed)
// //         {
// //             // ���б궨
// //             stCarLeftInfo.stCarSize.nOutType = 1;		// ���(��)
// //             if ((stCarLeftInfo.stCarSize.iCarWidth < 1.4f) 
// //                 || (stCarLeftInfo.stCarSize.iCarHeight < 2.7f))
// //             {
// //                 float fltTempS = 1.0f;
// //                 float fltRand = (float)(rand() % 51) / 150;
// //                 fltTempS += fltRand;
// // 
// //                 if (TrackInfo.m_nVotedCarType == CT_LARGE)
// //                 {
// //                     fltTempS *= 1.75f;
// //                 }
// //                 else if (TrackInfo.m_nVotedCarType == CT_MID)
// //                 {
// //                     fltTempS *= 1.20f;
// //                 }
// // 
// //                 stCarLeftInfo.stCarSize.iCarWidth = 1.5f * fltTempS;
// //                 stCarLeftInfo.stCarSize.iCarHeight = 3.4f * fltTempS;
// // 
// //             }
// // 
// //         }
// //         else
// //         {
// //             stCarLeftInfo.stCarSize.nOutType = 0;			// ���(����)
// //             // �ޱ궨
// //             if ((stCarLeftInfo.stCarSize.iCarWidth < m_nCarWBottom) 
// //                 || (stCarLeftInfo.stCarSize.iCarHeight < m_nCarLenBottom))
// //             {
// //                 float fltTempS = 1.1f;
// //                 float fltRand = (float)(rand() % 11) / 100;
// //                 fltTempS += fltRand;
// // 
// //                 if (TrackInfo.m_nVotedCarType == CT_LARGE)
// //                 {
// //                     fltTempS *= 1.75f;
// //                 }
// //                 else if (TrackInfo.m_nVotedCarType == CT_MID)
// //                 {
// //                     fltTempS *= 1.2f;
// //                 }
// // 
// //                 stCarLeftInfo.stCarSize.iCarWidth = m_nCarWBottom * fltTempS;
// //                 stCarLeftInfo.stCarSize.iCarHeight = m_nCarLenBottom * fltTempS;
// //             }
// //         }
// //     }
// 
//     // ����ṹ��ֵ
//     stCarLeftInfo.pbContent = cPlateInfo.rgbContent; //TrackInfo.m_rgbVotedResult;
//     stCarLeftInfo.bType = cPlateInfo.nPlateType; //TrackInfo.m_nVotedType;
//     stCarLeftInfo.bColor = cPlateInfo.color; //TrackInfo.m_nVotedColor;
//     stCarLeftInfo.fltAverageConfidence = TrackInfo.GetPlateAvgConf(); //TrackInfo.AverageConfidence();
//     stCarLeftInfo.fltFirstAverageConfidence = TrackInfo.GetPlateAvgFirstConf();
//     stCarLeftInfo.pResultImage = &ResultImage;
//     stCarLeftInfo.iFrameNo = TrackInfo.m_pimgLastSnapShot->GetFrameNo();//TrackInfo.m_pimgBestSnapShot->GetFrameNo();
//     stCarLeftInfo.iRefTime = TrackInfo.m_pimgLastSnapShot->GetRefTime();//TrackInfo.m_pimgBestSnapShot->GetRefTime();
//     stCarLeftInfo.nStartFrameNo = TrackInfo.m_nStartFrameNo;
//     stCarLeftInfo.nEndFrameNo = TrackInfo.m_nEndFrameNo;
//     stCarLeftInfo.nFirstFrameTime = TrackInfo.m_dwFirstFrameTime;
//     stCarLeftInfo.iObservedFrames = TrackInfo.GetPlateRecogCount();
//     stCarLeftInfo.iVotedObservedFrames = TrackInfo.GetVotedPlateTypeCount();
//     //stCarLeftInfo.nCarColor = TrackInfo.m_nVotedCarColor;   // TODO
//     stCarLeftInfo.fIsNight = (m_nEnvLightType == svEPApi::EP_RESULT_INFO::LT_NIGHT);
//     stCarLeftInfo.fOutputCarColor = false; // TODO //CTrackInfo::m_fEnableRecgCarColor;
// 
//     stCarLeftInfo.fReverseRun = (TrackInfo.m_nReverseRunCount >= 2);
// //     if(m_pParam->m_iDetectReverseEnable)
// //     {
// //         stCarLeftInfo.fReverseRun = TrackInfo.m_fReverseRunVoted;
// //     }
// //     else
// //     {
// //         stCarLeftInfo.fReverseRun = false;
// //     }
// 
//     // TODO ������÷�
//     stCarLeftInfo.nVoteCondition = (int)TRACK_MISS; //TrackInfo.m_nVoteCondition;
//     stCarLeftInfo.iAvgY = m_nEnvLight;
//     stCarLeftInfo.iCarAvgY = (int)cPlateInfo.nAvgY;
//     stCarLeftInfo.iCarVariance = (int)cPlateInfo.nVariance;
// 
//     stCarLeftInfo.nVideoDetID = TrackInfo.GetRoadNum();//TrackInfo.m_iRoad;	// ����
// //     stCarLeftInfo.iRoadNumberBegin =  CTrackInfo::m_iRoadNumberBegin;
// //     stCarLeftInfo.iStartRoadNum = CTrackInfo::m_iStartRoadNum;
// 
//     stCarLeftInfo.nCarLeftCount = 0;      // TODO ����ɶ��
//     //stCarLeftInfo.fltCarspeed = fltCarSpeed;  TODO spd
//     stCarLeftInfo.fltScaleOfDistance = 0.f; //fltScaleOfDistance;
// 
//     stCarLeftInfo.nPlateLightType = m_nPlateLightType;
// 
//      // û��ץ�Ŀ���ȥ���ɣ�
// //     if( TrackInfo.m_dwTriggerTime == 0 )
// //     {
// //         stCarLeftInfo.nCarArriveTime = TrackInfo.m_nCarArriveTime;
// //     }
// //     else
// //     {
// //         stCarLeftInfo.nCarArriveTime = TrackInfo.m_dwTriggerTime;
// //     }
//     stCarLeftInfo.nCarArriveTime = TrackInfo.m_nCarArriveTime;
// 
//     // TODO �б�Ҫ��
//     stCarLeftInfo.fCarIsStop = false; //TrackInfo.m_fCarIsStop;
//     stCarLeftInfo.iCarStopPassTime = 0; //TrackInfo.m_iCarStopPassTime;
// 
//     //�¼����
// //     stCarLeftInfo.coltIsOverYellowLine	= COLT_NO;   // ������裬�����ָĵ�
// //     stCarLeftInfo.coltIsCrossLine		= COLT_NO;   // ...
//     //ѹ����
//     stCarLeftInfo.coltIsOverYellowLine = RoadNum2CrossOverLineType(TrackInfo.m_nPressRoadLineNum);  //IsOverYellowLine(TrackInfo);
//     //�ж���ѹʵ�߻��ǻ���	
//     if(CTrackInfo::m_ActionDetectParam.iIsYellowLine[stCarLeftInfo.coltIsOverYellowLine] == 2)
//     {
//         stCarLeftInfo.fIsDoubleYellowLine = true;
//     }
//     else
//     {
//         stCarLeftInfo.fIsDoubleYellowLine = false;
//     }
//     //Խ��
//     stCarLeftInfo.coltIsCrossLine =                          //IsCrossLine(TrackInfo);
//         RoadNum2CrossOverLineType(TrackInfo.m_fHaveCrossRoadLine? 0: -1);   //TODO �Ƿ��б�Ҫ��
// 
//     stCarLeftInfo.iUnSurePeccancy = 0;
// 
//     if( 1/*m_pParam->m_fUsedTrafficLight*/ )
//     {
//         //���̵Ƴ����ŵļ���ʹ�õ�һ��ץ�Ĵ�ͼ��λ��
//         CRect rcPos = TrackInfo.GetPos(0);
//         HV_POINT ptC = rcPos.CenterPoint();
//         stCarLeftInfo.nVideoDetID = m_cRoadInfo.GetRoadNum(hvPoint(ptC.x, ptC.y));
// //             MatchRoad(TrackInfo.m_rgPlateInfo[0].rcPos.CenterPoint().x, 
// //             TrackInfo.m_rgPlateInfo[0].rcPos.CenterPoint().y);
// 
//         //��ʻ����
//         RUN_TYPE rt = RT_UNSURE;
//         //���������
//         if( stCarLeftInfo.fReverseRun )
//         {
//             rt = RT_CONVERSE;
//         }
//         else
//         {
//             // zhaopy
//             rt = TrackInfo.GetRunType();
//             // TODO �����ڸ���?
// //             if (m_fUsePathwayMethod)
// //             {
// //                 rt = GetAdjustRunType(TrackInfo);
// //                 if (rt == RT_UNSURE && !TrackInfo.m_fJump)
// //                 {
// //                     rt = CheckRunType(TrackInfo.m_rgRect[2], TrackInfo.m_rectLastImage);
// //                 }
// //             }
// //             else 
// //             {
// //                 rt = CheckRunType(TrackInfo.m_rgRect[2], TrackInfo.m_rectLastImage);
// //             }
//         }
// 
//         //�жϳ�����ʻ����,0:�ӽ���Զ,1:��Զ����
//         stCarLeftInfo.iRoadRunDirection = CTrackInfo::m_roadInfo[stCarLeftInfo.nVideoDetID].iRoadRunDirection;
//         int iCarDirect = 0;
//         if(stCarLeftInfo.fReverseRun)
//         {
//             iCarDirect = 1;
//         }
//         if(stCarLeftInfo.iRoadRunDirection == 1 && iCarDirect == 1)
//         {
//             rt = RT_UNSURE;
//             stCarLeftInfo.fReverseRun = false;
//         }
//         else if(stCarLeftInfo.iRoadRunDirection == 1 && iCarDirect == 0)
//         {
//             rt = RT_CONVERSE;
//             stCarLeftInfo.fReverseRun = true;
//         }
// 
//         //���˳��������ʻ����
//         int iFilter = CTrackInfo::m_roadInfo[stCarLeftInfo.nVideoDetID].iFilterRunType;
//         if( (iFilter & RRT_FORWARD) && rt == RT_FORWARD )
//         {
//             rt = RT_UNSURE;
//         }
//         else if( (iFilter & RRT_LEFT) && rt == RT_LEFT )
//         {
//             rt = RT_UNSURE;
//         }
//         else if( (iFilter & RRT_RIGHT) && rt == RT_RIGHT )
//         {
//             rt = RT_UNSURE;
//         }
//         else if( (iFilter & RRT_TURN) && rt == RT_TURN )
//         {
//             rt = RT_UNSURE;
//         }
// 
//         stCarLeftInfo.rtType = rt;
//         int iOnePosLightScene = TrackInfo.m_iOnePosLightScene;
//         int iTwoPosLightScene = TrackInfo.m_iPassStopLightScene;		
// 
//         if( CTrackInfo::m_roadInfo[stCarLeftInfo.nVideoDetID].iRoadType & RRT_LEFT )
//         {
//             iTwoPosLightScene = TrackInfo.m_iPassLeftStopLightScene;
//         }
// 
//         //�����"���˵�����ץ��ͼΪ�̵Ƶ�Υ��",���õ�����ץ���ߵƵ�״̬����Ϊ�жϵ�����
//         //Τ�����޸�,2010-12-22
//         if(m_iFilterRushPeccancy && iTwoPosLightScene != -1)
//         {
//             iTwoPosLightScene = TrackInfo.m_iThreePosLightScene;
//         }
// 
//         m_cTrafficLight.GetLightScene(iOnePosLightScene, &stCarLeftInfo.tsOnePosScene);
//         m_cTrafficLight.GetLightScene(iTwoPosLightScene, &stCarLeftInfo.tsTwoPosScene);
// 
//         /*PECCANCY_TYPE*/ int pt;
//         int iRoadType = (RRT_FORWARD | RRT_LEFT | RRT_RIGHT | RRT_TURN);
//         if( stCarLeftInfo.nVideoDetID != -1 )
//         {
//             iRoadType = CTrackInfo::m_roadInfo[stCarLeftInfo.nVideoDetID].iRoadType;
//             //�ƹ������,2011-03-07
//             stCarLeftInfo.iRoadType = CTrackInfo::m_roadInfo[stCarLeftInfo.nVideoDetID].iRoadType;
//         }
//         //ֻ�����Ӧ�����ĺ�ƿ�ʼʱ��
//         m_cTrafficLight.GetRedLightStartTime(&stCarLeftInfo.lrtRedStart);
//         if( (iRoadType & RRT_LEFT) == 0 ) 
//         {
//             stCarLeftInfo.lrtRedStart.dwLeftL = stCarLeftInfo.lrtRedStart.dwLeftH = 0;
//         }
//         if( (iRoadType & RRT_FORWARD) == 0 )
//         {
//             stCarLeftInfo.lrtRedStart.dwForwardL = stCarLeftInfo.lrtRedStart.dwForwardH = 0;
//         }
//         if( (iRoadType & RRT_RIGHT) == 0 ) 
//         {
//             stCarLeftInfo.lrtRedStart.dwRightL = stCarLeftInfo.lrtRedStart.dwRightH = 0;
//         }
//         if( (iRoadType & RRT_TURN) == 0 ) 
//         {
//             stCarLeftInfo.lrtRedStart.dwTurnL = stCarLeftInfo.lrtRedStart.dwTurnH = 0;
//         }
//         //�����ʻ������δ֪�����ڵĳ���ֻ��һ�ַ���ģ����������ʻ����Ϊ�������ġ�
//         if( stCarLeftInfo.rtType == RT_UNSURE && CTrackInfo::m_iRunTypeEnable == 1 )
//         {
//             if( iRoadType == RRT_LEFT )
//             {
//                 stCarLeftInfo.rtType = RT_LEFT;
//             }
//             else if( iRoadType == RRT_FORWARD )
//             {
//                 stCarLeftInfo.rtType = RT_FORWARD;
//             }
//             else if( iRoadType == RRT_RIGHT )
//             {
//                 stCarLeftInfo.rtType = RT_RIGHT;
//             }
//             else if( iRoadType == RRT_TURN )
//             {
//                 stCarLeftInfo.rtType = RT_TURN;
//             }
//         }
// 
//         bool fOverLine = false;
//         if( (stCarLeftInfo.coltIsOverYellowLine != COLT_INVALID &&  stCarLeftInfo.coltIsOverYellowLine != COLT_NO)
//             || (stCarLeftInfo.coltIsCrossLine != COLT_INVALID && stCarLeftInfo.coltIsCrossLine != COLT_NO) )
//         {
//             fOverLine = true;
//         }
// 
//         DWORD32 dwCaptureTime = (ResultImage.pimgBestCapture == NULL ? GetSystemTick() : ResultImage.pimgBestCapture->GetRefTime());		
// 
//         if( S_OK != CheckPeccancyType(iOnePosLightScene, iTwoPosLightScene, rt, iRoadType, dwCaptureTime, &pt) )
//         {
//             pt = PT_NORMAL;
//         }
//         if(m_iFilterRushPeccancy  && (pt & PT_RUSH) != PT_RUSH)
//         {
//             int ptTmp;
//             if( S_OK == CheckPeccancyType(iOnePosLightScene, TrackInfo.m_iPassStopLightScene, rt, iRoadType, dwCaptureTime, &ptTmp) )
//             {
//                 if((ptTmp & PT_RUSH) == PT_RUSH)
//                 {
//                     //HV_Trace("����ץ��ͼ�Ƶ�״̬��Ϊ���ִ�й���\n");
//                 }
//             }
//         }
// //         // TODO ����ָ����ƣ��羯�Ƿ�û�У�
// //         //����Ѿ��������������,���ҵ�����û����,�򴥷�������
// //         if( TrackInfo.m_fFirstTrigger && TrackInfo.m_fSecondTrigger	&& !TrackInfo.m_fThirdTrigger && pt != PT_NORMAL)
// //         {
// //             //int iPosThree = CTrackInfo::m_iCaptureThreePos * CTrackInfo::m_iHeight / 100;
// // 
// //             if(GetSystemTick() > (DWORD32)m_sMinTriggerTimeMs)
// //             {
// //                 m_dwTriggerTimes++;
// //                 if(m_dwTriggerTimes >= 0xffff)
// //                 {
// //                     m_dwTriggerTimes = 1;
// //                 }
// // 
// //                 DWORD32 dwRoadNum = (m_dwTriggerTimes << 16) | stCarLeftInfo.nVideoDetID;
// //                 m_pCallback->ThirdTrigger(m_LightType,dwRoadNum, NULL);
// //                 m_sMinTriggerTimeMs = GetSystemTick() + 300;
// //                 m_dwLastTriggerInfo = dwRoadNum;
// //                 //HV_Trace("firecarleft:%d,%d, %08x\n", m_dwTriggerTimes, stCarLeftInfo.nVideoDetID, dwRoadNum);
// //             }
// //             else
// //             {
// //                 DWORD32 dwRoadNum = (m_dwLastTriggerInfo & 0xffff0000) | stCarLeftInfo.nVideoDetID;
// //                 //HV_Trace("ThirdTrigger Readd:%08x\n", dwRoadNum);
// //                 m_pCallback->ReaddSignal(dwRoadNum);
// //             }
// //             TrackInfo.m_fThirdTrigger = true;
// //             TrackInfo.m_dwThirdTime = m_dwTriggerTimes;
// //             TrackInfo.m_dwThirdTimeMs = GetSystemTick();
// //         }
// 
//         //�Ƿ�ѹ�ߣ����Ƴ���������ж�.�ƹ���ע�͵���ֻҪ��ѹ�߶�Ҫ����������Υ������,2010-10-12
//         if( /*pt == PT_NORMAL && */fOverLine )
//         {
//             pt |= PT_OVERLINE;
//         }
//         //�ƹ����޸ģ�ͨ����÷�ʽ��������Υ��,2010-10-12
//         stCarLeftInfo.ptType |= pt;
// 
//         if( (ResultImage.pimgBeginCapture == NULL || ResultImage.pimgBestCapture == NULL || ResultImage.pimgLastCapture == NULL )
//             || ResultImage.pimgBeginCapture->GetRefTime() == ResultImage.pimgBestCapture->GetRefTime()
//             || ResultImage.pimgBeginCapture->GetRefTime() == ResultImage.pimgLastCapture->GetRefTime()
//             || ResultImage.pimgBestCapture->GetRefTime() == ResultImage.pimgLastCapture->GetRefTime() 
//             )
//         {
//             //ͼƬ����Υ�¹涨
//             stCarLeftInfo.iUnSurePeccancy = 1;
//         }
//         //���˵�����������Υ��
//         if( (stCarLeftInfo.ptType & PT_CONVERSE) == PT_CONVERSE 
//             && ( (iFilter & RRT_CONVERSE) && rt == RT_CONVERSE) )
//         {
//             stCarLeftInfo.ptType &= ~PT_CONVERSE;
//         }
// 
//         // TODO �Ƿ���Ҫ��
//         //ץ�Ĳ���ץ��ʱ��
// //         stCarLeftInfo.dwFirstTime = TrackInfo.m_dwFirstTime;
// //         stCarLeftInfo.dwSecondTime = TrackInfo.m_dwSecondTime;
// //         stCarLeftInfo.dwThirdTime = TrackInfo.m_dwThirdTime;
// //         if(CTrackInfo::m_iFlashlightMode == 1 && m_iAvgY < CTrackInfo::m_iFlashlightThreshold)
// //         {
// //             //HV_Trace("Tracker:s=%d:f=%d:t=%d\n", stCarLeftInfo.dwFirstTime, stCarLeftInfo.dwSecondTime, stCarLeftInfo.dwThirdTime);
// //         }
//     }
//     else
//     {
//         stCarLeftInfo.rtType = RT_UNSURE;
//         stCarLeftInfo.ptType = PT_NORMAL;
//     }
// 
//     // �ڵ�� rcBestPlatePos ���ǵ�һץ��ͼ
//     stCarLeftInfo.rcBestPlatePos = TrackInfo.m_rcBestPos;
//     stCarLeftInfo.rcLastPlatePos = TrackInfo.m_rcLastPos;
//     stCarLeftInfo.rcFirstPos = TrackInfo.m_rgRect[2];
//     stCarLeftInfo.rcSecondPos = TrackInfo.m_rgRect[3];
//     stCarLeftInfo.rcThirdPos = TrackInfo.m_rgRect[4];
//     
// //     // ������Ŀ�޸ĳ�������
// //     if (m_iOutPutType == 1)
// //     {
// //         if ((stCarLeftInfo.nCarType == CT_SMALL)
// //             || (stCarLeftInfo.nCarType == CT_MID)
// //             || (stCarLeftInfo.nCarType == CT_LARGE))
// //         {
// //             stCarLeftInfo.nCarType = CT_VEHICLE;
// //         }
// //     }
// 
//     RTN_HR_IF_FAILED( m_pCallback->CarLeft(
//         &stCarLeftInfo,
//         TrackInfo.m_pimgLastSnapShot->GetFrameName()//TrackInfo.m_pimgBestSnapShot->GetFrameName()
//         ));
//     
//     SaveEPResult(&stCarLeftInfo, &ResultImage);
// 
//     // ���ƣ�����ƣ����˶����ƣ����ﲻ��
// //     m_leftCar[stCarLeftInfo.nVideoDetID].car_plate = (PLATE_COLOR)(stCarLeftInfo.bColor);
// //     //leftCar[stCarLeftInfo.nVideoDetID].car_left_time = GetSystemTick();
// //     m_leftCar[stCarLeftInfo.nVideoDetID].frame_no = stCarLeftInfo.iFrameNo;
// 
//     // �ͷ���������ö�
//     if (ResultImage.pimgPlateBin != NULL)
//     {
//         ResultImage.pimgPlateBin->Release();
//         ResultImage.pimgPlateBin = NULL;
//     }
// 
//     return S_OK;
// }

bool CEPAppTracker::CheckRoadAndRun(RUN_TYPE runtype, int runroadtype)
{
    bool fRet = false;
    if( runtype == RT_UNSURE )
    {
        fRet = true;
    }
    else if( runtype == RT_FORWARD && (runroadtype &  RRT_FORWARD) )
    {
        fRet = true;
    }
    else if( runtype == RT_LEFT && (runroadtype & RRT_LEFT) )
    {
        fRet = true;
    }
    else if( runtype == RT_RIGHT && (runroadtype & RRT_RIGHT) )
    {
        fRet = true;
    }
    else if( runtype == RT_TURN && (runroadtype & RRT_TURN) )
    {
        fRet = true;
    }
    return fRet;
}

bool CEPAppTracker::CheckRush(int iPreLight, int iAcrossLight, int iThreeLight, int runroadtype, int runtype, DWORD32 dwCaptureTime)
{
    if( iPreLight < 0 || iAcrossLight < 0 || iThreeLight < 0 || iPreLight >= MAX_SCENE_COUNT || iAcrossLight >= MAX_SCENE_COUNT )
    {
        return false;
    }

    bool fRet = true;
    TRAFFICLIGHT_SCENE tsPre = {TLS_UNSURE, TLS_UNSURE, TLS_UNSURE, TLS_UNSURE};
    TRAFFICLIGHT_SCENE tsAcross = {TLS_UNSURE, TLS_UNSURE, TLS_UNSURE, TLS_UNSURE};
    TRAFFICLIGHT_SCENE tsThree = {TLS_UNSURE, TLS_UNSURE, TLS_UNSURE, TLS_UNSURE};
    m_cTrafficLight.GetLightScene(iPreLight, &tsPre);
    m_cTrafficLight.GetLightScene(iAcrossLight, &tsAcross);
    m_cTrafficLight.GetLightScene(iThreeLight, &tsThree);

    //�����̵�ʱ��
    LIGHT_TICK ltGreenTick;
    m_cTrafficLight.GetGreenLightStartTime(&ltGreenTick);
    if( ltGreenTick.dwLeft != 0 && dwCaptureTime > ltGreenTick.dwLeft && tsAcross.lsLeft != TLS_GREEN )
    {
        //HV_Trace("changes left light state to green\n");
        tsAcross.lsLeft = TLS_GREEN;
    }
    if( ltGreenTick.dwForward != 0 && dwCaptureTime > ltGreenTick.dwForward && tsAcross.lsForward != TLS_GREEN )
    {
        //HV_Trace("changes forward light state to green\n");
        tsAcross.lsForward = TLS_GREEN;
    }
    if( ltGreenTick.dwRight != 0 && dwCaptureTime > ltGreenTick.dwRight && tsAcross.lsRight != TLS_GREEN )
    {
        //HV_Trace("changes right light state to green\n");
        tsAcross.lsRight = TLS_GREEN;
    }
    if( ltGreenTick.dwTurn != 0 && dwCaptureTime > ltGreenTick.dwTurn && tsAcross.lsTurn != TLS_GREEN ) 
    {
        //HV_Trace("changes turn light state to green\n");
        tsAcross.lsTurn = TLS_GREEN;
    }

    bool fUnLeft = ( (tsPre.lsLeft == TLS_RED || tsPre.lsLeft == TLS_YELLOW)
    				&& (tsAcross.lsLeft == TLS_RED || tsAcross.lsLeft == TLS_YELLOW)
    				&& (tsThree.lsLeft == TLS_RED || tsThree.lsLeft == TLS_YELLOW) );
    bool fUnRight = ( (tsPre.lsRight == TLS_RED || tsPre.lsRight == TLS_YELLOW)
    		        && (tsAcross.lsRight == TLS_RED || tsAcross.lsRight == TLS_YELLOW)
    		        && (tsThree.lsRight == TLS_RED || tsThree.lsRight == TLS_YELLOW));
    bool fUnForward = ( (tsPre.lsForward == TLS_RED || tsPre.lsForward == TLS_YELLOW)
    					&& (tsAcross.lsForward == TLS_RED || tsAcross.lsForward == TLS_YELLOW)
    					&& (tsThree.lsForward == TLS_RED || tsThree.lsForward == TLS_YELLOW));
    bool fUnTurn = ( (tsPre.lsTurn == TLS_RED || tsPre.lsTurn == TLS_YELLOW)
    		        && (tsAcross.lsTurn == TLS_RED || tsAcross.lsTurn == TLS_YELLOW)
    		        && (tsThree.lsTurn == TLS_RED || tsThree.lsTurn == TLS_YELLOW));

    //ֻҪ������ʻ�ĳ������Ͷ�Ӧ�ĵ�״̬���ǽ��еģ��ǾͲ��Ǵ���ơ�
    //��֮���ǡ�
    fRet = true;
    if( ((runroadtype & RRT_FORWARD) && !fUnForward)
        || ((runroadtype & RRT_LEFT) && !fUnLeft) 
        || ((runroadtype & RRT_RIGHT) && !fUnRight)
        || ((runroadtype & RRT_TURN) && !fUnTurn)
        )
    {
        fRet = false;
    }

    if (runtype != RT_UNSURE )
    {
        //�����ʻ������ȷ��
        //��ֻҪ��Ӧ�ĵ�״̬�ǽ��еģ����Ǵ���ơ���֮���ǡ�
        if ((runtype == RT_FORWARD && fUnForward)
            || (runtype == RT_LEFT && fUnLeft)
            || (runtype == RT_RIGHT && fUnRight)
            || (runtype == RT_TURN && fUnTurn)
            )
        {
            fRet = true;
        }
        else
        {
            fRet = false;
        }
    }

    return fRet;
}


HRESULT CEPAppTracker::CheckCapture(
    HV_COMPONENT_IMAGE* pImage
    )
{
    // TODO ������cur��Ϊʲô��last��? ԭ���Ĵ����������д�ģ�ԭ����
    // ��˵��cur���ɿ�����last��ͳ�Ƴ����ģ���Ҫȷ��һ����㡣�������ݶ���ʶ��ӿ������
	// cur:��ǰ����һ֡��״̬�����״̬��һ������ȷ�ģ��п��ܻ�û�ﵽ�ȶ�
	// last:�����һ���ȶ���״̬��
    int iLightStatus = m_iLastLightStatus; 
    
    HV_COMPONENT_IMAGE& imgFrame = *pImage;
    int iFrameHeight = imgFrame.iHeight;
    int iPosStop  = ((m_cEPParam.iStopLinePos - 10) * iFrameHeight / 100);

    int iPosLeftStop = ((m_cEPParam.iLeftStopLinePos - 10) * iFrameHeight / 100);
    int nTrackPos = (m_cEPParam.iTrackAreaPos * iFrameHeight / 100);
    int nCapPos1 = (m_cEPParam.rgiCapturePos[0] * iFrameHeight / 100);
    int nCapPos2 = (m_cEPParam.rgiCapturePos[1] * iFrameHeight / 100);
    int nCapPos3 = (m_cEPParam.rgiCapturePos[2] * iFrameHeight / 100);

    for (int index=0; index<m_iTrackInfoCnt; ++index)
    {      
        CEPTrackInfo& cTrackInfo = m_rgTrackInfo[index];
        CRect rcPos = cTrackInfo.GetLastPos();

        //< ��¼����ʱ�ĺ��̵�״̬
        //�Ƿ����,��¼����ʱ�ĺ��̵�״̬
        // �ø��ٿ�ĵײ����жϡ�
        if(cTrackInfo.m_iPassStopLightScene == -1 && rcPos.bottom < iPosStop )
        {
            cTrackInfo.m_iPassStopLightScene = iLightStatus;

            cTrackInfo.m_nCarArriveTime = m_pCurIRefImage->GetRefTime(); // TODO ��ʱ��ֹͦ��ʱ�䣬��������ת����Ӧ���޸�
        }
        if( cTrackInfo.m_iPassLeftStopLightScene == -1 && rcPos.bottom < iPosLeftStop )
        {
            cTrackInfo.m_iPassLeftStopLightScene = iLightStatus;
        }

        // ��������쳣���⵼�µ����г��ߡ�
        if( rcPos.bottom > iPosStop )
        {
        	cTrackInfo.m_iPassStopLightScene = -1;
        }
        if(  rcPos.bottom > iPosLeftStop )
        {
        	cTrackInfo.m_iPassLeftStopLightScene = -1;
        }

        //>

        //������ͼץ�Ĵ���
        if (cTrackInfo.GetLastPlateRecogPosCount() == cTrackInfo.GetPosCount()
            && cTrackInfo.GetLastPlateDetPosCount() == cTrackInfo.GetPosCount()
            ||
            // �㷨���ø�֡���ʶ�����ʱ�����ʶ���ڼ�����һ֡�������
            // ��ʱץ����ֻ������һ֡�ģ�Ӧ��Ӱ�첻��
            cTrackInfo.GetLastPlateRecogPosCount() == cTrackInfo.GetPosCount() -1
            && cTrackInfo.GetLastPlateDetPosCount() == cTrackInfo.GetPosCount() -1
            )  
        {
            // ץ��������ͼ
            // �����֡����ʶ���ҿ��Ŷȱȴ�ǰ���������������ͼ
            float fltCurPlateConf = cTrackInfo.GetLastPlateConf();
            if (fltCurPlateConf > cTrackInfo.m_fltMaxPlateConf)
            {
                cTrackInfo.m_fltMaxPlateConf = fltCurPlateConf;

//                 //��βģʽ��������ǵ÷��������
//                 ReleaseIReferenceComponentImage(cTrackInfo.m_pimgLastSnapShot);
//                 cTrackInfo.m_pimgLastSnapShot = m_pCurIRefImage;
//                 cTrackInfo.m_pimgLastSnapShot->AddRef();

                cTrackInfo.m_rgRect[1] = rcPos; 
                cTrackInfo.m_rcLastPos = rcPos; 
            }
        }

        // ����쵽�ĳ���ͼ����������������ͼ����
        if (cTrackInfo.GetLastPlateDetPosCount() == cTrackInfo.GetPosCount())
        {
            // ע�⣬�㷨���ܲ��ø�֡���ʶ����ƣ���˳���ֻ��λ����Ϣ��������Ч
            svEPApi::EP_PLATE_INFO cPlateInfo;
            cTrackInfo.GetPlate(&cPlateInfo);

            sv::SV_RECT rcDetPos = cPlateInfo.rcDetPos;

            // 4����������

            rcDetPos.m_nLeft &= ~1;
            int iWidthPlate = rcDetPos.m_nRight - rcDetPos.m_nLeft;
            int iOffset = iWidthPlate % 4;
            if( iOffset != 0 )
            {
            	rcDetPos.m_nRight -= iOffset;
            }

            // TODO ����߼��е�ǣǿ�ɣ������һ��ʶ�𲻺��أ���Ϊ�ڵײ������ſ��Ŷȳ���
            // ���û������ɾ��ע��
            //Τ�����޸�,2011-2-28
            //����ǳ�βģʽ,�����������ǵ�һ�š�   ��
            bool fUpdate = true;
            //                 if( m_rgBestPlateInfo[nPlateType].fltConfidence != 0 )
            //                 {
            //                     fUpdate = false;
            //                 }

            EPBestPlateInfo& cBestPlateInfo = cTrackInfo.m_cLastDetPlateInfo; 
            if( rcDetPos.m_nBottom < nTrackPos 
                && cBestPlateInfo.pimgPlate != NULL)
            {
                fUpdate = false;
            }

            if( fUpdate )
            {
                cBestPlateInfo.dwFrameTime = m_pCurIRefImage->GetRefTime();

                // m_bestPlatetoBinImage �������ò��û���ã�

                //                     //weikt �Ѵ�����ĳ���Сͼ��������,�������ɶ�ֵͼ 2011-5-3
                //                     m_bestPlatetoBinImage.fltConfidence = LastInfo().fltTotalConfidence;
                //                     m_bestPlatetoBinImage.rcPlatePos = LastInfo().rcPlatePos;
                //                     m_bestPlatetoBinImage.rcPos = LastInfo().rcPos;
                //                     m_bestPlatetoBinImage.dwFrameTime = LastInfo().dwFrameTime;
                // 
                //                     SAFE_RELEASE(m_bestPlatetoBinImage.pimgPlate);
                //                     RTN_HR_IF_FAILED(CreateReferenceComponentImage(&m_bestPlatetoBinImage.pimgPlate,
                //                         HV_IMAGE_YUV_422,
                //                         imgRectifiedPlate.iWidth,
                //                         imgRectifiedPlate.iHeight
                //                         ));
                //                     if(m_bestPlatetoBinImage.pimgPlate != NULL)
                //                     {
                //                         RTN_HR_IF_FAILED(m_bestPlatetoBinImage.pimgPlate->Assign(imgRectifiedPlate));
                //                     }
                //weikt end

                // �������ö���
                // ��ԭʼ��Сͼ�����ô�����ġ�
                HV_COMPONENT_IMAGE imgCropped;
                imgCropped.nImgType = HV_IMAGE_YUV_422;
//                 CRect rectPlate = RECT_SV2HV(cPlateInfo.rcPlatePos);
// 
//                 if( rectPlate.left % 2 != 0 )
//                 {
//                     rectPlate.left -= 1;
//                 }
//                 rectPlate.bottom += (rectPlate.Height() * 90 / 100);
//                 rectPlate.right +=  (rectPlate.Width() * 45 / 100);
//                 if( rectPlate.right % 2 != 0 )
//                 {
//                     rectPlate.right -= 1;
//                 }

                HV_COMPONENT_IMAGE imgPlateCrop;
                CropImage(imgFrame, RECT_SV2HV(rcDetPos), &imgPlateCrop);

//                 if( (rectPlate.right) < SVRECT_WIDTH(rcDetPos)
//                     && (rectPlate.bottom) < SVRECT_HEIGHT(rcDetPos) )
//                 {
//                     CropImage(imgPlateCrop, rectPlate, &imgCropped);
//                 }
//                 else
                {
                    imgCropped = imgPlateCrop;
                }

                ReleaseIReferenceComponentImage(cBestPlateInfo.pimgPlate);
                RTN_HR_IF_FAILED(CreateReferenceComponentImage(&cBestPlateInfo.pimgPlate,
                    HV_IMAGE_YUV_422,
                    imgCropped.iWidth,
                    imgCropped.iHeight
                    ));
                if (imgCropped.nImgType != HV_IMAGE_YUV_422)
                {
                    RTN_HR_IF_FAILED(cBestPlateInfo.pimgPlate->Convert(imgCropped));
                }
                else
                {
                    RTN_HR_IF_FAILED(cBestPlateInfo.pimgPlate->Assign(imgCropped));
                }
            }
        }

        //��ѳ���ͼ����
        if (cTrackInfo.m_cLastDetPlateInfo.pimgPlate != NULL)  
        {
            if (cTrackInfo.GetLastPlateRecogPosCount() == cTrackInfo.GetPosCount() 
                || 
                // ��֡ʶ���ʶ����
                cTrackInfo.GetLastPlateRecogPosCount() == cTrackInfo.GetPosCount() -1
                && cTrackInfo.GetLastPlateDetPosCount() == cTrackInfo.GetPosCount() -1)
            {
                svEPApi::EP_PLATE_INFO cPlateInfo;
                cTrackInfo.GetPlate(&cPlateInfo);
                sv::CSvRect rcPlatePos = cPlateInfo.rcPos;

                EPBestPlateInfo& cBestPlateInfo = cTrackInfo.m_rgBestPlateInfo[cPlateInfo.nPlateType]; 
                
                if (cBestPlateInfo.pimgPlate != NULL)
                {
                   if (0 != memcmp(cPlateInfo.rgbContent, cBestPlateInfo.rgbContent, sizeof(cBestPlateInfo.rgbContent)))
                   {
                       cBestPlateInfo.fltConfidence -= cBestPlateInfo.fltConfidence / 3;
                   }
                   else
                   {
                       cBestPlateInfo.fltConfidence += cBestPlateInfo.fltConfidence / 3;
                   }
                }

                if (cBestPlateInfo.pimgPlate == NULL // û��ͼ����ֱ��ץ��һ��
                    || rcPlatePos.CenterPoint().m_nY > nTrackPos
                    && cPlateInfo.fltConf > cBestPlateInfo.fltConfidence
                    && cPlateInfo.fltFirstConf > 0.2 
                    && memcmp(cPlateInfo.rgbContent, cBestPlateInfo.rgbContent, sizeof(cBestPlateInfo.rgbContent)) != 0) // �������Ʋ�ͬ��ʱ�Ż�ͼ
                {
                    // ץ����������ͼ
                    sv::utTrace("������ͼ [%d] %d=%d(%d) %.2f %d\n", cTrackInfo.GetID(), cTrackInfo.GetLastPlateDetPosCount(), 
                        cTrackInfo.GetPlateRecogCount(), cTrackInfo.GetPosCount(),
                        cTrackInfo.GetLastPlateConf(), cPlateInfo.nPlateType);

                    // ��һ�δ������Ľ��ת�Ƶ���ѳ�����Ϣ��
                    cBestPlateInfo.Clear();
                    cTrackInfo.m_cLastDetPlateInfo.Detach(cBestPlateInfo);
  
                    cBestPlateInfo.fltConfidence = cTrackInfo.GetLastPlateConf();
                    cBestPlateInfo.rcPlatePos = RECT_SV2HV(cPlateInfo.rcPlatePos);
                    cBestPlateInfo.rcPos = RECT_SV2HV(cPlateInfo.rcPos);
                    memcpy(cBestPlateInfo.rgbContent, cPlateInfo.rgbContent, sizeof(cBestPlateInfo.rgbContent)); 

                    cTrackInfo.m_cLastDetPlateInfo.Clear();
                }
            }
        }
        
        // ������ץ��
        // 
        // ��һץ����
        // ���ݳ���������һץ��ͼλ��
        bool fFixFirstCapImage = (cTrackInfo.m_pimgBeginCapture != NULL 
            && cTrackInfo.GetLastPlateDetPosCount() == cTrackInfo.GetPosCount() // ��ǰ�쵽��
            && (cTrackInfo.GetLastPlateDetPos().bottom > nCapPos1
            		|| (cTrackInfo.GetLastPlateRecogPosCount() == cTrackInfo.GetPosCount() && cTrackInfo.GetLastPlateDetPos().bottom > nCapPos2) ) // �ж�λ�ã������һ��ʶ�𵽾ͱ�����¡�
            );
        if (cTrackInfo.m_rgfIsCapTime[0] || fFixFirstCapImage)  
        {    
            ReleaseIReferenceComponentImage(cTrackInfo.m_pimgBeginCapture);
            cTrackInfo.m_pimgBeginCapture = m_pCurIRefImage;
            cTrackInfo.m_pimgBeginCapture->AddRef();

            cTrackInfo.m_rgRect[2] = rcPos;
            cTrackInfo.m_iOnePosLightScene = iLightStatus;

            cTrackInfo.m_rgfIsCapTime[0] = false; // ��0ץ��ʱ���־
            sv::utTrace("==CAP [%d]_1. %s\n", cTrackInfo.GetID(), CapStr(&cTrackInfo));
        }

        // �ڶ�ץ����
        if (cTrackInfo.m_rgfIsCapTime[1])
        {
            ReleaseIReferenceComponentImage(cTrackInfo.m_pimgBestCapture);
            cTrackInfo.m_pimgBestCapture = m_pCurIRefImage;
            cTrackInfo.m_pimgBestCapture->AddRef();

            cTrackInfo.m_rcBestPos = rcPos; 

            cTrackInfo.m_rgRect[3] = rcPos;
            cTrackInfo.m_iTwoPosLightScene = iLightStatus; 

            cTrackInfo.m_rgfIsCapTime[1] = false;  // ��0ץ��ʱ���־
            sv::utTrace("==CAP [%d]_2. %s\n", cTrackInfo.GetID(), CapStr(&cTrackInfo));

            cTrackInfo.m_nCarArriveTime = m_pCurIRefImage->GetRefTime();
        }
        // ����ץ����
        // ��������ץ��ͼλ�ã��Ե���ץ��ͼλ���ӳ�
        bool fFixThirdCapImage = (cTrackInfo.m_pimgBestCapture != NULL 
            && (cTrackInfo.m_rcLastPos.bottom > nCapPos3  // m_rcLastPos��m_pimgLastSnapShot��Ҳ��ֵ���ǲ����߼��ص��ˣ�
            || cTrackInfo.m_pimgLastCapture == NULL) // LastCapImageΪ���жϽ���Ϊ�˱���m_rcLastPosû�б�����ֵ����� 
            && (m_pCurIRefImage->GetRefTime() - cTrackInfo.m_pimgBestCapture->GetRefTime() < 80*12*2   // 2s
            || m_pCurIRefImage->GetRefTime() - cTrackInfo.m_pimgBestCapture->GetRefTime() < 80*12*10   // 10s
            && cTrackInfo.m_rcLastPos.bottom > cTrackInfo.GetLastPos().bottom) 
            && !cTrackInfo.IsReverseRun()
            );
        //bool fFixThirdCapImage = false;
        if (cTrackInfo.m_rgfIsCapTime[2]
            || fFixThirdCapImage)
        {
            ReleaseIReferenceComponentImage(cTrackInfo.m_pimgLastCapture);
            cTrackInfo.m_pimgLastCapture = m_pCurIRefImage;
            cTrackInfo.m_pimgLastCapture->AddRef();

            cTrackInfo.m_rcLastPos = rcPos; 

            cTrackInfo.m_rgRect[4] = rcPos;
            cTrackInfo.m_iThreePosLightScene = iLightStatus; 

            cTrackInfo.m_rgfIsCapTime[2] = false;  // ��0ץ��ʱ���־
            sv::utTrace("==CAP [%d]_3. %s\n", cTrackInfo.GetID(), CapStr(&cTrackInfo));
        }

    }
    return S_OK;
}

HRESULT CEPAppTracker::CheckPeccancy(
    HV_COMPONENT_IMAGE* pImage
    )
{
    for (int i=0; i<m_iTrackInfoCnt; ++i)
    {      
        CEPTrackInfo& cTrackInfo = m_rgTrackInfo[i];

        if ((cTrackInfo.GetPosCount() % 3) == 0
            && CheckReverseRun(&cTrackInfo)) 
        {
            ++cTrackInfo.m_nReverseRunCount;
        }

    }
    return S_OK;
}

HRESULT CEPAppTracker::CheckOneState(CEPTrackInfo* pTrackInfo, HV_COMPONENT_IMAGE* pImage)
{
    // �״μ쵽����
    if (pTrackInfo->GetPosCount() == 1)
    {
        OnTrackerFirstDet(pTrackInfo);
    }

    CEPTrackInfo& TrackInfo = *pTrackInfo;
    if (TrackInfo.GetPlateRecogCount() != 0)
    {
        // ͨ����ʻ�����ж�
        if (!TrackInfo.m_fTrackReliable)
        {
            int iMinDist = (int)(m_iFrameHeight * 0.2f); // ��ʻ��С����
            HV_RECT rcFirst = TrackInfo.GetPos(0);
            HV_RECT rcLast = TrackInfo.GetLastPos();
            int iYDist = abs(rcFirst.bottom - rcLast.bottom);//abs((rcFirst.top+rcFirst.bottom) - (rcLast.top+rcLast.bottom)) >> 1;
            if (iYDist > iMinDist)
            {
                TrackInfo.m_fTrackReliable = true;
            }
            else if (TrackInfo.GetPlateRecogCount() > 4)  // ʶ�����>4Ҳ����
            {
                TrackInfo.m_fTrackReliable = true;
            }
        }
        //TrackInfo.m_fTrackReliable = true;  // ����ֱ�ӿ���

        if ( TrackInfo.m_pimgBestCapture != NULL 
            && TrackInfo.m_pimgBeginCapture == NULL )
        {
            TrackInfo.m_pimgBeginCapture = TrackInfo.m_pimgBestCapture;
            TrackInfo.m_pimgBeginCapture->AddRef();
        }
    }
    else
    {
        // ͨ����ʻ�����ж�
        if (!TrackInfo.m_fTrackReliable)
        {
            int iMinDist = (int)(m_iFrameHeight * 0.25f); // ��ʻ��С����
            HV_RECT rcFirst = TrackInfo.GetPos(0);
            HV_RECT rcLast = TrackInfo.GetLastPos();
            int iYDist = abs(rcFirst.bottom - rcLast.bottom);//abs((rcFirst.top+rcFirst.bottom) - (rcLast.top+rcLast.bottom)) >> 1;
            if (iYDist > iMinDist)
            {
                TrackInfo.m_fTrackReliable = true;
            }
            else
            {
                if (TrackInfo.GetPosCount() > 5)
                {
                    // ����ת���ܲ���Զ��������Ŀ��Է���һ��
                    if ( (iYDist > (int)(iMinDist*0.75f))
                        && (m_cRoadInfo.GetRoadNum(
                        hvPoint((rcLast.left+rcLast.right)>>1, (rcLast.top+rcLast.bottom)>>1))
                        == -1)
                        )
                    {
                        TrackInfo.m_fTrackReliable = true;
                    }
                }
            }
        }

        // ����ཻ����
        if ( TrackInfo.m_fTrackReliable && (TrackInfo.GetType() == CT_SMALL
            || TrackInfo.GetType() == CT_BIKE
            || TrackInfo.GetType() == CT_WALKMAN) )
        {
            if (IsOverlapWithBigCar(TrackInfo, m_rgTrackInfo, m_iTrackInfoCnt))
            {
                TrackInfo.m_fTrackReliable = false;
            }
        }

        // ���Ʊ���������ʱ���ж�����Ч����������Ϊ0����Ϊ���
        if (TrackInfo.GetState() == svEPApi::IEPTrack::TS_RESET
            && TrackInfo.GetValidDetCount() == 0)
        {
            TrackInfo.m_fTrackReliable = false;
        }
    }

    // ȥ����������ֹͣ�ߵĳ�
    if (TrackInfo.m_pimgLastCapture != NULL) 
    {
        int nCapPosLast = m_cEPParam.rgiCapturePos[2] * pImage->iHeight / 100; 
        CRect rcLast = TrackInfo.GetLastPos();
        int nCheckOutPos = rcLast.bottom - (rcLast.Height() >> 2);

        if (nCheckOutPos < nCapPosLast)
        {
            TrackInfo.Reset();
            sv::utTrace("App Reset [%d]. is out.\n", TrackInfo.GetID());
        }
    }

    return S_OK;
}

HRESULT CEPAppTracker::CheckPeccancyType(
    int iPreLight,			//ѹ��ǰ�ĺ��̵Ƴ���
    int iAcrossLight,		//���ߺ�ĺ��̵Ƴ���
    int iThreeLight,		//���ߺ�ĺ��̵Ƴ���
    RUN_TYPE runtype,	//��ʻ����
    int runroadtype,      //��ʻ��������
    DWORD32 dwCaptureTime,	//ץ��ʱ��
    /*PECCANCY_TYPE*/int * pPeccancyType //Υ������
    )
{
    if(pPeccancyType == NULL)
    {
        return E_POINTER;
    }
    *pPeccancyType = PT_NORMAL;
    //�ƹ����޸�Υ�����ͣ��ж���Υ�����͵ľ����������2010-10-12
    if( (runtype == RT_CONVERSE))
    {
        *pPeccancyType |= PT_CONVERSE; //����
    }
    if( (runroadtype & RRT_ESTOP) )
    {
        *pPeccancyType |= PT_ESTOP; //�ǻ�������
    }
    if(!(*pPeccancyType & PT_CONVERSE) && !(*pPeccancyType & PT_ESTOP) && CheckRush(iPreLight, iAcrossLight, iThreeLight, runroadtype, runtype, dwCaptureTime) )
    {
        *pPeccancyType |= PT_RUSH; //�����
    }
    if(!(*pPeccancyType & PT_CONVERSE) && !(*pPeccancyType & PT_ESTOP) && !CheckRoadAndRun(runtype, runroadtype))
    {
        *pPeccancyType |= PT_ACROSS; //����������ʻ
    }

    return S_OK;
}

HRESULT CEPAppTracker::CheckTrafficLight(HV_COMPONENT_IMAGE* pSceneImage)
{
    //���ú��̵ƽӿ�,���ص�ǰ֡�Ƶ�״̬���ȶ�ʶ���״̬
    return m_cTrafficLight.RecognizeTrafficLight(pSceneImage, &m_iCurLightStatus, &m_iLastLightStatus, 
        m_nEnvLightType == svEPApi::EP_RESULT_INFO::LT_NIGHT);
}

HRESULT CEPAppTracker::SetObjDetCfgParam(
    TRACKER_CFG_PARAM* pCfgParam,
    int iFrameWidth, 
    int iFrameHeight
    )
{
	// set roadinfo
    svEPApi::EP_PARAM& cParam = m_cEPParam;


	m_iRoadNumberBegin = pCfgParam->iRoadNumberBegin;							//���������ʼ����
	m_iStartRoadNum = pCfgParam->iStartRoadNum;								//��ʼ������
    cParam.iRoadInfoCount = pCfgParam->nRoadLineNumber; //��������

    // ������Ƶ�������
    // pCfgParam->cTrapArea

    // zhaopy
    // �����������Ǵ��ұ߿�ʼ��Ҫת����
    if(  m_iRoadNumberBegin == 1 )
    {
    	int index = 0;
    	for (int i = (cParam.iRoadInfoCount - 1); i>=0; --i, ++index)
		{
			cParam.rgRoadLine[index].ptTop.m_nX = pCfgParam->rgcRoadInfo[i].ptTop.x;
			cParam.rgRoadLine[index].ptTop.m_nY = pCfgParam->rgcRoadInfo[i].ptTop.y;
			cParam.rgRoadLine[index].ptBottom.m_nX = pCfgParam->rgcRoadInfo[i].ptBottom.x;
			cParam.rgRoadLine[index].ptBottom.m_nY = pCfgParam->rgcRoadInfo[i].ptBottom.y;
		}
    	for(int i = 0; i < cParam.iRoadInfoCount; ++i)
    	{
    		cParam.rgRoadLine[i].iRoadType = pCfgParam->rgcRoadInfo[i].iRoadType;
    	}
    }
    else
    {
    	for (int i=0; i<cParam.iRoadInfoCount; ++i)
		{
			cParam.rgRoadLine[i].ptTop.m_nX = pCfgParam->rgcRoadInfo[i].ptTop.x;
			cParam.rgRoadLine[i].ptTop.m_nY = pCfgParam->rgcRoadInfo[i].ptTop.y;
			cParam.rgRoadLine[i].ptBottom.m_nX = pCfgParam->rgcRoadInfo[i].ptBottom.x;
			cParam.rgRoadLine[i].ptBottom.m_nY = pCfgParam->rgcRoadInfo[i].ptBottom.y;
			cParam.rgRoadLine[i].iRoadType = pCfgParam->rgcRoadInfo[i].iRoadType;
		}
    }


   
    // ����Tracker�ĳ�����Ϣ
    HV_ROAD_LINE_INFO rgRoadInfo[10];
    for (int i=0; i<cParam.iRoadInfoCount; ++i)
    {
        rgRoadInfo[i].ptTop.x = cParam.rgRoadLine[i].ptTop.m_nX * 100 / iFrameWidth; 
        rgRoadInfo[i].ptTop.y = cParam.rgRoadLine[i].ptTop.m_nY * 100 / iFrameHeight; 
        rgRoadInfo[i].ptBottom.x = cParam.rgRoadLine[i].ptBottom.m_nX * 100 / iFrameWidth; 
        rgRoadInfo[i].ptBottom.y = cParam.rgRoadLine[i].ptBottom.m_nY * 100 / iFrameHeight; 
        rgRoadInfo[i].iRoadType = cParam.rgRoadLine[i].iRoadType;
    }
    m_cRoadInfo.Init(iFrameWidth, iFrameHeight, rgRoadInfo, cParam.iRoadInfoCount);
    CEPTrackInfo::SetRoadInfo(&m_cRoadInfo);    // �������ó���

    // ����ת��Ϊ�ٷֱ�
    for (int i=0; i<cParam.iRoadInfoCount; ++i)
    {
        cParam.rgRoadLine[i].ptTop.m_nX = cParam.rgRoadLine[i].ptTop.m_nX * 100 / iFrameWidth; 
        cParam.rgRoadLine[i].ptTop.m_nY = cParam.rgRoadLine[i].ptTop.m_nY * 100 / iFrameHeight; 
        cParam.rgRoadLine[i].ptBottom.m_nX = cParam.rgRoadLine[i].ptBottom.m_nX * 100 / iFrameWidth; 
        cParam.rgRoadLine[i].ptBottom.m_nY = cParam.rgRoadLine[i].ptBottom.m_nY * 100 / iFrameHeight; 

        printf("Road%d, [%d %d] [%d %d]\n", i, cParam.rgRoadLine[i].ptTop.m_nX, cParam.rgRoadLine[i].ptTop.m_nY,
            cParam.rgRoadLine[i].ptBottom.m_nX, cParam.rgRoadLine[i].ptBottom.m_nY);
    }

	// DetArea
	cParam.rcDetArea = *(sv::SV_RECT*)&(pCfgParam->cVideoDet.rcVideoDetArea);

	// cap pos
	cParam.rgiCapturePos[0] = pCfgParam->nCaptureOnePos;
	cParam.rgiCapturePos[1] = pCfgParam->nCaptureTwoPos;
	cParam.rgiCapturePos[2] = pCfgParam->nCaptureThreePos;
    
	// track pos
	cParam.iTrackAreaPos = pCfgParam->nCaptureTwoPos ;

	cParam.nTh = 4;
	
    if (iFrameWidth >= 2800)
    {
        cParam.fltXScale = 0.2f;
        cParam.fltYScale = 0.2f;//0.25f;
    }   
    else if (iFrameWidth >= 2440)
    {
        cParam.fltXScale = 0.25f;
        cParam.fltYScale = 0.25f;
    }        
    else if (iFrameWidth >= 1920)
    {
        cParam.fltXScale = 0.33f;
        cParam.fltYScale = 0.33f;
    }

	// dbg
// 	cParam.rcDetArea = sv::svRect(2, 40, 96, 59);
// 	cParam.iTrackAreaPos = 60;
// 	cParam.rgiCapturePos[0] = 80;
// 	cParam.rgiCapturePos[1] = 68;
// 	cParam.rgiCapturePos[2] = 53;
// 
// 	cParam.fltXScale = 0.25f;
// 	cParam.fltYScale = 0.25f;
	cParam.iDuskMaxLightTH = 65;
	cParam.iNightMaxLightTH = 45;
// 
// 	cParam.iSmallCarMinWidthRadio = 42;

//     m_pHvParam->GetInt(
//         "Tracker\\TrafficLight","OnePos",
//         &cParam.rgiCapturePos[0], cParam.rgiCapturePos[0],
//         0,100,
//         "ץ�ĵ�һ��ͼ��λ��","",5
//         );
//     m_pHvParam->GetInt(
//         "Tracker\\TrafficLight","TwoPos",
//         &cParam.rgiCapturePos[1], cParam.rgiCapturePos[1],
//         0,100,
//         "ץ�ĵڶ���ͼ��λ��","",5
//         );
//     m_pHvParam->GetInt(
//         "Tracker\\TrafficLight","ThreePos",
//         &cParam.rgiCapturePos[2], cParam.rgiCapturePos[2],
//         0,100,
//         "ץ�ĵ�����ͼ��λ��","",5
//         );

    //  model param , load
#define SV_INIT_MOD_DET_PARAM(model, pbdata, datelen, dettype, divx, dixy, scalenum, scalecoef, mergenum, maxroi, roadratio)\
    model.pbData = pbdata;\
    model.nDataLen = datelen;\
    model.nDetType = dettype;\
    model.nStepDivX = divx;\
    model.nStepDivY = dixy;\
    model.nScaleNum = scalenum;\
    model.fltScaleCoef = scalecoef;\
    model.nMergeNum = mergenum;\
    model.nMaxROI = maxroi;\
    model.fltRoadRatio = roadratio

    // ����С������
    SV_INIT_MOD_DET_PARAM(
        m_rgModelParam[0],
        svEPData::g_cEPDetModDaySmall.pbData, 
        svEPData::g_cEPDetModDaySmall.nDataLen,
        svEPApi::MOD_DET_INFO::DAY_SMALL_CAR,
        16, 8, 1, 0.9f, 2, 512, 
		0.6f             //�����Գ�����ȡ� TODO �����Ͽ�ĸ�0.5
        );
    // ����󳵲���
    SV_INIT_MOD_DET_PARAM(  
        m_rgModelParam[1],
        svEPData::g_cEPDetModDayBig.pbData, 
        svEPData::g_cEPDetModDayBig.nDataLen,
        svEPApi::MOD_DET_INFO::DAY_BIG_CAR,
        16, 8, 2, 0.7f, 2, 512, 
		1.1f			//�����Գ�����ȡ� TODO �����Ͽ�ĸ�1.0
        );
    // ����С������
    SV_INIT_MOD_DET_PARAM(
        m_rgModelParam[2], 
        svEPData::g_cEPDetModNightSmall.pbData, 
        svEPData::g_cEPDetModNightSmall.nDataLen,
        svEPApi::MOD_DET_INFO::NIGHT_SMALL_CAR,
        16, 8, 1, 0.75f,  2, 512, 
		0.6f			//�����Գ�����ȡ� TODO �����Ͽ�ĸ�0.5
        );
    // ���ϴ󳵲���
    SV_INIT_MOD_DET_PARAM(
        m_rgModelParam[3], 
        svEPData::g_cEPDetModNightBig.pbData, 
        svEPData::g_cEPDetModNightBig.nDataLen,
        svEPApi::MOD_DET_INFO::NIGHT_BIG_CAR,
        16, 8, 2, 0.7f,   2, 512, 
		1.1f			//�����Գ�����ȡ� TODO �����Ͽ�ĸ�1.0
        );

    // ֹͣ��λ��(ֱ��)
    cParam.iStopLinePos = pCfgParam->cTrafficLight.nStopLinePos;
    // ֹͣ��λ��(��ת)
    cParam.iLeftStopLinePos = pCfgParam->cTrafficLight.nLeftStopLinePos;

    return S_OK;
}

HRESULT CEPAppTracker::SetPlateRecogCfgParam(
    TRACKER_CFG_PARAM* pCfgParam,
    PlateRecogParam* pRecogParam
    )
{
    svEPApi::EP_PARAM& cParam = m_cEPParam;
    cParam.g_nDetMinScaleNum = pRecogParam->g_nDetMinScaleNum;          // ���Ƽ������С���=56*1.1^g_nDetMinScaleNum
    cParam.g_nDetMaxScaleNum = pRecogParam->g_nDetMaxScaleNum;          // ���Ƽ���������=56*1.1^g_nDetMaxScaleNum
    cParam.g_nDetMinStdVar = pRecogParam->g_nDetMinStdVar;            // ����>g_nDetMinStdVar���������Ϊ�����ǳ���
    cParam.g_nMinStopPlateDetNum = pRecogParam->g_nMinStopPlateDetNum;      // �ﵽ���������ֹͣ���
    cParam.g_fSubscanPredict = pRecogParam->g_fSubscanPredict;         // trackingʱ�Ƿ�ʹ��Ԥ���㷨
    cParam.g_nSubscanPredictMinScale = pRecogParam->g_nSubscanPredictMinScale;  // trackingԤ��ʱscale�仯��Χ����Сֵ
    cParam.g_nSubscanPredictMaxScale = pRecogParam->g_nSubscanPredictMaxScale;  // trackingԤ��ʱscale�仯��Χ�����ֵ

    cParam.g_kfltPlateInflateRate = pRecogParam->g_kfltPlateInflateRate.ToFloat();   // ��������ı���
    cParam.g_kfltPlateInflateRateV = pRecogParam->g_kfltPlateInflateRateV.ToFloat();  // �������ߵı���
    cParam.g_kfltHighInflateArea = pRecogParam->g_kfltHighInflateArea.ToFloat();        // ���Ƶ�yֵ>g_kfltHighInflateArea���õڶ��׸���һ��ı������ͼ���
    cParam.g_kfltPlateInflateRate2 = pRecogParam->g_kfltPlateInflateRate2.ToFloat();  // ����ļ�������ı���
    cParam.g_kfltPlateInflateRateV2 = pRecogParam->g_kfltPlateInflateRateV2.ToFloat(); // ����ļ������ߵı���

    cParam.g_fSegPredict = pRecogParam->g_fSegPredict;             // �Ƿ�ʹ���зֵ�Ԥ���㷨

    cParam.g_nMinPlateWidth = pRecogParam->g_nMinPlateWidth;           // ����ʶ�����С���ƿ�ȣ��Էָ��Ľ��Ϊ׼��
    cParam.g_nMaxPlateWidth = pRecogParam->g_nMaxPlateWidth;           // ����ʶ�������ƿ�ȣ��Էָ��Ľ��Ϊ׼��
    cParam.g_fBestResultOnly = pRecogParam->g_fBestResultOnly;         // ���ͼ�����ж��ʶ������ֻ�����õ�

    cParam.g_nMinDownSampleWidthInSeg = pRecogParam->g_nMinDownSampleWidthInSeg; // �ָ�ʱDownSample����С���
    cParam.g_nMinDownSampleHeightInSeg = pRecogParam->g_nMinDownSampleHeightInSeg;// �ָ�ʱDownSample����С�߶�

    // �ϰ��������
    // ע�⣬���غ���û�г�ʼ����ֵ 
    cParam.g_nContFrames_EstablishTrack = 3;

    // fEnableProcessBWPlate
    cParam.fEnableProcessBWPlate = pCfgParam->nEnableProcessBWPlate;

    // iPlateResizeThreshold
    cParam.iPlateResizeThreshold = pCfgParam->nPlateResizeThreshold;

    // �ָ�ǰ�Ƿ����ͼƬ��ǿ
    cParam.fEnablePlateEnhance = pCfgParam->nEnablePlateEnhance;

    //����ʶ�𿪹�
    cParam.fEnableBigPlate = pCfgParam->nEnableBigPlate;

    //Ĭ��˫�������Ϳ���
    cParam.iEnableDefaultDBType = pCfgParam->nDoublePlateTypeEnable;

    // Ĭ��˫��������
    cParam.nDefaultDBType = pCfgParam->nDefaultDBType;

    //��Ļ�м䵥�����ƺ����ֿ��
    cParam.iScrMidFiveCharWidth = pCfgParam->nMiddleWidth;

    //��Ļ�ײ��������ƺ����ֿ��
    cParam.iScrBotFiveCharWidth = pCfgParam->nBottomWidth;

    //��ǿ����ͼʶ��
    cParam.fUseEdgeMethod = pCfgParam->fUseEdgeMethod;

    //������ĸʶ�𿪹�
    cParam.fEnableAlpha_5 = pCfgParam->fEnableAlpha_5;

    //�������侯�ַ�����
    cParam.fEnableDefaultWJ = pCfgParam->fEnableDefaultWJ;

    //�������侯�ַ�
    memcpy(cParam.strDefaultWJChar, pCfgParam->szDefaultWJChar, sizeof(pCfgParam->szDefaultWJChar));
    
    //�ط�����ʶ�𿪹�
    cParam.fRecogGxPolice = pCfgParam->nRecogGxPolice;

    // ���ػ����֣�58Ϊ����
    cParam.nDefaultChnId = 58;
//     m_pHvParam->GetInt(
//         "Tracker\\Recognition\\DefaultChn","CharID", 
//         &cParam.nDefaultChnId, cParam.nDefaultChnId,
//         38,104,
//         "���ػ����ֱ��","",1
//         );

    //���Ƽ�ⷽ�����
    cParam.iMinVariance = 9;
    BOOL fMinVarianceUserCtrl = FALSE; // m_fMinVarianceUserCtrl;
    if (fMinVarianceUserCtrl)
    {
        cParam.g_nDetMinStdVar = cParam.iMinVariance;
    }
    else
    {
        cParam.iMinVariance = cParam.g_nDetMinStdVar;
    }

//     m_pHvParam->GetInt(
//         "Tracker\\Misc","MinVariance", 
//         &cParam.iMinVariance, cParam.iMinVariance,
//         0,120,
//         "���˷ǳ���������ֵ","",1
//         );

    cParam.nPlateDetect_Green = pCfgParam->nPlateDetect_Green;
    cParam.fltPlateDetect_StepAdj = pCfgParam->fltPlateDetect_StepAdj;

    cParam.fEnableAlphaRecog = pCfgParam->fAlpha7; 
    cParam.fEnableT1Model = pCfgParam->fEnableT1Model;
    cParam.fEnableAlpha_5 = pCfgParam->fEnableAlpha_5;

    cParam.nBlackPlate_S = pCfgParam->nProcessPlate_BlackPlate_S; // ���Ƶı��Ͷ�����
    cParam.nBlackPlate_L = pCfgParam->nProcessPlate_BlackPlate_L; // ������������
    cParam.nBlackPlateThreshold_H0 = pCfgParam->nProcessPlate_BlackPlateThreshold_H0; // ����ɫ������
    cParam.nBlackPlateThreshold_H1 = pCfgParam->nProcessPlate_BlackPlateThreshold_H1; // ����ɫ������
    cParam.nProcessPlate_LightBlue = pCfgParam->nProcessPlate_LightBlue; // ǳ���ƿ���
    cParam.fEnableDoublePlate = pCfgParam->fDoublePlateEnable; // ˫���ƿ���
    cParam.iSegWhitePlate = pCfgParam->nSegWhitePlate; //ǿ�ư��Ʒָ�

    // TODO pCfgParam ������
    cParam.fEnableNewWJSegment = TRUE;
    cParam.fRecogNewMilitary = TRUE;

    return S_OK;
}

void CEPAppTracker::UpdatemRedLightRect(HV_RECT* prgRect, const int& iCount)
{
	if( prgRect != NULL && iCount > 0 && iCount <= MAX_TRAFFICLIGHT_COUNT * 2)
	{
		memcpy(m_rgRedLightRect, prgRect, sizeof(HV_RECT) * iCount);
		m_nRedLightCount = iCount;
	}
}

HRESULT CEPAppTracker::InitTrafficLight(
    TRACKER_CFG_PARAM* pCfgParam
    )
{
    m_nRedLightCount = 0;
    memset(m_rgRedLightRect, 0, sizeof(m_rgRedLightRect));

    TRAFFIC_LIGHT_PARAM tlpTrafficLightParam ;

#if RUN_PLATFORM == PLATFORM_WINDOWS

    tlpTrafficLightParam = pCfgParam->cTrafficLight;
    //��������
    m_nLightCount = tlpTrafficLightParam.iLightGroupCount;
    //���δ��ֹͣ�ߵĳ���
    m_iOutputInLine = tlpTrafficLightParam.nOutputInLine;

    CTrackInfo::m_iCheckType = tlpTrafficLightParam.iCheckType;
    if( CTrackInfo::m_iCheckType == 1 )
    {
        tlpTrafficLightParam.iValidSceneStatusCount = 1;
    }

    //������õ���Ч����С��5����ʹ�ÿ���ת��ģʽ������ת��ģʽ�³����仯һ����˳��ġ�
    if( tlpTrafficLightParam.iValidSceneStatusCount < 5 )
    {
        tlpTrafficLightParam.fCheckSpeed = true;
    }

    if (m_nLightCount > 0)
    {
        m_cTrafficLight.SetTrafficLightParam(tlpTrafficLightParam);
    }

#else


    tlpTrafficLightParam.iCheckType = pCfgParam->cTrafficLight.nCheckType;
    tlpTrafficLightParam.iAutoScanLight = pCfgParam->cTrafficLight.nAutoScanLight;
    tlpTrafficLightParam.fDeleteLightEdge = pCfgParam->cTrafficLight.fDeleteLightEdge;
    tlpTrafficLightParam.iSceneCheckMode = pCfgParam->cTrafficLight.nSenceCheckMode;
    CTrackInfo::m_iStopLinePos = pCfgParam->cTrafficLight.nStopLinePos;
    CTrackInfo::m_iLeftStopLinePos = pCfgParam->cTrafficLight.nLeftStopLinePos;

    // zhaopy
    CTrackInfo::m_iRealForward = pCfgParam->cTrafficLight.iRealForward;
    tlpTrafficLightParam.iValidSceneStatusCount = pCfgParam->cTrafficLight.nValidSceneStatusCount;
    if( pCfgParam->cTrafficLight.nCheckType == 1 )
    {
        tlpTrafficLightParam.iValidSceneStatusCount = 1;
    }

    //������õ���Ч����С��5����ʹ�ÿ���ת��ģʽ������ת��ģʽ�³����仯һ����˳��ġ�
    if( tlpTrafficLightParam.iValidSceneStatusCount < 5 )
    {
        tlpTrafficLightParam.fCheckSpeed = true; // m_fCheckSpeed;
    }
    m_iOutputInLine = pCfgParam->cTrafficLight.nOutputInLine;
    tlpTrafficLightParam.iRedLightDelay = pCfgParam->cTrafficLight.nRedLightDelay;  
    CTrackInfo::m_iSpecificLeft = pCfgParam->cTrafficLight.nSpecificLeft;     
    CTrackInfo::m_iRunTypeEnable = pCfgParam->cTrafficLight.nRunTypeEnable;
    m_nLightCount = pCfgParam->cTrafficLight.nLightCount;
    tlpTrafficLightParam.iLightGroupCount = m_nLightCount;
    tlpTrafficLightParam.iTrafficLightBrightValue = pCfgParam->cTrafficLight.nTrafficLightBrightValue;
    tlpTrafficLightParam.iAutoScanThread = pCfgParam->cTrafficLight.nAutoScanThread;
    tlpTrafficLightParam.iSkipFrameNo = pCfgParam->cTrafficLight.nSkipFrameNO;
    tlpTrafficLightParam.fIsLightAdhesion = pCfgParam->cTrafficLight.fIsLightAdhesion;

    m_iFilterRushPeccancy = pCfgParam->cTrafficLight.nFilterRushPeccancy;
    //m_fFilterMoreReview = pCfgParam->cTrafficLight.nFilterMoreReview;  //�Ѿ�����Ҫ

    int p1(0),p2(0),p3(0),p4(0),p5(0),p6(0);
    char szLightState[32] = {0};

    for(int i = 0; i < m_nLightCount; i++)
    {
        sscanf(
            pCfgParam->cTrafficLight.rgszLightPos[i], 
            "(%d,%d,%d,%d),%d,%d,%s",&p1,&p2,&p3,&p4,&p5,&p6,
            szLightState
            );
        //����,ID,����,����(0=����,1=����)
        //m_rgLightInfo[i].SetRect(p1,p2,p3,p4,MIN_INT(CTrafficLightInfo::MAX_POS_COUNT, p5), p6);  // //�Ѿ�����Ҫ

        CRect rcTmp = CRect(p1,p2,p3,p4);
        tlpTrafficLightParam.rgtgiLight[i].iLightCount = MIN_INT(CTrafficLightInfo::MAX_POS_COUNT, p5);
        tlpTrafficLightParam.rgtgiLight[i].iLightType = p6;
        tlpTrafficLightParam.rgtgiLight[i].rcLight = rcTmp;

        if( p6 == 0 )
        {
        	 if( tlpTrafficLightParam.rgtgiLight[i].rcLight.Width()
        			 < tlpTrafficLightParam.rgtgiLight[i].iLightCount )
        	 {
        		 tlpTrafficLightParam.rgtgiLight[i].rcLight.left -= tlpTrafficLightParam.rgtgiLight[i].iLightCount;
        		 tlpTrafficLightParam.rgtgiLight[i].rcLight.left =
        				 tlpTrafficLightParam.rgtgiLight[i].rcLight.left >= 0 ? tlpTrafficLightParam.rgtgiLight[i].rcLight.left : 0;
        		 tlpTrafficLightParam.rgtgiLight[i].rcLight.right =
        				 tlpTrafficLightParam.rgtgiLight[i].rcLight.left + tlpTrafficLightParam.rgtgiLight[i].iLightCount;
        	 }
        }
        else if( p6 == 1 )
        {
			 if( tlpTrafficLightParam.rgtgiLight[i].rcLight.Height()
					 < tlpTrafficLightParam.rgtgiLight[i].iLightCount )
			 {
				 tlpTrafficLightParam.rgtgiLight[i].rcLight.top -= tlpTrafficLightParam.rgtgiLight[i].iLightCount;
				 tlpTrafficLightParam.rgtgiLight[i].rcLight.top =
						 tlpTrafficLightParam.rgtgiLight[i].rcLight.top >= 0 ? tlpTrafficLightParam.rgtgiLight[i].rcLight.top : 0;
				 tlpTrafficLightParam.rgtgiLight[i].rcLight.bottom =
						 tlpTrafficLightParam.rgtgiLight[i].rcLight.top + tlpTrafficLightParam.rgtgiLight[i].iLightCount;
			 }
        }


        if( strlen(szLightState) >= (p5 * 2) )
        {
            int index = 0;
            for(int k = 0; k < p5; ++k)
            {
                tlpTrafficLightParam.rgtgiLight[i].tpLightPos[k] = (TRAFFICLIGHT_POSITION)(szLightState[index] - '0');
                tlpTrafficLightParam.rgtgiLight[i].lsLightStatus[k] = (_TRAFFICLIGHT_LIGHT_STATUS)(szLightState[index+1] - '0');						

                if( (_TRAFFICLIGHT_LIGHT_STATUS)(szLightState[index+1] - '0') == TLS_RED 
                    && (m_nRedLightCount < MAX_TRAFFICLIGHT_COUNT * 2) )
                {
                    if( p6 == 0 )
                    {
                        int iTmpX = ((p3 - p1) / p5);
                        // zhaopy ������С�����ơ�
                        iTmpX = (iTmpX <= 0 ? 1 : iTmpX);
                        m_rgRedLightRect[m_nRedLightCount].left = iTmpX * k + p1;
                        m_rgRedLightRect[m_nRedLightCount].right = iTmpX * (k + 1) + p1;
                        m_rgRedLightRect[m_nRedLightCount].top = p2;
                        m_rgRedLightRect[m_nRedLightCount].bottom = p4;
                        m_nRedLightCount++;
                    }
                    else if( p6 == 1)
                    {
                        int iTmpY = ((p4 - p2) / p5);
                        iTmpY = (iTmpY <= 0 ? 1 : iTmpY);
                        m_rgRedLightRect[m_nRedLightCount].left = p1;
                        m_rgRedLightRect[m_nRedLightCount].right = p3;
                        m_rgRedLightRect[m_nRedLightCount].top = iTmpY * k + p2;
                        m_rgRedLightRect[m_nRedLightCount].bottom = iTmpY * (k + 1) + p2;
                        m_nRedLightCount++;
                    }
                }
                //SetTrafficLightType(i, k, (TRAFFICLIGHT_POSITION)(szLightState[index] - '0'), (_TRAFFICLIGHT_LIGHT_STATUS)(szLightState[index+1] - '0'));
                index += 2;
            }
        }
    }

    //���IO���̵�����
    char szIOLight[64] = {0};
    strcpy(szIOLight, pCfgParam->cTrafficLight.szIOConfig);
    if( strcmp(szIOLight, "00,00,00,00,00,00,00,00") != 0 )
    {
        int iIndex = (strlen(szIOLight) + 1) / 3;
        for(int i = 0; i < iIndex; ++i)
        {
            tlpTrafficLightParam.rgIOLight[i].iTeam = szIOLight[i * 3 + 0] - '0' - 1;
            tlpTrafficLightParam.rgIOLight[i].iPos = szIOLight[i * 3 + 1] - '0' - 1;
            tlpTrafficLightParam.rgIOLight[i].tpPos = tlpTrafficLightParam.rgtgiLight[tlpTrafficLightParam.rgIOLight[i].iTeam].tpLightPos[tlpTrafficLightParam.rgIOLight[i].iPos];
            tlpTrafficLightParam.rgIOLight[i].tlsStatus = tlpTrafficLightParam.rgtgiLight[tlpTrafficLightParam.rgIOLight[i].iTeam].lsLightStatus[tlpTrafficLightParam.rgIOLight[i].iPos]; 
//          TODO��m_rgIOLight�������ò��û������ʹ�õ�?
//             m_rgIOLight[i].iTeam = szIOLight[i * 3 + 0] - '0' - 1;
//             m_rgIOLight[i].iPos = szIOLight[i * 3 + 1] - '0' - 1;

            //GetTrafficLightType(m_rgIOLight[i].iTeam, m_rgIOLight[i].iPos, &m_rgIOLight[i]);
        }
    }

    tlpTrafficLightParam.iSceneCount = pCfgParam->cTrafficLight.nSceneCount;

    char szOtherInfo[64] = {0};
    int rgiLightStatus[4] = {0};

    //ClearLightScene();
    //ClearSceneInfo();
    for(int i = 0; i < MAX_SCENE_COUNT; i++)
    {
        memset(szOtherInfo, 0, 64);
        int nCount = sscanf(
            pCfgParam->cTrafficLight.rgszScene[i], 
            "(%d,%d,%d,%d),%s",
            &rgiLightStatus[0],
            &rgiLightStatus[1],
            &rgiLightStatus[2],
            &rgiLightStatus[3],
            szOtherInfo
            );
        if( 4 <= nCount)
        {
            TRAFFICLIGHT_SCENE ts;
            ts.lsLeft = (LIGHT_STATUS)rgiLightStatus[0];
            ts.lsForward = (LIGHT_STATUS)rgiLightStatus[1];
            ts.lsRight = (LIGHT_STATUS)rgiLightStatus[2];
            ts.lsTurn = (LIGHT_STATUS)rgiLightStatus[3];
            //SetLightScene(i, ts);
            SCENE_INFO tsInfo;
            strncpy(tsInfo.pszInfo, szOtherInfo, strlen(szOtherInfo) + 1);
            //SetSceneInfo(i, tsInfo);

            tlpTrafficLightParam.rgtsLightScene[i] = ts;
            memcpy(tlpTrafficLightParam.rgsiInfo[i].pszInfo, tsInfo.pszInfo, 64);
        }

        if (m_nLightCount > 0)
        {
            m_cTrafficLight.SetTrafficLightParam(tlpTrafficLightParam);

            // TODO
            m_cTrafficLight.SetTrafficLightCallback(m_pTrackerCallback);
        }
    }
#endif

    return S_OK;
}

HRESULT CEPAppTracker::SetMiscParam(
    TRACKER_CFG_PARAM* pCfgParam
    )
{
    m_iFilterRushPeccancy = pCfgParam->cTrafficLight.nFilterRushPeccancy;
    m_iAverageConfidenceQuan = pCfgParam->nAverageConfidenceQuan;
    m_iFirstConfidenceQuan = pCfgParam->nFirstConfidenceQuan;
    m_fltOverLineSensitivity = pCfgParam->fltOverLineSensitivity;

	//�¼�������
	if (true)
	{
		for (int i = 0; i < MAX_ROADLINE_NUM; i++)
		{
			m_ActionDetectParam.iIsCrossLine[i] = pCfgParam->cActionDetect.iIsCrossLine[i];
		}
	}
	if (true)
	{
		for (int i = 0; i < MAX_ROADLINE_NUM; i++)
		{
			m_ActionDetectParam.iIsYellowLine[i] = pCfgParam->cActionDetect.iIsYellowLine[i];
		}
	}
	m_ActionDetectParam.iDetectCarStopTime = pCfgParam->cActionDetect.iDetectCarStopTime;

    return S_OK;
}

int CEPAppTracker::Capture_CallBack(svEPApi::IEPTrack* pTrack, 
                                    int nCapPosIndex)
{
    if (m_pInstance != NULL)    
    {
        m_pInstance->DoCapture(pTrack, nCapPosIndex);
    }
    return 0;
}

HRESULT CEPAppTracker::DoCapture(svEPApi::IEPTrack* pITracker, int nCapPosIndex)
{
    // �ú������㷨���ӵ���
    // ע�⣺
    // 1.�������ٱ�������ʱ
    // 2.�쳣������Ч��Ӧ����Ϣ�ķ�ʽ������
    
    // TODO ǰ�˿���ץ��

    // ����ץ��ʱ���־
    if (nCapPosIndex >= 3)  return E_INVALIDARG;
    for (int i=0; i<m_iTrackInfoCnt; ++i)
    {
        if (m_rgTrackInfo[i].GetID() == pITracker->GetID())
        {
            m_rgTrackInfo[i].m_rgfIsCapTime[nCapPosIndex] = true;
        }
    }   
    return S_OK;
}

HRESULT CEPAppTracker::SetTrackerCallBack(ITracker* pCallBack)
{
	m_pTrackerCallback = pCallBack;
	return S_OK;
}


HRESULT CEPAppTracker::CalcCarSpeed(float &fltCarSpeed, float &fltScaleOfDistance, CEPTrackInfo* pTrackInfo, IScaleSpeed *pScaleSpeed)
{
	if( pTrackInfo == NULL || pScaleSpeed == NULL )
	{
		return E_POINTER;
	}

	const int iCalsCount = 3;
	float rgfltCarSpeed[iCalsCount] = {0.0f}, rgfltScale[iCalsCount] = {0.0f};
	if ( pTrackInfo->m_nPlateMovePosCount > 1 )
	{
		// ȡ��ǰ���ƺ͵�һ�����Ƶļ���ƽ���ٶȣ���Ϊ��ǰ���Ƶ��ٶ�
		CRect rcBegin = pTrackInfo->m_rgrcPlateMovePos[0];
		int iInfoIndex = pTrackInfo->m_nPlateMovePosCount - 1;
		for (int i = 0; i < iCalsCount; i++)
		{
			CRect rcEnd = pTrackInfo->m_rgrcPlateMovePos[iInfoIndex];
			//��Ҫ�ô�ĳ�����Ԥ�����
			if (rcEnd.Width() < rcBegin.Width())
			{
				rgfltCarSpeed[i] = m_pScaleSpeed->CalcCarSpeedNewMethod(
					rcEnd,
					rcBegin,
					pTrackInfo->m_rgdwPlateTick[iInfoIndex] - pTrackInfo->m_rgdwPlateTick[0],
					true,
					PLATE_NORMAL, //  todo.
					rgfltScale[i]
					);
			}
			else
			{
				rgfltCarSpeed[i] = m_pScaleSpeed->CalcCarSpeedNewMethod(
					rcBegin,
					rcEnd,
					pTrackInfo->m_rgdwPlateTick[iInfoIndex] - pTrackInfo->m_rgdwPlateTick[0],
					true,
					PLATE_NORMAL,
					rgfltScale[i]
					);
			}
			iInfoIndex--;
		}
		for (int i = 0; i < iCalsCount - 1; i++)
		{
			for (int j = i + 1; j< iCalsCount; j++)
			{
				if (rgfltCarSpeed[i] < rgfltCarSpeed[j])
				{
					float fltTemp = rgfltCarSpeed[i];
					rgfltCarSpeed[i] = rgfltCarSpeed[j];
					rgfltCarSpeed[j] = fltTemp;
					float fltTemp1 = rgfltScale[i];
					rgfltScale[i] = rgfltScale[j];
					rgfltScale[j] = fltTemp1;
				}
			}
		}
		for (int i = iCalsCount / 2; i >= 0; i--)
		{
			fltCarSpeed = rgfltCarSpeed[i];
			fltScaleOfDistance = rgfltScale[i];
			if (fltCarSpeed > 0.0f)
			{
				break;
			}
		}
		// �ٶ�ֵ��������������ٶ�ֵΪ0�����
		// huanggr 2011-11-07
		if (fltCarSpeed < 1.0f)
		{
			fltCarSpeed = 1.0f;
		}
	}
	return S_OK;
}


#if RUN_PLATFORM == PLATFORM_DSP_BIOS
extern "C" int DSP_dotprod (
    short * restrict x,    /* Pointer to first vector  */
    short * restrict y,    /* Pointer to second vector */
    int nx                 /* Length of vectors.       */
)
{
    int i;
    int sum1 = 0;
    int sum2 = 0;

    /* The kernel assumes that the data pointers are double word aligned */
    //_nassert((int)x % 8 == 0);
    //_nassert((int)y % 8 == 0);
    //_nassert(nx % 4 == 0);

    /* The kernel assumes that the input count is multiple of 4 */
    for (i = 0; i < nx; i+=4) {
        sum1 += _dotp2(_loll(_amem8_const(&x[i])),  _loll(_amem8_const(&y[i])));
        sum2 += _dotp2(_hill(_amem8_const(&x[i])),  _hill(_amem8_const(&y[i])));
    }

    return (sum1+sum2);
}
#endif
