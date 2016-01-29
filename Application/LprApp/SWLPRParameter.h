#ifndef __SW_LPR_PARAMETER_H__
#define __SW_LPR_PARAMETER_H__
#include "trackerdef.h"
#include "SWClassFactory.h"
#include "ProcessRule.h"
#include "AutoLinkParam.h"
#include "SWMatchTansformDataStruct.h"
#include "SWFilterStruct.h"
#include "SWFtpParameter.h"
#include "SWDomeRockerParameter.h"
#include "SWOnvifRtspParameter.h"

#define MAX_CAP_CAM_COUNT      4
#define MAX_CAM_COUNT          8 
#define MAX_STREETINFO_LENGTH  255
#define MAX_LEVEL_COUNT        14
#define SVR_PORT               8888

/**
* @brief ץ�����ģ������ṹ��
*/
typedef struct _tagCapCamParam
{
	INT nCamType;   //ץ���������(0-����Jϵ�����)
	INT nCamNum;    //ץ���������
	INT rgnSignalType[MAX_CAP_CAM_COUNT];   //ץ������ź�Դ����
	CHAR rgszRoadIP[MAX_CAP_CAM_COUNT][20]; //ץ�����IP

	_tagCapCamParam()
	{
		nCamType = 0;
		nCamNum = 0;
		for (INT i = 0; i < MAX_CAP_CAM_COUNT; i++)
		{
			rgnSignalType[i] = -1;
			sprintf(rgszRoadIP[i], "172.18.10.10%d", i);
		}
	}
}CAP_CAM_PARAM;

/**
* @brief �������ģ������ṹ��
*/
typedef struct _ResultSenderParam
{
	//�������ģ�����ز���
	PROCESS_RULE cProcRule;     /**< ������� */

	BOOL fInitHdd;              /**< �Ƿ��ʼ��Ӳ�� */
	
	INT  iDiskType;				        /**< ����Ӳ������ */
	

	CHAR szNetDiskIP[32];               /**< ����洢��IP��ַ */
	//CHAR szNetDiskUser[32];             /**< ����洢���û����� */
	//CHAR szNetDiskPwd[32];		        /**< ����洢���û����� */
	INT  iNetDiskSpace;   				/**< ����洢������,��λG */
	CHAR szNFS[32];                     /**< ����Ӳ��NFS·�� */
	CHAR szNFSParam[255];               /**< NFSЭ��Ĺ��ز���*/
	//CHAR szFtpRoot[32];                 /**< ����洢��FTP��·�� */

	
	//�ɿ��Ա���ģ��
	BOOL fIsSafeSaver;            /**< �Ƿ�������ȫ�洢 */
	CHAR szSafeSaverInitStr[64];  /**< ��ȫ��������ʼ������ */  //��ʱ���ã�Ԥ��
	INT  iRecordFreeSpacePerDisk;
	INT  iVideoFreeSpacePerDisk;
	INT  iSaveSafeType;                 /**< ��̬Ӳ�̴洢��ʽ 0:û�пͻ�������ʱ�Ŵ洢 1:һֱ�洢 */
	INT  iFileSize;                     /**< �����洢�е����ļ��Ĵ�С*/

	//������Ƶ��ز���
	BOOL fSaveVideo;         /**< ������Ƶ���� */
	INT  iSaveVideoType;                 /**< ��Ƶ�洢��ʽ 0:һֱ�洢 1:��Υ�½��ʱ�Ŵ洢 */
	INT iVideoDisplayTime;  /**< ������Ƶʱ���� */
	INT iDrawRect;          /**< ���ͺ���ʶ */
	INT iEddyType;          /**< ͼƬ��ת */

	//������Ϣģ�����
	BOOL fOutputAppendInfo;     //������Ƹ�����Ϣ����
	BOOL fOutputObservedFrames; //�����Ч֡������
	BOOL fOutputCarArriveTime;  //������ʱ�俪��
	BOOL fOutputFilterInfo; //���������Ϣ
	CHAR szStreetName[MAX_STREETINFO_LENGTH];       //·������
	CHAR szStreetDirection[MAX_STREETINFO_LENGTH];  //·�ڷ���

	INT iOutputOnlyPeccancy;//1:ֻ���Υ�³��� 2:ֻ�����Υ�½�� 3:������ʷ����ʱ,��Υ������ı�,Υ������ı���ͼƬ
	INT iSendRecordSpace; //������ͼ��(*100ms)
	INT iSendHisVideoSpace;//��ʷ¼���ͼ��(*100ms)

	INT iSpeedLimit;    //�ٶ�����ֵ(km/h)

	INT iCheckEventTime;    //�¼��������(��λ:����)

	INT iSaveType;    //������淽ʽ
	INT iWidth;         //��ͼ���
	INT iHeight;        //��ͼ�߶�
	INT iBestSnapshotOutput;    //����Ƶ��������ͼ���
	INT iLastSnapshotOutput;    //����Ƶ����ͼ���
	INT iOutputCaptureImage;    //��Υ�³����������ץ��ͼ
    INT iLastSnapshotCropOutput;    //����Ƶ����ͼ��ͼ���
    INT iCropWidthLevel;
    INT iCropHeightLevel;

	INT iFilterUnSurePeccancy;  //���˲��ϸ��Υ�¼�¼
	INT nOutputPeccancyType;    //���Υ�����ͷ�ʽ
	CHAR szPeccancyPriority[256]; //Υ���������ȼ�

	float fltCarAGCLinkage;  //��������AGC�������أ�0Ϊ�أ���0��ʾ��λʱ�䳵�������ڸ�ֵʱ����AGC��������ر�
	INT iAWBLinkage;        //����������AWB��������
	INT nRedPosBrightness;

	INT iEnablePanoramicCamera;     //ȫ�����ʹ�ܿ���
	CHAR szPanoramicCameraIP[20];   //ȫ�����IP

	CAP_CAM_PARAM cCapCamParam; //ץ���������
	AUTO_LINK_PARAM cAutoLinkParam; //�������Ӳ���
	FTP_PARAM cFtpParam;			//FTP���Ӳ���
	

	BOOL fMTUSetEnable;

	INT iCapMode;             //ץ��ģʽ (0:ץ��һ��; 1:����Υ��ץ��2��; 2:ȫ��ץ��2��)
	INT iFlashDifferentLane;  //����Ʒֳ�����
    INT iFlashOneByOne;         // �������������

	BOOL fEnableTriggerOut; // �������ʹ�ܿ���
	int nTriggerOutNormalStatus; // ���������̬����(����)
	int nTriggerOutPlusWidth; // �����������

	BOOL fEnableNtp;     //NTPʹ�ܿ���
	CHAR szNtpServerIP[20];   //NTP������IP
	INT iNtpSyncInterval;     //NTPͬ��ʱ����,��λsec
	INT iTimeZone;			//ʱ����Ϣ

	INT iUseRushRule;	//ʹ�ó�˹��򿪹�
	INT iEnableBackupIO;	//���ñ���IO
	INT iTimeIn;	//�źż��ʱ��
	INT iBarrierStatus;	//���˻�̧���ƽ

	_ResultSenderParam()
	{
		iBestSnapshotOutput = 1;
		iLastSnapshotOutput = 1;
		iOutputCaptureImage = 0;
        iLastSnapshotCropOutput = 1;
        iCropWidthLevel = 6;
        iCropHeightLevel = 4;
		iRecordFreeSpacePerDisk = 10;
		iVideoFreeSpacePerDisk = 10;
		iVideoDisplayTime = 500;
		iDrawRect = 0;
		iEddyType = 0;
		iSpeedLimit = 1000;
		iSaveType = 0;
		iWidth = 720;
		iHeight = 288;
		iCheckEventTime = 1;
		iFilterUnSurePeccancy = 0;
		fOutputAppendInfo = TRUE;
		fOutputObservedFrames = TRUE;
		fOutputCarArriveTime = FALSE;
		swpa_strcpy(szSafeSaverInitStr, "NULL");
		swpa_strcpy(szStreetName, "NULL");
		swpa_strcpy(szStreetDirection, "NULL");
		iOutputOnlyPeccancy = 0;
		iSendRecordSpace = 5;
		iSendHisVideoSpace = 3;
		nOutputPeccancyType = 0;
		fltCarAGCLinkage = 0.0f;
		iAWBLinkage = 0;
		fOutputFilterInfo = 0;
		fInitHdd = FALSE;
		iFileSize = 512;		
		iDiskType = 2;
		fIsSafeSaver = FALSE;
		iSaveSafeType = 1;
		fSaveVideo = FALSE;
		iSaveVideoType = 0;
		fMTUSetEnable = FALSE;
		swpa_strcpy(szPeccancyPriority, "����-�ǻ�������-�����-����������ʻ-ѹ��-����");
		iCapMode = 0;
        iFlashDifferentLane = 0;
        iFlashOneByOne = 1;

		fEnableTriggerOut = 1;
		nTriggerOutNormalStatus = 1;
		nTriggerOutPlusWidth = 5000;

		fEnableNtp = 0;
		iNtpSyncInterval = 3600; // 1Сʱ
		iTimeZone = 20;		//Ĭ�϶�8����GMT+8,0ΪGMT-12
		swpa_memset(szNtpServerIP, 0, sizeof(szNtpServerIP));
		swpa_strcpy(szNtpServerIP, "210.72.145.44");

		swpa_strcpy(szNetDiskIP, "172.18.10.10");
		//strcpy(szFtpRoot, "/array1");
		//strcpy(szNetDiskUser, "No User");
		//strcpy(szNetDiskPwd, "123");
		swpa_strcpy(szNFS, "/volume1/nfs");
		swpa_strcpy(szNFSParam, "-o nolock,tcp");
		iNetDiskSpace = 1;

		iUseRushRule = 0;
		iEnableBackupIO = 0;
		iTimeIn = 1500;
		iBarrierStatus = 0;
	};

} ResultSenderParam;

