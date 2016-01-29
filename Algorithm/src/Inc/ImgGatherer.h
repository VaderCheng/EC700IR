// ���ļ������ʽ����ΪWINDOWS-936��ʽ

/**
* @file	ImgGatherer.h
* @version	1.0
* @brief ͼ��ɼ�ģ��ӿڶ���
*/

#ifndef _IMG_GATHERER_H_
#define _IMG_GATHERER_H_

#include "hvutils.h"
#include "swimageobj.h"
#include "HvList.h"
#include "hvthreadbase.h"
#include "hvsocket.h"
#include "trackerdef.h"
#include "SocketStream.h"
#include "CamyuLinkOpt.h"
#include "HvSockUtils.h"
#include "OuterControlImpl_Linux.h"
#include "CameraController.h"
#include "CamTransmitBase.h"

const int IMG_LIST_COUNT = 2;
const int MAX_IMG_RECV_COUNT = 20;
const int MAX_CAM_COUNT = 8;

// �������˿ں�
#define SVR_PORT 8888
// ͼƬ���ֳ���
#define IMG_NAME_LEN 128
// ��ȡͼƬ����
#define GET_IMG_CMD 0x0101

/**
*  ��Ƶ֡���ݽṹ
*/
typedef struct _tagImgFrame
{
    IReferenceComponentImage* pRefImage;
    int iVideoID;
    char szCamIp[32];

    DWORD32 dwFrameType;  //֡���ͣ���ʱ��H.264����ʹ�ã�
    FpgaRegInfo cFpgaRegInfo;
    AgcAwbInfo cAgcAwbInfo;
    HV_COMPONENT_IMAGE imgVPIF;
    DWORD32 dwTimeTick;

    BOOL fSplitUV;
    BOOL fRotateY;
    BOOL fUseImgVPIF;

    _tagImgFrame()
    {
        memset(this, 0, sizeof(*this));
    }

} IMG_FRAME;

/**
* @brief ǰ��ͼ��ɼ�ģ������ṹ��
*/
typedef struct _tagImgFrameParam
{
    int iCamNum;                            //�������
    char rgstrCamIP[MAX_CAM_COUNT][32];     //ץ�����IP
    char rgstrHVCParm[MAX_CAM_COUNT][128];  //ץ��ʶ�����

    float fltCaptureFrameRate; //�ɼ�֡��
    int iOutputFrameRate;   // ���֡�ʣ�1��15��

    _tagImgFrameParam()
    {
        iCamNum = 1;
        for ( int i=0; i<MAX_CAM_COUNT; ++i )
        {
            memset(rgstrCamIP[i], 0, 32);
            memset(rgstrHVCParm[i], 0, 128);
        }
        fltCaptureFrameRate = 12.5;
        iOutputFrameRate = 15;
    };

} IMG_FRAME_PARAM;

const int MAX_CAP_CAM_COUNT = 4;

/**
* @brief ץ�����ģ������ṹ��
*/
typedef struct _tagCapCamParam
{
    int nCamType;   //ץ���������(0-����Jϵ�����)
    int nCamNum;    //ץ���������
    int rgnSignalType[MAX_CAP_CAM_COUNT];   //ץ������ź�Դ����
    char rgszRoadIP[MAX_CAP_CAM_COUNT][20]; //ץ�����IP

    _tagCapCamParam()
    {
        nCamType = 0;
        nCamNum = 0;
        for (int i = 0; i < MAX_CAP_CAM_COUNT; i++)
        {
            rgnSignalType[i] = -1;
            sprintf(rgszRoadIP[i], "172.18.10.10%d", i);
        }
    }
}CAP_CAM_PARAM;

