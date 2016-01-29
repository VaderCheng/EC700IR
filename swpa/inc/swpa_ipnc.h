#ifndef __SWPA_IPNC_H__
#define __SWPA_IPNC_H__
#ifdef __cplusplus
extern "C"
#ifndef DEFAULT
#define DEFAULT(val) =val
#endif
{
#endif
enum
{
	  SWPA_LINK_ALL
	, SWPA_LINK_A8
	, SWPA_LINK_VPSS
	, SWPA_LINK_VIDEO
	, SWPA_LINK_DSP
	, SWPA_LINK_MAX
};

enum
{
	  SWPA_IMAGE_NONE 
	, SWPA_IMAGE_YUV  
	, SWPA_IMAGE_H264 
	, SWPA_IMAGE_JPEG 
};

enum {
    SWPA_VIDEO_RES_1080P = 0,
    SWPA_VIDEO_RES_720P,
    SWPA_VIDEO_RES_540P,
    SWPA_VIDEO_RES_576P,
    SWPA_VIDEO_RES_480P
};

enum
{
	SWPA_VPSS_H264_CHANNEL = 0,
	SWPA_VPSS_JPEG_CHANNEL = 1,
	SWPA_VPSS_H264_SECOND_CHANNEL = 2,
	SWPA_VPSS_JPEG_SECOND_CHANNEL= 2,	//δ��������ͨ����H264�ڶ�·ʹ��ͬһ��ͨ����
	SWPA_VPSS_CHANNEL_MAX
};


#define CALLBACK_TYPE_IMAGE 0
#define CALLBACK_TYPE_DATA  1

typedef struct tagIPNC_DATA
{
	int   size; //image size
	void* addr[3];	//image virtual address
	void* phys[3];
	void* appData; 
	int   dataSize;
}IPNC_DATA;

typedef struct tagIMAGE
{
	int channel;
	int type;		//0:yuv,1:jpeg,2:h264
	int width;  //image width
	int height; //image height
	int pitch;  //image pitch
	int frameType; //0:I,1:P:2:B,only to h264
	int shutter;  //�ع�ʱ��
	int gain;     //���� 
	int rGain;    //��ƽ���rֵ
	int gGain;    //��ƽ���gֵ
	int bGain;    //��ƽ���bֵ
	int isCapture;//�Ƿ�ץ��ͼ��־	
	int iAvgY; // ƽ������
	int iRawAvgY; //ISP
	int iCaptureEdgeExt; //����������״̬ 0-�޴�����,1-�½���,2-������,3-�½��ػ���������
	int iCaptureFlag;	//ץ�ļ���
	int iCaptureEnableFlag;	//ץ��ʹ�ܱ�־λ�� 0λ���� 1λ���� 2λ�ڵ�ƽ 3λRGB
	int iCaptureShutter;	//ץ�Ŀ���
	int iCaptureGain;		//ץ������
	int iCaptureRGain;		//����R����
	int iCaptureGGain;		//����H����
	int iCaptureBGain;		//����B����
	IPNC_DATA data;	
	unsigned int uTimeStamp; //FPGA�ɼ�ʱ��
}IMAGE;

enum 
{
		//  �����������
    CMD_SET_EXP = 100,        //  �����ع�ʱ��
    CMD_GET_EXP,            //  ��ȡ�ع�ʱ��
    CMD_SET_GAIN,           //  ��������
    CMD_GET_GAIN,           //  ��ȡ����
    CMD_SET_RGB,            //  ����RGB
    CMD_GET_RGB,            //  ��ȡRGB
		//  ����Զ�������
    CMD_SET_AE_STATE,       //  ����AEʹ��
    CMD_GET_AE_STATE,       //  ��ȡAEʹ�ܱ�־
    CMD_SET_AE_THRESHOLD,   //  ����AE����ֵ
    CMD_GET_AE_THRESHOLD,   //  ��ȡAE��ǰ����ֵ
    CMD_SET_AE_EXP_RANGE,   //  ����AE�ع�ʱ�䷶Χ
    CMD_GET_AE_EXP_RANGE,   //  ��ȡAE�ع�ʱ�䷶Χ
    CMD_SET_AE_ZONE,        //  ����AE�������
    CMD_GET_AE_ZONE,        //  ��ȡAE�������
    CMD_SET_AE_GAIN_RANGE,  //  ����AE���淶Χ
    CMD_GET_AE_GAIN_RANGE,  //  ��ȡAE���淶Χ
    CMD_SET_SCENE,		//
    //CMD_GET_SCENE,		//
    CMD_SET_AWB_STATE,      //  ����AWBʹ��
    CMD_GET_AWB_STATE,      //  ��ȡAWBʹ��
//  ���ͼ����
    CMD_SET_WDR_STRENGTH,   //  ����WDRǿ��ֵ
    CMD_GET_WDR_STRENGTH,   //  ��ȡWDRǿ��ֵ
    CMD_SET_SHARPNESS_STATE,//  ������ʹ��
    CMD_GET_SHARPNESS_STATE,//  ��ȡ��ʹ��
    CMD_SET_SHARPNESS,      //  ������ֵ
    CMD_GET_SHARPNESS,      //  ��ȡ��ֵ
    CMD_SET_SATURATION_CONTRAST_STATE,  //  ���ñ��Ͷȡ��Աȶ�ʹ��
    CMD_GET_SATURATION_CONTRAST_STATE,  //  ��ȡ���Ͷȡ��Աȶ�ʹ��
    CMD_SET_SATURATION,     //  ���ñ��Ͷ�ֵ
    CMD_GET_SATURATION,     //  ��ȡ���Ͷ�ֵ
    CMD_SET_CONTRAST,       //  ���öԱȶ�ֵ
    CMD_GET_CONTRAST,       //  ��ȡ�Աȶ�ֵ
    CMD_SET_TNF_STATE,//  ���ý���TNFʹ��
    CMD_GET_TNF_STATE,//  ��ȡ����TNFʹ��
    CMD_SET_SNF_STATE,//  ���ý���SNFʹ��
    CMD_GET_SNF_STATE,//  ��ȡ����SNFʹ��
    CMD_SET_TNF_SNF_STRENGTH,   //  ���ý���ǿ��ֵ
    CMD_GET_TNF_SNF_STRENGTH,   //  ��ȡ����ǿ��ֵ
    CMD_SET_TRAFFIC_LIGTH_ENHANCE_STATE,    //  ���ú�ƼӺ�ʹ��
    CMD_GET_TRAFFIC_LIGTH_ENHANCE_STATE,    //  ��ȡ��ƼӺ�ʹ��
    CMD_SET_TRAFFIC_LIGTH_ENHANCE_REGION,   //  ���üӺ������޶�ȡ���
    CMD_SET_TRAFFIC_LIGTH_LUM_TH,    // ���ú�ƼӺ�������ֵ
    CMD_SET_GAMMA_ENABLE,  // ����Gamma ʹ��
    CMD_GET_GAMMA_ENABLE,  // ��ȡGammaʹ��
    CMD_SET_GAMMA,  // ����Gamma
    CMD_GET_GAMMA,  // ��ȡGamma
    CMD_SET_EDGE_ENHANCE, // ����ͼ���Ե��ǿ
    CMD_GET_EDGE_ENHANCE, //����
//  ���IO����
    CMD_SET_DCIRIS_AUTO_STATE,   //  �����Զ�DC��Ȧʹ��
    CMD_GET_DCIRIS_AUTO_STATE,   //  ��ȡ�Զ�DC��Ȧʹ�ܱ�־
    CMD_ZOOM_DCIRIS,   //   �Ŵ�DC��Ȧ
    CMD_SHRINK_DCIRIS,   //  ��СDC��Ȧ
    CMD_SET_FILTER_STATE,   //  �˹�Ƭ�л�
    CMD_SET_RELAY_STATE,    //  �̵���ͨ�Ͽ���
    CMD_SET_AC_SYNC_STATE,  //  ���õ���ͬ��ʹ��
    CMD_GET_AC_SYNC_STATE,  //  ��ȡ����ͬ��ʹ��
    CMD_SET_AC_SYNC_DELAY,  //  ���õ���ͬ����ʱ
    CMD_GET_AC_SYNC_DELAY,  //  ��ȡ����ͬ����ʱֵ

	CMD_SET_IO_ARG,			// ���ö������IO�ڲ���
    CMD_GET_IO_ARG,			// ��ȡ�������IO�ڲ���
		
	CMD_SET_EDGE_MOD,		// ���ô���ץ���ع���ģʽ
	CMD_GET_EDGE_MOD,		// ��ȡ����ץ���ع���ģʽ
	
		//  ����ɼ�����
    CMD_GET_H264_RES,       //  ��ȡH264�ֱ���
    CMD_SET_GRAY_IMAGE_STATE,   //  ���úڰ�ͼʹ��
    CMD_GET_GRAY_IMAGE_STATE,   //  ��ȡ�ڰ�ͼʹ�ܱ�־
		//  �������
    CMD_SET_H264_BITRATE,   //  ����H264����
    CMD_GET_H264_BITRATE,   //  ��ȡH264����
    CMD_SET_H264_BITRATE2,	//	���õڶ�·H264����
    CMD_GET_H264_BITRATE2,	//	��ȡ�ڶ�·H264����
    CMD_SET_H264_IFRAME_INTERVAL,    //  ����H264 I֡���
    CMD_GET_H264_IFRAME_INTERVAL,    //  ����H264 I֡���
    CMD_SET_JPEG_QUALITY,   //  ������Ƶ��JPEGѹ����
    CMD_GET_JPEG_QUALITY,   //  ������Ƶ��JPEGѹ����
    CMD_SET_CVBS_STD,       //  ����CVBS��ʽ
    CMD_GET_CVBS_STD,       //  ��ȡCVBS��ʽ
		//  DSP����
    CMD_SET_VIDEO_STREAM_RECOGNIZE_PARAMS,  //  ��Ƶ��ʶ�������ʼ��
    CMD_SET_PICTURE_SNAP_RECOGNIZE_PARAMS,  //  ץ��ͼʶ�������ʼ��

		//  ����
    CMD_SET_FPGA_REG,
    CMD_GET_FPGA_REG,

	//�������ģʽ
	CMD_SET_WORKMODE,

	CMD_SOFT_CAPTURE,

	CMD_SET_FLASH_GATE,
	
	CMD_GET_VERSION,

	CMD_SET_CAP_SHUTTER,
	CMD_SET_CAP_GAIN,
	CMD_SET_CAP_SHARPEN,
	CMD_SET_CAP_RGB,

	CMD_GET_DAY_NIGHT,
	CMD_SET_BACKLIGHT,
	
	CMD_SET_AWB_MODE,

	CMD_SET_H264_IFRAME_INTERVAL2,
    CMD_GET_H264_IFRAME_INTERVAL2,

    CMD_END
};

enum 
{
    IMX178,
    IMX185,
    IMX185_DUALVIDEO,
    IMX185_CVBSEXPORT,
    IMX178_TEST,
    IMX185_TEST,
    IMX136,
    IMX172,
    JPEG_DEC,
	IMX178_WITHOUTCVBS,
	IMX185_DUALVIDEO_CVBSEXPORT,
	IMX178_WITHOUT_NSF,
	DOME_CAMERA,
	IMX226,
	IMX249,
	IMX185_CROP_WITHOUTCVBS,
	IMX249_H264_VNF,
	ICX816,			//680w
	ICX816_640W,
	ICX816_600W,
	ICX816_SINGLE,
    IMX_MAX
};

enum
{
	SENSOR_IMX178,
    SENSOR_IMX185,
    SENSOR_IMX174,
    SENSOR_IMX249,
    SENSOR_ICX816,
    SENSOR_MAX
};

typedef struct tagIPNCCreateConfig {
	int imx;							//ǰ������
	int videoConfig;					//��һ·H264�ֱ���(imxΪIMX185_DUALVIDEO��Ч)
	int videoConfigSecond;				//�ڶ�·H264�ֱ���(imxΪIMX185_DUALVIDEO��Ч)
	int secondVideoFrameRate;			//�ڶ�·H264֡��(imxΪIMX185_DUALVIDEO��Ч)
	int verbose;						//��־������أ�Ĭ�Ϲر�
	int maxBitRate;						//��һ·�������
	int secondMaxBitRate;				//�ڶ�·�������
}IPNCCreateConfig;

/**
 *@brief ���ý������ݻص�����
 *@param      pContext �ص��������豸������
 *@param      pData    �ص�����ָ��
 *@param      nLen     ���ݳ���
 */
typedef void (*metadata_callback_func)(void *pContext, void *pData, int nLen);

/**
   @brief IPNC��������
   @param [IN] ipnc_create_config ����ʱ�����ò���
*/
int swpa_ipnc_create(IPNCCreateConfig *ipnc_create_config);


/**
 *@brief ���ý������ݻص�����
 *@param [in] link_id IPCLink��id��
 *@param [in] OnResult �ص�����,�ɹ�����0��ʧ�ܷ���-1
 *@param      pContext �ص��������豸������
 *@param      type     �ص�������������
 *@param               1:ͼƬ����
 *@param      struct_ptr �ṹ��ָ��
 */
void swpa_ipnc_setcallback(
	int link_id
	, int channel_id
	, int (*OnResult)(void *pContext, int type, void *struct_ptr)
	, void *pContext
	);

/**
 *@brief ���ø������ݽ������ݻص�����
 *@param [in] callbackfunc �ص�����,�ɹ�����0��ʧ�ܷ���-1
 *@param      pContext 		�ص��������豸������
 */
void swpa_ipnc_setmetadatacallback(
	metadata_callback_func callbackfunc
	, void *pContext
	);

//0:185,1:178
int swpa_ipnc_mode(void);
int swpa_get_sensor_type(void);
int swpa_ipnc_start(void);
void swpa_ipnc_release(void *addr, int fphys DEFAULT(0));
void* swpa_ipnc_get_app_ptr(void *addr);
int swpa_ipnc_get_app_size(void *addr);
int swpa_ipnc_resample(void *addr, void**y DEFAULT(0), void**uv DEFAULT(0));
int swpa_ipnc_Crop(void *addr,
					int left, int top, int right, int bottom,
				    int* piDetWidth, int* piDetHeight);
// ���ڽ�ͼ֮��� YUV ͼͼ��Ļ�ԭ
int swpa_ipnc_Crop_reset(void *addr,
                    int left, int top, int right, int bottom,
                    int iResetWidth, int iResetHeight,
                    int* piDetWidth, int* piDetHeight);
int swpa_ipnc_scale_up(void *addr, int up, int offset, int *pnewheight);
int swpa_ipnc_send_data (int link_id, void *addr, int size, void *appData DEFAULT(0), int dataSize DEFAULT(0));

int swpa_ipnc_get_vnf_frame(void *reg_frame_addr, IMAGE *vnf_image_info, int image_ts);


int swpa_ipnc_control(int link_id, int cmd, void *pPrm, int prmSize, int timeout DEFAULT(-1));
void swpa_ipnc_close(void);
void swpa_ipnc_cache_wbinv(void* blockPtr, unsigned int byteCnt, unsigned short type, unsigned short wait);
void swpa_ipnc_cache_inv(void* blockPtr, unsigned int byteCnt, unsigned short type, unsigned short wait);
void swpa_ipnc_cache_wb(void* blockPtr, unsigned int byteCnt, unsigned short type, unsigned short wait);
int  swpa_ipnc_add_jpg(void* buffer, int size);

void swpa_ipnc_printDetailedStatistics(void);
void swpa_ipnc_printBufferStatistics(void);
//M3�ڴ湲������ӿ�
void* swpa_ipnc_share_region_malloc(int index, int size);
void swpa_ipnc_share_region_free(int index, void* ptr, int size);
void swpa_ipnc_share_region_get_total_size(int *size);
void swpa_ipnc_share_region_get_free_size(int *size);
//M3ʵʱ����H264�ӿ�
int swpa_ipnc_set_target_bitrate(int chID, int target_bitrate);       //Ŀ������
int swpa_ipnc_set_iframe_interval(int chID, int interval);			//I֡���
int swpa_ipnc_set_rate_control(int chID, int rate_control);			//���ʿ��� 0:VBR 1:CBR
int swpa_ipnc_set_vbr_duration(int chID, int duration);				//�����仯����ʱ��
int swpa_ipnc_set_vbr_sensitivity(int chID, int sensitivity);		//�����仯������
int swpa_ipnc_get_target_bitrate(int chID, int *target_bitrate);
int swpa_ipnc_get_iframe_interval(int chID, int *interval);

#ifdef __cplusplus
}
#endif
#endif
