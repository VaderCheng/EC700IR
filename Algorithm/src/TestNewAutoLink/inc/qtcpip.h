#ifndef _QTCPIP_H_
#define _QTCPIP_H_

#include <assert.h>
#include "os.h"

namespace QTCPIP {
	//������ָ�붨��
	struct _COMMON_PACKET;
	typedef void ( *PROCESS_FUN )( struct _COMMON_PACKET *pPacket );
	//
	struct _TCB;
	//ȫ�����ݰ�����
	const int MAX_PACKET_LENGTH = 128 * 13 - 24;
	const WORD CDATA_TYPE_ETHER = 0;
	const WORD CDATA_TYPE_MSG = 1;
	typedef struct _COMMON_PACKET {
		struct _COMMON_PACKET *pFront, *pRear;
		union {
			PROCESS_FUN ProcessFun;
			DWORD resend_timeout;
			BYTE *pCopyPos;
		} u;
		_TCB *tcb_ptr;
		WORD tcp_data_signal;
		WORD tcp_data_attempts;
		WORD data_type;
		WORD data_len;
		WORD data[ MAX_PACKET_LENGTH / 2 ];
	} COMMON_PACKET;
	//

	//��ַ���
	const int ETHER_ADDR_SIZE = 6;
	const int IP_ADDR_SIZE = 4;

	//Ĭ�ϵ�ַ��Ϣ
	extern const DWORD32 DefaultIpAddr;
	extern const DWORD32 DefaultMask;
	extern const DWORD32 DefaultGateway;

	extern const BYTE8 ucDefaultMode;
	extern const WORD16 wDefaultLocalPort;
	extern const WORD16 wDefaultRemotePort;
	extern const DWORD32 dwDefaultRemoteAddr;
	
	extern BYTE8 ucgMode;
	extern WORD16 wgLocalPort;
	extern WORD16 wgRemotePort;
	extern DWORD32 dwgRemoteAddr;
	
	//��ַת��
	BOOL MyGetIpDWord(LPSTR lpszIP, DWORD32& dwIP);
	
	BOOL MyGetIpString(DWORD32 dwIP, LPSTR lpszIP);

	//����MAC��ַ
	extern int SetMacAddr( WORD16* rgMac);

	//��ȡMAC��ַ
	extern int GetMacAddr( WORD16* rgMac);

	//����IP��ַ
	extern int SetTcpipAddr( DWORD32 dwIP, DWORD32 dwMask, DWORD32 dwGateway );
	extern int SetTcpipAddrSecond(DWORD32 CommIPAddr, DWORD32 CommMask);

	//ȡ��IP��ַ
	extern int GetTcpipAddr( DWORD32 *pdwIP, DWORD32 *pdwMask, DWORD32 *pdwGateway );

	//ȡ�ñ�׼Э�����
	extern int GetStdPtlParam( BYTE8 *pucgMode, WORD16 *pwgLocalPort, WORD16 *pwRemotePort, DWORD32 *pdwRemoteAddr );

	//���ñ�׼Э�����
	extern int SetStdPtlParam( BYTE8 ucMode, WORD16 wLocalPort, WORD16 wRemotePort, DWORD32 dwRemoteAddr );
	
