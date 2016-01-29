/* 
    ���ĵ�ΪEDC���ú궨��ͽӿ�����,EDC����˼�弴Ϊ�ⲿ�豸����,�ڸ�IOM�豸������������������Ϊ������
Э��DSP��Ƶ�˿����ⲿ��Ƶ�����оƬ.
    ���л���2004��10��15��10:24�����޸�.
*/
#ifndef _EDC_H
#define _EDC_H

#include <std.h>                                 
#ifdef __cplusplus
extern "C" {
#endif


/* EDC��֧�ֵĿ������. */
/*------------------------------------------------------------------------------------------------------------------------------------*/
#define EDC_CONFIG          0x00000001       //config��������.
#define EDC_RESET           0x00000002       //reset��������.
#define EDC_START           0x00000003       //start��������.
#define EDC_STOP            0x00000004       //stop��������.
#define EDC_GET_CONFIG      0x00000005       //getconfig��������.
#define EDC_GET_STATUS      0x00000006       //getstatus��������.
#define EDC_SET_BRIGHT		0x00000007       //setbright��������.
#define EDC_GET_BRIGHT		0x00000008       //getbright��������.
#define EDC_SET_SATURATION	0x00000009       //setsaturation��������.
#define EDC_GET_SATURATION	0x0000000A       //getsaturation��������.
#define EDC_SET_USATURATION	0x00000019       //setsaturation��������.
#define EDC_GET_USATURATION	0x0000001A       //getsaturation��������.
#define EDC_SET_VSATURATION	0x00000039       //setsaturation��������.
#define EDC_GET_VSATURATION	0x0000003A       //getsaturation��������.
#define EDC_SET_CONTRAST	0x0000000B       //setcontrast��������.
#define EDC_GET_CONTRAST	0x0000000C       //getcontrast��������.
#define EDC_SET_HUE			0x0000000D       //sethue��������.
#define EDC_GET_HUE			0x0000000E       //gethue��������.
#define EDC_SET_BRIGHTS		0x0000000F       //������ز���ֵ.
#define EDC_CONFIG_DIR		0x00000010       //ֱ������ADоƬ,���տ���ģʽ.

#define EDC_USER			0x10000000
/*------------------------------------------------------------------------------------------------------------------------------------*/


/* EDC��֧�ֵĺ�����. */
/*------------------------------------------------------------------------------------------------------------------------------------*/
#define EDC_SUCCESS         0
#define EDC_FAILED          -1

typedef void*  EDC_Handle;

typedef struct EDC_Fxns {
    EDC_Handle  ( *open )( String name, Arg optArg );
    Int ( *close )( EDC_Handle devHandle );
    Int ( *ctrl )( EDC_Handle devHandle, Uns portNum, Uns cmd, Arg arg );
} EDC_Fxns;
/*------------------------------------------------------------------------------------------------------------------------------------*/




#ifdef __cplusplus
}
#endif
#endif