#define MAX_LEVEL_COUNT 14
/**
* @brief ǰ����������ṹ��
*/
typedef struct _tagCamCfgParam
{
    char szIP[100];
    int iCamType; // �����Э�����ͣ�0-һ�廯���Э�飬1-ר�ò���Э�飬2-Camyu�����Э��
    int iDynamicCfgEnable;
    int iDynamicTriggerEnable;
    int iTriggerDelay;
    int iTriggerPlateType; //���ڳ�βģʽ�£���Щ����������Ҫ����ץ�ģ���ͨ�����󣬻ƹ������ӣ�2012-01-04
    int iDynamicCfgEnableOnTrigger;

    // ��������ջ���֡��
    int iJpegBufferCount;
    int nTestProtocolPort;

    //�Զ�������
    int iAutoParamEnable;
    float fltDB;				//����
    int iMinExposureTime;
    int iMinGain;
    int iMaxExposureTime;
    int iMaxGain;
    int iEnableAGC;
    int iMaxAGCLimit;
    int iMinAGCLimit;

    int irgExposureTime[MAX_LEVEL_COUNT];
    int irgAGCLimit[MAX_LEVEL_COUNT];
    int irgGain[MAX_LEVEL_COUNT];
    int irgBrightness[MAX_LEVEL_COUNT];

    char rgszMode[MAX_LEVEL_COUNT][10];

    _tagCamCfgParam()
    {
        sprintf(szIP, "192.168.1.10");
        iCamType = 0;
        iDynamicCfgEnable = 0;
        iDynamicTriggerEnable =0;
        iTriggerDelay = 0;
        iTriggerPlateType = 0;
        iDynamicCfgEnableOnTrigger = 0;
        iJpegBufferCount = 1;
        iEnableAGC = 0;
        iAutoParamEnable = 0;
        fltDB = 3.0;
        iMinExposureTime  = 100;
        iMinGain  = 60;
        iMaxExposureTime  = 6000;
        iMaxGain  = 220;
        iMaxAGCLimit = 140;
        iMinAGCLimit = 10;
        nTestProtocolPort = SVR_PORT;

        int irgExposureTimeTemp[MAX_LEVEL_COUNT] =
        {
            8, 8, 8, 8, 12, 17, 24, 34, 48, 68, 96, 96, 96, 96
        };

        int irgBrightnessTemp[MAX_LEVEL_COUNT] =
        {
            -75, -50, -25, 0, 0, 0, 0, 0, 0, 0, 0, 25, 50, 75
        };

        for (int i = 0; i < MAX_LEVEL_COUNT; i++)
        {
            irgExposureTime[i] = irgExposureTimeTemp[i];
            irgAGCLimit[i] = -1;
            irgGain[i] = -1;
            irgBrightness[i] = irgBrightnessTemp[i];
            strcpy(rgszMode[i], "����");
        }
    }
}CAM_CFG_PARAM;

// ͼƬ�㲥�˿ں�
#define SVR_BROADCAST_PORT 8989
#define RCV_TIMEOUT 2000			// ���ճ�ʱ

/**
*  CBroadcastLink�ӿ���
*/
class IBroadcastLink
{
public:
    virtual ~IBroadcastLink(){};
    // ��ȡһ֡ͼƬ
    virtual HRESULT GetImage(char* pcBuf, DWORD32* pdwSize, DWORD32* pdwTime, char* pszFileName) = 0;
    // ��ʼ��
    virtual HRESULT Initialize() = 0;
};

/**
*  CBroadcastLinkʵ����
*/
class CBroadcastLink : public IBroadcastLink
{
public:
    CBroadcastLink();
    virtual ~CBroadcastLink();
    virtual HRESULT GetImage(char* pcBuf, DWORD32* pdwSize, DWORD32* pdwTime, char* pszFileName);
    virtual HRESULT Initialize();
private:
    HV_SOCKET_HANDLE m_sktData;
    CSocketStream* m_pstmData;
    DWORD32 m_dwRefTime;
};

#define TFTP_RRQ 0x0001		// ��������
#define TFTP_WRQ 0x0002		// д������
#define TFTP_DATA 0x0003		// ���ݴ��ͱ���
#define TFTP_ACK 0x0004		// ȷ�ϱ���
#define TFTP_ERROR 0x0005	// ������
#define TFTP_OACK 0x0006		// ѡ���
#define TFTP_UNKOWN 0x00FF	// δ֪����
#define SRV_PORT 69		// TFTP����˿�

// TFTPЭ��ͻ���
class CTftpPtl
{
public:
    CTftpPtl();
    ~CTftpPtl();
    HRESULT Connect(const char *pszAddress);
    HRESULT SendRRQ(const char *pszFileName, const char *pszMode, const char *pszBlkSize, const char *pszSize, int iTimeout);
    HRESULT SendWRQ(const char *pszFileName, const char *pszMode, const char *pszBlkSize, const char *pszSize, int iTimeout);
    HRESULT SendDATA(WORD16 wBlock, char *pcBuf, int iSize, int iTimeout);
    HRESULT SendACK(WORD16 wBlock, int iTimeout);
    HRESULT SendERROR();
    HRESULT DisConnect();
    HRESULT RecvData(char *pcBuf, int ilen, int *piRevLen, int iTimeout);
private:
    HV_SOCKET_HANDLE m_sktData;
    CSocketStream* m_pstmData;
    WORD16 m_wBlockSize;
    DWORD32 m_dwRemoteAddr;
    WORD16 m_wRemotePort;
};

/**
*  CNVCLinkʵ����
*/
#define RCV_BEGIN_TIMEOUT_NVC 100	// ���տ�ʼ��ʱ
#define RCV_TIMEOUT_NVC 500		// ���ճ�ʱ
#define MAX_RECV_SIZE_NVC 2048	// UDP��������

const DWORD32 JPEG_HEAD_SIZE = 595;

