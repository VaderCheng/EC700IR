/*
 * DSPLink.c
 *
 *  Created on: 2009-9-4
 *      Author: Administrator
 */
#include <std.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <csl_intc.h>
#include <csl_cache.h>
#include "CamDsp.h"

extern DSPLinkObj linkObj;

void tskMessageDispatch(MsgDispatch dispatch)
{
	Uint32 msg;
	DSPLinkBuffer *bufs;
	DSPLinkBuffer *input;
	DSPLinkBuffer *output;
	unsigned char *inbuf;
	unsigned char *outbuf;
	unsigned int inlen;
	unsigned int outlen;

	printf("hello world DSPLink...\n");

	// Comment by Shaorg: ����DSPLinkͨ��Э�鱾���ʱԼΪ 3 ~ 4 ms

	while (1)
	{
		while ((*((volatile unsigned int *)DSP5HWIREG))==0);
	  	*((volatile unsigned int *)DSPINTCLR)|=0x1;

		CACHE_wbInvAllL2(CACHE_WAIT);
		msg = linkObj.dspReg->value; //read the arm's message
		
		if (dispatch)
		{
			bufs   = (DSPLinkBuffer *)msg;
			input  = &bufs[0];
			output = &bufs[1];
			inbuf  = (unsigned char *)input->phys;
			outbuf = (unsigned char *)output->phys;
			inlen  = input->len;
			outlen = output->len;

			dispatch(inbuf, inlen, outbuf, outlen);
		}

		CACHE_wbInvAllL2(CACHE_WAIT);
		DSPLinkWrite(msg);
	}
}

/**
 * @brief ��ARM������Ϣ
 * @param value ���͵�ֵ
 * @return ==0 �ɹ�, !=0 ʧ��
 */
int DSPLinkWrite(Uint32 value)
{
	//set message
	linkObj.armReg->value = value;

	//trigger arm
	*(volatile Uint32*)linkObj.trigerInt |= 0x1;

	return 0;
}
