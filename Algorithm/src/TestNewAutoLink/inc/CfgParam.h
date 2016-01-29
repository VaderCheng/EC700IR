#pragma once

const int NOT_FIND = 9999;

typedef struct
{
	char szBuildNo[256];
	char szDate[128];
	char szTime[128];
	char szSysName[128];
	char szIPBackUp[16];
	int  nWorkMode;
}SOFT_VERSION_PARAM;

typedef struct _SYS_CONFIG_STRUCT {
	BYTE8 SysTargetID[8];
	BYTE8 SysNameLen;
	BYTE8 SysWorkMode;
	BYTE8 SysRecogFontMode;
	DWORD32 SysResetCount;
	BYTE8 VideoTriggerMode;
	DWORD32 VideoTriggerDelay;
	char SysName[256];
	bool fIsSysInit;
} SYS_CONFIG_STRUCT;

typedef struct _AREA_CTRL_PARAM
{
	int iEnableAreaUserCtrl;
	int iDetectorAreaLeft;
	int iDetectorAreaRight;
	int iDetectorAreaTop;
	int iDetectorAreaBottom;
	int iDetectorMinScaleNum;
	int iDetectorMaxScaleNum;
}AREA_CTRL_PARAM;


typedef struct _TRIGGER_PARAM
{
	//�������뿪��
	int iTriggerInEnable;
	//��������
	int iTriggerType;
	//����ģʽ���Ͷ���
	int iMultiResult;
	//����������ʱ
	int iTriggerInDelay;
	//
	int iTriggerInNoCarFlag;
	//
	int iTriggerOutEnable;
	//
	int iTriggerOutNormalStatus; 
	// 
	int iTriggerOutPlusWidth;


	//�������뿪��
	int iTriggerInEnable_2;
	//��������
	int iTriggerType_2;
	//����ģʽ���Ͷ���
	int iMultiResult_2;
	//����������ʱ
	int iTriggerInDelay_2;
	//
	int iTriggerInNoCarFlag_2;
	//
	int iTriggerOutEnable_2;
	//
	int iTriggerOutNormalStatus_2; 
	// 
	int iTriggerOutPlusWidth_2;
}TRIGGER_PARAM;
#ifndef MAX_HVC_CAMCOUNT
#define MAX_HVC_CAMCOUNT 8
#endif

