
#include "SWFC.h"
#include "SWCameralControlMSG.h"
#include "SWTaskThreadPool.h"
#include "SWPTZParameter.h"

//REGISTER_CLASS(CSWCameralControlMSG)

CSWCameralControlMSG::CSWCameralControlMSG()
: CSWMessage( MSG_CAMERA_CONTROL_START , MSG_CAMERA_CONTROL_END )
, m_pSWAgcAwbTask( new CSWAgcAwbTask())
, m_pSWCapture( new CSWCaptureTask())
, m_pSWChanneCorrTask( new CSWChannelDifferenceCorrectTask( m_pSWAgcAwbTask ))
, m_pSWImageQualitySet( new CSWImageQualitySet( ) )
, m_pSWExtDevControl( new CSWExtDevControl )
{
    // ��ʼ���̳߳�
    if( !CSWTaskThreadPool::GetNumThreads()){
        CSWTaskThreadPool::SetNumShortTaskThreads( 2 );
        CSWTaskThreadPool::SetNumBlockingTaskThreads( 0 );
        CSWTaskThreadPool::AddThreads( 2 );
    }
}

CSWCameralControlMSG::~CSWCameralControlMSG()
{
    // �ͷ�AGC����
    if( m_pSWAgcAwbTask != NULL )
        m_pSWAgcAwbTask->Signal( CSWTask::kKillEvent );
    m_pSWAgcAwbTask = NULL;
    // �ͷ�ץ������������
     if( m_pSWCapture != NULL )
        m_pSWCapture->Signal( CSWTask::kKillEvent );
    m_pSWCapture = NULL;
    // �ͷ���Ƶ������������
     if( m_pSWChanneCorrTask != NULL )
        m_pSWChanneCorrTask->Signal( CSWTask::kKillEvent );
    m_pSWChanneCorrTask = NULL;

    if( m_pSWImageQualitySet != NULL )
        delete m_pSWImageQualitySet;
    m_pSWImageQualitySet = NULL;

    if( m_pSWExtDevControl != NULL )
        delete m_pSWExtDevControl;
    m_pSWExtDevControl = NULL;

    // �˳����������߳�
    CSWTaskThreadPool::StopThreads( );
}
/**
* @brief MSG_INITIAL_PARAM �������������ʼ�������ṹ��
* @param [in] wParam ��CamAppParam*�ṹָ�����ͣ�
* @param [in] lParam ����
* @return �ɹ�����S_OK������ֵ����ʧ��
*/
HRESULT CSWCameralControlMSG::OnInitialCameralParam( WPARAM wParam,LPARAM lParam )
{
    LPCAMERAPARAM_INFO pCameraParam = (LPCAMERAPARAM_INFO)wParam;

    if( pCameraParam == NULL ) return S_FALSE;

    // ����AGC������ֵ
    this->OnSetAGCTh( pCameraParam->iAGCTh , 0 );


    // �������
    this->OnSetAGCGain( pCameraParam->iGain , 0 );
    // �������
    this->OnSetShutter( pCameraParam->iShutter , 0 );

    INT nValue = (pCameraParam->iAGCShutterLOri << 16);
    nValue |= (pCameraParam->iAGCGainLOri & 0x0000FFFF);

    // AGC���ŵ�������,AGC�����������
    this->OnSetAGCMinShuAndGain( nValue , 0 );

    nValue = (pCameraParam->iAGCShutterHOri << 16);
    nValue |= (pCameraParam->iAGCGainHOri & 0x0000FFFF);

    // AGC���ŵ�������,AGC�����������
    this->OnSetAGCMaxShuAndGain( nValue , 0 );

	// AGC �������
	for(int i = 0; i < 16; ++i)
	{
		CSWImageStatisticsInfo::SetImageListInfo((pCameraParam->rgiAGCZone[i] == 1), i);
	}
	
    DWORD parrRGB[3];

    parrRGB[0] = pCameraParam->iGainR;
    parrRGB[1] = pCameraParam->iGainG;
    parrRGB[2] = pCameraParam->iGainB;
    // R����,G����,B����
    this->OnSetRGBGain( (DWORD)parrRGB , 0 );

    // ��ʼ��ץ����Ϣ

    // ץ��RGB����ʹ��
    this->OnSetCaptureRGBEnable( pCameraParam->iCaptureRGBEnable , 0 );
    // ץ�Ŀ���ʹ��
    this->OnSetCaptureShutterEnable( pCameraParam->iCaptureShutterEnable, 0 );
    // ץ������ʹ��
    this->OnSetCaptureGainEnable( pCameraParam->iCaptureGainEnable , 0 );

	// ֻ��ʹ�ܵ�����²�����
	if( pCameraParam->iCaptureGainEnable == 1 )
	{
		this->OnSetCaptureGain( (WPARAM)&pCameraParam->iCaptureGain , 0);
	}
	if( pCameraParam->iCaptureShutterEnable == 1 )
	{
		this->OnSetCaptureShutter( (WPARAM)&pCameraParam->iCaptureShutter , 0 );
	}

	if( pCameraParam->iCaptureRGBEnable == 1 )
	{
		// ����ץ��RGB����
		INT rgRGB[3];
		rgRGB[0] = pCameraParam->iCaptureGainR;
		rgRGB[1] = pCameraParam->iCaptureGainG;
		rgRGB[2] = pCameraParam->iCaptureGainB;
		this->OnSetCaptureRGBGain( (WPARAM)rgRGB ,0);
	}

    // ������ֵ
    this->OnSetSharpeThreshold( pCameraParam->iSharpenThreshold , 0);
    // ���öԱȶ�
    this->OnSetContrastThreshold( pCameraParam->iContrastValue , 0);
    // ���� ���Ͷ�
    this->OnSetSaturationThreshold( pCameraParam->iSaturationValue , 0);
    // ����ɫ��
    this->OnSetTemperatureThreshold( pCameraParam->iTemThreshold , 0);
    // ����ɫ��
    this->OnSetHueThreshold( pCameraParam->iHueThreshold , 0);
    // ���ó���ֵ
    this->OnSetHazeremovalThreshold( pCameraParam->iHazeRemovalThreshold , 0);

    // ʹ��ץ�Ĳ�����У��
    this->OnSetCaptureEnable( pCameraParam->iCapiffCorEnable , 0);
    // ʹ��DC��Ȧ
    this->OnSetDCApertureEnable( pCameraParam->iEnableDCAperture , 0);
    // ʹ��AGC��AWB
    this->OnSetAGCEnable( pCameraParam->iAGCEnable , 0 );
    this->OnSetAWBEnable( pCameraParam->iAWBEnable, 0);
    // ������У��ʹ��
    this->OnSetDiffCorrEnable(pCameraParam->iDiffCorEnable , 0 );

    // ������ʹ��ģʽ
    INT nShareMode = pCameraParam->iSharpenCapture == 0 ? 3 : 2;
    nShareMode = pCameraParam->iEnableSharpen == 0 ? 1 : nShareMode;
    this->OnSetSharpeMode( nShareMode , 0);

    // ʹ��ͼ����ǿ
    this->OnSetColoMaxtrixMode( pCameraParam->iColorMatrixMode , 0);
    // ʹ�ܳ���
    this->OnSetHazeremovalMode( pCameraParam->iHazeRemovalMode , 0);
    // ��ʼ���ⲿ�豸
    m_pSWExtDevControl->InitialExtDevParam( pCameraParam );

	// ��ʼƫ�⾵Ϊ��ʹ�ܡ�
	this->OnSetFilterSwitch(1, 0);

    return S_OK;
}

