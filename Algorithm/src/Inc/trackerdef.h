// ���ļ������ʽ����ΪWINDOWS-936��ʽ

/**
* @file	trackerdef.h
* @version	1.0
* @brief Traker��ؽӿڶ���
*/

#ifndef _TRACKERCALLBACK_INCLUDED__
#define _TRACKERCALLBACK_INCLUDED__

#include "swobjbase.h"
#include "swimage.h"
#include "swimageobj.h"
#include "hvbaseobj.h"
#include "stdresultinfo.h"

#include "hvcoretype.h"
#include "swplate.h"

//�����ṹ�嶨��
#define BINARY_IMAGE_WIDTH 112				/**< ��ֵ��ͼ�Ŀ�� */
#define BINARY_IMAGE_HEIGHT 20				/**< ��ֵ��ͼ�ĸ߶� */

#ifndef MAX_TRAFFICLIGHT_COUNT
#define MAX_TRAFFICLIGHT_COUNT 8           /**< ��������*/
#endif
#ifndef MAX_SCENE_COUNT
#define MAX_SCENE_COUNT 10                  /**< ��󳡾���*/
#endif

namespace HiVideo
{
    //���̵�״̬
    typedef enum _TRAFFICLIGHT_LIGHT_STATUS
    {
        TLS_UNSURE = 0,
        TLS_RED,
        TLS_YELLOW,
        TLS_GREEN
    } LIGHT_STATUS;

    typedef struct _TRAFFICLIGHT_SCENE
    {
        LIGHT_STATUS lsLeft;
        LIGHT_STATUS lsForward;
        LIGHT_STATUS lsRight;
        LIGHT_STATUS lsTurn;
    } TRAFFICLIGHT_SCENE;
}

using namespace HiVideo;

/**
*  �������ݽṹ, �ó��������궨
*/
typedef struct
{
    //���ߵ�����������
    HV_POINT ptTop;
    HV_POINT ptBottom;
    int iLineType;
    int iRoadType;      //��������
    char szRoadTypeName[255];//������������
    int iFilterRunType; //Ҫ���˵���ʻ����
} ROAD_INFO;

/**
*  �¼��������
*/
const int MAX_ROADLINE_NUM = 4;
typedef struct _ACTIONDETECT_PARAM_TRACKER
{
    int iDetectCarStopTime;                 	// ����ͣ���ж�ʱ�䣬��λ��
    int iIsYellowLine[MAX_ROADLINE_NUM];		// 0-01������1-12�������Դ�����
    int iIsCrossLine[MAX_ROADLINE_NUM];		    // 0-01������1-12�������Դ�����
} ACTIONDETECT_PARAM_TRACKER;

typedef enum _TRAFFICLIGHT_POSITION
{
    TP_UNSURE = 0,
    TP_LEFT,
    TP_FORWARD,
    TP_RIGHT,
    TP_TURN,
    MAX_TP_COUNT
} TRAFFICLIGHT_POSITION;