typedef struct _VIDEO_PROCESS_PARAM
{
	// �Ƿ�ʹ��ȫ��ץ��ģ��.
	int iFullScreenCaptureEnable;

	// ȫ��ץ��ģ��ʹ��ģʽ.
	int iBestSnapshotOutput;
	int iLastSnapshotOutput;
	int iOutputCaptureImage;

	int iFullScreenCaptureImageBeginOutput;
	int iFullScreenCaptureImageBestOutput;
	int iFullScreenCaptureImageLastOutput;

	// ȫ��ץ����ƵԴ�˿�ֵ,֧����Ƶ��0,1,2,���Դ������,����Ϊ�˿�5588(ʮ����).
	int iFullScreenCapturePort;
	// ȫ��ץ��ģ����Ƶ������������.
	int iFullScreenCaptureBuffNum;
	int iFullScreenCaptureCtrlBuffNum;
	int iFullScreenCaptureCtrlBuffMode;
	// ץ��ģ��Դ����ֵ,�Աȶ�,���Ͷ�,ɫ��ֵ.
	int iFullScreenCaptureBrightness;
	int iFullScreenCaptureContrast;
	int iFullScreenCaptureSaturation;
	int iFullScreenCaptureSaturation_U;
	int iFullScreenCaptureSaturation_V;
	int iFullScreenCaptureHue;
	int iFullScreenCaptureMode;

	// ץ��ģ����Ƶͼ���С.
	int iFullScreenCaptureImageWidth;
	int iFullScreenCaptureImageHeight;
	// ץ��ģ����������.
	int iFullScreenCaptureImageFormat;
	int iFullScreenCaptureImageMaxWidth;
	int iFullScreenCaptureImageMaxHeight;

	// ϵͳ��������ƵԴ�˿ں�,֧����Ƶ��0,1,2,���Դ������,����Ϊ�˿�5588(ʮ����).
	int iMainVideoProcessPort;
	// ϵͳ��������ƵԴ����������.
	int iMainVideoProcessBuffNum;
	int iMainVideoProcessCtrlBuffNum;
	int iMainVideoProcessCtrlBuffMode;
	// ϵͳ��������ƵԴ����ֵ,�Աȶ�,���Ͷ�,ɫ��ֵ.
	int iMainVideoProcessBrightness;
	int iMainVideoProcessContrast;
	int iMainVideoProcessSaturation;
	int iMainVideoProcessSaturation_U;
	int iMainVideoProcessSaturation_V;
	int iMainVideoProcessHue;
	int iMainVideoProcessMode;
	// ϵͳ��������Ƶͼ���С.
	int iMainVideoProcessImageWidth;
	int iMainVideoProcessImageHeight;
	// ϵͳ��������Ƶ��������.
	int iMainVideoProcessImageFormat;
	int iMainVideoProcessImageMaxWidth;
	int iMainVideoProcessImageMaxHeight;

	int iVideoDetEnable;
	int iVideoDetLeft;
	int iVideoDetRight;
	int iVideoDetTop;
	int iVideoDetBottom;
	int iVideoDetNum;
	int iVideoDetMode;
	int iVideoDet_00Top;
	int iVideoDet_00Bottom;
	int iVideoDet_00Left;
	int iVideoDet_00Right;
	int iVideoDet_01Top;
	int iVideoDet_01Bottom;
	int iVideoDet_01Left;
	int iVideoDet_01Right;
	int iVideoDet_02Top;
	int iVideoDet_02Bottom;
	int iVideoDet_02Left;
	int iVideoDet_02Right;
	int iEnableDetReverseRun;
	int iSpan;
	int iOutputReverseRun;
	int iSaveType;
	int iWidth;
	int iHeigh;
	int iOutputFrameImage;//��������ͼ�������:0=��,1=֡
	
	int iCapCamType;		// ץ��������� 0:����Jϵ�������1:����1355N
	int iCapCamNum;		// ץ�����������
	int iCapCamDynaCfgEnable;	// ʹ�ܶ�̬�ı�ץ���������
	char szRoadIP_00[20];	// ����0�������IP
	int iSignalType_00;		// ����0���ź�Դ����
	int iDayPlus_00;		// ����0�����������
	int iDayExposure_00;	// ����0��������ع�ʱ��
	int iNightfallPlus_00;	// ����0�����������
	int iNightfallExposure_00;// ����0��������ع�ʱ��
	int iNightPlus_00;		// ����0�����������
	int iNightExposure_00;	// ����0��������ع�ʱ��
	char szRoadIP_01[20];	// ����1�������IP
	int iSignalType_01;		// ����1���ź�Դ����
	int iDayPlus_01;		// ����1�����������
	int iDayExposure_01;	// ����1��������ع�ʱ��
	int iNightfallPlus_01;	// ����1�����������
	int iNightfallExposure_01;// ����1��������ع�ʱ��
	int iNightPlus_01;		// ����1�����������
	int iNightExposure_01;	// ����1��������ع�ʱ��
	char szRoadIP_02[20];	// ����2�������IP
	int iSignalType_02;		// ����2���ź�Դ����
	int iDayPlus_02;		// ����2�����������
	int iDayExposure_02;	// ����2��������ع�ʱ��
	int iNightfallPlus_02;	// ����2�����������
	int iNightfallExposure_02;// ����2��������ع�ʱ��
	int iNightPlus_02;		// ����2�����������
	int iNightExposure_02;	// ����2��������ع�ʱ��
	char szRoadIP_03[20];	// ����3�������IP
	int iSignalType_03;		// ����3���ź�Դ����
	int iDayPlus_03;		// ����3�����������
	int iDayExposure_03;	// ����3��������ع�ʱ��
	int iNightfallPlus_03;	// ����3�����������
	int iNightfallExposure_03;// ����3��������ع�ʱ��
	int iNightPlus_03;		// ����3�����������
	int iNightExposure_03;	// ����3��������ع�ʱ��
	int iTriggerCycle;		// ���紥��ץ����������ڣ�����ֵΪ0ʱ����ʱ����
	int iMovingDirection;   //��Ƶ1��ʻ����
	int iMovingDirection_2; //��Ƶ2��ʻ����

	int iVideoDisplayTime;  //������Ƶʱ����(MS)
	int iSafeSaveEnable;    //�ɿ��Ա��濪��
	int iHDVideoEnable;		//Ӳ��¼�񿪹�
	int iFormatDisk;		//��ʽ��Ӳ��
	int iFDiskType;			//Ӳ�̷�����ʽ
	int iSendRecordSpace;	//������ͼ��
	int iSendVideoTime;		//Ӳ��¼����ʱ����
	int iDrawRect;			//����ͼ�����Ƿ񻭺��
	int iHvcCamNum;			//HVC�������
	char rgszHvcCamIP[MAX_HVC_CAMCOUNT][16];	//HVC���IP����
	char rgszHvcCamParam[MAX_HVC_CAMCOUNT][64];	//HVC�����������

	int iAviVideoHalf;		//AVI¼���߸���һ��
}VIDEO_PROCESS_PARAM;

