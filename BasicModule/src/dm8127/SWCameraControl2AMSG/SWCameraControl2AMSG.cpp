
#include "SWFC.h"
#include "SWCameraControl2AMSG.h"
#include "SWPTZParameter.h"

//#define SW_TRACE_DEBUG SW_TRACE_DEBUG
//#define SW_TRACE_DEBUG
//REGISTER_CLASS(CSWCameraControl2AMSG)

CSWCameraControl2AMSG::CSWCameraControl2AMSG()
    : CSWMessage( MSG_CAMERA_CONTROL_START , MSG_CAMERA_CONTROL_END )

{
    m_iManualShutter = 0;
    m_iManualGain    = 0;
    m_iManualGainR   = 0;
    m_iManualGainG   = 0;
    m_iManualGainB   = 0;
	m_dwLEDPolarity = 0;
	m_FilterStatus = 0;
	m_dwLEDOutputType = 0;
	m_dwTriggerOutPolarity = 0;
	m_dwTriggerOutOutputType = 0;
	m_dwAGCScene = 0;
	m_fIsDayNow = TRUE;
	m_IsDay=TRUE;
	swpa_memset(rgWriteRegParam,0,sizeof(DWORD)*2);
}

CSWCameraControl2AMSG::~CSWCameraControl2AMSG()
{

}
#define FAIL_RET( func ) if( S_OK != func ){SW_TRACE_DEBUG("=========ERROR!=========");return E_FAIL;}
/**
* @brief MSG_INITIAL_PARAM �������������ʼ�������ṹ��
* @param [in] wParam ��CamAppParam*�ṹָ�����ͣ�
* @param [in] lParam ����
* @return �ɹ�����S_OK������ֵ����ʧ��
*/
HRESULT CSWCameraControl2AMSG::OnInitialCameralParam( WPARAM wParam,LPARAM lParam )
{
    LPCAMERAPARAM_INFO pCameraParam = (LPCAMERAPARAM_INFO)wParam;

	SW_TRACE_DEBUG("%s %d pCameraParam = 0x%x\n",__FUNCTION__,__LINE__,pCameraParam);
    if( pCameraParam == NULL )
        return S_FALSE;

//    // ��ʱ����
//    {
//        DWORD dwParam[5];
//        //�ֳ�������Ҫͬʱ����������
//        dwParam[0] = pCameraParam->iFlashEnable;
//        dwParam[1] = pCameraParam->iFlashCoupling;
//        dwParam[2] = pCameraParam->iFlashOutputType;
//        dwParam[3] = pCameraParam->iFlashPolarity;
//        dwParam[4] = pCameraParam->iFlashPluseWidth;
//    //	FAIL_RET(this->OnSetF1TriggerOut((WPARAM)dwParam, 0));
//    //	FAIL_RET(this->OnSetAlarmTriggerOut((WPARAM)dwParam, 0));

//        SW_TRACE_DEBUG("SetF1 Alarm Params : <FlashEnable:%d, FlashCoupling:%d, FlashOutputType:%d, FlashPolarity:%d, FlashPluseWidth:%d, F1OutputType:%d>.\n",
//                       pCameraParam->iFlashEnable, pCameraParam->iFlashCoupling, pCameraParam->iFlashOutputType,
//                       pCameraParam->iFlashPolarity, pCameraParam->iFlashPluseWidth, pCameraParam->iF1OutputType);

//        OnSetF1TriggerOut((WPARAM)dwParam, 0);
//        OnSetAlarmTriggerOut((WPARAM)dwParam, 0);
//    }

    // ���� 928  Ϊ  2�� ���ƺ����ƽ���ƽ��
    DWORD dwRegValueAWB[2] = {0};
    dwRegValueAWB[0] = 0xa98;
    dwRegValueAWB[1] = 2;
    CSWMessage::SendMessage(MSG_SET_CAM_FPGA_REG, (WPARAM)dwRegValueAWB, NULL);

	//����ģʽ
	FAIL_RET(this->OnSetWorkMode((WPARAM)pCameraParam->iCameraWorkMode, 0));
	//��ƽ��ģʽ
	FAIL_RET(this->OnSetAWBMode((WPARAM)pCameraParam->iAWBMode, 0));

    // ����AGC��ʹ��
    FAIL_RET(this->OnSetAGCEnable( pCameraParam->iAGCEnable , 0 ));
    if(!pCameraParam->iAGCEnable)
    {
        // �������
        FAIL_RET(this->OnSetShutter( pCameraParam->iShutter , 0 ));
		// �������
        FAIL_RET(this->OnSetAGCGain( pCameraParam->iGain , 0 ));
    }
    // ����AWBʹ��
    FAIL_RET(this->OnSetAWBEnable( pCameraParam->iAWBEnable, 0));
    if(!pCameraParam->iAWBEnable)
    {
        int rgiValue[3] = {0};
        rgiValue[0] = pCameraParam->iGainR;
        rgiValue[1] = pCameraParam->iGainG;
        rgiValue[2] = pCameraParam->iGainB;
        //����R����,G����,B����
        FAIL_RET(this->OnSetRGBGain( (WPARAM)rgiValue , 0 ));
    }
    // ����AGC������ֵ
    FAIL_RET(this->OnSetAGCTh( pCameraParam->iAGCTh , 0 ));
    DWORD nValue= 0;
    //����AGC���ŷ�Χ
    nValue = (pCameraParam->iAGCShutterHOri << 16);
    nValue |= (pCameraParam->iAGCShutterLOri & 0x0000FFFF);
    FAIL_RET(this->OnSetAGCShutterRange( nValue , 0 ));

    // ����AGC���淶Χ
    nValue = (pCameraParam->iAGCGainHOri << 16);
    nValue |= (pCameraParam->iAGCGainLOri & 0x0000FFFF);
    FAIL_RET(this->OnSetAGCGainRange( nValue , 0 ));
	//
	FAIL_RET(this->OnSetAGCScene(pCameraParam->iAGCScene, 0));

    FAIL_RET(this->OnSetAGCZone((WPARAM)pCameraParam->rgiAGCZone, 0 ));

    //����WDRǿ��ֵ

    FAIL_RET(this->OnSetWDRStrength(pCameraParam->iWDRStrength , 0));
    // ������ֵ
    //    FAIL_RET(this->OnSetSharpeEnable(pCameraParam->iEnableSharpen , 0));
    //    FAIL_RET(this->OnSetSharpeThreshold( pCameraParam->iSharpenThreshold , 0));

    // ����ͼ����ǿʹ��
    FAIL_RET(this->OnSetColoMaxtrixEnable( pCameraParam->fColorMatrixEnable , 0));

    // todo.
    // Ŀǰ���Ͷ���AE�Զ����ã���AEʹ��ʱ,��ʼ��ʱ���������ô�ֵ�������ͻ��
    if(!pCameraParam->iAGCEnable)
    {		
		// ���� ���Ͷ�
		FAIL_RET(this->OnSetSaturationThreshold( pCameraParam->iSaturationValue , 0));
    }
    // ���öԱȶ�
	FAIL_RET(this->OnSetContrastThreshold( pCameraParam->iContrastValue , 0));

    //���ý���TNFʹ��
    FAIL_RET(this->OnSetTNFEnable( pCameraParam->fTNFEnable , 0));
    //���ý���SNFʹ��
    FAIL_RET(this->OnSetSNFEnable( pCameraParam->fSNFEnable , 0));
    //���ý���ǿ��ֵ
    FAIL_RET(this->OnSetDeNoiseState( pCameraParam->iDenoiseState , 0));

//    //���ú�ƼӺ�ʹ��
//    FAIL_RET(this->OnSetRedLightEnable( pCameraParam->fRedLightEnable , 0));

//    FAIL_RET(this->OnSetRedLightThreshold( (WPARAM)pCameraParam->iLumLThreshold , 0));

//	//�����̵Ƽ���
//	//2014-05-06: Ŀǰ�̵Ƽ������ƼӺ칲��ͬһ��ʹ�ܱ�־
//	//��ʹ�ܺ�ƼӺ���ͬʱʹ���̵Ƽ��̣���"��ͨ����ɫ��ǿ"������һЩ
//	FAIL_RET(this->OnSetGreenLightThreshold( (WPARAM)pCameraParam->iLumLThreshold , 0));

//    // ����DC��Ȧʹ��
//    FAIL_RET(this->OnSetDCApertureEnable( pCameraParam->iEnableDCAperture , 0));
    //���õ���ͬ��ʹ��
    FAIL_RET(this->OnSetACSyncEnable( pCameraParam->iExternalSyncMode ,0));
    //���õ���ͬ����ʱ
    FAIL_RET(this->OnSetACSyncDelay( pCameraParam->iSyncRelay,0 ));
    //�����˹�Ƭ�л�
    // todo. ��ʼ�����ǲ�ʹ�ܡ�
    //FAIL_RET(this->OnSetFilterSwitch( 0, 0));//������С���ʿ�ס������羯���ڷ�������Ҫ�˹�Ƭ��������ʱֱ������
    //���úڰ�ͼʹ��
    FAIL_RET(this->OnSetGrayImageEnable( pCameraParam->fGrayImageEnable , 0));

//    //����CVBS��ʽ
//    FAIL_RET(this->OnSetCVBSStd( pCameraParam->iCVBSMode , 0));

    // ȥ��GAMMA�Ĺ������á�
    //FAIL_RET(this->OnSetGammaStrength( pCameraParam->iGammaValue , 0));
    //FAIL_RET(this->OnSetGammaEnable( pCameraParam->iGammaEnable , 0));
    FAIL_RET(this->OnSetEdgeEnhance( pCameraParam->iEdgeValue, 0));

    // �ȹرյ�
    OnSetLEDSwitch(0, 0);

	//����ƿ��� EXP IO
    DWORD dwParam[5];	//polarity output_type
	dwParam[0] = pCameraParam->iLEDPolarity; //������������
	dwParam[1] =pCameraParam->iLEDOutputType;	//������������
	FAIL_RET(this->OnSetEXPIO((WPARAM)dwParam, 0));
	m_dwLEDPluseWidth = pCameraParam->iLEDPluseWidth;
	this->OnSetEXPPluseWidth(pCameraParam->iLEDPluseWidth, 0);

	//����ץ�� TG IO
	FAIL_RET(this->OnSetTGIO(pCameraParam->iCaptureEdgeOne,0));

	//�������	F1 IO
	// dwParam[0] = pCameraParam->nTriggerOutNormalStatus;	//�����������
	// dwParam[1] =pCameraParam->nCaptureSynOutputType;	//�����������

	//�ֳ�������Ҫͬʱ����������
	dwParam[0] = pCameraParam->iFlashEnable;
	dwParam[1] = pCameraParam->iFlashCoupling;
	dwParam[2] = pCameraParam->iFlashOutputType;
	dwParam[3] = pCameraParam->iFlashPolarity;
	dwParam[4] = pCameraParam->iFlashPluseWidth;
//	FAIL_RET(this->OnSetF1TriggerOut((WPARAM)dwParam, 0));
//	FAIL_RET(this->OnSetAlarmTriggerOut((WPARAM)dwParam, 0));

    SW_TRACE_DEBUG("SetF1 Alarm Params : <FlashEnable:%d, FlashCoupling:%d, FlashOutputType:%d, FlashPolarity:%d, FlashPluseWidth:%d, F1OutputType:%d>.\n",
                   pCameraParam->iFlashEnable, pCameraParam->iFlashCoupling, pCameraParam->iFlashOutputType,
                   pCameraParam->iFlashPolarity, pCameraParam->iFlashPluseWidth, pCameraParam->iF1OutputType);

	//���õ�·����ʹ��
    OnSetF1TriggerOut((WPARAM)dwParam, 0);
	
    OnSetAlarmTriggerOut((WPARAM)dwParam, 0);	
	
	// FAIL_RET(this->OnSetTriggerOut((WPARAM)dwParam,0));

	//������� ALM IO
	// dwParam[0] = pCameraParam->iALMPolarity; //�����������
	// dwParam[1] =pCameraParam->iALMOutputType;	//�����������
	// FAIL_RET(this->OnSetALMIO((WPARAM)dwParam,0));

//    //F1�������
//    FAIL_RET(this->OnSetF1OutputType((WPARAM)pCameraParam->iF1OutputType, 0));
     SW_TRACE_DEBUG("SetF1 OutputType.\n");
    OnSetF1OutputType((WPARAM)pCameraParam->iF1OutputType, 0);
	
	// ɫ��
	FAIL_RET(this->OnSetColorGradation((WPARAM)pCameraParam->iColorGradation, 0));

	m_nCaptureR = pCameraParam->iCaptureGainR;
	m_nCaptureG = pCameraParam->iCaptureGainG;
	m_nCaptureB = pCameraParam->iCaptureGainB;
	m_nCaptureShutter = pCameraParam->iCaptureShutter;
	m_nCaptureGain = pCameraParam->iCaptureGain;
	m_nCapSharpen = pCameraParam->iCaptureSharpenThreshold;
	m_fCaptureAutoParamEnable = pCameraParam->iCaptureAutoParamEnable;
	m_fEnableCaptureRGBGain = pCameraParam->iCaptureRGBEnable;
	m_fEnableCaptureShutter = pCameraParam->iCaptureShutterEnable;
	m_fEnableCaptureGain = pCameraParam->iCaptureGainEnable;
	//m_fEnableCaptureSharpen = pCameraParam->iCaptureSharpenEnable;

    // OnSetCaptureRGBEnable(pCameraParam->iCaptureRGBEnable, 0xF0F0F0F0);
//     OnSetCaptureShutterEnable(pCameraParam->iCaptureShutterEnable, 0xF0F0F0F0);
//     OnSetCaptureGainEnable(pCameraParam->iCaptureGainEnable, 0xF0F0F0F0);
    //OnSetCaptureSharpenEnable(pCameraParam->iCaptureSharpenEnable, 0xF0F0F0F0);

    DWORD dwRegValue[2] = {0};
    dwRegValue[0] = 0x914;
    dwRegValue[1] = 0;
    CSWMessage::SendMessage(MSG_GET_CAM_FPGA_REG, NULL, (LPARAM)dwRegValue);

    if((dwRegValue[1] & 0x3) != 0x3)
    {
        dwRegValue[1] |= 0x3;	//ǿ��ʹ��ץ�Ŀ��ź�����
        CSWMessage::SendMessage(MSG_SET_CAM_FPGA_REG, (WPARAM)dwRegValue, NULL);
    }

	DWORD WriteRegParam[2]={0x92f,0x0};
	OnSetCamFpgaReg((WPARAM)WriteRegParam, 0);
	
#if 0	
	INT rgCaptureShutterValue[2] = {0};
	INT rgCaptureGainValue[2] = {0};

	rgCaptureShutterValue[0] = pCameraParam->iCaptureShutter;
	rgCaptureShutterValue[1] = 1;//pCameraParam->iCaptureShutterEnable;
	rgCaptureGainValue[0]    = pCameraParam->iCaptureGain;
	rgCaptureGainValue[1]	 = 1;//pCameraParam->iCaptureGainEnable;

	OnSetCaptureGain((WPARAM)rgCaptureGainValue, NULL);
	OnSetCaptureShutter((WPARAM)rgCaptureShutterValue, NULL);
#else
    OnSetCaptureGain((WPARAM)&(pCameraParam->iCaptureGain), NULL);
    OnSetCaptureShutter((WPARAM)&(pCameraParam->iCaptureShutter), NULL);
#endif
	this->OnSetWDRLevel(0, 0);

	OnIncreaseBrightness();

    return S_OK;
}

