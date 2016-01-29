// ���ļ����������WINDOWS-936��ʽ
#include <ctime>
#include <iostream>
#include <fstream>
#include "qftp.h"
#include "swbasetype.h"
#include "HvSockUtils.h"


#ifdef CHIP_6467
#include "HvSockUtils.h"
#endif

using namespace std;

using namespace nsFTP;

CQFTP::CQFTP(const string& strFtpIp,const string& strUserName, const string& strPassWord, UINT nTimeout)
        : m_socketCtrlConnect(0)
        , m_wTimeout(nTimeout)
        , m_strFtpIP(strFtpIp)
        , m_strUserName(strUserName)
        , m_strPassWord(strPassWord)
{

}

CQFTP::~CQFTP()
{
    if (0 != m_socketCtrlConnect)
    {
        CloseSocket(m_socketCtrlConnect);      // ֱ�ӹر�����
        m_socketCtrlConnect = 0;
    }
}

HRESULT SvSetRecvTimeOut(const int& hSocket, int iMS)
{
    struct timeval uTime;
    uTime.tv_sec = iMS / 1000;
    uTime.tv_usec = (iMS % 1000) * 1000;
    int iRet = setsockopt(hSocket, SOL_SOCKET, SO_RCVTIMEO, (char*)&uTime, sizeof(timeval));
    return (0 == iRet) ? S_OK : E_FAIL;
}

HRESULT SvSetSendTimeOut(const int& hSocket, int iMS)
{
    struct timeval uTime;
    uTime.tv_sec = iMS / 1000;
    uTime.tv_usec = (iMS % 1000) * 1000;
    int iRet = setsockopt(hSocket, SOL_SOCKET, SO_SNDTIMEO, (char*)&uTime, sizeof(timeval));
    return (0 == iRet) ? S_OK : E_FAIL;
}

//注：Linux下异步SOCKET的连接并没有超时的说法，因此在调用该函数失败后请自行Sleep一段时间。
HRESULT SvConnect(const int& hSocket, const char* pszIPv4, WORD16 wPort, int iTimeout/*=-1*/)
{
    if (INVALID_SOCKET == hSocket || NULL == pszIPv4 )	return E_INVALIDARG;

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(wPort);
    addr.sin_addr.s_addr = inet_addr(pszIPv4);

    int iRtn = 0;
    if ( -1 == iTimeout )
    {
        iRtn = connect(hSocket, (sockaddr*)&addr, sizeof(addr));
        return (0 == iRtn) ? S_OK : E_FAIL;
    }

    HRESULT hr = E_FAIL;

    // 设为非阻塞模式
    int flags = fcntl(hSocket, F_GETFL, 0);
    flags |= O_NONBLOCK;
    fcntl(hSocket, F_SETFL, flags);

    iRtn = connect(hSocket, (sockaddr*)&addr, sizeof(addr));

    if ( 0 == iRtn )
    {
        hr = S_OK;
    }
    else
    {
        fd_set w;
        FD_ZERO(&w);
        FD_SET(hSocket, &w);

        struct timeval timeout;
        timeout.tv_sec = iTimeout / 1000;
        timeout.tv_usec = (iTimeout % 1000) * 1000;

        int ret = select(hSocket + 1, 0, &w, 0, &timeout);
        if (ret <= 0)
        {
            hr = E_FAIL;
        }
        else if (!FD_ISSET(hSocket, &w))
        {
            hr = E_FAIL;
        }
        else
        {
            ret = 0;
            int errLen = sizeof(int);
            if (getsockopt(hSocket, SOL_SOCKET, SO_ERROR, &ret, (socklen_t*)&errLen) < 0
                || ret != 0)
            {
                hr = E_FAIL;
            }
            else
            {
                hr = S_OK;
            }
        }
    }

    // 设为阻塞模式
    flags = fcntl(hSocket, F_GETFL, 0);
    flags &= (~O_NONBLOCK);
    fcntl(hSocket, F_SETFL, flags);
    return hr;
}

