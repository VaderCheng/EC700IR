// ���ļ��������ΪWINDOWS-936��ʽ
#include "LoadParam.h"
#include "HvParamIO.h"
#include "misc.h"
#include "platerecogparam.h"
#include "hvtarget_ARM.h"
#include <math.h>
#include "HvParamIO.h"
#include "hvsysinterface.h"
using namespace HvSys;

static ModuleParams g_cModuleParams;

//����ģʽ�б�����Ҫ��platerecogparam.h��һ��
char *g_pszPartWorkModeList[] =
{
    "���������"
};

CEprFileStorage g_cFileStorage(PARAM_START_ADDR, PARAM_SIZE);
CParamStore g_cParamStore;

HRESULT LoadModuleParam(ModuleParams& cModuleParams)
{
    if (FAILED(g_cFileStorage.Initialize("")))
    {
        HV_Trace(5, "Open param file failed, can't save setting!!!\n");
    }

    g_cParamStore.Initialize(&g_cFileStorage, true);
    LoadTcpipParam_1(&g_cParamStore, cModuleParams.cTcpipCfgParam_1);
    LoadTcpipParam_2(&g_cParamStore, cModuleParams.cTcpipCfgParam_2);
    //����ģʽһ��Ҫ���ȼ��أ��������Ĳ����п��ܼ��ش�
    LoadWorkModeParam(&g_cParamStore, cModuleParams);

    LoadResultSenderParam(&g_cParamStore, cModuleParams.cResultSenderParam);

    LoadDevParam(&g_cParamStore, cModuleParams.cDevParam);

    g_cParamStore.Save(HvCore::PSM_SIMPLE);

    //char szBuffer[64 * 1024] = {0};
    //GetParamXml(&g_cParamStore, (char*)szBuffer, sizeof(szBuffer));
    //printf(szBuffer);

    g_cModuleParams = cModuleParams;

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
        HvParamReadIntWithWrite(
            pcParamStore,
            "\\Tracker", "PlateRecogMode",
            &cModuleParams.nWorkModeIndex, cModuleParams.nWorkModeIndex,
            0, dwModeCount,
            "����ģʽ����", "", INNER_LEVEL
        );
        pcParamStore->Save();
        HV_Trace(5, "Load Default Param OK...%s\n", szModeName);
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

    pcParamStore->Clear();

    LoadTcpipParam_1(pcParamStore, cTcpipCfgParam1);
    LoadTcpipParam_2(pcParamStore, cTcpipCfgParam2);

    pcParamStore->Save();

    HV_Trace(5, "Param Load Default...\n");
}

static ResultSenderParam g_cResultSenderParam;

