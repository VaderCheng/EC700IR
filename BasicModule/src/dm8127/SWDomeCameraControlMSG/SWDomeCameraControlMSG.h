// ���ļ������ʽ������WIN936

#ifndef _SWDOMECAMERACONTROLMSG_H_
#define _SWDOMECAMERACONTROLMSG_H_
#include "SWObject.h"
#include "SWMessage.h"
#include "SWDomeCameraPreset.h"
#include "SWDomeCameraHScan.h"
#include "SWDomeCameraFScan.h"
#include "SWDomeCameraCruise.h"
#include "SWDomeCameraTimer.h"
#include "SWDomeCameraMask.h"
#include "SWDomeCameraPictureFreeze.h"
#include "SWDomeCameraWatchkeeping.h"
#include "SWDomeCameraTimer.h"
#include "SWDomeCameraStandby.h"



class CSWDomeCameraControlMSG : public CSWMessage, public CSWObject
{
    CLASSINFO(CSWDomeCameraControlMSG, CSWObject)

public:
    
    CSWDomeCameraControlMSG();
    virtual ~CSWDomeCameraControlMSG();
    
protected:
    /**
     * @brief MSG_INITIAL_DOMECAMERA �������������ʼ�������ṹ��
     * @param [in] wParam (CAMERAPARAM_INFO* �ṹָ�����ͣ�
     * @param [in] lParam ����
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnInitialDomeCamera( WPARAM wParam,LPARAM lParam );

	/**
     * @brief  
     * @param [in] wParam 
     * @param [out] lParam
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnResetDomePosition( WPARAM wParam,LPARAM lParam );

	/**
     * @brief  ����LED��ģʽ
     * @param [in] wParam 0���أ�1������3���Զ�
     * @param [out] lParam
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnSetLedMode( WPARAM wParam,LPARAM lParam );

    /**
     * @brief  ��ȡLED��ģʽ
     * @param [in] wParam ����
     * @param [out] lParam 0���أ�1������3���Զ�
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnGetLedMode( WPARAM wParam,LPARAM lParam );

	/**
     * @brief  
     * @param [in] wParam  BIT 0~7:Զ��  8~15:��  16~23:��
     * @param [in] lParam ����
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
	HRESULT OnSetLedPower(WPARAM wParam,LPARAM lParam);

	/**
     * @brief  
     * @param [in] wParam 
     * @param [in] lParam ����
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
	HRESULT OnGetLedPower(WPARAM wParam,LPARAM lParam);
	    
    /**
     * @brief  ����ˮƽ�Ƕ� 0~3599. ��λ0.1��
     * @param [in] wParam 
     * @param [in] lParam ����
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnSetPanCoordinate( WPARAM wParam,LPARAM lParam );
    /**
     * @brief  ��ȡˮƽ�Ƕ� 0~3599. ��λ0.1��
     * @param [in] wParam ����
     * @param [out] lParam 
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnGetPanCoordinate( WPARAM wParam,LPARAM lParam );
    
    /**
     * @brief ���ô�ֱ�Ƕ� 0~900. ��λ0.1��
     * @param [in] wParam 
     * @param [in] lParam ����
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnSetTiltCoordinate( WPARAM wParam,LPARAM lParam );
    /**
     * @brief  ��ȡ��ֱ�Ƕ� 0~900. ��λ0.1��
     * @param [in] wParam ����
     * @param [out] lParam 
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnGetTiltCoordinate( WPARAM wParam,LPARAM lParam );

	/**
     * @brief ����ˮƽ�ʹ�ֱ�Ƕ� 
     * @param [in] wParam 
     * @param [in] lParam ����
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
	HRESULT OnSetPanTiltCoordinate( WPARAM wParam,LPARAM lParam );

	/**
     * @brief ��ȡˮƽ�ʹ�ֱ�Ƕ� 
     * @param [in] wParam 
     * @param [in] lParam ����
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
	HRESULT OnGetPanTiltCoordinate( WPARAM wParam,LPARAM lParam );
    
    /**
     * @brief  ���ý���
     * @param [in] wParam  0~31424
     * @param [in] lParam ����
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnSetZoom( WPARAM wParam,LPARAM lParam );

    /**
     * @brief ��ȡ����
     * @param [in] wParam ����
     * @param [out] lParam 0~31424
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnGetZoom( WPARAM wParam,LPARAM lParam );

    /**
     * @brief ˮƽ��ֱת��
     * @param [in] wParam 
     * @param [in] lParam ����
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnStartPanTilt( WPARAM wParam,LPARAM lParam );
    /**
     * @brief  
     * @param [in] wParam ����
     * @param [out] lParam 
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnStopPanTilt( WPARAM wParam,LPARAM lParam );

    /**
     * @brief  ����ˮƽת���ٶ�  0 ~ MAX_SPEED
     * @param [in] wParam 
     * @param [in] lParam ����
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnSetPanSpeed( WPARAM wParam,LPARAM lParam );

    /**
     * @brief  
     * @param [in] wParam ����
     * @param [out] lParam 
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnGetPanSpeed( WPARAM wParam,LPARAM lParam );

	/**
     * @brief  �������ˮƽת���ٶ�  0 ~ MAX_SPEED
     * @param [in] wParam
     * @param [in] lParam ����
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnSetMaxPanSpeed( WPARAM wParam,LPARAM lParam );

    /**
     * @brief  
     * @param [in] wParam ����
     * @param [out] lParam 
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnGetMaxPanSpeed( WPARAM wParam,LPARAM lParam );


	/**
     * @brief  ���ô�ֱת���ٶ�  0 ~ MAX_SPEED
     * @param [in] wParam 
     * @param [in] lParam ����
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnSetTiltSpeed( WPARAM wParam,LPARAM lParam );

    /**
     * @brief  
     * @param [in] wParam ����
     * @param [out] lParam 
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnGetTiltSpeed( WPARAM wParam,LPARAM lParam );

	/**
     * @brief  �������ֱת���ٶ�  0 ~ MAX_SPEED
     * @param [in] wParam 
     * @param [in] lParam ����
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnSetMaxTiltSpeed( WPARAM wParam,LPARAM lParam );

    /**
     * @brief  
     * @param [in] wParam ����
     * @param [out] lParam
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnGetMaxTiltSpeed( WPARAM wParam,LPARAM lParam );

	/**
     * @brief ���þۼ�ֵ
     * @param [in] wParam 0x1000 - 0xc000
     *              0x1000: Over Inf
	 *				0x2000: 20 m
					0x3000: 10 m
					0x4000: 6 m
					0x5000: 4.2 m
					0x6000: 3.1 m
					0x7000: 2.5 m
					0x8000: 2.0 m
					0x9000: 1.65 m
					0xA000: 1.4 m
					0xB000: 1.2 m
					0xC000: 0.8 m
					0xD000: 30 cm
					(initial setting)
					0xE000: 11 cm
					0xF000: 1 cm
     * @param [in] lParam ����
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnSetFocus( WPARAM wParam,LPARAM lParam );

    /**
     * @brief ��ȡ�ۼ�ֵ
     * @param [in] wParam ����
     * @param [out] lParam 0x1000 - 0xd000
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnGetFocus( WPARAM wParam,LPARAM lParam );

	/**
     * @brief ���þ۽�ģʽ
     * @param [in] wParam 0:������1��Զ����3������Զ��
     * @param [in] lParam ����
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnSetFocusMode( WPARAM wParam,LPARAM lParam );

    /**
     * @brief ��ȡ�۽�ģʽ
     * @param [in] wParam ����
     * @param [out] lParam 0:������1��Զ����3������Զ��
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnGetFocusMode( WPARAM wParam,LPARAM lParam );

    /**
     * @brief  �Զ��۽�ʹ��
     * @param [in] wParam 1���ֶ���0���Զ�
     * @param [out] lParam
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnSetAFEnable( WPARAM wParam,LPARAM lParam );

    /**
     * @brief  ��ȡ�Զ��۽�ʹ��λ
     * @param [in] wParam ����
     * @param [out] lParam 1���ֶ���0���Զ�
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnGetAFEnable( WPARAM wParam,LPARAM lParam );

    /**
     * @brief  ִ��һ�δ����۽�
     * @param [in] wParam ��
     * @param [out] lParam ��
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnDoOneFocus( WPARAM wParam,LPARAM lParam );

	/**
     * @brief  
     * @param [in] wParam ��
     * @param [out] lParam ��
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnMovePointToCenter( WPARAM wParam,LPARAM lParam );

	/**
     * @brief  
     * @param [in] wParam ��
     * @param [out] lParam ��
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnMoveBlockToCenter( WPARAM wParam,LPARAM lParam );


	/**
     * @brief  
     * @param [in] wParam 0:close ,1~0xD
     * 				0D 	F1.6
					0C 	F2
					0B	F2.4
					0A	F2.8
					09	F3.4
					08	F4
					07	F4.8
					06	F5.6
					05	F6.8
					04	F8
					03	F9.6
					02	F11
					01	F14
					00	CLOSE
     * @param [in] lParam ����
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnSetIris( WPARAM wParam,LPARAM lParam );

    /**
     * @brief  
     * @param [in] wParam ����
     * @param [out] lParam 0:close ,1~0xD
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnGetIris( WPARAM wParam,LPARAM lParam );

	/**
     * @brief  
     * @param [in] wParam 
     * @param [in] lParam ����
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnSetIrisMode( WPARAM wParam,LPARAM lParam );

    /**
     * @brief  
     * @param [in] wParam ����
     * @param [out] lParam 
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnGetIrisMode( WPARAM wParam,LPARAM lParam );

	/**
     * @brief  ���ڹ�Ȧ
     * @param [in] wParam ����
     * @param [out] lParam 
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnTuneIris( WPARAM wParam,LPARAM lParam );

	/**
     * @brief  ���ڽ���
     * @param [in] wParam ����
     * @param [out] lParam 
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnTuneZoom( WPARAM wParam,LPARAM lParam );

	/**
     * @brief  ���ھ۽�
     * @param [in] wParam ����
     * @param [out] lParam 
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnTuneFocus( WPARAM wParam,LPARAM lParam );

	/**
     * @brief  �����佹
     * @param [in] wParam 
     * @param [in] lParam ����
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnStartZoom(WPARAM wParam,LPARAM lParam);

    /**
     * @brief  ֹͣ�佹
     * @param [in] wParam ����
     * @param [out] lParam
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnStopZoom(WPARAM wParam,LPARAM lParam);
    
    /**
     * @brief  ����ˮƽת��
     * @param [in] wParam 
     * @param [in] lParam ����
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnStartPan(WPARAM wParam,LPARAM lParam);

    /**
     * @brief  ֹͣˮƽת��
     * @param [in] wParam ����
     * @param [out] lParam
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnStopPan(WPARAM wParam,LPARAM lParam);

	/**
     * @brief  ��������ˮƽת��
     * @param [in] wParam 
     * @param [in] lParam ����
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnStartInfinityPan(WPARAM wParam,LPARAM lParam);

    /**
     * @brief  ֹͣ����ˮƽת��
     * @param [in] wParam ����
     * @param [out] lParam
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnStopInfinityPan(WPARAM wParam,LPARAM lParam);

    /**
     * @brief  ��ֱת��
     * @param [in] wParam 
     * @param [in] ����
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnStartTilt(WPARAM wParam,LPARAM lParam);

    /**
     * @brief  
     * @param [in] wParam ����
     * @param [in] lParam 
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnStopTilt(WPARAM wParam,LPARAM lParam);

	/**
     * @brief  
     * @param [in] wParam 
     * @param [in] ����
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnStartVTurnOver(WPARAM wParam,LPARAM lParam);

    /**
     * @brief  
     * @param [in] wParam ����
     * @param [in] lParam 
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnStopVTurnOver(WPARAM wParam,LPARAM lParam);

	/**
     * @brief  ������ˢ
     * @param [in] wParam 
     * @param [in] ����
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnStartWiper(WPARAM wParam,LPARAM lParam);

    /**
     * @brief  
     * @param [in] wParam ����
     * @param [in] lParam 
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnStopWiper(WPARAM wParam,LPARAM lParam);

	/**
     * @brief  
     * @param [in] wParam 
     * @param [in] lParam ����
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnSetWiperSpeed( WPARAM wParam,LPARAM lParam );

    /**
     * @brief  
     * @param [in] wParam ����
     * @param [out] lParam 
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnGetWiperSpeed( WPARAM wParam,LPARAM lParam );

	/**
     * @brief  
     * @param [in] wParam 
     * @param [in] lParam ����
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnSetWiperAutoStartTime( WPARAM wParam,LPARAM lParam );

    /**
     * @brief  
     * @param [in] wParam ����
     * @param [out] lParam 
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnGetWiperAutoStartTime( WPARAM wParam,LPARAM lParam );

	/**
     * @brief  
     * @param [in] wParam 
     * @param [in] lParam ����
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnSetPresetPos( WPARAM wParam,LPARAM lParam );

    /**
     * @brief  
     * @param [in] wParam ����
     * @param [out] lParam 
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnGetPresetPos( WPARAM wParam,LPARAM lParam );

	/**
     * @brief  
     * @param [in] wParam 
     * @param [in] lParam ����
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnCallPresetPos( WPARAM wParam,LPARAM lParam );

    /**
     * @brief  
     * @param [in] wParam ����
     * @param [out] lParam 
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnClearPresetPos( WPARAM wParam,LPARAM lParam );

	/**
     * @brief  
     * @param [in] wParam ����
     * @param [out] lParam 
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnGetPresetNameList( WPARAM wParam,LPARAM lParam );

	/**
     * @brief  
     * @param [in] wParam 
     * @param [in] lParam ����
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnSetHScanLeftPos( WPARAM wParam,LPARAM lParam );

    /**
     * @brief  
     * @param [in] wParam ����
     * @param [out] lParam 
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnGetHScanLeftPos( WPARAM wParam,LPARAM lParam );

	/**
     * @brief  
     * @param [in] wParam 
     * @param [in] lParam ����
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnSetHScanRightPos( WPARAM wParam,LPARAM lParam );

    /**
     * @brief  
     * @param [in] wParam ����
     * @param [out] lParam 
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnGetHScanRightPos( WPARAM wParam,LPARAM lParam );

	/**
     * @brief  
     * @param [in] wParam 
     * @param [in] lParam ����
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnSetHScanParam( WPARAM wParam,LPARAM lParam );

    /**
     * @brief  
     * @param [in] wParam ����
     * @param [out] lParam 
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnGetHScanParam( WPARAM wParam,LPARAM lParam );

	/**
     * @brief  
     * @param [in] wParam 
     * @param [in] lParam ����
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnCallHScan( WPARAM wParam,LPARAM lParam );

    /**
     * @brief  
     * @param [in] wParam ����
     * @param [out] lParam 
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnStopHScan( WPARAM wParam,LPARAM lParam );

	/**
     * @brief  
     * @param [in] wParam ����
     * @param [out] lParam 
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnClearHScan( WPARAM wParam,LPARAM lParam );

	/**
     * @brief  
     * @param [in] wParam
     * @param [in] lParam ����
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnStartFScanRecord( WPARAM wParam,LPARAM lParam );

    /**
     * @brief  
     * @param [in] wParam ����
     * @param [out] lParam 
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnStopFScanRecord( WPARAM wParam,LPARAM lParam );

	/**
     * @brief  
     * @param [in] wParam 
     * @param [in] lParam ����
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnCallFScan( WPARAM wParam,LPARAM lParam );

	/**
     * @brief  
     * @param [in] wParam 
     * @param [in] lParam ����
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnStopFScan( WPARAM wParam,LPARAM lParam );

    /**
     * @brief  
     * @param [in] wParam ����
     * @param [out] lParam 
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnClearFScan( WPARAM wParam,LPARAM lParam );

	/**
     * @brief  
     * @param [in] wParam 
     * @param [in] lParam ����
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnSetFScanParam( WPARAM wParam,LPARAM lParam );

    /**
     * @brief  
     * @param [in] wParam ����
     * @param [out] lParam 
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnGetFScanParam( WPARAM wParam,LPARAM lParam );

	/**
     * @brief  
     * @param [in] wParam ����
     * @param [out] lParam 
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnGetFScanNameList( WPARAM wParam,LPARAM lParam );

	/**
     * @brief  
     * @param [in] wParam 
     * @param [in] lParam ����
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnSetCruise( WPARAM wParam,LPARAM lParam );

    /**
     * @brief 
     * @param [in] wParam ����
     * @param [out] lParam 
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnGetCruise( WPARAM wParam,LPARAM lParam );

	/**
     * @brief  
     * @param [in] wParam 
     * @param [in] lParam ����
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnCallCruise( WPARAM wParam,LPARAM lParam );

	/**
     * @brief  
     * @param [in] wParam 
     * @param [in] lParam ����
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnStopCruise( WPARAM wParam,LPARAM lParam );

    /**
     * @brief  
     * @param [in] wParam ����
     * @param [out] lParam 
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnClearCruise( WPARAM wParam,LPARAM lParam );


	/**
     * @brief  
     * @param [in] wParam ����
     * @param [out] lParam 
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnSetWatchKeeping( WPARAM wParam,LPARAM lParam );


	/**
     * @brief  
     * @param [in] wParam 
     * @param [in] lParam ����
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnGetWatchKeeping( WPARAM wParam,LPARAM lParam );

    /**
     * @brief  
     * @param [in] wParam ����
     * @param [out] lParam 
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnClearWatchKeeping( WPARAM wParam,LPARAM lParam );

	/**
     * @brief  
     * @param [in] wParam 
     * @param [in] lParam ����
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnStartWatchKeeping( WPARAM wParam,LPARAM lParam );

    /**
     * @brief  
     * @param [in] wParam ����
     * @param [out] lParam 
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnStopWatchKeeping( WPARAM wParam,LPARAM lParam );


	/**
     * @brief  ������˽����
     * @param [in] wParam ����
     * @param [out] lParam 
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnSetMask( WPARAM wParam,LPARAM lParam );


	/**
     * @brief  ��ȡ��˽����
     * @param [in] wParam 
     * @param [in] lParam ����
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnGetMask( WPARAM wParam,LPARAM lParam );

    /**
     * @brief  �����˽����
     * @param [in] wParam ����
     * @param [out] lParam 
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnClearMask( WPARAM wParam,LPARAM lParam );

	/**
     * @brief ��ʾ��˽���������
     * @param [in] wParam ����
     * @param [out] lParam 
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnShowMask( WPARAM wParam,LPARAM lParam );

	/**
     * @brief  ������˽���������
     * @param [in] wParam ����
     * @param [out] lParam 
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnHideMask( WPARAM wParam,LPARAM lParam );

	/**
     * @brief  
     * @param [in] wParam 
     * @param [in] lParam ����
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnSetMaskFlag( WPARAM wParam,LPARAM lParam );

    /**
     * @brief 
     * @param [in] wParam ����
     * @param [out] lParam 
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnGetMaskFlag( WPARAM wParam,LPARAM lParam );

	/**
     * @brief 
     * @param [in] wParam ����
     * @param [out] lParam 
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnSetTimer( WPARAM wParam,LPARAM lParam );


	/**
     * @brief  
     * @param [in] wParam 
     * @param [in] lParam ����
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnGetTimer( WPARAM wParam,LPARAM lParam );

    /**
     * @brief  
     * @param [in] wParam ����
     * @param [out] lParam 
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnClearTimer( WPARAM wParam,LPARAM lParam );

	/**
     * @brief  
     * @param [in] wParam
     * @param [in] lParam ����
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnSetTimerFlag( WPARAM wParam,LPARAM lParam );

    /**
     * @brief  
     * @param [in] wParam ����
     * @param [out] lParam 
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnGetTimerFlag( WPARAM wParam,LPARAM lParam );

	/**
     * @brief  
     * @param [in] wParam 
     * @param [in] lParam ����
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnSetPictureFreeze( WPARAM wParam,LPARAM lParam );

    /**
     * @brief  
     * @param [in] wParam ����
     * @param [out] lParam 
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnGetPictureFreeze( WPARAM wParam,LPARAM lParam );

	/**
     * @brief  
     * @param [in] wParam ����
     * @param [out] lParam ����
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnSavePictureFreeze( WPARAM wParam,LPARAM lParam );

	/**
     * @brief  
     * @param [in] wParam 
     * @param [in] lParam ����
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnSetStandbyFlag( WPARAM wParam,LPARAM lParam );

    /**
     * @brief  
     * @param [in] wParam ����
     * @param [out] lParam 
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnGetStandbyFlag( WPARAM wParam,LPARAM lParam );

    /**
     * @brief  
     * @param [in] wParam ����
     * @param [out] lParam 
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
	HRESULT OnSetIdleTimeToStandby( WPARAM wParam,LPARAM lParam );
	
    /**
     * @brief  
     * @param [in] wParam ����
     * @param [out] lParam 
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
	HRESULT OnGetIdleTimeToStandby( WPARAM wParam,LPARAM lParam );

	
	/**
	 * @brief  
	 * @param [in] wParam ����
	 * @param [out] lParam 
	 * @return �ɹ�����S_OK������ֵ����ʧ��
	 */
	HRESULT OnStartStandbyCountDown( WPARAM wParam,LPARAM lParam );

