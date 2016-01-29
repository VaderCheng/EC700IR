#pragma once

#include "HvUtils.h"
#include "PlayControl.h"
#include "hvthread.h"
#include "HvJpegProcesser.h"
#include "CamyuSeter.h"
#include "TestServer.h"

#if (RUN_PLATFORM == PLATFORM_WINDOWS)
#include "AviFromJpg.h"
#include <time.h>

// AVI ������
typedef struct
{
	__time64_t lFirstTime;				// ��һ֡�ľ���ʱ��(��)
	DWORD32 dwBeginTick;				// �������һ֡��Tick(����)
	int nTickOffset;					// ʱ��ƫ��(����)

	DWORD32 dwCurAviFirstTick;			// ��ǰAvi�ļ��ĵ�һ֡ʱ�꣨���룩
	DWORD32 dwCurAviLastTick;			// ��ǰAvi�ļ���һ֡��ʱ��(����)

	int nMsecPerFrame;				// ÿ֡��������룩
	int nCurSecFrames;				// ��������֡������
	DWORD32 dwCurSecTick;			// ��ǰ���ʱ��
	DWORD32 dwLastSendTick;			// �ϴε�ѹ�����е�ʱ��
	DWORD32 dwNextSendTick;			// �´ε�ѹ�����е�ʱ��
} 
AVI_FRAME_CTRL;

#endif

const DWORD32 JPEG_LIST_COUNT = 10;
const DWORD32 FRAME_LIST_COUNT = 1;

// JPEG�����߳�
class CProcessJpgThread : public HiVideo::IRunable
{
public:
	enum {THREAD_GET_JPEG = 0, THREAD_DECODE_JPEG, THREAD_RECOG_JPEG, THREAD_AVI_COMPRESS};
	CProcessJpgThread(void);
	~CProcessJpgThread(void);
	HRESULT Create(int iThreadType, int iPriority, void *pvParamter);
	HRESULT Close();
	bool IsConnected();
	virtual HRESULT Run(void* pvParamter);
protected:
	int m_iThreadType;
	HiVideo::IThread* m_pThread;
};

class CJpegSequence: public IPlayControl
{
public:
	CJpegSequence(void);
	virtual ~CJpegSequence(void);

	// IPlayControl�ӿڵķ�������
	STDMETHOD(Play)();
	STDMETHOD(Next)();
	STDMETHOD(Prev)();
	STDMETHOD(Seek)(long long rt);
	STDMETHOD(Stop)();
	STDMETHOD(Pause)();
	STDMETHOD(SetCallback)(CFrameSinkCallback pfnCallback, LPVOID pParam=NULL);
	STDMETHOD(SetParam)(const PLAYCONTROL_PARAM &param);
	STDMETHOD(GetParam)(PLAYCONTROL_PARAM &param);
	STDMETHOD(GetStatus)(int &iStatus);
	STDMETHOD(GetCameraStatus)(int &iStatus);

	virtual bool ThreadIsOk();
	//ǰ�˲ɼ��߳�ʵ��
	STDMETHOD(Process)() = 0;
	//JPG��ѹ�߳�ʵ��
	STDMETHOD(ProcessJpeg)();
	//ʶ�����߳�ʵ��
	STDMETHOD(ProcessFrame)();
#if (RUN_PLATFORM == PLATFORM_WINDOWS)

	//AVI ѹ���߳�
	STDMETHOD(ProcessAviCompress)();
#endif

protected:
	CProcessJpgThread m_threadGetJpg; // ǰ�˲ɼ��߳�
	CProcessJpgThread m_threadDecodeJpg; // Jpeg��ѹ�߳�
	CProcessJpgThread m_threadRecogJpg; // ʶ�����߳�
	CProcessJpgThread m_threadAviCompress; // AVIѹ���߳�

#ifndef _NML_DEVICE
	CTestServer m_testServer; //����Э�������
#endif

	//��Ƶ֡���ݽṹ
	struct JPG_FRAME {
		IReferenceComponentImage *pRefImage;
		HVPOSITION pCurrent;			//��ǰ֡���ڵ�λ�ã���������Ƶǰ�󵼺�
		int iVideoID;
		JPG_FRAME()
			:pRefImage(NULL)
			,pCurrent(0)
			,iVideoID(0)
		{
		}
	};
	//��ѹJPG֡
	BOOL JpgToBmp(struct JPG_FRAME &jpgframe);
	//���ûص�
	BOOL SendBmp(struct JPG_FRAME &jpgframe);

	HVPOSITION m_pNavigator;
	virtual BOOL GetNextJpgFrame(struct JPG_FRAME &jpgframe, HVPOSITION &pCurrent) = 0;
	virtual BOOL GetPrevJpgFrame(struct JPG_FRAME &jpgframe, HVPOSITION &pCurrent) = 0;

	BOOL m_fOpened;
	BOOL m_fPaused;
	CFrameSinkCallback m_pfnCallback;
	LPVOID m_pCallbackParam;

	//��Ƶ֡��������
	BOOL m_fEnableSave;
	CHvString m_strSavePath;
	bool m_fUseStorageSave;			// ʹ�ýṹ���洢�ɼ��ļ�
	bool m_fTermOfHour;			// ��Сʱ����
	HiVideo::IJpegSaver* m_pJpegSaver;

	//JPG���ݻ������
	CHvList<struct JPG_FRAME, JPEG_LIST_COUNT + 3> m_queJpeg;
	HiVideo::ISemaphore* m_psemJpegQue;

	//֡�������
	CHvList<struct JPG_FRAME, JPEG_LIST_COUNT + 3> m_queFrame;
	HiVideo::ISemaphore* m_psemFrameQue;

	// YUV ͼ�����
	CHvList<IReferenceComponentImage *, 10> m_queYUV;

	bool m_fExitThread;			// �߳��˳���־

	HiVideo::IJpegProcesser* m_pJpegProcesser;

	// ǰ�˲ɼ�״̬
	int m_iGetImageStatus;

#if (RUN_PLATFORM == PLATFORM_WINDOWS)
	// ץ���������
	CCamyuSeter* m_pCamyuSeter;

	CAviFromJpg *m_pAvi;					// ʹ��AVIѹ������
	bool m_fAviCompress;					// ʹ��AVIѹ��
	bool m_fStopAviCompress;				// ֹͣAVIѹ���߳�
	bool m_fAviTimeInit;					// AVI ѹ��ʱ���ʼ����־

	int m_nFrameRate;						// ֡��(ȱʡ2)
	int m_nCompressRate;					// ѹ����(ȱʡ2500 25%)
	int m_nAviMinutes;						// AVIѹ����ʱ�䣨���ӣ�

	AVI_FRAME_CTRL m_stAVI;					// AVI֡������

#endif

};

