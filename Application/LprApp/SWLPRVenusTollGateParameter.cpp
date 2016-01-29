/*
 * SWLPRVenusTollGateParameter.cpp
 *
 *  Created on: 2013��12��27��
 *      Author: qinjj
 */
#include "SWFC.h"
#include "SWLPRApplication.h"
#include "SWLPRVenusTollGateParameter.h"
typedef struct Point{
    DOUBLE X;
    DOUBLE Y;
    Point(DOUBLE x,DOUBLE y)
    {
        X = x;
        Y = y;
    }
    Point()
    {
        memset(this,0,sizeof(this));
    }
}Point;


Point CalcLineCrossPoint(Point line1From, Point line1End, Point line2From, Point line2End)
{
    double k1 = 0, c1 = 0, k2 = 0, c2 = 0, x = -1, y = -1;
    bool Vertical1 = false, Vertical2 = false;
    if (line1From.X == line1End.X)
    {
        Vertical1 = true;
    }
    else
    {
        k1 = (line1End.Y - line1From.Y) / (line1End.X - line1From.X);
        c1 = line1From.Y - k1 * line1From.X;
    }

    if (line2From.X == line2End.X)
    {
        Vertical2 = true;
    }
    else
    {
        k2 = (line2End.Y - line2From.Y) / (line2End.X - line2From.X);
        c2 = line2From.Y - k2 * line2From.X;
    }

    if (Vertical1 && Vertical2)
    {
        x = -1;
        y = -1;
    }
    else if (Vertical1)
    {
        x = line1From.X;
        y = k2 * x + c2;
    }
    else if (Vertical2)
    {
        x = line2From.X;
        y = k1 * x + c1;
    }
    else if (k1 != k2)
    {
        x = (k1 * line1From.X - k2 * line2From.X - line1From.Y + line2From.Y) / (k1 - k2);
        y = k1 * x + c1;
    }
//    Point point;
//    point.X = x;
//    point.Y = y;
    return Point(x,y);
}

CSWLPRVenusTollGateParameter::CSWLPRVenusTollGateParameter()
{
	ResetParam();
}

CSWLPRVenusTollGateParameter::~CSWLPRVenusTollGateParameter()
{
}

void CSWLPRVenusTollGateParameter::ResetParam(VOID)
{
	//�ȵ��û���
	CSWLPRParameter::ResetParam();

	/*���ݾ��巽�����ò���*/

    Get().nWorkModeIndex = PRM_TOLLGATE;
    m_strWorkMode.Format("�շ�վ", Get().nWorkModeIndex);
	Get().nCameraWorkMode = 0;		//��������շ�վ�����������ģʽ
	Get().nMCFWUsecase = IMX185;

    //����Ĭ��ֵ
    for (int i = 0; i < MAX_ROADLINE_NUM; i++)
    {
        swpa_memset(&Get().cTrackerCfgParam.rgcRoadInfo[i], 0, sizeof(ROAD_INFO));
        swpa_strcpy(Get().cTrackerCfgParam.rgcRoadInfo[i].szRoadTypeName, "NULL");
        Get().cTrackerCfgParam.rgcRoadInfo[i].iLineType = 1;
        Get().cTrackerCfgParam.nRoadLineNumber = 2;
        Get().cTrackerCfgParam.rgcRoadInfo[i].iRoadType = (int)(RRT_FORWARD | RRT_LEFT | RRT_RIGHT | RRT_TURN);
        Get().cTrackerCfgParam.cActionDetect.iIsYellowLine[i] = 0;
        Get().cTrackerCfgParam.cActionDetect.iIsCrossLine[i] = 0;
    }


    //ͶƱ����
    Get().cTrackerCfgParam.nVoteFrameNum = 16;
    Get().cTrackerCfgParam.nMaxEqualFrameNumForVote = 8;

    Get().cTrackerCfgParam.iRoadNumberBegin  = 0;
    Get().cTrackerCfgParam.iStartRoadNum = 0;

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

//	// ץ��λ�ó�ʼֵ
//	Get().cTrackerCfgParam.nCaptureOnePos = 50;

	// ������Ƶ�������
	Get().cTrackerCfgParam.iUsedLight = 0;

	//����ɨ������Ĭ�ϴ�
	Get().cTrackerCfgParam.cTrapArea.fEnableDetAreaCtrl = TRUE;

	//����RGB�������÷�ΧΪ36~255,Ĭ��ֵ����Ϊ0
	Get().cCamAppParam.iGainR = 36;
	Get().cCamAppParam.iGainG = 36;
	Get().cCamAppParam.iGainB = 36;

}


