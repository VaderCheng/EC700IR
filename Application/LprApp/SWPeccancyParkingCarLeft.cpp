#include "SWFC.h"
#include "SWLPRApplication.h"
#include "SWPeccancyParkingCarLeft.h"

CSWPeccancyParkingCarLeft::CSWPeccancyParkingCarLeft()
{
	for(DWORD i = 0; i < 30; i++)
		m_rgpVideo[i] = NULL;
	m_iVideoCount = 0;
}

CSWPeccancyParkingCarLeft::~CSWPeccancyParkingCarLeft()
{
	for(DWORD i = 0; i < 30; i++)
	{
		if(m_rgpVideo[i] != NULL)
			SAFE_RELEASE(m_rgpVideo[i]);
	}
}


HRESULT CSWPeccancyParkingCarLeft::GetEventDetInfo(CSWString& strInfo)
{
    if (FAILED(CSWCarLeft::GetEventDetInfo(strInfo)))
    {
        return E_FAIL ;
    }
    return S_OK;
}

HRESULT CSWPeccancyParkingCarLeft::BuildPlateString(TiXmlDocument& xmlDoc)
{
    CSWString strTmp;
    TiXmlDeclaration* pDecl = new TiXmlDeclaration("1.0", "GB2312", "yes");
    TiXmlElement* pElement = new TiXmlElement("HvcResultDoc");
    xmlDoc.LinkEndChild(pDecl);
    xmlDoc.LinkEndChild(pElement);

    //ȡ��ResultSet��
    pElement = xmlDoc.RootElement()->FirstChildElement("ResultSet");
    if (!pElement)
    {
        pElement = new TiXmlElement("ResultSet");
        if (!pElement)
        {
            return E_OUTOFMEMORY ;
        }
        xmlDoc.RootElement()->LinkEndChild(pElement);
    }

    //ע�⣺һ��Ҫɾ���Ѿ����ڵĽ�
    TiXmlNode *pResultOld = pElement->FirstChild("Result");
    if (pResultOld)
    {
        pElement->RemoveChild(pResultOld);
    }
    TiXmlElement* pResult = new TiXmlElement("Result");
    if (pResult)
    {
        pElement->LinkEndChild(pResult);
    }

    //���ƺ���
    CSWString strPlate = GetPlateNo();
    TiXmlElement* pValue = new TiXmlElement("PlateName");
    TiXmlText* pText = new TiXmlText((LPCSTR)strPlate);
    if (pValue && pText)
    {
        pValue->LinkEndChild(pText);
        pResult->LinkEndChild(pValue);
    }

    //������ɫ
    pValue = new TiXmlElement("Color");
    if (pValue)
    {
        pValue->SetAttribute("raw_value", GetPlateColor());
        int nColorType = 0;
        if (swpa_strncmp((LPCSTR)strPlate, "��", 2) == 0)
        {
            nColorType = 1;
        }
        else if (swpa_strncmp((LPCSTR)strPlate, "��", 2) == 0)
        {
            nColorType = 2;
        }
        else if (swpa_strncmp((LPCSTR)strPlate, "��", 2) == 0)
        {
            nColorType = 3;
        }
        else if (swpa_strncmp((LPCSTR)strPlate, "��", 2) == 0)
        {
            nColorType = 4;
        }
        else if (swpa_strncmp((LPCSTR)strPlate, "��", 2) == 0)
        {
            nColorType = 5;
        }
        else
        {
            nColorType = 0;
        }
        pValue->SetAttribute("value", nColorType);
        pResult->LinkEndChild(pValue);
    }

    //��������
    pValue = new TiXmlElement("Type");
    if (pValue)
    {
        pValue->SetAttribute("raw_value", GetPlateType());
        int nPlateType = 0;
        switch (GetPlateType())
        {
        case PLATE_NORMAL:
            case PLATE_POLICE:
            nPlateType = 1;
            break;
        case PLATE_WJ:
            nPlateType = 2;
            break;
        case PLATE_POLICE2:
            nPlateType = 3;
            break;
        case PLATE_DOUBLE_YELLOW:
            case PLATE_DOUBLE_MOTO:
            nPlateType = 4;
            break;
        default:
            nPlateType = 0;
            break;
        }
        pValue->SetAttribute("value", nPlateType);
        pResult->LinkEndChild(pValue);
    }

    if (theApp->GetParam().Get().cResultSenderParam.fOutputAppendInfo == FALSE)
    {
        return S_OK ;
    }

    //������Ϣ
    /*if (theApp->GetParam().Get().cTrackerCfgParam.nDetReverseRunEnable)
    {
        //����������б�ʶ
        pValue = new TiXmlElement("ReverseRun");
        if (pValue)
        {
            strTmp.Format("%s", GetReverseRun() ? "��" : "��");
            pValue->SetAttribute("value", (LPCSTR)strTmp);
            pValue->SetAttribute("chnname", "����������ʻ");
            pResult->LinkEndChild(pValue);
        }
    }*/

    //�޳����ٶ�
    // if (theApp->GetParam().Get().cTrackerCfgParam.cScaleSpeed.fEnable && !GetCarspeed())
    // {
    //     swpa_utils_srand(CSWDateTime::GetSystemTick());
    //     SetCarspeed(swpa_utils_rand() % 20 + 20.0f);
    //     if (!GetContent()[0]
    //         && (GetCarType() == CT_WALKMAN || GetCarType() == CT_BIKE))
    //     {
    //         SetCarColor(CC_GREY);
    //     }
    // }

    // //����ٶ�
    // if (0 < GetCarspeed())
    // {
    //     //����
    //     pValue = new TiXmlElement("VideoScaleSpeed");
    //     if (pValue)
    //     {
    //         strTmp.Format("%d Km/h", (int)GetCarspeed());
    //         pValue->SetAttribute("value", (LPCSTR)strTmp);
    //         pValue->SetAttribute("chnname", "��Ƶ����");
    //         pResult->LinkEndChild(pValue);
    //     }
    //     //����ֵ
    //     pValue = new TiXmlElement("SpeedLimit");
    //     if (pValue)
    //     {
    //         strTmp.Format("%d Km/h", theApp->GetParam().Get().cTrackerCfgParam.iSpeedLimit);
    //         pValue->SetAttribute("value", (LPCSTR)strTmp);
    //         pValue->SetAttribute("chnname", "����ֵ");
    //         pResult->LinkEndChild(pValue);
    //     }
    //     //��������������
    //     pValue = new TiXmlElement("ScaleSpeedOfDistance");
    //     if (pValue)
    //     {
    //         strTmp.Format("%0.2f", GetScaleOfDistance());
    //         pValue->SetAttribute("value", (LPCSTR)strTmp);
    //         pValue->SetAttribute("chnname", "�������������");
    //         pResult->LinkEndChild(pValue);
    //     }
    // }

    //��Ч֡��
    if (theApp->GetParam().Get().cResultSenderParam.fOutputObservedFrames)
    {
		/*
        pValue = new TiXmlElement("ObservedFrames");
        if (pValue)
        {
            strTmp.Format("%d", GetObservedFrames());
            pValue->SetAttribute("value", (LPCSTR)strTmp);
            pValue->SetAttribute("chnname", "��Ч֡��");
            pResult->LinkEndChild(pValue);
        }

        //���Ŷ�
        pValue = new TiXmlElement("Confidence");
        if (pValue)
        {
            strTmp.Format("%.3f", swpa_exp(swpa_log(GetAverageConfidence()) * 0.143));
            pValue->SetAttribute("value", (LPCSTR)strTmp);
            pValue->SetAttribute("chnname", "ƽ�����Ŷ�");
            pResult->LinkEndChild(pValue);
        }

        //���ַ����Ŷ�
        pValue = new TiXmlElement("FirstCharConf");
        if (pValue)
        {
            strTmp.Format("%.3f", GetFirstAverageConfidence());
            pValue->SetAttribute("value", (LPCSTR)strTmp);
            pValue->SetAttribute("chnname", "���ֿ��Ŷ�");
            pResult->LinkEndChild(pValue);
        }
		*/
    }

	
	pValue = new TiXmlElement("BestSnapShotTime");
	if(pValue)
	{
		strTmp.Format("%d-%d-%d %02d:%02d:%02d:%03d", 
					  m_cDetectCarTime.year,
					  m_cDetectCarTime.month,
					  m_cDetectCarTime.day,
					  m_cDetectCarTime.hour,
					  m_cDetectCarTime.min,
					  m_cDetectCarTime.sec,
					  m_cDetectCarTime.msec);
		pValue->SetAttribute("value", (LPCSTR)strTmp);
		pValue->SetAttribute("chnname", "���ʱ��");
		pResult->LinkEndChild(pValue);
	}


	pValue = new TiXmlElement("BeginCaptureTime");
	if(pValue)
	{
		strTmp.Format("%d-%d-%d %02d:%02d:%02d:%03d", 
					  m_cPeccancyTime.year,
					  m_cPeccancyTime.month,
					  m_cPeccancyTime.day,
					  m_cPeccancyTime.hour,
					  m_cPeccancyTime.min,
					  m_cPeccancyTime.sec,
					  m_cPeccancyTime.msec);
		pValue->SetAttribute("value", (LPCSTR)strTmp);
		pValue->SetAttribute("chnname", "Υ��ʱ��");
		pResult->LinkEndChild(pValue);
	}

    //�������ʱ��
    // if (theApp->GetParam().Get().cResultSenderParam.fOutputCarArriveTime)
    // {
    //     pValue = new TiXmlElement("CarArriveTime");
    //     if (pValue)
    //     {
    //         CSWDateTime dt(GetFirstFrameTime());
    //         strTmp.Format("%04d-%02d-%02d %02d:%02d:%02d:%03d", dt.GetYear(),
    //             dt.GetMonth(), dt.GetDay(), dt.GetHour(), dt.GetMinute(),
    //             dt.GetSecond(), dt.GetMSSecond());
    //         pValue->SetAttribute("value", (LPCSTR)strTmp);
    //         pValue->SetAttribute("chnname", "�������ʱ��");
    //         pResult->LinkEndChild(pValue);
    //     }
    // }

	/*
    //˫��������
    switch (GetPlateType())
    {
    case PLATE_DOUBLE_YELLOW:
        strTmp = "˫";
        break;
    case PLATE_DOUBLE_MOTO:
        strTmp = "Ħ";
        break;
    default:
        strTmp = "";
        break;
    }
    if (!strTmp.IsEmpty())
    {
        pValue = new TiXmlElement("DoublePlate");
        if (pValue)
        {
            pValue->SetAttribute("value", (LPCSTR)strTmp);
            pValue->SetAttribute("chnname", "��������");
            pResult->LinkEndChild(pValue);
        }
    }
	*/

    //������ɫ
    if (theApp->GetParam().Get().cTrackerCfgParam.fEnableRecgCarColor)
    {
        pValue = new TiXmlElement("CarColor");
        if (pValue)
        {
            switch (GetCarColor())
            {
            case CC_WHITE:
                strTmp = "��ɫ";
                break;
            case CC_GREY:
                strTmp = "��ɫ";
                break;
            case CC_BLACK:
                strTmp = "��ɫ";
                break;
            case CC_RED:
                strTmp = "��ɫ";
                break;
            case CC_YELLOW:
                strTmp = "��ɫ";
                break;
            case CC_GREEN:
                strTmp = "��ɫ";
                break;
            case CC_BLUE:
                strTmp = "��ɫ";
                break;
            case CC_PURPLE:
                strTmp = "��ɫ";
                break;
            case CC_PINK:
                strTmp = "��ɫ";
                break;
            case CC_BROWN:
                strTmp = "��ɫ";
                break;
            default:
                strTmp = "δ֪";
                break;
            }
            pValue->SetAttribute("value", (LPCSTR)strTmp);
            pValue->SetAttribute("chnname", "������ɫ");
            pResult->LinkEndChild(pValue);
        }
    }

	//Ԥ��λ����
	pValue = new TiXmlElement("PresetName");
	if(pValue)
	{
		pValue->SetAttribute("value", m_strPresetName);
		pValue->SetAttribute("chnname", "Ԥ��λ");
		pResult->LinkEndChild(pValue);
	}

    //·������
    pValue = new TiXmlElement("StreetName");
    if (pValue)
    {
        pValue->SetAttribute("value", GetRoadName());
        pValue->SetAttribute("chnname", "·��");
        pResult->LinkEndChild(pValue);
    }

    //·�ڷ���
    // pValue = new TiXmlElement("StreetDirection");
    // if (pValue)
    // {
    //     pValue->SetAttribute("value", GetRoadDirection());
    //     pValue->SetAttribute("chnname", "·�ڷ���");
    //     pResult->LinkEndChild(pValue);
    // }
	pValue = new TiXmlElement("EventCheck");
	if(pValue)
	{
		pValue->SetAttribute("value", "Υ��:��<Υ��ͣ��>");
		pValue->SetAttribute("chnname", "�¼����");
		pResult->LinkEndChild(pValue);
	}

    //��Ƶ֡��
    if (GetImage(0) && GetImage(0)->GetFrameName())
    {
        pValue = new TiXmlElement("FrameName");
        if (pValue)
        {
            pValue->SetAttribute("value", GetImage(0)->GetFrameName());
            pValue->SetAttribute("chnname", "��Ƶ֡��");
            pResult->LinkEndChild(pValue);
        }
    }

    //��������
    pValue = new TiXmlElement("PlateLight");
    if (pValue)
    {
        strTmp.Format("%d", GetCarAvgY());
        pValue->SetAttribute("value", (LPCSTR)strTmp);
        pValue->SetAttribute("chnname", "��������");
        pResult->LinkEndChild(pValue);
    }

/*
    //����������
    pValue = new TiXmlElement("AmbientLight");
    if (pValue)
    {
        //todo: ��ܻ�������Ϊ0�����
        static INT iPrevAvgY = 10;
        INT iAvgY = GetAvgY();
        if (0 == iAvgY)
        {
            iAvgY = iPrevAvgY;
        }
        else
        {
            iPrevAvgY = iAvgY;
        }
  
        strTmp.Format("%d", iAvgY);
        pValue->SetAttribute("value", (LPCSTR)strTmp);
        pValue->SetAttribute("chnname", "��������");
        pResult->LinkEndChild(pValue);
    }



    //���ƶԱȶ�
    pValue = new TiXmlElement("PlateVariance");
    if (pValue)
    {
        strTmp.Format("%d", GetCarVariance());
        pValue->SetAttribute("value", (LPCSTR)strTmp);
        pValue->SetAttribute("chnname", "���ƶԱȶ�");
        pResult->LinkEndChild(pValue);
    }


    // ������Ϣ
    CSWString strFilterInfo = GetFilterInfo();
    if (!strFilterInfo.IsEmpty())
    {
        pValue = new TiXmlElement("ResultProcess");
        if (pValue)
        {
            pValue->SetAttribute("value", strFilterInfo);
            pValue->SetAttribute("chnname", "������Ϣ");
            pResult->LinkEndChild(pValue);
        }
    }
*/


	//��ʻ����
	// pValue = new TiXmlElement("FilterReverseEnable");
    // if (pValue)
    // {
    //     strTmp.Format("%s", GetReverseRun() ? "����" : "����");
    //     pValue->SetAttribute("value", (LPCSTR)strTmp);
    //     pValue->SetAttribute("chnname", "��ʻ����");
    //     pResult->LinkEndChild(pValue);
    // }

    return S_OK ;
}


