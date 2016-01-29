#ifndef _HV_SYS_ERR_H
#define _HV_SYS_ERR_H

#include "interface.h"

#ifdef __cplusplus
extern "C" {
#endif	//#ifdef __cplusplus

#define SYS_ERR_LIST(MACRO_NAME)			\
	MACRO_NAME(IDICRD, "IC��")				\
   	MACRO_NAME(I2C, "I2C����")			    \
   	MACRO_NAME(FALSH, "FLASH����")			\
    MACRO_NAME(PARAM, "PARAM����")		    \
	MACRO_NAME(MALLOC, "MEM ALLOC")			\
	MACRO_NAME(MFREE, "MEM FREE")			\
	MACRO_NAME(INIT, "��ʼ��" )				\
	MACRO_NAME(RUNNING, "����ʱ")			\
	MACRO_NAME(SONE, "S0")				    \
	MACRO_NAME(STWO, "S1")				    \
	MACRO_NAME(STHREE, "S2")			    \
	MACRO_NAME(MONE, "M0")				    \
	MACRO_NAME(MTWO, "M1")				    \
	MACRO_NAME(MTHREE, "M2")

#define DEFINE_ERR_ENUM(a, b) E_##a,

typedef enum {
	SYS_ERR_LIST(DEFINE_ERR_ENUM)
	ERR_Max_Num
} SysErrType;

/* ����˵��:�ú�������ϵͳ����ַ�,��������Զ˿�(COM����). */
extern int HV_Printf(
	LPCSTR 			format,     // ָ��������ݸ�ʽ.
	...                         // ��������.
);

extern int HVDG_Printf( LPCSTR format, ... );

/* ����˵��:�ú�������ϵͳ����,����˿�ΪCOM����. */
extern int HV_Scanf( 
	LPCSTR 			format,     // ָ���������ݸ�ʽ.
	void 			*pvData,    // ָ��ȡ�õ�����.
	char 			chBreak     // ������ֹ�ַ�.
);

/* ����˵��:�ú����������������ַ��� */
extern int HV_PWD_Scanf( 
            LPCSTR  format, 
            void    *pvData,
            int     nMaxPWDLenght, 
            char    chBreak
);

/* ����˵��:�ú�����������ϵͳ��֪�������Ĵ����¼�,����ϵͳֱ���˳�����,��ֹ���еĲ���. */
extern void HV_Abort(
	LPCSTR			pszString,  // ָ����Ϣ�ַ���.
	int             iVal        // ��������ֵ,����������Ϣ.
);

/* ����˵��:�ú�����������ϵͳ�޷���������ʱ�Ĵ����¼�,����ϵͳ�˳�����,�б�Ҫ����һЩ������. */
extern void HV_Exit(
    LPCSTR          pszString,  // ָ����Ϣ�ַ���.
    int             iExitFlag,  // �˳��ı�־ָʾ.    
    int             iVal        // ��������ֵ,����������Ϣ.
);

/* ����˵��:�ú�����������ϵͳ��֪�������Ĵ����¼�,����ϵͳֱ���˳�����,��ֹ���еĲ���. */
extern void HVDG_Abort(
	LPCSTR			pszString,  // ָ����Ϣ�ַ���.
	int             iVal        // ��������ֵ,����������Ϣ.
);

/* ����˵��:�ú�����������ϵͳ�޷���������ʱ�Ĵ����¼�,����ϵͳ�˳�����,�б�Ҫ����һЩ������. */
extern void HVDG_Exit(
    LPCSTR          pszString,  // ָ����Ϣ�ַ���.
    int             iExitFlag,  // �˳��ı�־ָʾ.    
    int             iVal        // ��������ֵ,����������Ϣ.
);

/* ����˵��:�ú�����������ϵͳ�����¼�,���ָ�������¼�ʱϵͳ��������η�Ӧ���������ͬ��������ͬ. */
extern int HV_Error(
	int             ErrType,    // ��������.
	LPCSTR			pszString,  // ָ����Ϣ�ַ���.
    int             iVal        // ��������ֵ,����������Ϣ.
);

extern BOOL g_fVpProcess[3];
extern BOOL g_fEthernetConnect[3];
extern BOOL g_fInputView;

#ifdef __cplusplus
}
#endif	//#ifdef __cplusplus

#endif	//#ifndef _HV_SYS_ERR_H

