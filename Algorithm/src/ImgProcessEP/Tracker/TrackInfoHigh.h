#ifndef _TRACKINFO_INCLUDED__
#define _TRACKINFO_INCLUDED__

#include <string.h>

#include "swimage.h"
#include "hvbaseobj.h"
#include "swplate.h"
#include "swimageobj.h"
#include "platerecogparam.h"

#include "DspLinkCmd.h"

#include "ItgArea.h"

#include "ObjMatch.h"
#include "HvImageNew.h"
#include "ScaleSpeed.h"


extern int GetAllPoints(HV_POINT* pAll, int iY, int *iPointCount);
extern BOOL IsOutSideRoad(int iX, int iY);


/**
*  Tracker state define
*/
enum TRACK_STATE
{
	tsInit = 0,
	tsNew,
	tsMoving,
	tsEnd,
	tsWaitRemove,
	tsTemTrack,				// ģ�����״̬
	tsTemTrackEnd			// ģ����ٽ���״̬
};

// �����ӳ�����Ϣ����
enum PLATEINFO_TYPE
{
	PI_LPR = 0,				// ����ʶ������
	PI_TEM					// ģ��ƥ������
};

struct CarSizeInfo
{
	HiVideo::CRect rcFG;		// ������Χ����(������Ƶ���)
	float fltCarW;				// ����
	float fltCarH;				// ����
};

typedef struct _CARCOLOR_INFO
{
	DWORD32 dwCarColorType;
	char szName[24];
	DWORD32 dwCarColorID;
} CARCOLOR_INFO;

extern _CARCOLOR_INFO g_rgCarColorInfo[];
#define CARCOLOR_NUM		ARRSIZE(g_rgCarColorInfo)

struct BestPlateInfo
{
	BestPlateInfo()
	{
		pimgPlate = NULL;
		fltConfidence = 0;
	}
	~BestPlateInfo()
	{
	}
	void Clear()
	{
		fltConfidence = 0;
		if (pimgPlate != NULL)
		{
			pimgPlate->Release();
			pimgPlate = NULL;
		}
	}
	HRESULT Detach(BestPlateInfo& dst)
	{
		dst = *this;
		pimgPlate = NULL;
		fltConfidence = 0;
		return S_OK;
	}
	IReferenceComponentImage *pimgPlate;
	CParamQuanNum fltConfidence;
	HiVideo::CRect rcPlatePos;				// �ָ�����Χλ��
	DWORD32 dwFrameTime;					// ��ǰ֡ʱ��
	HiVideo::CRect rcPos;					// ����������ͼ���е�λ��
};

struct PlateInfo
{
	PlateInfo()
		: color(PC_UNKNOWN)
		, nPlateType(PLATE_UNKNOWN)
		, iLastCharHeight(-1)
		, iLastVscale(-1)
		, fltTotalConfidence(0)
		, nVariance(0)
		, nAvgY(0)
		, fValid(true)
		, nInfoType(PI_LPR)			// Ĭ���ǳ���ʶ������
		, nCarType(CT_UNKNOWN)		// ��������δ֪
		, nCarColour(CC_UNKNOWN)
		, fReRecog(true)
		, iCarColor(0)
	{
		memset(rgfltConfidence, 0, sizeof rgfltConfidence);
		memset(rgbContent, 0, sizeof rgbContent);
		memset(&rcPos, 0, sizeof rcPos);
	};
	PlateInfo(HiVideo::CRect& rc)
		: color(PC_UNKNOWN)
		, nPlateType(PLATE_UNKNOWN)
		, iLastCharHeight(-1)
		, iLastVscale(-1)
		, fltTotalConfidence(0)
		, rcPos(rc)
		, nVariance(0)
		, nAvgY(0)
		, fValid(true)
		, nInfoType(PI_LPR)			// Ĭ���ǳ���ʶ������
		, nCarType(CT_UNKNOWN)		// ��������δ֪
		, nCarColour(CC_UNKNOWN)
		, fReRecog(true)
		, iCarColor(0)
	{
		memset(rgfltConfidence, 0, sizeof rgfltConfidence);
		memset(rgbContent, 0, sizeof rgbContent);
		memset(&stCarInfo, 0, sizeof stCarInfo);
	};