HRESULT CSWPeccancyParkingCarLeft::SetDetectCarTime(SWPA_DATETIME_TM &cTime)
{
	m_cDetectCarTime = cTime;
	return S_OK;
}

HRESULT CSWPeccancyParkingCarLeft::SetPeccancyTime(SWPA_DATETIME_TM &cTime)
{
	m_cPeccancyTime = cTime;
	return S_OK;
}

HRESULT CSWPeccancyParkingCarLeft::SetPresetName(LPCSTR szName)
{
	m_strPresetName = szName;
}


VOID CSWPeccancyParkingCarLeft::SetVideo(INT index, CSWImage *pVideo)
{
	if(index < 0 || index >= 30)
		return;
	if(m_rgpVideo[index] != NULL)
	{
		SAFE_RELEASE(m_rgpVideo[index]);
		m_iVideoCount--;
		m_iVideoCount = m_iVideoCount < 0 ? 0 : m_iVideoCount;
	}
	m_rgpVideo[index] = pVideo;
	if(pVideo != NULL)
	{
		SAFE_ADDREF(m_rgpVideo[index]);
		m_iVideoCount++;
	}
	else
	{
		SW_TRACE_DEBUG("ERROR:set video fail. index:%d\n", index);
	}
}

CSWImage *CSWPeccancyParkingCarLeft::GetVideo(INT index)
{
	if(index < 0 || index >= 30)
		return NULL;
	return m_rgpVideo[index];
}

INT CSWPeccancyParkingCarLeft::GetVideoCount()
{
	return m_iVideoCount;
}
