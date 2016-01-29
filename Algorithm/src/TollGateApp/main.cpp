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

CTollGate *g_pTollGate = NULL;

using namespace HiVideo;

#ifdef _USE_SEED_DEBUG_
/* DSPLink.c */
extern int g_fDebugStart;
#endif

/* HvDspLinkApi.cpp */
extern HRESULT InitDspLink(int argc, char** argv);

void PrintfErrorInfo(SLAVE_CPU_STATUS *pSlaveCapStatus, char *pszErrInfo);
//ƽ�д��� ����β�
HRESULT InitMemBlocksEx(int iCamType,int iEddy);

HRESULT SetDspLinkEddyType(int nEddyType);

HRESULT InitCaptureThread(
    CImgCapturer **ppImgCapturer,
    ICamTrigger **ppCamTrigger,
    COuterControlImpl *pOuterControl,
    ModuleParams *pModuleParams,
    int iCameraType   //ƽ�д�������β�
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

    WdtHandshake();

    int nLastLightType = (int)LIGHT_TYPE_COUNT;
    int nLastPulseLevel = 0;
    int nLastCplStatus = 0;
    CCamLightTypeSaver cCamLightTypeSaver;
    cCamLightTypeSaver.GetLastLightType(nLastLightType, nLastPulseLevel , nLastCplStatus);
    if(cModuleParams.cTrackerCfgParam.nCtrlCpl == 0)
    {
        nLastCplStatus = -1;
    }
    else
    {
        if(nLastCplStatus == -1)
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

    // ������Ͷ���
    IResultSender *pResultSender = NULL;
    // ��Ƶ����ȡ����
    IImgGatherer* pVideoGetter = NULL;
    // �����ź�ƥ�����
    COuterControlImpl *pOuterControl = NULL;
    // ץ�����ͼƬ���ն���
    CImgCapturer *pImgCapturer = NULL;
    // ����ץ���������
    ICamTrigger *pCamTrigger = NULL;
    // �Ƿ���Ҫ����ץ��
    BOOL fNeedCapture = FALSE;
    // �Ƿ��������������
    BOOL fCanSetBuzzer = FALSE;

    // ��ʼ���ź�ƥ��
    pOuterControl = new COuterControlImpl(cModuleParams.cSignalMatchParam);

    // ��ʹ���ź�ƥ�䲢��ץ�����������Ϊ0������²�����ץ���߳�
    if (cModuleParams.cSignalMatchParam.emOutPutPlateMode != PLATE_OUTPUT_AUTO
            && cModuleParams.cResultSenderParam.cCapCamParam.nCamNum != 0)
    {
        fNeedCapture = TRUE;
    }

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
    CCameraVideoLinkCtrl  videoLinkCtrl;

    AUTO_LINK_PARAM* pAutoLinkParam = &cModuleParams.cResultSenderParam.cAutoLinkParam;

    recordLinkCtrl.SetAutoLink(
        pAutoLinkParam->fAutoLinkEnable,
        pAutoLinkParam->szAutoLinkIP,
        pAutoLinkParam->iAutoLinkPort
    );

    recordLinkCtrl.EnableMTUSet(&cModuleParams.cResultSenderParam.fMTUSetEnable);
    recordLinkCtrl.SetCameraType(cModuleParams.cCamCfgParam.iCamType);

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
        ((CVideoGetter_Nvc*)pVideoGetter)->MountSignalMatch(pOuterControl);
        ((CVideoGetter_Nvc*)pVideoGetter)->SetCapCamCfgParam(&(cModuleParams.cResultSenderParam.cCapCamParam));
        HV_Trace(5, "NVCЭ��\n");
        break;
    }
    case 3: //�㲥Э��
    {
        pVideoGetter = new CVideoGetter_Broadcast();
        HV_Trace(5, "�㲥Э��\n");
        break;
    }
    case 2://����Э��
    default:
    {
        pVideoGetter = new CVideoGetter_Camyu();
        ((CVideoGetter_Camyu*)pVideoGetter)->MountSignalMatch(pOuterControl);
        ((CVideoGetter_Camyu*)pVideoGetter)->SetCapCamCfgParam(&(cModuleParams.cResultSenderParam.cCapCamParam));
        HV_Trace(5, "����Э��\n");
        break;
    }
    }

    pVideoGetter->MountTransmiter(&g_pCamTransmit);

    WdtHandshake();

    // ����ץ���߳�
    if (fNeedCapture)
    {
        InitCaptureThread(
            &pImgCapturer,
            &pCamTrigger,
            pOuterControl,
            &cModuleParams,
            cModuleParams.cCamCfgParam.iCamType
        );
    }

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
        ((CVideoGetter_VPIF*)pVideoGetter)->MountSignalMatch(fNeedCapture ? pOuterControl : NULL);
        //VPIF��Ҳ�̳��ڴ�����
        if (fNeedCapture)
        {
            pCamTrigger = (CVideoGetter_VPIF *)pVideoGetter;
        }
    }
    else
    {
        pVideoGetter = new CVideoGetter_Pci();
    }

    pResultSender = new CPciResultSender();
    ((CPciResultSender*)pResultSender)->Init(&cModuleParams.cResultSenderParam);

    WdtHandshake();

    HV_Trace(5, "start pci link.\n");
    // ����PCILink
    PCI_PARAM_SLAVE cPciParam;
    cPciParam.pCfgCamParam = &cModuleParams.cCamCfgParam;
    cPciParam.pVideoGetter = pVideoGetter;
    cPciParam.pOuterControler = pOuterControl;
    cPciParam.pAutoLinkParam = &cModuleParams.cResultSenderParam.cAutoLinkParam;
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

    // �����շ�վ���������ƶ���
    g_pTollGate = new CTollGate();
    g_pTollGate->SetModuleParams(cModuleParams);

    // ���ع������
    g_pTollGate->MountVideoRecoger(pVideoRecoger);
    g_pTollGate->MountVideoGetter(pVideoGetter);
    g_pTollGate->MountResultSender(pResultSender);
    g_pTollGate->MountSignalMatch(pOuterControl);
    g_pTollGate->MountImgCapturer(pImgCapturer);
    g_pTollGate->MountTriggerCam(pCamTrigger);
    g_pTollGate->MountLightTypeSaver(&cCamLightTypeSaver);
