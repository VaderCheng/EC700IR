/// @file
/// @brief �ڴ����
/// @author liaoy
/// @date 2013/8/26 10:46:40
///
/// �޸�˵��:
/// [2013/8/26 10:46:40 liaoy] ����汾

#pragma once

#include "sv_basetype.h"
#include "sv_error.h"
#include "string.h"

namespace sv
{
    /// �����ڴ�
    /// @note ���Ƭ�����벻��,���Զ���Ƭ������
    void* memAlloc(
        int nSize,                      ///< ���볤��
        SV_BOOL fFastMem = FALSE        ///< �Ƿ���Ƭ������,Ĭ��ΪFALSE
    );

    /// �ͷ��ڴ�
    void memFree(
        void* pAddr,                    ///< Ҫ�ͷŵĵ�ַ
        int nSize,                      ///< �ڴ泤��
        SV_BOOL fFastMem = FALSE        ///< �Ƿ���Ƭ������,Ĭ��ΪFALSE
    );

    /// �ڴ濽��
    __inline void* memCpy(
        void* pDest,                    ///< Ŀ�ĵ�ַ
        const void* pSrc,               ///< Դ��ַ
        int nLen                        ///< ����
    )
    {
        return memcpy(pDest, pSrc, nLen);
    }

    /// �ڴ�����
    __inline void* memSet(
        void* pDest,                    ///< Ŀ�ĵ�ַ
        int nVal,                       ///< ����ֵ
        int nLen                        ///< ����
    )
    {
        return memset(pDest, nVal, nLen);
    }

    /// �ڴ�Ƚ�
    /// @return �ȽϽ��
    /// @retval 0 ����һ��
    /// @retval ���� ���ݲ�һ��
    __inline int memCmp(
        const void* pSrc,           ///< Դ��ַ
        const void* pDest,          ///< Ŀ�ĵ�ַ
        int nLen                    ///< ����
    )
    {
        return memcmp(pSrc, pDest, nLen);
    }

    /// �����ڴ������
    class CMemAlloc
    {
    public:
        /// ���캯��
        CMemAlloc();

        /// ��������
        ~CMemAlloc();

        /// �ڴ����
        /// @note ���Ƭ�����벻��,���Զ���Ƭ������
        void* Alloc(
            int nSize,                      ///< ������ڴ泤��
            SV_BOOL fFastMem = FALSE        ///< �Ƿ���Ƭ������,Ĭ��ΪFALSE
        );

        /// �ֶ��ͷ��ڴ�
        void Free();

        /// ��ѯ�Ƿ���Ƭ�ڷ���
        SV_BOOL IsFastMem();
    private:
        /// �ڴ��ַ
        void* m_pAddr;

        /// �ڴ泤��
        int m_nSize;

        /// �Ƿ�Ƭ�ڷ���
        SV_BOOL m_fFastMem;
    };

    /// ����ڴ������
    template<int N>
    class CMultiMemAlloc_T
    {
    public:
        /// ���캯��
        CMultiMemAlloc_T()
        {
            for(int i = 0; i < N; i++)
            {
                m_rgAddr[i] = NULL;
                m_rgSize[i] = 0;
                m_rgFastMem[i] = FALSE;
            }
        }

        /// ��������
        ~CMultiMemAlloc_T()
        {
            Free();
        }

        /// �ڴ����
        /// @note ���Ƭ�����벻��,���Զ���Ƭ������
        void* Alloc(
            int nSize,                      ///< ������ڴ泤��
            SV_BOOL fFastMem = FALSE        ///< �Ƿ���Ƭ������,Ĭ��ΪFALSE
        )
        {
            // ���ҿ���λ��
            int nPos = -1;

            for(int i = 0; i < N; i++)
            {
                if(m_rgAddr[i] == NULL)
                {
                    nPos = i;
                    break;
                }
            }

            if(nPos == -1)
            {
                return NULL;
            }

            // �����ڴ�
            void *pTemp = NULL;
            nSize = ((nSize + 1023) >> 10) << 10;     //1024�ֽڶ���

            if(fFastMem)
            {
                pTemp = memAlloc(nSize, TRUE);
            }

            if(pTemp == NULL)
            {
                pTemp = memAlloc(nSize, FALSE);
                fFastMem = FALSE;
            }

            if(pTemp == NULL)
            {
                return NULL;
            }

            m_rgAddr[nPos] = pTemp;
            m_rgSize[nPos] = nSize;
            m_rgFastMem[nPos] = fFastMem;
            return pTemp;
        }

        /// �ֶ��ͷ��ڴ�
        void Free(
            void* pAddr = NULL      ///< Ҫ�ͷŵ��ڴ��ַ,���Ϊ����Ϊȫ���ͷ�
        )
        {
            for(int i = 0; i < N; i++)
            {
                if(m_rgAddr[i] == NULL)
                {
                    continue;
                }

                if(pAddr == NULL || m_rgAddr[i] == pAddr)
                {
                    memFree(m_rgAddr[i], m_rgSize[i], m_rgFastMem[i]);
                    m_rgAddr[i] = NULL;
                }
            }
        }

        /// ��ȡ���ɷ������
        int GetMaxAlloc()
        {
            return N;
        }

    private:
        /// ��ַ����
        void* m_rgAddr[N];

        /// ��������
        int m_rgSize[N];

        /// Ƭ���ڴ���
        SV_BOOL m_rgFastMem[N];
    };

    /// Ĭ�϶��ڴ������(���32���ڴ�)
    typedef CMultiMemAlloc_T<32> CMultiMemAlloc;

} // sv
