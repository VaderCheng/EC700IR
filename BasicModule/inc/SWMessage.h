///////////////////////////////////////////////////////////
//  SWMessage.h
//  Implementation of the Class CSWMessage
//  Created on:      27-����-2013 16:29:45
//  Original author: huanggc
///////////////////////////////////////////////////////////

#if !defined(EA_3172651B_B5B4_4ca8_8467_58A8FE88DC5E__INCLUDED_)
#define EA_3172651B_B5B4_4ca8_8467_58A8FE88DC5E__INCLUDED_
#ifdef WIN32
#include <windows.h>
#endif
#include "SWObjectList.h"
#include "SWClassFactory.h"
#include "SWMessageStruct.h"

/**
 * @brief ��Ϣ������࣬������Ҫ������Ϣ��ģ�鶼����̳д�ģ��
 */
typedef DWORD WPARAM;
typedef DWORD LPARAM;

/* BEGIN: Modified by huangdachang, 2015/8/14 
�ر�ע��: �Ժ��������ö�ٱ����з�����ϢId��Ҫ�ں��������Ӧ��ע�ͣ�ע����Ҫ��������wParam��lParam����Ϣ��ʽ��
�������ܡ���Ϣ�ṹ�����ڵ�λ�õȣ�ͬʱ����Ϣ�ṹ����е�ÿһ���ֶΣ���������ϸ��˵����ʹ��
������һ����֪�����ʹ�������Ϣ��ע�͸�ʽ����:
enum{

//Set��Ϣ����������ע��:
��ϢID1,//[type:SET][wParam:STPhoneMig*][lParam:sizeof(STPhoneMig)][local:<��Ϣ�ṹ�����ڵ�ͷ�ļ�>][desc:<��������>]
��ϢID2,//[type:SET][wParam:INT][lParam:0][local:<��Ϣ�ṹ�����ڵ�ͷ�ļ�>][desc:<��������>]

//Get��Ϣ����������ע��:
��ϢID1,//[type:GET][wParam:0][lParam:0][local:<��Ϣ�ṹ�����ڵ�ͷ�ļ�>][desc:<��������>]
��ϢID2,//[type:GET][wParam:sizeof(STPhoneMig)][lParam:STPhoneMig*][local:<��Ϣ�ṹ�����ڵ�ͷ�ļ�>][desc:<��������>]

}
*/
enum{

    // �����������ģ��
    MSG_CAMERA_CONTROL_START,

    MSG_INITIAL_PARAM ,                      // ���ó�ʼ������
    MSG_SET_AGCTH ,                          // ����AGC��������ֵ
    MSG_GET_AGCTH ,                          // ��ȡAGC��������ֵ

    MSG_SET_MAXSHU_GAIN,                    // ����AGC�����ż�����
    MSG_GET_MAXSHU_GAIN,                    // ��ȡAGC�����ż�����

    MSG_SET_SHU_RANGE,                    // ����AGC�ع�ʱ�䷶Χ
    MSG_GET_SHU_RANGE,                    // ��ȡAGC�ع�ʱ�䷶Χ

    MSG_SET_GAIN_RANGE,                    // ����AGC���淶Χ
    MSG_GET_GAIN_RANGE,                    // ��ȡAGC���淶Χ

	MSG_SET_AGC_SCENE,						//����AGC�龰ģʽ
	MSG_GET_AGC_SCENE,						//��ȡAGC�龰ģʽ

    MSG_SET_MINSHU_GAIN,                    // ����AGC��С���ż�����
    MSG_GET_MINSHU_GAIN,                    // ��ȡAGC��С���ż�����

    MSG_SET_AGCZONE,						// ����AGC�������
    MSG_GET_AGCZONE,						// ��ȡAGC�������

    MSG_SET_AGCGAIN,                        // �ֶ�AGC����ָ��
    MSG_GET_AGCGAIN,                        // ��ȡ��ǰ����
    MSG_SET_AGCENABLE,                      // �Զ�����ʹ��
    MSG_GET_AGCENABLE,                      // ��ȡ�Զ�����ʹ��
    MSG_SET_SHUTTER,                        // ���ÿ���ָ��
    MSG_GET_SHUTTER,                        // ��ȡ��ǰ����

    MSG_SET_AE_MODE,                        // AE
    MSG_GET_AE_MODE,                        // AE

    MSG_SET_RGBGAIN,                        // �ֶ���ƽ��ָ��
    MSG_GET_RGBGAIN,                        // ��ȡ��ǰRGB����
    MSG_SET_AWBENABLE,                      // �Զ���ƽ��ʹ��
    MSG_GET_AWBENABLE,                      // ��ȡ��ƽ��ʹ��
    MSG_SET_AWB_MODE,
    MSG_GET_AWB_MODE,

    MSG_SET_GAMMA,                          // ٤������ָ��
    MSG_SET_GAMMA_MODE,						// ����٤�����ģʽ����
    MSG_GET_GAMMA_MODE,						// ��ȡ٤�����ģʽ����
    MSG_SET_GAMMA_STRENGTH,                 // ����٤��ǿ��ֵ
    MSG_GET_GAMMA_STRENGTH,                 // ��ȡ٤��ǿ��ֵ
    MSG_SET_GAMMA_ENABLE,
    MSG_GET_GAMMA_ENABLE,

    MSG_SET_DIFFCORRENABLE,                 // ������У��ʹ��

    MSG_SET_SOFT_TOUCH,                     // ����ץ������

	MSG_SET_F1_OUTPUT_TYPE,					//����F1�������

	MSG_SET_CAPTURE_AUTO_PARAM_ENABLE,		//�Զ�ץ�Ĳ���ʹ��
	MSG_GET_CAPTURE_AUTO_PARAM_ENABLE,		//�Զ�ץ�Ĳ���ʹ��
    MSG_CAPTURERGB_ENABLE ,                 // ץ��RGBʹ��
    MSG_CAPTURESHUTTER_ENABLE,              // ץ�Ŀ���ʹ��
    MSG_CAPTUREGAIN_ENABLE,                 // ץ������ʹ��
    MSG_CAPTURESHARPEN_ENABLE,              // ץ����

    MSG_SET_CAPTURE_ENABLE,                 // ץ��ʹ��
    MSG_SET_CAPTURESHUTTER,                 // ץ�Ŀ���
    MSG_GET_CAPTURESHUTTER,                 // ��ȡ��ǰץ������

    MSG_SET_CAPTURE_SHARPEN_ENABLE,         // ץ����ʹ��
    MSG_SET_CAPTURE_SHARPEN_THR,            // ץ������ֵ
    MSG_GET_CAPTURE_SHARPEN_THR,            // ��ȡץ������ֵ

    MSG_SET_CAPTRUEGAIN,                    // ץ����������
    MSG_GET_CAPTUREGAIN,                    // ��ȡץ������

    MSG_SET_CAPTRUERGBGAIN,                 // ����ץ��RGB����
    MSG_GET_CAPTRUERGBGAIN,                 // ��ȡץ��RGB����

    MSG_SET_AGCPARAM,
    MSG_GET_AGCPARAM,
    MSG_SET_DCAPERTUREENABLE,               // �����Զ�DC��Ȧʹ�ܱ�־
    MSG_GET_DCAPERTUREENABLE,               // ��ȡ�Զ�DC��Ȧʹ�ܱ�־

    MSG_ZOOM_DCAPERTURE,       //�Ŵ�DC��Ȧ״̬
    MSG_SHRINK_DCAPERTURE,       //��СDC��Ȧ״̬

	MSG_SET_SHARPENMODE,                    // ������ʹ��ģʽ
    MSG_GET_SHARPENMODE,        //��ȡ��ʹ��ģʽ

    MSG_SET_SHARPEN_ENABLE,                    // ������ʹ��
    MSG_GET_SHARPEN_ENABLE,        //��ȡ��ʹ��

    MSG_SET_SHARPENTHRESHOLD,               // �����񻯷�ֵ
    MSG_GET_SHARPENTHRESHOLD,//��ȡ����ֵ

    MSG_SET_WDR_STRENGTH,                      // ����WDRǿ��ֵ
    MSG_GET_WDR_STRENGTH,//��ȡWDRǿ��ֵ

    MSG_SET_NR_LEVEL,
    MSG_GET_NR_LEVEL,

	MSG_SET_COLORMATRIXMODE,                // ����ͼ����ǿʹ��ģʽ
    MSG_GET_COLORMATRIXMODE,//��ȡͼ����ǿʹ��ģʽ

