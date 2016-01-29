#ifndef _TRACKERCALLBACK_INCLUDED__
#define _TRACKERCALLBACK_INCLUDED__

#include "swObjBase.h"
#include "swPlate.h"
#include "swImage.h"
#include "swImageObj.h"
#include "stdResultInfo.h"
#include "HvBaseObj.h"

// ���ܿ��źŶ���
#define MAX_SIGNAL_TYPE		10				// ����ź���������
#define MAX_SIGNAL_SOURCE	30				// ����ź�Դ����

//�����ṹ�嶨��
#define BINARY_IMAGE_WIDTH 112				// ��ֵ��ͼ�Ŀ��
#define BINARY_IMAGE_HEIGHT 20				// ��ֵ��ͼ�ĸ߶�

typedef enum {
	BIG_DAY_FRONTLIGHT = 0,
	BIG_DAY_FRONTLIGHT_2,
	DAY_FRONTLIGHT,
	DAY_FRONTLIGHT_2,
	DAY,
	DAY_2,
	DAY_BACKLIGHT,
	DAY_BACKLIGHT_2,
	BIG_DAY_BACKLIGHT,
	BIG_DAY_BACKLIGHT_2,
	NIGHTFALL,
	NIGHTFALL_2,
	NIGHT,
	NIGHT_2,
	LIGHT_TYPE_COUNT
} LIGHT_TYPE;

//�������ݽṹ, �ó��������궨
typedef struct 
{
	//���ߵ�����������
	HV_POINT ptTop;
	HV_POINT ptBottom;
	int iLineType;
	int iRoadType; //��������
	char szRoadTypeName[255];
	int iFilterRunType; //Ҫ���˵���ʻ����
} ROAD_INFO;

const int MAX_ROADLINE_NUM = 4;
typedef struct _ACTIONDETECT_PARAM_TRACKER
{
	int iDetectCarStopTime;	// ����ͣ���ж�ʱ�䣬��λ��
	int iIsYellowLine[MAX_ROADLINE_NUM];		//0-01������1-12�������Դ�����
	int iIsCrossLine[MAX_ROADLINE_NUM];		//0-01������1-12�������Դ�����
}ACTIONDETECT_PARAM_TRACKER;

//���̵�״̬
typedef enum _TRAFFICLIGHT_LIGHT_STATUS
{
	TLS_UNSURE = 0,
	TLS_RED,
	TLS_YELLOW,
	TLS_GREEN
} LIGHT_STATUS;

//���̵Ʒ���
typedef enum _TRAFFICLIGHT_POSITION
{
	TP_UNSURE = 0,
	TP_LEFT,
	TP_FORWARD,
	TP_RIGHT,
	TP_TURN,
	MAX_TP_COUNT
} TRAFFICLIGHT_POSITION;

//�Ƶ�����
typedef struct _TRAFFICLIGHT_TYPE
{
	int iTeam;
	int iPos;
	_TRAFFICLIGHT_POSITION tpPos;
	_TRAFFICLIGHT_LIGHT_STATUS tlsStatus;
}TRAFFICLIGHT_TYPE;

//���̵���󳡾���
#define MAX_SCENE_COUNT 10
typedef struct _TRAFFICLIGHT_SCENE
{
	LIGHT_STATUS lsLeft;
	LIGHT_STATUS lsForward;
	LIGHT_STATUS lsRight;
	LIGHT_STATUS lsTurn;
} TRAFFICLIGHT_SCENE;

typedef struct _SCENE_INFO
{
	char pszInfo[64];
}SCENE_INFO;

//Υ������,�ƹ����޸ģ�ÿ��Υ������ռһλ������Υ�����͵ĵ���;ͬʱע�͵�MAX_PT_COUNT;2010-10-12
typedef enum PECCANCY_TYPE
{
	PT_NORMAL      = 0,
	PT_RUSH        =0x01,
	PT_ACROSS      =0x02,
	PT_OVERLINE    =0x04,
	PT_CONVERSE    =0x08,
	PT_ESTOP       =0x10,
	PT_STOPING     =0x20,
	PT_OVERSPEED   =0x40
	//MAX_PT_COUNT
} PECCANCY_TYPE;

