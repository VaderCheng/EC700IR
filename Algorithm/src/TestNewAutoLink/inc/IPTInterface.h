#ifndef  _INCLUDE_INTERFACE_H_
#define _INCLUDE_INTERFACE_H_

#include "swbaseType.h"

//���ڲ�������ӿ�
class IComm {
public:
	virtual ~IComm() {};
	virtual HRESULT Send( const PBYTE8 pbData, DWORD32 dwDataSize ) = 0;
	virtual HRESULT Recv( PBYTE8 pbBuf, DWORD32 dwBufSize, PDWORD32 pdwRecvLen ) = 0;
};

//���
typedef enum
{
	CMD_RESET					= 0x10,		//��λ
	CMD_READ_RESETCOUNT			= 0x11,		//����λ������
	CMD_CLEAR_RESETCOUNT		= 0x12,		//��λ����������
	CMD_SET_TIME				= 0x13,		//����ϵͳʱ��
	CMD_READ_TIME				= 0x14,		//��ϵͳʱ��
	CMD_RESET_DEFAULT			= 0x15,		//�ָ�����Ĭ��
	CMD_READ_WORK_PARAM			= 0x16,		//���豸��������
	CMD_WRITE_WORK_PARAM		= 0x17,		//д�豸��������
	CMD_WRITE_COMM				= 0x20,
	CMD_READ_COMM				= 0x21,
	CMD_SET_COMM_PARAM			= 0x22,
	CMD_READ_COMM_PARAM			= 0x23,
	CMD_SET_DEVICE				= 0x24,
	CMD_READ_DEVICE				= 0x25,
	CMD_READ_DEVICE_EVENT		= 0x1D,		//���ⲿ�豸�¼�
	CMD_READ_DEVICE_EVENT_EX = 0x50,        //�°汾�Ķ��ⲿ�豸�¼��ӿ�
	CMD_SET_IO_OUT_SIGNAL		= 0x29,
	CMD_READ_IO_OUT_SIGNAL		= 0x2A,
	CMD_SET_IO_SHIELD			= 0x2B,
	CMD_READ_IO_SHIELD			= 0x2C,
	CMD_READ_IO_LEVEL			= 0x1C,		//��I/O�����
	CMD_OUTPUT_PULSE			= 0x1B,		//���һ������
	CMD_ENTER_UPDATE_STATE      = 0x38,		//��������״̬
	CMD_UPLOAD_UPDATE_DATA      = 0x39,		//�ϴ���������
	CMD_DOWNLOAD_UPDATE_DATA    = 0x3A,		//������������
	CMD_EXIT_UPDATE_STATE       = 0x3B,		//�˳�����״̬
	CMD_READ_SOFTWARE_VERSION   = 0x3C,		//��ת�����汾��Ϣ
	CMD_SET_IO_OUT_DELAY		= 0x40,
	CMD_READ_IO_OUT_DELAY		= 0x41,
	CMD_SET_IO_OUT_INIT_LEVEL   = 0x42,
	CMD_READ_IO_OUT_INIT_LEVEL  = 0x43,
	CMD_READ_ALL_IO_LEVEL           = 0x51       //��ȡ��ǰ����IO����˿ڵĵ�ƽ״̬
} CMD_TYPE;

//IPT���Ƴ���ӿ�
class IIPTControl{
public:
	virtual ~IIPTControl() {};
	virtual HRESULT SendCommand( const BYTE8 bCmdId, PBYTE8 pbData, DWORD32 dwSize ) = 0;
	virtual HRESULT RecvData( PBYTE8 pbCmdId, PBYTE8 pbData, PDWORD32 pdwLen, DWORD32 dwRelayMs ) = 0;
};

//IPT��������ӿ�
class IIPTDevice{
public:
	virtual ~IIPTDevice() {};
	virtual HRESULT ReadEvent( BYTE8* pbEventId, BYTE8* pbType,PBYTE8 pbEventDeviceType, DWORD32* pdwTime, PBYTE8 pbData, PDWORD32 pdwSize ) = 0;
	virtual HRESULT ReadEventEx( 
		BYTE8* pbInData, DWORD32 dwInLen,
		BYTE8* pbEventId, BYTE8* pbType,PBYTE8 pbEventDeviceType, DWORD32* pdwTime, PBYTE8 pbData, PDWORD32 pdwSize ) = 0;
	virtual HRESULT SyncTime( DWORD32 dwTime ) = 0;
	virtual HRESULT SendSignal(int nIoID, int nLevel, int nSignalWidth) = 0;
};


// ʵ��ʵ��
HRESULT CreateIComm(IComm** ppIComm, const char* pszCom);
HRESULT CreateIIPTControl(IIPTControl** ppIIPTControl, IComm * pIComm);
HRESULT CreateIPTDevice(IIPTDevice** ppIPTDevice, IIPTControl *pIControl);

#endif
