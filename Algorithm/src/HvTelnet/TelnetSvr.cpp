// ���ļ����������WINDOWS-936��ʽ
/*
 * TelnetSvr.cpp
 *
 *  Created on: May 9, 2011
 *      Author: ganzz
 */

#include "TelnetSvr.h"

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <iconv.h>
#include <signal.h>

#include "hvtarget_ARM.h"
#include "misc.h"
#include "ControlFunc.h"

#define LISTEN_BACKLOG 2
#define BUF_LEN 2048

#ifdef _CAM_APP_
/* main.c */
extern int g_nFpgaVersion; // FPGA�汾��
#endif

#if defined(_CAM_APP_) || defined(SINGLE_BOARD_PLATFORM)
/* DspBuildNo.h */
extern const char* PSZ_DSP_BUILD_NO;
extern const char* DSP_BUILD_DATE;
extern const char* DSP_BUILD_TIME;
#endif

static void GetVersionStr(char* pBuf);
unsigned short TelnetSvr::port = DEFAULT_TELNETD_PORT;
//////////////////////////////////////////////////////////

TelnetSvr* TelnetSvr::s_Instance = NULL;

TelnetSvr::TelnetSvr() :
        m_bRun(false),
        m_hSockSvr(-1)
{
}

TelnetSvr::~TelnetSvr()
{
    Stop();
    Close();
}

void TelnetSvr::Close()
{
    if (m_hSockSvr != -1)
    {
        shutdown(m_hSockSvr, 0x02);
        close(m_hSockSvr);
        m_hSockSvr = -1;
    }
}

TelnetSvr* TelnetSvr::GetInstance()
{
    if (s_Instance == NULL)
    {
        s_Instance = new TelnetSvr;
    }
    return s_Instance;
}

HRESULT TelnetSvr::Run(void* pvParam)
{
    if (m_bRun)
        return 0;

    m_bRun = true;

    m_hSockSvr = HvCreateSocket();

    if (m_hSockSvr == -1)
    {
        HV_Trace(5, "socket error!\n");
        return 0;
    }

    int ret = 0;
    ret = HvListen(m_hSockSvr,port,LISTEN_BACKLOG);

    if (ret == -1)
    {
        HV_Trace(5, "<TelnetSvr> listen error!\n");
        goto L_ERR;
    }
    HV_Trace(5, "<TelnetSvr> Listen at [%d]...\n", port);

    run_svr();

    return 1;

L_ERR:
    shutdown(m_hSockSvr, 0x02);
    close(m_hSockSvr);
    m_hSockSvr = -1;
    return ret;
}

Log* TelnetSvr::GetLog()
{
    return &m_log;
}

int TelnetSvr::u2g(char* input, int inputLen, char* output, int charOutPutLen)
{
    // ��ΪDM6467�ϵ�Linuxȱ��gb2312�ַ������������θú�����
    return inputLen;

    /*
    int nRt;
    iconv_t cd;
    cd = iconv_open("gb2312", "utf-8");
    if (cd == 0)
    {
        if (input != output)
            memcpy(output, input, inputLen);
        return inputLen;
    }

    int nOutlen = charOutPutLen;
    nRt = iconv(cd, &input, (size_t*)&inputLen, &output, (size_t*)&charOutPutLen);
    iconv_close(cd);

    return nOutlen - charOutPutLen;
    */
}

int TelnetSvr::send_main_menu(int hSock, char* buf)
{
    sprintf(buf, "\r\n������:\r\n"
            "    1 - ��ȡHDD��Ϣ\r\n"
            "    2 - ��ȡģ����Ϣ\r\n"
            "    3 - ��ӡ������Ϣ\r\n"
            "    4 - ��ʾ�汾��Ϣ\r\n"
#ifndef SINGLE_BOARD_PLATFORM
            "    5 - ��ƫ�⾵\r\n"
            "    6 - �ر�ƫ�⾵\r\n"
#endif
            "    7 - ��ȡCPU�Ĵ���\r\n"
            "    q - �˳�\r\n"
            "> "
           );
    int len = strlen(buf);
    len = u2g(buf, len, buf, BUF_LEN);
    return (SendAll(hSock, buf, len) == -1) ? 0 : 1;
}

