
#include "SWFC.h"
#include "SWRecognizeTransformTollGateFilter.h"

#include "TLDetResult.h"

#include "SWGB28181Parameter.h"
CSWRecognizeTransformTollGateFilter::CSWRecognizeTransformTollGateFilter()
	: CSWBaseFilter(1,4)
	, CSWMessage(MSG_RECOGNIZE_CTRL_START, MSG_RECOGNIZE_CTRL_END)
	, m_pTrackerCfg(NULL)
	, m_fSendJPEG(FALSE)
	, m_fSendDebug(FALSE)
	, m_pThread(NULL)
	, m_fInitialized(FALSE)
	, m_iCarArriveTriggerType(0)
	, m_iIOLevel(0)
	, m_fReverseRunFilterFlag(FALSE)
    , m_pProcQueueThread(NULL)
{
	GetIn(0)->AddObject(CLASSID(CSWImage));
	GetOut(0)->AddObject(CLASSID(CSWPosImage));
	GetOut(1)->AddObject(CLASSID(CSWCarLeft));
	GetOut(2)->AddObject(CLASSID(CSWCameraDataPDU));
	GetOut(3)->AddObject(CLASSID(CSWImage));
}

CSWRecognizeTransformTollGateFilter::~CSWRecognizeTransformTollGateFilter()
{
	Clear();
}

VOID CSWRecognizeTransformTollGateFilter::Clear()
{
}

HRESULT CSWRecognizeTransformTollGateFilter::Initialize(
	INT iGlobalParamIndex
	, INT nLastLightType
	, INT nLastPulseLevel
	, INT nLastCplStatus
	, PVOID pvParam
	)
{
	if( m_fInitialized )
	{
		return E_FAIL;
	}
	if( NULL == pvParam )
	{
		return E_INVALIDARG;
	}

	m_pTrackerCfg = (TRACKER_CFG_PARAM *)pvParam;
	m_pTrackerCfg->iPlatform = 2;

	INIT_VIDEO_RECOGER_PARAM cInitParam;
	cInitParam.nPlateRecogParamIndex = iGlobalParamIndex;
	cInitParam.nLastLightType = nLastLightType;
	cInitParam.nLastPulseLevel = nLastPulseLevel;
	cInitParam.nLastCplStatus = nLastCplStatus;
	cInitParam.dwArmTick = CSWDateTime::GetSystemTick();
	swpa_memcpy(&cInitParam.cTrackerCfgParam, m_pTrackerCfg, sizeof(TRACKER_CFG_PARAM));

	SW_TRACE_DEBUG("<RecognizeTransformFilter>work index : %d.", cInitParam.nPlateRecogParamIndex);
	HRESULT hr = CSWBaseLinkCtrl::GetInstance()->InitVideoRecoger(cInitParam);
	if(FAILED(hr))
	{
		SW_TRACE_DEBUG("RecognizeTransformFilter algorithm initialize failed!\n");
		Clear();
		return hr;
	}

	m_pThread = new CSWThread(this->GetName());
	if( NULL == m_pThread )
	{
		SW_TRACE_DEBUG("RecognizeTransformFilter thread initialize failed!\n");
		Clear();
		return E_OUTOFMEMORY;
	}

    // ganzz �����첽�����߳�
    m_pProcQueueThread = new CSWThread(this->GetName());
    if( NULL == m_pThread )
    {
        SW_TRACE_DEBUG("RecognizeTransformFilter sync thread initialize failed!\n");
        Clear();
        return E_OUTOFMEMORY;
    }
	m_cSemImage.Create(0, MAX_IMAGE_COUNT);
    m_semEP.Create(0, MAX_RPOC_QUEUE_LEN + 1);
    m_lstEPList.SetMaxCount(MAX_RPOC_QUEUE_LEN);

	m_fInitialized = TRUE;
	return S_OK;

}

HRESULT CSWRecognizeTransformTollGateFilter::RegisterCallBackFunction(PVOID OnDSPAlarm, PVOID pvParam)
{
	typedef VOID (*fnOnDSPAlarm)(PVOID pvParam, INT iInfo);
	HRESULT hr = CSWBaseLinkCtrl::GetInstance()->RegisterCallBack((fnOnDSPAlarm)OnDSPAlarm, pvParam);
	SW_TRACE_DEBUG("RegisterCallBackFunction(0x%08x,0x%08x) return 0x%08x\n", (INT)OnDSPAlarm, (INT)pvParam, hr);
	return hr;
}

