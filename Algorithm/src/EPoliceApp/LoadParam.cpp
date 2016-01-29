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
    "���Ӿ���"
};
#ifdef SINGLE_BOARD_PLATFORM
extern const char *PSZ_DSP_BUILD_NO;
extern const char *DSP_BUILD_DATE;
extern const char *DSP_BUILD_TIME;
#endif
CEprFileStorage g_cFileStorage(PARAM_START_ADDR, PARAM_SIZE);
CParamStore g_cParamStore;

int g_nSignal[100] = {0};

HRESULT LoadModuleParam(ModuleParams& cModuleParams)
{
    if (FAILED(g_cFileStorage.Initialize("")))
    {
        HV_Trace(5, "Open param file failed, can't save setting!!!\n");
    }
    g_cParamStore.Initialize(&g_cFileStorage, true);
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

    // ��ȡUBOOT�µ�IP�������Լ�����
    SetIpFromEEPROM();

#endif

    LoadTcpipParam_1(&g_cParamStore, cModuleParams.cTcpipCfgParam_1);
    LoadTcpipParam_2(&g_cParamStore, cModuleParams.cTcpipCfgParam_2);
    //����ģʽһ��Ҫ���ȼ��أ��������Ĳ����п��ܼ��ش�
    LoadWorkModeParam(&g_cParamStore, cModuleParams);
    LoadCamCfgParam(&g_cParamStore, cModuleParams.cCamCfgParam);
    LoadTrackerCfgParam(&g_cParamStore, cModuleParams.cTrackerCfgParam);
    LoadResultSenderParam(&g_cParamStore, cModuleParams.cResultSenderParam);
    LoadSignalMatchParam(&g_cParamStore, cModuleParams.cSignalMatchParam);
    LoadEventDetectCfgParam(&g_cParamStore, cModuleParams.cTrackerCfgParam, cModuleParams.cResultSenderParam);

    // ��ģ��֮���������
    cModuleParams.cResultSenderParam.iEddyType = cModuleParams.cTrackerCfgParam.iEddyType;
    cModuleParams.cTrackerCfgParam.iRoadNumberBegin = cModuleParams.cSignalMatchParam.iRoadNumberBegin;
    cModuleParams.cTrackerCfgParam.iStartRoadNum = cModuleParams.cSignalMatchParam.iStartRoadNum;
    cModuleParams.cSignalMatchParam.fEnableFlashLight = cModuleParams.cTrackerCfgParam.fEnableFlashLight;
    cModuleParams.cSignalMatchParam.fEPolice = TRUE;

    //��Ƶ�����ҪУ������
    cModuleParams.cTrackerCfgParam.cVideoDet.iEPOutPutDetectLine = 100 - cModuleParams.cTrackerCfgParam.cVideoDet.iEPOutPutDetectLine;
    cModuleParams.cTrackerCfgParam.cVideoDet.iEPFGDetectLine -= cModuleParams.cTrackerCfgParam.cVideoDet.rcVideoDetArea.top;
    if(cModuleParams.cTrackerCfgParam.cVideoDet.iEPFGDetectLine < 0)
    {
        cModuleParams.cTrackerCfgParam.cVideoDet.iEPFGDetectLine = 0;
    }
    int width = cModuleParams.cTrackerCfgParam.cVideoDet.rcVideoDetArea.bottom - cModuleParams.cTrackerCfgParam.cVideoDet.rcVideoDetArea.top;
    if(width <= 0)
    {
        width = 100;
    }
    cModuleParams.cTrackerCfgParam.cVideoDet.iEPFGDetectLine *= 100.0f / width;
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

    /*HvParamReadIntWithWrite(
         pcParamStore, "\\HvDsp\\Misc", "RecordFreeSpace",
         &cResultSenderParam.iRecordFreeSpacePerDisk, cResultSenderParam.iRecordFreeSpacePerDisk,
         0, 100,
         "�ɿ��Դ洢Ӳ�̿ռ�����", "",
         PROJECT_LEVEL);

     HvParamReadIntWithWrite(
         pcParamStore, "\\HvDsp\\Misc", "VideoFreeSpace",
         &cResultSenderParam.iVideoFreeSpacePerDisk, cResultSenderParam.iVideoFreeSpacePerDisk,
         0, 100,
         "��Ƶ¼��洢Ӳ�̿ռ�����", "",
         PROJECT_LEVEL);*/

    HvParamReadIntWithWrite(
        pcParamStore, "\\HvDsp\\Misc", "OutputObservedFrames",
        &cResultSenderParam.fOutputObservedFrames, cResultSenderParam.fOutputObservedFrames,
        0, 1,
        "�����Ч֡������", "",
        PROJECT_LEVEL);

    cResultSenderParam.fOutputCarArriveTime = 1;
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
    /*HvParamReadFloatWithWrite(
        pcParamStore, "\\HvDsp\\Misc", "CarAGCLinkage",
        &cResultSenderParam.fltCarAGCLinkage, cResultSenderParam.fltCarAGCLinkage,
        0.0, 100,
        "��������AGC������ֵ", "0��ʾAGC���복������������0��ʾ5�����ڵĳ��������ڵ��ڸ�ֵʱ����AGC������ʱ�ر�AGC",
        CUSTOM_LEVEL);
    HvParamReadIntWithWrite(
        pcParamStore, "\\HvDsp\\Misc", "AWBLinkage",
        &cResultSenderParam.iAWBLinkage, cResultSenderParam.iAWBLinkage,
        0, 1,
        "����������AWB��������", "",
        CUSTOM_LEVEL);*/
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
    //������Ҫ���ӵ�һ���ȥ
    HvParamReadIntWithWrite(
        pcParamStore, "\\HvDsp\\Misc", "OutputOnlyPeccancy",
        &cResultSenderParam.iOutputOnlyPeccancy, cResultSenderParam.iOutputOnlyPeccancy,
        0, 3,
        "ֻ���Υ�½����1:ֻ���Υ�³��� 2:ֻ�����Υ�½�� 3:������ʷ����ʱ,��Υ������ı�,Υ������ı���ͼƬ��", "",
        CUSTOM_LEVEL);

    HvParamReadIntWithWrite(
        pcParamStore, "\\HvDsp\\Misc", "SendRecordSpace",
        &cResultSenderParam.iSendRecordSpace, cResultSenderParam.iSendRecordSpace,
        0, 100,
        "������ͼ��(*100ms)", "",
        CUSTOM_LEVEL);

    HvParamReadIntWithWrite(
        pcParamStore, "\\HvDsp\\Misc", "SendHisVideoSpace",
        &cResultSenderParam.iSendHisVideoSpace, cResultSenderParam.iSendHisVideoSpace,
        1, 20,
        "��ʷ¼���ͼ��(*100ms)", "",
        CUSTOM_LEVEL);

    /*HvParamReadIntWithWrite(
        pcParamStore, "\\HvDsp\\VideoProc", "EnableCrop",
        &cResultSenderParam.iSaveType, cResultSenderParam.iSaveType,
        0, 3,
        "������淽ʽ", "",
        CUSTOM_LEVEL);
    HvParamReadIntWithWrite(
        pcParamStore, "\\HvDsp\\VideoProc", "Width",
        &cResultSenderParam.iWidth, cResultSenderParam.iWidth,
        180, 8192,
        "��ͼ���", "",
        CUSTOM_LEVEL);
    HvParamReadIntWithWrite(
        pcParamStore, "\\HvDsp\\VideoProc", "Height",
        &cResultSenderParam.iHeight, cResultSenderParam.iHeight,
        144, 8192,
        "��ͼ�߶�", "",
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
    */
    HvParamReadIntWithWrite(
        pcParamStore, "\\HvDsp\\VideoProc\\MainVideo", "OutputCaptureImage",
        &cResultSenderParam.iOutputCaptureImage, cResultSenderParam.iOutputCaptureImage,
        0, 1,
        "��Υ�³����������ץ��ͼ", "",
        CUSTOM_LEVEL);

    cResultSenderParam.iFilterUnSurePeccancy = 1;
    HvParamReadIntWithWrite(
        pcParamStore, "\\HvDsp\\Misc", "FilterUnSurePeccancy",
        &cResultSenderParam.iFilterUnSurePeccancy, cResultSenderParam.iFilterUnSurePeccancy,
        0, 1,
        "���˲��ϸ��Υ�¼�¼", "",
        PROJECT_LEVEL);
    HvParamReadIntWithWrite(
        pcParamStore,
        "\\Tracker\\TrafficLight", "OutputPeccancyType",
        &cResultSenderParam.nOutputPeccancyType, cResultSenderParam.nOutputPeccancyType,
        0, 1,
        "���Υ�����ͷ�ʽ", "0-�������ȼ����;1-�������Υ������", CUSTOM_LEVEL
    );
    HvParamReadStringWithWrite(
        pcParamStore,
        "\\Tracker\\TrafficLight", "OutputPeccancyPriority",
        cResultSenderParam.szPeccancyPriority,
        sizeof(cResultSenderParam.szPeccancyPriority),
        "Υ���������ȼ�", "Υ�����ȼ�����", CUSTOM_LEVEL
    );

    cResultSenderParam.nRedPosBrightness = 55;
    HvParamReadIntWithWrite(
        pcParamStore, "\\HvDsp\\Misc", "RedPosBrightness",
        &cResultSenderParam.nRedPosBrightness, cResultSenderParam.nRedPosBrightness,
        0, 255,
        "���ͺ��λ�û���������ֵ", "",
        PROJECT_LEVEL);

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
    {"\\OuterCtrl", "PlateHoldTime", (int*)&g_cSignalMatchParam.dwPlateHoldTime, 2000, 0, 600000, "���Ʊ���ʱ��", "", CUSTOM_LEVEL},
    {"\\OuterCtrl", "SignalHoldTime", (int*)&g_cSignalMatchParam.dwSignalHoldTime, 2000, 0, 600000, "�źű���ʱ��", "", CUSTOM_LEVEL},
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
    /*
        HvParamReadIntWithWrite(
            pcParamStore,
            "\\OuterCtrl", "CapImgCount",
            &cSignalMatchParam.iCapImgCount,
            cSignalMatchParam.iCapImgCount, 1, 2,
            "ץ��ͼ����", "",
            CUSTOM_LEVEL
        );
    */
    HvParamReadIntWithWrite(
        pcParamStore,
        "\\OuterCtrl\\OtherSignalSource", "SoftTrigger",
        &cSignalMatchParam.nSoftTrigSignalSource,
        cSignalMatchParam.nSoftTrigSignalSource, -1, 65535,
        "�����ź�Դ���", "",
        CUSTOM_LEVEL
    );

    HvParamReadIntWithWrite(
        pcParamStore,
        "\\Tracker\\RoadInfo", "RoadNumberBegin",
        &cSignalMatchParam.iRoadNumberBegin,
        cSignalMatchParam.iRoadNumberBegin, 0, 1,
        "���������ʼ����", "0:����ʼ,1:���ҿ�ʼ",
        CUSTOM_LEVEL
    );

    HvParamReadIntWithWrite(
        pcParamStore,
        "\\Tracker\\RoadInfo", "StartRoadNumber",
        &cSignalMatchParam.iStartRoadNum,
        cSignalMatchParam.iStartRoadNum, 0, 9,
        "��������ʼ���", "0:��0��ʼ,1:��1��ʼ,�Դ�����...",
        CUSTOM_LEVEL
    );
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
            CUSTOM_LEVEL
        );
        if (cSignalMatchParam.rgMatchSigType[cSignalMatchParam.rgSigSource[i].nType].fMainSignal)
        {
            cSignalMatchParam.nMainSourceNum++;
        }
    }
