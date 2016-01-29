/*
 * SWLPRVenusTollGateParameter.cpp
 *
 *  Created on: 2013��12��27��
 *      Author: qinjj
 */
#include "SWFC.h"
#include "SWLPRApplication.h"
#include "SWLPRVenusCameraParameter.h"


SWLPRVenusCameraParameter::SWLPRVenusCameraParameter()
{
	ResetParam();
}

SWLPRVenusCameraParameter::~SWLPRVenusCameraParameter()
{
}

void SWLPRVenusCameraParameter::ResetParam(VOID)
{
	//�ȵ��û���
	CSWLPRParameter::ResetParam();

	/*���ݾ��巽�����ò���*/

    Get().nWorkModeIndex = PRM_CAMERA;
    m_strWorkMode.Format("�����", Get().nWorkModeIndex);
	Get().nCameraWorkMode = 0;				//��������շ�վ�����������ģʽ
	Get().nMCFWUsecase = IMX185_DUALVIDEO;
    //����Ĭ��ֵ
    for (int i = 0; i < MAX_ROADLINE_NUM; i++)
    {
        swpa_memset(&Get().cTrackerCfgParam.rgcRoadInfo[i], 0, sizeof(ROAD_INFO));
        swpa_strcpy(Get().cTrackerCfgParam.rgcRoadInfo[i].szRoadTypeName, "NULL");
        Get().cTrackerCfgParam.rgcRoadInfo[i].iLineType = 1;
        Get().cTrackerCfgParam.rgcRoadInfo[i].iRoadType = (int)(RRT_FORWARD | RRT_LEFT | RRT_RIGHT | RRT_TURN);
        Get().cTrackerCfgParam.cActionDetect.iIsYellowLine[i] = 0;
        Get().cTrackerCfgParam.cActionDetect.iIsCrossLine[i] = 0;
    }

    //ɨ������Ĭ��ֵ
    Get().cTrackerCfgParam.cDetectArea.fEnableDetAreaCtrl = FALSE;
    Get().cTrackerCfgParam.cDetectArea.DetectorAreaLeft = 0;
    Get().cTrackerCfgParam.cDetectArea.DetectorAreaRight = 95;
    Get().cTrackerCfgParam.cDetectArea.DetectorAreaTop = 30;
    Get().cTrackerCfgParam.cDetectArea.DetectorAreaBottom = 90;
    Get().cTrackerCfgParam.cDetectArea.nDetectorMinScaleNum = 6;
    Get().cTrackerCfgParam.cDetectArea.nDetectorMaxScaleNum = 14;

    //ͶƱ����
    Get().cTrackerCfgParam.nVoteFrameNum = 16;
    Get().cTrackerCfgParam.nMaxEqualFrameNumForVote = 8;

    //��Ƶ������
    Get().cTrackerCfgParam.cVideoDet.nVideoDetMode = NO_VIDEODET;

    // ǰ���������
    Get().cCamCfgParam.iMaxExposureTime = 15000;
    Get().cCamCfgParam.iMinExposureTime = 0;
    Get().cCamCfgParam.iMaxGain = 180;

    // ����Ĭ�ϲ���ֵ
    Get().cCamCfgParam.iMinPSD = 300;
    Get().cCamCfgParam.iMaxPSD = 1500;

    //���ܿ�Ĭ��ֵ
    Get().cMatchParam.dwSignalKeepTime = 6000;
        Get().cMatchParam.dwPlateKeepTime = 2000;
        Get().cMatchParam.dwMatchMinTime = 3000;
        Get().cMatchParam.dwMatchMaxTime = 3000;

    //�շ�վĬ�ϲ��������
    Get().cTrackerCfgParam.nDetReverseRunEnable = 0;

    Get().cDevParam[1].iCommType = 1;	//����ֻ��һ������COM1,ֻ֧��485

    Get().cCamAppParam.iResolution = 0;	//Ĭ��Ϊ1080P

    // ץ��λ�ó�ʼֵ
    Get().cTrackerCfgParam.nCaptureOnePos = 50;

    // ������Ƶ�������
    Get().cTrackerCfgParam.iUsedLight = 0;

    //����ɨ������Ĭ�ϴ�
    Get().cTrackerCfgParam.cTrapArea.fEnableDetAreaCtrl = TRUE;

    //����RGB�������÷�ΧΪ36~255,Ĭ��ֵ����Ϊ0
    Get().cCamAppParam.iGainR = 36;
    Get().cCamAppParam.iGainG = 36;
    Get().cCamAppParam.iGainB = 36;

}

