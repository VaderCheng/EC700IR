#ifndef _RESULT_SEND_H
#define _RESULT_SEND_H

#include "hvutils.h"
#include "hv_opt.h"
#include "hvthread.h"
#include "hvinterface.h"
using namespace HiVideo;

#define BIGIMG_BUFSIZE ( 1024 * 1024 )  /**< 车牌大图数据包缓存区大小 */
const int MAX_PLATE_STRING_SIZE = 32 * 1024;  /**< 结果字符串最大长度 */
const int MAX_IMAGE_BUFFER_SIZE = 2 * 1024 * 1024;  /**< 结果图片发送最大缓冲区长度 */
const int HVIO_COUNT = 2;  /**< 结果发送队列长度 */

const int HVIO_INFOBEGIN_COUNT = HVIO_COUNT;
const int HVIO_INFOEND_COUNT = HVIO_COUNT;
const int HVIO_INLEFT_COUNT = HVIO_COUNT;
const int HVIO_PLATE_COUNT = HVIO_COUNT;
const int HVIO_BIGIMG_COUNT = HVIO_COUNT * 5;
const int HVIO_SMALLIMG_COUNT = HVIO_COUNT * 2;
const int HVIO_VIDEO_COUNT = HVIO_COUNT;
const int HVIO_STR_COUNT = HVIO_COUNT;

//---old
//const int HVIO_APPENDINFO_COUNT = HVIO_COUNT;
//const int HVIO_BIN_COUNT = HVIO_COUNT;
//const int HVIO_AVI_COUNT = 3;
//---new
const int HVIO_APPENDINFO_COUNT = 0;
const int HVIO_BIN_COUNT = 0;
const int HVIO_AVI_COUNT = 0;
//---

const int MAX_STREAMID_COUNT = 40;  //todo: 应该是要按照流的种类来定，这里需要考虑。

//发送数据类型最大缓冲数量
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

//心跳包格式
typedef struct _HVIO_Throb {
	unsigned int m_dwLen;
	unsigned short m_wType;
	unsigned short m_wVideoID;
	unsigned int m_dwIp;
	unsigned short m_wPort;
} HVIO_Throb;

//车辆信息开始接收标志数据包
class HVIO_CarInfoBegin
{
public:
	unsigned int m_dwLen;
	unsigned short m_wType;
	unsigned short m_wVideoID;
	unsigned int m_dwCarID;
};

//车辆信息结束接收标志数据包
class HVIO_CarInfoEnd
{
public:
	unsigned int m_dwLen;
	unsigned short m_wType;
	unsigned short m_wVideoID;
	unsigned int m_dwCarID;
};

//车辆到达离开数据包
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

//车牌数据包
class HVIO_CarPlate {
public:
	unsigned int m_dwLen;
	unsigned short m_wType;
	unsigned short m_wVideoID;
	unsigned int m_dwCarID;
	unsigned int m_dwTimeLow;
	unsigned int m_dwTimeHigh;
	char m_pCarPlate[MAX_PLATE_STRING_SIZE];
};

//车牌大图像数据包
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

//车牌小图像数据包
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

//车牌附加信息数据包
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

//实时视频流
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
};

//Avi数据块
class HVIO_AviBlock {
public:
	unsigned int m_dwLen;
	unsigned short m_wType;
	unsigned short m_wVideoID;
	unsigned int m_dwIndex;					// 序号
	unsigned short m_wBlocks;				    // 总块数
	unsigned short m_wCurBlock;				// 当前块索引
	unsigned short m_wYear;					// 年
	unsigned short m_wMon;					// 月
	unsigned short m_wDay;					// 日
	unsigned short m_wHour;					// 时
	unsigned short m_wMin;					// 分
	unsigned short m_wSec;					// 秒
	unsigned int m_dwAviLen;				    // 长度(分钟)
	unsigned int m_dwImageSize;
	unsigned char m_pImage[BIGIMG_BUFSIZE];
};

//硬盘录像数据头
class HVIO_HistoryVideo {
public:
	unsigned int m_dwLen;
	unsigned short m_wType;
	unsigned short m_wVideoID;
	unsigned int m_dwImageLen;
	unsigned int m_dwTimeLow;
	unsigned int m_dwTimeHigh;
};

//即时状态字符串数据包
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

//调试二进制数据包
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

