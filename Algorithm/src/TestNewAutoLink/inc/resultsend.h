#ifndef _RESULT_SEND_H
#define _RESULT_SEND_H

#include "HvInterface.h"
#include "swObjBase.h"

#include "HvThread.h"

#include "HvUtils.h"

#include "..\Protocol\hv_opt.h"

//��������ʽ
typedef struct _HVIO_Throb {
	unsigned int m_dwLen;
	unsigned short m_wType;
	unsigned short m_wVideoID;
	unsigned int m_dwIp;
	unsigned short m_wPort;
} HVIO_Throb;

//������Ϣ��ʼ���ձ�־���ݰ�
class HVIO_CarInfoBegin
{
public:
	unsigned int m_dwLen;
	unsigned short m_wType;
	unsigned short m_wVideoID;
	unsigned int m_dwCarID;
};

//������Ϣ�������ձ�־���ݰ�
class HVIO_CarInfoEnd
{
public:
	unsigned int m_dwLen;
	unsigned short m_wType;
	unsigned short m_wVideoID;
	unsigned int m_dwCarID;
};

//���������뿪���ݰ�
class HVIO_CarInLeft {
public:
	unsigned int m_dwLen;
	unsigned short m_wType;
	unsigned short m_wVideoID;
	unsigned int m_dwCarID;
	unsigned int m_dwTimeLow;
	unsigned int m_dwTimeHigh;
	unsigned short m_wInLeftFlag;
};

//�������ݰ�
class HVIO_CarPlate {
public:
	unsigned int m_dwLen;
	unsigned short m_wType;
	unsigned short m_wVideoID;
	unsigned int m_dwCarID;
	unsigned int m_dwTimeLow;
	unsigned int m_dwTimeHigh;
	char m_pCarPlate[ 256 ];
};

//���Ƹ�����Ϣ���ݰ�
class HVIO_AppendInfo {
public:
	unsigned int m_dwLen;
	unsigned short m_wType;
	unsigned short m_wVideoID;
	unsigned int m_dwCarID;
	unsigned int m_dwTimeLow;
	unsigned int m_dwTimeHigh;
	char m_pAppendInfo[ 65536 ];
};

//���ƴ�ͼ�����ݰ�
#ifdef _HIGH_DEVICE 
#define BIGIMG_BUFSIZE ( 1024 * 1024 ) 
#else 
#define BIGIMG_BUFSIZE ( 128 * 1024 ) 
#endif

#ifdef _HIGH_DEVICE 
class HVIO_BigImage {
public:
	unsigned int m_dwLen;
	unsigned short m_wType;
	unsigned short m_wVideoID;
	unsigned int m_dwCarID;
	unsigned int m_dwTimeLow;
	unsigned int m_dwTimeHigh;
	unsigned short m_wImageID;
	unsigned short m_wReserved;
	unsigned short m_wPlateWidth;
	unsigned short m_wPlateHeight;
	unsigned short m_wImageWidth;
	unsigned short m_wImageHeight;
	unsigned short m_wImageType;
	unsigned short m_wImageOffset;
	unsigned int m_dwImageSize;
	unsigned char m_pImage[BIGIMG_BUFSIZE];
};
#else
class HVIO_BigImage {
public:
	unsigned int m_dwLen;
	unsigned short m_wType;
	unsigned short m_wVideoID;
	unsigned int m_dwCarID;
	unsigned int m_dwTimeLow;
	unsigned int m_dwTimeHigh;
	unsigned short m_wImageID;
	unsigned short m_wReserved;
	unsigned short m_wPlateWidth;
	unsigned short m_wPlateHeight;
	unsigned short m_wImageWidth;
	unsigned short m_wImageHeight;
	unsigned short m_wImageType;
	unsigned short m_wImageOffset;
	unsigned int m_dwImageSize;

	//��ͼ���ڴ���CFastMemAlloc������ͷ�,ֻ���ڱ���
	//	unsigned char m_pImage[BIGIMG_BUFSIZE];
	unsigned char* m_pImage;
	unsigned int m_nBufLen;
	CFastMemAlloc cBufAlloc;
};
#endif

//����Сͼ�����ݰ�
class HVIO_SmallImage {
public:
	unsigned int m_dwLen;
	unsigned short m_wType;
	unsigned short m_wVideoID;
	unsigned int m_dwCarID;
	unsigned int m_dwTimeLow;
	unsigned int m_dwTimeHigh;
	unsigned short m_wImageWidth;
	unsigned short m_wImageHeight;
	unsigned short m_wImageType;
	unsigned short m_wImageOffset;
	unsigned int m_dwImageSize;
	unsigned char m_pImage[0x10000];
};

