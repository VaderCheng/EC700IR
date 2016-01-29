#ifndef _INCLUDE_PCPARAM_H_
#define _INCLUDE_PCPARAM_H_

#include "HvUtils.h"
#include "swBaseType.h"
#include "HvSysInterface.h"
#include "HvParamStore.h"
#include "HvCmdLink.h"
#include "..\HighVPRApp\PcCmdProcImpl.h"
#include "CapCamThread.h"
#include "CfgParam.h"
#include "..\Protocol\DataLinkImpl.h"
#include "..\Protocol\ResultSendImpl.h"
#include "..\Protocol\PcCmdFun.h"
#include "HvJpegProcesser.h"
#include "CamTrigger.h"
#include "..\highdsp\eventchecker.h"
//��Ƶ����
#if (RUN_PLATFORM == PLATFORM_WINDOWS)
#include "..\HighVPRApp\SaveVideo.h"
extern CSaveVideo g_cSaveVideo;
// AVI ����ӿ�
extern IAviSaver* g_pIAviSaver;

#endif
//ȫ�ֲ���
extern CParamStore g_cHvParamStore;
extern HvCore::IHvParam2* g_pHvParam;

//���У�����
extern IDENTIFYPASSWORD_PARAM g_IdentifyParam;

//�����ṹ��
extern SYS_CONFIG_STRUCT g_cSysConfig;

extern HVDSP_PARAM g_HvDspParam;
extern VIDEO_PROCESS_PARAM g_VideoProcParam;
extern AREA_CTRL_PARAM g_AreaParam;
extern CAM_CFG_PARAM g_CamCfgParam;
extern HVC_PARAM g_HvcCamParam;
extern TRIGGER_PARAM g_TriggerParam;
extern RADAR_PARAM g_RadarParam;
extern OTHER_SIGNAL_SOURCE_PARAM g_OtherSignalSourceParam;
extern float g_fltProcessFps;

//ͨѶ
extern CCmdListenThread<CNetCmd> g_cCmdListenThread;
extern CCmdListenThread<CNetCmd> g_cCmdListenAutoThread;
extern CDataLinkCtrlThread g_cDataLinkCtrlThread;
extern CHvResultSend g_cResultSend;
extern CPcCmdProc g_cCmdProc;
extern CIPSearchThread g_cIPSearchThread;
extern CCapCamThread g_cCapCamThread;
extern CCamTrigger g_cCamTrigger;
extern CEventChecker* g_pEventChecker;

extern DWORD32 g_dwSignalTime;				//Ӳ����ʱ��
extern bool g_fSignalIsMain;	// Ӳ�����Ƿ����ź�

//��ȫ����ģʽ
extern ISafeSaver* g_pISafeSaver;
extern char g_szSafeIniFile[256];
HRESULT InitSafeSaver( LPCSTR lpszIniFile );
extern HiVideo::IJpegProcesser* g_pJpegProcesser;

//
int GetWorkMode(void);
int GetFontMode(void);
HRESULT SetFontMode( DWORD32 dwMode );
HRESULT GetSysID( DWORD32* pdwIDLow, DWORD32* pdwIDHigh );
int GetSystemName( char* szName, int iSize );
int SetSystemName( char* szName );

HRESULT InitHvOBJ( LPCSTR lpszParamFile );

#endif