	HRESULT GetPlateName(PWORD16 pwszName) const
	{
		return ::GetPlateNameUnicode(pwszName, nPlateType, color, rgbContent);
	};
	HRESULT GetPlateNameAlpha(char* szName) const
	{
		return ::GetPlateNameAlpha(szName, nPlateType, color, rgbContent);
	};
	int Similarity(const PlateInfo &plateinfo)
	{
		if (nInfoType == PI_TEM)
		{
			return 0;
		}
		int iScore = 0;
		int iCharNum(7);
		if (PC_GREEN == plateinfo.color)
		{
			iCharNum = 8;
		}
		for (int i = 0; i < iCharNum; i++)
		{
			if (rgbContent[i] == plateinfo.rgbContent[i])
			{
				iScore++;
			}
		}
		return iScore;
	};
	HiVideo::CRect rcPos;			// ����������ͼ���е�λ��
	CarSizeInfo stCarInfo;			// ������Χ��Ϣ
	PLATE_COLOR color;
	PLATE_TYPE nPlateType;
	int iLastDetWidth;
	int iLastCharHeight;
	int iLastVscale;
	BYTE8 rgbContent[8];			// TODO: Magic Number
	CParamQuanNum rgfltConfidence[8];		// TODO: Magic Number
	CParamQuanNum fltTotalConfidence;
	HiVideo::CRect rcPlatePos;				// �ָ�����Χλ��
	UINT nFrameNo;					// ��ǰ֡��
	DWORD32 dwFrameTime;			// ��ǰ֡ʱ��
	int iHL;						// ����λ������
	bool fValid;					// �Ƿ����ͶƱ
	CAR_TYPE nCarType;				// �������ͣ����С�С��

	DWORD32 nVariance;		//���ƶԱȶ�
	DWORD32 nAvgY;				//��������

	HiVideo::CRect rgrcChar[8];

	PLATEINFO_TYPE nInfoType;		// ������Ϣ����
	CAR_COLOR nCarColour;
	bool fReRecog;
	int iCarColor;                  //1ǳ,2��,0Ĭ�ϲ�����ǳ
};

class CTrackInfoHeader
{
public:
	static const int s_knMaxPlateInfo = 200;

	PlateRecogParam *m_pParam;

	TRACK_STATE m_State;		// one of the four states

	bool m_fObservationExists;	//��ǰ֡�Ƿ���ڳ���
	UINT m_dwPredictTime;
	HiVideo::CRect m_rcPredict;

	bool m_fCarArrived;	//�Ƿ��Ѿ�����CarArrive�ص�
	bool m_fCarLefted; // �Ƿ��Ѿ����ù�CarLeft;
	UINT m_nCarArriveTime; //����CarArrive��ʱ��
	UINT m_nCarArriveRealTime; // CarArriveʵ��ʱ��
	DWORD32 m_dwCarArriveDopeTime;//CarArrive Ԥ��ʱ��

	DWORD32 m_dwLastProcessCarArrvieTime;
	// zhaopy
	DWORD32 m_dwDopeFirstPos;
	DWORD32 m_dwDopeEndPos;
	bool m_fIsTrigger;
	DWORD32 m_dwTriggerIndex;   //ץ������
	int    m_iCapCount;
	int    m_iCapSpeed;

	HiVideo::CRect m_rcCarArrivePos; //CarArriveʱ�ĳ���λ��
	//�����������һ��ͼ������
	HiVideo::CRect m_rcBestPos;
	HiVideo::CRect m_rcLastPos;
	bool m_fVoted;		//ǿ��ͶƱ���Ƿ��Ѿ�ͶƱ

	int m_iVideoID;		//��ǰ������������Ƶ���
	int m_nID;
	int m_nMissingTime;			// the number of frames recently missing
	int m_nObservedFrames;		// the total number of observed frames
	UINT m_nStartFrameNo;
	UINT m_nEndFrameNo;
	int m_nResultEqualTimes;	//���������ͬ�Ĵ���

	int m_nVoteCondition;

	PlateInfo m_rgPlateInfo[s_knMaxPlateInfo];
	int m_cPlateInfo;
	DWORD32 m_dwFirstFlameTime;			// ��һ֡�ĵ�32λʱ��
	bool m_fReverseRun;					// �������б�־
	bool m_fReverseRunVoted;
	int m_iVoteSimilarityNum;

