#pragma once
#include "trackerdef.h"
//#include "hvvartype.h"
#include "swImage.h"
#include "hvBaseObj.h"
#include "swPlate.h"
#include "swImageObj.h"
#include "..\Inc\HvImageNew.h"
#include "sequence.h"

#include "trafficlight_recognizer.h"

using namespace signalway;

#define TS_UNSURE 0
#define TS_GREEN 3
#define TS_YELLOW 5
#define TS_RED 9

#define MAX_TRAFFICLIGHT_COUNT 1	//��������
#define MAX_IO_LIGHT_COUNT 1
//���е��鵱ǰ״̬
typedef struct tag_LightTeamStatus
{
	DWORD32 nTeamCount;
	DWORD32 pdwStatus[MAX_TRAFFICLIGHT_COUNT];
	DWORD32 pnLightCount[MAX_TRAFFICLIGHT_COUNT];
	tag_LightTeamStatus()
	{
		memset(pdwStatus, 0, sizeof(DWORD32) * MAX_TRAFFICLIGHT_COUNT);
		memset(pnLightCount, 0, sizeof(DWORD32) * MAX_TRAFFICLIGHT_COUNT);
		nTeamCount = 0;
	}
}
LIGHT_TEAM_STATUS;

//���̵�ÿ�������Ĳ�����Ϣ
typedef struct _TRAFFICLIGHT_GROUP_INFO
{
	int iLightCount;
	int iLightType;
	HiVideo::CRect rcLight;
	TRAFFICLIGHT_POSITION tpLightPos[10];
	LIGHT_STATUS lsLightStatus[10];
}TRAFFIC_GROUP_INFO;
//���̵Ʋ���
typedef struct _TRAFFIC_LIGHT_PARAM
{	
	int iCheckType;
	int iAutoScanLight;
	int iSceneCheckMode;
	int iValidSceneStatusCount;
	int iRedLightDelay;
	
	int iLightGroupCount;
	TRAFFIC_GROUP_INFO rgtgiLight[MAX_TRAFFICLIGHT_COUNT];
	TRAFFICLIGHT_SCENE rgtsLightScene[MAX_SCENE_COUNT];
	SCENE_INFO rgsiInfo[MAX_SCENE_COUNT];
	TRAFFICLIGHT_TYPE rgIOLight[MAX_IO_LIGHT_COUNT];//��Ӻ��̵Ʋ���
	
	int iSceneCount;
	int iAutoScanThread;
	int iTrafficLightBrightValue;
	int iSkipFrameNo;
	BOOL fIsLightAdhesion;
	BOOL fDeleteLightEdge;
	bool fCheckSpeed;
}TRAFFIC_LIGHT_PARAM;


//CHvList ʵ��
typedef  int HVPOSITION;

template < class T, int MAX_COUNT >
class CHvList
{
public:
	CHvList()
		: m_iHead(0)
		, m_iTail(0)
		, m_pItem(NULL)
		, m_iBufferSize(0)
	//	, m_pSem(NULL)
	{
		m_iBufferSize += MAX_COUNT + 1;
		m_pItem = new T[m_iBufferSize];
		//HiVideo::ISemaphore::CreateInstance(&m_pSem, 1, 1);
	}
	~CHvList()
	{
		if( m_pItem != NULL )
		{
			delete[] m_pItem;
			m_pItem = NULL;
		}
		//if (m_pSem)
	//	{
	//		delete m_pSem;
	//		m_pSem = NULL;
	//	}
	};

public:
	HVPOSITION AddHead(const T& item)
	{
		HVPOSITION pos = 0;
		//m_pSem->Pend();
		if( (m_pItem != NULL) && !IsFull() )
		{
			m_iHead = (m_iHead + m_iBufferSize- 1) % m_iBufferSize;
			pos = m_iHead + 1;
			m_pItem[m_iHead] = item;
		}
		//m_pSem->Post();
		return pos;
	}

	HVPOSITION AddTail(const T& item)
	{
		HVPOSITION pos = 0;
		//m_pSem->Pend();
		if( (m_pItem != NULL) && !IsFull() )
		{
			pos = m_iTail + 1;
			m_pItem[m_iTail] = item;
			m_iTail = (m_iTail + 1) % m_iBufferSize;
		}
		//m_pSem->Post();
		return pos;
	}

	bool IsEmpty()
	{
		return (m_iTail == m_iHead);
	}

	bool IsFull()
	{
		return (((m_iTail + 1) % m_iBufferSize) == m_iHead);
	}

