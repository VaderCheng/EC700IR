#include "SWFC.h"
#include "SWLPRApplication.h"
#include "SWTrafficGateCarLeft.h"

CSWTrafficGateCarLeft::CSWTrafficGateCarLeft()
{
}

CSWTrafficGateCarLeft::~CSWTrafficGateCarLeft()
{
}

HRESULT CSWTrafficGateCarLeft::GetEventDetInfo(CSWString& strInfo)
{
	if(FAILED(CSWCarLeft::GetEventDetInfo(strInfo)))
	{
		return E_FAIL;
	}
	

	CSWString strEventCheck = "";
  if (theApp->GetParam().Get().cTrackerCfgParam.nDetReverseRunEnable && GetReverseRun())
  {
      if (!strEventCheck.IsEmpty())
      {
          strEventCheck += ",";
      }
      strEventCheck += "Reverse run";
      INT iPtType = GetPTType();
      iPtType |= PT_CONVERSE;
      SetPTType(iPtType);
  }

//  //ѹ��,�����ߴ����ұ�Ŵ�0��ʼ
//  if (GetDetectOverYellowLineEnable() && GetIsOverYellowLine() != COLT_INVALID)
//  {
//      if (GetIsOverYellowLine() == COLT_NO)
//      {
//          strInfo += "ѹ��:��\n";
//      }
//      else
//      {
//          CSWString strYellowLine = "";
//          strYellowLine.Format("ѹ��:������%d<%s>", GetIsOverYellowLine() + 1, GetIsDoubleYellowLine() ? "����" : "ʵ��");
          
//          strEventCheck += strYellowLine;
//          strEventCheck += ",";
          
//          strInfo += strYellowLine;
//          strInfo += "\n";

//          INT iPtType = GetPTType();
//          iPtType |= PT_OVERLINE;
//          SetPTType(iPtType);
//      }
//  }

//  //Խ��,�����ߴ����ұ�Ŵ�0��ʼ
//  if (GetDetectCrossLineEnable() && GetIsCrossLine() != COLT_INVALID)
//  {
//      if (GetIsCrossLine() == COLT_NO)
//      {
//          strInfo += "Խ��:��\n";
//      }
//      else
//      {
//          CSWString strCrossLine = "";
//          strCrossLine.Format("Խ��:������%d", GetIsCrossLine() + 1);
                    
//          strEventCheck += strCrossLine;
//          strEventCheck += ",";
          
//          strInfo += strCrossLine;
//          strInfo += "\n";

//          INT iPtType = GetPTType();
//          iPtType |= PT_OVERLINE;
//          SetPTType(iPtType);
//      }
//  }

  //����
  if ((int)(GetCarspeed()) > theApp->GetParam().Get().cResultSenderParam.iSpeedLimit)
  {
      if (!strEventCheck.IsEmpty())
      {
          strEventCheck += ",";
      }
      strEventCheck += "Overspeed";

      INT iPtType = GetPTType();
      iPtType |= PT_OVERSPEED;
      SetPTType(iPtType);
  }

  if (!strEventCheck.IsEmpty())
  {
  		//strInfo += "Violate regulations:Yes<";
  		strInfo += strEventCheck;
  		//strInfo += ">";
  }
	return S_OK;
}