typedef enum
{
    NO_VIDEODET = 0,			// ��ʹ����Ƶ���
    USE_BACK_DET_ONLY,			// ֻʹ�ñ������
    USE_VIDEODET,				// ʹ����Ƶ���
    VIDEODET_MODE_COUNT
} HV_VIDEODET_MODE;


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
typedef enum _RUN_TYPE
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
typedef enum _RUN_ROAD_TYPE
{
    RRT_FORWARD = 0x01,
    RRT_LEFT    = 0x02,
    RRT_RIGHT	= 0x04,
    RRT_TURN	= 0x08,
    RRT_ESTOP   = 0x10,
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
typedef struct _OUTPUT_CAR_SIZE_INFO
{
    int nOutType;				// ������� 0:���� 1����
    float iCarWidth;			// �������
    float iCarHeight;			// �����߶�
} OUTPUT_CAR_SIZE_INFO;

//  ʶ����ͼƬ���ݽṹ
typedef struct tagResultImageStruct
{
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

/**
* �����ߴ���Ϣ���
*/
typedef struct _CAR_SIZE_INFO
{
    int nOutType;				// ������� 0:���� 1����
    float iCarWidth;			// �������
    float iCarHeight;			// �����߶�
} CAR_SIZE_INFO;

/**
*  ʶ����Ľ�����ݽṹ����Tracker�ṩ��
*/
typedef struct _PROCESS_IMAGE_CORE_RESULT
{
    // ����Ԫ��4�ֽڶ���
    BYTE8 rgbContent[8];	            // ���ƺ�����Ϣ
    char szPlate[24];					// �����ַ���
    int nType;						    // ��������
    int nColor;						    // ������ɫ
    float fltAverageConfidence;		// ���Ŷ�
    float fltFirstAverageConfidence;	// ���ַ����Ŷ�
    int iAvgY;							// ����������
    int iCarAvgY;						// ��������
    int iCarVariance;					// ���ƶԱȶ�
    CAR_TYPE nCarType;				    // �������ͣ����С�С��
    CAR_COLOR nCarColor;				// ������ɫ
    int nRoadNo;						// ������
    CAR_SIZE_INFO cCarSize;		        // �����ߴ�
    DWORD32 nFrameNo;						// ���֡����
    DWORD32 nRefTime;						// ���֡ʱ��
    DWORD32 nStartFrameNo;					// ��ʼ֡����
    DWORD32 nEndFrameNo;					// ����֡����
    DWORD32 nFirstFrameTime;				// ��ʼ����ʱ��
    int iObservedFrames;				// �۲�֡��
    int iVotedObservedFrames;			// ���ͶƱ�������͵���Ч֡��
    BOOL fReverseRun;					// ���������˶���־
    int nVoteCondition;					// ͶƱ����
    //�ж���ѹʵ�߻���˫����
    BOOL fIsDoubleYellowLine;

    //����ץ�ĵļ���ʱ��
    DWORD32 dwFirstTime;
    DWORD32 dwSecondTime;
    DWORD32 dwThirdTime;

    CROSS_OVER_LINE_TYPE coltIsOverYellowLine;		//ѹ����
    CROSS_OVER_LINE_TYPE coltIsCrossLine;			//Խ��

    int nCarLeftCount;					// �����뿪֡��������
    float fltCarspeed;					// �����ٶ�
    float fltScaleOfDistance;			// ��������������
    int nPlateLightType;				// ��ǰ�������ȼ���
    int iCplStatus;						// ƫ�⾵״̬
    int iPulseLevel;                     //��������ȼ�
    UINT nCarArriveTime;				// CarArrive��ʱ��
    HV_RECT        rcBestPlatePos;		// ��������������
    HV_RECT        rcLastPlatePos;		// �����ֳ�������
    HV_RECT        rcFirstPos;		    // ��һ��ץ��λ�ó��Ƶ�����
    HV_RECT        rcSecondPos;		    // �ڶ���ץ��λ�ó��Ƶ�����
    HV_RECT        rcThirdPos;		    // ������ץ��λ�ó��Ƶ�����
    int nPlateWidth;                    // ����Сͼ���
    int nPlateHeight;                   // ����Сͼ�߶�
    BOOL fCarIsStop;					// �Ƿ�ֹͣ��
    int iCarStopPassTime;				// ֹͣ������ʱ��

    TRAFFICLIGHT_SCENE tsOnePosScene;		//���Ƶ�һ��λ�õĺ��̵Ƴ���
    TRAFFICLIGHT_SCENE tsTwoPosScene;		//���Ƶڶ���λ�õĺ��̵Ƴ���

    HV_RECT         rcRedLightPos[20];		//���λ������
    int             rcRedLightCount;			//�������
    CRect rgFaceRect[20];
    int nFaceCount;

    int iCapturerAvgY;
    bool fIsCapture;
	bool fIsNight;						//ҹ������־

    //��ƿ�ʼʱ��(���ʱ��,����)
    LIGHT_REALTIME lrtRedStart;

    //���Υ�½���Ƿ�ȷ��
    int iUnSurePeccancy;
    // ���������ƵID
    int iVideoID;
    // ǿ�Ʒ���ץ��ͼƬ����
    int iSendCapImageFlag;
    //������ʻ����
    RUN_TYPE rtType;
    //Υ������
    int ptType;
    //Խ�߼�⿪��
    int nDetectCrossLineEnable;
    //ѹ���߼�⿪��
    int nDetectOverYellowLineEnable;
    //ʶ����ͼƬ��
    RESULT_IMAGE_STRUCT cResultImg;
    DWORD32 dwBestSnapShotRefTime;
    DWORD32 dwLastSnapShotRefTime;
    DWORD32 dwBeginCaptureRefTime;
    DWORD32 dwBestCaptureRefTime;
    DWORD32 dwLastCaptureRefTime;

    DWORD32 dwTriggerTime;
	//ץ�ı�־
	DWORD32 dwTriggerIndex;
	//ƥ������ 0:ƥ��ʧ�� 1:����ץ�ı�־ƥ�� 2:����ʱ��ƥ��
	DWORD32 dwMatchResult;

	//ʵ�ʴ���ץ�ĵ�����
	int    iCapCount;
	//ץ��ʱ���ٶ�
	int    iCapSpeed;
	int    iMatchCount;

	//��ɫHSL����
    int iH;
    int iS;
    int iL;

    int nWdrLevel;

    _PROCESS_IMAGE_CORE_RESULT()
    {
        memset(this, 0, sizeof(_PROCESS_IMAGE_CORE_RESULT));
        coltIsOverYellowLine	= COLT_NO;
        coltIsCrossLine		= COLT_NO;
    }
} PROCESS_IMAGE_CORE_RESULT;

/**
*  ʶ���ⲿ������ݽṹ
*/
typedef struct _PROCESS_IMAGE_OTHER_INFO
{
    // ����Ԫ��4�ֽڶ���
    UINT iFrameNo;						// ���֡����
    UINT iRefTime;						// ���֡ʱ��
    UINT nStartFrameNo;					// ��ʼ֡����
    UINT nEndFrameNo;					// ����֡����
    UINT nFirstFrameTime;				// ��ʼ����ʱ��
    int iCarAvgY;						// ��������
    int iCarVariance;					// ���ƶԱȶ�

    //new add(todo: �¼�������ݣ�֮����Ҫ�����Ƿ�Ҫ�ٷ���)
    int nPlateLightType;                // ��ǰ�������ȼ���
    int iRoadNumberBegin;               // ������ŵķ��� 0:����ʼ, 1:���ҿ�ʼ
    int iStartRoadNum;                  // ��ʼ������
    int iObservedFrames;				// �۲�֡��
    int nCarLeftCount;					// �����뿪֡��������
    UINT nCarArriveTime;                // CarArrive��ʱ��

    _PROCESS_IMAGE_OTHER_INFO()
    {
        memset(this, 0, sizeof(_PROCESS_IMAGE_OTHER_INFO));
    }
} PROCESS_IMAGE_OTHER_INFO;

/**
*  CarLeft�������ݽṹ
*/
typedef struct tagCarLeftInfoStruct
{
    PROCESS_IMAGE_CORE_RESULT cCoreResult;
    PROCESS_IMAGE_OTHER_INFO cOtherInfo;
} CARLEFT_INFO_STRUCT;					// �����뿪��Ϣ

/**
*  CarArrive�������ݽṹ
*/
typedef struct tagCarArriveInfoStruct
{
    int iRoadNumber;
    UINT iPlateLightType;
    UINT dwTriggerOutDelay;
    UINT dwIsOutTrig;
    UINT dwCarArriveTime;
    UINT dwCarArriveRealTime;
    DWORD32 dwFirstPos;
    DWORD32 dwEndPos;
    tagCarArriveInfoStruct()
    {
        memset(this, 0, sizeof(tagCarArriveInfoStruct));
        iRoadNumber = -1;
    }
} CARARRIVE_INFO_STRUCT;

const DWORD32 EVENT_NONE = 0;
const DWORD32 EVENT_CARARRIVE = 0x0001;
const DWORD32 EVENT_CARLEFT = 0x0002;
const DWORD32 EVENT_FRAME_RECOED = 0x0004;
const int MAX_EVENT_COUNT = 8;

/**
*  ���ٿ���Ϣ
*/
typedef struct _TRACK_RECT_INFO
{
    DWORD32 dwTrackCount;
    CRect rgTrackRect[30];

    _TRACK_RECT_INFO()
    {
        memset(this, 0, sizeof(_TRACK_RECT_INFO));
    }
} TRACK_RECT_INFO;

typedef struct _SYNC_DET_DATA
{
    void* pbData;
    int nLen;
    _SYNC_DET_DATA()
    :   pbData(NULL)
    ,   nLen(0)
    {
    }
} SYNC_DET_DATA;
/**
*  ֡�����¼����ݽṹ
*/
typedef struct tagPrcoessEventInfoStruct
{
    DWORD32 dwEventId;
    LIGHT_TYPE cLightType; // �������ȵȼ�
    int iCplStatus;		   // ƫ�⾵״̬
    int nIOLevel;		  //IO״̬�����̵���Ϣ
    int iCarArriveTriggerType; //0-������;1-������ץ��;2-����ץ��
    BOOL fIsCheckLight;
    int nEnvLightType; // 0���죬1����2����
    int iPulseLevel;       //��������ȼ�
    int iFrameAvgY;   //֡ƽ������
    HV_RECT rcRedLight[20];
    TRACK_RECT_INFO cTrackRectInfo;
    int iCarArriveInfoCount;
    int iCarLeftInfoCount;
    CARARRIVE_INFO_STRUCT rgCarArriveInfo[MAX_EVENT_COUNT];
    CARLEFT_INFO_STRUCT rgCarLeftInfo[MAX_EVENT_COUNT];
    SYNC_DET_DATA cSyncDetData;  //�첽�������
    int nWDRLevel;	// WDR�ȼ�
	int rgiAllCarTrigger[MAX_EVENT_COUNT];
    tagPrcoessEventInfoStruct()
    {
        dwEventId = EVENT_NONE;
        iCarArriveInfoCount = 0;
        iCarLeftInfoCount = 0;
        nIOLevel = 0;
        fIsCheckLight = true;
        nEnvLightType = 0;
        iPulseLevel = 0;
        nWDRLevel = 0;
        memset(rcRedLight, 0, sizeof(rcRedLight));
		memset(rgiAllCarTrigger, 0, sizeof(rgiAllCarTrigger));
    }
} PROCESS_EVENT_STRUCT;

/**
*  �������֡�����¼����ݽṹ
*/
typedef struct tagPrcoessDetectEventInfoStruct
{
    HV_RECT rgcDetect[20];
    int rgiDetectType[20];
    int iDetectCount;
    tagPrcoessDetectEventInfoStruct()
    {
        memset(this, 0, sizeof(tagPrcoessDetectEventInfoStruct));
    }
} PROCESS_DETECT_INFO;


//����CARLEFT_INFO_STRUCT
HRESULT CopyCarLeftInfoStruct(
    CARLEFT_INFO_STRUCT *pDest,
    const CARLEFT_INFO_STRUCT *pSrc
);

//�ͷ�CARLEFT_INFO_STRUCT�ڲ������ڴ�
HRESULT FreeCarLeftInfoStruct(
    CARLEFT_INFO_STRUCT *pObj
);


/**
*  ֡�������ݽṹ
*/
typedef struct _FRAME_PROPERTY
{
    INT iWidth;
    INT iHeight;
    INT iStride;
}
FRAME_PROPERTY;

/**
* ��Ƶ������
*/
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

/**
* ԤУ������
*/
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

/**
* ɨ���������
*/
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

//�ı���ɨ�������ĸ��������
typedef struct TRAP_DETAREA
{
	BOOL fEnableDetAreaCtrl;
	int TopLeftX;
	int TopLeftY;
	int TopRightX;
	int TopRightY;
	int BottomLeftX;
	int BottomLeftY;
	int BottomRightX;
	int BottomRightY;
	int nDetectorMinScaleNum;
	int nDetectorMaxScaleNum;
	TRAP_DETAREA()
	{
		fEnableDetAreaCtrl = FALSE;
		TopLeftX = 0;
		TopLeftY = 0;
		TopRightX = 100;
		TopRightY = 100;
		BottomLeftX = 0;
		BottomLeftY = 0;
		BottomRightX = 100;
		BottomRightY = 100;
		nDetectorMinScaleNum = 6;
		nDetectorMaxScaleNum = 12;
	}
}TRAP_DETAREA;

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

//todo: ��Ҫ������ע���е�nMinPlateWidth��nMaxPlateWidthĬ��ֵ�빹�췽���еĳ�ʼ��ֵ��ͬ��
//attendtion by Shaorg�� MAX_DET_AREA���ɸ��ģ��������Bug����Ϊ�������ط�����16�����MAX_DET_AREAͬ�������
typedef struct _PR_PARAM
{
    enum{MAX_DET_AREA = 16};           //��������������
    UINT nSize;                         //�����ṹ�峤��
    INT nMinPlateWidth;			        //��С���Ƽ����,Ĭ��Ϊ100
    INT nMaxPlateWidth;		            //����Ƽ����,Ĭ��Ϊ200
    INT	nVariance;				        //���Ʒ�����ֵ,Ĭ��Ϊ30
    INT nDetAreaCount;	                //���������
    INT nRoadNum;                       //������
    HV_RECT rgDetArea[MAX_DET_AREA];    //�԰ٷ�����ʾ�ļ������,Ĭ��Ϊ0~100

    _PR_PARAM()
    {
        nSize = sizeof(_PR_PARAM);
        nMinPlateWidth = 60;
        nMaxPlateWidth = 250;
        nVariance = 30;
        nDetAreaCount = 1;
        nRoadNum = -1;
        for (int i = 0; i < MAX_DET_AREA; i++)
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

enum VOTE_CONDITION
{
    NEW2END,			//NEW״̬�¸��ٶ�ʧ
    LEAVE_TRACK_RECT,	//����ģʽ�³����뿪��������
    TRACK_MISS,			//�ڸ��������ڸ��ٶ�ʧ
    ENDRECT_MISS,		//�ڸ�����������ٶ�ʧ
    MAX_EQUAL_FRAME,	//����ʶ������ͬ
    MAX_VOTE_FRAME		//��Ч���������ֵ
};

typedef struct _CORE_PERFORMANCE_TEST_DATA
{
    BOOL m_fTestPerformance;			//���ܲ��Ա��
    char *m_pszPerformanceReport;		//���Ա��滺����
    DWORD32 m_dwReportSize;				//���Ա��滺������С
    HV_COMPONENT_IMAGE	m_imgTestData;	//����ͼƬ
    DWORD32 m_dwMaxTestCount;			//�����Դ���
    DWORD32 m_dwMaxTestDuration;		//������ʱ��(����)
    HRESULT m_hrTestFunc;				//���Ժ�������ֵ
} CORE_PERFORMANCE_TEST_DATA;

typedef struct _TRAFFICLIGHT_CFG_PARAM
{
    int nCheckType;         //���̵Ƽ�ⷽʽ
    int nAutoScanLight;     //��̬�����̵�λ�ÿ���
    int nSenceCheckMode;    //�������ģʽ
    int nStopLinePos;       //ֱ��ֹͣ��λ��
    int nLeftStopLinePos;   //��תֹͣ��λ��
    int nMatchScanSpeed;    //ɨ���ٶ�
    int nLightCount;        //��������
    int nValidSceneStatusCount;  //��Ч����״̬����
    int nOutputInLine;      //���δ��ֹͣ�ߵĳ���
    int nRedLightDelay;     //��ɫ�ƻƵƱ��Ƽ��(��)
    int nSpecificLeft;      //��ת�����
    int nRunTypeEnable;     //��ʻ����ǿ�����
    int nSceneCount;        //���̵Ƴ�����
    char rgszLightPos[MAX_TRAFFICLIGHT_COUNT][64];  //������Ϣ
    char rgszScene[MAX_SCENE_COUNT][32];  //������Ϣ

    int  nTrafficLightThread;
    int  nTrafficLightBrightValue;
    int  nAutoScanThread;
    int  nSkipFrameNO;
    BOOL fIsLightAdhesion;
    BOOL fDeleteLightEdge;
    int fltHMThreld;       //������ֵ
    int fltHMThreldNight;  //ҹ����ֵ
    //ģ��ƥ��ֱ��ͼ�ӳٸ���֡��
    int iHMDelay;             //�����ӳ�֡��
    int iHMDelayNight;        //ҹ���ӳ�֡��
    int	iMaxMatchDiff;
    int iRealForward;
    int  nFilterRushPeccancy;//���˵�3��ͼΪ�̵Ƶ����
    int  nFilterMoreReview;  //���˴󳵳����쿪��
    char szIOConfig[255];	 //��Ӻ��̵��ź�
    BOOL fEnhanceRedLight;   //�Ƿ���а�ɫ����Ӻ�
    BOOL fFilterNoPlatePeccancy;

    _TRAFFICLIGHT_CFG_PARAM()
    {
        nCheckType = 0;
        nAutoScanLight = 1;
        nSenceCheckMode = 0;
        nStopLinePos = 65;
        nLeftStopLinePos = 65;
        nMatchScanSpeed = 0;
        nLightCount = 0;
        nValidSceneStatusCount = 5;
        nOutputInLine = 1;
        nRedLightDelay = 0;
        nSpecificLeft = 0;
        nRunTypeEnable = 0;
        nSceneCount = 0;

        nTrafficLightThread = 3;
        nTrafficLightBrightValue = 3;
        nAutoScanThread = 10;
        nSkipFrameNO = 30;
        fIsLightAdhesion = FALSE;
        fDeleteLightEdge = FALSE;
        fFilterNoPlatePeccancy = FALSE;

        for (int i = 0; i < MAX_TRAFFICLIGHT_COUNT; i++)
        {
            strcpy(rgszLightPos[i], "(0,0,0,0),0,0,00");
        }
        for (int i = 0; i < MAX_SCENE_COUNT; i++)
        {
            strcpy(rgszScene[i], "(0,0,0,0),0");
        }

        nFilterRushPeccancy = 0;
        nFilterMoreReview = 0;
        fltHMThreld = 48;       //������ֵ
        fltHMThreldNight = 48;  //ҹ����ֵ
        //ģ��ƥ��ֱ��ͼ�ӳٸ���֡��
        iHMDelay = 7;             //�����ӳ�֡��
        iHMDelayNight = 7;        //ҹ���ӳ�֡��
        iMaxMatchDiff = 18;
        iRealForward = 40;

        strcpy(szIOConfig, "00,00,00,00,00,00,00,00");

        fEnhanceRedLight = TRUE;
    }
} TRAFFICLIGHT_CFG_PARAM;

typedef struct _SCALE_SPEED_PARAM
{
    BOOL fEnable;    //������ٿ���
    int nMethodsFlag;   //����ģʽ��0:ƽ��궨��, 1:�ռ�궨��
    float fltLeftDistance;  //��Ļ���ص����żܾ���
    float fltFullDistance;  //��Ļ���ص����żܾ���
    float fltCameraHigh;    //������߶�
    float fltAdjustCoef;    //����ϵ��
    float rgfltTransMarix[11];  //����ͼ�����굽�ռ�����ı任����

    float fltDistance;      // ��Ļ���ص����صľ���(��)
    float fltRoadWidth;     // �����������(��)

    _SCALE_SPEED_PARAM()
    {
        fEnable = FALSE;
        nMethodsFlag = 0;
        fltLeftDistance = 25.0f;
        fltFullDistance = 75.0f;
        fltCameraHigh = 6.5f;
        fltAdjustCoef = 1.050f;
        fltDistance = 90;
        fltRoadWidth = 3.75;
        rgfltTransMarix[0] = 165.472f;
        rgfltTransMarix[1] = -38.447f;
        rgfltTransMarix[2] = -4.968f;
        rgfltTransMarix[3] = 606.269f;
        rgfltTransMarix[4] = -2.743f;
        rgfltTransMarix[5] = 1.813f;
        rgfltTransMarix[6] = -177.545f;
        rgfltTransMarix[7] = 1089.211f;
        rgfltTransMarix[8] = 0.000f;
        rgfltTransMarix[9] = -0.035f;
        rgfltTransMarix[10] = -0.005f;
    }
}SCALE_SPEED_PARAM;

typedef struct _VIDEODET_CFG_PARAM
{
    int nVideoDetMode;  //��Ƶ���ģʽ
    int nDayBackgroudThreshold;     //���챳�������ֵ
    int nNightBackgroudThreshold;   //���ϱ��������ֵ
    int nVoteFrameCount;    //ͶƱ��Ч֡������֡�����ڴ�ֵ��ͶƱ
    int nCheckAcross;       //�Ƿ��������ʻ������
    int nShadowDetMode;     //��Ӱ���ģʽ
    int nShadowDetEffect;   //��Ӱ���ǿ���̶�ֵ
    int nDiffType; // ǰ����ȡ��ʽ
    int nMedFilter; // ���ƶ�������
    int nMedFilterUpLine; // ���ƶ�����������������
    int nMedFilterDownLine; // ���ƶ�����������������
    int nBigCarDelayFrameNo;
    CRect rcVideoDetArea;   //��Ƶ�������
	int iEPOutPutDetectLine;         //�羯��⵽����ģ�齻����
	int iEPFGDetectLine;
	int iNoPlateDetMode;     //���Ƴ����ģʽ 0:��׼ģʽ 1:ģ��ģʽ
	int iEPUseCarTailModel; //�羯���Ƴ�����Ƿ�ʹ��ģ�͹��˶��
	int iEPNoPlateScoreThreld; //���Ƴ����˶��ķ�����ֵ

    _VIDEODET_CFG_PARAM()
    {
        nVideoDetMode = USE_VIDEODET;
        nDayBackgroudThreshold = 20;
        nNightBackgroudThreshold = 50;
        nVoteFrameCount = 30;
        nCheckAcross = 0;
        nShadowDetMode = 0;
        nShadowDetEffect = 0;
        nDiffType = 0;
        nMedFilter = 0;
        nMedFilterUpLine = 0;
        nMedFilterDownLine = 0;
        rcVideoDetArea.SetRect(0, 0, 100, 100);
        nBigCarDelayFrameNo = 32;
		iEPOutPutDetectLine = 63;         //�羯��⵽����ģ�齻����
		iEPFGDetectLine = 45;             //���Ƴ������������(���붥���ٷֱ�)
		iNoPlateDetMode = 0;
		iEPUseCarTailModel = 0;
		iEPNoPlateScoreThreld = 300;
    }
}VIDEODET_CFG_PARAM;

// Tracker�����ò���
typedef struct _TRACKER_CFG_PARAM
{
    BOOL fEnableVote;	// ͶƱ����
    BOOL fAlpha5;		// ��5λ��ĸʶ�𿪹�
    BOOL fAlpha6;		// ��6λ��ĸʶ�𿪹�
    BOOL fAlpha7;		// ��7λ��ĸʶ�𿪹�
    BOOL fEnableRecgCarColor;       // ������ɫʶ�𿪹�
    BOOL fEnableFlashLight; //ץ�Ĳ����ģʽ����
    BOOL fDoublePlateEnable;//˫���Ƽ�⿪��
    BOOL fUseEdgeMethod; // ��ǿ����ͼʶ��
    BOOL fEnableT1Model; // T-1ģ�Ϳ���
    BOOL fEnableDefaultWJ; // �������侯�ַ�����
    BOOL fEnableAlpha_5; // ������ĸʶ�𿪹�
    BOOL fOutputCarSize; // ��������ߴ�
    BOOL fOnePlateMode; // ��ⵥ����
    BOOL fEnableDetFace; // ������⿪��

    float fltMinConfForOutput;      // ���Ŷ���ֵ
    float fltPlateDetect_StepAdj;   // ��������ϵ��
    float fltOverLineSensitivity;   // ѹ�߼��������

    int nFirstPlatePos;             // ���ȳ��Ʒ�λ
    int nPlateEnhanceThreshold;     // ������ǿ��ֵ
    int nMiddleWidth;   // ��Ļ�м䵥�����ƺ����ֿ��
    int nBottomWidth;   // ��Ļ�ײ��������ƺ����ֿ��
    BOOL fProcessPlate_BlackPlate_Enable; //ʹ�ܺ����жϹ���(���ƿɱ�����)
    int nProcessPlate_BlackPlate_S; // ���Ƶı��Ͷ�����
    int nProcessPlate_BlackPlate_L; // ������������
    int nProcessPlate_BlackPlateThreshold_H0; // ����ɫ������
    int nProcessPlate_BlackPlateThreshold_H1; // ����ɫ������
    int nProcessPlate_LightBlue;    // ǳ���ƿ���
    int nPlateDetect_Green; // ����ʶ�𿪹�
    int nNightThreshold; // ҹ��ģʽ��������
    int nPlateLightCheckCount; // ���ȵ������
    int nMinPlateBrightness; // �����������
    int nMaxPlateBrightness; // �����������
    int nCtrlCpl; // ����ƫ�⾵
    int nLightTypeCpl; // ƫ�⾵�������ȵȼ�
    int nEnablePlateEnhance; // �ָ�ǰ�Ƿ����ͼƬ��ǿ
    int nPlateResizeThreshold; // �ָ�ǰСͼ��������ֵ
    int nEnableBigPlate; // ����ʶ�𿪹�
    int nRecogAsteriskThreshold; //
    int iEddyType;          // ͼƬ��ת(0:����ת,1:��ʱ����ת90��)
    int nRoadLineNumber;    // ����������
    int nOutputRoadTypeName;//���������������
    int nCaptureOnePos;     //ץ�ĵ�һ��ͼ��λ��
    int nCaptureTwoPos;     //ץ�ĵڶ���ͼ��λ��
    int nCaptureThreePos;   //ץ�ĵ�����ͼ��λ��
    int nDetReverseRunEnable;   // ������п���
    int nSpan;  //�����������г������
    int iRoadNumberBegin;   //������ŵķ���0:����ʼ,1:���ҿ�ʼ
    int iStartRoadNum;  //��ʼ������
    int nFlashLightThreshold;   //ץ�Ĳ���������ֵ
    int nRecogGxPolice; // �ط��侯��ʶ�𿪹�
	int nRecogNewMilitary;//�¾���ʶ�𿪹�
    int nCarArrivedPos; // ��������λ�ã��������⣩
    int nCarArrivedPosNoPlate; // ���Ƴ�����λ��
    int nCarArrivedPosYellow; //��������λ�ã����ƣ�
    int nProcessType; // ��������
    int nOutPutType; // �������
    int nNightPlus; // ���ϼ�ǿ�ǻ��������
    int nWalkManSpeed; // �����ж�������
    int nCarArrivedDelay; // �������ﴥ���ӳپ���(��)
    int nVoteFrameNum; // ͶƱ�����
    int nMaxEqualFrameNumForVote; // ������ͬ�����������
    int nBlockTwinsTimeout; // ��ͬ�����Сʱ����(S)
    int nAverageConfidenceQuan; // ƽ���÷�����
    int nFirstConfidenceQuan; // ���ֵ÷�����
    int nRemoveLowConfForVote; // ͶƱǰȥ���͵÷ֳ��ưٷֱ�
    int nVoteCount; // ͶƱ��
    int nSegHeight; // �ָ�ο��߶�
    int nDoublePlateTypeEnable; // Ĭ��˫�������Ϳ���
    int nDefaultDBType; // Ĭ��˫��������
    int nMinFaceScale; // ��С�������
    int nMaxFaceScale; // ����������
    float fltBikeSensitivity; // �ǻ�����������
    char szDefaultWJChar[4]; // �������侯�ַ�

    int nSegWhitePlate; //����ǿ�Ʒָ�

    int nCarArriveTrig; //�Ƿ񴥷�ץ��
	int nTriggerPlateType;//����ץ�ĵĳ�������
    int nEnableProcessBWPlate;//�Ƿ����ڰ����

    ROAD_INFO rgcRoadInfo[MAX_ROADLINE_NUM];    //��������Ϣ
    ACTIONDETECT_PARAM_TRACKER cActionDetect;   //�¼�������
    DETECTAREA cDetectArea;	// ɨ������
	TRAP_DETAREA cTrapArea; //����ɨ������
    DETECTAREA cRecogSnapArea; // ץ��ͼʶ�����
    TRAFFICLIGHT_CFG_PARAM cTrafficLight;   //���̵Ʋ���
    SCALE_SPEED_PARAM cScaleSpeed;  //������ٲ���
    VIDEODET_CFG_PARAM cVideoDet;   //��Ƶ������

	DSPLinkBuffer cTriggerInfoBuf;    //���津��ץ��ʱ����Ϣ
	int iPlatform;        				//ƽ̨ 0:һ��� 1:����
	int iSpeedLimit;      				//����ֵ
	int iCapMode;    				//ץ��ģʽ (0:ץ��һ��; 1:����Υ��ץ��2��; 2:ȫ��ץ��2��)
	BOOL fPreVote;
	//������ɫʶ�����
	int nEnableColorReRecog;
	int nWGColorReThr;
	int nWColorReThr;
	int nBColorReThr;
	BOOL fEnableCorlorThr;
	int nBGColorThr;
	int nWGColorThr;

    //���²���Ϊ��Ƶ���շ�վר��
    int iOutPutNoPlate; //�Ƿ�������Ƴ�
    int iScanAreaTopOffset;  // ɨ���������봥���߰ٷֱ�
    int iScanAreaBottomOffset; // ɨ�����ײ��봥���߰ٷֱ�
    int iBuleTriggerToYellowOffset; // ���ƴ����ߵ����ƴ����ߵİٷֱ�
    int iPrevTime; //��ǰƥ��ʱ��
    int iPostTime; //���ƥ��ʱ��
    int iForceLightOffAtDay;
	int iForceLightOffAtNight;
	int iDisableFlashAtDay;
	int iForceLightThreshold;

	int iMovingDirection; //��ʻ����
    BOOL fFilterReverseEnable;        //�������г�����־
    int nRecogInVaildRatioTH;         // ʶ����Ч����������ֵ���ٷֱ�*100��0~100�����ڸðٷֱ��򲻳����

    int iUsedLight; // �Ƿ�ʹ��Ƶ�������
    
    //�շ�վֹͣ������ƫ�ƣ�ֹͣ�����ڷǻ��ƴ���������ƫ��m_nStopArriveLineOffset����
    //��ֱ�ӳ��ƣ������Զ�������ƫ��10��
    int nStopArriveLineOffset;

    //�շ�վ��ͶƱ�����С����֡�����ﲻ���򲻳����
    int nTollOutResultMinSimiFrame;

    int nPlateTrackerOutLine;

    int nMaxAGCTH;
	int iImageConstDelay;		//ͼ����ʱ����ʱ��

	int nDuskMaxLightTH;
	int nNightMaxLightTH;

    _TRACKER_CFG_PARAM()
    {
        fEnableVote = TRUE;
        fAlpha5 = FALSE;
        fAlpha6 = FALSE;
        fAlpha7 = FALSE;
        fEnableRecgCarColor = FALSE;
        fEnableFlashLight = FALSE;
        fltMinConfForOutput = 0.7f;
        nPlateEnhanceThreshold = 40;
        fProcessPlate_BlackPlate_Enable = FALSE;
        nProcessPlate_BlackPlate_S = 0;
        nProcessPlate_BlackPlate_L = 5;
        nProcessPlate_BlackPlateThreshold_H0 = 5;
        nProcessPlate_BlackPlateThreshold_H1 = 230;
        nProcessPlate_LightBlue = 0;
        nPlateDetect_Green = 0;
        nNightThreshold = 55;
        nPlateLightCheckCount = 5;
        nMinPlateBrightness = 80;
        nMaxPlateBrightness = 120;
        nCtrlCpl = 0;
        nLightTypeCpl = ((int)LIGHT_TYPE_COUNT / 2);
        nEnablePlateEnhance = 0;
        nPlateResizeThreshold = 120;
        nEnableBigPlate = 0;
        nFirstPlatePos = 0;
        fltPlateDetect_StepAdj = 1.5f;
        nMiddleWidth = 70;
        nBottomWidth = 90;
        iEddyType = 0;
        fltOverLineSensitivity = 2.75f;
        nRoadLineNumber = 4;
        nOutputRoadTypeName = 0;
        nCaptureOnePos = 0;
        nCaptureTwoPos = 0;
        nCaptureThreePos = 0;
        cActionDetect.iDetectCarStopTime = 10;
        nDetReverseRunEnable = 1;
        nSpan = 5;
        iRoadNumberBegin = 0;
        iStartRoadNum = 1;
        nFlashLightThreshold = 25;
        fDoublePlateEnable = 1;
        nRecogAsteriskThreshold = 0;
        fUseEdgeMethod = FALSE;
        fEnableT1Model = FALSE;
        fEnableDefaultWJ = FALSE;
        nRecogGxPolice = 0;
		nRecogNewMilitary = 0;
        fEnableAlpha_5 = FALSE;
        nCarArrivedPos = 50;
        nCarArrivedPosNoPlate = 50;
        nCarArrivedPosYellow = 40;
        nProcessType = 0;
        nOutPutType = 0;
        nNightPlus = 0;
        nWalkManSpeed = 100;
        fltBikeSensitivity = 4.0f;
        nCarArrivedDelay = 0;
        nVoteFrameNum = 35;
        nMaxEqualFrameNumForVote = 15;
        nBlockTwinsTimeout = 30;
        nFirstConfidenceQuan = 0;
        nRemoveLowConfForVote = 40;
        nAverageConfidenceQuan = 0;
        fOutputCarSize = FALSE;
        memset(szDefaultWJChar, 0, 4);
        memcpy(szDefaultWJChar, "16", 2);
        nVoteCount = 3;
        fOnePlateMode = TRUE;
        nSegHeight = 18;
        nDoublePlateTypeEnable = 0;
        nDefaultDBType = 0;

        nSegWhitePlate = 0;

        nCarArriveTrig = 1;
        nTriggerPlateType = 0;
        nEnableProcessBWPlate = 0;

		iPlatform = 0;
		iSpeedLimit = 0;
		iCapMode = 0;

		iOutPutNoPlate = 0;
		iScanAreaTopOffset = 28;
		iScanAreaBottomOffset = 12;
		iBuleTriggerToYellowOffset = 8;
        iPrevTime = 5000;
        iPostTime = 0;

		fPreVote = TRUE;
        fEnableDetFace = FALSE;
        nMinFaceScale = 2;
        nMaxFaceScale = 10;
		//������ɫʶ�����
		nEnableColorReRecog = 0;
		nWGColorReThr = 60;
		nWColorReThr = 150;
		nBColorReThr = 20;
		fEnableCorlorThr = false;
		nBGColorThr = 25;
		nWGColorThr = 150;

        nRecogInVaildRatioTH = 40;
        for (int i = 0; i < MAX_ROADLINE_NUM; i++)
        {
            memset(&rgcRoadInfo[i], 0, sizeof(ROAD_INFO));
            strcpy(rgcRoadInfo[i].szRoadTypeName, "NULL");
            rgcRoadInfo[i].iLineType = 1;
            rgcRoadInfo[i].iRoadType = (int)(RRT_FORWARD | RRT_LEFT | RRT_RIGHT | RRT_TURN);
            cActionDetect.iIsYellowLine[i] = 0;
            cActionDetect.iIsCrossLine[i] = 0;
        }
        iForceLightOffAtDay = 0;
		iForceLightOffAtNight = 0;
		iDisableFlashAtDay = 0;
		iForceLightThreshold = 100;

		iMovingDirection = 4;
        fFilterReverseEnable = false;

        iUsedLight = 1; // �Ƿ�ʹ��Ƶ�������

        nStopArriveLineOffset = 10;

        nTollOutResultMinSimiFrame = 5;

        nPlateTrackerOutLine = 20;

        nMaxAGCTH = 150;
		iImageConstDelay = 0;

		nDuskMaxLightTH=60;
		nNightMaxLightTH=13;
    }
} TRACKER_CFG_PARAM;

extern CORE_PERFORMANCE_TEST_DATA g_CorePerformanceTestData;

typedef struct _TRIGGER_CAM_INFO
{
    DWORD32     dwCarArriveTime;	//����ʱ��Tick
	DWORD32     dwRoadId;          //����
	DWORD32     dwTriggerIndex;   //�������

	_TRIGGER_CAM_INFO()
	{
		memset(this, 0, sizeof(*this));
	}
} TRIGGER_CAM_INFO;


#endif // _TRACKERCALLBACK_INCLUDED__
