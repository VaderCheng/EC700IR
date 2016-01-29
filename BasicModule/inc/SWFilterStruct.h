/**
* @file SWFilterStruct.h 
* @brief Filter���ⷢ���Ľṹ�塣
* @copyright Signalway All Rights Reserved
* @author zhaopy
* @date 2013-04-02
* @version 1.0
*/

#ifndef _SWFILTER_STRUCT_H_
#define _SWFILTER_STRUCT_H_

#include "SWBaseType.h"

// CSWNetSourceFilter
typedef struct tag_CameraInfo
{
	INT iCameraType;
	CHAR szIp[32];
	WORD wPort;
}
CAMERA_INFO;

// CSWH264SourceFilter
typedef struct tag_OverlayInfo
{
	INT iTopX; //����X
	INT iTopY;//����Y
	INT iFontSize;//�����С��������128
	DWORD dwColor;// ����RGB��ɫ
	BOOL fEnableTime;// ʹ��ʱ����ʾ
	INT	iTextLen;// �����ַ�������
	CHAR szInfo[512]; // �����ַ���
}
H264_OVERLAY_INFO;

typedef struct _JPEG_OVERLAY_INFO
{
	BOOL fEnable;	       //�Ƿ�ʹ��JPEG�ַ�����
	INT  iX;             //����λ��
	INT  iY;             //����λ��
	INT  iFontSize;      //�����ַ���С
	INT  iColor;         //������ɫ
	BOOL fEnableTime;    //����ʱ��
	CHAR szInfo[255];    //���Ӹ�����Ϣ
	BOOL fEnablePlate;   //ʶ�����Ƿ�Ҫ���ӳ��ƺ���
}JPEG_OVERLAY_INFO;

typedef struct tag_H264Param
{
	INT iForceFrameType; //ǿ����һ֡������ͣ���ǰ��Ч��Ӧ����Ϊ-1
	INT iTargetBitrate; // ���������ʣ�32Kbps-16Mbps
	INT intraFrameInterval; // I֡���
	INT iResolution; // H.264�ֱ���
	INT iRateControl;	//���ʿ��� 0��VBR 1��CBR
	INT iVbrDuration;   //�����仯�Ĳ���ʱ��
	INT iVbrSensivitity; //�����仯�����ж�
}
H264_PARAM;

typedef struct tag_H264FilterParam
{
	H264_OVERLAY_INFO cOverlayInfo;
	H264_PARAM cH264Param;
	INT iChannelNum;		//ͨ���ţ��������ֲ�ͬ��H264
}
H264_FILTER_PARAM;


#endif
