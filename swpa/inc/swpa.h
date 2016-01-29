/**
* @file swpa.h 
* @brief PlatformAPI����⿪�ŵ����ݶ���
* 
* - ������PlatformAPI��ͨ�õķ���ֵ
* 
* @copyright Signalway All Rights Reserved
* @author zhouy
* @date 2013-02-27
* @version 1.0
*/


#ifndef _SWPA_H_
#define _SWPA_H_ 



//-------------------- _XOPEN_SOURCE ------------------------//
///���뱣֤_XOPEN_SOURCE�б�define�ұ�define��500�������漰POSIX������ļ���arm_v5t_le-gcc�б��������
#ifndef _XOPEN_SOURCE

  #define _XOPEN_SOURCE 500

#elif 500 != _XOPEN_SOURCE

  #undef _XOPEN_SOURCE
  #define _XOPEN_SOURCE 500
  
#endif  


//�汾��
#include "swpa_version.h"
//OS Lib���
#include "swpa_datetime.h"
#include "swpa_mem.h"
#include "swpa_sem.h"
#include "swpa_socket.h"
#include "swpa_tcpip.h"
#include "swpa_thread.h"
#include "swpa_timer.h"
#include "swpa_hdd.h"
#include "swpa_string.h"
#include "swpa_utils.h"
#include "swpa_list.h"
#include "swpa_map.h"
#include "swpa_math.h"

//Driver Lib���
#include "swpa_device.h"
#include "swpa_file.h"
#include "swpa_dma.h"
#include "swpa_device_setting.h"

//������
#include "swpa_camera.h"
#include "swpa_vpif.h"
#include "swpa_ipnc.h"

#ifdef __cplusplus
extern "C"
{
#endif

//����ֵ����
///�ɹ�
#define SWPAR_OK 				( 0)

///ʧ��
#define SWPAR_FAIL 				(-1)

///�Ƿ�����
#define SWPAR_INVALIDARG		(-2)

///û��ʵ��
#define SWPAR_NOTIMPL			(-3)

///�ڴ治��
#define SWPAR_OUTOFMEMORY 		(-4)

///û�г�ʼ��
#define SWPAR_NOTINITED			(-5)

///������ʱ
#define SWPAR_OUTOFTIME			(-6)



// ��С����ֵ���塣
#define SWPAR_MIN				(-10)



#ifdef __cplusplus
}
#endif

#endif //_SWPA_H_



