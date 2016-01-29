// ���ļ����������WINDOWS-936��ʽ
#include "DataCtrl.h"
#include "ControlFunc.h"
#include "math.h"
#include "HvDspLinkApi.h"

using namespace HiVideo;
using namespace HvSys;
int g_iControllPannelWorkStyle = 0;
#ifdef SINGLE_BOARD_PLATFORM
extern bool g_fCopyrightValid;
#endif

extern HRESULT ConvertToJpeg(IReferenceComponentImage **ppRefImage, bool bReplace);

HRESULT ForceSend(DWORD32 dwVideoID)
{
    return g_pTollGate->ForceSend(dwVideoID);
}

HRESULT GetWorkModeInfo(SYS_INFO* pInfo)
{
    return(g_pTollGate->GetWorkModeInfo(pInfo));
}

HRESULT GetWorkModeList(SYS_INFO* pInfo)
{
    return(g_pTollGate->GetWorkModeList(pInfo));
}

HRESULT SetWorkMode(DWORD32 dwWorkMode)
{
    return(SetWorkModePart(dwWorkMode));
}

void DspSoftTriggerCallback(int unused)
{
#ifdef SINGLE_BOARD_PLATFORM
    if (g_pTollGate)
    {
        g_pTollGate->DspSoftTrigger();
    }
#endif
}

//--------------------------------------------------------

CTollGate::CTollGate()
        : m_pVideoRecoger(NULL)
        , m_pVideoGetter(NULL)
        , m_pXmlDoc(NULL)
        , m_pResultSender(NULL)
        , m_pSignalMatch(NULL)
        , m_pImgCapturer(NULL)
        , m_pCamTrigger(NULL)
        , m_nCarLeftCount(0)
        , m_nLightType(-1)
        , m_pCarLeftThread(NULL)
        , m_fltFrameRate(0.0f)
        , m_nLastLightLevel(0)
        , m_iOutPutNoPlate(0)
        , m_iPrevTime(0)
        , m_iPostTime(0)
        , m_fIsMatch(true)
        , m_pRefImage(NULL)
{
#ifdef SINGLE_BOARD_PLATFORM
    m_pVideoSender = NULL;
#endif
    m_pCarLeftThread = new CCarLeftThread(this);
    m_pCarLeftThread->Start(NULL);
    m_nFlashLampDiff = 5;
    m_iFlashStatus = -1;
}

CTollGate::~CTollGate()
{
    if (m_pSignalMatch)
    {
        delete m_pSignalMatch;
    }
    if (m_pVideoRecoger)
    {
        delete m_pVideoRecoger;
    }
    if (m_pVideoGetter)
    {
        delete m_pVideoGetter;
    }
    if (m_pCamTrigger)
    {
        delete m_pCamTrigger;
    }
    if (m_pImgCapturer)
    {
        delete m_pImgCapturer;
    }
    if (m_pXmlDoc)
    {
        delete m_pXmlDoc;
    }
    if (m_pResultSender)
    {
        delete m_pResultSender;
    }
    if (m_pCarLeftThread)
    {
        delete m_pCarLeftThread;
        m_pCarLeftThread = NULL;
    }
    if (m_pRefImage != NULL)
    {
        SAFE_RELEASE(m_pRefImage);
    }
}

HRESULT CTollGate::ForceSend(DWORD32 dwVideoID)
{
    SIGNAL_INFO cSignalInfo;
    cSignalInfo.nType = ((COuterControlImpl*)m_pSignalMatch)->GetSoftTrigSignalSource();
    cSignalInfo.dwSignalTime = GetSystemTick();
    cSignalInfo.dwInputTime = GetSystemTick();
    cSignalInfo.dwValue = 0;
    cSignalInfo.dwRoad = 0x00FF00FF;
    cSignalInfo.pImage = NULL;
    cSignalInfo.pImageLast = NULL;

    return m_pSignalMatch->AppendSignal(&cSignalInfo);
}

HRESULT CTollGate::GetWorkModeInfo(SYS_INFO* pInfo)
{
    DWORD32 dwModeIndex = 0;
    char szModeName[128] = "UNKNOWN";
    //m_cModuleParams.nWorkModeIndex������Ƿ���ǰ��ģʽ����
    //����轫������ת��Ϊ���ͺ������
    GetRecogParamNameOnIndex(m_cModuleParams.nWorkModeIndex, szModeName);
    pInfo->Info.WorkMode.dwMode = GetRecogParamIndexOnNamePart(szModeName);
    pInfo->Info.WorkMode.dwModeCount = GetRecogParamCountPart();
    return S_OK;
}

HRESULT CTollGate::GetWorkModeList(SYS_INFO* pInfo)
{
    if (pInfo->Info.WorkModeList.nMode >= GetRecogParamCountPart())
    {
        strcpy((char*)pInfo->Info.WorkModeList.pbListBuf, "UNKNOWN");
        pInfo->Info.WorkModeList.nLen = 8;
        return E_FAIL;
    }

    GetRecogParamNameOnIndexPart(
        pInfo->Info.WorkModeList.nMode,
        (char*)pInfo->Info.WorkModeList.pbListBuf
    );
    pInfo->Info.WorkModeList.nLen = strlen((char*)(pInfo->Info.WorkModeList.pbListBuf)) + 1;

    return S_OK;
}

void CTollGate::OnImage(void* pContext, IMG_FRAME imgFrame)
{
    CTollGate * pThis = (CTollGate *)pContext;

    if ( imgFrame.pRefImage != NULL )
    {
        imgFrame.pRefImage->AddRef();
    }

#ifdef SINGLE_BOARD_PLATFORM
    if (false == pThis->m_queImage.AddTail(imgFrame, 0 == pThis->m_cModuleParams.cCamCfgParam.iCamType))
#else
     // �����ץ��ͼ��һ�����Ƚ�����
    if(imgFrame.pRefImage->IsCaptureImage())
    {
        if(false == pThis->m_queImage.AddTail(imgFrame, 0 == pThis->m_cModuleParams.cCamCfgParam.iCamType))
        {
            SAFE_RELEASE(imgFrame.pRefImage);
        }
    }
    else if (false == pThis->m_queImage.AddTail(imgFrame, 0 != pThis->m_cModuleParams.cCamCfgParam.iCamType))
#endif
    {
        SAFE_RELEASE(imgFrame.pRefImage);
    }
}

