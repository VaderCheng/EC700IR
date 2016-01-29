/**
 * @file   pelco_protocol.h
 * @author 
 * @date   Tue Jun 17 09:20:40 2014
 * 
 * @brief  pelco protocol D implement
 * 
 * 
 */

#ifndef __PELCO_PROTOCOL_H
#define __PELCO_PROTOCOL_H

#include <stdint.h>
#include "common_protocol.h"

#ifdef __cplusplus
extern "C" {
#endif

/// �ɹ�
#define PELCO_SUCCESS       0
/// ʧ��
#define PELCO_FAILURE       -1
/// ��ʱ
#define PELCO_TIMEOUT       -6
/// ����ִ��ʧ��
#define PELCO_CMDERROR      -3

typedef struct _COM_interface PELCOInterface_t;

typedef struct _COM_packet PELCOPacket_t;

///////////////////////////////////////////////////////////////////////////////
/** 
 * ��ʼ���������
 * 
 * @param iface [in] ����Э��ӿڲ���
 * 
 * @return �ɹ���0  ʧ�ܣ�-1
 *
 * @note ��Ҫ��PELCOInterface_t���и�ֵ
 */
int pelco_open_serial(PELCOInterface_t *iface);
/** 
 * �ر��������
 * 
 * @param iface [in] ����Э��ӿڲ���
 * 
 * @return �ɹ���0  ʧ�ܣ�-1
 */
int pelco_close_serial(PELCOInterface_t *iface);
/** 
 * ��ȡ���ڻ�����ʣ�������
 * 
 * @param iface [in] ����Э��ӿڲ���
 * @param buffer [out] ������
 * @param buffer_size [out] ��������С
 * 
 * @return �ɹ���0  ʧ�ܣ�-1
 * @note �������ɷ�ֹ�´δ���ͨ��ʱ�򻺳������������ݶ����´������������
 */
int pelco_unread_bytes(PELCOInterface_t *iface, unsigned char *buffer, int *buffer_size);

///////////////////////////////////////////////////////////////////////////////
/** 
 * ֹͣ�������
 * 
 * @param iface [in] ����Э��ӿڲ���
 * 
 * @return �ɹ���0  ʧ�ܣ�-1 ��ʱ��-6
 * @note ÿ�ֶ�������Ҫ���øú�����ֹͣ�����������һֱִ��(���������ˮƽ��ת�����һֱת)��
 */
int pelco_camera_stop(PELCOInterface_t *iface);

/** 
 * �����ֱ�ƶ�
 * 
 * @param iface [in] ����Э��ӿڲ���
 * @param action [in] ��ֱ�ƶ����� 0 = down  1 = up
 * @param speed [in]  �ƶ��ٶȣ�1~0x3f
 * 
 * @return �ɹ���0  ʧ�ܣ�-1 ��ʱ��-6
 */
int pelco_camera_tilt(PELCOInterface_t *iface, int action, int speed);

/** 
 * ���ˮƽ�ƶ�
 * 
 * @param iface [in] ����Э��ӿڲ���
 * @param action [in] ˮƽ��ת���� 0 = left  1 = right
 * @param speed [in]  �ƶ��ٶȣ�1~0x3f
 * 
 * @return �ɹ���0  ʧ�ܣ�-1 ��ʱ��-6
 */
int pelco_camera_pan(PELCOInterface_t *iface, int action, int speed);

// a little advanced function
/** 
 * ���ͬʱˮƽ����ֱ�ƶ�
 * 
 * @param iface [in] ����Э��ӿڲ���
 * @param pan_act [in] ˮƽ��ת���� 0 = left  1 = right
 * @param pan_speed [in] �ƶ��ٶȣ�1~0x3f
 * @param tilt_act [in] ��ֱ�ƶ����� 0 = down  1 = up
 * @param tilt_speed [in] �ƶ��ٶȣ�1~0x3f
 * 
 * @return �ɹ���0  ʧ�ܣ�-1 ��ʱ��-6
 * @note �ú�����pelco_camera_pan��pelco_camera_tilt�Ļ����ʽ
 */
int pelco_camera_pan_tilt(PELCOInterface_t *iface, int pan_act, int pan_speed, int tilt_act, int tilt_speed);
///////////////////////////////////////////////////////////////////////////////

/** 
 * Ԥ��λ��غ���
 * 
 * @param iface [in] ����Э��ӿڲ���
 * @param preset_act [in] Ԥ��λ���� 0������ 1����� 2������
 * @param preset_id [in] 1~255 ��0����Ч
 * 
 * @return �ɹ���0  ʧ�ܣ�-1 ��ʱ��-6
 * @todo ����в���Ԥ��λ���Ѿ���ռ���ˣ���Ŀǰֻ�в���Ԥ��λ���ĵ����޷�֪���Ƿ�������������Ԥ��λ��
 */
int pelco_camera_preset(PELCOInterface_t *iface, int preset_act, int preset_id);

/** 
 * �������PT����
 * 
 * @param iface [in] ����Э��ӿڲ���
 * @param pan [in] 0~3599(0~359.9��)
 * @param tilt [in] 0~900(0��~90��)
 * 
 * @return �ɹ���0  ʧ�ܣ�-1 ��ʱ��-6
 * @note PT��ˮƽ(pan)����ֱ(tilt)�����ꡣˮƽ��Χֻ��3599������3600������0��Ч
 */
int pelco_camera_set_ptcoordinates(PELCOInterface_t *iface, int pan, int tilt);

/** 
 * ��ȡ���PT����
 * 
 * @param iface [in] ����Э��ӿڲ���
 * @param pan [out] 0~3599(0~359.9��)
 * @param tilt [out] 0~900(0��~90��)
 * 
 * @return �ɹ���0  ʧ�ܣ�-1 ��ʱ��-6
 */
int pelco_camera_get_ptcoordinates(PELCOInterface_t *iface, int* pan, int* tilt);

int pelco_camera_set_infraredpower(PELCOInterface_t *iface, int total_zoom, int zoom_1, int zoom_2);

int pelco_camera_factorytest_start(PELCOInterface_t *iface);

int pelco_camera_factorytest_stop(PELCOInterface_t *iface);

///////////////////////////////////////////////////////////////////////////////
//�����������޷�ȷ���Ļ�֧�ֵ�
///////////////////////////////////////////////////////////////////////////////
/*
no support!!!
3D��λ
zoom_act 0 = tele(�Ŵ�) 1 = wide(��С)
o_x,o_y: ��λ��Χ�����ĵ�����
width, height: ��Χ���
˵������width��height����һ������Ϊ0ʱ��������������б䱶����
ʾ��ͼ��
(0, 0)                                (100, 0)
+-----------------------------------------
|       |<----width --->|
|    -- +-------------------+
|    |  |                   |
| height|        .(o_x, o_y)|
|    |  |                   |
|   -- -+-------------------+
|
+---------------------------------------
(100, 0)
*/
//int pelco_camera_set_3dpos(PELCOInterface_t *iface, int zoom_act, int o_x, int o_y, int width, int height);

// action: 1 = auto scan 0 = manualscan
int pelco_camera_scan(PELCOInterface_t *iface, int action);
// action: 1 = on  0 = off
int pelco_camera_switch(PELCOInterface_t *iface, int action);
// action: 1 = open  0 = close
int pelco_camera_iris_switch(PELCOInterface_t *iface, int action);
// action: 0 = near 1 = far
int pelco_camera_focus(PELCOInterface_t *iface, int action);
// action: 0 = zoom tele  1 = zoom wide
int pelco_camera_zoom(PELCOInterface_t *iface, int action);

/* pattern
pattern_act: 0 = start  1 = stop  2 = run
pattern_id: ? todo
*/
int pelco_camera_pattern(PELCOInterface_t *iface, int pattern_act, int pattern_id);

///////////////////////////////////////////////////////

int pelco_camera_loop_test(PELCOInterface_t *iface, char *buffer, int len);

int pelco_camera_calibration(PELCOInterface_t *iface);

int pelco_camera_3d(PELCOInterface_t *iface, int centerx, int centery, int width, int height);

int pelco_camera_set_led_mode(PELCOInterface_t *iface, int mode);

int pelco_camera_set_led_power_mode(PELCOInterface_t *iface, int mode);

int pelco_camera_set_led_power(PELCOInterface_t *iface, int nearvalue, int mediumvalue, int farvalue);

int pelco_camera_sync_zoom(PELCOInterface_t *iface);

int pelco_camera_set_privacyzone(PELCOInterface_t *iface, int index, int enable, int width, int height);

int pelco_camera_set_privacycoord(PELCOInterface_t *iface, int index, int x, int y);
int pelco_camera_get_dome_version(PELCOInterface_t *iface, int *dome_version);

#ifdef __cplusplus
};
#endif

#endif /* __PELCO_PROTOCOL_H */
