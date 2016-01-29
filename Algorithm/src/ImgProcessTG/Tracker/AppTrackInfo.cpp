#include "AppTrackInfo.h"
#include "AppUtils.h"

using namespace sv;

namespace swTgApp 
{

CAppTrackInfo::TRACK_STATE TrackState2InfoState(int nTrackState)
{
    using namespace svTgIrApi;

    CAppTrackInfo::TRACK_STATE tsRet;

    switch(nTrackState)
    {
    case ITgTrack::TS_NEW:
        tsRet = CAppTrackInfo::TS_NEW;
        break;

    case ITgTrack::TS_TRACKING:
        tsRet = CAppTrackInfo::TS_TRACKING;
        break;

    case ITgTrack::TS_END:
        tsRet = CAppTrackInfo::TS_END;
        break;

    default:
        tsRet = CAppTrackInfo::TS_END;
    }

    return tsRet;
}

CAppTrackInfo::TRACK_TYPE TrackType2InfoType(svTgIrApi::ITgTrack::TRACK_TYPE nTrackType)
{
    using namespace svTgIrApi;

    CAppTrackInfo::TRACK_TYPE ttRet;

    // ������������
    switch(nTrackType)
    {
    case ITgTrack::TT_LARGE_CAR:
        ttRet = CAppTrackInfo::TT_LARGE_CAR;
        break;

    case ITgTrack::TT_MID_CAR:
        ttRet = CAppTrackInfo::TT_MID_CAR;
        break;

    case ITgTrack::TT_SMALL_CAR:
        ttRet = CAppTrackInfo::TT_SMALL_CAR;
        break;

    case ITgTrack::TT_BIKE:
        ttRet = CAppTrackInfo::TT_BIKE;
        break;

    case ITgTrack::TT_WALK_MAN:
        ttRet = CAppTrackInfo::TT_WAKL_MAN;
        break;
		
    default:
        ttRet = CAppTrackInfo::TT_UNKNOW;
    }

    return ttRet;
}

CAR_COLOR TgCarColor2CarColor(svTgIrApi::CAR_COLOR nTgColor)
{
    CAR_COLOR color;

    switch(nTgColor)
    {
    case svTgIrApi::CC_GREY:
        color = CC_GREY;
        break;

    case svTgIrApi::CC_WHITE:
        color = CC_WHITE;
        break;

    case svTgIrApi::CC_BLACK:
        color = CC_BLACK;
        break;

    case svTgIrApi::CC_RED:
        color = CC_RED;
        break;

    case svTgIrApi::CC_GREEN:
        color = CC_GREEN;
        break;

    case svTgIrApi::CC_BLUE:
        color = CC_BLUE;
        break;

    case svTgIrApi::CC_YELLOW:
        color = CC_YELLOW;
        break;

    default:
        color = CC_UNKNOWN;
    }

    return color;
}

CRoadInfo* CAppTrackInfo::m_pRoadInfo = NULL;
sv::SV_UINT32 CAppTrackInfo::s_iCurImageTick = 0;

CAppTrackInfo::CAppTrackInfo(void)
    :   m_pimgBestSnapShot(NULL)
    ,   m_pimgLastSnapShot(NULL)
{
    Free();
}


CAppTrackInfo::~CAppTrackInfo(void)
{
    Free();
}

unsigned int CAppTrackInfo::GetID()
{
    return m_dwID;
}

CAppTrackInfo::TRACK_STATE CAppTrackInfo::GetState()
{
    return m_nTrackState;
}

CAppTrackInfo::TRACK_TYPE CAppTrackInfo::GetType()
{
    return m_nTrackType;
}

int CAppTrackInfo::GetPosCount()
{
    return m_pTrack->GetLocusCount();
}

CSvRect CAppTrackInfo::GetPos(int nIndex)
{
    return m_pTrack->GetLocus(nIndex).m_rcPos;
}

CSvRect CAppTrackInfo::GetLastPos()
{
    return *(CSvRect*)&m_rcCurPos;
}

const svTgIrApi::TG_TRACK_LOCUS& CAppTrackInfo::GetLocus(int nIndex)
{
    return m_pTrack->GetLocus(nIndex);
}

const svTgIrApi::TG_TRACK_LOCUS& CAppTrackInfo::GetLastLocus()
{
    return m_pTrack->GetLocus(m_pTrack->GetLocusCount() - 1);
}

const svTgIrApi::TG_PLATE_INFO& CAppTrackInfo::GetPlate(int nIndex)  // ȡ������ʶ�𵽵ĳ���
{
    return m_pTrack->GetPlate(nIndex);
}

int CAppTrackInfo::GetPlateCount()
{
    return m_pTrack->GetPlateCount();
}

void CAppTrackInfo::End()
{
    // ע�⣺���ٽ��������յȲ���Ӧ����Free()�н��У������ڴ˽��С�
    // ����ÿ�����ٽ���������˺���
    m_pTrack->End();
    Update(m_pTrack); // End֮���㷨������һЩ������ͶƱ�ȣ���Ҫ����Update
}

sv::SV_RESULT CAppTrackInfo::GetResult(svTgIrApi::ITgTrack::TG_RESULT_INFO* pResInfo)
{
    return m_pTrack->GetResult(pResInfo);
}

sv::SV_RESULT CAppTrackInfo::GetExInfo(svTgIrApi::TG_TRACK_EX_INFO* pExInfo)
{
    return m_pTrack->GetExInfo(pExInfo);
}


int CAppTrackInfo::GetRoadNum()
{
    svTgIrApi::TG_TRACK_EX_INFO exInfo;
    m_pTrack->GetExInfo(&exInfo);

    return exInfo.nRoadNum;
}

// int CAppTrackInfo::GetPlateSimilarityCount()
// {
//     return m_cLastPlate.nSimilaryCount;
// }

void CAppTrackInfo::Free()
{
    m_pTrack = NULL;
    m_dwID = 0xFFFFFFFF;
    m_nTrackState = TS_FREE;

    ReleaseIReferenceComponentImage(m_pimgBestSnapShot);
    ReleaseIReferenceComponentImage(m_pimgLastSnapShot);

    m_nReverseRunCount = 0;

    for (int i=0; i<svTgIrApi::PLATE_TYPE_COUNT; ++i)
    {
        m_rgBestPlateInfo[i].Clear();
    }

    m_nCarArriveTime = 0;

    m_fCanOutput = false;

    m_rcBestPlatePos.SetRect(-1, -1, -1, -1);
    m_rcLastPlatePos.SetRect(-1, -1, -1, -1);

    m_fCarArrived =  FALSE;
    m_fIsTrigger  =  FALSE;
    m_nCarArriveRealTime = 0;

	m_fCaptureFrameHavePlate = false;
	m_nBestPlateSimilar = 0;
	m_nLastPlateSimilar = 0;

    m_nLastPlateY = 0;  
	
	m_dwTriggerCameraTimes = 0;
}

sv::SV_BOOL CAppTrackInfo::IsUpdated()
{
    return ((m_pTrack != NULL) && (m_pTrack->GetID() == m_dwID));
}

void CAppTrackInfo::SetRoadInfo(CRoadInfo* pRoadInfo)
{
    m_pRoadInfo = pRoadInfo;
}

void CAppTrackInfo::Update(svTgIrApi::ITgTrack* pITgTrack)
{
    m_pTrack = pITgTrack;
    // IDһ��Ҫ������������Ϊָ�����ݿ��ܻ�䣬��һ��ƥ�仹��ָ��ȡID�᲻��
    m_dwID = pITgTrack->GetID();

    // ������������
    m_nTrackType = TrackType2InfoType(pITgTrack->GetType());

    // ���¸���״̬
    m_nTrackState = TrackState2InfoState(pITgTrack->GetState());

    pITgTrack->GetExInfo(&m_cExInfo);
    m_nCarColor = TgCarColor2CarColor(m_cExInfo.nCarColor);

    m_rcCurPos = pITgTrack->GetLastPos();

    if (m_nTrackState == TS_END)
    {
        svTgIrApi::ITgTrack::TG_RESULT_INFO tkRes;
        pITgTrack->GetResult(&tkRes);
        m_nCarColor = TgCarColor2CarColor(tkRes.nCarColor);
        m_nTrackType = TrackType2InfoType(tkRes.nTrackType);
    }

    //     // ����ʱ�᷵��ʧ�ܣ�������Ҫ
    //     bool fHavePlate = pITgTrack->GetPlate(&m_cLastPlate);
    //
    //     nRecogInValidCount = m_cLastPlate.nRecogInValidCount;
    //
    //     // �״θ��¼�¼����λ��
    //     if (fHavePlate && GetLastPlateDetPosCount() == GetPosCount())
    //     {
    //         SV_RECT rcCurPlate;
    //         rcCurPlate.left = m_cLastPlate.rcDetPos.m_nLeft;
    //         rcCurPlate.right = m_cLastPlate.rcDetPos.m_nRight;
    //         rcCurPlate.top = m_cLastPlate.rcDetPos.m_nTop;
    //         rcCurPlate.bottom = m_cLastPlate.rcDetPos.m_nBottom;
    //         if (m_nPlateMovePosCount == 0
    //             || !IsOverLap(rcCurPlate, m_rgrcPlateMovePos[m_nPlateMovePosCount-1], 95)
    //             && m_nPlateMovePosCount < m_nMaxPlateMovePos)
    //         {
    //             m_rgrcPlateMovePos[m_nPlateMovePosCount] = rcCurPlate;
    //             m_rgdwPlateTick[m_nPlateMovePosCount] = CAppTrackInfo::s_iCurImageTick;
    //             ++m_nPlateMovePosCount;
    //         }
    //     }
}


sv::SV_RESULT CAppTrackInfo::UpdateAll(
    CAppTrackInfo* rgDjObj,
    int iMaxObj,
    int* piObjCnt,  // �������
    svTgIrApi::ITgTrack* rgpTracker[],
    int iTrackerCnt
)
{
    // ITgTrack to CDjAppObj
    // ע�⣬ITgTrackΪ��ʱ��CDjAppObj�е����н�����Զ����ɿ�����Ϊֱ����ITgTrackָ�룩��
    // ֻ��ID�ɿ�����������ID����CDjAppObj�е�ITgTrackָ��
    svTgIrApi::ITgTrack** rgResult = new svTgIrApi::ITgTrack*[iTrackerCnt];

    if(rgResult == NULL)
    {
        return RS_E_OUTOFMEMORY;
    }

    for(int i = 0; i < iTrackerCnt; ++i)
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
    //sv::utTrace("ITgTrack cnt:%d\n", iTrackerCnt);
    // ��ÿ��Obj�Ҷ�Ӧ��Result
    for(int i = 0; i < iObjCnt; ++i)
    {
        if(rgDjObj[i].GetState() == CAppTrackInfo::TS_END)  // �ϴ��˳��ģ���������
        {
            rgDjObj[i].Free();
            continue;
        }

        int j = 0;

        for(; j < iTrackerCnt; ++j)
        {
            if(rgResult[j] != NULL
                    && rgDjObj[i].GetID() == rgResult[j]->GetID())
            {
                rgDjObj[i].Update(rgResult[j]);
#if RUN_PLATFORM == PLATFORM_WINDOWS

                //sv::utTrace("Tracker obj Update [%d]\n", rgDjObj[i].GetID());
                //                 if(rgDjObj[i].GetPosCount() == 1
                //                         && rgDjObj[i].GetState() == CAppTrackInfo::TS_END
                //                   )
                //                 {
                //                     printf("TrackInfo Update Err0 [%d], p%d s%d\n", rgDjObj[i].GetID(),
                //                            rgDjObj[i].GetPosCount(), rgDjObj[i].GetState());
                //                     system("pause");
                //                 }

#endif

                rgResult[j] = NULL; // ʹ�ù����ÿգ�Ϊ�������Ƿ�ʹ�ù�
                break;
            }
        }

        if(j == iTrackerCnt)    // ûƥ�䵽
        {
            rgDjObj[i].Free();
        }
    }

    // ��ÿһ�� rgDjObj ���������Ϊ�������µĽ�����롣ͬʱ���յ��Ƴ��ж�
    int iRemainResIndex = 0;
    int iEmptyObjIndex = -1;

    for(int i = 0; i < iMaxObj; ++i)
    {

        if(rgDjObj[i].IsUpdated()     // ȡ�����
                || rgDjObj[i].GetState() == CAppTrackInfo::TS_END)      // ����Ϊ�˳�
        {
            // Obj�ǿգ������ƶ���ǰ��յ�λ��
            if(iEmptyObjIndex != -1)
            {
                // ����ʱע��ֻ�ܴ����ݿ���������ֱ�Ӹ�ֵ��ʱ�����������������²���ȷ
                char rgchbuf[sizeof(CAppTrackInfo)];
                memcpy(rgchbuf, &rgDjObj[iEmptyObjIndex], sizeof(CAppTrackInfo));
                memcpy(&rgDjObj[iEmptyObjIndex], &rgDjObj[i], sizeof(CAppTrackInfo));
                memcpy(&rgDjObj[i], rgchbuf, sizeof(CAppTrackInfo));
                ++iEmptyObjIndex;
                //printf("SWT %d-%d MPT+[%d]\n", iEmptyObjIndex, i, iEmptyObjIndex);
            }

            continue;
        }

        // Obj���пɸ���ֵ

        {
            int iObjIndex = (iEmptyObjIndex == -1) ? i : iEmptyObjIndex;

            if(rgDjObj[iObjIndex].m_pTrack != NULL)
            {
                // �쳣���
                rgDjObj[iObjIndex].Free();
            }
        }

        // ��δ����Ľ��
        for(; iRemainResIndex < iTrackerCnt; ++iRemainResIndex)
        {
            if(rgResult[iRemainResIndex] != NULL)
            {
                break;
            }
        }

        if(iRemainResIndex != iTrackerCnt)
        {
            // �ҵ��µ�
            CAppTrackInfo* pNewObj = NULL;

            if(iEmptyObjIndex != -1)
            {
                pNewObj = &rgDjObj[iEmptyObjIndex];
                ++iEmptyObjIndex;
            }
            else
            {
                pNewObj = &rgDjObj[i];
            }

            pNewObj->Update(rgResult[iRemainResIndex]);

#if SV_RUN_PLATFORM == SV_PLATFORM_WIN

            //sv::utTrace("Tracker obj new [%d]\n", pNewObj->GetID());
            if(rgResult[iRemainResIndex]->GetLocusCount() != 1
              )
            {
                printf("TrackInfo Update Err [%d], p%d s%d, ��һ�μ쵽Ӧ�ù켣��Ϊ1\n", rgResult[iRemainResIndex]->GetID(),
                       rgResult[iRemainResIndex]->GetLocusCount(), rgResult[iRemainResIndex]->GetState());
                system("pause");
            }

#endif
            //             if (pNewObj->GetState() == ITgTrack::TS_END)
            //             {
            //                 pNewObj->m_fExit = true;
            //                 sv::SV_Trace("EXIT[OBJ] %d\n", pNewObj->GetID());
            //             }
            rgResult[iRemainResIndex] = NULL;
        }
        else
        {
            // �����꣬û����
            if(iEmptyObjIndex == -1)
            {
                iEmptyObjIndex = i;
                //printf("SET MPT [%d]\n",  iEmptyObjIndex);
            }

        }
    }

    iObjCnt = (iEmptyObjIndex != -1) ? iEmptyObjIndex : iMaxObj;

    *piObjCnt = iObjCnt;
    delete [] rgResult;

    return RS_S_OK;
}

}