/**
/**
* @brief MSG_SET_COLORMATRIX_ENABLE,ʹ��ͼ����ǿ
* @param [in] wParam 0:�رգ�1����
* @param [in] lParam ����
* @return �ɹ�����S_OK������ֵ����ʧ��
*/
HRESULT CSWCameraControl2AMSG::OnSetColoMaxtrixEnable(WPARAM wParam,LPARAM lParam)
{
    DWORD Value = (DWORD)wParam;
    SW_TRACE_DEBUG("%s cmd = %d value = %d\n",__FUNCTION__,CMD_SET_SATURATION_CONTRAST_STATE, Value);

	
	
	
	
	
	return SendMessage(MSG_2A_SET_SATURATION_CONTRAST_STATE, (WPARAM)&Value , sizeof(DWORD));
}

/**
* @brief MSG_GET_COLORMATRIX_ENABLE,��ȡʹ��ͼ����ǿ
* @param [in] wParam ����
* @param [in] lParam 0:�رգ�1����
* @return �ɹ�����S_OK������ֵ����ʧ��
*/
HRESULT CSWCameraControl2AMSG::OnGetColoMaxtrixEnable(WPARAM wParam,LPARAM lParam)
{
    SW_TRACE_DEBUG("%s %d cmd = %d\n",__FUNCTION__,__LINE__,CMD_GET_SATURATION_CONTRAST_STATE);
	return SendMessage( MSG_2A_GET_SATURATION_CONTRAST_STATE, sizeof(DWORD),lParam);
}



/**
* @brief MSG_SET_SHARPEN_ENABLE,������ʹ��
* @param [in] wParam 0Ϊ�ر��񻯣�1������֡
* @param [in] lParam ����
* @return �ɹ�����S_OK������ֵ����ʧ��
*/
HRESULT CSWCameraControl2AMSG::OnSetSharpeEnable(WPARAM wParam,LPARAM lParam)
{
    SW_TRACE_DEBUG("%s %d cmd = %d\n",__FUNCTION__,__LINE__,CMD_SET_SHARPNESS_STATE);
    DWORD Value = (DWORD)wParam;
	//todo �Ѿ���֧��
	SW_TRACE_DEBUG("%s not support!\n",__FUNCTION__);
	return E_FAIL;
	//return SendMessage( MSG_2A_SET_SHARPNESS_STATE, (WPARAM)&Value , sizeof(DWORD));
    //return 0 == swpa_ipnc_control(0, CMD_SET_SHARPNESS_STATE ,(void*)&Value, sizeof(DWORD), 0)?S_OK:E_FAIL;
}

/**
* @brief MSG_GET_SHARPEN_ENABLE,��ȡ��ʹ��
* @param [in] wParam ����
* @param [in] lParam ���ͣ� DWORD*�� 0Ϊ�ر��񻯣�1������֡
* @return �ɹ�����S_OK������ֵ����ʧ��
*/
HRESULT CSWCameraControl2AMSG::OnGetSharpeEnable(WPARAM wParam,LPARAM lParam)
{
    //SW_TRACE_DEBUG("%s %d cmd = %d\n",__FUNCTION__,__LINE__,CMD_GET_SHARPNESS_STATE);//
    // return 0 == swpa_ipnc_control(0, CMD_GET_SHARPNESS_STATE ,(void*)lParam , sizeof(DWORD*), 0)?S_OK:E_FAIL;
    return E_FAIL;
}

/**
* @brief MSG_SET_SHARPENTHRESHOLD,�񻯷�ֵ
* @param [in] wParam �񻯷�ֵ[0~100]���Ƽ�ʹ��7
* @param [in] lParam ����
* @return �ɹ�����S_OK������ֵ����ʧ��
*/
HRESULT CSWCameraControl2AMSG::OnSetSharpeThreshold(WPARAM wParam,LPARAM lParam)
{
    //SW_TRACE_DEBUG("%s %d cmd = %d\n",__FUNCTION__,__LINE__,CMD_SET_SHARPNESS);
    DWORD Value = (DWORD)wParam;
    //  return 0 == swpa_ipnc_control(0, CMD_SET_SHARPNESS ,(void*)&Value , sizeof(DWORD), 0)?S_OK:E_FAIL;
    return E_FAIL;
}

/**
* @brief MSG_GET_SHARPENTHRESHOLD,�񻯷�ֵ
* @param [in] wParam ����
* @param [in] lParam ���ͣ�DWORD*, �񻯷�ֵ[0~100]
* @return �ɹ�����S_OK������ֵ����ʧ��
*/
HRESULT CSWCameraControl2AMSG::OnGetSharpeThreshold(WPARAM wParam,LPARAM lParam)
{
    //SW_TRACE_DEBUG("%s %d cmd = %d\n",__FUNCTION__,__LINE__,CMD_GET_SHARPNESS);
    // return 0 == swpa_ipnc_control(0, CMD_GET_SHARPNESS ,(void*)lParam , sizeof(DWORD*), 0)?S_OK:E_FAIL;
    return E_FAIL;
}

/**
* @brief MSG_SET_CONTRASTTHRESHOLD,���öԱȶ�
* @param [in] wParam ���ͣ�DWORD ,[-100~100]
* @param [in] lParam ����
* @return �ɹ�����S_OK������ֵ����ʧ��
*/
HRESULT CSWCameraControl2AMSG::OnSetContrastThreshold(WPARAM wParam,LPARAM lParam)
{
	DWORD Value = (DWORD)wParam;
    SW_TRACE_DEBUG("%s cmd = %d value = %d\n",__FUNCTION__,CMD_SET_CONTRAST, Value);

	
	
	
	
	
	return SendMessage( MSG_2A_SET_CONTRAST, (WPARAM)&Value , sizeof(DWORD));
}

/**
* @brief MSG_GET_CONTRASTTHRESHOLD,��ȡ�Աȶ�
* @param [in] wParam ����
* @param [in] lParam ���ͣ�DWORD* , [-100~100]
* @return �ɹ�����S_OK������ֵ����ʧ��
*/
HRESULT CSWCameraControl2AMSG::OnGetContrastThreshold(WPARAM wParam,LPARAM lParam)
{
    SW_TRACE_DEBUG("%s %d cmd = %d\n",__FUNCTION__,__LINE__,CMD_GET_CONTRAST);

	
	
	
	
	
	return SendMessage( MSG_2A_GET_CONTRAST, sizeof(DWORD), lParam );
}

/**
* @brief MSG_SET_SATURATIONTHRESHOLD,���ñ��Ͷ�
* @param [in] wParam ���Ͷ�[-100~100]
* @param [in] lParam ����
* @return �ɹ�����S_OK������ֵ����ʧ��
*/
HRESULT CSWCameraControl2AMSG::OnSetSaturationThreshold(WPARAM wParam,LPARAM lParam)
{
    DWORD Value = (DWORD)wParam;
	SW_TRACE_DEBUG("%s cmd = %d value = %d\n",__FUNCTION__,CMD_SET_SATURATION, Value);

	
	
	
	
	
	return SendMessage( MSG_2A_SET_SATURATION, (WPARAM)&Value , sizeof(DWORD));
}

/**
* @brief MSG_SET_SATURATIONTHRESHOLD,��ȡ���Ͷ�
* @param [in] wParam ����
* @param [in] lParam ���ͣ�DWORD* ,���Ͷ�[-100~100]
* @return �ɹ�����S_OK������ֵ����ʧ��
*/
HRESULT CSWCameraControl2AMSG::OnGetSaturationThreshold(WPARAM wParam,LPARAM lParam)
{
    SW_TRACE_DEBUG("%s %d cmd = %d\n",__FUNCTION__,__LINE__,CMD_GET_SATURATION);

	
	
	
	
	
	return SendMessage( MSG_2A_GET_SATURATION, sizeof(DWORD), lParam );
}

/**
* @brief MSG_SET_AGCTH ����AGCͼ����������ֵ
* @param [in] wParam ����ֵ
* @param [in] lParam ����
* @return �ɹ�����S_OK������ֵ����ʧ��
*/
HRESULT CSWCameraControl2AMSG::OnSetAGCTh( WPARAM wParam,LPARAM lParam )
{
    DWORD Value = (DWORD)wParam;
	SW_TRACE_DEBUG("%s cmd = %d value = %d\n",__FUNCTION__,CMD_SET_AE_THRESHOLD, Value);

	
	
	
	
	
	return SendMessage( MSG_2A_SET_AE_THRESHOLD, (WPARAM)&Value , sizeof(DWORD));
}

/**
* @brief MSG_SET_SHU_RANGE ����AGC�ع�ʱ�䷶Χ
* @param [in] wParam �عⷶΧDWORD���ͣ�4�ֽ�,0~15����С�ع�ʱ��16~32������ع�ʱ��
* @param [in] lParam ����
* @return �ɹ�����S_OK������ֵ����ʧ��
*/
HRESULT CSWCameraControl2AMSG::OnSetAGCShutterRange( WPARAM wParam,LPARAM lParam )
{
    DWORD Value = (DWORD)wParam;
	SW_TRACE_DEBUG("%s cmd = %d value = %d\n",__FUNCTION__,CMD_SET_AE_EXP_RANGE, Value);
    SendMessage(MSG_AUTO_SET_MAXAGCSHUTTER, Value >> 16, 0);

	
	
	
	
	
	return SendMessage( MSG_2A_SET_AE_EXP_RANGE, (WPARAM)&Value , sizeof(DWORD));
}
/**
* @brief MSG_GET_SHU_RANGE ��ȡAGC�ع�ʱ�䷶Χ
* @param [in] wParam ����
* @param [out] lParam �عⷶΧDWORD*���ͣ�4�ֽ�0~15����С�ع�ʱ��16~32������ع�ʱ��
* @return �ɹ�����S_OK������ֵ����ʧ��
*/
HRESULT CSWCameraControl2AMSG::OnGetAGCShutterRange( WPARAM wParam,LPARAM lParam )
{
    SW_TRACE_DEBUG("%s %d cmd = %d\n",__FUNCTION__,__LINE__,CMD_GET_AE_EXP_RANGE);

	
	
	
	
	
	return SendMessage( MSG_2A_GET_AE_EXP_RANGE, sizeof(DWORD), lParam );
}

/**
* @brief MSG_SET_SHU_RANGE ����AGC���淶Χ
* @param [in] wParam DWORD���ͣ�4�ֽ�0~15����С����ʱ��16~32���������ʱ��
* @param [in] lParam ����
* @return �ɹ�����S_OK������ֵ����ʧ��
*/
HRESULT CSWCameraControl2AMSG::OnSetAGCGainRange( WPARAM wParam,LPARAM lParam )
{
    DWORD Value = (DWORD)wParam;
	SW_TRACE_DEBUG("%s cmd = %d value = %d\n",__FUNCTION__,CMD_SET_AE_GAIN_RANGE, Value);

	
	
	
	
	
	return SendMessage( MSG_2A_SET_AE_GAIN_RANGE, (WPARAM)&Value , sizeof(DWORD));
}

/**
 * @brief MSG_SET_AGC_SCENE ����AGC�龰ģʽ
 * @param [in] wParam DWORD���ͣ�4�ֽڣ�0���Զ�, 1: �龰1, 2: �龰2
 * @param [in] lParam ����
 * @return �ɹ�����S_OK������ֵ����ʧ��
 */
HRESULT CSWCameraControl2AMSG::OnSetAGCScene( WPARAM wParam,LPARAM lParam )
{
    DWORD Value = (DWORD)wParam;
	SW_TRACE_DEBUG("%s cmd = %d value = %d\n",__FUNCTION__,CMD_SET_SCENE, Value);

	
	
	
	
	
	if (S_OK != SendMessage( MSG_2A_SET_SCENE, (WPARAM)&Value , sizeof(DWORD)))
	{
		SW_TRACE_NORMAL("Set AGC Scene Failed\n");
		return E_FAIL;
	}
	
	m_dwAGCScene = wParam;
	
	return S_OK;
}

/**
 * @brief MSG_GET_AGC_SCENE ��ȡAGC�龰ģʽ
 * @param [in] wParam ����
 * @param [in] lParam DWORD���ͣ�4�ֽڣ�0���Զ�, 1: �龰1, 2: �龰2
 * @return �ɹ�����S_OK������ֵ����ʧ��
 */
HRESULT CSWCameraControl2AMSG::OnGetAGCScene( WPARAM wParam,LPARAM lParam )
{
	SW_TRACE_DEBUG("%s %d cmd = %d\n",__FUNCTION__,__LINE__,CMD_GET_AE_EXP_RANGE);
	DWORD *pdwScene = (DWORD *)lParam;
	if (NULL == pdwScene)
	{
		SW_TRACE_NORMAL("OnGetAGCScene arg is NULL\n");
		return E_FAIL;
	}
	*pdwScene = m_dwAGCScene;
	
	return S_OK;
    //return 0 == swpa_ipnc_control(0, CMD_GET_SCENE ,(void*)lParam , sizeof(DWORD), 0)?S_OK:E_FAIL;
}

/**
* @brief MSG_GET_SHU_RANGE ��ȡAGC���淶Χ
* @param [in] wParam ����
* @param [out] lParam DWORD���ͣ�4�ֽ�0~15����С����ʱ��16~32���������ʱ��
* @return �ɹ�����S_OK������ֵ����ʧ��
*/
HRESULT CSWCameraControl2AMSG::OnGetAGCGainRange( WPARAM wParam,LPARAM lParam )
{
    SW_TRACE_DEBUG("%s %d cmd = %d\n",__FUNCTION__,__LINE__,CMD_GET_AE_GAIN_RANGE);

	
	
	
	
	
	return SendMessage( MSG_2A_GET_AE_GAIN_RANGE, sizeof(DWORD), lParam );
}

/**
* @brief MSG_SET_AGCZONE �豸AGC�������
* @param [in] wParam AGC�������ʹ�ܱ�־INT���ݣ�16�����ݡ�
* @param [in] ����
* @return �ɹ�����S_OK������ֵ����ʧ��
*/
HRESULT CSWCameraControl2AMSG::OnSetAGCZone(WPARAM wParam,LPARAM lParam)
{
    DWORD *rgiAGCZone = (DWORD *)wParam;
    DWORD Value = 0 ;
    for(int i = 0; i < 16; ++i)
    {
        Value |= (rgiAGCZone[i] == 1) <<i ;
    }
	SW_TRACE_DEBUG("%s cmd = %d value = %d\n",__FUNCTION__,CMD_SET_AE_ZONE, Value);

	
	
	
	

	return SendMessage( MSG_2A_SET_AE_ZONE, (WPARAM)&Value , sizeof(DWORD));
}

/**
* @brief MSG_GET_AGCZONE ��ȡAGC�������
* @param [in] wParam ����
* @param [in] lParam AGC�������ʹ�ܱ�־INT���ݣ�16�����ݡ�
* @return �ɹ�����S_OK������ֵ����ʧ��
*/
HRESULT CSWCameraControl2AMSG::OnGetAGCZone(WPARAM wParam,LPARAM lParam)
{
    SW_TRACE_DEBUG("%s %d cmd = %d\n",__FUNCTION__,__LINE__,CMD_GET_AE_ZONE);
    DWORD *rgiAGCZone = (DWORD *)lParam;
    DWORD nValue = 0 ;

	
	
	
	

	HRESULT hr = SendMessage( MSG_2A_GET_AE_ZONE, sizeof(DWORD), (LPARAM)&nValue);
    if(S_OK == hr)
    {
        //SW_TRACE_DEBUG("%s %d   nValue= %d \n",__FUNCTION__,__LINE__,nValue);
        for(int i = 0; i < 16; ++i)
        {
            rgiAGCZone[i] = (nValue >> i) & 0x1 ;
            //SW_TRACE_DEBUG("%s %d   rgiAGCZone[%d] = %d \n",__FUNCTION__,__LINE__,i,(nValue >> i) & 0x1);
        }
        return S_OK;
    }
    SW_TRACE_NORMAL("%s %d  error Ret= 0x%08x \n",__FUNCTION__,__LINE__,hr);
    return E_FAIL;

}


