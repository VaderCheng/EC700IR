#include "SWFC.h"
#include "SWCacheMemoryFactory.h"

/**
* @brief ���캯��
*/
CSWCacheMemoryFactory::CSWCacheMemoryFactory()
{
}
/**
* @brief ��������
*/
CSWCacheMemoryFactory::~CSWCacheMemoryFactory()
{
}

/**
* @brief �����ڴ�
* 
* @param [in] dwSize : Ҫ�����ڴ�Ĵ�С
* @return
* - NULL : ����ʧ��
* - ����: ���䵽���ڴ����ָ��
*/
CSWMemory* CSWCacheMemoryFactory::Alloc(const INT& iSize)
{
	return NULL;
}

/**
* @brief �ͷ��ڴ�
* 
* @param [in] pMemory : Ҫ�ͷŵ��ڴ����ָ��
* @return
* - S_OK : �ɹ�
* - E_FAIL: ʧ��
*/
HRESULT CSWCacheMemoryFactory::Free(CSWMemory* pMemory)
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
HRESULT CSWCacheMemoryFactory::GetAvailableSize(DWORD * pdwSize)
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
HRESULT CSWCacheMemoryFactory::GetTotalSize(DWORD * pdwSize)
{
	return E_NOTIMPL;
}