class CNVCLink
{
public:
    CNVCLink();
    ~CNVCLink();
    HRESULT Connect(const char* pszIP);
    HRESULT DisConnect();
    HRESULT ReadRegValue(BYTE8 bAddr, WORD16& wValue);
    HRESULT WriteRegValue(BYTE8 bAddr, WORD16 wValue);
    HRESULT GetImageHeight(DWORD32& dwHeight);
    HRESULT GetImageWidth(DWORD32& dwWidth);
    HRESULT GetDevVer(WORD16& wDevVer);
    HRESULT GetDevType(WORD16& wDevType);
    HRESULT SaveConfigForever();
    HRESULT RestoreDefaultConfig();
    HRESULT SetImgInfo(WORD16 wWidth = 2048, WORD16 wHeight = 1536);
    HRESULT SetQuality(WORD16 wQuality = 11);
    HRESULT GetJpgImge(char* pcBuf, DWORD32* pdwSize, DWORD32* pdwTime, WORD16 wRes = 1);
protected:
    HRESULT GetHeightFromCamera(WORD16& wHeight);
    HRESULT GetWidthFromCamera(WORD16& wWidth);
private:
    CTftpPtl m_tftp_ptl;
    BYTE8* m_pbRevBuf;
    int m_iQuality;
    WORD16 m_wWidth;
    WORD16 m_wHeight;
    BYTE8 m_rgbJpegHeader[JPEG_HEAD_SIZE];
};

const int MAX_RECV_BUF_SIZE = 1024 * 1024;

/**
*  CCamyuLink�ӿ���
*/
class ICamyuLink
{
public:
    virtual ~ICamyuLink(){};
    // ��ȡͼƬ��ʼ
    virtual HRESULT GetImageStart(const char* pszIP) = 0;
    // ��ȡͼƬ��ʼEx�������˸ú����Ͳ������GetImageStart
    virtual HRESULT GetImageStartEx(const char* pszIP, int iPort) = 0;
    // ��ȡͼƬ����
    virtual HRESULT GetImageStop() = 0;
    // ��ȡһ֡ͼƬ
    virtual HRESULT GetImage(char* pcBuf, DWORD32* pdwSize, DWORD32* pdwRefTime) = 0;
    // ���������ʼ
    virtual HRESULT CtrtCamStart(const char* pszIP) = 0;
    // �����������
    virtual HRESULT CtrtCamStop() = 0;
    // �������������
    virtual HRESULT SendCommand(DWORD32 dwCmdNo, DWORD32 dwParam1 = 0, DWORD32 dwParam2 = 0) = 0;
};

/**
*  CCamyuLinkʵ����
*/
class CCamyuLink : public ICamyuLink
{
public:
    CCamyuLink();
    virtual ~CCamyuLink();
    // ��ȡͼƬ��ʼ
    virtual HRESULT GetImageStart(const char* pszIP);
    // ��ȡͼƬ��ʼEx�������˸ú����Ͳ������GetImageStart
    virtual HRESULT GetImageStartEx(const char* pszIP, int iPort);
    // ��ȡͼƬ����
    virtual HRESULT GetImageStop();
    // ��ȡһ֡ͼƬ
    virtual HRESULT GetImage(char* pcBuf, DWORD32* pdwSize, DWORD32* pdwRefTime);
    // ���������ʼ
    virtual HRESULT CtrtCamStart(const char* pszIP);
    // �����������
    virtual HRESULT CtrtCamStop();
    // �������������
    virtual HRESULT SendCommand(DWORD32 dwCmdNo, DWORD32 dwParam1 = 0, DWORD32 dwParam2 = 0);

public:
    //��ȡ��ǰ����
    int GetCurRoadNum();
    // �ڲ�����������ݵ������,��ʱ���øú�������������
    HRESULT ProcessData();
    // ��ȡ��ǰ֡���ͣ�ָ�������������I֡��P֡
    int GetCurFrameType();

    HRESULT GetImageWidth(DWORD32& dwWidth);
    HRESULT GetImageHeight(DWORD32& dwHeight);
    HRESULT SetDataSktTimeout(DWORD32 dwTimeout);
    HRESULT SetCmdSktTimeout(DWORD32 dwTimeout);

private:
    HRESULT GetOneFrame(char* pcBuf, DWORD32* pdwSize, bool fMustData);
    HV_SOCKET_HANDLE m_sktData;
    HV_SOCKET_HANDLE m_sktCmd;
    CSocketStream* m_pstmData;
    CSocketStream* m_pstmCmd;
    char* m_pcTmpData;
    int m_iCurFrameType; // ���������������
    int m_iRoadNum;		//��ǰ������
    DWORD32 m_dwHeight;
    DWORD32 m_dwWidth;
};

//
// ����������Ϣͷ
typedef struct tag_head
{
    DWORD32 dwType;
    DWORD32 dwInfoLen;
    DWORD32 dwDataLen;
}
INFO_HEADER_HVCAM;

// ��������չ��Ϣͷ
typedef struct tag_block
{
    DWORD32 dwID;
    DWORD32 dwLen;
}
BLOCK_HEADER_HVCAM;