/**
* @brief MSG_SET_AGCTH ��ȡAGCͼ����������ֵ
* @param [in] wParam ����
* @param [out] lParam ��DWORD*���ͣ� ����ֵ����
* @return �ɹ�����S_OK������ֵ����ʧ��
*/
HRESULT CSWCameraControl2AMSG::OnGetAGCTh( WPARAM wParam,LPARAM lParam )
{
    SW_TRACE_DEBUG("%s %d cmd = %d\n",__FUNCTION__,__LINE__,CMD_GET_AE_THRESHOLD);

	
	
	
	
	
	return SendMessage( MSG_2A_GET_AE_THRESHOLD, sizeof(DWORD), lParam );
}
/**
* @brief ��Ϣ���պ���
* @param [in] wParam ����ֵ(��200)
* @param [in] lParam ����
* @return �ɹ�����S_OK������ֵ����ʧ��
*/
HRESULT CSWCameraControl2AMSG::OnSetAGCGain(WPARAM wParam,LPARAM lParam)
{
    DWORD Value = (DWORD)wParam;
	SW_TRACE_DEBUG("%s cmd = %d value = %d\n",__FUNCTION__,CMD_SET_GAIN, Value);
   	m_iManualGain = Value;
	
	
	
	
	
	
	return SendMessage( MSG_2A_SET_GAIN, (WPARAM)&Value , sizeof(DWORD));
}

/**
* @brief ��Ϣ���պ���
* @param [in] wParam ʹ����Ϣ��ʹ�ܱ�ǣ�0=��ʹ�ܣ�1 = ʹ��
* @param [in] ����
* @return �ɹ�����S_OK������ֵ����ʧ��
*/
HRESULT CSWCameraControl2AMSG::OnSetAGCEnable(WPARAM wParam,LPARAM lParam)
{
    BOOL Value = (BOOL)wParam;
    SW_TRACE_DEBUG("%s cmd = %d value = %d\n",__FUNCTION__,CMD_SET_AE_STATE, Value);

	
	
	
	

	return SendMessage( MSG_2A_SET_AE_STATE, (WPARAM)&Value , sizeof(BOOL));
}
/**
* @brief ��Ϣ���պ���
* @param [in] wParam ����
* @param [in] lParam ʹ����Ϣ��ʹ�ܱ�ǣ�0=��ʹ�ܣ�1 = ʹ��
* @return �ɹ�����S_OK������ֵ����ʧ��
*/
HRESULT CSWCameraControl2AMSG::OnGetAGCEnable(WPARAM wParam,LPARAM lParam)
{
    SW_TRACE_DEBUG("%s %d cmd = %d\n",__FUNCTION__,__LINE__,CMD_GET_AE_STATE);

	
	
	
	
	
	return SendMessage( MSG_2A_GET_AE_STATE, sizeof(DWORD), lParam );
}

/**
* @brief ��Ϣ���պ���
* @param [in] wParam ʹ����Ϣ��ʹ�ܱ�ǣ�0=��ʹ�ܣ�1 = ʹ��
* @param [in] ����
* @return �ɹ�����S_OK������ֵ����ʧ��
*/
HRESULT CSWCameraControl2AMSG::OnSetAWBEnable(WPARAM wParam,LPARAM lParam)
{
    DWORD Value = (DWORD)wParam;
    SW_TRACE_DEBUG("%s cmd = %d value = %d\n",__FUNCTION__,CMD_SET_AWB_STATE, Value);

	
	
	
	
	
	return SendMessage( MSG_2A_SET_AWB_STATE, (WPARAM)&Value , sizeof(BOOL));
}
/**
* @brief MSG_GET_AWBENABLE ��Ϣ���պ���
* @param [in] wParam ����
* @param [in] lParam ʹ����Ϣ��ʹ�ܱ�ǣ�0=��ʹ�ܣ�1 = ʹ��
* @return �ɹ�����S_OK������ֵ����ʧ��
*/
HRESULT CSWCameraControl2AMSG::OnGetAWBEnable(WPARAM wParam,LPARAM lParam)
{
    SW_TRACE_DEBUG("%s %d  cmd = %d\n",__FUNCTION__,__LINE__,CMD_GET_AWB_STATE);

	
	
	
	
	
	return SendMessage( MSG_2A_GET_AWB_STATE, sizeof(BOOL), lParam);
}


/**
* @brief ��Ϣ���պ���
* @param [in] wParam ����
* @param [out] lParam��INT*���ͣ� ���淵��ֵ(200)
* @return �ɹ�����S_OK������ֵ����ʧ��
*/
HRESULT CSWCameraControl2AMSG::OnGetAGCGain(WPARAM wParam,LPARAM lParam)
{
    SW_TRACE_DEBUG("%s %d cmd = %d\n",__FUNCTION__,__LINE__,CMD_GET_GAIN);

	
	
	
	

	
	return SendMessage( MSG_2A_GET_GAIN, sizeof(DWORD), lParam);
}


/**
* @brief ��Ϣ���պ���
* @param [in] wParam RGB����(0��7λΪRͨ��ֵ��8��15λGͨ��ֵ��16��23ΪBͨ��ֵ,����:0x00AFAAFF
* ,�ֱ�ΪFFΪBͨ�����棬AAΪGͨ�����棬AFΪRͨ������ֵ)
* ����ʹ����Ϣ����Ϊʹ�ܱ��0=��ʹ�ܣ�1 = ʹ��
* @param [in] lParam ����
* @return �ɹ�����S_OK������ֵ����ʧ��
*/
HRESULT CSWCameraControl2AMSG::OnSetRGBGain(WPARAM wParam,LPARAM lParam)
{
    DWORD *tmp = (DWORD *)wParam;
    if(tmp == NULL || tmp+1 == NULL || tmp+2== NULL)
    {
        //SW_TRACE_DEBUG("%s %d Value error \n",__FUNCTION__,__LINE__);
        return E_FAIL;
    }
    //SW_TRACE_DEBUG("%s %d  %d %d %d\n",__FUNCTION__,__LINE__,tmp[0],tmp[1],tmp[2]);
    DWORD Value;
    m_iManualGainR = tmp[0];
    m_iManualGainG = tmp[1];
    m_iManualGainB = tmp[2];
    Value = tmp[0]  | tmp[1] << 8 | tmp[2] << 16;
    SW_TRACE_DEBUG("%s cmd = %d value = %d\n",__FUNCTION__,CMD_SET_RGB, Value);

	
	
	
	

	
    return SendMessage( MSG_2A_SET_RGB, (WPARAM)&Value , sizeof(BOOL));

}
/**
* @brief ��Ϣ���պ���
* @param [in] wParam ����
* @param [out] lParam(DWORD*����) RGB����(0��7λΪRͨ��ֵ��8��15λGͨ��ֵ��16��23ΪGͨ��ֵ,����:0x00AFAAFF
* ,�ֱ�ΪFFΪBͨ�����棬AAΪGͨ�����棬AFΪRͨ������ֵ)
* @return �ɹ�����S_OK������ֵ����ʧ��
*/
HRESULT CSWCameraControl2AMSG::OnGetRGBGain(WPARAM wParam,LPARAM lParam)
{
    SW_TRACE_DEBUG("%s %d cmd = %d\n",__FUNCTION__,__LINE__,CMD_GET_RGB);
    DWORD *temp = (DWORD *)lParam;
    if(temp == NULL || temp+1 == NULL || temp +2 == NULL)
        return E_FAIL;
    int iValue;

	
	
	
	

	
	if (S_OK == SendMessage( MSG_2A_GET_RGB, sizeof(DWORD), (LPARAM)&iValue))
    {
        temp[0] = (iValue >> 0 ) & 0xff;
        temp[1] = (iValue >> 8 ) & 0xff;
        temp[2] = (iValue >> 16) & 0xff;
        return S_OK;
    }
    return E_FAIL;
}

/**
* @brief ��Ϣ���պ���
* @param [in] wParam ����ֵ
* @param [in] lParam ����
* @return �ɹ�����S_OK������ֵ����ʧ��
*/
HRESULT CSWCameraControl2AMSG::OnSetShutter(WPARAM wParam,LPARAM lParam)
{
    DWORD Value = (DWORD)wParam;
	m_iManualShutter = Value;
	SW_TRACE_DEBUG("%s cmd = %d value = %d\n",__FUNCTION__,CMD_SET_EXP, Value);

		
	
	
	
	return SendMessage( MSG_2A_SET_EXP, (WPARAM)&Value , sizeof(DWORD));
}

/**
* @brief MSG_SHUTTER ��ȡ����ָ��
* @param [in] wParam ����
* @param [out] lParam��INT*���ͣ� ����ֵ
* @return �ɹ�����S_OK������ֵ����ʧ��
*/
HRESULT CSWCameraControl2AMSG::OnGetShutter(WPARAM wParam,LPARAM lParam)
{
    SW_TRACE_DEBUG("%s %d cmd = %d\n",__FUNCTION__,__LINE__,CMD_GET_EXP);

		
	
	
	
	
	return SendMessage( MSG_2A_GET_EXP, sizeof(DWORD), lParam);
}


/**
* @brief MSG_SET_DCAPERTUREENABLE,ʹ��DC��Ȧ
* @param [in] wParam 1Ϊʹ�ܣ�0Ϊ��ʹ��
* @param [in] lParam ����
* @return �ɹ�����S_OK������ֵ����ʧ��
*/
HRESULT CSWCameraControl2AMSG::OnSetDCApertureEnable(WPARAM wParam,LPARAM lParam)
{
    DWORD Value = (DWORD)wParam;
	SW_TRACE_DEBUG("%s cmd = %d value = %d\n",__FUNCTION__,CMD_SET_DCIRIS_AUTO_STATE, Value);

	
	
	
	

	
	return SendMessage( MSG_2A_SET_DCIRIS_AUTO_STATE, (WPARAM)&Value , sizeof(DWORD));
}

/**
* @brief MSG_GET_DCAPERTUREENABLE,��ȡ�Զ�DC��Ȧʹ��
* @param [in] wParam  ����
* @param [in] lParam 1Ϊʹ�ܣ�0Ϊ��ʹ��
* @return �ɹ�����S_OK������ֵ����ʧ��
*/
HRESULT CSWCameraControl2AMSG::OnGetDCApertureEnable(WPARAM wParam,LPARAM lParam)
{
    SW_TRACE_DEBUG("%s %d cmd = %d\n",__FUNCTION__,__LINE__,CMD_GET_DCIRIS_AUTO_STATE);
	return SendMessage( MSG_2A_GET_DCIRIS_AUTO_STATE, sizeof(DWORD), lParam );
}

/**
* @brief MSG_ZOOM_DCAPERTURE,�Ŵ�DC��Ȧ
* @param [in] wParam ����
* @param [in] lParam ����
* @return �ɹ�����S_OK������ֵ����ʧ��
*/
HRESULT CSWCameraControl2AMSG::OnZoomDCAperture(WPARAM wParam,LPARAM lParam)
{
    SW_TRACE_DEBUG("%s %d cmd = %d\n",__FUNCTION__,__LINE__,CMD_ZOOM_DCIRIS);//δ�Գɹ�
    INT Value = 1;//����ֵ

		
	
	
	
	
    return SendMessage( MSG_2A_ZOOM_DCIRIS, (WPARAM)&Value , sizeof(INT));
}

/**
* @brief MSG_SHRINK_DCAPERTURE,��СDC��Ȧ
* @param [in] wParam ����
* @param [in] lParam ����
* @return �ɹ�����S_OK������ֵ����ʧ��
*/
HRESULT CSWCameraControl2AMSG::OnShrinkDCAperture(WPARAM wParam,LPARAM lParam)
{
    SW_TRACE_DEBUG("%s %d cmd = %d\n",__FUNCTION__,__LINE__,CMD_SHRINK_DCIRIS);//Ϊ���Գɹ���
    INT Value = 1;//����ֵ

	
	
	
	

	
   	return SendMessage( MSG_2A_SHRINK_DCIRIS, (WPARAM)&Value , sizeof(INT));
}

/**
* @brief MSG_SET_FILTERSWITCH �˹�Ƭ�л�
* @param [in] wParam 1�����˹�Ƭ��2�����˹�Ƭ
* @param [in] lParam ����
* @return �ɹ�����S_OK������ֵ����ʧ��
*/
HRESULT CSWCameraControl2AMSG::OnSetFilterSwitch(WPARAM wParam,LPARAM lParam)
{
    DWORD Value = (DWORD)wParam;
    m_FilterStatus = (2 == Value) ? 1 : 0;
	SW_TRACE_DEBUG("%s cmd = %d value = %d\n",__FUNCTION__,CMD_SET_FILTER_STATE, m_FilterStatus);

	
	
	
	

	
	return SendMessage( MSG_2A_SET_FILTER_STATE, (WPARAM)&m_FilterStatus , sizeof(DWORD));
}
/**
* @brief MSG_GET_FILTERSWITCH �˹�Ƭ�л�
* @param [in] wParam ����
* @param [in] lParam 1�����˹�Ƭ��2�����˹�Ƭ
* @return �ɹ�����S_OK������ֵ����ʧ��
*/
HRESULT CSWCameraControl2AMSG::OnGetFilterSwitch(WPARAM wParam,LPARAM lParam)
{
    SW_TRACE_DEBUG("%s %d cmd = %d\n",__FUNCTION__,__LINE__,CMD_SET_FILTER_STATE);
    DWORD *Value = (DWORD *)lParam;
    *Value = m_FilterStatus ? 2 : 1;
	
    return S_OK;
}

/**
* @brief MSG_SET_REDLIGHT_ENABLE ���ú�ƼӺ�ʹ��
* @param [in] wParam 0Ϊ��ʹ�ܣ�1Ϊʹ��
* @param [in] lParam ������
* @return �ɹ�����S_OK������ֵ����ʧ��
*/
HRESULT CSWCameraControl2AMSG::OnSetRedLightEnable(WPARAM wParam,LPARAM lParam)
{
    SW_TRACE_DEBUG("%s %d cmd = %d\n",__FUNCTION__,__LINE__,CMD_SET_TRAFFIC_LIGTH_ENHANCE_STATE);
    DWORD Value = (DWORD)wParam;
	//todo�Ѿ���֧��
	SW_TRACE_DEBUG("%s not support",__FUNCTION__);
	return E_FAIL;
	//return SendMessage( MSG_2A_SET_TRAFFIC_LIGTH_ENHANCE_STATE, (WPARAM)&Value , sizeof(DWORD));
    //return 0 == swpa_ipnc_control(0, CMD_SET_TRAFFIC_LIGTH_ENHANCE_STATE ,(void*)&Value , sizeof(DWORD), 0)?S_OK:E_FAIL;
}