INT CQFTP::ConnectSocket(SOCKET& hConnectSocket, const CHAR* szIP, INT iPort)
{
#ifdef WIN32
    SOCKADDR_IN addr;
    INT err;
    WORD wVersionRequested;
    WSADATA wsaData;
    wVersionRequested = MAKEWORD(1,1);
    err = WSAStartup(wVersionRequested, &wsaData);
    if(err != 0)
    {
        WSACleanup();
        return ERROR_QFTP_PARAMETER_INVALID;
    }
    if( LOBYTE(wsaData.wVersion) != 1 ||
        HIBYTE(wsaData.wVersion) != 1 )
    {
        WSACleanup();
        return ERROR_QFTP_PARAMETER_INVALID;
    }
#endif

    hConnectSocket = socket(AF_INET, SOCK_STREAM, 0);
    // ����socketʧ��
    if ( INVALID_SOCKET == hConnectSocket )
    {
        return ERROR_QFTP_SOCKET_INITIALIZE_FAILED;
    }

#ifdef WIN32
	if (0 == m_wTimeout)
	{
		int iRtn = connect(hConnectSocket, (sockaddr*)&addr, sizeof(addr));
		if (iRtn != 0)
		{
			return ERROR_QFTP_CONNECT_INVALID;
		}
	}
	else
	{
		struct sockaddr_in addr;
		addr.sin_addr.s_addr = inet_addr(szIP);
	    addr.sin_family = AF_INET;
	    addr.sin_port = htons(iPort);

	    // ��Ϊ������ģʽ
		u_long cmd_value = 1;
		ioctlsocket(hConnectSocket, FIONBIO, &cmd_value);

		int iRtn = connect(hConnectSocket, (sockaddr*)&addr, sizeof(addr));
		if (iRtn != 0)
		{
			struct timeval timeout ;
			fd_set r;
			FD_ZERO( &r );
			FD_SET(m_skt, &r);
			timeout.tv_sec = 0;
			timeout.tv_usec = m_wTimeout * 1000;
			int ret = select(0, NULL, &r, NULL, &timeout);
			if (ret == 0 || ret == SOCKET_ERROR)
			{
				return ERROR_QFTP_CONNECT_INVALID;
			}
		}
		// ��Ϊ����ģʽ
		u_long cmd_value = 0;
		ioctlsocket(hConnectSocket, FIONBIO, &cmd_value );
	}
    return ERROR_QFTP_OK;
#elif CHIP_6467
    if (S_OK == HvConnect(hConnectSocket, szIP, iPort, m_wTimeout))
    {
    	HvSetRecvTimeOut(hConnectSocket, m_wTimeout/2);
    	HvSetSendTimeOut(hConnectSocket, m_wTimeout);
    	return ERROR_QFTP_OK;
    }
    return ERROR_QFTP_CONNECT_INVALID;
	
#else
	//һ���linuxƽ̨
	 if (S_OK == SvConnect(hConnectSocket, szIP, iPort, m_wTimeout * 1000))
    {
    	SvSetRecvTimeOut(hConnectSocket, m_wTimeout/2 * 1000);
    	SvSetSendTimeOut(hConnectSocket, m_wTimeout * 1000);
    	return ERROR_QFTP_OK;
    }
	return ERROR_QFTP_CONNECT_INVALID;
	
#endif
}

// �������ݣ�ʹ��select����ʱʱ��Ĭ����Ϊ10s
INT CQFTP::Receive(SOCKET& socket, CHAR* pszBuffer, INT nSize)
{
    int nBytesReceived = recv(socket, pszBuffer, nSize, 0);
    if (nBytesReceived == SOCKET_ERROR)
    {
        debug("Receive error.\n");
        return ERROR_QFTP_RECV_ERROR;
    }

    return nBytesReceived;
}

// �������ݣ�ʹ��select����ʱʱ��Ĭ����Ϊ10s
INT CQFTP::Send(SOCKET& socket, const CHAR* pszBuffer, INT nSize)
{
    int nBytesSent=send(socket, pszBuffer, nSize, MSG_NOSIGNAL);
    if (nBytesSent != nSize)
    {
    	return ERROR_QFTP_SEND_ERROR;
    }
    return nBytesSent;
}

INT CQFTP::CloseSocket(SOCKET& hConnectSocket)
{
    if ( hConnectSocket != 0 )
    {
#ifdef WIN32
        closesocket(hConnectSocket);
#else
        close(hConnectSocket);
#endif
        hConnectSocket = 0;  // ��Ϊ0
        return ERROR_QFTP_OK;
    }
    else
    {
        return ERROR_QFTP_PARAMETER_INVALID;
    }
}

/**
 * LoginFtpServer - ��½������
 *
 * ����ֵ��
 * ERROR_QFTP_OK������
 * ERROR_QFTP_CONNECT_FAILED������ʧ��
 * ERROR_QFTP_SEND_ERROR����������ʧ��
 * ERROR_QFTP_USER_ERROR���û������������
 *
 */
