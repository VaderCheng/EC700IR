// ���ļ������ʽ������WIN936

// camerarecordlink.h
// ����ʶ������������Ϣ����ӿ�

#ifndef _CAMERARECORDLINK_H_
#define _CAMERARECORDLINK_H_

#include "config.h"
#include "CameraLinkBase.h"
#include "HvCameraLink.h"
#include "hvenum.h"
#include "tinyxml.h"

#include <list>


#define MAX_IP_LEN 64


class CCameraRecordLink;
// ��ʷ��������߳�
class CHistoryRecordProcess : public CHvThreadBase
{
public:
    // CHvThreadBase
    virtual const char* GetName()
    {
        static char szName[] = "CHistoryRecordProcess";
        return szName;
    }
    virtual HRESULT Run(void* pvParam);

    CHistoryRecordProcess(CCameraRecordLink* pRecordLink);
    ~CHistoryRecordProcess();

    HRESULT GetRefData(
        CCameraDataReference** ppRefRecord,
        CCameraData** ppCameraRecord,
        DWORD32 dwInfoSize,
        DWORD32 dwDataSize
    );

    BOOL ThreadIsOk();
    BOOL IsEndOfRecord();

private:
    CCameraRecordLink* m_pRecordLink;
    DWORD32 m_dwThreadIsOkMs;
    BOOL m_fIsEndOfRecord;
    static const int MAX_WAIT_MS = 200000;
};


//ʶ����������
class CCameraRecordLink : public CHvThreadBase
{
public:
    // CHvThreadBase
    virtual const char* GetName()
    {
        static char szName[] = "CCameraRecordLink";
        return szName;
    }
    virtual HRESULT Run(void* pvParam);
    bool ThreadIsOk();

public:
    CCameraRecordLink(HV_SOCKET_HANDLE hSocket, SEND_RECORD_PARAM& cSendRecordParam);
    ~CCameraRecordLink();

    HRESULT SendRecord(CCameraDataReference* pRefRecord);
    bool IsSendHistoryPlate();
    virtual bool IsRecordListEmpty();

    DWORD32 GetStatus()
    {
        return m_dwThreadStatus;
    }
    HRESULT SetSocket(HV_SOCKET_HANDLE& hSocket);

protected:
    HRESULT DisConnect();
    // ��ʼ������,����ʱ��ȷ���Ĳ�����
    HRESULT Init();
    HRESULT ProcessCmd(HvCore::IHvStream* pSocketStream);
    HRESULT ProcessHistroyPlate(HvCore::IHvStream* pSocketStream);

    bool IsSafeSaverEnable();
    int RecordOutputType();
    HRESULT GetCCameraDataReference(CCameraDataReference** pRefRecord);
    HRESULT ClearCCameraData();

private:

    HRESULT ProcessXMLInfo(unsigned char* pbInfo, const DWORD32& dwInfoSize);
    HRESULT DownloadRecordXML(const TiXmlElement* pCmdArgElement);

protected:
    CAMERA_INFO_HEADER m_cameraThrob;
    CAMERA_INFO_HEADER m_cameraThrobResponse;

    SEND_RECORD_PARAM& m_cSendRecordParam;
    char    m_szDevList[256];   //ָ������ĳЩ�豸�Ľ��
    DWORD32 m_dwDataInfo;       //ָ������ĳ����Ϣ�����ݣ�����Υ��
    bool m_fSendHistroyEnable;

    HV_SEM_HANDLE m_hSemRecord;
    HV_SEM_HANDLE m_hSemListRecord;  //Add by Shaorg: Ϊ��m_lstRecord�������̰߳�ȫ
    std::list<CCameraDataReference*> m_lstRecord;
    // ���ӵ�ǰ״̬: 0 : ����1�� ��������
    DWORD32 m_dwThreadStatus;

    CAMERA_INFO_HEADER		m_cHistoryEnd;    //��ʷ���������־���·��ͻ���

    HvCore::IHvStream* m_pSocketStream;
    bool m_fSendHistoryEnable;
    DWORD32 m_dwConnectedLogId;

    CHistoryRecordProcess* m_pHistoryPro;
    friend class CHistoryRecordProcess;

