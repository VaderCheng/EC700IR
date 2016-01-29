// ���ƣ�Common
// ���ܣ�һ���ۺ���
// 1. �ַ��滻--------------GetTrueString
		// 1). ֧�ֵ�ͨ�����
				// $(DeviceNo)				�豸���
				// $(DeviceIP)				�豸   IP
				// $(StreetName)			·������
				// $(StreetDirection)		·�ڷ���
				// $(StreetNameID)		·������ID
				// $(StreetDirectionID)	·�ڷ���ID

				// $(Year)				��
				// $(Month)			��
				// $(Day)				��
				// $(Hour)				ʱ
				// $(Minute)			��
				// $(Second)			��
				// $(MilliSecond)	����

				// $(Date)				���ڣ���ʽ��2013-12-10
				// $(Time)				ʱ�䣬��ʽ��13:46:50.207

				// $(PlateNo)			����
				// $(PlateType)		��������
				// $(PlateColor)		������ɫ
				// $(PlateColorCode)		������ɫ����

				// $(IllegalInfo)		Υ����Ϣ
				// $(IllegalType)		Υ������
				// $(IllegalCode)	Υ������

				// $(Speed)			����
				// $(LimitSpeed)	����
				// $(OverSpeedRate)		���ٰٷֱ�

				// $(CarType)		��������
				// $(CarTypeCode)		�������ʹ���

				// $(CarColor)		������ɫ
				// $(CarColorCode)		������ɫ����

				// $(CarNo, 08)     ����ID��8Ϊλ����0Ϊ��λǰ�ַ�

				// $(BigImageID, 01)        ͼƬID��1Ϊλ����0Ϊ��λǰ�ַ�
				
				// $(BigImageID_Letter)     ͼƬ��ĸID

				// $(RoadNum)		������

				// $(BigImageCount)     ͼƬ����

				// $(NewLine)		�س�����

// 2. ���Ĳ���:
		// 1). ͼƬ��ת����--------------IStreamToByteArray
		// 2). ����תͼƬ��--------------ByteArrayToIStream
		// 3). ��ȡ���Ĵ�С--------------GetStreamLength
		// 4). ��ȡ��ǰ����·��--------------GetCurrentDir


#pragma once

#include <Shlwapi.h>
#pragma  comment(lib, "Shlwapi.lib")
#include <Dbghelp.h>
#pragma comment(lib, "Dbghelp.lib")

typedef struct _ReplaceInfo
{
	CString strDeviceNo;            //�豸���
	CString strDeviceIP;              //�豸IP

	CString strStreetName;     //·������
	CString strStreetDirection;           //·�ڷ���
	CString strStreetNameID;           //·������ID
	CString strStreetDirectionID;        //·�ڷ���ID

	DWORD64 dwTimeMs;       //ʱ��

	CString strPlateNo;             //���ƺ���
	CString strPlateType;           //��������
	CString strPlateTypeCode;			//�������ͱ��
	CString strPlateColor;          //������ɫ
	CString strPlateColorCode;       //������ɫ����


	int iRoadNumber;                //������

	CString strIllegalInfo;            //Υ����Ϣ
	int iIllegalType;                   //Υ������
	CString strIllegalCode;         //Υ������

	int iSpeed;                     //����
	int iLimitSpeed;            //����
	float fltOverSpeedRate;       //���ٰٷֱ�

	DWORD64 dwRedLightStartTimeMs;      // ��ƿ�ʼʱ��

	CString strCarType; 		    // ��������
	CString strCarTypeCode;         //�������ʹ���

	CString strCarColor;            //������ɫ
	CString strCarColorCode;            //������ɫ����

	DWORD32 dwCarIndex;			//����ID

	int iBigImageCount;				//��ͼ����

	int iBigImageID;			//��ͼID
	CString strBigImageID; //��ͼID��ĸ

	_ReplaceInfo()
	{
		strDeviceNo = strDeviceIP = strStreetName = strStreetDirection = strStreetNameID = strStreetDirectionID = strPlateNo = \
		strPlateType = strPlateTypeCode = strPlateColor = strPlateColorCode = strIllegalInfo = strIllegalCode = strCarType = \
		strCarTypeCode = strCarColor = strCarColorCode = _T("Unknown");

		dwTimeMs = 0;
		iRoadNumber = -1;
		iIllegalType = -1;

		iSpeed = -1;
		iLimitSpeed = -1;
		fltOverSpeedRate = 0.0;

		dwRedLightStartTimeMs = 0;

		dwCarIndex = -1;

		iBigImageCount = -1;
		iBigImageID = -1;
	}

}ReplaceInfo;


class CCommon
{
public:
	CCommon(void);
	~CCommon(void);

public:
	// �����ַ��滻
	CString GetTrueString(CString strFormat, ReplaceInfo& RpInfo);

	// ��ȡ�ַ����滻��ʾ��
	CString GetStringExample(CString strFormat);
	// ��ȡ�ַ����滻��˵��
	CString GetStringInfo(CString strFormat);

	// ��֤·������
	BOOL MakeSurePathExists(CString strPath);

	// ͼƬ��ת����
	int IStreamToByteArray(IStream* pStreamIn, BYTE* pImgBuff, int& iLen);

	// ����д����
	int ByteArrayToIStream(BYTE* pImgBuff, int iLen, IStream* pStreamOut);

	// ��ȡһ�����Ĵ�С
	BOOL GetStreamLength(IStream* pStream, ULARGE_INTEGER* pulLenth);

	// ��ȡexe���ڵ�·������һ���ǽ��̵ĵ�ǰ·����
	CString GetExeDir(void);

	// ����һ��[iStart,iEnd)�����ڵ������
	int Random(int iStart, int iEnd);

	// ȡͼƬ����GLSID
	int GetEncoderClsid(const WCHAR* format, CLSID* pClsid);

	// ·�����
	int IsIllegalDir(CString strDir);
	
	// �ļ������
	int IsIllegalFileName(CString strDir);

	// �������ʱ����������
	int GetDiffSeconds(const SYSTEMTIME &t1, const SYSTEMTIME &t2);

private:
    void ReplaceNum(CString& strFormat, CString strArg, int iVal);

};