INT CQFTP::LoginFtpServer(void)
{
    INT iRetCode = ERROR_QFTP_UNKNOWNERROR;
    string strUser("USER ");
    string strPass("PASS ");
    strUser.append(m_strUserName);
    strUser.append("\r\n");

    strPass.append(m_strPassWord);
    strPass.append("\r\n");
	debug("want to LoginFtpServer\n");
    // ����
    if (ERROR_QFTP_OK != ConnectSocket(m_socketCtrlConnect, m_strFtpIP.c_str(), 21))
    {
        iRetCode = ERROR_QFTP_CONNECT_FAILED;
        goto END;
    }
    // ���Ӻ󣬵õ���ӭ��Ϣ
    m_iLastRecvAnswerCode = RecvAnswer(m_socketCtrlConnect, NULL, 0);
    if ( 220 != m_iLastRecvAnswerCode )
    {
        goto END;
    }
    // �û���
    iRetCode = Send(m_socketCtrlConnect, strUser.c_str(), (INT)strUser.size());
    if (iRetCode < 0)
    {
        goto END;
    }
    m_iLastRecvAnswerCode = RecvAnswer(m_socketCtrlConnect, NULL, 0);
    if (331 != m_iLastRecvAnswerCode)
    {
        iRetCode = m_iLastRecvAnswerCode;
        goto END;
    }
    // ���� ��ȷ����230�����󷵻�530
    iRetCode = Send(m_socketCtrlConnect, strPass.c_str(), (INT)strPass.size());
    if ( iRetCode < 0 )
    {
        goto END;
    }
    m_iLastRecvAnswerCode = RecvAnswer(m_socketCtrlConnect, NULL, 0);
    if (230 != m_iLastRecvAnswerCode)
    {
        iRetCode = ERROR_QFTP_USER_ERROR;
        goto END;
    }
    // һ������
    iRetCode = ERROR_QFTP_OK;

END:
    if ( ERROR_QFTP_OK != iRetCode )
    {
        CloseSocket(m_socketCtrlConnect);
    }

    return iRetCode;
}

/**
 * LogoutFtpServer - �ǳ���
 *
 * ����ֵ��
 * ERROR_QFTP_OK������
 * ERROR_QFTP_SEND_ERROR����������ʧ��
 *
 */
INT CQFTP::LogoutFtpServer()
{
     if (0 == m_socketCtrlConnect)
    {
        return ERROR_QFTP_PARAMETER_INVALID;
    }

    if ( Send(m_socketCtrlConnect, "QUIT\r\n", (INT)strlen("QUIT\r\n")) < 0 )
    {
        return ERROR_QFTP_SEND_ERROR;
    }
    m_iLastRecvAnswerCode = RecvAnswer(m_socketCtrlConnect, NULL, 0);

    if (m_iLastRecvAnswerCode != 221 )
    {
        CloseSocket(m_socketCtrlConnect);  // ��ʹ�ǳ�ʧ��Ҳ�ر�sokcet
        return ERROR_QFTP_LOGOUT_FAILED;
    }

    CloseSocket(m_socketCtrlConnect);

    return ERROR_QFTP_OK;
}

INT CQFTP::RecvAnswer(SOCKET& hConnectSocket, CHAR* pszRecvBuf, INT iLen)
{
    CHAR szRetCode[4];
    INT iRecvNum=0;
    INT iRecvBufferLen = 256;

    memset(m_szRecvBuf, 0, iRecvBufferLen);

    iRecvNum = Receive(hConnectSocket, m_szRecvBuf, iRecvBufferLen);
    if ( iRecvNum < 0 )
    {
        return iRecvNum;
    }
    else
    {
        memset(m_szRecvBuf + iRecvNum, 0, 1);

        if ( pszRecvBuf != NULL && iLen > iRecvNum )
        {
            strcpy(pszRecvBuf, m_szRecvBuf);
        }

        szRetCode[0] = m_szRecvBuf[0];
        szRetCode[1] = m_szRecvBuf[1];
        szRetCode[2] = m_szRecvBuf[2];
        szRetCode[3] = '\0';

#ifdef PRINT_MESSAGE
        //��ӡ��FTP���������صľ���Ӧ������
        debug("%s", m_szRecvBuf);
#endif
        return atoi(szRetCode);
    }
}

INT CQFTP::TestConnectValidity(void)
{

    if ( 0 == m_socketCtrlConnect )
    {
        return ERROR_QFTP_PARAMETER_INVALID;
    }

    int iRet = SendCommand(m_socketCtrlConnect, "NOOP \r\n");
    debug("socket: %d\n", m_socketCtrlConnect);
    if (200 == iRet)
    {
        return ERROR_QFTP_OK;
    }
    else
    {
        return ERROR_QFTP_CONNECT_INVALID;
    }
}

// ��������
// �ʺ�RMD��DELE��CWD����550��ʾ�޴�Ŀ¼���ļ����������ȷ(��DELE dir)
// NLST��LIST����450��ʾ�޴�Ŀ¼���ļ�
// MKD����550��ʾĿ¼�Ѿ�����
// ������ERROR_QFTP_SEND_ERROR���ɹ�������Ӧ��
INT CQFTP::SendCommand(SOCKET& hConnectSocket, const CHAR* pbCommand)
{
    INT iRet;
    iRet = Send(hConnectSocket, pbCommand, (INT)strlen(pbCommand));
    if (iRet < 0)
    {
        return iRet;
    }

    m_iLastRecvAnswerCode = RecvAnswer(hConnectSocket, NULL, 0);
    return m_iLastRecvAnswerCode;   // ֱ�ӷ�����Ӧ��
}