/*通用发送队列*/
template < class T, int MAX_COUNT >
class G_QUEUE {
private:
	T *ppItem[ MAX_COUNT ];
	int rgReference[ MAX_COUNT ];
	HV_SEM_HANDLE m_hSemCount;
	char Buf[32];

public:
	G_QUEUE()
	{
		HV_memset(rgReference, 0, sizeof(rgReference));
		HV_memset(ppItem, 0, sizeof(T*) * MAX_COUNT);
        CreateSemaphore(&m_hSemCount, 1, 1);
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
		DestroySemaphore(&m_hSemCount);
	}

	T *GetEmptyCell( int* pTmp )
	{
		if( pTmp != NULL ) *pTmp = -1;
		int i = 0;
		SemPend(&m_hSemCount);
		for( i = 0; i < MAX_COUNT; ++i )
		{
			if( rgReference[i] <= 0 ) break;
		}
		if( i == MAX_COUNT )
		{
			SemPost(&m_hSemCount);
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
			SemPost(&m_hSemCount);
			return ppItem[i];
		}
	};

	T *GetReference( int index )
	{
		SemPend(&m_hSemCount);

		if( index < 0 || index >= MAX_COUNT )
		{
			SemPost(&m_hSemCount);
			return NULL;
		}
		if( rgReference[ index ] <= 0 )
		{
			SemPost(&m_hSemCount);
			return NULL;
		}
		rgReference[ index ] ++;
		SemPost(&m_hSemCount);
		return ppItem[index] ;
	};

	void Release( int index )
	{
		SemPend(&m_hSemCount);

		if( index < 0
			|| index >= MAX_COUNT
			|| rgReference[ index ] <= 0 )
		{
			SemPost(&m_hSemCount);
			return;
		}
		rgReference[ index ] --;
		if( rgReference[ index ] <= 0 )
		{
			delete ppItem[index];
			ppItem[index] = NULL;
		}
		SemPost(&m_hSemCount);
	};

	int Send(
		HvCore::IHvStream* pStream,
		int index
		)
	{
		HRESULT hr = S_OK;
		SemPend(&m_hSemCount);

		if ( index < 0
			|| index >= MAX_COUNT
			|| rgReference[ index ] <= 0 )
		{
			SemPost(&m_hSemCount);
			return 0;
		}
		else
		{
			SemPost(&m_hSemCount);

			hr = pStream->Write( ppItem[index], ppItem[index]->m_dwLen + 4, NULL );
			Release(index);
			return (SUCCEEDED(hr))?1:-1;
		}
	};
};

//发送类型
class SENDSTYLE
{
public:
	int m_index;
	unsigned short m_wSendType;
};

/*发送数据类型队列*/
template < class T, int MAX_COUNT >
class Q_STYPE
{
private:
	T m_rgItem[ MAX_COUNT ];
	int m_iHead;
	int m_iCount;
	HV_SEM_HANDLE m_hSemCount;

public:
	Q_STYPE()
		: m_iHead( 0 )
		, m_iCount( 0 )
	{
		CreateSemaphore(&m_hSemCount, 1, 1);
	};

	~Q_STYPE()
	{
		DestroySemaphore(&m_hSemCount);
	};

	bool PushBack( const T& item )
	{
		SemPend(&m_hSemCount);
		if( m_iCount >= MAX_COUNT )
		{
			SemPost(&m_hSemCount);
			return false;
		}

		int iPos = ( m_iHead + m_iCount ) % MAX_COUNT;
		m_rgItem[ iPos ] = item;
		m_iCount += 1;
		SemPost(&m_hSemCount);
		return true;
	};

	bool GetFront( T& item )
	{
		SemPend(&m_hSemCount);
		if( m_iCount <= 0 )
		{
			SemPost(&m_hSemCount);
			return false;
		}
		item = m_rgItem[m_iHead];
		m_iHead = ( m_iHead + 1 ) % MAX_COUNT;
		m_iCount -= 1;
		SemPost(&m_hSemCount);
		return true;
	};
};

//--------------------------------------------------------------

//结果发送接口定义
interface IResultSend
{
	//用户接口：发送车辆信息开始接收标志
	STDMETHOD(SendInfoBegin)(
		DWORD32 dwVideoID,
		DWORD32 dwCarID
		) = 0;

	//用户接口：发送车辆信息结束接收标志
	STDMETHOD(SendInfoEnd)(
		DWORD32 dwVideoID,
		DWORD32 dwCarID
		) = 0;

	//用户接口：发送车辆到达离开标志
	STDMETHOD(SendInLeftFlag)(
		DWORD32 dwVideoID,
		DWORD32 dwCarID,
		DWORD32 dwTimeLow,
		DWORD32 dwTimeHigh,
		WORD16 wInLeftFlag
		) = 0;

