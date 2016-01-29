#include "SWFC.h"
#include "SWEPoliceEventCheckerTransformFilter.h"
#include "SWCarLeft.h"

CSWEPoliceEventCheckerTransformFilter::CSWEPoliceEventCheckerTransformFilter()
	: CSWBaseFilter(1,1)
{
	GetIn(0)->AddObject(CLASSID(CSWCarLeft));	
	GetOut(0)->AddObject(CLASSID(CSWString));
	m_dwLastOutputTime = 0;
	swpa_memset(m_rgdwLastCarArriveTime, 0, sizeof(m_rgdwLastCarArriveTime));
	swpa_memset(m_rgEventInfoSum, 0, sizeof(m_rgEventInfoSum));
	m_dwTotalRoadNum = 0;
	m_dwStartRoadNum = 0;
	m_dwInterval = 300000;
	m_semEvent.Create(0, 1);
}

CSWEPoliceEventCheckerTransformFilter::~CSWEPoliceEventCheckerTransformFilter()
{
}

HRESULT CSWEPoliceEventCheckerTransformFilter::Initialize(DWORD dwInterval)
{
	if(dwInterval > 0)
	{
		m_dwInterval = dwInterval;
	}
	return S_OK;
}

HRESULT CSWEPoliceEventCheckerTransformFilter::Run()
{
	if(S_OK == CSWBaseFilter::Run())
	{
		return m_cThreadEvent.Start(OnEvent, this);
	}
	return E_FAIL;
}

HRESULT CSWEPoliceEventCheckerTransformFilter::Stop()
{
	m_semEvent.Post();
	m_cThreadEvent.Stop();
	return CSWBaseFilter::Stop();
}
	
HRESULT CSWEPoliceEventCheckerTransformFilter::Receive(CSWObject* obj)
{
	if(IsDecendant(CSWCarLeft, obj))
	{
		CSWCarLeft *pCarLeft = (CSWCarLeft *)obj;
		if (pCarLeft->GetRoadNo() >= 0 && pCarLeft->GetRoadNo() < MAX_ROAD_NUM)
		{	//���泵����������ʼ������
			m_dwTotalRoadNum = pCarLeft->GetTrackerParam()->nRoadLineNumber - 1;
			m_dwStartRoadNum = pCarLeft->GetTrackerParam()->iStartRoadNum;
			//��������
			m_rgEventInfoSum[pCarLeft->GetRoadNo()].dwCarCount++;
			//�������ٶ�
			m_rgEventInfoSum[pCarLeft->GetRoadNo()].dwCarSpeed += pCarLeft->GetCarspeed();
			if(pCarLeft->GetCarspeed())
			{
				m_rgEventInfoSum[pCarLeft->GetRoadNo()].dwOccupancy += (DWORD32)(RUN_DISTANCE / pCarLeft->GetCarspeed());
			}
			//���㳵��ռ����
      DWORD dwNow = 0;
      for(int i = 0; i < 5; i++)
      {
      	if (pCarLeft->GetImage(i) && dwNow < pCarLeft->GetImage(i)->GetRefTime())
      	{
          dwNow = pCarLeft->GetImage(i)->GetRefTime();
      	}
    	}
      if (m_rgdwLastCarArriveTime[pCarLeft->GetRoadNo()] != 0)
      {
          m_rgEventInfoSum[pCarLeft->GetRoadNo()].dwCarDistance += (dwNow - m_rgdwLastCarArriveTime[pCarLeft->GetRoadNo()]);
      }
      m_rgdwLastCarArriveTime[pCarLeft->GetRoadNo()] = dwNow;
      
      //У��ʱ��
      DWORD32 dwTmp = m_dwLastOutputTime;
      if (dwTmp < pCarLeft->GetFirstFrameTime())
      {
          dwTmp = pCarLeft->GetFirstFrameTime();
      }
      if (dwNow < pCarLeft->GetFirstFrameTime())
      {
          dwNow = pCarLeft->GetFirstFrameTime();
      }
      if (dwNow < dwTmp)
      {
          dwNow = dwTmp;
      }
      m_rgEventInfoSum[pCarLeft->GetRoadNo()].dwOccupancy += (dwNow - dwTmp);      
      if (CT_LARGE == pCarLeft->GetCarType() && PLATE_DOUBLE_MOTO != pCarLeft->GetPlateType())
      {
          m_rgEventInfoSum[pCarLeft->GetRoadNo()].dwCamionOccupancy += (dwNow - dwTmp);
      }
        
      CSWDateTime dt1(dwNow), dt2(dwTmp);
      SW_TRACE_DEBUG(
      	"\n����:%d "
      	"\n���ƺ���:%s "
      	"\n���ʱ��:%04d-%02d-%02d %02d:%02d:%02d:%03d "
      	"\n�뿪ʱ��:%04d-%02d-%02d %02d:%02d:%02d:%03d "
      	"\n�ó���ռ��ʱ��:%d ms "
      	"\n��ǰͬ�����ڵ������г�ռ��ʱ��:%d ms "
      	"\n��ռ��ʱ��:%d ms",
      	pCarLeft->GetRoadNo(),
      	(LPCSTR)pCarLeft->GetPlateNo(),
      	dt2.GetYear(), dt2.GetMonth(), dt2.GetDay(), dt2.GetHour(), dt2.GetMinute(), dt2.GetSecond(), dt2.GetMSSecond(),
      	dt1.GetYear(), dt1.GetMonth(), dt1.GetDay(), dt1.GetHour(), dt1.GetMinute(), dt1.GetSecond(), dt1.GetMSSecond(),
      	dwNow - dwTmp,
      	m_rgEventInfoSum[pCarLeft->GetRoadNo()].dwOccupancy,
      	m_rgEventInfoSum[pCarLeft->GetRoadNo()].dwCamionOccupancy
      );
		}
	}
	return S_OK;
}

