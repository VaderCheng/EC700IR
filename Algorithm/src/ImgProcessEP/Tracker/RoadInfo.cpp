#include "RoadInfo.h"
#include <math.h>
#include "swwinerror.h"

namespace HiVideo
{
    //////////////////////////////////////////////////////////////////////////
    // CRoadInfo impl
    //

    CRoadInfo::CRoadInfo()
    {

    }

    CRoadInfo::~CRoadInfo()
    {

    }

    HRESULT CRoadInfo::Init(int iFrameWidth, int iFrameHeight, const HV_ROAD_LINE_INFO* rgRoadLine, int iRoadLineCnt)
    {
        m_iRoadCount = iRoadLineCnt - 1;

        if((m_iRoadCount <= 0)
                || (m_iRoadCount >= MAX_ROAD_INFO))
        {
            return E_INVALIDARG;
        }

        m_iSceneWidth = iFrameWidth;
        m_iSceneHeight = iFrameHeight;

        //int iCount = 0;
        for(int i = 0; i <= m_iRoadCount; i++)
        {
            m_rgRoadLine[i].ptTop.x = rgRoadLine[i].ptTop.x * iFrameWidth / 100;
            m_rgRoadLine[i].ptTop.y = rgRoadLine[i].ptTop.y * iFrameHeight / 100;
            m_rgRoadLine[i].ptBottom.x = rgRoadLine[i].ptBottom.x * iFrameWidth / 100;
            m_rgRoadLine[i].ptBottom.y = rgRoadLine[i].ptBottom.y * iFrameHeight / 100;
            m_rgRoadLine[i].iRoadType = rgRoadLine[i].iRoadType;
        }

        //system("pause");

        //����ֱ�߷���
        for(int i = 0; i <= m_iRoadCount; i++)
        {
            int dx = (m_rgRoadLine[i].ptBottom.x - m_rgRoadLine[i].ptTop.x);
            int dy = (m_rgRoadLine[i].ptBottom.y - m_rgRoadLine[i].ptTop.y);

            if(dx == 0)
            {
                //k ������
                //x ����, y��չ
                m_rgRoadLine[i].ptTop.y = 0;
                m_rgRoadLine[i].ptBottom.y = iFrameHeight - 1;
            }
            else
            {
                float K = (float)dy / dx;
                float B = m_rgRoadLine[i].ptTop.y - K * m_rgRoadLine[i].ptTop.x;

                float UY = 0;
                float UX = (UY - B) / K;

                float DY = (float)(iFrameHeight - 1);
                float DX = (DY - B) / K;

                if(UX < 0)
                {
                    UX = 0;
                    UY = B;
                }

                if(DX < 0)
                {
                    DX = 0;
                    DY = B;
                }

                if(UX > iFrameWidth)
                {
                    UX = (float)(iFrameWidth - 1);
                    UY = UX * K + B;
                }

                if(DX > iFrameWidth)
                {
                    DX = (float)(iFrameWidth - 1);
                    DY = DX * K + B;
                }

                //����
                m_rgRoadLine[i].ptTop.x = (int)(UX + 0.5f);
                m_rgRoadLine[i].ptTop.y = (int)(UY + 0.5f);

                m_rgRoadLine[i].ptBottom.x = (int)(DX + 0.5f);
                m_rgRoadLine[i].ptBottom.y = (int)(DY + 0.5f);

            }
        }

        ResetROI();
        return S_OK;
    }

    // pa��������ʼ�㣬pb�������յ㣬�������ϵ�Y��X
    float CRoadInfo::iswGetLineX(HV_POINT pa, HV_POINT pb, int iLineY)
    {
        int tx = pa.x;
        int ty = pa.y;
        int bx = pb.x;
        int by = pb.y;

        int iDX1 = bx - tx;
        int iDY1 = by - ty;

        if(iDX1 == 0)
        {
            iDX1 = 1;
        }

        if(iDY1 == 0)
        {
            iDY1 = 1;
        }

        int iDY = iLineY - ty;
        return ((float)iDX1 / iDY1) * iDY + tx + 0.5f;
    }

    //////////////////////////////////////////////////////////////////////////
    int CRoadInfo::GetRoadNum(HV_POINT pt)
    {
        pt.x += m_rcROI.left;
        pt.y += m_rcROI.top;

        int iLineNum = -1;

        // ���������ˮƽ�����복�����ཻ�ĵ�
        for(int i = 0; i <= m_iRoadCount; i++)
        {
            // ��������Y��ÿ�����ϵ�X
            float fltX = iswGetLineX(m_rgRoadLine[i].ptTop, m_rgRoadLine[i].ptBottom, pt.y);

            if((int)fltX >= pt.x)
            {
                iLineNum = i - 1;
                break;
            }
        }

        return iLineNum;
    }

