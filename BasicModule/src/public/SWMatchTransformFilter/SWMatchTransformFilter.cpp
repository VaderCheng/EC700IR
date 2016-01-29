#include "swpa.h"
#include "SWFC.h"
#include "SWMessage.h"
#include "SWCarLeft.h"
#include "SWMatchTransformFilter.h"
#include "SWCSRIKRadar.h"
#include "SWJVDCoilDevice.h"
#include "SWKNDRGDevice.h"

#include "SWRAPIERRadar.h"
#include "SWJZRadar.h"
#include "SWCOMTestDevice.h"

#define IMAGE_COUNT   5
#define SIGNAL_COUNT  5
#define CARLEFT_COUNT 5
#define OUTPUT_COUNT 5

	CSWMatchTransformFilter::CSWMatchTransformFilter()
	:CSWBaseFilter(2, 1)
	,CSWMessage(MSG_COM_TEST_BEGIN, MSG_COM_TEST_END)
	,m_pTrackerCfg(NULL)
	 ,m_fInitialize(FALSE)
	 ,m_fRadarTrigger(TRUE)
{

	GetIn(0)->AddObject(CLASSID(CSWImage));
	GetIn(1)->AddObject(CLASSID(CSWCarLeft));
	GetOut(0)->AddObject(CLASSID(CSWCarLeft));
	m_semMatch.Create(0, 1);
	m_semOutput.Create(0, 1);
	for(int i = 0; i < sizeof(m_pDevice)/sizeof(m_pDevice[0]); i++)
	{
		m_pDevice[i] = NULL;
	}

	m_pComTestDevic = NULL;
	m_iTESTINGCOMID = -1;
	//    m_iOutputThreadRestartTime = 0;
}

CSWMatchTransformFilter::~CSWMatchTransformFilter(){

	while(!m_cCarLeftList.IsEmpty() )
	{
		CARLEFT_MATCH_INFO* pCarLeftInfo = m_cCarLeftList.RemoveHead();
		if( pCarLeftInfo != NULL )
		{
			SAFE_RELEASE(pCarLeftInfo->pCarLeft);
			SAFE_DELETE(pCarLeftInfo);
		}
	}

	while(!m_cOutputList.IsEmpty())
	{
		CSWCarLeft* pCarLeft = m_cOutputList.RemoveHead();
		SAFE_RELEASE(pCarLeft);
	}

	for(int i = 0; i < sizeof(m_pDevice)/sizeof(m_pDevice[0]); i++)
	{
		SAFE_RELEASE(m_pDevice[i]);
	}

	SAFE_RELEASE(m_pComTestDevic);
}

HRESULT CSWMatchTransformFilter::Initialize(PVOID pvParam1, PVOID pvParam2,BOOL fRadarTrigger){

	Close();
	CSWAutoLock aLock(&m_cMutex);
	swpa_memcpy(&m_cParam, pvParam1, sizeof(MATCHPARAM));
	swpa_memcpy(&m_cDevParam, pvParam2, 2*sizeof(DEVICEPARAM));
	m_fRadarTrigger=fRadarTrigger;
	for(int i = 0; i < sizeof(m_pDevice)/sizeof(m_pDevice[0]); i++)
	{
		SAFE_RELEASE(m_pDevice[i]);
		switch(m_cDevParam[i].iDeviceType)
		{
			case 1: //�����״�
				if(!m_fRadarTrigger)
					m_cDevParam[i].iRoadNo=0xFF;
				m_pDevice[i] = new CSWCSRIKRadar(&m_cDevParam[i],m_fRadarTrigger);
				break;
			case 2: //�ս�������
				m_pDevice[i] = new CSWJVDCoilDevice(&m_cDevParam[i]);
				break;
			case 3 : //��Ӻ��̵��豸
				m_pDevice[i] = new CSWKNDRGDevice(&m_cDevParam[i]);
				break;
			case 4 : //����ά���״�
				m_pDevice[i] = new CSWRAPIERRadar(&m_cDevParam[i]);
				break;
			case 5 : //�Ĵ������״�
				m_pDevice[i] = new CSWJZRadar(&m_cDevParam[i]);
				break;
		}

		if(m_pDevice[i])
		{
			if(FAILED(m_pDevice[i]->Initialize(OnEvent, this, i)))
			{
				SAFE_RELEASE(m_pDevice[i]);
			}

			// �ڴ�����
			m_pDevice[i]->Start();
		}
	}
	m_fInitialize = TRUE;
	return S_OK;
}

VOID CSWMatchTransformFilter::Close(VOID)
{
	for(int i = 0; i < sizeof(m_pDevice)/sizeof(m_pDevice[0]); i++)
	{
		if(m_pDevice[i])
		{
			m_pDevice[i]->Stop();
			m_pDevice[i]->Close();	//�رմ���
		}
		SAFE_RELEASE(m_pDevice[i]);
	}

	m_cMutex.Lock();
	//���ڳ�ʱ�źŵĴ���
	for(int i = 0; i < sizeof(m_cParam.signal)/sizeof(SIGNAL_PARAM); i++)
	{
		for(int j = 0; j < sizeof(m_cParam.signal[i].signal)/sizeof(SIGNAL); j++)
		{
			SAFE_RELEASE(m_cParam.signal[i].signal[j].pImage);
		}	
	}
	swpa_memset(&m_cParam, 0, sizeof(m_cParam));
	m_fInitialize = FALSE;
	m_cMutex.Unlock();
}

HRESULT CSWMatchTransformFilter::Run(){

	HRESULT hr = CSWBaseFilter::Run();
	if(SUCCEEDED(hr))
	{
		m_matchThread.Start(OnMatchSignal, this);
		m_cOutputThread.Start(OnOutputProxy, this);
		/*for(int i = 0; i < sizeof(m_pDevice)/sizeof(m_pDevice[0]); i++)
		{
			if(m_pDevice[i])
			{
				m_pDevice[i]->Start();
			}
		}*/
	}
	return hr;
}

HRESULT CSWMatchTransformFilter::Stop(){

	CSWBaseFilter::Stop();

	m_matchThread.Stop();
	m_cOutputThread.Stop();
	for(int i = 0; i < sizeof(m_pDevice)/sizeof(m_pDevice[0]); i++)
	{
		if(m_pDevice[i])
		{
			m_pDevice[i]->Stop();
			m_pDevice[i]->Close();	//�رմ���
		}
	}
	if (NULL != m_pComTestDevic)
	{
		m_pComTestDevic->Stop();
		m_pComTestDevic->Close();	//�رմ���
	}
	m_iTESTINGCOMID = -1;
	return S_OK;
}

