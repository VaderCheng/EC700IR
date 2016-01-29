#pragma once

/// ���볣���ڴ�
typedef void* (*SV_CALLBACK_MEM_ALLOC)(
    int nSize  ///< ���볤��
);

/// ����Ƭ���ڴ�
typedef void* (*SV_CALLBACK_FAST_MEM_ALLOC)(
    int nSize  ///< ���볤��
);

/// �ͷ��ڴ�
typedef void (*SV_CALLBACK_MEM_FREE)(
    void* pAddr,                    ///< Ҫ�ͷŵĵ�ַ
    int nSize                       ///< �ڴ泤��
);

/// �ͷ�Ƭ���ڴ�
typedef void (*SV_CALLBACK_FAST_MEM_FREE)(
    void* pAddr,                    ///< Ҫ�ͷŵĵ�ַ
    int nSize                       ///< �ڴ泤��
);

/// Cache��Ч
typedef int (*SV_CALLBACK_CACHE_INVAILD)(
    void* pAddr,                    ///< Cache��Ч����ʼ��ַ
    int nSize                       ///< Cache��Ч�ĳ���
);

/// Cache��д
typedef int (*SV_CALLBACK_CACHE_WRITE_BACK)(
    void* pAddr,                    ///< Cache��д����ʼ��ַ
    int nSize                       ///< Cache��д�ĳ���
);

/// Cache��д����Ч
typedef int (*SV_CALLBACK_CACHE_WRITE_BACK_INVAILD)(
    void* pAddr,                    ///< Cache��д����Ч����ʼ��ַ
    int nSize                       ///< Cache��д����Ч�ĳ���
);

/// ��һ��DMA����������DMA�������������0��ʾʧ�ܡ���Ч���
typedef int (*SV_CALLBACK_DMA_OPEN)(void);

/// DMA����������0��ʾ�ɹ�
typedef int (*SV_CALLBACK_DMA_COPY)(
    int nHandle,   ///< SV_CALLBACK_DMA_OPEN �򿪵ľ��
    void* pDst,    ///< Ŀ���ַ
    void* pSrc,    ///< Ҫ������Դ���ݵ�ַ
    int nLen       ///< Ҫ���������ݳ���
);

/// �ȴ�DMA��������������0��ʾ�ɹ����ú���Ϊ����������
typedef int (*SV_CALLBACK_DMA_WAIT)(
    int nHandle            ///< SV_CALLBACK_DMA_OPEN �򿪵ľ��
);

/// DMA�رպ������ͷ�DMA���
typedef int (*SV_CALLBACK_DMA_CLOSE)(
    int nHandle            ///< SV_CALLBACK_DMA_OPEN �򿪵ľ��
);

/// ȡϵͳʱ��
typedef unsigned int (*SV_CALLBACK_GET_SYS_TIME)();

/// ��������
namespace sv
{
    // �ڴ�
    int SetCallBack_MemAlloc(SV_CALLBACK_MEM_ALLOC func);
    int SetCallBack_FastMemAlloc(SV_CALLBACK_FAST_MEM_ALLOC func);
    int SetCallBack_MemFree(SV_CALLBACK_MEM_FREE func);
    int SetCallBack_FastMemFree(SV_CALLBACK_FAST_MEM_FREE func);

    // cache
    int SetCallBack_CacheInvaild(SV_CALLBACK_CACHE_INVAILD func);
    int SetCallBack_CacheWriteBack(SV_CALLBACK_CACHE_WRITE_BACK func);
    int SetCallBack_CacheWriteBackInvaild(SV_CALLBACK_CACHE_WRITE_BACK_INVAILD func);

    // dma
    int SetCallBack_DmaOpen(SV_CALLBACK_DMA_OPEN func);
    int SetCallBack_DmaCopy(SV_CALLBACK_DMA_COPY func);
    int SetCallBack_DmaWait(SV_CALLBACK_DMA_WAIT func);
    int SetCallBack_DmaClose(SV_CALLBACK_DMA_CLOSE func);

    // ȡϵͳʱ��
    int SetCallBack_GetSysTime(SV_CALLBACK_GET_SYS_TIME func);
}