HRESULT CTollGate::Run(void* pvParam)
{
    DWORD32 dwVideoFrameCount = 0;
    DWORD32 dwFrameCount = 0;
    DWORD32 dwLastFrameCount = 0;
    DWORD32 dwSecondCount = GetSystemTick() / 1000;
    DWORD32 dwCurSecCount = 0;
    m_dwLastSendTime = GetSystemTick();
    // ���һ�����Ƴ�ʶ��ʱ��
    DWORD32 dwLastPlateTime = 0;
    DWORD32 dwSignalTime = 0;
    m_iOutPutNoPlate = m_cModuleParams.cTrackerCfgParam.iOutPutNoPlate;
    m_iPrevTime = m_cModuleParams.cTrackerCfgParam.iPrevTime;
    m_iPostTime = m_cModuleParams.cTrackerCfgParam.iPostTime;

    IMG_FRAME frame;
#ifdef SINGLE_BOARD_PLATFORM
    // ���������֤���
    int iFrameCount = 0;
    int iVerifyCrypt = 0;
    const int VERIFY_CRYPT_PARAM = 518400;
    srand((int)time(0));
#endif

    if(m_cModuleParams.cTrackerCfgParam.iDisableFlashAtDay == 0)
    {
        SetCaptureSynSignalEnableFlash(TRUE);
    }
    while (!m_fExit)
    {
        if (!m_pVideoGetter)
        {
            HV_Sleep(4000);
            continue;
        }

        frame = m_queImage.RemoveHead(500);
        if (NULL == frame.pRefImage)
        {
            HV_Trace(3, "get video frame error.\n");
            continue;
        }
#ifdef SINGLE_BOARD_PLATFORM
        //������֤
        iFrameCount++;
        if ( iFrameCount >= iVerifyCrypt )
        {
            if ( 0 != verify_crypt() )
            {
                // ������֤ʧ��
                Trace("\n:-(\n");
                g_fCopyrightValid = false;
            }
            else
            {
                // ������֤�ɹ�
                Trace("\n:-)\n");
                g_fCopyrightValid = true;
            }

            iFrameCount = 0;
            iVerifyCrypt = ( rand() * VERIFY_CRYPT_PARAM / RAND_MAX );
            iVerifyCrypt += VERIFY_CRYPT_PARAM;
        }

        static DWORD32 dwSendVideo = GetSystemTick();
        if (m_pVideoSender
                && m_cModuleParams.cResultSenderParam.iSaveVideo
                && GetSystemTick() - dwSendVideo >= (DWORD32)m_cModuleParams.cResultSenderParam.iVideoDisplayTime)
        {
            dwSendVideo = GetSystemTick();
            HV_COMPONENT_IMAGE imgFrame;
            frame.pRefImage->GetImage(&imgFrame);
            if (imgFrame.nImgType == HV_IMAGE_JPEG)
            {
                static DWORD32 dwLastTime = GetSystemTick();
                SEND_CAMERA_VIDEO videoInfo;
                videoInfo.dwVideoType = CAMERA_VIDEO_JPEG;
                videoInfo.dwFrameType = GetSystemTick() - dwLastTime >= 1000 ? CAMERA_FRAME_I : CAMERA_FRAME_P;
                if (videoInfo.dwFrameType == CAMERA_FRAME_I)
                {
                    dwLastTime = GetSystemTick();
                }
                GetSystemTime(&videoInfo.dwTimeLow, &videoInfo.dwTimeHigh);
                videoInfo.dwVideoSize = imgFrame.iWidth;
                videoInfo.pbVideo = GetHvImageData(&imgFrame, 0);
                videoInfo.pRefImage = frame.pRefImage;
                if ( FAILED(m_pVideoSender->SendCameraVideo(&videoInfo)) )
                {
                    HV_Trace(5, "<link>SendCameraVideo failed!\n");
                }
            }
        }
#endif
        dwFrameCount++;
        if (dwFrameCount >= 100)
        {
            dwCurSecCount = GetSystemTick() / 1000;
            m_fltFrameRate = (float)(dwFrameCount) / (dwCurSecCount - dwSecondCount);
            HV_Trace(5, "fps: %0.1f\n", m_fltFrameRate);
            dwFrameCount = 0;
            dwSecondCount = dwCurSecCount;
        }

        PROCESS_EVENT_STRUCT cProcessEvent;
        if (g_iControllPannelWorkStyle == 1)
        {
            bool fIsUpdatingPannelParam = ((CVideoGetter_VPIF*)m_pVideoGetter)->CheckIfChangeParam();
            if (fIsUpdatingPannelParam)
                cProcessEvent.fIsCheckLight = false;
            else
                cProcessEvent.fIsCheckLight = true;
        }
        else
        {
            cProcessEvent.fIsCheckLight = true;
        }
        bool fIsCapture = frame.pRefImage->IsCaptureImage();
        HRESULT hr = E_FAIL;
        // ץ��ͼ������ʶ��
        if(!fIsCapture)
        {
            DWORD32 dwBeginRecog = GetSystemTick();
            hr = m_pVideoRecoger->ProcessOneFrame(
                             frame.iVideoID,
                             frame.pRefImage,
                             NULL,
                             NULL,
                             &cProcessEvent
                         );
            DWORD32 dwRecogTime = GetSystemTick() - dwBeginRecog;
            if ( dwRecogTime > 120 )
            {
                HV_Trace(5, "<DataCtrl> Recog relay : %d\n", dwRecogTime);
            }

            //�ź�ƥ��
            if (hr == S_OK)
            {
                hr = m_pSignalMatch->ProcessOneFrame(
                         frame.iVideoID,
                         frame.pRefImage,
                         NULL,
                         NULL,
                         &cProcessEvent
                     );
                m_pVideoGetter->SetLightType(cProcessEvent.cLightType, cProcessEvent.iCplStatus);
                if (g_iControllPannelWorkStyle == 1)
                {
                    ((CVideoGetter_VPIF*)m_pVideoGetter)->SetPannelStatus(cProcessEvent.iCplStatus, cProcessEvent.iPulseLevel);
                    if(m_cModuleParams.cTrackerCfgParam.iDisableFlashAtDay)
                    {
                        SetCaptureSynSignalEnableFlash(cProcessEvent.fIsNight);
                    }
                }
            }

            m_pResultSender->PutVideo(
                &dwVideoFrameCount,
                &frame,
                cProcessEvent.cTrackRectInfo.dwTrackCount,
                cProcessEvent.cTrackRectInfo.rgTrackRect
            );
        }

        if(m_iOutPutNoPlate == 1)
        {
            // ��ǰʱ�䣬ÿ֡������
            DWORD32 dwCurTime = GetSystemTick();

            if(fIsCapture)
            {
                HV_Trace(5,"Sig time:%d\n",dwSignalTime);
                // ����ź�δ��ƥ���������һ���źţ���ɾ�����������ź�
                if(m_pRefImage != NULL)
                {
                    HV_Trace(5,"Other Signal:%d Last:%d",dwCurTime,dwLastPlateTime);
                }
                else
                {
					dwSignalTime = GetSystemTick();
                    m_pRefImage = frame.pRefImage;
                    m_pRefImage->AddRef();
                    m_fIsMatch = false;
                    HV_Trace(5,"Signal Arrive ...");
                }
            }

            // ���ֻ��ǰƥ������������
            if(fIsCapture && m_iPostTime <= 0)
            {
                HV_Trace(5,"Enter Pre ...");
                bool fMatchPlate = IsMatchPlateFT(dwCurTime,dwLastPlateTime,
                                            m_iPrevTime,m_iPostTime);
                if(fMatchPlate == true
                    || cProcessEvent.iCarLeftInfoCount > 0)
                {
                    // �ͷŹ����ڴ�����
                    SAFE_RELEASE(m_pRefImage);
                    m_fIsMatch = true;
                    HV_Trace(5,"Match Cur:%d Last:%d...",dwCurTime, dwLastPlateTime);
                }
                else
                {
                    OutPutNoPlateFT();
                }

            }
            else if(fIsCapture && m_iPostTime > 0)
            {
                HV_Trace(5,"Enter Pos ...");
                // �����Ҫ���ƥ�䣬�����ж���ǰƥ�����
                // ���ж����ƥ����� ���С����ǰƥ��ʱ�䣬˵��ƥ����
                // �շ�վģʽ�£�ÿ��ֻ��1���������Բ�ʹ�ö���

                // �����ǰʱ�� - �������ʱ�� < ���ƥ��ʱ�� ������
                if(dwCurTime - dwLastPlateTime < m_iPrevTime
                    || cProcessEvent.iCarLeftInfoCount > 0)
                {
                    m_fIsMatch = true;
                    SAFE_RELEASE(m_pRefImage);
                    HV_Trace(5,"Filter Signal Current Time:%d\n",dwCurTime);
                }
                // �����ǰʱ�� - �ź�ʱ�� > ���ƥ��ʱ�� ���޳���
                else if(dwCurTime - dwSignalTime > m_iPostTime
                    && cProcessEvent.iCarLeftInfoCount == 0)
                {
                    OutPutNoPlateFT();
                }
                else
                {
                    m_fIsMatch = false;
                }
            }
            else if(m_fIsMatch == false && m_iPostTime > 0)
            {
                // ������ƥ�䲻�ϣ������ƥ��
                // ��ʱ����Ҫ������ǰƥ��ʱ��
                if((dwCurTime - dwSignalTime > m_iPostTime)
                    && cProcessEvent.iCarLeftInfoCount == 0)
                {
                    OutPutNoPlateFT();
                }
            }
        }


        // �ͷŹ����ڴ�����
        SAFE_RELEASE(frame.pRefImage);

        if (S_OK == hr)
        {
            if ( cProcessEvent.dwEventId & EVENT_CARARRIVE )
            {
                for ( int n=0; n<cProcessEvent.iCarArriveInfoCount; ++n )
                {
                    CarArrive(&cProcessEvent.rgCarArriveInfo[n], NULL);
                    memset(&cProcessEvent.rgCarArriveInfo[n], 0, sizeof(CARARRIVE_INFO_STRUCT));
                }
            }

            if ( cProcessEvent.dwEventId & EVENT_CARLEFT )
            {
                for ( int n=0; n<cProcessEvent.iCarLeftInfoCount; ++n )
                {
                    m_pCarLeftThread->PutOneResult(cProcessEvent.rgCarLeftInfo[n]);
                    m_nCarLeftCount++;
                    //����г��ƽ������������������ʱ��
                    dwLastPlateTime = GetSystemTick();
                    if(m_fIsMatch == false)
                    {
                        m_fIsMatch = true;
                        SAFE_RELEASE(m_pRefImage);
                    }
                }
            }
        }
#ifndef SINGLE_BOARD_PLATFORM
        //���㳵������������������AGC����
        ScaleCarFlowRate();
        //���ݻ�����������AWB�������Զ���ƽ�⣬�����ֶ���ƽ��
        SetAWBFromLightType(cProcessEvent.cLightType);
#endif
    }

    return S_OK;
}