// �����ͷ
typedef struct tag_CameraCmdHeader
{
    DWORD32 dwID;
    DWORD32 dwInfoSize;
}
CAMERA_CMD_HEADER_HVCAM;

// �����Ӧ��
typedef struct tag_CameraCmdRespond
{
    DWORD32 dwID;
    DWORD32 dwInfoSize;
    int dwResult;
}
CAMERA_CMD_RESPOND_HVCAM;
//

/**
*  CHvCameraLink�ӿ���
*/
class IHvCameraLink
{
public:
    virtual ~IHvCameraLink(){};
    // ��ȡͼƬ��ʼ
    virtual HRESULT GetImageStart(const char* pszIP) = 0;
    // ��ȡͼƬ����
    virtual HRESULT GetImageStop() = 0;
    // ��ȡһ֡ͼƬ
    virtual HRESULT GetImage(char* pcBuf, DWORD32* pdwSize, DWORD32* pdwRefTime, DWORD32* pdwImgType) = 0;
    // ���������ʼ
    virtual HRESULT CtrtCamStart(const char* pszIP) = 0;
    // �����������
    virtual HRESULT CtrtCamStop() = 0;
    // �������������
    virtual HRESULT SendCommand( LPCSTR szCmd ) = 0;
};

/**
*  CHvCameraLinkʵ����
*/
class CHvCameraLink:public IHvCameraLink
{
public:
    CHvCameraLink();
    virtual ~CHvCameraLink();
    // ��ȡͼƬ��ʼ
    virtual HRESULT GetImageStart(const char* pszIP);
    // ��ȡͼƬ����
    virtual HRESULT GetImageStop();
    // ��ȡһ֡ͼƬ
    virtual HRESULT GetImage(char* pcBuf, DWORD32* pdwSize, DWORD32* pdwRefTime, DWORD32* pdwImgType);
    // ���������ʼ
    virtual HRESULT CtrtCamStart(const char* pszIP);
    // �����������
    virtual HRESULT CtrtCamStop();
    // �������������
    virtual HRESULT SendCommand( LPCSTR szCmd );

public:
    //��ȡ��ǰ����
    int GetCurRoadNum();
    // �ڲ�����������ݵ������,��ʱ���øú�������������
    HRESULT ProcessData();

    HRESULT GetImageWidth(DWORD32& dwWidth);
    HRESULT GetImageHeight(DWORD32& dwHeight);
    HRESULT SetDataSktTimeout(DWORD32 dwTimeout);
    HRESULT SetCmdSktTimeout(DWORD32 dwTimeout);
    HRESULT SendImageCmd(char* pszCmd);

private:
    HRESULT GetOneFrame(char* pcBuf, DWORD32* pdwSize, bool fMustData, DWORD32* pdwImgType);
    HV_SOCKET_HANDLE m_sktData;
    HV_SOCKET_HANDLE m_sktCmd;
    CSocketStream* m_pstmData;
    CSocketStream* m_pstmCmd;
    char* m_pcTmpData;
    int m_iCurFrameType;
    DWORD32 m_dwHeight;
    DWORD32 m_dwWidth;
};

/**
*  ����ץ������ӿ���
*/
class ICamTrigger
{
public:
    virtual ~ICamTrigger(void){};
    virtual HRESULT Trigger(LIGHT_TYPE nLightType, int iRoadNum = 0) = 0;
    virtual HRESULT HardTrigger(int iRoadNum) = 0;
    virtual HRESULT SetCamTriggerParam(CAM_CFG_PARAM *pCfgCamParam) = 0;
};

/**
*  ����ץ�����ʵ����
*/
class CHVTrigger : public ICamTrigger, public CHvThreadBase
{
public:
    CHVTrigger();
    virtual ~CHVTrigger(void);
    virtual const char* GetName()
    {
        static char szName[] = "CHVTrigger";
        return szName;
    }
    virtual HRESULT Run(void* pvParam);
    virtual HRESULT Trigger(LIGHT_TYPE nLightType, int iRoadNum = 0);
    virtual HRESULT HardTrigger(int iRoadNum);
    virtual HRESULT SetCamTriggerParam(CAM_CFG_PARAM *pCfgCamParam);
    HRESULT ProcessOnTrigger();
    bool ThreadIsOk();

private:
    int m_iCurRoadNum;

    DWORD32 m_dwLastThreadIsOkTime;
    LIGHT_TYPE m_nCurLightType;
    CHvCameraLink m_cHvCameraLink;
    CAM_CFG_PARAM *m_pCfgCamParam;
    HV_SEM_HANDLE m_hSemNeedTrigger;
};

