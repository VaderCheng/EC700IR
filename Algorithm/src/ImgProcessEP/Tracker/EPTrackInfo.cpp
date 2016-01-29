#include "EPTrackInfo.h"
#include "EPAppUtils.h" 

using namespace svEPApi;

  static bool IsOverLap(HV_RECT& rc0, HV_RECT& rc1, int p)
  {
      int l = SV_MAX(rc0.left,rc1.left);
      int t = SV_MAX(rc0.top,rc1.top);
      int r = SV_MIN(rc0.right,rc1.right);
      int b = SV_MIN(rc0.bottom,rc1.bottom);

      if ( l>=r || t>=b)
      {
          return false;
      }

      int nArea0 = (rc0.right - rc0.left + 1)*(rc0.bottom - rc0.top + 1);
      int nArea1 = (rc1.right - rc1.left + 1)*(rc1.bottom - rc1.top + 1);
      int nArea2 = (b-t+1) * (r-l+1);

      return (100 * nArea2) >= p * SV_MIN(nArea0,nArea1);
  }
  
CEPTrackInfo::TRACK_STATE TrackState2InfoState(int nTrackState)
{
    CEPTrackInfo::TRACK_STATE tsRet;
    switch(nTrackState)
    {
    case IEPTrack::TS_NEW:
        tsRet = CEPTrackInfo::TS_NEW;
        break;
    case IEPTrack::TS_TRACKING:
        tsRet = CEPTrackInfo::TS_TRACKING;
        break;
    case IEPTrack::TS_RESET:
        tsRet = CEPTrackInfo::TS_RESET;
        break;
    default:
        tsRet = CEPTrackInfo::TS_FREE;
    }
    return tsRet;
}

CRoadInfo* CEPTrackInfo::m_pRoadInfo = NULL;
DWORD32 CEPTrackInfo::s_iCurImageTick = 0;

CEPTrackInfo::CEPTrackInfo(void)
    :   m_pimgBestSnapShot(NULL)
    ,   m_pimgLastSnapShot(NULL)
    ,   m_pimgBeginCapture(NULL)
    ,   m_pimgBestCapture(NULL)
    ,   m_pimgLastCapture(NULL)
{
    Free();
}


CEPTrackInfo::~CEPTrackInfo(void)
{
    Free();
}

int CEPTrackInfo::GetID()
{
    return m_dwID;
}

int CEPTrackInfo::GetState()
{
    return m_nTrackState;
}

int CEPTrackInfo::GetType()
{
    return m_nTrackType;
}

int CEPTrackInfo::GetPosCount()
{
    return m_pEPTrack->GetPosCount();
}

HV_RECT CEPTrackInfo::GetPos(int nIndex)
{
	sv::SV_RECT rc = m_pEPTrack->GetPos(nIndex);
    return *(HV_RECT*)&rc;
}

HV_RECT CEPTrackInfo::GetLastPos()
{
	sv::SV_RECT rc = m_pEPTrack->GetLastPos();
    return *(HV_RECT*)&rc;
}

void CEPTrackInfo::Reset()
{
    m_pEPTrack->Reset();
    Update(m_pEPTrack); // RESET֮���㷨������һЩ������ͶƱ�ȣ���Ҫ����Update
}

CAR_COLOR CEPTrackInfo::GetColor()
{
    return m_pEPTrack->GetColor();
}

