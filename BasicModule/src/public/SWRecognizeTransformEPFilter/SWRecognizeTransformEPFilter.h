/**
* @file SWRecognizeTransformEPFilter.h 
* @brief ʶ��Filter
* @copyright Signalway All Rights Reserved
* @author zhaopy
* @date 2013-03-20
* @version 1.0
*/

#ifndef __SW_RECOGNIZE__TRANSFORM_EP_FILTER_H__
#define __SW_RECOGNIZE__TRANSFORM_EP_FILTER_H__

#include "SWBaseFilter.h"
#include "SWMessage.h"
#include "SWCarLeft.h"
#include "SWPosImage.h"
#include "SWCameraDataPDU.h"
#include "SWBaseLinkCtrl.h"
#include "EPAppDetCtrl.h"

class CSWRecognizeTransformEPFilter : public CSWBaseFilter, CSWMessage
{
	CLASSINFO(CSWRecognizeTransformEPFilter, CSWBaseFilter)
public:
	CSWRecognizeTransformEPFilter();
	virtual ~CSWRecognizeTransformEPFilter();

	/**
   *@brief ʶ��ģ���ʼ����
   */
	virtual HRESULT Initialize(INT iGlobalParamIndex, INT nLastLightType,INT nLastPulseLevel, INT nLastCplStatus, PVOID pvParam);
	virtual HRESULT RegisterCallBackFunction(PVOID OnDSPAlarm, PVOID pvParam);
	virtual HRESULT Run();
	virtual HRESULT Stop();

	static VOID OnProcessProxy(PVOID pvParam);
    static VOID OnProcessSyncProxy(PVOID pvParam);
public:
	/**
   *@brief ISWPlateRecognitionEvent �ص��ӿ�
   */
	virtual HRESULT CarArriveEvent(CARARRIVE_INFO_STRUCT *pCarArriveInfo);
	virtual HRESULT CarLeftEvent(CARLEFT_INFO_STRUCT *pCarLeftInfo);
    virtual HRESULT GB28181Alarm(LPCSTR szMsg);

protected:
	/**
   *@brief ����Receive�ӿڣ�����CSWImage����
   */
	virtual HRESULT Receive(CSWObject* obj);	

	/**
   *@brief �ṩͼƬʶ������
   */
	HRESULT OnRecognizePhoto(WPARAM wParam, LPARAM lParam);

	/**
   *@brief ����ʶ�𿪹�
   */
	HRESULT OnRecognizeGetJPEG(WPARAM wParam, LPARAM lParam);

	/**
   *@brief �����������
   */
	HRESULT OnRecognizeOutPutDebug(WPARAM wParam, LPARAM lParam);
	
	/**
	 *@brief ���ô���ʹ��
	 */
	HRESULT OnTriggerEnable(WPARAM wParam, LPARAM lParam);
	
	/**
	 *@brief IO���̵ƴ����¼�
	 */
	HRESULT OnIOEvent(WPARAM wParam, LPARAM lParam);

	/**
	 *@brief ��̬�޸�ʶ�����
	 */
	HRESULT OnModifyParam(WPARAM wParam, LPARAM lParam);

	/**
	 *@brief ��ȡ����ͳ��
	 */
	HRESULT OnGetCarLeftCount(WPARAM wParam, LPARAM lParam);

	/**
   *@brief ʶ�����߳�
   */
	HRESULT OnProcess();

    /**
    *@brief �첽ʶ�����߳�
    */
    HRESULT OnProcessSync();
    /*
	 *@brief �����Ƿ�������б�־
	 */
	HRESULT SetReverseRunFilterFlag(const BOOL fFilter)
    {
    	m_fReverseRunFilterFlag = fFilter;
    }

	/*
	 *@brief ����GB28181ʹ��
	 */
	HRESULT SetGB28181Enable(const BOOL fEnable)
   {
		m_fEnableGB28181 = fEnable;
   }


	/**
	 *@brief ��ȡ�Ƿ�������б�־
	 */
	BOOL GetReverseRunFilterFlag()
    {
    	return m_fReverseRunFilterFlag;
    }
	
protected:
	//�Զ���ӳ���
	SW_BEGIN_DISP_MAP(CSWRecognizeTransformEPFilter, CSWBaseFilter)
		SW_DISP_METHOD(Initialize, 5)
		SW_DISP_METHOD(RegisterCallBackFunction, 2)
		SW_DISP_METHOD(SetReverseRunFilterFlag, 1)
		SW_DISP_METHOD(SetGB28181Enable, 1)
	SW_END_DISP_MAP()
	//��Ϣӳ���
	SW_BEGIN_MESSAGE_MAP(CSWRecognizeTransformEPFilter, CSWMessage)
		SW_MESSAGE_HANDLER(MSG_RECOGNIZE_PHOTO, OnRecognizePhoto)
		SW_MESSAGE_HANDLER(MSG_RECOGNIZE_GETJPEG, OnRecognizeGetJPEG)
		SW_MESSAGE_HANDLER(MSG_RECOGNIZE_OUTPUTDEBUG, OnRecognizeOutPutDebug)
		SW_MESSAGE_HANDLER(MSG_RECOGNIZE_ENABLE_TRIGGER, OnTriggerEnable)
		SW_MESSAGE_HANDLER(MSG_RECOGNIZE_IOEVENT, OnIOEvent)
		SW_MESSAGE_HANDLER(MSG_RECOGNIZE_MODIFY_PARAM, OnModifyParam)
		SW_MESSAGE_HANDLER(MSG_RECOGNIZE_GET_CARLEFT_COUNT, OnGetCarLeftCount)
	SW_END_MESSAGE_MAP()

private:
	VOID Clear();

public:
	static const INT MAX_IMAGE_COUNT = 3;

private:
	TRACKER_CFG_PARAM *m_pTrackerCfg;
	BOOL m_fSendJPEG;
	BOOL m_fSendDebug;
	BOOL m_fInitialized;
	INT  m_iCarArriveTriggerType;
	INT  m_iIOLevel;

	CSWList<CSWImage*> m_lstImage;		// ͼƬ����
	CSWMutex m_cMutexImage;             // �����ٽ���
	CSWThread* m_pThread;				// ʶ���߳�
	CSWSemaphore m_cSemImage;			

	BOOL m_fReverseRunFilterFlag;
	DWORD m_dwPlateCount;		//����ͳ��

	BOOL m_fEnableGB28181;
    // ganzz
    CSWThread* m_pProcQueueThread;              // �����ж��߳�
    INT m_nMaxProcQueueLen;
    struct PROC_QUEUE_ELEM
    {
        CSWImage* pImage;
        void* pData;    // ��������ָ��
    };

    CSWSemaphore m_cSemProcQueue;

    CEPAppDetCtrl m_sEPCtrl;

    // zhaopy
    CSWMutex m_cMutexEPList;
    CSWList<PROC_QUEUE_ELEM> m_lstEPList;
    CSWSemaphore m_semEP;

    CSWMutex m_cMutexOutput;

    BOOL m_fModifyParam;
};
REGISTER_CLASS(CSWRecognizeTransformEPFilter)
#endif

