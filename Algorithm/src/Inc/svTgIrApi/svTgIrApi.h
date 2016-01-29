/// @file
/// @brief svTgIrApi����
/// @author ganzz
/// @date 2014/11/17 10:20:53
///
/// �޸�˵��:
/// [2014/11/17 10:20:53 ganzz] ����汾

#pragma once

#include "svBase/svBase.h"

/// ץ�Ļص������������Ƿ񴥷��ɹ���
typedef bool (*FUNC_CALLBACK_TRIIGER)(
    sv::SV_RECT rcPos,     /// �ص���λ��
    int nTrackID,          ///
    void* pVoid            /// �Զ�������
);

/// svTgIrApi�����ռ�
namespace svTgIrApi
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

    /// ������ɫ
    enum PLATE_COLOR
    {
        PC_UNKNOWN = 0,
        PC_BLUE,
        PC_YELLOW,
        PC_BLACK,
        PC_WHITE,
        PC_LIGHTBLUE,
        PC_GREEN,
        PC_COUNT
    };

    /// ��������
    enum PLATE_TYPE
    {
        PLATE_UNKNOWN = 0,                    //δ֪
        PLATE_NORMAL,                         //�������ƣ�����
        PLATE_WJ,                             //�侯����
        PLATE_POLICE,                         //�������е��л��ƣ�������Ƹ�ʽ��ͬ�ľ���
        PLATE_POLICE2,                        //��-A1234-���೵��
        PLATE_MILITARY,                       //����
        PLATE_DOUBLE_YELLOW,                  //˫�л��ƣ�˫�а���
        PLATE_DOUBLE_MOTO,                    //˫��Ħ����
        PLATE_INDIVIDUAL,                     //���Ի�����
        PLATE_DOUBLE_GREEN,                   //˫������
        PLATE_DOUBLE_WJ,                      //�侯����
        PLATE_MH,                             //�񺽳���
        PLATE_SHI,                            //ʹ����
        PLATE_DOUBLE_MILITARY,                //˫�����
        PLATE_TYPE_COUNT
    };

    //������ɫ
    typedef enum
    {
        CC_UNKNOWN = 0,         //δ֪
        CC_WHITE,               //��
        CC_GREY,                //��
        CC_BLACK,               //��
        CC_RED,                 //��
        CC_YELLOW,              //��
        CC_GREEN,               //��
        CC_BLUE,                //��
        CC_COUNT
    } CAR_COLOR;

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

    /// ���Ƽ������Ϣ
    /// ���ڲ�������Ϣ����һ��
    struct TG_PLATE_INFO
    {
        sv::CSvRect rcPos;          /// ���ʶ����λ��
        PLATE_COLOR nColor;
        PLATE_TYPE nPlateType;
        sv::SV_UINT8 rgbContent[8];     /// �����ַ���Ϣ
        sv::CSvRect rcPlatePos;         /// ���ʶ���Ʒָ�����Χλ�ã���rcPos��ʼ�����λ��
        int iHL;                        /// ����λ������
        sv::SV_UINT32 nVariance;        /// ���ƶԱȶ�
        sv::SV_UINT32 nAvgY;            /// ��������
        float fltDetConf;               /// ���ƿ��Ŷ�
        int nInValidCharCount;          /// ������ʶ����Ч���ַ���

        sv::SV_FLOAT rgfltConf[8];      /// �����ַ����Ŷ�����
        sv::SV_FLOAT fltTotalConf;      /// ���ƿ��Ŷ�

        TG_PLATE_INFO()
        {
        }
    };

    /// ÿ֡���ٿɻ�ȡ����չ��Ϣ
    struct TG_TRACK_EX_INFO
    {
        /// ���ڳ����ţ�����0��ͶƱǰΪ��ǰ֡�����ţ�ͶƱ����ͶƱΪ׼
        int nRoadNum;
        CAR_COLOR nCarColor;            /// ��ǰʶ����ɫ
        sv::SV_BOOL fIsTrackingMiss;    /// ��ǰ֡�Ƿ���ٶ�ʧ
        sv::CSvRect rcPredictCur;      /// ��ǰ֡Ԥ��λ�ã�һ��fIsTrackingMissΪTrueʱʹ��
        sv::CSvRect rcPredict80ms;      /// 80msԤ��λ��
        sv::CSvRect rcPredict160ms;     /// 160msԤ��λ��
    };

    /// �켣��λ
    struct TG_TRACK_LOCUS
    {
        sv::SV_RECT m_rcPos;
        TG_PLATE_INFO* m_pPlateInfo;  // ����ָ�룬����֡�첻����ʱΪ��
        int m_nPlateSimilarCnt;             //�����г��Ƶ�ͶƱ��������ƶ�
        sv::SV_UINT32 m_nFrameTime;
    };

    /// ������ٽ���ӿ�
    class ITgTrack
    {
    public:
        enum TRACK_STATE
        {
            TS_FREE = -1,   /// ������Ч״̬��������Σ�Ӧ�ò���ȡ����״̬
            TS_NEW = 0,     /// ��ʾ�����½����Ĳ�����״̬�����ǽ��״�����и�״̬
            TS_TRACKING = 1,   /// �������״̬
            TS_END = 2         /// ���ٽ���������������״̬��������֡�������
        };
        enum TRACK_TYPE
        {
            TT_UNKNOW = -1,
            TT_SMALL_CAR = 0,
            TT_MID_CAR = 1,
            TT_LARGE_CAR = 2,
            TT_BIKE = 3,        /// �ǻ�����
            TT_WALK_MAN = 4         /// ����
        };

        struct TG_RESULT_INFO
        {
            char szPlate[16];
            PLATE_COLOR nPlateColor;
            PLATE_TYPE nPlateType;
            sv::SV_UINT8 nPlateAvgLight;            /// ����ƽ������
            int nPlateVariance;
            sv::SV_UINT8 rgPlateContent[8];         /// �����ַ���Ϣ
            sv::SV_INT32 nPlateRecogInvalidCount;   /// ����ʶ����Ч�Ĵ���
            sv::SV_FLOAT fltPlateFirstConf;         /// ���ֿ��Ŷ�
            sv::SV_FLOAT fltPlateTotalConf;         /// ���ƿ��Ŷ�
            sv::SV_FLOAT fltPlateDetConf;

            int nPlateResSimilaryCount;  /// ���г����복��ͶƱ����������������ٽ�������Ч

            /// ȡͶƱ�õ��ĳ��������ڱ��������г����еļ����������ٽ�������Ч
            int nVotePlateTypeCount;

            ITgTrack::TRACK_TYPE nTrackType;  /// ��������

            int nValidDetCount;             /// ������Ч�����
            CAR_COLOR nCarColor;            /// ������ɫ
            sv::SV_FLOAT fltSpeed;          /// ����
        };
    public:
        virtual unsigned int GetID() = 0;

        virtual TRACK_STATE GetState() = 0;

        virtual TRACK_TYPE GetType() = 0;

        /// ��ȡ�켣��Ϣ������λ�á�����
        virtual const TG_TRACK_LOCUS& GetLocus(int nIndex) = 0;

        /// ȡ��ǰʱ�����һ���켣
        virtual sv::SV_RECT GetLastPos() = 0;

        /// ȡ�켣����
        virtual int GetLocusCount() = 0;

        /// ȡ��������
        virtual const TG_PLATE_INFO& GetPlate(int nIndex) = 0;

        /// ȡ��������
        virtual int GetPlateCount() = 0;

        /// ȡ��չ��Ϣ
        virtual sv::SV_RESULT GetExInfo(TG_TRACK_EX_INFO* pExInfo) = 0;

        /// �����ø��٣���ת�� TS_END ״̬������֡�����������
        virtual void End() = 0;

        /// ��ȡ�����Ϣ�����ڸ��ٽ���ʱ��TS_END״̬���Ż��и���Ϣ
        virtual sv::SV_RESULT GetResult(TG_RESULT_INFO* pResult) = 0;
    };

    // ����
    struct TG_PARAM
    {
        // ��������, �������
        int nDetTopLine;        /// �����ʼY���ٷֱ�*100
        int nDetBottomLine;     /// ������Y���ٷֱ�*100

        // ������ʶ����
        SV_ROAD_INFO rgRoadLine[5]; /// ������, Ŀǰ���ֻ֧��4����
        int nRoadLineCount;         /// ��������

        float fltXScale;
        float fltYScale;

        int nMergeOverlapRatio;  /// ���ϲ��� 0~100

        int nDuskMaxLightTH;
        int nNightMaxLightTH;

        // ������ز���
        int nDetMinScaleNum;           /// ���Ƽ������С���=56*1.1^nDetMinScaleNum��-1Ϊ�Զ�����
        int nDetMaxScaleNum;           /// ���Ƽ���������=56*1.1^nDetMaxScaleNum��-1Ϊ�Զ�����

        sv::SV_BOOL fEnableAlpha_5;     ///������ĸʶ�𿪹�
        float fltMinPlateConfTH;        /// ��ͳ��ƿ��Ŷ���ֵ
        sv::SV_BOOL fEnableGreenPlate;
        int nBlackPlate_S;  /// ���Ƶı��Ͷ�����
        int nBlackPlate_L;  /// ������������
        int nBlackPlateThreshold_H0;    /// ����ɫ�����ޣ����ڴ�ֵ���ж�Ϊ����
        int nBlackPlateThreshold_H1;    /// ����ɫ�����ޣ����ڴ�ֵ���ж�Ϊ����
        int nProcessPlate_LightBlue;    /// ǳ���ƿ���
        sv::SV_BOOL fEnableDoublePlate;     /// ˫���ƿ���
        sv::SV_BOOL fEnableShiGuanPlate;    /// ʹ��ʹ���Ƽ��

        // �����ٶȱ���ֵ��ÿ֡�ƶ�λ�����·��ٷֱ�*100��
        // �ǻ������ٶȵ��ڸ���ֵ����Ϊ����
        int nWalkManSpeedRatioTH;

        // ���Ƹ��������ռͼ��߶ȵİٷֱ�*100����30��Ϊ����30%���ܳ����ƽ��
        int nPlateTrackerOutLine;
        // ���������ģʽ��0:ֻҪû���߾Ͳ�����1:û���������л����Ų�������
        int nPlateOutLineMode;

        // �������ϴ���ץ����
        int nTriggerLine;

        /// ʹ���������
        sv::SV_BOOL fEnableCalcSpeed;
        // ����ʵ�ʿ��
        sv::SV_FLOAT fltRoadWidth;
        // ����ʵ�ʳ���(ͼ������Զ�˵�����˵ľ���)
        sv::SV_FLOAT fltRoadLength;

        /// ʹ�ܳ�����ɫʶ�𣬽�������Ч������������Ч
        sv::SV_BOOL fEnableRecogCarColor;

        TG_PARAM()
            :   nDetTopLine(30)
            ,   nDetBottomLine(75)
            ,   fltXScale(0.25f)
            ,   fltYScale(0.5f)
            ,   nRoadLineCount(0)
            ,   nMergeOverlapRatio(50)
            ,   nDuskMaxLightTH(70)
            ,   nNightMaxLightTH(20)

            ,   nDetMinScaleNum(-1)
            ,   nDetMaxScaleNum(-1)
            ,   fEnableAlpha_5(TRUE)
            ,   fltMinPlateConfTH(0.02f)
            ,   fEnableGreenPlate(FALSE)
            ,   nBlackPlate_S(10)
            ,   nBlackPlate_L(60)
            ,   nProcessPlate_LightBlue(1)
            ,   fEnableDoublePlate(1)
            ,   nBlackPlateThreshold_H0(100)
            ,   nBlackPlateThreshold_H1(200)
            ,   fEnableShiGuanPlate(FALSE)
            ,   nWalkManSpeedRatioTH(2)
            ,   nPlateTrackerOutLine(0)
            ,   nPlateOutLineMode(0)
            ,   nTriggerLine(60)
            ,   fEnableCalcSpeed(FALSE)
            ,   fltRoadWidth(3.75f)
            ,   fltRoadLength(55.0f)
            ,   fEnableRecogCarColor(FALSE)
        {
            memset(rgRoadLine, 0, sizeof(rgRoadLine));
        }
    };

    /// ���ƽӿ�
    class CTgCtrl
    {
    public:

        enum LIGHT_TYPE
        {
            LT_DAY,
            LT_DUSK,
            LT_NIGHT
        };

        struct TG_CTRL_INFO
        {
            /// ������������
            LIGHT_TYPE nLightType;
            int nAvgBrightness;
        };

        virtual ~CTgCtrl() {};

    public:
        /// ���س�����ģ�ͽӿ�
        virtual sv::SV_RESULT LoadDetModel(
            MOD_DET_INFO* pModDetInfo
        ) = 0;

        /// ����ʶ��ģ��
        virtual sv::SV_RESULT LoadRecogModel(
            char* szModelName,    //< ģ��������"CHNSF"
            void* pBuf,
            int nBufLen
        ) = 0;

        /// �ͷų�����ģ��
        virtual sv::SV_RESULT ReleaseDetModel() = 0;

        /// ��ʼ������
        virtual sv::SV_RESULT Init(const TG_PARAM* pParam) = 0;

        /// ����һ֡
        virtual sv::SV_RESULT Process(
            sv::SV_IMAGE imgFrame,      /// ����ͼƬ
            sv::SV_UINT32 nFrameTime,   /// ֡ʱ��
            DET_ROI* rgDetROI,          /// ���������������
            int nDetROICnt,             /// ��������������
            ITgTrack* rgpITrack[],      /// ���ؼ�����ӿ�����
            int nMaxITrack,             /// ��������ӿ�����
            int* pITrackCnt,            /// ���ؼ�����ӿ��������
            TG_CTRL_INFO* pInfo         /// ��ȡȫ����Ϣ
        ) = 0;

        // ʶ��һ֡������ץ��ʶ��
        virtual sv::SV_RESULT RecogOneFrame(
            sv::SV_IMAGE imgFrame,           /// ץ�ļ�ͼƬ
            sv::SV_UINT32 nFrameTime,        /// ֡ʱ��
            sv::SV_RECT rcPlateDetAear,      /// ���Ƽ������ʵ������
            TG_PLATE_INFO* rgPlateRes,       /// ��������ڼ쵽�����г���
            int nMaxPlateRes,                /// ����ƽ����
            int& nPlateResCnt,               /// ���صĳ��ƽ����
            int nDetMinScaleNum = -1,        /// ���Ƽ������С���=56*1.1^nDetMinScaleNum��-1Ϊ�Զ�����
            int nDetMaxScaleNum = -1         /// ���Ƽ���������=56*1.1^nDetMaxScaleNum��-1Ϊ�Զ�����
        ) = 0;

        virtual void SetTriggerCallBack(FUNC_CALLBACK_TRIIGER func, void* pVoid) = 0;

        // ǿ�ư���ģʽ����Processǰ���ã�Process���ú�ʧЧ����֡�����µ���
        virtual void ForceDuskEnvLightType() = 0;

        // ǿ�ư���ģʽ����Processǰ���ã�Process���ú�ʧЧ����֡�����µ���
        virtual void ForceDayEnvLightType() = 0;
    };

    /// �ܿش����ӿ�
    CTgCtrl* CreateTgCtrl();

    /// �ܿ��ͷŽӿ�
    void FreeTgCtrl(CTgCtrl* pCtrl);

    void GetPlateStr(
        const sv::SV_UINT8 rgbContent[],  // �����ַ�
        char* szName                // ����ĳ�����
    );
} // svTgIrApi

