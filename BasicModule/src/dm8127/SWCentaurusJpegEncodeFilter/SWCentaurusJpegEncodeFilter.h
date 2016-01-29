#ifndef __SW_CENTAURUS_JPEG_ENCODE_FILTER_H__
#define __SW_CENTAURUS_JPEG_ENCODE_FILTER_H__
#include "SWBaseFilter.h"

class CSWCentaurusJpegEncodeFilter : public CSWBaseFilter
{
	CLASSINFO(CSWCentaurusJpegEncodeFilter, CSWBaseFilter)
public:
	CSWCentaurusJpegEncodeFilter();
	virtual ~CSWCentaurusJpegEncodeFilter();
protected:
	virtual HRESULT Receive(CSWObject* obj);
	virtual HRESULT Run();
	virtual HRESULT Stop();
	
protected:
	
	HRESULT Initialize(PVOID pvParam);

	/**
	*@brief JPEGѹ���ص�����
	*
	*/
	INT PackJpegFrame(VOID *pvDataStruct);	
	static INT PackJpegFrameCallback(VOID *pvContext, INT iType, VOID *pvDataStruct);	

	SW_BEGIN_DISP_MAP(CSWCentaurusJpegEncodeFilter,CSWBaseFilter)
		SW_DISP_METHOD(Initialize, 1)
	SW_END_DISP_MAP();
	
private:
	BOOL IsInited() {return m_fInited;};
	
	/**
	*@brief ��ӡ����״̬��Ϣ����֡�ʡ��߿��
	*
	*/
	HRESULT PrintRunningInfo(CSWImage* pImage);
	
	/**
	*@brief ����JPEG
	*
	*/
	HRESULT SendJpeg();
	
	/**
	*@brief JPEG�����߳�������
	*
	*/
	static PVOID SendJpegProxy(PVOID pvArg);
	
private:
	BOOL m_fInited;
	
	DWORD m_dwQueueSize;
	CSWList<CSWImage*> m_lstJpeg;
	CSWSemaphore * m_pSemaLock;
	CSWSemaphore m_semaJpeg;
	CSWThread	m_cSendThread;

};
REGISTER_CLASS(CSWCentaurusJpegEncodeFilter)
#endif //__SW_CENTAURUS_JPEG_ENCODE_FILTER_H__

