#ifndef _TRACKERIMPL_INCLUDED__
#define _TRACKERIMPL_INCLUDED__

#include "Tracker.h"
#include "PlateDetector.h"
#include "ProcessPlate.h"
#include "Recognition.h"
#include "TrackInfoHigh.h"
#include "ScaleSpeed.h"			// ���ٽӿ�
#include "..\ObjectTracking\ObjectTracking.h"			// �ƶ�������ٽӿ�
#include "..\TrafficLight\trafficlightimpl.h"
#include "ItgArea.h"
#include "ObjectTrackingInfo.h"

#include "CarDet.h"

#define DRAW_RECT_NUM 10
#define DRAW_RECT_LEN 50

using namespace HiVideo;

typedef struct car_left
{
	DWORD32 car_left_time;
	PLATE_COLOR car_plate;
	DWORD32 frame_no;
}CAR_LEFT_STRUCT;

//TODO:	������˽ӿڣ���������ʹ�ü̳еķ�ʽ�������࣬���ø��ϡ�
class CFinalResultParam : public CTrackInfo
{
	PlateRecogParam *m_pParam;
	DWORD32 m_dwLastResultTime;		
public:
	void Create(CTrackInfo& TrackInfo, PlateRecogParam *pParam);
	HRESULT UpdateResult(const CTrackInfo& TrackInfo);
	bool IsTwinResult(const CTrackInfo& TrackInfo);
	//���¶��� quanjh 2011-03-06
	void UpdateDetectSaver(DWORD32 dw_TimeRef);
	//�������� quanjh 2011-03-06
	void SetOrder();

public:
	static int m_iBlockTwinsTimeout;
	static int m_iSendTwinResult;
	//m_iBlockTwinsTimeoutʱ���ڴ�ŵĳ�����Ϣ quanjh 2011-02-23
	struct DETECTSAVER
	{
		BYTE8 rgContent[8];
		DWORD32 dw_TrackEndTime;
		CRect rcPos;
	}m_detectsaver[7200];
	//m_iBlockTwinsTimeoutʱ���ڶ��г��ȼ��� quanjh 2011-02-23
	int m_iTimeinCount;
};

class CTrackerImpl : public ITracker
{
public:
	CTrackerImpl();

// ITracker
	virtual ~CTrackerImpl();
	void Clear();

	STDMETHOD(ProcessOneFrame)(
		PROCESS_ONE_FRAME_PARAM* pParam,
		PROCESS_ONE_FRAME_DATA* pProcessData,
		PROCESS_ONE_FRAME_RESPOND* pProcessRespond
	);
	STDMETHOD(ProcessPhoto)(
		HV_COMPONENT_IMAGE imgCurFrame,
		PVOID pvParam,
		PVOID pvData,
		PROCESS_PHOTO_RESPOND* pProcessRespond
		)
	{
		return E_NOTIMPL;
	}

	STDMETHOD(SetInspector)(
		IInspector *pInspector
		)
	{
		return E_NOTIMPL;
	}

	STDMETHOD(SetHvParam)(
		HvCore::IHvParam2* pHvParam
	)
	{
		return E_NOTIMPL;
	}

	STDMETHOD(SetHvModel)(
		HvCore::IHvModel* pModel
	);

	STDMETHOD(SetHvPerformance)(
		HvCore::IHvPerformance* pHvPerf
		);

	STDMETHOD(SetCharSet)(
		ChnCharSet nCharSet
	);

	STDMETHOD(SetCalibrateParam)(
		BOOL fEnable,
		int nRotate=0,
		int nTilt=0,
		BOOL fOutputImg = FALSE
	);

	STDMETHOD(SetPreSegmentParam)(
		BOOL fEnable
	);
	STDMETHOD(SetLPRMode)(PlateRecogMode nMode);

	STDMETHOD(SetVideoDet)(BOOL fEnabled, BOOL fSkipFrame = TRUE, int nDetLeft = 0, int nDetRight = 100);

	STDMETHOD(SetDetArea)( const DETECTAREA& cDetAreaInfo );

	STDMETHOD(ForceResult)(int iFlag = 0);

	STDMETHOD(GetTrackCount)(PDWORD32 pdwCount);
	STDMETHOD(ClearTrackInfo)(PROCESS_ONE_FRAME_RESPOND* pProcessRespond);

	STDMETHOD(GetPlatePos)(DWORD32 dwIndex, CRect *prcPlate, CRect* prcRect);