/**
* @brief MSG_CAPTURERGB_ENABLE ץ��RGBʹ��
* @param [in] wParam ʹ����Ϣ��ʹ�ܱ�ǣ�0=��ʹ�ܣ�1 = ʹ��
* @param [in] lParam ����
* @return �ɹ�����S_OK������ֵ����ʧ��
*/
HRESULT CSWCameralControlMSG::OnSetCaptureRGBEnable(WPARAM wParam,LPARAM lParam)
{
    return m_pSWCapture->SetRGBEnable( wParam );
}

HRESULT CSWCameralControlMSG::OnSetGammaMode(WPARAM wParam,LPARAM lParam)
{
	INT iMode = (INT)wParam;
	INT iRet = swpa_camera_imgproc_set_gamma_mode(iMode);
	return iRet == SWPAR_OK ? S_OK : E_FAIL;
}

HRESULT CSWCameralControlMSG::OnGetGammaMode(WPARAM wParam,LPARAM lParam)
{
	INT* piMode = (INT*)lParam;
	HRESULT hr = S_OK;
	if( piMode != NULL )
	{
		INT iRet = swpa_camera_imgproc_get_gamma_mode(piMode);
		hr = (iRet == SWPAR_OK ? S_OK : E_FAIL);
	}
	else
	{
		hr = E_INVALIDARG;
	}
	return hr;
}

/**
* @brief MSG_CAPTURESHUTTER_ENABLE ץ�Ŀ���ʹ��
* @param [in] wParam ʹ����Ϣ��ʹ�ܱ�ǣ�0=��ʹ�ܣ�1 = ʹ��
* @param [in] lParam ����
* @return �ɹ�����S_OK������ֵ����ʧ��
*/
HRESULT CSWCameralControlMSG::OnSetCaptureShutterEnable(WPARAM wParam,LPARAM lParam)
{
    return m_pSWCapture->SetShutterEnable( wParam );
}

