#include "TcpipCfg.h"
#include "HvParamIO.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <linux/netlink.h>
#include <linux/rtnetlink.h>

/* LoadParam.cpp */
extern CParamStore g_cParamStore;

HRESULT SetTcpipAddr_0(TcpipParam* pParam)
{
    char szCmd[256] = {0};

    SetIfUp("eth0");
    if ( 0 != SetLocalNetInfo("eth0", pParam->szIp, pParam->szNetmask) )
    {
        HV_Trace(5, "SetLocalNetInfo is failed.\n");
        return E_FAIL;
    }

    // �����޷���֪��һ�����õ�Ĭ�����أ������ص㶼������Ϊ0.0.0.0����ͨ���÷�ʽɾ����һ�����õ�Ĭ������
    // �������޶�ɾ��Ĭ�����أ��п����ϴ��Ѿ����ã�����ᱨ������֪��ɾ���Ƿ�ɹ�
    HvRoute("del default netmask 0.0.0.0");

    // ���255.255.255.255����ȷ���������͹㲥�����п����ϴ��Ѿ����ã�����ᱨ������֪������Ƿ�ɹ�
    HvRoute("add 255.255.255.255 eth0");

    // һ�����������ʧ�ܲ���Ӱ�����磬��������ж�HV_System�ķ���ֵ
    sprintf(szCmd, "add default gw %s", pParam->szGateway);
    HvRoute(szCmd);

    HV_Trace(5, "Tcpip addr set:\n ip: %s\n netmask: %s\n gateway: %s\n",
             pParam->szIp,
             pParam->szNetmask,
             pParam->szGateway
            );

    return S_OK;
}

HRESULT SetTcpipAddr_1(TcpipParam* pParam)
{
    char szCmd[256] = {0};

    SetIfUp("eth0.10");
    if ( 0 != SetLocalNetInfo("eth0.10", pParam->szIp, pParam->szNetmask) )
    {
        HV_Trace(5, "SetLocalNetInfo is failed.\n");
        return E_FAIL;
    }

    // �����޷���֪��һ�����õ�Ĭ�����أ������ص㶼������Ϊ0.0.0.0����ͨ���÷�ʽɾ����һ�����õ�Ĭ������
    // �������޶�ɾ��Ĭ�����أ��п����ϴ��Ѿ����ã�����ᱨ������֪��ɾ���Ƿ�ɹ�
    HvRoute("del default netmask 0.0.0.0 eth0.10");

    // ���255.255.255.255����ȷ���������͹㲥�����п����ϴ��Ѿ����ã�����ᱨ������֪������Ƿ�ɹ�
    HvRoute("add 255.255.255.255 eth0.10");

    sprintf(szCmd, "add default gw %s eth0.10", pParam->szGateway);
    if ( 0 != HvRoute(szCmd) )
    {
        // ������غ�IP����ͬһ�������ڽ���������ΪIP��������ֻ����LAN1��������»����������豸
        HV_Trace(5, "The gateway of eth0.10 is wrong, set it as IP[%s]!\n", pParam->szIp);

        strcpy(pParam->szGateway, pParam->szIp);
        sprintf(szCmd, "add default gw %s eth0.10", pParam->szGateway);
        if ( 0 != HvRoute(szCmd) )
        {
            HV_Trace(5, "%s failed.\n", szCmd);
            return E_FAIL;
        }
    }

    HV_Trace(5, "Tcpip addr_1 set:\n ip: %s\n netmask: %s\n gateway: %s\n",
             pParam->szIp,
             pParam->szNetmask,
             pParam->szGateway
            );

    return S_OK;
}

