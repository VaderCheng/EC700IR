#ifndef __EP_TRACK_INFO_H__
#define __EP_TRACK_INFO_H__

#include "svEPApi\svEPApi.h" 
#include "swplatetype.h"
#include "swImageObj.h"
#include "trackerdef.h"
#include "VirtualRefImage.h"
#include "RoadInfo.h"

struct EPBestPlateInfo
{
    EPBestPlateInfo()
    {
        pimgPlate = NULL;
        fltConfidence = 0.f;
    }
    ~EPBestPlateInfo()
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
    }
    // ת�Ƶ���һ������ 
    HRESULT Detach(EPBestPlateInfo& dst)
    {
        dst = *this;
        pimgPlate = NULL;
        fltConfidence = 0.f;
        return S_OK;
    }
    IReferenceComponentImage *pimgPlate;
    float fltConfidence;
    HiVideo::CRect rcPlatePos;				// �ָ�����Χλ��
    DWORD32 dwFrameTime;					// ��ǰ֡ʱ��
    HiVideo::CRect rcPos;					// ����������ͼ���е�λ��
    BYTE8 rgbContent[8];
};

class CEPTrackInfo
{
public:
    enum TRACK_STATE
    {
        TS_FREE = -1,
        TS_NEW = 0,
        TS_TRACKING = 1,
        TS_RESET = 2
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

    CEPTrackInfo(void);
    ~CEPTrackInfo(void);

    virtual int GetID();
    virtual int GetState();
    virtual int GetType();
    virtual int GetPosCount();
    virtual HV_RECT GetPos(int nIndex);
    virtual HV_RECT GetLastPos();
    virtual void Reset();    // �����ü��ʶ��ģ��Ľ���������ͷű���������
    virtual CAR_COLOR GetColor();

    virtual BOOL GetPlate(svEPApi::EP_PLATE_INFO* pPlateInfo);
    virtual int GetPlateRecogCount();
    virtual float GetPlateAvgConf(); // ȡ����ƽ�����Ŷ�
    virtual float GetPlateAvgFirstConf(); // ȡ����ƽ�����ֿ��Ŷ�
    virtual HV_RECT GetLastPlateDetPos();    
    virtual HV_RECT GetLastPlateRecogPos();
    virtual int GetLastPlateRecogPosCount(); // ȡ��һ��ʶ���Ƶ������˶��켣����
    virtual int GetLastPlateDetPosCount(); // ȡ��һ�μ쵽�Ƶ������˶��켣����

    virtual float GetLastPlateConf();
    virtual float GetLastPlateFirstConf();

    virtual int GetVotedPlateTypeCount();
    /// ȡ��Ч����������������Ϣ���ɿ����Կ���Ϊ���Ƴ������ƻ���
    /// ��ʱ�ж���Ч�����������ϸ�(����>10)����ж�Ϊ���Ƴ����Ƴ�
    virtual int GetValidDetCount(); 

    /// ��ǰ����״̬�Ƿ�Ϊѹ�ߣ�����ֹͣ�߷����ж�
    virtual BOOL IsPressRoadLine(int* pnRoadNum = NULL); 
    /// ��ǰ����״̬�Ƿ�Ϊֹͣ
    virtual BOOL IsStop();  
    /// ��ǰ����״̬�Ƿ�����
    virtual BOOL IsReverseRun(); 
    /// ��ǰ�Ƿ�ΪԽ�� 
    virtual BOOL IsCrossRoadLine(int* pnRoadNum = NULL);

    virtual int GetRoadNum();

    /// ȡ��ʻ����
    virtual RUN_TYPE GetRunType();

    /// ȡ����������
    virtual int GetPlateSimilarityCount();

public:
    void Free();   // �ͷű���������
    virtual BOOL IsUpdated();                    // �Ƿ��Ѿ����¼����

    // ���㷨����������и���
    static HRESULT UpdateAll(
        CEPTrackInfo* rgDjObj,
        int iMaxObj,
        int* piObjCnt,  // �������
        svEPApi::IEPTrack** rgpTracker, 
        int iTrackerCnt
        );

    static void SetRoadInfo(CRoadInfo* pRoadInfo);   // ���ó�����Ϣ����������

