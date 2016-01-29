/*
 * arch.h
 *
 *  Created on: 2013��12��2��
 *      Author: liuc
 */

#ifndef ARCH_H_
#define ARCH_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "ti_media_common_def.h"

#define ENC_MAX_CH 4

typedef enum {
    USECASE_FPGA_IMX178,
    USECASE_FPGA_IMX185,
    USECASE_FPGA_IMX185_DUALVIDEO,
    USECASE_FPGA_IMX185_CVBSEXPORT,
    USECASE_FPGA_IMX178_TEST,
    USECASE_FPGA_IMX185_TEST,
    USECASE_IMX136,
    USECASE_IMX172,
    USECASE_JPEG_DEC,
    USECASE_FPGA_IMX178_WITHOUTCVBS,
    USECASE_FPGA_IMX185_DUALVIDEO_CVBSEXPORT,
    USECASE_FPGA_IMX178_WITHOUT_NSF,
    USECASE_DOME_CAMERA,
	USECASE_FPGA_IMX226,
	USECASE_FPGA_IMX249,
	USECASE_FPGA_IMX185_CROP_WITHOUTCVBS,
	USECASE_FPGA_IMX249_H264_VNF,
	USECASE_FPGA_ICX816_680W,
	USECASE_FPGA_ICX816_640W,
	USECASE_FPGA_ICX816_600W,
	USECASE_FPGA_ICX816_SINGLE,
    USECASE_MAX
} UseCase;

typedef enum {
    VIDEO_RES_1080P = 0,
    VIDEO_RES_720P,
    VIDEO_RES_540P,
    VIDEO_RES_576P,
    VIDEO_RES_480P
} VideoConfig;

typedef struct{
    void (*CbFunc)(void *);
    void* CbParam;
}IpcCbParams;

typedef struct{
    int width;
    int height;
}Jpeg_Dec_Config;


typedef struct{
    int nImageInputRate;
    int nImageOutputRate;   /// ����ͼ����֡��Ϣ ���֡�ʵ���ʵ������֡�� x nImageInputRate/nImageOutputRate
    int nVideoInputRate;
    int nVideoOutputRate;   /// ������Ƶ��֡��Ϣ ���֡�ʵ���ʵ������֡�� x nVideoInputRate/nVideoOutputRate
    int nVideoInputRate2;
    int nVideoOutputRate2;  /// ���ڵڶ�·��Ƶ��֡  �����������Ϊʵ������ֵ
}SourceFpsConfig;

typedef struct {
    int cbSize; // Ӧ���ڽṹ���С
    struct filePath{
    char* szFwVpssM3; // VPSS M3��ִ���ļ�·��������·�������¶��ǣ�
    char* szFwVideoM3; // Video M3��ִ���ļ�·��
    char* szFwDsp; // DSP��ִ���ļ�·��
    char* szBinRemoteDbg; // remote_debug_client
    char* szBinSysPri; // SysPri��ִ���ļ�·��
    char* szBinFwLoad; // �̼����س����ִ���ļ�·��
    char* szBinPrcm; // ��Դ��������ִ���ļ�·��
    char* szBinMemrdwr; // �ڴ��д�����ִ���ļ�·��
    char* szKoOsa; // OSA�����ļ�·��
    char* szKoSyslink; // Syslink�����ļ�·��
    char* szKoFb; // FB�����ļ�·��
    char* szKoHdmi; // HDMI�����ļ�·��
    char* szKoVpss; // VPSS�����ļ�·��
    }FilePath;
    UseCase useCase; // Usecase��һ��usecase��Ӧһ��Chain����
    VideoConfig videoConfig; // ��Ƶ������������H.264���������ڶ�·YUV�ֱ������ã�
    VideoConfig videoConfig2;
    SourceFpsConfig sourceFpsConfig;
    int maxBitRate[ENC_MAX_CH];
    IpcCbParams vpssInCb;
    IpcCbParams outVpssCb;
    IpcCbParams videoInCb;
    IpcCbParams outVideoCb;
    IpcCbParams dspInCb;
    IpcCbParams outDspCb;
    IpcCbParams videoFrameInCb;
    IpcCbParams dspcallbackCb;
    int bExternal2A;
    void (*_2aMetadataCbFunc)(void *, unsigned int);
    Jpeg_Dec_Config dec_param;
    int bEnableBIOSLog;
    unsigned int capbufnum;//ָ��Cameralink buffer��������Ϊ0������Ĭ��ֵ��
    unsigned int vnfbufnum;//ָ��vnf�����buffer��������Ϊ0������Ĭ��ֵ��
    unsigned int extendJpegHight; // ��չUSECASE_FPGA_IMX185��VNF����Buffer�ĸ߶ȣ���Ӧ����128��
} IpcInitParams;

