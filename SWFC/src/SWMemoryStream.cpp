///////////////////////////////////////////////////////////
//  CSWMemoryStream.cpp
//  Implementation of the Class CSWMemoryStream
//  Created on:      28-����-2013 14:09:51
//  Original author: zhouy
///////////////////////////////////////////////////////////

#include "SWMemory.h"

#include "SWMemoryStream.h"

/**
 * @brief ���캯��
 *
 * @param [in] dwType : �ڴ����࣬Ŀǰȡֵ��Χ�С�ϵͳ���ڴ棬DSP�����ڴ棬Cache���ٻ��棬�ļ�ӳ���ڴ桿
 * @param [in] dwSize : Ҫ������ڴ�Ĵ�С����λΪ�ֽ�
 */
CSWMemoryStream::CSWMemoryStream(DWORD dwType, DWORD dwSize)
: m_pMemoryManagement( NULL )
, m_pBuffer( NULL )
{
	/*
    switch( dwType )
    {
    case 0:
        m_pMemoryManagement = new CSWCacheMemory( ); break;
    case 1:
        m_pMemoryManagement = new CSWMappedMemory( ); break;
    case 2:
        m_pMemoryManagement = new CSWSharedMemory( ); break;
    default:
        break;
    }

    // ��ʼ���ڴ��
    InitialMemory( dwSize );
	*/
}


/**
 * @brief ��������
 */
CSWMemoryStream::~CSWMemoryStream(){
	/*
    // �ͷ��ڴ���Դ
    if( m_pMemoryManagement != NULL ){

        if( m_pBuffer != NULL )
            m_pMemoryManagement->Free( m_pBuffer );
        m_pBuffer = NULL;

         delete m_pMemoryManagement;
    }

    m_pMemoryManagement = NULL;
	*/
}

 /**
 * @brief ��ʼ���ڴ��
 *
 * @param [DWORD] dwSize : �����ڴ��С
 * @return
 * - S_OK : �ɹ�
 * _ E_FAIL : ʧ��
 */
HRESULT CSWMemoryStream::InitialMemory( DWORD dwSize )
{
	/*
    if( m_pMemoryManagement == NULL || dwSize <= 0) return E_FAIL;

    m_pBuffer = m_pMemoryManagement->Alloc( dwSize );

    return ( m_pBuffer != NULL ? S_OK : E_FAIL);
	*/
	return E_NOTIMPL;
}

/**
 * @brief �ύ�����������
 *
 * @param [in] dwCommitFlags : �ύ��־
 * @return
 * - S_OK : �ɹ�
 * _ E_FAIL : ʧ��
 */
HRESULT CSWMemoryStream::Commit(DWORD dwCommitFlags)
{
	return  E_NOTIMPL;
}


/**
 * @brief �����ݿ�������
 *
 * @param [in] pStm : Ŀ����ָ��
 * @param [in] dwCb : �������ݵĳ���
 * @param [in] dwSrcPos : Դ������ʼ��
 * @param [in] dwDstPos : Ŀ��������ʼ��
 * @param [out] pdwLen : ʵ�ʿ��������ݳ���
 * @return
 * - S_OK : �ɹ�
 * - E_FAIL : ʧ��
 */
HRESULT CSWMemoryStream::CopyTo(CSWStream * pStm, DWORD dwCb, DWORD dwSrcPos, DWORD dwDstPos, DWORD * pdwLen)
{
	return  E_NOTIMPL;
}


/**
 * @brief ��ȡ�������뵽���ڴ���׵�ַ
 *
 * @return
 * - NULL : �ö�������ڴ�ʧ��
 * -����: �ڴ�ĵ�ַ
 */
PVOID CSWMemoryStream::GetAddr(){

	return  NULL;
}


/**
 * @brief ��ȡ�������뵽���ڴ�������׵�ַ
 *
 * @return
 * - NULL : �ö�������ڴ�ʧ��
 * -����: �ڴ�������ַ
 * @note
 * - Ŀǰֻ��DSP�����ڴ���Ч�������������ڴ���ԣ�����ֵ����GetAddr()�ķ���ֵ��ͬ
 */
LPVOID CSWMemoryStream::GetPhyAddr(){

	return  NULL;
}


/**
 * @brief �������豸�����ƺ���������
 *
 * @param [in] dwCmd : ��������ID
 * @param [in] pvArg : �����������ָ��
 * @return
 * - S_OK : �ɹ�
 * - E_FAIL : ʧ��
 */
HRESULT CSWMemoryStream::IOCtrl(DWORD dwCmd, VOID * pvArg)
{
	return  E_NOTIMPL;
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
HRESULT CSWMemoryStream::Read(PBYTE pvBuf, DWORD dwCB, PDWORD pdwRead)
{
	return  E_NOTIMPL;
}


/**
 * @brief �ض�λ�����ڲ�λ��ָ��
 *
 * @param [in] sdwOffset : ƫ����
 * @param [in] dwFromWhere : �����ʼλ�ã�0:�ļ�ͷ(SEEK_SET)��1:��ǰλ��(SEEK_CUR)��2:
 * �ļ�β(SEEK_END)
 * @param [out] pdwNewPos : ��λ�õ��ڲ��ļ�ָ��ֵ
 * @return
 * - S_OK : �ɹ�
 * - E_FAIL : ʧ��
 */
HRESULT CSWMemoryStream::Seek(INT sdwOffset, DWORD dwFromWhere, DWORD * pdwNewPos)
{
	return  E_NOTIMPL;
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
HRESULT CSWMemoryStream::Write(PBYTE pvBuf, DWORD dwCB, PDWORD pdwWritten)
{
	return  E_NOTIMPL;
}