HRESULT SWLPRVenusCameraParameter::Initialize(CSWString strFilePath)
{
	// ��ʼ��ǰ��������Ĭ��ֵ
	ResetParam();
	
    // �Ȼ���ĳ�ʼ��
    if (S_OK == CSWParameter<ModuleParams>::Initialize(strFilePath)
        && S_OK == InitSystem()
        && S_OK == InitCamera()
        && S_OK == InitHvDsp()
        && S_OK == InitCharacter()
        && S_OK == InitGB28181()
		&& S_OK == InitONVIF())
    {
        return S_OK ;
    }

    SW_TRACE_DEBUG("<SWLPRVenusCameraParameter> Initialize failed.\n");
    return E_FAIL ;
}

HRESULT SWLPRVenusCameraParameter::InitSystem(VOID)
{
    if (S_OK == CSWLPRParameter::InitSystem())
    {
        return S_OK ;
    }
    return E_FAIL ;

}

HRESULT SWLPRVenusCameraParameter::InitTracker(VOID)
{
    return S_OK;
}

HRESULT SWLPRVenusCameraParameter::InitHvDsp(VOID)
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
	
    GetEnum("\\HvDsp\\Misc"
        , "HDVideoEnable"
        , &Get().cResultSenderParam.fSaveVideo
        , Get().cResultSenderParam.fSaveVideo
        , "0:��¼��;1:¼��"
        , "Ӳ��¼�񿪹�"
        , ""
        , CUSTOM_LEVEL
        );

    GetEnum("\\HvDsp\\Misc"
        , "SafeSaver"
        , &Get().cResultSenderParam.fIsSafeSaver
        , Get().cResultSenderParam.fIsSafeSaver
        , "0:��ʹ��;1:ʹ��"
        , "��ȫ�洢ʹ��"
        , ""
        , PROJECT_LEVEL
    );

    GetEnum("\\HvDsp\\Misc"
        , "InitHdd"
        , &Get().cResultSenderParam.fInitHdd
        , Get().cResultSenderParam.fInitHdd
        , "0:��;1:������ʽ��"
        , "��ʼ��SD����ע��һ������Ч"
        , ""
        , CUSTOM_LEVEL
    );


    GetInt("\\HvDsp\\Misc"
        , "OneFileSize"
        , &Get().cResultSenderParam.iFileSize
        , Get().cResultSenderParam.iFileSize
        , 1
        , 1024
        , "�����洢�����ļ���С,(��λK)"
        , ""
        , PROJECT_LEVEL
    );

    GetEnum("\\HvDsp\\Misc"
        , "SaveSafeType"
        , &Get().cResultSenderParam.iSaveSafeType
        , Get().cResultSenderParam.iSaveSafeType
        , "0:û�пͻ�������ʱ�Ŵ洢;1:һֱ�洢"
        , "SD���洢��ʽ"
        , ""
        , PROJECT_LEVEL
    );

    GetEnum("\\HvDsp\\Misc"
        , "DiskType"
        , &Get().cResultSenderParam.iDiskType
        , Get().cResultSenderParam.iDiskType
        , "0:��;2:SD��"
        , "�洢����"
        , ""
        , CUSTOM_LEVEL
    );

    // zhaopy
    // ����ǹ�̬Ӳ����Ҫ�����Ƿ�ʹ��Ӳ�̵ı�־��
    if( 2 == Get().cResultSenderParam.iDiskType )
    {
        INT iUsed = (Get().cResultSenderParam.fIsSafeSaver || Get().cResultSenderParam.fSaveVideo) ? 1 : 0;
        INT iSet = 0;
        INT iRet = swpa_device_get_hdd_flag(&iSet);
        SW_TRACE_NORMAL("<Param>get hdd flag:%d. return:%d.", iSet, iRet);
        if( 0 == iRet && iSet != iUsed )
        {
            iRet = swpa_device_set_hdd_flag(iUsed);
            SW_TRACE_NORMAL("<Param>set hdd flag:%d. return:%d.", iUsed, iRet);
        }
    }

    /*GetString("\\HvDsp\\Misc"
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
    );*/

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
        , 1
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


    /*GetString("\\HvDsp\\Identify", "StreetName"
        , Get().cResultSenderParam.szStreetName
        , Get().cResultSenderParam.szStreetName
        , sizeof(Get().cResultSenderParam.szStreetName)
        , "·������"
        , ""
        , CUSTOM_LEVEL
        );

    GetString("\\HvDsp\\Identify"
        , "StreetDirection"
        , Get().cResultSenderParam.szStreetDirection
        , Get().cResultSenderParam.szStreetDirection
        , sizeof(Get().cResultSenderParam.szStreetDirection)
        , "·�ڷ���"
        , ""
        , CUSTOM_LEVEL
        );*/

    GetInt("\\HvDsp\\Trigger"
        , "TriggerOutEnable"
        , &Get().cResultSenderParam.fEnableTriggerOut
        , Get().cResultSenderParam.fEnableTriggerOut
        , 0
        , 1
        , "�������ʹ�ܿ���"
        , ""
        , CUSTOM_LEVEL
        );

    GetEnum("\\HvDsp\\Trigger"
        , "TriggerOutNormalStatus"
        , &Get().cResultSenderParam.nTriggerOutNormalStatus
        , Get().cResultSenderParam.nTriggerOutNormalStatus
        , "0:�ߵ�ƽ;1:�͵�ƽ"
        , "���������̬����"
        , ""
        , CUSTOM_LEVEL
    );

    GetInt("\\HvDsp\\Trigger"
        , "TriggerOutPlusWidth"
        , &Get().cResultSenderParam.nTriggerOutPlusWidth
        , Get().cResultSenderParam.nTriggerOutPlusWidth
        , 100
        , 30000
        , "�������ͨ��ʱ�䣬��λΪms"
        , ""
        , CUSTOM_LEVEL
        );

    return S_OK ;
}

