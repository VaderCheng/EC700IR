#include "SWFC.h"
#include "SWLPRHvcParameter.h"

BOOL CheckArea(int nLeft, int nTop, int nRight, int nBottom)
{
    if (nLeft < 0 || nTop < 0 || nRight < 0 || nBottom < 0
        || nLeft > 100 || nTop > 100 || nRight > 100 || nBottom > 100
        || (nLeft >= nRight) || (nTop >= nBottom))
    {
        return FALSE;
    }
    return TRUE;
}

CSWLPRHvcParameter::CSWLPRHvcParameter()
{
    Get().nWorkModeIndex = PRM_HVC;
    m_strWorkMode.Format("ץ��ʶ��");

    Get().cImgFrameParam.iCamNum = 1;
    strcpy(Get().cImgFrameParam.rgstrHVCParm[0], "[0,0,100,100],200,300,30");
    Get().cTrackerCfgParam.fltMinConfForOutput = 1.0f;
}

CSWLPRHvcParameter::~CSWLPRHvcParameter()
{
}

HRESULT CSWLPRHvcParameter::Initialize(CSWString strFilePath)
{
    // �Ȼ���ĳ�ʼ��
    if (S_OK == CSWParameter<ModuleParams>::Initialize(strFilePath)
        && S_OK == InitSystem()
        && S_OK == InitTracker()
        && S_OK == InitHvDsp()
        && S_OK == InitCamApp()
        && S_OK == Init368()
        && S_OK == InitCharacter() )
    {
        return S_OK ;
    }

    SW_TRACE_DEBUG("<LPRParameter> Initialize failed.\n");
    return E_FAIL ;
}

HRESULT CSWLPRHvcParameter::Init368(VOID)
{
    if (S_OK == CSWLPRParameter::Init368())
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
        return S_OK ;
    }
    return E_FAIL ;
}

HRESULT CSWLPRHvcParameter::InitSystem(VOID)
{
    if (S_OK == CSWLPRParameter::InitSystem())
    {
        return S_OK ;
    }
    return E_FAIL ;
}