// ����Ŀ¼
INT CQFTP::MakeDirectory(const CHAR* DirectoryPath)
{
    CHAR szCommand[256] = {0};

    sprintf(szCommand, "XMKD %s\r\n", DirectoryPath);
    return SendCommand(m_socketCtrlConnect, szCommand);
}

// �ı�Ŀ¼
INT CQFTP::ChangeDirectory(const CHAR* DirectoryPath)
{
    CHAR szCommand[256]= {0};

    sprintf(szCommand, "XCWD %s\r\n", DirectoryPath);

    return SendCommand(m_socketCtrlConnect, szCommand);
}

// ��������
INT CQFTP::SendData(SOCKET& hConnectSocket, CHAR* pbDataBuffer, INT nSize)
{
#define SENDSIZE 2*1024*1024
    INT nBytesSent = 0;
    INT nBytesThisTime;
    INT nBytesOneTime = SENDSIZE;   // ÿ����෢SENDSIZE
    CHAR* pTmp = pbDataBuffer;

    do{
        if (nSize < SENDSIZE)
        {
            nBytesOneTime = nSize - nBytesSent;
        }
        nBytesThisTime = Send(hConnectSocket, pTmp, nBytesOneTime); // һ�η�nBytesOneTime
        // ���select�������ͳ������ͳ�ʱ�������ط��ͳ���
        if (ERROR_QFTP_SELECT_ERROR == nBytesThisTime || ERROR_QFTP_SEND_ERROR == nBytesThisTime || \
            ERROR_QFTP_SEND_TIMEOUT == nBytesThisTime)
        {
            return ERROR_QFTP_SEND_ERROR;
        }

        nBytesSent += nBytesThisTime;
        pTmp += nBytesThisTime;

        INT nLeft = nSize - nBytesSent;
        if (nLeft < nBytesOneTime)
        {
            nBytesOneTime = nLeft;
        }
    } while (nBytesSent < nSize);

    return ERROR_QFTP_OK;
}

//��PASV�����Ӧ����Ϣ����ȡ�������Ķ˿�
//�磺227 Entering Passive Mode (192,168,1,110,12,34).
INT CQFTP::GetPortForPASV(CHAR* szRecvBuf)
{
    INT nPort = 0;
    INT iPos1 = 0;
    INT iPos2 = 0;
    INT iPos3 = 0;
    INT iBufLen = 0;
    CHAR buf1[8] = {0};
    CHAR buf2[8] = {0};

    iBufLen = (INT)strlen(szRecvBuf);

    if ( '2'==szRecvBuf[0] && '2' == szRecvBuf[1] && '7'==szRecvBuf[2] )
    {
        for ( INT n=(iBufLen-1); n>=0; --n )
        {
            if ( ',' == szRecvBuf[n] )
            {
                if ( 0 == iPos2 )
                {
                    iPos2 = n;
                }
                else
                {
                    iPos1 = n;
                    break;
                }
            }
            else if ( ')' == szRecvBuf[n] )
            {
                iPos3 = n;
            }
        }

        memset(buf1, 0, sizeof(buf1));
        memset(buf2, 0, sizeof(buf2));
        memcpy(buf1, szRecvBuf+iPos1+1, iPos2-iPos1-1);
        memcpy(buf2, szRecvBuf+iPos2+1, iPos3-iPos2-1);

        nPort = atoi(buf1)*256 + atoi(buf2);

        return nPort;
    }

    return -1;
}

