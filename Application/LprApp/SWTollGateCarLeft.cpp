#include "SWFC.h"
#include "SWLPRApplication.h"
#include "SWTollGateCarLeft.h"
#include "SWLPRParameter.h"

CSWTollGateCarLeft::CSWTollGateCarLeft()
{
}

CSWTollGateCarLeft::~CSWTollGateCarLeft()
{
}

HRESULT CSWTollGateCarLeft::GetEventDetInfo(CSWString& strInfo)
{
    if (FAILED(CSWCarLeft::GetEventDetInfo(strInfo)))
    {
        return E_FAIL ;
    }
    return S_OK;
}

HRESULT CSWTollGateCarLeft::BuildPlateString(TiXmlDocument& xmlDoc)
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
    if (theApp->GetParam().Get().cTrackerCfgParam.cScaleSpeed.fEnable && !GetCarspeed())
    {
        swpa_utils_srand(CSWDateTime::GetSystemTick());
        SetCarspeed(swpa_utils_rand() % 20 + 20.0f);
        if (!GetContent()[0]
            && (GetCarType() == CT_WALKMAN || GetCarType() == CT_BIKE))
        {
            SetCarColor(CC_GREY);
        }
    }

    //����ٶ�
    if (0 < GetCarspeed())
    {
        //����
        pValue = new TiXmlElement("VideoScaleSpeed");
        if (pValue)
        {
            strTmp.Format("%d Km/h", (int)GetCarspeed());
            pValue->SetAttribute("value", (LPCSTR)strTmp);
            pValue->SetAttribute("chnname", "��Ƶ����");
            pResult->LinkEndChild(pValue);
        }
        //����ֵ
        pValue = new TiXmlElement("SpeedLimit");
        if (pValue)
        {
            strTmp.Format("%d Km/h", theApp->GetParam().Get().cTrackerCfgParam.iSpeedLimit);
            pValue->SetAttribute("value", (LPCSTR)strTmp);
            pValue->SetAttribute("chnname", "����ֵ");
            pResult->LinkEndChild(pValue);
        }
        //��������������
        pValue = new TiXmlElement("ScaleSpeedOfDistance");
        if (pValue)
        {
            strTmp.Format("%0.2f", GetScaleOfDistance());
            pValue->SetAttribute("value", (LPCSTR)strTmp);
            pValue->SetAttribute("chnname", "�������������");
            pResult->LinkEndChild(pValue);
        }
    }

    //��Ч֡��
    if (theApp->GetParam().Get().cResultSenderParam.fOutputObservedFrames)
    {
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
    }

    //�������ʱ��
    if (theApp->GetParam().Get().cResultSenderParam.fOutputCarArriveTime)
    {
        pValue = new TiXmlElement("CarArriveTime");
        if (pValue)
        {
            CSWDateTime dt(GetFirstFrameTime());
            strTmp.Format("%04d-%02d-%02d %02d:%02d:%02d:%03d", dt.GetYear(),
                dt.GetMonth(), dt.GetDay(), dt.GetHour(), dt.GetMinute(),
                dt.GetSecond(), dt.GetMSSecond());
            pValue->SetAttribute("value", (LPCSTR)strTmp);
            pValue->SetAttribute("chnname", "�������ʱ��");
            pResult->LinkEndChild(pValue);
        }
    }

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

    //·������
    pValue = new TiXmlElement("StreetName");
    if (pValue)
    {
        pValue->SetAttribute("value", GetRoadName());
        pValue->SetAttribute("chnname", "·������");
        pResult->LinkEndChild(pValue);
    }

    //·�ڷ���
    pValue = new TiXmlElement("StreetDirection");
    if (pValue)
    {
        pValue->SetAttribute("value", GetRoadDirection());
        pValue->SetAttribute("chnname", "·�ڷ���");
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

    //��ǰ���ȼ���
    pValue = new TiXmlElement("PlateLightType");
    if (pValue)
    {
        strTmp.Format("%02d��", GetPlateLightType());
        pValue->SetAttribute("value", (LPCSTR)strTmp);
        pValue->SetAttribute("chnname", "��������ȵȼ�");
        pResult->LinkEndChild(pValue);
    }

    //��ǰƫ�⾵״̬
    pValue = new TiXmlElement("CplStatus");
    if (pValue)
    {
        strTmp.Format("%d", GetCplStatus());
        pValue->SetAttribute("value", (LPCSTR)strTmp);
        pValue->SetAttribute("chnname", "ƫ�⾵״̬");
        pResult->LinkEndChild(pValue);
    }

    //��ǰ�����������
    // todo.
    // �����²�ʹ��Ƶ���ƣ��������������ȼ���
    if( m_pTrackerCfg != NULL && m_pTrackerCfg->iUsedLight == 1 )
    {
		//��ǰ�����������
		pValue = new TiXmlElement("PulseLevel");
		if (pValue)
		{
			strTmp.Format("%d", GetPulseLevel());
			pValue->SetAttribute("value", (LPCSTR)strTmp);
			pValue->SetAttribute("chnname", "��������ȼ�");
			pResult->LinkEndChild(pValue);
		}
    }

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

    //��������
    pValue = new TiXmlElement("PlateLight");
    if (pValue)
    {
        strTmp.Format("%d", GetCarAvgY());
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

	//��ʻ����
	pValue = new TiXmlElement("FilterReverseEnable");
    if (pValue)
    {
        strTmp.Format("%s", GetReverseRun() ? "����" : "����");
        pValue->SetAttribute("value", (LPCSTR)strTmp);
        pValue->SetAttribute("chnname", "��ʻ����");
        pResult->LinkEndChild(pValue);
    }

    return S_OK ;
}

