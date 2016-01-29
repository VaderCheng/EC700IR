#include "swbasetype.h"
#include "hv_math.h"

#ifdef CHIP_6455
#pragma DATA_SECTION ("fastdata");
#endif
BYTE8 sqrtTable[MAX_SQRT_NUM];

// Exp���ұ�
#ifdef CHIP_6455
#pragma DATA_SECTION ("fastdata");
#endif
DWORD32 wLowBit[EXP_TABLE_LEN];
#ifdef CHIP_6455
#pragma DATA_SECTION ("fastdata");
#endif
DWORD32 wHighBit[EXP_TABLE_LEN];
#ifdef CHIP_6455
#pragma DATA_SECTION ("fastdata");
#endif
DWORD32 wLowBitN[EXP_TABLE_LEN];
#ifdef CHIP_6455
#pragma DATA_SECTION ("fastdata");
#endif
DWORD32 wHighBitN[EXP_TABLE_LEN];

// Exp���ұ�2
#ifdef CHIP_6455
#pragma DATA_SECTION ("fastdata");
#endif
WORD16 waExpValue[EXP_TABLE2_LEN];

//����exp,����20���ܼ��㣬�ɺ����Ᵽ֤
DWORD32 quanExpPositive(DWORD32 dwNum) {

	if (dwNum > 726817) {			//��������
		return 0xffffffff;
	}

	DWORD32 dwLowValue = wLowBit[dwNum & 0x3ff];
	DWORD32 dwHighValue = wHighBit[(dwNum >> 10) & 0x3ff];

	DWORD32 dwValue;

	if (dwHighValue >= 0x800000) {
		//��λΪ17λ����λ�������23λ����ȻDSP�����(DSPֻ��40λ)
		if (dwHighValue >= 0x8000000) {
			//���ƶ�9λ
			dwHighValue = dwHighValue >> 9;
			dwValue = (DWORD32)(((QWORD64)dwHighValue * dwLowValue) >> 7);
		} else {
			//���ƶ�4λ
			dwHighValue = dwHighValue >> 4;
			dwValue = (DWORD32)(((QWORD64)dwHighValue * dwLowValue) >> 12);
		}
	} else {
		dwValue = (DWORD32)(((QWORD64)dwHighValue * dwLowValue) >> QuanBit);
	}
	return dwValue;
}

//����exp,����20���ܼ��㣬�ɺ����Ᵽ֤
DWORD32 quanExpNegative(DWORD32 dwNum) {

	if (dwNum >= 1048576) {								//16 * 65536
		return 0;									//e-16 ������0����
	}

	DWORD32 dwLowValue = wLowBitN[dwNum & 0x3ff];
	DWORD32 dwHighValue = wHighBitN[(dwNum >> 10) & 0x3ff];
	DWORD32 dwValue;
	if (dwLowValue == 65536) {
		dwValue = dwHighValue;
	} else {
		dwValue = (dwHighValue * (WORD16)dwLowValue + 32768) >> QuanBit;
	}

	return dwValue;
}


//iShiftΪ����λ��,����Ϊż��
DWORD32 quanSqrt(DWORD32 dwNum, DWORD32 iShift) {
	DWORD32 dwSqrt;
	DWORD32 num = dwNum;
	int moveCount = 0;		//�ƶ�������,������ʾ���ƣ�������ʾ����(1��ʾ�ƶ�2λ)

	if (dwNum == 0)
		return 0;

	//��������ת��0~65536��ȡֵ��Χ���Բ������������λ����
	if (dwNum > 65536) {
		while(num > 65536) {
			num = num >> 2;
			moveCount--;			//
		} //���ڻ�С��65536����
	} else if (dwNum < 65536) {
		while(num < 65536) {
			num = num << 2;
			moveCount++;
		} //���ڻ����65536����
		if (num > 65536) {
			num = num >> 2;
			moveCount--;
		}
	}//���Ϊnum=65536��Χ�ڵ�����,moveCountΪ��λ����

	//���õ�sqrt����ֵ
	if (num == 65536)
		dwSqrt = 256;
	else {
		dwSqrt = (SDWORD32)sqrtTable[num - 1];
	}

	moveCount = (iShift >> 1) - moveCount;			//��������
	if (moveCount >= 0) {
		dwSqrt = dwSqrt << moveCount;
	} else {
		dwSqrt = dwSqrt >> (-moveCount);
	}
	return dwSqrt;
}