HRESULT CSWLPRVenusTollGateParameter::Initialize(CSWString strFilePath)
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
        && S_OK == InitAutoReboot())
    {
        return S_OK ;
    }

    SW_TRACE_DEBUG("<CSWLPRVenusTollGateParameter> Initialize failed.\n");
    return E_FAIL ;
}

HRESULT CSWLPRVenusTollGateParameter::InitSystem(VOID)
{
    if (S_OK == CSWLPRParameter::InitSystem())
    {

        return S_OK ;
    }
    return E_FAIL ;

}

HRESULT CSWLPRVenusTollGateParameter::InitTracker(VOID)
{
//#define YYY_DEBUG
#ifdef YYY_DEBUG
    GetInt("\\Tracker\\TrafficLight"
        , "OnePos"
        , &Get().cTrackerCfgParam.nCaptureOnePos
        , Get().cTrackerCfgParam.nCaptureOnePos
        , 0
        , 100
        , "ץ�ĵ�һ��ͼ��λ��"
        , ""
        , PROJECT_LEVEL
    );

    GetInt("\\Tracker\\TrafficLight"
        , "TwoPos"
        , &Get().cTrackerCfgParam.nCaptureTwoPos
        , Get().cTrackerCfgParam.nCaptureTwoPos
        , 0
        , 100
        , "ץ�ĵڶ���ͼ��λ��"
        , ""
        , PROJECT_LEVEL
    );
    Get().cTrackerCfgParam.nCaptureThreePos = 10;
    GetInt("\\Tracker\\TrafficLight"
        , "ThreePos"
        , &Get().cTrackerCfgParam.nCaptureThreePos
        , Get().cTrackerCfgParam.nCaptureThreePos
        , 0
        , 100
        , "ץ�ĵ�����ͼ��λ��"
        , ""
        , PROJECT_LEVEL
    );
#endif
//    GetEnum("Tracker\\Misc"
//        , "OutPutNoPlate"
//        , &Get().cTrackerCfgParam.iOutPutNoPlate
//        , Get().cTrackerCfgParam.iOutPutNoPlate
//        , "0:�ر�;1:��"
//        , "������Ƴ�"
//        , "��Ƶ���շ�վ����޳��ƽ��"
//        , PROJECT_LEVEL
//        );


    GetEnum("\\Tracker\\ProcessPlate"
        , "EnableDBGreenSegment"
        , &Get().cTrackerCfgParam.nPlateDetect_Green
        , Get().cTrackerCfgParam.nPlateDetect_Green
        , "0:�ر�;1:��"
        , "����ʶ�𿪹�"
        , ""
        , PROJECT_LEVEL
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
        , &Get().cTrackerCfgParam.nNightThreshold
        , Get().cTrackerCfgParam.nNightThreshold
        , 0
        , 240
        , "ҹ��ģʽ��������"
        , ""
        , PROJECT_LEVEL				//�㷨�Ż���طŵ�3������
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
        , PROJECT_LEVEL
        );

    Get().cTrackerCfgParam.nProcessPlate_BlackPlate_L = 30;
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

    GetInt("\\Tracker\\RoadInfo"
        , "RoadLineNumber"
        , &Get().cTrackerCfgParam.nRoadLineNumber
        , Get().cTrackerCfgParam.nRoadLineNumber
        , 2
        , 2
        , "����������"
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

    for (int i = 0; i < 2; i++)
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
            , PROJECT_LEVEL
        );

        GetInt(szSection
            , "RoadType"
            , &Get().cTrackerCfgParam.rgcRoadInfo[i].iRoadType
            , Get().cTrackerCfgParam.rgcRoadInfo[i].iRoadType
            , 1
            , 9999
            , "��Ӧ�ĳ�������"
            , ""
            , PROJECT_LEVEL
        );
    }

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
        , 1 //default value
        , "0:�ر�;1:��"
        , "������ĸʶ�𿪹�"
        , ""
        , PROJECT_LEVEL
        );

    Get().cTrackerCfgParam.iOutPutNoPlate = 1;
    GetEnum("Tracker\\Misc"
	   , "OutPutNoPlate"
	   , &Get().cTrackerCfgParam.iOutPutNoPlate
	   , Get().cTrackerCfgParam.iOutPutNoPlate
	   , "0:�ر�;1:��"
	   , "���Ƴ�ʶ�𿪹�"
	   , ""
	   , PROJECT_LEVEL
	   );

   GetInt("Tracker\\Misc"
	   , "ScanAreaTopOffset"
	   , &Get().cTrackerCfgParam.iScanAreaTopOffset
	   , Get().cTrackerCfgParam.iScanAreaTopOffset
	   , 0
	   , 100
	   , "����������봥���߰ٷֱ�"
	   , ""
	   , CUSTOM_LEVEL
	   );

   GetInt("Tracker\\Misc"
	   , "ScanAreaBottomOffset"
	   , &Get().cTrackerCfgParam.iScanAreaBottomOffset
	   , Get().cTrackerCfgParam.iScanAreaBottomOffset
	   , 0
	   , 100
	   , "������ײ��봥���߰ٷֱ�"
	   , ""
	   , CUSTOM_LEVEL
	   );

	Get().cTrackerCfgParam.nRecogInVaildRatioTH = 40;
	GetInt("Tracker\\Misc"
		, "RecogInVaildRatioTH"
		, &Get().cTrackerCfgParam.nRecogInVaildRatioTH
		, Get().cTrackerCfgParam.nRecogInVaildRatioTH
		, 0
		, 100
		, "ʶ����Ч������ֵ"
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

    Get().cTrackerCfgParam.nCarArrivedPos = 60;
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

    Get().cTrackerCfgParam.iBuleTriggerToYellowOffset = 12;
    GetInt("Tracker\\Misc"
        , "BuleTriggerToYellowOffset"
        , &Get().cTrackerCfgParam.iBuleTriggerToYellowOffset
        , Get().cTrackerCfgParam.iBuleTriggerToYellowOffset
        , 0
        , 100
        , "���ƴ����ߵ����ƴ����߰ٷֱ�"
        , ""
        , CUSTOM_LEVEL
        );
/*
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
*/
  //  Get().cTrackerCfgParam.nCarArrivedPosYellow = 50;
//    GetInt("Tracker\\Misc"
//        , "CarArrivedPosYellow"
//        , &Get().cTrackerCfgParam.nCarArrivedPosYellow
//        , Get().cTrackerCfgParam.nCarArrivedPosYellow
//        , 0
//        , 100
//        , "��������λ��(����)"
//        , ""
//        , CUSTOM_LEVEL
//        );

	GetEnum("Tracker\\Misc"
        , "MovingDirection"
        , &Get().cTrackerCfgParam.iMovingDirection
        , Get().cTrackerCfgParam.iMovingDirection
        , "0:��Զ����;1:�ɽ���Զ;4:���ⷽ��;"
        , "������ʻ����"
        , ""
        , CUSTOM_LEVEL
        );

	GetEnum("Tracker\\Misc"
        , "FilterReverseEnable"
        , &Get().cTrackerCfgParam.fFilterReverseEnable
        , Get().cTrackerCfgParam.fFilterReverseEnable
        , "0:������;1:����"
        , "�������з���"
        , ""
        , CUSTOM_LEVEL
        );

	GetInt("Tracker\\Misc"
        , "nSpan"
        , &Get().cTrackerCfgParam.nSpan
        , Get().cTrackerCfgParam.nSpan
        , -100
        , 100
        , "���з�����ֵ"
        , ""
        , PROJECT_LEVEL
        );

    Get().cTrackerCfgParam.nVoteFrameNum = 6;
    GetInt("Tracker\\TrackInfo"
        , "VoteFrameNum"
        , &Get().cTrackerCfgParam.nVoteFrameNum
        , Get().cTrackerCfgParam.nVoteFrameNum
        , 2
        , 1000
        , "ͶƱ�Ľ����"
        , ""
        , CUSTOM_LEVEL
        );
		
    Get().cTrackerCfgParam.nMaxEqualFrameNumForVote = 3;
    GetInt("Tracker\\TrackInfo"
        , "MaxEqualFrameNumForVote"
        , &Get().cTrackerCfgParam.nMaxEqualFrameNumForVote
        , Get().cTrackerCfgParam.nMaxEqualFrameNumForVote
        , 1, 1000
        , "������ͬ�����������"
        , ""
        , CUSTOM_LEVEL
        );
    Get().cTrackerCfgParam.nBlockTwinsTimeout = 60;
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

//    GetInt("Tracker\\TrackInfo"
//        , "MJPEGHeight"
//        , &Get().nMJPEGHeight
//        , Get().nMJPEGHeight
//        , 0
//        , 10000
//        , "JPEGͼ��߶�"
//        , ""
//        , PROJECT_LEVEL
//        );
//    GetInt("Tracker\\TrackInfo"
//        , "MJPEGWidth"
//        , &Get().nMJPEGWidth
//        , Get().nMJPEGWidth
//        , 0
//        , 10000
//        , "JPEGͼ����"
//        , ""
//        , PROJECT_LEVEL
//        );

//#ifndef YYY_DEBUG
//    INT CaptureOnePos = 28;
//    INT CaptureTwoPos = 12;
//#else
//    INT CaptureOnePos = Get().cTrackerCfgParam.nCaptureOnePos;
//    INT CaptureTwoPos = Get().cTrackerCfgParam.nCaptureTwoPos;
//#endif


    //��·��1
    Point line1From(Get().cTrackerCfgParam.rgcRoadInfo[0].ptTop.x,Get().cTrackerCfgParam.rgcRoadInfo[0].ptTop.y);
    Point line1End(Get().cTrackerCfgParam.rgcRoadInfo[0].ptBottom.x,Get().cTrackerCfgParam.rgcRoadInfo[0].ptBottom.y);
    //��·��2
    Point line2From(Get().cTrackerCfgParam.rgcRoadInfo[1].ptTop.x,Get().cTrackerCfgParam.rgcRoadInfo[1].ptTop.y);
    Point line2End(Get().cTrackerCfgParam.rgcRoadInfo[1].ptBottom.x,Get().cTrackerCfgParam.rgcRoadInfo[1].ptBottom.y);

//    GetEnum("Tracker\\DetAreaCtrl\\Normal"
//        , "EnableTrap"
//        , &Get().cTrackerCfgParam.cTrapArea.fEnableDetAreaCtrl
//        , Get().cTrackerCfgParam.cTrapArea.fEnableDetAreaCtrl
//        , "0:�ر�;1:��"
//        , "����ɨ��������ƿ���"
//        , ""
//        , PROJECT_LEVEL
//        );

    GetEnum("\\Tracker\\Recognition"
        , "EnableRecogCarColor"
        , &Get().cTrackerCfgParam.fEnableRecgCarColor
        , Get().cTrackerCfgParam.fEnableRecgCarColor
        , "0:��;1:��"
        , "������ɫʶ�𿪹�"
        , ""
        , CUSTOM_LEVEL
        );

    GetInt("\\Tracker\\ProcessPlate"
        , "SegWhitePlate"
        , &Get().cTrackerCfgParam.nSegWhitePlate
        , Get().cTrackerCfgParam.nSegWhitePlate
        , 0
        , 1
        , "ǿ�ư��Ʒָ�"
        , ""
        , PROJECT_LEVEL				//�㷨�Ż���طŵ�3������
        );

	//������ɫʶ�����
	GetInt("\\Tracker\\ProcessPlate"
        , "EnableColorReRecog"
        , &Get().cTrackerCfgParam.nEnableColorReRecog
        , 3	//default value
        , 0
        , 7
        , "�ڰ׻���ʶ��"
        , ""
        , PROJECT_LEVEL
        );

	GetInt("\\Tracker\\ProcessPlate"
        , "nBColorReThr"
        , &Get().cTrackerCfgParam.nBColorReThr
        , Get().cTrackerCfgParam.nBColorReThr
        , -60
        , 60
        , "����ʶ����ֵ"
        , ""
        , PROJECT_LEVEL
        );

	GetInt("\\Tracker\\ProcessPlate"
        , "nWColorReThr"
        , &Get().cTrackerCfgParam.nWColorReThr
        , Get().cTrackerCfgParam.nWColorReThr
        , 0
        , 240
        , "����ʶ����ֵ"
        , ""
        , PROJECT_LEVEL
        );

	GetInt("\\Tracker\\ProcessPlate"
        , "nWGColorReThr"
        , &Get().cTrackerCfgParam.nWGColorReThr
        , Get().cTrackerCfgParam.nWGColorReThr
        , 0
        , 240
        , "�Ұ���ʶ����ֵ"
        , ""
        , PROJECT_LEVEL
        );

	GetInt("\\Tracker\\ProcessPlate"
        , "EnableColorThr"
        , &Get().cTrackerCfgParam.fEnableCorlorThr
        , 0 //default
        , 0
        , 1
        , "�Һڿ��ƿ���"
        , ""
        , PROJECT_LEVEL
        );

	GetInt("\\Tracker\\ProcessPlate"
        , "nBGColorThr"
        , &Get().cTrackerCfgParam.nBGColorThr
        , Get().cTrackerCfgParam.nBGColorThr
        , 0
        , 240
        , "�Һڿ�����ֵ"
        , ""
        , PROJECT_LEVEL
        );

	GetInt("\\Tracker\\ProcessPlate"
        , "nWGColorThr"
        , &Get().cTrackerCfgParam.nWGColorThr
        , Get().cTrackerCfgParam.nWGColorThr
        , 0
        , 240
        , "�Ұ׿�����ֵ"
        , ""
        , PROJECT_LEVEL
        );

    return S_OK;
}