HRESULT CSWMatchTransformFilter::Receive(CSWObject* obj){

	if(m_fInitialize)
	{
		//ͼƬ
		if(IsDecendant(CSWImage, obj) && m_cParam.fEnable)
		{			
			CSWImage* pImage = (CSWImage *)obj;	
			if(pImage->IsCaptureImage())
			{
				static DWORD dwLastCapImageTime = 0;
				DWORD dwCapTimeDiff = pImage->GetRefTime() - dwLastCapImageTime;
				SW_TRACE_DEBUG("receive a capture image: refTime=%d, trigger id=%d, lastCapTime: %d, diff=%d timelen=%d\n",
						pImage->GetRefTime(), pImage->GetFlag(), dwLastCapImageTime, dwCapTimeDiff,CSWDateTime::GetSystemTick());
				dwLastCapImageTime = pImage->GetRefTime();

				if(!m_fRadarTrigger)
				{
					CARLEFT_MATCH_INFO* pCarLeftInfo = new CARLEFT_MATCH_INFO;
					CSWCarLeft* pCarLeft = NULL;
					if(S_OK != SendCommand(1, (WPARAM)&pCarLeft, 0) || !pCarLeft)
					{
						SW_TRACE_DEBUG("pCarLeft == NULL or is not CSWCarLeft object, used default CSWCarLeft\n");
						pCarLeft = new CSWCarLeft();
						pCarLeft->SetParameter(m_pTrackerCfg, NULL);
					}			
					pCarLeftInfo->pCarLeft = pCarLeft;
					pCarLeftInfo->pCarLeft->SetRoadNo(0xFF);
					pCarLeftInfo->dwInputTime = CSWDateTime::GetSystemTick();
					pCarLeftInfo->pCarLeft->SetImage(0, pImage);

					m_cCarLeftMutex.Lock();
					BOOL bFull=FALSE;
					if(!(bFull=m_cCarLeftList.IsFull()))
					{  
						CARLEFT_MATCH_INFO* pCarLeftMatchInfo = new CARLEFT_MATCH_INFO;
						pCarLeftMatchInfo->pCarLeft = pCarLeft;
						SW_TRACE_DEBUG("m_cCarLeftList add");
						m_cCarLeftList.AddTail(pCarLeftMatchInfo);
					}
					else
					{
						SW_TRACE_DEBUG("m_cCarLeftList is full:%d", m_cCarLeftList.GetCount());
					}
					m_cCarLeftMutex.Unlock();
					
					if( m_pTrackerCfg == NULL )
					{
						m_pTrackerCfg = pCarLeft->GetTrackerParam();
					}
					if(!bFull)	
						m_semMatch.Post();
					return S_OK;
				}
			}	
			//�ź�ƥ���ϣ���֪ͨƥ��
			if(true == AppendImage(pImage))
			{
				SW_TRACE_DEBUG("image %d match a signal, post to match thread", pImage->GetRefTime());
				m_semMatch.Post();
			}
		}
		//ʶ����
		else if(IsDecendant(CSWCarLeft, obj))
		{
			CSWCarLeft *pCarLeft = (CSWCarLeft *)obj;

			BOOL bFull=FALSE;
			m_cCarLeftMutex.Lock();
			if(!(bFull=m_cCarLeftList.IsFull()))
			{
				SW_TRACE_DEBUG("receive a CarLeft: CarArriveTime=%d, trigger id=%d, PlateNo=%s\n",
						pCarLeft->GetCarArriveTime(), pCarLeft->GetTriggerIndex(), (LPCSTR)pCarLeft->GetPlateNo());
				pCarLeft->AddRef();
				CARLEFT_MATCH_INFO* pCarLeftMatchInfo = new CARLEFT_MATCH_INFO;
				pCarLeftMatchInfo->pCarLeft = pCarLeft;
				m_cCarLeftList.AddTail(pCarLeftMatchInfo);
			}
			else
			{
				SW_TRACE_DEBUG("m_cCarLeftList is full:%d", m_cCarLeftList.GetCount());
			}		
			m_cCarLeftMutex.Unlock();

			SW_TRACE_DEBUG("m_cCarLeftList size:%d", m_cCarLeftList.GetCount());
			if( m_pTrackerCfg == NULL )
			{
				m_pTrackerCfg = pCarLeft->GetTrackerParam();
			}
			if(!bFull)	
				m_semMatch.Post();
		}
	}
	return S_OK;
}

bool CSWMatchTransformFilter::SignalIncludeImage(SIGNAL_PARAM* pSignal, bool fCapture)
{
	bool fHasImage = false;
	if(NULL != pSignal)
	{
		if(pSignal->dwType == CSWBaseDevice::IMAGE && TRUE == fCapture)
		{
			SW_TRACE_DEBUG("image match IMAGE signal type");
			fHasImage = true;
		}
		else if(pSignal->dwType == CSWBaseDevice::TRIGGER && TRUE == pSignal->fTrigger && TRUE == fCapture)
		{
			SW_TRACE_DEBUG("image match TRIGGER capture signal type");
			fHasImage = true;
		}
		else if(pSignal->dwType == CSWBaseDevice::TRIGGER && FALSE == pSignal->fTrigger && FALSE == fCapture)
		{
			SW_TRACE_DEBUG("image match TRIGGER normal signal type");
			fHasImage = true;
		}
		else if(pSignal->dwCondition == 2 && FALSE == fCapture)
		{
			SW_TRACE_DEBUG("image match dwCondition == 2 normal signal type");
			fHasImage = true;
		}
	}
	return fHasImage;
}

bool CSWMatchTransformFilter::AppendImage(CSWImage *pImage)
{
	CSWAutoLock aLock(&m_cMutex);
	bool fFound = false;
	bool fCapture = pImage->IsCaptureImage();
	//���������еĿռ䱣��ͼƬ
	for(int i = 0; i < sizeof(m_cParam.signal)/sizeof(SIGNAL_PARAM); i++)
	{	
		SIGNAL_PARAM* pSignal = &m_cParam.signal[i];
		if( pSignal->dwType == CSWBaseDevice::IMAGE && TRUE == fCapture )
		{
			for(int j = 0; j < sizeof(pSignal->signal)/sizeof(SIGNAL); j++)
			{	//ͼƬָ��Ϊ0
				if(NULL == pSignal->signal[j].pImage)
				{
					pSignal->signal[j].dwTime = pImage->GetRefTime();
					SW_TRACE_DEBUG("<Match>append image signal[%d],signal[%d] time:%d,RoadID:%d.\n", i,j, pSignal->signal[j].dwTime,pSignal->dwRoadID);
					pSignal->signal[j].pImage = pImage;
					pSignal->signal[j].fIsMatch = FALSE;
					pImage->AddRef();
					fFound = true;
					break;
				}
			}
		}
		else if( (pSignal->dwType == CSWBaseDevice::TRIGGER && TRUE == pSignal->fTrigger && TRUE == fCapture)
				|| (pSignal->dwType == CSWBaseDevice::TRIGGER && FALSE == pSignal->fTrigger && FALSE == fCapture)
				|| (pSignal->dwCondition == 2 && FALSE == fCapture)
		       )
		{
			for(int j = 0; j < sizeof(pSignal->signal)/sizeof(SIGNAL); j++)
			{
				if(NULL == pSignal->signal[j].pImage && pSignal->signal[j].dwTime > 0)
				{
					SW_TRACE_DEBUG("<Match>append image to signal[%d],signal[%d][type:%d] time:%d.\n",i, j, pSignal->dwType, pSignal->signal[j].dwTime);
					pSignal->signal[j].pImage = pImage;
					pImage->AddRef();
					fFound = true;
					break;
				}
			}
		}

	}
	return fFound;
}

