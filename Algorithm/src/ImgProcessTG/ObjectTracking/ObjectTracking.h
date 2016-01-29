#ifndef _OBJECTTRACKING_H_
#define _OBJECTTRACKING_H_

#include "swImageObj.h"
#include "hvBaseObj.h"

#include "hvinterface.h"
#include "HvCompute.h"
#include "HvImageNew.h"
//#include "Tracker.h"
#include "HvContours.h"
#include "HvObjShift.h"

#define SEQ_SIZE_MAX    30
#define SEQ_NUM         1000

using namespace HiVideo;

typedef struct struct_objdetect_info
{
    int*   piRectCount;
    int*   piRectEPCount;
    CRect* pRects;
    CRect* pEPRects;
    int    iMaxCount;
    int    iDetectKind;
    int*   piRectModelCount;
    CRect* pModelRects;
    int    iDownX;                 //X�������ű���
    int    iDownY;                 //Y�������ű���  
    CRect* pVideoDetArea;         //�����������Ƶ�������
    int    iSrcH;                 //ԭͼ��
    int    iSrcW;                 //ԭͼ��
    CPoint* pOffSet;
}OBJDETECT_INFO;

// zhaopy 0624 
struct ObjDetInfo
{
	HiVideo::CRect* videoDet;
	int*   piRectLightCount;
	CRect* pLightRects;
};

const int MAX_LIGHT_NIGHT_COUNT = 1;

//�ƶ�������ӿ�
class IObjectDetector
{
public:
	// zhaopy 0624
	//����һ֡��Ƶ, ȡ���ƶ���������
	virtual HRESULT Process(CPoint offset, const HV_COMPONENT_IMAGE *pCompImage, CRect *pRects, int iMaxRectCount, int *piRectCount, bool fIsNight, ObjDetInfo& detInfo) = 0;

	    
    virtual HRESULT ProcessEx(CPoint offset, 
                              const HV_COMPONENT_IMAGE *pCompImage, 
                              bool fIsNight, OBJDETECT_INFO& objDetectInfo) = 0;
                                 
	//�������ģ��
	virtual HRESULT ClearBackgroup() = 0;
	//ȡ�ñ�������
	virtual HRESULT GetAvgGrey(int &iAvgGrey) = 0;
	//���ó��ƺ����ֿ�Ȳ��ұ�, ���������Ӧ�߶ȵĳ������
	virtual HRESULT SetPlateWidth(int *piPlateFiveCharWidth, int iCount, int iHeightStep) = 0;
	//�ͷ�ģ��
	virtual HRESULT Release() = 0;

	virtual HRESULT SetHvParam(HvCore::IHvParam2* pHvParam) = 0;
	
    virtual HRESULT GetCurrFrameGray(IReferenceComponentImage** pGray) = 0;

    virtual HRESULT GetCurrFG(IReferenceComponentImage** pGray) = 0;

	// zhaopy 0624
	virtual IReferenceComponentImage* GetCurrFrame() = 0;
	virtual int GetMultiDownSampleScale() = 0;

	virtual HRESULT GetVersion(char* szBuf, int szBufLen) = 0;
};

//�����ƶ�������ģ��
HRESULT CreateObjectDetector(IObjectDetector **pObj);

//�ƶ����屳���ӿ�
class IObjTrackBackgroup
{
public:
	//Ԥ������
	virtual HRESULT Prepare(IReferenceComponentImage *pRefImage) = 0;
	//�������
	virtual HRESULT Clear() = 0;
	//�ͷ�ģ��
	virtual HRESULT Release() = 0;
	
	//add by: qinyj for: ������Χ(20090409)
	//�޸ķ�Χ����
	virtual HRESULT SetLimitedRange(const HV_RECT &rect) = 0;
	
	//add by: qinyj for:������Χ(20090409)
	//��ȡ��Χ����
	virtual HRESULT GetLimitedRange(HV_RECT &rect) = 0;
};

//�����ƶ����屳��ģ��
HRESULT CreateObjTrackBackgroup(IObjTrackBackgroup **pObj);

//�ƶ�������ٽӿ�
class IObjectTracker
{
public:
	//����ˢ��ģ��
	virtual HRESULT PrepareTemplate(IObjTrackBackgroup *pObjTrackBackgroup, CRect rcOrig) = 0;
	//����һ֡
	virtual HRESULT ProcessOneFrame(IObjTrackBackgroup *pObjTrackBackgroup, CRect rcOrig, CRect *prcNew) = 0;
	//���ýӿ�
	virtual HRESULT AddRef() = 0;
	virtual HRESULT Release() = 0;
};

//�����������ģ��
HRESULT CreateObjectTracker(IObjectTracker **ppObj);

