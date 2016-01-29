/*
 * CSWLPRJupiterDomeCameraParameter.cpp
 *
 *  Created on: 2013��12��27��
 *      Author: qinjj
 */
#include "SWFC.h"
#include "SWLPRApplication.h"
#include "SWLPRJupiterDomeCameraParameter.h"



CSWLPRJupiterDomeCameraParameter::CSWLPRJupiterDomeCameraParameter()
{
	ResetParam();
}

CSWLPRJupiterDomeCameraParameter::~CSWLPRJupiterDomeCameraParameter()
{
}

void CSWLPRJupiterDomeCameraParameter::ResetParam(VOID)
{
	//�ȵ��û���
	CSWLPRParameter::ResetParam();

	/*���ݾ��巽�����ò���*/
	
	Get().nWorkModeIndex = PRM_DOME_CAMERA;
	m_strWorkMode.Format("Υͣץ��");
	Get().nCameraWorkMode = 0;		//todo: ����Ӧ���Ƕ���?
	Get().nMCFWUsecase = DOME_CAMERA;

 
	//�ȳ�ʼ��ɨ������ÿ��ģʽ�����Լ���ɨ������
	Get().cTrackerCfgParam.cDetectArea.DetectorAreaLeft = 20;
	Get().cTrackerCfgParam.cDetectArea.DetectorAreaTop = 20;
	Get().cTrackerCfgParam.cDetectArea.DetectorAreaRight = 80;
	Get().cTrackerCfgParam.cDetectArea.DetectorAreaBottom = 90;
	Get().cTrackerCfgParam.cDetectArea.nDetectorMinScaleNum = 11;
	Get().cTrackerCfgParam.cDetectArea.nDetectorMaxScaleNum = 18;

	Get().cTrackerCfgParam.cVideoDet.nVideoDetMode = 2;

	//��ʼ����������
	Get().cTrackerCfgParam.cTrapArea.TopLeftX = Get().cTrackerCfgParam.cDetectArea.DetectorAreaLeft;
	Get().cTrackerCfgParam.cTrapArea.TopRightX = Get().cTrackerCfgParam.cDetectArea.DetectorAreaRight;
	Get().cTrackerCfgParam.cTrapArea.BottomLeftX = Get().cTrackerCfgParam.cDetectArea.DetectorAreaLeft;
	Get().cTrackerCfgParam.cTrapArea.BottomRightX = Get().cTrackerCfgParam.cDetectArea.DetectorAreaRight;

	Get().cTrackerCfgParam.cTrapArea.TopLeftY = Get().cTrackerCfgParam.cDetectArea.DetectorAreaTop;
	Get().cTrackerCfgParam.cTrapArea.TopRightY = Get().cTrackerCfgParam.cDetectArea.DetectorAreaTop;
	Get().cTrackerCfgParam.cTrapArea.BottomLeftY = Get().cTrackerCfgParam.cDetectArea.DetectorAreaBottom;
	Get().cTrackerCfgParam.cTrapArea.BottomRightY = Get().cTrackerCfgParam.cDetectArea.DetectorAreaBottom;

	Get().cTrackerCfgParam.cTrapArea.nDetectorMinScaleNum = Get().cTrackerCfgParam.cDetectArea.nDetectorMinScaleNum;
	Get().cTrackerCfgParam.cTrapArea.nDetectorMaxScaleNum = Get().cTrackerCfgParam.cDetectArea.nDetectorMaxScaleNum;

	Get().cCamCfgParam.iMaxExposureTime = 3800;
	Get().cCamCfgParam.iMinExposureTime = 0;
	Get().cCamCfgParam.iMinAGCLimit = 50;
	Get().cCamCfgParam.iMaxAGCLimit = 140;
	Get().cCamAppParam.iAGCGainHOri = 100;
	Get().cCamCfgParam.iNightShutterThreshold = 3000;
	Get().cCamCfgParam.iNightAvgYThreshold = 100;
	
	//����Ĭ�ϵ���Ƶ��������ϵ��
	Get().cTrackerCfgParam.cScaleSpeed.fltAdjustCoef = 0.96;

	Get().cDevParam[1].iCommType = 1;	//����ֻ��һ������COM1,ֻ֧��485

	Get().cCamAppParam.iResolution = 0;	//Ĭ��Ϊ1080P
	
	// ץ��λ�ó�ʼֵ
	Get().cTrackerCfgParam.nCaptureOnePos = 50;
	Get().cTrackerCfgParam.nCaptureTwoPos = 80;

	// ������Ƶ�������
	Get().cTrackerCfgParam.iUsedLight = 0;

	//����ɨ������Ĭ�ϴ�
	Get().cTrackerCfgParam.cTrapArea.fEnableDetAreaCtrl = TRUE;

	//����RGB�������÷�ΧΪ36~255,Ĭ��ֵ����Ϊ0
	Get().cCamAppParam.iGainR = 36;
	Get().cCamAppParam.iGainG = 36;
	Get().cCamAppParam.iGainB = 36;

	// WDR
	Get().cCamAppParam.iWDRValue = 2;

		//��ʼ��ͣ�������ز���
	swpa_memset(&Get().cMatchParam.cPeccancyParkingParam, 0, 
				sizeof(PECCANCY_PARKING_PARAM));

}


HRESULT CSWLPRJupiterDomeCameraParameter::Initialize(CSWString strFilePath)
{
	// ��ʼ��ǰ��������Ĭ��ֵ
	ResetParam();
	
    // �Ȼ���ĳ�ʼ��
    if (S_OK == CSWParameter<ModuleParams>::Initialize(strFilePath)
		&& S_OK == InitSystem()
		&& S_OK == InitTracker()
		&& S_OK == InitHvDsp()
        && S_OK == InitOuterCtrl()
        && S_OK == InitIPTCtrl()
        && S_OK == InitCamApp()
        && S_OK == InitCamera()
        && S_OK == InitCharacter()
        && S_OK == InitGB28181()
//        && S_OK == InitAutoReboot()
        && S_OK == InitONVIF())
    {
        return S_OK ;
    }

    SW_TRACE_DEBUG("<CSWLPRJupiterDomeCameraParameter> Initialize failed.\n");
    return E_FAIL ;
}

HRESULT CSWLPRJupiterDomeCameraParameter::InitSystem(VOID)
{
    if (S_OK == CSWLPRParameter::InitSystem())
    {
		
        return S_OK ;
    }
    return E_FAIL ;

}

HRESULT CSWLPRJupiterDomeCameraParameter::InitTracker(VOID)
{


	return S_OK;
}