HRESULT CTollGate::SetAWBFromLightType(LIGHT_TYPE emLightType)
{
    if (m_cModuleParams.cResultSenderParam.iAWBLinkage <= 0)
    {
        return S_OK;
    }

    static int nLastLightType = -1;
    static DWORD32 dwLastSecCount = GetSystemTick() / 1000;

    if (nLastLightType == emLightType)
    {
        return S_OK;
    }

    //60sͳ��һ��
    if (GetSystemTick () - dwLastSecCount < 60)
    {
        return S_OK;
    }

    dwLastSecCount = GetSystemTick();

    HRESULT hr;
    HRESULT hrAwb;
    int nAWBValue = 0;
    int nReturnSize = sizeof(HRESULT);

    if (emLightType == DAY)
    {
        nAWBValue = 0;
    }
    else
    {
        nAWBValue = 1;
    }

    hr = g_cHvPciLinkApi.SendData(
             PCILINK_SET_AWB_STATE,
             &nAWBValue, sizeof(int),
             &hrAwb, &nReturnSize
         );

    if (hr != S_OK || hrAwb != S_OK)
    {
        HV_Trace(5, "Set AWB failed!\n");
        return E_FAIL;
    }

    nLastLightType = emLightType;

    return S_OK;
}

HRESULT CTollGate::ScaleCarFlowRate()
{
    if (m_cModuleParams.cResultSenderParam.fltCarAGCLinkage <= 0.0f)
    {
        return S_OK;
    }

    static DWORD32 dwLastSecCount;

    DWORD32 dwCurSecCount = 0;
    HRESULT hr;
    HRESULT hrAgc;
    int nAGCValue = 0;
    int nReturnSize = sizeof(HRESULT);

    dwCurSecCount = GetSystemTick() / 1000;
    // ͳ�Ƴ�������5����ͳ��һ��
    if (dwCurSecCount - dwLastSecCount >= 5 * 60)
    {
        float fltCarAGCLinkage = (float)m_nCarLeftCount / (float)(dwCurSecCount - dwLastSecCount);

        if (fltCarAGCLinkage > m_cModuleParams.cResultSenderParam.fltCarAGCLinkage)
        {
            nAGCValue = 0;
        }
        else
        {
            nAGCValue = 1;
        }

        hr = g_cHvPciLinkApi.SendData(
                 PCILINK_SET_AGC_STATE,
                 &nAGCValue, sizeof(int),
                 &hrAgc, &nReturnSize
             );

        if (hr != S_OK || hrAgc != S_OK)
        {
            HV_Trace(5, "%s AGC failed!\n", nAGCValue == 0 ? "Close" : "Open");
            m_nCarLeftCount = 0;
            dwLastSecCount = dwCurSecCount;
            return E_FAIL;
        }

        m_nCarLeftCount = 0;
        dwLastSecCount = dwCurSecCount;
        m_cModuleParams.cCamCfgParam.iEnableAGC = nAGCValue;
    }

    return S_OK;
}