HRESULT CSWLPRVenusTollGateParameter::InitHvDsp(VOID)
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

    // Ĭ�������������ʱ��
    Get().cResultSenderParam.fOutputCarArriveTime = TRUE;

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

    GetInt("\\HvDsp\\Misc"
        , "SendRecordSpace"
        , &Get().cResultSenderParam.iSendRecordSpace
        , Get().cResultSenderParam.iSendRecordSpace
        , 0
        , 100
        , "������ͼ��(*100ms)"
        , ""
        , CUSTOM_LEVEL
        );

    GetInt("\\HvDsp\\Misc"
        , "SendHisVideoSpace"
        , &Get().cResultSenderParam.iSendHisVideoSpace
        , Get().cResultSenderParam.iSendHisVideoSpace
        , 1
        , 20
        , "��ʷ¼���ͼ��(*100ms)"
        , ""
        , CUSTOM_LEVEL
        );

    Get().cResultSenderParam.iBestSnapshotOutput = 0;
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

HRESULT CSWLPRVenusTollGateParameter::InitOuterCtrl(VOID)
{
#if 0		//��ʱȥ��������Ҫ����������ٴ�
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

    CSWString strTmp;
    for (int i = 0; i < sizeof(Get().cMatchParam.signal) / sizeof(SIGNAL_PARAM); i++)
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

#endif
    return S_OK ;
}


