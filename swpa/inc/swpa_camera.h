/**  
* @file  swpa_camera.h   
* @brief  ������
* @author luoqz@signalway.com.cn
* @date  2013-2-21
* @version  V0.1
* @note �����ڲ�ʵ�ֵľ�����ӿڲ�һ�������һЩ�ӿڻ�ȡ����ֵ������ʱ�����ֵ������ƫ����ǻ�ȡ����ֵ��ʵ����Чֵ������ƫ��
* @see ��ˮ��ƽ̨FPGA�Ĵ���˵���ĵ���
*
* @log 2014-07 lijj&lujy
*       �¼�ľ��ƽ̨��������ơ���о���Ƶ�API�ӿڡ�
*       1�����ʹ�ô���ttyS4ͨ�ţ���оʹ��ttyS3ͨ�ţ���ʹ�ñ��ļ�APIʱ��
*          �������swpa_camera_init��swpa_camera_deinit���г�ʼ�����˳�������
*          ���ڴ��ڲ��ǹ����豸�����������ģ��ͽ�����ʹ�ã���Ҫ������������������
*       2��ʹ�÷�ʽ��
          swpa_camera_init();   // �ȳ�ʼ��
          // ��������API
          swpa_camera_init_lens();  // ��ʼ����ͷ
          swpa_camera_ball_set_pt_coordinates(100, 100); // �������λ��(100,100)����(PT����)
          swpa_camera_basicparam_set_zoom_value(1000);   // ���ñ䱶��ֵ(Z)
          // 
          swpa_camera_deinit(); // �˳�
*/

/**
 @todo
��ֱת����Χ���������֧��˵���Դﵽ-10~90����Ŀǰ�õ��������֧�֣�
ֻ�ﵽ0~90��Ŀǰ�ĳ���ʵ��ֵ��ע�͡�

���⹦��ʹ�ñ䱶��ʵ�֣����泧�ҿ��ܿ���Ԥ��λ���õķ�ʽ������ʹ�ñ䱶��
*/
 
////����ֵ���� TODO: �Բ�ʱ�ã���ʽ����ʱֱ������swpa.h����
/////�ɹ�
//#define SWPAR_OK 				( 0)
/////ʧ��
//#define SWPAR_FAIL 				(-1)
/////�Ƿ�����
//#define SWPAR_INVALIDARG		(-2)
/////û��ʵ��
//#define SWPAR_NOTIMPL			(-3)
/////�ڴ治��
//#define SWPAR_OUTOFMEMORY 		(-4)
/////û�г�ʼ��
//#define SWPAR_NOTINITED			(-5)

#ifndef _SWPA_CAMERA_H_
#define _SWPA_CAMERA_H_