VOID CSWRecognizeTransformTollGateFilter::OnProcessProxy(PVOID pvParam)
{
	if(pvParam != NULL)
	{
        CSWRecognizeTransformTollGateFilter* pThis = (CSWRecognizeTransformTollGateFilter*)pvParam;
		pThis->OnProcess();
	}
}

VOID CSWRecognizeTransformTollGateFilter::OnProcessSyncProxy(PVOID pvParam)
{
    if(pvParam != NULL)
    {
        CSWRecognizeTransformTollGateFilter* pThis = (CSWRecognizeTransformTollGateFilter*)pvParam;
        pThis->OnProcessSync();
    }
}
HRESULT CSWRecognizeTransformTollGateFilter::Run()
{
	if( !m_fInitialized )
	{
        SW_TRACE_DEBUG("CSWRecognizeTransformTollGateFilter does not Initialize");
		return E_FAIL;
	}

	HRESULT hr = S_OK;
	if( GetState() != FILTER_RUNNING )
	{
		CSWBaseFilter::Run();
        hr = m_pThread->Start((START_ROUTINE)&CSWRecognizeTransformTollGateFilter::OnProcessProxy, (PVOID)this);
        if (hr == S_OK)
        {
            hr = m_pProcQueueThread->Start((START_ROUTINE)&CSWRecognizeTransformTollGateFilter::OnProcessSyncProxy, (PVOID)this);
        }
	}

	return hr;
}

HRESULT CSWRecognizeTransformTollGateFilter::Stop()
{
	if(NULL != m_pThread)
	{
		m_pThread->Stop();
	}
	return CSWBaseFilter::Stop();
}

HRESULT CSWRecognizeTransformTollGateFilter::OnRecognizePhoto(WPARAM wParam, LPARAM lParam)
{
	if( wParam == 0 && lParam == 0 )
	{
		return E_INVALIDARG;
	}

	CSWImage* pImage = (CSWImage*)wParam;
	CSWCarLeftArray *carLeft = (CSWCarLeftArray *)lParam;
	
	PROCESS_EVENT_STRUCT cEvent;

	// �ж��ǲ���ץ��ͼ��
	// ĿǰDSP����Ҫͨ���˱�־��������
	if( !pImage->IsCaptureImage() )
	{
		SW_TRACE_DEBUG("<RecognizeTransformFilter>Not captrue image, can't recognize!\n");
		return E_INVALIDARG;
	}
	DWORD dwNow = CSWDateTime::GetSystemTick();
	HRESULT hr = CSWBaseLinkCtrl::GetInstance()->ProcessOneFrame(pImage, &cEvent);	
	SW_TRACE_DEBUG("CSWBaseLinkCtrl::GetInstance()->ProessOneFrame %d ms", CSWDateTime::GetSystemTick() - dwNow);
	if(swpa_utils_file_exist("./test.txt"))
	{
		cEvent.dwEventId = EVENT_CARLEFT;
		cEvent.iCarLeftInfoCount = 2;
	}
	
	if(S_OK == hr && (cEvent.dwEventId & EVENT_CARLEFT))
	{
		carLeft->SetCount(cEvent.iCarLeftInfoCount);
		for(int i = 0; i < cEvent.iCarLeftInfoCount; i++)
		{
			carLeft->Get(i)->SetParameter(m_pTrackerCfg, &cEvent.rgCarLeftInfo[i]);
			if(swpa_utils_file_exist("./test.txt"))
			{
				carLeft->Get(i)->SetPlateNo("����");
				carLeft->Get(i)->SetCarColor(CC_WHITE);
			}
		}
	}
	return hr;
}

/**
*@brief ����ʶ�𿪹�
*/
HRESULT CSWRecognizeTransformTollGateFilter::OnRecognizeGetJPEG(WPARAM wParam, LPARAM lParam)
{
	m_fSendJPEG = (BOOL)wParam;
	if(!m_fSendJPEG)
	{
		m_fSendDebug = FALSE;
	}
	SW_TRACE_DEBUG("<RecognizeTransformFilter>Set RecognizeEnable %d,JPEG:%d.\n", (UINT)wParam, m_fSendJPEG);
	return S_OK;
}

