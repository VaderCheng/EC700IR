#ifndef _HVUTILSCLASS_H_
#define _HVUTILSCLASS_H_

#include <stdio.h>
#include <sys/wait.h>
#include "config.h"
#include "misc.h"
#include "hvthreadbase.h"
#include "ICMP.h"
//-------------------------------------------------

#ifdef _USE_SEED_DEBUG_
/* DSPLink.c */
extern int g_fDebugStart;
#endif

//��ʱ�����һ���ࣺ������ʱ�˳�����
class CUserExitThread : public CHvThreadBase
{
public:
    virtual const char* GetName()
    {
        static char szName[] =  "CUserExitThread";
        return szName;
    }

    virtual HRESULT Run(void *pvParamter)
    {
#if !defined(_HVCAM_PLATFORM_RTM_)
        Trace("CUserExitThread is Running...\n");

        while ( !m_fExit )
        {
            char c = getchar();
            if ( 'q' == c )
            {
                HV_Exit(HEC_SUCC|HEC_RESET_DEV, "ExitDebug");
            }
#ifdef _USE_SEED_DEBUG_
            else if ( 'g' == c )
            {
                g_fDebugStart = 1;
            }
#endif // _USE_SEED_DEBUG_
        }
        Trace("CUserExitThread is Exiting...\n");
        HV_Exit(HEC_FAIL, "UserExit");
#endif // #if !defined(_HVCAM_PLATFORM_RTM_) || defined(SINGLE_BOARD_PLATFORM)

        return S_OK;
    };
};

//-------------------------------------------------

#define PING_GATEWAY_T 10000  // ping�������ڡ���λ������

// Ping�����̡߳���;��ÿ��һ������pingһ�����أ�Ŀ����Ϊ�˸���ARP��
class CPingThread : public CHvThreadBase
{
public:
    void SetIPAddress(char * szIP, char * szNetmask, char* pszGateWay)
    {
        int a,b,c,d, e,f,g,h, i,j,k,l;
        if (4 == sscanf(szIP, "%d.%d.%d.%d", &a, &b, &c, &d)
                &&4 == sscanf(szNetmask, "%d.%d.%d.%d", &e, &f, &g, &h)
                &&4 == sscanf(pszGateWay, "%d.%d.%d.%d", &i, &j, &k, &l)
                && !i && !j && !k && !l)
        {
            i = a & e;
            j = b & f;
            k = c & g;
            l = 1;
        }
        else
        {
            i = 172;
            j = 18;
            k = 10;
            l = 49;
        }
        sprintf(szGateWay, "%d.%d.%d.%d", i, j, k, l);
    }

    virtual const char* GetName()
    {
        static char szName[] = "CPingThread";
        return szName;
    }

    virtual HRESULT Run(void *pvParamter)
    {
        Trace("CPingThread is Running...\n");

        CICMP cICMP;
        int num = 0;
        static char szPingCmd[64] = {"test"};

        while ( !m_fExit )
        {
            HV_Trace(5, "Ping Gateway===%s.\n", szGateWay);

            cICMP.setId(getpid());
            cICMP.setSeq(++num);
            cICMP.send_icmp(szGateWay, szPingCmd, strlen(szPingCmd));

            HV_Sleep(PING_GATEWAY_T);
        }

        Trace("CPingThread is Exiting.\n");
        return S_OK;
    };

private:
    char szGateWay[16];
};

//-------------------------------------------------

typedef enum tagHddOpType
{
    HOT_UNKNOWN,
    HOT_FULL_INIT,  // ��ȫ��ʼ��Ӳ�̣���������ʽ������⣬���ɼ�ⱨ�棩
    HOT_INIT,       // ��ʼ��Ӳ�̣���⣬���ɼ�ⱨ�棬���ط�����
    HOT_CHECKFACTOR,// �������
    HOT_NOTHING     // �������κ�Ӳ�̲���
} HddOpType;

// Ӳ�̲����߳�
class CHddOpThread : public CHvThreadBase
{
public:
    CHddOpThread();

    virtual const char* GetName()
    {
        static char szName[] = "CHddOpThread";
        return szName;
    }

    virtual HRESULT Run(void *pvParamter);

    void SetHddOpType(HddOpType eHddOpType, int iDiskCount);
    int GetCheckReport(char* szReport, int size);
    int GetCurStatusCode();
    const char * GetCurStatusCodeString(void);
    void SetStatusCodeString(const char *szStatus);
private:
    void ReadCheckReport(const char* szFileName, char* szReport, int& nReportLen);
    HRESULT AnalyticCheckReport();

    int m_nStatusCode;  //TODO:��Ҫȥ��MagicNum
    CHvString m_strStatusCode;
    HddOpType m_eHddOpType;
    int m_iDiskCount;
};

/* HvUtilsClass.cpp */
extern CHddOpThread g_cHddOpThread;

enum ECdrReport
{
	CDR_UNKNOWN = 0,
	CDR_UNDETECTION,      //δ���
	CDR_UNPARTITION,  		//Ӳ��δ����
	CDR_PARTITIONING, 	//Ӳ�̷�����
	CDR_DETECTING,    		//�����
	CDR_PASS,          	//���ͨ��
	CDR_UNPASS,        	//���ʧ��
	CDR_NODISK,        	//û��Ӳ��
	CDR_COUNT,
};

struct SCheckDiskReport
{
	int    	iResult;    //����ֵ
	char    szMsg[256]; //��Ϣ

	SCheckDiskReport()
	{
		memset(this, 0 ,sizeof(*this));
	}
};

//������ƥ����Ӳ�̻���U��
class CCheckDisk : public CHvThreadBase
{
public:
	CCheckDisk();
	~CCheckDisk();

	virtual const char* GetName()
	{
		static char szName[] = "CCheckDisk";
		return szName;
	}

	virtual HRESULT Run(void *pvParamter);

	//����
	int DoPartition();

	//�������
	int DoCheckDisk();

	//��ȡ�����
	void GetCheckDisk(SCheckDiskReport& sCheckDiskReport);

protected:
	bool IsPartition();
	bool HaveDisk();
	void GetDiskInfo();

private:
	SCheckDiskReport    m_sCheckDiskReport;
	int                 m_iSts;  //0���� 1������ 2�����
};

extern CCheckDisk g_cCheckDisk;

#endif  // _HVUTILSCLASS_H_