HRESULT SWLPRVenusCameraParameter::InitOuterCtrl(VOID)
{
    return S_OK ;
}


HRESULT SWLPRVenusCameraParameter::InitIPTCtrl(VOID)
{
    return S_OK;
}


HRESULT SWLPRVenusCameraParameter::InitCamApp(VOID)
{
    return S_OK;
}

HRESULT SWLPRVenusCameraParameter::InitCamera(VOID)
{
    GetInt("\\CamApp"
        , "JpegCompressRate"
        , &Get().cCamAppParam.iJpegCompressRate
        , Get().cCamAppParam.iJpegCompressRate
        , 1
        , 99
        , "��Ƶ��Jpegѹ��Ʒ��"
        , ""
        , CUSTOM_LEVEL
    );

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

    GetInt("\\CamApp"
        , "TargetBitRate"
        , &Get().cCamAppParam.iTargetBitRate
        , Get().cCamAppParam.iTargetBitRate
        , 512
        , 16*1024
        , "H.264�����������"
        , "��λ��Kbps��ǧ����ÿ�룩"
        , CUSTOM_LEVEL
    );
//�ڶ�·H264����
//��ʱ��֧�ֵ�������I֡���
/*
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
*/
	GetEnum("\\CamApp"
		, "ResolutionSecond"
		, &Get().cCamAppParam.iResolutionSecond
		, Get().cCamAppParam.iResolutionSecond
		, "0:1080P;1:720P;2:576P;3:480P"
		, "�ڶ�·H.264ͼ��ֱ���"
		, ""
		, CUSTOM_LEVEL
	);

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
//end

    GetInt("\\CamApp"
        , "AGCTh"
        , &Get().cCamAppParam.iAGCTh
        , Get().cCamAppParam.iAGCTh
        , 0
        , 255
        , "AGC����׼ֵ"
        , ""
        , CUSTOM_LEVEL
    );

    GetInt("\\CamApp"
        , "AGCShutterLOri"
        , &Get().cCamAppParam.iAGCShutterLOri
        , Get().cCamAppParam.iAGCShutterLOri
        , 0
        , 30000
        , "AGC���ŵ�������"
        , ""
        , CUSTOM_LEVEL
    );

    Get().cCamAppParam.iAGCShutterHOri = 10000;
    GetInt("\\CamApp"
        , "AGCShutterHOri"
        , &Get().cCamAppParam.iAGCShutterHOri
        , Get().cCamAppParam.iAGCShutterHOri
        , 0
        , 30000
        , "AGC���ŵ�������"
        , ""
        , CUSTOM_LEVEL
    );

    GetInt("\\CamApp"
        , "AGCGainLOri"
        , &Get().cCamAppParam.iAGCGainLOri
        , Get().cCamAppParam.iAGCGainLOri
        , 0
        , 480					//����185ǰ�����淶ΧΪ0~480
        , "AGC�����������"
        , ""
        , PROJECT_LEVEL
    );

    Get().cCamAppParam.iAGCGainHOri = 100;
    GetInt("\\CamApp"
        , "AGCGainHOri"
        , &Get().cCamAppParam.iAGCGainHOri
        , Get().cCamAppParam.iAGCGainHOri
        , 0
        , 480					//����185ǰ�����淶ΧΪ0~480
        , "AGC�����������"
        , ""
        , CUSTOM_LEVEL
    );

    GetEnum("\\CamApp"
        , "AGCScene"
        , &Get().cCamAppParam.iAGCScene
        , Get().cCamAppParam.iAGCScene
        , "0:�Զ�;1:ƫ��;2:��׼;3:����"
        , "�龰ģʽ"
        , ""
        , CUSTOM_LEVEL
    );

    GetInt("\\CamApp"
        , "GainR"
        , &Get().cCamAppParam.iGainR
        , Get().cCamAppParam.iGainR
        , 36
        , 255
        , "R����"
        , ""
        , CUSTOM_LEVEL
    );

    GetInt("\\CamApp"
        , "GainG"
        , &Get().cCamAppParam.iGainG
        , Get().cCamAppParam.iGainG
        , 36
        , 255
        , "G����"
        , ""
        , CUSTOM_LEVEL
    );

    GetInt("\\CamApp"
        , "GainB"
        , &Get().cCamAppParam.iGainB
        , Get().cCamAppParam.iGainB
        , 36
        , 255
        , "B����"
        , ""
        , CUSTOM_LEVEL
    );

    GetInt("\\CamApp"
        , "Gain"
        , &Get().cCamAppParam.iGain
        , Get().cCamAppParam.iGain
        , 0
        , 480					//����185ǰ���������Ϊ480
        , "����"
        , ""
        , CUSTOM_LEVEL
    );

    GetInt("\\CamApp"
        , "Shutter"
        , &Get().cCamAppParam.iShutter
        , Get().cCamAppParam.iShutter
        , 0
        , 30000
        , "����"
        , "��λ��us"
        , CUSTOM_LEVEL
    );

    GetEnum("\\CamApp"
        , "JpegAutoCompressEnable"
        , &Get().cCamAppParam.iJpegAutoCompressEnable
        , Get().cCamAppParam.iJpegAutoCompressEnable
        , "0:��ʹ��;1:ʹ��"
        , "Jpeg�Զ����ڿ���"
        , ""
        , CUSTOM_LEVEL
    );

    GetInt("\\CamApp"
        , "JpegExpectSize"
        , &Get().cCamAppParam.iJpegExpectSize
        , Get().cCamAppParam.iJpegExpectSize
        , 0
        , 1024*1024
        , "JpegͼƬ������С"
        , "��λ��Byte���ֽڣ�ע��Ϊ0���ʾ���Ըò���"
        , CUSTOM_LEVEL
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
        , 79
        , "Jpegѹ�����Զ���������"
        , ""
        , CUSTOM_LEVEL
    );

    Get().cCamAppParam.iJpegCompressRateH = 80;
    GetInt("\\CamApp"
        , "JpegCompressRateHigh"
        , &Get().cCamAppParam.iJpegCompressRateH
        , Get().cCamAppParam.iJpegCompressRateH
        , 1
        , 80
        , "Jpegѹ�����Զ���������"
        , ""
        , PROJECT_LEVEL 
    );

    Get().cCamAppParam.iAGCEnable = 1;
    GetEnum("\\CamApp"
        , "AGCEnable"
        , &Get().cCamAppParam.iAGCEnable
        , Get().cCamAppParam.iAGCEnable
        , "0:��ʹ��;1:ʹ��"
        , "AGCʹ��"
        , ""
        , CUSTOM_LEVEL
    );

    Get().cCamAppParam.iAWBEnable = 1;
    GetEnum("\\CamApp"
        , "AWBEnable"
        , &Get().cCamAppParam.iAWBEnable
        , Get().cCamAppParam.iAWBEnable
        , "0:��ʹ��;1:ʹ��"
        , "AWBʹ��"
        , ""
        , CUSTOM_LEVEL
    );

    //�ò��������ץ���йأ�����"TriggerOutEnable""�������ʹ�ܿ���"�йأ�����ȥ��
    GetEnum("\\CamApp"
        , "CaptureSynOutputType"
        , &Get().cCamAppParam.iCaptureSynOutputType
        , Get().cCamAppParam.iCaptureSynOutputType
        , "0:����(��ƽ);1:OC�����أ�"
        , "ץ���������"
        , ""
        , CUSTOM_LEVEL
        );

    GetEnum("\\CamApp"
        , "ExternSyncMode"
        , &Get().cCamAppParam.iExternalSyncMode
        , Get().cCamAppParam.iExternalSyncMode
        , "0:��ͬ��;1:�ڲ�����ͬ��"
        , "��ͬ��ģʽ"
        , ""
        , CUSTOM_LEVEL
        );
    GetInt("\\CamApp"
        , "SyncRelay"
        , &Get().cCamAppParam.iSyncRelay
        , Get().cCamAppParam.iSyncRelay
        , 0
        , 20000
        , "��ͬ���ź���ʱ"
        , "��λ:΢��"
        , CUSTOM_LEVEL
        );

    /*GetEnum("\\CamApp"
        , "CaptureEdge"
        , &Get().cCamAppParam.iCaptureEdge
        , Get().cCamAppParam.iCaptureEdge
        , "0:������;1:�ⲿ�½��ش���;2:�ⲿ�����ش���;3:�������½��ض�����"
        , "����ץ����"
        , ""
        , CUSTOM_LEVEL
    );*/

    char szAGC[255];
    //BOOL fAllAGCZoneIsOff = TRUE;
    for (int i = 0; i < 16; i++)
    {
        sprintf(szAGC, "AGC%d", i+1);
        GetEnum("\\CamApp\\AGCZone"
            , szAGC
            , &Get().cCamAppParam.rgiAGCZone[i]
            , Get().cCamAppParam.rgiAGCZone[i]
            , "0:�ر�;1:��"
            , ""
            , ""
            , CUSTOM_LEVEL
        );
        /*
        if (1 == Get().cCamAppParam.rgiAGCZone[i])
        {
            fAllAGCZoneIsOff = FALSE;
        }*/
    }
    //�ײ�������
    /*
    //����������ѡһ��
    if (TRUE == fAllAGCZoneIsOff)
    {
        Get().cCamAppParam.rgiAGCZone[0] = 1;
        UpdateInt("\\CamApp\\AGCZone"
            , "AGC0"
            , Get().cCamAppParam.rgiAGCZone[0]
            );
    }
    */

    GetEnum("\\CamApp"
        , "EnableDCAperture"
        , &Get().cCamAppParam.iEnableDCAperture
        , Get().cCamAppParam.iEnableDCAperture
        , "0:��ʹ��;1:ʹ��"
        , "ʹ��DC��Ȧ"
        , ""
        , CUSTOM_LEVEL
    );

    GetEnum("\\CamApp"
        , "ColorMatrixMode"
        , &Get().cCamAppParam.iColorMatrixMode
        , Get().cCamAppParam.iColorMatrixMode
        , "0:��ʹ��;1:ʹ��"
        , "ͼ����ǿ"
        , ""
        , CUSTOM_LEVEL
    );

    GetInt("\\CamApp"
        , "ContrastValue"
        , &Get().cCamAppParam.iContrastValue
        , Get().cCamAppParam.iContrastValue
        , -100
        , 100
        , "�Աȶ�"
        , ""
        , CUSTOM_LEVEL
    );

    GetInt("\\CamApp"
        , "SaturationValue"
        , &Get().cCamAppParam.iSaturationValue
        , Get().cCamAppParam.iSaturationValue
        , -100
        , 100
        , "���Ͷ�"
        , ""
        , CUSTOM_LEVEL
    );

    GetInt("\\CamApp"
        , "WDRThreshold"
        , &Get().cCamAppParam.iWDRValue
        , Get().cCamAppParam.iWDRValue
        , 0
        , 4095					//��̬��ΧΪ 0~4095
        , "WDRǿ��"
        , ""
        , CUSTOM_LEVEL
    );

    Get().cCamAppParam.iSNFTNFMode == 3;
    GetEnum("\\CamApp"
            , "NoiseReduction"
            , &Get().cCamAppParam.iSNFTNFMode
            , Get().cCamAppParam.iSNFTNFMode
            , "0:��ʹ��;1:TNFģʽ;2:SNFģʽ;3:TNF��SNFģʽ"
            , "����ģʽ"
            , ""
            , CUSTOM_LEVEL
            );

    Get().cCamAppParam.iTNFSNFValue = 0;
    GetEnum("\\CamApp"
            , "NoiseReductionThreshold"
            , &Get().cCamAppParam.iTNFSNFValue
            , Get().cCamAppParam.iTNFSNFValue
            , "0:�Զ�;1:��;2:��;3:��"
            , "����ǿ��"
            , ""
            , CUSTOM_LEVEL
            );

    GetEnum("\\CamApp"
        , "CVBSMode"
        , &Get().cCamAppParam.iCVBSMode
        , Get().cCamAppParam.iCVBSMode
        , "0:PAL;1:NTSC"
        , "CVBS��ʽ"
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
        , CUSTOM_LEVEL
    );
    //todoʹ��٤��ᵼ��ͼ��ƫɫ
    /*
    GetInt("\\CamApp"
        , "GammaValue"
        , &Get().cCamAppParam.iGammaValue
        , Get().cCamAppParam.iGammaValue
        , 10
        , 22
        , "���٤��"
        , ""
        , CUSTOM_LEVEL
    );
    GetEnum("\\CamApp"
        , "GammaEnable"
        , &Get().cCamAppParam.iGammaEnable
        , Get().cCamAppParam.iGammaEnable
        ,"0:��ʹ��;1:ʹ��"
        , "���٤��ʹ��"
        , ""
        , CUSTOM_LEVEL
    );
    */

    GetInt("\\CamApp"
        , "EdgeValue"
        , &Get().cCamAppParam.iEdgeValue
        , Get().cCamAppParam.iEdgeValue
        ,0
        ,255
        , "ͼ���Ե��ǿ"
        , "ͼ���Ե��ǿ"
        , CUSTOM_LEVEL
    );

    /*GetEnum("\\CamApp"
        , "LEDPolarity"
        , &Get().cCamAppParam.iLEDPolarity
        , Get().cCamAppParam.iLEDPolarity
        , "0:������;1:������"
        , "����ƿ��Ƽ���"
        , ""
        , CUSTOM_LEVEL
    );

    GetEnum("\\CamApp"
        , "LEDOutputType"
        , &Get().cCamAppParam.iLEDOutputType
        , Get().cCamAppParam.iLEDOutputType
        , "0:��������ƽ��;1:OC(����)"
        , "������������"
        , ""
        , CUSTOM_LEVEL
    );*/

    GetEnum("\\CamApp"
        , "ALMPolarity"
        , &Get().cCamAppParam.iALMPolarity
        , Get().cCamAppParam.iALMPolarity
        , "0:������;1:������"
        , "�������Ƽ���"
        , ""
        , PROJECT_LEVEL
    );
    GetEnum("\\CamApp"
        , "ALMOutputType"
        , &Get().cCamAppParam.iALMOutputType
        , Get().cCamAppParam.iALMOutputType
        , "0:��������ƽ��;1:OC(����)"
        , "�����������"
        , ""
        , PROJECT_LEVEL
    );
    GetEnum("\\CamApp"
        , "ColorGradation"
        , &Get().cCamAppParam.iColorGradation
        , Get().cCamAppParam.iColorGradation
        , "0:0~255;1:16~234"
        , "ɫ��"
        , "ɫ��"
        , CUSTOM_LEVEL
    );
	
	//Get().cCamAppParam.iCVBSExport = 1;
	GetEnum("\\CamApp"
		, "CVBSIMAGEMode"
		, &Get().cCamAppParam.iCVBSExport
		, Get().cCamAppParam.iCVBSExport
		, "0:CVBS ��С���;1:CVBS �������;2:�Զ���������"
		, "CVBSͼ��ģʽ"
		, ""
		, CUSTOM_LEVEL
	);
	if (Get().cCamAppParam.iCVBSExport != 0)
	{
		Get().nMCFWUsecase = IMX185_DUALVIDEO_CVBSEXPORT;
	}

    Get().cCamAppParam.iCVBSCropStartX = 600;
    GetInt("\\CamApp"
		, "CVBSCropStartX"
		, &Get().cCamAppParam.iCVBSCropStartX
		, Get().cCamAppParam.iCVBSCropStartX
		, 0
		, 1199
		, "CVBS�ü�ģʽ��ʼ������"
		, "CVBS�ü�ģʽ��ʼ������"
		, CUSTOM_LEVEL
	);

    Get().cCamAppParam.iCVBSCropStartY = 252;
    GetInt("\\CamApp"
		, "CVBSCropStartY"
		, &Get().cCamAppParam.iCVBSCropStartY
		, Get().cCamAppParam.iCVBSCropStartY
		, 0
		, 503
		, "CVBS�ü�ģʽ��ʼ������"
		, "CVBS�ü�ģʽ��ʼ������"
		, CUSTOM_LEVEL
	);


    return S_OK;
}

