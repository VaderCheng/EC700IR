/**
* @file SWRecognizeTransformFilter.h 
* @brief ʶ��Filter
* @copyright Signalway All Rights Reserved
* @author zhaopy
* @date 2013-03-20
* @version 1.0
*/

#ifndef __SW_RECOGNIZE__TRANSFORM_TOOLGATE_FILTER_H__
#define __SW_RECOGNIZE__TRANSFORM_TOOLGATE_FILTER_H__

#include "SWBaseFilter.h"
#include "SWMessage.h"
#include "SWCarLeft.h"
#include "SWPosImage.h"
#include "SWCameraDataPDU.h"
#include "SWBaseLinkCtrl.h"
#include "TLAppDetCtrl.h"
/**
 *2014-3-24 ���շ�վfilter��ʱû���õ�����filterΪ��֤���㷨����ӡ��ɸ���־��д������Դ�޸ġ�
 *������㷨�ú����ʹ�ø�filter
*/
class CSWRecognizeTransformTollGateFilter : public CSWBaseFilter, CSWMessage
{
	CLASSINFO(CSWRecognizeTransformTollGateFilter, CSWBaseFilter)
public:
	CSWRecognizeTransformTollGateFilter();
	virtual ~CSWRecognizeTransformTollGateFilter();

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

	/**
	 *@brief ��ȡ�Ƿ�������б�־
	 */
	BOOL GetReverseRunFilterFlag()
    {
    	return m_fReverseRunFilterFlag;
    }
	
protected:
	//�Զ���ӳ���
	SW_BEGIN_DISP_MAP(CSWRecognizeTransformTollGateFilter, CSWBaseFilter)
		SW_DISP_METHOD(Initialize, 5)
		SW_DISP_METHOD(RegisterCallBackFunction, 2)
		SW_DISP_METHOD(SetReverseRunFilterFlag, 1)
	SW_END_DISP_MAP()
	//��Ϣӳ���
	SW_BEGIN_MESSAGE_MAP(CSWRecognizeTransformTollGateFilter, CSWMessage)
		SW_MESSAGE_HANDLER(MSG_RECOGNIZE_PHOTO, OnRecognizePhoto)
		SW_MESSAGE_HANDLER(MSG_RECOGNIZE_GETJPEG, OnRecognizeGetJPEG)
		SW_MESSAGE_HANDLER(MSG_RECOGNIZE_OUTPUTDEBUG, OnRecognizeOutPutDebug)
		SW_MESSAGE_HANDLER(MSG_RECOGNIZE_ENABLE_TRIGGER, OnTriggerEnable)
		SW_MESSAGE_HANDLER(MSG_RECOGNIZE_IOEVENT, OnIOEvent)
		SW_MESSAGE_HANDLER(MSG_RECOGNIZE_MODIFY_PARAM, OnModifyParam)
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
    // ganzz
    CSWThread* m_pProcQueueThread;              // �����ж��߳�
    struct PROC_QUEUE_ELEM
    {
        CSWImage* pImage;
        void* pData;    // ��������ָ��
    };
    static const INT MAX_RPOC_QUEUE_LEN = 2;
    CSWSemaphore m_cSemProcQueue;

    CTLAppDetCtrl m_sTLCtrl;

    // zhaopy
    CSWMutex m_cMutexEPList;
    CSWList<PROC_QUEUE_ELEM> m_lstEPList;
    CSWSemaphore m_semEP;

    CSWMutex m_cMutexOutput;

    BOOL m_fModifyParam;
};
REGISTER_CLASS(CSWRecognizeTransformTollGateFilter)
#endif

