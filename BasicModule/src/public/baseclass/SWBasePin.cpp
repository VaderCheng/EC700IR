///////////////////////////////////////////////////////////
//  CSWBasePin.cpp
//  Implementation of the Class CSWBasePin
//  Created on:      27-����-2013 16:29:45
//  Original author: huanggc
///////////////////////////////////////////////////////////
#include "SWFC.h"
#include "SWBasePin.h"
#include "SWBaseFilter.h"
#include "SWBaseGraph.h"


/**
 * @brief ���캯������Ҫ�������ָ�������뻹�����Pin
 * @param [in] pFilter Filterָ��
 * @param [in] dwDir ����0��ʾ����, 1��ʾ���
 */
CSWBasePin::CSWBasePin(CSWBaseFilter * pFilter, DWORD dwDir)
:m_pFilter(pFilter)
,m_dwDirection(dwDir)
,m_pIn(NULL)
,m_State(PIN_NONE)
{
}


CSWBasePin::~CSWBasePin(){

}

/**
 * @brief ��Ӵ˽ӿڵ����ݸ�ʽ��ֻ�з��ϴ˸�ʽ�����ݵ�Ԫ���ܾ����˽ӿ�
 * @param [in] dwObjectID �����id��
 * @return �ɹ�����S_OK,����ֵΪ�������
 */
HRESULT CSWBasePin::AddObject(DWORD dwObjectID){

	m_lstObjectID.AddTail(dwObjectID);
	return S_OK;
}	


/**
 * @brief ������һ��Pin,����Ҫ��ֻ������ܹ���������,���������ӷ�ʽ���ǲ������
 * @param [in] pIn ����
 * @return �ɹ�����S_OK������ֵΪ�������
 */
HRESULT CSWBasePin::Connect(CSWBasePin * pIn){

	if(NULL != pIn
	&& NULL == m_pIn
	&& 1 == m_dwDirection
	&& 0 == pIn->m_dwDirection
	&& GetFilter() != pIn->GetFilter()
	&& GetFilter()->GetGraph() == pIn->GetFilter()->GetGraph())
	{
		DWORD dwOutID, dwInID;
		SW_POSITION posOut = m_lstObjectID.GetHeadPosition(); 
		while(m_lstObjectID.IsValid(posOut))
		{ 
			dwOutID = m_lstObjectID.GetNext(posOut);
			SW_POSITION posIn = pIn->m_lstObjectID.GetHeadPosition();
			while(pIn->m_lstObjectID.IsValid(posIn))
			{
				dwInID = pIn->m_lstObjectID.GetNext(posIn);
				if(dwOutID == dwInID)
				{
					m_lstConnectID.AddTail(dwInID);
				}
			}
		}
		if(!m_lstConnectID.IsEmpty())
		{
			m_pIn = pIn;
			pIn->m_pIn = this;
			return S_OK;
		}
	}
	return E_FAIL;
}


/**
 * @brief ��������
 * @param [in] pdu����Э�鵥Ԫ
 * @return �ɹ�����S_OK������ֵΪ�������
 */
HRESULT CSWBasePin::Deliver(CSWObject *obj){

	if(NULL != m_pIn && 0 == m_pIn->m_dwDirection && NULL != obj)
	{
		SW_POSITION posOut = m_lstConnectID.GetHeadPosition(); 
		while(m_lstConnectID.IsValid(posOut))
		{ 
			INT id = m_lstConnectID.GetNext(posOut);
			if(obj->IsDescendant(id))
			{
				return m_pIn->Receive(obj);
			}
		}		
	}
	return E_FAIL;
}


/**
 * @brief ���Filterָ��
 * @return �ɹ�����Filterָ�룬ʧ�ܷ���NULL
 */
CSWBaseFilter* CSWBasePin::GetFilter(){

	return  m_pFilter;
}


/**
 * @brief ������Ӵ�Pin����һ��Pin
 * @return �ɹ�����Pinָ�룬ʧ�ܷ���NULL
 */
CSWBasePin* CSWBasePin::GetPin(){

	return  m_pIn;
}


/**
 * @brief ����Pin��״̬
 * @return ����Pin��״̬
 */
PIN_STATE CSWBasePin::GetState(){

	return  m_State;
}


/**
 * @brief ��ͣPin
 * @return �ɹ�����S_OK������ֵΪ�������
 */
HRESULT CSWBasePin::Pause(){

	m_State = PIN_PAUSED;
	return S_OK;
}


/**
 * @brief ������������ݰ�
 * @param [in] pdu �������ݰ�
 * @�ɹ�����S_OK������ֵΪ�������
 */
HRESULT CSWBasePin::Receive(CSWObject *obj){

	return  GetFilter() ? GetFilter()->Receive(obj) : E_FAIL;
}


/**
 * @brief ����Pin
 * @return �ɹ�����S_OK������ֵΪ�������
 */
HRESULT CSWBasePin::Run(){

	m_State = PIN_RUNNING;
	return S_OK;
}


/**
 * @brief ֹͣPin
 * @return �ɹ�����S_OK������ֵΪ�������
 */
HRESULT CSWBasePin::Stop(){

	m_State = PIN_STOPPED;
	return S_OK;
}