/**
*
*/
HRESULT SWLPRVenusCameraParameter::InitCharacter(VOID)
{
//��һ·H264�ַ�����
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

    GetInt("\\Overlay\\H264"
          , "Size"
          , &Get().cOverlay.cH264Info.iFontSize
          , 32
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
    GetEnum("\\Overlay\\H264"
          , "DateTime"
          , &Get().cOverlay.cH264Info.fEnableTime
          , Get().cOverlay.cH264Info.fEnableTime
          , "0:��ʹ��;1:ʹ��"
          , "����ʱ��"
          , ""
          , CUSTOM_LEVEL
      );

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
//�ڶ�·H264�ַ�����
	  
	GetEnum("\\Overlay\\H264Second"
          , "Enable"
          , &Get().cOverlay.fH264SecondEnable
          , Get().cOverlay.fH264SecondEnable
          , "0:��ʹ��;1:ʹ��"
          , "H264�ַ�����ʹ��"
          , ""
          , CUSTOM_LEVEL
    );

    GetInt("\\Overlay\\H264Second"
          , "X"
          , &Get().cOverlay.cH264SecondInfo.iTopX
          , Get().cOverlay.cH264SecondInfo.iTopX
          , 0
          , 1920
          , "X����"
          , ""
          , CUSTOM_LEVEL
      );

    GetInt("\\Overlay\\H264Second"
          , "Y"
          , &Get().cOverlay.cH264SecondInfo.iTopY
          , Get().cOverlay.cH264SecondInfo.iTopY
          , 0
          , 1080
          , "Y����"
          , ""
          , CUSTOM_LEVEL
      );

    GetInt("\\Overlay\\H264Second"
          , "Size"
          , &Get().cOverlay.cH264SecondInfo.iFontSize
          , 32
          , 16
          , 128
          , "�����С"
          , ""
          , CUSTOM_LEVEL
      );
   	dwR = 255, dwG = 0,dwB = 0;
    GetInt("\\Overlay\\H264Second"
          , "R"
          , (INT *)&dwR
          , dwR
          , 0
          , 255
          , "������ɫR����"
          , ""
          , CUSTOM_LEVEL
      );
    GetInt("\\Overlay\\H264Second"
          , "G"
          , (INT *)&dwG
          , dwG
          , 0
          , 255
          , "������ɫG����"
          , ""
          , CUSTOM_LEVEL
      );
    GetInt("\\Overlay\\H264Second"
          , "B"
          , (INT *)&dwB
          , dwB
          , 0
          , 255
          , "������ɫB����"
          , ""
          , CUSTOM_LEVEL
      );
    Get().cOverlay.cH264SecondInfo.dwColor = (dwB | (dwG << 8) | (dwR << 16));
    SW_TRACE_DEBUG("h264 second color[0x%08x][0x%02x,0x%02x,0x%02x]", 
		Get().cOverlay.cH264SecondInfo.dwColor, dwR, dwG, dwB);
    GetEnum("\\Overlay\\H264Second"
          , "DateTime"
          , &Get().cOverlay.cH264SecondInfo.fEnableTime
          , Get().cOverlay.cH264SecondInfo.fEnableTime
          , "0:��ʹ��;1:ʹ��"
          , "����ʱ��"
          , ""
          , CUSTOM_LEVEL
      );

     GetString("\\Overlay\\H264Second"
          , "String"
          , Get().cOverlay.cH264SecondInfo.szInfo
          , Get().cOverlay.cH264SecondInfo.szInfo
          , sizeof(Get().cOverlay.cH264SecondInfo.szInfo)
          , ""
          , "������Ϣ"
          , CUSTOM_LEVEL
      );
    if(!swpa_strcmp(Get().cOverlay.cH264SecondInfo.szInfo, "NULL"))
    {
      swpa_strcpy(Get().cOverlay.cH264SecondInfo.szInfo, "");
    }

    // todo.�ַ����Ӽ����ַ��ܳ��������⣬������ʱ��������ո���β�������������������ʾ��ȫ��
    if( strlen(Get().cOverlay.cH264SecondInfo.szInfo) + 3 < sizeof(Get().cOverlay.cH264SecondInfo.szInfo) )
    {
        strcpy(Get().cOverlay.cH264SecondInfo.szInfo + strlen(Get().cOverlay.cH264SecondInfo.szInfo), "  ");
        SW_TRACE_NORMAL("<param>change overlay h264 len.%s", Get().cOverlay.cH264SecondInfo.szInfo);
    }
//end

      GetEnum("\\Overlay\\JPEG"
          , "Enable"
          , &Get().cOverlay.cJPEGInfo.fEnable
          , Get().cOverlay.cJPEGInfo.fEnable
          , "0:��ʹ��;1:ʹ��"
          , "JPEG�ַ�����ʹ��"
          , ""
          , CUSTOM_LEVEL
      );

    GetInt("\\Overlay\\JPEG"
          , "X"
          , &Get().cOverlay.cJPEGInfo.iX
          , Get().cOverlay.cJPEGInfo.iX
          , 0
          , 1920
          , "X����"
          , ""
          , CUSTOM_LEVEL
      );

    GetInt("\\Overlay\\JPEG"
          , "Y"
          , &Get().cOverlay.cJPEGInfo.iY
          , Get().cOverlay.cJPEGInfo.iY
          , 0
          , 1080
          , "Y����"
          , ""
          , CUSTOM_LEVEL
      );

    GetInt("\\Overlay\\JPEG"
          , "Size"
          , &Get().cOverlay.cJPEGInfo.iFontSize
          , Get().cOverlay.cJPEGInfo.iFontSize
          , 16
          , 128
          , "�����С"
          , ""
          , CUSTOM_LEVEL
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
          , CUSTOM_LEVEL
      );
    GetInt("\\Overlay\\JPEG"
          , "G"
          , (INT *)&dwG
          , dwG
          , 0
          , 255
          , "������ɫG����"
          , ""
          , CUSTOM_LEVEL
      );
    GetInt("\\Overlay\\JPEG"
          , "B"
          , (INT *)&dwB
          , dwB
          , 0
          , 255
          , "������ɫB����"
          , ""
          , CUSTOM_LEVEL
      );
    Get().cOverlay.cJPEGInfo.iColor = (dwB | (dwG << 8) | (dwR << 16));
    SW_TRACE_DEBUG("jpeg color[0x%08x][0x%02x,0x%02x,0x%02x]", Get().cOverlay.cJPEGInfo.iColor, dwR, dwG, dwB);
    GetEnum("\\Overlay\\JPEG"
          , "DateTime"
          , &Get().cOverlay.cJPEGInfo.fEnableTime
          , Get().cOverlay.cJPEGInfo.fEnableTime
          , "0:��ʹ��;1:ʹ��"
          , "����ʱ��"
          , ""
          , CUSTOM_LEVEL
      );

     GetString("\\Overlay\\JPEG"
          , "String"
          , Get().cOverlay.cJPEGInfo.szInfo
          , Get().cOverlay.cJPEGInfo.szInfo
          , sizeof(Get().cOverlay.cJPEGInfo.szInfo)
          , ""
          , "������Ϣ"
          , CUSTOM_LEVEL
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


HRESULT SWLPRVenusCameraParameter::InitGB28181(VOID)
{
    return CSWLPRParameter::InitGB28181();
}


HRESULT SWLPRVenusCameraParameter::InitONVIF(VOID)
{

    SW_TRACE_NORMAL(" HRESULT SWLPRVenusCameraParameter::InitONVIF(VOID)");
	


	 GetEnum("\\ONVIF"
			 , "OnvifEnable"
			 , &Get().cOnvifParam.iOnvifEnable
			 , Get().cOnvifParam.iOnvifEnable
			 , "0:��ʹ��;1:ʹ��"
			 , "����"
			 , ""
			 , CUSTOM_LEVEL);
	 GetEnum("\\ONVIF"
			 , "AuthenticateEnable"
			 , &Get().cOnvifParam.iAuthenticateEnable
			 , Get().cOnvifParam.iAuthenticateEnable
			 , "0:��ʹ��;1:ʹ��"
			 , "��Ȩ"
			 , "ʹ��ʱ��Ҫ�û�������ܽ��в���"
			 , CUSTOM_LEVEL);

	 //Ϊ��ͳһ��RTSP�ļ�Ȩ��ONVIF�ı���һ��
	 Get().cRtspParam.iAuthenticateEnable = Get().cOnvifParam.iAuthenticateEnable;
	 Get().cRtspParam.iRTSPStreamNum = 2;
	 GetEnum("\\ONVIF"
			 , "AutoControlBitrateEnable"
			 , &Get().cRtspParam.iAutoControlBitrateEnable
			 , Get().cRtspParam.iAutoControlBitrateEnable
			 , "0:��ʹ��;1:ʹ��"
			 , "�Զ��������ʿ���"
			 , "���������ӵ�����Զ�����������Ӧ����"
			 , CUSTOM_LEVEL);


    SW_TRACE_NORMAL(" HRESULT SWLPRVenusCameraParameter::InitONVIF(VOID) End");
}


