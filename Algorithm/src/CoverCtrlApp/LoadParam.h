/**
* @file		LoadParam.h
* @version	1.0
* @brief	�����ģ�������API
*/

#ifndef _LOADPARAM_H_
#define _LOADPARAM_H_

#include "ResultSenderImpl.h"
#include "HvParamStore.h"
#include "eprfilestorage.h"
#include "TcpipCfg.h"

/**< ����Ȩ�޵ȼ� */
enum ParamRank
{
    INNER_LEVEL = 1,    /**< �ڲ��з��� */
    PROJECT_LEVEL = 3,  /**< ���̼� */
    CUSTOM_LEVEL = 5    /**< �û��Զ��弶 */
};

typedef struct  _DevParam
{
    int        iFilterTime;     //����һ��ʱ���ڵ���ͬ���(��λ:��)
    int        iDevCount;       //�豸����
    char       szDevIP1[32];    //��һ̨�豸IP
    char       szDevIP2[32];    //...
    char       szDevIP3[32];
    char       szDevIP4[32];
    char       szDevIP5[32];
    char       szDevIP6[32];
    char       szDevIP7[32];
    char       szDevIP8[32];

    _DevParam()
    {
        memset(this, 0, sizeof(*this));
    }
}DevParam;

/**< ����ģ���ܲ��� */
typedef struct _ModuleParams
{
    int nWorkModeIndex;
    ResultSenderParam cResultSenderParam;
    TcpipParam cTcpipCfgParam_1;
    TcpipParam cTcpipCfgParam_2;
    DevParam    cDevParam;

    _ModuleParams()
    {
        nWorkModeIndex = 0;
    }
} ModuleParams;

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
* @brief ��������豸ģ��Ĳ���
* @return 0
*/
int LoadDevParam(
    CParamStore* pcParamStore,
    DevParam& cDevParam
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
* @brief �ָ�Ĭ�ϲ�����������IP����
* @return 0
*/
int LoadDefaultParam(
    CParamStore* pcParamStore,
    TcpipParam& cTcpipCfgParam1,
    TcpipParam& cTcpipCfgParam2
);
#endif
