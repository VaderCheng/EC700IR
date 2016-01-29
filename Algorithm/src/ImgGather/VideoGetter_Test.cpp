// ���ļ������ʽ������WINDOWS-936��ʽ

#include "ImgGatherer.h"
#include "HvSockUtils.h"
#include "hvtarget_ARM.h"

using namespace HiVideo;

CVideoGetter_Test::CVideoGetter_Test()
{
    m_cPlateLightType = DAY;
    m_fIsLightTypeChanged = false;
    m_dwImgWidth = 0;
    m_dwImgHeight = 0;
    m_dwFrameCount = 0;
    m_fIsCamConnected = FALSE;
}

CVideoGetter_Test::~CVideoGetter_Test()
{
    Stop(-1);
    m_cTestLink.Close();
}

HRESULT CVideoGetter_Test::SetCamCfgParam(LPVOID pCfgCamParam)
{
    if (pCfgCamParam == NULL)
    {
        return E_POINTER;
    }

    m_pCfgCamParam = (CAM_CFG_PARAM *)pCfgCamParam;
    return S_OK;
}

HRESULT CVideoGetter_Test::Run(void* pvParam)
{
    DWORD32 dwImgTime = 0;
    char szFramName[260] = {0};

    m_dwLastTime = GetSystemTick();
    m_cTestLink.Connect(m_pCfgCamParam->szIP, (WORD16)m_pCfgCamParam->nTestProtocolPort);

    IMG_FRAME frame;

    while (!m_fExit)
    {
        m_dwLastTime = GetSystemTick();
        if (!m_cTestLink.IsConnect())
        {
            if (S_OK != m_cTestLink.Connect(m_pCfgCamParam->szIP, (WORD16)m_pCfgCamParam->nTestProtocolPort))
            {
                m_fIsCamConnected = FALSE;
                SetLan1LedOff();
                HV_Sleep(4000);
            }
            continue;
        }

        // ����JPEG���ö���
        IReferenceComponentImage *pRefImage;
        HRESULT hr = CreateReferenceComponentImage(
                         &pRefImage,
                         HV_IMAGE_JPEG,
                         1600,
                         1200,
                         m_dwFrameCount++,
                         0,
                         0,
                         "NULL",
                         2
                     );
        if (S_OK == hr)
        {
            szFramName[0] = 0;
            HV_COMPONENT_IMAGE imgFrame;
            pRefImage->GetImage(&imgFrame);
            hr = m_cTestLink.GetImage(
                     GetHvImageData(&imgFrame, 0),
                     (DWORD32*)&imgFrame.iWidth,
                     &dwImgTime,
                     szFramName
                 );

            if (hr != S_OK)
            {
                pRefImage->Release();
                continue;
            }

            //����Э��ͼƬ�߿�������Ƶ֡�����8���ֽ���
            if (m_dwImgWidth == 0 || m_dwImgHeight == 0)
            {
                memcpy(&m_dwImgWidth, &szFramName[120], 4);
                memcpy(&m_dwImgHeight, &szFramName[124], 4);
            }

            if (m_dwImgWidth == 0 || m_dwImgHeight == 0)
            {
                m_dwImgWidth = 1600;
                m_dwImgHeight = 1200;
                HV_Trace(5, "can not calc image size, use default size(1600*1200)\n");
            }

            imgFrame.iHeight = (m_dwImgWidth | (m_dwImgHeight << 16));

            pRefImage->SetImageSize(imgFrame);
            pRefImage->SetFrameName(szFramName);
            pRefImage->SetRefTime(dwImgTime);
            m_fIsCamConnected = TRUE;

            Lan1LedLight();

            // ���ݵ��������
            frame.pRefImage = pRefImage;
            frame.iVideoID = 0;
            PutOneFrame(frame);
            pRefImage->Release();
        }
        else
        {
            HV_Trace(5, "not enough share memory to alloc jpg buffer\n");
        }
    }

    m_cTestLink.Close();
    m_fExit = TRUE;
    SetLan1LedOff();

    return S_OK;
}

HRESULT CVideoGetter_Test::GetCurStatus(char* pszStatus, int nStatusSizes)
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