bool CSWMatchTransformFilter::AppendSpeed(DWORD dwRoadID, DWORD dwTime, DWORD dwSpeed, DWORD dwSpeedType, DWORD dwDirection)
{
	CSWAutoLock aLock(&m_cMutex);
	bool fFound = false;
	for(int i = 0; false == fFound && i < sizeof(m_cParam.signal)/sizeof(SIGNAL_PARAM); i++)
	{	
		//��װ�����ֳ���
		if(!m_fRadarTrigger)
		{
			if(m_cParam.signal[i].dwType == CSWBaseDevice::SPEED)
			{
				m_cParam.signal[i].dwRoadID = 0xFF;
			}
		}

		if(m_cParam.signal[i].dwType == CSWBaseDevice::SPEED && (m_cParam.signal[i].dwRoadID == 0xFF || m_cParam.signal[i].dwRoadID == dwRoadID))
		{
			for(int j = 0; false == fFound && j < sizeof(m_cParam.signal[i].signal)/sizeof(SIGNAL); j++)
			{
				if(0 == m_cParam.signal[i].signal[j].dwTime)
				{
					m_cParam.signal[i].signal[j].dwTime = dwTime;
					m_cParam.signal[i].signal[j].dwSpeed = dwSpeed;
					m_cParam.signal[i].signal[j].dwSpeedType = dwSpeedType;
					m_cParam.signal[i].signal[j].dwDirection = dwDirection;
					m_cParam.signal[i].signal[j].fIsMatch = FALSE;
					m_cParam.signal[i].signal[j].pImage = NULL;
					SW_TRACE_DEBUG("<Match>append speed signal[%d] time:%d.\n", j, m_cParam.signal[i].signal[j].dwTime);
					fFound = true;
					break;
				}
			}
		}
	}
	return fFound;
}

void* CSWMatchTransformFilter::OnOutputProxy(void* pvParam)
{
	if( pvParam != NULL )
	{
		CSWMatchTransformFilter* pFilter = (CSWMatchTransformFilter*)pvParam;
		pFilter->OnOutput();
	}
}