    MSG_SET_COLORMATRIX_ENABLE,   // ����ͼ����ǿʹ�ܣ����Ͷȡ��Աȶȣ�
    MSG_GET_COLORMATRIX_ENABLE,    //��ȡͼ����ǿʹ�ܣ����Ͷȡ��Աȶȣ�

    MSG_SET_CONTRASTTHRESHOLD,              // ���öԱȶ���ֵ
    MSG_GET_CONTRASTTHRESHOLD,//��ȡ�Աȶ���ֵ

    MSG_SET_SATURATIONTHRESHOLD,            // ���ñ��Ͷ���ֵ
    MSG_GET_SATURATIONTHRESHOLD,//��ȡ���Ͷ���ֵ

    MSG_SET_TEMPERATURETHRESHOLD,           // ɫ��
	MSG_SET_HUETHRESHOLD,                   // ɫ��

	MSG_SET_HAZEREMOVALMODE,                // ���ó���ʹ��ģʽ
	MSG_SET_HAZEREMOVALTHRESHOLD,           // ����ֵ

	MSG_SET_FLASHRATE_ENABLE,
	MSG_SET_CAPTURESYNC_ENABLE,
	MSG_SET_FLASHRATE_PULSE,

	MSG_SET_FILTERSWITCH,                   // �˹�Ƭ�л�
    MSG_GET_FILTERSWITCH,                   // �˹�Ƭ�л�

    MSG_SET_REDLIGHT_ENABLE,//���ú�ƼӺ쿪��
    MSG_GET_REDLIGHT_ENABLE,//��ȡ��ƼӺ쿪��

    MSG_SET_REDLIGHT_RECT,					// ���ú�ƼӺ�����
    MSG_SET_TRAFFIC_LIGTH_LUM_TH, //���ú�ƼӺ�

	MSG_SET_GREENLIGHT_RECT,					// �����̵Ƽ�������
    MSG_SET_TRAFFIC_GREEN_LIGTH_LUM_TH, //�����̵Ƽ���

    MSG_SET_CAPTUREEDGE,    // ���ô���ץ����
	MSG_GET_CAPTUREEDGE,    // ��ȡ����ץ����

    MSG_GET_BARRIER_STATE,

    MSG_SET_SNF_ENABLE,//���ý���SNFʹ��
    MSG_GET_SNF_ENABLE,//��ȡ����SNFʹ��

    MSG_SET_TNF_ENABLE,//���ý���TNFʹ��
    MSG_GET_TNF_ENABLE,//��ȡ����TNFʹ��

    MSG_SET_DENOISE_MODE,//���ý���ģʽ
    MSG_GET_DENOISE_MODE,//��ȡ����ģʽ

    MSG_SET_DENOISE_STATE,//���ý���
    MSG_GET_DENOISE_STATE,//��ȡ����

    MSG_SET_AC_SYNC_ENABLE,//���õ���ͬ��ʹ�ܱ�־
    MSG_GET_AC_SYNC_ENABLE,//��ȡ����ͬ��ʹ�ܱ�־

    MSG_SET_AC_SYNC_DELAY,//���õ���ͬ����ʱֵ
    MSG_GET_AC_SYNC_DELAY,//��ȡ����ͬ����ʱֵ

    MSG_SET_GRAY_IMAGE_ENABLE,//���úڰ�ͼʹ�ܱ�־
    MSG_GET_GRAY_IMAGE_ENABLE,//��ȡ�ڰ�ͼʹ�ܱ�־

    MSG_SET_CVBS_STD,//����CVBS��ʽ
    MSG_GET_CVBS_STD,//��ȡCVBS��ʽ

    MSG_SET_FILL_LIGHT_ENABLE,//���ò����ʹ��
    MSG_GET_FILL_LIGHT_ENABLE,//��ȡ�����ʹ��

    MSG_SET_CAM_TEST,       //����Զ�������
    MSG_SET_CAM_FPGA_REG,       //�������FPGA�Ĵ���
    MSG_GET_CAM_FPGA_REG,       //�����ȡFPGA�Ĵ���

//    MSG_GET_JPEG_QUALITY,//��ʱ
//    MSG_SET_JPEG_QUALITY,//��ʱ

    MSG_SET_EDGE_ENHANCE,//����ͼ���Ե��ǿ
    MSG_GET_EDGE_ENHANCE,

    MSG_GET_MANUAL_VALUE,//��ȡ�ֶ�RGB ��������

    MSG_GET_CAMERA_WORKSTATE,//��ȡ�������״̬

    MSG_GET_M3_DAYNIGHT_STATUS,//��ȡM3�������ϱ�ʶ
	//
	MSG_SET_LED_SWITCH,		//���ò���ƿ���
	MSG_GET_LED_SWITCH,		//��ȡ����ƿ��״̬

	MSG_SET_WDR_LEVEL,  //����WDR�ȼ�

	MSG_SET_TRIGGEROUT_STATE,	//���ô������״̬
	
	MSG_SET_TRIGGEROUT,		//���ô�������� 	F1
	MSG_GET_TRIGGEROUT,		//��ȡ���������״̬(������) 	F1

	MSG_SET_F1_TRIGGEROUT,	//F1�������
	MSG_GET_F1_TRIGGEROUT,	//F1�������
	MSG_SET_ALM_TRIGGEROUT, //ALM�������
	MSG_GET_ALM_TRIGGEROUT, //ALM�������
	
	MSG_SET_EXP_IO,			//���ò���������(������) 	EXP
	MSG_GET_EXP_IO,			//��ȡ����������״̬(������) 	EXP
	
	MSG_SET_ALARM_IO,		//���ñ��������(������) 	ALM
	MSG_GET_ALARM_IO,		//��ȡ���������״̬(������) 	ALM

	MSG_SET_TG_IO,			//����ץ������ڴ�������
	MSG_GET_TG_IO,			//��ȡץ������ڴ�������
	MSG_SET_COLOR_GRADATION, //����ɫ�� 0 �� 0~255��1��16~235.

	MSG_SET_WORK_MODE,		//���ù���ģʽ 0-�շ�վ\�������1-����·ģʽ��6-���Ӿ��죬7-ͣ����

	MSG_SET_EXP_PLUSEWIDTH,

	MSG_SET_TNF_STATE,		//���ý���TNFʹ��,M3��ʵ����
	MSG_GET_TNF_STATE,		//��ȡ����TNFʹ��,M3��ʵ����
	MSG_SET_SNF_STATE,		//���ý���SNFʹ��,M3��ʵ����
	MSG_GET_SNF_STATE,		//��ȡ����SNFʹ��,M3��ʵ����
	// �����ء�
	MSG_INITIAL_DOMECAMERA,
	MSG_RESETDEFAULT_DOMECAMERA,
	MSG_RESTOREFACTORY_DOMECAMERA,
	MSG_GET_DOME_VERSION,
	MSG_GET_CAMERA_VERSION,
	MSG_GET_CAMERA_MODEL,

	MSG_SET_PAN_COORDINATE,
	MSG_GET_PAN_COORDINATE,
	MSG_SET_TILT_COORDINATE,
	MSG_GET_TILT_COORDINATE,
	MSG_SET_PT_COORDINATE,
	MSG_GET_PT_COORDINATE,
	MSG_SET_ZOOM,
	MSG_GET_ZOOM,
	MSG_START_PAN,
	MSG_STOP_PAN,
	MSG_START_INFINITY_PAN,
	MSG_STOP_INFINITY_PAN,
	MSG_START_TILT,
	MSG_STOP_TILT,
	MSG_START_PAN_TILT,
	MSG_STOP_PAN_TILT,
	MSG_SET_PAN_SPEED,
	MSG_GET_PAN_SPEED,
	MSG_SET_TILT_SPEED,
	MSG_GET_TILT_SPEED,
	MSG_SET_FOCUS,
	MSG_GET_FOCUS,
	MSG_SET_FOCUS_MODE,
	MSG_GET_FOCUS_MODE,
	MSG_SET_AF_ENABLE,
	MSG_GET_AF_ENABLE,
	MSG_DO_ONE_FOCUS,
	MSG_MOVE_POINT_TO_CENTER,
	MSG_MOVE_BLOCK_TO_CENTER,
	MSG_TUNE_IRIS,
	MSG_TUNE_ZOOM,
	MSG_TUNE_FOCUS,
	MSG_START_ZOOM,
	MSG_STOP_ZOOM,
	MSG_SET_DEFOG,
	MSG_SET_EXPOCOMP_VALUE,
	MSG_GET_EXPOCOMP_VALUE,