/**************************************************************************
* OpenPassiveDataConnection - ������ͨ�������NLST/LIST/RETR��STOR/APPE
* hConnectSocket������ͨ��socket���������
* hDataSocket������������ͨ��socket����������
* pbCommand��������ͨ���й������NLST/LIST/RETR��STOR/APPE
*
* �ú���ֻ�������������ͨ�������������ǰ�رո���������
*************************************************************************/
INT CQFTP::OpenPassiveDataConnection(SOCKET& hConnectSocket, SOCKET& hDataSocket, CHAR* pbCommand)
{
    WORD16 usPort;
    INT iRetCode = 0;
    const INT nPassiveLen = 128;
    CHAR szRecvPassiveBuffer[nPassiveLen] = {0};     // PASV�������Ϣ����������ͨ���˿ں�

    iRetCode = Send(hConnectSocket, "PASV \r\n", (INT)strlen("PASV \r\n"));
    if (iRetCode < 0)
    {
        goto END;
    }
    // 227 Entering Passive Mode (172,18,222,100,6,170).
    m_iLastRecvAnswerCode = RecvAnswer(hConnectSocket, szRecvPassiveBuffer, nPassiveLen);
    if (227 != m_iLastRecvAnswerCode)
    {
        iRetCode = ERROR_QFTP_CONNECT_FAILED;
        goto END;
    }
    // �õ�����ͨ���˿ں�
    usPort = GetPortForPASV(szRecvPassiveBuffer);

    // ��������ͨ��
    if (ERROR_QFTP_OK != ConnectSocket(hDataSocket, m_strFtpIP.c_str(), usPort))
    {
        iRetCode = ERROR_QFTP_CONNECT_FAILED;
        goto END;
    }

    // ��������
    iRetCode = Send(hConnectSocket, pbCommand, (INT)strlen(pbCommand));
    if (iRetCode < 0)
    {
        goto END;
    }
    // ��Ŀ¼���ļ�
    // ��RecvAnswer����� 226 Transfer complete������226�����ر���������
    // �����Ա�����ʱ(��RecvAnswer) g_szRecvBuf����Ϊǰһ������
    m_iLastRecvAnswerCode = RecvAnswer(hConnectSocket, NULL, 0);

    if (150 == m_iLastRecvAnswerCode || 125 == m_iLastRecvAnswerCode)  // 125 150 �ɹ�ִ������
    {
        iRetCode = ERROR_QFTP_OK;
        goto END;
    }
    else if (550 == m_iLastRecvAnswerCode || 450 == m_iLastRecvAnswerCode)   // Ŀ¼���ļ�������
    {
        iRetCode = ERROR_QFTP_FILEORDIR_NOT_FOUND;
        goto END;
    }
    else if (226 == m_iLastRecvAnswerCode)       // ���Ŀ¼���������(���ǳ������������������)
    {
        iRetCode = ERROR_QFTP_DATA_CONNECT_CLOSED;
        goto END;
    }

END:
    if (iRetCode != ERROR_QFTP_OK)
    {
        CloseSocket(hDataSocket);
    }
    return iRetCode;
}

////////////////////////////////// �ϴ����
/**
 * SaveBufferToFtp - ���滺�������ݵ�������
 *
 * hConnectSocket������ͨ��socket
 * pbFileName���ļ�����
 * pbBuffer��������
 * nSize����С
 * fAppend���Ƿ�append(�����ԣ���������֧��APPE����)
 *
 * ����ֵ��
 * ERROR_QFTP_OK������
 * ERROR_QFTP_SEND_ERROR����������/����ʧ��
 */
INT CQFTP::SaveBufferToFtp(SOCKET& hConnectSocket, const CHAR* pbFileName, CHAR* pbBuffer, INT nSize, BOOL fAppend)
{
    CHAR szCommand[256] = {0};
    SOCKET hDataSocket = 0;
    INT iRet;

    if (fAppend)
    {
        sprintf(szCommand, "APPE %s\r\n", pbFileName);   // δ����
    }
    else
    {
        sprintf(szCommand, "STOR %s\r\n", pbFileName);
    }

    iRet = Send(hConnectSocket, "TYPE I\r\n", (INT)strlen("TYPE I\r\n"));
    if ( iRet < 0)
    {
        return ERROR_QFTP_SEND_ERROR;
    }
    m_iLastRecvAnswerCode = RecvAnswer(hConnectSocket, NULL, 0);
    if (200 != m_iLastRecvAnswerCode)
    {
        return ERROR_QFTP_SEND_ERROR;
    }

    // ��������ͨ��
    iRet = OpenPassiveDataConnection(hConnectSocket, hDataSocket, szCommand);
    if (iRet < 0)
    {
        return ERROR_QFTP_SEND_ERROR;
    }

    iRet = SendData(hDataSocket, pbBuffer, nSize);
    if (iRet < 0)
    {
        return ERROR_QFTP_SEND_ERROR;
    }

    // �ر�����ͨ��
    CloseSocket(hDataSocket);

    m_iLastRecvAnswerCode = RecvAnswer(hConnectSocket, NULL, 0);

    return ERROR_QFTP_OK;
}

/**
 * SaveDataToFtp - �������ݵ�������
 *
 * strFileFullName��������·�����ļ�����
 * pbDataForSend��������
 * uDataLen����������С
 * fAppend���Ƿ��Ը�����ʽ����(�����ԣ���������֧��)
 */
