// ���ļ������ʽ����ΪWINDOWS-936��ʽ

#include "DataCtrl.h"
#include "HvDspLinkApi.h"
#include "hvtarget_ARM.h"
#include "TcpipCfg.h"
#include "ControlFunc.h"
#include "HvPciLinkApi.h"
#include "PciResultSender.h"
#include "DspLinkMemBlocks.h"
#include "OuterControlImpl_Linux.h"
#include "HvUtilsClass.h"
#include "HvParamIO.h"
#include <signal.h>
#include <execinfo.h>

HV_SEM_HANDLE g_hSemEDMA;
#ifdef SINGLE_BOARD_PLATFORM
#include "HvCameraLink.h"
#include "CameraCmdProcess.h"
#include "CameraCmdLink.h"
#include "CameraImageLink.h"
#include "CameraVideoLink.h"
#include "CameraRecordLink.h"
#include "DM6467ResultSender.h"
#include "CamTransmit.h"

HV_SEM_HANDLE g_hSemCameraDataCount;
HV_SEM_HANDLE g_hSemSwDev;
HV_SEM_HANDLE g_hSemEEPROM;
bool g_fCopyrightValid = true;
/* HvUtilsClass.cpp */
extern int g_nHddCheckStatus;

void CreateGlobalResource()
{
    if (CreateSemaphore(&g_hSemCameraDataCount, 1, 1) != 0)
    {
        HV_Trace(5, "create g_hSemCameraDataCount failed!\n");
        HV_Exit(HEC_FAIL, "create g_hSemCameraDataCount failed!");
    }
    if (CreateSemaphore(&g_hSemSwDev, 1, 1) != 0)
    {
        HV_Trace(5, "create g_hSemSwDev failed!\n");
        HV_Exit(HEC_FAIL, "create g_hSemSwDev failed!");
    }
    if (CreateSemaphore(&g_hSemEDMA, 1, 1) != 0)
    {
        HV_Trace(5, "create g_hSemEDMA failed!\n");
        HV_Exit(HEC_FAIL, "create g_hSemEDMA failed!");
    }
    if (CreateSemaphore(&g_hSemEEPROM, 1, 1) != 0)
    {
        HV_Trace(5, "create g_hSemEEPROM failed!\n");
        HV_Exit(HEC_FAIL, "create g_hSemEEPROM failed!");
    }
}

IResultSender * InitDM6467Protocol(
    CCameraImageLinkCtrl& imageLinkCtrl,
    CCameraRecordLinkCtrl& recordLinkCtrl,
    CCameraVideoLinkCtrl & videoLinkCtrl,
    CSafeSaverDm6467Impl* pcSafeSaver,
    CDM6467ResultSender & cDM6467DataSender,
    ResultSenderParam& cResultSenderParam
)
{
    if ( S_OK != imageLinkCtrl.Start(NULL) )
    {
        HV_Trace(5, "CCameraImageLinkCtrl Start failed!\n");
        HV_Exit(HEC_FAIL, "CCameraImageLinkCtrl Start failed!");
    }
    cDM6467DataSender.SetImageSenderCtrl(&imageLinkCtrl);

    SEND_RECORD_PARAM cSendRecordParamTmp;
    cSendRecordParamTmp.pcSafeSaver = cResultSenderParam.fIsSafeSaver ? pcSafeSaver : NULL;
    cSendRecordParamTmp.iOutputOnlyPeccancy = cResultSenderParam.iOutputOnlyPeccancy;
    cSendRecordParamTmp.iSendRecordSpace = cResultSenderParam.iSendRecordSpace;
    recordLinkCtrl.SetParam(&cSendRecordParamTmp);
    if ( S_OK != recordLinkCtrl.Start(NULL) )
    {
        HV_Trace(5, "CCameraRecordLinkCtrl Start failed!\n");
        HV_Exit(HEC_FAIL, "CCameraRecordLinkCtrl Start failed!");
    }

    SEND_VIDEO_PARAM cSendVideoParamTmp;
    cSendVideoParamTmp.pcSafeSaver = pcSafeSaver;
    cSendVideoParamTmp.iSendHisVideoSpace = cResultSenderParam.iSendHisVideoSpace;
    videoLinkCtrl.SetParam(&cSendVideoParamTmp);
    if ( S_OK != videoLinkCtrl.Start(NULL) )
    {
        HV_Trace(5, "CCameraVideoLinkCtrl Start failed!\n");
        HV_Exit(HEC_FAIL, "CCameraVideoLinkCtrl Start failed!");
    }

    cDM6467DataSender.SetRecordSenderCtrl(&recordLinkCtrl);
    cDM6467DataSender.SetSafeSaver(pcSafeSaver);
    cDM6467DataSender.Init(&cResultSenderParam);
    if (pcSafeSaver)
    {
        pcSafeSaver->SetFreeSpacePerDisk(cResultSenderParam.iRecordFreeSpacePerDisk, cResultSenderParam.iVideoFreeSpacePerDisk);
    }
    return &cDM6467DataSender;
}
#endif

