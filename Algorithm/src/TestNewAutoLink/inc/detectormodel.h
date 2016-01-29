#ifndef _MODEL_H
#define _MODEL_H

#ifdef __cplusplus
extern "C" {
#endif//#ifdef __cplusplus

#include "config.h"
#include "swPlate.h"
#include "swBaseType.h"
#include "swObjBase.h"
#include "SysUpdate.h"

#define USE_NEW_WJ			// ʹ����WJģ��

//ģ��ת������
#define AlphaCoef			100000			//alpha�ɸ�����ת���ɶ�������Ȩ��.
#define SaveCoef			1000000.0		//���������任�ɶ�����.
#define FinalHCoef			128				//FinalH�ɸ�����ת���ɶ�������Ȩ��(�����߾���)�����õ�.

#define FloatScaleSize			1.09f			//���ű���
#define IntegerScaleSize		(WORD16)(FloatScaleSize * (1 << 5))

#define MAX_SCALE_NUM		    25				//�������ż�

/* ���ʹ�õ�ģ�Ϳ����ݽṹ����. */
typedef struct {
	BYTE8 		bPosX, bPosY, bWidth, bHeight;	//��Ӧ�ڿ����Ͻ�����,����,��߶�
	BYTE8 		bType;							//�������(����featureֵ���㷽ʽ).
} FinalType;

/* ģ��layer��ԭʼ�������ݽṹ. */
typedef struct {
	DWORD32 	dwCascade;			//��layer����Ϊֹ���ж��ٸ�feauterģ�ͼ���
	float 		fltFinalThreshold;	//��layer����ֵ.
} _FeatureGroup;

/* ģ��feature��ԭʼ�������ݽṹ. */
typedef struct {
	FinalType 	FinalType;
	char 		chFinalSign;		//����Ϊ�з��ŵ�8λCHAR��������,������ģ�͵���ʱ������������ֵ�Ƚ�.
	float 		fltFinalL;			//��ǰFeature����ֵ,����ֵ 
	float 		fltFinalH;			//��ǰFeature����ֵ.
	float 		fltAlpha;			//��ǰFeature��Ȩֵ.
} _Feature;

//ԭʼ���ģ�ͽṹ
typedef struct _ModelInfo
{
	int nModelWidth, nModelHeight;
	int nMaxLayer, nMaxFeature;
	const _Feature *pFeature;
	const _FeatureGroup *pFeatureGroup;
	PLATE_TYPE PlateType;
} ModelInfo;

/* ����õ�layer�����ݽṹ. */
typedef struct {
	DWORD32 	dwCascade;			//ͬ��.
	SDWORD32 	fltFinalThreshold;	//������Layer����ֵ.
} FeatureGroupInt;

/* ����õ�feature���ݽṹ. */
typedef struct {
	FinalType 	FinalType;
	char 		chPositiveSign;		
	SDWORD32 	sdwFinalL;			//�������FinalL
	SDWORD32 	sdwFinalH;			//�������FinalH
	SDWORD32 	sdwAlpha;			//�������Alpha
} FeatureBuf;

/* ����õ�ģ�����ݷ�����FLASH�Ĵ洢�ṹ����. */
typedef struct _DET_MODEL_ {
	DWORD32 	dwHeaderSize;			//�ļ�ͷ��С.
	DWORD32 	dwDetModelFileSize;		//ģ�������ļ���С.
	DWORD32 	dwFeatureHeaderSize;	//Featureͷ��С.
	DWORD32 	dwGroupHeaderSize;		//Groupͷ��С.
	DWORD32 	dwLayerCount;			//ģ�ͷ���������.
	DWORD32 	dwFeatureCount;			//ģ����������Ŀ.
	DWORD32 	dwFeatureOffset;		//ģ�͵�Feature��ʼ��ŵ�ƫ����.
	DWORD32 	dwGroupOffset;			//ģ�͵�Group���Կ�ʼ��ŵ�ƫ����.
	DWORD32 	dwModelWidth;			//ģ���г��Ʊ�׼���.
	DWORD32 	dwModelHeight;			//ģ���г��Ʊ�׼�߶�.
	DWORD32 	dwModelPlateType;		//ģ���г�������.
	DWORD32 	dwReserved[2];			//��ǰ������.
} DET_MODEL;

/* �����ʹ�õĺ��Ĺ�������ԭ��. */
typedef int ( *SCAN_scalePlateFxn )( 
	PDWORD32	pdwItgImage,		//ָ��һ�����ͼ������ʼ����.
	WORD16		nStrideWidth,		//����ͼ�����ݱ����Stride���.
	void		*pModel,			//ָ��ģ������.
	WORD16		nScaleNum,			//��ǰʹ���ĸ�Scale���м��.
	WORD16		nVariance			//�����Varianceֵ.
);

/* �����ʹ�õ�ģ�����ݽṹ. */
typedef struct _DPModel {
	FeatureBuf 	*pFeatureBuf;			//ָ������SCALE�µ�feature���ݽṹ����.
	FeatureGroupInt *pFeatureGroup;		//ָ������SCALE�µ�layer���ݽṹ����.
	BOOL 		fInitialized;			//ָʾ��ǰģ�����ݽṹ�Ƿ��ѳ�ʼ��.
	int 		nMaxLayreNum;			//��ģ������layer��.
	int 		nMaxFeatureNum;			//��ģ������feature��.
	int 		nModelWidth;				//ģ�ͼ���Ŀ��.
	int 		nModelHeight;			//ģ�ͼ���ĸ߶�.
	PLATE_TYPE 	PlateType;				//��ģ�͵ĳ�������.
	PLATE_COLOR ModelColor;				//��ģ�͵ĳ���ɫ��.
	int 		nWinSizeScaleWidth[MAX_SCALE_NUM];		//��ģ���ڲ�ͬSCALE����µĿ���С���.
	int 		nWinSizeScaleHeight[MAX_SCALE_NUM];		//��ģ���ڲ�ͬSCALE����µĿ���С�߶�.
	DWORD32 	dwAreaSizeScale[MAX_SCALE_NUM];			//��ģ���ڲ�ͬSCALE����µĿ����.

	FeatureBuf 	*pFastFeatureBuf;			//ָ������SCALE�µ�feature���ݽṹ����.
	int nMinFastScale;
	int nMaxFastScale;
} DPModel;

extern DPModel fiveModel;
extern DPModel blueModel;
extern DPModel yellowModel;
extern DPModel dYellowModel;
extern DPModel wjModel;
extern DPModel DBGreenModel;

extern ModelInfo originModel[];

#ifdef __cplusplus
}
#endif //#ifdef __cplusplus

#endif //#ifndef _MODEL_H