//ʵʱ��Ƶ��
class HVIO_Video {
public:
	unsigned int m_dwLen;
	unsigned short m_wType;
	unsigned short m_wVideoID;
	unsigned short m_wImageType;
	unsigned short m_wImageOffset;
	unsigned int m_dwImageSize;
	unsigned char m_pImage[BIGIMG_BUFSIZE];
	unsigned char *m_pImage128;
public:
	HVIO_Video();
};

//Avi���ݿ�
class HVIO_AviBlock {
public:
	unsigned int m_dwLen;
	unsigned short m_wType;
	unsigned short m_wVideoID;
	unsigned int m_dwIndex;					// ���
	unsigned short m_wBlocks;				// �ܿ���
	unsigned short m_wCurBlock;				// ��ǰ������
	unsigned short m_wYear;					// ��
	unsigned short m_wMon;					// ��
	unsigned short m_wDay;					// ��
	unsigned short m_wHour;					// ʱ
	unsigned short m_wMin;					// ��
	unsigned short m_wSec;					// ��
	unsigned int m_dwAviLen;				// ����(����)
	unsigned int m_dwImageSize;
	unsigned char m_pImage[BIGIMG_BUFSIZE];
};

//Ӳ��¼������ͷ
class HVIO_HistoryVideo {
public:
	unsigned int m_dwLen;
	unsigned short m_wType;
	unsigned short m_wVideoID;
	unsigned int m_dwImageLen;
	unsigned int m_dwTimeLow;
	unsigned int m_dwTimeHigh;
};

//��ʱ״̬�ַ������ݰ�
class HVIO_String {
public:
	unsigned int m_dwLen;
	unsigned short m_wType;
	unsigned short m_wVideoID;
	unsigned int m_dwCarID;
	unsigned int m_dwTimeLow;
	unsigned int m_dwTimeHigh;
	char m_pStr[ 16 * 1024 ];
};

//���Զ��������ݰ�
class HVIO_Binary {
public:
	unsigned int m_dwLen;
	unsigned short m_wType;
	unsigned short m_wVideoID;
	unsigned int m_dwCarID;
	unsigned int m_dwTimeLow;
	unsigned int m_dwTimeHigh;
	unsigned int m_dwBinSize;
	unsigned char m_pBin[ 4096 ];
};

/*ͨ�÷��Ͷ���*/
template < class T, int MAX_COUNT >
class G_QUEUE {
private:
	T *ppItem[ MAX_COUNT ];
	int rgReference[ MAX_COUNT ];
	HiVideo::ISemaphore* m_pSemCount;

public:
	G_QUEUE()
	{
		HV_memset(rgReference, 0, sizeof(rgReference));
		HV_memset(ppItem, 0, sizeof(T*) * MAX_COUNT);
		HiVideo::ISemaphore::CreateInstance(&m_pSemCount,1,1);
	};

	~G_QUEUE()
	{
		for (int i = 0; i < MAX_COUNT; i++)
		{
			if (rgReference[i] > 0)
			{
				rgReference[i] = 0;
			}
			if( ppItem[i] != NULL )
			{
				delete ppItem[i];
				ppItem[i] = NULL;
			}
		}
		SAFE_DELETE(m_pSemCount);
	}

	T *GetEmptyCell( int* pTmp )
	{
		if( pTmp != NULL )
			*pTmp = -1;
		int i = 0;
		m_pSemCount->Pend();
		for( i = 0; i < MAX_COUNT; ++i )
		{
			if( rgReference[i] <= 0 )
				break;
		}
		if( i == MAX_COUNT )
		{
			m_pSemCount->Post();
			return NULL;
		}
		else
		{
			ppItem[i] = new T();
			if (ppItem[i] != NULL)
			{
				if( pTmp != NULL )
					*pTmp = i;
				rgReference[i] = 1;
			}
			m_pSemCount->Post();
			return ppItem[i];
		}
	};

	T *GetReference( int index )
	{
		m_pSemCount->Pend();

		if( index < 0 || index >= MAX_COUNT )
		{
			m_pSemCount->Post();
			return NULL;
		}
		if( rgReference[ index ] <= 0 )
		{
			m_pSemCount->Post();
			return NULL;
		}
		rgReference[ index ] ++;
		m_pSemCount->Post();
		return ppItem[index] ;
	};