HRESULT CSWLPRJupiterDomeCameraParameter::InitHvDsp(VOID)
{

	GetEnum("HvDsp\\Misc"
    	, "IPNCLogOutput"
    	, &Get().cCamCfgParam.iIPNCLogOutput
    	, Get().cCamCfgParam.iIPNCLogOutput
    	, "0:�����;1:���"
    	, "���IPNC��־��Ϣ"
    	, ""
    	, PROJECT_LEVEL
    );
	
   /* GetEnum("HvDsp\\Misc"
            , "OutputFilterInfo"
            , &Get().cResultSenderParam.fOutputFilterInfo
            , Get().cResultSenderParam.fOutputFilterInfo
            , "0:�����;1:���"
            , "���������Ϣ"
            , ""
            , CUSTOM_LEVEL
           );
           */

    // ��������.
    Get().cResultSenderParam.cProcRule.fOutPutFilterInfo = Get().cResultSenderParam.fOutputFilterInfo;

    GetEnum("\\HvDsp\\Misc"
            , "DrawRect"
            , &Get().cResultSenderParam.iDrawRect
            , Get().cResultSenderParam.iDrawRect
            , "0:������;1:����"
            , "����ͼ�񻭺��"
            , ""
            , PROJECT_LEVEL
           );

	Get().cResultSenderParam.fSaveVideo = FALSE;

	Get().cResultSenderParam.fIsSafeSaver = FALSE;

	Get().cResultSenderParam.fInitHdd = FALSE;
	Get().cResultSenderParam.iFileSize = 512;
	Get().cResultSenderParam.iSaveSafeType = 0;
	Get().cResultSenderParam.iDiskType = 0;

	/*
    GetString("\\HvDsp\\Misc"
            , "NetDiskIP"
            , Get().cResultSenderParam.szNetDiskIP
            , Get().cResultSenderParam.szNetDiskIP
            , sizeof(Get().cResultSenderParam.szNetDiskIP)
            , "����洢��IP��ַ"
            , ""
            , CUSTOM_LEVEL
            );

    GetInt("\\HvDsp\\Misc"
            , "NetDiskSpace"
            , &Get().cResultSenderParam.iNetDiskSpace
            , Get().cResultSenderParam.iNetDiskSpace
            , 1
            , 3696
            , "����洢������,��λG (�洢��ʼ��ʱ������)"
            , ""
            , CUSTOM_LEVEL
          );

    GetString("\\HvDsp\\Misc"
            , "NFS"
            , Get().cResultSenderParam.szNFS
            , Get().cResultSenderParam.szNFS
            , sizeof(Get().cResultSenderParam.szNFS)
            , "NFS·��"
            , ""
            , CUSTOM_LEVEL
            );

    GetString("\\HvDsp\\Misc"
            , "NFSParam"
            , Get().cResultSenderParam.szNFSParam
            , Get().cResultSenderParam.szNFSParam
            , sizeof(Get().cResultSenderParam.szNFSParam)
            , "NFS���ز���"
            , ""
            , PROJECT_LEVEL
            );
	*/

    GetEnum("\\HvDsp\\Misc"
            , "EnableNTP"
            , &Get().cResultSenderParam.fEnableNtp
            , Get().cResultSenderParam.fEnableNtp
            , "0:��ʹ��;1:ʹ��"
            , "NTPʹ�ܿ���"
            , ""
            , CUSTOM_LEVEL
           );


    GetInt("\\HvDsp\\Misc"
            , "NTPSyncInterval"
            , &Get().cResultSenderParam.iNtpSyncInterval
            , Get().cResultSenderParam.iNtpSyncInterval
            , 300
            , 2147483647
            , "NTPʱ��ͬ�����(��λ��)"
            , ""
            , CUSTOM_LEVEL
          );


    GetString("\\HvDsp\\Misc"
            , "NTPServerIP"
            , Get().cResultSenderParam.szNtpServerIP
            , Get().cResultSenderParam.szNtpServerIP
            , sizeof(Get().cResultSenderParam.szNtpServerIP)
            , "NTP������IP��ַ"
            , ""
            , CUSTOM_LEVEL
            );

	GetEnum("\\HvDsp\\Misc"
		, "TimeZone"
		, &Get().cResultSenderParam.iTimeZone
		, Get().cResultSenderParam.iTimeZone
		, "0:GMT-12;"
		  	"1:GMT-11;"
			"2:GMT-10;"
			"3:GMT-09;"
			"4:GMT-08;"
			"5:GMT-07;"
			"6:GMT-06;"
			"7:GMT-05;"
			"8:GMT-04;"
			"9:GMT-03;"
			"10:GMT-02;"
			"11:GMT-01;"
			"12:GMT+00;"
			"13:GMT+01;"
			"14:GMT+02;"
			"15:GMT+03;"
			"16:GMT+04;"
			"17:GMT+05;"
			"18:GMT+06;"
			"19:GMT+07;"
			"20:GMT+08;"
			"21:GMT+09;"
			"22:GMT+10;"
			"23:GMT+11;"
			"24:GMT+12"
		, "ʱ��"
		, ""
		, CUSTOM_LEVEL
	);

    GetString("\\HvDsp\\FilterRule"
            , "Compaty"
            , Get().cResultSenderParam.cProcRule.szCompatyRule
            , Get().cResultSenderParam.cProcRule.szCompatyRule
            , sizeof(Get().cResultSenderParam.cProcRule.szCompatyRule)
            , "ͨ�������"
            , ""
            , PROJECT_LEVEL
            );

    GetString("\\HvDsp\\FilterRule"
            , "Replace"
            , Get().cResultSenderParam.cProcRule.szReplaceRule
            , Get().cResultSenderParam.cProcRule.szReplaceRule
            , sizeof(Get().cResultSenderParam.cProcRule.szReplaceRule)
            , "�滻����"
            , ""
            , PROJECT_LEVEL
            );

    GetString("\\HvDsp\\FilterRule"
            , "Leach"
            , Get().cResultSenderParam.cProcRule.szLeachRule
            , Get().cResultSenderParam.cProcRule.szLeachRule
            , sizeof(Get().cResultSenderParam.cProcRule.szLeachRule)
            , "���˹���"
            , ""
            , PROJECT_LEVEL
            );

    GetEnum("\\HvDsp\\Misc"
            , "LoopReplaceEnable"
            , &Get().cResultSenderParam.cProcRule.fLoopReplace
            , Get().cResultSenderParam.cProcRule.fLoopReplace
            , "0:������;1:����"
            , "ѭ���滻"
            , ""
            , PROJECT_LEVEL
           );

    GetString("\\HvDsp\\Identify", "StreetName"
            , Get().cResultSenderParam.szStreetName
            , Get().cResultSenderParam.szStreetName
            , sizeof(Get().cResultSenderParam.szStreetName)
            , "·������"
            , ""
            , CUSTOM_LEVEL
            );

	/*
    GetString("\\HvDsp\\Identify"
            , "StreetDirection"
            , Get().cResultSenderParam.szStreetDirection
            , Get().cResultSenderParam.szStreetDirection
            , sizeof(Get().cResultSenderParam.szStreetDirection)
            , "·�ڷ���"
            , ""
            , CUSTOM_LEVEL
            );


    GetEnum("\\HvDsp\\Misc"
            , "OutputOnlyPeccancy"
            , &Get().cResultSenderParam.iOutputOnlyPeccancy
            , Get().cResultSenderParam.iOutputOnlyPeccancy
            , "0:ȫ�����;1:ֻ���Υ�½��;2:ֻ�����Υ�½��"
            , "��������ʽ"
            , ""
            , CUSTOM_LEVEL
           );

    GetInt("\\HvDsp\\Misc"
            , "SendRecordSpace"
            , &Get().cResultSenderParam.iSendRecordSpace
            , Get().cResultSenderParam.iSendRecordSpace
            , 2
            , 100
            , "������ͼ��(*100ms)"
            , ""
            , CUSTOM_LEVEL
          );

    GetInt("\\HvDsp\\Misc"
            , "SendHisVideoSpace"
            , &Get().cResultSenderParam.iSendHisVideoSpace
            , Get().cResultSenderParam.iSendHisVideoSpace
            , 2
            , 20
            , "��ʷ¼���ͼ��(*100ms)"
            , ""
            , CUSTOM_LEVEL
          );


    GetEnum("\\HvDsp\\VideoProc\\MainVideo"
            , "OutputBestSnap"
            , &Get().cResultSenderParam.iBestSnapshotOutput
            , Get().cResultSenderParam.iBestSnapshotOutput
            , "0:�����;1:���"
            , "����Ƶ��������ͼ���"
            , ""
            , CUSTOM_LEVEL
           );

    GetEnum("\\HvDsp\\VideoProc\\MainVideo"
            , "OutputLastSnap"
            , &Get().cResultSenderParam.iLastSnapshotOutput
            , Get().cResultSenderParam.iLastSnapshotOutput
            , "0:�����;1:���"
            , "����Ƶ����ͼ���"
            , ""
            , CUSTOM_LEVEL
           );


    GetEnum("\\HvDsp\\Misc"
            , "AutoLinkEnable"
            , &Get().cResultSenderParam.cAutoLinkParam.fAutoLinkEnable
            , Get().cResultSenderParam.cAutoLinkParam.fAutoLinkEnable
            , "0:��ʹ��;1:ʹ��"
            , "��������ʹ��"
            , ""
            , CUSTOM_LEVEL
           );

    GetString("\\HvDsp\\Misc"
            , "AutoLinkIP"
            , Get().cResultSenderParam.cAutoLinkParam.szAutoLinkIP
            , Get().cResultSenderParam.cAutoLinkParam.szAutoLinkIP
            , sizeof(Get().cResultSenderParam.cAutoLinkParam.szAutoLinkIP)
            , "�������ӷ�����IP"
            , ""
            , CUSTOM_LEVEL
            );

    GetInt("\\HvDsp\\Misc"
            , "AutoLinkPort"
            , &Get().cResultSenderParam.cAutoLinkParam.iAutoLinkPort
            , Get().cResultSenderParam.cAutoLinkParam.iAutoLinkPort
            , 0
            , 10000
            , "�������ӷ������˿�"
            , ""
            , CUSTOM_LEVEL
          );
*/

	GetEnum("\\HvDsp\\Misc"
		, "FtpEnable"
		, &Get().cResultSenderParam.cFtpParam.fFtpEnable
		, Get().cResultSenderParam.cFtpParam.fFtpEnable
		, "0:��ʹ��;1:ʹ��"
		, "FTPʹ��"
		, ""
		, CUSTOM_LEVEL
	);
	
	GetString("\\HvDsp\\Misc"
			, "FtpIP"
			, Get().cResultSenderParam.cFtpParam.szFtpIP
			, Get().cResultSenderParam.cFtpParam.szFtpIP
			, sizeof(Get().cResultSenderParam.cFtpParam.szFtpIP)
			, "FTP������IP"
			, ""
			, CUSTOM_LEVEL
	);
	GetString("\\HvDsp\\Misc"
			, "FtpUserName"
			, Get().cResultSenderParam.cFtpParam.szUserName
			, Get().cResultSenderParam.cFtpParam.szUserName
			, sizeof(Get().cResultSenderParam.cFtpParam.szUserName)
			, "FTP�û���"
			, ""
			, CUSTOM_LEVEL
	);
	GetString("\\HvDsp\\Misc"
			, "FtpPassWd"
			, Get().cResultSenderParam.cFtpParam.szPassWord
			, Get().cResultSenderParam.cFtpParam.szPassWord
			, sizeof(Get().cResultSenderParam.cFtpParam.szPassWord)
			, "FTP����"
			, ""
			, CUSTOM_LEVEL
	);
	
	GetInt("HvDsp\\Misc"
		, "FTPTimeOut"
		, &Get().cResultSenderParam.cFtpParam.iTimeOut
		, 5
		, 0
		, 10
		, "FTP���ӳ�ʱʱ��(��λ��)"
		, ""
		, CUSTOM_LEVEL
	);

	/* //�����֧��
    GetInt("HvDsp\\EventChecker"
            , "CheckEventTime"
            , &Get().cResultSenderParam.iCheckEventTime
            , Get().cResultSenderParam.iCheckEventTime
            , 0
            , 5
            , "�¼��������(��λ:����)"
            , ""
            , PROJECT_LEVEL
          );

    Get().cTrackerCfgParam.nDetReverseRunEnable = FALSE;
	Get().cTrackerCfgParam.nSpan = FALSE;
	

    Get().cTrackerCfgParam.fltOverLineSensitivity = 2.5;

	Get().cResultSenderParam.iSpeedLimit = 1000;
    Get().cTrackerCfgParam.iSpeedLimit = Get().cResultSenderParam.iSpeedLimit;

    GetEnum("\\HvDsp\\Misc"
            , "OutputCarSize"
            , &Get().cTrackerCfgParam.fOutputCarSize
            , Get().cTrackerCfgParam.fOutputCarSize
            , "0:�����;1:���"
            , "��������ߴ�"
            , ""
            , PROJECT_LEVEL
           );
		   */
    return S_OK;

}

