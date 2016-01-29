#ifndef _SCALESPEED_INCLUDED__
#define _SCALESPEED_INCLUDED__

#include "hvBaseObj.h"
#include "swObjBase.h"
#include "swImage.h"
#include "swImageObj.h"

#include "HvInterface.h"
#include "TrackerCallback.h"

using namespace HiVideo;

class IScaleSpeed
{
public:
	virtual ~IScaleSpeed() {};

	//new initialize func
	STDMETHOD(InitScaleSpeed)(
	)=0;

	STDMETHOD(SetHvParam)(
		HvCore::IHvParam2* pHvParam
	)=0;

	STDMETHOD(SetFrameProperty)(
		const FRAME_PROPERTY& cFrameProperty
	) = 0;

	// ���㳵���ٶ�
	virtual float STDMETHODCALLTYPE CalcCarSpeed(		// ���س����ٶ�
		CRect rcBegin,				// ��ʼλ������
		CRect rcEnd,				// ����λ������
		DWORD32 dwDruTime,			// ʱ��(����)
		bool fAdjustDistance,	// �Ƿ�ͨ�����ƿ�Ƚ����������
		PLATE_TYPE PlateType,		//��������
		float &fltScale			// ���ز���������ʵ�ʾ���ı���
	) = 0;

	virtual float STDMETHODCALLTYPE CalcCarSpeedNewMethod(		// ���س����ٶ�
		CRect rcBegin,				// ��ʼλ������
		CRect rcEnd,				// ����λ������
		DWORD32 dwDruTime,			// ʱ��(����)
		bool fAdjustDistance,	// �Ƿ�ͨ�����ƿ�Ƚ����������
		PLATE_TYPE PlateType,		//��������
		float &fltScale			// ���ز���������ʵ�ʾ���ı���
		) = 0;

	//������Ļ����
	virtual float STDMETHODCALLTYPE CalcActureDistance(HV_POINT point1, HV_POINT point2) = 0;
};

HRESULT CreateScaleSpeedInstance(IScaleSpeed** ppScaleSpeed, int iVideoID = 0);

#endif // _SCALESPEED_INCLUDED__
