#ifndef _HVDSP_PRIVATE_H
#define _HVDSP_PRIVATE_H

#ifdef __cplusplus
	extern "C" {
#endif	/* #ifdef __cplusplus */

#include "dspsys_def.h"
#include "dsp64xdefine.h"
#include "hvtarget.h"

/* Jpegѹ����˽Ϊ�ڲ���������. */
/*------------------------------------------------------------------------------------------------------*/
/*
����˵��:�ú���������ʼ��JPEGѹ������ʱ��ʹ�õ�ջ�ռ����.
��������S_OK��ʾ��ʼ���ɹ�.*/
extern HRESULT Jpeg_MemInit(
	int 						iInHeap, 		//Ƭ�ڶ�����.
	int 						iExtHeap 		//Ƭ��SDRAM������.
);
/*------------------------------------------------------------------------------------------------------*/

/* FLASH�ļ�ϵͳ��˽Ϊ�ڲ���������. */
/*------------------------------------------------------------------------------------------------------*/
/*
����˵��:�ú����������ļ�ϵͳ�������Ƴ�ʼ��,�����dwInitFileNumΪ0ʱ��ʾ�������й�
	�ļ�ϵͳ�е��ļ�ͷ�����ڴ�,�Ա����ļ�ϵͳ��������ʵ��.
��������ֵ:
	����S_OK��ʾ��ʼ���ɹ�.
	����E_FAIL��ʾӲ���ײ���������쳣.
	����S_FALSE��ʾ��ʼ��ʧ��,�統ǰû��ƽ̨û���ļ�ϵͳ��. */
extern HRESULT FILE_InitFileSystem( 
	DWORD32 					dwInitFileNum		//��ʼ���ļ���Ŀ.
);

/*
����˵��:�ú���������ʼ��FLASH CACHE�������ݽṹ.
��������ֵ:
	����S_OK��ʾ��ʼ���ɹ�.
	����E_POINTER��ʾpFlashCfgΪINVALID_POINTER.
	����E_FAIL��ʾ��ʼ��ʧ��,�˺��FLASH CACHE������ʽȫ��ʧЧ. */
extern HRESULT InitFlash( 
	FLASH_USER_CONFIG			*pFlashCfg
);

/*
����˵��:�ú���������FLASH�ж�ȡ����,���ڸ�FLASH����������Ҫ�κ�����������д����,
	���Կ���ֱ�Ӷ�FLASH������ռ���ж�����.
��������ֵ:
	����S_OK��ʾ��ȡ�ɹ�.
	����E_POINTER��ʾpbMemDstΪINVALID_POINTER.
	����E_INVALIDARG��ʾ����Ĳ����д�,��ȡ�����ݳ���FLASH��ǰ������.
	����E_FAIL��ʾӲ���ײ��������.
	����S_FALSE��ʾ��ȡFLASH����. */
extern HRESULT FLASH_CacheRead( 
	DWORD32 					dwSrcPos,		//��FLASH��ȡ���ݵ���ʼλ��.
	PBYTE8						pbMemDst, 		//ָ���ڴ��з��ö���������.
	DWORD32						dwBytesToRead,	//������ȡ�������ֽڳ���.
	PDWORD32					pdwBytesRead	//ʵ�ʶ����������ֽڳ���,ΪNULL����.
);

/* 
����˵��:�ú���������FLASH��д������,FLASH�ĵ�ַ�ռ�Ϊ�����ĵ�ַ�ռ�,����FLASH�Ķ�д
	��������Ҫʹ��ר�õ�FLASH��д��װ����,�������ֲ���Ԥ֪�Ľ��.
��������ֵ:
	����S_OK��ʾд������ɹ�.
	����E_POINTER��ʾpbMemSrc,ΪINVALID_POINTER.
	����E_INVALIDARG��ʾ����Ĳ����д�,д������ݳ���FLASH��ǰ������.
	����E_FAIL��ʾӲ���ײ��������.
	����S_FALSE��ʾд����FLASH����. */
extern HRESULT FLASH_CacheWrite( 
	const PBYTE8 				pbMemSrc,		//ָ����Ҫ��ȡ���ڴ�����.
	DWORD32 					dwDstPos,		//FLASHд������Ŀ����ʼλ��.
	DWORD32						dwBytesToWrite,	//����д��������ֽڳ���.
	PDWORD32 					pdwBytesWrite	//ʵ��д��������ֽڳ���,ΪNULL����.
);


/*
����˵��:�ú���������FLASH CACHE�е�����д����FLASH��, FLASH�ĵ�ַ�ռ�Ϊ�����ĵ�ַ�ռ�,
	����FLASH��д����ʱ,�����ǰ��д�������漰�����ݷǳ�����ʱ,Ӧ��ִ��д����֮��,
	����ִ��һ��FlushFlash����,��ʹ�ý�FLASH CACHE�е����������������ⲿFLASH��.
��������ֵ:
	����S_OK��ʾ�����ɹ�.
	����E_FAIL��ʾ����ʧ��.	 */
extern HRESULT FlushFlash( void );

/*
����˵��:��ȡFLASH ROMоƬ���豸ID��.
��������ֵ:
	����S_OK��ʾ��ȡ�ɹ�.
	����E_POINTER��ʾpdwFlashIDΪINVALID_POINTER.
	����E_FAIL��ʾ��ȡIDоƬ��ID��ʧ��.	*/
extern HRESULT FLASH_readID(
	PDWORD32					pdwFlashID		//ָ����ȡ�õ�ID����.
);

/*------------------------------------------------------------------------------------------------------*/


/* �����ӿ���˽Ϊ�ڲ��ӿں�������. */
/*------------------------------------------------------------------------------------------------------*/
typedef void * TIMER_HANDLE;			/* DSPƬ���û�ʱ��ģ��Ĳ������. */
typedef void * I2C_HANDLE;				/* DSPƬ������I2Cģ��Ĳ������. */
typedef void * GPIO_HANDLE;				/* DSPƬ������GPIOģ��Ĳ������. */

/* ���ݽṹ˵��:�����ݽṹ���������ָʾĿ���ϵͳ����Ĳ������. */
typedef struct SYS_DEVICE_HANDLE {
	I2C_HANDLE 					hSysI2C;		/* I2C�豸���.												*/
	GPIO_HANDLE					hSysGpio;		/* GPIO�豸���.                                            */
	TIMER_HANDLE				hSysTimer;		/* �û�ʱ���豸���.                                        */
} SYS_DEVICE_HANDLE;

/*
����˵��:�ú���������ʼ��Ŀ��ϵͳ,��ʼ��DSPƬ�����蹤������,�Լ��������蹤����������.�ú���һ�ν�ϵͳ
	��ʼ����������,����ȡ���й�Ӳ��ƽ̨����Ϣ.�ú���Ϊ�Զ��ӵ�ǰƽ̨�İ汾����ȡ����ǰ��Ӳ���汾��Ϣ,
	���Ҹ��ݵ�ǰӲ���汾�Ĳ�ͬ��ѡ��ͬ�ĳ�ʼ����ʽ.
��������ֵ:
	����S_OK��ʾ��ʼ���ɹ�;
	����S_FALSE��ʾ��ʼ��ʧ��;
	����E_FAIL���ʾΪӲ��ϵͳ������.*/
extern HRESULT InitHvSysTarget(
	SYS_DEVICE_HANDLE			*pSysHandle		//ָ���豸���,ΪNULLʱ�򲻴����豸���.	
);

extern HRESULT InitHvSysTargetPrivate( void );

/* 
����˵��:�����������ں�����ִ�е�(���жϺ���/ʱ���ж���,�ʶ������ں������ɹر�ȫ���жϻ���
	ʹ��ȫ���ж�ʹ�ܱ�־λ.����ͨ������������ڵķ��Ͷ����Լ�����IC��FIFO״̬��ȷ����ǰ�Ƿ�
	Ӧ�򴮿�IC��������.
��������ֵ:
	����S_OK��ʾ�ɹ�.
	����E_FAIL��ʾ�������ݳ����쳣,�統ǰϵͳ���ڶ�û�г�ʼ��. */
extern HRESULT SendDataToUart( void );

/*
����˵��:�����������ں�����ִ�е�(���жϺ���/ʱ���ж���,�ʶ������ں������ɹر�ȫ���жϻ���
	ʹ��ȫ���ж�ʹ�ܱ�־λ.����ͨ����⴮��IC����FIFO״̬��ȷ����ǰ�Ƿ�Ӧ�Ӵ���IC��FIFO��ȡ����.
��������ֵ:
	����S_OK��ʾ�ɹ�.
	����E_FAIL��ʾ�������ݳ����쳣,�統ǰϵͳ���ڶ�û�г�ʼ��. */
extern HRESULT ReadDataFromUart( void );

/*
����˵��:������ʼ��Ӳ��ƽ̨��DSPƬ��������ݼĴ���.
��������ֵ:
	����S_OK��ʾ��ʼ���ɹ�;
	����E_FAIL��ʾϵͳ���쳣(�Ĵ�������ֵ�����жϲ�һ��). */
extern HRESULT InitDeviceConfig( 
	DWORD32 					dwDeviceCfg		//Ƭ���������������ֵ.
);

/*
����˵��:�ú���������ʼ��DM642��EMIFA�ӿ�.
��������ֵ:
	����S_OK��ʾ��ʼ���ɹ�;
	����E_FAIL��ʾ��ʼ��ʧ��(�Ĵ�������ֵ�����жϲ�һ��).
	����E_POINTER��ʾpSysInfoΪINVALID_POINTER; */
extern HRESULT InitSystemEmif( 
	const EMIF_USER_CONFIG		*pEmifConfig	//ָ��EMIF��������.
);

/* 
����˵��:�ú���������ʼ��DSPƬ������I2Cģ��.��ǰ��ƽ̨��ʹ�õ�DSP I2Cģ��˿�ֻ��һ����ΪI2C_PORT0,
	������������ʼ����I2C�˿�.
��������ֵ:
	����S_OK��ʾ��ʼ���ɹ�;
	����E_FAIL��ʾ��ʼ��ʧ��;
	����E_POINTER��ʾpHandleΪINVALID_POINTER; */
extern HRESULT InitTargetI2CBus( 
	I2C_HANDLE					*pHandle		//ָ��I2C�豸���.
);

/*
����˵��:�ú���������ʼ��DSPƬ������GPIO,����ǰƽ̨�ϵ���ΪGPIO��ʹ�õ����GPIO���Ź���ʹ��,
	�ڳ�ʼ���ɹ�֮��,���ж�GPIO�Ĳ�������ʹ�ø�GPIO�豸���.DSP��GPIO��16��,ÿ��IO��Ӧ��16λ
	�е�һλ,�����MASK���������ĳλΪ1���ʾ���õ�ǰIOΪ�ù���.
��������ֵ:
	����S_OK��ʾ��ʼ���ɹ�;
	����S_FALSE��ʾ��ʼ��ʧ��;
	����E_POINTER��ʾpHandleΪINVALID_POINTER; */
extern HRESULT InitTargetGPIO(
	GPIO_HANDLE					*pHandle,		//ָ��GPIO�豸���.
	DWORD32						dwGpioInputMask,//������ʶGPIO��Ϊ�����MASK.
	DWORD32						dwGpioOutputMask,//������ʶGPIO��Ϊ�����MASK.
	DWORD32						dwGpioInOutMask	//������ʶGPIO��Ϊ���������MASK.
);

/* 
����˵��:�ú���������ʼ��DSPƬ��ʱ��,��ǰƽ̨DSP��֧�ֵ�ʱ��Ϊʱ��0,ʱ��1.
	�ڳ�ʼ���ɹ�֮��,���ж��û�ʱ�ӵĲ�������Ҫ��ʱ���豸���.
��������ֵ:
	����S_OK��ʾ��ʼ���ɹ�;
	����E_FAIL��ʾ��ʼ��ʧ��;
	����E_POINTER��ʾpHandle����pTimerConfigΪINVALID_POINTER;
	����E_INVALIDARG��ʾ��������,��ʱ���豸�Ų��Ϸ�����Ӳ���ж�ӳ��Ų��Ϸ�. */
extern HRESULT InitTargetTimer(
	TIMER_HANDLE				*pHandle,	
	const TIMER_USER_CONFIG		*pTimerConfig
);

/*
����˵��:�ú���������ʼ��Ŀ��崮��,������Ϊ�����ߴ�����ʹ��.ֻ����ȷ�ɹ��س�ʼ������֮��,
	�Դ��ڵĲ������п��ܻ�ɹ�.
��������ֵ:
	����S_OK��ʾ��ʼ���ɹ�;
	����S_FALSE��ʾ��ʼ��ʧ��;
	����E_INVALIDARG��ʾ��������. */
extern HRESULT COM_InitPortNormal(
	SYS_COM_PORT				ComPort,		//��ǰ��Ҫ��ʼ���Ĵ��ں�.
	DWORD32 					dwBaudRate		//���ڲ�����.
);

/*
����˵��:�ú���������ʼ��Ŀ��崮��,������ΪModem����ʹ��.ֻ����ȷ�ɹ��س�ʼ������֮��,
	�Դ��ڵĲ������п��ܻ�ɹ�.
��������ֵ:
	����S_OK��ʾ��ʼ���ɹ�;
	����S_FALSE��ʾ��ʼ��ʧ��;
	����E_INVALIDARG��ʾ��������. */
extern HRESULT COM_InitPortModem(
	SYS_COM_PORT				ComPort,		//��ǰ��Ҫ��ʼ���Ĵ��ں�.
	DWORD32 					dwBaudRate		//���ڲ�����.
);

/*
����˵��:�ú���������ʼ��ϵͳ���ڹ���,����Ӳ���汾�ŵĲ�ͬ��ѡ�ò�ͬ�ĳ�ʼ����ʽ.
��������ֵ:
	����S_OK��ʾ��ʼ���ɹ�.
	��E_FAIL��ʾ��ʼ��ʧ��. */
extern HRESULT InitSysUart( void );

/*
����˵��:�ú�������ȡ��Ӳ���汾��ϸ������ֵ. 
��������ֵ:
	����S_OK��ʾȡֵ�ɹ�.
	����E_POINTER��ʾpbConfigValΪINVALID_POINTER. */
extern HRESULT GetHardVerConfig( 
	PBYTE8						pbConfigVal
);

/* 
����˵��:��������hHandel�豸�����ָ���ʱ��.
��������ֵ:
	����S_OK��ʾ�����ɹ�.
	����E_HANDLE��ʾhHandleΪINVALID_HANDLE. */
extern HRESULT StartTimer( 
	TIMER_HANDLE				hHandle			//��ǰ��Ҫ����ʱ�ӵ��豸���.
);

/* 
����˵��:�ú���������hHandle��ָ���ʱ�ӵ�ǰ��ʱ����,�ö�ȡ�û�ʱ�Ӳ������Ƚ���ʱ����ͣ.
��������ֵ:
	����S_OK��ʾ��ȡ�ɹ�.
	����E_POINTER��ʾpdwSecondTimeΪINVALID_POINTER.
	����E_NOTIMPL��ʾ�û�ʱ��û�г�ʼ��. */
extern HRESULT GetSecond(
	PDWORD32					pdwSecondTime	//ָ��ȡ�õ�����.
);

/* 
����˵��:�ú���������hHandle��ָ���ʱ�ӵ�ǰ��ʱ����,�ö�ȡ�û�ʱ�Ӳ������Ƚ���ʱ����ͣ.
��������ֵ:
	����S_OK��ʾ��ȡ�ɹ�.
	����E_POINTER��ʾpdwMSecondTimeΪINVALID_POINTER.
	����E_NOTIMPL��ʾ�û�ʱ��û�г�ʼ��. */
extern HRESULT GetMSecond(
	PDWORD32					pdwMSecondTime	//ָ��ȡ�õ�����.
);

/*
����˵��:�ú�������ȡ��Ŀ��ϵͳӲ���汾�ź�������ڹ���Ϣ.
��������ֵ:
	����S_OK��ʾȡ��Ϣ�ɹ�;
	����E_POINTER��ʾpSysHandleΪINVALID_POINTER; */
extern HRESULT GetSysDevHandle(
	SYS_DEVICE_HANDLE			*pSysHandle		//ָ�������豸�������.
);

/* ����˵��:�ú���Ϊ10NSΪ��λ���ӳ�,�ӳ�ʱ��Ϊ"dwDelayTime * 10ns ".�����޷���ֵ. */
extern void Target_DelayTNS(
	DWORD32 					dwDelayTime		//�ӳ�10NS����.
);
/* ����˵��:�ú���Ϊ΢��Ϊ��λ���ӳ�,�ӳ�ʱ��Ϊ"dwDelayTime * 1us ".�����޷���ֵ. */
extern void Target_DelayUS(
	DWORD32 					dwDelayTime		//�ӳ�US����.
);


/* 
����˵��:�ú���������Ӳ��ƽ̨��DSP��ַ�ռ�ȡ��һ��ַ����,���������Ҫȡ���ݵ�ַ�Ƿ�Ϸ�,
ֻ��dwEmifAddr��ַ�����ݶ��ض���,���Ըõ�ַ�Ķ������Ƿ�Ե�ǰϵͳ������������Ӱ��,��������
�ṩ�κα�����ʩ.����ֱ�ӷ��ض�����ֵ. */
#define GetEmifData( dwEmifAddr )	( *( ( volatile DWORD32 * )( dwEmifAddr ) ) )

/* 
����˵��:�ú���������Ӳ��ƽ̨��DSP��ַ�ռ�һ��ַд������,���������Ҫд�����ݵĵ�ַ�Ƿ�Ϸ�,
ֻ������dwSetDataд�뵽dwEmifAddr��ַ,���Ըõ�ַ��д�����Ƿ�Ե�ǰϵͳ������������Ӱ��,��������
�ṩ�κα�����ʩ.�����޷���ֵ.*/
#define SetEmifData( dwEmifAddr, dwSetData )	*( ( volatile DWORD32 * )( dwEmifAddr ) ) = ( dwSetData )

/*
����˵��:�ú���������I2C�豸(������EEPROM)�ж�ȡһЩ�Ĵ�����ֵ.
��������ֵ:
	����S_OK��ʾ��ȡI2C�豸�����ɹ�.
	����E_POINTER��ʾpbMemDstΪINVALID_POINTER.
	����E_INVALIDARG��ʾ����Ĳ����д�,��ID�Ų���Ҫ��.
	����E_FAIL��ʾӲ���ײ��������.
	����S_FALSE��ʾ��ȡI2C�豸����. */
extern HRESULT I2C_ReadDevReg(
	BYTE8 						bSlaveDevAddr,	//I2C���豸�豸��ַ.
	BYTE8 						bDevSubAddr,	//��ȡI2C�豸�Ĵ����׵�ַ.
	PBYTE8 						pbMemDst,		//ָ�����������.
	BYTE8						bBytesToRead,	//������ȡ�������ֽڳ���.
	PBYTE8 						pbBytesRead		//ָ��ʵ�ʶ�ȡ�������ֽڳ���,��ΪNULL����.
);

/*
����˵��:�ú���������I2C�豸(������EEPROM)�Ĵ���д������.
��������ֵ:
	����S_OK��ʾд��I2C�豸�����ɹ�.
	����E_POINTER��ʾpbMemSrcΪINVALID_POINTER.
	����E_INVALIDARG��ʾ����Ĳ����д�,��ID�Ų���Ҫ��.
	����E_FAIL��ʾӲ���ײ��������.
	����S_FALSE��ʾд��I2C�豸����. */
extern HRESULT I2C_WriteDevReg(
	BYTE8 						bSlaveDevAddr,	//I2C���豸�豸��ַ.
	const PBYTE8				pbMemSrc, 		//ָ����Ҫд����I2C�豸������.
	BYTE8 						bDevSubAddr, 	//д��I2C�豸�Ĵ����׵�ַ.
	BYTE8						bBytesToWrite,	//����д��������ֽڳ���.
	PBYTE8 						pbBytesWrite	//ָ��ʵ��д��������ֽڳ���,��ΪNULL����.
);

/*
����˵��:�ú���������ʼ��ʱ�ӵ��ж����ʱ��Ƶ��.
��������ֵ:
	����S_OK��ʾ�����ɹ�.
	����E_FAIL��ʾ��������ϵͳ���쳣.
	����S_FALSE��ʾ����ʧ��. */
extern HRESULT InitRTCInt(
	RTC_INT_FREQUENCY 			IntFrequency
);
/*------------------------------------------------------------------------------------------------------------------------------------*/

/*
����˵��:�ú���������DSP��EMACģ����ȡ��MAC��ֵַ.
��������ֵ:
	����S_OK��ʾ��ȡ�ɹ�.
	����E_FAIL��ʾ��ȡ�Ĵ���ʧ��.
	����E_INVALIDARG��ʾ�������Ϸ�.
	����E_POINTER��ʾpbDataΪNULL> */
extern HRESULT GetEtherMacAddr( 
	PBYTE8				pbData,
	DWORD32				dwSize,
	PDWORD32			pdwLen
);

extern void FLASH_SetSection( DWORD32 dwExtAddr );
extern FLASH_USER_CONFIG *g_pFlashCfg;

/* ��ʼ��EEPROM */
extern HRESULT EEPROM_Init();

extern HRESULT ReadFlashParam(BYTE8* pbDat, DWORD32 dwLen);
extern HRESULT WriteFlashParam(BYTE8* pbDat, DWORD32 dwLen);

extern HRESULT CheckVideoPort( int iPort, BYTE8 *pbVariable );
extern HRESULT CheckUserConfigDefault( BYTE8 *pbVariable );
extern HRESULT SetUserConfigVariable( void );

/*
����˵��:�ú���������EEPROM�ж�ȡһ�����ݵ�����.
��������ֵ:
	����S_OK��ʾ��ȡEEPROM�����ɹ�.
	����E_POINTER��ʾpbMemDstΪINVALID_POINTER.
	����E_INVALIDARG��ʾ����Ĳ����д�.
	����E_FAIL��ʾӲ���ײ��������.
	����S_FALSE��ʾ��ȡEEPROM����. */
extern HRESULT EEPROM_CryptRead(
	DWORD32						dwReadPos,		//��ȡEEPROM����λ��.
	PBYTE8 						pbMemDst, 		//ָ�����������.
	DWORD32						dwBytesToRead,	//������ȡ�������ֽڳ���.
	PDWORD32					pdwBytesRead		//ʵ�ʶ�ȡ�������ֽڳ���,ΪNULL����.
);
/*
����˵��:�ú���������EEPROM��д��һ��������.
��������ֵ:
	����S_OK��ʾд��EEPROM�����ɹ�.
	����E_POINTER��ʾpbMemSrcΪINVALID_POINTER.
	����E_INVALIDARG��ʾ����Ĳ����д�.
	����E_FAIL��ʾӲ���ײ��������.
	����S_FALSE��ʾд��EEPROM����. */
extern HRESULT EEPROM_CryptWrite(
	const PBYTE8				pbMemSrc, 		//ָ��д�������.
	DWORD32 					dwWritePos, 	//д��EEPROM���׵�ַ.
	DWORD32						dwBytesToWrite,	//����д��������ֽڳ���.
	PDWORD32					pdwBytesWrite	//ʵ��д��������ֽڳ���,ΪNULL����.
);

void SetUart485Trans( int iPort );
void SetUart485Recv( int iPort );


HRESULT PCI_Init( void );

extern BYTE8 GetCpldVersion( void );

extern PBYTE8 GetDMAResultMemoryPtr( void );

extern PBYTE8 GetDMAVideoMemoryPtr( void );

extern PBYTE8 GetDMAReadMemoryPtr( void );
#ifdef __cplusplus
	}
#endif	//#ifdef __cplusplus

#endif //#ifndef _HVDSP_PRIVATE_H