#ifndef SINGLE_BOARD_PLATFORM
    HvParamReadIntWithWrite(
            pcParamStore,
            "\\OuterCtrl", "SpeedDecide",
            (int *)&cSignalMatchParam.fSpeedDecide,
            cSignalMatchParam.fSpeedDecide, 0, 1,
            "�Ƿ������������У��", "0-��;1-��",
            CUSTOM_LEVEL
        );

    HvParamReadIntWithWrite(
        pcParamStore,
        "\\OuterCtrl", "IPTEnable",
        &cSignalMatchParam.fEnableIPT,
        cSignalMatchParam.fEnableIPT, 0, 1,
        "IPTʹ�ܿ���", "",
        CUSTOM_LEVEL
    );
    HvParamReadIntWithWrite(
        pcParamStore,
        "\\OuterCtrl", "RadarType",
        &cSignalMatchParam.iRadarType,
        cSignalMatchParam.iRadarType, 0, 1,
        "����״�����", "0-������״�,1-�����״�",
        PROJECT_LEVEL
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

int LoadEventDetectCfgParam(
    CParamStore* pcParamStore,
    TRACKER_CFG_PARAM& cTrackerCfgParam,
    ResultSenderParam& cResultSenderParam
)
{
    char szSection[64];
    char szChName[256];
    //Ĭ��һ����,�ƹ����޸�
    cResultSenderParam.iCheckEventTime = 1;
    HvParamReadIntWithWrite(
        pcParamStore, "HvDsp\\EventChecker", "CheckEventTime",
        &cResultSenderParam.iCheckEventTime, cResultSenderParam.iCheckEventTime,
        0, 5,
        "�¼��������(��λ:����)", "",
        PROJECT_LEVEL
    );
    //���Ӿ��죬Ĭ�����м�⿪��Ϊ��
    cTrackerCfgParam.nDetReverseRunEnable = 1;
    HvParamReadIntWithWrite(
        pcParamStore, "\\Tracker\\DetReverseRun", "DetReverseRunEnable",
        &cTrackerCfgParam.nDetReverseRunEnable, cTrackerCfgParam.nDetReverseRunEnable,
        0, 1,
        "���м�⿪��", "",
        PROJECT_LEVEL
    );

    if (cTrackerCfgParam.nDetReverseRunEnable)
    {
        HvParamReadIntWithWrite(
            pcParamStore, "\\Tracker\\DetReverseRun", "Span",
            &cTrackerCfgParam.nSpan, cTrackerCfgParam.nSpan,
            -100, 100,
            "�����������г������", "",
            CUSTOM_LEVEL
        );
    }

    HvParamReadFloatWithWrite(
        pcParamStore,
        "\\HvDsp\\EventChecker", "OverLineSensitivity",
        &cTrackerCfgParam.fltOverLineSensitivity, cTrackerCfgParam.fltOverLineSensitivity,
        0.5, 5.0,
        "ѹ�߼��������", "", CUSTOM_LEVEL
    );

    HvParamReadIntWithWrite(
        pcParamStore, "\\HvDsp\\EventChecker", "SpeedLimit",
        &cResultSenderParam.iSpeedLimit, cResultSenderParam.iSpeedLimit,
        0, 1000,
        "�ٶ�����ֵ(km/h)", "",
        CUSTOM_LEVEL
    );

    for (int i = 1; i < MAX_ROADLINE_NUM - 1; i++)
    {
        sprintf(szSection, "IsCrossLine%d%d", i, i + 1);
        sprintf(szChName, "������%d���Խ��", i + 1);
        HvParamReadIntWithWrite(
            pcParamStore,
            "\\Tracker\\ActionDetect", szSection,
            &cTrackerCfgParam.cActionDetect.iIsCrossLine[i],
            cTrackerCfgParam.cActionDetect.iIsCrossLine[i],
            0, 1,
            szChName, "", CUSTOM_LEVEL
        );
    }

    for (int i = 0; i < MAX_ROADLINE_NUM; i++)
    {
        sprintf(szSection, "IsYellowLine%d%d", i, i + 1);
        sprintf(szChName, "������%d���ѹ��", i + 1);
        HvParamReadIntWithWrite(
            pcParamStore,
            "\\Tracker\\ActionDetect", szSection,
            &cTrackerCfgParam.cActionDetect.iIsYellowLine[i],
            cTrackerCfgParam.cActionDetect.iIsYellowLine[i],
            0, 2,
            szChName, "0:���ж�ѹ��,1:ѹʵ��,2:ѹ����", CUSTOM_LEVEL
        );
    }
}

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
    TRACKER_CFG_PARAM& cTrackerCfgParam
)
{
//#ifdef SINGLE_BOARD_PLATFORM
    HvParamReadIntWithWrite(
        pcParamStore,
        "\\HvDsp\\Misc", "EddyType",
        &cTrackerCfgParam.iEddyType, cTrackerCfgParam.iEddyType,
        0, 1,
        "ͼƬ��ת(0:����ת,1:��ʱ����ת90��)", "",
#ifdef SINGLE_BOARD_PLATFORM
        PROJECT_LEVEL
#else
        CUSTOM_LEVEL
#endif
    );
    HvParamReadIntWithWrite(
        pcParamStore,
        "HvDsp\\Misc", "OutputCarSize",
        &cTrackerCfgParam.fOutputCarSize, cTrackerCfgParam.fOutputCarSize,
        0, 1,
        "��������ߴ�", "", PROJECT_LEVEL
    );
//#endif
    HvParamReadIntWithWrite(
        pcParamStore,
        "\\Tracker\\DoublePlate", "DoublePlateTypeEnable",
        &cTrackerCfgParam.nDoublePlateTypeEnable, cTrackerCfgParam.nDoublePlateTypeEnable,
        0, 3,
        "Ĭ��˫�������Ϳ���", "", CUSTOM_LEVEL
    );

    HvParamReadIntWithWrite(
        pcParamStore,
        "\\Tracker\\DoublePlate", "DefaultType",
        &cTrackerCfgParam.nDefaultDBType, cTrackerCfgParam.nDefaultDBType,
        0, 1,
        "Ĭ��˫��������", "", CUSTOM_LEVEL
    );

    HvParamReadIntWithWrite(
        pcParamStore,
        "\\Tracker\\DoublePlate", "MiddleWidth",
        &cTrackerCfgParam.nMiddleWidth, cTrackerCfgParam.nMiddleWidth,
        0, 300,
        "��Ļ�м䵥�����ƺ����ֿ��", "", CUSTOM_LEVEL
    );

    HvParamReadIntWithWrite(
        pcParamStore,
        "\\Tracker\\DoublePlate", "BottomWidth",
        &cTrackerCfgParam.nBottomWidth, cTrackerCfgParam.nBottomWidth,
        0, 300,
        "��Ļ�ײ��������ƺ����ֿ��", "", CUSTOM_LEVEL
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

    cTrackerCfgParam.nEnableProcessBWPlate = 1;
    HvParamReadIntWithWrite(
        pcParamStore,
        "\\Tracker\\ProcessPlate", "EnableProcessBWPlate",
        &cTrackerCfgParam.nEnableProcessBWPlate, cTrackerCfgParam.nEnableProcessBWPlate,
        0, 1,
        "�Ƿ����ڰ����", "", PROJECT_LEVEL
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

    HvParamReadIntWithWrite(
        pcParamStore,
        "\\Tracker\\ProcessPlate\\BlackPlate", "CtrlCpl",
        &cTrackerCfgParam.nCtrlCpl, cTrackerCfgParam.nCtrlCpl,
        0, 1,
        "����ƫ�⾵", "",
#ifndef SINGLE_BOARD_PLATFORM
        CUSTOM_LEVEL
#else
        PROJECT_LEVEL
#endif
    );

    HvParamReadIntWithWrite(
        pcParamStore,
        "\\Tracker\\ProcessPlate\\BlackPlate", "LightTypeCpl",
        &cTrackerCfgParam.nLightTypeCpl, cTrackerCfgParam.nLightTypeCpl,
        0, 12,
        "����ƫ�⾵�����ȵȼ�", "",
#ifndef SINGLE_BOARD_PLATFORM
        CUSTOM_LEVEL
#else
        PROJECT_LEVEL
#endif
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
        "����ǿ��Ϩ�𲹹�ƻ���������ֵ", "", PROJECT_LEVEL
    );

    HvParamReadIntWithWrite(
        pcParamStore,
        "HvDsp\\Misc", "DisableFlashAtDay",
        &cTrackerCfgParam.iDisableFlashAtDay, cTrackerCfgParam.iDisableFlashAtDay,
        0, 1,
        "����ǿ��Ϩ�������", "", CUSTOM_LEVEL
    );

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
        "���Ʊ��Ͷ�����", "", PROJECT_LEVEL
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
        "��ǿ����ͼʶ��", "", PROJECT_LEVEL
    );
    cTrackerCfgParam.fEnableT1Model = 1;
    HvParamReadIntWithWrite(
        pcParamStore,
        "Tracker\\Recognition","EnableT1Model",
        &cTrackerCfgParam.fEnableT1Model, cTrackerCfgParam.fEnableT1Model,
        0, 1,
        "T-1ģ�Ϳ���", "", PROJECT_LEVEL
    );
    cTrackerCfgParam.fEnableDefaultWJ =1;
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
        "Tracker\\Misc", "CarArrivedPosNoPlate",
        &cTrackerCfgParam.nCarArrivedPosNoPlate, cTrackerCfgParam.nCarArrivedPosNoPlate,
        0, 100,
        "���Ƴ�����λ��", "", CUSTOM_LEVEL
    );
    HvParamReadIntWithWrite(
        pcParamStore,
        "Tracker\\Misc", "CarArrivedPosYellow",
        &cTrackerCfgParam.nCarArrivedPosYellow, cTrackerCfgParam.nCarArrivedPosYellow,
        0, 100,
        "��������λ��(����)", "", CUSTOM_LEVEL
    );
    HvParamReadIntWithWrite(
        pcParamStore,
        "Tracker\\Misc", "CoreProcessType",
        &cTrackerCfgParam.nProcessType, cTrackerCfgParam.nProcessType,
        0, 2,
        "�������� 0:ֻ���ִ���С�ͳ� 1:��������ַǻ��������� 2:��������ַǻ�����������", "", PROJECT_LEVEL
    );
    HvParamReadIntWithWrite(
        pcParamStore,
        "Tracker\\Misc", "CoreOutPutType",
        &cTrackerCfgParam.nOutPutType, cTrackerCfgParam.nOutPutType,
        0, 1,
        "������� 0:ȫ������� 1:����С�ͳ����Ϊ������", "", PROJECT_LEVEL
    );
    HvParamReadIntWithWrite(
        pcParamStore,
        "Tracker\\Misc", "NightPlus",
        &cTrackerCfgParam.nNightPlus, cTrackerCfgParam.nNightPlus,
        0, 1,
        "���ϼ�ǿ�ǻ��������,ֻ�г����㹻��ʱ����,��������Ӷ��", "", PROJECT_LEVEL
    );
    HvParamReadIntWithWrite(
        pcParamStore,
        "Tracker\\Misc", "WalkManSpeed",
        &cTrackerCfgParam.nWalkManSpeed, cTrackerCfgParam.nWalkManSpeed,
        0, 300,
        "�����жϵ�������", "", PROJECT_LEVEL
    );
    HvParamReadFloatWithWrite(
        pcParamStore,
        "Tracker\\Misc", "BikeSensitivity",
        &cTrackerCfgParam.fltBikeSensitivity, cTrackerCfgParam.fltBikeSensitivity,
        0.0f, 10.0f,
        "�ǻ�����������", "", PROJECT_LEVEL
    );
    HvParamReadIntWithWrite(
        pcParamStore,
        "Tracker\\Misc", "CarArrivedDelay",
        &cTrackerCfgParam.nCarArrivedDelay, cTrackerCfgParam.nCarArrivedDelay,
        0, 100,
        "�������ﴥ���ӳپ���(��)", "", PROJECT_LEVEL
    );
    HvParamReadIntWithWrite(
        pcParamStore,
        "Tracker\\Misc", "PreVote",
        &cTrackerCfgParam.fPreVote, cTrackerCfgParam.fPreVote,
        0, 1,
        "�Ƿ�����ԤͶƱ", "0-��ʹ��;1-ʹ��", PROJECT_LEVEL
    );
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
    HvParamReadIntWithWrite(
        pcParamStore,
        "Tracker\\TrackInfo", "AverageConfidenceQuan",
        &cTrackerCfgParam.nAverageConfidenceQuan, cTrackerCfgParam.nAverageConfidenceQuan,
        0, 65536,
        "ƽ���÷�����", "", 1
    );
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

    //�ȳ�ʼ��ɨ������ÿ��ģʽ�����Լ���ɨ������
    cTrackerCfgParam.cDetectArea.DetectorAreaLeft = 2;
    cTrackerCfgParam.cDetectArea.DetectorAreaTop = 60;
    cTrackerCfgParam.cDetectArea.DetectorAreaRight = 98;
    cTrackerCfgParam.cDetectArea.DetectorAreaBottom = 100;
    cTrackerCfgParam.cDetectArea.nDetectorMinScaleNum = 6;
    cTrackerCfgParam.cDetectArea.nDetectorMaxScaleNum = 10;
    //��ʼ����������
    cTrackerCfgParam.cTrapArea.TopLeftX = cTrackerCfgParam.cDetectArea.DetectorAreaLeft;
    cTrackerCfgParam.cTrapArea.TopRightX = cTrackerCfgParam.cDetectArea.DetectorAreaRight;
    cTrackerCfgParam.cTrapArea.BottomLeftX = cTrackerCfgParam.cDetectArea.DetectorAreaLeft;
    cTrackerCfgParam.cTrapArea.BottomRightX = cTrackerCfgParam.cDetectArea.DetectorAreaRight;

    cTrackerCfgParam.cTrapArea.TopLeftY = cTrackerCfgParam.cDetectArea.DetectorAreaTop;
    cTrackerCfgParam.cTrapArea.TopRightY = cTrackerCfgParam.cDetectArea.DetectorAreaTop;
    cTrackerCfgParam.cTrapArea.BottomLeftY = cTrackerCfgParam.cDetectArea.DetectorAreaBottom;
    cTrackerCfgParam.cTrapArea.BottomRightY = cTrackerCfgParam.cDetectArea.DetectorAreaBottom;

    cTrackerCfgParam.cTrapArea.nDetectorMinScaleNum = cTrackerCfgParam.cDetectArea.nDetectorMinScaleNum;
    cTrackerCfgParam.cTrapArea.nDetectorMaxScaleNum = cTrackerCfgParam.cDetectArea.nDetectorMaxScaleNum;

    HvParamReadIntWithWrite(
        pcParamStore,
        "Tracker\\DetAreaCtrl\\Normal","EnableTrap",
        &cTrackerCfgParam.cTrapArea.fEnableDetAreaCtrl, cTrackerCfgParam.cTrapArea.fEnableDetAreaCtrl,
        0,1,
        "����ɨ��������ƿ���","",5
    );

    HvParamReadIntWithWrite(
        pcParamStore,
        "Tracker\\DetAreaCtrl\\Normal","TopLeftX",
        &cTrackerCfgParam.cTrapArea.TopLeftX, cTrackerCfgParam.cTrapArea.TopLeftX,
        0,100,
        "ɨ��������������X","",5
    );
    HvParamReadIntWithWrite(
        pcParamStore,
        "Tracker\\DetAreaCtrl\\Normal","TopLeftY",
        &cTrackerCfgParam.cTrapArea.TopLeftY, cTrackerCfgParam.cTrapArea.TopLeftY,
        0,100,
        "ɨ��������������Y","",5
    );
    HvParamReadIntWithWrite(
        pcParamStore,
        "Tracker\\DetAreaCtrl\\Normal","TopRightX",
        &cTrackerCfgParam.cTrapArea.TopRightX, cTrackerCfgParam.cTrapArea.TopRightX,
        0,100,
        "ɨ��������������X","",5
    );
    HvParamReadIntWithWrite(
        pcParamStore,
        "Tracker\\DetAreaCtrl\\Normal","TopRightY",
        &cTrackerCfgParam.cTrapArea.TopRightY, cTrackerCfgParam.cTrapArea.TopRightY,
        0,100,
        "ɨ��������������Y","",5
    );

    HvParamReadIntWithWrite(
        pcParamStore,
        "Tracker\\DetAreaCtrl\\Normal","BottomLeftX",
        &cTrackerCfgParam.cTrapArea.BottomLeftX, cTrackerCfgParam.cTrapArea.BottomLeftX,
        0,100,
        "ɨ��������������X","",5
    );
    HvParamReadIntWithWrite(
        pcParamStore,
        "Tracker\\DetAreaCtrl\\Normal","BottomLeftY",
        &cTrackerCfgParam.cTrapArea.BottomLeftY, cTrackerCfgParam.cTrapArea.BottomLeftY,
        0,100,
        "ɨ��������������Y","",5
    );
    HvParamReadIntWithWrite(
        pcParamStore,
        "Tracker\\DetAreaCtrl\\Normal","BottomRightX",
        &cTrackerCfgParam.cTrapArea.BottomRightX, cTrackerCfgParam.cTrapArea.BottomRightX,
        0,100,
        "ɨ��������������X","",5
    );
    HvParamReadIntWithWrite(
        pcParamStore,
        "Tracker\\DetAreaCtrl\\Normal","BottomRightY",
        &cTrackerCfgParam.cTrapArea.BottomRightY, cTrackerCfgParam.cTrapArea.BottomRightY,
        0,100,
        "ɨ��������������Y","",5
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
    cTrackerCfgParam.cDetectArea.fEnableDetAreaCtrl = cTrackerCfgParam.cTrapArea.fEnableDetAreaCtrl;
    cTrackerCfgParam.cDetectArea.DetectorAreaLeft = cTrackerCfgParam.cTrapArea.TopLeftX > cTrackerCfgParam.cTrapArea.BottomLeftX?cTrackerCfgParam.cTrapArea.BottomLeftX:cTrackerCfgParam.cTrapArea.TopLeftX;
    cTrackerCfgParam.cDetectArea.DetectorAreaRight = cTrackerCfgParam.cTrapArea.TopRightX < cTrackerCfgParam.cTrapArea.BottomRightX?cTrackerCfgParam.cTrapArea.BottomRightX:cTrackerCfgParam.cTrapArea.TopRightX;
    cTrackerCfgParam.cDetectArea.DetectorAreaTop = cTrackerCfgParam.cTrapArea.TopLeftY > cTrackerCfgParam.cTrapArea.TopRightY?cTrackerCfgParam.cTrapArea.TopRightY:cTrackerCfgParam.cTrapArea.TopLeftY;
    cTrackerCfgParam.cDetectArea.DetectorAreaBottom = cTrackerCfgParam.cTrapArea.BottomLeftY > cTrackerCfgParam.cTrapArea.BottomRightY?cTrackerCfgParam.cTrapArea.BottomLeftY:cTrackerCfgParam.cTrapArea.BottomRightY;

    HvParamReadIntWithWrite(
        pcParamStore,
        "\\Tracker\\RoadInfo", "RoadLineNumber",
        &cTrackerCfgParam.nRoadLineNumber,
        cTrackerCfgParam.nRoadLineNumber, 2, MAX_ROADLINE_NUM,
        "����������", "",
        CUSTOM_LEVEL
    );

    HvParamReadIntWithWrite(
        pcParamStore,
        "\\Tracker\\RoadInfo", "OutputRoadTypeNameFlag",
        &cTrackerCfgParam.nOutputRoadTypeName,
        cTrackerCfgParam.nOutputRoadTypeName, 0, 1,
        "����������ͱ�ʶ", "0-�����;1-���",
        CUSTOM_LEVEL
    );

    for (int i = 0; i < MAX_ROADLINE_NUM; i++)
    {
        char szSection[256];
        sprintf(szSection, "\\Tracker\\RoadInfo\\Road%02d", i);

        HvParamReadIntWithWrite(
            pcParamStore,
            szSection, "TopX",
            &cTrackerCfgParam.rgcRoadInfo[i].ptTop.x,
            cTrackerCfgParam.rgcRoadInfo[i].ptTop.x, 0, 5000,
            "��X����", "",
            CUSTOM_LEVEL
        );
        HvParamReadIntWithWrite(
            pcParamStore,
            szSection, "TopY",
            &cTrackerCfgParam.rgcRoadInfo[i].ptTop.y,
            cTrackerCfgParam.rgcRoadInfo[i].ptTop.y, 0, 5000,
            "��Y����", "",
            CUSTOM_LEVEL
        );
        HvParamReadIntWithWrite(
            pcParamStore,
            szSection, "BottomX",
            &cTrackerCfgParam.rgcRoadInfo[i].ptBottom.x,
            cTrackerCfgParam.rgcRoadInfo[i].ptBottom.x, 0, 5000,
            "��X����", "",
            CUSTOM_LEVEL
        );
        HvParamReadIntWithWrite(
            pcParamStore,
            szSection, "BottomY",
            &cTrackerCfgParam.rgcRoadInfo[i].ptBottom.y,
            cTrackerCfgParam.rgcRoadInfo[i].ptBottom.y, 0, 5000,
            "��Y����", "",
            CUSTOM_LEVEL
        );
        HvParamReadIntWithWrite(
            pcParamStore,
            szSection, "LineType",
            &cTrackerCfgParam.rgcRoadInfo[i].iLineType,
            cTrackerCfgParam.rgcRoadInfo[i].iLineType, 1, 9999,
            "����", "",
            CUSTOM_LEVEL
        );
        HvParamReadIntWithWrite(
            pcParamStore,
            szSection, "RoadType",
            &cTrackerCfgParam.rgcRoadInfo[i].iRoadType,
            cTrackerCfgParam.rgcRoadInfo[i].iRoadType, 1, 9999,
            "��Ӧ�ĳ�������", "",
            CUSTOM_LEVEL
        );
        HvParamReadStringWithWrite(
            pcParamStore,
            szSection, "RoadTypeName",
            cTrackerCfgParam.rgcRoadInfo[i].szRoadTypeName,
            sizeof(cTrackerCfgParam.rgcRoadInfo[i].szRoadTypeName),
            "������������", "��ΪNULL����ȡ�ó������͵�������",
            CUSTOM_LEVEL);

        HvParamReadIntWithWrite(
            pcParamStore,
            szSection, "FilterRunType",
            &cTrackerCfgParam.rgcRoadInfo[i].iFilterRunType,
            cTrackerCfgParam.rgcRoadInfo[i].iFilterRunType, 0, 9999,
            "��������(1:ֱ��2:��ת4:��ת8:��ͷ32:����)", "",
            CUSTOM_LEVEL
        );
    }

    cTrackerCfgParam.nCaptureOnePos = 80;
    cTrackerCfgParam.nCaptureTwoPos = 60;
    cTrackerCfgParam.nCaptureThreePos = 45;
    HvParamReadIntWithWrite(
        pcParamStore,
        "\\Tracker\\TrafficLight", "OnePos",
        &cTrackerCfgParam.nCaptureOnePos,
        cTrackerCfgParam.nCaptureOnePos, 0, 100,
        "ץ�ĵ�һ��ͼ��λ��", "",
        CUSTOM_LEVEL
    );
    HvParamReadIntWithWrite(
        pcParamStore,
        "\\Tracker\\TrafficLight", "TwoPos",
        &cTrackerCfgParam.nCaptureTwoPos,
        cTrackerCfgParam.nCaptureTwoPos, 0, 100,
        "ץ�ĵڶ���ͼ��λ��", "",
        CUSTOM_LEVEL
    );
    HvParamReadIntWithWrite(
        pcParamStore,
        "\\Tracker\\TrafficLight", "ThreePos",
        &cTrackerCfgParam.nCaptureThreePos,
        cTrackerCfgParam.nCaptureThreePos, 0, 100,
        "ץ�ĵ�����ͼ��λ��", "",
        CUSTOM_LEVEL
    );
    HvParamReadIntWithWrite(
        pcParamStore,
        "\\Tracker\\TrafficLight", "FilterRushPeccancy",
        &cTrackerCfgParam.cTrafficLight.nFilterRushPeccancy,
        cTrackerCfgParam.cTrafficLight.nFilterRushPeccancy, 0, 1,
        "���˵�������ͼΪ�̵�ʱ��Υ��", "",
        CUSTOM_LEVEL
    );
    cTrackerCfgParam.cTrafficLight.nFilterMoreReview = 1;
    HvParamReadIntWithWrite(
        pcParamStore,
        "\\Tracker\\TrafficLight", "FilterMoreReview",
        &cTrackerCfgParam.cTrafficLight.nFilterMoreReview,
        cTrackerCfgParam.cTrafficLight.nFilterMoreReview, 0, 1,
        "���˴󳵳����쿪��", "",
        CUSTOM_LEVEL
    );
    cTrackerCfgParam.cScaleSpeed.fEnable = 1;
    HvParamReadIntWithWrite(
        pcParamStore,
        "\\Tracker\\ScaleSpeed", "Enable",
        &cTrackerCfgParam.cScaleSpeed.fEnable,
        cTrackerCfgParam.cScaleSpeed.fEnable,
        0, 1,
        "������ٿ���", "", CUSTOM_LEVEL
    );
    //�羯Ĭ��ʹ�ÿռ�궨�������Ҳ����ò�������
    cTrackerCfgParam.cScaleSpeed.nMethodsFlag = 1;
    HvParamReadFloatWithWrite(
        pcParamStore,
        "\\Tracker\\ScaleSpeed", "CameraHigh",
        &cTrackerCfgParam.cScaleSpeed.fltCameraHigh,
        cTrackerCfgParam.cScaleSpeed.fltCameraHigh,
        0.01f, 10000.0f,
        "������߶�", "", CUSTOM_LEVEL
    );
    HvParamReadFloatWithWrite(
        pcParamStore,
        "\\Tracker\\ScaleSpeed", "AdjustCoef",
        &cTrackerCfgParam.cScaleSpeed.fltAdjustCoef,
        cTrackerCfgParam.cScaleSpeed.fltAdjustCoef,
        0.01f, 10.0f,
        "����ϵ��", "", CUSTOM_LEVEL
    );
    for (int i = 0; i < 11; i++)
    {
        char szTransMarixText[32];
        char szTransMarixKey[32];
        sprintf(szTransMarixText, "�任����_%d", i);
        sprintf(szTransMarixKey, "TransMarix_%d", i);
        HvParamReadFloatWithWrite(
            pcParamStore,
            "\\Tracker\\ScaleSpeed", szTransMarixKey,
            &cTrackerCfgParam.cScaleSpeed.rgfltTransMarix[i],
            cTrackerCfgParam.cScaleSpeed.rgfltTransMarix[i],
            -3.40282347e+37F, 3.40282347e+37F,
            szTransMarixText, "", CUSTOM_LEVEL
        );
    }

    HvParamReadIntWithWrite(
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
    );
    HvParamReadIntWithWrite(
        pcParamStore,
        "\\Tracker\\VideoDet", "DayBackgroudThreshold",
        &cTrackerCfgParam.cVideoDet.nDayBackgroudThreshold,
        cTrackerCfgParam.cVideoDet.nDayBackgroudThreshold,
        0, 255,
        "���챳�������ֵ", "", PROJECT_LEVEL
    );
    HvParamReadIntWithWrite(
        pcParamStore,
        "\\Tracker\\VideoDet", "nBackgroudThreshold",
        &cTrackerCfgParam.cVideoDet.nNightBackgroudThreshold,
        cTrackerCfgParam.cVideoDet.nNightBackgroudThreshold,
        0, 255,
        "���ϱ��������ֵ", "", PROJECT_LEVEL
    );
    HvParamReadIntWithWrite(
        pcParamStore,
        "\\Tracker\\VideoDet", "VoteFrameCount",
        &cTrackerCfgParam.cVideoDet.nVoteFrameCount,
        cTrackerCfgParam.cVideoDet.nVoteFrameCount,
        15, 100,
        "ͶƱ��Ч֡������֡�����ڴ�ֵ��ͶƱ", "", PROJECT_LEVEL
    );
    HvParamReadIntWithWrite(
        pcParamStore,
        "\\Tracker\\VideoDet", "CheckAcross",
        &cTrackerCfgParam.cVideoDet.nCheckAcross,
        cTrackerCfgParam.cVideoDet.nCheckAcross,
        0, 1,
        "�Ƿ��������ʻ������", "", PROJECT_LEVEL
    );
    HvParamReadIntWithWrite(
        pcParamStore,
        "\\Tracker\\VideoDet", "ShadowDetMode",
        &cTrackerCfgParam.cVideoDet.nShadowDetMode,
        cTrackerCfgParam.cVideoDet.nShadowDetMode,
        0, 1,
        "��Ӱ���ģʽ", "0:��ʹ����Ӱ���, 1:ʹ����Ӱ���", PROJECT_LEVEL
    );
    HvParamReadIntWithWrite(
        pcParamStore,
        "\\Tracker\\VideoDet", "ShadowDetEffect",
        &cTrackerCfgParam.cVideoDet.nShadowDetEffect,
        cTrackerCfgParam.cVideoDet.nShadowDetEffect,
        0, 100,
        "��Ӱ���ǿ���̶�ֵ", "0:ʹ��ȱʡֵ(����ȷ), 1:��Ӱ���ǿ���̶���С, 100:��Ӱ���ǿ���̶����", PROJECT_LEVEL
    );
    HvParamReadIntWithWrite(
        pcParamStore,
        "\\Tracker\\VideoDet", "DiffType",
        &cTrackerCfgParam.cVideoDet.nDiffType,
        cTrackerCfgParam.cVideoDet.nDiffType,
        0, 1,
        "ǰ����ȡ��ʽ", "ֵΪ0:֡����ȡ ֵΪ1:֡���sobelǰ������Ը���·����������������", PROJECT_LEVEL
    );
    HvParamReadIntWithWrite(
        pcParamStore,
        "\\Tracker\\VideoDet", "MedFilter",
        &cTrackerCfgParam.cVideoDet.nMedFilter,
        cTrackerCfgParam.cVideoDet.nMedFilter,
        0, 1,
        "���ƶ�������", "ֵΪ0:���ƶ����ر�  ֵΪ1:���ƶ�����", PROJECT_LEVEL
    );
    HvParamReadIntWithWrite(
        pcParamStore,
        "\\Tracker\\VideoDet", "MedFilterUpLine",
        &cTrackerCfgParam.cVideoDet.nMedFilterUpLine,
        cTrackerCfgParam.cVideoDet.nMedFilterUpLine,
        0, 100,
        "���ƶ�����������������", "ֵΪ0:��Ļ���Ϸ� ֵΪ100:��Ļ���·�", PROJECT_LEVEL
    );
    HvParamReadIntWithWrite(
        pcParamStore,
        "\\Tracker\\VideoDet", "MedFilterDownLine",
        &cTrackerCfgParam.cVideoDet.nMedFilterDownLine,
        cTrackerCfgParam.cVideoDet.nMedFilterDownLine,
        0, 100,
        "���ƶ�����������������", "ֵΪ0:��Ļ���Ϸ� ֵΪ100:��Ļ���·�", PROJECT_LEVEL
    );
    HvParamReadIntWithWrite(
        pcParamStore,
        "\\Tracker\\TrafficLight", "EnableFlashLight",
        &cTrackerCfgParam.fEnableFlashLight, cTrackerCfgParam.fEnableFlashLight,
        0, 1,
        "ץ�Ĳ����ģʽ����", "", CUSTOM_LEVEL
    );
    HvParamReadIntWithWrite(
        pcParamStore,
        "\\Tracker\\TrafficLight", "FlashLightThreshold",
        &cTrackerCfgParam.nFlashLightThreshold, cTrackerCfgParam.nFlashLightThreshold,
        0, 255,
        "ץ�Ĳ���������ֵ", "", CUSTOM_LEVEL
    );
    cTrackerCfgParam.cVideoDet.iNoPlateDetMode = 1;
    HvParamReadIntWithWrite(
        pcParamStore,
        "\\Tracker\\VideoDet", "NoPlateDetMode",
        &cTrackerCfgParam.cVideoDet.iNoPlateDetMode,
        cTrackerCfgParam.cVideoDet.iNoPlateDetMode,
        0, 1,
        "���Ƴ����ģʽ (0:��׼ģʽ 1:ģ��ģʽ)", "", CUSTOM_LEVEL
    );

    HvParamReadIntWithWrite(
        pcParamStore,
        "\\Tracker\\VideoDet", "EPOutPutDetectLine",
        &cTrackerCfgParam.cVideoDet.iEPOutPutDetectLine,
        cTrackerCfgParam.cVideoDet.iEPOutPutDetectLine,
        0, 100,
        "���Ӿ���������ٽ�����", "", PROJECT_LEVEL
    );

    HvParamReadIntWithWrite(
        pcParamStore,
        "\\Tracker\\VideoDet", "EPFGDetectLine",
        &cTrackerCfgParam.cVideoDet.iEPFGDetectLine,
        cTrackerCfgParam.cVideoDet.iEPFGDetectLine,
        0, 100,
        "�羯���Ƴ������������(���붥���ٷֱ�)", "", PROJECT_LEVEL
    );
/*
    HvParamReadIntWithWrite(
        pcParamStore,
        "\\Tracker\\VideoDet", "EPUseCarTailModel",
        &cTrackerCfgParam.cVideoDet.iEPUseCarTailModel,
        cTrackerCfgParam.cVideoDet.iEPUseCarTailModel,
        0, 1,
        "�羯���Ƴ�����Ƿ�ʹ��ģ�͹��˶��", "", PROJECT_LEVEL
    );

    HvParamReadIntWithWrite(
        pcParamStore,
        "\\Tracker\\VideoDet", "EPNoPlateScoreThreld",
        &cTrackerCfgParam.cVideoDet.iEPNoPlateScoreThreld,
        cTrackerCfgParam.cVideoDet.iEPNoPlateScoreThreld,
        0, 1000,
        "���Ƴ����˶��ķ�����ֵ", "", PROJECT_LEVEL
    );

    //ɨ��������Ч���ж�
    if (!CheckArea(
                cTrackerCfgParam.cDetectArea.DetectorAreaLeft,
                cTrackerCfgParam.cDetectArea.DetectorAreaTop,
                cTrackerCfgParam.cDetectArea.DetectorAreaRight,
                cTrackerCfgParam.cDetectArea.DetectorAreaBottom
            ))
    {
        cTrackerCfgParam.cDetectArea.DetectorAreaLeft = 2;
        cTrackerCfgParam.cDetectArea.DetectorAreaTop = 60;
        cTrackerCfgParam.cDetectArea.DetectorAreaRight = 98;
        cTrackerCfgParam.cDetectArea.DetectorAreaBottom = 100;
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
*/
    //Scale��Ч���ж�
    if (cTrackerCfgParam.cDetectArea.nDetectorMinScaleNum >
            cTrackerCfgParam.cDetectArea.nDetectorMaxScaleNum)
    {
        pcParamStore->SetInt("\\Tracker\\DetAreaCtrl\\Normal", "MinScale", 6);
        pcParamStore->SetInt("\\Tracker\\DetAreaCtrl\\Normal", "MaxScale", 10);
        cTrackerCfgParam.cDetectArea.nDetectorMinScaleNum = 5;
        cTrackerCfgParam.cDetectArea.nDetectorMaxScaleNum = 9;
    }

    //��Ƶ���������Ч���ж�
    if (!CheckArea(
                cTrackerCfgParam.cVideoDet.rcVideoDetArea.left,
                cTrackerCfgParam.cVideoDet.rcVideoDetArea.top,
                cTrackerCfgParam.cVideoDet.rcVideoDetArea.right,
                cTrackerCfgParam.cVideoDet.rcVideoDetArea.bottom
            ))
    {
        pcParamStore->SetInt("\\Tracker\\VideoDet\\AreaCtrl", "Left", 0);
        pcParamStore->SetInt("\\Tracker\\VideoDet\\AreaCtrl", "Top", 0);
        pcParamStore->SetInt("\\Tracker\\VideoDet\\AreaCtrl", "Right", 100);
        pcParamStore->SetInt("\\Tracker\\VideoDet\\AreaCtrl", "Bottom", 100);

        cTrackerCfgParam.cVideoDet.rcVideoDetArea.left = 0;
        cTrackerCfgParam.cVideoDet.rcVideoDetArea.top =0;
        cTrackerCfgParam.cVideoDet.rcVideoDetArea.right = 100;
        cTrackerCfgParam.cVideoDet.rcVideoDetArea.bottom = 100;
    }
    LoadTrafficLightParam(pcParamStore, cTrackerCfgParam.cTrafficLight);
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
        0, 4,
        "Э������", "0:����ר��Э��; 1:NVC���; 2:�������; 3:�㲥Э��; 4:HV�����Э��", CUSTOM_LEVEL
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
    HvParamReadIntWithWrite(
        pcParamStore,
        "\\HvDsp\\Camera\\Ctrl", "DynamicTriggerEnable",
        &cCamCfgParam.iDynamicTriggerEnable, cCamCfgParam.iDynamicTriggerEnable,
        0, 1,
        "��Ƶ����ץ�����", "�Դ���Jϵ�������HV�������Ч", CUSTOM_LEVEL
    );
    HvParamReadIntWithWrite(
        pcParamStore,
        "\\HvDsp\\Camera\\Ctrl", "TriggerDelay",
        &cCamCfgParam.iTriggerDelay, cCamCfgParam.iTriggerDelay,
        0, 65535,
        "������ʱ(ms)", "", PROJECT_LEVEL
    );
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

int LoadTrafficLightParam(
    CParamStore* pcParamStore,
    TRAFFICLIGHT_CFG_PARAM& cTrafficLightCfgParam
)
{
    HvParamReadIntWithWrite(
        pcParamStore,
        "\\Tracker\\TrafficLight", "CheckType",
        &cTrafficLightCfgParam.nCheckType, cTrafficLightCfgParam.nCheckType,
        0, 1,
        "���̵Ƽ�ⷽʽ(0:��Ƶʶ��, 1:��Ӻ��̵���Ϣ)", "", CUSTOM_LEVEL
    );
    HvParamReadIntWithWrite(
        pcParamStore,
        "\\Tracker\\TrafficLight", "AutoScanLight",
        &cTrafficLightCfgParam.nAutoScanLight, cTrafficLightCfgParam.nAutoScanLight,
        0, 1,
        "��̬�����̵�λ�ÿ���", "", CUSTOM_LEVEL
    );
    HvParamReadIntWithWrite(
        pcParamStore,
        "Tracker\\TrafficLight","DeleteLightEdge",
        &cTrafficLightCfgParam.fDeleteLightEdge,cTrafficLightCfgParam.fDeleteLightEdge,
        0,1,
        "ȥ���ƿ��Ե����","",PROJECT_LEVEL
    );

    HvParamReadIntWithWrite(
        pcParamStore,
        "\\Tracker\\TrafficLight", "SceneCheckMode",
        &cTrafficLightCfgParam.nSenceCheckMode, cTrafficLightCfgParam.nSenceCheckMode,
        0, 1,
        "�������ģʽ", "", CUSTOM_LEVEL
    );
    HvParamReadIntWithWrite(
        pcParamStore,
        "\\Tracker\\TrafficLight", "StopLinePos",
        &cTrafficLightCfgParam.nStopLinePos, cTrafficLightCfgParam.nStopLinePos,
        0, 100,
        "ֹͣ��λ��(ֱ��)", "", CUSTOM_LEVEL
    );
    HvParamReadIntWithWrite(
        pcParamStore,
        "\\Tracker\\TrafficLight", "LeftStopLinePos",
        &cTrafficLightCfgParam.nLeftStopLinePos, cTrafficLightCfgParam.nLeftStopLinePos,
        0, 100,
        "ֹͣ��λ��(��ת)", "", CUSTOM_LEVEL
    );
    HvParamReadIntWithWrite(
        pcParamStore,
        "\\Tracker\\Templet", "ScanSpeed",
        &cTrafficLightCfgParam.nMatchScanSpeed, cTrafficLightCfgParam.nMatchScanSpeed,
        0, 100,
        "ɨ���ٶ�(0:��, 1:��)", "", PROJECT_LEVEL
    );
    HvParamReadIntWithWrite(
        pcParamStore,
        "\\Tracker\\TrafficLight", "LightCount",
        &cTrafficLightCfgParam.nLightCount, cTrafficLightCfgParam.nLightCount,
        0, MAX_TRAFFICLIGHT_COUNT,
        "��������", "", CUSTOM_LEVEL
    );
    HvParamReadIntWithWrite(
        pcParamStore,
        "\\Tracker\\TrafficLight", "ValidSceneStatus",
        &cTrafficLightCfgParam.nValidSceneStatusCount, cTrafficLightCfgParam.nValidSceneStatusCount,
        1, 100,
        "��Ч����״̬����", "��ͬ״̬Ҫ�ﵽ��ֵ�����", CUSTOM_LEVEL
    );
    HvParamReadIntWithWrite(
        pcParamStore,
        "\\Tracker\\TrafficLight", "OutputInLine",
        &cTrafficLightCfgParam.nOutputInLine, cTrafficLightCfgParam.nOutputInLine,
        0, 1,
        "���δ��ֹͣ�ߵĳ���", "", CUSTOM_LEVEL
    );
    HvParamReadIntWithWrite(
        pcParamStore,
        "\\Tracker\\TrafficLight", "RedLightDelay",
        &cTrafficLightCfgParam.nRedLightDelay, cTrafficLightCfgParam.nRedLightDelay,
        0, 300,
        "��ɫ�ƻƵƱ��Ƽ��(��)", "", CUSTOM_LEVEL
    );
    HvParamReadIntWithWrite(
        pcParamStore,
        "\\Tracker\\TrafficLight", "SpecificLeft",
        &cTrafficLightCfgParam.nSpecificLeft, cTrafficLightCfgParam.nSpecificLeft,
        0, 1,
        "��ת�����", "", CUSTOM_LEVEL
    );
    HvParamReadIntWithWrite(
        pcParamStore,
        "\\Tracker\\TrafficLight", "RunTypeEnable",
        &cTrafficLightCfgParam.nRunTypeEnable, cTrafficLightCfgParam.nRunTypeEnable,
        0, 1,
        "��ʻ����ǿ�����", "", CUSTOM_LEVEL
    );
    for (int i = 0; i < MAX_TRAFFICLIGHT_COUNT; i++)
    {
        char szLightPosKey[32];
        char szLightPosInfo[32];
        sprintf(szLightPosKey, "LightPos_%d", i + 1);
        sprintf(szLightPosInfo, "������Ϣ_%d", i + 1);
        HvParamReadStringWithWrite(
            pcParamStore,
            "\\Tracker\\TrafficLight", szLightPosKey,
            cTrafficLightCfgParam.rgszLightPos[i], (64-1),
            szLightPosInfo, "", CUSTOM_LEVEL
        );
    }
    HvParamReadIntWithWrite(
        pcParamStore,
        "\\Tracker\\TrafficLight\\Scene", "SceneCount",
        &cTrafficLightCfgParam.nSceneCount, cTrafficLightCfgParam.nSceneCount,
        0, MAX_SCENE_COUNT,
        "���̵Ƴ�����", "", CUSTOM_LEVEL
    );
    for (int i = 0; i < MAX_SCENE_COUNT; i++)
    {
        char szSceneKey[32];
        char szSceneInfo[32];
        sprintf(szSceneKey, "Scene_%d", i + 1);
        sprintf(szSceneInfo, "������Ϣ_%d", i + 1);
        HvParamReadStringWithWrite(
            pcParamStore,
            "\\Tracker\\TrafficLight\\Scene", szSceneKey,
            cTrafficLightCfgParam.rgszScene[i], (32-1),
            szSceneInfo, "", CUSTOM_LEVEL
        );
    }

    HvParamReadStringWithWrite(
        pcParamStore,
        "Tracker\\TrafficLight", "IOConfig",
        cTrafficLightCfgParam.szIOConfig, sizeof(cTrafficLightCfgParam.szIOConfig),
        "���IO���̵Ʋ���", "", CUSTOM_LEVEL
    );
    HvParamReadIntWithWrite(
        pcParamStore,
        "Tracker\\TrafficLight", "EnhanceRedLight",
        &cTrafficLightCfgParam.fEnhanceRedLight, cTrafficLightCfgParam.fEnhanceRedLight,
        0,1,
        "��Ʒ��������Ƿ�Ӻ�", "", CUSTOM_LEVEL
    );
    HvParamReadIntWithWrite(
        pcParamStore,
        "Tracker\\TrafficLight","TrafficLightThread",
        &cTrafficLightCfgParam.nTrafficLightThread, cTrafficLightCfgParam.nTrafficLightThread,
        0,5,
        "���̵�������","",CUSTOM_LEVEL
    );
    HvParamReadIntWithWrite(
        pcParamStore,
        "Tracker\\TrafficLight","TrafficLightBrightValue",
        &cTrafficLightCfgParam.nTrafficLightBrightValue, cTrafficLightCfgParam.nTrafficLightBrightValue,
        0,5,
        "���̵����ȵȼ�","",CUSTOM_LEVEL
    );
    HvParamReadIntWithWrite(
        pcParamStore,
        "Tracker\\TrafficLight","AutoScanThread",
        &cTrafficLightCfgParam.nAutoScanThread, cTrafficLightCfgParam.nAutoScanThread,
        0,10,
        "���̵ƶ�̬�����ֵ","",CUSTOM_LEVEL
    );
    HvParamReadIntWithWrite(
        pcParamStore,
        "Tracker\\TrafficLight","SkipFrameNO",
        &cTrafficLightCfgParam.nSkipFrameNO, cTrafficLightCfgParam.nSkipFrameNO,
        0,50,
        "��̬����ҵƼ��֡","",CUSTOM_LEVEL
    );
    HvParamReadIntWithWrite(
        pcParamStore,
        "Tracker\\TrafficLight","LightAdhesion",
        &cTrafficLightCfgParam.fIsLightAdhesion, cTrafficLightCfgParam.fIsLightAdhesion,
        0,1,
        "���̵��Ƿ�ճ����һ��","",CUSTOM_LEVEL
    );
    // zhaopy
    HvParamReadIntWithWrite(
        pcParamStore,
        "Tracker\\TrafficLight","HMThreld",
        &cTrafficLightCfgParam.fltHMThreld, cTrafficLightCfgParam.fltHMThreld,
        0,100,
        "������ֵ","",PROJECT_LEVEL
    );
    HvParamReadIntWithWrite(
        pcParamStore,
        "Tracker\\TrafficLight","HMThreldNight",
        &cTrafficLightCfgParam.fltHMThreldNight, cTrafficLightCfgParam.fltHMThreldNight,
        0,100,
        "ҹ����ֵ","",PROJECT_LEVEL
    );
    HvParamReadIntWithWrite(
        pcParamStore,
        "Tracker\\TrafficLight","HMDelay",
        &cTrafficLightCfgParam.iHMDelay, cTrafficLightCfgParam.iHMDelay,
        1,100,
        "�����ӳ�֡��","",PROJECT_LEVEL
    );
    HvParamReadIntWithWrite(
        pcParamStore,
        "Tracker\\TrafficLight","HMDelayNight",
        &cTrafficLightCfgParam.iHMDelayNight, cTrafficLightCfgParam.iHMDelayNight,
        1,100,
        "ҹ���ӳ�֡��","",PROJECT_LEVEL
    );
    HvParamReadIntWithWrite(
        pcParamStore,
        "Tracker\\TrafficLight","MaxMatchDiff",
        &cTrafficLightCfgParam.iMaxMatchDiff, cTrafficLightCfgParam.iMaxMatchDiff,
        1,100,
        "������ֵ","",PROJECT_LEVEL
    );
    HvParamReadIntWithWrite(
        pcParamStore,
        "Tracker\\TrafficLight","RealForward",
        &cTrafficLightCfgParam.iRealForward, cTrafficLightCfgParam.iRealForward,
        0,100,
        "ֱ���ж�λ��","",CUSTOM_LEVEL
    );
    HvParamReadIntWithWrite(
        pcParamStore,
        "\\Tracker\\TrafficLight", "FilterNoPlatePeccancy",
        &cTrafficLightCfgParam.fFilterNoPlatePeccancy, cTrafficLightCfgParam.fFilterNoPlatePeccancy,
        0, 1,
        "�����޳���Υ�½��", "", CUSTOM_LEVEL
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