typedef struct _HVC_PARAM
{
	int iCapCamNum;		// ץ�����������
	char rgCamIP[MAX_HVC_CAMCOUNT][20];	//ץ�����IP
	int rgiSignalNum[MAX_HVC_CAMCOUNT]; 	//��Ӧ���ź�Դ
	char rgHVCParam[MAX_HVC_CAMCOUNT][64];	//�ַ�����ʽ��ץ��ʶ�����
}HVC_PARAM;

typedef struct _RADAR_PARAM
{
	// �״�ʹ�ܿ���
	int fRadarUsed;
	// �״�����
	int iRadarType;
	//IDL4���������ĵĳ���( 0:1,2��Ȧ;  1:3,4��Ȧ ), Ĭ��Ϊ0;
	int iIDL4Road;
	//RAPIER�״����ó��������ʻ����( 0: ͬ��(��β); 1:����(��ͷ) )
	int iRAPIERDirection;
	//RAPIER�״ﰲװ��ˮƽ�Ƕ�
	int iRAPIERAngle;

	// �ò����������Ʊ����˶����������ٶ�����,Ĭ��Ϊ180km/h.
	int iRadarLimitSpeed;
	// �ò�������ָʾ�Ƿ񽫳����ٶ���Ϣ�������λ��,Ĭ��Ϊʹ��.
	int iCarSpeedOutputFlag;

	//�������,ʹ�ò�����ʱ
	BOOL fRadarSpecialControl;

	// �״������ȣ�Ŀǰֻ��Decatur_SI3�״���Ч
	int iSensitivity;
	// �״���ĵ������˶�����,Ŀǰֻ��Decatur_SI3�״���Ч
	// 0-���з���, 1- ʻ������, 2- ʻԶ����
	int iTargetWay;
	// �״ﲨ�Σ�0-Ka, 1-K,Ŀǰֻ��Decatur_SI3�״���Ч
	int iBand;
	// �ٶ�����ϵ�����԰ٷֱȷ�ʽ���ڣ������ֵΪ110��ʱ����ǳ�1.1
	// Ŀǰֻ��Decatur_SI3�״���Ч
	int iCoefficient;

	int iSpeedZeroTimes;
	int iRadarDirectionMode;
	int iRadarWorkMode;
	int iRadarWorkstation;
	int iRadarRateAttentionType;
}RADAR_PARAM;

typedef struct _BLUE_BLACK_PARAM
{
	int iH0;
	int iH1;
	int iS;
	int iL;

	_BLUE_BLACK_PARAM()
	{
		iH0	= 100;
		iH1	= 200;
		iS	= 10;
		iL	= 85;
	}
}BLUE_BLACK_PARAM;

typedef struct _MODE_SET_PARAM
{
	int iWorkMode;
	int iFontMode;

	_MODE_SET_PARAM()
	{
		iWorkMode = 0;
		iFontMode	  = 0;
	}
}MODE_SET_PARAM;

