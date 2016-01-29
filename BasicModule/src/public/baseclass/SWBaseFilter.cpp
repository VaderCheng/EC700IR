///////////////////////////////////////////////////////////
//  CSWBaseFilter.cpp
//  Implementation of the Class CSWBaseFilter
//  Created on:      27-����-2013 16:29:44
//  Original author: huanggc
///////////////////////////////////////////////////////////
#include "swpa.h"
#include "SWFC.h"
#include "SWBasePin.h"
#include "SWBaseFilter.h"
#include "SWBaseGraph.h"

/**
 * @brief ���캯������Ҫָ����Filter��������ĸ���
 */
CSWBaseFilter::CSWBaseFilter(DWORD dwInCount, DWORD dwOutCount)
:m_dwInCount(dwInCount)
,m_dwOutCount(dwOutCount)
,m_pGraph(NULL)
,m_fsState(FILTER_NONE)
{
	m_pIn = new CSWBasePin*[GetInCount()];
	for(DWORD i = 0; i < GetInCount(); i++)
	{
		m_pIn[i] = NULL;
	}
	
	m_pOut = new CSWBasePin*[GetOutCount()];
	for(DWORD i = 0; i < GetOutCount(); i++)
	{
		m_pOut[i] = NULL;
	}
}


CSWBaseFilter::~CSWBaseFilter(){

	for(DWORD i = 0; i < GetInCount(); i++)
	{
		if(m_pIn[i])
		{
			m_pIn[i]->Release();
		}
	}
	delete []m_pIn;
	
	for(DWORD i = 0; i < GetOutCount(); i++)
	{
		if(m_pOut[i])
		{
			m_pOut[i]->Release();
		}
	}
	delete []m_pOut;
}

/**
 *@brief ģ��ĳ�ʼ������
 *@param [in] pvParam ģ������ṹ��ָ��
 *@return �ɹ�����S_OK������ֵΪ�������
 */
HRESULT CSWBaseFilter::Initialize(PVOID pvParam)
{
	return E_NOTIMPL;
}

/**
 * @brief ����Filter������
 * @param [in] pszName ����Filter������
 * @return S_OK��ʾ�ɹ�,����ֵΪ�������
 */
HRESULT CSWBaseFilter::SetName(LPCSTR pszName)
{
	m_strName = "";
	if(pszName && swpa_strlen(pszName) > 0)
	{
		m_strName = pszName;
	}
	return S_OK;
}


/**
 * @brief ��ȡFilter������
 * @return �ɹ�����Filter�����ƣ�ʧ�ܷ���NULL
 */
LPCSTR CSWBaseFilter::GetName(void)
{
	return m_strName;
}
	
	
/**
 * @brief ��������
 * @param [in] pdu����Э�鵥Ԫ
 * @return �ɹ�����S_OK������ֵΪ�������
 */
HRESULT CSWBaseFilter::Deliver(CSWObject* obj){

	for(DWORD i = 0; i < GetOutCount(); i++)
	{
		GetOut(i)->Deliver(obj);
	}
	return S_OK;
}


/**
 * @brief ���AM��ָ��
 * @return ����AMָ��
 */
CSWBaseGraph* CSWBaseFilter::GetGraph(){

	return  m_pGraph;
}


/**
 * @brief ��������ָ��
 * @param [in] n �ڼ�������
 * @return �ɹ���������ָ�룬���󷵻�NULL
 */
CSWBasePin* CSWBaseFilter::GetIn(DWORD n){

	if(n < GetInCount())
	{
		if(NULL == m_pIn[n])
		{
			m_pIn[n] = CreateIn(n);
		}
		return m_pIn[n];
	}
	return NULL;
}


/**
 * @brief ������������
 * @return �������������
 */
DWORD CSWBaseFilter::GetInCount(){

	return  m_dwInCount;
}

/**
 *@brief ��������Pin
 *@param [in] n�ڼ�������
 *@return ��������Pinָ��
 */