    //�ϵ��������
    DWORD32 m_dwInfoOffset;
    DWORD32 m_dwDataOffset;

#ifdef SINGLE_BOARD_PLATFORM
    static const int MAX_CAMERA_RECORD_COUNT = 8;
#else
    static const int MAX_CAMERA_RECORD_COUNT = 2;
#endif

private:
    HV_SOCKET_HANDLE m_hSocket;

    static const int MAX_WAIT_MS = 200000;
    DWORD32 m_dwThreadIsOkMs;

    //�ɿ������ӷ�ʽ���
    DWORD32 m_dwSendTimeLow;
    DWORD32 m_dwSendTimeHigh;
    DWORD32 m_dwEndSendTimeLow;
    DWORD32 m_dwEndSendTimeHigh;
    int m_iRecordIndex;
};

//ʶ��������������
class CCameraRecordAutoLink : public CCameraRecordLink
{
public:
    virtual const char* GetName()
    {
        static char szName[] = "CCameraRecordAutoLink";
        return szName;
    }
    virtual HRESULT Run(void* pvParam);
public:
    CCameraRecordAutoLink(HV_SOCKET_HANDLE hSocket,
                        SEND_RECORD_PARAM& cSendRecordParam ,
                        char* pServerIp ,
                        WORD16 wPort);
    ~CCameraRecordAutoLink();
     bool IsConnect(void){return m_fConnected;}
private:
    HRESULT SetConnectionRequst();
    virtual bool IsRecordListEmpty();
private:
    HV_SOCKET_HANDLE m_hAutoLinkSocket;
    HvCore::IHvStream* m_pAutoLinkSocketStream;

    DWORD32 m_dwAutoConnectedLogId;

    WORD16 m_wAutoLinkPort;
    char m_szAutoLinkIP[MAX_IP_LEN];
    bool   m_fConnected;
};

// ʶ��������ö����
class CCameraRecordLinkEnum : public CHvEnumBase<CCameraRecordLink*>
{
protected:
    HRESULT OnDelNode(NODE* pNode)
    {
        SAFE_DELETE(pNode->Dat);
        return S_OK;
    }
};

// ʶ�������ӿ�����
class CCameraRecordLinkCtrl : public HiVideo::ISendRecord
            , public CCameraLinkCtrlBase
{
public:
    // ISendRecord
    virtual HRESULT SendRecord(const SEND_RECORD* pRecord, DWORD32 dwTimeMsLow, DWORD32 dwTimeMsHigh);
    virtual HRESULT SetParam(SEND_RECORD_PARAM* pParam);
    virtual HRESULT GetCurStatus(char* pszStatus, int nStatusSizes)
    {
        return (true == ThreadIsOk()) ? S_OK : E_FAIL;
    };

    // CCameraLinkCtrlBase
    virtual int GetLinkCount();
    // ɾ�����е����ӣ�ͬʱ���ص�ǰ����������
    virtual int DeleteFreeLink();
    // ������е����ӡ�
    virtual HRESULT ClearLink();
    // �����ܵ������߳�
    virtual HRESULT CreateLinkThread();
    // �µ�����
    virtual HRESULT SetLink(HV_SOCKET_HANDLE& hSocket);

    // ������������
    virtual HRESULT StartAutoLink();
    // ֹͣ��������
    virtual HRESULT StopAutoLink();

    bool ThreadIsOk();

	int GetRealTimeLinkCount();

public:
    CCameraRecordLinkCtrl();
    ~CCameraRecordLinkCtrl();

    HRESULT MakeNode(
        const DWORD32& dwID,
        const DWORD32& dwNodeLen,
        const unsigned char* pbNode,
        unsigned char* pbInfo
    );

    HRESULT SetAutoLink(bool fAutoLinkEnable ,char* pAutoLinkIP , WORD16 wAutoLinkPort);
    HRESULT EnumLock();
    HRESULT EnumUnLock();

private:
    static const int MAX_LINK_COUNT = 2;
    CCameraRecordLinkEnum m_cCameraRecordLinkEnum;
    HV_SEM_HANDLE m_hSemEnum;

    CCameraRecordAutoLink* m_cCameraRecordAutoLink;

    static const int MAX_WAIT_MS = 200000;
    SEND_RECORD_PARAM m_cSendRecordParam;

    bool m_fResumeBrokenTransfer;

    WORD16 m_wAutoLinkPort;
    char m_szAutoLinkIP[MAX_IP_LEN];
};

#endif
