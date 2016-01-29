/// @file
/// @brief svEPApi����
/// @author ganzz
/// @date 2013/10/28 9:07:38
///
/// �޸�˵��:
/// [2013/10/28 9:07:38 ganzz] ����汾

#pragma once

#include "svBase/svBase.h"
#include "swplatetype.h"

/// ǰ�����������ݾɳ��Ƽ��ʶ��ģ�ͽӿ�
namespace HvCore
{
    struct IHvModel;  //#include "hvinterface.h"
}
struct RECTA;

/// svEPApi�����ռ�
namespace svEPApi
{
    /// ��ȡȡ�汾��Ϣ
    const char* GetRevInfo();

    /// �������
    class DET_ROI : public sv::SV_RECT
    {
    public:
        int nType;
        int nSubType;
        float fltConf;
        sv::SV_UINT32 dwFlag;
    };

    /// �����ɼ��ӿڣ������ص���ʹ��
    class ISampGather
    {
    public:
        virtual sv::SV_RESULT OnDet(
            sv::SV_IMAGE* pImgFrame,
            sv::SV_RECT rcSamp
        ) = 0;

        virtual sv::SV_RESULT OnROI(
            sv::SV_IMAGE* pImgFrame,
            DET_ROI* pROI
        ) = 0;

        virtual sv::SV_RESULT OnResult(
            sv::SV_IMAGE* pImgFrame,
            DET_ROI* pROI
        ) = 0;
    };

    /// ����ģ�ͼ�����
    struct MOD_DET_INFO
    {
        /// ������ģ������
        enum MOD_DET_INFO_TYPE
        {
            TYPE_UNKNOW = -1,
            DAY_SMALL_CAR = 0,
            DAY_BIG_CAR = 1,
            NIGHT_SMALL_CAR = 2,
            NIGHT_BIG_CAR = 3,
            TYPE_COUNT = 4
        };

        unsigned char* pbData;
        int nDataLen;

        MOD_DET_INFO_TYPE nDetType; /// ��Ӧģ�͵ļ������
        float fltRoadRatio; /// ������ϵ�����복����İٷֱȣ�0.f~1.f

        int nStepDivX;  /// X����ϵ��
        int nStepDivY;  /// Y����ϵ��

        int nScaleNum;  // SCALE��
        float fltScaleCoef; /// SCALEϵ��, nScaleNum>1��Ч
        int nMergeNum; /// �ϲ���

        int nMaxROI; /// ����ѡ�����

        MOD_DET_INFO()
        {
            pbData = NULL;
            nDataLen = 0;

            nDetType = TYPE_UNKNOW;
            fltRoadRatio = 0.6f;

            nStepDivX = 16;
            nStepDivY = 8;

            nScaleNum = 1;
            fltScaleCoef = 0.9f;
            nMergeNum = 2;

            nMaxROI = 1000;
        }
    };

    /// ������Ϣ�������ߣ�
    struct SV_ROAD_INFO
    {
        sv::SV_POINT ptTop;     /// �������ϱ궨�㣬���ͼ���İٷֱ�x100��0~100
        sv::SV_POINT ptBottom;  /// �������±궨�㣬���ͼ���İٷֱ�x100��0~100
        int iRoadType;      /// �����ұ߳��������ͣ�Ŀǰ��Ч
    } ;

    /// ���μ������
    /// ��Ҫ����ȷ�����ҳ�����߽�
    struct SV_TRAP_TRACK_AREA 
    {
        //�ı���ɨ�������ĸ�������꣬�ٷֱ�
        int nTopLeftX;
        int nTopLeftY;
        int nTopRightX;
        int nTopRightY;
        int nBottomLeftX;
        int nBottomLeftY;
        int nBottomRightX;
        int nBottomRightY;
        SV_TRAP_TRACK_AREA()
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