	MSG_SET_IRIS,
	MSG_GET_IRIS,
	MSG_SET_IRIS_MODE,
	MSG_GET_IRIS_MODE,
	MSG_START_VTURNOVER,
	MSG_STOP_VTURNOVER,
	MSG_START_WIPER,
	MSG_STOP_WIPER,
	MSG_SET_WIPER_SPEED,
	MSG_GET_WIPER_SPEED,
	MSG_SET_WIPER_AUTOSTART_TIME,
	MSG_GET_WIPER_AUTOSTART_TIME,
	MSG_SET_LED_MODE,
	MSG_GET_LED_MODE,
	MSG_SET_LED_POWER,
	MSG_GET_LED_POWER,
	MSG_SET_MAX_PAN_SPEED,
	MSG_GET_MAX_PAN_SPEED,
	MSG_SET_MAX_TILT_SPEED,
	MSG_GET_MAX_TILT_SPEED,
	MSG_RESET_DOME_POSITION,

	MSG_SET_PRESET_POS,
	MSG_GET_PRESET_POS,
	MSG_CALL_PRESET_POS,
	MSG_CLEAR_PRESET_POS,
	MSG_SET_PRESET_CHECKPT_FLAG,
	MSG_GET_PRESET_CHECKPT_FLAG,
	MSG_GET_PRESET_NAME_LIST,

	MSG_SET_HSCAN_LEFT_POS,
	MSG_GET_HSCAN_LEFT_POS,
	MSG_SET_HSCAN_RIGHT_POS,
	MSG_GET_HSCAN_RIGHT_POS,
	MSG_START_HSCAN,
	MSG_STOP_HSCAN,
	MSG_SET_HSCAN_PARAM,
	MSG_GET_HSCAN_PARAM,
	MSG_CLEAR_HSCAN,

	MSG_START_FSCAN_RECORD,
	MSG_STOP_FSCAN_RECORD,
	MSG_CALL_FSCAN,
	MSG_STOP_FSCAN,
	MSG_CLEAR_FSCAN,
	MSG_SET_FSCAN_PARAM,
	MSG_GET_FSCAN_PARAM,
	MSG_GET_FSCAN_NAME_LIST,

	MSG_SET_CRUISE,
	MSG_GET_CRUISE,
	MSG_CALL_CRUISE,
	MSG_STOP_CRUISE,
	MSG_CLEAR_CRUISE,

	MSG_SET_WATCHKEEPING,
	MSG_GET_WATCHKEEPING,
	MSG_CLEAR_WATCHKEEPING,
	MSG_START_WATCHKEEPING,
	MSG_STOP_WATCHKEEPING,

	MSG_SET_MASK,
	MSG_GET_MASK,
	MSG_CLEAR_MASK,
	MSG_SHOW_MASK,
	MSG_HIDE_MASK,

	MSG_SET_FREEZEFLAG,
	MSG_GET_FREEZEFLAG,
	MSG_SAVE_FREEZEFLAG,

	MSG_SET_TIMER,
	MSG_GET_TIMER,
	MSG_CLEAR_TIMER,
	MSG_SET_TIMER_FLAG,
	MSG_GET_TIMER_FLAG,

	MSG_SET_IDLE_TIME_TO_SLEEP,
	MSG_GET_IDLE_TIME_TO_SLEEP,
	MSG_SET_SLEEP_FLAG,
	MSG_GET_SLEEP_FLAG,
	MSG_START_SLEEP_COUNTDOWN,
	MSG_GET_SLEEP_STATUS,
	
    MSG_SET_DOME_XML,
	MSG_GET_DOME_XML,	

	MSG_PAUSE,
	MSG_RESUME,

	MSG_GET_CUR_TASK,
    MSG_TOUCH_DOME_CAMERA,
    MSG_CAMERA_DO_CMD,
    MSG_CAMERA_CONTROL_END,

    //JPEGѹ��ģ��
    MSG_JPEGENCODE_START,
    MSG_JPEGENCODE,         //JPEGѹ��
	MSG_JPEGENCODE_COMPRESSRATE,	//������Ƶ��ѹ����
	MSG_JPEGENCODE_GET_COMPRESSRATE,
	MSG_JPEGENCODE_COMPRESSRATE_CAPTURE, //����ץ��ͼѹ����
	MSG_JPEGENCODE_GET_COMPRESSRATE_CAPTURE,
	MSG_SET_AUTO_JPEG_COMPRESS_ENABLE,
    MSG_GET_AUTO_JPEG_COMPRESS_ENABLE,//�Զ�jpegѹ��ʹ��       INT
    MSG_SET_AUTO_JPEG_COMPRESS_PARAM,//��ȡ�Զ�ѹ��ͼƬ�����ޡ����ޡ�ͼƬ��С  		һά���� 3
    MSG_GET_AUTO_JPEG_COMPRESS_PARAM,//��ȡ�Զ�ѹ��ͼƬ�����ޡ����ޡ�ͼƬ��С  		һά���� 3
    MSG_SET_JPEG_COMPRESS_RECT, //����JPEGѹ���Ľ�ͼ����
    MSG_JPEGENCODE_END,

    //�Զ�������ģ��
    MSG_AUTO_CONTROL_START,
    MSG_AUTO_CONTROL_READPARAM,
	MSG_AUTO_CONTROL_GET_REALPARAM,
	MSG_AUTO_CONTROL_GET_ENVPERIOD,	//��ȡʱ����Ϣ��0���죬1����
	MSG_AUTO_SET_MAXAGCSHUTTER,
	MSG_AUTO_CONTROL_SET_AUTO_CAPTURE_PARAM,	//�����Զ�ץ�Ĳ���
    MSG_AUTO_CONTROL_END,

    //ʶ��
    MSG_RECOGNIZE_CTRL_START,
    MSG_RECOGNIZE_PHOTO,
    MSG_RECOGNIZE_GETJPEG,
    MSG_RECOGNIZE_OUTPUTDEBUG,
    MSG_RECOGNIZE_ENABLE_TRIGGER,
    MSG_RECOGNIZE_IOEVENT,
    MSG_RECOGNIZE_MODIFY_PARAM,
    MSG_RECOGNIZE_GET_CARLEFT_COUNT,		//��ȡ����ͳ�Ƽ���
    MSG_RECOGNIZE_CHECKPT,
	MSG_RECOGNIZE_STOP,
	MSG_RECOGNIZE_MODIFY_PECCANCYPARKING_PARAM,
	MSG_RECOGNIZE_ENVPERIOD_CHANGED,
    MSG_RECOGNIZE_GET_DSP_ENV_LIGHT_TYPE,           // ��ȡ DSP �жϵĻ�����0���죬1����2���ϣ�
    MSG_RECOGNIZE_CTRL_END,

	//��������
    MSG_NETCOMMAND_START,
    MSG_NETCOMMAND_GET_PROTOCOL,
    MSG_SET_SYNC_POWER,
    MSG_GET_SYNC_POWER,
    MSG_GET_CAMERA_BASIC_INFO,
    MSG_GET_DEV_BASIC_INFO,
    MSG_GET_CAMERA_STATE,
    MSG_GET_VIDEO_STATE,
    MSG_GET_H264_CAPTION,
    MSG_GET_H264_SECOND_CAPTION,	//��ȡ�ڶ�·H264����
    MSG_GET_JPEG_CAPTION,
    MSG_GET_DEVSTATE,
  //  MSG_NULL,//������

    MSG_NETCOMMAND_END,

	//Application
	MSG_APP_START,
    MSG_APP_SETTIME,//����ʱ��
	MSG_APP_GETTIME,
    MSG_APP_SETIP, //����IP
	MSG_APP_GETIP,
    MSG_APP_SETXML, //�ϴ�XML
	MSG_APP_GETXML,
    MSG_APP_GETWORKMODE_COUNT,
    MSG_APP_GETWORKMODE,                //��ȡ����ģʽ
	MSG_APP_GETWORKMODEINDEX,
    MSG_APP_RESETDEVICE,                //�����豸
    MSG_APP_RESTORE_DEFAULT,            //�ָ�Ĭ��
    MSG_APP_RESTORE_FACTORY,            //�ָ�����
	MSG_APP_SETCHARACTER_ENABLE,
	MSG_APP_GETCHARACTER_ENABLE,
    MSG_APP_GETHVNAME,                  //��ȡ�豸����
    MSG_APP_GETHVID,                    //��ȡ�豸ID
    MSG_APP_GETPRODUCT_NAME,            //��ȡ��������
	MSG_APP_GETVIDEO_COUNT,
	MSG_APP_GETVERSION,

