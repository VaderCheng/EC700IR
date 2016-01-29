#ifndef _TARGET_DEFINE_H
#define _TARGET_DEFINE_H

#define UNIQUE_ID_SUPPORT			1

/* 
	����˵��:ϵͳӲ�����ü��������,Ϊ�˱�֤�ײ�ӿ���Ƶ����¼�����, ������Ҫ
		��Ӳ������ʹ����������Ӧ�İ汾�������,�ü���������ǻ���"����+�ӿڰ�"
		���·�����ȷ����,������й�����һ�㶼ֻ���кܴ�ı䶯,���ӿڰ������
		��Ҫ�ڵ�ǰ����Ӳ�������ϻ�������ͬ,����������Ӳ������SYS_TARGET_CONFIG
		�ṹ����,ֻ�дӴ��ں�GPIO��������ͬ,�������򲻱�(��Բ�ͬ�Ĵ�汾).
	Ӳ���汾��ȡ��:ϵͳ�ڳ�ʼ������ʱ���CPLD�ж�ȡ�йص�ǰ�汾����Ϣ,�ٴӽӿ�
		���϶�ȡһЩ�û����õĽӿ�״̬,	�Ӷ��������ɵ�ǰӲ���ľ���汾��,���
		CPLD�ж��õİ汾��Ϊ"0x10",Ҳ��Ϊ1.0��(�����汾),�ӽӿڰ��������ϢΪ
		0x03���ʾ��ǰ�汾��Ϊ1.0��,���Ϊ0x02��Ϊ1.1��,���Ϊ0x01��Ϊ1.2��,
		���Ϊ0x00,��Ϊ1.3��.		
	������ʹ��:�ӿڳ�������ȡ�õ�ǰӲ���İ汾��,��һЩ�ӿں����ľ���ʵ��ʱ
		����ݲ�ͬ�����ö���ͬ,�ϲ��û������������ʵ��ϸ��,�統ǰӲ���ӿ�
		֧�ִ����������,ֻ��Ҫ������Ӧ�Ĵ������������������,�����ǰӲ��
		��֧�ִ�����������򷵻���Ӧ�Ĵ���ֵ. */		
/*------------------------------------------------------------------------------------------------------*/
/* ���ڹ���ö��˵��:����ָʾ�����ⲿ�ӿڹ���ʹ��,���ڲ�ͬ�Ĵ��ڽӿڹ���ʹ��,�ڳ�ʼ����ʹ��ʱ����������ͬ. */
typedef enum {
	UART_NOT_USED 		= 0,			//��ǰ����û��ʹ��,�Ժ�Ըô��ڵ����в���ȫ������RET_VER_NOTSUPPORT����.
	UART_RS232_CTRL		= 1,			//��ǰ������ΪRS232ʹ��,��׼���ߴ���.
	UART_RS422_CTRL		= 1,			//RS422�ǵ������յ�,������ʹ��ʱ���Ե�ǰһ��Ĵ����շ�����.
	UART_RS485_CTRL		= 2,			//RS485�빤ͨѶʱ,�Ƕ෢���յ�,ÿ��ͨѶʱ����һ�����豸,��Ҫ�������߿��ƻ���.
	UART_MODEM_CTRL		= 3				//ȫMODEMʹ�÷�ʽ,��ʼ���ǲ�һ����,��ȫӲ���������������,����������RS232һ��.
} UART_COMM_TYPE;

/* ö������:ϵͳƽ̨DSP��֧�ֶ�ʱ���궨��. */
typedef enum {
	DSP_TIMER_DEV0 		= 0,
	DSP_TIMER_DEV1		= 1,
	DSP_TIMER_NUM		= 2
} DSP_TIMER_DEV;

#define DSP_IRQ_INT_START		4		//DSPӲ���ж�ӳ����ʼ�жϺ�.
#define DSP_IRQ_INT_CNT			15		//DSPӲ���жϺ������.

/* ö��˵��:��ö��ΪRTCʱ���ж������ʱ������,��Ϊ1.0�汾���Ź��Ĺ���ʱ��Ϊ��ʱ��,���������ʱ��Ƶ��Ϊ32K,
RTC�ж����ʱ��Ƶ�ʳ�ʼ������������Bootloader�ʼ����ʼ��. */
typedef enum {
	RTC_INT_1HZ 		= 0,
	RTC_INT_4KHZ		= 1,
	RTC_INT_8KHZ		= 2,
	RTC_INT_32KHZ		= 3
} RTC_INT_FREQUENCY;

