#include <signal.h>
#include <execinfo.h>
#include "hvutils.h"
#include "misc.h"

/* �������ͷ�ļ�ͨ������Դ�����Ŀ¼�µ�syncbuild.sh������ע��ֻ�����������һ�Σ� */
#include "../DspBuildNo.h"

#ifndef _HVCAM_PLATFORM_RTM_
static void OnSignal(int signalno)
{
    HV_Trace(5, "==============receive signalno=%d=================\n", signalno);
    bool bExit = true;
    for (int i = 0; i < 100; i++)
    {
        if (signalno == 17
        	|| signalno == 13)
        {
            bExit = false;
            break;
        }
    }
    if (bExit)
    {
        HV_Trace(5, "============exit process.==================\n");
        HV_Exit(HEC_FAIL, "OnSignal exit");
    }
}
#endif

static void* g_rgState[1000] = {0};
static void MySigHandle(int nSig)
{
    Trace("Sig: %d\n",nSig);

    static int nStateCount = 0;

    nStateCount = backtrace(g_rgState,1000);
    Trace("state count: %d\n", nStateCount);

    backtrace_symbols_fd(g_rgState, nStateCount, STDOUT_FILENO);

    HV_Exit(HEC_FAIL, "MySigHandle exit");
}

// ��ӡ���ְ汾��Ϣ
static void PrintVersionInfo()
{
    char szBuf[512] = {0};

    GetSN(szBuf, sizeof(szBuf));
    HV_Trace(5, "[SerialNo]: %s\n", szBuf);

    HV_Trace(5, "[App ver]: %s %s %s\n",
             PSZ_DSP_BUILD_NO,
             DSP_BUILD_DATE,
             DSP_BUILD_TIME);

    ReadDataFromFile("sys/swdev/version", (PBYTE8)szBuf, sizeof(szBuf));
    HV_Trace(5, "[slw-dev.ko ver]: %s", szBuf);

    ReadDataFromFile("/sys/swpci/version", (PBYTE8)szBuf, sizeof(szBuf));
    HV_Trace(5, "[slw-pci.ko ver]: %s", szBuf);

    ReadDataFromFile("/proc/version", (PBYTE8)szBuf, sizeof(szBuf));
		HV_Trace(5, "[Kernel ver]: %s", szBuf);
 
    GetUbootVersion(szBuf, sizeof(szBuf));
    HV_Trace(5, "[Uboot ver]: %s\n", szBuf);
}

#define SIG_MAX_COUNT 100

void LinuxSystemInit(void)
{
    system("dm.sh &"); //���к�̨�ű�

    if (HV_System("echo 2 > /proc/sys/vm/overcommit_memory")
            || HV_System("echo 70 > /proc/sys/vm/overcommit_ratio")
            || HV_System("echo 3 > /proc/cpu/alignment"))
    {
        HV_Trace(5, "LinuxSystemInit execute HV_System error.\n");
    }

#ifndef _HVCAM_PLATFORM_RTM_
    for (int i = 0; i < SIG_MAX_COUNT; i++)
    {
        if (i != SIGILL && i != SIGBUS && i != SIGSEGV)
        {
            signal(i, OnSignal);
        }
    }
#else
    HV_System("ulimit -c 0");
#endif

    //�ź���ش���
    signal(SIGILL, MySigHandle);
    signal(SIGBUS, SIG_IGN);
    signal(SIGSEGV, SIG_IGN);

    HV_Trace(5, "\n============================\n");
    PrintVersionInfo();
    HV_Trace(5, "\n============================\n");
}
