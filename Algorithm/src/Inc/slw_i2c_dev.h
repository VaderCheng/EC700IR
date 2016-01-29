

#ifndef __SLW_I2C_DEV__
#define __SLW_I2C_DEV__

#ifdef __cplusplus
extern "C" {
#endif

#include "slw_gpio.h"
#include "slw_gpio2iic.h"

#define EEPROM_AT24C1024_MAX_SIZE     0x20000
#define EEPROM_AT24C1024_SECTION_SIZE 0x10000
#define EEPROM_AT24C1024_PAGE_SIZE    256

/*------------------RTC--------------------*/
// RTC������ʼ��ַ��������.
#define RTC_TIME_START_ADDR 0x00 //RTCоƬ��ʱ��Ĵ�������ʼ��ַ.
#define RTC_INTOUT_ADDR     0x0E //RTCоƬ��INT����Ĵ�������ʼ��ַ.
#define RTC_TIME_LEN        7    //RTCоƬ����������ʱ����ռ�üĴ����ĸ���.
#define RTC_INTOUT_LEN      1    //RTCоƬ�����������ж�����ļĴ�������.

#define Bcd2Hex(bSrcData) (((bSrcData) >> 4) * 10 + ((bSrcData) & 0xF))
#define Hex2Bcd(bSrcData)	((((bSrcData) / 10) << 4) + ((bSrcData) % 10))

/* ö��˵��:��ö��ΪRTCʱ���ж������ʱ������,
            ��Ϊ1.0�汾���Ź��Ĺ���ʱ��Ϊ��ʱ��,
            ���������ʱ��Ƶ��Ϊ32K*/
typedef enum {
	RTC_INT_1HZ   = 0,
	RTC_INT_4KHZ  = 1,
	RTC_INT_8KHZ  = 2,
	RTC_INT_32KHZ = 3
} RTC_INT_FREQUENCY;

/* ���ݽṹ˵��:��������ʵʱʱ��. */
//typedef struct {
//	Uint16 wYear;    //����.
//	Uint16 wMonth;   //����.
//	Uint16 wDay;     //����.
//	Uint16 wWeekNum; //��ǰ���ڵ�������.
//	Uint16 wHour;    //С����,24Сʱ��.
//	Uint16 wMinute;  //Сʱ��.
//	Uint16 wSecond;  //����.
//	Uint16 wMSecond; //������.
//} REAL_TIME_STRUCT;


/*------------------LM75 -------------------*/
#define LM75_Temp_START_ADDR 0x00

typedef struct{
	Uint8   TempPolar; //�¶ȵ��������� 0: +  ,1: - ;
	Uint16  Temp;      //�¶Ⱦ���ֵ
}TEMP_STRUCT;


/*
����˵��:�ú���������EEPROM�ж�ȡһ�����ݵ�����.
��������ֵ:
	����S_OK��ʾ��ȡEEPROM�����ɹ�.
	����E_POINTER��ʾpbMemDstΪINVALID_POINTER.
	����E_INVALIDARG��ʾ����Ĳ����д�.
	����E_FAIL��ʾӲ���ײ��������.
	����S_FALSE��ʾ��ȡEEPROM����. */
HRESULT EEPROM_ReadSection(
	DWORD32  dwReadPos,	    // ��ȡEEPROM����λ��.
	PBYTE8   pbMemDst, 	    // ָ�����������.
	DWORD32  dwBytesToRead,	// ������ȡ�������ֽڳ���.
	PDWORD32 pdwBytesRead,  // ʵ�ʶ�ȡ�������ֽڳ�ΪNULL����
	BYTE8    bDeviceAddr);  // �豸��ַ

/*
����˵��:�ú���������EEPROM��д��һ��������.
��������ֵ:
	����S_OK��ʾд��EEPROM�����ɹ�.
	����E_POINTER��ʾpbMemSrcΪINVALID_POINTER.
	����E_INVALIDARG��ʾ����Ĳ����д�.
	����E_FAIL��ʾӲ���ײ��������.
	����S_FALSE��ʾд��EEPROM����. */
HRESULT EEPROM_WriteSection(
	const PBYTE8 pbMemSrc, 		  //ָ��д�������.
	DWORD32      dwWritePos, 	  //д��EEPROM���׵�ַ.
	DWORD32      dwBytesToWrite,//����д��������ֽڳ���.
	PDWORD32     pdwBytesWrite,	//ʵ��д��������ֽڳ���,ΪNULL����
	BYTE8        bDeviceAddr);   // �豸��ַ

/*
����˵��:�ú���������EEPROM�ж�ȡһ�����ݳ���
��������ֵ:
	����S_OK��ʾ��ȡEEPROM�����ɹ�.
	����E_POINTER��ʾpbMemDstΪINVALID_POINTER.
	����E_INVALIDARG��ʾ����Ĳ����д�.
	����E_FAIL��ʾӲ���ײ��������.
	����S_FALSE��ʾ��ȡEEPROM����. */
//HRESULT EEPROM_Read(
//	DWORD32  dwReadPos,		//��ȡEEPROM����λ��.
//	PBYTE8   pbMemDst, 		//ָ�����������.
//	DWORD32  dwBytesToRead,	//������ȡ�������ֽڳ���.
//	PDWORD32 pdwBytesRead);	//ʵ�ʶ�ȡ�������ֽڳ���,ΪNULL����.

/*
����˵��:�ú���������EEPROM�ж�ȡһ�����ݵ�����.
��������ֵ:
	����S_OK��ʾ��ȡEEPROM�����ɹ�.
	����E_POINTER��ʾpbMemDstΪINVALID_POINTER.
	����E_INVALIDARG��ʾ����Ĳ����д�.
	����E_FAIL��ʾӲ���ײ��������.
	����S_FALSE��ʾ��ȡEEPROM����. */
//HRESULT EEPROM_Write(
//	const PBYTE8 pbMemSrc, 		//ָ��д�������.
//	DWORD32      dwWritePos, 	//д��EEPROM���׵�ַ.
//	DWORD32      dwBytesToWrite,	//����д��������ֽڳ���.
//	PDWORD32     pdwBytesWrite);	//ʵ��д��������ֽڳ���,ΪNULL����.

/*
����˵��:�ú���������ȡʵʱʱ��оƬ�е�ʵʱʱ��ֵ(������),��ȷ����,���ǵ�ǰ��ʵʱʱ��оƬ�����Ϊ100��������.
������:
	����S_OK��ʾ��ȡRTC����ʱ��ɹ�.
	����E_POINTER��ʾpRealTimeΪINVALID_POINTER.
	����E_FAIL��ʾӲ��ײ�������?
	����S_FALSE��ʾ��ȡRTC����ʱ�����. */
//HRESULT RTC_GetDateTime(
//	REAL_TIME_STRUCT *pRealTime);		//ָ�������ʱ��.

/*
����˵��:�ú�����������ʵʱʱ��оƬʱ��(��������),��ȷ����.�����ݴ��ĸ��ֽ������������.�����û������õ�����
	д����RTC���ʱ�����ڼĴ�����.
��������ֵ:
	����S_OK��ʾ����RTC����ʱ��ɹ�.
	����E_POINTER��ʾpbTimeValΪINVALID_POINTER.
	����E_INVALIDARG��ʾ�����ʵʱʱ�����ݷǷ�.
	����E_FAIL��ʾӲ���ײ��������.
	����S_FALSE��ʾ����RTC����ʱ�����. */
//HRESULT RTC_SetDateTime(
//	const REAL_TIME_STRUCT *pRealTime); //ָ�����õ�ʱ��.

/*
����˵��:�ú���������ʼ��ʱ�ӵ��ж����ʱ��Ƶ��.
��������ֵ:
	����S_OK��ʾ�����ɹ�.
	����E_FAIL��ʾ��������ϵͳ���쳣.
	����S_FALSE��ʾ����ʧ��. */
HRESULT InitRTCInt(BYTE8 IntFrequency);

/*
����˵��:�ú����������¶ȴ�����LM75��ȡʵʱ�¶�
	�¶�����9λ�������LM75��16λTEMP�Ĵ����У�
��������ֵ:
	����S_OK��ʾ��ȡ�¶�ֵ�ɹ�.
	����E_POINTER��ʾpTempΪINVALID_POINTER.
	����E_FAIL��ʾӲ���ײ��������.
	����S_FALSE��ʾ��ȡ���¶�ֵ����. */
HRESULT LM75_GetTempData(TEMP_STRUCT *pTemp);

//////////////////// DS2460 ����оƬ ////////////////////
// address
#define DS2460_MEM_SHA      0x00
#define DS2460_MEM_MAC      0x40
#define DS2460_MEM_SSECRET  0x54
#define DS2460_MEM_CMD      0x5C
#define DS2460_MEM_MATCHMAC 0x5E
#define DS2460_MEM_ESECRET1 0x60
#define DS2460_MEM_ESECRET2 0x68
#define DS2460_MEM_ESECRET3 0x70
#define DS2460_MEM_EEPROM   0x80
#define DS2460_MEM_SERIAL   0xF0

// command
#define DS2460_CMD_COMPUTE  0x80
#define DS2460_CMD_TXSECRET 0x40

// ��ʼ��
HRESULT DS2460_init();
HRESULT DS2460_ReadSerialNumber(BYTE8* buffer, int len);
HRESULT DS2460_ComputeSHA(BYTE8 gpSHA, BYTE8 srcSecret, BYTE8 toMACBuffer);
HRESULT DS2460_MatchMAC(BYTE8* buffer, int len);
HRESULT DS2460_ReadMAC(BYTE8* buffer, int len);
HRESULT DS2460_WriteESecret1(BYTE8* buffer, int len);
HRESULT DS2460_WriteESecret2(BYTE8* buffer, int len);
HRESULT DS2460_WriteESecret3(BYTE8* buffer, int len);

HRESULT DS2460_WriteInputSHA(BYTE8* buffer, int len);
HRESULT DS2460_ReadInputSHA(BYTE8* buffer, int len);
HRESULT DS2460_WriteMAC(BYTE8* buffer, int len);
HRESULT DS2460_TransferSecret(BYTE8 dstSecret);
HRESULT DS2460_WriteSSecret(BYTE8* buffer, int len);

void ComputeSHAEE(unsigned char *MT,long *A,long *B, long *C, long *D,long *E);

#ifdef __cplusplus
}
#endif

#endif // __SLW_I2C_DEV__
