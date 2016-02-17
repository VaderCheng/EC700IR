#include "swpa.h"
#include "SWFC.h"
#include "SWCarLeft.h"
#include "swplate.h"
#include "SWMessage.h"

CHAR CSWCarLeft::s_szRoadName[128] = {0};
CHAR CSWCarLeft::s_szRoadDirection[128] = {0};
INT CSWCarLeft::s_iOutputBestImage = 1;
INT CSWCarLeft::s_iOutputLastImage = 1;
INT CSWCarLeft::s_iOutputCaptureImage = 0;
INT CSWCarLeft::s_iOutputCropImage = 0;
INT CSWCarLeft::s_iCropWidth = 1024;
INT CSWCarLeft::s_iCropHeight = 1024;
INT CSWCarLeft::s_iOutputHSL = 0;

CSWCarLeft::CSWCarLeft()
{
	m_pTrackerCfg = NULL;
	m_pCarLeft = new CARLEFT_INFO_STRUCT;
	swpa_memset(m_pCarLeft, 0, sizeof(CARLEFT_INFO_STRUCT));
	SetCarArriveTime(CSWDateTime::GetSystemTick());
	m_strFilterInfo = "";
	m_strSpeedType = "��Ƶ����";
	m_fReadOnly = false;
    m_fCropLastImage = FALSE;
    m_fCaptureRecogResult = FALSE;
}

HRESULT CSWCarLeft::SetParameter(TRACKER_CFG_PARAM *pCfg, CARLEFT_INFO_STRUCT *pCarLeft)
{
	if(pCfg)
	{
		m_pTrackerCfg = pCfg;
	}
	if(pCarLeft)
	{
		swpa_memcpy(m_pCarLeft, pCarLeft, sizeof(CARLEFT_INFO_STRUCT));
		if(!s_iOutputBestImage)
		{
			SetImage(0, NULL);
		}
		if(!s_iOutputLastImage)
		{
			SetImage(1, NULL);
		}
        if (!s_iOutputCropImage)
        {
            SetImage(0, NULL);
        }
		for(int i = 0; i < m_pCarLeft->cCoreResult.nFaceCount; i++)
		{
			m_pCarLeft->cCoreResult.rgFaceRect[i].top *= 2;
			m_pCarLeft->cCoreResult.rgFaceRect[i].bottom *= 2;
		}
		m_strPlateNo = "";
		m_strFilterInfo = "";
	}
	return S_OK;
}

CSWCarLeft::~CSWCarLeft()
{
   CSWImage *pImage[] = 
	{
		  (CSWImage*)m_pCarLeft->cCoreResult.cResultImg.pimgBestSnapShot
		, (CSWImage*)m_pCarLeft->cCoreResult.cResultImg.pimgLastSnapShot
		, (CSWImage*)m_pCarLeft->cCoreResult.cResultImg.pimgBeginCapture
		, (CSWImage*)m_pCarLeft->cCoreResult.cResultImg.pimgBestCapture
		, (CSWImage*)m_pCarLeft->cCoreResult.cResultImg.pimgLastCapture
		, (CSWImage*)m_pCarLeft->cCoreResult.cResultImg.pimgPlate
		, (CSWImage*)m_pCarLeft->cCoreResult.cResultImg.pimgPlateBin
	};
	for(int i = 0; i < GetImageCount(); i++)
	{
		SAFE_RELEASE(pImage[i]);
	}
	delete m_pCarLeft;
}



void CSWCarLeft::SetRoadInfo(const CHAR* szName, const CHAR* szDirection)
{
	if( szName != NULL && swpa_strlen(szName) < 128 )
	{
		swpa_strcpy(s_szRoadName, szName);
	}
	else
	{
		s_szRoadName[0] = 0;
	}

	if( szDirection != NULL && swpa_strlen(szDirection) < 128 )
	{
		swpa_strcpy(s_szRoadDirection, szDirection);
	}
	else
	{
		s_szRoadDirection[0] = 0;
	}
}

CHAR* CSWCarLeft::GetRoadName()
{
	return s_szRoadName;
}
CHAR* CSWCarLeft::GetRoadDirection()
{
	return s_szRoadDirection;
}

