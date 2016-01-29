///////////////////////////////////////////////////////////
//  CSWMemory.cpp
//  Implementation of the Class CSWMemory
//  Created on:      28-����-2013 14:09:48
//  Original author: zhouy
///////////////////////////////////////////////////////////
#include "SWFC.h"
#include "SWMemory.h"

/**
 * @brief ���캯��
 */
CSWMemory::CSWMemory(PVOID pvAddr, PVOID pvPhys, const INT& iSize, CSWMemoryFactory* pMemoryFactory)
	: m_pvAddr(pvAddr)
	, m_pvPhys(pvPhys)
	, m_iSize(iSize)
	, m_iTag(0)
	, m_pMemoryFactory(pMemoryFactory)
{
}

/**
 * @brief ��������
 */
CSWMemory::~CSWMemory()
{
	
}

/**
* @brief  ��ȡ������������ڴ档
*
* @param [in] iAddrType : ��ַ���ͣ�0: Ĭ��������ƽ̨�йأ�1��ǿ��ȡ�����ַ��2��ǿ��ȡ�����ַ��
* @return
* - �ڴ��׵�ַ
*/
PVOID CSWMemory::GetBuffer(const INT& iAddrType)
{
	PVOID pvRet = NULL;
	switch(iAddrType)
	{
	case 0:
#if RUN_PLATFORM == PLATFORM_DSP_BIOS
		pvRet = m_pvPhys;
#else 
		pvRet = m_pvAddr;
#endif
		break;
	case 1:
		pvRet = m_pvPhys;
		break;
	case 2:
		pvRet = m_pvAddr;
		break;
	default:
		pvRet = NULL;
		break;
	}
	return pvRet;
}

/**
* @brief  ��ȡ�����������ڴ�Ĵ�С��
*
* @param [in] iAddrType : ��ַ���ͣ�0: Ĭ��������ƽ̨�йأ�1��ǿ��ȡ�����ַ��2��ǿ��ȡ�����ַ��
* @return
*  -�����㣺�ڴ�Ĵ�С��
*  -С�ڵ����㣺����
*/ 
INT CSWMemory::GetSize()
{
	return m_iSize;
}



