#include "config.h"
#include "SWFC.h"
#include "SWLPRParameter.h"
#include "SWLPRApplication.h"

REGISTER_CLASS(CSWLPRParameter)
CSWLPRParameter::CSWLPRParameter()
{
	ResetParam();
}

CSWLPRParameter::~CSWLPRParameter()
{
}

void CSWLPRParameter::SetDefaultTracker()
{
	//DSP����Ĭ��ֵ
    Get().cTrackerCfgParam.fEnableVote = TRUE;
    Get().cTrackerCfgParam.fAlpha5 = FALSE;
	Get().cTrackerCfgParam.fAlpha6 = FALSE;
    Get().cTrackerCfgParam.fAlpha7 = FALSE;
    Get().cTrackerCfgParam.fEnableRecgCarColor = FALSE;
	Get().cTrackerCfgParam.fEnableFlashLight = FALSE;
    Get().cTrackerCfgParam.fltMinConfForOutput = 0.7f;
	Get().cTrackerCfgParam.nPlateEnhanceThreshold = 40;
	Get().cTrackerCfgParam.fProcessPlate_BlackPlate_Enable = FALSE;
	Get().cTrackerCfgParam.nProcessPlate_BlackPlate_S = 5;
	Get().cTrackerCfgParam.nProcessPlate_BlackPlate_L = 85;
	Get().cTrackerCfgParam.nProcessPlate_BlackPlateThreshold_H0 = 100;
	Get().cTrackerCfgParam.nProcessPlate_BlackPlateThreshold_H1 = 200;
    Get().cTrackerCfgParam.nProcessPlate_LightBlue = 0;
	Get().cTrackerCfgParam.nPlateDetect_Green = 0;
	Get().cTrackerCfgParam.nNightThreshold = 55;
	Get().cTrackerCfgParam.nPlateLightCheckCount = 5;
	Get().cTrackerCfgParam.nMinPlateBrightness = 80;
	Get().cTrackerCfgParam.nMaxPlateBrightness = 120;
	Get().cTrackerCfgParam.nCtrlCpl = 0;
	Get().cTrackerCfgParam.nLightTypeCpl = ((int)LIGHT_TYPE_COUNT / 2);
	Get().cTrackerCfgParam.nEnablePlateEnhance = 1;
	Get().cTrackerCfgParam.nPlateResizeThreshold = 120;
	Get().cTrackerCfgParam.nEnableBigPlate = 0;
	Get().cTrackerCfgParam.nFirstPlatePos = 0;
	Get().cTrackerCfgParam.fltPlateDetect_StepAdj = 1.5f;
	Get().cTrackerCfgParam.nMiddleWidth = 70;
	Get().cTrackerCfgParam.nBottomWidth = 90;
	Get().cTrackerCfgParam.iEddyType = 0;
    Get().cTrackerCfgParam.fltOverLineSensitivity = 2.75f;
    Get().cTrackerCfgParam.nRoadLineNumber = 3;
	Get().cTrackerCfgParam.nOutputRoadTypeName = 0;
    Get().cTrackerCfgParam.nCaptureOnePos = 50;
    Get().cTrackerCfgParam.nCaptureTwoPos = 80;
	Get().cTrackerCfgParam.nCaptureThreePos = 0;
    Get().cTrackerCfgParam.cActionDetect.iDetectCarStopTime = 10;
    Get().cTrackerCfgParam.nDetReverseRunEnable = 0;
	Get().cTrackerCfgParam.nSpan = 5;
	Get().cTrackerCfgParam.iRoadNumberBegin = 0;
    Get().cTrackerCfgParam.iStartRoadNum = 1;
	Get().cTrackerCfgParam.nFlashLightThreshold = 25;
	Get().cTrackerCfgParam.fDoublePlateEnable = 1;
	Get().cTrackerCfgParam.nRecogAsteriskThreshold = 0;
	Get().cTrackerCfgParam.fUseEdgeMethod = FALSE;
	Get().cTrackerCfgParam.fEnableT1Model = FALSE;
	Get().cTrackerCfgParam.fEnableDefaultWJ = TRUE;
	Get().cTrackerCfgParam.nRecogGxPolice = 1;
	Get().cTrackerCfgParam.fEnableAlpha_5 = FALSE;
    Get().cTrackerCfgParam.nCarArrivedPos = 60;
	Get().cTrackerCfgParam.nCarArrivedPosNoPlate = 55;
	Get().cTrackerCfgParam.nCarArrivedPosYellow = 70;
	Get().cTrackerCfgParam.nProcessType = 0;
	Get().cTrackerCfgParam.nOutPutType = 0;
	Get().cTrackerCfgParam.nNightPlus = 0;
    Get().cTrackerCfgParam.nWalkManSpeed = 200;
	Get().cTrackerCfgParam.fltBikeSensitivity = 4.0f;
	Get().cTrackerCfgParam.nCarArrivedDelay = 0;
	Get().cTrackerCfgParam.nVoteFrameNum = 35;
    Get().cTrackerCfgParam.nMaxEqualFrameNumForVote = 15;
    Get().cTrackerCfgParam.nBlockTwinsTimeout = 60;
	Get().cTrackerCfgParam.nFirstConfidenceQuan = 0;
	Get().cTrackerCfgParam.nRemoveLowConfForVote = 40;
	Get().cTrackerCfgParam.nAverageConfidenceQuan = 0;
	Get().cTrackerCfgParam.fOutputCarSize = FALSE;
	swpa_memset(Get().cTrackerCfgParam.szDefaultWJChar, 0, 4);
    swpa_memcpy(Get().cTrackerCfgParam.szDefaultWJChar, "16", 2);
	Get().cTrackerCfgParam.nVoteCount = 3;
	Get().cTrackerCfgParam.fOnePlateMode = TRUE;
	Get().cTrackerCfgParam.nSegHeight = 18;
	Get().cTrackerCfgParam.nDoublePlateTypeEnable = 0;
	Get().cTrackerCfgParam.nDefaultDBType = 0;
	Get().cTrackerCfgParam.nSegWhitePlate = 0;
    Get().cTrackerCfgParam.nCarArriveTrig = Get().cCamCfgParam.iDynamicTriggerEnable;
	Get().cTrackerCfgParam.nTriggerPlateType = 0;
	Get().cTrackerCfgParam.nEnableProcessBWPlate = 0;
	Get().cTrackerCfgParam.iPlatform = 0;
    Get().cTrackerCfgParam.iSpeedLimit = Get().cResultSenderParam.iSpeedLimit;
	Get().cTrackerCfgParam.iCapMode = 0;
	Get().cTrackerCfgParam.iOutPutNoPlate = 0;
	Get().cTrackerCfgParam.iPrevTime = 5000;
	Get().cTrackerCfgParam.iPostTime = 0;
	Get().cTrackerCfgParam.fPreVote = TRUE;
	Get().cTrackerCfgParam.fEnableDetFace = FALSE;
	Get().cTrackerCfgParam.nMinFaceScale = 2;
	Get().cTrackerCfgParam.nMaxFaceScale = 10;
	Get().cTrackerCfgParam.iForceLightOffAtDay = 0;
	Get().cTrackerCfgParam.iForceLightOffAtNight = 0;
	Get().cTrackerCfgParam.iDisableFlashAtDay = 0;
	Get().cTrackerCfgParam.iForceLightThreshold = 100;

    //Ĭ�ϳ�����
    Get().cTrackerCfgParam.rgcRoadInfo[0].ptTop.x = 1303;
    Get().cTrackerCfgParam.rgcRoadInfo[0].ptTop.y = 0;
    Get().cTrackerCfgParam.rgcRoadInfo[0].ptBottom.x = 5;
    Get().cTrackerCfgParam.rgcRoadInfo[0].ptBottom.y = 1761;

    Get().cTrackerCfgParam.rgcRoadInfo[1].ptTop.x = 1551;
    Get().cTrackerCfgParam.rgcRoadInfo[1].ptTop.y = 0;
    Get().cTrackerCfgParam.rgcRoadInfo[1].ptBottom.x = 1046;
    Get().cTrackerCfgParam.rgcRoadInfo[1].ptBottom.y = 1968;

    Get().cTrackerCfgParam.rgcRoadInfo[2].ptTop.x = 1816;
    Get().cTrackerCfgParam.rgcRoadInfo[2].ptTop.y = 0;
    Get().cTrackerCfgParam.rgcRoadInfo[2].ptBottom.x = 2269;
    Get().cTrackerCfgParam.rgcRoadInfo[2].ptBottom.y = 1986;

    Get().cTrackerCfgParam.rgcRoadInfo[3].ptTop.x = 2091;
    Get().cTrackerCfgParam.rgcRoadInfo[3].ptTop.y = 0;
    Get().cTrackerCfgParam.rgcRoadInfo[3].ptBottom.x = 3385;
    Get().cTrackerCfgParam.rgcRoadInfo[3].ptBottom.y = 1709;

	//ɨ������Ĭ��ֵ
    Get().cTrackerCfgParam.cDetectArea.fEnableDetAreaCtrl = TRUE;
	Get().cTrackerCfgParam.cDetectArea.DetectorAreaLeft = 0;
	Get().cTrackerCfgParam.cDetectArea.DetectorAreaRight = 100;
    Get().cTrackerCfgParam.cDetectArea.DetectorAreaTop = 15;
    Get().cTrackerCfgParam.cDetectArea.DetectorAreaBottom = 55;
    Get().cTrackerCfgParam.cDetectArea.nDetectorMinScaleNum = 5;
    Get().cTrackerCfgParam.cDetectArea.nDetectorMaxScaleNum = 9;

	//����ɨ������Ĭ��ֵ
    Get().cTrackerCfgParam.cTrapArea.fEnableDetAreaCtrl = TRUE;
    //��ʼ����������
    Get().cTrackerCfgParam.cTrapArea.TopLeftX =     Get().cTrackerCfgParam.cDetectArea.DetectorAreaLeft;
    Get().cTrackerCfgParam.cTrapArea.TopRightX =    Get().cTrackerCfgParam.cDetectArea.DetectorAreaRight;
    Get().cTrackerCfgParam.cTrapArea.BottomLeftX =  Get().cTrackerCfgParam.cDetectArea.DetectorAreaLeft;
    Get().cTrackerCfgParam.cTrapArea.BottomRightX = Get().cTrackerCfgParam.cDetectArea.DetectorAreaRight;

    Get().cTrackerCfgParam.cTrapArea.TopLeftY =     Get().cTrackerCfgParam.cDetectArea.DetectorAreaTop;
    Get().cTrackerCfgParam.cTrapArea.TopRightY =    Get().cTrackerCfgParam.cDetectArea.DetectorAreaTop;
    Get().cTrackerCfgParam.cTrapArea.BottomLeftY =  Get().cTrackerCfgParam.cDetectArea.DetectorAreaBottom;
    Get().cTrackerCfgParam.cTrapArea.BottomRightY = Get().cTrackerCfgParam.cDetectArea.DetectorAreaBottom;

    Get().cTrackerCfgParam.cTrapArea.nDetectorMinScaleNum = Get().cTrackerCfgParam.cDetectArea.nDetectorMinScaleNum;
    Get().cTrackerCfgParam.cTrapArea.nDetectorMaxScaleNum = Get().cTrackerCfgParam.cDetectArea.nDetectorMaxScaleNum;

	// ץ��ͼʶ�����  
    Get().cTrackerCfgParam.cRecogSnapArea.fEnableDetAreaCtrl = TRUE;
    Get().cTrackerCfgParam.cRecogSnapArea.DetectorAreaLeft = Get().cMatchParam.iDetectorAreaLeft;
    Get().cTrackerCfgParam.cRecogSnapArea.DetectorAreaTop = Get().cMatchParam.iDetectorAreaTop;
    Get().cTrackerCfgParam.cRecogSnapArea.DetectorAreaRight = Get().cMatchParam.iDetectorAreaRight;
    Get().cTrackerCfgParam.cRecogSnapArea.DetectorAreaBottom = Get().cMatchParam.iDetectorAreaBottom;
    Get().cTrackerCfgParam.cRecogSnapArea.nDetectorMinScaleNum = Get().cMatchParam.iDetectorMinScaleNum;
    Get().cTrackerCfgParam.cRecogSnapArea.nDetectorMaxScaleNum = Get().cMatchParam.iDetectorMaxScaleNum;

	//���̵�
	Get().cTrackerCfgParam.cTrafficLight.nCheckType = 0;
	Get().cTrackerCfgParam.cTrafficLight.nAutoScanLight = 1;
	Get().cTrackerCfgParam.cTrafficLight.nSenceCheckMode = 0;
	Get().cTrackerCfgParam.cTrafficLight.nStopLinePos = 65;
	Get().cTrackerCfgParam.cTrafficLight.nLeftStopLinePos = 65;
	Get().cTrackerCfgParam.cTrafficLight.nMatchScanSpeed = 0;
	Get().cTrackerCfgParam.cTrafficLight.nLightCount = 0;
	Get().cTrackerCfgParam.cTrafficLight.nValidSceneStatusCount = 5;
	Get().cTrackerCfgParam.cTrafficLight.nOutputInLine = 1;
	Get().cTrackerCfgParam.cTrafficLight.nRedLightDelay = 0;
	Get().cTrackerCfgParam.cTrafficLight.nSpecificLeft = 0;
	Get().cTrackerCfgParam.cTrafficLight.nRunTypeEnable = 0;
	Get().cTrackerCfgParam.cTrafficLight.nSceneCount = 0;
	Get().cTrackerCfgParam.cTrafficLight.nTrafficLightThread = 3;
	Get().cTrackerCfgParam.cTrafficLight.nTrafficLightBrightValue = 3;
	Get().cTrackerCfgParam.cTrafficLight.nAutoScanThread = 10;
	Get().cTrackerCfgParam.cTrafficLight.nSkipFrameNO = 30;
	Get().cTrackerCfgParam.cTrafficLight.fIsLightAdhesion = FALSE;
	Get().cTrackerCfgParam.cTrafficLight.fDeleteLightEdge = FALSE;
	Get().cTrackerCfgParam.cTrafficLight.fFilterNoPlatePeccancy = FALSE;

	for (int i = 0; i < MAX_TRAFFICLIGHT_COUNT; i++)
	{
		swpa_strcpy(Get().cTrackerCfgParam.cTrafficLight.rgszLightPos[i], "(0,0,0,0),0,0,00");
	}	
	for (int i = 0; i < MAX_SCENE_COUNT; i++)
	{
		swpa_strcpy(Get().cTrackerCfgParam.cTrafficLight.rgszScene[i], "(0,0,0,0),0");
	}

	Get().cTrackerCfgParam.cTrafficLight.nFilterRushPeccancy = 0;
	Get().cTrackerCfgParam.cTrafficLight.nFilterMoreReview = 0;
	Get().cTrackerCfgParam.cTrafficLight.fltHMThreld = 48;       //������ֵ
	Get().cTrackerCfgParam.cTrafficLight.fltHMThreldNight = 48;  //ҹ����ֵ
	//ģ��ƥ��ֱ��ͼ�ӳٸ���֡��
	Get().cTrackerCfgParam.cTrafficLight.iHMDelay = 7;             //�����ӳ�֡��
	Get().cTrackerCfgParam.cTrafficLight.iHMDelayNight = 7;        //ҹ���ӳ�֡��
	Get().cTrackerCfgParam.cTrafficLight.iMaxMatchDiff = 18;
	Get().cTrackerCfgParam.cTrafficLight.iRealForward = 40;
	strcpy(Get().cTrackerCfgParam.cTrafficLight.szIOConfig, "00,00,00,00,00,00,00,00");
	Get().cTrackerCfgParam.cTrafficLight.fEnhanceRedLight = TRUE;

	//������ٲ���
	Get().cTrackerCfgParam.cScaleSpeed.fEnable = FALSE;
	Get().cTrackerCfgParam.cScaleSpeed.nMethodsFlag = 0;
	Get().cTrackerCfgParam.cScaleSpeed.fltLeftDistance = 25.0f;
	Get().cTrackerCfgParam.cScaleSpeed.fltFullDistance = 75.0f;
	Get().cTrackerCfgParam.cScaleSpeed.fltCameraHigh = 6.5f;
    Get().cTrackerCfgParam.cScaleSpeed.fltAdjustCoef = 1.050f;
    Get().cTrackerCfgParam.cScaleSpeed.fltDistance = 90;
    Get().cTrackerCfgParam.cScaleSpeed.fltRoadWidth = 3.75;
	Get().cTrackerCfgParam.cScaleSpeed.rgfltTransMarix[0] = 165.472f;
	Get().cTrackerCfgParam.cScaleSpeed.rgfltTransMarix[1] = -38.447f;
	Get().cTrackerCfgParam.cScaleSpeed.rgfltTransMarix[2] = -4.968f;
	Get().cTrackerCfgParam.cScaleSpeed.rgfltTransMarix[3] = 606.269f;
	Get().cTrackerCfgParam.cScaleSpeed.rgfltTransMarix[4] = -2.743f;
	Get().cTrackerCfgParam.cScaleSpeed.rgfltTransMarix[5] = 1.813f;
	Get().cTrackerCfgParam.cScaleSpeed.rgfltTransMarix[6] = -177.545f;
	Get().cTrackerCfgParam.cScaleSpeed.rgfltTransMarix[7] = 1089.211f;
	Get().cTrackerCfgParam.cScaleSpeed.rgfltTransMarix[8] = 0.000f;
	Get().cTrackerCfgParam.cScaleSpeed.rgfltTransMarix[9] = -0.035f;
	Get().cTrackerCfgParam.cScaleSpeed.rgfltTransMarix[10] = -0.005f;

    // �µ�������ٲ���
    Get().cTrackerCfgParam.cScaleSpeed.fEnable = TRUE;
    Get().cTrackerCfgParam.cScaleSpeed.fltDistance = 90;
    Get().cTrackerCfgParam.cScaleSpeed.fltRoadWidth = 3.75;

	//��Ƶ������
	Get().cTrackerCfgParam.cVideoDet.nVideoDetMode = USE_VIDEODET;
	Get().cTrackerCfgParam.cVideoDet.nDayBackgroudThreshold = 20;
    Get().cTrackerCfgParam.cVideoDet.nNightBackgroudThreshold = 28;
	Get().cTrackerCfgParam.cVideoDet.nVoteFrameCount = 30;
	Get().cTrackerCfgParam.cVideoDet.nCheckAcross = 0;
	Get().cTrackerCfgParam.cVideoDet.nShadowDetMode = 0;
	Get().cTrackerCfgParam.cVideoDet.nShadowDetEffect = 0;
	Get().cTrackerCfgParam.cVideoDet.nDiffType = 0;
	Get().cTrackerCfgParam.cVideoDet.nMedFilter = 0;
	Get().cTrackerCfgParam.cVideoDet.nMedFilterUpLine = 15;
	Get().cTrackerCfgParam.cVideoDet.nMedFilterDownLine = 100;
	Get().cTrackerCfgParam.cVideoDet.rcVideoDetArea.left = 0;
	Get().cTrackerCfgParam.cVideoDet.rcVideoDetArea.top = 0;
	Get().cTrackerCfgParam.cVideoDet.rcVideoDetArea.right = 100;
	Get().cTrackerCfgParam.cVideoDet.rcVideoDetArea.bottom = 100;
	Get().cTrackerCfgParam.cVideoDet.nBigCarDelayFrameNo = 32;
	Get().cTrackerCfgParam.cVideoDet.iEPOutPutDetectLine = 63;         //�羯��⵽����ģ�齻����
	Get().cTrackerCfgParam.cVideoDet.iEPFGDetectLine = 45;             //���Ƴ������������(���붥���ٷֱ�)
	Get().cTrackerCfgParam.cVideoDet.iNoPlateDetMode = 0;
	Get().cTrackerCfgParam.cVideoDet.iEPUseCarTailModel = 0;
	Get().cTrackerCfgParam.cVideoDet.iEPNoPlateScoreThreld = 300;

	// Ĭ�Ϻ�ƼӺ췽ʽ
	Get().cTrackerCfgParam.cTrafficLight.fEnhanceRedLight = FALSE;

}

