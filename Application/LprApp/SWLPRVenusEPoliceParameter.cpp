/*
 * CSWLPRVenusEPoliceGateParameter.cpp
 *
 *  Created on: 2013��12��27��
 *      Author: qinjj
 */
#include "SWFC.h"
#include "SWLPRApplication.h"
#include "SWLPRVenusEPoliceParameter.h"


CSWLPRVenusEPoliceParameter::CSWLPRVenusEPoliceParameter()
{
	//����ģʽ
	Get().nWorkModeIndex = PRM_ELECTRONIC_POLICE;
	m_strWorkMode.Format("���Ӿ���");
	Get().nCameraWorkMode = 6;		//���Ӿ����������ģʽ
	Get().nMCFWUsecase = IMX178_WITHOUTCVBS;

}

CSWLPRVenusEPoliceParameter::~CSWLPRVenusEPoliceParameter()
{
}

// �ָ�Ĭ��
void CSWLPRVenusEPoliceParameter::ResetParam(VOID)
{

	//�ȵ��û���
	CSWLPRParameter::ResetParam();

	/*���ݾ��巽�����ò���*/
	
	Get().nLogLevel = 1;
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
  Get().cTrackerCfgParam.nProcessPlate_LightBlue = 1;
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
  Get().cTrackerCfgParam.nRoadLineNumber = 2;
  Get().cTrackerCfgParam.nOutputRoadTypeName = 0;
  Get().cTrackerCfgParam.nCaptureOnePos = 0;
  Get().cTrackerCfgParam.nCaptureTwoPos = 0;
  Get().cTrackerCfgParam.nCaptureThreePos = 0;
  Get().cTrackerCfgParam.cActionDetect.iDetectCarStopTime = 10;
  Get().cTrackerCfgParam.nDetReverseRunEnable = 1;
  Get().cTrackerCfgParam.nSpan = 5;
  Get().cTrackerCfgParam.iRoadNumberBegin = 0;
  Get().cTrackerCfgParam.iStartRoadNum = 0;
  Get().cTrackerCfgParam.nFlashLightThreshold = 25;
  Get().cTrackerCfgParam.fDoublePlateEnable = 1;
  Get().cTrackerCfgParam.nRecogAsteriskThreshold = 0;
  Get().cTrackerCfgParam.fUseEdgeMethod = FALSE;
  Get().cTrackerCfgParam.fEnableT1Model = FALSE;
  Get().cTrackerCfgParam.fEnableDefaultWJ = TRUE;
  Get().cTrackerCfgParam.nRecogGxPolice = 1;
  Get().cTrackerCfgParam.fEnableAlpha_5 = FALSE;
  Get().cTrackerCfgParam.nCarArrivedPos = 75;
  Get().cTrackerCfgParam.nCarArrivedPosNoPlate = 55;
  Get().cTrackerCfgParam.nCarArrivedPosYellow = 70;
  Get().cTrackerCfgParam.nProcessType = 0;
  Get().cTrackerCfgParam.nOutPutType = 0;
  Get().cTrackerCfgParam.nNightPlus = 0;
  Get().cTrackerCfgParam.nWalkManSpeed = 100;
  Get().cTrackerCfgParam.fltBikeSensitivity = 4.0f;
  Get().cTrackerCfgParam.nCarArrivedDelay = 0;
  Get().cTrackerCfgParam.nVoteFrameNum = 35;
  Get().cTrackerCfgParam.nMaxEqualFrameNumForVote = 15;
  Get().cTrackerCfgParam.nBlockTwinsTimeout = 180;
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
  Get().cTrackerCfgParam.nCarArriveTrig = 0;
  Get().cTrackerCfgParam.nTriggerPlateType = 0;
  Get().cTrackerCfgParam.nEnableProcessBWPlate = 0;
	Get().cTrackerCfgParam.iPlatform = 0;
	Get().cTrackerCfgParam.iSpeedLimit = 0;
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
  Get().cTrackerCfgParam.cDetectArea.DetectorAreaRight = 100;
  Get().cTrackerCfgParam.cDetectArea.DetectorAreaTop = 0;
  Get().cTrackerCfgParam.cDetectArea.DetectorAreaBottom = 40;
  Get().cTrackerCfgParam.cDetectArea.nDetectorMinScaleNum = 6;
  Get().cTrackerCfgParam.cDetectArea.nDetectorMaxScaleNum = 12;

  //����ɨ������Ĭ��ֵ
  Get().cTrackerCfgParam.cTrapArea.fEnableDetAreaCtrl = FALSE;
	Get().cTrackerCfgParam.cTrapArea.TopLeftX = 0;
	Get().cTrackerCfgParam.cTrapArea.TopLeftY = 0;
	Get().cTrackerCfgParam.cTrapArea.TopRightX = 100;
	Get().cTrackerCfgParam.cTrapArea.TopRightY = 100;
	Get().cTrackerCfgParam.cTrapArea.BottomLeftX = 0;
	Get().cTrackerCfgParam.cTrapArea.BottomLeftY = 0;
	Get().cTrackerCfgParam.cTrapArea.BottomRightX = 100;
	Get().cTrackerCfgParam.cTrapArea.BottomRightY = 100;
	Get().cTrackerCfgParam.cTrapArea.nDetectorMinScaleNum = 6;
	Get().cTrackerCfgParam.cTrapArea.nDetectorMaxScaleNum = 12;

	// ץ��ͼʶ�����
	Get().cTrackerCfgParam.cRecogSnapArea.fEnableDetAreaCtrl = FALSE;
  Get().cTrackerCfgParam.cRecogSnapArea.DetectorAreaLeft = 0;
  Get().cTrackerCfgParam.cRecogSnapArea.DetectorAreaRight = 100;
  Get().cTrackerCfgParam.cRecogSnapArea.DetectorAreaTop = 0;
  Get().cTrackerCfgParam.cRecogSnapArea.DetectorAreaBottom = 40;
  Get().cTrackerCfgParam.cRecogSnapArea.nDetectorMinScaleNum = 6;
  Get().cTrackerCfgParam.cRecogSnapArea.nDetectorMaxScaleNum = 12;

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

  //��Ƶ������
  Get().cTrackerCfgParam.cVideoDet.nVideoDetMode = USE_VIDEODET;
  Get().cTrackerCfgParam.cVideoDet.nDayBackgroudThreshold = 20;
  Get().cTrackerCfgParam.cVideoDet.nNightBackgroudThreshold = 50;
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

	//��������Ĭ��ֵ
	Get().cResultSenderParam.cAutoLinkParam.iAutoLinkPort = 6665;

	//�ռ�궨�����в���
	Get().cTrackerCfgParam.cScaleSpeed.fEnable = 1;
	Get().cTrackerCfgParam.cScaleSpeed.nMethodsFlag = 1;
	Get().cTrackerCfgParam.cScaleSpeed.fltCameraHigh = 6.5f;
	Get().cTrackerCfgParam.cScaleSpeed.fltAdjustCoef = 1.050f;
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
	//Ĭ�Ϲ��˲��ϸ�Υ��
	Get().cResultSenderParam.iFilterUnSurePeccancy = 1;
	//��ƼӺ���ֵ
	Get().cResultSenderParam.nRedPosBrightness = 55;
	//��ͨ�¼����
	Get().cResultSenderParam.iCheckEventTime = 1;
	//���Ӿ��죬Ĭ�����м�⿪��Ϊ��
	Get().cTrackerCfgParam.nDetReverseRunEnable = 1;
	// T-1
	Get().cTrackerCfgParam.fEnableT1Model = 1;
	// ������
	Get().cTrackerCfgParam.nEnableProcessBWPlate = 1;

	//���Ӿ�����Ƶ�������Ĭ��ֵ
	Get().cTrackerCfgParam.cVideoDet.rcVideoDetArea.left = 0;
	Get().cTrackerCfgParam.cVideoDet.rcVideoDetArea.top = 30;
	Get().cTrackerCfgParam.cVideoDet.rcVideoDetArea.right = 100;
	Get().cTrackerCfgParam.cVideoDet.rcVideoDetArea.bottom = 100;
	//�ȳ�ʼ��ɨ������ÿ��ģʽ�����Լ���ɨ������
	Get().cTrackerCfgParam.cDetectArea.DetectorAreaLeft = 2;
	Get().cTrackerCfgParam.cDetectArea.DetectorAreaTop = 60;
	Get().cTrackerCfgParam.cDetectArea.DetectorAreaRight = 98;
	Get().cTrackerCfgParam.cDetectArea.DetectorAreaBottom = 100;
	Get().cTrackerCfgParam.cDetectArea.nDetectorMinScaleNum = 6;
	Get().cTrackerCfgParam.cDetectArea.nDetectorMaxScaleNum = 10;
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

	// ץ��λ�ó�ʼֵ
	Get().cTrackerCfgParam.nCaptureOnePos = 85;
	Get().cTrackerCfgParam.nCaptureTwoPos = 65;
	Get().cTrackerCfgParam.nCaptureThreePos = 45;

	Get().cTrackerCfgParam.cTrafficLight.nFilterRushPeccancy = 1;

	// �羯Ĭ��ֻ���һ�Ŵ�ͼ
	Get().cResultSenderParam.iBestSnapshotOutput = 1;
	Get().cResultSenderParam.iLastSnapshotOutput = 0;
	//�޳��Ƽ��ģʽ
	Get().cTrackerCfgParam.cVideoDet.iNoPlateDetMode = 1;
	Get().cDevParam[1].iCommType = 1;	//����ֻ��һ������COM1,ֻ֧��485

	Get().cCamAppParam.iResolution = 0;	//Ĭ��Ϊ1080P

	// ������Ƶ�������
	Get().cTrackerCfgParam.iUsedLight = 0;

	//����ɨ������Ĭ�ϴ�
	Get().cTrackerCfgParam.cTrapArea.fEnableDetAreaCtrl = TRUE;

	//����RGB�������÷�ΧΪ36~255,Ĭ��ֵ����Ϊ0
	Get().cCamAppParam.iGainR = 36;
	Get().cCamAppParam.iGainG = 36;
	Get().cCamAppParam.iGainB = 36;

	Get().cCamAppParam.iAGCEnable = 1;
	Get().cCamAppParam.iAGCTh = 100;
	Get().cCamAppParam.iAGCShutterLOri = 100;
	Get().cCamAppParam.iAGCShutterHOri = 5500;
	Get().cCamAppParam.iAGCGainLOri = 0;
	Get().cCamAppParam.iAGCGainHOri = 200;
	Get().cCamAppParam.iAGCScene = 2;

	Get().cCamCfgParam.iDynamicCfgEnable = 1;
	Get().cCamCfgParam.iMaxAGCLimit = 150;
	Get().cCamCfgParam.iMinAGCLimit = 50;

	// Ĭ�ϵĽ��롣
	Get().cCamAppParam.iSNFTNFMode = 3;
	Get().cCamAppParam.iTNFSNFValue = 1;

	// Ĭ�ϵĶԱȶ�
	Get().cCamAppParam.iContrastValue = 19;


	// ɫ��Ĭ��Ϊ1
	Get().cCamAppParam.iColorGradation = 1;

	Get().cTrackerCfgParam.nLightTypeCpl = 13;

	Get().cCamCfgParam.iNightShutterThreshold = 4000;
	Get().cCamCfgParam.iNightAvgYThreshold = 80;

	// ·�����Ʒ���
	strcpy(Get().cResultSenderParam.szStreetName, "NULL");
	strcpy(Get().cResultSenderParam.szStreetDirection, "NULL");
}

