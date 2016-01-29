#include<string.h>
#include "SWFC.h"
#include "swpa_socket.h"
#include "SWSocket.h"
#include "swpa.h"
#include "swpa_string.h"

CSWSocket::CSWSocket()
: m_nState( INVALID )
, m_objSockek(INVALID)
{
    bzero( &m_ConnectionAttrInfo , sizeof(SWPA_SOCKET_ATTR_T) );
	m_iDomain = SWPA_AF_INET;
}
CSWSocket::~CSWSocket()
{
    this->Close( );
}
/**
 * @brief �ر�socket
 *
 * @return
 * - S_OK : �ɹ�
 * - E_FAIL : ʧ��
 */
HRESULT CSWSocket::Close(){

   INT nError = 0 ;
   if( m_objSockek != INVALID)
        nError = swpa_socket_delete (m_objSockek);
    m_nState = INVALID; m_objSockek = INVALID;
    return nError;
}
/**
 * @brief �ж�socket�Ƿ���Ч
 * @return
 * - true : ��Ч
 * - false : ��Ч
 */
bool CSWSocket::IsValid(){

    if( m_objSockek != INVALID) return true;

	return false;
}
/**
 * @brief �ж�socket�Ƿ���Ч����
 * @return
 * - true : ��Ч
 * - false : ��Ч
 */
bool CSWSocket::IsConnect( )
{
    return (m_nState == CONNECTIONT ? true : false);
}
/**
 * @brief TCP���Ӻ���
 *
 * @param [in] szIP : Ҫ���ӵ���IP��ַ
 * @param [in] wPort : Ҫ���ӵ��Ķ˿ں�
 * @return
 * - S_OK : �ɹ�
 * - E_FAIL : ʧ��
 */
HRESULT CSWSocket::Connect(const char * szIP, const WORD wPort){

    if( NULL == szIP || wPort <= 0 ) return E_FAIL;
    if( m_objSockek == INVALID) return E_FAIL;

    bzero(&m_objRemoteAddr,sizeof(SWPA_SOCKADDR));

	swpa_memset(m_objRemoteAddr.ip, 0, sizeof(m_objRemoteAddr.ip));
    if( szIP != NULL )
        swpa_strncpy(m_objRemoteAddr.ip , szIP , sizeof(m_objRemoteAddr.ip)-1);

    m_objRemoteAddr.port = wPort;


	if( swpa_socket_connect(m_objSockek,
                         &m_objRemoteAddr, sizeof(SWPA_SOCKADDR)) == SWPAR_FAIL) return E_FAIL;

    m_nState = CONNECTIONT ;

    // Ĭ������10���ͳ�ʱ
    this->SetSendTimeout( 1000*10 );

    return S_OK;

}

/**
 * @brief ������ʽ�׽������Ӻ���
 *
 * @param [in] szPath : �����ļ�
 * @return
 * - S_OK : �ɹ�
 * - E_FAIL : ʧ��
 */
HRESULT CSWSocket::Connect(const char * szPath)
{
    if( NULL == szPath ) return E_FAIL;
    if( INVALID == m_objSockek ) return E_FAIL;

    swpa_memset(&m_objLocalAddr,0,sizeof(SWPA_SOCKADDR_UN));

	//swpa_memset(m_objLocalAddr.sun_path, '\0', sizeof(m_objLocalAddr.sun_path));
    if( szPath != NULL )
        swpa_strncpy(m_objLocalAddr.sun_path, szPath , sizeof(m_objLocalAddr.sun_path)-1);

	if( swpa_socket_connect_un(m_objSockek,
                         &m_objLocalAddr, sizeof(SWPA_SOCKADDR_UN)) == SWPAR_FAIL) return E_FAIL;

    m_nState = CONNECTIONT ;

    // Ĭ������10���ͳ�ʱ
    this->SetSendTimeout( 1000*10 );

    return S_OK;	
}
/**
 * @brief ��IP���˿�
 *
 * @param [in] szIP : Ҫ���ӵ���IP��ַ
 * @param [in] wPort : Ҫ���ӵ��Ķ˿ں�
 * @return
 * - S_OK : �ɹ�
 * - E_FAIL : ʧ��
 */
HRESULT CSWSocket::Bind(const char * szIP, const WORD wPort){

    if( m_objSockek == INVALID) return E_FAIL;

    bzero(&m_objAddr,sizeof(SWPA_SOCKADDR));
    if( szIP != NULL )
        swpa_memcpy( m_objAddr.ip , szIP , swpa_strlen(szIP));

    m_objAddr.port = wPort;
    if( SWPAR_FAIL == swpa_socket_bind(m_objSockek, &m_objAddr ,sizeof( SWPA_SOCKADDR )))
        return E_FAIL;

    return S_OK;
}

