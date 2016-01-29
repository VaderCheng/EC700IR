// ���ļ��������ΪWINDOWS-936��ʽ
#include "LoadParam.h"
#include "HvParamIO.h"
#include "misc.h"
#include "platerecogparam.h"
#include "OuterControlImpl_Linux.h"
#include "hvtarget_ARM.h"
#include "ControlFunc.h"
#include <math.h>

//����ģʽ�б�����Ҫ��platerecogparam.h��һ��
char *g_pszPartWorkModeList[] =
{
    "�շ�վ"
};
#ifdef SINGLE_BOARD_PLATFORM
extern const char *PSZ_DSP_BUILD_NO;
extern const char *DSP_BUILD_DATE;
extern const char *DSP_BUILD_TIME;
#endif
CEprFileStorage g_cFileStorage(PARAM_START_ADDR, PARAM_SIZE);
CParamStore g_cParamStore;

HRESULT LoadModuleParam(ModuleParams& cModuleParams)
{
    if (FAILED(g_cFileStorage.Initialize("")))
    {
        HV_Trace(5, "Open param file failed, can't save setting!!!\n");
    }

    g_cParamStore.Initialize(&g_cFileStorage, true);

#ifdef SINGLE_BOARD_PLATFORM
    // ��ȡUBOOT�µ�IP�������Լ�����
    SetIpFromEEPROM();
#endif

    LoadTcpipParam_1(&g_cParamStore, cModuleParams.cTcpipCfgParam_1);
    LoadTcpipParam_2(&g_cParamStore, cModuleParams.cTcpipCfgParam_2);
    //����ģʽһ��Ҫ���ȼ��أ��������Ĳ����п��ܼ��ش�
    LoadWorkModeParam(&g_cParamStore, cModuleParams);
    LoadBuildNoParam();
    LoadCamCfgParam(&g_cParamStore, cModuleParams.cCamCfgParam);
    LoadTrackerCfgParam(&g_cParamStore, cModuleParams.cTrackerCfgParam, cModuleParams);
    LoadResultSenderParam(&g_cParamStore, cModuleParams.cResultSenderParam);
    LoadSignalMatchParam(&g_cParamStore, cModuleParams.cSignalMatchParam);

    // ��ģ��֮���������
    cModuleParams.cTrackerCfgParam.nCarArriveTrig = cModuleParams.cCamCfgParam.iDynamicTriggerEnable;
    cModuleParams.cResultSenderParam.iEddyType = cModuleParams.cTrackerCfgParam.iEddyType;
    cModuleParams.cTrackerCfgParam.iRoadNumberBegin = cModuleParams.cSignalMatchParam.iRoadNumberBegin;
    cModuleParams.cTrackerCfgParam.iStartRoadNum = cModuleParams.cSignalMatchParam.iStartRoadNum;
    cModuleParams.cSignalMatchParam.fEnableFlashLight = cModuleParams.cTrackerCfgParam.fEnableFlashLight;
    sscanf(
        cModuleParams.cSignalMatchParam.szRecogArea,
        "[%d,%d,%d,%d],%d,%d",
        &cModuleParams.cTrackerCfgParam.cRecogSnapArea.DetectorAreaLeft,
        &cModuleParams.cTrackerCfgParam.cRecogSnapArea.DetectorAreaTop,
        &cModuleParams.cTrackerCfgParam.cRecogSnapArea.DetectorAreaRight,
        &cModuleParams.cTrackerCfgParam.cRecogSnapArea.DetectorAreaBottom,
        &cModuleParams.cTrackerCfgParam.cRecogSnapArea.nDetectorMinScaleNum,
        &cModuleParams.cTrackerCfgParam.cRecogSnapArea.nDetectorMaxScaleNum
    );


    g_cParamStore.Save(HvCore::PSM_SIMPLE);

    return S_OK;
}

typedef struct _HV_PARAM_TYPE_INT
{
    CHvString strSection;
    CHvString strKey;
    INT* piVal;
    INT iDefault;
    INT iMin;
    INT iMax;
    CHvString strChName;
    CHvString strComment;
    BYTE8 nRank;

} HV_PARAM_TYPE_INT, *PHV_PARAM_TYPE_INT;

//--------------------------------------------------------------------------------------

int LoadBuildNoParam()
{
#ifdef SINGLE_BOARD_PLATFORM
    char szTemp[255];
    strcpy(szTemp, g_pszPartWorkModeList[0]);
    g_cParamStore.GetString(
        "\\System\\Build", "WorkMode",
        szTemp, sizeof(szTemp),
        "��ǰ����ģʽ", "", CUSTOM_LEVEL
    );
    g_cParamStore.SetString("\\System\\Build", "WorkMode", g_pszPartWorkModeList[0]);

    sprintf(szTemp, "%s %s %s", PSZ_DSP_BUILD_NO, DSP_BUILD_DATE, DSP_BUILD_TIME);
    g_cParamStore.GetString(
        "\\System\\Build", "No.",
        szTemp, sizeof(szTemp),
        "����汾��", "", CUSTOM_LEVEL
    );
    sprintf(szTemp, "%s %s %s", PSZ_DSP_BUILD_NO, DSP_BUILD_DATE, DSP_BUILD_TIME);
    g_cParamStore.SetString("\\System\\Build", "No.", szTemp);

    strcpy(szTemp, DSP_BUILD_DATE);
    g_cParamStore.GetString(
        "\\System\\Build", "Date",
        szTemp, sizeof(szTemp),
        "����汾����", "", CUSTOM_LEVEL
    );
    g_cParamStore.SetString("\\System\\Build", "Date", DSP_BUILD_DATE);

    strcpy(szTemp, DSP_BUILD_TIME);
    g_cParamStore.GetString(
        "\\System\\Build", "Time",
        szTemp, sizeof(szTemp),
        "����汾ʱ��", "", CUSTOM_LEVEL
    );
    g_cParamStore.SetString("\\System\\Build", "Time", DSP_BUILD_TIME);
#endif
}

int LoadWorkModeParam(
    CParamStore* pcParamStore,
    ModuleParams& cModuleParams
)
{
    DWORD32 dwModeCount = 0;

    GetPlateRecogParamCount(&dwModeCount);

    HvParamReadIntWithWrite(
        pcParamStore,
        "\\Tracker", "PlateRecogMode",
        &cModuleParams.nWorkModeIndex, cModuleParams.nWorkModeIndex,
        0, dwModeCount,
        "����ģʽ����", "", INNER_LEVEL
    );

    char szModeName[128] = {0};
    //����cModuleParams.nWorkModeIndex��ŵ��Ƿ���ǰ��ģʽ����
    //����轫������ת��Ϊ���ͺ������
    GetRecogParamNameOnIndex(cModuleParams.nWorkModeIndex, szModeName);
    //����Ƿ�����Чģʽ������������л�����Чģʽ

    if (GetRecogParamIndexOnNamePart(szModeName) == -1)
    {
        //�ָ�Ĭ�ϲ���
        LoadDefaultParam(pcParamStore, cModuleParams.cTcpipCfgParam_1, cModuleParams.cTcpipCfgParam_2);
        //���������ù���ģʽ
        GetRecogParamNameOnIndexPart(0, szModeName);
        cModuleParams.nWorkModeIndex = GetRecogParamIndexOnName(szModeName);
        pcParamStore->SetInt("\\Tracker", "PlateRecogMode", cModuleParams.nWorkModeIndex);
        pcParamStore->Save();
        HV_Trace(5, "%s\n", szModeName);
    }
    else
    {
        HV_Trace(5, "Load Mode OK...\n%s\n", szModeName);
    }
    return 0;
}

int LoadDefaultParam(
    CParamStore* pcParamStore,
    TcpipParam& cTcpipCfgParam1,
    TcpipParam& cTcpipCfgParam2
)
{
#ifdef SINGLE_BOARD_PLATFORM
    pcParamStore->GetString(
        "\\System\\CamLan", "CommIPAddr",
        cTcpipCfgParam1.szIp, 31
    );
    pcParamStore->GetString(
        "\\System\\CamLan", "CommMask",
        cTcpipCfgParam1.szNetmask, 31
    );
    pcParamStore->GetString(
        "\\System\\CamLan", "CommGateway",
        cTcpipCfgParam1.szGateway, 31
    );
    pcParamStore->GetString(
        "\\System\\TcpipCfg", "CommIPAddr",
        cTcpipCfgParam2.szIp, 31
    );
    pcParamStore->GetString(
        "\\System\\TcpipCfg", "CommMask",
        cTcpipCfgParam2.szNetmask, 31
    );
    pcParamStore->GetString(
        "\\System\\TcpipCfg", "CommGateway",
        cTcpipCfgParam2.szGateway, 31
    );
#endif

    pcParamStore->Clear();

#ifdef SINGLE_BOARD_PLATFORM
    LoadTcpipParam_1(pcParamStore, cTcpipCfgParam1);
    LoadTcpipParam_2(pcParamStore, cTcpipCfgParam2);
#endif

    pcParamStore->Save();

    HV_Trace(5, "Param Load Default...\n");
}

static ResultSenderParam g_cResultSenderParam;