HRESULT CSWLPRJupiterDomeCameraParameter::InitCamApp(VOID)
{

    GetString("\\HvDsp\\Camera\\Ctrl"
            , "Addr"
            , Get().cCamCfgParam.szIP
            , Get().cCamCfgParam.szIP
            , (100-1)
            , "ʶ�����IP"
            , ""
            , CUSTOM_LEVEL
            );

    GetEnum("\\HvDsp\\Camera\\Ctrl"
            , "ProtocolType"
            , &Get().cCamCfgParam.iCamType
            , Get().cCamCfgParam.iCamType
            , "0:һ���Э��;1:����Э��"
            , "Э������"
            , ""
            , PROJECT_LEVEL
           );

    GetInt("\\HvDsp\\Camera\\Ctrl"
            , "AddrPort"
            , &Get().cCamCfgParam.iCamPort
            , Get().cCamCfgParam.iCamPort
            , 1024
            , 99999
            , "����Э��˿ں�"
            , "�˿ں�"
            , PROJECT_LEVEL
          );

    Get().cCamCfgParam.iDynamicCfgEnable = 1;
    GetEnum("\\HvDsp\\Camera\\Ctrl"
            , "DynamicCfgEnable"
            , &Get().cCamCfgParam.iDynamicCfgEnable
            , Get().cCamCfgParam.iDynamicCfgEnable
            , "0:�ر�;1:��"
            , "��̬���ò���"
            , ""
            , CUSTOM_LEVEL
           );

	Get().cCamCfgParam.iDynamicTriggerEnable = 0;
	Get().cCamCfgParam.iTriggerDelay = 0;
	Get().cCamCfgParam.iNightShutterThreshold = 0;

    GetInt("\\HvDsp\\Camera\\Ctrl"
            , "NightShutterThreshold"
            , &Get().cCamCfgParam.iNightShutterThreshold
            , Get().cCamCfgParam.iNightShutterThreshold
            , 0
            , 30000
            , "ҹ���жϿ�����ֵ"
            , ""
            , PROJECT_LEVEL
          );

    GetInt("\\HvDsp\\Camera\\Ctrl"
            , "NightAvgYThreshold"
            , &Get().cCamCfgParam.iNightAvgYThreshold
            , Get().cCamCfgParam.iNightAvgYThreshold
            , 0
            , 255
            , "ҹ���жϻ���������ֵ"
            , ""
            , PROJECT_LEVEL
          );

    Get().cCamCfgParam.iEnableAGC = 1;

/*
    Get().cCamCfgParam.iAutoParamEnable = 1;
    if (Get().cCamCfgParam.iAutoParamEnable == 1)
    {
        UpdateEnum("\\HvDsp\\Camera\\Ctrl"
                , "AutoParamEnable"
                , 0
                );
    }

    char szText[255] = {0};
    char szChnText[255] = {0};
    const char *szLightType[] =
    {
        "BigSuitlight",
        "BigSuitlight2",
        "Suitlight",
        "Suitlight2",
        "Day",
        "Day2",
        "Backlight",
        "Backlight2",
        "BigBacklight",
        "BigBacklight2",
        "Nightfall",
        "Nightfall2",
        "Night",
        "Night2"
    };

    for (int i = 0; i < MAX_LEVEL_COUNT; i++)
    {
        sprintf(szText, "\\HvDsp\\Camera\\%s", szLightType[i]);
        sprintf(szChnText, "%d���ع�ʱ��", i);
        GetInt(szText
                , "ExposureTime"
                , &Get().cCamCfgParam.irgExposureTime[i]
                , Get().cCamCfgParam.irgExposureTime[i]
                , -1
                , 30000
                , szChnText
                , ""
                , PROJECT_LEVEL
              );

        sprintf(szChnText, "%d������", i);
        GetInt(szText
                , "Plus"
                , &Get().cCamCfgParam.irgGain[i]
                , Get().cCamCfgParam.irgGain[i]
                , -1
                , 360
                , szChnText
                , ""
                , PROJECT_LEVEL
              );

        sprintf(szChnText, "%d��AGC��������", i);
        GetInt(szText
                , "AGCLimit"
                , &Get().cCamCfgParam.irgAGCLimit[i]
                , Get().cCamCfgParam.irgAGCLimit[i]
                , -1
                , 255
                , szChnText
                , ""
                , CUSTOM_LEVEL
              );

    }

    if (Get().cCamCfgParam.iAutoParamEnable == 1)
    {
        InitCamCfgParam(&Get().cCamCfgParam, szLightType);
    }
  */

    return S_OK;
}