typedef struct _SCALE_SPEED_PARAM
{
	int iScaleSpeedEnable;
	float fCameraHigh;
	float fFullDistance;
	float fLeftDistance;
	float fAdjustCoef;
	int iFlagMethods;
	int iEddyType;
	float fTransMatrix[11];

	_SCALE_SPEED_PARAM()
	{
		iScaleSpeedEnable = 0;
		fCameraHigh = 7.5f;
		fFullDistance = 75.0f;
		fLeftDistance = 25.0f;
		fAdjustCoef = 1.05f;
		iFlagMethods = 0;
		fTransMatrix[0] = 0.4970550537f;
		fTransMatrix[1] = -241.9865264893f;
		fTransMatrix[2] = -30.4937019348f;
		fTransMatrix[3] = 0.5036621094f;
		fTransMatrix[4] = -21.2472610474f;
		fTransMatrix[5] = -6.6555485725f;
		fTransMatrix[6] = -231.5502014160f;
		fTransMatrix[7] = 0.7746582031f;
		fTransMatrix[8] = 0.0300335791f;
		fTransMatrix[9] = -0.0085402671f;
		fTransMatrix[10] = -0.0195075944f;
	}
}SCALE_SPEED_PARAM;


typedef struct _CAM_CFG_PARAM
{
	char szIP[100];	
	int iPort; //����Э�����Ӷ˿�
	int iCamType; // �����Э�����ͣ�0-ר�ò���Э�飬1-Nvc�����Э�飬2-Camyu�����Э��
	int iDynamicCfgEnable;
	int iDynamicTriggerEnable;
	int iTriggerDelay;
	int iDynamicCfgEnableOnTrigger;
	
	// ��������ջ���֡��
	int iJpegBufferCount;

	//�Զ�������(Ŀǰֻ��Camyu�������Ч)
	int iAutoParamEnable;
	float fltDB;				//����
	int iMinExposureTime;
	int iMinPlus;
	int iMaxExposureTime;
	int iMaxPlus;
	int iEnableAGC;
	int iMaxAGCLimit;
	int iMinAGCLimit;

	int iDayBrightness;
	int iDayAcutance;
	int iDaySturation;
	int iDayBlue;
	int iDayRed;
	char szDayScene[10];
	char szDayMode[10];
	int iDayExposureTime;
	int iDayWidth;
	int iDayHeight;
	int iDayAGCLimit;
	int iDayPlus;
	int iDayExposureTimeOnTrigger;
	int iDayPlusOnTrigger;

	int iDay2ExposureTime;
	int iDay2Plus;
	int iDay2Brightness;
	int iDay2AGCLimit;

	int iNightBrightness;
	int iNightAcutance;
	int iNightSturation;
	int iNightBlue;
	int iNightRed;
	char szNightScene[10];
	char szNightMode[10];
	int iNightExposureTime;
	int iNightWidth;
	int iNightHeight;
	int iNightAGCLimit;
	int iNightPlus;
	int iNightExposureTimeOnTrigger;
	int iNightPlusOnTrigger;

	int iNight2ExposureTime;
	int iNight2Plus;
	int iNight2Brightness;
	int iNight2AGCLimit;

	int iSuitlightBrightness;
	int iSuitlightAcutance;
	int iSuitlightSturation;
	int iSuitlightBlue;
	int iSuitlightRed;
	char szSuitlightScene[10];
	char szSuitlightMode[10];
	int iSuitlightExposureTime;
	int iSuitlightWidth;
	int iSuitlightHeight;
	int iSuitlightAGCLimit;
	int iSuitlightPlus;
	int iSuitlightExposureTimeOnTrigger;
	int iSuitlightPlusOnTrigger;

	int iSuitlight2ExposureTime;
	int iSuitlight2Plus;
	int iSuitlight2Brightness;
	int iSuitlight2AGCLimit;

	int iBacklightBrightness;
	int iBacklightAcutance;
	int iBacklightSturation;
	int iBacklightBlue;
	int iBacklightRed;
	char szBacklightScene[10];
	char szBacklightMode[10];
	int iBacklightExposureTime;
	int iBacklightWidth;
	int iBacklightHeight;
	int iBacklightAGCLimit;
	int iBacklightPlus;
	int iBacklightExposureTimeOnTrigger;
	int iBacklightPlusOnTrigger;

	int iBacklight2ExposureTime;
	int iBacklight2Plus;
	int iBacklight2Brightness;
	int iBacklight2AGCLimit;

	int iNightfallBrightness;
	int iNightfallAcutance;
	int iNightfallSturation;
	int iNightfallBlue;
	int iNightfallRed;
	char szNightfallScene[10];
	char szNightfallMode[10];
	int iNightfallExposureTime;
	int iNightfallWidth;
	int iNightfallHeight;
	int iNightfallAGCLimit;
	int iNightfallPlus;
	int iNightfallExposureTimeOnTrigger;
	int iNightfallPlusOnTrigger;

	int iNightfall2ExposureTime;
	int iNightfall2Plus;
	int iNightfall2Brightness;
	int iNightfall2AGCLimit;

	int iBigSuitlightBrightness;
	int iBigSuitlightAcutance;
	int iBigSuitlightSturation;
	int iBigSuitlightBlue;
	int iBigSuitlightRed;
	char szBigSuitlightScene[10];
	char szBigSuitlightMode[10];
	int iBigSuitlightExposureTime;
	int iBigSuitlightWidth;
	int iBigSuitlightHeight;
	int iBigSuitlightAGCLimit;
	int iBigSuitlightPlus;
	int iBigSuitlightExposureTimeOnTrigger;
	int iBigSuitlightPlusOnTrigger;

	int iBigSuitlight2ExposureTime;
	int iBigSuitlight2Plus;
	int iBigSuitlight2Brightness;
	int iBigSuitlight2AGCLimit;

	int iBigBacklightBrightness;
	int iBigBacklightAcutance;
	int iBigBacklightSturation;
	int iBigBacklightBlue;
	int iBigBacklightRed;
	char szBigBacklightScene[10];
	char szBigBacklightMode[10];
	int iBigBacklightExposureTime;
	int iBigBacklightWidth;
	int iBigBacklightHeight;
	int iBigBacklightAGCLimit;
	int iBigBacklightPlus;
	int iBigBacklightExposureTimeOnTrigger;
	int iBigBacklightPlusOnTrigger;

	int iBigBacklight2ExposureTime;
	int iBigBacklight2Plus;
	int iBigBacklight2Brightness;
	int iBigBacklight2AGCLimit;
}CAM_CFG_PARAM;

