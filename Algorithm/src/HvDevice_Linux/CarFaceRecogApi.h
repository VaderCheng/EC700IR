/// @file
/// @brief CarFaceRecogApi����
/// @author liaoy
/// @date 2014/6/6 10:23:18
///
/// �޸�˵��:
/// [2014/8/27 10:00:00 yangws] ������ȡ������ƥ������
/// [2014/8/18 14:00:00 yangws] ����SVMģ�ʹ���
/// [2014/6/6 10:23:18 liaoy] ����汾

#pragma once
#include "svBase/svBase.h"

/// CarFaceRecogApi�����ռ�
namespace CarFaceRecogApi
{
    /// ��ȡȡ�汾��Ϣ
    const char* GetRevInfo();

    /// ���������ṹ��
    struct SCAN_FACE_PARAM
    {
        /// ��������ʼ��չ����(10Bit����)
        int m_nInitLeftExtRatio[4];
        /// �����Ҳ��ʼ��չ����(10Bit����)
        int m_nInitRightExtRatio[4];
        /// ���Ƶײ���ʼ��չ����(10Bit����)
        int m_nInitBottomExtRatio[4];
        /// �����߶ȳ�ʼ��չ����(10Bit����)
        int m_nInitHeightExtRatio[4];
        /// �����ϲ���չ����(10Bit����)
        int m_nOutTopExtRatio[4];
        /// ���Ƶײ���չ����(10Bit����)
        int m_nOutBottomExtRatio[4];
        /// �����߿��(10Bit����)
        int m_nOutH2WRatio[4];

        /// ���캯��
        SCAN_FACE_PARAM()
        {
            m_nInitRightExtRatio[0] = m_nInitLeftExtRatio[0] = 1413;
            m_nInitRightExtRatio[1] = m_nInitLeftExtRatio[1] = 1843;
            m_nInitRightExtRatio[2] = m_nInitLeftExtRatio[2] = 1413;
            m_nInitRightExtRatio[3] = m_nInitLeftExtRatio[3] = 1843;

            m_nInitBottomExtRatio[0] = 512;
            m_nInitBottomExtRatio[1] = 205;
            m_nInitBottomExtRatio[2] = 512;
            m_nInitBottomExtRatio[3] = 0;

            m_nInitHeightExtRatio[0] = 3072;
            m_nInitHeightExtRatio[1] = 2560;
            m_nInitHeightExtRatio[2] = 3072;
            m_nInitHeightExtRatio[3] = 2560;

            m_nOutTopExtRatio[2] = m_nOutTopExtRatio[0] = 317;
            m_nOutTopExtRatio[3] = m_nOutTopExtRatio[1] = 317;

            m_nOutBottomExtRatio[2] = m_nOutBottomExtRatio[0] = 512;
            m_nOutBottomExtRatio[3] = m_nOutBottomExtRatio[1] = 512;

            m_nOutH2WRatio[2] = m_nOutH2WRatio[0] = 585;
            m_nOutH2WRatio[3] = m_nOutH2WRatio[1] = 630;
        }
    };

    class ICarFaceRecogCtrl
    {
    public:
        virtual ~ICarFaceRecogCtrl() {}
    public:
        /// ����ģ��
        /// @note 1. �ӿ���������ģ�����ݲ����ͷ�,�ڲ���ָ�����
        ///       2. ��ͬTYPE�ɼ��ز�ͬ��ģ��,�ϲ�����ʶ��.
        ///       3. ģ�����������Ϊ10��
        virtual sv::SV_RESULT LoadModel(
            int nTypeID,                    ///< ʶ�����ID
            sv::SV_UINT8* pModelBuf,        ///< ģ�ͻ���
            int nModelLen                   ///< ģ�ͻ��泤��
        ) = 0;

        /// ����svmģ��
        virtual sv::SV_RESULT LoadSvmModel(
            int nTypeID,                    ///< ʶ�����ID
            sv::SV_UINT8* pModelBuf,        ///< ģ�ͻ���
            int nModelLen                   ///< ģ�ͻ��泤��
        ) = 0;

