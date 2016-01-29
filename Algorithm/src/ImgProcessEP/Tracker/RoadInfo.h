#ifndef _ROAD_INFO_H_
#define _ROAD_INFO_H_

#include "hvbaseobj.h"

namespace HiVideo
{
    // ������Ϣ�������ߣ�
    struct HV_ROAD_LINE_INFO
    {
        //���ߵ�����������
        HV_POINT ptTop;     // �������ϱ궨��ٷֱ�
        HV_POINT ptBottom;  // �������±궨��ٷֱ�
        int iRoadType;      // �����ұ߳��������ͣ�Ŀǰ��Ч
    } ;

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
        HRESULT Init(int iFrameWidth, int iFrameHeight, const HV_ROAD_LINE_INFO* rgRoadLine, int iRoadLineCnt);

        int GetRoadNum(HV_POINT pt);

        int GetRoadWidth(HV_POINT pt, int* pRoadNum);
        int GetRoadWidth(int iRoadNum, int iY);
        int GetRoadLineX(int iLineNum, int iLineY);

        int GetRoadLineCount();
        int GetRoadCount();
        int GetRoadType(int iRoadNum);

        int GetSceneWidth();       // ȡ���õ�ԭ������
        int GetSceneHeight();      // ȡ���õ�ԭ������

        // ���ù�ע���򣬼����ú������в����ɲ��ü�ƫ�ƣ���ע���ú�Ҫ�����
        void SetROI(HV_RECT rcROI);
        HV_RECT GetROI();
        void ResetROI();
        int GetROIWidth();
        int GetROIHeight();
    private:
        float iswGetLineX(HV_POINT pa, HV_POINT pb, int iLineY);
        HV_ROAD_LINE_INFO m_rgRoadLine[MAX_ROAD_INFO];
        int  m_iRoadCount;
        int m_iSceneWidth;
        int m_iSceneHeight;
        HV_RECT m_rcROI;
    };

}


#endif // _ROAD_INFO_H_