/**
* @brief MSG_GET_REDLIGHT_ENABLE ��ȡ��ƼӺ�ʹ��
* @param [in] wParam ����
* @param [in] lParam 0Ϊ��ʹ�ܣ�1Ϊʹ��
* @return �ɹ�����S_OK������ֵ����ʧ��
*/
HRESULT CSWCameraControl2AMSG::OnGetRedLightEnable(WPARAM wParam,LPARAM lParam)
{
    SW_TRACE_DEBUG("%s %d cmd = %d\n",__FUNCTION__,__LINE__,CMD_GET_TRAFFIC_LIGTH_ENHANCE_STATE);
		//todo�Ѿ���֧��
	SW_TRACE_DEBUG("%s not support",__FUNCTION__);
	return E_FAIL;
	//return SendMessage( MSG_2A_GET_TRAFFIC_LIGTH_ENHANCE_STATE, sizeof(DWORD), lParam );
    //return 0 == swpa_ipnc_control(0, CMD_GET_TRAFFIC_LIGTH_ENHANCE_STATE ,(void*)lParam , sizeof(DWORD*), 0)?S_OK:E_FAIL;
}

/**
* @brief MSG_SET_REDLIGHT_RECT ���ú�ƼӺ�����
* @param [in] wParam SW_RECT*
* @param [in] lParam INT ��ƼӺ���������
* @return �ɹ�����S_OK������ֵ����ʧ��
*/
HRESULT CSWCameraControl2AMSG::OnSetRedLightRect(WPARAM wParam,LPARAM lParam)
{
    SW_TRACE_DEBUG("%s %d cmd = %d\n",__FUNCTION__,__LINE__,CMD_SET_TRAFFIC_LIGTH_ENHANCE_REGION);//δ��
	const int MAX_RED_RECT_COUNT = 8;
	SW_RECT* rgcRect = (SW_RECT*)wParam;
	int iCount = (int)lParam;
	if( rgcRect == NULL || iCount <= 0 )
	{
		return E_POINTER;
	}
	DWORD rgdwRedInfo[6];

	
	
	
	

	
    for(int i = 0; i < iCount; ++i)
    {
    	//SW_TRACE_DEBUG("<setred>%d(%d,%d,%d,%d).", i,
    	//		rgcRect[i].left, rgcRect[i].top, rgcRect[i].right, rgcRect[i].bottom);
    	rgdwRedInfo[0] = 0; //0:���
		rgdwRedInfo[1] = i;
    	rgdwRedInfo[2] = rgcRect[i].left;
    	rgdwRedInfo[3] = rgcRect[i].top / 2;
    	rgdwRedInfo[4] = rgcRect[i].right;
    	rgdwRedInfo[5] = rgcRect[i].bottom / 2;
		SendMessage( MSG_2A_SET_TRAFFIC_LIGTH_ENHANCE_REGION, (WPARAM)rgdwRedInfo , sizeof(rgdwRedInfo));//mark
    }
    return S_OK;
}

/**
* @brief MSG_SET_TRAFFIC_LIGTH_LUM_TH ���ú�ƼӺ�
* @param [in] wParam
* @param [in] lParam ��
* @return �ɹ�����S_OK������ֵ����ʧ��
*/
HRESULT CSWCameraControl2AMSG::OnSetRedLightThreshold(WPARAM wParam,LPARAM lParam)
{
    DWORD Value = (DWORD)wParam;
    // todo.
    // 178��ֵ���޸ģ�����ǿת
    Value = (Value == 0x2d00 ? 0x0B33 : 0x0FFF);
	Value = Value | (0x0 << 28); //28~31λΪ0�����
	SW_TRACE_DEBUG("%s cmd = %d value = %d\n",__FUNCTION__,CMD_SET_TRAFFIC_LIGTH_LUM_TH, Value);

	
	
	
	

	
	return SendMessage( MSG_2A_SET_TRAFFIC_LIGTH_LUM_TH, (WPARAM)&Value , sizeof(DWORD));
}



/**
* @brief MSG_SET_GREENLIGHT_RECT �����̵Ƽ�������
* @param [in] wParam SW_RECT*
* @param [in] lParam INT �̵ƼӺ���������
* @return �ɹ�����S_OK������ֵ����ʧ��
*/
HRESULT CSWCameraControl2AMSG::OnSetGreenLightRect(WPARAM wParam,LPARAM lParam)
{
    SW_TRACE_DEBUG("%s %d cmd = %d\n",__FUNCTION__,__LINE__,CMD_SET_TRAFFIC_LIGTH_ENHANCE_REGION);
	const int MAX_GREEN_RECT_COUNT = 8;
	SW_RECT* rgcRect = (SW_RECT*)wParam;
	int iCount = (int)lParam;
	if( rgcRect == NULL || iCount <= 0 )
	{
		return E_POINTER;
	}
	DWORD rgdwGreenInfo[6];

	
	
	
	

	
    for(int i = 0; i < iCount; ++i)
    {
    	SW_TRACE_DEBUG("<setgreed>%d(%d,%d,%d,%d).", i,
    			rgcRect[i].left, rgcRect[i].top, rgcRect[i].right, rgcRect[i].bottom);
    	rgdwGreenInfo[0] = 1; //1:�̵�
		rgdwGreenInfo[1] = i;
    	rgdwGreenInfo[2] = rgcRect[i].left;
    	rgdwGreenInfo[3] = rgcRect[i].top / 2;
    	rgdwGreenInfo[4] = rgcRect[i].right;
    	rgdwGreenInfo[5] = rgcRect[i].bottom / 2;
		SendMessage( MSG_2A_SET_TRAFFIC_LIGTH_ENHANCE_REGION, (WPARAM)rgdwGreenInfo , sizeof(rgdwGreenInfo));
    }
    return S_OK;
}

/**
* @brief MSG_SET_TRAFFIC_GREEN_LIGTH_LUM_TH �����̵Ƽ���
* @param [in] wParam
* @param [in] lParam ��
* @return �ɹ�����S_OK������ֵ����ʧ��
*/
HRESULT CSWCameraControl2AMSG::OnSetGreenLightThreshold(WPARAM wParam,LPARAM lParam)
{
    DWORD Value = (DWORD)wParam;
    // todo.
    // 178��ֵ���޸ģ�����ǿת
    Value = (Value == 0x2d00 ? 0x0B32 : 0x0FFF); 
	Value = Value | (0x1 << 28); //28~31λΪ1: �̵�
	SW_TRACE_DEBUG("%s cmd = %d value = %d\n",__FUNCTION__,CMD_SET_TRAFFIC_LIGTH_LUM_TH, Value);
	return SendMessage( MSG_2A_SET_TRAFFIC_LIGTH_LUM_TH, (WPARAM)&Value , sizeof(DWORD));
}




/**
* @brief MSG_SET_WDR_STRENGTH ����WDR��ֵ
* @param [in] wParam 0Ϊ��ʹ�ܣ�ֵԽ��ǿ��Խ��ȡֵ��Χ��0~4095
* @param [out] ����
* @return �ɹ�����S_OK������ֵ����ʧ��
*/
HRESULT CSWCameraControl2AMSG::OnSetWDRStrength(WPARAM wParam,LPARAM lParam)
{
    DWORD Value = (DWORD)wParam;
    SW_TRACE_DEBUG("%s %cmd = %d value = %d\n",__FUNCTION__,CMD_SET_WDR_STRENGTH,Value);

	
	
	
	
	
	return SendMessage( MSG_2A_SET_WDR_STRENGTH, (WPARAM)&Value , sizeof(DWORD));
}

/**
* @brief MSG_GET_WDR_THRESHOLD ����WDR��ֵ
* @param [in] wParam ����
* @param [out] lParam 0Ϊ��ʹ�ܣ�ֵԽ��ǿ��Խ��ȡֵ��Χ��0~4095
* @return �ɹ�����S_OK������ֵ����ʧ��
*/
HRESULT CSWCameraControl2AMSG::OnGetWDRStrength(WPARAM wParam,LPARAM lParam)
{
    SW_TRACE_DEBUG("%s %d cmd = %d\n",__FUNCTION__,__LINE__,CMD_GET_WDR_STRENGTH);

	
	
	
	
	
	return SendMessage( MSG_2A_GET_WDR_STRENGTH, sizeof(DWORD), lParam );
}


/**
* @brief MSG_SET_TNF_ENABLE ����TNFʹ��
* @param [in] wParam 0Ϊ��ʹ�ܣ�1Ϊʹ��
* @param [out] lParam ����
* @return �ɹ�����S_OK������ֵ����ʧ��
*/
HRESULT CSWCameraControl2AMSG::OnSetTNFEnable(WPARAM wParam,LPARAM lParam)
{
    DWORD Value = (DWORD)wParam;
	SW_TRACE_DEBUG("%s cmd = %d value = %d\n",__FUNCTION__,CMD_SET_TNF_STATE, Value);

	
	
	
		
	return SendMessage( MSG_2A_SET_TNF_STATE, (WPARAM)&Value, sizeof(DWORD));
}

/**
* @brief MSG_GET_TNF_ENABLE ��ȡTNFʹ��
* @param [in] wParam  ����
* @param [out] lParam 0Ϊ��ʹ�ܣ�1Ϊʹ��
* @return �ɹ�����S_OK������ֵ����ʧ��
*/
HRESULT CSWCameraControl2AMSG::OnGetTNFEnable(WPARAM wParam,LPARAM lParam)
{
    SW_TRACE_DEBUG("%s %d cmd = %d\n",__FUNCTION__,__LINE__,CMD_GET_TNF_STATE);

	
	
	
		
	return SendMessage( MSG_2A_GET_TNF_STATE, sizeof(DWORD), lParam );
}

/**
* @brief MSG_SET_SNF_ENABLE ����SNFʹ��
* @param [in] wParam 0Ϊ��ʹ�ܣ�1Ϊʹ��
* @param [out] lParam ����
* @return �ɹ�����S_OK������ֵ����ʧ��
*/
HRESULT CSWCameraControl2AMSG::OnSetSNFEnable(WPARAM wParam,LPARAM lParam)
{
    DWORD Value = (DWORD)wParam;
	SW_TRACE_DEBUG("%s cmd = %d value = %d\n",__FUNCTION__,CMD_SET_SNF_STATE, Value);

	
	
	
		
	return SendMessage( MSG_2A_SET_SNF_STATE, (WPARAM)&Value, sizeof(DWORD));
}

/**
* @brief MSG_GET_SNF_ENABLE ��ȡSNFʹ��
* @param [in] wParam  ����
* @param [out] lParam 0Ϊ��ʹ�ܣ�1Ϊʹ��
* @return �ɹ�����S_OK������ֵ����ʧ��
*/
HRESULT CSWCameraControl2AMSG::OnGetSNFEnable(WPARAM wParam,LPARAM lParam)
{
    SW_TRACE_DEBUG("%s %d cmd = %d\n",__FUNCTION__,__LINE__,CMD_GET_SNF_STATE);

	
	
	
		
	return SendMessage( MSG_2A_GET_SNF_STATE, sizeof(DWORD), lParam );
}

/**
* @brief MSG_SET_DENOISE_MODE ���ý���ģʽ
* @param [in] wParam 0Ϊ��ʹ�ܣ�1Ϊʹ��
* @param [out] lParam ����
* @return �ɹ�����S_OK������ֵ����ʧ��
*/
HRESULT CSWCameraControl2AMSG::OnSetDeNoiseMode(WPARAM wParam,LPARAM lParam)
{
    DWORD dwMode = (DWORD)wParam;
    DWORD dwSNFEnable = FALSE, dwTNFEnable=FALSE;
    switch (dwMode)
    {
    case 0:
        dwSNFEnable = FALSE;
        dwTNFEnable = FALSE;
        break;
	case 1:
        dwSNFEnable = FALSE;
        dwTNFEnable = TRUE;
        break;
    case 2:
        dwSNFEnable = TRUE;
        dwTNFEnable = FALSE;
        break;

    case 3:
        dwSNFEnable = TRUE;
        dwTNFEnable = TRUE;
        break;
    default:
        break;
    }

	SW_TRACE_DEBUG("%s cmd1 = %d value1 = %d, cmd1 = %d value1 = %d\n",
		__FUNCTION__,CMD_SET_TNF_STATE, dwTNFEnable,CMD_SET_SNF_STATE,dwSNFEnable);


	
	
	
	
	
    if (S_OK == SendMessage( MSG_2A_SET_TNF_STATE, (WPARAM)&dwTNFEnable, sizeof(DWORD))
		&& S_OK == SendMessage( MSG_2A_SET_SNF_STATE, (WPARAM)&dwSNFEnable, sizeof(DWORD)))
    {
        return S_OK;
    }
    return E_FAIL;
}
/**
* @brief MSG_GET_DENOISE_MODE ��ȡ����ģʽ
* @param [in] wParam  ����
* @param [out] lParam 0Ϊ��ʹ�ܣ�1Ϊʹ��
* @return �ɹ�����S_OK������ֵ����ʧ��
*/
HRESULT CSWCameraControl2AMSG::OnGetDeNoiseMode(WPARAM wParam,LPARAM lParam)
{
    SW_TRACE_DEBUG("%s %d  cmd1 = %d cmd2 = %d\n",__FUNCTION__,__LINE__,CMD_GET_SNF_STATE,CMD_GET_TNF_STATE);
    if( NULL == lParam )
    {
        return E_FAIL;
    }

		
	
	
	

    DWORD dwSNFEnable = 0, dwTNFEnable=0;
    DWORD dwMode = 0;
    /*if ( 0 == swpa_ipnc_control(0, CMD_GET_SNF_STATE ,(void*)&dwSNFEnable , sizeof(DWORD), 0)
         && 0 == swpa_ipnc_control(0, CMD_GET_TNF_STATE ,(void*)&dwTNFEnable , sizeof(DWORD), 0)
         )*/
    if (S_OK == SendMessage( MSG_2A_GET_SNF_STATE, sizeof(DWORD), (LPARAM)&dwSNFEnable)
		&& S_OK == SendMessage( MSG_2A_GET_TNF_STATE, sizeof(DWORD), (LPARAM)&dwTNFEnable))
    {
        if( 0 == dwSNFEnable && 0 == dwTNFEnable )
        {
            dwMode = 0;
        }
		else if( 0 == dwSNFEnable && 1 == dwTNFEnable )
        {
            dwMode = 1;
        }
        else if( 1 == dwSNFEnable && 0 == dwTNFEnable )
        {
            dwMode = 2;
        }
        else if( 1 == dwSNFEnable && 1 == dwTNFEnable )
        {
            dwMode = 3;
        }
        else
        {
            return E_FAIL;
        }
        *(DWORD*)lParam = dwMode;
        return S_OK;
    }
    return E_FAIL;
}

/**
* @brief MSG_SET_DENOISE_STATE ���ý���״̬
* @param [in] wParam 0~3:�Զ����͡��С���
* @param [out] lParam ����
* @return �ɹ�����S_OK������ֵ����ʧ��
*/
HRESULT CSWCameraControl2AMSG::OnSetDeNoiseState(WPARAM wParam,LPARAM lParam)
{
	DWORD Value = (DWORD)wParam;

    SW_TRACE_DEBUG("%s cmd = %d value = %d\n",__FUNCTION__,CMD_SET_TNF_SNF_STRENGTH, Value);
    
    return 0 == swpa_ipnc_control(0, CMD_SET_TNF_SNF_STRENGTH ,(void*)&Value , sizeof(DWORD), 0)?S_OK:E_FAIL;
}