/* ���ݽṹ˵��:�����ݽṹ��������Ŀ��ϵͳ�û�������Ϣ. */
typedef struct _SYS_GLOBAL_CONFIG {
    BOOL                fEmifInit;		//�Ƿ���Ҫ���û�����ȥ��ʼ��Ӳ��ƽ̨��EMIF�ӿ�.
    BOOL                fGpioInit;		//�Ƿ���Ҫ�û���ʼ��GPIOƬ������.
    BOOL                fI2CInit;		//�Ƿ���Ҫ�û���ʼ��I2CƬ������.
	BOOL				fTimerEnable;	//ʹ���û���ʱ��,�����ļ�Ĭ���û���ʱ��Ϊ��ʱ��2.
} SYS_GLOBAL_CONFIG;

/* ���ݽṹ˵��:�����ݽṹ��������EMIF�й�����,������EMIFA�ӿ�����(�����ݷ���ʱ��ı�Ե�й�����),��Ϊ��Ҫ����
SDRAMоƬ���йؽӿ�����(��SDRAM�ڴ�ҳ,��дʱ������).����ϵͳʹ�õ�ԭ��,һ�������,EMIFA�ӿ����û���Bootloader��
ʹ��,����������ʽ��������ʱ�����ʼ��(Bootloader�Ǵ�EMIFA�ӿ�����Flash�ж�ȡ����������SDRAM,���ʱ�û�����
��SDRAM������,�йس�ʼ����������SDRAM�н���). */
typedef struct _EMIF_USER_CONFIG {
	DWORD32				dwGblCtl;		//EMIFA�ӿ��������ԼĴ�������.
	DWORD32				dwCeCtl0;		//EMFIA CE0���ݷ��ʿ��ƼĴ�������.
	DWORD32				dwCeCtl1;		//EMFIA CE1���ݷ��ʿ��ƼĴ�������.
	DWORD32				dwCeCtl2;		//EMFIA CE2���ݷ��ʿ��ƼĴ�������.
	DWORD32				dwCeCtl3;		//EMFIA CE3���ݷ��ʿ��ƼĴ�������.
	DWORD32				dwSdCtl;		//EMIFA SDRAM������ƼĴ�������.
	DWORD32				dwSdTim;		//EMIFA SDRAM ʱ�����üĴ�������.
	DWORD32				dwSdExt;		//EMIFA SDRAM��չ�Ĵ�������.
	DWORD32				dwCeSec0;		//EMIFA CE0���ƼĴ���2����.
	DWORD32				dwCeSec1;		//EMIFA CE1���ƼĴ���2����.
	DWORD32				dwCeSec2;		//EMIFA CE2���ƼĴ���2����.
	DWORD32				dwCeSec3;		//EMIFA CE3���ƼĴ���2����.
} EMIF_USER_CONFIG;

/* ���ݽṹ˵��:�����ݽṹ���������û���ʱ���й�����,��ΪDSP��ʱ����һ������ȫʹ��CPU��Ƶ���õ���ʱ����Ϊ���ʱ��,
Ҳ��ʹ���ⲿ����ʱ����Ϊ��ʱ������ʱ��,���Ի��ж�ʱ����С���ȶ�ʱʱ�䳤��. */
typedef struct _TIMER_USER_CONFIG {	
	DWORD32 			dwTimerCtl;		//��ʱ�����ƼĴ�������.
	DWORD32				dwTimerPrd;		//��ʱ����Сʱ�侫��ʱʹ�ù���ʱ�ӵ�������.
	DWORD32				dwTimerCnt;		//��ʱ����ǰ����ʱ�����ڼ�����.	
	DSP_TIMER_DEV		UserTimerDev;	//�û���ʱ��������.
	DWORD32				dwTimerIRQ;		//�û���ʱ���жϺ����ж�ID��.
	DWORD32				dwMilliseconds;	//��ʱ����ǰ��С����ʱ��(��λΪms).
} TIMER_USER_CONFIG;

/* ���ݽṹ˵��:�����ݽṹ�������ô��ڵ��й�����,��ͬ��Ӳ���汾��Ӧ�ڲ�ͬ�Ĵ���ʹ�����. */
typedef struct _UART_USER_CONFIG {
	UART_COMM_TYPE		UartCommType;		//���ڽӿ����Ͷ���.
	DWORD32				dwUartBaudRate;		//���ڵĲ�����.
	WORD16				wUartIcBuffLen;		//����Ӳ���շ���������С.
	WORD16				wFifoBuffLen;		//��������շ���������С.
} UART_USER_CONFIG;

/* ���ݽṹ˵��:�����ݽṹ��������I2C������й�����,���豸��ַ,EEPROM����. */
typedef struct _I2C_USER_CONFIG {
	RTC_INT_FREQUENCY	RTCIntFrequecy;		//RTC�ж����ʱ��Ƶ��.
	DWORD32				dwEEPROMCapability;	//EEPROM������.
	DWORD32				dwPageSize;			//EEPROM��ҳ���ֽڴ�С.
	BYTE8				bEEPROMDeviceAddr;	//EEPROM���豸��ַ,��I2C���߶���.
	BYTE8				bRTCDeviceAddr;		//RTC���豸��ַ,��I2C���߶���.
} I2C_USER_CONFIG;