void CTollGate::DspSoftTrigger()
{
#ifdef SINGLE_BOARD_PLATFORM
    if (!m_pCamTrigger)
    {
        return;
    }
    m_pCamTrigger->Trigger((LIGHT_TYPE)0, 0);
#endif
}

HRESULT CTollGate::CarArrive(
    CARARRIVE_INFO_STRUCT *pCarArriveInfo,
    LPVOID pvUserData
)
{
    HV_Trace(
        3, "<CarArriveInfo>P:%d,%d, Depo:%d, Trigger:%d, Image:%d, ARM:%d\n",
        pCarArriveInfo->dwFirstPos,
        pCarArriveInfo->dwEndPos,
        pCarArriveInfo->dwTriggerOutDelay,
        pCarArriveInfo->dwCarArriveRealTime,
        pCarArriveInfo->dwCarArriveTime,
        GetSystemTick()
    );

    //һ�����DSP�˴�������������ARM�˴���
#ifdef SINGLE_BOARD_PLATFORM
    if (!m_pCamTrigger)
    {
        return S_OK;
    }

    m_pCamTrigger->Trigger(
        (LIGHT_TYPE)pCarArriveInfo->iPlateLightType,
        pCarArriveInfo->iRoadNumber
    );
#endif
    return S_OK;
}

HRESULT CTollGate::CarLeft(
    CARLEFT_INFO_STRUCT *pCarLeftInfo,
    LPVOID pvUserData
)
{
    if (!pCarLeftInfo || !m_pResultSender)
    {
        return E_FAIL;
    }

    int iStringLen = MAX_PLATE_STRING_SIZE;
    if (BuildPlateString(m_szPlateString, &iStringLen, pCarLeftInfo) == S_OK
            && m_pResultSender->PutResult(m_szPlateString, pCarLeftInfo) == S_OK)
    {
        m_dwLastSendTime = GetSystemTick();
    }

    return S_OK;
}

// �Ƿ�ƥ���ϳ���ʱ�� �˴�ֻ������ǰƥ��
bool CTollGate::IsMatchPlateFT(DWORD32 dwTimeNow, DWORD32 dwLastPlateTime,
                      DWORD32 dwPrevTime, DWORD32 dwPostTime)
{
    //�����ǰ�����ƥ��ʱ�䶼Ϊ0����ƥ�䲻�ɹ���Ӧ��������
    if(dwPrevTime <= 0 && dwPostTime <= 0)
    {
        HV_Trace(5,"OutPutNoPlate ...");
        return false;
    }

    //�����ǰƥ��ʱ�����Ƴ��ķ�Χ�ڣ���ƥ��ɹ���������
    if(dwTimeNow - dwLastPlateTime < dwPrevTime
       && dwPostTime <= 0)
    {
       HV_Trace(5,"Match the Prev Plate ...");
       return true;
    }

    if(dwTimeNow - dwLastPlateTime > dwPrevTime
       && dwPostTime <= 0)
    {
       HV_Trace(5,"Prev Plate time out...");
       return false;
    }

    return true;
}

// ������Ƴ���Ϣ
void CTollGate::OutPutNoPlateFT()
{
    HV_Trace(5,"Send Result:Cur:%d Pre:%d Pos:%d",GetSystemTick(), m_iPrevTime, m_iPostTime);
    CARLEFT_INFO_STRUCT stCarleftInfo;
    GetNoPlateInfo(stCarleftInfo, m_pRefImage);
    m_pCarLeftThread->PutOneResult(stCarleftInfo);
    SAFE_RELEASE(m_pRefImage);
    m_nCarLeftCount++;
    m_fIsMatch = true;
}