    MSG_APP_GETFIRMWAREVERSION, //��ȡ�̼����汾 new
    MSG_APP_GETNETPACKAGEVERSION, //��ȡnet���汾 new
    MSG_APP_GETBACKUPTVERSION, //��ȡ���ݰ汾 new
    MSG_APP_GETFPGAVERSION, //��ȡfpga�汾 new
    MSG_APP_GETKERNALVERSION, //��ȡKERNAL�汾 new
    MSG_APP_GETUBOOTVERSION, //��ȡuboot�汾 new
    MSG_APP_GETUBLVERSION, //��ȡ ubl �汾 new
    MSG_APP_GETSN, //��ȡSN new
    MSG_APP_GETCPU,//��ȡ�豸CPU״̬ int 0-100 new
    MSG_APP_GETRAM,//��ȡ�豸�ڴ�ʹ����״̬ int 0-100 new
 //   MSG_APP_GETDEVSTATE,//��ȡ�豸CPU���ڴ桢�¶ȡ����ӵ���λ��IP�б��豸ʱ�䡢Ӳ��/SD��ʹ��״̬����λ������ new
    MSG_APP_GET_LOG_LEVEL, //��ȡ��ӡ�ȼ� new
    MSG_APP_SET_LOG_LEVEL, //���ô�ӡ�ȼ�
    MAG_APP_GET_NTP_SERVER_IP,//ntp new
    MAG_APP_SET_NTP_SERVER_IP,//ntp new
    MAG_APP_GET_NTP_ENABLE,
    MAG_APP_SET_NTP_ENABLE,
    MAG_APP_GET_NTP_TIMEZONE,
    MAG_APP_SET_NTP_TIMEZONE,
    MAG_APP_SET_NTP_INTERVAL,
    MAG_APP_GET_NTP_INTERVAL,
 	MSG_APP_HDD_CHECKREPORT, //���豸�˻�ȡӲ��״̬����new

	MSG_APP_GETVRESION_STRING,
	MSG_APP_GETCONNECTED_IP,
	MSG_APP_HDD_FDISK,
	MSG_APP_HDD_CHECK,
	MSG_APP_HDD_STATUS,
	MSG_APP_RESET_COUNT, //��ȡ��λ����
	MSG_APP_RESET_REPORT, //���豸�˻�ȡ������λ��־�ļ�
	MSG_APP_RESET_MODE,
	MSG_APP_GETDEVTYPE,
	MSG_APP_GETCPU_TEMPERATURE,
	MSG_APP_UPDATE_STATUS,
	MSG_APP_GET_RUN_STATUS,

	MSG_APP_RADAR_TRIGGER,	//�״ﴥ��ץ��
	
	MSG_APP_COIL_STATUS,
	MSG_APP_COIL_TRIGGER,	
	MSG_APP_GET_BLACKBOX_MESSAGE,

	MSG_APP_SET_CUSTOMIZED_DEVNAME,	//�����豸����
	MSG_APP_GET_CUSTOMIZED_DEVNAME,	//��ȡ�豸����

	MSG_APP_REINIT_HDD,

	MSG_APP_SET_AWB_WORK_MODE,			//����AWB����ģʽ
	MSG_APP_GET_AWB_WORK_MODE,			//��ȡAWB����ģʽ
	MSG_APP_SET_AUTO_CONTROL_CAMMERA_ALL,		//�����������ģʽ
	MSG_APP_GET_AUTO_CONTROL_CAMMERA_ALL,		//��ȡ�������ģʽ
	MSG_APP_SET_H264_RESOLUTION,						//����H264�ֱ���
	MSG_APP_GET_H264_RESOLUTION,						//��ȡH264�ֱ���

	MSG_APP_REMOTE_PTZ_CONTROL,
	MSG_APP_REMOTE_GET_PTZ_INFO,
	//MSG_APP_REMOTE_SET_PTZ_INFO,

	MSG_APP_GET_28181_PARAM,//�������Ϣ
    MSG_APP_SET_28181_QUIT,	//�������Ϣ������GB28181ģ���˳�,mark
    MSG_APP_REBOOT,			//�������Ϣ
	MSG_APP_GET_RTSP_PARAM, //��RTSP����ͨ��
	MSG_APP_GET_ONVIF_PARAM,	//��ONVIF����ͨ��
	MSG_APP_SET_DENOISE_STATUS,	//�������Ϣ ���ý������
	
	
	MSG_APP_END,

	// DM368��ؿ���
	MSG_DM368_CTRL_START,
	MSG_DM368_INIT_MENU,
	MSG_DM368_MENU_LEFT,
	MSG_DM368_MENU_RIGHT,
	MSG_DM368_MENU_UP,
	MSG_DM368_MENU_DWON,
	MSG_DM368_SYNC_TIME,
	MSG_DM368_HEARTBEAT,
	MSG_DM368_CHAR_OVERLAY,
	MSG_DM368_MENU_OK,
	MSG_DM368_MENU_CANCEL,
	MSG_DM368_GET_VERSION,
	MSG_DM368_CTRL_END,

	//H264�洢
	MSG_H264HDD_FILTER_CTRL_START,
	MSG_H264HDD_FILTER_SET_TRANSMITTING_TYPE,
	MSG_H264HDD_FILTER_HISTORY_FILE_TRANSMITTING_START,
	MSG_H264HDD_FILTER_HISTORY_FILE_TRANSMITTING_STOP,
	MSG_H264HDD_FILTER_REALTIME_TRANSMITTING_START,
	MSG_H264HDD_FILTER_REALTIME_TRANSMITTING_STOP,
	MSG_H264HDD_FILTER_GET_HISTORY_FILE_TRANSMITTING_STATUS,
	MSG_H264HDD_FILTER_TRIGGER_VIDEO_SAVING,	
	MSG_H264HDD_FILTER_GB28181_VIDEOSAVING_ENABLE,
	MSG_H264HDD_FILTER_GB28181_VIDEOSEND_SCALE,
    MSG_H264HDD_FILTER_GB28181_FILE_TRANSMITTING_START,
    MSG_H264HDD_FILTER_GB28181_FILE_TRANSMITTING_STOP,
	MSG_H264HDD_FILTER_CTRL_END,
	
	//H264GB28181FILTER
	MSG_GB28181_FILTER_BEGIN,
    MSG_GB2818_TRANSFORM_COMMAND,
	MSG_GB28181_FILTER_END,

	//ʶ�����洢
	MSG_RESULTHDD_FILTER_CTRL_START,
	MSG_RESULTHDD_FILTER_SET_TRANSMITTING_TYPE,
	MSG_RESULTHDD_FILTER_HISTORY_FILE_TRANSMITTING_START,
	MSG_RESULTHDD_FILTER_HISTORY_FILE_TRANSMITTING_STOP,
	MSG_RESULTHDD_FILTER_REALTIME_TRANSMITTING_START,
	MSG_RESULTHDD_FILTER_REALTIME_TRANSMITTING_STOP,
	MSG_RESULTHDD_FILTER_GET_HISTORY_FILE_TRANSMITTING_STATUS,
	MSG_RESULTHDD_FILTER_CTRL_END,

	// 12v�������
	MSG_TRIGGER_OUT_BEGIN,
	MSG_TRIGGER_OUT_PROCESS,
    MSG_TRIGGER_OUT_ENABLE,//new
	MSG_TRIGGER_OUT_END,
	
