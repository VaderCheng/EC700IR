///////////////////////////////////////////////////////////
//  SWMappedMemoryFactory.h
//  Implementation of the Class CSWMemory
//  Created on:      28-����-2013 14:09:48
//  Original author: zhouy
///////////////////////////////////////////////////////////

#if !defined(SWMAPPEDMEMORYFACTORY__INCLUDED_)
#define SWMAPPEDMEMORYFACTORY__INCLUDED_

#include "SWMemoryFactory.h"

/**
 * @brief �ڴ�������
 */
class CSWMappedMemoryFactory : public CSWMemoryFactory
{
	friend class CSWMemoryFactory;
	CLASSINFO(CSWMappedMemoryFactory,CSWMemoryFactory)

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
	CSWMappedMemoryFactory();
	/**
	 * @brief ��������
	 */
	~CSWMappedMemoryFactory();
};
#endif // !defined(SWSHAREDMEMORYFACTORY__INCLUDED_)

