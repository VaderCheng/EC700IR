/// @file
/// @brief �������ͼ���������
/// @author liaoy
/// @date 2013/8/26 10:46:40
///
/// �޸�˵��:
/// [2013/8/26 10:46:40 liaoy] ����汾

#pragma once

#include "sv_basetype.h"
#include "sv_point.h"

namespace sv
{
    /// ���νṹ��
    struct SV_RECT
    {
        /// ��
        int m_nLeft;
        /// ��
        int m_nTop;
        /// ��
        int m_nRight;
        /// ��
        int m_nBottom;
    };

    /// ������
    class CSvRect : public SV_RECT
    {
    public:
        /// ���캯��
        CSvRect() {};

        /// ���캯��
        CSvRect(
            int nLeft,      ///< ��
            int nTop,       ///< ��
            int nRight,     ///< ��
            int nBottom ///< ��
        )
        {
            m_nLeft = nLeft;
            m_nTop = nTop;
            m_nRight = nRight;
            m_nBottom = nBottom;
        }

        /// �������캯��
        CSvRect(
            const SV_RECT& rectSrc      ///< Դ����
        )
        {
            m_nLeft = rectSrc.m_nLeft;
            m_nTop = rectSrc.m_nTop;
            m_nRight = rectSrc.m_nRight;
            m_nBottom = rectSrc.m_nBottom;
        }

    public:
        /// ���þ�������
        void SetRect(
            int nLeft,      ///< ��
            int nTop,       ///< ��
            int nRight,     ///< ��
            int nBottom ///< ��
        )
        {
            m_nLeft = nLeft;
            m_nTop  = nTop;
            m_nRight = nRight;
            m_nBottom = nBottom;
        }

        /// ���
        /// @note ���Ϊ��-��+1
        int GetWidth()
        {
            return m_nRight - m_nLeft + 1;
        }

        /// �߶�
        /// @note �߶�Ϊ��-��+1
        int GetHeight()
        {
            return m_nBottom - m_nTop + 1;
        }

        /// ���
        int GetArea()
        {
            return (m_nRight - m_nLeft + 1) * (m_nBottom - m_nTop + 1);
        }

        /// ���ĵ�
        SV_POINT CenterPoint()
        {
            SV_POINT cCentPoint;
            cCentPoint.m_nX = (m_nLeft + m_nRight) >> 1;
            cCentPoint.m_nY = (m_nBottom + m_nTop) >> 1;
            return cCentPoint;
        }
    };

    /// �����ص��жϺ���
    SV_BOOL rectIsOverlap(
        const SV_RECT* pRect0,      ///< ����0
        const SV_RECT* pRect1,      ///< ����1
        int p                       ///< �ص��ٷֱ���ֵ
    );

    /// ��ȡ�ص�����
    /// return �Ƿ��ص�
    SV_BOOL rectGetOverlap(
        const SV_RECT* pRect0,      ///< ����0
        const SV_RECT* pRect1,      ///< ����1
        SV_RECT* pRectOverlap       ///< �ص�����
    );

} // sv