	/**
	 * @brief  
	 * @param [in] wParam ����
	 * @param [out] lParam 
	 * @return �ɹ�����S_OK������ֵ����ʧ��
	 */
	HRESULT OnGetStandbyStatus( WPARAM wParam,LPARAM lParam );

	/**
     * @brief  
     * @param [in] wParam 
     * @param [in] lParam ����
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnSetXml( WPARAM wParam,LPARAM lParam );

    /**
     * @brief  
     * @param [in] wParam ����
     * @param [out] lParam 
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnGetXml( WPARAM wParam,LPARAM lParam );

	/**
     * @brief  
     * @param [in] wParam ����
     * @param [out] lParam 
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
	HRESULT OnPause( WPARAM wParam,LPARAM lParam );

	/**
     * @brief  
     * @param [in] wParam ����
     * @param [out] lParam 
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
	HRESULT OnResume( WPARAM wParam,LPARAM lParam );
	

	/**
     * @brief ����AEΪʹ��
     * @param [in] wParam 0: �Զ� 0x3:�ֶ���0xa���������ȣ�0xb����Ȧ����
     * @param [in] lParam ����
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnSetAEMode( WPARAM wParam,LPARAM lParam );

    /**
     * @brief ��ȡAE
     * @param [in] wParam ����
     * @param [out] lParam 0: �Զ� 0x3:�ֶ���0xa���������ȣ�0xb����Ȧ����
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnGetAEMode( WPARAM wParam,LPARAM lParam );

	/**
     * @brief ���ÿ��ŵȼ�
     * @param [in] wParam ��λ����
     * 				����         ʱ�䣨�룩
     * 				15		1/10000
					14		1/6000
					13		1/3500
					12		1/2500
					11		1/1750
					10		1/1250
					0F		1/1000
					0E		1/600
					0D		1/425
					0C		1/300
					0B		1/215
					0A		1/150
					09		1/120
					08		1/100
					07		1/75
					06		1/50
					05		1/25
					04		1/12
					03		1/6
					02		1/3
					01		1/2
					00		1/1
     * @param [in] lParam ����
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnSetShutter( WPARAM wParam,LPARAM lParam );

    /**
     * @brief ��ȡ���ŵȼ�
     * @param [in] wParam ����
     * @param [out] lParam ��λ��΢��
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnGetShutter( WPARAM wParam,LPARAM lParam );
	/**
     * @brief ��������ȼ�
     * @param [in] wParam  0 ~ 0xf
     * 				�ȼ�		DB
     * 				0F		+28 dB
					0E		+26 dB
					0D		+24 dB
					0C		+22 dB
					0B		+20 dB
					0A		+18 dB
					09		+16 dB
					08		+14 dB
					07		+12 dB
					06		+10 dB
					05		+8 dB
					04		+6 dB
					03		+4 dB
					02		+2 dB
					01		0 dB
					00		-3 dB
     * @param [in] lParam ����
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnSetAGCGain( WPARAM wParam,LPARAM lParam );

    /**
     * @brief ��ȡ����
     * @param [in] wParam ����
     * @param [out] lParam
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnGetAGCGain( WPARAM wParam,LPARAM lParam );

	/**
     * @brief ����AWBģʽ
     * @param [in] wParam 0~8��0Ϊ�Զ�
     * 			�ȼ�		˵��
     * 			0		�Զ�
     * 			1		ATW(Auto Tracing White balance)
     * 			2		����
     * 			3		����
     * 			4		����һ�ε���(�ðװ��ʱ���õ����ϵ����ڵ�ֵ�Żᶪʧ)
     * 			5		�ֶ�
     * 			6		�Զ�����
     * 			7		�Զ��ɵ�
     * 			8		�ɵ�
     * @param [in] lParam ����
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnSetAWBMode( WPARAM wParam,LPARAM lParam );

    /**
     * @brief ��ȡAWBģʽ
     * @param [in] wParam ����
     * @param [out] lParam 0~8��0Ϊ�Զ�
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnGetAWBMode( WPARAM wParam,LPARAM lParam );

    /**
     * @brief MSG_SET_RGBGAIN ����RGB����
     * @param [in] wParam DWORD rgb[3]
     * @param [in] lParam ����
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnSetRGBGain(WPARAM wParam,LPARAM lParam);

    /**
     * @brief MSG_GET_RGBGAIN ��Ϣ���պ���
     * @param [in] wParam ����
     * @param [out] lParam DWORD rgb[3];
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnGetRGBGain(WPARAM wParam,LPARAM lParam);

    /**
     * @brief MSG_SET_SATURATIONTHRESHOLD,���ñ��Ͷ�
     * @param [in] wParam ���Ͷ�0x0 -0x0e  0:60% ~ E:200%
     * @param [in] lParam ����
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnSetSaturationThreshold(WPARAM wParam,LPARAM lParam);

    /**
     * @brief MSG_GET_SATURATIONTHRESHOLD,��ȡ���Ͷ�
     * @param [in] wParam ����
     * @param [in] lParam ���Ͷ�0x0 -0x0e
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnGetSaturationThreshold(WPARAM wParam,LPARAM lParam);

    /**
     * @brief MSG_SET_SHARPENTHRESHOLD,�����񻯷�ֵ
     * @param [in] wParam �񻯷�ֵ[0~15]
     * @param [in] lParam ����
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnSetSharpeThreshold(WPARAM wParam,LPARAM lParam);

    /**
     * @brief MSG_GET_SHARPENTHRESHOLD,��ȡ�񻯷�ֵ
     * @param [in] wParam  ����
     * @param [in] lParam �񻯷�ֵ[0~15]
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnGetSharpeThreshold(WPARAM wParam,LPARAM lParam);

	/**
     * @brief MSG_SET_SHARPENTHRESHOLD,���ñ�Ե��ǿ
     * @param [in] wParam �񻯷�ֵ[0~15]
     * @param [in] lParam ����
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnSetEdgeEnhance(WPARAM wParam,LPARAM lParam);

    /**
     * @brief MSG_GET_SHARPENTHRESHOLD,��ȡ��Ե��ǿ
     * @param [in] wParam  ����
     * @param [in] lParam �񻯷�ֵ[0~15]
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnGetEdgeEnhance(WPARAM wParam,LPARAM lParam);

    /**
     * @brief ����GAMMAģʽ
     * @param [in] wParam ��0:	Standard,	1	to	4
     * @param [in] lParam ����
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnSetGammaMode(WPARAM wParam,LPARAM lParam);

    /**
     * @brief ��ȡGAMMAģʽ
     * @param [in] wParam  ����
     * @param [in] lParam ��0:	Standard,	1	to	4
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnGetGammaMode(WPARAM wParam,LPARAM lParam);

    /**
     * @brief ����WDR
     * @param [in] wParam ��1:��,	0����
     * @param [in] lParam ����
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnSetWDRStrength(WPARAM wParam,LPARAM lParam);

    /**
     * @brief ��ȡWDR
     * @param [in] wParam  ����
     * @param [in] lParam ��1:��,	0����
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnGetWDRStrength(WPARAM wParam,LPARAM lParam);

    /**
     * @brief ���ý���ȼ�
     * @param [in] wParam ��0:	OFF,	level	1	to	5
     * @param [in] lParam ����
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnSetNRLevel(WPARAM wParam,LPARAM lParam);

    /**
     * @brief ��ȡ����ȼ�
     * @param [in] wParam  ����
     * @param [in] lParam ��0:	OFF,	level	1	to	5
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnGetNRLevel(WPARAM wParam,LPARAM lParam);

    /**
     * @brief �ָ�����Ĭ��
     * @param [in] wParam  ����
     * @param [in] lParam  ����
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnResetDefaultDomeCamera(WPARAM wParam,LPARAM lParam);

	
	/**
	 * @brief �ָ���������
	 * @param [in] wParam  ����
	 * @param [in] lParam  ����
	 * @return �ɹ�����S_OK������ֵ����ʧ��
	 */
	HRESULT OnRestoreFactorySetting(WPARAM wParam,LPARAM lParam);