        /// �ͷ�����ģ��
        virtual sv::SV_RESULT FreeModel(
        ) = 0;

        ///����ʶ��
        virtual sv::SV_RESULT RecogCarFace(
            sv::SV_IMAGE* pImgSrc,                    ///ԭͼ��
            sv::SV_BOOL fIsNight,        ///����ģʽ
            sv::SV_BOOL fNoPlate,     //�Ƿ������Ƴ�
            sv::SV_RECT* pPlatePos,        ///����λ��
            sv::SV_BOOL fYellowPlate,          ///������ɫ
            sv::SV_RECT* pCarFacePos,    ///����λ��
            int nIDCount,
            sv::SV_UINT32* rgRegID                   ///ʶ�����ID
        ) = 0;

        /// ��λ����
        /// @note ���Ƴ�����λ��Ϊʵ��λ��,����λ��ΪĬ��ֵ0,
        ///       ���Ƴ�����λ��ΪĬ��ֵ0������λ��Ϊ���λ��
        virtual sv::SV_RESULT ScanCarFaceApi(
            const sv::SV_IMAGE* pImage,         ///< ������ͼ
            sv::SV_RECT* pRect,                 ///< ����λ��
            const SCAN_FACE_PARAM* pScanFaceParam,///< ������ȡ����
            sv::SV_BOOL fYellowPlate,           ///< �Ƿ����
            int nEnvType,                       ///< ����ģʽ:����(0)/����(1)
            sv::SV_RECT* pRectFace              ///< ����λ��
        ) = 0;

        /// ʶ��
        /// @note ��ͬTYPE��ͼ��ߴ�Ҫ����һ��,����Ӱ������
        virtual sv::SV_RESULT RecogSamp(
            int nTypeID,                        ///< ʶ�����ID
            const sv::SV_IMAGE* pImgSamp,       ///< ��ʶ�������
            int nMaxMatch,                  ///< ���ƥ����
            sv::SV_UINT32* rgID,                ///< ���ID����
            sv::SV_UINT32* rgConf               ///< ������Ŷ�
        ) = 0;

        /// Svmʶ��
        /// @note ��ͬTYPE��ͼ��ߴ�Ҫ����һ��,����Ӱ������
        virtual sv::SV_RESULT RecogSampBySvm(
            int nTypeID,                        ///< ʶ�����ID
            const sv::SV_IMAGE* pImgSamp,       ///< ��ʶ�������
            int nMaxMatch,                  ///< ���ƥ����
            sv::SV_UINT32* rgID,                ///< ���ID����
            sv::SV_INT32* rgConf               ///< ������Ŷ�
        ) = 0;

        /// ��ȡ����
        /// @note *pnFeatBufLen�����ʼ��,��ʼֵΪ�趨����󳤶�,����ʵ�ʳ���
        virtual sv::SV_RESULT ExtractFeature(
            const sv::SV_IMAGE* pImgSamp,     ///< ����ͼ��
            sv::SV_BOOL fYellowPlate,         ///< �Ƿ����
            sv::SV_UINT8* pFeatBuf,           ///< �������ݻ���
            int* pnFeatBufLen                 ///< �������泤��
        ) = 0;

        /// ������׼
        virtual sv::SV_RESULT MatchFeatures(
            const sv::SV_UINT8* pFeatDat,    ///< ��������
            int nFeatDatLen,                 ///< �������ݳ���
            int nMaxMatch,                   ///< ���ƥ����
            sv::SV_UINT32* rgID,             ///< ���ID����
            sv::SV_INT32* rgConf             ///< ������Ŷ�
        ) = 0;
    };

    ICarFaceRecogCtrl* CreateCarFaceRecogCtrl();
    sv::SV_RESULT FreeCarFaceRecogCtrl(ICarFaceRecogCtrl* pCtrl);
} // CarFaceRecogApi