PVOID CSWEPoliceEventCheckerTransformFilter::OnEvent(PVOID pvParam)
{
	CSWEPoliceEventCheckerTransformFilter *pThis = (CSWEPoliceEventCheckerTransformFilter *)pvParam;
	while(S_OK == pThis->m_cThreadEvent.IsValid() && FILTER_RUNNING == pThis->GetState())
	{	//�ȴ��¼���Ӧ
		if(S_OK == pThis->m_semEvent.Pend(pThis->m_dwInterval))
		{
			break;
		}
		//���ͽ�ͨ��Ϣ�ɼ�
		if (pThis->m_dwLastOutputTime)
    {
    	CSWDateTime dt1(pThis->m_dwLastOutputTime);
    	CSWDateTime dt2;
    	CSWString strEvent;
    	//ͳ��ʱ����
    	strEvent.Format("ͳ��ʱ��%04d-%02d-%02d %02d:%02d:%02d:%03d -- %04d-%02d-%02d %02d:%02d:%02d:%03d\n",
    		dt1.GetYear(), dt1.GetMonth(), dt1.GetDay(), dt1.GetHour(), dt1.GetMinute(), dt1.GetSecond(), dt1.GetMSSecond(),
      	dt2.GetYear(), dt2.GetMonth(), dt2.GetDay(), dt2.GetHour(), dt2.GetMinute(), dt2.GetSecond(), dt2.GetMSSecond()
    	);
    	DWORD dwTime = CSWDateTime::GetSystemTick() - pThis->m_dwLastOutputTime;
    	//ͳ��ÿ����������Ϣ
    	for (int i = 0; i < pThis->m_dwTotalRoadNum; i++)
    	{
    	    FLOAT fltPer1 = (float)(pThis->m_rgEventInfoSum[i].dwOccupancy * 100.0/ dwTime);
    	    FLOAT fltPer2 = (float)(pThis->m_rgEventInfoSum[i].dwCamionOccupancy * 100.0/ dwTime);
    	    INT iAvg = 0;
    	    INT iSpeed = 0;
    	    if (pThis->m_rgEventInfoSum[i].dwCarCount > 0)
    	    {
    	        iSpeed = pThis->m_rgEventInfoSum[i].dwCarSpeed / pThis->m_rgEventInfoSum[i].dwCarCount;
    	        iAvg = pThis->m_rgEventInfoSum[i].dwCarDistance / (pThis->m_rgEventInfoSum[i].dwCarCount * 1000);
    	    }
    	    
    	    CSWString strTmp;
					strTmp.Format("����:%d\n����ͳ��:%d\nռ����ͳ��:%.2f%%\n��ռ����ͳ��:%.2f%%\nƽ���ٶ�ͳ��:%dkm/h\n��ͷʱ�����:%d��\n",
    	        i + pThis->m_dwStartRoadNum,
    	        pThis->m_rgEventInfoSum[i].dwCarCount,
    	        swpa_min(fltPer1, 100),
    	        swpa_min(fltPer2, 100),
    	        iSpeed,
    	        swpa_min(iAvg, pThis->m_dwInterval)
    	    );
    	    strEvent += strTmp;
    	}
    	SW_TRACE_DEBUG("%s", (LPCSTR)strEvent);
    	CSWString *strTmp = new CSWString(strEvent);
    	pThis->GetOut(0)->Deliver(strTmp);
    	strTmp->Release();
    	swpa_memset(pThis->m_rgEventInfoSum, 0, sizeof(pThis->m_rgEventInfoSum));
    }
    pThis->m_dwLastOutputTime = CSWDateTime::GetSystemTick();
	}
	return 0;
}