CEPolice *g_pEPolice = NULL;

using namespace HiVideo;

#ifdef _USE_SEED_DEBUG_
/* DSPLink.c */
extern int g_fDebugStart;
#endif

/* HvDspLinkApi.cpp */
extern HRESULT InitDspLink(int argc, char** argv);

void PrintfErrorInfo(SLAVE_CPU_STATUS *pSlaveCapStatus, char *pszErrInfo);

HRESULT InitMemBlocksEx(int iCamType, int iEddyType);

HRESULT SetDspLinkEddyType(int nEddyType);

HRESULT InitCaptureThread(
    CImgCapturer **ppImgCapturer,
    ICamTrigger **ppCamTrigger,
    COuterControlImpl *pOuterControl,
    ModuleParams *pModuleParams,
    int iCameraType
);

/* HvSystemInit.cpp */
extern void LinuxSystemInit(void);

int main(int argc, char** argv)
{
    HV_Trace(5, "Begin...\n");

    LinuxSystemInit();

    CUserExitThread e;
    e.Start(NULL);

#ifdef SINGLE_BOARD_PLATFORM
    CreateGlobalResource();
    EnableWatchDog();
#else
    if (CreateSemaphore(&g_hSemEDMA, 1, 1) != 0)
    {
        HV_Trace(5, "create g_hSemEDMA failed!\n");
        HV_Exit(HEC_FAIL, "create g_hSemEDMA failed!");
    }
#endif

    WdtHandshake();

    // �����ģ�����
    ModuleParams cModuleParams;
    if ( S_OK != LoadModuleParam(cModuleParams) )
    {
        HV_Trace(5, "LoadModuleParam failed!\n");
        HV_Exit(HEC_FAIL|HEC_RESET_DEV, "LoadModuleParam failed!");
    }

    WdtHandshake();

#ifdef SINGLE_BOARD_PLATFORM
    // �ж��Ƿ�ָ���������
    if (IsRecoverFactorySettings())
    {
        RecoverFactorySettings();
        HV_Exit(HEC_SUCC, "RecoverFactorySettings!");
    }

    WdtHandshake();
#endif

    // ��ʼ�������ڴ�
    if (S_OK != InitMemBlocksEx(cModuleParams.cCamCfgParam.iCamType,
                                cModuleParams.cTrackerCfgParam.iEddyType))
    {
        HV_Trace(5, "Memory blocks initialize failed!\n");
        HV_Exit(HEC_RESET_DEV|HEC_FAIL, "Memory blocks initialize failed!");
    }

    // ��ʼ��DSPLink
    if ( S_OK != InitDspLink(argc, argv) )
    {
        HV_Trace(5, "InitDspLink is failed!\n");
        HV_Exit(HEC_RESET_DEV|HEC_FAIL, "InitDspLink is failed!");
    }

    // ����DSP��ͼƬ��ת��ʶ
    if (S_OK != SetDspLinkEddyType(cModuleParams.cTrackerCfgParam.iEddyType))
    {
        HV_Trace(5, "Set dsp eddy type failed!\n");
        HV_Exit(HEC_FAIL, "Set dsp eddy type failed!");
    }

    int nLastLightType = (int)LIGHT_TYPE_COUNT;
    int nLastPulseLevel = 0;
    int nLastCplStatus = 0;
    CCamLightTypeSaver cCamLightTypeSaver;
    cCamLightTypeSaver.GetLastLightType(nLastLightType , nLastPulseLevel , nLastCplStatus);
    if (cModuleParams.cTrackerCfgParam.nCtrlCpl == 0)
    {
        nLastCplStatus = -1;
    }
    else
    {
        if (nLastCplStatus == -1)
        {
            nLastCplStatus = 0;
        }
    }

    // ������Ƶ��ʶ�����
    CVideoRecoger* pVideoRecoger = new CVideoRecoger();

    // ��ʼ����Ƶ��ʶ�����
    if ( S_OK != pVideoRecoger->Init(cModuleParams.nWorkModeIndex,
                                     nLastLightType,
                                     nLastPulseLevel,
                                     nLastCplStatus,
                                     cModuleParams.cTrackerCfgParam) )
    {
#ifndef SINGLE_BOARD_PLATFORM
        g_cHvPciLinkApi.SendData(PCILINK_RESET_HV, NULL, 0);
#endif
        HV_Trace(5, "VideoRecoger init failed!\n");
        HV_Exit(HEC_FAIL, "VideoRecoger init failed!");
    }
    pVideoRecoger->SetEncodeType(1 + cModuleParams.cResultSenderParam.iOutputCaptureImage);

    // ������Ͷ���
    IResultSender *pResultSender = NULL;
    // ��Ƶ����ȡ����
    IImgGatherer* pVideoGetter = NULL;
    // �¼�������
    CEventChecker_EP *pEventChecker = NULL;
    // �����ź�ƥ�����
    COuterControlImpl *pOuterControl = NULL;
    // �Ƿ��������������
    BOOL fCanSetBuzzer = FALSE;

    // ��ʼ���¼����
    pEventChecker = new CEventChecker_EP();
    pEventChecker->SetResultParam(&cModuleParams.cResultSenderParam);
    pEventChecker->SetTrackerParam(&cModuleParams.cTrackerCfgParam);

    // ��ʼ���ź�ƥ��
    pOuterControl = new COuterControlImpl(cModuleParams.cSignalMatchParam);

#ifdef SINGLE_BOARD_PLATFORM

    WdtHandshake();

    // �豸������Ӧ�̶߳���
    CIPSearchThread cDevSearch;
    // ����1IP�Ϸ��Ա�ʶ
    BOOL fIsIP1Correct = FALSE;

    // �ж��Ƿ��������ڣ���������ʱʹ�ã�
    if (!IsTestCamLanPort())
    {
        // �����������ǲ��ԣ�����LAN1��LAN2��
        // ȷ��LAN1��LAN2����ͬһ�����ڣ������ͬһ�����ڣ�LAN1���ᱻǿ�йر�
        if (CheckIPAddr(cModuleParams.cTcpipCfgParam_1, cModuleParams.cTcpipCfgParam_2) == S_OK)
        {
            fIsIP1Correct = TRUE;
            SetTcpipAddr_1(&cModuleParams.cTcpipCfgParam_1);
        }
    }
    else
    {
        // ���в�������ڣ�ֻ����LAN1��Ϊͨ��Ϣ�˿�
        fIsIP1Correct = TRUE;
        SetTcpipAddr_1(&cModuleParams.cTcpipCfgParam_2);
    }

    SetAllLightOff();
    CSafeSaverDm6467Impl* pcSafeSaver = NULL;

    int iDiskCount = 0;
    if ( cModuleParams.cResultSenderParam.fIsSafeSaver )
    {
        ++iDiskCount;
    }
    if ( cModuleParams.cResultSenderParam.iSaveVideo )
    {
        ++iDiskCount;
    }

    if (0 < cModuleParams.cResultSenderParam.fInitHdd)
    {
        SetAllLightOn();
        g_cHddOpThread.SetHddOpType(cModuleParams.cResultSenderParam.fInitHdd == 1 ? HOT_FULL_INIT : HOT_CHECKFACTOR, iDiskCount);
        g_cHddOpThread.Start(NULL);
        HV_Trace(5, "Hdd init...\n");
    }
    if (cModuleParams.cResultSenderParam.fIsSafeSaver || cModuleParams.cResultSenderParam.iSaveVideo)
    {
        if (!cModuleParams.cResultSenderParam.fInitHdd)
        {
            SetAllLightOn();
            g_cHddOpThread.SetHddOpType(HOT_INIT, iDiskCount);
            g_cHddOpThread.Start(NULL);
            HV_Trace(5, "Hdd check...\n");
        }
        pcSafeSaver = new CSafeSaverDm6467Impl();
    }
    else if (cModuleParams.cResultSenderParam.fInitHdd <= 0)
    {
        fCanSetBuzzer = TRUE;
    }

    if (pcSafeSaver)
    {
        SSafeSaveParam sParam;
        sParam.fSaveRecord = cModuleParams.cResultSenderParam.fIsSafeSaver;
        sParam.fSaveVideo = cModuleParams.cResultSenderParam.iSaveVideo;
        pcSafeSaver->Init(&sParam);
    }

    CDM6467ResultSender  cResultSender;
    CCameraImageLinkCtrl imageLinkCtrl;
    CCameraRecordLinkCtrl recordLinkCtrl;

    AUTO_LINK_PARAM* pAutoLinkParam = &cModuleParams.cResultSenderParam.cAutoLinkParam;

    recordLinkCtrl.SetAutoLink(
        pAutoLinkParam->fAutoLinkEnable,
        pAutoLinkParam->szAutoLinkIP,
        pAutoLinkParam->iAutoLinkPort
    );

    recordLinkCtrl.EnableMTUSet(&cModuleParams.cResultSenderParam.fMTUSetEnable);

    CCameraVideoLinkCtrl  videoLinkCtrl;
    pResultSender = InitDM6467Protocol(
                        imageLinkCtrl,
                        recordLinkCtrl,
                        videoLinkCtrl,
                        pcSafeSaver,
                        cResultSender,
                        cModuleParams.cResultSenderParam
                    );

    // �����������Э������Լ�������Ͷ���
    ICameraCmdProcess* pICameraCmdProcess = NULL;
    if ( S_OK != CCameraCmdProcess::CreateICameraCmdProcess(&pICameraCmdProcess) )
    {
        HV_Trace(5, "CreateICameraCmdProcess is failed!\n");
        HV_Exit(HEC_FAIL, "CreateICameraCmdProcess is failed!");
    }

    CCameraCmdLinkCtrl cCameraCmdLinkCtrl(pICameraCmdProcess);
    if ( S_OK != cCameraCmdLinkCtrl.Start(NULL) )
    {
        HV_Trace(5, "CCameraCmdLinkCtrl Start failed!\n");
        HV_Exit(HEC_FAIL, "CCameraCmdLinkCtrl Start failed!");
    }

    WdtHandshake();

    switch (cModuleParams.cCamCfgParam.iCamType)
    {
    case 0://����ר��Э��
    {
        pVideoGetter = new CVideoGetter_Test();
        HV_Trace(5, "����ר��Э��\n");
        break;
    }
    case 1: //NVCЭ��
    {
        pVideoGetter = new CVideoGetter_Nvc();
        HV_Trace(5, "NVCЭ��\n");
        break;
    }
    case 3: //�㲥Э��
    {
        pVideoGetter = new CVideoGetter_Broadcast();
        HV_Trace(5, "�㲥Э��\n");
        break;
    }
    case 4: //HV�����Э��
    {
        pVideoGetter = new CVideoGetter_HvCamera();
        HV_Trace(5, "HV�����Э��\n");
        break;
    }
    case 2://����Э��
    default:
    {
        pVideoGetter = new CVideoGetter_Camyu();
        HV_Trace(5, "����Э��\n");
        break;
    }
    }

    pVideoGetter->MountTransmiter(&g_pCamTransmit);

    WdtHandshake();

    // �����豸������Ӧ�߳�
    if ( S_OK != cDevSearch.Create() )
    {
        HV_Trace(5, "IP search thread create failed!\n");
        HV_Exit(HEC_FAIL, "IP search thread create failed!");
    }

    // ��ʼ��HvTelnet����
    if ( -1 == TelnetInit() )
    {
        HV_Trace(5, "TelnetInit is Failed!\n");
        HV_Exit(HEC_FAIL, "TelnetInit is Failed!");
    }
#else
    // PCI�����̶߳���
    CPciDataRecvThreadSlave cPciDataRecv;

    //����������ͳ�ʼ����زɼ�����
    if (cModuleParams.cCamCfgParam.iCamType == 0)
    {
        pVideoGetter = new CVideoGetter_VPIF();
        ((CVideoGetter_VPIF*)pVideoGetter)->MountSignalMatch(pOuterControl);
    }
    else
    {
        pVideoGetter = new CVideoGetter_Pci();
    }

    pResultSender = new CPciResultSender();
    ((CPciResultSender*)pResultSender)->Init(&cModuleParams.cResultSenderParam, cModuleParams.cCamCfgParam.iCamType);

    WdtHandshake();

    HV_Trace(5, "start pci link.\n");

    PCI_PARAM_SLAVE cPciParam;
    cPciParam.pCfgCamParam = &cModuleParams.cCamCfgParam;
    cPciParam.pVideoGetter = pVideoGetter;
    cPciParam.pOuterControler = pOuterControl;
    cPciParam.pAutoLinkParam = &cModuleParams.cResultSenderParam.cAutoLinkParam;
    cPciParam.pTrafficCfgParam = &cModuleParams.cTrackerCfgParam.cTrafficLight;
    // ����PCILink
    if (S_OK != StartPciLink(&cPciDataRecv, cPciParam))
    {
        HV_Trace(5, "CPciDataRecvThreadSlave Start is failed!\n");
        HV_Exit(HEC_FAIL, "CPciDataRecvThreadSlave Start is failed!");
    }

#endif
    // �����������
    pVideoGetter->SetCamCfgParam(&cModuleParams.cCamCfgParam);

    // ����ͼƬ��ת��ʶ
    pVideoGetter->SetImageEddyType(cModuleParams.cTrackerCfgParam.iEddyType);

    pEventChecker->SetResultSender(pResultSender);

    // �����������������ƶ���
    g_pEPolice = new CEPolice();
    g_pEPolice->SetModuleParams(cModuleParams);
    g_pEPolice->SetFlashLampDiff(cModuleParams.cSignalMatchParam.iFlashLampDiff);

    // ���ع������
    g_pEPolice->MountVideoRecoger(pVideoRecoger);
    g_pEPolice->MountVideoGetter(pVideoGetter);
    g_pEPolice->MountResultSender(pResultSender);
    g_pEPolice->MountEventChecker(pEventChecker);
    g_pEPolice->MountSignalMatch(pOuterControl);
    g_pEPolice->MountLightTypeSaver(&cCamLightTypeSaver);
#ifdef SINGLE_BOARD_PLATFORM
    g_pEPolice->MountVideoSender(&videoLinkCtrl);
#else
    // DataCtrl���� ע����Ҫ��DataCtrl����ǰ
    if (S_OK != DataCtrlHandshake(&cPciDataRecv))
    {
        HV_Trace(5, "DataCtrlHandshake failed!\n");
        HV_Exit(HEC_FAIL|HEC_RESET_DEV, "DataCtrlHandshake failed!");
    }
#endif

    WdtHandshake();

    // ���и��������
    if (pOuterControl != NULL)
    {
        if (S_OK != pOuterControl->InitOuterControler())
        {
            HV_Trace(5, "SignalMatch init failed!\n");
            HV_Exit(HEC_FAIL, "SignalMatch init failed!");
        }
    }
    if (S_OK != pVideoGetter->Play(NULL))
    {
        HV_Trace(5, "VideoGetter play failed!\n");
        HV_Exit(HEC_FAIL, "VideoGetter play failed!");
    }

    // ע�⣺�ܿ��߳�һ��Ҫ���������
    if (S_OK != g_pEPolice->Start(NULL))
    {
        HV_Trace(5, "DataCtrl start failed!\n");
        HV_Exit(HEC_FAIL, "DataCtrl start failed!");
    }

#ifdef SINGLE_BOARD_PLATFORM
    // ��ʱ�����ؽ���ARPPing
    CPingThread cPingThread;
    cPingThread.SetIPAddress(cModuleParams.cTcpipCfgParam_1.szIp, cModuleParams.cTcpipCfgParam_1.szNetmask, cModuleParams.cTcpipCfgParam_1.szGateway);
    cPingThread.Start(NULL);

    // �����������ת���߳�
    if (S_OK == StartCamTransmit(
                cModuleParams.cCamCfgParam.iCamType,
                cModuleParams.cCamCfgParam.szIP,
                cModuleParams.cTcpipCfgParam_1,
                cModuleParams.cTcpipCfgParam_2)
       )
    {
        HV_Trace(5, "StartCamTransmit...\n");
    }
#endif

    WdtHandshake();

#if !defined(SINGLE_BOARD_PLATFORM) && defined(IPT_IN_MASTER)
    HRESULT hr = E_FAIL;
    if (cModuleParams.cSignalMatchParam.fEnableIPT)
    {
        HV_Trace(5, "start send pci data.\n");
        while (true)
        {
            HRESULT hrIpt = E_FAIL;
            int len = sizeof(hrIpt);
            HV_Trace(5, "start send pci data %d.\n", cModuleParams.cSignalMatchParam.nDeviceLen);
            hr = g_cHvPciLinkApi.SendData(
                     PCILINK_PCIIPTPARAM,
                     cModuleParams.cSignalMatchParam.bDeviceParam,
                     cModuleParams.cSignalMatchParam.nDeviceLen,
                     &hrIpt,
                     &len
                 );

            if ((hr != S_OK) || (hrIpt != S_OK))
            {
                HV_Trace(5, "init IPT failed, retry.\n");
                HV_Sleep(100);
            }
            else
            {
                break;
            }
        }
        HV_Trace(5, "init IPT ok.\n");
    }
#endif

#ifndef SINGLE_BOARD_PLATFORM
    //���ͽ���������ø���
    g_cHvPciLinkApi.SendData(PCILINK_RESULT_PARAM, &cModuleParams.cResultSenderParam, sizeof(cModuleParams.cResultSenderParam));
    //һ���ֱ������״�
    g_cHvPciLinkApi.SendData(PCILINK_OUTERDEVICE_TYPE, &cModuleParams.cSignalMatchParam.iRadarType, sizeof(int));
#endif

    WdtHandshake();
    if (fCanSetBuzzer)
    {
        SetBuzzerOn();
        HV_Sleep(200);
        SetBuzzerOff();
    }
    SetLan1LedOff();
    SetLan2LedOff();
    SetHddLedOff();
    SetStatLedOff();

    HV_Trace(5, "Running...\n");

    DWORD dwOutputFlag = 0;
    DWORD32 dwCurTick = 0;
    DWORD32 dwLastTick = GetSystemTick();
    SLAVE_CPU_STATUS cSlaveCpuStatus;

    while (true)
    {
#ifdef SINGLE_BOARD_PLATFORM
        if (!fIsIP1Correct && (GetSystemTick() - dwCurTick >= 5000))
        {
            HV_Trace(5, "LAN1��LAN2��IP��������Ϊͬһ���Σ�����ģ�\n");
            dwCurTick = GetSystemTick();
        }
#else
        dwCurTick = GetSystemTick();
        if (dwCurTick - g_pEPolice->GetLastResultTime() >= CPU_THROB_PERIOD)
        {
            GetSystemTime(&cSlaveCpuStatus.dwTimeLow, &cSlaveCpuStatus.dwTimeHigh);
            if (cSlaveCpuStatus.nStatusID == 0)
            {
                memset(cSlaveCpuStatus.szErrorText, 0, 32);
            }

            //����CPU����״̬
            if ( 0 == PciSendSlaveCpuState(&cSlaveCpuStatus) )
            {
                //����ʱ��
                g_pEPolice->SetLastResultTime(dwCurTick);
            }
            else
            {
                HV_Exit(HEC_FAIL|HEC_RESET_DEV, "Send CPU throb is failed!");
            }
        }
#endif

        if (GetSystemTick() - dwLastTick > 10000)
        {
            pResultSender->SendDevStateString(
                pVideoGetter->IsCamConnected(),
                g_pEPolice->GetFrameRate(),
                g_cHddOpThread.GetCurStatusCodeString()
            );
            dwLastTick = GetSystemTick();
        }

        HV_Sleep(500);

        // ��ظ�ģ������״̬
#ifdef SINGLE_BOARD_PLATFORM
        if ( S_OK != imageLinkCtrl.GetCurStatus(NULL, 0) )
        {
            PrintfErrorInfo(&cSlaveCpuStatus, "imageLinkCtrl is error!!!\n");
            HV_Exit(HEC_FAIL, "imageLinkCtrl is error!!!");
        }

        if (S_OK != recordLinkCtrl.GetCurStatus(NULL, 0) )
        {
            PrintfErrorInfo(&cSlaveCpuStatus, "recordLinkCtrl is error!!!\n");
            HV_Exit(HEC_FAIL, "recordLinkCtrl is error!!!");
        }

        if (S_OK != videoLinkCtrl.GetCurStatus(NULL, 0) )
        {
            PrintfErrorInfo(&cSlaveCpuStatus, "videoLinkCtrl is error!!!\n");
            HV_Exit(HEC_FAIL, "videoLinkCtrl is error!!!");
        }

        if (!g_fCopyrightValid)
        {
            cResultSender.DisableRecordSend();
        }
        else
        {
            cResultSender.EnableRecordSend();
        }

        if (!g_nHddCheckStatus)
        {
            SetAllLightFlash();
        }
        else if (g_nHddCheckStatus == 2)
        {
            SetAllLightOn();
        }
        else
        {
            SetAllLightOff();
        }
#endif
        int iErrCode=0;
        if ( !g_pEPolice->ThreadIsOk(&iErrCode) )
        {
            if (1 == iErrCode)
            {
                PrintfErrorInfo(&cSlaveCpuStatus, "VideoRecoger is error!!!\n");
                HV_Exit(HEC_FAIL, "VideoRecoger is error!!!");
            }
            else if (2 == iErrCode)
            {
                PrintfErrorInfo(&cSlaveCpuStatus, "VideoGetter is error!!!\n");
                HV_Exit(HEC_FAIL, "VideoGetter is error!!!");
            }
            else if (3 == iErrCode)
            {
                PrintfErrorInfo(&cSlaveCpuStatus, "SignalMatch is error!!!\n");
                HV_Exit(HEC_FAIL, "SignalMatch is error!!!");
            }
            else if (6 == iErrCode)
            {
                PrintfErrorInfo(&cSlaveCpuStatus, "Carleft thread is error!!!\n");
                HV_Exit(HEC_FAIL, "Carleft thread is error!!!");
            }
            continue;
        }

        if ( S_OK != pResultSender->GetCurStatus(NULL,0) )
        {
            PrintfErrorInfo(&cSlaveCpuStatus, "ResultSender is error!!!\n");
            HV_Exit(HEC_FAIL, "ResultSender is error!!!");
            continue;
        }
#ifndef SINGLE_BOARD_PLATFORM
        if ( S_OK != cPciDataRecv.GetCurStatus(NULL,0) )
        {
            HV_Exit(HEC_FAIL, "PciDataRecvThread is error!!!");
            continue;
        }
#endif
        WdtHandshake();
        WorkLedLight();
        //ͨ��д�ļ�֪ͨ�ػ����̵�ǰ�������ɻ���
        struct mytv
        {
            int iTick;
            int fMaster;
        }tmp;
        tmp.iTick = GetSystemTick();
#ifndef SINGLE_BOARD_PLATFORM
        tmp.fMaster = 0;
#else
        tmp.fMaster = 1;
#endif
        WriteDataToFile("/.running", (unsigned char *)&tmp, sizeof(tmp));

        if (((dwOutputFlag++) & 3) == 0)
        {
            static int s_nMemStatusShowFlag = 0;
            if ( s_nMemStatusShowFlag++ > 10 )
            {
                s_nMemStatusShowFlag = 0;
                ShareMemPoolStatusShow();
            }

            printf(".");
            // Linux��׼������л��棬����Ҫǿ��ˢ��
            fflush(stdout);
        }

        cSlaveCpuStatus.nStatusID = 0;
    }
    g_cHvDspLinkApi.Uninit();
#ifndef SINGLE_BOARD_PLATFORM
    g_cHvPciLinkApi.Uninit();
#endif

    HV_Exit(HEC_FAIL, "MainExit");
}