// ��ȡ���Ƴ���Ϣ
void CTollGate::GetNoPlateInfo(CARLEFT_INFO_STRUCT& stCarLeftInfo, IReferenceComponentImage* pRefImage)
{
    IReferenceComponentImage *pImage = pRefImage;
    RESULT_IMAGE_STRUCT reImage;

    reImage.pimgBestSnapShot = pImage;
    if (S_OK == ConvertToJpeg(&reImage.pimgBestSnapShot, true))
    {
        HV_Trace(5,"add BestSnapShot");
        reImage.pimgBestSnapShot->AddRef();
        reImage.pimgLastSnapShot = reImage.pimgBestSnapShot;
        reImage.pimgLastSnapShot->AddRef();
    }

    stCarLeftInfo.cCoreResult.iSendCapImageFlag = 1;
    reImage.pimgBestCapture = NULL;
    reImage.pimgLastCapture = NULL;

    reImage.pimgPlateBin = NULL;
    reImage.pimgPlate = NULL;

    // ����ṹ��ֵ
    stCarLeftInfo.cCoreResult.rgbContent[0] = 0;

    stCarLeftInfo.cCoreResult.nType = PLATE_UNKNOWN;
    stCarLeftInfo.cCoreResult.nColor = PC_UNKNOWN;
    stCarLeftInfo.cCoreResult.fltAverageConfidence = 0;
    stCarLeftInfo.cCoreResult.fltFirstAverageConfidence = 0;
    stCarLeftInfo.cCoreResult.cResultImg = reImage;
    stCarLeftInfo.cOtherInfo.nPlateLightType = m_nLastLightLevel;

    if (pImage != NULL)
    {
        stCarLeftInfo.cOtherInfo.iFrameNo = pImage->GetFrameNo();
        stCarLeftInfo.cOtherInfo.iRefTime = pImage->GetRefTime();
        stCarLeftInfo.cOtherInfo.nCarArriveTime = pImage->GetRefTime();
    }
    else
    {
        stCarLeftInfo.cOtherInfo.iFrameNo = 0;
        stCarLeftInfo.cOtherInfo.iRefTime = 0;
        stCarLeftInfo.cOtherInfo.nCarArriveTime = 0;
    }
    stCarLeftInfo.cOtherInfo.nStartFrameNo = 0;
    stCarLeftInfo.cOtherInfo.nEndFrameNo = 0;
    stCarLeftInfo.cOtherInfo.nFirstFrameTime = 0;
    stCarLeftInfo.cOtherInfo.iObservedFrames = 0;
    stCarLeftInfo.cCoreResult.fltAverageConfidence = 0;
    stCarLeftInfo.cCoreResult.fltFirstAverageConfidence = 0;
    stCarLeftInfo.cCoreResult.fReverseRun = false;
    stCarLeftInfo.cCoreResult.nRoadNo = 0;

    stCarLeftInfo.cOtherInfo.nCarLeftCount = 0;			            // ȡ�ó����뿪֡��������
    stCarLeftInfo.cCoreResult.iAvgY = 0;							// ��������
    stCarLeftInfo.cCoreResult.fltCarspeed = 0;						// ���Ƴ��ٶ�Ϊ0

    stCarLeftInfo.cOtherInfo.iRoadNumberBegin = 0;
    stCarLeftInfo.cOtherInfo.iStartRoadNum = 0;

    stCarLeftInfo.cCoreResult.fIsCapture = false;
}

