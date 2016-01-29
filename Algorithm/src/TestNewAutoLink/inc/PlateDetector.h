#ifndef _PLATEDETECTOR_INCLUDED__
#define _PLATEDETECTOR_INCLUDED__

#include "config.h"
#include "swBaseType.h"
#include "swPlate.h"
#include "swObjBase.h"
#include "swImage.h"
#include "PlateRecogParam.h"

#include "HvInterface.h"

#include "ItgArea.h"

typedef enum {
	ALL_DETECTOR_PHOTO = -2,	// ͼƬ����
	ALL_DETECTOR = -1,		// ʹ�����м����
	PLATE_BLUE_DETECTOR = 0,	// ʹ�õ���ڵװ��ֳ��Ƽ����
	PLATE_YELLOW_DETECTOR,		// ʹ�õ���׵׺��ֳ��Ƽ����
	PLATE_YELLOW2_DETECTOR,		// ʹ��˫��׵׺��ֳ��Ƽ����
	WJ_DETECTOR,				// ʹ��WJ�����
	PLATE_DBGREEN_DETECTOR,
	DETECTOR_COUNT
} DETECTOR_TYPE;

struct RECTA : public HV_RECT
{
	PLATE_COLOR color;
	PLATE_TYPE nPlateType;
	DWORD32 nVariance;
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

	STDMETHOD(Initialize)(
		PlateRecogParam*	pParam,
		int 				iMaxWidth,
		int 				iMaxHeight		
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
};

HRESULT CreatePlateDetectorInstance(IPlateDetector** ppDetector);
HRESULT InitializeDetectorModelInfo( void );

#endif // _PLATEDETECTOR_INCLUDED__