HRESULT CSWMatchTransformFilter::OnOutput()
{
	SW_TRACE_DEBUG("<MatchTransformFilter> output run...\n");
	INT iCount = 0, iTotalCount = 0;
	while( S_OK == m_cOutputThread.IsValid() && FILTER_RUNNING == GetState() )
	{
		m_semOutput.Pend(500);
		//swpa_thread_sleep_ms(500);

		//����
		m_cOutputMutex.Lock();
		CSWCarLeft* pCarLeft = NULL;
		if( !m_cOutputList.IsEmpty() )
		{
			iTotalCount = m_cOutputList.GetCount();
			pCarLeft = m_cOutputList.RemoveHead();
			SW_TRACE_DEBUG("OnOutput iTotalCount = %d.\n", iTotalCount);
		}
		if( pCarLeft == NULL )
		{
			m_cOutputMutex.Unlock();
			continue;
		}
		m_cOutputMutex.Unlock();

		//SW_TRACE_DEBUG("OnOutput <1111>\n");
		++iCount;
		SW_TRACE_DEBUG("<MatchTransformFilter>[%d] out put find %s. %d.\n", iTotalCount, (const CHAR*)pCarLeft->GetPlateNo(), iCount);

		for(int i = 0; i < pCarLeft->GetImageCount() - 2; i++)
		{
			// TODO: ֻ��ץ��ͼʶ��Ŀǰ���ܿ�ƥ�䵽 ��������ͼ�С�
			if (0 != i)
			{
				break;
			}

			if( pCarLeft->GetImage(i) == NULL)
			{
				continue;
			}
			// todo:�г�����ɫ��ץ��ͼ��Ҫ����ʶ����������ܻ���һ��СӰ�졣
			BOOL fIsNoPlateRecong = m_cParam.fCaptureRecong; //(-1 != pCarLeft->GetPlateNo().Find("�޳���") && m_cParam.fCaptureRecong);
			BOOL fIsNeedRecogColor = (pCarLeft->GetTrackerParam() != NULL && pCarLeft->GetTrackerParam()->fEnableRecgCarColor);			
			if( pCarLeft->GetImage(i)->IsCaptureImage() && (fIsNeedRecogColor || fIsNoPlateRecong) && pCarLeft->GetImage(i)->GetType() != SW_IMAGE_JPEG )
			{
				CSWCarLeftArray carLeft;
				DWORD dwBegin = CSWDateTime::GetSystemTick();
				//HRESULT hr = CSWMessage::SendMessage(MSG_RECOGNIZE_PHOTO, (WPARAM)pCarLeft, (LPARAM)&carLeft);
				SW_TRACE_DEBUG("<MatchTransformFilter>recognize photo %d ms.\n", CSWDateTime::GetSystemTick() - dwBegin);
				HRESULT hr=S_OK;
				if( S_OK == hr)
				{
					int index = -1;
					for(int k = 0; k < carLeft.GetCount(); k++)
					{
						// ��������Ҫ���ݳ���ƥ����
						if((carLeft.GetCount() <= 1) ||
								(carLeft[k]->GetRoadNo() == pCarLeft->GetRoadNo() + 1 || carLeft[k]->GetRoadNo() == 0xFF || pCarLeft->GetRoadNo() == 0xFF))
						{
							index = k;
							break;
						}
						else
						{
							SW_TRACE_DEBUG("<MatchTransformFilter>recognize index:%d. roadno:%d. match roadno:%d.\n", k, carLeft[k]->GetRoadNo(), pCarLeft->GetRoadNo()+1);
						}
					}

					//                    SW_RECT rcLast;
					//                    pCarLeft->GetResultRect(1, &rcLast);
					//                    SW_TRACE_DEBUG("Output CarLeft : PlateNo : %s, LastRect[%d, %d, %d, %d] \n",
					//                                   (LPCSTR)pCarLeft->GetPlateNo(), rcLast.left, rcLast.top, rcLast.right, rcLast.bottom);
					if( index == -1 )
					{
						SW_TRACE_DEBUG("<MatchTransformFilter>recognize count:%d.none match.\n", carLeft.GetCount());
					}
					else if ((swpa_strstr(carLeft[index]->GetPlateNo(), "��") == NULL) || (swpa_strstr(pCarLeft->GetPlateNo(), "��")  != NULL) ) // ץ��ʶ���н��������Ƶ��ʶ�����޳��ƾ��滻��ͼ
					{	
						//�滻���ƺ���
						if( fIsNoPlateRecong )
						{
							//                            SW_TRACE_DEBUG("<MatchTransformFilter>replace Capture Result : %s.\n", (LPCSTR)carLeft[index]->GetPlateNo());
							pCarLeft->m_fCaptureRecogResult = TRUE;
							pCarLeft->SetPlateNo(carLeft[index]->GetPlateNo());
							pCarLeft->SetFirstAverageConfidence(carLeft[index]->GetAverageConfidence());
							pCarLeft->SetFirstAverageConfidence(carLeft[index]->GetFirstAverageConfidence());
							pCarLeft->SetObservedFrames(carLeft[index]->GetObservedFrames());
							pCarLeft->SetPlateType(carLeft[index]->GetPlateType());
							pCarLeft->SetPlateColor(carLeft[index]->GetPlateColor());

							// ���ó���
							if(pCarLeft->GetCarType() == CT_WALKMAN
									|| pCarLeft->GetCarType() == CT_BIKE )
							{
								pCarLeft->SetCarType(CT_SMALL);
							}

							// ��ʶ��ĳ������������ BestPlatePos����������������� LastPlatePos
							SW_RECT rcBest;
							carLeft[index]->GetResultRect(0, &rcBest);
							pCarLeft->SetResultRect(1, &rcBest);
							//                            SW_TRACE_DEBUG("CaptureImage Recognize result : %s, BestRect[%d, %d, %d, %d]",
							//                                           (LPCSTR)carLeft[index]->GetPlateNo(), rcBest.left, rcBest.top, rcBest.right, rcBest.bottom);

							for(int k = 0; k < carLeft[index]->GetImageCount(); k++)
							{
								SW_RECT rc;
								CSWImage *pImage = carLeft[index]->GetImage(k, &rc);
								if(pImage)
								{
									pCarLeft->SetImage(k, pImage, &rc);
								}
							}
						}
						//�滻������ɫ
						if( fIsNeedRecogColor)
						{
							switch(carLeft[index]->GetCarColor())
							{
								case CC_WHITE:
									SW_TRACE_DEBUG("%s:CC_WHITE, IsNight=%d", (LPCSTR)carLeft[index]->GetPlateNo(), carLeft[index]->GetIsNight());
									break;
								case CC_GREY:
									SW_TRACE_DEBUG("%s:CC_GREY, IsNight=%d", (LPCSTR)carLeft[index]->GetPlateNo(), carLeft[index]->GetIsNight());
									break;
								case CC_BLACK:
									SW_TRACE_DEBUG("%s:CC_BLACK, IsNight=%d", (LPCSTR)carLeft[index]->GetPlateNo(), carLeft[index]->GetIsNight());
									break;
								case CC_RED:
									SW_TRACE_DEBUG("%s:CC_RED, IsNight=%d", (LPCSTR)carLeft[index]->GetPlateNo(), carLeft[index]->GetIsNight());
									break;
								case CC_YELLOW:
									SW_TRACE_DEBUG("%s:CC_YELLOW, IsNight=%d", (LPCSTR)carLeft[index]->GetPlateNo(), carLeft[index]->GetIsNight());
									break;
								case CC_GREEN:
									SW_TRACE_DEBUG("%s:CC_GREEN, IsNight=%d", (LPCSTR)carLeft[index]->GetPlateNo(), carLeft[index]->GetIsNight());
									break;
								case CC_BLUE:
									SW_TRACE_DEBUG("%s:CC_BLUE, IsNight=%d", (LPCSTR)carLeft[index]->GetPlateNo(), carLeft[index]->GetIsNight());
									break;
								case CC_PURPLE:
									SW_TRACE_DEBUG("%s:CC_PURPLE, IsNight=%d", (LPCSTR)carLeft[index]->GetPlateNo(), carLeft[index]->GetIsNight());
									break;
								case CC_PINK:
									SW_TRACE_DEBUG("%s:CC_PINK, IsNight=%d", (LPCSTR)carLeft[index]->GetPlateNo(), carLeft[index]->GetIsNight());
									break;
								case CC_BROWN:
									SW_TRACE_DEBUG("%s:CC_BROWN, IsNight=%d", (LPCSTR)carLeft[index]->GetPlateNo(), carLeft[index]->GetIsNight());
									break;
								default:
									SW_TRACE_DEBUG("%s:CC_UNKNOWN, IsNight=%d", (LPCSTR)carLeft[index]->GetPlateNo(), carLeft[index]->GetIsNight());
									break;
							}
							if(carLeft[index]->GetCarColor() != CC_UNKNOWN)
							{
								SW_TRACE_DEBUG("<MatchTransformFilter>replace Color.%s=>%s. %d=>%d.\n", (const CHAR*)pCarLeft->GetPlateNo(), (const CHAR*)carLeft[index]->GetPlateNo(), pCarLeft->GetCarColor(), carLeft[index]->GetCarColor());
								pCarLeft->SetCarColor((CAR_COLOR)carLeft[index]->GetCarColor());
							}
						}
						//��������
						pCarLeft->SetFaceRect(carLeft[index]);
					}
					// ��Ƶ���޳���  �� ץ��ʶ�����Ƴ� ���滻ץ��ͼ
					if( (swpa_strstr(pCarLeft->GetPlateNo(), "��")  != NULL) ||
							(index != -1 && swpa_strstr(carLeft[index]->GetPlateNo(), "��") == NULL) )
					{
						// �滻ץ��ͼ������ͼ
						CSWImage* pImage = pCarLeft->GetImage(0);
						if (NULL != pImage)
						{
							pCarLeft->SetImage(1, pImage);
							pCarLeft->SetImage(0, NULL);
						}
					}
				}
			}
		}
		// ȥ����������ͼ�������������λ��������Ҫ��ʱ���ڴ�
		if ( 0 )
		{
			pCarLeft->SetImage(0, NULL);
		}

		GetOut(0)->Deliver(pCarLeft);
		pCarLeft->Release();
		pCarLeft = NULL;
		//        SW_TRACE_DEBUG("OnOutput <4444>\n");
	}
	SW_TRACE_DEBUG("<MatchTransformFilter> output exit...\n");
	return S_OK;
}

HRESULT CSWMatchTransformFilter::OnStartCOMTest(WPARAM wParam, LPARAM lParam)
{
	INT iComId = (INT)wParam;
	if (iComId < 0 || iComId > 1)
	{
		SW_TRACE_NORMAL("Err: OnStartCOMTest COM ID %d err\n",iComId);
		return E_INVALIDARG;
	}

	if (NULL != m_pComTestDevic)	//ֻ�ܿ�ʼһ��
	{
		SW_TRACE_NORMAL("Err: Must stop last COM %d test server first\n",m_iTESTINGCOMID);
		return E_FAIL;
	}

	if (-1 != m_iTESTINGCOMID)
	{
		SW_TRACE_NORMAL("Err: Last Test COM %d already on server!\n",m_iTESTINGCOMID);
		return E_INVALIDARG;
	}

	//�ر������������
	if(m_pDevice[iComId])
	{
		m_pDevice[iComId]->Stop();
		m_pDevice[iComId]->Close();
	}

	DEVICEPARAM tParam;
	tParam.iBaudrate = 9600;
	tParam.iCommType = 1;		//485 ������ֻ��485���ڣ�ˮ�Ǽ���485����232

	m_pComTestDevic = new CSWCOMTestDevice(&tParam);
	if (NULL == m_pComTestDevic)
	{
		SW_TRACE_NORMAL("Create new Com test device failed!\n");
		return E_FAIL;
	}

	if (FAILED(m_pComTestDevic->Initialize(NULL,NULL,iComId)))
	{
		SW_TRACE_NORMAL("init Com test device <id:%d> failed!\n",iComId);
		SAFE_RELEASE(m_pComTestDevic);
		return E_FAIL;
	}
	else
	{
		m_pComTestDevic->Start();
	}

	m_iTESTINGCOMID = iComId;
	SW_TRACE_NORMAL("<OnStartCOMTest> Start test COM %d server!\n",iComId);

	return S_OK;
}