int TelnetSvr::send_log_menu(int hSock, char* buf)
{
    sprintf(buf, "\r\n��ѡ�������Ϣ����ȼ���\r\n"
            "    1 - �з��ȼ�\r\n"
            "    2 - ���̵ȼ�\r\n"
            "    3 - �û��ȼ�\r\n"
            "    4 - ���еȼ�\r\n"
            "    q - �����ϼ��˵�\r\n"
            "> "
           );
    int len = strlen(buf);
    len = u2g(buf, len, buf, BUF_LEN);
    return (SendAll(hSock, buf, len) == -1) ? 0 : 1;
}

int TelnetSvr::send_rwreg_menu(int hSock, char* buf)
{
    sprintf(buf, "\r\n��д��CPU�Ĵ�����\r\n"
            "    ��ȡ�Ĵ���(r/rw) : r [addr]\r\n"
            "    д��Ĵ���(rw)   : w [addr] [val]\r\n"
            "    �����ϼ��˵�     : q\r\n"
            "> "
           );
    int len = strlen(buf);
    len = u2g(buf, len, buf, BUF_LEN);
    return (SendAll(hSock, buf, len) == -1) ? 0 : 1;
}

int TelnetSvr::handle_input(SConn* pConn, char* buf)
{
    int rt = 1;

    if ((buf[0] == 'h') || (buf[0] == 'H'))
    {
L_DISPLAY_MAIN_MENU:
        if ( !send_main_menu(pConn->hSock, buf)
                || (-1 == Recv(pConn->hSock, buf, BUF_LEN, 0)) )
        {
            rt = 0;
        }

        switch (buf[0])
        {
        case '1':
            if ( !send_hdd_info(pConn->hSock, buf) )
                rt = 0;
            break;

        case '2':
            if ( !send_mod_info(pConn->hSock, buf) )
                rt = 0;
            break;

        case '3':
        {
L_DISPLAY_LOG_MENU:
            if ( ( !send_log_menu(pConn->hSock, buf) )
                    || ( -1 == Recv(pConn->hSock, buf, BUF_LEN, 0)) )
                rt = 0;

            int nType = 0;

            if (buf[0] == '1')
                nType = 1;
            else if (buf[0] == '2')
                nType = 3;
            else if (buf[0] == '3')
                nType = 5;
            else if (buf[0] == '4')
                nType = 7;
            else if (buf[0] == 'q')
                break;
            else
                goto L_DISPLAY_LOG_MENU;

            if ( !send_log(pConn, buf, nType) )
                rt = 0;
        }
        break;

        case '7':
            rw_reg_info(pConn->hSock,buf);
            break;

        case 'q':
            rt = 0;
            break;

        default:
            if ( !exec_user_cmd(pConn->hSock, buf, buf[0]) )
                rt = 0;
            break;
        } // end of switch

        if (rt != 0)
            goto L_DISPLAY_MAIN_MENU;
    } // end of if

    return rt;
}

int TelnetSvr::send_log(SConn* pConn, char* buf, int nType)
{
    int rt = 1;
    unsigned int pos = 0;
    int len;
    int nLevelGet;
    char szHead[20];

    // set timeout 100 ms
    struct timeval timeoOld;
    socklen_t timeolen = sizeof(timeoOld);
    getsockopt(pConn->hSock, SOL_SOCKET, SO_RCVTIMEO, &timeoOld, &timeolen);

    struct timeval timeo =
    {
        0, 100
    };
    setsockopt(pConn->hSock, SOL_SOCKET, SO_RCVTIMEO, &timeo, sizeof(timeo));

    while (1)
    {
        len = pConn->pLog->Read(&nLevelGet, buf, &pos);
        if ( len > 0 )
        {
            if ( 7 == nType || (nLevelGet == nType) )
            {
                len = u2g(buf, len, buf, BUF_LEN);
                /*
                                // ȥ�����һ����\r\n��
                                if ( '\n' == buf[len-1] )
                                {
                                    len -= 2;
                                }
                */
                if (-1 == SendAll(pConn->hSock, buf, len))
                {
                    rt = 0;
                    goto L_DONT;
                }
            }
        }

        // �����������Ϣ�Ĺ����У��û���������������˳�������Ϣ�����
        int nRead = Recv(pConn->hSock, buf, BUF_LEN, 0);
        if ( -1 == nRead)
        {
            if ( errno != EAGAIN )
            {
                rt = 0;
                goto L_DONT;
            }
        }
        else
        {
            HV_Trace(5, "TelnetSvr read %d\n", nRead);
            break;
        }
    }

L_DONT:
    setsockopt(pConn->hSock, SOL_SOCKET, SO_RCVTIMEO, &timeoOld, sizeof(timeoOld));
    return rt;
}