/**
* @brief MSG_GET_DENOISE_STATE ��ȡ����״̬
* @param [in] wParam  ����
* @param [out] lParam 0~3:�Զ����͡��С���
* @return �ɹ�����S_OK������ֵ����ʧ��
*/
HRESULT CSWCameraControl2AMSG::OnGetDeNoiseState(WPARAM wParam,LPARAM lParam)
{
    SW_TRACE_DEBUG("%s %d cmd = %d\n",__FUNCTION__,__LINE__,CMD_GET_TNF_SNF_STRENGTH);
    return 0 == swpa_ipnc_control(0, CMD_GET_TNF_SNF_STRENGTH ,(void*)lParam , sizeof(DWORD), 0)?S_OK:E_FAIL;
}


/**
* @brief MSG_SET_AC_SYNC_ENABLE ���õ���ͬ��ʹ��
* @param [in] wParam 0����ʹ�ܣ�1��ʹ��
* @param [out] lParam ����
* @return �ɹ�����S_OK������ֵ����ʧ��
*/
HRESULT CSWCameraControl2AMSG::OnSetACSyncEnable(WPARAM wParam,LPARAM lParam)
{
    DWORD Value = (DWORD)wParam;
	SW_TRACE_DEBUG("%s cmd = %d value = %d\n",__FUNCTION__,CMD_SET_AC_SYNC_STATE, Value);

	
	
	
	

	
	return SendMessage( MSG_2A_SET_AC_SYNC_STATE, (WPARAM)&Value , sizeof(BOOL));
}

/**
* @brief MSG_GET_AC_SYNC_ENABLE ��ȡ����ͬ��ʹ��
* @param [in] wParam  ����
* @param [out] lParam 0����ʹ�ܣ�1��ʹ��
* @return �ɹ�����S_OK������ֵ����ʧ��
*/
HRESULT CSWCameraControl2AMSG::OnGetACSyncEnable(WPARAM wParam,LPARAM lParam)
{
    SW_TRACE_DEBUG("%s %d cmd = %d\n",__FUNCTION__,__LINE__,CMD_GET_AC_SYNC_STATE);

		
	
	
	
	
	return SendMessage( MSG_2A_GET_AC_SYNC_STATE, sizeof(BOOL), lParam);
}

/**
* @brief MSG_SET_AC_SYNC_DELAY ���õ���ͬ����ʱ
* @param [in] wParam ��Χ��0~25500
* @param [out] lParam ����
* @return �ɹ�����S_OK������ֵ����ʧ��
*/
HRESULT CSWCameraControl2AMSG::OnSetACSyncDelay(WPARAM wParam,LPARAM lParam)
{
    DWORD Value = (DWORD)wParam;
	SW_TRACE_DEBUG("%s cmd = %d value = %d\n",__FUNCTION__,CMD_SET_AC_SYNC_DELAY, Value);

		
	
	
	
	
	return SendMessage( MSG_2A_SET_AC_SYNC_DELAY, (WPARAM)&Value , sizeof(DWORD));
}
/**
* @brief MSG_GET_AC_SYNC_DELAY ��ȡ����ͬ����ʱ
* @param [in] wParam  ����
* @param [out] lParam ��Χ��0~25500
* @return �ɹ�����S_OK������ֵ����ʧ��
*/
HRESULT CSWCameraControl2AMSG::OnGetACSyncDelay(WPARAM wParam,LPARAM lParam)
{
    SW_TRACE_DEBUG("%s %d cmd = %d\n",__FUNCTION__,__LINE__,CMD_GET_AC_SYNC_DELAY);

	
	
	
	

	
	return SendMessage( MSG_2A_GET_AC_SYNC_DELAY, sizeof(DWORD), lParam );
}

/**
* @brief MSG_SET_GRAY_IMAGE_ENABLE ���úڰ�ͼʹ��
* @param [in] wParam 0����ʹ�ܣ�1��ʹ��
* @param [out] lParam ����
* @return �ɹ�����S_OK������ֵ����ʧ��
*/
HRESULT CSWCameraControl2AMSG::OnSetGrayImageEnable(WPARAM wParam,LPARAM lParam)
{
    DWORD Value = (DWORD)wParam;
	SW_TRACE_DEBUG("%s cmd = %d value = %d\n",__FUNCTION__,CMD_SET_GRAY_IMAGE_STATE, Value);

	
	
	
	
	
	return SendMessage( MSG_2A_SET_GRAY_IMAGE_STATE, (WPARAM)&Value , sizeof(BOOL));
}
/**
* @brief MSG_GET_GRAY_IMAGE_ENABLE ��ȡ�ڰ�ͼʹ��
* @param [in] wParam  ����
* @param [out] lParam 0����ʹ�ܣ�1��ʹ��
* @return �ɹ�����S_OK������ֵ����ʧ��
*/
HRESULT CSWCameraControl2AMSG::OnGetGrayImageEnable(WPARAM wParam,LPARAM lParam)
{
    SW_TRACE_DEBUG("%s %d cmd = %d\n",__FUNCTION__,__LINE__,CMD_GET_GRAY_IMAGE_STATE);

	
	
	
		
	return SendMessage( MSG_2A_GET_GRAY_IMAGE_STATE, sizeof(BOOL), lParam);
}
/**
* @brief MSG_SET_CVBS_STD ����CVBS��ʽ
* @param [in] wParam 0��PAL��1��NTSC
* @param [out] lParam ����
* @return �ɹ�����S_OK������ֵ����ʧ��
*/
HRESULT CSWCameraControl2AMSG::OnSetCVBSStd(WPARAM wParam,LPARAM lParam)
{
    DWORD Value = (DWORD)wParam;
    SW_TRACE_DEBUG("%s cmd = %d value = %d\n",__FUNCTION__,CMD_SET_CVBS_STD, Value);
    if(0 == swpa_ipnc_control(0, CMD_SET_CVBS_STD ,(void*)&Value , sizeof(DWORD), 0))
    {
        //SW_TRACE_DEBUG("%s %d Value\n",__FUNCTION__,__LINE__,Value);
        return S_OK;
    }
    SW_TRACE_NORMAL("%s %d Value = %d failed\n",__FUNCTION__,__LINE__,Value);
    return E_FAIL;
}
/**
* @brief MSG_GET_CVBS_STD ��ȡCVBS��ʽ
* @param [in] wParam  ����
* @param [out] lParam 0��PAL��1��NTSC
* @return �ɹ�����S_OK������ֵ����ʧ��
*/
HRESULT CSWCameraControl2AMSG::OnGetCVBSStd(WPARAM wParam,LPARAM lParam)
{
    SW_TRACE_DEBUG("%s %d cmd = %d\n",__FUNCTION__,__LINE__,CMD_GET_CVBS_STD);
    return 0 == swpa_ipnc_control(0, CMD_GET_CVBS_STD ,(void*)lParam , sizeof(DWORD), 0)?S_OK:E_FAIL;
}
/**
* @brief MSG_SET_CAM_TEST ��������Զ�������
* @param [in] wParam ����
* @param [out] lParam ����
* @return �ɹ�����S_OK������ֵ����ʧ��
*/
HRESULT CSWCameraControl2AMSG::OnSetCamTest(WPARAM wParam,LPARAM lParam)
{
    SW_TRACE_DEBUG("%s %d \n",__FUNCTION__,__LINE__);
    //��ȡAGC��AWB״̬
    BOOL fAGCEnable = 0;
    BOOL fAWBEnable = 0;
    INT fAGCGain =0;
    INT fShutter =0;
    INT rgiValue[3] = {0};

    HRESULT hr = S_OK;

    FAIL_RET(OnGetAGCEnable(  NULL,(LPARAM)&fAGCEnable));
    FAIL_RET(OnGetAWBEnable(NULL, (LPARAM)&fAWBEnable));
    //��ȡR����,G����,B����
    FAIL_RET(this->OnGetRGBGain(NULL,(LPARAM)rgiValue ));
    //��ȡ����
    FAIL_RET(this->OnGetAGCGain(NULL,(LPARAM)&fAGCGain ));
    //��ȡ����
    FAIL_RET(this->OnGetShutter(NULL,(LPARAM)&fShutter ));


    BOOL fEnable = 0;
    FAIL_RET(OnSetAGCEnable( fEnable , 0 ));
    FAIL_RET(OnSetAWBEnable( fEnable , 0 ));

    //����������С����󣬲�����1/10 *��������-��С���ţ�����ʱ��2��
    for (int i = 1 ; i <= 50; i++)
    {
        FAIL_RET(OnSetShutter( 30000 *i/50 , 0));
        swpa_thread_sleep_ms(20);
    }
    //����������С����󣻲�����1/10 *���������-��С���棩����ʱ��2��
    for (int i = 1 ; i <= 50; i++)
    {
        FAIL_RET(OnSetAGCGain( 420 *i/50 , 0));
        swpa_thread_sleep_ms(20);
    }
    //����R��С�����; ������1/10 *�����Rֵ-��СRֵ������ʱ��2��
    DWORD nValue[3];
    swpa_memset(&nValue,0,sizeof(nValue));
    for (int i = 1 ; i <= 50; i++)
    {
        nValue[0] = 255 *i/50;
        FAIL_RET(OnSetRGBGain((WPARAM)nValue  , 0));
        swpa_thread_sleep_ms(20);
    }
    //����G��С�����; ������1/10 *�����Gֵ-��СGֵ������ʱ��2��
    for (int i = 1 ; i <= 50; i++)
    {
        nValue[1] = 255 *i/50;
        FAIL_RET(OnSetRGBGain((WPARAM)nValue , 0));
        swpa_thread_sleep_ms(20);
    }
    //����B��С�����; ������1/10 *�����Bֵ-��СBֵ������ʱ��2��
    for (int i = 1 ; i <= 50; i++)
    {
        nValue[2] = 255 *i/50;
        FAIL_RET(OnSetRGBGain((WPARAM)nValue , 0));
        swpa_thread_sleep_ms(20);
    }

    //�ָ�����
    FAIL_RET(this->OnSetAGCGain((WPARAM)fAGCGain,NULL));
    //�ָ�����
    FAIL_RET(this->OnSetShutter((WPARAM)fShutter,NULL));

    //�ָ� R����,G����,B����
    FAIL_RET(this->OnSetRGBGain((WPARAM)rgiValue,NULL));
    //�ָ�AGC\AWB״̬
    FAIL_RET(OnSetAGCEnable(fAGCEnable , 0));
    FAIL_RET(OnSetAWBEnable(fAWBEnable , 0));

	//�����
	DWORD dwLedState;
	FAIL_RET(OnGetLEDSwitch(0,(LPARAM)&dwLedState));
	
	FAIL_RET(OnSetLEDSwitch((0 ==dwLedState)?1:0 ,0));
	swpa_thread_sleep_ms(1000);
    FAIL_RET(OnSetAWBEnable(dwLedState , 0));
	
    //SW_TRACE_DEBUG("%s %d end \n",__FUNCTION__,__LINE__);
    return S_OK;
}


/**
* @brief MSG_SET_GAMMA_STRENGTH �������٤��
* @param [in] wParam ��Χ��10��22
* @param [out] lParam ����
* @return �ɹ�����S_OK������ֵ����ʧ��
*/
HRESULT CSWCameraControl2AMSG::OnSetGammaStrength(WPARAM wParam,LPARAM lParam)
{
    DWORD Value = (DWORD)wParam;
	SW_TRACE_DEBUG("%s cmd = %d value = %d\n",__FUNCTION__,CMD_SET_GAMMA, Value);

		
	
	
	
	
	return SendMessage( MSG_2A_SET_GAMMA, (WPARAM)&Value , sizeof(DWORD));
}

/**
* @brief MSG_GET_GAMMA_STRENGTH ��ȡ���٤��
* @param [in] wParam ����
* @param [out] lParam ��Χ��10��22
* @return �ɹ�����S_OK������ֵ����ʧ��
*/
HRESULT CSWCameraControl2AMSG::OnGetGammaStrength(WPARAM wParam,LPARAM lParam)
{
    SW_TRACE_DEBUG("%s %d cmd = %d\n",__FUNCTION__,__LINE__,CMD_GET_GAMMA);

		
	
	
	
	
	return SendMessage( MSG_2A_GET_GAMMA, sizeof(DWORD), lParam );
}

/**
* @brief MSG_SET_GAMMA_ENABLE �������٤��ʹ��
* @param [in] wParam  ����
* @param [out] lParam 0����ʹ�ܣ�1��ʹ��
* @return �ɹ�����S_OK������ֵ����ʧ��
*/
HRESULT CSWCameraControl2AMSG::OnSetGammaEnable(WPARAM wParam,LPARAM lParam)
{
    DWORD Value = (DWORD)wParam;
	SW_TRACE_DEBUG("%s cmd = %d value = %d\n",__FUNCTION__,CMD_SET_GAMMA_ENABLE, Value);

		
	
	
	
	return SendMessage( MSG_2A_SET_GAMMA_ENABLE, (WPARAM)&Value , sizeof(BOOL));
}

/**
* @brief MSG_GET_GAMMA_ENABLE ��ȡ���٤��ʹ��
* @param [in] wParam  0����ʹ�ܣ�1��ʹ��
* @param [out] lParam ����
* @return �ɹ�����S_OK������ֵ����ʧ��
*/
HRESULT CSWCameraControl2AMSG::OnGetGammaEnable(WPARAM wParam,LPARAM lParam)
{
    SW_TRACE_DEBUG("%s %d cmd = %d\n",__FUNCTION__,__LINE__,CMD_GET_GAMMA_ENABLE);

		
	
	
	
	return SendMessage( MSG_2A_GET_GAMMA_ENABLE, sizeof(BOOL), lParam);
}



/**
* @brief MSG_SET_CAM_FPGA_REG �������FGPAд
* @param [in] wParam ��Χ����
* @param [out] lParam ����
* @return �ɹ�����S_OK������ֵ����ʧ��
*/
HRESULT CSWCameraControl2AMSG::OnSetCamFpgaReg(WPARAM wParam,LPARAM lParam)
{
	DWORD* pValue = (DWORD *)wParam;
	/*SW_TRACE_DEBUG("%s cmd = %d value[0] = 0x%x value[1] = 0x%x\n",
		__FUNCTION__,CMD_SET_FPGA_REG, pValue[0], pValue[1]);*/
	return SendMessage( MSG_2A_SET_FPGA_REG, (WPARAM)pValue , sizeof(DWORD)*2);
}

/**
* @brief MSG_GET_GAM_FPGA_REG ������FGPA��
* @param [in] wParam ����
* @param [out] lParam ��Χ����
* @return �ɹ�����S_OK������ֵ����ʧ��
*/
HRESULT CSWCameraControl2AMSG::OnGetCamFpgaReg(WPARAM wParam,LPARAM lParam)
{
    SW_TRACE_DEBUG("%s %d  0x%x 0x%x cmd = %d\n",
		__FUNCTION__,__LINE__,((INT *)lParam)[0],((INT *)lParam)[1],CMD_GET_FPGA_REG);
	//todo:���̼�ͨ��ֻ֧�ֵ�������������ݣ���������ڶ�ȡ�Ĵ�����������Ҫ�����ö�ȡ�ĵ�ַ��Ȼ��Ŷ�ȡ
	if (S_OK == SendMessage(MSG_2A_SET_FPGA_READ_REG_ADDR, (WPARAM)&((PDWORD)lParam)[0], sizeof(DWORD))
		&& S_OK == SendMessage(MSG_2A_GET_FPGA_REG, sizeof(DWORD), (LPARAM)&((PDWORD)lParam)[1]))
	{
		return S_OK;
	}
	return E_FAIL;
	//return SendMessage( MSG_2A_GET_FPGA_REG, sizeof(DWORD)*2, lParam);
}