	//JPEG�ַ�����
	MSG_OVERLAY_JPEG_BEGIN,
	MSG_OVERLAY_PROCESS,
	MSG_OVERLAY_DOPROCESS,
	MSG_OSD_GET_JPEG_ENABLE,
    MSG_OSD_GET_JPEG_PLATE_ENABLE,
    MSG_OSD_GET_JPEG_TIME_ENABLE,
    MSG_OSD_GET_JPEG_TEXT,
    MSG_OSD_GET_JPEG_FONNTSIZE,
    MSG_OSD_GET_JPEG_FONT_RBG,
    MSG_OSD_GET_JPEG_POX,
    MSG_OSD_SET_JPEG_ENABLE,
    MSG_OSD_SET_JPEG_PLATE_ENABLE,
    MSG_OSD_SET_JPEG_TIME_ENABLE,
    MSG_OSD_SET_JPEG_TEXT,
    MSG_OSD_SET_JPEG_FONNTSIZE,
    MSG_OSD_SET_JPEG_FONT_RBG,//int 0~7 R 8~15 B 16~23 G
    MSG_OSD_SET_JPEG_POX, //x y
    MSG_GET_DEBUG_JPEG_STATUS,//��ȡ������������
    MSG_SET_DEBUG_JPEG_STATUS,
    MSG_OSD_ENABLE_JPEG_PTZ_OVERLAY,
    MSG_OSD_SET_JPEG_DOMEINFO_OVERLAY,
    MSG_OSD_SET_MJPEG_OVERLAY,		//ʱ�ܻ�ֹͣMJPEG�ַ����ӣ�0-�رգ�1-��(ֻΪ�۲����Ч��)
    MSG_OSD_SET_JPEG_DEV_ID,		//�����û��Զ����豸���
	MSG_OVERLAY_JPEG_END,
	
	//H264�ַ�����
	MSG_OVERLAY_H264_BEGIN,
	MSG_OSD_GET_H264_ENABLE,
    MSG_OSD_GET_H264_PLATE_ENABLE,
    MSG_OSD_GET_H264_TIME_ENABLE,
    MSG_OSD_GET_H264_TEXT,
    MSG_OSD_GET_H264_FONNTSIZE,
    MSG_OSD_GET_H264_FONT_RBG,
    MSG_OSD_GET_H264_POX,
    MSG_OSD_SET_H264_ENABLE,
    MSG_OSD_SET_H264_PLATE_ENABLE,
    MSG_OSD_SET_H264_TIME_ENABLE,
    MSG_OSD_SET_H264_TEXT,
    MSG_OSD_SET_H264_FONNTSIZE,
    MSG_OSD_SET_H264_FONT_RBG,
    MSG_OSD_SET_H264_POX,
    MSG_OSD_SET_H264_DEV_ID,	//�����û��Զ����豸���

    MSG_OSD_ENABLE_H264_PTZ_OVERLAY,
    MSG_OSD_SET_H264_DOMEINFO_OVERLAY,
 	MSG_OVERLAY_H264_END,

		//�ڶ�·H264�ַ�����
	MSG_OVERLAY_H264_SECOND_BEGIN,
	MSG_OSD_GET_H264_SECOND_ENABLE,
    MSG_OSD_GET_H264_SECOND_PLATE_ENABLE,
    MSG_OSD_GET_H264_SECOND_TIME_ENABLE,
    MSG_OSD_GET_H264_SECOND_TEXT,
    MSG_OSD_GET_H264_SECOND_FONNTSIZE,
    MSG_OSD_GET_H264_SECOND_FONT_RBG,
    MSG_OSD_GET_H264_SECOND_POX,
    MSG_OSD_SET_H264_SECOND_ENABLE,
    MSG_OSD_SET_H264_SECOND_PLATE_ENABLE,
    MSG_OSD_SET_H264_SECOND_TIME_ENABLE,
    MSG_OSD_SET_H264_SECOND_TEXT,
    MSG_OSD_SET_H264_SECOND_FONNTSIZE,
    MSG_OSD_SET_H264_SECOND_FONT_RBG,
    MSG_OSD_SET_H264_SECOND_POX,
 	MSG_OVERLAY_H264_SECOND_END,

	//H264����
	MSG_SET_H264_START,
    MSG_SET_H264_BITRATE,
    MSG_GET_H264_BITRATE,
	MSG_SET_H264_I_FRAME_INTERVAL,
	MSG_GET_H264_I_FRAME_INTERVAL,
	MSG_SET_H264_RATE_CONTROL,
	MSG_SET_H264_VBR_DURATION,
	MSG_SET_H264_VBR_SENSITIVITY,
    MSG_SET_H264_SECOND_BITRATE,
    MSG_GET_H264_SECOND_BITRATE,
	MSG_SET_H264_SECOND_I_FRAME_INTERVAL,
	MSG_GET_H264_SECOND_I_FRAME_INTERVAL,
	MSG_SET_H264_SECOND_RATE_CONTROL,
	MSG_SET_H264_SECOND_VBR_DURATION,
	MSG_SET_H264_SECOND_VBR_SENSITIVITY,
	MSG_SET_H264_END,
 	
	// �û�����new
	MSG_USER_BEGIN,
	MSG_USER_LOGIN,
    MSG_USER_GETLIST,
    MSG_USER_ADD,
    MSG_USER_DEL,
    MSG_USER_CHANGE,
    MSG_USER_RESTORE_DEFAULT,
    MSG_USER_MANAGE,
	MSG_USER_GETPASSWORD,
	MSG_USER_END,
	
	    //���ڲ���new
    MSG_COM_TEST_BEGIN,
    MSG_COM_TEST_ENABLE,
    MSG_COM_TEST_DISABLE,
	MSG_COM_OUTPUT_CTRL_ENABLE,		//���ܿض�̬����
    MSG_COM_TEST_END,

    MSG_GB28181_BEGIN,
    MSG_GB28181_SEND_ALARM,
    MSG_GB28181_PARAMTER_CHANGE_ALARM,
    MSG_GB28181_END,
    MSG_GB28181_QUIT,
	MSG_RTSP_BEGIN,
	MSG_RTSP_SEND_PARAM,
	MSG_RTSP_APP_RESTART,
	MSG_RTSP_END,

	MSG_ONVIF_BEGIN,
	MSG_ONVIF_SET_PARAM,
	MSG_ONVIF_APP_RESTART,
	MSG_APP_REMOTE_SET_PTZ_INFO,
	MSG_APP_REMOTE_SET_USER_INFO,
	MSG_ONVIF_END,

	MSG_SOURCE_BEGIN,
    MSG_SOURCE_GET_JPEG_RAW_FPS,
    MSG_SOURCE_GET_H264_RAW_FPS,
    MSG_SOURCE_GET_H264_SECOND_RAW_FPS,
    MSG_SOURCE_END,

    MSG_H264_BEGIN,
    MSG_H264_DO_PREVIEW,
    MSG_H264_END,

	MSG_H264_QUEUE_BEGIN,
    MSG_H264_QUEUE_GET_VIDEO,
    MSG_H264_QUEUE_END,

	//MD5����
    MSG_MD5_ENCRYPT_BEGIN,
    MSG_MD5_ENCRYPT_PROCESS,
    MSG_SET_ENCRYPT_PARAM,
    MSG_GET_ENCRYPT_CODE,
    MSG_MD5_ENCRYPT_END,
    
    //���Filter
    MSG_RESULT_FILTER_BEGIN,
    MSG_RESULT_FILTER_OVERLAY_POS_CHANGED,
    MSG_RESULT_FILTER_END,