HRESULT CTollGate::BuildPlateString(
    char* pszPlateString,
    int* piPlateStringSize,
    CARLEFT_INFO_STRUCT *pCarLeftInfo
)
{
    if (pCarLeftInfo == NULL || pszPlateString == NULL || piPlateStringSize == NULL) return E_INVALIDARG;

    if (!m_pXmlDoc)	//����ĵ�Ϊ���򴴽����ĵ�
    {
        m_pXmlDoc = new TiXmlDocument;
        TiXmlDeclaration* pDecl = new TiXmlDeclaration("1.0", "GB2312", "yes");
        TiXmlElement* pRoot = new TiXmlElement("HvcResultDoc");

        if ( !m_pXmlDoc || !pDecl || !pRoot )
        {
            SAFE_DELETE(m_pXmlDoc);
            SAFE_DELETE(pDecl);
            SAFE_DELETE(pRoot);
            return E_OUTOFMEMORY;
        }

        m_pXmlDoc->LinkEndChild(pDecl);
        m_pXmlDoc->LinkEndChild(pRoot);
    }

    //ȡ��ResultSet��
    TiXmlElement* pResultSet = m_pXmlDoc->RootElement()->FirstChildElement("ResultSet");
    if (!pResultSet)
    {
        pResultSet = new TiXmlElement("ResultSet");
        if ( !pResultSet ) return E_OUTOFMEMORY;
        m_pXmlDoc->RootElement()->LinkEndChild(pResultSet);
    }

    //ע�⣺һ��Ҫɾ���Ѿ����ڵĽ�
    TiXmlNode* pResultOld = pResultSet->FirstChild("Result");
    if ( pResultOld )
    {
        pResultSet->RemoveChild(pResultOld);
    }

    //д��Result
    TiXmlElement* pResult = new TiXmlElement("Result");
    if (pResult)
    {
        pResultSet->LinkEndChild(pResult);

        //����
        static char szConf[128] = {0};
        static char szPlateName[32] = {0};
        static char szFrameName[64] = {0};
        static char szCarType[16] = {0};
        bool fIsDouble = false;
        bool fIsDoubleMoto = false;

        GetPlateNameAlpha(
            (char*)szPlateName,
            ( PLATE_TYPE )pCarLeftInfo->cCoreResult.nType,
            ( PLATE_COLOR )pCarLeftInfo->cCoreResult.nColor,
            pCarLeftInfo->cCoreResult.rgbContent
        );

        if (strstr(szPlateName, "11111") != NULL)
        {
            HV_Trace(5, "M");
            return S_FALSE;
        }

        if (pCarLeftInfo->cCoreResult.nType == PLATE_DOUBLE_YELLOW)
        {
            fIsDouble = true;
        }
        if (pCarLeftInfo->cCoreResult.nType == PLATE_DOUBLE_MOTO)
        {
            fIsDoubleMoto = true;
        }

        HV_Trace(5, "%s\n", szPlateName);  //��������ַ���

        //���Ƴ�
        if ((*(pCarLeftInfo->cCoreResult.rgbContent)) == 0)
        {
            //HV_Trace("���Ƴ�");
        }

        TiXmlElement* pValue = new TiXmlElement("PlateName");
        TiXmlText* pText = new TiXmlText(szPlateName);
        if (pValue && pText)
        {
            pValue->LinkEndChild(pText);
            pResult->LinkEndChild(pValue);
        }

        int nColorType = 0;
        if (strncmp(szPlateName, "��", 2) == 0)
        {
            nColorType = 1;
        }
        else if (strncmp(szPlateName, "��", 2) == 0)
        {
            nColorType = 2;
        }
        else if (strncmp(szPlateName, "��", 2) == 0)
        {
            nColorType = 3;
        }
        else if (strncmp(szPlateName, "��", 2) == 0)
        {
            nColorType = 4;
        }
        else if (strncmp(szPlateName, "��", 2) == 0)
        {
            nColorType = 5;
        }
        else
        {
            nColorType = 0;
        }

        int nPlateType = 0;
        switch ( pCarLeftInfo->cCoreResult.nType )
        {
        case PLATE_NORMAL:
        case PLATE_POLICE:
            nPlateType = 1;
            break;
        case PLATE_WJ:
            nPlateType = 2;
            break;
        case PLATE_POLICE2:
            nPlateType = 3;
            break;
        case PLATE_DOUBLE_YELLOW:
        case PLATE_DOUBLE_MOTO:
            nPlateType = 4;
            break;
        default:
            nPlateType = 0;
            break;
        }

        pValue = new TiXmlElement("Color");
        if ( pValue )
        {
            pValue->SetAttribute("raw_value", pCarLeftInfo->cCoreResult.nColor);
            pValue->SetAttribute("value", nColorType);
            pResult->LinkEndChild(pValue);
        }

        pValue = new TiXmlElement("Type");
        if ( pValue )
        {
            pValue->SetAttribute("raw_value", pCarLeftInfo->cCoreResult.nType);
            pValue->SetAttribute("value", nPlateType);
            pResult->LinkEndChild(pValue);
        }

        //������������Ϣ
        if (m_cModuleParams.cResultSenderParam.fOutputAppendInfo)
        {
            if (m_cModuleParams.cResultSenderParam.fOutputObservedFrames)
            {
                //��Ч֡��
                pValue = new TiXmlElement("ObservedFrames");
                if (pValue)
                {
                    sprintf(szConf, "%d", pCarLeftInfo->cCoreResult.iObservedFrames);
                    pValue->SetAttribute("value", szConf);
                    pValue->SetAttribute("chnname", "��Ч֡��");
                    pResult->LinkEndChild(pValue);
                }

                //���Ŷ�
                pValue = new TiXmlElement("Confidence");
                if (pValue)
                {
                    sprintf(
                        szConf, "%.3f",
                        exp(log(pCarLeftInfo->cCoreResult.fltAverageConfidence) * 0.143)
                    );
                    pValue->SetAttribute("value", szConf);
                    pValue->SetAttribute("chnname", "ƽ�����Ŷ�");
                    pResult->LinkEndChild(pValue);
                }

                //���ַ����Ŷ�
                pValue = new TiXmlElement("FirstCharConf");
                if (pValue)
                {
                    sprintf(szConf, "%.3f", pCarLeftInfo->cCoreResult.fltFirstAverageConfidence);
                    pValue->SetAttribute("value", szConf);
                    pValue->SetAttribute("chnname", "���ֿ��Ŷ�");
                    pResult->LinkEndChild(pValue);
                }
            }

            //�������ʱ��
            if (m_cModuleParams.cResultSenderParam.fOutputCarArriveTime)
            {
                pValue = new TiXmlElement("CarArriveTime");
                if (pValue)
                {
                    sprintf(szConf, "%u", pCarLeftInfo->cCoreResult.nFirstFrameTime);
                    pValue->SetAttribute("value", szConf);
                    pValue->SetAttribute("chnname", "�������ʱ��");
                    pResult->LinkEndChild(pValue);
                }
            }

            if (fIsDouble)
            {
                //˫����
                pValue = new TiXmlElement("DoublePlate");
                if (pValue)
                {
                    pValue->SetAttribute("value", "˫");
                    pValue->SetAttribute("chnname", "��������");
                    pResult->LinkEndChild(pValue);
                }
            }

            if (fIsDoubleMoto)
            {
                //Ħ��
                pValue = new TiXmlElement("DoubleMoto");
                if (pValue)
                {
                    pValue->SetAttribute("value", "Ħ");
                    pValue->SetAttribute("chnname", "��������");
                    pResult->LinkEndChild(pValue);
                }
            }

            //����������
            pValue = new TiXmlElement("AmbientLight");
            if (pValue)
            {
                sprintf(szConf, "%d", pCarLeftInfo->cCoreResult.iAvgY);
                pValue->SetAttribute("value", szConf);
                pValue->SetAttribute("chnname", "��������");
                pResult->LinkEndChild(pValue);
            }

            //��������
            pValue = new TiXmlElement("PlateLight");
            if (pValue)
            {
                sprintf(szConf, "%d", pCarLeftInfo->cCoreResult.iCarAvgY);
                pValue->SetAttribute("value", szConf);
                pValue->SetAttribute("chnname", "��������");
                pResult->LinkEndChild(pValue);
            }

            //���ƶԱȶ�
            pValue = new TiXmlElement("PlateVariance");
            if (pValue)
            {
                sprintf(szConf, "%d", pCarLeftInfo->cCoreResult.iCarVariance);
                pValue->SetAttribute("value", szConf);
                pValue->SetAttribute("chnname", "���ƶԱȶ�");
                pResult->LinkEndChild(pValue);
            }

            /*//��������
            pValue = new TiXmlElement("CarType");
            if (pValue)
            {
                pValue->SetAttribute("value", szCarType);
                pValue->SetAttribute("chnname", "��������");
                pResult->LinkEndChild(pValue);
            }*/

            //������ɫ
            if (m_cModuleParams.cTrackerCfgParam.fEnableRecgCarColor)
            {
                pValue = new TiXmlElement("CarColor");
                if (pValue)
                {
                    GetCarColor(pCarLeftInfo, szConf);
                    pValue->SetAttribute("value", szConf);
                    pValue->SetAttribute("chnname", "������ɫ");
                    pResult->LinkEndChild(pValue);
                }
            }

            //·������
            pValue = new TiXmlElement("StreetName");
            if (pValue)
            {
                pValue->SetAttribute("value", m_cModuleParams.cResultSenderParam.szStreetName);
                pValue->SetAttribute("chnname", "·������");
                pResult->LinkEndChild(pValue);
            }

            //·�ڷ���
            pValue = new TiXmlElement("StreetDirection");
            if (pValue)
            {
                pValue->SetAttribute("value", m_cModuleParams.cResultSenderParam.szStreetDirection);
                pValue->SetAttribute("chnname", "·�ڷ���");
                pResult->LinkEndChild(pValue);
            }

            if (pCarLeftInfo->cCoreResult.cResultImg.pimgBestSnapShot)
            {
                //��Ƶ֡��
                strcpy(
                    szFrameName,
                    pCarLeftInfo->cCoreResult.cResultImg.pimgBestSnapShot->GetFrameName()
                );
                if (strlen(szFrameName) != 0
#ifdef SINGLE_BOARD_PLATFORM
                        && m_cModuleParams.cCamCfgParam.iCamType == 0 // ������0���ǲ���Э��
#else
                        && m_cModuleParams.cCamCfgParam.iCamType == 1
#endif
                   )
                {
                    pValue = new TiXmlElement("FrameName");
                    if ( pValue )
                    {
                        char *pFirst = szFrameName, *pSecond = szFrameName;
                        char *pTemp = strstr(pFirst, ".");
                        while (pTemp)
                        {
                            pSecond = pTemp;
                            pTemp = strstr(pTemp + 1, ".");
                        }
                        szFrameName[pSecond - pFirst] = 0;
                        pValue->SetAttribute("value", szFrameName);
                        pValue->SetAttribute("chnname", "��Ƶ֡��");
                        pResult->LinkEndChild(pValue);
                    }
                }
            }

            //��ǰ���ȼ���
            pValue = new TiXmlElement("PlateLightType");
            if (pValue)
            {
                sprintf(szConf, "%02d��", pCarLeftInfo->cCoreResult.nPlateLightType);
                pValue->SetAttribute("value", szConf);
                pValue->SetAttribute("chnname", "��������ȵȼ�");
                pResult->LinkEndChild(pValue);
            }
#ifndef SINGLE_BOARD_PLATFORM
            //��ǰƫ�⾵״̬
            pValue = new TiXmlElement("CplStatus");
            if (pValue)
            {
                sprintf(szConf, "%d", pCarLeftInfo->cCoreResult.iCplStatus);
                pValue->SetAttribute("value", szConf);
                pValue->SetAttribute("chnname", "ƫ�⾵״̬");
                pResult->LinkEndChild(pValue);
            }

            if (g_iControllPannelWorkStyle == 1)
            {
                //��ǰ�����������
                pValue = new TiXmlElement("PulseLevel");
                if (pValue)
                {
                    sprintf(szConf, "%d", pCarLeftInfo->cCoreResult.iPulseLevel);
                    pValue->SetAttribute("value", szConf);
                    pValue->SetAttribute("chnname", "��������ȼ�");
                    pResult->LinkEndChild(pValue);
                }

                //�����״̬�ж�
                static int iFillLightCount = 0;
                static int iFillLightMaxCount = (m_cModuleParams.cTrackerCfgParam.nPlateLightCheckCount << 1);
                if(iFillLightMaxCount < 20) iFillLightMaxCount = 20;
                int iMinAvgY = m_cModuleParams.cTrackerCfgParam.nMinPlateBrightness;
                if (pCarLeftInfo->cCoreResult.iPulseLevel > 0)
                {
                    if (pCarLeftInfo->cCoreResult.iCarAvgY < 50)
                    {
                        iFillLightCount++;
                    }
                    else
                    {
                        iFillLightCount = 0;
                    }
                    if (m_nLastLightLevel != pCarLeftInfo->cCoreResult.iPulseLevel)
                    {
                        iFillLightCount = 0;
                        m_nLastLightLevel = pCarLeftInfo->cCoreResult.iPulseLevel;
                    }
                    if (iFillLightCount > iFillLightMaxCount )
                    {
                        iFillLightCount = iFillLightMaxCount;
                        pValue = new TiXmlElement("FlashLightStatus");
                        if (pValue)
                        {
                            sprintf(szConf, "%s", "�쳣");
                            pValue->SetAttribute("value", szConf);
                            pValue->SetAttribute("chnname", "����ƹ���״̬");
                            pResult->LinkEndChild(pValue);
                        }
                    }
                    else
                    {
                        pValue = new TiXmlElement("FlashLightStatus");
                        if (pValue)
                        {
                            sprintf(szConf, "%s", "����");
                            pValue->SetAttribute("value", szConf);
                            pValue->SetAttribute("chnname", "����ƹ���״̬");
                            pResult->LinkEndChild(pValue);
                        }
                    }
                }

                //�����״̬�ж�
                static int iFlashLampCount = 0;
                if (pCarLeftInfo->cCoreResult.fIsCapture)
                {
                    if (pCarLeftInfo->cCoreResult.rgbContent[0] != 0)
                    {
                        if (pCarLeftInfo->cCoreResult.iPulseLevel > 0)
                        {
                            if (pCarLeftInfo->cCoreResult.iCapturerAvgY - pCarLeftInfo->cCoreResult.iAvgY
                                    < m_nFlashLampDiff)
                            {
                                iFlashLampCount++;
                            }
                            else
                            {
                                iFlashLampCount = 0;
                            }

                            if (iFlashLampCount > 5)
                            {
                                iFlashLampCount = 5;
                                pValue = new TiXmlElement("CaptureLightStatus");
                                if (pValue)
                                {
                                    sprintf(szConf, "%s", "�쳣");
                                    pValue->SetAttribute("value", szConf);
                                    pValue->SetAttribute("chnname", "����ƹ���״̬");
                                    pResult->LinkEndChild(pValue);
                                }
                            }
                            else
                            {
                                pValue = new TiXmlElement("CaptureLightStatus");
                                if (pValue)
                                {
                                    sprintf(szConf, "%s", "����");
                                    pValue->SetAttribute("value", szConf);
                                    pValue->SetAttribute("chnname", "����ƹ���״̬");
                                    pResult->LinkEndChild(pValue);
                                }
                            }
                        }
                        else
                        {
                            iFlashLampCount = 0;
                        }
                    }
                }
                else
                {
                    if (pCarLeftInfo->cCoreResult.rgbContent[0] != 0)
                        iFlashLampCount = 0;
                }
            }
#endif
        } // end of if (m_cModuleParams.cResultSenderParam.fOutputAppendInfo)

        m_pLightTypeSaver->SaveLightType(
            pCarLeftInfo->cCoreResult.nPlateLightType,
            pCarLeftInfo->cCoreResult.iPulseLevel,
            pCarLeftInfo->cCoreResult.iCplStatus,
            pCarLeftInfo->cCoreResult.nCarArriveTime
        );

    } // end of if (pResult)

    TiXmlPrinter cTxPr;
    if (m_pXmlDoc)
    {
        cTxPr.SetStreamPrinting();
        m_pXmlDoc->Accept(&cTxPr);

        int nCpyLen = MIN_INT(*piPlateStringSize, (int)cTxPr.Size() + 1);
        HV_memcpy( pszPlateString, cTxPr.CStr(), nCpyLen );
        pszPlateString[nCpyLen - 1] = '\0';
        *piPlateStringSize = nCpyLen;
    }
    else
    {
        *piPlateStringSize = 0;
    }

    return S_OK;
}