void CSWLPRParameter::SetDefaultCamApp()
{
	//�����ز���

	Get().cCamAppParam.iJpegStream = 1;
	Get().cCamAppParam.iH264Stream = 0;

	Get().cCamAppParam.iIFrameInterval = 10;
    Get().cCamAppParam.iTargetBitRate = 2 * 1024; //Kbps
    Get().cCamAppParam.iMaxBitRate = 4 * 1024;  //16 * 1024 Kbps
	Get().cCamAppParam.iFrameRate = 12;
    Get().cCamAppParam.iResolution = 0;
	Get().cCamAppParam.iRateControl = 0;       //0:VBR 1:CBR
	Get().cCamAppParam.iVbrDuration = 0;
	Get().cCamAppParam.iVbrSensivitity = 0;

	Get().cCamAppParam.iIFrameIntervalSecond = 10;	  /**< �ڶ�·H.264�����е�I֡��� */
	Get().cCamAppParam.iTargetBitRateSecond = 6*1024;	  /**< �ڶ�·H.264���������� */
	Get().cCamAppParam.iMaxBitRateSecond = 16 * 1024;
	Get().cCamAppParam.iFrameRateSecond = 25;		  /**< �ڶ�·H.264����֡�� */
	Get().cCamAppParam.iResolutionSecond = 0;		  /**< �ڶ�·H.264ͼ��ֱ��� */
	Get().cCamAppParam.iRateControlSecond = 0;
	Get().cCamAppParam.iVbrDurationSecond = 0;
	Get().cCamAppParam.iVbrSensivititySecond = 0;

	Get().cCamAppParam.iAGCTh = 100;
    Get().cCamAppParam.iAGCShutterLOri = 100;
    Get().cCamAppParam.iAGCShutterHOri = 3000;
    Get().cCamAppParam.iAGCGainLOri = 10;
    Get().cCamAppParam.iAGCGainHOri = 150;
	Get().cCamAppParam.iAGCScene = 0;
    Get().cCamAppParam.iAGCDayNightShutterControl = 0;
    Get().cCamAppParam.iAGCNightShutterHOri = 3500;

    Get().cCamAppParam.iGainR = 36;
    Get().cCamAppParam.iGainG = 36;
    Get().cCamAppParam.iGainB = 36;
	Get().cCamAppParam.iGain = 70;
	Get().cCamAppParam.iShutter = 2000;

	Get().cCamAppParam.iIris = 5;
	Get().cCamAppParam.iAutoFocus = 2;
	Get().cCamAppParam.iAWBMode = 0;
	Get().cCamAppParam.iAEMode = 0;
	Get().cCamAppParam.iWDREnable = 0;
	Get().cCamAppParam.iNRLevel = 0;

    Get().cCamAppParam.iCaptureGainR = 46;
	Get().cCamAppParam.iCaptureGainG = 36;
    Get().cCamAppParam.iCaptureGainB = 54;
    Get().cCamAppParam.iCaptureGain = 80;
    Get().cCamAppParam.iCaptureShutter = 1000;
    Get().cCamAppParam.iCaptureSharpenThreshold = 200;
    Get().cCamAppParam.iCaptureAutoParamEnable = 0;
    Get().cCamAppParam.iEnableCaptureGainRGB = 0;
    Get().cCamAppParam.iEnableCaptureGain = 1;
    Get().cCamAppParam.iEnableCaptureShutter = 1;
    Get().cCamAppParam.iEnableCaptureSharpen = 1;

	Get().cCamAppParam.iJpegCompressRate = 60;
	Get().cCamAppParam.iJpegCompressRateCapture = 80;


	strcpy(Get().cCamAppParam.szNFSParam, "-o timeo=1,soft,tcp,nolock,retry=1");
	memset(Get().cCamAppParam.szSafeSaverInitStr, 0, sizeof(Get().cCamAppParam.szSafeSaverInitStr));

    Get().cCamAppParam.iJpegExpectSize = 300;
	Get().cCamAppParam.iJpegCompressRateL = 30;
    Get().cCamAppParam.iJpegCompressRateH = 80;

	Get().cCamAppParam.iJpegType = 0;
    Get().cCamAppParam.iAGCEnable = 1;
	Get().cCamAppParam.iAWBEnable = 1;
	Get().cCamAppParam.iFlashRateSynSignalEnable = 0;
	Get().cCamAppParam.iFlashRateMultiplication = 1;
	Get().cCamAppParam.iFlashRatePolarity = 1;
	Get().cCamAppParam.iFlashRatePulseWidth = 4000;
	Get().cCamAppParam.iFlashRateOutputType = 0;
	Get().cCamAppParam.iCaptureSynPolarity = 1;			
	Get().cCamAppParam.iCaptureSynPulseWidth = 1000;		
	Get().cCamAppParam.iCaptureSynOutputType = 1;			
	Get().cCamAppParam.iCaptureSynCoupling = 0;			
	Get().cCamAppParam.iCaptureSynSignalEnable = 1;
	Get().cCamAppParam.iCaptureEdge = 0;
	Get().cCamAppParam.iFlashDifferentLane = 0;
	Get().cCamAppParam.iFlashDifferentLaneExt = 0;
	Get().cCamAppParam.iEnableDCAperture = 0;
	Get().cCamAppParam.iEnableSharpen = 0;
	Get().cCamAppParam.iSharpenCapture = 1;
	Get().cCamAppParam.iSharpenThreshold = 7;
	Get().cCamAppParam.iColorMatrixMode = 1;
	Get().cCamAppParam.iContrastValue = 0;
	Get().cCamAppParam.iSaturationValue = 8;
	Get().cCamAppParam.iTemThreshold = 32;
	Get().cCamAppParam.iHueThreshold = 0;
	Get().cCamAppParam.iHazeRemovalMode = 1;
	Get().cCamAppParam.iHazeRemovalThreshold = 0;

	//strcpy(szNetDiskIP, "172.18.10.10");
	strcpy(Get().cCamAppParam.szFtpRoot, "/array1");
	strcpy(Get().cCamAppParam.szNetDiskUser, "No User");
	strcpy(Get().cCamAppParam.szNetDiskPwd, "123");
	//strcpy(szNFS, "/volume1/nfs");
	//iNetDiskSpace = 1;

	Get().cCamAppParam.iAvgY = 30;

	Get().cCamAppParam.iGammaMode = 2;
	Get().cCamAppParam.rgiGamma[0][0] = 32;
	Get().cCamAppParam.rgiGamma[0][1] = 32;

	Get().cCamAppParam.rgiGamma[1][0] = 64;
	Get().cCamAppParam.rgiGamma[1][1] = 64;

	Get().cCamAppParam.rgiGamma[2][0] = 96;
	Get().cCamAppParam.rgiGamma[2][1] = 96;

	Get().cCamAppParam.rgiGamma[3][0] = 128;
	Get().cCamAppParam.rgiGamma[3][1] = 128;

	Get().cCamAppParam.rgiGamma[4][0] = 160;
	Get().cCamAppParam.rgiGamma[4][1] = 160;

	Get().cCamAppParam.rgiGamma[5][0] = 192;
	Get().cCamAppParam.rgiGamma[5][1] = 192;

	Get().cCamAppParam.rgiGamma[6][0] = 222;
	Get().cCamAppParam.rgiGamma[6][1] = 222;

	Get().cCamAppParam.rgiGamma[7][0] = 255;
	Get().cCamAppParam.rgiGamma[7][1] = 255;

	memcpy(Get().cCamAppParam.rgiDefGamma, Get().cCamAppParam.rgiGamma, 64);
	for(INT i = 0; i < 16; i++)
	{
	Get().cCamAppParam.rgiAGCZone[i] = 1;
	}
	// swpa_memset(rgiTrafficLigtZone,0,sizeof(rgiTrafficLigtZone));
	Get().cCamAppParam.iSaturationEnable = 0;
	Get().cCamAppParam.iContrastEnalbe = 0;
	//iSharpenEnable = 0;
	Get().cCamAppParam.iWDRValue = 256;		//Ĭ��Ϊ256
	Get().cCamAppParam.iSNFTNFMode = 0;
    Get().cCamAppParam.iTNFSNFValue = 1;
	Get().cCamAppParam.iGrayImageEnable = 0;
	Get().cCamAppParam.iCVBSMode = 0;                     /** CVBS��ʽ ��Χ:0~1   0��PAL��1��NTSC*/
    Get().cCamAppParam.iJpegAutoCompressEnable = 1;        /** Jpeg�Զ����ڿ��� */
	Get().cCamAppParam.iGammaValue = 10;
	Get().cCamAppParam.iGammaEnable = 0;
	Get().cCamAppParam.iEdgeValue = 0x80;

	Get().cCamAppParam.iX = 0;
	Get().cCamAppParam.iY = 0;
	Get().cCamAppParam.iFontSize = 32;             // ע��Ĭ����32����
	Get().cCamAppParam.iDateFormat = 0;
	Get().cCamAppParam.iFontColor = 0x00ff0000;    // ע����ʱ��Ĭ��ֵ��Ϊ��ɫ(0x00ff0000)
	strcpy(Get().cCamAppParam.szCharater, "");
	Get().cCamAppParam.iEnableCharacterOverlap = 0;
	Get().cCamAppParam.iEnableFixedLight = 0;      // ע��Ĭ������������泡�����ȶ��ı�

    Get().cCamAppParam.fEnableH264BrightnessAdjust = FALSE;
	Get().cCamAppParam.iAdjustPointX = 10;
	Get().cCamAppParam.iAdjustPointY = 25;
	Get().cCamAppParam.iEnableCCDSignalCheck = 1;

	Get().cCamAppParam.iExternalSyncMode = 1;
	Get().cCamAppParam.iSyncRelay = 2200;

    Get().cCamAppParam.iLEDPolarity = 0;
	Get().cCamAppParam.iLEDOutputType = 0;
	Get().cCamAppParam.iLEDPluseWidth = 5;

	Get().cCamAppParam.iALMPolarity = 1;
	Get().cCamAppParam.iALMOutputType = 0;

    Get().cCamAppParam.iF1OutputType = 1;

    Get().cCamAppParam.iFlashEnable		= 1;
	Get().cCamAppParam.iFlashPolarity		= 0;
    Get().cCamAppParam.iFlashOutputType	= 1;
	Get().cCamAppParam.iFlashCoupling  	= 0;
    Get().cCamAppParam.iFlashPluseWidth	= 50;

	Get().cCamAppParam.iColorGradation = 0;

	Get().cCamAppParam.iCVBSExport = 1;
	Get().cCamAppParam.iCVBSCropStartX = 600;
	Get().cCamAppParam.iCVBSCropStartY = 252;

    Get().cCamAppParam.iAWBWorkMode = 2;
	Get().cCamAppParam.iAutoControlCammeraAll = 1;
}

void CSWLPRParameter::SetDefaultCharacter()
{
	memset(&Get().cOverlay, 0, sizeof(Get().cOverlay));
    Get().cOverlay.cJPEGInfo.iColor = 0x00FF0000;
    Get().cOverlay.cJPEGInfo.iFontSize = 50;
 
	Get().cOverlay.cH264Info.iFontSize = 32;
	Get().cOverlay.cH264SecondInfo.iFontSize = 32;

    swpa_strcpy(Get().cOverlay.cH264Info.szInfo, "");
	swpa_strcpy(Get().cOverlay.cH264SecondInfo.szInfo, "�ڶ�·H264���ַ���������");
    swpa_strcpy(Get().cOverlay.cJPEGInfo.szInfo, "");
}

void CSWLPRParameter::SetDefaultOnvif()
{
	Get().cOnvifParam.iOnvifEnable = 1;
	Get().cOnvifParam.iAuthenticateEnable = 0;

	
	Get().cRtspParam.iRTSPStreamNum			= 2;				
	Get().cRtspParam.rgCommunicationMode[0]	= 0;
	Get().cRtspParam.rgCommunicationMode[1]	= 0;				
	Get().cRtspParam.iAuthenticateEnable		= 0;				
    Get().cRtspParam.iAutoControlBitrateEnable = 0;
	Get().cRtspParam.iRTSPServerPort			= 554;				
	strcpy(Get().cRtspParam.rgStreamName[0], "h264ESVideoTest");
	strcpy(Get().cRtspParam.rgStreamName[1], "h264ESVideoTestSecond");
}

void CSWLPRParameter::SetDefaultGB28181()
{
	swpa_memset(&Get().cGb28181, 0, sizeof(Get().cGb28181));
	Get().cGb28181.fEnalbe = FALSE;
	Get().cGb28181.fVideoSaveEnable = FALSE;

	swpa_strcpy(Get().cGb28181.szServerID,"34020000002000000001");
	swpa_strcpy(Get().cGb28181.szIPC_ID,"34020000001320000001");
	swpa_strcpy(Get().cGb28181.szServerIP,"172.18.11.123");
	swpa_strcpy(Get().cGb28181.szAlarmID,"34020000001340000001");
	swpa_strcpy(Get().cGb28181.szPassWord,"12345678");
	Get().cGb28181.iServerPort = 5060;
	Get().cGb28181.iIPC_Port = 5060;
	Get().cGb28181.iIPC_VideoPort = 5080;

	swpa_strcpy(Get().cGb28181.szLongitude,"0.0");
	swpa_strcpy(Get().cGb28181.szLatitude,"0.0");

	swpa_strcpy(Get().cGb28181.szManufacturer,"NULL");
	swpa_strcpy(Get().cGb28181.szBlock,"NULL");
	swpa_strcpy(Get().cGb28181.szAddress,"NULL");
	swpa_strcpy(Get().cGb28181.szOwner,"NULL");
	swpa_strcpy(Get().cGb28181.szCivilCode,"NULL");

}


void CSWLPRParameter::SetDefaultFrontPannel()
{
	Get().cFrontPannelParam.iUsedAutoControllMode = 0;
	Get().cFrontPannelParam.iAutoRunFlag = 1;
	Get().cFrontPannelParam.iPulseWidthMin = 5;
	Get().cFrontPannelParam.iPulseWidthMax = 42;
	Get().cFrontPannelParam.iPulseStep = 5;
	Get().cFrontPannelParam.iPulseLevel = 0;
	Get().cFrontPannelParam.iPolarizingPrismMode = -1;
	Get().cFrontPannelParam.iPalanceLightStatus = 0;
	Get().cFrontPannelParam.iFlash1PulseWidth = 100;
	Get().cFrontPannelParam.iFlash1Polarity = 0;
	Get().cFrontPannelParam.iFlash1Coupling = 0;
	Get().cFrontPannelParam.iFlash1ResistorMode = 0;
	Get().cFrontPannelParam.iFlash2PulseWidth = 100;
	Get().cFrontPannelParam.iFlash2Polarity = 0;
	Get().cFrontPannelParam.iFlash2Coupling = 0;
	Get().cFrontPannelParam.iFlash2ResistorMode = 0;
	Get().cFrontPannelParam.iFlash3PulseWidth = 100;
	Get().cFrontPannelParam.iFlash3Polarity = 0;
	Get().cFrontPannelParam.iFlash3Coupling = 0;
	Get().cFrontPannelParam.iFlash3ResistorMode = 0;
	Get().cFrontPannelParam.iDivFrequency = 1;
	Get().cFrontPannelParam.iOutPutDelay = 0;
}


void CSWLPRParameter::SetDefaultCamCfgParam()
{
	sprintf(Get().cCamCfgParam.szIP, "172.18.23.42");     
	Get().cCamCfgParam.iCamType = 0;
	Get().cCamCfgParam.iCamPort = 8888;
    Get().cCamCfgParam.iDynamicCfgEnable = 1;
    Get().cCamCfgParam.iDynamicTriggerEnable =1;
	Get().cCamCfgParam.iTriggerDelay = 0;
	Get().cCamCfgParam.iTriggerPlateType = 0;
	Get().cCamCfgParam.iDynamicCfgEnableOnTrigger = 0;
    Get().cCamCfgParam.iNightShutterThreshold = -1;// �˲���������ʹ�ã���˽�Ĭ��ֵ�������
    Get().cCamCfgParam.iNightAvgYThreshold = 10;
	Get().cCamCfgParam.iJpegBufferCount = 1;
    Get().cCamCfgParam.iEnableAGC = 1;
    Get().cCamCfgParam.iAutoParamEnable = 1;
	Get().cCamCfgParam.fltDB = 3.0;
    Get().cCamCfgParam.iMinExposureTime  = 0;
	Get().cCamCfgParam.iMinGain  = 70;
    Get().cCamCfgParam.iMaxExposureTime  = 2500;
	Get().cCamCfgParam.iMaxGain  = 220;
    Get().cCamCfgParam.iMaxAGCLimit = 163;
    Get().cCamCfgParam.iMinAGCLimit = 20;
    Get().cCamCfgParam.iMaxPSD = 3000;
    Get().cCamCfgParam.iMinPSD = 500;
	Get().cCamCfgParam.iMinPlateLight = 140;
	Get().cCamCfgParam.iMaxPlateLight = 160;
	Get().cCamCfgParam.nTestProtocolPort = SVR_PORT;

	Get().cCamCfgParam.iIPNCLogOutput = 0;
		
	INT irgExposureTimeTemp[MAX_LEVEL_COUNT] =
	{
		8, 8, 8, 8, 12, 17, 24, 34, 48, 68, 96, 96, 96, 96
	};

	for (INT i = 0; i < MAX_LEVEL_COUNT; i++)
	{
		Get().cCamCfgParam.irgExposureTime[i] = irgExposureTimeTemp[i];
		Get().cCamCfgParam.irgAGCLimit[i] = -1;
		Get().cCamCfgParam.irgGain[i] = -1;
	}

	Get().cCamCfgParam.iCaptureAutoParamEnable = 0;
	Get().cCamCfgParam.iCaptureDayShutterMax = 1500;
	Get().cCamCfgParam.iCaptureDayGainMax	= 180;
	Get().cCamCfgParam.iCaptureNightShutterMax = 2000;
	Get().cCamCfgParam.iCaptureNightGainMax = 180;
}


void CSWLPRParameter::SetDefaultHvDSP()
{
	Get().cResultSenderParam.iBestSnapshotOutput = 1;
	Get().cResultSenderParam.iLastSnapshotOutput = 1;
	Get().cResultSenderParam.iOutputCaptureImage = 0;
	Get().cResultSenderParam.iRecordFreeSpacePerDisk = 10;
	Get().cResultSenderParam.iVideoFreeSpacePerDisk = 10;
	Get().cResultSenderParam.iVideoDisplayTime = 500;
	Get().cResultSenderParam.iDrawRect = 1;
	Get().cResultSenderParam.iEddyType = 0;
	Get().cResultSenderParam.iSpeedLimit = 1000;
	Get().cResultSenderParam.iSaveType = 0;
	Get().cResultSenderParam.iWidth = 720;
	Get().cResultSenderParam.iHeight = 288;
	Get().cResultSenderParam.iCheckEventTime = 1;
	Get().cResultSenderParam.iFilterUnSurePeccancy = 0;
	Get().cResultSenderParam.fOutputAppendInfo = TRUE;
	Get().cResultSenderParam.fOutputObservedFrames = TRUE;
	Get().cResultSenderParam.fOutputCarArriveTime = FALSE;
	strcpy(Get().cResultSenderParam.szSafeSaverInitStr, "NULL");
	strcpy(Get().cResultSenderParam.szStreetName, "NULL");
	strcpy(Get().cResultSenderParam.szStreetDirection, "NULL");
	Get().cResultSenderParam.iOutputOnlyPeccancy = 0;
	Get().cResultSenderParam.iSendRecordSpace = 5;
	Get().cResultSenderParam.iSendHisVideoSpace = 3;
	Get().cResultSenderParam.nOutputPeccancyType = 0;
	Get().cResultSenderParam.fltCarAGCLinkage = 0.0f;
	Get().cResultSenderParam.iAWBLinkage = 0;
	Get().cResultSenderParam.fOutputFilterInfo = 0;
	Get().cResultSenderParam.fInitHdd = FALSE;
	Get().cResultSenderParam.iFileSize = 512;		
	Get().cResultSenderParam.iDiskType = 2;
	Get().cResultSenderParam.fIsSafeSaver = FALSE;
	Get().cResultSenderParam.iSaveSafeType = 1;
	Get().cResultSenderParam.fSaveVideo = FALSE;
	Get().cResultSenderParam.iSaveVideoType = 0;
	Get().cResultSenderParam.fMTUSetEnable = FALSE;
	strcpy(Get().cResultSenderParam.szPeccancyPriority, "����-�ǻ�������-�����-����������ʻ-ѹ��-����");
	Get().cResultSenderParam.iCapMode = 0;
	Get().cResultSenderParam.iFlashDifferentLane = 0;
    Get().cResultSenderParam.iFlashOneByOne = 0;

	Get().cResultSenderParam.fEnableTriggerOut = 1;
	Get().cResultSenderParam.nTriggerOutNormalStatus = 1;
	Get().cResultSenderParam.nTriggerOutPlusWidth = 5000;

	Get().cResultSenderParam.fEnableNtp = 0;
	Get().cResultSenderParam.iNtpSyncInterval = 3600; // 1Сʱ
	Get().cResultSenderParam.iTimeZone = 20;		//Ĭ�϶�8����GMT+8,0ΪGMT-12
	swpa_memset(Get().cResultSenderParam.szNtpServerIP, 0, sizeof(Get().cResultSenderParam.szNtpServerIP));
	swpa_strcpy(Get().cResultSenderParam.szNtpServerIP, "210.72.145.44");
	swpa_strcpy(Get().cResultSenderParam.szNetDiskIP, "172.18.10.10");
	swpa_strcpy(Get().cResultSenderParam.szNFS, "/volume1/nfs");
	swpa_strcpy(Get().cResultSenderParam.szNFSParam, "-o nolock,tcp");
	Get().cResultSenderParam.iNetDiskSpace = 1;

	Get().cResultSenderParam.iUseRushRule = 0;
	Get().cResultSenderParam.iEnableBackupIO = 0;
	Get().cResultSenderParam.iTimeIn = 1500;
	Get().cResultSenderParam.iBarrierStatus = 0;

		//��������Ĭ��ֵ
	Get().cResultSenderParam.cAutoLinkParam.iAutoLinkPort = 6665;

    // ��ͼ����
    Get().cResultSenderParam.iLastSnapshotCropOutput = 0;
    Get().cResultSenderParam.iCropWidthLevel = 6;
    Get().cResultSenderParam.iCropHeightLevel = 4;

}


