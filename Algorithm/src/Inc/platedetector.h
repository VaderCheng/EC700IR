#ifndef _PLATEDETECTOR_INCLUDED__
#define _PLATEDETECTOR_INCLUDED__

#include "config.h"
#include "swbasetype.h"
#include "swplate.h"
#include "swobjbase.h"
#include "swimage.h"
#include "platerecogparam.h"

#include "hvinterface.h"

#include "ItgArea.h"

// ����ģʽ, SetWorkMode()������nProcMode����ʹ��
#define PLATE_DET_MODE_DEFAULT 0
#define PLATE_DET_MODE_IR      1

typedef struct _PLATE_DETECT_CFG_PARAM
{
	int nPlateDetect_Green; // ����ʶ�𿪹�
	float fltPlateDetect_StepAdj; // ��������ϵ��
	int nPlateDetect_WJMergeNum;
	_PLATE_DETECT_CFG_PARAM()
	{
		nPlateDetect_Green = 0;
		fltPlateDetect_StepAdj = 1.0f;
		nPlateDetect_WJMergeNum = 20;
	}
} PLATE_DETECT_CFG_PARAM;

typedef enum {
	ALL_DETECTOR_PHOTO = -2,	// ͼƬ����
	ALL_DETECTOR = -1,		// ʹ�����м����
	PLATE_BLUE_DETECTOR = 0,	// ʹ�õ���ڵװ��ֳ��Ƽ����
	PLATE_YELLOW_DETECTOR,		// ʹ�õ���׵׺��ֳ��Ƽ����
	PLATE_YELLOW2_DETECTOR,		// ʹ��˫��׵׺��ֳ��Ƽ����
	WJ_DETECTOR,				// ʹ��WJ�����
	PLATE_DBGREEN_DETECTOR,
    CAR_DETECTOR,
	FACE_DETECTOR,				// ���������
	DETECTOR_COUNT
} DETECTOR_TYPE;

struct RECTA : public HV_RECT
{
	PLATE_COLOR color;
	PLATE_TYPE nPlateType;
	DWORD32 nVariance;
	int nConf;			// ���Ŷ�,Խ��Խ��
};

typedef struct _MULTI_ROI {
    int dwNumROI;   // number of Plates found;
    RECTA  *rcROI;
} MULTI_ROI;

class IPlateDetector
{
public:
	virtual ~IPlateDetector(){};
	IPlateDetector(){};

	STDMETHOD(ScanPlatesComponentImage)(
		DETECTOR_TYPE nDetectorType,
		HV_COMPONENT_IMAGE *pComponetImage,
		MULTI_ROI *ppROIs,
		int	nDetMinScaleNum,				// ���Ƽ������С���
		int	nDetMaxScaleNum	,			// ���Ƽ���������
		BOOL fFastMode = FALSE,			//�Ƿ�ʹ�ÿ���ɨ��.
		int nRefPlateWidth = 0,
		BOOL fDownSample = FALSE,		// Downsampling or not (currently ignored)
		int	*piAvgY	= NULL,				//��ǰͼ���ƽ������
		CItgArea* pItgArea = NULL
	) = 0;

        	/*
	����˵��:�ú��������Ӹ�����ͼ���м�⳵��,��⳵�ƵĽ��������ppROIs��.
	��������ֵ:
		����S_OK��ʾ���ɹ�ִ��.
		����E_POINTER��ʾpComponetImageΪNULL.
		����S_FALSE��ʾ���ʧ��. 
		
        �÷���
             nStep = 1 ʱֻ��һ�����,ppROIsIn��Ϊ��; 
             nStep = 2 ʱ���������,ppROIsInΪһ��������� 
		*/
    STDMETHOD(ScanPlatesComponentImagePhotoStep)(
        int nStep,                                      // ��ⲽ�� 
        HV_COMPONENT_IMAGE 	*pComponetImage,			//ָ����Ҫ���Ĵ�ͼ.
        MULTI_ROI 			*ppROIsIn,					//ָ���ƴ���Ķ���.
        MULTI_ROI 			*ppROIs,					//ָ���ƽ������.
        int				nDetMinScaleNum,				// ���Ƽ������С���
        int				nDetMaxScaleNum	,			// ���Ƽ���������
        BOOL 				fFastMode = FALSE,			//�Ƿ�ʹ�ÿ���ɨ��.
        int 				nRefPlateWidth = 0,			//�ο����ƿ��.
        BOOL 				fDownSample = FALSE,		//Downsampling or not (currently ignored)
        int					*piAvgY	= NULL,				//��ǰͼ���ƽ������
        CItgArea*		pItgArea = NULL
        ) = 0;

	STDMETHOD(Initialize)(
		PlateRecogParam*	pParam,
		PLATE_DETECT_CFG_PARAM *pDetectParam,
		int 				iMaxWidth,
		int 				iMaxHeight
	) = 0;

    STDMETHOD(SetWorkMode)(
        int iProcMode = PLATE_DET_MODE_DEFAULT
    ) = 0;

	STDMETHOD(SetRecParam)(
		PlateRecogParam*	pParam
	) = 0;

	STDMETHOD(SetImgSize)(
		int 				iMaxWidth,
		int 				iMaxHeight
	) = 0;

	STDMETHOD(SetHvParam)(
		HvCore::IHvParam2* pHvParam
	) = 0;

	STDMETHOD(SetHvPerformance)(
		HvCore::IHvPerformance* pHvPerf
	) = 0;

	STDMETHOD(SetHvModel)(
		HvCore::IHvModel* pHvModel
	) = 0;

	STDMETHOD(GetVersion)(
		char* szBuf, int szBufLen
	) = 0;
};

HRESULT CreatePlateDetectorInstance(IPlateDetector** ppDetector);
HRESULT InitializeDetectorModelInfo( void );

#endif // _PLATEDETECTOR_INCLUDED__
