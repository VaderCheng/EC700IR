#ifndef _SWPLATETYPE_H_
#define _SWPLATETYPE_H_

// ���Ͷ���
typedef enum _CAR_TYPE
{
	CT_UNKNOWN = 0,			// δ֪����
	CT_SMALL,				// С�ͳ�
	CT_MID,					// ���ͳ�
	CT_LARGE,				// ���ͳ�
	CT_WALKMAN,				// ����
	CT_BIKE,				// �ǻ�����
	CT_VEHICLE,				// ������
	CT_COUNT
} CAR_TYPE;

//���峵����ɫ
#define PLATE_COLOR_LIST(MACRO_NAME)						\
	MACRO_NAME(UNKNOWN, "δ֪")								\
	MACRO_NAME(BLUE, "��")									\
	MACRO_NAME(YELLOW, "��")								\
	MACRO_NAME(BLACK, "��")									\
	MACRO_NAME(WHITE, "��")									\
	MACRO_NAME(LIGHTBLUE, "ǳ��")							\
	MACRO_NAME(GREEN, "��")

#define DEFINE_PLATE_COLOR_ENUM(a, b)		PC_##a,

typedef enum {
	PLATE_COLOR_LIST(DEFINE_PLATE_COLOR_ENUM)
	PC_COUNT
} PLATE_COLOR;

//���峵������
#define PLATE_TYPE_LIST(MACRO_NAME)		\
	MACRO_NAME(		\
		UNKNOWN,		\
		"δ֪����",		\
		RECOG_CHN,		\
		RECOG_ALPHA,		\
		RECOG_ALL,		\
		RECOG_ALL,		\
		RECOG_DIGIT,		\
		RECOG_DIGIT,		\
		RECOG_DIGIT		\
	)		\
	MACRO_NAME(		\
		NORMAL,		\
		"�������ƣ�����",		\
		RECOG_CHNSF,		\
		RECOG_ALPHA,		\
		RECOG_ALL,		\
		RECOG_ALL,		\
		RECOG_ALL,		\
		RECOG_ALL,		\
		RECOG_ALL		\
	)		\
	MACRO_NAME(		\
		WJ,		\
		"�侯����",		\
		RECOG_DIGIT,		\
		RECOG_DIGIT,		\
		RECOG_WJ,		\
		RECOG_DIGIT,		\
		RECOG_DIGIT,		\
		RECOG_DIGIT,		\
		RECOG_DIGIT		\
	)		\
	MACRO_NAME(		\
		POLICE,		\
		"�������е��л��ƣ�������Ƹ�ʽ��ͬ�ľ���",		\
		RECOG_CHN,		\
		RECOG_ALPHA,		\
		RECOG_ALL,		\
		RECOG_ALL,		\
		RECOG_DIGIT,		\
		RECOG_DIGIT,		\
		RECOG_DIGIT_J		\
	)		\
	MACRO_NAME(		\
		POLICE2,		\
		"��-A1234-��",		\
		RECOG_CHNSF,		\
		RECOG_ALPHA,		\
		RECOG_ALL,		\
		RECOG_ALL,		\
		RECOG_DIGIT,		\
		RECOG_DIGIT,		\
		RECOG_DIGIT_J		\
	)		\
	MACRO_NAME(		\
		MILITARY,		\
		"����",		\
		RECOG_MILITARYCHN,		\
		RECOG_ALPHA,		\
		RECOG_DIGIT,		\
		RECOG_DIGIT,		\
		RECOG_DIGIT,		\
		RECOG_DIGIT,		\
		RECOG_DIGIT		\
	)		\
	MACRO_NAME(		\
		DOUBLE_YELLOW,		\
		"˫�л��ƣ�˫�а���",		\
		RECOG_CHNMT,		\
		RECOG_ALPHAMT,		\
		RECOG_ALLMT,		\
		RECOG_ALLMT,		\
		RECOG_DIGIT,		\
		RECOG_DIGIT,		\
		RECOG_DIGITGUA		\
	)		\
	MACRO_NAME(		\
		DOUBLE_MOTO,		\
		"˫��Ħ����",		\
		RECOG_CHNMT,		\
		RECOG_ALPHAMT,		\
		RECOG_ALLMT,		\
		RECOG_ALLMT,		\
		RECOG_DIGIT,		\
		RECOG_DIGIT,		\
		RECOG_DIGIT		\
	)		\
	MACRO_NAME(		\
		INDIVIDUAL,		\
		"���Ի�����",		\
		RECOG_ALL,		\
		RECOG_ALL,		\
		RECOG_ALL,		\
		RECOG_ALL,		\
		RECOG_ALL,		\
		RECOG_ALL,		\
		RECOG_ALL		\
	)					\
	MACRO_NAME(			\
		DOUBLE_GREEN,	\
		"˫������",		\
		RECOG_CHNSF,	\
		RECOG_DIGIT,	\
		RECOG_DIGIT,	\
		RECOG_ALL,		\
		RECOG_DIGIT,	\
		RECOG_DIGIT,	\
		RECOG_DIGIT		\
	)					\
	MACRO_NAME(			\
	   DOUBLE_WJ,		\
	   "�侯����",		\
	   RECOG_DIGIT,		\
	   RECOG_DIGIT,		\
	   RECOG_WJ,		\
	   RECOG_DIGIT,		\
	   RECOG_DIGIT,		\
	   RECOG_DIGIT,		\
	   RECOG_DIGIT		\
   )

#define DEFINE_PLATE_TYPE_ENUM(a, b, r1, r2, r3, r4, r5, r6, r7)		PLATE_##a,

typedef enum {
	PLATE_TYPE_LIST(DEFINE_PLATE_TYPE_ENUM)
	PLATE_TYPE_COUNT
} PLATE_TYPE;

typedef enum
{
	CC_UNKNOWN = 0,			//δ֪
	CC_WHITE,				//��
	CC_GREY,				//�� 
	CC_BLACK,				//�� 
	CC_RED,					//��
	CC_YELLOW,				//��
	CC_GREEN,				//��
	CC_BLUE,				//��
	CC_PURPLE,				//��
	CC_PINK,				//��
	CC_BROWN,				//��
	CC_COUNT
} CAR_COLOR;

//���������ַ���
#define g_cnPlateChars 7

#endif
