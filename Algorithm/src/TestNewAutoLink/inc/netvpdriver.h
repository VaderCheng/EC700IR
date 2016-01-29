#ifndef _NETVPDRIVER_H
#define _NETVPDRIVER_H

#include "dspsys_def.h"
#include "videoBaseType.h"

#ifdef __cplusplus
	extern "C" {
#endif	/* #ifdef __cplusplus */

PBYTE8 GetNetPacketBuffPtr( void );

HRESULT NET_InOneFrame(
	PBYTE8 				pbData,
	DWORD32				dwLen
);

HRESULT NET_GetOneFrame( 
	int 				iPort,		//��ǰ��������Ƶ��.
	HV_COMPONENT_IMAGE 	*pImage,	//ָ����ȡ�õ�ͼ������.
	VP_IMAGE_INFO		*pImageInfo	//ָ��ǰ��ȡ�õ�ͼ����Ϣ,���Ϊ������.
);

HRESULT NET_FreeOneFrame(
	int 				iPort,		//��ǰ��������Ƶ��.
	HV_COMPONENT_IMAGE 	*pImage,		//ָ����Ҫ�ͷŵ�ͼ��.
	VP_IMAGE_INFO		*pImageInfo	//ָ��ǰ��ȡ�õ�ͼ����Ϣ,���Ϊ������.
);

HRESULT NET_InOneFrame(
	PBYTE8 				pbData,
	DWORD32				dwLen
);

HRESULT NET_GetBufferCount(
	int					iPort,
	PINT 				piCount
);

HRESULT NET_SetBufferCount(
	int 				iPort,
	int		 			iCount
);

HRESULT NET_SetCallback(
	int 				iPort, 
	VIDEO_CALLBACK 		pfnCallback,
	PVOID 				pvUserData
);

HRESULT InitNetVideo(
	int 				iPort, 
	int					iHeap,
	PVOID				pvArg,
	VideoOptionParam 	*pParam
);

#ifdef __cplusplus
	}
#endif	/* #ifdef __cplusplus */

#endif	//#ifndef _NETINPUTPROCESS_H

