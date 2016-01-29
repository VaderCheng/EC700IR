/// @file
/// @brief ͼ�����ͼ���������
/// @author liaoy
/// @date 2013/8/26 10:46:40
///
/// �޸�˵��:
/// [2013/8/26 10:46:40 liaoy] ����汾

#pragma once

#include "sv_basetype.h"
#include "sv_error.h"
#include "sv_memory.h"
#include "sv_rect.h"

namespace sv
{
    /// ͼ������
    enum SV_IMAGE_TYPE
    {
        SV_IMAGE_UNKNOWN    = -1,   ///< δ֪��ʽ
        SV_IMAGE_YUV422     = 0,    ///< YUV����,UV��ȼ���
        SV_IMAGE_GRAY       = 1,    ///< ��ͨ���Ҷ�
        SV_IMAGE_RGB        = 2,    ///< ����ͨ��RGB
        SV_IMAGE_HSV        = 3,    ///< ����ͨ��HSV
        SV_IMAGE_ITG        = 4     ///< ��ͨ������ͼ
    };

    /// ͼ��ṹ��(λ��,ͨ�����ɱ�)
    template<class T, int N>
    struct SV_IMAGE_T
    {
        /// ͼ������
        SV_IMAGE_TYPE m_nType;
        /// ͼ����
        int m_nWidth;
        /// ͼ��߶�
        int m_nHeight;
        /// ͼ���ڴ��п��(���ֽ�Ϊ��λ)
        int m_rgStrideWidth[N];
        /// ͼ������ָ��
        T* m_pData[N];

        SV_IMAGE_T()
        {
            m_nType = SV_IMAGE_UNKNOWN;
            m_nWidth = 0;
            m_nHeight = 0;

            for(int i = 0; i < N; i++)
            {
                m_rgStrideWidth[i] = 0;
                m_pData[i] = 0;
            }
        }
    };

    /// 8λ3ͨ��ͼ��
    typedef SV_IMAGE_T<SV_UINT8, 3> SV_IMAGE;

    /// 32λ��ͨ������ͼ
    typedef SV_IMAGE_T<SV_UINT32, 1> SV_ITG;

    /// ͼ����
    class CSvImage : public SV_IMAGE
    {
    public:
        /// ���캯��
        CSvImage()
        {
            return;
        }
        /// ��������
        ~CSvImage()
        {
            Clear();
            return;
        }

    public:
        /// ������
        void Clear()
        {
            FreeData();

            m_nType =  SV_IMAGE_UNKNOWN;
            m_nWidth = 0;
            m_nHeight = 0;
        }

        /// �ͷ�ͼ���ڴ�,����������Ϣ
        void FreeData()
        {
            m_cAlloc.Free();

            for(int i = 0; i < 3; i++)
            {
                m_rgStrideWidth[i] = 0;
                m_pData[i] = NULL;
            }
        }

        /// ��������
        SV_RESULT Copy(
            const SV_IMAGE* pImgSrc,    ///< Դͼ��
            SV_BOOL fFastMem = FALSE    ///< �Ƿ񿽱���Ƭ��
        )
        {
            SV_BOOL fReCreate = FALSE;

            if(m_nType != pImgSrc->m_nType ||
                    m_nWidth < pImgSrc->m_nWidth ||
                    m_nHeight < pImgSrc->m_nHeight ||
                    fFastMem == TRUE
              )
            {
                fReCreate = TRUE;
            }

            if(fReCreate)
            {
                SV_RESULT hr = Create(pImgSrc->m_nType, pImgSrc->m_nWidth, pImgSrc->m_nHeight, fFastMem);

                if(SV_FAILED(hr))
                {
                    return hr;
                }
            }

            //����һ������
            SV_UINT8* restrict pSrcLine = pImgSrc->m_pData[0];
            SV_UINT8* restrict pDestLine = m_pData[0];

            for(int i = 0; i < pImgSrc->m_nHeight; i++, pSrcLine += pImgSrc->m_rgStrideWidth[0], pDestLine += m_rgStrideWidth[0])
            {
                memCpy(pDestLine, pSrcLine, pImgSrc->m_nWidth);
            }

            //����UV����
            if(pImgSrc->m_nType == SV_IMAGE_YUV422)
            {
                for(int i = 1; i <= 2; i++)
                {
                    pSrcLine = pImgSrc->m_pData[i];
                    pDestLine = m_pData[i];

                    for(int j = 0; j < pImgSrc->m_nHeight; j++, pSrcLine += pImgSrc->m_rgStrideWidth[i], pDestLine += m_rgStrideWidth[i])
                    {
                        memCpy(pDestLine, pSrcLine, (pImgSrc->m_nWidth + 1) >> 1);
                    }
                }
            }

            m_nWidth = pImgSrc->m_nWidth;
            m_nHeight = pImgSrc->m_nHeight;

            return RS_S_OK;
        }