	//
	//��̫��������
	const int ETHERNET_PACKET_MAXDATA = 1500;
	const int ETHERNET_PACKET_MINDATA = 46;
	const WORD EHTER_TYPE_ARP = 0x0806;
	const WORD ETHER_TYPE_IP = 0x0800;
	const WORD ETHER_BROADCAST_ADDR[ ETHER_ADDR_SIZE / 2 ] = { 0xffff, 0xffff, 0xffff };
	const WORD ETHER_HEAD_SIZE = 14;
	typedef struct _ETHERNET_PACKET {
		WORD d_mac[ ETHER_ADDR_SIZE / 2 ];
		WORD s_mac[ ETHER_ADDR_SIZE / 2 ];
		WORD next_protocol;
		WORD data[ ETHERNET_PACKET_MAXDATA / 2 ];
	} ETHERNET_PACKET;
	//
	//�������ݽṹ����
	typedef struct _LIST {
		COMMON_PACKET *pHead;
	} LIST;
	//
	//�������ĳ��λ�õ�ǰ���������ݰ�;
	//pList��ʾҪ�������ݵ�����;
	//pPosition��ʾҪ�����λ��;
	//pPacket��ʾҪ����������ݰ�;
	inline void ListInsertPacketFront( LIST *pList, COMMON_PACKET *pPosition, COMMON_PACKET *pPacket ) {
		assert((pList != NULL) && (pPacket != NULL));
		if ( pList->pHead == NULL ) {
			pPacket->pFront = pPacket;
			pPacket->pRear = pPacket;
			pList->pHead = pPacket;
		} else {
			pPosition->pFront->pRear = pPacket;
			pPacket->pFront = pPosition->pFront;
			pPosition->pFront = pPacket;
			pPacket->pRear = pPosition;
			if ( pList->pHead == pPosition ) {
				pList->pHead = pPacket;
			}
		}
	}
	//
	//�������ĳ��λ�õĺ󲿲������ݰ�;
	//pList��ʾҪ�������ݵ�����;
	//pPosition��ʾҪ�����λ��;
	//pPacket��ʾҪ����������ݰ�;
	inline void ListInsertPacketRear( LIST *pList, COMMON_PACKET *pPosition, COMMON_PACKET *pPacket ) {
		assert((pList != NULL) && (pPacket != NULL));
		if ( pList->pHead == NULL ) {
			pPacket->pFront = pPacket;
			pPacket->pRear = pPacket;
			pList->pHead = pPacket;
		} else {
			pPosition->pRear->pFront = pPacket;
			pPacket->pRear = pPosition->pRear;
			pPosition->pRear = pPacket;
			pPacket->pFront = pPosition;
		}
	}
	//ɾ������ָ��λ�õ����ݰ�
	//pList��ʾҪɾ�����ݵ�����;
	//pPosition��ʾҪɾ����λ��;
	inline void ListDeletePacket( LIST *pList, COMMON_PACKET *pPosition ) {
		assert((pList != NULL) && (pPosition != NULL));
		if ( pPosition->pFront == pPosition ) {
			pList->pHead = NULL;
		} else {
			pPosition->pFront->pRear = pPosition->pRear;
			pPosition->pRear->pFront = pPosition->pFront;
			if ( pList->pHead == pPosition ) {
				pList->pHead = pPosition->pRear;
			}
		}
	}
	//����һ�����ݰ�
	COMMON_PACKET *AllocPacket();
	//TCP���ݿ����
	COMMON_PACKET *TCPReclaim( COMMON_PACKET *packet_ptr );
	//����һ�����ݰ�
	void FreePacket( COMMON_PACKET *pPacket );
	
	//��������
	void SendMission( COMMON_PACKET *pPacket );
	//
	//ICMP���뺯��
	void ICMPInput( COMMON_PACKET *packet_ptr );
	//
	//UDP���뺯��
	void UDPInput( COMMON_PACKET *packet_ptr );
	//
	//UDP�㲥��������
	void UDPGetIP( COMMON_PACKET *packet_ptr );
	//
	void UDPMainModule(COMMON_PACKET *packet_ptr);
	//
	//TCP���뺯��
	void TCPMainModule( COMMON_PACKET *packet_ptr );
	//
	//ARP���뺯��
	void ARPInput( COMMON_PACKET *packet_ptr );
	//
	//IP���뺯��
	void IPInput( COMMON_PACKET *packet_ptr );
	//
	//TCP���ƿ鶨��
	typedef struct _TCB {
		DWORD state;
		
		//���������û���Ϣ��
		COMMON_PACKET *user_packet_ptr;

		WORD local_port;
		DWORD local_addr;
		WORD remote_port;
		DWORD remote_addr;

		DWORD sending_sn;
		DWORD remote_ack_sn;
		WORD remote_window;

		DWORD receiving_sn;
		WORD local_window;

		DWORD connect_timeout;
		DWORD listen_timeout;
		DWORD close_timeout;
		DWORD remote_ack_timeout;

		//Ԥ�Ƚ��ն���
		LIST wait_input_queue;
		//���ݽ��ն���
		LIST input_queue;
		//���ݽ���ָ��
		BYTE *input_data_ptr;

		//���ݷ���ָ��
		BYTE *output_data_ptr;

		//�����ط�����
		LIST resend_queue;
	} TCB;
	typedef TCB *TCP_HANDLE;