#ifdef SINGLE_BOARD_PLATFORM
    g_pTollGate->MountVideoSender(&videoLinkCtrl);
#else
    // TODO: ��������Ҫ��ʼ����ƼӺ죬��ʱ̫�ã���˴Ӷ���Ҫsleep
    // ������ʱ������������Ҫ����ͬ�����������DataCtrl
    // huanggr 2012-04-28
    HV_Sleep(8000);
#endif

    WdtHandshake();

    // ���и��������
    if (pOuterControl != NULL)
    {
        pOuterControl->MountVideoRecoger(pVideoRecoger);
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
    if ( S_OK != g_pTollGate->Start(NULL) )
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
        if (dwCurTick - g_pTollGate->GetLastResultTime() >= CPU_THROB_PERIOD)
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
                g_pTollGate->SetLastResultTime(dwCurTick);
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
                g_pTollGate->GetFrameRate(),
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
        if ( !g_pTollGate->ThreadIsOk(&iErrCode) )
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
            else if (4 == iErrCode)
            {
                PrintfErrorInfo(&cSlaveCpuStatus, "CamTrigger is error!!!\n");
                HV_Exit(HEC_FAIL, "CamTrigger is error!!!");
            }
            else if (5 == iErrCode)
            {
                PrintfErrorInfo(&cSlaveCpuStatus, "ImgCapturer is error!!!\n");
                HV_Exit(HEC_FAIL, "ImgCapturer is error!!!");
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
        //ͨ��д�ļ�֪ͨ�ػ����̵�ǰ�������ɻ���
        int iTime = GetSystemTick();
        WriteDataToFile("/.running", (unsigned char *)&iTime, sizeof(iTime));

        WdtHandshake();
        WorkLedLight();
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

//ƽ�д��� ����β�
HRESULT InitMemBlocksEx(int iCamType, int iEddyType)
{
    SHARE_MEM_MODE t;

#ifdef SINGLE_BOARD_PLATFORM
    t = LPRAPP_CAP_FACE_S;
#else
    if ( 0 == iCamType ) // һ�廯���
    {
        t = LPRAPP_NORMAL;
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
    int iCameraType  //ƽ�д��� ����β�
)
{
    static CAM_CFG_PARAM cCamTriggerParam;
    (*ppImgCapturer) = new CImgCapturer();
    (*ppImgCapturer)->SetCapCamParam(
        pModuleParams->cResultSenderParam.iEddyType,
        &pModuleParams->cResultSenderParam.cCapCamParam,
        pOuterControl
    );
    (*ppImgCapturer)->Play(NULL);

    (*ppCamTrigger) = new CCamTrigger();
    memcpy(&cCamTriggerParam, &(pModuleParams->cCamCfgParam), sizeof(pModuleParams->cCamCfgParam));
    strcpy(cCamTriggerParam.szIP, pModuleParams->cResultSenderParam.cCapCamParam.rgszRoadIP[0]);
    ((CCamTrigger*)(*ppCamTrigger))->SetCamTriggerParam(&cCamTriggerParam);
    ((CCamTrigger*)(*ppCamTrigger))->Start(NULL);

    return S_OK;
}