void CSWLPRParameter::SetDefaultIPTCtrl()
{
	for(int i = 0; i < 2; i++)
	{
		Get().cDevParam[i].iBaudrate = 9600;
		Get().cDevParam[i].iDeviceType = 0;
		Get().cDevParam[i].iRoadNo = 0;
		//����ֻ����RS485
		Get().cDevParam[i].iCommType = 1;
	}
}

void CSWLPRParameter::SetDefaultOuterCtrl()
{
	swpa_memset(&Get().cMatchParam, 0, sizeof(Get().cMatchParam));
    Get().cMatchParam.dwPlateKeepTime = 1500;
    Get().cMatchParam.dwSignalKeepTime = 2000;
    Get().cMatchParam.dwMatchMaxTime = 1500;
    Get().cMatchParam.dwMatchMinTime = 1500;
    Get().cMatchParam.fCaptureRecong = TRUE;

    Get().cMatchParam.signal[0].dwType = 3;
    Get().cMatchParam.signal[0].dwRoadID = 0xFF;
}



void CSWLPRParameter::ResetParam(VOID)
{	
	//��־Ĭ�ϵȼ�
	Get().nLogLevel = 1;
    Get().nWorkModeIndex = PRM_CAP_FACE;
    Get().nCameraWorkMode = 19;
    Get().nMJPEGWidth = 3392;
    Get().nMJPEGWidth = 2000;

	SetDefaultCamApp();
	SetDefaultCamCfgParam();
	SetDefaultCharacter();
	SetDefaultFrontPannel();
	SetDefaultGB28181();
	SetDefaultHvDSP();
	SetDefaultOnvif();
	SetDefaultOuterCtrl();
	SetDefaultTracker();
	SetDefaultIPTCtrl();
	
		
	Get().cAutoRebootParam.fEnable = FALSE;			
	Get().cAutoRebootParam.iCarLeftCountLimit = 50;
	Get().cAutoRebootParam.iSysUpTimeLimit = 48;	

	Get().cDomeRockerParam.fRockerEnable = FALSE;
	Get().cDomeRockerParam.iBaudrate = 9600;
    Get().cDomeRockerParam.iProtocol = 0;

	sprintf(Get().cTcpipCfgParam.szIp, "100.100.100.101");
	sprintf(Get().cTcpipCfgParam.szNetmask, "255.0.0.0");
	sprintf(Get().cTcpipCfgParam.szGateway, "100.100.1.1");

	//ǰ��ͼ��ɼ�ģ�����
	Get().cImgFrameParam.iCamNum = 1;
	for ( INT i=0; i<MAX_CAM_COUNT; ++i )
	{
		memset(Get().cImgFrameParam.rgstrCamIP[i], 0, 32);
		memset(Get().cImgFrameParam.rgstrHVCParm[i], 0, 128);
	}
	Get().cImgFrameParam.fltCaptureFrameRate = 12.5;
	Get().cImgFrameParam.iOutputFrameRate = 15;
	
}

HRESULT CSWLPRParameter::Initialize(CSWString strFilePath)
{
	// ��ʼ��ǰ��������Ĭ��ֵ
	ResetParam();
	
	if(S_OK == CSWParameter<ModuleParams>::Initialize(strFilePath)
	&& S_OK == InitSystem()
	&& S_OK == InitTracker()
	&& S_OK == InitHvDsp()
	&& S_OK == InitOuterCtrl()
	&& S_OK == InitIPTCtrl()
	&& S_OK == InitCamApp()
	&& S_OK == InitScaleSpeed()
	&& S_OK == Init368()
	&& S_OK == InitCharacter()
	&& S_OK == InitGB28181()
	&& S_OK == InitONVIF()
	)
	{
		return S_OK;		
	}

	SW_TRACE_DEBUG("<LPRParameter> Initialize failed.\n");
	return E_FAIL;
}

HRESULT CSWLPRParameter::GetWorkMode(INT &iWorkMode, CSWString & strWorkKode)
{
	iWorkMode = Get().nWorkModeIndex;
	strWorkKode = m_strWorkMode;
	return S_OK;
}

/**
 *@brief ��ʼ��system�ڵ�
 *@return �ɹ�����S_OK,����ֵ��ʾʧ��
 */
HRESULT CSWLPRParameter::InitSystem()
{
	/*
	GetEnum("\\Tracker"
		, "PlateRecogMode"
		, &Get().nWorkModeIndex
		, Get().nWorkModeIndex
		, (PSTR)(LPCSTR)m_strWorkMode
  	, "����ģʽ"
  	, ""
  	, INNER_LEVEL
	, TRUE
  );
  */
  		
	char szTemp[512];	
/*	
	GetString("\\System[ϵͳ]"
		, "ComMac"
		, szTemp
		, ""
		, sizeof(szTemp)
		, "MAC��ַ"
		, ""
		, CUSTOM_LEVEL
		, TRUE
		);
*/		
	GetEnum("\\System[ϵͳ]"
  	, "LogLevel"
  	, &Get().nLogLevel
  	, Get().nLogLevel
  	, "0:����ȼ�;1:����ȼ�;2:�����ȼ�;3:���Եȼ�"
  	, ""
  	, ""
  	, PROJECT_LEVEL
    );  
    
	DWORD dwLen = sizeof(szTemp);
	swpa_device_read_sn(szTemp, &dwLen);
	GetString("\\System[ϵͳ]"
		, "CommDevNo"
		, szTemp
		, ""
		, sizeof(szTemp)
		, "�豸���к�"
		, ""
		, CUSTOM_LEVEL
		, TRUE
		);
/*		
	GetString("\\System[ϵͳ]"
		, "Version"
		, szTemp
		, ""
		, sizeof(szTemp)
		, "�汾��Ϣ"
		, ""
		, CUSTOM_LEVEL
		, TRUE
		);
*/	
	INT iVersionLen = 512;
	char szVersion[iVersionLen];
	if( 0 != swpa_info_get_firmware_version(szVersion, &iVersionLen) )
	{
		SW_TRACE_DEBUG("<app>get firmware version failed!");
		szVersion[0] = 0;
	}
	else
	{
		CSWString str = szVersion;
		// kernel�İ汾̫����ȥ������ʾ��
		// todo.
		INT iPos = str.Find("kernel_dm6467");
		CSWString strVersion;
		if( -1 != iPos )
		{
			strVersion = str.Substr(0, iPos);
		}
		swpa_strcpy(szVersion, (const CHAR*)strVersion);
		SW_TRACE_DEBUG("<app>firmware version:%s.", szVersion);
	}
	sprintf(szTemp
		, "Algorithm:%d\n"
		"BasicModule:%d\n"
		"SWFC:%d\n"
		"PlatformAPI:%d\n"
		"App:%d\n"
		"Config:%d\n"
		"FileSystem:%d\n"
		"Plugin:%d\n"
		, DSP_VERSION
		, MODULE_VERSION
		, SWFC_VERSION
		, SWPA_VERSION
		, APP_VERSION
		, CONFIG_VERSION
		, ROOTFS_VERSION
		, PLUGIN_VERSION
		);

    GetString("\\System[ϵͳ]\\Build[����]"
    	, "Module No."
    	, szTemp
    	, szTemp
    	, sizeof(szTemp)
      , "ģ��汾��"
      , ""
      , CUSTOM_LEVEL
      , TRUE
    );
	GetString("\\System[ϵͳ]\\Build[����]"
		, "Firmware No."
		, szVersion
		, szVersion
		, sizeof(szVersion)
		, "�̼��汾��"
		, ""
		, CUSTOM_LEVEL
		, TRUE
		);
		
	sprintf(szTemp
    , "%s%d%s" 
		, VERSIONHEADER
        , MAIN_VERSION
#ifdef USER_VERSION
        , USER_VERSION
#else
		,""
#endif
		);
		
    GetString("\\System[ϵͳ]\\Build[����]"
    	, "No."
    	, szTemp
    	, szTemp
    	, sizeof(szTemp)
      , "����汾��"
      , ""
      , CUSTOM_LEVEL
      , TRUE
    );
    UpdateString("\\System[ϵͳ]\\Build[����]", "No.", szTemp);
    m_strVersion = szTemp;
      
    GetString("\\System[ϵͳ]\\TcpipCfg[��������]"
        , "CommIPAddr"
        , Get().cTcpipCfgParam.szIp
        , Get().cTcpipCfgParam.szIp
        , 31
        , "IP��ַ"
        , ""
        , CUSTOM_LEVEL
        , TRUE
    );
    
    GetString("\\System[ϵͳ]\\TcpipCfg[��������]"
    	, "CommMask"
    	, Get().cTcpipCfgParam.szNetmask
    	, Get().cTcpipCfgParam.szNetmask
    	, 31
    	, "��������"
    	, ""
    	, CUSTOM_LEVEL
    	, TRUE
    );
    
    GetString("\\System[ϵͳ]\\TcpipCfg[��������]"
    	, "CommGateway"
    	, Get().cTcpipCfgParam.szGateway
    	, Get().cTcpipCfgParam.szGateway
    	, 31
    	, "����"
    	, ""
    	, CUSTOM_LEVEL
    	, TRUE
    );

	char szIp[32];
	char szMask[32];
	char szGateway[32];
	char szMac[32];

	if( 0 == swpa_device_read_ipinfo(szIp, 32, szMask, 32, szGateway, 32) )
	{
		if( swpa_strcmp(szGateway, "0.0.0.0") == 0 )
		{
			swpa_strcpy(szGateway, "100.100.100.11");
		}
		SW_TRACE_DEBUG("<ip>SetIp:%s,%s,%s.\n", szIp, szMask, szGateway);
		if( 0 != swpa_tcpip_setinfo("eth0", szIp, szMask, szGateway) )
		{
			SW_TRACE_DEBUG("<ip>SetIp failed.:%s,%s,%s.\n", szIp, szMask, szGateway);
		}
		else if( swpa_strcmp(Get().cTcpipCfgParam.szIp, szIp) != 0
				|| swpa_strcmp(Get().cTcpipCfgParam.szNetmask, szMask) != 0
				|| swpa_strcmp(Get().cTcpipCfgParam.szGateway, szGateway) != 0 )
		{
			UpdateString("System[ϵͳ]\\TcpipCfg[��������]", "CommIPAddr", (const CHAR*)szIp);
			UpdateString("System[ϵͳ]\\TcpipCfg[��������]", "CommMask", (const CHAR*)szMask);
			UpdateString("System[ϵͳ]\\TcpipCfg[��������]", "CommGateway", (const CHAR*)szGateway);
		}

		if( 0 == swpa_tcpip_getinfo("eth0", szIp, 32, szMask, 32, szGateway, 32, szMac, 32) )
		{
			GetString("\\System[ϵͳ]\\TcpipCfg[��������]"
				, "CommMac"
				, szMac
				, szMac
				, 32
				, "MAC��ַ"
				, ""
				, CUSTOM_LEVEL
				, TRUE
				);
		}
	}
	// Ĭ��Ҫ�и�IP�������豸���޷����ӡ�
	else
	{
		swpa_tcpip_setinfo("eth0", "100.100.100.101", "255.0.0.0", "100.100.100.1");
	}

   return S_OK;
}

/**
 *@brief 
 */
