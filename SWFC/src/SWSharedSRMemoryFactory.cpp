#include "swpa.h"
#include "SWFC.h"
#include "SWSharedSRMemoryFactory.h"

/**
* @brief ���캯��
*/
CSWSharedSRMemoryFactory::CSWSharedSRMemoryFactory()
{
}
/**
* @brief ��������
*/
CSWSharedSRMemoryFactory::~CSWSharedSRMemoryFactory()
{
	CSWAutoLock aLock(&m_cMutex);
	while(!m_cMemoryList.IsEmpty())
	{
		SR_MEM_INFO* pcMemSR = m_cMemoryList.GetHead();
		if( pcMemSR != NULL && pcMemSR->pcMemory != NULL )
		{
			CSWMemory* pMem = pcMemSR->pcMemory;
			Free(pMem);
		}
		m_cMemoryList.RemoveHead();
	}
}

/**
* @brief �����ڴ�
* 
* @param [in] dwSize : Ҫ�����ڴ�Ĵ�С
* @return
* - NULL : ����ʧ��
* - ����: ���䵽���ڴ����ָ��
*/
CSWMemory* CSWSharedSRMemoryFactory::Alloc(const INT& iSize)
{
	CSWAutoLock aLock(&m_cMutex);
	int iFlag = 2;
	PVOID pvTemp = swpa_ipnc_share_region_malloc(iFlag, iSize);
	if( pvTemp == NULL )
	{
		iFlag = 1;
		pvTemp = swpa_ipnc_share_region_malloc(iFlag, iSize);
		if( pvTemp == NULL )
		{
			SWFC_DEBUG_ERR("IPC_ShareRegion_Malloc failed!");
			return NULL;
		}
	}
	PVOID pvAddr = pvTemp;
	PVOID pvPhys = pvTemp;
	CSWMemory* pMemory = new CSWMemory(pvAddr, pvPhys, iSize, this);

	SR_MEM_INFO* pcMemInfo = new SR_MEM_INFO;
	if( pcMemInfo == NULL )
	{
		if( pMemory != NULL )
		{
			delete pMemory;
			pMemory = NULL;
		}
		return NULL;
	}

	pcMemInfo->iFlag = iFlag;
	pcMemInfo->pcMemory = pMemory;

	m_cMemoryList.AddTail(pcMemInfo);
	return pMemory;
}

/**
* @brief �ͷ��ڴ�
* 
* @param [in] pMemory : Ҫ�ͷŵ��ڴ����ָ��
* @return
* - S_OK : �ɹ�
* - E_FAIL: ʧ��
*/
HRESULT CSWSharedSRMemoryFactory::Free(CSWMemory* pMemory)
{
	CSWAutoLock aLock(&m_cMutex);
	if( NULL == pMemory )
	{
		return E_INVALIDARG;
	}

	SW_POSITION pos = m_cMemoryList.GetHeadPosition();
	while(m_cMemoryList.IsValid(pos))
	{
		SW_POSITION posLast = pos;
		SR_MEM_INFO* pcMemSR = m_cMemoryList.GetNext(pos);
		if( pcMemSR != NULL && pcMemSR->pcMemory != NULL && pcMemSR->pcMemory == pMemory)
		{
			CSWMemory* pMem = pcMemSR->pcMemory;
			swpa_ipnc_share_region_free(pcMemSR->iFlag, pMem->GetBuffer(), pMem->GetSize());
			m_cMemoryList.RemoveAt(posLast);
			delete pMem;
			delete pcMemSR;
			pMem = NULL;
			pcMemSR = NULL;
			return S_OK;
		}
	}

	SWFC_DEBUG_ERR("CSWSharedSRMemoryFactory free failed!");
	return E_FAIL;
}

/**
* @brief ��ȡ�ڴ�Ŀ���������λΪ�ֽ�
* 
* @param [out] pdwSize : ��ȡ���Ŀ�����
* @return
* - S_OK : �ɹ�
* - E_FAIL : ʧ��
*/
HRESULT CSWSharedSRMemoryFactory::GetAvailableSize(DWORD * pdwSize)
{
	INT iSize = 0;
	swpa_ipnc_share_region_get_free_size(&iSize);
	*pdwSize = (DWORD)iSize;
	return S_OK;
}
/**
* @brief ��ȡ�ڴ����������λΪ�ֽ�
* 
* @param [out] pdwSize : ��ȡ��������
* @return
* - S_OK : �ɹ�
* - E_FAIL : ʧ��
*/
HRESULT CSWSharedSRMemoryFactory::GetTotalSize(DWORD * pdwSize)
{
	INT iSize = 0;
	swpa_ipnc_share_region_get_total_size(&iSize);
	*pdwSize = (DWORD)iSize;
	return S_OK;
}

