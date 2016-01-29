#ifndef CSWUDPSOCKET_H
#define CSWUDPSOCKET_H

#include "SWSocket.h"


class CSWUDPSocket : public CSWSocket
{
public:

    CSWUDPSocket(const CHAR* outRemoteAddr , const WORD wRemotePort
                 , const CHAR* stLocalAddr , const WORD wLocalPort);
    CSWUDPSocket( );
    virtual ~CSWUDPSocket();

    // �����׽���
    HRESULT Create( );

    // UDP���ݽ���
    HRESULT RecvFrom(const CHAR* outSocketAddr,const  WORD outSocketPort,
                        VOID* ioBuffer, const DWORD inBufLen, DWORD* outRecvLen);

    HRESULT RecvFrom( VOID* ioBuffer, const DWORD inBufLen, DWORD* outRecvLen);

    // UDP���ݴ���
    HRESULT SendTo(const VOID* pbBuf, const DWORD dwSize, const CHAR* inRemoteAddr
                   ,const DWORD inRemotePort, DWORD *dwSendlen);

    HRESULT SendTo(const VOID* pbBuf, const DWORD dwSize , DWORD *dwSendlen);

     // �㲥����
    HRESULT Broadcast(const WORD wDstPort, const BYTE *pbBuf, const DWORD dwSize);

	 
	 HRESULT GetPeerName(char* szName , WORD *wPort );

protected:


};

#endif // CSWUDPSOCKET_H