void PrintfErrorInfo(SLAVE_CPU_STATUS *pSlaveCapStatus, char *pszErrInfo)
{
    pSlaveCapStatus->nStatusID = -1;
    strcpy(pSlaveCapStatus->szErrorText, pszErrInfo);
    HV_Trace(5, pSlaveCapStatus->szErrorText);
}

HRESULT InitMemBlocksEx(int iCamType, int iEddyType)
{
    SHARE_MEM_MODE t;

#ifdef SINGLE_BOARD_PLATFORM
    t = LPRAPP_EPOLICE_S;
#else
    if ( 0 == iCamType ) // һ�廯���
    {
        if ( 0 == iEddyType )
        {
#ifndef _CAMERA_PIXEL_500W_
            t = LPRAPP_NORMAL;
#else
    t = LPRAPP_EPOLICE_500W;
#endif
        }
        else
        {
            t = LPRAPP_EDDY;
        }
    }
    else
    {
        t = LPRAPP_OTHER;
    }
#endif

    return InitDspLinkMemBlocks(t);
}

HRESULT SetDspLinkEddyType(int nEddyType)
{
    IMAGE_EDDYTYPE_PARAM cEddyType;
    cEddyType.nEddyType = nEddyType;
    return g_cHvDspLinkApi.SendImageEddyTypeCmd(&cEddyType);
}