void CSWCarLeft::SetOutputMode(const INT& iBestImage, const INT& iLastImage, const INT& iCaptureImage, const INT& iLastCropImage, const INT &iCropWidthLevel, const INT &iCropHeightLevel)
{
	s_iOutputBestImage = iBestImage;
	s_iOutputLastImage = iLastImage;
	s_iOutputCaptureImage = iCaptureImage;
    s_iOutputCropImage = iLastCropImage;

    switch(iCropWidthLevel)
    {
        case 1:
            s_iCropWidth = 640;
            break;
        case 2:
            s_iCropWidth = 768;
            break;
        case 3:
            s_iCropWidth = 896;
            break;
        case 4:
            s_iCropWidth = 1024;
            break;
        case 5:
            s_iCropWidth = 1152;
            break;
        case 6:
            s_iCropWidth = 1280;
            break;
        default:
            break;
    }

    switch(iCropHeightLevel)
    {
        case 1:
            s_iCropHeight = 640;
            break;
        case 2:
            s_iCropHeight = 768;
            break;
        case 3:
            s_iCropHeight = 896;
            break;
        case 4:
            s_iCropHeight = 1024;
            break;
        case 5:
            s_iCropHeight = 1152;
            break;
        case 6:
            s_iCropHeight = 1280;
            break;
        default:
            break;
    }
}

INT CSWCarLeft::GetOutputBestImage()
{
	return s_iOutputBestImage;
}

INT CSWCarLeft::GetOutputLastImage()
{
	return s_iOutputLastImage;
}

INT CSWCarLeft::GetOutputCaptureImage()
{
    return s_iOutputCaptureImage;
}

INT CSWCarLeft::GetOutputCropImage()
{
    return s_iOutputCropImage;
}

void CSWCarLeft::GetCropWidthHeight(INT &iCropWidth, INT &iCropHeight)
{
    iCropWidth = s_iCropWidth;
    iCropHeight = s_iCropHeight;
}

void CSWCarLeft::SetOutputHSLFlag(const INT& iEnable)
{
	s_iOutputHSL = iEnable;
}

INT CSWCarLeft::GetOutputHSLFlag()
{
	return s_iOutputHSL;
}

VOID CSWCarLeft::SetFilterInfo(CSWString& strFilterInfo)
{
	m_strFilterInfo = strFilterInfo;
}

CSWString CSWCarLeft::GetFilterInfo()
{
	return m_strFilterInfo;
}

DWORD CSWCarLeft::GetOutPutRoadNo(VOID)
{
	if (m_pTrackerCfg != NULL)
	{
		if (0 == m_pTrackerCfg->iRoadNumberBegin)//����ʼ
		{
			return m_pTrackerCfg->iStartRoadNum + GetRoadNo();
		}
		else		//���ҿ�ʼ
		{
			return m_pTrackerCfg->iStartRoadNum + (m_pTrackerCfg->nRoadLineNumber-1-1 - GetRoadNo());
		}
	}
	else
	{
		return GetRoadNo();
	}
}

HRESULT CSWCarLeft::GetEventDetInfo(CSWString& strInfo)
{
	// todo.
	// ��Щ���ٶȵ��޸��볬�ٵ�Υ���ж�
	// ��DONE�ӿ����ظ��Ĳ�����

	// ����Υ�µ��жϣ�Ŀǰֻ����������ٵĳ��١�����豸�ķ�ʽ��֧�֡�
	// ����Υ���ڴ��ж��д˲�̫���ʡ�todo.
	BOOL fIsOverSpeed = FALSE;
	if ( NULL != m_pTrackerCfg && GetCarspeed() >= 0.1f && ((int)GetCarspeed() > m_pTrackerCfg->iSpeedLimit))
	{
		fIsOverSpeed = true;
	}
	if (fIsOverSpeed)
	{
		INT iPtType = GetPTType();
		iPtType |= PT_OVERSPEED;
		SetPTType(iPtType);
	}

	// ʶ����Ԥ����
	// ��������,�������0�ĳ��١�
	// ������Υ�µ��ж��С�
	if (NULL != m_pTrackerCfg && m_pTrackerCfg->cScaleSpeed.fEnable && GetCarspeed() == 0 )
	{
		swpa_utils_srand(CSWDateTime::GetSystemTick());
		FLOAT fltCarspeed = swpa_utils_rand() % 20 + 10.0f;
		SetCarspeed(fltCarspeed);
	}


	return S_OK;  
}