    struct EP_PARAM
    {
        //  ��������, �������
        sv::SV_RECT rcDetArea;  /// ��Ƶ������������ԭͼ�����ٷֱ�*100��0~100
        SV_TRAP_TRACK_AREA cTrapTrackArea;  /// ���θ������򣬸��ٳ���������������
        int iTrackAreaPos;      /// ��������λ�ã������ԭͼ�����ٷֱ�*100��0~100

        // ������ʶ����
        SV_ROAD_INFO rgRoadLine[5]; /// ������, Ŀǰ���ֻ֧��4����
        int iRoadInfoCount;         /// ��������

        /// 0~3���µ��ϣ�����ץ���ߣ������ԭͼ�����ٷֱ�*100��0~100
        int rgiCapturePos[3];

        int nTh; /// ����ƽ��������ֵ

        // �������Ϳ�����·��ٷֱ�*100
        /// ���ͳ���С�����Գ�����ٷֱ�*100��0~100�����ڵ��ڸ�ֵ��С�ڴ��ͳ�ֵ��Ϊ���ͳ�
        int iMidCarMinWidthRatio;
        /// ���ͳ���С�����Գ�����ٷֱ�*100��0~100�����ڵ��ڸ�ֵ��Ϊ���ͳ�
        int iLargeCarMinWidthRatio;
        /// С�ͳ���С�����Գ�����ٷֱ�*100��0~100�����ڵ��ڸ�ֵ��С�ڴ��ͳ�ֵ��Ϊ���ͳ�
        int iSmallCarMinWidthRatio;
        /// �����ٶȱȣ�ƽ��ÿ֡�ƶ�λ�����·��ٷֱ�*100��0~100��������ڸ�ֵ��Ϊ�ǻ�����
        int iWalkManSpeedRatio;

        /// ���������ж�Scaleϵ��*100, �����ϵƹ�����첻ͬ�������Ŀ��С�����
        /// Ҳ�᲻ͬ���������Ϳ���жϲ��������Ը�ϵ���ټ��㡣
        /// ��80ʱ��iMidCarMinWidthRatio_night = iMidCarMinWidthRatio*80/100��Ĭ��80
        int iNightTypeScaleRatio;

        /// �������������ֵ��ƽ���������ȵ��ڸ�ֵ��Ϊ���ϣ�0~155
        int iNightMaxLightTH;
        /// �������������ֵ��ƽ���������ȵ��ڸ�ֵ�Ҹ���������ֵ��Ϊ��������Ϊ����
        int iDuskMaxLightTH;

        float fltXScale;  /// ���ͼ������ϵͳ
        float fltYScale;  /// ���ͼ������ϵͳ

        int iStopLinePos;  /// ֹͣ��λ��(ֱ��)�����ͼ��߶�*100��0~100
        int iLeftStopLinePos; /// ��ת��ͣ����λ����ֱ��ͣ���ߵĲ�ֵ�����ͼ��߶�*100��0~100.

        sv::SV_POINT rgptLeftTurnWaitArea[6];   /// ��ת��ת����������6����ȷ��

        // ԭPlateRecogParam����
        float g_fltTrackInflateX;       /// trackingʱ��һ֡������X�����ϵ����������trackrect_width�ı���
        float g_fltTrackInflateY;       /// trackingʱ��һ֡������Y�����ϵ����������trackrect_height�ı���

        int g_nContFrames_EstablishTrack;       /// of continuously observed frames for establishing a track
        int g_nMissFrames_EndTrack;             /// of missing frames for ending a track
        int g_nMissFrames_EndTrackQuick;        /// of missing frames for ending a track in EndTrackArea
        int g_nObservedFrames_ConsiderAsTrack;  /// of observed frames to be considered as a successful track
        float g_fltThreshold_StartTrack;        /// threshold of a good recognition for starting a track
        float g_fltThreshold_BeObservation;     /// threshold of a good recognition to be an observation
        float g_fltMinConfidenceForVote;        /// minimal confidence for voting, when there are not enought observations

