#ifndef _HVPARAMINC_INCLUDED_
#define _HVPARAMINC_INCLUDED_

#include "ResultSenderImpl.h"
#include "HvParamStore.h"
#include "eprfilestorage.h"
#include "OuterControlImpl_Linux.h"
#include "trackerdef.h"
#include "TcpipCfg.h"
#include "ImgGatherer.h"

/**< ����Ȩ�޵ȼ� */
#define INNER_LEVEL     1 /**< �ڲ��з��� */
#define PROJECT_LEVEL   3 /**< ���̼� */
#define CUSTOM_LEVEL    5 /**< �û��Զ��弶 */

/**< ����ģ���ܲ��� */
typedef struct _ModuleParams
{
    int nWorkModeIndex;
    ResultSenderParam cResultSenderParam;
    SignalMatchParam cSignalMatchParam;
    TcpipParam cTcpipCfgParam_1;
    TcpipParam cTcpipCfgParam_2;
    IMG_FRAME_PARAM cImgFrameParam;
    TRACKER_CFG_PARAM cTrackerCfgParam;
    CAM_CFG_PARAM cCamCfgParam;

    _ModuleParams()
    {
        nWorkModeIndex = 0;
        strcpy(cTcpipCfgParam_1.szIp, "192.168.1.11");
		strcpy(cTcpipCfgParam_1.szNetmask, "255.255.255.0");
		strcpy(cTcpipCfgParam_1.szGateway, "0.0.0.0");
    }
} ModuleParams;

#endif
