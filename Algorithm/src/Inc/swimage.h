// ���ļ������ʽ������WINDOWS-936��ʽ

#ifndef __IMAGE_H__
#define __IMAGE_H__

#include "swbasetype.h"
#include "swplate.h"

#ifdef CHIP_6467
#include "DSPLink.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif		// #ifdef __cplusplus

#ifndef PI
#define PI 3.1415926f
#endif

#ifndef CHIP_6467
typedef struct _DSPLinkBuffer_
{
    void *addr; // virtual address for linux(arm)
    unsigned int phys; // physics address for dsp
    unsigned int len;  // len of the buffer
}DSPLinkBuffer;
#endif

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
    HV_IMAGE_YUV_420,			// YUV_420���ͣ����з�ʽ��YYYY....UV....
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

HRESULT ConvertYCbCr2HSV( int iHSVStride, BYTE8 *pbHSVImg, HV_COMPONENT_IMAGE *pYCbCrImg );
HRESULT ConvertYCbCr2HSB( int iHSBStride, BYTE8 *pbHSVImg, HV_COMPONENT_IMAGE *pYCbCrImg, int x, int y, int h, int w );

HRESULT CreateItgImage( int iItgStride, DWORD32 *pdwItgImg, int iSrcHeight, int iSrcWidth, int iSrcStride, BYTE8 *pSrcImg );
HRESULT CreatItgSqImage(
						int iItgStride, DWORD32 *pdwItgSqImg,
						int iSrcHeight, int iSrcWidth, int iSrcStride,
						RESTRICT_PBYTE8 pbSrcImg
						);
HRESULT HvCopyImage( int iDestStride, BYTE8 *pbDestImg, int iSrcHeight, int iSrcWidth, int iSrcStride, BYTE8 *pbSrcImg );
HRESULT CopyComponentImage( HV_COMPONENT_IMAGE *pDestImg, HV_COMPONENT_IMAGE *pSrcImg );
HRESULT ConvertBGR2YCbCr( HV_COMPONENT_IMAGE *pYCbCrImg, int iWidth, int iHeight, int iStride, BYTE8 *pbRGBImg );
HRESULT InvertGrayImage( int iDestStride, BYTE8 *pbDestImg, int iHeight, int iWidth, int iSrcStride, BYTE8 *pbSrcImg );
HRESULT GrayImageGaussian3_3( int iDestStride, BYTE8 *pbDestImg, int iSrcHeight, int iSrcWidth, int iSrcStride, BYTE8 *pbSrcImg );
HRESULT ConvertYCbCr2BGR( int iBGRStride, BYTE8 *pbBGRImg, const HV_COMPONENT_IMAGE *pYCbCrImg );
HRESULT MarkLine(HV_COMPONENT_IMAGE imgSegResult, int iLine, BYTE8 bColor);
HRESULT MarkRect(HV_COMPONENT_IMAGE imgSegResult, HV_RECT rect, BYTE8 bColor);
HRESULT BGR2HSL(BYTE8 bB, BYTE8 bG, BYTE8 bR, BYTE8 *pH, BYTE8 *pS, BYTE8 *pL);
HRESULT YCbCr2BGR(BYTE8 Y, BYTE8 Cb, BYTE8 Cr, PBYTE8 b, PBYTE8 g, PBYTE8 r);
HRESULT YCbCr2HSV(BYTE8 Y, BYTE8 Cb, BYTE8 Cr, PBYTE8 h, PBYTE8 s, PBYTE8 v);
HRESULT IMAGE_ConvertYCbCr2BGR( const HV_COMPONENT_IMAGE *pYCbCrImg, BYTE8* pbR, BYTE8* pbG, BYTE8* pbB,  int iBGRStride);

HRESULT hvImageResizeGray(HV_COMPONENT_IMAGE *hvDst, HV_COMPONENT_IMAGE *hvSrc);  //�ı�ͼ���С

HRESULT hvNormalizeMeanVar(					//��׼�Ĺ�һ������
    HV_COMPONENT_IMAGE* pImageNormalized,	// normalized image, buffer should be located outside
    const HV_COMPONENT_IMAGE* pImageSrc,	// source image
    int fltMean,				            // mean of normalized image
    int fltVar                              // Var. of normalized image
);

