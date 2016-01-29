// ���ļ����������WINDOWS-936��ʽ
#include "hvthreadbase.h"
#include "LoadParam.h"
#include "hvtarget_ARM.h"
#include "IPSearchThread.h"
#include "HvUtilsClass.h"
#include "CameraCmdProcess.h"
#include "CameraCmdLink.h"
#include "CameraRecordLink.h"
#include "DataCtrl.h"
#include "DspLinkMemBlocks.h"
#include "HvParamIO.h"
#include "CamTransmit.h"

HV_SEM_HANDLE g_hSemCameraDataCount;
HV_SEM_HANDLE g_hSemSwDev;
HV_SEM_HANDLE g_hSemEDMA;
HV_SEM_HANDLE g_hSemEEPROM;

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

#ifdef PC_TEST

int main(int argc, char** argv)
{
    CUserExitThread e;
    e.Start(NULL);

    CSafeSaverDm6467Impl* pcSafeSaver = new CSafeSaverDm6467Impl();

    CHvBoxHolder cHvBoxHolder;
    cHvBoxHolder.Init(NULL, pcSafeSaver);

    HV_Sleep(200);
    HV_Trace(5, "Running...\n");

    while (true)
    {
        HV_Sleep(5000);
        cHvBoxHolder.ShowStatus();
        printf(".");
        fflush(stdout);
    }

    return 0;
}

#else

/* HvSystemInit.cpp */
extern void LinuxSystemInit(void);

