/**
* @file SWResultVideoTransformFilter.h 
* @brief H264����ģ��
* 
* @copyright Signalway All Rights Reserved
* @author zhouy
* @date 2013-03-22
* @version 1.0
*/


#ifndef __SW_RESULT_VIDEO_TRANSFORM_FILTER_H__
#define __SW_RESULT_VIDEO_TRANSFORM_FILTER_H__

#include "SWBaseFilter.h"
#include "SWMessage.h"
#include "SWRecord.h"
#include "SWList.h"




/**
* @brief H264����ģ��
*
*
* �������ܣ�һ���Ǳ������ݣ���һ���Ƕ�ȡ���ݡ�
* 
*/
class CSWResultVideoTransformFilter : public CSWBaseFilter, CSWMessage
{
	CLASSINFO(CSWResultVideoTransformFilter, CSWBaseFilter)
		
public:

	/**
	* @brief ���캯��
	* 
	*/
	CSWResultVideoTransformFilter();

	/**
	* @brief ��������
	* 
	*/
	virtual ~CSWResultVideoTransformFilter();

	/**
	* @brief ��ʼ������
	* @retval S_OK : �ɹ�
	* @retval E_INVALIDARG : �����Ƿ�
	* @retval E_OUTOFMEMORY : �ڴ治��
	* @retval E_FAIL : ��ʼ��ʧ��
	* 
	*/
	HRESULT Initialize();

	virtual HRESULT Run();

	virtual HRESULT Stop();

	virtual HRESULT Receive(CSWObject* obj);	

	


	SW_BEGIN_DISP_MAP(CSWResultVideoTransformFilter,CSWBaseFilter)
		SW_DISP_METHOD(Initialize, 0)
	SW_END_DISP_MAP();

protected:




	
private:

	static PVOID AddVideoProxy(PVOID pvArg);

	HRESULT AddVideo();



	SW_BEGIN_MESSAGE_MAP(CSWResultVideoTransformFilter, CSWMessage)
		
	SW_END_MESSAGE_MAP();

private:

	BOOL				m_fInited;
	
	CSWList<CSWImage*>	m_lstFrame;	/// ֡����
	
	CSWList<CSWRecord*>	m_lstRecord;	/// �������
	CSWSemaphore		m_cRecordLock;	
		
	DWORD				m_dwQueueSize;
	CSWSemaphore		m_cSemaQueueSync;

	CSWMemoryFactory * 	m_pcMemoryFactory;

	CSWSemaphore        m_cTickLock;	/// �����洢ʱ�������	

	CSWThread			m_cThread;
	
};

REGISTER_CLASS(CSWResultVideoTransformFilter)
	
#endif //__SW_RESULT_VIDEO_TRANSFORM_FILTER_H__