//������ʻ����
typedef enum RUN_TYPE
{
	RT_UNSURE = 0,
	RT_FORWARD,
	RT_LEFT,
	RT_RIGHT,
	RT_TURN,
	RT_CONVERSE,
	RT_ESTOP,
	MAX_RT_COUNT
} RUN_TYPE;

//������ʻ��������
typedef enum RUN_ROAD_TYPE
{
	RRT_FORWARD = 0x01,
	RRT_LEFT          = 0x02,
	RRT_RIGHT		 = 0x04,
	RRT_TURN	       = 0x08,
	RRT_ESTOP        = 0x10,
	RRT_CONVERSE = 0x20
} RUN_ROAD_TYPE;

//�����ľ���ʱ��
typedef struct tag_LightRealTime
{
	DWORD32 dwLeftL;
	DWORD32 dwLeftH;
	DWORD32 dwForwardL;
	DWORD32 dwForwardH;
	DWORD32 dwRightL;
	DWORD32 dwRightH;
	DWORD32 dwTurnL;
	DWORD32 dwTurnH;
	tag_LightRealTime()
	{
		Reset();
	}
	void Reset()
	{
		dwLeftL = dwLeftH = dwForwardL = dwForwardH = dwRightL = dwRightH = dwTurnL = dwTurnH = 0;
	}
}
LIGHT_REALTIME;

//���������ʱ��
typedef struct tag_LightTick
{
	DWORD32 dwLeft;
	DWORD32 dwForward;
	DWORD32 dwRight;
	DWORD32 dwTurn;
	tag_LightTick()
	{
		Reset();
	}
	void Reset()
	{
		dwLeft = dwForward = dwRight = dwTurn = 0;
	}
}
LIGHT_TICK;

//ѹ�ߺ�Խ������
typedef enum CROSS_OVER_LINE_TYPE
{
	COLT_INVALID = -1,
	COLT_NO = -2,
	COLT_LINE0 = 0,
	COLT_LINE1 = 1,
	COLT_LINE2 = 2,
	COLT_LINE3 = 3,
	COLT_LINE4 = 4
}CROSS_OVER_LINE_TYPE;

// �����ߴ���Ϣ���
typedef struct _OUTPUT_CAR_SIZE_INFO {
	int nOutType;				// ������� 0:���� 1����
	float iCarWidth;			// �������
	float iCarHeight;			// �����߶�
} OUTPUT_CAR_SIZE_INFO;

typedef struct tagResultImageStruct {
	IReferenceComponentImage *pimgPlate;
	IReferenceComponentImage *pimgBestSnapShot;
	IReferenceComponentImage *pimgLastSnapShot;
	IReferenceComponentImage *pimgBeginCapture;
	IReferenceComponentImage *pimgBestCapture;
	IReferenceComponentImage *pimgLastCapture;
	IReferenceComponentImage *pimgPlateBin;
	tagResultImageStruct()
	{
		memset(this, 0, sizeof(tagResultImageStruct));
	}
} RESULT_IMAGE_STRUCT;

HRESULT CopyResultImageStruct(RESULT_IMAGE_STRUCT *pDest, const RESULT_IMAGE_STRUCT *pSrc);
HRESULT CopyResultImageStructOnlyJPEG( RESULT_IMAGE_STRUCT *pDest, const RESULT_IMAGE_STRUCT *pSrc );
HRESULT FreeResultImageStruct(RESULT_IMAGE_STRUCT *pObj);

