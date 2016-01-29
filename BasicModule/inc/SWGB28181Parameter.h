#ifndef __SWGB28181INFO_PARAMETER_H__
#define __SWGB28181INFO_PARAMETER_H__

#include "SWFC.h"

#define NAME_LEN 32
typedef struct _GB28181_info
{
    BOOL enable;
    CHAR server_id[NAME_LEN];
    CHAR server_region[NAME_LEN];
    CHAR server_ip[NAME_LEN];
    CHAR server_port[NAME_LEN];
    CHAR ipc_id[NAME_LEN];
    CHAR ipc_region[NAME_LEN];
    CHAR ipc_username[NAME_LEN];
    CHAR ipc_pwd[NAME_LEN];
    CHAR ipc_alarm[NAME_LEN];
    CHAR ipc_port[NAME_LEN];
    CHAR mediaSend_port[NAME_LEN];
    INT  mediaSend_FPS;
    INT  mediaSend_wight;
    INT  mediaSend_height;
    CHAR szDeviceName[NAME_LEN];
    CHAR szManufacturer[NAME_LEN];/*�豸����*//*Ĭ��ֵ��csenn*/
    CHAR szModel[NAME_LEN];/*�豸�ͺ�*//*Ĭ��ֵ��YF1109*/
    CHAR szFirmware[NAME_LEN];/*�豸�̼��汾*//*Ĭ��ֵ��V1.0*/
    CHAR device_encode[NAME_LEN];/*�Ƿ����*//*ȡֵ��Χ��ON/OFF*//*Ĭ��ֵ��ON*/
    CHAR device_record[NAME_LEN];/*�Ƿ�¼��*//*ȡֵ��Χ��ON/OFF*//*Ĭ��ֵ��OFF*/
    CHAR szOwner[NAME_LEN]; /*�豸����*/
    CHAR szCivilCode[NAME_LEN];/*��������*/
    CHAR szBlock[NAME_LEN]; /*����*/
    CHAR szAddress[NAME_LEN];/*��װ��ַ*/
  //  CHAR device_Status[NAME_LEN];/*�豸״̬*/
    CHAR szLongitude[NAME_LEN];/*����*/
    CHAR szLatitude[NAME_LEN];/*γ��*/
//    CHAR device_Online[NAME_LEN]; /* �Ƿ�����/ONLINE/OFFLINE*/
    CHAR device_guard[NAME_LEN]; /*���� ONDUTY OFFDUTY*/
//    CHAR device_time[NAME_LEN];/*2013-12-12T00:00:00.000*/
    _GB28181_info()
    {
        swpa_memset(this,0,sizeof(_GB28181_info));
        enable = FALSE;
        swpa_strcpy(server_id,"34020000002000000001");
        swpa_strcpy(server_region,"3402000000");
        swpa_strcpy(server_ip,"172.18.10.113");
        swpa_strcpy(server_port,"5060");
        swpa_strcpy(ipc_id,"34020000001320000001");
        swpa_strcpy(ipc_region,"3402000000");
        swpa_strcpy(ipc_pwd,"12345678");
        swpa_strcpy(ipc_username,"00000000002");
        swpa_strcpy(ipc_port,"5060");
        swpa_strcpy(ipc_alarm,"34020000001340000001");
        swpa_strcpy(mediaSend_port,"5080");
        mediaSend_FPS = 25;
        mediaSend_wight = 1920;
        mediaSend_height = 1080;
        swpa_strcpy(szDeviceName,"LS130200473");
        swpa_strcpy(szManufacturer,"signalway");
        swpa_strcpy(szModel,"EC200W");
        swpa_strcpy(szFirmware,"V1.0");

        swpa_strcpy(device_encode,"ON");
        swpa_strcpy(device_record,"OFF");
        swpa_strcpy(szOwner,"null");
        swpa_strcpy(szCivilCode,"null");
        swpa_strcpy(szBlock,"null");
        swpa_strcpy(szAddress,"null");
   //     swpa_strcpy(device_Status,"ONLINE");
        swpa_strcpy(szLongitude,"0.0");
        swpa_strcpy(szLatitude,"0.0");
  //      swpa_strcpy(device_Online,"ONLINE");
        swpa_strcpy(device_guard,"OFFDUTY");
   //     swpa_strcpy(device_time,"2013-12-12T00:00:00.000");
    }
}GB28181_info,*LPGB28181PARAM_INFO;


typedef struct _GB28181_AlarmInfo{
      CHAR szAlarmTime[NAME_LEN];/*2013-12-12T00:00:00.000*/
      CHAR szAlarmMsg[NAME_LEN];
      _GB28181_AlarmInfo()
      {
           swpa_memset(this,0,sizeof(*this));
      }
}GB28181_AlarmInfo,*LP_GB28181_AlarmInfo;



typedef enum {
    GB28181_CMD_PLAY         = 0,               //����ʵʱ��Ƶ
    GB28181_CMD_PLAYBACK     = 1,               //������ʷ��Ƶ
    GB28181_CMD_DOWNLOAD     = 2,               //������ʷ��Ƶ
    GB28181_CMD_BYE          = 3,               //��������
    GB28181_CMD_BACKAWAY     = 4,               //��󲥷�
    GB28181_CMD_FORWARD      = 5,               //��ǰ����
    GB28181_CMD_DRAG         = 6,               //�϶�ָ����
    GB28181_CMD_PAUSE        = 7                //��ͣ
}CMDTYPE;


typedef struct _GB28181_Control_Param{
             CMDTYPE   Messege_ID;
                 INT   Channel_ID;
    SWPA_DATETIME_TM   sBeginTime;
    SWPA_DATETIME_TM   sEndTime;
               FLOAT   fltScale;

}GB28181_Control_Param;

typedef struct _client_info{
    INT did;    //�ỰID
   CHAR videoSendIP[32];
   DWORD videoSendPort;

}client_info;


typedef struct _H264Header{
   SWPA_DATETIME_TM timerecord;
                INT iSize;
                INT iHeight;
                INT iWidth;
                INT iType;
}H264Header;


#endif