/**
*@brief �������ýṹ��
*/
typedef struct _TcpipParam
{
	CHAR szIp[32];
	CHAR szNetmask[32];
	CHAR szGateway[32];

	_TcpipParam()
	{
		sprintf(szIp, "100.100.100.101");
		sprintf(szNetmask, "255.0.0.0");
		sprintf(szGateway, "100.100.1.1");
	}
} TcpipParam;

/**
* @brief ǰ��ͼ��ɼ�ģ������ṹ��
*/
typedef struct _tagImgFrameParam
{
	INT iCamNum;                            //�������
	CHAR rgstrCamIP[MAX_CAM_COUNT][32];     //ץ�����IP
	CHAR rgstrHVCParm[MAX_CAM_COUNT][128];  //ץ��ʶ�����

	float fltCaptureFrameRate; //�ɼ�֡��
	INT iOutputFrameRate;   // ���֡�ʣ�1��15��

	_tagImgFrameParam()
	{
		iCamNum = 1;
		for ( INT i=0; i<MAX_CAM_COUNT; ++i )
		{
			memset(rgstrCamIP[i], 0, 32);
			memset(rgstrHVCParm[i], 0, 128);
		}
		fltCaptureFrameRate = 12.5;
		iOutputFrameRate = 15;
	};

} IMG_FRAME_PARAM;

