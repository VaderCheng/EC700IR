///////////////////////////////////////////////////////////
//  CSWMemory.h
//  Implementation of the Class CSWMemory
//  Created on:      28-����-2013 14:09:48
//  Original author: zhouy
///////////////////////////////////////////////////////////

#if !defined(SWMEMORYFACTORY__INCLUDED_)
#define SWMEMORYFACTORY__INCLUDED_

#include "SWObject.h"
#include "SWMemory.h"

typedef enum
{
	SW_NORMAL_MEMORY = 0,
	SW_SHARED_MEMORY,
	SW_CACHE_MEMORY,
	SW_MAPPED_MEMORY,
	SW_SHARED_SR_MEMORY
}
SW_MEMORY_TYPE;


/**
 * @brief �ڴ�������
 */
class CSWMemoryFactory : public CSWObject
{
	
CLASSINFO(CSWMemoryFactory,CSWObject)

public:
	static CSWMemoryFactory* GetInstance(const SW_MEMORY_TYPE& cMemoryType = SW_SHARED_MEMORY);

public:
	/**
	 * @brief �����ڴ�
	 * 
	 * @param [in] dwSize : Ҫ�����ڴ�Ĵ�С
	 * @return
	 * - NULL : ����ʧ��
	 * - ����: ���䵽���ڴ����ָ��
	 */
	virtual CSWMemory* Alloc(const INT& iSize);
	
	/**
	 * @brief �����ڴ�
	 * 
	 * @param [in] pAddr  : �ڴ����ַ
	 * @param [in] pPhys  : �ڴ�ʵ�ʵ�ַ
	 * @param [in] dwSize : Ҫ�����ڴ�Ĵ�С
	 * @return
	 * - NULL : ����ʧ��
	 * - ����: ���䵽���ڴ����ָ��
	 */
	virtual CSWMemory* Alloc(PVOID pAddr, PVOID pPhys, const INT& iSize);

	/**
	 * @brief �ͷ��ڴ�
	 * 
	 * @param [in] pMemory : Ҫ�ͷŵ��ڴ����ָ��
	 * @return
	 * - S_OK : �ɹ�
	 * - E_FAIL: ʧ��
	 */
	virtual HRESULT Free(CSWMemory* pMemory);

	/**
	 * @brief ��ȡ�ڴ�Ŀ���������λΪ�ֽ�
	 * 
	 * @param [out] pdwSize : ��ȡ���Ŀ�����
	 * @return
	 * - S_OK : �ɹ�
	 * - E_FAIL : ʧ��
	 */
	virtual HRESULT GetAvailableSize(DWORD * pdwSize);
	/**
	 * @brief ��ȡ�ڴ����������λΪ�ֽ�
	 * 
	 * @param [out] pdwSize : ��ȡ��������
	 * @return
	 * - S_OK : �ɹ�
	 * - E_FAIL : ʧ��
	 */
	virtual HRESULT GetTotalSize(DWORD * pdwSize);

	virtual DWORD AddRef()
	{
		return 0;
	}
	virtual HRESULT Release()
	{
		return S_OK;
	}

protected:
	/**
	 * @brief ���캯��
	 */
	CSWMemoryFactory();
	/**
	 * @brief ��������
	 */
	virtual ~CSWMemoryFactory();
};
#endif // !defined(SWMEMORYFACTORY__INCLUDED_)

