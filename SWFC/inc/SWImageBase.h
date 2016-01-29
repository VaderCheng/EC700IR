#ifndef _SWIMAGEBASE_H_
#define _SWIMAGEBASE_H_

#include "SWBaseType.h"

/**
 * @brief ͼ������ö��
 */
typedef enum
{
	SW_IMAGE_YUV_422 = 0,
	SW_IMAGE_YUV_411,
	SW_IMAGE_CbYCrY,
	SW_IMAGE_YCbYCr,
	SW_IMAGE_BGR,
	SW_IMAGE_GRAY,
	SW_IMAGE_HSV,
	SW_IMAGE_HSB,
	SW_IMAGE_BIN,
	SW_IMAGE_JPEG,              // jpegѹ������
	SW_IMAGE_BT1120,            // BT1120���ͣ����з�ʽ��YYYY....UVUV....
	SW_IMAGE_RAW12,             // Bayer12ԭʼ��������
	SW_IMAGE_BT1120_ROTATE_Y,   // Y��ʱ����ת90�Ȳ���ȡ��������UV���ֲ��䡣
	SW_IMAGE_BT1120_UV,         // ��BT1120�Ľ�֯��UV���з������ڵ������ε�����
	SW_IMAGE_H264,              // h.264ѹ������
	SW_IMAGE_H264_HISTORY,              // h.264ѹ������
	SW_IMAGE_BT1120_FIELD,      // ��BT1120������������ȡ�������.
	SW_IMAGE_YUV420SP,
	SW_IMAGE_TYPE_COUNT
}
SW_IMAGE_TYPE;

/**
 * @brief ͼ�����ݽṹ
 */
typedef struct _tag_component_image
{
	SW_IMAGE_TYPE  cImageType;		// ͼ������
	PBYTE rgpbData[3];	    // ͼ���ڴ�����
	DWORD phys[3];          // ͼ�������ַ
	DWORD len[3];           // ÿ��������ͼ���С
	INT rgiStrideWidth[3];  // ͼ��洢���(���ֽ�Ϊ��λ).
	INT iWidth;					    // ͼ��ʵ�ʿ��
	INT iHeight;				    // ͼ��ʵ�ʸ߶�
	INT iSize;						  // ͼ���С
} SW_COMPONENT_IMAGE;

/*
//-=- ���ݽṹ����
typedef struct _DSPLinkBuffer_
{
	void *addr; // virtual address for linux(arm)
	unsigned int phys; // physics address for dsp
	unsigned int len;  // len of the buffer
}DSPLinkBuffer;

typedef enum
{
	HV_IMAGE_YUV_422 = 0,
	HV_IMAGE_YUV_411,
	HV_IMAGE_CbYCrY,
	HV_IMAGE_YCbYCr,
	HV_IMAGE_BGR,
	HV_IMAGE_GRAY,
	HV_IMAGE_HSV,
	HV_IMAGE_HSB,
	HV_IMAGE_BIN,
	HV_IMAGE_JPEG,              // jpegѹ������
	HV_IMAGE_BT1120,            // BT1120���ͣ����з�ʽ��YYYY....UVUV....
	HV_IMAGE_RAW12,             // Bayer12ԭʼ��������
	HV_IMAGE_BT1120_ROTATE_Y,   // Y��ʱ����ת90�Ȳ���ȡ��������UV���ֲ��䡣
	HV_IMAGE_BT1120_UV,         // ��BT1120�Ľ�֯��UV���з������ڵ������ε�����
	HV_IMAGE_H264,              // h.264ѹ������
	HV_IMAGE_BT1120_FIELD,      // ��BT1120������������ȡ�������
	HV_IMAGE_TYPE_COUNT
} HV_IMAGE_TYPE;

typedef struct _HV_COMPONENT_IMAGE
{
	DSPLinkBuffer rgImageData[3];   // ͼ������
	int iStrideWidth[3];		    // ͼ��洢���(���ֽ�Ϊ��λ).
	int iHeight;				    // ͼ��߶�
	int iWidth;					    // ͼ��ʵ�ʿ��
	HV_IMAGE_TYPE nImgType;		    // ͼ������
	int iMemPos;				    // ͼ�������ڴ�λ�� 0:Ƭ���ڴ� 1:Ƭ���ڴ� 2:�����ڴ棨DM6467��
} HV_COMPONENT_IMAGE;
//===========
*/

#endif

