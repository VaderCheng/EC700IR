#ifndef _TRACKERIMPL_INCLUDED__
#define _TRACKERIMPL_INCLUDED__

#include "tracker.h"
#include "platedetector.h"
#include "processplate.h"
#include "recognition.h"
#include "TrackInfoHigh.h"

using namespace HiVideo;

class CTrackerImpl_Photo : public ITracker
{
public:
	CTrackerImpl_Photo();

	// ITracker
	virtual ~CTrackerImpl_Photo();
	void Clear();

	STDMETHOD(ProcessOneFrame)(
		PROCESS_ONE_FRAME_PARAM* pParam,
		PROCESS_ONE_FRAME_DATA* pProcessData,
		PROCESS_ONE_FRAME_RESPOND* pProcessRespond
		)
	{
		return E_NOTIMPL;
	}

	STDMETHOD(ProcessPhoto)(
		HV_COMPONENT_IMAGE imgCurFrame,
		PVOID pvParam,
		PVOID pvData,
		PROCESS_PHOTO_RESPOND* pProcessRespond
		);

	STDMETHOD(SetInspector)(
		IInspector *pInspector
		);

	STDMETHOD(SetHvParam)(
		HvCore::IHvParam2* pHvParam
		);

	STDMETHOD(SetHvModel)(
		HvCore::IHvModel* pModel
		);

	STDMETHOD(SetFirstPulseLevel)(
		int nPulseLevel)
	{
		return S_OK;
	}
	STDMETHOD(SetFirstCplStatus)(
		int nCplStatus)
	{
		return S_OK;
	}
	//ͼƬ������ʵ��
	STDMETHOD(SetHvPerformance)(
		HvCore::IHvPerformance* pHvPerf
		);

	STDMETHOD(SetCharSet)(
		ChnCharSet nCharSet
		)
	{
		return E_NOTIMPL;
	}
	STDMETHOD(SetCalibrateParam)(
		BOOL fEnable,
		int nRotate=0,
		int nTilt=0,
		BOOL fOutputImg = FALSE
		)
	{
		return E_NOTIMPL;
	}
	STDMETHOD(SetPreSegmentParam)(
		BOOL fEnable
		)
	{
		return E_NOTIMPL;
	}
	STDMETHOD(SetLPRMode)(PlateRecogMode nMode)
	{
		return E_NOTIMPL;
	}
	STDMETHOD(SetVideoDet)(BOOL fEnabled, BOOL fSkipFrame = TRUE, int nDetLeft = 0, int nDetRight = 100)
	{
		return E_NOTIMPL;
	}
	STDMETHOD(SetScaleSpeed)(
		BOOL fEnabled,
		float fltFullWidth,
		float fltCameraHigh,
		float fltLeftWidth,
		float fltAdjustCoef
		)
	{
		return E_NOTIMPL;
	}
	STDMETHOD(SetDetArea)( const DETECTAREA& cDetAreaInfo )
	{
		return E_NOTIMPL;
	}
	STDMETHOD(ForceResult)(int i)
	{
		return E_NOTIMPL;
	}
	STDMETHOD(GetTrackCount)(PDWORD32 pdwCount)
	{
		return E_NOTIMPL;
	}
	STDMETHOD(GetPlatePos)(DWORD32 dwIndex, HiVideo::CRect *prcPlate, HiVideo::CRect *prcRect)
	{
		return E_NOTIMPL;
	}
	STDMETHOD(ClearTrackInfo)()
	{
		return E_NOTIMPL;
	}
	STDMETHOD(IsVoted)(DWORD32 dwIndex, bool *pfVoted)
	{
		return E_NOTIMPL;
	}

	STDMETHOD(GetDetectCount)(PDWORD32 pdwCount) {return S_OK;}
	STDMETHOD(GetDetectPos)(DWORD32 dwIndex, HiVideo::CRect *prcDetected) {return S_OK;}
	STDMETHOD(GetObjCount)(PDWORD32 pdwCount)  {return S_OK;}
	STDMETHOD(GetObjPos)(DWORD32 dwIndex, HiVideo::CRect *prcObj)  {return S_OK;}