	//TCPӦ�ó���ӿ�
	
	//TCP��������, �������Ӿ��������NULL��ʾʧ�ܡ�
	TCP_HANDLE TCPConnect( DWORD remote_addr, WORD remote_port );
	//TCP����,�������Ӿ��, ����NULL��ʾ��ʱ��
	//timeout����Ϊ0��ʾ���޵ȴ���
	TCP_HANDLE TCPListen( WORD local_port, int timeout = 0 );
	//��ѯ���ӵ�Զ�˵�ַ�˿ںͱ�����ַ�˿�
	int TCPGetAddrPort( TCP_HANDLE tcp_handle, DWORD *pRemoteAddr, WORD *pRemotePort, DWORD *pLocalAddr, WORD *pLocalPort );
	//TCP��������,����ʵ�ʽ��յ������ݡ����tcp_handle��Ч�򷵻�-1������Զ��Ѿ��ر��򷵻�-2��
	int TCPSend( TCP_HANDLE tcp_handle, BYTE *data_ptr, int data_len );
	//TCP��������,���ؽ��յ����ֽ�.���tcp_handle��Ч�򷵻�-1��
	//����Զ��Ѿ��ر��򷵻�-2,��ʱ����-3��
	//timeout����Ϊ0��ʾ���޵ȴ���
	int TCPReceive( TCP_HANDLE tcp_handle, BYTE *data_ptr, int data_len, int timeout = 0);
	//���������Ƿ��Ѿ�׼����
	bool TCPRecvReady( TCP_HANDLE tcp_handle );
	//TCP�ر����ӡ�
	void TCPClose( TCP_HANDLE tcp_handle );

	
	//UDP���ƿ鶨��
	typedef struct _UCB {
		DWORD state;
		WORD local_port;
		DWORD local_addr;
		//���������û���Ϣ��
		COMMON_PACKET *user_packet_ptr;
		//���ݽ��ն���
		LIST input_queue;
		int buffer_count;
	} UCB;
	typedef UCB *UDP_HANDLE;
	
	
	//UDPӦ�ó���ӿ�
	
	//UDP�󶨱��ض˿�
	UDP_HANDLE UDPConnect(WORD wLocalPort, DWORD dwLocalAddrIndex = 0);
	//UDP��������
	int UDPSend(UDP_HANDLE hUDP, BYTE *pData, int iDataLen, DWORD dwRemoteAddr, WORD wRemotePort);
	//UDP��������
	int UDPReceive(UDP_HANDLE hUDP, BYTE *pBuffer, int iBufferLen, DWORD *pRemoteAddr, WORD *pRemotePort, int iTimeout);
	//UDP�رն˿�
	void UDPClose(UDP_HANDLE hUDP);

	//
	//��ʼ��TCPIPЭ��
	void InitTCPIP();
	//
}

void ShowTCB();
int GetAllConnectedIP(DWORD32* pdwCount, DWORD32* pdwIP, WORD16* pwPort);

inline int SaveRecv( QTCPIP::TCP_HANDLE hHandle, void *pBuf, int iLen, int iTimeout = 2000) {
	int iTemp, iCount;
	QTCPIP::BYTE *pTemp = reinterpret_cast< QTCPIP::BYTE* >( pBuf );

	iCount = 0;
	do {
		if ( ( iTemp = QTCPIP::TCPReceive( hHandle, &pTemp[ iCount ], iLen - iCount, iTimeout ) ) <= 0 )
		{
		 	return iTemp;
		}
		iCount += iTemp;
	} while ( iCount < iLen );
	return iLen;
}

#define SaveSend( hHandle, pBuf, iLen ) QTCPIP::TCPSend( reinterpret_cast< QTCPIP::TCP_HANDLE >( hHandle ), reinterpret_cast< QTCPIP::BYTE* >( pBuf ), iLen )

#endif