static HV_PARAM_TYPE_INT g_rgcResultSenderInt[] =
{
#ifdef SINGLE_BOARD_PLATFORM
    {"\\HvDsp\\Misc", "SafeSaveEnable", (int*)&g_cResultSenderParam.fIsSafeSaver, g_cResultSenderParam.fIsSafeSaver, 0, 1, "�Ƿ�������ȫ�洢", "0���������� 1������", CUSTOM_LEVEL},
#endif
    //{"\\HvDsp\\Misc", "DoReplace", (int*)&g_cResultSenderParam.cProcRule.fReplace, g_cResultSenderParam.cProcRule.fReplace, 0, 1, "�Ƿ��滻", "Ĭ��ֵ��0�� ��Сֵ��0�� ���ֵ��1\n0�����滻�� 1���滻", PROJECT_LEVEL},
    //{"\\HvDsp\\Misc", "DoLeach", (int*)&g_cResultSenderParam.cProcRule.fLeach, g_cResultSenderParam.cProcRule.fLeach, 0, 1, "�Ƿ����", "Ĭ��ֵ��0�� ��Сֵ��0�� ���ֵ��1\n0�������ˣ� 1������", PROJECT_LEVEL},
    //{"\\HvDsp\\Misc", "DoCompaty", (int*)&g_cResultSenderParam.cProcRule.fCompaty, g_cResultSenderParam.cProcRule.fCompaty, 0, 1, "�Ƿ�����ͨ�������", "Ĭ��ֵ��0�� ��Сֵ��0�� ���ֵ��1\n0�������ã� 1������", CUSTOM_LEVEL},
    {"\\HvDsp\\Misc", "LoopReplaceEnable", (int*)&g_cResultSenderParam.cProcRule.fLoopReplace, g_cResultSenderParam.cProcRule.fLoopReplace, 0, 1, "ѭ���滻", "0�������ã� 1������", CUSTOM_LEVEL},
};

static int g_iResultSenderIntArraySize = ARRSIZE(g_rgcResultSenderInt);

int LoadResultSenderParam(
    CParamStore* pcParamStore,
    ResultSenderParam& cResultSenderParam
)
{
    PHV_PARAM_TYPE_INT pParamPointInt = (PHV_PARAM_TYPE_INT)&g_rgcResultSenderInt;

    for ( int n = 0; n<g_iResultSenderIntArraySize; ++n )
    {
        HvParamReadIntWithWrite(
            pcParamStore,
            pParamPointInt[n].strSection.GetBuffer(),
            pParamPointInt[n].strKey.GetBuffer(),
            pParamPointInt[n].piVal, pParamPointInt[n].iDefault,
            pParamPointInt[n].iMin, pParamPointInt[n].iMax,
            pParamPointInt[n].strChName.GetBuffer(),
            pParamPointInt[n].strComment.GetBuffer(),
            pParamPointInt[n].nRank
        );
    }

    cResultSenderParam = g_cResultSenderParam;

    //attendtion by Shaorg: �����ַ��Ͳ������ܳ�ʼ��Ϊ�ջ�һ���ո񣬷��򽫵��²����޷����ġ�
    HvParamReadIntWithWrite(
        pcParamStore, "\\HvDsp\\Misc", "OutputAppendInfo",
        &cResultSenderParam.fOutputAppendInfo, cResultSenderParam.fOutputAppendInfo,
        0, 1,
        "���������Ϣ����", "",
        PROJECT_LEVEL);

    HvParamReadIntWithWrite(
        pcParamStore, "\\HvDsp\\Misc", "OutputObservedFrames",
        &cResultSenderParam.fOutputObservedFrames, cResultSenderParam.fOutputObservedFrames,
        0, 1,
        "�����Ч֡������", "",
        PROJECT_LEVEL);

    HvParamReadIntWithWrite(
        pcParamStore, "\\HvDsp\\Misc", "OutputCarArriveTime",
        &cResultSenderParam.fOutputCarArriveTime, cResultSenderParam.fOutputCarArriveTime,
        0, 1,
        "������ʱ�俪��", "",
        PROJECT_LEVEL);

    HvParamReadIntWithWrite(
        pcParamStore,
        "HvDsp\\Misc", "OutputFilterInfo",
        &cResultSenderParam.fOutputFilterInfo, cResultSenderParam.fOutputFilterInfo,
        0, 1,
        "���������Ϣ", "", CUSTOM_LEVEL
    );

    HvParamReadIntWithWrite(
        pcParamStore, "\\HvDsp\\Misc", "DrawRect",
        &cResultSenderParam.iDrawRect, cResultSenderParam.iDrawRect,
        0, 1,
        "����ͼ�񻭺��", "",
        PROJECT_LEVEL);

    HvParamReadIntWithWrite(
        pcParamStore, "\\HvDsp\\Misc", "VideoDisplayTime",
        &cResultSenderParam.iVideoDisplayTime, cResultSenderParam.iVideoDisplayTime,
        1, 1000000,
        "������Ƶʱ����(MS)", "",
        CUSTOM_LEVEL);

#ifdef SINGLE_BOARD_PLATFORM
    HvParamReadIntWithWrite(
        pcParamStore, "\\HvDsp\\Misc", "HDVideoEnable",
        &cResultSenderParam.iSaveVideo, cResultSenderParam.iSaveVideo,
        0, 1,
        "Ӳ��¼�񿪹�", "",
        CUSTOM_LEVEL);

    HvParamReadIntWithWrite(
        pcParamStore, "\\HvDsp\\Misc", "InitHdd",
        &cResultSenderParam.fInitHdd,
        cResultSenderParam.fInitHdd, 0, 2,
        "��ʼ��Ӳ��", "1-������ʽ��;2-�������;ע��һ������Ч",
        CUSTOM_LEVEL);
#endif

#ifndef SINGLE_BOARD_PLATFORM

#endif
    HvParamReadStringWithWrite(
        pcParamStore, "\\HvDsp\\FilterRule", "Compaty",
        cResultSenderParam.cProcRule.szCompatyRule,
        sizeof(cResultSenderParam.cProcRule.szCompatyRule),
        "ͨ�������", "", CUSTOM_LEVEL);

    HvParamReadStringWithWrite(
        pcParamStore, "\\HvDsp\\FilterRule", "Replace",
        cResultSenderParam.cProcRule.szReplaceRule,
        sizeof(cResultSenderParam.cProcRule.szReplaceRule),
        "�滻����", "", CUSTOM_LEVEL);

    HvParamReadStringWithWrite(
        pcParamStore, "\\HvDsp\\FilterRule", "Leach",
        cResultSenderParam.cProcRule.szLeachRule,
        sizeof(cResultSenderParam.cProcRule.szLeachRule),
        "���˹���", "", CUSTOM_LEVEL);

    HvParamReadStringWithWrite(
        pcParamStore, "\\HvDsp\\Identify", "StreetName",
        cResultSenderParam.szStreetName,
        sizeof(cResultSenderParam.szStreetName),
        "·������", "",
        CUSTOM_LEVEL);

    HvParamReadStringWithWrite(
        pcParamStore, "\\HvDsp\\Identify", "StreetDirection",
        cResultSenderParam.szStreetDirection,
        sizeof(cResultSenderParam.szStreetDirection),
        "·�ڷ���", "",
        CUSTOM_LEVEL);


    HvParamReadIntWithWrite(
        pcParamStore, "\\HvDsp\\VideoProc\\MainVideo", "OutputBestSnap",
        &cResultSenderParam.iBestSnapshotOutput, cResultSenderParam.iBestSnapshotOutput,
        0, 1,
        "����Ƶ��������ͼ���", "",
        CUSTOM_LEVEL);
    HvParamReadIntWithWrite(
        pcParamStore, "\\HvDsp\\VideoProc\\MainVideo", "OutputLastSnap",
        &cResultSenderParam.iLastSnapshotOutput, cResultSenderParam.iLastSnapshotOutput,
        0, 1,
        "����Ƶ����ͼ���", "",
        CUSTOM_LEVEL);
    //--
#ifdef SINGLE_BOARD_PLATFORM
    HvParamReadIntWithWrite(
        pcParamStore, "\\HvDsp\\VideoProc\\CaptureVideo", "CamType",
        &cResultSenderParam.cCapCamParam.nCamType, cResultSenderParam.cCapCamParam.nCamType,
        0, 0,
        "ץ���������(0-����Jϵ�����)", "",
        CUSTOM_LEVEL);
    HvParamReadIntWithWrite(
        pcParamStore, "\\HvDsp\\VideoProc\\CaptureVideo", "CamNum",
        &cResultSenderParam.cCapCamParam.nCamNum, cResultSenderParam.cCapCamParam.nCamNum,
        0, MAX_CAP_CAM_COUNT,
        "ץ���������", "",
        CUSTOM_LEVEL);

    char szSection[32], szComment[32];
    for (int i = 0; i < MAX_CAP_CAM_COUNT; i++)
    {
        sprintf(szSection, "RoadIP_%02d", i);
        sprintf(szComment, "%d�����IP", i);
        HvParamReadStringWithWrite(
            pcParamStore, "\\HvDsp\\VideoProc\\CaptureVideo", szSection,
            cResultSenderParam.cCapCamParam.rgszRoadIP[i],
            sizeof(cResultSenderParam.cCapCamParam.rgszRoadIP[i]),
            szComment, "",
            CUSTOM_LEVEL);

        sprintf(szSection, "SignalType_%02d", i);
        sprintf(szComment, "%d������ź�Դ����", i);
        HvParamReadIntWithWrite(
            pcParamStore, "\\HvDsp\\VideoProc\\CaptureVideo", szSection,
            &cResultSenderParam.cCapCamParam.rgnSignalType[i],
            cResultSenderParam.cCapCamParam.rgnSignalType[i],
            -1, 65535,
            szComment, "",
            CUSTOM_LEVEL);
    }
#else
    //TODO:һ�廯���ƽ̨ץ�������Ϊ1
    cResultSenderParam.cCapCamParam.nCamNum = 1;
#endif

    HvParamReadIntWithWrite(
        pcParamStore, "\\HvDsp\\Misc", "TraceRank",
        &g_nTraceRank,
        5, 1, 5,
        "������Ϣ������ƣ�1:�з���,3:���̼�,5:�û�����", "",
        PROJECT_LEVEL);

    HvParamReadIntWithWrite(
        pcParamStore,
        "\\HvDsp\\Misc", "AutoLinkEnable",
        &cResultSenderParam.cAutoLinkParam.fAutoLinkEnable, cResultSenderParam.cAutoLinkParam.fAutoLinkEnable,0,1,
        "��������ʹ��", "", CUSTOM_LEVEL
    );

    HvParamReadStringWithWrite(
        pcParamStore,
        "\\HvDsp\\Misc", "AutoLinkIP",
        cResultSenderParam.cAutoLinkParam.szAutoLinkIP, sizeof(cResultSenderParam.cAutoLinkParam.szAutoLinkIP),
        "�������ӷ�����IP", "", CUSTOM_LEVEL
    );

    HvParamReadIntWithWrite(
        pcParamStore,
        "\\HvDsp\\Misc", "AutoLinkPort",
        &cResultSenderParam.cAutoLinkParam.iAutoLinkPort, 6665,
        0,10000,
        "�������ӷ������˿�", "", CUSTOM_LEVEL
    );
#ifdef SINGLE_BOARD_PLATFORM

    HvParamReadIntWithWrite(
        pcParamStore,
        "\\HvDsp\\Misc", "MTUSetEnable",
        &cResultSenderParam.fMTUSetEnable , cResultSenderParam.fMTUSetEnable,
        0,1,
        "MTU����ʹ��", "", CUSTOM_LEVEL
    );

#endif

    return 0;
}