/**
* @brief ǰ����������ṹ��
*/
typedef struct _tagCamCfgParam
{
	CHAR szIP[100];
	INT iCamType; // �����Э�����ͣ�0-һ�廯���Э�飬1-ר�ò���Э�飬2-Camyu�����Э��
	INT iCamPort; //����Э��˿�
	INT iDynamicCfgEnable;
	INT iDynamicTriggerEnable;
	INT iTriggerDelay;
	INT iTriggerPlateType; //���ڳ�βģʽ�£���Щ����������Ҫ����ץ�ģ���ͨ�����󣬻ƹ������ӣ�2012-01-04
	INT iDynamicCfgEnableOnTrigger;
	INT iNightShutterThreshold;
    INT iNightAvgYThreshold;        // ����������ֵ
    INT iDuskAvgYThreshold;         // ����������ֵ
    //INT iWorkType;	//��װ��װ���ͣ�0Ϊ��װ(����쳵)��1�ǲ�װ(�״�쳵)

	// ��������ջ���֡��
	INT iJpegBufferCount;
	INT nTestProtocolPort;

	//�Զ�������
	INT iAutoParamEnable;
	float fltDB;				//����
	INT iMinExposureTime;
	INT iMinGain;
	INT iMaxExposureTime;
	INT iMaxGain;
	INT iEnableAGC;
	INT iMaxAGCLimit;
	INT iMinAGCLimit;
	INT iMaxPSD;
	INT iMinPSD;
	INT iMinPlateLight;
	INT iMaxPlateLight;

	INT irgExposureTime[MAX_LEVEL_COUNT];
	INT irgAGCLimit[MAX_LEVEL_COUNT];
	INT irgGain[MAX_LEVEL_COUNT];

	INT iCaptureAutoParamEnable;		/**< �Զ�ץ�Ĳ�������ʹ��*/
	INT iCaptureDayShutterMax;
	INT iCaptureDayGainMax;
	INT iCaptureNightShutterMax;
	INT iCaptureNightGainMax;

	INT iIPNCLogOutput;

	_tagCamCfgParam()
	{
		swpa_sprintf(szIP, "172.18.23.42");     
		iCamType = 0;
		iCamPort = 8888;
		iDynamicCfgEnable = 0;
		iDynamicTriggerEnable =0;
		iTriggerDelay = 0;
		iTriggerPlateType = 0;
		iDynamicCfgEnableOnTrigger = 0;
		iNightShutterThreshold = 50000;// �˲���������ʹ�ã���˽�Ĭ��ֵ�������
		iNightAvgYThreshold = 0;
        iDuskAvgYThreshold = 50;

		//iWorkType=0;
		
		iJpegBufferCount = 1;
		iEnableAGC = 0;
		iAutoParamEnable = 0;
		fltDB = 3.0;
		iMinExposureTime  = 100;
		iMinGain  = 70;
		iMaxExposureTime  = 6000;
		iMaxGain  = 220;
		iMaxAGCLimit = 140;
		iMinAGCLimit = 10;
		iMaxPSD = 4200;
		iMinPSD = 1000;
		iMinPlateLight = 140;
		iMaxPlateLight = 160;
		nTestProtocolPort = SVR_PORT;

		iIPNCLogOutput = 0;
			
		INT irgExposureTimeTemp[MAX_LEVEL_COUNT] =
		{
			8, 8, 8, 8, 12, 17, 24, 34, 48, 68, 96, 96, 96, 96
		};

		for (INT i = 0; i < MAX_LEVEL_COUNT; i++)
		{
			irgExposureTime[i] = irgExposureTimeTemp[i];
			irgAGCLimit[i] = -1;
			irgGain[i] = -1;
		}

		iCaptureAutoParamEnable = 0;
		iCaptureDayShutterMax = 1500;
		iCaptureDayGainMax	= 180;
		iCaptureNightShutterMax = 2000;
		iCaptureNightGainMax = 180;

	}
}CAM_CFG_PARAM;

