#pragma once

#include "swbasetype.h"
#include "swimage.h"

#include "svTgVvdApi/svTgVvdApi.h"
#include "hvinterface.h"
//#include "DspLinkCmd.h"
#include "AppTrackInfo.h"
#include "platerecogparam.h"
#include "RoadInfo.h"
#include "trackerdef.h"
#include "DspLinkCmd.h"
#include "VirtualRefImage.h"

class ITrackerCallback; // trackercallback.h

namespace swTgApp 
{

    class CPlateLightCtrl
    {
    public:
        CPlateLightCtrl();
        ~CPlateLightCtrl();
        void Init(int nMaxPlateY, int nMinPlateY, int nCheckOut, int nNightTH, int nMaxAgcTh);
        void SetFirstLightType(LIGHT_TYPE nLightType);

        HRESULT CheckLight(int nEnvAvgY);
        HRESULT UpdatePlateLight(int iY);

        LIGHT_TYPE GetPlateLightType();
        int GetWDRLevel();
        void SetArmEnvPeriodInfo(bool fIsArmNight){m_fIsARMNight = fIsArmNight;}	//����ARM�˵�ʱ���жϽ��	

        void SetMaxAgcTh(int nTh) {m_nMaxAGCTH = nTh;}
    private:
        HRESULT UpdateLightType(int iY, bool fIsAvgBrightness);

        // zhaopy
        LIGHT_TYPE m_nFirstLightType;
        unsigned int m_dwLastCarLeftTime;
        LIGHT_TYPE m_LightType;

        int m_iMaxPlateBrightness;
        int m_iMinPlateBrightness;
        int m_iPlateLightCheckCount;
        int m_iNightThreshold;

        LIGHT_TYPE m_nPlateLightType;
        int m_nWDRLevel;
        BOOL m_fIsARMNight;			//ARM���ж�Ϊ����
        int m_nMaxAGCTH;
        int m_nEnvAvgY;
    };

    struct LAST_RESULT_INFO
    {
        char szPlate[32];
        unsigned int nArriveTime;
    };


    struct TRIGGER_INFO
    {
        sv::CSvRect rcTriggerRecog;   // ץ���ϱ�����
        DWORD32 nTrackId;             // ����ID
        DWORD32 nTriggerTimesCnt;     // ������������
    };

    class CAppTrackCtrl
    {
    public:
        CAppTrackCtrl(void);
        virtual ~CAppTrackCtrl(void);

        HRESULT Init(
            TRACKER_CFG_PARAM* pCfgParam,
            PlateRecogParam* pRecogParam,
            int iFrameWidth, 
            int iFrameHeight
            );
        HRESULT Uninit();

        HRESULT Process(
            PROCESS_ONE_FRAME_PARAM* pProcParam,
            PROCESS_ONE_FRAME_DATA* pProcessData,
            PROCESS_ONE_FRAME_RESPOND* pProcessRespond
            );

        // ���ó�ʼ�������ȵȼ�
        void SetFirstLightType(LIGHT_TYPE nLightType);

        bool TriggerCallBack(
            sv::SV_RECT rcPos,      /// �ص���λ��
            int nTrackID
            );
    private:

        /*HRESULT RecogOneFrame(
            PROCESS_ONE_FRAME_PARAM* pProcParam,
            PROCESS_ONE_FRAME_DATA* pProcessData,
            PROCESS_ONE_FRAME_RESPOND* pProcessRespond,
            HV_COMPONENT_IMAGE hvImgFrame,
            HiVideo::CRect& rcRecogArae
            );*/

        HRESULT PreProcess(
            PROCESS_ONE_FRAME_PARAM* pProcParam,
            PROCESS_ONE_FRAME_DATA* pProcessData,
            PROCESS_ONE_FRAME_RESPOND* pProcessRespond
            );
        HRESULT PostProcess();


        HRESULT CheckLightType(int iCarY, bool fIsAvgBrightness);

        HRESULT ProcessTrackState(
            HV_COMPONENT_IMAGE *pImage,
            PROCESS_ONE_FRAME_RESPOND* pProcessRespond
            );
        HRESULT OnTrackerFirstDet(
            CAppTrackInfo* pTrackInfo
            );
        HRESULT OnTrackerEnd(
            CAppTrackInfo* pTrackInfo
            );

        bool IsTrackerCanOutput(CAppTrackInfo* pTrackInfo);

        HRESULT FireCarLeftEvent(CAppTrackInfo* pTrackInfo);

        ///���ץ��
        HRESULT CheckCapture(
            HV_COMPONENT_IMAGE* pImage
            );

        ///����Υ��
        HRESULT CheckPeccancy(
            HV_COMPONENT_IMAGE* pImage
            );