/**
*  ����ץ�����ʵ����
*/
class CCamTrigger : public ICamTrigger, public CHvThreadBase
{
public:
    CCamTrigger();
    virtual ~CCamTrigger(void);
    virtual const char* GetName()
    {
        static char szName[] = "CCamTrigger";
        return szName;
    }
    virtual HRESULT Run(void* pvParam);
    virtual HRESULT Trigger(LIGHT_TYPE nLightType, int iRoadNum = 0);
    virtual HRESULT HardTrigger(int iRoadNum);
    virtual HRESULT SetCamTriggerParam(CAM_CFG_PARAM *pCfgCamParam);
    HRESULT ProcessOnTrigger();
    bool ThreadIsOk();

private:
    int m_iCurRoadNum;

    DWORD32 m_dwLastThreadIsOkTime;
    LIGHT_TYPE m_nCurLightType;
    CCamyuLink m_cCamyuLink;
    CAM_CFG_PARAM *m_pCfgCamParam;
    HV_SEM_HANDLE m_hSemNeedTrigger;
};

/**
*  IImgGatherer�ӿ�
*/
class IImgGatherer
{
public:
	IImgGatherer():m_pOnImage(NULL),m_pContext(NULL){}
    virtual ~IImgGatherer(){};
    virtual HRESULT GetCurStatus(char* pszStatus, int nStatusSizes) = 0;
    virtual HRESULT SetImgFrameParam(const IMG_FRAME_PARAM& cParam) = 0;
    virtual HRESULT SetCamCfgParam(LPVOID pCfgCamParam) = 0;
    virtual HRESULT SetLightType(LIGHT_TYPE cLightType, int iCplStatus) = 0;
    virtual HRESULT Play(LPVOID lpParam) = 0;
    virtual HRESULT SetImageEddyType(int nEddyType) = 0;
    virtual HRESULT MountTransmiter(ICamTransmit** ppCamTransmit) = 0;
    virtual BOOL IsCamConnected() = 0;

public:
	void RegisterImageCallbackFunction(void (*OnImage)(void* pContext, IMG_FRAME imgFrame), void* pContext)
	{
		m_pOnImage = OnImage;
		m_pContext = pContext;
	}
	virtual HRESULT PutOneFrame(IMG_FRAME imgFrame)
	{
		if (m_pOnImage)
		{
			m_pOnImage(m_pContext, imgFrame);
			return S_OK;
		}
		return E_FAIL;
	}

private:
	void (*m_pOnImage)(void* pContext, IMG_FRAME imgFrame);
	void* m_pContext;
};

/**
*  ץ��ͼ�������
*/
class CCamyuReceiver : public CHvThreadBase
{
public:
    CCamyuReceiver(void* pParent, HV_SOCKET_HANDLE hSock);
    ~CCamyuReceiver(void);

    // CHvThreadBase Interface
    virtual const char* GetName()
    {
        static char szName[] = "CCamyuReceiver";
        return szName;
    }
    virtual HRESULT Run(void* pvParam);

private:
    HV_SOCKET_HANDLE m_hSock;
    void* m_pParent;
    IMG_FRAME_PARAM m_cImgFrameParam;
};

/**
*  ץ��ͼ��������
*/
class CCamyuListener : public CHvThreadBase
{
public:
    CCamyuListener(void* pParent);
    ~CCamyuListener(void);
    // CHvThreadBase Interface
    virtual const char* GetName()
    {
        static char szName[] =  "CCamyuListener";
        return szName;
    }
    virtual HRESULT Run(void* pvParam);
    bool ThreadIsOk();

public:
    BOOL m_fIsCamConnected;

private:
    CCamyuReceiver* m_rgpCamyuReceiver[MAX_IMG_RECV_COUNT];
    void* m_pParent;
    int m_iStat;
    int m_iStatTimes;
};

/**
*  CImgCapturerʵ����
*/
class CImgCapturer : public IImgGatherer
{
public:
    CImgCapturer();
    virtual ~CImgCapturer();
    // IImgGatherer�ӿ�
    virtual HRESULT GetCurStatus(char* pszStatus, int nStatusSizes);
    virtual HRESULT SetImgFrameParam(const IMG_FRAME_PARAM& cParam);
    virtual HRESULT SetLightType(LIGHT_TYPE cLightType, int iCplStatus)
    {
        return E_NOTIMPL;
    }
    virtual HRESULT SetCamCfgParam(LPVOID pParam)
    {
        return E_NOTIMPL;
    }
    virtual HRESULT MountTransmiter(ICamTransmit** ppCamTransmit)
    {
        return E_NOTIMPL;
    }
    virtual HRESULT Play(LPVOID lpParam)
    {
        return m_pCamyuListener->Start(NULL);
    }
    virtual HRESULT SetImageEddyType(int nEddyType)
    {
        m_nEddyType = nEddyType;
        return S_OK;
    }
    virtual BOOL IsCamConnected()
    {
        if (m_pCamyuListener)
        {
            return m_pCamyuListener->m_fIsCamConnected;
        }
        return FALSE;
    }

public:
    HRESULT SetCapCamParam(
        int nEddyType,
        CAP_CAM_PARAM *pCapCamParam,
        COuterControlImpl *pSignalMatch
    );
private:
    CCamyuListener* m_pCamyuListener;
    IMG_FRAME_PARAM m_cImgFrameParam;
    CAP_CAM_PARAM *m_pCapCamParam;
    COuterControlImpl *m_pSignalMatch;
    int m_nEddyType;

