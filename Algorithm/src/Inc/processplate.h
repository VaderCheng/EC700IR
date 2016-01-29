#ifndef _PROCESSPLATE_INCLUDED__
#define _PROCESSPLATE_INCLUDED__

#include "swobjbase.h"
#include "swimage.h"
#include "hvinterface.h"
#include "swplate.h"
#include "inspector.h"
#include "platedetector.h"
#include "recognition.h"

#define NO_PREDICT_PLATE_TYPE_MODE				4
#define FORWARD_PREDICT_PROCESS_PLATE_MODE		3
#define BACKOFF_PREDICT_PROCESS_PLATE_MODE		1
#define NO_PREDICT_PROCESS_PLATE_MODE			0

// ����ģʽ, SetWorkMode()������nProcMode����ʹ��
#define PROC_PLATE_MODE_DEFAULT 0
#define PROC_PLATE_MODE_IR      1

struct CRectifyInfo
{
	float fltHAngle, fltVAngle;
};

struct CBinaryInfo
{
	int iGrayThreshold;			// ��ֵ����ֵ
	int iBlackOnWhite;			// �׵׺���
	int iMaybeWhitePlate;		// �п����ǰ���
	CBinaryInfo()
		: iGrayThreshold(128)
		, iBlackOnWhite(0)
		, iMaybeWhitePlate(-1)
	{
	}
};

typedef struct _PROCESS_PLATE_CFG_PARAM
{
	BOOL fBlackPlateThreshold_Enable; //�����ж�����ʹ�� (���ƿɱ�����)
	int nBlackPlate_S; // ���Ƶı��Ͷ�����
	int nBlackPlate_L; // ������������
	int nBlackPlateThreshold_H0; // ����ɫ������
	int nBlackPlateThreshold_H1; // ����ɫ������
	int nProcessPlate_LightBlue; // ǳ���ƿ���
	BOOL fEnableDoublePlate; // ˫���ƿ���
	int iSegWhitePlate; //ǿ�ư��Ʒָ�
	BOOL fEnableShiGuanPlate;   // ʹ��ʹ���Ƽ��
	
	//��С��߱�ϵ�����ָ��㷨��Ĭ����С��߱ȳ��Ը�ϵ����Ϊ��С��߱ȼ��㡣
	//Ĭ��1.0��ʹ��Ĭ�Ͽ�߱ȡ���Сʱ�ܹ�֧�ָ�С���ַ���߱ȣ����Ƕȡ���б�ȣ�
	float fltMinWHRatioCoef;
	_PROCESS_PLATE_CFG_PARAM()
	{
		fBlackPlateThreshold_Enable = 0;
		nBlackPlate_S = 10;
		nBlackPlate_L = 85;
		nProcessPlate_LightBlue = 1;
		fEnableDoublePlate = 1;
		nBlackPlateThreshold_H0 = 100;
		nBlackPlateThreshold_H1 = 200;
		iSegWhitePlate = 0;
		fEnableShiGuanPlate = FALSE;
		fltMinWHRatioCoef = 1.f;
	}
} PROCESS_PLATE_CFG_PARAM;

class IProcessPlate
{
public:
	virtual ~IProcessPlate(){};

	STDMETHOD(CalcRectifyInfo)(
		HV_COMPONENT_IMAGE *pImgInput,
		HV_COMPONENT_IMAGE *pImgRectified,
		CRectifyInfo *pRectifyInfo,
		CBinaryInfo *pBinaryInfo,
		PLATE_TYPE *pPlateType,
		PLATE_COLOR *pColor,
		bool fFilt = false,
		BOOL fBigPlate = FALSE
	) = 0;
	STDMETHOD(CalRectifyInfoKL)(
		HV_COMPONENT_IMAGE *pImgInput, 
		CRectifyInfo *pRectifyInfo,
		PLATE_TYPE *pPlateType,
		PLATE_COLOR *pColor
	) = 0;

	STDMETHOD(RectifyPlate)(
		HV_COMPONENT_IMAGE *pImgInput,
		CRectifyInfo *pRectifyInfo,
		HV_COMPONENT_IMAGE *pImgOutput,
		BOOL fConservativeCut
	) = 0;

	STDMETHOD(SegmentPlate)(
		HV_COMPONENT_IMAGE *pImgInput,
		PLATE_COLOR *PlateColor,
		PLATE_TYPE *pPlateType,
		int *piRefMaxScore,
		int *piRefCharHeight,
		int *piRefVScale,
		CBinaryInfo *pBinaryInfo,
		HV_RECT **ppRect,
		int iFastSegMode,
		BOOL fIsNight,
		int iFiveCharWidth
		) = 0;

	STDMETHOD(Initialize)(
		int iWorkMode,
		PROCESS_PLATE_CFG_PARAM* pCfgParam
	) = 0;

	STDMETHOD(SetWorkMode)(
		int iWorkMode,
		BOOL fFramePlate = FALSE,
		BOOL fPhotoMode = FALSE,
		int iProcMode = PROC_PLATE_MODE_DEFAULT
	) = 0;

	STDMETHOD(SetInspector)(
		IInspector *pInspector
	)=0;

	STDMETHOD(SetDetector)(
		IPlateDetector *pDetector
	) = 0;

	STDMETHOD(SetRecognition)(
		IRecognition *pRecognition
	) = 0;

	STDMETHOD(SetHvParam)(
		HvCore::IHvParam2* pHvParam
	) = 0;

	STDMETHOD(SetHvPerformance)(
		HvCore::IHvPerformance* pHvPerf
	) = 0;
	STDMETHOD(SetSrcImgGray)(
		int iAvgGray, bool fUseFlag
	) = 0;   
	STDMETHOD(GetVersion)(
		char* szBuf, int szBufLen
	) = 0;
};

HRESULT CreateProcessPlateInstance(IProcessPlate **ppProcessPlate);

#endif // _PROCESSPLATE_INCLUDED__