// ע����֧�ֵ���ģʽ
// ����򿪵���ģʽ����ꡰSINGLE_BOARD_PLATFORM�����ܱ���ͨ��!!!
int main(int argc, char** argv)
{
    HV_Trace(5, "Begin...\n");
    HV_Trace(5, "%s %s\n", __TIME__, __DATE__);

    LinuxSystemInit();

    CUserExitThread e;
    e.Start(NULL);

    // ʹ�ܿ��Ź���
    // ע��ʹ��֮ǰ����ι������ֹ�������򿪿��Ź���ͬʱ����ι����ʱ�������豸��λ�������������
    WdtHandshake();
    CPLD_Write(CPLD_WTD_ENABLE, 1);
    WdtHandshake();

    // �����ģ�����
    ModuleParams cModuleParams;
    if ( S_OK != LoadModuleParam(cModuleParams) )
    {
        HV_Trace(5, "LoadModuleParam failed!\n");
        HV_Exit(HEC_FAIL|HEC_RESET_DEV, "LoadModuleParam failed!");
    }

    WdtHandshake();

    // �ж��Ƿ�ָ���������
    if (IsRecoverFactorySettings())
    {
        RecoverFactorySettings();
        HV_Exit(HEC_SUCC, "RecoverFactorySettings!");
    }

    WdtHandshake();

    // ��ʼ�������ڴ�
    if (S_OK != InitDspLinkMemBlocks(LPRAPP_SINGLE_BOARD_PLATFORM))
    {
        HV_Trace(5, "Memory blocks initialize failed!\n");
        HV_Exit(HEC_RESET_DEV|HEC_FAIL, "Memory blocks initialize failed!");
    }

    WdtHandshake();

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

        // �����豸IP
        SetTcpipAddr_2(&cModuleParams.cTcpipCfgParam_2);
    }
    else
    {
        // ���в�������ڣ�ֻ����LAN1��Ϊͨ��Ϣ�˿�
        fIsIP1Correct = TRUE;
        SetTcpipAddr_1(&cModuleParams.cTcpipCfgParam_2);
    }

    if (S_OK == StartCamTransmit(0,
                                 "",
                                 cModuleParams.cTcpipCfgParam_1,
                                 cModuleParams.cTcpipCfgParam_2))
    {
        HV_Trace(5, "StartCamTransmit...\n");
    }

    WdtHandshake();
    CSafeSaverDm6467Impl* pcSafeSaver = NULL;
    if (1 == cModuleParams.cResultSenderParam.fInitHdd)
    {
        SetAllLightOn();
        g_cHddOpThread.SetHddOpType(HOT_FULL_INIT);
        g_cHddOpThread.Start(NULL);
        HV_Trace(5, "Hdd init...\n");
    }
    if (cModuleParams.cResultSenderParam.fIsSafeSaver)
    {
        if (!cModuleParams.cResultSenderParam.fInitHdd)
        {
            SetAllLightOn();
            g_cHddOpThread.SetHddOpType(HOT_INIT);
            g_cHddOpThread.Start(NULL);
            HV_Trace(5, "Hdd check...\n");
        }
        WdtHandshake();
        while (1 != g_nHddCheckStatus)
        {
            HV_Trace(5, "�ȴ�Ӳ�̳�ʼ�����...\n");
            HV_Sleep(1000);
            WdtHandshake();
        }
        pcSafeSaver = new CSafeSaverDm6467Impl();
        SSafeSaveParam sParam;
        sParam.fCoverCtrl = true;
        sParam.fSaveRecord = true;
        pcSafeSaver->Init(&sParam);
    }

    SEND_RECORD_PARAM cParam;
    cParam.pcSafeSaver = pcSafeSaver;
    CCameraRecordLinkCtrl cCameraRecordLinkCtrl;
    cCameraRecordLinkCtrl.SetParam(&cParam);
    if ( S_OK != cCameraRecordLinkCtrl.Start(NULL) )
    {
        HV_Trace(5, "CCameraRecordLinkCtrl Start failed!\n");
        HV_Exit(HEC_FAIL, "CCameraRecordLinkCtrl Start failed!");
    }

    WdtHandshake();
    ICameraCmdProcess* pICameraCmdProcess = NULL;
    if ( S_OK != CCameraCmdProcess::CreateICameraCmdProcess(&pICameraCmdProcess) )
    {
        HV_Trace(5, "CreateICameraCmdProcess failed!\n");
        HV_Exit(HEC_FAIL, "CreateICameraCmdProcess failed!");
    }

    CCameraCmdLinkCtrl cCameraCmdLinkCtrl(pICameraCmdProcess);
    if ( S_OK != cCameraCmdLinkCtrl.Start(NULL) )
    {
        HV_Trace(5, "CCameraCmdLinkCtrl Start failed!\n");
        HV_Exit(HEC_FAIL, "CCameraCmdLinkCtrl Start failed!");
    }

    WdtHandshake();
    CHvBoxHolder cHvBoxHolder;
    cHvBoxHolder.Init(&cModuleParams, pcSafeSaver);

    HV_Sleep(300);

    // �����豸������Ӧ�߳�
    CIPSearchThread cDevSearch;
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

    WdtHandshake();

    /*
    SetBuzzerOn();
    HV_Sleep(200);
    SetBuzzerOff();
    */

    SetLan1LedOff();
    SetLan2LedOff();
    SetHddLedOff();
    SetStatLedOff();

    HV_Trace(5, "Running...\n");

    // ��ʱ�����ؽ���ARPPing
    CPingThread cPingThread;
    cPingThread.SetIPAddress(cModuleParams.cTcpipCfgParam_1.szIp, cModuleParams.cTcpipCfgParam_1.szNetmask, cModuleParams.cTcpipCfgParam_1.szGateway);
    cPingThread.Start(NULL);

    WdtHandshake();

    DWORD32 dwIPTick = 0;
    DWORD32 dwBoxTick = 0;
    DWORD32 dwDotTick = 0;
    DWORD32 dwMemTick = GetSystemTick();
    while (1)
    {
        if (!fIsIP1Correct && (GetSystemTick() - dwIPTick >= 5000))
        {
            HV_Trace(5, "LAN1��LAN2��IP��������Ϊͬһ���Σ�����ģ�\n");
            dwIPTick = GetSystemTick();
        }

        HV_Sleep(500);

        if (GetSystemTick() - dwBoxTick >= 10000)
        {
            cHvBoxHolder.ShowStatus();
            dwBoxTick = GetSystemTick();
        }

        if (GetSystemTick() - dwMemTick >= 20000)
        {
            ShareMemPoolStatusShow();
            dwMemTick = GetSystemTick();
        }

        WdtHandshake();
        WorkLedLight();

        if (GetSystemTick() - dwDotTick >= 1000)
        {
            HV_Trace(5,".");
            fflush(stdout);
            dwDotTick = GetSystemTick();
        }
    }

    HV_Exit(HEC_FAIL, "MainExit");
}
#endif
