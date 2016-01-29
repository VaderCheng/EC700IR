#ifndef __SW_CARLEFT_H__
#define __SW_CARLEFT_H__
#include "tinyxml.h"
#include "SWBaseLinkCtrl.h"
#include "swpa_datetime.h"

#define CHECK_LOCK() {if(m_fReadOnly){return;}}
#define DECLEAR_GET_FUNCTION(type, func, var) virtual type func(VOID){return (type)m_pCarLeft->cCoreResult.var;}
#define DECLEAR_SET_FUNCTION(type, func, var) virtual VOID func(type var){CHECK_LOCK() m_pCarLeft->cCoreResult.var = var;}

enum
{
	BEST_SNAPSHOT,		//��һ�Ŵ�ͼ
	LAST_SNAPSHOT,    //�ڶ��Ŵ�ͼ
	BEGIN_CAPTURE,    //��һ��ץ��ͼ
	BEST_CAPTURE,     //�ڶ���ץ��ͼ
	LAST_CAPTURE,     //������ץ��ͼ
	PLATE_IMAGE,      //����Сͼ
	PLATE_BIN,        //���ƶ�ֵͼ
	MAX
};

class CSWCarLeft : public CSWObject
{
	friend class CSWRecognizeTransformFilter;
	CLASSINFO(CSWCarLeft, CSWObject)
public:
					 CSWCarLeft();
	virtual ~CSWCarLeft();
public:	
	virtual  HRESULT   SetParameter(TRACKER_CFG_PARAM *pCfg, CARLEFT_INFO_STRUCT *pCarLeft);
	virtual  HRESULT   GetEventDetInfo(CSWString& strInfo);
	virtual  HRESULT   BuildPlateString(TiXmlDocument& xmlDoc);
	virtual  CSWString BuildPlateString(VOID);
	virtual  INT       GetImageCount(void);
	virtual  CSWImage* GetImage(INT index, SW_RECT* rc = NULL);
    virtual  VOID GetResultRect(INT index, SW_RECT* rc);
	virtual  VOID      SetImage(INT index, CSWImage *pImage, SW_RECT* rc = NULL);
    virtual  VOID      SetResultRect(INT index, SW_RECT* rc);
	virtual  CSWString GetPlateNo(VOID);
	virtual  HRESULT   SetPlateNo(LPCSTR szPlateNo);
	virtual  HRESULT   GetCarSize(INT &iOutType, FLOAT &fltWidth, FLOAT &fltHeight);
	virtual  SW_RECT   GetFaceRect(INT index);
	virtual  VOID      SetFaceRect(CSWCarLeft *pCarLeft);
	virtual  TRACKER_CFG_PARAM* GetTrackerParam();
	virtual  CSWString BuildNormalString();
	virtual  VOID	SetFilterInfo(CSWString& strFilterInfo);
	virtual  CSWString GetFilterInfo();
	virtual  CSWString GetSpeedType(void){return m_strSpeedType;}
	virtual  void      SetSpeedType(LPCSTR szType){m_strSpeedType = szType;}

	/**
	 *@brief ��ȡ���������ţ�����ʼ��������д����Ǵ��ҿ�ʼ����йأ��ڲ����Ǵ���0��ʼ
	 */
	virtual DWORD GetOutPutRoadNo(VOID);

