#include "SWFC.h"
#include "SWLPREPoliceParameter.h"

CSWLPREPoliceParameter::CSWLPREPoliceParameter()
{
	//����ģʽ
	Get().nWorkModeIndex = PRM_ELECTRONIC_POLICE;
	m_strWorkMode.Format("���Ӿ���");
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
	Get().cTrackerCfgParam.nCaptureOnePos = 80;
	Get().cTrackerCfgParam.nCaptureTwoPos = 60;
	Get().cTrackerCfgParam.nCaptureThreePos = 45;

	// �羯Ĭ��ֻ���һ�Ŵ�ͼ
	Get().cResultSenderParam.iBestSnapshotOutput = 1;
	Get().cResultSenderParam.iLastSnapshotOutput = 0;
	//�޳��Ƽ��ģʽ
	Get().cTrackerCfgParam.cVideoDet.iNoPlateDetMode = 1;
}

CSWLPREPoliceParameter::~CSWLPREPoliceParameter()
{
}

HRESULT CSWLPREPoliceParameter::InitTracker(VOID)
{
	if(S_OK == CSWLPRParameter::InitTracker())
	{
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
    }

	GetEnum("\\Tracker\\VideoDet"
		, "NoPlateDetMode"
		, &Get().cTrackerCfgParam.cVideoDet.iNoPlateDetMode
		, Get().cTrackerCfgParam.cVideoDet.iNoPlateDetMode
		, "0:��׼ģʽ;1:ģ��ģʽ"
		, "���Ƴ����ģʽ"
		, ""
		, CUSTOM_LEVEL
		);

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
    
    GetEnum("\\Tracker\\TrafficLight", "FilterMoreReview"
    	, &Get().cTrackerCfgParam.cTrafficLight.nFilterMoreReview
    	, Get().cTrackerCfgParam.cTrafficLight.nFilterMoreReview
    	, "0:�ر�;1:��"
    	, "���˴󳵳����쿪��"
    	, ""
    	, CUSTOM_LEVEL
    );
    
    GetInt("\\Tracker\\VideoDet"
    	, "EPOutPutDetectLine"
    	, &Get().cTrackerCfgParam.cVideoDet.iEPOutPutDetectLine
    	, Get().cTrackerCfgParam.cVideoDet.iEPOutPutDetectLine
    	, 0
    	, 100
    	, "���Ӿ���������ٽ�����"
    	, ""
    	, PROJECT_LEVEL
    );

    GetInt("\\Tracker\\VideoDet"
    	, "EPFGDetectLine"
    	, &Get().cTrackerCfgParam.cVideoDet.iEPFGDetectLine
    	, Get().cTrackerCfgParam.cVideoDet.iEPFGDetectLine
    	, 0
    	, 100
    	, "�羯���Ƴ������������(���붥���ٷֱ�)"
    	, ""
    	, PROJECT_LEVEL
    );

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
		return S_OK;
	}
	return E_FAIL;
}

VOID CSWLPREPoliceParameter::LoadTrafficLightParam(VOID)
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
    GetInt("\\Tracker\\Templet"
    	, "ScanSpeed"
    	, &Get().cTrackerCfgParam.cTrafficLight.nMatchScanSpeed
    	, Get().cTrackerCfgParam.cTrafficLight.nMatchScanSpeed
    	, 0
    	, 100
    	, "ɨ���ٶ�(0:��, 1:��)"
    	, ""
    	, PROJECT_LEVEL
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
    	, "SpecificLeft"
    	, &Get().cTrackerCfgParam.cTrafficLight.nSpecificLeft
    	, Get().cTrackerCfgParam.cTrafficLight.nSpecificLeft
    	, "0:��;1:��"
    	, "��ת�����"
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
		, CUSTOM_LEVEL
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
    
    GetInt("Tracker\\TrafficLight"
    	, "HMThreld"
    	, &Get().cTrackerCfgParam.cTrafficLight.fltHMThreld
    	, Get().cTrackerCfgParam.cTrafficLight.fltHMThreld
    	, 0
    	, 100
    	, "������ֵ"
    	, ""
    	, PROJECT_LEVEL
    );
    
    GetInt("Tracker\\TrafficLight"
    	, "HMThreldNight"
    	, &Get().cTrackerCfgParam.cTrafficLight.fltHMThreldNight
    	, Get().cTrackerCfgParam.cTrafficLight.fltHMThreldNight
    	, 0
    	, 100
    	, "ҹ����ֵ"
    	, ""
    	, PROJECT_LEVEL
    );
    
    GetInt("Tracker\\TrafficLight","HMDelay"
    	, &Get().cTrackerCfgParam.cTrafficLight.iHMDelay
    	, Get().cTrackerCfgParam.cTrafficLight.iHMDelay
    	, 1
    	, 100
    	, "�����ӳ�֡��"
    	, ""
    	, PROJECT_LEVEL
    );
    
    GetInt("Tracker\\TrafficLight"
    	, "HMDelayNight"
    	, &Get().cTrackerCfgParam.cTrafficLight.iHMDelayNight
    	, Get().cTrackerCfgParam.cTrafficLight.iHMDelayNight
    	, 1
    	, 100
    	, "ҹ���ӳ�֡��"
    	, ""
    	, PROJECT_LEVEL
    );
    
    GetInt("Tracker\\TrafficLight"
    	, "MaxMatchDiff"
    	, &Get().cTrackerCfgParam.cTrafficLight.iMaxMatchDiff
    	, Get().cTrackerCfgParam.cTrafficLight.iMaxMatchDiff
    	, 1
    	, 100
    	, "������ֵ"
    	, ""
    	, PROJECT_LEVEL
    );
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
    
    GetEnum("\\Tracker\\TrafficLight"
    	, "FilterNoPlatePeccancy"
    	, &Get().cTrackerCfgParam.cTrafficLight.fFilterNoPlatePeccancy
    	, Get().cTrackerCfgParam.cTrafficLight.fFilterNoPlatePeccancy
    	, "0:������;1:����"
    	, "�����޳���Υ�½��"
    	, ""
    	, CUSTOM_LEVEL
    );

	// Ŀǰ������֧��ץ�ġ�
	/*
	GetEnum("\\Tracker\\TrafficLight"
		, "EnableFlashLight"
		, &Get().cTrackerCfgParam.fEnableFlashLight
		, Get().cTrackerCfgParam.fEnableFlashLight
		, "0:�ر�;1��"
		, "ץ�Ĳ����ģʽ����"
		, ""
		, CUSTOM_LEVEL
		);

	GetInt("\\Tracker\\TrafficLight"
		, "FlashLightThreshold"
		, &Get().cTrackerCfgParam.nFlashLightThreshold
		, Get().cTrackerCfgParam.nFlashLightThreshold
		, 0
		, 255
		, "ץ�Ĳ���������ֵ"
		, ""
		, CUSTOM_LEVEL
		);
		*/
}
	