//368�˽ṹ��
typedef struct _CamAppParam
{
	INT iJpegStream;        /**< �Ƿ���Jpeg����0:��1���ǡ� */
	INT iH264Stream;        /**< �Ƿ���H.264����0:��1���ǡ� */

	INT iIFrameInterval;    /**< H.264�����е�I֡��� */
	INT iTargetBitRate;     /**< H.264���������� */
	INT iMaxBitRate;		/**< H.264������� */
	INT iFrameRate;         /**< H.264����֡�� */
	INT iResolution;        /**< H.264ͼ��ֱ��� */
	INT iRateControl;		/**< H.264���ʿ���*/
	INT iVbrDuration;		/**< H.264�жϳ����仯�Ĳ���ʱ��*/
	INT iVbrSensivitity;	/**< H.264�жϳ����仯��������*/

	INT iIFrameIntervalSecond;    /**< �ڶ�·H.264�����е�I֡��� */
	INT iTargetBitRateSecond;     /**< �ڶ�·H.264���������� */
	INT iMaxBitRateSecond;		  /**< �ڶ�·H.264������� */
	INT iFrameRateSecond;         /**< �ڶ�·H.264����֡�� */
	INT iResolutionSecond;        /**< �ڶ�·H.264ͼ��ֱ��� */
	INT iRateControlSecond;
	INT iVbrDurationSecond;
	INT iVbrSensivititySecond;

	INT iAGCTh;             /**< AGC�������� */
	INT iAGCShutterLOri;    /**< AGC���ŵ������� */
	INT iAGCShutterHOri;    /**< AGC���ŵ������� */
	INT iAGCGainLOri;       /**< AGC����������� */
	INT iAGCGainHOri;       /**< AGC����������� */
	INT iAGCScene;			/**< AGC�龰ģʽ */
    INT iAGCDayNightShutterControl;     /**< AGC���졢���϶����������޿��� */
    INT iAGCNightShutterHOri;    /**< AGC���ŵ������� */

	INT iGainR;             /**< R���� */
	INT iGainG;             /**< G���� */
	INT iGainB;             /**< B���� */
	INT iGain;              /**< ������� */
	INT iShutter;           /**< ����ʱ�� */

	INT iCaptureGainR;      /**< ץ��R���� */
	INT iCaptureGainG;      /**< ץ��G���� */
    INT iCaptureGainB;      /**< ץ��B���� */
	INT iCaptureGain;       /**< ץ��������� */
	INT iCaptureShutter;    /**< ץ�Ŀ���ʱ�� */
	INT iCaptureSharpenThreshold;       /**< ץ�����ֵ */

	INT iCaptureAutoParamEnable;		/**< �Զ�ץ�Ĳ�������ʹ��*/
	INT iEnableCaptureGainRGB;          /**< ʹ��ץ��RGB��ɫ���� */
	INT iEnableCaptureGain;             /**< ʹ��ץ������ */
	INT iEnableCaptureShutter;          /**< ʹ��ץ�Ŀ��� */
	INT iEnableCaptureSharpen;          /**< ʹ��ץ����*/

	INT iJpegCompressRate;              /**< Jpegѹ���� */
	INT iJpegCompressRateCapture;       /**< ץ��Jpegͼѹ���� */

	CHAR szNFSParam[255];               /**< NFSЭ��Ĺ��ز���*/
	CHAR szSafeSaverInitStr[64];        /**< ��ȫ��������ʼ������ */

	//    BOOL fIsSideInstall;                /**< ǰ�˾�ͷ�Ƿ�Ϊ��װ */
	INT iJpegExpectSize;                /**< JpegͼƬ������С����λ��Byte����ͨ���Զ�����Jpegѹ����ʵ�� */
	INT iJpegCompressRateL;             /**< Jpegѹ�����Զ��������� */
	INT iJpegCompressRateH;             /**< Jpegѹ�����Զ��������� */

	INT iJpegType;                      /**< JpegͼƬ��ʽ */
	//    INT iOutputFrameRate;               /**< ������֡�� */

	INT iAGCEnable;                     /**< AGCʹ�� */
	INT iAWBEnable;                     /**< AWBʹ�� */
	INT iFlashRateSynSignalEnable;      /**< Ƶ��ͬ���ź�ʹ�� */
	INT iFlashRateMultiplication;		/**< Ƶ������Ƿ�Ƶ��1����Ƶ��� */
	INT iFlashRatePolarity;				/**< Ƶ��������ԡ�1������0����*/
	INT iFlashRatePulseWidth;			/**< Ƶ���������100 �� 20000. ΢��*/
	INT iFlashRateOutputType;			/**< Ƶ��������͡�1����������ƽ��, 0:OC(����) */
	INT iCaptureSynSignalEnable;        /**< ץ��ͬ���ź�ʹ�� */
	INT iCaptureSynPolarity;			/**< ץ��������ԡ�1������0����*/
	INT iCaptureSynPulseWidth;			/**< ץ���������100 �� 20000. ΢��*/
	INT iCaptureSynOutputType;			/**< ץ��������͡�1����������ƽ��, 0:OC(����) */
	INT iCaptureSynCoupling;			/**< Ƶ����ץ���Ƿ���ϡ� 1����ϣ�0������ϡ�*/
	//    INT iENetSyn;                       /**< ����ͬ�� 0:15fps 1:12.5fps */
	INT iCaptureEdge;                   /**< ץ�Ĵ����� 0:�ⲿ�½��ش��� 1:�ⲿ�����ش��� 2: �������½��ض����� 3�������� */
	INT iFlashDifferentLane;            /**< ����Ʒֳ����� 0:���ֳ��� 1: �ֳ��� */
	INT iFlashDifferentLaneExt;         /**< ����Ʒֳ�����(Ӳ����) 0:���ֳ��� 1: �ֳ��� */

	//CHAR szNetDiskIP[32];               /**< ����洢��IP��ַ */
	CHAR szFtpRoot[32];                 /**< ����洢��FTP��·�� */
	CHAR szNetDiskUser[32];             /**< ����洢���û����� */
	CHAR szNetDiskPwd[32];		        /**< ����洢���û����� */
	//INT  iNetDiskSpace;   				/**< ����洢������,��λG */
	//CHAR szNFS[32];                     /**< ����Ӳ��NFS·�� */
	INT  iAvgY;                         /**< ����������ֵ*/

	INT iGammaMode;						 /**< Gama ͨ��ѡ��ֵ��0��6467��368����GAMMA��1��6467��368�ޣ�2��6467��368�У�3��6467��368��*/
	INT  rgiDefGamma[8][2];             /**< Ĭ��Gamaֵ*/
	INT  rgiGamma[8][2];
	INT rgiAGCZone[16];

	INT iX;                             /**< �ַ���X���� */
	INT iY;                             /**< �ַ���y���� */
	INT iFontSize;                      /**< �����С(ע���˴�СΪ��������������Ϊ24*24�������ֵΪ24) */
	INT iDateFormat;                    /**< ���ڸ�ʽ */
	INT iFontColor;                     /**< ������ɫ(RGB) */
	CHAR szCharater[255];               /**< ���󻺳���*/
	INT iEnableCharacterOverlap;        /**< �ַ�����ʹ�� */
	INT iEnableFixedLight;              /**< ������ɫ�Ƿ�̶����� */

	// A67
	INT iEnableSharpen;                 /**< ʹ���񻯿��� 0: ��ʹ��  1��ʹ�� */
	INT iSharpenThreshold;              /**< �񻯷�ֵ[0~100] */
	INT iSharpenCapture;                /**< ��ץ��ͼ��1����ץ��ͼ 0��������ͼ��*/
	INT iContrastValue;                 /**< �Աȶ�ֵ[-100~100] */
	INT iSaturationValue;               /**< ���Ͷ�ֵ[-100~100] */
	INT iEnableDCAperture;              /**< ʹ��DC��Ȧ 0: ��ʹ��  1��ʹ�� */
	INT iColorMatrixMode;				/**< ʹ��ͼ����ǿ���ܣ�1 Ϊ�ر�ʹ�ܣ�2Ϊʹ��ץ�ģ�3Ϊʹ������֡ */
	INT iTemThreshold;              	/**< ɫ��5������[32,40,52,55,60,70] */
	INT iHueThreshold;              	/**< ɫ��[-180~180] */
	INT iHazeRemovalMode;				/**< ʹ��ͼ������ܣ�1 Ϊ�ر�ʹ�ܣ�2Ϊʹ��ץ�ģ�3Ϊʹ������֡ */
	INT iHazeRemovalThreshold;			/**< ͼ�����ֵ[0~10] */

	// A67
	BOOL fEnableH264BrightnessAdjust;
	INT iAdjustPointX;
	INT iAdjustPointY;

	INT iEnableCCDSignalCheck;

	// ��ͬ������
	INT iExternalSyncMode;		// 0-������;1-�����ڲ�����ͬ����2-�����ⲿ����ͬ����Ŀǰ����ӵڶ�·ץ���źŽӿڽ���ͬ���źţ�
	INT iSyncRelay;				// ��ͬ���ź��ӳ٣���Χ0-20000����λΪ1us.

    INT iSaturationEnable;              /**< ���Ͷ�ʹ�� */
    INT iContrastEnalbe;                /**< �Աȶ�ʹ��*/
    //    INT iSharpenEnable;                 /**< �񻯶�ʹ��*/
    INT iWDRValue;                      /**  WDRǿ��ֵ 0Ϊ��ʹ�ܣ�ֵԽ��ǿ��Խ��ȡֵ��Χ��0~4095*/
    INT iSNFTNFMode;                    /** 0:��ʹ��;1:TNFģʽ;2:SNFģʽ;3:TNF��SNFģʽ*/
    INT iTNFSNFValue;                     /** ����ǿ��ֵ0~3:�Զ����͡��С���*/
    INT iGrayImageEnable;                     /** �ڰ�ͼʹ��  0:��ʹ��;1:ʹ��*/
    INT iCVBSMode;                     /** CVBS��ʽ ��Χ:0~1   0��PAL��1��NTSC*/
    INT iJpegAutoCompressEnable;        /** Jpeg�Զ����ڿ��� */
    INT iGammaValue;                  /** ���٤�� */
    INT iGammaEnable;
    INT iEdgeValue;                /** ͼ���Ե��ǿ*/

	//���ǳ�������Ʋ���
	INT iLEDPolarity;				/**< ����ƿ���������ԡ�1������0����*/
	INT iLEDOutputType;				/**< �����������͡�1����������ƽ��, 0:OC(����)*/
	INT iLEDPluseWidth;              /**< ���������0~255*/
	INT iALMPolarity;				/**< ����������ԡ�1������0����*/
	INT iALMOutputType;				/**< ����������͡�1����������ƽ��, 0:OC(����)*/
	INT iF1OutputType;				/**< F1������͡�1:���������ź�;0:���˻�����ź�*/

	INT iFlashEnable;
	INT iFlashPolarity;
	INT iFlashOutputType;
	INT iFlashCoupling;
	INT iFlashPluseWidth;

	INT iColorGradation;			/**<ɫ�� 0 �� 0~255��1��16~235 */

	INT iCVBSExport;				/**< CVBS���ͼ��ģʽ��0:��Сģʽ, 1: �ü�ģʽ*/
	INT	iCVBSCropStartX;				/**< CVBS�ü�ģʽ��ʼ������ */
	INT iCVBSCropStartY;				/**< CVBS�ü�ģʽ��ʼ������ */


	INT iIris;	/**< ��Ȧ�趨ֵ��ֻ��ľ����Ч */
	INT iAutoFocus;	/**< �Զ��۽�ֵ��ֻ��ľ����Ч */
	INT iAWBMode;                     /**< AWBģʽ ��ֻ��ľ����Ч */
	INT iAEMode;                     /**< AEģʽ ��ֻ��ľ����Ч*/
	INT iWDREnable;                     /**< WDR���� ��ֻ��ľ����Ч*/
	INT iNRLevel;                     /**< ����ȼ���ֻ��ľ����Ч*/

	INT iAWBWorkMode;					/**< AWB����ģʽ:0,ƫ��ģʽ,1:ƫ��ģʽ>*/
	INT iAutoControlCammeraAll;			/*< ȫ�Զ����ƿ��أ�0-�ֶ����ƣ�1-ȫ�Զ�����*/
	_CamAppParam()
	{
		iJpegStream = 1;
		iH264Stream = 0;

		iIFrameInterval = 10;
		iTargetBitRate = 6*1024; //Kbps
		iMaxBitRate = 16 * 1024;  //16 * 1024 Kbps
		iFrameRate = 12;
		iResolution = 2;
		iRateControl = 0;       //0:VBR 1:CBR
		iVbrDuration = 0;
		iVbrSensivitity = 0;
		
		iIFrameIntervalSecond = 10;	  /**< �ڶ�·H.264�����е�I֡��� */
		iTargetBitRateSecond = 6*1024;	  /**< �ڶ�·H.264���������� */
		iMaxBitRateSecond = 16 * 1024;
		iFrameRateSecond = 25;		  /**< �ڶ�·H.264����֡�� */
		iResolutionSecond = 0;		  /**< �ڶ�·H.264ͼ��ֱ��� */
		iRateControlSecond = 0;
		iVbrDurationSecond = 0;
		iVbrSensivititySecond = 0;

		iAGCTh = 100;
		iAGCShutterLOri = 100;
        iAGCShutterHOri = 3000;
		iAGCGainLOri = 70;
        iAGCGainHOri = 150;
		iAGCScene = 0;

        iAGCDayNightShutterControl = 0;
        iAGCNightShutterHOri = 5000;

		iGainR = 0;
		iGainG = 0;
		iGainB = 0;
		iGain = iAGCGainLOri;
		iShutter = 2000;

		iIris = 5;
		iAutoFocus = 2;
		iAWBMode = 0;
		iAEMode = 0;
		iWDREnable = 0;
		iNRLevel = 0;

		iCaptureGainR = 36;
		iCaptureGainG = 36;
        iCaptureGainB = 36;
        iCaptureGain = 80;
        iCaptureShutter = 1000;
		iCaptureSharpenThreshold = 0xa0;
        iCaptureAutoParamEnable = 0;
        iEnableCaptureGainRGB = 0;
        iEnableCaptureGain = 1;
        iEnableCaptureShutter = 1;
		iEnableCaptureSharpen = 0;

		iJpegCompressRate = 60;
		iJpegCompressRateCapture = 80;


		swpa_strcpy(szNFSParam, "-o timeo=1,soft,tcp,nolock,retry=1");
		memset(szSafeSaverInitStr, 0, sizeof(szSafeSaverInitStr));

		iJpegExpectSize = 0;
		iJpegCompressRateL = 30;
		iJpegCompressRateH = 90;

		iJpegType = 0;
		iAGCEnable = 0;
		iAWBEnable = 1;
		iFlashRateSynSignalEnable = 0;
		iFlashRateMultiplication = 1;
		iFlashRatePolarity = 1;
		iFlashRatePulseWidth = 4000;
		iFlashRateOutputType = 0;
		iCaptureSynPolarity = 1;			
		iCaptureSynPulseWidth = 1000;		
		iCaptureSynOutputType = 1;			
		iCaptureSynCoupling = 0;			
		iCaptureSynSignalEnable = 1;
		iCaptureEdge = 0;
		iFlashDifferentLane = 0;
		iFlashDifferentLaneExt = 0;
		iEnableDCAperture = 0;
		iEnableSharpen = 0;
		iSharpenCapture = 1;
		iSharpenThreshold = 7;
		iColorMatrixMode = 1;
		iContrastValue = 0;
		iSaturationValue = 8;
		iTemThreshold = 32;
		iHueThreshold = 0;
		iHazeRemovalMode = 1;
		iHazeRemovalThreshold = 0;

		//strcpy(szNetDiskIP, "172.18.10.10");
		swpa_strcpy(szFtpRoot, "/array1");
		swpa_strcpy(szNetDiskUser, "No User");
		swpa_strcpy(szNetDiskPwd, "123");
		//strcpy(szNFS, "/volume1/nfs");
		//iNetDiskSpace = 1;

		iAvgY = 30;

		iGammaMode = 2;
		rgiGamma[0][0] = 32;
		rgiGamma[0][1] = 32;

		rgiGamma[1][0] = 64;
		rgiGamma[1][1] = 64;

		rgiGamma[2][0] = 96;
		rgiGamma[2][1] = 96;

		rgiGamma[3][0] = 128;
		rgiGamma[3][1] = 128;

		rgiGamma[4][0] = 160;
		rgiGamma[4][1] = 160;

		rgiGamma[5][0] = 192;
		rgiGamma[5][1] = 192;

		rgiGamma[6][0] = 222;
		rgiGamma[6][1] = 222;

		rgiGamma[7][0] = 255;
		rgiGamma[7][1] = 255;

		swpa_memcpy(rgiDefGamma, rgiGamma, 64);
		for(INT i = 0; i < 16; i++)
		{
			rgiAGCZone[i] = 1;
        }
       // swpa_memset(rgiTrafficLigtZone,0,sizeof(rgiTrafficLigtZone));
        iSaturationEnable = 0;
        iContrastEnalbe = 0;
        //iSharpenEnable = 0;
        iWDRValue = 256;		//Ĭ��Ϊ256
		iSNFTNFMode = 3;
        iTNFSNFValue = 0;
        iGrayImageEnable = 0;
		iCVBSMode = 0;                     /** CVBS��ʽ ��Χ:0~1   0��PAL��1��NTSC*/
    	iJpegAutoCompressEnable = 0;        /** Jpeg�Զ����ڿ��� */
		iGammaValue = 10;
		iGammaEnable = 0;
        iEdgeValue = 0x80;

		iX = 0;
		iY = 0;
		iFontSize = 32;             // ע��Ĭ����32����
		iDateFormat = 0;
		iFontColor = 0x00ff0000;    // ע����ʱ��Ĭ��ֵ��Ϊ��ɫ(0x00ff0000)
		swpa_strcpy(szCharater, "");
		iEnableCharacterOverlap = 0;
		iEnableFixedLight = 0;      // ע��Ĭ������������泡�����ȶ��ı�

		fEnableH264BrightnessAdjust = FALSE;
		iAdjustPointX = 10;
		iAdjustPointY = 25;
		iEnableCCDSignalCheck = 1;

		iExternalSyncMode = 1;
		iSyncRelay = 2200;

		iLEDPolarity = 1;
		iLEDOutputType = 0;
		iLEDPluseWidth = 5;
		
		iALMPolarity = 1;
		iALMOutputType = 0;

        iF1OutputType = 1;
	
        iFlashEnable		= 1;
		iFlashPolarity		= 0;
        iFlashOutputType	= 1;
		iFlashCoupling  	= 0;
		iFlashPluseWidth	= 0;
		
		iColorGradation = 0;

		iCVBSExport = 1;
		iCVBSCropStartX = 600;
		iCVBSCropStartY = 252;

        iAWBWorkMode = 2;
		iAutoControlCammeraAll = 1;

	};

} CamAppParam;

