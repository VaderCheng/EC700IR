// ���ļ������ʽ������WINDOWS-936��ʽ

/**
* @file     DspLinkCmd.h
* @version  1.0
* @brief    DSPLinkЭ�鶨��
*/

#ifndef _DSPLINKCMD_H_
#define _DSPLINKCMD_H_

#ifndef _NO_DSPBIOS_
#include "trackerdef.h"
#endif


typedef enum
{
    DCI_UNKNOWN = 0,		// δ֪����
    DCI_RECTIFY_TIME,		// ʱ�ӽ�������
    DCI_INIT_PHOTO_RECOGER, // ��ʼ��ͼƬʶ���������
    DCI_PROCESS_PHOTO,		// ʶ��ͼƬ����
    DCI_INIT_VIDEO_RECOGER, // ��ʼ����Ƶ��ʶ������
    DCI_PROCESS_ONE_FRAME,  // ����һ֡��Ƶ������
    DCI_JPEG_ENCODE,		// Jpeg��������
    DCI_H264_ENCODE,		// H.264��������
    DCI_CAM_DSP,            // ���ƽ̨�źŴ�������
    DCI_IMAGE_EDDY_TYPE,    // ����ͼƬ��ת��ʶ����
    DCI_STRING_OVERLAYINIT, // ��ʼ���ַ���������
    DCI_MODIFY_VIDEO_PARAM, // ��̬�޸�ʶ�����
    DCI_UPDATE_ALG_PARAM, // ��̬�޸��㷨����
    DCI_UPDATE_MODEL_DATA, // ��̬���³������ģ������
    DCI_COUNT
} DSP_CMD_ID;

typedef struct _DSP_RESPOND_HEAD
{
    DWORD32 dwCmdId;
    int iResult;
    char szErrorInfo[4096];
    int iAppendDataSize;
} DSP_RESPOND_HEAD;

#ifndef _NO_DSPBIOS_

typedef struct _IMAGE_EDDYTYPE_PARAM
{
    int nEddyType;
} IMAGE_EDDYTYPE_PARAM;

typedef struct _RECTIFY_TIME_PARAM
{
    DWORD32 dwArmCurTime;
} RECTIFY_TIME_PARAM;

typedef struct _INIT_PHOTO_RECOGER_PARAM
{
    int nPlateRecogParamIndex;
    TRACKER_CFG_PARAM cTrackerCfgParam;
} INIT_PHOTO_RECOGER_PARAM;

typedef struct _PROCESS_PHOTO_PARAM
{
    PR_PARAM prPram;
    IReferenceComponentImage* rgpSmallImage[MAX_EVENT_COUNT];
    IReferenceComponentImage* rgpBinImage[MAX_EVENT_COUNT];
#ifdef __cplusplus
    _PROCESS_PHOTO_PARAM()
    {
        memset(this, 0, sizeof(*this));
    }
#endif
} PROCESS_PHOTO_PARAM;

typedef struct _PROCESS_PHOTO_DATA
{
    HV_COMPONENT_IMAGE hvImage;
    HV_COMPONENT_IMAGE hvImageYuv;
    HV_COMPONENT_IMAGE rghvImageSmall[MAX_EVENT_COUNT];
    HV_COMPONENT_IMAGE rghvImageBin[MAX_EVENT_COUNT];
} PROCESS_PHOTO_DATA;

typedef struct _PROCESS_PHOTO_RESPOND
{
    int iResultCount;
    PROCESS_IMAGE_CORE_RESULT rgProcessPhotoResult[MAX_EVENT_COUNT];
#ifdef __cplusplus
    _PROCESS_PHOTO_RESPOND()
    {
        iResultCount = 0;
    }
#endif
} PROCESS_PHOTO_RESPOND;

typedef struct _TRIG_EVENT
{
    DWORD32 dwEventId;
    int iCarArriveCount;
    int iCarLeftCount;
    CARARRIVE_INFO_STRUCT rgCarArriveInfo[MAX_EVENT_COUNT];
    PROCESS_IMAGE_CORE_RESULT rgCarLeftCoreInfo[MAX_EVENT_COUNT];
#ifdef __cplusplus
    _TRIG_EVENT()
    {
        iCarArriveCount = 0;
        iCarLeftCount = 0;
        dwEventId = 0x00;
    }
#endif
} TRIG_EVENT;

typedef enum
{
    IOF_UNKNOWN = 0,	// δ֪
    IOF_ADD_REF,		// ��������
    IOF_RELEASE,        // �ͷ��ڴ�
    IOF_COUNT
} IMG_OPER_FLAG;

/**
* ͼƬ�ڴ������¼��Ԫ*/
typedef struct _IMG_MEM_OPER
{
    IReferenceComponentImage* pMemAddr;   // ������ַ
    int nOperFlag;
} IMG_MEM_OPER;

