#include "stdafx.h"
#include "Misc_SmartCommon.h"

CCommon::CCommon(void)
{

}

CCommon::~CCommon(void)
{

}

CString CCommon::GetTrueString(CString strFormat, ReplaceInfo& RpInfo)
{
	CString strResult = strFormat;

	CTime tmTime(RpInfo.dwTimeMs / 1000);
	int iSSTime = (int)(RpInfo.dwTimeMs % 1000);

	CTime tmRedTime(RpInfo.dwRedLightStartTimeMs / 1000);
	int iSSRedTime =  (int)(RpInfo.dwRedLightStartTimeMs % 1000);

	strResult.Replace(_T("$(DeviceNo)"), RpInfo.strDeviceNo);
	strResult.Replace(_T("$(DeviceIP)"), RpInfo.strDeviceIP);
	strResult.Replace(_T("$(StreetName)"), RpInfo.strStreetName);
	strResult.Replace(_T("$(StreetDirection)"), RpInfo.strStreetDirection);
	strResult.Replace(_T("$(StreetNameID)"), RpInfo.strStreetNameID);
	strResult.Replace(_T("$(StreetDirectionID)"), RpInfo.strStreetDirectionID);

	strResult.Replace(_T("$(Year)"), tmTime.Format(_T("%Y")));
	strResult.Replace(_T("$(Month)"), tmTime.Format(_T("%m")));
	strResult.Replace(_T("$(Day)"), tmTime.Format(_T("%d")));
	strResult.Replace(_T("$(Hour)"), tmTime.Format(_T("%H")));
	strResult.Replace(_T("$(Minute)"), tmTime.Format(_T("%M")));
	strResult.Replace(_T("$(Second)"), tmTime.Format(_T("%S")));
	CString strTemp;
	strTemp.Format(_T("%03d"), iSSTime);
	strResult.Replace(_T("$(MilliSecond)"), strTemp);

	strResult.Replace(_T("$(Date)"), tmTime.Format(_T("%Y-%m-%d")));
	strTemp.Format(_T("%s.%03d"), tmTime.Format(_T("%H:%M:%S")), iSSTime);
	strResult.Replace(_T("$(Time)"), strTemp);

	strResult.Replace(_T("$(PlateNo)"), RpInfo.strPlateNo);
	strResult.Replace(_T("$(PlateType)"), RpInfo.strPlateType);
	strResult.Replace(_T("$(PlateTypeCode)"), RpInfo.strPlateTypeCode);
	strResult.Replace(_T("$(PlateColor)"), RpInfo.strPlateColor);
	strResult.Replace(_T("$(PlateColorCode)"), RpInfo.strPlateColorCode);

	strResult.Replace(_T("$(IllegalInfo)"), RpInfo.strIllegalInfo);
	strTemp.Format(_T("%d"), RpInfo.iIllegalType);
	strResult.Replace(_T("$(IllegalType)"), strTemp);
	strResult.Replace(_T("$(IllegalCode)"), RpInfo.strIllegalCode);

	strTemp.Format(_T("%d"), RpInfo.iSpeed);
	strResult.Replace(_T("$(Speed)"), strTemp);
	strTemp.Format(_T("%d"), RpInfo.iLimitSpeed);
	strResult.Replace(_T("$(LimitSpeed)"), strTemp);

	strTemp.Format(_T("%0.2f"), RpInfo.fltOverSpeedRate);
	strResult.Replace(_T("$(OverSpeedRate)"), strTemp);

	strResult.Replace(_T("$(CarType)"), RpInfo.strCarType);
	strResult.Replace(_T("$(CarTypeCode)"), RpInfo.strCarTypeCode);
	strResult.Replace(_T("$(CarColor)"), RpInfo.strCarColor);
	strResult.Replace(_T("$(CarColorCode)"), RpInfo.strCarColorCode);
	strResult.Replace(_T("$(BigImageID_Letter)"), RpInfo.strBigImageID);

	ReplaceNum(strResult, _T("CarNo"), RpInfo.dwCarIndex);
	ReplaceNum(strResult, _T("BigImageID"), RpInfo.iBigImageID);
	ReplaceNum(strResult, _T("RoadNum"), RpInfo.iRoadNumber);

	strTemp.Format(_T("%d"), RpInfo.iBigImageCount);
	strResult.Replace(_T("$(BigImageCount)"), strTemp);

	strResult.Replace(_T("$(NewLine)"), _T("\r\n"));

	return strResult;
}

