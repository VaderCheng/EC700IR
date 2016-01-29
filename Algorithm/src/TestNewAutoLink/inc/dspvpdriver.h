#ifndef _DSPVPDRIVER_H
#define _DSPVPDRIVER_H

#include "dspsys_def.h"
#include "VideoBaseType.h"

#ifdef __cplusplus
	extern "C" {
#endif	/* #ifdef __cplusplus */

#define DSP_VPPORT_MAX_NUM			3

/*
����˵��:�ú�������ȡ��ĳһ��Ƶ�ڵ�����ֵ.
��������ֵ:
	����S_OK��ʾ��ȡ����ֵ�ɹ�.
	����E_POINTER��ʾpiBrightnessΪINVALID_POINTER.
	����E_INVALIDARG��ʾ����Ĳ����д�,�ö˿�ϵͳ��ǰ��֧��.
	����E_FAIL��ʾӲ���ײ��������.
	����S_FALSE��ʾ��ǰ����Ƶ�˿�δʹ��. */
HRESULT VIDEO_GetVideoBrightness( 
	int 		iPort, 
	PINT 		piBrightness
);

/*
����˵��:�ú�����������ĳһ��Ƶ�ڵ�����ֵ.
��������ֵ:
	����S_OK��ʾ��������ֵ�ɹ�.	
	����E_INVALIDARG��ʾ����Ĳ����д�,�ö˿�ϵͳ��ǰ��֧��.
	����E_FAIL��ʾӲ���ײ��������.
	����S_FALSE��ʾ��ǰ����Ƶ�˿�δʹ��. */
HRESULT VIDEO_SetVideoBrightness( 
	int 		iPort,
	int 		iBrightness
);

/*
����˵��:�ú�������ȡ��ĳһ��Ƶ�ڵı��Ͷ�ֵ.
��������ֵ:
	����S_OK��ʾ��ȡ���Ͷ�ֵ�ɹ�.
	����E_POINTER��ʾpiSaturationΪINVALID_POINTER.
	����E_INVALIDARG��ʾ����Ĳ����д�,�ö˿�ϵͳ��ǰ��֧��.
	����E_FAIL��ʾӲ���ײ��������;
	����S_FALSE��ʾ��ǰ����Ƶ�˿�δʹ��. */
HRESULT VIDEO_GetVideoSaturation(
	int 		iPort, 
	PINT 		piSaturation,
	int			iType
);

/*
����˵��:�ú�����������ĳһ��Ƶ�ڵı��Ͷ�ֵ.
��������ֵ:
	����S_OK��ʾ���ñ��Ͷ�ֵ�ɹ�.	
	����E_INVALIDARG��ʾ����Ĳ����д�,�ö˿�ϵͳ��ǰ��֧��.
	����E_FAIL��ʾӲ���ײ��������.
	����S_FALSE��ʾ��ǰ����Ƶ�˿�δʹ��. */
HRESULT VIDEO_SetVideoSaturation( 
	int 		iPort, 
	int 		iSaturation,
	int			iType
);

/*
����˵��:�ú�������ȡ��ĳһ��Ƶ�ڵĶԱȶ�ֵ.
��������ֵ:
	����S_OK��ʾ��ȡ�Աȶ�ֵ�ɹ�.
	����E_POINTER��ʾpiContrastΪINVALID_POINTER.
	����E_INVALIDARG��ʾ����Ĳ����д�,�ö˿�ϵͳ��ǰ��֧��.
	����E_FAIL��ʾӲ���ײ��������;
	����S_FALSE��ʾ��ǰ����Ƶ�˿�δʹ��. */
HRESULT VIDEO_GetVideoContrast( 
	int 		iPort, 
	PINT 		piContrast
);

/*
����˵��:�ú�����������ĳһ��Ƶ�ڵĶԱȶ�ֵ.
��������ֵ:
	����S_OK��ʾ���öԱȶ�ֵ�ɹ�.	
	����E_INVALIDARG��ʾ����Ĳ����д�,�ö˿�ϵͳ��ǰ��֧��.
	����E_FAIL��ʾӲ���ײ��������.
	����S_FALSE��ʾ��ǰ����Ƶ�˿�δʹ��. */
HRESULT VIDEO_SetVideoContrast( 
	int 		iPort, 
	int 		iContrast
);

/*
����˵��:�ú�������ȡ��ĳһ��Ƶ�ڵ�ɫ��ֵ.
��������ֵ:
	����S_OK��ʾ��ȡɫ��ֵ�ɹ�.
	����E_POINTER��ʾpiHueΪINVALID_POINTER.
	����E_INVALIDARG��ʾ����Ĳ����д�,�ö˿�ϵͳ��ǰ��֧��.
	����E_FAIL��ʾӲ���ײ��������;
	����S_FALSE��ʾ��ǰ����Ƶ�˿�δʹ��. */
HRESULT VIDEO_GetVideoHue( 
	int 		iPort, 
	PINT 		piHue
);

/*
����˵��:�ú�����������ĳһ��Ƶ�ڵ�ɫ��ֵ.
��������ֵ:
	����S_OK��ʾ����ɫ��ֵ�ɹ�.	
	����E_INVALIDARG��ʾ����Ĳ����д�,�ö˿�ϵͳ��ǰ��֧��.
	����E_FAIL��ʾӲ���ײ��������.
	����S_FALSE��ʾ��ǰ����Ƶ�˿�δʹ��. */
HRESULT VIDEO_SetVideoHue( 
	int 		iPort, 
	int 		iHue
);

/*
����˵��:�ú�������ȡ��ĳһ��Ƶ�ڵ�״̬,���������Ӧ�ò�������.
	����S_OK��ʾȡ״ֵ̬�ɹ�.	
	����E_INVALIDARG��ʾ����Ĳ����д�,�ö˿�ϵͳ��ǰ��֧��.
	����E_FAIL��ʾӲ���ײ��������.
	����S_FALSE��ʾ��ǰ����Ƶ�˿�δʹ��. */
HRESULT VIDEO_GetStatus(
	int				iPort,
	PDWORD32 		pdwValue,
	DWORD32			dwFirstLen,
	PDWORD32		pdwReadLen
);

/*
����˵��:�ú���������λĳһ��Ƶ�ڵ�ǰ��.
	����S_OK��ʾ��λ�ɹ�.	
	����E_INVALIDARG��ʾ����Ĳ����д�,�ö˿�ϵͳ��ǰ��֧��.
	����E_FAIL��ʾӲ���ײ��������.
	����S_FALSE��ʾ��ǰ����Ƶ�˿�δʹ��. */
HRESULT VIDEO_ResetDevice(
	int 			iPort,
	PBOOL 			pfStatus
);

/*
����˵��:�ú���������λĳһ��Ƶ����������.
	����S_OK��ʾ��λ�ɹ�.	
	����E_INVALIDARG��ʾ����Ĳ����д�,�ö˿�ϵͳ��ǰ��֧��.
	����E_FAIL��ʾӲ���ײ��������.
	����S_FALSE��ʾ��ǰ����Ƶ�˿�δʹ��. */
HRESULT VIDEO_ResetDriver(
	int 			iPort,
	PBOOL 			pfStatus
);

/* 
����˵��:�ú����������ûص�����.��ÿ�δ�VP��ȡ��һ��ʱ,�����û�ͨ���ص���������һЩ״ֵ̬.
��������ֵ:
	����S_OK��ʾ���ûص������ɹ�.	
	����E_INVALIDARG��ʾ����Ĳ����д�,�ö˿�ϵͳ��ǰ��֧��.
	����E_POINTER��ʾpfnCaptureCallbackΪINVALID_POINTER.
	����S_FALSE��ʾ��ǰ����Ƶ�˿�δʹ��. */
HRESULT VIDEO_SetCaptureCallback(
	int 			iPort, 
	VIDEO_CALLBACK 	pfnCaptureCallback,
	PVOID 			pvUserData
);

/*
����˵��:�ú�������ȡ�õ�ǰ��Ƶ�˿������㻺��������.
��������ֵ:
	����S_OK��ʾȡ�����������ɹ�.	
	����E_INVALIDARG��ʾ����Ĳ����д�,�ö˿�ϵͳ��ǰ��֧��.
	����E_POINTER��ʾpiCountΪINVALID_POINTER.
	����S_FALSE��ʾ��ǰ����Ƶ�˿�δʹ��. */
HRESULT VIDEO_GetBufferCount( 
	int 			iPort, 
	PINT	 		piCount
);

/*
����˵��:�ú�������ȡ�õ�ǰ��Ƶ�˿������㻺��������.
��������ֵ:
	����S_OK��ʾ���û����������ɹ�.	
	����E_INVALIDARG��ʾ����Ĳ����д�,��ö˿�ϵͳ��ǰ��֧�ֺͻ�������Ŀ����.	
	����S_FALSE��ʾ��ǰ����Ƶ�˿�δʹ��. */
HRESULT VIDEO_SetBufferCount(
	int 			iPort,
	int 			iCount
);

/* 
����˵��:�ú���������ĳһ��Ƶ��ȡ��һ֡ͼ���Լ���ͼ�����Ϣ.
��������ֵ:
	����S_OK��ʾȡ�����������ɹ�.
	����E_INVALIDARG��ʾ����Ĳ����д�,�ö˿�ϵͳ��ǰ��֧��.
	����E_POINTER��ʾpImage��pwImageInfoΪINVALID_POINTER.
	����S_FALSE��ʾ��ǰ����Ƶ�˿�δʹ��. */
HRESULT VIDEO_GetOneFrame(
	int 				iPort,		//��ǰ��������Ƶ��.
	HV_COMPONENT_IMAGE 	*pImage,	//ָ����ȡ�õ�ͼ������.
	VP_IMAGE_INFO		*pImageInfo	//ָ��ǰ��ȡ�õ�ͼ����Ϣ,���Ϊ������.
);

/* 
����˵��:�ú��������ͷ���ȡ�õ���Ƶͼ��.
��������ֵ:
	����S_OK��ʾȡ�����������ɹ�.
	����E_INVALIDARG��ʾ����Ĳ����д�,�ö˿�ϵͳ��ǰ��֧��.
	����E_POINTER��ʾpImageΪINVALID_POINTER.
	����S_FALSE��ʾ��ǰ����Ƶ�˿�δʹ��. */
HRESULT VIDEO_FreeOneFrame(
	int 				iPort,		//��ǰ��������Ƶ��.
	HV_COMPONENT_IMAGE 	*pImage,		//ָ����Ҫ�ͷŵ�ͼ��.
	VP_IMAGE_INFO		*pImageInfo	//ָ��ǰ��ȡ�õ�ͼ����Ϣ,���Ϊ������.
);

HRESULT InitVpVideo( 
	int 				iPort,
	int					iHeap,
	PVOID				pvArg,
	VideoOptionParam	*pParam
);

#ifdef __cplusplus
	}
#endif	//#ifdef __cplusplus

#endif	//#ifndef _VPDRIVER_H

