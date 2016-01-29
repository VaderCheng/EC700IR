// ���ļ����������WINDOWS-936��ʽ

#include "PciResultSender.h"
#include "tinyxml.h"

/* PciDataRecvThread_Slave.cpp */
extern int g_iSendSlaveImage;

/* ImgProcesser.cpp */
extern HRESULT AllImageAddRef(RESULT_IMAGE_STRUCT *pResultImage);
extern HRESULT AllImageRelease(RESULT_IMAGE_STRUCT *pResultImage);

CPciResultSender::CPciResultSender()
        : m_dwCarID(0)
{
    m_dwLastTick = 0;
    m_nCamType = 0;
    CreateSemaphore(&m_hSemQueue, 0, MAX_QUEUE_LIST_COUNT);
    CreateSemaphore(&m_hSemResult, 1, 1);
    CreateSemaphore(&m_hSemVideo, 1, 1);
}

CPciResultSender::~CPciResultSender()
{
    DestroySemaphore(&m_hSemQueue);
    DestroySemaphore(&m_hSemVideo);
    DestroySemaphore(&m_hSemResult);
}

HRESULT CPciResultSender::Run(void* pvParam)
{
    static int iTotolFaildTimes = 0;
    while (!m_fExit)
    {
        m_dwLastTick = GetSystemTick();
        if (SemPend(&m_hSemQueue, 1000) != S_OK)
        {
            continue;
        }

        LockResult();
        if (m_rgResultInfo.GetSize() != 0)
        {
            RECOG_RESULT_INFO* pResultInfo = m_rgResultInfo.RemoveHead();
            UnLockResult();
            if (pResultInfo != NULL)
            {
                HRESULT hr;
                int i;
                for (i = 0; i < 1; i++)
                {
                    hr = SendResult(pResultInfo);
                    if (hr == S_OK)
                    {
                        break;
                    }
                    HV_Sleep(50);
                }
                if (S_OK != hr)
                {
                    HV_Trace(5, "PCI send result failed! Try time = %d...\n", i);
                    //if (i >= 3)
                    {
                        iTotolFaildTimes++;
                        HV_Trace(5, "PCI send result failed Times = %d\n", iTotolFaildTimes);
                        if(iTotolFaildTimes >= 5)
                        {
                            HV_Exit(HEC_FAIL, "PCI send result failed 5 Times");
                        }
                        //HV_Exit(HEC_FAIL, "PCI send result failed");
                    }
                }
                else
                {
                    iTotolFaildTimes = 0;
                }
                delete pResultInfo;
            }
        }
        else
        {
            UnLockResult();
        }
        LockVideo();
        if (m_rgVideoInfo.GetSize() != 0)
        {
            VIDEO_RESULT_INFO* pVideoInfo = m_rgVideoInfo.RemoveHead();
            UnLockVideo();
            if (pVideoInfo != NULL)
            {
                HRESULT hr;
                int i;
                for (i = 0; i < 1; i++)
                {
                    hr = SendVideo(pVideoInfo);
                    if (hr == S_OK)
                    {
                        break;
                    }
                    HV_Sleep(50);
                }
                if (S_OK != hr)
                {
                    HV_Trace(5, "PCI send video failed! Try time = %d...\n", i);
                    //if (i >= 5)
                    {
                        HV_Exit(HEC_FAIL, "PCI send video failed");
                    }
                }
                delete pVideoInfo;
            }
        }
        else
        {
            UnLockVideo();
        }
    }
    return S_OK;
}

