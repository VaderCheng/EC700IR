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
      strEventCheck += "����";
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
      strEventCheck += "����";

      INT iPtType = GetPTType();
      iPtType |= PT_OVERSPEED;
      SetPTType(iPtType);
  }

  if (!strEventCheck.IsEmpty())
  {
  		strInfo += "Υ��:��<";
  		strInfo += strEventCheck;
  		strInfo += ">";
  }
	return S_OK;
}

HRESULT CSWTrafficGateCarLeft::BuildPlateString(TiXmlDocument& xmlDoc)
{
	if(FAILED(CSWCarLeft::BuildPlateString(xmlDoc)))
	{
		return E_FAIL;
	}
	return S_OK;
}

