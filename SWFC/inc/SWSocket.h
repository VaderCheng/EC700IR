#ifndef CSWSOCKET_H
#define CSWSOCKET_H


#include "SWObject.h"
#include "swpa_socket.h"

class CSWSocket : public CSWObject
{
public:
    enum{
     INVALID = -1,        // ��Ч״̬
     SOCKCREATE,          // �����׽���
     CONNECTIONT,         // ����״̬
    };
public:


    CSWSocket();
    virtual ~CSWSocket();

	HRESULT Close();
	bool IsValid();
    bool IsConnect( );

	HRESULT Connect(const char * szIP, const WORD wPort);
	HRESULT Connect(const char * szPath);
    HRESULT Bind(const char * szIP, const WORD wPort);
	HRESULT Bind(const char * szPath);

    HRESULT Getsockname( char* szName );

	HRESULT GetPeerName(char* szName , WORD *wPort = NULL );
	HRESULT GetRecvTimeout(DWORD* pdwMS);
	HRESULT GetSendTimeout(DWORD* pdwMS);

	HRESULT SetRecvTimeout(const DWORD dwMS);
	HRESULT SetSendTimeout(const DWORD dwMS);

	HRESULT SetSendBufferSize(const DWORD dwSize);
	HRESULT SetRecvBufferSize(const DWORD dwSize);
  // ����Socket
  HRESULT Attach( SWPA_SOCKET_T hSock );
  // �����׽���
  HRESULT Detach( );
  HRESULT SetSockOpt(INT iLevel,INT iOptname ,PVOID pOptval, INT iOptlen);
protected:
	HRESULT Create( INT nSocketType = SWPA_SOCK_STREAM );
	HRESULT Create( INT nSocketType, INT nDomain);
protected:
	// �׽��ֽṹ��
	SWPA_SOCKET_T m_objSockek;
	// ������Ϣ
	SWPA_SOCKET_ATTR_T m_ConnectionAttrInfo;

	// �������ӵ�ַ
	SWPA_SOCKADDR m_objAddr;
	// Զ�̵�ַ
	SWPA_SOCKADDR m_objRemoteAddr;

	//�����׽��ֵ�ַ
	SWPA_SOCKADDR_UN m_objLocalAddr;

	//�׽�������
	INT m_iDomain;

    // ����״̬
    INT m_nState;
};

#endif // CSWSOCKET_H