	BYTE8 m_rgbVotedResult[8];
	PLATE_TYPE m_nVotedType;
	PLATE_COLOR m_nVotedColor;

	CParamQuanNum m_fltMaxConfidence;	// track��plate�����confidence

	//����Ԥ��λ�ü���õ��Ļ���ͼ
	CItgArea m_cItgArea;
	bool m_fCarIsStop; // �Ƿ�ֹͣ����־
	int m_iCarStopTick;
	int m_iCarStopPassTime;

	CAR_TYPE m_nVotedCarType;	// ����(����С)
	CAR_COLOR m_nVotedCarColor;

	CObjMatch *m_pObj;			// ģ��ƥ��
	bool m_fCanOutput;			// �Ƿ��������(�Ѿ�ͶƱ�����ŶȲ����ģ����������)
	bool m_fhasTemple;			// ��ǰ�Ƿ���ģ��
	bool m_fTempTrackExists;	// ģ��ƥ�����
	bool m_fTempCloseEdge;		// ģ���Ƿ񿿽���Ե
	int m_nCloseEdgeCount;		// ģ�濿����Ե֡������
	int m_nMinYPos;				// ���̵�״̬�£�ģ����ٵ������СY����
	int m_nMaxYPos;				// ���̵�״̬�£�ģ����ٵ����Y����ƫ��
	int m_nMaxXPos;				// ���̵�״̬�£�ģ����ٵ����X����ƫ��

	//���̵����
	int m_iOnePosLightScene;
	int m_iTwoPosLightScene;
	int m_iThreePosLightScene;
	int m_iPassStopLightScene;
	int m_iPassLeftStopLightScene;
	bool m_fPassStopLine;
	//�Ƿ�Ҫץ��
	bool m_fFirstTrigger;
	bool m_fSecondTrigger;
	bool m_fThirdTrigger;

	//ץ�ĵ�ʱ��
	DWORD32 m_dwFirstTime;
	DWORD32 m_dwSecondTime;
	DWORD32 m_dwThirdTime;

	DWORD32 m_dwFirstTimeMs;
	DWORD32 m_dwSecondTimeMs;
	DWORD32 m_dwThirdTimeMs;

	float m_rgfltFrameSpeed[1000];
	int m_iFrameSpeedCount;

	int m_iFirstType;

	DWORD32 m_dwTriggerTime;	//����Ȧλ�õ�ʱ��

	int m_iRoad;
	//ÿ��ͼƬ������
	HiVideo::CRect m_rgRect[5];
	//���һ��ͼƬ������
	HiVideo::CRect m_rectLastImage;
	CarSizeInfo m_stCarInfo;		// ������Χ��Ϣ(������Ƶ���)	

	bool m_fPredictCarArrive;		//��ǰ֡�Ƿ�����CarArriveԤ��
	// zhaopy
	int m_iCarStopFrame;
	DWORD32 m_dwBestSnapShotRefTime;
	DWORD32 m_dwLastSnapShotRefTime;
	DWORD32 m_dwBeginCaptureRefTime;
	DWORD32 m_dwBestCaptureRefTime;
	DWORD32 m_dwLastCaptureRefTime;

	//zhaopy
	// �ڶ���ץ��
	bool m_fHasTrigger;
	DWORD32 m_dwTriggerTimeMs;
	DWORD32 m_dwTriggerImageTime;
};

class CTrackInfo : public CTrackInfoHeader
{
public:
	CTrackInfo();
	~CTrackInfo();

	HRESULT Detach(CTrackInfo &tiDest, PROCESS_ONE_FRAME_RESPOND* pProcessRespond);

	IReferenceComponentImage *m_pimgBestSnapShot;
	IReferenceComponentImage *m_pimgLastSnapShot;
	IReferenceComponentImage *m_pimgBeginCapture;
	IReferenceComponentImage *m_pimgBestCapture;
	IReferenceComponentImage *m_pimgLastCapture;

	//��ʱͼƬ������
	IReferenceComponentImage *m_pimgBestCaptureTemp;
	HiVideo::CRect m_rectBestCaptureTemp;
	DWORD32 m_dwBestCaptureRefTimeTemp;