void CCommon::ReplaceNum(CString& strFormat, CString strArg, int iVal)
{
    int iIndex = -1;
    int iIndexEnd = -1;
	CString strTemp=_T("$(")+strArg;
	iIndex=strFormat.Find(strTemp);

	if (iIndex < 0)
        return;

    while (iIndex >= 0)
    {
        iIndexEnd = strFormat.Find(_T(")"), iIndex);

        if (iIndexEnd < 0)
            return;

        CString strReplace = strFormat.Mid(iIndex, iIndexEnd - iIndex + 1);

        if (iVal < 0)
        {
            strFormat.Replace(strReplace, NULL);
            strTemp = _T("$") + strArg;
            iIndex = strFormat.Find(strTemp);
            continue;
        }
        if ((iIndexEnd - iIndex) == (strArg.GetLength() + 3) )
        {
            strTemp.Format(_T("%u"), iVal);
            strFormat.Replace(strReplace, strTemp);
        }
        else
        {
            CString strFmt;
            CString strNum;
            int iStart = iIndex + strArg.GetLength() + 3;
            int iCount = iIndexEnd - iStart;
            strTemp = strFormat.Mid(iStart, iCount);

            strFmt.Format(_T("%%%su"), strTemp);
            strNum.Format(strFmt, iVal);

            strFormat.Replace(strReplace, strNum);
        }
        strTemp = _T("$(") + strArg;
        iIndex = strFormat.Find(strTemp);
    }

}

BOOL CCommon::MakeSurePathExists(CString strPath)
{
    if (PathFileExists(strPath))
        return TRUE;

    return MakeSureDirectoryPathExists(strPath);
}

int CCommon::IStreamToByteArray(IStream* pStreamIn, BYTE* pImgBuff, int& iLen)
{
    if (pStreamIn == NULL || pImgBuff == NULL || iLen == 0)
    {
        iLen = 0;
        return iLen;
    }
    LARGE_INTEGER liTemp = {0};
    pStreamIn->Seek(liTemp, STREAM_SEEK_SET, NULL);
    ULONG ulSize = 0;
    if (pStreamIn->Read(pImgBuff, iLen, &ulSize) == S_OK)
    {
        iLen = ulSize;
    }
    else
    {
        iLen = 0;
    }
    return iLen;
}

int CCommon::ByteArrayToIStream(BYTE* pImgBuff, int iLen, IStream* pStreamOut)
{
    int iRetLen = 0;
    if (pImgBuff == NULL || iLen == 0 || pStreamOut == NULL)
        return iRetLen;

    LARGE_INTEGER liTemp = {0};
    ULARGE_INTEGER uliZero = {0};
    pStreamOut->Seek(liTemp, STREAM_SEEK_SET, NULL);
    pStreamOut->SetSize(uliZero);
    ULONG ulRealSize = 0;
    if (pStreamOut->Write(pImgBuff, iLen, &ulRealSize) == S_OK)
        iRetLen = ulRealSize;

    return iRetLen;
}

BOOL CCommon::GetStreamLength(IStream* pStream, ULARGE_INTEGER* puliLenth)
{
    if (pStream == NULL)
        return FALSE;

    LARGE_INTEGER liMov;
    liMov.QuadPart = 0;

    ULARGE_INTEGER uliEnd, uliBegin;

    HRESULT hr = S_FALSE;

    hr = pStream->Seek(liMov, STREAM_SEEK_END, &uliEnd);
    if (FAILED(hr))
        return FALSE;

    hr = pStream->Seek(liMov, STREAM_SEEK_SET, &uliBegin);
    if (FAILED(hr))
        return FALSE;

    // ��ֵ�������ĳ���
    puliLenth->QuadPart = uliEnd.QuadPart - uliBegin.QuadPart;

    return TRUE;

}

CString CCommon::GetExeDir(void)
{
	CString strPath;

	TCHAR szFileName[ MAX_PATH ] = {0};
	GetModuleFileName( NULL, szFileName, MAX_PATH );	//ȡ�ð�����������ȫ·��
	PathRemoveFileSpec( szFileName );				//ȥ��������

	strPath = szFileName;
	strPath += "\\";
	return strPath;
}