/**
* @brief MSG_CAPTURESHUTTER_ENABLE ץ������ʹ��
* @param [in] wParam ʹ����Ϣ��ʹ�ܱ�ǣ�0=��ʹ�ܣ�1 = ʹ��
* @param [in] lParam ����
* @return �ɹ�����S_OK������ֵ����ʧ��
*/
HRESULT CSWCameralControlMSG::OnSetCaptureGainEnable(WPARAM wParam,LPARAM lParam)
{
    HRESULT hr = m_pSWCapture->SetGainEnable( wParam );
	// ץ�������ʹ��Ҫ��ץ�Ĳ���У��ͬ����
	// ʹ��ץ�Ĳ�����У��
	hr = OnSetCaptureEnable( wParam , 0);

	return hr;
}
/**
* @brief MSG_SET_COLORMATRIXMODE,ʹ��ͼ����ǿ
* @param [in] wParam 1Ϊ�ر�ͼ����ǿ��2Ϊͼ����ǿץ�ģ�3ͼ����ǿ����֡
* @param [in] lParam ����
* @return �ɹ�����S_OK������ֵ����ʧ��
*/
HRESULT CSWCameralControlMSG::OnSetColoMaxtrixMode(WPARAM wParam,LPARAM lParam)
{
    return m_pSWImageQualitySet->SetColorParamMode( wParam );
}
/**
* @brief MSG_SET_SHARPENMODE,��ʹ��
* @param [in] wParam 1Ϊ�ر��񻯣�2Ϊ��ץ�ģ�3������֡
* @param [in] lParam ����
* @return �ɹ�����S_OK������ֵ����ʧ��
*/
HRESULT CSWCameralControlMSG::OnSetSharpeMode(WPARAM wParam,LPARAM lParam)
{
    m_pSWImageQualitySet->SetSharpenMode( wParam ); return S_OK;
}
/**
* @brief MSG_SET_SHARPENTHRESHOLD,�񻯷�ֵ
* @param [in] wParam �񻯷�ֵ[0~100]���Ƽ�ʹ��7
* @param [in] lParam ����
* @return �ɹ�����S_OK������ֵ����ʧ��
*/
HRESULT CSWCameralControlMSG::OnSetSharpeThreshold(WPARAM wParam,LPARAM lParam)
{
    return m_pSWImageQualitySet->SetSharpenParam( wParam );
}
/**
* @brief ��Ϣ���պ���
* @param [in] wParam ����Gammaֵ�б�ָ��Gamma[8][2]
* @param [in] lParam ����
* @return �ɹ�����S_OK������ֵ����ʧ��
*/
HRESULT CSWCameralControlMSG::OnSetGamma(WPARAM wParam,LPARAM lParam)
{
    SWFC_DEBUG_ARG("(%d,%d)", wParam, lParam);
    m_pSWImageQualitySet->SetGamma( (INT*)wParam ); return S_OK;
}
/**
* @brief MSG_SET_CONTRASTTHRESHOLD,���öԱȶ�
* @param [in] wParam �Աȶ�[-100~100]
* @param [in] lParam ����
* @return �ɹ�����S_OK������ֵ����ʧ��
*/
HRESULT CSWCameralControlMSG::OnSetContrastThreshold(WPARAM wParam,LPARAM lParam)
{
    return m_pSWImageQualitySet->SetImageContrast( wParam );
}

/**
* @brief MSG_SET_SATURATIONTHRESHOLD,���ñ��Ͷ�
* @param [in] wParam �Աȶ�[-100~100]
* @param [in] lParam ����
* @return �ɹ�����S_OK������ֵ����ʧ��
*/
HRESULT CSWCameralControlMSG::OnSetSaturationThreshold(WPARAM wParam,LPARAM lParam)
{
    return m_pSWImageQualitySet->SetImageSaturation( wParam );
}


/**
* @brief MSG_SET_SATURATIONTHRESHOLD,����ɫ��
* @param [in] wParam ɫ��5������[32,40,52,55,60,70]
* @param [in] lParam ����
* @return �ɹ�����S_OK������ֵ����ʧ��
*/
HRESULT CSWCameralControlMSG::OnSetTemperatureThreshold(WPARAM wParam,LPARAM lParam)
{
    return m_pSWImageQualitySet->SetImageTemperature( wParam );
}

/**
* @brief MSG_SET_HUETHRESHOLD,����ɫ��
* @param [in] wParam �Աȶ�[-180~180]
* @param [in] lParam ����
* @return �ɹ�����S_OK������ֵ����ʧ��
*/
HRESULT CSWCameralControlMSG::OnSetHueThreshold(WPARAM wParam,LPARAM lParam)
{
    return m_pSWImageQualitySet->SetImageHue( wParam );
}

/**
* @brief MSG_SET_HAZEREMOVALMODE,ʹ�ܳ���ģʽ
* @param [in] wParam 1Ϊ�رճ���2Ϊ����ץ�ģ�3Ϊ��������֡
* @param [in] lParam ����
* @return �ɹ�����S_OK������ֵ����ʧ��
*/
HRESULT CSWCameralControlMSG::OnSetHazeremovalMode(WPARAM wParam,LPARAM lParam)
{
    return m_pSWImageQualitySet->SetHazeRemovalMode( wParam );
}

