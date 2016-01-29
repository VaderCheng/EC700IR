#ifndef _RECOGNITION_INCLUDED__
#define _RECOGNITION_INCLUDED__

#include "swobjbase.h"
#include "swimage.h"
#include "swbasetype.h"
#include "swplate.h"
#include "inspector.h"
//#include "trackerdef.h"
//#include "charquotietymatrix.h"
#include "hvcoretype.h"
#include "hvinterface.h"

using namespace HiVideo;

// ����ģʽ, SetWorkMode()������nProcMode����ʹ��
#define RECOG_MODE_DEFAULT 0
#define RECOG_MODE_IR     1

typedef struct _RECOGNITION_CFG_PARAM
{
	BOOL fEnableAlphaRecog; 
	BOOL fEnableT1Model;
	BOOL fEnableAlpha_5;
	BOOL fEnableEnhanceIR_GX;  // ��ǿ�����������ģ�Ϳ���
	_RECOGNITION_CFG_PARAM()
	{
		fEnableAlphaRecog = FALSE;
		fEnableT1Model = FALSE;
		fEnableAlpha_5 = FALSE;
		fEnableEnhanceIR_GX = FALSE;
	}
} RECOGNITION_CFG_PARAM;

#define QUANTIFY_DIGIT	16			//����λ����2���ݣ���16��ʾ65536

#define RECOGNITION_TYPE_LIST(MACRO_NAME)	\
	MACRO_NAME(ALL=0, "ȫ")                 \
	MACRO_NAME(ALLMT, "Ħ��ȫ")             \
	MACRO_NAME(DIGIT, "����")	            \
	MACRO_NAME(ALPHA, "��ĸ")	            \
	MACRO_NAME(ALPHAMT, "Ħ����ĸ")	            \
	MACRO_NAME(CHN, "����")					\
	MACRO_NAME(CHNMT, "Ħ�к���")				\
	MACRO_NAME(CHNSF, "ʡ�ݺ���")			\
	MACRO_NAME(DIGIT_J, "���־�")			\
	MACRO_NAME(MILITARYCHN , "��������")	\
	MACRO_NAME(WJ, "�侯")					\
	MACRO_NAME(DIGITGUA, "���ֹ�")			\
	MACRO_NAME(GA, "�۰�")					\
	MACRO_NAME(COLOR, "��ɫ")				\
	MACRO_NAME(COLORNIGHT, "��ɫ����")					\
	MACRO_NAME(CHNALHPA, "������ĸ")		\
	MACRO_NAME(CHNALHPAMT, "˫�㺺����ĸ")		\
	MACRO_NAME(ALPHAH, "��ĸ��")            \
	MACRO_NAME(CARLOGO, "����")				   \

#define DEFINE_ROGTYPE_ENUM(a, b) RECOG_##a,

enum RECOGNITION_TYPE
{
	RECOGNITION_TYPE_LIST(DEFINE_ROGTYPE_ENUM)
	RECOG_MAXTYPE
};

//enum ALLCOLOR
//{
//	COLOR_HEI = 1,
//	COLOR_HUI,
//	COLOR_HONG,
//	COLOR_HUANG,
//	COLOR_BAI,
//	COLOR_LAN,
//	COLOR_LU,
//	COLOR_ZI,
//	COLOR_FEN,
//	COLOR_ZONG
//};

class IRecognition
{
public:
	virtual ~IRecognition() {};

	STDMETHOD(Evaluate)(
		HV_COMPONENT_IMAGE *pInput,				//���׺��ͼ��
		RECOGNITION_TYPE RecogType,		//ʶ������
		PLATE_TYPE nPlateType,			// ��������
		PLATE_COLOR nPlateColor,		//������ɫ
		PBYTE8 pbResult,				//���ؽ����ָ���ַ����UNICODE�ַ�����
		PSDWORD32 pdwProb,				//�����Ŀ��Ŷȣ���������QUANTIFY_DIGIT����
		int iCharPos=-1					//Optional. For outputing the normalized image
	)=0;

	STDMETHOD(EvaluateColor)(
		HV_COMPONENT_IMAGE *pInput,
		PBYTE8 pbResult,
		PSDWORD32 pdwProb,
		LIGHT_TYPE m_LightType
	)=0;
	//����ʶ�� change liangjf
	STDMETHOD(EvaluateCarLogo)(
		HV_COMPONENT_IMAGE *pInput,
		int *pbResult,                   
		PSDWORD32 pdwProb                 
		)=0;

	STDMETHOD(SetInspector)(
		IInspector *pInspector
	)=0;

	STDMETHOD(SetCharSet)(				//���ú����ַ���
		ChnCharSet nCharSet
	)=0;

	/*STDMETHOD(SetCharQuotietyMatrix)(				// �����ַ�Ȩֵ����
		ICharQuotietyMatrix *pCharQuotietyMatrix
	) = 0;*/

	STDMETHOD(SetHvParam)(HvCore::IHvParam2* pHvParam) = 0;

	STDMETHOD(SetHvModel)(HvCore::IHvModel* pModel) = 0;

	STDMETHOD(SetHvPerformance)(
		HvCore::IHvPerformance* pHvPerf
	) = 0;

	STDMETHOD(InitRecognition)(RECOGNITION_CFG_PARAM* pCfgParam) = 0;

    STDMETHOD(SetWorkMode)(
        int iProcMode = RECOG_MODE_DEFAULT
    ) = 0;

	STDMETHOD(EraseNail)(
		bool fIsNail,
		HV_RECT rgrcNailPos,
		int* piBottomLine
	) = 0;

	STDMETHOD(CheckValid)(
		HV_COMPONENT_IMAGE imgChar,
		BOOL fIsWhiteChar,
		int nId,
		BOOL *pValid
	) = 0;

	STDMETHOD(GetVersion)(
		char* szBuf, int szBufLen
	) = 0;
};

HRESULT CreateRecognition(IRecognition **ppRecognition);

#endif // _RECOGNITION_INCLUDED__