	//ΪHVCOREʵ�ֵĳ�ʼ������
	STDMETHOD(InitTracker)(TRACKER_CFG_PARAM* pCfgParam);
	STDMETHOD(ModifyTracker)(TRACKER_CFG_PARAM* pCfgParam) {return S_OK;}

	// �������ģ�͵ĸ��½ӿ�
	STDMETHOD(UpdateModel)(unsigned int uModelId, unsigned char* pbData, unsigned int cbDataSize) {return S_OK;}
	// ��̬�����㷨�����ӿ�
	STDMETHOD(UpdateAlgParam)(unsigned char* pbData, unsigned int cbDataSize) {return S_OK;}

	//���Լ��ģ������ܣ����Ե�ģ�����ͺͲ��Բ���ͨ��ȫ�ֲ����б�����,
	//ȫ�ֲ����б�ͨ��INI�ļ�����.
	STDMETHOD(TestPerformance)(
		HV_COMPONENT_IMAGE *phciTest,	//��������
		char *pszReport,				//���Ա���
		DWORD32 dwReportBufSize,		//���Ա��滺��������
		DWORD32 dwMaxTestCount,			//�����Դ���
		DWORD32 dwMaxTestDuration		//������ʱ��(����)
		)
	{
		return E_NOTIMPL;
	}

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

/*
	// �õ���������
	STDMETHOD(GetLightType)(LIGHT_TYPE* pLightType)
	{
		return E_NOTIMPL;
	}*/

	// �������ȼ����ʼֵ
	STDMETHOD(SetFirstLightType)(LIGHT_TYPE nLightType)
	{
		return E_NOTIMPL;
	}
	

	STDMETHOD(GetItgArea)(
		CItgArea** ppItgArea,
		DWORD32 dwRefTime
		)
	{
		*ppItgArea = NULL;
		return S_OK;
	}

		// zhaopy
	STDMETHOD(CheckEvent)() { return E_NOTIMPL; }

public:
	//�����������
	HRESULT OnCreate();

protected:
	BOOL m_fInitialized;
	INT m_nWidth, m_nHeight, m_nStride;

	PlateRecogParam *m_pParam;

	IInspector *m_pInspector;
	IPlateDetector *m_pPlateDetector;
	IProcessPlate *m_pProcessPlate;
	IRecognition *m_pRecognition;

	HvCore::IHvModel* m_pHvModel;
	HvCore::IHvPerformance* m_pHvPerf;

	static const int s_knMaxPlateCandidate = 200;
	RECTA m_rgTempPlatePos[s_knMaxPlateCandidate];

	int m_iRoadNumber;	//��������
	ROAD_INFO m_roadInfo[CTrackInfo::s_iMaxRoadNumber];

	//��ͼ��������
	int m_nMaxCropWidth;
	int m_nMaxCropHeight;

	//��ʼ��ʱ��������
	HRESULT OnInit();

	HRESULT SetFrameProperty(
		const FRAME_PROPERTY& FrameProperty
		);

	HRESULT GetInflateRect(
		CRect &rect,		// in, out
		PLATE_COLOR color,
		PLATE_TYPE &type,
		INT maxRectHeight	// �ж��Ƿ���inflate rate2
		);

	//���߽�ͼ
	HRESULT CropPlateImage(
		const HV_COMPONENT_IMAGE& imgPhoto,
		CRect& rcPlate,
		PLATE_TYPE nPlateType,
		HV_COMPONENT_IMAGE* rgPlateImage,
		int nCount	//ָ����ȡͼ�Ľ����
		);

	//Ԥ����
	HRESULT PreTreatment(
		HV_COMPONENT_IMAGE& imgPlate,
		PLATE_TYPE nPlateType,
		PLATE_COLOR nPlateColor,
		BOOL fEnhance,
		CPersistentComponentImage* rgImgRectified,
		int nCount,	 //ָ��Ԥ�������ɵ�ͼ����,ͨ���ı䴹ֱУ���Ƕ�
		CBinaryInfo &BinInfo
		);

