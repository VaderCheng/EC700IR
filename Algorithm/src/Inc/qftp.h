#ifndef _QFTP_H_
#define _QFTP_H_

#include <cstdlib>
#include <cstring>
#include <cerrno>
#include <string>
#include <deque>

//#define __DEBUG_          // ��ӡ������Ϣ��
//#define PRINT_MESSAGE   // ��ӡ������������Ϣ

#ifdef WIN32    // windows

    #include <winsock2.h>   // ����ŵ�windows.hǰ
    #pragma comment (lib,"ws2_32.lib")
    #include <windows.h>

    #ifndef WORD16
    typedef unsigned short WORD16;
    #endif
    #ifndef DWORD32
    typedef unsigned int DWORD32;
    #endif

    #define MSG_NOSIGNAL 0  // for send

    #ifdef __DEBUG_
    #define debug printf
    #else
    #define debug
    #endif

#else   // linux

    #include <unistd.h>
    #include <sys/stat.h>
    #include <sys/types.h>
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>

    typedef char CHAR;
    typedef int INT;
    typedef unsigned int UINT;
    typedef int BOOL;
    typedef unsigned short WORD16;
    typedef unsigned int DWORD32;
    typedef struct sockaddr SOCKADDR;
    typedef struct sockaddr_in SOCKADDR_IN;
    typedef int SOCKET;

    #ifndef INVALID_SOCKET
    #define INVALID_SOCKET (-1)
    #endif

    #ifndef SOCKET_ERROR
    #define SOCKET_ERROR (-1)
    #endif

    #ifdef __DEBUG_
    #define debug(fmt...) printf(fmt)
    #else
    #define debug(fmt...)
    #endif

#endif  /* WIN32 */

#ifndef FALSE
#define FALSE   0
#endif
#ifndef TRUE
#define TRUE    1
#endif
#ifdef __cplusplus
#ifndef NULL
#define NULL    0
#endif
#endif

//�Զ��������
// ͨ����ӡ���ӿڷ���ֵ���õ�����ԭ��
#define ERROR_QFTP_BASE -1000
#define ERROR_QFTP_OK 0

#define ERROR_QFTP_UNKNOWNERROR                     ERROR_QFTP_BASE+0
#define ERROR_QFTP_CONNECT_INVALID                  ERROR_QFTP_BASE+1
#define ERROR_QFTP_LOGOUT_FAILED                    ERROR_QFTP_BASE+2
#define ERROR_QFTP_PARAMETER_INVALID                ERROR_QFTP_BASE+3
#define ERROR_QFTP_SOCKET_INITIALIZE_FAILED         ERROR_QFTP_BASE+4
#define ERROR_QFTP_CONNECT_FAILED                   ERROR_QFTP_BASE+5
#define ERROR_QFTP_CMD_ERROR                        ERROR_QFTP_BASE+6
#define ERROR_QFTP_FILEORDIR_NOT_FOUND              ERROR_QFTP_BASE+7
#define ERROR_QFTP_DATA_CONNECT_CLOSED              ERROR_QFTP_BASE+8

// select��������
#define ERROR_QFTP_SELECT_ERROR                 ERROR_QFTP_BASE+9
// ���ճ���
#define ERROR_QFTP_RECV_ERROR                   ERROR_QFTP_BASE+10
// ���ͳ���
#define ERROR_QFTP_SEND_ERROR                   ERROR_QFTP_BASE+11
// ���ճ�ʱ
#define ERROR_QFTP_RECV_TIMEOUT                 ERROR_QFTP_BASE+12
//���ͳ�ʱ
#define ERROR_QFTP_SEND_TIMEOUT                 ERROR_QFTP_BASE+13
// �û����������
#define ERROR_QFTP_USER_ERROR                   ERROR_QFTP_BASE+14
// �ļ���Ŀ¼δ�ҵ�
#define ERROR_QFTP_NOT_FOUND                    ERROR_QFTP_BASE+15
// ɾ���ļ�����
#define ERROR_QFTP_DEL_ERROR                    ERROR_QFTP_BASE+16
//Ŀ¼���ļ�
#define ERROR_QFTP_NO_FILE_IN_DIR               ERROR_QFTP_BASE+17
#define MAX_CMD_LEN         256

using namespace std;

namespace nsFTP
{

class  CQFTP
{
public:
    // Ĭ��Ϊ������½����ʱ��λMS 0��ʾ����ģʽ
    CQFTP(const string& strHostName, const string& strUserName = "anonymous", const string& strPassWord = "yetpasswd", UINT nTimeout = 1000);
    ~CQFTP();
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
    INT LoginFtpServer(void);