#ifdef __cplusplus
extern "C"
{
#endif

typedef enum PROTOCOL_TYPE_e
{
    PROTOCOL_VISCA = 0,
    PROTOCOL_PELCO
}PROTOCOL_TYPE;

/*
* �������ģʽ
*/
// AEģʽ
typedef enum AE_MODE_e
{
  AE_FULL_AUTO = 0x0,   // note�������ֵ��Ҫ����Ķ���
  AE_MANUAL = 0x3,
  AE_SHUTTER_PRIORITY = 0xa,
  AE_IRIS_PRIORITY = 0xb
}AE_MODE;

// AFģʽ
typedef enum AF_MODE_e
{
  AF_AUTO = 0x2,    // note�������ֵ��Ҫ����Ķ���
  AF_MANUAL = 0x3
}AF_MODE;

// �Խ�ģʽ
typedef enum FOCUS_MODE_e
{
    FOCUS_NEAR = 0,  // ����
    FOCUS_FAR,       // Զ��
    FOCUS_ONE_PUSH,  // �����Խ�
    FOCUS_INFINITY, // ����Զ�Խ�
    FOCUS_STOP
}FOCUS_MODE;

// ��ƽ��ģʽ
typedef enum AWB_MODE_e
{
  AWB_AUTO = 0,
  AWB_INDOOR,
  AWB_OUTDOR,
  AWB_ONE_PUSH_WB,
  AWB_ATW,
  AWB_MANUAL,
  AWB_OUTDOOR_AUTO,
  AWB_SODIUM_LAMP_AUTO,
  AWB_SODIUM_LAMP
}AWB_MODE;

// WDRģʽ
typedef enum WDR_MODE_e
{
	WDR_ON = 0x2,   // note�������ֵ��Ҫ����Ķ���
	WDR_OFF = 0x3
}WDR_MODE;

// �佹ģʽ
typedef enum ZOOM_MODE_e
{
	ZOOM_TELE = 0,
	ZOOM_WIDE,
    ZOOM_STOP,
}ZOOM_MODE;

// ��Ȧģʽ
typedef enum IRIS_MODE_e
{
	IRIS_RST = 0,   // ��λ
	IRIS_UP,        // �Ŵ�
    IRIS_DOWN,      // ��С
}IRIS_MODE;

// ��о���ģʽ
typedef enum MONITOR_MODE_e
{
	MODE_1080I_60 = 0x01, // ���֡��Ϊ30fps����Ϊ��оĬ��ֵ(ľ�ǲ����ã�)
    MODE_1080P_30 = 0x06, // ľ��ʹ��
    MODE_1080P_25 = 0x08, // ľ��ʹ��
    MODE_720P_60 = 0x09,
    MODE_720P_30 = 0x0e,
    MODE_720P_25 = 0x11,
}MONITOR_MODE;

// ˮƽת��ģʽ
typedef enum PAN_TYPE_t
{
    PAN_LEFT = 0,
    PAN_RIGHT,
}PAN_TYPE;

// ��ֱת��ģʽ
typedef enum TILT_TYPE_t
{
    TILT_DOWN = 0,
    TILT_UP,
}TILT_TYPE;

// Ԥ��λ����
typedef enum PRESET_TYPE_t
{
    PRESET_SET = 0,
    PRESET_CLEAR,
    PRESET_CALL,
}PRESET_TYPE;

// ɨ��߽�
typedef enum SCAN_BOUND_t
{
    SCAN_BOUND_LEFT = 0,
    SCAN_BOUND_RIGHT,
}SCAN_BOUND;

/**
***********************
* �����ʼ�� 
* ע����ʼ����ؽӿڣ�ֻ��ľ��ƽ̨����
***********************
*/


/**
* @brief ��ʼ���������\n
* @retval 0 : ִ�гɹ�
* @retval -1 : ִ��ʧ��
* @note ���ʹ�ô���ttyS4ͨ�ţ���оʹ��ttyS3ͨ�ţ��ú����Դ��ڽ��г�ʼ����
*       ��������Ҫ������������������豸Ҫʹ��ttyS3��ttyS4���������ݣ��벻Ҫ���ñ�����������
*/
int swpa_camera_init(void);

/**
* @brief ȥ��ʼ���������\n
* @retval 0 : ִ�гɹ�
* @retval -1 : ִ��ʧ��
* @note ���ʹ�ô���ttyS4ͨ�ţ���оʹ��ttyS3ͨ�ţ��ú����Դ��ڽ��йرղ�����
*       ��������Ҫ�����������swpa_camara_init����ʹ��
*/
int swpa_camera_deinit(void);

/////////////////////////////////////////////////////////////////////////////
/**
* @brief ���������/�ϵ�\n
*
* @param [in] power 0-�ϵ�(standbyģʽ) 1-�ϵ�
* @retval 0 : ִ�гɹ�
* @retval -1 : ִ��ʧ��
*/
int swpa_camera_set_power(int power);

/**
* @brief ��ȡ�����/�ϵ�״̬\n
*
* @param [out] power 0-�ϵ�(standbyģʽ) 1-�ϵ� -1��������
* @retval 0 : ִ�гɹ�
* @retval -1 : ִ��ʧ��
*/
int swpa_camera_get_power(int *power);

/**
* @brief ��ʼ����ͷ\n
*
* @retval 0 : ִ�гɹ�
* @retval -1 : ִ��ʧ��
* @note ����Զ���Խ���Ȼ��Խ�����һ�ε�λ��
*/
int swpa_camera_init_lens(void);

/**
* @brief ��λ���\n
*
* @retval 0 : ִ�гɹ�
* @retval -1 : ִ��ʧ��
* @note ��λʱ��Լ12��(6500�ͺ�)
*/
int swpa_camera_reset(void);

/////////////////////////////////////////////////////////////////////////////
/**
* @brief ��ȡ��о�汾��\n
*
* @param [out] vendor ���� (0x20��ʾ��sony)
* @param [out] model ģ�� (0x045f��ʾFCB-EH6300��0x0446��ʾFCB-EH6500��0x2203��ʾPE2203)
* @param [out] rom_version �̼��汾��
* @retval 0 : ִ�гɹ�
* @retval -1 : ִ��ʧ��
* @note ����ľ�����ֵ�����ַ���
*/
int swpa_camera_get_version(unsigned int *vendor, unsigned int *model, unsigned int *rom_version);

/**
* @brief ���û�о����ֱ��ʡ�֡��\n
*
* @param [in] mode ģʽ���ο�MONITOR_MODE
* @retval 0 : ִ�гɹ�
* @retval -1 : ִ��ʧ��
* @note ���úã�Ҫ����swpa_camera_reset����λ������Ч����ǰ��MODE_1080P_30��MODE_1080P_25����Ч
*/
int swpa_camera_set_monitoring_mode(MONITOR_MODE mode);

/**
* @brief ��ȡ��о����ֱ��ʡ�֡��\n
*
* @param [out] mode ģʽ���ο�MONITOR_MODE
* @retval 0 : ִ�гɹ�
* @retval -1 : ִ��ʧ��
* @note ��ȡ��ǰ��о�ķֱ��ʡ�֡��ģʽ
*/
int swpa_camera_get_monitoring_mode(MONITOR_MODE *mode);

/**
* @brief ���û�оͬ����ʽ\n
*
* @param [in] mode ģʽ��0-sav/eav off - sav/eav on
* @retval 0 : ִ�гɹ�
* @retval -1 : ִ��ʧ��
* @note ����6500�ͺŻ�о�����ֲ��ø�λ���Ҳ��������
*       ľ�Ǳ���ʹ��ģʽ0����sav/eav off
*/
int swpa_camera_set_sync_mode(int mode);

/**
* @brief ��ȡ��оͬ����ʽ\n
*
* @param [out] mode ģʽ��0-sav/eav off - sav/eav on
* @retval 0 : ִ�гɹ�
* @retval -1 : ִ��ʧ��
* @note ʹ�ñ�������ȡͬ����ʽ�������Ϊ0����Ҫ����swpa_camera_set_sync_mode����Ϊ0
*/
int swpa_camera_get_sync_mode(int* mode);

///////////////////////////////////////////////////////////////////////////////
/**
***********************
* �����ص�ת��
* swpa_camera_conver
***********************
*/

/**
* @brief ����ת��\n
*
* 
* @param [in] shutter ����ֵ��ʵ��д��FPGA�����ֵ��
* @retval ����ʵ�ʵĿ���ֵ����λ΢�롣
* @note ľ�ǲ�ʹ��
* @see .
*/
int swpa_camera_conver_shutter(
	int shutter
);

/**
* @brief ����ת��\n
*
* 
* @param [in] gain ����ֵ��ʵ��д��FPGA�����ֵ��
* @retval ����ʵ�ʵ�����ֵ����λ0.1db��
* @note ľ�ǲ�ʹ��
* @see .
*/
int swpa_camera_conver_gain(
	int gain
);

/**
* @brief RGB����ת��\n
* @param [in] rgb_gain rgb����ֵ��ʵ��д��FPGA�����ֵ��
* @retval ����ʵ�ʵ�RGB����ֵ.
* @note ľ�ǲ�ʹ��
* @see .
*/
int swpa_camera_conver_rgb_gain(
	int rgb_gain
	);


/**
***********************
* �����������
* swpa_camera_basicparam_
***********************
*/

/**
* @brief ���ÿ���\n
*
* 
* @param [in] shutter ����ֵ����λ΢�룬200w/280w��Χ0-30000��500w��Χ0-54000��ľ�������Χ0x0 - 0x15
* @retval 0 : ִ�гɹ�
* @retval -1 : ִ��ʧ��
* @note �����ڲ�ʵ�ֵľ��ȱȴ˽ӿڵͣ���˻�ȡ����ֵ������ʱ�����ֵ������ƫ��
*       ľ�ǣ�shutterʵ�����ǿ���ֵ��������ֵԽС��Խ��
* @see .
*/
int swpa_camera_basicparam_set_shutter(int shutter);

/**
* @brief ��ȡ����\n
*
* 
* @param [out] shutter ����ֵ����λ΢�룬200w/280w��Χ0-30000��500w��Χ0-54000
* @retval 0 : ִ�гɹ�
* @retval -1 : ִ��ʧ��
* @note �����ڲ�ʵ�ֵľ��ȱȴ˽ӿڵͣ���˻�ȡ����ֵ������ʱ�����ֵ������ƫ��
* @see .
*/
int swpa_camera_basicparam_get_shutter(int *shutter);

/**
* @brief ��������\n
*
* 
* @param [in] gain ����ֵ����λ0.1db����Χ0-360��ľ�������Χ:0x0 - 0xf
* @retval 0 : ִ�гɹ�
* @retval -1 : ִ��ʧ��
* @note .
* @see swpa_camera_init_set_gain_correction
*/
int swpa_camera_basicparam_set_gain(int gain);

/**
* @brief ��ȡ����\n
*
* 
* @param [out] gain ����ֵ����λ0.1db����Χ0-360
* @retval 0 : ִ�гɹ�
* @retval -1 : ִ��ʧ��
* @note .
* @see .
*/
int swpa_camera_basicparam_get_gain(int *gain);

/**
* @brief ������������ֵ\n
* @param [in] gain ����ֵ��ľ�������Χ:0x04 - 0xf
* @retval 0 : ִ�гɹ�
* @retval -1 : ִ��ʧ��
* @note 0x4 = +6dB 0x5 = +8dB ... 0xf = +28dB
*
*/
int swpa_camera_basicparam_set_gain_limit(int gain);

/**
* @brief ��ȡ��������ֵ\n
* @param [out] gain ����ֵ��ľ�������Χ:0x04 - 0xf
* @retval 0 : ִ�гɹ�
* @retval -1 : ִ��ʧ��
* @note 0x4 = +6dB 0x5 = +8dB ... 0xf = +28dB
*
*/
int swpa_camera_basicparam_get_gain_limit(int *gain);

/**
* @brief ����RGB����\n
*
* 
* @param [in] gain_r R���棬��λ0.07db����Χ0-255��ľ�������Χ:0x00 - 0xff
* @param [in] gain_g G���棬��λ0.07db����Χ0-255��ľ�������������G����!!
* @param [in] gain_b B���棬��λ0.07db����Χ0-255��ľ�������Χ:0x00 - 0xff
* @retval 0 : ִ�гɹ�
* @retval -1 : ִ��ʧ��
* @note .
* @see .
*/
int swpa_camera_basicparam_set_rgb_gain(
	int gain_r,
	int gain_g,
	int gain_b
); 

/**
* @brief ��ȡRGB����\n
*
* 
* @param [in] gain_r R���棬��λ0.07db����Χ0-255
* @param [in] gain_g G���棬��λ0.07db����Χ0-255
* @param [in] gain_b B���棬��λ0.07db����Χ0-255
* @retval 0 : ִ�гɹ�
* @retval -1 : ִ��ʧ��
* @note .
* @see .
*/
int swpa_camera_basicparam_get_rgb_gain(
	int * gain_r,
	int * gain_g,
	int * gain_b
); 

/**
* @brief ���úڵ�ƽ\n
*
* 
* @param [in] value1 ͨ��1��ֵ����Χ0-1023
* @param [in] value2 ͨ��2��ֵ����Χ0-1023
* @param [in] value3 ͨ��3��ֵ����Χ0-1023
* @param [in] value4 ͨ��4��ֵ����Χ0-1023
* @retval 0 : ִ�гɹ�
* @retval -1 : ִ��ʧ��
* @note ��ǰ�豸�߱���ͨ������ͨ��swpa_camera_init_get_ad_count��ѯ
*       ľ�ǲ�ʹ��
* @see .
*/
int swpa_camera_basicparam_set_clamp_level(
	int value1,
	int value2,
	int value3,
	int value4
); 

/**
* @brief ��ȡ�ڵ�ƽ\n
*
* 
* @param [out] value1 ͨ��1��ֵ����Χ0-1023
* @param [out] value2 ͨ��2��ֵ����Χ0-1023
* @param [out] value3 ͨ��3��ֵ����Χ0-1023
* @param [out] value4 ͨ��4��ֵ����Χ0-1023
* @retval 0 : ִ�гɹ�
* @retval -1 : ִ��ʧ��
* @note ��ǰ�豸�߱���ͨ������ͨ��swpa_camera_init_get_ad_count��ѯ
*       ľ�ǲ�ʹ��
* @see .
*/
int swpa_camera_basicparam_get_clamp_level(
	int * value1,
	int * value2,
	int * value3,
	int * value4
); 

/**
* @brief ����ץ�ĵĿ���\n
*
* 
* @param [in] shutter ����ֵ����λ΢�룬200w/280w��Χ0-30000��500w��Χ0-54000��-1��ʾ���ץ��ȡ��ֵͬ
* @retval 0 : ִ�гɹ�
* @retval -1 : ִ��ʧ��
* @note .
* @see .
*/
int swpa_camera_basicparam_set_capture_shutter(
	int shutter
);

/**
* @brief ��ȡץ�ĵĿ���\n
*
* 
* @param [out] shutter ����ֵ����λ΢�룬200w/280w��Χ0-30000��500w��Χ0-54000��-1��ʾ���ץ��ȡ��ֵͬ
* @retval 0 : ִ�гɹ�
* @retval -1 : ִ��ʧ��
* @note .
* @see .
*/
int swpa_camera_basicparam_get_capture_shutter(
	int * shutter
);

/**
* @brief ����ץ�ĵ�����\n
*
* 
* @param [in] gain ����ֵ����λ0.1db����Χ0-360��-1��ʾ���ץ��ȡ��ֵͬ
* @retval 0 : ִ�гɹ�
* @retval -1 : ִ��ʧ��
* @note .
* @see swpa_camera_init_set_capture_gain_correction
*/
int swpa_camera_basicparam_set_capture_gain(
	int gain
);

/**
* @brief ��ȡץ�ĵ�����\n
*
* 
* @param [out] gain ����ֵ����λ0.1db����Χ0-360��-1��ʾ���ץ��ȡ��ֵͬ
* @retval 0 : ִ�гɹ�
* @retval -1 : ִ��ʧ��
* @note .
* @see .
*/
int swpa_camera_basicparam_get_capture_gain(
	int * gain
);

/**
* @brief ����ץ�ĵ�RGB����\n
*
* 
* @param [in] gain_r R���棬��λ0.07db����Χ0-255��-1��ʾ���ץ��ȡ��ֵͬ
* @param [in] gain_g G���棬��λ0.07db����Χ0-255��-1��ʾ���ץ��ȡ��ֵͬ
* @param [in] gain_b B���棬��λ0.07db����Χ0-255��-1��ʾ���ץ��ȡ��ֵͬ
* @retval 0 : ִ�гɹ�
* @retval -1 : ִ��ʧ��
* @retval -2 : �����Ƿ�
* @note RGB��������ֻҪ��һ��Ϊ-1��������Ϊ���ץ��ȡ��ֵͬ
* @see .
*/
int swpa_camera_basicparam_set_capture_rgb_gain(
	int gain_r,
	int gain_g,
	int gain_b
);

/**
* @brief ��ȡץ�ĵ�RGB����\n
*
* 
* @param [in] gain_r R���棬��λ0.07db����Χ0-255��-1��ʾ���ץ��ȡ��ֵͬ
* @param [in] gain_g G���棬��λ0.07db����Χ0-255��-1��ʾ���ץ��ȡ��ֵͬ
* @param [in] gain_b B���棬��λ0.07db����Χ0-255��-1��ʾ���ץ��ȡ��ֵͬ
* @retval 0 : ִ�гɹ�
* @retval -1 : ִ��ʧ��
* @note .
* @see .
*/
int swpa_camera_basicparam_get_capture_rgb_gain(
	int * gain_r,
	int * gain_g,
	int * gain_b
);

/**
* @brief ����ץ�ĺڵ�ƽ\n
*
* 
* @param [in] value1 ͨ��1��ֵ����Χ0-1023
* @param [in] value2 ͨ��2��ֵ����Χ0-1023
* @param [in] value3 ͨ��3��ֵ����Χ0-1023
* @param [in] value4 ͨ��4��ֵ����Χ0-1023
* @retval 0 : ִ�гɹ�
* @retval -1 : ִ��ʧ��
* @note ��ǰ�豸�߱���ͨ������ͨ��swpa_camera_init_get_ad_count��ѯ
* @see .
*/
int swpa_camera_basicparam_set_capture_clamp_level(
	int value1,
	int value2,
	int value3,
	int value4
); 

/**
* @brief ��ȡץ�ĺڵ�ƽ\n
*
* 
* @param [out] value1 ͨ��1��ֵ����Χ0-1023
* @param [out] value2 ͨ��2��ֵ����Χ0-1023
* @param [out] value3 ͨ��3��ֵ����Χ0-1023
* @param [out] value4 ͨ��4��ֵ����Χ0-1023
* @retval 0 : ִ�гɹ�
* @retval -1 : ִ��ʧ��
* @note ��ǰ�豸�߱���ͨ������ͨ��swpa_camera_init_get_ad_count��ѯ
* @see .
*/
int swpa_camera_basicparam_get_capture_clamp_level(
	int * value1,
	int * value2,
	int * value3,
	int * value4
); 

/**
* @brief �����ع�ģʽ\n
* @param [in] mode :AE ģʽ���μ�AE_MODE����
* @retval 
* 0 - �ɹ�
* -1 - ʧ��
* @note ľ��ʹ��
* @see .
*/
int swpa_camera_basicparam_set_AE(AE_MODE mode);

/**
* @brief ��ȡ�ع�ģʽ\n
* @param [out] mode :AE ģʽ���μ�AE_MODE����
* @retval 
* 0 - �ɹ�
* -1 - ʧ��
* @note ľ��ʹ��
* @see .
*/
int swpa_camera_basicparam_get_AE(AE_MODE *mode);

/**
* @brief ���öԽ�ģʽ\n
* @param [in] mode : AF ģʽ���μ�AF_MODE����
* @retval 
* 0 - �ɹ�
* -1 - ʧ��
* @note ľ��ʹ��
* @see .
*/
int swpa_camera_basicparam_set_AF(AF_MODE mode);

/**
* @brief ��ȡ�Խ�ģʽ\n
* @param [out] mode :AF ģʽ���μ�AF_MODE����
* @retval 
* 0 - �ɹ�
* -1 - ʧ��
* @note ľ��ʹ��
* @see .
*/
int swpa_camera_basicparam_get_AF(AF_MODE *mode);

/**
* @brief ���öԽ�ģʽ\n
* @param [in] mode : �Խ�ģʽ
* @retval 
* 0 - �ɹ�
* -1 - ʧ��
* @note ����������Զ���Խ��������ֶ��Խ�ģʽ������
*       ע�⣬�����������������ܼ�����һ���޶�Ӧ�Ļ�ȡģʽ����
*       ľ��ʹ��
*/
int swpa_camera_basicparam_set_focus(FOCUS_MODE mode);

/**
* @brief ���öԽ�ģʽ\n
* @param [int] mode : �Խ�ģʽ
* @param [int] speed: �Խ��ٶ� ��Χ��0x0~0x07
* @retval 
* 0 - �ɹ�
* -1 - ʧ��
* @note ������ֻ����Զ���Խ��������ֶ��Խ�ģʽ������
        ��������swpa_camera_basicparam_set_focus���ƣ��������ٶȿ���
*       ľ��ʹ��
*/
int swpa_camera_basicparam_set_focus_speed(FOCUS_MODE mode, int speed);

/**
* @brief ���öԽ���ֵ\n
* @param [int] value : �Խ���ֵ ��Χ��0x1000 - 0xd000
* @retval 
* 0 - �ɹ�
* -1 - ʧ��
* @note ����������Զ���Խ��������ֶ��Խ�ģʽ������
        0x1000��ʾ���ˣ�0xd000��ʾԶ�ˣ��ֲ���ֵ��Χ��0xc000��ʵ�ʲ���Ϊ0xd000
*       ľ��ʹ��
*/
int swpa_camera_basicparam_set_focus_value(int value);

/**
* @brief ��ȡ�Խ���ֵ\n
* @param [out] value : �Խ���ֵ ��Χ��0x1000 - 0xd000
* @retval 
* 0 - �ɹ�
* -1 - ʧ��
* @note 0x1000��ʾ���ˣ�0xd000��ʾԶ�ˣ��ֲ���ֵ��Χ��0xc000��ʵ�ʲ���Ϊ0xd000
*/
int swpa_camera_basicparam_get_focus_value(int *value);


/**
* @brief �����������ģʽ\n
* @param [in] mode �� ����ģʽ: 0 - Off; 1 - On
* @retval 
* 0 - �ɹ�
* -1 - ʧ��
* @note ľ��ʹ��
*/
int swpa_camera_basicparam_set_infrared_mode(int value);


/**
* @brief �����Զ���ƽ��\n
* @param [in] mode �� ��ƽ��ģʽ
* @retval 
* 0 - �ɹ�
* -1 - ʧ��
* @note ľ��ʹ��
*/
int swpa_camera_basicparam_set_AWB(AWB_MODE mode);

/**
* @brief ��ȡ��ƽ��ģʽ\n
* @param [out] mode :AWB ģʽ
* @retval 
* 0 - �ɹ�
* -1 - ʧ��
* @note
*/
int swpa_camera_basicparam_get_AWB(AWB_MODE *mode);

/**
* @brief ���ù�Ȧģʽ\n
* @param [in] mode
* @retval 
* 0 - �ɹ�
* -1 - ʧ��
* @note *  ��������AEģʽΪ�ֶ����Ȧ��������²��ܵ��ڹ�Ȧ����
*          ��ͬ
*/
int swpa_camera_basicparam_set_iris_mode(IRIS_MODE mode);

/**
* @brief ���ù�Ȧ\n
* @param [in] value
* @retval 
* 0 - �ɹ�
* -1 - ʧ��
* @note ��Ȧֵ��Χ0x0~0x11��ֵԽ�󣬹�ȦԽ��0��ʾ�رչ�Ȧ
*/
int swpa_camera_basicparam_set_iris(int iris);

/**
* @brief ��ȡ��Ȧ\n
* @param [out] value
* @retval 
* 0 - �ɹ�
* -1 - ʧ��
* @note
*/
int swpa_camera_basicparam_get_iris(int *iris);

/**
* @brief ���ñ䱶\n
* @param [in] mode
* @retval 
* 0 - �ɹ�
* -1 - ʧ��
* @note �������ֱ��зŴ���С��ֹͣ�������ܣ�
*       ע�⣬�����������������ܼ�����һ���޶�Ӧ�Ļ�ȡģʽ����
*       ľ��ʹ��
*/
int swpa_camera_basicparam_set_zoom_mode(ZOOM_MODE mode);

/**
* @brief ���ñ䱶���ͼ����ٶ�\n
* @param [in] mode ��ZOOM_MODEö��
* @param [in] speed �ٶȣ�0~7��ֵԽ���ٶ�Խ��
* @retval 
* 0 - �ɹ�
* -1 - ʧ��
* @note ����ͬswpa_camera_basicparam_set_zoom_mode���������ٶȵĿ��ƣ�
        ��������STOP˵��������STOP��Ч��
*       ľ��ʹ��
*/
int swpa_camera_basicparam_set_zoom_speed(ZOOM_MODE mode, int speed);

/**
* @brief ���ñ䱶��ֵ\n
* @param [in] value 0~0x4000
* @retval 
* 0 - �ɹ�
* -1 - ʧ��
* @note ������ֱ�ӵ��ñ䱶����ֵ��ע�⣬ľ���ϲ�ʹ�����ֱ䱶�����ֵΪ0x4000
*       ľ��ʹ��
*/
int swpa_camera_basicparam_set_zoom_value(int value);

/**
* @brief ��ȡ�䱶\n
* @param [out] value 0~0x4000
* @retval 
* 0 - �ɹ�
* -1 - ʧ��
*/
int swpa_camera_basicparam_get_zoom_value(int *value);

/**
* @brief ʹ��/��ֹ���ֱ佹\n
* @param [in] enable 1��ʹ�� 0����ֹ
* @retval 
* 0 - �ɹ�
* -1 - ʧ��
* @note ľ��ʹ��
*/
int swpa_camera_basicparam_dzoom_enable(int enable);

/**
* @brief ����ͼ��\n
* @param [in] mode 0-������ͼ�� 1-����ͼ��
* @note ľ��ʹ��
* @retval 
* 0 - �ɹ�
* -1 - ʧ��
*/
int swpa_camera_basicparam_set_freeze(int mode);

/**
* @brief ��ȡ����ͼ��\n
* @param [in] mode 0-������ͼ�� 1-����ͼ��
* @note ľ��ʹ��
* @retval 
* 0 - �ɹ�
* -1 - ʧ��
*/
int swpa_camera_basicparam_get_freeze(int *mode);

////////////
/**
* @brief ������˽�ڱ������С\n
* @param [in] id ����id�ţ���Χ0~23
* @param [in] half_width  ��˽������һ����ֵ
* @param [in] half_height ��˽����ߵ�һ����ֵ
* @param [in] is_new      �Ƿ������½����� ��Χ��1���½�һ������ 0����������
* @retval 
* 0 - �ɹ�
* -1 - ʧ��
* @note ľ��ʹ��
*       ��������Ҫ�����к���˽�ڱ��йص�������������ܱ����ڻ�о�У�
*             ��о�ϵ�����Ч��Ҫ�ٴν������� ������
*
*       is_new����˵�������Ϊ0����ʾ��ԭ������ID�����ϸ��������С(���겻�ı�)��
*       ���Ϊ1�����ص���ǰͼ�����ĵ㲢��������
*       ע�⣺swpa_camera_basicparam_set_ptz�ǻ�����ID��
*
*/
int swpa_camera_basicparam_set_mask(int id, int half_width, int half_height, int is_new);

/**
* @brief ������˽�ڱ����ꡢ�����С\n
* @param [in] id ����id�ţ���Χ0~23
* @param [in] center_x    ��˽��������ĵ�X����
* @param [in] center_y    ��˽��������ĵ�Y����
* @param [in] half_width  ��˽������һ����ֵ
* @param [in] half_height ��˽����ߵ�һ����ֵ
* 
* @retval 
* 0 - �ɹ�
-1 - ʧ��
* @note ľ��ʹ��
*       ��������Ҫ�����������õ����������ת�����ı䣡����
ʾ��ͼ��

+---------------------------------------------------
|                 |<----half_width*2 --->|
|              -- +----------------------+
|              |  |                      |
|    half_height*2|         .(o_x, o_y)  |
|              |  |                      |
|             -- -+----------------------+
|
+---------------------------------------------------
o_x=center_x
o_y=center_y
*/
int swpa_camera_basicparam_set_nolock_mask(int id, int center_x, int center_y, int half_width, int half_height);

/**
* @brief ��ʾָ��ID�š���ɫ����˽�ڱ�����\n
* @param [in] id ����id�ţ���Χ0~23
* @param [in] color �ڱ��������ɫ����ֵ���������˵��
* @retval 
* 0 - �ɹ�
* -1 - ʧ��
* @note �����ñ�����������ʾ��˽�ڱ�����
*       ���������е���˽������ʾͬһ����ɫ(����ͬ������[����]�ò�ͬ����ɫ)
*
��ɫ����ֵ��
Black   0x00
Gray1   0x01
Gray2   0x02
Gray3   0x03
Gray4   0x04
Gray5   0x05
Gray6   0x06
White   0x07
Red     0x08
Green   0x09
Blue    0x0A
Cyan    0x0B
Yellow  0x0C
Magenta 0x0D
*/
int swpa_camera_basicparam_set_display(int id, int color);

/**
* @brief ��ȡ��˽�ڱ������ID\n
* @param [out] display ����id�ţ�һ��24bit������ID��Ӧdisplay�е�һ������
* @retval 
* 0 - �ɹ�
* -1 - ʧ��
* @note display�ĵ�0λΪ1����ʾIDΪ0��������Ч
*/
int swpa_camera_basicparam_get_display(int *display);

// not used
int swpa_camera_basicparam_get_monitor(int *monitor);

/**
* @brief ɾ����˽�ڱ�����\n
* @param [in] id ����id�ţ���Χ-1~23 ��������Ҫ������idΪ-1ʱ��ʾɾ�����е����� ������
* @retval 
* 0 - �ɹ�
* -1 - ʧ��
* @note Ҫɾ��ĳһ��˽���򣬴���ָ����Ӧ������ID��
*
*/
int swpa_camera_basicparam_clear_display(int id);

/**
* @brief ���û�о��pt\n
* @param [in] pan ˮƽ���� ��Χ-2048~2048
* @param [in] tilt ��ֱ���� ��Χ-1024~1024
* @retval 
* 0 - �ɹ�
* -1 - ʧ��
* @note ��о����pt����ľ���ԼΪ0.088������ֵ��ο�Э���ĵ�
* @todo ʵ���ϵĻ�о��pt�������pt�ǲ�ͬ�ģ����ﻹҪ��һ��ת��
*/
int swpa_camera_basicparam_set_pt(int pan, int tilt);

/**
* @brief ��ȡ��о��pt\n
* @param [out] pan ˮƽ���� ��Χ-2048~2048
* @param [out] tilt ��ֱ���� ��Χ-1024~1024
* @retval 
* 0 - �ɹ�
* -1 - ʧ��
* @note �����ԣ���ȡ����pt�ǲ�׼ȷ�ġ�������ʹ�ô˺���
* @todo ʵ���ϵĻ�о��pt�������pt�ǲ�ͬ�ģ����ﻹҪ��һ��ת��
*/
int swpa_camera_basicparam_get_pt(int *pan, int *tilt);

/**
* @brief ���û�о��˽�ڱ������ptzֵ\n
* @param [in] id   ����id�ţ���Χ0~23
* @param [in] pan  ˮƽ���� ��Χ0~3599(0~359.9��)
* @param [in] tilt ��ֱ���� ��Χ0~900(0~90��)
* @param [in] zoom �佹ֵ����Χ0~31424
* @retval 
* 0 - �ɹ�
* -1 - ʧ��
* @note zoomֵӦ����swpa_camera_basicparam_set_zoom_value����ͬ��
*      ��������ñ���������zoom�����ȡ����zoom��Ĭ��ֵ(0)
* @todo ʵ���ϵĻ�о��pt�������pt�ǲ�ͬ�ģ����ﻹҪ��һ��ת��
*/
int swpa_camera_basicparam_set_ptz(int id, int pan, int tilt, int zoom);

/**
* @brief ��ȡ��о��˽�ڱ������ptzֵ\n
* @param [in] id ����id�ţ���Χ0~23
* @param [out] pan
* @param [out] tilt
* @param [out] zoom
* @retval 
* 0 - �ɹ�
* -1 - ʧ��
* @note  �����ԣ���ȡ����pt�ǲ�׼ȷ�ġ�������ʹ�ô˺���(����ʹ�ò��룬����ȡ���Ĳ��ǲ�����ʽ)
*
*/
int swpa_camera_basicparam_get_ptz(int id, int *pan, int *tilt, int *zoom);


/**
* @brief ���û�о��high resolutionģʽ\n
* @param [in] mode ģʽ����: 0-Off; 1-On
* @retval 
* 0 - �ɹ�
* -1 - ʧ��
* @note ľ��ר��
*/
int swpa_camera_basicparam_set_hrmode(int mode);


/**
* @brief ��ȡ��о��high resolutionģʽ\n
* @param [out] mode ģʽ����: 0-Off; 1-On
* @retval 
* 0 - �ɹ�
* -1 - ʧ��
* @note ľ��ר��
*/
int swpa_camera_basicparam_get_hrmode(int* mode);

/**
* @brief ���û�о���عⲹ��ģʽ\n
* @param [in] mode ģʽ����: 0-Off; 1-On
* @retval 
* 0 - �ɹ�
* -1 - ʧ��
* @note ľ��ר��
*/
int swpa_camera_basicparam_set_expcomp_mode(int mode);

/**
* @brief ��ȡ��о���عⲹ��ģʽ\n
* @param [out] mode ģʽ����: 0-Off; 1-On
* @retval 
* 0 - �ɹ�
* -1 - ʧ��
* @note ľ��ר��
*/
int swpa_camera_basicparam_get_expcomp_mode(int* mode);

/**
* @brief ���û�о���عⲹ��ֵ\n
* @param [in] value ȡֵ��Χ: 0x0 ~ 0xe
* @retval 
* 0 - �ɹ�
* -1 - ʧ��
* @note ľ��ר��
*/
int swpa_camera_basicparam_set_expcomp_value(int value);


/**
* @brief ��ȡ��о���عⲹ��ֵ\n
* @param [out] value ȡֵ��Χ: 0x0 ~ 0xe
* @retval 
* 0 - �ɹ�
* -1 - ʧ��
* @note ľ��ר��
*/
int swpa_camera_basicparam_get_expcomp_value(int* value);



/**
* @brief ���û�о��������\n
* @param [in] value ȡֵ��Χ: 0x0 ~ 0x1
* @retval 
* 0 - �ɹ�
* -1 - ʧ��
* @note ľ��ר��
*/
int swpa_camera_basicparam_set_stabilizer(int value);


// not used
int swpa_camera_basicparam_set_grid(int mode);
//////////////////////////////////////////////////

/**
***********************
* ��������������
* swpa_camera_io_
***********************
*/

/**
* @brief ����ץ��\n
*
* 
* @param [in] flash_set �������������ϣ�Ŀǰ֧��3·����ƣ��ӵ�λ��ʼ����һλ��Ӧ��һ·����ƣ��ڶ�λ��Ӧ�ڶ�·����ơ�����λΪ1���ʾ���ø�·����ƣ�
* @param [in] info ץ����Ϣ����Ӧ��ץ��ͼ����д���Ϣ��������ץ��ͼ�ı�ʶ����Ӳ������֡��Ϊ�ۼӷ�ʽ
* @retval 0 : ִ�гɹ�
* @retval -1 : ִ��ʧ��
* @note ���flash_set���õ���������뵱ǰ���õ�������鲻һ�£�������ʱ��ʱ����flash_set���ã���ɺ��ٻָ�ԭ����ʹ��λ����
* @see .
*/
int swpa_camera_io_soft_capture(
	int flash_set,
	int info
);

// ���ʱ�����㣬���ʱ��������������info��Ϣ����ÿ��Ӳ����ʱ�Զ��ۼ�
int swpa_camera_io_clear_time();

/**
* @brief ����Ӳ����ץ�Ĳ���\n
*
* 
* @param [in] capture_edge_mode1 ��һ·Ӳ����ץ���صĹ���ģʽ��0-�ⲿ�½��ش�����1-�ⲿ�����ش�����2-�������½��ض�������3-������
* @param [in] capture_edge_mode2 �ڶ�·Ӳ����ץ���صĹ���ģʽ��0-�ⲿ�½��ش�����1-�ⲿ�����ش�����2-�������½��ض�������3-������
* @param [in] flash_set_1 ��һ·Ӳ�������������ϣ�Ŀǰ֧��3·����ƣ��ӵ�λ��ʼ����һλ��Ӧ��һ·����ƣ��ڶ�λ��Ӧ�ڶ�·����ơ�����λΪ1���ʾ���ø�·����ƣ�
* @param [in] flash_set_2 �ڶ�·Ӳ�������������ϣ�Ŀǰ֧��3·����ƣ��ӵ�λ��ʼ����һλ��Ӧ��һ·����ƣ��ڶ�λ��Ӧ�ڶ�·����ơ�����λΪ1���ʾ���ø�·����ƣ�
* @retval 0 : ִ�гɹ�
* @retval -1 : ִ��ʧ��
* @note .
* @see .
*/
int swpa_camera_io_set_capture_param(
	int capture_edge_mode1,
	int capture_edge_mode2,
	int flash_set_1,
	int flash_set_2
	);

/**
* @brief ��ȡӲ����ץ�Ĳ���\n
*
* 
* @param [out] capture_edge_mode1 ��һ·Ӳ����ץ���صĹ���ģʽ��0-�ⲿ�½��ش�����1-�ⲿ�����ش�����2-�������½��ض�������3-������
* @param [out] capture_edge_mode2 �ڶ�·Ӳ����ץ���صĹ���ģʽ��0-�ⲿ�½��ش�����1-�ⲿ�����ش�����2-�������½��ض�������3-������
* @param [out] lane_differentiation �Ƿ�ֳ�����0-���ֳ�������·Ӳ��������ʹ��������ƹ�����1-�ֳ�������һ·Ӳ����ֻ��ʹ��һ·����ƹ������ڶ�·Ӳ����ֻ��ʹ�ڶ�·����ƹ���
* @retval 0 : ִ�гɹ�
* @retval -1 : ִ��ʧ��
* @note .
* @see .
*/
int swpa_camera_io_get_capture_param(
	int * capture_edge_mode1,
	int * capture_edge_mode2,
	int * lane_differentiation
);

/**
* @brief ����io����ץ�Ĳ���\n
*
* 
* @param [in] lane_differentiation �Ƿ�ֳ�����0-���ֳ�����ÿ��io��������ʹ��������ƹ�����1-�ֳ�����ÿ��io����ʱָ���ļ�·����ƹ���
* @retval 0 : ִ�гɹ�
* @retval -1 : ִ��ʧ��
* @note io����ץ�ģ���ָ��CPU�ܽ�ֱ�ӷ���ץ������б���Ӳ����
* @see .
*/
int swpa_camera_io_set_iocapture_param(
	int lane_differentiation
);

/**
* @brief ��ȡio����ץ�Ĳ���\n
*
* 
* @param [out] lane_differentiation �Ƿ�ֳ�����0-���ֳ�����ÿ��io��������ʹ��������ƹ�����1-�ֳ�����ÿ��io����ʱָ���ļ�·����ƹ���
* @retval 0 : ִ�гɹ�
* @retval -1 : ִ��ʧ��
* @note io����ץ�ģ���ָ��CPU�ܽ�ֱ�ӷ���ץ������б���Ӳ����
* @see .
*/
int swpa_camera_io_get_iocapture_param(
	int * lane_differentiation
);

/**
* @brief ����֡�ʣ���ͬ����ģʽ\n
*
* 
* @param [in] mode ��ͬ��ģʽ��1-�����ڲ�����ͬ����2-�����ⲿ����ͬ����Ŀǰ����ӵڶ�·ץ���źŽӿڽ���ͬ���źţ���3-200w/280wʱ25֡ģʽ��500wʱ12.5֡ģʽ��4-200w/280wʱ30֡ģʽ��500wʱ15֡ģʽ
* @param [in] external_sync_delay ��ͬ���ź��ӳ٣���Χ0-200����λΪ0.1ms
* @retval 0 : ִ�гɹ�
* @retval -1 : ִ��ʧ��
* @note external_sync_delayֻ��modeΪ1��2ʱ��Ч��
* @see .
*/
int swpa_camera_io_set_framerate_mode(
	int mode,
	int external_sync_delay
);

/**
* @brief ��ȡ֡�ʣ���ͬ����ģʽ\n
*
* 
* @param [out] mode ��ͬ��ģʽ��1-�����ڲ�����ͬ����2-�����ⲿ����ͬ����Ŀǰ����ӵڶ�·ץ���źŽӿڽ���ͬ���źţ���3-200w/280wʱ25֡ģʽ��500wʱ12.5֡ģʽ��4-200w/280wʱ30֡ģʽ��500wʱ15֡ģʽ
* @param [out] external_sync_delay ��ͬ���ź��ӳ٣���Χ0-200����λΪ0.1ms
* @retval 0 : ִ�гɹ�
* @retval -1 : ִ��ʧ��
* @note external_sync_delayֻ��modeΪ1��2ʱ��Ч��
* @see .
*/
int swpa_camera_io_get_framerate_mode(
	int * mode,
	int * external_sync_delay
);

/**
* @brief ����Ƶ�����źſ���\n
*
* 
* @param [in] enable Ƶ���źſ��أ�0-�ر�Ƶ��ͬ���ź������1-����Ƶ��ͬ���ź����
* @retval 0 : ִ�гɹ�
* @retval -1 : ִ��ʧ��
* @note .
* @see .
*/
int swpa_camera_io_set_strobe_signal(
	int enable
);

/**
* @brief ��ȡƵ�����źſ���\n
*
* 
* @param [out] enable Ƶ���źſ��أ�0-�ر�Ƶ��ͬ���ź������1-����Ƶ��ͬ���ź����
* @retval 0 : ִ�гɹ�
* @retval -1 : ִ��ʧ��
* @note .
* @see .
*/
int swpa_camera_io_get_strobe_signal(
	int * enable
);

/**
* @brief ����������źſ���\n
*
* 
* @param [in] enable_flash_set ���õ��������ϣ�Ŀǰ֧��3·����ƣ��ӵ�λ��ʼ����һλ��Ӧ��һ·����ƣ��ڶ�λ��Ӧ�ڶ�·����ơ�����λΪ1���ʾ���ø�·����ƣ���Ĭ��ֵΪ7����·��ʹ��
* @retval 0 : ִ�гɹ�
* @retval -1 : ִ��ʧ��
* @note .
* @see .
*/
int swpa_camera_io_set_flash_signal(
	int enable_flash_set
);

/**
* @brief ��ȡ������źſ���\n
*
* 
* @param [out] enable_flash_set ���õ��������ϣ�Ŀǰ֧��3·����ƣ��ӵ�λ��ʼ����һλ��Ӧ��һ·����ƣ��ڶ�λ��Ӧ�ڶ�·����ơ�����λΪ1���ʾ���ø�·����ƣ�
* @retval 0 : ִ�гɹ�
* @retval -1 : ִ��ʧ��
* @note .
* @see .
*/
int swpa_camera_io_get_flash_signal(
	int * enable_flash_set
);

/**
* @brief ����Ƶ������Ʋ���\n
*
* 
* @param [in] multiplication ��Ƶ��1-��Ƶ��>=50hz����0-����Ƶ������ƣ�
* @param [in] polarity ���ԣ�1-�����ԣ�0-������
* @param [in] pulse_width ������Χ1-200������0.1ms���������ֵ��ΧΪ0.1ms-20ms
* @param [in] output_type ������ͣ�0-��������ƽ����1-OC�����أ�
* @param [in] enable_when_capture ץ��֡Ƶ����ʹ�ܣ�1-ʹ�ܣ�0-��ʹ��
* @retval 0 : ִ�гɹ�
* @retval -1 : ִ��ʧ��
* @note .
* @see .
*/
int swpa_camera_io_set_strobe_param(
	int multiplication, 
	int polarity, 
	int pulse_width,
	int output_type,
	int enable_when_capture
);

/**
* @brief ��ȡƵ������Ʋ���\n
*
* 
* @param [out] multiplication ��Ƶ��1-��Ƶ��>=50hz����0-����Ƶ������ƣ�
* @param [out] polarity ���ԣ�1-�����ԣ�0-������
* @param [out] pulse_width ������Χ1-200������0.1ms���������ֵ��ΧΪ0.1ms-20ms
* @param [out] output_type ������ͣ�0-��������ƽ����1-OC�����أ�
* @param [out] enable_when_capture ץ��֡Ƶ����ʹ�ܣ�1-ʹ�ܣ�0-��ʹ��
* @retval 0 : ִ�гɹ�
* @retval -1 : ִ��ʧ��
* @note .
* @see .
*/
int swpa_camera_io_get_strobe_param(
	int * multiplication, 
	int * polarity, 
	int * pulse_width,
	int * output_type,
	int * enable_when_capture
);

/**
* @brief ��������Ʋ���\n
*
* 
* @param [in] id ���õ��������ţ�1��3��
* @param [in] coupling ץ����ϵ�Ƶ������ץ��ʱ��Ƶ���Ƶ�����ʱ������Ҫ��������Ƶ�������Ĭ��ֵΪ1��1-��ϣ�0-�����
* @param [in] polarity ���ԣ�Ĭ��ֵΪ1��1-�����ԣ�0-������
* @param [in] pulse_width ����Ĭ��ֵΪ1����Χ1-100������0.1ms���������ֵ��ΧΪ0.1ms-10ms
* @param [in] output_type ������ͣ�Ĭ��ֵΪ1��0-��������ƽ����1-OC�����أ�
* @retval 0 : ִ�гɹ�
* @retval -1 : ִ��ʧ��
* @note .
* @see .
*/
int swpa_camera_io_set_flash_param(
	int id,
	int coupling,
	int polarity,
	int pulse_width,
	int output_type
);

/**
* @brief ��ȡ����Ʋ���\n
*
* 
* @param [in] id ��ȡ���������ţ�1��3��
* @param [out] coupling ץ����ϵ�Ƶ������ץ��ʱ��Ƶ���Ƶ�����ʱ������Ҫ��������Ƶ�������Ĭ��ֵΪ1��1-��ϣ�0-�����
* @param [out] polarity ���ԣ�Ĭ��ֵΪ1��1-�����ԣ�0-������
* @param [out] pulse_width ����Ĭ��ֵΪ1����Χ1-100������0.1ms���������ֵ��ΧΪ0.1ms-10ms
* @param [out] output_type ������ͣ�Ĭ��ֵΪ1��0-��������ƽ����1-OC�����أ�
* @retval 0 : ִ�гɹ�
* @retval -1 : ִ��ʧ��
* @note .
* @see .
*/
int swpa_camera_io_get_flash_param(
	int id,
	int * coupling,
	int * polarity,
	int * pulse_width,
	int * output_type
);

/**
* @brief ���õصƲ���\n
*
* 
* @param [in] enable �صƿ��أ�0-�رյصƣ�1-�����ص�
* @retval 0 : ִ�гɹ�
* @retval -1 : ִ��ʧ��
* @note .
* @see .
*/
int swpa_camera_io_set_lamp(
	int enable
);

/**
* @brief ��ȡ�صƲ���\n
*
* 
* @param [out] enable �صƿ��أ�0-�رյصƣ�1-�����ص�
* @retval 0 : ִ�гɹ�
* @retval -1 : ִ��ʧ��
* @note .
* @see .
*/
int swpa_camera_io_get_lamp(
	int * enable
);

/**
* @brief �����Զ�DC��Ȧ����ع�Ȧ��\n
*
* 
* @param [in] mode ��Ȧ����ģʽ��0-ֹͣ��1-�򿪣�2-�ر�
* @retval 0 : ִ�гɹ�
* @retval -1 : ִ��ʧ��
* @note ��DC��Ȧ����ȫ�ػ����ǰ��״̬ʱ��DC��Ȧ��ֹͣ״̬ʱ��������һֱֹͣ����ֻ�������ݵ�ͣ������Ȧ���Զ����ſ���������Ҫ��ͣ����ֹͣ�͹ر�״̬�л������ܻ�������ֹͣ�� 
* @see .
*/
int swpa_camera_io_control_dc_iris(
	int mode
);

/**
* @brief �����˹�Ƭ\n
*
* 
* @param [in] mode �˹�Ƭ����ģʽ��0-ֹͣ��1-�����˹�Ƭ��2-���⣨��ƫ���˹�Ƭ
* @retval 0 : ִ�гɹ�
* @retval -1 : ִ��ʧ��
* @note �л��˹�Ƭ������2s������Ϊֹͣ״̬��������1��2Ϊ�������ô˽ӿں󣬵ȴ�2s����0Ϊ�������ô˽ӿڡ�
*       �˹�Ƭ�ĵ�ǰ״̬���޷��ӼĴ�������ͼƬ�����Ϣ��ȡ����Ҫ���ϲ��������ģ������ά����
* @see .
*/
int swpa_camera_io_control_filter(
	int mode
);

/**
* @brief ���ñ�������\n
*
* 
* @param [in] x1 ������������Ͻ�x���꣬��Χ0-2047
* @param [in] y1 ������������Ͻ�y���꣬��Χ0-2047
* @param [in] x2 ������������½�x���꣬��Χ0-2047
* @param [in] y2 ������������½�y���꣬��Χ0-2047
* @retval 0 : ִ�гɹ�
* @retval -1 : ִ��ʧ��
* @note .
* @see .
*/
int swpa_camera_io_set_alarm_zone(
	int x1, int y1,
	int x2, int y2
);

/**
* @brief ��ȡ��������\n
*
* 
* @param [in] x1 ������������Ͻ�x���꣬��Χ0-2047
* @param [in] y1 ������������Ͻ�y���꣬��Χ0-2047
* @param [in] x2 ������������½�x���꣬��Χ0-2047
* @param [in] y2 ������������½�y���꣬��Χ0-2047
* @retval 0 : ִ�гɹ�
* @retval -1 : ִ��ʧ��
* @note .
* @see .
*/
int swpa_camera_io_get_alarm_zone(
	int * x1, int * y1,
	int * x2, int * y2
);


/**
* @brief ��ȡ���˻�״̬
*
* 
* @param [out] pstate ���˻�״ֵ̬����Χ0-1
* @retval 0 : ִ�гɹ�
* @retval -1 : ִ��ʧ��
* @note .
* @see .
*/
int swpa_camera_io_get_barrier_state(int* pstate);


/**
* @brief ʹ��TG2��Ϊ�ⲿ�ź����빦�ܣ�����������
*
* 
* @param [in] enable ���ⲿ�ź�����ʹ�ñ�־����Χ0-1
* @retval 0 : ִ�гɹ�
* @retval -1 : ִ��ʧ��
* @note .
* @see .
*/
int swpa_camera_io_backupio_enable(int enable);

/**
***********************
* ���ͼ����
* swpa_camera_imgproc_
***********************
*/



/**
* @brief ����Gammaֵ\n
*
* 
* @param [in] data Gammaֵ
* @retval 0 : ִ�гɹ�
* @retval -1 : ִ��ʧ��
* @note .
* @see .
*/
int swpa_camera_imgproc_set_gamma(
	int data[8][2]
);

/**
* @brief ��ȡGammaֵ\n
*
* 
* @param [out] data Gammaֵ
* @retval 0 : ִ�гɹ�
* @retval -1 : ִ��ʧ��
* @note Ӧ����ʱû��ʹ��������ȡֵ���̽ϸ��ӣ���ʵ��
* @see .
*/
int swpa_camera_imgproc_get_gamma(
	int data[8][2]
);

/**
* @brief ����Gamma ͨ��ѡ��ģʽ\n
*
* 
* @param [in] mode ͨ��ѡ��ֵ��0��6467��368����GAMMA��1��6467��368�ޣ�2��6467��368�У�3��6467��368��
* @retval 0 : ִ�гɹ�
* @retval -1 : ִ��ʧ��
* @note .
* @see .
*/
int swpa_camera_imgproc_set_gamma_mode(
	int mode
);

/**
* @brief ��ȡGamma ͨ��ѡ��ģʽ\n
*
* 
* @param [in] mode ͨ��ѡ��ֵ��0��6467��368����GAMMA��1��6467��368�ޣ�2��6467��368�У�3��6467��368��
* @retval 0 : ִ�гɹ�
* @retval -1 : ִ��ʧ��
* @note .
* @see .
*/
int swpa_camera_imgproc_get_gamma_mode(
	int* mode
	);

/**
* @brief ���ú�ƼӺ����\n
*
* 
* @param [in] h_threshold ����ƵƼӺ��йأ�HSL�ռ䣬ɫ����ֵ����Χ0-184320������Ϊ1/512�ȣ�����ﷶΧ��0-360��
* @param [in] l_threshold ����ƵƼӺ��йأ�HSL�ռ䣬������ֵ����Χ0-16383
* @param [in] s_threshold ����ƵƼӺ��йأ�HSL�ռ䣬���Ͷ���ֵ����Χ0-16383
* @param [in] color_factor ����ƵƼӺ��йأ�ɫ��˷����ӣ���Χ0-511
* @param [in] lum_l_threshold ����׵ƼӺ��йأ�������ֵ����Χ0-16383
* @param [in] lum_l_factor ����׵ƼӺ��йأ����ȳ˷����ӣ���Χ0-511
* @param [in] lum_h_factor ����׵ƼӺ��йأ�ɫ��˷����ӣ���Χ0-511
* @retval 0 : ִ�гɹ�
* @retval -1 : ִ��ʧ��
* @note .
* @see .
*/
int swpa_camera_imgproc_set_trafficlight_enhance_param(
	int h_threshold, 
	int l_threshold, 
	int s_threshold, 
	int color_factor,
	int lum_l_threshold,
	int lum_l_factor,
	int lum_h_factor
);

/**
* @brief ��ȡ��ƼӺ����\n
*
* 
* @param [out] h_threshold ����ƵƼӺ��йأ�HSL�ռ䣬ɫ����ֵ����Χ0-184320������Ϊ1/512�ȣ�����ﷶΧ��0-360��
* @param [out] l_threshold ����ƵƼӺ��йأ�HSL�ռ䣬������ֵ����Χ0-16383
* @param [out] s_threshold ����ƵƼӺ��йأ�HSL�ռ䣬���Ͷ���ֵ����Χ0-16383
* @param [out] color_factor ����ƵƼӺ��йأ�ɫ��˷����ӣ���Χ0-511
* @param [out] lum_l_threshold ����׵ƼӺ��йأ�������ֵ����Χ0-16383
* @param [out] lum_l_factor ����׵ƼӺ��йأ����ȳ˷����ӣ���Χ0-511
* @param [out] lum_h_factor ����׵ƼӺ��йأ�ɫ��˷����ӣ���Χ0-511
* @retval 0 : ִ�гɹ�
* @retval -1 : ִ��ʧ��
* @note .
* @see .
*/
int swpa_camera_imgproc_get_trafficlight_enhance_param(
	int * h_threshold, 
	int * l_threshold, 
	int * s_threshold, 
	int * color_factor,
	int * lum_l_threshold,
	int * lum_l_factor,
	int * lum_h_factor
);

/**
* @brief ���ú�ƼӺ�����\n
*
* 
* @param [in] id ��ƼӺ�����IDֵ���Ϸ�ֵΪ1��8
* @param [in] x1 �Ӻ���������Ͻ�x����
* @param [in] y1 �Ӻ���������Ͻ�y����
* @param [in] x2 �Ӻ���������½�x����
* @param [in] y2 �Ӻ���������½�y����
* @retval 0 : ִ�гɹ�
* @retval -1 : ִ��ʧ��
* @note .
* @see .
*/
int swpa_camera_imgproc_set_trafficlight_enhance_zone(
	int id,
	int x1, int y1,
	int x2, int y2
);

/**
* @brief ��ȡ��ƼӺ�����\n
*
* 
* @param [in] id ��ƼӺ�����IDֵ���Ϸ�ֵΪ1��8
* @param [out] x1 �Ӻ���������Ͻ�x����
* @param [out] y1 �Ӻ���������Ͻ�y����
* @param [out] x2 �Ӻ���������½�x����
* @param [out] y2 �Ӻ���������½�y����
* @retval 0 : ִ�гɹ�
* @retval -1 : ִ��ʧ��
* @note �Ĵ�����ʱ��֧��ȡֵ����ʵ�ִ˽ӿ�
* @see .
*/
int swpa_camera_imgproc_get_trafficlight_enhance_zone(
	int id,
	int * x1, int * y1,
	int * x2, int * y2
);

/**
* @brief ����ɫ�ʲ���\n
*
* 
* @param [in] mode ����ģʽ 1-�ر�ɫ�ʵ������ܣ�2-������ץ��֡��3-����������֡
* @param [in] contrast_value �Աȶ�ֵ����Χ��[-100~100]��Ĭ��ֵ��0(��ֵ���Ӧ�ò㣬����õ���������FPGA�Ĵ�����Ĭ��ֵ)
* @param [in] saturation_value ���Ͷ�ֵ����Χ��[-100~100]��Ĭ��ֵ��0(��ֵ���Ӧ�ò㣬����õ���r/g/b������FPGA�Ĵ�����Ĭ��ֵ)
* @param [in] color_temperature_value ɫ��ֵ
* @param [in] hue_value ɫ��ֵ��ɫ�ʡ�ɫ�ࣩ
* @retval 0 : ִ�гɹ�
* @retval -1 : ִ��ʧ��
* @note .
* @see .
*/
int swpa_camera_imgproc_set_color_param(
	int mode,
	int contrast_value,
	int saturation_value,
	int color_temperature_value,
	int hue_value
);

/**
* @brief ��ȡɫ�ʲ���\n
*
* 
* @param [out] mode ����ģʽ 1-�ر�ɫ�ʵ������ܣ�2-������ץ��֡��3-����������֡
* @param [out] contrast_value �Աȶ�ֵ����Χ��[-100~100]��Ĭ��ֵ��0(��ֵ���Ӧ�ò㣬����õ���������FPGA�Ĵ�����Ĭ��ֵ)
* @param [out] saturation_value ���Ͷ�ֵ����Χ��[-100~100]��Ĭ��ֵ��0(��ֵ���Ӧ�ò㣬����õ���r/g/b������FPGA�Ĵ�����Ĭ��ֵ)
* @param [out] color_temperature_value ɫ��ֵ
* @param [out] hue_value ɫ��ֵ��ɫ�ʡ�ɫ�ࣩ
* @retval 0 : ִ�гɹ�
* @retval -1 : ִ��ʧ��
* @note Ӧ����ʱû��ʹ����������ʱû��ʵ�ֽ��Ĵ���ֵ��ֵ��㷨����˲�ʵ�ִ˽ӿ�
* @see .
*/
int swpa_camera_imgproc_get_color_param(
	int * mode,
	int * contrast_value,
	int * saturation_value,
	int * color_temperature_value,
	int * hue_value
);

/**
* @brief ��������ز���\n
*
* 
* @param [in] mode ����ģʽ 1-�ر��񻯹��ܣ�2-������ץ��֡��3-����������֡  ľ�ǲ�ʹ��
* @param [in] threshold ��ֵ ��Χ0-127 ľ�Ƿ�Χ0~15
* @retval 0 : ִ�гɹ�
* @retval -1 : ִ��ʧ��
* @note 
* @see .
*/
int swpa_camera_imgproc_set_sharpen_param(
	int mode,
	int threshold
);

/**
* @brief ��ȡ����ز���\n
*
* 
* @param [out] mode ����ģʽ 1-�ر��񻯹��ܣ�2-������ץ��֡��3-����������֡  ľ�ǲ�ʹ��
* @param [out] threshold ��ֵ ��Χ0-127  ľ�Ƿ�Χ0~15
* @retval 0 : ִ�гɹ�
* @retval -1 : ִ��ʧ��
* @note 
* @see .
*/
int swpa_camera_imgproc_get_sharpen_param(
	int * mode,
	int * threshold
);

/**
* @brief ���ó�����ز���\n
*
* 
* @param [in] mode ����ģʽ 1-�رճ����ܣ�2-������ץ��֡��3-����������֡
* @param [in] factor ����ϵ�� ��Χ��[1~10]�� ֵԽ�ߣ������Խ�ɾ�����ͼ���䰵
* @retval 0 : ִ�гɹ�
* @retval -1 : ִ��ʧ��
* @note ����ϵ�������û����ã�Ĭ�ϼ��Ƽ�ֵΪ8���Ƽ���Χ��6~8
* @todo ľ���ݲ�֧��ͼ��ĳ���
* @see .
*/
int swpa_camera_imgproc_set_defog_param(
	int mode,
	int factor
);

/**
* @brief ��ȡ������ز���\n
*
* 
* @param [out] mode ����ģʽ 1-�رճ����ܣ�2-������ץ��֡��3-����������֡
* @param [out] factor ����ϵ�� ��Χ��[1~10]�� ֵԽ�ߣ������Խ�ɾ�����ͼ���䰵
* @retval 0 : ִ�гɹ�
* @retval -1 : ִ��ʧ��
* @note .
* @see .
*/
int swpa_camera_imgproc_get_defog_param(
	int * mode,
	int * factor
);

/**
* @brief ���ò�ɫ/�ڰ����ģʽ\n
*
* 
* @param [in] mode ����ģʽ 0-��ɫ��1-�ڰס�Ĭ��Ϊ0
* @retval 0 : ִ�гɹ�
* @retval -1 : ִ��ʧ��
* @note .
* @see .
*/
int swpa_camera_imgproc_set_color_black(
	int mode
);

/**
* @brief ��ȡ��ɫ/�ڰ����ģʽ\n
*
* 
* @param [out] mode ����ģʽ 0-��ɫ��1-�ڰס�Ĭ��Ϊ0
* @retval 0 : ִ�гɹ�
* @retval -1 : ִ��ʧ��
* @note .
* @see .
*/
int swpa_camera_imgproc_get_color_black(
	int * mode
);

/**
* @brief ���ñ��Ͷ�\n
* @param [in] value 0x0 -0x0e
* @retval 
* 0 - �ɹ�
* -1 - ʧ��
* @note ľ��ʹ��
* @see .
*/
int swpa_camera_imgproc_set_saturation(
    int saturation
);

/**
* @brief ��ȡ���Ͷ�\n
* @param [out] value 0x0 -0x0e
* @retval 
* 0 - �ɹ�
* -1 - ʧ��
* @note ľ��ʹ��
* @see .
*/
int swpa_camera_imgproc_get_saturation(
    int *saturation
);

/**
* @brief ����gamma\n
* @param [in] value ��о:   0:	Standard,	1	to	4
* @retval 
* 0 - �ɹ�
* -1 - ʧ��
* @note ľ��ʹ��
* @see .
*/
int swpa_camera_imgproc_set_gamma_type(
    int gamma
);

/**
* @brief ��ȡgamma\n
* @param [out] value
* @retval 
* 0 - �ɹ�
* -1 - ʧ��
* @note ľ��ʹ��
* @see .
*/
int swpa_camera_imgproc_get_gamma_type(
    int *gamma
);

/**
* @brief ����WDR\n
* @param [in] mode ��wdr ģʽ �μ�WDR_MODE
* @retval 
* 0 - �ɹ�
* -1 - ʧ��
* @note ľ��ʹ��
* @see .
*/
int swpa_camera_imgproc_set_WDR(
    WDR_MODE mode
);

/**
* @brief ��ȡWDR\n
* @param [out] mode ��wdr ģʽ
* @retval 
* 0 - �ɹ�
* -1 - ʧ��
* @note ľ��ʹ��
* @see .
*/
int swpa_camera_imgproc_get_WDR(
    WDR_MODE *mode
);

/**
* @brief ���ý���\n
* @param [in] value   ��о:   0:	OFF,	level	1	to	5
* @retval 
* 0 - �ɹ�
* -1 - ʧ��
* @note ľ��ʹ��
* @see .
*/
int swpa_camera_imgproc_set_NR(
    int NR
);

/**
* @brief ��ȡ����\n
* @param [out] value
* @retval 
* 0 - �ɹ�
* -1 - ʧ��
* @note ľ��ʹ��
* @see .
*/
int swpa_camera_imgproc_get_NR(
    int *NR
);

/**
***********************
* �����ʼ�������ã�������㡢ǰ����������ȣ�,�Ȳ���
* swpa_camera_init_
***********************
*/

/**
* @brief ����RAW RGB����\n
*
* 
* @param [in] gain_r R���棬��Χ0-131071��17λ�޷������ͣ�����λΪ1/2^14�����൱�ڿ��Ա��ı���Ϊ[0.0-8.0>
* @param [in] gain_gr Gr���棬��Χ0-131071��17λ�޷������ͣ�����λΪ1/2^14�����൱�ڿ��Ա��ı���Ϊ[0.0-8.0>
* @param [in] gain_gb Gb������Χ0-131071��17λ�޷������ͣ�����λΪ1/2^14�����൱�ڿ��Ա��ı���Ϊ[0.0-8.0>
* @param [in] gain_b B���棬��Χ0-131071��17λ�޷������ͣ�����λΪ1/2^14�����൱�ڿ��Ա��ı���Ϊ[0.0-8.0>
* @retval 0 : ִ�гɹ�
* @retval -1 : ִ��ʧ��
* @note �������������ӿڣ�fpga�ĸ�����Ĭ��ֵΪ0x4000��������Ϊ������1.0��
* @see .
*/
int swpa_camera_init_set_raw_rgb_gain(
	int gain_r,
	int gain_gr,
	int gain_gb,
	int gain_b
); 

/**
* @brief ��������ĸ�ͨ������ֵ\n
*
* 
* @param [in] gain_correction1 ����ĵ�1ͨ������ֵ����Χ-1023~1023������0.0359db�������淶ΧΪ-36db~36db
* @param [in] gain_correction2 ����ĵ�2ͨ������ֵ����Χ-1023~1023������0.0359db�������淶ΧΪ-36db~36db
* @param [in] gain_correction3 ����ĵ�3ͨ������ֵ����Χ-1023~1023������0.0359db�������淶ΧΪ-36db~36db
* @param [in] gain_correction4 ����ĵ�4ͨ������ֵ����Χ-1023~1023������0.0359db�������淶ΧΪ-36db~36db
* @retval 0 : ִ�гɹ�
* @retval -1 : ִ��ʧ��
* @note ��������Ϊ��ֵ������������ʱ�Զ�������ֵ���ӵ���ͨ���ϡ�
*       ��ƴ˽ӿ�ֻ�ڳ�ʼ��ʱ����һ�Σ�����ڲ�����֤�̰߳�ȫ��
*       ��ͬ�ֱ��ʵ��豸�߱���ͬ��ͨ��������500W�豸������ͨ������ֻ��gain_correction1��gain_correction2��������
* @see swpa_camera_basicparam_set_gain
*/
int swpa_camera_init_set_gain_correction(
	int gain_correction1,
	int gain_correction2,
	int gain_correction3,
	int gain_correction4
);

/**
* @brief ����ץ������ĸ�ͨ������ֵ\n
*
* 
* @param [in] gain_correction1 ����ĵ�1ͨ������ֵ����Χ-1023~1023������0.0359db�������淶ΧΪ-36db~36db
* @param [in] gain_correction2 ����ĵ�2ͨ������ֵ����Χ-1023~1023������0.0359db�������淶ΧΪ-36db~36db
* @param [in] gain_correction3 ����ĵ�3ͨ������ֵ����Χ-1023~1023������0.0359db�������淶ΧΪ-36db~36db
* @param [in] gain_correction4 ����ĵ�4ͨ������ֵ����Χ-1023~1023������0.0359db�������淶ΧΪ-36db~36db
* @retval 0 : ִ�гɹ�
* @retval -1 : ִ��ʧ��
* @note ��������Ϊ��ֵ������������ʱ�Զ�������ֵ���ӵ���ͨ���ϡ�
*       ��ƴ˽ӿ�ֻ�ڳ�ʼ��ʱ����һ�Σ�����ڲ�����֤�̰߳�ȫ��
*       ��ͬ�ֱ��ʵ��豸�߱���ͬ��ͨ��������500W�豸������ͨ������ֻ��gain_correction1��gain_correction2��������
* @see swpa_camera_basicparam_set_capture_gain
*/
int swpa_camera_init_set_capture_gain_correction(
	int gain_correction1,
	int gain_correction2,
	int gain_correction3,
	int gain_correction4
);

/**
* @brief ���û���ȥ������\n
*
* 
* @param [in] threshold ��ֵ����Χ0-16383��Ĭ��ֵ8000����ֵԽС������ȥ��Ч��Խ���ԣ�����̫С���˵�ĳЩ���ǻ���ĵ㣬ȡ���ֵʱ��û�л���ȥ�����ܡ�
* @retval 0 : ִ�гɹ�
* @retval -1 : ִ��ʧ��
* @note .
* @see .
*/
int swpa_camera_init_set_dead_pixel_removal(
	int threshold
); 

/**
* @brief ��ȡ�豸��ص�����\n
*
* 
* @param [out] max_shutter �������ֵ����λ1us��������СֵΪ0
* @param [out] max_gain �������ֵ����λ0.1db��������СֵΪ0
* @param [out] ad_channels ADͨ����
* @retval 0 : ִ�гɹ�
* @retval -1 : ִ��ʧ��
* @note .
* @see .
*/
int swpa_camera_init_get_device_dependent_info(
	int * max_shutter,
	int * max_gain,
	int * ad_channels
); 

///////////////////////////////////////////////////////////////////////////////
/**
***********************
* ������ľ��ƽ̨�����ؽӿ�ʵ��
*/

/** 
 * ֹͣ�������
 *
 * @return �ɹ���0  ʧ�ܣ�-1 ��ʱ��-6
 * @note ÿ�ֶ�������Ҫ���øú�����ֹͣ�����������һֱִ��(���������ˮƽ��ת�����һֱת)��
 */
int swpa_camera_ball_stop(void);

/** 
 * �����ֱ�ƶ�
 * @param action [in] ��ֱ�ƶ����� 0 = down  1 = up
 * @param speed [in]  �ƶ��ٶȣ�1~0x3f
 * 
 * @return �ɹ���0  ʧ�ܣ�-1 �����Ƿ���-2 ��ʱ��-6
 */
int swpa_camera_ball_tilt(TILT_TYPE action, int speed);

/** 
 * ���ˮƽ�ƶ�
 * @param action [in] ˮƽ��ת���� 0 = left  1 = right
 * @param speed [in]  �ƶ��ٶȣ�1~0x3f
 * 
 * @return �ɹ���0  ʧ�ܣ�-1 �����Ƿ���-2 ��ʱ��-6
 */
int swpa_camera_ball_pan(PAN_TYPE action, int speed);

/** 
 * ���ͬʱˮƽ����ֱ�ƶ�
 * @param pan_act [in] ˮƽ��ת���� 0 = left  1 = right
 * @param pan_speed [in] �ƶ��ٶȣ�1~0x3f
 * @param tilt_act [in] ��ֱ�ƶ����� 0 = down  1 = up
 * @param tilt_speed [in] �ƶ��ٶȣ�1~0x3f
 * 
 * @return �ɹ���0  ʧ�ܣ�-1 �����Ƿ���-2 ��ʱ��-6
 * @note �ú�����swpa_camera_ball_pan��swpa_camera_ball_tilt�Ļ����ʽ
 */
int swpa_camera_ball_pan_tilt(PAN_TYPE pan_act, int pan_speed, TILT_TYPE tilt_act, int tilt_speed);
///////////////////////////////////////////////////////////////////////////////

/** 
 * Ԥ��λ��غ���
 * @param preset_act [in] Ԥ��λ���� 0������ 1����� 2������
 * @param preset_id [in] 1~255 ��0����Ч
 * 
 * @return �ɹ���0  ʧ�ܣ�-1 �����Ƿ���-2 ��ʱ��-6
 * @todo ����в���Ԥ��λ���Ѿ���ռ���ˣ���Ŀǰֻ�в���Ԥ��λ���ĵ����޷�֪���Ƿ�������������Ԥ��λ��
 */
int swpa_camera_ball_preset(PRESET_TYPE preset_act, int preset_id);

/** 
 * �������PT����
 * @param pan [in] 0~3599(0~359.9��)
 * @param tilt [in] -100~900(-10��~90��)
 * 
 * @return �ɹ���0  ʧ�ܣ�-1 �����Ƿ���-2 ��ʱ��-6
 * @note PT��ˮƽ(pan)����ֱ(tilt)�����ꡣˮƽ��Χֻ��3599������3600������0��Ч
 * @todo ����Ҫ��ֱ�Ƕ���-10~90������������н��в����жϡ�
 */
int swpa_camera_ball_set_pt_coordinates(int pan, int tilt);

/** 
 * ��ȡ���PT����
 * @param pan [out] 0~3599(0~359.9��)
 * @param tilt [out] 0~900(0��~90��)
 * 
 * @return �ɹ���0  ʧ�ܣ�-1 �����Ƿ���-2 ��ʱ��-6
 * @note ˵�������������Ϊ0.1������ֵ1���������ȡ֮�����1����
 */
int swpa_camera_ball_get_pt_coordinates(int* pan, int* tilt);

/** 
 * ���ú���ģʽ
 *
 * @param mode [in] ģʽ��0-�ر� 1-���� 2-�Զ�����
 * @return �ɹ���0  ʧ�ܣ�-1 ��ʱ��-6
 */
 /* //Marked in Version B 
int swpa_camera_ball_set_infrared_led(int mode);*/

/** 
 * ���ú��⹦��
 * @param zoom_total [in] ��о�ܵı䱶��(������ܷŴ���ٱ�)
 * @param zoom_1 [in]     ��ǰ�䱶�ʵ���������
 * @param zoom_2 [in]     ��ǰ�䱶�ʵ�С������
 * 
 * @return �ɹ���0  ʧ�ܣ�-1 ��ʱ��-6
 * @note �����õ�ǰ�ı䱶��Ϊ3.75����zoom_1Ϊ3,zoom_2Ϊ75
 *       �緢�͵�ǰ�䱶Ϊ1�������������������͵�ǰ�䱶Ϊ20������Զ������� ��
 *       �������ڵ���swpa_camera_ball_set_infrared_led�򿪺��������²������ã�����
 */
/* //Marked in Version B 
int swpa_camera_ball_set_infrared_power(int zoom_total, int zoom_1, int zoom_2);*/

/** 
 * ���ú���ƿ���������
 * @param sensitivity [in] ������ֵ ��Χ��1-10
 * 
 * @return �ɹ���0  ʧ�ܣ�-1 �����Ƿ���-2 ��ʱ��-6
 * @note ��ֵԽС������ƿ�����Ҫ�Ļ������ն�Խ�ͣ�����Ĭ��Ϊ3
 */
int swpa_camera_ball_set_infrared_sensitive(int sensitivity);

/** 
 * ���ú��⿪����ֵ����
 * @param threshold [in] ��ֵ ��Χ��1-5
 * 
 * @return �ɹ���0  ʧ�ܣ�-1 �����Ƿ���-2 ��ʱ��-6
 * @note �������ƿ���Ƶ�������԰��л�����ֵ�ʵ����󣬳���Ĭ��Ϊ2
 */
int swpa_camera_ball_set_infrared_threshold(int threshold);


/** 
 * ������ˢ
 *
 * @return �ɹ���0  ʧ�ܣ�-1 ��ʱ��-6
 * @note ������ˢ�����Զ������ƶ�3�Σ�Ȼ��ֹͣ
 */
int swpa_camera_ball_set_wiper(void);

///////////////////////////////////////////////////////////////////////////////

/** 
 * Զ�̸�λ
 *
 * @return �ɹ���0  ʧ�ܣ�-1 ��ʱ��-6
 * @note ��λ��������Զ������Լ����(��ת��)
 */
int swpa_camera_ball_remote_reset(void);

/** 
 * �ָ���������
 *
 * @return �ɹ���0  ʧ�ܣ�-1 ��ʱ��-6
 */
int swpa_camera_ball_restore_factory_settings(void);

/** 
 * ��ʼ�ϻ�����
 *
 * @return �ɹ���0  ʧ�ܣ�-1 ��ʱ��-6
 */
int swpa_camera_ball_factorytest_start(void);

/** 
 * �ر��ϻ�����
 *
 * @return �ɹ���0  ʧ�ܣ�-1 ��ʱ��-6
 */
int swpa_camera_ball_factorytest_stop(void);

/** 
 * ������������˪����
 *
 * @param mode [in] ģʽ��0-�ر� 1-����
 * @return �ɹ���0  ʧ�ܣ�-1 ��ʱ��-6
 * @note ������ʵ���Ͽ�������ϵķ���(�����ȹ���)ʹ����ɢȥ�������͹ر�֮��������̫Ƶ��
 */
int swpa_camera_ball_set_defog_fan(int mode);


/** 
 * ���������(3�������)У׼
 *
 * @return �ɹ���0  ʧ�ܣ�-1 
 * @note: ľ��ר��
 */
int swpa_camera_ball_calibration(void);

/** 
 * ���3D��λ
 *
 * @param centerx [in] ��λ���ĵ�x���� ��Χ��0 ~ 1920
 * @param centery [in] ��λ���ĵ�y���� ��Χ��0 ~ 1080
 * @param width [in] ��λ������ ��Χ��0 ~ 1920
 * @param height [in] ��λ����߶� ��Χ��0 ~ 1080
 * @return �ɹ���0  ʧ�ܣ�-1 
 * @note: ľ��ר��
 */
int swpa_camera_ball_3d(int centerx, int centery, int width, int height);

/** 
 * �����������ƹ���ģʽ
 *
 * @param mode [in] ����ģʽö�٣�0x0:LED_AUTO; 0x1:LED_TIMING; 0x2:LED_ON; 0x3:LED_OFF;0x4:LED_CMR_CTRL
 * @param powermode [in] ƥ��ģʽö�٣�
 *                  0x0: LED_POWER_STD ������Ʊ�׼ģʽ�����ʸ��ݵ�ǰ�䱶�������ƥ��;
 *                  0x1: LED_POWER_MATCH�������ƥ��ģʽ�������1��ʱ������ƿ��ƹ���Ϊ80%;
 *                  0x2: LED_POWER_USR : �Զ���ģʽ����ģʽ�£�����Ƹ��鹦�ʿ����������á�
 * @return �ɹ���0  ʧ�ܣ�-1 
 * @note: ľ��ר��
 */
int swpa_camera_ball_set_led_mode(int mode, int powermode);



/** 
 * �����������ƹ���
 *
 * @param nearpower [in] ����ƹ���ֵ����Χ��0 ~ 10
 * @param mediumpower [in] �й�ƹ���ֵ ��Χ��0 ~ 10
 * @param farpower [in] Զ��ƹ���ֵ ��Χ��0 ~ 10
 * @return �ɹ���0  ʧ�ܣ�-1 
 * @note: ľ��ר��
 */
int swpa_camera_ball_set_led_power(int nearpower, int mediumpower, int farpower);

/** 
 * ����������о����zoomֵͬ��
 *
 * @return �ɹ���0  ʧ�ܣ�-1 
 * @note: ľ��ר��
 */
int swpa_camera_ball_sync_zoom();

/** 
 * ���������˽����(�����о�������˽�ڸǹ���)
 *
 * @param index [in] ��˽�����ţ���Χ��0 ~ 8
 * @param enable [in] ʹ�ܱ�־ ��Χ��0 (��ʹ�ܣ�����)�� 1(ʹ�ܣ���ʾ)
 * @param width [in] �ڸ������� ��Χ��0 ~ 80
 * @param height [in] �ڸ�����߶� ��Χ��0 ~ 45
 * @return �ɹ���0  ʧ�ܣ�-1 
 * @note: ľ��ר��
 */
int swpa_camera_ball_set_privacyzone(int index, int enable, int width, int height);

/** 
 * ���������˽��������(���B�汾��֧��������������꣬��ǿ�Ƴɻ�������)
 *
 * @param index [in] ��˽�����ţ���Χ��0 ~ 8
 * @param x [in] �������ĺ����꣬��Χ��0 ~ 100
 * @param y [in] �������������꣬��Χ��0 ~ 100
 * @return �ɹ���0  ʧ�ܣ�-1 
 * @note: ľ��ר�ã����ȵ��øú������ٵ���swpa_camera_ball_set_privacyzone()����������ʾ�ڸ�����
 */
int swpa_camera_ball_set_privacycoord(int index, int x, int y);



/*
*  ----������----
*/


 /** 
 * ���û�о��������� δ����ͨ��
 *
 * @param 
 * type : visca ����pelco
 * write_buf : ����buffer
 * write_buf_size : �����
 * @return �ɹ���0  ʧ�ܣ�-1 ʧ��
 */
int spwa_camera_send_cmd(PROTOCOL_TYPE type,unsigned char *write_buf,int write_buf_size);

/** 
* ��ȡ��о��������� δ����ͨ��
*
* @param 
* type : visca ����pelco
* write_buf : ����buffer
* write_buf_size : �����
* awk_buf : ����buf
* awk_buf_size : �������ݳ���
* @return �ɹ���0  ʧ�ܣ�-1 ʧ��
*/
int spwa_camera_recive_cmd(PROTOCOL_TYPE type,unsigned char *write_buf,int write_buf_size,unsigned char *awk_buf,int *awk_buf_size);

/** 
 * ���о�������������
 * @param type [in] Э�����ͣ��ο�PROTOCOL_TYPE����(��֧�ָ�ö���е�ֵ)
 * @param buffer [in] ����
 * @param buffer_size [in] ���ݳ���
 * 
 * @return �ɹ���0  ʧ�ܣ�-1 �����Ƿ���-2 ��ʱ��-6
 * @note ������ֱ��ʹ��ϵͳ��write�����������ݴ���
 */
int spwa_camera_com_send_data(PROTOCOL_TYPE type,unsigned char *buffer, int buffer_size);

/** 
 * ��ȡ��о�����������
 * @param type [in] Э�����ͣ��ο�PROTOCOL_TYPE����(��֧�ָ�ö���е�ֵ)
 * @param buffer [in] ����
 * @param buffer_size [in] ���ݳ���
 * 
 * @return �ɹ���0  ʧ�ܣ�-1 �����Ƿ���-2 ��ʱ��-6
 * @note ֱ��ʹ��ϵͳ��read������������
 */
int spwa_camera_com_rcv_data(PROTOCOL_TYPE type, unsigned char *buffer, int buffer_size);

/** 
 * ��ȡ��о�����������
 * @param type [in] Э�����ͣ��ο�PROTOCOL_TYPE����(��֧�ָ�ö���е�ֵ)
 * @param buffer [out] ���ݻ�����
 * @param buffer_size [in/out] ���ݳ��ȣ�����ʱ����ʾϣ�����յ����ݣ�����Ϊʵ�ʽ��յ�����
 * 
 * @return �ɹ���0  ʧ�ܣ�-1 �����Ƿ���-2 ��ʱ��-6
 * @note ��������ӳ�ʱ���ջ���
 */
int spwa_camera_com_rcv_packet_data(PROTOCOL_TYPE type, unsigned char *buffer, int* buffer_size);

/** 
 * ��ȡ��о�����������
 * @param type [in] Э�����ͣ��ο�PROTOCOL_TYPE����(��֧�ָ�ö���е�ֵ)
 * @param buffer [out] ���ݻ�����
 * @param buffer_size [in/out] ���ݳ��ȣ�����ʱ����ʾϣ�����յ����ݣ�����Ϊʵ�ʽ��յ�����
 * @param terminator [in] ������
 * 
 * @return �ɹ���0  ʧ�ܣ�-1 �����Ƿ���-2 ��ʱ��-6
 * @note ��������spwa_camera_com_rcv_packet_data��֮ͬ�����ڣ����Խ��յ�terminator������Ϊ��־��
 *       �ʺ�visca��0xff���������ݷ�ʽ
 */
int spwa_camera_com_rcv_packet_terminator(PROTOCOL_TYPE type, unsigned char *buffer, int *buffer_size, int terminator);

/**
   @brief ͼ�����µ�ת
   @param value [IN] ON:0x02 OFF:0x03
 */
int swpa_camera_basicparam_set_picture_flip(int value);

/**
   @brief ͼ�����ҵ�ת
   @param value [IN] ON:0x02 OFF:0x03
 */
int swpa_camera_basicparam_set_lr_reverse(int value);

/**
   @brief ��ȡ����̼��汾��С
   @param pDomeVersion [OUT] bit:0-7 �Ͱ汾�ֶ� 8-15�߰汾�ֶ�
 */
int swpa_camera_ball_get_dome_version(int *pDomeVersion);

/**
   @brief ��ȡ��о�汾��Ϣ
   @param pCamVersion [OUT] byte0-1:VenderID byte2-3:ModelID byte4-5:ROM version
 */
int swpa_camera_ball_get_camera_version(char *pCamVersion);

#ifdef __cplusplus
}
#endif

#endif // _SWPA_CAMERA_H_