	/**
     * @brief ��ȡ��ǰ���е�����
     * @param [in] wParam  ����
     * @param [in] lParam  ����
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnGetCurTask(WPARAM wParam,LPARAM lParam);

	/**
     * @brief ���¶�����Ϣ���������ģ��
     * @param [in] wParam  ����
     * @param [in] lParam  ����
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
	HRESULT OnTouchDomeCamera(WPARAM wParam,LPARAM lParam);

	/**
     * @brief ���ó���
     * @param [in] wParam  ����
     * @param [in] lParam  ����
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
	HRESULT OnSetDefog(WPARAM wParam,LPARAM lParam);

	/**
     * @brief �����عⲹ��
     * @param [in] wParam ������ֵ: 0x0 ~ 0xE
     *      0E | +7 | +10.5 dB
     *      0D | +6 | +9 dB
     *      0C | +5 | +7.5 dB
     *      0B | +4 | +6 dB
     *      0A | +3 | +4.5 dB
     *      09 | +2 | +3 dB
     *      08 | +1 | +1.5 dB
     *      07 |  0  |  0 dB
     *      06 | -1 | -1.5 dB
     *      05 | -2 | -3 dB
     *      04 | -3 | -4.5 dB
     *      03 | -4 | -6 dB
     *      02 | -5 | -7.5 dB
     *      01 | -6 | -9 dB
     *      00 | -7 | -10.5 dB
     * @param [in] lParam ����
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnSetExpoCompValue(WPARAM wParam,LPARAM lParam);

    /**
     * @brief ��ȡ�عⲹ��
     * @param [in] wParam  ����
     * @param [out] lParam ������ֵ: 0x0 ~ 0xE
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnGetExpoCompValue(WPARAM wParam,LPARAM lParam);

	/**
     * @brief ִ�������о����
     * @param [in] wParam  : �����ֽ�
     * @param [in] lParam �������ֽ���
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
	HRESULT OnCameraDoCmd(WPARAM wParam,LPARAM lParam);


	/**
	 @brief ��ȡ����̼��汾
	 @param [in] wParam : ����
	 @param [out] lParam : bit 0-7 �Ͱ汾�� 8-15�߰汾��
	 @return �ɹ�����S_OK������ֵ����ʧ��
	 */
	HRESULT OnGetDomeVersion(WPARAM wParam, LPARAM lParam);