HRESULT CSWMatchTransformFilter::OnStopCOMTest(WPARAM wParam, LPARAM lParam)
{
	INT iComId = (INT)wParam;
	if (iComId < 0 || iComId > 1)
	{
		SW_TRACE_NORMAL("Err: OnStopCOMTest COM ID %d err\n",iComId);
		return E_INVALIDARG;
	}

	if (m_iTESTINGCOMID != iComId)
	{
		SW_TRACE_NORMAL("Err: COM ID Err last %d cur %d\n",m_iTESTINGCOMID,iComId);
		return E_FAIL;
	}

	if (m_pComTestDevic)
	{
		m_pComTestDevic->Stop();
		m_pComTestDevic->Close();
		SAFE_RELEASE(m_pComTestDevic);
	}

	//�ָ��������������
	if(m_pDevice[iComId])
	{
		if(S_OK == m_pDevice[iComId]->Initialize(OnEvent, this, iComId))
		{
			m_pDevice[iComId]->Start();
		}
		else
		{
			SW_TRACE_NORMAL("init device <id:%d> failed!\n",iComId);
			SAFE_RELEASE(m_pDevice[iComId]);
		}
	}

	m_iTESTINGCOMID = -1;

	SW_TRACE_NORMAL("<OnStopCOMTest> Stop test COM %d server!\n",iComId);

	return S_OK;
}

VOID CSWMatchTransformFilter::OnEvent(PVOID pvParam, CSWBaseDevice *pDevice, CSWBaseDevice::DEVICE_TYPE type, DWORD dwTime, PDWORD pdwValue)
{
	CSWMatchTransformFilter *pThis = (CSWMatchTransformFilter *)pvParam;
	if(type == CSWBaseDevice::SPEED)
	{
		SW_TRACE_DEBUG("receive speed event! time: %d[speed:%d, roadid:%d].", dwTime, pdwValue[0], pdwValue[1]);
		if(pThis->AppendSpeed(pdwValue[1], dwTime, pdwValue[0], IsDecendant(CSWJVDCoilDevice, pDevice) ? E_SIG_COIL_SPEED : E_SIG_RADAR_SPEED,pdwValue[2]))
		{
			SW_TRACE_DEBUG("speed %d[%d, %d, %d] match a signal, post to match thread", dwTime, pdwValue[0], pdwValue[1], pdwValue[2]);
			pThis->m_semMatch.Post();
		}
	}
}