	MSG_2A_BEGIN,
			//  �����������
    MSG_2A_SET_EXP,      		//  �����ع�ʱ��
    MSG_2A_GET_EXP,            //  ��ȡ�ع�ʱ��
    MSG_2A_SET_GAIN,           //  ��������
    MSG_2A_GET_GAIN,           //  ��ȡ����
    MSG_2A_SET_RGB,            //  ����RGB
    MSG_2A_GET_RGB,            //  ��ȡRGB
		//  ����Զ�������
    MSG_2A_SET_AE_STATE,       //  ����AEʹ��
    MSG_2A_GET_AE_STATE,       //  ��ȡAEʹ�ܱ�־
    MSG_2A_SET_AE_THRESHOLD,   //  ����AE����ֵ
    MSG_2A_GET_AE_THRESHOLD,   //  ��ȡAE��ǰ����ֵ
    MSG_2A_SET_AE_EXP_RANGE,   //  ����AE�ع�ʱ�䷶Χ
    MSG_2A_GET_AE_EXP_RANGE,   //  ��ȡAE�ع�ʱ�䷶Χ
    MSG_2A_SET_AE_ZONE,        //  ����AE�������
    MSG_2A_GET_AE_ZONE,        //  ��ȡAE�������
    MSG_2A_SET_AE_GAIN_RANGE,  //  ����AE���淶Χ
    MSG_2A_GET_AE_GAIN_RANGE,  //  ��ȡAE���淶Χ
    MSG_2A_SET_SCENE,		//
    //MSG_2A_GET_SCENE,		//
    MSG_2A_SET_AWB_STATE,      //  ����AWBʹ��
    MSG_2A_GET_AWB_STATE,      //  ��ȡAWBʹ��
//  ���ͼ����
    MSG_2A_SET_WDR_STRENGTH,   //  ����WDRǿ��ֵ
    MSG_2A_GET_WDR_STRENGTH,   //  ��ȡWDRǿ��ֵ
    //MSG_2A_SET_SHARPNESS_STATE,//  ������ʹ��	//todo�Ѿ���֧��
    //MSG_2A_GET_SHARPNESS_STATE,//  ��ȡ��ʹ��
    //MSG_2A_SET_SHARPNESS,      //  ������ֵ		//todo�Ѿ���֧��
    //MSG_2A_GET_SHARPNESS,      //  ��ȡ��ֵ
    MSG_2A_SET_SATURATION_CONTRAST_STATE,  //  ���ñ��Ͷȡ��Աȶ�ʹ��
    MSG_2A_GET_SATURATION_CONTRAST_STATE,  //  ��ȡ���Ͷȡ��Աȶ�ʹ��
    MSG_2A_SET_SATURATION,     //  ���ñ��Ͷ�ֵ
    MSG_2A_GET_SATURATION,     //  ��ȡ���Ͷ�ֵ
    MSG_2A_SET_CONTRAST,       //  ���öԱȶ�ֵ
    MSG_2A_GET_CONTRAST,       //  ��ȡ�Աȶ�ֵ
    MSG_2A_SET_TNF_STATE,		//  ���ý���TNFʹ��,ֻ���û����ã������Ƿ���Ч��2A�㷨�Զ�����
    MSG_2A_GET_TNF_STATE,		//  ��ȡ����TNFʹ��
    MSG_2A_SET_SNF_STATE,		//  ���ý���SNFʹ��,ֻ���û����ã������Ƿ���Ч��2A�㷨�Զ�����
    MSG_2A_GET_SNF_STATE,		//  ��ȡ����SNFʹ��
    //MSG_2A_SET_TNF_SNF_STRENGTH,   //  ���ý���ǿ��ֵ
    //MSG_2A_GET_TNF_SNF_STRENGTH,   //  ��ȡ����ǿ��ֵ

	//MSG_2A_SET_TRAFFIC_LIGTH_ENHANCE_STATE,    //  ���ú�ƼӺ�ʹ�� //todo�Ѿ���֧��
    //MSG_2A_GET_TRAFFIC_LIGTH_ENHANCE_STATE,    //  ��ȡ��ƼӺ�ʹ��

	MSG_2A_SET_TRAFFIC_LIGTH_ENHANCE_REGION,   	//  ���üӺ������޶�ȡ���
   	MSG_2A_SET_TRAFFIC_LIGTH_LUM_TH,    		// ���ú�ƼӺ�������ֵ
    MSG_2A_SET_GAMMA_ENABLE,  	// ����Gamma ʹ��
    MSG_2A_GET_GAMMA_ENABLE,  	// ��ȡGammaʹ��
    MSG_2A_SET_GAMMA,  			// ����Gamma
    MSG_2A_GET_GAMMA,  			// ��ȡGamma
    MSG_2A_SET_EDGE_ENHANCE, 	// ����ͼ���Ե��ǿ
    MSG_2A_GET_EDGE_ENHANCE, 	//����
//  ���IO����
    MSG_2A_SET_DCIRIS_AUTO_STATE,   //  �����Զ�DC��Ȧʹ��
    MSG_2A_GET_DCIRIS_AUTO_STATE,   //  ��ȡ�Զ�DC��Ȧʹ�ܱ�־
    MSG_2A_ZOOM_DCIRIS,   		//   �Ŵ�DC��Ȧ							//qinjj mark
    MSG_2A_SHRINK_DCIRIS,   	//  ��СDC��Ȧ
    MSG_2A_SET_FILTER_STATE,   	//  �˹�Ƭ�л�
    MSG_2A_SET_AC_SYNC_STATE,  	//  ���õ���ͬ��ʹ��
    MSG_2A_GET_AC_SYNC_STATE,  	//  ��ȡ����ͬ��ʹ��
    MSG_2A_SET_AC_SYNC_DELAY,  	//  ���õ���ͬ����ʱ
    MSG_2A_GET_AC_SYNC_DELAY,  	//  ��ȡ����ͬ����ʱֵ

	MSG_2A_SET_IO_ARG,			// ���ö������IO�ڲ���
    MSG_2A_GET_IO_ARG,			// ��ȡ�������IO�ڲ���
		
	MSG_2A_SET_EDGE_MOD,		// ���ô���ץ���ع���ģʽ
	MSG_2A_GET_EDGE_MOD,		// ��ȡ����ץ���ع���ģʽ
	
		//  ����ɼ�����
    //MSG_2A_GET_H264_RES,       //  ��ȡH264�ֱ���
    MSG_2A_SET_GRAY_IMAGE_STATE,   //  ���úڰ�ͼʹ��
    MSG_2A_GET_GRAY_IMAGE_STATE,   //  ��ȡ�ڰ�ͼʹ�ܱ�־
		//  �������
    //MSG_2A_SET_H264_BITRATE,   //  ����H264����
    //MSG_2A_GET_H264_BITRATE,   //  ��ȡH264����
    //MSG_2A_SET_H264_BITRATE2,	//	���õڶ�·H264����
    //MSG_2A_GET_H264_BITRATE2,	//	��ȡ�ڶ�·H264����
    //MSG_2A_SET_H264_IFRAME_INTERVAL,    //  ����H264 I֡���
    //MSG_2A_GET_H264_IFRAME_INTERVAL,    //  ����H264 I֡���
    //MSG_2A_SET_JPEG_QUALITY,   //  ������Ƶ��JPEGѹ����
    //MSG_2A_GET_JPEG_QUALITY,   //  ������Ƶ��JPEGѹ����
    //MSG_2A_SET_CVBS_STD,       //  ����CVBS��ʽ
    //MSG_2A_GET_CVBS_STD,       //  ��ȡCVBS��ʽ
		//  DSP����
    //MSG_2A_SET_VIDEO_STREAM_RECOGNIZE_PARAMS,  //  ��Ƶ��ʶ�������ʼ��
    //MSG_2A_SET_PICTURE_SNAP_RECOGNIZE_PARAMS,  //  ץ��ͼʶ�������ʼ��

		//  ����
    MSG_2A_SET_FPGA_REG,
    MSG_2A_SET_FPGA_READ_REG_ADDR,				//todo:���ڽ��̼�ͨ�ŵ����ƣ���Ҫ��ȡ�Ĵ���֮ǰͨ��һ������������Ҫ��ȡ�ļĴ����ĵ�ַ
    MSG_2A_GET_FPGA_REG,

	//�������ģʽ
	MSG_2A_SET_WORKMODE,

	MSG_2A_SOFT_CAPTURE,

	MSG_2A_SET_FLASH_GATE,
	
	MSG_2A_GET_VERSION,
	
	//MSG_2A_SET_CAP_AUTO_PARAM_ENABLE,			//ץ�Ĳ����Զ���
	//MSG_2A_GET_CAP_AUTO_PARAM_ENABLE,
	MSG_2A_SET_CAP_SHUTTER,
	MSG_2A_SET_CAP_GAIN,
	MSG_2A_SET_CAP_SHARPEN,
	MSG_2A_SET_CAP_RGB,

	MSG_2A_GET_DAY_NIGHT,
	MSG_2A_SET_BACKLIGHT,
	
	MSG_2A_SET_AWB_MODE,

	//MSG_2A_SET_H264_IFRAME_INTERVAL2,
    //MSG_2A_GET_H264_IFRAME_INTERVAL2,
    MSG_2A_SET_RELAY_STATE,    //  �̵�������
    
	MSG_2A_END,
	
	MSG_MAX
};


//��Ϣӳ��궨��
//#define SW_BEGIN_MESSAGE_MAP(theClass, baseClass) virtual HRESULT OnMessage(DWORD dwID,WPARAM wParam,LPARAM lParam){HRESULT hr = E_NOTIMPL;if(swpa_strcmp(#theClass, #baseClass)){hr = baseClass::OnMessage(dwID, wParam, lParam);}if(hr == E_NOTIMPL){switch(dwID){
//#define SW_MESSAGE_HANDLER(dwID, func) case dwID : hr = func(wParam,lParam); break;

