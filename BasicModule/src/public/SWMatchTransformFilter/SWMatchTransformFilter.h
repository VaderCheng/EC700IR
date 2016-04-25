#ifndef __SW_MATCH_TRANSFORM_FILTER_H__
#define __SW_MATCH_TRANSFORM_FILTER_H__
#include "SWBaseFilter.h"
#include "SWBaseDevice.h"
#include "SWCarLeft.h"
#include "SWMatchTansformDataStruct.h"
#define NOT_MATCH_TIME 0xFFFFFFFF
#define BEGIN_MAP_MATCH_FUNCTION()\
	typedef bool (CSWMatchTransformFilter::*pFunMatch)(SIGNAL_PARAM *pSignal, CARLEFT_MATCH_INFO *pMatchInfo);\
	pFunMatch GetFunction(int type)\
	{\
		switch(type)\
		{
#define MAP_MATCH_FUNCTION(id,func)  case id : return &CSWMatchTransformFilter::func; break;
#define END_MAP_MATCH_FUNCTION()\
			default : return NULL;\
		}\
	}

// ������Ϣ�ṹ��
typedef struct tag_CarLeftMatchInfo
{
	CSWCarLeft* pCarLeft;
	DWORD dwInputTime;
	DWORD dwMatchImageTime[3];
	DWORD dwMatchSpeedTime;
	DWORD dwOutputCondition; // 0:����ʶ����;1:���źų�ʱ�Ľ����	
	
	tag_CarLeftMatchInfo()
	{
		pCarLeft = NULL;
		dwInputTime = CSWDateTime::GetSystemTick();
		for(int i = 0; i < 3; i++)
		{
			dwMatchImageTime[i] = NOT_MATCH_TIME;
		}
		dwMatchSpeedTime = NOT_MATCH_TIME;
		dwOutputCondition = 0;
	}
}
CARLEFT_MATCH_INFO;

class CSWMatchTransformFilter : public CSWBaseFilter, CSWMessage
{
	CLASSINFO(CSWMatchTransformFilter, CSWBaseFilter)
public:
	CSWMatchTransformFilter();
	virtual ~CSWMatchTransformFilter();
protected:	
	virtual HRESULT Initialize(PVOID pvParam1, PVOID pvParam2,BOOL fRadarTrigger);
	virtual HRESULT Run();
	virtual HRESULT Stop();
	virtual HRESULT Receive(CSWObject* obj);
protected:
  /**
   *@brief �ⲿ�豸�źŴ����߳�
   */
	static void* OnProcessIPTSignal(void* pvParam);
	/**
	 *@brief �ź�ƥ���߳�
	 */
	static void* OnMatchSignal(void* pvParam);

	/**
	 *@brief �������߳�
	 */
	static void* OnOutputProxy(void* pvParam);
	HRESULT OnOutput();

	/**
   *@brief �������ڲ��Է���
   */
	HRESULT OnStartCOMTest(WPARAM wParam, LPARAM lParam);

	/**
   *@brief ֹͣ���ڲ��Է���
   */
	HRESULT OnStopCOMTest(WPARAM wParam, LPARAM lParam);

	/**
   *@brief �״�����źſ���
   */
	HRESULT OnRadarCtrl(WPARAM wParam, LPARAM lParam);

		/**
		   @brief ���ܿ�ʹ�ܿ���
		   @param [in] 1ʹ�� 0��ʹ��
		 */
	HRESULT OnOutputCtrlEnable(WPARAM wParam, LPARAM lParam);

	//���µ�ǰ���ڵ����д����һ�û�н������ٵ�
	HRESULT OnUpdateAllCarTrigger(WPARAM wParam, LPARAM lParam);

protected:
	/**
	 *@brief �ź��Ƿ����ͼ��
	 */
	bool SignalIncludeImage(SIGNAL_PARAM* pSignal, bool fCapture = true);
	
	/**
	 *@brief ��ͼƬ�ź���ӵ�����
	 */
	bool AppendImage(CSWImage *pImage);
	