	void Release( int index )
	{
		m_pSemCount->Pend();
		if( index < 0
			|| index >= MAX_COUNT 
			|| rgReference[ index ] <= 0 )
		{
			m_pSemCount->Post();
			return;
		}
		rgReference[ index ] --;
		if( rgReference[ index ] <= 0 )
		{
			delete ppItem[index];
			ppItem[index] = NULL;
		}
		m_pSemCount->Post();
	};

#ifdef _HIGH_DEVICE 
	int Send(
		HvCore::IHvStream* pStream,
		int index
		)
	{
		HRESULT hr = S_OK;
		m_pSemCount->Pend();
		if ( index < 0
			|| index >= MAX_COUNT
			|| rgReference[ index ] <= 0 
			|| ppItem[index]->m_dwLen <= 0 )
		{
			m_pSemCount->Post();
			return 0;
		}
		else
		{
			m_pSemCount->Post();
			hr = pStream->Write( ppItem[index], ppItem[index]->m_dwLen + 4, NULL );
			Release(index);
			return (SUCCEEDED(hr))?1:-1;
		}
	};
#else
	int Send(
		HvCore::IHvStream* pStream,
		int index
		)
	{
		HRESULT hr = S_OK;
		m_pSemCount->Pend();
		if ( index < 0
			|| index >= MAX_COUNT
			|| rgReference[ index ] <= 0 )
		{
			m_pSemCount->Post();
			return 0;
		}
		else
		{
			m_pSemCount->Post();

			if( ppItem[index]->m_wType != _TYPE_BIG_IMAGE )
			{
				hr = pStream->Write( ppItem[index], ppItem[index]->m_dwLen + 4, NULL );
			}
			else
			{
				BYTE8* pBuf = *(BYTE8**)((BYTE8*)ppItem[index] + ppItem[index]->m_dwLen + 4);
				UINT nLen = *(UINT*)((BYTE8*)ppItem[index] + ppItem[index]->m_dwLen + 8);
				UINT nHeadLen = ppItem[index]->m_dwLen + 4;
				int nOriLen = ppItem[index]->m_dwLen; //����ԭʼ����
				ppItem[index]->m_dwLen += nLen;

				hr = pStream->Write( ppItem[index], nHeadLen, NULL );
				ppItem[index]->m_dwLen = nOriLen;

				if(SUCCEEDED(hr))
				{
					hr = pStream->Write(pBuf,nLen,NULL); 
				}
			}

			Release(index);
			return (SUCCEEDED(hr))?1:-1;
		}
	};
#endif

};

class SENDSTYLE
{
public:
	int m_index;
	unsigned short m_wSendType;
};

//�����������Ͷ���
template < class T, int MAX_COUNT >
class Q_STYPE
{
private:
	T m_rgItem[ MAX_COUNT ];
	int m_iHead;
	int m_iCount;

	HiVideo::ISemaphore* m_pSemCount;

public:
	Q_STYPE()
		: m_iHead( 0 )
		, m_iCount( 0 )
	{
		HiVideo::ISemaphore::CreateInstance(&m_pSemCount,1,1);
	};

	~Q_STYPE()
	{
		SAFE_DELETE(m_pSemCount);
	};

	bool PushBack( const T& item )
	{
		m_pSemCount->Pend();
		if( m_iCount >= MAX_COUNT )
		{
			m_pSemCount->Post();
			return false;
		}

		int iPos = ( m_iHead + m_iCount ) % MAX_COUNT;
		m_rgItem[ iPos ] = item;
		m_iCount += 1;
		m_pSemCount->Post();
		return true;
	};

	bool GetFront( T& item )
	{
		m_pSemCount->Pend();
		if( m_iCount <= 0 )
		{
			m_pSemCount->Post();
			return false;
		}
		item = m_rgItem[m_iHead];
		m_iHead = ( m_iHead + 1 ) % MAX_COUNT;
		m_iCount -= 1;
		m_pSemCount->Post();
		return true;
	};
};


#ifdef _HIGH_DEVICE
	const int HVIO_COUNT = 10;
#else
	const int HVIO_COUNT = 3;
#endif