        // for plate detection
        int g_nDetMinScaleNum;          /// ���Ƽ������С���=56*1.1^g_nDetMinScaleNum
        int g_nDetMaxScaleNum;          /// ���Ƽ���������=56*1.1^g_nDetMaxScaleNum
        int g_nDetMinStdVar;            /// ����>g_nDetMinStdVar���������Ϊ�����ǳ���
        int g_nMinStopPlateDetNum;      /// �ﵽ���������ֹͣ���
        sv::SV_BOOL g_fSubscanPredict;         /// trackingʱ�Ƿ�ʹ��Ԥ���㷨
        int g_nSubscanPredictMinScale;  /// trackingԤ��ʱscale�仯��Χ����Сֵ
        int g_nSubscanPredictMaxScale;  /// trackingԤ��ʱscale�仯��Χ�����ֵ

        float g_kfltPlateInflateRate;   /// ��������ı���
        float g_kfltPlateInflateRateV;  /// �������ߵı���
        float g_kfltHighInflateArea;    /// ���Ƶ�yֵ>g_kfltHighInflateArea���õڶ��׸���һ��ı������ͼ���
        float g_kfltPlateInflateRate2;  /// ����ļ�������ı���
        float g_kfltPlateInflateRateV2; /// ����ļ������ߵı���

        sv::SV_BOOL g_fSegPredict;             /// �Ƿ�ʹ���зֵ�Ԥ���㷨

        int g_nMinPlateWidth;           /// ����ʶ�����С���ƿ�ȣ��Էָ��Ľ��Ϊ׼��
        int g_nMaxPlateWidth;           /// ����ʶ�������ƿ�ȣ��Էָ��Ľ��Ϊ׼��
        sv::SV_BOOL g_fBestResultOnly;         /// ���ͼ�����ж��ʶ������ֻ�����õ�

        int g_nMinDownSampleWidthInSeg;     /// �ָ�ʱDownSample����С���
        int g_nMinDownSampleHeightInSeg;    /// �ָ�ʱDownSample����С�߶�
        // ԭPlateRecogParam����

        int fEnableProcessBWPlate;
        int fEnablePlateEnhance;
        int iPlateResizeThreshold;
        int fEnableBigPlate;
        int iEnableDefaultDBType;   ///Ĭ��˫�������Ϳ���
        int nDefaultDBType;         /// Ĭ��˫��������
        int iScrMidFiveCharWidth;   /// ��Ļ�м䵥�����ƺ����ֿ�� old m_iMiddleWidth
        int iScrBotFiveCharWidth;   /// ��Ļ�ײ��������ƺ����ֿ�� old m_iBottomWidth
        int fUseEdgeMethod;         /// ��ǿ����ͼʶ��

        int fEnableDefaultWJ;       ///"�������侯�ַ�����"
        char strDefaultWJChar[4];
        int fRecogGxPolice;         ///"�ط�����ʶ�𿪹�"
        int nDefaultChnId;          /// ���ػ����֣�58Ϊ����

        int nRemoveLowConfForVote;  /// ͶƱǰȥ���͵÷ֳ��ưٷֱ�
        int iMinVariance;           /// ���˷ǳ���������ֵ
        int iMinPlateBrightness;
        int iMaxPlateBrightness;

        // plate det cfg param
        int nPlateDetect_Green;     /// ����ʶ�𿪹�
        float fltPlateDetect_StepAdj;   /// ��������ϵ��

        // recog cfg param
        int fEnableAlphaRecog;  /// ��7λ��ĸʶ�𿪹�
        int fEnableT1Model;     /// T-1ģ�Ϳ���
        int fEnableAlpha_5;     /// ������ĸʶ�𿪹�

        // proc plate cfg param
        int nBlackPlate_S;      /// ���Ƶı��Ͷ�����
        int nBlackPlate_L;      /// ������������
        int nBlackPlateThreshold_H0;    /// ����ɫ������
        int nBlackPlateThreshold_H1;    /// ����ɫ������
        int nProcessPlate_LightBlue;    /// ǳ���ƿ���
        int fEnableDoublePlate;         /// ˫���ƿ���
        int iSegWhitePlate;             /// ǿ�ư��Ʒָ�

        int fEnableNewWJSegment;
        int fRecogNewMilitary;
        int fOutputYellowMidCar;        /// ����������ͳ�

        int fEnableDynamicDetMinStdVar;  /// ʹ�ܶ�̬�����С��׼��


        EP_PARAM()
            :   fEnableProcessBWPlate(TRUE)
            ,   fEnablePlateEnhance(TRUE)
            ,   iPlateResizeThreshold(120)
            ,   fEnableBigPlate(FALSE)
            ,   iEnableDefaultDBType(0)
            ,   nDefaultDBType(0)
            ,   iScrMidFiveCharWidth(0)
            ,   iScrBotFiveCharWidth(0)
            ,   fUseEdgeMethod(TRUE)
            ,   fEnableDefaultWJ(TRUE)
            ,   fRecogGxPolice(TRUE)
            ,   nDefaultChnId(58)
            ,   nRemoveLowConfForVote(40)
            ,   iMinVariance(9)
            ,   iMinPlateBrightness(80)
            ,   iMaxPlateBrightness(120)
            // ������ԭPlateRecogParam����
            ,   g_nDetMinScaleNum(6)   // ���Ƽ������С���=56*1.1^g_nDetMinScaleNum
            ,   g_nDetMaxScaleNum(10)  // ���Ƽ���������=56*1.1^g_nDetMaxScaleNum
            ,   g_nDetMinStdVar(16)    // ����>g_nDetMinStdVar���������Ϊ�����ǳ���
            ,   g_nMinStopPlateDetNum(0)      // �ﵽ���������ֹͣ���
            ,   g_fSubscanPredict(TRUE)       // trackingʱ�Ƿ�ʹ��Ԥ���㷨
            ,   g_nSubscanPredictMinScale(-2)    // trackingԤ��ʱscale�仯��Χ����Сֵ
            ,   g_nSubscanPredictMaxScale(1)     // trackingԤ��ʱscale�仯��Χ�����ֵ
            ,   g_kfltPlateInflateRate(0.05f)    // ��������ı���
            ,   g_kfltPlateInflateRateV(0.10f)   // �������ߵı���
            ,   g_kfltHighInflateArea(0.0f)     // ���Ƶ�yֵ>g_kfltHighInflateArea���õڶ��׸���һ��ı������ͼ���
            ,   g_kfltPlateInflateRate2(0.0f)   // ����ļ�������ı���
            ,   g_kfltPlateInflateRateV2(0.0f)  // ����ļ������ߵı���
            ,   g_fSegPredict(TRUE)             // �Ƿ�ʹ���зֵ�Ԥ���㷨
            ,   g_nMinPlateWidth(60) // ����ʶ�����С���ƿ�ȣ��Էָ��Ľ��Ϊ׼��
            ,   g_nMaxPlateWidth(200)           // ����ʶ�������ƿ�ȣ��Էָ��Ľ��Ϊ׼��
            ,   g_fBestResultOnly(FALSE)         // ���ͼ�����ж��ʶ������ֻ�����õ�
            ,   g_nMinDownSampleWidthInSeg(400) // �ָ�ʱDownSample����С���
            ,   g_nMinDownSampleHeightInSeg(52) // �ָ�ʱDownSample����С�߶�

