/**
* @file SWH264QueueRenderFilter.h 
* @brief H264����ģ��
* 
* @copyright Signalway All Rights Reserved
* @author zhouy
* @date 2013-03-22
* @version 1.0
*/


#ifndef __SW_H264_QUEUE_RENDER_FILTER_H__
#define __SW_H264_QUEUE_RENDER_FILTER_H__

#include "SWBaseFilter.h"
#include "SWMessage.h"
#include "SWImage.h"
#include "SWList.h"




/**
* @brief H264����ģ��
*
*
* �������ܣ�һ���Ǳ������ݣ���һ���Ƕ�ȡ���ݡ�
* 
*/
class CSWH264QueueRenderFilter : public CSWBaseFilter, CSWMessage
{
	CLASSINFO(CSWH264QueueRenderFilter, CSWBaseFilter)
		
public:

	/**
	* @brief ���캯��
	* 
	*/
	CSWH264QueueRenderFilter();

	/**
	* @brief ��������
	* 
	*/
	virtual ~CSWH264QueueRenderFilter();

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
	
	
public:


	SW_BEGIN_DISP_MAP(CSWH264QueueRenderFilter,CSWBaseFilter)
		SW_DISP_METHOD(Initialize, 0)
	SW_END_DISP_MAP();


protected:


	
	HRESULT FrameEnqueue(CSWImage* pImage);

	/**
	* @brief �ѹؼ�֡�ŵ�֡����
	* @param [in] pImage : �ؼ�֡����ָ��
	* @param [out] piBufSize : �����֡���ݴ�С
	* @retval S_OK : �ɹ�
	* @retval E_INVALIDARG : �����Ƿ�
	* @retval E_OUTOFMEMORY : �ڴ治��
	* @retval E_FAIL : ʧ��
	*/
	HRESULT KeyFrameEnqueue(CSWImage* pImage, INT* piBufSize);


	/**
	* @brief �ѷǹؼ�֡�ŵ�֡����
	* @param [in] pImage : �ǹؼ�֡����ָ��
	* @param [out] piBufSize : �����֡���ݴ�С
	* @retval S_OK : �ɹ�
	* @retval E_INVALIDARG : �����Ƿ�
	* @retval E_OUTOFMEMORY : �ڴ治��
	* @retval E_FAIL : ʧ��
	*/
	HRESULT NonKeyFrameEnqueue(CSWImage* pImage, INT* piBufSize);

	/**
	* @brief ���ļ����ñ������
	* @param [in] sTimeStamp : �ļ���ʱ���
	* @retval S_OK : �ɹ�
	* @retval E_INVALIDARG : �����Ƿ�
	* @retval E_OUTOFMEMORY : �ڴ治��
	* @retval E_FAIL : ʧ��
	*/
	HRESULT FileEnqueue(const SWPA_TIME& sTimeStamp);

	/**
	* @brief H264��Ƶ�ļ������߳�������
	* @retval S_OK : �ɹ�
	* @retval E_INVALIDARG : �����Ƿ�
	* @retval E_OUTOFMEMORY : �ڴ治��
	* @retval E_FAIL : ʧ��
	*/
	HRESULT OnHistoryFileSaving(VOID);


	

	/**
	* @brief H264��Ƶ��ʷ�ļ������߳�������
	* @param [in] dwPinID : ��ʷ�ļ�����Pin ID
	* @retval S_OK : �ɹ�
	* @retval E_INVALIDARG : �����Ƿ�
	* @retval E_OUTOFMEMORY : �ڴ治��
	* @retval E_FAIL : ʧ��
	*/
	HRESULT OnHistoryFileTransmitting(const DWORD dwPinID);


	
	HRESULT GetImageTimeStamp(CSWImage * pImage, SWPA_TIME* pTimeStamp);


	
private:
	
	HRESULT ClearBuffer();
	
	static PVOID H264EnqueueProxy(PVOID pvArg);

	HRESULT H264Enqueue();

	
	HRESULT OnGetH264(WPARAM wParam, LPARAM lParam);



	SW_BEGIN_MESSAGE_MAP(CSWH264QueueRenderFilter, CSWMessage)
		SW_MESSAGE_HANDLER(MSG_H264_QUEUE_GET_VIDEO, OnGetH264)
	SW_END_MESSAGE_MAP();

private:

	BOOL				m_fInited;
	
	CSWList<CSWImage*>	m_lstFrame;	/// ֡����
	
	CSWList<CSWObject*>	m_lstObj;	/// �������
	CSWSemaphore		m_cObjLock;	/// ���󱣴������	
	
	CSWList<CSWImage*>	m_lstFile;		/// �ļ��������	
	CSWSemaphore		m_cFileLock;	/// �ļ����������	
	
	DWORD				m_dwQueueSize;
	CSWSemaphore		m_cSemaObjSync;

	CSWMemoryFactory * 	m_pcMemoryFactory;

	CSWThread			m_cThread;
	
};

REGISTER_CLASS(CSWH264QueueRenderFilter)
	
#endif //__SW_H264_QUEUE_RENDER_FILTER_H__


