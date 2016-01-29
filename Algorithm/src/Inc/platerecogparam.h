// ���ļ����������WINDOWS-936��ʽ

#ifndef _PARAMETER_INCLUDED__
#define _PARAMETER_INCLUDED__

#include "swbasetype.h"
#include "swimage.h"
#include "swimageobj.h"

#define CHN_CHAR_SET_LIST(MACRO_NAME)			\
	MACRO_NAME(FULLNN, "ȫ����ģ��")

#define DEFINE_CCS_ENUM(a, b) CCS_##a,

enum ChnCharSet
{
	CHN_CHAR_SET_LIST(DEFINE_CCS_ENUM)
	CCS_MaxSet
};

enum CarOrientation
{
	CO_HEAD=0,
	CO_TAIL,
	CO_COUNT
};

#define RECOG_MODE_LIST(MACRO_NAME)									\
	MACRO_NAME(TOLLGATE, "�շ�վ")									\
	MACRO_NAME(HIGHWAY_HEAD, "���� - ��ͷ")							\
	MACRO_NAME(HIGHWAY_TAIL, "���� - ��β")							\
	MACRO_NAME(PATROL_LEFT, "�ƶ����� - ���")						\
	MACRO_NAME(PATROL_RIGHT, "�ƶ����� - �Ҳ�")						\
	MACRO_NAME(PORTABLE, "������")									\
	MACRO_NAME(ELECTRONIC_POLICE, "���Ӿ���")						\
	MACRO_NAME(TRAFFICE_EVENT_HEAD, "��ͨ�¼����-��ͷ")			\
	MACRO_NAME(TRAFFICE_EVENT_TAIL, "��ͨ�¼����-��β")			\
	MACRO_NAME(HVC,	"ץ��ʶ��")					                    \
	MACRO_NAME(PARK, "ͣ����˫·")				                    \
	MACRO_NAME(PARK_SINGLE, "ͣ������·")				            \
	MACRO_NAME(CAP_FACE, "��������ץ��")							\
    MACRO_NAME(COVER_CTRL, "���������")						    \

#define DEFINE_PRM_ENUM(a, b) PRM_##a,

enum PlateRecogMode
{
	RECOG_MODE_LIST(DEFINE_PRM_ENUM)
	PRM_MaxMode
};

// ����ʶ���Ҫ��,���ٺ�һ��
enum PlateRecogSpeed
{
	PR_SPEED_NORMAL = 0,	// ���ڳ���
	PR_SPEED_FAST			// ·���ƶ�����
};

// �����˶�����
enum MovingDirection
{
	MD_TOP2BOTTOM = 0,		// ��Զ����
	MD_BOTTOM2TOP,			// �ɽ���Զ
	MD_LEFT2RIGHT,			// ������
	MD_RIGHT2LEFT,			// ���Ҽ���
	MD_NOTSURE,			    // ���ⷽ��
	MD_COUNT
};

template<BYTE8 QuanBit>
class CQuanNum
{
	static const DWORD32 QuanScale = (1 << QuanBit);

	SDWORD32 m_dwQuan;

	//friend CQuanNum<QuanBit> operator*(int i, const CQuanNum<QuanBit>& a);

public:
	CQuanNum()
	{
		m_dwQuan = 0;
	}
	CQuanNum(const CQuanNum& a)
	{
		m_dwQuan = a.m_dwQuan;
	}
	CQuanNum(float f)
	{
		m_dwQuan = (SDWORD32)(f * QuanScale+0.5);
	}
	CQuanNum& operator=(const CQuanNum& a)
	{
		m_dwQuan = a.m_dwQuan;
		return *this;
	}
	CQuanNum& operator=(float f)
	{
		m_dwQuan = (SDWORD32)(f * QuanScale+0.5);
		return *this;
	}
	CQuanNum operator*(int i) const
	{
		CQuanNum q;
		q.m_dwQuan = m_dwQuan * i;
		return q;
	}
	CQuanNum& operator*=(int i)
	{
		m_dwQuan *= i;
		return *this;
	}
	CQuanNum& operator*=(float i)
	{
		CQuanNum a(i);
		(*this) *= a;
		return *this;
	}
	CQuanNum& operator/=(int i)
	{
		m_dwQuan /= i;
		return *this;
	}
	CQuanNum operator+(const CQuanNum& a) const
	{
		CQuanNum q;
		q.m_dwQuan = m_dwQuan + a.m_dwQuan;
		return q;
	}
	CQuanNum operator-(const CQuanNum& a) const
	{
		CQuanNum q;
		q.m_dwQuan = m_dwQuan - a.m_dwQuan;
		return q;
	}
	CQuanNum& operator+=(const CQuanNum& a)
	{
		m_dwQuan += a.m_dwQuan;
		return *this;
	}
	CQuanNum operator-=(const CQuanNum& a)
	{
		m_dwQuan -= a.m_dwQuan;
		return *this;
	}
	CQuanNum operator*(const CQuanNum& a) const
	{
		// TODO: May overflow
		CQuanNum q;
		q.m_dwQuan = (m_dwQuan * a.m_dwQuan) >> QuanBit;
		return q;
	}
	CQuanNum& operator*=(const CQuanNum& a)
	{
		// TODO: May overflow
		m_dwQuan = (m_dwQuan * a.m_dwQuan) >> QuanBit;
		return *this;
	}
	bool operator<(const CQuanNum& b) const
	{
		return (m_dwQuan<b.m_dwQuan);
	}
	bool operator>(const CQuanNum& b) const
	{
		return (m_dwQuan>b.m_dwQuan);
	}

