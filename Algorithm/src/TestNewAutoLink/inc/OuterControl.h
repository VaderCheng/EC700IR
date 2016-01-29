#ifndef _OUTERCONTROL_INCLUDED__
#define _OUTERCONTROL_INCLUDED__

#include "swObjBase.h"
#include "swImage.h"
#include "HvInterface.h"
#include "TrackerCallBack.h"
#include "hvvartype.h"

#ifdef WIN32
#pragma warning(disable:4183)
#endif
// ������Ϣ����
typedef enum
{
	ATTACH_INFO_NOTHING = 0,				// û�и�����Ϣ
	ATTACH_INFO_SPEED,						// ���������ٶ�
	ATTACH_INFO_LAST_IMAGE,					// ����Last����ͼƬ
	ATTACH_INFO_WEIGHT						// ��������
} ATTACH_INFO_TYPE;

// ���ƺ��ź�״̬����
typedef enum  
{
	STATUS_INIT = 0,					// ��ʼ״̬
	STATUS_ADD_IMAGE,					// ���źŸ���ͼƬ
	STATUS_OUTTIME,						// ��Ϣ����ʱ������
	STATUS_MATCH						// ��Ϣ�Ѿ�ƥ��
}INFO_STATUS_TYPE;

// ƥ������
typedef enum
{
	MATCH_PLATE = 0,				// ����ƥ��
	MATCH_NO_PLATE			// ����ƥ��
}MATCH_TYPE;

// ���Ʒ�ʽ����
typedef enum
{
	PLATE_OUTPUT_AUTO = 0,					// �Զ����Ʒ�ʽ���������źţ�����ƥ��
	PLATE_OUTPUT_SIGNAL						// ���Ʊ��뿼���ź�
} OUTPUT_PLATE_MODE;

// ����ģʽ����
typedef enum
{
	WORK_MODE_SINGLE = 0,					// ��������ʽ
	WORK_MODE_MULTI							// �೵����ʽ
} WORK_MODE_TYPE;

// ���ƶ������ݽṹ
typedef struct
{
	CARLEFT_INFO_STRUCT *pCarleftInfo;		// ������Ϣ
	LPVOID pvUserData;						// �û���������
	INFO_STATUS_TYPE nStatus;				// ����״̬
	DWORD32 dwPlateTime;					// ����ʱ��
	DWORD32 dwInputTime;					// ����е�ʱ��
	BYTE8 bValue;							// ��ֵ(�ٶ�)
} PLATE_INFO_hxl;

// �ź����ݽṹ
typedef struct _SIGNAL_INFO
{
	int nType;								// �ź�Դ����
	INFO_STATUS_TYPE nStatus;				// �ź�״̬
	DWORD32 dwSignalTime;					// �ź�ʱ��
	DWORD32 dwInputTime;					// �����ʱ��
	BYTE8 bValue;							// ��ֵ(�ٶ�)
	DWORD32 dwRoad;							// �������(��16λΪ0x00FF),�����0x00FF00FF,����ȫ����
	DWORD32 dwFlag;							//�����Ÿ�16λ��ֵ
	DWORD32 dwOutTime;						//�źŹ���ʱ��
	int iModifyRoad;						//���ڲ�ͬ����������һ��ʱ������Ƿ���Ҫ����������
	IReferenceComponentImage *pImage;		// �źŴ���ʱ�Ĵ�ͼ
	IReferenceComponentImage *pImageLast;	// �źŴ���ʱ�Ĵ�ͼ(last)
	_SIGNAL_INFO()
	{
		dwRoad = 0;
		dwFlag = 0;
		iModifyRoad = 0;
		dwOutTime = 0;
	}
} SIGNAL_INFO;

// �ź�Դ
typedef struct
{
	int nType;								// �ź�����, ��SIGNAL_INFOָ��
	DWORD32 dwPrevTime;						// �ź�ǰ��Чʱ��(����)
	DWORD32 dwPostTime;						// �źź���Чʱ��(����)
	int iRoad;									// �����ĳ������,��0��ʼ,�����������3,��3��ʾȫ����
} SIGNAL_SOURCE;


// �ź����ͽṹ
typedef struct
{
	char strName[40];						// ��������
	bool fMainSignal;						// �Ƿ����ź�	true:���ź� false:�����ź�
	ATTACH_INFO_TYPE atType;				// ������Ϣ����
} SIGNAL_TYPE;

// ƥ�������Ϣ�ṹ
typedef struct
{
	int nSigSourceType;					// �ź�Դ���
	HVPOSITION nSignalPos;				// ƥ����ź�λ��
	HVPOSITION nSignalPos_2;			// ƥ��ĵڶ����ź�λ��
	HVPOSITION nSignalPos_3;			// ƥ��ĵڶ����ź�λ��
	SDWORD32 sdwTimeDiff;				// ƥ��ʱ�����
	DWORD32 dwPosDiff;					// ƥ��������
	BYTE8 bValueDiff;					// ֵ�Ĳ���(�ٶȵ�)
} MATCH_ANALYSIS;

// ƥ����Ϣ����
typedef struct
{
	MATCH_TYPE nMatchType;			// ƥ������ 0:���Ƴ� 1:���Ƴ�
	void *pInfo;						// PlateInfo����SignalInfoָ�룬ȡ����nMatchType
	BYTE8 bSpeed;						// �ٶ�
	BYTE8 bWeight;					// ����
	IReferenceComponentImage *pLastImage;	// ���ӵ�ͼ��
	IReferenceComponentImage *pLastImage_2;	// ���ӵĵڶ���ͼ��
	IReferenceComponentImage *pLastImage_3;	// ���ӵĵ�����ͼ��
} MATCH_INFO;

//�������ʱ��
typedef struct
{
	int  iRoadNumber;			//�������ڳ���
	DWORD32 dwTime;		//�����źŲ���ʱ��
	DWORD32 dwDelay;		//�����ź��ӳٷ���ʱ��
} OUTPUT_SIGNAL;

class IOuterControler : public ITrackerCallback
{
public:
	virtual ~IOuterControler() {};

	STDMETHOD(Run)() = 0;
	STDMETHOD(Stop)() = 0;

	STDMETHOD(SetCallBack)(
		ITrackerCallback *pCallback
	)=0;
//	STDMETHOD(ForceResult)() = 0;

	//ΪHvCoreʵ�ֵ�Init����
	STDMETHOD(InitOuterControler)() = 0;

	//���ò�������ӿ�
	STDMETHOD(SetHvParam)(HvCore::IHvParam2* pHvParam) = 0;

	//ITrackCallBack
	STDMETHOD(CarArrive)(
		CARARRIVE_INFO_STRUCT *pCarArriveInfo,
		LPVOID pvUserData
		) = 0;

	STDMETHOD(CarLeft)(
		CARLEFT_INFO_STRUCT *pCarLeftInfo,
		LPVOID pvUserData
		) = 0;

	STDMETHOD(DisposeUserData)(
		UINT iFrameNo,
		UINT iRefTime,
		LPVOID pvUserData
		) = 0;

	STDMETHOD(ProcessOneFrame)(
		IReferenceComponentImage *pImage
		) = 0;

	virtual bool ThreadIsOk() = 0;
};

extern HRESULT CreateOuterControler(IOuterControler** ppOuterControler);

extern HRESULT AppendSignal(SIGNAL_INFO *pSignalInfo);		// �����ź�
extern HRESULT ClearSignalQueue();								// ����źŶ���

#endif // _OUTERCONTROL_INCLUDED__