HRESULT CSWLPRJupiterDomeCameraParameter::InitIPTCtrl(VOID)
{
	CSWString strCOMM;
    CSWString strDomeRocker;
	  

  //////////////////////////////////////////
  strDomeRocker.Format("\\Device[�ⲿ�豸]\\DomeRocker");
  Get().cDomeRockerParam.fRockerEnable = TRUE;
  Get().cDomeRockerParam.iBaudrate = 115200;
  Get().cDomeRockerParam.iProtocol = 0;
  /*
  GetEnum((LPCSTR)strDomeRocker
            , "EnableRocker"
            , &Get().cDomeRockerParam.fRockerEnable
            , Get().cDomeRockerParam.fRockerEnable
            , "0:��ʹ��;1:ʹ��"
            , "���ҡ��ʹ��"
            , ""
            , CUSTOM_LEVEL
           );



  GetEnum((LPCSTR)strDomeRocker
			, "Baudrate"
			, &Get().cDomeRockerParam.iBaudrate
			, Get().cDomeRockerParam.iBaudrate
			, "300:300"
			  ";600:600"
			  ";1200:1200"
			  ";2400:2400"
			  ";4800:4800"
			  ";9600:9600"
			  ";19200:19200"
			  ";38400:38400"
			  ";43000:43000"
			  ";56000:56000"
			  ";57600:57600"
			  ";115200:115200"
			, "������"
			, ""
			, PROJECT_LEVEL
		);

  GetEnum((LPCSTR)strDomeRocker
            , "RockerProtocol"
            , &Get().cDomeRockerParam.iProtocol
            , Get().cDomeRockerParam.iProtocol
            , "0:PELCO-D;"  // todo  1:PELCO-P
            , "ҡ��Э��"
            , "Ŀǰֻ֧��PELCO-DЭ��"
            , PROJECT_LEVEL
           );
  */

  
  return S_OK;
}

HRESULT CSWLPRJupiterDomeCameraParameter::InitOuterCtrl(VOID)
{
	Get().cMatchParam.cPeccancyParkingParam.iNightUseLedLight = 1;
	GetEnum("\\OuterCtrl[���ܿ�]\\PeccancyParking"
    	, "NightUseLedLight"
    	, &Get().cMatchParam.cPeccancyParkingParam.iNightUseLedLight
    	, Get().cMatchParam.cPeccancyParkingParam.iNightUseLedLight
    	, "0:������;1:����"
    	, "ҹ��ʶ��ʹ��LED��"
    	, ""
    	, CUSTOM_LEVEL
    );

	CHAR szConfidenceFilter[64] = "0.0";
	GetString("\\OuterCtrl[���ܿ�]\\PeccancyParking"
			  , "ConfidenceFilter"
			  , szConfidenceFilter
			  , "1.0"
			  , 64
			  , "���Ŷȹ�����"
			  , "���Ŷȹ����������Ŷȵ��ڴ�ֵ����"
			  , CUSTOM_LEVEL);
     	swpa_sscanf(szConfidenceFilter, "%f",
					&Get().cMatchParam.cPeccancyParkingParam.fltConfidenceFilter);

		

		//��ʼ��Ԥ��λ��Ϣ
	for(INT i = 0; i < DETECT_AREA_COUNT_MAX; i++)
	{
		//TODO Ŀǰֻ���ĸ���
		CHAR szDetectArea1[64] = "[900,500];[1100,500];[1100,600];[900,600]";
		CSWString strDetAreaNode;
		CSWString strDetAreaName;
		strDetAreaNode.Format("\\OuterCtrl[���ܿ�]\\PeccancyParking\\DetectArea%d", i + 1);
		strDetAreaName.Format("DetectArea");


		Get().cMatchParam.cPeccancyParkingParam.cPeccancyParkingArea[i].iPresetNum = -1;
		GetInt((LPCSTR)strDetAreaNode
			   , "PresetNumber"
			   , &Get().cMatchParam.cPeccancyParkingParam.cPeccancyParkingArea[i].iPresetNum
			   , Get().cMatchParam.cPeccancyParkingParam.cPeccancyParkingArea[i].iPresetNum
			   , -1
			   , 256
			   , "����Ԥ��λ���"
			   , ""
			   , CUSTOM_LEVEL
				);
					
		Get().cMatchParam.cPeccancyParkingParam.cPeccancyParkingArea[i].iDetAreaNum = i + 1;
		CHAR szPeccancyTime[64] = "00:00";
		GetString((LPCSTR)strDetAreaNode
				  , "BeginTime"
				  , szPeccancyTime
				  , "00:00"
				  , 64
				  , "Υ�¿�ʼʱ��"
				  , "Υ�¿�ʼʱ�䣬ʱ:��"
				  , CUSTOM_LEVEL);
     	swpa_sscanf(szPeccancyTime, "%d:%d",
					&Get().cMatchParam.cPeccancyParkingParam.cPeccancyParkingArea[i].iBeginTime[0],
					&Get().cMatchParam.cPeccancyParkingParam.cPeccancyParkingArea[i].iBeginTime[1]);
							
								
		swpa_strcpy(szPeccancyTime, "23:59");
		GetString((LPCSTR)strDetAreaNode
				  , "EndTime"
				  , szPeccancyTime
				  , "23:59"
				  , 64
				  , "Υ�½���ʱ��"
				  , "Υ�½���ʱ�䣬ʱ:��"
				  , CUSTOM_LEVEL);
		swpa_sscanf(szPeccancyTime, "%d:%d",
					&Get().cMatchParam.cPeccancyParkingParam.cPeccancyParkingArea[i].iEndTime[0],
					&Get().cMatchParam.cPeccancyParkingParam.cPeccancyParkingArea[i].iEndTime[1]);
									
		Get().cMatchParam.cPeccancyParkingParam.cPeccancyParkingArea[i].iDetAreaPosCount = 4;

		GetString((LPCSTR)strDetAreaNode
				  , (LPCSTR)strDetAreaName
			   , szDetectArea1
			   , szDetectArea1
			   , 64
			   , "ʶ��������"
			   , "ʶ��������(��λ:����)"
			   , CUSTOM_LEVEL);
		swpa_sscanf(szDetectArea1, "[%d,%d];[%d,%d];[%d,%d];[%d,%d]",
					&Get().cMatchParam.cPeccancyParkingParam.cPeccancyParkingArea[i].rgiPosX[0],
					&Get().cMatchParam.cPeccancyParkingParam.cPeccancyParkingArea[i].rgiPosY[0],
					&Get().cMatchParam.cPeccancyParkingParam.cPeccancyParkingArea[i].rgiPosX[1],
					&Get().cMatchParam.cPeccancyParkingParam.cPeccancyParkingArea[i].rgiPosY[1],
					&Get().cMatchParam.cPeccancyParkingParam.cPeccancyParkingArea[i].rgiPosX[2],
					&Get().cMatchParam.cPeccancyParkingParam.cPeccancyParkingArea[i].rgiPosY[2],
					&Get().cMatchParam.cPeccancyParkingParam.cPeccancyParkingArea[i].rgiPosX[3],
					&Get().cMatchParam.cPeccancyParkingParam.cPeccancyParkingArea[i].rgiPosY[3]);

		
		Get().cMatchParam.cPeccancyParkingParam.cPeccancyParkingArea[i].iLimitTime = 900;
		GetInt((LPCSTR)strDetAreaNode
			   , "LimitTime"
			   , &Get().cMatchParam.cPeccancyParkingParam.cPeccancyParkingArea[i].iLimitTime
			   , Get().cMatchParam.cPeccancyParkingParam.cPeccancyParkingArea[i].iLimitTime
			   , 0
			   , 3600
			   , "Υ��ʱ����ֵ"
			   , "ͣ���೤ʱ��ﵽΥ�£���λ:��"
			   , CUSTOM_LEVEL);
	}


    Get().cMatchParam.fCaptureRecong = FALSE;



return S_OK;
}

