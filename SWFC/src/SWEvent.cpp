///////////////////////////////////////////////////////////
//  CSWEvent.cpp
//  Implementation of the Class CSWEvent
//  Created on:      28-����-2013 14:09:50
//  Original author: zhouy
///////////////////////////////////////////////////////////

#include "SWEvent.h"




CSWEvent::CSWEvent(){

}


CSWEvent::~CSWEvent(){

}


HRESULT CSWEvent::Create(){

	return  E_NOTIMPL;
}


HRESULT CSWEvent::Delete(){

	return  E_NOTIMPL;
}


/**
 * @brief Sets the event to available (signaled), releases waiting threads, and
 * sets the event to unavailable (nonsignaled).
 * @return
 * - S_OK : �ɹ�
 * - E_FAIL : ʧ��
 * @note .
 */
HRESULT CSWEvent::PulseEvent(){

	return  E_NOTIMPL;
}


/**
 * @brief Sets the event to unavailable (nonsignaled).
 * 
 * @return
 * - S_OK : �ɹ�
 * - E_FAIL : ʧ��
 * @note .
 */
HRESULT CSWEvent::ResetEvent(){

	return  E_NOTIMPL;
}


/**
 * @brief Sets the event to available (signaled) and releases any waiting threads.
 * 
 * 
 * @return
 * - S_OK : �ɹ�
 * - E_FAIL : ʧ��
 * @note .
 */
HRESULT CSWEvent::SetEvent(){

	return  E_NOTIMPL;
}


/**
 * @brief ����
 * 
 * @return
 * - S_OK : �ɹ�
 * - E_FAIL : ʧ��
 */
HRESULT CSWEvent::Unlock(){

	return  E_NOTIMPL;
}