/**
 * @brief �󶨱����׽��ֵ�ַ
 *
 * @param [in] szPath : �����ļ�
 * @return
 * - S_OK : �ɹ�
 * - E_FAIL : ʧ��
 */
HRESULT CSWSocket::Bind(const char * szPath)
{
	if (NULL == szPath) return E_FAIL;
    if( INVALID == m_objSockek ) return E_FAIL;

	swpa_memset(m_objLocalAddr.sun_path, '\0', sizeof(m_objLocalAddr.sun_path));
    if( szPath != NULL )
        swpa_strncpy(m_objLocalAddr.sun_path, szPath , sizeof(m_objLocalAddr.sun_path)-1);

    if( SWPAR_FAIL == swpa_socket_bind_un(m_objSockek, &m_objLocalAddr ,sizeof( SWPA_SOCKADDR_UN )))
        return E_FAIL;

    return S_OK;
}

/**
 * @brief �������� socket
 *
 * @param [in] nSocketType : ���� socket����(UDP\TCP)
 * @return
 * - S_OK : �ɹ�
 * - E_FAIL : ʧ��
 */
HRESULT CSWSocket::Create(INT nSocketType){

    if( m_objSockek != INVALID) this->Close( );

    SWPA_SOCKET_ATTR_T stSocketAttr;
    bzero(&stSocketAttr,sizeof(SWPA_SOCKET_ATTR_T));
    stSocketAttr.af = SWPA_AF_INET;
    stSocketAttr.type = nSocketType;
	m_iDomain = SWPA_AF_INET;

    if( SWPAR_FAIL == swpa_socket_create( &m_objSockek , &stSocketAttr ))
        return E_FAIL;

    return S_OK;
}

/**
 * @brief ���� socket
 *
 * @param [in] nSocketType : ���� socket����(UDP\TCP)
 * @param [in] nDomain : �׽�������(����\����)
 * @return
 * - S_OK : �ɹ�
 * - E_FAIL : ʧ��
 */
HRESULT CSWSocket::Create( INT nSocketType, INT nDomain)
{
    if( m_objSockek != INVALID) this->Close( );

    SWPA_SOCKET_ATTR_T stSocketAttr;
    bzero(&stSocketAttr,sizeof(SWPA_SOCKET_ATTR_T));
    stSocketAttr.af = nDomain;
    stSocketAttr.type = nSocketType;
	m_iDomain = nDomain;
	
    if( SWPAR_FAIL == swpa_socket_create( &m_objSockek , &stSocketAttr ))
        return E_FAIL;

    return S_OK;
}


/**
* @brief ��ȡ�׽��ֵı��������ַ
* @param [out] szName �����������ַ
* - S_OK : �ɹ�
* - E_FAIL : ʧ��
*/
HRESULT CSWSocket::Getsockname( char* szName )
{
    if( m_objSockek == INVALID
       || NULL == szName ) return E_FAIL;

    struct SWPA_SOCKADDR addr;

    DWORD addrlen = sizeof(addr);
    bzero(&addr,sizeof(addr));

    if( swpa_socket_getsockname( m_objSockek , &addr , &addrlen) == SWPAR_OK)
        swpa_strcpy( szName , addr.ip);
    else return E_FAIL;

    return S_OK;
}
/**
 * @brief ��ȡsocket�ĶԷ���ַ
 *
 * @param [in] szIP : Ҫ���ӵ���IP��ַ
 * @param [in] szName : �Է���IP��ַ
 * @return
 * - S_OK : �ɹ�
 * - E_FAIL : ʧ��
 */
HRESULT CSWSocket::GetPeerName(char* szName , WORD *wPort  ){

    if( m_objSockek == INVALID
       || NULL == szName ) return E_FAIL;
    struct SWPA_SOCKADDR addr;
    DWORD dwAddrlen = sizeof( SWPA_SOCKADDR );
    if( S_OK == swpa_socket_getpeername( m_objSockek , &addr , & dwAddrlen ))
    {
        swpa_memcpy( szName , addr.ip , sizeof( addr.ip ));
        if( wPort != NULL )
            *wPort = addr.port;
    }

    return S_OK;
}
/**
 * @brief ��ȡ���ճ�ʱʱ��
 *
 * @param [out] pdwMS : ��ȡ���ĳ�ʱʱ��
 * @return
 * - S_OK : �ɹ�
 * - E_FAIL : ʧ��
 */
HRESULT CSWSocket::GetRecvTimeout(DWORD* pdwMS){

    if( m_objSockek == INVALID) return E_FAIL;

    struct SWPA_TIMEVAL timeout;
    INT leng = sizeof(struct SWPA_TIMEVAL );

	if(swpa_socket_getopt( m_objSockek , SWPA_SOL_SOCKET
                       , SWPA_SO_RCVTIMEO , (VOID*)&timeout , &leng) == SWPAR_OK)
	{
	    *pdwMS = timeout.tv_usec ; return S_OK;
	}

	return E_FAIL;
}