HRESULT CPciResultSender::SendVideo(VIDEO_RESULT_INFO* pVideoInfo)
{
    if (S_OK != ConvertToJpeg(&pVideoInfo->pimgVideo))
    {
        return E_FAIL;
    }

    int nPciHandle = -1;
    HRESULT hr = g_cHvPciLinkApi.BeginSendData(&nPciHandle, PCILINK_DEBUG_SLAVE_IMAGE);

    if (hr != S_OK)
    {
        return E_FAIL;
    }

    HV_COMPONENT_IMAGE img;
    //ͼƬ��Ϣ
    pVideoInfo->pimgVideo->GetImage(&img);
    g_cHvPciLinkApi.PutSendData(nPciHandle, &img, sizeof(HV_COMPONENT_IMAGE));

    PCI_PLATERECT_INFO cRectInfo;
    unsigned char *pbDest = cRectInfo.rgbRectInfo;
    // ��ת��־
    if (m_pResultSenderParam->iEddyType && m_nCamType > 0)
    {
        memcpy(pbDest, "EddyLeft", 8);
        pbDest += 8;
        cRectInfo.dwImageOffset += 8;
    }
    //������64��
    if (pVideoInfo->cTrackRectInfo.dwTrackCount > 64)
    {
        pVideoInfo->cTrackRectInfo.dwTrackCount = 64;
    }

    // �����־
    if (pVideoInfo->cTrackRectInfo.dwTrackCount && m_pResultSenderParam->iDrawRect)
    {
        memcpy(pbDest, "rect", sizeof(int));
        pbDest += sizeof(int);
        cRectInfo.dwImageOffset += sizeof(int);
        memcpy(pbDest, &pVideoInfo->cTrackRectInfo.dwTrackCount, sizeof(int));
        pbDest += sizeof(int);
        cRectInfo.dwImageOffset += sizeof(int);
        for (int i = 0; i < (int)pVideoInfo->cTrackRectInfo.dwTrackCount; i++)
        {
            pVideoInfo->cTrackRectInfo.rgTrackRect[i].top *= 2;
            pVideoInfo->cTrackRectInfo.rgTrackRect[i].bottom *= 2;
            memcpy(pbDest, &pVideoInfo->cTrackRectInfo.rgTrackRect[i], sizeof(HV_RECT));
            pbDest += sizeof(HV_RECT);
            cRectInfo.dwImageOffset += sizeof(HV_RECT);
        }
    }

    g_cHvPciLinkApi.PutSendData(nPciHandle, &cRectInfo, cRectInfo.dwImageOffset + sizeof(DWORD32));

    //ͼƬ����
    g_cHvPciLinkApi.PutSendData(nPciHandle, GetHvImageData(&img, 0), img.iWidth);

    hr = g_cHvPciLinkApi.EndSendData(nPciHandle);

    if (hr != S_OK)
    {
        HV_Exit(HEC_FAIL|HEC_RESET_DEV, "Send video to pci failed");
    }
    return hr;
}