typedef struct tagCarLeftInfoStruct {
	PBYTE8 pbContent;					// ���ƺ�����Ϣ
	BYTE8 bType;						// ��������
	BYTE8 bColor;						// ���ƺ���
	float fltAverageConfidence;			// ���Ŷ�
	float fltFirstAverageConfidence;	// ���ַ����Ŷ�
	RESULT_IMAGE_STRUCT *pResultImage;	// ���ͼƬ��Ϣ
	UINT iFrameNo;						// ���֡����
	UINT iRefTime;						// ���֡ʱ��
	UINT nStartFrameNo;					// ��ʼ֡����
	UINT nEndFrameNo;					// ����֡����
	UINT nFirstFrameTime;				// ��ʼ����ʱ��
	int iObservedFrames;				// �۲�֡��
	int iVotedObservedFrames;			// ���ͶƱ�������͵���Ч֡��
	bool fReverseRun;					// ���������˶���־
	int nVoteCondition;					// ͶƱ����
	int iAvgY;							//����������
	int iCarAvgY;						//��������
	int iCarVariance;					//���ƶԱȶ�
	int nVideoDetID;					// ��Ƶ���ID��(�������Ƴ�ʹ��)
	int nCarLeftCount;					// �����뿪֡��������
	float fltCarspeed;					// ������Ϣ
	float fltScaleOfDistance;			// ��������������
	int nPlateLightType;				// ��ǰ�������ȼ���
	UINT nCarArriveTime;				// CarArrive��ʱ��
	HiVideo::CRect rcBestPlatePos;		// ��������������
	HiVideo::CRect rcLastPlatePos;		// �����ֳ�������
	HiVideo::CRect rcSecondPos;		// �ڶ���ץ��λ�ó��Ƶ�����
	bool fCarIsStop;					// �Ƿ�ֹͣ��
	int iCarStopPassTime;				// ֹͣ������ʱ��
	CAR_TYPE nCarType;					// �������ͣ����С�С��
	CAR_COLOR nCarColor;				// ������ɫ
	bool fOutputCarColor;				//���������ɫ����
	OUTPUT_CAR_SIZE_INFO stCarSize;		// �����ߴ�

	int                 iRoadType;    //�������ͣ��ƹ�����ӣ�2011-03-07
	RUN_TYPE            rtType;       //������ʻ����
	/*PECCANCY_TYPE*/ int ptType;	//Υ������
	
	CROSS_OVER_LINE_TYPE coltIsOverYellowLine;		//ѹ����
	CROSS_OVER_LINE_TYPE coltIsCrossLine;			//Խ��
	
	TRAFFICLIGHT_SCENE tsOnePosScene;		//���Ƶ�һ��λ�õĺ��̵Ƴ���
	TRAFFICLIGHT_SCENE tsTwoPosScene;		//���Ƶڶ���λ�õĺ��̵Ƴ���

	//��ƿ�ʼʱ��(���ʱ��,����)
	LIGHT_REALTIME lrtRedStart;

	//���Υ�½���Ƿ�ȷ��
	int iUnSurePeccancy;
	// ���������ƵID
	int iVideoID;
	// ǿ�Ʒ���ץ��ͼƬ����
	int iSendCapImageFlag;

	//������ŵķ���0:����ʼ,1:���ҿ�ʼ
	int iRoadNumberBegin;
	//��ʼ������
	int iStartRoadNum;
	//�ж���ѹʵ�߻���˫����
	bool fIsDoubleYellowLine;

	//ץ�Ĳ���ץ�ĵļ���ʱ��
	DWORD32 dwFirstTime;
	DWORD32 dwSecondTime;
	DWORD32 dwThirdTime;

	//��ǰ�Ƿ�������
	bool fIsNight;

	tagCarLeftInfoStruct()
	{
		memset(this, 0, sizeof(tagCarLeftInfoStruct));
		coltIsOverYellowLine	= COLT_NO;
		coltIsCrossLine		= COLT_NO;
	}
} CARLEFT_INFO_STRUCT;					// �����뿪��Ϣ

typedef struct tagCarArriveInfoStruct {
	int iRoadNumber;
	// ���������ƵID
	int iVideoID;
	UINT iPlateLightType;
	UINT dwTriggerOutDelay;
	UINT dwIsOutTrig;
	UINT dwCarArriveTime;
	tagCarArriveInfoStruct()
	{
		memset(this, 0, sizeof(tagCarArriveInfoStruct));
		iRoadNumber = -1;
	}
} CARARRIVE_INFO_STRUCT;


//����CARLEFT_INFO_STRUCT

HRESULT CopyCarLeftInfoStruct(
							  CARLEFT_INFO_STRUCT *pDest,
							  const CARLEFT_INFO_STRUCT *pSrc, 
							  int iFlag = 0			//�Ƿ�ֻ����JPEGͼƬ��0����1����
							  );

//�ͷ�CARLEFT_INFO_STRUCT�ڲ������ڴ�
HRESULT FreeCarLeftInfoStruct(
							  CARLEFT_INFO_STRUCT *pObj
							  );


//֡����
typedef struct _FRAME_PROPERTY
{
	INT iWidth;
	INT iHeight;
	INT iStride;
}
FRAME_PROPERTY;