	STDMETHOD(GetDetectCount)(PDWORD32 pdwCount);
	STDMETHOD(GetDetectPos)(DWORD32 dwIndex, CRect *prcDetected);

	STDMETHOD(GetObjCount)(PDWORD32 pdwCount);
	STDMETHOD(GetObjPos)(DWORD32 dwIndex, CRect *prcObj);
	STDMETHOD(GetEPObjCount)(PDWORD32 pdwCount);
	STDMETHOD(GetEPObjPos)(DWORD32 dwIndex, CRect *prcObj);

	STDMETHOD(IsVoted)(DWORD32 dwIndex, bool *pfVoted);

	//ΪHVCOREʵ�ֵĳ�ʼ������
	STDMETHOD(InitTracker)(TRACKER_CFG_PARAM* pCfgParam);

	//ΪHVCOREʵ�ֵĶ�̬�޸Ĳ���
	STDMETHOD(ModifyTracker)(TRACKER_CFG_PARAM* pCfgParam);

	// �������ģ�͵ĸ��½ӿ�
	STDMETHOD(UpdateModel)(unsigned int uModelId, unsigned char* pbData, unsigned int cbDataSize);

	// ��̬�����㷨�����ӿ�
	STDMETHOD(UpdateAlgParam)(unsigned char* pbData, unsigned int cbDataSize);

	//���Լ��ģ������ܣ����Ե�ģ�����ͺͲ��Բ���ͨ��ȫ�ֲ����б�����,
	//ȫ�ֲ����б�ͨ��INI�ļ�����.
	STDMETHOD(TestPerformance)(
		HV_COMPONENT_IMAGE *phciTest,	//��������
		char *pszReport,				//���Ա���
		DWORD32 dwReportBufSize,		//���Ա��滺��������
		DWORD32 dwMaxTestCount,			//�����Դ���
		DWORD32 dwMaxTestDuration		//������ʱ��(����)
	);

	STDMETHOD(ModuleTest)( 	
		MODULE_ID nModuleID,
		DWORD32 dwFlag,
		HV_COMPONENT_IMAGE *pImg,
		STD_RESULT* pStdResult,
		PVOID pvArgDat = NULL
		)
	{
		return E_NOTIMPL;
	}

	// �õ���������
	STDMETHOD(GetLightType)(LIGHT_TYPE* pLightType);

	STDMETHOD(SetFirstLightType)(LIGHT_TYPE nLightType);

	STDMETHOD(SetFirstPulseLevel)(int nPulseLevel);

	STDMETHOD(SetFirstCplStatus)(int nCplStatus);
	//ȡ�õ�ǰ��������
	STDMETHOD(GetItgArea)(CItgArea** ppItgArea, DWORD32 dwRefTime);

	// zhaopy
	STDMETHOD(CheckEvent)();

	//��⵱ǰ���еĸ����Ƿ���ҪCararrive
	HRESULT CheckCarArrive(
		PROCESS_ONE_FRAME_RESPOND* pProcessRespond,
		int iDiffTime = 0
		);
	HRESULT ProcessCarArrive(
		CTrackInfo* pTrack, 
		DWORD32 dwImageTime, 
		float fltDistance, 
		PROCESS_ONE_FRAME_RESPOND* pProcessRespond
		);

	HRESULT CheckCarArriveTrigger();
	// zhaopy
	HRESULT ProcessNoPlateCarArrive(
		CObjTrackInfo* pObjTrack, 
		DWORD32 dwImageTime, 
		float fltDistance, 
		PROCESS_ONE_FRAME_RESPOND* pProcessRespond
		);

	void CalcTrapDetArea(
		TRAP_DETAREA* pTrapDetArea,
		int nCarArriveLineY,  // �ٷֱ�
		int nFrameWidth,
		int nFrameHeight
		);

	HRESULT DetectionParkingBox(
		PROCESS_ONE_FRAME_PARAM* pParam,
		PROCESS_ONE_FRAME_DATA* pProcessData,
		PROCESS_ONE_FRAME_RESPOND* pProcessRespond
	);

private:
	BOOL m_fEnableCarArriveTrigger;	// CarArrive�Ƿ񴥷����ץ��
	int  m_iTriggerPlateType; //����ץ�ĵĳ�������,0-���г���;1-����
	// zhaopy
	static const int MIN_TRIGGER_TIME = 100;	// ���δ�����С���ʱ��
	DWORD32 m_dwLastTriggerTick;	// ���һ�δ�����ʱ��
	DWORD32 m_dwTriggerCameraTimes;		// ��ǰ��������

public:
	//�����������
	HRESULT OnCreate();