HRESULT CSWLPRParameter::InitTracker(VOID)
{
	GetEnum("\\Tracker\\ProcessPlate"
		, "EnableDBGreenSegment"
		, &Get().cTrackerCfgParam.nPlateDetect_Green
		, Get().cTrackerCfgParam.nPlateDetect_Green
		, "0:�ر�;1:��"
    , "����ʶ�𿪹�"
    , ""
    , CUSTOM_LEVEL
    );

  GetEnum("\\Tracker\\ProcessPlate"
  	, "LightBlueFlag"
  	, &Get().cTrackerCfgParam.nProcessPlate_LightBlue
  	, Get().cTrackerCfgParam.nProcessPlate_LightBlue
  	, "0:�ر�;1:��"
  	, "ǳ���ƿ���"
  	, ""
  	, PROJECT_LEVEL
    );  
    
  GetEnum("\\Tracker\\ProcessPlate"
  	, "EnableBigPlate"
  	, &Get().cTrackerCfgParam.nEnableBigPlate
  	, Get().cTrackerCfgParam.nEnableBigPlate
  	, "0:�ر�;1:��"
  	, "����ʶ�𿪹�"
  	, ""
  	, PROJECT_LEVEL			//�㷨�Ż���طŵ�3������
    );

    GetEnum("\\Tracker\\ProcessPlate"
    	, "EnablePlateEnhance"
    	, &Get().cTrackerCfgParam.nEnablePlateEnhance
    	, Get().cTrackerCfgParam.nEnablePlateEnhance
    	, "0:�ر�;1:��"
    	, "�ָ�ǰ�Ƿ����ͼƬ��ǿ"
    	, ""
    	, PROJECT_LEVEL
    );

    GetEnum("\\Tracker\\ProcessPlate"
    	, "EnableProcessBWPlate"
    	, &Get().cTrackerCfgParam.nEnableProcessBWPlate
    	, Get().cTrackerCfgParam.nEnableProcessBWPlate
    	, "0:�ر�;1:��"
    	, "�Ƿ����ڰ����"
    	, ""
    	, PROJECT_LEVEL
    );

    GetInt("\\Tracker\\ProcessPlate"
    	, "PlateResizeThreshold"
    	, &Get().cTrackerCfgParam.nPlateResizeThreshold
    	, Get().cTrackerCfgParam.nPlateResizeThreshold
    	, 0
    	, 200
    	, "�ָ�ǰСͼ��������ֵ"
    	, ""
    	, PROJECT_LEVEL
    );

    GetInt("\\Tracker\\ProcessPlate\\BlackPlate"
    	, "NightThreshold"
    	,	&Get().cTrackerCfgParam.nNightThreshold
    	, Get().cTrackerCfgParam.nNightThreshold
    	, 0
    	, 240
    	, "ҹ��ģʽ��������"
    	, ""
    	, CUSTOM_LEVEL
    );

    GetInt("\\Tracker\\ProcessPlate\\BlackPlate"
    	, "PlateLightCheckCount"
    	, &Get().cTrackerCfgParam.nPlateLightCheckCount
    	, Get().cTrackerCfgParam.nPlateLightCheckCount
    	, 3
    	, 50
    	, "���ȵ������"
    	, ""
    	, CUSTOM_LEVEL
    );

    GetInt("\\Tracker\\ProcessPlate\\BlackPlate"
    	, "MinPlateBrightness"
    	, &Get().cTrackerCfgParam.nMinPlateBrightness
    	, Get().cTrackerCfgParam.nMinPlateBrightness
    	, 1
    	, 255
    	, "�����������"
    	, ""
    	, CUSTOM_LEVEL
    );
		
    GetInt("\\Tracker\\ProcessPlate\\BlackPlate"
    	, "MaxPlateBrightness"
    	, &Get().cTrackerCfgParam.nMaxPlateBrightness
    	, Get().cTrackerCfgParam.nMaxPlateBrightness
    	, 1
    	, 255
    	, "�����������"
    	, ""
    	, CUSTOM_LEVEL
    );
    
    GetInt("\\Tracker\\ProcessPlate\\BlackPlate"
    	, "H0"
    	, &Get().cTrackerCfgParam.nProcessPlate_BlackPlateThreshold_H0
    	, Get().cTrackerCfgParam.nProcessPlate_BlackPlateThreshold_H0
    	, 0
    	, 240
    	, "����ɫ������"
    	, ""
    	, PROJECT_LEVEL
    );

    GetInt("\\Tracker\\ProcessPlate\\BlackPlate"
    	, "H1"
    	, &Get().cTrackerCfgParam.nProcessPlate_BlackPlateThreshold_H1
    	, Get().cTrackerCfgParam.nProcessPlate_BlackPlateThreshold_H1
    	, Get().cTrackerCfgParam.nProcessPlate_BlackPlateThreshold_H0
    	, 240
    	, "����ɫ������"
    	, ""
    	, PROJECT_LEVEL
    );

    GetInt("\\Tracker\\ProcessPlate\\BlackPlate"
    	, "S"
    	, &Get().cTrackerCfgParam.nProcessPlate_BlackPlate_S
    	, Get().cTrackerCfgParam.nProcessPlate_BlackPlate_S
    	, 0
    	, 240
    	, "���Ʊ��Ͷ�����"
    	, ""
    	, CUSTOM_LEVEL
    );

    GetInt("\\Tracker\\ProcessPlate\\BlackPlate"
    	, "L"
    	, &Get().cTrackerCfgParam.nProcessPlate_BlackPlate_L
    	, Get().cTrackerCfgParam.nProcessPlate_BlackPlate_L
    	, 0
    	, 240
    	, "������������"
    	, ""
    	, PROJECT_LEVEL
    );

    GetEnum("Tracker\\Recognition"
    	, "UseEdgeMethod"
    	, &Get().cTrackerCfgParam.fUseEdgeMethod
    	, Get().cTrackerCfgParam.fUseEdgeMethod
    	, "0:����ǿ;1:��ǿ"
    	, "��ǿ����ͼʶ��"
    	, ""
    	, PROJECT_LEVEL
    );
    
    GetEnum("Tracker\\Recognition"
    	, "EnableT1Model"
    	, &Get().cTrackerCfgParam.fEnableT1Model
    	, Get().cTrackerCfgParam.fEnableT1Model
    	, "0:�ر�;1:��"
    	, "T-1ģ�Ϳ���"
    	, ""
    	, PROJECT_LEVEL
    );

    GetEnum("Tracker\\Misc"
    	, "EnableAlphaRecog"
    	, &Get().cTrackerCfgParam.fEnableAlpha_5
    	, Get().cTrackerCfgParam.fEnableAlpha_5
    	, "0:�ر�;1:��"
    	, "������ĸʶ�𿪹�"
    	, ""
    	, CUSTOM_LEVEL
    );
    
    GetInt("Tracker\\Recognition"
    	, "RecogAsteriskThreshold"
    	, &Get().cTrackerCfgParam.nRecogAsteriskThreshold
    	, Get().cTrackerCfgParam.nRecogAsteriskThreshold
    	, 0
    	, 100
    	, "�÷ֵ�����ֵ(�ٷֱ�)��#��ʾ"
    	, ""
    	, PROJECT_LEVEL
    );
    
    GetEnum("Tracker\\Misc"
    	, "RecogGXPolice"
    	, &Get().cTrackerCfgParam.nRecogGxPolice
    	, Get().cTrackerCfgParam.nRecogGxPolice
    	, "0:�ر�;1:��"
    	, "�ط�����ʶ�𿪹�"
    	, ""
    	, PROJECT_LEVEL
    );
    
    GetInt("Tracker\\Misc"
    	, "CarArrivedPos"
    	, &Get().cTrackerCfgParam.nCarArrivedPos
    	, Get().cTrackerCfgParam.nCarArrivedPos
    	, 0
    	, 100
    	, "��������λ��(��������������)"
    	, ""
    	, CUSTOM_LEVEL
    );
    
    GetInt("Tracker\\Misc"
    	, "CarArrivedPosNoPlate"
    	, &Get().cTrackerCfgParam.nCarArrivedPosNoPlate
    	, Get().cTrackerCfgParam.nCarArrivedPosNoPlate
    	, 0
    	, 100
    	, "���Ƴ�����λ��"
    	, ""
    	, CUSTOM_LEVEL
    );
    
    GetInt("Tracker\\Misc"
    	, "CarArrivedPosYellow"
    	, &Get().cTrackerCfgParam.nCarArrivedPosYellow
    	, Get().cTrackerCfgParam.nCarArrivedPosYellow
    	, 0
    	, 100
    	, "��������λ��(����)"
    	, ""
    	, CUSTOM_LEVEL
    );
    
    GetEnum("Tracker\\Misc"
    	, "CoreProcessType"
    	, &Get().cTrackerCfgParam.nProcessType
    	, Get().cTrackerCfgParam.nProcessType
    	, "0:ֻ���ִ���С�ͳ�;1:��������ַǻ���������;2:��������ַǻ�����������"
    	, "��������"
    	, ""
    	, CUSTOM_LEVEL
    );
    
    GetEnum("Tracker\\Misc"
    	, "CoreOutPutType"
    	, &Get().cTrackerCfgParam.nOutPutType
    	, Get().cTrackerCfgParam.nOutPutType
    	, "0:ȫ�������;1:����С�ͳ����Ϊ������"
    	, "������� 0:ȫ�������;1:����С�ͳ����Ϊ������"
    	, ""
    	, CUSTOM_LEVEL
    );
    
    GetEnum("Tracker\\Misc"
    	, "NightPlus"
    	, &Get().cTrackerCfgParam.nNightPlus
    	, Get().cTrackerCfgParam.nNightPlus
    	, "0:�ر�;1:��"
    	, "���ϼ�ǿ�ǻ��������,ֻ�г����㹻��ʱ����,��������Ӷ��"
    	, ""
    	, PROJECT_LEVEL
    );
    
    GetInt("Tracker\\Misc"
    	, "WalkManSpeed"
    	, &Get().cTrackerCfgParam.nWalkManSpeed
    	, Get().cTrackerCfgParam.nWalkManSpeed
    	, 0
    	, 300
    	, "�����жϵ�������"
    	, ""
    	, PROJECT_LEVEL
    );
    
    GetFloat("Tracker\\Misc"
    	, "BikeSensitivity"
    	, &Get().cTrackerCfgParam.fltBikeSensitivity
    	, Get().cTrackerCfgParam.fltBikeSensitivity
    	, 0.0f
    	, 10.0f
    	, "�ǻ�����������"
    	, ""
    	, PROJECT_LEVEL
    );
    
    GetInt("Tracker\\Misc"
    	, "CarArrivedDelay"
    	, &Get().cTrackerCfgParam.nCarArrivedDelay
    	, Get().cTrackerCfgParam.nCarArrivedDelay
    	, 0
    	, 100
    	, "�������ﴥ���ӳپ���(��)"
    	, ""
    	, PROJECT_LEVEL
    );
    
    GetInt("Tracker\\TrackInfo"
    	, "VoteFrameNum"
    	, &Get().cTrackerCfgParam.nVoteFrameNum
    	, Get().cTrackerCfgParam.nVoteFrameNum
    	, 4
    	, 1000
    	, "ͶƱ�Ľ����"
    	, ""
    	, CUSTOM_LEVEL
    );
    
    GetInt("Tracker\\TrackInfo"
    	, "MaxEqualFrameNumForVote"
    	, &Get().cTrackerCfgParam.nMaxEqualFrameNumForVote
    	, Get().cTrackerCfgParam.nMaxEqualFrameNumForVote
    	, 4, 1000
    	, "������ͬ�����������"
    	, ""
    	, CUSTOM_LEVEL
    );
    
    GetInt("Tracker\\TrackInfo"
    	, "BlockTwinsTimeout"
    	, &Get().cTrackerCfgParam.nBlockTwinsTimeout
    	, Get().cTrackerCfgParam.nBlockTwinsTimeout
    	, 0
    	, 3600
    	, "��ͬ�����Сʱ����(S)"
    	, ""
    	, CUSTOM_LEVEL
    );
    
    GetInt("Tracker\\TrackInfo"
    	, "AverageConfidenceQuan"
    	, &Get().cTrackerCfgParam.nAverageConfidenceQuan
    	, Get().cTrackerCfgParam.nAverageConfidenceQuan
    	, 0
    	, 65536
    	, "ƽ���÷�����"
    	, ""
    	, 1
    );
    
    GetInt("Tracker\\TrackInfo"
    	, "FirstConfidenceQuan"
    	, &Get().cTrackerCfgParam.nFirstConfidenceQuan
    	, Get().cTrackerCfgParam.nFirstConfidenceQuan
    	, 0
    	, 65536
    	, "���ֵ÷�����"
    	, ""
    	, PROJECT_LEVEL
    );
    
    GetInt("Tracker\\TrackInfo"
    	, "RemoveLowConfForVote"
    	, &Get().cTrackerCfgParam.nRemoveLowConfForVote
    	, Get().cTrackerCfgParam.nRemoveLowConfForVote
    	, 0
    	, 100
    	, "ͶƱǰȥ���͵÷ֳ��ưٷֱ�"
    	, ""
    	, PROJECT_LEVEL
    );
    
    //��ʼ����������
    Get().cTrackerCfgParam.cTrapArea.TopLeftX =     Get().cTrackerCfgParam.cDetectArea.DetectorAreaLeft;
    Get().cTrackerCfgParam.cTrapArea.TopRightX =    Get().cTrackerCfgParam.cDetectArea.DetectorAreaRight;
    Get().cTrackerCfgParam.cTrapArea.BottomLeftX =  Get().cTrackerCfgParam.cDetectArea.DetectorAreaLeft;
    Get().cTrackerCfgParam.cTrapArea.BottomRightX = Get().cTrackerCfgParam.cDetectArea.DetectorAreaRight;

    Get().cTrackerCfgParam.cTrapArea.TopLeftY =     Get().cTrackerCfgParam.cDetectArea.DetectorAreaTop;
    Get().cTrackerCfgParam.cTrapArea.TopRightY =    Get().cTrackerCfgParam.cDetectArea.DetectorAreaTop;
    Get().cTrackerCfgParam.cTrapArea.BottomLeftY =  Get().cTrackerCfgParam.cDetectArea.DetectorAreaBottom;
    Get().cTrackerCfgParam.cTrapArea.BottomRightY = Get().cTrackerCfgParam.cDetectArea.DetectorAreaBottom;

    Get().cTrackerCfgParam.cTrapArea.nDetectorMinScaleNum = Get().cTrackerCfgParam.cDetectArea.nDetectorMinScaleNum;
    Get().cTrackerCfgParam.cTrapArea.nDetectorMaxScaleNum = Get().cTrackerCfgParam.cDetectArea.nDetectorMaxScaleNum;

    GetEnum("Tracker\\DetAreaCtrl\\Normal"
    	,"EnableTrap"
    	, &Get().cTrackerCfgParam.cTrapArea.fEnableDetAreaCtrl
    	, Get().cTrackerCfgParam.cTrapArea.fEnableDetAreaCtrl
    	, "0:�ر�;1:��"
    	, "����ɨ��������ƿ���"
    	, ""
    	, CUSTOM_LEVEL
    );

    GetInt("Tracker\\DetAreaCtrl\\Normal"
    	, "TopLeftX"
    	, &Get().cTrackerCfgParam.cTrapArea.TopLeftX
    	, Get().cTrackerCfgParam.cTrapArea.TopLeftX
    	, 0
    	, 100
    	, "ɨ��������������X"
    	, ""
    	, CUSTOM_LEVEL
    );
    
    GetInt("Tracker\\DetAreaCtrl\\Normal"
    	, "TopLeftY" 
    	, &Get().cTrackerCfgParam.cTrapArea.TopLeftY
    	, Get().cTrackerCfgParam.cTrapArea.TopLeftY
    	, 0
    	, 100
    	, "ɨ��������������Y"
    	, ""
    	, CUSTOM_LEVEL
    );
    
    GetInt("Tracker\\DetAreaCtrl\\Normal"
    	, "TopRightX"
    	, &Get().cTrackerCfgParam.cTrapArea.TopRightX
    	, Get().cTrackerCfgParam.cTrapArea.TopRightX
    	, 0
    	, 100
    	, "ɨ��������������X"
    	, ""
    	, CUSTOM_LEVEL
    );
    
    GetInt("Tracker\\DetAreaCtrl\\Normal"
    	, "TopRightY"
    	, &Get().cTrackerCfgParam.cTrapArea.TopRightY
    	, Get().cTrackerCfgParam.cTrapArea.TopRightY
    	, 0
    	, 100
    	, "ɨ��������������Y"
    	, ""
    	, CUSTOM_LEVEL
    );

    GetInt("Tracker\\DetAreaCtrl\\Normal","BottomLeftX"
    	, &Get().cTrackerCfgParam.cTrapArea.BottomLeftX
    	, Get().cTrackerCfgParam.cTrapArea.BottomLeftX
    	, 0 
    	, 100
    	, "ɨ��������������X"
    	, ""
    	, CUSTOM_LEVEL
    );
    
    GetInt("Tracker\\DetAreaCtrl\\Normal"
    	, "BottomLeftY"
    	, &Get().cTrackerCfgParam.cTrapArea.BottomLeftY
    	, Get().cTrackerCfgParam.cTrapArea.BottomLeftY
    	, 0
    	, 100
    	, "ɨ��������������Y"
    	, ""
    	, CUSTOM_LEVEL
    );
    
    GetInt("Tracker\\DetAreaCtrl\\Normal"
    	, "BottomRightX"
    	, &Get().cTrackerCfgParam.cTrapArea.BottomRightX
    	, Get().cTrackerCfgParam.cTrapArea.BottomRightX
    	, 0
    	, 100
    	, "ɨ��������������X"
    	, ""
    	, CUSTOM_LEVEL
    );
    
    GetInt("Tracker\\DetAreaCtrl\\Normal"
    	, "BottomRightY"
    	, &Get().cTrackerCfgParam.cTrapArea.BottomRightY
    	, Get().cTrackerCfgParam.cTrapArea.BottomRightY
    	, 0
    	, 100
    	, "ɨ��������������Y"
    	, ""
    	, CUSTOM_LEVEL
    );
    
    GetInt("\\Tracker\\DetAreaCtrl\\Normal"
    	, "MinScale"
    	, &Get().cTrackerCfgParam.cDetectArea.nDetectorMinScaleNum
    	, Get().cTrackerCfgParam.cDetectArea.nDetectorMinScaleNum
    	, 0
    	, 16
    	, "������С���"
    	, "56*(1.1^MinScale)"
    	, CUSTOM_LEVEL
    );
    GetInt("\\Tracker\\DetAreaCtrl\\Normal"
    	, "MaxScale"
    	, &Get().cTrackerCfgParam.cDetectArea.nDetectorMaxScaleNum
    	, Get().cTrackerCfgParam.cDetectArea.nDetectorMaxScaleNum
    	, 0
    	, 16
    	, "���������"
    	, "56*(1.1^MaxScale)"
    	, CUSTOM_LEVEL
    );
    
    Get().cTrackerCfgParam.cDetectArea.fEnableDetAreaCtrl = Get().cTrackerCfgParam.cTrapArea.fEnableDetAreaCtrl;
    Get().cTrackerCfgParam.cDetectArea.DetectorAreaLeft   = Get().cTrackerCfgParam.cTrapArea.TopLeftX    > Get().cTrackerCfgParam.cTrapArea.BottomLeftX  ? Get().cTrackerCfgParam.cTrapArea.BottomLeftX : Get().cTrackerCfgParam.cTrapArea.TopLeftX;
    Get().cTrackerCfgParam.cDetectArea.DetectorAreaRight  = Get().cTrackerCfgParam.cTrapArea.TopRightX   < Get().cTrackerCfgParam.cTrapArea.BottomRightX ? Get().cTrackerCfgParam.cTrapArea.BottomRightX: Get().cTrackerCfgParam.cTrapArea.TopRightX;
    Get().cTrackerCfgParam.cDetectArea.DetectorAreaTop    = Get().cTrackerCfgParam.cTrapArea.TopLeftY    > Get().cTrackerCfgParam.cTrapArea.TopRightY    ? Get().cTrackerCfgParam.cTrapArea.TopRightY   : Get().cTrackerCfgParam.cTrapArea.TopLeftY;
    Get().cTrackerCfgParam.cDetectArea.DetectorAreaBottom = Get().cTrackerCfgParam.cTrapArea.BottomLeftY > Get().cTrackerCfgParam.cTrapArea.BottomRightY ? Get().cTrackerCfgParam.cTrapArea.BottomLeftY : Get().cTrackerCfgParam.cTrapArea.BottomRightY;

    GetInt("\\Tracker\\RoadInfo"
    	, "RoadLineNumber"
    	, &Get().cTrackerCfgParam.nRoadLineNumber
    	, Get().cTrackerCfgParam.nRoadLineNumber
    	, 2
        , MAX_ROADLINE_NUM
    	, "����������"
    	, ""
    	, CUSTOM_LEVEL
    );
    
    GetEnum("\\Tracker\\RoadInfo"
    	, "RoadNumberBegin"
    	, &Get().cTrackerCfgParam.iRoadNumberBegin 
    	, Get().cTrackerCfgParam.iRoadNumberBegin
    	, "0:����ʼ;1:���ҿ�ʼ"
      , "���������ʼ����"
      , ""
      , CUSTOM_LEVEL
    );

    GetEnum("\\Tracker\\RoadInfo"
    	, "StartRoadNumber"
    	, &Get().cTrackerCfgParam.iStartRoadNum
    	, Get().cTrackerCfgParam.iStartRoadNum
    	, "0:0;1:1;2:2;3:3;4:4;5:5;6:6;7:7;8:8;9:9"
    	, "��������ʼ���"
    	, "0:��0��ʼ,1:��1��ʼ,�Դ�����..."
    	, CUSTOM_LEVEL
    );

    for (int i = 0; i < MAX_ROADLINE_NUM; i++)
    {
        char szSection[256];
        sprintf(szSection, "\\Tracker\\RoadInfo\\Road%02d", i);

        GetInt(szSection
        	, "TopX"
        	, &Get().cTrackerCfgParam.rgcRoadInfo[i].ptTop.x
        	, Get().cTrackerCfgParam.rgcRoadInfo[i].ptTop.x
        	, 0
        	, 5000
        	, "��X����"
        	, ""
        	, CUSTOM_LEVEL
        );
        
        GetInt(szSection
        	, "TopY"
        	, &Get().cTrackerCfgParam.rgcRoadInfo[i].ptTop.y
        	, Get().cTrackerCfgParam.rgcRoadInfo[i].ptTop.y
        	, 0
        	, 5000
        	, "��Y����"
        	, ""
        	, CUSTOM_LEVEL
        );
        
        GetInt(szSection
        	, "BottomX"
        	, &Get().cTrackerCfgParam.rgcRoadInfo[i].ptBottom.x
        	, Get().cTrackerCfgParam.rgcRoadInfo[i].ptBottom.x
        	, 0
        	, 5000
        	, "��X����"
        	, ""
        	, CUSTOM_LEVEL
        );
        
        GetInt(szSection
        	, "BottomY"
        	, &Get().cTrackerCfgParam.rgcRoadInfo[i].ptBottom.y
        	, Get().cTrackerCfgParam.rgcRoadInfo[i].ptBottom.y
        	, 0
        	, 5000
        	, "��Y����"
        	, ""
        	, CUSTOM_LEVEL
        );
        
        GetInt(szSection
        	, "LineType"
        	, &Get().cTrackerCfgParam.rgcRoadInfo[i].iLineType
        	, Get().cTrackerCfgParam.rgcRoadInfo[i].iLineType
        	, 1
        	, 9999
        	, "����"
        	, ""
        	, CUSTOM_LEVEL
        );
        
        GetInt(szSection
        	, "RoadType"
        	, &Get().cTrackerCfgParam.rgcRoadInfo[i].iRoadType
        	, Get().cTrackerCfgParam.rgcRoadInfo[i].iRoadType
        	, 1
        	, 9999
        	, "��Ӧ�ĳ�������"
        	, ""
        	, CUSTOM_LEVEL
        );
    }
    GetEnum("\\Tracker\\VideoDet"
    	, "VideoDetMode"
    	, &Get().cTrackerCfgParam.cVideoDet.nVideoDetMode
    	, Get().cTrackerCfgParam.cVideoDet.nVideoDetMode
    	, "0:��ʹ����Ƶ���;1:ֻʹ�ñ������;2:ʹ����Ƶ���"
    	, "��Ƶ���ģʽ"
    	, ""
    	, CUSTOM_LEVEL
    );
    
    GetInt("\\Tracker\\VideoDet\\AreaCtrl"
    	, "Left"
    	, &Get().cTrackerCfgParam.cVideoDet.rcVideoDetArea.left
    	, Get().cTrackerCfgParam.cVideoDet.rcVideoDetArea.left
    	, 0
    	, 100
    	, "��Ƶ���������"
    	, ""
    	, CUSTOM_LEVEL
    );
    
    GetInt("\\Tracker\\VideoDet\\AreaCtrl"
    	, "Right"
    	, &Get().cTrackerCfgParam.cVideoDet.rcVideoDetArea.right
    	, Get().cTrackerCfgParam.cVideoDet.rcVideoDetArea.right
    	, 0
    	, 100
    	, "��Ƶ���������"
    	, ""
    	, CUSTOM_LEVEL
    );
    
    GetInt("\\Tracker\\VideoDet\\AreaCtrl"
    	, "Top"
    	, &Get().cTrackerCfgParam.cVideoDet.rcVideoDetArea.top
    	, Get().cTrackerCfgParam.cVideoDet.rcVideoDetArea.top
    	, 0
    	, 100
    	, "��Ƶ���������"
    	, ""
    	, CUSTOM_LEVEL
    );
    
    GetInt("\\Tracker\\VideoDet\\AreaCtrl"
    	, "Bottom"
    	, &Get().cTrackerCfgParam.cVideoDet.rcVideoDetArea.bottom
    	, Get().cTrackerCfgParam.cVideoDet.rcVideoDetArea.bottom
    	, 0
    	, 100
    	, "��Ƶ���������"
    	, ""
    	, CUSTOM_LEVEL
    );
    
    GetInt("\\Tracker\\VideoDet"
    	, "DayBackgroudThreshold"
    	, &Get().cTrackerCfgParam.cVideoDet.nDayBackgroudThreshold
    	, Get().cTrackerCfgParam.cVideoDet.nDayBackgroudThreshold
    	, 0
    	, 255
    	, "���챳�������ֵ"
    	, ""
    	, PROJECT_LEVEL
    );
    
    GetInt("\\Tracker\\VideoDet"
    	, "nBackgroudThreshold"
    	, &Get().cTrackerCfgParam.cVideoDet.nNightBackgroudThreshold
    	, Get().cTrackerCfgParam.cVideoDet.nNightBackgroudThreshold
    	, 0
    	, 255
    	, "���ϱ��������ֵ"
    	, ""
    	, PROJECT_LEVEL
    ); 
    
    GetInt("\\Tracker\\VideoDet"
    	, "VoteFrameCount"
    	, &Get().cTrackerCfgParam.cVideoDet.nVoteFrameCount
    	, Get().cTrackerCfgParam.cVideoDet.nVoteFrameCount
    	, 15
    	, 100
    	, "ͶƱ��Ч֡������֡�����ڴ�ֵ��ͶƱ"
    	, ""
    	, PROJECT_LEVEL
    );
    
    GetEnum("\\Tracker\\VideoDet"
    	, "CheckAcross"
    	, &Get().cTrackerCfgParam.cVideoDet.nCheckAcross
    	, Get().cTrackerCfgParam.cVideoDet.nCheckAcross
    	, "0:��;1:��"
    	, "�Ƿ��������ʻ������"
    	, ""
    	, PROJECT_LEVEL
    );
    
    GetEnum("\\Tracker\\VideoDet"
    	, "ShadowDetMode"
    	, &Get().cTrackerCfgParam.cVideoDet.nShadowDetMode
    	, Get().cTrackerCfgParam.cVideoDet.nShadowDetMode
    	, "0:��ʹ����Ӱ���;1:ʹ����Ӱ���"
    	, "��Ӱ���ģʽ"
    	, ""
    	, PROJECT_LEVEL
    );
    
    GetInt("\\Tracker\\VideoDet"
    	, "ShadowDetEffect"
    	, &Get().cTrackerCfgParam.cVideoDet.nShadowDetEffect
    	, Get().cTrackerCfgParam.cVideoDet.nShadowDetEffect
    	, 0
    	, 100
    	, "��Ӱ���ǿ���̶�ֵ"
    	, "0:ʹ��ȱʡֵ(����ȷ), 1:��Ӱ���ǿ���̶���С, 100:��Ӱ���ǿ���̶����"
    	, PROJECT_LEVEL
    );
    
    GetEnum("\\Tracker\\VideoDet"
    	, "DiffType"
    	, &Get().cTrackerCfgParam.cVideoDet.nDiffType
    	, Get().cTrackerCfgParam.cVideoDet.nDiffType
    	, "0:֡����ȡ;1:֡���sobelǰ��"
    	, "ǰ����ȡ��ʽ"
    	, "ֵΪ0:֡����ȡ ֵΪ1:֡���sobelǰ������Ը���·����������������"
    	, PROJECT_LEVEL
    );
    
    GetEnum("\\Tracker\\VideoDet"
    	, "MedFilter"
    	, &Get().cTrackerCfgParam.cVideoDet.nMedFilter
    	, Get().cTrackerCfgParam.cVideoDet.nMedFilter
    	, "0:�ر�;1:��"
    	, "���ƶ�������"
    	, ""
    	, PROJECT_LEVEL
    );
    
    GetInt("\\Tracker\\VideoDet"
    	, "MedFilterUpLine"
    	, &Get().cTrackerCfgParam.cVideoDet.nMedFilterUpLine
    	, Get().cTrackerCfgParam.cVideoDet.nMedFilterUpLine
    	, 0
    	, 100
    	, "���ƶ�����������������"
    	, "ֵΪ0:��Ļ���Ϸ� ֵΪ100:��Ļ���·�"
    	, PROJECT_LEVEL
    );
    
    GetInt("\\Tracker\\VideoDet"
    	, "MedFilterDownLine"
    	, &Get().cTrackerCfgParam.cVideoDet.nMedFilterDownLine
    	, Get().cTrackerCfgParam.cVideoDet.nMedFilterDownLine
    	, 0
    	, 100
    	, "���ƶ�����������������"
    	, "ֵΪ0:��Ļ���Ϸ� ֵΪ100:��Ļ���·�"
    	, PROJECT_LEVEL
    );
  
  //Scale��Ч���ж�
  if (Get().cTrackerCfgParam.cDetectArea.nDetectorMinScaleNum > Get().cTrackerCfgParam.cDetectArea.nDetectorMaxScaleNum)
  {
      UpdateInt("\\Tracker\\DetAreaCtrl\\Normal"
      	, "MinScale"
      	, 6
      );
      UpdateInt("\\Tracker\\DetAreaCtrl\\Normal"
      	, "MaxScale"
      	, 10
      );
      
      Get().cTrackerCfgParam.cDetectArea.nDetectorMinScaleNum = 5;
      Get().cTrackerCfgParam.cDetectArea.nDetectorMaxScaleNum = 9;
  }

  //��Ƶ���������Ч���ж�
  if (!CheckArea(
              Get().cTrackerCfgParam.cVideoDet.rcVideoDetArea.left,
              Get().cTrackerCfgParam.cVideoDet.rcVideoDetArea.top,
              Get().cTrackerCfgParam.cVideoDet.rcVideoDetArea.right,
              Get().cTrackerCfgParam.cVideoDet.rcVideoDetArea.bottom
          ))
  {
      UpdateInt("\\Tracker\\VideoDet\\AreaCtrl"
      	, "Left"
      	, 0
      );
      UpdateInt("\\Tracker\\VideoDet\\AreaCtrl"
      	, "Top"
      	, 0
      );
      UpdateInt("\\Tracker\\VideoDet\\AreaCtrl"
      	, "Right"
      	, 100
      );
      UpdateInt("\\Tracker\\VideoDet\\AreaCtrl"
      	, "Bottom"
      	, 100
      );
	
      Get().cTrackerCfgParam.cVideoDet.rcVideoDetArea.left = 0;
      Get().cTrackerCfgParam.cVideoDet.rcVideoDetArea.top =0;
      Get().cTrackerCfgParam.cVideoDet.rcVideoDetArea.right = 100;
      Get().cTrackerCfgParam.cVideoDet.rcVideoDetArea.bottom = 100;
  }
	return S_OK;
}