INT CQFTP::SaveDataToFtp(string strFileFullName, CHAR* pbDataForSend, UINT uDataLen, BOOL fAppend)
{
    INT iRet;
    if ( 0 == m_socketCtrlConnect || strFileFullName.empty() || NULL == pbDataForSend)
    {
        return ERROR_QFTP_PARAMETER_INVALID;
    }
    INT iDestPathLen = (INT)strFileFullName.size();
    if ( iDestPathLen >= (MAX_CMD_LEN-16) )
    {
        return ERROR_QFTP_PARAMETER_INVALID;
    }

    size_t iSlash;
    string strDirectory;
    iSlash = strFileFullName.find_last_of('/');
    if (iSlash != string::npos)
        strDirectory = strFileFullName.substr(0, iSlash+1); // ������_Ŀ¼��_(��/xx/2011/08/15/14/)

    if (strDirectory.empty())   // ����Ŀ¼��ֱ���ϴ�
    {
        iRet = SaveBufferToFtp(m_socketCtrlConnect, strFileFullName.c_str(), pbDataForSend, uDataLen, fAppend);
        if (iRet < 0)
        {
            return iRet;
        }
        return ERROR_QFTP_OK;
    }
    string strTemp;
    iRet = ChangeDirectory(strDirectory.c_str());  // ���Խ����Ŀ¼(������ڣ��Ͳ����ٴ���)
    if (550 == iRet) // Ŀ¼�����ڣ����������ϴ�
    {
        for (size_t i = 1; i < strDirectory.size(); i++)
        {
            if ('/' == strDirectory.at(i))  // �ҵ�Ŀ¼
            {
                strTemp = strDirectory.substr(0, i);
                MakeDirectory(strTemp.c_str());     // ������ڣ����������ô���
            }
        }
        // ������ϣ�����
        iRet = SaveBufferToFtp(m_socketCtrlConnect, strFileFullName.c_str(), pbDataForSend, uDataLen, fAppend);
        if (iRet < 0)
        {
            return iRet;
        }
        return ERROR_QFTP_OK;
    }
    // ���ڣ�ֱ���ϴ�(������·��)
    else
    {
        iRet = SaveBufferToFtp(m_socketCtrlConnect, strFileFullName.c_str(), pbDataForSend, uDataLen, fAppend);
        if (iRet < 0)
        {
            return iRet;
        }
        return ERROR_QFTP_OK;
    }
    return ERROR_QFTP_OK;
}

// д���������ݵ�FTP��
// szFileName: ������·�����ļ���
// szBuffer��������
// dwLength����������С
// �ɹ�����0��ʧ�ܷ���ERROR_QFTP_SEND_ERROR
INT CQFTP::WriteFileToFtp(CHAR* szFileName, CHAR* szBuffer, DWORD32 dwLength)
{
    INT iRet;
    iRet = SaveDataToFtp(szFileName, szBuffer, dwLength, 0);
    if (iRet != ERROR_QFTP_OK)
    {
        return ERROR_QFTP_SEND_ERROR;
    }

    return ERROR_QFTP_OK;
}

/////////////////////////////////////////////////////////////////////
// �ӷ����������ļ����ڴ���
// pbRemoteFileΪ��·�����ļ�
// ������ERROR_QFTP_RECV_ERROR
// û���ҵ��ļ�����ERROR_QFTP_NOT_FOUND
// �ɹ�����ʵ�ʵĳ���(���������ļ���ȡ��С���Ǹ�)
INT CQFTP::GetFileFromFtp(const CHAR* pbRemoteFile, CHAR* szBuffer, DWORD32 dwBufLen)
{
    SOCKET hDataSocket;
    INT iRet = 0;
    CHAR szCommand[256] = {0};
    CHAR szDataBuffer[8*1024] = {0};    // 8KB ������

    iRet = Send(m_socketCtrlConnect, "TYPE I\r\n", (INT)strlen("TYPE I\r\n"));
    if (iRet < 0)
    {
        return ERROR_QFTP_RECV_ERROR;
    }

    m_iLastRecvAnswerCode = RecvAnswer(m_socketCtrlConnect, NULL, 0);
    if (200 != m_iLastRecvAnswerCode)
    {
        return ERROR_QFTP_RECV_ERROR;
    }

    sprintf(szCommand, "RETR %s\r\n", pbRemoteFile);

    // ������ͨ��
    iRet = OpenPassiveDataConnection(m_socketCtrlConnect, hDataSocket, szCommand);
    if (ERROR_QFTP_FILEORDIR_NOT_FOUND == iRet) // û���ҵ��ļ�
    {
        return ERROR_QFTP_NOT_FOUND;
    }

    DWORD32 dwFileSize = 0;
    // we get the message like  "150 Opening BINARY mode data connection for 1234.dat (29068 bytes)"
    // we need the file size: 29068
    sscanf(m_szRecvBuf, "%*[^(](%d[^ ]", &dwFileSize);  // ����ļ���С

    INT nNum = 0;
    CHAR* pTmp = szBuffer;
    INT nTmpSize = dwFileSize > dwBufLen ? dwBufLen : dwFileSize;
    INT nTrueSize = nTmpSize;

    if (dwFileSize <= 0)//��ʱFTP����������һ������150,����125(ֻ�гɹ���ʾ��û���ļ���С)
    {
    	nTrueSize = 0;
    	nNum = Receive(hDataSocket, szDataBuffer, sizeof(szDataBuffer));
    	while (nNum > 0)
    	{
    		memcpy(szBuffer + nTrueSize, szDataBuffer, nNum);
    		nTrueSize += nNum;
    		nNum = Receive(hDataSocket, szDataBuffer, sizeof(szDataBuffer));
    	}
    }
    else
    {
	    // ȡС��
	    while (nTmpSize > 0)
	    {
	        nNum = Receive(hDataSocket, szDataBuffer, sizeof(szDataBuffer));
	        // select�������ճ�����ʱ(�����)��ֱ�ӷ��أ�������
	        if (ERROR_QFTP_SELECT_ERROR == nNum || ERROR_QFTP_RECV_ERROR == nNum || ERROR_QFTP_RECV_TIMEOUT == nNum)
	        {
	            return ERROR_QFTP_RECV_ERROR;
	        }
	        if (nNum > nTmpSize)
	        {
	            nNum = nTmpSize;
	        }
	        memcpy(pTmp, szDataBuffer, nNum);
	        pTmp = pTmp + nNum;
	        nTmpSize -= nNum;
	    }
	}

    // �ر���������
    CloseSocket(hDataSocket);
    m_iLastRecvAnswerCode = RecvAnswer(m_socketCtrlConnect, NULL, 0);

    return nTrueSize;
}