void* TelnetSvr::handle_conn(void* lp)
{
    char buf[BUF_LEN];

    SConn* pConn = (SConn*)lp;

    HV_Trace(5, "TelnetSvr ====handle %s:%d ====\n", inet_ntoa(pConn->addr.sin_addr),
             ntohs(pConn->addr.sin_port));

    sprintf(buf, "welcome�����롮h�����ɵ��������˵���\r\n");

    if ( -1 == SendAll(pConn->hSock, buf, strlen(buf)))
        goto L_DONT;

    while (1)
    {
        if ( -1 == SendAll(pConn->hSock, "\r\n> ", 4))
            goto L_DONT;

        if ( -1 == Recv(pConn->hSock, buf, BUF_LEN, 0))
            goto L_DONT;

        HV_Trace(5, "TelnetSvr read %d\n", buf[0]);

        if ( !handle_input(pConn, buf) )
            goto L_DONT;
    }

L_DONT:
    HV_Trace(5, "exit conn\n");
    shutdown(pConn->hSock, 0x02);
    close(pConn->hSock);
    delete pConn;
    return 0;
}

void TelnetSvr::run_svr()
{
    HV_Trace(5, "<TelnetSvr>::run_svr\n");

    socklen_t addr_len = sizeof(struct sockaddr_in);

    while (!m_fExit)
    {
        // Comment by Shaorg: ����new������pConn���û�ֱ�ӹر�telnet�ն�ʱ���ܻ�ò���delete��������ڴ�й©�����⡣
        SConn* pConn = new SConn;

        HvAccept(m_hSockSvr,pConn->hSock);

        if (pConn->hSock == -1)
        {
            HV_Trace(5, "accept error!\n");
            delete pConn;
            continue;
        }

        HvGetPeerName(pConn->hSock,
			&pConn->addr.sin_addr.s_addr,
			&pConn->addr.sin_port
			);

        pConn->pLog = &m_log;

        HV_Trace(5, "====connect from %s:%d ====\n",
                 inet_ntoa(pConn->addr.sin_addr),
                 ntohs(pConn->addr.sin_port)
                );

        handle_conn((void*)pConn);
    }
}

int TelnetSvr::SendAll(HV_SOCKET_HANDLE sock, char * buf, int len)
{
    int nSendAll;
    int nSend;

    for (nSendAll = 0; nSendAll < len;)
    {
        nSend = Send(sock, buf + nSendAll, len - nSendAll, MSG_NOSIGNAL);
        if (nSend == -1)
            return -1;

        nSendAll += nSend;
    }
    return nSendAll;
}

#ifdef _CAM_APP_
#include "../CamApp/DataCtrl.h"  // ��ȡģ���Լ�Ӳ�̵�״̬��Ϣ
#endif

int TelnetSvr::send_hdd_info(int hSock, char* buf)
{
    /*
    char* pInfo = "\r\nHDD����";
    int len = strlen(pInfo);
    memcpy(buf, pInfo, len);
    */

    int len = BUF_LEN;

#ifdef _CAM_APP_
    g_pCamApp->GetHddStateInfo(buf, len);
#else
    // todo: ����ƽ̨�µĻ�ȡӲ����Ϣ
    strcpy(buf, "ok");
    len = strlen(buf);
#endif

    len = u2g(buf, len, buf, BUF_LEN);

    return ( (-1 == SendAll(hSock, buf, len)) || (-1 == SendAll(hSock, "\r\n", 2)) ) ? 0 : 1;
}

int TelnetSvr::send_mod_info(int hSock, char* buf)
{
    /*
    char* pInfo = "\r\nģ������";
    int len = strlen(pInfo);
    memcpy(buf, pInfo, len);
    */

    int len = BUF_LEN;

#ifdef _CAM_APP_
    g_pCamApp->GetModStateInfo(buf, len);
#else
    // todo: ����ƽ̨�µĻ�ȡģ����Ϣ
    strcpy(buf, "ok");
    len = strlen(buf);
#endif

    len = u2g(buf, len, buf, BUF_LEN);

    return ( (-1 == SendAll(hSock, buf, len)) || (-1 == SendAll(hSock, "\r\n", 2)) ) ? 0 : 1;
}