/**
* @brief MSG_SET_HUETHRESHOLD,���ó���ֵ
* @param [in] wParam ����ֵ[0~10]
* @param [in] lParam ����
* @return �ɹ�����S_OK������ֵ����ʧ��
*/
HRESULT CSWCameralControlMSG::OnSetHazeremovalThreshold(WPARAM wParam,LPARAM lParam)
{
    return m_pSWImageQualitySet->SetHazeRemovalParam( wParam );
}
/**
* @brief MSG_SET_AGCTH ����AGCͼ����������ֵ
* @param [in] wParam ����ֵ
* @param [in] lParam ����
* @return �ɹ�����S_OK������ֵ����ʧ��
*/
HRESULT CSWCameralControlMSG::OnSetAGCTh( WPARAM wParam,LPARAM lParam )
{
    m_pSWAgcAwbTask->SetAGCTh( wParam ); return S_OK;
}

/**
* @brief MSG_SET_AGCZONE �豸AGC�������
* @param [in] wParam AGC�������ʹ�ܱ�־INT���ݣ�16�����ݡ�
* @param [in] ����
* @return �ɹ�����S_OK������ֵ����ʧ��
*/
HRESULT CSWCameralControlMSG::OnSetAGCZone(WPARAM wParam,LPARAM lParam)
{
	INT* piZone = (INT*)wParam;
	if( piZone != NULL )
	{
		for(int i = 0; i < 16; ++i)
		{
			CSWImageStatisticsInfo::SetImageListInfo((piZone[i] == 1), i);
		}
		SW_TRACE_DEBUG("<CameralControlMSG>set agc zone.\n");
		return S_OK;
	}
	return E_INVALIDARG;
}

/**
* @brief MSG_SET_AGCTH ��ȡAGCͼ����������ֵ
* @param [in] wParam ����
* @param [out] lParam ��INT*���ͣ� ����ֵ����
* @return �ɹ�����S_OK������ֵ����ʧ��
*/
HRESULT CSWCameralControlMSG::OnGetAGCTh( WPARAM wParam,LPARAM lParam )
{
    m_pSWAgcAwbTask->GetAGCTh( *((INT*)lParam));return S_OK;
}
/**
* @brief ��Ϣ���պ���
* @param [in] wParam ����ֵ(��200)
* @param [in] lParam ����
* @return �ɹ�����S_OK������ֵ����ʧ��
*/
HRESULT CSWCameralControlMSG::OnSetAGCGain(WPARAM wParam,LPARAM lParam)
{
   SWFC_DEBUG_ARG("(%d,%d)", wParam, lParam);
   m_pSWAgcAwbTask->SetAGCGain(wParam) ;
   return S_OK;
}

/**
* @brief ��Ϣ���պ���
* @param [in] wParam ʹ����Ϣ��ʹ�ܱ�ǣ�0=��ʹ�ܣ�1 = ʹ��
* @param [in] ����
* @return �ɹ�����S_OK������ֵ����ʧ��
*/
HRESULT CSWCameralControlMSG::OnSetAGCEnable(WPARAM wParam,LPARAM lParam)
{
	SWFC_DEBUG_ARG("(%d,%d)", wParam, lParam);
    m_pSWAgcAwbTask->SetAGCEnable( wParam );

    if( wParam == 1 ) m_pSWAgcAwbTask->Signal( CSWTask::kStartEvent  );

    return S_OK;
}

HRESULT CSWCameralControlMSG::OnGetAGCEnable(WPARAM wParam,LPARAM lParam)
{
	SWFC_DEBUG_ARG("(%d,%d)", wParam, lParam);
	*(INT *)lParam = m_pSWAgcAwbTask->GetAGCEnable();
	return S_OK;
}

/**
* @brief ��Ϣ���պ���
* @param [in] wParam ʹ����Ϣ��ʹ�ܱ�ǣ�0=��ʹ�ܣ�1 = ʹ��
* @param [in] ����
* @return �ɹ�����S_OK������ֵ����ʧ��
*/
HRESULT CSWCameralControlMSG::OnSetAWBEnable(WPARAM wParam,LPARAM lParam)
{
    SWFC_DEBUG_ARG("(%d,%d)", wParam, lParam);
    m_pSWAgcAwbTask->SetAWBEnable( wParam );

    if( wParam == 1 ) m_pSWAgcAwbTask->Signal( CSWTask::kStartEvent  );

    return S_OK;
}

HRESULT CSWCameralControlMSG::OnGetAWBEnable(WPARAM wParam,LPARAM lParam)
{
	SWFC_DEBUG_ARG("(%d,%d)", wParam, lParam);
	*(INT *)lParam = m_pSWAgcAwbTask->GetAWBEnable();
	return S_OK;
}


/**
* @brief ��Ϣ���պ���
* @param [in] wParam ����
* @param [out] lParam��INT*���ͣ� ���淵��ֵ(200)
* @return �ɹ�����S_OK������ֵ����ʧ��
*/
HRESULT CSWCameralControlMSG::OnGetAGCGain(WPARAM wParam,LPARAM lParam)
{
    m_pSWAgcAwbTask->GetAGCGain( *((INT*)lParam) );

    return S_OK;
}