	HRESULT RecognizePlate(
		HV_COMPONENT_IMAGE imgPlate,
		PlateInfo &Info,
		CPersistentComponentImage& imgRectified
		);

	//ʶ������ɫ�����ⲿ������ʹ��
	HRESULT RecognizeCarColour(
		HV_COMPONENT_IMAGE imgPlateOri,
		PlateInfo &Info,
		int iAvgGray                                  //��������
		);

	LIGHT_TYPE m_LightType; //����������

	HRESULT RecogSnapImg(
		PROCESS_ONE_FRAME_PARAM* pParam,
		PROCESS_ONE_FRAME_DATA* pProcessData,
		PROCESS_ONE_FRAME_RESPOND* pProcessRespond,
		HV_COMPONENT_IMAGE& imgFrame,
		CRect& rcRecogArae,
		int nDetMinScale,
		int nDetMaxScale
		);


protected:
	static const int s_knMaxTrackInfo = 30;
	static const int s_knMaxPlateCandidate = 200;
	static const int s_knMaxObjCandidate = 30;
	static const int s_knMaxVideoChannel = 4;

	BOOL m_fEnablePreciseRectify;

	CFinalResultParam m_LastResult;

	RECTA m_rgTempPlatePos[s_knMaxPlateCandidate];

	BOOL m_fInitialized;
	INT m_nWidth, m_nHeight, m_nStride;

	CTrackInfo m_rgTrackInfo[s_knMaxTrackInfo];
	int m_cTrackInfo;
	int m_rgiFiveCharWidth[40];
	int m_nDefaultDBType;

	PlateRecogParam *m_pParam;
	CFinalResultParam* m_pLastResult;

	//������Ϣ�ṹ�����Ƿ��Ѿ���ʼ��
	bool m_fLastResultInited;

	IInspector *m_pInspector;
	IPlateDetector *m_pPlateDetector;
	IProcessPlate *m_pProcessPlate;
	IRecognition *m_pRecognition;
	IScaleSpeed *m_pScaleSpeed;							// ���ٽӿ�

	IObjectDetector *m_rgpObjectDetector[s_knMaxVideoChannel];	//�ƶ�������ӿ�
	CRect m_rgrcDetected[s_knMaxObjCandidate];
	int m_cDetected;
	//����ɨ������,��ԭ�еļ������
    CRect m_rgrcScanEP[s_knMaxObjCandidate];
    int m_cScanEP;
    //��βģ��ɨ��
    CRect m_rgrcCarTail[s_knMaxObjCandidate];
    int m_cCarTail;
	CObjTrackInfo m_rgObjTrackInfo[s_knMaxTrackInfo];	//�������
	int m_cObjTrackInfo;

	// zhaopy 0624
	CRect m_rcLightDetected[MAX_LIGHT_NIGHT_COUNT];
	int m_iLightDetected;

	//���ӵ羯�����Ƴ�����
    CObjTrackInfo m_rgEPObjTrackInfo[s_knMaxTrackInfo];	//�羯�����ӵ��������
    int m_cEPObjTrackInfo;
    OBJDETECT_INFO m_objDetectInfo;
	HvCore::IHvModel* m_pHvModel;
	HvCore::IHvPerformance* m_pHvPerf;

	int m_iAvgY;				//����������
	LIGHT_TYPE m_PlateLightType;	//�������ȼ�������

	CItgArea m_cItgArea; //ȫ�����ͼ����

	// �Ƿ�ʹ�ò����
	int m_iUsedLight;
	//��ͨ�����
	CTrafficLightImpl m_cTrafficLight;

	//��ͨ�����
	CTrafficLightInfo m_rgLightInfo[MAX_TRAFFICLIGHT_COUNT];

	//���ڻص����
	HV_RECT m_rgLightRect[MAX_TRAFFICLIGHT_COUNT];	//����λ��
	DWORD32 m_rgLightStatus[MAX_TRAFFICLIGHT_COUNT]; //����״̬
	CItgArea m_rgLightArea[MAX_TRAFFICLIGHT_COUNT];  //��ҪJPEG��ѹ�ĵ���λ��

	int m_nRedLightCount;
	HV_RECT m_rgRedLightRect[MAX_TRAFFICLIGHT_COUNT * 2];	//��Ƶ���λ��

	//���IO���̵���Ϣ
	//static const int MAX_IO_LIGHT_COUNT = 8;
	TRAFFICLIGHT_TYPE m_rgIOLight[MAX_IO_LIGHT_COUNT];

	//��ʼ��ʱ��������
	HRESULT OnInit();

	HRESULT SetFrameProperty(
		const FRAME_PROPERTY& FrameProperty,
		PROCESS_ONE_FRAME_RESPOND* pProcessRespond
	);

	HRESULT GetInflateRect(
		CRect &rect,		// in, out
		PLATE_COLOR color, 
		PLATE_TYPE &type,
		INT maxRectHeight	// �ж��Ƿ���inflate rate2
	);

	HRESULT RecognizeCarColour(
		HV_COMPONENT_IMAGE imgPlateOri,
		PlateInfo &Info,
		int iAvgGray ,                                 //��������
		BOOL fIsNight
	);

	// 2011.3 ����������ɫʶ��
	HRESULT RecognizeColorArea(
		const HV_COMPONENT_IMAGE imgInput, 
		HV_RECT rcCrop, 
		BYTE8 *cTempAdd, 
		BYTE8 *piResult,
		int iCutLeng,
		LIGHT_TYPE m_PlateLightType
	);
	HRESULT CalcPlateColor(HV_COMPONENT_IMAGE imgFrame,PBYTE8 pBW,int &nColor);
	HRESULT GetPlateColor(PBYTE8 RBG1,PBYTE8 RBG2, int &nColor);
	HRESULT GetColorResult(PBYTE8 RBG1,PBYTE8 RBG2,bool *fDiffThr, int &nColor);

	HRESULT ProcessTrackState(
		CTrackInfo *pTrack,									//��ǰ����ĸ���
		CRect &rcTrackArea,									//��������
		PlateInfo &plateObserved,							//��ǰ֡��⵽�ĳ�����Ϣ
		HV_COMPONENT_IMAGE &imgCalibratedFrame,				//������������Ƶ֡
		CPersistentComponentImage &imgBestRectified,		//���������ĳ���Сͼ
		HV_COMPONENT_IMAGE &imgOriSnapFrame,				//ԭʼ����Ƶ֡
		HV_COMPONENT_IMAGE &imgOriCapFrame, 
		PROCESS_ONE_FRAME_PARAM* pParam,
		PROCESS_ONE_FRAME_RESPOND* pProcessRespond	
	);

	HRESULT ProcessTemTrackState(							// ����ģ��ƥ��״��ĳ��?
		CTrackInfo *pTrack,									//��ǰ����ĸ���
		CRect &rcTrackArea
		);

	HRESULT TrackingObj(PROCESS_ONE_FRAME_PARAM* pParam, HV_COMPONENT_IMAGE &imgCurImage, PROCESS_ONE_FRAME_RESPOND* pProcessRespond);
	HRESULT ShowObjTracks(PROCESS_ONE_FRAME_PARAM* pParam, int iVideoID, PROCESS_ONE_FRAME_RESPOND* pProcessRespond);
	HRESULT ShowTracks(	
		PROCESS_ONE_FRAME_PARAM* pParam, 
		PROCESS_ONE_FRAME_DATA* pProcessData, 
		PROCESS_ONE_FRAME_RESPOND* pProcessRespond
		);
	HRESULT RemoveInvalidTracks(PROCESS_ONE_FRAME_RESPOND* pProcessRespond);
	HRESULT FireCarLeftEvent(
		CTrackInfo& TrackInfo, 
		PROCESS_ONE_FRAME_PARAM* pParam,
		PROCESS_ONE_FRAME_DATA* pProcessData, 
		PROCESS_ONE_FRAME_RESPOND* pProcessRespond
		);
	HRESULT FireNoPlateCarLeftEvent(PROCESS_ONE_FRAME_PARAM* pParam, CObjTrackInfo& ObjTrackInfo, PROCESS_ONE_FRAME_RESPOND* pProcessRespond);
	HRESULT TrackingObjByTem(PROCESS_ONE_FRAME_PARAM* pParam, HV_COMPONENT_IMAGE &imgCurImage, PROCESS_ONE_FRAME_RESPOND* pProcessRespond);
    CRect GetEPNoPlatePredictRect(UINT nNowTime, CObjTrackInfo* pObjTrack);
	