int  TelnetSvr::rw_reg_info(int hSock, char* buf)
{
    int fd;
    int len = BUF_LEN;
    DSPLinkMem		reg;
    int i,blankpos;

    fd = open("/dev/DSPLink", O_RDWR, S_IRUSR | S_IWUSR);

    if(!fd)
    {
        HV_Trace(5, "Open DSPLink failed!\n");
        return -1;
    }

L_DISPLAY_RWREG_MENU:
    send_rwreg_menu(hSock, buf);

    if(-1 == Recv(hSock, buf, BUF_LEN, 0))
        return -1;

    switch(buf[0])
    {
    case 'r':
    case 'R':
        for(i=0; i<BUF_LEN; i++)
        {
            if(buf[i] == ' ')
            {
                blankpos = i;
                break;
            }
        }

        reg.addr    = 0xffffffff&strtol(&buf[blankpos], NULL, 16);
        reg.value   = ioctl(fd, GET_REG, reg.addr);

        sprintf(buf,
			"!----Read  Finish----!\r\n  [Addr ]:0x%08X\r\n  [Value]:0x%08X\r\n",
			reg.addr,
			reg.value
			);

        len = strlen(buf);
        len = u2g(buf, len, buf, BUF_LEN);

        SendAll(hSock, buf, len);
        SendAll(hSock, "\r\n", 2);

        goto L_DISPLAY_RWREG_MENU;

        break;
    case 'w':
    case 'W':
        for(i=0; i<BUF_LEN; i++)
        {
            if(buf[i] == ' ')
            {
                blankpos = i;
                break;
            }
        }

        for(++i; i<BUF_LEN; i++)
        {
            if(buf[i] == ' ')
            {
                break;
            }
        }

        reg.addr    = 0xffffffff&strtol(&buf[blankpos], NULL, 16);
        reg.value   = 0xffffffff&strtol(&buf[i], NULL, 16);

        ioctl(fd, SET_REG, &reg);

        sprintf(buf,
			"!----Write Finish----!\r\n  [Addr ]:0x%08X\r\n  [Value]:0x%08X\r\n",
			reg.addr,
			reg.value
			);

        len = strlen(buf);
        len = u2g(buf, len, buf, BUF_LEN);

        SendAll(hSock, buf, len);
        SendAll(hSock, "\r\n", 2);

        goto L_DISPLAY_RWREG_MENU;

        break;
    case 'q':
    case 'Q':
        close(fd);
        break;
    default:
        goto L_DISPLAY_RWREG_MENU;
        break;
    }

    return 0;
}