	//用户接口：发送车牌
	STDMETHOD(SendCarPlate)(
		DWORD32 dwVideoID,
		DWORD32 dwCarID,
		DWORD32 dwTimeLow,
		DWORD32 dwTimeHigh,
		const char *pPlate
		) = 0;

	//用户接口：发送车牌附加信息
	STDMETHOD(SendAppendInfo)(
		DWORD32 dwVideoID,
		DWORD32 dwCarID,
		DWORD32 dwTimeLow,
		DWORD32 dwTimeHigh,
		const char *pPlateInfo
		) = 0;

	//用户接口：发送车辆YUV大图
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

	//发送YUV小图
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

	//发送二值化小图
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

	//发送视频
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


	//用户接口：发送字符流
	STDMETHOD(SendString)(
		DWORD32 dwVideoID,
		WORD16 wStreamID,
		DWORD32 dwTimeLow,
		DWORD32 dwTimeHigh,
		const char *pString
		) = 0;

	//用户接口：发送二进制流
	STDMETHOD(SendBin)(
		DWORD32 dwVideoID,
		WORD16 wStreamID,
		WORD16 wBinSize,
		const unsigned char *pBuffer
		) = 0;
};

interface IResultIO
{
	//用户接口：发送车辆信息开始接收标志
	STDMETHOD(SendInfoBegin)(
		const int& index,
		DWORD32 dwFlag = 0
		) = 0;

	//用户接口：发送车辆信息结束接收标志
	STDMETHOD(SendInfoEnd)(
		const int& index,
		DWORD32 dwFlag = 0
		) = 0;

	//用户接口：发送车辆到达离开标志
	STDMETHOD(SendInLeftFlag)(
		const int& index,
		DWORD32 dwFlag = 0
		) = 0;

	//用户接口：发送车牌
	STDMETHOD(SendCarPlate)(
		const int& index,
		DWORD32 dwFlag = 0
		) = 0;

	//用户接口：发送车牌附加信息
	STDMETHOD(SendAppendInfo)(
		const int& index,
		DWORD32 dwFlag = 0
		) = 0;

	//用户接口：发送车辆YUV大图
	STDMETHOD(SendBigImage)(
		const int& index,
		DWORD32 dwFlag = 0
		) = 0;

	//发送YUV小图
	STDMETHOD(SendSmallImageYUV)(
		const int& index,
		DWORD32 dwFlag = 0
		) = 0;

	//发送二值化小图
	STDMETHOD(SendBinaryImage)(
		const int& index,
		DWORD32 dwFlag = 0
		) = 0;

	//发送视频
	STDMETHOD(SendVideoYUV)(
		const int& index,
		DWORD32* pdwSendCount,
		DWORD32 dwFlag = 0
		) = 0;

	//用户接口：发送字符流
	STDMETHOD(SendString)(
		const int& index,
		DWORD32 dwFlag = 0
		) = 0;

	//用户接口：发送二进制流
	STDMETHOD(SendBin)(
		const int& index,
		DWORD32 dwFlag = 0
		) = 0;

	//设置过滤条件
	STDMETHOD(SetStreamID)(
		DWORD32 dwIP,
		WORD16 wPort,
		WORD16 wVideoID,
		WORD16 wStreamID
		) = 0;

	//删除过滤条件
	STDMETHOD(DeleteStreamID)(
		DWORD32 dwIP,
		WORD16 wPort,
		WORD16 wVideoID,
		WORD16 wStreamID
		) = 0;

	//清除所有过滤条件
	STDMETHOD(ClearAllStreamID)(
		DWORD32 dwIP,
		WORD16 wPort
		) = 0;

	//设置安全传送模式
	STDMETHOD(SetSafeMode)(
		DWORD32 dwIP,
		WORD16 wPort,
		DWORD32 dwTimeLow,
		DWORD32 dwTimeHigh,
		int index,
		int iPageIndex = -1,
		int iPageSize   = 0
		) = 0;

	//查询AVI文件
	STDMETHOD(GetAviByTime)(
		DWORD32 dwIP,
		WORD16 wPort,
		DWORD32 dwTimeLow,
		DWORD32 dwTimeHigh,
		DWORD32 *pdwAviTimeLow,
		DWORD32 *pdwAviTimeHigh,
		DWORD32 *pdwAviLen
		) = 0;

	//查询硬盘录像
	STDMETHOD(GetHistoryVideo)(
		DWORD32 dwIP,
		WORD16 wPort,
		DWORD32 dwTimeLow,
		DWORD32 dwTimeHigh,
		DWORD32 dwTimeSliceS
		) = 0;
};

#endif