	/**
	 @brief ��ȡ��о�汾, lParam�����6���ֽ�
	 @param [in] wParam : ����
	 @param [out] lParam : byte0-1:VenderID byte2-3:ModelID byte4-5:ROM version
	 @return �ɹ�����S_OK������ֵ����ʧ��
	 */
	HRESULT OnGetCameraVersion(WPARAM wParam, LPARAM lParam);

	/**
	 @brief ��ȡ��о�ͺ�
	 @param [in] wParam : ����
	 @param [out] lParam : �ͺ�ö��ֵ
	 @return �ɹ�����S_OK������ֵ����ʧ��
	 */
	HRESULT OnGetCameraModel(WPARAM wParam, LPARAM lParam);


     //��Ϣӳ��궨��
    SW_BEGIN_MESSAGE_MAP(CSWDomeCameraControlMSG, CSWMessage)
        //��ʼ��ģ��
        SW_MESSAGE_HANDLER(MSG_INITIAL_DOMECAMERA, OnInitialDomeCamera)
        //�ָ�����Ĭ��
        SW_MESSAGE_HANDLER(MSG_RESETDEFAULT_DOMECAMERA, OnResetDefaultDomeCamera)
        //�ָ���������
        SW_MESSAGE_HANDLER(MSG_RESTOREFACTORY_DOMECAMERA, OnRestoreFactorySetting)
        