/**
* @brief ��Ϣ���պ���
* @param [in] wParam RGB����(0��7λΪBͨ��ֵ��8��15λGͨ��ֵ��16��23ΪRͨ��ֵ,����:0x00AFAAFF
* ,�ֱ�ΪFFΪBͨ�����棬AAΪGͨ�����棬AFΪRͨ������ֵ)
* ����ʹ����Ϣ����Ϊʹ�ܱ��0=��ʹ�ܣ�1 = ʹ��
* @param [in] lParam ����
* @return �ɹ�����S_OK������ֵ����ʧ��
*/
HRESULT CSWCameralControlMSG::OnSetRGBGain(WPARAM wParam,LPARAM lParam)
{
    SWFC_DEBUG_ARG("(%d,%d)", wParam, lParam);
    m_pSWAgcAwbTask->SetAWBGain( (DWORD*)wParam );
    return S_OK;
}
/**
* @brief ��Ϣ���պ���
* @param [in] wParam ����
* @param [out] lParam(DWORD*����) RGB����(0��7λΪBͨ��ֵ��8��15λGͨ��ֵ��16��23ΪRͨ��ֵ,����:0x00AFAAFF
* ,�ֱ�ΪFFΪBͨ�����棬AAΪGͨ�����棬AFΪRͨ������ֵ)
* @return �ɹ�����S_OK������ֵ����ʧ��
*/
HRESULT CSWCameralControlMSG::OnGetRGBGain(WPARAM wParam,LPARAM lParam)
{
    m_pSWAgcAwbTask->GetAWBGain( *((DWORD*)lParam) );
    return S_OK;
}

/**
* @brief ��Ϣ���պ���
* @param [in] wParam ����ֵ
* @param [in] lParam ����
* @return �ɹ�����S_OK������ֵ����ʧ��
*/
HRESULT CSWCameralControlMSG::OnSetShutter(WPARAM wParam,LPARAM lParam)
{
    SWFC_DEBUG_ARG("(%d,%d)", wParam, lParam);
    m_pSWAgcAwbTask->SetAGCShutter(wParam) ; return S_OK;
}

/**
* @brief MSG_SHUTTER ��ȡ����ָ��
* @param [in] wParam ����
* @param [out] lParam��INT*���ͣ� ����ֵ
* @return �ɹ�����S_OK������ֵ����ʧ��
*/
HRESULT CSWCameralControlMSG::OnGetShutter(WPARAM wParam,LPARAM lParam)
{
    m_pSWAgcAwbTask->GetAGCShutter( *((INT*)lParam) ); return S_OK;
}


/**
* @brief ��Ϣ���պ���
* @param [in] wParam ʹ����Ϣ��ʹ�ܱ�ǣ�0=��ʹ�ܣ�1 = ʹ��
* @param [in] lParam ����
* @return �ɹ�����S_OK������ֵ����ʧ��
*/
HRESULT CSWCameralControlMSG::OnSetDiffCorrEnable(WPARAM wParam,LPARAM lParam)
{
    SWFC_DEBUG_ARG("(%d,%d)", wParam, lParam);
	SW_TRACE_DEBUG("<cameramsg>set video diff %d.", (INT)wParam);
    return m_pSWChanneCorrTask->Signal((wParam == 1
                    ? CSWTask::kStartEvent : CSWTask::kIdleEvent));
}

/**
* @brief ��Ϣ���պ���
* @param [in] wParam ʹ����Ϣ��ʹ�ܱ�ǣ�0=��ʹ�ܣ�1 = ʹ��
* @param [in] lParam ����
* @return �ɹ�����S_OK������ֵ����ʧ��
*/
HRESULT CSWCameralControlMSG::OnSetCaptureEnable(WPARAM wParam,LPARAM lParam)
{
	SWFC_DEBUG_ARG("(%d,%d)", wParam, lParam);
	SW_TRACE_DEBUG("<cameramsg>set captrue diff %d.", (INT)wParam);
    return m_pSWCapture->Signal((wParam == 1
                    ? CSWTask::kStartEvent : CSWTask::kIdleEvent));
}
/**
* @brief ��Ϣ���պ���
* @param [in] wParam ����ֵ
* @param [in] lParam ����
* @return �ɹ�����S_OK������ֵ����ʧ��
*/
HRESULT CSWCameralControlMSG::OnSetCaptureShutter(WPARAM wParam,LPARAM lParam)
{
    SWFC_DEBUG_ARG("(%d,%d)", (*(INT*)wParam), lParam);
    m_pSWCapture->SetShutter( (*(INT*)wParam) ); return S_OK;
}