    HRESULT CheckEPNoPlateConfidence(HV_COMPONENT_IMAGE* pImgCalibratedFrame, int iVideID);
    HRESULT CheckEPNoPlateConfidence2(HV_COMPONENT_IMAGE* pImgCalibratedFrame, int iVideID);

	HRESULT TestDetPerformance(
		HV_COMPONENT_IMAGE *phciTest,	//����ͼ��
		char *pszReport,				//���Ա���
		DWORD32 dwReportBufSize,		//���Ա��滺��������
		DWORD32 dwMaxTestCount,			//�����Դ���
		DWORD32 dwMaxTestDuration		//������ʱ��(����)
	);
	HRESULT TestSegPerformance(
		HV_COMPONENT_IMAGE *phciTest,	//����ͼ��
		char *pszReport,				//���Ա���
		DWORD32 dwReportBufSize,		//���Ա��滺��������
		DWORD32 dwMaxTestCount,			//�����Դ���
		DWORD32 dwMaxTestDuration		//������ʱ��(����)
	);
	STDMETHOD(GetTrafficLightStatus)(
		unsigned char* pbStatus
		)
	{
		*pbStatus = m_nIOLevel;
		return S_OK;
	}
	STDMETHOD(TrafficLightStatus)(
		UINT nLightCount,		//��������
		DWORD32*	rgLightStatus,	//����״̬
		INT*	pSceneStatus,	//�û����ݵ���״̬�Զ���ĳ���״̬
		HV_RECT* rgLightPos,		//��������λ��
		UINT nRedLightCount,   //��Ƶ�����
		HV_RECT* rgRedLightPos,	//��Ƶ�����(���ں�ƵļӺ�)
		HV_COMPONENT_IMAGE* pSceneImage//ȫ��ͼ
		)
	{
		m_nRedLightCount = nRedLightCount;
		for(int i = 0; i < nRedLightCount; i++)
		{
			m_rgRedLightRect[i] = rgRedLightPos[i];
		}
		return S_OK;
	}

	//�жϰ����ҹ;
	HRESULT CheckLight();
	//�ж�˳�����
	HRESULT CheckLightType(int iCarY, bool fIsAvgBrightness);
	// �����жϲ�������
	HRESULT SetCarTypeParam(int nWidth, int nHeight);

	HRESULT CheckTrafficLight(HV_COMPONENT_IMAGE* pSceneImage );

	//���㳵��
	HRESULT CalcCarSpeed(float &fltCarSpeed, float &fltScaleOfDistance, CTrackInfo &TrackInfo);
	HRESULT CalcObjSpeed(float &fltCarSpeed, float &fltScaleOfDistance, CObjTrackInfo &ObjTrackInfo);

	// �Ƿ���Դ���ģ��(ƥ��)
	bool IsCanFoundTemple(CTrackInfo *pTrack);
	// �����Ƿ�ֹͣ
	bool IsCarStop(CTrackInfo *pTrack);
	// ����ģ��������λ��
	void UpdateOffset(CTrackInfo *pTrack);

	void RoadInfoParamInit();
	void InitRoadInfoParam();
	//ͨ��IOȡ��ǰ���̵�״̬
	DWORD32 GetIOStatus(int iTeam, BYTE8 bLevel);
	//�ж��Ƿ�ѹ��
	bool IsOverLine(int iX, int iY);
	//�����ʻ��?
	RUN_TYPE CheckRunType(HiVideo::CRect rect0, HiVideo::CRect rect1);
	RUN_TYPE CheckRunTypeEx(HiVideo::CRect rect0, HiVideo::CRect rect1);

	//ж��Ƿ��п���Υ�?
	bool CheckPeccancy(CTrackInfo *pTrack);
	bool CheckObjectPeccancy(CObjTrackInfo* pObjTrackInfo);

	// zhaopy
	bool CheckPeccancyTrigger(CTrackInfo* pTrack);
	bool CheckObjectPeccancyTrigger(CObjTrackInfo* pObjTrackInfo, int iObjIndex);
	HRESULT MatchTrackInfoAndObjectInfo(PROCESS_ONE_FRAME_PARAM *pParam, PROCESS_ONE_FRAME_RESPOND* pProcessRespond);
	//����ÿһ֡���һ֡��֡���ʱ����һ��֡��
	HRESULT CalcFrameSpeed(CTrackInfo *pTrack);
	//�¼����
	//ѹ����
	CROSS_OVER_LINE_TYPE IsOverYellowLine(CTrackInfo& TrackInfo);
	//Խ��
	CROSS_OVER_LINE_TYPE IsCrossLine(CTrackInfo& TrackInfo);