HRESULT hvLocalNormalizeMeanVar(			//�ֲ���һ��
    HV_COMPONENT_IMAGE* pImageNormalized,	// normalized image, buffer should be located outside
    const HV_COMPONENT_IMAGE* pImageSrc,	// source image
    int fltMean,				            // mean of normalized image
    int fltVar
);
HRESULT hvLocalNormalizeMeanVar_IR(			//�ֲ���һ��,����汾
    HV_COMPONENT_IMAGE* pImageNormalized,	// normalized image, buffer should be located outside
    const HV_COMPONENT_IMAGE* pImageSrc,	// source image
    int fltMean,				            // mean of normalized image
    int fltVar
);

//�Ҷ�ת���ɶ�ֵ��ͼ
HRESULT GrayToBin( int fIsWhiteChar, RESTRICT_PBYTE8 pbBin, const RESTRICT_PBYTE8 pbYUV, int iX, int iY, int iWidth, int iHeight, int iStride );
HRESULT IMAGE_GrayToBin(HV_COMPONENT_IMAGE imgSrc, RESTRICT_PBYTE8 pbBin);
HRESULT IMAGE_CalcBinaryThreshold(HV_COMPONENT_IMAGE imgSrc, int iA1, int iA2, PBYTE8 pbThreshold);
HRESULT IMAGE_Gray2BinNew( int iIsWhiteChar, BYTE8 *pbBinImg,  HV_COMPONENT_IMAGE imgFrame ,int nContrast) ;
//���ŻҶ�ͼ
HRESULT ScaleGrey(
    RESTRICT_PBYTE8 pDstImg,
    int nDstWidth,
    int nDstHeight,
    int nDstStride,
    const RESTRICT_PBYTE8 pSrcImg,
    int nSrcWidth,
    int nSrcHeight,
    int nSrcStride
);

// �����ĵĻҶȼ����ֵ����ֵ������
int CalcBinaryThreByCenter(const BYTE8 *pbYUV, int iWidth, int iHeight, int iStride, HV_RECT *rtCenter);
// ����ֵ��ֵ���������ֵ��ͼ
int GrayToBinByThre(BOOL fIsWhiteChar, const RESTRICT_PBYTE8 pbYUV, int iWidth, int iHeight, int iStride, int iThre, RESTRICT_PBYTE8 pbBin);

//�Ҷ�ת���ɶ�ֵ��ͼ,��������λ�ü�����ֵ
HRESULT GrayToBinByCenter(
    int fIsWhiteChar, RESTRICT_PBYTE8 pbBin, const RESTRICT_PBYTE8 pbGrey,
    int iWidth, int iHeight, int iStride, HV_RECT *rtCenter
);

// Sobel ������ֱ��ˮƽ��Ե
HRESULT IMAGE_sobel_V_NEW(
    RESTRICT_PBYTE8  pbIn,
    RESTRICT_PBYTE8  pbOut,
    int iCols,
    int iRows,
    int	iStride
);

HRESULT IMAGE_sobel_V(
    RESTRICT_PBYTE8  pbIn,
    RESTRICT_PBYTE8  pbOut,
    int     iCols,
    int     iRows
);

HRESULT hvImageSobelV(
    HV_COMPONENT_IMAGE imgSrc,
    HV_COMPONENT_IMAGE imgDst
);

HRESULT IMAGE_sobel_H(
    RESTRICT_PBYTE8  pbIn,
    RESTRICT_PBYTE8  pbOut,
    int iCols,
    int iRows
);

HRESULT hvImageSobelH(
    HV_COMPONENT_IMAGE imgSrc,
    HV_COMPONENT_IMAGE imgDst
);

HRESULT IMAGE_sobel_New(
    RESTRICT_PBYTE8  pbIn,
    RESTRICT_PBYTE8  pbOut,
    int iCols,
    int iRows
);

int IMAGE_SegLocalMeanBin(
    HV_COMPONENT_IMAGE* pImg,
    BYTE8 *pbBinImg,
    int iIsWhiteChar
);