	HRESULT RecognizeChar(
		HV_COMPONENT_IMAGE imgPlate,
		HV_RECT rcChar,
		RECOGNITION_TYPE RecogType,
		PlateInfo	&cPlateInfo,
		PBYTE8 pbResult,
		PSDWORD32 pdwProb,
		int iCharPos
		);

	HRESULT RecognizePlate(
		HV_COMPONENT_IMAGE imgPlate,
		PlateInfo &Info,
		CBinaryInfo &BinaryInfo,
		int nFiveCharWidth
		);

	HRESULT RecognizeColorArea(
		const HV_COMPONENT_IMAGE imgInput, 
		//IReferenceComponentImage* pTempImg,
		HV_RECT rcCrop, 
		BYTE8 *cTempAdd, 
		BYTE8 *piResult,
		int iCutLeng,
		LIGHT_TYPE m_PlateLightType
		);

	HRESULT RecognizeCarColourPlus(
		HV_COMPONENT_IMAGE imgPlateOri,
		PlateInfo &Info,
		int iAvgGray,
		BOOL fIsNight = FALSE
		);

// 	HRESULT RecognizeCarColour(
// 		HV_COMPONENT_IMAGE imgPlate,
// 		PlateInfo &Info,
// 		BOOL fIsNight = FALSE
// 		);

	HRESULT CalcCarSize(
		HV_COMPONENT_IMAGE *pImgCurFrame,
		OUTPUT_CAR_SIZE_INFO *pCarSize,
		CAR_TYPE nCarType
		);

	HRESULT Vote(
		PlateInfo** rgInfo,
		int nCount,
		PlateInfo* pResult,
		CParamQuanNum* pfltConf
		);

	void RoadInfoParamInit();
	void InitRoadInfoParam();

	int LeftOrRight(int iX, int iY, int iLineNum, int* piDistance = NULL);
	int MatchRoad(int iX, int iY);

private:
	//�����ò���
	PlateRecogParam m_PlateRecogParam;

	// �Ƿ����������Ϣ
	int m_fOutputDebugInfo;

	// �Ƿ��жϹ�������־
	int m_iRecogGxPolice;

	// ���ػ����֣�38~68, 58Ϊ����
	int m_nDefaultChnId;

	//��WJ������
	BOOL m_fEnableNewWJSegment;
	//��WJ�ַ��滻
	BOOL  m_fEnableDefaultWJ;
	char  m_strDefaultWJChar[3];

	//����У������
	BOOL m_fEnableRectify;

	//������ɫʶ�𿪹�
	BOOL m_fEnableRecgCarColor;

	//���ȳ��Ʒ�λ
	int m_nFirstPlatePos;

	//Ħ�г������ֵ
	int m_iMiddleWidth;
	int m_iBottomWidth;

	BOOL m_fOutputCarSize;
	//������ɫʶ�����
	int m_nEnableColorReRecog;  //С���Ƿ�ȡ��������������ʶ����
	int m_nWGColorReThr;        //�Ұ���ʶ�𡪡�ƫ����ʶ���
	int m_nWColorReThr;        //����ʶ�𡪡��Ұ׵�����ƫ����ʶ���
	int m_nBColorReThr;        //����ʶ�𡪡��Һڵ�����ƫ����ʶ���
	bool m_fEnableCorlorThr;  //���������복�����ȱȽϿ���
	int m_nBGColorThr;        //�������ȱȻ������ȴ�BGColorThr�ĺ�ɫ�������óɻ�ɫ��
	int m_nWGColorThr;        //���ϳ������ȱȻ������ȴ�WGColorThr�Ļ�ɫ�������óɰ�ɫ��

	TRACKER_CFG_PARAM m_cfgParam;
};

#endif // _TRACKERIMPL_INCLUDED__