int IPC_Init(IpcInitParams *ipcInitParams);

int IPC_Deinit(void);

void IPC_Start(void);

void IPC_Stop(void);

enum {
//  �����������
    IPC_CONTROL_CMD_SET_EXP = 100,              //  �����ع�ʱ��
    IPC_CONTROL_CMD_GET_EXP = 101,              //  ��ȡ�ع�ʱ��
    IPC_CONTROL_CMD_SET_GAIN = 102,             //  ��������
    IPC_CONTROL_CMD_GET_GAIN = 103,             //  ��ȡ����
    IPC_CONTROL_CMD_SET_RGB = 104,              //  ����RGB
    IPC_CONTROL_CMD_GET_RGB = 105,              //  ��ȡRGB
//  ����Զ�������
    IPC_CONTROL_CMD_SET_AE_STATE = 106,         //  ����AEʹ��
    IPC_CONTROL_CMD_GET_AE_STATE = 107,         //  ��ȡAEʹ�ܱ�־
    IPC_CONTROL_CMD_SET_AE_THRESHOLD = 108,     //  ����AE����ֵ
    IPC_CONTROL_CMD_GET_AE_THRESHOLD = 109,     //  ��ȡAE��ǰ����ֵ
    IPC_CONTROL_CMD_SET_AE_EXP_RANGE = 110,     //  ����AE�ع�ʱ�䷶Χ
    IPC_CONTROL_CMD_GET_AE_EXP_RANGE = 111,     //  ��ȡAE�ع�ʱ�䷶Χ
    IPC_CONTROL_CMD_SET_AE_ZONE = 112,          //  ����AE�������
    IPC_CONTROL_CMD_GET_AE_ZONE = 113,          //  ��ȡAE�������
    IPC_CONTROL_CMD_SET_AE_GAIN_RANGE = 114,    //  ����AE���淶Χ
    IPC_CONTROL_CMD_GET_AE_GAIN_RANGE = 115,    //  ��ȡAE���淶Χ
    IPC_CONTROL_CMD_SET_AWB_STATE = 116,        //  ����AWBʹ��
    IPC_CONTROL_CMD_GET_AWB_STATE = 117,        //  ��ȡAWBʹ��
//  ���ͼ����
    IPC_CONTROL_CMD_SET_WDR_STRENGTH = 118,     //  ����WDRǿ��ֵ
    IPC_CONTROL_CMD_GET_WDR_STRENGTH = 119,     //  ��ȡWDRǿ��ֵ
    IPC_CONTROL_CMD_SET_SHARPNESS_STATE = 120,  //  ������ʹ��
    IPC_CONTROL_CMD_GET_SHARPNESS_STATE = 121,  //  ��ȡ��ʹ��
    IPC_CONTROL_CMD_SET_SHARPNESS = 122,        //  ������ֵ
    IPC_CONTROL_CMD_GET_SHARPNESS = 123,        //  ��ȡ��ֵ
    IPC_CONTROL_CMD_SET_SATURATION_CONTRAST_STATE = 124,        //  ���ñ��Ͷȡ��Աȶ�ʹ��
    IPC_CONTROL_CMD_GET_SATURATION_CONTRAST_STATE = 125,        //  ��ȡ���Ͷȡ��Աȶ�ʹ��
    IPC_CONTROL_CMD_SET_SATURATION = 126,       //  ���ñ��Ͷ�ֵ
    IPC_CONTROL_CMD_GET_SATURATION = 127,       //  ��ȡ���Ͷ�ֵ
    IPC_CONTROL_CMD_SET_CONTRAST = 128,         //  ���öԱȶ�ֵ
    IPC_CONTROL_CMD_GET_CONTRAST = 129,         //  ��ȡ�Աȶ�ֵ
    IPC_CONTROL_CMD_SET_TNF_STATE = 130,        //  ���ý���TNFʹ��
    IPC_CONTROL_CMD_GET_TNF_STATE = 131,        //  ��ȡ����TNFʹ��
    IPC_CONTROL_CMD_SET_SNF_STATE = 132,        //  ���ý���SNFʹ��
    IPC_CONTROL_CMD_GET_SNF_STATE = 133,        //  ��ȡ����SNFʹ��
    IPC_CONTROL_CMD_SET_TNF_SNF_STRENGTH = 134, //  ���ý���ǿ��ֵ
    IPC_CONTROL_CMD_GET_TNF_SNF_STRENGTH = 135, //  ��ȡ����ǿ��ֵ
    IPC_CONTROL_CMD_SET_TRAFFIC_LIGTH_ENHANCE_STATE = 136,      //  ���ú�ƼӺ�ʹ��
    IPC_CONTROL_CMD_GET_TRAFFIC_LIGTH_ENHANCE_STATE = 137,      //  ��ȡ��ƼӺ�ʹ��
    IPC_CONTROL_CMD_SET_TRAFFIC_LIGTH_ENHANCE_REGION = 138,     //  ���üӺ������޶�ȡ���
    IPC_CONTROL_CMD_SET_TRAFFIC_LIGTH_LUM_TH = 139,             // ���ú�ƼӺ�������ֵ
    IPC_CONTROL_CMD_SET_GAMMA_ENABLE = 140,     // ����Gamma ʹ��
    IPC_CONTROL_CMD_GET_GAMMA_ENABLE = 141,     // ��ȡGammaʹ��
    IPC_CONTROL_CMD_SET_GAMMA = 142,            // ����Gamma
    IPC_CONTROL_CMD_GET_GAMMA = 143,            // ��ȡGamma
    IPC_CONTROL_CMD_SET_EDGE_ENHANCE = 144,     // ����ͼ���Ե��ǿ
    IPC_CONTROL_CMD_GET_EDGE_ENHANCE = 145,     //����ȡͼ���Ե��ǿֵ

//  ���IO����
    IPC_CONTROL_CMD_SET_DCIRIS_AUTO_STATE = 146,//  �����Զ�DC��Ȧʹ��
    IPC_CONTROL_CMD_GET_DCIRIS_AUTO_STATE = 147,//  ��ȡ�Զ�DC��Ȧʹ�ܱ�־
    IPC_CONTROL_CMD_ZOOM_DCIRIS = 148,          //   �Ŵ�DC��Ȧ
    IPC_CONTROL_CMD_SHRINK_DCIRIS = 149,        //  ��СDC��Ȧ
    IPC_CONTROL_CMD_SET_FILTER_STATE = 150,     //  �˹�Ƭ�л�
    IPC_CONTROL_CMD_SET_AC_SYNC_STATE = 151,    //  ���õ���ͬ��ʹ��
    IPC_CONTROL_CMD_GET_AC_SYNC_STATE = 152,    //  ��ȡ����ͬ��ʹ��
    IPC_CONTROL_CMD_SET_AC_SYNC_DELAY = 153,    //  ���õ���ͬ����ʱ
    IPC_CONTROL_CMD_GET_AC_SYNC_DELAY = 154,    //  ��ȡ����ͬ����ʱֵ