/**
* ͼƬ�ڴ������¼
*/
typedef struct _IMG_MEM_OPER_LOG
{
    int nCount;
    IMG_MEM_OPER rgOperInfo[1024];
#ifdef __cplusplus
    _IMG_MEM_OPER_LOG()
    {
        memset(this, 0, sizeof(_IMG_MEM_OPER_LOG));
    }
#endif
} IMG_MEM_OPER_LOG;

/**
* ͼƬ�ڴ��������
*/
void ImageAddRef(IMG_MEM_OPER_LOG* pLog, IReferenceComponentImage* pRefImage);
void ImageRelease(IMG_MEM_OPER_LOG* pLog, IReferenceComponentImage* pRefImage);

typedef struct _INIT_VIDEO_RECOGER_PARAM
{
    int nPlateRecogParamIndex;
    int nLastLightType;
    int nLastPulseLevel;
	int nLastCplStatus;
	DWORD32 dwArmTick;
    TRACKER_CFG_PARAM cTrackerCfgParam;
} INIT_VIDEO_RECOGER_PARAM;

// Υͣ����������ز���
typedef struct tag_CarDetectParam
{
	// Υͣ����������
	BOOL fIsDetectStopBox; 			// �Ƿ��������ļ��
	int icarBoxwidth;				// ������������   ����
	float fltCarBoxleft;			// �������Ͻ�X����   ͼ��ٷֱ�
	float fltCarBoxTop;				// �������Ͻ�Y����   ͼ��ٷֱ�
	float fltCarBoxright;   		// �������½�X����   ͼ��ٷֱ�
	float fltCarBoxbottom;			// �������½�Y����   ͼ��ٷֱ�
	int iStepDivX;					// �����ⲽ��ϵ��X    ����
	int iStepDivY;					// �����ⲽ��ϵ��Y    ����
	int iScaleNum;					// ��С������         �����ֵ��С�����ظ�������
	float fltScaleCoef;				// ��С���ϵ����          ����⳵�����
	int iMergeNum;					// �ص�������кϲ�����    �ϲ�����
	int iMaxROI;					// ����ѡ�����       �������ɸѡ����󳵿���
	int iDetType;					// ������ͣ�����һ�μ�⻹�ǽ��ж��μ��   ����dsp�Ƿ���Ҫ�������ź����껹ԭ
	int iDetNO;						// ͬ�ϴ���dsp��
	float fltDetsalc;				// ȡ���������С���г�2�Ĵ�������������Ļ�ԭ��
	int iDetSelTypeNO;				// ���ģ�͵�ѡ��
	BOOL fIsDay;
} CAR_DETECT_PARAM;

// ��֡ʶ�����
typedef struct tag_FrameRecognizeParam
{
    DETECTAREA cRecogSnapArea; // ץ��ͼʶ�����
    int  iPlateUpExpend;        // ����������չ ����λ��ռ���Ƶİٷֱȣ�
	int  iPlateDownExpend;      // ����������չ ����λ��ռ���Ƶİٷֱȣ�
	bool fPlateExpendEnable;    // ����ʶ����չ����
    int iEnvStatus;     // ����״̬������ARM��  3:day, 2:dusk, 1:night

    HV_RECT rectCaptureRegArea;     // ץ��ͼʶ������
} FRAME_RECOGNIZE_PARAM;

typedef struct _PROCESS_ONE_FRAME_PARAM
{
    int iVideoID;
    BOOL fIsCaptureImage;
    DWORD32 dwFrameNo;
    DWORD32 dwImageTime;
    DWORD32 dwCurTick;
	int iDiffTick;
    BOOL fIsCheckLightType;
	BOOL fIsARMNight;		//Arm���ж�Ϊ����

    IReferenceComponentImage* pCurFrame;
    IReferenceComponentImage* rgpSmallImage[MAX_EVENT_COUNT];
    IReferenceComponentImage* rgpBinImage[MAX_EVENT_COUNT];
	DWORD32 dwIOLevel;
	int iEnvStatus;         // ����״̬������ARM��  3:day, 2:dusk, 1:night
	int iCarArriveTriggerType;

	CAR_DETECT_PARAM cDetectParam;
	FRAME_RECOGNIZE_PARAM cFrameRecognizeParam;

#ifdef __cplusplus
    _PROCESS_ONE_FRAME_PARAM()
    {
        memset(this, 0, sizeof(*this));
    }
#endif
} PROCESS_ONE_FRAME_PARAM;