		//basic
        SW_MESSAGE_HANDLER(MSG_SET_PAN_COORDINATE , OnSetPanCoordinate)
        SW_MESSAGE_HANDLER(MSG_GET_PAN_COORDINATE , OnGetPanCoordinate)

        SW_MESSAGE_HANDLER(MSG_SET_TILT_COORDINATE , OnSetTiltCoordinate)
        SW_MESSAGE_HANDLER(MSG_GET_TILT_COORDINATE , OnGetTiltCoordinate)

        SW_MESSAGE_HANDLER(MSG_SET_PT_COORDINATE , OnSetPanTiltCoordinate)
        SW_MESSAGE_HANDLER(MSG_GET_PT_COORDINATE , OnGetPanTiltCoordinate)
        
        SW_MESSAGE_HANDLER(MSG_RESET_DOME_POSITION , OnResetDomePosition)

        //PTZ
		SW_MESSAGE_HANDLER(MSG_SET_ZOOM , OnSetZoom)
        SW_MESSAGE_HANDLER(MSG_GET_ZOOM , OnGetZoom)
        SW_MESSAGE_HANDLER(MSG_START_PAN , OnStartPan )
        SW_MESSAGE_HANDLER(MSG_STOP_PAN , OnStopPan )
        SW_MESSAGE_HANDLER(MSG_START_ZOOM , OnStartZoom )
        SW_MESSAGE_HANDLER(MSG_STOP_ZOOM , OnStopZoom )
        SW_MESSAGE_HANDLER(MSG_START_TILT , OnStartTilt )
        SW_MESSAGE_HANDLER(MSG_STOP_TILT , OnStopTilt )
        SW_MESSAGE_HANDLER(MSG_START_PAN_TILT , OnStartPanTilt )
        SW_MESSAGE_HANDLER(MSG_STOP_PAN_TILT , OnStopPanTilt )
        SW_MESSAGE_HANDLER(MSG_SET_PAN_SPEED , OnSetPanSpeed )
        SW_MESSAGE_HANDLER(MSG_GET_PAN_SPEED , OnGetPanSpeed )
        SW_MESSAGE_HANDLER(MSG_SET_MAX_PAN_SPEED , OnSetMaxPanSpeed )
        SW_MESSAGE_HANDLER(MSG_GET_MAX_PAN_SPEED , OnGetMaxPanSpeed )
        SW_MESSAGE_HANDLER(MSG_SET_TILT_SPEED , OnSetTiltSpeed )
	 	SW_MESSAGE_HANDLER(MSG_GET_TILT_SPEED , OnGetTiltSpeed )
        SW_MESSAGE_HANDLER(MSG_SET_MAX_TILT_SPEED , OnSetMaxTiltSpeed )
        SW_MESSAGE_HANDLER(MSG_GET_MAX_TILT_SPEED , OnGetMaxTiltSpeed )		
		SW_MESSAGE_HANDLER(MSG_START_INFINITY_PAN , OnStartInfinityPan )
		SW_MESSAGE_HANDLER(MSG_STOP_INFINITY_PAN , OnStopInfinityPan )
	 
		
        // �۽�
        SW_MESSAGE_HANDLER(MSG_SET_FOCUS , OnSetFocus )
        SW_MESSAGE_HANDLER(MSG_GET_FOCUS , OnGetFocus )
        SW_MESSAGE_HANDLER(MSG_SET_FOCUS_MODE , OnSetFocusMode )
        SW_MESSAGE_HANDLER(MSG_GET_FOCUS_MODE , OnGetFocusMode )
        SW_MESSAGE_HANDLER(MSG_SET_AF_ENABLE , OnSetAFEnable )
        SW_MESSAGE_HANDLER(MSG_GET_AF_ENABLE , OnGetAFEnable )
        SW_MESSAGE_HANDLER(MSG_DO_ONE_FOCUS , OnDoOneFocus )
        // ��Ȧ
        SW_MESSAGE_HANDLER(MSG_SET_IRIS , OnSetIris )
        SW_MESSAGE_HANDLER(MSG_GET_IRIS , OnGetIris )
        SW_MESSAGE_HANDLER(MSG_SET_IRIS_MODE,OnSetIrisMode)
        SW_MESSAGE_HANDLER(MSG_GET_IRIS_MODE,OnGetIrisMode)

