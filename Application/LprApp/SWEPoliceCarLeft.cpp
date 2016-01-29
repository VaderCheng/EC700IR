#include "SWFC.h"
#include "SWLPRApplication.h"
#include "SWEPoliceCarLeft.h"

CSWEPoliceCarLeft::CSWEPoliceCarLeft()
{
}

CSWEPoliceCarLeft::~CSWEPoliceCarLeft()
{
}

HRESULT CSWEPoliceCarLeft::GetEventDetInfo(CSWString& strInfo)
{	
	if(FAILED(CSWCarLeft::GetEventDetInfo(strInfo)))
	{
		return E_FAIL;
	}
	if (theApp->GetParam().Get().cTrackerCfgParam.nOutputRoadTypeName && GetRoadNo() >=0 && GetRoadNo() < MAX_ROADLINE_NUM - 1)
    {
#define STRING_ROAD_TYPE "ֱ��;��ת;ֱ��,��ת;��ת;ֱ��,��ת;��ת,��ת;ֱ��,��ת,��ת;��ͷ;ֱ��,��ͷ;��ת,��ͷ;ֱ��,��ת,��ͷ;��ת,��ͷ;ֱ��,��ת,��ͷ;��ת,��ת,��ͷ;ֱ��,��ת,��ת,��ͷ;�ǻ�������;"
        CSWString strRoadType = theApp->GetParam().Get().cTrackerCfgParam.rgcRoadInfo[GetRoadNo()].szRoadTypeName;
        CSWString szTmp = strRoadType;
        szTmp.ToUpper();
        if (strRoadType.IsEmpty() || szTmp == (CSWString)"NULL")
        {
            CSWString strTmp = STRING_ROAD_TYPE;
            int i = 0, j = 0;
            for (int pos = 0; j >= 0 && pos < theApp->GetParam().Get().cTrackerCfgParam.rgcRoadInfo[GetRoadNo()].iRoadType;)
            {
                j = strTmp.Find(";", i);
                pos++;
                if (pos < theApp->GetParam().Get().cTrackerCfgParam.rgcRoadInfo[GetRoadNo()].iRoadType)
                    i = j + 1;
            }
            if (j < 0)
                j = strTmp.Length();
            szTmp = strTmp.Substr(i, j - i);
            if (szTmp.IsEmpty())
                szTmp = "δ֪";
            strRoadType = CSWString("��������:") + szTmp;
        }
        else
            strRoadType = CSWString("��������:") + strRoadType;

        strInfo += strRoadType;
        strInfo += "\n";
    }
	//��ʻ����
	switch (GetRunType())
  {
  case RT_FORWARD:  strInfo += "��ʻ����:ֱ��\n"; break;
  case RT_LEFT:     strInfo += "��ʻ����:��ת\n"; break;
  case RT_RIGHT:    strInfo += "��ʻ����:��ת\n"; break;
  case RT_TURN:     strInfo += "��ʻ����:��ͷ\n"; break;
  case RT_CONVERSE: strInfo += "��ʻ����:����\n"; break;
  default:          strInfo += "��ʻ����:δ֪\n"; break;
  }

  
  if(GetPTType() != PT_NORMAL)
  {
  	if(theApp->GetParam().Get().cResultSenderParam.iFilterUnSurePeccancy && GetUnSurePeccancy())
  	{
  		strInfo += "Υ��:��";
		// ����ǲ��ϸ�Υ�£�ȷ�����ͼƬ����
		if( GetOutputCaptureImage() != 1 )
		{
			if( s_iOutputBestImage == 0 )
			{
				SetImage(2, NULL);
			}
			if( s_iOutputLastImage == 0 )
			{
				SetImage(3, NULL);
			}
			SetImage(4, NULL);
		}
  	}
  	else
  	{
  		strInfo += "Υ��:��";
  	}
  	//������Ҫ����Υ�����ȼ����ж�
    if (!theApp->GetParam().Get().cResultSenderParam.nOutputPeccancyType)
    {
        CSWString str = theApp->GetParam().Get().cResultSenderParam.szPeccancyPriority;
        if (str[str.Length()-1] != '-')
            str += "-";
    
        int i = 0, j = 0;
        while (i != -1 && j != -1)
        {
            j = str.Find("-", i);
            if (j != -1)
            {
                CSWString s = str.Substr(i, j-i);
                if (s == (CSWString)"����"
                        && (GetPTType() & PT_CONVERSE) == PT_CONVERSE)
                {
                    SetPTType(PT_CONVERSE);
                    break;
                }
                if (s == (CSWString)"�ǻ�������"
                        && (GetPTType() & PT_ESTOP) == PT_ESTOP)
                {
                    SetPTType(PT_ESTOP);
                    break;
                }
                if (s == (CSWString)"�����"
                        && (GetPTType() & PT_RUSH) == PT_RUSH)
                {
                    SetPTType(PT_RUSH);
                    break;
                }
                if (s == (CSWString)"����������ʻ"
                        && (GetPTType() & PT_ACROSS) == PT_ACROSS)
                {
                    SetPTType(PT_ACROSS);
                    break;
                }
                if (s == (CSWString)"ѹ��"
                        && (GetPTType() & PT_OVERLINE) == PT_OVERLINE)
                {
                    SetPTType(PT_OVERLINE);
                    break;
                }
                if (s == (CSWString)"����"
                        && (GetPTType() & PT_OVERSPEED) == PT_OVERSPEED)
                {
                    SetPTType(PT_OVERSPEED);
                    break;
                }
                i = j+1;
            }
        }
    }
  	//�����޸�,2010-12-08
    BOOL fFound = FALSE;
    BOOL fFirst = TRUE;
    CSWString strMid = "";
    if (!fFound && (GetPTType() & PT_CONVERSE) == PT_CONVERSE)
    {
        strMid += "����";
        if (!theApp->GetParam().Get().cResultSenderParam.nOutputPeccancyType)
            fFound = TRUE;
    }
    if (!fFound && (GetPTType() & PT_ESTOP) == PT_ESTOP)
    {
    		if(!strMid.IsEmpty())
    		{
    			strMid += ";";
    		}	
        strMid += "�ǻ�������";
        if (!theApp->GetParam().Get().cResultSenderParam.nOutputPeccancyType)
            fFound = TRUE;
    }
    if (!fFound && (GetPTType() & PT_RUSH) == PT_RUSH)
    {
    		if(!strMid.IsEmpty())
    		{
    			strMid += ";";
    		}
        strMid += "�����";
        if (!theApp->GetParam().Get().cResultSenderParam.nOutputPeccancyType)
            fFound = TRUE;
    }
    if (!fFound && (GetPTType() & PT_ACROSS) == PT_ACROSS)
    {
    		if(!strMid.IsEmpty())
    		{
    			strMid += ";";
    		}
        strMid += "����������ʻ";
        if (!theApp->GetParam().Get().cResultSenderParam.nOutputPeccancyType)
            fFound = TRUE;
    }
    if (!fFound && (GetPTType() & PT_OVERLINE) == PT_OVERLINE)
    {
    		if(!strMid.IsEmpty())
    		{
    			strMid += ";";
    		}
        strMid += "ѹ��";
        if (!theApp->GetParam().Get().cResultSenderParam.nOutputPeccancyType)
            fFound = TRUE;
    }
    if (!fFound && (GetPTType() & PT_OVERSPEED) == PT_OVERSPEED)
    {
    		if(!strMid.IsEmpty())
    		{
    			strMid += ";";
    		}
        strMid += "����";
        if (!theApp->GetParam().Get().cResultSenderParam.nOutputPeccancyType)
            fFound = TRUE;
    }
    if (!strMid.IsEmpty())
    {
        //ȥ�����һ���ֺ�
        strInfo += "<";
        strInfo += strMid;
        strInfo += ">\n";
    }
  }
  else
  {
  	strInfo += "Υ��:��\n";
  }
  
  //ѹ��,�����ߴ����ұ�Ŵ�0��ʼ
  if (GetDetectOverYellowLineEnable() && GetIsOverYellowLine() != COLT_INVALID)
  {
      if (GetIsOverYellowLine() == COLT_NO)
      {
          strInfo += "ѹ��:��\n";
      }
      else
      {
          CSWString strYellowLine = "";
          strYellowLine.Format("ѹ��:������%d<%s>\n", GetIsOverYellowLine() + m_pTrackerCfg->iStartRoadNum, GetIsDoubleYellowLine() ? "����" : "ʵ��");
          strInfo += strYellowLine;
      }
  }
  //Խ��,�����ߴ����ұ�Ŵ�0��ʼ
  if (GetDetectCrossLineEnable() && GetIsCrossLine() != COLT_INVALID)
  {
      if (GetIsCrossLine() == COLT_NO)
      {
          strInfo += "Խ��:��\n";
      }
      else
      {
          CSWString strCrossLine = "";
          strCrossLine.Format("Խ��:������%d\n", GetIsCrossLine() + m_pTrackerCfg->iStartRoadNum);
          strInfo += strCrossLine;
      }
  }
  strInfo += "ֹͣ��ǰ:";
  switch (GetOnePosScene().lsForward)
  {
  case TLS_UNSURE:strInfo += "ֱ??;"; break;
  case TLS_RED:   strInfo += "ֱ��;"; break;
  case TLS_YELLOW:strInfo += "ֱ��;"; break;
  case TLS_GREEN: strInfo += "ֱ��;"; break;
  }

  switch (GetOnePosScene().lsLeft)
  {
  case TLS_UNSURE:strInfo += "��??;"; break;
  case TLS_RED:   strInfo += "���;"; break;
  case TLS_YELLOW:strInfo += "���;"; break;
  case TLS_GREEN: strInfo += "����;"; break;
  }

  switch (GetOnePosScene().lsRight)
  {
  case TLS_UNSURE:strInfo += "��??;"; break;
  case TLS_RED:   strInfo += "�Һ�;"; break;
  case TLS_YELLOW:strInfo += "�һ�;"; break;
  case TLS_GREEN: strInfo += "����;"; break;
  }

  switch (GetOnePosScene().lsTurn)
  {
  case TLS_UNSURE:strInfo += "��??"; break;
  case TLS_RED   :strInfo += "����"; break;
  case TLS_YELLOW:strInfo += "����"; break;
  case TLS_GREEN :strInfo += "����"; break;
  }

  strInfo += "\nֹͣ�ߺ�:";
  switch (GetTwoPosScene().lsForward)
  {
  case TLS_UNSURE:strInfo += "ֱ??;"; break;
  case TLS_RED:   strInfo += "ֱ��;"; break;
  case TLS_YELLOW:strInfo += "ֱ��;"; break;
  case TLS_GREEN: strInfo += "ֱ��;"; break;
  }

  switch (GetTwoPosScene().lsLeft)
  {
  case TLS_UNSURE:strInfo += "��??;"; break;
  case TLS_RED:   strInfo += "���;"; break;
  case TLS_YELLOW:strInfo += "���;"; break;
  case TLS_GREEN: strInfo += "����;"; break;
  }

  switch (GetTwoPosScene().lsRight)
  {
  case TLS_UNSURE:strInfo += "��??;"; break;
  case TLS_RED:   strInfo += "�Һ�;"; break;
  case TLS_YELLOW:strInfo += "�һ�;"; break;
  case TLS_GREEN: strInfo += "����;"; break;
  }

  switch (GetTwoPosScene().lsTurn)
  {
  case TLS_UNSURE:strInfo +="��??"; break;
  case TLS_RED:   strInfo +="����"; break;
  case TLS_YELLOW:strInfo +="����"; break;
  case TLS_GREEN: strInfo +="����"; break;
  }

  //��ƿ�ʼʱ��
  CSWString str;
  
  //��ת���
  if ( GetRedStart().dwLeftL != 0 )
  {
  		CSWDateTime dt(GetRedStart().dwLeftL);
      str.Format("\n��ת���ʱ��:%02d:%02d:%02d:%03d", dt.GetHour(), dt.GetMinute(), dt.GetSecond(), dt.GetMSSecond());
      strInfo += str;
  }
  
  //ֱ�к��
  if ( GetRedStart().dwForwardL != 0 )
  {
      CSWDateTime dt(GetRedStart().dwForwardL);
      str.Format("\nֱ�к��ʱ��:%02d:%02d:%02d:%03d", dt.GetHour(), dt.GetMinute(), dt.GetSecond(), dt.GetMSSecond());
      strInfo += str;
  }
  
  //��ת���
  if ( GetRedStart().dwRightL != 0 )
  {
      CSWDateTime dt(GetRedStart().dwRightL);
      str.Format("\n��ת���ʱ��:%02d:%02d:%02d:%03d", dt.GetHour(), dt.GetMinute(), dt.GetSecond(), dt.GetMSSecond());
      strInfo += str;
  }
  
  //��ͷ���
  if ( GetRedStart().dwTurnL != 0 )
  {
      CSWDateTime dt(GetRedStart().dwTurnL);
      str.Format("\n��ͷ���ʱ��:%02d:%02d:%02d:%03d", dt.GetHour(), dt.GetMinute(), dt.GetSecond(), dt.GetMSSecond());
      strInfo += str;
  }
  
  //��Ӻ������
  if (GetRedLightCount() > 0 && GetRedLightCount() < 20 && GetAvgY() < theApp->GetParam().Get().cResultSenderParam.nRedPosBrightness)
  {
      strInfo += "\nRedLight:";
      for (int i = 0; i < GetRedLightCount(); ++i)
      {
          str.Format("(%04d,%04d,%04d,%04d)", GetRedLightPos()[i].left, GetRedLightPos()[i].top, GetRedLightPos()[i].right, GetRedLightPos()[i].bottom);
          strInfo += str;
      }
  }
	return S_OK;
}


