#ifndef __IMAGE_H__
#define __IMAGE_H__

#include "swBaseType.h"
#include "swPlate.h"

#ifdef __cplusplus
extern "C" {
#endif		// #ifdef __cplusplus

#ifndef PI
#define PI 3.1415926f
#endif

typedef enum {
	HV_IMAGE_YUV_422 = 0,
	HV_IMAGE_YUV_411,
	HV_IMAGE_CbYCrY,
	HV_IMAGE_YCbYCr,
	HV_IMAGE_BGR,
	HV_IMAGE_GRAY,
	HV_IMAGE_HSV,
	HV_IMAGE_HSB,
	HV_IMAGE_BIN,
	HV_IMAGE_JPEG,			// jpegѹ������
	HV_IMAGE_TYPE_COUNT
} HV_IMAGE_TYPE;

typedef struct _HV_COMPONENT_IMAGE {
	BYTE8 *pbData[3];			// ͼ������
	int iStrideWidth[3];		// ͼ��洢���(���ֽ�Ϊ��λ).
	int iHeight;				// ͼ��߶�
	int iWidth;					// ͼ��ʵ�ʿ��
	HV_IMAGE_TYPE nImgType;		// ͼ������
} HV_COMPONENT_IMAGE;

HRESULT ConvertYCbCr2HSV( int iHSVStride, BYTE8 *pbHSVImg, HV_COMPONENT_IMAGE *pYCbCrImg );
HRESULT ConvertYCbCr2HSB( int iHSBStride, BYTE8 *pbHSVImg, HV_COMPONENT_IMAGE *pYCbCrImg, int x, int y, int h, int w );

HRESULT CreateItgImage(
	int iItgStride, DWORD32 *pdwItgImg, int iSrcHeight, int iSrcWidth, int iSrcStride, BYTE8 *pSrcImg );
HRESULT HvCopyImage(
	int iDestStride, BYTE8 *pbDestImg, int iSrcHeight, int iSrcWidth, int iSrcStride, BYTE8 *pbSrcImg );
HRESULT CopyComponentImage( HV_COMPONENT_IMAGE *pDestImg, HV_COMPONENT_IMAGE *pSrcImg );
HRESULT ConvertBGR2YCbCr(
	HV_COMPONENT_IMAGE *pYCbCrImg, int iWidth, int iHeight, int iStride, BYTE8 *pbRGBImg );

HRESULT ConvertBGR2YCbCrEx(
			 HV_COMPONENT_IMAGE *pYCbCrImg, int iWidth, int iHeight, int iStride, BYTE8 *pbRGBImg );
HRESULT ConvertBayer2YCbCr(
			   HV_COMPONENT_IMAGE *pYCbCrImg,
			   int iWidth, int iHeight, int iStride,
			   BYTE8 *pbBayerBuff
			   );
HRESULT InvertGrayImage(
	int iDestStride, BYTE8 *pbDestImg, int iHeight, int iWidth, int iSrcStride, BYTE8 *pbSrcImg );
HRESULT GrayImageGaussian3_3(
	int iDestStride, BYTE8 *pbDestImg, int iSrcHeight, int iSrcWidth, int iSrcStride, BYTE8 *pbSrcImg );
HRESULT ConvertYCbCr2BGR( int iBGRStride, BYTE8 *pbBGRImg, const HV_COMPONENT_IMAGE *pYCbCrImg );
HRESULT ConvertYCbCr2BGREx( int iBGRStride, BYTE8 *pbBGRImg, const HV_COMPONENT_IMAGE *pYCbCrImg );
HRESULT MarkLine(HV_COMPONENT_IMAGE imgSegResult, int iLine, BYTE8 bColor);
HRESULT MarkRect(HV_COMPONENT_IMAGE imgSegResult, HV_RECT rect, BYTE8 bColor);
HRESULT YCbCr2BGR(BYTE8 Y, BYTE8 Cb, BYTE8 Cr, PBYTE8 b, PBYTE8 g, PBYTE8 r);
HRESULT YCbCr2HSV(BYTE8 Y, BYTE8 Cb, BYTE8 Cr, PBYTE8 h, PBYTE8 s, PBYTE8 v);
HRESULT IMAGE_ConvertYCbCr2BGR( const HV_COMPONENT_IMAGE *pYCbCrImg, BYTE8* pbR, BYTE8* pbG, BYTE8* pbB,  int iBGRStride);

HRESULT hvImageResizeGray(HV_COMPONENT_IMAGE *hvDst, HV_COMPONENT_IMAGE *hvSrc);	//�ı�ͼ���С
HRESULT hvNormalizeMeanVar(										//��׼�Ĺ�һ������
	HV_COMPONENT_IMAGE* pImageNormalized,	// normalized image, buffer should be located outside
	const HV_COMPONENT_IMAGE* pImageSrc,	// source image
	int fltMean,				// mean of normalized image
	int fltVar);				// Var. of normalized image
HRESULT hvLocalNormalizeMeanVar(									//�ֲ���һ��
	HV_COMPONENT_IMAGE* pImageNormalized,	// normalized image, buffer should be located outside
	const HV_COMPONENT_IMAGE* pImageSrc,	// source image
	int fltMean,				// mean of normalized image
	int fltVar);

//�Ҷ�ת���ɶ�ֵ��ͼ
HRESULT GrayToBin( int fIsWhiteChar, RESTRICT_PBYTE8 pbBin, const RESTRICT_PBYTE8 pbYUV, int iX, int iY, int iWidth, int iHeight, int iStride );
HRESULT IMAGE_GrayToBin( HV_COMPONENT_IMAGE imgSrc, RESTRICT_PBYTE8 pbBin);
HRESULT IMAGE_CalcBinaryThreshold(HV_COMPONENT_IMAGE imgSrc, int iA1, int iA2, PBYTE8 pbThreshold);

//���ŻҶ�ͼ
HRESULT ScaleGrey(
				  RESTRICT_PBYTE8 pDstImg,
				  int nDstWidth,
				  int nDstHeight,
				  int nDstStride,
				  const RESTRICT_PBYTE8 pSrcImg,
				  int nSrcWidth,
				  int nSrcHeight,
				  int nSrcStride);

// �����ĵĻҶȼ����ֵ����ֵ������
int CalcBinaryThreByCenter(const BYTE8 *pbYUV, int iWidth, int iHeight, int iStride, HV_RECT *rtCenter);
// ����ֵ��ֵ���������ֵ��ͼ
int GrayToBinByThre(BOOL fIsWhiteChar, const RESTRICT_PBYTE8 pbYUV, int iWidth, int iHeight, int iStride, int iThre, RESTRICT_PBYTE8 pbBin);

//�Ҷ�ת���ɶ�ֵ��ͼ,��������λ�ü�����ֵ
HRESULT GrayToBinByCenter(int fIsWhiteChar, RESTRICT_PBYTE8 pbBin, const RESTRICT_PBYTE8 pbGrey,
						  int iWidth, int iHeight, int iStride, HV_RECT *rtCenter);

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
					  HV_COMPONENT_IMAGE imgDst);

