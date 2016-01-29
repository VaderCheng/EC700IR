// ���ļ������ʽ������WIN936
#ifndef SWCENTAURUSSOURCEFILTER_H
#define SWCENTAURUSSOURCEFILTER_H

#include "SWBaseFilter.h"
#include "SWMessage.h"


class CSWCentaurusSourceFilter : public CSWBaseFilter, public CSWMessage
{
    CLASSINFO(CSWCentaurusSourceFilter, CSWBaseFilter)
public:
    CSWCentaurusSourceFilter();
    virtual ~CSWCentaurusSourceFilter();

	static VOID OnProcessH264Proxy(PVOID pvParam);		//����H264�ص�����
	static VOID	OnProcessJPEGProxy(PVOID pvParam);		//

protected:
    virtual HRESULT Run();
    virtual HRESULT Stop();
    static int OnResult(void *pContext, int type, void *struct_ptr);

	/**
   	*@brief �������߳�
   	*/
	HRESULT OnProcessH264();
	HRESULT OnProcessJPEG();
	
protected:

	HRESULT Initialize( INT iCVBSExport, INT iCVBSCropStartX, INT iCVBSCropStartY);

	/**
	*@brief ��ȡJPEG�ɼ�֡��
	*
	*/
	HRESULT OnGetJpegRawFps(WPARAM wParam,LPARAM lParam);

	/**
	*@brief ��ȡH264�ɼ�֡��
	*
	*/
	HRESULT OnGetH264RawFps(WPARAM wParam,LPARAM lParam);

	/**
	*@brief ��ȡH264�ڶ�ͨ���ɼ�֡��
	*
	*/
	HRESULT OnGetH264SecondRawFps(WPARAM wParam,LPARAM lParam);

	/**
	*@brief ���ò�����ͼƬ��չ��Ϣ
	*
	*/
	HRESULT UpdateExtensionInfo(CSWImage* pImage, IMAGE *pImageInfo);

	
	/**
	*@brief ͼƬ��չ��Ϣ
	*
	*/
	static PVOID GetExtensionInfo(PVOID pvArg);
	
	 //��Ϣӳ��궨��
    SW_BEGIN_MESSAGE_MAP(CSWCentaurusSourceFilter, CSWMessage)
    	SW_MESSAGE_HANDLER(MSG_SOURCE_GET_JPEG_RAW_FPS, OnGetJpegRawFps)
    	SW_MESSAGE_HANDLER(MSG_SOURCE_GET_H264_RAW_FPS, OnGetH264RawFps)
    	SW_MESSAGE_HANDLER(MSG_SOURCE_GET_H264_SECOND_RAW_FPS, OnGetH264SecondRawFps)
    SW_END_MESSAGE_MAP()

    //�Զ���ӳ���
	SW_BEGIN_DISP_MAP(CSWCentaurusSourceFilter, CSWBaseFilter)
		SW_DISP_METHOD(Initialize, 3)
	SW_END_DISP_MAP()
	
public:
	static const INT MAX_IMAGE_COUNT = 1;

private:
	static FLOAT	m_fltJpegRawFps;
	static FLOAT	m_fltH264RawFps;
	static FLOAT 	m_fltH264SecondRawFps;
	static INT m_iJpegFrameCount;
	static INT m_iH264FrameCount;
	static INT m_iJpegDropCount;
	static INT m_iH264DropCount;
	static INT m_iH264SecondFrameCount;
	static INT m_iCVBSExport;
	static INT m_iCVBSCropStartX;
	static INT m_iCVBSCropStartY;

	BOOL m_fInitialized;

	CSWList<CSWImage*> m_lstH264Image;		// H264ͼƬ����
	CSWList<CSWImage*> m_lstJPEGImage;		// JPEG

	CSWMutex m_cMutexH264Image;             // H264�����ٽ���
	CSWMutex m_cMutexJPEGImage; 			// JPEG�����ٽ���

	CSWThread* m_pThreadH264;				// H264�����߳�
	CSWSemaphore m_cSemImageH264;		

	CSWThread* m_pThreadJPEG;				// JPEG�����߳�
	CSWSemaphore m_cSemImageJPEG;		

	INT m_iShutter;
	INT m_iAGCGain;
	INT m_iRGBGain[3];
	CSWThread m_cThreadExtInfo;	

public:
	static INT m_iAvgY;
	static INT m_iRawAvgY;
};
REGISTER_CLASS(CSWCentaurusSourceFilter)


#endif // SWCENTAURUSSOURCEFILTER_H
