#ifndef _DSPSYS_DEF_H
#define _DSPSYS_DEF_H

#include "swbasetype.h"
#include "swwinerror.h"
#include "swimage.h"
#include "HvDspTime.h"

//#define _DEBUG_HZH
#define _EEPROM_CRC_CHECK
#define _BOOTLOADER_OK

/* �궨��˵��:��Ч�����ָ�볣������. */
#ifndef INVALID_HANDLE
	#define INVALID_HANDLE 				NULL
#endif

#ifndef INVALID_POINTER
	#define INVALID_POINTER 			NULL
#endif



/* �궨��˵��:������Ϊĳ���ʹ�ܿ���,��������ʱʹ��. */
#ifndef FUNC_ENABLE
	#define FUNC_ENABLE					TRUE
#endif

#ifndef FUNC_DISABLE
	#define FUNC_DISABLE				FALSE
#endif


#endif