//--------------------------------------------------------------------------------------

static SignalMatchParam g_cSignalMatchParam;

static HV_PARAM_TYPE_INT g_rgcSignalMatchInt[] =
{
    {"\\OuterCtrl", "PlateHoldTime", (int*)&g_cSignalMatchParam.dwPlateHoldTime, 5000, 0, 600000, "���Ʊ���ʱ��", "", CUSTOM_LEVEL},
    {"\\OuterCtrl", "SignalHoldTime", (int*)&g_cSignalMatchParam.dwSignalHoldTime, 5000, 0, 600000, "�źű���ʱ��", "", CUSTOM_LEVEL},
    {"\\OuterCtrl", "OutPutPlateMode", &g_cSignalMatchParam.emOutPutPlateMode, 0, 0, 1, "���ܿ�ʹ�ܿ���", "0=�Զ�;1=�ź�", CUSTOM_LEVEL},
#ifdef SINGLE_BOARD_PLATFORM
    {"\\OuterCtrl", "CapCommandTime", &g_cSignalMatchParam.iCapCommandTime, 300, 0, 1000, "ץ��ͼ�������ʱ����", "", CUSTOM_LEVEL},
    {"\\OuterCtrl", "HardTriggerCap", (int*)&g_cSignalMatchParam.fHardTriggerCap, 0, 0, 1, "Ӳ����ץ�Ŀ���", "", CUSTOM_LEVEL},
#endif
    {"\\OuterCtrl", "ForcePlate", (int*)&g_cSignalMatchParam.fForcePlate, 0, 0, 1, "ǿ�Ƴ�ͼ", "", PROJECT_LEVEL},
    {"\\OuterCtrl", "SignalTypeNum", &g_cSignalMatchParam.nSignalTypeNum, 0, 0, 10, "�ź���������", "��Ч��Χ��0 - 10", CUSTOM_LEVEL},
    {"\\OuterCtrl", "SignalSourceNum", &g_cSignalMatchParam.nSignalSourceNum, 0, 0, 30, "�ź�Դ����", "��Ч��Χ��0 - 30", CUSTOM_LEVEL},
};

static int g_iSignalMatchIntArraySize = ARRSIZE(g_rgcSignalMatchInt);