		SW_MESSAGE_HANDLER(MSG_TUNE_IRIS,OnTuneIris)
        SW_MESSAGE_HANDLER(MSG_TUNE_ZOOM,OnTuneZoom)
        SW_MESSAGE_HANDLER(MSG_TUNE_FOCUS,OnTuneFocus)
	 
		SW_MESSAGE_HANDLER(MSG_START_VTURNOVER,OnStartVTurnOver)
        SW_MESSAGE_HANDLER(MSG_STOP_VTURNOVER,OnStopVTurnOver)

        // ��ˢ
        SW_MESSAGE_HANDLER(MSG_START_WIPER, OnStartWiper)
        SW_MESSAGE_HANDLER(MSG_STOP_WIPER, OnStopWiper)
        SW_MESSAGE_HANDLER(MSG_SET_WIPER_SPEED, OnSetWiperSpeed)
        SW_MESSAGE_HANDLER(MSG_GET_WIPER_SPEED, OnGetWiperSpeed)
		SW_MESSAGE_HANDLER(MSG_SET_WIPER_AUTOSTART_TIME, OnSetWiperAutoStartTime)
        SW_MESSAGE_HANDLER(MSG_GET_WIPER_AUTOSTART_TIME, OnGetWiperAutoStartTime)

        // LED��
        SW_MESSAGE_HANDLER(MSG_SET_LED_MODE, OnSetLedMode)
        SW_MESSAGE_HANDLER(MSG_GET_LED_MODE, OnGetLedMode)
        SW_MESSAGE_HANDLER(MSG_SET_LED_POWER, OnSetLedPower)
        SW_MESSAGE_HANDLER(MSG_GET_LED_POWER, OnGetLedPower)

		//preset
		SW_MESSAGE_HANDLER(MSG_SET_PRESET_POS, OnSetPresetPos)
        SW_MESSAGE_HANDLER(MSG_GET_PRESET_POS, OnGetPresetPos)
		SW_MESSAGE_HANDLER(MSG_CALL_PRESET_POS, OnCallPresetPos)
        SW_MESSAGE_HANDLER(MSG_CLEAR_PRESET_POS, OnClearPresetPos)
        SW_MESSAGE_HANDLER(MSG_GET_PRESET_NAME_LIST, OnGetPresetNameList)

