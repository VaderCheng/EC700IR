#ifndef _DJ_APP_TRACKER_H_
#define _DJ_APP_TRACKER_H_

#include "swbasetype.h"
#include "swimage.h"

#include "svEPApi/svEPApi.h"
#include "hvinterface.h"
//#include "DspLinkCmd.h"
#include "EPTrackInfo.h"
#include "../TrafficLight/TrafficLightImpl.h"
#include "platerecogparam.h"
#include "RoadInfo.h"
#include "trackerdef.h"
#include "DspLinkCmd.h"
#include "VirtualRefImage.h"
#include "ScaleSpeed.h"

class ITrackerCallback; // trackercallback.h

// ��ʱ����ĳ�����Ϣ�ṹ�����ڹ�����ͬ����
typedef struct tag_CarInfo
{
	char szPlate[32];
	DWORD32 iPlateTick;
	int iPlateRoad;
}
PLATE_INFO;

class CEPAppTracker
{
public:
    CEPAppTracker(void);
    ~CEPAppTracker(void);

    HRESULT Init(
        TRACKER_CFG_PARAM* pCfgParam,
        PlateRecogParam* pRecogParam,
        HvCore::IHvModel* pHvParam, 
        int iFrameWidth, 
        int iFrameHeight,
        IScaleSpeed *pScaleSpeed
        );

    // todo.
    // ���̵���ػص���ǰ���ǻص����ϵ�TRACKER�ӿڣ�
    // �������������½ӿ�
    HRESULT SetTrackerCallBack(ITracker* pCallBack);
    void UpdatemRedLightRect(HV_RECT* prgRect, const int& iCount);


    HRESULT Uninit();

    HRESULT Process(
        PROCESS_ONE_FRAME_PARAM* pProcParam,
        PROCESS_ONE_FRAME_DATA* pProcessData,
        PROCESS_ONE_FRAME_RESPOND* pProcessRespond
        );
    HRESULT PreProcess(
        PROCESS_ONE_FRAME_PARAM* pProcParam,
        PROCESS_ONE_FRAME_DATA* pProcessData,
        PROCESS_ONE_FRAME_RESPOND* pProcessRespond
        );
    HRESULT PostProcess();

    /// ���ûص���������Init֮ǰ
    HRESULT SetCallBackFunc(ITrackerCallback* pCallback);

    HRESULT SetFirstLightType(LIGHT_TYPE nLightType);

    HRESULT CalcCarSpeed(float &fltCarSpeed, float &fltScaleOfDistance, CEPTrackInfo* pTrackInfo, IScaleSpeed *m_pScaleSpeed);
private:

    HRESULT ProcessTrackState(
        HV_COMPONENT_IMAGE *pImage,
        PROCESS_ONE_FRAME_RESPOND* pProcessRespond
        );
    HRESULT OnTrackerFirstDet(
        CEPTrackInfo* pTrackInfo
        );
    HRESULT OnTrackerEnd(
        CEPTrackInfo* pTrackInfo
        );

    bool IsTrackerCanOutput(CEPTrackInfo* pTrackInfo);

    HRESULT FireCarLeftEvent(CEPTrackInfo* pTrackInfo);

    ///�Ƿ񰴳�����ʻ
    bool CheckRoadAndRun(RUN_TYPE runtype, int runroadtype);

    ///�Ƿ񴳺��
    bool CheckRush(int iPreLight, int iAcrossLight, int iThreeLight, int runroadtype, int runtype, DWORD32 dwCaptureTime);

    ///���ץ��
    HRESULT CheckCapture(
        HV_COMPONENT_IMAGE* pImage
        );

    ///����Υ��
    HRESULT CheckPeccancy(
        HV_COMPONENT_IMAGE* pImage
        );

    ///����ÿ֡�������ٵ�״̬
    HRESULT CheckOneState(CEPTrackInfo* pTrackInfo, HV_COMPONENT_IMAGE* pImage);

    HRESULT CheckPeccancyType(
        int iPreLight,			//ѹ��ǰ�ĺ��̵Ƴ���
        int iAcrossLight,		//���ߺ�ĺ��̵Ƴ���
        int iThreeLight,		//������ץ��λ�õĺ��̵Ƴ���
        RUN_TYPE runtype,	//��ʻ����
        int runroadtype,      //��ʻ��������
        DWORD32 dwCaptureTime,	//ץ��ʱ��
        /*PECCANCY_TYPE*/int * pPeccancyType //Υ������,�ƹ����޸�Ϊint����
        );
    HRESULT CheckTrafficLight(HV_COMPONENT_IMAGE* pSceneImage );

