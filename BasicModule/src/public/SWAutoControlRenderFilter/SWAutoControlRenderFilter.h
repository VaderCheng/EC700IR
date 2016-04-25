#ifndef __SW_AUTO_CONTROL_RENDER_FILTER_H__
#define __SW_AUTO_CONTROL_RENDER_FILTER_H__
#include "SWBaseFilter.h"
#include "SWMessage.h"

class CSWAutoControlRenderFilter : public CSWBaseFilter, CSWMessage
{
	CLASSINFO(CSWAutoControlRenderFilter, CSWBaseFilter)
public:
	CSWAutoControlRenderFilter();
	virtual ~CSWAutoControlRenderFilter();
	/**
	 *@brief �Ƿ�ʹ��ģ���AGC
	 *@param [in] fEnable       �Ƿ�ʹ�ܴ�ģ��
	 *@param [in] fEnableAGC    �Ƿ�ʹ��AGC
	 *@return �ɹ�����S_OK������ֵΪ�������
	 */
	HRESULT Enable(BOOL fEnable,
	        BOOL fEnableAGC,
	        INT iMinPSD,
	        INT iMaxPSD,
	        INT iEnableCpl,
	        INT iMinPlateY,
	        INT iMaxPlateY);
	/**
	 *@brief ����ÿ���ȼ���AGC���ޡ��ع�ʱ�������
	 *@param [in] iLevel �ȼ�
	 *@param [in] iAGCLimit AGC����
	 *@param [in] iExposureTime �ع�ʱ��
	 *@param [in] iGain ����
	 */
	HRESULT SetLevel(INT iLevel, INT iAGCLimit, INT iExposureTime, INT iGain);
	/**
	 *@brief �����Զ�ץ�Ĳ�����
	 *@param [in] fEnable	�Ƿ�ʹ��
	 *@param [in] iDayShutter �������ץ�Ŀ���
	 *@param [in] iDayGain	  �������ץ������
	 *@param [in] iNightShutter �������ץ�Ŀ���
	 *@param [in] iNightGain	�������ץ������
	 */
	HRESULT SetAutoCaptureParam(BOOL fEnable, INT iDayShutter, INT iDayGain, INT iNightShutter, INT iNightGain);
    HRESULT SetCaptureShutterGain(INT iCaptureShutter, INT iCaptureGain);

protected:
	virtual HRESULT Run();
	virtual HRESULT Stop();	
	virtual HRESULT Receive(CSWObject* obj);	
	static void* OnProcessCameraPDU(void* pvParam);

	// ץ�Ĳ���������
	VOID UpdateCaptureParam(const IMAGE_EXT_INFO& cInfo);
	// ������һ�γ�ʼ��ȷ������Ҫ�������顣
	VOID DoInit();
	
	HRESULT SaveParam(INT iLightType, INT iCplStatus, INT iPluseLevel);

    HRESULT SetNightThresholdArg(INT nNightShutter, INT nNightThreshold, INT nDuskThreshold, INT nMaxAgcShutter);

    HRESULT SetDayNightShutterHOri(INT iDayNightShutterEnable, INT iDayShutterHOri, INT iNightShutterHOri,INT iGainHOri, INT iNightGainHOri, INT iShutterLOri);
protected:
		//�Զ���ӳ���
	SW_BEGIN_DISP_MAP(CSWAutoControlRenderFilter, CSWBaseFilter)
		SW_DISP_METHOD(Enable, 7)
		SW_DISP_METHOD(SetLevel, 4)
		SW_DISP_METHOD(SetAutoCaptureParam, 5)
        SW_DISP_METHOD(SetCaptureShutterGain, 2)
        SW_DISP_METHOD(SetNightThresholdArg, 4)
        SW_DISP_METHOD(SetDayNightShutterHOri, 6)
	SW_END_DISP_MAP()
	
	/**
	 *@breif ��ȡ������ȼ�
	 *@param [in] wParam, INT*�Ĵ�СΪ3��һά����,0:iLightType;1:iCplStatus,2:iPluseLevel
	 *@param [in] lParam, ��
	 *@return �ɹ�����S_OKʧ�ܷ���E_FAIL
	 */
	HRESULT OnReadParam(WPARAM wParam, LPARAM lParam);

	/**
	 *@breif ��ȡ�����ʵʱ�ȼ���Ϣ
	 *@param [in] wParam, ��
	 *@param [in] lParam, INT*�Ĵ�СΪ3��һά����,0:iLightType;1:iCplStatus,2:iPluseLevel
	 *@return �ɹ�����S_OKʧ�ܷ���E_FAIL
	 */
	HRESULT OnGetRealParam(WPARAM wParam, LPARAM lParam);

