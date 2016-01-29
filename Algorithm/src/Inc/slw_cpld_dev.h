

#ifndef __SLW_CPLD_DEV__
#define __SLW_CPLD_DEV__

#ifdef __cplusplus
extern "C" {
#endif

#include "slw_gpio.h"
#include "slw_spi.h"


#define GPIO_WATCHDOG  5

enum {
  CPLD_LAN2_LED = 0,
  CPLD_STAT_LED,
  CPLD_WORK_LED,
  CPLD_LAN1_LED,
  CPLD_HARD_LED,
  CPLD_BUZZER,
  CPLD_MainBoard_LED1,
  CPLD_MainBoard_LED2,
  CPLD_RELAY,
  CPLD_USB_POWER,
  CPLD_MAX_DEV
};

#define CPLD_MAX_LED CPLD_RELAY

// CPLD�豸
typedef struct _cpld_dev
{
	Uint8 index;
	Uint8 addr; // ��ַ
	Uint8 bit;  // λ��
} cpld_dev;

// ����˵��:�ú���������WDT���ƹ���״̬�и�����
// �������β�
// �����޷���ֵ
void ResWDT(void);

// �ú���������ȡCPLD�İ汾��(0x20)
void get_cpld_version(Uint8* version);

// ����˵��:����CPLD�豸�Ŀ���
// �����β�: devΪ�豸���
//           on_off 0Ϊ��,��0Ϊ��
// �����޷���ֵ
void set_cpld_dev(Uint8 dev, Uint8 on_off);

// ����˵��:����CPLD��LED�豸��˸,��Ϊ������øú���֮��Ƽ���
// �����β�:devΪ�豸���
// �����޷���ֵ
void cpld_led_blink(Uint8 dev);

#ifdef __cplusplus
}
#endif

#endif // __SLW_CPLD_DEV__