        /// ����ͼ��
        SV_RESULT Create(
            SV_IMAGE_TYPE nType,        ///< ����ͼ������
            SV_INT32 nWidth,            ///< ͼ����
            SV_INT32 nHeight,           ///< ͼ��߶�
            SV_BOOL fFastMem = FALSE    ///< �Ƿ�ʹ��Ƭ���ڴ洴��
        )
        {
            Clear();

            int nBufSize = 0;

            switch(nType)
            {
            case SV_IMAGE_YUV422:
                m_rgStrideWidth[0] = (nWidth + 31) / 32 * 32;
                m_rgStrideWidth[1] = (((nWidth + 1) >> 1) + 31) / 32 * 32;
                m_rgStrideWidth[2] = (((nWidth + 1) >> 1) + 31) / 32 * 32;
                nBufSize = (m_rgStrideWidth[0] + m_rgStrideWidth[1] + m_rgStrideWidth[2]) * nHeight;
                break;

            case SV_IMAGE_GRAY:
                m_rgStrideWidth[0] = (nWidth + 31) / 32 * 32;
                m_rgStrideWidth[1] = 0;
                m_rgStrideWidth[2] = 0;
                nBufSize = m_rgStrideWidth[0] * nHeight;
                break;

            case SV_IMAGE_RGB:
            case SV_IMAGE_HSV:
                m_rgStrideWidth[0] = (nWidth * 3 + 3) / 4 * 4;
                m_rgStrideWidth[1] = (nWidth * 3 + 3) / 4 * 4;
                m_rgStrideWidth[2] = (nWidth * 3 + 3) / 4 * 4;
                nBufSize = m_rgStrideWidth[0] * nHeight;
                break;

            default:
                m_rgStrideWidth[0] = 0;
                m_rgStrideWidth[1] = 0;
                m_rgStrideWidth[2] = 0;
                break;
            }

            if(nBufSize == 0)
            {
                return RS_E_INVALIDARG;
            }

            SV_UINT8* pBuf = (SV_UINT8*)m_cAlloc.Alloc(nBufSize, fFastMem);

            if(pBuf == NULL)
            {
                return RS_E_OUTOFMEMORY;
            }

            switch(nType)
            {
            case SV_IMAGE_YUV422:
                m_pData[0] = pBuf;
                m_pData[1] = m_pData[0] + m_rgStrideWidth[0] * nHeight;
                m_pData[2] = m_pData[1] + m_rgStrideWidth[1] * nHeight;
                break;

            case SV_IMAGE_GRAY:
                m_pData[0] = pBuf;
                m_pData[1] = NULL;
                m_pData[2] = NULL;
                break;

            case SV_IMAGE_RGB:
            case SV_IMAGE_HSV:
                m_pData[0] = pBuf;
                m_pData[1] = pBuf + 1;
                m_pData[2] = pBuf + 2;
                break;

            default:
                break;
            }

            m_nType = nType;
            m_nWidth = nWidth;
            m_nHeight = nHeight;

            return RS_S_OK;
        }

    private:
        /// �ڴ������
        CMemAlloc m_cAlloc;
    };

