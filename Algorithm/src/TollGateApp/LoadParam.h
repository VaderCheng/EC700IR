/**
* @file		LoadParam.h
* @version	1.0
* @brief	�����ģ�������API
*/

#ifndef _LOADPARAM_H_
#define _LOADPARAM_H_

#include "HvParamInc.h"

/**
* @brief ���빦��ģ���ܲ���
* @return �ɹ���S_OK��ʧ�ܣ�E_FAIL
*/
HRESULT LoadModuleParam(ModuleParams& cModuleParams);

/**
* @brief ���빤��ģʽ����
* @return 0
*/
int LoadWorkModeParam(
    CParamStore* pcParamStore,
    ModuleParams& cModuleParams
);

/**
* @brief ���롰������͡�ģ��Ĳ���
* @return 0
*/
int LoadResultSenderParam(
    CParamStore* pcParamStore,
    ResultSenderParam& cResultSenderParam
);

/**
* @brief ���롰�ź�ƥ������ģ��Ĳ���
* @return 0
*/
int LoadSignalMatchParam(
    CParamStore* pcParamStore,
    SignalMatchParam& cSignalMatchParam
);

/**
* @brief ���롰Tcpip��ģ��Ĳ���,��������1��ַ
* @return 0
*/
int LoadTcpipParam_1(
    CParamStore* pcParamStore,
    TcpipParam& cTcpipCfgParam
);

/**
* @brief ���롰Tcpip��ģ��Ĳ���,��������2��ַ
* @return 0
*/
int LoadTcpipParam_2(
    CParamStore* pcParamStore,
    TcpipParam& cTcpipCfgParam
);

/**
* @brief ����Trackerģ��Ĳ���
* @return 0
*/
int LoadTrackerCfgParam(
    CParamStore* pcParamStore,
    TRACKER_CFG_PARAM& cTrackerCfgParam,
    ModuleParams& cModuleParams
);

/**
* @brief ����������ò���
* @return 0
*/
int LoadCamCfgParam(
    CParamStore* pcParamStore,
    CAM_CFG_PARAM& cCamCfgParam
);

/**
* @brief ��ȡ�����п���ģʽ����
* @return ����ģʽ����
*/
int GetRecogParamCountPart();

/**
* @brief ͨ��ģʽ����ȡ������ģʽ����
* @return ������ģʽ����
*/
int GetRecogParamIndexOnNamePart(char *pszModeName);

/**
* @brief ͨ��������ģʽ������ȡģʽ��
* @return S_OK �ɹ�
*         E_FAIL ����������Χ
*         E_POINTER ָ�����
*/
HRESULT GetRecogParamNameOnIndexPart(DWORD32 dwIndex, char *pszModeName);

/**
* @brief ����ģʽ
* @return S_OK �ɹ�
*         E_FAIL ʧ��
*/
HRESULT SetWorkModePart(DWORD32 dwWorkMode);

/**
* @brief �ָ�Ĭ�ϲ�����������IP��ģʽ����
* @return 0
*/
int LoadDefaultParam(
    CParamStore* pcParamStore,
    TcpipParam& cTcpipCfgParam1,
    TcpipParam& cTcpipCfgParam2
    );

/**
* @brief ���ذ汾��Ϣ
* @return 0
*/
int LoadBuildNoParam();
#endif