int LoadSignalMatchParam(
    CParamStore* pcParamStore,
    SignalMatchParam& cSignalMatchParam
)
{
    PHV_PARAM_TYPE_INT pParamPointInt = (PHV_PARAM_TYPE_INT)&g_rgcSignalMatchInt;

    for ( int n = 0; n<g_iSignalMatchIntArraySize; ++n )
    {
        HvParamReadIntWithWrite(
            pcParamStore,
            pParamPointInt[n].strSection.GetBuffer(),
            pParamPointInt[n].strKey.GetBuffer(),
            pParamPointInt[n].piVal, pParamPointInt[n].iDefault,
            pParamPointInt[n].iMin, pParamPointInt[n].iMax,
            pParamPointInt[n].strChName.GetBuffer(),
            pParamPointInt[n].strComment.GetBuffer(),
            pParamPointInt[n].nRank
        );
    }

    cSignalMatchParam = g_cSignalMatchParam;
    HvParamReadStringWithWrite(
        pcParamStore,
        "IPTCtrl", "IPTComStr",
        cSignalMatchParam.szComStr, sizeof(cSignalMatchParam.szComStr),
        "IPT����", "", PROJECT_LEVEL
    );
    pcParamStore->GetBin("IPTCtrl", "DeviceParam", cSignalMatchParam.bDeviceParam, &cSignalMatchParam.nDeviceLen, "IPT�ڲ�����", "", 3);

    char szSection[200];
    char szAreaNames[200];
    for (int i = 0; i < MAX_IPT_PORT_NUM; i++)
    {
        sprintf(szSection, "IPTCtrl\\Port%02d", i);
        cSignalMatchParam.rgIPTInfo[i].iComSource = -1;
        HvParamReadIntWithWrite(
            pcParamStore,
            szSection, "ComSignalSource",
            &cSignalMatchParam.rgIPTInfo[i].iComSource, cSignalMatchParam.rgIPTInfo[i].iComSource,
            -1, (g_cSignalMatchParam.nSignalSourceNum - 1),
            "�ź�Դ", "",
            CUSTOM_LEVEL
        );
        for (int j = 0; j < MAX_IPT_PIN_NUM; j++)
        {
            sprintf(szAreaNames, "SignalSource%01d", j);
            cSignalMatchParam.rgIPTInfo[i].rgSigSource[j] = -1;
            HvParamReadIntWithWrite(
                pcParamStore,
                szSection, szAreaNames,
                &cSignalMatchParam.rgIPTInfo[i].rgSigSource[j], cSignalMatchParam.rgIPTInfo[i].rgSigSource[j],
                -1, (g_cSignalMatchParam.nSignalSourceNum - 1),
                "�ź�Դ", "",
                CUSTOM_LEVEL
            );
        }
        cSignalMatchParam.rgIPTInfo[i].iOutSigSource = -1;
        HvParamReadIntWithWrite(
            pcParamStore,
            szSection, "OutSignalSource",
            &cSignalMatchParam.rgIPTInfo[i].iOutSigSource, cSignalMatchParam.rgIPTInfo[i].iOutSigSource,
            -1, (g_cSignalMatchParam.nSignalSourceNum - 1),
            "�����ƽ�źŶ�Ӧ���ź�Դ", "",
            CUSTOM_LEVEL
        );
        HvParamReadIntWithWrite(
            pcParamStore,
            szSection, "OutSignalLevel",
            &cSignalMatchParam.rgIPTInfo[i].iOutSigLevel, cSignalMatchParam.rgIPTInfo[i].iOutSigLevel,
            0, 1,
            "�����ƽ", "",
            CUSTOM_LEVEL
        );
        HvParamReadIntWithWrite(
            pcParamStore,
            szSection, "OutSignalWidth",
            &cSignalMatchParam.rgIPTInfo[i].iOutSigWidth, cSignalMatchParam.rgIPTInfo[i].iOutSigWidth,
            0, 5000,
            "�������", "",
            CUSTOM_LEVEL
        );
        cSignalMatchParam.rgIPTInfo[i].fUse =
            (cSignalMatchParam.rgIPTInfo[i].iComSource != -1
             || cSignalMatchParam.rgIPTInfo[i].rgSigSource[0] != -1
             || cSignalMatchParam.rgIPTInfo[i].rgSigSource[1] != -1
             || cSignalMatchParam.rgIPTInfo[i].iOutSigSource != -1) ? true : false;

    }
    HvParamReadIntWithWrite(
        pcParamStore,
        "\\OuterCtrl", "MainSignalType",
        &cSignalMatchParam.iMainSignalType, -1,
        -1, (cSignalMatchParam.nSignalTypeNum - 1),
        "���źű��", "",
        CUSTOM_LEVEL
    );
#ifdef SINGLE_BOARD_PLATFORM
    HvParamReadIntWithWrite(
        pcParamStore,
        "\\OuterCtrl", "CapImgCount",
        &cSignalMatchParam.iCapImgCount,
        cSignalMatchParam.iCapImgCount, 1, 2,
        "ץ��ͼ����", "",
        CUSTOM_LEVEL
    );
#endif
    HvParamReadIntWithWrite(
        pcParamStore,
        "\\OuterCtrl\\OtherSignalSource", "SoftTrigger",
        &cSignalMatchParam.nSoftTrigSignalSource,
        cSignalMatchParam.nSoftTrigSignalSource, -1, 65535,
        "�����ź�Դ���", "",
        CUSTOM_LEVEL
    );

    //---

    //char szSection[256];
    for (int i = 0; i < cSignalMatchParam.nSignalTypeNum; i++)
    {
        sprintf(szSection, "\\OuterCtrl\\SignalType%02d", i);
        HvParamReadIntWithWrite(
            pcParamStore,
            szSection, "AttachType",
            (int*)&cSignalMatchParam.rgMatchSigType[i].atType,
            ATTACH_INFO_NOTHING, 0, 255,
            "�ź�����(0:����, 1:�ٶ�, 2:ץ��)", "",
            CUSTOM_LEVEL
        );

        cSignalMatchParam.rgMatchSigType[i].fMainSignal \
        = (i == cSignalMatchParam.iMainSignalType ? true : false);

        if (cSignalMatchParam.rgMatchSigType[i].atType == 0
                && cSignalMatchParam.rgMatchSigType[i].fMainSignal
                && cSignalMatchParam.emOutPutPlateMode == PLATE_OUTPUT_SIGNAL)
        {
            cSignalMatchParam.fSignalIsMain = true;
        }

        HvParamReadStringWithWrite(
            pcParamStore,
            szSection, "Name",
            cSignalMatchParam.rgMatchSigType[i].strName, 64,
            "��������", "",
            CUSTOM_LEVEL
        );

        cSignalMatchParam.nMainSigTypeNum = 1;
    }

    //---

    for (int i = 0; i < cSignalMatchParam.nSignalSourceNum; i++)
    {
        sprintf(szSection, "\\OuterCtrl\\SignalSource%02d", i);
        HvParamReadIntWithWrite(
            pcParamStore,
            szSection, "MatchType",
            &cSignalMatchParam.rgSigSource[i].nType,
            cSignalMatchParam.rgSigSource[i].nType, 0, 255,
            "�ź����ͱ��", "",
            CUSTOM_LEVEL
        );

        if ((cSignalMatchParam.rgSigSource[i].nType < 0)
                || (cSignalMatchParam.rgSigSource[i].nType >= cSignalMatchParam.nSignalTypeNum))
        {
            cSignalMatchParam.rgSigSource[i].nType = 0;
        }

        HvParamReadIntWithWrite(
            pcParamStore,
            szSection, "PrevTime",
            (int *)(&cSignalMatchParam.rgSigSource[i].dwPrevTime),
            cSignalMatchParam.rgSigSource[i].dwPrevTime, 0, 600000,
            "�ź�ǰʱ��", "",
            CUSTOM_LEVEL
        );

        HvParamReadIntWithWrite(
            pcParamStore,
            szSection, "PostTime",
            (int *)(&cSignalMatchParam.rgSigSource[i].dwPostTime),
            cSignalMatchParam.rgSigSource[i].dwPostTime, 0, 600000,
            "�źź�ʱ��", "",
            CUSTOM_LEVEL
        );

        HvParamReadIntWithWrite(
            pcParamStore,
            szSection, "Road",
            &cSignalMatchParam.rgSigSource[i].iRoad,
            cSignalMatchParam.rgSigSource[i].iRoad, 0, 0xff,
            "����(0xff:ȫ����)", "",
            PROJECT_LEVEL
        );
        //�շ�վģʽ�£�������ǿ����Ϊ0
        cSignalMatchParam.rgSigSource[i].iRoad = 0;

        if (cSignalMatchParam.rgMatchSigType[cSignalMatchParam.rgSigSource[i].nType].fMainSignal)
        {
            cSignalMatchParam.nMainSourceNum++;
        }
    }

#ifndef SINGLE_BOARD_PLATFORM
    cSignalMatchParam.fEnableIPT = 0;  //ƽ�д������
    HvParamReadIntWithWrite(
        pcParamStore,
        "\\OuterCtrl", "IPTEnable",
        &cSignalMatchParam.fEnableIPT,
        cSignalMatchParam.fEnableIPT, 0, 1,
        "IPTʹ�ܿ���", "",
        CUSTOM_LEVEL
    );
#endif

    return 0;
}

//--------------------------------------------------------------------------------------

int LoadTcpipParam_1(
    CParamStore* pcParamStore,
    TcpipParam& cTcpipCfgParam
)
{
#ifdef SINGLE_BOARD_PLATFORM
    HvParamReadStringWithWrite(
        pcParamStore,
        "\\System\\CamLan", "CommIPAddr",
        cTcpipCfgParam.szIp, 31,
        "IP��ַ", "", CUSTOM_LEVEL
    );
    HvParamReadStringWithWrite(
        pcParamStore,
        "\\System\\CamLan", "CommMask",
        cTcpipCfgParam.szNetmask, 31,
        "��������", "", CUSTOM_LEVEL
    );
    HvParamReadStringWithWrite(
        pcParamStore,
        "\\System\\CamLan", "CommGateway",
        cTcpipCfgParam.szGateway, 31,
        "����", "", CUSTOM_LEVEL
    );
#endif
    return 0;
}

int LoadTcpipParam_2(
    CParamStore* pcParamStore,
    TcpipParam& cTcpipCfgParam
)
{
#ifdef SINGLE_BOARD_PLATFORM
    HvParamReadStringWithWrite(
        pcParamStore,
        "\\System\\TcpipCfg", "CommIPAddr",
        cTcpipCfgParam.szIp, 31,
        "IP��ַ", "", CUSTOM_LEVEL
    );
    HvParamReadStringWithWrite(
        pcParamStore,
        "\\System\\TcpipCfg", "CommMask",
        cTcpipCfgParam.szNetmask, 31,
        "��������", "", CUSTOM_LEVEL
    );
    HvParamReadStringWithWrite(
        pcParamStore,
        "\\System\\TcpipCfg", "CommGateway",
        cTcpipCfgParam.szGateway, 31,
        "����", "", CUSTOM_LEVEL
    );

    char szIp[32] = {0};
    char szMask[32] = {0};
    char szGateway[32] = {0};
    char szMac[32] = {0};
    char szSn[128] = {0};

    GetLocalTcpipAddr("eth0.20", szIp, szMask, szGateway, szMac);

    HvParamWriteString(
        pcParamStore,
        "\\System", "CommMac",
        szMac, 31,
        "MAC��ַ", "", CUSTOM_LEVEL
    );

    GetSN(szSn, sizeof(szSn));
    HvParamWriteString(
        pcParamStore,
        "\\System", "CommDevNo",
        szSn, 127,
        "�豸���", "", CUSTOM_LEVEL
    );
#endif
    return 0;
}

//--------------------------------------------------------------------------------------

BOOL CheckArea(int nLeft, int nTop, int nRight, int nBottom)
{
    if (nLeft < 0 || nTop < 0 || nRight < 0 || nBottom < 0
            || nLeft > 100 || nTop > 100 || nRight > 100 || nBottom > 100
            || (nLeft >= nRight) || (nTop >= nBottom))
    {
        return FALSE;
    }
    return TRUE;
}