/**
* @brief ��Ϣ���պ���
* @param [in] wParam ����
* @param [out] lParam(INT*����) ����ֵ
* @return �ɹ�����S_OK������ֵ����ʧ��
*/
HRESULT CSWCameralControlMSG::OnGetCaptureShutter(WPARAM wParam,LPARAM lParam)
{
    SWFC_DEBUG_ARG("(%d,%d)", wParam, (INT*)lParam);
    m_pSWCapture->GetShutter((*(INT*)lParam));
	return S_OK;
}
/**
* @brief ��Ϣ���պ���
* @param [in] wParam ����ֵ(200)
* @param [in] lParam ����
* @return �ɹ�����S_OK������ֵ����ʧ��
*/
HRESULT CSWCameralControlMSG::OnSetCaptureGain(WPARAM wParam,LPARAM lParam)
{
    SWFC_DEBUG_ARG("(%d,%d)", (*((INT*)wParam)), lParam);
     m_pSWCapture->SetGain( (*((INT*)wParam)) );
	 // ץ�������ʹ��Ҫ��ץ�Ĳ���У��ͬ����
	 // ʹ��ץ�Ĳ�����У��
	 if( (*((INT*)wParam)) != -1 )
	 {
		 OnSetCaptureEnable( 1 , 0);
	 }
	 return S_OK;
}
/**
* @brief ��Ϣ���պ���
* @param [in] wParam ����
* @param [out] lParam(INT* ����) ����ֵ
* @return �ɹ�����S_OK������ֵ����ʧ��
*/
HRESULT CSWCameralControlMSG::OnGetCaptureGain(WPARAM wParam,LPARAM lParam)
{
    SWFC_DEBUG_ARG("(%d,%d)", wParam, lParam);
    m_pSWCapture->GetGain( *((INT*)lParam )); return S_OK;
}

/**
* @brief ��Ϣ���պ���
* @param [in] wParam RGB��������.0:R,1:G,2:B
* @param [in] lParam ����
* note �����ת�ĳ�(0��7λΪBͨ��ֵ��8��15λGͨ��ֵ��16��23ΪRͨ��ֵ,����:0x00AFAAFF
*   �ֱ�ΪFFΪBͨ�����棬AAΪGͨ�����棬AFΪRͨ������ֵ)
* @return �ɹ�����S_OK������ֵ����ʧ��
*/
HRESULT CSWCameralControlMSG::OnSetCaptureRGBGain(WPARAM wParam,LPARAM lParam)
{
	INT* piRGB = (INT*)wParam;
	if( piRGB != NULL )
	{
		INT iRGB = 0;
		if( piRGB[0] == -1 || piRGB[1] == -1 || piRGB[2] == -1 )
		{
			iRGB = 0xFF000000;
		}
		else
		{
			iRGB = (((piRGB[0] & 0x000000FF) << 16) | ((piRGB[1] & 0x000000FF) << 8) | (piRGB[2] & 0x000000FF));
		}
		SW_TRACE_DEBUG("<CameralControlMSG>Setcapture RGB:(%d,%d,%d)==>0x%08x.\n", piRGB[0], piRGB[1], piRGB[2], iRGB);
		m_pSWCapture->SetRGBGain( iRGB );
		return S_OK;
	}
	return E_FAIL;
}
/**
* @brief ��Ϣ���պ���
* @param [in] wParam ����
* @param [out] lParam(INT��������)0:R,1:G,2:B
* @return �ɹ�����S_OK������ֵ����ʧ��
*/
HRESULT CSWCameralControlMSG::OnGetCaptureRGBGain(WPARAM wParam,LPARAM lParam)
{
    SWFC_DEBUG_ARG("(%d,%d)", wParam, lParam);
	INT* piRGB = (INT*)lParam;
	if( NULL != piRGB )
	{
		DWORD dwRGB = 0;
		m_pSWCapture->GetRGBGain(dwRGB);

		piRGB[0] = ((dwRGB & 0x00FF0000) >> 16);
		piRGB[1] = ((dwRGB & 0x0000FF00) >> 8);
		piRGB[2] = (dwRGB & 0x000000FF);
		return S_OK;
	}
	return E_FAIL;
}
/**
* @brief MSG_SET_SOFTTOUCH,����ץ������
* @param [in] wParam ��1�ֽ�Ϊ�������������ϣ�Ŀǰ֧��3·����ƣ��ӵ�λ��ʼ
* ����һλ��Ӧ��һ·����ƣ��ڶ�λ��Ӧ�ڶ�·����ơ�����λΪ1���ʾ���ø�·����ƣ�
* @param [in] wParam ��3�ֽ�Ϊץ����Ϣ����Ӧ��ץ��ͼ����д���Ϣ��������ץ��ͼ�ı�ʶ��
* ��Ӳ������֡��Ϊ�ۼӷ�ʽ
*  ����: 0x00000013,1Ϊ�������Ϣ��3Ϊץ����Ϣ
* @param [in] lParam ����
* @return �ɹ�����S_OK������ֵ����ʧ��
*/
HRESULT CSWCameralControlMSG::OnSetCaptureSofttouch(WPARAM wParam,LPARAM lParam)
{
	int iSet = 0;
	int iInfo = 0;
	iSet = ( wParam & 0xFF000000 ) >> 24;
	iInfo =( wParam & 0X00FFFFFF);
	if(iSet == 0 || iSet > 7 ){
		iSet = 7;
	}

	SW_TRACE_DEBUG("<CameralControlMSG>set:%d, info:%d.\n", iSet, iInfo);
    return swpa_camera_io_soft_capture(iSet , iInfo);
}