/**
* @brief MSG_SET_AGCPARAM ����AGC����
* @param [in] wParam  DWORD[4]
* @param [out] lParam ����
* @return �ɹ�����S_OK������ֵ����ʧ��
*/
HRESULT CSWCameraControl2AMSG::OnSetAGCParam(WPARAM wParam,LPARAM lParam)
{
    DWORD* pdwParam = (DWORD*)wParam;
    SW_TRACE_DEBUG("%s %d %d %d %d %d \n ",__FUNCTION__,__LINE__,pdwParam[0],pdwParam[1],pdwParam[2],pdwParam[3]);
    DWORD Shutter = pdwParam[0] | pdwParam[1] << 16;
    DWORD Gain =  pdwParam[2] | pdwParam[3] << 16;
	SendMessage(MSG_REALTIME_SET_MAXAGCSHUTTERGAIN,pdwParam[1],pdwParam[3]);
    OnSetAGCShutterRange(Shutter,lParam);
    OnSetAGCGainRange(Gain,lParam);
    return S_OK;
}

/**
* @brief MSG_GET_AGCPARAM ��ȡAGC����
* @param [in] wParam  ����
* @param [out] lParam DWORD[4]
* @return �ɹ�����S_OK������ֵ����ʧ��
*/
HRESULT CSWCameraControl2AMSG::OnGetAGCParam(WPARAM wParam,LPARAM lParam)
{
    DWORD* pdwParam = (DWORD*)lParam;
    SW_TRACE_DEBUG("%s %d cmd = %d\n",__FUNCTION__,__LINE__,CMD_SET_SATURATION_CONTRAST_STATE);
    DWORD Shutter;
    DWORD Gain;
    OnGetAGCShutterRange(wParam,Shutter);
    OnGetAGCGainRange(wParam,Gain);
    pdwParam[0] = Shutter & 0xffff;
    pdwParam[1] = (Shutter & 0xffff0000) >>16;
    pdwParam[2] = Gain & 0xffff;
    pdwParam[3] = (Gain & 0xffff0000) >> 16;
    return S_OK;
}


/**
* @brief MSG_SET_EDGE_ENHANCE ͼ���Ե��ǿ
* @param [in] wParam BOOL
* @param [out] lParam ����
* @return �ɹ�����S_OK������ֵ����ʧ��
*/
HRESULT CSWCameraControl2AMSG::OnSetEdgeEnhance(WPARAM wParam,LPARAM lParam)
{
    DWORD Value = (DWORD)wParam;
	SW_TRACE_DEBUG("%s cmd = %d value = %d\n",__FUNCTION__,CMD_SET_EDGE_ENHANCE, Value);

	
	
	
	

	
	return SendMessage( MSG_2A_SET_EDGE_ENHANCE, (WPARAM)&Value , sizeof(DWORD));
}

/**
* @brief MSG_GET_EDGE_ENHANCE ͼ���Ե��ǿ
* @param [in] wParam ����
* @param [out] lParam BOOL*
* @return �ɹ�����S_OK������ֵ����ʧ��
*/
HRESULT CSWCameraControl2AMSG::OnGetEdgeEnhance(WPARAM wParam,LPARAM lParam)
{
    SW_TRACE_DEBUG("%s %d cmd = %d\n",__FUNCTION__,__LINE__,CMD_GET_EDGE_ENHANCE);

		
	
	
	
	return SendMessage( MSG_2A_GET_EDGE_ENHANCE, sizeof(DWORD), lParam );
}

/**
* @brief MSG_GET_MANUAL_VALUE ��ȡ�ֶ�RGB ��������
* @param [in] wParam ����
* @param [out] lParam
* @return �ɹ�����S_OK������ֵ����ʧ��
*/
HRESULT  CSWCameraControl2AMSG::OnGetManualParamValue(WPARAM wParam,LPARAM lParam)
{
    SW_TRACE_DEBUG("%s %d \n",__FUNCTION__,__LINE__);
    if(lParam == NULL)
        return E_FAIL;
    INT *iValue=(INT*)lParam;
    iValue[0] = m_iManualShutter;
    iValue[1] = m_iManualGain;
    iValue[2] = m_iManualGainR;
    iValue[3] = m_iManualGainG;
    iValue[4] = m_iManualGainB;
    return S_OK;
}
/**
* @brief ��ȡ�������״̬
* @param [in] wParam ����
* @param [out] lParam
* @return �ɹ�����S_OK������ֵ����ʧ��
*/
HRESULT CSWCameraControl2AMSG::OnGetCameraWorkState(WPARAM wParam,LPARAM lParam)
{
    SW_TRACE_DEBUG("%s %d \n",__FUNCTION__,__LINE__);
    if(lParam == NULL)
        return E_FAIL;
    CHAR *szStr = (CHAR *)lParam;

		
	
	
	

	FLOAT fltJpegFps = 0.0, fltH264Fps = 0.0;
	CSWMessage::SendMessage(MSG_SOURCE_GET_JPEG_RAW_FPS, 0, (LPARAM)&fltJpegFps);
	CSWMessage::SendMessage(MSG_SOURCE_GET_H264_RAW_FPS, 0, (LPARAM)&fltH264Fps);

	if (12.5 - 1 > fltJpegFps || 12.5 + 1 < fltJpegFps
		|| 25.0 - 2 > fltH264Fps || 25.0 + 2 < fltH264Fps)
	{
		SW_TRACE_NORMAL("Info: JPEG fps:%f, H264 %f\n",fltJpegFps, fltH264Fps);
		swpa_sprintf(szStr,"�쳣:jpeg֡��:%.1f H264֡��:%.1f.",fltJpegFps,fltH264Fps);
	}
	else
	{
		swpa_sprintf(szStr,"����:jpeg֡��:%.1f H264֡��:%.1f.",fltJpegFps,fltH264Fps);
	}
    return S_OK;
}

/**
* @brief MSG_SET_LED_SWITCH ���ò���ƿ���
* @param [in] wParam ����
* @param [out] lParam
* @return �ɹ�����S_OK������ֵ����ʧ��
*/
HRESULT CSWCameraControl2AMSG::OnSetLEDSwitch(WPARAM wParam,LPARAM lParam)
{
	SW_TRACE_DEBUG("%s %d \n",__FUNCTION__,__LINE__);

	DWORD dwSwitch = (DWORD)wParam;
	
	if(rgWriteRegParam[0]==0x927)
		OnSetCamFpgaReg((WPARAM)rgWriteRegParam, 0);
			
	tFPGA_IO_ARG tIOArg;
	tIOArg.type = E_NORMAL_LED;
	if (S_OK != SendMessage( MSG_2A_GET_IO_ARG, sizeof(tIOArg), (LPARAM)&tIOArg))
	//if (0 != swpa_ipnc_control(0, CMD_GET_IO_ARG ,(void*)&tIOArg , sizeof(tIOArg), 0))
	{
	    SW_TRACE_NORMAL("OnSetLEDSwitch CMD_GET_IO_ARG failed!");
	    return E_FAIL;
	}

	//ʹ�ܵĻ������ͬ���ԣ���ʹ������෴����
	tIOArg.enable = dwSwitch;
	tIOArg.coupling = 0;
	tIOArg.freq_num = 1;
	//tIOArg.polarity = m_dwLEDPolarity;	//LED����
	//tIOArg.output_type = m_dwLEDOutputType;		//�������
	//tIOArg.pulse_width = m_dwLEDPluseWidth;
	return SendMessage( MSG_2A_SET_IO_ARG, (WPARAM)&tIOArg , sizeof(tIOArg));

}


/**
* @brief MSG_GET_LED_SWITCH ��ȡ�����״̬
* @param [in] wParam ����
* @param [out] lParam
* @return �ɹ�����S_OK������ֵ����ʧ��
*/
HRESULT CSWCameraControl2AMSG::OnGetLEDSwitch(WPARAM wParam,LPARAM lParam)
{
	SW_TRACE_DEBUG("%s %d \n",__FUNCTION__,__LINE__);
	if(lParam == NULL)
		 return E_FAIL;

		
	
	
	

	DWORD* pdwParam = (DWORD*)lParam;
	
	tFPGA_IO_ARG tIOArg;
	tIOArg.type = E_NORMAL_LED;
	if (S_OK != SendMessage( MSG_2A_GET_IO_ARG, sizeof(tIOArg), (LPARAM)&tIOArg))
	//if(0 != swpa_ipnc_control(0, CMD_GET_IO_ARG ,(void*)&tIOArg , sizeof(tIOArg), 0))
	{
		return E_FAIL;
	}
	
	pdwParam[0] = tIOArg.enable;//(tIOArg.polarity == m_dwLEDPolarity)?1:0;
	//SW_TRACE_DEBUG("Get LED switch %d,polarity %d outputtype %d\n",
	//	pdwParam[0],tIOArg.polarity,tIOArg.output_type);
	
	return S_OK;
}

/**
* @brief MSG_SET_TRIGGEROUT_STATE �������
* @param [in] wParam 0-��ʼ, 1-����
* @param [out] lParam
* @return �ɹ�����S_OK������ֵ����ʧ��
*/
HRESULT CSWCameraControl2AMSG::OnSetTriggerOutState(WPARAM wParam,LPARAM lParam)
{
	DWORD dwState = (DWORD)wParam;
	tFPGA_IO_ARG tIOArg;
	tIOArg.type = E_TRIGGEROUT;
	tIOArg.polarity = (0 == dwState) ? m_dwTriggerOutPolarity : (!m_dwTriggerOutPolarity);
	tIOArg.output_type = m_dwTriggerOutOutputType;

	//SW_TRACE_DEBUG("Set Trigger out state: %d,polarity %d, outputtype %d m_dwTriggerOutPolarity=%d\n",
	//		dwState,tIOArg.polarity,tIOArg.output_type,m_dwTriggerOutPolarity);
	return SendMessage( MSG_2A_SET_IO_ARG, (WPARAM)&tIOArg , sizeof(tIOArg));
}



/**
* @brief MSG_SET_TRIGGEROUT ���ô������
* @param [in] polarity output_type 
* @param [out] lParam ����
* @return �ɹ�����S_OK������ֵ����ʧ��
*/
HRESULT CSWCameraControl2AMSG::OnSetTriggerOut(WPARAM wParam,LPARAM lParam)
{
	SW_TRACE_DEBUG("%s %d \n",__FUNCTION__,__LINE__);
	//polarity output_type
	DWORD *pParam = (DWORD *)wParam;
	DWORD dwPolarity = pParam[0];
	DWORD dwOutputType = pParam[1];
	tFPGA_IO_ARG tIOArg;
	tIOArg.type = E_TRIGGEROUT;
	tIOArg.enable = 1;
	tIOArg.coupling = 0;
	tIOArg.polarity = dwPolarity;
	tIOArg.output_type = dwOutputType;
	 //todo huanggr
	 rgWriteRegParam[1]&=~0xFF;
	 rgWriteRegParam[1]|=0xFF;
	tIOArg.pulse_width =(rgWriteRegParam[0]==0x927?(rgWriteRegParam[1]):m_dwLEDPluseWidth);
	
	
	
	

	
	if (S_OK != SendMessage( MSG_2A_SET_IO_ARG, (WPARAM)&tIOArg , sizeof(tIOArg)))
	{
		SW_TRACE_NORMAL("OnSetTriggerOut CMD_SET_IO_ARG Failed");
		return E_FAIL;
	}
	m_dwTriggerOutPolarity = dwPolarity;
	m_dwTriggerOutOutputType = dwOutputType;
	
	return S_OK;
}

/**
* @brief MSG_GET_EXP_IO ��ȡ���������״̬
* @param [in] ����
* @param [out] polarity output_type
* @return �ɹ�����S_OK������ֵ����ʧ��
*/
HRESULT CSWCameraControl2AMSG::OnGetTriggerOut(WPARAM wParam,LPARAM lParam)
{
	SW_TRACE_DEBUG("%s %d \n",__FUNCTION__,__LINE__);
	//polarity output_type

	if(lParam == NULL)
		return E_FAIL;

	DWORD* pdwParam = (DWORD*)lParam;
	
	tFPGA_IO_ARG tIOArg;
	tIOArg.type = E_TRIGGEROUT;

	
	
	
	

	
	if (S_OK != SendMessage( MSG_2A_GET_IO_ARG, sizeof(tIOArg), (LPARAM)&tIOArg))
	{
		SW_TRACE_NORMAL("OnGetTriggerOut CMD_GET_IO_ARG Failed\n");
		return E_FAIL;
	}
	
	pdwParam[0] = tIOArg.polarity;
	pdwParam[1] = tIOArg.output_type;
	//SW_TRACE_DEBUG("Get TriggerOut IO state polarity %d outputtype %d\n",
	//	tIOArg.polarity,tIOArg.output_type);
	
	return S_OK;
}

HRESULT CSWCameraControl2AMSG::OnSetF1TriggerOut(WPARAM wParam,LPARAM lParam)
{
	SW_TRACE_DEBUG("%s %d \n",__FUNCTION__,__LINE__);
	//polarity output_type
	DWORD *pParam = (DWORD *)wParam;
	// DWORD dwPolarity = pParam[0];
	// DWORD dwOutputType = pParam[1];
	tFPGA_IO_ARG tIOArg;
	tIOArg.type 		= E_TRIGGEROUT;
	tIOArg.enable 		= pParam[0];
	tIOArg.coupling 	= pParam[1];
	tIOArg.output_type 	= pParam[2];
	tIOArg.polarity 	= pParam[3];
	tIOArg.pulse_width	= pParam[4];

		
	
	
	

//	tIOArg.pulse_width = m_dwLEDPluseWidth; //todo huanggr
	if (S_OK != SendMessage( MSG_2A_SET_IO_ARG, (WPARAM)&tIOArg , sizeof(tIOArg)))
	{
		SW_TRACE_NORMAL("OnSetTriggerOut CMD_SET_IO_ARG Failed");
		return E_FAIL;
	}
	m_dwTriggerOutPolarity 		= tIOArg.polarity;
	m_dwTriggerOutOutputType 	= tIOArg.output_type;
	
	return S_OK;
}


HRESULT CSWCameraControl2AMSG::OnGetF1TriggerOut(WPARAM wParam,LPARAM lParam)
{
	SW_TRACE_DEBUG("%s %d \n",__FUNCTION__,__LINE__);
	//polarity output_type

	if(lParam == NULL)
		return E_FAIL;

	DWORD* pdwParam = (DWORD*)lParam;

		
	
	
	
	
	tFPGA_IO_ARG tIOArg;
	tIOArg.type = E_TRIGGEROUT;
	if (S_OK != SendMessage( MSG_2A_GET_IO_ARG, sizeof(tIOArg), (LPARAM)&tIOArg))
	{
		SW_TRACE_NORMAL("OnGetTriggerOut CMD_GET_IO_ARG Failed\n");
		return E_FAIL;
	}
	pdwParam[0] = tIOArg.enable;
	pdwParam[1] = tIOArg.coupling;
	pdwParam[2] = tIOArg.output_type;
	pdwParam[3] = tIOArg.polarity;
	pdwParam[4] = tIOArg.pulse_width;

	//SW_TRACE_DEBUG("Get TriggerOut IO state polarity %d outputtype %d\n",
	//	tIOArg.polarity,tIOArg.output_type);
	
	return S_OK;
}

