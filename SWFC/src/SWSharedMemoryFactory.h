///////////////////////////////////////////////////////////
//  CSWMemory.h
//  Implementation of the Class CSWMemory
//  Created on:      28-二月-2013 14:09:48
//  Original author: zhouy
///////////////////////////////////////////////////////////

#if !defined(SWSHAREDMEMORYFACTORY__INCLUDED_)
#define SWSHAREDMEMORYFACTORY__INCLUDED_

#include "SWMemoryFactory.h"
#include "SWList.h"

/**
 * @brief 内存管理基类
 */
class CSWSharedMemoryFactory : public CSWMemoryFactory
{
	friend class CSWMemoryFactory;
	CLASSINFO(CSWSharedMemoryFactory,CSWMemoryFactory)
	
public:
	 /**
	 * @brief 申请内存
	 * 
	 * @param [in] dwSize : 要申请内存的大小
	 * @return
	 * - NULL : 申请失败
	 * - 其它: 分配到的内存对象指针
	 */
	virtual CSWMemory* Alloc(const int& iSize);
	
	virtual CSWMemory* Alloc(PVOID pAddr, PVOID pPhys, const INT& iSize);
	/**
	 * @brief 释放内存
	 * 
	 * @param [in] pMemory : 要释放的内存对象指针
	 * @return
	 * - S_OK : 成功
	 * - E_FAIL: 失败
	 */
	virtual HRESULT Free(CSWMemory* pMemory);

	/**
	 * @brief 获取内存的可用量，单位为字节
	 * 
	 * @param [out] pdwSize : 获取到的可用量
	 * @return
	 * - S_OK : 成功
	 * - E_FAIL : 失败
	 */
	virtual HRESULT GetAvailableSize(DWORD * pdwSize);
	/**
	 * @brief 获取内存的总量，单位为字节
	 * 
	 * @param [out] pdwSize : 获取到的总量
	 * @return
	 * - S_OK : 成功
	 * - E_FAIL : 失败
	 */
	virtual HRESULT GetTotalSize(DWORD * pdwSize);

protected:
	/**
	 * @brief 构造函数
	*/
	CSWSharedMemoryFactory();
	/**
	 * @brief 析构函数
	 */
	~CSWSharedMemoryFactory();
	
private:
	CSWList<CSWMemory*, 1024> m_cMemoryList;
	CSWMutex m_cMutex;
};
#endif // !defined(SWSHAREDMEMORYFACTORY__INCLUDED_)