int CCommon::Random(int iStart, int iEnd)
{
	srand((unsigned int)time(NULL));
	int a = rand() % (iEnd - iStart) + iStart;

	return a;
}

CString CCommon::GetStringExample(CString strFormat)
{
	CString strExampleText = strFormat; // ʾ��

	// ʾ���滻
	strExampleText.Replace(_T("$(Year)"), "2014");
	strExampleText.Replace(_T("$(Month)"), "02");
	strExampleText.Replace(_T("$(Day)"), "14");
	strExampleText.Replace(_T("$(Hour)"), "15");
	strExampleText.Replace(_T("$(Minute)"), "36");
	strExampleText.Replace(_T("$(Second)"), "27");
	strExampleText.Replace(_T("$(MilliSecond)"), "698");

	strExampleText.Replace(_T("$(DeviceNo)"), "8888");
	strExampleText.Replace(_T("$(DeviceIP)"), "172.18.70.12");
	strExampleText.Replace(_T("$(StreetName)"), "����·");
	strExampleText.Replace(_T("$(StreetDirection)"), "������");
	strExampleText.Replace(_T("$(StreetNameID)"), "01");
	strExampleText.Replace(_T("$(StreetDirectionID)"), "03");

	strExampleText.Replace(_T("$(PlateNo)"), "����ANB110");
	strExampleText.Replace(_T("$(PlateType)"), "��ͨ");
	strExampleText.Replace(_T("$(PlateTypeCode)"), "putong");
	strExampleText.Replace(_T("$(PlateColor)"), "��");
	strExampleText.Replace(_T("$(PlateColorCode)"), "lan");

	strExampleText.Replace(_T("$(IllegalInfo)"), "�����");
	strExampleText.Replace(_T("$(IllegalType)"), "3");
	strExampleText.Replace(_T("$(IllegalCode)"), "6213");

	strExampleText.Replace(_T("$(Speed)"), "100");
	strExampleText.Replace(_T("$(LimitSpeed)"), "120");
	strExampleText.Replace(_T("$(OverSpeedRate)"), "4%");

	strExampleText.Replace(_T("$(CarType)"), "С�ͳ�");
	strExampleText.Replace(_T("$(CarTypeCode)"), "xiao");
	strExampleText.Replace(_T("$(CarColor)"), "��");
	strExampleText.Replace(_T("$(CarColorCode)"), "huang");

	strExampleText.Replace(_T("$(CarNo)"), "1234");
	strExampleText.Replace(_T("$(BigImageID, 01)"), "1");
	strExampleText.Replace(_T("$(BigImageID_Letter)"), "a");
	strExampleText.Replace(_T("$(RoadNum)"), "02");
	strExampleText.Replace(_T("$(BigImageCount)"), "3");

	return strExampleText;
}