HRESULT CSWLPRJupiterDomeCameraParameter::InitCamera(VOID)
{
	Get().cCamAppParam.iJpegCompressRate = 75;
/*
    GetInt("\\CamApp"
    	, "JpegCompressRate"
    	, &Get().cCamAppParam.iJpegCompressRate
    	, Get().cCamAppParam.iJpegCompressRate
    	, 1
    	, 99
    	, "��Ƶ��Jpegѹ��Ʒ��"
    	, ""
    	, PROJECT_LEVEL
    );
*/


    GetInt("\\CamApp"
    	, "IFrameInterval"
    	, &Get().cCamAppParam.iIFrameInterval
    	, Get().cCamAppParam.iIFrameInterval
    	, 2			//����Ҫ����СI֡���Ϊ2
    	, 25
    	, "H.264��I֡���"
    	, ""
    	, CUSTOM_LEVEL
    );

    if( Get().cGb28181.fEnalbe )
    {
    	Get().cCamAppParam.iIFrameInterval = 4;
        UpdateInt("\\CamApp"
            , "IFrameInterval"
            , Get().cCamAppParam.iIFrameInterval
            );
    }

	GetEnum("\\CamApp"
    	, "Resolution"
    	, &Get().cCamAppParam.iResolution
    	, Get().cCamAppParam.iResolution
    	, "0:1080P;1:720P"
    	, "H.264ͼ��ֱ���"
    	, ""
    	, CUSTOM_LEVEL
    );
    
	Get().cCamAppParam.iTargetBitRate = 2 * 1024;
    GetInt("\\CamApp"
    	, "TargetBitRate"
    	, &Get().cCamAppParam.iTargetBitRate
    	, Get().cCamAppParam.iTargetBitRate
    	, 512
    	, 16*1024
    	, "H.264�����������"
    	, "��λ��Kbps"
    	, CUSTOM_LEVEL
		);

	Get().cCamAppParam.iMaxBitRate = 2 * Get().cCamAppParam.iTargetBitRate;
    GetInt("\\CamApp"
    	, "MaxBitRate"
    	, &Get().cCamAppParam.iMaxBitRate
    	, Get().cCamAppParam.iMaxBitRate
    	, 512
    	, 16*1024
    	, "H.264�������������"
    	, "��λ��Kbps"
    	, CUSTOM_LEVEL
		);

	GetEnum("\\CamApp"
			, "RateControl"
			, &Get().cCamAppParam.iRateControl
			, Get().cCamAppParam.iRateControl
			, "0:VBR;1:CBR"
			, "H.264��������"
			, "0:VBR �ɱ�����;1:CBR �㶨����"
			, CUSTOM_LEVEL);
	
	//�ڶ�·H264����
	GetInt("\\CamApp"
		, "IFrameIntervalSecond"
		, &Get().cCamAppParam.iIFrameIntervalSecond
		, Get().cCamAppParam.iIFrameIntervalSecond
		, 2
		, 25
		, "�ڶ�·H.264��I֡���"
		, ""
		, CUSTOM_LEVEL
	);

	GetEnum("\\CamApp"
		, "ResolutionSecond"
		, &Get().cCamAppParam.iResolutionSecond
		, Get().cCamAppParam.iResolutionSecond
		, "0:1080P;1:720P;3:576P;4:480P"
		, "�ڶ�·H.264ͼ��ֱ���"
		, ""
		, CUSTOM_LEVEL
	);

	Get().cCamAppParam.iTargetBitRateSecond = 2 * 1024;
	GetInt("\\CamApp"
		, "TargetBitRateSecond"
		, &Get().cCamAppParam.iTargetBitRateSecond
		, Get().cCamAppParam.iTargetBitRateSecond
		, 512
		, 16*1024
		, "�ڶ�·H.264�����������"
		, "��λ��Kbps��ǧ����ÿ�룩"
		, CUSTOM_LEVEL
	);

	GetInt("\\CamApp"
		, "MaxBitRateSecond"
		, &Get().cCamAppParam.iMaxBitRateSecond
		, Get().cCamAppParam.iMaxBitRateSecond
		, 512
		, 16*1024
		, "�ڶ�·H.264�������������"
		, "��λ��Kbps��ǧ����ÿ�룩"
		, CUSTOM_LEVEL
	);

	//
	GetInt("\\CamApp"
		, "FrameRateSecond"
		, &Get().cCamAppParam.iFrameRateSecond
		, Get().cCamAppParam.iFrameRateSecond
		, 15
		, 25
		, "�ڶ�·H.264�����֡��"
		, "��λ��fps��֡ÿ�룩"
		, CUSTOM_LEVEL
	);

	GetEnum("\\CamApp"
			, "RateControlSecond"
			, &Get().cCamAppParam.iRateControlSecond
			, Get().cCamAppParam.iRateControlSecond
			, "0:VBR;1:CBR"
			, "�ڶ�·H.264��������"
			, "0:VBR �ɱ�����;1:CBR �㶨����"
			, CUSTOM_LEVEL);
	//end
	

    GetInt("\\CamApp"
    	, "GainR"
    	, &Get().cCamAppParam.iGainR
    	, Get().cCamAppParam.iGainR
    	, 0
    	, 255
    	, "R����"
    	, "ע��:AWB�ֶ�ģʽ������Ч"
    	, CUSTOM_LEVEL
    );

    /* //�����о��G�����޷���
    GetInt("\\CamApp"
    	, "GainG"
    	, &Get().cCamAppParam.iGainG
    	, Get().cCamAppParam.iGainG
    	, 36
    	, 255
    	, "G����"
    	, ""
    	, PROJECT_LEVEL
    );*/

    GetInt("\\CamApp"
    	, "GainB"
    	, &Get().cCamAppParam.iGainB
    	, Get().cCamAppParam.iGainB
    	, 0
    	, 255
    	, "B����"
    	, "ע��:AWB�ֶ�ģʽ������Ч"
    	, CUSTOM_LEVEL
    );

	Get().cCamAppParam.iAutoFocus = 0;
	GetEnum("\\CamApp"
        , "AF"
        , &Get().cCamAppParam.iAutoFocus
        , Get().cCamAppParam.iAutoFocus
        , "0:�Զ��۽�;1:�ֶ��۽�;"
        , "�۽�ģʽ"
        , ""
        , CUSTOM_LEVEL
    );


	Get().cCamAppParam.iGain = 6;
	GetEnum("\\CamApp"
        , "Gain"
        , &Get().cCamAppParam.iGain
        , Get().cCamAppParam.iGain
        , "15:+28dB;"
			"14:+26dB;"
			"13:+24dB;"
			"12:+22dB;"
			"11:+20dB;"
			"10:+18dB;"
			"9:+16dB;"
			"8:+14dB;"
			"7:+12dB;" 
			"6:+10dB;"
			"5:+8dB;"
			"4:+6dB;"
			"3:+4dB;"
			"2:+2dB;"
			"1:0dB;"
			"0:-3dB"
        , "����"
        , "ע��:AE�Զ�ģʽ�����ø�ֵ��Ч!"
        , CUSTOM_LEVEL
    );


	Get().cCamAppParam.iShutter = 10;
	GetEnum("\\CamApp"
        , "Shutter"
        , &Get().cCamAppParam.iShutter
        , Get().cCamAppParam.iShutter
        , "21:1/10000;"
			"20:1/6000;"
			"19:1/3500;"
			"18:1/2500;"
			"17:1/1750;"
			"16:1/1250;"
			"15:1/1000;"
			"14:1/600;"
			"13:1/425;"
			"12:1/300;"
			"11:1/215;"
			"10:1/150;"
			"9:1/120;"
			"8:1/100;"
			"7:1/75;"
			"6:1/50;"
			"5:1/25;"
			"4:1/12;"
			"3:1/6;"
			"2:1/3;"
			"1:1/2;"
			"0:1/1"
        , "����"
        , "ע��:AE�Զ�ģʽ�����ø�ֵ��Ч!"
        , CUSTOM_LEVEL
    );

	Get().cCamAppParam.iIris = 7;
	GetEnum("\\CamApp"
        , "Iris"
        , &Get().cCamAppParam.iIris
        , Get().cCamAppParam.iIris
        , "13:F1.6;"
			"12:F2;"
			"11:F2.4;"
			"10:F2.8;"
			"9:F3.4;"
			"8:F4;"
			"7:F4.8;"
			"6:F5.6;"
			"5:F6.8;"
			"4:F8;"
			"3:F9.6;"
			"2:F11;"
			"1:F14;"
			"0:CLOSE"
        , "��Ȧ"
        , "ע��:AE�Զ�ģʽ�����ø�ֵ��Ч!"
        , CUSTOM_LEVEL
    );


	Get().cCamAppParam.iJpegAutoCompressEnable = 1;
	Get().cCamAppParam.iJpegExpectSize = 200 * 1024;  //Ĭ��200KB
/*
    GetEnum("\\CamApp"
        , "JpegAutoCompressEnable"
        , &Get().cCamAppParam.iJpegAutoCompressEnable
        , Get().cCamAppParam.iJpegAutoCompressEnable
        , "0:��ʹ��;1:ʹ��"
        , "Jpeg�Զ����ڿ���"
        , ""
        , PROJECT_LEVEL
    );


    GetInt("\\CamApp"
    	, "JpegExpectSize"
    	, &Get().cCamAppParam.iJpegExpectSize
    	, Get().cCamAppParam.iJpegExpectSize
    	, 0
    	, 1024*1024
    	, "JpegͼƬ������С"
    	, "��λ��Byte���ֽڣ�ע��Ϊ0���ʾ���Ըò���"
    	, PROJECT_LEVEL
    );

    GetEnum("\\CamApp"
    	, "JpegType"
    	, &Get().cCamAppParam.iJpegType
    	, Get().cCamAppParam.iJpegType
    	, "0:YUV�ֿ�;1:YUV���"
    	, "JpegͼƬ��ʽ"
    	, ""
    	, PROJECT_LEVEL
    );

    GetInt("\\CamApp"
    	, "JpegCompressRateLow"
    	, &Get().cCamAppParam.iJpegCompressRateL
    	, Get().cCamAppParam.iJpegCompressRateL
    	, 1
    	, 100
    	, "Jpegѹ�����Զ���������"
    	, ""
    	, PROJECT_LEVEL
    );

    GetInt("\\CamApp"
    	, "JpegCompressRateHigh"
    	, &Get().cCamAppParam.iJpegCompressRateH
    	, Get().cCamAppParam.iJpegCompressRateH
    	, 1
    	, 100
    	, "Jpegѹ�����Զ���������"
    	, ""
    	, PROJECT_LEVEL
    );
	*/
					

	Get().cCamAppParam.iAEMode = 0;
    GetEnum("\\CamApp"
    	, "AEMode"
    	, &Get().cCamAppParam.iAEMode
    	, Get().cCamAppParam.iAEMode
    	, "0:�Զ�;1:�ֶ�;2:��������;3:��Ȧ����"
    	, "AEģʽ"
    	, ""
    	, CUSTOM_LEVEL
    );

	Get().cCamAppParam.iAWBMode = 0;
	GetEnum("\\CamApp"
    	, "AWBMode"
    	, &Get().cCamAppParam.iAWBMode
    	, Get().cCamAppParam.iAWBMode
    	, "0:�Զ�;"
    		"1:ATW(Auto Tracing White balance);"
    		"2:����;"
    		/*"3:����;"*/ //Bug 9109: ������Ѷ��о��֧�ָ�ģʽ��������"6:�Զ�����;"ģʽ�����ȥ��
    		"4:����һ�ε���(�ðװ��ʱ���õ����ϵ����ڵ�ֵ�Żᶪʧ);"
    		"5:�ֶ�;"
    		"6:�Զ�����;"
    		"7:�Զ��ɵ�;"
    		"8:�ɵ�"
    	, "AWBģʽ"
    	, ""
    	, CUSTOM_LEVEL
    );
    
    GetInt("\\CamApp"
    	, "SaturationValue"
    	, &Get().cCamAppParam.iSaturationValue
    	, Get().cCamAppParam.iSaturationValue
    	, 0
    	, 0xe
    	, "���Ͷ�"
    	, ""
    	, CUSTOM_LEVEL
    );

	Get().cCamAppParam.iWDREnable = 0;
	GetEnum("\\CamApp"
            , "WDR"
            , &Get().cCamAppParam.iWDREnable
            , Get().cCamAppParam.iWDREnable
            , "1:����;0:�ر�"
            , "WDR"
            , ""
            , CUSTOM_LEVEL
            );

	Get().cCamAppParam.iSNFTNFMode = 0;
    GetEnum("\\CamApp"
            , "NoiseReduction"
            , &Get().cCamAppParam.iNRLevel
            , Get().cCamAppParam.iNRLevel
            , "0:��ʹ��;1:�ȼ�1;2:�ȼ�2;3:�ȼ�3;4:�ȼ�4;5:�ȼ�5"
            , "����ģʽ"
            , ""
            , CUSTOM_LEVEL
            );

    GetEnum("\\CamApp"
        , "BlackWhiteMode"
        , &Get().cCamAppParam.iGrayImageEnable
        , Get().cCamAppParam.iGrayImageEnable
        , "0:��ʹ��;1:ʹ�� "
        , "�ڰ�ͼ���ģʽ"
        ,""
        , PROJECT_LEVEL
    );

	Get().cCamAppParam.iGammaValue = 0;
	GetEnum("\\CamApp"
        , "GammaValue"
        , &Get().cCamAppParam.iGammaValue
        , Get().cCamAppParam.iGammaValue
        , "0:��ʹ��;1:ֱ��;2:S����-��;3:S����-��;4:S����-��"
        , "���٤��"
        ,""
        , CUSTOM_LEVEL
    );

	Get().cCamAppParam.iSharpenThreshold = 8;
	GetInt("\\CamApp"
        , "SharpenValue"
        , &Get().cCamAppParam.iSharpenThreshold
        , Get().cCamAppParam.iSharpenThreshold
        , 0
        , 15
        , "ͼ�����"
        , "ͼ�����"
        , PROJECT_LEVEL
    );

	Get().cCamAppParam.iEdgeValue = 0;
	GetInt("\\CamApp"
        , "EdgeValue"
        , &Get().cCamAppParam.iEdgeValue
        , Get().cCamAppParam.iEdgeValue
        , 0
        , 255
        , "ͼ���Ե��ǿ"
        , "ͼ���Ե��ǿ"
        , CUSTOM_LEVEL
    );

	
	/*
	�����cvbs�����
	GetEnum("\\CamApp"
        , "CVBSMode"
        , &Get().cCamAppParam.iCVBSMode
        , Get().cCamAppParam.iCVBSMode
        , "0:PAL;1:NTSC"
        , "CVBS��ʽ"
        , ""
        , CUSTOM_LEVEL
    );
	
	Get().cCamAppParam.iCVBSExport = 1;		//Ĭ�� �� �������
	GetEnum("\\CamApp"
		, "CVBSIMAGEMode"
		, &Get().cCamAppParam.iCVBSExport
		, Get().cCamAppParam.iCVBSExport
		, "0:CVBS ��С���;1:CVBS �������"
		, "CVBSͼ��ģʽ"
		, ""
		, CUSTOM_LEVEL
	);*/

	return S_OK;
}