	/**
	 *@breif ��ȡ��ǰʱ����Ϣ
	 *@param [in] wParam, ��
	 *@param [in] lParam, INT*,0:����;1:����
	 *@return �ɹ�����S_OKʧ�ܷ���E_FAIL
	 */
	HRESULT OnGetEnvPeriod(WPARAM wParam, LPARAM lParam);

    /**
     *@breif �������AGC����ֵ
     *@param [in] wParam, ����ֵ
     *@param [in] lParam, ��
     *@return �ɹ�����S_OKʧ�ܷ���E_FAIL
     */
    HRESULT OnSetMaxAgcShutter(WPARAM wParam, LPARAM lParam);


	/**
	   @brief �Զ�����ץ�Ĳ���
	   @param [in] wParam ��
	   @param [in] wParam ��
	   @return �ɹ�����S_OKʧ�ܷ���E_FAIL
	 */
	HRESULT OnSetCaptureAutoParam(WPARAM wParam, LPARAM lParam);

	HRESULT SetRealTimeDayNightShutterHOri(WPARAM wParam, LPARAM lParam);

	//��Ϣӳ���
	SW_BEGIN_MESSAGE_MAP(CSWAutoControlRenderFilter, CSWMessage)
		SW_MESSAGE_HANDLER(MSG_AUTO_CONTROL_READPARAM, OnReadParam)
		SW_MESSAGE_HANDLER(MSG_AUTO_CONTROL_GET_REALPARAM, OnGetRealParam)
		SW_MESSAGE_HANDLER(MSG_AUTO_CONTROL_GET_ENVPERIOD, OnGetEnvPeriod)
		SW_MESSAGE_HANDLER(MSG_AUTO_SET_MAXAGCSHUTTER, OnSetMaxAgcShutter)
		SW_MESSAGE_HANDLER(MSG_REALTIME_SET_MAXAGCSHUTTERGAIN, SetRealTimeDayNightShutterHOri)
		//SW_MESSAGE_HANDLER(MSG_AUTO_CONTROL_SET_AUTO_CAPTURE_PARAM, OnSetCaptureAutoParam)
	SW_END_MESSAGE_MAP()
public:
	INT  m_iLightType;
	BOOL m_fEnableAGC;
	INT  m_irgAGCLimit[14];

private:
	BOOL m_fEnable;
	INT  m_iMinPSD;
	INT  m_iMaxPSD;
	INT  m_iCplStatus;
	INT  m_iPluseLevel;
	INT  m_irgExposureTime[14];
	INT  m_irgGain[14];
	BOOL m_fEnableAutoCapture;
	INT m_iDayShutterMax;
	INT m_iDayGainMax;
	INT m_iNightShutterMax;
	INT m_iNightGainMax;
	INT m_iGainMin;
	INT m_iShutterMin;
	INT m_iEnableCpl;

	INT m_iIsDayCount;
    INT m_iIsDuskCount;
	BOOL m_fIsDay;
	BOOL m_fIsDayEx;
    INT m_iEnvType;     // ���졢��������״̬��3:day, 2:dusk, 1:night
	BOOL m_fIsInit; // �Ƿ��Ѿ������������ϵ��жϡ�
	BOOL m_fForceChange;	// �ڳ�������ʱǿ�Ƹı�ץ�Ĳ�����
	INT m_iCaptureShutter;
	INT m_iCaptureGain;

	INT m_iTempCaptureShutter;
	INT m_iTempCaptureGain;
	
	BOOL m_fNeedUpdateCaptureParam;
	INT m_iCaptureImageCount;
	INT m_iTotalAvgY;
	
	CSWObjectList m_lstPDU;
	CSWThread m_thCameraPDU;

	INT m_iGammaMode;			// ����GAMMAģʽ.
    INT m_nNightShutter;        // ҹ���жϿ�����ֵ
    INT m_nNightThreshold;      // ҹ���жϻ���������ֵ
    INT m_nDuskThreshold;       // �����жϻ���������ֵ

	int m_nAvgPlateY;

	INT m_nMinPlateY;
	INT m_nMaxPlateY;
	INT m_nWDRLevel;

	BOOL m_fUseMaxAgcShutter;

	DWORD m_dwAvgY;	//����

    // ���졢���� ������������ֵ
    INT m_iAGCDayNightShutterControl;       // ���� 0:��   1:��
    INT m_iAGCDayShutterHOri;       // �����������
    INT m_iAGCNightShutterHOri;     // ���Ͽ�������
    INT m_iAGCShutterLOri;      // ��������
    INT m_iAGCNightGainHOri; 	//������������
    INT m_iAGCGainHOri; 	//������������
};
REGISTER_CLASS(CSWAutoControlRenderFilter)
#endif