    friend class CCamyuReceiver;
};

/**
*  CVideoGetter_Nvcʵ����
*/
class CVideoGetter_Nvc : public IImgGatherer, public CHvThreadBase
{
public:
    CVideoGetter_Nvc();
    virtual ~CVideoGetter_Nvc();
    // IImgGatherer�ӿ�
    virtual HRESULT SetCamCfgParam(LPVOID pCfgCamParam);
    virtual HRESULT GetCurStatus(char* pszStatus, int nStatusSizes);
    virtual HRESULT SetLightType(LIGHT_TYPE cLightType, int iCplStatus);
    virtual HRESULT SetImgFrameParam(const IMG_FRAME_PARAM& cParam)
    {
        return E_NOTIMPL;
    }
    virtual HRESULT Play(LPVOID lpParam)
    {
        return Start(lpParam);
    }
    virtual HRESULT SetImageEddyType(int nEddyType)
    {
        m_nEddyType = nEddyType;
        return S_OK;
    }
    virtual HRESULT MountTransmiter(ICamTransmit** ppCamTransmit)
    {
        m_ppCamTransmit = ppCamTransmit;
        return S_OK;
    }
    virtual BOOL IsCamConnected()
    {
        return m_fIsCamConnected;
    }

    HRESULT SetCapCamCfgParam(LPVOID pCfgCamParam);
    HRESULT CamSetLight(WORD16 wValue);
    HRESULT CamSetMode(WORD16 wValue, WORD16 wTime);
    HRESULT MountSignalMatch(COuterControlImpl *pSignalMatch)
    {
        if (!pSignalMatch)
        {
            return E_POINTER;
        }
        m_pSignalMatch = pSignalMatch;
        return S_OK;
    }

public:
    //CHvThreadBase
    virtual const char* GetName()
    {
        static char szName[] =  "CVideoGetter_Nvc";
        return szName;
    }
    virtual HRESULT Run(void*pvParam);

public:
    bool SetCamParameter();

private:
    CNVCLink m_cNvcLink;
    CAM_CFG_PARAM *m_pCfgCamParam;
    CAP_CAM_PARAM *m_pCapCamParam;
    COuterControlImpl *m_pSignalMatch;
    DWORD32 m_dwLastTime;
    DWORD32 m_dwFrameCount;
    LIGHT_TYPE m_cPlateLightType;
    int m_nEddyType;
    bool m_fIsLightTypeChanged;
    bool m_bActive;
    BOOL m_fIsCamConnected;
    ICamTransmit** m_ppCamTransmit;
};

/**
*  CVideoGetter_Camyuʵ����
*/
class CVideoGetter_Camyu : public IImgGatherer, public CHvThreadBase
{
public:
    CVideoGetter_Camyu();
    virtual ~CVideoGetter_Camyu();
    // IImgGatherer�ӿ�
    virtual HRESULT SetCamCfgParam(LPVOID pCfgCamParam);
    virtual HRESULT GetCurStatus(char* pszStatus, int nStatusSizes);
    virtual HRESULT SetLightType(LIGHT_TYPE cLightType, int iCplStatus);
    virtual HRESULT SetImgFrameParam(const IMG_FRAME_PARAM& cParam)
    {
        return E_NOTIMPL;
    }
    virtual HRESULT Play(LPVOID lpParam)
    {
        return Start(lpParam);
    }
    virtual HRESULT SetImageEddyType(int nEddyType)
    {
        m_nEddyType = nEddyType;
        return S_OK;
    }
    virtual HRESULT MountTransmiter(ICamTransmit** ppCamTransmit)
    {
        m_ppCamTransmit = ppCamTransmit;
        return S_OK;
    }
    virtual BOOL IsCamConnected()
    {
        return m_fIsCamConnected;
    }

    HRESULT SetCapCamCfgParam(LPVOID pCfgCamParam);
    HRESULT MountSignalMatch(COuterControlImpl *pSignalMatch)
    {
        if (!pSignalMatch)
        {
            return E_POINTER;
        }
        m_pSignalMatch = pSignalMatch;
        return S_OK;
    }

public:
    //CHvThreadBase
    virtual const char* GetName()
    {
        static char szName[] =  "CVideoGetter_Camyu";
        return szName;
    }
    virtual HRESULT Run(void*pvParam);

public:
    bool SetCamParameter();

private:
    CCamyuLink m_cCamyuLink;
    CAM_CFG_PARAM *m_pCfgCamParam;
    CAP_CAM_PARAM *m_pCapCamParam;
    COuterControlImpl *m_pSignalMatch;
    DWORD32 m_dwLastTime;
    DWORD32 m_dwFrameCount;
    LIGHT_TYPE m_cPlateLightType;
    int m_nEddyType;
    bool m_fIsLightTypeChanged;
    bool m_bActive;
    BOOL m_fIsCamConnected;
    ICamTransmit** m_ppCamTransmit;
};