const int HVIO_INFOBEGIN_COUNT = HVIO_COUNT;
const int HVIO_INFOEND_COUNT = HVIO_COUNT;
const int HVIO_INLEFT_COUNT = HVIO_COUNT;
const int HVIO_PLATE_COUNT = HVIO_COUNT;
const int HVIO_APPENDINFO_COUNT = HVIO_COUNT;
const int HVIO_BIGIMG_COUNT = HVIO_COUNT * 5;
const int HVIO_SMALLIMG_COUNT = HVIO_COUNT * 2;
const int HVIO_VIDEO_COUNT = HVIO_COUNT;
const int HVIO_STR_COUNT = HVIO_COUNT;
const int HVIO_BIN_COUNT = HVIO_COUNT;
const int HVIO_AVI_COUNT = 3;

const int MAX_STREAMID_COUNT = 40;

//��������������󻺳�����
const int MAX_TYPE_COUNT 
	= HVIO_INFOBEGIN_COUNT 
	+ HVIO_INFOEND_COUNT 
	+ HVIO_INLEFT_COUNT 
	+ HVIO_PLATE_COUNT 
	+ HVIO_BIGIMG_COUNT 
	+ HVIO_SMALLIMG_COUNT
	+ HVIO_VIDEO_COUNT
	+ HVIO_STR_COUNT
	+ HVIO_BIN_COUNT
	+ HVIO_AVI_COUNT;

//////////////////////////////////////////////////////////////////////////
//�ӿڶ���
interface IResultSend
{
	//�û��ӿڣ����ͳ�����Ϣ��ʼ���ձ�־
	STDMETHOD(SendInfoBegin)(
		DWORD32 dwVideoID,
		DWORD32 dwCarID
		) = 0;

	//�û��ӿڣ����ͳ�����Ϣ�������ձ�־
	STDMETHOD(SendInfoEnd)(
		DWORD32 dwVideoID,
		DWORD32 dwCarID
		) = 0;

	//�û��ӿڣ����ͳ��������뿪��־
	STDMETHOD(SendInLeftFlag)(
		DWORD32 dwVideoID,
		DWORD32 dwCarID,
		DWORD32 dwTimeLow,
		DWORD32 dwTimeHigh,
		WORD16 wInLeftFlag
		) = 0;

	//�û��ӿڣ����ͳ���
	STDMETHOD(SendCarPlate)(
		DWORD32 dwVideoID,
		DWORD32 dwCarID,
		DWORD32 dwTimeLow,
		DWORD32 dwTimeHigh,
		const char *pPlate
		) = 0;

	//�û��ӿڣ����ͳ��Ƹ�����Ϣ
	STDMETHOD(SendAppendInfo)(
		DWORD32 dwVideoID,
		DWORD32 dwCarID,
		DWORD32 dwTimeLow,
		DWORD32 dwTimeHigh,
		const char *pPlateInfo
		) = 0;

	//�û��ӿڣ����ͳ���YUV��ͼ
	STDMETHOD(SendBigImage)(
		DWORD32 dwVideoID,
		DWORD32 dwCarID,
		DWORD32 dwTimeLow,
		DWORD32 dwTimeHigh,
		WORD16 wImageID,
		WORD16 wReserved,
		WORD16 wPlateWidth,
		WORD16 wPlateHeight,
		WORD16 wImageWidth,
		WORD16 wImageHeight,
		WORD16 wStrideWidth,
		unsigned char *pData,
		DWORD32 dwDataSize, 
		unsigned char* pbOtherInfo = NULL,
		WORD16 wInfoSize = 0
		) = 0;

	//����YUVСͼ
	STDMETHOD(SendSmallImageYUV)(
		DWORD32 dwVideoID,
		DWORD32 dwCarID,
		DWORD32 dwTimeLow,
		DWORD32 dwTimeHigh,
		WORD16 wImageWidth,
		WORD16 wImageHeight,
		WORD16 wStrideWidth,
		unsigned char *pY,
		unsigned char *pCb,
		unsigned char *pCr
		) = 0;

	//���Ͷ�ֵ��Сͼ
	STDMETHOD(SendBinaryImage)(
		DWORD32 dwVideoID,
		DWORD32 dwCarID,
		DWORD32 dwTimeLow,
		DWORD32 dwTimeHigh,
		WORD16 wImageWidth,
		WORD16 wImageHeight,
		WORD16 wStrideWidth,
		unsigned char *pBin
		) = 0;

	//������Ƶ
	STDMETHOD(SendVideoYUV)(
		DWORD32 dwVideoID,
		unsigned char *pY,
		unsigned char *pCb,
		unsigned char *pCr
		) = 0;

	STDMETHOD(SendVideo)(
		DWORD32 dwVideoID,
		BYTE8* pImgData,
		DWORD32 dwImgSize,
		DWORD32* pdwSendCount,
		DWORD32 dwImgTime,
		int nRectCount = 0,
		HV_RECT *pRect = NULL
		) = 0;