HRESULT CSWLPRParameter::InitScaleSpeed(VOID)
{

	GetEnum("\\Tracker\\ScaleSpeed"
		, "Enable"
		, &Get().cTrackerCfgParam.cScaleSpeed.fEnable
		, Get().cTrackerCfgParam.cScaleSpeed.fEnable
		, "0:�ر�;1:��"
		, "������ٿ���"
		, ""
		, CUSTOM_LEVEL
		);

	GetEnum("\\Tracker\\ScaleSpeed"
			, "MethodsFlag"
			, &Get().cTrackerCfgParam.cScaleSpeed.nMethodsFlag
			, Get().cTrackerCfgParam.cScaleSpeed.nMethodsFlag
			, "0:ƽ��궨��;1:�ռ�궨��"
			, "����ģʽ"
			, ""
			, CUSTOM_LEVEL
			);
	
	GetFloat("\\Tracker\\ScaleSpeed"
		, "LeftDistance"
		, &Get().cTrackerCfgParam.cScaleSpeed.fltLeftDistance
		, Get().cTrackerCfgParam.cScaleSpeed.fltLeftDistance
		, 0.01f
		, 10000.0f
		, "��Ļ���ص����żܾ���"
		, ""
		, CUSTOM_LEVEL
		);
	GetFloat("\\Tracker\\ScaleSpeed"
		, "FullDistance"
		, &Get().cTrackerCfgParam.cScaleSpeed.fltFullDistance
		, Get().cTrackerCfgParam.cScaleSpeed.fltFullDistance
		, 0.01f
		, 10000.0f
		, "��Ļ���ص����żܾ���"
		, ""
		, CUSTOM_LEVEL
		);
	GetFloat("\\Tracker\\ScaleSpeed"
		, "CameraHigh"
		, &Get().cTrackerCfgParam.cScaleSpeed.fltCameraHigh
		, Get().cTrackerCfgParam.cScaleSpeed.fltCameraHigh
		, 0.01f
		, 10000.0f
		, "������߶�"
		, ""
		, CUSTOM_LEVEL
		);
	
	GetFloat("\\Tracker\\ScaleSpeed"
		, "AdjustCoef"
		, &Get().cTrackerCfgParam.cScaleSpeed.fltAdjustCoef
		, Get().cTrackerCfgParam.cScaleSpeed.fltAdjustCoef
		, 0.01f
		, 10.0f
		, "����ϵ��"
		, ""
		, CUSTOM_LEVEL
		);
		
	for (int i = 0; i < 11; i++)
	{
		char szTransMarixText[32];
		char szTransMarixKey[32];
		sprintf(szTransMarixText, "�任����_%d", i);
		sprintf(szTransMarixKey, "TransMarix_%d", i);

		GetFloat("\\Tracker\\ScaleSpeed"
			, szTransMarixKey
			, &Get().cTrackerCfgParam.cScaleSpeed.rgfltTransMarix[i]
			, Get().cTrackerCfgParam.cScaleSpeed.rgfltTransMarix[i]
			, -3.40282347e+37F
			, 3.40282347e+37F
			, szTransMarixText
			, ""
			, CUSTOM_LEVEL
			);
	}	
	return S_OK;
}


BOOL CSWLPRParameter::CheckArea(int nLeft, int nTop, int nRight, int nBottom)
{
    if (nLeft < 0 || nTop < 0 || nRight < 0 || nBottom < 0
            || nLeft > 100 || nTop > 100 || nRight > 100 || nBottom > 100
            || (nLeft >= nRight) || (nTop >= nBottom))
    {
        return FALSE;
    }
    return TRUE;
}
/**
 *@brief 
 */
HRESULT CSWLPRParameter::InitHvDsp(VOID)
{/*
    GetEnum( "\\HvDsp\\Misc"
    	, "OutputAppendInfo"
    	, &Get().cResultSenderParam.fOutputAppendInfo
    	, Get().cResultSenderParam.fOutputAppendInfo
    	, "0:�ر�;1:��"
    	, "���������Ϣ����"
    	, ""
    	, PROJECT_LEVEL
    );
 
    GetEnum("\\HvDsp\\Misc"
    	, "OutputObservedFrames"
    	, &Get().cResultSenderParam.fOutputObservedFrames
    	, Get().cResultSenderParam.fOutputObservedFrames
    	, "0:�����;1:���"
    	, "�����Ч֡������"
    	, ""
    	, PROJECT_LEVEL
    );

    GetEnum("\\HvDsp\\Misc"
    	, "OutputCarArriveTime"
    	, &Get().cResultSenderParam.fOutputCarArriveTime
    	, Get().cResultSenderParam.fOutputCarArriveTime
    	, "0:�����;1:���"
    	, "������ʱ�俪��"
    	, ""
    	, PROJECT_LEVEL
    );
*/
	GetEnum("HvDsp\\Misc"
    	, "IPNCLogOutput"
    	, &Get().cCamCfgParam.iIPNCLogOutput
    	, Get().cCamCfgParam.iIPNCLogOutput
    	, "0:�����;1:���"
    	, "���IPNC��־��Ϣ"
    	, ""
    	, PROJECT_LEVEL
    );
	
    GetEnum("HvDsp\\Misc"
    	, "OutputFilterInfo"
    	, &Get().cResultSenderParam.fOutputFilterInfo
    	, Get().cResultSenderParam.fOutputFilterInfo
    	, "0:�����;1:���"
    	, "���������Ϣ"
    	, ""
    	, CUSTOM_LEVEL
    );

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
/*
    GetInt("\\HvDsp\\Misc"
    	, "VideoDisplayTime"
    	, &Get().cResultSenderParam.iVideoDisplayTime
    	, Get().cResultSenderParam.iVideoDisplayTime
    	, 1
    	, 1000000
    	, "������Ƶʱ����(MS)"
    	, ""
    	, CUSTOM_LEVEL
    );
*/
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
    	, CUSTOM_LEVEL
    );

	GetEnum("\\HvDsp\\Misc"
    	, "InitHdd"
    	, &Get().cResultSenderParam.fInitHdd
    	, Get().cResultSenderParam.fInitHdd
    	, "0:��;1:������ʽ��"
    	, "��ʼ��Ӳ�̡�ע��һ������Ч"
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
    	, "��̬Ӳ�̴洢��ʽ"
    	, ""
    	, CUSTOM_LEVEL
    );

    GetEnum("\\HvDsp\\Misc"
	    , "DiskType"
	    , &Get().cResultSenderParam.iDiskType
	    , Get().cResultSenderParam.iDiskType
	    , "0:����Ӳ��;2:��̬Ӳ��"
	    , "Ӳ������"
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
   		, "FlashDifferentLane"
   		, &Get().cResultSenderParam.iFlashDifferentLane
   		, Get().cResultSenderParam.iFlashDifferentLane
   		, "0:���ֳ�����;1:�ֳ�����"
   		, "����Ʒֳ�������������"
   		, ""
   		, CUSTOM_LEVEL
   	);

    GetEnum("\\HvDsp\\Misc"
        , "FlashOneByOne"
        , &Get().cResultSenderParam.iFlashOneByOne
        , Get().cResultSenderParam.iFlashOneByOne
        , "0:�ر�;1:����"
        , "���������"
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
    	, CUSTOM_LEVEL
    );

    GetString("\\HvDsp\\FilterRule"
    	, "Replace"
    	, Get().cResultSenderParam.cProcRule.szReplaceRule
    	, Get().cResultSenderParam.cProcRule.szReplaceRule
    	, sizeof(Get().cResultSenderParam.cProcRule.szReplaceRule)
    	, "�滻����"
    	, ""
    	, CUSTOM_LEVEL
    );

    GetString("\\HvDsp\\FilterRule"
    	, "Leach"
    	, Get().cResultSenderParam.cProcRule.szLeachRule
    	, Get().cResultSenderParam.cProcRule.szLeachRule
    	, sizeof(Get().cResultSenderParam.cProcRule.szLeachRule)
    	, "���˹���"
    	, ""
    	, CUSTOM_LEVEL
    );
    
    GetEnum("\\HvDsp\\Misc"
   		, "LoopReplaceEnable"
   		, &Get().cResultSenderParam.cProcRule.fLoopReplace
   		, Get().cResultSenderParam.cProcRule.fLoopReplace
   		, "0:������;1:����"
   		, "ѭ���滻"
   		, ""
   		, CUSTOM_LEVEL
   	);

    GetString("\\HvDsp\\Identify", "StreetName"
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

    GetEnum("\\Tracker\\DetReverseRun"
    	, "DetReverseRunEnable"
    	, &Get().cTrackerCfgParam.nDetReverseRunEnable
    	, Get().cTrackerCfgParam.nDetReverseRunEnable
    	, "0:��;1:��"
    	, "���м�⿪��"
    	, ""
    	, PROJECT_LEVEL
    );

    GetInt("\\Tracker\\DetReverseRun"
    	, "Span"
    	, &Get().cTrackerCfgParam.nSpan
    	, Get().cTrackerCfgParam.nSpan
    	, -100
    	, 100
    	, "�����������г������"
    	, ""
    	, CUSTOM_LEVEL
    );

    GetFloat("\\HvDsp\\EventChecker"
    	, "OverLineSensitivity"
    	, &Get().cTrackerCfgParam.fltOverLineSensitivity
    	, Get().cTrackerCfgParam.fltOverLineSensitivity
    	, 0.5
    	, 5.0
    	, "ѹ�߼��������"
    	, ""
    	, CUSTOM_LEVEL
    );

    GetInt("\\HvDsp\\EventChecker"
    	, "SpeedLimit"
    	, &Get().cResultSenderParam.iSpeedLimit
    	, Get().cResultSenderParam.iSpeedLimit
    	, 0
    	, 1000
    	, "�ٶ�����ֵ(km/h)"
    	, ""
    	, CUSTOM_LEVEL
    );
	Get().cTrackerCfgParam.iSpeedLimit = Get().cResultSenderParam.iSpeedLimit;



		char szSection[64];
    char szChName[256];
    for (int i = 1; i < MAX_ROADLINE_NUM - 1; i++)
    {
        sprintf(szSection, "IsCrossLine%d%d", i, i + 1);
        sprintf(szChName, "������%d���Խ��", i + 1);
        GetEnum("\\Tracker\\ActionDetect"
        	, szSection
        	, &Get().cTrackerCfgParam.cActionDetect.iIsCrossLine[i]
        	, Get().cTrackerCfgParam.cActionDetect.iIsCrossLine[i]
        	, "0:�����;1:���"
        	, szChName
        	, ""
        	, CUSTOM_LEVEL
        );
    }

    for (int i = 0; i < MAX_ROADLINE_NUM; i++)
    {
        sprintf(szSection, "IsYellowLine%d%d", i, i + 1);
        sprintf(szChName, "������%d���ѹ��", i + 1);
        GetEnum("\\Tracker\\ActionDetect"
        	, szSection
        	, &Get().cTrackerCfgParam.cActionDetect.iIsYellowLine[i]
        	, Get().cTrackerCfgParam.cActionDetect.iIsYellowLine[i]
        	, "0:���ж�ѹ��;1:ѹʵ��;2:ѹ����"
        	, szChName
        	, ""
        	, CUSTOM_LEVEL
        );
    }

	return S_OK;
}

HRESULT CSWLPRParameter::InitIPTCtrl(VOID)
{
	CSWString strCOMM;
	for(int i = 0; i < 2; i++)
	{
		strCOMM.Format("\\Device[�ⲿ�豸]\\COM%d[�˿�%02d]", i, i);
		GetEnum((LPCSTR)strCOMM
		  	  	, "Baudrate"
		  	  	, &Get().cDevParam[i].iBaudrate
		  	  	, 9600
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
		  	  	, CUSTOM_LEVEL
		  		);
  	
		GetEnum((LPCSTR)strCOMM
		  	  	, "DeviceType"
		  	  	, &Get().cDevParam[i].iDeviceType
		  	  	, 0
		  	  	, "0:��;1:�����״�;2:�ս�������;3:���͵º��̵�ת����;4:����ά���״�;5:�Ĵ������״�"
		  	  	, "�ⲿ�豸����"
		  	  	, ""
		  	  	, CUSTOM_LEVEL
		  		);
		GetInt((LPCSTR)strCOMM
				, "Road"
				, &Get().cDevParam[i].iRoadNo
				, Get().cDevParam[i].iRoadNo
				, 0
				, 255
				, "�����������"
				, "�����������"
				, CUSTOM_LEVEL
				);
			
		GetEnum((LPCSTR)strCOMM
				, "CommType"
				, &Get().cDevParam[i].iCommType
				, Get().cDevParam[i].iCommType
				, "0:RS232;1:RS485"
				, "��������"
				, ""
				, CUSTOM_LEVEL
				);
  	  
  	//��Ȧ������
  		GetInt((LPCSTR)strCOMM
				, "CoilInstance"
				, &Get().cDevParam[i].cCoilParam.iTowCoilInstance
				, Get().cDevParam[i].cCoilParam.iTowCoilInstance
				, 0
				, 100
				, "2����Ȧ�ľ���(����)"
				, "��Ȧ����"
				, CUSTOM_LEVEL
				);
		
		GetEnum((LPCSTR)strCOMM
		  	  	, "CoilEgdeType"
		  	  	, &Get().cDevParam[i].cCoilParam.iCoilEgdeType
		  	  	, Get().cDevParam[i].cCoilParam.iCoilEgdeType
		  	  	, "0:������;1:�½���"
		  	  	, "��������"
		  	  	, ""
		  	  	, CUSTOM_LEVEL
		  		);
  	
  		GetInt((LPCSTR)strCOMM
		  	  	, "Trigger"
		  	  	, (INT *)&Get().cDevParam[i].cCoilParam.bTrigger
		  	  	, (INT)Get().cDevParam[i].cCoilParam.bTrigger
		  	  	, 0
		  	  	, 15
		  	  	, "����ץ��"
		  	  	, "1:��Ȧ1����ץ��;2:��Ȧ2����ץ��;4:��Ȧ3����ץ��;8:��Ȧ4����ץ��"
		  	  	, CUSTOM_LEVEL
		  		);
  }    
	return S_OK;
}

