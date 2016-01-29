//*****************************************************
//****ע��hv2iplΪ����opencv�������ʾ���ļ�***********
//****��Ӧ��cpp��h�ļ�ֻ����debug����ģʽ��ʹ��********
//*****************************************************

//#define _OPENCV_DEBUG

#include "swimage.h"

#if defined(_OPENCV_DEBUG) && defined(_DEBUG) && (RUN_PLATFORM != PLATFORM_DSP_BIOS)

#ifndef _HV2IPL_H_
#define _HV2IPL_H_

#include "cv.h"
#include "cxcore.h"
#include "highgui.h"

HRESULT HvImageToIplImageBGR(IplImage *pDst, const HV_COMPONENT_IMAGE *pSrc);
HRESULT HvImageToIplImageGrey(IplImage *pDst, const HV_COMPONENT_IMAGE *pSrc);

HRESULT HvImageDebugShow(const HV_COMPONENT_IMAGE *pImg, CvSize csShowWinSize);

#endif

#endif