    int CRoadInfo::GetRoadWidth(HV_POINT pt, int* pRoadNum)
    {
        pt.x += m_rcROI.left;
        pt.y += m_rcROI.top;

        int iLineNum = -1;
        float fltX = -1.0f;
        float preFltX = -1.0f;
        int iCount = 0;

        // ���������ˮƽ�����복�����ཻ�ĵ�
        // ���������X�����������Xֵ
        for(int i = 0; i <= m_iRoadCount; i++, iCount++)
        {
            // ��������Y��ÿ�����ϵ�X
            fltX = iswGetLineX(m_rgRoadLine[i].ptTop, m_rgRoadLine[i].ptBottom, pt.y);

            if((int)fltX >= pt.x)
            {
                iLineNum = i - 1;
                break;
            }
            else
            {
                preFltX = fltX;
            }
        }

        // ������������Xֵ�����������
        float fltRoadWidth = fabs(fltX - preFltX);

        if(pRoadNum != NULL)
        {
            *pRoadNum = iLineNum;
        }

        // �����󳵵��⣬Ҳ�������ٳ������
        if(iLineNum == -1)
        {
            if(iCount > m_iRoadCount)
            {
                //�ұߵ�·�⣬Ӧ�������
                preFltX = iswGetLineX(m_rgRoadLine[m_iRoadCount - 1].ptTop, m_rgRoadLine[m_iRoadCount - 1].ptBottom, pt.y);
                fltRoadWidth = fabs(fltX - preFltX);
            }
            else if(iCount == 0)
            {
                //fltxΪ��һ����
                float fltLineX = iswGetLineX(m_rgRoadLine[1].ptBottom, m_rgRoadLine[1].ptTop, pt.y);
                fltRoadWidth = fabs(fltX - fltLineX);
            }
        }

        if(fltRoadWidth < 1e-6)
        {
            return 1;
        }

        return (int)(fltRoadWidth + 0.5f);
    }

    int CRoadInfo::GetRoadWidth(int iRoadNum, int iY)
    {
        int iWidth = GetRoadLineX(iRoadNum + 1, iY) - GetRoadLineX(iRoadNum, iY);

        if(iWidth < 0)
        {
            iWidth = 0;
        }

        return iWidth;
    }

    int CRoadInfo::GetRoadLineCount()
    {
        return m_iRoadCount + 1;
    }

    int CRoadInfo::GetRoadCount()
    {
        return m_iRoadCount;
    }

    int CRoadInfo::GetRoadType(int iRoadNum)
    {
        return ((iRoadNum >= 0) && (iRoadNum < GetRoadCount()))
               ?
               m_rgRoadLine[iRoadNum].iRoadType
               :
               -1;
    }

    // ���������������ϵ�Y��X
    int CRoadInfo::GetRoadLineX(int iLineNum, int iLineY)
    {
        iLineY += m_rcROI.top;
        return (iLineNum > m_iRoadCount)
               ?
               -1
               :
               (int)iswGetLineX(m_rgRoadLine[iLineNum].ptTop,
                                m_rgRoadLine[iLineNum].ptBottom,
                                iLineY);
    }

    int CRoadInfo::GetSceneWidth()
    {
        return m_iSceneWidth;
    }

    int CRoadInfo::GetSceneHeight()
    {
        return m_iSceneHeight;
    }

    // ���ù�ע���򣬼��ɲ��ü�ƫ�ƣ���ע���ú�Ҫ�����
    void CRoadInfo::SetROI(HV_RECT rcROI)
    {
        m_rcROI = rcROI;
    }

    HV_RECT CRoadInfo::GetROI()
    {
        return m_rcROI;
    }

    void CRoadInfo::ResetROI()
    {
        m_rcROI.left = 0;
        m_rcROI.top = 0;
        m_rcROI.right = m_iSceneWidth - 1;
        m_rcROI.bottom = m_iSceneHeight - 1;
    }

    int CRoadInfo::GetROIWidth()
    {
        return m_rcROI.right - m_rcROI.left + 1;
    }
    int CRoadInfo::GetROIHeight()
    {
        return m_rcROI.bottom - m_rcROI.top + 1;
    }
} // svEPApi

