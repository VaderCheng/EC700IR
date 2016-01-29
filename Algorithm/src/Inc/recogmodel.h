#ifndef _MODEL_INCLUDED__
#define _MODEL_INCLUDED__

#include "swbasetype.h"
#include "hvutils.h"

// TODO: �����������Щ������ô�죿
#define MAX_LAYER_UNITS_NUM 256			// NNģ�Ͳ����Ԫ��
#define MAX_SVM_FEATURE_NUM	80			// SVM ���������
#define MAX_SVM_CLASS_NUM	45			// SVM ���ʶ������
#define MAX_SVM_SV_NUM	2100			// SVM ���֧�ֵ���������

// ģ�����Ͷ���
#define TYPE_SVM 0
#define TYPE_NN	 1
#define TYPE_TEMP 2

// ģ����Ϣ�����
typedef struct
{
	char name[20];					// ģ����
	INT year;						// �����գ�����3����ʾ�汾��Ϣ
	INT month;
	INT day;
} StModelInfo;


// ����NNģ���õ��Ľṹ
typedef struct                    /* A LAYER OF A NET:                     */
{
	INT				Num_Units;	  /* - number of units in this layer       */
	SDWORD32*		Output;       /* - output of ith unit                  */
	SDWORD32*		Weight;       /* - connection weights to ith unit      */
	SDWORD32*		Bias;         /* - Bias added to each units            */
} LAYER;

typedef struct                    /* A NET:                                */
{
	INT           Num_Layers;     /* The number of layers                  */
	LAYER*        Layer;          /* - layers of this net                  */
	LAYER*        InputLayer;     /* - input layer                         */
	LAYER*        OutputLayer;    /* - output layer                        */
	INT*          nLabel;         /* �ַ��±�*/
	StModelInfo*  info;
} NET;

// ����SVMģ���õ��Ľṹ
struct svm_parameter
{
	SDWORD32 gamma;				  /* for poly/rbf/sigmoid */
};

struct svm_model
{
	svm_parameter *param;	// parameter
	INT nr_class;			// number of classes, = 2 in regression/one class svm
	INT l;					// total #SV
	SDWORD32 *SV;			// SVs (SV[l])
	SDWORD32 *sv_coef;		// coefficients for SVs in decision functions (sv_coef[n-1][l])
	SDWORD32 *rho;			// constants in decision functions (rho[n*(n-1)/2])
	// for classification only
	INT *label;				// label of each class (label[n])
	INT *nSV;				// number of SVs for each class (nSV[n])
							// nSV[0] + nSV[1] + ... + nSV[n-1] = l
	INT size;				// size of input image
	INT nFeatureNum;		// number of Features
	SDWORD32 *Mean;			// PCA mean vector
	SDWORD32 *PrjMatrix;	// PCA projection matrix
	StModelInfo*  info;
};

// ģ���ļ�ͷ��Ϣ
typedef struct
{
	char modelFlag;					// ģ�ͱ�־	0:SVM 1:NN
	char name[19];					// ģ����
	INT year;						// �����գ�����3�����ֱ�ʾ�汾��Ϣ
	INT month;
	INT day;
} StFileHead;

//ģ�����ݿ�ͷ��Ϣ����
typedef struct
{
	DWORD32 DataCount;					// ��������(����������ͷ)

	DWORD32 ParaOff;
	DWORD32 rhoOff;
	DWORD32 nSVOff;
	DWORD32 labelOff;

	DWORD32 NodeOff;					// ����4��Ϊ�������ݿ��ƫ��
	DWORD32 CoefOff;
	DWORD32 MeanOff;
	DWORD32 PrjMatrixOff;

	DWORD32 ModelOff;

	DWORD32 InfoOff;

	DWORD32 Res[9];						// ����
} SVMFileHead;

typedef struct
{
	DWORD32 DataCount;					// ��������(����������ͷ)

	DWORD32 rgiOutput0Off;
	DWORD32 rgiOutput1Off;
	DWORD32 rgiOutput2Off;

	DWORD32 Weight1Off;
	DWORD32 Bias1Off;
	DWORD32 Weight2Off;
	DWORD32 Bias2Off;

	DWORD32 rglayerOff;					// ����4��Ϊ�������ݿ��ƫ��
	DWORD32 rgiLabelOff;
	DWORD32 knetOff;

	DWORD32 InfoOff;
	DWORD32 Res[8];						// ����
} NNFileHead;

typedef struct
{
	DWORD32 DataCount;					// ��������(����������ͷ)

	DWORD32 rgcfCharOff;
	DWORD32 Res[18];					//����
} TEMPFileHead;

// ģ��ʹ�õ�ȫ�ֱ���

// #�����֡���ĸ����������#��I��ģ��������0��ģ�����
const int g_nSymbols = 38;
const int g_cnStdCharWidth = 14;
const int g_cnStdCharHeight = 14;
const int g_cnStdCharLength = g_cnStdCharWidth * g_cnStdCharHeight;

const int g_cnStdChnWidth = 16;
const int g_cnStdChnHeight = 16;
const int g_cnStdChnLength = g_cnStdChnWidth * g_cnStdChnHeight;

const int g_cnLogoChnWidth = 24;
const int g_cnLogoChnHeight = 24;
const int g_cnLogoChnLength = g_cnLogoChnWidth * g_cnLogoChnHeight;
const int g_cnNomalizedMean = 80;	// for image normalized
const int g_cnNomalizedStdVar = 128;

// �ַ�������
extern const WORD16 g_wszCharSet[];			// �ַ�����
extern const int g_cnChars ;				// �ַ����ϳ���
extern int g_nDagJun_BCEGHJKLNSVZ_;

// ģ�ͺ�����
// �ַ�
void Distribute(
	const SDWORD32* pfBuf,
	int nWidth, int nHeight, int nOffset,
	SDWORD32& fMean,
	SDWORD32& fVar
);
// �ַ�
void DistributeGray(
	const SDWORD32* pfBuf,
	int nWidth, int nHeight, int nOffset,
	SDWORD32& fMean,
	SDWORD32& fVar
);
// ����ˮƽ����
SDWORD32 GetHorizonCenter(
	const SDWORD32* pfSample,
	int nWidth, int nHeight);

// SVM ������ֵ
SDWORD32 kernelEvaluation(
	const SDWORD32 *x,
	const SDWORD32 *y,
	const svm_parameter* param,
	const int nFeatureNum
);


// ���Ŷ�ģ���
extern SDWORD32 * g_rgcfCharTemplate;

#endif // _MODEL_INCLUDED__