HRESULT CSWLPRParameter::InitOuterCtrl(VOID)
{
	GetEnum("\\OuterCtrl[���ܿ�]"
    	, "OutPutPlateMode"
    	, &Get().cMatchParam.fEnable
    	, Get().cMatchParam.fEnable
    	, "0:�ر�;1:��"
    	, "���ܿ�ʹ�ܿ���"
    	, ""
    	, CUSTOM_LEVEL
    );
    
	GetInt("\\OuterCtrl[���ܿ�]"
		, "SignalHoldTime"
		, (INT *)&Get().cMatchParam.dwSignalKeepTime
		, Get().cMatchParam.dwSignalKeepTime
		, 500
		, 600000
		, "�źű���ʱ��"
		, ""
		, CUSTOM_LEVEL
	);
	
	GetInt("\\OuterCtrl[���ܿ�]"
		, "PlateHoldTime"
		, (INT *)&Get().cMatchParam.dwPlateKeepTime
		, Get().cMatchParam.dwPlateKeepTime
		, 500
		, 600000
		, "���Ʊ���ʱ��"
		, ""
		, CUSTOM_LEVEL
	);
	
	GetInt("\\OuterCtrl[���ܿ�]"
		, "MatchMinTime"
		, (INT *)&Get().cMatchParam.dwMatchMinTime
		, Get().cMatchParam.dwMatchMinTime
		, 0
		, 600000
		, "������ǰƥ���ź����ʱ���"
		, ""
		, CUSTOM_LEVEL
	);
	
	GetInt("\\OuterCtrl[���ܿ�]"
		, "MatchMaxTime"
		, (INT *)&Get().cMatchParam.dwMatchMaxTime
		, Get().cMatchParam.dwMatchMaxTime
		, 0
		, 600000
		, "�������ƥ���ź����ʱ���"
		, ""
		, CUSTOM_LEVEL
	);

	GetEnum("\\OuterCtrl[���ܿ�]"
		, "RecogSnapImg"
		, &Get().cMatchParam.fCaptureRecong
		, Get().cMatchParam.fCaptureRecong
		, "0:�ر�;1:��"
		, "ץ��ͼƬʶ�𿪹�"
		, ""
		, CUSTOM_LEVEL
		);
	if(Get().cTrackerCfgParam.fEnableDetFace)
	{
		Get().cMatchParam.fCaptureRecong = TRUE;
	}

	CHAR szRecogArea[64] = {0};
	swpa_strcpy(szRecogArea, "[0,50,100,100],6,12");
	GetString("\\OuterCtrl[���ܿ�]"
		, "RecogArea"
		, szRecogArea
		, szRecogArea
		, 64
		, "ץ��ʶ������(%)[��,��,��,��],��С���ƣ������"
		, "ץ��ʶ������(%)[��,��,��,��],��С���ƣ������"
		, CUSTOM_LEVEL
		);
	swpa_sscanf(
		szRecogArea,
		"[%d,%d,%d,%d],%d,%d",
		&Get().cMatchParam.iDetectorAreaLeft,
		&Get().cMatchParam.iDetectorAreaTop,
		&Get().cMatchParam.iDetectorAreaRight,
		&Get().cMatchParam.iDetectorAreaBottom,
		&Get().cMatchParam.iDetectorMinScaleNum,
		&Get().cMatchParam.iDetectorMaxScaleNum
		);

	Get().cTrackerCfgParam.cRecogSnapArea.fEnableDetAreaCtrl = TRUE;
	Get().cTrackerCfgParam.cRecogSnapArea.DetectorAreaLeft = Get().cMatchParam.iDetectorAreaLeft;
	Get().cTrackerCfgParam.cRecogSnapArea.DetectorAreaTop = Get().cMatchParam.iDetectorAreaTop;
	Get().cTrackerCfgParam.cRecogSnapArea.DetectorAreaRight = Get().cMatchParam.iDetectorAreaRight;
	Get().cTrackerCfgParam.cRecogSnapArea.DetectorAreaBottom = Get().cMatchParam.iDetectorAreaBottom;
	Get().cTrackerCfgParam.cRecogSnapArea.nDetectorMinScaleNum = Get().cMatchParam.iDetectorMinScaleNum;
	Get().cTrackerCfgParam.cRecogSnapArea.nDetectorMaxScaleNum = Get().cMatchParam.iDetectorMaxScaleNum;

	
	CSWString strTmp;
	for(int i = 0; i < sizeof(Get().cMatchParam.signal)/sizeof(SIGNAL_PARAM); i++)
	{
		strTmp.Format("\\OuterCtrl[���ܿ�]\\Signal%02d", i);
		GetEnum((LPCSTR)strTmp
    	, "SignalType"
    	, (INT *)&Get().cMatchParam.signal[i].dwType
    	, Get().cMatchParam.signal[i].dwType
    	, "0:��;1:�ٶ�;2:����;3:ץ��ͼ"
    	, "�ź�����"
    	, ""
    	, CUSTOM_LEVEL
    );

		GetInt((LPCSTR)strTmp
			, "RoadID"
			, (INT *)&Get().cMatchParam.signal[i].dwRoadID
			, 0
			, 0
			, 0xFF
			, "������"
			, ""
			, CUSTOM_LEVEL
		);		
    
    GetEnum((LPCSTR)strTmp
			, "SignalID"
			, (INT *)&Get().cMatchParam.signal[i].dwSignalID
			, Get().cMatchParam.signal[i].dwSignalID
			, "0:0;1:1;2:2"
			, "ͬһ�������ź�����"
			, ""
			, PROJECT_LEVEL
		);
		
		GetEnum((LPCSTR)strTmp
			, "Trigger"
			, (INT *)&Get().cMatchParam.signal[i].fTrigger
			, Get().cMatchParam.signal[i].fTrigger
			, "0:������ץ��;1:����ץ��"
			, "���ź��Ƿ�Ҫ����ץ��"
			, ""
			, CUSTOM_LEVEL
		);
		
		GetEnum((LPCSTR)strTmp
			, "Recognize"
			, (INT *)&Get().cMatchParam.signal[i].fRecognize
			, Get().cMatchParam.signal[i].fRecognize
			, "0:��ʶ��;1:ʶ��"
			, "ͼƬ�Ƿ�Ҫʶ��"
			, ""
			, PROJECT_LEVEL
		);
		
		GetEnum((LPCSTR)strTmp
			, "Condition"
			, (INT *)&Get().cMatchParam.signal[i].dwCondition
			, Get().cMatchParam.signal[i].dwCondition
			, "0:���źſ��п���,ֻҪ�г��Ƽ��ɳ���;1:�����д��źźͳ��Ʋ��ܳ����;2:ֻҪ�д��ź�,�ͱض�����"
			, "ƥ������"
			, ""
			, CUSTOM_LEVEL
		);
	}
	return S_OK;
}

/**
 *
 */
HRESULT CSWLPRParameter::InitCamApp(VOID)
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
   	, CUSTOM_LEVEL
    );
  
  GetInt("\\HvDsp\\Camera\\Ctrl"
   	, "AddrPort"
   	, &Get().cCamCfgParam.iCamPort
   	, Get().cCamCfgParam.iCamPort
   	, 1024
   	, 99999
   	, "����Э��˿ں�"
   	, "�˿ں�"
   	, CUSTOM_LEVEL
    );
      
  GetEnum("\\HvDsp\\Camera\\Ctrl"
  	, "DynamicCfgEnable"
  	, &Get().cCamCfgParam.iDynamicCfgEnable
  	, Get().cCamCfgParam.iDynamicCfgEnable
    , "0:�ر�;1:��"
    , "��̬���ò���"
    , ""
    , CUSTOM_LEVEL
    );
    
    GetEnum("\\HvDsp\\Camera\\Ctrl"
    	, "DynamicTriggerEnable"
    	, &Get().cCamCfgParam.iDynamicTriggerEnable
    	, Get().cCamCfgParam.iDynamicTriggerEnable
      , "0:�ر�;1:��"
      , "��Ƶ����ץ�����"
      , "�Դ���Jϵ�������HV�������Ч"
      , CUSTOM_LEVEL
    );
    
    GetInt("\\HvDsp\\Camera\\Ctrl"
    	, "TriggerDelay"
    	, &Get().cCamCfgParam.iTriggerDelay
    	, Get().cCamCfgParam.iTriggerDelay
    	, 0
    	, 65535
    	, "������ʱ(ms)"
    	, ""
    	, PROJECT_LEVEL
    );
    
    GetEnum("\\HvDsp\\Camera\\Ctrl"
    	, "AGCEnable"
    	, &Get().cCamCfgParam.iEnableAGC
    	, Get().cCamCfgParam.iEnableAGC
      , "0:�ر�;1:��"
      , "ʹ��AGC"
      , ""
      , CUSTOM_LEVEL
    );
    
    GetEnum("\\HvDsp\\Camera\\Ctrl"
    	, "AutoParamEnable"
    	, &Get().cCamCfgParam.iAutoParamEnable
    	, Get().cCamCfgParam.iAutoParamEnable
      , "0:�ر�;1:��"
      , "�Զ�����������"
      , ""
      , CUSTOM_LEVEL
    );
  	
  	if (Get().cCamCfgParam.iAutoParamEnable == 1)
  	{
  	    UpdateEnum("\\HvDsp\\Camera\\Ctrl"
  	    	, "AutoParamEnable"
  	    	, 0
  	    );
  	}

	GetInt("\\HvDsp\\Camera\\Ctrl"
  	, "MaxAGCLimit"
  	, &Get().cCamCfgParam.iMaxAGCLimit
  	, Get().cCamCfgParam.iMaxAGCLimit
  	, 0
  	, 255
  	, "���AGC����ֵ"
  	, ""
  	, CUSTOM_LEVEL
    );
    
    GetInt("\\HvDsp\\Camera\\Ctrl"
    	, "MinAGCLimit"
    	, &Get().cCamCfgParam.iMinAGCLimit
    	, Get().cCamCfgParam.iMinAGCLimit
    	, 0
    	, 255
    	, "��СAGC����ֵ"
    	, ""
    	, CUSTOM_LEVEL
    );
    
    GetFloat("\\HvDsp\\Camera\\Ctrl"
    	, "DB"
    	, &Get().cCamCfgParam.fltDB
    	, Get().cCamCfgParam.fltDB
    	, 1.0f
    	, 10.0f
    	, "����"
    	, ""
    	, CUSTOM_LEVEL
    );
    
    GetInt("\\HvDsp\\Camera\\Ctrl"
    	, "MinExposureTime"
    	, &Get().cCamCfgParam.iMinExposureTime
    	, Get().cCamCfgParam.iMinExposureTime
    	, 0
    	, 30000
    	, "��С�ع�ʱ��"
    	, ""
    	, CUSTOM_LEVEL
    );
    
    GetInt("\\HvDsp\\Camera\\Ctrl"
    	, "MinPlus"
    	, &Get().cCamCfgParam.iMinGain
    	, Get().cCamCfgParam.iMinGain
    	, 70
    	, 360
    	, "��С����"
    	, ""
    	, CUSTOM_LEVEL
    );
    
    GetInt("\\HvDsp\\Camera\\Ctrl"
    	, "MaxExposureTime"
    	, &Get().cCamCfgParam.iMaxExposureTime
    	, Get().cCamCfgParam.iMaxExposureTime
    	, 0
    	, 30000
    	, "����ع�ʱ��"
    	, ""
    	, CUSTOM_LEVEL
    );
    
    GetInt("\\HvDsp\\Camera\\Ctrl"
    	, "MaxPlus"
    	, &Get().cCamCfgParam.iMaxGain
    	, Get().cCamCfgParam.iMaxGain
    	, 70
    	, 360
    	, "�������"
    	, ""
    	, CUSTOM_LEVEL
    );

	GetInt("\\HvDsp\\Camera\\Ctrl"
		, "MinPSD"
		, &Get().cCamCfgParam.iMinPSD
		, Get().cCamCfgParam.iMinPSD
		, 100
		, 20000
		, "Ƶ��������Сֵ"
		, ""
		, CUSTOM_LEVEL
		);

    GetInt("\\HvDsp\\Camera\\Ctrl"
    	, "MaxPSD"
    	, &Get().cCamCfgParam.iMaxPSD
    	, Get().cCamCfgParam.iMaxPSD
    	, 100
    	, 20000
    	, "Ƶ���������ֵ"
    	, ""
    	, CUSTOM_LEVEL
    );

	GetEnum("\\HvDsp\\Camera\\Ctrl"
		, "CtrlCpl"
		, &Get().cTrackerCfgParam.nCtrlCpl
		, Get().cTrackerCfgParam.nCtrlCpl
		, "0:��ʹ��;1:ʹ��"
		, "�Զ�����ƫ�⾵"
		, "0:��ʹ��;1:ʹ��"
		, CUSTOM_LEVEL
		);

	GetInt("\\HvDsp\\Camera\\Ctrl"
		, "LightTypeCpl"
		, &Get().cTrackerCfgParam.nLightTypeCpl
		, Get().cTrackerCfgParam.nLightTypeCpl
		, 0
		, 13
		, "ƫ�⾵�Զ��л������ȵȼ���ֵ"
		, ""
		, CUSTOM_LEVEL
		);
    
    if(Get().cCamCfgParam.iMinPSD >= Get().cCamCfgParam.iMaxPSD)
    {
    	Get().cCamCfgParam.iMinPSD = 100;
    	Get().cCamCfgParam.iMaxPSD = 20000;
    	UpdateInt("\\HvDsp\\Camera\\Ctrl", "MinPSD", Get().cCamCfgParam.iMinPSD);
    	UpdateInt("\\HvDsp\\Camera\\Ctrl", "MaxPSD", Get().cCamCfgParam.iMaxPSD);
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
        	, CUSTOM_LEVEL
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
        	, CUSTOM_LEVEL
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
    
	return S_OK;
}

HRESULT CSWLPRParameter::InitCamCfgParam(CAM_CFG_PARAM *pCfgCamParam, const char **szLightType)
{
    if (pCfgCamParam->iMinExposureTime == 0)
    {
        float fTmp = pCfgCamParam->fltDB / 20;
        fTmp = pow((float)10.0, fTmp);
        pCfgCamParam->irgExposureTime[13] = pCfgCamParam->iMaxExposureTime;
        pCfgCamParam->irgGain[13] = pCfgCamParam->iMaxGain;

        for (int i = 12; i >= 0; i--)
        {
            int iNewExposureTime, iNewGain;
            int iLastExposureTime, iLastGain;
            iLastGain = pCfgCamParam->irgGain[i + 1];
            iNewGain = iLastGain - (int)(pCfgCamParam->fltDB * 10);
            iLastExposureTime = pCfgCamParam->irgExposureTime[i + 1];
            if ( iNewGain >= pCfgCamParam->iMinGain )
            {
                pCfgCamParam->irgExposureTime[i] = iLastExposureTime;
                pCfgCamParam->irgGain[i] = iNewGain;
            }
            else
            {
                iNewExposureTime = (int)(((float)iLastExposureTime / fTmp) + 0.5);
                pCfgCamParam->irgExposureTime[i] = iNewExposureTime;
                pCfgCamParam->irgGain[i] = iLastGain;
            }
        }
    }
    else
    {
        float fTmp = pCfgCamParam->fltDB / 20;
        fTmp = pow((float)10.0, fTmp);
        int iGainLevel = (int)(pCfgCamParam->fltDB * 10);

        pCfgCamParam->irgExposureTime[0] = pCfgCamParam->iMinExposureTime;
        pCfgCamParam->irgGain[0] = pCfgCamParam->iMinGain;

        for (int i = 1; i < 14; i++)
        {
            int iNewExposureTime, iNewGain;
            int iLastExposureTime, iLastGain;
            iLastExposureTime = pCfgCamParam->irgExposureTime[i - 1];
            iLastGain = pCfgCamParam->irgGain[i - 1];
            iNewExposureTime = (int)(((float)iLastExposureTime * fTmp) + 0.5);
            if ( iNewExposureTime <= pCfgCamParam->iMaxExposureTime )
            {
                pCfgCamParam->irgExposureTime[i] = iNewExposureTime;
                pCfgCamParam->irgGain[i] = iLastGain;
            }
            else
            {
                iNewGain = iLastGain + iGainLevel;
                if ( iNewGain > pCfgCamParam->iMaxGain )
                {
                    pCfgCamParam->irgExposureTime[i] = iLastExposureTime;
                    pCfgCamParam->irgGain[i] = iLastGain;
                }
                else
                {
                    pCfgCamParam->irgExposureTime[i] = iLastExposureTime;
                    pCfgCamParam->irgGain[i] = iNewGain;
                }
            }
        }
    }

    char szText[255];
    int iAGCDB = (pCfgCamParam->iMaxAGCLimit - pCfgCamParam->iMinAGCLimit) / 13;
    if (iAGCDB > 0)
    {
        for (int i = 0; i < 14; i++)
        {
            sprintf(szText, "\\HvDsp\\Camera\\%s", szLightType[i]);
            pCfgCamParam->irgAGCLimit[i] = pCfgCamParam->iMinAGCLimit + (iAGCDB * i);
            UpdateInt(szText, "AGCLimit", pCfgCamParam->irgAGCLimit[i]);
        }
        Get().cTrackerCfgParam.nMaxAGCTH = pCfgCamParam->irgAGCLimit[13];
    }

    if (pCfgCamParam->iEnableAGC)
    {
        for (int i = 0; i < 14; i++)
        {
            pCfgCamParam->irgExposureTime[i] = -1;
            pCfgCamParam->irgGain[i] = -1;
        }
    }

    for (int i = 0; i < 14; i++)
    {
        sprintf(szText, "\\HvDsp\\Camera\\%s", szLightType[i]);
        UpdateInt(szText, "ExposureTime", pCfgCamParam->irgExposureTime[i]);
        UpdateInt(szText, "Plus", pCfgCamParam->irgGain[i]);
    }

    return S_OK;
}

HRESULT CSWLPRParameter::Init368(VOID)
{
	/*
    GetEnum("\\CamApp"
    	, "SendType"
    	, &Get().cCamAppParam.iSendType
    	, Get().cCamAppParam.iSendType
    	, "-1:������;0:HvCam����Э��"
    	, "������ģʽ"
    	, ""
    	,PROJECT_LEVEL
    );
	*/

    GetEnum("\\CamApp"
    	, "JpegStream"
    	, &Get().cCamAppParam.iJpegStream
    	, Get().cCamAppParam.iJpegStream
    	, "0:��ʹ��;1:ʹ��"
    	, "Jpeg��ʹ��"
    	, ""
    	, CUSTOM_LEVEL
    );

    GetEnum("\\CamApp"
    	, "H264Stream"
    	, &Get().cCamAppParam.iH264Stream
    	, Get().cCamAppParam.iH264Stream
    	, "0:��ʹ��;1:ʹ��"
    	, "H.264��ʹ��"
    	, ""
    	, CUSTOM_LEVEL
    );

    if (1 == Get().cCamAppParam.iH264Stream)
    {
        Get().cCamAppParam.iJpegStream = 0;
    }

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
    	, "JpegCompressRateCapture"
    	, &Get().cCamAppParam.iJpegCompressRateCapture
    	, Get().cCamAppParam.iJpegCompressRateCapture
    	, 1
    	, 99
    	, "ץ��Jpegѹ��Ʒ��"
    	, ""
    	, CUSTOM_LEVEL
    );

    GetInt("\\CamApp"
    	, "IFrameInterval"
    	, &Get().cCamAppParam.iIFrameInterval
    	, Get().cCamAppParam.iIFrameInterval
    	, 1
    	, 15
    	, "H.264��I֡���"
    	, ""
    	, CUSTOM_LEVEL
    );

	GetEnum("\\CamApp"
    	, "Resolution"
    	, &Get().cCamAppParam.iResolution
    	, Get().cCamAppParam.iResolution
    	, "1:720P;2:1080P"
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
    	, "��λ��Kbps"
    	, CUSTOM_LEVEL
    );

		GetEnum("\\CamApp"
    	, "EnableCCDSignalCheck"
    	, &Get().cCamAppParam.iEnableCCDSignalCheck
    	, Get().cCamAppParam.iEnableCCDSignalCheck
    	, "0:������;1:����"
    	, "CCD����"
    	, ""
    	, CUSTOM_LEVEL
    );
    
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
    	, 70
    	, 360
    	, "AGC�����������"
    	, ""
    	, PROJECT_LEVEL
    );

    GetInt("\\CamApp"
    	, "AGCGainHOri"
    	, &Get().cCamAppParam.iAGCGainHOri
    	, Get().cCamAppParam.iAGCGainHOri
    	, 70
    	, 360
    	, "AGC�����������"
    	, ""
    	, CUSTOM_LEVEL
    );


    GetInt("\\CamApp"
    	, "GainR"
    	, &Get().cCamAppParam.iGainR
    	, Get().cCamAppParam.iGainR
    	, 0
    	, 255
    	, "R����"
    	, ""
    	, CUSTOM_LEVEL
    );

    GetInt("\\CamApp"
    	, "GainG"
    	, &Get().cCamAppParam.iGainG
    	, Get().cCamAppParam.iGainG
    	, 0
    	, 255
    	, "G����"
    	, ""
    	, CUSTOM_LEVEL
    );

    GetInt("\\CamApp"
    	, "GainB"
    	, &Get().cCamAppParam.iGainB
    	, Get().cCamAppParam.iGainB
    	, 0
    	, 255
    	, "B����"
    	, ""
    	, CUSTOM_LEVEL
    );

    GetInt("\\CamApp"
    	, "Gain"
    	, &Get().cCamAppParam.iGain
    	, Get().cCamAppParam.iGain
    	, 70
    	, 360
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

    GetInt("\\CamApp"
    	, "CaptureGainR"
    	, &Get().cCamAppParam.iCaptureGainR
    	, Get().cCamAppParam.iCaptureGainR
    	, 0
    	, 255
    	, "ץ��R����"
    	, ""
    	, CUSTOM_LEVEL
    );

    GetInt("\\CamApp"
    	, "CaptureGainG"
    	, &Get().cCamAppParam.iCaptureGainG
    	, Get().cCamAppParam.iCaptureGainG
    	, 0
    	, 255
    	, "ץ��G����"
    	, ""
    	, CUSTOM_LEVEL
    );

    GetInt("\\CamApp"
    	, "CaptureGainB"
    	, &Get().cCamAppParam.iCaptureGainB
    	, Get().cCamAppParam.iCaptureGainB
    	, 0
    	, 255
    	, "ץ��B����"
    	, ""
    	, CUSTOM_LEVEL
    );

    GetInt("\\CamApp"
    	, "CaptureGain"
    	, &Get().cCamAppParam.iCaptureGain
    	, Get().cCamAppParam.iCaptureGain
    	, 70
    	, 360
    	, "ץ������"
    	, ""
    	, CUSTOM_LEVEL
    );

    GetInt("\\CamApp"
    	, "CaptureShutter"
    	, &Get().cCamAppParam.iCaptureShutter
    	, Get().cCamAppParam.iCaptureShutter
    	, 0
    	, 30000
    	, "ץ�Ŀ���"
    	, "��λ��us"
    	, CUSTOM_LEVEL
    );

    GetEnum("\\CamApp"
    	, "EnableCaptureGainRGB"
    	, &Get().cCamAppParam.iEnableCaptureGainRGB
    	, Get().cCamAppParam.iEnableCaptureGainRGB
    	, "0:��ʹ��;1:ʹ��"
    	, "ץ��RGB����ʹ��"
    	, ""
    	, CUSTOM_LEVEL
    );

    GetEnum("\\CamApp"
    	, "EnableCaptureGain"
    	, &Get().cCamAppParam.iEnableCaptureGain
    	, Get().cCamAppParam.iEnableCaptureGain
    	, "0:��ʹ��;1:ʹ��"
    	, "ץ������ʹ��"
    	, ""
    	, CUSTOM_LEVEL
    );

    GetEnum("\\CamApp"
    	, "EnableCaptureShutter"
    	, &Get().cCamAppParam.iEnableCaptureShutter
    	, Get().cCamAppParam.iEnableCaptureShutter
    	, "0:��ʹ��;1:ʹ��"
    	, "ץ�Ŀ���ʹ��"
    	, ""
    	, CUSTOM_LEVEL
    );
/*
    GetEnum("\\CamApp"
    	, "CameraSideInstall"
    	, &Get().cCamAppParam.fIsSideInstall
    	, Get().cCamAppParam.fIsSideInstall
    	, "0:��װ;1:��װ"
    	, "ǰ�˾�ͷ��װ"
    	, ""
    	, CUSTOM_LEVEL
    );
	*/

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
    	, 100
    	, "Jpegѹ�����Զ���������"
    	, ""
    	, CUSTOM_LEVEL
    );

    GetInt("\\CamApp"
    	, "JpegCompressRateHigh"
    	, &Get().cCamAppParam.iJpegCompressRateH
    	, Get().cCamAppParam.iJpegCompressRateH
    	, 1
    	, 100
    	, "Jpegѹ�����Զ���������"
    	, ""
    	, CUSTOM_LEVEL
    );