HRESULT CPciResultSender::SendResult(RECOG_RESULT_INFO* pResultInfo)
{
    int nPciHandle = -1;
    HRESULT hr = g_cHvPciLinkApi.BeginSendData(&nPciHandle, PCILINK_RECOGNITION_RESULT);

    if (hr != S_OK)
    {
        return E_FAIL;
    }
    //�����Ϣ
    g_cHvPciLinkApi.PutSendData(nPciHandle, pResultInfo->szPlateInfo, MAX_PLATE_STRING_SIZE);

    IReferenceComponentImage *prgImage[5] =
    {
        pResultInfo->pimgBestSnapShot,
        pResultInfo->pimgLastSnapShot,
        pResultInfo->pimgBeginCapture,
        pResultInfo->pimgBestCapture,
        pResultInfo->pimgLastCapture
    };

    PCILINK_IMAGE_TYPE rgcImageType[5] =
    {
        PCILINK_IMAGE_BEST_SNAPSHOT,
        PCILINK_IMAGE_LAST_SNAPSHOT,
        PCILINK_IMAGE_BEGIN_CAPTURE,
        PCILINK_IMAGE_BEST_CAPTURE,
        PCILINK_IMAGE_LAST_CAPTURE
    };

    //ͼƬ����
    int width = 0, height = 0;
    if (m_pResultSenderParam->iBestSnapshotOutput)
    {
        //��ʱ���ν�ͼ���� �ƹ��� 2011-09-07
        /*if(m_pResultSenderParam->iSaveType == 1 || m_pResultSenderParam->iSaveType == 2)
        {
            width = m_pResultSenderParam->iWidth;
            height = m_pResultSenderParam->iHeight;
        }*/
        CopyBigImageToShareBuffer(
            prgImage[0],
            rgcImageType[0],
            pResultInfo->dwCarID,
            pResultInfo->rcBestPlatePos,
            pResultInfo->rcRedLightPos,
            pResultInfo->rcRedLightCount,
            pResultInfo->rcFacePos,
            0,
            nPciHandle
        );
    }
    width = 0, height = 0;
    if (m_pResultSenderParam->iLastSnapshotOutput)
    {
        /*if(m_pResultSenderParam->iSaveType == 1 || m_pResultSenderParam->iSaveType == 3)
        {
            width = m_pResultSenderParam->iWidth;
            height = m_pResultSenderParam->iHeight;
        }*/
        CopyBigImageToShareBuffer(
            prgImage[1],
            rgcImageType[1],
            pResultInfo->dwCarID,
            pResultInfo->rcLastPlatePos,
            pResultInfo->rcRedLightPos,
            pResultInfo->rcRedLightCount,
            pResultInfo->rcFacePos,
            pResultInfo->nFaceCount,
            nPciHandle
        );
    }
/*
    if (m_pResultSenderParam->iOutputCaptureImage
            || strstr(pResultInfo->szPlateInfo, "Υ��:��") != NULL
            || m_pResultSenderParam->iCapMode == 2
       )
*/
    {
        HV_RECT *rc[3] =
        {
            &pResultInfo->rcFirstPos,
            &pResultInfo->rcSecondPos,
            &pResultInfo->rcThirdPos
        };

        for (int i = 2; i < 5; i++)
        {
            CopyBigImageToShareBuffer(
                prgImage[i],
                rgcImageType[i],
                pResultInfo->dwCarID,
                *rc[i - 2],
                pResultInfo->rcRedLightPos,
                pResultInfo->rcRedLightCount,
                pResultInfo->rcFacePos,
                0,
                nPciHandle
            );
        }
    }
    CopySmallImageToShareBuffer(
        pResultInfo->pimgPlate,
        pResultInfo->dwTimeMsLow,
        pResultInfo->dwTimeMsHigh,
        pResultInfo->dwCarID,
        nPciHandle
    );
    CopyBinImageToShareBuffer(
        pResultInfo->pimgPlateBin,
        pResultInfo->dwTimeMsLow,
        pResultInfo->dwTimeMsHigh,
        pResultInfo->dwCarID,
        nPciHandle
    );
    hr = g_cHvPciLinkApi.EndSendData(nPciHandle, NULL, 0, 10000);

    if (S_OK != hr)
    {
        HV_Trace(5, "PCI send result failed...\n");
    }
    return hr;
}

HRESULT CPciResultSender::CopyBigImageToShareBuffer(
    IReferenceComponentImage *pImage,
    int nImageType,
    DWORD32 dwCarID,
    HV_RECT rcPlate,
    HV_RECT *rcRedLight,
    int nRedLightCount,
    HV_RECT *rcFacePos,
    int      nFaceCount,
    int nPciHandle
)
{
    if (!pImage)
    {
        return E_FAIL;
    }

    HV_COMPONENT_IMAGE imgTemp;
    DWORD32 dwTimeLow, dwTimeHigh;
    PCI_IMAGE_INFO cImageInfo;
    int nImgInfoSize = sizeof(PCI_IMAGE_INFO);

    pImage->GetImage(&imgTemp);

    ConvertTickToSystemTime(pImage->GetRefTime(), dwTimeLow, dwTimeHigh);

    //ͼƬ��Ϣ��С
    g_cHvPciLinkApi.PutSendData(nPciHandle, &nImgInfoSize, sizeof(int));

    //ͼƬ����
    cImageInfo.dwCarID = dwCarID;
    cImageInfo.dwImgType = nImageType;
    cImageInfo.dwTimeLow = dwTimeLow;
    cImageInfo.dwTimeHigh = dwTimeHigh;
    cImageInfo.dwImgWidth = ((imgTemp.iHeight & 0x0000FFFF) & (~0x1));
    cImageInfo.dwImgHeight = imgTemp.iHeight >> 16;

    if (m_pResultSenderParam->iEddyType && m_nCamType > 0)
    {
        cImageInfo.dwEddyType = 1;
    }

    cImageInfo.rcPlate = rcPlate;
    cImageInfo.nRedLightCount = nRedLightCount;
    if ( nRedLightCount > 0 )
    {
        memcpy(cImageInfo.rcRedLightPos, rcRedLight, nRedLightCount*sizeof(HiVideo::CRect));
    }

    cImageInfo.nFaceCount = nFaceCount;
    if ( nFaceCount > 0 )
    {
        memcpy(cImageInfo.rcFacePos , rcFacePos ,nFaceCount *sizeof(HiVideo::CRect));
    }


    g_cHvPciLinkApi.PutSendData(nPciHandle, &cImageInfo, sizeof(PCI_IMAGE_INFO));

    //ͼƬ���ݴ�С
    g_cHvPciLinkApi.PutSendData(nPciHandle, &imgTemp.iWidth, sizeof(int));

    //����ͼƬ����
    g_cHvPciLinkApi.PutSendData(nPciHandle, GetHvImageData(&imgTemp, 0), imgTemp.iWidth);
    return S_OK;
}

