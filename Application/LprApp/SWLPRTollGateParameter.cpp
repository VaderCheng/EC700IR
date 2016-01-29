#include "SWFC.h"
#include "SWLPRApplication.h"
#include "SWLPRTollGateParameter.h"


CSWLPRTollGateParameter::CSWLPRTollGateParameter()
{
    Get().nWorkModeIndex = PRM_TOLLGATE;
    m_strWorkMode.Format("�շ�վ", Get().nWorkModeIndex);

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
}

CSWLPRTollGateParameter::~CSWLPRTollGateParameter()
{
}

HRESULT CSWLPRTollGateParameter::InitTracker(VOID)
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
        , CUSTOM_LEVEL
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
        , CUSTOM_LEVEL
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

    GetEnum("Tracker\\DetAreaCtrl\\Normal"
        , "EnableTrap"
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

    GetInt("Tracker\\DetAreaCtrl\\Normal", "BottomLeftX"
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
    Get().cTrackerCfgParam.cDetectArea.DetectorAreaLeft =
        Get().cTrackerCfgParam.cTrapArea.TopLeftX > Get().cTrackerCfgParam.cTrapArea.BottomLeftX ?
            Get().cTrackerCfgParam.cTrapArea.BottomLeftX : Get().cTrackerCfgParam.cTrapArea.TopLeftX;
    Get().cTrackerCfgParam.cDetectArea.DetectorAreaRight =
        Get().cTrackerCfgParam.cTrapArea.TopRightX < Get().cTrackerCfgParam.cTrapArea.BottomRightX ?
            Get().cTrackerCfgParam.cTrapArea.BottomRightX : Get().cTrackerCfgParam.cTrapArea.TopRightX;
    Get().cTrackerCfgParam.cDetectArea.DetectorAreaTop =
        Get().cTrackerCfgParam.cTrapArea.TopLeftY > Get().cTrackerCfgParam.cTrapArea.TopRightY ?
            Get().cTrackerCfgParam.cTrapArea.TopRightY : Get().cTrackerCfgParam.cTrapArea.TopLeftY;
    Get().cTrackerCfgParam.cDetectArea.DetectorAreaBottom =
        Get().cTrackerCfgParam.cTrapArea.BottomLeftY > Get().cTrackerCfgParam.cTrapArea.BottomRightY ?
            Get().cTrackerCfgParam.cTrapArea.BottomLeftY : Get().cTrackerCfgParam.cTrapArea.BottomRightY;

    //Scale��Ч���ж�
    if (Get().cTrackerCfgParam.cDetectArea.nDetectorMinScaleNum
        > Get().cTrackerCfgParam.cDetectArea.nDetectorMaxScaleNum)
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
        , CUSTOM_LEVEL
        );

	//������ɫʶ�����	
	GetInt("\\Tracker\\ProcessPlate"
        , "EnableColorReRecog"
        , &Get().cTrackerCfgParam.nEnableColorReRecog
        , Get().cTrackerCfgParam.nEnableColorReRecog
        , 0
        , 7
        , "�ڰ׻���ʶ��"
        , ""
        , CUSTOM_LEVEL
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
        , Get().cTrackerCfgParam.fEnableCorlorThr
        , 0
        , 1
        , "�Һڿ��ƿ���"
        , ""
        , CUSTOM_LEVEL
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

HRESULT CSWLPRTollGateParameter::InitHvDsp(VOID)
{
/*	
    GetEnum("\\HvDsp\\Misc"
        , "OutputAppendInfo"
        , &Get().cResultSenderParam.fOutputAppendInfo
        , Get().cResultSenderParam.fOutputAppendInfo
        , "0:�ر�;1:��"
        , "���������Ϣ����"
        , ""
        , PROJECT_LEVEL
        );

    // ��������.
    Get().cResultSenderParam.cProcRule.fOutPutFilterInfo = Get().cResultSenderParam.fOutputAppendInfo;

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
    GetInt("\\HvDsp\\Trigger"
        , "TriggerOutNormalStatus"
        , &Get().cResultSenderParam.nTriggerOutNormalStatus
        , Get().cResultSenderParam.nTriggerOutNormalStatus
        , 0
        , 1
        , "���������̬���ƣ�0:�ߵ�ƽ��1:�͵�ƽ"
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

	//�շ�վĬ�ϲ��������
    /*
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
        */
    return S_OK ;
}

HRESULT CSWLPRTollGateParameter::InitOuterCtrl(VOID)
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

    CSWString strTmp;
    for (int i = 0; i < sizeof(Get().cMatchParam.signal) / sizeof(SIGNAL_PARAM); i++)
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
            , "Condition"
            , (INT *)&Get().cMatchParam.signal[i].dwCondition
            , Get().cMatchParam.signal[i].dwCondition
            , "0:���źſ��п���,ֻҪ�г��Ƽ��ɳ���;1:�����д��źźͳ��Ʋ��ܳ����;2:ֻҪ�д��ź�,�ͱض�����"
            , "ƥ������"
            , ""
            , CUSTOM_LEVEL
            );
    }
    return S_OK ;
}

HRESULT CSWLPRTollGateParameter::InitScaleSpeed(VOID)
{
	return S_OK;
}

HRESULT CSWLPRTollGateParameter::InitIPTCtrl(VOID)
{
	if(S_OK == CSWLPRParameter::InitIPTCtrl())
	{
		CSWString strCOMM;
		for(int i = 0; i < 2; i++)
		{
			strCOMM.Format("\\Device[�ⲿ�豸]\\COM%d[�˿�%02d]", i, i);
			GetEnum((LPCSTR)strCOMM
  		  	, "DeviceType"
  		  	, &Get().cDevParam[i].iDeviceType
  		  	, 0
  		  	, "0:��;1:�����״�;2:�ս�������"
  		  	, "�ⲿ�豸����"
  		  	, ""
  		  	, CUSTOM_LEVEL
  		);
		}
		return S_OK;
	}
	return E_FAIL;
}

HRESULT CSWLPRTollGateParameter::Init368(VOID)
{
	if(S_OK == CSWLPRParameter::Init368())
	{
		Get().cCamAppParam.iFlashDifferentLaneExt = 0;
		GetEnum("\\CamApp"
    	, "FlashDifferentLaneEx"
    	, &Get().cCamAppParam.iFlashDifferentLaneExt
    	, Get().cCamAppParam.iFlashDifferentLaneExt
    	, "0:���ֳ�����;1:�ֳ�����"
    	, "����Ʒֳ�����(Ӳ����)"
    	, ""
    	, PROJECT_LEVEL
    );
		return S_OK;
	}
	return E_FAIL;
}