		//for jupiter
	virtual HRESULT SetDetectCarTime(SWPA_DATETIME_TM &cTime){return S_OK;}
	virtual HRESULT SetPeccancyTime(SWPA_DATETIME_TM &cTime){return S_OK;}
	virtual HRESULT SetPresetName(LPCSTR szName){return S_OK;}
	virtual VOID SetVideo(INT index, CSWImage *pVideo){return;}
	virtual CSWImage *GetVideo(INT index){return S_OK;}
	virtual INT GetVideoCount(){return 0;}
	/**
	 *@brief ���Ӵ������,���ô˺���֮��,���еĲ�����Ϊֻ��,����д
	 */
	virtual  void      Done(void);
public:
	static  void SetRoadInfo(const CHAR* szName, const CHAR* szDirection);
	static  CHAR* GetRoadName();
	static  CHAR* GetRoadDirection();
    static  void SetOutputMode(const INT& iBestImage, const INT& iLastImage, const INT& iCaptureImage,
                               const INT& iLastCropImage = 0, const INT&iCropWidthLevel = 4, const INT&iCropHeightLevel = 4);
	static  INT GetOutputBestImage();
	static  INT GetOutputLastImage();
	static  INT GetOutputCaptureImage();
    static  INT GetOutputCropImage();
    static VOID GetCropWidthHeight(INT& iCropWidth, INT& iCropHeight);
	static  void SetOutputHSLFlag(const INT& iEnable);
	static  INT GetOutputHSLFlag();

public:
	DECLEAR_GET_FUNCTION(PBYTE, GetContent,                rgbContent)
	DECLEAR_GET_FUNCTION(INT,   GetPlateType,              nType)
	DECLEAR_SET_FUNCTION(INT,   SetPlateType,              nType)
	DECLEAR_GET_FUNCTION(INT,   GetPlateColor,             nColor)
	DECLEAR_SET_FUNCTION(INT,   SetPlateColor,             nColor)
	DECLEAR_GET_FUNCTION(FLOAT, GetAverageConfidence,      fltAverageConfidence)
	DECLEAR_SET_FUNCTION(FLOAT, SetAverageConfidence,      fltAverageConfidence)
	DECLEAR_GET_FUNCTION(FLOAT, GetFirstAverageConfidence, fltFirstAverageConfidence)
	DECLEAR_SET_FUNCTION(FLOAT, SetFirstAverageConfidence, fltFirstAverageConfidence)
	DECLEAR_GET_FUNCTION(INT,   GetAvgY,                   iAvgY)
	DECLEAR_GET_FUNCTION(INT,   GetCarAvgY,                iCarAvgY)
	DECLEAR_GET_FUNCTION(INT,   GetCarVariance,            iCarVariance)
	DECLEAR_GET_FUNCTION(INT,   GetCarType,                nCarType)
	DECLEAR_SET_FUNCTION(CAR_TYPE,   SetCarType,           nCarType)
	DECLEAR_GET_FUNCTION(INT,   GetCarColor,               nCarColor)
	DECLEAR_SET_FUNCTION(CAR_COLOR, SetCarColor,           nCarColor)
	DECLEAR_GET_FUNCTION(INT,   GetRoadNo,                 nRoadNo)
	DECLEAR_SET_FUNCTION(INT,   SetRoadNo,                 nRoadNo)
	DECLEAR_GET_FUNCTION(INT,   GetStartFrameNo,           nStartFrameNo)
	DECLEAR_GET_FUNCTION(DWORD, GetEndFrameNo,             nEndFrameNo)
	DECLEAR_GET_FUNCTION(DWORD, GetFirstFrameTime,         nFirstFrameTime)
	DECLEAR_GET_FUNCTION(INT,   GetObservedFrames,         iObservedFrames)
	DECLEAR_SET_FUNCTION(INT,   SetObservedFrames,         iObservedFrames)
	DECLEAR_GET_FUNCTION(INT,   GetVotedObservedFrames,    iVotedObservedFrames)
	DECLEAR_GET_FUNCTION(INT,   GetVoteCondition,          nVoteCondition)
	DECLEAR_GET_FUNCTION(INT,   GetReverseRun,             fReverseRun)
	DECLEAR_SET_FUNCTION(INT,   SetReverseRun,             fReverseRun)
	DECLEAR_GET_FUNCTION(BOOL,  GetIsDoubleYellowLine,     fIsDoubleYellowLine)
	DECLEAR_GET_FUNCTION(FLOAT, GetCarspeed,               fltCarspeed)
	DECLEAR_SET_FUNCTION(FLOAT, SetCarspeed,               fltCarspeed)
	DECLEAR_GET_FUNCTION(FLOAT, GetScaleOfDistance,        fltScaleOfDistance)
	DECLEAR_GET_FUNCTION(INT,   GetPlateLightType,         nPlateLightType)
	DECLEAR_GET_FUNCTION(INT,   GetCplStatus,			   iCplStatus)
	DECLEAR_GET_FUNCTION(INT,   GetPulseLevel,			   iPulseLevel)
	DECLEAR_GET_FUNCTION(INT,   GetVideoID,                iVideoID)
	DECLEAR_GET_FUNCTION(INT,   GetTriggerTime,            dwTriggerTime)
	DECLEAR_GET_FUNCTION(INT,   GetTriggerIndex,           dwTriggerIndex)
	DECLEAR_GET_FUNCTION(INT,   GetCarArriveTime,          nCarArriveTime)
	DECLEAR_SET_FUNCTION(INT,   SetCarArriveTime,          nCarArriveTime)
	DECLEAR_GET_FUNCTION(INT,   GetIsOverYellowLine,       coltIsOverYellowLine)
	DECLEAR_SET_FUNCTION(CROSS_OVER_LINE_TYPE,   SetIsOverYellowLine,       coltIsOverYellowLine)
	DECLEAR_SET_FUNCTION(CROSS_OVER_LINE_TYPE,   SetIsCrossLine,            coltIsCrossLine)
	DECLEAR_GET_FUNCTION(INT,   GetIsCrossLine,            coltIsCrossLine)
	DECLEAR_GET_FUNCTION(INT,   GetCarIsStop,              fCarIsStop)
	DECLEAR_GET_FUNCTION(INT,   GetCarStopPassTime,        iCarStopPassTime)
	DECLEAR_GET_FUNCTION(TRAFFICLIGHT_SCENE,GetOnePosScene,tsOnePosScene)
	DECLEAR_GET_FUNCTION(TRAFFICLIGHT_SCENE,GetTwoPosScene,tsTwoPosScene)
	DECLEAR_GET_FUNCTION(INT,   GetUnSurePeccancy,         iUnSurePeccancy)
	DECLEAR_GET_FUNCTION(INT,   GetRunType,                rtType)
	DECLEAR_GET_FUNCTION(INT,   GetPTType,                 ptType)
	DECLEAR_SET_FUNCTION(INT,   SetPTType,                 ptType)
	DECLEAR_SET_FUNCTION(INT,   SetLightType,              nPlateLightType)
	DECLEAR_SET_FUNCTION(INT,   SetPulseLevel,				iPulseLevel)
	DECLEAR_SET_FUNCTION(INT,   SetCplStatus,				iCplStatus)
	DECLEAR_GET_FUNCTION(INT,   GetDetectCrossLineEnable,  nDetectCrossLineEnable)
	DECLEAR_GET_FUNCTION(INT,   GetDetectOverYellowLineEnable, nDetectOverYellowLineEnable)
	DECLEAR_GET_FUNCTION(LIGHT_REALTIME,   GetRedStart,    lrtRedStart)
	DECLEAR_GET_FUNCTION(HV_RECT*,GetRedLightPos,          rcRedLightPos)
	DECLEAR_GET_FUNCTION(INT,    GetRedLightCount,         rcRedLightCount)
    DECLEAR_GET_FUNCTION(HV_RECT*,GetFaceRect,             rgFaceRect)
    DECLEAR_GET_FUNCTION(HV_RECT,GetBestPlateRect,             rcBestPlatePos)
    DECLEAR_GET_FUNCTION(HV_RECT,GetLastPlateRect,             rcLastPlatePos)
	DECLEAR_GET_FUNCTION(INT,    GetFaceCount,             nFaceCount)	
	DECLEAR_GET_FUNCTION(BOOL,    GetIsNight,             fIsNight)	
	DECLEAR_GET_FUNCTION(INT,    GetiH,             iH)
	DECLEAR_GET_FUNCTION(INT,    GetiS,             iS)
	DECLEAR_GET_FUNCTION(INT,    GetiL,             iL)
	DECLEAR_GET_FUNCTION(INT,    GetWdrLevel,             nWdrLevel)
protected:
	CARLEFT_INFO_STRUCT* m_pCarLeft;
	TRACKER_CFG_PARAM*   m_pTrackerCfg;
	CSWString m_strPlateNo;
	CSWString m_strFilterInfo; // ������Ϣ
	CSWString m_strSpeedType;
	// ·����Ϣ
	static CHAR s_szRoadName[128];
	static CHAR s_szRoadDirection[128];
	// ͼƬ�������
	static INT s_iOutputBestImage;
	static INT s_iOutputLastImage;
    static INT s_iOutputCaptureImage;
    static INT s_iOutputCropImage;