BOOL CEPTrackInfo::GetPlate(EP_PLATE_INFO* pPlateInfo)
{
    // ��ǰ֡ûȡ
    if (GetLastPlateDetPosCount() != 0)
    {
        *pPlateInfo = m_cLastPlate;
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

int CEPTrackInfo::GetPlateRecogCount()
{
    return m_pEPTrack->GetPlateRecogCount();
}

float CEPTrackInfo::GetPlateAvgConf()
{
    return m_cLastPlate.fltConf;
}

float CEPTrackInfo::GetPlateAvgFirstConf()
{
    return m_cLastPlate.fltFirstConf;
}

HV_RECT CEPTrackInfo::GetLastPlateDetPos()
{
    return *(HV_RECT*)&m_cLastPlate.rcDetPos;
}

HV_RECT CEPTrackInfo::GetLastPlateRecogPos()
{
    return *(HV_RECT*)&m_cLastPlate.rcPos;
}

int CEPTrackInfo::GetLastPlateDetPosCount()
{
    return m_cLastPlate.nDetPosIndex + 1;
}

float CEPTrackInfo::GetLastPlateConf()
{
    return m_cLastPlate.fltConf;
}

float CEPTrackInfo::GetLastPlateFirstConf()
{
    return m_cLastPlate.fltFirstConf;
}
  
int CEPTrackInfo::GetLastPlateRecogPosCount()
{
    return m_cLastPlate.nRecogPosIndex + 1;
}

int CEPTrackInfo::GetVotedPlateTypeCount()
{
    return m_cLastPlate.nVotePlateTypeCount;
}

int CEPTrackInfo::GetValidDetCount()
{
    return m_pEPTrack->GetValidDetCount();
}

/// ��ǰ����״̬�Ƿ�Ϊѹ�ߣ�����ֹͣ�߷����ж�
BOOL CEPTrackInfo::IsPressRoadLine(int* pnRoadNum)
{
    if (m_nPressRoadLineNum != -1)
    {
        if (pnRoadNum != NULL)
        {
            *pnRoadNum = m_nPressRoadLineNum;
        }
        return TRUE;
    }
    return FALSE;  
}

/// ��ǰ����״̬�Ƿ�Ϊֹͣ
BOOL CEPTrackInfo::IsStop()
{
    return FALSE; // TODO  
}

/// ��ǰ����״̬�Ƿ�����
BOOL CEPTrackInfo::IsReverseRun()
{
    return (m_nReverseRunCount > 0); 
}

/// ��ǰ�Ƿ�ΪԽ�� 
BOOL CEPTrackInfo::IsCrossRoadLine(int* pnRoadNum/*= NULL*/)
{
    if (m_nCrossRoadLineNum != -1)
    {
        if (pnRoadNum != NULL)
        {
            *pnRoadNum = m_nCrossRoadLineNum;
        }
        return TRUE;
    }
    return FALSE;  
}

int CEPTrackInfo::GetRoadNum()
{
    return m_pEPTrack->GetRoadNum();
}

RUN_TYPE CEPTrackInfo::GetRunType()
{
    return m_nRunType;
}

int CEPTrackInfo::GetPlateSimilarityCount()
{
    return m_cLastPlate.nSimilaryCount;
}

void CEPTrackInfo::Free()
{
    m_pEPTrack = NULL;
    m_dwID = 0xFFFFFFFF;
    m_nTrackState = TS_FREE;

    ReleaseIReferenceComponentImage(m_pimgBestSnapShot);
    ReleaseIReferenceComponentImage(m_pimgLastSnapShot);
    ReleaseIReferenceComponentImage(m_pimgBeginCapture);
    ReleaseIReferenceComponentImage(m_pimgBestCapture);
    ReleaseIReferenceComponentImage(m_pimgLastCapture);	

    m_nPressRoadLineNum = -1;
    m_nReverseRunCount = 0;
    m_nCrossRoadLineNum = -1;

    m_iOnePosLightScene = m_iTwoPosLightScene = m_iThreePosLightScene = -1;
    m_iPassStopLightScene = m_iPassLeftStopLightScene = -1;

    m_fltMaxPlateConf = 0.f;
    m_fTrackReliable = false;

    for (int i=0; i<PLATE_TYPE_COUNT; ++i)
    {
        m_rgBestPlateInfo[i].Clear();
    }

    m_cLastPlate.nDetPosIndex = -1;
    m_cLastPlate.nRecogPosIndex = -1;

    m_cLastDetPlateInfo.Clear();

    m_nPlateMovePosCount = 0;
    m_nCarArriveTime = 0;

    nRecogInValidCount = 0;
}

BOOL CEPTrackInfo::IsUpdated()
{
    return ((m_pEPTrack != NULL) && (m_pEPTrack->GetID() == m_dwID));
}

HRESULT CEPTrackInfo::UpdateAll(
    CEPTrackInfo* rgDjObj,
    int iMaxObj,
    int* piObjCnt,  // �������
    IEPTrack** rgpTracker, 
    int iTrackerCnt
    )
{
    // IEPTrack to CDjAppObj
    // ע�⣬IEPTrackΪ��ʱ��CDjAppObj�е����н�����Զ����ɿ�����Ϊֱ����IEPTrackָ�룩��
    // ֻ��ID�ɿ�����������ID����CDjAppObj�е�IEPTrackָ��
    IEPTrack** rgResult = new IEPTrack*[iTrackerCnt];
    if (rgResult == NULL)   return E_OUTOFMEMORY;

    for (int i=0; i<iTrackerCnt; ++i)
    {
        rgResult[i] = rgpTracker[i];
    }
    int iObjCnt = *piObjCnt;

    //     //dbg
    //     printf("STEP1[OBJ] ");
    //     for (int i=0; i<iMaxObj; ++i)
    //     { if (!rgDjObj[i].IsGetResult())
    //     printf("R%d ", rgDjObj[i].GetID()); 
    //     else
    //         printf("%d ", rgDjObj[i].GetID()); 
    //     }printf("\n");

    // ��ÿ��Obj�Ҷ�Ӧ��Result
    for (int i=0; i<iObjCnt; ++i)  
    {
        if (rgDjObj[i].GetState() == CEPTrackInfo::TS_RESET) // �ϴ��˳��ģ���������
        {
            rgDjObj[i].Free();
            continue;
        }

        int j=0;
        for (; j<iTrackerCnt; ++j)
        {
            if (rgResult[j] != NULL
                && rgDjObj[i].GetID() == rgResult[j]->GetID())
            {
                rgDjObj[i].Update(rgResult[j]);
#if RUN_PLATFORM == PLATFORM_WINDOWS
                //sv::utTrace("Tracker obj Update [%d]\n", rgDjObj[i].GetID());
                if (rgDjObj[i].GetPosCount() == 1
                    && rgDjObj[i].GetState() != CEPTrackInfo::TS_RESET
                    )
                {
                    printf("TrackInfo Update Err0 [%d], p%d s%d\n", rgDjObj[i].GetID(), 
                        rgDjObj[i].GetPosCount(), rgDjObj[i].GetState());
                    system("pause");
                }
#endif

                rgResult[j] = NULL; // ʹ�ù����ÿգ�Ϊ�������Ƿ�ʹ�ù�
                break;
            }
        }
        if (j == iTrackerCnt)   // ûƥ�䵽
        {  
            rgDjObj[i].Free();
        }
    }

    // ��ÿһ�� rgDjObj ���������Ϊ�������µĽ�����롣ͬʱ���յ��Ƴ��ж�
    int iRemainResIndex = 0;
    int iEmptyObjIndex = -1;
    for (int i=0; i<iMaxObj; ++i)
    {
        
        if (rgDjObj[i].IsUpdated()    // ȡ�����
            || rgDjObj[i].GetState() == CEPTrackInfo::TS_RESET)      // ����Ϊ�˳�
        {
            // Obj�ǿգ������ƶ���ǰ��յ�λ��
            if (iEmptyObjIndex != -1)   
            {
                // ����ʱע��ֻ�ܴ����ݿ���������ֱ�Ӹ�ֵ��ʱ�����������������²���ȷ
                char rgchbuf[sizeof(CEPTrackInfo)];
                memcpy(rgchbuf, &rgDjObj[iEmptyObjIndex], sizeof(CEPTrackInfo));
                memcpy(&rgDjObj[iEmptyObjIndex], &rgDjObj[i], sizeof(CEPTrackInfo));
                memcpy(&rgDjObj[i], rgchbuf, sizeof(CEPTrackInfo));
                ++iEmptyObjIndex;
                //printf("SWT %d-%d MPT+[%d]\n", iEmptyObjIndex, i, iEmptyObjIndex);
            }
            continue;
        }

        // Obj���пɸ���ֵ 

        {
            int iObjIndex = (iEmptyObjIndex == -1) ? i : iEmptyObjIndex;
            if (rgDjObj[iObjIndex].m_pEPTrack != NULL)   
            {
                // �쳣���
                rgDjObj[iObjIndex].Free();
            }
        }

        // ��δ����Ľ��
        for (; iRemainResIndex<iTrackerCnt; ++iRemainResIndex)
        {
            if (rgResult[iRemainResIndex] != NULL)    break;
        }
        if (iRemainResIndex != iTrackerCnt)   
        {
            // �ҵ��µ�
            CEPTrackInfo* pNewObj = NULL;
            if (iEmptyObjIndex != -1)
            {
                pNewObj = &rgDjObj[iEmptyObjIndex];
                ++iEmptyObjIndex;
            }
            else
            {
                pNewObj = &rgDjObj[i];
            }
            pNewObj->Update(rgResult[iRemainResIndex]);

#if RUN_PLATFORM == PLATFORM_WINDOWS
            //sv::utTrace("Tracker obj new [%d]\n", pNewObj->GetID());
            if (rgResult[iRemainResIndex]->GetPosCount() != 1
                )
            {
                printf("TrackInfo Update Err [%d], p%d s%d\n", rgResult[iRemainResIndex]->GetID(), 
                    rgResult[iRemainResIndex]->GetPosCount(), rgResult[iRemainResIndex]->GetState());
                system("pause");
            }
#endif
            //             if (pNewObj->GetState() == IEPTrack::TS_RESET)  
            //             {
            //                 pNewObj->m_fExit = true;
            //                 sv::SV_Trace("EXIT[OBJ] %d\n", pNewObj->GetID());
            //             }
            rgResult[iRemainResIndex] = NULL;
        }
        else
        {
            // �����꣬û����
            if (iEmptyObjIndex == -1)
            {
                iEmptyObjIndex = i;
                //printf("SET MPT [%d]\n",  iEmptyObjIndex);
            }

        }
    }
    iObjCnt = (iEmptyObjIndex != -1) ? iEmptyObjIndex : iMaxObj;

    *piObjCnt = iObjCnt;
    delete [] rgResult;

    return S_OK;
}

void CEPTrackInfo::SetRoadInfo(CRoadInfo* pRoadInfo)
{
    m_pRoadInfo = pRoadInfo;
}

void CEPTrackInfo::Update(IEPTrack* pIEPTrack)
{
    m_pEPTrack = pIEPTrack;
    // IDһ��Ҫ������������Ϊָ�����ݿ��ܻ�䣬��һ��ƥ�仹��ָ��ȡID�᲻��
    m_dwID = pIEPTrack->GetID();  

    // ������������
    switch(pIEPTrack->GetType())
    {
    case IEPTrack::TT_LARGE_CAR:
        m_nTrackType = TT_LARGE_CAR;
        break;
    case IEPTrack::TT_MID_CAR:
        m_nTrackType = TT_MID_CAR;
        break;
    case IEPTrack::TT_SMALL_CAR:
        m_nTrackType = TT_SMALL_CAR;
        break;
    case IEPTrack::TT_BIKE:
        m_nTrackType = TT_BIKE;
        break;
    case IEPTrack::TT_WALK_MAN:
        m_nTrackType = TT_WALK_MAN;
        break;
    default:
        m_nTrackType = CEPTrackInfo::TT_UNKNOW;
    }

    // ���¸���״̬
    m_nTrackState = TrackState2InfoState(pIEPTrack->GetState());

    // ����ʱ�᷵��ʧ�ܣ�������Ҫ 
    bool fHavePlate = pIEPTrack->GetPlate(&m_cLastPlate);

    nRecogInValidCount = m_cLastPlate.nRecogInValidCount;

    // �״θ��¼�¼����λ��
    if (fHavePlate && GetLastPlateDetPosCount() == GetPosCount())
    {
        HV_RECT rcCurPlate;
        rcCurPlate.left = m_cLastPlate.rcDetPos.m_nLeft;
        rcCurPlate.right = m_cLastPlate.rcDetPos.m_nRight;
        rcCurPlate.top = m_cLastPlate.rcDetPos.m_nTop;
        rcCurPlate.bottom = m_cLastPlate.rcDetPos.m_nBottom;
        if (m_nPlateMovePosCount == 0
            || !IsOverLap(rcCurPlate, m_rgrcPlateMovePos[m_nPlateMovePosCount-1], 95)
            && m_nPlateMovePosCount < m_nMaxPlateMovePos)
        {
            m_rgrcPlateMovePos[m_nPlateMovePosCount] = rcCurPlate;
            m_rgdwPlateTick[m_nPlateMovePosCount] = CEPTrackInfo::s_iCurImageTick;
            ++m_nPlateMovePosCount;
        }
    }
}