/**
* @brief ����Ƶ���ź�ʹ�ܡ�
* @param [in] wParam 0��1
* @param [in] lParam ����
* @return �ɹ�����S_OK������ֵ����ʧ��
*/
HRESULT CSWCameralControlMSG::OnSetFlashRateEnable(WPARAM wParam,LPARAM lParam)
{
	INT iEnable = (INT)wParam;
	SW_TRACE_DEBUG("<CameralControlMSG>SetFlashRateEnable:%d.\n", iEnable);
	return (0 == swpa_camera_io_set_strobe_signal(iEnable)) ? S_OK : E_FAIL;
}

/**
* @brief ����ץ���ź�ʹ��
* @param [in] wParam 0��1
* @param [in] lParam ����
* @return �ɹ�����S_OK������ֵ����ʧ��
*/
HRESULT CSWCameralControlMSG::OnSetCaptrueSyncEnable(WPARAM wParam,LPARAM lParam)
{
	INT iEnable = (INT)wParam;
	SW_TRACE_DEBUG("<CameralControlMSG>SetCaptrueSyncEnable:%d.\n", iEnable);
	INT iValue = (iEnable == 1) ? 7 : 0;
	return (0 == swpa_camera_io_set_flash_signal(iValue)) ? S_OK : E_FAIL;
}

/**
* @brief ����Ƶ������
* @param [in] wParam ���� 100~20000����λ΢��
* @param [in] lParam ����
* @return �ɹ�����S_OK������ֵ����ʧ��
	*/
HRESULT CSWCameralControlMSG::OnSetFlashRatePulse(WPARAM wParam,LPARAM lParam)
{
	INT iValue = (INT)wParam;
	iValue /= 100;
	INT iFlashRateMultiplication = 0;
	INT iFlashRatePolarity = 0;
	INT iFlashRatePulseWidth = 0;
	INT iFlashRateOutputType = 0;
	INT iCaptureSynCoupling = 0;

	HRESULT hr = E_FAIL;

	if( 0 == swpa_camera_io_get_strobe_param(
		&iFlashRateMultiplication,
		&iFlashRatePolarity,
		&iFlashRatePulseWidth,
		&iFlashRateOutputType,
		&iCaptureSynCoupling) )
	{
		if( 0 == swpa_camera_io_set_strobe_param(
			iFlashRateMultiplication,
			iFlashRatePolarity,
			iValue,
			iFlashRateOutputType,
			iCaptureSynCoupling) )
		{
			hr = S_OK;
		}
	}

	SW_TRACE_DEBUG("<CameralControlMSG>SetFlashRatePulse:%d. ret=0x%08x\n", iValue, hr);

	return hr;
}


/**
* @brief MSG_SET_MAXSHU_GAIN,�����豸��Ϣ
* @param [in] wParam ��16λ������
* @param [in] wParam ��16λΪ�������
*  ����: 0x100300C8,1003Ϊ�����ţ�00C8Ϊ�������
* @param [in] lParam ����
* @return �ɹ�����S_OK������ֵ����ʧ��
*/
HRESULT CSWCameralControlMSG::OnSetAGCMaxShuAndGain(WPARAM wParam,LPARAM lParam)
{
    DWORD dwMaxShutter = ((0xFFFF0000&wParam) >> 16);
    DWORD dwMaxGain = (0x0000FFFF&wParam);

    SWFC_DEBUG_ARG("(%d,%d)", dwMaxShutter, dwMaxGain);

   m_pSWAgcAwbTask->SetAGCMaxParam( (INT)dwMaxShutter , (INT)dwMaxGain );

   return S_OK;
}


/**
* @brief MSG_GET_MAXSHU_GAIN,�����豸��Ϣ
* @param [in] wParam ����
* @param [in] lParam lParam(DWORD*����) ��16λ������
* @param [in] lParam lParam(DWORD*����) ��16λΪ�������
*  ����: 0x100300C8,1003Ϊ�����ţ�00C8Ϊ�������
* @return �ɹ�����S_OK������ֵ����ʧ��
*/
HRESULT CSWCameralControlMSG::OnGetAGCMaxShuAndGain( WPARAM wParam,LPARAM lParam )
{
    DWORD dwMaxShuttor = 0 ; DWORD dwMaxGain = 0 ;
    m_pSWAgcAwbTask->GetAGCParam( dwMaxShuttor , dwMaxGain );

    *((DWORD*)lParam) = ( dwMaxShuttor << 16 );
    *((DWORD*)lParam) |= dwMaxGain;

    return S_OK;
}

/**
* @brief MSG_SET_MINSHU_GAIN,�����豸��Ϣ
* @param [in] wParam ��16λ��С����
* @param [in] wParam ��16λΪ��С����
*  ����: 0x100300C8,1003Ϊ�����ţ�00C8Ϊ�������
* @param [in] lParam ����
* @return �ɹ�����S_OK������ֵ����ʧ��
*/
HRESULT CSWCameralControlMSG::OnSetAGCMinShuAndGain(WPARAM wParam,LPARAM lParam)
{
    DWORD dwMinShutter = ((0xFFFF0000&wParam) >> 16);
    DWORD dwMinGain = (0x0000FFFF&wParam);

    m_pSWAgcAwbTask->SetAGCMinParam( dwMinShutter , dwMinGain );
}