		//hscan
        SW_MESSAGE_HANDLER(MSG_SET_HSCAN_LEFT_POS, OnSetHScanLeftPos)
        SW_MESSAGE_HANDLER(MSG_GET_HSCAN_LEFT_POS, OnGetHScanLeftPos)
        SW_MESSAGE_HANDLER(MSG_SET_HSCAN_RIGHT_POS, OnSetHScanRightPos)
        SW_MESSAGE_HANDLER(MSG_GET_HSCAN_RIGHT_POS, OnGetHScanRightPos)
        SW_MESSAGE_HANDLER(MSG_SET_HSCAN_PARAM, OnSetHScanParam)
        SW_MESSAGE_HANDLER(MSG_GET_HSCAN_PARAM, OnGetHScanParam)
        SW_MESSAGE_HANDLER(MSG_START_HSCAN, OnCallHScan)
        SW_MESSAGE_HANDLER(MSG_STOP_HSCAN, OnStopHScan)
        SW_MESSAGE_HANDLER(MSG_CLEAR_HSCAN, OnClearHScan)

		// fscan
		SW_MESSAGE_HANDLER(MSG_START_FSCAN_RECORD , OnStartFScanRecord)
        SW_MESSAGE_HANDLER(MSG_STOP_FSCAN_RECORD , OnStopFScanRecord)
        SW_MESSAGE_HANDLER(MSG_CALL_FSCAN , OnCallFScan)
        SW_MESSAGE_HANDLER(MSG_STOP_FSCAN , OnStopFScan)
        SW_MESSAGE_HANDLER(MSG_CLEAR_FSCAN , OnClearFScan)
        SW_MESSAGE_HANDLER(MSG_SET_FSCAN_PARAM , OnSetFScanParam)
        SW_MESSAGE_HANDLER(MSG_GET_FSCAN_PARAM , OnGetFScanParam)
        SW_MESSAGE_HANDLER(MSG_GET_FSCAN_NAME_LIST, OnGetFScanNameList)

	 	//cruise
        SW_MESSAGE_HANDLER(MSG_SET_CRUISE , OnSetCruise)
        SW_MESSAGE_HANDLER(MSG_GET_CRUISE , OnGetCruise)
        SW_MESSAGE_HANDLER(MSG_CALL_CRUISE , OnCallCruise)
        SW_MESSAGE_HANDLER(MSG_STOP_CRUISE , OnStopCruise)
        SW_MESSAGE_HANDLER(MSG_CLEAR_CRUISE , OnClearCruise)

        //watchkeeping
        SW_MESSAGE_HANDLER(MSG_SET_WATCHKEEPING , OnSetWatchKeeping)
        SW_MESSAGE_HANDLER(MSG_GET_WATCHKEEPING , OnGetWatchKeeping)
        SW_MESSAGE_HANDLER(MSG_CLEAR_WATCHKEEPING , OnClearWatchKeeping)
        SW_MESSAGE_HANDLER(MSG_START_WATCHKEEPING , OnStartWatchKeeping)
        SW_MESSAGE_HANDLER(MSG_STOP_WATCHKEEPING , OnStopWatchKeeping)

		//mask
        SW_MESSAGE_HANDLER(MSG_SET_MASK , OnSetMask)
        SW_MESSAGE_HANDLER(MSG_GET_MASK , OnGetMask)
        SW_MESSAGE_HANDLER(MSG_CLEAR_MASK , OnClearMask)
        SW_MESSAGE_HANDLER(MSG_SHOW_MASK , OnShowMask)//todo: msgid
        SW_MESSAGE_HANDLER(MSG_HIDE_MASK , OnHideMask)//todo: msgid


		//timer
        SW_MESSAGE_HANDLER(MSG_SET_TIMER , OnSetTimer)
        SW_MESSAGE_HANDLER(MSG_GET_TIMER , OnGetTimer)
        SW_MESSAGE_HANDLER(MSG_CLEAR_TIMER , OnClearTimer)
        SW_MESSAGE_HANDLER(MSG_SET_TIMER_FLAG , OnSetTimerFlag)
        SW_MESSAGE_HANDLER(MSG_GET_TIMER_FLAG , OnGetTimerFlag)


		//picturefreeze
        SW_MESSAGE_HANDLER(MSG_SET_FREEZEFLAG , OnSetPictureFreeze)
        SW_MESSAGE_HANDLER(MSG_GET_FREEZEFLAG , OnGetPictureFreeze)
		SW_MESSAGE_HANDLER(MSG_SAVE_FREEZEFLAG, OnSavePictureFreeze)

	 	//sleep
        SW_MESSAGE_HANDLER(MSG_SET_SLEEP_FLAG , OnSetStandbyFlag)
        SW_MESSAGE_HANDLER(MSG_GET_SLEEP_FLAG , OnGetStandbyFlag)
        SW_MESSAGE_HANDLER(MSG_SET_IDLE_TIME_TO_SLEEP, OnSetIdleTimeToStandby)
        SW_MESSAGE_HANDLER(MSG_GET_IDLE_TIME_TO_SLEEP , OnGetIdleTimeToStandby)
        SW_MESSAGE_HANDLER(MSG_START_SLEEP_COUNTDOWN, OnStartStandbyCountDown)
        SW_MESSAGE_HANDLER(MSG_GET_SLEEP_STATUS, OnGetStandbyStatus)

		SW_MESSAGE_HANDLER(MSG_MOVE_POINT_TO_CENTER, OnMovePointToCenter)
        SW_MESSAGE_HANDLER(MSG_MOVE_BLOCK_TO_CENTER, OnMoveBlockToCenter)	 
        

		SW_MESSAGE_HANDLER(MSG_SET_DOME_XML, OnSetXml)
        SW_MESSAGE_HANDLER(MSG_GET_DOME_XML , OnGetXml)

		SW_MESSAGE_HANDLER(MSG_PAUSE, OnPause)
        SW_MESSAGE_HANDLER(MSG_RESUME, OnResume)

		SW_MESSAGE_HANDLER(MSG_GET_CUR_TASK, OnGetCurTask)
		SW_MESSAGE_HANDLER(MSG_TOUCH_DOME_CAMERA, OnTouchDomeCamera)
		SW_MESSAGE_HANDLER(MSG_SET_DEFOG, OnSetDefog)
	 	