HRESULT CSWLPRVenusEPoliceParameter::Initialize(CSWString strFilePath)
{
	// �ָ�Ĭ��
	ResetParam();
    // �Ȼ���ĳ�ʼ��
    if (S_OK == CSWParameter<ModuleParams>::Initialize(strFilePath)
		&& S_OK == InitSystem()
		&& S_OK == InitTracker()
		&& S_OK == InitHvDsp()
        && S_OK == InitOuterCtrl()
        && S_OK == InitIPTCtrl()
        && S_OK == InitCamApp()
        && S_OK == InitScaleSpeed()
        && S_OK == InitCamera()
        && S_OK == InitCharacter() 
        && S_OK == InitGB28181() 
        && S_OK == InitAutoReboot()
		&& S_OK == InitONVIF())
    {
        return S_OK ;
    }

    SW_TRACE_DEBUG("<CSWLPRVenusEPoliceParameter> Initialize failed.\n");
    return E_FAIL ;
}

HRESULT CSWLPRVenusEPoliceParameter::InitSystem(VOID)
{
    if (S_OK == CSWLPRParameter::InitSystem())
    {
        return S_OK ;
    }
    return E_FAIL ;

}

// ���μ�����
void CSWLPRVenusEPoliceParameter::DeleteTrackerParam(VOID)
{
	// �µ羯����Ҫ����ɨ���������Ρ�

    GetEnum("Tracker\\DetAreaCtrl\\Normal"
    	,"EnableTrap"
    	, &Get().cTrackerCfgParam.cTrapArea.fEnableDetAreaCtrl
    	, Get().cTrackerCfgParam.cTrapArea.fEnableDetAreaCtrl
    	, "0:�ر�;1:��"
    	, "����ɨ��������ƿ���"
    	, ""
    	, DELETE_LEVEL
    );

    GetInt("Tracker\\DetAreaCtrl\\Normal"
    	, "TopLeftX"
    	, &Get().cTrackerCfgParam.cTrapArea.TopLeftX
    	, Get().cTrackerCfgParam.cTrapArea.TopLeftX
    	, 0
    	, 100
    	, "ɨ��������������X"
    	, ""
    	, DELETE_LEVEL
    );

    GetInt("Tracker\\DetAreaCtrl\\Normal"
    	, "TopLeftY"
    	, &Get().cTrackerCfgParam.cTrapArea.TopLeftY
    	, Get().cTrackerCfgParam.cTrapArea.TopLeftY
    	, 0
    	, 100
    	, "ɨ��������������Y"
    	, ""
    	, DELETE_LEVEL
    );

    GetInt("Tracker\\DetAreaCtrl\\Normal"
    	, "TopRightX"
    	, &Get().cTrackerCfgParam.cTrapArea.TopRightX
    	, Get().cTrackerCfgParam.cTrapArea.TopRightX
    	, 0
    	, 100
    	, "ɨ��������������X"
    	, ""
    	, DELETE_LEVEL
    );

    GetInt("Tracker\\DetAreaCtrl\\Normal"
    	, "TopRightY"
    	, &Get().cTrackerCfgParam.cTrapArea.TopRightY
    	, Get().cTrackerCfgParam.cTrapArea.TopRightY
    	, 0
    	, 100
    	, "ɨ��������������Y"
    	, ""
    	, DELETE_LEVEL
    );

    GetInt("Tracker\\DetAreaCtrl\\Normal","BottomLeftX"
    	, &Get().cTrackerCfgParam.cTrapArea.BottomLeftX
    	, Get().cTrackerCfgParam.cTrapArea.BottomLeftX
    	, 0
    	, 100
    	, "ɨ��������������X"
    	, ""
    	, DELETE_LEVEL
    );

    GetInt("Tracker\\DetAreaCtrl\\Normal"
    	, "BottomLeftY"
    	, &Get().cTrackerCfgParam.cTrapArea.BottomLeftY
    	, Get().cTrackerCfgParam.cTrapArea.BottomLeftY
    	, 0
    	, 100
    	, "ɨ��������������Y"
    	, ""
    	, DELETE_LEVEL
    );

    GetInt("Tracker\\DetAreaCtrl\\Normal"
    	, "BottomRightX"
    	, &Get().cTrackerCfgParam.cTrapArea.BottomRightX
    	, Get().cTrackerCfgParam.cTrapArea.BottomRightX
    	, 0
    	, 100
    	, "ɨ��������������X"
    	, ""
    	, DELETE_LEVEL
    );

    GetInt("Tracker\\DetAreaCtrl\\Normal"
    	, "BottomRightY"
    	, &Get().cTrackerCfgParam.cTrapArea.BottomRightY
    	, Get().cTrackerCfgParam.cTrapArea.BottomRightY
    	, 0
    	, 100
    	, "ɨ��������������Y"
    	, ""
    	, DELETE_LEVEL
    );

    Get().cTrackerCfgParam.cVideoDet.nVideoDetMode = 2;
    GetEnum("\\Tracker\\VideoDet"
    	, "VideoDetMode"
    	, &Get().cTrackerCfgParam.cVideoDet.nVideoDetMode
    	, Get().cTrackerCfgParam.cVideoDet.nVideoDetMode
    	, "0:��ʹ����Ƶ���;1:ֻʹ�ñ������;2:ʹ����Ƶ���"
    	, "��Ƶ���ģʽ"
    	, ""
    	, DELETE_LEVEL
    );

    GetInt("\\Tracker\\VideoDet"
        	, "DayBackgroudThreshold"
        	, &Get().cTrackerCfgParam.cVideoDet.nDayBackgroudThreshold
        	, Get().cTrackerCfgParam.cVideoDet.nDayBackgroudThreshold
        	, 0
        	, 255
        	, "���챳�������ֵ"
        	, ""
        	, DELETE_LEVEL
        );

        GetInt("\\Tracker\\VideoDet"
        	, "nBackgroudThreshold"
        	, &Get().cTrackerCfgParam.cVideoDet.nNightBackgroudThreshold
        	, Get().cTrackerCfgParam.cVideoDet.nNightBackgroudThreshold
        	, 0
        	, 255
        	, "���ϱ��������ֵ"
        	, ""
        	, DELETE_LEVEL
        );

        GetInt("\\Tracker\\VideoDet"
        	, "VoteFrameCount"
        	, &Get().cTrackerCfgParam.cVideoDet.nVoteFrameCount
        	, Get().cTrackerCfgParam.cVideoDet.nVoteFrameCount
        	, 15
        	, 100
        	, "ͶƱ��Ч֡������֡�����ڴ�ֵ��ͶƱ"
        	, ""
        	, DELETE_LEVEL
        );

        GetEnum("\\Tracker\\VideoDet"
        	, "CheckAcross"
        	, &Get().cTrackerCfgParam.cVideoDet.nCheckAcross
        	, Get().cTrackerCfgParam.cVideoDet.nCheckAcross
        	, "0:��;1:��"
        	, "�Ƿ��������ʻ������"
        	, ""
        	, DELETE_LEVEL
        );

        GetEnum("\\Tracker\\VideoDet"
        	, "ShadowDetMode"
        	, &Get().cTrackerCfgParam.cVideoDet.nShadowDetMode
        	, Get().cTrackerCfgParam.cVideoDet.nShadowDetMode
        	, "0:��ʹ����Ӱ���;1:ʹ����Ӱ���"
        	, "��Ӱ���ģʽ"
        	, ""
        	, DELETE_LEVEL
        );

        GetInt("\\Tracker\\VideoDet"
        	, "ShadowDetEffect"
        	, &Get().cTrackerCfgParam.cVideoDet.nShadowDetEffect
        	, Get().cTrackerCfgParam.cVideoDet.nShadowDetEffect
        	, 0
        	, 100
        	, "��Ӱ���ǿ���̶�ֵ"
        	, "0:ʹ��ȱʡֵ(����ȷ), 1:��Ӱ���ǿ���̶���С, 100:��Ӱ���ǿ���̶����"
        	, DELETE_LEVEL
        );

        GetEnum("\\Tracker\\VideoDet"
        	, "DiffType"
        	, &Get().cTrackerCfgParam.cVideoDet.nDiffType
        	, Get().cTrackerCfgParam.cVideoDet.nDiffType
        	, "0:֡����ȡ;1:֡���sobelǰ��"
        	, "ǰ����ȡ��ʽ"
        	, "ֵΪ0:֡����ȡ ֵΪ1:֡���sobelǰ������Ը���·����������������"
        	, DELETE_LEVEL
        );

        GetEnum("\\Tracker\\VideoDet"
        	, "MedFilter"
        	, &Get().cTrackerCfgParam.cVideoDet.nMedFilter
        	, Get().cTrackerCfgParam.cVideoDet.nMedFilter
        	, "0:�ر�;1:��"
        	, "���ƶ�������"
        	, ""
        	, DELETE_LEVEL
        );

        GetInt("\\Tracker\\VideoDet"
        	, "MedFilterUpLine"
        	, &Get().cTrackerCfgParam.cVideoDet.nMedFilterUpLine
        	, Get().cTrackerCfgParam.cVideoDet.nMedFilterUpLine
        	, 0
        	, 100
        	, "���ƶ�����������������"
        	, "ֵΪ0:��Ļ���Ϸ� ֵΪ100:��Ļ���·�"
        	, DELETE_LEVEL
        );

        GetInt("\\Tracker\\VideoDet"
        	, "MedFilterDownLine"
        	, &Get().cTrackerCfgParam.cVideoDet.nMedFilterDownLine
        	, Get().cTrackerCfgParam.cVideoDet.nMedFilterDownLine
        	, 0
        	, 100
        	, "���ƶ�����������������"
        	, "ֵΪ0:��Ļ���Ϸ� ֵΪ100:��Ļ���·�"
        	, DELETE_LEVEL
        );

        GetInt("Tracker\\Misc"
			, "CarArrivedPos"
			, &Get().cTrackerCfgParam.nCarArrivedPos
			, Get().cTrackerCfgParam.nCarArrivedPos
			, 0
			, 100
			, "��������λ��(��������������)"
			, ""
			, DELETE_LEVEL
		);

		GetInt("Tracker\\Misc"
			, "CarArrivedPosNoPlate"
			, &Get().cTrackerCfgParam.nCarArrivedPosNoPlate
			, Get().cTrackerCfgParam.nCarArrivedPosNoPlate
			, 0
			, 100
			, "���Ƴ�����λ��"
			, ""
			, DELETE_LEVEL
		);

		GetInt("Tracker\\Misc"
			, "CarArrivedPosYellow"
			, &Get().cTrackerCfgParam.nCarArrivedPosYellow
			, Get().cTrackerCfgParam.nCarArrivedPosYellow
			, 0
			, 100
			, "��������λ��(����)"
			, ""
			, DELETE_LEVEL
		);

	    GetInt("Tracker\\TrackInfo"
	    	, "VoteFrameNum"
	    	, &Get().cTrackerCfgParam.nVoteFrameNum
	    	, Get().cTrackerCfgParam.nVoteFrameNum
	    	, 4
	    	, 1000
	    	, "ͶƱ�Ľ����"
	    	, ""
	    	, DELETE_LEVEL
	    );

	    GetInt("Tracker\\TrackInfo"
	    	, "MaxEqualFrameNumForVote"
	    	, &Get().cTrackerCfgParam.nMaxEqualFrameNumForVote
	    	, Get().cTrackerCfgParam.nMaxEqualFrameNumForVote
	    	, 4, 1000
	    	, "������ͬ�����������"
	    	, ""
	    	, DELETE_LEVEL
	    );

	    GetInt("\\Tracker\\ProcessPlate\\BlackPlate"
	    	, "NightThreshold"
	    	,	&Get().cTrackerCfgParam.nNightThreshold
	    	, Get().cTrackerCfgParam.nNightThreshold
	    	, 0
	    	, 240
	    	, "ҹ��ģʽ��������"
	    	, ""
	    	, DELETE_LEVEL
	    );


	    GetInt("\\Tracker\\VideoDet\\AreaCtrl"
	    	, "Left"
	    	, &Get().cTrackerCfgParam.cVideoDet.rcVideoDetArea.left
	    	, Get().cTrackerCfgParam.cVideoDet.rcVideoDetArea.left
	    	, 0
	    	, 100
	    	, "��Ƶ���������"
	    	, ""
	    	, DELETE_LEVEL
	    );

	    GetInt("\\Tracker\\VideoDet\\AreaCtrl"
	    	, "Right"
	    	, &Get().cTrackerCfgParam.cVideoDet.rcVideoDetArea.right
	    	, Get().cTrackerCfgParam.cVideoDet.rcVideoDetArea.right
	    	, 0
	    	, 100
	    	, "��Ƶ���������"
	    	, ""
	    	, DELETE_LEVEL
	    );

	    GetInt("\\Tracker\\VideoDet\\AreaCtrl"
	    	, "Top"
	    	, &Get().cTrackerCfgParam.cVideoDet.rcVideoDetArea.top
	    	, Get().cTrackerCfgParam.cVideoDet.rcVideoDetArea.top
	    	, 0
	    	, 100
	    	, "��Ƶ���������"
	    	, ""
	    	, DELETE_LEVEL
	    );

	    GetInt("\\Tracker\\VideoDet\\AreaCtrl"
	    	, "Bottom"
	    	, &Get().cTrackerCfgParam.cVideoDet.rcVideoDetArea.bottom
	    	, Get().cTrackerCfgParam.cVideoDet.rcVideoDetArea.bottom
	    	, 0
	    	, 100
	    	, "��Ƶ���������"
	    	, ""
	    	, DELETE_LEVEL
	    );

}