            ,   nPlateDetect_Green(0)
            ,   fltPlateDetect_StepAdj(1.0f)
            ,   fEnableAlphaRecog(FALSE)
            ,   fEnableT1Model(FALSE)
            ,   fEnableAlpha_5(FALSE)
            ,   nBlackPlate_S(10)
            ,   nBlackPlate_L(85)
            ,   nProcessPlate_LightBlue(1)
            ,   fEnableDoublePlate(TRUE)
            ,   nBlackPlateThreshold_H0(100)
            ,   nBlackPlateThreshold_H1(200)
            ,   iSegWhitePlate(0)
            ,   fOutputYellowMidCar(TRUE)
            ,   fEnableDynamicDetMinStdVar(TRUE)
        {
            rcDetArea.m_nLeft = 2;
            rcDetArea.m_nTop = 40;
            rcDetArea.m_nRight = 98;
            rcDetArea.m_nBottom = 99;

            iTrackAreaPos = 55;

            rgRoadLine[0].ptTop.m_nX = 19;
            rgRoadLine[0].ptTop.m_nY = 51;
            rgRoadLine[0].ptBottom.m_nX = 2;
            rgRoadLine[0].ptBottom.m_nY = 79;
            rgRoadLine[1].ptTop.m_nX = 42;
            rgRoadLine[1].ptTop.m_nY = 45;
            rgRoadLine[1].ptBottom.m_nX = 31;
            rgRoadLine[1].ptBottom.m_nY = 93;
            rgRoadLine[2].ptTop.m_nX = 59;
            rgRoadLine[2].ptTop.m_nY = 42;
            rgRoadLine[2].ptBottom.m_nX = 65;
            rgRoadLine[2].ptBottom.m_nY = 92;
            rgRoadLine[3].ptTop.m_nX = 74;
            rgRoadLine[3].ptTop.m_nY = 43;
            rgRoadLine[3].ptBottom.m_nX = 95;
            rgRoadLine[3].ptBottom.m_nY = 81;
            iRoadInfoCount = 4;

            nTh = 4;

            // 0~3���µ��ϣ�����ץ���ߣ������ԭͼ�����ٷֱ�*100
            rgiCapturePos[0] = 85;
            rgiCapturePos[1] = 70;
            rgiCapturePos[2] = 40;

            // ���������·�����ٷֱ�*100
            iLargeCarMinWidthRatio = 95;
            iMidCarMinWidthRatio = 85;
            iSmallCarMinWidthRatio = 42;

            // �����ٶȱȣ�ÿ֡�ƶ�λ�����·��ٷֱ�*100
            iWalkManSpeedRatio = 4;

            iNightTypeScaleRatio = 80;

            iNightMaxLightTH = 22;
            iDuskMaxLightTH = 45;

            fltXScale = 0.25f;
            fltYScale = 0.25f;

            iStopLinePos = 65;
            iLeftStopLinePos = 65;

            memcpy(strDefaultWJChar, "16", 3);
        }
    };

    /// ���Ƽ������Ϣ
    struct EP_PLATE_INFO
    {
        sv::SV_RECT rcPos;          /// ���ʶ����λ��
        PLATE_COLOR color;
        PLATE_TYPE nPlateType;
        sv::SV_UINT8 rgbContent[8];     /// �����ַ���Ϣ
        sv::SV_RECT rcPlatePos;         /// ���ʶ���Ʒָ�����Χλ�ã���rcPos��ʼ�����λ��
        int iHL;                        /// ����λ������
        sv::SV_UINT32 nVariance;        /// ���ƶԱȶ�
        sv::SV_UINT32 nAvgY;            /// ��������

        sv::SV_UINT32 nRecogTime;  /// ���ʶ���Ƶ�ʱ��

        /// ʶ���������������еĹ켣�±꣬�������ڼ���˾�һ��ʶ��
        /// ע�⣺���ø�֡ʶ�����ʱ��ʶ���ڼ���һ֡���У��Ҹ�֡�����м�⣬
        ///       ʶ��֡��nRecogPosIndexֻ��nDetPosIndexһ�£������ǵ��ڹ켣��-1��
        ///       ����ֵֻ����ʶ�������켣�±�Ĺ�ϵ����켣����ֱ�����
        int nRecogPosIndex;
        /// ��⳵�������������еĹ켣�±꣬��켣��-1�����ǰ֡�쵽��
        int nDetPosIndex;
        sv::SV_RECT rcDetPos;  /// �쵽����λ�ã�ע��쵽��һ��ʶ��

        float fltConf;       /// ���ƿ��Ŷȣ����ٽ�����ΪͶƱ���ƽ�����Ŷ�
        float fltFirstConf;  /// ���ֿ��Ŷȣ����ٽ�����ΪͶƱ���ƽ�����ֿ��Ŷ�

        int nSimilaryCount;  /// ����ʶ���������������ٽ�������Ч

        /// ȡͶƱ�õ��ĳ��������ڱ��������г����еļ����������ٽ�������Ч
        int nVotePlateTypeCount;

        char szPlate[16];   /// ���ճ����ַ����������ٽ�������Ч
        int nRecogInValidCount;  /// ʶ����Ч�����������ٽ�������Ч

        int nPlatePosXOffset; /// ����λ��X����ƫ�ưٷֱȣ�-100~100��0Ϊ���м䣬����ѹ���ж�

        EP_PLATE_INFO()
            :   color(PC_UNKNOWN)
            ,   nPlateType(PLATE_UNKNOWN)
            ,   iHL(0)
            ,   nVariance(0)
            ,   nAvgY(0)
            ,   nRecogTime(0)
            ,   nRecogPosIndex(-1)
            ,   nDetPosIndex(-1)
            ,   fltConf(0.f)
            ,   fltFirstConf(0.f)
            ,   nSimilaryCount(0)
            ,   nVotePlateTypeCount(0)
        	,   nRecogInValidCount(0)
            ,   nPlatePosXOffset(0)
        {
            rcPos.m_nLeft = rcPos.m_nRight
                            = rcPos.m_nTop = rcPos.m_nBottom = 0;
            rcDetPos.m_nLeft = rcDetPos.m_nRight
                               = rcDetPos.m_nTop = rcDetPos.m_nBottom = 0;
            memset(rgbContent, 0, sizeof(rgbContent));
            szPlate[0] = '\0';
        }
    };

    /// �羯���ٽ���ӿ�
    class IEPTrack
    {
    public:
        enum TRACK_STATE
        {
            TS_FREE = -1,   /// ������Ч״̬
            TS_NEW = 0,     /// ��ʾ�����½����Ĳ�����״̬�����ǽ��״�����и�״̬
            TS_TRACKING = 1,   /// �������״̬
            TS_RESET = 2       /// ���ٽ���������������״̬��������֡�������
        };
        enum TRACK_TYPE
        {
            TT_UNKNOW = -1,
            TT_SMALL_CAR = 0,
            TT_MID_CAR = 1,
            TT_LARGE_CAR = 2,
            TT_BIKE = 3,
            TT_WALK_MAN = 4
        };

    public:
        virtual int GetID() = 0;
        virtual int GetState() = 0;
        virtual int GetType() = 0;
        virtual int GetPosCount() = 0;
        virtual sv::SV_RECT GetPos(int nIndex) = 0;
        virtual sv::SV_RECT GetLastPos() = 0;
        virtual void Reset() = 0;

        /// ��ȡ����
        virtual bool GetPlate(
            EP_PLATE_INFO* pPlateInfo /// ����ָ��
        ) = 0;
        virtual int GetPlateRecogCount() = 0;

        /// ȡ��Ч����������������Ϣ���ɿ����Կ���Ϊ���Ƴ������ƻ���
        /// ��ʱ�ж���Ч�����������ϸ�(����>10)����ж�Ϊ���Ƴ����Ƴ�
        virtual int GetValidDetCount() = 0;