	// Ϊÿ�����Ͷ�����һ���÷���ߵĳ���ͼ��
	BestPlateInfo m_rgBestPlateInfo[PLATE_TYPE_COUNT];
	//���洦���ĳ���Сͼ,�������ɶ�ֵ��ͼ weikt 2011-5-3
	BestPlateInfo m_bestPlatetoBinImage;

	IReferenceComponentImage *GetLastPlateImage()
	{
		if (m_cPlateInfo <= 0) return NULL;
		return m_rgBestPlateInfo[LastInfo().nPlateType].pimgPlate;
	}

	void Clear(PROCESS_ONE_FRAME_RESPOND* pProcessRespond);

	inline const PlateInfo& LastInfo() const
	{
		return m_rgPlateInfo[m_cPlateInfo-1];
	}

	inline PlateInfo& LastInfo()
	{
		return m_rgPlateInfo[m_cPlateInfo-1];
	}
	HiVideo::CRect CurrentPosition();
	HiVideo::CRect PredictPosition(UINT nNowTime , BOOL fEnableDetReverseRun, IScaleSpeed *pScaleSpeed);
	CParamQuanNum TrackScore() const;

	// �жϸ�Track�Ƿ�ͶƱ, ������
	bool PrepareOutPutFinalResult(DWORD32 dwCurTick);
	HRESULT NewTrack(
		PROCESS_ONE_FRAME_PARAM* pParam,
		HV_COMPONENT_IMAGE imgPlate,
		PlateInfo plate,
		int iLastStatus,
		int iCurStatus,
		PROCESS_ONE_FRAME_RESPOND* pProcessRespond
	);
	HRESULT UpdatePlateImage(
		HV_COMPONENT_IMAGE imgInputPlate,
		PROCESS_ONE_FRAME_PARAM* pParam,
		int iLastStatus,
		int iCurStatus,
		PROCESS_ONE_FRAME_RESPOND* pProcessRespond
	);
	HRESULT UpdateBestImage(
		IReferenceComponentImage *
		);
	HRESULT Vote(PLATE_TYPE &nVotedType, PLATE_COLOR &nVotedColor, PBYTE8 pbVotedContent);
	HRESULT RemoveInvalidVotes();

	//���ӶԸ��ٽ���ԤͶƱ�Ļ���,����true�������Գ��ƣ�false������û�ﵽ���Ƶ�����
	bool  PreVote(void);

	HRESULT UpdateObjTemple(HV_COMPONENT_IMAGE& img, HiVideo::CRect &rcObj);	// �����������ģ�?

	float AverageConfidence();
	float AverageFirstConfidence();
	CParamQuanNum AveragePosConfidence(int iPos);

	int Similarity(const PlateInfo &plateinfo);
	bool IsTwins(CTrackInfo &trackInfo);
	float AverageStdVar();
	float AveragePlateBrightness();

	//����Ԥ������,�ڽ�ѹʱʹ��
	CItgArea* GetPredictArea(
		DWORD32 dwRefTime,
		const HiVideo::CRect& rcFullSearch,
		const HiVideo::CRect& rcTrackArea,
		BOOL fEnableDetReverseRun,
		IScaleSpeed *pScaleSpeed
		);

public:
	//�����ò���,����һ��Tracker��������
	// ͶƱ��
	static int m_nVoteFrameNum;
	// ���������ͬ, Vote
	static int m_nMaxEqualFrameNumForVote;
	// �Ƿ����ͶƱ��������
	static int m_fOutputVoteCondition;

	//ͶƱʱ��ȥ���÷���͵ĳ���
	static int m_nRemoveLowConfForVote;

	// ʶ��÷ֵ�����ֵ(�ٷֱ�)����*��ʾ
	static int m_nRecogAsteriskThreshold;

	static int m_iBestLightMode; // �Ƿ�ʹ������ͼģʽ����ֵΪ1ʱ�ø����������ͼ����������ͼ
	static int m_iBestLightWaitTime; // ����ͼ�ĵȴ�ʱ��

