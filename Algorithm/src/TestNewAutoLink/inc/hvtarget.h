#ifndef _HVTARGET_H
#define _HVTARGET_H

#ifdef __cplusplus
	extern "C" {
#endif	/* #ifdef __cplusplus */


#include "dspsys_def.h"

typedef struct _SYS_WORK_TIMER {
	int iGetType;
	DWORD32 dwMSCountLow;
	DWORD32 dwMSCountHigh;
} SYS_WORK_TIMER;

extern SYS_WORK_TIMER SysWorkTimer;

/* ö������:ϵͳ֧�ִ��ںź궨��. */
typedef enum {
	SYS_DEBUG_COM 		= 0,
	SYS_CTRL_COM		= 1,
	MAX_SYS_COM_NUM		= 2
} SYS_COM_PORT;

/* ö������:�����ն��û�����ؼ��ַ���־,����Ϊ�������ͳ�������. */
typedef enum {
	USER_KEY_ESC		= 1,
	USER_KEY_BACKSPACE 	= 2,
	USER_KEY_ENTER		= 4
} USER_KEY_VALUE;

/* ö������:DSP��������ʽ. */
typedef enum {
	DSP_BOOT_EMULATION	= 0,
	DSP_BOOT_HPI		= 1,
	DSP_BOOT_RESERVED	= 2,
	DSP_BOOT_EMIFA		= 3
} DSP_BOOT_MODE;

/* ö������:DSP���ڴ�ӳ�䷽ʽ. */
typedef enum {
	DSP_MEMMAP_EXTERNAL = 0,			/* �ⲿ�洢����ַ��0��ʼ. */
	DSP_MEMMAP_INTERNAL	= 1				/* �ڲ��洢����ַ��0��ʼ. */
} DSP_MEMMAP_MODE;

/* ���ݽṹ˵��:�����ݽṹ��������ָʾDSP���й�Ƭ���豸��Ϣ�Լ�DSP�汾�ͺ���Ϣ. */
typedef struct _DSP_CHIP_INFO {
	BYTE8				bCpuTypeId;		/* DSP��Ʒ����ID��.											*/					
	BYTE8 				bCpuVerId;		/* DSP�汾ID��.                                             */
	BYTE8				bEndianMode;	/* DSP����ʹ�õ��ֽ���,Ϊ1��ʾΪ���ֽ���,Ϊ0��ʾΪ���ֽ���. */
	BYTE8				bVpConfig;		/* DSP��Ƭ������VP�˿�ʹ�����,bit0--VP0,bit1-VP1,bit2--VP2 */
	DSP_BOOT_MODE		DspBootMode;	/* DSP��ǰ��ʹ�õ�����ģʽ.									*/					
	DSP_MEMMAP_MODE		DspMemMapMode;	/* DSP���ڴ�ӳ��ģʽ(��û��ʹ��L2 Cache�������),			*/
										/* Ϊ0��ʾΪ�ⲿ��ַ��0��ʼ,Ϊ1��ʾ�ڲ���ַ��0��ʼ.			*/	
	BOOL				fEmacEnable;	/* DSP��Ƭ������EMAC�Ƿ�ʹ��.                               */
	BOOL				fPciEnable;		/* DSP��Ƭ������PCI�Ƿ�ʹ��.	                            */
} DSP_CHIP_INFO;

/* ���ݽṹ˵��:�����ݽṹ����ָʾĿ����ϵͳ����Ϣ. */
typedef struct _TARGET_SYSTEM_INFO {
	DWORD32 			dwTargetTypeId;	/* Ŀ����Ӳ��ƽ̨���Ͱ汾��.								*/
	DSP_CHIP_INFO		DspChipInfo;	/* �й�DSPƬ������ʹ�������Լ���ǰʹ��DSPģʽ�й���Ϣ.	    */
} TARGET_SYSTEM_INFO;


extern DWORD32 g_dwLed;

extern int g_iResetFlag;

__inline int GetResetFlag()
{
	return g_iResetFlag;
}

__inline void SetResetFlag()
{
	g_iResetFlag = 1;
}

/* ��������API�������ӿ�˵��. */
/*------------------------------------------------------------------------------------------------------*/
/* ����˵��:�ú���Ϊ����Ϊ��λ���ӳ�,�ӳ�ʱ��Ϊ"dwDelayTime * 1ms ".�����޷���ֵ. */
extern void Target_DelayMS(
	DWORD32 					dwDelayTime		//�ӳ�MS����.
);

/*
����˵��:�ú�������ȡ��Ŀ��ϵͳӲ���汾�ź�������ڹ���Ϣ.
��������ֵ:
	����S_OK��ʾȡ��Ϣ�ɹ�;	
	����E_POINTER��ʾpSysInfoΪINVALID_POINTER; */
extern HRESULT GetTargetSysInfo( 
	TARGET_SYSTEM_INFO 			*pSysInfo		//ָ��ȡ��ƽ̨�й���Ϣ����.
);

/* ����˵��:�ú��������ر�ȫ���ж�,�������عر�ȫ���ж�֮ǰ��ȫ���ж�ʹ��. */
extern DWORD32 CloseGlobalInterrupt( void );

/* ����˵��:�ú��������ָ�ȫ���ж�,���ݴ����ȫ���ж�ʹ�����. */
extern void RestoreGlobalInterrupt( DWORD32 dwGie );

/* ����˵��:�ر�ȫ���ж�ʹ��. */
extern void CloseAllInterrupt( void );

/* ����˵��:ʹ��ȫ���ж�. */
extern void EnableAllInterrupt( void );

/* ����˵��:�ú���������������Ϩ��ϵͳƽ̨�Ĺ���ָʾ��,�統ǰLED��Ϊ������øú���֮��Ƽ���,�����޷���ֵ. */
extern void WorkLedLight( void );

/* ����˵��:�ú���������������Ϩ��ϵͳƽ̨��LAN��,�統ǰLED��Ϊ������øú���֮��Ƽ���,�����޷���ֵ. */
extern void LanLedLight( void );
extern void SetLanLedOn( void );
extern void SetLanLedOff( void );
/* ����˵��:�ú���������������Ϩ��ϵͳƽ̨��Hardָʾ��,�統ǰLED��Ϊ������øú���֮��Ƽ���,�����޷���ֵ. */
extern void HardLedLight( void );
extern void SetHardLedOn( void );
extern void SetHardLedOff( void );
/* ����˵��:�ú���������������Ϩ��ϵͳƽ̨��Alarm1ָʾ��,�統ǰLED��Ϊ������øú���֮��Ƽ���,�����޷���ֵ. */
extern void Alarm1LedLight( void );
extern void SetAlarm1LedOn( void );
extern void SetAlarm1LedOff( void );
/* ����˵��:�ú���������������Ϩ��ϵͳƽ̨��Alarm2ָʾ��,�統ǰLED��Ϊ������øú���֮��Ƽ���,�����޷���ֵ. */
extern void Alarm2LedLight( void );
extern void SetAlarm2LedOn( void );
extern void SetAlarm2LedOff( void );
/* ����˵��:�ú��������÷�������.�����޷���ֵ. */
extern void SetBuzzerOn( void );
/* ����˵��:�ú��������÷���������.�����޷���ֵ. */
extern void SetBuzzerOff( void );

/* ����˵��:�ú�����������ϵͳƽ̨����Ϊ����ָʾ��.�������β�,�����޷���ֵ. */
extern void SetWorkLedOn( void );

/* ����˵��:�ú�������Ϩ��ϵͳƽ̨����Ϊ����ָʾ��.�������β�,�����޷���ֵ. */
extern void SetWorkLedOff( void );

// �Ϻ����ƶ��е�LED�Ʋ���
extern void WorkLedLight_Jiaoji( void );
extern void SetWorkLedOn_Jiaoji( void );
extern void SetWorkLedOff_Jiaoji( void );
extern void SetHardLedOn_Jiaoji( void );
extern void SetHardLedOff_Jiaoji( void );
extern void Lan1LedLight_Jiaoji( void );
extern void SetLan1LedOn_Jiaoji( void );
extern void SetLan1LedOff_Jiaoji( void );
extern void SetLan2LedOn_Jiaoji( void );
extern void SetLan2LedOff_Jiaoji( void );
extern void SetAlm1LedOn_Jiaoji( void );
extern void SetAlm1LedOff_Jiaoji( void );
extern void SetAlm2LedOn_Jiaoji( void );
extern void SetAlm2LedOff_Jiaoji( void );
extern void Link1LedLight_Jiaoji( void );
extern void Link2LedLight_Jiaoji( void );
extern void Link3LedLight_Jiaoji( void );
extern void Link4LedLight_Jiaoji( void );
extern void Link5LedLight_Jiaoji( void );
extern void Link6LedLight_Jiaoji( void );
extern void Link7LedLight_Jiaoji( void );
extern void Link8LedLight_Jiaoji( void );
extern void SetLink1LedOn_Jiaoji( void );
extern void SetLink1LedOff_Jiaoji( void );
extern void SetLink2LedOn_Jiaoji( void );
extern void SetLink2LedOff_Jiaoji( void );
extern void SetLink3LedOn_Jiaoji( void );
extern void SetLink3LedOff_Jiaoji( void );
extern void SetLink4LedOn_Jiaoji( void );
extern void SetLink4LedOff_Jiaoji( void );
extern void SetLink5LedOn_Jiaoji( void );
extern void SetLink5LedOff_Jiaoji( void );
extern void SetLink6LedOn_Jiaoji( void );
extern void SetLink6LedOff_Jiaoji( void );
extern void SetLink7LedOn_Jiaoji( void );
extern void SetLink7LedOff_Jiaoji( void );
extern void SetLink8LedOn_Jiaoji( void );
extern void SetLink8LedOff_Jiaoji( void );
extern void Stat1LedLight_Jiaoji( void );
extern void Stat2LedLight_Jiaoji( void );
extern void Stat3LedLight_Jiaoji( void );
extern void Stat4LedLight_Jiaoji( void );
extern void Stat5LedLight_Jiaoji( void );
extern void Stat6LedLight_Jiaoji( void );
extern void Stat7LedLight_Jiaoji( void );
extern void Stat8LedLight_Jiaoji( void );
extern void SetStat1LedOn_Jiaoji( void );
extern void SetStat1LedOff_Jiaoji( void );
extern void SetStat2LedOn_Jiaoji( void );
extern void SetStat2LedOff_Jiaoji( void );
extern void SetStat3LedOn_Jiaoji( void );
extern void SetStat3LedOff_Jiaoji( void );
extern void SetStat4LedOn_Jiaoji( void );
extern void SetStat4LedOff_Jiaoji( void );
extern void SetStatk5LedOn_Jiaoji( void );
extern void SetStat5LedOff_Jiaoji( void );
extern void SetStat6LedOn_Jiaoji( void );
extern void SetStat6LedOff_Jiaoji( void );
extern void SetStat7LedOn_Jiaoji( void );
extern void SetStat7LedOff_Jiaoji( void );
extern void SetStat8LedOn_Jiaoji( void );
extern void SetStat8LedOff_Jiaoji( void );

/* ����˵��:�ú���������WDT���ƹ���ι����ǰ״̬��Ϊ��.�������β�,�����޷���ֵ. */
extern void ClearWDT( void );

/* ����˵��:�ú���������WDT���ƹ���ι����ǰ״̬��Ϊ��.�������β�,�����޷���ֵ. */
extern void SetWDT( void );

/* ����˵��:�ú���������WDT���ƹ���״̬�и�����. �������β�,�����޷���ֵ. */
extern void ResWDT( void );

/* ����˵��:�ú�������ȡ��ϵͳƽ̨��������ĵ�ǰ״ֵ̬.�������β�,��������ֵ�����λ��Ч. */
extern BYTE8 GetTriggerIn( void );

/* ����˵��:�ú�������ȡ��ϵͳƽ̨��������ĵ�ǰ״ֵ̬.�������β�,��������ֵ�����λ��Ч. */
extern BYTE8 GetTriggerIn_2( void );

/* ����˵��:�ú�����������bSetBitλֵ���ô��������״̬.�����޷���ֵ. */
extern void SetTriggerOut(
	BYTE8 						bSetBit			//����ֵ,ֻ�����һλ��Ч.
);

/* ����˵��:�ú�����������bSetBitλֵ���ô��������״̬.�����޷���ֵ. */
extern void SetTriggerOutEx(
	BYTE8 bSetBit,			//����ֵ,ֻ�����һλ��Ч.
	DWORD32 dwVideoID		//��ƵID
);

/* ����˵��:�ú���������ʼ����ǰϵͳLED���MASK.*/
extern void InitLedMask();

/* ����˵��:�ú�������ȡ�õ�ǰϵͳLED�Ƶ�MASK.����ֱ�ӷ��ظ�MASK. */
extern DWORD32 GetLedMask( void );

/* ����˵��:�ú�����������ƽ̨��ʹ��dwLedMask����ʶ��GPIOָʾ��(ʹ��DSP��GPIO���ŵĵ�). */
extern void SetLedOn( 
	DWORD32 					dwLedMask		//LED�Ƶ�GPIO MASK.
);

/* ����˵��:�ú�������Ϩ��ƽ̨��ʹ��dwLedMask����ʶ��GPIOָʾ��(ʹ��DSP��GPIO���ŵĵ�). */
extern void SetLedOff( 
	DWORD32 					dwLedMask		//LED�Ƶ�GPIO MASK.
);

/* ����˵��:����ֱ�ӷ���Ӳ��ƽ̨��������. */
extern DWORD32 GetJumperInput( void );

/* ����˵��:�ú�������ȡ��Ӳ��ƽ̨�û����뿪������ֵ.�������β�.����ֱ�ӷ��ظ�ֵ. */
extern DWORD32 GetUserConfigPort( void );
/*------------------------------------------------------------------------------------------------------------------------------------*/




/* �����������API�������ӿ�˵��. */
/*------------------------------------------------------------------------------------------------------------------------------------*/
/* 
����˵��:�ú�������ȡ��IDоƬ��ID��,��ID���������ɸ�APIʹ���߽���.
��������ֵ:
	����S_OK��ʾ��ȡ�ɹ�.	
	����E_POINTER��ʾpbIdValueΪINVALID_POINTER.	
	����E_NOTIMPL��ʾ��ǰӲ��ƽ̨��֧��.
	����E_FAIL��ʾӲ���ײ��������.
	����S_FALSE��ʾֻ����ID�ŵĲ������ݻ���dwBytesToRead����ʵ��ΨһID���ݳ���.	*/
extern HRESULT GetUniqueId( 
	PBYTE8						pbIdValue,		//ָ��ȡ�õ�ID������.
	DWORD32						dwBytesToRead,	//������ȡ�������ֽڳ���.
	PDWORD32					pdwBytesRead	//ʵ�ʶ����������ֽڳ���,ΪNULL����.
);

#ifdef _HV_PRODUCT
/* 
����˵��:�ú���������FLASH�ж�ȡ����,��ǰӲ��ƽ̨���ж�FLASH�Ķ���������ͨ���ú������ܱ�֤��FLASH��
	ȡ��ȷ/��Ч.
��������ֵ:
	����S_OK��ʾ��ȡ�ɹ�.
	����E_POINTER��ʾpbMemDstΪINVALID_POINTER.
	����E_INVALIDARG��ʾ����Ĳ����д�,��ȡ�����ݳ���FLASH��ǰ������.
	����E_FAIL��ʾӲ���ײ��������.
	����S_FALSE��ʾ��ȡFLASH����. */
extern HRESULT FLASH_Read(
	DWORD32 					dwSrcPos,		//��FLASH��ȡ���ݵ���ʼλ��.
	PBYTE8						pbMemDst, 		//ָ���ڴ��з��ö���������.
	BYTE8						bReadType,		//FLASH��ȡģʽ,Ϊ0ʱ��ʾΪ͸����,Ϊ1Ϊ���ܶ�
	DWORD32						dwBytesToRead,	//������ȡ�������ֽڳ���.
	PDWORD32					pdwBytesRead	//ʵ�ʶ����������ֽڳ���,ΪNULL����.
	
);

/* 
����˵��:�ú���������FLASH��д������,��ǰӲ��ƽ̨���ж�FLASH��д��������ͨ���ú������ܱ�֤��FLASH��д��
	������ȷ/��Ч(������д��FLASHҳ��������ȱ�ERASE��).
��������ֵ:
	����S_OK��ʾд������ɹ�.
	����E_POINTER��ʾpbMemSrcΪINVALID_POINTER.
	����E_INVALIDARG��ʾ����Ĳ����д�,д������ݳ���FLASH��ǰ������.
	����E_NOTIMPL��ʾ��ǰFLASHд��֧��.	
	����E_FAIL��ʾӲ���ײ��������.
	����S_FALSE��ʾд����FLASH����. */
extern HRESULT FLASH_Write( 
	const PBYTE8 				pbMemSrc,		//ָ����Ҫ��ȡ���ڴ�����.
	DWORD32 					dwDstPos,		//FLASHд������Ŀ����ʼλ��.
	BYTE8 						bWriteType,		//FLASHдģʽ,0Ϊ͸��д,1Ϊ����д
	DWORD32						dwBytesToWrite,	//����д��������ֽڳ���.
	PDWORD32 					pdwBytesWrite	//ʵ��д��������ֽڳ���,ΪNULL����.
);

/*
����˵��:�ú�����������FLASH������,���ƽ���洢��С��Ԫȫ����Ϊ1,FLASH��д����ֻ�ܶ���С��Ԫ����1->0��
	д����,�����ɽ���0->1�Ĳ���,������д������֮ǰ��Ҫ����洢���Ƚ��в�������,��������С��λΪ��.
��������ֵ:
	����S_OK��ʾд������ɹ�.
	����E_INVALIDARG��ʾ����Ĳ����д�.
	����E_NOTIMPL��ʾ��ǰFLASH������֧��.
	����E_FAIL��ʾӲ���ײ��������.
	����S_FALSE��ʾд����FLASH����. */
extern HRESULT FLASH_Erase( 
	DWORD32						dwStartPos, 	//��Ҫ����FLASH����λ��.
	DWORD32 					dwLength		//��ǰ��Ҫ��������,���ֽ�Ϊ��λ.
);

extern HRESULT FLASH_RandomWrite(
	DWORD32 					dwFlashAddr, 
	BYTE8 						*pSrcData, 
	int 						iLen
);
#endif

/*
����˵��:�ú�����������FLASH�洢ʱ��ʹ�õļ�������.
��������ֵ:
	����S_OK��ʾ���óɹ�.
	����E_POINTER��ʾpbSrcCodeΪINVALID_POINTER. */
extern HRESULT SetSecurityCode( 
	const BYTE8					*pbSrcCode,		//ָ����Ϊ�������ӵ�����.
	DWORD32						dwCodeLen		//����������ӵ������ֽڳ���.
);


/* ��ʼ��EEPROM */
extern HRESULT EEPROM_Init();

/* ��ȡEEPROM�������� */

extern HRESULT EEPROM_FreeRead(
	DWORD32 					dwReadPos,			//��ȡEEPROM����λ��.
	PBYTE8						pbMemDst,			//ָ�����������.
	DWORD32 					dwBytesToRead,		//������ȡ�������ֽڳ���.
	PDWORD32					pdwBytesRead		//ʵ�ʶ�ȡ�������ֽڳ���,ΪNULL����.
);

/* ��ȡEEPROM���������� */
extern HRESULT EEPROM_EncryptRead(
	DWORD32						dwReadPos,		//��ȡEEPROM����λ��.
	PBYTE8 						pbMemDst, 		//ָ�����������.
	DWORD32						dwBytesToRead,	//������ȡ�������ֽڳ���.
	PDWORD32					pdwBytesRead		//ʵ�ʶ�ȡ�������ֽڳ���,ΪNULL����.
);

/* д��EEPROM�Ǽ����� */
extern HRESULT EEPROM_FreeWrite(
	const PBYTE8				pbMemSrc, 		//ָ��д�������.
	DWORD32 					dwWritePos, 	//д��EEPROM���׵�ַ.
	DWORD32						dwBytesToWrite,	//����д��������ֽڳ���.
	PDWORD32					pdwBytesWrite	//ʵ��д��������ֽڳ���,ΪNULL����.
);

/* д��EEPROM������ */
extern HRESULT EEPROM_EncryptWrite(
	const PBYTE8				pbMemSrc, 		//ָ��д�������.
	DWORD32 					dwWritePos, 	//д��EEPROM���׵�ַ.
	DWORD32						dwBytesToWrite,	//����д��������ֽڳ���.
	PDWORD32					pdwBytesWrite	//ʵ��д��������ֽڳ���,ΪNULL����.
);

/*
����˵��:�ú���������EEPROM�ж�ȡһ�����ݵ�����.
��������ֵ:
	����S_OK��ʾ��ȡEEPROM�����ɹ�.
	����E_POINTER��ʾpbMemDstΪINVALID_POINTER.
	����E_INVALIDARG��ʾ����Ĳ����д�.
	����E_FAIL��ʾӲ���ײ��������.
	����S_FALSE��ʾ��ȡEEPROM����. */
extern HRESULT EEPROM_Read(
	DWORD32 					dwReadPos,		//��ȡEEPROM����λ��.
	PBYTE8						pbMemDst,		//ָ�����������.
	DWORD32 					dwBytesToRead,	//������ȡ�������ֽڳ���.
	PDWORD32					pdwBytesRead	//ʵ�ʶ�ȡ�������ֽڳ���,ΪNULL����.
);

/*
����˵��:�ú���������EEPROM��д��һ��������.
��������ֵ:
	����S_OK��ʾд��EEPROM�����ɹ�.
	����E_POINTER��ʾpbMemSrcΪINVALID_POINTER.
	����E_INVALIDARG��ʾ����Ĳ����д�.
	����E_FAIL��ʾӲ���ײ��������.
	����S_FALSE��ʾд��EEPROM����. */
extern HRESULT EEPROM_Write(
	const PBYTE8				pbMemSrc,		//ָ��д�������.
	DWORD32 					dwWritePos, 	//д��EEPROM���׵�ַ.
	DWORD32 					dwBytesToWrite, //����д��������ֽڳ���.
	PDWORD32					pdwBytesWrite	//ʵ��д��������ֽڳ���,ΪNULL����.
);

/*
����˵��:�ú���������ȡʵʱʱ��оƬ�е�ʵʱʱ��ֵ(������),��ȷ����,���ǵ�ǰ��ʵʱʱ��оƬ�����Ϊ100��������.
��������ֵ:
	����S_OK��ʾ��ȡRTC����ʱ��ɹ�.
	����E_POINTER��ʾpRealTimeΪINVALID_POINTER.
	����E_FAIL��ʾӲ���ײ��������.
	����S_FALSE��ʾ��ȡRTC����ʱ�����. */
extern HRESULT RTC_GetDateTime( 
	REAL_TIME_STRUCT 			*pRealTime		//ָ�������ʱ��.
);

/*
����˵��:�ú�����������ʵʱʱ��оƬʱ��(��������),��ȷ����.�����ݴ��ĸ��ֽ������������.�����û������õ�����
	д����RTC���ʱ�����ڼĴ�����.
��������ֵ:
	����S_OK��ʾ����RTC����ʱ��ɹ�.
	����E_POINTER��ʾpbTimeValΪINVALID_POINTER.
	����E_INVALIDARG��ʾ�����ʵʱʱ�����ݷǷ�.
	����E_FAIL��ʾӲ���ײ��������.
	����S_FALSE��ʾ����RTC����ʱ�����. */
extern HRESULT RTC_SetDateTime(
	const REAL_TIME_STRUCT 		*pRealTime		//ָ�����õ�ʱ��.
);

/* 
����˵��:�ú���������ComPort���ڵķ��Ͷ�������д�ַ�,�����ǰ�ô��ڶ˿ڷ��Ͷ�������������ȴ���ʱ�ж�
	(��ʱ�жϽ����Ͷ����е����ݷ��������ڶ˿ڵ�Ӳ��FIFO��),ֻ������Ҫ��ķ����ַ�������'\0'ʱ�Ż�
	ֹͣ���Ͷ�������д����.
	�ú���ֻ����TSK����SWI��ʹ��,������ܻ���������״̬.
��������ֵ:
	����S_OK��ʾ�򴮿ڶ˿�д�����ݳɹ�.
	����E_POINTER��ʾpszStringΪINVALID_POINTER.
	����E_INVALIDARG��ʾ����Ĳ����д�,�紮�ڶ˿ڷǷ�.
	����E_FAIL��ʾӲ���ײ��������.
	����S_FALSE��ʾ�򴮿ڶ˿�д������ʧ��. */
extern HRESULT ComWriteStr(
	SYS_COM_PORT 				ComPort, 		//��ʾ��ǰ�����Ĵ��ڶ˿ں�.
	const PCHAR					pszString		//ָ����Ҫд����ַ���,�ô���'\0'Ϊ������־
);

/* 
����˵��:�ú���������ComPort���ڵķ��Ͷ�������д����������,�����ǰ�ô��ڶ˿ڷ��Ͷ��������
	�����ȴ���ʱ�ж�(��ʱ�жϽ����Ͷ����е����ݷ��������ڶ˿ڵ�Ӳ��FIFO��).
	�ú���ֻ����TSK����SWI��ʹ��,������ܻ���������״̬.
��������ֵ:
	����S_OK��ʾ�򴮿ڶ˿�д�����ݳɹ�.
	����E_POINTER��ʾpbDataΪINVALID_POINTER.
	����E_INVALIDARG��ʾ����Ĳ����д�,�紮�ڶ˿ڷǷ�.
	����E_FAIL��ʾӲ���ײ��������.
	����S_FALSE��ʾ�򴮿ڶ˿�д������ʧ��. */
extern HRESULT ComWriteData(
	SYS_COM_PORT 				ComPort, 		//��ʾ��ǰ�����Ĵ��ڶ˿ں�.
	const PBYTE8				pbData,			//ָ����Ҫд�������
	DWORD32						dwBytesToWrite,	//����д��������ֽڸ���.
	PDWORD32					pdwBytesWrite	//ʵ��д��������ֽڸ���,ΪNULL����.
);

/*
����˵��:�ú���������ComPort���ڵĽ��ն����ж�ȡ�ַ���,�����ǰ�ô��ڶ˿ڽ��ն���Ϊ����������ȴ���ʱ�ж�
	(��ʱ�жϻ�ô��ڶ˿ڵ�Ӳ��FIFO�ж�ȡ����).�ú���ֻ����������س����Ż������жϵ�ǰ��ȡ�ַ�������,
	�ڶ�ȡ�ַ������ȳ���������󳤶�ʱ,�ú���������pszString�������κ�����(������յ�ȫ������).
	�ú���ֻ����TSK����SWI��ʹ��,������ܻ���������״̬.
��������ֵ:
	����S_OK��ʾ�Ӵ��ڶ˿ڶ�ȡ���ݳɹ�.
	����E_POINTER��ʾpszStringΪINVALID_POINTER.
	����E_INVALIDARG��ʾ����Ĳ����д�,紮�ڶ˿ڷǷ?
	����E_FAIL��ʾӲ���ײ��������.
	����S_FALSE��ʾ�Ӵ��ڶ˿ڶ�ȡ�����ݳ��ȳ����û����趨��������ݳ���. */
extern HRESULT ComReadStr(
	SYS_COM_PORT 				ComPort, 		//��ʾ��ǰ�����Ĵ��ڶ˿ں�.
	PCHAR 						pszString,		//ָ��������ַ�������.
	DWORD32						dwMaxBytesToRead,//������������ַ���Ŀ.
	PDWORD32					pdwBytesRead	//ʵ�ʶ������ַ�����,�����س���,���ΪNULL����.
);

/*
����˵��:�ú���������ComPort���ڵĽ��ն����ж�ȡ�ַ�����,�����ǰ�ô��ڶ˿ڽ��ն���Ϊ��ֱ�ӷ��ص�ǰʵ�ʶ���
	�����ݸ���.��ʱ�ж�(���жϺ����л�ô��ڶ˿ڵ�Ӳ��FIFO�ж�ȡ����).
��������ֵ:
	����S_OK��ʾ�Ӵ��ڶ˿ڶ�ȡ���ݳɹ�.
	����E_POINTER��ʾpbDataΪINVALID_POINTER.
	����E_INVALIDARG��ʾ����Ĳ����д�,�紮�ڶ˿ڷǷ�.
	����E_FAIL��ʾӲ���ײ��������.
	����S_FALSE��ʾ�Ӵ��ڶ˿ڶ�ȡ�����ݸ�������dwBytesToRead��ָ���ĸ���. */
extern HRESULT ComReadData(
	SYS_COM_PORT 				ComPort, 		//��ʾ��ǰ�����Ĵ��ڶ˿ں�.
	PBYTE8						pbData,			//ָ�����������
	DWORD32						dwBytesToRead,	//����������ַ���Ŀ.
	PDWORD32					pdwBytesRead	//ʵ�ʶ������ַ�����,���ΪNULL����.
);

/*
����˵��:�ú��������崮�ڵ���Ӳ��
*/
extern HRESULT ClearComData(
	SYS_COM_PORT 				ComPort 		//��ʾ��ǰ�����Ĵ��ڶ˿ں�.
);

/*
����˵��:�ú��������򴮿�д���ַ���,�����ַ������н����ַ�'\0'Ϊ��ֹ��־��.
	�ú����������κεط�����,���Ǳ���Ҫ��ȷ�÷��ͳ�����һֱ�ȴ�Ӳ�����ͽ�����ŷ��ص�.
��������ֵ:
	����S_OK��ʾ�򴮿ڶ˿�д�����ݳɹ�.
	����E_POINTER��ʾpszStringΪINVALID_POINTER.
	����E_INVALIDARG��ʾ����Ĳ����д�,�紮�ڶ˿ڷǷ�.
	����E_FAIL��ʾӲ���ײ��������.
	����S_FALSE��ʾ�򴮿ڶ˿�д������ʧ��. */
extern HRESULT COM_WriteStr( 
	SYS_COM_PORT 				ComPort, 		//��ʾ��ǰ�����Ĵ��ڶ˿ں�.
	const PCHAR					pszString		//ָ����Ҫд����ַ�������.
);

/*
����˵��:�ú��������򴮿�д�붨��������.
	�ú����������κεط�����,���Ǳ���Ҫ��ȷ�÷��ͳ�����һֱ�ȴ�Ӳ�����ͽ�����ŷ��ص�.
��������ֵ:
	����S_OK��ʾ�򴮿ڶ˿�д�����ݳɹ�.
	����E_POINTER��ʾpszStringΪINVALID_POINTER.
	����E_INVALIDARG��ʾ����Ĳ����д�,�紮�ڶ˿ڷǷ�.
	����E_FAIL��ʾӲ���ײ��������.
	����S_FALSE��ʾ�򴮿ڶ˿�д������ʧ��. */
extern HRESULT COM_WriteData(
	SYS_COM_PORT 				ComPort, 		//��ʾ��ǰ�����Ĵ��ڶ˿ں�.
	const PBYTE8				pbData,			//ָ����Ҫд�������
	DWORD32						dwBytesToWrite,	//����д��������ֽڳ���.
	PDWORD32					pdwBytesWrite	//ʵ��д��������ֽڳ���,���ΪNULL����.
);

/*
����˵��:�ú��������Ӵ��ڶ�ȡ�ַ���,�ú���ֻ���������س���ʱ�Ż���������.�ڶ�ȡ�ַ������ȳ���������󳤶�ʱ,
	�ú���������pszString�������κ�����(������յ�ȫ������).
	�ú����������κεط�����,��������ѯ���ڶ˿�Ӳ��FIFO�Ƿ���յ��ַ���,�����������񶼱���ǰ������ռ.
��������ֵ:
	����S_OK��ʾ�Ӵ��ڶ˿ڶ�ȡ���ݳɹ�.
	����E_POINTER��ʾpszStringΪINVALID_POINTER.
	����E_INVALIDARG��ʾ����Ĳ����д�,�紮�ڶ˿ڷǷ�.
	����E_FAIL��ʾӲ���ײ��������.
	����S_FALSE��ʾ�Ӵ��ڶ˿ڶ�ȡ����ʧ��. */
extern HRESULT COM_ReadStr( 
	SYS_COM_PORT 				ComPort, 		//��ʾ��ǰ�����Ĵ��ڶ˿ں�.
	PCHAR 						pszString,		//ָ��������ַ�������.
	DWORD32						dwMaxBytesToRead,//������������ַ���Ŀ.
	PDWORD32					pdwBytesRead	//ʵ�ʶ������ַ�����,�����س���,���ΪNULL����.
);

/*
����˵��:�������Ӵ��ڶ�ȡ����,�Զ��������õ������ݳ���Ϊ��ֹ����.
	�ú����������κεط�����,��������ѯ�����Ӳ��FIFO�Ƿ���յ��ַ���,�����������񶼱���ǰ������ռ.
��������ֵ:
	����S_OK��ʾ�Ӵ��ڶ˿ڶ�ȡ���ݳɹ�.
	����E_POINTER��ʾpbData����pdwLenΪINVALID_POINTER.
	����E_INVALIDARG��ʾ����Ĳ����д�,�紮�ڶ˿ڷǷ�.
	����E_FAIL��ʾӲ���ײ��������.
	����S_FALSE��ʾ�Ӵ��ڶ˿ڶ�ȡ����ʧ��. */
extern HRESULT COM_ReadData(
	SYS_COM_PORT 				ComPort, 		//��ʾ��ǰ�����Ĵ��ڶ˿ں�.
	PBYTE8						pbData,			//ָ�����������
	DWORD32						dwBytesToRead,	//����������ַ���Ŀ.
	PDWORD32					pdwBytesRead	//ʵ�ʶ������ַ�����,���ΪNULL����.
);	

/*
����˵��:�ú����Ӵ��ڶ˿ڶ�ȡһ�ַ�,����ֱ�ӷ��ظ��ַ�.
	�ú����������κεط�����,��������ѯ���ڶ˿�Ӳ��FIFO�Ƿ���յ��ַ���,�����������񶼱���ǰ������ռ. */
extern CHAR COM_GetChar( 
	SYS_COM_PORT 				ComPort 		//��ʾ��ǰ�����Ĵ��ڶ˿ں�.
);

/*
����˵��:�ú����򴮿ڶ˿����һ���ַ�,�����޷���ֵ.
	�ú����������κεط�����,���Ǳ���Ҫ��ȷ�÷��ͳ�����һֱ�ȴ�Ӳ�����ͽ�����ŷ��ص�. */
extern void COM_PutChar( 
	SYS_COM_PORT 				ComPort, 		//��ʾ��ǰ�����Ĵ��ڶ˿ں�.
	CHAR						chVal			//��ǰҪ���͵�����.
);
/*------------------------------------------------------------------------------------------------------------------------------------*/



/* 
����˵��:�ú��������жϵ�ǰ����Ƶ�˿��Ƿ�ʹ��.
��������ֵ:
	����S_OK��ʾȡ�����������ɹ�.
	����E_INVALIDARG��ʾ����Ĳ����д�,�ö˿�ϵͳ��ǰ��֧��.
	����E_POINTER��ʾpfPresentΪINVALID_POINTER.
	����S_FALSE��ʾ��ǰ����Ƶ�˿�δʹ��. */
extern HRESULT VIDEO_IsPortPresent(
	int 				iPort, 
	PBOOL 				pfPresent
);


/*------------------------------------------------------------------------------------------------------------------------------------*/
// ATA IDE Ӳ�̽ӿ�
extern HRESULT ATA_ReadSector(
	DWORD32					dwDeviceID,	
	DWORD32					dwSectorStartLBA,
	PBYTE8					pbMemDst,
	DWORD32					dwSectorsToRead,
	PDWORD32				pdwReadSectors
);

extern HRESULT ATA_WriteSector(
	DWORD32					dwDeviceID,	
	PBYTE8					pbMemSrc,
	DWORD32					dwSectorStartLBA,
	DWORD32					dwSectorsToWrite,
	PDWORD32				pdwWriteSectors
);

extern HRESULT ATA_HdCheck( void );
extern HRESULT ATA_HdReadId(
	PCHAR				pszString,
	DWORD32				dwBytesToRead,
	PDWORD32			pdwReadBytes
);
extern HRESULT ATA_Reset( void );
/*------------------------------------------------------------------------------------------------------------------------------------*/

// SATA �ӿ�.
/*------------------------------------------------------------------------------------------------------------------------------------*/
extern HRESULT SATA_ReadSector(
	DWORD32					dwDeviceID,	
	DWORD32					dwSectorStartLBA,
	PBYTE8					pbMemDst,
	DWORD32					dwSectorsToRead,
	PDWORD32				pdwReadSectors
);

extern HRESULT SATA_ReadSectors(
	DWORD32					dwDeviceID,	
	DWORD32					dwSectorStartLBA,
	PBYTE8					pbMemDst,
	DWORD32					dwSectorsToRead,
	PDWORD32				pdwReadSectors
);
extern HRESULT SATA_WriteSector(
	DWORD32					dwDeviceID,	
	PBYTE8					pbMemSrc,
	DWORD32					dwSectorStartLBA,
	DWORD32					dwSectorsToWrite,
	PDWORD32				pdwWriteSectors
);

extern HRESULT SATA_WriteSectors(
	DWORD32			dwDeviceID,	
	PBYTE8			pbMemSrc,
	DWORD32			dwSectorStartLBA,
	DWORD32			dwSectorsToWrite,
	PDWORD32		pdwWriteSectors
);

extern HRESULT SATA_HdCheck( PBYTE8 pVar );
extern HRESULT SATA_HdReadId(
	PCHAR				pszString,
	DWORD32				dwBytesToRead,
	PDWORD32			pdwReadBytes
);
extern HRESULT SATA_Reset( void );
extern int SATA_GetStatus();
extern WORD16 SATA_GetDiskSize();
extern HRESULT SATA_CheckError( PDWORD32 pdwError );
/*------------------------------------------------------------------------------------------------------------------------------------*/

extern HRESULT NAND_ReadFlashId(
	PBYTE8					pbID,
	DWORD32					dwBytesToRead,
	PDWORD32				pdwReadBytes
);


//������
extern HRESULT NANDFLASH_Read(
	DWORD32 					dwSrcPos,		//��FLASH��ȡ���ݵ���ʼλ��.
	PBYTE8						pbMemDst, 		//ָ���ڴ��з��ö���������.
	BYTE8						bReadType,		//FLASH��ȡģʽ,Ϊ0ʱ��ʾΪ͸����,Ϊ1Ϊ���ܶ�
	DWORD32						dwBytesToRead,	//������ȡ�������ֽڳ���.
	PDWORD32					pdwBytesRead	//ʵ�ʶ����������ֽڳ���,ΪNULL��?
	
);

//ҳд������ִ�д˯��֮ǰ���뱣֤ҳ�������ֽڱ���ȫΪ0xFF����������Ȳ���
extern HRESULT NANDFLASH_Write(
	const PBYTE8 				pbMemSrc,		//ָ����Ҫ��ȡ���ڴ�����.
	DWORD32 					dwDstPos,		//FLASHд������Ŀ����ʼλ��.
	BYTE8 						bWriteType,		//FLASHдģʽ,0Ϊ͸��д,1Ϊ����д
	DWORD32						dwBytesToWrite,	//����д��������ֽڳ���.
	PDWORD32 					pdwBytesWrite	//ʵ��д��������ֽڳ���,ΪNULL����.
);

//ҳд������ִ�д˺���֮ǰ���뱣֤ҳ�������ֽڱ���ȫΪ0xFF����������Ȳ���
extern HRESULT NANDFLASH_WritePage(
	const PBYTE8 				pbMemSrc,		//ָ����Ҫ��ȡ���ڴ�����.
	DWORD32 					dwDstPage,		//FLASHд������Ŀ����ʼλ��.
	BYTE8 						bWriteType,		//FLASHдģʽ,0Ϊ͸��д,1Ϊ����д
	DWORD32						dwPagesToWrite,	//����д��������ֽڳ���.
	PDWORD32 					pdwPagesWrite	//ʵ��д��������ֽڳ���,ΪNULL����.
);

//���������
extern HRESULT NANDFLASH_Erase( 
	DWORD32						dwStartPos, 	//��Ҫ����FLASH����λ��.
	DWORD32 					dwLength		//��ǰ��Ҫ��������,���ֽ�Ϊ��λ.
);

//Blank ��麯��
extern HRESULT NANDFLASH_Verify(
	DWORD32						dwStartPos, 	//��Ҫ����FLASH����λ��.
	DWORD32 					dwLength		//��ǰ��Ҫ���ĳ���,���ֽ�Ϊ��λ.
);

/*
����˵��:�ú�����������CPU��һЩBUG�������޸�,�ʶ��ú�����Ҫ�����̳߳�ʼ��ʱ����.
*/
extern HRESULT InitCPURegisterOk( void );

/*
����˵��:�ú���������⵱ǰ�Ƿ���VVD�Ľӿڰ�.
*/
extern HRESULT IsVVDInterfaceBoard( void );

/* ����˵��:�ú�����������bSetBitλֵ����DPIO 3�����������״̬.�����޷���ֵ. */
extern void SetTriggerOutDPIO(
	BYTE8 	bPos,				// λ��
	BYTE8 	bSetBit			// ����ֵ,ֻ�����һλ��Ч(0��1)
);

#ifdef __cplusplus
	}
#endif /* #ifdef __cplusplus */



#endif /* #ifndef _HVTARGET_H */