typedef struct _PROCESS_ONE_FRAME_DATA
{
    HV_COMPONENT_IMAGE hvImage;
    HV_COMPONENT_IMAGE hvImageYuv;
    HV_COMPONENT_IMAGE rghvImageSmall[MAX_EVENT_COUNT];
    HV_COMPONENT_IMAGE rghvImageBin[MAX_EVENT_COUNT];
    SYNC_DET_DATA cSyncDetData;
#ifdef __cplusplus
    _PROCESS_ONE_FRAME_DATA()
    {
       memset(this, 0, sizeof(*this));
    }
#endif
} PROCESS_ONE_FRAME_DATA;

typedef struct _PROCESS_ONE_FRAME_RESPOND
{
    TRIG_EVENT cTrigEvent;
    LIGHT_TYPE cLightType; // ���ȼ���
    int nWDRLevel;	// WDR�ȼ�
	int iCplStatus; // ƫ�⾵״̬ 1:ʹ�� 0:��ʹ�� -1:δ����
	int iPulseLevel;
	int iFrameAvgY; //֡ƽ������
	HV_RECT rcRedLight[20];
    TRACK_RECT_INFO cTrackRectInfo;
    IMG_MEM_OPER_LOG cImgMemOperLog;
    int nEnvLightType; // 0���죬1����2����
	int iDetType[20];
#ifdef __cplusplus
    _PROCESS_ONE_FRAME_RESPOND()
    {
       memset(this, 0, sizeof(*this));
    }
#endif
} PROCESS_ONE_FRAME_RESPOND;

#endif

// --- Jpeg���� ---

typedef struct _JPEG_ENCODE_PARAM
{
    DWORD32 dwCompressRate;         // Jpegѹ����
	DWORD32 dwJpegDataType;         // Jpeg�������ͣ�0����ʾJpeg���ݶη����Σ�1����ʾJpeg���ݶ�Ϊһ�Σ�ע��Ŀǰ����500wģʽ����Ч��
    INT iEddy;                      // ��ʱ����ת90��

#ifdef __cplusplus
    _JPEG_ENCODE_PARAM()
    {
        memset(this, 0, sizeof(*this));
    }
#endif
} JPEG_ENCODE_PARAM;

typedef struct tagTEXT
{
	int iAlpha;
	int iSize;
	int iWidth;
	int iHeight;
#ifdef __cplusplus	
	tagTEXT()
	{
		memset(this, 0, sizeof(*this));
	}
#endif
}TEXT;

typedef struct _JPEG_ENCODE_DATA
{
    HV_COMPONENT_IMAGE hvImageYuv;  // ͼƬԴ���ݣ���ʽ��CbYCrY
    HV_COMPONENT_IMAGE hvImageJpg;
    BYTE8 szDateTimeStrings[32];    // Ҫ���ӵ�JpegͼƬ�е�����ʱ���ַ�����Ϊ�����ʾ�����õ��ӹ��ܣ�
	
	PBYTE8 phys;    //����λͼָ��
	char   szTime[255]; //ʱ������
	int    x;       //���ӵ�xλ��
	int    y;		//���ӵ�yλ��
	BYTE8  yColor;  //����λͼ��Y��ɫ����
	BYTE8  uColor;  //����λͼ��U��ɫ����
	BYTE8  vColor;  //����λͼ��V��ɫ����

#ifdef __cplusplus
    _JPEG_ENCODE_DATA()
    {
        memset(this, 0, sizeof(*this));
    }
#endif
} JPEG_ENCODE_DATA;

typedef struct _JPEG_ENCODE_RESPOND
{
    DWORD32 dwJpegLen;
    DWORD32 dwExtErrCode;

#ifdef __cplusplus
    _JPEG_ENCODE_RESPOND()
    {
        memset(this, 0, sizeof(*this));
    }
#endif
} JPEG_ENCODE_RESPOND;

// --- H.264���� ---

//H.264�������������Ͷ���
#define OPTYPE_OPEN 1
#define OPTYPE_ENCODE 2
#define OPTYPE_CLOSE 3

typedef struct _H264_ENCODE_PARAM
{
    DWORD32 dwOpType;               // �������ͣ����������룬�ر�
    DWORD32 dwIntraFrameInterval;   // I֡��������磺��ֵΪ3��������Ϊ��I P P I P P ...��
    DWORD32 dwTargetBitRate;        // ������
    DWORD32 dwFrameRate;            // ֡��
    DWORD32 dwInputWidth;           // ����ͼ��Ŀ��
    DWORD32 dwInputHeight;          // ����ͼ��ĸ߶�
    DWORD32 dwOutputBufSize;        // H.264֡������������Ĵ�С
    BOOL fEnableH264BrightnessAdjust;   // ʹ��H264���ȵ���
    int iAdjustPointX;                  // H264���ȵ��ڵ�Xֵ
    int iAdjustPointY;                  // H264���ȵ��ڵ�Yֵ
#ifdef __cplusplus
    _H264_ENCODE_PARAM()
    {
        memset(this, 0, sizeof(*this));
    }
#endif
} H264_ENCODE_PARAM;

