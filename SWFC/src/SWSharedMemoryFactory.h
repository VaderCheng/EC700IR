///////////////////////////////////////////////////////////
//  CSWMemory.h
//  Implementation of the Class CSWMemory
//  Created on:      28-����-2013 14:09:48
//  Original author: zhouy
///////////////////////////////////////////////////////////

#if !defined(SWSHAREDMEMORYFACTORY__INCLUDED_)
#define SWSHAREDMEMORYFACTORY__INCLUDED_

#include "SWMemoryFactory.h"
#include "SWList.h"

/**
 * @brief �ڴ�������
 */
class CSWSharedMemoryFactory : public CSWMemoryFactory
{
	friend class CSWMemoryFactory;
	CLASSINFO(CSWSharedMemoryFactory,CSWMemoryFactory)
	
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

protected:
	/**
	 * @brief ���캯��
	*/
	CSWSharedMemoryFactory();
	/**
	 * @brief ��������
	 */
	~CSWSharedMemoryFactory();
	
private:
	CSWList<CSWMemory*, 1024> m_cMemoryList;
	CSWMutex m_cMutex;
};
#endif // !defined(SWSHAREDMEMORYFACTORY__INCLUDED_)

