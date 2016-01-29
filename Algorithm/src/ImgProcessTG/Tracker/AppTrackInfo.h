#pragma once

#include "svTgIrApi/svTgIrApi.h"
#include "swplatetype.h"
#include "swImageObj.h"
#include "trackerdef.h"
#include "VirtualRefImage.h"
#include "RoadInfo.h"

namespace swTgApp 
{

struct BestPlateInfo
{
    BestPlateInfo()
    {
        pimgPlate = NULL;
        fltConfidence = 0.f;
		nPlateSimilar = 0;
    }
    ~BestPlateInfo()
    {
    }
    void Clear()
    {
        fltConfidence = 0.f;
        if (pimgPlate != NULL)
        {
            pimgPlate->Release();
            pimgPlate = NULL;
        }
        rgbContent[0] = 0;
		nPlateSimilar = 0;
    }
    // ת�Ƶ���һ������ 
    HRESULT Detach(BestPlateInfo& dst)
    {
        dst = *this;
        pimgPlate = NULL;
        fltConfidence = 0.f;
		nPlateSimilar = 0;
        return S_OK;
    }
    IReferenceComponentImage *pimgPlate;
    float fltConfidence;
	int nPlateSimilar;						// ��ǰ�Ƶ����ƶ�
    HiVideo::CRect rcPlatePos;				// �ָ�����Χλ��
    DWORD32 dwFrameTime;					// ��ǰ֡ʱ��
    HiVideo::CRect rcPos;					// ����������ͼ���е�λ��
    BYTE8 rgbContent[8];
};

class CAppTrackInfo
{
public:
    enum TRACK_STATE
    {
        TS_FREE = -1,
        TS_NEW = 0,
        TS_TRACKING = 1,
        TS_END = 2
    };
    enum TRACK_TYPE
    {
        TT_UNKNOW = -1,
        TT_SMALL_CAR = 0,
        TT_MID_CAR = 1,
        TT_LARGE_CAR = 2,
        TT_BIKE = 3,
        TT_WAKL_MAN = 4
    };

    CAppTrackInfo(void);
    ~CAppTrackInfo(void);

    virtual unsigned int GetID();
    virtual TRACK_STATE GetState();
    virtual TRACK_TYPE GetType();
    virtual int GetPosCount();
    virtual sv::CSvRect GetPos(int nIndex);
    virtual sv::CSvRect GetLastPos();
    virtual const svTgIrApi::TG_TRACK_LOCUS& GetLocus(int nIndex);
    virtual const svTgIrApi::TG_TRACK_LOCUS& GetLastLocus();
    virtual const svTgIrApi::TG_PLATE_INFO& GetPlate(int nIndex);  // ȡ������ʶ�𵽵ĳ���
    virtual int GetPlateCount();
    virtual void End();    // ���������ʶ��ģ��Ľ���������ͷű���������

    /// ȡ�����Ϣ����������ʶ������������ΪTS_END״̬���ݲ���Ч��
    virtual sv::SV_RESULT GetResult(svTgIrApi::ITgTrack::TG_RESULT_INFO* pResInfo);

    /// ȡ������չ��Ϣ
    virtual sv::SV_RESULT GetExInfo(svTgIrApi::TG_TRACK_EX_INFO* pExInfo);   

//     /// ��ǰ����״̬�Ƿ�Ϊѹ�ߣ�����ֹͣ�߷����ж�
//     virtual sv::SV_BOOL IsPressRoadLine(int* pnRoadNum = NULL);
//     /// ��ǰ����״̬�Ƿ�Ϊֹͣ
//     virtual sv::SV_BOOL IsStop();
//     /// ��ǰ����״̬�Ƿ�����
//     virtual sv::SV_BOOL IsReverseRun();
//     /// ��ǰ�Ƿ�ΪԽ��
//     virtual sv::SV_BOOL IsCrossRoadLine(int* pnRoadNum = NULL);

    virtual int GetRoadNum();

public:
    void Free();   // �ͷű���������
    virtual sv::SV_BOOL IsUpdated();                    // �Ƿ��Ѿ����¼����

    // ���㷨����������и���
    static sv::SV_RESULT UpdateAll(
        CAppTrackInfo* rgDjObj,
        int iMaxObj,
        int* piObjCnt,  // �������
        svTgIrApi::ITgTrack** rgpTracker,
        int iTrackerCnt
    );

    static void SetRoadInfo(swTgApp::CRoadInfo* pRoadInfo);   // ���ó�����Ϣ����������

    // ��ǰ֡��ʱ�䡣
    static sv::SV_UINT32 s_iCurImageTick;

    svTgIrApi::ITgTrack* m_pTrack;

    // ���㷨�ӿڻ�ȡ�ĸ�����չ��Ϣ
    svTgIrApi::TG_TRACK_EX_INFO m_cExInfo;

    BOOL m_fCarArrived;
    BOOL m_fIsTrigger;
    DWORD32 m_nCarArriveRealTime;
    CRect m_rcCarArrivePos;
private:
    virtual void Update(svTgIrApi::ITgTrack* pITgTrack);  // ʹ�ü��������״̬��Ϣ��
    // < ��DjTrackģ���Ӧ����
    sv::SV_UINT32 m_dwID;

    // >
    TRACK_STATE m_nTrackState;
    TRACK_TYPE m_nTrackType;

    // ����������Ϣ����������ߵ���
    static swTgApp::CRoadInfo* m_pRoadInfo;

public:

    sv::CSvRect m_rcCurPos;

    // ���Ƴ���ͼ
    BestPlateInfo m_rgBestPlateInfo[svTgIrApi::PLATE_TYPE_COUNT]; // Ϊÿ�����Ͷ�����һ���÷���ߵĳ���ͼ��
    IVirtualRefImage *m_pimgBestSnapShot;    // ��������ͼ
    IVirtualRefImage *m_pimgLastSnapShot;

    sv::SV_UINT32 m_nStartFrameNo;
    sv::SV_UINT32 m_nEndFrameNo;
    sv::SV_UINT32 m_dwFirstFrameTime;

    sv::SV_UINT32 m_nCarArriveTime;  // TODO ʵ�ָ�ֵ,��ʱ�ù�ֹͦ��ʱ��

    int m_nReverseRunCount;     // ÿ����֡��һ������ 

    sv::CSvRect m_rcBestPos;   // ��Ӧm_pimgBestSnapShot
    sv::CSvRect m_rcLastPos;   // ��Ӧm_pimgLastSnapShot
    sv::CSvRect m_rcBestPlatePos;   // ��Ӧm_pimgBestSnapShot�������겻һ���У���-1Ϊ��Чֵ��
    sv::CSvRect m_rcLastPlatePos;   // ��Ӧm_pimgLastSnapShot�������겻һ���У���-1Ϊ��Чֵ��
    
    int m_nBestPlateSimilar;		// ��Ѵ�ͼ�������ƶ�
    int m_nLastPlateSimilar;		// ����ͼ�������ƶ�

    RUN_TYPE m_nRunType; // ��ʻ���ͣ����ٽ�������Ч

    // ����λ�ù켣��Updateʱ�Զ����¡�
    // ע�⣺������ÿ֡����¼��ֻ�е��������ƶ�ʱ�Ż��¼����������ͬһ��λ��ʱ����¼
    static const int m_nMaxPlateMovePos = 64;
    int m_nPlateMovePosCount;
    HV_RECT m_rgrcPlateMovePos[m_nMaxPlateMovePos];
    // zhaopy ��¼ʱ��
    DWORD32 m_rgdwPlateTick[m_nMaxPlateMovePos];

    bool m_fCanOutput;

    CAR_COLOR m_nCarColor;   // ���ٽ���ʱ��ΪͶƱ���

    // ��������ţ�ע��ΪӦ����أ�����ʼ��������֮��
    int m_nOutRoadNum;

    // ץ��ͼ�Ƿ����ƣ�Ŀǰֻ��԰�����Ч
    bool m_fCaptureFrameHavePlate;
    int m_nLastPlateY;    
	
    // Ҫ��¼ץ�ļ����������ʱʹ��
    DWORD32 m_dwTriggerCameraTimes;
};

}