HRESULT CSWCameraControl2AMSG::OnSetAlarmTriggerOut(WPARAM wParam,LPARAM lParam)
{
	SW_TRACE_DEBUG("%s %d \n",__FUNCTION__,__LINE__);
	//polarity output_type
	DWORD *pParam = (DWORD *)wParam;
	// DWORD dwPolarity = pParam[0];
	// DWORD dwOutputType = pParam[1];
	tFPGA_IO_ARG tIOArg;
	tIOArg.type 		= E_ALARM_OUT;
	tIOArg.enable 		= pParam[0];
	tIOArg.coupling 	= pParam[1];
	tIOArg.output_type 	= pParam[2];
	tIOArg.polarity 	= pParam[3];
	tIOArg.pulse_width	= pParam[4];

//	tIOArg.pulse_width = m_dwLEDPluseWidth; //todo huanggr
		
	rgWriteRegParam[0]=0x927;
	rgWriteRegParam[1]=0;
	rgWriteRegParam[1] = ((pParam[0]<<11)|(pParam[1]<<10)|(pParam[2]<<9)|(pParam[3]<<8)|pParam[4]);		

	if (S_OK != SendMessage( MSG_2A_SET_IO_ARG, (WPARAM)&tIOArg , sizeof(tIOArg)))
	{
		SW_TRACE_NORMAL("OnSetTriggerOut CMD_SET_IO_ARG Failed");
		return E_FAIL;
	}
	m_dwTriggerOutPolarity 		= tIOArg.polarity;
	m_dwTriggerOutOutputType 	= tIOArg.output_type;
	
	return S_OK;
}


HRESULT CSWCameraControl2AMSG::OnGetAlarmTriggerOut(WPARAM wParam,LPARAM lParam)
{
	SW_TRACE_DEBUG("%s %d \n",__FUNCTION__,__LINE__);
	//polarity output_type

	if(lParam == NULL)
		return E_FAIL;

	DWORD* pdwParam = (DWORD*)lParam;
	
	tFPGA_IO_ARG tIOArg;
	tIOArg.type = E_ALARM_OUT;

		
	
	
	

	if (S_OK != SendMessage( MSG_2A_GET_IO_ARG, sizeof(tIOArg), (LPARAM)&tIOArg))
	{
		SW_TRACE_NORMAL("OnGetTriggerOut CMD_GET_IO_ARG Failed\n");
		return E_FAIL;
	}
	pdwParam[0] = tIOArg.enable;
	pdwParam[1] = tIOArg.coupling;
	pdwParam[2] = tIOArg.output_type;
	pdwParam[3] = tIOArg.polarity;
	pdwParam[4] = tIOArg.pulse_width;

	//SW_TRACE_DEBUG("Get TriggerOut IO state polarity %d outputtype %d\n",
	//	tIOArg.polarity,tIOArg.output_type);
	
	return S_OK;
}

/**
* @brief MSG_SET_EXP_IO ���ò���ƿ��ƿ����
* @param [in] polarity output_type 
* @param [out] lParam ����
* @return �ɹ�����S_OK������ֵ����ʧ��
*/
HRESULT CSWCameraControl2AMSG::OnSetEXPIO(WPARAM wParam,LPARAM lParam)
{
	SW_TRACE_DEBUG("%s %d \n",__FUNCTION__,__LINE__);
	//polarity output_type
	DWORD *pParam = (DWORD *)wParam;
	DWORD dwPolarity = pParam[0];
	DWORD dwOutputType = pParam[1];

	rgWriteRegParam[1]&=~((1<<8)|(1<<9));
	rgWriteRegParam[1]|=((dwPolarity<<8)|(dwOutputType<<9));

	if(rgWriteRegParam[0]==0x927)
		OnSetCamFpgaReg((WPARAM)rgWriteRegParam, 0);
	
	/*tFPGA_IO_ARG tIOArg;
	tIOArg.type = E_NORMAL_LED;

		
	
	
	
	
	if (S_OK != SendMessage( MSG_2A_GET_IO_ARG, sizeof(tIOArg), (LPARAM)&tIOArg))
    {
        SW_TRACE_NORMAL("OnSetEXPIO CMD_GET_IO_ARG Failed!");
        return E_FAIL ;
    }

	tIOArg.polarity = dwPolarity;
	tIOArg.output_type = dwOutputType;
	//tIOArg.pulse_width = m_dwLEDPluseWidth;
	if (S_OK != SendMessage( MSG_2A_SET_IO_ARG, (WPARAM)&tIOArg , sizeof(tIOArg)))
	{
		SW_TRACE_NORMAL("OnSetEXPIO CMD_SET_IO_ARG Failed!");
		return E_FAIL ;
	}
	
	m_dwLEDPolarity = dwPolarity;		//���������ò���
	m_dwLEDOutputType = dwOutputType;*/
	
	return S_OK;
}


/**
* @brief MSG_GET_EXP_IO ��ȡ����ƿ��ƿ�״̬
* @param [in] ���� 
* @param [out] polarity output_type
* @return �ɹ�����S_OK������ֵ����ʧ��
*/
HRESULT CSWCameraControl2AMSG::OnGetEXPIO(WPARAM wParam,LPARAM lParam)
{
	SW_TRACE_DEBUG("%s %d \n",__FUNCTION__,__LINE__);
	//polarity output_type

	if(lParam == NULL)
		return E_FAIL;

	DWORD* pdwParam = (DWORD*)lParam;
	
	tFPGA_IO_ARG tIOArg;
	tIOArg.type = E_NORMAL_LED;

		
	
	
	
	
	if (S_OK != SendMessage( MSG_2A_GET_IO_ARG, sizeof(tIOArg), (LPARAM)&tIOArg))
	{
		SW_TRACE_NORMAL("OnGetEXPIO CMD_GET_IO_ARG Failed\n");
		return E_FAIL;
	}
	
	pdwParam[0] = tIOArg.polarity;
	pdwParam[1] = tIOArg.output_type;
	//SW_TRACE_DEBUG("Get EXP IO state polarity %d outputtype %d\n",
	//	tIOArg.polarity,tIOArg.output_type);
	
	return S_OK;
}

/**
* @brief MSG_SET_EXP_PLUSEWIDTH ���ò��������
* @param [in] pluse width
* @param [out] lParam ����
* @return �ɹ�����S_OK������ֵ����ʧ��
*/
HRESULT CSWCameraControl2AMSG::OnSetEXPPluseWidth(WPARAM wParam,LPARAM lParam)
{
    DWORD dwValue = (DWORD)wParam;

	rgWriteRegParam[1]&=~0xFF;
	rgWriteRegParam[1]|=dwValue;

	if(rgWriteRegParam[0]==0x927)
		OnSetCamFpgaReg((WPARAM)rgWriteRegParam, 0);

    /*tFPGA_IO_ARG tIOArg;
    tIOArg.type = E_NORMAL_LED;
		
	if (S_OK != SendMessage( MSG_2A_GET_IO_ARG, sizeof(tIOArg), (LPARAM)&tIOArg))
    {
        SW_TRACE_NORMAL("OnSetEXPPluseWidth CMD_GET_IO_ARG Failed!");
        return E_FAIL ;
    }

    tIOArg.pulse_width = dwValue;
	if (S_OK != SendMessage( MSG_2A_SET_IO_ARG, (WPARAM)&tIOArg , sizeof(tIOArg)))
    {
        SW_TRACE_NORMAL("OnSetEXPPluseWidth CMD_SET_IO_ARG Failed!");
        return E_FAIL ;
    }

    m_dwLEDPluseWidth = dwValue;*/

    // todo huanggr
    /*
    DWORD rgdwValue[2];
    rgdwValue[0] = m_dwTriggerOutPolarity;
    rgdwValue[1] = m_dwTriggerOutOutputType;
    OnSetTriggerOut((WPARAM)rgdwValue, 0);
*/
    return S_OK;
}

/**
* @brief MSG_SET_ALARM_IO ���ø澯���ƿ����
* @param [in] polarity output_type 
* @param [out] lParam ����
* @return �ɹ�����S_OK������ֵ����ʧ��
*/
HRESULT CSWCameraControl2AMSG::OnSetALMIO(WPARAM wParam,LPARAM lParam)
{
	SW_TRACE_DEBUG("%s %d \n",__FUNCTION__,__LINE__);
	//polarity output_type
	DWORD *pParam = (DWORD *)wParam;
	DWORD dwPolarity = pParam[0];
	DWORD dwOutputType = pParam[1];
	tFPGA_IO_ARG tIOArg;
	tIOArg.type = E_ALARM_OUT;
	tIOArg.polarity = dwPolarity;
	tIOArg.output_type = dwOutputType;

		
	
	
	
	
	return SendMessage( MSG_2A_SET_IO_ARG, (WPARAM)&tIOArg , sizeof(tIOArg));
}


/**
* @brief MSG_GET_ALARM_IO ��ȡ�澯���ƿ�״̬
* @param [in] ����
* @param [out] polarity output_type 
* @return �ɹ�����S_OK������ֵ����ʧ��
*/
HRESULT CSWCameraControl2AMSG::OnGetALMIO(WPARAM wParam,LPARAM lParam)
{
	SW_TRACE_DEBUG("%s %d \n",__FUNCTION__,__LINE__);
	//polarity output_type
	if(lParam == NULL)
		return E_FAIL;

	DWORD* pdwParam = (DWORD*)lParam;
	
	tFPGA_IO_ARG tIOArg;
	tIOArg.type = E_ALARM_OUT;

		
	
	
	
	
	if (S_OK != SendMessage( MSG_2A_GET_IO_ARG, sizeof(tIOArg), (LPARAM)&tIOArg))
	{
		SW_TRACE_NORMAL("OnGetALMIO CMD_GET_IO_ARG Failed\n");
		return E_FAIL;
	}
	
	pdwParam[0] = tIOArg.polarity;
	pdwParam[1] = tIOArg.output_type;
	//SW_TRACE_DEBUG("Get ALM IO state polarity %d outputtype %d\n",
	//	tIOArg.polarity,tIOArg.output_type);
	
	return S_OK;
}


/**
* @brief MSG_SET_TG_IO ���ô���ץ�Ŀڴ�������
* @param [in] ������
* @param [out] lParam ����
* @return �ɹ�����S_OK������ֵ����ʧ��
*/
HRESULT CSWCameraControl2AMSG::OnSetTGIO(WPARAM wParam,LPARAM lParam)
{
	//SW_TRACE_DEBUG("%s %d \n",__FUNCTION__,__LINE__);
	//EgdeType
	DWORD dwEgdeType = (DWORD)wParam;
	SW_TRACE_DEBUG("OnSetTGIO EgdeType %d\n",dwEgdeType);

	
	
	
	

	
	return SendMessage( MSG_2A_SET_EDGE_MOD, (WPARAM)&dwEgdeType , sizeof(DWORD));
}


/**
* @brief MSG_GET_TG_IO ��ȡ����ץ�Ŀڴ�������
* @param [in] ����
* @param [out] ������
* @return �ɹ�����S_OK������ֵ����ʧ��
*/
HRESULT CSWCameraControl2AMSG::OnGetTGIO(WPARAM wParam,LPARAM lParam)
{
	SW_TRACE_DEBUG("%s %d \n",__FUNCTION__,__LINE__);

		
	
	
	

	return SendMessage( MSG_2A_GET_EDGE_MOD, sizeof(DWORD), lParam);
}

HRESULT CSWCameraControl2AMSG::OnSetColorGradation(WPARAM wParam,LPARAM lParam)
{
	//SW_TRACE_DEBUG("%s %d \n",__FUNCTION__,__LINE__);
	//EgdeType
	DWORD dwColor = (DWORD)wParam;
	SW_TRACE_DEBUG("OnSetColorGradation value: %d\n",dwColor);

	DWORD rgdwParam[2];
	rgdwParam[0] = 0xa30;
	rgdwParam[1] = dwColor;
	return this->OnSetCamFpgaReg((WPARAM)rgdwParam,0);
}

HRESULT CSWCameraControl2AMSG::OnSetCaptureEnable(WPARAM wParam, LPARAM lParam)
{
	//TODO: complete it
	return S_OK;
}

/**
* @brief MSG_SET_WORK_MODE ���ù���ģʽ
* @param [in] ����ģʽ
* @param [int] ����
* @return �ɹ�����S_OK������ֵ����ʧ��
*/
HRESULT CSWCameraControl2AMSG::OnSetWorkMode(WPARAM wParam,LPARAM lParam)
{
	DWORD Value = (DWORD)wParam;
	SW_TRACE_DEBUG("%s cmd = %d value = %d\n",__FUNCTION__,CMD_SET_WORKMODE, Value);

		
	
	
	
	
	return SendMessage( MSG_2A_SET_WORKMODE, (WPARAM)&Value , sizeof(DWORD));
}

/**
* @brief MSG_SET_AWB_MODE ���ð�ƽ��ģʽ
* @param [in] ����ģʽ
* @param [int] ����
* @return �ɹ�����S_OK������ֵ����ʧ��
*/
HRESULT CSWCameraControl2AMSG::OnSetAWBMode(WPARAM wParam,LPARAM lParam)
{
	DWORD Value = (DWORD)wParam;
	SW_TRACE_DEBUG("%s cmd = %d value = %d\n",__FUNCTION__,CMD_SET_AWB_MODE, Value);

	
	
	
	

	
	return SendMessage( MSG_2A_SET_AWB_MODE, (WPARAM)&Value, sizeof(DWORD));
}

HRESULT CSWCameraControl2AMSG::OnSetCaptureSofttouch(WPARAM wParam,LPARAM lParam)
{
	//static DWORD s_dwLastInfo = 0;
	const DWORD iLaneInfSoftReg = 0x92f;
	DWORD crgWriteRegParam[2], dwReg[2];
	INT iAGCEnable = 0;
	INT iSet = 0;
	INT iInfo = 0;
	iSet = ( wParam & 0xFF000000 ) >> 24;
	iInfo =( wParam & 0x00FFFFFF);

    //iSet = (iSet == 0) ? 1 : 2;

	//SW_TRACE_DEBUG("<CameralControlMSG>set:%d, info:%d.\n", iSet, iInfo);
	//if((s_dwLastInfo + 1) != iInfo)
	//	SW_TRACE_DEBUG("**********ERROR:set:%d, info:%d.\n", iSet, iInfo);
	//s_dwLastInfo = iInfo;

	//�Զ�����ץ�Ĳ���
	/*if(m_fCaptureAutoParamEnable)
	{
		CSWMessage::SendMessage(MSG_AUTO_CONTROL_SET_AUTO_CAPTURE_PARAM, NULL, NULL);
	}*/

	crgWriteRegParam[0] = iLaneInfSoftReg;
	if(!m_IsDay)
	{			
		if(iInfo==0)
		{
			iSet=3;
		}		
		//���õ�·����ʹ��		
		crgWriteRegParam[1] = iSet;
	}
	else
	{		
		crgWriteRegParam[1] = 0;
	}
	OnSetCamFpgaReg((WPARAM)crgWriteRegParam, 0);
	
	//SW_TRACE_DEBUG("%s cmd = %d value = %d\n",__FUNCTION__,CMD_SOFT_CAPTURE, iInfo);

	return SendMessage( MSG_2A_SOFT_CAPTURE, (WPARAM)&iInfo , sizeof(INT));
}

HRESULT CSWCameraControl2AMSG::OnSetDspFlag(WPARAM wParam,LPARAM lParam)
{
	INT iStatus=(INT)wParam;
	if(iStatus==0)
		m_IsDay=TRUE;
	else
		m_IsDay=FALSE;
	return S_OK;
}

