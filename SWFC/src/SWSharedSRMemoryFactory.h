///////////////////////////////////////////////////////////
//  SWShareSRMemoryFactory.h
//  Implementation of the Class CSWMemory
//  Created on:
//  Original author:
///////////////////////////////////////////////////////////

#if !defined(SWSHAREDSRMEMORYFACTORY__INCLUDED_)
#define SWSHAREDSRMEMORYFACTORY__INCLUDED_

#include "SWMemoryFactory.h"
#include "SWList.h"

typedef struct tag_MemoryInfo
{
	int iFlag;
	CSWMemory* pcMemory;
}SR_MEM_INFO;

/**
 * @brief �ڴ�������
 */
class CSWSharedSRMemoryFactory : public CSWMemoryFactory
{
	friend class CSWMemoryFactory;
	CLASSINFO(CSWSharedSRMemoryFactory,CSWMemoryFactory)
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
	CSWSharedSRMemoryFactory();
	~CSWSharedSRMemoryFactory();
private:
	CSWList<SR_MEM_INFO*> m_cMemoryList;
	CSWMutex m_cMutex;
};
#endif // !defined(SWSHAREDSRMEMORYFACTORY__INCLUDED_)

