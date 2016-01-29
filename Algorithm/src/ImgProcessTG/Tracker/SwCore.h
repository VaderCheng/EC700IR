#ifndef _SWCORE_INCLUDE_H
#define _SWCORE_INCLUDE_H

#ifdef __cplusplus
extern "C" {
#endif		// #ifdef __cplusplus


enum
{
	CACHE_YTPE_ALL
};

/// ��Ϣ���
typedef void (*DEBUG_OUTPUT)(
    char* szMsg
    );

typedef void (*F_CACHE_WBINV)(
		void* blockPtr, unsigned int byteCnt, unsigned short type, unsigned short wait
    );

typedef void (*F_CACHE_INV)(
		void* blockPtr, unsigned int byteCnt, unsigned short type, unsigned short wait
		);

typedef unsigned int (*GET_SYSTEM_TICK_MS)();
/// ��һ��DMA����������DMA�������������0��ʾʧ�ܡ���Ч���
typedef int (*F_DMA_OPEN)(void);

/// DMA����������0��ʾ�ɹ�
typedef int (*F_DMA_COPY)(
    int nHandle,   ///< SV_CALLBACK_DMA_OPEN �򿪵ľ��
    void* pSrc,    ///< Ҫ������Դ���ݵ�ַ
    void* pDst,    ///< Ŀ���ַ
    int nLen       ///< Ҫ���������ݳ���
);

/// �ȴ�DMA��������������0��ʾ�ɹ����ú���Ϊ����������
typedef int (*F_DMA_WAIT)(
    int nHandle            ///< SV_CALLBACK_DMA_OPEN �򿪵ľ��
);

/// DMA�رպ������ͷ�DMA���
typedef int (*F_DMA_CLOSE)(
    int nHandle            ///< SV_CALLBACK_DMA_OPEN �򿪵ľ��
);

typedef struct
{
	DEBUG_OUTPUT pfDebugOutPut;
	F_CACHE_WBINV pfCacheWbInv;
	F_CACHE_INV pfCacheInv;
	GET_SYSTEM_TICK_MS pfGetSystemTick;
	F_DMA_OPEN pfDMAOpen;
	F_DMA_COPY pfDMACopy;
	F_DMA_WAIT pfDMAWait;
	F_DMA_CLOSE pfDMAClose;
}PLATFORM_FUCTION_LIST;


int InitVideoParam(void* pvParam, int iSize, void* pvRespond, int iRespondSize);
int ModifyVideoParam(void* pvParam, int iSize, void* pvRespond, int iRespondSize);
int ProcessOneFrame(void* pvParam, int iSize, void* pvRespond, int iRespondSize);

void SetFuctionCallBack(PLATFORM_FUCTION_LIST* pcList);

#ifdef __cplusplus
}
#endif		// #ifdef __cplusplus

#endif
