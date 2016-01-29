#include "SWFC.h"
#include "SWSharedMemoryFactory.h"
#include "swpa_mem.h"

/**
* @brief ���캯��
*/
CSWSharedMemoryFactory::CSWSharedMemoryFactory()
{
	swpa_sharedmem_init();
}
/**
* @brief ��������
*/
CSWSharedMemoryFactory::~CSWSharedMemoryFactory()
{
	CSWAutoLock aLock(&m_cMutex);
	while(!m_cMemoryList.IsEmpty())
	{
		CSWMemory* pMemory = m_cMemoryList.GetHead();
		Free(pMemory);
		m_cMemoryList.RemoveHead();
	}
	swpa_sharedmem_uninit();
}

/**
* @brief �����ڴ�
* 
* @param [in] dwSize : Ҫ�����ڴ�Ĵ�С
* @return
* - NULL : ����ʧ��
* - ����: ���䵽���ڴ����ָ��
*/
CSWMemory* CSWSharedMemoryFactory::Alloc(const INT& iSize)
{
	CSWAutoLock aLock(&m_cMutex);
	PVOID pvAddr = NULL;
	PVOID pvPhys = NULL;
	INT iRet = swpa_sharedmem_alloc(&pvAddr, &pvPhys, iSize);
	if( iRet != 0 )
	{
		SWFC_DEBUG_ERR("swpa_sharedmem_alloc failed!");
		return NULL;
	}
	CSWMemory* pMemory = new CSWMemory(pvAddr, pvPhys, iSize, this);
	SW_POSITION pos = m_cMemoryList.AddTail(pMemory);
	if( !m_cMemoryList.IsValid(pos) )
	{
		SW_TRACE_DEBUG("SharedMemoryFactory alloc list full!.\n");
	}
	return pMemory;
}

CSWMemory* CSWSharedMemoryFactory::Alloc(PVOID pAddr, PVOID pPhys, const INT& iSize)
{
	CSWAutoLock aLock(&m_cMutex);
	CSWMemory* pMemory = new CSWMemory(pAddr, pPhys, iSize, this);
	pMemory->SetTag(1);
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
HRESULT CSWSharedMemoryFactory::Free(CSWMemory* pMemory)
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
			INT iRet = 0;
			if(!pMemory->GetTag())
			{
				iRet = swpa_sharedmem_free(pMemory->GetBuffer(MEM_PHY_ADDR), 1);
			}
			else
			{
				swpa_ipnc_release(pMemory->GetBuffer());
			}
			m_cMemoryList.RemoveAt(posLast);
			delete pMem;
			pMem = NULL;
			return iRet == 0 ? S_OK : E_FAIL;
		}
	}

	SWFC_DEBUG_ERR("CSWSharedMemoryFactory free failed!");
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
HRESULT CSWSharedMemoryFactory::GetAvailableSize(DWORD * pdwSize)
{
	if( pdwSize == NULL )
	{
		return E_INVALIDARG;
	}

	*pdwSize = 0;
	SWPA_MEM_INFO cMemInfo;
	if( 0 == swpa_sharedmem_getinfo(&cMemInfo) )
	{
		(*pdwSize) = cMemInfo.free;
		return S_OK;
	}

	return E_FAIL;
}
/**
* @brief ��ȡ�ڴ����������λΪ�ֽ�
* 
* @param [out] pdwSize : ��ȡ��������
* @return
* - S_OK : �ɹ�
* - E_FAIL : ʧ��
*/
HRESULT CSWSharedMemoryFactory::GetTotalSize(DWORD * pdwSize)
{
	if( pdwSize == NULL )
	{
		return E_INVALIDARG;
	}

	*pdwSize = 0;
	SWPA_MEM_INFO cMemInfo;
	if( 0 == swpa_sharedmem_getinfo(&cMemInfo) )
	{
		(*pdwSize) = cMemInfo.size;
		return S_OK;
	}

	return E_FAIL;
}