HRESULT CSWLPRVenusEPoliceParameter::InitTracker(VOID)
{
	if(S_OK == CSWLPRParameter::InitTracker())
	{

		// �µ羯����Ҫ����ɨ���������Ρ�
		DeleteTrackerParam();
		Get().cTrackerCfgParam.cTrapArea.fEnableDetAreaCtrl = 1;




		GetEnum("Tracker\\Misc"
	    	, "CoreProcessType"
	    	, &Get().cTrackerCfgParam.nProcessType
	    	, Get().cTrackerCfgParam.nProcessType
	    	, "0:ֻ���ִ���С�ͳ�;1:��������ַǻ���������;2:��������ַǻ�����������"
	    	, "��������"
	    	, ""
	    	, PROJECT_LEVEL
	    );

	    GetEnum("Tracker\\Misc"
	    	, "CoreOutPutType"
	    	, &Get().cTrackerCfgParam.nOutPutType
	    	, Get().cTrackerCfgParam.nOutPutType
	    	, "0:ȫ�������;1:����С�ͳ����Ϊ������"
	    	, "������� 0:ȫ�������;1:����С�ͳ����Ϊ������"
	    	, ""
	    	, PROJECT_LEVEL
	    );

		GetEnum("\\Tracker\\DoublePlate"
			, "DoublePlateTypeEnable"
			, &Get().cTrackerCfgParam.nDoublePlateTypeEnable
			, Get().cTrackerCfgParam.nDoublePlateTypeEnable
			, "0:���Ƴ���;1:Ħ�г���;3:�Զ����"
		    , "˫�������Ϳ���"
		    , ""
		    , CUSTOM_LEVEL
	    );

		GetEnum("Tracker\\Misc"
	        , "EnableAlphaRecog"
	        , &Get().cTrackerCfgParam.fEnableAlpha_5
	        , 1 //default value
	        , "0:�ر�;1:��"
	        , "������ĸʶ�𿪹�"
	        , ""
	        , PROJECT_LEVEL
	        );

		GetEnum("\\Tracker\\ProcessPlate"
	        , "EnableDBGreenSegment"
	        , &Get().cTrackerCfgParam.nPlateDetect_Green
	        , Get().cTrackerCfgParam.nPlateDetect_Green
	        , "0:�ر�;1:��"
	        , "����ʶ�𿪹�"
	        , ""
	        , PROJECT_LEVEL
	        );


	    GetEnum("\\Tracker\\DoublePlate"
	    	, "DefaultType"
	    	, &Get().cTrackerCfgParam.nDefaultDBType
	    	, Get().cTrackerCfgParam.nDefaultDBType
	    	, "0:Ħ�г���;1:���Ƴ���"
	      , "Ĭ��˫��������"
	      , ""
	      , CUSTOM_LEVEL
	    );

		GetEnum("\\Tracker\\RoadInfo"
			, "OutputRoadTypeNameFlag"
			, &Get().cTrackerCfgParam.nOutputRoadTypeName
			, Get().cTrackerCfgParam.nOutputRoadTypeName
			, "0:�����;1:���"
			, "����������ͱ�ʶ"
			, ""
			, CUSTOM_LEVEL
			);
		for (int i = 0; i < MAX_ROADLINE_NUM; i++)
	    {
	        char szSection[256];
	        sprintf(szSection, "\\Tracker\\RoadInfo\\Road%02d", i);

	        GetString(szSection
	        	, "RoadTypeName"
	        	, Get().cTrackerCfgParam.rgcRoadInfo[i].szRoadTypeName
	        	, Get().cTrackerCfgParam.rgcRoadInfo[i].szRoadTypeName
	        	, sizeof(Get().cTrackerCfgParam.rgcRoadInfo[i].szRoadTypeName)
	        	, "������������"
	        	, "��ΪNULL����ȡ�ó������͵�������"
	        	, CUSTOM_LEVEL
	        );

	        // todo.
	        //����֡���Ĳ��졣
	        Get().cTrackerCfgParam.rgcRoadInfo[i].ptTop.y /= 2;
	        Get().cTrackerCfgParam.rgcRoadInfo[i].ptBottom.y /= 2;
	    }




	    GetInt("\\Tracker\\DoublePlate"
	    	, "MiddleWidth"
	    	, &Get().cTrackerCfgParam.nMiddleWidth
	    	, Get().cTrackerCfgParam.nMiddleWidth
	    	, 0
	    	, 300
	    	, "��Ļ�м䵥�����ƺ����ֿ��"
	    	, ""
	    	, CUSTOM_LEVEL
	    );

	    GetInt("\\Tracker\\DoublePlate"
	    	, "BottomWidth"
	    	, &Get().cTrackerCfgParam.nBottomWidth
	    	, Get().cTrackerCfgParam.nBottomWidth
	    	, 0
	    	, 300
	    	, "��Ļ�ײ��������ƺ����ֿ��"
	    	, ""
	    	, CUSTOM_LEVEL
	    );

	    GetInt("\\Tracker\\TrafficLight"
	    	, "OnePos"
	    	, &Get().cTrackerCfgParam.nCaptureOnePos
	    	, Get().cTrackerCfgParam.nCaptureOnePos
	    	, 0
	    	, 100
	    	, "ץ�ĵ�һ��ͼ��λ��"
	    	, ""
	    	, CUSTOM_LEVEL
	    );

	    GetInt("\\Tracker\\TrafficLight"
	    	, "TwoPos"
	    	, &Get().cTrackerCfgParam.nCaptureTwoPos
	    	, Get().cTrackerCfgParam.nCaptureTwoPos
	    	, 0
	    	, 100
	    	, "ץ�ĵڶ���ͼ��λ��"
	    	, ""
	    	, CUSTOM_LEVEL
	    );

	    GetInt("\\Tracker\\TrafficLight"
	    	, "ThreePos"
	    	, &Get().cTrackerCfgParam.nCaptureThreePos
	    	, Get().cTrackerCfgParam.nCaptureThreePos
	    	, 0
	    	, 100
	    	, "ץ�ĵ�����ͼ��λ��"
	    	, ""
	    	, CUSTOM_LEVEL
	    );

	    GetInt("\\Tracker\\ProcessPlate\\BlackPlate"
	    	, "S"
	    	, &Get().cTrackerCfgParam.nProcessPlate_BlackPlate_S
	    	, Get().cTrackerCfgParam.nProcessPlate_BlackPlate_S
	    	, 0
	    	, 240
	    	, "���Ʊ��Ͷ�����"
	    	, ""
	    	, PROJECT_LEVEL
	    );

	    for (int i = 0; i < MAX_ROADLINE_NUM; i++)
	    {
	        char szSection[256];
	        sprintf(szSection, "\\Tracker\\RoadInfo\\Road%02d", i);

			// todo. ���ϵ����ʹ��������⣬����INT�����档
			GetInt(szSection
				, "FilterRunType"
				, &Get().cTrackerCfgParam.rgcRoadInfo[i].iFilterRunType
				, Get().cTrackerCfgParam.rgcRoadInfo[i].iFilterRunType
				, 0
				, 100
				, "������ʻ����"
				, "1:ֱ��;2:��ת;4:��ת;8:��ͷ;16:����"
				, CUSTOM_LEVEL
				);
			/*
	    		GetSet(szSection
	    		, "FilterRunType"
	    		, &Get().cTrackerCfgParam.rgcRoadInfo[i].iFilterRunType
	    		, Get().cTrackerCfgParam.rgcRoadInfo[i].iFilterRunType
	    		, "1:ֱ��;2:��ת;4:��ת;8:��ͷ;16:����"
	    		, 255
	    		, "������ʻ����"
	    		, ""
	    		, CUSTOM_LEVEL
	        );
			*/
	    }

		//��Ƶ�����ҪУ������
		Get().cTrackerCfgParam.cVideoDet.iEPOutPutDetectLine = 100 - Get().cTrackerCfgParam.cVideoDet.iEPOutPutDetectLine;
		Get().cTrackerCfgParam.cVideoDet.iEPFGDetectLine -= Get().cTrackerCfgParam.cVideoDet.rcVideoDetArea.top;
		if(Get().cTrackerCfgParam.cVideoDet.iEPFGDetectLine < 0)
		{
			Get().cTrackerCfgParam.cVideoDet.iEPFGDetectLine = 0;
		}
		int width = Get().cTrackerCfgParam.cVideoDet.rcVideoDetArea.bottom - Get().cTrackerCfgParam.cVideoDet.rcVideoDetArea.top;
		if(width <= 0)
		{
			width = 100;
		}
		Get().cTrackerCfgParam.cVideoDet.iEPFGDetectLine *= 100.0f / width;

	    LoadTrafficLightParam();

	    // ������Ƶ�������
		// ���ݳ����ߺ͵�����ץ�������Ĭ��ֵ��
	    Get().cTrackerCfgParam.cTrapArea.TopLeftX = 27;
	    Get().cTrackerCfgParam.cTrapArea.TopLeftY = 24;
	    Get().cTrackerCfgParam.cTrapArea.TopRightX = 67;
	    Get().cTrackerCfgParam.cTrapArea.TopRightY = 24;
	    Get().cTrackerCfgParam.cTrapArea.BottomLeftX = 7;
	    Get().cTrackerCfgParam.cTrapArea.BottomLeftY = 100;
	    Get().cTrackerCfgParam.cTrapArea.BottomRightX = 90;
	    Get().cTrackerCfgParam.cTrapArea.BottomRightY = 100;

		GetInt("\\Tracker\\VideoDet\\AreaCtrl"
			, "TopLeftX"
			, &Get().cTrackerCfgParam.cTrapArea.TopLeftX
			, Get().cTrackerCfgParam.cTrapArea.TopLeftX
			, 0
			, 100
			, "������������X"
			, ""
			, CUSTOM_LEVEL
		);

		GetInt("\\Tracker\\VideoDet\\AreaCtrl"
			, "TopLeftY"
			, &Get().cTrackerCfgParam.cTrapArea.TopLeftY
			, Get().cTrackerCfgParam.cTrapArea.TopLeftY
			, 0
			, 100
			, "������������Y"
			, ""
			, CUSTOM_LEVEL
		);

		GetInt("\\Tracker\\VideoDet\\AreaCtrl"
			, "TopRightX"
			, &Get().cTrackerCfgParam.cTrapArea.TopRightX
			, Get().cTrackerCfgParam.cTrapArea.TopRightX
			, 0
			, 100
			, "������������X"
			, ""
			, CUSTOM_LEVEL
		);

		GetInt("\\Tracker\\VideoDet\\AreaCtrl"
			, "TopRightY"
			, &Get().cTrackerCfgParam.cTrapArea.TopRightY
			, Get().cTrackerCfgParam.cTrapArea.TopRightY
			, 0
			, 100
			, "������������Y"
			, ""
			, CUSTOM_LEVEL
		);

		GetInt("\\Tracker\\VideoDet\\AreaCtrl","BottomLeftX"
			, &Get().cTrackerCfgParam.cTrapArea.BottomLeftX
			, Get().cTrackerCfgParam.cTrapArea.BottomLeftX
			, 0
			, 100
			, "������������X"
			, ""
			, CUSTOM_LEVEL
		);

		GetInt("\\Tracker\\VideoDet\\AreaCtrl"
			, "BottomLeftY"
			, &Get().cTrackerCfgParam.cTrapArea.BottomLeftY
			, Get().cTrackerCfgParam.cTrapArea.BottomLeftY
			, 0
			, 100
			, "������������Y"
			, ""
			, CUSTOM_LEVEL
		);

		GetInt("\\Tracker\\VideoDet\\AreaCtrl"
			, "BottomRightX"
			, &Get().cTrackerCfgParam.cTrapArea.BottomRightX
			, Get().cTrackerCfgParam.cTrapArea.BottomRightX
			, 0
			, 100
			, "������������X"
			, ""
			, CUSTOM_LEVEL
		);

		GetInt("\\Tracker\\VideoDet\\AreaCtrl"
			, "BottomRightY"
			, &Get().cTrackerCfgParam.cTrapArea.BottomRightY
			, Get().cTrackerCfgParam.cTrapArea.BottomRightY
			, 0
			, 100
			, "������������Y"
			, ""
			, CUSTOM_LEVEL
		);

		// �����������������������
		Get().cTrackerCfgParam.cVideoDet.rcVideoDetArea.left =
			    Get().cTrackerCfgParam.cTrapArea.TopLeftX > Get().cTrackerCfgParam.cTrapArea.BottomLeftX ?
			    		Get().cTrackerCfgParam.cTrapArea.BottomLeftX : Get().cTrackerCfgParam.cTrapArea.TopLeftX;
		Get().cTrackerCfgParam.cVideoDet.rcVideoDetArea.right =
				Get().cTrackerCfgParam.cTrapArea.TopRightX < Get().cTrackerCfgParam.cTrapArea.BottomRightX ?
						Get().cTrackerCfgParam.cTrapArea.BottomRightX: Get().cTrackerCfgParam.cTrapArea.TopRightX;
		Get().cTrackerCfgParam.cVideoDet.rcVideoDetArea.top =
				Get().cTrackerCfgParam.cTrapArea.TopLeftY > Get().cTrackerCfgParam.cTrapArea.TopRightY ?
						Get().cTrackerCfgParam.cTrapArea.TopRightY   : Get().cTrackerCfgParam.cTrapArea.TopLeftY;
		Get().cTrackerCfgParam.cVideoDet.rcVideoDetArea.bottom =
				Get().cTrackerCfgParam.cTrapArea.BottomLeftY > Get().cTrackerCfgParam.cTrapArea.BottomRightY ?
						Get().cTrackerCfgParam.cTrapArea.BottomLeftY : Get().cTrackerCfgParam.cTrapArea.BottomRightY;


		return S_OK;
	}
	return E_FAIL;
}