    static INT s_iCropWidth;
    static INT s_iCropHeight;
	// HSL �������
	static INT s_iOutputHSL;
private:
	BOOL   m_fReadOnly;

public:
    BOOL m_fCropLastImage;      // �Ƿ���Ҫ��ȡ����ͼ
    BOOL m_fCaptureRecogResult;     // �Ƿ���ץ��ʶ��ͼ���
};

class CSWCarLeftArray : public CSWObject
{
	CLASSINFO(CSWCarLeft, CSWObject)
public:	
	CSWCarLeftArray(INT nMaxCount = 10)
	{
		m_nMaxCount = nMaxCount;
		m_nCount = 0;
		m_pCarLeft = new CSWCarLeft*[m_nMaxCount];
		for(int i = 0; i < m_nMaxCount; i++)
		{
			m_pCarLeft[i] = new CSWCarLeft();
		}
	}
	virtual ~CSWCarLeftArray()
	{
		for(int i = 0; i < m_nMaxCount; i++)
		{
			SAFE_RELEASE(m_pCarLeft[i]);			
		}
		delete []m_pCarLeft;
	}
	HRESULT SetCount(INT iCount)
	{
		if(m_nCount > m_nMaxCount)
		{
			return E_FAIL;
		}
		m_nCount = iCount;
		return S_OK;
	}
	INT GetCount(void)
	{
		return m_nCount;
	}
	CSWCarLeft* operator[](INT index)
	{
		return index < m_nMaxCount ? m_pCarLeft[index] : NULL;
	}
	CSWCarLeft* Get(INT index)
	{
		return (*this)[index];
	}
private:
	INT m_nMaxCount;
	INT m_nCount;
	CSWCarLeft **m_pCarLeft;
};
#endif

