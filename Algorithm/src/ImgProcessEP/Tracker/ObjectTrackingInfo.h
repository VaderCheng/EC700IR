#ifndef _OBJECTTRACKING_INFO_H_
#define _OBJECTTRACKING_INFO_H_

#include "swImageObj.h"
#include "hvBaseObj.h"

#include "hvinterface.h"
#include "Tracker.h"
#include "ObjMatch.h"
#include "HvBaseType.h"

using namespace HiVideo;

struct CarSizeInfoObj
{
	HiVideo::CRect rcFG;		// ������Χ����(������Ƶ���)
	float fltCarW;				// ����
	float fltCarH;				// ����
};

const int MAX_PLATETRACK_COUNT = 30;
const int MAX_OBJINFO_COUNT = 100;
const int PLATE_WIDTH = 70;

class CObjInfo
{
public:
	CObjInfo()
		:m_nFrameNo(0)
		,m_dwFrameTime(0)
		,m_rcFG(0, 0, 0, 0)
		,m_nCarType(CT_UNKNOWN)
		,m_nCarW(0)
		,m_nCarH(0)
		,m_fLight(FALSE)  // zhaopy 0624
	{
	}
	~CObjInfo()
	{
	}
	HRESULT Clear()
	{
		m_nFrameNo = 0;
		m_dwFrameTime = 0;
		m_rcFG = CRect(0, 0, 0, 0);
		m_nCarType = CT_UNKNOWN;
		m_nCarW = m_nCarH = 0;
		return S_OK;
	}
	UINT m_nFrameNo;						// ��ǰ֡��
	DWORD32 m_dwFrameTime;					// ��ǰ֡ʱ��
	CRect m_rcFG;							// ǰ��λ�ô�С
	CAR_TYPE m_nCarType;					// ����
	int m_nCarW;							// ����
	int m_nCarH;							// ����
	BOOL m_fLight;                          // �Ƿ񳵵�	 zhaopy 0624
};

typedef enum _OBJ_TRACK_STATE
{
	OBJ_TRACK_INIT,
	OBJ_TRACK_NEW,
	OBJ_TRACK_MOVING,
    OBJ_TRACK_TEM,
    OBJ_TRACK_LK,
	OBJ_TRACK_END
} 
OBJ_TRACK_STATE;

class CObjTrackInfo
{
public:
	int m_iVideoID;
	OBJ_TRACK_STATE m_objTrackState;
	BOOL m_fTrack;
	int m_rgiPlateTrackID[MAX_PLATETRACK_COUNT];
	int m_cPlateTrackID;
	bool m_fCarArrived;
	UINT m_nCarArriveTime;
	UINT m_nCarArriveRealTime;
	CRect m_rcCarArrivePos;
	int m_iRoad;
	bool m_fVoted;
	int m_iMissingCount;
	int m_iObjInfoCount;
	CObjInfo m_rgObjInfo[MAX_OBJINFO_COUNT];
	CAR_TYPE m_nVotedCarType;							// ͶƱ��ĳ�����
	int m_nCarArrivedCount;								// ��¼���㳵�������ƵĴ���
	bool m_fIsMan;										// ���˱�־
	int m_iObjFrameCount;								//��֡��
	float m_rgfltFrameSpeed[200];							//ͨ�����ص��λ�Ʋ�����һ���ٶ�
    BOOL m_fEPOutput;                                                               //���Ƶ羯���
    BOOL m_fNewTem;
    int m_iTemYBegin;                                   //�׸�ģ��y�����ʼֵ
    BOOL m_fFirstPredict;
    int m_iEPNoPlateScore;                              //���Ƴ�����,��ģ���й�
    int m_iEPDetSkipFrames;                             //��Ծ�ļ�����
    int m_iTrackID;

	IReferenceComponentImage *m_pFirstRefImage;
	IReferenceComponentImage *m_pBestReftImage;
	IReferenceComponentImage *m_pLastRefImage;
	CRect m_rcFirstCarPos;
	CRect m_rcBestCarPos;
	CRect m_rcLastCarPos;

	//���̵����
	IReferenceComponentImage *m_pOneRefImage;
	IReferenceComponentImage *m_pTwoRefImage;
	IReferenceComponentImage *m_pThreeRefImage;
	int m_iOnePosLightScene;
	int m_iTwoPosLightScene;
	int m_iThreePosLightScene;
	int m_iPassStopLightScene;
	int m_iPassLeftStopLightScene;

	//�Ƿ�Ҫץ��
	bool m_fFirstTrigger;
	bool m_fSecondTrigger;
	bool m_fThirdTrigger;

	//ץ�ĵ�ʱ��
	DWORD32 m_dwFirstTime;
	DWORD32 m_dwSecondTime;
	DWORD32 m_dwThirdTime;

