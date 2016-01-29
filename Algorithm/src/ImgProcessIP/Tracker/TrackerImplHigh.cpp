#include "TrackerImplHigh.h"
#include "HvUtils.h"
#include "GlobalDataTbl.h"
#include "EPAppUtils.h"

// todo.
#include "SwCore.h"

#define NORMAL_NUM 12
#define YELLOW_NUM 20
#define BIGSUN_NUM 10
#define MAX2(a, b)  ((a) > (b) ? (a) : (b))
#define MIN2(a, b)  ((a) < (b) ? (a) : (b))
#define WALK_MAN_MISSING_TIME	6

_CARCOLOR_INFO g_rgCarColorInfo[] =
{
	{ 0,	"δ֪",	    0		},
	{ 1,	"��",		116		},
	{ 2,	"��",		120		},
	{ 3,	"��",		47	    },
	{ 4,	"��",	    118		},
	{ 5,	"��",		119		},
	{ 6,	"��",	    122		},
	{ 7,	"��",		121		},
	{ 8,	"��",		123		},
	{ 9,	"��",		124		},
	{ 10,	"��",	    125		}
};

/*
_LOGO_INFO g_rgLogoInfo[] =
{
	{ 0,	"δ֪",	0		},
	{ 1,	"����",		126		},
	{ 2,	"�µ�",		127		},
	{ 3,	"����",		128		},
	{ 4,	"����",	129		},
	{ 5,	"���ǵ�",		130		},
	{ 6,	"����",	    131		},
	{ 7,	"���",		132		},
	{ 8,	"����",		133		},
	{ 9,	"����",		134		},
	{ 10,	"�ۺ�",	    135		},
	{ 11,	"����",	    136		},
	{ 12,	"����",	137		},
	{ 13,	"����",		138		},
	{ 14,	"����",	139		},
	{ 15,	"����",		140		},
	{ 16,	"����",	    141	    },
	{ 17,	"�ʹ�",		142		},
	{ 18,	"����",		143		},
	{ 19,	"����",		144		},
	{ 20,	"����",	145		},
	{ 21,	"��������",	146		},  
	{ 22,	"�׿���˹",	    147		},
	{ 23,	"�Ա�",	    148		},
	{ 24,	"��ľ",	149		},
	{ 25,	"���Դ�",		150	},
	{ 26,	"ŷ��",	151		},
	{ 27,	"����",		152		},
	{ 28,	"����",	    153	    },
	{ 29,	"�ղ�",		154		},
	{ 30,	"����",		155		},
	{ 31,	"˫��",		156		},
	{ 32,	"˹��³",	157		},
	{ 33,	"����",	158		},
	{ 34,	"����",	    159		},
	{ 35,	"�ִ�",	    160		},
	{ 36,	"ѩ����",	161		},
	{ 37,	"һ��",		162		},
	{ 38,	"Ӣ�����",	163		},
	{ 39,	"����",		164		},
	{ 40,	"����",	    165	    },
	{ 41,	"����",		166		},
	{ 42,	"����",	167		}

};
*/

#if RUN_PLATFORM == PLATFORM_DSP_BIOS
#include "HvDspTime.h"
#endif
#if RUN_PLATFORM == PLATFORM_WINDOWS
#include <TIME.h>
#endif

extern char g_szDebugInfo[256];

#define MAX_SMARTSAVE_COUNT 4

int CFinalResultParam::m_iBlockTwinsTimeout = 90;
int CFinalResultParam::m_iSendTwinResult = 1;

// ��Ƶ�������Ĭ��ֵ
const int VIDEO_DETECT_LEFT = 0;
const int VIDEO_DETECT_RIGHT = 100;
const int VIDEO_DETECT_TOP = 0;
const int VIDEO_DETECT_BOTTOM = 100;
int CTrackerImpl::m_sMinTriggerTimeMs = 0;


// zhaopy todo.
// ������Ϣ���
// ƽ̨���ӿ�
PLATFORM_FUCTION_LIST* g_pFuction = NULL;

void Venus_OutputDebug(char* szMsg)
{
	return;
	if( g_pFuction != NULL && g_pFuction->pfDebugOutPut != NULL )
	{
		g_pFuction->pfDebugOutPut(szMsg);
	}
}

enum
{
	LOG_LV_ERROR 	= 0,
	LOG_LV_NORMAL  	= 1,
	LOG_LV_OPERATE 	= 2,
	LOG_LV_DEBUG   	= 3
};

void OutPutDebugMsg(int iLevel, char* szMsg)
{
	if( g_pFuction != NULL && g_pFuction->pfDebugOutPut != NULL )
	{
		char* szMsgInfo = new char[strlen(szMsg) + 64];
		if( NULL != szMsgInfo )
		{
			sprintf(szMsgInfo, "<DSPLOG><LV:%d>%s", iLevel, szMsg);
			g_pFuction->pfDebugOutPut(szMsgInfo);
			delete[] szMsgInfo;
			szMsgInfo = NULL;
		}
	}
}

int Dsp_DebugInfo(char* szfmt, ...)
{
    //	if ( !IsDebugDeviceEnabled(iDeviceID) ) return 0;
#if 1
	return 0;
#else
    static char s_szbuf[1024];

    va_list arglist;
    int iRetVal = 0;

    s_szbuf[0] = 0;

    va_start( arglist, szfmt );
    iRetVal = std::vsnprintf(s_szbuf, sizeof(s_szbuf) - 1, szfmt, arglist);
    va_end( arglist );


#if 1
    Venus_OutputDebug(s_szbuf);
#else
    int nAddLen = strlen(s_szbuf);
    if (nAddLen + g_nDspDebugInfoLen < 1024-1)
    {
        memcpy(g_pDspDebugInfo + g_nDspDebugInfoLen, s_szbuf, nAddLen + 1);	  
        g_nDspDebugInfoLen += nAddLen;	
    }
#endif

    return iRetVal;
#endif
}

unsigned int Venus_GetSystemTick()
{
	if( g_pFuction != NULL && g_pFuction->pfGetSystemTick != NULL )
	{
		return g_pFuction->pfGetSystemTick();
	}
	return GetSystemTick();
}

// �㷨��������������ӿ�
void OutPutDebugInfo(unsigned char* pbData, unsigned int cbDataSize)
{
	if( g_pFuction != NULL && g_pFuction->pfOutPutDebugInfo != NULL )
	{
		g_pFuction->pfOutPutDebugInfo(pbData, cbDataSize);
	}
}

namespace Plate
{
	// grammer of the characters
#define DEFINE_PLATE_GRAMMER_NAME(a, b, r1, r2, r3, r4, r5, r6, r7)		r1, r2, r3, r4, r5, r6, r7,
	const int g_cnType[PLATE_TYPE_COUNT][7]=
	{
		PLATE_TYPE_LIST(DEFINE_PLATE_GRAMMER_NAME)
	};
}

#undef RTN_HR_IF_FAILED
#define RTN_HR_IF_FAILED(func)                          \
{	                                                    \
	HRESULT hr=func;                                    \
	if (FAILED(hr))                                     \
	{                                                   \
		char szMsg[256];								\
		sprintf(szMsg, "%s l:%d FAILED:%08X\n", #func, __LINE__, hr);\
		Venus_OutputDebug(szMsg);                       \
		HV_DebugInfo(		                            \
				DEBUG_STR_ERROR_FILE,                   \
				"FAILED %s(hr=0x%08X): ", #func"\n", hr \
				);                                      \
		return hr;	                                    \
	}	                                                \
}

#define VALUE_LIMIT(x, down, up)  ((x > up) ? up : ((x < down) ? down : x))

extern int GetLineX(int iY, int iLineNum);

static void CalcTLTrapDetArea(
		TRAP_DETAREA* pTrapDetArea,
		int nStartY,
		int nEndY,
		int nFrameWidth,
		int nFrameHeight
		)
{
	int nBottomLine = nFrameHeight * nEndY / 100;
	int nTopLine = nFrameHeight * nStartY / 100;
	if (nTopLine < 0)  nTopLine = 0;

	int nBottomLineX0 = 0;
	int nBottomLineX1 = 0;
	int nTopLineX0 = 0;
	int nTopLineX1 = 0;
	int nSlopeRatio0 = 0.f;  //road0, dx / dy * 100
	int nSlopeRatio1 = 0.f;  //road1, dx / dy * 100

	char szMsg[255];
	sprintf(szMsg, "nCarArriveLine:%d [%d %d %d %d]\n", nBottomLine, GetLineX(nTopLine, 0), GetLineX(nTopLine, 1), GetLineX(nBottomLine, 0), GetLineX(nBottomLine, 1));

	Venus_OutputDebug(szMsg);

	if(CTrackInfo::m_iRoadNumberBegin == 0)
	{
		nBottomLineX0 = GetLineX(nBottomLine, 0);
		nBottomLineX1 = GetLineX(nBottomLine, 1);
		nTopLineX0 = GetLineX(nTopLine, 0);
		nTopLineX1 = GetLineX(nTopLine, 1);
	}
	else
	{
		nBottomLineX0 = GetLineX(nBottomLine, 1);
		nBottomLineX1 = GetLineX(nBottomLine, 0);
		nTopLineX0 = GetLineX(nTopLine, 1);
		nTopLineX1 = GetLineX(nTopLine, 0);
	}

	nTopLineX0 = (nTopLineX0 + nBottomLineX0) >> 1;
	nTopLineX1 = (nTopLineX1 + nBottomLineX1) >> 1;

	nBottomLineX0 = nBottomLineX0 * 100 / nFrameWidth;
	nBottomLine = nBottomLine * 100 / nFrameHeight;
	nBottomLineX1 = nBottomLineX1 * 100 / nFrameWidth;
	nTopLineX0 = nTopLineX0 * 100 / nFrameWidth;
	nTopLine = nTopLine * 100 / nFrameHeight;
	nTopLineX1 = nTopLineX1 * 100 / nFrameWidth;

	pTrapDetArea->BottomLeftX = VALUE_LIMIT(nBottomLineX0, 0, 100);
	pTrapDetArea->BottomLeftY = VALUE_LIMIT(nBottomLine, 0, 100);
	pTrapDetArea->BottomRightX = VALUE_LIMIT(nBottomLineX1, 0, 100);
	pTrapDetArea->BottomRightY = VALUE_LIMIT(nBottomLine, 0, 100);

	pTrapDetArea->TopLeftX = VALUE_LIMIT(nTopLineX0, 0, 100);
	pTrapDetArea->TopLeftY = VALUE_LIMIT(nTopLine, 0, 100);
	pTrapDetArea->TopRightX = VALUE_LIMIT(nTopLineX1, 0, 100);
	pTrapDetArea->TopRightY = VALUE_LIMIT(nTopLine, 0, 100);

	sprintf(szMsg,"CalcTrapDetArea [%d %d] [%d %d] [%d %d] [%d %d]\n",
	pTrapDetArea->TopLeftX, pTrapDetArea->TopLeftY,
	pTrapDetArea->TopRightX, pTrapDetArea->TopRightY,
	pTrapDetArea->BottomLeftX, pTrapDetArea->BottomLeftY,
	pTrapDetArea->BottomRightX, pTrapDetArea->BottomRightY);

	Venus_OutputDebug(szMsg);
}

static void CalcPlateScale(TRAP_DETAREA* pTrapDetArea, int nFrameHeight,  int* pnMinScale, int* pnMaxScale)
{
    //���㳵�ƿ��
    int iMin = 0;
    int iMax = 0;
    int iPlateWidth = (int)((pTrapDetArea->TopRightX - pTrapDetArea->TopLeftX) * nFrameHeight /100 * 0.18f);
    for (int i = 0; i < 16;i++)
    {
        if (56 * pow(1.1, i) > iPlateWidth)
        {
            iMax = i;
            break;
        }
        else
        {
            iMin = i;
        }
    }
    int iDetMinScaleNum = iMin;
    iPlateWidth = (int)((pTrapDetArea->BottomRightX - pTrapDetArea->BottomLeftX) * nFrameHeight /100 * 0.18f);
    for (int i = iMin; i < 16;i++)
    {
        if (56 * pow(1.1, i) > iPlateWidth)
        {
            iMax = i;
            break;
        }
    }
    int iDetMaxScaleNum = iMax;
    int iDiff = iDetMaxScaleNum - iDetMinScaleNum;
    if (iDiff > 4)
    {
    	iDetMinScaleNum =  iDetMaxScaleNum - 4;
    }
    if (iDetMinScaleNum < 0) iDetMinScaleNum = 0;
    if (iDetMaxScaleNum > 16) iDetMaxScaleNum = 16;
    if (iDetMaxScaleNum <= iDetMinScaleNum) iDetMaxScaleNum = iDetMinScaleNum + 2;

    *pnMinScale = iDetMinScaleNum;
    *pnMaxScale = iDetMaxScaleNum;
}

// ����ר�ü����⡢ɨ������
void CTrackerImpl::CalcTrapDetArea(
		TRAP_DETAREA* pTrapDetArea,
		int nCarArriveLineY,  // �ٷֱ�
		int nFrameWidth,
		int nFrameHeight
		)
{
	if (nFrameWidth == 0 || nFrameHeight == 0) return;
	if (nCarArriveLineY < 50) nCarArriveLineY = 50;
	int nBottomLine = nFrameHeight * nCarArriveLineY / 100;
	int nTopLine = nFrameHeight * (nCarArriveLineY - 35) / 100;

	int nBottomLineX0 = 0;
	int nBottomLineX1 = 0;
	int nTopLineX0 = 0;
	int nTopLineX1 = 0;
	int nSlopeRatio0 = 0.f;  //road0, dx / dy * 100
	int nSlopeRatio1 = 0.f;  //road1, dx / dy * 100

	if(CTrackInfo::m_iRoadNumberBegin == 0)
	{
		nBottomLineX0 = GetLineX(nBottomLine, 0);
		nBottomLineX1 = GetLineX(nBottomLine, 1);
		nTopLineX0 = GetLineX(nTopLine, 0);
		nTopLineX1 = GetLineX(nTopLine, 1);
	}
	else
	{
		nBottomLineX0 = GetLineX(nBottomLine, 1);
		nBottomLineX1 = GetLineX(nBottomLine, 0);
		nTopLineX0 = GetLineX(nTopLine, 1);
		nTopLineX1 = GetLineX(nTopLine, 0);
	}

	nBottomLineX0 = nBottomLineX0 * 100 / nFrameWidth;
	nBottomLine = nBottomLine * 100 / nFrameHeight;
	nBottomLineX1 = nBottomLineX1 * 100 / nFrameWidth;
	nTopLineX0 = nTopLineX0 * 100 / nFrameWidth;
	nTopLine = nTopLine * 100 / nFrameHeight;
	nTopLineX1 = nTopLineX1 * 100 / nFrameWidth;

	pTrapDetArea->BottomLeftX = VALUE_LIMIT(nBottomLineX0, 0, 100);
	pTrapDetArea->BottomLeftY = VALUE_LIMIT(nBottomLine, 0, 100);
	pTrapDetArea->BottomRightX = VALUE_LIMIT(nBottomLineX1, 0, 100);
	pTrapDetArea->BottomRightY = VALUE_LIMIT(nBottomLine, 0, 100);

	pTrapDetArea->TopLeftX = VALUE_LIMIT(nTopLineX0, 0, 100);
	pTrapDetArea->TopLeftY = VALUE_LIMIT(nTopLine, 0, 100);
	pTrapDetArea->TopRightX = VALUE_LIMIT(nTopLineX1, 0, 100);
	pTrapDetArea->TopRightY = VALUE_LIMIT(nTopLine, 0, 100);

	// ���ݾ����������������
	m_cDetAreaInfo.DetectorAreaLeft = MIN(m_cfgParam.cTrapArea.TopLeftX, m_cfgParam.cTrapArea.BottomLeftX);
	m_cDetAreaInfo.DetectorAreaTop = MIN(m_cfgParam.cTrapArea.TopLeftY, m_cfgParam.cTrapArea.BottomLeftY);
	m_cDetAreaInfo.DetectorAreaRight = MAX(m_cfgParam.cTrapArea.TopRightX, m_cfgParam.cTrapArea.BottomRightX);
	m_cDetAreaInfo.DetectorAreaBottom = MAX(m_cfgParam.cTrapArea.TopRightY, m_cfgParam.cTrapArea.BottomRightY);

	m_rcVideoDetArea.left = 10;//m_cDetAreaInfo.DetectorAreaLeft;
	m_rcVideoDetArea.top = 0;//m_cDetAreaInfo.DetectorAreaTop;
	m_rcVideoDetArea.right = 90;//m_cDetAreaInfo.DetectorAreaRight;
	m_rcVideoDetArea.bottom = 100;//m_cDetAreaInfo.DetectorAreaBottom;

	/*char szT[1024] = {0};
	sprintf(szT, "[%d_%d_%d_%d]-----[%d_%d_%d_%d]\n",
		m_cDetAreaInfo.DetectorAreaLeft,
		m_cDetAreaInfo.DetectorAreaTop,
		m_cDetAreaInfo.DetectorAreaRight,
		m_cDetAreaInfo.DetectorAreaBottom,
		m_rcVideoDetArea.left,
		m_rcVideoDetArea.top,
		m_rcVideoDetArea.right,
		m_rcVideoDetArea.right
		);
	Venus_OutputDebug(szT);*/
}

void CFinalResultParam::Create(CTrackInfo &TrackInfo, PlateRecogParam *pParam)
{
	*((CTrackInfoHeader*)this)=TrackInfo;
	m_pParam=pParam;
	m_dwLastResultTime = GetSystemTick();

	m_pObj = NULL;				// ���ܸ���������󣬷����ͷŵ�ʱ�����
}

void CFinalResultParam::UpdateDetectSaver(DWORD32 dw_TimeRef)
{
	int iCount = 0;
	DWORD32 dwNowTime = dw_TimeRef;

	if(dwNowTime == 0)
	{
		dwNowTime = GetSystemTick();
	}

	for(int ix=0; ix<m_iTimeinCount; ix++)
	{
		if(m_detectsaver[ix].dw_TrackEndTime <= 0)
		{
			m_detectsaver[ix].dw_TrackEndTime = GetSystemTick();
		}

		if(dwNowTime > m_detectsaver[ix].dw_TrackEndTime)
		{
			if( (dwNowTime - m_detectsaver[ix].dw_TrackEndTime) >= (DWORD32)(m_iBlockTwinsTimeout * 1000))
			{
				iCount ++;
			}
		}
		else if(dwNowTime < m_detectsaver[ix].dw_TrackEndTime)
		{
			if( (m_detectsaver[ix].dw_TrackEndTime - dwNowTime) >= (DWORD32)( m_iBlockTwinsTimeout * 1000))
			{
				iCount ++;
			}
		}
	}

	if(iCount > 0)
	{
		this->m_iTimeinCount -= iCount;

		if(this->m_iTimeinCount < 0)
		{
			//HV_Trace("\nOutOfRange... m_iTimeinCount = 0\n");
			this->m_iTimeinCount = 0;
		}

		for(int i=0; i<m_iTimeinCount; i++)
		{
			m_detectsaver[i].dw_TrackEndTime = m_detectsaver[i+iCount].dw_TrackEndTime;
			m_detectsaver[i+iCount].dw_TrackEndTime = 0;
			for(int j=0; j<8; j++)
			{
				m_detectsaver[i].rgContent[j] = m_detectsaver[i+iCount].rgContent[j];
				m_detectsaver[i+iCount].rgContent[j] = 0;
			}
		}
	}	
}

void CFinalResultParam::SetOrder()
{
	DWORD32 dwEndTimeTemp = 0;
	BYTE8 rgContentTemp[8] = {0};
	for(int iX=0; iX<=m_iTimeinCount; iX++)
	{
		for(int iY=iX+1; iY<=m_iTimeinCount; iY++)
		{
			if(m_detectsaver[iX].dw_TrackEndTime > m_detectsaver[iY].dw_TrackEndTime)
			{
				dwEndTimeTemp = m_detectsaver[iX].dw_TrackEndTime;
				m_detectsaver[iX].dw_TrackEndTime = m_detectsaver[iY].dw_TrackEndTime;
				m_detectsaver[iY].dw_TrackEndTime = dwEndTimeTemp;

				for(int iH=0;iH<8;iH++)
				{
					rgContentTemp[iH] = m_detectsaver[iX].rgContent[iH];
					m_detectsaver[iX].rgContent[iH] = m_detectsaver[iY].rgContent[iH];
					m_detectsaver[iY].rgContent[iH] = rgContentTemp[iH];
				}
			}
		}
	}

}
//TODO: ���ݳ��Ƴ��ֵ�ʱ���ж�
bool CFinalResultParam::IsTwinResult(const CTrackInfo &TrackInfo)
{
	int iRoad = MatchRoad(TrackInfo.m_rgPlateInfo[0].rcPos.CenterPoint().x, TrackInfo.m_rgPlateInfo[0].rcPos.CenterPoint().y);

	if(m_iTimeinCount <= 0)
	{
		return false;
	}
	//�ж��Ƿ�����һ����һ��ʱ����ͨ������ �ȱȽϺ����������
	if(m_iSendTwinResult)
	{	//���ó��Ƶ�ʱ����Ϊ����  
		DWORD32 dwTimeRef = 0;//TrackInfo.m_dwBestSnapShotRefTime;
		//���¶���
		UpdateDetectSaver(dwTimeRef);
	
		//�ȽϽ��
		int iCount = 0;
		//HV_Trace("���и��º����鳤��:%d\n",m_iTimeinCount);
		for(int ix=0; ix<m_iTimeinCount; ix++)
		{
			for(int iy=0; iy<8; iy++)
			{
				if(m_detectsaver[ix].rgContent[iy] == TrackInfo.m_rgbVotedResult[iy])
				{
					iCount++;
				}
			}
			if(iCount == 8)
			{
				// ���Ӿ��컹��Ҫ�ж�ͬһ�������ڲ���
				if(GetCurrentMode() == PRM_ELECTRONIC_POLICE)
				{
					if(iRoad == MatchRoad(m_detectsaver[ix].rcPos.CenterPoint().x, m_detectsaver[ix].rcPos.CenterPoint().y))
					{
						return true;
					}
				}
				else
				{
					return true;
				}
			}
			else
			{
				iCount = 0;
			}
		}

		//�������������
		if(dwTimeRef != 0)
		{
			m_detectsaver[m_iTimeinCount].dw_TrackEndTime = dwTimeRef;
		}
		else
		{
			m_detectsaver[m_iTimeinCount].dw_TrackEndTime = GetSystemTick();
		}

		for(int i = 0; i < TrackInfo.m_cPlateInfo; i++)
		{
			if(TrackInfo.m_rgPlateInfo[i].nInfoType == PI_LPR)
			{
				m_detectsaver[m_iTimeinCount].rcPos = TrackInfo.m_rgPlateInfo[i].rcPos;
			}
		}

		for(int i=0; i<8; i++)
		{
			m_detectsaver[m_iTimeinCount].rgContent[i] = TrackInfo.m_rgbVotedResult[i];
		}

		//����
		SetOrder();
		
		m_iTimeinCount ++;
	}

	// �ж��Ƿ�����һ����ͨ����������
	if ( m_iSendTwinResult &&
		GetSystemTick() - m_dwLastResultTime >= ( DWORD32 )( m_iBlockTwinsTimeout * 1000 )
		)
	{
		return false;
	}
	// �����ͣ��״̬�������
	if (m_fCarIsStop)
	{
		return false;
	}
	CRect rcLastEnd=LastInfo().rcPos;
	CRect rcThisBegin=TrackInfo.m_rgPlateInfo[0].rcPos;
	int cDiff(0);

	// TODO: Magic Number
	for (int i=0; i<7; i++)
	{
		if (TrackInfo.m_rgbVotedResult[i]!=m_rgbVotedResult[i])
		{
			cDiff++;
		}
	}

	if (cDiff == 0)
	{
		// ���Ӿ��컹��Ҫ�ж�ͬһ�������ڲ���
		if(GetCurrentMode() == PRM_ELECTRONIC_POLICE)
		{
			if(iRoad == MatchRoad(rcLastEnd.CenterPoint().x, rcLastEnd.CenterPoint().y))
			{
				return true;
			}
		}
		else
		{
			return true;
		}
	}
	if (cDiff > 2)
	{
		return false;
	}

	rcThisBegin.InflateRect(
		(m_pParam->g_fltTrackInflateX*rcThisBegin.Width()).ToInt(),
		(m_pParam->g_fltTrackInflateY*rcThisBegin.Height()).ToInt()
		);

	switch (m_pParam->g_MovingDirection)
	{
	case MD_TOP2BOTTOM:			// ��Զ����
		if (rcThisBegin.bottom >= rcLastEnd.top)
		{
			return true;
		}
		break;
	case MD_BOTTOM2TOP:			// �ɽ���Զ
		if (rcThisBegin.top <= rcLastEnd.bottom)
		{
			return true;
		}
		break;
	case MD_LEFT2RIGHT:			// ������
		if (rcThisBegin.right >= rcLastEnd.left)
		{
			return true;
		}
		break;
	case MD_RIGHT2LEFT:			// ���Ҽ���
		if (rcThisBegin.left <= rcLastEnd.right)
		{
			return true;
		}
		break;
	}

	return false;
}


HRESULT CFinalResultParam::UpdateResult(const CTrackInfo &TrackInfo)
{
	// TODO: Did nothing, just block the subsequent result
	return S_OK;
}

HRESULT CreateTracker_Video(ITracker** ppTracker, int iVideoID/* = 0*/)
{
	if (ppTracker == NULL)
	{
		return E_POINTER;
	}

	CTrackerImpl *pTrackerImpl = new CTrackerImpl();

	if (pTrackerImpl == NULL)
	{
		return E_OUTOFMEMORY;
	}

	//�������������
	RTN_HR_IF_FAILED(pTrackerImpl->OnCreate());

	// final assignment
	*ppTracker=pTrackerImpl;

	return S_OK;
}

CTrackerImpl::CTrackerImpl()
: m_fInitialized(FALSE)
, m_nWidth(0)
, m_nHeight(0)
, m_nStride(0)
, m_pParam(NULL)
, m_pPlateDetector(NULL)
, m_pProcessPlate(NULL)
, m_pRecognition(NULL)
, m_pScaleSpeed(NULL)
, m_cTrackInfo(0)
, m_cDetected(0)
, m_cObjTrackInfo(0)
, m_pInspector(NULL)
, m_pLastResult(NULL)
, m_fEnablePreciseRectify(FALSE)
, m_nVideoDetMode(NO_VIDEODET)
, m_pHvModel(NULL)
, m_LightType(DAY)	
, m_nCarLenTop(0)
, m_nCarLenBottom(0)
, m_nCarWTop(0)
, m_nCarWBottom(0)
, m_dwLastCarLeftTime(0)
, m_iLastLightStatus(-1)
, m_iCurLightStatus(-1)
, m_iLastOkLightStatus(-1)
, m_nFirstLightType(LIGHT_TYPE_COUNT)
, m_fOutputCarSize(FALSE)
, m_iLastTriggerTimeMs(0)
, m_iLastFirstTime(0)
, m_iLastSecondTime(0)
, m_iLastThirdTime(0)
,m_dwTriggerTimes(0)
,m_dwLastTriggerInfo(0)
,m_iRushSpeed(200)
,m_iVoteFrameCount(30)
,m_iCheckAcross(0)
,m_iFilterRushPeccancy(0)
,m_iCarArriveTrig(0)
,m_cEPObjTrackInfo(0)
,m_iNoPlateDetMode(0)
,m_iPulseLevel(0)
,m_iLightDetected(0)
,m_iEdgeFilterThreld(150)
, m_fEnableDetFace(FALSE)
, m_nMinFaceScale(2)
, m_nMaxFaceScale(20)
, m_cScanEP(0)
, m_iUsedLight(1)
, m_fIsModifyParam(false)

{
	srand(0);
	for (int i = 0; i < s_knMaxVideoChannel; i++)
	{
		m_rgpObjectDetector[i] = NULL;
	}
	m_PlateLightType = LIGHT_TYPE((int)LIGHT_TYPE_COUNT / 2);

	// ������Ŀ����������ʼ��
	m_iOutPutType = 0;
	m_iWalkManSpeed = 100;
	m_fltBikeSensitivity = 4.0;

	m_nIOLevel = 0;

	OnInit();
}

void CTrackerImpl::Clear()
{
	SAFE_DELETE(m_pPlateDetector);
	SAFE_DELETE(m_pProcessPlate);
	SAFE_DELETE(m_pRecognition);
	//	SAFE_DELETE(m_pPretreatment);
	SAFE_DELETE(m_pScaleSpeed);

	for (int i = 0; i < s_knMaxVideoChannel; i++)
	{
		if (m_rgpObjectDetector[i] != NULL)
		{
			m_rgpObjectDetector[i]->Release();
			m_rgpObjectDetector[i] = NULL;
		}
	}
}

CTrackerImpl::~CTrackerImpl()
{
	Clear();
}

//��������ģ��
HRESULT CTrackerImpl::OnCreate()
{
	Clear();

	//��ʼ����������
	GlobleDataSrcInit();

	//�������
	RTN_HR_IF_FAILED(CreatePlateDetectorInstance(&m_pPlateDetector));
	RTN_HR_IF_FAILED(CreateRecognition(&m_pRecognition));
	RTN_HR_IF_FAILED(CreateProcessPlateInstance(&m_pProcessPlate));
	//	RTN_HR_IF_FAILED(CreatePretreatmentInstance(&m_pPretreatment));

	RTN_HR_IF_FAILED(CreateScaleSpeedInstance(&m_pScaleSpeed));				// ���ٽӿ�

	for (int i = 0; i < s_knMaxVideoChannel; i++)
	{
		RTN_HR_IF_FAILED(CreateObjectDetector(&m_rgpObjectDetector[i]));
	}

	//���������ϵ
	RTN_HR_IF_FAILED(m_pProcessPlate->SetDetector(m_pPlateDetector));
	RTN_HR_IF_FAILED(m_pProcessPlate->SetRecognition(m_pRecognition));


	RTN_HR_IF_FAILED(m_cCarDetector.Init());

	return S_OK;
}

HRESULT CTrackerImpl::OnInit()
{
	//����
	//ClearTrackInfo();

	//�ڲ������ָ���ʼֵ
	m_fInitialized = FALSE;
	m_nWidth = 0;
	m_nHeight = 0;
	m_nStride = 0;
	m_cTrackInfo = 0;
	m_pLastResult = NULL;

	//�����ò���
	m_cDetAreaInfo.fEnableDetAreaCtrl = FALSE;
	PlateRecogMode prmCur = GetCurrentParam()->g_PlateRcogMode;
	if(  prmCur >= PRM_MaxMode )
	{
		m_cDetAreaInfo.DetectorAreaLeft = 0;
		m_cDetAreaInfo.DetectorAreaRight = 100;
		m_cDetAreaInfo.DetectorAreaTop = 0;
		m_cDetAreaInfo.DetectorAreaBottom = 40;
		m_cDetAreaInfo.nDetectorMinScaleNum = 6;
		m_cDetAreaInfo.nDetectorMaxScaleNum = 12;
	}
	else
	{
		m_cDetAreaInfo.DetectorAreaLeft = g_rgPlateRecogParam[prmCur].g_rectDetectionArea.left;
		m_cDetAreaInfo.DetectorAreaRight = g_rgPlateRecogParam[prmCur].g_rectDetectionArea.right;
		m_cDetAreaInfo.DetectorAreaTop = g_rgPlateRecogParam[prmCur].g_rectDetectionArea.top;
		m_cDetAreaInfo.DetectorAreaBottom = g_rgPlateRecogParam[prmCur].g_rectDetectionArea.bottom;
		m_cDetAreaInfo.nDetectorMinScaleNum = g_rgPlateRecogParam[prmCur].g_nDetMinScaleNum;
		m_cDetAreaInfo.nDetectorMaxScaleNum = g_rgPlateRecogParam[prmCur].g_nDetMaxScaleNum;
	}

	m_fEnableScaleSpeed = FALSE;
	m_fOutputCarSize = FALSE;

	// �Ƿ����������Ϣ
	m_fOutputDebugInfo = 0;

	// �Ƿ��жϹ�������־
	m_iRecogGxPolice = 1;

	//�¾���ʶ�𿪹�
	m_iRecogNewMilitary = 1;

	//��5λ�Ƿ���ĸʶ��
	m_fEnableAlpha_5 = FALSE;

	// ƽ���÷�����
	m_iAverageConfidenceQuan = 0;

	// ���ֵ÷�����
	m_iFirstConfidenceQuan = 0;

	// �Ƿ�������ͬ�ĳ��Ʋ��ܳ�����
	m_iSendTwinResult = 1;

	// ������ͬ���Ƴ��ֵ�ʱ����(��)
	m_iBlockTwinsTimeout = 30;

	//�¼�������
	CTrackInfo::m_ActionDetectParam.iDetectCarStopTime = 10;
	for (int i = 0; i < MAX_ROADLINE_NUM; i++)
	{
		CTrackInfo::m_ActionDetectParam.iIsCrossLine[i] = 0;
		CTrackInfo::m_ActionDetectParam.iIsYellowLine[i] = 0;
	}

	// ���ػ����֣�58Ϊ����
	m_nDefaultChnId = 58;

	//˫����
	m_iEnableDefaultDBType = 0;
	m_nDefaultDBType = 0;
	m_iMiddleWidth = 0;
	m_iBottomWidth = 0;

	//���Ƽ�ⷽ�����
	m_fMinVarianceUserCtrl = FALSE;
	m_iMinVariance = 9;

	m_iSpan = 5;

	m_fEnableNewWJSegment = TRUE;
	m_fEnableDefaultWJ    = TRUE;
	m_fEnableBigPlate = FALSE;
	HV_memset(m_rgiFiveCharWidth, 0, sizeof(m_rgiFiveCharWidth));
	HV_memset( m_strDefaultWJChar, 0, 3 );
	HV_memcpy( m_strDefaultWJChar, "16", 2);

	m_fEnablePlateEnhance = TRUE;
	m_iPlateResizeThreshold = 120;

	//�����ҹ��Ƶ������ֵ
	m_iNightThreshold = 55;
	//Ĭ��ֵȫ��Ϊ5
	m_iPlateLightCheckCount = 5;
	//����������ȣ����ڴ����Ȼ��л������ģʽ��
	m_iMinPlateBrightness = 80;
	//����������ȣ����ڴ����Ȼ��л������ģʽ)
	m_iMaxPlateBrightness = 120;
	//����ƫ�⾵
	m_iCtrlCpl = 0;
	m_iCplStatus = -1;
	m_iAdjustForAvgBrigtness = 0;
	m_dwAdjustTick = GetSystemTick();
	m_nLightTypeCpl = LIGHT_TYPE((int)LIGHT_TYPE_COUNT / 2);

	//�������ﴥ��λ��
	m_iCarArrivedPos = 50;
	m_iCarArrivedPosYellow = 40;
	m_iCarArrivedDelay = 0;

	memset(m_rgRedLightRect, 0, sizeof(m_rgRedLightRect));

	if(GetCurrentParam()->g_PlateRcogMode != PRM_TOLLGATE)
	{
		m_nVideoDetMode = 2;
	}
	else
	{
		m_nVideoDetMode = 0;
	}
	m_rcVideoDetArea.left = VIDEO_DETECT_LEFT;
	m_rcVideoDetArea.right = VIDEO_DETECT_RIGHT;
	m_rcVideoDetArea.top = VIDEO_DETECT_TOP;
	m_rcVideoDetArea.bottom = VIDEO_DETECT_BOTTOM;

	//��������
	m_nLightCount = 0;
	m_iLastLightStatus = -1;
	m_iCurLightStatus = -1;
	m_iLastOkLightStatus = -1;
	m_nRedLightCount = 0;
	//�����������С��ͬ����״̬��
	m_nValidSceneStatusCount = 5;
	m_fCheckSpeed = false;
	m_iSceneCount = 0;
	m_nErrorSceneCount = 0;

	m_iBigCarDelayFrameNo = 32;

	// ͶƱ��
	CTrackInfo::m_nVoteFrameNum = 35;
	// ���������ͬ, Vote
	CTrackInfo::m_nMaxEqualFrameNumForVote = 15;
	// �Ƿ����ͶƱ��������
	CTrackInfo::m_fOutputVoteCondition = 0;

	//ͶƱʱ��ȥ���÷���͵ĳ���
	CTrackInfo::m_nRemoveLowConfForVote = 40;

	// ʶ��÷ֵ�����ֵ(�ٷֱ�)����*��ʾ
	CTrackInfo::m_nRecogAsteriskThreshold = 0;

	CTrackInfo::m_iCarArrivedPos = 50;

	if( GetCurrentParam()->g_MovingDirection == MD_TOP2BOTTOM  )
	{
		CTrackInfo::m_iCaptureOnePos  = 45;
		CTrackInfo::m_iCaptureTwoPos  = 60;
		CTrackInfo::m_iCaptureThreePos = 80;
		m_fEnableProcessBWPlate = FALSE;
	}
	else if( GetCurrentParam()->g_MovingDirection == MD_BOTTOM2TOP )
	{
		CTrackInfo::m_iCaptureOnePos  = 80;
		CTrackInfo::m_iCaptureTwoPos  = 60;
		CTrackInfo::m_iCaptureThreePos = 45;
		m_fEnableProcessBWPlate = TRUE;
	}
	//���̵����
	CTrackInfo::m_iStopLinePos      = 65;
	CTrackInfo::m_iLeftStopLinePos = 65;
	CTrackInfo::m_iCheckType = 0;
	CTrackInfo::m_iAutoScanLight = 0;
	CTrackInfo::m_iSceneCheckMode = 0;
	CTrackInfo::m_iRedLightDelay = 0;

	CTrackInfo::m_iFlashlightMode = 0;
	CTrackInfo::m_iFlashlightThreshold = 25;
	CTrackInfo::m_iRealForward = 40;

#if RUN_PLATFORM == PLATFORM_WINDOWS
	CTrackInfo::m_iMatchScanSpeed = 1;
#else
	CTrackInfo::m_iMatchScanSpeed = 0;
#endif

	CTrackInfo::m_iSpecificLeft		= 0;
	CTrackInfo::m_iRunTypeEnable	= 0;

	CTrackInfo::m_iBestLightMode = 0;
	CTrackInfo::m_iBestLightWaitTime = 250;

	CObjectDetector::m_iDayBackgroudThreshold = 20;	//���챳�������ֵ
	CObjectDetector::m_iNightBackgroudThreshold = 50;	//���ϱ��������ֵ

	for(int i = 0; i < MAX_IO_LIGHT_COUNT; ++i)
	{
		m_rgIOLight[i].iPos = -1;
		m_rgIOLight[i].iTeam = -1;
		m_rgIOLight[i].tlsStatus = TLS_UNSURE;
		m_rgIOLight[i].tpPos = TP_UNSURE;
	}

	for(int i = 0; i < 4; ++i)
	{
		m_redrealtime.Reset();
		m_greentick.Reset();
	}

	m_iOutputInLine = 1;

	m_fltOverLineSensitivity = 2.75f;

	m_iOutPutType = 0;
	m_iWalkManSpeed = 100;
	m_fltBikeSensitivity = 4.0;

	m_iCheckAcross = 0;
	m_iVoteFrameCount = 30;

	m_iFilterRushPeccancy = 0;

	m_fLastResultInited = false;

	m_iCarArrivedPosNoPlate = 50;

	CTrackInfo::m_iTrafficLightThread = 3;
	CTrackInfo::m_iTrafficLightBrightValue = 3;
	CTrackInfo::m_iAutoScanThread = 8;
	m_fFilterMoreReview = FALSE;

	m_fUseEdgeMethod = FALSE;

	// zhaopy
	m_dwTriggerCameraTimes = 0;
	m_dwLastTriggerTick = 0;
	if(GetCurrentParam()->g_PlateRcogMode == PRM_CAP_FACE)
	{
		m_fEnableCarArriveTrigger = TRUE;
	}
	else
	{
		m_fEnableCarArriveTrigger = FALSE;
	}
	m_iCarArriveTrig = 0;
	m_iTriggerPlateType = 0;
	m_fltHMThreld = 0.48;       //������ֵ
	m_fltHMThreldNight = 0.48;  //ҹ����ֵ
	//ģ��ƥ��ֱ��ͼ�ӳٸ���֡��
	m_HMDelay = 8;             //�����ӳ�֡��
	m_HMDelayNight = 8;        //ҹ���ӳ�֡��
	m_iMaxMatchDiff = 18;		// ������������ֵ

	m_fFilterNoPlatePeccancy = FALSE;
	//������ɫʶ�����
	m_nEnableColorReRecog = 0;
	m_nWGColorReThr =  60;
	m_nWColorReThr = 150;
	m_nBColorReThr = 20;
	m_fEnableCorlorThr = false;
	m_nBGColorThr = 25;
	m_nWGColorThr = 150;
	
	// �Ƿ�ʹ�ò����
	m_iUsedLight = 1;
	
	SetSVCallBack();
	return S_OK;
}

//����������ᵼ��ʶ�����޷�����,������������Ч�Դ�?
#define TRIM_PARAM(v, min, max) ( v = MIN_INT(MAX_INT((min), v), (max)) )


//ΪHvCoreʵ�ֵĶ�̬�޸Ĳ����ӿ�
HRESULT CTrackerImpl::ModifyTracker(TRACKER_CFG_PARAM* pCfgParam)
{
	if (pCfgParam == NULL)
	{
		return E_INVALIDARG;
	}

	m_pParam = GetCurrentParam();

	//�ָ���ʼ״̬
	// ����Ѿ���ʼ���˾Ͳ������˲�����
	if( !m_fInitialized )
	{
		OnInit();
	}
	//��·��Ϣ����
	CTrackInfo::m_iRoadNumber = pCfgParam->nRoadLineNumber;
	CTrackInfo::m_iStartRoadNum = pCfgParam->iStartRoadNum;
	CTrackInfo::m_iRoadNumberBegin = pCfgParam->iRoadNumberBegin;
	for(int i = 0; i < CTrackInfo::s_iMaxRoadNumber; i++)
	{
		CTrackInfo::m_roadInfo[i] = pCfgParam->rgcRoadInfo[i];
		CTrackInfo::m_roadInfo[i].ptBottom.y /=2;
		CTrackInfo::m_roadInfo[i].ptTop.y /=2;
	}

	// ganzz 02-19
	CObjectDetector::m_iRoadNumber = CTrackInfo::m_iRoadNumber;

	//����ץ��ͼλ��
	CTrackInfo::m_iCaptureOnePos = pCfgParam->nCaptureOnePos;
	CTrackInfo::m_iCaptureTwoPos = pCfgParam->nCaptureTwoPos;
	CTrackInfo::m_iCaptureThreePos = pCfgParam->nCaptureThreePos;

	//������п���
	m_pParam->m_iDetectReverseEnable = pCfgParam->nDetReverseRunEnable;
	m_iSpan = pCfgParam->nSpan;

	//ָ����ʻ����
	m_TravelDirection = (MovingDirection)pCfgParam->iMovingDirection;

	//�շ�վģʽ�½���ץ��
	m_iCarArriveTrig = pCfgParam->nCarArriveTrig;
	if(pCfgParam->nCarArriveTrig == 1)
	{
		m_fEnableCarArriveTrigger = true;
	}
	else if(pCfgParam->nCarArriveTrig == 0)
	{
		m_fEnableCarArriveTrigger = false;
	}
	//����ץ�ĵĳ�������,�ƹ�������
	m_iTriggerPlateType = pCfgParam->nTriggerPlateType;

	//ѹ�������Ȳ���
	m_fltOverLineSensitivity = pCfgParam->fltOverLineSensitivity;

	//�¼�������
	if (m_pParam->m_iDetectCrossLineEnable)
	{
		for (int i = 0; i < MAX_ROADLINE_NUM; i++)
		{
			CTrackInfo::m_ActionDetectParam.iIsCrossLine[i] = pCfgParam->cActionDetect.iIsCrossLine[i];
		}
	}
	if (m_pParam->m_iDetectOverYellowLineEnable)
	{
		for (int i = 0; i < MAX_ROADLINE_NUM; i++)
		{
			CTrackInfo::m_ActionDetectParam.iIsYellowLine[i] = pCfgParam->cActionDetect.iIsYellowLine[i];
		}
	}
	CTrackInfo::m_ActionDetectParam.iDetectCarStopTime = pCfgParam->cActionDetect.iDetectCarStopTime;

	//ɨ���������
	m_cDetAreaInfo = pCfgParam->cDetectArea;

	//��ʶ��ɨ���������
	m_cRecogSnapArea = pCfgParam->cRecogSnapArea;

	TRAFFIC_LIGHT_PARAM tlpTrafficLightParam;

	//����Ƶ����
	if(GetCurrentMode() == PRM_ELECTRONIC_POLICE)
	{
		CObjectDetector::m_iEPOutPutDetectLine = pCfgParam->cVideoDet.iEPOutPutDetectLine;
		CObjectDetector::m_iEPFGDetectLine = pCfgParam->cVideoDet.iEPFGDetectLine;
		CTrackInfo::m_iCheckType = pCfgParam->cTrafficLight.nCheckType;
		tlpTrafficLightParam.iCheckType = pCfgParam->cTrafficLight.nCheckType;
		CTrackInfo::m_iAutoScanLight = pCfgParam->cTrafficLight.nAutoScanLight;
		tlpTrafficLightParam.iAutoScanLight = pCfgParam->cTrafficLight.nAutoScanLight;
		tlpTrafficLightParam.fDeleteLightEdge = pCfgParam->cTrafficLight.fDeleteLightEdge;
		CTrackInfo::m_iSceneCheckMode = pCfgParam->cTrafficLight.nSenceCheckMode;
		tlpTrafficLightParam.iSceneCheckMode = pCfgParam->cTrafficLight.nSenceCheckMode;
		CTrackInfo::m_iStopLinePos = pCfgParam->cTrafficLight.nStopLinePos;
		CTrackInfo::m_iLeftStopLinePos = pCfgParam->cTrafficLight.nLeftStopLinePos;
		CTrackInfo::m_iMatchScanSpeed = pCfgParam->cTrafficLight.nMatchScanSpeed;
		CTrackInfo::m_iFlashlightMode = pCfgParam->fEnableFlashLight;
		CTrackInfo::m_iFlashlightThreshold = pCfgParam->nFlashLightThreshold;
		// zhaopy
		CTrackInfo::m_iRealForward = pCfgParam->cTrafficLight.iRealForward;
		m_fltHMThreld = (float)((float)pCfgParam->cTrafficLight.fltHMThreld / 100.0);
		m_fltHMThreldNight = (float)((float)pCfgParam->cTrafficLight.fltHMThreldNight / 100.0);
		m_HMDelay = pCfgParam->cTrafficLight.iHMDelay;
		m_HMDelayNight = pCfgParam->cTrafficLight.iHMDelayNight;
		m_iMaxMatchDiff = pCfgParam->cTrafficLight.iMaxMatchDiff;
		m_nValidSceneStatusCount = pCfgParam->cTrafficLight.nValidSceneStatusCount;
		if(CTrackInfo::m_iCheckType == 1)
		{
			m_nValidSceneStatusCount = 1;
		}
		tlpTrafficLightParam.iValidSceneStatusCount = m_nValidSceneStatusCount;
		//������õ���Ч����С��5����ʹ�ÿ���ת��ģʽ������ת��ģʽ�³����仯һ����˳��ġ�
		if( m_nValidSceneStatusCount < 5 )
		{
			m_fCheckSpeed = true;
			tlpTrafficLightParam.fCheckSpeed = m_fCheckSpeed;
		}
		m_iOutputInLine = pCfgParam->cTrafficLight.nOutputInLine;
		CTrackInfo::m_iRedLightDelay = pCfgParam->cTrafficLight.nRedLightDelay;
		tlpTrafficLightParam.iRedLightDelay = pCfgParam->cTrafficLight.nRedLightDelay;
		CTrackInfo::m_iSpecificLeft = pCfgParam->cTrafficLight.nSpecificLeft;
		CTrackInfo::m_iRunTypeEnable = pCfgParam->cTrafficLight.nRunTypeEnable;
		m_nLightCount = pCfgParam->cTrafficLight.nLightCount;
		tlpTrafficLightParam.iLightGroupCount = m_nLightCount;
		CTrackInfo::m_iTrafficLightThread = pCfgParam->cTrafficLight.nTrafficLightThread;
		CTrackInfo::m_iTrafficLightBrightValue = pCfgParam->cTrafficLight.nTrafficLightBrightValue;
		tlpTrafficLightParam.iTrafficLightBrightValue = pCfgParam->cTrafficLight.nTrafficLightBrightValue;
		CTrackInfo::m_iAutoScanThread = pCfgParam->cTrafficLight.nAutoScanThread;
		tlpTrafficLightParam.iAutoScanThread = pCfgParam->cTrafficLight.nAutoScanThread;
		tlpTrafficLightParam.iSkipFrameNo = pCfgParam->cTrafficLight.nSkipFrameNO;
		tlpTrafficLightParam.fIsLightAdhesion = pCfgParam->cTrafficLight.fIsLightAdhesion;

		m_iMinPlateBrightness = pCfgParam->nMinPlateBrightness;
		m_iMaxPlateBrightness = pCfgParam->nMaxPlateBrightness;

		m_iFilterRushPeccancy = pCfgParam->cTrafficLight.nFilterRushPeccancy;
		m_fFilterMoreReview = pCfgParam->cTrafficLight.nFilterMoreReview;

		// ģ�忪ʼ���ߡ�
		CTrackInfo::m_nTrafficLightTempleYPos = CTrackInfo::m_iStopLinePos + 16;
		if( CTrackInfo::m_nTrafficLightTempleYPos > 85 ) CTrackInfo::m_nTrafficLightTempleYPos = 85;

		int p1(0),p2(0),p3(0),p4(0),p5(0),p6(0);
		char szLightState[32] = {0};
		
		//���ú��̵�״̬
		for(int i = 0; i < MAX_TRAFFICLIGHT_COUNT; i++)
		{
			m_rgLightInfo[i].Reset();
		}

		//ClearTrafficLightType();

		for(int i = 0; i < m_nLightCount; i++)
		{
			sscanf(
				pCfgParam->cTrafficLight.rgszLightPos[i], 
				"(%d,%d,%d,%d),%d,%d,%s",&p1,&p2,&p3,&p4,&p5,&p6,
				szLightState
			);
			//����,ID,����,����(0=����,1=����)
			m_rgLightInfo[i].SetRect(p1,p2,p3,p4,MIN_INT(CTrafficLightInfo::MAX_POS_COUNT, p5), p6);
			
			CRect rcTmp = CRect(p1,p2,p3,p4);
			tlpTrafficLightParam.rgtgiLight[i].iLightCount = MIN_INT(CTrafficLightInfo::MAX_POS_COUNT, p5);
			tlpTrafficLightParam.rgtgiLight[i].iLightType = p6;
			tlpTrafficLightParam.rgtgiLight[i].rcLight = rcTmp;

			if( strlen(szLightState) >= (p5 * 2) )
			{
				int index = 0;
				for(int k = 0; k < p5; ++k)
				{
					tlpTrafficLightParam.rgtgiLight[i].tpLightPos[k] = (TRAFFICLIGHT_POSITION)(szLightState[index] - '0');
					tlpTrafficLightParam.rgtgiLight[i].lsLightStatus[k] = (_TRAFFICLIGHT_LIGHT_STATUS)(szLightState[index+1] - '0');						

					if( (_TRAFFICLIGHT_LIGHT_STATUS)(szLightState[index+1] - '0') == TLS_RED 
						&& (m_nRedLightCount < MAX_TRAFFICLIGHT_COUNT * 2) )
					{
						if( p6 == 0 )
						{
							int iTmpX = ((p3 - p1) / p5);
							m_rgRedLightRect[m_nRedLightCount].left = iTmpX * k + p1;
							m_rgRedLightRect[m_nRedLightCount].right = iTmpX * (k + 1) + p1;
							m_rgRedLightRect[m_nRedLightCount].top = p2;
							m_rgRedLightRect[m_nRedLightCount].bottom = p4;
							m_nRedLightCount++;
						}
						else if( p6 == 1)
						{
							int iTmpY = ((p4 - p2) / p5);
							m_rgRedLightRect[m_nRedLightCount].left = p1;
							m_rgRedLightRect[m_nRedLightCount].right = p3;
							m_rgRedLightRect[m_nRedLightCount].top = iTmpY * k + p2;
							m_rgRedLightRect[m_nRedLightCount].bottom = iTmpY * (k + 1) + p2;
							m_nRedLightCount++;
						}
					}
					//SetTrafficLightType(i, k, (TRAFFICLIGHT_POSITION)(szLightState[index] - '0'), (_TRAFFICLIGHT_LIGHT_STATUS)(szLightState[index+1] - '0'));
					index += 2;
				}
			}
		}

		//���IO���̵�����
		char szIOLight[64] = {0};
		strcpy(szIOLight, pCfgParam->cTrafficLight.szIOConfig);
		if( strcmp(szIOLight, "00,00,00,00,00,00,00,00") != 0 )
		{
			int iIndex = (strlen(szIOLight) + 1) / 3;
			for(int i = 0; i < iIndex; ++i)
			{
				tlpTrafficLightParam.rgIOLight[i].iTeam = szIOLight[i * 3 + 0] - '0' - 1;
				tlpTrafficLightParam.rgIOLight[i].iPos = szIOLight[i * 3 + 1] - '0' - 1;
				tlpTrafficLightParam.rgIOLight[i].tpPos = tlpTrafficLightParam.rgtgiLight[tlpTrafficLightParam.rgIOLight[i].iTeam].tpLightPos[tlpTrafficLightParam.rgIOLight[i].iPos];
				tlpTrafficLightParam.rgIOLight[i].tlsStatus = tlpTrafficLightParam.rgtgiLight[tlpTrafficLightParam.rgIOLight[i].iTeam].lsLightStatus[tlpTrafficLightParam.rgIOLight[i].iPos]; 

				m_rgIOLight[i].iTeam = szIOLight[i * 3 + 0] - '0' - 1;
				m_rgIOLight[i].iPos = szIOLight[i * 3 + 1] - '0' - 1;

				//GetTrafficLightType(m_rgIOLight[i].iTeam, m_rgIOLight[i].iPos, &m_rgIOLight[i]);
			}
		}

		m_iSceneCount = pCfgParam->cTrafficLight.nSceneCount;
		tlpTrafficLightParam.iSceneCount = m_iSceneCount;

		char szOtherInfo[64] = {0};
		int rgiLightStatus[4] = {0};

		//ClearLightScene();
		//ClearSceneInfo();
		for(int i = 0; i < MAX_SCENE_COUNT; i++)
		{
			memset(szOtherInfo, 0, 64);
			int nCount = sscanf(
				pCfgParam->cTrafficLight.rgszScene[i], 
				"(%d,%d,%d,%d),%s",
				&rgiLightStatus[0],
				&rgiLightStatus[1],
				&rgiLightStatus[2],
				&rgiLightStatus[3],
				szOtherInfo
			);
			if( 4 <= nCount)
			{
				TRAFFICLIGHT_SCENE ts;
				ts.lsLeft = (LIGHT_STATUS)rgiLightStatus[0];
				ts.lsForward = (LIGHT_STATUS)rgiLightStatus[1];
				ts.lsRight = (LIGHT_STATUS)rgiLightStatus[2];
				ts.lsTurn = (LIGHT_STATUS)rgiLightStatus[3];
		  //	SetLightScene(i, ts);
				SCENE_INFO tsInfo;
				strncpy(tsInfo.pszInfo, szOtherInfo, strlen(szOtherInfo) + 1);
			//	SetSceneInfo(i, tsInfo);

				tlpTrafficLightParam.rgtsLightScene[i] = ts;
				memcpy(tlpTrafficLightParam.rgsiInfo[i].pszInfo, tsInfo.pszInfo, 64);
			}

			if (m_nLightCount > 0)
			{
				m_cTrafficLight.SetTrafficLightParam(tlpTrafficLightParam);
				m_cTrafficLight.SetTrafficLightCallback(this);
			}
		}
	}

	//��Ƶ������
	m_nVideoDetMode = pCfgParam->cVideoDet.nVideoDetMode;
	m_rcVideoDetArea = pCfgParam->cVideoDet.rcVideoDetArea;
	CObjectDetector::m_iDayBackgroudThreshold = pCfgParam->cVideoDet.nDayBackgroudThreshold;
	CObjectDetector::m_iNightBackgroudThreshold = pCfgParam->cVideoDet.nNightBackgroudThreshold;
	m_iVoteFrameCount = pCfgParam->cVideoDet.nVoteFrameCount;
	m_iCheckAcross = pCfgParam->cVideoDet.nCheckAcross;
	CObjectDetector::m_iShadowDetect = pCfgParam->cVideoDet.nShadowDetMode;
	CObjectDetector::m_iShadowEffect = pCfgParam->cVideoDet.nShadowDetEffect;
	CObjectDetector::m_iDiffType = pCfgParam->cVideoDet.nDiffType;
	CObjectDetector::m_iMedFilter = pCfgParam->cVideoDet.nMedFilter;
	CObjectDetector::m_iMedFilterUpLine = pCfgParam->cVideoDet.nMedFilterUpLine;
	CObjectDetector::m_iMedFilterDownLine = pCfgParam->cVideoDet.nMedFilterDownLine;
	m_iBigCarDelayFrameNo = pCfgParam->cVideoDet.nBigCarDelayFrameNo;

	if (m_cDetAreaInfo.fEnableDetAreaCtrl)
	{
		m_pParam->g_rectDetectionArea.left = m_cDetAreaInfo.DetectorAreaLeft;
		m_pParam->g_rectDetectionArea.right = m_cDetAreaInfo.DetectorAreaRight;
		m_pParam->g_rectDetectionArea.top = m_cDetAreaInfo.DetectorAreaTop;
		m_pParam->g_rectDetectionArea.bottom = m_cDetAreaInfo.DetectorAreaBottom;
		m_pParam->g_nDetMinScaleNum = m_cDetAreaInfo.nDetectorMinScaleNum;
		m_pParam->g_nDetMaxScaleNum = m_cDetAreaInfo.nDetectorMaxScaleNum;
	}

	if (m_fMinVarianceUserCtrl)
	{
		m_pParam->g_nDetMinStdVar = m_iMinVariance;
	}
	else
	{
		m_iMinVariance = m_pParam->g_nDetMinStdVar;
	}
	m_iNoPlateDetMode = pCfgParam->cVideoDet.iNoPlateDetMode;
	CTrackInfo::m_fEPUseCarTailModel = pCfgParam->cVideoDet.iEPUseCarTailModel;
	CTrackInfo::m_iEPNoPlateScoreThreld = pCfgParam->cVideoDet.iEPNoPlateScoreThreld;

	PROCESS_PLATE_CFG_PARAM cfgProcessPlateParam;
	RECOGNITION_CFG_PARAM cfgRecognitionParam;
	PLATE_DETECT_CFG_PARAM cfgPlateDetectParam;
	SCALESPEED_CFG_PARAM cfgScaleSpeedParam;

	//ProcessPlate
	memcpy(&m_cfgParam, pCfgParam, sizeof(m_cfgParam));
	cfgProcessPlateParam.nBlackPlate_S = pCfgParam->nProcessPlate_BlackPlate_S;
	cfgProcessPlateParam.nBlackPlate_L = pCfgParam->nProcessPlate_BlackPlate_L;
	cfgProcessPlateParam.nBlackPlateThreshold_H0 = pCfgParam->nProcessPlate_BlackPlateThreshold_H0;
	cfgProcessPlateParam.nBlackPlateThreshold_H1 = pCfgParam->nProcessPlate_BlackPlateThreshold_H1;
	cfgProcessPlateParam.nProcessPlate_LightBlue = pCfgParam->nProcessPlate_LightBlue;
	cfgProcessPlateParam.fEnableDoublePlate = pCfgParam->fDoublePlateEnable;
	cfgProcessPlateParam.iSegWhitePlate = pCfgParam->nSegWhitePlate; //���Ʒָ�
    cfgProcessPlateParam.fltMinWHRatioCoef = 0.5f; //֧����б��

	//Recognition
	cfgRecognitionParam.fEnableAlphaRecog = pCfgParam->fAlpha7;
	cfgRecognitionParam.fEnableT1Model = pCfgParam->fEnableT1Model;
	cfgRecognitionParam.fEnableAlpha_5 = pCfgParam->fEnableAlpha_5;

	//Detector
	cfgPlateDetectParam.nPlateDetect_Green = pCfgParam->nPlateDetect_Green;

	m_iNightThreshold = pCfgParam->nNightThreshold;
	m_iPlateLightCheckCount = pCfgParam->nPlateLightCheckCount;
	m_iMinPlateBrightness = pCfgParam->nMinPlateBrightness;
	m_iMaxPlateBrightness = pCfgParam->nMaxPlateBrightness;
	m_iCtrlCpl = pCfgParam->nCtrlCpl;
	if(m_iCtrlCpl == 1)
	m_iCplStatus = 0;
	m_nLightTypeCpl = (LIGHT_TYPE)pCfgParam->nLightTypeCpl;
	m_iForceLightOffAtDay = pCfgParam->iForceLightOffAtDay;
	m_iForceLightOffThreshold = pCfgParam->iForceLightThreshold;
	m_iForceLightOffFlag = 0;
	// �Ƿ�ʹ�ò����
	m_iUsedLight = pCfgParam->iUsedLight;
	m_fEnablePlateEnhance = pCfgParam->nEnablePlateEnhance;
	m_iPlateResizeThreshold = pCfgParam->nPlateResizeThreshold;
	m_fEnableBigPlate = pCfgParam->nEnableBigPlate;
	CTrackInfo::m_nRecogAsteriskThreshold = pCfgParam->nRecogAsteriskThreshold;
	m_fUseEdgeMethod = pCfgParam->fUseEdgeMethod;
	CTrackInfo::m_fEnableRecgCarColor = pCfgParam->fEnableRecgCarColor;
	m_fEnableDefaultWJ = pCfgParam->fEnableDefaultWJ;
	strncpy(m_strDefaultWJChar, pCfgParam->szDefaultWJChar, 3);
	m_fEnableProcessBWPlate = pCfgParam->nEnableProcessBWPlate;
	m_iRecogGxPolice = pCfgParam->nRecogGxPolice;
	m_iRecogNewMilitary = pCfgParam->nRecogNewMilitary;
	m_fEnableAlpha_5 = pCfgParam->fEnableAlpha_5;
	m_iCarArrivedPos = pCfgParam->nCarArrivedPos;
	CTrackInfo::m_iCarArrivedPos = m_iCarArrivedPos;
	m_iCarArrivedPosNoPlate = pCfgParam->nCarArrivedPosNoPlate;
	CObjectDetector::m_iProcessType = pCfgParam->nProcessType;
	m_iOutPutType = pCfgParam->nOutPutType;
	CObjectDetector::m_iNightPlus = pCfgParam->nNightPlus;
	m_iWalkManSpeed = pCfgParam->nWalkManSpeed;
	m_fltBikeSensitivity = pCfgParam->fltBikeSensitivity;
	m_iCarArrivedDelay = pCfgParam->nCarArrivedDelay;
	CTrackInfo::m_nVoteFrameNum = pCfgParam->nVoteFrameNum;
	CTrackInfo::m_nMaxEqualFrameNumForVote = pCfgParam->nMaxEqualFrameNumForVote;
	m_iBlockTwinsTimeout = pCfgParam->nBlockTwinsTimeout;
	m_iAverageConfidenceQuan = pCfgParam->nAverageConfidenceQuan;
	m_iFirstConfidenceQuan = pCfgParam->nFirstConfidenceQuan;
	CTrackInfo::m_nRemoveLowConfForVote = pCfgParam->nRemoveLowConfForVote;
	m_fOutputCarSize = pCfgParam->fOutputCarSize;
	m_iEnableDefaultDBType = pCfgParam->nDoublePlateTypeEnable;
	m_nDefaultDBType = pCfgParam->nDefaultDBType;
	m_iMiddleWidth = pCfgParam->nMiddleWidth;
	m_iBottomWidth = pCfgParam->nBottomWidth;
	m_fEnableDetFace = pCfgParam->fEnableDetFace;
	m_nMinFaceScale = pCfgParam->nMinFaceScale;
	m_nMaxFaceScale = pCfgParam->nMaxFaceScale;
	m_fFilterNoPlatePeccancy = pCfgParam->cTrafficLight.fFilterNoPlatePeccancy;

	//������ٲ���
	cfgScaleSpeedParam.nMethodsFlag = pCfgParam->cScaleSpeed.nMethodsFlag;
	cfgScaleSpeedParam.fltLeftDistance = pCfgParam->cScaleSpeed.fltLeftDistance;
	cfgScaleSpeedParam.fltFullDistance = pCfgParam->cScaleSpeed.fltFullDistance;
	cfgScaleSpeedParam.fltCameraHigh = pCfgParam->cScaleSpeed.fltCameraHigh;
	cfgScaleSpeedParam.fltAdjustCoef = pCfgParam->cScaleSpeed.fltAdjustCoef;
	m_fEnableScaleSpeed = pCfgParam->cScaleSpeed.fEnable;
	
	if (GetCurrentParam()->g_PlateRcogMode == PRM_TOLLGATE)
	{
		m_iCarArrivedPosYellow = pCfgParam->nCarArrivedPos - pCfgParam->iBuleTriggerToYellowOffset > 0 ?
			pCfgParam->nCarArrivedPos - pCfgParam->iBuleTriggerToYellowOffset : 0;
	}
	else
	{
		m_iCarArrivedPosYellow = pCfgParam->nCarArrivedPosYellow;
	}

	//������ɫʶ�����
	m_nEnableColorReRecog = pCfgParam->nEnableColorReRecog;
	m_nWGColorReThr =  pCfgParam->nWGColorReThr;
	m_nWColorReThr = pCfgParam->nWColorReThr;
	m_nBColorReThr = pCfgParam->nBColorReThr;
	m_fEnableCorlorThr = pCfgParam->fEnableCorlorThr;
	m_nBGColorThr = pCfgParam->nBGColorThr;
	m_nWGColorThr = pCfgParam->nWGColorThr;

	for(int i = 0; i < 11; i++)
	{
		cfgScaleSpeedParam.rgfltTransMarix[i] = pCfgParam->cScaleSpeed.rgfltTransMarix[i];
	}

	if(GetCurrentMode() != PRM_HVC)
	{
		// ��ץ��ģʽ
		if (!((m_fOutputCarSize == TRUE) && (m_nVideoDetMode == USE_VIDEODET)))
		{
			m_fOutputCarSize = FALSE;
		}
	}
	CFinalResultParam::m_iBlockTwinsTimeout = m_iBlockTwinsTimeout;
	CFinalResultParam::m_iSendTwinResult = m_iSendTwinResult;

	//�������ʼ��
	// TODO magic number
	{
		RTN_HR_IF_FAILED(m_pPlateDetector->Initialize(m_pParam, &cfgPlateDetectParam, 0, 0));
		RTN_HR_IF_FAILED(m_pRecognition->InitRecognition(&cfgRecognitionParam));
		RTN_HR_IF_FAILED(m_pProcessPlate->Initialize(NO_PREDICT_PROCESS_PLATE_MODE, &cfgProcessPlateParam));
	}
	RTN_HR_IF_FAILED(m_pScaleSpeed->InitScaleSpeed(&cfgScaleSpeedParam));			// ���ٽӿڳ�ʼ��

	//�����������
	for (int i=0; i<s_knMaxTrackInfo; i++)
	{
		m_rgTrackInfo[i].m_pParam=m_pParam;
	}

	// ����ģʽ���ɳ������Զ�����ɨ���������Ƶ�������
	if(GetCurrentParam()->g_PlateRcogMode == PRM_CAP_FACE)
	{
		// �����������
		CalcTrapDetArea(&m_cfgParam.cTrapArea,  CTrackInfo::m_iCaptureOnePos, m_nWidth, m_nHeight);
	}
	else if (GetCurrentParam()->g_PlateRcogMode == PRM_TOLLGATE)
	{
		CalcTLTrapDetArea(
				&m_cfgParam.cTrapArea,
				m_iCarArrivedPos-pCfgParam->iScanAreaTopOffset > 0 ? m_iCarArrivedPos-pCfgParam->iScanAreaTopOffset : 0,
				m_iCarArrivedPos+pCfgParam->iScanAreaBottomOffset < 99 ? m_iCarArrivedPos+pCfgParam->iScanAreaBottomOffset : 99,
				m_nWidth,
				m_nHeight);
		
		// ���ݾ����������������
		m_cDetAreaInfo.DetectorAreaLeft = MIN(m_cfgParam.cTrapArea.TopLeftX, m_cfgParam.cTrapArea.BottomLeftX);
		m_cDetAreaInfo.DetectorAreaTop = MIN(m_cfgParam.cTrapArea.TopLeftY, m_cfgParam.cTrapArea.BottomLeftY);
		m_cDetAreaInfo.DetectorAreaRight = MAX(m_cfgParam.cTrapArea.TopRightX, m_cfgParam.cTrapArea.BottomRightX);
		m_cDetAreaInfo.DetectorAreaBottom = MAX(m_cfgParam.cTrapArea.TopRightY, m_cfgParam.cTrapArea.BottomRightY);

		m_pParam->g_rectDetectionArea.left = m_cDetAreaInfo.DetectorAreaLeft;
		m_pParam->g_rectDetectionArea.right = m_cDetAreaInfo.DetectorAreaRight;
		m_pParam->g_rectDetectionArea.top = m_cDetAreaInfo.DetectorAreaTop;
		m_pParam->g_rectDetectionArea.bottom = m_cDetAreaInfo.DetectorAreaBottom;

		m_rcVideoDetArea.left = m_cDetAreaInfo.DetectorAreaLeft;
		m_rcVideoDetArea.top = m_cDetAreaInfo.DetectorAreaTop;
		m_rcVideoDetArea.right = m_cDetAreaInfo.DetectorAreaRight;
		m_rcVideoDetArea.bottom = m_cDetAreaInfo.DetectorAreaBottom;
	}

	m_fIsModifyParam = true;

	return S_OK;
}

//��ʼ��TRACKER�͸����
HRESULT CTrackerImpl::InitTracker(TRACKER_CFG_PARAM* pCfgParam)
{
	m_fInitialized = FALSE;

	RTN_HR_IF_FAILED(ModifyTracker(pCfgParam));


	m_fInitialized = TRUE;

	return S_OK;
}

//����ģ�ͼ��ؽӿ�
HRESULT CTrackerImpl::SetHvModel(HvCore::IHvModel* pHvModel)
{
	m_pHvModel = pHvModel;

	if (m_pPlateDetector) m_pPlateDetector->SetHvModel(m_pHvModel);
	if (m_pRecognition) m_pRecognition->SetHvModel(m_pHvModel);
	return S_OK;
}

HRESULT CTrackerImpl::SetHvPerformance(HvCore::IHvPerformance* pHvPerf)
{
	m_pHvPerf = pHvPerf;

	if (m_pPlateDetector) m_pPlateDetector->SetHvPerformance(pHvPerf);
	if (m_pRecognition) m_pRecognition->SetHvPerformance(pHvPerf);

	if (m_pProcessPlate) m_pProcessPlate->SetHvPerformance(pHvPerf);

	return S_OK;
}

//���ù���ģ�����
HRESULT CTrackerImpl::SetLPRMode(PlateRecogMode nMode)
{
	HRESULT hr;

	DWORD32 dwParamCount(0);
	GetPlateRecogParamCount(&dwParamCount);

	if (nMode < 0 || nMode >= (int)dwParamCount)
	{
		hr = E_INVALIDARG;
	}
	else
	{
		LoadPlateRecogParam(nMode);
		m_pParam = GetCurrentParam();

		if (m_cDetAreaInfo.fEnableDetAreaCtrl)
		{
			m_pParam->g_rectDetectionArea.left = m_cDetAreaInfo.DetectorAreaLeft;
			m_pParam->g_rectDetectionArea.right = m_cDetAreaInfo.DetectorAreaRight;
			m_pParam->g_rectDetectionArea.top = m_cDetAreaInfo.DetectorAreaTop;
			m_pParam->g_rectDetectionArea.bottom = m_cDetAreaInfo.DetectorAreaBottom;
			m_pParam->g_nDetMinScaleNum = m_cDetAreaInfo.nDetectorMinScaleNum;
			m_pParam->g_nDetMaxScaleNum = m_cDetAreaInfo.nDetectorMaxScaleNum;
		}

		if (m_fMinVarianceUserCtrl)
		{
			m_pParam->g_nDetMinStdVar = m_iMinVariance;
		}

//		if (m_pHvParam)
//		{
//			m_pHvParam->SetInt("Tracker", "PlateRecogMode", nMode);
//			m_pHvParam->Save(0);
//		}

		hr = S_OK;
	}

	return hr;
}

HRESULT CTrackerImpl::SetFrameProperty(
									   const FRAME_PROPERTY& FrameProperty,
									   PROCESS_ONE_FRAME_RESPOND* pProcessRespond
									   )
{
	ClearTrackInfo(pProcessRespond);

	m_nWidth = FrameProperty.iWidth;
	m_nHeight = FrameProperty.iHeight;
	m_nStride = FrameProperty.iStride;

	CTrackInfo::m_iHeight = m_nHeight;
    CTrackInfo::m_iWidth = m_nWidth;

	if (m_pPlateDetector)
	{
		m_pPlateDetector->SetImgSize(m_nWidth, m_nHeight);
	}
	if (m_fEnableScaleSpeed)
	{
		// ��ʼ����������
		m_pScaleSpeed->SetFrameProperty(FrameProperty);
	}

	// �����жϲ�������
	SetCarTypeParam(m_nWidth, m_nHeight);

	// ����ģʽ���ɳ������Զ�����ɨ���������Ƶ�������
	if(GetCurrentParam()->g_PlateRcogMode == PRM_CAP_FACE)
	{
		// �����������
		CalcTrapDetArea(&m_cfgParam.cTrapArea,  CTrackInfo::m_iCaptureOnePos, m_nWidth, m_nHeight);
	}
	else if (GetCurrentParam()->g_PlateRcogMode == PRM_TOLLGATE)
	{
		CalcTLTrapDetArea(
				&m_cfgParam.cTrapArea,
				m_iCarArrivedPos-m_cfgParam.iScanAreaTopOffset > 0 ? m_iCarArrivedPos-m_cfgParam.iScanAreaTopOffset : 0,
				m_iCarArrivedPos+m_cfgParam.iScanAreaBottomOffset < 99 ? m_iCarArrivedPos+m_cfgParam.iScanAreaBottomOffset : 99,
				m_nWidth,
				m_nHeight);

		CalcPlateScale(&m_cfgParam.cTrapArea, m_nWidth, &m_pParam->g_nDetMinScaleNum, &m_pParam->g_nDetMaxScaleNum);

		// ���ݾ����������������
		m_cDetAreaInfo.DetectorAreaLeft = MIN(m_cfgParam.cTrapArea.TopLeftX, m_cfgParam.cTrapArea.BottomLeftX);
		m_cDetAreaInfo.DetectorAreaTop = MIN(m_cfgParam.cTrapArea.TopLeftY, m_cfgParam.cTrapArea.BottomLeftY);
		m_cDetAreaInfo.DetectorAreaRight = MAX(m_cfgParam.cTrapArea.TopRightX, m_cfgParam.cTrapArea.BottomRightX);
		m_cDetAreaInfo.DetectorAreaBottom = MAX(m_cfgParam.cTrapArea.TopRightY, m_cfgParam.cTrapArea.BottomRightY);

		m_pParam->g_rectDetectionArea.left = m_cDetAreaInfo.DetectorAreaLeft;
		m_pParam->g_rectDetectionArea.right = m_cDetAreaInfo.DetectorAreaRight;
		m_pParam->g_rectDetectionArea.top = m_cDetAreaInfo.DetectorAreaTop;
		m_pParam->g_rectDetectionArea.bottom = m_cDetAreaInfo.DetectorAreaBottom;

		m_rcVideoDetArea.left = m_cDetAreaInfo.DetectorAreaLeft;
		m_rcVideoDetArea.top = m_cDetAreaInfo.DetectorAreaTop;
		m_rcVideoDetArea.right = m_cDetAreaInfo.DetectorAreaRight;
		m_rcVideoDetArea.bottom = m_cDetAreaInfo.DetectorAreaBottom;
	}

	return S_OK;
}

HRESULT CTrackerImpl::SetCharSet(ChnCharSet nCharSet)
{
	return m_pRecognition->SetCharSet(nCharSet);
}

HRESULT CTrackerImpl::SetVideoDet(BOOL fEnabled, BOOL fSkipFrame , int nDetLeft, int nDetRight)
{
	return S_OK;
}

HRESULT CTrackerImpl::SetCalibrateParam(BOOL fEnable, int nRotate, int nTilt, BOOL fOutputImg)
{
	//	m_fEnablePreCalibrate = fEnable;
	//	m_fOutputCalibrateImg = fOutputImg;
	//	if (m_pHvParam)
	//	{
	//		m_pHvParam->SetInt("Tracker\\PreTreatment","Enable", m_fEnablePreCalibrate);
	//		m_pHvParam->SetInt("Tracker\\PreTreatment","OutputImg", m_fOutputCalibrateImg);
	//		m_pHvParam->Save(0);
	//	}

	//	if (m_pPretreatment)
	//	{
	//		m_pPretreatment->SetParam(nRotate,nTilt);
	//	}

	return S_OK;
}

HRESULT CTrackerImpl::SetPreSegmentParam(BOOL fEnable)
{
	m_fEnablePreciseRectify = fEnable;
	return S_OK;
}

HRESULT CTrackerImpl::SetDetArea(	const DETECTAREA& cDetAreaInfo)
{
	m_cDetAreaInfo = cDetAreaInfo;

	if (m_cDetAreaInfo.fEnableDetAreaCtrl)
	{
		m_pParam->g_rectDetectionArea.left = m_cDetAreaInfo.DetectorAreaLeft;
		m_pParam->g_rectDetectionArea.right = m_cDetAreaInfo.DetectorAreaRight;
		m_pParam->g_rectDetectionArea.top = m_cDetAreaInfo.DetectorAreaTop;
		m_pParam->g_rectDetectionArea.bottom = m_cDetAreaInfo.DetectorAreaBottom;
		m_pParam->g_nDetMinScaleNum = m_cDetAreaInfo.nDetectorMinScaleNum;
		m_pParam->g_nDetMaxScaleNum = m_cDetAreaInfo.nDetectorMaxScaleNum;
	}
	else
	{
//		LoadPlateRecogParam(GetWorkMode());
		m_pParam = GetCurrentParam();
	}

//	if (m_pHvParam)
//	{
//		m_pHvParam->SetInt("Tracker\\DetAreaCtrl","Enable", m_cDetAreaInfo.fEnableDetAreaCtrl);
//		m_pHvParam->SetInt("Tracker\\DetAreaCtrl","Left", m_cDetAreaInfo.DetectorAreaLeft);
//		m_pHvParam->SetInt("Tracker\\DetAreaCtrl","Right", m_cDetAreaInfo.DetectorAreaRight);
//		m_pHvParam->SetInt("Tracker\\DetAreaCtrl","Top", m_cDetAreaInfo.DetectorAreaTop);
//		m_pHvParam->SetInt("Tracker\\DetAreaCtrl","Bottom", m_cDetAreaInfo.DetectorAreaBottom);
//		m_pHvParam->SetInt("Tracker\\DetAreaCtrl","MinScale", m_cDetAreaInfo.nDetectorMinScaleNum);
//		m_pHvParam->SetInt("Tracker\\DetAreaCtrl","MaxScale", m_cDetAreaInfo.nDetectorMaxScaleNum);
//		m_pHvParam->Save(0);
//	}

	if (m_fMinVarianceUserCtrl)
	{
		m_pParam->g_nDetMinStdVar = m_iMinVariance;
	}

	return S_OK;
}

//TODO:	������������Ƿ����Ƶ����ģ����
HRESULT CTrackerImpl::GetInflateRect(
									 CRect &rect,			// in, out.
									 PLATE_COLOR color,
									 PLATE_TYPE &PlateType,
									 INT maxRectHeight		// �ж��Ƿ���inflate rate2
									 )
{
	int nWidth(rect.Width());
	int nHeight(rect.Height());

	if (	PLATE_DOUBLE_YELLOW == PlateType ||
		PLATE_DOUBLE_MOTO == PlateType )
	{
		rect.left -= (nWidth * 80) >> 10;
		rect.right += (nWidth * 80) >> 10;
		rect.top -= (nHeight * 120) >> 10;
		rect.bottom += (nHeight * 40) >> 10;
	}
	else if (PLATE_DOUBLE_GREEN == PlateType)
	{
		rect.right += (nWidth * 40) >> 10;
		rect.top += (nHeight * 130) >> 10;
		rect.bottom -= (nHeight * 20) >> 10;
	}
	else
	{
		rect.left -= (nWidth * 120) >> 10;
		rect.right += (nWidth * 120) >> 10;
	}

	nHeight = rect.Height();
	if (nHeight >= 100)		//100Ϊ�ָ�ʱ����ͼ���߶�
	{
		rect.top += (nHeight - 100) >> 1;
		rect.bottom -= nHeight - 100 - ((nHeight - 100) >> 1);
	}
	nWidth = rect.Width();
	if (nWidth >= 400)		//400Ϊ�ָ�ʱ����ͼ�����
	{
		rect.left += (nWidth - 399) >> 1;
		rect.right -= nWidth - 399 - ((nWidth - 399) >> 1);
	}

	return S_OK;
}

HRESULT CTrackerImpl::RecognizePlate(
									 HV_COMPONENT_IMAGE imgPlate,
									 PlateInfo &Info,
									 CPersistentComponentImage& imgRectified
									 )
{
	HV_DebugInfo(DEBUG_STR_TRACK_FILE, "Enter RecognizePlate()\n");

	if (imgPlate.iHeight <= 0 || imgPlate.iWidth <= 0
        || imgPlate.iWidth * 1 < imgPlate.iHeight * 3 && imgPlate.iWidth < 120)  // �쳣��ȳ��Ʋ�����, ganzz140708
	{
		return S_FALSE;
	}

	CPersistentComponentImage imgYUVPlate;
	if (imgPlate.nImgType != HV_IMAGE_YUV_422)
	{
		imgYUVPlate.Create(HV_IMAGE_YUV_422, imgPlate.iWidth, imgPlate.iHeight);
		imgYUVPlate.Convert(imgPlate);
		imgPlate = (HV_COMPONENT_IMAGE)imgYUVPlate;
	}
	HV_COMPONENT_IMAGE imgOrigPlate = imgPlate;
	CPersistentComponentImage imgBlackWhitePlate;
	if (m_fEnableProcessBWPlate && DAY == m_LightType)
	{
		imgBlackWhitePlate.Assign(imgPlate);
		ProcessBWPlate(&imgBlackWhitePlate);
		imgPlate = (HV_COMPONENT_IMAGE)imgBlackWhitePlate;
	}

	CPersistentComponentImage imgEnhancePlate;
	if (m_fEnablePlateEnhance)
	{
		imgEnhancePlate.Assign(imgPlate);
		PlateEnhance(&imgEnhancePlate);
		imgPlate = (HV_COMPONENT_IMAGE)imgEnhancePlate;
	}

	int iLastCharHeight=Info.iLastCharHeight;
	float fltPlateScaled = 1.0f;
	CPersistentComponentImage imgSample;
	/*
	int nStandarPlateWidth = m_iPlateResizeThreshold;
	if (imgPlate.iWidth > 250)
	{
	    nStandarPlateWidth = 250;	
	}

	if (imgPlate.iWidth < nStandarPlateWidth || imgPlate.iWidth > 250)
	{
		fltPlateScaled = imgPlate.iWidth / (float)nStandarPlateWidth;
		imgSample.Create(HV_IMAGE_YUV_422, nStandarPlateWidth, (int)(imgPlate.iHeight / fltPlateScaled));
		ImageScaleSize(imgSample, imgPlate);
		imgPlate = (HV_COMPONENT_IMAGE)imgSample;
		iLastCharHeight = (int)(iLastCharHeight / fltPlateScaled);
	}
	*/

	if (imgPlate.iWidth < m_iPlateResizeThreshold)
	{
		fltPlateScaled = imgPlate.iWidth / (float)m_iPlateResizeThreshold;
		imgSample.Create(HV_IMAGE_YUV_422, m_iPlateResizeThreshold, (int)(imgPlate.iHeight / fltPlateScaled));
		ImageScaleSize(imgSample, imgPlate);
		imgPlate = (HV_COMPONENT_IMAGE)imgSample;
		iLastCharHeight = (int)(iLastCharHeight / fltPlateScaled);
	}
	if (imgPlate.iHeight <= MIN_SEG_IMG_HIGH)
	{
		return S_FALSE;
	}

	if (m_pInspector)
	{
		RTN_HR_IF_FAILED(m_pInspector->ShowComponentImage(ipOriginalPlate, &imgPlate));
	}

	DWORD32 dwTickCount = GetSystemTick();

	RTN_HR_IF_FAILED(m_pProcessPlate->SetWorkMode(
		m_pParam->g_fSegPredict ? FORWARD_PREDICT_PROCESS_PLATE_MODE :NO_PREDICT_PROCESS_PLATE_MODE
		));

	CRectifyInfo RectifyInfo;
	CBinaryInfo BinaryInfo;

	RTN_HR_IF_FAILED(imgRectified.Assign(imgPlate));

	// ���ڳ�ͷ ���� ʹ���µĳ��ƽ�������
	if (m_LightType == NIGHT 
		&& (GetCurrentParam()->g_PlateRcogMode == PRM_CAP_FACE || GetCurrentParam()->g_PlateRcogMode == PRM_HIGHWAY_HEAD))
	{	
		RTN_HR_IF_FAILED(m_pProcessPlate->CalRectifyInfoKL(&imgPlate, &RectifyInfo, &(Info.nPlateType), &(Info.color)));
		if (RectifyInfo.fltHAngle > 4)
		{
			RectifyInfo.fltHAngle = 4;
		}
		else if (RectifyInfo.fltHAngle < -4)
		{
			RectifyInfo.fltHAngle = -4;
		}
//		RectifyInfo.fltVAngle = 90 + RectifyInfo.fltHAngle;		// ԭ��ֱ���������� ��ֱ�����ο�ˮƽ����
		RectifyInfo.fltVAngle = (RectifyInfo.fltVAngle - 90) / 2 + 90;	// ��ֱ�Ƕ�ֻҪһ��
	}
	else
	{
	RTN_HR_IF_FAILED(m_pProcessPlate->CalcRectifyInfo(&imgPlate,
		&imgRectified, &RectifyInfo, &BinaryInfo, &(Info.nPlateType), 
		&(Info.color), false, m_fEnableBigPlate));
	}

	// TODO: Why none constant RectifyInfo?
	// ASSUME: The rectified image won't be larger than the original one

	imgRectified.iHeight=imgPlate.iHeight;
	imgRectified.iWidth=imgPlate.iWidth;

	RTN_HR_IF_FAILED(m_pProcessPlate->RectifyPlate(
		&imgPlate, &RectifyInfo, &imgRectified, m_fEnableBigPlate
		));

	HV_DebugInfo(	DEBUG_STR_TRACK_FILE,
		"Rectify Info: HAngle=%f, VAngle=%f, RectifiedWidth=%d, RectifiedHeight=%d\n",
		RectifyInfo.fltHAngle, RectifyInfo.fltVAngle, imgRectified.iWidth, imgRectified.iHeight
		);


	HV_DebugInfo( DEBUG_STR_TIME_FILE,
		"Calc Rectify Time = %d\n",

		GetSystemTick()-dwTickCount
		);

	if (m_pInspector)
	{
		RTN_HR_IF_FAILED(m_pInspector->ShowComponentImage(ipRectified, &imgRectified));
	}

	int iMaxScore = 0;

	int iNormalFiveCharWidth(0);
	if (m_iEnableDefaultDBType > 0 &&
		m_pParam->g_MovingDirection == MD_BOTTOM2TOP)
	{
		// ��βĦ��ģʽ
		int iOffset = (Info.rcPos.bottom
			+ Info.rcPos.top) / 50;
		iNormalFiveCharWidth = m_rgiFiveCharWidth[iOffset];
	}

	int iFastSegMode(2);

	const unsigned int knPlateStrLen = 8;		// TODO: Magic Number
	CRect rgrcCharPos[knPlateStrLen];
	HV_RECT *pCharPos=rgrcCharPos;
	//�����Ƴ�δ������������ڳ�ͷ�Ϳ�������ץ��ģʽ�£��ڶ�������ΪTRUE
	//ʹ������������Ϊfalseʹ�þɷ���
	//�����޸�
	if((GetCurrentParam()->g_PlateRcogMode == PRM_HIGHWAY_HEAD ||GetCurrentParam()->g_PlateRcogMode == PRM_CAP_FACE)
		&& m_fUseEdgeMethod )
	{
		m_pProcessPlate->SetSrcImgGray(m_iAvgY, true);
	}
	
	if(Info.nPlateType == PLATE_DOUBLE_YELLOW &&
		MD_BOTTOM2TOP != m_pParam->g_MovingDirection)
	{
		iNormalFiveCharWidth = 1; //�ǳ�βģʽ���ж�Ħ������
	}

	RTN_HR_IF_FAILED(m_pProcessPlate->SegmentPlate(
		&imgRectified, &(Info.color), &(Info.nPlateType),
		&iMaxScore, &iLastCharHeight, &(Info.iLastVscale),
		&BinaryInfo, &pCharPos, iFastSegMode,
		(m_LightType == NIGHT),
		iNormalFiveCharWidth
		));


	if ((Info.nPlateType == PLATE_DOUBLE_MOTO)
		&& (!(m_iEnableDefaultDBType > 0 && MD_BOTTOM2TOP == m_pParam->g_MovingDirection)))
	{
		// ��Ħ�г�βģʽ, û��Ħ�г���
		Info.nPlateType = PLATE_DOUBLE_YELLOW;
	}

	Info.iLastCharHeight = (int)(iLastCharHeight * fltPlateScaled);

	//��˫���ơ���������ɫ����Ϊ��ɫ
	if (Info.nPlateType == PLATE_INDIVIDUAL)
		Info.color = PC_WHITE;

	{
		HV_DebugInfo(DEBUG_STR_TRACK_FILE, "�ָ�����\n");
		HV_DebugInfo( DEBUG_STR_TRACK_FILE ,
			"Type=%d, Color=%d\n",
			Info.nPlateType, Info.color
			);
		for (int i=0; i<g_cnPlateChars; i++)
		{
			HV_DebugInfo( DEBUG_STR_TRACK_FILE,
				"[%02d] (%d, %d) -> (%d, %d)\n",
				i, rgrcCharPos[i].left, rgrcCharPos[i].top,
				rgrcCharPos[i].right, rgrcCharPos[i].bottom
				);
		}
	}

	HV_DebugInfo( DEBUG_STR_TIME_FILE, "Segmentation Time = %d\n", GetSystemTick()-dwTickCount );

	// ����ָ�ʧ��
	if (iMaxScore<0 || Info.nPlateType==PLATE_UNKNOWN)
	{
		HV_DebugInfo( DEBUG_STR_TRACK_FILE, "�ָ�ʧ�� iMaxScore=%d\n", iMaxScore);

		for (int i=0; i<g_cnPlateChars; i++)
		{
			Info.rgbContent[i] = 0;
			Info.rgfltConfidence[i] = 0.0f;
		}
		Info.fltTotalConfidence = 0.0f;

		return S_FALSE;
	}

	int iCharNum = (Info.nPlateType == PLATE_DOUBLE_GREEN) ? 8 : 7;
	if (m_pInspector)
	{
		CPersistentComponentImage imgSegResult;
		RTN_HR_IF_FAILED(imgSegResult.Assign(imgRectified));

		for (int i = 0; i < iCharNum; i++)
		{
			MarkRect(imgSegResult, rgrcCharPos[i], 240);
		}

		if (m_fOutputDebugInfo)
		{
			//TODO:	��������ģ��
			CRect rcPlatePos(imgRectified.iWidth + 1, imgRectified.iHeight+1, -1, -1);
			rcPlatePos.left = rgrcCharPos[0].left;
			rcPlatePos.right = rgrcCharPos[6].right;
			rcPlatePos.top = (rgrcCharPos[0].top < rgrcCharPos[6].top) ?
				rgrcCharPos[0].top : rgrcCharPos[6].top;
			rcPlatePos.bottom = (rgrcCharPos[0].bottom > rgrcCharPos[6].bottom) ?
				rgrcCharPos[0].bottom : rgrcCharPos[6].bottom;

			// ���㳵���ַ�����
			CFastMemAlloc FastStack;
			HV_COMPONENT_IMAGE *pImgSeg = &imgRectified;
			int iSumL = 0;
			int iWidth = rcPlatePos.right - rcPlatePos.left;
			int iHeight = rcPlatePos.bottom - rcPlatePos.top;
			int iSize = iWidth * iHeight;

			BYTE8 *hsbData = ( BYTE8* )FastStack.StackAlloc(iSize * 3);
			if (NULL == hsbData)
			{
				return E_OUTOFMEMORY;
			}

			if (ConvertYCbCr2HSB(iWidth * 3,
				&hsbData[0],
				pImgSeg,
				rcPlatePos.left, rcPlatePos.top, iHeight, iWidth) == S_OK)
			{
				for ( int i =0; i < iSize; i++)
				{
					iSumL += hsbData[i * 3 + 2];
				}
			}
			iSumL /= iSize;
			Info.iHL = iSumL;

		}

		RTN_HR_IF_FAILED(m_pInspector->ShowComponentImage(ipSegmented, &imgSegResult, &Info));
	}

	// �����ݶ�λ��
	GetPlateNail(Info.color, imgRectified, rgrcCharPos);

	dwTickCount = GetSystemTick();

	CRect rcPlatePos(imgRectified.iWidth+1, imgRectified.iHeight+1, -1, -1);
	Info.fltTotalConfidence = 1.0f;
	int n1Count(0);
    int nInValidCount = 0;    // ��ʶ��Ч��
    int nValidNum1Count = 0;  // ��ʶ��Ч��Ϊ1�ĸ���

	bool bIsOne[10] = {0};//added by liujie ��������1�ĸ���
	for (int i=0; i<iCharNum; i++)
	{
		if (rcPlatePos.left>rgrcCharPos[i].left)
		{
			rcPlatePos.left=rgrcCharPos[i].left;
		}
		if (rcPlatePos.top>rgrcCharPos[i].top)
		{
			rcPlatePos.top=rgrcCharPos[i].top;
		}
		if (rcPlatePos.right<rgrcCharPos[i].right)
		{
			rcPlatePos.right=rgrcCharPos[i].right;
		}
		if (rcPlatePos.bottom<rgrcCharPos[i].bottom)
		{
			rcPlatePos.bottom=rgrcCharPos[i].bottom;
		}

		Info.rgrcChar[i].SetRect( 
			(int)(rgrcCharPos[i].left * fltPlateScaled),
			(int)(rgrcCharPos[i].top * fltPlateScaled),
			(int)(rgrcCharPos[i].right * fltPlateScaled),
			(int)(rgrcCharPos[i].bottom * fltPlateScaled)
			);

		HV_COMPONENT_IMAGE imgChar;
		imgChar.nImgType = HV_IMAGE_GRAY;

		SetHvImageData(&imgChar, 0, GetHvImageData(&imgRectified, 0)+rgrcCharPos[i].left+
			rgrcCharPos[i].top*imgRectified.iStrideWidth[0]);
		imgChar.iWidth=rgrcCharPos[i].Width();
		imgChar.iHeight=rgrcCharPos[i].Height();
		imgChar.iStrideWidth[0]=imgRectified.iStrideWidth[0];

		SDWORD32 swProb;
		bool fRecogChar = true;		// ʶ���ַ�����

		RECOGNITION_TYPE rtToRecog;
		if (i >= knPlateStrLen - 1)
		{
			rtToRecog = RECOG_DIGIT;
		}
		else
		{
			rtToRecog = (RECOGNITION_TYPE)Plate::g_cnType[Info.nPlateType][i];
		}

		if ((4 == i || 5 == i) && PC_LIGHTBLUE == Info.color && PLATE_WJ != Info.nPlateType && PLATE_POLICE2 != Info.nPlateType)
		{
			rtToRecog = (RECOGNITION_TYPE)Plate::g_cnType[PLATE_NORMAL][i];
		}

		//��0λ�ǳ�βģʽʹ��ȫ����ģ��
		/*if(	 i == 0 && MD_BOTTOM2TOP != m_pParam->g_MovingDirection && PLATE_DOUBLE_YELLOW == Info.nPlateType)
		{
			rtToRecog = RECOG_CHN;
		}*/

		// �������� ǰ�������
		switch(Info.nPlateType)
		{
		case PLATE_NORMAL:			// ������
			if (i == 6)
			{
				if ((Info.rgbContent[0] == 57 ) && (Info.rgbContent[1] == 36))
				{
					//���ӶԸ۰��Ƶ�֧��,�۰���Ϊ"��Z-****��"��"��Z-****��"
					rtToRecog = RECOG_GA;
				}
			}
			break;
		case PLATE_POLICE:							// ���ơ��������ƺͰ���
		case PLATE_DOUBLE_YELLOW:					// ˫����
			if (m_fEnableAlpha_5 && ((4 == i) || (5 == i)))
			{
				rtToRecog = RECOG_ALL;
			}
			if( m_iRecogNewMilitary == 1 && i==0)  
			{
				if(Info.nPlateType == PLATE_POLICE)
				{
					rtToRecog = RECOG_CHNALHPA;
				}
				if(Info.nPlateType ==PLATE_DOUBLE_YELLOW && Info.color != PC_YELLOW)
				{
					rtToRecog = RECOG_CHNALHPAMT;
				}
			}

			// �Ծ�V ���;��Ƶ�֧��
			if (i >= 2 && (Info.rgbContent[0] == 38) && (Info.rgbContent[1] == 32))
			{
				rtToRecog = RECOG_DIGIT;		// 3~7 λȫ��ʹ������ʶ��
			}
			//��һλ�ַ�ʶ�𲢲�һ��׼ȷ����Ĭ��ʶ������ʶ�𼴿�
			if (i >= 2 && i<= 3 && (IsMilitaryAlpha(Info.rgbContent[0]) || IsMilitaryChn(Info.rgbContent[0])))  //�����¾������ַ�Ϊ��ĸ���ж�
			{
				// �����Ǿ��ƺ���
				rtToRecog = RECOG_DIGIT;		// 3~7 λȫ��ʹ������ʶ��
				break;
			}
			break;
		case PLATE_INDIVIDUAL:		// ���Ի�����
			if (i == 0)
			{
				//������ i==0 ʱʶ��ڶ�λ����ĸ
				rtToRecog = RECOG_ALPHA;
			}
			break;
		case PLATE_WJ:				// �侯����
			if ( m_fEnableNewWJSegment && 1 == i)
			{
				// ���侯����֧��
				rtToRecog = RECOG_CHNSF;
			}
			break;
		case PLATE_DOUBLE_WJ:
			if ( m_fEnableNewWJSegment && 1 == i)
			{
				// ���侯����֧��
				rtToRecog = RECOG_CHNMT;
			}
			break;
		case PLATE_POLICE2:
			if (i ==6)
			{
				// ��7λ����ʶ��
				Info.rgbContent[6] = 37;			// ǿ�����óɾ���
				fRecogChar = false;					// ��ʶ��
				swProb = 65535;						// ���Ŷ�
			}
			break;
		case PLATE_DOUBLE_MOTO:
			if( i == 4 || i == 5 || i == 6)
			{
				rtToRecog = RECOG_ALL;
			}
			break;
		default:
			break;
		}

		int nRecogCount(0);
		while(fRecogChar)
		{
			/*
			RTN_HR_IF_FAILED(m_pRecognition->Evaluate(
				&imgChar,
				rtToRecog,
				Info.nPlateType,
				Info.color,
				Info.rgbContent+i,
				&swProb,
				i
				));
				*/

			// zhaopy
			// ʶ����ܻ᷵��ʧ�ܣ����ָ�������ʱ���˴��账��������⡣
			// ��Ĭ�ϸ�һ���̶����ַ���
			if( S_OK != m_pRecognition->Evaluate(
				&imgChar,
				rtToRecog,
				Info.nPlateType,
				Info.color,
				Info.rgbContent+i,
				&swProb,
				i
				) )
			{
				// ���Ŷ�Ϊ0.
				swProb = 0;	
				// �̶����0�ַ�
				Info.rgbContent[i] = 1;
			}


			fRecogChar = false;
			nRecogCount++;

			if (nRecogCount > 1)
			{
				break;					// �ڶ���ʶ��ֱ���˳�
			}

			// ��Ҫ����ʶ��Ĺ���
			if ((PLATE_POLICE == Info.nPlateType) && (i == 6))
			{
				// ���ڹ����������һλ�Ĵ���������Ҫ����ʶ��
				if (Info.rgbContent[6] == 37)			// ���һλ�ǡ�����
				{
					// ֻ�д��ˡ��������ơ����ص�����£���������֡���*****��������
					// �ǡ�����ͷһ�ɲ���������ֽ��?
					if (!((m_iRecogGxPolice == 1) && (Info.rgbContent[0] == 58)))	// ���𡱣�������
					{
						// ʹ������ģ������ʶ��
						rtToRecog = RECOG_DIGIT;
						fRecogChar = true;
					}
				}
				else if(Info.rgbContent[6] == 45)	// li ����"��"���һλҲ����ĸ
				{
					rtToRecog = RECOG_ALL;
					fRecogChar = true;
				}
			} // if
		}

		// ȱʡ�侯�滻(���¹����޹�)
		//if ( m_fEnableNewWJSegment 
		//	  && ((m_fEnableDefaultWJ && PLATE_WJ == Info.nPlateType)  || (m_fEnableDefaultWJ && PLATE_DOUBLE_WJ == Info.nPlateType) ) //˫���侯ǿ���滻
		//	  && strlen(m_strDefaultWJChar) > 1)
		//{
		//	if (0 == i)
		//	{
		//		*(Info.rgbContent) = m_strDefaultWJChar[0] - 47;
		//	}
		//	if (1 == i)
		//	{
		//		*(Info.rgbContent + 1) = m_strDefaultWJChar[1] - 47;
		//	}
		//}

		////�������Ĭ���侯С����ʶ�������С��>32,��ǿ��Ϊ16
		//if(!m_fEnableDefaultWJ 
		//	&& (PLATE_WJ == Info.nPlateType || PLATE_DOUBLE_WJ == Info.nPlateType)
		//	&& (i == 1 && (Info.rgbContent[0] == 4 && Info.rgbContent[1] > 3 || Info.rgbContent[0] > 4))) 
		//{
		//	Info.rgbContent[0] = '1' - 47;
		//	Info.rgbContent[1] = '6' - 47;
		//}

		//ʶ��˫����ʱ����E��ʶ���A��Bʱ�����ٴ�ʶ��(���¹����޹�)
		if (Info.nPlateType == PLATE_DOUBLE_YELLOW && i == 1 &&
			( Info.rgbContent[1] == 11 || Info.rgbContent[1] == 12 )
			&& imgChar.iHeight >= 5
			)
		{
			BYTE8 bContentTemp(Info.rgbContent[i]);
			SDWORD32 swProbTemp(swProb);

			imgChar.iHeight--;

			/*
			RTN_HR_IF_FAILED(m_pRecognition->Evaluate(
				&imgChar,
				rtToRecog,
				Info.nPlateType,
				Info.color,
				Info.rgbContent+i,
				&swProb,
				i
				));
				*/

			if( S_OK != m_pRecognition->Evaluate(
				&imgChar,
				rtToRecog,
				Info.nPlateType,
				Info.color,
				Info.rgbContent+i,
				&swProb,
				i) )
			{
				// ���Ŷ�Ϊ0.
				swProb = 0;	
				// �̶����0�ַ�
				Info.rgbContent[i] = 1;
			}

			if (Info.rgbContent[i] != 15)
			{
				Info.rgbContent[i] = bContentTemp;
				swProb = swProbTemp;
			}
		}

		Info.rgfltConfidence[i]=(float)swProb/(1<<QUANTIFY_DIGIT);
		// ���ʶ���1,���ʵ����Ϳ��Ŷ�
		if (Info.rgbContent[i] == 2)
		{
			n1Count++;
			bIsOne[i] = true;
		}

		// ��ʶ,����Ƿ�Ϊ�����ַ�
        int fValid = TRUE;
        BOOL fIsWhite = (Info.color == PC_BLUE || Info.color == PC_BLACK);
        m_pRecognition->CheckValid(imgChar, fIsWhite, Info.rgbContent[i], &fValid);
        if(!fValid)
        { 
            Info.rgfltConfidence[i] = Info.rgfltConfidence[i].ToFloat() * 0.9;
            nInValidCount++;
        }
        else
        {
        	// Ϊ'1'������ж�
        	if (Info.rgbContent[i] == 2)
        	{
        		nValidNum1Count++;
        	}
        }

		HV_DebugInfo( DEBUG_STR_TRACK_FILE,
			"[%02d] Content=%d Prob=%d\n",
			i, Info.rgbContent[i], swProb
			);

		// WJ��ʽ�����м���֣������õ�һ����
		if ((Info.nPlateType == PLATE_WJ && i>1) ||
			(Info.nPlateType == PLATE_DOUBLE_GREEN && i > 2) ||
			(Info.nPlateType != PLATE_WJ && Info.nPlateType != PLATE_DOUBLE_GREEN && i>0))
		{
			Info.fltTotalConfidence*=Info.rgfltConfidence[i];
		}

		if (Info.fltTotalConfidence<m_pParam->g_fltThreshold_BeObservation)
		{
			HV_DebugInfo( DEBUG_STR_TRACK_FILE,
				"�����뿪RecognizePlate(), "
				"TotalConf<fltThreshold_BeObservation\n"
				"i=%d, TotalConf=%f\n",
				i, Info.fltTotalConfidence.ToFloat()
				);

			return S_FALSE;
		}
	}

	// �����λʶ���1,���ʵ����Ϳ��Ŷ�
	//if (n1Count > 3 && m_pParam->g_PlateRecogSpeed == PR_SPEED_NORMAL)
	//{
	//	Info.fltTotalConfidence*= 0.8f;
	//}
	//added by liujie
	//�����������1�������ﵽ3����1���Ϳ��Ŷ�
	int iMax1CC = 0;
	int tempCC = 0;
	for (int i = 0; i < iCharNum; i++)
	{
		if(bIsOne[i])
		{
			tempCC++;
		} else if(i < iCharNum - 1){
			if (tempCC > iMax1CC)
			{
				iMax1CC = tempCC;
			}
			tempCC = 0;
		} 
	}
	iMax1CC = tempCC>iMax1CC?tempCC:iMax1CC;
	if (iMax1CC >= 3)
	{
		Info.fltTotalConfidence*= 0.5f;
	}
	//end added

	if (Info.rgbContent[0] >= 103 && Info.rgbContent[0] <= 113
		&& (37 == Info.rgbContent[6] || PLATE_NORMAL == Info.nPlateType) )
	{
		Info.rgbContent[0] = m_nDefaultChnId;
	}

	Info.rcPlatePos = rcPlatePos;

	//���㳵������
	int iPlateYSum = 0, iPlatePixelCount = 0;
	CRect rcOrigPlatePos(rcPlatePos);
	rcOrigPlatePos.left = (int)(rcOrigPlatePos.left * fltPlateScaled);
	rcOrigPlatePos.right = (int)(rcOrigPlatePos.right * fltPlateScaled);
	rcOrigPlatePos.top = (int)(rcOrigPlatePos.top * fltPlateScaled);
	rcOrigPlatePos.bottom = (int)(rcOrigPlatePos.bottom * fltPlateScaled);
	for (int y = rcOrigPlatePos.top; y < rcOrigPlatePos.bottom; y++)
	{
		for (int x = rcOrigPlatePos.left; x < rcOrigPlatePos.right; x++)
		{
			iPlatePixelCount++;
			iPlateYSum += GetHvImageData(&imgOrigPlate, 0)[y * imgOrigPlate.iStrideWidth[0] + x];
		}
	}
	Info.nAvgY = iPlateYSum / iPlatePixelCount;

	HV_DebugInfo(	DEBUG_STR_TRACK_FILE,
		"PlatePos: (%d, %d) - (%d, %d)\n",
		rcPlatePos.left, rcPlatePos.top, rcPlatePos.right, rcPlatePos.bottom
		);

	rcPlatePos.InflateRect(rcPlatePos.Width()/8, rcPlatePos.Height()/4);
	if (rcPlatePos.IntersectRect(rcPlatePos, CRect(0, 0, imgRectified.iWidth, imgRectified.iHeight)))
	{
		HV_COMPONENT_IMAGE imgCropped;
		imgCropped.nImgType = HV_IMAGE_YUV_422;
		CropImage(imgRectified, rcPlatePos, &imgCropped);

		// �����ָ������
		Info.rcPlatePos.OffsetRect(-rcPlatePos.left, -rcPlatePos.top);	
		//Info.rcPlatePos = rcPlatePos;

		CPersistentComponentImage imgTemp;
		imgTemp.Assign(imgCropped);
		imgTemp.Detach(imgRectified);
	}

	Info.rcPlatePos.left = (int)(Info.rcPlatePos.left * fltPlateScaled);
	Info.rcPlatePos.right = (int)(Info.rcPlatePos.right * fltPlateScaled);
	Info.rcPlatePos.top = (int)(Info.rcPlatePos.top * fltPlateScaled);
	Info.rcPlatePos.bottom = (int)(Info.rcPlatePos.bottom * fltPlateScaled);
    Info.nInValidCount = nInValidCount;
    if (nInValidCount == 3)
    {
    	if (nValidNum1Count > 1)
    	{
    		Info.nInValidCount += nValidNum1Count - 1;
    	}
    }

	HV_DebugInfo(DEBUG_STR_TIME_FILE, "Recognition Time = %d\n", GetSystemTick()-dwTickCount);

	HV_DebugInfo(DEBUG_STR_TRACK_FILE, "==== BEGIN RecognizePlateResult\n");
	HV_DebugInfo(DEBUG_STR_TRACK_FILE,
		"TotalConf=%f, Type=%d, Color=%d\n",
		Info.fltTotalConfidence.ToFloat(),
		Info.nPlateType, Info.color
		);

	for (int i=0; i<g_cnPlateChars; i++)
	{
		HV_DebugInfo( DEBUG_STR_TRACK_FILE,
			"Char [%d], Content=%d Conf=%f\n",
			i, Info.rgbContent[i], Info.rgfltConfidence[i].ToFloat());
	}
	HV_DebugInfo(DEBUG_STR_TRACK_FILE, "==== END RecognizePlateResult\n");


	return S_OK;
}

// �����ݶ���λ��
HRESULT CTrackerImpl::GetPlateNail(PLATE_COLOR color, 
								   HV_COMPONENT_IMAGE imgPlate, 
								   CRect* pCharPos)
{
	CRect rgNailPos[7];
	bool fIsNail = false;
	HV_RECT rgrcNailPos = {0,0,0,0};
	int rgBottomLine[8] = {0};
	int iCharNum = 7;
	if (color == PC_YELLOW || color == PC_WHITE || color == PC_LIGHTBLUE)
	{
		for (int i = 0; i < iCharNum; i++)
		{
			GetNailPos(imgPlate, pCharPos[i], &rgNailPos[i].top, &rgNailPos[i].bottom, &rgBottomLine[i], color);
		}
		int iCountTop = 0;
		int iCountDown = 0;
		int iPointTop = 0;
		int iPointDown = 0;
		for (int i = 0; i < iCharNum; i++)
		{
			if ((i != 1 || i != 5) 
				&& rgNailPos[1].top < rgNailPos[i].top && rgNailPos[5].top < rgNailPos[i].top
				&& rgNailPos[1].top == 0 && rgNailPos[5].top == 0)
			{
				if(rgNailPos[i].top)
					iPointTop += rgNailPos[i].top;
				iCountTop++;
			}
		}
		for (int i = 0; i < iCharNum; i++)
		{
			if ((i != 1 || i != 5) 
				&& rgNailPos[1].bottom > rgNailPos[i].bottom && rgNailPos[5].bottom > rgNailPos[i].bottom
				&& rgNailPos[1].bottom + 1== pCharPos[1].Height()  && rgNailPos[5].bottom  + 1 == pCharPos[5].Height())
			{
				iPointDown += rgNailPos[i].bottom;
				iCountDown++;
			}
		}

		if (iCountDown >= 4 || iCountTop >= 4)
		{
			fIsNail = true;
			if(iCountTop >= 4)
				rgrcNailPos.top = iPointTop / iCountTop;
			if(iCountDown >= 4)
				rgrcNailPos.bottom = (iPointDown + (iCountDown >> 1)) / iCountDown;	// ��������
		}
		// ���F-E ȥF�ĵ׿�
		int iCountLineNum = 0;
		int iCountCol = 0;
		for (int i = 0; i < iCharNum; ++i)
		{
			if(rgBottomLine[i] < pCharPos[i].Height() && rgBottomLine[i] != 0)
			{
				iCountLineNum++;
				iCountCol += rgBottomLine[i];
			}
		}

		if (iCountLineNum < 6 ||
			(iCountLineNum == 7 && iCountCol == (pCharPos[0].Height() - 4) * iCountLineNum))	// �²�ȫ��ʱ Ҳ��ȥ
		{
			memset(rgBottomLine, 0, sizeof(rgBottomLine));
		}
	}
	m_pRecognition->EraseNail(fIsNail, rgrcNailPos, rgBottomLine);

	return S_OK;
}
HRESULT CTrackerImpl::DownSampleYUV422(HV_COMPONENT_IMAGE imgSrc, HV_COMPONENT_IMAGE imgDst)
{
	if (imgSrc.nImgType != HV_IMAGE_YUV_422 || imgDst.nImgType != HV_IMAGE_YUV_422)
	{
		return E_INVALIDARG;
	}
	int i, j;
	int iBlockNum = imgDst.iWidth >> 1;
	int iStepW = imgSrc.iWidth / iBlockNum;
	int iStepW2 = iStepW >> 1;
	int iStepH = imgSrc.iHeight / imgDst.iHeight;
	PBYTE8 pbSrcY = GetHvImageData(&imgSrc, 0);
	PBYTE8 pbSrcU = GetHvImageData(&imgSrc, 1);
	PBYTE8 pbSrcV = GetHvImageData(&imgSrc, 2);
	PBYTE8 pbDstY = GetHvImageData(&imgDst, 0);
	PBYTE8 pbDstU = GetHvImageData(&imgDst, 1);
	PBYTE8 pbDstV = GetHvImageData(&imgDst, 2);
	PBYTE8 pbSrcYTemp, pbSrcUTemp, pbSrcVTemp, pbDstYTemp, pbDstUTemp, pbDstVTemp;
	for (i = 0; i < imgDst.iHeight; i++)
	{
		pbSrcYTemp = pbSrcY; pbSrcUTemp = pbSrcU; pbSrcVTemp = pbSrcV;
		pbDstYTemp = pbDstY; pbDstUTemp = pbDstU; pbDstVTemp = pbDstV;
		for (j = 0; j < iBlockNum; j++)
		{
			*pbDstYTemp = *pbSrcYTemp;
			*(pbDstYTemp + 1) = *(pbSrcYTemp + 1);
			*pbDstUTemp = *pbSrcUTemp;
			*pbDstVTemp = *pbSrcVTemp;
			pbSrcYTemp += iStepW; pbSrcUTemp += iStepW2; pbSrcVTemp += iStepW2;
			pbDstYTemp += 2; pbDstUTemp++;	pbDstVTemp++;
		}
		pbSrcY += imgSrc.iStrideWidth[0] * iStepH;
		pbSrcU += imgSrc.iStrideWidth[1] * iStepH;
		pbSrcV += imgSrc.iStrideWidth[2] * iStepH;
		pbDstY += imgDst.iStrideWidth[0];
		pbDstU += imgDst.iStrideWidth[1];
		pbDstV += imgDst.iStrideWidth[2];
	}
	return S_OK;	
}

inline int CaclAvgGray(HV_COMPONENT_IMAGE imgCropPlate, int iAdd)
{
	// iAdd Ϊ��ͼ���
	int nWidth = imgCropPlate.iWidth;
	int nHeight = imgCropPlate.iHeight;
	int nCount = nWidth * nHeight;
	if(nCount == 0) return 0;

	nCount = 0;
	int nSum = 0;
	RESTRICT_PBYTE8 pLine = GetHvImageData(&imgCropPlate, 0);
	if (imgCropPlate.nImgType == HV_IMAGE_CbYCrY)
	{
		for(int i = 0; i < nHeight; i++, pLine += imgCropPlate.iStrideWidth[0])
		{
			for(int j = 1; j < nWidth; j += 2)
			{
				nSum += pLine[j];
				nCount++;
			}
		}
	}
	if(imgCropPlate.nImgType == HV_IMAGE_YCbYCr)
	{
		for(int i = 0; i < nHeight; i++, pLine += imgCropPlate.iStrideWidth[0])
		{
			for(int j = 0; j < nWidth; j += 2)
			{
				nSum += pLine[j];
				nCount++;
			}
		}
	}
	if(imgCropPlate.nImgType == HV_IMAGE_YUV_422 ||
		imgCropPlate.nImgType == HV_IMAGE_BT1120 ||
		imgCropPlate.nImgType ==  HV_IMAGE_BT1120_FIELD ||
		imgCropPlate.nImgType == HV_IMAGE_YUV_420 )
	{
		for(int i = 0; i < nHeight; i++, pLine += imgCropPlate.iStrideWidth[0])
		{
			for(int j = 0; j < nWidth; j++)
			{
				nSum += pLine[j];
				nCount++;
			}
		}
	}

	return nSum/nCount;
}

int GetCarColorType( DWORD32 dwCarColorID )
{
	DWORD32 dwCarColorType(0);
	int i;
	for (i = 0; i < CARCOLOR_NUM; i++)
	{
		if ( dwCarColorID == g_rgCarColorInfo[i].dwCarColorID)
		{
			dwCarColorType = g_rgCarColorInfo[i].dwCarColorType;
			break;
		}
	}

	if(i == CARCOLOR_NUM) return 0;
	return dwCarColorType;
}

HRESULT CTrackerImpl::RecognizeCarColour(
	HV_COMPONENT_IMAGE imgPlate,
	PlateInfo &Info,
	int iAvgGray                                  //��������
	)
{

	// ץ�ĳ�����ɫʶ��ֻ�����Ͻ���
	// ץ��ͼ������ɫʶ���Ч�������Ƶ���ĺã�������������ơ�
//	if (m_LightType != NIGHT)
//	{
//		return S_FALSE;
//	}
	//LockTracker();
	bool FisNight = FALSE;
	if(NIGHT == m_LightType)
		FisNight = TRUE;

	HRESULT hr = RecognizeCarColour(imgPlate, Info,iAvgGray, true);
	//UnLockTracker();
	return hr;
}

HRESULT CTrackerImpl::RecognizeColorArea(
	const HV_COMPONENT_IMAGE imgInput, 
	HV_RECT rcCrop, 
	BYTE8 *cTempAdd, 
	BYTE8 *piResult,
	int iCutLeng,
	LIGHT_TYPE m_PlateLightType
	)
{
	HV_COMPONENT_IMAGE imgCutArea;

	CPersistentComponentImage CTempImg;
	CTempImg.Create(HV_IMAGE_BGR,iCutLeng + 2, iCutLeng + 2);

	if(GetHvImageData(&CTempImg, 0) == NULL)
	{
		*piResult = 0;
		return E_POINTER;
	}

	if(rcCrop.right - rcCrop.left <= 0 ||  rcCrop.bottom - rcCrop.top <= 0
		|| rcCrop.left < 0 || rcCrop.right >= imgInput.iWidth || rcCrop.top < 0 || rcCrop.bottom >= imgInput.iHeight
	    //	|| rcCrop.right - rcCrop.left > iCutLeng || rcCrop.bottom - rcCrop.top > iCutLeng
		|| rcCrop.left >= imgInput.iWidth || rcCrop.top >= imgInput.iHeight 
		|| rcCrop.right < 0 || rcCrop.bottom < 0)
	{
		*piResult = 0;
		return S_OK;
	}

	RTN_HR_IF_FAILED(CropImage(imgInput, rcCrop, &imgCutArea));

	if(SUCCEEDED(ConvertYCbCr2BGR(CTempImg.iStrideWidth[0], GetHvImageData(&CTempImg, 0), &imgCutArea)))
	{
		HV_COMPONENT_IMAGE pInput;
		pInput.iHeight = 1;
		pInput.iWidth  = 7;
		SDWORD32 idwProb = 0;
		BYTE8 cResult = 0;
		int iTempAdd[4] = {0};
		int iTempCut = imgCutArea.iHeight * imgCutArea.iWidth;
		BYTE8 *pcRBG = GetHvImageData(&CTempImg, 0);
		for(int i = 0; i < imgCutArea.iHeight; i++, pcRBG += CTempImg.iStrideWidth[0])
		{
			BYTE8 *pbRGBTemp = pcRBG;						// ��ǰ�е��׵�ַ
			for(int j = 0; j < imgCutArea.iWidth; j++, pbRGBTemp += 3)
			{
				iTempAdd[1] += pbRGBTemp[0];
				iTempAdd[2] += pbRGBTemp[1];
				iTempAdd[3] += pbRGBTemp[2];
			}
		}

		cTempAdd[3] = iTempAdd[1] / iTempCut;		// B
		cTempAdd[2] = iTempAdd[2] / iTempCut;		// G
		cTempAdd[1] = iTempAdd[3] / iTempCut;		// R
		BGR2HSL(cTempAdd[3],cTempAdd[2], cTempAdd[1], &cTempAdd[4], &cTempAdd[5], &cTempAdd[6]);
		SetHvImageData(&pInput, 0, cTempAdd);
		//m_PlateLightType<=25�ð���ģ��ʶ�𣬷���������ģ��ʶ��
		m_pRecognition->EvaluateColor(&pInput, &cResult, &idwProb, m_PlateLightType);
		CAR_COLOR nCarColor = (CAR_COLOR)GetCarColorType(cResult);

		//��ɫ����
		if(nCarColor == CC_BLUE)
		{
			if(cTempAdd[4] >=115 && cTempAdd[4] <= 130 && cTempAdd[6] >= 190)
			{
				nCarColor = CC_WHITE;
			}
			else if((cTempAdd[4] >=120 && cTempAdd[4] <= 135 && (cTempAdd[5]+cTempAdd[6]) < 120)
				||(cTempAdd[4] >=120 && cTempAdd[4] <= 125 && cTempAdd[6] <= 60 && cTempAdd[5]<=130)
				||(cTempAdd[4]<120 && cTempAdd[6]<150))
			{
				nCarColor = CC_GREEN;
			}
			else if(cTempAdd[4] >=140 && cTempAdd[4] <= 150 && cTempAdd[5] <= 65 && cTempAdd[6] <= 45)
			{
				nCarColor = CC_BLACK;
			}
		}
		if(nCarColor == CC_GREY )
		{
			if((cTempAdd[4] >=125 && cTempAdd[4] <= 140 && cTempAdd[6] >= 180) ||
				(cTempAdd[4] >=50 && cTempAdd[4] <= 140 && cTempAdd[5]<=10 && cTempAdd[6]>= 170))
			{
				nCarColor = CC_WHITE;
			}
			if(cTempAdd[4] >=140 && cTempAdd[4] <= 160 && cTempAdd[5]<=35 && cTempAdd[6]<=50)
			{
				nCarColor = CC_BLACK;
			}
		}
		if(nCarColor == CC_YELLOW && cTempAdd[5] < 30)
		{
			nCarColor = CC_GREY;
		}
		if(nCarColor == CC_RED)
		{
			if(cTempAdd[6] < 10 ||(cTempAdd[6] < 20 && cTempAdd[5]<120))
			{
				nCarColor = CC_BLACK;
			}
			else if((cTempAdd[4] > 20&& cTempAdd[4] < 30 && (cTempAdd[5] +cTempAdd[6]) > 200)|| (cTempAdd[4] >12 && cTempAdd[4]<=20 && cTempAdd[6]<=40))
			{
				nCarColor = CC_BROWN;
			}		
		}

		*piResult = nCarColor;
	}
	else
	{
		*piResult = 0;		// δ֪�����0
	}

	return S_OK;
}

HRESULT CTrackerImpl::RecognizeCarColour(
	HV_COMPONENT_IMAGE imgPlateOri,                   //֡ͼ��ԭʼ��ͼ��
	PlateInfo &Info,
	int iAvgGray,                                    //��������
	BOOL fIsNight
	)
{
	Info.iCarColor = 0;
	if (Info.nPlateType == PLATE_DOUBLE_MOTO || Info.nCarType == CT_BIKE)
	{
		Info.nCarColour = CC_UNKNOWN;
		return S_OK;
	}

	if(imgPlateOri.iHeight < 200 || imgPlateOri.iWidth < 200)			
	{
		Info.nCarColour = CC_UNKNOWN;
		return S_OK;
	}

	//ʶ����Ϊ���Ƶĳ�����ɫֱ�Ӹ�ֵΪ��ɫ
	if(Info.rgbContent[6] == 37)
	{
		Info.nCarColour = CC_WHITE;
		Info.iCarColor = 1;
		return S_OK;
	}


	HV_COMPONENT_IMAGE imgCutOri;
	LIGHT_TYPE m_PlateLightType = DAY;
	BYTE8 cTempAdd[7];	
	CRect rcCharPos; 

	bool fIsSmallCar = (Info.color != PC_YELLOW && Info.rgbContent[6] != 114 && Info.rgbContent[6] != 115);
	int iWidth = Info.rgrcChar[6].right - Info.rgrcChar[0].left;
	int iHeight = Info.rcPos.bottom - Info.rcPos.top;   //���Ƹ߶�
	float fAdjTop = 3;             //�߶ȵ���ϵ��
	CRect rcPositionPos;
	if(fIsSmallCar)  
	{
		rcPositionPos.top = (long)MAX2(Info.rcPos.top - iHeight*3,0);
		rcPositionPos.bottom = (long)MAX2(Info.rcPos.top - iHeight,0);
		rcPositionPos.left = (long)MAX2(Info.rcPos.left - iWidth/2,0);
		rcPositionPos.right = (long)MIN2(Info.rcPos.right + iWidth/2,imgPlateOri.iWidth-1);
	}
	else 
	{
		rcPositionPos.top = (long)MAX2(Info.rcPos.top - iHeight*4,0);
		rcPositionPos.bottom = Info.rcPos.top;
		rcPositionPos.left = (long)MAX2(Info.rcPos.left - iWidth*1.8,0);
		rcPositionPos.right = Info.rcPos.right;
	}
	CropImage(imgPlateOri,rcPositionPos,&imgCutOri);
	cTempAdd[0] = BYTE8(CaclAvgGray(imgCutOri, 2));	

	int DiffY = cTempAdd[0] - iAvgGray;  
	//�۰��ơ������������Ƚ�С����fIsSmallCarΪ��
	if((Info.rgbContent[6] == 114 || Info.rgbContent[6] == 115) && cTempAdd[0] < 50)
	{
		fIsSmallCar = true;
	}

	//������
	if (Info.nPlateType != PLATE_DOUBLE_YELLOW)  
	{	
		rcCharPos.top = Info.rcPos.top + Info.rgrcChar[0].top;
		rcCharPos.left = Info.rcPos.left + Info.rgrcChar[0].left;
		rcCharPos.bottom = Info.rcPos.top + Info.rgrcChar[0].bottom;
		rcCharPos.right = Info.rcPos.left + Info.rgrcChar[6].right;
	}
	//˫����
	else
	{
		rcCharPos.bottom = Info.rcPos.top + Info.rgrcChar[3].bottom;
		rcCharPos.right = Info.rcPos.left + Info.rgrcChar[6].right;
		rcCharPos.top = Info.rcPos.top + Info.rgrcChar[3].top;
		rcCharPos.left = Info.rcPos.left + Info.rgrcChar[2].left;	
	}
	int iPlateH = rcCharPos.bottom - rcCharPos.top + 1;               //���Ƹ߶�
	int iPlateW = rcCharPos.right - rcCharPos.left + 1;		           //���ƿ��
	int iPlateMid = int((rcCharPos.right + rcCharPos.left) * 0.5);     //���ƿ���е�����

	int iCutLeng = iPlateH;
	if(iCutLeng > 14)  iCutLeng = 14;
	if(iCutLeng < 8)	iCutLeng = 8;
	if(iPlateH < 15) iPlateH =15;
	if(fIsSmallCar && (Info.rgbContent[6] == 114 || Info.rgbContent[6] == 115))
	{
		fAdjTop = 5;
	}

	CRect rcCutArea[YELLOW_NUM];            //������ɫ��ȡ����
	BYTE8 rgResult[2][YELLOW_NUM] = {0};   //������ɫʶ����
	BYTE8 rgCutAreaL[2][YELLOW_NUM] = {0}; 
	int iPlateColor,iPlateColor2;
	int rgPlateColor[2];            //ͶƱ��ĳ�����ɫʶ����
	int rgPlateColorProb[2] = {0};	// ���Ŷ�

	IReferenceComponentImage* pTmpImg = NULL;		

	CreateReferenceComponentImage(
		&pTmpImg,
		HV_IMAGE_BGR,
		iCutLeng + 1,
		iCutLeng + 1
		);


	// С����������ɫ���ǻ�ɫ��
	if(fIsSmallCar)
	{
		rcCutArea[0].top = (long)MAX2((Info.rcPos.top - fAdjTop*iPlateH),0);
		rcCutArea[0].left  = iPlateMid - iCutLeng/2;

		rcCutArea[1].top = rcCutArea[0].top;
		rcCutArea[1].left  = (long)MAX2((iPlateMid - iPlateW),0);
		rcCutArea[2].top = rcCutArea[0].top;
		rcCutArea[2].left  = (long)MIN2((iPlateMid + iPlateW), imgPlateOri.iWidth - iCutLeng - 1);

		rcCutArea[11].top = (long)MAX2((rcCutArea[0].top - iPlateH),0);
		rcCutArea[11].left  = rcCutArea[0].left;
		rcCutArea[3].top = (long)MAX2((rcCutArea[0].top -iPlateH),0);
		rcCutArea[3].left  = (long)MAX2((iPlateMid - iPlateW/2),0);
		rcCutArea[4].top = rcCutArea[3].top;
		rcCutArea[4].left  = (long)MAX2((rcCutArea[3].left  - iPlateW),0);

		rcCutArea[5].top = rcCutArea[3].top;
		rcCutArea[5].left  = (long)MIN2((iPlateMid + iPlateW/2), imgPlateOri.iWidth - iCutLeng - 1);
		rcCutArea[6].top = rcCutArea[3].top;
		rcCutArea[6].left  = (long)MIN2((rcCutArea[5].left  + iPlateW), imgPlateOri.iWidth - iCutLeng - 1);
		//
		rcCutArea[7].top = (long)MAX2((rcCutArea[0].top - 2*iPlateH),0);
		rcCutArea[7].left  = (long)MAX2((iPlateMid - iPlateW),0);
		rcCutArea[8].top = rcCutArea[7].top;
		rcCutArea[8].left  = (long)MAX2((iPlateMid  - 0.3*iPlateW),0);
		rcCutArea[9].top = rcCutArea[7].top;
		rcCutArea[9].left  = (long)MIN2((iPlateMid + 0.3*iPlateW), imgPlateOri.iWidth - iCutLeng - 1);
		rcCutArea[10].top = rcCutArea[7].top;
		rcCutArea[10].left  = (long)MIN2((iPlateMid  + iPlateW), imgPlateOri.iWidth - iCutLeng - 1);
		for(int i=0;i<NORMAL_NUM;i++)
		{
			rcCutArea[i].bottom = rcCutArea[i].top + iCutLeng;
			rcCutArea[i].right = rcCutArea[i].left + iCutLeng;
		}
		//������ɫʶ��
		for(int i = 0; i < NORMAL_NUM; i++)
		{
			BYTE8 *piResult = &rgResult[0][i];
			RecognizeColorArea(imgPlateOri, rcCutArea[i], cTempAdd, piResult,iCutLeng, m_PlateLightType);
			rgCutAreaL[0][i] = cTempAdd[6];
		}
		//ͳ����ɫ����
		int rgCountColor[11] = {0};
		for(int j = 0; j < NORMAL_NUM; j++)
		{
			rgCountColor[rgResult[0][j]]++;
		}
		//����ɫ����
		int nColorScore[11] = {0};
		int nColorScore2[11] = {0};
		for(int j=0;j<11;j++)
		{
			if(j<4)
			{
				nColorScore[j] = rgCountColor[j] * 10;
			}
			else
			{
				if(rgCountColor[j] <= 3)
				{
					nColorScore[j] =  rgCountColor[j] * 18;
				}
				else
				{
					nColorScore[j] =  rgCountColor[j] * 24;
				}
			}		
		}

		//�������������ɫ��Ϊ������ɫ�Ľ��
		int nColorResult=0,nSecondResult = 0;;
		for(int i=0;i<11;i++)
		{
			if(nColorScore[nColorResult] < nColorScore[i] || (nColorScore[nColorResult] == nColorScore[i] && i>3 && nColorResult < 4)) 
			{
				nSecondResult = nColorResult;
				nColorResult = i;
			}
			else if(nColorScore[nSecondResult] <= nColorScore[i] && nColorScore[i] > 0) 
			{
				nSecondResult = i;
			}
		}
		//��ɫ��һ�㲻�����в�ɫ�������
		if(nColorResult == 3 && nSecondResult > 3 && nColorScore[nSecondResult] > 70)
		{
			nColorResult = nSecondResult;
		}

		iPlateColor = nColorResult;
		//��ǳɫ����
		int iAvgL = 0;
		int nCountL = 0;

		for(int i=0;i<NORMAL_NUM;i++)
		{
			if(rgResult[0][i] == iPlateColor)
			{
				iAvgL += rgCutAreaL[0][i];
				nCountL++;
			}
		}
		iAvgL = iAvgL/nCountL;

		if( iAvgL >=130)
			Info.iCarColor = 1;
		else
			Info.iCarColor = 2;

		rgPlateColor[0] = nColorResult;  //��һ��ʶ����
		rgPlateColorProb[0] = nColorScore[nColorResult];  //��һ��ʶ�������Ŷ�
		//ʶ��ɺڻҵ���ʶ��һ�Σ���Ҫ��Ϊ����߲�ɫ��ʶ��
		if(nColorResult == 2 || nColorResult == 3)  
		{
			//�����ƶ�һ���߶���ʶ��һ��
			for(int i = 0; i < NORMAL_NUM; i++)
			{
				rcCutArea[i].top = MAX2(rcCutArea[i].top - iPlateH, 0);
				rcCutArea[i].bottom = rcCutArea[i].top + iCutLeng;

				BYTE8 *piResult = &rgResult[1][i];
				RecognizeColorArea(imgPlateOri, rcCutArea[i], cTempAdd, piResult,iCutLeng, m_PlateLightType);
				rgCutAreaL[1][i] = cTempAdd[6];
			}
			for(int j=0;j<11;j++)
			{
				rgCountColor[j] = 0;
			}
			//ͳ����ɫ����
			for(int j = 0; j < NORMAL_NUM; j++)
			{
				rgCountColor[rgResult[1][j]]++;
			}
			//����ɫ����
			for(int j=0;j<11;j++)
			{
				if(j<4)
				{
					nColorScore2[j] = rgCountColor[j] * 10;
				}
				else
				{
					if(rgCountColor[j] < 3)
					{
						nColorScore2[j] =  rgCountColor[j] * 18;
					}
					else
					{
						nColorScore2[j] =  rgCountColor[j] * 24;
					}
				}		
			}

			//�������������ɫ��Ϊ������ɫ�Ľ��
			nColorResult=0;
			nSecondResult = 0;;
			for(int i=0;i<11;i++)
			{
				if(nColorScore2[nColorResult] < nColorScore2[i] || (nColorScore2[nColorResult] == nColorScore2[i] && i>3 && nColorResult < 4)) 
				{
					nSecondResult = nColorResult;
					nColorResult = i;
				}
				else if(nColorScore2[nSecondResult] <= nColorScore2[i] && nColorScore2[i] > 0) 
				{
					nSecondResult = i;
				}
			}
			//��ɫ��һ�㲻�����в�ɫ�������
			if(nColorResult == 3 && nSecondResult > 3 && nColorScore2[nSecondResult] > 70)
			{
				nColorResult = nSecondResult;
			}

			rgPlateColor[1] = nColorResult;  //��2��ʶ����
			rgPlateColorProb[1] = nColorScore2[nColorResult];  //��2��ʶ�������Ŷ�
			//���ν��ȡ����
			if((rgPlateColorProb[0] < rgPlateColorProb[1] && rgPlateColor[1] != 3) || (rgPlateColor[1] > 3 && rgPlateColorProb[1] >= 96))
			{
				iPlateColor = rgPlateColor[1];
			}
		}

		//����ʶ��ȡ��������λ��ʶ��
		if(iPlateColor == CC_BLACK && (m_nEnableColorReRecog & 1) && DiffY < m_nBColorReThr) 
		{
			//���㳵���ұ�һС������ĳ�������
			rcPositionPos.top = Info.rcPos.top;
			rcPositionPos.bottom = Info.rcPos.bottom;
			rcPositionPos.left =Info.rcPos.left + Info.rgrcChar[6].right + 15;
			rcPositionPos.right = (long)MIN2(rcPositionPos.left + iHeight*2,imgPlateOri.iWidth-1);
			CropImage(imgPlateOri,rcPositionPos,&imgCutOri);
			int TempY = BYTE8(CaclAvgGray(imgCutOri, 2));		

			rcCutArea[0].top = Info.rcPos.top;
			rcCutArea[0].left  = Info.rcPos.left;
			rcCutArea[1].top = rcCutArea[0].top;
			rcCutArea[1].left  = Info.rcPos.left-iPlateW/2;
			rcCutArea[2].top = rcCutArea[0].top;
			rcCutArea[2].left  = Info.rcPos.right;
			rcCutArea[3].top = rcCutArea[0].top;
			rcCutArea[3].left  = Info.rcPos.right + iPlateW/2;
			for(int i=0;i<4;i++)
			{
				rcCutArea[i].bottom = rcCutArea[i].top + iCutLeng;
				rcCutArea[i].right = rcCutArea[i].left + iCutLeng;
			}

			for(int i = 0; i < 4; i++)
			{
				BYTE8 *piResult = &rgResult[0][i];
				RecognizeColorArea(imgPlateOri, rcCutArea[i], cTempAdd, piResult,iCutLeng, m_PlateLightType);	
			}

			int rgCountColor[11] = {0};
			for(int j = 0; j < 4; j++)
			{
				rgCountColor[rgResult[0][j]]++;
			}
			for(int j=4;j<=7;j++)
			{
				if(rgCountColor[j] == 4 && j!= 5) //��������4������ʶ��ɺ�����
				{
					iPlateColor = j;
				}
			}
			if(iPlateColor <=3 && DiffY >= -10 && (rgCountColor[3] <= 2 || TempY >= 30)) 
			{
				iPlateColor = CC_GREY;
			}

		}
		//���ݻ������Ⱥͳ�����ɫ���úںͻҽ��
		if(iPlateColor == CC_BLACK && m_fEnableCorlorThr && DiffY > m_nBGColorThr)
		{
			iPlateColor = CC_GREY;
		}
		//����ʶ��
		if(iPlateColor == CC_WHITE && (m_nEnableColorReRecog & 2) && cTempAdd[0]> m_nWColorReThr)
		{
			//���㳵���ұ�һС������ĳ�������
			rcPositionPos.top = Info.rcPos.top;
			rcPositionPos.bottom = Info.rcPos.bottom;
			rcPositionPos.left =Info.rcPos.left + Info.rgrcChar[6].right + 15;
			rcPositionPos.right = (long)MIN2(rcPositionPos.left + iHeight*2,imgPlateOri.iWidth-1);
			CropImage(imgPlateOri,rcPositionPos,&imgCutOri);
			int TempY = BYTE8(CaclAvgGray(imgCutOri, 2));		

			rcCutArea[0].top = Info.rcPos.top - 3;
			rcCutArea[0].left  = rcCharPos.left - iPlateW/2;
			rcCutArea[1].top = rcCutArea[0].top - 2;
			rcCutArea[1].left  = rcCharPos.left-iPlateW;
			rcCutArea[2].top = rcCutArea[0].top;
			rcCutArea[2].left  = rcCharPos.right + iPlateW/2;
			rcCutArea[3].top = rcCutArea[1].top;
			rcCutArea[3].left  = rcCharPos.right + iPlateW;
			for(int i=0;i<4;i++)
			{
				rcCutArea[i].bottom = rcCutArea[i].top + 12;
				rcCutArea[i].right = rcCutArea[i].left + 12;
			}
			
			for(int i = 0; i < 4; i++)
			{
				BYTE8 *piResult = &rgResult[0][i];
				RecognizeColorArea(imgPlateOri, rcCutArea[i], cTempAdd, piResult,iCutLeng, m_PlateLightType);	
				
			}
			int rgCountColor[11] = {0};
			for(int j = 0; j < 4; j++)
			{
				rgCountColor[rgResult[0][j]]++;
			}

			for(int j=4;j<=7;j++)
			{
				if(rgCountColor[j] == 4 /*&& j!= 5*/) //��������4������ʶ��ɺ�����
				{
					iPlateColor = j;
				}
			}
			if(iPlateColor <=3 && TempY <= 160 && iAvgL-cTempAdd[0] <=60 
				&& rgCountColor[1] <= 2 && rgCountColor[3] < 4) 
			{
				iPlateColor = CC_GREY;
			}

		}

		if(m_LightType == NIGHT && iPlateColor == CC_GREY && m_fEnableCorlorThr && DiffY > m_nWGColorThr)
		{
			iPlateColor = CC_WHITE;
		}
		//ʶ��ɰ׻�ҡ��������ɺڣ��շ�վ�����£���ȡ������������ʶ��
		if((iPlateColor == CC_GREY || iPlateColor == CC_WHITE)&& (m_nEnableColorReRecog & 4) && (DiffY < m_nWGColorReThr || iAvgGray < (m_nWGColorReThr+40)))
		{
			//���㳵���ұ�һС������ĳ�������
			rcPositionPos.top = Info.rcPos.top;
			rcPositionPos.bottom = Info.rcPos.bottom;
			rcPositionPos.left =Info.rcPos.left + Info.rgrcChar[6].right + 15;
			rcPositionPos.right = (long)MIN2(rcPositionPos.left + iHeight*2,imgPlateOri.iWidth-1);
			CropImage(imgPlateOri,rcPositionPos,&imgCutOri);
			int TempY = BYTE8(CaclAvgGray(imgCutOri, 2));

			rcCutArea[0].top = Info.rcPos.top - 3;
			rcCutArea[0].left  = rcCharPos.left - iPlateW/2;
			rcCutArea[1].top = rcCutArea[0].top - 2;
			rcCutArea[1].left  = rcCharPos.left-iPlateW;
			rcCutArea[2].top = rcCutArea[0].top;
			rcCutArea[2].left  = rcCharPos.right + iPlateW/2;
			rcCutArea[3].top = rcCutArea[1].top;
			rcCutArea[3].left  = rcCharPos.right + iPlateW;

			for(int i=0;i<4;i++)
			{
				rcCutArea[i].bottom = rcCutArea[i].top + iCutLeng;
				rcCutArea[i].right = rcCutArea[i].left + iCutLeng;
			}
			for(int i = 0; i < 4; i++)
			{
				BYTE8 *piResult = &rgResult[0][i];
				RecognizeColorArea(imgPlateOri, rcCutArea[i], cTempAdd, piResult,iCutLeng, m_PlateLightType);	
				//�շ�վ�º�ʶ��ɻ�
				if(*piResult == CC_GREY && cTempAdd[6] < 35 && TempY < 30 && (cTempAdd[0]-cTempAdd[6]) > 120)
				{
					*piResult = CC_BLACK;
				}
			}

			int rgCountColor[11] = {0};
			for(int j = 0; j < 4; j++)
			{
				rgCountColor[rgResult[0][j]]++;
			}

			for(int j=4;j<=7;j++)
			{
				if(rgCountColor[j] >= 3 && j!= 5) //��������4������ʶ��ɺ�����
				{
					iPlateColor = j;
				}
			}
			if(iPlateColor <=3 && (rgCountColor[3] >= 3 || TempY < 20 || (TempY < 35 && rgCountColor[3] >= 2)))   //����������3������ʶ��ɺ�
			{
				iPlateColor = CC_BLACK;
			}
			else if(iPlateColor == CC_WHITE && rgCountColor[1] <= 2)
			{
				iPlateColor = CC_GREY;
			}
		}
	}

	else
	{
		rcCutArea[0].top = (long)MAX2((Info.rcPos.top - fAdjTop*iPlateH),0);
		rcCutArea[0].left  = (long)MAX2((iPlateMid - iPlateW),0);

		rcCutArea[1].top = rcCutArea[0].top;
		rcCutArea[1].left  = (long)MIN2((iPlateMid + iPlateW), imgPlateOri.iWidth - iCutLeng - 1);
		//
		rcCutArea[2].top = (long)MAX2((rcCutArea[0].top - iPlateH),0);
		rcCutArea[2].left  = (long)MAX2((iPlateMid - 1.5*iPlateW),0);

		rcCutArea[3].top = rcCutArea[2].top;
		rcCutArea[3].left  = (long)MIN2((iPlateMid + 1.5*iPlateW), imgPlateOri.iWidth - iCutLeng - 1);

		rcCutArea[4].top = rcCutArea[2].top;
		rcCutArea[4].left  = (long)MAX2((rcCutArea[2].left  - iPlateW/2),0);

		rcCutArea[5].top = rcCutArea[2].top;
		rcCutArea[5].left  =  (long)MIN2((rcCutArea[3].left  + iPlateW/2), imgPlateOri.iWidth - iCutLeng - 1);
		//
		rcCutArea[6].top = (long)MAX2((rcCutArea[0].top - 2*iPlateH),0);
		rcCutArea[6].left  = (long)MAX2((iPlateMid - 0.5*iPlateW),0);
		rcCutArea[7].top = rcCutArea[6].top;
		rcCutArea[7].left  = (long)MIN2((iPlateMid + 0.5*iPlateW), imgPlateOri.iWidth - iCutLeng - 1);

		rcCutArea[8].top = rcCutArea[6].top;
		rcCutArea[8].left  = (long)MAX2((rcCutArea[6].left  - iPlateW),0);
		rcCutArea[9].top = rcCutArea[6].top;
		rcCutArea[9].left  = (long)MIN2((rcCutArea[7].left + iPlateW), imgPlateOri.iWidth - iCutLeng - 1);

		rcCutArea[10].top = rcCutArea[6].top;
		rcCutArea[10].left  = (long)MAX2((rcCutArea[8].left  - iPlateW),0);
		rcCutArea[11].top = rcCutArea[6].top;
		rcCutArea[11].left  = (long)MIN2((rcCutArea[9].left  + iPlateW), imgPlateOri.iWidth - iCutLeng - 1);
		//
		rcCutArea[12].top = (long)MAX2((rcCutArea[0].top - 3*iPlateH),0);
		rcCutArea[12].left  = (long)MAX2((iPlateMid - iPlateW),0);
		rcCutArea[13].top = rcCutArea[12].top;
		rcCutArea[13].left  = (long)MIN2((iPlateMid + iPlateW), imgPlateOri.iWidth - iCutLeng - 1);

		rcCutArea[14].top = rcCutArea[12].top;
		rcCutArea[14].left  = (long)MAX2((rcCutArea[12].left  - iPlateW),0);
		rcCutArea[15].top = rcCutArea[12].top;
		rcCutArea[15].left  = (long)MIN2((rcCutArea[13].left  + iPlateW), imgPlateOri.iWidth - iCutLeng - 1);
		//������������
		rcCutArea[16].top = Info.rcPos.top;
		rcCutArea[16].left  = (long)MAX2(rcCharPos.left - iPlateW/2,0);
		rcCutArea[17].top = rcCutArea[16].top;
		rcCutArea[17].left  = (long)MAX2(rcCharPos.left-iPlateW,0);
		rcCutArea[18].top = rcCutArea[16].top;
		rcCutArea[18].left  = (long)MIN2(rcCharPos.right + iPlateW/2,imgPlateOri.iWidth - iCutLeng -1);
		rcCutArea[19].top = rcCutArea[16].top;
		rcCutArea[19].left  = (long)MIN2(rcCharPos.right + iPlateW,imgPlateOri.iWidth - iCutLeng -1);

		for(int i=0;i<YELLOW_NUM;i++)
		{
			rcCutArea[i].bottom = rcCutArea[i].top + iCutLeng;
			rcCutArea[i].right = rcCutArea[i].left + iCutLeng;
		}

		for(int i = 0; i < YELLOW_NUM; i++)
		{
			BYTE8 *piResult = &rgResult[0][i];
			RecognizeColorArea(imgPlateOri, rcCutArea[i], cTempAdd, piResult,iCutLeng, m_PlateLightType);
			rgCutAreaL[0][i] = cTempAdd[6];
		}
		//ͳ����ɫ����
		int rgCountColor[11] = {0};
		for(int j = 0; j < YELLOW_NUM; j++)
		{
			rgCountColor[rgResult[0][j]]++;
		}
		//����ɫ����
		int nColorScore[11] = {0};
		int nColorScore2[11] = {0};
		for(int j=0;j<11;j++)
		{	
			if(j==1 || j>3)
			{
				nColorScore[j] = rgCountColor[j] * 30;
			}
			else if(j==2)
			{
				nColorScore[j] = rgCountColor[j] * 12;
			}
			else if(j==3)
			{
				nColorScore[j] = rgCountColor[j] * 8;
			}
		}

		//�������������ɫ��Ϊ������ɫ�Ľ��
		int nColorResult=0,nSecondResult = 0;
		for(int i=0;i<11;i++)
		{
			if(nColorScore[nColorResult] < nColorScore[i] || (nColorScore[nColorResult] == nColorScore[i] && i>3 && nColorResult < 4)) 
			{
				nSecondResult = nColorResult;
				nColorResult = i;
			}
			else if(nColorScore[nSecondResult] <= nColorScore[i] && nColorScore[i] > 0) 
			{
				nSecondResult = i;
			}
		}
		//��ɫ��ʶ�����ɫ������
		if(nColorResult == 7 && nSecondResult == 4 && nColorScore[nSecondResult] > 120)
		{
			nColorResult = nSecondResult;
		}
		//��ɫ��һ�㲻�����в�ɫ
		if(nColorResult == 3 && nSecondResult > 3 && nColorScore[nSecondResult] >= 90)
		{
			nColorResult = nSecondResult;
		}

		iPlateColor = nColorResult;
		rgPlateColor[0] = nColorResult;  //��һ��ʶ����
		rgPlateColorProb[0] = nColorScore[nColorResult];  //��һ��ʶ�������Ŷ�

		//������ɫ�󳵳���ϸߣ���ȡ�����Ʋ��֣���������ʶ��ɺ졪�����óɺڣ�����һ���߶���ʶ��
		if(nColorResult == 4 && nSecondResult == 7 && nColorScore[nColorResult]- nColorScore[nColorResult] <= 60)
		{
			nColorResult = 3;
		}

		//��ǳɫ����
		int iAvgL = 0;
		int nCountL = 0;

		for(int i=0;i<YELLOW_NUM;i++)
		{
			if(rgResult[0][i] == iPlateColor)
			{
				iAvgL += rgCutAreaL[0][i];
				nCountL++;
			}
		}
		iAvgL = iAvgL/nCountL;

		if(iAvgL >=130)
			Info.iCarColor = 1;
		else
			Info.iCarColor = 2;


		//ʶ��ɺڻҵ���ʶ��һ��
		if(nColorResult == 1 || nColorResult ==2 || nColorResult == 3 || nColorResult == 10)
		{
			//�����ƶ�2���߶���ʶ��һ��
			for(int i = 0; i < YELLOW_NUM; i++)
			{
				rcCutArea[i].top = MAX(rcCutArea[i].top - 2 * iCutLeng, 0);
				rcCutArea[i].bottom = rcCutArea[i].top + iCutLeng;

				BYTE8 *piResult = &rgResult[1][i];
				RecognizeColorArea(imgPlateOri, rcCutArea[i], cTempAdd, piResult,iCutLeng, m_PlateLightType);
				rgCutAreaL[1][i] = cTempAdd[6];
			}

			//ͳ����ɫ����
			for(int j=0;j<11;j++)
			{
				rgCountColor[j] = 0;
			}
			for(int j = 0; j < YELLOW_NUM; j++)
			{
				rgCountColor[rgResult[1][j]]++;
			}

			//����ɫ����
			for(int j=0;j<11;j++)
			{	
				if(j==1 || j>3)
				{
					nColorScore2[j] = rgCountColor[j] * 30;
				}
				else if(j==2)
				{
					nColorScore2[j] = rgCountColor[j] * 10;
				}
				else if(j==3)
				{
					nColorScore2[j] = rgCountColor[j] * 5;
				}
			}

			//�������������ɫ��Ϊ������ɫ�Ľ��
			nColorResult=0;
			nSecondResult = 0;;
			for(int i=0;i<11;i++)
			{
				if(nColorScore2[nColorResult] < nColorScore2[i] || (nColorScore2[nColorResult] == nColorScore2[i] && i>3 && nColorResult < 4)) 
				{
					nSecondResult = nColorResult;
					nColorResult = i;
				}
				else if(nColorScore2[nSecondResult] <= nColorScore2[i] && nColorScore2[i] > 0) 
				{
					nSecondResult = i;
				}
			}
			//��ɫ��һ�㲻�����в�ɫ�������
			if(nColorResult == 3 && (nSecondResult > 3 && nColorScore2[nSecondResult] > 60))
			{
				nColorResult = nSecondResult;
			}

			rgPlateColor[1] = nColorResult;  //��2��ʶ����
			rgPlateColorProb[1] = nColorScore2[nColorResult];  //��2��ʶ�������Ŷ�
			
			if(rgPlateColor[0] == 1 || rgPlateColor[0] == 10)  //��һ��ʶ��ɰ�ɫ����ɫ�������ƶ�һ��λ��ʶ��ɺ�������Ȳ��ҷ�������120
			{
				if(rgPlateColorProb[1] >= 120 && rgPlateColor[1] >=4 && rgPlateColor[1] <= 7)
				{
					iPlateColor = rgPlateColor[1];
				}
				if((rgPlateColor[0] == 1 && rgPlateColorProb[0] < 100 && rgCountColor[1]<=2) ||
					(rgPlateColor[0] == 10 && rgPlateColorProb[0] < 100 && rgCountColor[10]<=2))
				{
					iPlateColor = 2;
				}
			}
			else if((rgPlateColorProb[0] < rgPlateColorProb[1] && rgPlateColor[1] != 3) || 
				((rgPlateColor[1] > 3 || rgPlateColor[1] == 1) &&rgPlateColorProb[1] > 90))
			{
				iPlateColor = rgPlateColor[1];
			}

		}
		//�������ȴ��ڻ������ȡ��������óɻ�
		if(iPlateColor == CC_BLACK && m_fEnableCorlorThr && DiffY > m_nBGColorThr)
		{
			iPlateColor = CC_GREY;
		}
		if(m_LightType == NIGHT && iPlateColor == CC_GREY && m_fEnableCorlorThr&& DiffY > m_nWGColorThr)
		{
			iPlateColor = CC_WHITE;
		}
		//�۰��ƴ�һ��Ϊ��ɫ
		if((Info.rgbContent[6] == 114 || Info.rgbContent[6] == 115) && ((iPlateColor > 3 && rgPlateColorProb[0] < 150 &&  rgPlateColorProb[1] < 150)
			||(iPlateColor == 2 && (nColorScore[1] >=90 || nColorScore2[1] >= 90))))
		{
			iPlateColor = 1;
		}
	}
	Info.nCarColour = (CAR_COLOR)iPlateColor;
	if (iPlateColor == CC_BLACK)  Info.iCarColor = 2;
	if(iPlateColor == CC_WHITE)  Info.iCarColor = 1;

	pTmpImg->Release();
	pTmpImg = NULL;

	return S_OK;
}

HRESULT MapParamRect(CRect& rcResult, CRect rcWhole, CRect rcParam, float fltScale=.01f)
{
	rcResult.left=rcWhole.left+(int)(rcWhole.Width()*rcParam.left*fltScale + 0.5);
	rcResult.top=rcWhole.top+(int)(rcWhole.Height()*rcParam.top*fltScale + 0.5);
	rcResult.right=rcWhole.left+(int)(rcWhole.Width()*rcParam.right*fltScale + 0.5);
	rcResult.bottom=rcWhole.top+(int)(rcWhole.Height()*rcParam.bottom*fltScale + 0.5);

	return S_OK;
}

//ͨ��IOȡ��ǰ���̵�״̬
DWORD32 CTrackerImpl::GetIOStatus(int iTeam, BYTE8 bLevel)
{
	DWORD32 dwRet = 0;
	BYTE8 bTmp = bLevel;
	int iLightCount = m_rgLightInfo[iTeam].GetPosCount();

	for( int i = 0; i < 8; ++i )
	{
		if( (bTmp & 1) && (m_rgIOLight[i].iTeam == iTeam) )
		{
			dwRet |= (0x01 << (4 * (iLightCount - m_rgIOLight[i].iPos -1)));
		}
		bTmp >>= 1;
	}

	return dwRet;
}
#if 1
HRESULT CTrackerImpl::CheckTrafficLight(HV_COMPONENT_IMAGE* pSceneImage)
{
	//���ú��̵ƽӿ�,���ص�ǰ֡�Ƶ�״̬���ȶ�ʶ���״̬
	m_cTrafficLight.RecognizeTrafficLight(pSceneImage, &m_iCurLightStatus, &m_iLastLightStatus, m_LightType == NIGHT);
	return S_OK;
}
#else
HRESULT CTrackerImpl::CheckTrafficLight(HV_COMPONENT_IMAGE* pSceneImage)
{
	static int nSceneStatusPos= 0;

	LIGHT_TEAM_STATUS ltsInfo;
	ltsInfo.nTeamCount = m_nLightCount;

	for(int i = 0; i<m_nLightCount; i++)
	{
		if( CTrackInfo::m_iCheckType == 0 )
		{
			if( CTrackInfo::m_iAutoScanLight == 1 )
			{
				m_rgLightInfo[i].UpdateStatus2( pSceneImage );
				m_rgLightRect[i] = m_rgLightInfo[i].GetLastRect();
			}
			else
			{
				m_rgLightInfo[i].UpdateStatus( pSceneImage );
				m_rgLightRect[i] = m_rgLightInfo[i].GetRect();
			}

			m_rgLightStatus[i] = m_rgLightInfo[i].GetLastStatus();
		}
		else
		{
			m_rgLightRect[i] = m_rgLightInfo[i].GetRect();
			BYTE8 bLevel = m_nIOLevel;
//			if( S_OK != m_pCallback->GetTrafficLightStatus(&bLevel) ) 
//			{
//				return E_FAIL;
//			}
			m_rgLightStatus[i] = GetIOStatus( i, bLevel );
		}
		ltsInfo.pdwStatus[i] = m_rgLightStatus[i];
		ltsInfo.pnLightCount[i] = m_rgLightInfo[i].GetPosCount();
	}

	//ȡ�õ�ǰ����
	bool fFlag = (CTrackInfo::m_iSceneCheckMode == 0);
/*
	char szTemp[255];
	sprintf(szTemp, "m_iSceneCount = %d, m_iLastOkLightStatus = %d, fFlag =%d\n", m_iSceneCount, m_iLastOkLightStatus, fFlag);
	strcat(g_szDebugInfo, szTemp);
*/
	int nCurScene = TransitionScene(ltsInfo, m_iSceneCount, m_iLastOkLightStatus, fFlag);
/*
	sprintf(szTemp, "m_iSceneCount = %d, m_iLastOkLightStatus = %d, fFlag =%d, nCurScene = %d\n", m_iSceneCount, m_iLastOkLightStatus, fFlag, nCurScene);
	strcat(g_szDebugInfo, szTemp);
*/
	//if(m_pCallback)
	if(true)
	{
		int nSceneStatus = -1;

		//�Ѻ�Ƶ�λ�ô���ȥ
		//m_pCallback->TrafficLightStatus(
		//	m_nLightCount, 
		//	m_rgLightStatus,
		//	&nSceneStatus,
		//	m_rgLightRect, 
		//	m_nRedLightCount,
		//	m_rgRedLightRect,
		//	pSceneImage
		//	);

		nSceneStatus = nCurScene;

		m_iCurLightStatus = nSceneStatus;
		//����û������˳���״̬����볡��״̬����
		//if(hr == S_OK && nSceneStatus != -1)
		if( nSceneStatus != -1 )
		{
			m_nErrorSceneCount = 0;
			SetSceneStatus(nSceneStatusPos, nSceneStatus);
			nSceneStatusPos++;	//��һ���洢λ��,ͬʱҲ�Ǽ���

			int iRelayTimeMs = m_nValidSceneStatusCount * 100;

			//���볡��״̬���ж��Ƿ����
			if(m_nValidSceneStatusCount > 0 && nSceneStatusPos >= m_nValidSceneStatusCount)
			{
				int nLastPos = nSceneStatusPos - 1;
				int nLastStatus = GetSceneStatus(nLastPos);
				BOOL fValid = TRUE;
				for( int i = nLastPos - 1; i > (nLastPos - m_nValidSceneStatusCount); i--)
				{
					if( GetSceneStatus(i) != nLastStatus)
					{
						fValid = FALSE;
						break;
					}
				}

				//����ǿ���ģʽ������һ֡�ж�Ϊ��һ����������.
				if( m_fCheckSpeed && !fValid && m_iLastOkLightStatus != -1 && m_iLastOkLightStatus != nSceneStatus)
				{
					int iNextStatus = (m_iLastOkLightStatus + 1) % m_iSceneCount; 
					if( iNextStatus == nSceneStatus ) 
					{
						fValid = true;
						iRelayTimeMs = 100;
						nLastStatus = nSceneStatus;
					}
				}

				if( fValid && nLastStatus != m_iLastLightStatus)
				{
					//������̵Ʊ�ɺ�������лƵ��ӳ�
					TRAFFICLIGHT_SCENE tsLast, tsRelay;
					GetLightScene(m_iLastOkLightStatus, &tsLast);
					GetLightScene(nLastStatus, &tsRelay);
					DWORD32 dwTick = GetSystemTick();
					bool fRelayTimeOut = true;
					if( CTrackInfo::m_iRedLightDelay > 0 )
					{
						if( tsLast.lsLeft == TLS_GREEN && tsRelay.lsLeft == TLS_RED )
						{
							if( m_ltRedRelay.dwLeft == 0 ) m_ltRedRelay.dwLeft = dwTick;
							if( int(dwTick - m_ltRedRelay.dwLeft) <  (CTrackInfo::m_iRedLightDelay * 1000) )
							{
								fRelayTimeOut = false;
							}
						}
						if( tsLast.lsForward == TLS_GREEN && tsRelay.lsForward == TLS_RED )
						{
							if( m_ltRedRelay.dwForward == 0 ) m_ltRedRelay.dwForward = dwTick;
							if( int(dwTick - m_ltRedRelay.dwForward) <  (CTrackInfo::m_iRedLightDelay * 1000) )
							{
								fRelayTimeOut = false;
							}
						}
						if( tsLast.lsRight == TLS_GREEN && tsRelay.lsRight == TLS_RED )
						{
							if( m_ltRedRelay.dwRight == 0 ) m_ltRedRelay.dwRight = dwTick;
							if( int(dwTick - m_ltRedRelay.dwRight) <  (CTrackInfo::m_iRedLightDelay * 1000) )
							{
								fRelayTimeOut = false;
							}
						}
						if( tsLast.lsTurn == TLS_GREEN && tsRelay.lsTurn == TLS_RED )
						{
							if( m_ltRedRelay.dwTurn == 0 ) m_ltRedRelay.dwTurn = dwTick;
							if( int(dwTick - m_ltRedRelay.dwTurn) <  (CTrackInfo::m_iRedLightDelay * 1000) )
							{
								fRelayTimeOut = false;
							}
						}
					}

					if( fRelayTimeOut )
					{
						m_ltRedRelay.Reset();
						//zhaopy
						TRAFFICLIGHT_SCENE tsShow;
						GetLightScene(nLastStatus, &tsShow);
						char szInfo[256];
						char szTemp[64];
						switch(tsShow.lsLeft)
						{
							case TLS_GREEN:
								strcpy( szTemp, "LG��");
								strcat(szInfo, szTemp);
								break;
							case TLS_RED:
								strcpy( szTemp, "LR��");
								strcat(szInfo, szTemp);
								break;
							default:
								strcpy( szTemp, "L??��");
								strcat(szInfo, szTemp);
								break;
						}
						switch(tsShow.lsForward)
						{
							case TLS_GREEN:
								strcpy( szTemp, "FG��");
								strcat(szInfo, szTemp);
								break;
							case TLS_RED:
								strcpy( szTemp, "FR��");
								strcat(szInfo, szTemp);
								break;
							default:
								strcpy( szTemp, "F??��");
								strcat(szInfo, szTemp);
								break;
						}
						switch(tsShow.lsRight)
						{
							case TLS_GREEN:
								strcpy( szTemp, "RG��");
								strcat(szInfo, szTemp);
								break;
							case TLS_RED:
								strcpy( szTemp, "RR��");
								strcat(szInfo, szTemp);
								break;
							default:
								strcpy( szTemp, "R??��");
								strcat(szInfo, szTemp);
								break;
						}
						HV_Trace(5, "%s\n", szInfo);
						//strcpy(g_szDebugInfo, szInfo);
						
						m_iLastLightStatus = nLastStatus;
						TRAFFICLIGHT_SCENE tsSceneStatus, tsNow;
						GetLightScene(m_iLastLightStatus, &tsSceneStatus);
						GetLightScene(m_iLastLightStatus, &tsNow);

						int iGreenRelayMs = 1000;

						//m_pCallback->TrafficSceneStatus(tsSceneStatus);

						//���º�ƿ�ʼʱ��
						if( nLastStatus != -1 )
						{
							m_iLastOkLightStatus = nLastStatus;
						}
						//һ������룬���̵�ʱ��ֻ��L��û��H����ARM����ͨ��ConvertTickToSystemTime����ת��,�ƹ����޸�,2011-07-21
						DWORD32 dwTimeLow = GetSystemTick() - iRelayTimeMs, dwTimeHigh = 0;
						//ConvertTickToSystemTime((GetSystemTick() - iRelayTimeMs), dwTimeLow, dwTimeHigh);

						if(  tsNow.lsLeft == TLS_RED && tsLast.lsLeft != TLS_RED )
						{
							m_redrealtime.dwLeftL = dwTimeLow;
							m_redrealtime.dwLeftH = dwTimeHigh;
						}
						else if( tsNow.lsLeft != TLS_RED )
						{
							m_redrealtime.dwLeftL = m_redrealtime.dwLeftH = 0;
						}
						if(  tsNow.lsLeft == TLS_GREEN && tsLast.lsLeft != TLS_GREEN )
						{
							m_greentick.dwLeft = GetSystemTick() - iRelayTimeMs - iGreenRelayMs;
						}
						else if( tsNow.lsLeft != TLS_GREEN )
						{
							m_greentick.dwLeft = 0;
						}

						if( tsNow.lsForward == TLS_RED && tsLast.lsForward != TLS_RED )
						{
							m_redrealtime.dwForwardL = dwTimeLow;
							m_redrealtime.dwForwardH = dwTimeHigh;
						}
						else if( tsNow.lsForward != TLS_RED )
						{
							m_redrealtime.dwForwardL = m_redrealtime.dwForwardH = 0;
						}
						if( tsNow.lsForward == TLS_GREEN && tsLast.lsForward != TLS_GREEN )
						{
							m_greentick.dwForward = GetSystemTick() - iRelayTimeMs - iGreenRelayMs;
						}
						else if( tsNow.lsForward != TLS_GREEN )
						{
							m_greentick.dwForward = 0;
						}

						if(  tsNow.lsRight == TLS_RED && tsLast.lsRight != TLS_RED )
						{
							m_redrealtime.dwRightL = dwTimeLow;
							m_redrealtime.dwRightH = dwTimeHigh;

						}
						else if( tsNow.lsRight != TLS_RED )
						{
							m_redrealtime.dwRightL = m_redrealtime.dwRightH = 0;
						}
						if(  tsNow.lsRight == TLS_GREEN && tsLast.lsRight != TLS_GREEN )
						{
							m_greentick.dwRight = GetSystemTick() - iRelayTimeMs - iGreenRelayMs;
						}
						else if( tsNow.lsRight != TLS_GREEN )
						{
							m_greentick.dwRight = 0;
						}

						if(  tsNow.lsTurn == TLS_RED && tsLast.lsTurn != TLS_RED )
						{
							m_redrealtime.dwTurnL = dwTimeLow;
							m_redrealtime.dwTurnH = dwTimeHigh;
						}
						else if( tsNow.lsTurn != TLS_RED )
						{
							m_redrealtime.dwTurnL = m_redrealtime.dwTurnH = 0;
						}
						if(  tsNow.lsTurn == TLS_GREEN && tsLast.lsTurn != TLS_GREEN )
						{
							m_greentick.dwTurn = GetSystemTick() - iRelayTimeMs - iGreenRelayMs;
						}
						else if( tsNow.lsTurn != TLS_GREEN )
						{
							m_greentick.dwTurn = 0;
						}
					}
				}
			}
		}
		else
		{
			//�����Ч�����������趨��ֵ����ѵ�ǰ�ĳ������-1.
			m_nErrorSceneCount++;
			if( m_nErrorSceneCount >= (m_nValidSceneStatusCount * 2))
			{
				m_iLastLightStatus = -1;
				TRAFFICLIGHT_SCENE tsSceneStatus;
				GetLightScene(m_iLastLightStatus, &tsSceneStatus);
				//m_pCallback->TrafficSceneStatus(tsSceneStatus);
				//��Ч�����п������м�״̬�����ʱ�䲻�����á�
			}
			//�����Ч����֡������趨��ֵ�������ú��̵ƿ�λ��,���ʱ������
			if( m_nErrorSceneCount > CTrafficLightInfo::MAX_ERROR_COUNT )
			{
				m_nErrorSceneCount = 0;
				for(int i = 0; i < 4; ++i)
				{
					m_redrealtime.Reset();
					m_greentick.Reset();
				}
				if( CTrackInfo::m_iAutoScanLight == 1 )
				{
					for(int i = 0; i <m_nLightCount; ++i)
					{
						if( S_OK == m_rgLightInfo[i].ReScan() )
						{
							HV_Trace(5, "TrafficLight ReScan ...\n");
						}
					}
				}
			}
		}
	}
	return S_OK;
}
#endif

HRESULT CTrackerImpl::DetectionParkingBox(
		PROCESS_ONE_FRAME_PARAM* pParam,
		PROCESS_ONE_FRAME_DATA* pProcessData,
		PROCESS_ONE_FRAME_RESPOND* pProcessRespond
	)
{
	int nResult = 0;
	const int MAX_DET_ROI = 100;
	svIPDetApi::DET_ROI rgDetRoi[MAX_DET_ROI];

	HRESULT hr(S_OK);

	if ( ( pParam == NULL ) || ( pProcessData == NULL ) || ( pProcessRespond == NULL ) ) return E_FAIL;
	HV_COMPONENT_IMAGE imgFrame = pProcessData->hvImageYuv;

	hr = m_cCarDetector.DetectCar(pParam, imgFrame, rgDetRoi, MAX_DET_ROI, &nResult);

	if(S_OK != hr)
	{
		return hr;
	}

	if(nResult >= MAX_DET_ROI)
	{
		nResult = MAX_DET_ROI;
	}

	pProcessRespond->cTrackRectInfo.dwTrackCount = nResult;

	for(int i = 0; i < nResult; i++)
	{
		if( i < 20 )
		{
			pProcessRespond->cTrackRectInfo.rgTrackRect[i].left = rgDetRoi[i].m_nLeft;
			pProcessRespond->cTrackRectInfo.rgTrackRect[i].top = rgDetRoi[i].m_nTop;
			pProcessRespond->cTrackRectInfo.rgTrackRect[i].right = rgDetRoi[i].m_nRight;
			pProcessRespond->cTrackRectInfo.rgTrackRect[i].bottom = rgDetRoi[i].m_nBottom;
			pProcessRespond->iDetType[i] = rgDetRoi[i].m_nType;
		}
	}

	return S_OK;
}


HRESULT CTrackerImpl::ProcessOneFrame(
		PROCESS_ONE_FRAME_PARAM* pParam,
		PROCESS_ONE_FRAME_DATA* pProcessData,
		PROCESS_ONE_FRAME_RESPOND* pProcessRespond
)
{
	HV_COMPONENT_IMAGE& imgSnapFrame = pProcessData->hvImageYuv;
	HV_COMPONENT_IMAGE imgCapFrame;
	SetHvImageData(&imgCapFrame, 0, NULL);
	SetHvImageData(&imgCapFrame, 1, NULL);
	SetHvImageData(&imgCapFrame, 2, NULL);

	if ( (imgSnapFrame.iWidth != m_nWidth || imgSnapFrame.iHeight != m_nHeight)
			|| m_fIsModifyParam )
	{
		FRAME_PROPERTY cFrameProperty;
		cFrameProperty.iWidth = imgSnapFrame.iWidth;
		cFrameProperty.iHeight = imgSnapFrame.iHeight;
		cFrameProperty.iStride = imgSnapFrame.iStrideWidth[0];

		SetFrameProperty(cFrameProperty, pProcessRespond);
		m_fIsModifyParam = false;
	}

//	char szMsg[256];
//	sprintf(szMsg, "ProcessOneFrame begin.");
//	OutPutDebugMsg(LOG_LV_NORMAL, szMsg);

	if (!m_fInitialized)
	{
		return E_UNEXPECTED;
	}

	if (pParam->fIsCaptureImage)
	{
		CRect rc(
			pParam->cFrameRecognizeParam.cRecogSnapArea.DetectorAreaLeft,
			pParam->cFrameRecognizeParam.cRecogSnapArea.DetectorAreaTop,
			pParam->cFrameRecognizeParam.cRecogSnapArea.DetectorAreaRight,
			pParam->cFrameRecognizeParam.cRecogSnapArea.DetectorAreaBottom
			);
		return RecogSnapImg(
			pParam,
			pProcessData,
			pProcessRespond,
			imgSnapFrame,
			rc,
			pParam->cFrameRecognizeParam.cRecogSnapArea.nDetectorMinScaleNum,
			pParam->cFrameRecognizeParam.cRecogSnapArea.nDetectorMaxScaleNum
			);
	}

    if ( pParam->cDetectParam.fIsDetectStopBox )
	{
		pProcessData->hvImageYuv.iStrideWidth[0] = (pProcessData->hvImageYuv.iStrideWidth[0] >> 1);
		pProcessData->hvImageYuv.iHeight <<= 1;
		return DetectionParkingBox(pParam,pProcessData,pProcessRespond);
	}

	return S_OK;
}


bool CTrackerImpl::CheckPeccancyTrigger(CTrackInfo* pTrack)
{
	if( pTrack == NULL ) return false;
	if( pTrack->m_fHasTrigger ) return true;

	int iTwoPos = CTrackInfo::m_iCaptureTwoPos * m_nHeight / 100;
	int iCurPos = pTrack->LastInfo().rcPos.CenterPoint().y;
	bool fIsConverse = (pTrack->LastInfo().rcPos.CenterPoint().y - pTrack->m_rgPlateInfo[0].rcPos.CenterPoint().y) > (m_nHeight * 3 / 100);
	bool fPassPos = fIsConverse ? (iCurPos > iTwoPos) : (iTwoPos > iCurPos);

	if( fPassPos && CheckPeccancy(pTrack) )
	{
		DWORD32 dwCurTick = GetSystemTick();
		// trigger
		if( (dwCurTick - m_dwLastTriggerTick) > MIN_TRIGGER_TIME )
		{
			TriggerCamera(0xff);
			m_dwLastTriggerTick = dwCurTick;
			// set
			pTrack->m_fHasTrigger = true;
			pTrack->m_dwTriggerTimeMs = dwCurTick; // TODO: image time?
		}
		else
		{
			// set
			pTrack->m_fHasTrigger = true;
			pTrack->m_dwTriggerTimeMs = m_dwLastTriggerTick; // TODO: image time?
		}
	}

	return true;
}
//�ж��Ƿ��п���Υ��
bool CTrackerImpl::CheckPeccancy(CTrackInfo *pTrack)
{
	if( pTrack == NULL ) return false;

	if( pTrack->m_cPlateInfo < 3 ) return false;

	int iMove = abs(pTrack->m_rgPlateInfo[0].rcPos.CenterPoint().y -  pTrack->LastInfo().rcPos.CenterPoint().y);
	if( iMove < (CTrackInfo::m_iHeight * 2 / 100) ) return false;

	//�Ƿ��Ƿǻ�������
	int iRoad = MatchRoad(pTrack->LastInfo().rcPos.CenterPoint().x, pTrack->LastInfo().rcPos.CenterPoint().y);
	if( iRoad < 0 || iRoad >= CTrackInfo::m_iRoadNumber )
	{
		return false;
	}
	if( CTrackInfo::m_roadInfo[iRoad].iRoadType & RRT_ESTOP )
	{
		return true;
	}
	//�Ƿ��п��ܴ����
	BOOL fRush = FALSE;
	TRAFFICLIGHT_SCENE tsStatus;
	//GetLightScene(m_iLastOkLightStatus, &tsStatus);
	m_cTrafficLight.GetLightScene(m_iLastLightStatus, &tsStatus);
	if( ((CTrackInfo::m_roadInfo[iRoad].iRoadType & RRT_FORWARD) && tsStatus.lsForward == TLS_RED)
		|| ((CTrackInfo::m_roadInfo[iRoad].iRoadType & RRT_LEFT) && tsStatus.lsLeft == TLS_RED)
		|| ((CTrackInfo::m_roadInfo[iRoad].iRoadType & RRT_RIGHT) && tsStatus.lsRight == TLS_RED)
		|| ((CTrackInfo::m_roadInfo[iRoad].iRoadType & RRT_TURN) && tsStatus.lsTurn == TLS_RED) )
	{
		return true;
	}

	//�Ƿ�ѹ��
	CROSS_OVER_LINE_TYPE olt = IsOverYellowLine((*pTrack));
	if( olt != COLT_INVALID && olt != COLT_NO )
	{
		return true;
	}
	//�Ƿ�����
	switch (m_pParam->g_MovingDirection)
	{
	case MD_TOP2BOTTOM:
		if (pTrack->LastInfo().rcPos.top -
			pTrack->m_rgPlateInfo[0].rcPos.top > -(m_nHeight * 3 / 100))
		{
		}
		else
		{
			return true;
		}
		break;
	case MD_BOTTOM2TOP:
		if (pTrack->LastInfo().rcPos.top -
			pTrack->m_rgPlateInfo[0].rcPos.top
			< (m_nHeight * 3  / 100))
		{
		}
		else
		{
			return true;
		}
		break;
	default:
		break;
	}

	return false;
}

bool CTrackerImpl::CheckObjectPeccancyTrigger(CObjTrackInfo* pObjTrackInfo, int iObjIndex)
{
	if( pObjTrackInfo == NULL ) return false;
	if( pObjTrackInfo->m_fHasTrigger ) return true;

	//��ǰ���ڸ��ٳ��Ƶ�����
	bool fPlateTracking = false;
	for (int j = 0; j < m_cTrackInfo; j++)
	{
		if (m_rgTrackInfo[j].m_State == tsInit ||
			m_rgTrackInfo[j].m_State == tsWaitRemove)
		{
			continue;
		}
		if (m_rgObjTrackInfo[iObjIndex].MatchPlateTrackID(m_rgTrackInfo[j].m_nID))
		{
			fPlateTracking = true;
			break;
		}
	}
	if( fPlateTracking ) return true;

	int iTwoPos = CTrackInfo::m_iCaptureTwoPos * m_nHeight / 100;
	int iCurPos = pObjTrackInfo->LastInfo().m_rcFG.bottom;
	bool fIsConverse = (pObjTrackInfo->LastInfo().m_rcFG.bottom - pObjTrackInfo->LastInfo().m_rcFG.bottom) > (m_nHeight * 3  / 100);
	bool fPassPos = fIsConverse ? (iCurPos > iTwoPos) : (iTwoPos > iCurPos);

	if( fPassPos && CheckObjectPeccancy(pObjTrackInfo) )
	{
		DWORD32 dwCurTick = GetSystemTick();
		// trigger
		if( (dwCurTick - m_dwLastTriggerTick) > MIN_TRIGGER_TIME )
		{
			TriggerCamera(0xff);
			m_dwLastTriggerTick = dwCurTick;
			// set
			pObjTrackInfo->m_fHasTrigger = true;
			pObjTrackInfo->m_dwTriggerTimeMs = dwCurTick; // TODO: image time?
		}
		else
		{
			// set
			pObjTrackInfo->m_fHasTrigger = true;
			pObjTrackInfo->m_dwTriggerTimeMs = m_dwLastTriggerTick; // TODO: image time?
		}
	}

	return true;
}

//�ж����Ƴ��Ƿ����Υ��
bool CTrackerImpl::CheckObjectPeccancy(CObjTrackInfo* pObjTrackInfo)
{
	if( pObjTrackInfo == NULL ) return false;

	if( pObjTrackInfo->m_iObjInfoCount < 2 ) return false;

	int iMove = abs(pObjTrackInfo->m_rgObjInfo[0].m_rcFG.CenterPoint().y -  pObjTrackInfo->LastInfo().m_rcFG.CenterPoint().y);
	if( iMove < (CTrackInfo::m_iHeight * 2 / 100) ) return false;

	//�Ƿ��Ƿǻ�������
	int iRoad = MatchRoad(pObjTrackInfo->LastInfo().m_rcFG.CenterPoint().x, pObjTrackInfo->LastInfo().m_rcFG.CenterPoint().y);
	if( iRoad >= 0 && iRoad < CTrackInfo::m_iRoadNumber
		&& (CTrackInfo::m_roadInfo[iRoad].iRoadType & RRT_ESTOP) )
	{
		return true;
	}
	//�Ƿ��п��ܴ����
	BOOL fRush = FALSE;
	TRAFFICLIGHT_SCENE tsStatus;
	//GetLightScene(m_iLastOkLightStatus, &tsStatus);
	m_cTrafficLight.GetLightScene(m_iLastLightStatus, &tsStatus);
	if( ((CTrackInfo::m_roadInfo[iRoad].iRoadType & RRT_FORWARD) && tsStatus.lsForward == TLS_RED)
		|| ((CTrackInfo::m_roadInfo[iRoad].iRoadType & RRT_LEFT) && tsStatus.lsLeft == TLS_RED)
		|| ((CTrackInfo::m_roadInfo[iRoad].iRoadType & RRT_RIGHT) && tsStatus.lsRight == TLS_RED)
		|| ((CTrackInfo::m_roadInfo[iRoad].iRoadType & RRT_TURN) && tsStatus.lsTurn == TLS_RED) )
	{
		fRush = TRUE;
	}

	//�Ƿ�����
	switch (m_pParam->g_MovingDirection)
	{
	case MD_TOP2BOTTOM:
		if (pObjTrackInfo->LastInfo().m_rcFG.top -
			pObjTrackInfo->m_rgObjInfo[0].m_rcFG.top > -(m_nHeight * 3 / 100))
		{
		}
		else
		{
			return true;
		}
		break;
	case MD_BOTTOM2TOP:
		if (pObjTrackInfo->LastInfo().m_rcFG.top -
			pObjTrackInfo->m_rgObjInfo[0].m_rcFG.top
			< m_nHeight * 3 / 100)
		{
		}
		else
		{
			return true;
		}
		break;
	default:
		break;
	}

	return false;
}
HRESULT CTrackerImpl::CalcFrameSpeed(CTrackInfo *pTrack)
{
	int iPosOne  = CTrackInfo::m_iCaptureOnePos * CTrackInfo::m_iHeight / 100;
	if(pTrack->m_iFrameSpeedCount < 1000 && pTrack->m_cPlateInfo > 1 && pTrack->LastInfo().rcPos.CenterPoint().y < iPosOne)
	{
		float fltPos = (float)(pTrack->m_rgPlateInfo[0].rcPos.CenterPoint().y - pTrack->LastInfo().rcPos.CenterPoint().y);
		float fltTime = float((pTrack->LastInfo().dwFrameTime - pTrack->m_rgPlateInfo[0].dwFrameTime) / 1000.0);
		if(fltTime * 1000 > 0)
			pTrack->m_rgfltFrameSpeed[ pTrack->m_iFrameSpeedCount++ ] = fltPos / fltTime;
		else
			pTrack->m_rgfltFrameSpeed[ pTrack->m_iFrameSpeedCount++ ] = 0;
	}
	return S_OK;
}

HRESULT CTrackerImpl::ProcessTrackState(
	CTrackInfo *pTrack, 								//��ǰ����ĸ���
	CRect &rcTrackArea, 								//��������
	PlateInfo &plateObserved,							//��ǰ֡��⵽�ĳ�����Ϣ
	HV_COMPONENT_IMAGE &imgCalibratedFrame,		//������������Ƶ֡
	CPersistentComponentImage &imgBestRectified,		//���������ĳ���Сͼ
	HV_COMPONENT_IMAGE &imgOriSnapFrame,					//ԭʼ����Ƶ֡
	HV_COMPONENT_IMAGE &imgOriCapFrame,	 				//ԭʼ��ȫ��֡
	PROCESS_ONE_FRAME_PARAM* pParam,
	PROCESS_ONE_FRAME_RESPOND* pProcessRespond
)
{
	//ץ�Ĳ����ģʽ�²���Ҫ�����ص���ʱ��ı�
	if(CTrackInfo::m_iFlashlightMode == 1)
	{
		CalcFrameSpeed(pTrack);
	}

	//weikt 20110423 ��֤����Сͼ����Ƶ���������
	CRect rectIntersect;
	CRect rcPlatePosLast = plateObserved.rcPos;
	CRect rcMax(0, 0, m_nWidth, m_nHeight);
	CRect rcVideoDet;
	RTN_HR_IF_FAILED(MapParamRect(rcVideoDet, rcMax, m_rcVideoDetArea));
	BOOL fIntersect = rectIntersect.IntersectRect(&plateObserved.rcPos, &rcVideoDet);
	if (fIntersect)
	{
		if(rectIntersect != plateObserved.rcPos)
		{
			HV_Trace(1, "IntersectRect ...\n");
			plateObserved.rcPos = rectIntersect;
		}
	}
	else
	{
		//����ȫ����������ĳ�����Ϊ�� fObservaionExists
		pTrack->m_fObservationExists = false;
	}
	//weikt end


	if (pTrack->m_fObservationExists)
	{
		pTrack->m_nObservedFrames++;
		if (pTrack->m_cPlateInfo>0 &&
			0 == HV_memcmp(plateObserved.rgbContent,
			pTrack->LastInfo().rgbContent,
			sizeof(plateObserved.rgbContent)))
		{
			pTrack->m_nResultEqualTimes++;
		}
		else
		{
			pTrack->m_nResultEqualTimes=0;
		}

		pTrack->m_rgPlateInfo[pTrack->m_cPlateInfo++]=plateObserved;

		HV_COMPONENT_IMAGE imgObservedPlate;
		// ת��ͼƬ��ʽ,����Ӧʶ��ģ��, ��ת��BT1120��ʽ�����⴦��
		CPersistentComponentImage imgPlateRotateY;
		if (imgCalibratedFrame.nImgType == HV_IMAGE_BT1120_ROTATE_Y)
		{
			imgPlateRotateY.Create(HV_IMAGE_YUV_422, plateObserved.rcPos.Width(), plateObserved.rcPos.Height());
			imgPlateRotateY.CropAssign(imgCalibratedFrame, plateObserved.rcPos);
			imgObservedPlate = imgPlateRotateY;
		}
		else
		{
			RTN_HR_IF_FAILED(CropImage(
				imgCalibratedFrame, plateObserved.rcPos, &imgObservedPlate
			));
		}
		

		if( CTrackInfo::m_iFlashlightMode == 1 && m_iAvgY < CTrackInfo::m_iFlashlightThreshold  )
		{
			CheckPeccancyTrigger(pTrack);
		}
		if (GetCurrentParam()->g_PlateRcogMode == PRM_TOLLGATE)
		{
			//���CarArrive
			CheckCarArrive(pProcessRespond, pParam->iDiffTick);
		}

		RTN_HR_IF_FAILED(pTrack->UpdatePlateImage(
			imgObservedPlate, pParam, m_iLastLightStatus, m_iCurLightStatus, pProcessRespond 
		));
	}

//	// ����ͼģʽ���滻������ͼ
//	RTN_HR_IF_FAILED(pTrack->UpdateBestImage(pImage));

	// State machine
	switch (pTrack->m_State)
	{
	case tsNew: 	// if the current track is new

		if (pTrack->m_fObservationExists)
		{
			pTrack->m_nMissingTime = 0;
			// perform the state transition
			if (pTrack->m_nObservedFrames >= m_pParam->g_nContFrames_EstablishTrack
				|| (pTrack->m_rgPlateInfo[0].nPlateType == PLATE_DOUBLE_GREEN && pTrack->m_nObservedFrames >= 2)
				)
			{
				pTrack->m_State=tsMoving;
			}
		}
		else
		{
			pTrack->m_nMissingTime++;
			pTrack->LastInfo().iLastCharHeight=-1;
			pTrack->LastInfo().iLastVscale=-1;
			if (pTrack->m_nMissingTime > 1)
			{
				pTrack->m_State = tsWaitRemove;
				pTrack->m_nVoteCondition = (int)NEW2END;
			}
		}
		break;
	case tsMoving:
		if(pTrack->m_fObservationExists)
		{
			pTrack->m_nMissingTime = 0;
			// ����fastģʽ��ֻҪ����end���򣬾ͽ���track
			BOOL fInEndTrackArea = !rcTrackArea.Contains(pTrack->LastInfo().rcPos);
			if (fInEndTrackArea && m_pParam->g_PlateRecogSpeed == PR_SPEED_FAST)
			{
				pTrack->m_State=tsEnd;
				pTrack->m_nVoteCondition = (int)LEAVE_TRACK_RECT;
				break;
			}
		}
		else
		{
			pTrack->m_nMissingTime++;
			pTrack->LastInfo().iLastCharHeight=-1;
			pTrack->LastInfo().iLastVscale=-1;
			BOOL fPredictInEndTrackArea = !rcTrackArea.IntersectsWith(pTrack->PredictPosition(pParam->dwImageTime, m_pParam->m_iDetectReverseEnable, m_pScaleSpeed));
			if (fPredictInEndTrackArea && pTrack->m_nMissingTime > m_pParam->g_nMissFrames_EndTrackQuick)
			{
				pTrack->m_State=tsEnd;
				pTrack->m_nVoteCondition = (int)ENDRECT_MISS;
			}
			else if (pTrack->m_nMissingTime > m_pParam->g_nMissFrames_EndTrack)
			{
				pTrack->m_State=tsEnd;
				pTrack->m_nVoteCondition = TRACK_MISS;
			}
		}
		break;
	default:
		break;
	}

	return S_OK;
}

HRESULT CTrackerImpl::ShowTracks(PROCESS_ONE_FRAME_PARAM* pParam, PROCESS_ONE_FRAME_DATA* pProcessData, PROCESS_ONE_FRAME_RESPOND* pProcessRespond)
{
//	if (NULL == pCurImage)
//	{
//		return S_FALSE;
//	}
// TODO:��ɵ���ģ��
/*	if (m_pInspector)
	{
		for (int i=0; i<ipTrackMax-ipTrack0; i++)
		{
			if (i<m_cTrackInfo)
			{
				CTrackInfo *pTrackInfo = m_rgTrackInfo + i;

				HV_COMPONENT_IMAGE imgBestPlate;
				RTN_HR_IF_FAILED(m_rgTrackInfo[i].GetLastPlateImage()->GetImage(&imgBestPlate));
				RTN_HR_IF_FAILED(m_pInspector->ShowComponentImage(
					ipTrack0+i, &imgBestPlate, (DEBUG_INSPECTOR_INFO *)pTrackInfo));
			}
			else
			{
				RTN_HR_IF_FAILED(m_pInspector->ShowComponentImage(
					ipTrack0+i, NULL));
			}
		}
	}
*/
	//���CarArrive
	CheckCarArrive(pProcessRespond, pParam->iDiffTick);

	for (int i=0; i<m_cTrackInfo; i++)
	{	
		//�羯����Ҫ����ԤͶƱ,2011-8-17
		if(GetCurrentParam()->m_fUsedTrafficLight)
		{
			//�ж���ͶƱ
			if(m_cfgParam.fPreVote
			&& m_rgTrackInfo[i].PreVote() 
			&& m_rgTrackInfo[i].AverageConfidence()*100 > m_iAverageConfidenceQuan
			&& m_rgTrackInfo[i].AverageFirstConfidence()*100 > m_iFirstConfidenceQuan)
			{
				for (int j = 0; j < m_cObjTrackInfo; j++)
				{
					if (m_rgObjTrackInfo[j].m_objTrackState != OBJ_TRACK_INIT 
					 && m_rgObjTrackInfo[j].MatchPlateTrackID(m_rgTrackInfo[i].m_nID))
						m_rgObjTrackInfo[j].ClearImage(pProcessRespond);
				}
			}			
		}
		//���CarArrive
		if (m_rgTrackInfo[i].m_State == tsMoving &&
			!m_rgTrackInfo[i].m_fCarArrived)
		{
			int iTriggerPos;
			if (m_rgTrackInfo[i].LastInfo().color == PC_YELLOW)
			{
				iTriggerPos = m_iCarArrivedPosYellow;
			}
			else
			{
				iTriggerPos = m_iCarArrivedPos;
			}
			CRect rcPos = m_rgTrackInfo[i].LastInfo().rcPos;
			int iPosY = iTriggerPos * m_nHeight / 100;
			if (m_pParam->g_MovingDirection == MD_TOP2BOTTOM 
				&& rcPos.CenterPoint().y > iPosY)
			{
				//���㴥����ʱ���벹��
				float fltDistance = m_pScaleSpeed->CalcActureDistance(rcPos.CenterPoint(), CPoint(rcPos.CenterPoint().x, iPosY));
				fltDistance = m_iCarArrivedDelay - fltDistance;
				ProcessCarArrive(&m_rgTrackInfo[i], pParam->dwImageTime, fltDistance, pProcessRespond);
			}
			else if( m_pParam->g_MovingDirection == MD_BOTTOM2TOP 
				&& rcPos.CenterPoint().y < iPosY )
			{
				//���㴥����ʱ���벹��
				float fltDistance = m_pScaleSpeed->CalcActureDistance(rcPos.CenterPoint(), CPoint(rcPos.CenterPoint().x, iPosY));
				fltDistance = m_iCarArrivedDelay - fltDistance;
				ProcessCarArrive(&m_rgTrackInfo[i], pParam->dwImageTime, fltDistance, pProcessRespond);
			}
		}
		
		bool bOutputFinalResult = m_rgTrackInfo[i].PrepareOutPutFinalResult(pParam->dwImageTime);
		
		bool fDeleteObjTrack = false;
		if ( bOutputFinalResult )
		{
			if (!m_rgTrackInfo[i].m_fCarArrived)
			{
				int iTriggerPos;
				if (m_rgTrackInfo[i].LastInfo().color == PC_YELLOW)
				{
					iTriggerPos = m_iCarArrivedPosYellow;
				}
				else
				{
					iTriggerPos = m_iCarArrivedPos;
				}
				CRect rcPos = m_rgTrackInfo[i].LastInfo().rcPos;
				int iPosY = iTriggerPos * m_nHeight / 100;

				//���㴥����ʱ���벹��
				float fltDistance = m_pScaleSpeed->CalcActureDistance(rcPos.CenterPoint(), CPoint(rcPos.CenterPoint().x, iPosY));
				fltDistance = m_iCarArrivedDelay + fltDistance;
				ProcessCarArrive(&m_rgTrackInfo[i], pParam->dwImageTime, fltDistance, pProcessRespond);
			}
			m_rgTrackInfo[i].m_fReverseRunVoted = false;
			switch (m_pParam->g_MovingDirection)
			{
			case MD_TOP2BOTTOM:
				if (m_rgTrackInfo[i].LastInfo().rcPos.top - 
					m_rgTrackInfo[i].m_rgPlateInfo[0].rcPos.top < -(m_nHeight * m_iSpan * 0.01f))
				{
					m_rgTrackInfo[i].m_fReverseRunVoted = true;
				}
				break;
			case MD_BOTTOM2TOP:
				if (m_rgTrackInfo[i].LastInfo().rcPos.top -
					m_rgTrackInfo[i].m_rgPlateInfo[0].rcPos.top
					> m_nHeight * m_iSpan * 0.01f)
				{
					m_rgTrackInfo[i].m_fReverseRunVoted = true;
				}
				break;
			case MD_LEFT2RIGHT:
				if (m_rgTrackInfo[i].LastInfo().rcPos.left - 
					m_rgTrackInfo[i].m_rgPlateInfo[0].rcPos.left < -(m_nWidth * m_iSpan * 0.01f))
				{
					m_rgTrackInfo[i].m_fReverseRunVoted = true;
				}
				break;
			case MD_RIGHT2LEFT:
				if (m_rgTrackInfo[i].LastInfo().rcPos.left -
					m_rgTrackInfo[i].m_rgPlateInfo[0].rcPos.left
					> m_nWidth * m_iSpan * 0.01f)
				{
					m_rgTrackInfo[i].m_fReverseRunVoted = true;
				}
				break;
			}

			// Output Final Result
			RTN_HR_IF_FAILED(m_rgTrackInfo[i].Vote(
				m_rgTrackInfo[i].m_nVotedType,
				m_rgTrackInfo[i].m_nVotedColor,
				m_rgTrackInfo[i].m_rgbVotedResult));
			m_rgTrackInfo[i].m_fVoted = true;

			//�ж�����·��Ƶ�Ƿ�����ͬ�ĸ��١�
			for (int j = i + 1; j < m_cTrackInfo; j++)
			{
				if (m_rgTrackInfo[i].m_iVideoID == m_rgTrackInfo[j].m_iVideoID ||
					m_rgTrackInfo[j].m_fVoted)
					continue;
				if (m_rgTrackInfo[i].IsTwins(m_rgTrackInfo[j]))
				{
					m_rgTrackInfo[j].m_fVoted = true;
				}
			}

			//���ݵ����Ƹ��º����ֿ��
			if (PLATE_NORMAL == m_rgTrackInfo[i].m_nVotedType ||
				PLATE_POLICE == m_rgTrackInfo[i].m_nVotedType)
			{
				int iBottomIndex;
				for (iBottomIndex = m_rgTrackInfo[i].m_cPlateInfo - 1; iBottomIndex >= 0; iBottomIndex--)
				{
					if (memcmp(m_rgTrackInfo[i].m_rgPlateInfo[iBottomIndex].rgbContent, m_rgTrackInfo[i].m_rgbVotedResult, 7) == 0)
					{
						break;
					}
				}
				int iTopIndex;
				for (iTopIndex = 0; iTopIndex < iBottomIndex; iTopIndex++)
				{
					if (memcmp(m_rgTrackInfo[i].m_rgPlateInfo[iTopIndex].rgbContent, m_rgTrackInfo[i].m_rgbVotedResult, 7) == 0)
					{
						break;
					}
				}
				if (iBottomIndex > iTopIndex)
				{
					int iTopY = m_rgTrackInfo[i].m_rgPlateInfo[iTopIndex].rcPos.top;
					int iBottomY = m_rgTrackInfo[i].m_rgPlateInfo[iBottomIndex].rcPos.top;
					int iTopWidth = m_rgTrackInfo[i].m_rgPlateInfo[iTopIndex].rgrcChar[6].right - 
						m_rgTrackInfo[i].m_rgPlateInfo[iTopIndex].rgrcChar[2].left;
					int iBottomWidth = m_rgTrackInfo[i].m_rgPlateInfo[iBottomIndex].rgrcChar[6].right - 
						m_rgTrackInfo[i].m_rgPlateInfo[iBottomIndex].rgrcChar[2].left;
					if (iBottomWidth < iTopWidth)
					{
						int iTemp = iTopY;
						iTopY = iBottomY;
						iBottomY = iTemp;
						iTemp = iTopWidth;
						iTopWidth = iBottomWidth;
						iBottomWidth = iTemp;
					}
					float fltDy = (float)(iBottomY - iTopY);
					float fltDWidth = (float)(iBottomWidth - iTopWidth);
					if (fltDy > 1.0f && fltDWidth > 1.0f)
					{
						float fltK = fltDWidth / fltDy;
						for (int j = 0; j < 39; j++)
						{
							float fltNewWidth = (j * 25 - iTopY) * fltK + iTopWidth + 0.5f;
							if (m_rgiFiveCharWidth[j] == 0)
							{
								m_rgiFiveCharWidth[j] = (int)fltNewWidth;
							}
							else
							{
								m_rgiFiveCharWidth[j] = (int)(m_rgiFiveCharWidth[j] * 0.75 + fltNewWidth * 0.25);
							}
							if (m_rgiFiveCharWidth[j] < 0)
							{
								m_rgiFiveCharWidth[j] = 0;
							}
						}
						m_rgiFiveCharWidth[39] = 1;
					}
					RTN_HR_IF_FAILED(m_rgpObjectDetector[pParam->iVideoID]->SetPlateWidth(m_rgiFiveCharWidth, 40, 25));
				}
			}

			if (MD_BOTTOM2TOP == m_pParam->g_MovingDirection)
			{
				if (0 == m_iEnableDefaultDBType)
				{
					if (PLATE_DOUBLE_MOTO == m_rgTrackInfo[i].m_nVotedType)
					{
						m_rgTrackInfo[i].m_nVotedType = PLATE_DOUBLE_YELLOW;
					}
				}
				else if (1 == m_iEnableDefaultDBType)
				{
					if (PLATE_DOUBLE_YELLOW == m_rgTrackInfo[i].m_nVotedType)
					{
						m_rgTrackInfo[i].m_nVotedType = PLATE_DOUBLE_MOTO;
					}
				}
				else
				{
					if (3 == m_iEnableDefaultDBType && !m_rgiFiveCharWidth[39])
					{
						if (m_iMiddleWidth > 0 && m_iBottomWidth > 0
							&& m_iBottomWidth - m_iMiddleWidth >= 0)
						{
							for (int j = 38; j >= 0; j--)
							{
								m_rgiFiveCharWidth[j] = m_iBottomWidth - 
									(m_iBottomWidth - m_iMiddleWidth) * 
									(m_nHeight - j * 25 - 12) * 2 / m_nHeight;
								if (m_rgiFiveCharWidth[j] < 0)
								{
									m_rgiFiveCharWidth[j] = 0;
								}
							}
						}
						m_rgiFiveCharWidth[39] = 1;
					}
					if (PLATE_DOUBLE_YELLOW == m_rgTrackInfo[i].m_nVotedType ||
						PLATE_DOUBLE_MOTO == m_rgTrackInfo[i].m_nVotedType)
					{
						if (m_nDefaultDBType)
						{
							m_rgTrackInfo[i].m_nVotedType = PLATE_DOUBLE_MOTO;
						}
						else
						{
							m_rgTrackInfo[i].m_nVotedType = PLATE_DOUBLE_YELLOW;
						}

						int iOffset, iCharWidth;
						bool fChangeType(false);
						for (int j = 0; j < m_rgTrackInfo[i].m_cPlateInfo; j++)
						{
							iOffset = (m_rgTrackInfo[i].m_rgPlateInfo[j].rcPos.bottom 
								+ m_rgTrackInfo[i].m_rgPlateInfo[j].rcPos.top) / 50;
							iCharWidth = m_rgTrackInfo[i].m_rgPlateInfo[j].rgrcChar[6].right
								- m_rgTrackInfo[i].m_rgPlateInfo[j].rgrcChar[2].left;
							if (iOffset >=0 && iOffset < 39 && iCharWidth > 0)
							{
								if (!fChangeType && m_rgiFiveCharWidth[iOffset] > 0)
								{
									fChangeType = true;
									if (iCharWidth < m_rgiFiveCharWidth[iOffset])
									{
										m_rgTrackInfo[i].m_nVotedType = PLATE_DOUBLE_MOTO;
									}
									else
									{
										m_rgTrackInfo[i].m_nVotedType = PLATE_DOUBLE_YELLOW;
									}
								}
							}
						}
					}
				}
			}

			float fltFirstCharConfidence = m_rgTrackInfo[i].AverageFirstConfidence();
			float fltAverageConfidence = m_rgTrackInfo[i].AverageConfidence();

			HV_DebugInfo( DEBUG_STR_TRACK_FILE, "\nfirst char = %f\n", fltFirstCharConfidence );
			HV_DebugInfo( DEBUG_STR_TRACK_FILE, "average = %f\n", fltAverageConfidence );

			//���ʹ�ܺ��̵�ץ�ģ�����˵�δ��ͣ���߾�ֹ�ĳ����Լ���ֹ�ĳ���.
			bool fMove = false;
			if( m_pParam->m_fUsedTrafficLight && m_pParam->g_MovingDirection == MD_BOTTOM2TOP )
			{
				const int MIN_REMOVE = 3;
				int iLastPos = (int)(((float)m_rgTrackInfo[i].LastInfo().rcPos.CenterPoint().y / (float)CTrackInfo::m_iHeight) * 100 + 0.5);
				int iFirstPos = (int)(((float)m_rgTrackInfo[i].m_rgPlateInfo[0].rcPos.CenterPoint().y / (float)CTrackInfo::m_iHeight) * 100 + 0.5);

				bool fIsStop = (IsCarStop(&m_rgTrackInfo[i]) || (HV_ABS(iFirstPos - iLastPos) < MIN_REMOVE));
				if( m_iOutputInLine == 0 
					&& !m_rgTrackInfo[i].m_fReverseRunVoted 
					&&  fIsStop )
				{
					fMove = true;
//						HV_Trace("Move... \n");
				}
				else if( m_iOutputInLine == 1
					&& !m_rgTrackInfo[i].m_fReverseRunVoted 
					&& ( (HV_ABS(iFirstPos - iLastPos) < MIN_REMOVE) ))
				{
					fMove = true;
//						HV_Trace("Move... \n");
				}
			}

			//added by liujie , ���ڳ�ͷ���������ƶ����������
			bool fVoteSimCon = true;
			if (GetCurrentParam()->g_PlateRcogMode == PRM_HIGHWAY_HEAD || GetCurrentParam()->g_PlateRcogMode == PRM_CAP_FACE)
			{
				fVoteSimCon = (m_rgTrackInfo[i].m_iVoteSimilarityNum >= m_pParam->g_nContFrames_EstablishTrack);
			} 
			else 
			{
				fVoteSimCon = (m_rgTrackInfo[i].m_iVoteSimilarityNum > m_pParam->g_nContFrames_EstablishTrack + 1);
			}//end added

			if (!fMove &&  
				fltAverageConfidence * 100 > m_iAverageConfidenceQuan &&
				(GetCurrentParam()->g_PlateRcogMode != PRM_TOLLGATE 
				|| GetCurrentParam()->g_PlateRcogMode == PRM_TOLLGATE && fltFirstCharConfidence * 100 >= 5) &&
				m_rgTrackInfo[i].m_nRecogInVaildCount <= m_cfgParam.nRecogInVaildRatioTH &&
				/*fltFirstCharConfidence * 100 > m_iFirstConfidenceQuan &&*/ fVoteSimCon)
			{
				//�����շ�վģʽ��
				if(GetCurrentParam()->g_PlateRcogMode != PRM_TOLLGATE && fltFirstCharConfidence * 100 > m_iFirstConfidenceQuan)
				{
					if (m_pLastResult && m_pLastResult->IsTwinResult(m_rgTrackInfo[i]))
					{
						//������ͬ����ʱ����Ӧ����Ƶ������Ҳȥ��.
						RTN_HR_IF_FAILED(m_pLastResult->UpdateResult(m_rgTrackInfo[i]));
						fDeleteObjTrack = true;
					}
					else
					{
						if( m_pParam->m_fUseTemplet )
						{
							m_rgTrackInfo[i].m_fCanOutput = true;
						}
						else
						{
							m_rgTrackInfo[i].m_fCanOutput = true;
							RTN_HR_IF_FAILED(FireCarLeftEvent(m_rgTrackInfo[i], pParam, pProcessData, pProcessRespond));

						}
					}			
				}
				//�շ�վ����ģʽ��
				else if(GetCurrentParam()->g_PlateRcogMode == PRM_TOLLGATE)
				{
					// ��������
					bool fFilterReverseRun = m_cfgParam.fFilterReverseEnable
							&& m_rgTrackInfo[i].m_cPlateInfo > 1
							&& (m_pParam->g_MovingDirection == MD_TOP2BOTTOM
									&& m_rgTrackInfo[i].LastInfo().rcPos.bottom < m_rgTrackInfo[0].LastInfo().rcPos.CenterPoint().y
							   || m_pParam->g_MovingDirection == MD_BOTTOM2TOP
									&& m_rgTrackInfo[i].LastInfo().rcPos.bottom > m_rgTrackInfo[0].LastInfo().rcPos.CenterPoint().y);

					// ���˼�����������
					bool fFilterMultiDet = false;
					if (m_pParam->g_MovingDirection == MD_TOP2BOTTOM && m_rgTrackInfo[i].m_State == tsEnd)
					{
					    // ���߶�
						int nDetH = (m_cfgParam.cTrapArea.BottomLeftY + m_cfgParam.cTrapArea.BottomRightY
								- m_cfgParam.cTrapArea.TopLeftY - m_cfgParam.cTrapArea.TopRightY) / 2
								* m_nHeight / 100;
						// ���һ֡�߶�
						int nMoveH = m_rgTrackInfo[i].LastInfo().rcPos.CenterPoint().y - m_cfgParam.cTrapArea.TopLeftY * m_nHeight / 100;

						if (nMoveH < (nDetH >> 2))  // û�ƶ���1/4���߶ȵ�ȥ��
						{
							fFilterMultiDet = true;
						}
					}
					if((m_rgTrackInfo[i].m_rgbVotedResult[6] == 37 &&
						fltFirstCharConfidence * 100 < m_iFirstConfidenceQuan)
						||
						(!m_rgTrackInfo[i].m_fCarArrived && m_rgTrackInfo[i].m_nObservedFrames > 10
							&& m_rgTrackInfo[i].m_iVoteSimilarityNum * 100 < 25 * m_rgTrackInfo[i].m_nObservedFrames)
							|| fFilterReverseRun
							|| fFilterMultiDet)

					{		
						fDeleteObjTrack = true;
					}
					else
					{
						if (m_pLastResult && m_pLastResult->IsTwinResult(m_rgTrackInfo[i]))
						{
							//������ͬ����ʱ����Ӧ����Ƶ������Ҳȥ��.
							RTN_HR_IF_FAILED(m_pLastResult->UpdateResult(m_rgTrackInfo[i]));
							fDeleteObjTrack = true;
						}
						else
						{
							if( m_pParam->m_fUseTemplet )
							{
								m_rgTrackInfo[i].m_fCanOutput = true;
							}
							else
							{
								m_rgTrackInfo[i].m_fCanOutput = true;
								RTN_HR_IF_FAILED(FireCarLeftEvent(m_rgTrackInfo[i], pParam, pProcessData, pProcessRespond));
							}
						}
					}
					
				}
			}
			else
			{
				//�շ�վģʽ�£�����ͬʱ�׺���ƽ�����Ŷ�С������ֵ
				if(GetCurrentParam()->g_PlateRcogMode == PRM_TOLLGATE && 
					m_rgTrackInfo[i].m_rgbVotedResult[6] == 37 &&
					fltFirstCharConfidence * 100 < m_iFirstConfidenceQuan)
				{		
					fDeleteObjTrack = true;
				}
				else
				{
					/*
					//����������ǿ�Ƴ�
					if(!m_pParam->m_fUseTemplet 
						&& m_rgTrackInfo[i].m_nVotedColor == PC_WHITE)
					{
						if (m_pLastResult && m_pLastResult->IsTwinResult(m_rgTrackInfo[i]))
						{
							//������ͬ����ʱ����Ӧ����Ƶ������Ҳȥ��.
							RTN_HR_IF_FAILED(m_pLastResult->UpdateResult(m_rgTrackInfo[i]));
							fDeleteObjTrack = true;
						}
						else
						{
							m_rgTrackInfo[i].m_fCanOutput = true;
							RTN_HR_IF_FAILED(FireCarLeftEvent(m_rgTrackInfo[i], pParam, pProcessData, pProcessRespond));

						}
					}
					*/
				}
			}
			if (m_pLastResult)
			{
				// ����ҪClear��Ŀǰ��LastResult��ֻ����Header
				//m_pLastResult->Clear();
			}
			if(m_rgTrackInfo[i].m_fCanOutput == true)
			{
				m_pLastResult=&m_LastResult;
				m_pLastResult->Create(m_rgTrackInfo[i], m_pParam);

				//��ʼ��������Ϣ�ṹ����
				if(!m_fLastResultInited)
				{
					//��ʼ������������ų�����Ϣ������
					m_pLastResult->m_iTimeinCount = 0;
					if(m_pLastResult->m_iBlockTwinsTimeout > 0)
					{
						for(int iDetect=0; iDetect<m_iBlockTwinsTimeout*2; iDetect++)
						{
							m_pLastResult->m_detectsaver[iDetect].dw_TrackEndTime = 0;
							HV_memset(m_pLastResult->m_detectsaver[iDetect].rgContent,0,sizeof(m_pLastResult->m_detectsaver[iDetect].rgContent));
						}
					}
					else
					{
						m_pLastResult->m_detectsaver[0].dw_TrackEndTime = 0;
						HV_memset(m_pLastResult->m_detectsaver[0].rgContent,0,sizeof(m_pLastResult->m_detectsaver[0].rgContent));
					}

					//�õ���һ��������Ϣ�ļ�¼
					DWORD32 dw_Now = m_rgTrackInfo[i].m_dwBestSnapShotRefTime;
					if(dw_Now != 0)
					{
						m_pLastResult->m_detectsaver[0].dw_TrackEndTime = dw_Now;
					}
					else
					{
						m_pLastResult->m_detectsaver[0].dw_TrackEndTime = GetSystemTick();
					}

					for(int iDet=0; iDet<8; iDet++)
					{
						m_pLastResult->m_detectsaver[0].rgContent[iDet] = m_rgTrackInfo[i].m_rgbVotedResult[iDet];//m_rgTrackInfo[i].m_rgPlateInfo[0].rgbContent[iDet];
					}

					m_pLastResult->m_iTimeinCount ++;
					m_fLastResultInited = true;
					HV_Trace(1, "\nFinalResultParam_Init...\n");
				}
			}
		}

		if (m_rgTrackInfo[i].m_fCarArrived)
		{
			for (int j = 0; j < m_cObjTrackInfo; j++)
			{
				if (!m_rgObjTrackInfo[j].m_fCarArrived &&
					m_rgObjTrackInfo[j].m_objTrackState != OBJ_TRACK_INIT &&
					m_rgObjTrackInfo[j].MatchPlateTrackID(m_rgTrackInfo[i].m_nID))
				{
					m_rgObjTrackInfo[j].m_fCarArrived = true;
					m_rgObjTrackInfo[j].m_nCarArriveTime = m_rgTrackInfo[i].m_nCarArriveTime;
					m_rgObjTrackInfo[j].m_nCarArriveRealTime = m_rgTrackInfo[i].m_nCarArriveRealTime;
				}
			}
		}
		if (m_rgTrackInfo[i].m_fCanOutput || fDeleteObjTrack)
		{
			for (int j = 0; j < m_cObjTrackInfo; j++)
			{
				if (!m_rgObjTrackInfo[j].m_fVoted &&
					m_rgObjTrackInfo[j].m_objTrackState != OBJ_TRACK_INIT &&
					m_rgObjTrackInfo[j].MatchPlateTrackID(m_rgTrackInfo[i].m_nID))
				{
					m_rgObjTrackInfo[j].m_fVoted = true;
					//���ӵ羯����������������ٵ�ͼƬ��Ϣ,�ƹ�������,2011-08-14
					if(GetCurrentParam()->m_fUsedTrafficLight && m_cfgParam.fPreVote)
						m_rgObjTrackInfo[j].ClearImage(pProcessRespond);
				}
			}
			if (fDeleteObjTrack && GetCurrentParam()->g_PlateRcogMode == PRM_TOLLGATE)
			{
				// �շ�վģʽ�½��������������Ƴ��������Ӧ�����Ƴ��Ĺ���
				for (int j = 0; j < m_cObjTrackInfo; j++)
				{
					if (m_rgObjTrackInfo[j].m_cPlateTrackID == 1 &&
						m_rgObjTrackInfo[j].MatchPlateTrackID(m_rgTrackInfo[i].m_nID))
					{
						m_rgObjTrackInfo[j].m_cPlateTrackID = 0;
					}
				}
			}
		}

		if (m_rgTrackInfo[i].m_State == tsEnd)
		{
			if (!m_pParam->m_fUseTemplet)
			{
				m_rgTrackInfo[i].m_State = tsWaitRemove;
			}
			else if(m_pParam->m_fUseTemplet)
			{
				// ʹ��ģ��ƥ��(Ŀǰֻ�ں���ɫ�����)
				if (m_rgTrackInfo[i].m_fCanOutput)
				{
					if (m_rgTrackInfo[i].m_fhasTemple)
					{
						// �Ѿ�������ģ�壬�����ģ�����״̬
						m_rgTrackInfo[i].m_State = tsTemTrack;			// �����Ѿ����Ƶģ�����ģ��ƥ�����״̬
						m_rgTrackInfo[i].m_fTempCloseEdge = false;		// ģ���Ƿ񿿽���Ե
						m_rgTrackInfo[i].m_nCloseEdgeCount = 0;			// ģ�濿����Ե֡������
					}
					else
					{
						// û��ģ�壬���������
						RTN_HR_IF_FAILED(FireCarLeftEvent(m_rgTrackInfo[i], pParam, pProcessData, pProcessRespond));
						m_rgTrackInfo[i].m_State = tsWaitRemove;
					}
				}
				else
				{
					m_rgTrackInfo[i].m_State = tsWaitRemove;
				}
			}
		}
		else if (m_rgTrackInfo[i].m_State == tsTemTrackEnd)
		{
				if (m_rgTrackInfo[i].m_fCanOutput)
				{
					RTN_HR_IF_FAILED(FireCarLeftEvent(m_rgTrackInfo[i], pParam, pProcessData, pProcessRespond));
				}
				m_rgTrackInfo[i].m_State = tsWaitRemove;
		}
	}

	return S_OK;
}

HRESULT CTrackerImpl::FireCarLeftEvent(
	CTrackInfo& TrackInfo, 
	PROCESS_ONE_FRAME_PARAM* pParam, 
	PROCESS_ONE_FRAME_DATA* pProcessData, 
	PROCESS_ONE_FRAME_RESPOND* pProcessRespond
	)
{
	//��Ҫ�����жϳ��Ƶĸ����Ƿ�������ĸ���������ֱ�ӷ���false
	if(pProcessRespond->cTrigEvent.iCarLeftCount >= MAX_EVENT_COUNT)
		return S_FALSE;

	// ����������Ʊ�־
	TrackInfo.m_fCarLefted = true;

	m_dwLastCarLeftTime = GetSystemTick();
	if(pParam->fIsCheckLightType)
	{
	    CheckLightType((int)TrackInfo.AveragePlateBrightness(), false);
	}
	//��̬������СVariance���羯��Ϊʶ���ʵ�, �����ö�̬��ʽ
	if (m_pParam->g_PlateRecogSpeed == PR_SPEED_NORMAL 
		|| m_LightType == NIGHT
		|| GetCurrentParam()->g_PlateRcogMode == PRM_ELECTRONIC_POLICE)
	{
		m_pParam->g_nDetMinStdVar = m_iMinVariance;
	}
	else
	{
		int iCurrentStdVar = TrackInfo.LastInfo().nVariance - 40;
		if (iCurrentStdVar < 1)
		{
			iCurrentStdVar = 1;
		}
		m_pParam->g_nDetMinStdVar = (int)(m_pParam->g_nDetMinStdVar * 0.95 +
			iCurrentStdVar * iCurrentStdVar * 0.05);
	}
	PROCESS_IMAGE_CORE_RESULT* pCurResult = &(pProcessRespond->cTrigEvent.rgCarLeftCoreInfo[pProcessRespond->cTrigEvent.iCarLeftCount]);
	//zhaopy
	pCurResult->dwLastSnapShotRefTime = TrackInfo.m_dwLastSnapShotRefTime;
	pCurResult->dwBestSnapShotRefTime = TrackInfo.m_dwBestSnapShotRefTime;
	pCurResult->dwBeginCaptureRefTime = TrackInfo.m_dwBeginCaptureRefTime;
	pCurResult->dwBestCaptureRefTime = TrackInfo.m_dwBestCaptureRefTime;
	pCurResult->dwLastCaptureRefTime = TrackInfo.m_dwLastCaptureRefTime;

	pCurResult->dwTriggerTime = 0;
	if( TrackInfo.m_fHasTrigger && TrackInfo.m_dwTriggerTimeMs != 0 )
	{
		pCurResult->dwTriggerTime = TrackInfo.m_dwTriggerTimeMs;
	}

	if( m_fEnableCarArriveTrigger && TrackInfo.m_fIsTrigger && TrackInfo.m_nCarArriveRealTime != 0 )
	{
		pCurResult->dwTriggerTime = TrackInfo.m_nCarArriveRealTime;
	}
	//�ƹ����޸�,2011-08-06
	if(!TrackInfo.m_pimgBestSnapShot)
	{
		TrackInfo.m_pimgBestSnapShot = TrackInfo.m_pimgBeginCapture;
		if(TrackInfo.m_pimgBestSnapShot)
			ImageAddRef(&pProcessRespond->cImgMemOperLog, TrackInfo.m_pimgBestSnapShot);
		TrackInfo.m_rcBestPos = TrackInfo.m_rgRect[2];
		TrackInfo.m_rgRect[0] = TrackInfo.m_rgRect[2];
		pCurResult->dwBestSnapShotRefTime = TrackInfo.m_dwBeginCaptureRefTime;
	}
	if(!TrackInfo.m_pimgLastSnapShot)
	{
		TrackInfo.m_pimgLastSnapShot = TrackInfo.m_pimgBestCapture;
		if(TrackInfo.m_pimgLastSnapShot)
			ImageAddRef(&pProcessRespond->cImgMemOperLog, TrackInfo.m_pimgLastSnapShot);		
		TrackInfo.m_rcLastPos = TrackInfo.m_rgRect[3];
		TrackInfo.m_rgRect[1] = TrackInfo.m_rgRect[3];
		pCurResult->dwLastSnapShotRefTime = TrackInfo.m_dwBestCaptureRefTime;
	}

	//���Ӻ����Ϣ
	if(m_nRedLightCount < 20)
	{
		pCurResult->rcRedLightCount = m_nRedLightCount;
		for(int i = 0; i < m_nRedLightCount; i++)
		{
			pCurResult->rcRedLightPos[i].left   = m_rgRedLightRect[i].left;
			pCurResult->rcRedLightPos[i].top    = m_rgRedLightRect[i].top * 2;
			pCurResult->rcRedLightPos[i].right  = m_rgRedLightRect[i].right;
			pCurResult->rcRedLightPos[i].bottom = m_rgRedLightRect[i].bottom * 2;
		}
	}

	RESULT_IMAGE_STRUCT* pResultImage = &(pCurResult->cResultImg);
	pResultImage->pimgBestSnapShot = TrackInfo.m_pimgBestSnapShot;
	pResultImage->pimgLastSnapShot = TrackInfo.m_pimgLastSnapShot;

	// zhaopy 
	if(GetCurrentParam()->g_PlateRcogMode == PRM_ELECTRONIC_POLICE)
	{
		pResultImage->pimgBeginCapture = TrackInfo.m_pimgBeginCapture;
		pResultImage->pimgBestCapture = TrackInfo.m_pimgBestCapture;
		pResultImage->pimgLastCapture = TrackInfo.m_pimgLastCapture;
	}
	else
	{
		pResultImage->pimgBeginCapture = NULL;
		pResultImage->pimgBestCapture = NULL;
		pResultImage->pimgLastCapture = NULL;
	}

	IReferenceComponentImage* pimgPlate = TrackInfo.m_rgBestPlateInfo[TrackInfo.m_nVotedType].pimgPlate;	// Сͼ
	bool fUseDB = false;

	if ((pimgPlate == NULL) && (PLATE_DOUBLE_MOTO == TrackInfo.m_nVotedType))
	{
		fUseDB = true;				// ʹ��˫�ƴ���Ħ��
		pimgPlate = TrackInfo.m_rgBestPlateInfo[PLATE_DOUBLE_YELLOW].pimgPlate;	// Сͼ
	}

	if (pimgPlate != NULL)
	{
		// CopyСͼ�������ڴ�
		HV_COMPONENT_IMAGE imgPlate;

		if (FAILED(pimgPlate->GetImage(&imgPlate)) ||
			imgPlate.iWidth == 0 ||
			imgPlate.iHeight == 0 )
		{
			pimgPlate = NULL;
		}
		else
		{
			// �ж�Сͼ�������Ƿ��㹻
			bool fIsOk = true;
			int iImageBufferSize = pProcessData->rghvImageSmall[pProcessRespond->cTrigEvent.iCarLeftCount].iWidth * pProcessData->rghvImageSmall[pProcessRespond->cTrigEvent.iCarLeftCount].iHeight;

			if(imgPlate.iWidth * imgPlate.iHeight * 2 > iImageBufferSize)
			{
				fIsOk = false;
			}

			if( fIsOk )
			{
				CPersistentComponentImage imgTemp;
				if(SUCCEEDED(imgTemp.Create(HV_IMAGE_YUV_422, imgPlate.iWidth, 2*imgPlate.iHeight)))
				{
					// ʹ�ô˽ӿ���ЩСͼ����С��������ȷ���Ƿ��Ǵ��㷨�����⡣�ָĳ�ֱ�������еĿ�����
					//PlateResize(imgPlate, imgTemp);
					for (int i = 0; i < 3; i++)
					{
						PBYTE8 pSrc = GetHvImageData(&imgPlate, i);
						PBYTE8 pDes = GetHvImageData(&imgTemp, i);

						if (pSrc && pDes)
						{
							if( 0 == i )
							{
								for( int j = 0; j < imgPlate.iHeight; ++j )
								{
									memcpy(pDes, pSrc, imgPlate.iWidth);
									pDes += imgTemp.iStrideWidth[i];
									memcpy(pDes, pSrc, imgPlate.iWidth);
									pDes += imgTemp.iStrideWidth[i];
									pSrc += imgPlate.iStrideWidth[i];
								}
							}
							else
							{
								int iCopySize = imgPlate.iWidth / 2;
								for( int j = 0; j < imgPlate.iHeight; ++j )
								{
									memcpy(pDes, pSrc, iCopySize);
									pDes += imgTemp.iStrideWidth[i];
									memcpy(pDes, pSrc, iCopySize);
									pDes += imgTemp.iStrideWidth[i];
									pSrc += imgPlate.iStrideWidth[i];
								}
							}

						}
					}

					imgPlate = imgTemp;
				}
				for (int i = 0; i < 3; i++)
				{
					PBYTE8 pSrc = GetHvImageData(&imgPlate, i);
					PBYTE8 pDes = GetHvImageData(&pProcessData->rghvImageSmall[pProcessRespond->cTrigEvent.iCarLeftCount], i);
					if (pSrc && pDes)
					{
						memcpy(pDes, pSrc, imgPlate.iStrideWidth[i] * imgPlate.iHeight);
					}
				}

				pCurResult->nPlateWidth = imgPlate.iWidth;
				pCurResult->nPlateHeight = imgPlate.iHeight;
			}
			else
			{
				pimgPlate = NULL;
			}
		}
	}

	if (pimgPlate != NULL)
	{
		HV_COMPONENT_IMAGE imgPlate;

		if (pimgPlate->GetImage(&imgPlate) == S_OK
			&& GetHvImageData(&imgPlate, 0) != NULL)
		{
			BYTE8 rgbResizeImg[BINARY_IMAGE_WIDTH * BINARY_IMAGE_HEIGHT];
			HiVideo::CRect *prcPlatePos;
			/*prcPlatePos = &TrackInfo.m_rgBestPlateInfo[TrackInfo.m_nVotedType].rcPlatePos;
			if (fUseDB)
			{
				prcPlatePos = &TrackInfo.m_rgBestPlateInfo[PLATE_DOUBLE_YELLOW].rcPlatePos;
			}*/

			int iWidth = BINARY_IMAGE_WIDTH;
			int iHeight = BINARY_IMAGE_HEIGHT;
			int iStride = BINARY_IMAGE_WIDTH;

			// ֻ������Ĳ��ֶ�ֵ��ͼ
			ScaleGrey(rgbResizeImg, iWidth, iHeight, iStride,
				GetHvImageData(&imgPlate, 0),
				imgPlate.iWidth,
				imgPlate.iHeight,
				imgPlate.iStrideWidth[0]
				);

			// �����ֵ����ֵ
			HiVideo::CRect rcArea(0, 0, iWidth, iHeight);
			int iThre = CalcBinaryThreByCenter(
				rgbResizeImg,
				iWidth,
				iHeight,
				iStride,
				&rcArea);

			// ���ݳ�����ɫ���ö�ֵ������
			BOOL fIsWhiteChar = FALSE;
			if ((TrackInfo.m_nVotedColor == PC_BLUE) ||
				(TrackInfo.m_nVotedColor == PC_BLACK)	||
				(TrackInfo.m_nVotedColor == PC_GREEN))
			{
				fIsWhiteChar = TRUE;
			}
			GrayToBinByThre(
				fIsWhiteChar,
				rgbResizeImg,
				iWidth,
				iHeight,
				iStride,
				iThre,
				GetHvImageData(&pProcessData->rghvImageBin[pProcessRespond->cTrigEvent.iCarLeftCount], 0)
				);
		}
	}

	//���㳵��
	float fltCarSpeed(0.0f);
	float fltScaleOfDistance(1.0f);
	CalcCarSpeed(fltCarSpeed, fltScaleOfDistance, TrackInfo);

	pCurResult->nDetectCrossLineEnable = m_pParam->m_iDetectCrossLineEnable;
	pCurResult->nDetectOverYellowLineEnable = m_pParam->m_iDetectOverYellowLineEnable;

	// ��������
	pCurResult->nCarType = TrackInfo.m_nVotedCarType;			// �������

	// �����ߴ�
	if (m_fOutputCarSize == TRUE)
	{
		pCurResult->cCarSize.iCarWidth = TrackInfo.m_stCarInfo.fltCarW / 100;
		pCurResult->cCarSize.iCarHeight = TrackInfo.m_stCarInfo.fltCarH / 100;

		if (m_fEnableScaleSpeed)
		{
			// ���б궨
			pCurResult->cCarSize.nOutType = 1;		// ���(��)
			if ((pCurResult->cCarSize.iCarWidth < 1.4f) 
				|| (pCurResult->cCarSize.iCarHeight < 2.7f))
			{
				float fltTempS = 1.0f;
				float fltRand = (float)(rand() % 51) / 150;
				fltTempS += fltRand;

				if (TrackInfo.m_nVotedCarType == CT_LARGE)
				{
					fltTempS *= 1.75f;
				}
				else if (TrackInfo.m_nVotedCarType == CT_MID)
				{
					fltTempS *= 1.20f;
				}

				pCurResult->cCarSize.iCarWidth = 1.5f * fltTempS;
				pCurResult->cCarSize.iCarHeight = 3.4f * fltTempS;

			}

		}
		else
		{
			pCurResult->cCarSize.nOutType = 0;			// ���(����)
			// �ޱ궨
			if ((pCurResult->cCarSize.iCarWidth < m_nCarWBottom) 
				|| (pCurResult->cCarSize.iCarHeight < m_nCarLenBottom))
			{
				float fltTempS = 1.1f;
				float fltRand = (float)(rand() % 11) / 100;
				fltTempS += fltRand;

				if (TrackInfo.m_nVotedCarType == CT_LARGE)
				{
					fltTempS *= 1.75f;
				}
				else if (TrackInfo.m_nVotedCarType == CT_MID)
				{
					fltTempS *= 1.2f;
				}

				pCurResult->cCarSize.iCarWidth = m_nCarWBottom * fltTempS;
				pCurResult->cCarSize.iCarHeight = m_nCarLenBottom * fltTempS;
			}
		}
	}
	else
	{//��Ϊarm���ݽ����Ƿ���������ߴ�ı�ǻᱻDSP�޸ģ�ͨ���������Ϊ3����֤�Ƿ���������ߴ�
		pCurResult->cCarSize.nOutType = 2; 
	}

	// ����ṹ��ֵ
	memcpy(pCurResult->rgbContent, TrackInfo.m_rgbVotedResult, 8);
	pCurResult->nType = TrackInfo.m_nVotedType;
	pCurResult->nColor = TrackInfo.m_nVotedColor;

	pCurResult->fltAverageConfidence = TrackInfo.AverageConfidence();
	pCurResult->fltFirstAverageConfidence = TrackInfo.AverageFirstConfidence();
	pCurResult->nStartFrameNo = TrackInfo.m_nStartFrameNo;
	pCurResult->nEndFrameNo = TrackInfo.m_nEndFrameNo;
	pCurResult->nFirstFrameTime = TrackInfo.m_dwFirstFlameTime;
	pCurResult->iObservedFrames = TrackInfo.m_nObservedFrames;
	pCurResult->iVotedObservedFrames = 0;
	pCurResult->nCarColor = TrackInfo.m_nVotedCarColor;
	pCurResult->fIsNight = (m_LightType == NIGHT) ? true : false;
//	pCurResult->fOutputCarColor = CTrackInfo::m_fEnableRecgCarColor;
	for( int i = 0; i < TrackInfo.m_cPlateInfo; ++i )
	{
		if( TrackInfo.m_rgPlateInfo[i].nInfoType == PI_LPR 
			&& TrackInfo.m_rgPlateInfo[i].nPlateType == TrackInfo.m_nVotedType )
		{
			pCurResult->iVotedObservedFrames++;
		}
	}
	
	if(m_pParam->m_iDetectReverseEnable)
	{
		pCurResult->fReverseRun = TrackInfo.m_fReverseRunVoted;
	}
	else
	{
		pCurResult->fReverseRun = false;
	}

	//�����ж���ʻ���򣬼�⳵���Ƿ��복���趨����һ��
	switch ( m_TravelDirection )
	{
	case MD_TOP2BOTTOM:
		if (TrackInfo.LastInfo().rcPos.top - 
			TrackInfo.m_rgPlateInfo[0].rcPos.top < -(m_nHeight * m_iSpan * 0.01f))
		{
			pCurResult->fReverseRun = true;
		}
		break;
	case MD_BOTTOM2TOP:
		if (TrackInfo.LastInfo().rcPos.top -
			TrackInfo.m_rgPlateInfo[0].rcPos.top
			> m_nHeight * m_iSpan * 0.01f)
		{
			pCurResult->fReverseRun = true;
		}
		break;
	case MD_LEFT2RIGHT:
		if (TrackInfo.LastInfo().rcPos.left - 
			TrackInfo.m_rgPlateInfo[0].rcPos.left < -(m_nWidth * m_iSpan * 0.01f))
		{
			pCurResult->fReverseRun = true;
		}
		break;
	case MD_RIGHT2LEFT:
		if (TrackInfo.LastInfo().rcPos.left -
			TrackInfo.m_rgPlateInfo[0].rcPos.left
			> m_nWidth * m_iSpan * 0.01f)
		{
			pCurResult->fReverseRun = true;
		}
		break;
	case MD_NOTSURE:
	default:
	}

	//pCurResult->fTravelReverse =TrackInfo.m_fTravelReverse;
	
	pCurResult->nVoteCondition = TrackInfo.m_nVoteCondition;
	pCurResult->iAvgY = m_iAvgY;
	pCurResult->iCarAvgY = (int)TrackInfo.AveragePlateBrightness();
	pCurResult->iCarVariance = (int)TrackInfo.AverageStdVar();

	pCurResult->nRoadNo = TrackInfo.m_iRoad;			// ��Ƶ����������������Ϊ0(�������,��Ϊ-1)
	//pCurResult->iRoadNumberBegin =  CTrackInfo::m_iRoadNumberBegin;
	//pCurResult->iStartRoadNum = CTrackInfo::m_iStartRoadNum;
	
	pCurResult->nCarLeftCount = 0;
	pCurResult->fltCarspeed = fltCarSpeed;
	pCurResult->fltScaleOfDistance = fltScaleOfDistance;
	pCurResult->nPlateLightType = m_PlateLightType;
	pCurResult->iCplStatus = m_iCplStatus;
	pCurResult->iPulseLevel = m_iPulseLevel;
	if(pCurResult->fIsNight && pCurResult->iPulseLevel < 1)
	{
		pCurResult->iPulseLevel = 1;
	}
	
	if(pCurResult->iObservedFrames < 6 && pCurResult->fltCarspeed > 200.0f)
	{
		pCurResult->fltCarspeed = 0.0f;
	}

	if(m_iForceLightOffAtDay)
	{
		if(m_LightType == DAY && m_iPulseLevel > 0 
			&& m_iForceLightOffFlag == 1)
		{
			m_iPulseLevel = 0;
		}
	}
	if( TrackInfo.m_dwTriggerTime == 0 )
	{
		pCurResult->nCarArriveTime = TrackInfo.m_nCarArriveTime;
	}
	else
	{
		pCurResult->nCarArriveTime = TrackInfo.m_dwTriggerTime;
	}


	pCurResult->fCarIsStop = TrackInfo.m_fCarIsStop;
	pCurResult->iCarStopPassTime = TrackInfo.m_iCarStopPassTime;

	//�¼����
	pCurResult->coltIsOverYellowLine	= COLT_NO;
	pCurResult->coltIsCrossLine		= COLT_NO;
	//ѹ����
	pCurResult->coltIsOverYellowLine = IsOverYellowLine(TrackInfo);
	//�ж���ѹʵ�߻��ǻ���
	if(CTrackInfo::m_ActionDetectParam.iIsYellowLine[pCurResult->coltIsOverYellowLine] == 2)
	{
		pCurResult->fIsDoubleYellowLine = true;
	}
	else
	{
		pCurResult->fIsDoubleYellowLine = false;
	}
	//Խ��
	pCurResult->coltIsCrossLine = IsCrossLine(TrackInfo);

	pCurResult->iUnSurePeccancy = 0;

	if( m_pParam->m_fUsedTrafficLight )
	{
		//���̵Ƴ����ŵļ���ʹ�õ�һ��ץ�Ĵ�ͼ��λ��
		pCurResult->nRoadNo = MatchRoad(TrackInfo.m_rgPlateInfo[0].rcPos.CenterPoint().x, TrackInfo.m_rgPlateInfo[0].rcPos.CenterPoint().y);
		
		//��ʻ����
		RUN_TYPE rt = RT_UNSURE;
		//���������
		if( pCurResult->fReverseRun )
		{
			rt = RT_CONVERSE;
		}
		else
		{
			rt = CheckRunType(TrackInfo.m_rgRect[2], TrackInfo.m_rectLastImage);
		}
		
		//���˳�����ص���ʻ����
		int iFilter = CTrackInfo::m_roadInfo[pCurResult->nRoadNo].iFilterRunType;
		if( (iFilter & RRT_FORWARD) && rt == RT_FORWARD )
		{
			rt = RT_UNSURE;
		}
		else if( (iFilter & RRT_LEFT) && rt == RT_LEFT )
		{
			rt = RT_UNSURE;
		}
		else if( (iFilter & RRT_RIGHT) && rt == RT_RIGHT )
		{
			rt = RT_UNSURE;
		}
		else if( (iFilter & RRT_TURN) && rt == RT_TURN )
		{
			rt = RT_UNSURE;
		}

		pCurResult->rtType = rt;
		int iOnePosLightScene = TrackInfo.m_iOnePosLightScene;
		int iTwoPosLightScene = TrackInfo.m_iPassStopLightScene;

		if( CTrackInfo::m_roadInfo[pCurResult->nRoadNo].iRoadType & RRT_LEFT )
		{
			iTwoPosLightScene = TrackInfo.m_iPassLeftStopLightScene;
		}
		//�����"���˵�����ץ��ͼΪ�̵Ƶ�Υ��",���õ�����ץ���ߵƵ�״̬����Ϊ�жϵ�����
		//Τ�����޸�,2010-12-22
		if(m_iFilterRushPeccancy && iTwoPosLightScene != -1)
		{
			iTwoPosLightScene = TrackInfo.m_iThreePosLightScene;
		}
		//GetLightScene(iOnePosLightScene, &(pCurResult->tsOnePosScene));
		//GetLightScene(iTwoPosLightScene, &(pCurResult->tsTwoPosScene));
		m_cTrafficLight.GetLightScene(iOnePosLightScene, &pCurResult->tsOnePosScene);
		m_cTrafficLight.GetLightScene(iTwoPosLightScene, &pCurResult->tsTwoPosScene);

		int pt;
		int iRoadType = (RRT_FORWARD | RRT_LEFT | RRT_RIGHT | RRT_TURN);
		if( pCurResult->nRoadNo != -1 )
		{
			iRoadType = CTrackInfo::m_roadInfo[pCurResult->nRoadNo].iRoadType;
		}
		//ֻ�����Ӧ�����ĺ�ƿ�ʼʱ��
		//pCurResult->lrtRedStart = m_redrealtime;
		m_cTrafficLight.GetRedLightStartTime(&pCurResult->lrtRedStart);
		if( (iRoadType & RRT_LEFT) == 0 ) 
		{
			pCurResult->lrtRedStart.dwLeftL = pCurResult->lrtRedStart.dwLeftH = 0;
		}
		if( (iRoadType & RRT_FORWARD) == 0 )
		{
			pCurResult->lrtRedStart.dwForwardL = pCurResult->lrtRedStart.dwForwardH = 0;
		}
		if( (iRoadType & RRT_RIGHT) == 0 ) 
		{
			pCurResult->lrtRedStart.dwRightL = pCurResult->lrtRedStart.dwRightH = 0;
		}
		if( (iRoadType & RRT_TURN) == 0 ) 
		{
			pCurResult->lrtRedStart.dwTurnL = pCurResult->lrtRedStart.dwTurnH = 0;
		}
		//����л������δ֪�����ڵĳ���ֻ��һ�ַ���ģ����������ʻ����Ϊ������򵡣
		if( pCurResult->rtType == RT_UNSURE && CTrackInfo::m_iRunTypeEnable == 1 )
		{
			if( iRoadType == RRT_LEFT )
			{
				pCurResult->rtType = RT_LEFT;
			}
			else if( iRoadType == RRT_FORWARD )
			{
				pCurResult->rtType = RT_FORWARD;
			}
			else if( iRoadType == RRT_RIGHT )
			{
				pCurResult->rtType = RT_RIGHT;
			}
			else if( iRoadType == RRT_TURN )
			{
				pCurResult->rtType = RT_TURN;
			}
		}
	
		bool fOverLine = false;
		if( (pCurResult->coltIsOverYellowLine != COLT_INVALID &&  pCurResult->coltIsOverYellowLine != COLT_NO)
			|| (pCurResult->coltIsCrossLine != COLT_INVALID && pCurResult->coltIsCrossLine != COLT_NO) )
		{
			fOverLine = true;
		}

		DWORD32 dwCaptureTime = (pResultImage->pimgBestCapture == NULL ? GetSystemTick() : TrackInfo.m_dwBestCaptureRefTime);

		if( S_OK != CheckPeccancyType(iOnePosLightScene, iTwoPosLightScene, rt, iRoadType, dwCaptureTime, &pt) )
		{
			pt = PT_NORMAL;
		}
		if(m_iFilterRushPeccancy  && (pt & PT_RUSH) != PT_RUSH)
		{
			int ptTmp;
			if( S_OK == CheckPeccancyType(iOnePosLightScene, TrackInfo.m_iPassStopLightScene, rt, iRoadType, dwCaptureTime, &ptTmp) )
			{
				if((ptTmp & PT_RUSH) == PT_RUSH)
				{
					HV_Trace(1, "����ץ��ͼ�Ƶ�״̬��Ϊ���ִ�й���\n");
				}
			}
		}

		//����Ѿ��������������,���ҵ�����û����,�򴥷�������
		if( TrackInfo.m_fFirstTrigger && TrackInfo.m_fSecondTrigger	&& !TrackInfo.m_fThirdTrigger && pt != PT_NORMAL)
		{
			//int iPosThree = CTrackInfo::m_iCaptureThreePos * CTrackInfo::m_iHeight / 100;

			if(GetSystemTick() > (DWORD32)m_sMinTriggerTimeMs)
			{
				m_dwTriggerTimes++;
				if(m_dwTriggerTimes >= 0xffff)
				{
					m_dwTriggerTimes = 1;
				}
				
				DWORD32 dwRoadNum = (m_dwTriggerTimes << 16) | pCurResult->nRoadNo;
				//m_pCallback->ThirdTrigger(m_LightType,dwRoadNum, NULL);
				m_sMinTriggerTimeMs = GetSystemTick() + 300;
				m_dwLastTriggerInfo = dwRoadNum;
				//HV_Trace(1, "firecarleft:%d,%d, %08x\n", m_dwTriggerTimes, stCarLeftInfo.nVideoDetID, dwRoadNum);
			}
			else
			{
				DWORD32 dwRoadNum = (m_dwLastTriggerInfo & 0xffff0000) | pCurResult->nRoadNo;
				//HV_Trace(1, "ThirdTrigger Readd:%08x\n", dwRoadNum);
				//m_pCallback->ReaddSignal(dwRoadNum);
			}
			TrackInfo.m_fThirdTrigger = true;
			TrackInfo.m_dwThirdTime = m_dwTriggerTimes;
			TrackInfo.m_dwThirdTimeMs = GetSystemTick();
		}

		//�Ƿ�ѹ�ߣ����Ƴ���������ж�.�ƹ���ע�͵���ֻҪ��ѹ�߶�Ҫ����������Υ������,2010-10-12
		if( fOverLine )
		{
			pt |= PT_OVERLINE;
		}
		//�ƹ����޸ģ�ͨ����÷�ʽ��������Υ��,2010-10-12
		pCurResult->ptType |= pt;

	//	sprintf(g_szDebugInfo, "[FireCarLeft]rtType=%d, ptType=%d, pt=%d", pCurResult->rtType, pCurResult->ptType, pt);

		if( (pResultImage->pimgBeginCapture == NULL || pResultImage->pimgBestCapture == NULL || pResultImage->pimgLastCapture == NULL )
			|| TrackInfo.m_dwBeginCaptureRefTime == TrackInfo.m_dwBestCaptureRefTime
			|| TrackInfo.m_dwBeginCaptureRefTime == TrackInfo.m_dwLastCaptureRefTime
			|| TrackInfo.m_dwBestCaptureRefTime == TrackInfo.m_dwLastCaptureRefTime 
			)
		{
			//ͼƬ����Υ�¹涨
			pCurResult->iUnSurePeccancy = 1;
		}

		//���˵�����������Υ��
		if( (pCurResult->ptType & PT_CONVERSE) == PT_CONVERSE 
			&& ( (iFilter & RRT_CONVERSE) && rt == RT_CONVERSE) )
		{
			pCurResult->ptType &= ~PT_CONVERSE;
		}
		
		//ץ�Ĳ���ץ��ʱ��
		pCurResult->dwFirstTime = TrackInfo.m_dwFirstTime;
		pCurResult->dwSecondTime = TrackInfo.m_dwSecondTime;
		pCurResult->dwThirdTime = TrackInfo.m_dwThirdTime;
//		if(CTrackInfo::m_iFlashlightMode == 1 && m_iAvgY < CTrackInfo::m_iFlashlightThreshold)
//		{
//			HV_Trace("Tracker:s=%d:f=%d:t=%d\n", stCarLeftInfo.dwFirstTime, stCarLeftInfo.dwSecondTime, stCarLeftInfo.dwThirdTime);
//		}
	}
	else
	{
		pCurResult->rtType = RT_UNSURE;
		pCurResult->ptType = PT_NORMAL;
	}

	pCurResult->rcBestPlatePos = TrackInfo.m_rcBestPos;
	pCurResult->rcLastPlatePos = TrackInfo.m_rcLastPos;
	pCurResult->rcFirstPos = TrackInfo.m_rgRect[2];
	pCurResult->rcSecondPos = TrackInfo.m_rgRect[3];
	pCurResult->rcThirdPos = TrackInfo.m_rgRect[4];

	//У������
	pCurResult->rcBestPlatePos.left   = pCurResult->rcBestPlatePos.left  * 100 / m_nWidth;
	pCurResult->rcBestPlatePos.top    = pCurResult->rcBestPlatePos.top   * 100 / m_nHeight;
	pCurResult->rcBestPlatePos.right  = pCurResult->rcBestPlatePos.right  * 100 / m_nWidth;
	pCurResult->rcBestPlatePos.bottom = pCurResult->rcBestPlatePos.bottom * 100 / m_nHeight;

	pCurResult->rcLastPlatePos.left   = pCurResult->rcLastPlatePos.left  * 100 / m_nWidth;
	pCurResult->rcLastPlatePos.top    = pCurResult->rcLastPlatePos.top   * 100 / m_nHeight;
	pCurResult->rcLastPlatePos.right  = pCurResult->rcLastPlatePos.right  * 100 / m_nWidth;
	pCurResult->rcLastPlatePos.bottom = pCurResult->rcLastPlatePos.bottom * 100 / m_nHeight;

	pCurResult->rcFirstPos.left   = pCurResult->rcFirstPos.left  * 100 / m_nWidth;
	pCurResult->rcFirstPos.top    = pCurResult->rcFirstPos.top   * 100 / m_nHeight;
	pCurResult->rcFirstPos.right  = pCurResult->rcFirstPos.right  * 100 / m_nWidth;
	pCurResult->rcFirstPos.bottom = pCurResult->rcFirstPos.bottom * 100 / m_nHeight;

	pCurResult->rcSecondPos.left   = pCurResult->rcSecondPos.left  * 100 / m_nWidth;
	pCurResult->rcSecondPos.top    = pCurResult->rcSecondPos.top   * 100 / m_nHeight;
	pCurResult->rcSecondPos.right  = pCurResult->rcSecondPos.right  * 100 / m_nWidth;
	pCurResult->rcSecondPos.bottom = pCurResult->rcSecondPos.bottom * 100 / m_nHeight;

	pCurResult->rcThirdPos.left   = pCurResult->rcThirdPos.left  * 100 / m_nWidth;
	pCurResult->rcThirdPos.top    = pCurResult->rcThirdPos.top   * 100 / m_nHeight;
	pCurResult->rcThirdPos.right  = pCurResult->rcThirdPos.right  * 100 / m_nWidth;
	pCurResult->rcThirdPos.bottom = pCurResult->rcThirdPos.bottom * 100 / m_nHeight;
	//ץ�ı�־
	if (TrackInfo.m_fIsTrigger)
	{
		pCurResult->dwTriggerIndex = TrackInfo.m_dwTriggerIndex;
		pCurResult->iCapCount = TrackInfo.m_iCapCount;
		pCurResult->iCapSpeed = TrackInfo.m_iCapSpeed;
	}

	// ������Ŀ�޸ĳ�������
	if (m_iOutPutType == 1)
	{
		if ((pCurResult->nCarType == CT_SMALL)
			|| (pCurResult->nCarType == CT_MID)
			|| (pCurResult->nCarType == CT_LARGE))
		{
			pCurResult->nCarType = CT_VEHICLE;
		}
	}

//	RTN_HR_IF_FAILED( m_pCallback->CarLeft(
//		&stCarLeftInfo,
//		TrackInfo.m_pimgBestSnapShot->GetFrameName() ));

	pProcessRespond->cTrigEvent.dwEventId |= EVENT_CARLEFT;
	pProcessRespond->cTrigEvent.iCarLeftCount++;

	m_leftCar[pCurResult->nRoadNo].car_plate = (PLATE_COLOR)(pCurResult->nColor);
	m_leftCar[pCurResult->nRoadNo].frame_no = pParam->dwFrameNo;

	return S_OK;
}

HRESULT CTrackerImpl::ForceResult(int iFlag /*= 0 */)
{
/*	HV_Trace("\n*S*");

	CARARRIVE_INFO_STRUCT carArriveInfo;
	carArriveInfo.iPlateLightType = m_PlateLightType;
	carArriveInfo.dwIsOutTrig = 1;
	carArriveInfo.dwCarArriveTime = GetSystemTick();
	RTN_HR_IF_FAILED(m_pCallback->CarArrive(
		&carArriveInfo,
		NULL
		));

	int iBestPlate(-1);
	for (int i=0; i<m_cTrackInfo; i++)
	{
		if (m_rgTrackInfo[i].m_State == tsMoving ||
			m_rgTrackInfo[i].m_State == tsEnd)
		{
			RTN_HR_IF_FAILED(m_rgTrackInfo[i].Vote(
				m_rgTrackInfo[i].m_nVotedType,
				m_rgTrackInfo[i].m_nVotedColor,
				m_rgTrackInfo[i].m_rgbVotedResult));
			m_rgTrackInfo[i].m_fVoted = true;

			if (iBestPlate<0 ||
				m_rgTrackInfo[iBestPlate].AverageConfidence()<m_rgTrackInfo[i].AverageConfidence())
			{
				iBestPlate = i;
			}
		}
	}

	if (iBestPlate>=0)
	{
		if (!m_rgTrackInfo[iBestPlate].m_fCarArrived || m_rgTrackInfo[iBestPlate].m_fCarLefted)
		{
			CARARRIVE_INFO_STRUCT carArriveInfo;
			m_rgTrackInfo[iBestPlate].m_iRoad = MatchRoad(m_rgTrackInfo[iBestPlate].LastInfo().rcPos.CenterPoint().x, m_rgTrackInfo[iBestPlate].LastInfo().rcPos.CenterPoint().y);
			carArriveInfo.iRoadNumber = m_rgTrackInfo[iBestPlate].m_iRoad;
			carArriveInfo.iPlateLightType = m_PlateLightType;
			RTN_HR_IF_FAILED(m_pCallback->CarArrive(
				&carArriveInfo,
				NULL
				));
			m_rgTrackInfo[iBestPlate].m_fCarArrived = true;
			m_rgTrackInfo[iBestPlate].m_nCarArriveTime = GetSystemTick();
			m_rgTrackInfo[iBestPlate].m_rcCarArrivePos = m_rgTrackInfo[iBestPlate].LastInfo().rcPos;
		}
		RTN_HR_IF_FAILED(FireCarLeftEvent(m_rgTrackInfo[iBestPlate]));
	}
	return (iBestPlate>=0 ? S_OK : S_FALSE);*/
	return S_FALSE;
}


HRESULT CTrackerImpl::GetTrackCount(PDWORD32 pdwCount)
{
	if (pdwCount)
	{
		*pdwCount = m_cTrackInfo;
	}
	else
	{
		return E_POINTER;
	}

	return S_OK;
}

/*
HRESULT CTrackerImpl::GetPlatePos(
	DWORD32 dwIndex,
	DWORD32 *pdwLeft,
	DWORD32 *pdwTop,
	DWORD32 *pdwRight,
	DWORD32 *pdwBottom
)
{
	if ( dwIndex >= ( DWORD32 )m_cTrackInfo )
	{
		return E_FAIL;
	}

	if (m_rgTrackInfo[ dwIndex ].m_fObservationExists)
	{
		HiVideo::CRect& rect = m_rgTrackInfo[ dwIndex ].LastInfo().rcPos;
		*pdwLeft = rect.left;
		*pdwTop = rect.top;
		*pdwRight = rect.right;
		*pdwBottom = rect.bottom;
	}
	else
	{
		*pdwLeft = 0;
		*pdwTop = 0;
		*pdwRight = 0;
		*pdwBottom = 0;
	}

	return S_OK;
}
*/

HRESULT CTrackerImpl::GetPlatePos(
				  DWORD32 dwIndex,
				  CRect *prcPlate,
				  CRect *prcRect
				  )
{
	if (dwIndex >= ( DWORD32 )m_cTrackInfo ||
		prcPlate == NULL)
	{
		return E_FAIL;
	}

	bool fHasTrack(true);
	if (!m_pParam->m_fUseTemplet)
	{
		// δʹ��ģ��ƥ�䣬���Ƹ�����Ψһ��׼
		fHasTrack = m_rgTrackInfo[ dwIndex ].m_fObservationExists;
	}
	else
	{
		// ʹ����ģ��ƥ�䣬���Ƹ��ٺ�ģ��ƥ��߱�һ�����н��
		fHasTrack =  (m_rgTrackInfo[ dwIndex ].m_fObservationExists)
			|| (m_rgTrackInfo[ dwIndex ].m_fTempTrackExists);

	}

	if (fHasTrack)
	{
		*prcPlate = m_rgTrackInfo[ dwIndex ].LastInfo().rcPos;
		*prcRect = m_rgTrackInfo[ dwIndex ].m_rcPredict;
	}
	else
	{
		*prcPlate = CRect(-1, -1, -1, -1);
		*prcRect = CRect(-1, -1, -1, -1);
		return S_FALSE;
	}

	return S_OK;
}

HRESULT CTrackerImpl::GetDetectCount(PDWORD32 pdwCount)
{
	if (pdwCount == NULL)
	{
		return E_POINTER;
	}
	*pdwCount = m_cDetected;
	return S_OK;
}

HRESULT CTrackerImpl::GetDetectPos(DWORD32 dwIndex, CRect *prcDetected)
{
	if (dwIndex >= (DWORD32)m_cDetected ||
		prcDetected == NULL)
	{
		return E_FAIL;
	}
	*prcDetected = m_rgrcDetected[dwIndex];
	return S_OK;
}

HRESULT CTrackerImpl::GetObjCount(PDWORD32 pdwCount)
{
	if (pdwCount == NULL)
	{
		return E_POINTER;
	}
	*pdwCount = m_cObjTrackInfo;
	return S_OK;
}

HRESULT CTrackerImpl::GetObjPos(DWORD32 dwIndex, CRect *prcObj)
{
	if (dwIndex >= (DWORD32)m_cObjTrackInfo ||
		prcObj == NULL)
	{
		return E_FAIL;
	}
	if (m_rgObjTrackInfo[dwIndex].m_fTrack)
	{
		*prcObj = m_rgObjTrackInfo[dwIndex].LastInfo().m_rcFG;
	}
	else
	{
		*prcObj = CRect(-1, -1, -1, -1);
	}
	return S_OK;
}

HRESULT CTrackerImpl::GetEPObjCount(PDWORD32 pdwCount)
{
	if (pdwCount == NULL)
	{
		return E_POINTER;
	}
	*pdwCount = m_cEPObjTrackInfo;
	return S_OK;
}

HRESULT CTrackerImpl::GetEPObjPos(DWORD32 dwIndex, CRect *prcObj)
{
	if (dwIndex >= (DWORD32)m_cEPObjTrackInfo ||
		prcObj == NULL)
	{
		return E_FAIL;
	}
	if (m_rgEPObjTrackInfo[dwIndex].m_fTrack)
	{
		*prcObj = m_rgEPObjTrackInfo[dwIndex].LastInfo().m_rcFG;
	}
	else
	{
		*prcObj = CRect(-1, -1, -1, -1);
	}
	return S_OK;
}

HRESULT CTrackerImpl::ClearTrackInfo(PROCESS_ONE_FRAME_RESPOND* pProcessRespond)
{
	for (int i=0; i<m_cTrackInfo; i++)
	{
		m_rgTrackInfo[i].Clear(pProcessRespond);
	}
	m_cTrackInfo = 0;
	for (int i = 0; i < m_cObjTrackInfo; i++)
	{
		m_rgObjTrackInfo[i].Clear(pProcessRespond);
	}
	m_cObjTrackInfo = 0;
	for (int i = 0; i < s_knMaxVideoChannel; i++)
	{
		if (m_rgpObjectDetector[i])
		{
			m_rgpObjectDetector[i]->ClearBackgroup();
		}
	}
	m_cDetected = 0;
	return S_OK;
}

HRESULT CTrackerImpl::IsVoted(DWORD32 dwIndex, bool *pfVoted)
{
	if ( dwIndex >= ( DWORD32 )m_cTrackInfo ) return E_FAIL;
	*pfVoted = m_rgTrackInfo[ dwIndex ].m_fVoted;
	return S_OK;
}

const int TEST_DETECT_TYPE = 0;
const int TEST_SEGMENT_TYPE = 1;
const int TEST_RECOGNITION_TYPE = 2;
int g_iCoreTestType = TEST_DETECT_TYPE;		//���Ե�ģ������

const int DET_TEST_BLUE = 0;
const int DET_TEST_YELLOW = 1;
const int DET_TEST_DYELLOW = 2;
int g_iDetTestType = DET_TEST_YELLOW;			//���ģ���������
int g_iDetTestFastScanFlag = (int)FALSE;		//���ģ���SubScan��־
int g_iDetTestColor = (int)PC_UNKNOWN;			//���ģ��ĳ�����ɫԤ��
int g_iDetTestLastWidth = 0;					//���ģ��ĳ��ƿ��Ԥ��

int g_iSegRecTestColor = (int)PC_UNKNOWN;		//�ָ�ģ��ĳ�����ɫ����
int g_iSegRecTestType = (int)PLATE_UNKNOWN;		//�ָ�ģ��ĳ������Ͳ���


HRESULT CTrackerImpl::TestDetPerformance(
	HV_COMPONENT_IMAGE *phciTest,	//����ͼ��
	char *pszReport,				//���Ա���
	DWORD32 dwReportBufSize,		//���Ա��滺��������
	DWORD32 dwMaxTestCount,			//�����Դ���
	DWORD32 dwMaxTestDuration		//������ʱ��(����)
)
{
	strcat(pszReport, "���ģ���������\n");
	if (m_pPlateDetector == NULL)
	{
		strcat(pszReport, "\t���ģ��δ��ʼ��!\n");
		return E_OBJ_NO_INIT;
	}
	DWORD32 dwCount = 0;
	DWORD32 dwBeginTime = GetSystemTick();
	DWORD32 dwEndTime;
	HRESULT hr;
	MULTI_ROI MultiROI =
	{
		ARRSIZE(m_rgTempPlatePos),
		m_rgTempPlatePos
	};
	switch (g_iDetTestType)
	{
		case DET_TEST_BLUE:
			do {
				MultiROI.dwNumROI = ARRSIZE(m_rgTempPlatePos);
				hr = m_pPlateDetector->ScanPlatesComponentImage(
					PLATE_BLUE_DETECTOR,
					phciTest,
					&MultiROI,
					m_pParam->g_nDetMinScaleNum,
					m_pParam->g_nDetMaxScaleNum,
					g_iDetTestFastScanFlag,
					(PLATE_COLOR)g_iDetTestColor,
					g_iDetTestLastWidth);
				if (hr != S_OK)
				{
					goto RETURN_REPORT;
				}
				dwEndTime = GetSystemTick();
				dwCount++;
				SystemDog();
			} while (dwCount < dwMaxTestCount &&
					 dwEndTime - dwBeginTime < dwMaxTestDuration);
			break;
		case DET_TEST_YELLOW:
			do {
				MultiROI.dwNumROI = ARRSIZE(m_rgTempPlatePos);
				hr = m_pPlateDetector->ScanPlatesComponentImage(
					PLATE_YELLOW_DETECTOR,
					phciTest,
					&MultiROI,
					m_pParam->g_nDetMinScaleNum,
					m_pParam->g_nDetMaxScaleNum,
					g_iDetTestFastScanFlag,
					(PLATE_COLOR)g_iDetTestColor,
					g_iDetTestLastWidth);
				if (hr != S_OK)
				{
					goto RETURN_REPORT;
				}
				dwEndTime = GetSystemTick();
				dwCount++;
				SystemDog();
			} while (dwCount < dwMaxTestCount &&
					 dwEndTime - dwBeginTime < dwMaxTestDuration);
			break;
		case DET_TEST_DYELLOW:
			do {
				MultiROI.dwNumROI = ARRSIZE(m_rgTempPlatePos);
				hr = m_pPlateDetector->ScanPlatesComponentImage(
					PLATE_YELLOW2_DETECTOR,
					phciTest,
					&MultiROI,
					m_pParam->g_nDetMinScaleNum,
					m_pParam->g_nDetMaxScaleNum,
					g_iDetTestFastScanFlag,
					(PLATE_COLOR)g_iDetTestColor,
					g_iDetTestLastWidth);
				if (hr != S_OK)
				{
					goto RETURN_REPORT;
				}
				dwEndTime = GetSystemTick();
				dwCount++;
				SystemDog();
			} while (dwCount < dwMaxTestCount &&
					 dwEndTime - dwBeginTime < dwMaxTestDuration);
			break;
		default:
			break;
	}

RETURN_REPORT:
	char szBuffer[200];
	sprintf(szBuffer, "\thr = 0x%08X\n", hr);
	strcat(pszReport, szBuffer);
	sprintf(szBuffer, "\tROI_Count = %d\n", MultiROI.dwNumROI);
	strcat(pszReport, szBuffer);
	for (int i = 0; i < MultiROI.dwNumROI; i++)
	{
		sprintf(szBuffer, "\t\tROI left=%d, top=%d, right=%d, bottom=%d, color=%d, type=%d\n",
			MultiROI.rcROI[i].left, MultiROI.rcROI[i].top, MultiROI.rcROI[i].right, MultiROI.rcROI[i].bottom,
			MultiROI.rcROI[i].color, MultiROI.rcROI[i].nPlateType);
		strcat(pszReport, szBuffer);
	}
	sprintf(szBuffer, "\tg_iCoreTestType = %d\n", g_iCoreTestType);
	strcat(pszReport, szBuffer);
	sprintf(szBuffer, "\tg_iDetTestType = %d\n", g_iDetTestType);
	strcat(pszReport, szBuffer);
	sprintf(szBuffer, "\tg_iDetTestFastScanFlag = %d\n", g_iDetTestFastScanFlag);
	strcat(pszReport, szBuffer);
	sprintf(szBuffer, "\tg_iDetTestColor = %d\n", g_iDetTestColor);
	strcat(pszReport, szBuffer);
	sprintf(szBuffer, "\tg_iDetTestLastWidth = %d\n", g_iDetTestLastWidth);
	strcat(pszReport, szBuffer);
	sprintf(szBuffer, "\tTotal Duration = %d\n", dwEndTime - dwBeginTime);
	strcat(pszReport, szBuffer);
	sprintf(szBuffer, "\tTest Count = %d\n", dwCount);
	strcat(pszReport, szBuffer);
	sprintf(szBuffer, "\tAverage Time = %d\n", (dwEndTime - dwBeginTime) / dwCount);
	strcat(pszReport, szBuffer);
	return hr;
}

/*
HRESULT CTrackerImpl::TestSegPerformance(
	HV_COMPONENT_IMAGE *phciTest,	//����ͼ��
	char *pszReport,				//���Ա���
	DWORD32 dwReportBufSize,		//���Ա��滺��������
	DWORD32 dwMaxTestCount,			//�����Դ���
	DWORD32 dwMaxTestDuration		//������ʱ��(����)
)
{
	strcat(pszReport, "�ָ�ʶ��ģ���������:\n");
	DWORD32 dwCount = 0;
	DWORD32 dwBeginTime = GetSystemTick();
	DWORD32 dwEndTime;
	HRESULT hr;
	PlateInfo plateNew;
	do {
		plateNew.color = (PLATE_COLOR)g_iSegRecTestColor;
		plateNew.nPlateType = (PLATE_TYPE)g_iSegRecTestType;
		plateNew.iLastDetWidth = phciTest->iWidth;
		CPersistentComponentImage imgRectified;
		hr = RecognizePlate(*phciTest, plateNew, imgRectified);
		if (hr != S_OK)
		{
			goto RETURN_REPORT;
		}
		dwEndTime = GetSystemTick();
		dwCount++;
		SystemDog();
	} while (dwCount < dwMaxTestCount &&
			 dwEndTime - dwBeginTime < dwMaxTestDuration);

RETURN_REPORT:
	char szBuffer[200];
	sprintf(szBuffer, "\thr = 0x%08X\n", hr);
	strcat(pszReport, szBuffer);
	sprintf(szBuffer, "\tg_iSegRecTestColor = %d\n", g_iSegRecTestColor);
	strcat(pszReport, szBuffer);
	sprintf(szBuffer, "\tg_iSegRecTestType = %d\n", g_iSegRecTestType);
	strcat(pszReport, szBuffer);
	sprintf(szBuffer, "\tTotalConf=%f, Type=%d, Color=%d\n",
		plateNew.fltTotalConfidence.ToFloat(),
		plateNew.nPlateType, plateNew.color
		);
	strcat(pszReport, szBuffer);
	char szPlate[20] = {0};
	GetPlateNameAlpha(
		szPlate,
		plateNew.nPlateType,
		plateNew.color,
		plateNew.rgbContent);
	sprintf(szBuffer, "\tPlateNum=%s\n", szPlate);
	strcat(pszReport, szBuffer);
	sprintf(szBuffer, "\tTotal Duration = %d\n", dwEndTime - dwBeginTime);
	strcat(pszReport, szBuffer);
	sprintf(szBuffer, "\tTest Count = %d\n", dwCount);
	strcat(pszReport, szBuffer);
	sprintf(szBuffer, "\tAverage Time = %d\n", (dwEndTime - dwBeginTime) / dwCount);
	strcat(pszReport, szBuffer);
	return hr;
}
*/

//���Լ��ģ������ܣ����Ե�ģ�����ͺͲ��Բ���ͨ��ȫ�ֲ����б�����,
//ȫ�ֲ����б�ͨ��INI�ļ�����.
HRESULT CTrackerImpl::TestPerformance(
	HV_COMPONENT_IMAGE *phciTest,	//��������
	char *pszReport,				//���Ա���
	DWORD32 dwReportBufSize,		//���Ա��滺��������
	DWORD32 dwMaxTestCount,			//�����Դ���
	DWORD32 dwMaxTestDuration		//������ʱ��(����)
)
{
	return E_NOTIMPL;
/*
	if (phciTest == NULL || pszReport == NULL)
	{
		return E_POINTER;
	}
	if (dwReportBufSize == 0 ||
		dwMaxTestCount == 0 && dwMaxTestDuration == 0)
	{
		return E_INVALIDARG;
	}
	HRESULT hr = S_OK;
	switch (g_iCoreTestType)
	{
		case TEST_DETECT_TYPE:
			hr = TestDetPerformance(
				phciTest,
				pszReport,
				dwReportBufSize,
				dwMaxTestCount,
				dwMaxTestDuration
			);
			break;
		case TEST_SEGMENT_TYPE:
			hr = TestSegPerformance(
				phciTest,
				pszReport,
				dwReportBufSize,
				dwMaxTestCount,
				dwMaxTestDuration
			);
			break;
		case TEST_RECOGNITION_TYPE:
		default:
			break;
	}
	return hr;*/
}


HRESULT CTrackerImpl::GetLightType(LIGHT_TYPE* pLightType)
{
	if (NULL == pLightType) return E_POINTER;
	*pLightType = m_PlateLightType;
	return S_OK;
}

HRESULT CTrackerImpl::CheckLight()
{
	const int FIRST_CHECK_COUNT = 5;
	const int FIRST_COUNT_THRESHOLD = 3;
	const int MAX_CHECK_COUNT = 1000;
	const int COUNT_THRESHOLD = 900;

	static bool fFirstTime = true;
	static int iFrameCount = 0;
	static int iPositiveDayCount = 0;
	static int iPositiveNightCount = 0;
	static int iPositiveLightOff = 0;
	static int iPositiveLightOn = 0;
	static int iSuccessCount = 0;

	iFrameCount++;
	if (fFirstTime)
	{
		if (iSuccessCount < 2)
		{
			iFrameCount = 0;
			if (m_nFirstLightType >= 0 && m_nFirstLightType < LIGHT_TYPE_COUNT)
			{
				m_PlateLightType = m_nFirstLightType;
				iSuccessCount = 2;
//				HV_Trace("\nUse last PlateLightType");
			}
			else
			{
				if (CheckLightType(m_iAvgY, true) == S_OK)
				{
					iSuccessCount++;
				}
			}
		}
		else
		{
			if ( iFrameCount >= FIRST_CHECK_COUNT )
			{
				fFirstTime = false;
				if (iPositiveNightCount >= FIRST_COUNT_THRESHOLD)
				{
					m_LightType = NIGHT;
				}
				else
				{
					m_LightType = DAY;
				}

				if(iPositiveLightOff >= FIRST_COUNT_THRESHOLD)
				{
					m_iForceLightOffFlag = 1;
				}
				if(iPositiveLightOn >= FIRST_COUNT_THRESHOLD)
				{
					m_iForceLightOffFlag = 0;
				}
				// �ж��Ƿ���Ҫʹ��ƫ�⾵
				// ����ģʽ���ҳ������ȵȼ�С���м�ֵ,ʹ��ƫ�⾵
				if (DAY == m_LightType && m_PlateLightType <= m_nLightTypeCpl)
				{
					if (1 != m_iCplStatus && 1 == m_iCtrlCpl)
					{
						m_iCplStatus = 1;
						m_iAdjustForAvgBrigtness = 1;
						m_dwAdjustTick = GetSystemTick();
						m_iForceLightOffFlag = 1;
					}
				}
//				HV_Trace("\nFirstTime end...\nLightType=%d, PlateLightType=%d", m_LightType, m_PlateLightType);
			}
			else
			{
				if (m_iAvgY <= m_iNightThreshold)
				{
					iPositiveNightCount++;
				}
				else
				{
					iPositiveDayCount++;
				}

				if(m_iAvgY <= m_iForceLightOffThreshold)
				{
					iPositiveLightOn++;
				}
				else
				{
					iPositiveLightOff++;
				}
			}
		}
	}
	else
	{
		// 5������û�������ͨ�����������ж�
		if (GetSystemTick() - m_dwLastCarLeftTime > 300000)
		{
			CheckLightType(m_iAvgY, true);
		}

		// ƫ�⾵״̬�б仯ʱ��ͨ�����������ж�һ��
		// ƫ�⾵״̬�������ʵ����Ҫ500msʱ��
		if (m_iAdjustForAvgBrigtness)// && GetSystemTick() - m_dwAdjustTick >= 500)
		{
			CheckLightType(m_iAvgY, true);	
		}

		if ( m_iAvgY > m_iNightThreshold )
		{
			iPositiveDayCount++;
		}
		else
		{
			iPositiveNightCount++;
		}

		if(m_iAvgY <= m_iForceLightOffThreshold)
		{
			iPositiveLightOn++;
		}
		else
		{
			iPositiveLightOff++;
		}

		if ( iFrameCount >= MAX_CHECK_COUNT )
		{
			if ( iPositiveDayCount >= COUNT_THRESHOLD )
			{
				m_LightType = DAY;
			}
			if ( iPositiveNightCount >= COUNT_THRESHOLD )
			{
				m_LightType = NIGHT;
			}

			if(iPositiveLightOn >= COUNT_THRESHOLD)
			{
				m_iForceLightOffFlag = 0;
			}
			if(iPositiveLightOff >= COUNT_THRESHOLD)
			{
				m_iForceLightOffFlag = 1;
			}
//			HV_Trace("\nLightType=%d, PlateLightType=%d", m_LightType, m_PlateLightType);
			iFrameCount = 0;
			iPositiveDayCount = 0;
			iPositiveNightCount = 0;
			iPositiveLightOn = 0;
			iPositiveLightOff = 0;
		}

		// �ж��Ƿ���Ҫʹ��ƫ�⾵
		// ����ģʽ���ҳ������ȵȼ�С���м�ֵ,ʹ��ƫ�⾵
		if (DAY == m_LightType && m_PlateLightType <= m_nLightTypeCpl)
		{
			if (1 != m_iCplStatus && 1 == m_iCtrlCpl)
			{
				m_iCplStatus = 1;
				m_iAdjustForAvgBrigtness = 1;
				m_dwAdjustTick = GetSystemTick();
				m_iForceLightOffFlag = 1;
			}
		}

		if(NIGHT == m_LightType)
		{
			if (0 != m_iCplStatus && 1 == m_iCtrlCpl)
			{
				m_iCplStatus = 0;
				m_iAdjustForAvgBrigtness = 1;
				m_dwAdjustTick = GetSystemTick();
				m_iForceLightOffFlag = 0;
			}
		}
	}
	if(m_iForceLightOffAtDay)
	{
		if(m_LightType == DAY && m_iPulseLevel > 0 
			&& m_iForceLightOffFlag == 1)
		{
			m_iPulseLevel = 0;
		}
	}
	return S_OK;
}

// 
HRESULT CTrackerImpl::CheckLightType(int iCarY, bool fIsAvgBrightness)
{
	HRESULT hr = S_FALSE;
	static int iFrameCount = 0;
	static int iSumBrightness = 0;
	static int s_iMinValue = 255;
	static int s_iMaxValue = 0;
	static int s_iSucessCount = 0;
	static int s_iFirstSetLightType = 0;
	if((m_nFirstLightType >= 0 && m_nFirstLightType < LIGHT_TYPE_COUNT) || s_iSucessCount >= 2)
	{

		if(s_iFirstSetLightType != 1)
		{
			s_iFirstSetLightType = 1;
		}
		s_iSucessCount = 0;
	}

	// ������������������Ҫ��Ӧʱ䣬�ʵȴ?S��ż����һ֡����
	static DWORD32 s_dwLastSetParamTick = GetSystemTick();
	DWORD32 dwTimeInterval = m_cfgParam.iPlatform == 0 ? 6000 : 6000;
	if (GetSystemTick() - s_dwLastSetParamTick < dwTimeInterval && fIsAvgBrightness)
	{
		if(m_iForceLightOffAtDay)
		{
			if(m_LightType == DAY && m_iPulseLevel > 0 
				&& m_iForceLightOffFlag == 1)
			{
				m_iPulseLevel = 0;
			}
		}
		return S_FALSE;
	}

	iFrameCount++;
	iSumBrightness += iCarY;

	int iMaxBrightness = 0;
	int iMinBrightness = 0;

	iMaxBrightness = m_iMaxPlateBrightness;
	iMinBrightness = m_iMinPlateBrightness;

	if (iCarY > s_iMaxValue)
	{
		s_iMaxValue = iCarY;
	}
	if (iCarY < s_iMinValue)
	{
		s_iMinValue = iCarY;
	}

	if (iFrameCount >= m_iPlateLightCheckCount && iFrameCount > 2)
	{
		int iAvgBrightness = (iSumBrightness - s_iMaxValue - s_iMinValue) / (iFrameCount - 2);
		if (iAvgBrightness < iMinBrightness && m_PlateLightType < LIGHT_TYPE_COUNT)
		{
			if (m_PlateLightType < LIGHT_TYPE_COUNT - 1)
			{
				m_PlateLightType = (LIGHT_TYPE)((int)m_PlateLightType + 1);
			}
			else
			{
				// �����ȼ���������ߣ���ص�ƫ�⾵
				if (0 != m_iCplStatus && 1 == m_iCtrlCpl)
				{
					m_iCplStatus = 0;
					m_iAdjustForAvgBrigtness = 1;
					m_dwAdjustTick = GetSystemTick();
				}
				else if( m_iUsedLight == 1 && m_iPulseLevel < 8)
				{
					if(s_iFirstSetLightType == 0)
					{
						s_iFirstSetLightType = 1;
						m_iPulseLevel = 6;
					}
					else
					{
						m_iPulseLevel++;
					}
					m_iAdjustForAvgBrigtness = 0;
					hr = S_OK;
					//ͨ���������ȼ��ʱ����Ȳ��ܴﵽ���ȼ�
					if(fIsAvgBrightness && m_iPulseLevel > 6)
					{
						m_iPulseLevel = 6;
					}
				}
				else
				{
					m_iAdjustForAvgBrigtness = 0;
					hr = S_OK;
				}
			}
		}
		else if (iAvgBrightness > iMaxBrightness
				&& (m_iUsedLight == 1 && m_iPulseLevel > 0) )
		{
			m_iPulseLevel--;
			m_iAdjustForAvgBrigtness = 0;
			hr = S_OK;
		}
		else if(iAvgBrightness > iMaxBrightness && DAY == m_LightType
			&& m_iCtrlCpl == 1 && m_PlateLightType <= m_nLightTypeCpl && m_iCplStatus != 1)
		{
			m_iCplStatus = 1;
			m_iAdjustForAvgBrigtness = 1;
			m_dwAdjustTick = GetSystemTick();
		}
		else if (iAvgBrightness > iMaxBrightness && m_PlateLightType > 0)
		{
			m_PlateLightType = (LIGHT_TYPE)((int)m_PlateLightType - 1);
		}
		else
		{
			m_iAdjustForAvgBrigtness = 0;
			hr = S_OK;
		}
		s_dwLastSetParamTick = GetSystemTick();
		iFrameCount = 0;
		iSumBrightness = 0;
		s_iMinValue = 255;
		s_iMaxValue = 0;
	}
	if(hr == S_OK)
	{
		s_iSucessCount++;
	}
	if(m_iForceLightOffAtDay)
	{
		if(m_LightType == DAY && m_iPulseLevel > 0 
			&& m_iForceLightOffFlag == 1)
		{
			m_iPulseLevel = 0;
		}
	}
	return hr;
}

HRESULT CTrackerImpl::RemoveInvalidTracks(PROCESS_ONE_FRAME_RESPOND* pProcessRespond)
{
	int iDist;

	iDist = 0;
	for (int i=0; i<m_cTrackInfo; i++)
	{
		if (m_rgTrackInfo[i].m_State == tsWaitRemove)
		{
			m_rgTrackInfo[i].Clear(pProcessRespond); //������������������ͷ�m_pimgLastCaptureָ��ʱ��m_pimgLastCapture�Χָ�?
			iDist++;
		}
		else
		{
			if (iDist>0)
			{
				RTN_HR_IF_FAILED(m_rgTrackInfo[i].Detach(m_rgTrackInfo[i-iDist], pProcessRespond));
			}
		}
	}
	m_cTrackInfo-=iDist;

	iDist = 0;
	for (int i=0; i<m_cObjTrackInfo; i++)
	{
		if (m_rgObjTrackInfo[i].m_objTrackState == OBJ_TRACK_END ||
			m_rgObjTrackInfo[i].m_objTrackState == OBJ_TRACK_INIT)
		{
			m_rgObjTrackInfo[i].Clear(pProcessRespond);
			iDist++;
		}
		else
		{
			if (iDist > 0)
			{
				m_rgObjTrackInfo[i - iDist].Assign(m_rgObjTrackInfo[i], pProcessRespond);
				m_rgObjTrackInfo[i].Clear(pProcessRespond);
			}
		}
	}
	m_cObjTrackInfo -= iDist;
	//EP track faults
    iDist = 0;
    for (int i=0; i<m_cEPObjTrackInfo; i++)
    {
        if (m_rgEPObjTrackInfo[i].m_objTrackState == OBJ_TRACK_END ||
            m_rgEPObjTrackInfo[i].m_objTrackState == OBJ_TRACK_INIT)
        {
            m_rgEPObjTrackInfo[i].Clear(pProcessRespond);
            //HV_Trace("\nclear id:[%d]",m_rgEPObjTrackInfo[i].m_Feature.iObjId);
            iDist++;
        }
        else
        {
            if (iDist > 0)
            {
                //m_rgEPObjTrackInfo[i - iDist] = m_rgEPObjTrackInfo[i];
                m_rgEPObjTrackInfo[i - iDist].Assign(m_rgEPObjTrackInfo[i], pProcessRespond);
                m_rgEPObjTrackInfo[i].Clear(pProcessRespond);
                //HV_Trace("\ncopy and clear id:[%d]",m_rgObjTrackInfo[i].m_Feature.iObjId);
            }
        }
    }
    m_cEPObjTrackInfo -= iDist;

	return S_OK;
}

HRESULT CTrackerImpl::FireNoPlateCarLeftEvent( 
	PROCESS_ONE_FRAME_PARAM* pParam, 
	CObjTrackInfo& ObjTrackInfo, 
	PROCESS_ONE_FRAME_RESPOND* pProcessRespond 
	)
{

	if(ObjTrackInfo.IsClearImage() || pProcessRespond->cTrigEvent.iCarLeftCount >= MAX_EVENT_COUNT)
		return S_FALSE;

	int iRoad = ObjTrackInfo.m_iRoad;
	if( iRoad == -1 )
	{
		iRoad = MatchRoad(ObjTrackInfo.m_rgRect[0].CenterPoint().x, ObjTrackInfo.m_rgRect[0].CenterPoint().y);
		if( iRoad < 0 || iRoad > 15 )
		{
			iRoad = 0;
		}
	}
	if (GetCurrentParam()->g_PlateRcogMode == PRM_HIGHWAY_HEAD 
		|| GetCurrentParam()->g_PlateRcogMode == PRM_CAP_FACE)
	{
		DWORD32 frameElapse;
		if (m_leftCar[iRoad].car_plate == PC_YELLOW)
		{
			frameElapse = pParam->dwFrameNo - m_leftCar[iRoad].frame_no;
			if (frameElapse < m_iBigCarDelayFrameNo)
			{
				return S_OK;
			}
		}
	}

//	if (!m_pCallback)
//	{
//		return S_FALSE;
//	}

//	RESULT_IMAGE_STRUCT ResultImage;
//	ResultImage.pimgBestSnapShot = ObjTrackInfo.m_pBestReftImage;
//	ResultImage.pimgLastSnapShot = ObjTrackInfo.m_pLastRefImage;

	// ���Ƴ���ͶƱ
	ObjTrackInfo.Vote(ObjTrackInfo.m_nVotedCarType);
	float fltCount = 0.0;
	for(int i = 0; i < ObjTrackInfo.m_iObjFrameCount;i++)
	{
		fltCount += ObjTrackInfo.m_rgfltFrameSpeed[i];
	}

	if(ObjTrackInfo.m_iObjFrameCount > 1)
	{
		fltCount /= ObjTrackInfo.m_iObjFrameCount;
	}
	//HV_Trace("��֡��:%d,ƽ���ٶȣ�%0.1f\r\n",ObjTrackInfo.m_iObjFrameCount, fltCount);
	
	if((ObjTrackInfo.m_nVotedCarType == CT_WALKMAN || ObjTrackInfo.m_nVotedCarType == CT_BIKE) 
		&& (int)fltCount < m_iWalkManSpeed)
	{
		ObjTrackInfo.m_nVotedCarType = CT_WALKMAN;			// �µ������ж�
	}
	else
	{
		if (ObjTrackInfo.m_nVotedCarType == CT_WALKMAN)
		{
			ObjTrackInfo.m_nVotedCarType = CT_BIKE;
		}
	}

	PROCESS_IMAGE_CORE_RESULT* pCurResult = &(pProcessRespond->cTrigEvent.rgCarLeftCoreInfo[pProcessRespond->cTrigEvent.iCarLeftCount]);

	// ����ṹ��ֵ
	memset(pCurResult->rgbContent, 0, sizeof(pCurResult->rgbContent));
	pCurResult->nCarType = ObjTrackInfo.m_nVotedCarType;
	pCurResult->fltCarspeed = 0;

	//zhaopy
	// todo... �޳���û��λ�õ�����������Ҫ���롣
	pCurResult->dwBestSnapShotRefTime = ObjTrackInfo.m_dwBestSnapShotRefTime;
	pCurResult->dwLastSnapShotRefTime = ObjTrackInfo.m_dwLastSnapShotRefTime;
	pCurResult->dwBeginCaptureRefTime = ObjTrackInfo.m_dwBeginCaptureRefTime;
	pCurResult->dwBestCaptureRefTime = ObjTrackInfo.m_dwBestCaptureRefTime;
	pCurResult->dwLastCaptureRefTime = ObjTrackInfo.m_dwLastCaptureRefTime;

	pCurResult->dwTriggerTime = 0;
	if( ObjTrackInfo.m_fHasTrigger && ObjTrackInfo.m_dwTriggerTimeMs != 0 )
	{
		pCurResult->dwTriggerTime = ObjTrackInfo.m_dwTriggerTimeMs;
	}
	else if( m_fEnableCarArriveTrigger && ObjTrackInfo.m_nCarArriveRealTime != 0 )
	{
		pCurResult->dwTriggerTime = ObjTrackInfo.m_nCarArriveRealTime;
	}

	//���Ӻ����Ϣ
	if(m_nRedLightCount < 20)
	{
		pCurResult->rcRedLightCount = m_nRedLightCount;
		for(int i = 0; i < m_nRedLightCount; i++)
		{
			pCurResult->rcRedLightPos[i].left   = m_rgRedLightRect[i].left;
			pCurResult->rcRedLightPos[i].top    = m_rgRedLightRect[i].top * 2;
			pCurResult->rcRedLightPos[i].right  = m_rgRedLightRect[i].right;
			pCurResult->rcRedLightPos[i].bottom = m_rgRedLightRect[i].bottom * 2;
		}
	}

	RESULT_IMAGE_STRUCT* pResultImage = &(pCurResult->cResultImg);
	if(!ObjTrackInfo.m_pBestReftImage)
	{
		ObjTrackInfo.m_pBestReftImage = ObjTrackInfo.m_pOneRefImage;
		if(ObjTrackInfo.m_pOneRefImage)
			ImageAddRef(&pProcessRespond->cImgMemOperLog, ObjTrackInfo.m_pBestReftImage);
		pCurResult->dwBestSnapShotRefTime = pCurResult->dwBeginCaptureRefTime;
	}
	if(!ObjTrackInfo.m_pLastRefImage)
	{
		ObjTrackInfo.m_pLastRefImage = ObjTrackInfo.m_pTwoRefImage;
		if(ObjTrackInfo.m_pLastRefImage)
			ImageAddRef(&pProcessRespond->cImgMemOperLog, ObjTrackInfo.m_pLastRefImage);
		pCurResult->dwLastSnapShotRefTime = pCurResult->dwBestCaptureRefTime;
	}
	pResultImage->pimgBestSnapShot = ObjTrackInfo.m_pBestReftImage;
	pResultImage->pimgLastSnapShot = ObjTrackInfo.m_pLastRefImage;

	//�羯ģʽ���������ץ��ͼ
	//�ƹ��� 2011-08-12
	if(GetCurrentParam()->g_PlateRcogMode == PRM_ELECTRONIC_POLICE)
	{
		pResultImage->pimgBeginCapture = ObjTrackInfo.m_pOneRefImage;
		pResultImage->pimgBestCapture = ObjTrackInfo.m_pTwoRefImage;
		pResultImage->pimgLastCapture = ObjTrackInfo.m_pThreeRefImage;
	}
	else
	{
		pResultImage->pimgBeginCapture = NULL;
		pResultImage->pimgBestCapture = NULL;
		pResultImage->pimgLastCapture = NULL;
	}


//	pCurResult->pResultImage = &ResultImage;
//	pCurResult->iFrameNo = ObjTrackInfo.m_pBestReftImage->GetFrameNo();
//	pCurResult->iRefTime = ObjTrackInfo.m_pLastRefImage->GetRefTime();
//	pCurResult->nStartFrameNo = ObjTrackInfo.m_pFirstRefImage->GetFrameNo();
//	pCurResult->nEndFrameNo = ObjTrackInfo.m_pLastRefImage->GetFrameNo();
//	pCurResult->nFirstFrameTime = ObjTrackInfo.m_pFirstRefImage->GetRefTime();
	pCurResult->nDetectCrossLineEnable = m_pParam->m_iDetectCrossLineEnable;
	pCurResult->nDetectOverYellowLineEnable = m_pParam->m_iDetectOverYellowLineEnable;
	pCurResult->nFirstFrameTime = ObjTrackInfo.m_dwFirstFlameTime;
	pCurResult->iObservedFrames = ObjTrackInfo.m_iObjInfoCount;
	pCurResult->iVotedObservedFrames = ObjTrackInfo.m_iObjInfoCount;	
	pCurResult->nPlateLightType = m_PlateLightType;
	pCurResult->iCplStatus = m_iCplStatus;
	pCurResult->iPulseLevel = m_iPulseLevel;
    if( ObjTrackInfo.m_fHasTrigger && ObjTrackInfo.m_dwTriggerTimeMs != 0 )
	{
		pCurResult->nCarArriveTime = pParam->dwCurTick;//ObjTrackInfo.m_dwTriggerTimeMs;
	}
	else
	{
		// ���Ƴ����ȡͼƬʱ��
		pCurResult->nCarArriveTime = pParam->dwCurTick;//ObjTrackInfo.m_nCarArriveRealTime;
	}

	pCurResult->rcBestPlatePos = ObjTrackInfo.m_rcBestCarPos;
	pCurResult->rcLastPlatePos = ObjTrackInfo.m_rcLastCarPos;
	pCurResult->rcSecondPos = ObjTrackInfo.m_rgRect[1];
	//pCurResult->nCarColor = ObjTrackInfo.m_nVotedCarColor;
	//pCurResult->fOutputCarColor = CTrackInfo::m_fEnableRecgCarColor;
	pCurResult->fIsNight = (m_LightType == NIGHT) ? true : false;
	if(pCurResult->fIsNight && pCurResult->iPulseLevel < 1)
	{
		pCurResult->iPulseLevel = 1;
	}
	if(m_iForceLightOffAtDay)
	{
		if(m_LightType == DAY && m_iPulseLevel > 0 
			&& m_iForceLightOffFlag == 1)
		{
			m_iPulseLevel = 0;
		}
	}

	pCurResult->coltIsOverYellowLine	= COLT_NO;
	pCurResult->coltIsCrossLine		= COLT_NO;

	pCurResult->fReverseRun = false;
	switch (m_pParam->g_MovingDirection)
	{
	case MD_TOP2BOTTOM:
		if (ObjTrackInfo.LastInfo().m_rcFG.CenterPoint().y - ObjTrackInfo.m_rgObjInfo[0].m_rcFG.CenterPoint().y
			 < -(m_nHeight * m_iSpan * 0.01f))
		{
			pCurResult->fReverseRun = true;
		}
		break;
	case MD_BOTTOM2TOP:
		if (ObjTrackInfo.LastInfo().m_rcFG.CenterPoint().y - ObjTrackInfo.m_rgObjInfo[0].m_rcFG.CenterPoint().y
			 > m_nHeight * m_iSpan * 0.01f)
		{
			pCurResult->fReverseRun = true;
		}
		break;
	case MD_LEFT2RIGHT:
		if (ObjTrackInfo.LastInfo().m_rcFG.CenterPoint().x - ObjTrackInfo.m_rgObjInfo[0].m_rcFG.CenterPoint().x
			 < -(m_nHeight * m_iSpan * 0.01f))
		{
			pCurResult->fReverseRun = true;
		}
		break;
	case MD_RIGHT2LEFT:
		if (ObjTrackInfo.LastInfo().m_rcFG.CenterPoint().x - ObjTrackInfo.m_rgObjInfo[0].m_rcFG.CenterPoint().x
			  > m_nHeight * m_iSpan * 0.01f)
		{
			pCurResult->fReverseRun = true;
		}
		break;
	default:
		;
	}

	//�����ж���ʻ���򣬼�⳵���Ƿ��복���趨����һ��
	switch ( m_TravelDirection )
	{
	case MD_TOP2BOTTOM:
		if (ObjTrackInfo.LastInfo().m_rcFG.CenterPoint().y - ObjTrackInfo.m_rgObjInfo[0].m_rcFG.CenterPoint().y
			 < -(m_nHeight * m_iSpan * 0.01f))
		{
			pCurResult->fReverseRun = true;
		}
		break;
	case MD_BOTTOM2TOP:
		if (ObjTrackInfo.LastInfo().m_rcFG.CenterPoint().y - ObjTrackInfo.m_rgObjInfo[0].m_rcFG.CenterPoint().y
			 > m_nHeight * m_iSpan * 0.01f)
		{
			pCurResult->fReverseRun = true;
		}
		break;
	case MD_LEFT2RIGHT:
		if (ObjTrackInfo.LastInfo().m_rcFG.CenterPoint().x - ObjTrackInfo.m_rgObjInfo[0].m_rcFG.CenterPoint().x
			 < -(m_nHeight * m_iSpan * 0.01f))
		{
			pCurResult->fReverseRun = true;
		}
		break;
	case MD_RIGHT2LEFT:
		if (ObjTrackInfo.LastInfo().m_rcFG.CenterPoint().x - ObjTrackInfo.m_rgObjInfo[0].m_rcFG.CenterPoint().x
			  > m_nHeight * m_iSpan * 0.01f)
		{
			pCurResult->fReverseRun = true;
		}
		break;
	case MD_NOTSURE:
	default:
	}

	//�������У�����ץ��ͼ˳�򵹹��?
	//�羯ģʽ���������ץ��ͼ
	//�ƹ��� 2011-10-26
	if(GetCurrentParam()->g_PlateRcogMode == PRM_ELECTRONIC_POLICE)
	{
		if( pCurResult->fReverseRun )
		{
			pResultImage->pimgBeginCapture = ObjTrackInfo.m_pThreeRefImage;
			pResultImage->pimgBestCapture = ObjTrackInfo.m_pTwoRefImage;
			pResultImage->pimgLastCapture = ObjTrackInfo.m_pOneRefImage;

			pCurResult->rcBestPlatePos = ObjTrackInfo.m_rgRect[2];
			pCurResult->rcLastPlatePos = ObjTrackInfo.m_rgRect[1];
			pCurResult->rcFirstPos = ObjTrackInfo.m_rgRect[2];
			pCurResult->rcSecondPos= ObjTrackInfo.m_rgRect[1];
			pCurResult->rcThirdPos = ObjTrackInfo.m_rgRect[0];
		}
		else 
		{
			pResultImage->pimgBeginCapture = ObjTrackInfo.m_pOneRefImage;
			pResultImage->pimgBestCapture = ObjTrackInfo.m_pTwoRefImage;
			pResultImage->pimgLastCapture = ObjTrackInfo.m_pThreeRefImage;

			pCurResult->rcBestPlatePos = ObjTrackInfo.m_rgRect[0];
			pCurResult->rcLastPlatePos = ObjTrackInfo.m_rgRect[1];
			pCurResult->rcFirstPos = ObjTrackInfo.m_rgRect[0];
			pCurResult->rcSecondPos= ObjTrackInfo.m_rgRect[1];
			pCurResult->rcThirdPos = ObjTrackInfo.m_rgRect[2];
		}
	}

	//У������
	pCurResult->rcBestPlatePos.left   = pCurResult->rcBestPlatePos.left  * 100 / m_nWidth;
	pCurResult->rcBestPlatePos.top    = pCurResult->rcBestPlatePos.top   * 100 / m_nHeight;
	pCurResult->rcBestPlatePos.right  = pCurResult->rcBestPlatePos.right  * 100 / m_nWidth;
	pCurResult->rcBestPlatePos.bottom = pCurResult->rcBestPlatePos.bottom * 100 / m_nHeight;

	pCurResult->rcLastPlatePos.left   = pCurResult->rcLastPlatePos.left  * 100 / m_nWidth;
	pCurResult->rcLastPlatePos.top    = pCurResult->rcLastPlatePos.top   * 100 / m_nHeight;
	pCurResult->rcLastPlatePos.right  = pCurResult->rcLastPlatePos.right  * 100 / m_nWidth;
	pCurResult->rcLastPlatePos.bottom = pCurResult->rcLastPlatePos.bottom * 100 / m_nHeight;

	pCurResult->rcFirstPos.left   = pCurResult->rcFirstPos.left  * 100 / m_nWidth;
	pCurResult->rcFirstPos.top    = pCurResult->rcFirstPos.top   * 100 / m_nHeight;
	pCurResult->rcFirstPos.right  = pCurResult->rcFirstPos.right  * 100 / m_nWidth;
	pCurResult->rcFirstPos.bottom = pCurResult->rcFirstPos.bottom * 100 / m_nHeight;

	pCurResult->rcSecondPos.left   = pCurResult->rcSecondPos.left  * 100 / m_nWidth;
	pCurResult->rcSecondPos.top    = pCurResult->rcSecondPos.top   * 100 / m_nHeight;
	pCurResult->rcSecondPos.right  = pCurResult->rcSecondPos.right  * 100 / m_nWidth;
	pCurResult->rcSecondPos.bottom = pCurResult->rcSecondPos.bottom * 100 / m_nHeight;

	pCurResult->rcThirdPos.left   = pCurResult->rcThirdPos.left  * 100 / m_nWidth;
	pCurResult->rcThirdPos.top    = pCurResult->rcThirdPos.top   * 100 / m_nHeight;
	pCurResult->rcThirdPos.right  = pCurResult->rcThirdPos.right  * 100 / m_nWidth;
	pCurResult->rcThirdPos.bottom = pCurResult->rcThirdPos.bottom * 100 / m_nHeight;


	pCurResult->iAvgY = m_iAvgY;
	pCurResult->nRoadNo = ObjTrackInfo.m_iRoad;			// �������
	//pCurResult->iRoadNumberBegin =  CTrackInfo::m_iRoadNumberBegin;
	//pCurResult->iStartRoadNum = CTrackInfo::m_iStartRoadNum;
	pCurResult->iUnSurePeccancy = 0;

	if (pCurResult->nRoadNo == -1)
	{
		pCurResult->nRoadNo = MatchRoad(ObjTrackInfo.m_rgRect[0].CenterPoint().x, ObjTrackInfo.m_rgRect[0].CenterPoint().y);
		//�ƹ������,2011-03-07
		//pCurResult->iRoadType = CTrackInfo::m_roadInfo[pCurResult->nRoadNo].iRoadType;
	}

	if( m_pParam->m_fUsedTrafficLight )
	{
		//���̵Ƴ����ŵļ���ʹ��ץ�ĵĵ�һ�Ŵ�ͼ��λ��
		pCurResult->nRoadNo = MatchRoad(ObjTrackInfo.m_rgObjInfo[0].m_rcFG.CenterPoint().x, ObjTrackInfo.m_rgObjInfo[0].m_rcFG.CenterPoint().y);

		//��ʻ����
		RUN_TYPE rt = RT_UNSURE;
		//���������
		if( pCurResult->fReverseRun )
		{
			rt = RT_CONVERSE;
		}
		else
		{
			rt = CheckRunType(ObjTrackInfo.m_rgRect[0], ObjTrackInfo.m_rgRect[2]);
		}

		//���˳��������ʻ����
		int iFilter = CTrackInfo::m_roadInfo[pCurResult->nRoadNo].iFilterRunType;
		if( (iFilter & RRT_FORWARD) && rt == RT_FORWARD )
		{
			rt = RT_UNSURE;
		}
		else if( (iFilter & RRT_LEFT) && rt == RT_LEFT )
		{
			rt = RT_UNSURE;
		}
		else if( (iFilter & RRT_RIGHT) && rt == RT_RIGHT )
		{
			rt = RT_UNSURE;
		}
		else if( (iFilter & RRT_TURN) && rt == RT_TURN )
		{
			rt = RT_UNSURE;
		}
		pCurResult->rtType = rt;

		int iOnePosLightScene = ObjTrackInfo.m_iOnePosLightScene;
		int iTwoPosLightScene = ObjTrackInfo.m_iPassStopLightScene;

		if( CTrackInfo::m_roadInfo[pCurResult->nRoadNo].iRoadType & RRT_LEFT )
		{
			iTwoPosLightScene = ObjTrackInfo.m_iPassLeftStopLightScene;
		}
		//�����"���˵�����ץ��ͼΪ�̵Ƶ�Υ��",���õ�����ץ���ߵƵ�״̬����Ϊ�жϵ�����
		//Τ�����޸�,2010-12-22
		if(m_iFilterRushPeccancy && iTwoPosLightScene != -1)
		{
			iTwoPosLightScene = ObjTrackInfo.m_iThreePosLightScene;
		}

		//GetLightScene(iOnePosLightScene, &pCurResult->tsOnePosScene);
		//GetLightScene(iTwoPosLightScene, &pCurResult->tsTwoPosScene);
		m_cTrafficLight.GetLightScene(iOnePosLightScene, &pCurResult->tsOnePosScene);
		m_cTrafficLight.GetLightScene(iTwoPosLightScene, &pCurResult->tsTwoPosScene);

		/*PECCANCY_TYPE*/int pt = 0;
		int iRoadType = (RRT_FORWARD | RRT_LEFT | RRT_RIGHT | RRT_TURN);
		if( pCurResult->nRoadNo != -1 )
		{
			iRoadType = CTrackInfo::m_roadInfo[pCurResult->nRoadNo].iRoadType;
			//�ƹ������,2011-03-07
			//pCurResult->iRoadType = CTrackInfo::m_roadInfo[pCurResult->nRoadNo].iRoadType;
		}

		//ֻ�����Ӧ�����ĺ�ƿ�ʼʱ��
		//pCurResult->lrtRedStart = m_redrealtime;
		m_cTrafficLight.GetRedLightStartTime(&pCurResult->lrtRedStart);
		if( (iRoadType & RRT_LEFT) == 0 ) 
		{
			pCurResult->lrtRedStart.dwLeftL = pCurResult->lrtRedStart.dwLeftH = 0;
		}
		if( (iRoadType & RRT_FORWARD) == 0 )
		{
			pCurResult->lrtRedStart.dwForwardL = pCurResult->lrtRedStart.dwForwardH = 0;
		}
		if( (iRoadType & RRT_RIGHT) == 0 ) 
		{
			pCurResult->lrtRedStart.dwRightL = pCurResult->lrtRedStart.dwRightH = 0;
		}
		if( (iRoadType & RRT_TURN) == 0 ) 
		{
			pCurResult->lrtRedStart.dwTurnL = pCurResult->lrtRedStart.dwTurnH = 0;
		}

		//�����ʻ������δ֪�����ڵĳ���ֻ��һ�ַ���ģ����������ʻ����Ϊ�������ġ�
		if( pCurResult->rtType == RT_UNSURE && CTrackInfo::m_iRunTypeEnable == 1 )
		{
			if( iRoadType == RRT_LEFT )
			{
				pCurResult->rtType = RT_LEFT;
			}
			else if( iRoadType == RRT_FORWARD )
			{
				pCurResult->rtType = RT_FORWARD;
			}
			else if( iRoadType == RRT_RIGHT )
			{
				pCurResult->rtType = RT_RIGHT;
			}
			else if( iRoadType == RRT_TURN )
			{
				pCurResult->rtType = RT_TURN;
			}
		}

		DWORD32 dwCaptureTime = (pResultImage->pimgBestCapture == NULL ? GetSystemTick() : ObjTrackInfo.m_dwFirstTime);
		if( S_OK != CheckPeccancyType(iOnePosLightScene, iTwoPosLightScene, rt, iRoadType, dwCaptureTime, &pt) )
		{
			pt = PT_NORMAL;
		}
		if(m_iFilterRushPeccancy  && (pt & PT_RUSH) != PT_RUSH)
		{
//			int ptTmp;
//			if( S_OK == CheckPeccancyType(iOnePosLightScene, ObjTrackInfo.m_iPassStopLightScene, rt, iRoadType, dwCaptureTime, &ptTmp) )
//			{
//				if((ptTmp & PT_RUSH) == PT_RUSH)
//				{
//					HV_Trace(1, "����ץ��ͼ�Ƶ�״̬��Ϊ���ִ�й���\n");
//				}
//			}
		}
		
		//����Ѿ��������������,���ҵ�����û����,�򴥷�������
		if( ObjTrackInfo.m_fFirstTrigger && ObjTrackInfo.m_fSecondTrigger && !ObjTrackInfo.m_fThirdTrigger && pt != PT_NORMAL )
		{
			if(GetSystemTick() > (DWORD32)m_sMinTriggerTimeMs)
			{
				m_dwTriggerTimes++;
				if(m_dwTriggerTimes >= 0xffff)
				{
					m_dwTriggerTimes = 1;

				}
				
			   	//DWORD32 dwRoadNum = (m_dwTriggerTimes << 16) | stCarLeftInfo.nVideoDetID;
				//m_pCallback->ThirdTrigger(m_LightType,dwRoadNum, NULL);
				m_sMinTriggerTimeMs = GetSystemTick() + 300;
				//m_dwLastTriggerInfo = dwRoadNum;
				//HV_Trace(1, "firecarleft:%d,%d, %08x\n", m_dwTriggerTimes, stCarLeftInfo.nVideoDetID, dwRoadNum);
			}
			else
			{
				//DWORD32 dwRoadNum = (m_dwLastTriggerInfo & 0xffff0000) | stCarLeftInfo.nVideoDetID;
				//HV_Trace(1, "ThirdTrigger Readd:%08x\n", dwRoadNum);
				//m_pCallback->ReaddSignal(dwRoadNum);
			}
			ObjTrackInfo.m_fThirdTrigger = true;
			ObjTrackInfo.m_dwThirdTime = m_dwTriggerTimes;
			ObjTrackInfo.m_dwThirdTimeMs = GetSystemTick();
		}
		// Τ�����޸� end
		pCurResult->ptType |= pt;

		//sprintf(g_szDebugInfo, "[FireNoPlateCarLeft]rtType=%d, ptType=%d, pt=%d", pCurResult->rtType, pCurResult->ptType, pt);
/*
		if( (pResultImage->pimgBeginCapture == NULL || pResultImage->pimgBestCapture == NULL || pResultImage->pimgLastCapture == NULL )
			|| pResultImage->pimgBeginCapture->GetRefTime() == pResultImage->pimgBestCapture->GetRefTime()
			|| pResultImage->pimgBeginCapture->GetRefTime() == pResultImage->pimgLastCapture->GetRefTime()
			|| pResultImage->pimgBestCapture->GetRefTime() == pResultImage->pimgLastCapture->GetRefTime() 
			)
		{
			//ͼƬ����Υ�¹涨
			pCurResult->iUnSurePeccancy = 1;
		}
		if( (pResultImage->pimgBeginCapture == NULL || pResultImage->pimgBestCapture == NULL || pResultImage->pimgLastCapture == NULL )
			|| TrackInfo.m_dwBeginCaptureRefTime == TrackInfo.m_dwBestCaptureRefTime
			|| TrackInfo.m_dwBeginCaptureRefTime == TrackInfo.m_dwLastCaptureRefTime
			|| TrackInfo.m_dwBestCaptureRefTime == TrackInfo.m_dwLastCaptureRefTime 
			)
		{
			//ͼƬ����Υ�¹涨
			pCurResult->iUnSurePeccancy = 1;
		}
*/
		//���˵�����������Υ��
		if( (pCurResult->ptType & PT_CONVERSE) == PT_CONVERSE
			&& ( (iFilter & RRT_CONVERSE) && rt == RT_CONVERSE) )
		{
			pCurResult->ptType &= ~PT_CONVERSE;
		}

		// zhaopy
		if (m_fFilterNoPlatePeccancy)
		{
			pCurResult->ptType = PT_NORMAL;
		}
		pCurResult->dwFirstTime = ObjTrackInfo.m_dwFirstTime;
		pCurResult->dwSecondTime = ObjTrackInfo.m_dwSecondTime;
		pCurResult->dwThirdTime = ObjTrackInfo.m_dwThirdTime;
		if(CTrackInfo::m_iFlashlightMode == 1 && m_iAvgY < CTrackInfo::m_iFlashlightThreshold)
		{
//			HV_Trace("Tracker:s=%d:f=%d:t=%d\n", pCurResult->dwFirstTime, pCurResult->dwSecondTime, pCurResult->dwThirdTime);
		}
	}
	else
	{
		pCurResult->rtType = RT_UNSURE;
		pCurResult->ptType = PT_NORMAL;
	}


	// �����ߴ�
	if (m_fOutputCarSize == TRUE)
	{
		pCurResult->cCarSize.iCarWidth = ObjTrackInfo.m_stCarInfo.fltCarW / 100;
		pCurResult->cCarSize.iCarHeight = ObjTrackInfo.m_stCarInfo.fltCarH / 100;

		if (m_fEnableScaleSpeed)
		{
			// ���б궨
			pCurResult->cCarSize.nOutType = 1;		// �?��)
			if ((pCurResult->cCarSize.iCarWidth < 1.4f) 
				|| (pCurResult->cCarSize.iCarHeight < 2.7f))
			{
				float fltTempS = 1.0f;
				float fltRand = (float)(rand() % 51) / 150;
				fltTempS += fltRand;

				if (ObjTrackInfo.m_nVotedCarType == CT_LARGE)
				{
					fltTempS *= 1.75f;
				}
				else if (ObjTrackInfo.m_nVotedCarType == CT_MID)
				{
					fltTempS *= 1.20f;
				}

				pCurResult->cCarSize.iCarWidth = 1.5f * fltTempS;
				pCurResult->cCarSize.iCarHeight = 3.4f * fltTempS;
			}
		}
		else
		{
			pCurResult->cCarSize.nOutType = 0;
			//�ޱ궨									// ���(����)
			if ((pCurResult->cCarSize.iCarWidth < m_nCarWBottom) 
				|| (pCurResult->cCarSize.iCarHeight < m_nCarLenBottom))
			{
				float fltTempS = 1.1f;
				float fltRand = (float)(rand() % 11) / 100;
				fltTempS += fltRand;

				if (ObjTrackInfo.m_nVotedCarType == CT_LARGE)
				{
					fltTempS *= 1.75f;
				}
				else if (ObjTrackInfo.m_nVotedCarType == CT_MID)
				{
					fltTempS *= 1.2f;
				}

				pCurResult->cCarSize.iCarWidth = m_nCarWBottom * fltTempS;
				pCurResult->cCarSize.iCarHeight = m_nCarLenBottom * fltTempS;
			}
		}
	}

	// ������Ŀ�޸ĳ�������
	if (m_iOutPutType == 1)
	{
		if ((pCurResult->nCarType == CT_SMALL)
			|| (pCurResult->nCarType == CT_MID)
			|| (pCurResult->nCarType == CT_LARGE))
		{
			pCurResult->nCarType = CT_VEHICLE;
		}
	}

	if (ObjTrackInfo.m_fIsTrigger)
	{
		pCurResult->dwTriggerIndex = ObjTrackInfo.m_dwTriggerIndex;
		pCurResult->iCapCount = ObjTrackInfo.m_iCapCount;
	}

//	RTN_HR_IF_FAILED( m_pCallback->CarLeft(
//		&stCarLeftInfo,
//		ResultImage.pimgBestSnapShot->GetFrameName()
//		));

	pProcessRespond->cTrigEvent.dwEventId |= EVENT_CARLEFT;
	pProcessRespond->cTrigEvent.iCarLeftCount++;

	return S_OK;
}

HRESULT CTrackerImpl::SetCarTypeParam(int nWidth, int nHeight)
{
	//m_nCarLenTop = 298;
	//m_nCarLenBottom = 695;

	m_nCarLenTop = 260;
	m_nCarLenBottom = 680;

	m_nCarWTop = 196;
	m_nCarWBottom = 570;

	if (nWidth == 1920)
	{
		// 1920 * 1200 ��ͼ��
		m_nCarLenTop = 266;
		m_nCarLenBottom = 534;

		m_nCarWTop = 202;
		m_nCarWBottom = 460;
	}
	else if (nWidth == 1600)
	{
		// ץ��ͼ1600 * 1200
		m_nCarLenTop = 420;
		m_nCarLenBottom = 690;

		m_nCarWTop = 320;
		m_nCarWBottom = 635;
	}

	{
		// û����Ƶ���٣�ʹ��Ĭ�ϵľ���ֵ��������б��
		m_fltSlopeH =  (float)(m_nCarLenBottom - m_nCarLenTop) / (float)(nHeight * 2 - m_nCarLenTop);
		m_fltSlopeW =  (float)(m_nCarWBottom - m_nCarWTop) / (float)(nHeight * 2);
	}

	return S_OK;
}

HRESULT CTrackerImpl::GetItgArea(CItgArea** ppItgArea, DWORD32 dwRefTime)
{
	if(m_nWidth == 0 || m_nHeight == 0)
	{
		*ppItgArea = NULL;
		return S_FALSE;
	}

	CRect rcMax(0, 0, m_nWidth, m_nHeight);
	CItgArea *pItgArea, *pCurArea;
	pCurArea = pItgArea = &m_cItgArea; //ȫ���
	if (m_nVideoDetMode == NO_VIDEODET)
	{
		CRect rcTrackArea;
		RTN_HR_IF_FAILED(MapParamRect(rcTrackArea, rcMax, m_pParam->g_rectTrackArea));
		CRect rcFullSearch;
		RTN_HR_IF_FAILED(MapParamRect(rcFullSearch, rcMax, m_pParam->g_rectDetectionArea));
		RTN_HR_IF_FAILED(m_cItgArea.SetRect(rcFullSearch, FALSE));

		//ȡ��ÿ��Tracker��Ԥ��λ��
		CItgArea *pTrackItgArea; 
		for(int i = 0; i < m_cTrackInfo; i++)
		{
			if( pTrackItgArea = (m_rgTrackInfo + i)->GetPredictArea(dwRefTime, rcFullSearch, rcTrackArea, m_pParam->m_iDetectReverseEnable, m_pScaleSpeed) )
			{
				pCurArea->SetNextArea(pTrackItgArea);
				pCurArea = pTrackItgArea;
			}
		}
	}
	else
	{
		CRect rcFullSearch;
		RTN_HR_IF_FAILED(MapParamRect(rcFullSearch, rcMax, m_rcVideoDetArea));
		RTN_HR_IF_FAILED(m_cItgArea.SetRect(rcFullSearch, FALSE));
	}
	//���̵�λ��
	if (m_pParam->m_fUsedTrafficLight)
	{
		for (int i = 0; i < m_nLightCount; i++)
		{
			//CRect rcLight = m_rgLightInfo[i].GetInflatedRect();
			CTrafficLightInfo* pTrafficLightInfo = m_cTrafficLight.GetTrafficLightInfo(i);
			if (pTrafficLightInfo == NULL) continue;
			CRect rcLight = pTrafficLightInfo->GetInflatedRect();
			if( rcLight.IntersectRect(rcMax, rcLight) )
			{
				m_rgLightArea[i].SetRect(rcLight, FALSE);
				pCurArea->SetNextArea(&m_rgLightArea[i]);
				pCurArea = &m_rgLightArea[i];
			}
		}
	}
	pCurArea->SetNextArea(NULL);
	*ppItgArea = pItgArea;

	return S_OK;
}

HRESULT CTrackerImpl::TrackingObj(PROCESS_ONE_FRAME_PARAM* pParam, HV_COMPONENT_IMAGE &imgCurImage, PROCESS_ONE_FRAME_RESPOND* pProcessRespond)
{
	CRect rcMax(0, 0, m_nWidth, m_nHeight);
	CRect rcFullSearch;
	RTN_HR_IF_FAILED(MapParamRect(rcFullSearch, rcMax, m_pParam->g_rectDetectionArea));
	CRect rcVideoDet;
	RTN_HR_IF_FAILED(MapParamRect(rcVideoDet, rcMax, m_rcVideoDetArea));
	
	//��Ƶ����
	for (int i = 0; i < m_cObjTrackInfo; i++)
	{
		m_rgObjTrackInfo[i].m_fTrack = FALSE;
	}

	bool fCalcCarType;
	if ((CObjectDetector::m_iProcessType == 1) || (CObjectDetector::m_iProcessType == 2))
	{
		// ������Ŀ
		fCalcCarType = m_rgiFiveCharWidth[39] || ((m_nCarLenTop > 0) && (m_nCarLenBottom > 0));
	}
	else
	{
		// Ĭ��
		fCalcCarType = (m_nCarLenTop > 0) && (m_nCarLenBottom > 0);
	}
	for (int i = 0; i < m_cDetected; i++)
	{
		int rgiPlateTrackID[MAX_PLATETRACK_COUNT];
		int cPlateTrackID(0);
		CObjInfo objInfo;
		objInfo.m_rcFG = m_rgrcDetected[i];
		objInfo.m_dwFrameTime = pParam->dwImageTime;
		objInfo.m_nFrameNo = pParam->dwFrameNo;
		objInfo.m_nCarType = CT_UNKNOWN;

		// zhaopy 0624
		bool fIsLightGroup = (i < m_iLightDetected);
		objInfo.m_fLight = fIsLightGroup;

		// ���㳵��
		if (fCalcCarType)
		{
			if ((CObjectDetector::m_iProcessType == 1) || (CObjectDetector::m_iProcessType == 2))
			{
				// ������Ŀ
				if (m_rgiFiveCharWidth[39])
				{
					int iOffset = m_rgrcDetected[i].bottom / 25;
					if (iOffset >= 0 && iOffset < 39)
					{
						//HV_Trace("CarWidth=%02f\n", (float)m_rgrcDetected[i].Width() / m_rgiFiveCharWidth[iOffset]);
						if (m_rgrcDetected[i].Width() < m_rgiFiveCharWidth[iOffset] * m_fltBikeSensitivity)
						{
							objInfo.m_nCarType = CT_WALKMAN;
						}
					}
				}
				else  //  zhaopy Ĭ���Ƿǻ�������0624
				{
					objInfo.m_nCarType = CT_WALKMAN;
				}

				if (objInfo.m_nCarType == CT_UNKNOWN)
				{
					int nYPos1 = m_nHeight >> 1;
					int nYPos2 = m_nHeight - 10;

					if ((m_rgrcDetected[i].bottom < nYPos2) 
						&& (m_rgrcDetected[i].bottom > (m_nCarLenTop>> 1))
						&& ((m_rgrcDetected[i].top > 10) || (m_rgrcDetected[i].bottom >= nYPos1)))
					{
						float fltCarLen = m_fltSlopeH * (m_rgrcDetected[i].bottom * 2 - m_nCarLenTop) + m_nCarLenTop;
						float fltThre1 = fltCarLen * 1.40f / 2;
						float fltThre2 = fltCarLen * 1.70f / 2;
						if (m_rgrcDetected[i].Height() < fltThre1)
						{
							objInfo.m_nCarType = CT_SMALL;
						}
						else if (m_rgrcDetected[i].Height() > fltThre2)
						{
							objInfo.m_nCarType = CT_LARGE;
						}
						else
						{
							objInfo.m_nCarType = CT_MID;
						}
					}
				}
			}
			else
			{
				// Ĭ��
				int nYPos1 = m_nHeight >> 1;
				int nYPos2 = m_nHeight - 10;

				if ((m_rgrcDetected[i].bottom < nYPos2) 
					&& (m_rgrcDetected[i].bottom > (m_nCarLenTop>> 1))
					&& ((m_rgrcDetected[i].top > 10) || (m_rgrcDetected[i].bottom >= nYPos1)))
				{
					float fltCarLen = m_fltSlopeH * (m_rgrcDetected[i].bottom * 2 - m_nCarLenTop) + m_nCarLenTop;
					float fltThre1 = fltCarLen * 1.25f / 2;
					float fltThre2 = fltCarLen * 1.70f / 2;
					if (m_rgrcDetected[i].Height() < fltThre1)
					{
						objInfo.m_nCarType = CT_SMALL;
					}
					else if (m_rgrcDetected[i].Height() > fltThre2)
					{
						objInfo.m_nCarType = CT_LARGE;
					}
					else
					{
						objInfo.m_nCarType = CT_MID;
					}
				}

				// ���㳵������
				if (m_fEnableScaleSpeed)
				{
					// ����Ƶ����
					objInfo.m_nCarW = int(100 * m_pScaleSpeed->CalcActureDistance(CPoint(m_rgrcDetected[i].left,
						m_rgrcDetected[i].bottom), CPoint(m_rgrcDetected[i].right, m_rgrcDetected[i].bottom)) / 1.0f); 
					objInfo.m_nCarH = int(100 * m_pScaleSpeed->CalcActureDistance(CPoint(m_rgrcDetected[i].left,
						m_rgrcDetected[i].top), CPoint(m_rgrcDetected[i].left, m_rgrcDetected[i].bottom)) / 2.5f);
				}
				else
				{
					int iTempCarW = m_rgrcDetected[i].right - m_rgrcDetected[i].left;
					int iTempCarH = (m_rgrcDetected[i].bottom - m_rgrcDetected[i].top) * 2;

					objInfo.m_nCarW = int(iTempCarW + m_fltSlopeW * (m_nHeight * 2 - iTempCarH));
					objInfo.m_nCarH = int(iTempCarH + m_fltSlopeH * (m_nHeight * 2 - iTempCarH));
				}
			}
		}

		//�����⵽���������ж��ٸ����Ƹ���
		for (int j = 0; j < m_cTrackInfo; j++)
		{
			if (m_rgTrackInfo[j].m_iVideoID != pParam->iVideoID ||
				m_rgTrackInfo[j].m_State == tsInit ||
				m_rgTrackInfo[j].m_State == tsWaitRemove ||
				!m_rgTrackInfo[j].m_fObservationExists)
			{
				continue;
			}
			if (m_rgrcDetected[i].IntersectsWith(m_rgTrackInfo[j].LastInfo().rcPos))
			{
				rgiPlateTrackID[cPlateTrackID++] = m_rgTrackInfo[j].m_nID;
				if (fCalcCarType)
				{
					// ���ӳ�����Ϣ
					m_rgTrackInfo[j].LastInfo().nCarType = objInfo.m_nCarType;

					if (m_fOutputCarSize == TRUE)
					{
						m_rgTrackInfo[j].LastInfo().stCarInfo.rcFG = objInfo.m_rcFG;

						m_rgTrackInfo[j].LastInfo().stCarInfo.fltCarW = (float)objInfo.m_nCarW;
						m_rgTrackInfo[j].LastInfo().stCarInfo.fltCarH = (float)objInfo.m_nCarH;

						//// �������
						//CRect rcTempCar = m_rgTrackInfo[j].LastInfo().stCarInfo.rcFG;
						//m_rgTrackInfo[j].LastInfo().stCarInfo.fltCarW = m_pScaleSpeed->CalcActureDistance(CPoint(rcTempCar.left,
						//	rcTempCar.bottom), CPoint(rcTempCar.right, rcTempCar.bottom));
						//m_rgTrackInfo[j].LastInfo().stCarInfo.fltCarH = m_pScaleSpeed->CalcActureDistance(CPoint(rcTempCar.left,
						//	rcTempCar.top), CPoint(rcTempCar.left, rcTempCar.bottom));
					} // if (m_fOutputCarSize)
					// TODO ���ݳ�������У�����͡�ͳ��
				}
			}
		}

		//�����⵽�����������ĸ���Ƶ����
		int iMaxArea = 0;
		int iSecondArea = 0;
		int iObjIndex(-1);
		int iObjSecondIndex = -1;

		for (int j = 0; j < m_cObjTrackInfo; j++)
		{
			if (m_rgObjTrackInfo[j].m_iVideoID == pParam->iVideoID)
			{
				HiVideo::CRect rcIntersect;
				if (rcIntersect.IntersectRect(&(m_rgObjTrackInfo[j].LastInfo().m_rcFG), &(objInfo.m_rcFG)))
				{
					int iInterArea = rcIntersect.Area();
					if (iInterArea > iMaxArea)
					{
						iObjIndex = j;
						iMaxArea = iInterArea;
					}
					else if( iInterArea > iSecondArea )
					{
						iObjSecondIndex = j;
						iSecondArea = iInterArea;
				}
			}
		}
		}
		// zhaopy 0624
		// �������еĳ��������е�ɾ����
		if( iObjIndex != -1  && iObjSecondIndex != -1 )
		{
			if( IsCanOutputReverseRun(&m_rgObjTrackInfo[iObjIndex])
				&& m_rgObjTrackInfo[iObjIndex].m_objTrackState == OBJ_TRACK_MOVING )
			{
				m_rgObjTrackInfo[iObjIndex].m_objTrackState = OBJ_TRACK_END;
				iObjIndex = -1;
			}
			if( IsCanOutputReverseRun(&m_rgObjTrackInfo[iObjSecondIndex])
				&& m_rgObjTrackInfo[iObjSecondIndex].m_objTrackState == OBJ_TRACK_MOVING )
			{
				m_rgObjTrackInfo[iObjSecondIndex].m_objTrackState = OBJ_TRACK_END;
				iObjSecondIndex = -1;
			}
			iObjIndex = (iObjIndex == -1 ? iObjSecondIndex : iObjIndex);
		}

		if (iObjIndex >= 0 && m_rgObjTrackInfo[iObjIndex].m_objTrackState != OBJ_TRACK_END)
		{
			if(S_OK == m_rgObjTrackInfo[iObjIndex].Process(objInfo, imgCurImage, rcVideoDet, m_iLastLightStatus, m_iCurLightStatus, pParam, pProcessRespond))
			{
			if (m_rgObjTrackInfo[iObjIndex].m_fTrack)
			{
				//m_rgObjTrackInfo[iObjIndex].RectifyRectBottom(rcMax.bottom, fLightIsNight);
				for (int k = 0; k < cPlateTrackID; k++)
				{
					if (!m_rgObjTrackInfo[iObjIndex].MatchPlateTrackID(rgiPlateTrackID[k]))
					{
						m_rgObjTrackInfo[iObjIndex].AddPlateTrackID(rgiPlateTrackID[k]);
					}
				}
			}

				if(CTrackInfo::m_iFlashlightMode == 1 && m_iAvgY < CTrackInfo::m_iFlashlightThreshold)
				{
					// zhaopy
					CheckObjectPeccancyTrigger(&m_rgObjTrackInfo[iObjIndex], iObjIndex);
				}
			}
		}

		CPoint ptTemp = m_rgrcDetected[i].CenterPoint();
		if (iObjIndex < 0 &&
			rcFullSearch.Contains(&ptTemp) &&
			(m_rgrcDetected[i].Height() < rcVideoDet.Height() * 0.5)
			|| GetCurrentParam()->g_PlateRcogMode == PRM_TOLLGATE )
		{
			//ֻ������������ڵ��ƶ����� weikt 2011-7-21
			if (m_cfgParam.cTrapArea.fEnableDetAreaCtrl && GetCurrentParam()->g_PlateRcogMode != PRM_TOLLGATE)
			{
				if (!IsContainOfTrap(m_cfgParam.cTrapArea, m_rgrcDetected[i]))
				{
					continue;
				}
			}
			//end

			if (m_cObjTrackInfo == s_knMaxObjCandidate)
			{
				m_cObjTrackInfo--;
			}
			m_rgObjTrackInfo[m_cObjTrackInfo].Clear(pProcessRespond);

			if ((CObjectDetector::m_iProcessType == 1) || (CObjectDetector::m_iProcessType == 2))
			{
				// ������Ŀ
				bool fIsManRoad(false);
				if (CTrackInfo::m_iRoadNumber > 2)
				{
					// ���㳵��
					int nTempRoad = MatchRoad(objInfo.m_rcFG.CenterPoint().x, objInfo.m_rcFG.CenterPoint().y);
					fIsManRoad = (CTrackInfo::m_iManRoadNum == nTempRoad);
				}

				RTN_HR_IF_FAILED(m_rgObjTrackInfo[m_cObjTrackInfo].New(pParam, objInfo, imgCurImage, m_iLastLightStatus, 
					m_iCurLightStatus, fIsManRoad, pProcessRespond));

			}
			else
			{
				// Ĭ��
				RTN_HR_IF_FAILED(m_rgObjTrackInfo[m_cObjTrackInfo].New(pParam, objInfo, imgCurImage,m_iLastLightStatus, m_iCurLightStatus, false, pProcessRespond));
			}

			for (int k = 0; k < cPlateTrackID; k++)
			{
				m_rgObjTrackInfo[m_cObjTrackInfo].AddPlateTrackID(rgiPlateTrackID[k]);
			}
			m_cObjTrackInfo++;
		}
	}

	for (int i = 0; i < m_cObjTrackInfo; i++)
	{
		bool fIsManRoad = m_rgObjTrackInfo[i].m_fIsMan;			// ���˵������ĸ���

		if (m_rgObjTrackInfo[i].m_iVideoID != pParam->iVideoID ||
			m_rgObjTrackInfo[i].m_objTrackState == OBJ_TRACK_INIT)
		{
			continue;
		}
		if (!m_rgObjTrackInfo[i].m_fTrack)
		{
			m_rgObjTrackInfo[i].m_iMissingCount++;
		}
		else
		{
			if (m_rgObjTrackInfo[i].LastInfo().m_rcFG == CRect(0, 0, 0, 0))
			{
				m_rgObjTrackInfo[i].m_iMissingCount++;
				m_rgObjTrackInfo[i].LastInfo().Clear();
				m_rgObjTrackInfo[i].m_iObjInfoCount--;
			}
			else
			{
				m_rgObjTrackInfo[i].m_iMissingCount = 0;
			}
		}
		switch (m_rgObjTrackInfo[i].m_objTrackState)
		{
		case OBJ_TRACK_NEW:
			if (!fIsManRoad)
			{
				// �������������ĸ���
				if (m_rgObjTrackInfo[i].m_iMissingCount >= 2)
				{
					m_rgObjTrackInfo[i].m_objTrackState = OBJ_TRACK_END;
				}
				else if (m_rgObjTrackInfo[i].m_iObjInfoCount >= 3)
				{
					m_rgObjTrackInfo[i].m_objTrackState = OBJ_TRACK_MOVING;
				}
			}
			else
			{
				// ���˵������ĸ���
				if (m_rgObjTrackInfo[i].m_iMissingCount >= WALK_MAN_MISSING_TIME)
				{
					m_rgObjTrackInfo[i].m_objTrackState = OBJ_TRACK_END;
				}
				else if (m_rgObjTrackInfo[i].m_iObjInfoCount >= 2)
				{
					m_rgObjTrackInfo[i].m_objTrackState = OBJ_TRACK_MOVING;
				}
			}
			break;
		case OBJ_TRACK_MOVING:
			if (!fIsManRoad)
			{
				// �������������ĸ���
				if (m_rgObjTrackInfo[i].m_iMissingCount >= 3)
				{
					m_rgObjTrackInfo[i].m_objTrackState = OBJ_TRACK_END;
				}
			}
			else
			{
				// ���˵������ĸ���
				if (m_rgObjTrackInfo[i].m_iMissingCount >= WALK_MAN_MISSING_TIME)
				{
					m_rgObjTrackInfo[i].m_objTrackState = OBJ_TRACK_END;
				}
			}
			break;
		}
	}

	//����ص���Trackֻ����һ��
	for (int i = 0; i < m_cObjTrackInfo; i++)
	{
		if (m_rgObjTrackInfo[i].m_objTrackState == OBJ_TRACK_INIT ||
			!m_rgObjTrackInfo[i].m_fTrack)
		{
			continue;
		}
		for (int j = i + 1; j < m_cObjTrackInfo; j++)
		{
			if (m_rgObjTrackInfo[j].m_objTrackState == OBJ_TRACK_INIT ||
				!m_rgObjTrackInfo[j].m_fTrack)
			{
				continue;
			}
			if (m_rgObjTrackInfo[i].LastInfo().m_rcFG == m_rgObjTrackInfo[j].LastInfo().m_rcFG)
			{

				if (m_rgObjTrackInfo[i].CheckImageClear() && !m_rgObjTrackInfo[j].CheckImageClear() )
				{
					m_rgObjTrackInfo[i].Clear(pProcessRespond);
				}
				else if( !m_rgObjTrackInfo[i].CheckImageClear() && m_rgObjTrackInfo[j].CheckImageClear() )
				{
					m_rgObjTrackInfo[j].Clear(pProcessRespond);
				}
				else if (m_rgObjTrackInfo[i].m_iObjInfoCount >= m_rgObjTrackInfo[j].m_iObjInfoCount)
				{
					m_rgObjTrackInfo[j].Clear(pProcessRespond);
				}
				else
				{
					m_rgObjTrackInfo[i].Clear(pProcessRespond);
				}
/*
				if (m_rgObjTrackInfo[i].m_iObjInfoCount >= m_rgObjTrackInfo[j].m_iObjInfoCount)
				{
					m_rgObjTrackInfo[j].Clear(pProcessRespond);
				}
				else
				{
					m_rgObjTrackInfo[i].Clear(pProcessRespond);
				}
*/
			}
		}
	}
	return S_OK;
}

bool CTrackerImpl::IsCanOutputReverseRun(CObjTrackInfo* pObj)
{
	bool fRet = false;
	if( pObj == NULL ) return fRet;

	CRect rcMax(0, 0, m_nWidth, m_nHeight);
	CRect rcVideoDetArea;
	RTN_HR_IF_FAILED(MapParamRect(rcVideoDetArea, rcMax, m_rcVideoDetArea));

	CRect rcFirstFG = pObj->m_rgObjInfo[0].m_rcFG;
	CRect rcLastFG = pObj->LastInfo().m_rcFG;

	int iVoteDistance = rcVideoDetArea.Height() * 5 / 10;
	int iDistance = 0;
	switch (m_pParam->g_MovingDirection)
	{
	case MD_TOP2BOTTOM:
		iDistance = rcLastFG.CenterPoint().y - rcFirstFG.CenterPoint().y;
		if( iDistance < 0 && abs(iDistance) > iVoteDistance )
		{
			fRet = true;
		}
		break;
	case MD_BOTTOM2TOP:	
		iDistance = rcFirstFG.CenterPoint().y - rcLastFG.CenterPoint().y;
		if( iDistance < 0 && abs(iDistance) > iVoteDistance )
		{
			fRet = true;
		}
		break;
	}

	return fRet;
}

HRESULT CTrackerImpl::ShowObjTracks(
	PROCESS_ONE_FRAME_PARAM* pParam, 
	int iVideoID,
	PROCESS_ONE_FRAME_RESPOND* pProcessRespond
	)
{
//	if (NULL == pCurImage)
//	{
//		return S_FALSE;
//	}
//	if (!m_pCallback)
//	{
//		return S_FALSE;
//	}
	CRect rcMax(0, 0, m_nWidth, m_nHeight);
	CRect rcSearchArea;
	CRect rcVideoDetArea;
	int iVoteDistance;
	RTN_HR_IF_FAILED(MapParamRect(rcSearchArea, rcMax, m_pParam->g_rectDetectionArea));
	RTN_HR_IF_FAILED(MapParamRect(rcVideoDetArea, rcMax, m_rcVideoDetArea));
	for (int i = 0; i < m_cObjTrackInfo; i++)
	{
		//��ǰ���ڸ��ٳ��Ƶ�����
		bool fPlateTracking = false;
		for (int j = 0; j < m_cTrackInfo; j++)
		{
			if (m_rgTrackInfo[j].m_State == tsInit ||
				m_rgTrackInfo[j].m_State == tsWaitRemove)
			{
				continue;
			}
			if (m_rgObjTrackInfo[i].MatchPlateTrackID(m_rgTrackInfo[j].m_nID))
			{
				fPlateTracking = true;
				break;
			}
		}
		// �շ�վ�й��Ƶ�һֱ���Ϊ����
		if (GetCurrentParam()->g_PlateRcogMode == PRM_TOLLGATE)
		{
			if (m_rgObjTrackInfo[i].m_cPlateTrackID != 0)
			{
				fPlateTracking = true;
			}
		}

		//������������ź�
		if (m_rgObjTrackInfo[i].m_objTrackState == OBJ_TRACK_MOVING &&
			!m_rgObjTrackInfo[i].m_fCarArrived &&
			!fPlateTracking &&
			(m_rgObjTrackInfo[i].m_iObjInfoCount >= 5 && GetCurrentParam()->g_PlateRcogMode != PRM_TOLLGATE
			|| m_rgObjTrackInfo[i].m_iObjInfoCount >= 4 && GetCurrentParam()->g_PlateRcogMode == PRM_TOLLGATE))
		{
			// zhaopy todo... ���еĳ���Ҫ��һʱ��ץ�ģ���Ȼ����ȥ̫Զ��ʱ�����ῴ��ȫ��
			CRect rcPos = m_rgObjTrackInfo[i].LastInfo().m_rcFG;
			//int iPosY = m_iCarArrivedPos * m_nHeight / 100;
			int iPosY = m_iCarArrivedPosNoPlate * m_nHeight / 100;
			// zhaopy 0624
			bool fIsMoving = false;
			int iMove = abs(m_rgObjTrackInfo[i].LastInfo().m_rcFG.CenterPoint().y - m_rgObjTrackInfo[i].m_rgObjInfo[0].m_rcFG.CenterPoint().y);
			if( iMove > (m_nHeight * 10 / 100) ) fIsMoving = true;
			if ( m_pParam->g_MovingDirection == MD_TOP2BOTTOM && rcPos.bottom > iPosY)
			{
				m_rgObjTrackInfo[i].m_nCarArrivedCount++;
				if (m_rgObjTrackInfo[i].m_nCarArrivedCount > 1 
					&& (fIsMoving || GetCurrentParam()->g_PlateRcogMode == PRM_TOLLGATE))
				{
					//֪ͨ��������
					// zhaopy 0624
					if ( m_iAvgY < 20 
						&& CObjectDetector::m_iNightPlus == 1 
						&& CObjectDetector::m_iProcessType == 2 )
					{
						if (!RainyFlashControl(&rcPos, iVideoID))
						{
							m_rgObjTrackInfo[i].m_nCarArrivedCount = 0;
							continue;
						}
					}

					ProcessNoPlateCarArrive(&m_rgObjTrackInfo[i], pParam->dwImageTime, 0.0, pProcessRespond);
				}
			}
			else if( m_pParam->g_MovingDirection == MD_BOTTOM2TOP &&  rcPos.bottom < iPosY)
			{
				m_rgObjTrackInfo[i].m_nCarArrivedCount++;
				if (m_rgObjTrackInfo[i].m_nCarArrivedCount > 1 && fIsMoving)
				{
					//֪ͨ��������
					ProcessNoPlateCarArrive(&m_rgObjTrackInfo[i], pParam->dwImageTime, 0.0, pProcessRespond);
				}
			}
		}
		//������Ƴ��ź�
		//�羯�����ģ���ٵĽ����������Ƴ��?
		BOOL fIsEPMode = false;
        if(1 == m_iNoPlateDetMode)
        {
            fIsEPMode = true;
        }
        
		if ((GetCurrentParam()->g_PlateRcogMode == PRM_TOLLGATE && m_rgObjTrackInfo[i].m_objTrackState == OBJ_TRACK_MOVING
				|| m_rgObjTrackInfo[i].m_objTrackState == OBJ_TRACK_END)
				&& (GetCurrentParam()->g_PlateRcogMode != PRM_TOLLGATE
				|| m_rgObjTrackInfo[i].LastInfo().m_rcFG.bottom > m_iCarArrivedPos * m_nHeight / 100)
				&&
			!m_rgObjTrackInfo[i].m_fVoted &&
			!fPlateTracking &&
			m_rgObjTrackInfo[i].m_iObjInfoCount >= 6 && (!fIsEPMode))
		{
			CRect rcFirstFG = m_rgObjTrackInfo[i].m_rgObjInfo[0].m_rcFG;
			CRect rcLastFG = m_rgObjTrackInfo[i].LastInfo().m_rcFG;

			int nObjCountTemp(50000);

			if ((CObjectDetector::m_iProcessType == 1) || (CObjectDetector::m_iProcessType == 2))
			{
				// ������Ҫ������������ж�
				nObjCountTemp = m_iVoteFrameCount;
			}

			// �����������
			if (m_rgObjTrackInfo[i].m_iObjInfoCount > nObjCountTemp)
			{
				int iDistance;
				iDistance = rcLastFG.CenterPoint().y - rcFirstFG.CenterPoint().y;
				int iVoteDistance = rcVideoDetArea.Height() * 25 / 100;
				if ( (m_pParam->g_MovingDirection == MD_TOP2BOTTOM && iDistance < 0)
					|| (m_pParam->g_MovingDirection == MD_BOTTOM2TOP && iDistance > 0) )
				{
					iDistance = abs(iDistance);
					iVoteDistance >>= 1;
				}

				//����λ��
				int iDistanceX = abs(rcLastFG.CenterPoint().x - rcFirstFG.CenterPoint().x);
				int iVoteDistanceX = rcVideoDetArea.Width() * 50 / 100;

				
				if(iDistance >= iVoteDistance
					|| (iDistanceX > iVoteDistanceX && m_iCheckAcross == 1) )
				{
					//���ӳ��п�����ҹ����Ķ��
					// zhaopy 0624
					BOOL fRainyOutPut = true;
					if( m_LightType == NIGHT 
						&& CObjectDetector::m_iProcessType == 2 
						&& CObjectDetector::m_iNightPlus == 1 
						&& m_pParam->g_MovingDirection == MD_TOP2BOTTOM)
					{

						fRainyOutPut = RainyObjOutPutControl(&rcLastFG, i);
					}

					if (!m_rgObjTrackInfo[i].m_fCarArrived && fRainyOutPut)
					{
						//ͨ��������?
						ProcessNoPlateCarArrive(&m_rgObjTrackInfo[i], pParam->dwImageTime, 0.0, pProcessRespond);
					}

					if( fRainyOutPut )
					{
					m_rgObjTrackInfo[i].m_fVoted = true;
					FireNoPlateCarLeftEvent(pParam, m_rgObjTrackInfo[i], pProcessRespond);
					}
					
					//HV_Trace("TrackObjCount:%d, Dis:%d, Vote:%d\n", m_rgObjTrackInfo[i].m_iObjInfoCount, iDistance, iVoteDistance);
				}
				else
				{
					//HV_Trace("ERROR: TrackObjCount:%d, Dis:%d, Vote:%d\n", m_rgObjTrackInfo[i].m_iObjInfoCount, iDistance, iVoteDistance);
				}
			}
			else
			{
				int iDistance;
				MovingDirection movingDirection;
				bool fMove = false;
				bool fTollGateOut = false;

				switch (m_pParam->g_MovingDirection)
				{
				case MD_TOP2BOTTOM:
//					iDistance = rcLastFG.CenterPoint().y - rcFirstFG.CenterPoint().y;
					iDistance = MAX_INT((rcLastFG.bottom - rcFirstFG.bottom),
						rcLastFG.CenterPoint().y - rcFirstFG.CenterPoint().y);
					if (m_LightType == NIGHT && rcFirstFG.top < (rcVideoDetArea.bottom >> 3)
						&& ((rcFirstFG.CenterPoint().x > rcVideoDetArea.right * 3 / 4 
						&& rcLastFG.CenterPoint().x > rcVideoDetArea.right * 3 / 4)
						|| (rcFirstFG.CenterPoint().x < rcVideoDetArea.right / 4
						&& rcLastFG.CenterPoint().x > rcVideoDetArea.right / 4)))	// �������ߵĳ�����ֵ����
					{
						iVoteDistance = rcVideoDetArea.Height() * 4 / 10;
					}
					else
					{
						iVoteDistance = rcVideoDetArea.Height() * 5 / 10;
					}
					if (iDistance < 0)
					{
						iDistance = -iDistance;
						movingDirection = MD_BOTTOM2TOP;
						if(CObjectDetector::m_iNightPlus == 1)
						{
							iVoteDistance >>= 1;
						}
					}
					else
					{
						movingDirection = MD_TOP2BOTTOM;
					}
                    static unsigned int s_LastOutNoPlateTime = 0;


                    if (GetCurrentParam()->g_PlateRcogMode == PRM_TOLLGATE)
                    {
                    	bool fMoving = false;
                    	// ���Ƴ�Moving״̬����ͣ�����Ÿ������
                    	if (m_rgObjTrackInfo[i].m_objTrackState == OBJ_TRACK_MOVING)
                    	{
                    		CRect rcPos = m_rgObjTrackInfo[i].LastInfo().m_rcFG;
        					CRect rcPosPre = m_rgObjTrackInfo[i].m_rgObjInfo[m_rgObjTrackInfo[i].m_iObjInfoCount-3].m_rcFG;
        					if (rcPosPre.IntersectRect(&rcPosPre, &rcPos))
        					{
        						if (rcPosPre.Area() * 100 < rcPos.Area() * 95)  // �߶��ཻ
        						{
        							fMoving = true;
        						}
        					}
                    	}
                    	if (!fMoving && m_rgObjTrackInfo[i].m_rgObjInfo[0].m_rcFG.bottom < (m_iCarArrivedPos + 15) * m_nHeight / 100
								&& m_rgObjTrackInfo[i].m_rgObjInfo[0].m_rcFG.bottom + 3 * m_nHeight / 100 < m_rgObjTrackInfo[i].LastInfo().m_rcFG.bottom
								&& Venus_GetSystemTick() - s_LastOutNoPlateTime > 1000 * 10)   // 10s
						{
							fTollGateOut = true;
						}
                    }

					if ( fTollGateOut ||
							((GetCurrentParam()->g_PlateRcogMode != PRM_TOLLGATE) &&
							 iDistance > iVoteDistance &&
							((movingDirection == MD_TOP2BOTTOM && rcSearchArea.top <= rcLastFG.top) ||
									(movingDirection == MD_BOTTOM2TOP && rcSearchArea.bottom >= rcLastFG.bottom))))
					{
						//���ӳ��п�����ҹ����Ķ��
						// zhaopy 0624
						BOOL fRainyOutPut = true;
						if( m_LightType == NIGHT 
							&& CObjectDetector::m_iProcessType == 2 
							&& CObjectDetector::m_iNightPlus == 1 
							&& movingDirection == MD_TOP2BOTTOM)
						{

							fRainyOutPut = RainyObjOutPutControl(&rcLastFG, i);

						}
						if (!m_rgObjTrackInfo[i].m_fCarArrived && fRainyOutPut)
						{
							//֪ͨ��������
							ProcessNoPlateCarArrive(&m_rgObjTrackInfo[i], pParam->dwImageTime, 0.0, pProcessRespond);
						}

						if( fRainyOutPut )
						{
						m_rgObjTrackInfo[i].m_fVoted = true;
						FireNoPlateCarLeftEvent(pParam, m_rgObjTrackInfo[i], pProcessRespond);
						s_LastOutNoPlateTime = Venus_GetSystemTick();  //��¼�ϴ����Ƴ���ʱ��
						}
						
					}
					break;
				case MD_BOTTOM2TOP:
					iDistance = rcLastFG.CenterPoint().y - rcFirstFG.CenterPoint().y;
					iVoteDistance =rcVideoDetArea.Height() * 5 / 10;
					if (iDistance < 0)
					{
						iDistance = -iDistance;
						movingDirection = MD_BOTTOM2TOP;						
					}
					else
					{
						movingDirection = MD_TOP2BOTTOM;
						if(CObjectDetector::m_iNightPlus == 1)
						{
							iVoteDistance >>= 1;
						}
					}
					//2011-3-15 Τ���� �羯ģʽ��ȥ���󳵳�����
					if( m_pParam->m_fUsedTrafficLight && m_pParam->g_MovingDirection == MD_BOTTOM2TOP && movingDirection == MD_BOTTOM2TOP && m_fFilterMoreReview )
					{						
						if(iDistance > iVoteDistance && FAILED(CheckNoPlate(&m_rgObjTrackInfo[i])))
						{
							fMove = true;
							//HV_Trace("Filter More Review...\n");
						}
					}

					if (iDistance > iVoteDistance && !fMove &&
						((movingDirection == MD_TOP2BOTTOM && rcSearchArea.top <= rcLastFG.top) ||
						(movingDirection == MD_BOTTOM2TOP && rcSearchArea.bottom >= rcLastFG.bottom)))
					{
						if (!m_rgObjTrackInfo[i].m_fCarArrived)
						{
							//֪ͨ��������
							ProcessNoPlateCarArrive(&m_rgObjTrackInfo[i], pParam->dwImageTime, 0.0, pProcessRespond);
						}
						m_rgObjTrackInfo[i].m_fVoted = true;
						FireNoPlateCarLeftEvent(pParam, m_rgObjTrackInfo[i], pProcessRespond);
					}
					break;
				case MD_LEFT2RIGHT:
				case MD_RIGHT2LEFT:
					iDistance = rcLastFG.CenterPoint().x - rcFirstFG.CenterPoint().x;
					if (iDistance < 0)
					{
						iDistance = -iDistance;
						movingDirection = MD_RIGHT2LEFT;
					}
					else
					{
						movingDirection = MD_LEFT2RIGHT;
					}
					iVoteDistance = rcVideoDetArea.Width() * 5 / 10;
					if (iDistance > iVoteDistance &&
						((movingDirection == MD_LEFT2RIGHT && rcSearchArea.left <= rcLastFG.left) ||
						(movingDirection == MD_RIGHT2LEFT && rcSearchArea.right >= rcLastFG.right)))
					{
						m_rgObjTrackInfo[i].m_fVoted = true;
						FireNoPlateCarLeftEvent(pParam, m_rgObjTrackInfo[i], pProcessRespond);
					}
					break;
				}
			}
		}
	}


	//�羯���Ƴ������ĸ��ٳ����Լ�����ץ�ĵ�����
    //����Ӧ������һ������
    if(1 == m_iNoPlateDetMode)
    {
        for (int i = 0; i < m_cEPObjTrackInfo; i++)
        {
			/*char szInfo[32] = {0};
			sprintf(szInfo, "S:%d A:%d C:%d N:%d\n"
			       ,m_rgEPObjTrackInfo[i].m_objTrackState
			       ,m_rgEPObjTrackInfo[i].m_fCarArrived
			       ,m_rgEPObjTrackInfo[i].m_iObjInfoCount
			       ,m_rgEPObjTrackInfo[i].m_fNewTem
			       );
			strcat(g_szDebugInfo, szInfo);*/
            //������������ź�
            if (m_rgEPObjTrackInfo[i].m_objTrackState == OBJ_TRACK_TEM&&
                !m_rgEPObjTrackInfo[i].m_fCarArrived && m_rgEPObjTrackInfo[i].m_fEPOutput)
            {
                CRect rcPos = m_rgEPObjTrackInfo[i].LastInfo().m_rcFG;
                //int iPosY = m_iCarArrivedPos * m_nHeight / 100;
                int iPosY = m_iCarArrivedPosNoPlate * m_nHeight / 100;
                if(m_pParam->g_MovingDirection == MD_BOTTOM2TOP && rcPos.bottom < iPosY)
                {
                    m_rgEPObjTrackInfo[i].m_nCarArrivedCount++;
                    if (m_rgEPObjTrackInfo[i].m_nCarArrivedCount > 1)
                    {
                        //֪ͨ��������
                        ProcessNoPlateCarArrive(&m_rgEPObjTrackInfo[i], pParam->dwImageTime, 0.0, pProcessRespond);
                    }
                }
            }
            //������ٽ����ĳ����ź�
            if (m_rgEPObjTrackInfo[i].m_objTrackState == OBJ_TRACK_END 
                && !m_rgEPObjTrackInfo[i].m_fVoted 
                && m_rgEPObjTrackInfo[i].m_fEPOutput
                && m_rgEPObjTrackInfo[i].m_iObjInfoCount >= 6  
                && m_rgEPObjTrackInfo[i].m_fNewTem
                )
            {
                CRect rcFirstFG = m_rgEPObjTrackInfo[i].m_rgObjInfo[0].m_rcFG;
                CRect rcLastFG = m_rgEPObjTrackInfo[i].LastInfo().m_rcFG;
                int iDistance;
                int iTemDis, iVoteTemDis;
                MovingDirection movingDirection;
                bool fMove = false;
                switch (m_pParam->g_MovingDirection)
                {
                case MD_BOTTOM2TOP:
                    iDistance = rcLastFG.CenterPoint().y - rcFirstFG.CenterPoint().y;
                    iTemDis = rcLastFG.CenterPoint().y - m_rgEPObjTrackInfo[i].m_iTemYBegin;
                    iVoteDistance = rcVideoDetArea.Height() >> 1;
                    //iVoteDistance = rcVideoDetArea.Height() * 4 / 10;
                    iVoteTemDis = rcVideoDetArea.Height()*4/10;
                    if (iDistance < 0)
                    {
                        iDistance = -iDistance;
                        movingDirection = MD_BOTTOM2TOP;						
                    }
                    else
                    {
                        movingDirection = MD_TOP2BOTTOM;
                    }

                    if (iTemDis > 0)continue;
                    if (iTemDis < 0)iTemDis = -iTemDis;


                    //HV_Trace("\n id:[%d],dis:[%d]",  m_rgEPObjTrackInfo[i].m_Feature.iObjId, iDistance);
                    if ((iDistance > iVoteDistance || iTemDis > iVoteTemDis) &&
                    	((movingDirection == MD_BOTTOM2TOP && rcSearchArea.bottom >= rcLastFG.bottom)))
                    {
                        if (!m_rgEPObjTrackInfo[i].m_fCarArrived)
                        {
                            //֪ͨ��������
                            ProcessNoPlateCarArrive(&m_rgEPObjTrackInfo[i], pParam->dwImageTime, 0.0, pProcessRespond);
                        }
                        m_rgEPObjTrackInfo[i].m_fVoted = true;
                        FireNoPlateCarLeftEvent(pParam, m_rgEPObjTrackInfo[i], pProcessRespond);
                    }
                    break;
                }
            }
        }
    }
        
	return S_OK;
}

HRESULT CTrackerImpl::TrackingObjByTem(PROCESS_ONE_FRAME_PARAM* pParam, HV_COMPONENT_IMAGE &imgCurImage, PROCESS_ONE_FRAME_RESPOND* pProcessRespond)
{
    CRect rcMax(0, 0, m_nWidth, m_nHeight);
    CRect rcFullSearch;
    RTN_HR_IF_FAILED(MapParamRect(rcFullSearch, rcMax, m_pParam->g_rectDetectionArea));
    CRect rcVideoDet;
    RTN_HR_IF_FAILED(MapParamRect(rcVideoDet, rcMax, m_rcVideoDetArea));

    //��Ƶ����
    for (int i = 0; i < m_cEPObjTrackInfo; i++)
    {
        m_rgEPObjTrackInfo[i].m_fTrack = FALSE;
    }
    
    //�����µĸ���
    
    //for (int i = 0; i < m_cDetected; i++)
    for (int i = 0; i < m_cScanEP; i++)
    {
        CObjInfo objInfo;
        objInfo.m_rcFG = m_rgrcScanEP[i];
        //����
        objInfo.m_dwFrameTime = pParam->dwImageTime;
        objInfo.m_nFrameNo = pParam->dwFrameNo;
        objInfo.m_nCarType = CT_UNKNOWN;
        
        //�ж��Ƿ��뵱ǰģ����ٵ������ཻ
        bool fInterSet = false;
        if(!fInterSet)
        {
            for (int j = 0; j < m_cEPObjTrackInfo; j++)
            {
                if (m_rgEPObjTrackInfo[j].m_objTrackState == OBJ_TRACK_TEM)
                {
                    HiVideo::CRect rInterSet;
                    HiVideo::CRect lastRect = m_rgEPObjTrackInfo[j].LastInfo().m_rcFG;
                    if(rInterSet.IntersectRect(lastRect, objInfo.m_rcFG))
                    {
                        fInterSet = true;
                    }
                }
            }
        }
         //
        if(!fInterSet)
        {
            for (int j = 0; j < m_cTrackInfo; j++)
            {
                if (m_rgTrackInfo[j].m_iVideoID != pParam->iVideoID )
                {
                    continue;
                }

				if( m_rgTrackInfo[j].m_State == tsNew 
				||	m_rgTrackInfo[j].m_State == tsWaitRemove)continue;
                //TODO: �Ժ�Ҳ�����ݳ��Ƶĸ�����Ϣ���������Ƴ��ĸ��ٽ���
                if (m_rgrcScanEP[i].IntersectsWith(m_rgTrackInfo[j].LastInfo().rcPos))
                {
                    fInterSet = true;
                    break;
                }
            }
        }
         
        if (fInterSet)continue;
        

        if (m_cEPObjTrackInfo == s_knMaxObjCandidate)
        {
            m_cEPObjTrackInfo--;
        }
        m_rgEPObjTrackInfo[m_cEPObjTrackInfo].Clear(pProcessRespond);

        int iMaxArea = 0;
        int iObjIndex(-1);
        for (int j = 0; j < m_cEPObjTrackInfo; j++)
        {
            if (m_rgEPObjTrackInfo[j].m_iVideoID == pParam->iVideoID && m_rgEPObjTrackInfo[j].m_objTrackState != OBJ_TRACK_TEM)
            {
                HiVideo::CRect rcIntersect;
                if (rcIntersect.IntersectRect(&(m_rgEPObjTrackInfo[j].LastInfo().m_rcFG), &(objInfo.m_rcFG)))
                {
                    int iInterArea = rcIntersect.Area();
                    if (iInterArea > iMaxArea)
                    {
                        iObjIndex = j;
                        iMaxArea = iInterArea;
                    }
                }
            }
        }
        if(iObjIndex>=0)
        {

            if (m_rgEPObjTrackInfo[iObjIndex].m_objTrackState != OBJ_TRACK_TEM)
            {
                m_rgEPObjTrackInfo[iObjIndex].Process(objInfo, imgCurImage, rcVideoDet, m_iLastLightStatus, m_iCurLightStatus, pParam, pProcessRespond);
            } 
        } else 
        {
            //��������κε�ģ������ཻ
            //CPoint ptTemp = m_rgrcDetected[i].CenterPoint();
            //CRect r = m_rgrcDetected[i];
            CPoint ptTemp = m_rgrcScanEP[i].CenterPoint();
            CRect r = m_rgrcScanEP[i];
            CRect rInter(0,0,0,0);
            BOOL fHasTem = false;
            for (int k = 0; k < m_cEPObjTrackInfo; k++)
            {
                if(m_rgEPObjTrackInfo[k].m_objTrackState != OBJ_TRACK_TEM)continue;
                CRect rTem = m_rgEPObjTrackInfo[k].LastInfo().m_rcFG;
                if (rInter.IntersectRect(r, rTem))
                {
                    fHasTem = true;
                    break;
                } 
            }

            if(iObjIndex < 0 && rcFullSearch.Contains(&ptTemp) && !fHasTem)
            {
                //���ٽ���
                
                if (m_cEPObjTrackInfo == s_knMaxObjCandidate)
                {
                    m_cEPObjTrackInfo--;
                }
                m_rgEPObjTrackInfo[m_cEPObjTrackInfo].Clear(pProcessRespond);
                RTN_HR_IF_FAILED(m_rgEPObjTrackInfo[m_cEPObjTrackInfo].New(pParam, objInfo, imgCurImage, m_iLastLightStatus, m_iCurLightStatus, false, pProcessRespond));
            }
        }

    //�⼸��д����ɶ��Ӧ�õĴ����ȱ���
    /*for (int k = 0; k < cPlateTrackID; k++)
    {
        m_rgEPObjTrackInfo[m_cEPObjTrackInfo].AddPlateTrackID(rgiPlateTrackID[k]);
    }*/
    m_cEPObjTrackInfo++;
    
    }
    //���еĸ���
    //��ʾԤ�����
    //m_RectNum[3] = 0;
    for(int i = 0; i < m_cEPObjTrackInfo; i++)
    {
        if(m_rgEPObjTrackInfo[i].m_objTrackState == OBJ_TRACK_TEM)
        {
            //���Ƴ���ģ����٣�Ԥ������
            CRect rPredict = GetEPNoPlatePredictRect(pParam->dwImageTime, m_rgEPObjTrackInfo + i);
            //HV_Trace("\nrPredict.top:[%d], rPredict.left:[%d], rPredict.bottom:[%d], rPredict.top:[%d], area:[%d]", rPredict.top, rPredict.left, rPredict.bottom, rPredict.right, (rPredict.bottom - rPredict.top)*(rPredict.right - rPredict.left));
            rPredict.IntersectRect(&rPredict, &rcMax);
            //��ʾԤ�����
            //m_DrawRect[3][m_RectNum[3]] = rPredict;
            //m_RectNum[3]++;
            //if(m_RectNum[3] >= DRAW_RECT_LEN)m_RectNum[3] = DRAW_RECT_LEN - 1;
            //DWORD32 dwBeginTick = GetSystemTick();

			// zhaopy Ԥ���������ʱ������Ч�ġ�Ԥ�⺯��Ӧ�������⡣
			if( !rPredict.IsRectEmpty() )
			{
				RTN_HR_IF_FAILED(m_rgEPObjTrackInfo[i].ProcessEx(pParam, imgCurImage, rcVideoDet, rPredict, m_pParam->m_iDetectReverseEnable, m_iLastLightStatus, m_LightType, pProcessRespond));
			}

			// RTN_HR_IF_FAILED(m_rgEPObjTrackInfo[i].ProcessEx(pParam, imgCurImage, rcVideoDet, rPredict, m_pParam->m_iDetectReverseEnable, m_iLastLightStatus, m_LightType, pProcessRespond));
			//HV_Trace("Perpare ProcessEx time = %d\n", GetSystemTick() - dwBeginTick);
        }
    }

    //����״̬ת��
    for (int i = 0; i < m_cEPObjTrackInfo; i++)
    {
        //bool fIsManRoad = m_rgEPObjTrackInfo[i].m_fIsMan;			// ���˵���������?

        if (m_rgEPObjTrackInfo[i].m_iVideoID != pParam->iVideoID ||
            m_rgEPObjTrackInfo[i].m_objTrackState == OBJ_TRACK_INIT)
        {
            continue;
        }
        if (!m_rgEPObjTrackInfo[i].m_fTrack)
        {
            m_rgEPObjTrackInfo[i].m_iMissingCount++;
        }
        else
        {
            if (m_rgEPObjTrackInfo[i].LastInfo().m_rcFG == CRect(0, 0, 0, 0))
            {
                m_rgEPObjTrackInfo[i].m_iMissingCount++;
                m_rgEPObjTrackInfo[i].LastInfo().Clear();
                m_rgEPObjTrackInfo[i].m_iObjInfoCount--;
            } 
            else
            {
                m_rgEPObjTrackInfo[i].m_iMissingCount = 0;
            }
        }
        int iLineY;
        BOOL fTransToTemState = false;
        switch (m_rgEPObjTrackInfo[i].m_objTrackState)
        {
        case OBJ_TRACK_NEW:

                // �������������ĸ���
                if (m_rgEPObjTrackInfo[i].m_iMissingCount >= 1)
                {
                    m_rgEPObjTrackInfo[i].m_objTrackState = OBJ_TRACK_END;
                }
                else if (m_rgEPObjTrackInfo[i].m_iObjInfoCount >= 3)
                {
                    m_rgEPObjTrackInfo[i].m_objTrackState = OBJ_TRACK_MOVING;
                }
                break;
        case OBJ_TRACK_MOVING:
            if (m_rgEPObjTrackInfo[i].m_iMissingCount >= 1)
            {
                m_rgEPObjTrackInfo[i].m_objTrackState = OBJ_TRACK_END;
            }
            //iLineY = rcFullSearch.top + (100 - CObjectDetector::m_iEPOutPutDetectLine)*rcFullSearch.Height()/100;
            iLineY = (100 - CObjectDetector::m_iEPOutPutDetectLine) * rcMax.Height()/100;
            fTransToTemState = m_rgEPObjTrackInfo[i].IsTransToTemState(imgCurImage, m_iLastLightStatus, m_iCurLightStatus, rcVideoDet.TopLeft(),  iLineY);
            if (fTransToTemState)
            {
                //�ж��Ƿ�����ɵ����Ƴ����پ���̫��
                CRect roi = m_rgEPObjTrackInfo[i].LastInfo().m_rcFG;
                for (int j = 0; j < m_cEPObjTrackInfo; j++)
                {
                    if(j != i && m_rgEPObjTrackInfo[j].m_objTrackState == OBJ_TRACK_TEM)
                    {
                        //�����ж�
                        CRect r = m_rgEPObjTrackInfo[j].LastInfo().m_rcFG;
                        CRect rVir = r;
                        rVir.left -= r.Width()*15/10;
                        rVir.right += r.Width()*15/10;
                        rVir.bottom += r.Height()*4;
                        rVir.IntersectRect(&rVir, &rcMax);
                        if (rVir.IntersectRect(&rVir, &roi))
                        {
                                fTransToTemState = false;
                                break;
                        }
                    }
                } 
            }
            //if (!fTransToTemState)
            //{
            //    m_rgEPObjTrackInfo[i].m_objTrackState = OBJ_TRACK_END;
            //}
            //if (m_rgEPObjTrackInfo[i].IsTransToTemState(iVideoID,
            //    pImage,
            //    m_iLastLightStatus, m_iCurLightStatus,
            //    rcVideoDet.TopLeft(),  iLineY))
            if (fTransToTemState)
            {
                m_rgEPObjTrackInfo[i].m_objTrackState = OBJ_TRACK_TEM;
                CObjInfo objInfo = m_rgEPObjTrackInfo[i].LastInfo();
                HvSize EpSize;
                //���Ƴ�һ�����ƵĿ��,��ʱ�ó����������ƣ�֮�����Ҫ׼ȷ�Ļ���û������Ǹ�5�ֳ������
                CRect roi = objInfo.m_rcFG;
                
                int iLineW = 100;
                MatchRoad(roi.CenterPoint().x, roi.CenterPoint().y, &iLineW);
                if (iLineW > 0)
                {
                    EpSize.width = iLineW/6;
                    EpSize.height = EpSize.width>>1;
                } else 
                {
                    EpSize.width = 50;
                    EpSize.height = EpSize.width>>1;
                }
                DWORD32 dwBeginTick = GetSystemTick();
                RTN_HR_IF_FAILED(m_rgEPObjTrackInfo[i].NewTemEx2(pParam->iVideoID, objInfo,
                    pParam, imgCurImage, pProcessRespond,
                    m_iLastLightStatus, m_iCurLightStatus,
                    rcVideoDet.TopLeft(), EpSize));
                    m_rgEPObjTrackInfo[i].m_fNewTem = true;
                    //HV_Trace("Perpare new tem time = %d\n", GetSystemTick() - dwBeginTick);
            }
            
            break;
        case OBJ_TRACK_TEM:
            if (m_rgEPObjTrackInfo[i].m_iMissingCount >= 1)
            {
                m_rgEPObjTrackInfo[i].m_objTrackState = OBJ_TRACK_END;
            }
            //TODO ����복�Ƶĸ����ཻ����������������ཻ������������
             BOOL fConTemEnd = false; 
             HiVideo::CRect rNoPlate = m_rgEPObjTrackInfo[i].LastInfo().m_rcFG;
             CTrackInfo* pTrack = NULL;
             int iLineNoPlateNum = 0;
             iLineNoPlateNum = MatchRoad(rNoPlate.CenterPoint().x, rNoPlate.CenterPoint().y);
             int iEPLineY = (100 - CObjectDetector::m_iEPOutPutDetectLine)*rcMax.Height()/100;
            for (int j = 0; j < m_cTrackInfo; j++)
            {
                pTrack = m_rgTrackInfo + j;
                HiVideo::CRect rPlate = pTrack->LastInfo().rcPos;
                //�����ģ��ƥ���ཻ�����
                PlateInfo plateInfo = pTrack->LastInfo();
                PLATE_COLOR plateColor = plateInfo.color;
                PLATEINFO_TYPE plateType = plateInfo.nInfoType;
                HiVideo::CRect rInterSet;
                
                if( pTrack->m_State == tsNew 
                    ||  pTrack->m_State == tsWaitRemove)continue;

                if(rInterSet.IntersectRect(rPlate, rNoPlate))
                {
                    fConTemEnd = true;
                }
                //if ( !fConTemEnd && plateColor == PC_YELLOW && plateType == PI_LPR)
                //{
                //    CRect rectVir = rPlate;
                //    int iWidth = rectVir.Width()*12/10;
                //    int iHeight = rectVir.Height();
                //    rectVir.left -= iWidth;
                //    rectVir.right += iWidth;
                //    rectVir.top -= iHeight*8;
                //    rectVir.IntersectRect(&rectVir,&rcMax);
                //    if(rInterSet.IntersectRect(rectVir, rNoPlate))
                //    {
                //        fConTemEnd = true;
                //        //m_rgEPObjTrackInfo[i].m_fEPOutput =false;
                //    }
                //}
                
                if ( !fConTemEnd && plateType == PI_LPR) 
                {
                    if (plateColor == PC_YELLOW)
                    {
                        //���������ڳ���
                        int iLineYellow = MatchRoad(rPlate.CenterPoint().x, rPlate.CenterPoint().y);
                        if (( iLineYellow == iLineNoPlateNum && rPlate.top > rNoPlate.bottom) 
                            && (rPlate.CenterPoint().y > iEPLineY )
                            && (rPlate.top - rNoPlate.bottom) < (rcMax.Height()>>1))
                        {
                            fConTemEnd = true;
                            m_rgEPObjTrackInfo[i].m_fEPOutput = false;
                        }
                    } else 
                    {
                        //������
                            CRect rectVir = rPlate;
                            int iWidth = rectVir.Width()*12/10;
                            int iHeight = rectVir.Height();
                            
                            rectVir.left -= (iWidth);
                            rectVir.right += (iWidth);
                            rectVir.top -= (iHeight<<3);
                            
                            rectVir.IntersectRect(&rectVir,&rcMax);
                            if(rInterSet.IntersectRect(rectVir, rNoPlate))
                            {
                                fConTemEnd = true;
                                //if(rNoPlate.bottom > iEPLineY)fConTemEnd = true; 
                                //m_rgEPObjTrackInfo[i].m_fEPOutput = false;
                            }
                    }
                } 
                if (fConTemEnd)
                {
                    break;
                }
            }
            if (fConTemEnd)
            {
                m_rgEPObjTrackInfo[i].m_objTrackState = OBJ_TRACK_END;
            }
            break;
        }
    }

    
    //���羯��ÿһ���Ѿ���ģ�����״̬�����Ƴ����٣���ģ�͵ķ���ȥʶ���������ǲ���һ����ĳ���
    if(m_LightType == DAY && CTrackInfo::m_fEPUseCarTailModel == 1)    
    {
        CheckEPNoPlateConfidence2(&imgCurImage, pParam->iVideoID);
    }

	return S_OK;
    
}

HRESULT CTrackerImpl::CalcCarSpeed(float &fltCarSpeed, float &fltScaleOfDistance, CTrackInfo &TrackInfo)
{
	const int iCalsCount = 3;
	float rgfltCarSpeed[iCalsCount] = {0.0f}, rgfltScale[iCalsCount] = {0.0f};
	if (m_fEnableScaleSpeed &&
		TrackInfo.m_cPlateInfo > 1)
	{
		// ȡ��ǰ���ƺ͵�һ�����Ƶļ���ƽ���ٶȣ���Ϊ��ǰ���Ƶ��ٶ�
		CRect rcBegin = TrackInfo.m_rgPlateInfo[0].rcPos.TopLeft() + TrackInfo.m_rgPlateInfo[0].rcPlatePos;
		int iInfoIndex = TrackInfo.m_cPlateInfo - 1;
		for (int i = 0; i < iCalsCount; i++)
		{
			while (iInfoIndex > 0)
			{
				if (TrackInfo.m_fVoted &&
					TrackInfo.m_rgPlateInfo[iInfoIndex].nInfoType != PI_LPR)
				{
					iInfoIndex--;
				}
				else
				{
					break;
				}
			}
			if (iInfoIndex == 0) break;
			CRect rcEnd = TrackInfo.m_rgPlateInfo[iInfoIndex].rcPos.TopLeft() + TrackInfo.m_rgPlateInfo[iInfoIndex].rcPlatePos;
			//��Ҫ�ô�ĳ�����Ԥ�����
			if (rcEnd.Width() < rcBegin.Width())
			{
				rgfltCarSpeed[i] = m_pScaleSpeed->CalcCarSpeedNewMethod(
					rcEnd,
					rcBegin,
					TrackInfo.m_rgPlateInfo[iInfoIndex].dwFrameTime - TrackInfo.m_rgPlateInfo[0].dwFrameTime,
					true,
					TrackInfo.m_nVotedType,
					rgfltScale[i]
					);
			}
			else
			{
				rgfltCarSpeed[i] = m_pScaleSpeed->CalcCarSpeedNewMethod(
					rcBegin,
					rcEnd,
					TrackInfo.m_rgPlateInfo[iInfoIndex].dwFrameTime - TrackInfo.m_rgPlateInfo[0].dwFrameTime,
					true,
					TrackInfo.m_nVotedType,
					rgfltScale[i]
					);
			}
			iInfoIndex--;
		}
		for (int i = 0; i < iCalsCount - 1; i++)
		{
			for (int j = i + 1; j< iCalsCount; j++)
			{
				if (rgfltCarSpeed[i] < rgfltCarSpeed[j])
				{
					float fltTemp = rgfltCarSpeed[i];
					rgfltCarSpeed[i] = rgfltCarSpeed[j];
					rgfltCarSpeed[j] = fltTemp;
					float fltTemp1 = rgfltScale[i];
					rgfltScale[i] = rgfltScale[j];
					rgfltScale[j] = fltTemp1;
				}
			}
		}
		for (int i = iCalsCount / 2; i >= 0; i--)
		{
			fltCarSpeed = rgfltCarSpeed[i];
			fltScaleOfDistance = rgfltScale[i];
			if (fltCarSpeed > 0.0f)
			{
				break;
			}
		}
		// �ٶ�ֵ��������������ٶ�ֵΪ0�����
		// huanggr 2011-11-07
		if (fltCarSpeed < 1.0f)
		{
			fltCarSpeed = 1.0f;
		}
	}
	return S_OK;
}

HRESULT CTrackerImpl::CalcObjSpeed( float &fltCarSpeed, float &fltScaleOfDistance, CObjTrackInfo &ObjTrackInfo )
{
	const int iCalsCount = 3;
	float rgfltObjSpeed[iCalsCount] = {0.0f}, rgfltScale[iCalsCount] = {0.0f};
	if (m_fEnableScaleSpeed &&
		ObjTrackInfo.m_iObjInfoCount > 1)
	{
		// ȡ��ǰλ�ú͵�һ��λ�ü���ƽ���ٶȣ���Ϊ��ǰ�˶�������ٶ�
		CRect rcBegin = ObjTrackInfo.m_rgObjInfo[0].m_rcFG;
		int iInfoIndex = ObjTrackInfo.m_iObjInfoCount - 1;
		for (int i = 0; i < iCalsCount; i++)
		{
			if (iInfoIndex == 0) break;
			CRect rcEnd = ObjTrackInfo.m_rgObjInfo[iInfoIndex].m_rcFG;
			//rgfltObjSpeed[i] = m_pScaleSpeed->CalcCarSpeed(
			rgfltObjSpeed[i] = m_pScaleSpeed->CalcCarSpeedNewMethod(
				rcBegin,
				rcEnd,
				ObjTrackInfo.m_rgObjInfo[iInfoIndex].m_dwFrameTime - ObjTrackInfo.m_rgObjInfo[0].m_dwFrameTime,
				false,
				PLATE_UNKNOWN,
				rgfltScale[i]
				);
			iInfoIndex--;
		}
		for (int i = 0; i < iCalsCount - 1; i++)
		{
			for (int j = i + 1; j< iCalsCount; j++)
			{
				if (rgfltObjSpeed[i] < rgfltObjSpeed[j])
				{
					float fltTemp = rgfltObjSpeed[i];
					rgfltObjSpeed[i] = rgfltObjSpeed[j];
					rgfltObjSpeed[j] = fltTemp;
					float fltTemp1 = rgfltScale[i];
					rgfltScale[i] = rgfltScale[j];
					rgfltScale[j] = fltTemp1;
				}
			}
		}
		if (ObjTrackInfo.m_iObjInfoCount > 2)
		{
			fltCarSpeed = rgfltObjSpeed[iCalsCount / 2];
			fltScaleOfDistance = rgfltScale[iCalsCount / 2];
		}
		else
		{
			fltCarSpeed = rgfltObjSpeed[0];
			fltScaleOfDistance = rgfltScale[0];
		}
	}
	return S_OK;
}

// ����ģ��ƥ��״̬�ĳ���
HRESULT CTrackerImpl::ProcessTemTrackState(
	CTrackInfo *pTrack,									//��ǰ����ĸ���
	CRect &rcTrackArea
	)
{
	// ������ٶ�ʧ�������ģ�����״̬(�������ᶪʧ)
	if ((!pTrack->m_fTempTrackExists) || (!pTrack->m_fhasTemple))
	{
		pTrack->m_State = tsTemTrackEnd;
		return S_OK;
	}

	// ������������������ж�
	CRect rcLast = pTrack->LastInfo().rcPos;

	bool fInEndTrackArea = !rcTrackArea.Contains(rcLast);		// ������������
	int nTempGap = 5;

	if (!fInEndTrackArea)
	{
		// �жϵ�ǰ֡�Ƿ񿿽���Ե
		bool fCloseEdge = (rcLast.top - nTempGap <= rcTrackArea.top) 
			|| (rcLast.bottom + nTempGap >= rcTrackArea.bottom)
			|| (rcLast.left - nTempGap <= rcTrackArea.left)
			|| (rcLast.right + nTempGap >= rcTrackArea.right);

		if (fCloseEdge)
		{
			pTrack->m_nCloseEdgeCount++;						// ������Ե������
		}

		if (!pTrack->m_fReverseRun)
		{
			// ������ʻ
			if (pTrack->m_fTempCloseEdge)
			{
				// ��һ֡�Ѿ�������Ե
				if (!fCloseEdge)
				{
					// �з��������Ͻ�������
					fInEndTrackArea = true;
				}
				else
				{
					if (pTrack->m_nCloseEdgeCount >= 2)
					{
						// ������Ե2֡�ڱ������
						fInEndTrackArea = true;
					}
				}
			}
			else
			{
				// ��һ֡û�п�����Ե
				if (fCloseEdge)
				{
					pTrack->m_fTempCloseEdge = true;
				}
			}
		}
		else
		{
			// �����л������Ե��������?
			if (fCloseEdge)
			{
				fInEndTrackArea = true;					// ������ʻ���Ͻ�������
			}
		}
	}

	// ����ж��Ƿ��������
	if (fInEndTrackArea)
	{
		pTrack->m_State = tsTemTrackEnd;							// ������ǰ����
	}

	return S_OK;
}

void CTrackerImpl::RoadInfoParamInit()
{
	CTrackInfo::m_iRoadNumber = 2;
	CTrackInfo::m_iRoadNumberBegin = 0;
	CTrackInfo::m_iStartRoadNum = 0;
	for(int i = 0; i < CTrackInfo::s_iMaxRoadNumber; ++i)
	{
		CTrackInfo::m_roadInfo[i].ptTop.x = 0;
		CTrackInfo::m_roadInfo[i].ptTop.y = 0;
		CTrackInfo::m_roadInfo[i].ptBottom.x = 0;
		CTrackInfo::m_roadInfo[i].ptBottom.y = 0;
		CTrackInfo::m_roadInfo[i].iLineType = 0;

		//strcpy(CTrackInfo::m_roadInfo[i].szRoadTypeName, "NULL");
		CTrackInfo::m_roadInfo[i].iRoadType = (int)(RRT_FORWARD | RRT_LEFT | RRT_RIGHT | RRT_TURN);
		CTrackInfo::m_roadInfo[i].iFilterRunType = 0;
	}
}

void CTrackerImpl::InitRoadInfoParam()
{
	RoadInfoParamInit();
/*	if(GetCurrentParam()->g_PlateRcogMode != PRM_TOLLGATE)
	{
		m_pHvParam->GetInt(
			"Tracker\\RoadInfo", "RoadLineNumber",
			&CTrackInfo::m_iRoadNumber, CTrackInfo::m_iRoadNumber,
			2, CTrackInfo::s_iMaxRoadNumber,
			"����������", "", 5
			);		//������
		m_pHvParam->GetInt(
			"Tracker\\RoadInfo", "RoadNumberBegin",
			&CTrackInfo::m_iRoadNumberBegin, CTrackInfo::m_iRoadNumberBegin,
			0, 1,
			"���������ʼ����", ",0:����ʼ,1:���ҿ�ʼ", 5
			);
		m_pHvParam->GetInt(
			"Tracker\\RoadInfo", "StartRoadNumber",
			&CTrackInfo::m_iStartRoadNum, CTrackInfo::m_iStartRoadNum,
			0, 1,
			"��������ʼ���", ",0:��0��ʼ,1:��1��ʼ", 5
			);		//��������ʼ���

		// ������Ŀ�������(���˵�)
		if(CTrackInfo::m_iRoadNumber > 2)
		{
			m_pHvParam->GetInt(
				"Tracker\\RoadInfo", "ManRoadNumber",
				&CTrackInfo::m_iManRoadNum, CTrackInfo::m_iManRoadNum,
				0, CTrackInfo::s_iMaxRoadNumber - 1,
				"���˵����", "", 3
				);
		}
		else
		{
			CTrackInfo::m_iManRoadNum = -2;
		}
		//�ƹ�������,2011-03-07
		if(GetCurrentParam()->g_PlateRcogMode == PRM_ELECTRONIC_POLICE)
		{
			m_pHvParam->GetInt("Tracker\\RoadInfo", "OutputRoadTypeNameFlag", 
			&CTrackInfo::m_iOutputRoadTypeName,CTrackInfo::m_iOutputRoadTypeName,
			0,1,
			"����������ͱ�ʶ", "0-�����;1-���", 5);
		}
		char szSection[256] = {0};
		// ��ȡ������Ϣ
		for(int i = 0; i < CTrackInfo::s_iMaxRoadNumber; ++i)
		{
			sprintf(szSection, "Tracker\\RoadInfo\\Road%02d", i);

			m_pHvParam->GetInt(
				szSection, "TopX",
				&CTrackInfo::m_roadInfo[i].ptTop.x, CTrackInfo::m_roadInfo[i].ptTop.x,
				0,5000,
				"��X����","",5
				);
			m_pHvParam->GetInt(
				szSection, "TopY",
				&CTrackInfo::m_roadInfo[i].ptTop.y, CTrackInfo::m_roadInfo[i].ptTop.y,
				0,5000,
				"��Y����","",5
				);
			m_pHvParam->GetInt(
				szSection, "BottomX",
				&CTrackInfo::m_roadInfo[i].ptBottom.x, CTrackInfo::m_roadInfo[i].ptBottom.x,
				0,5000,
				"��X����","",5
				);
			m_pHvParam->GetInt(
				szSection, "BottomY",
				&CTrackInfo::m_roadInfo[i].ptBottom.y, CTrackInfo::m_roadInfo[i].ptBottom.y,
				0,5000,
				"��Y����","",5
				);
			m_pHvParam->GetInt(
				szSection, "LineType",
				&CTrackInfo::m_roadInfo[i].iLineType, CTrackInfo::m_roadInfo[i].iLineType,
				1,9999,
				"����","",5
				);

			m_pHvParam->GetInt(
				szSection, "RoadType",
				&CTrackInfo::m_roadInfo[i].iRoadType, CTrackInfo::m_roadInfo[i].iRoadType,
				1,9999,
				"��Ӧ�ĳ�������(�����)","",5
				);

			if (m_pParam->g_PlateRcogMode == PRM_ELECTRONIC_POLICE)
			{
				m_pHvParam->GetString(
					szSection, "RoadTypeName",
					CTrackInfo::m_roadInfo[i].szRoadTypeName, 255, 
					"������������", 
					"��ΪNULL����ȡ�ó������͵�������",5
					);
			}
			if (m_pParam->g_PlateRcogMode == PRM_ELECTRONIC_POLICE)
			{
				m_pHvParam->GetInt(
					szSection, "FilterRunType",
					&CTrackInfo::m_roadInfo[i].iFilterRunType, CTrackInfo::m_roadInfo[i].iFilterRunType,
					0,9999,
					"��������(1:ֱ��2:��ת4:��ת8:��ͷ32:����)","",5
					);
			}
		}
	}*/
}

//�ж��Ƿ�ѹ��
bool CTrackerImpl::IsOverLine(int iX, int iY)
{
	bool fRet = false;
	return fRet;
}

RUN_TYPE CTrackerImpl::CheckRunTypeEx(HiVideo::CRect rect0, HiVideo::CRect rect1)
{
	CPoint cpBegin, cpEnd;
	cpBegin = rect0.CenterPoint();
	cpEnd = rect1.CenterPoint();

	RUN_TYPE rt = RT_UNSURE;
	int iRoadBegin = MatchRoad(cpBegin.x, cpBegin.y);
	int iRoadEnd =   MatchRoad(cpEnd.x, cpEnd.y);

	int iRoadCount = CTrackInfo::m_iRoadNumber - 1;
	bool fIsOutLeft = false;
	bool fIsOutRight = false;
	bool fIsOutForward = false;
	if( CTrackInfo::m_iRoadNumberBegin == 0 )
	{
		if( LeftOrRight(cpEnd.x, cpEnd.y, 0) == -1 )
		{
			fIsOutLeft = true;
		}
		if( LeftOrRight(cpEnd.x, cpEnd.y, CTrackInfo::m_iRoadNumber - 1) == 1 )
		{
			fIsOutRight = true;
		}
	}
	else
	{
		if( LeftOrRight(cpEnd.x, cpEnd.y, CTrackInfo::m_iRoadNumber - 1) == -1 )
		{
			fIsOutLeft = true;
		}
		if( LeftOrRight(cpEnd.x, cpEnd.y, 0) == 1 )
		{
			fIsOutRight = true;
		}
	}
	
	int iForwardPosY = CTrackInfo::m_iRealForward * CTrackInfo::m_iHeight / 100;
	if( cpEnd.y < iForwardPosY )
	{
		fIsOutForward = true;
	}

	if( fIsOutLeft )
	{
		rt = RT_LEFT;
	}
	else if( fIsOutRight )
	{
		rt = RT_RIGHT;
	}
	else if( fIsOutForward )
	{
		rt = RT_FORWARD;
	}

	int iStopLineY = CTrackInfo::m_iStopLinePos * CTrackInfo::m_iHeight / 100;
	if(cpEnd.y > iStopLineY)
	{
		rt = RT_UNSURE;
	}

	return rt;
}
RUN_TYPE CTrackerImpl::CheckRunType(HiVideo::CRect rect0, HiVideo::CRect rect1)
{
	// zhaopy
	return CheckRunTypeEx(rect0, rect1);
	int iX1, iY1, iX2, iY2;
	iX1 = rect0.CenterPoint().x;
	iY1 = rect0.CenterPoint().y;
	iX2 = rect1.CenterPoint().x;
	iY2 = rect1.CenterPoint().y;

	RUN_TYPE rt = RT_UNSURE;
	int iYScr = (int)((float)iY2 / (float)m_nHeight * 100);
	int iXScr = (int)((float)iX2 / (float)m_nWidth * 100);

	int iRoad1 = MatchRoad(iX1, iY1);
	int iRoad2 = MatchRoad(iX2, iY2);

	if( iYScr < (CTrackInfo::m_iLeftStopLinePos + 5) && iXScr < 10 )
	{
		return RT_LEFT;
	}
	else if( iYScr < (CTrackInfo::m_iStopLinePos + 5) && iXScr > 90 )
	{
		return RT_RIGHT;
	}

	bool fCanDo = (iYScr < 50 
					|| (iYScr <= CTrackInfo::m_iLeftStopLinePos && iXScr <= 15) 
					|| (iYScr <= CTrackInfo::m_iStopLinePos && iXScr >= 85) 
					|| (iYScr <= CTrackInfo::m_iStopLinePos && iRoad1 != iRoad2) );
	if( !fCanDo ) return rt;

	//�ǲ�����ת��
	bool fIsLeftRoad2 = 0 != (CTrackInfo::m_roadInfo[iRoad2].iRoadType & RRT_LEFT);
	//�ǲ�����ת��
	bool fIsRightRoad2 = 0 != (CTrackInfo::m_roadInfo[iRoad2].iRoadType & RRT_RIGHT);

	int iDL, iDR;
	int iLRL;
	int iLRR;
	if(CTrackInfo::m_iRoadNumberBegin == 0)
	{
		iLRL = LeftOrRight(iX2, iY2, iRoad2, &iDL);
		iLRR = LeftOrRight(iX2, iY2, iRoad2 + 1, &iDR);
	}
	else
	{
		iLRR = LeftOrRight(iX2, iY2, iRoad2, &iDL);
		iLRL = LeftOrRight(iX2, iY2, iRoad2 + 1, &iDR);
	}

	if( (iRoad2 == 0 && CTrackInfo::m_iRoadNumberBegin == 0) || 
		(iRoad2 == (CTrackInfo::m_iRoadNumber - 2) && CTrackInfo::m_iRoadNumberBegin == 1))
	{
		if( iLRL == -1)
		{
			if( fIsLeftRoad2 || (iDL > (rect1.Width() / 2)) || iRoad1 != iRoad2 ) rt = RT_LEFT;
		}
		else if( iRoad1 != iRoad2 && fIsLeftRoad2 && (CTrackInfo::m_iSpecificLeft == 1) && (iDR > (rect1.Width() / 2)) )
		{
			rt = RT_LEFT;
		}
		else if( fIsLeftRoad2 && iRoad1 == iRoad2 && (iDR < (rect1.Width() / 2)) )
		{
			rt = RT_FORWARD;
		}
		else if( !fIsLeftRoad2 )
		{
			rt = RT_FORWARD;
		}
	}
	else if( (iRoad2 == (CTrackInfo::m_iRoadNumber - 2)  && CTrackInfo::m_iRoadNumberBegin == 0 ) || 
		(iRoad2 == 0 && CTrackInfo::m_iRoadNumberBegin == 1))
	{
		if( iLRR == 1 )
		{
			if( (iRoad1 != iRoad2) || fIsRightRoad2 || (iDR > (rect1.Width() / 2)) ) rt = RT_RIGHT;
		}
		else
		{
			rt = RT_FORWARD;
		}
	}
	else 
	{
		rt = RT_FORWARD;
	}

	return rt;
}

//Խ��
//�жϣ���һ֡�����һ֡�Ƿ�����ͬһ����
CROSS_OVER_LINE_TYPE CTrackerImpl::IsCrossLine(CTrackInfo& TrackInfo)
{
//	CROSS_OVER_LINE_TYPE coltType = COLT_NO;
	if (0 == m_pParam->m_iDetectCrossLineEnable)
	{
		return COLT_INVALID;
	}
	if (CTrackInfo::m_iRoadNumber <= 2)
	{
		return COLT_NO;
	}
	CROSS_OVER_LINE_TYPE coltLines[4] = {COLT_NO, COLT_LINE1, COLT_LINE2, COLT_LINE3};

	int iCenterX0, iCenterX1, iCenterY0, iCenterY1;
	iCenterX0 = (TrackInfo.m_rgPlateInfo[0].rcPos.right + TrackInfo.m_rgPlateInfo[0].rcPos.left) / 2;
	iCenterY0 = (TrackInfo.m_rgPlateInfo[0].rcPos.bottom + TrackInfo.m_rgPlateInfo[0].rcPos.top) / 2;

	//���̵�ģʽֻ��Ҫ�ж�ֹͣ��ǰ�ĳ���
	if (m_pParam->m_fUsedTrafficLight)
	{
		int iPosStop  = CTrackInfo::m_iStopLinePos * m_nHeight / 100;
		iCenterX1 = iCenterX0;
		iCenterY1 = iCenterY0;
		for (int i = 0; i < TrackInfo.m_cPlateInfo; i++)
		{
			if( TrackInfo.m_rgPlateInfo[i].nInfoType != PI_LPR )
			{
				continue;
			}
			int iTmpX = (TrackInfo.m_rgPlateInfo[i].rcPos.right + TrackInfo.m_rgPlateInfo[i].rcPos.left) / 2;
			int iTmpY = (TrackInfo.m_rgPlateInfo[i].rcPos.bottom + TrackInfo.m_rgPlateInfo[i].rcPos.top) / 2;
			if (iTmpY > iPosStop && iTmpY < iCenterY1)
			{
				iCenterX1 = iTmpX;
				iCenterY1 = iTmpY;
			}
		}
	}
	else
	{
		iCenterX1 = (TrackInfo.LastInfo().rcPos.right + TrackInfo.LastInfo().rcPos.left) / 2;
		iCenterY1 = (TrackInfo.LastInfo().rcPos.bottom + TrackInfo.LastInfo().rcPos.top) / 2;
	}

	int iRoad0, iRoad1;
	iRoad0 = MatchRoad(iCenterX0, iCenterY0);
	iRoad1 = MatchRoad(iCenterX1, iCenterY1);

	if (iRoad0 == iRoad1)
	{
		return COLT_NO;
	}
	if (iRoad0 > iRoad1)
	{
		int iTmp = iRoad1;
		iRoad1 = iRoad0;
		iRoad0 = iTmp;
	}

	int index = iRoad1;
	if( index >= 0 && index < 4 && CTrackInfo::m_ActionDetectParam.iIsCrossLine[index] == 1)
	{
		return coltLines[index];
	}

	return COLT_NO;
}

//ѹ���ж�
CROSS_OVER_LINE_TYPE CTrackerImpl::IsOverYellowLine(CTrackInfo& TrackInfo)
{
	if (0 == m_pParam->m_iDetectOverYellowLineEnable)
	{
		return COLT_INVALID;
	}

	CROSS_OVER_LINE_TYPE coltLines[5] = {COLT_LINE0, COLT_LINE1, COLT_LINE2, COLT_LINE3, COLT_LINE4};
	
	int iCount[5] = {0, 0, 0, 0, 0};
    
	for (int i = 0; i < CTrackInfo::m_iRoadNumber; i++)
	{
		if ((CTrackInfo::m_ActionDetectParam.iIsYellowLine[i] != 1 &&
			CTrackInfo::m_ActionDetectParam.iIsYellowLine[i] != 2) ||
			CTrackInfo::m_roadInfo[i].ptTop.y - CTrackInfo::m_roadInfo[i].ptBottom.y == 0 )
		{
			continue;
		}
		for (int t = 0; t < TrackInfo.m_cPlateInfo; t++)
		{
			if( TrackInfo.m_rgPlateInfo[t].nInfoType != PI_LPR )
			{
				continue;
			}
			//���ƿ�ȵ�һ��
			int iHalfWidth = (TrackInfo.m_rgPlateInfo[t].rcPos.right - TrackInfo.m_rgPlateInfo[t].rcPos.left) / 2;
			// ��������������
			iHalfWidth = (int)(iHalfWidth * m_fltOverLineSensitivity);
			// ���ݳ��ͣ�����������Ӧ��С
			switch (TrackInfo.m_nVotedCarType)
			{
			case CT_SMALL:
				break;
			case CT_MID:
				iHalfWidth = (int)(iHalfWidth * 1.2f);
				break;
			case CT_LARGE:
				iHalfWidth = (int)(iHalfWidth * 1.5f);
				break;
			}
			//�����ĵ�
			int iCenterX = (TrackInfo.m_rgPlateInfo[t].rcPos.right + TrackInfo.m_rgPlateInfo[t].rcPos.left) / 2;
			int iCenterY = (TrackInfo.m_rgPlateInfo[t].rcPos.bottom + TrackInfo.m_rgPlateInfo[t].rcPos.top) / 2;
			//���̵�ģʽֻ��Ҫ�ж�ֹͣ��ǰ�ĳ���
			if (m_pParam->m_fUsedTrafficLight && iCenterY < CTrackInfo::m_iStopLinePos * m_nHeight / 100)
			{
				continue;
			}
			//����߶�ӦY�ϵ�X
			float fTop = 1.0f*(iCenterY - CTrackInfo::m_roadInfo[i].ptTop.y) * 
				(CTrackInfo::m_roadInfo[i].ptTop.x - CTrackInfo::m_roadInfo[i].ptBottom.x);
			float fBottom = 1.0f*(CTrackInfo::m_roadInfo[i].ptTop.y - CTrackInfo::m_roadInfo[i].ptBottom.y);
			int iXCenter =  (int)(fTop / fBottom) + CTrackInfo::m_roadInfo[i].ptTop.x;
			if (abs(iCenterX - iXCenter) < iHalfWidth)
			{
				iCount[i]++;
			}
		}
	}
	
	int iMax = 0;
	int iIndex(0);
	for (int i = 0; i < 5; i++)
	{
		if (iCount[i] > iMax)
		{
			iMax = iCount[i];
			iIndex = i;
		}
	}
	if (0 >= iMax)
	{
		return COLT_NO;
	}
	else
	{
		return coltLines[iIndex];
	}
}

bool CTrackerImpl::IsCanFoundTemple(CTrackInfo *pTrack)
{
	bool fFoundTemple(false);				// �Ƿ���ģ��
	if (m_pParam->m_fUsedTrafficLight)
	{
		// ���̵�ֻ�Գ���λ������Ļ�߶�73% ֮ǰ�Ľ���ģ��
		if (pTrack->m_fTempTrackExists)
		{
			fFoundTemple = true;			// ģ����ٳɹ������������ģ��
		}
		else if (pTrack->m_fObservationExists)
		{
			// ���Ƹ��٣������ж�λ������Ļ�߶�72% ���ϲŽ���ģ��(Ŀ�ļ��ٶ������Ч��)
			int nPerY = (pTrack->LastInfo().rcPos.top * 100) / m_nHeight;
			fFoundTemple = (nPerY < CTrackInfo::m_nTrafficLightTempleYPos);		// ���̵�������72%�Ժ󲻻Ὠ��ģ��ƥ��
		}
	}
	else
	{
		// �Ǻ��̵�ģʽ��һֱ������ģ��
		if ((pTrack->m_fObservationExists) || (pTrack->m_fTempTrackExists))
		{
			fFoundTemple = true;
		}
	}

	return fFoundTemple;
}

// ����ֹͣ
const int g_nStopCarYOffset = 2;					// ����ֹͣ�ж�����(Y���������ƫ��)
const int g_nFrameCount = 4;
bool CTrackerImpl::IsCarStop(CTrackInfo *pTrack)
{
	bool fStop = true;

	if (pTrack->m_cPlateInfo < g_nFrameCount)
	{
		pTrack->m_iCarStopFrame =0;
		return false;				// ����g_nFrameCount֡���ж�
	}

	// ����3֡��ƽ������Y ����
	int nMeanY(0);
	int rgnY[g_nFrameCount];
	//added by liujie �����ж�X��������
	int nMeanX(0);
	int rgnX[g_nFrameCount];
	for (int i = 0; i < g_nFrameCount; i++)
	{
		rgnY[i] = (pTrack->m_rgPlateInfo[pTrack->m_cPlateInfo - g_nFrameCount + i].rcPos.top
			+ pTrack->m_rgPlateInfo[pTrack->m_cPlateInfo - g_nFrameCount + i].rcPos.bottom) / 2;
		nMeanY += rgnY[i];
	}
	for (int i = 0; i < g_nFrameCount; i++)
	{
		rgnX[i] = (pTrack->m_rgPlateInfo[pTrack->m_cPlateInfo - g_nFrameCount + i].rcPos.left
			+ pTrack->m_rgPlateInfo[pTrack->m_cPlateInfo - g_nFrameCount + i].rcPos.right) / 2;
		nMeanX += rgnX[i];
	}
	nMeanY /= g_nFrameCount;
	nMeanX /= g_nFrameCount;

	// ����ֹͣ�ж�
	for (int i = 0; i < g_nFrameCount; i++)
	{
		if (abs(rgnY[i] - nMeanY) > g_nStopCarYOffset)
		{
			// ƫ�����ֵ,�����ƶ�
			pTrack->m_iCarStopFrame =0;
			fStop = false;
			break;
		}
		//added by liujie ����X�����ϵ��ж�
		if (abs(rgnX[i] - nMeanX) > g_nStopCarYOffset)
		{
			// ƫ�����ֵ,�����ƶ�
			pTrack->m_iCarStopFrame =0;
			fStop = false;
			break;
		}

	}

	if(pTrack->LastInfo().rcPos.CenterPoint().y ==  pTrack->m_rgPlateInfo[pTrack->m_cPlateInfo - 2].rcPos.CenterPoint().y
		&& pTrack->LastInfo().rcPos.CenterPoint().x ==  pTrack->m_rgPlateInfo[pTrack->m_cPlateInfo - 2].rcPos.CenterPoint().x)
	{
		pTrack->m_iCarStopFrame++;
	}

	// zhaopy 
	int iFrameCount = 12;
	if( m_LightType == NIGHT )
	{
		iFrameCount = 6;
	}
	if(pTrack->m_iCarStopFrame > iFrameCount)
	{
		fStop = true;
	}
	else {
		fStop = false;
	}

	return fStop;
}

//�������ƫ����
static const  int X_OFFSET = 150;
static const  int Y_OFFSET = 250;
static const int g_nFrameOffsetCount = 3;

void CTrackerImpl::UpdateOffset(CTrackInfo *pTrack)
{
	if( pTrack == NULL || pTrack->m_cPlateInfo < g_nFrameOffsetCount ) return;
	// ����g_nFrameCount֡��ƽ������Y �����X����
	int nMeanY(0);
	int nMeanX(0);
	int rgnY[g_nFrameOffsetCount];
	int rgnX[g_nFrameOffsetCount];
	for (int i = 0; i < g_nFrameOffsetCount; i++)
	{
		rgnY[i] = abs(pTrack->m_rgPlateInfo[pTrack->m_cPlateInfo - g_nFrameOffsetCount + i].rcPos.top
			- pTrack->m_rgPlateInfo[pTrack->m_cPlateInfo - g_nFrameOffsetCount + i].rcPos.bottom);
		nMeanY += rgnY[i];
		rgnX[i] = abs(pTrack->m_rgPlateInfo[pTrack->m_cPlateInfo - g_nFrameOffsetCount + i].rcPos.right
			- pTrack->m_rgPlateInfo[pTrack->m_cPlateInfo - g_nFrameOffsetCount + i].rcPos.left);
		nMeanX += rgnX[i];
	}
	nMeanY /= g_nFrameOffsetCount;
	nMeanX /= g_nFrameOffsetCount;

	if( nMeanY > 5 )
	{
		pTrack->m_nMaxYPos = nMeanY * Y_OFFSET / 100;
	}
	if( nMeanX > 3 )
	{
		pTrack->m_nMaxXPos = nMeanX * X_OFFSET / 100;
	}
}

HRESULT CTrackerImpl::SetFirstLightType(LIGHT_TYPE nLightType)
{
	m_nFirstLightType = nLightType;
	m_dwLastCarLeftTime = GetSystemTick();
	return S_OK;
}

HRESULT CTrackerImpl::SetFirstPulseLevel(int nPulseLevel)
{
	m_iPulseLevel = nPulseLevel;
	return S_OK;
}

HRESULT CTrackerImpl::SetFirstCplStatus(int nCplStatus)
{
	m_iCplStatus = nCplStatus;
	return S_OK;
}


//�Ƿ񰴳�����ʻ
bool CTrackerImpl::CheckRoadAndRun(RUN_TYPE runtype, int runroadtype)
{
	bool fRet = false;
	if( runtype == RT_UNSURE )
	{
		fRet = true;
	}
	else if( runtype == RT_FORWARD && (runroadtype &  RRT_FORWARD) )
	{
		fRet = true;
	}
	else if( runtype == RT_LEFT && (runroadtype & RRT_LEFT) )
	{
		fRet = true;
	}
	else if( runtype == RT_RIGHT && (runroadtype & RRT_RIGHT) )
	{
		fRet = true;
	}
	else if( runtype == RT_TURN && (runroadtype & RRT_TURN) )
	{
		fRet = true;
	}
	else if(runtype == RT_CONVERSE)
	{
		fRet = true;
	}
	return fRet;
}

//�Ƿ񴳺��
bool CTrackerImpl::CheckRush(int iPreLight, int iAcrossLight, int runroadtype, int runtype, DWORD32 dwCaptureTime)
{
	if( iPreLight < 0 || iAcrossLight < 0 || iPreLight >= MAX_SCENE_COUNT || iAcrossLight >= MAX_SCENE_COUNT )
	{
		return false;
	}

	bool fRet = true;
	TRAFFICLIGHT_SCENE tsPre = {TLS_UNSURE, TLS_UNSURE, TLS_UNSURE, TLS_UNSURE};
	TRAFFICLIGHT_SCENE tsAcross = {TLS_UNSURE, TLS_UNSURE, TLS_UNSURE, TLS_UNSURE};
	//GetLightScene(iPreLight, &tsPre);
	//GetLightScene(iAcrossLight, &tsAcross);
	m_cTrafficLight.GetLightScene(iPreLight, &tsPre);
	m_cTrafficLight.GetLightScene(iAcrossLight, &tsAcross);
	//�����̵�ʱ��
	m_cTrafficLight.GetGreenLightStartTime(&m_greentick);
	if( m_greentick.dwLeft != 0 && dwCaptureTime > m_greentick.dwLeft && tsAcross.lsLeft != TLS_GREEN )
	{
//		HV_Trace("changes left light state to green\n");
		tsAcross.lsLeft = TLS_GREEN;
	}
	if( m_greentick.dwForward != 0 && dwCaptureTime > m_greentick.dwForward && tsAcross.lsForward != TLS_GREEN )
	{
//		HV_Trace("changes forward light state to green\n");
		tsAcross.lsForward = TLS_GREEN;
	}
	if( m_greentick.dwRight != 0 && dwCaptureTime > m_greentick.dwRight && tsAcross.lsRight != TLS_GREEN )
	{
//		HV_Trace("changes right light state to green\n");
		tsAcross.lsRight = TLS_GREEN;
	}
	if( m_greentick.dwTurn != 0 && dwCaptureTime > m_greentick.dwTurn && tsAcross.lsTurn != TLS_GREEN ) 
	{
//		HV_Trace("changes turn light state to green\n");
		tsAcross.lsTurn = TLS_GREEN;
	}

	bool fUnLeft = ( (tsPre.lsLeft == TLS_RED || tsPre.lsLeft == TLS_YELLOW) && (tsAcross.lsLeft == TLS_RED || tsAcross.lsLeft == TLS_YELLOW) );
	bool fUnRight = ( (tsPre.lsRight == TLS_RED || tsPre.lsRight == TLS_YELLOW) && (tsAcross.lsRight == TLS_RED || tsAcross.lsRight == TLS_YELLOW) );
	bool fUnForward = ( (tsPre.lsForward == TLS_RED || tsPre.lsForward == TLS_YELLOW) && (tsAcross.lsForward == TLS_RED || tsAcross.lsForward == TLS_YELLOW) );
	bool fUnTurn = ( (tsPre.lsTurn == TLS_RED || tsPre.lsTurn == TLS_YELLOW) && (tsAcross.lsTurn == TLS_RED || tsAcross.lsTurn == TLS_YELLOW) );

	//ֻҪ������ʻ�ĳ������Ͷ�Ӧ�ĵ�״̬���ǽ��еģ��ǾͲ�Ǵ���ơ?
	//��֮���ǡ�
	fRet = true;
	if( ((runroadtype & RRT_FORWARD) && !fUnForward)
		|| ((runroadtype & RRT_LEFT) && !fUnLeft) 
		|| ((runroadtype & RRT_RIGHT) && !fUnRight)
		|| ((runroadtype & RRT_TURN) && !fUnTurn)
		)
	{
		fRet = false;
	}
	
	if( !fRet &&  runtype != RT_UNSURE )
	{
		//�����ʻ������ȷ��
		//��ֻҪ��Ӧ�ĵ�״̬�ǽ��еģ����Ǵ���ơ���֮���ǡ�
		if( (runtype == RT_FORWARD && fUnForward && (runroadtype & RRT_FORWARD))
			|| (runtype == RT_LEFT && fUnLeft && (runroadtype & RRT_LEFT))
			|| (runtype == RT_RIGHT && fUnRight && (runroadtype & RRT_RIGHT))
			|| (runtype == RT_TURN && fUnTurn&& (runroadtype & RRT_TURN)) )
		{
			fRet = true;
		}
	}

	return fRet;
}

HRESULT CTrackerImpl::CheckPeccancyType(
						  int iPreLight,			//ѹ��ǰ�ĺ��̵Ƴ���
						  int iAcrossLight,		//���ߺ�ĺ��̵Ƴ���
						  RUN_TYPE runtype,	//��ʻ����
						  int runroadtype,      //��ʻ��������
						  DWORD32 dwCaptureTime,	//ץ��ʱ��
						  /*PECCANCY_TYPE*/int * pPeccancyType //Υ������
						  )
{
	if(pPeccancyType == NULL)
	{
		return E_POINTER;
	}
	*pPeccancyType = PT_NORMAL;
	//�ƹ����޸�Υ�����ͣ��ж���Υ�����͵ľ����������2010-10-12
	if( (runtype == RT_CONVERSE) )
	{
		*pPeccancyType |= PT_CONVERSE; //����
	}
	/*else*/ if( (runroadtype & RRT_ESTOP) )
	{
		*pPeccancyType |= PT_ESTOP; //�ǻ�������
	}
	/*else*/ if(!(*pPeccancyType & RT_CONVERSE) && !(*pPeccancyType & PT_ESTOP) && CheckRush(iPreLight, iAcrossLight, runroadtype, runtype, dwCaptureTime) )
	{
		*pPeccancyType |= PT_RUSH; //�����
	}
	/*else*/ if(!CheckRoadAndRun(runtype, runroadtype))
	{
		*pPeccancyType |= PT_ACROSS; //����������ʻ
	}

	return S_OK;
}

HRESULT CTrackerImpl::CheckCarArrive(PROCESS_ONE_FRAME_RESPOND* pProcessRespond, int iDiffTime)
{

    //bool fTollMode = (GetCurrentParam()->g_PlateRcogMode == PRM_TOLLGATE);
	if (/*!fTollMode &&*/ (m_fEnableScaleSpeed == FALSE || m_fEnableCarArriveTrigger == FALSE) ) return S_FALSE;

	HRESULT hr = S_OK;
	DWORD32 dwCurTick = GetSystemTick();
	DWORD32 dwCurRealTick = dwCurTick;
	if( iDiffTime < 0 ) dwCurTick += (-iDiffTime);
	if( iDiffTime >= 0 ) dwCurTick -= iDiffTime;
	CTrackInfo* pTrack = NULL;
	//bool fNeedCarArrive = false;

    int nCarArrivePos = m_iCarArrivedPos * m_nHeight / 100;
	for(int i = 0; i < m_cTrackInfo; ++i)
	{
		bool fNeedCarArrive = false;
		pTrack = &m_rgTrackInfo[i];
		if( pTrack->m_fCarArrived ) continue;

        /*if (fTollMode)
        {
            if (pTrack->m_cPlateInfo > 1 && (
            		m_pParam->g_MovingDirection == MD_TOP2BOTTOM && pTrack->LastInfo().rcPos.bottom >= nCarArrivePos
                    && pTrack->m_rgPlateInfo[pTrack->m_cPlateInfo-2].rcPos.bottom <= nCarArrivePos
                    ||
            		m_pParam->g_MovingDirection == MD_BOTTOM2TOP && pTrack->LastInfo().rcPos.bottom <= nCarArrivePos
                    && pTrack->m_rgPlateInfo[pTrack->m_cPlateInfo-2].rcPos.bottom >= nCarArrivePos)
                    )
            {
                pTrack->m_fCarArrived = true;
            }
            continue;
        }*/
		
		DWORD32 dwImageTime = pTrack->LastInfo().dwFrameTime;
		int iTriggerPos = pTrack->LastInfo().color == PC_YELLOW ? m_iCarArrivedPosYellow : m_iCarArrivedPos;
		CRect rcPos = pTrack->LastInfo().rcPos;
		int iPosY = iTriggerPos * m_nHeight / 100;
		int iCurPos = (rcPos.CenterPoint().y * 100 / m_nHeight);
		if ( (m_pParam->g_MovingDirection == MD_TOP2BOTTOM && rcPos.CenterPoint().y > iPosY)
			|| (m_pParam->g_MovingDirection == MD_BOTTOM2TOP && rcPos.CenterPoint().y < iPosY) )
		{
			fNeedCarArrive = true;
		}  
		else if (m_cfgParam.iPlatform == 1 && iCurPos > 40 && pTrack->m_cPlateInfo < 4)
		{
			//fNeedCarArrive = true;
		}
		else if( (pTrack->m_dwCarArriveDopeTime == 0 || pTrack->m_dwCarArriveDopeTime > dwCurTick) //Ԥ��ʱ��Ϊ0���ߴ��ڵ�ǰʱ��
				&& ( pTrack->m_cPlateInfo >= 4 || (iCurPos > 25 && pTrack->m_cPlateInfo >= 2) )   //��������֡
				&& !pTrack->m_fPredictCarArrive //��ǰ֡û��Ԥ���
				)
		{

			pTrack->m_fPredictCarArrive = true;
			//Ԥ��CarArrive��ʱ��.
			float fltDistance = m_pScaleSpeed->CalcActureDistance(rcPos.CenterPoint(), CPoint(rcPos.CenterPoint().x, iPosY));
			//���㳵�����ﴥ���ӳ�ʱ��
			float fltCarSpeed(0.0f);
			float fltScaleOfDistance(1.0f);
			CalcCarSpeed(fltCarSpeed, fltScaleOfDistance, (*pTrack));

			float fltSpeedTemp = fltCarSpeed;
			if( fltCarSpeed > 10.0f && fltDistance > 0.0f )
			{
				int iRelay = 80;
				if (m_cfgParam.iPlatform == 1)
				{
					iRelay = 120;
					if( m_iAvgY < 25 )
					{
						iRelay = 80;
					}
					if( fltCarSpeed < 20.0f )
					{
						iRelay = 80;
					}
					iRelay += 40;
				}
				fltCarSpeed /= 3600;
				pTrack->m_dwCarArriveDopeTime = (DWORD32) (fltDistance / fltCarSpeed) + dwImageTime;
				if( pTrack->m_dwCarArriveDopeTime > (DWORD32)iRelay )
				{
					pTrack->m_dwCarArriveDopeTime -= iRelay;
				}
				//HV_Trace("LastImageTime:%d, DopeTime:%d, CurTime:%d, speed: %0.2f pointY:%d, fltDistance:%0.2f\n", 
					//dwImageTime, pTrack->m_dwCarArriveDopeTime, dwCurTick, fltTemp, rcPos.CenterPoint().y, fltDistance);
			}
			if( pTrack->m_dwDopeFirstPos == 0 )
			{
				pTrack->m_dwDopeFirstPos = pTrack->m_dwCarArriveDopeTime;
				pTrack->m_dwDopeEndPos = iCurPos;
			}
			else
			{
				pTrack->m_dwDopeEndPos = pTrack->m_dwCarArriveDopeTime;
			}
		}

		if( fNeedCarArrive 
			|| (pTrack->m_dwCarArriveDopeTime != 0 
				&& pTrack->m_dwCarArriveDopeTime < dwCurTick ) )
		{
			float fltDistance = 0.0f;
			fltDistance = m_pScaleSpeed->CalcActureDistance(rcPos.CenterPoint(), CPoint(rcPos.CenterPoint().x, iPosY));
			if( fNeedCarArrive )
			{
				fltDistance = m_iCarArrivedDelay - fltDistance;
			}
			else
			{
				fltDistance = m_iCarArrivedDelay + fltDistance;
				//HV_Trace("LastImageTime:%d, DopeTime:%d, CurTime:%d\n", dwImageTime, pTrack->m_dwCarArriveDopeTime, dwCurTick);
			}
			hr = ProcessCarArrive(pTrack, fNeedCarArrive ? dwImageTime : dwCurTick, fltDistance, pProcessRespond);
			//hr = ProcessCarArrive(pTrack, dwImageTime, fltDistance, pProcessRespond);
		}
		else
		{
			pTrack->m_dwLastProcessCarArrvieTime = pTrack->m_dwCarArriveDopeTime;
		}
	}
	return hr;
}

HRESULT CTrackerImpl::ProcessNoPlateCarArrive(
				CObjTrackInfo* pObjTrack, 
				DWORD32 dwImageTime, 
				float fltDistance, 
				PROCESS_ONE_FRAME_RESPOND* pProcessRespond
				)
{
	HRESULT hr = S_OK;
	if( pObjTrack == NULL )
	{
		 return E_POINTER;
	}

	CARARRIVE_INFO_STRUCT carArriveInfo;
	CRect rcPos = pObjTrack->LastInfo().m_rcFG;
	pObjTrack->m_iRoad = MatchRoad(rcPos.CenterPoint().x, rcPos.CenterPoint().y);
	carArriveInfo.iRoadNumber = pObjTrack->m_iRoad;
	carArriveInfo.iPlateLightType = m_PlateLightType;

	carArriveInfo.dwTriggerOutDelay = 0;
	carArriveInfo.dwCarArriveTime = dwImageTime;
	carArriveInfo.dwCarArriveRealTime = GetSystemTick();

	CARARRIVE_INFO_STRUCT* pCarArriveInfo = &pProcessRespond->cTrigEvent.rgCarArriveInfo[pProcessRespond->cTrigEvent.iCarArriveCount++];
	memcpy(pCarArriveInfo, &carArriveInfo, sizeof(CARARRIVE_INFO_STRUCT));
	pProcessRespond->cTrigEvent.dwEventId |= EVENT_CARARRIVE;

	DWORD32 dwCurTick = GetSystemTick();
	pObjTrack->m_fCarArrived = true;
	pObjTrack->m_nCarArriveTime = dwImageTime;
	pObjTrack->m_nCarArriveRealTime = dwCurTick;
	pObjTrack->m_rcCarArrivePos = rcPos;

	// zhaopy ����ץ��
	CapObjTrack(pObjTrack);

	return hr;
}

HRESULT CTrackerImpl::ProcessCarArrive(CTrackInfo* pTrack, DWORD32 dwImageTime, float fltDistance, PROCESS_ONE_FRAME_RESPOND* pProcessRespond)
{
	HRESULT hr = S_OK;
	// ֪ͨ��������
	CARARRIVE_INFO_STRUCT carArriveInfo;
	pTrack->m_iRoad = MatchRoad(pTrack->LastInfo().rcPos.CenterPoint().x, pTrack->LastInfo().rcPos.CenterPoint().y);

	DWORD32 dwCurTick = GetSystemTick();

	// zhaopy
	carArriveInfo.iRoadNumber = pTrack->m_iRoad;
	carArriveInfo.iPlateLightType = m_PlateLightType;
	// zhaopy
	carArriveInfo.dwTriggerOutDelay = pTrack->m_dwCarArriveDopeTime;
	carArriveInfo.dwCarArriveTime = dwImageTime;
	carArriveInfo.dwCarArriveRealTime = dwCurTick;

	carArriveInfo.dwFirstPos = pTrack->m_dwDopeFirstPos;
	carArriveInfo.dwEndPos = pTrack->m_dwDopeEndPos;

	CARARRIVE_INFO_STRUCT* pCarArriveInfo = &pProcessRespond->cTrigEvent.rgCarArriveInfo[pProcessRespond->cTrigEvent.iCarArriveCount++];
	memcpy(pCarArriveInfo, &carArriveInfo, sizeof(CARARRIVE_INFO_STRUCT));
	
	pProcessRespond->cTrigEvent.dwEventId |= EVENT_CARARRIVE;

	pTrack->m_fCarArrived = true;
	if( !pTrack->m_fIsTrigger && m_fEnableCarArriveTrigger )
	{
		pTrack->m_nCarArriveTime = dwImageTime;
		pTrack->m_nCarArriveRealTime = dwCurTick;
	}
	else if( !m_fEnableCarArriveTrigger )
	{
		pTrack->m_nCarArriveTime = dwImageTime;
		pTrack->m_nCarArriveRealTime = dwCurTick;
	}
	pTrack->m_rcCarArrivePos = pTrack->LastInfo().rcPos;

	// zhaopy ����ץ��
	CapTrack(pTrack);

	return hr;
}

HRESULT CTrackerImpl::CheckEvent()
{
	return CheckCarArriveTrigger();
}

HRESULT CTrackerImpl::CheckCarArriveTrigger()
{
	HRESULT hr = S_OK;
	if(m_fEnableCarArriveTrigger == FALSE)
	{
		return hr;
	}

	CTrackInfo* pTrack = NULL;
	DWORD32 dwCurTick = GetSystemTick();
	for(int i = 0; i < m_cTrackInfo; ++i)
	{
		pTrack = &m_rgTrackInfo[i];
		if( pTrack->m_fCarArrived || pTrack->m_fIsTrigger || pTrack->m_dwCarArriveDopeTime == 0) continue;

		if( pTrack->m_dwCarArriveDopeTime < dwCurTick)
		{
			// zhaopy ����ץ��
			pTrack->m_nCarArriveTime = dwCurTick;
			pTrack->m_nCarArriveRealTime = dwCurTick - 1 ;
			CapTrack(pTrack);
		}
	}
	return hr;
}

//��������֡�Ƿ�����֡���ϸ��ٿ�ĵײ��ǳ������������һ���λ�á�
//ֻ���ڳ�βģʽ��
// weikt 2011-4-1
HRESULT CTrackerImpl::CheckNoPlate(CObjTrackInfo* pObj)
{
	HRESULT fRet = E_FAIL;
	if( pObj == NULL ) return fRet;

	CRect rcMax(0, 0, m_nWidth, m_nHeight);
	CRect rcVideoDetArea;
	RTN_HR_IF_FAILED(MapParamRect(rcVideoDetArea, rcMax, m_rcVideoDetArea));


	
	int iFrameCount = 0;
	int iBottomPos = 0;
	int iVoteDistance = rcVideoDetArea.Height() * 30 / 100;
	for(int i = pObj->m_iObjInfoCount - 1; i >= pObj->m_iObjInfoCount - 4; --i)
	{
		iBottomPos = abs( pObj->m_rgObjInfo[i].m_rcFG.bottom - rcVideoDetArea.bottom );
		if( iBottomPos > iVoteDistance )
		{
			iFrameCount++;
		}
	}

	fRet = (iFrameCount >= 3 ? S_OK : E_FAIL);

	if(SUCCEEDED(fRet) && SUCCEEDED(RemoveMoreReview(pObj)))
	{
		fRet = E_FAIL;
	}

	return fRet;
}
//���˴���ֹͣ��ǰͣ�µĶ��  weikt 2011-4-1
HRESULT CTrackerImpl::RemoveMoreReview(CObjTrackInfo* pObj)
{
	int iPre = 0;
	//int iPreH = 0;
	int ilessY = 0;
	int iMoreY = 0;

	CRect rcMax(0, 0, m_nWidth, m_nHeight);
	CRect rcVideoDetArea;
	RTN_HR_IF_FAILED(MapParamRect(rcVideoDetArea, rcMax, m_rcVideoDetArea));
	int iVoteDistance = rcVideoDetArea.Height() * 60 / 100;

	CRect rcFirstFG = pObj->m_rgObjInfo[0].m_rcFG;
	CRect rcLastFG = pObj->LastInfo().m_rcFG;
	int iAvg = abs(rcLastFG.bottom - rcFirstFG.bottom) / pObj->m_iObjInfoCount;
	HRESULT fMove = E_FAIL;
	int iDiffMax = 0;
	int iTtoB = 0;
	for(int k = 0; k < pObj->m_iObjInfoCount - 1; k++)
	{
		int iTmp = pObj->m_rgObjInfo[k].m_rcFG.right - pObj->m_rgObjInfo[k].m_rcFG.left;
	
		if(k == 0)
		{
			iPre = iTmp;
		}
		if( ( iPre / iTmp > 3 ) 
			|| (iTmp / iPre > 3 )
			)
		{
			//ǰ����֡�Ŀ�Ȳ����ָ��ֵ,���Ҹ߶Ȳ�С��ָ��ֵʱ,���˵�
			iDiffMax++;
			if(iDiffMax > 1)
			{
				fMove = S_OK;			
				//HV_Trace("Move(w1=%d,w2=%d)... \n",iPre,iTmp);
			}
		}
		iPre = iTmp;

		if(k > 0)
		{
			int iCurCenter = (pObj->m_rgObjInfo[k].m_rcFG.bottom + pObj->m_rgObjInfo[k].m_rcFG.top) / 2;
			int iPreCenter = (pObj->m_rgObjInfo[k - 1].m_rcFG.bottom + pObj->m_rgObjInfo[k - 1].m_rcFG.top) / 2;
			if(iPreCenter / iCurCenter > 3)
			{
				//���ǰ����֡���ĵ������д����������˵�
				fMove = S_OK;
				//HV_Trace("Move(CurCenter=%d,PreCenter=%d)... \n",iCurCenter,iPreCenter);
			}
			if(iCurCenter >= iPreCenter)
			{
				iTtoB++;
			}
		}

		int Diff = abs(pObj->m_rgObjInfo[k].m_rcFG.bottom - pObj->m_rgObjInfo[k + 1].m_rcFG.bottom);
		if(Diff > iAvg)
		{
			iMoreY++;
		}
		else
		{
			ilessY++;
		}
	}
	if(iTtoB > 6)
	{
		fMove = S_OK;
		//HV_Trace("Move %d ...\n", iTtoB);
	}
	if(ilessY > 0 && iMoreY > 0)
	{
		if(ilessY / iMoreY > (pObj->m_iObjInfoCount - 1) * 0.9)
		{
			//�������ƽ���������ֵС��С��ƽ��������ֵ�ĺܴ�������˵�

			fMove = S_OK;
			//HV_Trace("Move(less=%d,more=%d)...\n", ilessY, iMoreY);
		}
	}
	return fMove;
}

void DM6467_DelayUS( int iDelayUS )
{
	volatile int iTemp1, iTemp2, i;


	for ( i = 0; i < iDelayUS; i ++ )
	{
		for ( iTemp1 = 0; iTemp1 < 10; iTemp1 ++ )
		{
			for ( iTemp2 = 0; iTemp2 < 1000; iTemp2 ++ )
			{
			}
		}
	}
}


extern "C"
{

#ifndef CHIP_8127
	void PRECISE_DelayUS(DWORD32 dwCount);
#else
	void PRECISE_DelayUS(DWORD32 dwCount)
	{
		return;
	}
#endif
}



//================--------- zhaopy �������ץ�ĵĴ��� --------------===============================

#define PWM0_BAR    0x01C22000
#define PWM0_START_REG  (PWM0_BAR + 0xC )

void CTrackerImpl::TriggerCamera(const int iRoadNum)
{
	if (m_cfgParam.iPlatform  == 1)//����
	{
	    volatile unsigned int *pwn_reg;
	    
	    pwn_reg = (volatile unsigned int *)(PWM0_START_REG); // val set to 1
	    (*pwn_reg) = 1;

	    return;
	}
	else if(m_cfgParam.iPlatform  == 2) //ˮ��ƽ̨
	{
		// 4���ֽ� ��1���ֽ���Ϊ�������������ʼΪ0�����������ֽ���Ϊץ�Ĵ��������
		unsigned int dwFlag = 0;
		dwFlag = iRoadNum;
		dwFlag <<= 24;
		dwFlag |= (m_dwTriggerCameraTimes & 0x00FFFFFF);
		volatile unsigned int *pwm1 = (volatile unsigned int *)0x01C22414;
		*pwm1 = dwFlag;

		volatile unsigned int *pwm0 = (volatile unsigned int *)0x01C2200C;
		*pwm0 = 1;
		return;

	}

	const DWORD32 GPIO_REG_BASE = 0x01c67000;
	const DWORD32 GPIO_REG_BANK_01_BASE = GPIO_REG_BASE + 0x10;	 // GPIO[0:31]
	const DWORD32 GPIO_REG_BANK_23_BASE = GPIO_REG_BASE + 0x38;	// GPIO[32:47]

	volatile int gpio;
	unsigned int addr;
	volatile unsigned int *gpio_dir, *gpio_set, *gpio_clr;
	unsigned int mask;

	if (m_cfgParam.iPlatform == 0)
	{
#ifdef __OLD__
		gpio = 26; // �ɰ�, ͨ��GP26�źŵ��½��ش���ץ��
#else
		gpio = 36; // �°�, ͨ��GP36�źŵ��½��ش���ץ��
#endif
	}
	else
	{
		gpio = 41; // ����
		// TODO: �˴�GPIO��������ARM��CPLD������ͻ
		// ��ɷ��������죬�����ʱ����  huanggr 2012-04-16
		return;
	}

	if (gpio < 32) 
	{
		addr = GPIO_REG_BANK_01_BASE;
		mask = 1 << gpio;
	} 
	else 
	{
		addr = GPIO_REG_BANK_23_BASE;
		mask = 1 << (gpio - 32);
	}

	if (m_cfgParam.iPlatform == 0)
	{
		// һ���������ʽ
		gpio_clr = (volatile unsigned int *)(addr + 0x0c); // val set to 0
		(*gpio_clr) |= mask;


		int iSleepUs = 1800;
		if (iRoadNum >= 0)
		{
			int iRealRoadNum = iRoadNum;
			if (iRealRoadNum == 0)//��һ����
			{
				iSleepUs = 200;
			}
			else if (iRealRoadNum == 1)//�ڶ�����
			{
				iSleepUs = 700;
			}
			else if (iRealRoadNum == 2)//��������
			{
				iSleepUs = 1300;
			}
		}

		PRECISE_DelayUS(iSleepUs);

		gpio_set = (volatile unsigned int *)(addr + 0x08); // val set to 1
		(*gpio_set) |= mask;
	}
	else
	{
		// ���崥����ʽ
		// ͨ��DSP�˸�ARM��һ���жϣ�
		// Ȼ�����ARM�˵Ļص��������д���ץ�����
		gpio_set = (volatile unsigned int *)(addr + 0x08); // val set to 1
		(*gpio_set) |= mask;

		DM6467_DelayUS(1);

		gpio_clr = (volatile unsigned int *)(addr + 0x0c); // val set to 0
		(*gpio_clr) |= mask;
	}

	// dir set to out
	gpio_dir = (volatile unsigned int *)addr;
	(*gpio_dir) &= (~mask);
}

CRect CTrackerImpl::GetEPNoPlatePredictRect(UINT nNowTime, CObjTrackInfo* pObjTrack)
{
    CRect rect(0,0,0,0);
    if(!pObjTrack)
    {
        return CRect(0,0,0,0);
    }
    if(pObjTrack->m_objTrackState != OBJ_TRACK_TEM)
    {
        return CRect(0,0,0,0);
    }
    if(pObjTrack->m_iObjInfoCount < 3)
    {
        return CRect(0,0,0,0);
    }
    
    if(pObjTrack->m_fFirstPredict)
    {
        //��һ��Ԥ��
        float fltCarSpeed(0.0f);
        CRect r1 = pObjTrack->m_rgObjInfo[pObjTrack->m_iObjInfoCount - 1].m_rcFG;
        CRect r2 = pObjTrack->m_rgObjInfo[pObjTrack->m_iObjInfoCount - 2].m_rcFG;
        //��Ϊr1��ģ��
        //����r2��һ����r1ͬ����ߵĹ��?
        int iSH = r2.Height();
        CPoint center = r2.CenterPoint();
        int iBottom = r2.bottom - (iSH>>1);
        int iTop = iBottom - r1.Height();
        int iLeft = center.x - (r1.Width()>>1);
        int iRight = center.x + (r1.Width()>>1);
        //
        r2.bottom = iBottom;
        r2.top = iTop;
        r2.left = iLeft;
        r2.right = iRight;
        //
        
        float fltInterval1 = (float)(pObjTrack->m_rgObjInfo[pObjTrack->m_iObjInfoCount - 1].m_dwFrameTime
            - pObjTrack->m_rgObjInfo[pObjTrack->m_iObjInfoCount - 2].m_dwFrameTime);
        float fltScale(0.0f);
        fltCarSpeed = m_pScaleSpeed->CalcCarSpeedNewMethod(
            r2,
            r1,
            (DWORD32)fltInterval1,
            true,
            PLATE_NORMAL,
            fltScale
            );
        //HV_Trace("\nfltCarSpeed:[%f]", fltCarSpeed);
        float fltInterval = (float)(nNowTime - pObjTrack->m_rgObjInfo[pObjTrack->m_iObjInfoCount - 1].m_dwFrameTime);
        float fltDistanceNew = (fltInterval / 1000) * (fltCarSpeed * 1000 / 3600) * 3;
        //HV_Trace("\nfltDistanceNew:[%f]", fltDistanceNew);
        //if(fltDistanceNew >= 600)fltDistanceNew = 0.0f;
        if( fltDistanceNew < 0 ) fltDistanceNew = -fltDistanceNew;
        HV_POINT ptDest;
        HV_POINT ptOrg;
        ptOrg.x = pObjTrack->m_rgObjInfo[pObjTrack->m_iObjInfoCount - 1].m_rcFG.CenterPoint().x;
        ptOrg.y = pObjTrack->m_rgObjInfo[pObjTrack->m_iObjInfoCount - 1].m_rcFG.CenterPoint().y;

        if (m_pScaleSpeed->GetActurePoint(ptOrg, ptDest, fltDistanceNew))
        {
            rect.top = ptDest.y - (pObjTrack->m_rgObjInfo[pObjTrack->m_iObjInfoCount - 1].m_rcFG.Height());
            rect.left = pObjTrack->m_rgObjInfo[pObjTrack->m_iObjInfoCount - 1].m_rcFG.CenterPoint().x - (pObjTrack->m_rgObjInfo[pObjTrack->m_iObjInfoCount - 1].m_rcFG.Width() * 15 / 10);
            //rect.left = pObjTrack->m_rgObjInfo[pObjTrack->m_iObjInfoCount - 1].m_rcFG.CenterPoint().x - (pObjTrack->m_rgObjInfo[pObjTrack->m_iObjInfoCount - 1].m_rcFG.Width());
            rect.bottom = pObjTrack->m_rgObjInfo[pObjTrack->m_iObjInfoCount - 1].m_rcFG.bottom + pObjTrack->m_rgObjInfo[pObjTrack->m_iObjInfoCount - 1].m_rcFG.Height() / 2;
            rect.right = pObjTrack->m_rgObjInfo[pObjTrack->m_iObjInfoCount - 1].m_rcFG.CenterPoint().x + (pObjTrack->m_rgObjInfo[pObjTrack->m_iObjInfoCount - 1].m_rcFG.Width() * 15 / 10);
            //rect.right = pObjTrack->m_rgObjInfo[pObjTrack->m_iObjInfoCount - 1].m_rcFG.CenterPoint().x + (pObjTrack->m_rgObjInfo[pObjTrack->m_iObjInfoCount - 1].m_rcFG.Width());
        } else {
        //��֪Ϊ�λ�ȡ����
        }
        
           
            
        pObjTrack->m_fFirstPredict = false;
        
    } else 
    {
        float fltCarSpeed(0.0f);
        float fltInterval1 = (float)(pObjTrack->m_rgObjInfo[pObjTrack->m_iObjInfoCount - 1].m_dwFrameTime
            - pObjTrack->m_rgObjInfo[pObjTrack->m_iObjInfoCount - 2].m_dwFrameTime);

        float fltScale(0.0f);
        fltCarSpeed = m_pScaleSpeed->CalcCarSpeedNewMethod(
            pObjTrack->m_rgObjInfo[pObjTrack->m_iObjInfoCount - 2].m_rcFG,
            pObjTrack->m_rgObjInfo[pObjTrack->m_iObjInfoCount - 1].m_rcFG,
            (DWORD32)fltInterval1,
            true,
            PLATE_NORMAL,
            fltScale
            );

        float fltInterval = (float)(nNowTime - pObjTrack->m_rgObjInfo[pObjTrack->m_iObjInfoCount - 1].m_dwFrameTime);
        float fltDistanceNew = (fltInterval / 1000) * (fltCarSpeed * 1000 / 3600) * 3;
        if( fltDistanceNew < 0 ) fltDistanceNew = -fltDistanceNew;
        HV_POINT ptDest;
        HV_POINT ptOrg;
        ptOrg.x = pObjTrack->m_rgObjInfo[pObjTrack->m_iObjInfoCount - 1].m_rcFG.CenterPoint().x;
        ptOrg.y = pObjTrack->m_rgObjInfo[pObjTrack->m_iObjInfoCount - 1].m_rcFG.CenterPoint().y;


        if (m_pScaleSpeed->GetActurePoint(ptOrg, ptDest, fltDistanceNew))
        {
            rect.top = ptDest.y - (pObjTrack->m_rgObjInfo[pObjTrack->m_iObjInfoCount - 1].m_rcFG.Height());
            //rect.left = pObjTrack->m_rgObjInfo[pObjTrack->m_iObjInfoCount - 1].m_rcFG.CenterPoint().x - (pObjTrack->m_rgObjInfo[pObjTrack->m_iObjInfoCount - 1].m_rcFG.Width() * 15 / 10);
            rect.left = pObjTrack->m_rgObjInfo[pObjTrack->m_iObjInfoCount - 1].m_rcFG.CenterPoint().x - (pObjTrack->m_rgObjInfo[pObjTrack->m_iObjInfoCount - 1].m_rcFG.Width());
            rect.bottom = pObjTrack->m_rgObjInfo[pObjTrack->m_iObjInfoCount - 1].m_rcFG.bottom + pObjTrack->m_rgObjInfo[pObjTrack->m_iObjInfoCount - 1].m_rcFG.Height() / 2;
            //rect.right = pObjTrack->m_rgObjInfo[pObjTrack->m_iObjInfoCount - 1].m_rcFG.CenterPoint().x + (pObjTrack->m_rgObjInfo[pObjTrack->m_iObjInfoCount - 1].m_rcFG.Width() * 15 / 10);
            rect.right = pObjTrack->m_rgObjInfo[pObjTrack->m_iObjInfoCount - 1].m_rcFG.CenterPoint().x + (pObjTrack->m_rgObjInfo[pObjTrack->m_iObjInfoCount - 1].m_rcFG.Width());
        }
    }
    return rect;

}
HRESULT CTrackerImpl::CheckEPNoPlateConfidence(HV_COMPONENT_IMAGE* pImgCalibratedFrame, int iVideID)
{
    //for test first
    /////////////////////////////////////////////////////////////
    //��������������
    HV_COMPONENT_IMAGE imgCarRegion;
    imgCarRegion.nImgType = HV_IMAGE_YUV_422;
    CRect rcMax1(0, 0, m_nWidth, m_nHeight);
    CRect temprcet(0,30,100,100);
    CRect rcSearchRegion;
    //HV_COMPONENT_IMAGE imgCalibratedFrame;
    RTN_HR_IF_FAILED(MapParamRect(rcSearchRegion, rcMax1, temprcet));
    RTN_HR_IF_FAILED(CropImage(*pImgCalibratedFrame, rcSearchRegion, &imgCarRegion));
    MULTI_ROI MultiROI=
    {
        ARRSIZE(m_rgEPCarPos),
            m_rgEPCarPos
    };
    int iDetMinScale = 8;
    int iDetMaxScale = 25;

    //int iDetMinScale = 7;
    //int iDetMaxScale = 17;
    RTN_HR_IF_FAILED(m_pPlateDetector->ScanPlatesComponentImage(
        CAR_DETECTOR,
        &imgCarRegion, //imgCalibratedFrame,
        &MultiROI,
        iDetMinScale,//m_pParam->g_nDetMinScaleNum,
        iDetMaxScale,//m_pParam->g_nDetMaxScaleNum,
        TRUE,
        0,
        FALSE,
        NULL,
        NULL
        ));

    //g_nDetResultCount = MultiROI.dwNumROI;
    //for(int i = 0; i < MultiROI.dwNumROI; i++)
    //{
    //    g_rgDetResult[i].left = m_rgTempPlatePos[i].left;
    //    g_rgDetResult[i].top = m_rgTempPlatePos[i].top+rcSearchRegion.top;
    //    g_rgDetResult[i].right = m_rgTempPlatePos[i].right;
    //    g_rgDetResult[i].bottom = m_rgTempPlatePos[i].bottom+rcSearchRegion.top;
    //}
    //���ģ�ͳ�������
    /*m_RectNum[4] = 0;
    for (int i = 0; i < MultiROI.dwNumROI; i++)
    {
        m_DrawRect[4][m_RectNum[4]].top = m_rgEPCarPos[i].top + rcSearchRegion.top;
        m_DrawRect[4][m_RectNum[4]].left = m_rgEPCarPos[i].left;
        m_DrawRect[4][m_RectNum[4]].right = m_rgEPCarPos[i].right;
        m_DrawRect[4][m_RectNum[4]].bottom = m_rgEPCarPos[i].bottom + rcSearchRegion.top;
        m_RectNum[4]++;
        if(m_RectNum[4] >= DRAW_RECT_LEN)m_RectNum[4] = DRAW_RECT_LEN - 1;
    }*/
    
    return S_OK;
}

//���ݼ��Ľ�������г�����ɨ��
HRESULT CTrackerImpl::CheckEPNoPlateConfidence2(HV_COMPONENT_IMAGE* pImgCalibratedFrame, int iVideID)
{
    //�ܷ�ѳߴ罵һ��
    //������û֡������������Ժ�Ҫ�ĳ�3֡��һ��
    CRect rcMax(0, 0, m_nWidth, m_nHeight);
    int iDetMinScale = 8;
    int iDetMaxScale = 25;
    //int iDetMinScale = 7;
    //int iDetMaxScale = 17;
    MULTI_ROI MultiROI=
    {
        ARRSIZE(m_rgEPCarPos),
            m_rgEPCarPos
    };
    //m_RectNum[4] = 0;
    for(int i = 0; i < m_cEPObjTrackInfo; i++)
    {
        if(m_rgEPObjTrackInfo[i].m_objTrackState == OBJ_TRACK_TEM)
        {
            //if(((m_rgEPObjTrackInfo[i].m_iObjInfoCount) & 3) != 3)continue;
            CRect roi = m_rgEPObjTrackInfo[i].LastInfo().m_rcFG;
            if(roi.bottom < (m_nHeight>>1))
            {
                continue;
            }
            CRect rLarge = roi;
            //����roi
            int iWidth = roi.Width();
            int iHeight = roi.Height();
            
            rLarge.left -= iWidth*3;
            rLarge.right += iWidth*3;
            rLarge.top -= iHeight*3;
            rLarge.bottom += iHeight*3;
           
            rLarge.IntersectRect(&rLarge, &rcMax);
            //HV_Trace("rLarge.width [%d], rLarge.height [%d]", rLarge.Width(), rLarge.Height());
            //rLarge is the search area
            HV_COMPONENT_IMAGE imgCarRegion;
            imgCarRegion.nImgType = HV_IMAGE_YUV_422;
            RTN_HR_IF_FAILED(CropImage(*pImgCalibratedFrame, rLarge, &imgCarRegion));
            //MultiROI.dwNumROI = 0;
            //memset(&MultiROI, 0x0, sizeof(MultiROI));

            RTN_HR_IF_FAILED(m_pPlateDetector->ScanPlatesComponentImage(
                CAR_DETECTOR,
                &imgCarRegion, //imgCalibratedFrame,
                &MultiROI,
                iDetMinScale,//m_pParam->g_nDetMinScaleNum,
                iDetMaxScale,//m_pParam->g_nDetMaxScaleNum,
                TRUE,
                0,
                FALSE,
                NULL,
                NULL
                ));
            if(MultiROI.dwNumROI > 0)
            {
                for (int j = 0 ; j < MultiROI.dwNumROI; j++ )
                {
                    //�ж��Ƿ��ģ��ľ��ο��Ƿ��ཻ��
                    CRect rDetect;
                    CRect rInterSet(0,0,0,0);
                    rDetect.top = m_rgEPCarPos[j].top + rLarge.top;
                    rDetect.bottom = m_rgEPCarPos[j].bottom + rLarge.top;
                    rDetect.left = m_rgEPCarPos[j].left + rLarge.left;
                    rDetect.right = m_rgEPCarPos[j].right + rLarge.left;
                    
                    //m_DrawRect[4][m_RectNum[4]] = rDetect;
                    
                   // m_RectNum[4]++;
                    //if(m_RectNum[4] >= DRAW_RECT_LEN)m_RectNum[4] = DRAW_RECT_LEN - 1;
                    
                    if(rInterSet.IntersectRect(&rDetect, &roi))
                    {
                        //good!
                        m_rgEPObjTrackInfo[i].m_iEPNoPlateScore += 100;
                        break;
                    }
                    
                }
            }
            
        }
    }
    return S_OK;
}
void CTrackerImpl::SetTriggerInfo(const DWORD32 dwLastImageTime
								 ,const DWORD32 dwCarArriveTime
						         ,const DWORD32 dwRoadId
						         ,const DWORD32 dwTriggerIndex
						         )
{
	if (m_cfgParam.iPlatform == 0)
	{
		return;
	}
	PBYTE8 pbTmp = (PBYTE8)m_cfgParam.cTriggerInfoBuf.phys;
	if (pbTmp && m_cfgParam.cTriggerInfoBuf.len >= sizeof(TRIGGER_CAM_INFO))
	{
		TRIGGER_CAM_INFO cTriggerCamInfo;
		cTriggerCamInfo.dwCarArriveTime = dwCarArriveTime;
		cTriggerCamInfo.dwRoadId = dwRoadId;
		cTriggerCamInfo.dwTriggerIndex = dwTriggerIndex;
		memcpy(pbTmp, &cTriggerCamInfo, sizeof(cTriggerCamInfo));

		// todo. Ҫ������
		//CACHE_wbL2(pbTmp, sizeof(cTriggerCamInfo), CACHE_WAIT);
		HV_writeback(pbTmp, sizeof(cTriggerCamInfo));

		/*char szInfo[256] = {0};
		sprintf(szInfo, "DSP Trigger Info LTime(%d) ATime(%d) RTime(%d) Road(%d) Index(%d)\n"
				,dwLastImageTime
				,cTriggerCamInfo.dwCarArriveTime
				,GetSystemTick()
				,cTriggerCamInfo.dwRoadId
				,cTriggerCamInfo.dwTriggerIndex
				);
		strcat(g_szDebugInfo, szInfo);*/
	}
}

//ץ�ĸ���(�г���)
void CTrackerImpl::CapTrack(CTrackInfo* pTrack)
{
	if (pTrack->m_fIsTrigger)//�Ѿ�ץ�Ĺ�
	{
		return;
	}

	bool fCanTrigger = false; //ҵ�����Ƿ�����ץ��
	if (m_fEnableCarArriveTrigger)
	{
		if(!m_iTriggerPlateType //���г���
		  ||(m_iTriggerPlateType == 1 && pTrack->LastInfo().color == PC_YELLOW)//������
		)
		{
			fCanTrigger = true;
		}
	}
	if (false  == fCanTrigger)
	{
		return;
	}

	bool fRealFirst = false;
	DWORD32 dwCurTick = GetSystemTick();
	if ((dwCurTick - m_dwLastTriggerTick) > MIN_TRIGGER_TIME
		|| m_cfgParam.iPlatform == 2) // ˮ��ƽ̨������ץ�ļ��ʱ��
	{
		fRealFirst = true;
		++m_dwTriggerCameraTimes;
		if(m_dwTriggerCameraTimes >= 0xffff)
		{
			m_dwTriggerCameraTimes = 1;
		}
		//��ץ����Ϣ��¼�������ڴ���
		SetTriggerInfo(pTrack->LastInfo().dwFrameTime
		               ,pTrack->m_nCarArriveTime
					   ,pTrack->m_iRoad
					   ,m_dwTriggerCameraTimes
					   );
		
		TriggerCamera(pTrack->m_iRoad);
		m_dwLastTriggerTick = dwCurTick;
	}
	pTrack->m_dwTriggerIndex = m_dwTriggerCameraTimes;
	pTrack->m_iCapCount = 1;

	if (m_cfgParam.iPlatform == 0 && m_cfgParam.iCapMode)//�ڶ���ץ��
	{
		bool fSecond = false;
		if (m_cfgParam.iCapMode == 1)
		{
			if (m_cfgParam.iSpeedLimit != 0)
			{
				float fltCarSpeed(0.0f);
				float fltScaleOfDistance(1.0f);
				CalcCarSpeed(fltCarSpeed, fltScaleOfDistance, (*pTrack));
				pTrack->m_iCapSpeed = fltCarSpeed;
				if (fltCarSpeed > m_cfgParam.iSpeedLimit * 0.9)
				{
					fSecond = true;
				}
			}
		}
		else if (m_cfgParam.iCapMode == 2)
		{
			fSecond = true;
		}
		if (fSecond)
		{
			//ʹCarArriveTime���һ��ץ�ĵ�ʱ����ӽӽ�
			if (false == fRealFirst)
			{
				pTrack->m_nCarArriveTime = m_dwLastTriggerTick;
			}
			
			pTrack->m_iCapCount = 2;

			++m_dwTriggerCameraTimes;
			if(m_dwTriggerCameraTimes >= 0xffff)
			{
				m_dwTriggerCameraTimes = 1;
			}
			HV_Sleep(5);
			TriggerCamera(pTrack->m_iRoad);
			m_dwLastTriggerTick = dwCurTick;
		}
	}
	pTrack->m_fIsTrigger = true;
}

//ץ������(�޳���)
void CTrackerImpl::CapObjTrack(CObjTrackInfo* pObjTrack)
{
	if (pObjTrack->m_fIsTrigger)//�Ѿ�ץ�Ĺ�
	{
		return;
	}
	
	//ҵ�����Ƿ�����ץ��
	if (false == m_fEnableCarArriveTrigger || m_iTriggerPlateType)
	{
		return;
	}

	bool fRealFirst = false;
	DWORD32 dwCurTick = GetSystemTick();
	if ((dwCurTick - m_dwLastTriggerTick) > MIN_TRIGGER_TIME)
	{
		fRealFirst = true;
		++m_dwTriggerCameraTimes;
		if(m_dwTriggerCameraTimes >= 0xffff)
		{
			m_dwTriggerCameraTimes = 1;
		}
		//��ץ����Ϣ��¼�������ڴ���
		SetTriggerInfo(pObjTrack->LastInfo().m_dwFrameTime
					  ,pObjTrack->m_nCarArriveTime
		              ,pObjTrack->m_iRoad
		              ,m_dwTriggerCameraTimes
		              );
		TriggerCamera(pObjTrack->m_iRoad);
		m_dwLastTriggerTick = dwCurTick;
	}
	
	pObjTrack->m_dwTriggerIndex = m_dwTriggerCameraTimes;
	pObjTrack->m_iCapCount = 1;
	
	if (m_cfgParam.iCapMode == 2)
	{
		//ʹCarArriveTime���һ��ץ�ĵ�ʱ����ӽӽ�
		if (false == fRealFirst)
		{
			pObjTrack->m_nCarArriveTime = m_dwLastTriggerTick;
		}
		
		pObjTrack->m_iCapCount = 2;

		++m_dwTriggerCameraTimes;
		if(m_dwTriggerCameraTimes >= 0xffff)
		{
			m_dwTriggerCameraTimes = 1;
		}
		HV_Sleep(5);
		TriggerCamera(pObjTrack->m_iRoad);
		m_dwLastTriggerTick = dwCurTick;
	}
	
	pObjTrack->m_fIsTrigger = true;
}


//��Ӧ���޸ģ������㷨���ܴ����·���߽�ȥ����
BOOL CTrackerImpl::RainyFlashControl(HiVideo::CRect* pPos, 
									 int iVideoID)
{
	//������ҹ��������2�ĳ�������
	CRect rcMax(0, 0, m_nWidth, m_nHeight);
	CRect rcVideoDet;
	RTN_HR_IF_FAILED(MapParamRect(rcVideoDet, rcMax, m_rcVideoDetArea));

	int iLineWidth = 0;
	int iLineNum = 0;
	BOOL fFlash = true;
	iLineNum = MatchRoad(pPos->CenterPoint().x, pPos->CenterPoint().y, &iLineWidth);
	float fltRadio = (iLineWidth == 0) ? (1.0f) : (pPos->Width()/(float)iLineWidth);

	if(iLineNum == 1)//�ұ߳���
	{
		IReferenceComponentImage* pCurrGray = m_rgpObjectDetector[iVideoID]->GetCurrFrame();
		if (pCurrGray != NULL)
		{
			CPersistentComponentImage imgGray;
			pCurrGray->GetImage(&imgGray);
			//�ڿ���������û�г��ƣ���ȱ����Ƿ����
			//���û�г��ƣ��ұȽ���������
			int iDownScaleX, iDownScaleY;
			HV_RECT rect;
			iDownScaleX = iDownScaleY = m_rgpObjectDetector[iVideoID]->GetMultiDownSampleScale();
			HvSize S = hvSize(imgGray.iWidth, imgGray.iHeight);
			HvSize orgSize = hvSize(CTrackInfo::m_iWidth, CTrackInfo::m_iHeight);
			CoodinateTrans(iDownScaleX, 
				iDownScaleY, 
				rcVideoDet.TopLeft(), *pPos, rect 
				, S, orgSize,
				COODINATE_RECT_SHRINK_SHIFT);
			//HV_Trace("rect.top")
			//20120425 liujie modify
			//���ָ÷����е�����������ʱ�ϳ�����Լ30ms
			//�ֲ������������
			//if(RainyFlashCheck(&imgGray, rect, iDownScaleX, iDownScaleY))fFlash = false;
			if(RainyFlashCheck2(&imgGray, rect, iDownScaleX, iDownScaleY))fFlash = false;
			if (fFlash)
			{
				//�鿴���еļ������Ƿ��е���
				//�ҵ�����ײ��ĵ��飬��bottomֵ��Ϊ��������ץ������
				CRect rLastYRect(0,0,0,0);
				int iMaxBottom = 0; 
				for (int i = 0; i < m_iLightDetected; i++)
				{
					if(m_rgrcDetected[i].bottom > iMaxBottom)
					{
						iMaxBottom = m_rgrcDetected[i].bottom;
						rLastYRect = m_rgrcDetected[i];
					}
				}
				if (iMaxBottom != 0)
				{
					if(iMaxBottom < rcVideoDet.CenterPoint().y)fFlash = false;
				}
			}
			//end of modify 20120425
			if (fFlash)
			{
				//��������ҿ�߱Ȳ����ϳ��������˵�
				if (fltRadio < 0.35f && rect.right > S.width - 5)
				{
					fFlash = false;
				}
			}

		}
	}
	return fFlash;
}

//��Ӧ�Ե��޸ķ���������Ƶ��������ͻ�ƺ����Ҫȥ��!
//����ʹ���ڳ��п���
BOOL CTrackerImpl::RainyObjOutPutControl(HiVideo::CRect* pPos, int iTrackID)
{
	CRect rcMax(0, 0, m_nWidth, m_nHeight);
	CRect rcVideoDet;
	RTN_HR_IF_FAILED(MapParamRect(rcVideoDet, rcMax, m_rcVideoDetArea));
	//Ӧ�����е����ʷ�켣ͳ�������ֿ��Ƿ�ߵ��ϵĳ���
	BOOL fOutPut = true;
	CObjTrackInfo* pObjTrack = m_rgObjTrackInfo + iTrackID;
	//�����3֡Ū����
	//3֡�������ҿ�Ƚ�С
	int iCount = pObjTrack->m_iObjInfoCount;
	int iLineWidth = 0;
	int iLineNum = 0;
	BOOL fFlash = true;
	float fltRadio1, fltRadio2, fltRadio3;

	iLineNum = MatchRoad(pPos->CenterPoint().x, pPos->CenterPoint().y, &iLineWidth);
	float fltRadio = (iLineWidth == 0) ? (1.0f) : (pPos->Width()/(float)iLineWidth);

	if(iCount > 6 && iLineNum == 1)
	{
		CRect roi1 = pObjTrack->LastInfo().m_rcFG;
		CRect roi2 = pObjTrack->m_rgObjInfo[iCount - 2].m_rcFG;
		CRect roi3 = pObjTrack->m_rgObjInfo[iCount - 3].m_rcFG;

		fltRadio1 = (iLineWidth == 0) ? (1.0f) : (roi1.Width()/(float)iLineWidth);
		fltRadio2 = (iLineWidth == 0) ? (1.0f) : (roi2.Width()/(float)iLineWidth);
		fltRadio3 = (iLineWidth == 0) ? (1.0f) : (roi3.Width()/(float)iLineWidth);

		//���ٽ��̶�
		int iDis = 0;
		iDis += (abs(rcVideoDet.right - roi1.right) + abs(rcVideoDet.right - roi1.left))/2;
		iDis += (abs(rcVideoDet.right - roi2.right) + abs(rcVideoDet.right - roi2.left))/2;
		iDis += (abs(rcVideoDet.right - roi3.right) + abs(rcVideoDet.right - roi3.left))/2;
		iDis /= 3;
		//HV_Trace("\n iDis:[%d]", iDis);
		//if(iDis < 150)fOutPut = false;

		if (fltRadio1 < 0.35f 
			&& fltRadio2 < 0.38f 
			&& fltRadio3 < 0.40f
			//&& roi1.right > rcVideoDet.right - 20 
			//&& roi2.right > rcVideoDet.right - 20
			//&& roi3.right > rcVideoDet.right - 20
			&& iDis < m_iEdgeFilterThreld
			)
		{
			fOutPut = false;
		}       
	}
	return fOutPut;
}

bool CTrackerImpl::IsContainOfTrap(TRAP_DETAREA trapDetArea, CRect rectPlate)
{
	bool bret = false;
	int iLX = -1;
	int iRX = -1;
	int iTY = -1;
	int iBY = -1;

	// ��̬�޸����󣬲����þ�̬��
	float fltTopLeftX = (float)(1.0f * trapDetArea.TopLeftX * m_nWidth / 100 + 0.5);
	float fltTopLeftY = (float)(1.0f * trapDetArea.TopLeftY * m_nHeight / 100 + 0.5);
	float fltBottomLeftX = (float)(1.0f * trapDetArea.BottomLeftX* m_nWidth / 100 + 0.5);
	float fltBottomLeftY = (float)(1.0f * trapDetArea.BottomLeftY * m_nHeight / 100 + 0.5);
	float fltTopRightX = (float)(1.0f * trapDetArea.TopRightX * m_nWidth / 100 + 0.5);
	float fltTopRightY = (float)(1.0f * trapDetArea.TopRightY * m_nHeight / 100 + 0.5);
	float fltBottomRightX = (float)(1.0f * trapDetArea.BottomRightX * m_nWidth / 100 + 0.5);
	float fltBottomRightY = (float)(1.0f * trapDetArea.BottomRightY * m_nHeight / 100 + 0.5);

	//�����
	float fltDX1 = fltTopLeftX - fltBottomLeftX;
	float fltDY1 = fltTopLeftY - fltBottomLeftY;	
	if(fltDX1 == 0.0f) fltDX1 = 1.0f;
	if(fltDY1 == 0.0f) fltDY1 = 1.0f;

	float fltLDY = (float)rectPlate.CenterPoint().y - fltBottomLeftY;
	iLX = (int)((fltDX1 / fltDY1) * fltLDY + fltBottomLeftX + 0.5);

	//�ұ���
	float fltRDX1 = fltTopRightX - fltBottomRightX;
	float fltRDY1 = fltTopRightY - fltBottomRightY;	
	if(fltRDX1 == 0.0f) fltRDX1 = 1.0f;
	if(fltRDY1 == 0.0f) fltRDY1 = 1.0f;

	float fltRDY = (float)rectPlate.CenterPoint().y - fltBottomRightY;
	iRX = (int)((fltRDX1 / fltRDY1) * fltRDY + fltBottomRightX + 0.5);

	//�ϱ���
	float fltTDX1 = fltTopRightX - fltTopLeftX;
	float fltTDY1 = fltTopRightY - fltTopLeftY;	
	if(fltTDX1 == 0.0f) fltTDX1 = 1.0f;
	if(fltTDY1 == 0.0f) fltTDY1 = 1.0f;

	float fltTDX = (float)rectPlate.CenterPoint().x - fltTopLeftX;
	iTY = (int)((fltTDY1 / fltTDX1 ) * fltTDX + fltTopLeftY + 0.5);

	//�±���
	float fltBDX1 = fltBottomRightX - fltBottomLeftX;
	float fltBDY1 = fltBottomRightY - fltBottomLeftY;	
	if(fltBDX1 == 0.0f) fltBDX1 = 1.0f;
	if(fltBDY1 == 0.0f) fltBDY1 = 1.0f;

	float fltBDX = (float)rectPlate.CenterPoint().x - fltBottomLeftX;
	iBY = (int)((fltBDY1 / fltBDX1 ) * fltBDX + fltBottomLeftY + 0.5);


	//�жϰ�����ϵ
	if(rectPlate.CenterPoint().x >= iLX &&  rectPlate.CenterPoint().x <= iRX && rectPlate.CenterPoint().y >= iTY && rectPlate.CenterPoint().y <= iBY)
	{
		bret = true;
	}
	return bret;
}

HRESULT CTrackerImpl::GetColorResult(PBYTE8 RBG1,PBYTE8 RBG2,bool *fDiffThr, int &nColor)
{
	int Color1,Color2;
	//�����ж�
	if(RBG1[5] > 130)
		Color2 = 4;
	else if(RBG1[5] < 90)
		Color2 = 3;
	else
	{
		if(RBG1[3] > 80 && RBG1[3] < 190)
			Color2 = 4;
		else
			Color2 = 3;
	}
	//��ɫ�ж�
	if(*fDiffThr)
	{
		if(RBG2[3]>=20 && RBG2[3]<60 && RBG2[4]>=RBG2[5])  //����RBG2[4]>=RBG2[5]Ϊ�˱�֤û�а���ʶ��ɻ�ɫ����ͨ�����жϣ�
			Color1 = 2;
		else if(RBG2[3]>=60 && RBG2[3]<100)
			Color1 = 6;
		else if(RBG2[3]>=100 && RBG2[3]<190)   //���ڰ���ʶ������ƣ����жϴ���+��ƫ����
			Color1 = 1;
		else
			Color1 = 4;
	}
	else
	{
		if(RBG2[5] > 110)
			Color1 = 4;
		else
			Color1 = 3;
	}
	//��ɫ���
	if(Color1 == 2 && Color2 == 3)
		nColor = 2;
	else if(Color1 == 1 && Color2 == 4)
		nColor = 1;
	else if(Color1 == 3 && Color2 == 4)
		nColor = 3;
	else if(Color1 == 4 && Color2 == 3)
		nColor = 4;
	else if(Color1 == 1 && Color2 == 3)
		nColor = 5;
	else if(Color1 == 6 && Color2 == 4)
		nColor = 6;
	else
		nColor = 0;   //�Ƿ����ֱ������Ϊ4

	return  S_OK;
}
//�����ƫ���İ��ƻ�ƫ�Ƶİ���
HRESULT CTrackerImpl::GetPlateColor(PBYTE8 RBG1,PBYTE8 RBG2, int &nColor)
{
	//����RGB�Ĳ�ֵ
	int DiffRGB1,DiffRGB2;
	DiffRGB1 = HV_ABS(RBG1[2]-RBG1[1]) + HV_ABS(RBG1[2]-RBG1[0]) + HV_ABS(RBG1[1]-RBG1[0]);
	DiffRGB2 = HV_ABS(RBG2[2]-RBG2[1]) + HV_ABS(RBG2[2]-RBG2[0]) + HV_ABS(RBG2[1]-RBG2[0]);
	//ȡ��������
	int FontIdx = 0;
	if(DiffRGB1>DiffRGB2)  //�������ʹ�����ɫ�����жϴ���
	{
		if(RBG1[3]<60 && RBG2[3]<60)   //����
		{
			if(RBG1[5] > RBG2[5])
				FontIdx = 2;
			else
				FontIdx = 1;
		}
		else if(RBG1[3]<180 && RBG2[3]<180)  //����
		{
			if(RBG1[5] > RBG2[5])
				FontIdx = 1;
			else
				FontIdx = 2;
		}
		else
		{
			FontIdx = 0;
		}
	}
	else
	{
		FontIdx = 1;
	}
	//��ɫʶ��
	bool DiffThr = false;
	if(FontIdx == 1)
	{
		if(DiffRGB2 > 60) DiffThr = true;
		GetColorResult(RBG1,RBG2,&DiffThr,nColor);  //�����ɫ��Ϣ�ɿ?
	}
	else if(FontIdx == 2)                           //���ֳ�����ɫʶ�����
	{
		if(DiffRGB1 > 60) DiffThr = true;
		GetColorResult(RBG2,RBG1,&DiffThr,nColor);
		if(nColor == 3)
			nColor = 4;
		else if(nColor == 4)
			nColor = 3;
	}
	else
	{
		nColor = 0;
	}

	return  S_OK;
}
HRESULT CTrackerImpl::CalcPlateColor(HV_COMPONENT_IMAGE imgFrame,PBYTE8 pBW, int &nColor)
{
	int Y1=0,U1=0,V1=0,nCount1=0;
	int Y2=0,U2=0,V2=0,nCount2=0;
	//�޸ĸ߶Ⱥ������
	int nTop = imgFrame.iHeight/3;
	int nBottom = imgFrame.iHeight - nTop;
	int nLeft = imgFrame.iWidth/6;
	int nRight = imgFrame.iWidth - nLeft;

	RESTRICT_PBYTE8 pYLine = GetHvImageData(&imgFrame, 0) + nTop*imgFrame.iStrideWidth[0];
	RESTRICT_PBYTE8	pULine = GetHvImageData(&imgFrame, 1) + nTop*imgFrame.iStrideWidth[1];
	RESTRICT_PBYTE8 pVLine = GetHvImageData(&imgFrame, 2) + nTop*imgFrame.iStrideWidth[2];
	RESTRICT_PBYTE8 pBinLine = pBW+ nTop*imgFrame.iWidth;
	for(int i=nTop;i<=nBottom;i++,
		pYLine += imgFrame.iStrideWidth[0], 
		pULine += imgFrame.iStrideWidth[1],
		pVLine += imgFrame.iStrideWidth[2],
		pBinLine += imgFrame.iWidth)
	{
		for(int j=nLeft;j<=nRight;j++)
		{
			if(*(pBinLine + j) == 255)
			{
				Y1+= pYLine[j];
				U1+= pULine[j>>1];
				V1+= pVLine[j>>1];
				nCount1++;
			}
			else
			{
				Y2+= pYLine[j];
				U2+= pULine[j>>1];
				V2+= pVLine[j>>1];
				nCount2++;
			}
		}
	}
	//�õ�YUV��Ϣ��������YUVת����HSLRGB
	BYTE8 cTempRGB1[6],cTempRGB2[6];
	if (nCount1==0) nCount1=1;
	if (nCount2==0) nCount2=1;
	YCbCr2BGR(Y1/nCount1,U1/nCount1,V1/nCount1,&cTempRGB1[2],&cTempRGB1[1],&cTempRGB1[0]);
	YCbCr2BGR(Y2/nCount2,U2/nCount2,V2/nCount2,&cTempRGB2[2],&cTempRGB2[1],&cTempRGB2[0]);
	BGR2HSL(cTempRGB1[2],cTempRGB1[1], cTempRGB1[0], &cTempRGB1[3], &cTempRGB1[4], &cTempRGB1[5]);
	BGR2HSL(cTempRGB2[2],cTempRGB2[1], cTempRGB2[0], &cTempRGB2[3], &cTempRGB2[4], &cTempRGB2[5]);
	GetPlateColor(cTempRGB1,cTempRGB2,nColor);
	return  S_OK;

}

// �������ģ�͵ĸ��½ӿ�
HRESULT CTrackerImpl::UpdateModel(unsigned int uModelId, unsigned char* pbData, unsigned int cbDataSize)
{
	// todo.
	// �ڴ�����ģ�͵Ķ�̬���ء�
	char szMsg[256];
	sprintf(szMsg, "UpdateModel. id:%d, pbData:%08x, size:%d.",
			uModelId, pbData, cbDataSize);
	OutPutDebugMsg(LOG_LV_DEBUG, szMsg);
	return S_OK;
}

// ��̬�����㷨�����ӿ�
HRESULT CTrackerImpl::UpdateAlgParam(unsigned char* pbData, unsigned int cbDataSize)
{
	// todo.
	// �ڴ����Ӳ�������
	char szMsg[256];
	sprintf(szMsg, "UpdateAlgParam. pbData:%08x, size:%d.",
			pbData, cbDataSize);
	OutPutDebugMsg(LOG_LV_DEBUG, szMsg);
	return S_OK;
}


//=================------------------------------------=============================