typedef struct _FrontPannelParam
{
	int iUsedAutoControllMode;
	int iAutoRunFlag;
	int iPulseWidthMin;
	int iPulseWidthMax;
	int iPulseStep;
	int iPulseLevel;
	int iPolarizingPrismMode;
	int iPalanceLightStatus;
	int iFlash1PulseWidth;
	int iFlash1Polarity;
	int iFlash1Coupling;
	int iFlash1ResistorMode;
	int iFlash2PulseWidth;
	int iFlash2Polarity;
	int iFlash2Coupling;
	int iFlash2ResistorMode;
	int iFlash3PulseWidth;
	int iFlash3Polarity;
	int iFlash3Coupling;
	int iFlash3ResistorMode;
	int iDivFrequency;
	int iOutPutDelay;

	_FrontPannelParam()
	{
		iUsedAutoControllMode = 0;
		iAutoRunFlag = 1;
		iPulseWidthMin = 5;
		iPulseWidthMax = 42;
		iPulseStep = 5;
		iPulseLevel = 0;
		iPolarizingPrismMode = -1;
		iPalanceLightStatus = 0;
		iFlash1PulseWidth = 100;
		iFlash1Polarity = 0;
		iFlash1Coupling = 0;
		iFlash1ResistorMode = 0;
		iFlash2PulseWidth = 100;
		iFlash2Polarity = 0;
		iFlash2Coupling = 0;
		iFlash2ResistorMode = 0;
		iFlash3PulseWidth = 100;
		iFlash3Polarity = 0;
		iFlash3Coupling = 0;
		iFlash3ResistorMode = 0;
		iDivFrequency = 1;
		iOutPutDelay = 0;
	};
}FrontPannelParam;