// ��FTP��ָ���ļ�����������
// szFileName��FTP�ļ���
// szBuffer��������
// szBufLen����������С
// [OUT]dwDataLen��ʵ�ʶ�ȡ�Ĵ�С
// ��ȡ�ļ�ʧ�ܻ�ָ��������С��ʵ���ļ����ȣ�����dwDataLenΪ������ָ���Ĵ�С���ɵ������ж��Ƿ����
// ָ�������������ļ�ʵ�ʳ��ȣ�����0��������dwDataLenΪʵ���ļ�����
// ���������쳣����ERROR_QFTP_RECV_ERROR������������0
INT CQFTP::ReadFileFromFtp(CHAR* szFileName, CHAR* szBuffer, DWORD32 dwBufLen, DWORD32& dwDataLen)
{
    INT iRet = 0;
    iRet = GetFileFromFtp(szFileName, szBuffer, dwBufLen);
    if (ERROR_QFTP_RECV_ERROR == iRet)
    {
        return ERROR_QFTP_RECV_ERROR;
    }
    else if (ERROR_QFTP_NOT_FOUND == iRet)  // û���ҵ��ļ�������0������dwDataLenΪ0
    {
        dwDataLen = 0;
        return ERROR_QFTP_OK;
    }
    // �ҵ���������ʵ�����ĳ���
    dwDataLen = iRet;

    return ERROR_QFTP_OK;
}

// ɾ�������ļ�
// szFileName: FTP��������������·�����ļ���(�硰/array1/guest-share/user1/foo.dat��)
// ����ֵ��
// ERROR_QFTP_DEL_ERROR���ļ������ڻ�ɾ��������ȷ
// ERROR_QFTP_OK���ɹ�ɾ���ļ�
INT CQFTP::DeleteOneFile(CHAR* szFileName)
{
    INT iRet = 0;
    CHAR szCommand[256] = {0};

    sprintf(szCommand, "DELE %s\r\n", szFileName);

    iRet = SendCommand(m_socketCtrlConnect, szCommand);
    if (iRet == 550)
    {
        return ERROR_QFTP_DEL_ERROR;
    }

    return ERROR_QFTP_OK;
}

INT CQFTP::List(const CHAR* pbPath)
{
    SOCKET hDataSocket = 0;
    CHAR szCommand[256] = {0};
    CHAR szDataBuffer[16] = {0};
    INT iRet;
    INT nNum;

    sprintf(szCommand, "NLST %s\r\n", pbPath);
    iRet = OpenPassiveDataConnection(m_socketCtrlConnect, hDataSocket, szCommand);
    if (ERROR_QFTP_FILEORDIR_NOT_FOUND == iRet)
    {
        debug("List() %s: File or directory not found.\n",  pbPath);
        return ERROR_QFTP_NOT_FOUND;
    }
    else if (ERROR_QFTP_DATA_CONNECT_CLOSED == iRet)  // Ŀ¼���ļ���ֱ�ӷ���ERROR_QFTP_NO_FILE_IN_DIR
    {
        return ERROR_QFTP_NO_FILE_IN_DIR;
    }

    //TODO ������յ�������
    nNum = Receive(hDataSocket, szDataBuffer, sizeof(szDataBuffer));
    // select�������ճ�����ʱ(�����)��ֱ�ӷ��أ�������
    if (nNum < 0)
    {
        return ERROR_QFTP_RECV_ERROR;
    }
    szDataBuffer[nNum] = '\0';
    debug("receive bufer: %s, num: %d\n", szDataBuffer, nNum);
    CloseSocket(hDataSocket);      // �ر���������
    m_iLastRecvAnswerCode = RecvAnswer(m_socketCtrlConnect, NULL, 0);

    return ERROR_QFTP_OK;
}