HRESULT InitCaptureThread(
    CImgCapturer **ppImgCapturer,
    ICamTrigger **ppCamTrigger,
    COuterControlImpl *pOuterControl,
    ModuleParams *pModuleParams,
    int iCameraType
)
{
    static CAM_CFG_PARAM cCamTriggerParam;
    if (iCameraType != 4)
    {
        (*ppImgCapturer) = new CImgCapturer();
        (*ppImgCapturer)->SetCapCamParam(
            pModuleParams->cResultSenderParam.iEddyType,
            &pModuleParams->cResultSenderParam.cCapCamParam,
            pOuterControl
        );
        (*ppImgCapturer)->Play(NULL);
    }

    if (iCameraType == 4)
    {
        (*ppCamTrigger) = new CHVTrigger();
    }
    else
    {
        (*ppCamTrigger) = new CCamTrigger();
    }
    memcpy(&cCamTriggerParam, &(pModuleParams->cCamCfgParam), sizeof(pModuleParams->cCamCfgParam));
    strcpy(cCamTriggerParam.szIP, pModuleParams->cResultSenderParam.cCapCamParam.rgszRoadIP[0]);
    ((CCamTrigger*)(*ppCamTrigger))->SetCamTriggerParam(&cCamTriggerParam);
    ((CCamTrigger*)(*ppCamTrigger))->Start(NULL);

    return S_OK;
}
