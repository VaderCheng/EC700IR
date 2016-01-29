#ifndef _SCALESPEED_INCLUDED__
#define _SCALESPEED_INCLUDED__

#include "hvBaseObj.h"
#include "swObjBase.h"
#include "swImage.h"
#include "swImageObj.h"

#include "HvInterface.h"
#include "TrackerDef.h"

using namespace HiVideo;

typedef struct _SCALESPEED_CFG_PARAM
{
    int nMethodsFlag;   //����ģʽ��0:ƽ��궨��, 1:�ռ�궨��
    float fltLeftDistance;  //��Ļ���ص����żܾ���
    float fltFullDistance;  //��Ļ���ص����żܾ���
    float fltCameraHigh;    //������߶�
    float fltAdjustCoef;    //����ϵ��
    float rgfltTransMarix[11];  //����ͼ�����굽�ռ�����ı任����

	_SCALESPEED_CFG_PARAM()
	{
        nMethodsFlag = 0;
        fltLeftDistance = 25.0f;
        fltFullDistance = 75.0f;
        fltCameraHigh = 7.5f;
        fltAdjustCoef = 1.050f;
        rgfltTransMarix[0] = 0.4970550537f;
        rgfltTransMarix[1] = -241.9865264893f;
        rgfltTransMarix[2] = -30.4937019348f;
        rgfltTransMarix[3] = 0.5036621094f;
        rgfltTransMarix[4] = -21.2472610474f;
        rgfltTransMarix[5] = -6.6555485725f;
        rgfltTransMarix[6] = -231.5502014160f;
        rgfltTransMarix[7] = 0.7746582031f;
        rgfltTransMarix[8] = 0.0300335791f;
        rgfltTransMarix[9] = -0.0085402671f;
        rgfltTransMarix[10] = -0.0195075944f;
	}
}SCALESPEED_CFG_PARAM;

class IScaleSpeed
{
public:
	virtual ~IScaleSpeed() {};

	//new initialize func
	STDMETHOD(InitScaleSpeed)(
		SCALESPEED_CFG_PARAM *pScaleSpeedParam
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
	virtual bool GetActurePoint(HV_POINT ptOrg, HV_POINT& ptDest, float fltDistance) = 0;
};

HRESULT CreateScaleSpeedInstance(IScaleSpeed** ppScaleSpeed, int iVideoID = 0);

#endif // _SCALESPEED_INCLUDED__