HRESULT CSWSocket::SetSendBufferSize(const DWORD dwSize)
{
	if( m_objSockek == INVALID) return E_FAIL;

	INT leng = sizeof(dwSize);


	if(swpa_socket_opt( m_objSockek , SWPA_SOL_SOCKET
		, SWPA_SO_SNDBUF , (VOID*)&dwSize , leng) == SWPAR_OK)
	{
		return S_OK;
	}
	
	return E_FAIL;
}


HRESULT CSWSocket::SetRecvBufferSize(const DWORD dwSize)
{
	if( m_objSockek == INVALID) return E_FAIL;

	INT leng = sizeof(dwSize);


	if(swpa_socket_opt( m_objSockek , SWPA_SOL_SOCKET
		, SWPA_SO_RCVBUF , (VOID*)&dwSize , leng) == SWPAR_OK)
	{
		return S_OK;
	}

	return E_FAIL;
}

/**
 * @brief ��ȡ���ͳ�ʱʱ��
 *
 * @param [out] pdwMS : ��ȡ���ĳ�ʱʱ��
 * @return
 * - S_OK : �ɹ�
 * - E_FAIL : ʧ��
 */
HRESULT CSWSocket::GetSendTimeout(DWORD* pdwMS){

    if( m_objSockek == INVALID) return E_FAIL;


   struct SWPA_TIMEVAL timeout;
   INT leng = sizeof(struct SWPA_TIMEVAL );

	if(swpa_socket_getopt( m_objSockek , SWPA_SOL_SOCKET
                        , SWPA_SO_SNDTIMEO , (VOID*)&timeout , &leng) == SWPAR_OK)
	{
	    *pdwMS = timeout.tv_usec ; return S_OK;

	}

	return E_FAIL;
}

/**
 * @brief ���ý��ճ�ʱʱ��
 *
 * @param [out] dwMS :�����õĳ�ʱʱ��
 * @return
 * - S_OK : �ɹ�
 * - E_FAIL : ʧ��
 */
HRESULT CSWSocket::SetRecvTimeout(const DWORD dwMS){

    if( m_objSockek == INVALID) return E_FAIL;

    struct SWPA_TIMEVAL timeout;
    timeout.tv_sec = dwMS / 1000;
    timeout.tv_usec = (dwMS % 1000) * 1000;

	if( SWPAR_FAIL == swpa_socket_opt( m_objSockek , SWPA_SOL_SOCKET
                     , SWPA_SO_RCVTIMEO , (VOID*)&timeout , sizeof(struct SWPA_TIMEVAL)))
        return E_FAIL;

    return S_OK;
}
/**
 * @brief ���÷��ͳ�ʱʱ��
 *
 * @param [out] dwMS :�����õĳ�ʱʱ��
 * @return
 * - S_OK : �ɹ�
 * - E_FAIL : ʧ��
 */
HRESULT CSWSocket::SetSendTimeout(const DWORD dwMS){

    if( m_objSockek == INVALID) return E_FAIL;

    struct SWPA_TIMEVAL timeout;
    timeout.tv_sec = dwMS / 1000;
    timeout.tv_usec = (dwMS % 1000) * 1000;

	if( SWPAR_FAIL == swpa_socket_opt( m_objSockek , SWPA_SOL_SOCKET , SWPA_SO_SNDTIMEO
                                   , (VOID*)&timeout , sizeof(struct SWPA_TIMEVAL)))
	{
	    return E_FAIL;
	}


    return S_OK;
}
/**
 * @brief SOCKET����
 *
 * @param [in] hSock : ��Ҫ�������׽���
 * @return
 * - S_OK : �ɹ�
 * - E_FAIL : ʧ��
 */
HRESULT CSWSocket::Attach( SWPA_SOCKET_T hSock )
{
    if( hSock == INVALID) return E_FAIL;

    m_objSockek = hSock;
    m_nState = CONNECTIONT; return S_OK;
}
/**
 * @brief �����׽���
 *
 * @return
 * - S_OK : �ɹ�
 * - E_FAIL : ʧ��
 */
HRESULT CSWSocket::Detach(  )
{
    m_objSockek = INVALID; m_nState ^= CONNECTIONT; return S_OK;
}

HRESULT CSWSocket::SetSockOpt(INT iLevel,INT iOptname ,PVOID pOptval, INT iOptlen)
{
	if( m_objSockek == INVALID) return E_FAIL;
	return SWPAR_OK == swpa_socket_opt(m_objSockek, iLevel, iOptname, pOptval, iOptlen) ? S_OK : E_FAIL;
}