HRESULT CSWLPRJupiterDomeCameraParameter::InitCharacter(VOID)
{
	
	Get().cOverlay.fH264Eanble = 1;
	GetEnum("\\Overlay\\H264"
		  , "Enable"
		  , &Get().cOverlay.fH264Eanble
		  , Get().cOverlay.fH264Eanble
		  , "0:��ʹ��;1:ʹ��"
		  , "H264�ַ�����ʹ��"
		  , ""
		  , CUSTOM_LEVEL
	);
	
	GetInt("\\Overlay\\H264"
		  , "X"
		  , &Get().cOverlay.cH264Info.iTopX
		  , Get().cOverlay.cH264Info.iTopX
		  , 0
		  , 1920
		  , "X����"
		  , ""
		  , CUSTOM_LEVEL
	  );
	  
	GetInt("\\Overlay\\H264"
		  , "Y"
		  , &Get().cOverlay.cH264Info.iTopY
		  , Get().cOverlay.cH264Info.iTopY
		  , 0
		  , 1080
		  , "Y����"
		  , ""
		  , CUSTOM_LEVEL
	  ); 

	Get().cOverlay.cH264Info.iFontSize = 64;
	GetInt("\\Overlay\\H264"
		  , "Size"
		  , &Get().cOverlay.cH264Info.iFontSize
		  , Get().cOverlay.cH264Info.iFontSize
		  , 16
		  , 128
		  , "�����С"
		  , ""
		  , CUSTOM_LEVEL
	  ); 
	DWORD dwR = 255, dwG = 0,dwB = 0;  
	GetInt("\\Overlay\\H264"
		  , "R"
		  , (INT *)&dwR
		  , dwR
		  , 0
		  , 255
		  , "������ɫR����"
		  , ""
		  , CUSTOM_LEVEL
	  );
	GetInt("\\Overlay\\H264"
		  , "G"
		  , (INT *)&dwG
		  , dwG
		  , 0
		  , 255
		  , "������ɫG����"
		  , ""
		  , CUSTOM_LEVEL
	  );
	GetInt("\\Overlay\\H264"
		  , "B"
		  , (INT *)&dwB
		  , dwB
		  , 0
		  , 255
		  , "������ɫB����"
		  , ""
		  , CUSTOM_LEVEL
	  );
	Get().cOverlay.cH264Info.dwColor = (dwB | (dwG << 8) | (dwR << 16));
	SW_TRACE_DEBUG("h264 color[0x%08x][0x%02x,0x%02x,0x%02x]", Get().cOverlay.cH264Info.dwColor, dwR, dwG, dwB);
		Get().cOverlay.cH264Info.fEnableTime = 1;  //Ĭ��ʱ��ʱ�����
	GetEnum("\\Overlay\\H264"
		  , "DateTime"
		  , &Get().cOverlay.cH264Info.fEnableTime
		  , Get().cOverlay.cH264Info.fEnableTime
		  , "0:��ʹ��;1:ʹ��"
		  , "����ʱ��"
		  , ""
		  , CUSTOM_LEVEL
	);


	swpa_sprintf(Get().cOverlay.cH264Info.szInfo, " ");
	GetString("\\Overlay\\H264"
		  , "String"
		  , Get().cOverlay.cH264Info.szInfo
		  , Get().cOverlay.cH264Info.szInfo
		  , sizeof(Get().cOverlay.cH264Info.szInfo)
		  , ""
		  , "������Ϣ"
		  , CUSTOM_LEVEL
	);
    if(!swpa_strcmp(Get().cOverlay.cH264Info.szInfo, "NULL"))
	{
	    swpa_strcpy(Get().cOverlay.cH264Info.szInfo, "");
	}
	
	// todo.�ַ����Ӽ����ַ��ܳ��������⣬������ʱ��������ո���β�������������������ʾ��ȫ��
	if( strlen(Get().cOverlay.cH264Info.szInfo) + 3 < sizeof(Get().cOverlay.cH264Info.szInfo) )
	{
		strcpy(Get().cOverlay.cH264Info.szInfo + strlen(Get().cOverlay.cH264Info.szInfo), "  ");
		SW_TRACE_NORMAL("<param>change overlay h264 len.%s", Get().cOverlay.cH264Info.szInfo);
	}
	

	  Get().cOverlay.cJPEGInfo.fEnable = 1;
	  GetEnum("\\Overlay\\JPEG"
		  , "Enable"
		  , &Get().cOverlay.cJPEGInfo.fEnable
		  , Get().cOverlay.cJPEGInfo.fEnable
		  , "0:��ʹ��;1:ʹ��"
		  , "JPEG�ַ�����ʹ��"
		  , ""
		  , PROJECT_LEVEL
	  );
	  
	GetInt("\\Overlay\\JPEG"
		  , "X"
		  , &Get().cOverlay.cJPEGInfo.iX
		  , Get().cOverlay.cJPEGInfo.iX
		  , 0
		  , 1920
		  , "X����"
		  , ""
		  , PROJECT_LEVEL
	  );
	  
	GetInt("\\Overlay\\JPEG"
		  , "Y"
		  , &Get().cOverlay.cJPEGInfo.iY
		  , Get().cOverlay.cJPEGInfo.iY
		  , 0
		  , 1080
		  , "Y����"
		  , ""
		  , PROJECT_LEVEL
	  ); 

	Get().cOverlay.cJPEGInfo.iFontSize = 64;
	GetInt("\\Overlay\\JPEG"
		  , "Size"
		  , &Get().cOverlay.cJPEGInfo.iFontSize
		  , Get().cOverlay.cJPEGInfo.iFontSize
		  , 16
		  , 128
		  , "�����С"
		  , ""
		  , PROJECT_LEVEL
	  ); 

	dwR = 255;
	dwG = 0;
	dwB = 0;
	GetInt("\\Overlay\\JPEG"
		  , "R"
		  , (INT *)&dwR
		  , dwR
		  , 0
		  , 255
		  , "������ɫR����"
		  , ""
		  , PROJECT_LEVEL
	  );
	GetInt("\\Overlay\\JPEG"
		  , "G"
		  , (INT *)&dwG
		  , dwG
		  , 0
		  , 255
		  , "������ɫG����"
		  , ""
		  , PROJECT_LEVEL
	  );
	GetInt("\\Overlay\\JPEG"
		  , "B"
		  , (INT *)&dwB
		  , dwB
		  , 0
		  , 255
		  , "������ɫB����"
		  , ""
		  , PROJECT_LEVEL
	  );
	Get().cOverlay.cJPEGInfo.iColor = (dwB | (dwG << 8) | (dwR << 16));
	SW_TRACE_DEBUG("jpeg color[0x%08x][0x%02x,0x%02x,0x%02x]", Get().cOverlay.cJPEGInfo.iColor, dwR, dwG, dwB);
	Get().cOverlay.cJPEGInfo.fEnableTime = 1;
	GetEnum("\\Overlay\\JPEG"
		  , "DateTime"
		  , &Get().cOverlay.cJPEGInfo.fEnableTime
		  , Get().cOverlay.cJPEGInfo.fEnableTime
		  , "0:��ʹ��;1:ʹ��"
		  , "����ʱ��"
		  , ""
		  , PROJECT_LEVEL
	  );

	 //swpa_sprintf(Get().cOverlay.cJPEGInfo.szInfo, "�ַ���������");
	 swpa_sprintf(Get().cOverlay.cJPEGInfo.szInfo, " ");
	 GetString("\\Overlay\\JPEG"
		  , "String"
		  , Get().cOverlay.cJPEGInfo.szInfo
		  , " " //Get().cOverlay.cJPEGInfo.szInfo
		  , sizeof(Get().cOverlay.cJPEGInfo.szInfo)
		  , ""
		  , "������Ϣ"
		  , PROJECT_LEVEL
	  );

	if(!swpa_strcmp(Get().cOverlay.cJPEGInfo.szInfo, "NULL"))
	{
	  swpa_strcpy(Get().cOverlay.cJPEGInfo.szInfo, "");
	}
	 // todo.�ַ����Ӽ����ַ��ܳ��������⣬������ʱ��������ո���β�������������������ʾ��ȫ��
	 if( strlen(Get().cOverlay.cJPEGInfo.szInfo) + 3 < sizeof(Get().cOverlay.cJPEGInfo.szInfo) )
	 {
		strcpy(Get().cOverlay.cJPEGInfo.szInfo + strlen(Get().cOverlay.cJPEGInfo.szInfo), "  ");
		SW_TRACE_NORMAL("<param>change overlay jpeg len.%s", Get().cOverlay.cJPEGInfo.szInfo);
	 }

	  //����û�е��ӳ��ƺ���ʹ�ܿ���
	/*GetEnum("\\Overlay\\JPEG"
		  , "Plate"
		  , &Get().cOverlay.cJPEGInfo.fEnablePlate
		  , Get().cOverlay.cJPEGInfo.fEnablePlate
		  , "0:��ʹ��;1:ʹ��"
		  , "���ӳ��ƺ���"
		  , ""
		  , CUSTOM_LEVEL
	  );*/
	  return S_OK;
}