/**
*@brief �����������
*/
HRESULT CSWRecognizeTransformTollGateFilter::OnRecognizeOutPutDebug(WPARAM wParam, LPARAM lParam)
{
	m_fSendDebug = (BOOL)wParam;
	SW_TRACE_DEBUG("<RecognizeTransformFilter>Set RecognizeOutPutDebug %d.,Debug:%d\n", (UINT)wParam, m_fSendDebug);
	return S_OK;
}

HRESULT CSWRecognizeTransformTollGateFilter::OnTriggerEnable(WPARAM wParam, LPARAM lParam)
{
	SW_TRACE_DEBUG("OnTriggerEnable(0x%08x,0x%08x)", wParam, lParam);
	m_iCarArriveTriggerType = (INT)wParam;
	return S_OK;
}

HRESULT CSWRecognizeTransformTollGateFilter::OnIOEvent(WPARAM wParam, LPARAM lParam)
{
	m_iIOLevel = (INT)wParam;
	SW_TRACE_DEBUG("OnIOEvent(0x%08x,0x%08x)", wParam, lParam);
	return S_OK;
}

/**
 *@brief ��̬�޸�ʶ�����
 */
HRESULT CSWRecognizeTransformTollGateFilter::OnModifyParam(WPARAM wParam, LPARAM lParam)
{
	TRACKER_CFG_PARAM* pCfgParam = (TRACKER_CFG_PARAM*)wParam;
	HRESULT hr = CSWBaseLinkCtrl::GetInstance()->ModifyVideoRecogerParam((*pCfgParam));
	if(FAILED(hr))
	{
		SW_TRACE_DEBUG("ModifyVideoRecogerParam failed!\n");
	}
	m_fModifyParam = TRUE;
	return hr;
}

HRESULT CSWRecognizeTransformTollGateFilter::Receive(CSWObject* obj)
{
	if(IsDecendant(CSWImage, obj))
	{
		CSWImage* pImage = (CSWImage*)obj;
		// ʶ���ʱ�򲻽���ץ��ͼ
		if(pImage->IsCaptureImage())
		{
			if(m_fSendJPEG)
			{
				CSWPosImage* pPosImage = new CSWPosImage(pImage, 0, m_fSendDebug);
				GetOut(0)->Deliver(pPosImage);
				pPosImage->Release();
			}
			return S_OK;
		}
		// ����Э��ȴ������п��пռ�
		// Ĭ��VPIF�ɼ���֡�����ǡ�VPIF����
		if(swpa_strcmp("VPIF", pImage->GetFrameName()) != 0 )
		{
			while( TRUE )
			{
				m_cMutexImage.Lock();
				if( m_lstImage.GetCount() < MAX_IMAGE_COUNT )
				{
					m_cMutexImage.Unlock();
					break;
				}
				else
				{
					m_cMutexImage.Unlock();
					CSWApplication::Sleep(100);
				}
			}
		}
		//�������
		m_cMutexImage.Lock();
		if( m_lstImage.GetCount() < MAX_IMAGE_COUNT )
		{
			pImage->AddRef();
			m_lstImage.AddTail(pImage);
			if( S_OK != m_cSemImage.Post() )
			{
				SW_TRACE_DEBUG("<RecognizeTransformFilter>SemImage post failed. list count:%d.\n", m_lstImage.GetCount());
			}
		}
		m_cMutexImage.Unlock();
	}

	return S_OK;
}

