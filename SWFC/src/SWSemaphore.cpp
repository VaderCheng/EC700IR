///////////////////////////////////////////////////////////
//  CSWSemaphore.cpp
//  Implementation of the Class CSWSemaphore
//  Created on:      28-����-2013 14:09:49
//  Original author: zhouy
///////////////////////////////////////////////////////////

#include "SWSemaphore.h"

CSWSemaphore::CSWSemaphore(){

    m_cSem = 0;
}

/**
 * @brief ���캯��
 * 
 * @param [in] dwInitCount : ��ʼ��Ŀ��Ĭ��Ϊ1
 * @param [in] dwMaxCount : �����Ŀ��Ĭ��Ϊ1
 */
CSWSemaphore::CSWSemaphore(DWORD dwInitCount, DWORD dwMaxCount, DWORD dwSemId){

	m_cSem = 0;
	m_sem_id = dwSemId;
	if (dwSemId == 0)
	{
		/* �����ڵ��ź��� */
		swpa_sem_create(&m_cSem, dwInitCount, dwMaxCount);
	}
	else
	{
		/* ���̼���ź��� */
		swpa_processsem_create(&m_cSem, dwInitCount, dwMaxCount, dwSemId);
	}
}

/**
 * @brief ��������
 */
CSWSemaphore::~CSWSemaphore(){

    Delete();
}

/**
 * @brief �ź�����������
 * 
 * @param [in] dwInitCount : ��ʼ��Ŀ��Ĭ��Ϊ1
 * @param [in] dwMaxCount : �����Ŀ��Ĭ��Ϊ1
 * @return
 * - S_OK : �ɹ�
 * - E_FAIL : ʧ��
 */
HRESULT CSWSemaphore::Create(DWORD dwInitCount, DWORD dwMaxCount, DWORD dwSemId){

	m_sem_id = dwSemId;

	Delete();
	if (dwSemId == 0)
	{
		/* �����ڵ��ź��� */
		if (0 == swpa_sem_create(&m_cSem, dwInitCount, dwMaxCount))
		{
			return S_OK;
		}
	}
	else
	{
		/* ���̼���ź��� */
		if (0 == swpa_processsem_create(&m_cSem, dwInitCount, dwMaxCount, dwSemId))
		{
			return S_OK;
		}
	}
	
	return  E_FAIL;
}

HRESULT CSWSemaphore::Delete(){

	if (0 == m_cSem)
	{
		return S_OK;
	}
	if (0 == swpa_sem_delete(&m_cSem))
	{
		if (0 == m_sem_id)
		{
			m_cSem = 0;
		}
		return S_OK;
	}
	return  E_FAIL;
}

/**
 * @brief ������ͬ��������Ƶ���Դ��
 * 
 * @param [in] dwTimeout : ��������ʱʱ������λΪ����
 * @return
 * - S_OK : �ɹ�
 * - E_FAIL : ʧ��
 * @note
 * - �����ͬ�������ǿ��õģ� ��Pend()���ɹ��ط��أ������̴߳�������ӵ�����������;
 * - ���ͬ�����󲻿��ã���Pend()����<i>dwTimeOut</i>����ָ���ĺ������ڵȴ���ͬ�������Ϊ���ã�
 * - ������ͬ��������ָ������ʱ����û�б�Ϊ���õģ� ��Pend()����ʧ�ܡ�
 */
HRESULT CSWSemaphore::Pend(const INT sdwTimeout){

    if ( 0 == m_cSem )
    {
        return E_FAIL;
    }
    if ( 0 == swpa_sem_pend(&m_cSem, sdwTimeout) )
    {
        return S_OK;
    }
	return  E_FAIL;
}

/**
 * @brief ����Ը�ͬ��������Ƶ���Դ������
 * 
 * @param [in] dwTimeout : ����������ʱʱ������λΪ����@return
 * - S_OK : �ɹ�
 * - E_FAIL : ʧ��
 */
HRESULT CSWSemaphore::Post(){

    if ( 0 == m_cSem )
    {
        return E_FAIL;
    }
	if ( 0 == swpa_sem_post(&m_cSem) )
    {
        return S_OK;
    }
	return  E_FAIL;
}

HRESULT CSWSemaphore::Value()
{
	return swpa_sem_value(&m_cSem);
}

HRESULT CSWSemaphore::GetId()
{
	return m_sem_id;
}

