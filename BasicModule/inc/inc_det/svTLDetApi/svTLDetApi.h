/// @file
/// @brief svTLDetApi����
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

/// svTLDetApi�����ռ�
namespace svTLDetApi
{
    /// ��ȡȡ�汾��Ϣ
    const char* GetRevInfo();

    struct TL_DET_PARAM
    {
        // ��������, �������
        int nRoadX00;   // ����X
        int nRoadY00;   // ����Y
        int nRoadX01;   // ����X
        int nRoadY01;   // ����Y
        int nRoadX10;   // ����X
        int nRoadY10;   // ����Y
        int nRoadX11;   // ����X
        int nRoadY11;   // ����Y

        int nStartDetLine;  // ��ʼ�����, ͼ��߶Ȱٷֱ�,0~100
        int nEndDetLine;    // ֹͣ�����, ͼ��߶Ȱٷֱ�, 0~100

        // for plate detection
        int nDetMinScaleNum;          /// ���Ƽ������С���=56*1.1^nDetMinScaleNum
        int nDetMaxScaleNum;          /// ���Ƽ���������=56*1.1^nDetMaxScaleNum

        TL_DET_PARAM()
            :   nDetMinScaleNum(5)   // ���Ƽ������С���=56*1.1^nDetMinScaleNum
            ,   nDetMaxScaleNum(9)  // ���Ƽ���������=56*1.1^nDetMaxScaleNum
        {
            nRoadX00 = 70;
            nRoadY00 = 50;
            nRoadX01 = 30;
            nRoadY01 = 75;
            nRoadX10 = 86;
            nRoadY10 = 50;
            nRoadX11 = 78;
            nRoadY11 = 75;
            nStartDetLine = 35;
            nEndDetLine = 65;
        }
    };

    ///������ɫ
    enum SV_PLATE_COLOR
    {
        PC_UNKNOWN = -1,    ///<δ֪
        PC_BLUE    = 0,     ///<��ɫ
        PC_BLACK   = 1,     ///<��ɫ
        PC_GREEN   = 2,     ///<��ɫ
        PC_YELLOW  = 3,     ///<��ɫ
        PC_WHITE   = 4      ///<��ɫ
    };

    ///��������
    enum SV_PLATE_TYPE
    {
        PT_UNKNOWN       = -1,       ///<δ֪
        PT_POLICE        = 0,        ///<��ͨ����
        PT_LOCALPOLICE   = 1,        ///<�ط�����
        PT_MILITARY      = 2,        ///<����
        PT_WJ            = 3,        ///<�侯��
        PT_MH            = 4,        ///<����
        PT_SHI           = 5,        ///<ʹ����
        PT_MOTO          = 6         ///<Ħ�г���
    };

    struct SV_PLATERECT
    {
        sv::CSvRect cPlatePos;           ///<����λ��SV_RECT
        sv::SV_INT32 nPlateLine;            ///<�����ַ�������1�������ƣ�2��˫����
        sv::SV_BOOL fWhiteChar;             ///<�ڰ��Ʊ�ʶ��0�����ƣ�1������
        SV_PLATE_COLOR nPlateColor;     ///<������ɫ��API��
        SV_PLATE_TYPE nPlateType;       ///<�������ͣ�
        sv::SV_UINT32 nVariance;            ///<���Ʒ�����ֵ
        sv::SV_INT32 nConf;                 ///<���ƿ��Ŷ�
        sv::SV_INT32 nFlag;                 ///<���Ʊ�ʶ���ۺ�ʹ��
    };

    /// �շ�վ���ƽӿ�
    class CTLDetCtrl
    {
    public:

        virtual ~CTLDetCtrl() {};

    public:
        /// ��ʼ������
        virtual sv::SV_RESULT Init(TL_DET_PARAM* pParam) = 0;

        /// ����һ֡
        virtual sv::SV_RESULT Process(
            sv::SV_IMAGE imgFrame,  /// ����ͼƬ
            int* pnAvgY,            /// �����ƽ������
            SV_PLATERECT* rgPlateROI,      /// ���س��ƽ���ӿ�����
            int nMaxPlateROI,       /// ����ƽ���ӿ�����
            int* pPlateROICnt          /// ���س��ƽ���ӿ��������
        ) = 0;
    };

    /// �ܿش����ӿ�
    CTLDetCtrl* CreateTLDetCtrl();

    /// �ܿ��ͷŽӿ�
    void FreeTLDetCtrl(CTLDetCtrl* pCtrl);

    //////////////////////////////////////////////////////////////////////////
    /// ģ���շ�վ���
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
            NIGHT_SMALL_CAR = 1,
            TYPE_COUNT = 2
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

    struct TL_CAR_DET_PARAM
    {
        // ��������, �������
        int nRoadX00;   // ����X
        int nRoadY00;   // ����Y
        int nRoadX01;   // ����X
        int nRoadY01;   // ����Y
        int nRoadX10;   // ����X
        int nRoadY10;   // ����Y
        int nRoadX11;   // ����X
        int nRoadY11;   // ����Y

        int nStartDetLine;  // ��ʼ�����, ͼ��߶Ȱٷֱ�,0~100
        int nEndDetLine;    // ֹͣ�����, ͼ��߶Ȱٷֱ�, 0~100

        float fltXScale;
        float fltYScale;

        int nNightEnvLightTh;  // ���ϻ���������ֵ�����ڸ�ֵ������

        TL_CAR_DET_PARAM()
            :   fltXScale(0.25f)
            ,   fltYScale(0.25f)
        {
            nRoadX00 = 70;
            nRoadY00 = 50;
            nRoadX01 = 30;
            nRoadY01 = 75;
            nRoadX10 = 86;
            nRoadY10 = 50;
            nRoadX11 = 78;
            nRoadY11 = 75;
            nStartDetLine = 35;
            nEndDetLine = 65;
            nNightEnvLightTh = 50;
        }
    };

    /// �շ�վ���ƽӿ�
    class CTLCarDetCtrl
    {
    public:

        virtual ~CTLCarDetCtrl() {};

    public:
        /// ��ʼ������
        virtual sv::SV_RESULT Init(TL_CAR_DET_PARAM* pParam) = 0;
        
        virtual sv::SV_RESULT LoadDetModel(
            MOD_DET_INFO* pParam
            ) = 0;
        virtual sv::SV_RESULT ReleaseDetModel() = 0;

        /// ����һ֡
        virtual sv::SV_RESULT Process(
            sv::SV_IMAGE imgFrame,  /// ����ͼƬ
            DET_ROI* rgDetROI,      /// ���ؼ�����ӿ�����
            int nMaxDetROI,         /// ��������ӿ�����
            int* pDetROICnt         /// ���ؼ�����ӿ��������
            ) = 0;
    };

    /// �ܿش����ӿ�
    CTLCarDetCtrl* CreateTLCarDetCtrl();

    /// �ܿ��ͷŽӿ�
    void FreeTLCarDetCtrl(CTLCarDetCtrl* pCtrl);

} // svTLDetApi