/**
* @brief MSG_GET_MINSHU_GAIN,�����豸��Ϣ
* @param [in] wParam ����
* @param [in] lParam lParam(DWORD*����) ��16λ��С����
* @param [in] lParam lParam(DWORD*����) ��16λΪ��С����
*  ����: 0x100300C8,1003Ϊ�����ţ�00C8Ϊ�������
* @return �ɹ�����S_OK������ֵ����ʧ��
*/
HRESULT CSWCameralControlMSG::OnGetAGCMinShuAndGain(WPARAM wParam,LPARAM lParam)
{
    DWORD dwMinShuttor = 0 ; DWORD dwinGain = 0 ;
    m_pSWAgcAwbTask->GetAGCMinParam( dwMinShuttor , dwinGain );

    *((DWORD*)lParam) = ( dwMinShuttor << 16 );
    *((DWORD*)lParam) |= dwinGain;

    return S_OK;
}



HRESULT CSWCameralControlMSG::OnSetAGCParam(WPARAM wParam,LPARAM lParam)
{
	DWORD* pdwParam = (DWORD*)wParam;

	m_pSWAgcAwbTask->SetAGCMinParam( pdwParam[0] , pdwParam[2] );
	m_pSWAgcAwbTask->SetAGCMaxParam( pdwParam[1] , pdwParam[3] );

	return S_OK;
}



 /**
* @brief MSG_SET_DCAPERTUREENABLE,ʹ��DC��Ȧ
* @param [in] wParam 1Ϊʹ�ܣ�0Ϊ��ʹ��
* @param [in] lParam ����
* @return �ɹ�����S_OK������ֵ����ʧ��
*/
HRESULT CSWCameralControlMSG::OnSetDCApertureEnable(WPARAM wParam,LPARAM lParam)
{
		SW_TRACE_DEBUG("CSWCameralControlMSG::OnSetDCApertureEnable(0x%08x, 0x%08x)", wParam, lParam);
    m_pSWAgcAwbTask->SetAutoAperture( wParam ); return S_OK;
}
/**
* @brief MSG_SET_FILTERSWITCH �˹�Ƭ�л�
* @param [in] wParam 1�����˹�Ƭ��2�����˹�Ƭ
* @param [in] lParam ����
* @return �ɹ�����S_OK������ֵ����ʧ��
*/
HRESULT CSWCameralControlMSG::OnSetFilterSwitch(WPARAM wParam,LPARAM lParam)
{
    // ��ʼ���ⲿ�豸
    m_pSWExtDevControl->FilterSwitch( wParam );
}


/**
* @brief MSG_SET_REDLIGHT_RECT ���ú�ƼӺ�����
* @param [in] wParam SW_RECT*
* @param [in] lParam INT ��ƼӺ���������
* @return �ɹ�����S_OK������ֵ����ʧ��
*/
HRESULT CSWCameralControlMSG::OnSetRedLightRect(WPARAM wParam,LPARAM lParam)
{
	// ��ʼ���ⲿ�豸
	return m_pSWExtDevControl->SetRedLightRect((SW_RECT*)wParam, (INT)lParam);
}

/**
* @brief MSG_SET_CAPTUREEDGE ���ô���ץ����
* @param [in] wParam 0~3
* @param [out] ����
* @return �ɹ�����S_OK������ֵ����ʧ��
*/
HRESULT CSWCameralControlMSG::OnSetCaptureEdge(WPARAM wParam,LPARAM lParam)
{
    return m_pSWExtDevControl->SetCaptureEdge((int)wParam);
}

/**
* @brief MSG_GET_CAPTUREEDGE ��ȡ����ץ����
* @param [in] ����
* @param [out] lParam 0~3
* @return �ɹ�����S_OK������ֵ����ʧ��
*/
HRESULT CSWCameralControlMSG::OnGetCaptureEdge(WPARAM wParam,LPARAM lParam)
{
    return m_pSWExtDevControl->GetCaptureEdge((int*)(&lParam));
}

/*
* @brief MSG_GET_BARRIER_STATE ��ȡ���˻�״̬
* @param [in] ����
* @param [out] lParam ��INT��
* @return �ɹ�����S_OK������ֵ����ʧ��
*/
HRESULT CSWCameralControlMSG::OnGetBarrierState(WPARAM wParam,LPARAM lParam)
{
    return m_pSWExtDevControl->GetBarrierState((int*)(lParam));
}


HRESULT CSWCameralControlMSG::OnGetPTZInfo(PVOID pvBuffer, INT iSize)
{
    SW_TRACE_DEBUG("Info: got GetPTZInfo msg...\n");

    //send remote msg to notify the ptz info
    PTZ_INFO sInfo;

	swpa_memset(&sInfo, 0, sizeof(sInfo));

	/* not support any PTZ feature */
        
    return SendRemoteMessage(MSG_APP_REMOTE_SET_PTZ_INFO, (PVOID)&sInfo, sizeof(sInfo));
}