/**
*  CVideoGetter_HvCamerʵ����
*/
class CVideoGetter_HvCamera : public IImgGatherer, public CHvThreadBase
{
public:
    CVideoGetter_HvCamera();
    virtual ~CVideoGetter_HvCamera();
    // IImgGatherer�ӿ�
    virtual HRESULT SetCamCfgParam(LPVOID pCfgCamParam);
    virtual HRESULT GetCurStatus(char* pszStatus, int nStatusSizes);
    virtual HRESULT SetLightType(LIGHT_TYPE cLightType, int iCplStatus);
    virtual HRESULT SetImgFrameParam(const IMG_FRAME_PARAM& cParam)
    {
        return E_NOTIMPL;
    }
    virtual HRESULT Play(LPVOID lpParam)
    {
        return Start(lpParam);
    }
    virtual HRESULT SetImageEddyType(int nEddyType)
    {
        m_nEddyType = nEddyType;
        return S_OK;
    }
    virtual HRESULT MountTransmiter(ICamTransmit** ppCamTransmit)
    {
        m_ppCamTransmit = ppCamTransmit;
        return S_OK;
    }
    virtual BOOL IsCamConnected()
    {
        return m_fIsCamConnected;
    }

    HRESULT SetCapCamCfgParam(LPVOID pCfgCamParam);
    HRESULT MountSignalMatch(COuterControlImpl *pSignalMatch)
    {
        if (!pSignalMatch)
        {
            return E_POINTER;
        }
        m_pSignalMatch = pSignalMatch;
        return S_OK;
    }
    HRESULT SetCaptureMode(LPVOID pCfgCamParam);

public:
    //CHvThreadBase
    virtual const char* GetName()
    {
        static char szName[] =  "CVideoGetter_HvCamera";
        return szName;
    }
    virtual HRESULT Run(void*pvParam);
    HRESULT RunCommMode();
    HRESULT RunCaptureMode();

public:
    bool SetCamParameter();

private:
    CHvCameraLink m_cHvCameraLink;
    CAM_CFG_PARAM *m_pCfgCamParam;
    CAP_CAM_PARAM *m_pCapCamParam;
    COuterControlImpl *m_pSignalMatch;
    DWORD32 m_dwLastTime;
    DWORD32 m_dwFrameCount;
    LIGHT_TYPE m_cPlateLightType;
    int m_nEddyType;
    bool m_fIsLightTypeChanged;
    bool m_bActive;
    BOOL m_fIsCamConnected;
    ICamTransmit** m_ppCamTransmit;
    bool m_fCaptureMode;
};

/**
*  CVideoGetter_Camyuʵ����
*/
class CVideoGetter_Broadcast : public IImgGatherer, public CHvThreadBase
{
public:
    CVideoGetter_Broadcast();
    virtual ~CVideoGetter_Broadcast();
    // IImgGatherer�ӿ�
    virtual HRESULT GetCurStatus(char* pszStatus, int nStatusSizes);
    virtual HRESULT Play(LPVOID lpParam)
    {
        return Start(lpParam);
    }
    virtual HRESULT SetImageEddyType(int nEddyType)
    {
        m_nEddyType = nEddyType;
        return S_OK;
    }
    virtual HRESULT SetLightType(LIGHT_TYPE cLightType, int iCplStatus)
    {
        return E_NOTIMPL;
    }
    virtual HRESULT SetCamCfgParam(LPVOID pCfgCamParam)
    {
        return E_NOTIMPL;
    }
    virtual HRESULT SetImgFrameParam(const IMG_FRAME_PARAM& cParam)
    {
        return E_NOTIMPL;
    }
    virtual HRESULT MountTransmiter(ICamTransmit** ppCamTransmit)
    {
        return E_NOTIMPL;
    }
    virtual BOOL IsCamConnected()
    {
        return TRUE;
    }
public:
    //CHvThreadBase
    virtual const char* GetName()
    {
        static char szName[] =  "CVideoGetter_Broadcast";
        return szName;
    }
    virtual HRESULT Run(void*pvParam);

private:
    CBroadcastLink m_cBroadcastLink;
    DWORD32 m_dwLastTime;
    DWORD32 m_dwFrameCount;
    int m_nEddyType;
};

//////////////////////////////////////////////////////////////////////////
// ����ͼƬ���ݸ�ʽ
// | ͼƬʱ�� | ͼƬ���� |  ͼƬ���� | ͼƬ���� |
// | 4 BYTE |128 BYTE | 4 BYTE  | n BYTE   |
//////////////////////////////////////////////////////////////////////////