INT CQFTP::KeepAlive()
{
    INT iRet = 0;

    iRet = List("/");
    if (ERROR_QFTP_OK != iRet)
        return ERROR_QFTP_CONNECT_INVALID;
    return ERROR_QFTP_OK;
}

// ��ȡĿ¼���ļ����ƣ����浽������
INT CQFTP::GetFileName(SOCKET& s, deque<string>& qFileName)
{
    INT nBytesThisTime = 0;
    INT nSize = 1024;
    CHAR szDataBuffer[1024] = {0};
    PCHAR pbTemp = szDataBuffer;
    string strFile;

    nBytesThisTime = Receive(s, pbTemp, nSize); // first time

    if (ERROR_QFTP_RECV_ERROR == nBytesThisTime)
    {
        return ERROR_QFTP_RECV_ERROR;
    }
    strFile.append(pbTemp, nBytesThisTime); // �����յ���������ӵ�strFile�У�ע����Ҫָ���ֽ���
    // �Ƚ���һ����
    while (strFile.length())
    {
        size_t iCRLF = strFile.find('\n');
        if (iCRLF != string::npos)  // �ҵ�
        {
            qFileName.push_back(strFile.substr(0, iCRLF-1));
            strFile.erase(0, iCRLF+1);            // ɾ��xxx\r\n
        }
        // �������δ��\r\n�������ļ������������ٽ������˴���������
        else
        {
            break;
        }
    }
    while (nBytesThisTime != 0)
    {
        nBytesThisTime = Receive(s, pbTemp, nSize);

        if (ERROR_QFTP_RECV_ERROR == nBytesThisTime)
        {
            return ERROR_QFTP_RECV_ERROR;
        }

        strFile.append(pbTemp, nBytesThisTime); // append
        while (strFile.length())
        {
            size_t iCRLF = strFile.find('\n');
            if (iCRLF != string::npos)  // we got it
            {
                qFileName.push_back(strFile.substr(0, iCRLF-1));
                strFile.erase(0, iCRLF+1);            // ɾ��xxx\r\n
            }
            else
            {
                break;
            }
        }
    }   // end of while

    return ERROR_QFTP_OK;
}

// �г�ָ��Ŀ¼����������(Ŀ¼���ļ�����)�����浽������
INT CQFTP::ListToDeque(const CHAR*  pbPath, deque<string>& qFileName)
{
    SOCKET hDataSocket = 0;
    char szCommand[256] = {0};
    int ret;

    sprintf(szCommand, "NLST %s\r\n", pbPath);
    ret = OpenPassiveDataConnection(m_socketCtrlConnect, hDataSocket, szCommand);
    if (ERROR_QFTP_FILEORDIR_NOT_FOUND == ret)
    {
        debug("List() %s: File or directory not found.\n",  pbPath);
        return ERROR_QFTP_NOT_FOUND;
    }
    else if (ERROR_QFTP_DATA_CONNECT_CLOSED == ret)  // Ŀ¼���ļ���ֱ�ӷ���ERROR_QFTP_NO_FILE_IN_DIR
    {
        return ERROR_QFTP_NO_FILE_IN_DIR;
    }
    else if (ERROR_QFTP_OK != ret)
    {
    	return ERROR_QFTP_CONNECT_FAILED;
    }

    GetFileName(hDataSocket, qFileName);    // ���ļ��б��浽qFileName��
    CloseSocket(hDataSocket);               // �ر���������
    m_iLastRecvAnswerCode = RecvAnswer(m_socketCtrlConnect, NULL, 0);

    return ERROR_QFTP_OK;
}

// ɾ��һ��Ŀ¼����Ŀ¼�²��ܴ���Ŀ¼
INT CQFTP::DeleteOneDirectory(const CHAR* pbDirectory)
{
    CHAR szCommand[256] = {0};
    deque<string> qFileName;
    deque<string> sDirectory;

    sprintf(szCommand, "XCWD %s\r\n", pbDirectory);

    int ret = SendCommand(m_socketCtrlConnect, szCommand);
    if (550 == ret) // Ŀ¼������
    {
        debug("%s: No such directory.\n", pbDirectory);
        return ERROR_QFTP_NOT_FOUND;
    }

    ret = ListToDeque("", qFileName);       // �г���Ŀ¼�µ� _�ļ�_�ŵ�qFileName������
    if (ERROR_QFTP_NOT_FOUND == ret
        || ERROR_QFTP_NO_FILE_IN_DIR == ret
       )   // ���û�����ݣ���Ϊ��Ŀ¼����ɾ����Ŀ¼
    {
        return ERROR_QFTP_OK;
    }

    while (qFileName.size() > 0)
    {
        ret = DeleteOneFile((CHAR*)qFileName.front().c_str());
        qFileName.pop_front();              // ����������ȡ��һ��
    }

    return ERROR_QFTP_OK;
}