HRESULT CSWRecognizeTransformTollGateFilter::OnProcess()
{
    SW_TRACE_DEBUG("CSWRecognizeTransformTollGateFilter run.\n");

	DWORD dwFps = 0;
	DWORD dwBeginTick = CSWDateTime::GetSystemTick();


	CSWCameraDataPDU* pCamPDU = NULL;
	m_fModifyParam = TRUE;

	while(S_OK == m_pThread->IsValid() && GetState() == FILTER_RUNNING)
	{
		if( S_OK != m_cSemImage.Pend(1000) )
		{
			SW_TRACE_DEBUG("m_cSemImage.Pend failed.");
			continue;
		}
		m_cMutexImage.Lock();
		if( !m_lstImage.IsEmpty() )
		{
			//SW_TRACE_DEBUG("m_fEnable=%d,m_fOutPutDebug=%d", m_fEnable, m_fOutPutDebug);
			CSWImage* pImage = m_lstImage.RemoveHead();
			m_cMutexImage.Unlock();

			if(NULL != pImage && m_fModifyParam )
			{
			    m_sTLCtrl.Init(m_pTrackerCfg);
			    m_fModifyParam = FALSE;
			    SAFE_RELEASE(pImage);
			    continue;
			}
			// TODO Process
			SW_COMPONENT_IMAGE cImageCop;
			pImage->GetImage(&cImageCop);
            //SW_TRACE_DEBUG("ImgType %d,%d\n", pImage->GetType(), cImageCop.cImageType);
			HV_COMPONENT_IMAGE hvImageYuv;

			hvImageYuv.nImgType = HV_IMAGE_YUV_420;
			hvImageYuv.iHeight = (pImage->GetHeight()) >> 1;
			hvImageYuv.iWidth = pImage->GetWidth();

			hvImageYuv.iStrideWidth[0] = (cImageCop.rgiStrideWidth[0]) << 1;
			hvImageYuv.iStrideWidth[1] = (cImageCop.rgiStrideWidth[1]) << 1;
			hvImageYuv.iStrideWidth[2] = 0;

			hvImageYuv.rgImageData[0].addr = cImageCop.rgpbData[0];
			hvImageYuv.rgImageData[1].addr = cImageCop.rgpbData[1];
			hvImageYuv.rgImageData[2].addr = 0;

//             static unsigned int s_nLastTick = 0;
// 
//             while (CSWDateTime::GetSystemTick() - s_nLastTick < 80)
//             {
//                 CSWApplication::Sleep(100);
//             }
//             if (CSWDateTime::GetSystemTick() > s_nLastTick + 90)
//             {
//                 SW_TRACE_DEBUG("WASTE TIME : %d\n", CSWDateTime::GetSystemTick() - s_nLastTick - 80);   
//             }
//             s_nLastTick = CSWDateTime::GetSystemTick(); 

			//todo.   
            const int MAX_SYNC_DATA_SIZE = 1024 * 32;
			int iDataSize = 0;
			unsigned char* pbData = new unsigned char[MAX_SYNC_DATA_SIZE];

			if( !m_fSendJPEG || m_fSendDebug )
			{
                static int xx = 0;
                if(m_pTrackerCfg && m_pTrackerCfg->iOutPutNoPlate)
                {
//                    if(xx++%100==0)
//                    {
//                        xx=0;
//                        SW_TRACE_DEBUG("CSWRecognizeTransformTollGateFilter �����Ƴ�\n");
//                    }
                        m_sTLCtrl.Process(hvImageYuv,
                            pbData, MAX_SYNC_DATA_SIZE, &iDataSize);//yyyyyy
                }
                else
                {
//                    if(xx++%100==0)
//                    {
//                        xx=0;
//                        SW_TRACE_DEBUG("CSWRecognizeTransformTollGateFilter �����Ƴ�\n");
//                    }
                    iDataSize= 128;
                }
			}
            //if (pImage->GetFrameName() != NULL) SW_TRACE_DEBUG("FRAME: %s\n", pImage->GetFrameName());

			m_cMutexEPList.Lock();
			if( m_lstEPList.IsFull() )
			{
                //SW_TRACE_DEBUG("DROP FRAME : %s\n", pImage->GetFrameName());

				PROC_QUEUE_ELEM cElem = m_lstEPList.RemoveTail();
				SAFE_RELEASE(cElem.pImage);
				PROCESS_EVENT_STRUCT* pProcessEvent =
					(PROCESS_EVENT_STRUCT*)cElem.pData;
				if( pProcessEvent != NULL
						&& NULL != pProcessEvent->cSyncDetData.pbData )
				{
					delete[] pProcessEvent->cSyncDetData.pbData;
					pProcessEvent->cSyncDetData.pbData = NULL;
				}
				SAFE_DELETE(pProcessEvent);
			}
			 // ����Ԫ�ؿռ�
			PROCESS_EVENT_STRUCT* pProcessEvent = new PROCESS_EVENT_STRUCT;
			if (pProcessEvent == NULL)
			{
				SW_TRACE_DEBUG("Sync queue out of mem.");
				m_cMutexEPList.Unlock();
				continue;
			}
            
			swpa_memset(pProcessEvent, 0, sizeof(PROCESS_EVENT_STRUCT));
			pProcessEvent->fIsCheckLight = true;
			pProcessEvent->iCarArriveTriggerType = m_iCarArriveTriggerType;
			pProcessEvent->nIOLevel = m_iIOLevel;
			PROC_QUEUE_ELEM cNewElem;
			pImage->AddRef();
			cNewElem.pImage = pImage;
			cNewElem.pData = pProcessEvent;
			pProcessEvent->cSyncDetData.pbData = pbData;
			pProcessEvent->cSyncDetData.nLen = iDataSize;
			m_lstEPList.AddTail(cNewElem);
			m_semEP.Post();
			m_cMutexEPList.Unlock();

            //�ͷ���Դ
            SAFE_RELEASE(pImage);
        }
        else
        {
            m_cMutexImage.Unlock();
        }
	}

	SW_TRACE_DEBUG("RecognizeTransformFilter exit.\n");
	return S_OK;
}