HRESULT CSWLPRVenusTollGateParameter::InitIPTCtrl(VOID)
{
#if 1		//���ڲ�����Ҫ���򿪲�����������������

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
		, PROJECT_LEVEL
	);
	  
	GetEnum((LPCSTR)strCOMM
		, "DeviceType"
		, &Get().cDevParam[i].iDeviceType
		, 0
		, "0:��;1:�����״�;2:�ս�������;4:����ά���״�;5:�Ĵ������״�"
		, "�ⲿ�豸����"
		, ""
		, PROJECT_LEVEL
	);
	GetInt((LPCSTR)strCOMM
		, "Road"
		, &Get().cDevParam[i].iRoadNo
		, Get().cDevParam[i].iRoadNo
		, 0
		, 255
		, "�����������"
		, "�����������"
		, PROJECT_LEVEL
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
		, PROJECT_LEVEL
	);
#endif
		  
	return S_OK;
}


HRESULT CSWLPRVenusTollGateParameter::InitCamApp(VOID)
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
      Get().cCamCfgParam.iEnableAGC = 1;
//	  GetEnum("\\HvDsp\\Camera\\Ctrl"
//		  , "AGCEnable"
//		  , &Get().cCamCfgParam.iEnableAGC
//		  , Get().cCamCfgParam.iEnableAGC
//		, "0:�ر�;1:��"
//		, "ʹ��AGC"
//		, ""
//		, CUSTOM_LEVEL
//	  );

