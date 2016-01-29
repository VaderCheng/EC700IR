/**
* @file SWHDDTransformFilter.h 
* @brief ����ģ��
* 
* @copyright Signalway All Rights Reserved
* @author zhouy
* @date 2013-03-22
* @version 1.0
*/


#ifndef __SW_HDD_TRANSFORM_FILTER_H__
#define __SW_HDD_TRANSFORM_FILTER_H__

#include "SWBaseFilter.h"


#include "SWList.h"
#include "BigFile.h"
#include "SWMessage.h"


typedef struct __IMAGE_FILE_INFO
{
#define MAX_FRAME_NAME_SIZE 256

	SW_COMPONENT_IMAGE sComponent;
	DWORD dwFrameNo;							// 图像帧编�?
	DWORD dwRefTimeMS;				        // 图像取得时的系统时标.毫秒.
	CHAR szFrameName[MAX_FRAME_NAME_SIZE];	// 图像帧文件名�?
	BOOL fIsCaptureImage;						// 是否是抓拍帧�?
	SWPA_TIME	sTimeStamp;                  //

	__IMAGE_FILE_INFO()
	{
		swpa_memset(this, 0, sizeof(*this));
	}
		
}_IMAGE_FILE_INFO;





/**
* @brief ����ģ�����
*
*
* �������ܣ�һ���Ǳ������ݣ���һ���Ƕ�ȡ���ݡ�
* 
*/
class CSWHDDTransformFilter : public CSWBaseFilter
{
	CLASSINFO(CSWHDDTransformFilter, CSWBaseFilter)
		
public:
	CSWHDDTransformFilter(const DWORD dwInCount, const DWORD dwOutCount);
	
	virtual ~CSWHDDTransformFilter();

	HRESULT Initialize(const CHAR * szDirecory, const LONGLONG llTotalSize, const INT iFileSize, const BOOL
fIsResult);
	

	
protected:

	virtual HRESULT Run();
	virtual HRESULT Stop();

	
	HRESULT SetTransmittingType(const DWORD dwPinID, const DWORD dwOutType);

	HRESULT StartHistoryFileTransmitting(const DWORD dwPinID, const SWPA_DATETIME_TM * psTimeBegin, const SWPA_DATETIME_TM * psTimeEnd, const DWORD dwCarID);
	
	HRESULT StopHistoryFileTransmitting(const DWORD dwPinID);

	HRESULT StartRealtimeTransmitting(const DWORD dwPinID);

	HRESULT StopRealtimeTransmitting(const DWORD dwPinID);

	HRESULT GetHistoryFileTransmittingStatus(const DWORD dwPinID, DWORD* pdwStatus);

protected:

	/**
	* @brief ��ȡͼƬ��ʱ���������ͼƬ��refTimeת��ΪSWPA_TIME����
	* @param [in] pImage : ͼƬ����ָ��
	* @param [out] pTimeStamp : ����ת�����SWPA_TIME����ʱ��
	* @retval S_OK : �ɹ�
	* @retval E_INVALIDARG : �����Ƿ�
	* @retval E_FAIL : ʧ��
	*/
	HRESULT GetImageTimeStamp(CSWImage * pImage, SWPA_TIME* pTimeStamp);
	
	CSWImage * CreateImageFromBuffer(const PBYTE pbBuf, const DWORD dwBufSize);	
		
	virtual HRESULT OnHistoryFileSaving(VOID) {SW_TRACE_DEBUG("Err: Not Implemented in Base Class\n"); return E_NOTIMPL;};
	
	virtual HRESULT OnHistoryFileTransmitting(const DWORD dwPinID) {SW_TRACE_DEBUG("Err: Not Implemented in Base Class\n"); return E_NOTIMPL;};

	static VOID* OnHistoryFileSavingProxy(VOID* pParam);
	
	static VOID* OnHistoryFileTransmittingProxy(VOID* pParam);

	static HRESULT UpdateCapacity(const DWORD dwSize);

	static BOOL CheckCurrentCapacity();
	
	//HRESULT ParseTime(SWPA_DATETIME_TM& myTime,	const CHAR * szTimeString);

	HRESULT SetSaveType(const INT iSaveType)
	{
		if (0 != iSaveType && 1 != iSaveType)
		{
			return E_INVALIDARG;
		}
		
		m_iSaveType = iSaveType;
		return S_OK;
	}

	INT GetSaveType(VOID)
	{
		return m_iSaveType;
	}


	HRESULT SetSaveHistoryFlag(const BOOL fEnable)
	{		
		m_fSaveHistoryFlag = fEnable;
		return S_OK;
	}

	INT GetSaveHistoryFlag(VOID)
	{
		return m_fSaveHistoryFlag;
	}

	virtual FILTER_STATE GetState();

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

	CSWString GetStorageStatus(VOID)
	{
		CSWString str = "";

		if (m_fInited)
		{
			str.Format("%lldMB / %lldMB",
				(LONGLONG)m_pBigFile->GetSingleFileSize() * (LONGLONG)m_pBigFile->GetAvailableCount() / 1024 / 1024,
				(LONGLONG)m_pBigFile->GetSingleFileSize() * (LONGLONG)m_pBigFile->GetTotalCount() / 1024 / 1024);
		}
		
		return str;
	}

	
	virtual HRESULT ReportStatus(LPCSTR szInfo=NULL){return E_NOTIMPL;};
	virtual HRESULT ClearBuffer();
	
	HRESULT StartSaving();

protected:
	
	CBigFile*			m_pBigFile;
	
	DWORD*				m_dwHistoryTransmittingStatus;//[HDD_TRANSFORM_OUT_COUNT];
	
	BOOL*				m_fHistoryTransmitting;//[HDD_TRANSFORM_OUT_COUNT];
	SWPA_DATETIME_TM*   m_tHistoryBeginTime;//[HDD_TRANSFORM_OUT_COUNT];
	SWPA_DATETIME_TM*   m_tHistoryEndTime;//[HDD_TRANSFORM_OUT_COUNT];
	DWORD*				m_dwCarID;//[HDD_TRANSFORM_OUT_COUNT];
	BOOL*				m_fSendThreadStarted;//[HDD_TRANSFORM_OUT_COUNT];	
	DWORD*				m_dwOutType;//[HDD_TRANSFORM_OUT_COUNT];

	BOOL				m_fHistoryFileSaveDisabled;
	
	DWORD       m_dwLastTime;

private:

	BOOL				m_fInited;
	//CSWList<CSWRecord*>	m_lstFile;
	//CSWList<VOID*>		m_lstFrameBuf;
	//CSWMutex*			m_pFileMutex;
	
	CSWThread*			m_pSaveThread;
	CSWThread**			m_pSendThread;//[HDD_TRANSFORM_OUT_COUNT];

	
	INT					m_iSaveType;
	
	INT					m_iSendInterval;

	BOOL				m_fSaveHistoryFlag;

	static DWORD		s_dwCapacity;
	static CSWMutex		s_cMutexCapacity;
};

	
#endif //__SW_HDD_TRANSFORM_FILTER_H__


