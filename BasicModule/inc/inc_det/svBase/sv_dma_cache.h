/// @file
/// @brief DMA��CACHE��ز���
/// @author ganzz
/// @date 10/29/2013 10:12:23
///
/// �޸�˵��:
/// [10/29/2013 10:12:23 ganzz] ����汾

#pragma once

#include "sv_basetype.h"
#include "sv_error.h"

namespace sv
{

    // Cache���
    /// �ж��Ƿ�ʹ����CACHE����
    SV_BOOL cacheIsOprEnable();
    SV_RESULT cacheInvalidate(void *ptr, SV_UINT32 count);
    SV_RESULT cacheWriteBack(void *ptr, SV_UINT32 count);
    SV_RESULT cacheWriteBackInvalidate(void *ptr, SV_UINT32 count);

    // DMA���
    /// DMA���
    typedef int SV_DMA_HANDLE;

    /// �ж��Ƿ�ʹ����DMA
    SV_BOOL dmaIsEnable();

    /// ��DMA��������һ�������0Ϊ��Ч
    SV_DMA_HANDLE dmaOpen();

    /// DMA�ر�
    void dmaClose(SV_DMA_HANDLE hDma);

    /// DMA����
    /// DMAʹ�ܺ������ʹ��DMA��δʹ��DMA��ʧ��ʱ��dmaCopy��������memCpy����
    SV_RESULT dmaCopy(
        SV_DMA_HANDLE hDma,
        void* pbDst,
        void* pbSrc,
        int iSize
    );

    /// DMA�ȴ�������ʽ
    void dmaWait(SV_DMA_HANDLE hDma);

}
