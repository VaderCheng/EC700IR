#ifndef _CAMDSPPROC_H_
#define _CAMDSPPROC_H_

#include <csl_cache.h>
#include "swwinerror.h"
#include "swBaseType.h"
#include "DmaCopyApi.h"

#ifdef __cplusplus
extern "C" {
#endif

// ---------------- ��[200wǰ��BT1120��ʱ����ת90��]�����ݸ�ʽת������ ----------------

// Jpeg����ʹ��
void BT1120_ROTATE_Y_1200_1600_To_YUV422P_1200_1600(
	const PBYTE8 pbSrcY,
	const PBYTE8 pbSrcUV,
	PBYTE8 pbDstY,
	PBYTE8 pbDstU,
	PBYTE8 pbDstV
);

// H.264����ʹ��
void BT1120_ROTATE_Y_1200_1600_To_YUV422SP_1184_800(
	PBYTE8 pbBT1120Y,
	PBYTE8 pbBT1120UV,
	PBYTE8 pbYUV422SP_Y,
	PBYTE8 pbYUV422SP_UV
);

// ---------------- ��[500wǰ��BT1120��]�����ݸ�ʽת������ ----------------

// Jpeg����ʹ��
void BT1120_2448_1024_To_YUV420P_2448_2048(
	PBYTE8 pbSrcY,
	PBYTE8 pbSrcUV,
	PBYTE8 pbDstY,
	PBYTE8 pbDstU,
	PBYTE8 pbDstV
);

// H.264����ʹ��
void BT1120_2448_1024_To_YUV422SP_1216_1024(
	PBYTE8 pbSrcY,
	PBYTE8 pbSrcUV,
	PBYTE8 pbDstY,
	PBYTE8 pbDstUV
);

// ---------------- �������ݸ�ʽת������ ----------------

// BT1120 UV to SplitUV
void BT1120UV_To_SplitUV(
	PBYTE8 pbSrcUV,
	int iWidth,
	int iHeight,
	PBYTE8 pbDstU,
	PBYTE8 pbDstV
);

void BT1120UV_To_SplitUV_ex(
	PBYTE8 pbSrcUV,
	int iWidth,
	int iHeight,
	PBYTE8 pbDstU,
	PBYTE8 pbDstV
);


// 200wǰ��25fpsģʽ�µ�Jpeg����ʹ��
void BT1120UV_1600_1200_SplitUV(
	PBYTE8 pbSrc,
	PBYTE8 pbDstU,
	PBYTE8 pbDstV
);

// 1080p�ֱ��ʵ����ģʽ��ʹ��
void BT1120UV_1920_1080_SplitUV(
	PBYTE8 pbSrc,
	PBYTE8 pbDstU,
	PBYTE8 pbDstV
);

void BT1120Field_2448_1024_To_UYVY_2448_2048(
	const PBYTE8 pbImgSrcY, 
	const PBYTE8 pbImgSrcUV, 
	PBYTE8 pbCbYCrY
);

void BT1120_1600_1200_To_UYVY_1600_1200(
	const PBYTE8 pbImgSrcY, 
	const PBYTE8 pbImgSrcUV, 
	PBYTE8 pbCbYCrY
);

/*
void BT1120_To_UYVY(
	const PBYTE8 pbImgSrcY, 
	const PBYTE8 pbImgSrcUV, 
	const int& iWidth,
	const int& iHeight,
	PBYTE8 pbCbYCrY
);
*/

#ifdef __cplusplus
}
#endif

#endif