/*
    GetInt("\\CamApp"
    	, "OutputFrameRate"
    	, &Get().cCamAppParam.iOutputFrameRate
    	, Get().cCamAppParam.iOutputFrameRate
    	, 0
    	, 15
    	, "������֡��"
    	, "�����Ƶ��������֡��"
    	, CUSTOM_LEVEL
    );
	*/

    GetEnum("\\CamApp"
    	, "AGCEnable"
    	, &Get().cCamAppParam.iAGCEnable
    	, Get().cCamAppParam.iAGCEnable
    	, "0:��ʹ��;1:ʹ��"
    	, "AGCʹ��"
    	, ""
    	, CUSTOM_LEVEL
    );

    GetEnum("\\CamApp"
    	, "AWBEnable"
    	, &Get().cCamAppParam.iAWBEnable
    	, Get().cCamAppParam.iAWBEnable
    	, "0:��ʹ��;1:ʹ��"
    	, "AWBʹ��"
    	, ""
    	, CUSTOM_LEVEL
    );

    GetEnum("\\CamApp"
    	, "FlashRateSynSignalEnable"
    	, &Get().cCamAppParam.iFlashRateSynSignalEnable
    	, Get().cCamAppParam.iFlashRateSynSignalEnable
    	, "0:��ʹ��;1:ʹ��"
    	, "Ƶ��ͬ���ź�ʹ��"
    	, ""
    	, CUSTOM_LEVEL
    );
	GetEnum("\\CamApp"
		, "FlashRateMultiplication"
		, &Get().cCamAppParam.iFlashRateMultiplication
		, Get().cCamAppParam.iFlashRateMultiplication
		, "0:��ʹ�ܱ�Ƶ;1:ʹ�ܱ�Ƶ"
		, "Ƶ����Ƶ"
		, ""
		, CUSTOM_LEVEL
		);
	GetEnum("\\CamApp"
		, "FlashRatePolarity"
		, &Get().cCamAppParam.iFlashRatePolarity
		, Get().cCamAppParam.iFlashRatePolarity
		, "0:������;1:������"
		, "Ƶ������"
		, ""
		, CUSTOM_LEVEL
		);
	GetInt("\\CamApp"
		, "FlashRatePulseWidth"
		, &Get().cCamAppParam.iFlashRatePulseWidth
		, Get().cCamAppParam.iFlashRatePulseWidth
		, 100
		, 20000
		, "Ƶ��������λ��΢�룩"
		, ""
		, CUSTOM_LEVEL
		);
	GetEnum("\\CamApp"
		, "FlashRateOutputType"
		, &Get().cCamAppParam.iFlashRateOutputType
		, Get().cCamAppParam.iFlashRateOutputType
		, "0:����(��ƽ);1:OC�����أ�"
		, "Ƶ���������"
		, ""
		, CUSTOM_LEVEL
		);

    GetEnum("\\CamApp"
    	, "CaptureSynSignalEnable"
    	, &Get().cCamAppParam.iCaptureSynSignalEnable
    	, Get().cCamAppParam.iCaptureSynSignalEnable
    	, "0:��ʹ��;1:ʹ��"
    	, "ץ��ͬ���ź�ʹ��"
    	, ""
    	, CUSTOM_LEVEL
    );
	GetEnum("\\CamApp"
		, "CaptureSynPolarity"
		, &Get().cCamAppParam.iCaptureSynPolarity
		, Get().cCamAppParam.iCaptureSynPolarity
		, "0:������;1:������"
		, "ץ�ļ���"
		, ""
		, CUSTOM_LEVEL
		);
	GetInt("\\CamApp"
		, "CaptureSynPulseWidth"
		, &Get().cCamAppParam.iCaptureSynPulseWidth
		, Get().cCamAppParam.iCaptureSynPulseWidth
		, 100
		, 10000
		, "ץ��������λ��΢�룩"
		, ""
		, CUSTOM_LEVEL
		);
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
		, "CaptureSynCoupling"
		, &Get().cCamAppParam.iCaptureSynCoupling
		, Get().cCamAppParam.iCaptureSynCoupling
		, "0:�����;1:���"
		, "ץ���Ƿ���Ƶ�����"
		, ""
		, CUSTOM_LEVEL
		);
/*
    GetEnum("\\CamApp"
    	, "ENetSyn"
    	, &Get().cCamAppParam.iENetSyn
    	, Get().cCamAppParam.iENetSyn
    	, "0:15fps;1:12.5fps"
    	, "����ͬ��ʹ��"
    	, ""
    	, CUSTOM_LEVEL
    );
	*/

	GetEnum("\\CamApp"
		, "ExternSyncMode"
		, &Get().cCamAppParam.iExternalSyncMode
		, Get().cCamAppParam.iExternalSyncMode
		, "0:��ͬ��;1:�ڲ�����ͬ��;2:�ⲿ�ź�ͬ��"
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

    GetEnum("\\CamApp"
    	, "CaptureEdge"
    	, &Get().cCamAppParam.iCaptureEdge
    	, Get().cCamAppParam.iCaptureEdge
    	, "0:�ⲿ�½��ش���;1:�ⲿ�����ش���;2:�������½��ض�����;3:������"
    	, "����ץ����"
    	, ""
    	, CUSTOM_LEVEL
    );

    GetEnum("\\CamApp"
    	, "FlashDifferentLaneEx"
    	, &Get().cCamAppParam.iFlashDifferentLaneExt
    	, Get().cCamAppParam.iFlashDifferentLaneExt
    	, "0:���ֳ�����;1:�ֳ�����"
    	, "����Ʒֳ�����(Ӳ����)"
    	, ""
    	, CUSTOM_LEVEL
    );
/*
    GetInt("\\CamApp\\Overlay"
    	, "Character_X"
    	, &Get().cCamAppParam.iX
    	, Get().cCamAppParam.iX
    	, 0
    	, 1600
    	, "�ַ���X����"
    	, "ע����װʱ�����Ϊ1600����װʱΪ1200����������Ϊ����ֵ"
    	, CUSTOM_LEVEL
    );

    GetInt("\\CamApp\\Overlay"
    	, "Character_Y"
    	, &Get().cCamAppParam.iY
    	, Get().cCamAppParam.iY
    	, 0
    	, 1600
    	, "�ַ���Y����"
    	, "ע����װʱ�����Ϊ1200����װʱΪ1600����������Ϊ����ֵ"
    	, CUSTOM_LEVEL
    );

    GetEnum("\\CamApp\\Overlay"
    	, "Character_DateFormat"
    	, &Get().cCamAppParam.iDateFormat
    	, Get().cCamAppParam.iDateFormat
    	, "0: '/';1: '-'"
    	, "���ڸ�ʽ"
    	, ""
    	, CUSTOM_LEVEL
    );

    GetInt("\\CamApp\\Overlay"
    	, "Character_FontColor"
    	, &Get().cCamAppParam.iFontColor
    	, Get().cCamAppParam.iFontColor
    	, 0
    	, 0x00ffffff
    	, "������ɫ"
    	, "������ɫΪRGB��ʽ"
    	, CUSTOM_LEVEL
    );

    GetString("\\CamApp\\Overlay"
    	, "Character"
    	, Get().cCamAppParam.szCharater
    	, Get().cCamAppParam.szCharater
    	, 255
    	, "���ӵ��ַ���"
    	, ""
    	, CUSTOM_LEVEL
    );

    GetEnum("\\CamApp\\Overlay"
    	, "EnableCharacter"
    	, &Get().cCamAppParam.iEnableCharacterOverlap
    	, Get().cCamAppParam.iEnableCharacterOverlap
    	, "0:��ʹ��;1:ʹ��"
    	, "�ַ�����ʹ��"
    	, ""
    	, CUSTOM_LEVEL
    );

    GetEnum("\\CamApp\\Overlay"
    	, "EnableFixedLight"
    	, &Get().cCamAppParam.iEnableFixedLight
    	, Get().cCamAppParam.iEnableFixedLight
    	, "0:���̶�;1:�̶�"
    	, "�ַ���������̶�����"
    	, ""
    	, CUSTOM_LEVEL
    );
	*/
   
	/*
    GetEnum("\\CamApp"
    	, "EnableH264BrightnessAdjust"
    	, &Get().cCamAppParam.fEnableH264BrightnessAdjust
    	, Get().cCamAppParam.fEnableH264BrightnessAdjust
    	, "0:��ʹ��;1:ʹ��"
    	, "ʹ��H264���ȵ���", "0:��ʹ��;1:ʹ��"
    	, CUSTOM_LEVEL
    );

    GetInt("\\CamApp"
    	, "AdjustPointX"
    	, &Get().cCamAppParam.iAdjustPointX
    	, Get().cCamAppParam.iAdjustPointX
    	, 1
    	, 254
    	, "H264���ȵ��ڵ�Xֵ"
    	, ""
    	, CUSTOM_LEVEL
    );
    
    GetInt("\\CamApp"
    	, "AdjustPointY"
    	, &Get().cCamAppParam.iAdjustPointY
    	, Get().cCamAppParam.iAdjustPointY
    	, 0
    	, 255
    	, "H264���ȵ��ڵ�Yֵ"
    	, ""
    	, CUSTOM_LEVEL
    );
	*/
		
    /*GetString("\\CamApp\\NetDisk"
    	, "NetDiskIP"
    	, Get().cCamAppParam.szNetDiskIP
    	, Get().cCamAppParam.szNetDiskIP
    	, sizeof(Get().cCamAppParam.szNetDiskIP)
    	, "����洢��IP��ַ"
    	, ""
    	, CUSTOM_LEVEL
    );
    
    GetInt("\\CamApp\\NetDisk"
    	, "NetDiskSpace"
    	, &Get().cCamAppParam.iNetDiskSpace
    	, Get().cCamAppParam.iNetDiskSpace
    	, 1
    	, 3696
    	, "����洢������,��λG (�洢��ʼ��ʱ������)"
    	, ""
    	, CUSTOM_LEVEL
    );

    GetString("\\CamApp\\NetDisk"
    	, "NFS"
    	, Get().cCamAppParam.szNFS
    	, Get().cCamAppParam.szNFS
    	, sizeof(Get().cCamAppParam.szNFS)
    	, "NFS·��"
    	, ""
    	, CUSTOM_LEVEL
    );

    GetString("\\CamApp\\NetDisk"
    , "NFSParam"
    , Get().cCamAppParam.szNFSParam
    , Get().cCamAppParam.szNFSParam
    , sizeof(Get().cCamAppParam.szNFSParam)
    , "NFS���ز���"
    , ""
    , PROJECT_LEVEL
    );*/
    
/*
// �Զ����������жϵ��������Զ��������ģ�����ж�
// 
    GetInt("\\CamApp\\Gamma"
    	, "AvgY"
    	, &Get().cCamAppParam.iAvgY
    	, Get().cCamAppParam.iAvgY
    	, 0
    	, 255
    	, "����������ֵ"
    	, "���ڴ���ֵ�ĲŽ���Gamma����"
    	, CUSTOM_LEVEL
    );
	*/

	GetInt("\\CamApp\\Gamma"
		, "Mode"
		, &Get().cCamAppParam.iGammaMode
		, Get().cCamAppParam.iGammaMode
		, 0
		, 3
		, "LUTͨ��ѡ��ģʽ"
		, "ͨ��ѡ��ֵ��0��JPEG��H264���ޣ�1��JPEG��H264�ޣ�2��JPEG��H264�У�3��JPEG��H264��"
		, CUSTOM_LEVEL
		);

    GetInt("\\CamApp\\Gamma"
    	, "X0"
    	, &Get().cCamAppParam.rgiGamma[0][0]
    	, Get().cCamAppParam.rgiGamma[0][0]
    	, 0
    	, 255
    	, "X0"
    	, ""
    	, CUSTOM_LEVEL
    );
    
    GetInt("\\CamApp\\Gamma"
    	, "Y0"
    	, &Get().cCamAppParam.rgiGamma[0][1]
    	, Get().cCamAppParam.rgiGamma[0][1]
    	, 0
    	, 255
    	, "Y0"
    	, ""
    	, CUSTOM_LEVEL
    );

    GetInt("\\CamApp\\Gamma"
    	, "X1"
    	, &Get().cCamAppParam.rgiGamma[1][0]
    	, Get().cCamAppParam.rgiGamma[1][0]
    	, 0
    	, 255
    	, "X1"
    	, ""
    	, CUSTOM_LEVEL
    );
    
    GetInt("\\CamApp\\Gamma"
    	, "Y1"
    	, &Get().cCamAppParam.rgiGamma[1][1]
    	, Get().cCamAppParam.rgiGamma[1][1]
    	, 0
    	, 255
    	, "Y1"
    	, ""
    	, CUSTOM_LEVEL
    );

    GetInt("\\CamApp\\Gamma"
    	, "X2"
    	, &Get().cCamAppParam.rgiGamma[2][0]
    	, Get().cCamAppParam.rgiGamma[2][0]
    	, 0
    	, 255
    	, "X2"
    	, ""
    	, CUSTOM_LEVEL
    );
    GetInt("\\CamApp\\Gamma"
    	, "Y2"
    	, &Get().cCamAppParam.rgiGamma[2][1]
    	, Get().cCamAppParam.rgiGamma[2][1]
    	, 0
    	, 255
    	, "Y2"
    	, ""
    	, CUSTOM_LEVEL
    );

    GetInt("\\CamApp\\Gamma"
    	, "X3"
    	, &Get().cCamAppParam.rgiGamma[3][0]
    	, Get().cCamAppParam.rgiGamma[3][0]
    	, 0
    	, 255
    	, "X3"
    	, ""
    	, CUSTOM_LEVEL
    );
    
    GetInt("\\CamApp\\Gamma"
    	, "Y3"
    	, &Get().cCamAppParam.rgiGamma[3][1]
    	, Get().cCamAppParam.rgiGamma[3][1]
    	, 0
    	, 255
    	, "Y3"
    	, ""
    	, CUSTOM_LEVEL
    );

    GetInt("\\CamApp\\Gamma"
    	, "X4"
    	, &Get().cCamAppParam.rgiGamma[4][0]
    	, Get().cCamAppParam.rgiGamma[4][0]
    	, 0
    	, 255
    	, "X4"
    	, ""
    	, CUSTOM_LEVEL
    );
    GetInt("\\CamApp\\Gamma"
    	, "Y4"
    	, &Get().cCamAppParam.rgiGamma[4][1]
    	, Get().cCamAppParam.rgiGamma[4][1]
    	, 0
    	, 255
    	, "Y4"
    	, ""
    	, CUSTOM_LEVEL
    );

    GetInt("\\CamApp\\Gamma"
    	, "X5"
    	, &Get().cCamAppParam.rgiGamma[5][0]
    	, Get().cCamAppParam.rgiGamma[5][0]
    	, 0
    	, 255
    	, "X5"
    	, ""
    	, CUSTOM_LEVEL
    );
    
    GetInt("\\CamApp\\Gamma"
    	, "Y5"
    	, &Get().cCamAppParam.rgiGamma[5][1]
    	, Get().cCamAppParam.rgiGamma[5][1]
    	, 0
    	, 255
    	, "Y5"
    	, ""
    	, CUSTOM_LEVEL
    );

    GetInt("\\CamApp\\Gamma"
    	, "X6"
    	, &Get().cCamAppParam.rgiGamma[6][0]
    	, Get().cCamAppParam.rgiGamma[6][0]
    	, 0
    	, 255
    	, "X6"
    	, ""
    	, CUSTOM_LEVEL
    );
    GetInt("\\CamApp\\Gamma"
    	, "Y6"
    	, &Get().cCamAppParam.rgiGamma[6][1]
    	, Get().cCamAppParam.rgiGamma[6][1]
    	, 0
    	, 255
    	, "Y6"
    	, ""
    	, CUSTOM_LEVEL
    );

    GetInt("\\CamApp\\Gamma"
    	, "X7"
    	, &Get().cCamAppParam.rgiGamma[7][0]
    	, Get().cCamAppParam.rgiGamma[7][0]
    	, 0
    	, 255
    	, "X7"
    	, ""
    	, CUSTOM_LEVEL
    );
    GetInt("\\CamApp\\Gamma"
    	, "Y7"
    	, &Get().cCamAppParam.rgiGamma[7][1]
    	, Get().cCamAppParam.rgiGamma[7][1]
    	, 0
    	, 255
    	, "Y7"
    	, ""
    	, CUSTOM_LEVEL
    );

    char szAGC[255];
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
    }
    //У��gamma�Ƿ����
    bool fValid = true;
    for (int i = 1; i < 8; i++)
    {
        if (Get().cCamAppParam.rgiGamma[i - 1][0] > Get().cCamAppParam.rgiGamma[i][0]
                || Get().cCamAppParam.rgiGamma[i - 1][0] < 0 || Get().cCamAppParam.rgiGamma[i - 1][0] > 255
                || Get().cCamAppParam.rgiGamma[i][0] < 0 || Get().cCamAppParam.rgiGamma[i][0] > 255
                || Get().cCamAppParam.rgiGamma[i - 1][1] < 0 || Get().cCamAppParam.rgiGamma[i - 1][1] > 255
                || Get().cCamAppParam.rgiGamma[i][1] < 0 || Get().cCamAppParam.rgiGamma[i][1] > 255)
        {
            fValid = false;
            break;
        }
    }
    if (!fValid)
    {
        for (int i = 0; i < 8; i++)
        {
            Get().cCamAppParam.rgiGamma[i][0] = Get().cCamAppParam.rgiDefGamma[i][0];
            Get().cCamAppParam.rgiGamma[i][1] = Get().cCamAppParam.rgiDefGamma[i][1];
        }
        
        UpdateInt("\\CamApp\\Gamma", "X0", Get().cCamAppParam.rgiGamma[0][0]);
        UpdateInt("\\CamApp\\Gamma", "Y0", Get().cCamAppParam.rgiGamma[0][1]);
        UpdateInt("\\CamApp\\Gamma", "X1", Get().cCamAppParam.rgiGamma[1][0]);
        UpdateInt("\\CamApp\\Gamma", "Y1", Get().cCamAppParam.rgiGamma[1][1]);
        UpdateInt("\\CamApp\\Gamma", "X2", Get().cCamAppParam.rgiGamma[2][0]);
        UpdateInt("\\CamApp\\Gamma", "Y2", Get().cCamAppParam.rgiGamma[2][1]);
        UpdateInt("\\CamApp\\Gamma", "X3", Get().cCamAppParam.rgiGamma[3][0]);
        UpdateInt("\\CamApp\\Gamma", "Y3", Get().cCamAppParam.rgiGamma[3][1]);
        UpdateInt("\\CamApp\\Gamma", "X4", Get().cCamAppParam.rgiGamma[4][0]);
        UpdateInt("\\CamApp\\Gamma", "Y4", Get().cCamAppParam.rgiGamma[4][1]);
        UpdateInt("\\CamApp\\Gamma", "X5", Get().cCamAppParam.rgiGamma[5][0]);
        UpdateInt("\\CamApp\\Gamma", "Y5", Get().cCamAppParam.rgiGamma[5][1]);
        UpdateInt("\\CamApp\\Gamma", "X6", Get().cCamAppParam.rgiGamma[6][0]);
        UpdateInt("\\CamApp\\Gamma", "Y6", Get().cCamAppParam.rgiGamma[6][1]);
        UpdateInt("\\CamApp\\Gamma", "X7", Get().cCamAppParam.rgiGamma[7][0]);
        UpdateInt("\\CamApp\\Gamma", "Y7", Get().cCamAppParam.rgiGamma[7][1]);
    }
    
    GetEnum("\\CamApp"
    	, "EnableDCAperture"
    	, &Get().cCamAppParam.iEnableDCAperture
    	, Get().cCamAppParam.iEnableDCAperture
    	, "0:ʹ��;1:ʹ��"
    	, "ʹ��DC��Ȧ"
    	, ""
    	, CUSTOM_LEVEL
    );
    
    GetEnum("\\CamApp"
    	, "EnableSharpen"
    	, &Get().cCamAppParam.iEnableSharpen
    	, Get().cCamAppParam.iEnableSharpen
    	, "0:ʹ��;1:ʹ��"
    	, "ʹ����"
    	, ""
    	, CUSTOM_LEVEL
    );
    
    GetEnum("\\CamApp"
    	, "SharpenCapture"
    	, &Get().cCamAppParam.iSharpenCapture
    	, Get().cCamAppParam.iSharpenCapture
    	, "0:������ͼ;1:��ץ��ͼ"
    	, "��ץ��ͼ"
    	, ""
    	, CUSTOM_LEVEL
    );
    
    GetInt("\\CamApp"
    	, "SharpenThreshold"
    	, &Get().cCamAppParam.iSharpenThreshold
    	, Get().cCamAppParam.iSharpenThreshold
    	, 0
    	, 100
    	, "����ֵ"
    	, ""
    	, CUSTOM_LEVEL
    );
    
	// ������������棬���⻹��Ҫת��һ�¡�
	int iMode = Get().cCamAppParam.iColorMatrixMode - 1;
    GetEnum("\\CamApp"
    	, "ColorMatrixMode"
    	, &iMode
    	, iMode
    	, "0:Ϊ�ر�ʹ��;1:Ϊʹ��ץ��;2:Ϊʹ������֡"
    	, "ͼ����ǿ"
    	, ""
    	, CUSTOM_LEVEL
    );
	Get().cCamAppParam.iColorMatrixMode = iMode + 1;
    
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
    	, "HueThreshold"
    	, &Get().cCamAppParam.iHueThreshold
    	, Get().cCamAppParam.iHueThreshold
    	, -180
    	, 180
    	, "ɫ��"
    	, ""
    	, CUSTOM_LEVEL
    );
    
    GetEnum("\\CamApp"
    	, "TemThreshold"
    	, &Get().cCamAppParam.iTemThreshold
    	, Get().cCamAppParam.iTemThreshold
    	, "0:0;32:32;40:40;52:52;55:55;60:60;70:70"
    	, "ɫ��"
    	, ""
    	, CUSTOM_LEVEL
    );    
    
    GetEnum("\\CamApp"
    	, "HazeRemovalMode"
    	, &Get().cCamAppParam.iHazeRemovalMode
    	, Get().cCamAppParam.iHazeRemovalMode
    	, "1:Ϊ�ر�ʹ��;2:Ϊʹ��ץ��;3:Ϊʹ������֡"
    	, "������"
    	, ""
    	, CUSTOM_LEVEL
    );
    
    GetInt("\\CamApp"
    	, "HazeRemovalThreshold"
    	, &Get().cCamAppParam.iHazeRemovalThreshold
    	, Get().cCamAppParam.iHazeRemovalThreshold
    	, 0
    	, 10
    	, "������ֵ"
    	, ""
    	, CUSTOM_LEVEL
    );

	return S_OK;
}

