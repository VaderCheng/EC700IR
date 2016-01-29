///////////////////////////////////////////////////////////
//  SWCacheMemoryFactory.h
//  Implementation of the Class CSWMemory
//  Created on:      28-����-2013 14:09:48
//  Original author: zhouy
///////////////////////////////////////////////////////////

#if !defined(SWCACHEMEMORYFACTORY__INCLUDED_)
#define SWCACHEMEMORYFACTORY__INCLUDED_

#include "SWMemoryFactory.h"

/**
 * @brief �ڴ�������
 */
class CSWCacheMemoryFactory : public CSWMemoryFactory
{
	friend class CSWMemoryFactory;
	CLASSINFO(CSWCacheMemoryFactory,CSWMemoryFactory)
public:
	 /**
	 * @brief �����ڴ�
	 * 
	 * @param [in] dwSize : Ҫ�����ڴ�Ĵ�С
	 * @return
	 * - NULL : ����ʧ��
	 * - ����: ���䵽���ڴ����ָ��
	 */
	virtual CSWMemory* Alloc(const int& iSize);

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

protected:
	/**
	 * @brief ���캯��
	*/
	CSWCacheMemoryFactory();
	/**
	 * @brief ��������
	 */
	~CSWCacheMemoryFactory();

};
#endif // !defined(SWMAPPEDMEMORYFACTORY__INCLUDED_)