typedef struct _VIDEODET_PARAM
{
	BOOL fEnable;
	BOOL fAllowFrameSkip;
	INT nDetLeft;
	INT nDetRight;

	_VIDEODET_PARAM()
	{
		fEnable = FALSE;
		fAllowFrameSkip = TRUE;
		nDetLeft = 0;
		nDetRight = 100;
	}
}
VIDEODET_PARAM;

//ԤУ������
typedef struct _PRECALIBRATE_PARAM
{
	BOOL fEnable;
	INT nRotate;
	INT	nTilt;
	BOOL fOutputImg;

	_PRECALIBRATE_PARAM()
	{
		fEnable = FALSE;
		nRotate = 0;
		nTilt = 0;
		fOutputImg = FALSE;
	}
}
PRECALIBRATE_PARAM;

typedef struct _VOTEINFO
{
	DWORD32 dwIndex;
	BOOL fVoted;

	_VOTEINFO()
	{
		dwIndex = 0;
		fVoted = FALSE;
	}
}
VOTEINFO;

typedef struct _DETECTAREA
{
	BOOL fEnableDetAreaCtrl;
	int DetectorAreaLeft;
	int DetectorAreaRight;
	int DetectorAreaTop;
	int DetectorAreaBottom;
	int nDetectorMinScaleNum;
	int nDetectorMaxScaleNum;

	_DETECTAREA()
	{
		fEnableDetAreaCtrl = FALSE;
		DetectorAreaLeft = 0;
		DetectorAreaRight = 100;
		DetectorAreaTop = 0;
		DetectorAreaBottom = 40;
		nDetectorMinScaleNum = 6;
		nDetectorMaxScaleNum = 12;
	}
}
DETECTAREA;

typedef struct _DBL_DETECTAREA
{
	BOOL fEnableDetAreaCtrl;
	int DetectorAreaLeft;
	int DetectorAreaRight;
	int DetectorAreaTop;
	int DetectorAreaBottom;
	int nDetectorMinScaleNum;
	int nDetectorMaxScaleNum;

	_DBL_DETECTAREA()
	{
		fEnableDetAreaCtrl = FALSE;
		DetectorAreaLeft = 0;
		DetectorAreaRight = 100;
		DetectorAreaTop = 0;
		DetectorAreaBottom = 75;
		nDetectorMinScaleNum = 4;
		nDetectorMaxScaleNum = 10;
	}
}
DBL_DETECTAREA;

typedef struct _PR_PARAM
{
	enum{MAX_DET_AREA = 16}; //��������������
	UINT nSize;		//�����ṹ�峤��
	INT nMinPlateWidth;			//��С���Ƽ����,Ĭ��Ϊ100
	INT nMaxPlateWidth;		//����Ƽ����,Ĭ��Ϊ200
	INT	nVariance;					//���Ʒ�����ֵ,Ĭ��Ϊ30
	INT nDetAreaCount;	//���������
	HV_RECT rgDetArea[MAX_DET_AREA];		//�԰ٷ�����ʾ�ļ������,Ĭ��Ϊ0~100

	_PR_PARAM()
	{
		nSize = sizeof(_PR_PARAM);
		nMinPlateWidth = 60;
		nMaxPlateWidth = 250;
		nVariance = 30;
		nDetAreaCount = 1;
		for(int i = 0; i < MAX_DET_AREA; i++)
		{
			rgDetArea[i].left = 0;
			rgDetArea[i].top = 0;
			rgDetArea[i].right = 100;
			rgDetArea[i].bottom = 100;
		}	
	}
}
PR_PARAM;

typedef struct _PLATE_POS
{
	DWORD32 dwIndex;
	DWORD32 dwLeft;
	DWORD32 dwTop;
	DWORD32 dwRight;
	DWORD32 dwBottom;

	_PLATE_POS()
	{
		dwIndex = 0;
		dwLeft = 0;
		dwTop = 0;
		dwRight = 0;
		dwBottom = 0;
	}
} PLATE_POS;

typedef struct _TESTPERFORMANCE_INFO
{
	HV_COMPONENT_IMAGE* pImgDat;
	char* pszPerformanceReport;
	DWORD32 dwReportSize;
	DWORD32 dwMaxTestCount;
	DWORD32 dwMaxTestDuration;
}
TESTPERFORMANCE_INFO;