static HV_PARAM_TYPE_INT g_rgcResultSenderInt[] =
{
    {"\\HvDsp\\Misc", "SafeSaveEnable", (int*)&g_cResultSenderParam.fIsSafeSaver, 1, 0, 1, "�Ƿ�������ȫ�洢", "Ĭ��ֵ��1�� ��Сֵ��0�� ���ֵ��1\n0���������� 1������", CUSTOM_LEVEL},
    {"\\HvDsp\\Misc", "DoReplace", (int*)&g_cResultSenderParam.cProcRule.fReplace, g_cResultSenderParam.cProcRule.fReplace, 0, 1, "�Ƿ��滻", "Ĭ��ֵ��0�� ��Сֵ��0�� ���ֵ��1\n0�����滻�� 1���滻", PROJECT_LEVEL},
    {"\\HvDsp\\Misc", "DoLeach", (int*)&g_cResultSenderParam.cProcRule.fLeach, g_cResultSenderParam.cProcRule.fLeach, 0, 1, "�Ƿ����", "Ĭ��ֵ��0�� ��Сֵ��0�� ���ֵ��1\n0�������ˣ� 1������", PROJECT_LEVEL},
    {"\\HvDsp\\Misc", "DoCompaty", (int*)&g_cResultSenderParam.cProcRule.fCompaty, g_cResultSenderParam.cProcRule.fCompaty, 0, 1, "�Ƿ�����ͨ�������", "Ĭ��ֵ��0�� ��Сֵ��0�� ���ֵ��1\n0�������ã� 1������", CUSTOM_LEVEL},
    {"\\HvDsp\\Misc", "LoopReplace", (int*)&g_cResultSenderParam.cProcRule.fLoopReplace, g_cResultSenderParam.cProcRule.fLoopReplace, 0, 1, "�Ƿ�����ѭ���滻", "Ĭ��ֵ��0�� ��Сֵ��0�� ���ֵ��1\n0�������ã� 1������", CUSTOM_LEVEL},
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

    HvParamReadIntWithWrite(
        pcParamStore, "\\HvDsp\\Misc", "InitHdd",
        &cResultSenderParam.fInitHdd,
        cResultSenderParam.fInitHdd, 0, 1,
        "��ʼ��Ӳ��", "��Ӳ�̽��з����Լ���ʽ����ע��һ������Ч",
        CUSTOM_LEVEL);

    HvParamReadIntWithWrite(
        pcParamStore, "\\HvDsp\\Misc", "TraceRank",
        &g_nTraceRank,
        5, 1, 5,
        "������Ϣ������ƣ�1:�з���,3:���̼�,5:�û�����", "",
        PROJECT_LEVEL);

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

int LoadDevParam(
    CParamStore* pcParamStore,
    DevParam& cDevParam
)
{
    cDevParam.iFilterTime = 10;
    strcpy(cDevParam.szDevIP1, "172.18.10.10");
    strcpy(cDevParam.szDevIP2, "172.18.10.10");
    strcpy(cDevParam.szDevIP3, "172.18.10.10");
    strcpy(cDevParam.szDevIP4, "172.18.10.10");
    strcpy(cDevParam.szDevIP5, "172.18.10.10");
    strcpy(cDevParam.szDevIP6, "172.18.10.10");
    strcpy(cDevParam.szDevIP7, "172.18.10.10");
    strcpy(cDevParam.szDevIP8, "172.18.10.10");

    HvParamReadIntWithWrite(
        pcParamStore, "\\HvDsp\\Misc", "FilterTime",
        &cDevParam.iFilterTime,
        cDevParam.iFilterTime, 1, 30,
        "���˲�ͬһ���һ��ʱ���ڵ���ͬ���(��λ:��)","",
        CUSTOM_LEVEL);

    HvParamReadIntWithWrite(
        pcParamStore, "\\HvDsp\\Misc", "DevCount",
        &cDevParam.iDevCount,
        cDevParam.iDevCount, 0, 8,
        "һ�������","",
        CUSTOM_LEVEL);

    HvParamReadStringWithWrite(
        pcParamStore,
        "\\HvDsp\\Misc", "DevIPAddr1",
        cDevParam.szDevIP1, 31,
        "��1��һ���IP��ַ", "", CUSTOM_LEVEL
    );

    HvParamReadStringWithWrite(
        pcParamStore,
        "\\HvDsp\\Misc", "DevIPAddr2",
        cDevParam.szDevIP2, 31,
        "��2��һ���IP��ַ", "", CUSTOM_LEVEL
    );

    HvParamReadStringWithWrite(
        pcParamStore,
        "\\HvDsp\\Misc", "DevIPAddr3",
        cDevParam.szDevIP3, 31,
        "��3��һ���IP��ַ", "", CUSTOM_LEVEL
    );

    HvParamReadStringWithWrite(
        pcParamStore,
        "\\HvDsp\\Misc", "DevIPAddr4",
        cDevParam.szDevIP4, 31,
        "��4��һ���IP��ַ", "", CUSTOM_LEVEL
    );

    HvParamReadStringWithWrite(
        pcParamStore,
        "\\HvDsp\\Misc", "DevIPAddr5",
        cDevParam.szDevIP5, 31,
        "��5��һ���IP��ַ", "", CUSTOM_LEVEL
    );

    HvParamReadStringWithWrite(
        pcParamStore,
        "\\HvDsp\\Misc", "DevIPAddr6",
        cDevParam.szDevIP6, 31,
        "��6��һ���IP��ַ", "", CUSTOM_LEVEL
    );

    HvParamReadStringWithWrite(
        pcParamStore,
        "\\HvDsp\\Misc", "DevIPAddr7",
        cDevParam.szDevIP7, 31,
        "��7��һ���IP��ַ", "", CUSTOM_LEVEL
    );

    HvParamReadStringWithWrite(
        pcParamStore,
        "\\HvDsp\\Misc", "DevIPAddr8",
        cDevParam.szDevIP8, 31,
        "��8��һ���IP��ַ", "", CUSTOM_LEVEL
    );
}

//--------------------------------------------------------------------------------------
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

//ParseExecXmlExtCmd.cpp
//--------------------------------------------------------------------------------------
HRESULT GetWorkModeInfo(SYS_INFO* pInfo)
{
    DWORD32 dwModeIndex = 0;
    char szModeName[128] = "UNKNOWN";
    //m_cModuleParams.nWorkModeIndex������Ƿ���ǰ��ģʽ����
    //����轫������ת��Ϊ���ͺ������
    GetRecogParamNameOnIndex(g_cModuleParams.nWorkModeIndex, szModeName);
    pInfo->Info.WorkMode.dwMode = GetRecogParamIndexOnNamePart(szModeName);
    pInfo->Info.WorkMode.dwModeCount = GetRecogParamCountPart();
    return S_OK;
}

HRESULT GetWorkModeList(SYS_INFO* pInfo)
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

HRESULT SetWorkMode(DWORD32 dwWorkMode)
{
    return(SetWorkModePart(dwWorkMode));
}