typedef struct _H264_ENCODE_DATA
{
    HV_COMPONENT_IMAGE hvImageYuv;  // ͼƬԴ���ݣ���ʽ��CbYCrY
    HV_COMPONENT_IMAGE hvImageFrame;
    BYTE8 szDateTimeStrings[32];    // Ҫ���ӵ�H.264�����е�����ʱ���ַ�����Ϊ�����ʾ�����õ��ӹ��ܣ�
    BOOL fDoubleStream;             // fDoubleStream��Ϊ�˽��˫����ʱ������ɫ��ʾ��һ�¶�����
#ifdef __cplusplus
    _H264_ENCODE_DATA()
    {
        memset(this, 0, sizeof(*this));
    }
#endif
} H264_ENCODE_DATA;

//֡���Ͷ���
#define FRAME_TYPE_H264_I 0
#define FRAME_TYPE_H264_P 1
#define FRAME_TYPE_H264_SKIP 2

typedef struct _H264_ENCODE_RESPOND
{
    DWORD32 dwFrameLen;
    DWORD32 dwFrameType;  // ֡���ͣ�0:I֡, 1:P֡, 2:����֡��
    DWORD32 dwExtErrCode;

#ifdef __cplusplus
    _H264_ENCODE_RESPOND()
    {
        memset(this, 0, sizeof(*this));
    }
#endif
} H264_ENCODE_RESPOND;

// --- �ַ����� ---

typedef struct _STRING_OVERLAY_PARAM
{
    // x��y����ͼƬ�п�ʼ������Ϣ�ַ�������ʼλ��
    // w��h����������Ϣ�ַ����ĵ���������ߣ������ص�Ϊ��λ��
    // iIsFixedLight������̶����ȣ�1���̶� 0�������汳��ɫ�仯���仯��
    // rgbDotMatrix����������Ϣ�ַ����ĵ�������
    // bFontR��bFontG��bFontB����������Ϣ�ַ�����������ɫ
    // iFontColor: ��������Ϣ�ַ�����������ɫ
    // fIsSideInstall����װ
    int x;
    int y;
    int w;
    int h;
    int iIsFixedLight;
    int iFontColor;
    BOOL fIsSideInstall;

#ifdef __cplusplus
    _STRING_OVERLAY_PARAM()
    {
        memset(this, 0, sizeof(*this));
    }
#endif
} STRING_OVERLAY_PARAM;

// �������ݣ��洢�ڹ����ڴ��У�����ARM��DSP�䴫��
typedef struct _STRING_OVERLAY_DATA
{
    DSPLinkBuffer rgDotMatrix;

#ifdef __cplusplus
    _STRING_OVERLAY_DATA()
    {
        memset(this, 0, sizeof(*this));
    }
#endif
} STRING_OVERLAY_DATA;

typedef struct _STRING_OVERLAY_RESPOND
{
    DWORD32 dwExtErrCode;

#ifdef __cplusplus
    _STRING_OVERLAY_RESPOND()
    {
        dwExtErrCode = 0;
    }
#endif
} STRING_OVERLAY_RESPOND;

// --- ���ƽ̨�����źŴ��� ---

//CamDsp�������Ͷ���
#define PROC_TYPE_JPEGENC 1          // Jpeg����
#define PROC_TYPE_H264ENC 2          // H.264����
#define PROC_TYPE_JPEGH264ENC 3      // Jpeg��H.264���룬����˫����

typedef struct _CAM_DSP_PARAM
{
    DWORD32 dwProcType;  // CamDsp��������
    JPEG_ENCODE_PARAM cJpegEncodeParam;
    H264_ENCODE_PARAM cH264EncodeParam;

#ifdef __cplusplus
    _CAM_DSP_PARAM()
    {
        memset(this, 0, sizeof(*this));
    }
#endif
} CAM_DSP_PARAM;

typedef struct _CAM_DSP_DATA
{
    JPEG_ENCODE_DATA cJpegEncodeData;
    H264_ENCODE_DATA cH264EncodeData;

#ifdef __cplusplus
    _CAM_DSP_DATA()
    {
        memset(this, 0, sizeof(*this));
    }
#endif
} CAM_DSP_DATA;

typedef struct _CAM_DSP_RESPOND
{
    JPEG_ENCODE_RESPOND cJpegEncodeRespond;
    H264_ENCODE_RESPOND cH264EncodeRespond;
    DWORD32 dwExtErrCode;

#ifdef __cplusplus
    _CAM_DSP_RESPOND()
    {
        memset(this, 0, sizeof(*this));
    }
#endif
} CAM_DSP_RESPOND;

#endif
