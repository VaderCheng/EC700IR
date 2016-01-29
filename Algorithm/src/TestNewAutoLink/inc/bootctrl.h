#ifndef _BOOTCTRL_H
#define _BOOTCTRL_H

#include "swbasetype.h"
#include "swWinError.h"

#ifdef __cplusplus
	extern "C" {
#endif	//#ifdef __cplusplus

typedef enum {
	BOOT_DOWNLOAD = 0,
	BOOT_TESTBEGIN = 1,
	BOOT_TESTNEW = 2,
	BOOT_NORMAL = 3,
	BOOT_PRODUCT = 4,
	BOOT_MAX_MODE_NUM = 5
} BootMode;

/* ����˵��:ȡ������ģʽ�汾��,��������0��ʾ������ȷ,����-1��ʾ����EEPROMʧ��. */
extern int GetBootConfigVer( WORD16 *pVersion );
/* ����˵��:��������ģʽ�汾��,��������0��ʾ������ȷ,����-1��ʾ����EEPROMʧ��. */
extern int SetBootConfigVer( WORD16 wVersion );
/* ����˵��:ȡ������������Ϣ,��������0��ʾ������ȷ,����-1��ʾ����EEPROMʧ��. */
extern int GetBootMode( BootMode *bootMode, DWORD32 *ptrBootPrg );
/* ����˵��:��������������Ϣ,��������0��ʾ������ȷ,����-1��ʾ����EEPROMʧ��. */
extern int SetBootMode( BootMode bootMode, DWORD32 dwBootPrg );


#ifdef __cplusplus
	}
#endif	//#ifdef __cplusplus

#endif	//#ifndef _BOOTCTRL_H