	T RemoveHead()
	{
		T item;
		//m_pSem->Pend();
		if( !IsEmpty() )
		{
			item = m_pItem[m_iHead];
			m_iHead = (m_iHead + 1) % m_iBufferSize;
		}
		//m_pSem->Post();
		return item;
	}

	T RemoveTail()
	{
		T item;
		//m_pSem->Pend();
		if( !IsEmpty() )
		{
			m_iTail = (m_iTail + m_iBufferSize - 1) % m_iBufferSize;
			item = m_pItem[ m_iTail];
		}
		//m_pSem->Post();
		return item;
	}

	void RemoveAll()
	{
		//m_pSem->Pend();
		m_iHead = m_iTail = 0;
		//m_pSem->Post();
	}

	HVPOSITION GetHeadPosition()
	{
		HVPOSITION pos = 0;
		if( !IsEmpty() )
		{
			pos = m_iHead + 1;
		}
		return pos;
	}

	HVPOSITION GetTailPosition()
	{
		HVPOSITION pos = 0;
		if( !IsEmpty() )
		{
			pos = ((m_iTail + m_iBufferSize - 1) % m_iBufferSize) + 1;
		}
		return pos;
	}

	T& GetNext( HVPOSITION& rPosition )
	{
		int iItem = rPosition - 1;
		HVPOSITION pos = 0;
		if( !IsEmpty() && 0<= iItem && iItem < m_iBufferSize )
		{
			if( m_iHead < m_iTail 
				&& (iItem >= m_iHead && iItem < m_iTail) )
			{
				if( ((iItem + 1) % m_iBufferSize) != m_iTail )
				{
					pos = ((iItem + 1) % m_iBufferSize) + 1;
				}
			}
			if( m_iHead > m_iTail 
				&& !((iItem >= m_iTail) && (iItem < m_iHead)) )
			{
				if( ((iItem + 1) % m_iBufferSize) != m_iTail )
				{
					pos = ((iItem + 1) % m_iBufferSize) + 1;
				}
			}
		}
		else
		{
			iItem = 0;
		}

		rPosition = pos;
		return m_pItem[iItem];
	}

	T& GetPrev( HVPOSITION& rPosition )
	{
		int iItem = rPosition - 1;
		HVPOSITION pos = 0;
		if( !IsEmpty() && iItem >= 0 && iItem < m_iBufferSize )
		{
			if( m_iHead < m_iTail 
				&& (iItem >= m_iHead && iItem < m_iTail) )
			{
				if( iItem != m_iHead )
				{
					pos = ((iItem + m_iBufferSize - 1) % m_iBufferSize) + 1;
				}
			}
			if( m_iHead > m_iTail 
				&& !((iItem >= m_iTail) && (iItem < m_iHead)) )
			{
				if( iItem != m_iHead )
				{
					pos = ((iItem + m_iBufferSize - 1) % m_iBufferSize) + 1;
				}
			}
		}
		else
		{
			iItem = 0;
		}

		rPosition = pos;
		return m_pItem[iItem];
	}

	int GetSize()
	{
		return ((m_iTail + m_iBufferSize - m_iHead) % m_iBufferSize);
	}

private:
	T* m_pItem;
	int m_iHead;
	int m_iTail;
	int m_iBufferSize;
};

class CTrafficLightInfo
{
public:
	enum {MAX_POS_COUNT = 8};	//����λ�û���

public:
	HRESULT UpdateStatus(HV_COMPONENT_IMAGE* pSceneImage);
	HRESULT UpdateStatus2(HV_COMPONENT_IMAGE* pSceneImage);
	HRESULT UpdateStatus3(HV_COMPONENT_IMAGE* pSceneImage, bool fNight) ; // �����˽�ͨ��λ�õĶ�λ; added by zoul, 20110926

	HRESULT SetRect(const HV_RECT& cRect, int nLightCount, int nType);
	HRESULT SetRect(int nLeft, int nTop, int nRight, int nBottom, int nLightCount, int nType);
	HRESULT SetAdjustRect(const HV_RECT& rcRect);
	HRESULT SetTrafficLightParam(TRAFFIC_LIGHT_PARAM tlpParam);

public:
	DWORD32 GetLastStatus()
	{
		return m_nLastStatus;
	}

	int GetPosCount()
	{
		return m_nLightCount;
	}

	HV_RECT GetRect()
	{
		return m_rcLight;
	}

	HV_RECT GetLastRect()
	{
		HiVideo::CRect rcTemp = m_rcLastLight;
		rcTemp += m_rcLightInflated.TopLeft();
		return rcTemp;
	}