HRESULT IMAGE_sobel_H(
	RESTRICT_PBYTE8  pbIn,
	RESTRICT_PBYTE8  pbOut,
	int iCols,
	int iRows
);

HRESULT hvImageSobelH(
					  HV_COMPONENT_IMAGE imgSrc,
					  HV_COMPONENT_IMAGE imgDst);

HRESULT IMAGE_sobel_New(
	RESTRICT_PBYTE8  pbIn,
	RESTRICT_PBYTE8  pbOut,
	int iCols,
	int iRows
);

HRESULT IMAGE_sobel_New_XM(
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

//ͼ����ǿ, �ҶȰ��ֲ�����+ǿ����Ե
HRESULT PlateEnhance( HV_COMPONENT_IMAGE* pSrcImg );

//�����ڰ���
HRESULT ProcessBWPlate( HV_COMPONENT_IMAGE* pSrcImg );
//��������
HRESULT PlateResize(HV_COMPONENT_IMAGE imgSrc, HV_COMPONENT_IMAGE imgDst);

// ͼ����ʱ����ת90��
HRESULT ImageRotate90(HV_COMPONENT_IMAGE imgSrc, HV_COMPONENT_IMAGE imgDst);

#ifdef __cplusplus
}
#endif		// #ifdef __cplusplus

#endif		// #ifndef __IMAGE_H__

