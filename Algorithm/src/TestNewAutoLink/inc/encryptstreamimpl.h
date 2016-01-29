#pragma once

#include "HvStream.h"
#include "Fastcrc32.h"

#define HV_ENC_RSA_NORMAL 			0x00000000	//�Զ��巽ʽ
#define HV_ENC_RSA_PKCS					0x00000001		// PKCS#1��׼
#define HV_ENC_BLOCK_TYPE0			0x00000000	//����������(����Ӹ��ӿ�)
#define HV_ENC_BLOCK_TYPE1				0x00000010		//������1
#define HV_ENC_BLOCK_TYPE2			0x00000020	//������2
#define HV_ENC_BLOCK_TYPE3			0x00000030	//������3
#define HV_ENC_BUF_SIZE0     			0x00000000	//�û�ָ�������С
#define HV_ENC_BUF_SIZE1				0x00000100		//��������1
#define HV_ENC_BUF_SIZE2				0x00000200	//��������2
#define HV_ENC_BUF_SIZE3				0x00000300	//��������3
#define HV_ENC_INTERVAL_0				0x00000000	//���ܼ��0(�޼��)
#define HV_ENC_INTERVAL_1				0x00001000		//���ܼ��1
#define HV_ENC_INTERVAL_2				0x00002000	//���ܼ��2
#define HV_ENC_INTERVAL_3				0x00003000	//���ܼ��3

namespace HiVideo
{
	struct EncryptStreamInfo
	{
		BOOL fWrite;
		BOOL fValid;
		BOOL fInitialized;

		UINT nStreamStart;
		UINT nCurPos;
		INT nCurBlockNum;
		INT nInterval;

		UINT nEncryptBufSize;
		UINT nAvailBufSize;

		UINT nBlockSize;
		UINT nBlkOffset;

		UINT nStreamSize;
		UINT nCrc32;
	};

	struct EncryptStreamHead
	{
		UINT  Tag;
		UINT  Flags;
		UINT  BufSize;
		UINT  RsaDatLen;
		UINT	Reserved1;
		UINT	Reserved2;
	};

	struct EncryptStreamTail
	{
		UINT Size;
		UINT Crc;
		UINT Reserved1;
		UINT Reserved2;
		UINT Valid;
	} ;

	class CEncryptStreamImpl : public IEncryptStream
	{
	public:
		CEncryptStreamImpl(void);
		~CEncryptStreamImpl(void);

		HVMETHOD(Initialize)(
			IStream* pIOStream,
			BOOL fWrite,
			const UINT* pKey,
			const RSA_KEY* pRsaKey = NULL,
			UINT nEncryptBufSize = 2048,
			DWORD32 dwFlags = HV_ENC_BUF_SIZE1|HV_ENC_BLOCK_TYPE2|HV_ENC_RSA_PKCS
			);

		HVMETHOD(Initialize)(
			IStream* pIOStream,
			BOOL fWrite,
			const UINT* pKey,
			const RSA_PRIVATE_KEY* pRsaKey = NULL,
			UINT nEncryptBufSize = 2048,
			DWORD32 dwFlags = HV_ENC_BUF_SIZE1|HV_ENC_BLOCK_TYPE2|HV_ENC_RSA_PKCS
			);

		HVMETHOD(Close)(void);

		HVMETHOD(Read)( 
			PVOID pv,
			UINT cb,
			PUINT pcbRead
			);

		HVMETHOD(Write)( 
			LPCVOID pv,
			UINT cb,
			PUINT pcbWritten
			);

		HVMETHOD(Seek)( 
			INT dlibMove,
			STREAM_SEEK ssOrigin,
			PUINT plibNewPosition
			);
			
		HVMETHOD(GetVerifyInfo)(
			DWORD32* pCrc, 
			DWORD32* pSize
			);

		HVMETHOD(SetSize)( 
			UINT libNewSize
			);

		HVMETHOD(CopyTo)( 
			IStream *pstm,
			UINT cb,
			PUINT pcbRead,
			PUINT pcbWritten
			);

		HVMETHOD(Release)()
		{
			delete this;
			return HR_S_OK;
		};

	protected:
		IStream* m_pIOStream;

		BYTE8* m_pbBuf;

		UINT m_rgKey[4];

		CFastCrc32 m_FastCrc;

		EncryptStreamInfo m_Info;

		HVRESULT CheckHead(EncryptStreamHead& head);
		HVRESULT CheckTail(EncryptStreamTail& tail);

		HVRESULT UpdateHead(const EncryptStreamHead& head,vlong& keydat);
		HVRESULT RandMem(BYTE8* pbBuf,UINT nSize);

		HVRESULT Flush(void);
	};
}