int LoadTrackerCfgParam(
    CParamStore* pcParamStore,
    TRACKER_CFG_PARAM& cTrackerCfgParam,
    ModuleParams& cModuleParams
)
{
    HvParamReadIntWithWrite(
        pcParamStore,
        "\\Tracker\\Recognition", "EnableRecongCarColor",
        &cTrackerCfgParam.fEnableRecgCarColor, cTrackerCfgParam.fEnableRecgCarColor,
        0, 1,
        "������ɫʶ�𿪹�", "", CUSTOM_LEVEL
    );

    //�շ�վ���в��� ǿ�ư��Ʒָ�
    HvParamReadIntWithWrite(
        pcParamStore,
        "\\Tracker\\ProcessPlate", "SegWhitePlate",
        &cTrackerCfgParam.nSegWhitePlate, cTrackerCfgParam.nSegWhitePlate,
        0, 1,
        "ǿ�ư��Ʒָ�", "", CUSTOM_LEVEL
    );

//==========ProcessPlate==============
    HvParamReadIntWithWrite(
        pcParamStore,
        "\\Tracker\\ProcessPlate", "EnableDBGreenSegment",
        &cTrackerCfgParam.nPlateDetect_Green, cTrackerCfgParam.nPlateDetect_Green,
        0, 1,
        "����ʶ�𿪹�", "", CUSTOM_LEVEL
    );

    HvParamReadIntWithWrite(
        pcParamStore,
        "\\Tracker\\ProcessPlate", "LightBlueFlag",
        &cTrackerCfgParam.nProcessPlate_LightBlue, cTrackerCfgParam.nProcessPlate_LightBlue,
        0, 1,
        "ǳ���ƿ���", "", PROJECT_LEVEL
    );

    HvParamReadIntWithWrite(
        pcParamStore,
        "\\Tracker\\DoublePlate", "DoublePlateEnable",
        &cTrackerCfgParam.fDoublePlateEnable, cTrackerCfgParam.fDoublePlateEnable,
        1, 3,
        "˫���Ƽ�⿪��", "", PROJECT_LEVEL
    );

    HvParamReadIntWithWrite(
        pcParamStore,
        "\\Tracker\\ProcessPlate", "EnableBigPlate",
        &cTrackerCfgParam.nEnableBigPlate, cTrackerCfgParam.nEnableBigPlate,
        0, 1,
        "����ʶ�𿪹�", "", CUSTOM_LEVEL
    );

    HvParamReadIntWithWrite(
        pcParamStore,
        "\\Tracker\\ProcessPlate", "EnablePlateEnhance",
        &cTrackerCfgParam.nEnablePlateEnhance, cTrackerCfgParam.nEnablePlateEnhance,
        0, 1,
        "�ָ�ǰ�Ƿ����ͼƬ��ǿ", "", PROJECT_LEVEL
    );

    HvParamReadIntWithWrite(
        pcParamStore,
        "\\Tracker\\ProcessPlate", "PlateResizeThreshold",
        &cTrackerCfgParam.nPlateResizeThreshold, cTrackerCfgParam.nPlateResizeThreshold,
        0, 200,
        "�ָ�ǰСͼ��������ֵ", "", PROJECT_LEVEL
    );

    HvParamReadIntWithWrite(
        pcParamStore,
        "\\Tracker\\ProcessPlate\\BlackPlate", "NightThreshold",
        &cTrackerCfgParam.nNightThreshold, cTrackerCfgParam.nNightThreshold,
        0, 240,
        "ҹ��ģʽ��������", "", CUSTOM_LEVEL
    );

    HvParamReadIntWithWrite(
        pcParamStore,
        "\\Tracker\\ProcessPlate\\BlackPlate", "PlateLightCheckCount",
        &cTrackerCfgParam.nPlateLightCheckCount, cTrackerCfgParam.nPlateLightCheckCount,
        3, 50,
        "���ȵ������", "", CUSTOM_LEVEL
    );

    HvParamReadIntWithWrite(
        pcParamStore,
        "\\Tracker\\ProcessPlate\\BlackPlate", "MinPlateBrightness",
        &cTrackerCfgParam.nMinPlateBrightness, cTrackerCfgParam.nMinPlateBrightness,
        1, 255,
        "�����������", "", CUSTOM_LEVEL
    );

    HvParamReadIntWithWrite(
        pcParamStore,
        "\\Tracker\\ProcessPlate\\BlackPlate", "MaxPlateBrightness",
        &cTrackerCfgParam.nMaxPlateBrightness, cTrackerCfgParam.nMaxPlateBrightness,
        1, 255,
        "�����������", "", CUSTOM_LEVEL
    );
#ifndef SINGLE_BOARD_PLATFORM
    HvParamReadIntWithWrite(
        pcParamStore,
        "\\Tracker\\ProcessPlate\\BlackPlate", "CtrlCpl",
        &cTrackerCfgParam.nCtrlCpl, cTrackerCfgParam.nCtrlCpl,
        0, 1,
        "����ƫ�⾵", "", CUSTOM_LEVEL
    );

    HvParamReadIntWithWrite(
        pcParamStore,
        "\\Tracker\\ProcessPlate\\BlackPlate", "LightTypeCpl",
        &cTrackerCfgParam.nLightTypeCpl, cTrackerCfgParam.nLightTypeCpl,
        0, 12,
        "����ƫ�⾵�����ȵȼ�", "", CUSTOM_LEVEL
    );

        HvParamReadIntWithWrite(
        pcParamStore,
        "HvDsp\\Misc", "ForceLightOffAtDay",
        &cTrackerCfgParam.iForceLightOffAtDay, cTrackerCfgParam.iForceLightOffAtDay,
        0, 1,
        "����ǿ��Ϩ�𲹹��", "", CUSTOM_LEVEL
    );

    HvParamReadIntWithWrite(
        pcParamStore,
        "HvDsp\\Misc", "ForceLightOffThreshold",
        &cTrackerCfgParam.iForceLightThreshold, cTrackerCfgParam.iForceLightThreshold,
        50, 200,
        "����ǿ��Ϩ�𲹹�ƻ������ȷ�ֵ", "", PROJECT_LEVEL
    );

    HvParamReadIntWithWrite(
        pcParamStore,
        "HvDsp\\Misc", "DisableFlashAtDay",
        &cTrackerCfgParam.iDisableFlashAtDay, cTrackerCfgParam.iDisableFlashAtDay,
        0, 1,
        "����ǿ��Ϩ�������", "", CUSTOM_LEVEL
    );
#endif
    HvParamReadIntWithWrite(
        pcParamStore,
        "\\Tracker\\ProcessPlate\\BlackPlate", "H0",
        &cTrackerCfgParam.nProcessPlate_BlackPlateThreshold_H0, cTrackerCfgParam.nProcessPlate_BlackPlateThreshold_H0,
        0, 240,
        "����ɫ������", "", PROJECT_LEVEL
    );

    HvParamReadIntWithWrite(
        pcParamStore,
        "\\Tracker\\ProcessPlate\\BlackPlate", "H1",
        &cTrackerCfgParam.nProcessPlate_BlackPlateThreshold_H1, cTrackerCfgParam.nProcessPlate_BlackPlateThreshold_H1,
        cTrackerCfgParam.nProcessPlate_BlackPlateThreshold_H0, 240,
        "����ɫ������", "", PROJECT_LEVEL
    );

    HvParamReadIntWithWrite(
        pcParamStore,
        "\\Tracker\\ProcessPlate\\BlackPlate", "S",
        &cTrackerCfgParam.nProcessPlate_BlackPlate_S, cTrackerCfgParam.nProcessPlate_BlackPlate_S,
        0, 240,
        "���Ʊ��Ͷ�����", "", CUSTOM_LEVEL
    );

    HvParamReadIntWithWrite(
        pcParamStore,
        "\\Tracker\\ProcessPlate\\BlackPlate", "L",
        &cTrackerCfgParam.nProcessPlate_BlackPlate_L, cTrackerCfgParam.nProcessPlate_BlackPlate_L,
        0, 240,
        "������������", "", PROJECT_LEVEL
    );
//========End ProcessPlate==================

    HvParamReadIntWithWrite(
        pcParamStore,
        "Tracker\\Recognition","UseEdgeMethod",
        &cTrackerCfgParam.fUseEdgeMethod, cTrackerCfgParam.fUseEdgeMethod,
        0, 1,
        "��ǿ����ͼʶ��", "", CUSTOM_LEVEL
    );
    HvParamReadIntWithWrite(
        pcParamStore,
        "Tracker\\Recognition","EnableT1Model",
        &cTrackerCfgParam.fEnableT1Model, cTrackerCfgParam.fEnableT1Model,
        0, 1,
        "T-1ģ�Ϳ���", "", PROJECT_LEVEL
    );
    HvParamReadIntWithWrite(
        pcParamStore,
        "Tracker\\Recognition\\DefaultChn", "EnableDefaultWJ",
        &cTrackerCfgParam.fEnableDefaultWJ, cTrackerCfgParam.fEnableDefaultWJ,
        0, 1,
        "�������侯�ַ�����", "", CUSTOM_LEVEL
    );
    HvParamReadStringWithWrite(
        pcParamStore,
        "Tracker\\Recognition\\DefaultChn", "DefaultWJChar",
        cTrackerCfgParam.szDefaultWJChar, sizeof(cTrackerCfgParam.szDefaultWJChar) - 1,
        "�������侯�ַ�", "", CUSTOM_LEVEL
    );
    HvParamReadIntWithWrite(
        pcParamStore,
        "Tracker\\Misc", "EnableAlphaRecog",
        &cTrackerCfgParam.fEnableAlpha_5, cTrackerCfgParam.fEnableAlpha_5,
        0, 1,
        "������ĸʶ�𿪹�", "", CUSTOM_LEVEL
    );
    HvParamReadIntWithWrite(
        pcParamStore,
        "Tracker\\Recognition", "RecogAsteriskThreshold",
        &cTrackerCfgParam.nRecogAsteriskThreshold, cTrackerCfgParam.nRecogAsteriskThreshold,
        0, 100,
        "�÷ֵ�����ֵ(�ٷֱ�)��#��ʾ", "", PROJECT_LEVEL
    );
    HvParamReadIntWithWrite(
        pcParamStore,
        "Tracker\\Misc", "RecogGXPolice",
        &cTrackerCfgParam.nRecogGxPolice, cTrackerCfgParam.nRecogGxPolice,
        0, 1,
        "�ط�����ʶ�𿪹�", "", PROJECT_LEVEL
    );
    HvParamReadIntWithWrite(
        pcParamStore,
        "Tracker\\Misc", "CarArrivedPos",
        &cTrackerCfgParam.nCarArrivedPos, cTrackerCfgParam.nCarArrivedPos,
        0, 100,
        "��������λ��(��������������)", "", CUSTOM_LEVEL
    );
    HvParamReadIntWithWrite(
        pcParamStore,
        "Tracker\\Misc", "CarArrivedPosYellow",
        &cTrackerCfgParam.nCarArrivedPosYellow, cTrackerCfgParam.nCarArrivedPosYellow,
        0, 100,
        "��������λ��(����)", "", CUSTOM_LEVEL
    );

    // ��Ƶ���շ�վΪ�����Ƴ���ʹ�õĲ���
    HvParamReadIntWithWrite(
        pcParamStore,
        "Tracker\\Misc", "OutPutNoPlate",
        &cTrackerCfgParam.iOutPutNoPlate, cTrackerCfgParam.iOutPutNoPlate,
        0, 1,
        "������Ƴ�", "��Ƶ���շ�վ����޳��ƽ��", CUSTOM_LEVEL
    );
    HvParamReadIntWithWrite(
        pcParamStore,
        "Tracker\\Misc", "PrevTime",
        &cTrackerCfgParam.iPrevTime, cTrackerCfgParam.iPrevTime,
        0, 60000,
        "��ǰƥ�䳵��ʱ��", "�ź���ǰ���ҳ���ʱ��", CUSTOM_LEVEL
    );
    HvParamReadIntWithWrite(
        pcParamStore,
        "Tracker\\Misc", "PostTime",
        &cTrackerCfgParam.iPostTime, cTrackerCfgParam.iPostTime,
        0, 60000,
        "���ƥ�䳵��ʱ��", "�ź�������ҳ���ʱ��", CUSTOM_LEVEL
    );
    /*HvParamReadIntWithWrite(
        pcParamStore,
        "Tracker\\Misc", "CarArriveTrig",
        &cTrackerCfgParam.nCarArriveTrig, cTrackerCfgParam.nCarArriveTrig,
        0, 1,
        "�շ�վ����ץ��", "", CUSTOM_LEVEL
        );*/
    /*HvParamReadIntWithWrite(
        pcParamStore,
        "\\Tracker\\VideoDet", "VideoDetMode",
        &cTrackerCfgParam.cVideoDet.nVideoDetMode,
        cTrackerCfgParam.cVideoDet.nVideoDetMode,
        NO_VIDEODET, VIDEODET_MODE_COUNT,
        "��Ƶ���ģʽ", "0:��ʹ����Ƶ���, 1:ֻʹ�ñ������, 2:ʹ����Ƶ���", CUSTOM_LEVEL
    );
    HvParamReadIntWithWrite(
        pcParamStore,
        "\\Tracker\\VideoDet\\AreaCtrl", "Left",
        &cTrackerCfgParam.cVideoDet.rcVideoDetArea.left,
        cTrackerCfgParam.cVideoDet.rcVideoDetArea.left,
        0, 100,
        "��Ƶ���������", "", CUSTOM_LEVEL
    );
    HvParamReadIntWithWrite(
        pcParamStore,
        "\\Tracker\\VideoDet\\AreaCtrl", "Right",
        &cTrackerCfgParam.cVideoDet.rcVideoDetArea.right,
        cTrackerCfgParam.cVideoDet.rcVideoDetArea.right,
        0, 100,
        "��Ƶ���������", "", CUSTOM_LEVEL
    );
    HvParamReadIntWithWrite(
        pcParamStore,
        "\\Tracker\\VideoDet\\AreaCtrl", "Top",
        &cTrackerCfgParam.cVideoDet.rcVideoDetArea.top,
        cTrackerCfgParam.cVideoDet.rcVideoDetArea.top,
        0, 100,
        "��Ƶ���������", "", CUSTOM_LEVEL
    );
    HvParamReadIntWithWrite(
        pcParamStore,
        "\\Tracker\\VideoDet\\AreaCtrl", "Bottom",
        &cTrackerCfgParam.cVideoDet.rcVideoDetArea.bottom,
        cTrackerCfgParam.cVideoDet.rcVideoDetArea.bottom,
        0, 100,
        "��Ƶ���������", "", CUSTOM_LEVEL
    );*/

    /*HvParamReadIntWithWrite(
        pcParamStore,
        "Tracker\\Misc", "CarArrivedDelay",
        &cTrackerCfgParam.nCarArrivedDelay, cTrackerCfgParam.nCarArrivedDelay,
        0, 100,
        "�������ﴥ���ӳپ���(��)", "", CUSTOM_LEVEL
        );*/
    HvParamReadIntWithWrite(
        pcParamStore,
        "Tracker\\TrackInfo", "VoteFrameNum",
        &cTrackerCfgParam.nVoteFrameNum, cTrackerCfgParam.nVoteFrameNum,
        0, 1000,
        "ͶƱ�Ľ����", "", CUSTOM_LEVEL
    );
    HvParamReadIntWithWrite(
        pcParamStore,
        "Tracker\\TrackInfo", "MaxEqualFrameNumForVote",
        &cTrackerCfgParam.nMaxEqualFrameNumForVote, cTrackerCfgParam.nMaxEqualFrameNumForVote,
        0, 1000,
        "������ͬ�����������", "", CUSTOM_LEVEL
    );
    HvParamReadIntWithWrite(
        pcParamStore,
        "Tracker\\TrackInfo", "BlockTwinsTimeout",
        &cTrackerCfgParam.nBlockTwinsTimeout, cTrackerCfgParam.nBlockTwinsTimeout,
        0, 3600,
        "��ͬ�����Сʱ����(S)", "", CUSTOM_LEVEL
    );
    /*HvParamReadIntWithWrite(
        pcParamStore,
        "Tracker\\TrackInfo", "AverageConfidenceQuan",
        &cTrackerCfgParam.nAverageConfidenceQuan, cTrackerCfgParam.nAverageConfidenceQuan,
        0, 65536,
        "ƽ���÷�����", "", 1
        );*/
    HvParamReadIntWithWrite(
        pcParamStore,
        "Tracker\\TrackInfo", "FirstConfidenceQuan",
        &cTrackerCfgParam.nFirstConfidenceQuan, cTrackerCfgParam.nFirstConfidenceQuan,
        0, 65536,
        "���ֵ÷�����", "", PROJECT_LEVEL
    );
    HvParamReadIntWithWrite(
        pcParamStore,
        "Tracker\\TrackInfo", "RemoveLowConfForVote",
        &cTrackerCfgParam.nRemoveLowConfForVote, cTrackerCfgParam.nRemoveLowConfForVote,
        0, 100,
        "ͶƱǰȥ���͵÷ֳ��ưٷֱ�", "", PROJECT_LEVEL
    );

    HvParamReadIntWithWrite(
        pcParamStore,
        "\\Tracker\\DetAreaCtrl\\Normal", "Enable",
        &cTrackerCfgParam.cDetectArea.fEnableDetAreaCtrl,
        cTrackerCfgParam.cDetectArea.fEnableDetAreaCtrl,
        0, 1,
        "ɨ��������ƿ���", "", CUSTOM_LEVEL
    );

    //�շ�վģʽ�²�ʹ����Ƶ���
    cTrackerCfgParam.cVideoDet.nVideoDetMode = 0;
    HV_Trace(5, "��Ƶ��⿪��:%d\n",cTrackerCfgParam.cVideoDet.nVideoDetMode);
    //�ȳ�ʼ��ɨ������ÿ��ģʽ�����Լ���ɨ������
    cTrackerCfgParam.cDetectArea.DetectorAreaLeft = 0;
    cTrackerCfgParam.cDetectArea.DetectorAreaTop = 30;
    cTrackerCfgParam.cDetectArea.DetectorAreaRight = 95;
    cTrackerCfgParam.cDetectArea.DetectorAreaBottom = 90;
    cTrackerCfgParam.cDetectArea.nDetectorMinScaleNum = 6;
    cTrackerCfgParam.cDetectArea.nDetectorMaxScaleNum = 14;
    HvParamReadIntWithWrite(
        pcParamStore,
        "\\Tracker\\DetAreaCtrl\\Normal", "Left",
        &cTrackerCfgParam.cDetectArea.DetectorAreaLeft,
        cTrackerCfgParam.cDetectArea.DetectorAreaLeft,
        0, 100,
        "ɨ�������������", "���ٷֱ���", CUSTOM_LEVEL
    );
    HvParamReadIntWithWrite(
        pcParamStore,
        "\\Tracker\\DetAreaCtrl\\Normal", "Top",
        &cTrackerCfgParam.cDetectArea.DetectorAreaTop,
        cTrackerCfgParam.cDetectArea.DetectorAreaTop,
        0, 100,
        "ɨ�������������", "���ٷֱ���", CUSTOM_LEVEL
    );
    HvParamReadIntWithWrite(
        pcParamStore,
        "\\Tracker\\DetAreaCtrl\\Normal", "Right",
        &cTrackerCfgParam.cDetectArea.DetectorAreaRight,
        cTrackerCfgParam.cDetectArea.DetectorAreaRight,
        0, 100,
        "ɨ�������������", "���ٷֱ���", CUSTOM_LEVEL
    );
    HvParamReadIntWithWrite(
        pcParamStore,
        "\\Tracker\\DetAreaCtrl\\Normal", "Bottom",
        &cTrackerCfgParam.cDetectArea.DetectorAreaBottom,
        cTrackerCfgParam.cDetectArea.DetectorAreaBottom,
        0, 100,
        "ɨ�������������", "���ٷֱ���", CUSTOM_LEVEL
    );
    HvParamReadIntWithWrite(
        pcParamStore,
        "\\Tracker\\DetAreaCtrl\\Normal", "MinScale",
        &cTrackerCfgParam.cDetectArea.nDetectorMinScaleNum,
        cTrackerCfgParam.cDetectArea.nDetectorMinScaleNum,
        0, 16,
        "������С���", "56*(1.1^MinScale)", CUSTOM_LEVEL
    );
    HvParamReadIntWithWrite(
        pcParamStore,
        "\\Tracker\\DetAreaCtrl\\Normal", "MaxScale",
        &cTrackerCfgParam.cDetectArea.nDetectorMaxScaleNum,
        cTrackerCfgParam.cDetectArea.nDetectorMaxScaleNum,
        0, 16,
        "���������", "56*(1.1^MaxScale)", CUSTOM_LEVEL
    );

    //ɨ��������Ч���ж�
    if (!CheckArea(
                cTrackerCfgParam.cDetectArea.DetectorAreaLeft,
                cTrackerCfgParam.cDetectArea.DetectorAreaTop,
                cTrackerCfgParam.cDetectArea.DetectorAreaRight,
                cTrackerCfgParam.cDetectArea.DetectorAreaBottom
            ))
    {
        cTrackerCfgParam.cDetectArea.DetectorAreaLeft = 0;
        cTrackerCfgParam.cDetectArea.DetectorAreaTop = 30;
        cTrackerCfgParam.cDetectArea.DetectorAreaRight = 95;
        cTrackerCfgParam.cDetectArea.DetectorAreaBottom = 90;
        pcParamStore->SetInt(
            "\\Tracker\\DetAreaCtrl\\Normal", "Left",
            cTrackerCfgParam.cDetectArea.DetectorAreaLeft
        );
        pcParamStore->SetInt(
            "\\Tracker\\DetAreaCtrl\\Normal", "Top",
            cTrackerCfgParam.cDetectArea.DetectorAreaTop
        );
        pcParamStore->SetInt(
            "\\Tracker\\DetAreaCtrl\\Normal", "Right",
            cTrackerCfgParam.cDetectArea.DetectorAreaRight
        );
        pcParamStore->SetInt(
            "\\Tracker\\DetAreaCtrl\\Normal", "Bottom",
            cTrackerCfgParam.cDetectArea.DetectorAreaBottom
        );
    }

    //Scale��Ч���ж�
    if (cTrackerCfgParam.cDetectArea.nDetectorMinScaleNum >
            cTrackerCfgParam.cDetectArea.nDetectorMaxScaleNum)
    {
        pcParamStore->SetInt("\\Tracker\\DetAreaCtrl\\Normal", "MinScale", 6);
        pcParamStore->SetInt("\\Tracker\\DetAreaCtrl\\Normal", "MaxScale", 14);
        cTrackerCfgParam.cDetectArea.nDetectorMinScaleNum = 6;
        cTrackerCfgParam.cDetectArea.nDetectorMaxScaleNum = 14;
    }

    return 0;
}