int TelnetSvr::exec_user_cmd(int hSock, char* buf, char cmd)
{
    int len = BUF_LEN;

    switch (cmd)
    {
    case '4':
    {
        strcpy(buf, "\r\n");
        GetVersionStr(buf+2);
    }
    break;

#ifdef _CAM_APP_
    case '5':
    {
        if (g_pCamApp != NULL)
        {
            g_pCamApp->OpenPL();
            strcpy(buf, "\nOpenPL");
        }
        else
        {
            strcpy(buf, "\nNULL");
        }
    }
    break;

    case '6':
    {
        if (g_pCamApp != NULL)
        {
            g_pCamApp->ClosePL();
            strcpy(buf, "\nClosePL");
        }
        else
        {
            strcpy(buf, "\nNULL");
        }
    }
    break;
#endif

    case '!':
    {
        DWORD32 dwDataLen = 0;
        char* pbuf = new char [32 * 1024];
        if (pbuf != NULL)
        {
            GetAllDebugStateInfo((BYTE8*)pbuf, &dwDataLen);

            char* pDataPtr = pbuf;

            // ���������ݻ�����⣬ÿ�����8K
            while (dwDataLen != 0)
            {
                int iSend = 8 * 1024;
                if (dwDataLen > iSend)
                {
                    dwDataLen -= iSend;
                }
                else
                {
                    iSend = dwDataLen;
                    dwDataLen = 0;
                }
                if (-1 == SendAll(hSock, pDataPtr, iSend))
                {
                    break;
                }
                pDataPtr += iSend;
            }
            delete [] pbuf;
            strcpy(buf, "\r\n------------------");
        }
        else
        {
            strcpy(buf, "\r\nmemory not enough");
        }
    }
    break;

    case '@':
    {
        DWORD32 dwDataLen = 0;
        char* pbuf = new char [32 * 1024];
        if (pbuf != NULL)
        {
            GetAllResetLog((BYTE8*)pbuf, &dwDataLen);

            char* pDataPtr = pbuf;

            // ���������ݻ�����⣬ÿ�����8K
            while (dwDataLen != 0)
            {
                int iSend = 8 * 1024;
                if (dwDataLen > iSend)
                {
                    dwDataLen -= iSend;
                }
                else
                {
                    iSend = dwDataLen;
                    dwDataLen = 0;
                }
                if (-1 == SendAll(hSock, pDataPtr, iSend))
                {
                    break;
                }
                pDataPtr += iSend;
            }

            delete [] pbuf;
            strcpy(buf, "\r\n------------------");
        }
        else
        {
            strcpy(buf, "\r\nmemory not enough");
        }
    }
    break;

    default:
        strcpy(buf, "\nNULL");
        break;
    }

    //len = u2g(buf, len, buf, BUF_LEN);
    len = strlen(buf);

    return ( (-1 == SendAll(hSock, buf, len)) || (-1 == SendAll(hSock, "\r\n", 2)) ) ? 0 : 1;
}

/////////////////////////////////////////////////////////////

// ��ȡ�ںˡ������汾��Ϣ
static void ReadInfoFromFile(char* file, char* buf, int len)
{
    FILE* fp = NULL;
    char info[256] = {0};
    fp = fopen(file, "r");
    if (fp == NULL)
    {
        perror("open file error");
        return;
    }
    fread(info, 1, sizeof(info), fp);
    if ( buf != NULL && len > (int)strlen(info) )
    {
        strcpy(buf, info);
    }
    fclose(fp);
}

static void GetVersionStr(char* pBuf)
{
    char szValue[512] = {0};
    int iLen;

    GetSN(szValue, sizeof(szValue));
    iLen = sprintf(pBuf, "[SerialNo]: %s\r\n", szValue);
    pBuf += iLen;

    GetUbootVersion(szValue, sizeof(szValue));
    iLen = sprintf(pBuf, "[Uboot ver]: %s\r\n", szValue);
    pBuf += iLen;

#if defined(_CAM_APP_) || defined(SINGLE_BOARD_PLATFORM)
    iLen = sprintf(pBuf, "[App ver]: %s %s %s\r\n\r\n",
                   PSZ_DSP_BUILD_NO,
                   DSP_BUILD_DATE,
                   DSP_BUILD_TIME);
    pBuf += iLen;
#endif

//  ע�����ô˷����������Ӳ鿴�汾��ʱ����ֳ��������豸��λ�����
//    DebugSystemPrint("cat /proc/version", szValue, sizeof(szValue));
//    iLen = sprintf(pBuf, "[Kernel ver]: %s\r\n", szValue);
//    pBuf += iLen;

    // ��ֱ�Ӷ��ļ�����
    ReadInfoFromFile("/proc/version", szValue, sizeof(szValue));
    iLen = sprintf(pBuf, "[Kernel ver]: %s\r\n", szValue);
    pBuf += iLen;

#ifdef _CAM_APP_
    iLen = sprintf(pBuf, "[FPGA ver]: 0x%0X\r\n", g_nFpgaVersion);
    pBuf += iLen;
#endif

    GetCPLDVersion(szValue, 1);
    iLen = sprintf(pBuf, "[CPLD ver]: 0x%0X\r\n", (int)szValue[0]);
    pBuf += iLen;

//    DebugSystemPrint("cat /sys/swdev/version", szValue, sizeof(szValue));
//    iLen = sprintf(pBuf, "[slw-dev.ko ver]: %s\r\n", szValue);
//    pBuf += iLen;

    ReadInfoFromFile("/sys/swdev/version", szValue, sizeof(szValue));
    iLen = sprintf(pBuf, "[slw-dev.ko ver]: %s\r\n", szValue);
    pBuf += iLen;
}