typedef struct _COLOR_INFO
{
    PLATE_COLOR nPlateColor;

    BYTE8 nCharType;		//0: ���� 1: ���� 2: δ֪

    BYTE8 Hue_1;	//������ƽ��ɫ��
    BYTE8 Sat_1;	//������ƽ�����϶�

    BYTE8 Hue_0;	//������ƽ��ɫ��
    BYTE8 Sat_0;	//������ƽ�����϶�
}
COLOR_INFO;

HRESULT GetColorInfo(
    const HV_COMPONENT_IMAGE* pImage,
    COLOR_INFO* pColorInfo
);

//��ͨ�����⻯
HRESULT GreyEqualization(HV_COMPONENT_IMAGE image);

//BGR���⻯
HRESULT BgrEqualization(PBYTE8 pBGR, int iWidth, int iHeight, int iStride);

//�Ҷ�����
void GrayStretch(
    RESTRICT_PBYTE8 pBuf,
    DWORD32 nWidth,
    DWORD32 nHeight,
    DWORD32 nStrideWidth,
    BYTE8 nMin,
    BYTE8 nMax
);
void GrayStretch_IR(
					RESTRICT_PBYTE8 pBuf,
					DWORD32 nWidth,
					DWORD32 nHeight,
					DWORD32 nStrideWidth,
					BYTE8 nMin,
					BYTE8 nMax
				 );
void GrayStretch_IR2(
					 RESTRICT_PBYTE8 pBuf, 
					 DWORD32 nWidth, 
					 DWORD32 nHeight, 
					 DWORD32 nStrideWidth,
					 BYTE8 nMin,
					 BYTE8 nMax,
					 int nMinY,
					 int nMaxY
				  );

//ͼ����ǿ, �ҶȰ��ֲ�����+ǿ����Ե
HRESULT PlateEnhance( HV_COMPONENT_IMAGE* pSrcImg );
HRESULT PlateEnhance_IR( HV_COMPONENT_IMAGE* pSrcImg );
HRESULT PlateEnhance_IR2( HV_COMPONENT_IMAGE* pSrcImg ,int nMinY,int nMaxY);

//��������
HRESULT PlateResize(HV_COMPONENT_IMAGE imgSrc, HV_COMPONENT_IMAGE imgDst);
HRESULT PlateResizeYUV420ToYUV422(HV_COMPONENT_IMAGE imgSrc, HV_COMPONENT_IMAGE imgDst);

// ͼ����ʱ����ת90��
HRESULT ImageRotate90(HV_COMPONENT_IMAGE imgSrc, HV_COMPONENT_IMAGE imgDst);

int CaclPlateLight(HV_COMPONENT_IMAGE imgCropPlate);

void EnhanceColor(HV_COMPONENT_IMAGE* pImg);

//�����ڰ���
HRESULT ProcessBWPlate( HV_COMPONENT_IMAGE* pSrcImg );

HRESULT IMAGE_sobel_New_XM(
    RESTRICT_PBYTE8  pbIn,
    RESTRICT_PBYTE8  pbOut,
    int iCols,
    int iRows
);

// ��򷨶�ֵ��
HRESULT Otsu(HV_COMPONENT_IMAGE* pImage, HV_COMPONENT_IMAGE* pImageOut,  int* piCount, int iIsWhiteChar);

HRESULT OtsuForThresh(HV_COMPONENT_IMAGE* pImage, int* piThresh, int *iCount1);

// �����˨��λ��
HRESULT GetNailPos(HV_COMPONENT_IMAGE imgPlate,
				   HV_RECT rgrcCharPos,
				   int* piCharTop,
				   int* piCharDown,
				   int* piBottomLine,
				   PLATE_COLOR PlateColor);
// �����˨��λ��,����汾
HRESULT GetNailPos_IR(HV_COMPONENT_IMAGE imgPlate,
				   HV_RECT rgrcCharPos,
				   int* piCharTop,
				   int* piCharDown,
				   int* piBottomLine,
				   PLATE_COLOR PlateColor);
				   
#ifdef __cplusplus
}
#endif		// #ifdef __cplusplus

#endif		// #ifndef __IMAGE_H__
