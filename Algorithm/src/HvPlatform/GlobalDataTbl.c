#include "globaldatatbl.h"
#include "math.h"
#include "string.h"

#define MAX_INT(x, y)	(x) > (y) ? (x) : (y)
#define robust_f2(a, x, c)				\
	{									\
		(a) = (x) * (x) * (c) + 0.1f;	\
		if ((a) > 1.0f)					\
		{								\
			(a) = 1.0f;					\
		}								\
	}

#if RUN_PLATFORM != PLATFORM_LINUX

#ifndef CHIP_8127
int round(double val) {
	int iResult;

	iResult = ( int )val;
	val -= iResult;
	if ( val >= 0.5 ) return iResult + 1;
	if ( val <= -0.5 ) return iResult - 1;
	return iResult;
}
#else
	double round(double v);
#endif

#else
    double round(double v);
#endif

void InitSqrtTable()
{
	int i;

	//ʵʼ��ƽ������.��ƽ��������г�ʼ��,Ϊ������򷽲��󷽸���ʹ�õ�.
	for ( i = 0; i < MAX_SQRT_NUM; i ++ )
	{
		int iValue = (int)(sqrt((float)i) + 0.5);
		sqrtTable[i] = (iValue > 255) ? 255 : iValue;
	}
}

void InitExpTable()
{
	int i;

	// ��ʼ��Exp��LUT
	for (i = 0; i < EXP_TABLE_LEN; i++) {
		double dbValue = exp((double)i / MATH_QUAN_SCALE);
		wLowBit[i] = (int)(dbValue * MATH_QUAN_SCALE + 0.5);
	}

	// ��ʼ��Exp��LUT
	for (i = 0; i < EXP_TABLE_LEN; i++) {
		double dbValue = exp((double)i / MATH_QUAN_SCALE);
		wLowBit[i] = (int)(dbValue * MATH_QUAN_SCALE + 0.5);
	}
	for (i = 0;i < EXP_TABLE_LEN;i++) {
		double dbValue = exp((((double)i * EXP_TABLE_LEN) / MATH_QUAN_SCALE));
		QWORD64 qwValue = (QWORD64)(dbValue * MATH_QUAN_SCALE + 0.5);
		wHighBit[i] = (qwValue > 0xffffffff) ? 0xffffffff: (DWORD32)qwValue;
	}
	for (i = 0; i < EXP_TABLE_LEN; i++) {
		double dbValue = exp((double)-i / MATH_QUAN_SCALE);
		wLowBitN[i] = (int)(dbValue * MATH_QUAN_SCALE + 0.5);
	}
	for (i = 0;i < EXP_TABLE_LEN;i++) {
		double dbValue = exp((((double)-i * EXP_TABLE_LEN) / MATH_QUAN_SCALE));
		wHighBitN[i] = (int)(dbValue * MATH_QUAN_SCALE + 0.5);
	}

	// ��ʼ��������exp��
	for (i = -(EXP_TABLE2_LEN - 1); i <= 0; i++) {
		waExpValue[EXP_TABLE2_LEN + i - 1] = round( 32768 * exp( (double)i / 1024.0f ));
	}
}

