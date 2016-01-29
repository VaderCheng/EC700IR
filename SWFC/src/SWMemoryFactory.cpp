#include "SWFC.h"
#include "SWMemoryFactory.h"
#include "SWNormalMemoryFactory.h"
#include "SWSharedMemoryFactory.h"
#include "SWMappedMemoryFactory.h"
#include "SWCacheMemoryFactory.h"
#include "SWSharedSRMemoryFactory.h"

/**
* @brief ���캯��
*/
CSWMemoryFactory::CSWMemoryFactory()
{
}
/**
* @brief ��������
*/
CSWMemoryFactory::~CSWMemoryFactory()
{
}

CSWMemoryFactory* CSWMemoryFactory::GetInstance(const SW_MEMORY_TYPE& cMemoryType)
{
	CSWMemoryFactory *pMemory = NULL;
	switch(cMemoryType)
	{
	case SW_NORMAL_MEMORY:
		{
			static CSWNormalMemoryFactory normalMemory;
			pMemory = &normalMemory;
		}
		break;
		
	case SW_SHARED_MEMORY:
		{
			static CSWSharedMemoryFactory shareMemory;
			pMemory = &shareMemory;
		}
		break;

	case SW_CACHE_MEMORY:
		{
			static CSWCacheMemoryFactory cacheMemory;
			pMemory = &cacheMemory;
		}
		break;

	case SW_MAPPED_MEMORY:
		{
			static CSWMappedMemoryFactory mmapMemory;
			pMemory = &mmapMemory;
		}
		break;
	case SW_SHARED_SR_MEMORY:
		{
			static CSWSharedSRMemoryFactory srShareMemory;
			pMemory = &srShareMemory;
		}	
		break;
	}
	return pMemory;
}

/**
 * @brief �����ڴ�
 * 
 * @param [in] dwSize : Ҫ�����ڴ�Ĵ�С
 * @return
 * - NULL : ����ʧ��
 * - ����: ���䵽���ڴ����ָ��
 */
CSWMemory* CSWMemoryFactory::Alloc(const INT& iSize)
{  
	return NULL;
}

CSWMemory* CSWMemoryFactory::Alloc(PVOID pAddr, PVOID pPhys, const INT& iSize)
{
	return Alloc(iSize);
}

/**
 * @brief �ͷ��ڴ�
 * 
 * @param [in] pMemory : Ҫ�ͷŵ��ڴ����ָ��
 * @return
 * - S_OK : �ɹ�
 * - E_FAIL: ʧ��
 */
HRESULT CSWMemoryFactory::Free(CSWMemory* pMemory)
{
	return E_NOTIMPL;
}

/**
 * @brief ��ȡ�ڴ�Ŀ���������λΪ�ֽ�
 * 
 * @param [out] pdwSize : ��ȡ���Ŀ�����
 * @return
 * - S_OK : �ɹ�
 * - E_FAIL : ʧ��
 */
HRESULT CSWMemoryFactory::GetAvailableSize(DWORD * pdwSize)
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
HRESULT CSWMemoryFactory::GetTotalSize(DWORD * pdwSize)
{
	return E_NOTIMPL;
}