        // �������
        // AE
        SW_MESSAGE_HANDLER(MSG_SET_AE_MODE , OnSetAEMode )
        SW_MESSAGE_HANDLER(MSG_GET_AE_MODE , OnGetAEMode )
        SW_MESSAGE_HANDLER(MSG_SET_SHUTTER , OnSetShutter )
        SW_MESSAGE_HANDLER(MSG_GET_SHUTTER , OnGetShutter)
        SW_MESSAGE_HANDLER(MSG_SET_AGCGAIN , OnSetAGCGain )
        SW_MESSAGE_HANDLER(MSG_GET_AGCGAIN , OnGetAGCGain )
        // AWB
        SW_MESSAGE_HANDLER(MSG_SET_AWB_MODE , OnSetAWBMode )
        SW_MESSAGE_HANDLER(MSG_GET_AWB_MODE , OnGetAWBMode )
        SW_MESSAGE_HANDLER(MSG_SET_RGBGAIN , OnSetRGBGain )
        SW_MESSAGE_HANDLER(MSG_GET_RGBGAIN , OnGetRGBGain )
        // ���Ͷ�
		SW_MESSAGE_HANDLER(MSG_SET_SATURATIONTHRESHOLD, OnSetSaturationThreshold)
        SW_MESSAGE_HANDLER(MSG_GET_SATURATIONTHRESHOLD, OnGetSaturationThreshold)
        // ����ֵ
		//SW_MESSAGE_HANDLER(MSG_SET_SHARPENTHRESHOLD,OnSetSharpeThreshold)
        //SW_MESSAGE_HANDLER(MSG_GET_SHARPENTHRESHOLD,OnGetSharpeThreshold)
        SW_MESSAGE_HANDLER(MSG_SET_SHARPENTHRESHOLD,OnSetEdgeEnhance)
        SW_MESSAGE_HANDLER(MSG_GET_SHARPENTHRESHOLD,OnGetEdgeEnhance)
        // ����GAMMAģʽ  0:	Standard,	1	to	4
		SW_MESSAGE_HANDLER(MSG_SET_GAMMA_STRENGTH,OnSetGammaMode)
        SW_MESSAGE_HANDLER(MSG_GET_GAMMA_STRENGTH,OnGetGammaMode)
        // ����WDR
        SW_MESSAGE_HANDLER(MSG_SET_WDR_STRENGTH, OnSetWDRStrength)
        SW_MESSAGE_HANDLER(MSG_GET_WDR_STRENGTH, OnGetWDRStrength)
        // ���ý���ȼ�  0:	OFF,	level	1	to	5
        SW_MESSAGE_HANDLER(MSG_SET_DENOISE_STATE, OnSetNRLevel)
        SW_MESSAGE_HANDLER(MSG_GET_DENOISE_STATE, OnGetNRLevel)	 

		// �عⲹ��
        SW_MESSAGE_HANDLER(MSG_SET_EXPOCOMP_VALUE, OnSetExpoCompValue)
        SW_MESSAGE_HANDLER(MSG_GET_EXPOCOMP_VALUE, OnGetExpoCompValue)

		// ��ȡ����̼��汾
		SW_MESSAGE_HANDLER(MSG_GET_DOME_VERSION, OnGetDomeVersion)
		// ��ȡ��о�汾��Ϣ
		SW_MESSAGE_HANDLER(MSG_GET_CAMERA_VERSION, OnGetCameraVersion)
		// ��ȡ��о�ͺ�
		SW_MESSAGE_HANDLER(MSG_GET_CAMERA_MODEL, OnGetCameraModel)
		
	 	SW_MESSAGE_HANDLER(MSG_CAMERA_DO_CMD, OnCameraDoCmd)

    SW_END_MESSAGE_MAP();


	/**
     *@brief ���������̨
     *@param [PVOID] pvBuffer PTZ���Ʋ���
     *@param [PVOID] iSize ������С
     *@return �ɹ�����S_OK,����ֵΪ�������
     */
	HRESULT OnPTZControl(PVOID pvBuffer, INT iSize);

	/**
     *@brief ��ȡ�����̨����
     *@param [PVOID] pvBuffer PTZ���Ʋ���
     *@param [PVOID] iSize ������С
     *@return �ɹ�����S_OK,����ֵΪ�������
     */
	HRESULT OnGetPTZInfo(PVOID pvBuffer, INT iSize);


	
	//Զ����Ϣӳ�亯��
    SW_BEGIN_REMOTE_MESSAGE_MAP(CSWDomeCameraControlMSG, CSWMessage)
		SW_REMOTE_MESSAGE_HANDLER(MSG_APP_REMOTE_PTZ_CONTROL, OnPTZControl)
		SW_REMOTE_MESSAGE_HANDLER(MSG_APP_REMOTE_GET_PTZ_INFO, OnGetPTZInfo)
    SW_END_REMOTE_MESSAGE_MAP();
    
private:
    CSWDomeCameraSetting& GetSetting(){ return m_cSetting;};
	VOID Wait(){swpa_thread_sleep_ms(500);};

	HRESULT HandleOperation(const INT& iOperation, const INT& iArg = 0);

	HRESULT SetLastTask(const INT& iTask, const INT& iTaskID);

	HRESULT GetLastTask(INT& iTask, INT& iTaskID);

	HRESULT SwitchTask(const BOOL& fSwitchOn, const INT& iTask = -1, const INT& iArg = -1);

	static PVOID InfinityPan(PVOID pvArg);
	
	static PVOID VTurnOver(PVOID pvArg);

	static PVOID ResetDome(PVOID pvArg);
	
	static PVOID SyncZoom(PVOID pvArg);

	static PVOID Defog(PVOID pvArg);
	
protected:

	BOOL m_fInited;
	DWORD m_dwFScanRecordingID;

	CSWDomeCameraSetting m_cSetting;
	
	CSWDomeCameraPreset m_cPreset;
	CSWDomeCameraHScan m_cHScan;
	CSWDomeCameraFScan m_cFScan;
	CSWDomeCameraCruise m_cCruise;

	CSWDomeCameraPictureFreeze m_cPictureFreeze;
	CSWDomeCameraWatchKeeping m_cWatchKeeping;
	CSWDomeCameraTimer m_cTimer;
	CSWDomeCameraStandby m_cStandby;
	CSWDomeCameraMask m_cMask;

	CSWThread m_cInfinityPanThread;
	BOOL m_fInfinityPanStarted;

	CSWThread m_cDefogThread;
	BOOL m_fDefogStarted;

	
	// ��Щ������Ҫ�ڴ�ģ�鱣�档
	int m_iLedMode;	// LED ģʽ

	BOOL m_fVTurnOver;
	CSWThread m_cVTurnOverThread;
	CSWThread m_cResetDomeThread;

	BOOL m_fSyncZoom;
	CSWThread m_cSyncZoomThread;

	BOOL m_fRepeatPanTilt;

	CSWCamera *m_pCamera;	
public:
	// �����Ӧ��
	// ����佹30����
	static const int MAX_ZOOM_COUNT = 30;
	static ZOOM_RATIO s_cZoomTable[MAX_ZOOM_COUNT];
};
//!!!!!!!!!!!!!!!!!!!!!!!! testonly::::::::::: REGISTER_CLASS(CSWDomeCameraControlMSG)
#endif // _SWDOMECAMERACONTROLMSG_H_

