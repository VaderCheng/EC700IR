/*
 * message.c
 *
 *  Created on: 2009-9-5
 *      Author: Administrator
 */
#include <std.h>
#include <stdio.h>
#include <stdlib.h>

extern void ProcessDspCmd(
	unsigned char *pbInbuf, 
	unsigned int nInlen, 
	unsigned char *pbOutbuf, 
	unsigned int nOutlen
);

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
	ProcessDspCmd(inbuf, inlen, outbuf, outlen);
}