HRESULT CSWRecognizeTransformTollGateFilter::OnProcessSync()
{
    static DWORD dwFps = 0;
    DWORD dwBeginTick = CSWDateTime::GetSystemTick();

    CSWPosImage* pPosImage = NULL;
    CSWCameraDataPDU* pCamPDU = NULL;

    while(S_OK == m_pProcQueueThread->IsValid())
    {
        if( S_OK != m_semEP.Pend(1000) )
        {
            SW_TRACE_DEBUG("m_semEP.Pend failed.");
            continue;
        }
        m_cMutexEPList.Lock();
        PROC_QUEUE_ELEM cQueueElem;
        if( !m_lstEPList.IsEmpty() )
        {
            cQueueElem = m_lstEPList.RemoveHead();
        }
        else
        {
            m_cMutexEPList.Unlock();
            continue;
        }
        m_cMutexEPList.Unlock();

        CSWImage* pImage = cQueueElem.pImage;
        PROCESS_EVENT_STRUCT* pProcessEvent = (PROCESS_EVENT_STRUCT*)(cQueueElem.pData);

        DWORD dwBeginTickRecong = CSWDateTime::GetSystemTick();
        HRESULT hr = S_OK;

        if(!m_fSendJPEG || m_fSendDebug)
        {
#if 1
            hr = CSWBaseLinkCtrl::GetInstance()->ProcessOneFrame(pImage, pProcessEvent);
#else
            hr = S_OK;
#endif
            //�ͷ���Դ
            if(pImage->GetLastImage())
            {
                pImage->Clear();
            }
        }
        if(S_OK == hr)
        {
            if( !m_fSendJPEG || m_fSendDebug )
            {
                if ( pProcessEvent->dwEventId & EVENT_CARARRIVE )
                {
                    for( int i = 0; i < pProcessEvent->iCarArriveInfoCount; ++i )
                    {	 
                        CarArriveEvent(&pProcessEvent->rgCarArriveInfo[i]);
                    }
                }
                if( pProcessEvent->dwEventId & EVENT_CARLEFT )
                {
                    for( int i = 0; i < pProcessEvent->iCarLeftInfoCount; ++i )
                    {
                        CarLeftEvent(&pProcessEvent->rgCarLeftInfo[i]);
                    }
                }					


                //���������
                pCamPDU = new CSWCameraDataPDU();
                pCamPDU->SetLightType(pProcessEvent->cLightType);
                pCamPDU->SetCplStatus(pProcessEvent->iCplStatus);
                pCamPDU->SetPluseLevel(pProcessEvent->iPulseLevel);
                GetOut(2)->Deliver(pCamPDU);
                SAFE_RELEASE(pCamPDU);	
            }	
        }
        else
        {
            SW_TRACE_DEBUG("<RecognizeTransformFilter>ProceeOneFrame failed. 0x%08x.\n", hr);
            // �����DSP��ʱ,��λ�豸.
            if( hr == E_UNEXPECTED )
            {
                CSWMessage::SendMessage(MSG_APP_RESETDEVICE, 0, 0);
            }
        }


        if( m_fSendJPEG )
        {

        	if ( m_fSendDebug )
        	{
				int nDetROICnt;
				svTLDetApi::DET_ROI *prgROI;
				svTLDetApi::SeparateResultRefPoint(
						pProcessEvent->cSyncDetData.pbData,
						pProcessEvent->cSyncDetData.nLen,
						NULL,
						NULL,
						NULL,
						&prgROI,
						&nDetROICnt
					);

				if(pProcessEvent->cTrackRectInfo.dwTrackCount + nDetROICnt> 0)
				{
					//CSWPosImage* pPosImage = NULL;
					SAFE_RELEASE(pPosImage);
					pPosImage = new CSWPosImage(pImage, pProcessEvent->cTrackRectInfo.dwTrackCount+nDetROICnt, m_fSendDebug);

					for(int i = 0; i < pProcessEvent->cTrackRectInfo.dwTrackCount; i++)
					{
						pProcessEvent->cTrackRectInfo.rgTrackRect[i].top *= 2;
						pProcessEvent->cTrackRectInfo.rgTrackRect[i].bottom *= 2;
						pPosImage->GetRect(i) = *(SW_RECT *)&pProcessEvent->cTrackRectInfo.rgTrackRect[i];
						//SW_TRACE_DEBUG("<%d><%d,%d,%d,%d>.\n", pProcessEvent->cTrackRectInfo.dwTrackCount, pProcessEvent->cTrackRectInfo.rgTrackRect[i].left,pProcessEvent->cTrackRectInfo.rgTrackRect[i].right,pProcessEvent->cTrackRectInfo.rgTrackRect[i].top,pProcessEvent->cTrackRectInfo.rgTrackRect[i].bottom);
					}
					int n = pProcessEvent->cTrackRectInfo.dwTrackCount;
					for(int i = 0; i < nDetROICnt; i++)
					{
						SW_RECT rc = *(SW_RECT *)&prgROI[i];
						rc.top *=2;
						rc.bottom *=2;
						pPosImage->GetRect(i+n) = rc;

	//                	pPosImage->GetRect(i+n) = *(SW_RECT *)&prgROI[i];
						//SW_TRACE_DEBUG("<%d><%d,%d,%d,%d>.\n", pProcessEvent->cTrackRectInfo.dwTrackCount, pProcessEvent->cTrackRectInfo.rgTrackRect[i].left,pProcessEvent->cTrackRectInfo.rgTrackRect[i].right,pProcessEvent->cTrackRectInfo.rgTrackRect[i].top,pProcessEvent->cTrackRectInfo.rgTrackRect[i].bottom);
					}
					GetOut(0)->Deliver(pPosImage);
				}
				else
				{
					SAFE_RELEASE(pPosImage);
					pPosImage = new CSWPosImage(pImage, 0, m_fSendDebug);
					GetOut(0)->Deliver(pPosImage);
				}
        	}
        	else
        	{
        		SAFE_RELEASE(pPosImage);
				pPosImage = new CSWPosImage(pImage, 0, m_fSendDebug);
				GetOut(0)->Deliver(pPosImage);
        	}
        }

        SAFE_RELEASE(pPosImage);

        // print fps
        if( dwFps++ >= 50 )
        {
            DWORD dwCurTick = CSWDateTime::GetSystemTick();
            CHAR szInfo[256] = {0};
            CHAR szMsg[256] = {0};
            swpa_sprintf(szInfo, "recognize fps: %.1f JPEG:%d,Debug:%d.", float(50*1000) / (dwCurTick - dwBeginTick), m_fSendJPEG, m_fSendDebug);

            SW_TRACE_NORMAL("--- %s---\n", szInfo);

            swpa_sprintf(szMsg, "ʶ��:%s", szInfo);
            CSWMessage::SendMessage(MSG_APP_UPDATE_STATUS, (WPARAM)szMsg, 0);

            dwBeginTick = dwCurTick;
            dwFps = 0;
        }
        //�ͷ���Դ
        SAFE_RELEASE(pImage);
        if( NULL != pProcessEvent->cSyncDetData.pbData )
        {
            delete[] pProcessEvent->cSyncDetData.pbData;
            pProcessEvent->cSyncDetData.pbData = NULL;
        }
        SAFE_DELETE(pProcessEvent);
    }
    /*
    if( m_fSendJPEG)
    {
    //�����������YUV��
    if(NULL == pPosImage)
    {
    pPosImage = new CSWPosImage(pImage, 0, m_fSendDebug);
    }
    GetOut(0)->Deliver(pPosImage);
    }
    SAFE_RELEASE(pPosImage);		
    SAFE_RELEASE(pImage);
    */

}

