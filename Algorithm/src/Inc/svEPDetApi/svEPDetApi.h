/// @file
/// @brief svEPDetApi����
/// @author ganzz
/// @date 2014/1/2 10:44:33
///
/// �޸�˵��:
/// [2014/1/2 10:44:33 ganzz] ����汾

#pragma once

#include "svBase/svBase.h"

/// ǰ�����������ݾɳ��Ƽ��ʶ��ģ�ͽӿ�
namespace HvCore
{
    struct IHvModel;  //#include "hvinterface.h"
}
struct RECTA;

/// svEPDetApi�����ռ�
namespace svEPDetApi
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
    };

    struct EP_DET_PARAM
    {
        // ��������, �������
        sv::SV_RECT rcDetArea;  /// ��Ƶ������������ԭͼ�����ٷֱ�*100��0~100
        int nTrackPos;

        // ������ʶ����
        SV_ROAD_INFO rgRoadLine[5]; /// ������, Ŀǰ���ֻ֧��4����
        int iRoadInfoCount;         /// ��������

        int nTh; //����ƽ��������ֵ

        float fltXScale;
        float fltYScale;

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

        int nPlateDetect_Green;     /// ����ʶ�𿪹�
        float fltPlateDetect_StepAdj;   /// ��������ϵ��

        EP_DET_PARAM()
            :   g_nDetMinScaleNum(6)   // ���Ƽ������С���=56*1.1^g_nDetMinScaleNum
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
        {
            nTrackPos = 70;

            rcDetArea.m_nLeft = 2;
            rcDetArea.m_nTop = 40;
            rcDetArea.m_nRight = 98;
            rcDetArea.m_nBottom = 99;

            fltXScale = 0.25f;
            fltYScale = 0.25f;
        }
    };

    /// �羯���ƽӿ�
    class CEPDetCtrl
    {
    public:

        enum LIGHT_TYPE
        {
            LT_DAY,
            LT_DUSK,
            LT_NIGHT
        };

        virtual ~CEPDetCtrl() {};

    public:
        /// ���س�����ģ�ͽӿ�
        virtual sv::SV_RESULT LoadDetModel(
            MOD_DET_INFO* pModDetInfo
            ) = 0;

        /// �ͷų�����ģ��
        virtual sv::SV_RESULT ReleaseDetModel() = 0;

        /// ��ʼ������
        virtual sv::SV_RESULT Init(EP_DET_PARAM* pParam) = 0;

        /// ���ó��Ƽ��ָ�ʶ���ģ�ͽӿ�
        virtual sv::SV_RESULT SetHvModel(HvCore::IHvModel* pHvModel) = 0;

        /// ����һ֡
        virtual sv::SV_RESULT Process(
            sv::SV_IMAGE imgFrame,  /// ����ͼƬ
            LIGHT_TYPE nLightTpye,  /// ������������
            DET_ROI* rgDetROI,      /// ���ؼ�����ӿ�����
            int nMaxDetROI,         /// ��������ӿ�����
            int* pDetROICnt,        /// ���ؼ�����ӿ��������
            RECTA* rgPlateROI,      /// ���س��ƽ���ӿ�����
            int nMaxPlateROI,       /// ����ƽ���ӿ�����
            int* pPlateROICnt          /// ���س��ƽ���ӿ��������
            ) = 0;
    };

    /// �羯�ܿش����ӿ�
    CEPDetCtrl* CreateEPDetCtrl();

    /// �羯�ܿ��ͷŽӿ�
    void FreeEPDetCtrl(CEPDetCtrl* pCtrl);

} // svEPDetApi
