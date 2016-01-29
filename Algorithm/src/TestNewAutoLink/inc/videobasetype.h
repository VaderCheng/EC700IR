#ifndef _VIDEO_PRIVATE_H
#define _VIDEO_PRIVATE_H

#ifdef __cplusplus
	extern "C" {
#endif	/* #ifdef __cplusplus */

#include "swBaseType.h"

/* �ص���������. */
typedef HRESULT VIDEO_CALLBACK( PVOID pvUserData, PVOID pvImgInfo, PVOID pvImage );

/* ��Ƶ֡��Ϣ���ݽṹ����. */
typedef struct {
	DWORD32			dwTime;		//ȡ����Ƶʱ��.
	DWORD32			dwFlag;		//ĳЩ��ʶ.
	DWORD32			dwFrameNo; //��Ƶ���.
	PVOID			pvCustom;	//ָ������һЩ��Ϣ.
} VP_IMAGE_INFO;

typedef struct tagVideoOptionParam {
	int 			iBuffMode;
	int				iBuffNum;
	int 			iBuffCtrlNum;
	int				iWidth;
	int				iHeight;
	int				iBrightness;
	int				iContrast;
	int				iSaturation;
	int 			iSaturation_U;
	int				iSaturation_V;
	int				iHue;
	int				iMode;
	int 			iImageType;
	int				iMaxWidth;
	int				iMaxHeight;
} VideoOptionParam;

typedef enum {
	IMAGE_FM_JPEG,
	IMAGE_FM_YUV,
	IMAGE_FM_RGB,
	IMAGE_MAX_FM_NUM
} IMAGE_FILE_FORMAT;

typedef enum {
	VP_SAT_UV = 0,
	VP_SAT_U = 1,
	VP_SAT_V = 2
} VP_SAT_TYPE;

#ifdef __cplusplus
	}
#endif	/* #ifdef __cplusplus */

#endif
