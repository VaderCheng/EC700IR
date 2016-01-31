#include "AppTrackCtrl.h"
#include "hvutils.h"
#include "TrackInfoHigh.h"
#include "AppUtils.h" 
//#include "DetModelData.h"
#include "AppUtils.h"

#include "AsyncDetResult.h"

#if RUN_PLATFORM == PLATFORM_WINDOWS
#include "ExLib.h"
#include "DebugConsole.h"
#endif

using namespace sv;

#ifdef RTN_HR_IF_FAILED 
#undef RTN_HR_IF_FAILED
#define RTN_HR_IF_FAILED(func)                          \
{	                                                    \
    HRESULT hr=func;                                    \
    if (FAILED(hr))                                     \
{                                                   \
    char szMsg[256];								\
    sprintf(szMsg, "%s l:%d FAILED:%08X\n", #func, __LINE__, hr);\
    sv::utTrace(szMsg);                       \
    return hr;	                                    \
}	                                                \
}
#endif

#if RUN_PLATFORM == PLATFORM_WINDOWS
void ShowImgTrack(HV_COMPONENT_IMAGE* pimg, swTgApp::CAppTrackInfo* rgTrackInfo, int iInfoCnt, int iMs, char* szName = NULL)
{
    int* rgiInfoID = new int[iInfoCnt*2];
    HV_RECT* rgrcInfo = new HV_RECT[iInfoCnt*2];
    int iInfo1 = 0;
    int iInfo2 = 0;
    int* rgiInfoID2 = new int[iInfoCnt*2];
    HV_RECT* rgrcInfo2 = new HV_RECT[iInfoCnt*2];

    for (int i=0; i<iInfoCnt; ++i)
    {

        if (rgTrackInfo[i].GetPlateCount() != 0)
        {
            rgrcInfo2[iInfo2] = RECT_SV2HV(rgTrackInfo[i].GetLastPos());//rgTrackInfo[i].GetLastPlatePos();
            rgiInfoID2[iInfo2] = rgTrackInfo[i].GetID();
            iInfo2++;

            if (rgTrackInfo[i].GetLastLocus().m_pPlateInfo != NULL)
            {
                rgrcInfo2[iInfo2] = RECT_SV2HV(rgTrackInfo[i].GetLastLocus().m_pPlateInfo->rcPos);
                rgiInfoID2[iInfo2] = rgTrackInfo[i].GetID();
                iInfo2++;
            }
        }
        else if (rgTrackInfo[i].GetPosCount() != 1 
            || rgTrackInfo[i].GetState() != swTgApp::CAppTrackInfo::TS_END) 
        {
            rgrcInfo[iInfo1] = RECT_SV2HV(rgTrackInfo[i].GetLastPos());
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

namespace swTgApp 
{

    /// ץ�Ļص�����
    bool TriggerCallBack(
        sv::SV_RECT rcPos,      /// �ص���λ��
        int nTrackID,           ///
        void* pVoid
        )
    {
        return ((CAppTrackCtrl*)pVoid)->TriggerCallBack(rcPos, nTrackID);
    }

    //////////////////////////////////////////////////////////////////////////
    // CPlateLightCtrl
    // 

    CPlateLightCtrl::CPlateLightCtrl()
    {
        // zhaopy
        m_nFirstLightType = LIGHT_TYPE_COUNT;
        m_dwLastCarLeftTime = 0;
        m_LightType = DAY;
        m_fIsARMNight = FALSE;
        m_nPlateLightType = DAY_BACKLIGHT_2;	//Ĭ�ϴ��м俪ʼ
        m_nWDRLevel = 0;
        m_nMaxAGCTH = 150;
        m_nEnvAvgY = 0;
    }

    CPlateLightCtrl::~CPlateLightCtrl()
    {
    }

    void CPlateLightCtrl::Init(int nMaxPlateY, int nMinPlateY, int nCheckOut, int nNightTH, int nMaxAgcTh)
    {
        m_iMaxPlateBrightness = nMaxPlateY;
        m_iMinPlateBrightness = nMinPlateY;
        m_iPlateLightCheckCount = nCheckOut;
        m_iNightThreshold = nNightTH;
        m_nMaxAGCTH = nMaxAgcTh;
    }

    void CPlateLightCtrl::SetFirstLightType(LIGHT_TYPE nLightType)
    {
        m_nFirstLightType = nLightType;
        m_dwLastCarLeftTime = sv::utGetSystemTick();
    }

    HRESULT CPlateLightCtrl::UpdateLightType(int iCarY, bool fIsAvgBrightness)
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

        /*char szMsg[256];
        sprintf(szMsg, "<123ex>CheckLightType y:%d, flag:%d. count:%d. cur:%d",
        iCarY, fIsAvgBrightness, m_iPlateLightCheckCount, iFrameCount + 1);
        Venus_OutputDebug(szMsg);*/

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
            static DWORD32 s_dwTick = 0;
            DWORD32 dwCurTick = sv::utGetSystemTick();
            BOOL fCanUpdateWdrLevel = FALSE;
            // WDR����Ҫ�����������û��Ӧ������������������ڵȼ�
            if (dwCurTick < s_dwTick || (dwCurTick - s_dwTick) >= 15000)
            {
                fCanUpdateWdrLevel = TRUE;
            }

            int iAvgBrightness = (iSumBrightness - s_iMaxValue - s_iMinValue) / (iFrameCount - 2);
            if (iAvgBrightness < iMinBrightness && m_nPlateLightType < LIGHT_TYPE_COUNT)
            {
                if (m_nPlateLightType < LIGHT_TYPE_COUNT - 1)
                {
                    /*sprintf(szMsg, "<123ex>CheckLightType ++m_nPlateLightType:%d. iAvgBrightness:%d. Exp:(%d~%d),Count:%d",
                    m_nPlateLightType, iAvgBrightness, iMaxBrightness, iMinBrightness, iFrameCount);
                    Venus_OutputDebug(szMsg);*/
                    m_nPlateLightType = (LIGHT_TYPE)((int)m_nPlateLightType + 1);
                }
                //�������¿���WDR
                else if (fIsAvgBrightness == FALSE	//TODO �Ƿ��б�Ҫͨ�����������WDR?
                    && m_fIsARMNight == FALSE
                    && m_nWDRLevel < 4
                    && m_nEnvAvgY > (m_nMaxAGCTH - 30) //�����������¿���WDR
                    && fCanUpdateWdrLevel)
                {
                    m_nWDRLevel++;
                    m_nEnvAvgY = 0;
                    s_dwTick = dwCurTick;
                }
                else
                {
                    hr = S_OK;
                }
            }
            else if (iAvgBrightness > iMaxBrightness )
            {
                // �ȼ�СWDR�ȼ��ټ�С����ȼ�
                if (m_nWDRLevel > 0)
                {
                    m_nWDRLevel--;
                    s_dwTick = dwCurTick;
                }
                else if( m_nPlateLightType > 0 )
                {
                    /*sprintf(szMsg, "<123ex>CheckLightType --m_nPlateLightType:%d. iAvgBrightness:%d. Exp:(%d~%d),Count:%d",
                    m_nPlateLightType, iAvgBrightness, iMaxBrightness, iMinBrightness, iFrameCount);
                    Venus_OutputDebug(szMsg);*/

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

            if (m_fIsARMNight)	//����ǿ����ߵȼ�
            {
                m_nPlateLightType = (LIGHT_TYPE)((int)LIGHT_TYPE_COUNT -1);
                m_nWDRLevel = 0;
            }

            s_dwLastSetParamTick = sv::utGetSystemTick();
            iFrameCount = 0;
            iSumBrightness = 0;
            s_iMinValue = 255;
            s_iMaxValue = 0;
        }
        if(hr == RS_S_OK)
        {
            s_iSucessCount++;
        }
        return hr;
    }

    HRESULT CPlateLightCtrl::CheckLight(int nEnvAvgY)
    {
        const int FIRST_CHECK_COUNT = 5;
        const int FIRST_COUNT_THRESHOLD = 3;
        const int MAX_CHECK_COUNT = 1000;
        const int COUNT_THRESHOLD = 900;

        static bool fFirstTime = true;
        static int iFrameCount = 0;
        static int m_iPositiveDayCount = 0;
        static int m_iPositiveNightCount = 0;
        static int m_iPositiveLightOff = 0;
        static int m_iPositiveLightOn = 0;
        static int iSuccessCount = 0;
        static int s_nAvgYCount = 0;
        static int s_nAvgY = 0;

        s_nAvgY += nEnvAvgY;
        if (s_nAvgYCount++ > 1000)
        {
            m_nEnvAvgY = (s_nAvgY / s_nAvgYCount);
            s_nAvgYCount = 0;
            s_nAvgY = 0;
        }

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
                    /*char szMsg[256];
                    sprintf(szMsg, "<123exdo>CheckLightType iSuccessCount:%d,fFirstTime:%d,nEnvAvgY:%d",
                    iSuccessCount, fFirstTime, nEnvAvgY);
                    Venus_OutputDebug(szMsg);*/

                    if (UpdateLightType(nEnvAvgY, true) == S_OK)
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
                    if (m_iPositiveNightCount >= FIRST_COUNT_THRESHOLD)
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
                    if (nEnvAvgY <= m_iNightThreshold)
                    {
                        m_iPositiveNightCount++;
                    }
                    else
                    {
                        m_iPositiveDayCount++;
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
                /*char szMsg[256];
                sprintf(szMsg, "<123exdo>CheckLightType tick:%d,m_dwLastCarLeftTime:%d,nEnvAvgY:%d",
                sv::utGetSystemTick(), m_dwLastCarLeftTime,nEnvAvgY);
                Venus_OutputDebug(szMsg);*/
                UpdateLightType(nEnvAvgY, true);
            }

            if ( nEnvAvgY > m_iNightThreshold )
            {
                m_iPositiveDayCount++;
            }
            else
            {
                m_iPositiveNightCount++;
            }

            if ( iFrameCount >= MAX_CHECK_COUNT )
            {
                if ( m_iPositiveDayCount >= COUNT_THRESHOLD )
                {
                    m_LightType = DAY;
                }
                if ( m_iPositiveNightCount >= COUNT_THRESHOLD )
                {
                    m_LightType = NIGHT;
                }

                iFrameCount = 0;
                m_iPositiveDayCount = 0;
                m_iPositiveNightCount = 0;
                m_iPositiveLightOn = 0;
                m_iPositiveLightOff = 0;
            }
        }

        return S_OK;
    }

    HRESULT CPlateLightCtrl::UpdatePlateLight(int iY)
    {
        m_dwLastCarLeftTime = sv::utGetSystemTick();
        return UpdateLightType(iY, false);
    }

    LIGHT_TYPE CPlateLightCtrl::GetPlateLightType()
    {
        return m_nPlateLightType;
    }

    int CPlateLightCtrl::GetWDRLevel()
    {
        return m_nWDRLevel;
    }

    CAR_TYPE TrackInfoType2AppType(int nInfoType)
    {
        CAR_TYPE ttRet;
        switch(nInfoType)
        {
        case CAppTrackInfo::TT_LARGE_CAR:
            ttRet = CT_LARGE;
            break;
        case CAppTrackInfo::TT_MID_CAR:
            ttRet = CT_MID;
            break;
        case CAppTrackInfo::TT_SMALL_CAR:
            ttRet = CT_SMALL;
            break;
        case CAppTrackInfo::TT_BIKE:
            ttRet = CT_BIKE;
            break;
        case CAppTrackInfo::TT_WAKL_MAN:
            ttRet = CT_WALKMAN;
            break;
        default:
            ttRet = CT_UNKNOWN;
        }
        return ttRet;
    }

    inline void FixRect(sv::SV_RECT& rc, int imgWidth, int imgHeight)
    {
        if(rc.m_nLeft < 0)
        {
            rc.m_nLeft = 0;
        }

        if(rc.m_nRight > imgWidth - 1)
        {
            rc.m_nRight = imgWidth - 1;
        }

        if(rc.m_nTop < 0)
        {
            rc.m_nTop = 0;
        }

        if(rc.m_nBottom > imgHeight - 1)
        {
            rc.m_nBottom = imgHeight - 1;
        }

        if(rc.m_nLeft > rc.m_nRight)
        {
            rc.m_nRight = rc.m_nLeft;
        }

        if(rc.m_nTop > rc.m_nBottom)
        {
            rc.m_nBottom = rc.m_nTop;
        }
    }
    //////////////////////////////////////////////////////////////////////////
    // CAppTrackCtrl
    // 

    CAppTrackCtrl::CAppTrackCtrl(void)
        :
    m_pTgCtrl(NULL),
        m_iTrackInfoCnt(0),
        m_pCallback(NULL),
        m_pCurIRefImage(NULL)
    {

//        svTgIrApi::MOD_DET_INFO* pModel = m_rgModDetInfo;
//
//        // ����С��
//        pModel->nDetType = svTgIrApi::MOD_DET_INFO::DAY_SMALL_CAR;
//        pModel->pbData = swTgApp::g_cTgDetMode_DaySmall.pData;
//        pModel->nDataLen = swTgApp::g_cTgDetMode_DaySmall.nDataLen;
//        pModel->fltRoadRatio = 0.6f;
//        pModel->nStepDivX = 8;
//        pModel->nStepDivY = 6;
//        pModel->nScaleNum = 3;
//        pModel->nMergeNum = 2;
//        pModel->nMaxROI = 500;
//        ++pModel;
//
//        // �����
//        pModel->nDetType = svTgIrApi::MOD_DET_INFO::DAY_LARGE_CAR;
//        pModel->pbData = swTgApp::g_cTgDetMode_DayLarge.pData;
//        pModel->nDataLen = swTgApp::g_cTgDetMode_DayLarge.nDataLen;
//        pModel->fltRoadRatio = 0.9f;
//        pModel->nStepDivX = 8;
//        pModel->nStepDivY = 6;
//        pModel->nScaleNum = 3;
//        pModel->nMergeNum = 2;
//        pModel->nMaxROI = 500;
//        ++pModel;
//
//        // ����С��
//        pModel->nDetType = svTgIrApi::MOD_DET_INFO::DUSK_SMALL_CAR;
//        pModel->pbData = swTgApp::g_cTgDetMode_DaySmall.pData;         //����ʹ�ð���ģ��
//        pModel->nDataLen = swTgApp::g_cTgDetMode_DaySmall.nDataLen;
//        pModel->fltRoadRatio = 0.6f;
//        pModel->nStepDivX = 8;
//        pModel->nStepDivY = 6;
//        pModel->nScaleNum = 3;
//        pModel->nMergeNum = 2;
//        pModel->nMaxROI = 500;
//        ++pModel;
//
//        // ����С����ǿģ�ͣ�ģ�Ͳ�����Ҫ�Ͱ���С��һ��
//        *pModel = *(pModel - 1);
//        pModel->nDetType = svTgIrApi::MOD_DET_INFO::DUSK_SMALL_CAR_EX;
//        pModel->pbData = swTgApp::g_cTgDetMode_DuskSmallEx.pData;
//        pModel->nDataLen = swTgApp::g_cTgDetMode_DuskSmallEx.nDataLen;
//        ++pModel;
//
//        // �����
//        pModel->nDetType = svTgIrApi::MOD_DET_INFO::DUSK_LARGE_CAR;
//        pModel->pbData = swTgApp::g_cTgDetMode_DayLarge.pData;         //����ʹ�ð���ģ��
//        pModel->nDataLen = swTgApp::g_cTgDetMode_DayLarge.nDataLen;
//        pModel->fltRoadRatio = 0.9f;
//        pModel->nStepDivX = 8;
//        pModel->nStepDivY = 6;
//        pModel->nScaleNum = 3;
//        pModel->nMergeNum = 2;
//        pModel->nMaxROI = 500;
//        ++pModel;
//
//        // �������ǿģ�ͣ�ģ�Ͳ�����Ҫ�Ͱ���С��һ��
//        *pModel = *(pModel - 1);
//        pModel->nDetType = svTgIrApi::MOD_DET_INFO::DUSK_LARGE_CAR_EX;
//        pModel->pbData = swTgApp::g_cTgDetMode_DuskLargeEx.pData;
//        pModel->nDataLen = swTgApp::g_cTgDetMode_DuskLargeEx.nDataLen;
//        ++pModel;
//
//        // ����С��
//        pModel->nDetType = svTgIrApi::MOD_DET_INFO::NIGHT_SMALL_CAR;
//        pModel->pbData = swTgApp::g_cTgDetMode_NightSmall.pData;
//        pModel->nDataLen = swTgApp::g_cTgDetMode_NightSmall.nDataLen;
//        pModel->fltRoadRatio = 0.6f;
//        pModel->nStepDivX = 8;
//        pModel->nStepDivY = 8;
//        pModel->nScaleNum = 3;
//        pModel->nMergeNum = 2;
//        pModel->nMaxROI = 500;
//        ++pModel;
//
//        // ���ϴ�
//        pModel->nDetType = svTgIrApi::MOD_DET_INFO::NIGHT_LARGE_CAR;
//        pModel->pbData = swTgApp::g_cTgDetMode_NightLarge.pData;
//        pModel->nDataLen = swTgApp::g_cTgDetMode_NightLarge.nDataLen;
//        pModel->fltRoadRatio = 0.9f;
//        pModel->nStepDivX = 8;
//        pModel->nStepDivY = 8;
//        pModel->nScaleNum = 3;
//        pModel->nMergeNum = 2;
//        pModel->nMaxROI = 500;
//        ++pModel;
    }

    CAppTrackCtrl::~CAppTrackCtrl(void)
    {
        if (m_pTgCtrl != NULL)
        {
            svTgIrApi::FreeTgCtrl(m_pTgCtrl);
            m_pTgCtrl = NULL;
        }
    }

    HRESULT CAppTrackCtrl::Init(
        TRACKER_CFG_PARAM* pCfgParam,
        PlateRecogParam* pRecogParam,
        int iFrameWidth, 
        int iFrameHeight
        )
    {  
        if (pCfgParam == NULL || pRecogParam == NULL)
        {
            return E_INVALIDARG;
        }

        SetSVCallBack();

        sv::utTrace("=================SVEPVER:%s================\n", svTgIrApi::GetRevInfo());

        m_cRecogParam = *pRecogParam;
        m_cTrackerCfgParam = *pCfgParam;

        if (m_pCurIRefImage != NULL)
        {
            m_pCurIRefImage->Release();
            m_pCurIRefImage = NULL;
        }; 

        // init var
        m_iFrameWidth = iFrameWidth;
        m_iFrameHeight = iFrameHeight;

        for (int i=0; i<m_iTrackInfoCnt; ++i)
        {
            m_rgTrackInfo[i].Free();
        }
        m_iTrackInfoCnt = 0;
        m_nEnvLightType = svTgIrApi::CTgCtrl::LT_DAY;
        m_nEnvLight = 160;

        m_iAverageConfidenceQuan = 0;
        m_iFirstConfidenceQuan = 0;
        m_nLastResultPlateCount = 0;

        m_dwFrameDelay = 0;
        m_dwProcessStartSystemTick = 0;

        memset(m_rgLastResultPlate, 0, sizeof(m_rgLastResultPlate));

        // create ctrl
        if (m_pTgCtrl == NULL)
        {
            m_pTgCtrl = svTgIrApi::CreateTgCtrl();
        }

        if (m_pTgCtrl == NULL)
        {
            return E_OUTOFMEMORY;
        }

        // ���ñ���Ĳ���
        SetCtrlParam(pCfgParam);

        RTN_HR_IF_FAILED(SetTgApiParam(pCfgParam, pRecogParam, iFrameWidth, iFrameHeight));

        // load det model
//        for (int i=0; i<MAX_MOD_DET_INFO; ++i)
//        {
//            RTN_HR_IF_SVFAILED(m_pTgCtrl->LoadDetModel(&m_rgModDetInfo[i]));
//        }
        m_cApiParam.nDetBottomLine = 80;
        HRESULT hr = Svresult2Hresult(m_pTgCtrl->Init(&m_cApiParam));

        m_pTgCtrl->SetTriggerCallBack(swTgApp::TriggerCallBack, this);
        m_cPlateLightCtrl.Init(pCfgParam->nMaxPlateBrightness, pCfgParam->nMinPlateBrightness,
            pCfgParam->nPlateLightCheckCount, pCfgParam->nNightThreshold,
            pCfgParam->nMaxAGCTH);

        m_fEnableCarArriveTrigger = TRUE;

        m_dwTriggerCameraTimes = 0;
        m_dwLastTriggerTick = 0;

        return hr;

    }

    HRESULT CAppTrackCtrl::Uninit()
    {

        if (NULL != m_pTgCtrl)
        {
            svTgIrApi::FreeTgCtrl(m_pTgCtrl);
            m_pTgCtrl = NULL;
        }

        return S_OK;
    }

    HRESULT CAppTrackCtrl::Process(
        PROCESS_ONE_FRAME_PARAM* pProcParam,
        PROCESS_ONE_FRAME_DATA* pProcessData,
        PROCESS_ONE_FRAME_RESPOND* pProcessRespond
        )
    {
        if (m_pTgCtrl == NULL || pProcParam == NULL)  return E_FAIL;

        RTN_HR_IF_FAILED(PreProcess(pProcParam, pProcessData, pProcessRespond));

        DWORD32 dwFrameTimeMs = pProcParam->dwImageTime;

        HV_COMPONENT_IMAGE hvImgFrame = pProcessData->hvImageYuv;

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

        HRESULT hr = S_OK;
        const int MAX_RES = 30;
        svTgIrApi::ITgTrack* rgpTgRes[MAX_RES];
        int nResCnt = 0;

        // �����첽�������
        unsigned int nFrameTime;
        swTgApp::DET_ROI* pDetROI = NULL;
        int nDetROICnt = 0;
        swTgApp::SeparateResultRefPoint(pProcessData->cSyncDetData.pbData, 
            pProcessData->cSyncDetData.nLen, 
            &nFrameTime, &pDetROI, &nDetROICnt); 

        svTgIrApi::CTgCtrl::TG_CTRL_INFO exInfo;

        //sv::utTrace("# async roi:%d, datlen:%d\n", nDetROICnt, pProcessData->cSyncDetData.nLen);

		//pansx modify ����ʲô�����Ҫץ��
		m_fEnableCarArriveTrigger = TRUE;
        //m_fEnableCarArriveTrigger = (m_nEnvLightType == svTgIrApi::CTgCtrl::LT_DAY) ? FALSE : TRUE;

        // add fanghy, �����ȼ�С�� 7 ��ʱ����ʹ��ץ�ģ������˳��ĵ���ץ�Ĺ��ص����
        //if (m_fEnableCarArriveTrigger && m_cPlateLightCtrl.GetPlateLightType() < DAY_BACKLIGHT_2)
        //{
        //    m_fEnableCarArriveTrigger = FALSE;
        //}
		//pProcParam->fIsCaptureImage=FALSE;
#if 0
        // ץ��ʶ��
        pProcParam->fIsCaptureImage = TRUE;
        if (pProcParam->fIsCaptureImage)
        {
            CRect rc(0, 50, 100, 100);    // debug
#else
        // ץ��ʶ��
		if (pProcParam->fIsCaptureImage)
		{
        	// ת���ɰٷֱ�
			CRect rc(
				pProcParam->cFrameRecognizeParam.rectCaptureRegArea.left * 100 / hvImgFrame.iWidth,
				pProcParam->cFrameRecognizeParam.rectCaptureRegArea.top * 100 / hvImgFrame.iHeight,
				pProcParam->cFrameRecognizeParam.rectCaptureRegArea.right * 100 / hvImgFrame.iWidth,
				pProcParam->cFrameRecognizeParam.rectCaptureRegArea.bottom * 100 / hvImgFrame.iHeight
			);
#endif
			DWORD32 dwBeginTime = sv::utGetSystemTick();

			// ץ��ʶ��
			HRESULT rt = RecogOneFrame(pProcParam,
                pProcessData,
				pProcessRespond,
				hvImgFrame,
				rc
				);

			static DWORD32 s_dwTotalTick = 0;
			static int s_nRecCount = 0;
			dwBeginTime = sv::utGetSystemTick() - dwBeginTime;
			s_dwTotalTick += dwBeginTime;
			s_nRecCount++;
			sv::utTrace("#RecogOneFrame rc:[%d %d %d %d] t:%dms. %d\n",
				rc.left, rc.top, rc.right, rc.bottom, dwBeginTime, s_dwTotalTick / s_nRecCount);

			if (rt != S_OK)
			{
				utTrace("m_pTgCtrl RecogOneFrame %08X. imgtype %d\n", rt, hvImgFrame.nImgType);
			}
            return RS_S_OK;
        }

        // ������ʱ
        m_dwFrameDelay = dwFrameTimeMs - CAppTrackInfo::s_iCurImageTick;
        m_dwProcessStartSystemTick = sv::utGetSystemTick();

        // ���¸���ʱ��
        CAppTrackInfo::s_iCurImageTick = dwFrameTimeMs;

        if (m_cPlateLightCtrl.GetPlateLightType() < 4)
        {
            m_pTgCtrl->ForceDayEnvLightType();
        }

        // ������ʶ��
        sv::SV_RESULT svRet = m_pTgCtrl->Process(svImgFrame, 
            dwFrameTimeMs, 
            (svTgIrApi::DET_ROI*)pDetROI, nDetROICnt,
            rgpTgRes, 
            MAX_RES, 
            &nResCnt,
            &exInfo
            ); 

        if (svRet != sv::RS_S_OK)
        {
            utTrace("m_pTgCtrl Process %08X. imgtype %d\n", svRet, hvImgFrame.nImgType);
        }
        RTN_HR_IF_SVFAILED( svRet );

        // ȡ�������ȵȽ����Ϣ
        m_nEnvLightType = exInfo.nLightType;
        m_nEnvLight = exInfo.nAvgBrightness;

        if (pProcParam->iEnvStatus == 2 && m_nEnvLightType == svTgIrApi::CTgCtrl::LT_DAY)
        {
            m_nEnvLightType = svTgIrApi::CTgCtrl::LT_DUSK;
			utTrace("EnvStatus == 2, and EnvLightType turn to svTgIrApi::CTgCtrl::LT_DUSK\n");
        }

        // �������ת����CAppTrackInfo
        RTN_HR_IF_FAILED( CAppTrackInfo::UpdateAll(m_rgTrackInfo, MAX_TRACK_INFO, &m_iTrackInfoCnt, rgpTgRes, nResCnt) );

        //sv::utTrace("#APP Tracker get det %d. up %d\n", nResCnt, m_iTrackInfoCnt);

#if RUN_PLATFORM == PLATFORM_WINDOWS
        ShowImgTrack(&hvImgFrame, m_rgTrackInfo, m_iTrackInfoCnt, 1, "out");
#endif

        // zhaopy �жϳ���
        m_fIsCheckLightType = pProcParam->fIsCheckLightType;
        if(m_fIsCheckLightType)
        {
            //utTrace("<123dspex0> Check LightType:%d\n", m_cPlateLightCtrl.GetPlateLightType());
            m_cPlateLightCtrl.SetArmEnvPeriodInfo(pProcParam->fIsARMNight);
            m_cPlateLightCtrl.CheckLight(m_nEnvLight);
        }

        ProcessTrackState(&hvImgFrame, pProcessRespond);

        // ������Ҫ��һЩ�������Ĵ�����
        PostProcess();

        pProcessRespond->iFrameAvgY = m_nEnvLight;

        return hr;
    }

    HRESULT CAppTrackCtrl::RecogOneFrame(
        PROCESS_ONE_FRAME_PARAM* pProcParam,
        PROCESS_ONE_FRAME_DATA* pProcessData,
        PROCESS_ONE_FRAME_RESPOND* pProcessRespond,
        HV_COMPONENT_IMAGE hvImgFrame,
        HiVideo::CRect& rcRecogArae
        )
    {
        SV_RECT rcDet =
        {
            (int)((float)hvImgFrame.iWidth * rcRecogArae.left / 100),
            (int)((float)hvImgFrame.iHeight * rcRecogArae.top / 100),  // �Ը���λ��Ϊ��
            (int)((float)hvImgFrame.iWidth * rcRecogArae.right / 100),
            (int)((float)hvImgFrame.iHeight * rcRecogArae.bottom / 100)
        };
        sv::utTrace("Photo %d %d %d %d, w:%d h:%d\n", rcDet.m_nLeft, rcDet.m_nTop, rcDet.m_nRight, rcDet.m_nBottom, hvImgFrame.iWidth, hvImgFrame.iHeight);
        FixRect(rcDet, hvImgFrame.iWidth, hvImgFrame.iHeight);

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

        const int MAX_PLATE_INFO = 8;
        int nPlateInfoCnt = 0;
        svTgIrApi::TG_PLATE_INFO rgPlateInfo[MAX_PLATE_INFO];
        SV_RESULT rt = m_pTgCtrl->RecogOneFrame(svImgFrame,
            pProcParam->dwImageTime, rcDet, 
            rgPlateInfo, MAX_PLATE_INFO, nPlateInfoCnt, 
            m_cTrackerCfgParam.cRecogSnapArea.nDetectorMinScaleNum,
            m_cTrackerCfgParam.cRecogSnapArea.nDetectorMaxScaleNum
            );
        RTN_SVR_IF_FAILED(rt);

        int nMaxCoreInfo = sizeof(pProcessRespond->cTrigEvent.rgCarLeftCoreInfo) / sizeof(pProcessRespond->cTrigEvent.rgCarLeftCoreInfo[0]);
        
        bool fGetPlate = false;
        if (rt == RS_S_OK)
        {
            for (int i = 0; i < nPlateInfoCnt; ++ i)
            {
                if (pProcessRespond->cTrigEvent.iCarLeftCount >= nMaxCoreInfo)
                {
                    break;
                }
                svTgIrApi::TG_PLATE_INFO& plateInfo = rgPlateInfo[i];
                // ����ṹ��ֵ
                PROCESS_IMAGE_CORE_RESULT* pCurResult;  
                pCurResult = &(pProcessRespond->cTrigEvent.rgCarLeftCoreInfo[pProcessRespond->cTrigEvent.iCarLeftCount]);
                //�س���Сͼ, �㳵������
                HiVideo::CRect& rect = *(CRect*)&plateInfo.rcPos;
                HV_COMPONENT_IMAGE imgCropPlate;
                CropImage(hvImgFrame, rect, &imgCropPlate);
                CPersistentComponentImage imgYUVPlate;
                if (imgCropPlate.nImgType != HV_IMAGE_YUV_422)
                {
                    imgYUVPlate.Create(HV_IMAGE_YUV_422, imgCropPlate.iWidth, imgCropPlate.iHeight);
                    imgYUVPlate.Convert(imgCropPlate);
                    imgCropPlate = (HV_COMPONENT_IMAGE)imgYUVPlate;
                }
                for (int i = 0; i < 3; i++)
                {
                    PBYTE8 pSrc = GetHvImageData(&imgCropPlate, i);
                    PBYTE8 pDes = GetHvImageData(&pProcessData->rghvImageSmall[pProcessRespond->cTrigEvent.iCarLeftCount], i);
                    if (pSrc && pDes)
                    {
                        memcpy(pDes, pSrc, imgCropPlate.iStrideWidth[i] * imgCropPlate.iHeight);
                    }
                }
                pCurResult->nPlateWidth = imgCropPlate.iWidth;
                pCurResult->nPlateHeight = imgCropPlate.iHeight;

                memcpy(pCurResult->rgbContent, plateInfo.rgbContent, 8);
                pCurResult->nType = plateInfo.nPlateType;
                pCurResult->nColor = plateInfo.nColor;
                pCurResult->nCarColor = CC_UNKNOWN;
                pCurResult->rcBestPlatePos = rect;
                pCurResult->fltAverageConfidence = plateInfo.fltTotalConf;
                pCurResult->fltFirstAverageConfidence = plateInfo.rgfltConf[0];	

                pCurResult->iCarAvgY = plateInfo.nAvgY;
                pCurResult->iCarVariance = plateInfo.nVariance;

                pCurResult->nRoadNo = RecoverRoadNum(m_cRoadInfo.GetRoadNum(plateInfo.rcPos.CenterPoint()));

                pProcessRespond->cTrigEvent.dwEventId |= EVENT_CARLEFT;
                pProcessRespond->cTrigEvent.iCarLeftCount++;

                fGetPlate = true;
#if RUN_PLATFORM == PLATFORM_WINDOWS

                RESULT_IMAGE_STRUCT* pResultImage = &(pCurResult->cResultImg);
                pResultImage->pimgLastSnapShot = m_pCurIRefImage->GetImage() ;
                pResultImage->pimgBestSnapShot = NULL;
                pResultImage->pimgBeginCapture = NULL;
                pResultImage->pimgBestCapture = NULL;
                pResultImage->pimgLastCapture = NULL;
                pCurResult->dwLastSnapShotRefTime = 0;

                pCurResult->rcBestPlatePos = rect;
#endif
            }
        }

#if RUN_PLATFORM == PLATFORM_WINDOWS
        if (pProcessRespond->cTrigEvent.iCarLeftCount == 0)
        {
        	pProcessRespond->cTrigEvent.iCarLeftCount = 1;
        	PROCESS_IMAGE_CORE_RESULT* pCurResult;
        	pCurResult = &(pProcessRespond->cTrigEvent.rgCarLeftCoreInfo[0]);
        	memset(pCurResult->rgbContent, 0, 8);
        	RESULT_IMAGE_STRUCT* pResultImage = &(pCurResult->cResultImg);
			pResultImage->pimgLastSnapShot = m_pCurIRefImage->GetImage() ;
			pResultImage->pimgBestSnapShot = NULL;
			pResultImage->pimgBeginCapture = NULL;
			pResultImage->pimgBestCapture = NULL;
			pResultImage->pimgLastCapture = NULL;
			pCurResult->dwLastSnapShotRefTime = 0;
			pProcessRespond->cTrigEvent.dwEventId |= EVENT_CARLEFT;
        }
#endif

        pProcessRespond->cTrigEvent.dwEventId |= EVENT_FRAME_RECOED;

        return fGetPlate ? S_OK : S_FALSE;
    }

    void CAppTrackCtrl::SetFirstLightType(LIGHT_TYPE nLightType)
    {
        m_cPlateLightCtrl.SetFirstLightType(nLightType);
    }

    HRESULT CAppTrackCtrl::PreProcess(
        PROCESS_ONE_FRAME_PARAM* pProcParam,
        PROCESS_ONE_FRAME_DATA* pProcessData,
        PROCESS_ONE_FRAME_RESPOND* pProcessRespond
        )
    {
        m_pProcessData = pProcessData;
        m_pProcessRespond = pProcessRespond;

        // ��ʼ�������ڴ�����ָ��
        IVirtualRefImage::SetImgMemOperLog(&pProcessRespond->cImgMemOperLog);

        if (!pProcParam->fIsCaptureImage)
        {
            if (m_pCurIRefImage != NULL)
            {
                m_pCurIRefImage->Release();
                m_pCurIRefImage = NULL;
            }
            RTN_HR_IF_FAILED(CeaeteIVirtualRefImage(&m_pCurIRefImage, 
                pProcParam->pCurFrame, pProcParam->dwFrameNo, pProcParam->dwImageTime));
        }

        return S_OK;
    }

    HRESULT CAppTrackCtrl::PostProcess()
    {

        m_pProcessRespond->cLightType = m_cPlateLightCtrl.GetPlateLightType();
        m_pProcessRespond->nEnvLightType = m_nEnvLightType;
        m_pProcessRespond->nWDRLevel = m_cPlateLightCtrl.GetWDRLevel();

        // ���ٿ���Ϣ
        int iMaxRect = sizeof(m_pProcessRespond->cTrackRectInfo.rgTrackRect) / sizeof(m_pProcessRespond->cTrackRectInfo.rgTrackRect[0]);
        m_pProcessRespond->cTrackRectInfo.dwTrackCount = MIN(iMaxRect, m_iTrackInfoCnt);

        // һ������ֻ���һ����
        for (int i=0; i<(int)m_pProcessRespond->cTrackRectInfo.dwTrackCount; ++i)
        {
            SV_RECT rcLastPos = m_rgTrackInfo[i].GetLastPos();
            m_pProcessRespond->cTrackRectInfo.rgTrackRect[i] = RECT_SV2HV(rcLastPos);
        }

        // ת��Ϊ��������ͼ��
        if (m_pCurIRefImage != NULL)
        {
            m_pCurIRefImage->Release(); 
            m_pCurIRefImage = NULL;
        }

        return S_OK;
    }

    HRESULT CAppTrackCtrl::ProcessTrackState(
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
            if (m_rgTrackInfo[i].GetState() == CAppTrackInfo::TS_END)
            {
                OnTrackerEnd(&m_rgTrackInfo[i]);
            }
        }

        return S_OK;
    }


    HRESULT CAppTrackCtrl::OnTrackerFirstDet(
        CAppTrackInfo* pTrackInfo
        )
    {
        CAppTrackInfo& TrackInfo = *pTrackInfo; 
        TrackInfo.m_nStartFrameNo = m_pCurIRefImage->GetFrameNo();
        TrackInfo.m_dwFirstFrameTime = m_pCurIRefImage->GetRefTime();

        // ������������ץһ��ͼ��˵
        CSvRect rcPos = TrackInfo.GetLastPos(); 


        //     if (TrackInfo.m_pimgLastSnapShot != NULL) 
        //     {
        //         if (pTrackInfo->GetState() == CAppTrackInfo::TS_END)
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
        TrackInfo.m_rcBestPos = rcPos;

        return S_OK;
    }

    HRESULT CAppTrackCtrl::OnTrackerEnd(
        CAppTrackInfo* pTrackInfo
        )
    {
        if (IsTrackerCanOutput(pTrackInfo))
        {        
            if (!pTrackInfo->m_fCarArrived)
            {
                utTrace("[%d] CarArrive--OnTrackerEnd---\n", pTrackInfo->GetID());
                ProcessCarArrive(pTrackInfo, pTrackInfo->s_iCurImageTick, m_pProcessRespond);
            }
            // �������ȵȼ�
            if(m_fIsCheckLightType)
            {
                svTgIrApi::ITgTrack::TG_RESULT_INFO tkRes;
                pTrackInfo->GetResult(&tkRes);
                // �������Ƴ����³�������
                if( tkRes.szPlate[0] != 0 && tkRes.nPlateAvgLight > 0)
                {
                    /*char szMsg[256];
                    sprintf(szMsg, "<123exdo>CheckLightType plate.tkRes.nPlateAvgLight:%d",tkRes.nPlateAvgLight);
                    Venus_OutputDebug(szMsg);*/
                    m_cPlateLightCtrl.UpdatePlateLight((int)tkRes.nPlateAvgLight);
                }
            }

            // �����
            FireCarLeftEvent(pTrackInfo);
        }

        return S_OK;
    }

    bool CAppTrackCtrl::IsTrackerCanOutput(CAppTrackInfo* pTrackInfo)
    {
        // ִ�е����˵�����Ѿ�����

        CAppTrackInfo& TrackInfo = *pTrackInfo;

        bool fCanOutput = false;

        svTgIrApi::ITgTrack::TG_RESULT_INFO tkRes;
        TrackInfo.GetResult(&tkRes);

        if (m_nEnvLightType != svTgIrApi::CTgCtrl::LT_DUSK)
        {
            // ���������ƴ���
            if (tkRes.szPlate[0] != 0)   // �������
            {
                // ��Ч�����
                int nPlateRecogCount = TrackInfo.GetPlateCount(); 

                sv::utTrace("===[%d] ac:%.2f>%d fc:%.2f>50 ||sm:%d>1 || vd:%d>3 || inv%d\n",
                    TrackInfo.GetID(),
                    tkRes.fltPlateTotalConf * 100, m_iAverageConfidenceQuan,
                    tkRes.fltPlateFirstConf * 100, 
                    tkRes.nPlateResSimilaryCount,
                    tkRes.nValidDetCount,
                    tkRes.nPlateRecogInvalidCount
                    );

                // ȥ���
                if(tkRes.fltPlateTotalConf < 0.05f && tkRes.nValidDetCount < 8)
                {
                    sv::utTrace("DROP[%d] Low Total Conf ...\n", TrackInfo.GetID());
                    return false;	
                }

                // ʶ��ȫ��Ч��ȥ��
                if ( 
                    (nPlateRecogCount <= 3 
                    && tkRes.nValidDetCount < 8
                    &&(nPlateRecogCount == tkRes.nPlateRecogInvalidCount
                    || tkRes.nPlateResSimilaryCount < 1)
                    )
                    || (nPlateRecogCount * 30 > tkRes.nPlateResSimilaryCount * 100
                    && nPlateRecogCount * 50 < tkRes.nPlateRecogInvalidCount * 100))
                {
                    sv::utTrace("DROP[%d] PlateRecogInvalid ...[%d>%d  %d<%d]\n", TrackInfo.GetID(),
                        nPlateRecogCount * 30, tkRes.nPlateResSimilaryCount * 100,
                        nPlateRecogCount * 50, tkRes.nPlateRecogInvalidCount * 100);
                    return false;
                }

                // �Ƿ�������
                bool fSimilary = 
                    (tkRes.nPlateResSimilaryCount > 1 
                    && tkRes.nPlateResSimilaryCount < 4 
                    && tkRes.nPlateResSimilaryCount * 2 > nPlateRecogCount
                    ) // �������ٵ�ʱ��������Ʊ��ʸߣ�����Ϊ������
                    || (tkRes.nPlateResSimilaryCount >= 4 
                    && tkRes.nPlateResSimilaryCount < 10 
                    && tkRes.nPlateResSimilaryCount * 3 > nPlateRecogCount
                    )
                    || (tkRes.nPlateResSimilaryCount >= 10 
                    && tkRes.nPlateResSimilaryCount * 4 > nPlateRecogCount
                    );  

                // �Ƿ���Ŷȸ�
                bool fConfHigh = tkRes.fltPlateFirstConf * 100 > 50 
                    || (tkRes.fltPlateTotalConf > 0.3f && tkRes.fltPlateFirstConf > 0.05f);

                // �Ƿ����ȶ�
                bool fDetGood = tkRes.nValidDetCount > 3;

                // ���ƴ���
                if (tkRes.fltPlateTotalConf * 100 > m_iAverageConfidenceQuan 
                    && (fSimilary
                    || fConfHigh
                    || fDetGood)
                    )
                {
                    fCanOutput = true;
                }
                else
                {
                    sv::utTrace("DROP[%d]...[%d_%d_%d_%d]\n",
                    		TrackInfo.GetID(),
                    		m_iAverageConfidenceQuan,
                    		fSimilary, fConfHigh, fDetGood);
                }
                //         if (fCanOutput)
                //         {       
                //         	CSvRect rcLast = TrackInfo.GetLastPos();
                //         	// ���²�
                //             SV_POINT ptBCLast = {(rcLast.m_nLeft + rcLast.m_nRight)>>1,
                //                 rcLast.m_nBottom - ((rcLast.m_nBottom - rcLast.m_nTop)>>1)};
                //             int nRoadNum = m_cRoadInfo.GetRoadNum(ptBCLast);
                //         }
            }
            else
            {
                // ���ƴ���

                ///zhugl
                int nPosY = TrackInfo.GetLastPos().CenterPoint().m_nY;
                int nPosFirstY = TrackInfo.GetPos(0).CenterPoint().m_nY;
                bool fReversRun = (nPosY < nPosFirstY);

                int nLinePointY0 = m_cRoadInfo.GetRoadLinePointY0();
                int nPosDiff = 15;
                int nValidDetCount = 3;
                int nTopYLimit = 38;

                // ������߳��������Ƴ����ɹ�������
                if (TrackInfo.GetRoadNum() == 0)
                {
                    if (nLinePointY0 < 50)
                    {
                        nPosDiff = 10;
                        nValidDetCount = 2;
                        nTopYLimit = 34;
                    }
                    else if (nLinePointY0 < 55)
                    {
                        nPosDiff = 11;
                        nTopYLimit = 36;
                    }
                    else if (nLinePointY0 < 60)
                    {
                        nPosDiff = 12;
                    }
                    else if (nLinePointY0 < 65)
                    {
                        nPosDiff = 14;
                    }
                }

                if (tkRes.nValidDetCount >= nValidDetCount 
                    && (fReversRun || nPosY > m_iFrameHeight * nTopYLimit / 100)      //���Ƽ������Ϸ������
                    && abs(nPosY - nPosFirstY) > m_iFrameHeight * nPosDiff / 100)   //һ��Ҫ�ƶ�һ������
                {
                    fCanOutput = true;
                }
                else
                {
                    sv::utTrace("[%d] Drop Move Y:%d %d>%d %d>%d %d\n",
                        TrackInfo.GetID(), nPosY, 
                        abs(nPosY - nPosFirstY), m_iFrameHeight * nPosDiff / 100, 
                        tkRes.nValidDetCount, nValidDetCount,
                        nTopYLimit
                        );
                }
            }

            // ��ץ��ʶ����ʱ��Ӧ�����������ϣ�
            if (TrackInfo.m_fCaptureFrameHavePlate)
            {
                fCanOutput = true;
            }
        }
        else
        {
            // ����ģʽ
            int nPosY = TrackInfo.GetLastPos().CenterPoint().m_nY;
            int nPosFirstY = TrackInfo.GetPos(0).CenterPoint().m_nY;
            bool fReversRun = (nPosY < nPosFirstY);

            int nLinePointY0 = m_cRoadInfo.GetRoadLinePointY0();
            int nPosDiff = 15;
            int nValidDetCount = 3;
            int nTopYLimit = 38;

            // ������߳��������Ƴ����ɹ�������
            if (TrackInfo.GetPlateCount() <= 0
                && TrackInfo.GetRoadNum() == 0)
            {
                if (nLinePointY0 < 50)
                {
                    nPosDiff = 10;
                    nValidDetCount = 2;
                    nTopYLimit = 34;
                }
                else if (nLinePointY0 < 55)
                {
                    nPosDiff = 11;
                    nTopYLimit = 36;
                }
                else if (nLinePointY0 < 60)
                {
                    nPosDiff = 12;
                }
                else if (nLinePointY0 < 65)
                {
                    nPosDiff = 14;
                }
            }

            if ((tkRes.nValidDetCount >= nValidDetCount && abs(nPosY - nPosFirstY) > m_iFrameHeight * nPosDiff / 100
                || TrackInfo.GetPlateCount() > 1
                || TrackInfo.m_fCaptureFrameHavePlate)  
                && (fReversRun || nPosY > m_iFrameHeight * nTopYLimit / 100)      //���Ƽ������Ϸ������
               )   //һ��Ҫ�ƶ�һ������
            {
                fCanOutput = true;
            }
            else
            {
                sv::utTrace("[Dusk] [%d] Drop Move Y:%d %d>%d %d>%d %d\n",
                    TrackInfo.GetID(), nPosY, 
                    abs(nPosY - nPosFirstY), m_iFrameHeight * nPosDiff / 100, 
                    tkRes.nValidDetCount, nValidDetCount,
                    nTopYLimit
                    );
            }
        }

        return fCanOutput;
    }

    HRESULT CAppTrackCtrl::FireCarLeftEvent(CAppTrackInfo* pTrackInfo)
    {

        svTgIrApi::ITgTrack::TG_RESULT_INFO tkRes;
        pTrackInfo->GetResult(&tkRes);

        bool fHavePlate = (tkRes.szPlate[0] != 0);
        if (fHavePlate)
        {
            sv::utTrace("[FIRE] id:[%d], %s, r:%d, t%d\n", pTrackInfo->GetID(), tkRes.szPlate, pTrackInfo->GetRoadNum(),
                tkRes.nPlateType);
        }
        else
        {
            sv::utTrace("[FIRE] id:[%d], NULL, r:%d t%d\n", pTrackInfo->GetID(), pTrackInfo->GetRoadNum(),
                tkRes.nPlateType);
        }

        if (m_pProcessRespond == NULL || m_pProcessData == NULL)    return S_OK;

        // ���ڳ����ڵĳ����ˡ�
        int nLastPosRoadNum = pTrackInfo->GetRoadNum();		//���λ�õó��ĳ�����
        int nBestImgPosRoadNum = m_cRoadInfo.GetRoadNum(pTrackInfo->m_rcBestPos.CenterPoint());//��һ����ͼ��λ�ó�����
        if (-1 == nLastPosRoadNum
            && -1 == nBestImgPosRoadNum) 	//��һ��λ�ú����һ��λ�ö����ڻ��������ڣ������
        {
            return S_OK;
        }

        //��Ҫ�����жϳ��Ƶĸ����Ƿ�������ĸ���������ֱ�ӷ���false
        if(m_pProcessRespond->cTrigEvent.iCarLeftCount >= MAX_EVENT_COUNT)
            return S_FALSE;

        PROCESS_IMAGE_CORE_RESULT* pCurResult = &(m_pProcessRespond->cTrigEvent.rgCarLeftCoreInfo[m_pProcessRespond->cTrigEvent.iCarLeftCount]);

        CAppTrackInfo& TrackInfo = *pTrackInfo;
        pCurResult->iCapSpeed = TrackInfo.GetID();//TrackInfo.m_nID;  //TODO what's this? ID = speead?

        //  TODO ��������ʱ��

        pCurResult->dwLastSnapShotRefTime = 0;
        pCurResult->dwBestSnapShotRefTime = 0;
        pCurResult->dwBeginCaptureRefTime = 0;
        pCurResult->dwBestCaptureRefTime = 0;
        pCurResult->dwLastCaptureRefTime = 0;

        pCurResult->nFirstFrameTime = TrackInfo.m_dwFirstFrameTime;
        pCurResult->nFrameNo = m_pCurIRefImage->GetFrameNo();						// ���֡����
        pCurResult->nRefTime = m_pCurIRefImage->GetRefTime();						// ���֡ʱ��
        pCurResult->nStartFrameNo = TrackInfo.m_nStartFrameNo;					// ��ʼ֡����
        pCurResult->nEndFrameNo = m_pCurIRefImage->GetFrameNo();   // ����֡����

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

        RESULT_IMAGE_STRUCT* pResultImage = &(pCurResult->cResultImg);
        pResultImage->pimgBestSnapShot = (TrackInfo.m_pimgBestSnapShot != NULL) 
            ? TrackInfo.m_pimgBestSnapShot->GetImage() : NULL;
        pResultImage->pimgLastSnapShot = (TrackInfo.m_pimgLastSnapShot != NULL) 
            ? TrackInfo.m_pimgLastSnapShot->GetImage() : NULL;


        pResultImage->pimgBeginCapture = NULL;
        pResultImage->pimgBestCapture = NULL;
        pResultImage->pimgLastCapture = NULL;

        pCurResult->dwLastSnapShotRefTime = (TrackInfo.m_pimgLastSnapShot != NULL) 
            ? TrackInfo.m_pimgLastSnapShot->GetRefTime() : 0;
        pCurResult->dwBestSnapShotRefTime = (TrackInfo.m_pimgBestSnapShot != NULL) 
            ? TrackInfo.m_pimgBestSnapShot->GetRefTime() : 0;

        pCurResult->rcBestPlatePos = RECT_SV2HV(TrackInfo.m_rcBestPos);
        pCurResult->rcLastPlatePos = RECT_SV2HV(TrackInfo.m_rcLastPos);
        pCurResult->rcFirstPos = TrackInfo.m_rcCarArrivePos;

#if SV_RUN_PLATFORM == SV_PLATFORM_DSP
        // TODO : DSP��ʱ BestSnap��LastCaptureһ��
        //     pResultImage->pimgLastCapture = pResultImage->pimgLastSnapShot;
        //     pCurResult->rcThirdPos = pCurResult->rcBestPlatePos;
        //     pCurResult->dwLastCaptureRefTime = pCurResult->dwBestSnapShotRefTime;
#endif

        // �ж��Ƿ�Ϊ���Ƴ�������
        if (fHavePlate)
        {
            if (pTrackInfo->GetPlateCount() <= 1
                && tkRes.fltPlateFirstConf < 0.15f
                && tkRes.nPlateColor != PC_YELLOW)
            {    
                fHavePlate = FALSE;
                // �������
                memset(tkRes.rgPlateContent, 0, 8);
                tkRes.szPlate[0] = 0;
            }
        }

        if (fHavePlate)
        {
            int nVoteType = tkRes.nPlateType;	
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
                    //                 if( tkRes.nPlateType == PLATE_NORMAL)
                    //                 {
                    //                 	m_iLastPlateWidth = imgPlate.iWidth;
                    //                 }

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

                    int nVotedColor = tkRes.nPlateColor;	
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
        memcpy(pCurResult->rgbContent, tkRes.rgPlateContent, 8);
#if RUN_PLATFORM == PLATFORM_WINDOWS
        strcpy(pCurResult->szPlate, tkRes.szPlate);
#endif
        pCurResult->nType = tkRes.nPlateType;
        pCurResult->nColor = tkRes.nPlateColor;

        if( fHavePlate )
        {
            pCurResult->fltAverageConfidence = tkRes.fltPlateTotalConf;
            pCurResult->fltFirstAverageConfidence = tkRes.fltPlateFirstConf;
            pCurResult->iObservedFrames = pTrackInfo->GetPlateCount();
            pCurResult->iCarAvgY = (int)tkRes.nPlateAvgLight;
        }
        else
        {
            pCurResult->fltAverageConfidence = 0;
            pCurResult->fltFirstAverageConfidence = 0;
            pCurResult->iObservedFrames = 0;//tkRes.nValidDetCount;
            pCurResult->iCarAvgY = 0;
        }

        pCurResult->nStartFrameNo = TrackInfo.m_nStartFrameNo;
        pCurResult->nEndFrameNo = TrackInfo.m_nEndFrameNo;
        pCurResult->nFirstFrameTime = TrackInfo.m_dwFirstFrameTime;
        pCurResult->iVotedObservedFrames = tkRes.nVotePlateTypeCount;
        pCurResult->nCarColor = TrackInfo.m_nCarColor;
        pCurResult->fIsNight = (m_nEnvLightType == svTgIrApi::CTgCtrl::LT_NIGHT);
        //	pCurResult->fOutputCarColor = CTrackInfo::m_fEnableRecgCarColor;

        // ����
        if(m_iDetectReverseEnable)
        {
            pCurResult->fReverseRun = (TrackInfo.m_nReverseRunCount >= 3);
        }
        else
        {
            pCurResult->fReverseRun = false;
        }

        pCurResult->nVoteCondition = m_fEnableCarArriveTrigger;
        pCurResult->iAvgY = m_nEnvLight;

        pCurResult->iCarVariance = tkRes.nPlateVariance;  // dbg
        // Ĭ��ʹ�����λ�����ڳ����ţ�����;ʻ����������������ʹ�õ�һ��λ����������
        pCurResult->nRoadNo = (-1 == nLastPosRoadNum)?nBestImgPosRoadNum:nLastPosRoadNum;			

        //�¼����
        pCurResult->coltIsOverYellowLine	= COLT_NO;
        pCurResult->coltIsCrossLine		= COLT_NO;

        //pCurResult->iRoadNumberBegin =  CTrackInfo::m_iRoadNumberBegin;
        //pCurResult->iStartRoadNum = CTrackInfo::m_iStartRoadNum;

        pCurResult->nCarLeftCount = 0;

        //TODO ���㳵��
        // 	float fltCarSpeed(0.0f);
        // 	float fltScaleOfDistance(1.0f);
        // 	CalcCarSpeed(fltCarSpeed, fltScaleOfDistance, TrackInfo);
		float fltCarSpeed(0.0f);
        fltCarSpeed = tkRes.fltSpeed * m_cTrackerCfgParam.cScaleSpeed.fltAdjustCoef;	// �ٶȳ�������ϵ��
        pCurResult->fltCarspeed = (m_cApiParam.fEnableCalcSpeed && (fltCarSpeed <= 1.f || fltCarSpeed > 300.0f)) ? (rand() % 40) + 20 : fltCarSpeed;
        if (pCurResult->fltCarspeed != fltCarSpeed)
			utTrace("InvalidSpeed rand %0.2f\n", pCurResult->fltCarspeed);
		
		pCurResult->fltScaleOfDistance = 0.f;//fltScaleOfDistance;   // TODO ����ɶ��

        pCurResult->nPlateLightType = m_cPlateLightCtrl.GetPlateLightType();

        // TODO m_iCplStatus��m_iPulseLevel���ü����
        // 	pCurResult->iCplStatus = m_iCplStatus;
        // 	pCurResult->iPulseLevel = m_iPulseLevel;
        // 	if(pCurResult->fIsNight && pCurResult->iPulseLevel < 1)
        // 	{
        // 		pCurResult->iPulseLevel = 1;
        // 	}
        // 	
        // 	if(m_iForceLightOffAtDay)
        // 	{
        // 		if(m_LightType == DAY && m_iPulseLevel > 0 
        // 			&& m_iForceLightOffFlag == 1)
        // 		{
        // 			m_iPulseLevel = 0;
        // 		}
        // 	}

        //if( TrackInfo.m_nCarArriveTime > 0 )
        //{
        //	pCurResult->nCarArriveTime = TrackInfo.m_nCarArriveTime;   // TODO �����Է�
        //}
        //else
        // cararriveʱ��ʹ�õڶ�ץ��λ�õ�ʱ�䡣
        {
            pCurResult->nCarArriveTime = pTrackInfo->m_nCarArriveTime;//pCurResult->dwBestSnapShotRefTime;
        }

#if RUN_PLATFORM == PLATFORM_WINDOWS
        // check vaild
        pTrackInfo->m_pimgBestSnapShot->GetImage()->GetFrameName();
        //pTrackInfo->m_pimgLastSnapShot->GetImage()->GetFrameName();
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

        // TODO
        pCurResult->ptType = PT_NORMAL;

//         // У�����ֻ꣬�����Ƴ���ת�ɰٷֱȣ����Ƴ���ץ�����ܿ���Ҫ��ʵ����
//         if (fHavePlate)
//         {
//             pCurResult->rcBestPlatePos.left   = pCurResult->rcBestPlatePos.left  * 100 / m_iFrameWidth;
//             pCurResult->rcBestPlatePos.top    = pCurResult->rcBestPlatePos.top   * 100 / m_iFrameHeight;
//             pCurResult->rcBestPlatePos.right  = pCurResult->rcBestPlatePos.right  * 100 / m_iFrameWidth;
//             pCurResult->rcBestPlatePos.bottom = pCurResult->rcBestPlatePos.bottom * 100 / m_iFrameHeight;
// 
//             pCurResult->rcLastPlatePos.left   = pCurResult->rcLastPlatePos.left  * 100 / m_iFrameWidth;
//             pCurResult->rcLastPlatePos.top    = pCurResult->rcLastPlatePos.top   * 100 / m_iFrameHeight;
//             pCurResult->rcLastPlatePos.right  = pCurResult->rcLastPlatePos.right  * 100 / m_iFrameWidth;
//             pCurResult->rcLastPlatePos.bottom = pCurResult->rcLastPlatePos.bottom * 100 / m_iFrameHeight;
// 
//             pCurResult->rcFirstPos.left   = pCurResult->rcFirstPos.left  * 100 / m_iFrameWidth;
//             pCurResult->rcFirstPos.top    = pCurResult->rcFirstPos.top   * 100 / m_iFrameHeight;
//             pCurResult->rcFirstPos.right  = pCurResult->rcFirstPos.right  * 100 / m_iFrameWidth;
//             pCurResult->rcFirstPos.bottom = pCurResult->rcFirstPos.bottom * 100 / m_iFrameHeight;
// 
//             pCurResult->rcSecondPos.left   = pCurResult->rcSecondPos.left  * 100 / m_iFrameWidth;
//             pCurResult->rcSecondPos.top    = pCurResult->rcSecondPos.top   * 100 / m_iFrameHeight;
//             pCurResult->rcSecondPos.right  = pCurResult->rcSecondPos.right  * 100 / m_iFrameWidth;
//             pCurResult->rcSecondPos.bottom = pCurResult->rcSecondPos.bottom * 100 / m_iFrameHeight;
// 
//             pCurResult->rcThirdPos.left   = pCurResult->rcThirdPos.left  * 100 / m_iFrameWidth;
//             pCurResult->rcThirdPos.top    = pCurResult->rcThirdPos.top   * 100 / m_iFrameHeight;
//             pCurResult->rcThirdPos.right  = pCurResult->rcThirdPos.right  * 100 / m_iFrameWidth;
//             pCurResult->rcThirdPos.bottom = pCurResult->rcThirdPos.bottom * 100 / m_iFrameHeight;
//         }

        pCurResult->nWdrLevel = m_cPlateLightCtrl.GetWDRLevel();

        // ץ�ı�־����
        pCurResult->dwTriggerIndex = pTrackInfo->m_dwTriggerCameraTimes & 0x00FFFFFF;
        if (fHavePlate)
        {
            //  ������ͬ����
            for(int i = 0; i < MAX_LAST_RES_PLATE; ++i)
            {
                if (strcmp(tkRes.szPlate, m_rgLastResultPlate[i].szPlate) == 0)
                {
                    sv::utTrace("Plate Same [%d] %s,%d < %d\n", pTrackInfo->GetID(), tkRes.szPlate,
                        (int)((long long)pTrackInfo->m_nCarArriveTime - (long long)m_rgLastResultPlate[i].nArriveTime),
                        m_iBlockTwinsTimeout * 1000
                        );
                }

                if( m_rgLastResultPlate[i].nArriveTime > 0
                    && m_rgLastResultPlate[i].nArriveTime < pTrackInfo->m_nCarArriveTime
                    && abs((long long)pTrackInfo->m_nCarArriveTime - (long long)m_rgLastResultPlate[i].nArriveTime) < (m_iBlockTwinsTimeout * 1000)
                    && strcmp(tkRes.szPlate, m_rgLastResultPlate[i].szPlate) == 0
                    && pCurResult->ptType == PT_NORMAL )
                {
                    memset(pCurResult, 0, sizeof(pCurResult));
                    sv::utTrace("Plate Same [%d]\n", pTrackInfo->GetID());
                    return S_FALSE;
                }
            }
            // ���µ����ƶ��С�
            int index = m_nLastResultPlateCount;
            strcpy( m_rgLastResultPlate[index].szPlate, tkRes.szPlate);
            m_rgLastResultPlate[index].nArriveTime = pTrackInfo->m_nCarArriveTime;
            m_nLastResultPlateCount++;
            m_nLastResultPlateCount = (m_nLastResultPlateCount % MAX_LAST_RES_PLATE);
        }

        m_pProcessRespond->cTrigEvent.dwEventId |= EVENT_CARLEFT;
        m_pProcessRespond->cTrigEvent.iCarLeftCount++;

        // ���ƣ�����ƣ����˶����Ʋ�ʹ��m_leftCar�����ﲻ��
        // 	m_leftCar[pCurResult->nRoadNo].car_plate = (PLATE_COLOR)(pCurResult->nColor);
        // 	m_leftCar[pCurResult->nRoadNo].frame_no = pParam->dwFrameNo;

        return S_OK;
    }

    HRESULT CAppTrackCtrl::CheckCapture(
        HV_COMPONENT_IMAGE* pImage
        )
    {

        HV_COMPONENT_IMAGE& imgFrame = *pImage;
        int iFrameHeight = imgFrame.iHeight;
        int iArriveOnePos = (m_iArriveOnePos * iFrameHeight / 100);
        int iArriveTwoPos = (m_iArriveTwoPos * iFrameHeight / 100);

        // ��һץ��ͼ���������ƿɸ��µ�λ��
        int iArriveOnePos_PlateMinTH  = (iArriveOnePos - 10 * iFrameHeight / 100);
        int iArriveOnePos_PlateMaxTH  = (iArriveOnePos + 10 * iFrameHeight / 100);

        // �ڶ�ץ��ͼλ�ã������������ֵ������������
        int iArriveTwoPos_PlateMinTH  = (iArriveTwoPos - 10 * iFrameHeight / 100);
        int iArriveTwoPos_PlateMaxTH  = (iArriveTwoPos + 5 * iFrameHeight / 100);

        // ͼ��߽���ֵ
        int nImageBoundTH = iFrameHeight * 3 / 100;

        for (int index=0; index<m_iTrackInfoCnt; ++index)
        {      
            CAppTrackInfo& cTrackInfo = m_rgTrackInfo[index];
            CSvRect rcPos = cTrackInfo.GetLastPos();
            svTgIrApi::TG_TRACK_EX_INFO& cExInfo = cTrackInfo.m_cExInfo;

            // ���ƣ����ж��Ƿ���ץ��ͼ����
            bool fGetPlateInOnePos = false;
            bool fGetPlateInTwoPos = false;
            const svTgIrApi::TG_TRACK_LOCUS& cLastLocus = cTrackInfo.GetLastLocus();  // ����Ĺ켣
            bool fGetPlate = cLastLocus.m_pPlateInfo != NULL;
            SV_POINT ptPlate = {0, 0};
			int nPlateSimilar = cLastLocus.m_nPlateSimilarCnt;	//��ǰ������ͶƱ�������ƶ�
            if (fGetPlate)
            {
                ptPlate = cLastLocus.m_pPlateInfo->rcPos.CenterPoint();
                // �жϳ����Ƿ��ڵ�һץ��ͼ����
                fGetPlateInOnePos = (ptPlate.m_nY >= iArriveOnePos_PlateMinTH 
                    && ptPlate.m_nY <= iArriveOnePos_PlateMaxTH);
                fGetPlateInTwoPos = (ptPlate.m_nY >= iArriveTwoPos_PlateMinTH 
                    && ptPlate.m_nY <= iArriveTwoPos_PlateMaxTH);
            }

//             // ������ץ�ģ�û��������ǰһֱ����
             bool fUpdateArrive = false;   // �����˴���ͼ
//             if (cTrackInfo.m_pimgBestSnapShot == NULL   // û�й�ͼ
//                 || (!cExInfo.fIsTrackingMiss            // û���Ƶģ�û�ж�ʧ�����ĵ�û�й��ߣ������
//                 && rcPos.CenterPoint().m_nY <= iArriveOnePos
//                 && cTrackInfo.m_rcBestPlatePos.m_nLeft == -1)
//                 || (fGetPlate && ptPlate.m_nY <= iArriveOnePos)
//                 || (fGetPlate && cTrackInfo.m_rcBestPlatePos.m_nLeft == -1) // ��һ���Ƹ���
//                 )
//             {
//                 ReleaseIReferenceComponentImage(cTrackInfo.m_pimgBestSnapShot);
//                 cTrackInfo.m_pimgBestSnapShot = m_pCurIRefImage;
//                 cTrackInfo.m_pimgBestSnapShot->AddRef();
//
//                 cTrackInfo.m_rcBestPos = rcPos;
//                 if (fGetPlate)   // �г������¼����λ��
//                 {
//                     cTrackInfo.m_rcBestPlatePos = rcPos;
//                 }
//
//                 sv::utTrace("==CAP-best [%d] CurPos:%d,ArrivePos:%d\n",
//                     cTrackInfo.GetID(), rcPos.CenterPoint().m_nY, iArriveOnePos);
//
//                 fUpdateArrive = true;
//
//                 cTrackInfo.m_nCarArriveTime = m_pCurIRefImage->GetRefTime();
//             }

            // �жϳ����Ƿ񿿽�����
            bool fCloseBound = (rcPos.m_nLeft < nImageBoundTH) || (rcPos.m_nRight > imgFrame.iWidth - nImageBoundTH);
            // ���Ƴ�Ԥ���ѵ���ץ����
            bool fNoPlatePredictArrive =     
                (cTrackInfo.GetPlateCount() == 0
                && cExInfo.fIsTrackingMiss           // ��ʧ��֮ǰץ��ͼ��Զ����Ԥ�����ĵ�û�й��ߣ������
                && cTrackInfo.m_rcLastPos.CenterPoint().m_nY < iArriveTwoPos_PlateMinTH
                && cExInfo.rcPredictCur.CenterPoint().m_nY > cTrackInfo.m_rcLastPos.CenterPoint().m_nY
                && cExInfo.rcPredictCur.CenterPoint().m_nY < iArriveTwoPos_PlateMinTH  // Ԥ����ܻ���һ�ģ���˽���λ�òŸ�����
                && cTrackInfo.m_rcLastPlatePos.m_nLeft == -1
                && !fCloseBound);

            // ����ץ��ͼ
            if (cTrackInfo.m_pimgLastSnapShot == NULL 
                || fNoPlatePredictArrive
                ||  (!cExInfo.fIsTrackingMiss          // û�ж�ʧ�����ĵ�û�й��ߣ������
                && rcPos.CenterPoint().m_nY <= iArriveTwoPos
                && rcPos.m_nBottom < (iFrameHeight - 80))
                //&& cTrackInfo.m_rcLastPlatePos.m_nLeft == -1)
                //|| (fGetPlate && ptPlate.m_nY <= iArriveTwoPos)
                //|| (fGetPlate && cTrackInfo.m_rcLastPlatePos.m_nLeft == -1)               // �ڵڶ��߸����쵽��
                )
            {    
                ReleaseIReferenceComponentImage(cTrackInfo.m_pimgLastSnapShot);
                cTrackInfo.m_pimgLastSnapShot = m_pCurIRefImage;
                cTrackInfo.m_pimgLastSnapShot->AddRef();

                cTrackInfo.m_rcLastPos = rcPos;

                if (fGetPlate)   // �г������¼����λ��
                {
                    cTrackInfo.m_rcLastPlatePos = rcPos;
                }
				fUpdateArrive = true;
                sv::utTrace("==CAP-last [%d] CurPos:%d,ArriveTwoPos:%d,fGetPlate:%d,Similar:%d\n", 
                    cTrackInfo.GetID(),rcPos.CenterPoint().m_nY, iArriveTwoPos, fGetPlate,cLastLocus.m_nPlateSimilarCnt);
            }

            //< Сͼ������ͼ�����洦��
            if (cTrackInfo.GetLastLocus().m_pPlateInfo != NULL)
            {
                svTgIrApi::TG_PLATE_INFO& cPlateInfo = *cTrackInfo.GetLastLocus().m_pPlateInfo;
                sv::SV_RECT rcPlatePos = cPlateInfo.rcPos;

                // 4����������
                rcPlatePos.m_nLeft &= ~1;
                int iWidthPlate = rcPlatePos.m_nRight - rcPlatePos.m_nLeft;
                int iOffset = iWidthPlate % 4;
                if( iOffset != 0 )
                {
                    rcPlatePos.m_nRight -= iOffset;
                }

                if (1)
                {
                    BestPlateInfo& cBestPlateInfo = cTrackInfo.m_rgBestPlateInfo[cPlateInfo.nPlateType]; 

                    // ���Ƶ���߿��Ŷ�
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
                        //&& cPlateInfo.fltTotalConf > cBestPlateInfo.fltConfidence
                        //|| fUpdateArrive	//�������ͼ����
                        || nPlateSimilar > cBestPlateInfo.nPlateSimilar			//���ƶȴ��������ֱ�Ӹ���
                        || (7 == nPlateSimilar 
                        && (cPlateInfo.fltTotalConf > 0.3 
                        || cPlateInfo.fltTotalConf > cBestPlateInfo.fltConfidence)))	//���ƶ�����˵Ļ�����Ҫ���Ŷȴ��ڲſɸ���
                    {
                        // ץ����������ͼ
                        sv::utTrace("SmallPlate [%d] PCount:%d(PosCount%d) Conf:%.2f(%.2f) PType:%d,Sim:%d,GetPlate:%d,Pos<%d>\n",
                            cTrackInfo.GetID() ,cTrackInfo.GetPlateCount(), cTrackInfo.GetPosCount(),
                            cPlateInfo.fltTotalConf, cBestPlateInfo.fltConfidence, cPlateInfo.nPlateType, 
                            nPlateSimilar,fGetPlate,cPlateInfo.rcPos.CenterPoint().m_nY);
                        //
                        float fltMaxConfidence = SV_MAX(cBestPlateInfo.fltConfidence, cPlateInfo.fltTotalConf);

                        cBestPlateInfo.Clear();

                        cBestPlateInfo.nPlateSimilar = nPlateSimilar;
                        cBestPlateInfo.fltConfidence = fltMaxConfidence;
                        cBestPlateInfo.rcPlatePos = RECT_SV2HV(cPlateInfo.rcPlatePos);
                        cBestPlateInfo.rcPos = RECT_SV2HV(cPlateInfo.rcPos);
                        memcpy(cBestPlateInfo.rgbContent, cPlateInfo.rgbContent, sizeof(cBestPlateInfo.rgbContent)); 

                        cBestPlateInfo.dwFrameTime = m_pCurIRefImage->GetRefTime();

                        // �������ö���
                        // ��ԭʼ��Сͼ�����ô�����ġ�
                        HV_COMPONENT_IMAGE imgCropped;
                        imgCropped.nImgType = HV_IMAGE_YUV_422;

                        HV_COMPONENT_IMAGE imgPlateCrop;
                        HV_RECT hvrc = RECT_SV2HV(rcPlatePos);
                        CropImage(imgFrame, hvrc, &imgPlateCrop);

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
            }
            //> Сͼ
        }
        return S_OK;
    }

    HRESULT CAppTrackCtrl::CheckPeccancy(
        HV_COMPONENT_IMAGE* pImage
        )
    {
        //     for (int i=0; i<m_iTrackInfoCnt; ++i)
        //     {      
        //         CAppTrackInfo& cTrackInfo = m_rgTrackInfo[i];
        // 
        // 
        //     }
        return S_OK;
    }

    HRESULT CAppTrackCtrl::CheckOneState(CAppTrackInfo* pTrackInfo, HV_COMPONENT_IMAGE* pImage)
    {
        // �״μ쵽����
        if (pTrackInfo->GetPosCount() == 1)
        {
            OnTrackerFirstDet(pTrackInfo);
        }

        //     // ȥ����������ֹͣ�ߵĳ�
        //     if (TrackInfo.m_pimgLastCapture != NULL) 
        //     {
        //         int nCapPosLast = m_cApiParam.rgiCapturePos[2] * pImage->iHeight / 100; 
        //         CRect rcLast = TrackInfo.GetLastPos();
        //         int nCheckOutPos = rcLast.bottom - (rcLast.Height() >> 2);
        // 
        //         if (nCheckOutPos < nCapPosLast)
        //         {
        //             TrackInfo.Reset();
        //             sv::utTrace("App Reset [%d]. is out.\n", TrackInfo.GetID());
        //         }

        if (!pTrackInfo->m_fCarArrived)
        {
            // ������ʱ����������ץ��λ��,������ǰ������
            // ע��ֻ�и��٣�һֱ�ƶ��������ô��߼�
            int nTriggerPosOffSet = 0;
            if (m_dwFrameDelay != 0 && m_dwProcessStartSystemTick != 0       // �쳣��������
                && m_dwFrameDelay < 4000 && m_dwProcessStartSystemTick < 4000)
            {
                int nCurProcessUseTime = sv::utGetSystemTick() - m_dwProcessStartSystemTick;
                int nDelayMs = m_dwFrameDelay + nCurProcessUseTime;

                if (nDelayMs > 90)
                {
                    nTriggerPosOffSet += 10;
                    if (nDelayMs > 180)
                    {
                        nTriggerPosOffSet += 5;
                    }
                }
            }

            // ץ��λ��
            int nTriggerPos = (m_cTrackerCfgParam.nCarArrivedPos - nTriggerPosOffSet) * m_iFrameHeight / 100;

            if (pTrackInfo->m_cExInfo.rcPredict80ms.CenterPoint().m_nY > nTriggerPos)
            {
                bool fCanArrive = false;

                if (pTrackInfo->GetPlateCount() > 0)
                {
                    fCanArrive = true;
                }
                else if (pTrackInfo->GetPosCount() > 1)
                {
                    // �޳���
                    int nFirstPos = pTrackInfo->GetPos(0).CenterPoint().m_nY;
                    int nLastPos = pTrackInfo->GetLastPos().CenterPoint().m_nY;

                    int nTH = 7 * m_iFrameHeight / 100;

                    if (pTrackInfo->GetPosCount() > 1 && SV_ABS(nLastPos - nFirstPos) > nTH)
                    {
                        fCanArrive =  true;
                    }
                }

                if (fCanArrive)
                {
                    utTrace("[%d] CarArrive-----CheckOneState------\n", pTrackInfo->GetID());
                    ProcessCarArrive(pTrackInfo, pTrackInfo->s_iCurImageTick, m_pProcessRespond);
                }
            }

        }

        return S_OK;
    }

    HRESULT CAppTrackCtrl::SetTgApiParam(
        TRACKER_CFG_PARAM* pCfgParam,
        PlateRecogParam* pRecogParam,
        int iFrameWidth, 
        int iFrameHeight
        )
    {
        // set roadinfo
        svTgIrApi::TG_PARAM& cParam = m_cApiParam;

        cParam.nRoadLineCount = pCfgParam->nRoadLineNumber; //��������
        cParam.nRoadLineCount = SV_MIN(sizeof(cParam.rgRoadLine)/sizeof(cParam.rgRoadLine[0]), cParam.nRoadLineCount);
        for (int i=0; i<cParam.nRoadLineCount; ++i)
        {
#if SV_RUN_PLATFORM == SV_PLATFORM_WIN  
            cParam.rgRoadLine[i].ptTop.m_nX = pCfgParam->rgcRoadInfo[i].ptTop.x ; 
            cParam.rgRoadLine[i].ptTop.m_nY = pCfgParam->rgcRoadInfo[i].ptTop.y ; 
            cParam.rgRoadLine[i].ptBottom.m_nX = pCfgParam->rgcRoadInfo[i].ptBottom.x; 
            cParam.rgRoadLine[i].ptBottom.m_nY = pCfgParam->rgcRoadInfo[i].ptBottom.y;  
#else
            // �豸�ϵ����껹��֡ͼ��ʵ���꣡��Ҫ����2
            cParam.rgRoadLine[i].ptTop.m_nX = pCfgParam->rgcRoadInfo[i].ptTop.x * 100 / iFrameWidth; 
            cParam.rgRoadLine[i].ptTop.m_nY = pCfgParam->rgcRoadInfo[i].ptTop.y * 100 / iFrameHeight / 2; 
            cParam.rgRoadLine[i].ptBottom.m_nX = pCfgParam->rgcRoadInfo[i].ptBottom.x * 100 / iFrameWidth; 
            cParam.rgRoadLine[i].ptBottom.m_nY = pCfgParam->rgcRoadInfo[i].ptBottom.y * 100 / iFrameHeight / 2; 
#endif
            cParam.rgRoadLine[i].iRoadType = pCfgParam->rgcRoadInfo[i].iRoadType;
        }

        // ����Tracker�ĳ�����Ϣ
        SV_ROAD_LINE_INFO rgRoadInfo[10];
        for (int i=0; i<cParam.nRoadLineCount; ++i)
        {
            rgRoadInfo[i].ptTop.m_nX = cParam.rgRoadLine[i].ptTop.m_nX; 
            rgRoadInfo[i].ptTop.m_nY = cParam.rgRoadLine[i].ptTop.m_nY;
            rgRoadInfo[i].ptBottom.m_nX = cParam.rgRoadLine[i].ptBottom.m_nX ; 
            rgRoadInfo[i].ptBottom.m_nY = cParam.rgRoadLine[i].ptBottom.m_nY;

            rgRoadInfo[i].iRoadType = cParam.rgRoadLine[i].iRoadType;
        }
        m_cRoadInfo.Init(iFrameWidth, iFrameHeight, rgRoadInfo, cParam.nRoadLineCount);
        CAppTrackInfo::SetRoadInfo(&m_cRoadInfo);    // �������ó���

        if (iFrameWidth >= 3300)
        {
            cParam.fltXScale = 0.2f;
            cParam.fltYScale = 0.33f;
        }
        else if (iFrameWidth >= 1920)
        {
            cParam.fltXScale = 0.25f;
            cParam.fltYScale = 0.5f;
        }

        cParam.nDuskMaxLightTH = 70;
        cParam.nNightMaxLightTH = 10;
        // 	cParam.iSmallCarMinWidthRadio = 42;

        cParam.nDetMinScaleNum = pCfgParam->cDetectArea.nDetectorMinScaleNum;   // ���Ƽ������С���=56*1.1^g_nDetMinScaleNum
        cParam.nDetMaxScaleNum = pCfgParam->cDetectArea.nDetectorMaxScaleNum;  // ���Ƽ���������=56*1.1^g_nDetMaxScaleNum

		cParam.fEnableCalcSpeed = pCfgParam->cScaleSpeed.fEnable;		// ������ٿ���
		cParam.fltRoadWidth = pCfgParam->cScaleSpeed.fltRoadWidth;		// �����������(��)
		cParam.fltRoadLength = pCfgParam->cScaleSpeed.fltDistance;		// ��Ļ���ص����صľ���(��)
        sv::utTrace("=====Scale Max:%d,Min:%d=====\n",cParam.nDetMaxScaleNum,cParam.nDetMinScaleNum);

        cParam.fEnableGreenPlate = pCfgParam->nPlateDetect_Green;

        cParam.nBlackPlate_S = pCfgParam->nProcessPlate_BlackPlate_S; // ���Ƶı��Ͷ�����
        cParam.nBlackPlate_L = pCfgParam->nProcessPlate_BlackPlate_L; // ������������
        cParam.nBlackPlateThreshold_H0 = pCfgParam->nProcessPlate_BlackPlateThreshold_H0; // ����ɫ������
        cParam.nBlackPlateThreshold_H1 = pCfgParam->nProcessPlate_BlackPlateThreshold_H1; // ����ɫ������

        cParam.nTriggerLine = pCfgParam->nCarArrivedPos;

    	cParam.nProcessPlate_LightBlue = pCfgParam->nProcessPlate_LightBlue;

        //cParam.fEnableCalcSpeed = TRUE;
								   
								   
        cParam.fEnableRecogCarColor = TRUE;     //ʹ�ܳ�����ɫʶ��

        return S_OK;
    }

    HRESULT CAppTrackCtrl::SetCtrlParam(
        TRACKER_CFG_PARAM* pCfgParam
        )
    {
        m_iAverageConfidenceQuan = pCfgParam->nAverageConfidenceQuan;
        m_iFirstConfidenceQuan = pCfgParam->nFirstConfidenceQuan;

        m_iBlockTwinsTimeout = pCfgParam->nBlockTwinsTimeout;
        m_iArriveOnePos = pCfgParam->nCaptureOnePos;
        m_iArriveTwoPos = pCfgParam->nCaptureTwoPos;
        m_iDetectReverseEnable = pCfgParam->nDetReverseRunEnable;

        return S_OK;
    }

    bool CAppTrackCtrl::ProcessCarArrive(
        CAppTrackInfo* pTrack, 
        DWORD32 dwImageTime, 
        PROCESS_ONE_FRAME_RESPOND* pProcessRespond
        )
    {
        // ע����������ﲻ�ܵ���pTrack�õ�ITgTrackָ��ĺ���

        if (pTrack->m_fCarArrived)
        {
            return true;
        }

        int nRoadNum = pTrack->GetRoadNum();
        if (nRoadNum == -1)
        {
            return false;
        }
        DWORD32 dwCurTick = GetSystemTick();
        CSvRect rcPos = pTrack->GetLastPos();

        // zhaopy ����ץ��
        if(m_fEnableCarArriveTrigger && !pTrack->m_fIsTrigger )
        {	
            if ((dwCurTick - m_dwLastTriggerTick) > MIN_TRIGGER_TIME)
            {
				//�����ʱ��������ٳ���ʱARM��ȡ������Ϣ�Ĵ���������ֵͬ�����
				//const int cntIntervalTime = 5;
				//while(1)
				//{
				//	dwCurTick = GetSystemTick();
				//	if(((dwCurTick - m_dwLastTriggerTick) > cntIntervalTime) ||
				//	   (dwCurTick < m_dwLastTriggerTick) ||
				//	   (m_dwLastTriggerTick == 0))
				//	{
				//		break;
				//	}
				//	else
				//	{
				//		volatile int iDelayCount = 100000;
				//		while(iDelayCount > 0) iDelayCount--;
				//	}
				//}

                m_dwLastTriggerTick = dwCurTick;
                ++m_dwTriggerCameraTimes;
                TriggerCamera(nRoadNum);
            }

            utTrace("[%d] Trigger cnt:%d_%u-------------------\n", pTrack->GetID(), m_dwTriggerCameraTimes, m_dwLastTriggerTick);
            pTrack->m_fIsTrigger = TRUE;
        }
        // ץ�ļ���
        pTrack->m_dwTriggerCameraTimes = m_dwTriggerCameraTimes;

        pTrack->m_nCarArriveTime = dwImageTime;
        pTrack->m_nCarArriveRealTime = dwCurTick;    //?

        // zhaopyץ��ͼƬ��ʱ��.
        pTrack->m_nCarArriveRealTime = m_dwLastTriggerTick; // TODO: image time? 

        // ֪ͨ��������
        CARARRIVE_INFO_STRUCT carArriveInfo;

        //pTrack->m_nOutRoadNum = RecoverRoadNum(m_cRoadInfo.GetRoadNum(rcPos.CenterPoint()));
        pTrack->m_rcBestPos = rcPos;     // һ��Ҫ���������ܳ�ץ��ͼ

        // zhaopy
        carArriveInfo.iRoadNumber = m_cRoadInfo.GetRoadNum(rcPos.CenterPoint());//pTrack->m_nOutRoadNum;
        carArriveInfo.iPlateLightType = m_cPlateLightCtrl.GetPlateLightType();
        // zhaopy
        //carArriveInfo.dwTriggerOutDelay = m_pCurIRefImage->GetRefTime();
        carArriveInfo.dwCarArriveTime = dwImageTime;
        carArriveInfo.dwCarArriveRealTime = dwCurTick;

        carArriveInfo.dwFirstPos = rcPos.CenterPoint().m_nY * 100 / m_iFrameHeight;
        carArriveInfo.dwEndPos = carArriveInfo.dwFirstPos;

        CARARRIVE_INFO_STRUCT* pCarArriveInfo = &pProcessRespond->cTrigEvent.rgCarArriveInfo[pProcessRespond->cTrigEvent.iCarArriveCount++];
        memcpy(pCarArriveInfo, &carArriveInfo, sizeof(CARARRIVE_INFO_STRUCT));
        pProcessRespond->cTrigEvent.dwEventId |= EVENT_CARARRIVE;

        pTrack->m_fCarArrived = true;

        pTrack->m_rcCarArrivePos = *(HV_RECT*)&rcPos;

        return true;
    }

	void CAppTrackCtrl::pwm_writeinfo_in_dsp(DWORD32 temp_data)
	{
		volatile DWORD32 *temp_addr = (volatile DWORD32 *)0x0804603c;    // timer5 trcc dsp���ڴ��ַ
		*temp_addr = temp_data;
	}


	void CAppTrackCtrl::pwm_int_in_dsp(void)
	{
		volatile DWORD32 *temp_addr = NULL;
		temp_addr = (volatile DWORD32 *)0x0804402c; // timer4 int dsp���ڴ��ַ
		*temp_addr = 1; // д1��ʾ���д���
	}

    /// ��ԭ��ԭʼ�����ţ��Զ��ָ�������ʼ�Ų�Ϊ0��������������
    int CAppTrackCtrl::RecoverRoadNum(int iRoadNum)
    {
        if (m_cTrackerCfgParam.iRoadNumberBegin != 0)
        {
            iRoadNum = m_cTrackerCfgParam.nRoadLineNumber - 2 - iRoadNum;
        }

        return m_cTrackerCfgParam.iStartRoadNum + iRoadNum;
    }
	
	void CAppTrackCtrl::TriggerCamera(const int iRoadNum)
    {
		DWORD32 dwFlag = (iRoadNum << 24);
		dwFlag |= (m_dwTriggerCameraTimes & 0x00FFFFFF);
		pwm_writeinfo_in_dsp(dwFlag);
		pwm_int_in_dsp();
    }

    bool CAppTrackCtrl::TriggerCallBack(
        sv::SV_RECT rcPos,      /// �ص���λ��
        int nTrackID
        )
    {
        CAppTrackInfo* pTrackInfo = NULL;
        for (int i=0; i<m_iTrackInfoCnt; ++i)
        {
            if (m_rgTrackInfo[i].GetID() == nTrackID)
            {
                pTrackInfo = &m_rgTrackInfo[i]; 
            }
        }

        if (pTrackInfo == NULL)
        {
            return false;
        }

        if (!pTrackInfo->m_fCarArrived)
        {
            return ProcessCarArrive(pTrackInfo, CAppTrackInfo::s_iCurImageTick, m_pProcessRespond); 
        }
        else
        {
            return true;
        }
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

}