HRESULT CSWTrafficGateCarLeft::BuildPlateString(TiXmlDocument& xmlDoc)
{
	// if(FAILED(CSWCarLeft::BuildPlateString(xmlDoc)))
	// {
	// 	return E_FAIL;
	// }
	// return S_OK;
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
		if ( !pElement ) 
		{
			return E_OUTOFMEMORY;
		}
		xmlDoc.RootElement()->LinkEndChild(pElement);
	}

	//ע�⣺һ��Ҫɾ���Ѿ����ڵĽ�
	TiXmlNode *pResultOld = pElement->FirstChild("Result");
	if ( pResultOld )
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
	TiXmlText* pText=new TiXmlText("Unlicensed");
	if (pValue && pText)
	{
		pValue->LinkEndChild(pText);
		pResult->LinkEndChild(pValue);
	}
  
	//������ɫ
	pValue = new TiXmlElement("Color");
	if ( pValue )
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
	/*pValue = new TiXmlElement("Type");
	if ( pValue )
	{
		pValue->SetAttribute("raw_value", GetPlateType());
		int nPlateType = 0;
		switch ( GetPlateType() )
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
	}*/
  
	//������Ϣ
	if(m_pTrackerCfg)
	{
		if(m_pTrackerCfg->nDetReverseRunEnable)
		{
			//����������б�ʶ
			pValue = new TiXmlElement("ReverseRun");
			if (pValue)
			{
				strTmp.Format("%s", GetReverseRun() ? "Yes" : "No");
				pValue->SetAttribute("value", (LPCSTR)strTmp);
				pValue->SetAttribute("chnname", "Reverse Run");
				pResult->LinkEndChild(pValue);
			}
		} 
    
		//�޳����ٶ�
		if(m_pTrackerCfg->cScaleSpeed.fEnable && !GetCarspeed())
		{
			swpa_utils_srand(CSWDateTime::GetSystemTick());
			SetCarspeed(swpa_utils_rand() % 20 + 20.0f);
			if (!GetContent()[0] && (GetCarType() == CT_WALKMAN || GetCarType() == CT_BIKE))
			{
				SetCarColor(CC_GREY);
			}
		}

		//����ٶ�
		if(0 < (INT)GetCarspeed() && (INT)GetCarspeed() < 500)
		{			
			if((swpa_strcmp(m_strSpeedType,"Vedio Speed")!=0)||(m_pTrackerCfg->cScaleSpeed.fEnable&&(swpa_strcmp(m_strSpeedType,"Vedio Speed")==0)))
			{
				pValue = new TiXmlElement("VideoScaleSpeed");
				if (pValue)
				{
					strTmp.Format("%d Km/h", (int)GetCarspeed());
					pValue->SetAttribute("value", (LPCSTR)strTmp);
					pValue->SetAttribute("chnname", (LPCSTR)m_strSpeedType);
					pResult->LinkEndChild(pValue);
				}

				//����ֵ
				pValue = new TiXmlElement("SpeedLimit");
				if (pValue)
				{
			  		strTmp.Format("%d Km/h", m_pTrackerCfg->iSpeedLimit);
			  		pValue->SetAttribute("value", (LPCSTR)strTmp);
			  		pValue->SetAttribute("chnname", "Speed Limit");
			  		pResult->LinkEndChild(pValue);
				}
			}
		}
    
		//����ٶ�
		/*if(0 < (INT)GetCarspeed() && (INT)GetCarspeed() < 500)
		{ //����
			pValue = new TiXmlElement("VideoScaleSpeed");
			if (pValue)
			{
				strTmp.Format("%d Km/h", (int)GetCarspeed());
				pValue->SetAttribute("value", (LPCSTR)strTmp);
				pValue->SetAttribute("chnname", (LPCSTR)m_strSpeedType);
				pResult->LinkEndChild(pValue);
			}
		}
		else
		{
			pValue = new TiXmlElement("VideoScaleSpeed");
			if (pValue)
			{
				swpa_utils_srand(CSWDateTime::GetSystemTick());
				INT iRandSpeed = (INT)(swpa_utils_rand() % 20 + 20.0f);
				strTmp.Format("%d Km/h", iRandSpeed);
				SW_TRACE_DEBUG("Plate %s set random speed value:%d", (LPCSTR)strPlate, iRandSpeed);
				pValue->SetAttribute("value", (LPCSTR)strTmp);
				pValue->SetAttribute("chnname", (LPCSTR)m_strSpeedType);
				pResult->LinkEndChild(pValue);
			}

			//����ֵ
			pValue = new TiXmlElement("SpeedLimit");
			if (pValue)
			{
		  		strTmp.Format("%d Km/h", m_pTrackerCfg->iSpeedLimit);
		  		pValue->SetAttribute("value", (LPCSTR)strTmp);
		  		pValue->SetAttribute("chnname", "Speed Limit");
		  		pResult->LinkEndChild(pValue);
			}  
		}*/
		//��Ч֡��
		pValue = new TiXmlElement("ObservedFrames");
		if (pValue)
		{
			strTmp.Format("%d", GetObservedFrames());
			pValue->SetAttribute("value", (LPCSTR)strTmp);
			pValue->SetAttribute("chnname", "Effective Frames");
			pResult->LinkEndChild(pValue);
        }

        //track id
        pValue = new TiXmlElement("TrackID");
        if (pValue)
        {
            strTmp.Format("%d", GetTrackId());
            pValue->SetAttribute("value", (LPCSTR)strTmp);
            pValue->SetAttribute("chnname", "Track ID");
            pResult->LinkEndChild(pValue);
        }

		//���Ŷ�
		/*pValue = new TiXmlElement("Confidence");
		if (pValue)
		{
			strTmp.Format("%.3f", swpa_exp(swpa_log(GetAverageConfidence()) * 0.143));
			pValue->SetAttribute("value", (LPCSTR)strTmp);
			pValue->SetAttribute("chnname", "ƽ�����Ŷ�");
			pResult->LinkEndChild(pValue);
		}*/

		//���ַ����Ŷ�
		/*pValue = new TiXmlElement("FirstCharConf");
		if (pValue)
		{
			strTmp.Format("%.3f", GetFirstAverageConfidence());
			pValue->SetAttribute("value", (LPCSTR)strTmp);
			pValue->SetAttribute("chnname", "���ֿ��Ŷ�");
			pResult->LinkEndChild(pValue);
		}*/
    
		//�������ʱ��
		pValue = new TiXmlElement("CarArriveTime");
		if (pValue)
		{
    		CSWDateTime dt(GetFirstFrameTime());
			strTmp.Format("%04d-%02d-%02d %02d:%02d:%02d:%03d", dt.GetYear(), dt.GetMonth(), dt.GetDay(), dt.GetHour(), dt.GetMinute(), dt.GetSecond(), dt.GetMSSecond());
			pValue->SetAttribute("value", (LPCSTR)strTmp);
			pValue->SetAttribute("chnname", "Vehicle Detection Time");
			pResult->LinkEndChild(pValue);
		}
    
		//˫��������
		/*switch(GetPlateType())
		{
		case PLATE_DOUBLE_YELLOW: strTmp = "˫"; break;
		case PLATE_DOUBLE_MOTO  : strTmp = "Ħ"; break;
		default : strTmp = ""; break;
		}
		if(!strTmp.IsEmpty())
		{
			pValue = new TiXmlElement("DoublePlate");
			if (pValue)
			{
				pValue->SetAttribute("value", (LPCSTR)strTmp);
				pValue->SetAttribute("chnname", "��������");
				pResult->LinkEndChild(pValue);
			}
		}*/                    
    
		//�����ߴ�
		/*if(m_pTrackerCfg->fOutputCarSize && GetCarType() != CT_WALKMAN && GetCarType() != CT_BIKE && GetPlateType() != PLATE_DOUBLE_MOTO)
		{
			INT iOutType;
			FLOAT iWidth, iHeight;
			pValue = new TiXmlElement("CarSize");
			if(pValue && S_OK == GetCarSize(iOutType, iWidth, iHeight))
			{
				if(!iOutType)
				{
					strTmp.Format("%d ����(����):%d", (INT)iWidth, (INT)iHeight);    			
				}
				else
				{    			
					strTmp.Format("%.2f ����(��):%.2f", iWidth, iHeight);
				}
				pValue->SetAttribute("value", (LPCSTR)strTmp);
				strTmp.Format("����(%s)", iOutType == 0 ? "����" : "��");
				pValue->SetAttribute("chnname", (LPCSTR)strTmp);
				pResult->LinkEndChild(pValue);
			}
		}*/
    
		//��������
		/*pValue = new TiXmlElement("CarType");
		if (pValue)
		{
			if (CT_LARGE == GetCarType() && PLATE_DOUBLE_MOTO != GetPlateType())
			{
				strTmp = "��";
			}
			else if (CT_SMALL == GetCarType() && PLATE_DOUBLE_MOTO != GetPlateType())
			{
				strTmp = "С";
			}
			else if (CT_MID == GetCarType() && PLATE_DOUBLE_MOTO != GetPlateType())
			{
				strTmp = "��";
			}
			else if (PLATE_DOUBLE_MOTO == GetPlateType())
			{
				strTmp = "Ħ�г�";
			}
			else if (CT_WALKMAN == GetCarType())
			{
				strTmp = "����";
			}
			else if (CT_BIKE == GetCarType())
			{
				strTmp = "�ǻ�����";
			}
			else if (CT_VEHICLE == GetCarType())
			{
				strTmp = "������";
			}
			else
			{
				strTmp = "δ֪";
			}
			pValue->SetAttribute("value", (LPCSTR)strTmp);
			pValue->SetAttribute("chnname", "��������");
			pResult->LinkEndChild(pValue);
		}*/
    
		//������ɫ
		/*if (m_pTrackerCfg->fEnableRecgCarColor)
		{
			pValue = new TiXmlElement("CarColor");
			if (pValue)
			{
				switch (GetCarColor())
				{
				case CC_WHITE:  strTmp = "��ɫ"; break;
				case CC_GREY :  strTmp = "��ɫ"; break;
				case CC_BLACK:  strTmp = "��ɫ"; break;
				case CC_RED:    strTmp = "��ɫ"; break;
				case CC_YELLOW: strTmp = "��ɫ"; break;
				case CC_GREEN:  strTmp = "��ɫ"; break;
				case CC_BLUE:   strTmp = "��ɫ"; break;
				case CC_PURPLE: strTmp = "��ɫ"; break;
				case CC_PINK:   strTmp = "��ɫ"; break;
				case CC_BROWN:  strTmp = "��ɫ"; break;
				default:        strTmp = "δ֪"; break;
				}
				pValue->SetAttribute("value", (LPCSTR)strTmp);
				pValue->SetAttribute("chnname", "������ɫ");
				pResult->LinkEndChild(pValue);
			}
		}*/

		//������
		pValue = new TiXmlElement("RoadNumber");
		if (pValue)
		{
			//strTmp.Format("%d", GetRoadNo() + (255 == GetRoadNo() ? 0 : m_pTrackerCfg->iStartRoadNum));
			strTmp.Format("%d", GetOutPutRoadNo());
			pValue->SetAttribute("value", (LPCSTR)strTmp);
			pValue->SetAttribute("chnname", "Lane");
			pResult->LinkEndChild(pValue);
		}
    
		//��ʼ������
		pValue = new TiXmlElement("BeginRoadNumber");
		if (pValue)
		{
			if (m_pTrackerCfg->iRoadNumberBegin == 0)
			{
				strTmp.Format("<Left,%d>", m_pTrackerCfg->iStartRoadNum);
			}
			else
			{
				strTmp.Format("<Right,%d>", m_pTrackerCfg->iStartRoadNum);
			}
			pValue->SetAttribute("value", (LPCSTR)strTmp);
			pValue->SetAttribute("chnname", "Start Lane");
			pResult->LinkEndChild(pValue);
		}
    
		//·������
		pValue = new TiXmlElement("StreetName");
		if (pValue)
		{
			pValue->SetAttribute("value", GetRoadName());
			pValue->SetAttribute("chnname", "Road Name");
			pResult->LinkEndChild(pValue);
		}
    
		//·�ڷ���
		pValue = new TiXmlElement("StreetDirection");
		if (pValue)
		{
			pValue->SetAttribute("value", GetRoadDirection());
			pValue->SetAttribute("chnname", "Road Direction");
			pResult->LinkEndChild(pValue);
		}   
	}
  
  	//�¼������Ϣ
  	strTmp = "";
  	if (S_OK == GetEventDetInfo(strTmp) && !strTmp.IsEmpty())
  	{
  		pValue = new TiXmlElement("EventCheck");
  		if (pValue)
  		{
  	   		pValue->SetAttribute("value", (LPCSTR)strTmp);
  	    	pValue->SetAttribute("chnname", "Event Check");
  	    	pResult->LinkEndChild(pValue);
  		}
	}
	//��Ƶ֡��
	CSWString strFrameName;
	for(int i = 0; i < 5; i++)
	{
		if(GetImage(i) && GetImage(i)->GetFrameName())
		{
			strFrameName = GetImage(i)->GetFrameName();
			break;
		}
	}
	if(!strFrameName.IsEmpty())
	{
		pValue = new TiXmlElement("FrameName");
		if ( pValue )
		{
			pValue->SetAttribute("value", (LPCSTR)strFrameName);
			pValue->SetAttribute("chnname", "Name of Video Frame");
			pResult->LinkEndChild(pValue);        
		}
	}
	
	//��ǰ���ȼ���
	pValue = new TiXmlElement("PlateLightType");
	if (pValue)
	{
		strTmp.Format("Level %02d", GetPlateLightType());
		pValue->SetAttribute("value", (LPCSTR)strTmp);
		pValue->SetAttribute("chnname", "Camera Brightness");
		pResult->LinkEndChild(pValue);
	}

	//��ǰƫ�⾵״̬
	/*pValue = new TiXmlElement("CplStatus");
	  if (pValue)
	  {
	  strTmp.Format("%d", GetCplStatus());
	  pValue->SetAttribute("value", (LPCSTR)strTmp);
	  pValue->SetAttribute("chnname", "ƫ�⾵״̬");
	  pResult->LinkEndChild(pValue);
	  }

    pValue = new TiXmlElement("WdrLevel");
    if (pValue)
    {
        strTmp.Format("%d", GetWdrLevel());
        pValue->SetAttribute("value", (LPCSTR)strTmp);
        pValue->SetAttribute("chnname", "WDR�ȼ�");
        pResult->LinkEndChild(pValue);
    }*/

	//��ǰ�����������
	// todo.
	// �����²�ʹ��Ƶ���ƣ��������������ȼ���
	/*if( m_pTrackerCfg != NULL && m_pTrackerCfg->iUsedLight == 1 )
	{
		pValue = new TiXmlElement("PulseLevel");
		if (pValue)
		{
			strTmp.Format("%d", GetPulseLevel());
			pValue->SetAttribute("value", (LPCSTR)strTmp);
			pValue->SetAttribute("chnname", "Strobe Pulse Width");
			pResult->LinkEndChild(pValue);
		}
	}*/
  
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
		pValue->SetAttribute("chnname", "Ambient Brightness");
		pResult->LinkEndChild(pValue);
	}

	//��������
	/*pValue = new TiXmlElement("PlateLight");
	if (pValue)
	{
		strTmp.Format("%d", GetCarAvgY());
		pValue->SetAttribute("value", (LPCSTR)strTmp);
		pValue->SetAttribute("chnname", "��������");
		pResult->LinkEndChild(pValue);
	}*/

	//���ƶԱȶ�
	/*pValue = new TiXmlElement("PlateVariance");
	if (pValue)
	{
		strTmp.Format("%d", GetCarVariance());
		pValue->SetAttribute("value", (LPCSTR)strTmp);
		pValue->SetAttribute("chnname", "���ƶԱȶ�");
		pResult->LinkEndChild(pValue);
	} */

	// ������Ϣ
	CSWString strFilterInfo = GetFilterInfo();
	if( !strFilterInfo.IsEmpty() )
	{
		pValue = new TiXmlElement("ResultProcess");
		if (pValue)
		{
			pValue->SetAttribute("value", strFilterInfo);
			pValue->SetAttribute("chnname", "Background processing information");
			pResult->LinkEndChild(pValue);
		} 
	}

	return S_OK;
}

