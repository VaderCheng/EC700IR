/**
* @file SWH264HDDTransformFilter.h 
* @brief H264����ģ��
* 
* @copyright Signalway All Rights Reserved
* @author zhouy
* @date 2013-03-22
* @version 1.0
*/


#ifndef __SW_H264HDD_TRANSFORM_FILTER_H__
#define __SW_H264HDD_TRANSFORM_FILTER_H__

#include "SWBaseFilter.h"
#include "SWMessage.h"
#include "SWImage.h"
#include "SWList.h"
//#include "BigFile.h"
#include "SWHDDTransformFilter.h"


#define H264HDD_TRANSFORM_IN_COUNT  1
#define H264HDD_TRANSFORM_OUT_COUNT 4




/**
* @brief H264����ģ��
*
*
* �������ܣ�һ���Ǳ������ݣ���һ���Ƕ�ȡ���ݡ�
* 
*/
class CSWH264HDDTransformFilter : public CSWHDDTransformFilter, CSWMessage
{
	CLASSINFO(CSWH264HDDTransformFilter, CSWHDDTransformFilter)
		
public:

	/**
	* @brief ���캯��
	* 
	*/
	CSWH264HDDTransformFilter();

	/**
	* @brief ��������
	* 
	*/
	virtual ~CSWH264HDDTransformFilter();

	/**
	* @brief ��ʼ������
	* @param [in] szDirecory : �洢��ʷ�ļ��Ĵ���·��
	* @param [in] llTotalSize : �洢��ʷ�ļ��Ĵ����ܿռ��С
	* @param [in] iFileSize : ������ʷ�ļ��Ķ�����С
	* @retval S_OK : �ɹ�
	* @retval E_INVALIDARG : �����Ƿ�
	* @retval E_OUTOFMEMORY : �ڴ治��
	* @retval E_FAIL : ��ʼ��ʧ��
	* 
	*/
	HRESULT Initialize(const CHAR * szDirecory, const LONGLONG llTotalSize, const INT iFileSize, const INT iSaveVideoType);

	
	
public:

	HRESULT SetSendInterval(const INT iTimeInterval)
	{
		if (0 > iTimeInterval)
		{
			return E_INVALIDARG;
		}
		
		m_iSendInterval = iTimeInterval;
		return S_OK;
	}

	INT GetSendInterval(VOID)
	{
		return m_iSendInterval;
	}
	HRESULT SetVideoSaveEnable(const BOOL fVideoSaveEnable );
	
	BOOL GetVideoSaveEnable(VOID);


	SW_BEGIN_DISP_MAP(CSWH264HDDTransformFilter,CSWBaseFilter)
		SW_DISP_METHOD(Initialize, 4)
		SW_DISP_METHOD(SetSendInterval, 1)
		SW_DISP_METHOD(SetSaveType, 1)
		SW_DISP_METHOD(SetVideoSaveEnable, 1)
		//SW_DISP_METHOD(StopHistoryFileTransmitting, 1)
		//SW_DISP_METHOD(GetHistoryFileTransmittingStatus, 2)
	SW_END_DISP_MAP();

	virtual HRESULT Receive(CSWObject* obj);	

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


	
private:

	virtual HRESULT ReportStatus(LPCSTR szInfo=NULL);
	
	virtual HRESULT ClearBuffer();
	
	HRESULT SaveVideo(BYTE* pVideoFile);

	CSWImage * CreateVideo(PBYTE pbRawData);
	
	/**
	* @brief �������pin�Ĵ�����������
	* @param [in] dwPinID : Pin ID��ȡֵΪ0��1
	* @param [in] dwOutType : ������ͣ�ȡֵΪH264HDD_TRANSMITTING_HISTORY ��H264HDD_TRANSMITTING_REALTIME
	* @retval S_OK : �ɹ�
	* @retval E_INVALIDARG : �����Ƿ�
	* @retval E_FAIL : ʧ��
	* 
	*/
	HRESULT OnSetTransmittingType(WPARAM wParam, LPARAM lParam);
	

	/**
	* @brief ����ĳ��Pin������ʷ�ļ�����
	* @param [in] dwPinID : Pin ID��ȡֵΪ0��1
	* @param [in] szTimeBegin : ��ʷ�ļ�����ʼʱ��
	* @param [in] szTimeEnd : ��ʷ�ļ��Ľ���ʱ��
	* @retval S_OK : �ɹ�
	* @retval E_INVALIDARG : �����Ƿ�
	* @retval E_OUTOFMEMORY : �ڴ治��
	* @retval E_FAIL : ʧ��
	* 
	*/
	HRESULT OnStartHistoryFileTransmitting(WPARAM wParam, LPARAM lParam);


	/**
	* @brief ֹͣĳ��Pin������ʷ�ļ�����
	* @param [in] dwPinID : Pin ID��ȡֵΪ0��1
	* @retval S_OK : �ɹ�
	* @retval E_INVALIDARG : �����Ƿ�
	* @retval E_OUTOFMEMORY : �ڴ治��
	* @retval E_FAIL : ʧ��
	* 
	*/
	HRESULT OnStopHistoryFileTransmitting(WPARAM wParam, LPARAM lParam);


	/**
	* @brief ?a??GB28181 Pin��?��?��������???t��y?Y
	* @param [in] dwPinID : Pin ID
	* @param [in] szTimeBegin : ��������???t��??e��?����??
	* @param [in] szTimeEnd : ��������???t��??����?����??
	* @retval S_OK : 3��1|
	* @retval E_INVALIDARG : 2?��y��?����
	* @retval E_OUTOFMEMORY : ?����?2?��?
	* @retval E_FAIL : ����㨹
	* 
	*/
	HRESULT OnStartGB28181FileTransmitting(WPARAM wParam, LPARAM lParam);