/**
*  ����Э�����ʵ����
*/
class CTestLink
{
public:
    CTestLink();
    ~CTestLink();
    HRESULT Connect(const char* pszIP, WORD16 wPort);
    HRESULT GetImage(
        unsigned char* pcBuf,
        DWORD32* pdwSize,
        DWORD32* pdwTime,
        char* pszFileName
    );
    HRESULT Close();
    BOOL IsConnect();

private:
    HV_SOCKET_HANDLE m_hSocket;
    CSocketStream* m_pStream;
    DWORD32 m_dwRefTime;
    BOOL m_fConnectFlag;
};

/**
*  CVideoGetter_Testʵ����
*/
class CVideoGetter_Test : public IImgGatherer, public CHvThreadBase
{
public:
    CVideoGetter_Test();
    virtual ~CVideoGetter_Test();
    // IImgGatherer�ӿ�
    virtual HRESULT GetCurStatus(char* pszStatus, int nStatusSizes);
    virtual HRESULT SetCamCfgParam(LPVOID pCfgCamParam);
    virtual HRESULT SetLightType(LIGHT_TYPE cLightType, int iCplStatus)
    {
        return E_NOTIMPL;
    }
    virtual HRESULT SetImgFrameParam(const IMG_FRAME_PARAM& cParam)
    {
        return E_NOTIMPL;
    }
    virtual HRESULT MountTransmiter(ICamTransmit** ppCamTransmit)
    {
        return E_NOTIMPL;
    }
    virtual HRESULT Play(LPVOID lpParam)
    {
        return Start(lpParam);
    }
    virtual HRESULT SetImageEddyType(int nEddyType)
    {
        m_nEddyType = nEddyType;
        return S_OK;
    }
    virtual BOOL IsCamConnected()
    {
        return m_fIsCamConnected;
    }
public:
    //CHvThreadBase
    virtual const char* GetName()
    {
        static char szName[] =  "CVideoGetter_Test";
        return szName;
    }
    virtual HRESULT Run(void*pvParam);

private:
    CTestLink m_cTestLink;
    CAM_CFG_PARAM *m_pCfgCamParam;
    LIGHT_TYPE m_cPlateLightType;

    DWORD32 m_dwLastTime;
    DWORD32 m_dwImgWidth;
    DWORD32 m_dwImgHeight;
    DWORD32 m_dwFrameCount;

    int m_nEddyType;

    bool m_fIsLightTypeChanged;
    BOOL m_fIsCamConnected;
};

/**
*  CVideoGetter_Pciʵ����
*/
class CVideoGetter_Pci : public IImgGatherer, public CHvThreadBase
{
public:
    CVideoGetter_Pci();
    virtual ~CVideoGetter_Pci();

    HRESULT PutOneFrame(IMG_FRAME imgFrame);
    // IImgGatherer�ӿ�
    virtual HRESULT GetCurStatus(char* pszStatus, int nStatusSizes)
    {
        DWORD32 dwCurTime = GetSystemTick();
        if (dwCurTime < m_dwLastTime)
        {
            return S_OK;
        }
        if (dwCurTime - m_dwLastTime > 20000 && m_dwLastTime > 0)
        {
            HV_Trace(5, "current time = %d, last time = %d, escape = %d\n", dwCurTime, m_dwLastTime, dwCurTime - m_dwLastTime);
            return E_FAIL;
        }
        return S_OK;
    }
    virtual HRESULT SetLightType(LIGHT_TYPE cLightType, int iCplStatus)
    {
        return E_NOTIMPL;
    }
    virtual HRESULT SetImgFrameParam(const IMG_FRAME_PARAM& cParam)
    {
        return E_NOTIMPL;
    }
    virtual HRESULT SetCamCfgParam(LPVOID pCfgCamParam)
    {
        return E_NOTIMPL;
    }
    virtual HRESULT MountTransmiter(ICamTransmit** ppCamTransmit)
    {
        return E_NOTIMPL;
    }
    virtual HRESULT Play(LPVOID lpParam)
    {
        return Start(lpParam);
    }
    virtual HRESULT SetImageEddyType(int nEddyType)
    {
        m_nEddyType = nEddyType;
        return S_OK;
    }
    virtual BOOL IsCamConnected()
    {
        return TRUE;
    }

public:
    //CHvThreadBase
    virtual const char* GetName()
    {
        static char szName[] =  "CVideoGetter_PCI";
        return szName;
    }
    virtual HRESULT Run(void*pvParam)
    {
        while (!m_fExit)
        {
            m_dwLastTime = GetSystemTick();
            HV_Sleep(5000);
        }
        return S_OK;
    }

private:
    DWORD32 m_dwLastTime;
    int m_nEddyType;
};

#endif
