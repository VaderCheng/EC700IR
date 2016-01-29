// ���ļ����������WINDOWS-936��ʽ
#ifndef __DATA_CTRL_H__
#define __DATA_CTRL_H__

#include <stdio.h>
#include <string>
#include <vector>
#include <set>
#include "hvthreadbase.h"
#include "HvSockUtils.h"
#include "CameraLinkBase.h"
#include "LoadParam.h"

//����״̬
const int CONN_STATUS_UNKNOWN   = 0;	//δ֪
const int CONN_STATUS_NORMAL    = 1;	//����
const int CONN_STATUS_DISCONN   = 2;	//�Ͽ�
const int CONN_STATUS_RECONN    = 3;	//������

const int MAX_HV_BOX            = 8;
const int MAX_RECORD_BLEN       = 30;

//ʶ�����ص�����
typedef HRESULT (*RECORD_CALLBACK)(PVOID pUserData                          //�û�������
                                  ,const char* szDev                         //�豸��ʶ(��Ż�IP)
                                  ,DWORD32 dwTimeLow
                                  ,DWORD32 dwTimeHigh
                                  ,const CAMERA_INFO_HEADER* pInfoHeader
                                  ,const unsigned char* pbInfo
                                  ,const unsigned char* pbData
                                  );

struct SResultInfo
{
    std::string    strDev;          //�ĸ��豸
    std::string    strPlateNO;      //����
    DWORD          dwTime;          //ʱ��
    SResultInfo();
	SResultInfo(const SResultInfo& rs);
	SResultInfo& operator=(const SResultInfo& rs);
};

struct SRecordData
{
    bool                fUsed;
    char                szIP[32];
    DWORD32             dwTimeLow;
    DWORD32             dwTimeHigh;
    CAMERA_INFO_HEADER  cInfoHeader;
    unsigned char*      pbInfo;
    unsigned char*      pbData;

    SRecordData()
    {
        memset(this, 0, sizeof(*this));
    }
};

typedef std::vector<SResultInfo> ListResultInfo;

//����ʶ�������߳�
class CRecvRecordThread : public CHvThreadBase
{
public:
    virtual HRESULT Run(void *pvParamter);
};

//����߳�
class CMonitorThread : public CHvThreadBase
{
public:
    virtual HRESULT Run(void *pvParamter);
};

//�����߳�
class CSaveThread : public CHvThreadBase
{
public:
    virtual HRESULT Run(void *pvParamter);

private:
    SRecordData     m_sRecordData;
};

class CHvBoxHolder;

class CHvBox
{
public:
    CHvBox(const char* szIP);

    ~CHvBox();

public:
    //���豸
    HRESULT Open();

    //�ر��豸
    void Close();

    //��ȡ����״̬
	HRESULT GetConnStatus(DWORD* pdwConnStatus);

	//�������������ջص�����
    static HRESULT SetCallBack(PVOID pFunc, PVOID pUserData, const char* szConnCmd);

	friend class CRecvRecordThread;
	friend class CMonitorThread;
	friend class CHvBoxHolder;
	friend class CSaveThread;

protected:
    //������ڵļ�¼
    static void ClearTimeOutResult();

    //����һ����¼
    static bool AddResultInfo(const SResultInfo& sResultInfo);

private:
    static DWORD                            m_dwInstanceCount;              //ʵ������
    static HV_SEM_HANDLE                    m_pSemRecord;
    static PVOID                            m_pUserData;
    static RECORD_CALLBACK                  m_pRecordCallBack;              //ʶ�����ص�����
    static ListResultInfo                   m_listResultInfo;               //��¼����Ľ����Ϣ
    static DWORD                            m_dwRemainTime;                 //��¼����ʱ��

    static SRecordData                      m_szRecordData[MAX_RECORD_BLEN];
    static std::list<int>                   m_listDataPos;
    static CSaveThread                      m_cSaveThread;                  //�����߳�

private:
    char                                    m_szIP[32];                     //�豸IP
	HV_SOCKET_HANDLE                        m_sktRecord;
	char                                    m_szRecordConnCmd[128];
	DWORD                                   m_dwRecordConnStatus;

	CRecvRecordThread                       m_cRecvRecordThread;
	CMonitorThread                          m_cMonitorThread;

	DWORD                                   m_dwRecvRecordThreadLastTick;

	bool                                    m_fOpen;
	DWORD                                   m_dwTryCount;                   //���������Ĵ���
};

typedef CHvBox* PHvBox;

class CHvBoxHolder
{
public:
    CHvBoxHolder();
    ~CHvBoxHolder();

    //��ʼ������
    HRESULT Init(const ModuleParams* pModuleParams, ISafeSaver* pcSafeSaver);

    //��ӡ����״̬
    void ShowStatus();
private:
    PHvBox                                   m_szPHvBox[MAX_HV_BOX];
    std::set<std::string>                    m_setDev;                      //�豸�б�
    bool                                     m_fInit;                       //�Ƿ��ʼ����
};
#endif