HRESULT CSWLPRParameter::InitFrontPannel(VOID)
{
	  GetEnum("\\CamApp\\FrontPannel"
	  	, "PannelStyle"
	  	, &Get().cFrontPannelParam.iUsedAutoControllMode
	  	, Get().cFrontPannelParam.iUsedAutoControllMode
	  	, "0:�����;1:���ذ�"
	  	, "���ư�����"
	  	, ""
	  	, CUSTOM_LEVEL
	  );

    GetEnum("\\CamApp\\FrontPannel"
    	, "AutoRun"
    	, &Get().cFrontPannelParam.iAutoRunFlag
    	, Get().cFrontPannelParam.iAutoRunFlag
    	, "0:Ϊ�ֶ�����;1:Ϊ�Զ�����"
    	, "���з�ʽ"
    	, ""
    	, CUSTOM_LEVEL
    );

    GetEnum("\\CamApp\\FrontPannel"
    	, "DivFrequency"
    	, &Get().cFrontPannelParam.iDivFrequency
    	, Get().cFrontPannelParam.iDivFrequency
    	, "1:1;2:2;4:4;8:8"
    	, "��Ƶϵ��"
    	, ""
    	, CUSTOM_LEVEL
    );

    GetInt("\\CamApp\\FrontPannel"
    	, "OutPutDelay"
    	, &Get().cFrontPannelParam.iOutPutDelay
    	, Get().cFrontPannelParam.iOutPutDelay
    	, 0
    	, 150
    	, "����ӳ�"
    	, "1��Ӧ0.1MS"
    	, CUSTOM_LEVEL
    );

    GetInt("\\CamApp\\FrontPannel"
    	, "PulseWidthMin"
    	, &Get().cFrontPannelParam.iPulseWidthMin
    	, Get().cFrontPannelParam.iPulseWidthMin
    	, 1
    	, 72
    	, "Ƶ��������Сֵ"
    	, ""
    	, CUSTOM_LEVEL
    );

    GetInt("\\CamApp\\FrontPannel"
    	, "PulseWidthMax"
    	, &Get().cFrontPannelParam.iPulseWidthMax
    	, Get().cFrontPannelParam.iPulseWidthMax
    	, 9
    	, 80
    	, "Ƶ���������ֵ"
    	, ""
    	, CUSTOM_LEVEL
    );

    if (Get().cFrontPannelParam.iPulseWidthMax - Get().cFrontPannelParam.iPulseWidthMin < 8)
    {
        Get().cFrontPannelParam.iPulseWidthMin = 5;
        Get().cFrontPannelParam.iPulseWidthMax = 42;
        
        UpdateInt("\\CamApp\\FrontPannel"
        	, "PulseWidthMin"
        	, Get().cFrontPannelParam.iPulseWidthMin
        );

        UpdateInt("\\CamApp\\FrontPannel"
        	, "PulseWidthMax"
        	, Get().cFrontPannelParam.iPulseWidthMax
      	);
    }

    Get().cFrontPannelParam.iPulseStep = (int)((Get().cFrontPannelParam.iPulseWidthMax - Get().cFrontPannelParam.iPulseWidthMin) / 7);
    GetInt("\\CamApp\\FrontPannel\\Flash1Info"
    	, "PulseWidth"
    	, &Get().cFrontPannelParam.iFlash1PulseWidth
    	, Get().cFrontPannelParam.iFlash1PulseWidth
    	, 100
    	, 4000
    	, "����"
    	, ""
    	, CUSTOM_LEVEL
    );

    GetEnum("\\CamApp\\FrontPannel\\Flash1Info"
    	, "Polarity"
    	, &Get().cFrontPannelParam.iFlash1Polarity
    	, Get().cFrontPannelParam.iFlash1Polarity
    	, "0:������;1:������"
    	, "����"
    	, ""
    	, CUSTOM_LEVEL
    );

    GetEnum("\\CamApp\\FrontPannel\\Flash1Info"
    	, "Coupling"
    	, &Get().cFrontPannelParam.iFlash1Coupling
    	, Get().cFrontPannelParam.iFlash1Coupling
    	, "0:����ϵ�Ƶ��;1:Ϊ��ϵ�Ƶ��"
			, "���"
			, ""
			, CUSTOM_LEVEL
		);

    GetEnum("\\CamApp\\FrontPannel\\Flash1Info"
    	, "ResistorMode"
    	, &Get().cFrontPannelParam.iFlash1ResistorMode
    	, Get().cFrontPannelParam.iFlash1ResistorMode
    	, "0:��·����;1:��ƽ����"
    	, "��������"
    	, ""
    	, CUSTOM_LEVEL
    );

    GetInt("\\CamApp\\FrontPannel\\Flash2Info"
    	, "PulseWidth"
    	, &Get().cFrontPannelParam.iFlash2PulseWidth
    	, Get().cFrontPannelParam.iFlash2PulseWidth
    	, 100
    	, 4000
    	, "����"
    	, ""
    	, CUSTOM_LEVEL
    );

    GetEnum("\\CamApp\\FrontPannel\\Flash2Info"
    	, "Polarity"
    	, &Get().cFrontPannelParam.iFlash2Polarity
    	, Get().cFrontPannelParam.iFlash2Polarity
    	, "0:������;1:������"
    	, "����"
    	, ""
    	, CUSTOM_LEVEL
    );

    GetEnum("\\CamApp\\FrontPannel\\Flash2Info"
    	, "Coupling"
    	, &Get().cFrontPannelParam.iFlash2Coupling
    	, Get().cFrontPannelParam.iFlash2Coupling
    	, "0:����ϵ�Ƶ��;1:��ϵ�Ƶ��"
			, "���"
			, ""
			, CUSTOM_LEVEL
		);

    GetEnum("\\CamApp\\FrontPannel\\Flash2Info"
    	, "ResistorMode"
    	, &Get().cFrontPannelParam.iFlash2ResistorMode
    	, Get().cFrontPannelParam.iFlash2ResistorMode
    	, "0:��·����;1:��ƽ����"
    	, "��������"
    	, ""
    	, CUSTOM_LEVEL
    );

    GetInt("\\CamApp\\FrontPannel\\Flash3Info"
    	, "PulseWidth"
    	, &Get().cFrontPannelParam.iFlash3PulseWidth
    	, Get().cFrontPannelParam.iFlash3PulseWidth
    	, 100
    	, 4000
    	, "����"
    	, ""
    	, CUSTOM_LEVEL
    );

    GetEnum("\\CamApp\\FrontPannel\\Flash3Info"
    	, "Polarity"
    	, &Get().cFrontPannelParam.iFlash3Polarity
    	, Get().cFrontPannelParam.iFlash3Polarity
    	, "0:������;1:������"
    	, "����"
    	, ""
    	, CUSTOM_LEVEL
    );

    GetEnum("\\CamApp\\FrontPannel\\Flash3Info"
    	, "Coupling"
    	, &Get().cFrontPannelParam.iFlash3Coupling
    	, Get().cFrontPannelParam.iFlash3Coupling
    	, "0:����ϵ�Ƶ��;1:��ϵ�Ƶ��"
    	, "���"
    	, ""
    	, CUSTOM_LEVEL
    );

    GetEnum("\\CamApp\\FrontPannel\\Flash3Info"
    	, "ResistorMode"
    	, &Get().cFrontPannelParam.iFlash3ResistorMode
    	, Get().cFrontPannelParam.iFlash3ResistorMode
    	, "0:��·����;1:��ƽ����"
    	, "��������"
    	, ""
    	, CUSTOM_LEVEL
    );
        
	return S_OK;
}

HRESULT CSWLPRParameter::InitCharacter(VOID)
{

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
    	, 0
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
    	, ""
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
    	, 1936
    	, "X����"
    	, ""
    	, CUSTOM_LEVEL
    );
    
  GetInt("\\Overlay\\JPEG"
    	, "Y"
    	, &Get().cOverlay.cJPEGInfo.iY
    	, Get().cOverlay.cJPEGInfo.iY
    	, 0
    	, 1425
    	, "Y����"
    	, ""
    	, CUSTOM_LEVEL
    ); 
    
  GetInt("\\Overlay\\JPEG"
    	, "Size"
    	, &Get().cOverlay.cJPEGInfo.iFontSize
    	, Get().cOverlay.cJPEGInfo.iFontSize
    	, 0
    	, 128
    	, "�����С"
    	, ""
    	, CUSTOM_LEVEL
    ); 
  /*  
  GetInt("\\Overlay\\JPEG"
    	, "Color"
    	, (INT *)&Get().cOverlay.cJPEGInfo.iColor
    	, Get().cOverlay.cJPEGInfo.iColor
    	, 0x00000000
    	, 0x00FFFFFF
    	, "������ɫ"
    	, ""
    	, CUSTOM_LEVEL
    );
  */  
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
  Get().cOverlay.cJPEGInfo.iColor = (dwR | (dwG << 8) | (dwB << 16));
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
    	, ""
    	, sizeof(Get().cOverlay.cJPEGInfo.szInfo)
    	, ""
    	, "������Ϣ"
    	, CUSTOM_LEVEL
    );
    
   // todo.�ַ����Ӽ����ַ��ܳ��������⣬������ʱ��������ո���β�������������������ʾ��ȫ��
   if( strlen(Get().cOverlay.cJPEGInfo.szInfo) + 3 < sizeof(Get().cOverlay.cJPEGInfo.szInfo) )
   {
 	  strcpy(Get().cOverlay.cJPEGInfo.szInfo + strlen(Get().cOverlay.cJPEGInfo.szInfo), "  ");
 	  SW_TRACE_NORMAL("<param>change overlay jpeg len.%s", Get().cOverlay.cJPEGInfo.szInfo);
   }
    
  GetEnum("\\Overlay\\JPEG"
    	, "Plate"
    	, &Get().cOverlay.cJPEGInfo.fEnablePlate
    	, Get().cOverlay.cJPEGInfo.fEnablePlate
    	, "0:��ʹ��;1:ʹ��"
    	, "���ӳ��ƺ���"
    	, ""
    	, CUSTOM_LEVEL
    );
	return S_OK;
}
 
 HRESULT CSWLPRParameter::InitGB28181(VOID)
 {

     SW_TRACE_NORMAL(" HRESULT CSWLPRParameter::InitGB28181(VOID)");

     GetEnum("\\GB28181"
         , "Enable"
         , &Get().cGb28181.fEnalbe
         , Get().cGb28181.fEnalbe
         , "0:�ر�;1:����"
         , "����"
         , ""
         , CUSTOM_LEVEL
     );

     // �޸�I֡���
/*
     if( Get().cGb28181.fEnalbe )
     {
    	 Get().cCamAppParam.iIFrameInterval = 4;
         UpdateInt("\\CamApp"
             , "IFrameInterval"
             , Get().cCamAppParam.iIFrameInterval
             );
     }
*/
     
     GetString("\\GB28181"
          , "ServerID"
          , Get().cGb28181.szServerID
          , ""
          , sizeof(Get().cGb28181.szServerID)
          , "������ID"
          , "������ID"
          , CUSTOM_LEVEL
      );
     GetString("\\GB28181"
          , "ServerRegion"
          , Get().cGb28181.szServerRegion
          , ""
          , sizeof(Get().cGb28181.szServerRegion)
          , "��������"
          , "�������� Ϊ��ĬĬ����������IDǰ10λ"
          , PROJECT_LEVEL
      );
     GetString("\\GB28181"
          , "IPC_ID"
          , Get().cGb28181.szIPC_ID
          , ""
          , sizeof(Get().cGb28181.szIPC_ID)
          , "�豸ID"
          , "�豸ID"
          , CUSTOM_LEVEL
      );
     GetString("\\GB28181"
          , "IPC_Region"
          , Get().cGb28181.szIPC_Region
          , ""
          , sizeof(Get().cGb28181.szIPC_Region)
          , "�豸��"
          , "�豸�� Ϊ��ĬĬ�����豸IDǰ10λ"
          , PROJECT_LEVEL
      );
     GetString("\\GB28181"
          , "szAlarmID"
          , Get().cGb28181.szAlarmID
          , ""
          , sizeof(Get().cGb28181.szAlarmID)
          , "����ID"
          , "����ID"
          , CUSTOM_LEVEL
      );
     GetString("\\GB28181"
          , "szPassWord"
          , Get().cGb28181.szPassWord
          , ""
          , sizeof(Get().cGb28181.szPassWord)
          , "ע������"
          , "ע������"
          , CUSTOM_LEVEL
      );


     GetString("\\GB28181"
          , "ServerIP"
          , Get().cGb28181.szServerIP
          , ""
          , sizeof(Get().cGb28181.szServerIP)
          , "������IP��ַ"
          , "������IP��ַ"
          , CUSTOM_LEVEL
      );
     GetInt("\\GB28181"
         , "ServerPort"
         , &Get().cGb28181.iServerPort
         , Get().cGb28181.iServerPort
         , 0
         , 10000
         , "�������˿�"
         , ""
         , CUSTOM_LEVEL
     );

		 
	  GetEnum("\\GB28181"
         , "VideoSaveEnable"
         , &Get().cGb28181.fVideoSaveEnable
         , Get().cGb28181.fVideoSaveEnable
         , "0:�ر�;1:����"
         , "����¼��洢����"
         , "�Ƿ�ʵʱ����¼��洢���ء���Ӧ�ֶ�¼����ֶ�ֹͣ¼��"
         , CUSTOM_LEVEL
     );
     SW_TRACE_DEBUG("GetEnum.VideoSaveEnable:%d\n",Get().cGb28181.fVideoSaveEnable);
	 
	 
     GetInt("\\GB28181"
         , "IPC_Port"
         , &Get().cGb28181.iIPC_Port
         , Get().cGb28181.iIPC_Port
         , 0
         , 10000
         , "�豸��Ϣ�����˿�"
         , ""
         , PROJECT_LEVEL
     );
     GetInt("\\GB28181"
         , "IPC_VideoPort"
         , &Get().cGb28181.iIPC_VideoPort
         , Get().cGb28181.iIPC_VideoPort
         , 0
         , 10000
         , "�豸ý�巢�Ͷ˿�"
         , ""
         , PROJECT_LEVEL
     );

     GetString("\\GB28181"
          , "szLongitude"
          , Get().cGb28181.szLongitude
          , ""
          , sizeof(Get().cGb28181.szLongitude)
          , "����"
          , "����"
          , PROJECT_LEVEL
      );
     GetString("\\GB28181"
          , "szLatitude"
          , Get().cGb28181.szLatitude
          , ""
          , sizeof(Get().cGb28181.szLatitude)
          , "γ��"
          , "γ��"
          , PROJECT_LEVEL
      );
     GetString("\\GB28181"
          , "szManufacturer"
          , Get().cGb28181.szManufacturer
          , ""
          , sizeof(Get().cGb28181.szManufacturer)
          , "����"
          , "����"
          , PROJECT_LEVEL
      );
     GetString("\\GB28181"
          , "szBlock"
          , Get().cGb28181.szBlock
          , ""
          , sizeof(Get().cGb28181.szBlock)
          , "����"
          , "����"
          , PROJECT_LEVEL
      );
     GetString("\\GB28181"
          , "szAddress"
          , Get().cGb28181.szAddress
          , ""
          , sizeof(Get().cGb28181.szAddress)
          , "��װ��ַ"
          , "��װ��ַ"
          , PROJECT_LEVEL
      );
     GetString("\\GB28181"
          , "szOwner"
          , Get().cGb28181.szOwner
          , ""
          , sizeof(Get().cGb28181.szOwner)
          , "�豸����"
          , "�豸����"
          , PROJECT_LEVEL
      );
     GetString("\\GB28181"
          , "szCivilCode"
          , Get().cGb28181.szCivilCode
          , ""
          , sizeof(Get().cGb28181.szCivilCode)
          , "��������"
          , "��������"
          , PROJECT_LEVEL
           );


    SW_TRACE_NORMAL(" HRESULT CSWLPRParameter::InitGB28181(VOID) end");

    return S_OK;
 }

HRESULT CSWLPRParameter::InitONVIF(VOID)
{

    SW_TRACE_NORMAL(" HRESULT CSWLPRParameter::InitONVIF(VOID)");


	GetEnum("\\ONVIF"
			 , "OnvifEnable"
			 , &Get().cOnvifParam.iOnvifEnable
			 , Get().cOnvifParam.iOnvifEnable
			 , "0:��ʹ��;1:ʹ��"
			 , "����"
			 , ""
             , PROJECT_LEVEL);
	 GetEnum("\\ONVIF"
			 , "AuthenticateEnable"
			 , &Get().cOnvifParam.iAuthenticateEnable
			 , Get().cOnvifParam.iAuthenticateEnable
			 , "0:��ʹ��;1:ʹ��"
			 , "��Ȩ"
			 , "ʹ��ʱ��Ҫ�û�������ܽ��в���"
             , PROJECT_LEVEL);

	 //Ϊ��ͳһ��RTSP�ļ�Ȩ��ONVIF�ı���һ��
	 Get().cRtspParam.iAuthenticateEnable = Get().cOnvifParam.iAuthenticateEnable;
	 Get().cRtspParam.iRTSPStreamNum = 1;
	 GetEnum("\\ONVIF"
			 , "AutoControlBitrateEnable"
			 , &Get().cRtspParam.iAutoControlBitrateEnable
			 , Get().cRtspParam.iAutoControlBitrateEnable
			 , "0:��ʹ��;1:ʹ��"
			 , "�Զ��������ʿ���"
			 , "���������ӵ�����Զ�����������Ӧ����"
             , PROJECT_LEVEL);

    SW_TRACE_NORMAL(" HRESULT CSWLPRParameter::InitONVIF(VOID) End");

	return S_OK;
}