	/**
	* @brief ����?1GB28181  Pin��?��?��������???t��y?Y
	* @param [in] dwPinID : Pin ID
	* @retval S_OK : 3��1|
	* @retval E_INVALIDARG : 2?��y��?����
	* @retval E_OUTOFMEMORY : ?����?2?��?
	* @retval E_FAIL : ����㨹
	* 
	*/
	HRESULT OnStopGB28181FileTransmitting(WPARAM wParam, LPARAM lParam);


	HRESULT OnStartRealtimeTransmitting(WPARAM wParam, LPARAM lParam);
	
	HRESULT OnStopRealtimeTransmitting(WPARAM wParam, LPARAM lParam);
	

	/**
	* @brief ��ȡĳ��Pin������ʷ�ļ����ݵ�״̬
	* @param [in] dwPinID : Pin ID��ȡֵΪ0��1
	* @param [out] pdwStatus : ����״̬��ָ�룬����ǿգ���ش���ֵ��Χ��
	* - H264HDD_HISTORY_TRANSMITTING_NOTSTARTED,
	* - H264HDD_HISTORY_TRANSMITTING_ONGOING,	
	* - H264HDD_HISTORY_TRANSMITTING_FINISHED,
	* @retval S_OK : �ɹ�
	* @retval E_INVALIDARG : �����Ƿ�
	* @retval E_OUTOFMEMORY : �ڴ治��
	* @retval E_FAIL : ʧ��
	* 
	*/
	HRESULT OnGetHistoryFileTransmittingStatus(WPARAM wParam, LPARAM lParam);

	
	HRESULT OnTriggerVideoSaving(WPARAM wParam, LPARAM lParam);

	
	HRESULT SendFrameToGB28181(const DWORD dwPinID, CSWImage * pFrame);
	

	SW_BEGIN_MESSAGE_MAP(CSWH264HDDTransformFilter, CSWMessage)
		SW_MESSAGE_HANDLER(MSG_H264HDD_FILTER_SET_TRANSMITTING_TYPE, OnSetTransmittingType)
		SW_MESSAGE_HANDLER(MSG_H264HDD_FILTER_HISTORY_FILE_TRANSMITTING_START, OnStartHistoryFileTransmitting)
		SW_MESSAGE_HANDLER(MSG_H264HDD_FILTER_HISTORY_FILE_TRANSMITTING_STOP, OnStopHistoryFileTransmitting)
		SW_MESSAGE_HANDLER(MSG_H264HDD_FILTER_REALTIME_TRANSMITTING_START, OnStartRealtimeTransmitting)
		SW_MESSAGE_HANDLER(MSG_H264HDD_FILTER_REALTIME_TRANSMITTING_STOP, OnStopRealtimeTransmitting)
		SW_MESSAGE_HANDLER(MSG_H264HDD_FILTER_GET_HISTORY_FILE_TRANSMITTING_STATUS, OnGetHistoryFileTransmittingStatus)
		SW_MESSAGE_HANDLER(MSG_H264HDD_FILTER_TRIGGER_VIDEO_SAVING, OnTriggerVideoSaving)
		SW_MESSAGE_HANDLER(MSG_H264HDD_FILTER_GB28181_FILE_TRANSMITTING_START, OnStartGB28181FileTransmitting)
		SW_MESSAGE_HANDLER(MSG_H264HDD_FILTER_GB28181_FILE_TRANSMITTING_STOP, OnStopGB28181FileTransmitting)
	SW_END_MESSAGE_MAP();
	
protected:
    HRESULT OnGB28181VideoSavingEnable (PVOID pvBuffer, INT iSize);
    HRESULT OnGB28181VideoSendScale(PVOID pvBuffer, INT iSize);
 
    SW_BEGIN_REMOTE_MESSAGE_MAP(CSWH264HDDTransformFilter, CSWMessage)
        SW_REMOTE_MESSAGE_HANDLER(MSG_H264HDD_FILTER_GB28181_VIDEOSAVING_ENABLE, OnGB28181VideoSavingEnable)
        SW_REMOTE_MESSAGE_HANDLER(MSG_H264HDD_FILTER_GB28181_VIDEOSEND_SCALE, OnGB28181VideoSendScale)
    SW_END_REMOTE_MESSAGE_MAP();

private:

	BOOL				m_fInited;
	
	//CSWList<BYTE*>		m_lstFrameBuf;	/// ֡����
	CSWList<CSWImage*>		m_lstFrame;	/// ֡����
	
	CSWList<CSWMemory*>	m_lstFile;		/// �ļ��������	
	//CSWMutex*			m_pFileLock;	/// �ļ����������	
	CSWSemaphore*			m_pFileLock;	/// �ļ����������	
	DWORD				m_dwSaveQueueSize;
	CSWSemaphore*		m_pSemaSaveQueueSync;

	INT					m_iSendInterval;
	INT					m_iSendPauseTime;
	INT					m_iRecordSendInterval;

	CSWMemoryFactory * 	m_pcMemoryFactory;

	INT					m_iSaveVideoType;
	CSWList<DWORD>		m_lstTriggerTick;
	CSWSemaphore*       m_pcTickLock;	/// �����洢ʱ�������	
	
	BOOL m_fGBVideoSaveCtlEnable;
	BOOL m_fGBVideoSaveRecord;
	int m_iGBVideoSkipBlock;    // ¼��ط��Ϸţ����������ݿ���
	//CSWFile* m_VideoRecordList;
	INT m_iIsGB28181Saveing;
	CSWDateTime m_TempEndTime;
	BOOL				m_fGB28181Backward;
};

REGISTER_CLASS(CSWH264HDDTransformFilter)
	
#endif //__SW_H264HDD_TRANSFORM_FILTER_H__