HRESULT CPciResultSender::CopySmallImageToShareBuffer(
    IReferenceComponentImage *pImage,
    DWORD32 dwTimeMsLow,
    DWORD32 dwTimeMsHigh,
    DWORD32 dwCarID,
    int nPciHandle
)
{
    if (!pImage)
    {
        return E_FAIL;
    }

    HV_COMPONENT_IMAGE imgTemp;
    PCI_IMAGE_INFO cImageInfo;
    int nImgInfoSize = sizeof(PCI_IMAGE_INFO);

    pImage->GetImage(&imgTemp);

    unsigned char *pY = GetHvImageData(&imgTemp, 0);
    unsigned char *pCb = GetHvImageData(&imgTemp,1);
    unsigned char *pCr = GetHvImageData(&imgTemp, 2);
    if ((pY == NULL) || (pCb == NULL) || (pCr == NULL))
    {
        return E_FAIL;
    }

    //ͼƬ��Ϣ��С
    g_cHvPciLinkApi.PutSendData(nPciHandle, &nImgInfoSize, sizeof(int));

    //ͼƬ����
    cImageInfo.dwCarID = dwCarID;
    cImageInfo.dwImgType = PCILINK_IMAGE_SMALL_IMAGE;
    cImageInfo.dwTimeLow = dwTimeMsLow;
    cImageInfo.dwTimeHigh = dwTimeMsHigh;
    cImageInfo.dwImgWidth = imgTemp.iWidth & (~0x1);
    cImageInfo.dwImgHeight = imgTemp.iHeight;
    g_cHvPciLinkApi.PutSendData(nPciHandle, &cImageInfo, sizeof(PCI_IMAGE_INFO));

    //ͼƬ���ݴ�С
    DWORD32 dwImgSize = cImageInfo.dwImgWidth * cImageInfo.dwImgHeight * 2;
    g_cHvPciLinkApi.PutSendData(nPciHandle, &dwImgSize, sizeof(DWORD32));

    int iHeight;
    size_t iLineSize = (size_t)cImageInfo.dwImgWidth / 2;

    //����ͼƬ����
    unsigned char *pSrc = pY;
    for (iHeight = 0; iHeight < imgTemp.iHeight; iHeight++, pSrc += imgTemp.iStrideWidth[0])
    {
        g_cHvPciLinkApi.PutSendData(nPciHandle, pSrc, cImageInfo.dwImgWidth);
    }
    pSrc = pCb;
    for (iHeight = 0; iHeight < imgTemp.iHeight; iHeight++, pSrc += (imgTemp.iStrideWidth[0] / 2))
    {
        g_cHvPciLinkApi.PutSendData(nPciHandle, pSrc, iLineSize);
    }
    pSrc = pCr;
    for (iHeight = 0; iHeight < imgTemp.iHeight; iHeight++, pSrc += (imgTemp.iStrideWidth[0] / 2))
    {
        g_cHvPciLinkApi.PutSendData(nPciHandle, pSrc, iLineSize);
    }
    return S_OK;
}

