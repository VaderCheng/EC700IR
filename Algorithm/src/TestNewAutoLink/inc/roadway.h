#ifndef _ROADWAY_INCLUDED__
#define _ROADWAY_INCLUDED__

#include "swbasetype.h"
#include "HvInterface.h"
#include "trackercallback.h"

#define MAX_ROADWAY_NUM		4					// ��󳵵�����

// ͳ�Ƴ�����Ϣ
typedef struct _CAR_STAT_INFO
{
	BYTE8 nRoadway;					// �������
	DWORD32 dwLeftTime;				// �����뿪ʱ��
	DWORD32 dwSpeed;				// �����ٶ�
} CAR_STAT_INFO;

// �����ָ���
typedef struct TRACK_SEP_LINE_TAG
{
	HV_POINT poBegin;			// �ָ��߿�ʼ
	HV_POINT poEnd;				// �ָ��߽���
} TRACK_SEP_LINE;

// �����ӿ�
class IRoadway
{
public:
	virtual ~IRoadway() {};
/*
	STDMETHOD(ProcessOneFrame)(
		int iVideoID,
		IReferenceComponentImage *pImage,
		IReferenceComponentImage *pCapImage
	)=0;
	STDMETHOD(ProcessPhoto)(
		IReferenceComponentImage *pImage,
		PVOID pvParam
	)=0;
*/
	STDMETHOD(SetHvParam)(
		HvCore::IHvParam2* pHvParam
		) = 0;

	// ���ó���
	STDMETHOD(SetRoadway)(
		int nRoadwayNum,				// ��������
		TRACK_SEP_LINE *pLine,			// �����ָ���
		float fltRoadWidth				// ·����
	)=0;

	STDMETHOD(SetFrameProperty)(
		const FRAME_PROPERTY& cFrameProperty
		) = 0;

	// �������������
	STDMETHOD(SetCameraParam)(
		float fltCameraHigh,				// ������߶�
		float fltFarDistance,				// �������ֱ�㵽������ӽ�Զ�˵�ֱ�߾���
		float fltNearDistance				// �������ֱ�㵽������ӽǽ��˵�ֱ�߾���
		) = 0;

	// ����ʵ������
	virtual HV_POINT CalcAccCoordinate(
		HV_POINT &ptSrc							// ԭʼ����
		) = 0;

	// ��������ֱ�߾���
	virtual float CalcDistance(
		HV_POINT ptBegin,				// ��ʼλ������
		HV_POINT ptEnd					// ����λ������
		) = 0;

	// �����м䳵���ÿ��(����)
	virtual int GetRoadwayWidth(int nPosY) = 0;

	// ����������������
	virtual int GetRoadwayNum(HV_RECT rcObj) = 0;


	// ��������������
	virtual int GetObjWidthType(HV_RECT rcObj) = 0;		// 0:��С 1:С�� 2:�� 3:����
	// ��������߶ȶ�����
	virtual int GetObjHeightType(HV_RECT rcObj) = 0;	// 0:��С 1:С�� 2:�� 3:����

	// ȡ�ó���ͳ����Ϣ����
	STDMETHOD(GetTrackStatInfo)(int *pnTrackNum, TrackStatInfo *pStatData) = 0;

	// ���³����뿪��Ϣ(ͳ����)
	STDMETHOD(UpdateCarLeft)(CAR_STAT_INFO *pLeftCar) = 0;

};

extern HRESULT CreateRoadwayInstance(IRoadway** ppRoadway);

#endif // _ROADWAY_INCLUDED__
