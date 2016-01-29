/**
* @file	tracker.h
* @version	1.0
* @brief Tracker�ӿ�
*/

#ifndef _TRACKER_INCLUDED__
#define _TRACKER_INCLUDED__

#include "swobjbase.h"
#include "swimage.h"
#include "trackerdef.h"
#include "platerecogparam.h"
#include "inspector.h"
#include "swimageobj.h"
#include "hvinterface.h"
#include "ItgArea.h"
#include "DspLinkCmd.h"


/**
* Tracker�ӿڶ���
*/
class ITracker
{
public:
	virtual ~ITracker() {};
	STDMETHOD(ProcessOneFrame)(
		PROCESS_ONE_FRAME_PARAM* pParam,
		PROCESS_ONE_FRAME_DATA* pProcessData,
		PROCESS_ONE_FRAME_RESPOND* pProcessRespond
	)=0;
	STDMETHOD(ProcessPhoto)(
		HV_COMPONENT_IMAGE imgCurFrame,
		PVOID pvParam,
		PVOID pvData,
		PROCESS_PHOTO_RESPOND* pProcessRespond
		)=0;
	STDMETHOD(SetInspector)(
		IInspector *pInspector
	)=0;
	STDMETHOD(SetCharSet)(
		ChnCharSet nCharSet
	)=0;
	STDMETHOD(SetCalibrateParam)(
		BOOL fEnable ,
		int nRotate = 0,
		int nTilt = 0 ,
		BOOL fOutputImg = FALSE
	)=0;
	STDMETHOD(SetPreSegmentParam)(
		BOOL fEnable
	)=0;
	STDMETHOD(SetLPRMode)(PlateRecogMode nMode) = 0;
	STDMETHOD(ForceResult)(int iFlag = 0) = 0;
	STDMETHOD(GetTrackCount)(PDWORD32 pdwCount)=0;
	STDMETHOD(GetPlatePos)(DWORD32 dwIndex, HiVideo::CRect *prcPlate, HiVideo::CRect *prcRect) = 0;
	STDMETHOD(GetDetectCount)(PDWORD32 pdwCount) = 0;
	STDMETHOD(GetDetectPos)(DWORD32 dwIndex, HiVideo::CRect *prcDetected) = 0;

	STDMETHOD(GetObjCount)(PDWORD32 pdwCount) = 0;
	STDMETHOD(GetObjPos)(DWORD32 dwIndex, HiVideo::CRect *prcObj) = 0;

	STDMETHOD(IsVoted)(DWORD32 dwIndex, bool *pfVoted) = 0;
	STDMETHOD(SetVideoDet)(
		BOOL fEnabled,
		BOOL fSkipFrame = TRUE,
		int nDetLeft = 0,
		int nDetRight = 100
	) = 0;
	STDMETHOD(SetDetArea)(
		const DETECTAREA& cDetAreaInfo
	) = 0;

	// �õ���������
//	STDMETHOD(GetLightType)(LIGHT_TYPE* pLightType) = 0;
	// �������ȼ����ʼֵ
	STDMETHOD(SetFirstLightType)(LIGHT_TYPE nLightType) = 0;

	STDMETHOD(SetFirstPulseLevel)(int nPulseLevel) = 0;

	STDMETHOD(SetFirstCplStatus)(int nCplStatus) = 0;
	//ΪHvCoreʵ�ֵ�Init����
	STDMETHOD(InitTracker)(TRACKER_CFG_PARAM* pCfgParam) = 0;
	//ΪHvCoreʵ�ֵĶ�̬�޸Ĳ����ӿ�
	STDMETHOD(ModifyTracker)(TRACKER_CFG_PARAM* pCfgParam) = 0;

	// �������ģ�͵ĸ��½ӿ�
	STDMETHOD(UpdateModel)(unsigned int uModelId, unsigned char* pbData, unsigned int cbDataSize) = 0;
	// ��̬�����㷨�����ӿ�
	STDMETHOD(UpdateAlgParam)(unsigned char* pbData, unsigned int cbDataSize) = 0;

	//���Լ��ģ������ܣ����Ե�ģ�����ͺͲ��Բ���ͨ��ȫ�ֲ����б�����,
	//ȫ�ֲ����б�ͨ��INI�ļ�����.
	STDMETHOD(TestPerformance)(
		HV_COMPONENT_IMAGE *phciTest,	//��������
		char *pszReport,				//���Ա���
		DWORD32 dwReportBufSize,		//���Ա��滺��������
		DWORD32 dwMaxTestCount,			//�����Դ���
		DWORD32 dwMaxTestDuration		//������ʱ��(����)
		) = 0;

	//ģ�����
	STDMETHOD(ModuleTest)(
		MODULE_ID nModuleID,
		DWORD32 dwFlag,
		HV_COMPONENT_IMAGE *pImg,
		STD_RESULT* pStdResult,
		PVOID pvArgDat = NULL
		) = 0;

	//����ģ�ͼ��ؽӿ�
	STDMETHOD(SetHvModel)(HvCore::IHvModel* pModel) = 0;

	STDMETHOD(SetHvPerformance)(HvCore::IHvPerformance* pHvPerf) = 0;

	STDMETHOD(GetItgArea)(
		CItgArea** ppItgArea,
		DWORD32 dwRefTime
		) = 0;

	// zhaopy
	STDMETHOD(CheckEvent)() = 0;

	//���̵����
	//ȡ��ǰ���̵Ƶ�״̬,ͨ��IPT
	STDMETHOD(GetTrafficLightStatus)(
		unsigned char* pbStatus
		)
	{
		return E_NOTIMPL;
	}
	STDMETHOD(TrafficLightStatusEx)(
		UINT iGroupIndex,			//��Ӧ�ĸ�����ļ���� 
		UINT iRedLightCount,		//�������
		HV_RECT* rgRedLightPos,		//���������
		UINT iGreenLightCount,   //�̵�����
		HV_RECT* rgGreenLightPos	//�̵��������
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
		TRAFFICLIGHT_SCENE tsSceneStatus,	//�û��Զ���ĳ���״̬
		char* pLightStatus
		)
	{
		return E_NOTIMPL;
	}
};

extern HRESULT CreateTracker_Photo(ITracker** ppTracker);
extern HRESULT CreateTracker_Video(ITracker** ppTracker, int iVideoID = 0);
extern HRESULT DesdroyTracker(ITracker** ppTracker);

#endif // _TRACKER_INCLUDED__