HRESULT CSWLPREPoliceParameter::InitHvDsp(VOID)
{
	if(S_OK == CSWLPRParameter::InitHvDsp())
	{
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
		return S_OK;
	}
	return E_FAIL;
}

HRESULT CSWLPREPoliceParameter::InitScaleSpeed(VOID)
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

HRESULT CSWLPREPoliceParameter::InitCamApp(VOID)
{
	if(S_OK == CSWLPRParameter::InitCamApp())
	{
		GetEnum("\\HvDsp\\Camera\\Ctrl"
    	, "DynamicTriggerEnable"
    	, &Get().cCamCfgParam.iDynamicTriggerEnable
    	, Get().cCamCfgParam.iDynamicTriggerEnable
      , "0:�ر�;1:��"
      , "��Ƶ����ץ�����"
      , "�Դ���Jϵ�������HV�������Ч"
      , PROJECT_LEVEL
    );
		return S_OK;
	}
	return E_FAIL;
}

HRESULT CSWLPREPoliceParameter::InitOuterCtrl(VOID)
{
	if(S_OK == CSWLPRParameter::InitOuterCtrl())
	{
		GetEnum("\\OuterCtrl[���ܿ�]"
		, "RecogSnapImg"
		, &Get().cMatchParam.fCaptureRecong
		, Get().cMatchParam.fCaptureRecong
		, "0:�ر�;1:��"
		, "ץ��ͼƬʶ�𿪹�"
		, ""
		, PROJECT_LEVEL
		);

		CHAR szRecogArea[64] = {0};
		swpa_strcpy(szRecogArea, "[0,50,100,100],9,14");
		GetString("\\OuterCtrl[���ܿ�]"
			, "RecogArea"
			, szRecogArea
			, szRecogArea
			, 64
			, "ץ��ʶ������(%)[��,��,��,��],��С���ƣ������"
			, "ץ��ʶ������(%)[��,��,��,��],��С���ƣ������"
			, PROJECT_LEVEL
			);
			Get().cTrackerCfgParam.cRecogSnapArea.fEnableDetAreaCtrl = FALSE;
			CSWString strTmp;
			for(int i = 0; i < sizeof(Get().cMatchParam.signal)/sizeof(SIGNAL_PARAM); i++)
			{
				strTmp.Format("\\OuterCtrl[���ܿ�]\\Signal%02d", i);
				GetEnum((LPCSTR)strTmp
  		  	, "SignalType"
  		  	, (INT *)&Get().cMatchParam.signal[i].dwType
  		  	, Get().cMatchParam.signal[i].dwType
  		  	, "0:��;1:�ٶ�;2:����"
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
					, "0:������ץ��"
					, "���ź��Ƿ�Ҫ����ץ��"
					, ""
					, CUSTOM_LEVEL
				);
				
				GetEnum((LPCSTR)strTmp
					, "Recognize"
					, (INT *)&Get().cMatchParam.signal[i].fRecognize
					, Get().cMatchParam.signal[i].fRecognize
					, "0:��ʶ��"
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
	return E_FAIL;
}

