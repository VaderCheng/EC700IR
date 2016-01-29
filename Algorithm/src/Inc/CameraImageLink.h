// ���ļ������ʽ������WIN936

// cameraimagelink.h
// �������ͼƬ����ӿ�

#ifndef _CAMERAIMAGELINK_H_
#define _CAMERAIMAGELINK_H_

#include "CameraLinkBase.h"
#include "HvCameraLink.h"
#include "hvenum.h"
#include "tinyxml.h"

#include <list>

//���ͼƬ������
class CCameraImageLink : public CHvThreadBase
{
public:
    // CHvThreadBase
    virtual const char* GetName()
    {
        static char szName[] = "CCameraImageLink";
        return szName;
    }
    virtual HRESULT Run(void* pvParam);
    bool ThreadIsOk();

public:
    CCameraImageLink(HV_SOCKET_HANDLE hSocket);
    ~CCameraImageLink();

    HRESULT SendImage(CCameraDataReference* pRefImage);
    HRESULT IsEnableRecogVideo()
    {
        return m_fEnableRecogVideo;
    }

    DWORD32 GetStatus()
    {
        return m_dwThreadStatus;
    }
    HRESULT SetSocket(HV_SOCKET_HANDLE& hSocket);

    BOOL CheckIsPanoramicMode()
    {
        return m_fPanoramicMode;
    }

private:
    HRESULT ProcessCmd(HvCore::IHvStream* pSocketStream);
    HRESULT ProcessXMLInfo(unsigned char* pbInfo, const DWORD32& dwInfoSize);
    HRESULT SetImgType(const TiXmlElement* pCmdArgElement);
    HRESULT DisConnect();
    // ��ʼ������,����ʱ��ȷ���Ĳ�����
    HRESULT Init();

private:
    static const int MAX_CAMERA_IMAGE_COUNT = 2;
    std::list<CCameraDataReference*> m_lstImage;

    CAMERA_INFO_HEADER m_cameraThrob;
    CAMERA_INFO_HEADER m_cameraThrobResponse;

    HV_SEM_HANDLE m_hSemImage;
    HV_SEM_HANDLE m_hSemListImage;  //Add by Shaorg: Ϊ��m_lstImage�������̰߳�ȫ
    HV_SOCKET_HANDLE m_hSocket;
    HvCore::IHvStream* m_pSocketStream;

    static const int MAX_WAIT_MS = 200000;
    DWORD32 m_dwThreadIsOkMs;

    DWORD32 m_dwConnectedLogId;

    BOOL m_fEnableRecogVideo;
    BOOL m_fPanoramicMode;
    // ���ӵ�ǰ״̬: 0 : ����1�� ��������
    DWORD32 m_dwThreadStatus;
};


// ���ͼƬ����ö����
class CCameraImageLinkEnum : public CHvEnumBase<CCameraImageLink*>
{
protected:
    HRESULT OnDelNode(NODE* pNode)
    {
        SAFE_DELETE(pNode->Dat);
        return S_OK;
    }
};

// ���ͼƬ���ӿ�����
class CCameraImageLinkCtrl : public HiVideo::ISendCameraImage
            , public CCameraLinkCtrlBase
{
public:
    // ISendCamImage
    virtual HRESULT SendCameraImage(const SEND_CAMERA_IMAGE* pImage);
    virtual HRESULT SetParam(const SEND_IMAGE_PARAM* pParam);
    virtual HRESULT GetCurStatus(char* pszStatus, int nStatusSizes)
    {
        return (true == ThreadIsOk()) ? S_OK : E_FAIL;
    };
    virtual HRESULT GetIsCaptureMode(BOOL* fIsCaptureLink)
    {
        if(fIsCaptureLink == NULL)
        {
            return E_FAIL;
        }
        *fIsCaptureLink = FALSE;
        if(GetRunCaptureModeLinkCount() > 0)
        {
            *fIsCaptureLink = TRUE;
        }
        return S_OK;
    }

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
    //ֻ��ȡץ��ͼ�����Ӹ���
    virtual int GetRunCaptureModeLinkCount();

    bool ThreadIsOk();

public:
    CCameraImageLinkCtrl();
    ~CCameraImageLinkCtrl();

private:
    HRESULT MakeNode(
        const DWORD32& dwID,
        const DWORD32& dwNodeLen,
        const unsigned char* pbNode,
        unsigned char* pbInfo
    );

    HRESULT EnumLock();
    HRESULT EnumUnLock();
    HRESULT SendLock();
    HRESULT SendUnLock();

private:
    static const int MAX_LINK_COUNT = 2;
    CCameraImageLinkEnum m_cCameraImageLinkEnum;
    HV_SEM_HANDLE m_hSemEnum;
    HV_SEM_HANDLE m_hSemSendCameraImage;

    static const int MAX_WAIT_MS = 200000;
};

#endif