    /// ����ͼ��
    class CSvItg : public SV_ITG
    {
    public:
        /// ���캯��
        CSvItg()
        {
            return;
        }
        /// ��������
        ~CSvItg()
        {
            Clear();
            return;
        }

    public:
        /// ������
        void Clear()
        {
            m_cAlloc.Free();

            m_nType = SV_IMAGE_UNKNOWN;
            m_nWidth = 0;
            m_nHeight = 0;
            m_rgStrideWidth[0] = 0;
            m_pData[0] = NULL;
        }

        /// ��������
        SV_RESULT Copy(
            const SV_ITG* pItgSrc,      ///< Դ����ͼ
            SV_BOOL fFastMem = FALSE    ///< �Ƿ񿽱���Ƭ��
        )
        {
            SV_BOOL fReCreate = FALSE;

            if(m_nWidth < pItgSrc->m_nWidth ||
                    m_nHeight < pItgSrc->m_nHeight ||
                    fFastMem == TRUE)
            {
                fReCreate = TRUE;
            }

            if(fReCreate)
            {
                SV_RESULT hr = Create(pItgSrc->m_nWidth, pItgSrc->m_nHeight, fFastMem);

                if(SV_FAILED(hr))
                {
                    return hr;
                }
            }

            SV_UINT32* pSrcLine = pItgSrc->m_pData[0];
            SV_UINT32* pDestLine = m_pData[0];

            for(int i = 0; i < pItgSrc->m_nHeight; i++, pSrcLine += pItgSrc->m_rgStrideWidth[0], pDestLine += m_rgStrideWidth[0])
            {
                memCpy(pDestLine, pSrcLine, (pItgSrc->m_nWidth + 1) * sizeof(SV_UINT32));
            }

            m_nType = pItgSrc->m_nType;
            m_nWidth = pItgSrc->m_nWidth;
            m_nHeight = pItgSrc->m_nHeight;

            return RS_S_OK;
        }

        /// ��������ͼ�������ڴ�
        /// @note ��߶�Ӧԭʼͼ��ߴ�,�ǻ���ͼ�ڴ�ߴ�
        SV_RESULT Create(
            SV_INT32 nWidth,            ///< ͼ����
            SV_INT32 nHeight,           ///< ͼ��߶�
            SV_BOOL fFastMem = FALSE    ///< �Ƿ�ʹ��Ƭ���ڴ洴��
        )
        {
            Clear();

            m_rgStrideWidth[0] = (nWidth + 1) * sizeof(SV_UINT32);
            int nBufSize = m_rgStrideWidth[0] * (nHeight + 1);

            if(nBufSize <= 0)
            {
                return RS_E_INVALIDARG;
            }

            SV_UINT32* pBuf = (SV_UINT32*)m_cAlloc.Alloc(nBufSize, fFastMem);

            if(pBuf == NULL)
            {
                return RS_E_OUTOFMEMORY;
            }

            m_pData[0] = pBuf;

            m_nType = SV_IMAGE_ITG;
            m_nWidth = nWidth;
            m_nHeight = nHeight;

            return RS_S_OK;
        }

    private:
        /// �ڴ������
        CMemAlloc m_cAlloc;
    };

    /// ��ͼ
    /// @note pRectCrop���ڴ�����,��x:[0,nWidth-1], y:[0,nHeight-1],��ͼ�����߿�
    SV_RESULT imCrop(
        const SV_IMAGE* pImgSrc,        ///< Դͼ��
        const SV_RECT* pRectCrop,       ///< ��ͼ����
        SV_IMAGE* pImgCrop              ///< ���ͼ
    );

    /// ����ͼ��
    /// @note ֻ֧��YUV422��GRAY
    SV_RESULT imCopy(
        const SV_IMAGE* pImgSrc,
        SV_IMAGE* pImgDest
    );