VOID CSWLPRVenusEPoliceParameter::LoadTrafficLightParam(VOID)
{
    GetEnum("\\Tracker\\TrafficLight"
    	, "CheckType"
    	, &Get().cTrackerCfgParam.cTrafficLight.nCheckType
    	, Get().cTrackerCfgParam.cTrafficLight.nCheckType
    	, "0:��Ƶ���;1:��Ӻ��̵�"
    	, "���̵Ƽ�ⷽʽ"
    	, ""
    	, CUSTOM_LEVEL
    );

    GetEnum("\\Tracker\\TrafficLight"
    	, "AutoScanLight"
    	, &Get().cTrackerCfgParam.cTrafficLight.nAutoScanLight
    	, Get().cTrackerCfgParam.cTrafficLight.nAutoScanLight
    	, "0:�ر�;1:��"
    	, "��̬�����̵�λ�ÿ���"
    	, ""
    	, CUSTOM_LEVEL
    );

    GetEnum("Tracker\\TrafficLight"
    	, "DeleteLightEdge"
    	, &Get().cTrackerCfgParam.cTrafficLight.fDeleteLightEdge
    	, Get().cTrackerCfgParam.cTrafficLight.fDeleteLightEdge
    	, "0:�ر�;1:��"
    	, "ȥ���ƿ��Ե����"
    	, ""
    	, PROJECT_LEVEL
    );

    GetEnum("\\Tracker\\TrafficLight"
    	, "SceneCheckMode"
    	, &Get().cTrackerCfgParam.cTrafficLight.nSenceCheckMode
    	, Get().cTrackerCfgParam.cTrafficLight.nSenceCheckMode
    	, "0:�Զ����;1:�ֶ���"
    	, "�������ģʽ"
    	, ""
    	, CUSTOM_LEVEL
    );


    GetInt("\\Tracker\\TrafficLight"
    	, "StopLinePos"
    	, &Get().cTrackerCfgParam.cTrafficLight.nStopLinePos
    	, Get().cTrackerCfgParam.cTrafficLight.nStopLinePos
    	, 0
    	, 100
    	, "ֹͣ��λ��(ֱ��)"
    	, ""
    	, CUSTOM_LEVEL
    );

    GetInt("\\Tracker\\TrafficLight"
    	, "LeftStopLinePos"
    	, &Get().cTrackerCfgParam.cTrafficLight.nLeftStopLinePos
    	, Get().cTrackerCfgParam.cTrafficLight.nLeftStopLinePos
    	, 0
    	, 100
    	, "ֹͣ��λ��(��ת)"
    	, ""
    	, CUSTOM_LEVEL
    );

    GetInt("\\Tracker\\TrafficLight"
    	, "LightCount"
    	, &Get().cTrackerCfgParam.cTrafficLight.nLightCount
    	, Get().cTrackerCfgParam.cTrafficLight.nLightCount
    	, 0
    	, MAX_TRAFFICLIGHT_COUNT
    	, "��������"
    	, ""
    	, CUSTOM_LEVEL
    );

    GetInt("\\Tracker\\TrafficLight"
    	, "ValidSceneStatus"
    	, &Get().cTrackerCfgParam.cTrafficLight.nValidSceneStatusCount
    	, Get().cTrackerCfgParam.cTrafficLight.nValidSceneStatusCount
    	, 1
    	, 100
    	, "��Ч����״̬����"
    	, "��ͬ״̬Ҫ�ﵽ��ֵ�����"
    	, CUSTOM_LEVEL
    );

    // todo.
    // �ֻ��Ʋ���ֹͣ�߲��������
    GetEnum("\\Tracker\\TrafficLight"
    	, "OutputInLine"
    	, &Get().cTrackerCfgParam.cTrafficLight.nOutputInLine
    	, Get().cTrackerCfgParam.cTrafficLight.nOutputInLine
    	, "0:�����;1:���"
    	, "���δ��ֹͣ�ߵĳ���"
    	, ""
    	, CUSTOM_LEVEL
    );

    GetInt("\\Tracker\\TrafficLight"
    	, "RedLightDelay"
    	, &Get().cTrackerCfgParam.cTrafficLight.nRedLightDelay
    	, Get().cTrackerCfgParam.cTrafficLight.nRedLightDelay
    	, 0
    	, 300
    	, "��ɫ�ƻƵƱ��Ƽ��(��)"
    	, ""
    	, CUSTOM_LEVEL
    );


    GetEnum("\\Tracker\\TrafficLight"
    	, "RunTypeEnable"
    	, &Get().cTrackerCfgParam.cTrafficLight.nRunTypeEnable
    	, Get().cTrackerCfgParam.cTrafficLight.nRunTypeEnable
    	, "0:��;1:��"
    	, "��ʻ����ǿ�����"
    	, ""
    	, CUSTOM_LEVEL
    );

	GetEnum("\\Tracker\\TrafficLight"
		, "FilterRushPeccancy"
		, &Get().cTrackerCfgParam.cTrafficLight.nFilterRushPeccancy
		, Get().cTrackerCfgParam.cTrafficLight.nFilterRushPeccancy
		, "0:������;1:����"
		, "���˵�������ͼΪ�̵�ʱ��Υ��"
		, ""
		, PROJECT_LEVEL
		);

    for (int i = 0; i < MAX_TRAFFICLIGHT_COUNT; i++)
    {
        char szLightPosKey[32];
        char szLightPosInfo[32];
        sprintf(szLightPosKey, "LightPos_%d", i + 1);
        sprintf(szLightPosInfo, "������Ϣ_%d", i + 1);

        GetString("\\Tracker\\TrafficLight"
        	, szLightPosKey
        	, Get().cTrackerCfgParam.cTrafficLight.rgszLightPos[i]
        	, Get().cTrackerCfgParam.cTrafficLight.rgszLightPos[i]
        	, (64-1)
        	, szLightPosInfo
        	, ""
        	, CUSTOM_LEVEL
        );
    }

    GetInt("\\Tracker\\TrafficLight\\Scene"
    	, "SceneCount"
    	, &Get().cTrackerCfgParam.cTrafficLight.nSceneCount
    	, Get().cTrackerCfgParam.cTrafficLight.nSceneCount
    	, 0
    	, MAX_SCENE_COUNT
    	, "���̵Ƴ�����"
    	, ""
    	, CUSTOM_LEVEL
    );

    for (int i = 0; i < MAX_SCENE_COUNT; i++)
    {
        char szSceneKey[32];
        char szSceneInfo[32];
        sprintf(szSceneKey, "Scene_%d", i + 1);
        sprintf(szSceneInfo, "������Ϣ_%d", i + 1);

        GetString("\\Tracker\\TrafficLight\\Scene"
        	, szSceneKey
        	, Get().cTrackerCfgParam.cTrafficLight.rgszScene[i]
        	, Get().cTrackerCfgParam.cTrafficLight.rgszScene[i]
        	, (32-1)
        	, szSceneInfo
        	, ""
        	, CUSTOM_LEVEL
        );
    }

    GetString("Tracker\\TrafficLight"
    	, "IOConfig"
    	, Get().cTrackerCfgParam.cTrafficLight.szIOConfig
    	, Get().cTrackerCfgParam.cTrafficLight.szIOConfig
    	, sizeof(Get().cTrackerCfgParam.cTrafficLight.szIOConfig)
    	, "���IO���̵Ʋ���"
    	, ""
    	, CUSTOM_LEVEL
    );

    GetEnum("Tracker\\TrafficLight"
    	, "EnhanceRedLight"
    	, &Get().cTrackerCfgParam.cTrafficLight.fEnhanceRedLight
    	, Get().cTrackerCfgParam.cTrafficLight.fEnhanceRedLight
    	, "0:���Ӻ�;1:�Ӻ�"
    	, "��Ʒ��������Ƿ�Ӻ�"
    	, ""
    	, CUSTOM_LEVEL
    );

    GetInt("Tracker\\TrafficLight"
    	, "TrafficLightThread"
    	, &Get().cTrackerCfgParam.cTrafficLight.nTrafficLightThread
    	, Get().cTrackerCfgParam.cTrafficLight.nTrafficLightThread
    	, 0
    	, 5
    	, "���̵�������"
    	, ""
    	, CUSTOM_LEVEL
    );

    GetInt("Tracker\\TrafficLight"
    	, "TrafficLightBrightValue"
    	, &Get().cTrackerCfgParam.cTrafficLight.nTrafficLightBrightValue
    	, Get().cTrackerCfgParam.cTrafficLight.nTrafficLightBrightValue
    	, 0
    	, 5
    	, "���̵����ȵȼ�"
    	, ""
    	, CUSTOM_LEVEL
    );

    GetInt("Tracker\\TrafficLight"
    	, "AutoScanThread"
    	, &Get().cTrackerCfgParam.cTrafficLight.nAutoScanThread
    	, Get().cTrackerCfgParam.cTrafficLight.nAutoScanThread
    	, 0
    	, 10
    	, "���̵ƶ�̬�����ֵ"
    	, ""
    	, CUSTOM_LEVEL
    );

    GetInt("Tracker\\TrafficLight"
    	, "SkipFrameNO"
    	, &Get().cTrackerCfgParam.cTrafficLight.nSkipFrameNO
    	, Get().cTrackerCfgParam.cTrafficLight.nSkipFrameNO
    	, 0
    	, 50
    	, "��̬����ҵƼ��֡"
    	, ""
    	, CUSTOM_LEVEL
    );

    GetEnum("Tracker\\TrafficLight"
    	, "LightAdhesion"
    	, &Get().cTrackerCfgParam.cTrafficLight.fIsLightAdhesion
    	, Get().cTrackerCfgParam.cTrafficLight.fIsLightAdhesion
    	, "0:��;1:��"
    	, "���̵��Ƿ�ճ����һ��"
    	, ""
    	, CUSTOM_LEVEL
    );

    // �õ�����ץ������Ϊֱ���ж��ߡ�
    /*
    GetInt("Tracker\\TrafficLight"
    	, "RealForward"
    	, &Get().cTrackerCfgParam.cTrafficLight.iRealForward
    	, Get().cTrackerCfgParam.cTrafficLight.iRealForward
    	, 0
    	, 100
    	, "ֱ���ж�λ��"
    	, ""
    	, CUSTOM_LEVEL
    );
    */
    
    GetEnum("\\Tracker\\TrafficLight"
    	, "FilterNoPlatePeccancy"
    	, &Get().cTrackerCfgParam.cTrafficLight.fFilterNoPlatePeccancy
    	, Get().cTrackerCfgParam.cTrafficLight.fFilterNoPlatePeccancy
    	, "0:������;1:����"
    	, "�����޳���Υ�½��"
    	, ""
    	, CUSTOM_LEVEL
    );
}