CString CCommon::GetStringInfo(CString strFormat)
{
	CString strInfoText = strFormat; // ˵��

	// ˵���滻
	strInfoText.Replace(_T("$(Year)"), "��");
	strInfoText.Replace(_T("$(Month)"), "��");
	strInfoText.Replace(_T("$(Day)"), "��");
	strInfoText.Replace(_T("$(Hour)"), "ʱ");
	strInfoText.Replace(_T("$(Minute)"), "��");
	strInfoText.Replace(_T("$(Second)"), "��");
	strInfoText.Replace(_T("$(MilliSecond)"), "����");

	strInfoText.Replace(_T("$(DeviceNo)"), "�豸���");
	strInfoText.Replace(_T("$(DeviceIP)"), "�豸IP");
	strInfoText.Replace(_T("$(StreetName)"), "·������");
	strInfoText.Replace(_T("$(StreetDirection)"), "·�ڷ���");
	strInfoText.Replace(_T("$(StreetNameID)"), "·������ID");
	strInfoText.Replace(_T("$(StreetDirectionID)"), "·�ڷ���ID");

	strInfoText.Replace(_T("$(PlateNo)"), "���ƺ�");
	strInfoText.Replace(_T("$(PlateType)"), "��������");
	strInfoText.Replace(_T("$(PlateTypeCode)"), "�������ͱ���");
	strInfoText.Replace(_T("$(PlateColor)"), "������ɫ");
	strInfoText.Replace(_T("$(PlateColorCode)"), "������ɫ����");

	strInfoText.Replace(_T("$(IllegalInfo)"), "Υ����Ϣ");
	strInfoText.Replace(_T("$(IllegalType)"), "Υ������");
	strInfoText.Replace(_T("$(IllegalCode)"), "Υ������");

	strInfoText.Replace(_T("$(Speed)"), "����");
	strInfoText.Replace(_T("$(LimitSpeed)"), "����");
	strInfoText.Replace(_T("$(OverSpeedRate)"), "���ٰٷֱ�");

	strInfoText.Replace(_T("$(CarType)"), "��������");
	strInfoText.Replace(_T("$(CarTypeCode)"), "�������ʹ���");
	strInfoText.Replace(_T("$(CarColor)"), "������ɫ");
	strInfoText.Replace(_T("$(CarColorCode)"), "������ɫ����");

	strInfoText.Replace(_T("$(CarNo)"), "����ID");
	strInfoText.Replace(_T("$(BigImageID, 01)"), "ͼƬID");
	strInfoText.Replace(_T("$(BigImageID_Letter)"), "ͼƬ��ĸID");
	strInfoText.Replace(_T("$(RoadNum)"), "������");
	strInfoText.Replace(_T("$(BigImageCount)"), "ͼƬ����");

	return strInfoText;
}

// ȡͼƬ����GLSID
int CCommon::GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
{
	UINT  num = 0;
	UINT  size = 0;

	ImageCodecInfo* pImageCodecInfo = NULL;

	GetImageEncodersSize(&num, &size);
	if(size == 0)
		return -1;

	pImageCodecInfo = (ImageCodecInfo*)(malloc(size));
	if(pImageCodecInfo == NULL)
		return -1;

	GetImageEncoders(num, size, pImageCodecInfo);

	for(UINT j = 0; j < num; ++j)
	{
		if( wcscmp(pImageCodecInfo[j].MimeType, format) == 0 )
		{
			*pClsid = pImageCodecInfo[j].Clsid;
			free(pImageCodecInfo);
			return j;
		}    
	}
	free(pImageCodecInfo);
	return -1;
}

// ·�����
int CCommon::IsIllegalDir(CString strDir)
{
	if (strDir.IsEmpty())
		return 1;

	if (-1 != strDir.Find("\\\\"))
		return 1;

	// ���Υ���ַ�
	char chPathNameIllChar[8] = { '/', '*', '?', '"', '<', '>', '|', ':'};
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < strDir.GetLength(); j++)
		{
			if (chPathNameIllChar[i] == strDir.GetAt(j))
			{
				return  1;
			}
		}
	}
	return 0;
}

// �ļ������
int CCommon::IsIllegalFileName(CString strDir)
{
	if (strDir.IsEmpty())
		return 1;

	// ���Υ���ַ�
	char chPathNameIllChar[9] = { '/', '*', '?', '"', '<', '>', '|', ':', '\\'};
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < strDir.GetLength(); j++)
		{
			if (chPathNameIllChar[i] == strDir.GetAt(j))
			{
				return  1;
			}
		}
	}
	return 0;
}

//time_t FileTimeToTime_t(const FILETIME &ft)  
//{  
//    ULARGE_INTEGER ui;  
//    ui.LowPart = ft.dwLowDateTime;  
//    ui.HighPart = ft.dwHighDateTime;  
//    return ((LONGLONG)(ui.QuadPart - 116444736000000000) / 10000000);  
//}  
//
//int CCommon::GetDiffSeconds(const SYSTEMTIME &t1, const SYSTEMTIME &t2)
//{
//    FILETIME fTime1 = { 0, 0 };
//    FILETIME fTime2 = { 0, 0 };  
//    SystemTimeToFileTime(&t1, &fTime1);  
//    SystemTimeToFileTime(&t2, &fTime2);
//  
//    time_t tt1 = FileTimeToTime_t(fTime1);  
//    time_t tt2 = FileTimeToTime_t(fTime2);  
//  
//    return (int)(tt2 - tt1);
//}
