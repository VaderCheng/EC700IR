#pragma once

#include "svBase/svBase.h"

namespace swTgApp
{
    // ������Ϣ�������ߣ�
    typedef struct _SV_ROAD_LINE_INFO
    {
        //���ߵ�����������
        sv::SV_POINT ptTop;     // �������ϱ궨��ٷֱ�
        sv::SV_POINT ptBottom;  // �������±궨��ٷֱ�
        int iRoadType;      // �����ұ߳��������ͣ�Ŀǰ��Ч
    } SV_ROAD_LINE_INFO;

    class CRoadInfo
    {
    public:
        enum
        {
            MAX_ROAD_INFO = 5
        };
    public:
        CRoadInfo();
        ~CRoadInfo();

        // rgRoadLine���������飬����� rgRoadLine Ϊ�ٷֱ�
        sv::SV_RESULT Init(int iFrameWidth, int iFrameHeight, const SV_ROAD_LINE_INFO* rgRoadLine, int iRoadLineCnt);

        int GetRoadNum(sv::SV_POINT pt);

        int GetRoadWidth(sv::SV_POINT pt, int* pRoadNum);
        int GetRoadWidth(int iRoadNum, int iY);
        int GetRoadLineX(int iLineNum, int iLineY);
        int GetRoadLineY(int iLineNum, int iLineX);

        int GetRoadLineCount();
        int GetRoadCount();
        int GetRoadType(int iRoadNum);

        int GetSceneWidth();       // ȡ���õ�ԭ������
        int GetSceneHeight();      // ȡ���õ�ԭ������

        // ���ù�ע���򣬼����ú������в����ɲ��ü�ƫ�ƣ���ע���ú�Ҫ�����
        void SetROI(sv::SV_RECT rcROI);
        sv::SV_RECT GetROI();
        void ResetROI();
        int GetROIWidth();
        int GetROIHeight();
        int GetRoadLinePointY0(); // ��ȡ������0����߽��������Yֵ
    private:
        float iswGetLineX(sv::SV_POINT pa, sv::SV_POINT pb, int iLineY);
        float iswGetLineY(sv::SV_POINT pa, sv::SV_POINT pb, int iLineX);
        SV_ROAD_LINE_INFO m_rgRoadLine[MAX_ROAD_INFO];
        int  m_iRoadCount;
        int m_iSceneWidth;
        int m_iSceneHeight;
        sv::SV_RECT m_rcROI;
        int m_nRoadLinePointY0;
    };

    // ֱ����
    class CSvLine
    {
    public:
        CSvLine();
        CSvLine(int x0, int y0, int x1, int y1);
        CSvLine(sv::SV_POINT pt0, sv::SV_POINT pt1);
        void Reset(int x0, int y0, int x1, int y1);
        void Reset(sv::SV_POINT pt0, sv::SV_POINT pt1);

        int GetLineX(int y);
        int GetLineY(int x);

        // y = K * x + B �е�ϵ�� �� x = K * y + B
        float m_fltK;
        float m_fltB;

    private:
        sv::SV_BOOL m_fKx;  // �Ƿ��� y = K * x + B ���ͣ������� x = K * y + B ����
    };

    /// ���μ������
    /// ��Ҫ����ȷ�����ҳ�����߽�
    struct SV_TRAP_AREA
    {
        //�ı���ɨ�������ĸ��������
        int nTopLeftX;
        int nTopLeftY;
        int nTopRightX;
        int nTopRightY;
        int nBottomLeftX;
        int nBottomLeftY;
        int nBottomRightX;
        int nBottomRightY;
        SV_TRAP_AREA()
        {
            nTopLeftX = 27;
            nTopLeftY = 24;
            nTopRightX = 67;
            nTopRightY = 24;
            nBottomLeftX = 7;
            nBottomLeftY = 100;
            nBottomRightX = 90;
            nBottomRightY = 100;
        }
    };

    class CSvTrapArea : private SV_TRAP_AREA
    {
    public:
        enum OUT_TYPE
        {
            OT_NOT_OUT = 0,
            OT_LEFT = 1,
            OT_TOP = 2,
            OT_RIGHT = 4,
            OT_BOTTOM = 8
        };
        CSvTrapArea();
        ~CSvTrapArea();

        /// ���������Ϊ�ٷֱ�*100
        sv::SV_RESULT Init(
            int nFrameWidth,   // ԭʼͼ���
            int nFrameHeight,  // ԭʼͼ���
            int nTopLeftX,
            int nTopLeftY,
            int nTopRightX,
            int nTopRightY,
            int nBottomLeftX,
            int nBottomLeftY,
            int nBottomRightX,
            int nBottomRightY
        );

        /// �Ƿ������������
        sv::SV_BOOL IsContainOfPoint(sv::SV_POINT pt);
        /// �Ƿ���������
        sv::SV_BOOL IsOutOfArea(sv::SV_POINT pt, int* pOutTypeBit = NULL);

        /// ȡ�������
        int GetSceneWidth();
        int GetSceneHeight();
        sv::SV_BOOL IsVaild();

    private:
        float m_fltSlopeLeft;
        float m_fltSlopeTop;
        float m_fltSlopeRight;
        float m_fltSlopeBottom;

        /// ��ʼ����ͼ����
        int m_nSceneWidth;
        int m_nSceneHeight;
        sv::SV_BOOL m_fInit;
    };

    class CSvLeftTurnArea
    {
    public:
        CSvLeftTurnArea();
        ~CSvLeftTurnArea();

        sv::SV_RESULT Init(sv::SV_POINT rgptLeftTurnWaitArea[2],
                           sv::SV_POINT rgRoadStopLine[2],  // ֹͣ��������
                           int nFrameWidth, int nFrameHeigh);

        sv::SV_BOOL IsOutOfArea(sv::SV_POINT ptCheck);

        // ��ת���Ƿ���Ч
        sv::SV_BOOL IsVaild();

    private:
        float m_fltB;
        float m_fltA;
        sv::SV_BOOL m_fInit;
    };

}