	//���˴���ֹͣ��ǰͣ�µĶ�� weikt 2011-4-19
	HRESULT CheckNoPlate(CObjTrackInfo* pObj);
	HRESULT RemoveMoreReview(CObjTrackInfo* pObj);

	// zhaopy 0624
	//added by liujie ��ҹ������������
	BOOL RainyFlashControl(HiVideo::CRect* pPos, int iVideoID);
	BOOL RainyObjOutPutControl(HiVideo::CRect* pPos, int iTrackID);

	/////////////////////////////////////////////////////////////////
	// Υ���ж�

	bool CheckRoadAndRun(RUN_TYPE runtype, int runroadtype);
	bool CheckRush(int iPreLight, int iAcrossLight, int runroadtype, int runtype, DWORD32 dwCaptureTime);

	HRESULT CheckPeccancyType(
		int iPreLight,			//ѹ��ǰ�ĺ��̵Ƴ���
		int iAcrossLight,		//���ߺ�ĺ��̵Ƴ���
		RUN_TYPE runtype,	//��ʻ����
		int runroadtype,      //��ʻ��������
		DWORD32 dwCaptureTime,	//ץ��ʱ��
		/*PECCANCY_TYPE*/int * pPeccancyType //Υ������,�ƹ����޸�Ϊint����
		);

	HRESULT DownSampleYUV422(HV_COMPONENT_IMAGE imgSrc, HV_COMPONENT_IMAGE imgDst);

	// �����ݶ�λ��
	HRESULT GetPlateNail(
		PLATE_COLOR color,			
		HV_COMPONENT_IMAGE imgPlate, 
		CRect* pCharPos
		);

	// zhaopy 0624
	bool IsCanOutputReverseRun(CObjTrackInfo* pObj);

// zhaopy һ���ץ��
private:
	void TriggerCamera(const int iRoadNum);
	//��¼��ץ����Ϣ
	void SetTriggerInfo(const DWORD32 dwLastImageTime
					   ,const DWORD32 dwCarArriveTime
	                   ,const DWORD32 dwRoadId
	                   ,const DWORD32 dwTriggerIndex
	                   );

	//ץ�ĸ���(�г���)
	void CapTrack(CTrackInfo* pTrack);
	
	//ץ������(�޳���)
	void CapObjTrack(CObjTrackInfo* pObjTrack);

	//�жϳ��������Ƿ�������ı��������� weikt 2011-7-20
	bool IsContainOfTrap(TRAP_DETAREA trapDetArea,CRect rectPlate);

    //< TrackerImplHigh_PhotoRecog.cpp
    HRESULT CalcPlateColor(HV_COMPONENT_IMAGE imgFrame,PBYTE8 pBW, PLATE_TYPE nPlateType,PLATE_COLOR &nColor,int &nAvgY1, int &nAvgY2);

    HRESULT PreTreatment(
        HV_COMPONENT_IMAGE& imgPlate,
        PLATE_TYPE nPlateType,
        PLATE_COLOR nPlateColor,
        BOOL fEnhance,
        CPersistentComponentImage* rgImgRectified,
        int nCount,	 //ָ��Ԥ�������ɵ�ͼ����,ͨ���ı䴹ֱУ���Ƕ�
        CBinaryInfo &BinInfo
        );

    HRESULT CropPlateImage(
        const HV_COMPONENT_IMAGE& imgPhoto,
        CRect& rcOriPos,
        PLATE_TYPE nPlateType,
        int nDetConf,
        HV_COMPONENT_IMAGE* rgPlateImage,
        int nCount	//ָ����ȡͼ�Ľ����
        );

    HRESULT CTrackerImpl::Vote(
        PlateInfo** rgInfo,
        int nCount,
        PlateInfo* pResult,
        CParamQuanNum* pfltConf
        );

    HRESULT RecogSnapImg(
        HV_COMPONENT_IMAGE& imgFrame,
        CRect& rcRecogArae,
        int nDetMinScale,
        int nDetMaxScale
        );
    //>

private:
	//�����ò���
	DETECTAREA m_cDetAreaInfo;

	DETECTAREA m_cRecogSnapArea;