bool CTollGate::ThreadIsOk(int* piErrCode)
{
    if ( m_pVideoRecoger && S_OK != m_pVideoRecoger->GetCurStatus(NULL, 0) )
    {
        if (piErrCode) *piErrCode = 1;
        return false;
    }

    if ( m_pVideoGetter && S_OK != m_pVideoGetter->GetCurStatus(NULL, 0) )
    {
        if (piErrCode) *piErrCode = 2;
        return false;
    }

    if (!m_pSignalMatch->ThreadIsOk())
    {
        if (piErrCode) *piErrCode = 3;
        return false;
    }

#ifdef SINGLE_BOARD_PLATFORM
    if (m_pCamTrigger && !((CCamTrigger*)m_pCamTrigger)->ThreadIsOk())
    {
        if (piErrCode) *piErrCode = 4;
        return false;
    }

    if (m_pImgCapturer && m_pImgCapturer->GetCurStatus(NULL, 0) != S_OK)
    {
        if (piErrCode) *piErrCode = 5;
        return false;
    }
#endif

    if (m_pCarLeftThread && m_pCarLeftThread->IsThreadOK() != S_OK)
    {
        if (piErrCode)
        {
            (*piErrCode) = 6;
        }
        return false;
    }

    return true;
}

void CTollGate::SetModuleParams(const ModuleParams& cParam)
{
    m_cModuleParams = cParam;
}