/**
* @brief MSG_SET_F1_OUTPUT_TYPE ����F1���ģʽ
* @param [in] F1���ģʽ
* @param [int] ����
* @return �ɹ�����S_OK������ֵ����ʧ��
*/
HRESULT CSWCameraControl2AMSG::OnSetF1OutputType(WPARAM wParam,LPARAM lParam)
{
	INT Value = (INT)wParam ? 1 : 0;
	SW_TRACE_DEBUG("%s cmd = %d value = %d\n",__FUNCTION__,CMD_SET_FLASH_GATE, Value);

		
	
	
	
	
	return SendMessage( MSG_2A_SET_FLASH_GATE, (WPARAM)&Value , sizeof(INT));
}

/**
* @brief MSG_CAPTURERGB_ENABLE ץ��RGBʹ��
* @param [in] wParam ʹ����Ϣ��ʹ�ܱ�ǣ�0=��ʹ�ܣ�1 = ʹ��
* @param [in] lParam ����
* @return �ɹ�����S_OK������ֵ����ʧ��
*/
HRESULT CSWCameraControl2AMSG::OnSetCaptureRGBEnable(WPARAM wParam,LPARAM lParam)
{

    /*int fEnable = (int)wParam;
    int rgnValue[3] = {0};
    rgnValue[0] = fEnable ? m_nCaptureR : -1;
    rgnValue[1] = fEnable ? m_nCaptureG : -1;
    rgnValue[2] = fEnable ? m_nCaptureB : -1;
    OnSetCaptureRGBGain((WPARAM)rgnValue, lParam);*/

	DWORD rgFpgaParam[2] = {0};
	rgFpgaParam[0] = 0x914;     // ʹ��ץ�Ŀ���
	int fEnable = (int)wParam;
	if(fEnable==0)
		rgFpgaParam[1] = 0x03;
	else
    	rgFpgaParam[1] = 0x0b;
    OnSetCamFpgaReg((WPARAM)rgFpgaParam, 0);

	if(fEnable!=0)
	{
	    rgFpgaParam[0] = 0xb0e;		// R ץ��
	    rgFpgaParam[1] = 0x4000;
	    OnSetCamFpgaReg((WPARAM)rgFpgaParam, 0);

	    rgFpgaParam[0] = 0xb0f;		// G ץ��
	    rgFpgaParam[1] = 0x7200;
	    OnSetCamFpgaReg((WPARAM)rgFpgaParam, 0);

	    rgFpgaParam[0] = 0xb10;		// G ץ��
	    rgFpgaParam[1] = 0x7200;
	    OnSetCamFpgaReg((WPARAM)rgFpgaParam, 0);

	    rgFpgaParam[0] = 0xb11;		// B ץ��
	    rgFpgaParam[1] = 0xa500;
	    OnSetCamFpgaReg((WPARAM)rgFpgaParam, 0);
	}
    
    return S_OK;
}

/**
* @brief MSG_CAPTURESHUTTER_ENABLE ץ�Ŀ���ʹ��
* @param [in] wParam ʹ����Ϣ��ʹ�ܱ�ǣ�0=��ʹ�ܣ�1 = ʹ��
* @param [in] lParam ����
* @return �ɹ�����S_OK������ֵ����ʧ��
*/
HRESULT CSWCameraControl2AMSG::OnSetCaptureShutterEnable(WPARAM wParam,LPARAM lParam)
{
/*
    int fEnable = (int)wParam;
    int nValue = fEnable ? m_nCaptureShutter : -1;
    OnSetCaptureShutter((WPARAM)&nValue, lParam);
    */
    return S_OK;
}

/**
* @brief MSG_CAPTURESHUTTER_ENABLE ץ������ʹ��
* @param [in] wParam ʹ����Ϣ��ʹ�ܱ�ǣ�0=��ʹ�ܣ�1 = ʹ��
* @param [in] lParam ����
* @return �ɹ�����S_OK������ֵ����ʧ��
*/
HRESULT CSWCameraControl2AMSG::OnSetCaptureGainEnable(WPARAM wParam,LPARAM lParam)
{
/*
    int fEnable = (int)wParam;
    int nValue = fEnable ? m_nCaptureGain : -1;
    OnSetCaptureGain((WPARAM)&nValue, lParam);
    */
    return S_OK;
}

/**
 * @brief MSG_CAPTURESHARPEN_ENABLE ץ����ʹ��
 * @param [in] wParam ʹ����Ϣ��ʹ�ܱ�ǣ�0=��ʹ�ܣ�1 = ʹ��
 * @param [in] lParam ����
 * @return �ɹ�����S_OK������ֵ����ʧ��
 */
HRESULT CSWCameraControl2AMSG::OnSetCaptureSharpenEnable(WPARAM wParam,LPARAM lParam)
{
/*
    int fEnable = (int)wParam;
    int nValue = fEnable ? m_nCapSharpen : -1;
    OnSetCaptureSharpen((WPARAM)&nValue, lParam);
    */
    return S_OK;
}

/**
* @brief ��Ϣ���պ���
* @param [in] wParam ����ֵ
* @param [in] lParam ����
* @return �ɹ�����S_OK������ֵ����ʧ��
*/
HRESULT CSWCameraControl2AMSG::OnSetCaptureShutter(WPARAM wParam,LPARAM lParam)
{
    DWORD* Value = (DWORD*)wParam;
    SW_TRACE_DEBUG("Set Capture Shutter:%d",*Value);

		
	
	
	

    return SendMessage( MSG_2A_SET_CAP_SHUTTER, (WPARAM)Value , sizeof(DWORD));
}

/**
* @brief ��Ϣ���պ���
* @param [in] wParam ����
* @param [out] lParam(INT*����) ����ֵ
* @return �ɹ�����S_OK������ֵ����ʧ��
*/
HRESULT CSWCameraControl2AMSG::OnGetCaptureShutter(WPARAM wParam,LPARAM lParam)
{
    //swpa_camera_basicparam_get_capture_shutter((int*)lParam);
    return S_OK;
}

/**
* @brief ��Ϣ���պ���
* @param [in] wParam ����ֵ(200)
* @param [in] lParam ����
* @return �ɹ�����S_OK������ֵ����ʧ��
*/
HRESULT CSWCameraControl2AMSG::OnSetCaptureGain(WPARAM wParam,LPARAM lParam)
{
    DWORD* Value = (DWORD*)wParam;
    SW_TRACE_DEBUG("Set Capture Gain:%d",*Value);

		
	
	
	

    return SendMessage( MSG_2A_SET_CAP_GAIN, (WPARAM)Value , sizeof(DWORD));
}

/**
* @brief ��Ϣ���պ���
* @param [in] wParam ����
* @param [out] lParam(INT* ����) ����ֵ
* @return �ɹ�����S_OK������ֵ����ʧ��
*/
HRESULT CSWCameraControl2AMSG::OnGetCaptureGain(WPARAM wParam,LPARAM lParam)
{
    //swpa_camera_basicparam_get_capture_gain((int*)lParam);
    return S_OK;
}

/**
* @brief ��Ϣ���պ���
* @param [in] wParam RGB��������.0:R,1:G,2:B
* @param [in] lParam ����
* note �����ת�ĳ�(0��7λΪBͨ��ֵ��8��15λGͨ��ֵ��16��23ΪRͨ��ֵ,����:0x00AFAAFF
*   �ֱ�ΪFFΪBͨ�����棬AAΪGͨ�����棬AFΪRͨ������ֵ)
* @return �ɹ�����S_OK������ֵ����ʧ��
*/
HRESULT CSWCameraControl2AMSG::OnSetCaptureRGBGain(WPARAM wParam,LPARAM lParam)
{
    int *prgnValue = (int*)wParam;
	
    int nEnable = ((INT *)wParam)[3];
    int nValue = 0;

    if (prgnValue[0] == -1 || prgnValue[1] == -1 || prgnValue[2] == -1)
    {
        nEnable = 0;
    }
    else
    {
        m_nCaptureR = prgnValue[0];
        m_nCaptureG = prgnValue[1];
        m_nCaptureB = prgnValue[2];
    }
	DWORD Value = prgnValue[0]  | prgnValue[1] << 8 | prgnValue[2] << 16;

		
	
	
	
	
    return SendMessage(MSG_2A_SET_CAP_RGB,(WPARAM)&Value,sizeof(DWORD));
}

/**
* @brief ��Ϣ���պ���
* @param [in] wParam ����
* @param [out] lParam(INT��������)0:R,1:G,2:B
* @return �ɹ�����S_OK������ֵ����ʧ��
*/
HRESULT CSWCameraControl2AMSG::OnGetCaptureRGBGain(WPARAM wParam,LPARAM lParam)
{
    //int *prgnValue = (int*)lParam;
    //swpa_camera_basicparam_get_capture_rgb_gain(&prgnValue[0], &prgnValue[1], &prgnValue[2]);
    return S_OK;
}

/**
* @brief MSG_SET_CAPTURE_SHARPEN_THR ����ץ�����ָ��
* @param [in] wParam ���ֵ
* @param [in] lParam ����
* @return �ɹ�����S_OK������ֵ����ʧ��
*/
HRESULT CSWCameraControl2AMSG::OnSetCaptureSharpen(WPARAM wParam,LPARAM lParam)
{
	DWORD Value = (DWORD)wParam;
	SW_TRACE_DEBUG("Set Capture Sharpen:%d",Value);

		
	
	
	

	return SendMessage(MSG_2A_SET_CAP_SHARPEN, (WPARAM)&Value , sizeof(DWORD));
}

/**
* @brief MSG_GET_CAPTURE_SHARPEN_THR ��ȡץ�����ָ��
* @param [in] wParam ����
* @param [out] lParam ���ֵ
* @return �ɹ�����S_OK������ֵ����ʧ��
*/
HRESULT CSWCameraControl2AMSG::OnGetCaptureSharpen(WPARAM wParam,LPARAM lParam)
{
    //swpa_camera_basicparam_get_capture_sharpen((int*)lParam);
    return S_OK;
}

/**
 * @brief MSG_GET_M3_DAYNIGHT_STATUS ��ȡM3�������ϱ�ʶ
 * @param [in] wParam ����
 * @param [out] lParam ��ʶֵ
 * @return �ɹ�����S_OK������ֵ����ʧ��
 */
HRESULT CSWCameraControl2AMSG::OnGetM3DayNightStatus(WPARAM wParam,LPARAM lParam)
{
	return SendMessage( MSG_2A_GET_DAY_NIGHT, sizeof(DWORD), lParam);
}

HRESULT CSWCameraControl2AMSG::OnSetWDRLevel(WPARAM wParam,LPARAM lParam)
{
    int Value = (int)wParam;
	SW_TRACE_DEBUG("%s cmd = %d value = %d\n",__FUNCTION__,CMD_SET_BACKLIGHT, Value);

	
	
	
	

	
	return SendMessage( MSG_2A_SET_BACKLIGHT, (WPARAM)&Value , sizeof(INT));
}

HRESULT CSWCameraControl2AMSG::OnSetTNFState(WPARAM wParam,LPARAM lParam)
{
    DWORD Value = (DWORD)wParam;
	SW_TRACE_DEBUG("%s cmd = %d value = %d\n",__FUNCTION__,CMD_SET_TNF_STATE, Value);
    return 0 == swpa_ipnc_control(0, CMD_SET_TNF_STATE ,(void*)&Value , sizeof(DWORD), 0)?S_OK:E_FAIL;
}
HRESULT CSWCameraControl2AMSG::OnGetTNFState(WPARAM wParam,LPARAM lParam)
{
	//todo
	return E_FAIL;
}
HRESULT CSWCameraControl2AMSG::OnSetSNFState(WPARAM wParam,LPARAM lParam)
{
    DWORD Value = (DWORD)wParam;
	SW_TRACE_DEBUG("%s cmd = %d value = %d\n",__FUNCTION__,CMD_SET_SNF_STATE, Value);
    return 0 == swpa_ipnc_control(0, CMD_SET_SNF_STATE ,(void*)&Value , sizeof(DWORD), 0)?S_OK:E_FAIL;
}
HRESULT CSWCameraControl2AMSG::OnGetSNFState(WPARAM wParam,LPARAM lParam)
{
	//todo
	return E_FAIL;
}


HRESULT CSWCameraControl2AMSG::OnSetCaptureAutoParamEnable(WPARAM wParam,LPARAM lParam)
{
	m_fCaptureAutoParamEnable = ((INT)wParam == 1) ? TRUE : FALSE;

	//CSWMessage::SendMessage(MSG_AUTO_CONTROL_SET_AUTO_CAPTURE_PARAM, wParam, NULL);

	//SW_TRACE_DEBUG("OnSetCaptureAutoParamEnable value:%d \n",m_fCaptureAutoParamEnable);

	//send remote msg
	return S_OK;
}

HRESULT CSWCameraControl2AMSG::OnGetCaptureAutoParamEnable(WPARAM wParam,LPARAM lParam)
{
	INT *pEnable = (INT *)lParam;
	*pEnable = (m_fCaptureAutoParamEnable ? 1 : 0);

	//send remote msg
	return S_OK;
}

HRESULT CSWCameraControl2AMSG::OnGetPTZInfo(PVOID pvBuffer, INT iSize)
{
    SW_TRACE_DEBUG("Info: got GetPTZInfo msg...\n");

    //send remote msg to notify the ptz info
    PTZ_INFO sInfo;

	swpa_memset(&sInfo, 0, sizeof(sInfo));

	/* not support any PTZ feature */

		
	
	
	
        
    return SendRemoteMessage(MSG_APP_REMOTE_SET_PTZ_INFO, (PVOID)&sInfo, sizeof(sInfo));
}


BOOL CSWCameraControl2AMSG::OnIncreaseBrightness()
{
	DWORD rgFpgaParam[2] = {0};
	//����Ϊ����ͼ�����ã� �����żĴ�������  __WRITE_FPGA_START__
	rgFpgaParam[0] = 0xb81;		//gamma ����
    rgFpgaParam[1] = 0x00;
	OnSetCamFpgaReg((WPARAM)rgFpgaParam, 0);


	rgFpgaParam[0] = 0xb83;		//������������
    rgFpgaParam[1] = 0x02;
	OnSetCamFpgaReg((WPARAM)rgFpgaParam, 0);

	rgFpgaParam[0] = 0xa3c;
	rgFpgaParam[1] = 0xd0;
	OnSetCamFpgaReg((WPARAM)rgFpgaParam, 0);

	rgFpgaParam[0] = 0xa84;
	rgFpgaParam[1] = 0x03;
	OnSetCamFpgaReg((WPARAM)rgFpgaParam, 0);

    rgFpgaParam[0] = 0x914;     // ʹ��ץ�Ŀ���
    rgFpgaParam[1] = 0x0b;
    OnSetCamFpgaReg((WPARAM)rgFpgaParam, 0);

    rgFpgaParam[0] = 0xb0e;		// R ץ��
    rgFpgaParam[1] = 0x4000;
    OnSetCamFpgaReg((WPARAM)rgFpgaParam, 0);

    rgFpgaParam[0] = 0xb0f;		// G ץ��
    rgFpgaParam[1] = 0x7200;
    OnSetCamFpgaReg((WPARAM)rgFpgaParam, 0);

    rgFpgaParam[0] = 0xb10;		// G ץ��
    rgFpgaParam[1] = 0x7200;
    OnSetCamFpgaReg((WPARAM)rgFpgaParam, 0);

    rgFpgaParam[0] = 0xb11;		// B ץ��
    rgFpgaParam[1] = 0xa500;
    OnSetCamFpgaReg((WPARAM)rgFpgaParam, 0);
	// __ WRITE_FPGA_END__

	return TRUE;
}