CSWString CSWCarLeft::BuildNormalString()
{
	CSWString strInfo = "";

	if( NULL != m_pTrackerCfg )
	{
		CSWString strTmp;
		//ƽ�����Ŷ�
		//strTmp.Format("ƽ�����Ŷ�:%.3f\n", swpa_exp(swpa_log(GetAverageConfidence()) * 0.143));
		//strInfo.Append(strTmp);
		//���ַ����Ŷ�
		//strTmp.Format("���ֿ��Ŷ�:%.3f\n", GetFirstAverageConfidence());
		//strInfo.Append(strTmp);

		//����������б�ʶ
		if(m_pTrackerCfg->nDetReverseRunEnable)
		{
			strTmp.Format("����������ʻ:%s\n", GetReverseRun()? "��" : "��");
			strInfo.Append(strTmp);
		}

		//�������
		//strTmp.Format("����:%d\n", GetRoadNo() + (255 == GetRoadNo() ? 0 : m_pTrackerCfg->iStartRoadNum));
		strTmp.Format("����:%d\n",GetOutPutRoadNo());
		strInfo.Append(strTmp);

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
		if(0 < GetCarspeed())
		{ 
			//����
			strTmp.Format("��Ƶ����:%d Km/h\n", (int)GetCarspeed());
			strInfo.Append(strTmp);
		}

		//˫��������
		/*switch(GetPlateType())
		{
		case PLATE_DOUBLE_YELLOW: strTmp = "��������:˫"; break;
		case PLATE_DOUBLE_MOTO  : strTmp = "��������:Ħ"; break;
		default : strTmp = ""; break;
		}
		if(!strTmp.IsEmpty())
		{
			strInfo.Append(strTmp);
		}*/

		//Υ������
		//strTmp.Format("Υ������:%d\n", GetPTType());
		//strInfo.Append(strTmp);
	}
	
	return strInfo;
}

