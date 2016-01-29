#ifndef _HV_IO_H
#define _HV_IO_H

#include "swBaseType.h"
#include "..\hvdsp\UserConfig.h"
#include "hvtarget.h"
#include "HvDspTime.h"

void GetSystemID(
	DWORD32 *pdwIDLow,
	DWORD32 *pdwIDHigh
);

int GetSystemName(
	char *pName,
	int iSize
);

int SetSystemName(
	char *pName
);

int GetHvMode(
	WORD16 wVideoID,
	BYTE8 *pbMinSpeed,
	BYTE8 *pbMaxSpeed,
	BYTE8 *pbModeFlag,
	BYTE8 *pbMoveDirection,
	BYTE8 *pbMaxPlateNum,
	BYTE8 *pbProvince
);

int SetHvMode(
	WORD16 wVideoID,
	BYTE8 bMinSpeed,
	BYTE8 bMaxSpeed,
	BYTE8 bModeFlag,
	BYTE8 bMoveDirection,
	BYTE8 bMaxPlateNum,
	BYTE8 bProvince
);

int SetHvIP(
	DWORD32 dwIP,
	DWORD32 dwMask,
	DWORD32 dwGateway
);

DWORD32 GetVersion();

int ResetHv( DWORD32 dwMode );
int GetSysMode( void );
bool IsSysInit( void );

int GetAllVideoName(
	int *piCount,
	WORD16 pwVideoID[],
	char pName[][ 256 ]
);

int GetVideoName(
	WORD16 wVideoID,
	char *pName
);

int SetVideoName(
	WORD16 wVideoID,
	char *pName
);

int GetTcpipAddr( DWORD32 *pdwIP, DWORD32 *pdwMask, DWORD32 *pdwGateway );

//����ϵͳָʾ��
inline DWORD32 SetSysLed( DWORD32 dwLed )
{
	g_dwLed = dwLed;
	return 0;
}

//��ȡϵͳָʾ�����
inline DWORD32 GetSysLed( void )
{
	return g_dwLed;
}

//ÿ��дflash�����ݳ���
#define FLASH_CONTORL_LENGTH		0x8000

//����flash�飬ÿ�ε��ò���64k�Ŀռ䣬dwStartAddr������64k�ı�����
int FlashEraseBlock( DWORD32 dwStartAddr );
//дflash�飬ÿ�ε���д512�ֽڵ����ݣ�dwStartAddr������512�ı�����pWriteData�ĳ��ȱ�����512��
int FlashWriteData( DWORD32 dwStartAddr, BYTE8 *pWriteData );
//��flash�飬ÿ�ε��ö�512�ֽڵ����ݣ�dwStartAddr������512�ı�����pWriteData�ĳ��ȱ�����512��
int FlashReadData( DWORD32 dwStartAddr, BYTE8 *pReadData );


int EepromWriteData( WORD16 wStartAddr, BYTE8 *pWriteData );
int EepromReadData( WORD16 wStartAddr, BYTE8 *pReadData );

//��д��׼Э�����ò���
int GetStdPtlParam( BYTE8 *pucMode, WORD16 *pwLocalPort, WORD16 *pwRemotePort, DWORD32 *pdwRemoteAddr );
int SetStdPtlParam( BYTE8 ucMode, WORD16 wLocalPort, WORD16 wRemotePort, DWORD32 dwRemoteAddr );

#endif