HRESULT CSWLPRJupiterDomeCameraParameter::InitGB28181(VOID)
{
	return CSWLPRParameter::InitGB28181();
}


HRESULT CSWLPRJupiterDomeCameraParameter::InitAutoReboot(VOID)
{	

	GetEnum("\\AutoReboot"
	  , "Enable"
	  , &Get().cAutoRebootParam.fEnable
	  , Get().cAutoRebootParam.fEnable
	  , "0:��ʹ��;1:ʹ��"
	  , "�Զ���λ"
	  , ""
	  , CUSTOM_LEVEL
	);	

	GetInt("\\AutoReboot"
		  , "TrafficFlows"
		  , &Get().cAutoRebootParam.iCarLeftCountLimit
		  , Get().cAutoRebootParam.iCarLeftCountLimit
		  , 0
		  , 1000
		  , "��ͨ����"
		  , "��λΪʮ�����ڳ�������"
		  , CUSTOM_LEVEL
	  );

	GetInt("\\AutoReboot"
		  , "SystemUpTime"
		  , &Get().cAutoRebootParam.iSysUpTimeLimit
		  , Get().cAutoRebootParam.iSysUpTimeLimit
		  , 24
		  , 720
		  , "����ʱ��"
		  , "��λ:Сʱ"
		  , CUSTOM_LEVEL
	  );


	return S_OK;
}