typedef struct _OVERLAY
{
	BOOL fH264Eanble;
	BOOL fH264SecondEnable;	//�ڶ�·H264�ַ�����ʹ��
	H264_OVERLAY_INFO cH264Info;
	H264_OVERLAY_INFO cH264SecondInfo;	//�ڶ�·H264�ַ����Ӳ���
	JPEG_OVERLAY_INFO cJPEGInfo;
	_OVERLAY()
	{
		memset(this, 0, sizeof(*this));
		cJPEGInfo.iColor = 0x000000FF;
		cJPEGInfo.iFontSize = 32;
		swpa_strcpy(cH264Info.szInfo, "$(Time)  ·������:$(RoadName)  ·�ڷ���:$(RoadDir)  �豸���:$(DevID)");
		swpa_strcpy(cH264SecondInfo.szInfo, "�ڶ�·H264���ַ���������");
		swpa_strcpy(cJPEGInfo.szInfo, "$(Time)  ·������:$(RoadName)  ·�ڷ���:$(RoadDir)  $(PlateName)  �豸���:$(DevID)  ��α��:$(EncryptCode)");
	}
}OVERLAY;
#define NAME_LEN 32
typedef struct _GB28181PARAM
{
    BOOL fEnalbe;
    BOOL fVideoSaveEnable;
    CHAR szServerID[NAME_LEN];
    CHAR szServerRegion[NAME_LEN];
    CHAR szIPC_ID[NAME_LEN];
    CHAR szIPC_Region[NAME_LEN];
    CHAR szServerIP[NAME_LEN];
    CHAR szAlarmID[NAME_LEN];
    CHAR szPassWord[NAME_LEN];
    INT  iServerPort;
    INT  iIPC_Port;
    INT  iIPC_VideoPort;
    CHAR szLongitude[NAME_LEN];/*����*/
    CHAR szLatitude[NAME_LEN];/*γ��*/
//    CHAR szDeviceName[NAME_LEN];
    CHAR szManufacturer[NAME_LEN];
//    CHAR szModel[NAME_LEN];/*�豸�ͺ�*/
//    CHAR szFirmware[NAME_LEN];/*�豸�̼��汾*/
    CHAR szBlock[NAME_LEN]; /*����*/
    CHAR szAddress[NAME_LEN];/*��װ��ַ*/
    CHAR szOwner[NAME_LEN]; /*�豸����*/
    CHAR szCivilCode[NAME_LEN];/*��������*/


    _GB28181PARAM()
    {
        swpa_memset(this, 0, sizeof(*this));
        fEnalbe = FALSE;
        fVideoSaveEnable = FALSE;

        swpa_strcpy(szServerID,"34020000002000000001");
        //swpa_strcpy(szServerRegion,"3402000000");
        swpa_strcpy(szIPC_ID,"34020000001320000001");
       // swpa_strcpy(szIPC_Region,"3402000000");
        swpa_strcpy(szServerIP,"172.18.11.123");
        swpa_strcpy(szAlarmID,"34020000001340000001");
        swpa_strcpy(szPassWord,"12345678");
        iServerPort = 5060;
        iIPC_Port = 5060;
        iIPC_VideoPort = 5080;

        swpa_strcpy(szLongitude,"0.0");
        swpa_strcpy(szLatitude,"0.0");

     //   swpa_strcpy(szDeviceName,"LS130200000");

        swpa_strcpy(szManufacturer,"NULL");
    //    swpa_strcpy(szModel,"EC200W");
   //     swpa_strcpy(szFirmware,"V1.0");
        swpa_strcpy(szBlock,"NULL");
        swpa_strcpy(szAddress,"NULL");
        swpa_strcpy(szOwner,"NULL");
        swpa_strcpy(szCivilCode,"NULL");

    }

}GB28181PARAM;