HRESULT CSWLPRVenusEPoliceParameter::InitHvDsp(VOID)
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
		, "SaveVideoType"
		, &Get().cResultSenderParam.iSaveVideoType
		, Get().cResultSenderParam.iSaveVideoType
		, "0:һֱ¼��;1:ֻ��Υ�½��ǰ��15��¼����ֻ����Υ�½����"
		, "Ӳ��¼������"
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

	// �羯��Υ��ֻ���һ�Ŵ�ͼ�����ṩ���䡣
	Get().cResultSenderParam.iBestSnapshotOutput = 1;
	Get().cResultSenderParam.iLastSnapshotOutput = 0;


	GetEnum("\\HvDsp\\Misc"
		, "FilterUnSurePeccancy"
		, &Get().cResultSenderParam.iFilterUnSurePeccancy
		, Get().cResultSenderParam.iFilterUnSurePeccancy
		, "0:������;1:����"
		, "���˲��ϸ��Υ�¼�¼"
		, ""
		, PROJECT_LEVEL
		);


	GetEnum("\\HvDsp\\VideoProc\\MainVideo"
		, "OutputCaptureImage"
		, &Get().cResultSenderParam.iOutputCaptureImage
		, Get().cResultSenderParam.iOutputCaptureImage
		, "0:ֻ��Υ�³���������Ź켣ͼ;1:���г�����������Ź켣ͼ"
		, "��Υ�³����������ץ��ͼ"
		, ""
		, CUSTOM_LEVEL
		);



	GetEnum("\\Tracker\\TrafficLight"
		, "OutputPeccancyType"
		, &Get().cResultSenderParam.nOutputPeccancyType
		, Get().cResultSenderParam.nOutputPeccancyType
		, "0:�������ȼ����;1:�������Υ������"
		, "���Υ�����ͷ�ʽ"
		, ""
		, CUSTOM_LEVEL
	);

	GetString("\\Tracker\\TrafficLight"
		, "OutputPeccancyPriority"
		, Get().cResultSenderParam.szPeccancyPriority
		, Get().cResultSenderParam.szPeccancyPriority
		, sizeof(Get().cResultSenderParam.szPeccancyPriority)
		, "Υ���������ȼ�"
		, "Υ�����ȼ�����"
		, CUSTOM_LEVEL
	);

	GetInt("\\HvDsp\\Misc"
		, "RedPosBrightness"
		, &Get().cResultSenderParam.nRedPosBrightness
		, Get().cResultSenderParam.nRedPosBrightness
		, 0
		, 255
		, "���ͺ��λ�û���������ֵ"
		, ""
		, PROJECT_LEVEL
	);

	GetEnum("\\HvDsp\\Trigger"
		, "TriggerOutNormalStatus"
		, &Get().cResultSenderParam.nTriggerOutNormalStatus
		, Get().cResultSenderParam.nTriggerOutNormalStatus
		, "0:�ߵ�ƽ;1:�͵�ƽ"
		, "���������̬����"
		, ""
		, PROJECT_LEVEL
	);

	 int iTemp = 1;
	 GetEnum("\\HvDsp\\EventChecker"
			, "OverLineSensitivity"
			, &iTemp
			, iTemp
			, "0:��;1:��;2:��"
			, "ѹ�߼��������"
			, ""
			, CUSTOM_LEVEL
		);
	 if( iTemp == 0 )
	 {
		 Get().cTrackerCfgParam.fltOverLineSensitivity = 2.5;
	 }
	 else if(  iTemp == 1 )
	 {
		 Get().cTrackerCfgParam.fltOverLineSensitivity = 1.75;
	 }
	 else if( iTemp == 2 )
	 {
		 Get().cTrackerCfgParam.fltOverLineSensitivity = 1.0;
	 }

	return S_OK;

}