	//���̵����
	static int m_iCheckType;
	static int m_iAutoScanLight;		//��̬�����̵�λ�ÿ���
	static int m_iSceneCheckMode	;	//�����ļ��ģʽ
	static int m_iCaptureOnePos;
	static int m_iCaptureTwoPos;
	static int m_iCaptureThreePos;
	static int m_iStopLinePos;
	static int m_iLeftStopLinePos;			//��ת��ͣ����λ����ֱ��ͣ���ߵĲ�ֵ.
	static int m_iHeight;
	static int m_iWidth;
	static int m_iRedLightDelay;			//����ж�Υ���ӳ�ʱ��
	// zhaopy
	static int m_iRealForward;
	static int m_iFlashlightMode;
	static int m_iFlashlightThreshold;


	static int m_iSpecificLeft;			//��ǿ��ת�ж�
	static int m_iRunTypeEnable;		//ǿ�������ʻ���ͣ�������ʻ����δ���жϣ��򰴳������ڵĳ������������

	//CarArrive��λ��(����Ȧ��λ��)
	static int m_iCarArrivedPos;

	static int m_iMatchScanSpeed;		// ģ��ƥ��ɨ���ٶ�

	//������Ϣ
	static int m_iRoadNumber;								//����������
	static int m_iRoadNumberBegin;							//���������ʼ����
	static int m_iStartRoadNum;								//��ʼ������
	static const int s_iMaxRoadNumber = 5;				//������������Ϊ5
	static int m_iOutputRoadTypeName;					//�Ƿ������������
	static ROAD_INFO m_roadInfo[s_iMaxRoadNumber];

	//�¼�������
	static ACTIONDETECT_PARAM_TRACKER m_ActionDetectParam;

	// ���̵������ģ��ƥ��λ��
	static int m_nTrafficLightTempleYPos;

	// ������Ŀ���˵����
	static int m_iManRoadNum;

	//��ɫʶ�𿪹�
	static bool m_fEnableRecgCarColor;

	static int  m_nOutputPeccancyType;

	//�ƹ����������ȼ�����˳��,2010-12-07
	static char szPeccancyPriority[255];

	//Τ���� 2011-3-9���Ӻ��̵�ʶ�����	
	static int m_iTrafficLightBrightValue;
	static int m_iTrafficLightThread;
	static int m_iAutoScanThread;

	bool   m_bPreVote;
	//added by liujie 
	//�羯���Ƴ���βģ��ɨ�迪��
	static int m_fEPUseCarTailModel;
	static int m_iEPNoPlateScoreThreld;

};

//���㳵��
extern int MatchRoad(int iX, int iY, int *piRoadWidth = NULL);
extern int LeftOrRight(int iX, int iY, int iLineNum, int* piDistance = NULL);
//ȡ������ɫ���
extern int GetCarColorType( DWORD32 dwCarColorID );