	/**
	 *@brief ���ٶ��ź���ӵ�����
	 */
	bool AppendSpeed(DWORD dwRoadID, DWORD dwTime, DWORD dwSpeed, DWORD dwSpeedType,DWORD dwDirection);
	
	/**
	 *@brief �ٶ�ƥ�亯��
	 */
	bool MatchSpeed(SIGNAL_PARAM *pSignal, CARLEFT_MATCH_INFO *pMatchInfo);
	/**
	 *@brief ץ��ͼƥ�亯��
	 */
	bool MatchImage(SIGNAL_PARAM *pSignal, CARLEFT_MATCH_INFO *pMatchInfo);
	/**
	 *@brief ����ƥ�亯��
	 */
	bool MatchTrigger(SIGNAL_PARAM *pSignal, CARLEFT_MATCH_INFO *pMatchInfo);
	
	static VOID OnEvent(PVOID pvParam, CSWBaseDevice *pDevice, CSWBaseDevice::DEVICE_TYPE type, DWORD dwTime, PDWORD pdwValue);
	
	VOID Close(VOID);

	BOOL CheckTriggerIndexTracker(INT iTriggerIndex);
	
	/**
	 *@brief OLE��ʼ��
	 */
	SW_BEGIN_DISP_MAP(CSWMatchTransformFilter,CSWBaseFilter)
		SW_DISP_METHOD(Initialize, 3)
	SW_END_DISP_MAP()
	
	/**
	 *@brief ƥ�亯��ӳ���
	 */

	BEGIN_MAP_MATCH_FUNCTION()
		MAP_MATCH_FUNCTION(CSWBaseDevice::SPEED, MatchSpeed)
		MAP_MATCH_FUNCTION(CSWBaseDevice::IMAGE, MatchImage)
		MAP_MATCH_FUNCTION(CSWBaseDevice::TRIGGER, MatchTrigger)
	END_MAP_MATCH_FUNCTION()

	//��Ϣӳ���
	SW_BEGIN_MESSAGE_MAP(CSWMatchTransformFilter, CSWMessage)
	SW_MESSAGE_HANDLER(MSG_COM_TEST_ENABLE, OnStartCOMTest)
 
	SW_MESSAGE_HANDLER(MSG_COM_OUTPUT_CTRL_ENABLE, OnOutputCtrlEnable)
 
	SW_MESSAGE_HANDLER(MSG_COM_TEST_DISABLE, OnStopCOMTest)

	SW_MESSAGE_HANDLER(MSG_SET_RADARENABLE, OnRadarCtrl)

	SW_MESSAGE_HANDLER(MSG_UPDATE_ALL_TRIGGER, OnUpdateAllCarTrigger)
	SW_END_MESSAGE_MAP()
private:
	
	BOOL        m_fInitialize;
	MATCHPARAM  m_cParam;
	DEVICEPARAM m_cDevParam[2];
	CSWSemaphore m_semMatch;
	CSWMutex  m_cMutex;
	CSWBaseDevice *m_pDevice[2];
	CSWThread m_matchThread;

	CSWBaseDevice *m_pComTestDevic;		//���ڲ���
	INT m_iTESTINGCOMID;				//���ڲ��ԵĴ��ں�

	BOOL m_fRadarTrigger;	//�״ﴥ��ץ��

	// zhaopy
	CSWThread m_cOutputThread;
	CSWList<CSWCarLeft*, 6> m_cOutputList;
	CSWMutex  m_cOutputMutex;
	CSWSemaphore m_semOutput;

	CSWList<CARLEFT_MATCH_INFO*,6> m_cCarLeftList;
	CSWMutex m_cCarLeftMutex;
	CSWMutex m_cImportMutex;

	TRACKER_CFG_PARAM* m_pTrackerCfg;

	INT m_rgiAllCarTrigger[MAX_EVENT_COUNT];
	CSWMutex  m_cAllCarTriggerMutex;

	BOOL m_Radar;

//    INT m_iOutputThreadRestartTime;     // �������߳���������
};
REGISTER_CLASS(CSWMatchTransformFilter)
#endif

