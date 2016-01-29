/*
 * message.c
 *
 *  Created on: 2009-9-5
 *      Author: Administrator
 */
#include <std.h>
#include <stdio.h>
#include <stdlib.h>
#include <tsk.h>
#include <sem.h>
#include <DSPLink.h>
#include "DmaCopyApi.h"

/**
 * @brief �ص�����
 * @param inbuf ��������
 * @param inlen �������ݵĳ���
 * @param outbuf �������
 * @param outlen ������ݵĳ���
 * @return ��
 */
void MessageProc(
	unsigned char *inbuf,
	unsigned int inlen,
	unsigned char *outbuf,
	unsigned int outlen
)
{
	static int iFlagInitDma = 0;
	if ( 0 == iFlagInitDma )
	{
		iFlagInitDma = 1;
	}

	ProcessDspCmd(inbuf, inlen, outbuf, outlen);
}