HRESULT CSWLPRHvcParameter::InitTracker(VOID)
{
    GetEnum("\\Tracker\\Misc"
        , "VoteCount"
        , &Get().cTrackerCfgParam.nVoteCount
        , Get().cTrackerCfgParam.nVoteCount
        , "0:�����;1:���"
        , "����Ƶ��������ͼ���"
        , ""
        , PROJECT_LEVEL
        );

    GetEnum("\\Tracker\\Misc"
        , "Alpha5"
        , &Get().cTrackerCfgParam.fAlpha5
        , Get().cTrackerCfgParam.fAlpha5
        , "0:�ر�;1:����"
        , "����λ��ĸʶ��"
        , ""
        , CUSTOM_LEVEL
        );

    GetEnum("\\Tracker\\Misc"
        , "Alpha6"
        , &Get().cTrackerCfgParam.fAlpha6
        , Get().cTrackerCfgParam.fAlpha6
        , "0:�ر�;1:����"
        , "����λ��ĸʶ��"
        , ""
        , CUSTOM_LEVEL
        );

    GetEnum("\\Tracker\\Misc"
        , "Alpha7"
        , &Get().cTrackerCfgParam.fAlpha7
        , Get().cTrackerCfgParam.fAlpha7
        , "0:�ر�;1:����"
        , "����λ��ĸʶ��"
        , ""
        , CUSTOM_LEVEL
        );

    GetInt("\\Tracker\\Misc"
        , "PlateEnhanceThreshold"
        , &Get().cTrackerCfgParam.nPlateEnhanceThreshold
        , Get().cTrackerCfgParam.nPlateEnhanceThreshold
        , 0
        , 255
        , "������ǿ��ֵ"
        , ""
        , PROJECT_LEVEL
        );

    GetFloat("\\Tracker\\Misc"
        , "MinConfForOutput"
        , &Get().cTrackerCfgParam.fltMinConfForOutput
        , Get().cTrackerCfgParam.fltMinConfForOutput
        , 0
        , 1
        , "���Ŷ���ֵ"
        , ""
        , PROJECT_LEVEL
        );

    GetInt("\\Tracker\\Misc"
        , "FirstPlatePos"
        , &Get().cTrackerCfgParam.nFirstPlatePos
        , Get().cTrackerCfgParam.nFirstPlatePos
        , 0
        , 1
        , "���ȳ��Ʒ�λ"
        , ""
        , PROJECT_LEVEL
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

    GetEnum("\\Tracker\\Recognition"
        , "EnableRecogCarColor"
        , &Get().cTrackerCfgParam.fEnableRecgCarColor
        , Get().cTrackerCfgParam.fEnableRecgCarColor
        , "0:��;1:��"
        , "������ɫʶ�𿪹�"
        , ""
        , CUSTOM_LEVEL
        );

    GetInt("\\Tracker\\Recognition"
        , "EnableT1Model"
        , &Get().cTrackerCfgParam.fEnableT1Model
        , Get().cTrackerCfgParam.fEnableT1Model
        , 0
        , 1
        , "T-1ģ�Ϳ���"
        , ""
        , PROJECT_LEVEL
        );

     GetInt("\\Tracker\\Misc"
        , "RecogGXPolice"
        , &Get().cTrackerCfgParam.nRecogGxPolice
        , Get().cTrackerCfgParam.nRecogGxPolice
        , 0
        , 1
        , "�ط�����ʶ�𿪹�"
        , ""
        , INNER_LEVEL
        );

    GetFloat("\\Tracker\\Misc"
        , "DetSetpAdj"
        , &Get().cTrackerCfgParam.fltPlateDetect_StepAdj
        , Get().cTrackerCfgParam.fltPlateDetect_StepAdj
        , 1.0
        , 5.0
        , "��������ϵ��"
        , ""
        , PROJECT_LEVEL
        );

    GetInt("\\Tracker\\Misc"
        , "OnePlateMode"
        , &Get().cTrackerCfgParam.fOnePlateMode
        , Get().cTrackerCfgParam.fOnePlateMode
        , 0
        , 1
        , "��ⵥ����"
        , ""
        , PROJECT_LEVEL
        );

    GetInt("\\Tracker\\Misc"
        , "SegHeight"
        , &Get().cTrackerCfgParam.nSegHeight
        , Get().cTrackerCfgParam.nSegHeight
        , 10
        , 100
        , "�ָ�ο��߶�"
        , ""
        , PROJECT_LEVEL
        );

    GetInt("\\Tracker\\ProcessPlate"
        , "EnableDBGreenSegment"
        , &Get().cTrackerCfgParam.nPlateDetect_Green
        , Get().cTrackerCfgParam.nPlateDetect_Green
        , 0
        , 1
        , "����ʶ�𿪹�"
        , ""
        , CUSTOM_LEVEL
        );

    GetInt("\\Tracker\\DoublePlate"
        , "DoublePlateEnable"
        , &Get().cTrackerCfgParam.fDoublePlateEnable
        , Get().cTrackerCfgParam.fDoublePlateEnable
        , 1
        , 3
        , "˫���Ƽ�⿪��"
        , ""
        , PROJECT_LEVEL
        );

    GetInt("\\Tracker\\ProcessPlate"
        , "LightBlueFlag"
        , &Get().cTrackerCfgParam.nProcessPlate_LightBlue
        , Get().cTrackerCfgParam.nProcessPlate_LightBlue
        , 0
        , 1
        , "ǳ���ƿ���"
        , ""
        , PROJECT_LEVEL
        );

    GetInt("\\Tracker\\ProcessPlate\\BlackPlate"
        , "Enable"
        , &Get().cTrackerCfgParam.fProcessPlate_BlackPlate_Enable
        , Get().cTrackerCfgParam.fProcessPlate_BlackPlate_Enable
        , 0
        , 1
        , "ʹ�ܺ����жϹ���"
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
        , CUSTOM_LEVEL
        );

    GetInt("\\Tracker\\ProcessPlate\\BlackPlate"
        , "H1"
        , &Get().cTrackerCfgParam.nProcessPlate_BlackPlateThreshold_H1
        , Get().cTrackerCfgParam.nProcessPlate_BlackPlateThreshold_H1
        , Get().cTrackerCfgParam.nProcessPlate_BlackPlateThreshold_H0
        , 240
        , "����ɫ������"
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
        , CUSTOM_LEVEL
        );
    return S_OK ;
}

HRESULT CSWLPRHvcParameter::InitHvDsp(VOID)
{
    GetEnum("\\HvDsp\\VideoProc\\MainVideo"
        , "OutputBestSnap"
        , &Get().cResultSenderParam.iBestSnapshotOutput
        , Get().cResultSenderParam.iBestSnapshotOutput
        , "0:�����;1:���"
        , "����Ƶ��������ͼ���"
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

    GetEnum("\\HvDsp\\Misc"
        , "OutputObservedFrames"
        , &Get().cResultSenderParam.fOutputObservedFrames
        , Get().cResultSenderParam.fOutputObservedFrames
        , "0:�����;1:���"
        , "�����Ч֡������"
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

    GetString("\\HvDsp\\VideoProc\\HvcCamCfg"
        , "HVCParm_00"
        , Get().cImgFrameParam.rgstrHVCParm[0]
        , Get().cImgFrameParam.rgstrHVCParm[0]
        , sizeof(Get().cImgFrameParam.rgstrHVCParm[0])
        , "ץ��ʶ�����"
        , ""
        , CUSTOM_LEVEL
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
            , "UseRushRule"
            , &Get().cResultSenderParam.iUseRushRule
            , Get().cResultSenderParam.iUseRushRule
            , "0:��ʹ��;1:ʹ��"
            , "�ӷѳ忨���ʹ�ܿ���"
            , ""
            , CUSTOM_LEVEL
        );

        GetInt("\\HvDsp\\Misc"
            , "TimeIn"
            , &Get().cResultSenderParam.iTimeIn
            , Get().cResultSenderParam.iTimeIn
            , 100
            , 2147483647
            , "�����忨�ж���ֵ(��λ����)"
            , ""
            , CUSTOM_LEVEL
        );

        GetInt("\\HvDsp\\Misc"
            , "EnableBackupIO"
            , &Get().cResultSenderParam.iEnableBackupIO
            , Get().cResultSenderParam.iEnableBackupIO
            , 0
            , 1
            , "���ñ���IO"
            , ""
            , PROJECT_LEVEL
        );

        GetInt("\\HvDsp\\Misc"
            , "BarrierNormalState"
            , &Get().cResultSenderParam.iBarrierStatus
            , Get().cResultSenderParam.iBarrierStatus
            , 0
            , 1
            , "���˻�̧���ƽ"
            , ""
            , CUSTOM_LEVEL
        );


    int nLeft = 0;
    int nTop = 0;
    int nRight = 100;
    int nBottom = 100;
    int nMinPlateWidth = 200;
    int nMaxPlateWidth = 300;
    int nVariance = 30;
    char szArea[128] = {0};
    BOOL fChange = FALSE;
    sscanf(Get().cImgFrameParam.rgstrHVCParm[0],
        "[%d,%d,%d,%d],%d,%d,%d",
        &nLeft, &nTop, &nRight, &nBottom,
        &nMinPlateWidth, &nMaxPlateWidth,
        &nVariance
        );
    if ((nMinPlateWidth <= 0 || nMinPlateWidth > 650)
        || (nMaxPlateWidth <= 0 || nMaxPlateWidth > 650)
        || nMinPlateWidth >= nMaxPlateWidth)
    {
        nMinPlateWidth = 200;
        nMaxPlateWidth = 300;
        fChange = TRUE;
    }
    if (!CheckArea(nLeft, nTop, nRight, nBottom))
    {
        nLeft = nTop = 0;
        nRight = nBottom = 100;
        fChange = TRUE;
    }
    if (fChange)
    {
        sprintf(szArea,
            "[%d,%d,%d,%d],%d,%d,%d",
            nLeft, nTop, nRight, nBottom,
            nMinPlateWidth, nMaxPlateWidth,
            nVariance
            );
        memcpy(Get().cImgFrameParam.rgstrHVCParm[0], szArea, strlen(szArea));
    }
    return S_OK ;
}

HRESULT CSWLPRHvcParameter::InitOuterCtrl(VOID)
{
    return S_OK;
}

HRESULT CSWLPRHvcParameter::InitIPTCtrl(VOID)
{
    return S_OK;
}

HRESULT CSWLPRHvcParameter::InitCamApp(VOID)
{
    GetString("\\HvDsp\\Camera\\Ctrl"
        , "Addr"
        , Get().cCamCfgParam.szIP
        , Get().cCamCfgParam.szIP
        , (100 - 1)
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

    /*
    GetEnum("\\HvDsp\\Camera\\Ctrl"
      	, "DynamicCfgEnable"
      	, &Get().cCamCfgParam.iDynamicCfgEnable
      	, Get().cCamCfgParam.iDynamicCfgEnable
        , "0:�ر�;1:��"
        , "��̬���ò���"
        , ""
        , CUSTOM_LEVEL
        );
        */

    GetEnum("\\HvDsp\\Camera\\Ctrl"
    			, "CaptureAutoParamEnable"
    			, &Get().cCamCfgParam.iCaptureAutoParamEnable
    			, Get().cCamCfgParam.iCaptureAutoParamEnable
    			, "0:�ر�;1:��"
    			, "ץ�Ķ��������Զ�����"
    			, "0:�ر�;1:��"
    			, CUSTOM_LEVEL
    			);

    Get().cCamCfgParam.iDynamicCfgEnable = 0;
    		GetInt("\\HvDsp\\Camera\\Ctrl"
    			, "CaptureDayShutterMax"
    			, &Get().cCamCfgParam.iCaptureDayShutterMax
    			, Get().cCamCfgParam.iCaptureDayShutterMax
    			, 400
    			, 3000
    			, "ץ�İ���������"
    			, ""
    			, CUSTOM_LEVEL
    			);
    		GetInt("\\HvDsp\\Camera\\Ctrl"
    			, "CaptureDayGainMax"
    			, &Get().cCamCfgParam.iCaptureDayGainMax
    			, Get().cCamCfgParam.iCaptureDayGainMax
    			, 100
    			, 280
    			, "ץ�İ����������"
    			, ""
    			, CUSTOM_LEVEL
    			);
    		GetInt("\\HvDsp\\Camera\\Ctrl"
    			, "CaptureNightShutterMax"
    			, &Get().cCamCfgParam.iCaptureNightShutterMax
    			, Get().cCamCfgParam.iCaptureNightShutterMax
    			, 400
    			, 3000
    			, "ץ������������"
    			, ""
    			, CUSTOM_LEVEL
    			);
    		GetInt("\\HvDsp\\Camera\\Ctrl"
    			, "CaptureNightGainMax"
    			, &Get().cCamCfgParam.iCaptureNightGainMax
    			, Get().cCamCfgParam.iCaptureNightGainMax
    			, 100
    			, 280
    			, "ץ�������������"
    			, ""
    			, CUSTOM_LEVEL
    			);

    return S_OK ;
}