    IPC_CONTROL_CMD_SET_IO_ARG = 156,// ���ö������IO�ڲ���
    IPC_CONTROL_CMD_GET_IO_ARG = 157,// ��ȡ�������IO�ڲ���

    IPC_CONTROL_CMD_SET_EDGE_MOD = 158,// ���ô���ץ���ع���ģʽ
    IPC_CONTROL_CMD_GET_EDGE_MOD = 159,// ��ȡ����ץ���ع���ģʽ

//  ����ɼ�����
    IPC_CONTROL_CMD_GET_H264_RES = 160,         //  ��ȡH264�ֱ���
    IPC_CONTROL_CMD_SET_GRAY_IMAGE_STATE = 161, //  ���úڰ�ͼʹ��
    IPC_CONTROL_CMD_GET_GRAY_IMAGE_STATE = 162, //  ��ȡ�ڰ�ͼʹ�ܱ�־
//  �������
    IPC_CONTROL_CMD_SET_H264_BITRATE = 163,     //  ����H264����
    IPC_CONTROL_CMD_GET_H264_BITRATE = 164,     //  ��ȡH264����
    IPC_CONTROL_CMD_SET_H264_BITRATE2 = 165,     //  ���õڶ�·H264����
    IPC_CONTROL_CMD_GET_H264_BITRATE2 = 166,     //  ��ȡ�ڶ�·H264����
    IPC_CONTROL_CMD_SET_H264_IFRAME_INTERVAL = 167,    //  ����H264 I֡���
    IPC_CONTROL_CMD_GET_H264_IFRAME_INTERVAL = 168,    //  ��ȡH264 I֡���
    IPC_CONTROL_CMD_SET_JPEG_QUALITY = 169,     //  ������Ƶ��JPEGѹ����
    IPC_CONTROL_CMD_GET_JPEG_QUALITY = 170,     //  ��ȡ��Ƶ��JPEGѹ����
    IPC_CONTROL_CMD_SET_CVBS_STD = 171,         //  ����CVBS��ʽ
    IPC_CONTROL_CMD_GET_CVBS_STD = 172,         //  ��ȡCVBS��ʽ
//  DSP����
    IPC_CONTROL_CMD_SET_VIDEO_STREAM_RECOGNIZE_PARAMS = 173,  //  ��Ƶ��ʶ�������ʼ��
    IPC_CONTROL_CMD_SET_PICTURE_SNAP_RECOGNIZE_PARAMS = 174,  //  ץ��ͼʶ�������ʼ��

    IPC_CONTROL_CMD_SET_SCENE=175,//�����龰ģʽ
	
//  ����
    IPC_CONTROL_CMD_SET_FPGA_REG = 176,         // ����fpga�Ĵ���
    IPC_CONTROL_CMD_GET_FPGA_REG = 177,         // ��ȡfpga�Ĵ���

    IPC_CONTROL_CMD_SET_WORKMODE = 178,

    IPC_CONTROL_CMD_SOFT_CAPTURE = 179,

    IPC_CONTROL_CMD_SET_FLASH_GATE=180,

    IPC_CONTROL_CMD_GET_VERSION = 181,

    IPC_CONTROL_CMD_SET_CAP_SHUTTER = 182,
    IPC_CONTROL_CMD_SET_CAP_GAIN = 183,
    IPC_CONTROL_CMD_SET_CAP_SHARPEN = 184,
    IPC_CONTROL_CMD_SET_CAP_RGB = 185,
    
    IPC_CONTROL_CMD_GET_DAY_NIGHT = 186,
    IPC_CONTROL_CMD_SET_BACKLIGHT = 187,//�ݲ�ʵ��
    IPC_CONTROL_CMD_SET_AWB_MODE = 188,
    
    IPC_CONTROL_CMD_SET_H264_IFRAME_INTERVAL2 = 189,
    IPC_CONTROL_CMD_GET_H264_IFRAME_INTERVAL2 = 190,

    IPC_CONTROL_CMD_SET_RELAY_STATE = 191,     //  �̵�������

    IPC_CONTROL_CMD_GET_CAMERA_FPS = 192,

    IPC_CONTROL_CMD_END
};

typedef struct {
    int nAddr;
    int nValue;
}fpgaRegSet;

typedef enum IPC_ShareRegion_e
{
    IPC_SHARE_REGION_1 = 1,
    IPC_SHARE_REGION_2 
}IPC_SHARE_REGION;

typedef struct IPC_SHAREREGION_STATUS_t
{
    unsigned int TotalSize;
    unsigned int TotalFreeSize;
    unsigned int LargestFreeSize;
}IPC_SHAREREGION_STATUS;

/* ����ֵ��
 * 0���ɹ�
 * -1��һ���Դ��󣬿��������������������Ĵ��󣬿����Ǵ��䵽m3����
 * -2��Э��������󣬱��磬���������Χֵ��[0~10]�����ڴ˷�Χ����-2
 * ע��Ϊ���ִ���һ���ԣ���ʹ�����������ҲҪ����һ���ǿյ�pPamram������Ϊint����
 */
int IPC_Control(int nCmd, void *pParam, int nSize);