HRESULT CPciResultSender::CopyBinImageToShareBuffer(
    IReferenceComponentImage *pImage,
    DWORD32 dwTimeMsLow,
    DWORD32 dwTimeMsHigh,
    DWORD32 dwCarID,
    int nPciHandle
)
{
    if (!pImage)
    {
        return E_FAIL;
    }

    HV_COMPONENT_IMAGE imgTemp;
    PCI_IMAGE_INFO cImageInfo;
    int nImgInfoSize = sizeof(PCI_IMAGE_INFO);

    pImage->GetImage(&imgTemp);

    //ͼƬ��Ϣ��С
    g_cHvPciLinkApi.PutSendData(nPciHandle, &nImgInfoSize, sizeof(int));

    //ͼƬ����
    cImageInfo.dwCarID = dwCarID;
    cImageInfo.dwImgType = PCILINK_IMAGE_BIN_IMAGE;
    cImageInfo.dwTimeLow = dwTimeMsLow;
    cImageInfo.dwTimeHigh = dwTimeMsHigh;
    cImageInfo.dwImgWidth = imgTemp.iWidth;
    cImageInfo.dwImgHeight = imgTemp.iHeight;
    g_cHvPciLinkApi.PutSendData(nPciHandle, &cImageInfo, sizeof(PCI_IMAGE_INFO));

    //ͼƬ���ݴ�С
    DWORD32 dwImgSize = (imgTemp.iWidth >> 3) * imgTemp.iHeight;
    g_cHvPciLinkApi.PutSendData(nPciHandle, &dwImgSize, sizeof(DWORD32));

    //����ͼƬ����
    unsigned char *pSrc = GetHvImageData(&imgTemp, 0);
    int iHeight;
    int iDestLine = imgTemp.iWidth >> 3;
    for (iHeight = 0; iHeight < imgTemp.iHeight; iHeight++, pSrc += iDestLine)
    {
        g_cHvPciLinkApi.PutSendData(nPciHandle, pSrc, iDestLine);
    }
    return S_OK;
}