class CObjectDetector: public IObjectDetector
{
public:
	CObjectDetector()
		:m_iDownSampleScale(1)
		,m_pElementDay(NULL)
		,m_pElementNight(NULL)
		,m_pElementLight(NULL)
		,m_pElementNoon(NULL)
		,m_pElementBig(NULL)
		,m_pImgOldFrame(NULL)
		,m_cFiveCharWidth(0)
		,m_iHeightStep(0)
		,m_pHvParam(NULL)
		,m_pImgFG(NULL)
		,m_pImgNewFrame(NULL)
		,m_pOld(NULL) // zhaopy 0624
	{
	}
	~CObjectDetector()
	{
		ClearBackgroup();
	}
	//����һ֡��Ƶ, ȡ���ƶ���������
	virtual HRESULT Process(CPoint offset, const HV_COMPONENT_IMAGE *pCompImage, CRect *pRects, int iMaxRectCount, int *piRectCount, bool fIsNight, ObjDetInfo& detInfo);
	    //�羯��������Ƶ���
    virtual HRESULT ProcessEx(CPoint offset, 
                              const HV_COMPONENT_IMAGE *pCompImage, 
                              bool fIsNight, OBJDETECT_INFO& objDetectInfo);
                              
	//�������ģ��
	virtual HRESULT ClearBackgroup();
	//ȡ�ñ�������
	virtual HRESULT GetAvgGrey(int &iAvgGrey);
	//���ó��ƺ����ֿ�Ȳ��ұ�, ���������Ӧ�߶ȵĳ������
	virtual HRESULT SetPlateWidth(int *piPlateFiveCharWidth, int iCount, int iHeightStep);
	virtual HRESULT SetHvParam(HvCore::IHvParam2* pHvParam);
	//�ͷ�ģ��
	virtual HRESULT Release()
	{
		delete this;
		return S_OK;
	}
    virtual HRESULT GetCurrFrameGray(IReferenceComponentImage** pGray);
    virtual HRESULT GetCurrFG(IReferenceComponentImage** pFG);
	
	// zhaopy 0624
	virtual IReferenceComponentImage* GetCurrFrame();
	virtual int GetMultiDownSampleScale();
	HRESULT FindCarLight(HvSeq* pLights, HV_COMPONENT_IMAGE *pBin, HvMemStorage* storage);//ҹ���ҵ�

	virtual HRESULT GetVersion(char* szBuf, int szBufLen);

protected:
	int m_nNewWidth;					// DownSample���ͼ����
	int m_nNewHeight;					// DownSample���ͼ��߶�

	//��Ƶͼ��DOWNSAMPLE�ı���
	int m_iDownSampleScale;
	int m_iDownX;
    int m_iDownY;
	int m_iAvgGrey;

	HvCore::IHvParam2* m_pHvParam;

	HvConvKernel* m_pElementDay;
	HvConvKernel* m_pElementNight;
	HvConvKernel* m_pElementNoon;
	HvConvKernel* m_pElementBig;
	HvConvKernel* m_pElementLight;
	
	//֡���������
	IReferenceComponentImage *m_pImgOldFrame;		//add by: qinyj  for: ֡�
	IReferenceComponentImage *m_pImgNewFrame;
    IReferenceComponentImage *m_pImgFG;

	IReferenceComponentImage *m_pOld;      //���ٿռ䱣����֡��ͼ  zhaopy 0624

	//���ƺ����ֿ�ȶ�Ӧ��
	int m_rgiFiveCharWidth[40];
	int m_cFiveCharWidth;
	int m_iHeightStep;
	int GetMaxCarWidth(int x, int y);
	
    //void MObjPickSimple(HV_COMPONENT_IMAGE* pFG, HvCarSeq* pCars, HvMemStorage* storage);
    void MObjPickSimple(HV_COMPONENT_IMAGE* pFG, HvSeq* pCars, HvMemStorage* storage);
    HRESULT SegmentCarEP2(HV_COMPONENT_IMAGE* pGray,
                         HV_COMPONENT_IMAGE* pFG,
                         HV_COMPONENT_IMAGE* pTempImg,
                         HV_COMPONENT_IMAGE* pImgYUV,
                         HV_COMPONENT_IMAGE* pImgHSV,
                         OBJDETECT_INFO& objDetInfo,
                         HvSeq* detectCars);
                         //HvCarSeq& oldCars);
public:
	static int m_iProcessType;					// �������� 0:ȱʡ 1:�������˼����Ŀ
	static int m_iNightPlus;						// ���ϼ�ǿ�Էǻ������ļ�⡣��������ʱ�ſ�ʹ�á�
	static int m_iShadowDetect;                //�Ƿ������Ӱ���
	static int m_iShadowEffect;                //��Ӱ���ǿ���̶�ֵ
	static int m_iDiffType;                    //0: ����֡����; 1: ��ֺ���sobel��Եͼ 
	static int m_iMedFilter;                   //��ֵ�˲�����
	static int m_iMedFilterUpLine;             //��ֵ�˲������������
	static int m_iMedFilterDownLine;           //��ֵ�˲������������
    static int m_iEPOutPutDetectLine;           //�羯�����¼�������
    static int m_iEPFGDetectLine;               //ǰ�������

	// zhaopy 0624
	static int m_iNightMerge;                  
	static int m_iLightDetectThreld;           //ҹ�������ֵ����ֵ
	static int m_iInverseMaskThreld;           //����mask��ֵ
	static int m_iLightGroup;                  //������ٿ���
	static int m_iRectEnlage;                   //ҹ�����κϲ�����

	// ganzz 0219
	static int m_iDayBackgroudThreshold;	//���챳�������ֵ
	static int m_iNightBackgroudThreshold;	//���ϱ��������ֵ
	static int m_iNightLightBinThreshold;   //���ϵ�����ټ���ֵ����ֵ
	static int m_iRoadNumber;					//��������
};

#endif
