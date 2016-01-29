///////////////////////////////////////////////////////////
//  CSWBaseGraph.cpp
//  Implementation of the Class CSWBaseGraph
//  Created on:      27-����-2013 16:29:45
//  Original author: huanggc
///////////////////////////////////////////////////////////
#include "swpa.h"
#include "SWFC.h"
#include "SWBasePin.h"
#include "SWBaseFilter.h"
#include "SWBaseGraph.h"


CSWBaseGraph::CSWBaseGraph(){

}


CSWBaseGraph::~CSWBaseGraph(){

	SW_POSITION pos = m_lstFilter.GetHeadPosition();
	while(m_lstFilter.IsValid(pos))
	{
		m_lstFilter.GetNext(pos)->Release();
	}
}

INT CSWBaseGraph::GetVersion(VOID)
{
#ifdef VERSION
	return VERSION;
#else
	return -1;
#endif
}

/**
 * @brief ���һ��Filter��ֻ�����Filter��AMȥ����ʵ������
 * @param [in] pszName ָ��Filter������
 * @param [in] pFilter Filter����
 * @return �ɹ�����S_OK,����ֵ����������
 */
HRESULT CSWBaseGraph::AddFilter(LPCSTR pszName, CSWBaseFilter* pFilter){

	if(NULL != pszName && NULL != pFilter)
	{
		CSWBaseFilter* pFindFilter = NULL;
		if(NULL != (pFindFilter = QueryFilter(pszName)))
		{
			pFindFilter->Release();
			return E_FAIL;
		}
			
		pFilter->AddRef();
		pFilter->SetName(pszName);
		pFilter->m_pGraph = this;
		m_lstFilter.AddTail(pFilter);
		
		return S_OK;
	}
	return E_FAIL;
}


/**
 * @brief ����2��Pin
 * @param [in] pOut���Pin
 * @param [in] pIn ����Pin
 * @return �ɹ�����S_OK������ֵΪ�������
 */
HRESULT CSWBaseGraph::Connect(CSWBasePin* pOut, CSWBasePin* pIn){

	if(NULL != pOut
	&& NULL != pIn
	&& NULL != pOut->GetFilter()
	&& NULL != pIn->GetFilter()
	&& this == pOut->GetFilter()->GetGraph()
	&& this == pIn->GetFilter()->GetGraph())
	{
		return pOut->Connect(pIn);
	}
	return E_FAIL;
}


/**
 * @brief ��Ӧ���µ�Filter�������¼�
 * @param [in] pFilter �������¼���Filter
 * @param [in] dwID  dwID
 * @param [in] wParam �������1
 * @param [in] lParam �������2
 * @return Э���Զ��巵������
 */
HRESULT CSWBaseGraph::OnCommand(CSWBaseFilter* pFilter, DWORD dwID, DWORD wParam, DWORD lParam){

	return  E_NOTIMPL;
}


/**
 * @brief ��ͣ����Filter�����У������ʵ���ǵ�������������Filter��Pause����
 * @return �ɹ�����S_OK������ֵ����������
 */
HRESULT CSWBaseGraph::Pause(){

	SW_POSITION pos = m_lstFilter.GetHeadPosition();
	while(m_lstFilter.IsValid(pos))
	{
		m_lstFilter.GetNext(pos)->Pause();
	}
	return S_OK;
}


/**
 * @brief ��ѯAM���Ƿ��д�Filter,ͨ��pszName��ѯ
 * @param [in] pszName Filter������
 * @param [out] pFilter  �����ѯ����Filter
 * @return �ɹ�����S_OK,����ֵΪ�������
 */
CSWBaseFilter* CSWBaseGraph::QueryFilter(LPCSTR pszName){

	CSWBaseFilter* pFindFilter = NULL;
	bool fOk = false;
	SW_POSITION pos = m_lstFilter.GetHeadPosition();
	while(m_lstFilter.IsValid(pos))
	{
		pFindFilter = m_lstFilter.GetNext(pos);
		if(!swpa_strcmp(pFindFilter->GetName(), pszName))
		{
			pFindFilter->AddRef();
			fOk = true;
			break;
		}
	}
	return fOk ? pFindFilter : NULL;
}


/**
 * @brief ɾ��Filter
 * @param [in] pszName Filter����
 * @return �ɹ�����S_OK������ֵΪ�������
 */
HRESULT CSWBaseGraph::RemoveFilter(LPCSTR lpzName){

	CSWBaseFilter* pFindFilter = NULL;
	SW_POSITION pos = m_lstFilter.GetHeadPosition();
	while(m_lstFilter.IsValid(pos))
	{
		SW_POSITION posNow = pos;
		pFindFilter = m_lstFilter.GetNext(pos);
		if(!swpa_strcmp(pFindFilter->GetName(), lpzName))
		{
			pFindFilter->Release();
			m_lstFilter.RemoveAt(posNow);
			return S_OK;
		}
	}
	return E_FAIL;
}


/**
 * @brief  �������е�Filter,���ڲ�ʵ���ǵ���������Filter��Run����
 * @return �ɹ�����S_OK������ֵΪ�������
 */
HRESULT CSWBaseGraph::Run(){

	SW_POSITION pos = m_lstFilter.GetHeadPosition();
	while(m_lstFilter.IsValid(pos))
	{
		m_lstFilter.GetNext(pos)->Run();
	}
	return S_OK;
}


/**
 * @brief  ֹͣ���е�Filter,���ڲ�ʵ���ǵ���������Filter��Stop����
 * @return �ɹ�����S_OK������ֵΪ�������
 */
HRESULT CSWBaseGraph::Stop(){

	SW_POSITION pos = m_lstFilter.GetHeadPosition();
	while(m_lstFilter.IsValid(pos))
	{
		CSWBaseFilter *pFilter = m_lstFilter.GetNext(pos);
		SW_TRACE_NORMAL("stop %s...\n", pFilter->GetName());
		pFilter->Stop();
	}
	return S_OK;
}

FILTER_STATE CSWBaseGraph::GetState()
{
	FILTER_STATE fs = FILTER_RUNNING;
	SW_POSITION pos = m_lstFilter.GetHeadPosition();
	while(m_lstFilter.IsValid(pos))
	{
		CSWBaseFilter *pFilter = m_lstFilter.GetNext(pos);
		if(FILTER_RUNNING != pFilter->GetState())
		{
			SW_TRACE_NORMAL("%s error.\n", pFilter->GetName());
			fs = pFilter->GetState();
		}
	}
	return fs;
}