HRESULT CSWCarLeft::BuildPlateString(TiXmlDocument& xmlDoc)
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
  TiXmlText* pText = new TiXmlText((LPCSTR)strPlate);
  if (pValue && pText)
  {
      pValue->LinkEndChild(pText);
      pResult->LinkEndChild(pValue);
  }
  
  //������ɫ
  /*pValue = new TiXmlElement("Color");
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
  pValue = new TiXmlElement("Type");
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
      		strTmp.Format("%s", GetReverseRun() ? "��" : "��");
          pValue->SetAttribute("value", (LPCSTR)strTmp);
          pValue->SetAttribute("chnname", "����������ʻ");
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

    // ���˵�����
    {
        pValue = new TiXmlElement("ResultType");
        if (pValue)
        {
            CSWImage* pImage = GetImage(1);
            if (pImage)
            {
                strTmp.Format("<�����Դ��%s  ��ͼ��Դ��%s>",
                              m_fCaptureRecogResult ? "ץ��ʶ��" : "��Ƶʶ��" ,
                              pImage->IsCaptureImage() ? "ץ�Ĵ�ͼ"  : "��Ƶ��ͼ");

                pValue->SetAttribute("value", (LPCSTR)strTmp);
                pValue->SetAttribute("chnname", "���˵��");
                pResult->LinkEndChild(pValue);
            }
        }
    }
    
    //����ٶ�
    if(0 < GetCarspeed())
    { //����
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
          pValue->SetAttribute("chnname", "����ֵ");
          pResult->LinkEndChild(pValue);
      }
//      //��������������
//      pValue = new TiXmlElement("ScaleSpeedOfDistance");
//      if (pValue)
//      {
//          strTmp.Format("%0.2f", GetScaleOfDistance());
//          pValue->SetAttribute("value", (LPCSTR)strTmp);
//          pValue->SetAttribute("chnname", "�������������");
//          pResult->LinkEndChild(pValue);
//      }
    }
    
    //��Ч֡��
    pValue = new TiXmlElement("ObservedFrames");
    if (pValue)
    {
        strTmp.Format("%d", GetObservedFrames());
        pValue->SetAttribute("value", (LPCSTR)strTmp);
        pValue->SetAttribute("chnname", "��Ч֡��");
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
    }

    //���ַ����Ŷ�
    pValue = new TiXmlElement("FirstCharConf");
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
        pValue->SetAttribute("chnname", "�������ʱ��");
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
    if(m_pTrackerCfg->fOutputCarSize && GetCarType() != CT_WALKMAN && GetCarType() != CT_BIKE && GetPlateType() != PLATE_DOUBLE_MOTO)
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
    }
    
    //��������
    pValue = new TiXmlElement("CarType");
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
    }
    
//    //������ɫ
//    if (m_pTrackerCfg->fEnableRecgCarColor)
//    {
//        pValue = new TiXmlElement("CarColor");
//        if (pValue)
//        {
//            switch (GetCarColor())
//    			  {
//    			  case CC_WHITE:  strTmp = "��ɫ"; break;
//    			  case CC_GREY :  strTmp = "��ɫ"; break;
//    			  case CC_BLACK:  strTmp = "��ɫ"; break;
//    			  case CC_RED:    strTmp = "��ɫ"; break;
//    			  case CC_YELLOW: strTmp = "��ɫ"; break;
//    			  case CC_GREEN:  strTmp = "��ɫ"; break;
//    			  case CC_BLUE:   strTmp = "��ɫ"; break;
//    			  case CC_PURPLE: strTmp = "��ɫ"; break;
//    			  case CC_PINK:   strTmp = "��ɫ"; break;
//    			  case CC_BROWN:  strTmp = "��ɫ"; break;
//    			  default:        strTmp = "δ֪"; break;
//    			  }
//            pValue->SetAttribute("value", (LPCSTR)strTmp);
//            pValue->SetAttribute("chnname", "������ɫ");
//            pResult->LinkEndChild(pValue);
//        }
//    }
    
    //������
    pValue = new TiXmlElement("RoadNumber");
    if (pValue)
    {
        //strTmp.Format("%d", GetRoadNo() + (255 == GetRoadNo() ? 0 : m_pTrackerCfg->iStartRoadNum));
		strTmp.Format("%d", GetOutPutRoadNo());
		pValue->SetAttribute("value", (LPCSTR)strTmp);
        pValue->SetAttribute("chnname", "����");
        pResult->LinkEndChild(pValue);
    }
    
    //��ʼ������
    pValue = new TiXmlElement("BeginRoadNumber");
    if (pValue)
    {
        if (m_pTrackerCfg->iRoadNumberBegin == 0)
        {
            strTmp.Format("<��,%d>", m_pTrackerCfg->iStartRoadNum);
        }
        else
        {
            strTmp.Format("<��,%d>", m_pTrackerCfg->iStartRoadNum);
        }
        pValue->SetAttribute("value", (LPCSTR)strTmp);
        pValue->SetAttribute("chnname", "��ʼ������");
        pResult->LinkEndChild(pValue);
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
  }
  
  //�¼������Ϣ
  strTmp = "";
  if (S_OK == GetEventDetInfo(strTmp) && !strTmp.IsEmpty())
  {
  	pValue = new TiXmlElement("EventCheck");
  	if (pValue)
  	{
  	    pValue->SetAttribute("value", (LPCSTR)strTmp);
  	    pValue->SetAttribute("chnname", "�¼����");
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

//  //��ǰƫ�⾵״̬
//  pValue = new TiXmlElement("CplStatus");
//  if (pValue)
//  {
//	  strTmp.Format("%d", GetCplStatus());
//	  pValue->SetAttribute("value", (LPCSTR)strTmp);
//	  pValue->SetAttribute("chnname", "ƫ�⾵״̬");
//	  pResult->LinkEndChild(pValue);
//  }
//    pValue = new TiXmlElement("WdrLevel");
//    if (pValue)
//    {
//        strTmp.Format("%d", GetWdrLevel());
//        pValue->SetAttribute("value", (LPCSTR)strTmp);
//        pValue->SetAttribute("chnname", "WDR�ȼ�");
//        pResult->LinkEndChild(pValue);
//    }
//  //��ǰ�����������
//  // todo.
//  // �����²�ʹ��Ƶ���ƣ��������������ȼ���
//  if( m_pTrackerCfg != NULL && m_pTrackerCfg->iUsedLight == 1 )
//  {
//	  pValue = new TiXmlElement("PulseLevel");
//	  if (pValue)
//	  {
//		  strTmp.Format("%d", GetPulseLevel());
//		  pValue->SetAttribute("value", (LPCSTR)strTmp);
//		  pValue->SetAttribute("chnname", "��������ȼ�");
//		  pResult->LinkEndChild(pValue);
//	  }
//  }

  // ����ͼ��������
  /*pValue = new TiXmlElement("LastPlatePos");
  if (pValue)
  {
      HV_RECT rectLast = GetLastPlateRect();
      strTmp.Format("left = %d, top = %d, right = %d, bottom = %d",
                    rectLast.left, rectLast.top, rectLast.right, rectLast.bottom);
      pValue->SetAttribute("value", (LPCSTR)strTmp);
      pValue->SetAttribute("chnname", "����ͼ��������");
      pResult->LinkEndChild(pValue);
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
      pValue->SetAttribute("chnname", "��������");
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
  }

  //���ƶԱȶ�
  pValue = new TiXmlElement("PlateVariance");
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
		  pValue->SetAttribute("chnname", "������Ϣ");
		  pResult->LinkEndChild(pValue);
	  } 
  }

  return S_OK;
}

TRACKER_CFG_PARAM* CSWCarLeft::GetTrackerParam()
{
	return m_pTrackerCfg;
}

CSWString CSWCarLeft::BuildPlateString(VOID)
{
	TiXmlDocument xmlDoc;
	if(S_OK == BuildPlateString(xmlDoc))
	{
		TiXmlPrinter cTxPr;
		xmlDoc.Accept(&cTxPr);
		return cTxPr.CStr();
	}
	return "";
}

INT CSWCarLeft::GetImageCount(void)
{
	return 7;
}

CSWImage* CSWCarLeft::GetImage(INT index, SW_RECT* rc)
{
	CSWImage *pImage[] = 
	{
		  (CSWImage*)m_pCarLeft->cCoreResult.cResultImg.pimgBestSnapShot
		, (CSWImage*)m_pCarLeft->cCoreResult.cResultImg.pimgLastSnapShot
		, (CSWImage*)m_pCarLeft->cCoreResult.cResultImg.pimgBeginCapture
		, (CSWImage*)m_pCarLeft->cCoreResult.cResultImg.pimgBestCapture
		, (CSWImage*)m_pCarLeft->cCoreResult.cResultImg.pimgLastCapture
		, (CSWImage*)m_pCarLeft->cCoreResult.cResultImg.pimgPlate
		, (CSWImage*)m_pCarLeft->cCoreResult.cResultImg.pimgPlateBin
	};
	SW_RECT *imgRC[] =
	{
		  (SW_RECT *)&m_pCarLeft->cCoreResult.rcBestPlatePos
		, (SW_RECT *)&m_pCarLeft->cCoreResult.rcLastPlatePos
		, (SW_RECT *)&m_pCarLeft->cCoreResult.rcFirstPos
		, (SW_RECT *)&m_pCarLeft->cCoreResult.rcSecondPos
		, (SW_RECT *)&m_pCarLeft->cCoreResult.rcThirdPos
		, NULL
		, NULL
	};
	for(int i = 0; i < GetImageCount(); i++)
	{
		if(i == index)
		{
			if(NULL != rc)
			{
				if(NULL != imgRC[index])
				{
					//*rc = *imgRC[index];
					swpa_memcpy(rc, imgRC[index], sizeof(SW_RECT));
				}
				else
				{
					swpa_memset(rc, 0, sizeof(SW_RECT));
				}
			}
			return pImage[i];
		}
	}
    return NULL;
}

void CSWCarLeft::GetResultRect(INT index, SW_RECT *rc)
{
    SW_RECT *imgRC[] =
    {
          (SW_RECT *)&m_pCarLeft->cCoreResult.rcBestPlatePos
        , (SW_RECT *)&m_pCarLeft->cCoreResult.rcLastPlatePos
        , (SW_RECT *)&m_pCarLeft->cCoreResult.rcFirstPos
        , (SW_RECT *)&m_pCarLeft->cCoreResult.rcSecondPos
        , (SW_RECT *)&m_pCarLeft->cCoreResult.rcThirdPos
        , NULL
        , NULL
    };

    for(int i = 0; i < GetImageCount(); i++)
    {
        if(i == index)
        {
            if(NULL != rc)
            {
                if(NULL != imgRC[index])
                {
                    //*rc = *imgRC[index];
                    swpa_memcpy(rc, imgRC[index], sizeof(SW_RECT));
                }
                else
                {
                    swpa_memset(rc, 0, sizeof(SW_RECT));
                }
            }
            return ;
        }
    }
}

VOID CSWCarLeft::SetImage(INT index, CSWImage *pImg, SW_RECT* rc)
{
	//CHECK_LOCK() //marked by zhouy@2014/02/12: no need to lock at image setting.
	CSWImage **pImage[] = 
	{
		  (CSWImage**)&m_pCarLeft->cCoreResult.cResultImg.pimgBestSnapShot
		, (CSWImage**)&m_pCarLeft->cCoreResult.cResultImg.pimgLastSnapShot
		, (CSWImage**)&m_pCarLeft->cCoreResult.cResultImg.pimgBeginCapture
		, (CSWImage**)&m_pCarLeft->cCoreResult.cResultImg.pimgBestCapture
		, (CSWImage**)&m_pCarLeft->cCoreResult.cResultImg.pimgLastCapture
		, (CSWImage**)&m_pCarLeft->cCoreResult.cResultImg.pimgPlate
		, (CSWImage**)&m_pCarLeft->cCoreResult.cResultImg.pimgPlateBin
	};
	SW_RECT *imgRC[] =
	{
		  (SW_RECT *)&m_pCarLeft->cCoreResult.rcBestPlatePos
		, (SW_RECT *)&m_pCarLeft->cCoreResult.rcLastPlatePos
		, (SW_RECT *)&m_pCarLeft->cCoreResult.rcFirstPos
		, (SW_RECT *)&m_pCarLeft->cCoreResult.rcSecondPos
		, (SW_RECT *)&m_pCarLeft->cCoreResult.rcThirdPos
		, NULL
		, NULL
	};
    for(int i = 0; i < sizeof(pImage)/sizeof(CSWImage *); i++)
	{
		if(i == index)
		{
			// ����ͬһͼ����쳣����
			if( *pImage[i] != pImg )
			{
				SAFE_RELEASE(*pImage[i]);
				*pImage[i] = pImg;
				SAFE_ADDREF(pImg);
			}
			if(NULL != rc && NULL != imgRC[index])
			{
				swpa_memcpy(imgRC[index], rc, sizeof(SW_RECT));
			}
		}
    }
}

void CSWCarLeft::SetResultRect(INT index, SW_RECT *rc)
{
    SW_RECT *imgRC[] =
    {
          (SW_RECT *)&m_pCarLeft->cCoreResult.rcBestPlatePos
        , (SW_RECT *)&m_pCarLeft->cCoreResult.rcLastPlatePos
        , (SW_RECT *)&m_pCarLeft->cCoreResult.rcFirstPos
        , (SW_RECT *)&m_pCarLeft->cCoreResult.rcSecondPos
        , (SW_RECT *)&m_pCarLeft->cCoreResult.rcThirdPos
        , NULL
        , NULL
    };

    for(int i = 0; i < GetImageCount(); i++)
    {
        if(i == index)
        {
            if(NULL != rc && NULL != imgRC[index])
            {
                swpa_memcpy(imgRC[index], rc, sizeof(SW_RECT));
            }
            return ;
        }
    }
}

CSWString CSWCarLeft::GetPlateNo(VOID)
{	
	if(m_strPlateNo.IsEmpty())
	{
		CHAR szPlateName[255] = "";
		GetPlateNameAlpha(szPlateName, (PLATE_TYPE)GetPlateType(), (PLATE_COLOR)GetPlateColor(), GetContent());
		m_strPlateNo = szPlateName;
	}
	return m_strPlateNo;	
}

HRESULT CSWCarLeft::SetPlateNo(LPCSTR szPlateNo)
{
	if(!m_fReadOnly)
	{
		m_strPlateNo = szPlateNo;
		return S_OK;
	}
	return E_FAIL;
}

HRESULT CSWCarLeft::GetCarSize(INT &iOutType, FLOAT &fltWidth, FLOAT &fltHeight)
{
	iOutType = m_pCarLeft->cCoreResult.cCarSize.nOutType;
	fltWidth = m_pCarLeft->cCoreResult.cCarSize.iCarWidth;
	fltHeight= m_pCarLeft->cCoreResult.cCarSize.iCarHeight;
	return S_OK;
}

SW_RECT CSWCarLeft::GetFaceRect(INT index)
{
	SW_RECT rc = {0, 0, 0, 0};
	return index < m_pCarLeft->cCoreResult.nFaceCount ? *(SW_RECT *)&m_pCarLeft->cCoreResult.rgFaceRect[index] : rc;
}

VOID CSWCarLeft::SetFaceRect(CSWCarLeft *pCarLeft)
{
	for(int i = 0; i < pCarLeft->m_pCarLeft->cCoreResult.nFaceCount; i++)
	{
		SW_TRACE_DEBUG("face(%d,%d,%d,%d)", pCarLeft->m_pCarLeft->cCoreResult.rgFaceRect[i].left, pCarLeft->m_pCarLeft->cCoreResult.rgFaceRect[i].top, pCarLeft->m_pCarLeft->cCoreResult.rgFaceRect[i].right, pCarLeft->m_pCarLeft->cCoreResult.rgFaceRect[i].bottom);
	}
	m_pCarLeft->cCoreResult.nFaceCount = pCarLeft->m_pCarLeft->cCoreResult.nFaceCount;
	swpa_memcpy(m_pCarLeft->cCoreResult.rgFaceRect, pCarLeft->m_pCarLeft->cCoreResult.rgFaceRect, pCarLeft->m_pCarLeft->cCoreResult.nFaceCount*sizeof(HV_RECT));
}

void CSWCarLeft::Done(void)
{
	// ʶ����Ԥ����
	// ��������,�������0�ĳ��١�
	if (NULL != m_pTrackerCfg && m_pTrackerCfg->cScaleSpeed.fEnable && GetCarspeed() == 0 )
	{
		swpa_utils_srand(CSWDateTime::GetSystemTick());
		FLOAT fltCarspeed = swpa_utils_rand() % 20 + 20.0f;
		SetCarspeed(fltCarspeed);
	}
	// ����Υ�µ��жϣ�Ŀǰֻ����������ٵĳ��١�����豸�ķ�ʽ��֧�֡�
	// ����Υ���ڴ��ж��д˲�̫���ʡ�todo.
	BOOL fIsOverSpeed = FALSE;
	if ( NULL != m_pTrackerCfg && GetCarspeed() >= 0.1f && ((int)GetCarspeed() > m_pTrackerCfg->iSpeedLimit))
	{
		fIsOverSpeed = true;
	}
	if (fIsOverSpeed)
	{
		INT iPtType = GetPTType();
		iPtType |= PT_OVERSPEED;
		SetPTType(iPtType);
	}
	// �������Ƴ��ĳ�����ɫ��
	if( GetPlateNo().Find("�޳���") != -1 && GetCarType() != CT_WALKMAN && GetCarType() != CT_BIKE)
	{
		SetCarColor(CC_GREY);
	}
	// ����������
	if( GetCarType() == CT_UNKNOWN )
	{
		SetCarType(CT_SMALL);
	}

	// zhaopy �޳��ƲŽ��д����滻��
	if( GetPlateNo().Find("�޳���") != -1 )
	{
	  if(GetCarType() == CT_WALKMAN)
	  {
		  SetPlateNo("  ����");
		  SetCarColor(CC_UNKNOWN);
	  }
	  //todo,Ϊ��Ҫ����жϣ����Ƿǻ���������Ħ�г�����
	  else if(GetCarType() == CT_BIKE && GetPlateType() != PLATE_DOUBLE_MOTO)
	  {
		  SetPlateNo("  �ǻ�����");
		  SetCarColor(CC_UNKNOWN);
	  }
	}

	// �滻����ȼ���ز�������ʵʱ��һ�¡�
	INT rgiInfo[3] = {0};
	if( m_pTrackerCfg != NULL && S_OK == CSWMessage::SendMessage(MSG_AUTO_CONTROL_GET_REALPARAM,0,(LPARAM)rgiInfo) )
	{
		SetLightType(rgiInfo[0]);
		if( m_pTrackerCfg->nCtrlCpl == 1 )
		{
			SetCplStatus(rgiInfo[1]);
		}
		else
		{
			SetCplStatus(-1);
		}
		SetPulseLevel(rgiInfo[2]);
	}	
	
	//todo,�ڴ˺����ڲ�Ҳ�����޸�Υ�����ͣ�����Ҫ������֮ǰ���ô˺���
	CSWString strEvent;
	GetEventDetInfo(strEvent);
	
	m_fReadOnly = TRUE;
}