	BOOL m_fEnableScaleSpeed;						// �������ʹ��

	// �Ƿ����������Ϣ
	int m_fOutputDebugInfo;

	// �Ƿ��жϹ�������־
	int m_iRecogGxPolice;
	//�¾���ʶ�𿪹�
	int m_iRecogNewMilitary;

	//��5λ�Ƿ���ĸʶ��
	int m_fEnableAlpha_5;

	// �Ƿ�������ͬ�ĳ��Ʋ��ܳ�����
	int m_iSendTwinResult;

	// ������ͬ���Ƴ��ֵ�ʱ����(��)
	int m_iBlockTwinsTimeout;

	// ƽ���÷�����
	int m_iAverageConfidenceQuan;

	// ���ֵ÷�����
	int m_iFirstConfidenceQuan;

	// ���ػ����֣�38~68, 58Ϊ����
	int m_nDefaultChnId;				

	//˫���ƿ���
	int m_iEnableDefaultDBType;
	int m_iMiddleWidth;
	int m_iBottomWidth;
	
	//���Ƽ�ⷽ�����
	BOOL m_fMinVarianceUserCtrl;
	int m_iMinVariance;

	//��WJ������
	BOOL m_fEnableNewWJSegment;
	//��WJ�ַ��滻
	BOOL  m_fEnableDefaultWJ;
	char  m_strDefaultWJChar[3];

	//����ʶ�𿪹�
	BOOL  m_fEnableBigPlate;
	//�ָ�ǰ�Ƿ����ͼƬ��ǿ
	BOOL m_fEnablePlateEnhance;
	//�Ƿ����ڰ���
	BOOL m_fEnableProcessBWPlate;
	//�ָ�ǰСͼ��������ֵ
	int m_iPlateResizeThreshold;
	// ��������ͼδʶ��
	BOOL m_fUseEdgeMethod;

	//��������
	int m_nLightCount;	
	//��ǰ����
	int m_iLastLightStatus;
	//ʵʱ���̵�״̬
	int m_iCurLightStatus;
	//��һ����Ч�ĺ��̵�״̬
	int m_iLastOkLightStatus;

	int m_iLastTriggerTimeMs;
	static int m_sMinTriggerTimeMs;			//���δ�����ʱ����
	DWORD32 m_dwLastTriggerInfo;								//���һ�δ�������Ϣ
	static const int s_iMinTriggerPosTime = 200;		//����λ�ô�������Сʱ����		
	int m_iLastFirstTime;
	int m_iLastSecondTime;
	int m_iLastThirdTime;
	int m_iRushSpeed;									//����Ƶ��ٶ�

	int m_iSceneCount;
	//��ͨ�Ƴ���״̬������
	int m_nValidSceneStatusCount;
	//����ת������
	bool m_fCheckSpeed;
	//������Ч������
	int m_nErrorSceneCount;

	int m_iSpan;

	//�����ҹ��Ƶ������ֵ
	int m_iNightThreshold;
	int m_iPlateLightCheckCount;		//�������ȼ����
	int m_iMinPlateBrightness;
	int m_iMaxPlateBrightness;
	//�Ƿ����ƫ�⾵
	int m_iCtrlCpl;
	//��ǰƫ�⾵״̬ 1:ʹ�� 0:��ʹ�� -1��δ����
	int m_iCplStatus;
	//�Ƿ�ǿ�ư����𲹹��
	int m_iForceLightOffAtDay;
	int m_iForceLightOffThreshold;
	int m_iForceLightOffFlag;
	//�Ƿ���Ҫ���¸��ݻ������ȵ���
	int m_iAdjustForAvgBrigtness;
	//��Ҫ������ʱ��
	DWORD32 m_dwAdjustTick;
	// zhaopy ���ڵ����ȵȼ�����
	LIGHT_TYPE m_nLightTypeCpl;
	int m_iPulseLevel;

	//��������λ��
	int m_iCarArrivedPos;
	//���Ƴ�����λ��
	int m_iCarArrivedPosNoPlate;

	//��������λ��(����)
	int m_iCarArrivedPosYellow;
	//���������ӳپ���
	int m_iCarArrivedDelay;

	// �����жϲ���
	int m_nCarLenTop;				// С����ͼ�󶥲��ĸ߶�
	int m_nCarLenBottom;			// С����ͼ��ײ��ĸ߶�
	int m_nCarWTop;					// С����ͼ�󶥲��Ŀ��
	int m_nCarWBottom;				// С����ͼ��ײ��Ŀ��
	float m_fltSlopeH;
	float m_fltSlopeW;