char *g_rgszLightType[] =
{
    "BigSuitlight",
    "BigSuitlight2",
    "Suitlight",
    "Suitlight2",
    "Day",
    "Day2",
    "Backlight",
    "Backlight2",
    "BigBacklight",
    "BigBacklight2",
    "Nightfall",
    "Nightfall2",
    "Night",
    "Night2"
};

HRESULT InitCamCfgParam(CParamStore* pcParamStore, CAM_CFG_PARAM *pCfgCamParam)
{
    if (pCfgCamParam->iMinExposureTime == 0)
    {
        float fTmp = pCfgCamParam->fltDB / 20;
        fTmp = pow((float)10.0, fTmp);
        pCfgCamParam->irgExposureTime[13] = pCfgCamParam->iMaxExposureTime;
        pCfgCamParam->irgGain[13] = pCfgCamParam->iMaxGain;

        for (int i = 12; i >= 0; i--)
        {
            int iNewExposureTime, iNewGain;
            int iLastExposureTime, iLastGain;
            iLastGain = pCfgCamParam->irgGain[i + 1];
            iNewGain = iLastGain - (int)(pCfgCamParam->fltDB * 10);
            iLastExposureTime = pCfgCamParam->irgExposureTime[i + 1];
            if ( iNewGain >= pCfgCamParam->iMinGain )
            {
                pCfgCamParam->irgExposureTime[i] = iLastExposureTime;
                pCfgCamParam->irgGain[i] = iNewGain;
            }
            else
            {
                iNewExposureTime = (int)(((float)iLastExposureTime / fTmp) + 0.5);
                pCfgCamParam->irgExposureTime[i] = iNewExposureTime;
                pCfgCamParam->irgGain[i] = iLastGain;
            }
        }
    }
    else
    {
        float fTmp = pCfgCamParam->fltDB / 20;
        fTmp = pow((float)10.0, fTmp);
        int iGainLevel = (int)(pCfgCamParam->fltDB * 10);

        pCfgCamParam->irgExposureTime[0] = pCfgCamParam->iMinExposureTime;
        pCfgCamParam->irgGain[0] = pCfgCamParam->iMinGain;

        for (int i = 1; i < 14; i++)
        {
            int iNewExposureTime, iNewGain;
            int iLastExposureTime, iLastGain;
            iLastExposureTime = pCfgCamParam->irgExposureTime[i - 1];
            iLastGain = pCfgCamParam->irgGain[i - 1];
            iNewExposureTime = (int)(((float)iLastExposureTime * fTmp) + 0.5);
            if ( iNewExposureTime <= pCfgCamParam->iMaxExposureTime )
            {
                pCfgCamParam->irgExposureTime[i] = iNewExposureTime;
                pCfgCamParam->irgGain[i] = iLastGain;
            }
            else
            {
                iNewGain = iLastGain + iGainLevel;
                if ( iNewGain > pCfgCamParam->iMaxGain )
                {
                    pCfgCamParam->irgExposureTime[i] = iLastExposureTime;
                    pCfgCamParam->irgGain[i] = iLastGain;
                }
                else
                {
                    pCfgCamParam->irgExposureTime[i] = iLastExposureTime;
                    pCfgCamParam->irgGain[i] = iNewGain;
                }
            }
        }
    }

    char szText[255];
    int iAGCDB = (pCfgCamParam->iMaxAGCLimit - pCfgCamParam->iMinAGCLimit) / 13;
    if (iAGCDB > 0)
    {
        for (int i = 0; i < 14; i++)
        {
            sprintf(szText, "\\HvDsp\\Camera\\%s", g_rgszLightType[i]);
            pCfgCamParam->irgAGCLimit[i] = pCfgCamParam->iMinAGCLimit + (iAGCDB * i);
            pcParamStore->SetInt(szText, "AGCLimit", pCfgCamParam->irgAGCLimit[i]);
        }
    }

    if (pCfgCamParam->iEnableAGC)
    {
        for (int i = 0; i < 14; i++)
        {
            pCfgCamParam->irgExposureTime[i] = -1;
            pCfgCamParam->irgGain[i] = -1;
        }
    }

    for (int i = 0; i < 14; i++)
    {
        sprintf(szText, "\\HvDsp\\Camera\\%s", g_rgszLightType[i]);
        pcParamStore->SetInt(szText, "ExposureTime", pCfgCamParam->irgExposureTime[i]);
        pcParamStore->SetInt(szText, "Plus", pCfgCamParam->irgGain[i]);
    }

    return S_OK;
}