HRESULT CSWRecognizeTransformTollGateFilter::CarArriveEvent(CARARRIVE_INFO_STRUCT *pCarArriveInfo)
{
	//todo..
	return S_OK;
}

HRESULT CSWRecognizeTransformTollGateFilter::CarLeftEvent(CARLEFT_INFO_STRUCT *pCarLeftInfo)
{
	static int siPlateCount = 0;
	siPlateCount++;
	SW_TRACE_DEBUG("<RecognizeTransformFilter>car:%d.\n", siPlateCount);


	if(NULL == pCarLeftInfo)
	{
		return E_INVALIDARG;
	}
	//֪ͨGraph����CarLeft����
	CSWCarLeft* pCarLeft = NULL;
	SendCommand(1, (WPARAM)&pCarLeft, (LPARAM)pCarLeftInfo);
	if( pCarLeft == NULL || !IsDecendant(CSWCarLeft, pCarLeft))
	{
		SAFE_RELEASE(pCarLeft);
		SW_TRACE_DEBUG("pCarLeft == NULL or is not CSWCarLeft object, used default CSWCarLeft\n");
		pCarLeft = new CSWCarLeft();
	}
	pCarLeft->SetParameter(m_pTrackerCfg, pCarLeftInfo);

	if(NULL != pCarLeft)
	{
        GB28181Alarm((LPCSTR)pCarLeft->GetPlateNo());
		CSWString s;
		switch(pCarLeftInfo->cCoreResult.nVoteCondition)
		{
		case NEW2END : s = "NEW״̬�¸��ٶ�ʧ";break;
		case LEAVE_TRACK_RECT: s = "����ģʽ�³����뿪��������";break;
		case TRACK_MISS: s = "�ڸ��������ڸ��ٶ�ʧ";break;
		case ENDRECT_MISS: s = "�ڸ�����������ٶ�ʧ";break;
		case MAX_EQUAL_FRAME: s = "����ʶ������ͬ";break;
		case MAX_VOTE_FRAME: s = "��Ч���������ֵ";break;
		}
		SW_TRACE_NORMAL("object carleft name:%s,PlateNo:%s[0x%08x,0x%08x,0x%08x,0x%08x,0x%08x,0x%08x,0x%08x][%s]", pCarLeft->Name(), (LPCSTR)pCarLeft->GetPlateNo(), pCarLeft->GetImage(0), pCarLeft->GetImage(1), pCarLeft->GetImage(2), pCarLeft->GetImage(3), pCarLeft->GetImage(4), pCarLeft->GetImage(5), pCarLeft->GetImage(6), (LPCSTR)s);

		if (pCarLeft->GetReverseRun()
			&& GetReverseRunFilterFlag())
		{
			SW_TRACE_DEBUG("Info: ReverseRun. Discard this carleft obj.\n");
		}
		else
		{
		        GetOut(1)->Deliver(pCarLeft);
		}
		
		GetOut(3)->Deliver(pCarLeft->GetImage(PLATE_IMAGE));
		pCarLeft->Release();
		SW_TRACE_DEBUG("object carleft done.\n");
	}
	return S_OK;
}
HRESULT CSWRecognizeTransformTollGateFilter::GB28181Alarm(LPCSTR szMsg)
{
    printf("%s %d\n",__FUNCTION__,__LINE__);
    GB28181_AlarmInfo info;
    swpa_strcpy(info.szAlarmMsg,szMsg);
    SWPA_DATETIME_TM tm;
    CSWDateTime dt;
    dt.GetTime(&tm);
    swpa_sprintf(info.szAlarmTime,"%d-%02d-%02dT%02d:%02d:%02d.%03d",tm.year,tm.month,tm.day,tm.hour,tm.min,tm.sec,tm.msec);
    SW_TRACE_NORMAL("CSWRecognizeTransformTollGateFilter::OnSendAlarm %s %s %d ",info.szAlarmTime,info.szAlarmMsg,MSG_GB28181_SEND_ALARM);
    SendRemoteMessage(MSG_GB28181_SEND_ALARM,&info,sizeof(info));
    return S_OK;
}