//HvDsp����
typedef struct _HVDSP_PARAM 
{	
	BOOL fSysInitialized;

	BOOL fFirstSendPlate;

	BOOL fOutputReverseRun;

	BOOL fEnableDetReverseRun;
	BOOL fOutputAppendInfo;
	BOOL fOutputCarSize;		// ��������ߴ�

	BOOL fOutputObservedFrames;

	BOOL fOutputCarArriveTime;

	BOOL fOutputVoteCondition;

	BOOL fOutputVideoFrameName;//��Ƶ֡���������

	int iBigImageCompressionRadio;

	int iVideoCompressionRadio;

	int fOutputFrameImage; 

	//������г���
	int iOutputReverseRun;

	//�Ƿ���ʶ���������ʾ��Ƶ.
	int iVideoTracker;		
	//�Ӷ���֡����ʾһ����Ƶͼ��.	
	int iVideoDisplayNum;
	// ���ٺ��뷢��һ֡ͼ��
	int iVideoDisplayTime;
	// ����ͼ�����Ƿ񻭺��
	int iDrawRect;

	int iComTerminalOpen;

	//���괮��Э��ʹ��
	int iStdCommFlag;
	//���괮��Э��ʹ�õĴ��ں�
	int iStdCommNumber;
	//���괮��Э���Ƿ��ʹ�ͼ
	int iStdCommSendBigImageFlag;
	//���괮��Э���Ƿ��Ͷ�ֵͼ
	int iStdCommSendBinaryImageFlag;
	//���괮��Э���Ƿ���Сͼ
	int iStdCommSendSmallImageFlag;

	BOOL fDoLeach;
	BOOL fDoReplace;

	int nExportParamRank;
	int nExportRankInfo;

	BOOL fLoopReplace;

	// �������
	int iLightBlueFlag;
	int iRecogGxPolice;
	int iOutputBrigtness;
	int iOutputFilterInfo;

	// ˫�廥��ģʽ
	int iHostType;

	//GPS
	int iGPSEnable;			//ʹ�ܿ���
	int iGPSSyncSpaceS;		//Уʱ��ʱ����(��λ��)
	
	//////////////////////////////////////////////////////////////////////////
	//�Ϻ��������в���
	// ����ʶ�����ɣ�
	int iRecoDevCount;
	char rgszRecoDevAddr[8][20];
	// �Ƿ������ͬ����
	int iTwinFilterFlag;
	int iTwinsTime;
	int iMaxPlateWait;
	char szSpecialFiltrateChar[20];

	int iOutputOnlyPeccancy;
	int iSaveVideoNum;

	//���̵�ͼƬ��ǿ����
	int iImgTransEnable;
	int iTransBrightness;
	int iTransHueThreshold;
	int iTransCompressRate;

	//ͼƬ��ת����
	int iEddyType;
	//���˲��ϸ�Υ�½��
	int iFilterUnSurePeccancy;

	int iSendVideoTime;

	int iSendRecordSpace;

	//���ͺ�����껷��������ֵ
	//�������ȵ��ڴ�ֵ�ŷ��ͺ������
	int iRedPosBrightness;

	//�������Ӳ���
	int iAutoLinkEnable;
	char szAutoLinkIp[32];
	int iAutoLinkPort;

	// �¼�������ڣ���λ������
	int iCheckEventTime; 
	int iEventOutputType; //�Ƿ�ʵʱ����¼�����ź�

	//����Э�����������
	int iTestServerEnable;

	//�ٶ�����
	int iSpeedLimit;
	
	//������Ʋ�����쳣��ʾ
	int iOutputFillLightStatus;

	//���ץ�Ĳ�����쳣��ʾ
	int iOutputFlashLampStatus;

	//����������Сֵ,С�����ֵ���Ʋ���ƿ����쳣
	int iPlateLightMin;

	//ץ��ͼ��ʶ��ͼ���Ȳ�ֵ��С�����ֵץ�Ĳ���ƿ����쳣
	int iFlashLampDiff;

}HVDSP_PARAM;