    //˵�����ǳ�
    //�ɹ����أ�ERROR_QFTP_OK
    INT LogoutFtpServer(void);

    //˵����������ӵ���Ч��
    //������Ч���أ�ERROR_QFTP_OK
    INT TestConnectValidity(void);

    ///////////////////// �µĽӿ�
    // ��ȡFTP�������ϵ��ļ����ڴ���
    // szFileName: FTP��������������·�����ļ���(�硰/array1/guest-share/user1/foo.dat��)
    // szBuffer: ������ָ��
    // dwLength����������С
    // [OUT]dwDataLen��ʵ�ʶ�ȡ�������ݴ�С
    // ˵�����������쳣ʱ������ERROR_QFTP_RECV_ERROR�������������0��
    // ���ָ���ļ������ڣ�dwDataLenֵΪ0�����������С��ʵ���ļ����ȣ�dwDataLenΪ��������С
    INT ReadFileFromFtp(CHAR* szFileName, CHAR* szBuffer, DWORD32 dwLength, DWORD32& dwDataLen);

    // �������ݵ�FTP��
    // szFileName: FTP��������������·�����ļ���(�硰/array1/guest-share/user1/foo.dat��)
    // szBuffer: ������ָ��
    // dwLength����������С
    // ��������0��ʧ�ܷ���ERROR_QFTP_SEND_ERROR
    INT WriteFileToFtp(CHAR* szFileName, CHAR* szBuffer, DWORD32 dwLength);

    // ɾ�������ļ�
    // szFileName: FTP��������������·�����ļ���(�硰/array1/guest-share/user1/foo.dat��)
    // �ɹ�����0��ʧ�ܷ���ERROR_QFTP_DEL_ERROR
    INT DeleteOneFile(CHAR* szFileName);

    // ��FTPһֱ��������״̬
    // �ɹ�����0��ʧ�ܷ���������ЧERROR_QFTP_CONNECT_INVALID
    // �ú����ǲ������ݴ��䣬��FTP��������״̬�����������쳣ʱ������ʧ��
    INT KeepAlive(void);

    // ɾ��һ��Ŀ¼����Ŀ¼�²��ܴ���Ŀ¼
	INT DeleteOneDirectory(const CHAR* pbDirectory);
    // ���º���Ϊ���ڲ�ʹ��
private:
    INT ConnectSocket(SOCKET& hConnectSocket, const CHAR* szIP, INT iPort);
    INT Send(SOCKET& socket, const CHAR* pszBuffer, INT nSize);
    INT Receive(SOCKET& socket, CHAR* pszBuffer, INT nSize);
    INT CloseSocket(SOCKET& hConnectSocket);

    INT RecvAnswer(SOCKET& hConnectSocket, CHAR* pszRecvBuf, INT iLen);
    INT SendCommand(SOCKET& hConnectSocket, const CHAR* pbCommand);

    INT ChangeDirectory(const CHAR* DirectoryPath);
    INT MakeDirectory(const CHAR* DirectoryPath);

    INT OpenPassiveDataConnection(SOCKET& hConnectSocket, SOCKET& hDataSocket, CHAR* pbCommand);
    INT GetPortForPASV(CHAR* szRecvBuf);
    INT SendData(SOCKET& hConnectSocket, CHAR* pbDataBuffer, INT nSize);
    INT SaveBufferToFtp(SOCKET& hConnectSocket, const CHAR* pbFileName, CHAR* pbBuffer, INT nSize, BOOL fAppend);
    INT SaveDataToFtp(string strFileFullName, CHAR* pbDataForSend, UINT uDataLen, BOOL fAppend=FALSE);

    INT GetFileFromFtp(const CHAR* pbRemoteFile, CHAR* szBuffer, DWORD32 dwBufLen);

    INT List(const CHAR* pbPath);

    INT GetFileName(SOCKET& s, deque<string>& qFileName);

	INT ListToDeque(const CHAR*  pbPath, deque<string>& qFileName);
private:

    SOCKET m_socketCtrlConnect;     // ����(����)ͨ��socket
    INT    m_iLastRecvAnswerCode;   // ���������ش��ţ���226 550 200��
    UINT   m_wTimeout;              // ��ʱʱ��(��λΪ��)
    CHAR   m_szRecvBuf[1024];       //���ջ�����(��������ص���Ϣ)
    string m_strFtpIP;              //���ڴ洢IP��ַ
    string m_strUserName;           //���ڴ洢�û���
    string m_strPassWord;           //���ڴ洢����
};  // class CQFTP

}   // namespace nsFTP

#endif /* _QFTP_H_ */