HRESULT CSWLPRVenusEPoliceParameter::InitScaleSpeed(VOID)
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
			, "1:�ռ�궨��"
			, "����ģʽ"
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

HRESULT CSWLPRVenusEPoliceParameter::InitCamApp(VOID)
{
	 GetString("\\HvDsp\\Camera\\Ctrl"
		 	, "Addr"
		 	, Get().cCamCfgParam.szIP
		 	, Get().cCamCfgParam.szIP
		 	, (100-1)
		 	, "ʶ�����IP"
		 	, ""
		 	, PROJECT_LEVEL
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

	
	  
	  GetEnum("\\HvDsp\\Camera\\Ctrl"
			, "DynamicCfgEnable"
			, &Get().cCamCfgParam.iDynamicCfgEnable
			, Get().cCamCfgParam.iDynamicCfgEnable
			, "0:�ر�;1:��"
			, "��̬���ò���"
			, ""
			, CUSTOM_LEVEL
	    );
	    

	  GetInt("\\HvDsp\\Camera\\Ctrl"
	    	, "MaxAGCLimit"
	    	, &Get().cCamCfgParam.iMaxAGCLimit
	    	, Get().cCamCfgParam.iMaxAGCLimit
	    	, 0
	    	, 255
	    	, "���AE����ֵ"
	    	, ""
	    	, CUSTOM_LEVEL
	      );

	  Get().cCamCfgParam.iMinAGCLimit = 50; //�羯��С����Ĭ��ֵ�޸�Ϊ50
	  GetInt("\\HvDsp\\Camera\\Ctrl"
	      	, "MinAGCLimit"
	      	, &Get().cCamCfgParam.iMinAGCLimit
	      	, Get().cCamCfgParam.iMinAGCLimit
	      	, 0
	      	, 255
	      	, "��СAE����ֵ"
	      	, ""
	      	, CUSTOM_LEVEL
	      );

	  Get().cCamCfgParam.iEnableAGC = 1;

	    int iAGCDB = (Get().cCamCfgParam.iMaxAGCLimit - Get().cCamCfgParam.iMinAGCLimit) / 13;
	    if (iAGCDB > 0)
	    {
	        for (int i = 0; i < 14; i++)
	        {
	        	Get().cCamCfgParam.irgAGCLimit[i] = Get().cCamCfgParam.iMinAGCLimit + (iAGCDB * i);
	        }
	    }
	    GetEnum("\\HvDsp\\Camera\\Ctrl"
	    		, "CtrlCpl"
	    		, &Get().cTrackerCfgParam.nCtrlCpl
	    		, Get().cTrackerCfgParam.nCtrlCpl
	    		, "0:��ʹ��;1:ʹ��"
	    		, "�Զ�����ƫ�⾵"
	    		, "����ʹ�ܣ����ϲ�ʹ��"
	    		, CUSTOM_LEVEL
	    );

	  return S_OK;
}

HRESULT CSWLPRVenusEPoliceParameter::InitIPTCtrl(VOID)
{
	CSWString strCOMM;
	INT i = 1;		//����ֻ��һ�����贮�ڣ�ΪCOM1
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
		, "0:��;3:���͵º��̵�ת����"
		, "�ⲿ�豸����"
		, ""
		, CUSTOM_LEVEL
	);

		
  return S_OK;
}