HRESULT SetTcpipAddr_2(TcpipParam* pParam)
{
    char szCmd[256] = {0};

    SetIfUp("eth0.20");
    if ( 0 != SetLocalNetInfo("eth0.20", pParam->szIp, pParam->szNetmask) )
    {
        HV_Trace(5, "SetLocalNetInfo is failed.\n");
        return E_FAIL;
    }

    // �����޷���֪��һ�����õ�Ĭ�����أ������ص㶼������Ϊ0.0.0.0����ͨ���÷�ʽɾ����һ�����õ�Ĭ������
    // �������޶�ɾ��Ĭ�����أ��п����ϴ��Ѿ����ã�����ᱨ������֪��ɾ���Ƿ�ɹ�
    HvRoute("del default netmask 0.0.0.0 eth0.20");

    // ���255.255.255.255����ȷ���������͹㲥�����п����ϴ��Ѿ����ã�����ᱨ������֪������Ƿ�ɹ�
    HvRoute("add 255.255.255.255 eth0.20");

    sprintf(szCmd, "add default gw %s eth0.20", pParam->szGateway);
    if ( 0 != HvRoute(szCmd) )
    {
        // ������غ�IP����ͬһ�������ڽ���������ΪIP��������ֻ����LAN1��������»����������豸
        HV_Trace(5, "The gateway of eth0.20 is wrong, set it as IP[%s]!\n", pParam->szIp);

        strcpy(pParam->szGateway, pParam->szIp);
        sprintf(szCmd, "add default gw %s eth0.20", pParam->szGateway);
        if ( 0 != HvRoute(szCmd) )
        {
            HV_Trace(5, "%s failed.\n", szCmd);
            return E_FAIL;
        }

        g_cParamStore.SetString(
            "\\System\\TcpipCfg", "CommIPAddr",
            pParam->szIp
        );
        g_cParamStore.SetString(
            "\\System\\TcpipCfg", "CommMask",
            pParam->szNetmask
        );
        g_cParamStore.SetString(
            "\\System\\TcpipCfg", "CommGateway",
            pParam->szGateway
        );

        if ( S_OK != g_cParamStore.Save() ) // ���浽EEPROM������
        {
            HV_Trace(5, "Save ip failed!");
            return E_FAIL;
        }
    }

    HV_Trace(5, "Tcpip addr_2 set:\n ip: %s\n netmask: %s\n gateway: %s\n",
             pParam->szIp,
             pParam->szNetmask,
             pParam->szGateway
            );

    return S_OK;
}

void GetLocalTcpipAddr(const char* pszEth, char* pszIpAddr, char* pszNetmask, char* pszGateway, char* pszMac)
{
    GetLocalNetInfo(pszEth, pszIpAddr, pszNetmask, pszMac);
    GetDefaultGateway(pszEth, pszGateway);
}

void ConvertStrIP2DWORD(char *pszIP, DWORD32 &dwIP)
{
    DWORD32 rgdwIP[4] = {0};
    sscanf(
        pszIP, "%u.%u.%u.%u",
        &rgdwIP[3], &rgdwIP[2],
        &rgdwIP[1], &rgdwIP[0]
    );
    BYTE8 *pIP = (BYTE8*)&dwIP;
    pIP[0] = (BYTE8)rgdwIP[0];
    pIP[1] = (BYTE8)rgdwIP[1];
    pIP[2] = (BYTE8)rgdwIP[2];
    pIP[3] = (BYTE8)rgdwIP[3];
}

HRESULT CheckIPAddr(TcpipParam cTcpipCfgParam1, TcpipParam cTcpipCfgParam2)
{
    DWORD32 dwIP1, dwIP2, dwszNetmask1, dwszNetmask2;
    ConvertStrIP2DWORD(cTcpipCfgParam1.szIp, dwIP1);
    ConvertStrIP2DWORD(cTcpipCfgParam1.szNetmask, dwszNetmask1);
    ConvertStrIP2DWORD(cTcpipCfgParam2.szIp, dwIP2);
    ConvertStrIP2DWORD(cTcpipCfgParam2.szNetmask, dwszNetmask2);

    if ((dwIP1 & dwszNetmask1) == (dwIP2 & dwszNetmask2))
    {
        // �ر�LAN1����
        SetIfDown("eth0.10");
        HV_Trace(5, "Error IP, Lan1 had shut down!\n");
        return E_FAIL;
    }
    return S_OK;
}