	BOOL m_fOutputCarSize;			// ��������ߴ�

	int	m_nVideoDetMode;			// 0:����Ƶ��� 1:ֻ�Ǳ������ 2:������Ƶ���
	CRect m_rcVideoDetArea;		//��Ƶ�������

	DWORD32 m_dwLastCarLeftTime; // ���һ�γ���ʱ��

	//ÿ��������̵ƿ�ʼʱ��(��ת,ֱ��,��ת,��ͷ��
	LIGHT_REALTIME m_redrealtime;
	//�̵ƿ�ʼ���ʱ��
	LIGHT_TICK m_greentick;
	//����ӳ�ʱ��
	LIGHT_TICK m_ltRedRelay;

	LIGHT_TYPE m_nFirstLightType;
	int m_iOutputInLine;
	// ѹ�߼�������Ȳ���
	float m_fltOverLineSensitivity;

	int m_iOutPutType;					// �����ʽ
	float m_fltBikeSensitivity;			//�ǻ������жϵ�������
	int m_iWalkManSpeed;				//�ж����˵�������

	DWORD32 m_dwTriggerTimes;			//��������

	//��Ƶ���ͶƱ֡��.
	int m_iVoteFrameCount;
	//��Ƶ����Ƿ��������ʻ������
	int m_iCheckAcross;
	//���˵�������ץ��ͼΪ�̵Ƶ�Υ��
	int m_iFilterRushPeccancy;

	//Τ���� ���˴󳵳�������� 2011-4-6
	BOOL m_fFilterMoreReview;
	float m_fltHMThreld;       //������ֵ
	float m_fltHMThreldNight;  //ҹ����ֵ
	//ģ��ƥ��ֱ��ͼ�ӳٸ���֡��
	int m_HMDelay;             //�����ӳ�֡��
	int m_HMDelayNight;        //ҹ���ӳ�֡��
	int m_iMaxMatchDiff;

	TRACKER_CFG_PARAM m_cfgParam;
	//added by liujie ���ƴ󳵶��
	CAR_LEFT_STRUCT m_leftCar[16];
	int m_iBigCarDelayFrameNo;

	int m_iCarArriveTrig; //�շ�վģʽ���Ƿ����ץ��

	int m_nIOLevel;
	
	//added by liujie MFCǰ����ʾ���ο�
	//int m_RectNum[DRAW_RECT_NUM];
	//HiVideo::CRect m_DrawRect[DRAW_RECT_NUM][DRAW_RECT_LEN];

	int m_iNoPlateDetMode; //���Ƴ����ģʽ 0:��׼ģʽ 1:ģ��ģʽ

	//ģ�ͼ���
	RECTA m_rgEPCarPos[s_knMaxPlateCandidate];

	// zhaopy 0624
	//������ҹ�����ұ��⳵���������
	int m_iEdgeFilterThreld;
	
	// zhaopy
	// �������Ƴ�Υ��
	BOOL m_fFilterNoPlatePeccancy;
    BOOL m_fEnableDetFace;
	int m_nMinFaceScale;
	int m_nMaxFaceScale;

	// ������ʻ���� -- �����ж�����
	MovingDirection	m_TravelDirection;	

	//������ɫʶ�����
	int m_nEnableColorReRecog;  //С���Ƿ�ȡ��������������ʶ����
	int m_nWGColorReThr;        //�Ұ���ʶ�𡪡�ƫ����ʶ���
	int m_nWColorReThr;        //����ʶ�𡪡��Ұ׵�����ƫ����ʶ���
	int m_nBColorReThr;        //����ʶ�𡪡��Һڵ�����ƫ����ʶ���
	bool m_fEnableCorlorThr;  //���������복�����ȱȽϿ���
	int m_nBGColorThr;        //�������ȱȻ������ȴ�BGColorThr�ĺ�ɫ�������óɻ�ɫ��
	int m_nWGColorThr;        //���ϳ������ȱȻ������ȴ�WGColorThr�Ļ�ɫ�������óɰ�ɫ��

	// �Ƿ����޸Ĳ����Ĳ���,������Ҫ�����㷨��SetImageSize�ӿ�
	bool m_fIsModifyParam;
	CCarDetector m_cCarDetector;
};

#endif // _TRACKERIMPL_INCLUDED__