// ������ֳ��ƿո���
void InitSpaceWidth()
{
	float fSpace1, fSpace2;
	int iSum, iChar;
	int iWidth;

	// baWJSpaceWidth ����(�侯���ϸ����ո�Ŀ��)������
	for (iWidth = MIN_CHAR_WIDTH; iWidth <= MAX_CHAR_WIDTH; iWidth ++ )
	{
		fSpace2 = (float)WJ_SPACE2_WIDTH * iWidth;
		iSum = round( fSpace2 );
		baWJSpaceWidth[iWidth - MIN_CHAR_WIDTH][0] = iSum;

		fSpace1 = (float)WJ_SPACE1_WIDTH * iWidth;
		for (iChar = 1; iChar < PLATE_CHAR_NUM - 1; iChar ++ ) {
			fSpace1 += fSpace2;
			baWJSpaceWidth[iWidth - MIN_CHAR_WIDTH][iChar] = MAX_INT( 1, round( fSpace1 ) - iSum );
			iSum += MAX_INT( 1, round( fSpace1 ) - iSum );
		}
		fSpace1 += fSpace2 / 2.0f;
		baWJSpaceWidth[iWidth - MIN_CHAR_WIDTH][iChar] = MAX_INT( 1, round( fSpace1 ) - iSum );

		iSum += MAX_INT( 1, round( fSpace1 ) - iSum );
		baWJSpaceWidth[iWidth - MIN_CHAR_WIDTH][iChar + 1] = iSum;
	}

	// baNormalSpaceWidth ����(��ͨ���ϸ����ո�Ŀ��)������
	for ( iWidth = MIN_CHAR_WIDTH; iWidth <= MAX_CHAR_WIDTH; iWidth ++ )
	{
		fSpace2 = NORMAL_SPACE2_WIDTH * iWidth;
		iSum = round( fSpace2 );
		baNormalSpaceWidth[iWidth - MIN_CHAR_WIDTH][0] = iSum;

		fSpace1 = NORMAL_SPACE1_WIDTH * iWidth;
		for ( iChar = 1; iChar < PLATE_CHAR_NUM - 1; iChar ++ ) {
			fSpace1 += fSpace2;
			baNormalSpaceWidth[iWidth - MIN_CHAR_WIDTH][iChar] = MAX_INT( 1, round( fSpace1 ) - iSum );
			iSum += MAX_INT( 1, round( fSpace1 ) - iSum );
		}
		fSpace1 += fSpace2 / 2.0f;
		baNormalSpaceWidth[iWidth - MIN_CHAR_WIDTH][iChar] = MAX_INT( 1, round( fSpace1 ) - iSum );
		iSum += MAX_INT( 1, round( fSpace1 ) - iSum );
		baNormalSpaceWidth[iWidth - MIN_CHAR_WIDTH][iChar + 1] = iSum;
	}

	// baPoliceSpaceWidth ����(���澯���ϸ����ո�Ŀ��)������
	for ( iWidth = MIN_CHAR_WIDTH; iWidth <= MAX_CHAR_WIDTH; iWidth ++ )
	{
		fSpace1 = NORMAL_SPACE1_WIDTH * iWidth;
		iSum = round( fSpace1 );
		baPoliceSpaceWidth[iWidth - MIN_CHAR_WIDTH][0] = iSum;

		fSpace2 = NORMAL_SPACE2_WIDTH * iWidth;
		for ( iChar = 1; iChar < PLATE_CHAR_NUM - 1; iChar ++ ) {
			fSpace1 += fSpace2;
			baPoliceSpaceWidth[iWidth - MIN_CHAR_WIDTH][iChar] = MAX_INT( 1, round( fSpace1 ) - iSum );
			iSum += MAX_INT( 1, round( fSpace1 ) - iSum );
		}
		fSpace1 += fSpace2 / 2.0f;
		baPoliceSpaceWidth[iWidth - MIN_CHAR_WIDTH][iChar] = MAX_INT( 1, round( fSpace1 ) - iSum );
		iSum += MAX_INT( 1, round( fSpace1 ) - iSum );
		baPoliceSpaceWidth[iWidth - MIN_CHAR_WIDTH][iChar + 1] = iSum;
	}
	for ( iWidth = MIN_CHAR_WIDTH; iWidth <= MAX_CHAR_WIDTH; iWidth ++ )
	{
		iSum = 0;
		for( iChar=0;iChar<=2;iChar++)
		{
			baShiGuanSpaceWidth[iWidth - MIN_CHAR_WIDTH][iChar] =  MAX_INT(1,baNormalSpaceWidth[iWidth - MIN_CHAR_WIDTH][iChar+2] - 1);
			iSum += baShiGuanSpaceWidth[iWidth - MIN_CHAR_WIDTH][iChar];
		}
		baShiGuanSpaceWidth[iWidth - MIN_CHAR_WIDTH][3] =  MAX_INT(1,baNormalSpaceWidth[iWidth - MIN_CHAR_WIDTH][1] - 1);
		iSum += baShiGuanSpaceWidth[iWidth - MIN_CHAR_WIDTH][3];
		baShiGuanSpaceWidth[iWidth - MIN_CHAR_WIDTH][4] =  MAX_INT(1,baNormalSpaceWidth[iWidth - MIN_CHAR_WIDTH][0] - 1);
		iSum += baShiGuanSpaceWidth[iWidth - MIN_CHAR_WIDTH][4];
		for(iChar=5;iChar<7;iChar++)
		{
			baShiGuanSpaceWidth[iWidth - MIN_CHAR_WIDTH][iChar] =  MAX_INT(1,baNormalSpaceWidth[iWidth - MIN_CHAR_WIDTH][iChar] - 1);
			iSum += baShiGuanSpaceWidth[iWidth - MIN_CHAR_WIDTH][iChar];
		}
		baShiGuanSpaceWidth[iWidth - MIN_CHAR_WIDTH][7] = iSum;
	}
}