        /// ���ڳ����ţ�����0��ͶƱǰΪ��ǰ֡�����ţ�ͶƱ����ͶƱΪ׼
        virtual int GetRoadNum() = 0;

        /// ȡ������ɫ
        virtual CAR_COLOR GetColor() = 0;

#if SV_RUN_PLATFORM == SV_PLATFORM_WIN
        virtual sv::SV_BOOL IsModelDetRes()
        {
            return TRUE;
        }
#endif
    };

    struct EP_RESULT_INFO
    {
        /// ������������
        enum LIGHT_TYPE
        {
            LT_DAY,     /// ����
            LT_NIGHT    /// ����
        };
        LIGHT_TYPE nLightType;
        int nAvgBrightness;
    };

    /// �ı���Ϣ����ص�ԭ��
    typedef int (*EP_CALLBACK_CAPTURE)(
        svEPApi::IEPTrack* pTrack,     ///< ����ָ��
        int nCapPosIndex      ///< ץ��λ���±�:0��1��2���ӽ���Զ
    );

    /// �羯���ƽӿ�
    class CEPCtrl
    {
    public:

        virtual ~CEPCtrl() {};

    public:
        /// ���س�����ģ�ͽӿ�
        virtual sv::SV_RESULT LoadDetModel(
            MOD_DET_INFO* pModDetInfo
        ) = 0;

        /// �ͷų�����ģ��
        virtual sv::SV_RESULT ReleaseDetModel() = 0;

        /// ��ʼ������
        virtual sv::SV_RESULT Init(EP_PARAM* pParam) = 0;

        /// ���ó��Ƽ��ָ�ʶ���ģ�ͽӿ�
        virtual sv::SV_RESULT SetHvModel(HvCore::IHvModel* pHvModel) = 0;

        /// ����һ֡
        virtual sv::SV_RESULT Process(
            sv::SV_IMAGE imgFrame,  /// ����ͼƬ
            sv::SV_UINT32 dwFrameTimeMs,  /// ��֡ʱ�꣬��ȷ������
            IEPTrack** rgTrack,     /// ���ظ��ٽ���ӿ�����
            int nMaxTrack,          /// �����ٽ���ӿ�����
            int* pTrackCount,       /// ���ظ��ٽ���ӿ��������
            EP_RESULT_INFO* pResultInfo = NULL  /// �����Ϣ����������Ͷ��壬��Ϊ��
        ) = 0;

        /// ����һ֡���첽���Ƶ��ô˽ӿڣ��贫��������򡢳��Ƽ���
        virtual sv::SV_RESULT ProcessTrack(
            sv::SV_IMAGE imgFrame,  /// ����ͼƬ
            sv::SV_UINT32 dwFrameTimeMs,  /// ��֡ʱ�꣬��ȷ������
            DET_ROI* rgDetROI,      /// �����������
            int nDetROICnt,         /// ����������
            RECTA* pPlateROI,       /// ���Ƽ�������
            int nPlateROICnt,       /// ���Ƽ������
            IEPTrack** rgTrack,     /// ���ظ��ٽ���ӿ�����
            int nMaxTrack,          /// �����ٽ���ӿ�����
            int* pTrackCount,       /// ���ظ��ٽ���ӿ��������
            EP_RESULT_INFO* pResultInfo = NULL  /// �����Ϣ����������Ͷ��壬��Ϊ��
        ) = 0;

        /// ץ�Ļص��ӿ�
        virtual sv::SV_RESULT SetCaptureCallBack(
            EP_CALLBACK_CAPTURE pFuncCallBack
        ) = 0;

        /// �����ɼ��ӿ�
        virtual sv::SV_RESULT SetSampGather(
            ISampGather* pGather
        ) = 0;

    };

    /// �羯�ܿش����ӿ�
    CEPCtrl* CreateEPCtrl();

    /// �羯�ܿ��ͷŽӿ�
    void FreeEPCtrl(CEPCtrl* pCtrl);

} // svEPApi