        ///����ÿ֡�������ٵ�״̬
        HRESULT CheckOneState(CAppTrackInfo* pTrackInfo, HV_COMPONENT_IMAGE* pImage);
        ///����Ƿ��������������������ʱ����ץ��
        HRESULT CheckCarArrive(CAppTrackInfo* pTrackInfo, HV_COMPONENT_IMAGE* pImage);

        HRESULT SetTgApiParam(
            TRACKER_CFG_PARAM* pCfgParam,
            PlateRecogParam* pRecogParam,
            int iFrameWidth, 
            int iFrameHeight
            );

        HRESULT SetCtrlParam(
            TRACKER_CFG_PARAM* pCfgParam
            );


        // �����Ƿ�ɹ�ץ��
        bool ProcessCarArrive(
            CAppTrackInfo* pTrack, 
            DWORD32 dwImageTime, 
            PROCESS_ONE_FRAME_RESPOND* pProcessRespond
            );

		void pwm_writeinfo_in_dsp(DWORD32 temp_data);
		void pwm_int_in_dsp(void);
        void TriggerCamera(const int iRoadNum);
		CROSS_OVER_LINE_TYPE IsOverYellowLine(CAppTrackInfo* pTrack);
        /// ��ԭ��ԭʼ�����ţ��Զ��ָ�������ʼ�Ų�Ϊ0��������������
        //int RecoverRoadNum(int iRoadNum);
		bool IsNeedCaptureAll();
    protected:
        static const int MAX_MOD_DET_INFO = svTgVvdApi::MOD_DET_INFO::TYPE_COUNT;
        svTgVvdApi::MOD_DET_INFO m_rgModDetInfo[MAX_MOD_DET_INFO];
        svTgVvdApi::TG_PARAM m_cApiParam;
        svTgVvdApi::CTgCtrl* m_pTgCtrl;
        svTgVvdApi::CTgCtrl::LIGHT_TYPE m_nEnvLightType;  // ������������

        TRACKER_CFG_PARAM m_cTrackerCfgParam;

        int m_nEnvLight;  // ��������

        int m_iFrameWidth;
        int m_iFrameHeight;

        swTgApp::CRoadInfo m_cRoadInfo;

        static const int MAX_TRACK_INFO = 30;
        CAppTrackInfo m_rgTrackInfo[MAX_TRACK_INFO];
        int m_iTrackInfoCnt;

        ITrackerCallback* m_pCallback;

        PlateRecogParam m_cRecogParam;
        PROCESS_ONE_FRAME_DATA* m_pProcessData;
        PROCESS_ONE_FRAME_RESPOND* m_pProcessRespond;

        IVirtualRefImage* m_pCurIRefImage;

        // ���ڹ�����ͬ����
        static const int MAX_LAST_RES_PLATE = 50;
        LAST_RESULT_INFO m_rgLastResultPlate[MAX_LAST_RES_PLATE];
        int m_nLastResultPlateCount;


        int m_iAverageConfidenceQuan; // ƽ���÷�����
        int m_iFirstConfidenceQuan;  // ���ֵ÷�����
        int m_iBlockTwinsTimeout; // ��ͬ�ƹ���ʱ�䣬��λ��
        int m_iArriveOnePos;        // ȡ��һ�Ŵ�ͼλ��
        int m_iArriveTwoPos;		// ȡ�ڶ��Ŵ�ͼλ��
        int m_iDetectReverseEnable;

        BOOL m_fIsCheckLightType;
        CPlateLightCtrl m_cPlateLightCtrl;
        // �Ƿ�ʹ�ܳ�������ץ�ģ��ڱ����ж�̬�ı�
        BOOL m_fEnableCarArriveTrigger;

        static const int MIN_TRIGGER_TIME = 150;	// ���δ�����С���ʱ��
        DWORD32 m_dwLastTriggerTick;	// ���һ�δ�����ʱ��
		DWORD32 m_dwLastTriggerRoadNum;
        DWORD32 m_dwTriggerCameraTimes;		// ��ǰ��������

        DWORD32 m_dwRecvTriggerImageTimes;  // ���յ���ץ��ͼ����
//
//        // ץ��ʶ��λ�ö���
//        static const int MAX_TRIGGER_INFO = 10;
//        TRIGGER_INFO m_rgTriggerInfo[MAX_TRIGGER_INFO];
//        int m_nTriggerInfoCnt;

        // ֡�ӳٺ��룬�����ӳ�ʱ���������ʱ��
        DWORD32 m_dwFrameDelay;
        // ��ʼ����ʱ��ϵͳʱ��
        DWORD32 m_dwProcessStartSystemTick;    
    };

}
