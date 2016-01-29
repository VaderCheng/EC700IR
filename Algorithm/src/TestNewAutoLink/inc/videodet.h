#ifndef _VIDEODET_INCLUDED__
#define _VIDEODET_INCLUDED__

#include "hvBaseObj.h"
#include "swObjBase.h"
#include "swImage.h"
#include "VideoDetCallback.h"
#include "swImageObj.h"

#include "HvInterface.h"
#include "TrackerCallback.h"

using namespace HiVideo;

class IVideoDet
{
public:
	virtual ~IVideoDet() {};
	STDMETHOD(ProcessOneFrame)(
		IReferenceComponentImage *pImage,
		IReferenceComponentImage *pCaptureImage,
		bool fDayNigh
	)=0;
	STDMETHOD(SetCallBack)(
		IVideoDetCallback *pCallback
	)=0;
	STDMETHOD(SetParam)(
		BOOL fSkipFrame,
		int nVideoDetLeft = 0,
		int nVideoDetRight = 100
	)=0;
	STDMETHOD(SetHvParam)(
		HvCore::IHvParam2* pHvParam
	)=0;
	//new initialize func
	STDMETHOD(InitVideoDet)(
		HV_RECT rcDetArea
	)=0;

	// ���³����뿪��Ϣ
	STDMETHOD(UpdateCarLeft)(
		HiVideo::CRect rcPos,				// ����λ��
		PLATE_COLOR color,					// ������ɫ
		bool fNoPlate						// �Ƿ����Ƴ�
		)=0;

	// ���³����뿪��Ϣ
	virtual bool IsNewCarMoving(
		)=0;

	// �Ƿ�������ĳ���
	virtual bool IsCloseToPlate(
		HiVideo::CRect rcPos				// ����λ��
		)=0;

	// ȡ�ó����뿪����ֵ
	virtual int GetCarLeftCount(
		)=0;

	virtual void FireCarLeavingEvent(
		)=0;

};

HRESULT CreateVideoDetInstance(IVideoDet** ppVideoDet, int nID);

#endif // _VIDEODET_INCLUDED__
