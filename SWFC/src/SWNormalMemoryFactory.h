///////////////////////////////////////////////////////////
//  SWNormalMemoryFactory.h
//  Implementation of the Class CSWMemory
//  Created on:      28-����-2013 14:09:48
//  Original author: zhouy
///////////////////////////////////////////////////////////

#if !defined(SWNORMALMEMORYFACTORY__INCLUDED_)
#define SWNORMALMEMORYFACTORY__INCLUDED_

#include "SWMemoryFactory.h"
#include "SWList.h"

/**
 * @brief �ڴ�������
 */
class CSWNormalMemoryFactory : public CSWMemoryFactory
{
	friend class CSWMemoryFactory;
	CLASSINFO(CSWNormalMemoryFactory,CSWMemoryFactory)
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
	CSWNormalMemoryFactory();
	/**
	 * @brief ��������
	 */
	~CSWNormalMemoryFactory();

private:
	CSWList<CSWMemory*> m_cMemoryList;
	CSWMutex m_cMutex;
};
#endif // !defined(SWSHAREDMEMORYFACTORY__INCLUDED_)

