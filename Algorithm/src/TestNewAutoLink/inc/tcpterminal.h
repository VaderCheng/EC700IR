#ifndef _TCPTERMINAL_H_
#define _TCPTERMINAL_H_

//�ն˷������
void TcpTerminalProcess();

//���ն�����
int ReadTcpTerminal( void *pRecvBuf, int iRecvLen );

//д�ն�����
int WriteTcpTerminal( const void *pSendBuf, int iSendLen );

//д�ն��ַ���
int WriteTcpTerminalString( const char *pSendBuf );

#endif