//	  GetEnum("\\HvDsp\\Camera\\Ctrl"
//		  , "AutoParamEnable"
//		  , &Get().cCamCfgParam.iAutoParamEnable
//		  , Get().cCamCfgParam.iAutoParamEnable
//		, "0:�ر�;1:��"
//		, "�Զ�����������"
//		, ""
//		, CUSTOM_LEVEL
//	  );
      Get().cCamCfgParam.iAutoParamEnable = 1;
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
	  
//	  GetFloat("\\HvDsp\\Camera\\Ctrl"
//		  , "DB"
//		  , &Get().cCamCfgParam.fltDB
//		  , Get().cCamCfgParam.fltDB
//		  , 1.0f
//		  , 10.0f
//		  , "����"
//		  , ""
//		  , CUSTOM_LEVEL
//	  );
	  
//	  GetInt("\\HvDsp\\Camera\\Ctrl"
//		  , "MinExposureTime"
//		  , &Get().cCamCfgParam.iMinExposureTime
//		  , Get().cCamCfgParam.iMinExposureTime
//		  , 0
//		  , 30000
//		  , "��С�ع�ʱ��"
//		  , ""
//		  , CUSTOM_LEVEL
//	  );
	  
//	  GetInt("\\HvDsp\\Camera\\Ctrl"
//		  , "MinPlus"
//		  , &Get().cCamCfgParam.iMinGain
//		  , Get().cCamCfgParam.iMinGain
//		  , 70
//		  , 360
//		  , "��С����"
//		  , ""
//		  , CUSTOM_LEVEL
//	  );
	  