CSWBasePin* CSWBaseFilter::CreateIn(DWORD n)
{
	return new CSWBasePin(this, 0);
}


/**
 * @brief ��������ָ��
 * @param [in] n �ڼ������
 * @return �ɹ��������ָ�룬���󷵻�NULL
 */
CSWBasePin* CSWBaseFilter::GetOut(DWORD n){

	if(n < GetOutCount())
	{
		if(NULL == m_pOut[n])
		{
			m_pOut[n] = CreateOut(n);
		}
		return m_pOut[n];
	}
	return NULL;
}


/**
 * @brief ������������
 * @return �������������
 */
DWORD CSWBaseFilter::GetOutCount(){

	return  m_dwOutCount;
}

/**
 *@brief �������Pin
 *@param [in] n�ڼ������
 *@return ��������Pinָ��
 */
CSWBasePin* CSWBaseFilter::CreateOut(DWORD n)
{
	return new CSWBasePin(this, 1);
}	
/**
 * @brief ����Filter��״̬
 */
FILTER_STATE CSWBaseFilter::GetState(){

	if(m_fsState == FILTER_RUNNING)
	{
		PIN_STATE bsState = PIN_RUNNING;
		for(DWORD i = 0; i < GetInCount() && bsState == PIN_RUNNING; i++)
		{	
			if(bsState != GetIn(i)->GetState())
			{
				bsState = GetIn(i)->GetState();
				break;
			}
		}
	
		for(DWORD i = 0; i < GetOutCount() && bsState == PIN_RUNNING; i++)
		{
			if(bsState != GetOut(i)->GetState())
			{
				bsState = GetOut(i)->GetState();
				break;
			}
		}
		switch(bsState)
		{
		case PIN_PAUSED  : m_fsState = FILTER_PAUSED; break;
		case PIN_STOPPED : m_fsState = FILTER_STOPPED;break;
		}
	}
	return  m_fsState;
}


/**
 * @brief ��ͣFilter
 * @return �ɹ�����S_OK������ֵΪ�������
 */
HRESULT CSWBaseFilter::Pause(){

	m_fsState = FILTER_PAUSED;
	for(DWORD i = 0; i < GetInCount(); i++)
	{
		GetIn(i)->Pause();
	}
	
	for(DWORD i = 0; i < GetOutCount(); i++)
	{
		GetOut(i)->Pause();
	}	
	return S_OK;
}


/**
 * @brief ������������ݰ�
 * @param [in] pdu �������ݰ�
 * @�ɹ�����S_OK������ֵΪ�������
 */
HRESULT CSWBaseFilter::Receive(CSWObject* obj){

	return Deliver(obj);
}


/**
 * @brief ����Filter
 * @return �ɹ�����S_OK������ֵΪ�������
 */
HRESULT CSWBaseFilter::Run(){

	m_fsState = FILTER_RUNNING;
	for(DWORD i = 0; i < GetInCount(); i++)
	{
		GetIn(i)->Run();
	}
	
	for(DWORD i = 0; i < GetOutCount(); i++)
	{
		GetOut(i)->Run();
	}
	return S_OK;
}


/**
 * @brief ����AM��������
 * @param [in] dwID  dwID
 * @param [in] wParam �������1
 * @param [in] lParam �������2
 * @return Э���Զ��巵������
 */
HRESULT CSWBaseFilter::SendCommand(DWORD dwID, DWORD wParam, DWORD lParam){

	return  GetGraph() ? GetGraph()->OnCommand(this, dwID, wParam, lParam) : E_NOTIMPL;
}


/**
 * @brief ֹͣFilter
 * @return �ɹ�����S_OK������ֵΪ�������
 */
HRESULT CSWBaseFilter::Stop(){

	m_fsState = FILTER_STOPPED;
	for(DWORD i = 0; i < GetInCount(); i++)
	{
		GetIn(i)->Stop();
	}
	
	for(DWORD i = 0; i < GetOutCount(); i++)
	{
		GetOut(i)->Stop();
	}
	return S_OK;
}