/* BEGIN: Modified by huangdch, 2015/8/14 
*��Ϣ�궨��:������Ϣӳ�����Ϣ����ע��*/
#define SW_BEGIN_MESSAGE_MAP(theClass, baseClass) virtual HRESULT OnMessage(DWORD  mID,WPARAM wParam,LPARAM lParam){HRESULT hr = E_NOTIMPL;if(swpa_strcmp(#theClass, #baseClass)){hr = baseClass::OnMessage( mID, wParam, lParam);}if(hr == E_NOTIMPL){switch( mID){case 0 :
#define SW_MESSAGE_HANDLER(dwID, func) case dwID : if (mID) {hr = func(wParam,lParam);} else {hr=MessageRegister(dwID, #func);} if (mID) break;
#define SW_END_MESSAGE_MAP()default: break;}} return hr;}

/* BEGIN: Modified by huangdch, 2015/8/14 
*������ܺ���������OnSet��OnGet��ͷ�Ĺ淶,��ͨ�����������ĵ�����������ָ��,֧��MSG_TYPE_SET��MSG_TYPE_GET����ֵ*/
#define SW_MESSAGE_TYPE_HANDLER(dwID, func, msgType) case dwID : if (mID) {hr = func(wParam,lParam);} else {hr=MessageRegister(dwID, #func, msgType);} if (mID) break;

/* BEGIN: Modified by huangdch, 2015/8/14 
*�ر�ע��:�̳���Ҫ�ڹ��캯���е���������������ϢId���͵�ע��*/
#define CALL_MESSAGE_REGISTER_FUNCTION() this->OnMessage(0,0,0); this->SharedMemRegister();


//Remote��Ϣӳ��궨��
#define SW_BEGIN_REMOTE_MESSAGE_MAP(theClass, baseClass) virtual HRESULT OnRemoteMessage(DWORD dwID, PVOID pvBuffer, INT iSize){HRESULT hr = E_NOTIMPL;if(swpa_strcmp(#theClass, #baseClass)){hr = baseClass::OnRemoteMessage( dwID,  pvBuffer, iSize);}if(hr == E_NOTIMPL){switch(dwID){
#define SW_REMOTE_MESSAGE_HANDLER(dwID, func) case dwID : hr = func(pvBuffer,iSize); break;
#define SW_END_REMOTE_MESSAGE_MAP()default: break;}}return hr;}

/* BEGIN: Modified by ����, 2015/8/14 
*�����ڴ���Ϣ�ڵ�
*/

#define MSG_SHM_WAIT_RESP_TIME  2000  //�ȴ���ϢӦ������ʱʱ��,��λ����,-1��ʾһֱ�ȴ�,���Ҫ����ʵ��������е���
#define MSG_SHM_UPDATE_TIME 	5     //����ƽ̨�ڴ��ʱ����,��λ��
#define MSG_SHM_DATA_BUF_LEN	1024  //���̼�ת����Ϣ����󳤶�
#define MSG_TYPE_SET  1 			  //��Ϣ����ʽ:set��ʽ
#define MSG_TYPE_GET  2  			  //��Ϣ����ʽ:Get��ʽ
#define MSG_SHM_SEM_FULL_START   0   //sem_full�źŵ���ʼλ��[0],��ʼֵΪ0
#define MSG_SHM_SEM_EMPTRY_START   1   //sem_full�źŵ���ʼλ��[1],��ʼֵΪ MSG_SHM_NODE_NUMBER
#define MSG_SHM_SEM_FULL_RET_START   2  //sem_full_ret�źŵ���ʼλ��[2~7],��ʼֵΪ 0
#define MSG_SHM_SEM_EMPTRY_RET_START  8 //sem_full_ret�źŵ���ʼλ��[8~13],��ʼֵΪ 1

#define	MSG_SHM_PROCESS_MUTEX	 10002 
#define	MSG_SHM_REGISTER_KEY	 10003
#define	MSG_SHM_REGISTER_START_KEY     20000

typedef struct _STSharedMemRegisterNode
{
	char szProcName[MSG_SHM_PROCESS_LEN]; //������
	DWORD id;  //��������Ӧ��id
	int iProcessId; //����id
}STSharedMemRegisterNode;


typedef struct _STSWShmMsg
{
	INT iProcessId;//����Id
	ULONG lInsertTime;//��Ϣд���ʱ��,time_t��,ʵ����Ϣ�ȵ��ȴ���,����Ƿ��ط�����Ӧ����Ϣ,������0
	INT iState;//�ڵ�״̬ 0 ��ʾ��д,1��ʾ��������,2��ʾ��������,3��ʾ��������
	DWORD dwId;//��ϢID
	DWORD iParamLen;//�������ĳ���
	CHAR szParamBuf[MSG_SHM_DATA_BUF_LEN];//������
	INT iRet;//��Ϣ���������͵�ʱ���ʾ�Ƿ���Ҫ����ֵ0��ʾ��Ҫ,1��ʾ����Ҫ����ֵ
}STSWShmMsg;

/* BEGIN: Modified by ����, 2015/8/14 
*�����ڴ���Ϣ������
*/
typedef struct _STSWShmMessage
{
	INT m_KeyId;  //�����ڴ��keyֵ
	STSWShmMsg  m_shmMsgNode[MSG_SHM_NODE_NUMBER];//�����ڴ����Ľڵ���
}STSWShmMessage;

class CSWMessage
{	//��Ϣ��ṹ
	class CMSG : public CSWObject
	{
	public:
		CMSG(DWORD dwID, WPARAM wParam, LPARAM lParam):m_dwID(dwID),m_wParam(wParam),m_lParam(lParam){}
		virtual ~CMSG(){}
		DWORD m_dwID;		 //��Ϣid
		WPARAM  m_wParam;  //��Ϣ�帽����Ϣ1
		LPARAM  m_lParam;  //��Ϣ�帽����Ϣ2
	};
public:
	CSWMessage(DWORD dwStartID, DWORD dwEndID);
	virtual ~CSWMessage();

	/**
	 * @brief ������ʽ������Ϣ
	 * @param [in] id ��Ϣ��id��
	 * @param [in] wParam ��Ϣ�����1
	 * @param [in] lParam ��Ϣ�����2
	 * @return S_OK����ɹ�������ֵΪ�������
	 */
	static HRESULT PostMessage(DWORD id, WPARAM wParam = 0, LPARAM lParam = 0);
	/**
	 * @brief ����ʽ������Ϣ
	 * @param [in] id ��Ϣ��id��
	 * @param [in] wParam ��Ϣ�����1
	 * @param [in] lParam ��Ϣ�����2
	 * @return S_OK����ɹ�������ֵΪ�������
	 */
	static HRESULT SendMessage(DWORD id, WPARAM wParam = 0, LPARAM lParam = 0);
	
	static HRESULT SendRemoteMessage(DWORD id, PVOID pvBuffer = NULL, INT iSize = 0);
public:
	/**
	 * @brief ��Ϣ�����麯��
	 * @param [in] wParam ��Ϣ�����1
	 * @param [in] lParam ��Ϣ�����2
	 * @return �ɹ�����S_OK������ֵ����ʧ��
	 */
	virtual HRESULT OnMessage(DWORD id, WPARAM wParam, LPARAM lParam);

	virtual HRESULT OnRemoteMessage(DWORD id, PVOID pvBuffer, INT iSize);
	
protected:
	/**
	 * @brief ��Ϣ�����̺߳���
	 * @param [in] pvParam �̺߳���
	 */
	static void* OnMSG(void *pvParam);
	
	/**
	 * @brief �����socket��Ϣ�����߳�
	 * @param [in] pvParam �̺߳���
	 */
	static void* OnRemoteMSG(void *pvParam);

protected:

	/*****************************************************************************
	 �� �� ��  : CSWMessage.InitShmMsg
	 ��������  : �����ڴ��ʼ��
	 �������  : ��
	 �������  : ��
	 �� �� ֵ  : HRESULT
	 ���ú���  : 
	 ע������  : 
	 
	 �޸���ʷ      :
	  1.��    ��   : 2015��9��4��
		��    ��   : huangdch
		�޸�����   : �����ɺ���
	*****************************************************************************/
	HRESULT InitShmMsg();

