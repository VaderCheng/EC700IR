///////////////////////////////////////////////////////////
//  CSWObject.cpp
//  Implementation of the Class CSWObject
//  Created on:      28-����-2013 14:09:46
//  Original author: zy
///////////////////////////////////////////////////////////

#include "SWObject.h"

/**
 * @brief ���캯��
 */
CSWObject::CSWObject(){

    m_dwRef = 1;
    swpa_mutex_create(&m_hMutex, NULL);
}

/**
 * @brief ��������
 */
CSWObject::~CSWObject(){

	swpa_mutex_delete(&m_hMutex);
}

/**
 * @brief ���ü�����1
 */
DWORD CSWObject::AddRef(){
    
  swpa_mutex_lock(&m_hMutex, -1);
  ++m_dwRef;
	swpa_mutex_unlock(&m_hMutex);
	return m_dwRef;
}

/**
 * @brief ���ü�����1�����ڼ���Ϊ0ʱ���ٵ��ö���
 * 
 * @return
 * - S_OK : �ɹ�
 * - E_FAIL : ʧ��
 */
HRESULT CSWObject::Release(){

  swpa_mutex_lock(&m_hMutex, -1);
  int iRef = --m_dwRef;
	swpa_mutex_unlock(&m_hMutex);

  if ( iRef == 0 ) 
  {
      delete this;
  }
	return S_OK;
}

DWORD CSWObject::GetRefCount(){
	return m_dwRef;
}