	// Converting to build-in types
	int ToInt() const
	{
		return (m_dwQuan >> QuanBit);
	}
	float ToFloat() const
	{
		return (float)m_dwQuan / QuanScale;
	}
};

typedef CQuanNum<13> CParamQuanNum;

/*
template<BYTE8 BitLen> CQuanNum<BitLen> operator*(int i, const CQuanNum<BitLen>& a);
static CQuanNum<13> operator*(int i, const CQuanNum<13>& a)
{
	CQuanNum<13> q;
	q.m_dwQuan = a.m_dwQuan * i;
	return q;
}
*/

//����汾�����������ʹ�õĲ����ṹ
class PlateRecogParam_High
{
public:
	// Ӧ�ó���
	PlateRecogMode g_PlateRcogMode;
	PlateRecogSpeed g_PlateRecogSpeed;
	MovingDirection	g_MovingDirection;	// �����˶�����

	// for tracking
	HV_RECT g_rectDetectionArea;	// ������ʱ�ļ������, percentage
	HV_RECT g_rectTrackArea;	    // track�뿪���������ٽ�������, percentage

	CParamQuanNum g_fltTrackInflateX;		// trackingʱ��һ֡������X�����ϵ����������trackrect_width�ı���
	CParamQuanNum g_fltTrackInflateY;		// trackingʱ��һ֡������Y�����ϵ����������trackrect_height�ı���

	int g_nContFrames_EstablishTrack;		// of continuously observed frames for establishing a track
	int g_nMissFrames_EndTrack;				// of missing frames for ending a track
	int g_nMissFrames_EndTrackQuick;		// of missing frames for ending a track in EndTrackArea
	CParamQuanNum g_fltThreshold_StartTrack;		// threshold of a good recognition for starting a track
	CParamQuanNum g_fltThreshold_BeObservation;	    // threshold of a good recognition to be an observation

	// for plate detection
	int g_nDetMinScaleNum;			// ���Ƽ������С���=56*1.1^g_nDetMinScaleNum
	int g_nDetMaxScaleNum;			// ���Ƽ���������=56*1.1^g_nDetMaxScaleNum
	int g_nDetMinStdVar;			// ����>g_nDetMinStdVar���������Ϊ�����ǳ���
	int	g_nMinStopPlateDetNum;		// �ﵽ���������ֹͣ���
	bool g_fSubscanPredict;			// trackingʱ�Ƿ�ʹ��Ԥ���㷨
	int g_nSubscanPredictMinScale;	// trackingԤ��ʱscale�仯��Χ����Сֵ
	int g_nSubscanPredictMaxScale;	// trackingԤ��ʱscale�仯��Χ�����ֵ

	CParamQuanNum g_kfltPlateInflateRate;	// ��������ı���
	CParamQuanNum g_kfltPlateInflateRateV;	// �������ߵı���
	CParamQuanNum g_kfltHighInflateArea;	// ���Ƶ�yֵ>g_kfltHighInflateArea���õڶ��׸���һ��ı������ͼ���
	CParamQuanNum g_kfltPlateInflateRate2;	// ����ļ�������ı���
	CParamQuanNum g_kfltPlateInflateRateV2;	// ����ļ������ߵı���

	bool g_fSegPredict;				// �Ƿ�ʹ���зֵ�Ԥ���㷨

	int g_nMinPlateWidth;			// ����ʶ�����С���ƿ�ȣ��Էָ��Ľ��Ϊ׼��
	int g_nMaxPlateWidth;			// ����ʶ�������ƿ�ȣ��Էָ��Ľ��Ϊ׼��
	bool g_fBestResultOnly;			// ���ͼ�����ж��ʶ������ֻ�����õ�

	int g_nMinDownSampleWidthInSeg;	    // �ָ�ʱDownSample����С���
	int g_nMinDownSampleHeightInSeg;   // �ָ�ʱDownSample����С�߶�

	HV_RECT g_rectDetectionArea_DBL;
	int g_nDBLDetMinScaleNum_DBL;
	int g_nDBLDetMaxScaleNum_DBL;
};

class PlateRecogParam
{
public:
	// Ӧ�ó���
	PlateRecogMode g_PlateRcogMode;
	PlateRecogSpeed g_PlateRecogSpeed;
	MovingDirection	g_MovingDirection;	// �����˶�����

	// for tracking
	int g_cnDetStep;				// �Ƿ�ÿ֡���������⣬���Ǹ�g_cnDetStep��һ��
	HV_RECT g_rectDetectionArea;	// ������ʱ�ļ������, percentage
	HV_RECT g_rectTrackArea;	    // track�뿪���������ٽ�������, percentage

	CParamQuanNum g_fltTrackInflateX;		// trackingʱ��һ֡������X�����ϵ����������trackrect_width�ı���
	CParamQuanNum g_fltTrackInflateY;		// trackingʱ��һ֡������Y�����ϵ����������trackrect_height�ı���

	int g_nContFrames_EstablishTrack;		        // of continuously observed frames for establishing a track
	int g_nMissFrames_EndTrack;				        // of missing frames for ending a track
	int g_nMissFrames_EndTrackQuick;		        // of missing frames for ending a track in EndTrackArea
	int g_nObservedFrames_ConsiderAsTrack;	        // of observed frames to be considered as a successful track
	CParamQuanNum g_fltThreshold_StartTrack;		// threshold of a good recognition for starting a track
	CParamQuanNum g_fltThreshold_BeObservation;	    // threshold of a good recognition to be an observation
	CParamQuanNum g_fltMinConfidenceForVote;		// minimal confidence for voting, when there are not enought observations

	// for plate detection
	int g_nDetMinScaleNum;			// ���Ƽ������С���=56*1.1^g_nDetMinScaleNum
	int g_nDetMaxScaleNum;			// ���Ƽ���������=56*1.1^g_nDetMaxScaleNum
	int g_nDetMinStdVar;			// ����>g_nDetMinStdVar���������Ϊ�����ǳ���
	int	g_nMinStopPlateDetNum;		// �ﵽ���������ֹͣ���
	bool g_fSubscanPredict;			// trackingʱ�Ƿ�ʹ��Ԥ���㷨
	int g_nSubscanPredictMinScale;	// trackingԤ��ʱscale�仯��Χ����Сֵ
	int g_nSubscanPredictMaxScale;	// trackingԤ��ʱscale�仯��Χ�����ֵ

	CParamQuanNum g_kfltPlateInflateRate;	// ��������ı���
	CParamQuanNum g_kfltPlateInflateRateV;	// �������ߵı���
	CParamQuanNum g_kfltHighInflateArea;	// ���Ƶ�yֵ>g_kfltHighInflateArea���õڶ��׸���һ��ı������ͼ���
	CParamQuanNum g_kfltPlateInflateRate2;	// ����ļ�������ı���
	CParamQuanNum g_kfltPlateInflateRateV2;	// ����ļ������ߵı���

	bool g_fSegPredict;				// �Ƿ�ʹ���зֵ�Ԥ���㷨

	int g_nMinPlateWidth;			// ����ʶ�����С���ƿ�ȣ��Էָ��Ľ��Ϊ׼��
	int g_nMaxPlateWidth;			// ����ʶ�������ƿ�ȣ��Էָ��Ľ��Ϊ׼��
	bool g_fBestResultOnly;			// ���ͼ�����ж��ʶ������ֻ�����õ�

	int g_nMinDownSampleWidthInSeg;	// �ָ�ʱDownSample����С���
	int g_nMinDownSampleHeightInSeg;// �ָ�ʱDownSample����С�߶�

	int m_fUsedTrafficLight;		//���Ӿ��쿪��
	int m_fUseTemplet;				// ģ��ƥ�俪��

	int m_iDetectCarStopEnable;		    //����ֹͣ��⿪��
	int m_iDetectReverseEnable;			//�������м�⿪��
	int m_iDetectOverYellowLineEnable;	//����ѹ���߼�⿪��
	int m_iDetectCrossLineEnable;		//�������߼�⿪��
	PlateRecogParam &operator=(const PlateRecogParam_High &param)
	{
		g_PlateRcogMode = param.g_PlateRcogMode;
		g_PlateRecogSpeed = param.g_PlateRecogSpeed;
		g_MovingDirection = param.g_MovingDirection;

		g_cnDetStep = 0;
		g_rectDetectionArea = param.g_rectDetectionArea;
		g_rectTrackArea = param.g_rectTrackArea;

		g_fltTrackInflateX = param.g_fltTrackInflateX;
		g_fltTrackInflateY = param.g_fltTrackInflateY;

		g_nContFrames_EstablishTrack = param.g_nContFrames_EstablishTrack;
		g_nMissFrames_EndTrack = param.g_nMissFrames_EndTrack;
		g_nMissFrames_EndTrackQuick = param.g_nMissFrames_EndTrackQuick;
		g_nObservedFrames_ConsiderAsTrack = 0;
		g_fltThreshold_StartTrack = param.g_fltThreshold_StartTrack;
		g_fltThreshold_BeObservation = param.g_fltThreshold_BeObservation;
		g_fltMinConfidenceForVote = 0.0f;

		g_nDetMinScaleNum = param.g_nDetMinScaleNum;
		g_nDetMaxScaleNum = param.g_nDetMaxScaleNum;
		g_nDetMinStdVar = param.g_nDetMinStdVar;
		g_nMinStopPlateDetNum = param.g_nMinStopPlateDetNum;
		g_fSubscanPredict = param.g_fSubscanPredict;
		g_nSubscanPredictMinScale = param.g_nSubscanPredictMinScale;
		g_nSubscanPredictMaxScale = param.g_nSubscanPredictMaxScale;

		g_kfltPlateInflateRate = param.g_kfltPlateInflateRate;
		g_kfltPlateInflateRateV = param.g_kfltPlateInflateRateV;
		g_kfltHighInflateArea = param.g_kfltHighInflateArea;
		g_kfltPlateInflateRate2 = param.g_kfltPlateInflateRate2;
		g_kfltPlateInflateRateV2 = param.g_kfltPlateInflateRateV2;

		g_fSegPredict = param.g_fSegPredict;

		g_nMinPlateWidth = param.g_nMinPlateWidth;
		g_nMaxPlateWidth = param.g_nMaxPlateWidth;
		g_fBestResultOnly = param.g_fBestResultOnly;

		g_nMinDownSampleWidthInSeg = param.g_nMinDownSampleWidthInSeg;
		g_nMinDownSampleHeightInSeg = param.g_nMinDownSampleHeightInSeg;

		m_fUsedTrafficLight = 0;
		m_fUseTemplet = 0;

		m_iDetectCarStopEnable = 0;
		m_iDetectReverseEnable = 0;
		m_iDetectOverYellowLineEnable = 0;
		m_iDetectCrossLineEnable = 0;

		return *this;
	}
};

extern "C" HRESULT GetPlateRecogParamCount(PDWORD32 pdwCount);
HRESULT LoadPlateRecogParam(DWORD32 dwIndex);
int GetRecogParamIndexOnName(LPCSTR lpszName);
HRESULT GetRecogParamNameOnIndex(DWORD32 dwIndex, char *pszName);
extern "C" PlateRecogParam* GetCurrentParam();
extern "C" PlateRecogMode GetCurrentMode();

extern "C" PlateRecogParam g_rgPlateRecogParam[PRM_MaxMode];

#if RUN_PLATFORM == PLATFORM_DSP_BIOS
LPCSTR GetPlateRecogParamName(DWORD32 dwIndex);
#else
HRESULT GetPlateRecogParamName(LPWSTR lpszName, DWORD32 dwBufLen, DWORD32 dwIndex);
#endif

extern "C" DWORD32 GetChnCharSetCount();
LPCSTR GetChnCharSetName( DWORD32 dwIndex );

//�������������ļ����к�
#define SERIAL_CONFIG		100

#endif // _PARAMETER_INCLUDED__