void* CSWMatchTransformFilter::OnMatchSignal(void* pvParam)
{
	CSWMatchTransformFilter* pThis = (CSWMatchTransformFilter *)pvParam;
	CARLEFT_MATCH_INFO* pCarLeftMatchInfo = NULL;

	SW_TRACE_DEBUG("<MatchTransformFilter> Match run...\n");
	while(S_OK == pThis->m_matchThread.IsValid() && FILTER_RUNNING == pThis->GetState() && pThis->m_fInitialize)
	{
		pThis->m_semMatch.Pend(400);
		// ��ʱ�������ܻ���� output �߳̿���������������豸
		{
			if (pThis->m_cOutputList.GetCount() >= 12)
			{
				//                pThis->m_cOutputThread.Stop();
				//                swpa_thread_sleep_ms(500);
				//                pThis->m_iOutputThreadRestartTime ++;
				//                pThis->m_cOutputThread.Start(OnOutputProxy, pThis);
				//                SW_TRACE_DEBUG("<MatchTransformFilter> Match <OutputListCount = %d, m_cOutputThread is dead, restart...restarttimes = %d>\n",
				//                               pThis->m_cOutputList.GetCount(), pThis->m_iOutputThreadRestartTime);

				//                if (pThis->m_iOutputThreadRestartTime >= 3)
				{
					SW_TRACE_DEBUG("<MatchTransformFilter> Match <m_cOutputThread is dead, RESETDEVICE>\n");
					CSWMessage::SendMessage(MSG_APP_RESETDEVICE, 0, 0);
				}
			}
		}

		//���ܿز�ʹ�ܵ������
		//if(!pThis->m_cParam.fEnable)
		if(0)
		{
			pThis->m_cCarLeftMutex.Lock();
			while( !pThis->m_cCarLeftList.IsEmpty() )
			{
				pCarLeftMatchInfo = pThis->m_cCarLeftList.RemoveHead();
				if( pCarLeftMatchInfo != NULL )
				{
					CSWCarLeft *pCarLeft = pCarLeftMatchInfo->pCarLeft;
					if( pCarLeft != NULL )
					{
						pThis->m_cOutputMutex.Lock();
						if(!pThis->m_cOutputList.IsFull())
						{
							pCarLeft->AddRef();
							pThis->m_cOutputList.AddTail(pCarLeft);
							SW_TRACE_DEBUG("pThis->m_cOutputList.GetCount():%d", pThis->m_cOutputList.GetCount());
							pThis->m_semOutput.Post();
						}
						else
						{
							SW_TRACE_DEBUG("m_cOutputList is full, egnore the result, total size:%d", pThis->m_cOutputList.GetCount());
						}
						pThis->m_cOutputMutex.Unlock();
					}
					SAFE_RELEASE(pCarLeftMatchInfo->pCarLeft);
					delete pCarLeftMatchInfo;
					pCarLeftMatchInfo = NULL;
				}
			}
			pThis->m_cCarLeftMutex.Unlock();
		}
		//ʹ�����ܿأ������ź�ƥ��
		else
		{
			pThis->m_cMutex.Lock();
			SW_TRACE_DEBUG("OutCtrl run\n");
			//���ڳ�ʱ�źŵĴ���
			for(int i = 0; i < sizeof(pThis->m_cParam.signal)/sizeof(SIGNAL_PARAM); i++)
			{
				if(CSWBaseDevice::NONE < pThis->m_cParam.signal[i].dwType && pThis->m_cParam.signal[i].dwType < CSWBaseDevice::MAX_COUNT)
				{
					DWORD dwRefTime=CSWDateTime::GetSystemTick();
					for(int j = 0; j < sizeof(pThis->m_cParam.signal[i].signal)/sizeof(SIGNAL); j++)
					{		
						if((pThis->m_cParam.signal[i].signal[j].dwTime > 0) &&
								(dwRefTime - pThis->m_cParam.signal[i].signal[j].dwTime > pThis->m_cParam.dwSignalKeepTime) &&
								(dwRefTime > pThis->m_cParam.signal[i].signal[j].dwTime))
						{
							switch(pThis->m_cParam.signal[i].dwCondition)
							{
								case 0 : //���źſ��п��ޣ�ֱ�Ӹɵ�
									if(pThis->m_cParam.signal[i].signal[j].pImage!=NULL)
									{
										SW_TRACE_DEBUG("signal[%d][%d][%d - %d > %d] time out, ignore  the signal TriggerIndex:%d",
												j, pThis->m_cParam.signal[i].dwType, dwRefTime,
												pThis->m_cParam.signal[i].signal[j].dwTime, pThis->m_cParam.dwSignalKeepTime,
												pThis->m_cParam.signal[i].signal[j].pImage->GetFlag());
									}
									pThis->m_cParam.signal[i].signal[j].dwTime = 0;
									SAFE_RELEASE(pThis->m_cParam.signal[i].signal[j].pImage);
									break;

								case 1 : //���źű���ȴ����Ʋ��ܳ����,��ʱֱ�Ӹɵ�
									SW_TRACE_DEBUG("signal[%d][%d - %d > %d] time out, ignore  the signal",
											pThis->m_cParam.signal[i].dwType, dwRefTime,
											pThis->m_cParam.signal[i].signal[j].dwTime, pThis->m_cParam.dwSignalKeepTime);
									pThis->m_cParam.signal[i].signal[j].dwTime = 0;
									SAFE_RELEASE(pThis->m_cParam.signal[i].signal[j].pImage);
									break;

								case 2 : //ǿ�Ƴ���
									// ����Ѿ�ƥ����Ͳ������
									{
										if( !pThis->m_cParam.signal[i].signal[j].fIsMatch )
										{
											pThis->m_cCarLeftMutex.Lock();
											if(!pThis->m_cCarLeftList.IsFull())
											{
												SW_TRACE_DEBUG("signal[%d][%d - %d > %d] time out, fire to carleft",
														pThis->m_cParam.signal[i].dwType, dwRefTime,
														pThis->m_cParam.signal[i].signal[j].dwTime, pThis->m_cParam.dwSignalKeepTime);
												CARLEFT_MATCH_INFO* pCarLeftInfo = new CARLEFT_MATCH_INFO;
												CSWCarLeft* pCarLeft = NULL;
												if(S_OK != pThis->SendCommand(1, (WPARAM)&pCarLeft, 0) || !pCarLeft)
												{
													SW_TRACE_DEBUG("create carleft failed, use default CSWCarLeft.");
													pCarLeft = new CSWCarLeft();
													pCarLeft->SetParameter(pThis->m_pTrackerCfg, NULL);
												}
												SW_TRACE_DEBUG("create carleft name:%s, image:0x%08x", pCarLeft->Name(), pThis->m_cParam.signal[i].signal[j].pImage);
												pCarLeftInfo->pCarLeft = pCarLeft;
												pCarLeftInfo->pCarLeft->SetCarArriveTime(pThis->m_cParam.signal[i].signal[j].dwTime);
												pCarLeftInfo->pCarLeft->SetRoadNo(pThis->m_cParam.signal[i].dwRoadID);
												pCarLeftInfo->dwOutputCondition = 1;
												pCarLeftInfo->dwInputTime = dwRefTime;
												pCarLeftInfo->pCarLeft->SetImage(1, pThis->m_cParam.signal[i].signal[j].pImage);
												pThis->m_cCarLeftList.AddTail(pCarLeftInfo);
												pThis->m_cParam.signal[i].signal[j].fIsMatch = TRUE;
												SW_TRACE_DEBUG("m_cCarLeftList size:%d", pThis->m_cCarLeftList.GetCount());
											}
											else
											{
												SW_TRACE_DEBUG("m_cCarLeftList is full:%d", pThis->m_cCarLeftList.GetCount());
											}
											pThis->m_cCarLeftMutex.Unlock();
										}
										else
										{
											if(pThis->m_cParam.signal[i].signal[j].pImage!=NULL)
											{
												SW_TRACE_DEBUG("signal[%d][%d - %d > %d] time out and it has match, ignore the signal,pImage=0x%08x",
														pThis->m_cParam.signal[i].dwType, dwRefTime,
														pThis->m_cParam.signal[i].signal[j].dwTime, pThis->m_cParam.dwSignalKeepTime,
														pThis->m_cParam.signal[i].signal[j].pImage);
											}
											pThis->m_cParam.signal[i].signal[j].dwTime = 0;
											SAFE_RELEASE(pThis->m_cParam.signal[i].signal[j].pImage);
										}
									}
									break;
							}
						}
					}
				}
			}
			pThis->m_cMutex.Unlock();

			//������������
			pThis->m_cCarLeftMutex.Lock();
			SW_POSITION pos = pThis->m_cCarLeftList.GetHeadPosition();
			SW_POSITION posLast = pos;
			while(pThis->m_cCarLeftList.IsValid(pos))
			{
				posLast = pos;
				pCarLeftMatchInfo = pThis->m_cCarLeftList.GetNext(pos);
				//�����ź�����ƥ�䳵��
				//bool fOk = true;
				bool fOk = false;
				pThis->m_cMutex.Lock();
				for(int i = 0; i < sizeof(pThis->m_cParam.signal)/sizeof(SIGNAL_PARAM); i++)
				{
					if(CSWBaseDevice::NONE < pThis->m_cParam.signal[i].dwType
							&& pThis->m_cParam.signal[i].dwType < CSWBaseDevice::MAX_COUNT
							&& (pCarLeftMatchInfo->pCarLeft->GetRoadNo() == pThis->m_cParam.signal[i].dwRoadID || 0xFF == pThis->m_cParam.signal[i].dwRoadID))
					{
						pFunMatch pMatch = pThis->GetFunction(pThis->m_cParam.signal[i].dwType);
						if(!pMatch)
						{
							SW_TRACE_DEBUG("dwType = %d, not found match function\n", pThis->m_cParam.signal[i].dwType);
							continue;
						}
						if((pThis->*pMatch)(&pThis->m_cParam.signal[i], pCarLeftMatchInfo))
						{
							fOk=true;
						}
						else
						{
							SW_TRACE_DEBUG("match error type:%d,RoadID=%d\n", pThis->m_cParam.signal[i].dwType,pThis->m_cParam.signal[i].dwRoadID);
						}
						/*if(!pMatch)
						{
							fOk = false;
							SW_TRACE_DEBUG("dwType = %d, not found match function", pThis->m_cParam.signal[i].dwType);
						}
						else if(false == (pThis->*pMatch)(&pThis->m_cParam.signal[i], pCarLeftMatchInfo))
						{
							// ��ʹ��һ����ƥ�䲻�ϣ�ҲҪ����ƥ���������͡�
							// ֻ��¼�����Ƿ�����ȫƥ��
							fOk = false;
							//break;
						}
						else
						{
							fOk = true;
						}*/
					}
				}
				pThis->m_cMutex.Unlock();
				//ƥ����ϣ��ŵ�������С�
				if(fOk)
				{
					SW_TRACE_DEBUG("match carleft:%s\n", (LPCSTR)pCarLeftMatchInfo->pCarLeft->GetPlateNo());
					pThis->m_cOutputMutex.Lock();
					if(!pThis->m_cOutputList.IsFull())
					{
						SAFE_ADDREF(pCarLeftMatchInfo->pCarLeft);
						pThis->m_cOutputList.AddTail(pCarLeftMatchInfo->pCarLeft);
						SW_TRACE_DEBUG("m_cOutputList size:%d", pThis->m_cOutputList.GetCount());
						pThis->m_semOutput.Post();
					}
					else
					{
						SW_TRACE_DEBUG("m_cOutputList is full, egnore the result, total size:%d", pThis->m_cOutputList.GetCount());
					}
					pThis->m_cOutputMutex.Unlock();

					SAFE_RELEASE(pCarLeftMatchInfo->pCarLeft);
					delete pCarLeftMatchInfo;
					pThis->m_cCarLeftList.RemoveAt(posLast);
				}
				//�жϳ����Ƿ�ʱ�������ʱ����ֱ��ɾ��
				else if(CSWDateTime::GetSystemTick() - pCarLeftMatchInfo->dwInputTime > pThis->m_cParam.dwPlateKeepTime)
				{
					//�жϸó������ڵĳ����Ŷ�Ӧ���ź��Ƿ��޹ؽ�Ҫ������ǿ�Ƴ��ƣ�������
					fOk = true;
					for(int i = 0; i < sizeof(pThis->m_cParam.signal)/sizeof(SIGNAL_PARAM); i++)
					{
						if(pThis->m_cParam.signal[i].dwRoadID == 0xFF || pThis->m_cParam.signal[i].dwRoadID == pCarLeftMatchInfo->pCarLeft->GetRoadNo())
						{
							if(pThis->m_cParam.signal[i].dwCondition == 1)
							{
								fOk = false;
							}
						}
					}

					SW_TRACE_DEBUG("carleft ID:%d time out, %s the plate:%s, current time:%d inputTime:%d diffTime:%d\n",
							pCarLeftMatchInfo->pCarLeft->GetTriggerIndex(),
							fOk ? "fire output" : "delete",
							(LPCSTR)pCarLeftMatchInfo->pCarLeft->GetPlateNo(),
							CSWDateTime::GetSystemTick(),
							pCarLeftMatchInfo->dwInputTime,
							CSWDateTime::GetSystemTick() - pCarLeftMatchInfo->dwInputTime);
					if(fOk)
					{
						CSWCarLeft *pCarLeft = pCarLeftMatchInfo->pCarLeft;

						pThis->m_cOutputMutex.Lock();
						if(!pThis->m_cOutputList.IsFull())
						{
							pCarLeft->AddRef();
							pThis->m_cOutputList.AddTail(pCarLeft);
							SW_TRACE_DEBUG("m_cOutputList size:%d", pThis->m_cOutputList.GetCount());
							pThis->m_semOutput.Post();
						}
						else
						{
							SW_TRACE_DEBUG("m_cOutputList is full, egnore the result, total size:%d", pThis->m_cOutputList.GetCount());
						}
						pThis->m_cOutputMutex.Unlock();
					}
					pCarLeftMatchInfo->pCarLeft->Release();
					delete pCarLeftMatchInfo;
					pThis->m_cCarLeftList.RemoveAt(posLast);
				}
			}
			pThis->m_cCarLeftMutex.Unlock();
		}
	}

	SW_TRACE_DEBUG("<MatchTransformFilter> Match exit...\n");
	return 0;
}