	//�û��ӿڣ������ַ���
	STDMETHOD(SendString)(
		DWORD32 dwVideoID,
		WORD16 wStreamID,
		DWORD32 dwTimeLow,
		DWORD32 dwTimeHigh,
		const char *pString
		) = 0;

	//�û��ӿڣ����Ͷ�������
	STDMETHOD(SendBin)(
		DWORD32 dwVideoID,
		WORD16 wStreamID,
		WORD16 wBinSize,
		const unsigned char *pBuffer
		) = 0;
};

interface IResultIO
{
	//�û��ӿڣ����ͳ�����Ϣ��ʼ���ձ�־
	STDMETHOD(SendInfoBegin)(
		const int& index,
		DWORD32 dwFlag = 0
		) = 0;

	//�û��ӿڣ����ͳ�����Ϣ�������ձ�־
	STDMETHOD(SendInfoEnd)(
		const int& index,
		DWORD32 dwFlag = 0
		) = 0;

	//�û��ӿڣ����ͳ��������뿪��־
	STDMETHOD(SendInLeftFlag)(
		const int& index,
		DWORD32 dwFlag = 0
		) = 0;

	//�û��ӿڣ����ͳ���
	STDMETHOD(SendCarPlate)(
		const int& index,
		DWORD32 dwFlag = 0
		) = 0;

	//�û��ӿڣ����ͳ��Ƹ�����Ϣ
	STDMETHOD(SendAppendInfo)(
		const int& index,
		DWORD32 dwFlag = 0
		) = 0;

	//�û��ӿڣ����ͳ���YUV��ͼ
	STDMETHOD(SendBigImage)(
		const int& index,
		DWORD32 dwFlag = 0
		) = 0;

	//����YUVСͼ
	STDMETHOD(SendSmallImageYUV)(
		const int& index,
		DWORD32 dwFlag = 0
		) = 0;

	//���Ͷ�ֵ��Сͼ
	STDMETHOD(SendBinaryImage)(
		const int& index,
		DWORD32 dwFlag = 0
		) = 0;

	//������Ƶ
	STDMETHOD(SendVideoYUV)(
		const int& index,
		DWORD32* pdwSendCount,
		DWORD32 dwFlag = 0
		) = 0;

	//�û��ӿڣ������ַ���
	STDMETHOD(SendString)(
		const int& index,
		DWORD32 dwFlag = 0
		) = 0;

	//�û��ӿڣ����Ͷ�������
	STDMETHOD(SendBin)(
		const int& index,
		DWORD32 dwFlag = 0
		) = 0;

	//���ù�������
	STDMETHOD(SetStreamID)(
		DWORD32 dwIP,
		WORD16 wPort,
		WORD16 wVideoID,
		WORD16 wStreamID
		) = 0;

	//ɾ����������
	STDMETHOD(DeleteStreamID)(
		DWORD32 dwIP,
		WORD16 wPort,
		WORD16 wVideoID,
		WORD16 wStreamID
		) = 0;

	//������й�������
	STDMETHOD(ClearAllStreamID)(
		DWORD32 dwIP,
		WORD16 wPort
		) = 0;

	//���ð�ȫ����ģʽ
	STDMETHOD(SetSafeMode)(
		DWORD32 dwIP,
		WORD16 wPort,
		DWORD32 dwTimeLow,
		DWORD32 dwTimeHigh,
		int index,
		int iPageIndex = -1,
		int iPageSize   = 0
		) = 0;

	//��ѯAVI�ļ�
	STDMETHOD(GetAviByTime)(
		DWORD32 dwIP,
		WORD16 wPort,
		DWORD32 dwTimeLow,
		DWORD32 dwTimeHigh,
		DWORD32 *pdwAviTimeLow,
		DWORD32 *pdwAviTimeHigh,
		DWORD32 *pdwAviLen
		) = 0;

	//��ѯӲ��¼��
	STDMETHOD(GetHistoryVideo)(
		DWORD32 dwIP,
		WORD16 wPort,
		DWORD32 dwTimeLow,
		DWORD32 dwTimeHigh,
		DWORD32 dwTimeSliceS
		) = 0;
	//���ý�����յ���������(ֻ����Υ�½��,ȫ�����)
	STDMETHOD(SetReceivePeccancy)(
		DWORD32 dwIP,
		WORD16 wPort,
		int iOutputOnlyPeccancy = -1
		) = 0;
};

#endif


