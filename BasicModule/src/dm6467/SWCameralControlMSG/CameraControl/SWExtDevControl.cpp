#include "SWFC.h"
#include "SWExtDevControl.h"

CSWExtDevControl::CSWExtDevControl()
{
    //ctor
	m_iFlashDifferentLaneExt = 0;
}

CSWExtDevControl::~CSWExtDevControl()
{
    //dtor
}

void CSWExtDevControl::InitialExtDevParam(LPCAMERAPARAM_INFO pCameraParam)
{
    if (pCameraParam == NULL)
        return;

    // ���²���ֻ��Ҫ��ʼ��ʱ���ü���
    // Ƶ��ͬ���ź�ʹ��
    swpa_camera_io_set_strobe_signal(pCameraParam->iFlashRateSynSignalEnable);
    // Ƶ������Ƿ�Ƶ��1����Ƶ���
    swpa_camera_io_set_strobe_param(pCameraParam->iFlashRateMultiplication
        , pCameraParam->iFlashRatePolarity
        , pCameraParam->iFlashRatePulseWidth
        , pCameraParam->iFlashRateOutputType
        , pCameraParam->iCaptureSynCoupling);

    INT iEnable = 0;
    if (pCameraParam->iCaptureSynSignalEnable == 1)
    {
        iEnable = 7;
    }
    swpa_camera_io_set_flash_signal(iEnable);

    // ʹ��ץ�ĵ�
    swpa_camera_io_set_flash_param(1
        , pCameraParam->iCaptureSynCoupling
        , pCameraParam->iCaptureSynPolarity
        , pCameraParam->iCaptureSynPulseWidth
        , pCameraParam->iCaptureSynOutputType);

    swpa_camera_io_set_flash_param(2
        , pCameraParam->iCaptureSynCoupling
        , pCameraParam->iCaptureSynPolarity
        , pCameraParam->iCaptureSynPulseWidth
        , pCameraParam->iCaptureSynOutputType);

    if (pCameraParam->fEnableTriggerOut)
    {
        swpa_camera_io_set_flash_param(3
            , 0
            , pCameraParam->nTriggerOutNormalStatus
            , 0
            , pCameraParam->iCaptureSynOutputType);
    }
    else
    {
        swpa_camera_io_set_flash_param(3
            , pCameraParam->iCaptureSynCoupling
            , pCameraParam->iCaptureSynPolarity
            , pCameraParam->iCaptureSynPulseWidth
            , pCameraParam->iCaptureSynOutputType);
    }

    // ���ú�ƼӺ������
    swpa_camera_imgproc_set_trafficlight_enhance_param(
        pCameraParam->iHThreshold,
        pCameraParam->iLThreshold,
        pCameraParam->iSThreshold,
        pCameraParam->iColorFactor,
        pCameraParam->iLumLThreshold,
        pCameraParam->iLumLFactor,
        pCameraParam->iLumHFactor);

    // ���õ���ͬ����
    SetExternSync(pCameraParam->iExternalSyncMode, pCameraParam->iSyncRelay);
    // ����Ӳ��������
    int itrigger1, itrigger2;
    m_iFlashDifferentLaneExt = pCameraParam->iFlashDifferentLaneExt;
    if (1 == pCameraParam->iFlashDifferentLaneExt)
    {
        itrigger1 = 1;
        itrigger2 = 2;
    }
    else
    {
        itrigger1 = itrigger2 = 7;
    }
    swpa_camera_io_set_capture_param(pCameraParam->iCaptureEdgeOne, pCameraParam->iCaptureEdgeTwo, itrigger1, itrigger2);
}

HRESULT CSWExtDevControl::FilterSwitch(DWORD dwFilterType)
{
    // �л��˹�Ƭ
    swpa_camera_io_control_filter(dwFilterType);
    swpa_thread_sleep_ms(600);

    // ���õ��ֹͣ
    return swpa_camera_io_control_filter(0);
}

// ���ú�ƼӺ�����
HRESULT CSWExtDevControl::SetRedLightRect(SW_RECT* pRect, INT iCount)
{
    if (0 == pRect)
    {
        return E_INVALIDARG ;
    }

    INT iRectCount = (INT)iCount;
    const INT MAX_RECT_COUNT = 8;
    iRectCount = (iRectCount > MAX_RECT_COUNT || iRectCount < 0) ? 0 : iRectCount;
    INT iRet = 0;
    for (INT i = 0; i < iRectCount; ++i)
    {
        iRet = swpa_camera_imgproc_set_trafficlight_enhance_zone(i, pRect[i].left, pRect[i].top, pRect[i].right, pRect[i].bottom);
    }

    SW_TRACE_DEBUG("<SWExtDevControl>SetRedRect count:%d,ret=%d.", iRectCount, iRet);
    return S_OK ;
}

// ���õ���ͬ��ģʽ
HRESULT CSWExtDevControl::SetExternSync(INT iMode, INT iRelayUs)
{
    if (iMode <= 0 || iMode > 2)
    {
        return S_OK ;
    }

    INT iValue = (iRelayUs <= 0) ? 0 : (iRelayUs / 100);

    INT iRet = swpa_camera_io_set_framerate_mode(iMode, iValue);
    SW_TRACE_DEBUG("<ExtDevControl>set extern sync mode:%d,%d, return:%d.", iMode, iValue, iRet);

    return iRet == 0 ? S_OK : E_FAIL ;
}

HRESULT CSWExtDevControl::SetCaptureEdge(int nValue)
{
    int nMode1, nMode2;
    nMode1 = nMode2 = nValue;

    int itrigger1, itrigger2;
    itrigger1 = itrigger2 = 7;
    if (1 == m_iFlashDifferentLaneExt)
    {
        itrigger1 = 1;
        itrigger2 = 2;
    }
    else
    {
        itrigger1 = itrigger2 = 7;
    }

    if (0 != swpa_camera_io_set_capture_param(nMode1, nMode2, itrigger1, itrigger2))
    {
        return E_FAIL;
    }

    return S_OK;
}

HRESULT CSWExtDevControl::GetCaptureEdge(int *pnValue)
{
    // TODO:��Ҫ���Ƿֳ��������
    int nMode1, nMode2, nLane;
    if (0 != swpa_camera_io_get_capture_param(&nMode1, &nMode2, &nLane))
    {
        return E_FAIL;
    }
    (*pnValue) = nMode1;
    return S_OK;
}

HRESULT CSWExtDevControl::GetBarrierState(int *pnValue)
{
    int iStatus = 0;
    if (0 != swpa_camera_io_get_barrier_state(&iStatus))
    {
        return E_FAIL;
    }
    (*pnValue) = iStatus;
    return S_OK;
}