HRESULT CPciResultSender::PutResult(
    LPCSTR szResultInfo,
    LPVOID lpcData
)
{
    if (!lpcData)
    {
        return E_FAIL;
    }
    /*
        char* pszPeccancy = strstr(szResultInfo, "Υ��:��");
        // ֻ���Υ��
        if (m_pResultSenderParam->iOutputOnlyPeccancy == 1 && pszPeccancy == NULL)
        {
            HV_Trace(5, "ignore the result because of iOutputOnlyPeccancy == 1");
            return S_OK;
        }
        // ֻ�����Υ��
        if (m_pResultSenderParam->iOutputOnlyPeccancy == 2 && pszPeccancy)
        {
            HV_Trace(5, "ignore the result because of iOutputOnlyPeccancy == 2");
            return S_OK;
        }
    */
    CARLEFT_INFO_STRUCT *lpCarLeftInfo = (CARLEFT_INFO_STRUCT*)lpcData;

    RECOG_RESULT_INFO *pResultInfo = new RECOG_RESULT_INFO();
    static char szAppendInfo[4096] = {0};
    static char szValue[32] = {0};
    if (pResultInfo == NULL)
    {
        HV_Trace(5, "RECOG_RESULT_INFO alloc failed!\n");
        return E_FAIL;
    }
    TiXmlDocument xmlDoc;
    xmlDoc.Parse(szResultInfo);
    TiXmlElement* pRootElement = xmlDoc.RootElement();
    TiXmlElement* pResultSet = pRootElement->FirstChildElement("ResultSet");
    TiXmlElement* pResult = pResultSet->FirstChildElement("Result");
    TiXmlElement* pPlateName = pResult->FirstChildElement("PlateName");
    TiXmlElement* pTemp;
    TiXmlElement* pValue;
    ConvertTickToSystemTime(
        GetSystemTick(),
        pResultInfo->dwTimeMsLow,
        pResultInfo->dwTimeMsHigh
    );
    pResultInfo->dwCarID = m_dwCarID;
    // ����ID
    pValue = new TiXmlElement("CarID");
    if (pValue)
    {
        sprintf(szValue, "%u", m_dwCarID);
        pValue->SetAttribute("value", szValue);
        pResult->LinkEndChild(pValue);
    }
    // ��32λʶ����ʱ��
    pValue = new TiXmlElement("TimeLow");
    if (pValue)
    {
        sprintf(szValue, "%u", pResultInfo->dwTimeMsLow);
        pValue->SetAttribute("value", szValue);
        pResult->LinkEndChild(pValue);
    }
    // ��32λʶ����ʱ��
    pValue = new TiXmlElement("TimeHigh");
    if (pValue)
    {
        sprintf(szValue, "%u", pResultInfo->dwTimeMsHigh);
        pValue->SetAttribute("value", szValue);
        pResult->LinkEndChild(pValue);
    }
    char* pszPlate = NULL;
    if (m_pResultSenderParam->cProcRule.fLeach
            || m_pResultSenderParam->cProcRule.fReplace)
    {
        //��XML�н����������ַ����͸�����Ϣ
        const char* pszPlateName = pPlateName->GetText();
        const char* pszChnName = NULL;
        const char* pszValue = NULL;
        pszPlate = (char*)pszPlateName;
        szAppendInfo[0] = 0;
        pTemp = pResult->FirstChildElement();
        while (pTemp)
        {
            pszChnName = pTemp->Attribute("chnname");
            pszValue = pTemp->Attribute("value");
            if (pszChnName && pszValue)
            {
                if (strstr(pszChnName, "�¼����") == NULL)
                {
                    strcat(szAppendInfo, pszChnName);
                    strcat(szAppendInfo, ":");
                    strcat(szAppendInfo, pszValue);
                    strcat(szAppendInfo, "\n");
                }
                else
                {
                    strcat(szAppendInfo, pszValue);
                    strcat(szAppendInfo, "\n");
                }
            }
            pTemp = pTemp->NextSiblingElement();
        }

        RESULT_INFO resultInfo;
        resultInfo.strPlate = pszPlateName;
        resultInfo.strOther = szAppendInfo;
        resultInfo.strApplied = "";
        if (m_resultFilter.FilterProcess(&resultInfo))
        {
            if (resultInfo.fLeach)
            {
                delete pResultInfo;
                return S_FALSE;  //������Ҫ���˵Ľ��
            }

            pPlateName->FirstChild()->SetValue(resultInfo.strPlate.GetBuffer());
            if (!resultInfo.strApplied.IsEmpty()
                    && m_pResultSenderParam->fOutputFilterInfo)
            {
                pValue = new TiXmlElement("ResultProcess");
                if (pValue)
                {
                    pValue->SetAttribute("value", resultInfo.strApplied.GetBuffer());
                    pValue->SetAttribute("chnname", "������Ϣ");
                    pResult->LinkEndChild(pValue);
                }
            }
        }
    }
    //��������XML�ַ���
    TiXmlPrinter cTxPr;
    xmlDoc.Accept(&cTxPr);
    strncpy(pResultInfo->szPlateInfo, cTxPr.CStr(), MAX_PLATE_STRING_SIZE);
    LockResult();

    int nListSize = m_rgResultInfo.GetSize();
    if (nListSize >= MAX_RESULT_LIST_COUNT)
    {
        delete pResultInfo;
        HV_Trace(5, "Result list is full!\n");
        UnLockResult();
        return E_FAIL;
    }
    AllImageAddRef(&lpCarLeftInfo->cCoreResult.cResultImg);
    //plate
    pResultInfo->szPlate[0] = 0;
    if (pszPlate != NULL && strlen(pszPlate) > 0)
    {
        strcpy( pResultInfo->szPlate, pszPlate );
    }
    //������Ϣ
    pResultInfo->rcBestPlatePos = lpCarLeftInfo->cCoreResult.rcBestPlatePos;
    pResultInfo->rcLastPlatePos = lpCarLeftInfo->cCoreResult.rcLastPlatePos;
    pResultInfo->rcFirstPos    = lpCarLeftInfo->cCoreResult.rcFirstPos;
    pResultInfo->rcSecondPos    = lpCarLeftInfo->cCoreResult.rcSecondPos;
    pResultInfo->rcThirdPos    = lpCarLeftInfo->cCoreResult.rcThirdPos;
    pResultInfo->rcRedLightCount= lpCarLeftInfo->cCoreResult.rcRedLightCount;
    memcpy(pResultInfo->rcRedLightPos, lpCarLeftInfo->cCoreResult.rcRedLightPos, pResultInfo->rcRedLightCount * sizeof(HiVideo::CRect));
    pResultInfo->nFaceCount = lpCarLeftInfo->cCoreResult.nFaceCount;
    for (int i = 0; i< 20 ; i++)
    {
        pResultInfo->rcFacePos[i].bottom = lpCarLeftInfo->cCoreResult.rgFaceRect[i].bottom * 2;
        pResultInfo->rcFacePos[i].top = lpCarLeftInfo->cCoreResult.rgFaceRect[i].top * 2;
        pResultInfo->rcFacePos[i].left = lpCarLeftInfo->cCoreResult.rgFaceRect[i].left;
        pResultInfo->rcFacePos[i].right = lpCarLeftInfo->cCoreResult.rgFaceRect[i].right;
    }
    pResultInfo->pimgPlate = lpCarLeftInfo->cCoreResult.cResultImg.pimgPlate;
    pResultInfo->pimgBestSnapShot = lpCarLeftInfo->cCoreResult.cResultImg.pimgBestSnapShot;
    pResultInfo->pimgLastSnapShot = lpCarLeftInfo->cCoreResult.cResultImg.pimgLastSnapShot;
    pResultInfo->pimgBeginCapture = lpCarLeftInfo->cCoreResult.cResultImg.pimgBeginCapture;
    pResultInfo->pimgBestCapture = lpCarLeftInfo->cCoreResult.cResultImg.pimgBestCapture;
    pResultInfo->pimgLastCapture = lpCarLeftInfo->cCoreResult.cResultImg.pimgLastCapture;
    pResultInfo->pimgPlateBin = lpCarLeftInfo->cCoreResult.cResultImg.pimgPlateBin;
    int nIndex = m_rgResultInfo.AddTail(pResultInfo);
    UnLockResult();

    if (nIndex < nListSize)
    {
        delete pResultInfo;
        AllImageRelease(&lpCarLeftInfo->cCoreResult.cResultImg);
        HV_Trace(5, "Add result to list failed!\n");
        return E_FAIL;
    }
    m_dwCarID++;

    SemPost(&m_hSemQueue);

    return S_OK;
}

