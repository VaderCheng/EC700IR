///////////////////////////////////////////////////////////
//  CSWSocketStream.cpp
//  Implementation of the Class CSWTcpStream
//  Created on:      28-����-2013 14:09:47
//  Original author: zy
///////////////////////////////////////////////////////////

#include "SWTCPSocket.h"
#include "SWUDPSocket.h"
#include "SWSocketStream.h"

/**
 * @brief ���캯��
 */
CSWTcpStream::CSWTcpStream( CSWTCPSocket *pTCPSocket )
:m_pTCPSocket( pTCPSocket )
{
    if( m_pTCPSocket != NULL ) m_pTCPSocket->AddRef( );
}

/**
 * @brief ��������
 */
CSWTcpStream::~CSWTcpStream(){
    if( m_pTCPSocket != NULL ) m_pTCPSocket->Release( );
}

/**
 * @brief ��д�뺯��
 *
 * @param [in] pvBuf : ���Ҫд�����ݵĵĻ�����
 * @param [in] dwCB : д�����ݵĵĴ�С
 * @param [out] pdwWritten : ʵ��д����ֽ���
 * @return
 * - S_OK : �ɹ�
 * _ E_FAIL : ʧ��
 */
HRESULT CSWTcpStream::Write(PVOID pvBuf, DWORD dwCB, PDWORD pdwWritten){

    if( m_pTCPSocket == NULL || !m_pTCPSocket->IsValid()) return E_FAIL;

    return m_pTCPSocket->Send( pvBuf , dwCB , pdwWritten);
}

/**
 * @brief ����ȡ����
 *
 * @param [in] pvBuf : ��Ŷ�ȡ����Ļ�����
 * @param [in] dwCB : ��Ŷ�ȡ����Ļ������Ĵ�С
 * @param [out] pdwRead : ʵ�ʶ�ȡ�����ֽ���
 * @return
 * - S_OK : �ɹ�
 * _ E_FAIL : ʧ��
 */
HRESULT CSWTcpStream::Read(PVOID pvBuf, DWORD dwCB, PDWORD pdwRead){

    if( m_pTCPSocket == NULL || !m_pTCPSocket->IsValid()) return E_FAIL;

    return m_pTCPSocket->Read( pvBuf , dwCB , pdwRead);
}


/**
 * @brief ��ȡ��������ý�����ĺ���
 *
 * @param [out] pMediumObj : ý�����ָ��
 * @return
 * - S_OK : �ɹ�
 * _ E_FAIL : ʧ��
 */
HRESULT CSWTcpStream::GetMedium(CSWObject ** pMediumObj)
{
	if (NULL == pMediumObj)
	{
		return E_FAIL;
	}

	*pMediumObj = m_pTCPSocket;

	return S_OK;
}



/**
 * @brief ���캯��
 */
CSWUdpStream::CSWUdpStream( CSWUDPSocket *pUPDSocket )
:m_pUDPSocket( pUPDSocket )
{
    if( m_pUDPSocket != NULL ) m_pUDPSocket->AddRef( );
}
/**
 * @brief ��������
 */
CSWUdpStream::~CSWUdpStream(){
    if( m_pUDPSocket != NULL ) m_pUDPSocket->Release( );
}

/**
 * @brief ��д�뺯��
 *
 * @param [in] pvBuf : ���Ҫд�����ݵĵĻ�����
 * @param [in] dwCB : д�����ݵĵĴ�С
 * @param [out] pdwWritten : ʵ��д����ֽ���
 * @return
 * - S_OK : �ɹ�
 * _ E_FAIL : ʧ��
 */
HRESULT CSWUdpStream::Write(PVOID pvBuf, DWORD dwCB, PDWORD pdwWritten){

    if( m_pUDPSocket == NULL || !m_pUDPSocket->IsValid()) return E_FAIL;

	return  m_pUDPSocket->SendTo( pvBuf , dwCB , pdwWritten);
}
/**
 * @brief ����ȡ����
 *
 * @param [in] pvBuf : ��Ŷ�ȡ����Ļ�����
 * @param [in] dwCB : ��Ŷ�ȡ����Ļ������Ĵ�С
 * @param [out] pdwRead : ʵ�ʶ�ȡ�����ֽ���
 * @return
 * - S_OK : �ɹ�
 * _ E_FAIL : ʧ��
 */
HRESULT CSWUdpStream::Read(PVOID pvBuf, DWORD dwCB, PDWORD pdwRead){
   if( m_pUDPSocket == NULL || !m_pUDPSocket->IsValid()) return E_FAIL;

	return  m_pUDPSocket->RecvFrom( pvBuf, dwCB, pdwRead );
}


/**
 * @brief ��ȡ��������ý�����ĺ���
 *
 * @param [out] pMediumObj : ý�����ָ��
 * @return
 * - S_OK : �ɹ�
 * _ E_FAIL : ʧ��
 */
HRESULT CSWUdpStream::GetMedium(CSWObject ** pMediumObj)
{
	if (NULL == pMediumObj)
	{
		return E_FAIL;
	}

	*pMediumObj = m_pUDPSocket;

	return S_OK;
}


