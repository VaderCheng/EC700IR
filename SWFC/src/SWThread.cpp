///////////////////////////////////////////////////////////
//  CSWThread.cpp
//  Implementation of the Class CSWThread
//  Created on:      28-����-2013 14:09:46
//  Original author: zy
///////////////////////////////////////////////////////////

#include "SWThread.h"

typedef struct tga_THREAD_ATTR
{
    int stack_size;
    int priotiry;
}THREAD_ATTR_HANDLE;

LPVOID CSWThread::ThreadFunc(LPVOID pArg)
{
    CSWThread* pcThread = (CSWThread*)pArg;
    pcThread->m_fExit = FALSE;
    if(pcThread->m_pfRoutine)
    {
    	pcThread->m_pfRoutine(pcThread->m_pvArg);
    }
    else if(E_NOTIMPL == pcThread->Run())
    {
    	swpa_thread_sleep_ms(1000);
    }
    pcThread->m_fExit = TRUE;
    return NULL;
}

CSWThread::CSWThread(){

	m_pfRoutine = NULL;
	m_pvArg = NULL;
	
    m_dwPriority = NORMAL_PRIORITY_LEVEL;
	m_dwStackSize = NORMAL_STACK_SIZE;

    m_fExit = TRUE;
	m_hThread = 0;
}

/**
 * @brief ���캯��
 * @param  [in] szName : ���߳���
 * @param [in] dwStackSize: �̵߳�ջ��С
 * @param [in] dwPriority: �߳����ȼ�
 */
CSWThread::CSWThread(PCSTR szName, DWORD dwStackSize, DWORD dwPriority){

	m_pfRoutine = NULL;
	m_pvArg = NULL;
	
    m_dwPriority = dwPriority;
	m_dwStackSize = dwStackSize;

	m_fExit = TRUE;
	m_hThread = 0;
}


/**
 * @brief ��������
 */
CSWThread::~CSWThread(){

    Stop();	
}


/**
 * @brief ��ȡ�߳�����״̬
 * @param  [out] dwStatus : ״̬��
 * @return - S_OK : �ɹ� - E_FAIL : ʧ��
 */
HRESULT CSWThread::GetLastStatus(DWORD& dwStatus){

	return  E_NOTIMPL;
}


/**
 * @brief �ж��߳��Ƿ���Ч���Ƿ���
 * @return - S_OK : �ɹ� - E_FAIL : ʧ��
 */
HRESULT CSWThread::IsValid(){

	return  m_fExit ? E_FAIL : S_OK;
}


/**
 * @brief �߳������г��򣬸��߳������ظú���ʵ���书�ܡ�
 * @return - S_OK : �ɹ� - E_FAIL : ʧ��
 */
HRESULT CSWThread::Run(){

	return  E_NOTIMPL;
}


/**
 * @brief �����������߳�
 * @return - S_OK : �ɹ� - E_FAIL : ʧ��
 */
HRESULT CSWThread::Start(){

	THREAD_ATTR_HANDLE cAttr;
	cAttr.priotiry = m_dwPriority;
	cAttr.stack_size = m_dwStackSize;

	if (0 != m_hThread)
	{
		return E_ACCESSDENIED;
	}
	
	m_fExit = FALSE;
	swpa_thread_create(&m_hThread, cAttr.stack_size, cAttr.priotiry, (LPVOID (*)(LPVOID))ThreadFunc, this);
	if ( 0 != m_hThread )
	{
	    return S_OK;
	}
	return E_FAIL;
}


/**
 * @brief �����������߳�,ִ�д�����߳�������
 * 
 * @param [in] pfRoutine : �߳�������
 * @param [in] pvArg : �߳��������Ĳ���
 * @return
 * - S_OK : �ɹ�
 * - E_FAIL : ʧ��
 * @note
 * - START_ROUTINE�Ķ���Ϊ typedef VOID * (*START_ROUTINE)(VOID *);
 */
HRESULT CSWThread::Start(START_ROUTINE pfRoutine, PVOID pvArg){

	THREAD_ATTR_HANDLE cAttr;
	cAttr.priotiry = m_dwPriority;
	cAttr.stack_size = m_dwStackSize;

	if (0 != m_hThread)
	{
		return E_ACCESSDENIED;
	}
	
	m_pfRoutine = pfRoutine;
	m_pvArg = pvArg;
	
	m_fExit = FALSE;
	swpa_thread_create(&m_hThread, cAttr.stack_size, cAttr.priotiry, (LPVOID (*)(LPVOID))ThreadFunc, this);
	if ( 0 != m_hThread )
	{
		return S_OK;
	}
	return E_FAIL;
}


/**
 * @brief ֹͣ�̲߳��˳�
 * @return - S_OK : �ɹ� - E_FAIL : ʧ��
 */
HRESULT CSWThread::Stop(){

	m_fExit = TRUE;
	void* retval = NULL;
	
	if (0 != m_hThread)
	{
		swpa_thread_join(m_hThread, &retval);
		swpa_thread_destroy(m_hThread);
		m_hThread = 0;
	}
	return S_OK;
}

