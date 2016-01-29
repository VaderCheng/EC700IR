#ifndef _STRINGOVERLAY_H_
#define _STRINGOVERLAY_H_

#include "swbasetype.h"
#include "swwinerror.h"
#include "swimage.h"
#include "DspLink.h"
#include "DspLinkCmd.h"

#ifdef __cplusplus
extern "C" {
#endif

/** 
 * ��ʼ���ַ�����DSP����
 * 
 * @param pStringOverlayParam   ����
 * @param pStringOverlayData    ��������
 * @param pStringOverlayRespond ��Ӧ��
 * 
 * @return S_OK
 */
HRESULT InitStringOverlay(
	const STRING_OVERLAY_PARAM* pStringOverlayParam,
	const STRING_OVERLAY_DATA*  pStringOverlayData,
	STRING_OVERLAY_RESPOND* pStringOverlayRespond
);

/** 
 * ��ʼ���ַ�������ز���
 * 
 * @param x ��������X����
 * @param y ��������Y����
 * @param w ������(�����ȣ��ǵ�������)
 * @param h ����߶�
 * @param iIsFixedLight �������ȹ̶� 
 * @param iFontColor ������ɫ(RGB)
 * @param fIsSideInstall ��װ
 * @param rgbDotMatrix �������ݻ�����
 * 
 * @note
 *       �����з��ֲ�װ��H264�߶�Ϊ��ת�ߵ�һ�룬����Ҫ�˲������ж�
 *       ���⣺�Ƿ����еĲ�װ����ˣ��д���֤
 */
void SetOverlayMessageContent(
	int x, int y,
	int w, int h,
	int iIsFixedLight,
	int iFontColor,
	BOOL  fIsSideInstall,
	const PBYTE8 rgbDotMatrix
);

/** 
 * ����YUV422P�ռ�(��200W JPEG����)
 * 
 * @param szDateTimeStrings ʱ���ַ���(��ARM�˴���)
 * @param pbY Y������
 * @param iYStride Y���
 * @param pbU U������
 * @param iUStride U���
 * @param pbV V������
 * @param iVStride V���
 * @param iImageWidth ͼ����
 * @param iImageHeight ͼ��߶�
 *
 * @note 
 *        ��Ҫͼ���������Ϊ��Ҫ�����ӵ����������ڿɼ�ͼ��Χ��
 */
void DoStringOverlayYUV422P(
	const PBYTE8 szDateTimeStrings,
	PBYTE8 pbY, int iYStride,
	PBYTE8 pbU, int iUStride,
	PBYTE8 pbV, int iVStride,
	int iImageWidth, int iImageHeight
);

/** 
 * ����YUV422SP�ռ�(��H264����)
 * 
 * @param szDateTimeStrings ʱ���ַ���(��ARM�˴���)
 * @param fDoubleStream ˫������־
 * @param pbY Y������
 * @param iYStride Y���
 * @param pbC UV������
 * @param iCStride UV���
 * @param iImageWidth ͼ����
 * @param iImageHeight ͼ��߶�
 *
 * @note 
 *        ��Ҫͼ���������Ϊ��Ҫ�����ӵ����������ڿɼ�ͼ��Χ��
 *        ˫����ʱ��H264�����룬�ڴ�����²����е��ӣ�����������ɫ
 *        ��JPEG����һ�£�����װʱ����Ҫ���е���
 */
void DoStringOverlayYUV422SP(
	const PBYTE8 szDateTimeStrings,
	BOOL fDoubleStream,
	PBYTE8 pbY, int iYStride,
	PBYTE8 pbC, int iCStride,
	int iImageWidth, int iImageHeight
);

/** 
 * ����YUV420P�ռ�(��500W JPEG����)
 * 
 * @param szDateTimeStrings ʱ���ַ���(��ARM�˴���)
 * @param pbY Y������
 * @param iYStride Y���
 * @param pbU U������
 * @param iUStride U���
 * @param pbV V������
 * @param iVStride V���
 * @param iImageWidth ͼ����
 * @param iImageHeight ͼ��߶�
 *
 * @note 
 *        ��Ҫͼ���������Ϊ��Ҫ�����ӵ����������ڿɼ�ͼ��Χ��
 */
void DoStringOverlayYUV420P(
	const PBYTE8 szDateTimeStrings,
	PBYTE8 pbY, int iYStride,
	PBYTE8 pbU, int iUStride,
	PBYTE8 pbV, int iVStride,
	int iImageWidth, int iImageHeight
);

#ifdef __cplusplus
}
#endif

#endif