    // ��ǰ֡��ʱ�䡣
    static DWORD32 s_iCurImageTick;

private:
    virtual void Update(svEPApi::IEPTrack* pIEPTrack);  // ʹ�ü��������״̬��Ϣ��
    // < ��DjTrackģ���Ӧ����
    DWORD32 m_dwID;
    svEPApi::IEPTrack* m_pEPTrack;
    // >
    TRACK_STATE m_nTrackState;
    TRACK_TYPE m_nTrackType;

    // �����ε��㷨�ӿ�ȡ���ƣ����ﱣ����һ֡������Ϣ
    svEPApi::EP_PLATE_INFO m_cLastPlate;  // ����Ϣ�ڸ��½���ж�ʱ�Զ�����  

    // ����������Ϣ����������ߵ���
    static CRoadInfo* m_pRoadInfo;

public:

    // ���Ƴ���ͼ
    EPBestPlateInfo m_rgBestPlateInfo[PLATE_TYPE_COUNT]; // Ϊÿ�����Ͷ�����һ���÷���ߵĳ���ͼ��
    IVirtualRefImage *m_pimgBestSnapShot;    // ��������ͼ
    IVirtualRefImage *m_pimgLastSnapShot;
    IVirtualRefImage *m_pimgBeginCapture;
    IVirtualRefImage *m_pimgBestCapture;
    IVirtualRefImage *m_pimgLastCapture;
    DWORD32 m_nStartFrameNo;
    DWORD32 m_nEndFrameNo;
	DWORD32 m_dwFirstFrameTime;

    DWORD32 m_nCarArriveTime;  // TODO ʵ�ָ�ֵ,��ʱ�ù�ֹͦ��ʱ��

    int m_nPressRoadLineNum;    // -1Ϊû�и��¼�
    int m_nCrossRoadLineNum;    // -1Ϊû�и��¼�
    int m_nReverseRunCount;     // ÿ����֡��һ������ 

    int m_iOnePosLightScene;
    int m_iTwoPosLightScene;
    int m_iThreePosLightScene;
    int m_iPassStopLightScene;
    int m_iPassLeftStopLightScene;


    HV_RECT m_rcBestPos;  // �����������û�ã�����ʱ�ȼ���m_rgRect[0]��˳�еȼ���m_rgRect[3]
    HV_RECT m_rcLastPos;  // �����������û�ã�����ʱ�ȼ���m_rgRect[1]��˳�еȼ���m_rgRect[4]
    ///ÿ��ͼƬ������
    ///2��3��4�ֱ�Ϊ����ץ��λ�ã��������ϸ��Ӧ�ӽ���Զ�����ߵ�λ�ã���˳�����޹� 
    ///0��1ԭ����ʾ����λ�ã��������Ѿ����ã�Ŀǰ��Ϊ�˴������
    HiVideo::CRect m_rgRect[5]; 

    // ����ƿ��Ŷȣ� ����ץ��
    float m_fltMaxPlateConf; 

    bool m_fTrackReliable; //�����Ƿ�ɿ���ֻ�пɿ��Ĳ��ǳ��������֮һ
    
    bool m_rgfIsCapTime[3]; //�Ƿ�֡ץ�� 

    //��һ�μ쵽�Ƶĳ�����Ϣ�������㷨���ܲ��ø�֡���ʶ����Ҫ��������
    //�������˸�֡���ʶ������һ֡Ϊ��⡢����п�����Ҫ������ѳ���ʱ����������Ч
    EPBestPlateInfo m_cLastDetPlateInfo;  

    RUN_TYPE m_nRunType; // ��ʻ���ͣ����ٽ�������Ч

    // ����λ�ù켣��Updateʱ�Զ����¡�
    // ע�⣺������ÿ֡����¼��ֻ�е��������ƶ�ʱ�Ż��¼����������ͬһ��λ��ʱ����¼
    static const int m_nMaxPlateMovePos = 64;
    int m_nPlateMovePosCount;
    HV_RECT m_rgrcPlateMovePos[m_nMaxPlateMovePos];
    // zhaopy ��¼ʱ��
    DWORD32 m_rgdwPlateTick[m_nMaxPlateMovePos];

    // ʶ����Ч֡��
    int nRecogInValidCount;
};

#endif // __EP_TRACK_INFO_H__

