/// @file
/// @brief svTgIrDetApi����
/// @author ganzz
/// @date 2014/11/17 10:35:42
///
/// �޸�˵��:
/// [2014/11/17 10:35:42 ganzz] ����汾

#pragma once


#include "svBase/svBase.h"

// TODO: �����ռ��޸�Ϊ����ֵ
/// svTgIrDetApi�����ռ�
namespace svTgIrDetApi
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
            TYPE_UNKNOW =       -1,
            DAY_SMALL_CAR =     0,
            DAY_LARGE_CAR =     1,
            DUSK_SMALL_CAR =    2,
            DUSK_LARGE_CAR =    3,
            DUSK_SMALL_CAR_EX = 4,
            DUSK_LARGE_CAR_EX = 5,
            NIGHT_SMALL_CAR =   6,
            NIGHT_LARGE_CAR =   7,
            //BIKE_AND_WALKMAN =    8,
            //BIKE_AND_WALKMAN_EX = 9,
            TYPE_COUNT =        8
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

    struct TG_DET_PARAM
    {
        // ��������, �������
        int nDetTopLine;  // �����ʼY
        int nDetBottomLine;    // ������Y

        // ������ʶ����
        SV_ROAD_INFO rgRoadLine[5]; /// ������, Ŀǰ���ֻ֧��4����
        int nRoadLineCount;         /// ��������

        float fltXScale;
        float fltYScale;

        int nMergeOverlapRatio;  /// ���ϲ��� 0~100

        sv::SV_BOOL fEnableWalkManDet;      /// ���˼��ǻ�������⿪��

        sv::SV_BOOL fEnableDynamicDetStep;      /// ʹ�ܶ�̬��ⲽ��

        TG_DET_PARAM()
        {
            nDetTopLine = 30;
            nDetBottomLine = 75;

            fltXScale = 0.25f;
            fltYScale = 0.5f;

            nRoadLineCount = 0;
            memset(rgRoadLine, 0, sizeof(rgRoadLine));

            nMergeOverlapRatio = 50;
            fEnableWalkManDet = FALSE;
            fEnableDynamicDetStep = TRUE;
        }
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

    /// ���ƽӿ�
    class CTgDetCtrl
    {
    public:

        enum LIGHT_TYPE
        {
            LT_DAY,
            LT_DUSK,
            LT_NIGHT
        };

        virtual ~CTgDetCtrl() {};

    public:
        /// ���س�����ģ�ͽӿ�
        virtual sv::SV_RESULT LoadDetModel(
            MOD_DET_INFO* pModDetInfo
        ) = 0;

        /// �ͷų�����ģ��
        virtual sv::SV_RESULT ReleaseDetModel() = 0;

        /// ��ʼ������
        virtual sv::SV_RESULT Init(const TG_DET_PARAM* pParam) = 0;

        /// ����һ֡
        virtual sv::SV_RESULT Process(
            sv::SV_IMAGE imgFrame,  /// ����ͼƬ
            LIGHT_TYPE nLightTpye,  /// ������������
            DET_ROI* rgDetROI,      /// ���ؼ�����ӿ�����
            int nMaxDetROI,         /// ��������ӿ�����
            int* pDetROICnt         /// ���ؼ�����ӿ��������
        ) = 0;

        /// �����ɼ��ӿ�
        virtual sv::SV_RESULT SetSampGather(
            ISampGather* pGather
        ) = 0;

        // ǿ�ư���ģʽ����Processǰ���ã�Process���ú�ʧЧ����֡�����µ���
        virtual void ForceDuskEnvLightType() = 0;
    };

    /// �羯�ܿش����ӿ�
    CTgDetCtrl* CreateTgDetCtrl();

    /// �羯�ܿ��ͷŽӿ�
    void FreeTgDetCtrl(CTgDetCtrl* pCtrl);

} // svTgIrDetApi
