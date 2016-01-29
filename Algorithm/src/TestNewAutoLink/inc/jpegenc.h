#ifndef _JPEGENC_H
#define _JPEGENC_H

#ifdef __cplusplus
	extern "C" {
#endif	/* #ifdef __cplusplus */

#include "dspsys_def.h"

/*
����˵��:
	�Դ����ԭʼYUVͼ�����JPEGѹ������.
	����ṩ��JPEG Buf��������ͨ��pdwImgSize������Ҫ�Ĵ�С	
��������ֵ:
	����S_OK��ʾJPEGѹ���ɹ�.
	����E_POINTER��ʾpImage����pbJpegImgDataΪINVALID_POINTER.
	����E_INVALIDARG��ʾ����Ĳ����д�.
	����E_FAIL��ʾJPEGѹ��ʧ��. */
extern HRESULT Jpeg_Enc(
	HV_COMPONENT_IMAGE 	*pImage,		// ����ͼ��
	int 				nQuality, 		// JPEGͼ������.
	PBYTE8 				pbJpegImgData, 	// ָ��JPEGѹ���������.
	DWORD32				dwMaxImgSize,	// JPEGѹ������������ֽڳ���.
	PDWORD32			pdwImgSize,		// JPEGѹ�������ݵ�ʵ���ֽڳ���, ΪNULL��������.
	BOOL				fVScale			// ��ֱ�����Ƿ�����,0=����, 1=����
);

/*
����˵��:
	�Դ����ԭʼYUVͼ�����JPEG��ѹ����.
��������ֵ:
	����S_OK��ʾJPEG��ѹ�ɹ�.
	����E_POINTER��ʾpbJpegImgData����pImageΪINVALID_POINTER.
	����E_INVALIDARG��ʾ����Ĳ����д�.
	����E_FAIL��ʾJPEG��ѹʧ��. */
extern HRESULT Jpeg_Dec(
	PBYTE8 				pbJpegImgData, 	// ָ��JPEG����.
	DWORD32				dwDataLen,		// JPEG���ݳ���.
	HV_COMPONENT_IMAGE 	*pImage			// ���ͼ��
);


#ifdef __cplusplus
	}
#endif	//#ifdef __cplusplus

#endif	//#ifndef _JPEGENC_H