void CTollGate::GetCarColor(CARLEFT_INFO_STRUCT *pCarLeftInfo, char *pszConf)
{
    switch (pCarLeftInfo->cCoreResult.nCarColor)
    {
    case CC_WHITE:
        sprintf(pszConf, "��ɫ");
        break;
    case CC_GREY:
        sprintf(pszConf, "��ɫ");
        break;
    case CC_BLACK:
        sprintf(pszConf, "��ɫ");
        break;
    case CC_RED:
        sprintf(pszConf, "��ɫ");
        break;
    case CC_YELLOW:
        sprintf(pszConf, "��ɫ");
        break;
    case CC_GREEN:
        sprintf(pszConf, "��ɫ");
        break;
    case CC_BLUE:
        sprintf(pszConf, "��ɫ");
        break;
    case CC_PURPLE:
        sprintf(pszConf, "��ɫ");
        break;
    case CC_PINK:
        sprintf(pszConf, "��ɫ");
        break;
    case CC_BROWN:
        sprintf(pszConf, "��ɫ");
        break;
    default:
        sprintf(pszConf, "δ֪");
        break;
    }
}

void CTollGate::GetCarType(PROCESS_IMAGE_CORE_RESULT *pImgCoreInfo, char *pszConf)
{
    if (pImgCoreInfo->nCarType == CT_LARGE && PLATE_DOUBLE_MOTO != pImgCoreInfo->nType)
    {
        strcpy(pszConf, "��");
    }
    else if (pImgCoreInfo->nCarType == CT_SMALL && PLATE_DOUBLE_MOTO != pImgCoreInfo->nType)
    {
        strcpy(pszConf, "С");
    }
    else if (pImgCoreInfo->nCarType == CT_MID && PLATE_DOUBLE_MOTO != pImgCoreInfo->nType)
    {
        strcpy(pszConf, "��");
    }
    else if (PLATE_DOUBLE_MOTO == pImgCoreInfo->nType)
    {
        strcpy(pszConf, "Ħ�г�");
    }
    else if (pImgCoreInfo->nCarType == CT_WALKMAN)
    {
        strcpy(pszConf, "����");
    }
    else if (pImgCoreInfo->nCarType == CT_BIKE)
    {
        strcpy(pszConf, "�ǻ�����");
    }
    else if (pImgCoreInfo->nCarType == CT_VEHICLE)
    {
        strcpy(pszConf, "������");
    }
    else
    {
        strcpy(pszConf, "δ֪");
    }
}

HRESULT CTollGate::SetCaptureSynSignalEnableFlash(bool fIsNight)
{
    HRESULT hr = E_FAIL;
    bool fIsNeedUpdate = false;
    int nRet = E_FAIL;
    int nSize = sizeof(nRet);
    int nStatue;

    if(m_iFlashStatus == -1)
    {
        if(fIsNight)
        {
            nStatue = 1;
        }
        else
        {
            nStatue = 0;
        }
        fIsNeedUpdate = true;
    }
    else
    {
        if(fIsNight && m_iFlashStatus == 0)
        {
            nStatue = 1;
            fIsNeedUpdate = true;
        }
        else if(!fIsNight && m_iFlashStatus == 1)
        {
            nStatue = 0;
            fIsNeedUpdate = true;
        }
    }

    if(!fIsNeedUpdate)
    {
        return S_OK;
    }

    hr = g_cHvPciLinkApi.SendData(
        PCILINK_SET_CAPTURE_SYNSIGNAL,
        &nStatue,
        sizeof(nStatue),
        &nRet,
        &nSize);
    if(hr == S_OK)
    {
        if(nRet == nStatue)
        {
            hr = S_OK;
            m_iFlashStatus = nStatue;
        }
        else
        {
            hr = E_FAIL;
        }
    }
    return hr;
}