	HV_RECT GetInflatedRect()
	{
		return m_rcLightInflated;
	}
	HV_RECT GetAdjustRect()
	{
		return m_rcLightAdjust;
	}
	void UpdateLastSureLight(HiVideo::CRect crectLastLight)
	{
		m_rcLastSureLight = crectLastLight;
	}
	HV_RECT GetLastSureLight()
	{
		return m_rcLastSureLight;
	}
	int GetLightDirection()
	{
		return m_nType;
	}

	HRESULT Reset()
	{
		m_fInit = false;
		HV_RECT rcInit = {0,0,0,0};
		m_rcLight = rcInit;
		m_rcLastLight = rcInit;
		m_nID = 0;
		m_nType = 0;
		m_nLightCount = 0;
		m_nLastStatus = TS_UNSURE;

		m_iFirstAdjust = 0;
		m_fltAdjustValue = 1;

		m_iMaxRedThreshold= -1;
		m_iMinRedThreshold = -1;
		m_iRedThreshold	 = 40;
		m_iMaxGreenThreshold	= -1;
		m_iMinGreenThreshold	= -1;
		m_iGreenThreshold = 15;
		
		m_iRedLightCountFinded = 0;
		m_iGreenLightCountFinded = 0;
		memset(m_rcRedLightPosFinded, 0, sizeof(m_rcRedLightPosFinded));
		memset(m_rcGreenLightPosFinded, 0, sizeof(m_rcGreenLightPosFinded));
		return S_OK;
	}

	HRESULT ReScan()
	{
		//�����û��ʼ��
		if( !m_fInit ) return S_FALSE;

		HV_RECT rcInit = {0,0,0,0};
		m_rcLastLight = rcInit;
		m_nLastStatus = TS_UNSURE;
		m_fInit = false;
		m_iFirstAdjust = 0;
		m_fltAdjustValue = 1;
		m_rcLight = m_rcInitLight;
		return S_OK;
	}
	HRESULT SetLightStatus(int iIndex,_TRAFFICLIGHT_LIGHT_STATUS trafficLightStatus)
	{
		m_trafficLight[iIndex].tlsStatus = trafficLightStatus;
		return S_OK;
	}

public:
	CTrafficLightInfo();
	~CTrafficLightInfo();

protected:
	DWORD32 GetLightStatus(HV_COMPONENT_IMAGE imgLight, int nPosCount, int nType);
	DWORD32 GetLightStatus2(HV_COMPONENT_IMAGE imgLight, HiVideo::CRect rcLight);

protected:
	INT m_nID;
	HiVideo::CRect m_rcLight;	//��ʼλ�ÿ�
	HiVideo::CRect m_rcLastLight; //��ǰλ��
	HiVideo::CRect m_rcLightInflated;
	INT m_nType;		//0 = ����, 1 = ����
	INT m_nLightCount;	//����������
	DWORD32 m_nLastStatus;

	HvConvKernel* m_pElement;

	TRAFFICLIGHT_TYPE m_trafficLight[5];
	bool m_fInit;
	//���̵�ƫ�Ƶ�������
	int m_iFirstAdjust;		//��һ�ε���
	float m_fltAdjustValue;		//�����ı���
	HiVideo::CRect m_rcLightAdjust;	

	HiVideo::CRect m_rcInitLight;
	//��������ʶ��ĵƵ�λ��
	HiVideo::CRect m_rcLastSureLight;

	CHvList<HiVideo::CRect, 5> m_lstAdjustQuence;

	signalway::CTrafficLightRecognizer m_cTrafficLightObj;
	TRAFFIC_LIGHT_PARAM m_tlpTrafficLightParam;

private:
	//���������λ��
	void AdjuestLightPos(HV_RECT rcSrc);
	bool IsValidityLight(HV_RECT rcSrcLight, HV_RECT rcNewLight, int iWitdh, int iHeight);
	bool IsNeedAdjust(HV_RECT rcSrcLight, HV_RECT rcNewLight, int iWitdh, int iHeight);
	
	//�������ӽӿ�,����̬��⵽�ĵƵ����괫��ȥ��ʾ
	void CreateLightGroupPos(HV_RECT rctSrc);	
private:
	signalway::sequence <signalway::trafficlight>  m_queFindLights;
	// zhaopy
	int m_iMaxRedThreshold;
	int m_iMinRedThreshold;
	int m_iRedThreshold;
	int m_iMaxGreenThreshold;
	int m_iMinGreenThreshold;
	int m_iGreenThreshold;
	
public:
	int m_iRedLightCountFinded;
	int m_iGreenLightCountFinded;
	HV_RECT m_rcRedLightPosFinded[20];
	HV_RECT m_rcGreenLightPosFinded[20];
};