void CSWEPoliceCarLeft::Done(void)
{
	CSWCarLeft::Done();
	//ͼƬ�������
	//  �����Υ�½��������ǿ��������Ź켣ͼ��ǰ�����Ŵ�ͼ�������
	//  �����Υ�������õ����Ŵ�ͼ�������ʽ��������羯Ŀǰ��DSP�˵�һ���ڶ��Ŵ�ͼ�õ��Ǻ͵����������Ŵ�ͼһ�¡�
	if( s_iOutputCaptureImage == 1 || m_pCarLeft->cCoreResult.ptType != PT_NORMAL )
	{
		SetImage(0, NULL);
		SetImage(1, NULL);
	}
	else
	{
		// ���������ͼ�����ͼ�ı�־Ϊ��������ͼ�Ĵ�ͼ��־����ǰ������ͼ�Ĵ�ͼ��־��Ŀǰ�羯����ˡ�
		if( GetImage(2) != NULL && GetImage(3) != NULL && GetImage(4) != NULL )
		{
			SetImage(0, NULL);
			SetImage(1, NULL);
			if( s_iOutputBestImage == 0 )
			{
				SetImage(2, NULL);
			}
			if( s_iOutputLastImage == 0 )
			{
				SetImage(3, NULL);
			}
			SetImage(4, NULL);
		}
		else
		{
			SetImage(2, NULL);
			SetImage(3, NULL);
			SetImage(4, NULL);
			if( s_iOutputBestImage == 0 )
			{
				SetImage(0, NULL);
			}
			if( s_iOutputLastImage == 0 )
			{
				SetImage(1, NULL);
			}
		}
		
	}	


}

