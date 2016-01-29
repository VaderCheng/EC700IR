/// @file
/// @brief svIPDetApi����
/// @author ganzz
/// @date 2014/5/28 17:10:58
///
/// �޸�˵��:
/// [2014/5/28 17:10:58 ganzz] ����汾

#pragma once
#include "svBase/svBase.h"

// TODO: �����ռ��޸�Ϊ����ֵ
/// svIPDetApi�����ռ�
namespace svIPDetApi
{
    /// ��ȡȡ�汾��Ϣ
    const char* GetRevInfo();

    class DET_ROI : public sv::SV_RECT
    {
    public:
        int m_nType;
        int m_nSubType;
        float m_fltConf;
        sv::SV_UINT32 m_nFlag;
    };

    //ͨ�ü��ʵ��
    typedef struct _DET_PARAM
    {
        int nDetType; //��Ӧģ�͵ļ������
        int nDetWidth; //������ϵ��

        int nStepDivX;  //����ϵ��
        int nStepDivY;

        int nScaleNum;  //SCALE��
        float fltScaleCoef; //SCALEϵ��, nScaleNum>1��Ч
        int nMergeNum; //�ϲ���
        int nMergeOverlapRatio; //�ϲ��ص��ٷֱ�*100���ص��ʴ��ڸ�ֵ�����ϲ�

        int nMaxROI; //����ѡ�����

        _DET_PARAM()
        {
            nDetType = -1;
            nDetWidth = 256;

            nStepDivX = 16;
            nStepDivY = 8;

            nScaleNum = 1;
            fltScaleCoef = 0.9f;
            nMergeNum = 2;
            nMergeOverlapRatio = 90;

            nMaxROI = 1000;
        }
    }
    DET_PARAM;

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

    struct CTRL_PARAM
    {
        int nNightTH;  /// ����������ֵ
        int nEnvLightMaxCheckFrameCount;   /// ��������ÿ��ͳ�Ƶ�֡��
        /// ���������л���Ҫ�ﵽ��֡����ֵ��һ��ͳ���б��ж�Ϊĳ�����Ĵ������ڸ�ֵ���ܹ��л�
        int nEnvLightSwitchFramesTH;

        int nL2DetCandidateNum;     // ��������ѡ����
        int nL2DetVoteNum;          // �������ͶƱ������ѡ�������������ɳ����

        CTRL_PARAM()
            :   nNightTH(60)
            ,   nEnvLightMaxCheckFrameCount(300)
            ,   nEnvLightSwitchFramesTH(256)
            ,   nL2DetCandidateNum(6)
            ,   nL2DetVoteNum(1)
        {
        }
    };

    class CDetCtrl
    {
    public:
        enum LIGHT_TYPE
        {
            LT_DAY,     /// ����
            LT_NIGHT    /// ����
        };

        virtual ~CDetCtrl() {}
    public:

        //��ʼ���ӿڣ������ȳ�ʼ����ȷ���ɹ�
        virtual sv::SV_RESULT Init(CTRL_PARAM* pCtrlParam) = 0;

        //����ģ��
        virtual sv::SV_RESULT LoadModel(
            int nType,       // �����ͣ�0Ϊ���죬1Ϊ����
            int nSubType,    // ������
            sv::SV_UINT8* pModelDat,
            int nDataLen
        ) = 0;

        //���ض���ģ��
        //ע�⣺�ӿ���������ģ�����ݲ����ͷ�
        virtual sv::SV_RESULT LoadL2Model(
            int nType,       // �����ͣ�0Ϊ���죬1Ϊ����
            sv::SV_UINT8* pModelDat,
            int nDataLen
        ) = 0;

        //�ͷ�ģ��
        virtual sv::SV_RESULT ReleaseModel() = 0;

        //�ͷŶ���ģ��
        virtual sv::SV_RESULT ReleaseL2Model() = 0;

        //����һ֡
        virtual sv::SV_RESULT Process(
            sv::SV_IMAGE imgFrame,      //����ͼƬ
            int nType,                  //�������, 0Ϊ���죬1Ϊ����
            DET_PARAM* rgParam,         //��Ӧ���͵ļ�����
            DET_ROI* rgResult,          //�����
            int nMaxResult,
            int* pResultCount
        ) = 0;

        //���㻷������
        virtual sv::SV_RESULT CalcEnvLight(
            sv::SV_IMAGE imgFrame,      //����ͼƬ
            LIGHT_TYPE* pLightType,      //��ǰ��������
            int* pCurLight              //��ǰ��������
        ) = 0;

        /// �����ɼ��ӿ�
        virtual sv::SV_RESULT SetSampGather(
            ISampGather* pGather
        ) = 0;
    };


    CDetCtrl* CreateDetCtrl();
    void FreeDetCtrl(CDetCtrl* pCtrl);

} // svIPDetApi