int LoadCamCfgParam(
    CParamStore* pcParamStore,
    CAM_CFG_PARAM& cCamCfgParam
)
{
#ifdef SINGLE_BOARD_PLATFORM
    HvParamReadStringWithWrite(
        pcParamStore,
        "\\HvDsp\\Camera\\Ctrl", "Addr",
        cCamCfgParam.szIP, (100-1),
        "ʶ�����IP", "", CUSTOM_LEVEL
    );
    cCamCfgParam.iCamType = 2;
    HvParamReadIntWithWrite(
        pcParamStore,
        "\\HvDsp\\Camera\\Ctrl", "ProtocolType",
        &cCamCfgParam.iCamType, cCamCfgParam.iCamType,
        0, 3,
        "Э������", "0:����ר��Э��; 1:NVC���; 2:�������; 3:�㲥Э��;", CUSTOM_LEVEL
    );
#else
    HvParamReadStringWithWrite(
        pcParamStore,
        "\\HvDsp\\Camera\\Ctrl", "Addr",
        cCamCfgParam.szIP, (100-1),
        "ʶ�����IP", "", INNER_LEVEL
    );
    cCamCfgParam.iCamType = 0;
    HvParamReadIntWithWrite(
        pcParamStore,
        "\\HvDsp\\Camera\\Ctrl", "ProtocolType",
        &cCamCfgParam.iCamType, cCamCfgParam.iCamType,
        0, 3,
        "Э������", "0:һ���Э��; 1:����ר��Э��1; 2:����ר��Э��2; 3:����ר��Э��3;", INNER_LEVEL
    );
#endif
    HvParamReadIntWithWrite(
        pcParamStore,
        "\\HvDsp\\Camera\\Ctrl", "DynamicCfgEnable",
        &cCamCfgParam.iDynamicCfgEnable, cCamCfgParam.iDynamicCfgEnable,
        0, 1,
        "��̬���ò���", "", CUSTOM_LEVEL
    );

#ifdef SINGLE_BOARD_PLATFORM
    HvParamReadIntWithWrite(
        pcParamStore,
        "\\HvDsp\\Camera\\Ctrl", "DynamicTriggerEnable",
        &cCamCfgParam.iDynamicTriggerEnable, cCamCfgParam.iDynamicTriggerEnable,
        0, 1,
        "��Ƶ����ץ�����", "ֻ�Դ���Jϵ�������Ч", CUSTOM_LEVEL
    );
    HvParamReadIntWithWrite(
        pcParamStore,
        "\\HvDsp\\Camera\\Ctrl", "TriggerDelay",
        &cCamCfgParam.iTriggerDelay, cCamCfgParam.iTriggerDelay,
        0, 65535,
        "������ʱ(ms)", "", CUSTOM_LEVEL
    );
#else
    HvParamReadIntWithWrite(
        pcParamStore,
        "\\HvDsp\\Camera\\Ctrl", "DynamicTriggerEnable",
        &cCamCfgParam.iDynamicTriggerEnable, cCamCfgParam.iDynamicTriggerEnable,
        0, 1,
        "��Ƶ����ץ��", "", CUSTOM_LEVEL
    );
#endif

    HvParamReadIntWithWrite(
        pcParamStore,
        "\\HvDsp\\Camera\\Ctrl", "AGCEnable",
        &cCamCfgParam.iEnableAGC, cCamCfgParam.iEnableAGC,
        0, 1,
        "ʹ��AGC", "", CUSTOM_LEVEL
    );
    HvParamReadIntWithWrite(
        pcParamStore,
        "\\HvDsp\\Camera\\Ctrl", "AutoParamEnable",
        &cCamCfgParam.iAutoParamEnable, cCamCfgParam.iAutoParamEnable,
        0, 1,
        "�Զ�����������", "", CUSTOM_LEVEL
    );
    if (cCamCfgParam.iAutoParamEnable == 1)
    {
        pcParamStore->SetInt("\\HvDsp\\Camera\\Ctrl", "AutoParamEnable", 0);
    }

    HvParamReadIntWithWrite(
        pcParamStore,
        "\\HvDsp\\Camera\\Ctrl", "MaxAGCLimit",
        &cCamCfgParam.iMaxAGCLimit, cCamCfgParam.iMaxAGCLimit,
        0, 255,
        "���AGC����ֵ", "", CUSTOM_LEVEL
    );
    HvParamReadIntWithWrite(
        pcParamStore,
        "\\HvDsp\\Camera\\Ctrl", "MinAGCLimit",
        &cCamCfgParam.iMinAGCLimit, cCamCfgParam.iMinAGCLimit,
        0, 255,
        "��СAGC����ֵ", "", CUSTOM_LEVEL
    );
    HvParamReadFloatWithWrite(
        pcParamStore,
        "\\HvDsp\\Camera\\Ctrl", "DB",
        &cCamCfgParam.fltDB, cCamCfgParam.fltDB,
        1.0f, 10.0f,
        "����", "", CUSTOM_LEVEL
    );
    cCamCfgParam.iMinExposureTime = 0;
    cCamCfgParam.iMinGain = 60;
    cCamCfgParam.iMaxExposureTime = 4500;
    cCamCfgParam.iMaxGain = 180;
    HvParamReadIntWithWrite(
        pcParamStore,
        "\\HvDsp\\Camera\\Ctrl", "MinExposureTime",
        &cCamCfgParam.iMinExposureTime, cCamCfgParam.iMinExposureTime,
        0, 54000,
        "��С�ع�ʱ��", "", CUSTOM_LEVEL
    );
    HvParamReadIntWithWrite(
        pcParamStore,
        "\\HvDsp\\Camera\\Ctrl", "MinPlus",
        &cCamCfgParam.iMinGain, cCamCfgParam.iMinGain,
        0, 360,
        "��С����", "", CUSTOM_LEVEL
    );
    HvParamReadIntWithWrite(
        pcParamStore,
        "\\HvDsp\\Camera\\Ctrl", "MaxExposureTime",
        &cCamCfgParam.iMaxExposureTime, cCamCfgParam.iMaxExposureTime,
        0, 54000,
        "����ع�ʱ��", "", CUSTOM_LEVEL
    );
    HvParamReadIntWithWrite(
        pcParamStore,
        "\\HvDsp\\Camera\\Ctrl", "MaxPlus",
        &cCamCfgParam.iMaxGain, cCamCfgParam.iMaxGain,
        0, 360,
        "�������", "", CUSTOM_LEVEL
    );

    char szText[255] = {0};
    char szChnText[255] = {0};
    for (int i = 0; i < MAX_LEVEL_COUNT; i++)
    {
        sprintf(szText, "\\HvDsp\\Camera\\%s", g_rgszLightType[i]);
        sprintf(szChnText, "%d���ع�ʱ��", i);
        HvParamReadIntWithWrite(
            pcParamStore,
            szText, "ExposureTime",
            &cCamCfgParam.irgExposureTime[i], cCamCfgParam.irgExposureTime[i],
            -1, 54000,
            szChnText, "", CUSTOM_LEVEL
        );

        sprintf(szChnText, "%d������", i);
        HvParamReadIntWithWrite(
            pcParamStore,
            szText, "Plus",
            &cCamCfgParam.irgGain[i], cCamCfgParam.irgGain[i],
            -1, 360,
            szChnText, "", CUSTOM_LEVEL
        );

        sprintf(szChnText, "%d��AGC��������", i);
        HvParamReadIntWithWrite(
            pcParamStore,
            szText, "AGCLimit",
            &cCamCfgParam.irgAGCLimit[i], cCamCfgParam.irgAGCLimit[i],
            -1, 255,
            szChnText, "", CUSTOM_LEVEL
        );
#ifdef SINGLE_BOARD_PLATFORM
        sprintf(szChnText, "%d������", i);
        HvParamReadIntWithWrite(
            pcParamStore,
            szText, "Brightness",
            &cCamCfgParam.irgBrightness[i], cCamCfgParam.irgBrightness[i],
            -75, 75,
            szChnText, "", CUSTOM_LEVEL
        );

        sprintf(szChnText, "%d��ģʽ", i);
        HvParamReadStringWithWrite(
            pcParamStore,
            szText, "Mode",
            cCamCfgParam.rgszMode[i], 10,
            szChnText, "�ٶ�, ����, ƽ��, ����", PROJECT_LEVEL
        );
#endif
    }

    if (cCamCfgParam.iAutoParamEnable == 1)
    {
        InitCamCfgParam(pcParamStore, &cCamCfgParam);
    }

    HvParamReadIntWithWrite(
        pcParamStore,
        "HvDsp\\Camera\\Ctrl", "AddrPort",
        &cCamCfgParam.nTestProtocolPort, cCamCfgParam.nTestProtocolPort,
        8100, 8999,
        "����Э��˿�", "",
#ifdef SINGLE_BOARD_PLATFORM
        CUSTOM_LEVEL
#else
        INNER_LEVEL
#endif
    );

    return 0;
}