bool CSWMatchTransformFilter::MatchSpeed(SIGNAL_PARAM *pSignal, CARLEFT_MATCH_INFO *pMatchInfo)
{
	DWORD dwDiffTime = pMatchInfo->dwMatchSpeedTime;
	DWORD dwMatchIndex = 0xFFFFFFFF;
	if(m_fRadarTrigger)	//��װģʽƥ�����
	{
		for(int i = 0; i < sizeof(pSignal->signal)/sizeof(pSignal->signal[0]); i++)
		{	//�źŴ���
			if(pSignal->signal[i].dwTime > 0)
			{	//�źźͳ��ƵĲ�ֵ
				if(pSignal->signal[i].dwTime >= pMatchInfo->dwInputTime
						&& pSignal->signal[i].dwTime - pMatchInfo->dwInputTime <= m_cParam.dwMatchMaxTime)
				{
					dwDiffTime = pSignal->signal[i].dwTime - pMatchInfo->dwInputTime;
					dwMatchIndex = i;
				}
				//���ƺ��źŵĲ�ֵ
				else if(pSignal->signal[i].dwTime < pMatchInfo->dwInputTime 
						&& pMatchInfo->dwInputTime - pSignal->signal[i].dwTime <= m_cParam.dwMatchMinTime
						&& pMatchInfo->dwInputTime - pSignal->signal[i].dwTime < dwDiffTime)
				{
					dwDiffTime = pMatchInfo->dwInputTime - pSignal->signal[i].dwTime;
					dwMatchIndex = i;
				}
			}		
		}

		if(dwMatchIndex != 0xFFFFFFFF)
		{
			SW_TRACE_DEBUG("match speed time:%d, carleft time:%d, interval:%d", pSignal->signal[dwMatchIndex].dwTime, pMatchInfo->pCarLeft->GetCarArriveTime(), (INT)pSignal->signal[dwMatchIndex].dwTime - (INT)pMatchInfo->pCarLeft->GetCarArriveTime());

			pMatchInfo->pCarLeft->SetCarspeed(pSignal->signal[dwMatchIndex].dwSpeed);
			pMatchInfo->pCarLeft->SetSpeedType((E_SIG_RADAR_SPEED == pSignal->signal[dwMatchIndex].dwSpeedType)?"�״����":"��Ȧ����");
			pMatchInfo->dwMatchSpeedTime = dwDiffTime;	


			// ��������źų�ʱ���Ľ����
			if(pMatchInfo->dwOutputCondition == 1)
			{
				SW_TRACE_DEBUG("�״����źų�ʱ");
				pMatchInfo->pCarLeft->SetImage(0, pSignal->signal[dwMatchIndex].pImage);
				pMatchInfo->pCarLeft->SetImage(1, pSignal->signal[dwMatchIndex].pImage);
			}
			// ����ƥ���־
			pSignal->signal[dwMatchIndex].fIsMatch = TRUE;
			//�źų�ʱ��ɾ��
			//if(CSWDateTime::GetSystemTick() - pSignal->signal[dwMatchIndex].dwTime > m_cParam.dwSignalKeepTime)
			{
				SAFE_RELEASE(pSignal->signal[dwMatchIndex].pImage);
				pSignal->signal[dwMatchIndex].dwTime = 0;
				pSignal->signal[dwMatchIndex].dwSpeed = 0;
			}
		}
	}
	else	//��װģʽ����ƥ�䣬���״�ʱ����Ϊ����ʱ��
	{
		for(int i = 0; i < sizeof(pSignal->signal)/sizeof(pSignal->signal[0]); i++)
		{
			if(pSignal->signal[i].dwTime > 0)
			{
				dwMatchIndex = i;
			}
		}

		if(dwMatchIndex != 0xFFFFFFFF)
		{
			SW_TRACE_DEBUG("match speed time:%d, dwMatchIndex:%d, interval:%d speed value:%d", pSignal->signal[dwMatchIndex].dwTime, dwMatchIndex, pSignal->signal[dwMatchIndex].dwSpeed);
			pMatchInfo->pCarLeft->SetCarspeed(pSignal->signal[dwMatchIndex].dwSpeed);
			pMatchInfo->pCarLeft->SetCarArriveTime(pSignal->signal[dwMatchIndex].dwTime);
			pMatchInfo->pCarLeft->SetSpeedType("�״����");
			pMatchInfo->pCarLeft->SetReverseRun(pSignal->signal[dwMatchIndex].dwDirection==2?TRUE:FALSE);

			// ����ƥ���־
			pSignal->signal[dwMatchIndex].fIsMatch = TRUE;
			//�źų�ʱ��ɾ��
			if(CSWDateTime::GetSystemTick() - pSignal->signal[dwMatchIndex].dwTime > m_cParam.dwSignalKeepTime)
			{
				SAFE_RELEASE(pSignal->signal[dwMatchIndex].pImage);
				pSignal->signal[dwMatchIndex].dwTime = 0;
				pSignal->signal[dwMatchIndex].dwSpeed = 0;
			}
		}

	}
	return dwDiffTime != NOT_MATCH_TIME;
}

