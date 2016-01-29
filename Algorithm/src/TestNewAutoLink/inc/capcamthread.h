#pragma once
#include "hvvartype.h"
#include "hvthreadbase.h"
#include "hvsocket.h"

const int LISTEN_PORT = 9999; // ץ��ͼ�������˿�
const int MAX_RECV_THREAD = 10;
const int MAX_RECV_BUF_LEN = 1048576; // 1M
const int MAX_VIDEO_DATA_LEN = 4194304; // 4M

// ץ��������ݽ����߳�
class CCapCamRecvThread : public CHvThreadBase
{
public:
	CCapCamRecvThread(HiVideo::ISocket* pSocket);
	~CCapCamRecvThread(void);
	// CHvThreadBase Interface
	virtual HRESULT Run(void* pvParam);
private:
	HiVideo::ISocket* m_pSocket;
};


#if (RUN_PLATFORM == PLATFORM_WINDOWS)
//
// ץ��������ݽ����߳�
class CNCCamRecvThread : public CHvThreadBase
{
public:
	CNCCamRecvThread(void);
	~CNCCamRecvThread(void);
	// CHvThreadBase Interface
	virtual HRESULT Run(void* pvParam);

public:

	static const int MAX_CAM_IP_COUNT = 50;
	typedef struct
	{
		BYTE	CamId;			//���ID
		BYTE	CamIp[4];	//���IP��ַ
	}CAMLIST;

	typedef struct
	{
		DWORD32 dwCamCount;
		DWORD32 rgdwSignalTypeNum[MAX_CAM_IP_COUNT];
		CAMLIST   rgCamList[MAX_CAM_IP_COUNT];
	}CAMSIGNAL;

	CAMSIGNAL m_camsignal;

	typedef UINT (__cdecl *CALLBACK_PROC)(LPVOID);

	typedef bool (WINAPI *MYFUNC1 )(BYTE *);
	typedef bool (WINAPI *MYFUNC2 )();
	typedef bool (WINAPI *MYFUNC3 )(BYTE *);
	typedef bool (WINAPI *MYFUNC4 )(BYTE *,BYTE *);
	typedef bool (WINAPI *MYFUNC5 )(BYTE,CAMLIST *);
	typedef bool (WINAPI *MYFUNC6 )(BYTE *);
	typedef bool (WINAPI *MYFUNC7 )(BYTE *,BYTE);
	typedef bool (WINAPI *MYFUNC8 )(BYTE *,WORD);
	typedef bool (WINAPI *MYFUNC9 )(BYTE *,WORD);
	typedef bool (WINAPI *MYFUNC10)(BYTE *,WORD);
	typedef bool (WINAPI *MYFUNC11)(BYTE *,WORD);
	typedef bool (WINAPI *MYFUNC12)(BYTE *,WORD);
	typedef bool (WINAPI *MYFUNC13)(BYTE *,WORD);
	typedef bool (WINAPI *MYFUNC14)(BYTE *,BYTE);
	typedef bool (WINAPI *MYFUNC15)(BYTE *,BYTE);
	typedef bool (WINAPI *MYFUNC16)(BYTE *,BYTE);
	typedef bool (WINAPI *MYFUNC17)(BYTE *,BYTE);
	typedef bool (WINAPI *MYFUNC18)(BYTE *);
	typedef bool (WINAPI *MYFUNC19)(BYTE *);
	typedef bool (WINAPI *MYFUNC20)(BYTE *,BYTE *);
	typedef bool (WINAPI *MYFUNC21)(BYTE *,BYTE *);
	typedef int (WINAPI *MYFUNC22)(CALLBACK_PROC,LPVOID);
	typedef bool (WINAPI *MYFUNC23)(BYTE *);
	typedef int (WINAPI *MYFUNC24)(CALLBACK_PROC,LPVOID);
	typedef bool (WINAPI *MYFUNC25)(BYTE*,WORD *,WORD *,BYTE*);
	typedef int (WINAPI *MYFUNC26)(CALLBACK_PROC,LPVOID);
	typedef bool (WINAPI *MYFUNC27)(BYTE*,BYTE*);
	typedef bool (WINAPI *MYFUNC28)(BYTE*,BYTE*);
	typedef bool (WINAPI *MYFUNC29)(HDC,WORD,WORD,WORD,WORD,BYTE *);
	typedef bool (WINAPI *MYFUNC30)(LPCTSTR,WORD,WORD,BYTE *);
	typedef bool (WINAPI *MYFUNC31)(LPCTSTR,WORD,WORD,BYTE,BYTE *);
	typedef bool (WINAPI *MYFUNC32)(BYTE,LPCTSTR);
	typedef bool (WINAPI *MYFUNC33)(BYTE *,BYTE);
	typedef bool (WINAPI *MYFUNC34)(BYTE *,BOOL,BYTE,BYTE,BYTE);
	typedef bool (WINAPI *MYFUNC35)(BYTE *,BOOL);
	typedef bool (WINAPI *MYFUNC36)(BYTE *,WORD);
	typedef bool (WINAPI *MYFUNC37)(BYTE*,DWORD*,WORD,WORD,BYTE,BYTE *);

