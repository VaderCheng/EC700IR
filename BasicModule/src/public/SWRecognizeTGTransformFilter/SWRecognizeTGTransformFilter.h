/**
* @file SWRecognizeTGTransformFilter.h 
* @brief ʶ��Filter
* @copyright Signalway All Rights Reserved
* @author zhaopy
* @date 2013-03-20
* @version 1.0
*/

#ifndef __SW_RECOGNIZE_TG_TRANSFORM_FILTER_H__
#define __SW_RECOGNIZE_TG_TRANSFORM_FILTER_H__

#include "SWBaseFilter.h"
#include "SWMessage.h"
#include "SWCarLeft.h"
#include "SWPosImage.h"
#include "SWCameraDataPDU.h"
#include "SWBaseLinkCtrl.h"
#include "AppDetCtrl.h"

class CSWRecognizeTGTransformFilter : public CSWBaseFilter, CSWMessage
{
	CLASSINFO(CSWRecognizeTGTransformFilter, CSWBaseFilter)
public:
	CSWRecognizeTGTransformFilter();
	virtual ~CSWRecognizeTGTransformFilter();

	/**
   *@brief ʶ��ģ���ʼ����
   */
	virtual HRESULT Initialize(INT iGlobalParamIndex, 
		INT nLastLightType,INT nLastPulseLevel, INT nLastCplStatus, 
		PVOID pvParam, INT iMinPlateLight, INT iMaxPlateLight,BOOL fRecognizeEnable);
	
	virtual HRESULT RegisterCallBackFunction(PVOID OnDSPAlarm, PVOID pvParam);
	virtual HRESULT Run();
	virtual HRESULT Stop();
	//ARM�˸����߳�
	static VOID OnProcessProxy(PVOID pvParam);
	//DSP��ʶ���߳�
	static VOID OnProcessSyncProxy(PVOID pvParam);
	//����ƥ���߳�
	static VOID OnMatchImageProxy(PVOID pvParam);
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
     *@brief ��������״̬�任
     *@brief wParam: 0��ʾ���죬1��ʾ����
     */
	HRESULT OnEnvPeriodChanged(WPARAM wParam, LPARAM lParam);

    /**
     *@brief ��ȡ DSP ����״̬
     *@brief lParam: ��0���죬1����2���ϣ�
     */
    HRESULT OnGetDspEnvLightType(WPARAM wParam, LPARAM lParam);

	/**
   *@brief ʶ�����߳�
   */
	HRESULT OnProcess();

	/**
	*@brief ʶ��DSP�ٴ����߳�
	*/
	HRESULT OnProcessSync();

	/**
	*@brief ����ͼƬƥ���߳�
	*/
	HRESULT OnMatchImage();
	
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

	/**
	   @brief ��ʼ��FPGA������Ƶ�����
	 */
//	HRESULT OnInitFpgaFlash();
	
protected:
	//�Զ���ӳ���
	SW_BEGIN_DISP_MAP(CSWRecognizeTGTransformFilter, CSWBaseFilter)
		SW_DISP_METHOD(Initialize, 8)
		SW_DISP_METHOD(RegisterCallBackFunction, 2)
		SW_DISP_METHOD(SetReverseRunFilterFlag, 1)
	SW_END_DISP_MAP()
	//��Ϣӳ���
	SW_BEGIN_MESSAGE_MAP(CSWRecognizeTGTransformFilter, CSWMessage)
		SW_MESSAGE_HANDLER(MSG_RECOGNIZE_PHOTO, OnRecognizePhoto)
		SW_MESSAGE_HANDLER(MSG_RECOGNIZE_GETJPEG, OnRecognizeGetJPEG)
		SW_MESSAGE_HANDLER(MSG_RECOGNIZE_OUTPUTDEBUG, OnRecognizeOutPutDebug)
		SW_MESSAGE_HANDLER(MSG_RECOGNIZE_ENABLE_TRIGGER, OnTriggerEnable)
		SW_MESSAGE_HANDLER(MSG_RECOGNIZE_IOEVENT, OnIOEvent)
		SW_MESSAGE_HANDLER(MSG_RECOGNIZE_MODIFY_PARAM, OnModifyParam)
		SW_MESSAGE_HANDLER(MSG_RECOGNIZE_GET_CARLEFT_COUNT, OnGetCarLeftCount)
		SW_MESSAGE_HANDLER(MSG_RECOGNIZE_ENVPERIOD_CHANGED, OnEnvPeriodChanged)
        SW_MESSAGE_HANDLER(MSG_RECOGNIZE_GET_DSP_ENV_LIGHT_TYPE, OnGetDspEnvLightType)
	SW_END_MESSAGE_MAP()

private:
	VOID Clear();

public:
	static const INT MAX_IMAGE_COUNT = 1;
	static const INT MAX_DECT_COUNT = 1;
	static const INT MAX_MATCH_COUNT = 5;
private:
	BOOL m_fRecognizeEnable;	//ʶ�𿪹�
	
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
	
	CSWThread* m_pProcQueueThread;		//ARM��ʶ���������߳�

	struct PROC_QUEUE_ELEM		//ARM�˴���������
	{
		CSWImage* pImage;
		PVOID pData;
	};
    CSWMutex m_cMutexProcQueue;
    CSWList<PROC_QUEUE_ELEM> m_lstProcQueueList;
    CSWSemaphore m_cSemProcQueue;

	CSWThread* m_pMatchThread;			//ƥ���߳�
	CSWMutex m_cMutexMatch;
    CSWList<PROC_QUEUE_ELEM> m_lstMatchImage;
	CSWSemaphore m_cSemMatch;			

	swTgApp::CAppDetCtrl m_cAlgDetCtrl;
    BOOL m_fModifyParam;
	int m_nEnvLightType;

//#define PLATE_COUNT_MAX 5	//���ͳ�Ƴ��Ƹ���
#define PULSE_WIDTH_LEVEL_MAX 8	//�������ȼ�
#define PULSE_WIDTH_LEVEL_MIN 0	//��С����ȼ�

	INT m_iPlateLightCheckCount;// �������
	INT m_iExpectPlateLightMax;	//�����������ֵ
	INT m_iExpectPlateLightMin;	//ϣ��������Сֵ
	
	//���ݳ������Ƚ���LED�������
	INT m_iPlateLightSum;	//�ܳ�������
	INT m_iPlateCount;		//���Ƽ���
	INT m_iPlateLightMax;	//���������
	INT m_iPlateLightMin;	//��С��������
	INT m_iUpStep;			//���ϵ��Ĳ���ֵ�������������Ļ���ֵ�ۼӣ����ڲ����ӿ�
	INT m_iDownStep;		//���µ��Ĳ���ֵ�������������Ļ���ֵ�ۼӣ����ڲ����ӿ�

	//ͬʱ��Ҫ�ο��������ȣ���Ҫ�ǰ���ʱ����Ҫ����0������ʱ����Ϊ����
	INT m_iEnvLightSum;		//�ܻ�������
	INT m_iFrameCount;		//ͳ�ƻ�������֡����
	
	INT m_iPulseWidthLevel;		//��ǰ����ȼ�

	INT AdjustPulseWidth(INT iPlateLight);
	VOID ResetAdjustPulseWidthInfo(INT iDefPulseWidth);

    INT m_nCurEnvPeriod; // ���졢��������״̬��3:day, 2:dusk, 1:night

    INT m_nEnvFromM3;       // ����M3  3:day, 2:dusk, 1:night
};
REGISTER_CLASS(CSWRecognizeTGTransformFilter)
#endif