    HRESULT SetObjDetCfgParam(
        TRACKER_CFG_PARAM* pCfgParam,
        int iFrameWidth, 
        int iFrameHeight
        );

    HRESULT SetPlateRecogCfgParam(
        TRACKER_CFG_PARAM* pCfgParam,
        PlateRecogParam* pRecogParam
        );
    HRESULT InitTrafficLight(
        TRACKER_CFG_PARAM* pCfgParam
        );
    HRESULT SetMiscParam(
        TRACKER_CFG_PARAM* pCfgParam
        );
    
    // ץ�Ļص�
    static int Capture_CallBack(
        svEPApi::IEPTrack* pTrack,     ///< ����ָ��
        int nCapPosIndex      ///< ץ��λ���±�:0��1��2���ӽ���Զ
        );
    static CEPAppTracker* m_pInstance;  // ���ڵ���DoCapture
    // ��������ץ�ĺ���
    HRESULT DoCapture(svEPApi::IEPTrack* pITracker, int nCapPosIndex);

    HRESULT CheckLight();
    HRESULT CheckLightType(int iCarY, bool fIsAvgBrightness);

protected:
    static const int MAX_MOD_DET_INFO = svEPApi::MOD_DET_INFO::TYPE_COUNT;
    svEPApi::MOD_DET_INFO m_rgModelParam[MAX_MOD_DET_INFO];
    svEPApi::EP_PARAM m_cEPParam;
	svEPApi::CEPCtrl* m_pEPCtrl;
    svEPApi::EP_RESULT_INFO::LIGHT_TYPE m_nEnvLightType;  // ������������
    LIGHT_TYPE m_nPlateLightType;

    // zhaopy
    LIGHT_TYPE m_nFirstLightType;
    unsigned int m_dwLastCarLeftTime;
    LIGHT_TYPE m_LightType;
    int m_iMaxPlateBrightness;
    int m_iMinPlateBrightness;
    int m_iPlateLightCheckCount;
    int m_iNightThreshold;
    bool m_fIsCheckLightType;

    int m_nEnvLight;  // ��������

    int m_iFrameWidth;
    int m_iFrameHeight;

    HiVideo::CRoadInfo m_cRoadInfo;

    static const int MAX_DJ_TRACK_INFO = 30;
    CEPTrackInfo m_rgTrackInfo[MAX_DJ_TRACK_INFO];
    int m_iTrackInfoCnt;

    ITrackerCallback* m_pCallback;

    //���˵�������ץ��ͼΪ�̵Ƶ�Υ��
    int m_iFilterRushPeccancy;

    //<���̵�ר�ò���
	CTrafficLightImpl m_cTrafficLight;
    //��������
    int m_nLightCount;	
    //��ǰ����
    int m_iLastLightStatus;
    //ʵʱ���̵�״̬
    int m_iCurLightStatus;
    int m_iOutputInLine; //���δ��ֹͣ�ߵĳ���

    int m_nRedLightCount; 
    HV_RECT m_rgRedLightRect[MAX_TRAFFICLIGHT_COUNT * 2];	//��Ƶ���λ��
    //>

    int m_iBestLightMode;  // �Ƿ�ʹ������ͼģʽ����ֵΪ1ʱ�ø����������ͼ����������ͼ

    // ƽ���÷�����
    int m_iAverageConfidenceQuan;

    // ���ֵ÷�����
    int m_iFirstConfidenceQuan;

    //�¼�������
    ACTIONDETECT_PARAM_TRACKER m_ActionDetectParam;
    float m_fltOverLineSensitivity;   // ѹ�߼��������
    PlateRecogParam m_cRecogParam;
    PROCESS_ONE_FRAME_DATA* m_pProcessData;
    PROCESS_ONE_FRAME_RESPOND* m_pProcessRespond;

    IVirtualRefImage* m_pCurIRefImage;

    // todo
    ITracker* m_pTrackerCallback;

	int m_iRoadNumberBegin;							//���������ʼ����
	int m_iStartRoadNum;								//��ʼ������

	IScaleSpeed *m_pScaleSpeed;
	bool m_fEnableScaleSpeed;

	bool m_fFilterNoPlatePeccancy;
	int m_iBlockTwinsTimeout;

	// ���ڹ�����ͬ����
	static const int MAX_PLATE_INFO_COUNT = 50;
	PLATE_INFO m_rgPlateInfo[MAX_PLATE_INFO_COUNT];
	int m_iPlateInfoCount;

	// ���һ�����Ƴ��ĳ��ƿ��
	int m_iLastPlateWidth;
	// ���ٳ���
	int m_iLimitSpeed;
};

#endif //_DJ_APP_TRACKER_H_