/* typedef struct _RTSP_PARAM */
/* { */
/* 	INT iMajorCommunicationMode;  // ������ͨѸģʽ  0:���� 1:�鲥 */
/* 	INT iMinorCommunicationMode;  // ������ */
/* 	INT iRTSPStreamNum; // RTSP����Filter����,���������� */

/* 	_RTSP_PARAM() */
/* 	{ */
/* 		iMajorCommunicationMode = 0;  //Ĭ���ǵ��� */
/* 		iMinorCommunicationMode = 0;  //Ĭ���ǵ��� */
/* 		iRTSPStreamNum = 1; //Ĭ����һ· */
/* 	} */
/* }RTSP_PARAM; */


typedef struct _AutoRebootPARAM
{
	BOOL fEnable;			/** �Զ���λʹ�� ��Χ:0~1   0����ʹ�ܣ�1��ʹ��*/
	INT iCarLeftCountLimit;	/** ���������� ��Χ:0~1000 ʱ�����Ҫ��С�ڸ�ֵ���ܸ�λ*/
	INT iSysUpTimeLimit;	/** ϵͳ����ʱ��(��λСʱh)���� ��Χ:24~720 ϵͳ����ʱ�������������ſ��Ǹ�λ*/
	_AutoRebootPARAM()
	{
		fEnable = FALSE;			//Ĭ�ϲ�ʹ��
		iCarLeftCountLimit = 50;	//ƽ��һ����5����
		iSysUpTimeLimit = 48;		//2*24
	}
	
}AutoRebootPARAM;

