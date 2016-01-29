#ifndef CSWTCPSOCKET_H
#define CSWTCPSOCKET_H

#include "SWSocket.h"

//todo...
#define ONACCEPT VOID*

class CSWTCPSocket : public CSWSocket
{
public:
    CSWTCPSocket();
    virtual ~CSWTCPSocket();

    // �����׽���
	HRESULT Create( BOOL fLocalSock = FALSE);


    // ���ݽ���
	HRESULT Read(VOID *pbBuf, DWORD dwSize , DWORD *rcvDataLen = NULL);
	// ���ݴ���
	HRESULT Send(const VOID *pbBuf, DWORD dwSize , DWORD *dwOutLengthSent = NULL);
    // ����
    HRESULT Listen( INT backlog = 128 );
    // ���������׽���
    HRESULT Accept( SWPA_SOCKET_T &outSock );

protected:
private:
	BOOL m_fLocalSock;
};

#endif // CSWTCPSOCKET_H