const int MAX_ROAD_COUNT = 5;
typedef struct _OUTERCONTROL_PARAM
{
	int iEnable;
	int iPlateHoldMs;
	int iSignalHoldMs;
//	int iTackType;

	int iSignalTypeNumber;
	int iSignalSourceNumber;

	int iRoadNumber;
	int rgiRoadTopX[MAX_ROAD_COUNT];
	int rgiRoadTopY[MAX_ROAD_COUNT];
	int rgiRoadBottomX[MAX_ROAD_COUNT];
	int rgiRoadBottomY[MAX_ROAD_COUNT];

	int rgiSignalType[20];
	int iMainSignalType;

	int rgiSignalSoureMatchType[20];
	int rgiSignalSoureRoad[20];
	int rgiSignalSourePrevMs[20];
	int rgiSignalSourePostMs[20];
}OUTERCONTROL_PARAM;

typedef struct _IPT_PARAM
{
	//����
	char szIPTCom[20];
	int rgiComSignalSource[4];
	int rgiOutSigSource[4];
	int rgiOutSigLevel[4];
	int rgiOutSigWidth[4];
	int rgiPortSignalSource0[4];
	int rgiPortSignalSource1[4];
	//����
	int iCtrlTriggerSignalSource;
	int iNetTriggerSignalSource;
	int iCtrlSpeedSignalSource;

	//IPT���ò���
	BOOL fIPTPulse;             //�ж�IPT�������Ⱥ͵�ƽ�����������Ƿ����
	BOOL rgfDeviceUsed[4];	
	//IO���������
	int iIoinType;				//IO�ڵ�����
	int iIoinDelayTime;			//IO�ڵ�����������ʱ
	int iIoinEffective;		//IO����Ч����λ
	//��Ȧ���ٲ���
	int iCoilType;				//��Ȧ��������
	int iCoilDistance;			//ǰ����Ȧ�ľ���
	int iCoilLimitSpeed;		//��Ȧ���ٵ���С�����ٶ�
	int iCoilCofSpeed;			//��Ȧ���ٵ��ٶ�ϵ��
	int iCoilDelayTime;			//��Ȧ���ٵ�����������ʱ
	int iCoilTimeScale;			//��Ȧ���ٵ�ʱ��ʱ��
	//IO���������
	int iIooutControlType;		//������Ʒ�ʽ
	int iIooutEffective;		//��Ч����λ
	int iIooutDelayTime;		//����������ʱ
	int iIooutControlSource;	//IO�ź�Դ����λ������Դ
	int iIooutSignalSource;		//IO����Դ
	int iIooutOutDelay;			//�����ʱ
	int iIooutPolar;			//IO�����ź�Դ����
	int iIooutInitLevel;		//�����ʼ����ƽ
	int iIooutInput;			//IO���뷴������
	//�״���ٲ���
	int iRadarType;				//�״��������
	int iRadarMeasureTime;		//����ʱ����
	int iRadarCofSpeed;			//�ٶ�ϵ��
	int iRadarLimitSpeed;		//��С��������
	int iRadarCarDirection;		//�����˶�����
}IPT_PARAM;

