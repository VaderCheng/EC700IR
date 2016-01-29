#include "SWFC.h"
#include "SWLPRApplication.h"
#include "SWHvcCarLeft.h"

CSWHvcCarLeft::CSWHvcCarLeft()
	: m_fTollEvasion(FALSE)
	, m_dwTollEvasionTick(0)
{
	m_dwTollEvasionTick = CSWDateTime::GetSystemTick();
}

CSWHvcCarLeft::~CSWHvcCarLeft()
{
}

HRESULT CSWHvcCarLeft::GetEventDetInfo(CSWString& strInfo)
{
    if (FAILED(CSWCarLeft::GetEventDetInfo(strInfo)))
    {
        return E_FAIL ;
    }
    return S_OK ;
}

CSWString CSWHvcCarLeft::BuildPlateString(VOID)
{
    TiXmlDocument xmlDoc;
    if (S_OK == BuildPlateString(xmlDoc))
    {
        TiXmlPrinter cTxPr;
        xmlDoc.Accept(&cTxPr);
        return cTxPr.CStr();
    }
    return "";
}

HRESULT CSWHvcCarLeft::BuildPlateString(TiXmlDocument& xmlDoc)
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
        case PLATE_DOUBLE_WJ:
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

    if (theApp->GetParam().Get().cResultSenderParam.iUseRushRule)
    {
    	pValue = new TiXmlElement("TollEvasion");
        if (pValue)
        {
            pValue->SetAttribute("value", m_fTollEvasion ? "��" : "��");
            pValue->SetAttribute("chnname", "�忨�ӷ�");
            pResult->LinkEndChild(pValue);
        }

        if (m_fTollEvasion)
        {
            CSWDateTime cBeginTime(m_dwTollEvasionTick - 10* 1000);
            CSWDateTime cEndTime(m_dwTollEvasionTick + 10* 1000);
            CHAR szBeginTime[256] = {0};
            CHAR szEndTime[256] = {0};

            swpa_snprintf(szBeginTime, sizeof(szBeginTime), "%4d-%02d-%02d-%02d-%02d-%02d",
                cBeginTime.GetYear(), cBeginTime.GetMonth(), cBeginTime.GetDay(), cBeginTime.GetHour(), cBeginTime.GetMinute(), cBeginTime.GetSecond());
        
            swpa_snprintf(szEndTime, sizeof(szEndTime), "%4d-%02d-%02d-%02d-%02d-%02d",
                cEndTime.GetYear(), cEndTime.GetMonth(), cEndTime.GetDay(), cEndTime.GetHour(), cEndTime.GetMinute(), cEndTime.GetSecond());
        
            pValue = new TiXmlElement("TollEvasionBeginTime");
            if (pValue)
            {
                pValue->SetAttribute("value", szBeginTime);
                pValue->SetAttribute("chnname", "��ʼʱ��");
                pResult->LinkEndChild(pValue);
        }

            pValue = new TiXmlElement("TollEvasionEndTime");
            if (pValue)
            {
                pValue->SetAttribute("value", szEndTime);
                pValue->SetAttribute("chnname", "����ʱ��");
                pResult->LinkEndChild(pValue);
            }
        }
    }

    //��Ч֡��
    if (theApp->GetParam().Get().cResultSenderParam.fOutputObservedFrames)
    {
        pValue = new TiXmlElement("ObservedFrames");
        if (pValue)
        {
            pValue->SetAttribute("value", "1");
            pValue->SetAttribute("chnname", "��Ч֡��");
            pResult->LinkEndChild(pValue);
        }
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
                strTmp = "��ɫ";
                break;
            }
            pValue->SetAttribute("value", (LPCSTR)strTmp);
            pValue->SetAttribute("chnname", "������ɫ");
            pResult->LinkEndChild(pValue);
        }
    }

    //����ʱ��
    DWORD32 dwProcTime = 180 - DWORD32(1 + (54.0 * rand() / (RAND_MAX + 1.0)) - 27);
    pValue = new TiXmlElement("ProcTime");
    if (pValue)
    {
        strTmp.Format("%d", dwProcTime);
        pValue->SetAttribute("value", (LPCSTR)strTmp);
        pValue->SetAttribute("chnname", "����ʱ��");
        pResult->LinkEndChild(pValue);
    }

    //����HSLֵ
    if(1 == GetOutputHSLFlag()) //���Ʋ���ʹ��
    {
        pValue = new TiXmlElement("HSL");
        if (pValue)
        {
        	CHAR szConf[256];
            sprintf(szConf, "H:%d\tS:%d\tL:%d", GetiH(), GetiS(), GetiL());
            pValue->SetAttribute("value", szConf);
            pValue->SetAttribute("chnname", "����HSLֵ");
            pResult->LinkEndChild(pValue);
        }
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

    return S_OK ;
}

