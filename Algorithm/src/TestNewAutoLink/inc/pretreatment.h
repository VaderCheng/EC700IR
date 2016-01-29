#ifndef _PRETREATMENT_INCLUDED__
#define _PRETREATMENT_INCLUDED__

#include "HvInterface.h"
#include "TrackerCallback.h"

class IPretreatment
{
public:
	virtual ~IPretreatment() {};

	STDMETHOD(Initialize)() = 0;

	STDMETHOD(SetHvParam)(HvCore::IHvParam2* pHvParam) = 0;

	STDMETHOD(PreCalibrate)(
		HV_COMPONENT_IMAGE Img,    //ԭʼͼ��
		HV_COMPONENT_IMAGE *pImgCalibrated  //У�����ͼ��
	) = 0;

	STDMETHOD(SetParam)(
		INT nHRotate=0,				//ˮƽ��ת�Ƕ�
		INT nVTilt=0				//��ֱ��б�Ƕ�
	) = 0;

};

HRESULT CreatePretreatmentInstance(IPretreatment **ppPretreatment );


#endif // _PRETREATMENT_INCLUDED__