	MYFUNC1 	NC_SystemInit	;			
	MYFUNC2 	NC_SystemFree	;			
	MYFUNC3 	NC_Reset		;			
	MYFUNC4 	NC_GetActip		;
	MYFUNC5 	NC_SetCamlist	;			
	MYFUNC6 	NC_GetCaminfo	;			
	MYFUNC7 	NC_SetAWB		;			
	MYFUNC8 	NC_SetR			;			
	MYFUNC9 	NC_SetG			;
	MYFUNC10	NC_SetB			;			
	MYFUNC11	NC_SetExpor		;			
	MYFUNC12	NC_SetGain		;			
	MYFUNC13	NC_SetBlk		;			
	MYFUNC14	NC_SetPrvMode	;			
	MYFUNC15	NC_SetLut		;			
	MYFUNC16	NC_Setprive		;			
	MYFUNC17	NC_Setflash		;			
	MYFUNC18	NC_SetSave		;			
	MYFUNC19	NC_SetSnapShot	;			
	MYFUNC20	NC_SetCamip		;			
	MYFUNC21	NC_SetServerip	;				
	MYFUNC22	NC_CaminfoCallback;
	MYFUNC23	NC_GetCaminfobuf;			
	MYFUNC24	NC_CamPreviewCallback;
	MYFUNC25	NC_GetCamPreviewbuf;
	MYFUNC26	NC_CamPhotoCallback;
	MYFUNC27	NC_GetCamRawbuf;			
	MYFUNC28	NC_GetCamPhotobuf;
	MYFUNC29	NC_DrawDisplay;
	MYFUNC30	NC_SaveImgBMP;
	MYFUNC31	NC_SaveImgJPG;
	MYFUNC32	NC_Updatefireware;			
	MYFUNC33	NC_SetNetMode;	
	MYFUNC34	NC_SetAEC;
	MYFUNC35	NC_SetAFC;
	MYFUNC36	NC_SetPAGDELAY;
	MYFUNC37	NC_BMP2JPG;


	UINT CaminfoProc();
	UINT CamPhotoProc();
	UINT CamPreviewProc();

	int AddCamIp(DWORD32 dwSignalTypeNum, BYTE* pbIp);

private:
	static const int IMAGEWIDTH = 1360;
	static const int IMAGEHIGTH = 1024;
	static const int IMAGESIZE = 1360*1024;

	HINSTANCE   m_hInst;

	typedef struct{
		BYTE	HAND[4];		//���ͷ
		DWORD	       CAMID;			//���ID
		DWORD 	FVER;			//FPGA�汾
		DWORD 	DVER;			//DSP�汾
		BYTE	CAMDMAC[6];		//���MAC
		BYTE	CAMIPAddress[4];	//���IP��ַ
		BYTE	SERDMAC[6];		//������MAC
		BYTE	SERIPAddress[4];	//������IP��ַ
		WORD	sourceport;		//Դ�˿�
		WORD	targetport;		//Ŀ�Ķ˿�
		WORD	Total_Pixel;	//������
		WORD	Width;			//ԭʼ��
		WORD	Height;			//ԭʼ��
		WORD	R;
		WORD	G;
		WORD	B;
		WORD	exp;			//�ع�
		WORD	gain;			//����
		WORD	blckl;			//�ڵ�ƽ
		BYTE	mode;			//ģʽ
		BYTE	lut;			//���ұ�
		WORD	shaketime;		//����ʱ��
		WORD	shaketimeov;	//��ʱʱ��
		BYTE	previwen;		//Ԥ��
		BYTE	flashen;		//ץ�Ĳ����
		BYTE	AWB;			//�Զ���ƽ��
		BYTE	AFE;			//�Զ��ع�
		BYTE	NETMODE;		//����ģʽ
		BYTE	AECTOP;			//����ع�ֵ
		BYTE	AGCTOP;			//�������ֵ
		BYTE	BLC;			//�عⲹ��ֵ
		WORD	PAGDELAY;		//���ݰ�����ʱ
		BYTE	AFL;			//�Զ�ץ�Ĳ����
	}CAMINFO;
	CAMINFO * m_pCamInfo;  //�������ֵ

	typedef struct
	{
		WORD	Imgtype;			//ͼ������
		WORD	Width;				//ͼ���
		WORD	Height;				//ͼ���
		WORD	Carspeed1;			//����1
		WORD	Carspeed2;			//����2
		WORD	Carlengh1;			//����1
		WORD	Carlengh2;			//����2
		WORD	Radspeed;			//�״ﳵ��
		WORD	Radlengh;			//�״ﳵ��
		WORD	Res ;				//����
		BYTE	ImageData[IMAGESIZE*3];    //������ 
	}IMGinfo;
	IMGinfo* m_pImgInfo;

	BYTE* m_pbInfoBuff;
	BYTE* m_pbPhotoBuff;
	BYTE* m_pbJpegBuff;

	bool m_fInitialized;
};
#else
//
// ץ��������ݽ����߳�
class CNCCamRecvThread : public CHvThreadBase
{
public:
	CNCCamRecvThread(void);
	~CNCCamRecvThread(void)
	{
		m_fExit = true;
		StopThread(-1);
	};
	// CHvThreadBase Interface
	virtual HRESULT Run(void* pvParam)
	{
		m_fExit = true;
		return E_FAIL;
	}
};
#endif

// ץ����������߳�
class CCapCamThread : public HiVideo::IRunable
{
public:
	CCapCamThread(void);
	~CCapCamThread(void);
	HRESULT Create();
	HRESULT Close();
	bool IsConnected();
	bool ThreadIsOk();
	virtual HRESULT Run(void* pvParamter);
protected:
	bool m_fRunControl;
	bool m_fRunStatus;
	HiVideo::IThread* m_pThread;
	CCapCamRecvThread* m_rgpRecvThread[MAX_RECV_THREAD];

#ifndef _NML_DEVICE 
	char m_rgbRecvBuf[MAX_RECV_BUF_LEN];
	char m_rgbVideoData[MAX_VIDEO_DATA_LEN];
#endif

	DWORD32 m_dwCurVideoPos;
};
