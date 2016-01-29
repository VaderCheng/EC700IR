#ifndef __SW_DOMECAMERA_PARAMETER_H__
#define __SW_DOMECAMERA_PARAMETER_H__


#include "SWFC.h"
#include "SWDomeCameraStruct.h"





typedef struct _tagDomeCameraParams
{
	INT		iPanCoordinate;
	INT		iTiltCoordinate;

	INT		iPanSpeed;
	INT		iMaxPanSpeed;
	INT		iTiltSpeed;
	INT		iMaxTiltSpeed;

	INT 	iFocusMode;

	INT		iLEDMode;
	INT		iLEDPower;

	INT		iLastTask;
	INT		iLastTaskID;
	
	PRESETPOS_PARAM_STRUCT	sPresetParam[MAX_PRESET];
	HSCAN_PARAM_STRUCT		sHScanParam[MAX_HSCAN_PATH];
	FSCAN_PARAM_STRUCT		sFScanParam[MAX_FSCAN_PATH];
	CRUISE_PARAM_STRUCT		sCruiseParam[MAX_CRUISE];
	WATCHKEEPING_PARAM_STRUCT	sWatchKeepingParam;
	PICTUREFREEZE_PARAM_STRUCT	sPictureFreezeParam;
	MASK_PARAM_STRUCT		sMaskParam[MAX_MASK];
	TIMER_PARAM_STRUCT		sTimerParam[MAX_TIMER];
	STANDBY_PARAM_STRUCT		sStandbyParam;

	_tagDomeCameraParams()
	{
		iPanCoordinate = 0;
		iTiltCoordinate = 0;
		iPanSpeed = 15;
        iMaxPanSpeed = MAX_SPEED;//0x3F
        iTiltSpeed = 15;
        iMaxTiltSpeed = MAX_SPEED;//0x3F
		iLEDMode = 0;
		iLEDPower = 100;
		iLastTask = -1;
		iLastTaskID = -1;
	}
} DomeCameraParams;



class CSWDomeCameraSetting : public CSWParameter<DomeCameraParams>
{
	CLASSINFO(CSWDomeCameraSetting, CSWObject)
public:
	

	CSWDomeCameraSetting();
	virtual ~CSWDomeCameraSetting();
	virtual HRESULT Initialize(CSWString& strFilePath);


	/**
	*@brief ��ʼ����������
	*@return �ɹ�����S_OK,����ֵ��ʾʧ��
	*/
	virtual HRESULT InitBasicParam();
	

	/**
	*@brief ��ʼ��Ԥ��λ
	*@return �ɹ�����S_OK,����ֵ��ʾʧ��
	*/
	virtual HRESULT InitPreset();

	/**
	*@brief ��ʼ��ˮƽɨ��
	*@return �ɹ�����S_OK,����ֵ��ʾʧ��
	*/
	virtual HRESULT InitHScan();

	/**
	*@brief ��ʼ������ɨ��
	*@return �ɹ�����S_OK,����ֵ��ʾʧ��
	*/
	virtual HRESULT InitFScan();

	/**
	*@brief ��ʼ��Ѳ��
	*@return �ɹ�����S_OK,����ֵ��ʾʧ��
	*/
	virtual HRESULT InitCruise();

	/**
	*@brief ��ʼ������
	*@return �ɹ�����S_OK,����ֵ��ʾʧ��
	*/
	virtual HRESULT InitWatchKeeping();

	/**
	*@brief ��ʼ����ʱ
	*@return �ɹ�����S_OK,����ֵ��ʾʧ��
	*/
	virtual HRESULT InitTimer();

	/**
	*@brief ��ʼ����˽����
	*@return �ɹ�����S_OK,����ֵ��ʾʧ��
	*/
	virtual HRESULT InitMask();

	/**
	*@brief ��ʼ�����涳��
	*@return �ɹ�����S_OK,����ֵ��ʾʧ��
	*/
	virtual HRESULT InitPictureFreeze();

	/**
	*@brief ��ʼ������
	*@return �ɹ�����S_OK,����ֵ��ʾʧ��
	*/
	virtual HRESULT InitSleep();

public:
	/**
	*@brief �ָ�Ĭ�ϲ���
	*@return �ɹ�����S_OK,����ֵ��ʾʧ��
	*/
	HRESULT ResetDefault();


protected:

	enum{DELETE_LEVEL = 0, INNER_LEVEL = 1, PROJECT_LEVEL = 3, CUSTOM_LEVEL = 5};

	CSWString m_strFilePath;
};
#endif

