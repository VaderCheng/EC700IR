/*
 * DSPLink.h
 *
 *  Created on: 2009-9-4
 *      Author: Administrator
 */

#ifndef DSPLINK_H_
#define DSPLINK_H_

#include "DSPLinkType.h"

#ifdef _ARM_APP_
#include <cmem.h>
#include "slw.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _DSPLinkBuffer_ {
	void *addr; // virtual address for linux(arm)
	unsigned int phys; // physics address for dsp
	unsigned int len;  // len of the buffer
}DSPLinkBuffer;

/*Driver memeobj*/
typedef struct _tagDSPLinkMem{
	Uint32	addr;
	Uint32	value;
	Uint32	size;
}DSPLinkMem,*DSPLinkMemHandle;

/*DSP Link resgister*/
typedef struct _tagDSPLinkReg{
	Uint32	value;
	Uint32	type;
	Uint32	status;
}DSPLinkReg,*DSPLinkRegHandle;

enum{
	INVALID = 0,
	VALID
};

#ifndef _ARM_APP_
typedef struct _tagDSPLinkObj{
#ifndef _DSP_
	/*ARM side*/
	struct 	semaphore	mutex;
	SEM_Obj				sem_msg;
#else

	#ifndef _NO_DSPBIOS_
	/*DSP side*/
	LCK_Handle			mutex;
	SEM_Handle			sem_msg;
	#endif

#endif /*_DSP_*/
	Uint32				trigerInt;
	DSPLinkRegHandle	armReg;
	DSPLinkRegHandle	dspReg;
}DSPLinkObj,*DSPLinkHandle;
#endif /*_ARM_APP_*/

/* DSPLink CMD */
enum{
	SET_TRI_INT = 0,
	SET_ARM_MSG_REG,
	SET_DSP_MSG_REG,
	GET_REG,
	SET_REG,
	DATA_COPY,
	GET_STATUS
};

/*ARM BOOT DSP Register*/
#define		DSPLINK_PTSTAT			(0x01C41128)
#define		DSPLINK_MDCTL1			(0x01C41A04)
#define		DSPLINK_PTCMD			(0x01C41120)
#define		DSPLINK_MDSTAT1			(0x01C41804)
#define		DSPLINK_MDSTAT2			(0x01C41808)
#define		DSPLINK_MDSTAT3			(0x01C4180C)
#define		DSPLINK_DSPBOOTADDR		(0x01C40008)

/*
 * User need update this section sync to DSP/BIOS mem
 */
#define		TASK_STACK_SIZE	(4096) // DSP�����stack size
#define 	DSP_ENTRY_POINT	(0x8FFFF000) // OUT�ļ����غ����еĵ�ַ

#ifndef _NO_DSPBIOS_
    #ifndef _CAM_APP_
        #define 	DSPLINK_MEM_ARM	(0x11827100)
        #define		DSPLINK_MEM_DSP	(0x11827200)
	#else
        /* CamApp's Arm side*/
        #define 	DSPLINK_MEM_ARM	(0x1182f000)
        #define		DSPLINK_MEM_DSP	(0x1182f800)
	#endif
#else
    /* CamApp's Dsp side*/
	#define 	DSPLINK_MEM_ARM	(0x1182f000)
	#define		DSPLINK_MEM_DSP	(0x1182f800)
#endif

/*ARM <-> DSP interrupt register*/
#define 	DSPINTARM		(0x01c40074)
#define		ARMINTDSP		(0x01C40064)
#define		DSPINTCLR		(0x01c40068)

//DSP�˵�5��Ӳ�жϼĴ�����ַ��ע������TI��H.264���ԭ���޷���Ӧ�жϣ�ֻ�ܲ�����ѯ�жϼĴ����ķ�ʽ��
#define 	DSP5HWIREG		(0x01c40060)


/**
 * @brief DSP�Ļص�����
 * @param inbuf ��������
 * @param inlen �������ݵĳ���
 * @param outbuf �������
 * @param outlen ������ݵĳ���
 * @return ��
 */
typedef void (*MsgDispatch)(unsigned char *inbuf,
	unsigned int inlen,
	unsigned char *outbuf,
	unsigned int outlen);

#ifndef _DSP_
/**
 * @brief ��ʼ��DSP������OUT�ļ���DSP����
 * @param OUT�ļ����ļ���
 * @return ��
 */
void DSPLinkSetup(char *DspFile);

/**
 * @brief ����DSP����
 * @param input �������ݵĽṹ��
 * @param output ��������ݵĽṹ��
 * @param timeout ��ʱʱ��
 * @return ==0 �ɹ�, !=0 ʧ��
 */
int do_process(DSPLinkBuffer *input, DSPLinkBuffer *output, long timeout_ms);
#else
/**
 * @brief ����DSP��ARM��ͨ��,��ARM����DSPʱ�͵��ûص�����
 * @param dispatch �ص�����
 * @return ��
 */
void DSPLinkSetup(MsgDispatch dispatch);
#endif /* _DSP_ */

int CreateDSPBuffer(DSPLinkBuffer *obj, Uint32 len);
void FreeDSPBuffer(DSPLinkBuffer *obj);
void ExitDSPLink(void);

#ifdef __cplusplus
}
#endif

#endif /* DSPLINK_H_ */
