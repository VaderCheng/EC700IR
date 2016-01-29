/**
* @file SWJPEGCachTransformFilter.h
* @brief JPEGͼƬ����Filter
* @copyright Signalway All Rights Reserved
* @author quanjh
* @date 2013-03-20
* @version 1.0
*/

#ifndef __SW_JPEGCACH__TRANSFORM_FILTER_H__
#define __SW_JPEGCACH__TRANSFORM_FILTER_H__

#include "SWBaseFilter.h"
#include "SWMessage.h"
#include "SWCarLeft.h"
#include "SWPosImage.h"
#include "SWCameraDataPDU.h"
#include "SWBaseLinkCtrl.h"

class CSWJPEGCachTransformFilter : public CSWBaseFilter, CSWMessage
{
    CLASSINFO(CSWJPEGCachTransformFilter, CSWBaseFilter)
public:
    CSWJPEGCachTransformFilter();
    virtual ~CSWJPEGCachTransformFilter();

	/**
   *@brief ʶ��ģ���ʼ����
   */
	virtual HRESULT Initialize(INT iGlobalParamIndex, INT nLastLightType,INT nLastPulseLevel, INT nLastCplStatus, PVOID pvParam);
	virtual HRESULT RegisterCallBackFunction(PVOID OnDSPAlarm, PVOID pvParam);
	virtual HRESULT Run();
	virtual HRESULT Stop();

	static VOID OnProcessProxy(PVOID pvParam);

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
    SW_BEGIN_DISP_MAP(CSWJPEGCachTransformFilter, CSWBaseFilter)
		SW_DISP_METHOD(Initialize, 5)
		SW_DISP_METHOD(RegisterCallBackFunction, 2)
		SW_DISP_METHOD(SetReverseRunFilterFlag, 1)
	SW_END_DISP_MAP()
	//��Ϣӳ���
    SW_BEGIN_MESSAGE_MAP(CSWJPEGCachTransformFilter, CSWMessage)
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
    CSWThread* m_pThread;				// �����߳�
	CSWSemaphore m_cSemImage;			

	BOOL m_fReverseRunFilterFlag;
};
REGISTER_CLASS(CSWJPEGCachTransformFilter)
#endif