HRESULT CSWLPRVenusEPoliceParameter::InitOuterCtrl(VOID)
{
	return S_OK;
}

HRESULT CSWLPRVenusEPoliceParameter::InitCamera(VOID)
{

    GetInt("\\CamApp"
    	, "JpegCompressRate"
    	, &Get().cCamAppParam.iJpegCompressRate
    	, Get().cCamAppParam.iJpegCompressRate
    	, 1
    	, 99
    	, "��Ƶ��Jpegѹ����"
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
	
	GetEnum("\\CamApp"
    	, "Resolution"
    	, &Get().cCamAppParam.iResolution
    	, Get().cCamAppParam.iResolution
    	//, "1:720P;2:1080P"
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
    	, "��λ��Kbps"
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
    	, 0
    	, 480					//����185ǰ�����淶ΧΪ0~480
    	, "AGC�����������"
    	, ""
    	, PROJECT_LEVEL
    );

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

	GetEnum("\\CamApp"
    	, "CaptureEdge"
    	, &Get().cCamAppParam.iCaptureEdge
    	, Get().cCamAppParam.iCaptureEdge
    	, "0:������;1:�ⲿ�½��ش���;2:�ⲿ�����ش���;3:�������½��ض�����"
    	, "����ץ����"
    	, ""
    	, PROJECT_LEVEL
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
    
    GetEnum("\\CamApp"
    	, "EnableDCAperture"
    	, &Get().cCamAppParam.iEnableDCAperture
    	, Get().cCamAppParam.iEnableDCAperture
    	, "0:��ʹ��;1:ʹ��"
    	, "ʹ��DC��Ȧ"
    	, ""
    	, PROJECT_LEVEL
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
    GetEnum("\\CamApp"
            , "NoiseReduction"
            , &Get().cCamAppParam.iSNFTNFMode
            , Get().cCamAppParam.iSNFTNFMode
            , "0:��ʹ��;3:ʹ��"
            , "����ģʽ"
            , ""
            , CUSTOM_LEVEL
            );
    GetEnum("\\CamApp"
            , "NoiseReductionThreshold"
            , &Get().cCamAppParam.iTNFSNFValue
            , Get().cCamAppParam.iTNFSNFValue
            , "0:�Զ�;1:��;2:��;3:��"
            , "����ǿ��"
            , ""
            , PROJECT_LEVEL
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
        , 0
        , 255
        , "ͼ���Ե��ǿ"
        , "ͼ���Ե��ǿ"
        , CUSTOM_LEVEL
    );

	GetEnum("\\CamApp"
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
	);
	
    GetInt("\\CamApp"
          , "LEDPluseWidth"
          , &Get().cCamAppParam.iLEDPluseWidth
          , Get().cCamAppParam.iLEDPluseWidth
          , 0
          , 255
          , "���������"
          , ""
          , CUSTOM_LEVEL
      );

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

	return S_OK;
}

HRESULT CSWLPRVenusEPoliceParameter::InitCharacter(VOID)
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
		  , 3072
		  , "X����"
		  , ""
		  , CUSTOM_LEVEL
	  );
	  
	GetInt("\\Overlay\\JPEG"
		  , "Y"
		  , &Get().cOverlay.cJPEGInfo.iY
		  , Get().cOverlay.cJPEGInfo.iY
		  , 0
		  , 2048
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


	  return S_OK;
}


HRESULT CSWLPRVenusEPoliceParameter::InitGB28181(VOID)
{
	return CSWLPRParameter::InitGB28181();
}


HRESULT CSWLPRVenusEPoliceParameter::InitAutoReboot(VOID)
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

HRESULT CSWLPRVenusEPoliceParameter::InitONVIF(VOID)
{

    SW_TRACE_NORMAL(" HRESULT CSWLPRVenusEPoliceParameter::InitONVIF(VOID)");
	
	return CSWLPRParameter::InitONVIF();

}