//ʶ���豸���Ľṹ��
typedef struct _ModuleParams
{
	INT               nLogLevel;
	INT               nWorkModeIndex;
	INT 			  nCameraWorkMode;		//�������ģʽ
	INT				  nMCFWUsecase;			//MCFW Usecase
	ResultSenderParam cResultSenderParam;
	TcpipParam        cTcpipCfgParam;
	IMG_FRAME_PARAM   cImgFrameParam;
	TRACKER_CFG_PARAM cTrackerCfgParam;
	CAM_CFG_PARAM     cCamCfgParam;

	CamAppParam       cCamAppParam;
	FrontPannelParam  cFrontPannelParam;

	MATCHPARAM        cMatchParam;
	DEVICEPARAM       cDevParam[2];

	OVERLAY           cOverlay;

    GB28181PARAM      cGb28181;
	RTSP_PARAM_INFO   cRtspParam;
	ONVIF_PARAM_INFO  cOnvifParam;
    INT               nMJPEGWidth;
    INT               nMJPEGHeight;
	AutoRebootPARAM	  cAutoRebootParam;
   	DOMEROCKER_PARAM cDomeRockerParam;  // ���ҡ�˲���
	_ModuleParams()
	{
		nLogLevel = SW_LOG_LV_NORMAL;
		nWorkModeIndex = 0;
		nCameraWorkMode = 0;
		nMCFWUsecase = 0;
        nMJPEGWidth = 1920;
        nMJPEGHeight = 1080;
	}
} ModuleParams;

enum
{
	PRM_TOLLGATE,               //�շ�վ
	PRM_HIGHWAY_HEAD,           //���� - ��ͷ
	PRM_HIGHWAY_TAIL,           //���� - ��β
	PRM_PATROL_LEFT,            //�ƶ����� - ���
	PRM_PATROL_RIGHT,           //�ƶ����� - �Ҳ�
	PRM_PORTABLE,               //������
	PRM_ELECTRONIC_POLICE,      //���Ӿ���
	PRM_TRAFFICE_EVENT_HEAD,    //��ͨ�¼����-��ͷ
	PRM_TRAFFICE_EVENT_TAIL,    //��ͨ�¼����-��β
	PRM_HVC,	                  //ץ��ʶ��
	PRM_PARK,                   //ͣ����˫·
	PRM_PARK_SINGLE,            //ͣ������·
	PRM_CAP_FACE,               //��������
	PRM_COVER_CTRL,             //���������
	PRM_CAMERA,
	PRM_DOME_CAMERA,					//����Υͣץ��
	PRM_TRAFFICE_FACE,			//249 ��������
	PRM_MAX
};

class CSWLPRParameter : public CSWParameter<ModuleParams>
{
	CLASSINFO(CSWLPRParameter, CSWObject)
public:
	enum{DELETE_LEVEL = 0, INNER_LEVEL = 1, PROJECT_LEVEL = 3, CUSTOM_LEVEL = 5};

	CSWLPRParameter();
	virtual ~CSWLPRParameter();
	virtual HRESULT Initialize(CSWString strFilePath);
	virtual HRESULT GetWorkMode(INT &iWorkMode, CSWString & strWorkKode);
	virtual LPCSTR  GetVersion(void){return (LPCSTR)m_strVersion;}

	virtual void ResetParam(VOID);
	/**
	*@brief ��ʼ��system�ڵ�
	*@return �ɹ�����S_OK,����ֵ��ʾʧ��
	*/
	virtual HRESULT InitSystem();

	/**
	*@brief 
	*/
	virtual HRESULT InitTracker(VOID);

	/**
	*@brief 
	*/
	virtual HRESULT InitHvDsp(VOID);

	/**
	*
	*/
	virtual HRESULT InitOuterCtrl(VOID);

	/**
	*
	*/
	virtual HRESULT InitIPTCtrl(VOID);

	/**
	*
	*/
	virtual HRESULT InitCamApp(VOID);

	/**
	*
	*/
	virtual HRESULT Init368(VOID);

	/**
	*
	*/
	virtual HRESULT InitFrontPannel(VOID);

	/**
	*
	*/
	virtual HRESULT InitScaleSpeed(VOID);
	
	/**
	*
	*/
	virtual HRESULT InitCharacter(VOID);
    /**
    *
    */
    virtual HRESULT InitGB28181(VOID);

	/*
	 *@biref Init ONVIF Param
	 *@return On success, S_OK is returned 
	 */
	virtual HRESULT InitONVIF(VOID);

protected:
	HRESULT InitCamCfgParam(CAM_CFG_PARAM *pCfgCamParam, const CHAR **szLightType);
	BOOL CheckArea(INT nLeft, INT nTop, INT nRight, INT nBottom);
	void SetDefaultTracker();
	void SetDefaultCamApp();
	void SetDefaultCamCfgParam();
	void SetDefaultCharacter();
	void SetDefaultGB28181();
	void SetDefaultOnvif();
	void SetDefaultFrontPannel();
	void SetDefaultHvDSP();
	void SetDefaultIPTCtrl();
	void SetDefaultOuterCtrl();
	
	
	
protected:
	CSWString m_strWorkMode;
	CSWString m_strVersion;
};
#endif

