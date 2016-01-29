#ifndef _IMG_TRANS_H
#define _IMG_TRANS_H

#include "HvUtils.h"
#include "hvbaseobj.h"

HRESULT ImgTrans(
				 BYTE8* pSrcBuf, //ͼ������
				 int nSrcBufLen,
				 BYTE8*	 pDstBuf, //�������
				 int& nDstBufLen, //��������С
				 int nBrightness, //������ǿ, -255~255
				 int nContrast, //�Աȶ�, -100~100
				 int nThrehold, //������ֵ
				 int nStretch, //�������쿪��
				 int nCompressRate //ѹ����, 0~100
				 );

HRESULT TrafficLightEnhance(
							BYTE8* pSrcBuf, //ͼ������
							int nSrcBufLen,
							BYTE8*	 pDstBuf, //�������
							int& nDstBufLen, //��������С
							HiVideo::CRect* rgPos = NULL,	 // ����λ�ü�����	
							int nPosCount = 1, 
							int nBrightness = 0, //������ǿ
							int nHueThreshold = 120,	//ɫ����ֵ
							int nCompressRate = 90 //ѹ����
							);

#endif