int GetRecogParamCountPart()
{
    return ARRSIZE(g_pszPartWorkModeList);
}

int GetRecogParamIndexOnNamePart(char *pszModeName)
{
    int nIndex = -1, nCount = GetRecogParamCountPart();
    for (int i = 0; i < nCount; i++)
    {
        if (strcmp(g_pszPartWorkModeList[i], pszModeName) == 0)
        {
            nIndex = i;
            break;
        }
    }

    return nIndex;
}

HRESULT GetRecogParamNameOnIndexPart(DWORD32 dwIndex, char *pszModeName)
{
    if ((int)dwIndex >= GetRecogParamCountPart())
    {
        return E_FAIL;
    }

    if (pszModeName == NULL)
    {
        return E_POINTER;
    }

    strcpy(pszModeName, g_pszPartWorkModeList[dwIndex]);

    return S_OK;
}

HRESULT SetWorkModePart(DWORD32 dwWorkMode)
{
    char szModeName[128] = {0};
    DWORD32 dwWorkModeCount = 0;

    if ((int)dwWorkMode >= GetRecogParamCountPart())
    {
        return E_FAIL;
    }

    GetPlateRecogParamCount(&dwWorkModeCount);

    //ͨ�����ͺ�ģʽ������ȡģʽ��
    if (S_OK != GetRecogParamNameOnIndexPart(dwWorkMode, szModeName))
    {
        return E_FAIL;
    }

    //ͨ��ģʽ����ȡ����ǰ��ģʽ����
    dwWorkMode = GetRecogParamIndexOnName(szModeName);

    HRESULT hr = HvParamWriteInt(
                     &g_cParamStore,
                     "\\Tracker", "PlateRecogMode",
                     dwWorkMode, 0, 0, dwWorkModeCount - 1,
                     "����ģʽ����", "",
                     INNER_LEVEL, TRUE
                 );

    return hr;
}