	/*****************************************************************************
	 �� �� ��  : CSWMessage.MessageRegister
	 ��������  : ��Ϣ������ע��ӿ�
	 �������  : INT id 		   
				 CHAR* szFunName  OnSet��ͷ��ʾSet��Ϣ,OnGet��ͷ��ʾGet��Ϣ   
				 INT msgFlag = -1  �Զ�����Ϣ���ͱ�ʶ,Ĭ�ϸ��ݺ�����ʶ��,1��ʾset��Ϣ,2��ʾget��Ϣ
	 �������  : ��
	 �� �� ֵ  : virtual
	 ���ú���  : 
	 ע������  : ���������������Ҫ��,��Ҫ�������������
	 
	 �޸���ʷ      :
	  1.��    ��   : 2015��9��4��
		��    ��   : huangdch
		�޸�����   : �����ɺ���
	*****************************************************************************/
	virtual HRESULT MessageRegister(INT id, const CHAR* szFunName, INT msgFlag = -1);

	/*****************************************************************************
	 �� �� ��  : CSWMessage.SharedMemRegister
	 ��������  : �����ڴ�Idע��ӿ�
	 �������  : ��
	 �������  : ��
	 �� �� ֵ  : virtual
	 ���ú���  : 
	 ע������  : 
	 
	 �޸���ʷ      :
	  1.��    ��   : 2015��9��4��
		��    ��   : huangdch
		�޸�����   : �����ɺ���
	*****************************************************************************/
	virtual HRESULT SharedMemRegister();

	/*****************************************************************************
	 �� �� ��  : CSWMessage.SendProcessMessage
	 ��������  : ���̼���Ϣͨ�Žӿ�
	 �������  : DWORD id ��Ϣid	  
				 DWORD wParam  
				 DWORD lParam  
				 INT& iRet     
	 �������  : ��
	 �� �� ֵ  : static
	 ���ú���  : 
	 ע������  : 
	 
	 �޸���ʷ      :
	  1.��    ��   : 2015��9��4��
		��    ��   : huangdch
		�޸�����   : �����ɺ���
	*****************************************************************************/
	static HRESULT SendProcessMessage(DWORD id, DWORD wParam, DWORD lParam, INT& iRet);

	/*****************************************************************************
	 �� �� ��  : CSWMessage.FindKey
	 ��������  : ������ϢId���Ҷ�Ӧ�Ĺ����ڴ�keyֵ
	 �������  : DWORD id  
	 �������  : ��
	 �� �� ֵ  : HRESULT -1��ʾ����ʧ�� ����ֵ�ɹ�
	 ���ú���  : 
	 ע������  : 
	 
	 �޸���ʷ      :
	  1.��    ��   : 2015��9��4��
		��    ��   : huangdch
		�޸�����   : �����ɺ���
	*****************************************************************************/
	static HRESULT FindKey(DWORD id);

	/*****************************************************************************
	 �� �� ��  : CSWMessage.SingleSend
	 ��������  : ��ָ�����̷�����Ϣ
	 �������  : INT iKeyId    
				 DWORD id      
				 DWORD wParam  
				 DWORD lParam  
				 INT result    
	 �������  : ��
	 �� �� ֵ  : HRESULT 0�ɹ� ��0ʧ��
	 ���ú���  : 
	 ע������  : 
	 
	 �޸���ʷ      :
	  1.��    ��   : 2015��9��4��
		��    ��   : huangdch
		�޸�����   : �����ɺ���
	*****************************************************************************/
	static HRESULT SingleSend(INT iKeyId, DWORD id, DWORD wParam, DWORD lParam, INT &result);

	/*****************************************************************************
	 �� �� ��  : CSWMessage.MassSend
	 ��������  : ��ƽ̨�����еĽ��̷�����Ϣ
	 �������  : INT& result  
	 �������  : ��
	 �� �� ֵ  : HRESULT 0�ɹ� ��0ʧ��
	 ���ú���  : 
	 ע������  : 
	 
	 �޸���ʷ      :
	  1.��    ��   : 2015��9��4��
		��    ��   : huangdch
		�޸�����   : �����ɺ���
	*****************************************************************************/
	static HRESULT MassSend(INT& result);

	/*****************************************************************************
	 �� �� ��  : CSWMessage.AnalyResponse
	 ��������  : �����㲥Ӧ�����Ϣ
	 �������  : CHAR* szResult MassSend()���ص���Ϣ  
	 �������  : ��
	 �� �� ֵ  : static
	 ���ú���  : 
	 ע������  : 
	 
	 �޸���ʷ      :
	  1.��    ��   : 2015��9��4��
		��    ��   : huangdch
		�޸�����   : �����ɺ���
	*****************************************************************************/
	static HRESULT AnalyResponse(CHAR* szResult);

	/*****************************************************************************
	 �� �� ��  : CSWMessage.FindProcByKeyId
	 ��������  : ����KeyId������ؽ��̵���Ϣ
	 �������  : const int KeyId	 

	 �������  : ��
	 �� �� ֵ  : HRESULT <0ʧ�� ����ֵ��Ӧ������±�
	 ���ú���  : 
	 ע������  : 
	 
	 �޸���ʷ      :
	  1.��    ��   : 2015��9��4��
		��    ��   : huangdch
		�޸�����   : �����ɺ���
	*****************************************************************************/
	static HRESULT FindProcByKeyId(const DWORD dwKeyId);

	/*****************************************************************************
	 �� �� ��  : CSWMessage.UpdateProcByKeyId
	 ��������  : ����id��ע����ǿ�Ƹ���ƽ̨����Ϣ
	 �������  : const int KeyId
	 			 int flag  < 0 ��ʾ���ٲ�һ��ע���� >=0 ��ʾע������Ӧ�������±�
	 �������  : ��
	 �� �� ֵ  : static
	 ���ú���  : 
	 ע������  : 
	 
	 �޸���ʷ      :
	  1.��    ��   : 2015��9��5��
	    ��    ��   : huangdch
	    �޸�����   : �����ɺ���

	*****************************************************************************/
	static HRESULT UpdateProcByKeyId(const DWORD dwKeyId, INT iFlag = -1);

	/*****************************************************************************
	 �� �� ��  : CSWMessage.OnProMSG
	 ��������  : ���̼���Ϣ�����߳�
	 �������  : void* pvParam  
	 �������  : ��
	 �� �� ֵ  : static
	 ���ú���  : 
	 ע������  : 
	 
	 �޸���ʷ      :
	  1.��    ��   : 2015��9��4��
		��    ��   : huangdch
		�޸�����   : �����ɺ���
	*****************************************************************************/
	static void* OnProMSG(void* pvParam);
	
private:
	static CSWThread* m_pMsgThread;
	static CSWThread* m_pRemoteMsgThread;	//��������socket��Ϣ
	static CSWObjectList m_objMSGList;

	static CSWList<CSWMessage *>m_clsMSGList;
	static CSWRWMutex m_rwLock;
	DWORD m_dwStartID;
	DWORD m_dwEndID;
	
	static CSWUDPSocket m_udpSocket;

private:
	/* BEGIN: Modified by huangdch, 2015/8/14 
	*���̼�ͨ���õ�����ر���
	*/
	static CSWMutex m_tMsgMutex; //������Ϣע����
	static STSharedMemRegisterNode* m_SharedSharedMemRegister; //�����ڴ�ע����Ϣ

	static CSWList<STMessageRangNode*>m_MSGRangList;//����������Ϣid��Ӧ��key��Χ,�ٸ��ݷ�Χ��vec_prcess���ҳ���Ӧ�Ĺ����ڴ�����ָ��

	static STSWShmMessage* m_pShm; //�����ڴ����ݽ�����
	static STProcessManageNode* m_ProcessManageNode; //��ǰƽ̨�����н�����ص���Ϣ

	//��ǰ������ص���Ϣ
	static CSWMutex tMutex; 	 //���̼以����
	static CSWSemaphore m_ArySem[MSG_SHM_SEM_TOTAIL];//���̼��ź���
	static CSWRWMutex m_rwProcLock;  //��ǰ���̸��ºͲ�ѯ��д��
	static STProcessManageCur m_ProcessManageCur; //��ǰ���̽ڵ�

	static CSWThread* m_pMsgProcThread; //ר���ٸ�һ���߳���������̼����Ϣ

	static char m_ProcessName[MSG_SHM_PROCESS_LEN]; //��ǰƽ̨�Ľ�����

};
#endif // !defined(EA_3172651B_B5B4_4ca8_8467_58A8FE88DC5E__INCLUDED_)