// (��бʱ�����ϸ������ڴ�ֱ�����ϵ�λ��)������
void InitSlantPixel()
{
	int iWidth, iHigh, iChar;
	float fSlantRatio[ MAX_SLANT_NUM ] = { 0.15f / 25.0f, -0.15f / 25.0f };

	for ( iHigh = MIN_SEG_IMG_HIGH; iHigh <= MAX_SEG_IMG_HIGH; iHigh ++ )
	{
		for ( iWidth = 0; iWidth < MAX_SLANT_NUM; iWidth ++ ) {
			for ( iChar = 1; iChar < PLATE_CHAR_NUM - 1; iChar ++ )
			{
				sbaSlantPixel[iHigh - MIN_SEG_IMG_HIGH][iWidth][iChar - 1] = round(iChar * iHigh * fSlantRatio[iWidth]);
			}
			sbaSlantPixel[iHigh - MIN_SEG_IMG_HIGH][iWidth][iChar - 1] =
								round((PLATE_CHAR_NUM - 1) * iHigh * fSlantRatio[iWidth]);
		}
	}
}

void InitScoreLUT()
{
	float fltTemp;
	float a;
	int iValue, i;
	for(i = 0; i < 256; i++)
	{
		fltTemp = ((float)i)/255;

		// ("��"��Եͼ���ֱ�)������
		robust_f2(a, fltTemp, 1);
		iValue = round(1000000 * (1 + log10(a)));
		iaCharEdgeScoreLUT[i] = iValue;

		// ("��"�ڰ�ͼ���ֱ�)������
		robust_f2(a, fltTemp, 1);
		iValue = round(1000000 * (1 + log10(a)));
		iaCharBinScoreLUT[i] = iValue;

		// ("�ո�"Hueͼ���ֱ�)������
		robust_f2(a, fltTemp, 1);
		iValue = round(1000000 * (1 + log10(a)));
		iaSpaceHueScoreLUT[i] = iValue;

		fltTemp = 1 - fltTemp;

		// (Bar�ڰ�ͼ���ֱ�)������
		robust_f2(a, fltTemp * fltTemp, 1);
		iValue = round(1000000 * (1 + log10(a)));
		iaBarBinScoreLUT[i] = iValue;

		// ("�ո�"�ڰ�ͼ���ֱ�)������
		robust_f2(a, fltTemp * fltTemp, 1);
		iValue = round(1000000 * (1 + log10(a)));
		iaSpaceBinScoreLUT[i] = iValue;

		// (Bar��Եͼ���ֱ�)������
		robust_f2(a, fltTemp * fltTemp, 1);
		iValue = round(1000000 * (1 + log10(a)));
		iaBarEdgeScoreLUT[i] = iValue;
	}
}

// ��ʼ��Hue��ɫ��
void InitHueColorTable()
{
	int i;
	for (i = 0; i < 20; i++)
	{
		g_hueColorTable[i] = 4;					// ��ɫ
	}
	for (i = 20; i < 60; i++)
	{
		g_hueColorTable[i] = 1;					// ��ɫ
	}
	for (i = 60; i < 100; i++)
	{
		g_hueColorTable[i] = 5;					// ��ɫ
	}
	for (i = 100; i < 190; i++)
	{
		g_hueColorTable[i] = 0;					// ��ɫ
	}
	for (i = 190; i < 241; i++)
	{
		g_hueColorTable[i] = 4;					// ��ɫ
	}
}

void GlobleDataSrcInit( void ) {
	static int flag = 0;

	if ( flag ) return;

	// ��ʼ��ƽ������
	InitSqrtTable();
	// ��ʼ������Exp��
	InitExpTable();
	// ������ֳ��ƿո���
	InitSpaceWidth();
	// (��бʱ�����ϸ������ڴ�ֱ�����ϵ�λ��)������
	InitSlantPixel();
	// ����������ֱ�
	InitScoreLUT();
	// ��ʼ��Hue��ɫ��
	InitHueColorTable();

	flag = 1;
}

