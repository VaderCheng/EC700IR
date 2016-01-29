/// @file
/// @brief ���ߺ�������
/// @author liaoy
/// @date 2013/8/26 10:46:40
///
/// �޸�˵��:
/// [2013/8/26 10:46:40 liaoy] ����汾

#pragma once

#include "sv_basetype.h"
#include "sv_error.h"

#include "sv_rect.h"
#include "sv_image.h"

#if SV_RUN_PLATFORM == SV_PLATFORM_WIN
#include <stdio.h>
#endif

namespace sv
{
    /// �ı���Ϣ����ص�ԭ��
    typedef int (*TRACE_CALLBACK_TXT)(
        const char* szInfo,     ///< �ַ���
        int nLen                ///< �ַ�������+1
    );

    /// �����ı�����ص�
    void utSetTraceCallBack_TXT(
        TRACE_CALLBACK_TXT hCallBack    ///< �ص�����ָ��
    );

    /// ������Ϣ���
    /// @note ��Ϣ���Ȳ��ܳ���640�ֽ�
    void utTrace(
        char* szFmt,        ///< �����ʽ
        ...
    );

    /// ȡ��ϵͳTICK������
    /// @return ϵͳTICK������
    SV_UINT32 utGetSystemTick();

    /// ��ʱ��ʼ
#define TS(tag) SV_UINT32 nTime_##tag = sv::utGetSystemTick(); //

    /// ��ʱ����
#define TE(tag) utTrace("%s: %d ms\n", #tag, sv::utGetSystemTick()-nTime_##tag); //

#define SAFE_DELETE(p) if(p) {delete p; p = NULL;}

    /// ������
    SV_RESULT utMarkLine(
        const SV_IMAGE* pImgSrc,        ///< Դͼ��
        int nLinePos,                   ///< ����λ��
        SV_UINT8 nColor                 ///< ����ɫ
    );

    /// ����
    SV_RESULT utMarkRect(
        const SV_IMAGE* pImgSrc,        ///< Դͼ��
        const SV_RECT* pRect,           ///< ����λ��
        SV_UINT8 nColor                 ///< ����ɫ
    );

    /// ����Ҷ��ڴ�ͼ
    SV_RESULT utSaveGrayImage(
        const SV_WCHAR* szPath,         ///< ����·��
        const void* pImgBuf,            ///< ͼ������
        int nWidth,                     ///< ��
        int nHeight,                    ///< ��
        int nStrideWidth                ///< �ڴ�ɨ���п�
    );

    /// ����Ҷ��ڴ�ͼ
    SV_RESULT utSaveGrayImage(
        const char* szPath,         ///< ����·��
        const void* pImgBuf,            ///< ͼ������
        int nWidth,                     ///< ��
        int nHeight,                    ///< ��
        int nStrideWidth                ///< �ڴ�ɨ���п�
    );

    /// ����BMPͼƬ
    SV_RESULT utSaveImage(
        const SV_WCHAR* szPath,     ///< ����·��
        const SV_IMAGE* pImg        ///< Դͼ��
    );

    /// ����BMPͼƬ
    SV_RESULT utSaveImage(
        const char* szPath,         ///< ����·��
        const SV_IMAGE* pImg        ///< Դͼ��
    );

    /// ����JPEGͼƬ����֧��GRAY��YUV422����
    SV_RESULT utSaveImage_Jpeg(
        const SV_WCHAR* szPath,     ///< ����·��
        const SV_IMAGE* pImg,       ///< Դͼ��
        int nQuality = 95           ///< ѹ������
    );

    /// ����JPEGͼƬ����֧��GRAY��YUV422����
    SV_RESULT utSaveImage_Jpeg(
        const char* szPath,         ///< ����·��
        const SV_IMAGE* pImg,       ///< Դͼ��
        int nQuality = 95           ///< ѹ������
    );

    /// ��ȡJPEGͼƬ
    /// @note ��ͨ��pImg ����
    SV_RESULT utReadImage_Jpeg(
        const SV_UINT8* pJpegDat,       ///< JPEG����
        int nDatLen,                    ///< ���ݳ���
        SV_IMAGE* pImg,                 ///< ��ѹ��ͼ��,ֻ֧��YUV422��ʽ
        int* pWidth,                    ///< ��
        int* pHeight                    ///< ��
    );

#if SV_RUN_PLATFORM == SV_PLATFORM_WIN
    /// �����ڴ����ݵ��ı��ļ�
    template<typename T>
    SV_RESULT utMemDump(
        const char* szDumpFile,
        const T* pDat,
        int nWidth,
        int nHeight,
        int nStride = -1
    )
    {
        FILE* hFile = NULL;
        fopen_s(&hFile, szDumpFile, "w");

        if(hFile == NULL)
        {
            return RS_E_UNEXPECTED;
        }

        if(nStride == -1)
        {
            nStride = nWidth;
        }

        T* pLine = (T*)pDat;

        for(int i = 0; i < nHeight; i++, pLine += nStride)
        {
            for(int j = 0; j < nWidth; j++)
            {
                fprintf(hFile, "%8.3f,", (float)pLine[j]);
            }

            fprintf(hFile, "\n");
        }

        fclose(hFile);

        return RS_S_OK;
    }

    /// ��ȡ�ı��ļ����ڴ�
    template<typename T>
    SV_RESULT utLoadMem(
        const char* szMemFile,
        T* pDat,
        int nLen,
        int* pWidth,
        int* pHeight
    )
    {
        if(pDat == NULL || pWidth == NULL || pHeight == NULL)
        {
            return RS_E_INVALIDARG;
        }

        *pWidth = 0;
        *pHeight = 0;

        FILE* hFile = NULL;
        fopen_s(&hFile, szMemFile, "r");

        if(hFile == NULL)
        {
            return RS_E_UNEXPECTED;
        }

        const int nMaxLen = 100000;
        CMemAlloc cAlloc;
        char* pLine = (char*)cAlloc.Alloc(nMaxLen * sizeof(char));

        if(pLine == NULL)
        {
            return RS_E_UNEXPECTED;
        }

        SV_BOOL fLoadOK = TRUE;
        int nReadCount = 0;
        int nHeight = 0;
        int nWidth = 0;

        while(fgets(pLine, nMaxLen, hFile))
        {
            pLine[strlen(pLine) - 1] = '\0';

            char* pTokNext = NULL;
            int nLineWidth = 0;
            char* pTok = strtok_s(pLine, ",", &pTokNext);

            while(pTok)
            {
                if(nReadCount >= nLen)
                {
                    fLoadOK = FALSE;
                    break;
                }

                pDat[nReadCount++] = (T)atof(pTok);
                pTok = strtok_s(NULL, ",", &pTokNext);
                nLineWidth++;
            }

            nWidth = SV_MAX(nWidth, nLineWidth);
            nHeight++;
        }

        fclose(hFile);

        if(!fLoadOK)
        {
            return RS_E_OUTOFMEMORY;
        }

        *pWidth = nWidth;
        *pHeight = nHeight;

        return RS_S_OK;
    }
#else
    template<typename T>
    SV_RESULT utMemDump(
        const char* szDumpFile,
        const T* pDat,
        int nWidth,
        int nHeight,
        int nStride = -1
    )
    {
        return RS_E_NOTIMPL;
    }

    template<typename T>
    SV_RESULT utLoadMem(
        const char* szMemFile,
        T* pDat,
        int nLen,
        int* pWidth,
        int* pHeight
    )
    {
        return RS_E_NOTIMPL;
    }
#endif

} // sv