bool CSWMatchTransformFilter::MatchImage(SIGNAL_PARAM *pSignal, CARLEFT_MATCH_INFO *pMatchInfo)
{	
	SW_TRACE_DEBUG("MatchImage:%d",pMatchInfo->dwInputTime);
	DWORD dwDiffTime = pMatchInfo->dwMatchImageTime[1];
	DWORD dwMatchIndex = 0xFFFFFFFF;

	//���ж�ͼƬ�ı�־λ
	for(int i = 0; i < sizeof(pSignal->signal)/sizeof(pSignal->signal[0]); i++)
	{
		if(pSignal->signal[i].dwTime > 0 && pSignal->signal[i].pImage)
		{
			SW_TRACE_DEBUG("CSWMatchTransformFilter::MatchImage< CarLeft TriggerIndex = %d, CaptureImage trigger id = %d >.\n",
					pMatchInfo->pCarLeft->GetTriggerIndex(), pSignal->signal[i].pImage->GetFlag());
			if(0 < pSignal->signal[i].pImage->GetFlag() && 
					pSignal->signal[i].pImage->GetFlag() == pMatchInfo->pCarLeft->GetTriggerIndex())
			{
				SW_TRACE_DEBUG("match car left by trigger id=%d", pSignal->signal[i].pImage->GetFlag());
				dwMatchIndex = i;
				dwDiffTime = 0;
				break;
			}
		}
	}

	// ������ʹ�� trigger id ��ƥ�䣬�տ�ʼƥ�䲻��Ҳ��Ҫ�ȴ�һ��ʱ��
	if (dwMatchIndex == 0xFFFFFFFF)
	{
		DWORD dwPlatePassTime = CSWDateTime::GetSystemTick() - pMatchInfo->dwInputTime;
		if (dwPlatePassTime < m_cParam.dwPlateKeepTime / 2)
		{
			SW_TRACE_DEBUG("no match by trigger id , continue PlatePassTime = %d, LimitTime = %d.\n", dwPlatePassTime, m_cParam.dwPlateKeepTime / 2);
			return FALSE;
		}
	}


	//���ж��ٽ�ʱ���
	if(dwMatchIndex == 0xFFFFFFFF)
	{
		for(int i = 0; i < sizeof(pSignal->signal)/sizeof(pSignal->signal[0]); i++)
		{
			if(pSignal->signal[i].dwTime > 0)
			{
				if(pSignal->signal[i].dwTime >= pMatchInfo->dwInputTime
						&& pSignal->signal[i].dwTime - pMatchInfo->dwInputTime <= m_cParam.dwMatchMaxTime)
				{
					dwDiffTime = pSignal->signal[i].dwTime - pMatchInfo->dwInputTime;
					dwMatchIndex = i;
				}
				else if(pSignal->signal[i].dwTime < pMatchInfo->dwInputTime 
						&& pMatchInfo->dwInputTime - pSignal->signal[i].dwTime <= m_cParam.dwMatchMinTime)
				{
					dwDiffTime = pMatchInfo->dwInputTime - pSignal->signal[i].dwTime;
					dwMatchIndex = i;
				}
			}
		}
	}

	if(dwMatchIndex != 0xFFFFFFFF)
	{	
		//ѹ��ͼƬ
		//�����ʱ��ѹ��ͼƬ��
		//CSWMessage::SendMessage(MSG_JPEGENCODE, (WPARAM)pSignal->signal[dwMatchIndex].pImage);
		//�滻���
		SW_RECT rc;
		swpa_memset(&rc, 0, sizeof(SW_RECT));
		if(pMatchInfo->pCarLeft->GetOutputLastImage())
			pMatchInfo->pCarLeft->SetImage(0, pSignal->signal[dwMatchIndex].pImage, NULL);
		pMatchInfo->dwMatchImageTime[1] = dwDiffTime;
		SW_TRACE_DEBUG("match image time:%d, carleft time:%d, interval:%d, carleft Trigger:%d", 
				pSignal->signal[dwMatchIndex].dwTime, 
				pMatchInfo->pCarLeft->GetCarArriveTime(), 
				(INT)pSignal->signal[dwMatchIndex].dwTime - (INT)pMatchInfo->pCarLeft->GetCarArriveTime(),
				pMatchInfo->pCarLeft->GetTriggerIndex());
		// ����ƥ���־
		pSignal->signal[dwMatchIndex].fIsMatch = TRUE;
		// ��������źų�ʱ���Ľ����
		if(pMatchInfo->dwOutputCondition == 1)
		{
			SW_TRACE_DEBUG("ץ��ͼ���źų�ʱ");
			pMatchInfo->pCarLeft->SetImage(0, pSignal->signal[dwMatchIndex].pImage, &rc);
			pMatchInfo->pCarLeft->SetImage(1, pSignal->signal[dwMatchIndex].pImage, &rc);
		}
		//�źų�ʱ��ɾ��
		if(CSWDateTime::GetSystemTick() - pSignal->signal[dwMatchIndex].dwTime > m_cParam.dwSignalKeepTime)
		{
			SAFE_RELEASE(pSignal->signal[dwMatchIndex].pImage);
			pSignal->signal[dwMatchIndex].dwTime = 0;
			pSignal->signal[dwMatchIndex].dwSpeed = 0;
		}
	}	
	return dwDiffTime != NOT_MATCH_TIME;
}

bool CSWMatchTransformFilter::MatchTrigger(SIGNAL_PARAM *pSignal, CARLEFT_MATCH_INFO *pMatchInfo)
{
	return true;
}

/**
  @brief ���ܿ�ʹ�ܿ���
  @param [in] 1ʹ�� 0��ʹ��
 */
HRESULT CSWMatchTransformFilter::OnOutputCtrlEnable(WPARAM wParam, LPARAM lParam)
{
	INT iEnable = (INT)wParam;

	m_cParam.fEnable = (iEnable == 1) ? TRUE : FALSE;

	return S_OK;
}