    /// RGBתYCbCr
    /// @note RGB�洢�ṹΪ B|G|R
    SV_RESULT imRGB2YCbCr(
        const SV_IMAGE* pImgRGB,    ///< RGBͼ
        SV_IMAGE* pImgYCbCr     ///< YCbCr���ͼ
    );

    /// YCbCrתRGB
    /// @note RGB�洢�ṹΪ B|G|R
    SV_RESULT imYCbCr2RGB(
        const SV_IMAGE* pImgYCbCr,      ///< YCbCrͼ
        SV_IMAGE* pImgRGB               ///< RGB���ͼ
    );

    /// GrayתRGB
    SV_RESULT imGray2RGB(
        const SV_IMAGE* pImgGray,   ///< GRAYͼ
        SV_IMAGE* pImgRGB           ///< RGBͼ
    );

    /// YCbCrתHSV
    SV_RESULT imYCbCr2HSV(
        const SV_IMAGE* pImgYCbCr,      ///< YCbCrԴͼ
        SV_IMAGE* pImgHSV               ///< HSVͼ
    );

    /// ��ֵ������
    enum BW_TYPE
    {
        SV_THRESH_BINARY        =   0,  ///< value = value > threshold ? max_value : 0
        SV_THRESH_BINARY_INV    =   1,  ///< value = value > threshold ? 0 : max_value
        SV_THRESH_TRUNC         =   2,  ///< value = value > threshold ? threshold : value
        SV_THRESH_TOZERO        =   3,  ///< value = value > threshold ? value : 0
        SV_THRESH_TOZERO_INV    =   4   ///< value = value > threshold ? 0 : value
    };

    /// ��ֵ������
    SV_RESULT imTransBW(
        const SV_IMAGE* pImgSrc,    ///< Դͼ��
        BW_TYPE nType,              ///< �任����
        SV_UINT8 nThresh,           ///< ��ֵ
        SV_UINT8 nMaxVal,           ///< ���ֵ
        SV_IMAGE* pImgDst           ///< ���ͼ
    );

    /// ��ͼ���
    SV_UINT32 imCalcSum(
        const SV_IMAGE* pImgSrc ///< Դͼ��
    );

    /// ��ָ�������ֵ
    SV_FLOAT imCalcAvg(
        const SV_IMAGE* pImgSrc,            ///< Դͼ��
        const SV_IMAGE* pImgMask = NULL ///< ģ��ͼ
    );

    /// ͼ���
    SV_RESULT imAdd(
        const SV_IMAGE* pImgSrc0,       ///< Դͼ��0
        const SV_IMAGE* pImgSrc1,       ///< Դͼ��1
        SV_IMAGE* pImgDst               ///< ���ͼ
    );

    /// ͼ�����ֵ���
    SV_RESULT imAbsDiff(
        const SV_IMAGE* pImgSrc0,       ///< Դͼ��0
        const SV_IMAGE* pImgSrc1,       ///< Դͼ��1
        SV_IMAGE* pImgDst               ///< ���ͼ
    );

    /// ͼ����
    /// @note ֻ�����һ������
    SV_RESULT imAnd(
        const SV_IMAGE* pImgSrc0,       ///< Դͼ��0
        const SV_IMAGE* pImgSrc1,       ///< Դͼ��1
        SV_IMAGE* pImgDst               ///< ���ͼ
    );

    /// ͼ���
    /// @note ֻ�����һ������
    SV_RESULT imOr(
        const SV_IMAGE* pImgSrc0,       ///< Դͼ��0
        const SV_IMAGE* pImgSrc1,       ///< Դͼ��1
        SV_IMAGE* pImgDst               ///< ���ͼ
    );

    /// ͼ���
    /// @note ֻ�����һ������
    SV_RESULT imNot(
        const SV_IMAGE* pImgSrc,        ///< Դͼ��
        SV_IMAGE* pImgDst               ///< ���ͼ
    );

    /// ͼ�����
    SV_RESULT imFillGray(
        SV_IMAGE* pImgSrc,              ///< Դͼ��
        int nValue                      ///< ���ֵ
    );