/* ���ݽṹ˵��:�����ݽṹ��������FLASH�й�����,���ڿ���FLASH����. */
typedef struct _FLASH_USER_CONFIG {
	DWORD32				dwFlashCapability;	//FLASH���������.
	DWORD32				dwFlashStartAddr;	//FLASH���ʵ���ʼ��ַ.
	BOOL				fFlashProtected;	//FLASH�Ƿ���Ҫ���ܱ���.	
	DWORD32				dwFlashProtectedPos;//FLASH��������ʼ��ַ.
	DWORD32				dwFlashProtectedLen;//FLASH��������ʼ��ַ.
	DWORD32				dwFlashCacheSize;	//FLASH��CACHE������������С.
	DWORD32				dwFlashSectionSize;	//FLASH��Section��С;
} FLASH_USER_CONFIG;

/* ���ݽṹ˵��:�����ݽṹ����˵��Ӳ��ƽ̨��GPIO����,����ֻ�涨��ͬ���͵�GPIO,����һЩ�����GPIO���������
Ӳ��ƽ̨�����������. */
typedef struct _GPIO_USER_CONFIG {
	DWORD32				dwWatchDogIO;		//���ҽ���ֻ��һ��GPIO�����ⲿ���Ź�WDI�ź�.
	DWORD32				dwSpringInput;		//��ǰ���ֻ��һ��GPIO������������.
	DWORD32				dwSpringOutput;		//��ǰ���ֻ��һ��GPIO��Ϊ�������.
	DWORD32				dwIdSigIO;			//��ΪΨһIDоƬ����IO,��IO�Զ���ΪInput/Output������.
	DWORD32				dwWorkLEDIO;		//��ǰ���ֻ��һ��GPIO��������ָʾ����ʹ��.
	DWORD32				dwLedIO;			//��LEDһ�������,��ʱ��LED����Ϊһ��,ֻ�в����������ڵ�GPIO�Ż���Ч.
	DWORD32				dwVerInputIO;		//Ӳ���汾�ŵ�С�汾������GPIO.
	DWORD32				dwDebugOutputIO;	//��GPIO��ΪDEBUG����ķ���һ�����(������LED��),ֻ�в���������ڵ�GPIO�Ż���Ч,����ʱʹ��.
} GPIO_USER_CONFIG;

/* ���ݽṹ˵��:�����ݽṹ����ȫ�ֶ���ӿڲ�����ʹ�õ�Ӳ������. */
typedef struct _SYS_TARGET_CONFIG {
	DWORD32				dwHardwareVer;		//Ӳ���汾��.
	DWORD32				dwDspDevCfg;		//DSP��Ƭ���豸����.
	SYS_GLOBAL_CONFIG	SysGlobalCfg;		//ȫ��ϵͳ����.
	EMIF_USER_CONFIG	EmifUserCfg;		//EMFIA�ӿ�����.
	FLASH_USER_CONFIG	FlashUserCfg;		//FLASH��������.
	TIMER_USER_CONFIG	TimerUserCfg;		//�û���ʱ������.
	I2C_USER_CONFIG		I2CUserCfg;			//I2C������������.
	GPIO_USER_CONFIG	GPIOUserCfg;		//GPIO�û�����.
	UART_USER_CONFIG	MainUserCfg;		//����������.	
	UART_USER_CONFIG	rgSlaveUserCfg[3];	//�Ӵ�������.
} SYS_TARGET_CONFIG;


/* ����˵��:�ú����������õ�ǰӲ���汾��,����ȡ�õ�ǰ���е�����������. */
extern HRESULT				SetTargetHardVer( 
	DWORD32 				dwHardVer				//ƽ̨Ӳ���汾��.
);

/* ����˵��:���º�����Ϊȡ�ø����������õ�����ָ��,�������ĺô��Ǳ����һ�����ݸ���. */
extern DWORD32 				*GetDspDevCfgPtr( void );
extern SYS_GLOBAL_CONFIG	*GetSysGlbCfgPtr( void );
extern EMIF_USER_CONFIG		*GetEmifCfgPtr( void );
extern FLASH_USER_CONFIG	*GetFlashCfgPtr( void );
extern TIMER_USER_CONFIG	*GetTimerCfgPtr( void );
extern I2C_USER_CONFIG		*GetI2CCfgPtr( void );
extern GPIO_USER_CONFIG		*GetGpioCfgPtr( void );
extern UART_USER_CONFIG		*GetMainUartCfgPtr( void );
extern UART_USER_CONFIG		*GetSlaveUartCfgPtr( void );

extern BYTE8 VdADDeviceAddr[3];

#endif /* #ifndef _TARGET_DEFINE_H */


