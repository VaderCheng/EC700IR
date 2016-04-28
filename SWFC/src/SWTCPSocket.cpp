#include<stdio.h>
#include "SWTCPSocket.h"
#include "swpa.h"

CSWTCPSocket::CSWTCPSocket()
{
    //ctor
    m_fLocalSock = FALSE;
}

CSWTCPSocket::~CSWTCPSocket()
{
    //dtor
}
/**
 * @brief ����TCP socket
 *
 * @param [in] wPort : �󶨵Ķ˿ں�,Ĭ��Ϊ0
 * @return
 * - S_OK : �ɹ�
 * - E_FAIL : ʧ��
 */
HRESULT CSWTCPSocket::Create(  BOOL fLocalSock /*= FALSE*/ ){

	if (TRUE == fLocalSock)
	{
		m_fLocalSock = TRUE;
		return CSWSocket::Create( SWPA_SOCK_STREAM , SWPA_AF_UNIX);
	}
	else
	{
    	return CSWSocket::Create( SWPA_SOCK_STREAM );
	}
}
/**
* @brief �ȴ��ͻ�������
*
* @param [in] skt �׽��ֽṹ��
* @param [in] backlog �׽��ּ����������������󳤶�
* @retval 0 �ɹ�
* @retval -1 ʧ��
* @see swpa_socket.h
*/
HRESULT CSWTCPSocket::Listen( INT backlog )
{
    if( m_objSockek == INVALID) return E_FAIL;

    return swpa_socket_listen(m_objSockek, backlog);
}
HRESULT CSWTCPSocket::Accept( SWPA_SOCKET_T &outSock )
{
    if( m_objSockek == INVALID) return E_FAIL;

    DWORD leng = sizeof(m_objAddr);

    if( SWPAR_FAIL == swpa_socket_accept(m_objSockek, &m_objAddr
                              , &leng , &outSock))
    {
      return E_FAIL;
    }

    return S_OK;
}

/**
 * @brief ���ݽ���
 *
 * @param [PVOID] pbBuf : ��Ҫ�洢�����ݻ���
 * @param [DWORD] dwSize : �����С
 * @param [PDWORD] rcvDataLen : ʵ�ʽ��յ����ݴ�С
 * @return
 * - S_OK : �ɹ�
 * - E_FAIL : ʧ��
 */
HRESULT CSWTCPSocket::Read(VOID *pbBuf, DWORD dwSize , DWORD *rcvDataLen ){

    if( NULL == pbBuf || 0 == dwSize
       || !this->IsConnect( ) ) return E_FAIL;

    DWORD dwCurrentPos = 0 ;DWORD dwReadLen = dwSize;
    while( dwCurrentPos < dwSize )
    {
        if( SWPAR_FAIL == swpa_socket_recv( m_objSockek , (CHAR*)pbBuf + dwCurrentPos, &dwReadLen ))
        {
            if( swpa_socket_get_lastererrocode( ) != SWPA_EAGAIN && this->IsConnect( ))
                m_nState ^= CONNECTIONT;

            if( rcvDataLen != NULL ) *rcvDataLen = dwCurrentPos;
            return E_FAIL;
        }

        dwCurrentPos += dwReadLen; dwReadLen = dwSize - dwCurrentPos;
    }

    if( rcvDataLen != NULL ) *rcvDataLen = dwCurrentPos;

    return S_OK;
}
/**
 * @brief ���ݴ���
 *
 * @param [LPCVOID] pbBuf : ��Ҫ��������ݻ���
 * @param [DWORD] dwSize : ��Ҫ��������ݴ�С
 * @param [PDWORD] dwOutLengthSent : ʵ�ʴ�������ݴ�С
 * @return
 * - S_OK : �ɹ�
 * - E_FAIL : ʧ��
 */
HRESULT CSWTCPSocket::Send(const VOID* pbBuf, DWORD dwSize , DWORD *dwOutLengthSent){

    if( NULL == pbBuf || 0 == dwSize
       || !this->IsConnect( ) ) return E_FAIL;

    int nSendSize = 0; DWORD dwOutLength = 0;
    while( nSendSize < dwSize )
    {
       if( SWPAR_FAIL == swpa_socket_send(m_objSockek, (CHAR*)pbBuf + nSendSize
                                          , dwSize - nSendSize, &dwOutLength )
          || dwOutLength == 0)
        {
            if( /*swpa_socket_get_lastererrocode( ) != SWPA_EAGAIN &&*/ this->IsConnect( ))
                m_nState ^= CONNECTIONT;

            return E_FAIL;
        }

		   nSendSize += dwOutLength; dwOutLength = dwSize - nSendSize;
    }

    if( dwOutLengthSent != NULL ) *dwOutLengthSent = nSendSize;

    //printf( "Send data success len %d\n",dwSize);

    return S_OK;
}

