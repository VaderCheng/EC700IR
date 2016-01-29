#include "swpa.h"
#include "SWFC.h"
#include "SWNormalMemoryFactory.h"

/**
* @brief ���캯��
*/
CSWNormalMemoryFactory::CSWNormalMemoryFactory()
{
	swpa_mem_init();
}
/**
* @brief ��������
*/
CSWNormalMemoryFactory::~CSWNormalMemoryFactory()
{
	CSWAutoLock aLock(&m_cMutex);
	while(!m_cMemoryList.IsEmpty())
	{
		CSWMemory* pMemory = m_cMemoryList.GetHead();
		Free(pMemory);
		m_cMemoryList.RemoveHead();
	}
	swpa_mem_uninit();
}

/**
* @brief �����ڴ�
* 
* @param [in] dwSize : Ҫ�����ڴ�Ĵ�С
* @return
* - NULL : ����ʧ��
* - ����: ���䵽���ڴ����ָ��
*/
CSWMemory* CSWNormalMemoryFactory::Alloc(const INT& iSize)
{
	CSWAutoLock aLock(&m_cMutex);
	PVOID pvAddr = swpa_mem_alloc(iSize);
	PVOID pvPhys = pvAddr;
	if( pvAddr == NULL )
	{
		SWFC_DEBUG_ERR("swpa_mem_alloc failed!");
		return NULL;
	}
	CSWMemory* pMemory = new CSWMemory(pvAddr, pvPhys, iSize, this);
	m_cMemoryList.AddTail(pMemory);
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
HRESULT CSWNormalMemoryFactory::Free(CSWMemory* pMemory)
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
		CSWMemory* pMem = m_cMemoryList.GetNext(pos);
		if( pMem == pMemory )
		{
			INT iRet = swpa_mem_free(pMemory->GetBuffer());
			m_cMemoryList.RemoveAt(posLast);
			delete pMem;
			pMem = NULL;
			return iRet == 0 ? S_OK : E_FAIL;
		}
	}

	SWFC_DEBUG_ERR("CSWNormalMemoryFactory free failed!");
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
HRESULT CSWNormalMemoryFactory::GetAvailableSize(DWORD * pdwSize)
{
	return E_NOTIMPL;
}
/**
* @brief ��ȡ�ڴ����������λΪ�ֽ�
* 
* @param [out] pdwSize : ��ȡ��������
* @return
* - S_OK : �ɹ�
* - E_FAIL : ʧ��
*/
HRESULT CSWNormalMemoryFactory::GetTotalSize(DWORD * pdwSize)
{
	return E_NOTIMPL;
}