typedef struct _OTHER_SIGNAL_SOURCE_PARAM
{
	// �����ź�Դ���
	int iSoftTriggerSignalSource;
}OTHER_SIGNAL_SOURCE_PARAM;

typedef struct _VIDEO_DETECTOR_PARAM
{
	int iRoadNumber;
	float ftRoadWidth;	
	int rgiGapLine_Top_X[6];
	int rgiGapLine_Bottom_X[6];
	int rgiGapLine_Top_Y[6];
	int rgiGapLine_Bottom_Y[6];

	int iVideoMaskNumber;
	int rgiVideoMask_Left[4];
	int rgiVideoMask_Right[4];
	int rgiVideoMask_Top[4];
	int rgiVideoMask_Bottom[4];

	int iPulseType;
	int iPulseWidth;
	int iOutputSignalLow;
	int iOutputSignalHight;

}VIDEO_DETECTOR_PARAM;

#ifndef MAX_LIGHT_COUNT
#define MAX_LIGHT_COUNT 10
#endif
#ifndef  MAX_SCENE_COUNT
#define MAX_SCENE_COUNT 10
#endif

typedef struct _TRAFFICLIGHT_PARAM
{
	int iEnable;
	int nLightCount;
	int nValidSceneStatusCount;
	int iStopLine;
	int iLeftStopLine;
	int iOnePos;
	int iTwoPos;
	int iThreePos;
	int iCheckType;
	char szIOConfig[64];
	char rgszLight[MAX_LIGHT_COUNT][64];
	int nSceneCount;
	char rgszScene[MAX_SCENE_COUNT][64];	
}TRAFFICLIGHT_PARAM;
typedef struct _ROADINFO_PARAM
{
	int iRoadNum;
	int iRoadNumBegin;			//���������ʼ��0������ʼ��1�����ҿ�ʼ
	int iStartRoadNum;			//��ʼ������,0:��0��ʼ,1:��1��ʼ
	int rgiTop_X[5];
	int rgiTop_Y[5];
	int rgiBottom_X[5];
	int rgiBottom_Y[5];
	int rgiRoadType[5];
}ROADINFO_PARAM;

//#ifndef MAX_IDENTIFY_PASSWORD_LENGTH
//#define MAX_IDENTIFY_PASSWORD_LENGTH 256
//#endif
#ifndef MAX_STREETINFO_LENGTH
#define MAX_STREETINFO_LENGTH 255
#endif
typedef struct _IDENTIFYPASSWORD_PARAM
{
	int iPasswordLength;
	char rgszPassword[MAX_IDENTIFY_PASSWORD_LENGTH];
	char rgszStreetName[MAX_STREETINFO_LENGTH];
	char rgszStreetDirection[MAX_STREETINFO_LENGTH];
}IDENTIFYPASSWORD_PARAM;

typedef struct _ACTIONDETECT_PARAM
{
	int iDetectCarStopEnable;
	int iDetectReverseEnable;
	int iOutputReverseEnable;
	int iDetectOverYellowLineEnable;
	int iIsYellowLine[5];		//0-01������1-12�������Դ�����
	int iDetectCrossLineEnable;
	int iIsCrossLine[5];		//0-01������1-12�������Դ�����
	float ftOverLineSensitivity;   //ѹ�߼��������
}ACTIONDETECT_PARAM;


