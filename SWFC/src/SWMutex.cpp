///////////////////////////////////////////////////////////
//  CSWMutex.cpp
//  Implementation of the Class CSWMutex
//  Created on:      28-����-2013 14:09:49
//  Original author: zhouy
///////////////////////////////////////////////////////////

#include "SWMutex.h"

CSWMutex::CSWMutex(int MutexId){

	m_cMutex = 0;
	m_mutex_id = MutexId;
	Create(MutexId);
}

CSWMutex::~CSWMutex(){

    Delete();
}

HRESULT CSWMutex::Create(int MutexId){

	if (0 != m_cMutex)
	{
		Delete();
	}
	m_mutex_id = MutexId;
	if (0 == MutexId)
	{
		if (0 == swpa_mutex_create(&m_cMutex, NULL))
		{
			return S_OK;
		}
	}
	else
	{
		if (0 == swpa_processmutex_create(&m_cMutex, MutexId))
		{
			return S_OK;
		}
	}
	return  E_FAIL;
}

HRESULT CSWMutex::Delete(){

	if (0 == m_cMutex)
	{
		return S_OK;
	}
	else if (0 == swpa_mutex_delete(&m_cMutex))
	{
		if (0 == m_mutex_id)
		{
			m_cMutex = 0;
		}
		return S_OK;
	}
	else
	{
		return E_FAIL;
	}
}


/**
 * @brief ������öԸ�ͬ��������Ƶ���Դ�ķ��ʡ�
 * 
 * @param [in] dwTimeout : ��������ʱʱ������λΪ����
 * @return
 * - S_OK : �ɹ�
 * - E_FAIL : ʧ��
 * @note
 * - �����ͬ�������ǿ��õģ� ��Lock()���ɹ��ط��أ������̴߳�������ӵ�����������;
 * - ���ͬ�����󲻿��ã���Lock()����<i>dwTimeOut</i>����ָ���ĺ������ڵȴ���ͬ�������Ϊ���ã�
 * - ������ͬ��������ָ������ʱ����û�б�Ϊ���õģ� ��Lock()����ʧ�ܡ�
 */
HRESULT CSWMutex::Lock(INT iTimeout/* = -1 */){

    if ( 0 == m_cMutex )
    {
        return E_FAIL;
    }
	if ( 0 == swpa_mutex_lock(&m_cMutex, iTimeout) )
    {
        return S_OK;
    }
	return  E_FAIL;
}


/**
 * @brief ����
 * 
 * @return
 * - S_OK : �ɹ�
 * - E_FAIL : ʧ��
 */
HRESULT CSWMutex::Unlock(){

    if ( 0 == m_cMutex )
    {
        return E_FAIL;
    }
	if ( 0 == swpa_mutex_unlock(&m_cMutex) )
    {
        return S_OK;
    }
	return  E_FAIL;
}

HRESULT CSWMutex::GetId()
{
	return m_mutex_id;
}


CSWRWMutex::CSWRWMutex(){

    m_cRwlock = 0;
    Create();
}


CSWRWMutex::~CSWRWMutex(){

    Delete();
}


HRESULT CSWRWMutex::Create(){

    Delete();

    if ( 0 == swpa_rwlock_create(&m_cRwlock, NULL) )
    {
        return S_OK;
    }
	return  E_FAIL;
}


HRESULT CSWRWMutex::Delete(){

    if ( 0 == m_cRwlock )
    {
        return S_OK;
    }
	else if ( 0 == swpa_rwlock_delete(&m_cRwlock) )
    {
        return S_OK;
    }
    else
    {
	    return E_FAIL;
    }
}


/**
 * @brief ������
 * 
 * @param [in] dwTimeout : ��������ʱʱ������λΪ����
 * @return
 * - S_OK : �ɹ�
 * - E_FAIL : ʧ��
 * @note .
 */
HRESULT CSWRWMutex::RLock(INT iTimeout/* = -1 */){

    if ( 0 == m_cRwlock )
    {
        return E_FAIL;
    }
	if ( 0 == swpa_rwlock_lock(&m_cRwlock, iTimeout, 1) )
    {
        return S_OK;
    }
	return  E_FAIL;
}


/**
 * @brief ����
 * 
 * @return
 * - S_OK : �ɹ�
 * - E_FAIL : ʧ��
 */
HRESULT CSWRWMutex::Unlock(){

    if ( 0 == m_cRwlock )
    {
        return E_FAIL;
    }
	if ( 0 == swpa_rwlock_unlock(&m_cRwlock) )
    {
        return S_OK;
    }
	return  E_FAIL;
}


/**
 * @brief д����
 * 
 * @param [in] dwTimeout : ��������ʱʱ������λΪ����
 * @return
 * - S_OK : �ɹ�
 * - E_FAIL : ʧ��
 * @note .
 */
HRESULT CSWRWMutex::WLock(INT iTimeout/* = -1 */){

    if ( 0 == m_cRwlock )
    {
        return E_FAIL;
    }
	if ( 0 == swpa_rwlock_lock(&m_cRwlock, iTimeout, 0) )
    {
        return S_OK;
    }
	return  E_FAIL;
}




CSWSyncObject::CSWSyncObject(){

}


CSWSyncObject::~CSWSyncObject(){

}


HRESULT CSWSyncObject::Create(int MutexId){

	return  E_NOTIMPL;
}


HRESULT CSWSyncObject::Delete(){

	return  E_NOTIMPL;
}


/**
 * @brief ������öԸ�ͬ��������Ƶ���Դ�ķ��ʡ�
 * 
 * @param [in] dwTimeout : ��������ʱʱ������λΪ����
 * @return
 * - S_OK : �ɹ�
 * - E_FAIL : ʧ��
 * @note
 * - �����ͬ�������ǿ��õģ� ��Lock()���ɹ��ط��أ������̴߳�������ӵ�����������;
 * - ���ͬ�����󲻿��ã���Lock()����<i>dwTimeOut</i>����ָ���ĺ������ڵȴ���ͬ�������Ϊ���ã�
 * - ������ͬ��������ָ������ʱ����û�б�Ϊ���õģ� ��Lock()����ʧ�ܡ�
 */
HRESULT CSWSyncObject::Lock(INT iTimeout/* = -1 */){

	return  E_NOTIMPL;
}


/**
 * @brief ����
 * 
 * @return
 * - S_OK : �ɹ�
 * - E_FAIL : ʧ��
 */
HRESULT CSWSyncObject::Unlock(){

	return  E_NOTIMPL;
}