typedef struct _DEBUG_INSPECTOR_INFO
{
	UINT nFrameNo;	// ֡��
	int iHL;		// ����

	int nPlatePosLeft;
	int nPlatePosTop;
	int nPlatePosRight;
	int nPlatePosBottom;

	BYTE8 rgbContent[7];
	int rgfltConfidence[7];
	int AveragePosConfidence[7];

	UINT color;
	UINT nPlateType;
	int iLastDetWidth;
	int iLastCharHeight;
	int iLastVscale;
	BOOL fIndividualPlate;
	BOOL fYellowPlate;
	_DEBUG_INSPECTOR_INFO()
	{
		memset(this, 0, sizeof(_DEBUG_INSPECTOR_INFO));
	}
}
DEBUG_INSPECTOR_INFO;

enum VOTE_CONDITION {
	NEW2END,			//NEW״̬�¸��ٶ�ʧ
	LEAVE_TRACK_RECT,	//����ģʽ�³����뿪��������
	TRACK_MISS,			//�ڸ��������ڸ��ٶ�ʧ
	ENDRECT_MISS,		//�ڸ�����������ٶ�ʧ
	MAX_EQUAL_FRAME,	//����ʶ������ͬ
	MAX_VOTE_FRAME		//��Ч���������ֵ
};

typedef struct _CORE_PERFORMANCE_TEST_DATA {
	BOOL m_fTestPerformance;			//��ܲ��Կ��?
	char *m_pszPerformanceReport;		//���Ա��滺����
	DWORD32 m_dwReportSize;				//���Ա��滺������С
	HV_COMPONENT_IMAGE	m_imgTestData;	//����ͼƬ
	DWORD32 m_dwMaxTestCount;			//�����Դ���
	DWORD32 m_dwMaxTestDuration;		//������ʱ��(����)
	HRESULT m_hrTestFunc;				//���Ժ�������ֵ
} CORE_PERFORMANCE_TEST_DATA;

extern CORE_PERFORMANCE_TEST_DATA g_CorePerformanceTestData;

class ITrackerCallback
{
public:
	STDMETHOD(CarArrive)(
		CARARRIVE_INFO_STRUCT *pCarArriveInfo,
		LPVOID pvUserData
	) = 0;

	STDMETHOD(CarLeft)(
		struct tagCarLeftInfoStruct *pCarLeftInfo,
		LPVOID pvUserData
	)=0;

	STDMETHOD(DisposeUserData)(
		UINT iFrameNo,
		UINT iRefTime,
		LPVOID pvUserData
	)
	{
		return S_OK;
	}

	//��������λ��
	STDMETHOD(FirstTrigger)(
			LIGHT_TYPE lighttype,
			DWORD32 dwRoad,
			LPVOID pvOtherInfo
		)
	{
		return E_NOTIMPL;
	}

	STDMETHOD(SecondTrigger)(
		LIGHT_TYPE lighttype,
		DWORD32 dwRoad,
		LPVOID pvOtherInfo
		)
	{
		return E_NOTIMPL;
	}

	STDMETHOD(ThirdTrigger)(
		LIGHT_TYPE lighttype,
		DWORD32 dwRoad,
		LPVOID pvOtherInfo
		)
	{
		return E_NOTIMPL;
	}
	STDMETHOD(ReaddSignal)(
	DWORD32 dwTriggerFlag
	)
	{
		return E_NOTIMPL;	
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
		return E_NOTIMPL;
	}

	STDMETHOD(TrafficSceneStatus)(
		TRAFFICLIGHT_SCENE tsSceneStatus	//�û��Զ���ĳ���״̬
		)
	{
		return E_NOTIMPL;
	}

	//ȡ��ǰ���̵Ƶ�״̬,ͨ��IPT
	STDMETHOD(GetTrafficLightStatus)(
		unsigned char* pbStatus
		)
	{
		return E_NOTIMPL;
	}

	//ȡ��ǰ���̵ƿ�λ����Ϣ
	STDMETHOD(GetTrafficLightPos)(
		PUINT cbLightCount,		//��������
		HV_RECT* pLightPos,		//��������λ��
		PUINT cbRedLightCount,   //��Ƶ�����
		HV_RECT* pRedLightPos	//��Ƶ�����(���ں�ƵļӺ�)
		)
	{
		return E_NOTIMPL;
	}
};

#endif // _TRACKERCALLBACK_INCLUDED__