HRESULT CPciResultSender::PutVideo(
    DWORD32* pdwSendCount,
    LPVOID lpFrameData,
    int nRectCount/* = 0*/,
    HV_RECT *pRect/* = NULL*/
)
{
    if (lpFrameData == NULL || pdwSendCount == NULL || pRect == NULL)
    {
        return E_POINTER;
    }

    if (!g_iSendSlaveImage)
    {
        return S_FALSE;
    }

    IMG_FRAME *pFrame = (IMG_FRAME *)lpFrameData;
    DWORD32 dwImgTime = pFrame->pRefImage->GetRefTime();

    static DWORD32 dwLastSendTime = 0;

    if (0 != dwImgTime)
    {
        if (dwImgTime -  dwLastSendTime < (DWORD32)m_pResultSenderParam->iVideoDisplayTime)
        {
            return S_OK;
        }
        else
        {
            dwLastSendTime = dwImgTime;
        }
    }

    VIDEO_RESULT_INFO *pVideoInfo = new VIDEO_RESULT_INFO();
    if (pVideoInfo == NULL)
    {
        HV_Trace(5, "VIDEO_RESULT_INFO alloc failed!\n");
        return E_FAIL;
    }

    pVideoInfo->pimgVideo = pFrame->pRefImage;
    pVideoInfo->cTrackRectInfo.dwTrackCount = nRectCount;
    memcpy(pVideoInfo->cTrackRectInfo.rgTrackRect, pRect, sizeof(CRect) * 20);
    pFrame->pRefImage->AddRef();

    LockVideo();

    int nListSize = m_rgVideoInfo.GetSize();
    if (nListSize >= MAX_VIDEO_LIST_COUNT)
    {
        delete pVideoInfo;
        HV_Trace(5, "Video list is full!\n");
        UnLockVideo();
        return E_FAIL;
    }

    int nIndex = m_rgVideoInfo.AddTail(pVideoInfo);

    UnLockVideo();

    if (nIndex < nListSize)
    {
        delete pVideoInfo;
        HV_Trace(5, "Add video to list failed!\n");
        return E_FAIL;
    }

    SemPost(&m_hSemQueue);

    return S_OK;
}

HRESULT CPciResultSender::PutString(
    WORD16 wVideoID,
    WORD16 wStreamID,
    DWORD32 dwTimeLow,
    DWORD32 dwTimeHigh,
    const char *pString
)
{
    static PCI_STRING_INFO cStringInfo;
    cStringInfo.wVideoID = wVideoID;
    cStringInfo.wStreamID = wStreamID;
    cStringInfo.dwTimeLow = dwTimeLow;
    cStringInfo.dwTimeHigh = dwTimeHigh;
    strcpy(cStringInfo.szString, pString);

    return g_cHvPciLinkApi.SendData(
               PCILINK_SEND_STRING,
               &cStringInfo,
               sizeof(PCI_STRING_INFO)
           );
}