int IPC_GetFullVpssM3Frames(VIDEO_FRAMEBUF_LIST_S * pFrameBufList);
int IPC_PutEmptyVpssM3Frames(VIDEO_FRAMEBUF_LIST_S *pFrameBufList);

int IPC_PutFullVpssM3Frames(VIDEO_FRAMEBUF_LIST_S * pFrameBufList);
int IPC_GetEmptyVpssM3Frames(VIDEO_FRAMEBUF_LIST_S *pFrameBufList);

int IPC_GetFullVideoM3Frames(VIDEO_FRAMEBUF_LIST_S * pFrameBufList);
int IPC_PutEmptyVideoM3Frames(VIDEO_FRAMEBUF_LIST_S *pFrameBufList);

int IPC_PutFullDspFrames(VIDEO_FRAMEBUF_LIST_S *pFrameBufList);
int IPC_GetEmptyDspFrames(VIDEO_FRAMEBUF_LIST_S *pFrameBufList);
int IPC_GetFullDspData(VIDEO_FRAMEBUF_LIST_S *pFrameBufList);
int IPC_PutEmptyDspData(VIDEO_FRAMEBUF_LIST_S *pFrameBufList);

int IPC_PutFullVideoM3Frames(VIDEO_FRAMEBUF_LIST_S *pFrameBufList);
int IPC_GetEmptyVideoM3Frames(VIDEO_FRAMEBUF_LIST_S *pFrameBufList);
int IPC_GetFullVideoM3Bits(VCODEC_BITSBUF_LIST_S* pBitsBufList);
int IPC_PutEmptyVideoM3Bits(VCODEC_BITSBUF_LIST_S* pBitsBufList);

int IPC_GetEmptyVideoM3Bits(VCODEC_BITSBUF_LIST_S* pBitsBufList);
int IPC_PutFullVideoM3Bits(VCODEC_BITSBUF_LIST_S* pBitsBufList);

void IPC_FramesPrintInfo(VIDEO_FRAMEBUF_S *buf);
void IPC_FramesPrintListInfo(VIDEO_FRAMEBUF_LIST_S *bufList,
        char *listName);

void IPC_BitsPrintInfo(VCODEC_BITSBUF_S *buf);
void IPC_BitsPrintListInfo(VCODEC_BITSBUF_LIST_S *bufList,
        char *listName);

void IPC_printDetailedStatistics(void);
void IPC_printBufferStatistics(void);

/*��ȡvnf Link ʣ��buffer������-1��ʾʧ��*/
Int32 IPC_GetVnfEmptyBufferNum(void);

void IPC_Cache_WbInv(void* blockPtr, unsigned int byteCnt, unsigned short type, unsigned short wait);
void IPC_Cache_Inv(void* blockPtr, unsigned int byteCnt, unsigned short type, unsigned short wait);
void IPC_Cache_Wb(void* blockPtr, unsigned int byteCnt, unsigned short type, unsigned short wait);

/*ע��:   ShareRegion ��ؽӿڣ���Ҫ�ڵ���IPC_Start�ӿں�������ã��ڵ���IPC_Stop֮ǰҪ�ͷ�����ڴ�*/
void *IPC_ShareRegion_Malloc(IPC_SHARE_REGION sr_index,int size);

void IPC_ShareRegion_Free(IPC_SHARE_REGION sr_index,void *ptr,int size);

void IPC_ShareRegion_Get_status(IPC_SHARE_REGION sr_index,IPC_SHAREREGION_STATUS *sr_status);

int IPC_Enc_Set_TargetBitrate(int chId,int targetBitrate);

int IPC_Enc_Set_IframeIntervel(int chId,int Intervel);

int IPC_Enc_Set_RateControl(int chId,int rate_control);/*0 - VBR; 1 - CBR*/

int IPC_Enc_Set_VbrDuration(int chId,int Duration);/*1 - 3600*/

int IPC_Enc_Set_VbrSensitivity(int chId,int Sensitivity);/*1 - 8*/

int IPC_Enc_Get_TargetBitrate(int chId,int *targetBitrate);
    
int IPC_Enc_Get_IframeIntervel(int chId,int *intervel);

#ifdef __cplusplus
}
#endif

#endif /* ARCH_H_ */