//	  GetInt("\\HvDsp\\Camera\\Ctrl"
//		  , "MaxExposureTime"
//		  , &Get().cCamCfgParam.iMaxExposureTime
//		  , Get().cCamCfgParam.iMaxExposureTime
//		  , 0
//		  , 30000
//		  , "����ع�ʱ��"
//		  , ""
//		  , CUSTOM_LEVEL
//	  );
	  
//	  GetInt("\\HvDsp\\Camera\\Ctrl"
//		  , "MaxPlus"
//		  , &Get().cCamCfgParam.iMaxGain
//		  , Get().cCamCfgParam.iMaxGain
//		  , 70
//		  , 360
//		  , "�������"
//		  , ""
//		  , CUSTOM_LEVEL
//	  );
	
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

HRESULT CSWLPRVenusTollGateParameter::InitCamera(VOID)
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
//    JpegCompressRateHigh = 80;
//    GetInt("\\CamApp"
//    	, "JpegCompressRateHigh"
//    	, &Get().cCamAppParam.iJpegCompressRateH
//    	, Get().cCamAppParam.iJpegCompressRateH
//    	, 1
//    	, 100
//    	, "Jpegѹ�����Զ���������"
//    	, ""
//    	, CUSTOM_LEVEL
//    );

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
    	, CUSTOM_LEVEL
    );

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

    Get().cCamAppParam.iSNFTNFMode = 3;
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

	GetEnum("\\CamApp"
		, "LEDPolarity"
		, &Get().cCamAppParam.iLEDPolarity
		, Get().cCamAppParam.iLEDPolarity
		, "0:������;1:������"
		, "����ƿ��Ƽ���"
		, ""
		, CUSTOM_LEVEL
	);
/*
	GetInt("\\CamApp"
		, "LEDPulseWidth"
		, &Get().cCamAppParam.iLEDPulseWidth
		, Get().cCamAppParam.iLEDPulseWidth
		, 100
		, 20000
		, "���������"
		, "100 �� 20000. ΢��"
		, CUSTOM_LEVEL
	);
*/

	GetEnum("\\CamApp"
		, "LEDOutputType"
		, &Get().cCamAppParam.iLEDOutputType
		, Get().cCamAppParam.iLEDOutputType
		, "0:��������ƽ��;1:OC(����)"
		, "������������"
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
		, "F1OutputType"
		, &Get().cCamAppParam.iF1OutputType
		, Get().cCamAppParam.iF1OutputType
		, "0:���˻�����ź�;1:���������ź�"
		, "F1�������"
		, ""
		, CUSTOM_LEVEL
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

    Get().cCamAppParam.iCVBSExport = 1;		//Ĭ�� �� �������
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
		Get().nMCFWUsecase = IMX185_CVBSEXPORT;
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
HRESULT CSWLPRVenusTollGateParameter::InitCharacter(VOID)
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
//	  SW_TRACE_DEBUG("h264 color[0x%08x][0x%02x,0x%02x,0x%02x]", Get().cOverlay.cH264Info.dwColor, dwR, dwG, dwB);
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
//	SW_TRACE_DEBUG("jpeg color[0x%08x][0x%02x,0x%02x,0x%02x]", Get().cOverlay.cJPEGInfo.iColor, dwR, dwG, dwB);
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


HRESULT CSWLPRVenusTollGateParameter::InitGB28181(VOID)
{
	return CSWLPRParameter::InitGB28181();
}

HRESULT CSWLPRVenusTollGateParameter::InitAutoReboot(VOID)
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