    /// ��ˮƽ����Sobel
    SV_RESULT imSobelH(
        const SV_IMAGE* pImgSrc,        ///< Դͼ��
        SV_IMAGE* pImgDst               ///< Ŀ��ͼ
    );

    /// ����ֱ����Sobel
    SV_RESULT imSobelV(
        const SV_IMAGE* pImgSrc,        ///< Դͼ��
        SV_IMAGE* pImgDst               ///< Ŀ��ͼ
    );

    /// ��Sobelͼ
    SV_RESULT imSobel(
        const SV_IMAGE* pImgSrc,        ///< Դͼ��
        SV_IMAGE* pImgDst               ///< Ŀ��ͼ
    );

    /// �����������
    SV_RESULT imResize(
        const SV_IMAGE* pImgSrc,        ///< Դͼ��
        SV_IMAGE* pImgDst               ///< Ŀ��ͼ
    );

    SV_RESULT imResizeCube(
        const  SV_IMAGE* pImgSrc,
        SV_IMAGE* pImgDst
    );

    /// ��С2^nRatio��
    SV_RESULT imDownSamp(
        const SV_IMAGE* pImgSrc,
        SV_IMAGE* pImgDst,
        int nRatio
    );

    /// ��˹�˲�
    /// @note 5x5,����=0.5,ֻ����Y����
    SV_RESULT imGaussian(
        SV_IMAGE* pSrc,
        SV_IMAGE* pDest
    );

    /// 32λ����ͼ
    SV_RESULT imCalcItg(
        const SV_IMAGE* pImgSrc,        ///< Դͼ
        SV_ITG* pItg                    ///< ����ͼ
    );

    /// 32λƽ������ͼ
    SV_RESULT imCalcSqItg(
        const SV_IMAGE* pImgSrc,        ///< Դͼ
        SV_ITG* pItg                    ///< ����ͼ
    );

    /// �û���ͼ��������ҶȺ�
    /// @note ΪЧ�ʿ��ǲ�����Ч���ж�
    __inline SV_UINT32 imCalcAreaSum(
        const SV_ITG* pItg,         ///< ����ͼ
        const SV_RECT* pArea        ///< �������
    )
    {
        int x0 = pArea->m_nLeft;
        int y0 = pArea->m_nTop;
        int x1 = pArea->m_nRight + 1;
        int y1 = pArea->m_nBottom + 1;
        int w = pItg->m_nWidth + 1;
        return  pItg->m_pData[0][ y1 * w + x1] -
                pItg->m_pData[0][ y0 * w + x1] -
                pItg->m_pData[0][ y1 * w + x0] +
                pItg->m_pData[0][ y0 * w + x0];
    }

    /// ��̬ѧ���Ͳ���
    /// @note ֻ֧�־�������ģ��
    SV_RESULT imDilate(
        const SV_IMAGE* pImgSrc,  ///< Դͼ��
        int nElemW,               ///< ����ģ���
        int nElemH,               ///< ����ģ���
        SV_IMAGE* pImgDst         ///< Ŀ��ͼ
    );

    /// ��̬ѧ��ʴ����
    /// @note ֻ֧�־��θ�ʴģ��
    SV_RESULT imErode(
        const SV_IMAGE* pImgSrc,  ///< Դͼ��
        int nElemW,               ///< ��ʴģ���
        int nElemH,               ///< ��ʴģ���
        SV_IMAGE* pImgDst         ///< Ŀ��ͼ
    );

    /// ���������ο�
    /// @note Դͼ�񽫱��ı�
    SV_RESULT imFindContoursRect(
        SV_IMAGE* pImgSrc,        ///< Դͼ��
        SV_RECT* rgRects,         ///< ��������
        int nRectMax,             ///< ��������������
        int* pnRectCnt            ///< �ҵ��ľ��θ���
    );

    /// 2-SIGMA ͼ������
    /// @note ֱ����Դͼ�ϱ任
    SV_RESULT imHistAdj(
        SV_IMAGE* pImgTrans
    );
} // sv