//====================================
/*
extern TRAFFICLIGHT_SCENE g_rgLightScene[MAX_SCENE_COUNT];
extern void ClearLightScene();
extern void SetLightScene(int iPos, TRAFFICLIGHT_SCENE ts);
extern bool GetLightScene(int iPos, TRAFFICLIGHT_SCENE* pts);

extern SCENE_INFO g_rgSceneInfo[MAX_SCENE_COUNT];
extern void ClearSceneInfo();
extern void SetSceneInfo(int iPos, const SCENE_INFO& ts);
extern bool GetSceneInfo(int iPos, SCENE_INFO* pts);

#define MAX_LIGHT_TYPE_COUNT 32
extern TRAFFICLIGHT_TYPE g_rgLightType[MAX_LIGHT_TYPE_COUNT];
extern int g_iLightTypeCount;
extern void ClearTrafficLightType();
extern void SetTrafficLightType(int iTeam, int iPos, _TRAFFICLIGHT_POSITION tpPos, _TRAFFICLIGHT_LIGHT_STATUS tlsStatush);
extern bool GetTrafficLightType(int iTeam, int iPos, TRAFFICLIGHT_TYPE* ltLightType);

#define STATUS_BUF_LEN 100
extern int g_rgSceneStatus[STATUS_BUF_LEN];
extern void SetSceneStatus(int nPos, int nStatus);
extern int GetSceneStatus(int nPos);

#define TS_UNSURE 0
#define TS_GREEN 3
#define TS_YELLOW 5
#define TS_RED 9

#define MAX_TRAFFICLIGHT_COUNT 10	//��������

//���е��鵱ǰ״̬
typedef struct tag_LightTeamStatus
{
	DWORD32 nTeamCount;
	DWORD32 pdwStatus[MAX_TRAFFICLIGHT_COUNT];
	DWORD32 pnLightCount[MAX_TRAFFICLIGHT_COUNT];
	tag_LightTeamStatus()
	{
		memset(pdwStatus, 0, sizeof(DWORD32) * MAX_TRAFFICLIGHT_COUNT);
		memset(pnLightCount, 0, sizeof(DWORD32) * MAX_TRAFFICLIGHT_COUNT);
		nTeamCount = 0;
	}
}
LIGHT_TEAM_STATUS;

//����������״̬�����س���״̬���
extern int TransitionScene(const LIGHT_TEAM_STATUS& ltsInfo, const int& iSceneCount, const int& iLastOkLightStatus, bool fFlag);

class CTrafficLightInfo
{
public:
	enum {MAX_POS_COUNT = 8};	//����λ�û���

public:
	HRESULT UpdateStatus(HV_COMPONENT_IMAGE* pSceneImage);
	HRESULT UpdateStatus2(HV_COMPONENT_IMAGE* pSceneImage);
	
	HRESULT SetRect(const HV_RECT& cRect, int nLightCount, int nType);
	HRESULT SetRect(int nLeft, int nTop, int nRight, int nBottom, int nLightCount, int nType);

public:
	DWORD32 GetLastStatus()
	{
		return m_nLastStatus;
	}

	int GetPosCount()
	{
		return m_nLightCount;
	}

	HV_RECT GetRect()
	{
		return m_rcLight;
	}

	HV_RECT GetLastRect()
	{
		HiVideo::CRect rcTemp = m_rcLastLight;
		rcTemp += m_rcLightInflated.TopLeft();
		return rcTemp;
	}

	HV_RECT GetInflatedRect()
	{
		return m_rcLightInflated;
	}

	HRESULT Reset()
	{
		m_fInit = false;
		HV_RECT rcInit = {0,0,0,0};
		m_rcLight = rcInit;
		m_rcLastLight = rcInit;
		m_nID = 0;
		m_nType = 0;
		m_nLightCount = 0;
		m_nLastStatus = TS_UNSURE;

		return S_OK;
	}

	HRESULT ReScan()
	{
		//�����û��ʼ��
		if( !m_fInit ) return S_FALSE;

		HV_RECT rcInit = {0,0,0,0};
		m_rcLastLight = rcInit;
		m_nLastStatus = TS_UNSURE;
		m_fInit = false;
		return S_OK;
	}

public:
	CTrafficLightInfo()
	{
		m_fInit = false;
		HV_RECT rcInit = {0,0,0,0};
		m_rcLight = rcInit;
		m_rcLastLight = rcInit;
		m_nID = 0;
		m_nType = 0;
		m_nLightCount = 0;
		m_nLastStatus = TS_UNSURE;

		m_pElement = hvCreateStructuringElementEx(
			5,
			5,
			2,
			2,
			HV_SHAPE_RECT);
	}
	~CTrafficLightInfo()
	{
		hvReleaseStructuringElement(&m_pElement);
	}

protected:
	DWORD32 GetLightStatus(HV_COMPONENT_IMAGE imgLight, int nPosCount, int nType);
	DWORD32 GetLightStatus2(HV_COMPONENT_IMAGE imgLight, HiVideo::CRect rcLight);

protected:
	INT m_nID;
	HiVideo::CRect m_rcLight;	//��ʼλ�ÿ�
	HiVideo::CRect m_rcLastLight; //��ǰλ��
	HiVideo::CRect m_rcLightInflated;
	INT m_nType;		//0 = ����, 1 = ����
	INT m_nLightCount;	//����������
	DWORD32 m_nLastStatus;

	HvConvKernel* m_pElement;

	bool m_fInit;

public:
	//��������֡���м�״���������ú��̵ƿ�λ��.
	static const int MAX_ERROR_COUNT = 500;
};
*/
#endif // _TRACKINFO_INCLUDED__
