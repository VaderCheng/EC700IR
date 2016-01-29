///////////////////////////////////////////////////////////
//  CSWAutoLock.cpp
//  Implementation of the Class CSWAutoLock
//  Created on:      28-����-2013 14:09:54
//  Original author: zhouy
///////////////////////////////////////////////////////////

#include "SWAutoLock.h"

CSWAutoLock::CSWAutoLock(){

    m_pcSyncObj = NULL;
}

/**
 * @brief ���캯�����ڸú����е���pcSyncObj��lock()������pcSyncObj������
 */
CSWAutoLock::CSWAutoLock(CSWSyncObject* pcSyncObj){

    m_pcSyncObj = pcSyncObj;
    if ( m_pcSyncObj != NULL )
    {
        m_pcSyncObj->Lock(-1);
    }
}

/**
 * @brief �������������������pcSyncObj��unlock()��������pcSyncObj������
 */
CSWAutoLock::~CSWAutoLock(){

    if ( m_pcSyncObj != NULL )
    {
        m_pcSyncObj->Unlock();
    }
}

HRESULT CSWAutoLock::Lock(INT iTimeout/* = -1*/)
{
    if ( m_pcSyncObj != NULL )
    {
        return m_pcSyncObj->Lock(iTimeout);
    }
    return E_FAIL;
}

HRESULT CSWAutoLock::Unlock()
{
    if ( m_pcSyncObj != NULL )
    {
        return m_pcSyncObj->Unlock();
    }
    return E_FAIL;
}

