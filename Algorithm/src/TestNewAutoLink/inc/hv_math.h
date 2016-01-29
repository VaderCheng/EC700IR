#ifndef __FAST_MATH_H__
#define __FAST_MATH_H__

#include "GlobalDataTbl.h"

#define MIN_INT( int1, int2 )	( ( int1 ) < ( int2 ) ? ( int1 ) : ( int2 ) )
#define MAX_INT( int1, int2 )	( ( int1 ) > ( int2 ) ? ( int1 ) : ( int2 ) )
#define HV_ABS( val )	( ( val ) > 0 ? ( val ) : -( val ) )

#define RANGE_INT( iVal, iMin, iMax )	\
	( ( ( iVal ) > ( iMin ) ) ? ( ( ( iVal ) <= ( iMax ) ) ? ( iVal ) : ( iMax ) ) : ( iMin ) )

#define ROUND_SHR_POSITIVE( Dividend, iShiftRightCount )	\
	( ( ( Dividend ) & ( 1 << ( ( iShiftRightCount ) - 1 ) ) ) ? ( ( Dividend ) >> ( iShiftRightCount ) ) + 1 : ( ( Dividend ) >> ( iShiftRightCount ) ) )

#define ROUND_SHR_NEGATIVE( Dividend, iShiftRightCount )	\
		( -( ( ( -( Dividend ) ) & ( 1 << ( ( iShiftRightCount ) - 1 ) ) ) ? ( ( -( Dividend ) ) >> ( iShiftRightCount ) ) + 1 : ( ( -( Dividend ) ) >> ( iShiftRightCount ) ) ) )

// iShiftRightCount >= 1
#define ROUND_SHR( Dividend, iShiftRightCount )	\
	( ( ( Dividend ) >= 0 ) ? ROUND_SHR_POSITIVE( Dividend, iShiftRightCount ) : ROUND_SHR_NEGATIVE( Dividend, iShiftRightCount ) )

inline int ROUND_DIV_POSITIVE( int iDividend, int iDivisor ) {
	int iRoundDivTemp;

	iRoundDivTemp = ( iDividend << 1 ) / ( iDivisor );
	return ( iRoundDivTemp & 1 ) ? ( ( iRoundDivTemp >> 1 ) + 1 ) : ( iRoundDivTemp >> 1 );
}

inline int ROUND_DIV_NEGATIVE( int iDividend, int iDivisor ) {
	int iRoundDivTemp;

	iRoundDivTemp = ( ( -iDividend ) << 1 ) / ( iDivisor );
	return -( ( iRoundDivTemp & 1 ) ? ( ( iRoundDivTemp >> 1 ) + 1 ) : ( iRoundDivTemp >> 1 ) );
}

inline int ROUND_DIV( int iDividend, int iDivisor ) {
	int iRoundDivTemp;

	iRoundDivTemp = ( iDividend << 1 ) / ( iDivisor );
	if ( iRoundDivTemp >= 0 ) return ( iRoundDivTemp & 1 ) ? ( ( iRoundDivTemp >> 1 ) + 1 ) : ( iRoundDivTemp >> 1 );
	iRoundDivTemp = -iRoundDivTemp;
	return -( ( iRoundDivTemp & 1 ) ? ( ( iRoundDivTemp >> 1 ) + 1 ) : ( iRoundDivTemp >> 1 ) );
}

// EXP_FROM_TABLE( iX ) = round( 1024.0f * exp( iX / 1024.0f ) )
// �����Ŀ������exp(x), ���� -7 < x <= 0.
// ͨ�������Ҫ�����±��ʽ: round( K * exp( -Dividend / Divisor ) ), 
// ���� -7 < -Dividend / Divisor <= 0, K < 65536
//		ROUND_SHR( K * EXP_FROM_TABLE( ( Dividend << EXP_FLOAT_SHR ) / Divisor ), EXP_RESULT_SHR )
#define EXP_FLOAT_SHR				10
#define EXP_RESULT_SHR				15
#define EXP_FROM_TABLE( iX )		( ( iX >= 7168 ) ? 0 : waExpValue[ 7167 - iX ] )

#define SQRT_FROM_TABLE( dwX )		( sqrtTable[ dwX ] )

#ifndef __ASM_FILE__
	//extern WORD16 waExpValue[];
#endif		// #ifndef __ASM_FILE__


//��ѧ�׶���������㷨ʹ�õĺ�ͺ���
const DWORD32 QuanScale =  65536;			//��������
const BYTE8 QuanBit =  16;					//����λ������ 2��16�η�

#define QUAN_NUM(a)		((a) << QuanBit)		//����һ���������������������Ҳ����������걣������
#define DEQUAN_NUM(a)	((a) >> QuanBit)		//ȡ������,������������˺����ʹ������걣������

//�����������ȡ������
//#define DEQUAN_NUM(a)	(((a) + QuanScale >> 1) >> QuanBit)		//ȡ������,������������˺����ʹ������걣������

//����exp
DWORD32 quanExpPositive(DWORD32 dwNum);		//����exp(����)
DWORD32 quanExpNegative(DWORD32 dwNum);		//����exp(����)

//SDWORD32 quanSqrt(SQWORD64 n);
DWORD32 quanSqrt(DWORD32 dwNum, DWORD32 iShift);

#endif		// #ifndef __FAST_MATH_H__