	bool    m_fIsTrigger;
	//ץ�ı��
	DWORD32 m_dwTriggerIndex;
	int    m_iCapCount;

	DWORD32 m_dwFirstTimeMs;
	DWORD32 m_dwSecondTimeMs;
	DWORD32 m_dwThirdTimeMs;
	int m_iLastTriggerTimeMs;
	static const int s_iMinTriggerPosTime = 200;		//����λ�ô�������Сʱ����

	DWORD32 m_dwFirstFlameTime;	//�������ʱ��

	//����
	HiVideo::CRect m_rgRect[3];

	CarSizeInfoObj m_stCarInfo;		// ������Χ��Ϣ(������Ƶ���)

	DWORD32 m_dwBestSnapShotRefTime;
	DWORD32 m_dwLastSnapShotRefTime;
	DWORD32 m_dwBeginCaptureRefTime;
	DWORD32 m_dwBestCaptureRefTime;
	DWORD32 m_dwLastCaptureRefTime;

	//zhaopy
	// �ڶ���ץ��
	bool m_fHasTrigger;
	DWORD32 m_dwTriggerTimeMs;
	DWORD32 m_dwTriggerImageTime;
	CObjTrackInfo();
	~CObjTrackInfo();

	HRESULT New(PROCESS_ONE_FRAME_PARAM* pParam, CObjInfo &objInfo, HV_COMPONENT_IMAGE &imgCurFrame, int iLightScene, int iCurLight, bool fIsMan, PROCESS_ONE_FRAME_RESPOND* pProcessRespond);
	HRESULT Process(CObjInfo &objInfo, HV_COMPONENT_IMAGE &imgCurFrame, HiVideo::CRect rcVideoDet, int iLightScene, int iCurLight, PROCESS_ONE_FRAME_PARAM* pParam, PROCESS_ONE_FRAME_RESPOND* pProcessRespond);
    //�����Ƴ�������ӵ��ģ����ٵĹ���
    CObjMatch* m_pObj;
    UINT m_dwPredictTime;
    HiVideo::CRect m_rcPredict;
    int m_iObjStopFrame;
    HRESULT NewTemEx2(int iVideoID, 
                      CObjInfo &objInfo, 
                      PROCESS_ONE_FRAME_PARAM* pParam, 
                  	  HV_COMPONENT_IMAGE &imgCurImage, 
                  	  PROCESS_ONE_FRAME_RESPOND* pProcessRespond,
                      int iLightScene, 
                      int iCurLight,
                      CPoint offset,
                      HvSize EPSize, 
                      bool fIsMan = false);
    HRESULT ProcessEx(PROCESS_ONE_FRAME_PARAM* pParam, 
                  	  HV_COMPONENT_IMAGE& imgSrc, 
					  HiVideo::CRect rcVideoDet,
                      CRect& rSearch,
                      BOOL fEnableDetReverseRun,
                      int iLightScene,
					  LIGHT_TYPE lightType,
			          PROCESS_ONE_FRAME_RESPOND* pProcessRespond
                      );

    bool IsPassLine();
    bool IsTerminateTrack(HiVideo::CRect &rcVideoDet, LIGHT_TYPE lightType);
    BOOL IsTransToTemState(HV_COMPONENT_IMAGE& imgSrc, 
                  	  		int iLightScene, 
                            int iCurLight, 
                            CPoint offset, 
                            int iLineY
                            );
    BOOL IsObjStop();
    
    //ģ��ƥ��
    HRESULT UpdateObjTemple(HV_COMPONENT_IMAGE &imgCurImage, CRect &rcObj);
    
    CRect PredictPosition(UINT nNowTime, BOOL fEnableDetReverseRun);
    CRect PredictPositionSimple(UINT nNowTime, BOOL fEnableDetReverseRun, HvSize imgSize);
	HRESULT Clear(PROCESS_ONE_FRAME_RESPOND* pProcessRespond);
	HRESULT ClearImage(PROCESS_ONE_FRAME_RESPOND* pProcessRespond);
	
	CObjInfo &LastInfo();
	HRESULT Vote(CAR_TYPE &nVotedCarType);
	bool MatchPlateTrackID(int iPlateTrackID);
	HRESULT AddPlateTrackID(int iPlateTrackID);

    //ץ�Ĳ�������ͼ
    HRESULT FlashLightImage(DWORD32* pdwTriggerTime, int* piMinTriggerTimeMS, DWORD32* pdwLastTriggerInfo, int iLightType);

      //src��������ᱻ�޸�,����������
	HRESULT Assign(CObjTrackInfo &src, PROCESS_ONE_FRAME_RESPOND* pProcessRespond);

	bool IsClearImage(void);
	bool CheckImageClear(void){return m_bClearImage;}

protected:
	int m_iBestDistance;
	bool m_bClearImage;
};

#endif //_OBJECTTRACKING_INFO_H_