HRESULT CSWLPRJupiterDomeCameraParameter::InitONVIF(VOID)
{

    SW_TRACE_NORMAL(" HRESULT CSWLPRJupiterDomeCameraParameter::InitONVIF(VOID)");

	Get().cRtspParam.iRTSPStreamNum = 2;
    Get().cRtspParam.rgCommunicationMode[RTSP_STREAM_MAJOR] = 0;
    Get().cRtspParam.rgCommunicationMode[RTSP_STREAM_MINOR] = 0;


/*
	GetEnum("\\RTSP"
        , "CommunicationMode"
        , &Get().cRtspParam.iMajorCommunicationMode
        , Get().cRtspParam.iMajorCommunicationMode
        , "0:����;1:�鲥"
        , "ͨѸ��ʽ"
        , ""
        , PROJECT_LEVEL
     );


	GetInt("\\RTSP"
		   , "iRTSPStreamNum"
		   , &Get().cRtspParam.iRTSPStreamNum
		   , Get().cRtspParam.iRTSPStreamNum
		   , 1
		   , 2
		   , "RTSP��Ƶ������"
		   , ""
		   , PROJECT_LEVEL
		);
*/

    SW_TRACE_NORMAL(" HRESULT CSWLPRJupiterDomeCameraParameter::InitONVIF(VOID) End");

    return S_OK;
}



