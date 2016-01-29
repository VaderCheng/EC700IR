#include ".\trafficlightinfo.h"
#include "..\Tracker\EPAppUtils.h"

CTrafficLightInfo::CTrafficLightInfo()
{
	m_fInit = false;
	HV_RECT rcInit = {0,0,0,0};
	m_rcLight = rcInit;
	m_rcLastLight = rcInit;
	m_nID = 0;
	m_nType = 0;
	m_nLightCount = 0;
	m_nLastStatus = TS_UNSURE;

	m_iMaxRedThreshold = -1;
	m_iMinRedThreshold = -1;
	m_iRedThreshold	 = 40;
	m_iMaxGreenThreshold	= -1;
	m_iMinGreenThreshold	= -1;
	m_iGreenThreshold	= 15;
	m_pElement = hvCreateStructuringElementEx(
		5,
		5,
		2,
		2,
		HV_SHAPE_RECT);
	m_iFirstAdjust = 0;
	m_fltAdjustValue = 1;
	m_rcLightAdjust = rcInit;
	m_lstAdjustQuence.RemoveAll();
	
}

CTrafficLightInfo::~CTrafficLightInfo()
{
	hvReleaseStructuringElement(&m_pElement);
}
__inline int GetPixelValue(BYTE8 r, BYTE8 g, BYTE8 b)
{
	BYTE8 max = MAX_INT(MAX_INT(r,g),b);

	return max;
}

__inline int GetPixelValue2(int r, int g, int b)
{
	int max = MAX_INT((r * 2 - g - b), (g + b - r * 2));
	if (max > 255) max = 255;
	if (max < 0) max = 0;
	return max;
}
__inline int GetPixelRedValue(int r, int g, int b)
{
	int max = (r * 2 - g - b) / 2 ;
	if (max < 0) max = 0;
	return max;
}
__inline int GetPixelGreenValue(int r, int g, int b)
{
	int max = (g + b - r * 2) / 2;
	if (max < 0) max = 0;
	return max;
}

void CropGrayImg(HV_COMPONENT_IMAGE& imgSrc, HV_RECT rcCrop, HV_COMPONENT_IMAGE& imgCrop)
{
	imgCrop.nImgType = HV_IMAGE_GRAY;
	imgCrop.iStrideWidth[0] = imgSrc.iStrideWidth[0];
	imgCrop.iStrideWidth[1] = imgSrc.iStrideWidth[1];
	imgCrop.iStrideWidth[2] = imgSrc.iStrideWidth[2];
	imgCrop.iWidth = rcCrop.right - rcCrop.left;
	imgCrop.iHeight = rcCrop.bottom - rcCrop.top;

	SetHvImageData(&imgCrop, 0, GetHvImageData(&imgSrc, 0) + rcCrop.top * imgSrc.iStrideWidth[0] + rcCrop.left);
	SetHvImageData(&imgCrop, 1, NULL);
	SetHvImageData(&imgCrop, 2, NULL);
}

int CalcLightValue(HV_COMPONENT_IMAGE& imgLight)
{
	int nCount = 0;
	BYTE8* pBufLine = GetHvImageData(&imgLight, 0);
	for(int i = 0; i < imgLight.iHeight; i++, pBufLine += imgLight.iStrideWidth[0])
	{
		for(int j = 0; j < imgLight.iWidth; j++)
		{
			if(pBufLine[j]  == 255) nCount++;
		}
	}

	return nCount*255/(imgLight.iWidth * imgLight.iHeight);
}

//��������ƫ�̻�ƫ��
HRESULT LightIsGreen(
					const HV_COMPONENT_IMAGE& imgR,
					const HV_COMPONENT_IMAGE &imgG,
					const HV_COMPONENT_IMAGE &imgB,
					const HiVideo::CRect rect,
					bool &fIsGreen)
{
	if (imgR.nImgType != HV_IMAGE_GRAY ||
		imgG.nImgType != HV_IMAGE_GRAY ||
		imgB.nImgType != HV_IMAGE_GRAY ||
		GetHvImageData(&imgR, 0) == NULL ||
		GetHvImageData(&imgG, 0) == NULL ||
		GetHvImageData(&imgB, 0) == NULL)
	{
		return E_INVALIDARG;
	}

	int iSum = 0;
	for(int y = rect.top; y < rect.bottom; y++)
	{
		for(int x = rect.left; x < rect.right; x++)
		{
			iSum += *(GetHvImageData(&imgG, 0) + y * imgG.iStrideWidth[0] + x);
			iSum += *(GetHvImageData(&imgB, 0) + y * imgB.iStrideWidth[0] + x);
			iSum -= *(GetHvImageData(&imgR, 0) + y * imgR.iStrideWidth[0] + x) * 2;
		}
	}
	fIsGreen = (iSum > 0);
	return S_OK;
}
DWORD32 CTrafficLightInfo::GetLightStatus(HV_COMPONENT_IMAGE imgLight, int nPosCount, int nType)
{
	if(nType != 0 && nType != 1) return TS_UNSURE;

	//תΪYUV_422
	CPersistentComponentImage imgTrans;
	if(imgLight.nImgType != HV_IMAGE_YUV_422)
	{
		imgTrans.Create(HV_IMAGE_YUV_422, imgLight.iWidth, imgLight.iHeight);
		imgTrans.Convert(imgLight);
		imgLight = imgTrans;
	}

	//ת����RGB
	CPersistentComponentImage imgR, imgG, imgB, imgLR, imgLG;
	imgR.Create(HV_IMAGE_GRAY, imgLight.iWidth, imgLight.iHeight);
	imgG.Create(HV_IMAGE_GRAY, imgLight.iWidth, imgLight.iHeight);
	imgB.Create(HV_IMAGE_GRAY, imgLight.iWidth, imgLight.iHeight);
	imgLR.Create(HV_IMAGE_GRAY, imgLight.iWidth, imgLight.iHeight);
	imgLG.Create(HV_IMAGE_GRAY, imgLight.iWidth, imgLight.iHeight);
	IMAGE_ConvertYCbCr2BGR(&imgLight, GetHvImageData(&imgR, 0), GetHvImageData(&imgG, 0), GetHvImageData(&imgB, 0), imgR.iStrideWidth[0]);


	//��������ֵ
	RESTRICT_PBYTE8 pRLine = GetHvImageData(&imgR, 0);
	RESTRICT_PBYTE8 pGLine = GetHvImageData(&imgG, 0);
	RESTRICT_PBYTE8 pBLine = GetHvImageData(&imgB, 0);
	RESTRICT_PBYTE8 pLRLine = GetHvImageData(&imgLR, 0);
	RESTRICT_PBYTE8 pLGLine = GetHvImageData(&imgLG, 0);
	for(int i = 0; i < imgR.iHeight; i++)
	{
		for(int j = 0; j < imgR.iWidth; j++)
		{
			pLRLine[j] = (BYTE8)(GetPixelRedValue(pRLine[j], pGLine[j], pBLine[j]));
			pLGLine[j] = (BYTE8)(GetPixelGreenValue(pRLine[j], pGLine[j], pBLine[j]));
		}
		pRLine += imgR.iStrideWidth[0];
		pGLine += imgG.iStrideWidth[0];
		pBLine += imgB.iStrideWidth[0];
		pLRLine += imgLR.iStrideWidth[0];
		pLGLine += imgLG.iStrideWidth[0];
	}

	//��ֵ��
	BYTE8 bRedThreshold;
	IMAGE_CalcBinaryThreshold(imgLR, 1, 1, &bRedThreshold);
	if ((int)bRedThreshold > m_iMaxRedThreshold|| m_iMaxRedThreshold == -1) m_iMaxRedThreshold = (int)bRedThreshold;
	if ((int)bRedThreshold < m_iMinRedThreshold || m_iMinRedThreshold == -1) m_iMinRedThreshold = (int)bRedThreshold;
	if (m_iMaxRedThreshold != -1 && m_iMinRedThreshold != -1 
		&& m_iMaxRedThreshold > (m_iMinRedThreshold * 2) 
		&& m_iMaxRedThreshold > 40)
	{
		m_iRedThreshold = m_iMaxRedThreshold;
		m_iMaxRedThreshold = m_iMinRedThreshold = -1;
	}
	if ((int)bRedThreshold < m_iRedThreshold) bRedThreshold = (BYTE8)m_iRedThreshold;
	hvThreshold(&imgLR, &imgLR, bRedThreshold, 255, HV_THRESH_BINARY);

	BYTE8 bGreenThreshold;
	IMAGE_CalcBinaryThreshold(imgLG, 1, 1, &bGreenThreshold);
	if ((int)bGreenThreshold > m_iMaxGreenThreshold || m_iMaxGreenThreshold == -1) m_iMaxGreenThreshold = (int)bGreenThreshold;
	if ((int)bGreenThreshold < m_iMinGreenThreshold|| m_iMinGreenThreshold == -1) m_iMinGreenThreshold = (int)bGreenThreshold;
	if (m_iMaxGreenThreshold != -1 && m_iMinGreenThreshold != -1 
		&& m_iMaxGreenThreshold > (m_iMinGreenThreshold * 2) 
		&& m_iMaxGreenThreshold > 15)
	{
		m_iGreenThreshold = m_iMaxGreenThreshold;
		m_iMaxGreenThreshold = m_iMinGreenThreshold = -1;
	}
	if ((int)bGreenThreshold < m_iGreenThreshold) bGreenThreshold = (BYTE8)m_iGreenThreshold;
	hvThreshold(&imgLG, &imgLG, bGreenThreshold, 255, HV_THRESH_BINARY);
	hvDilateNew(&imgLG, &imgG, m_pElement);
	hvErodeNew(&imgG, &imgLG, m_pElement);
	hvDilateNew(&imgLR, &imgR, m_pElement);
	hvErodeNew(&imgR, &imgLR, m_pElement);

	//���ɷָ��
	HiVideo::CRect rcPos[CTrafficLightInfo::MAX_POS_COUNT];

	if( nType == 0)
	{
		for (int i = 0; i < nPosCount; i++)
		{
			rcPos[i].top = 0;
			rcPos[i].bottom = imgLight.iHeight;
			rcPos[i].right = (i + 1) * imgLight.iWidth / nPosCount;

			if(i == 0)
			{
				rcPos[i].left = 0;
			}
			else
			{
				rcPos[i].left = rcPos[i-1].right;
			}
		}
	}
	else if( nType == 1 )
	{
		for (int i = 0; i < nPosCount; i++)
		{
			rcPos[i].left = 0;
			rcPos[i].right = imgLight.iWidth;
			rcPos[i].bottom = (i + 1) * imgLight.iHeight / nPosCount;

			if(i == 0)
			{
				rcPos[i].top = 0;
			}
			else
			{
				rcPos[i].top = rcPos[i-1].bottom;
			}
		}
	}
	else
	{
		return TS_UNSURE;
	}

	//����ÿ���Ƶ�����ֵ
	HV_COMPONENT_IMAGE imgCrop;
	HiVideo::CRect rcCrop;
	int r[CTrafficLightInfo::MAX_POS_COUNT] = {0};
	int g[CTrafficLightInfo::MAX_POS_COUNT] = {0};

	for(int i = 0; i < nPosCount; i++)
	{
		rcCrop = rcPos[i];
		//ȥ����ԵӰ��
		if ((bRedThreshold >= 100 && m_trafficLight[i].tlsStatus == TLS_RED && !m_tlpTrafficLightParam.fDeleteLightEdge)
			|| (bRedThreshold >= 100 && m_trafficLight[i].tlsStatus == TLS_UNSURE && !m_tlpTrafficLightParam.fDeleteLightEdge)
			|| (bGreenThreshold >= 100 && m_trafficLight[i].tlsStatus == TLS_GREEN && !m_tlpTrafficLightParam.fDeleteLightEdge))
		{
			
		}
		else
		{
			rcCrop.DeflateRect(rcCrop.Width() /6, rcCrop.Height()/6);
		}
		CropGrayImg(imgLR, rcCrop, imgCrop);
		r[i] = CalcLightValue(imgCrop);
		CropGrayImg(imgLG, rcCrop, imgCrop);
		g[i] = CalcLightValue(imgCrop);
	}

	DWORD32 nStatus = 0;
	int iBrighValue = 30 + m_tlpTrafficLightParam.iTrafficLightBrightValue * 15;
	for( int i = 0; i < nPosCount; i++)
	{
		nStatus <<=4;
		if (r[i] > g[i])
		{
			if (r[i] > iBrighValue)
			{
				nStatus |= TS_RED;
			}
		}
		else
		{
			if (g[i] > iBrighValue)
			{
				nStatus |= TS_GREEN;
			}
		}
	}

	return nStatus;
}

HRESULT CTrafficLightInfo::UpdateStatus(HV_COMPONENT_IMAGE* pSceneImage)
{
	if( !pSceneImage ) return E_INVALIDARG;

	HiVideo::CRect rcLight = GetRect();
	HiVideo::CRect rcMax(0, 0, pSceneImage->iWidth, pSceneImage->iHeight);

	if(rcLight.bottom <= rcLight.top ||
		rcLight.right <= rcLight.left ||
		!rcLight.IntersectRect(&rcMax, &rcLight))
	{
		m_nLastStatus = TS_UNSURE;
		return S_FALSE;
	}

	CPersistentComponentImage imgTemp;
	if(pSceneImage->nImgType == HV_IMAGE_BT1120_ROTATE_Y)
	{
		if(FAILED(imgTemp.Create(HV_IMAGE_YUV_422, rcLight.Width(), rcLight.Height())))
		{
			return E_OUTOFMEMORY;
		}
		imgTemp.CropAssign(*pSceneImage, rcLight);
	}
	else
	{
		HV_COMPONENT_IMAGE imgCrop;
		CropImage(*pSceneImage, rcLight, &imgCrop);	
		if(FAILED(imgTemp.Create(HV_IMAGE_YUV_422, imgCrop.iWidth, imgCrop.iHeight)))
		{
			return E_OUTOFMEMORY;
		}
		imgTemp.Convert(imgCrop);
	}
	CPersistentComponentImage imgLight;
	if(FAILED(imgLight.Create(HV_IMAGE_YUV_422, imgTemp.iWidth, imgTemp.iHeight*2)))
	{
		return E_OUTOFMEMORY;
	}
	PlateResize(imgTemp, imgLight);

	m_nLastStatus = GetLightStatus(imgLight, m_nLightCount, m_nType);

	return S_OK;
}

HRESULT CTrafficLightInfo::UpdateStatus2(HV_COMPONENT_IMAGE* pSceneImage)
{
	if( !pSceneImage ) return E_INVALIDARG;

	HiVideo::CRect rcLight = GetRect();
	HiVideo::CRect rcMax(0, 0, pSceneImage->iWidth, pSceneImage->iHeight);

	if(!m_rcLightInflated.IntersectRect(m_rcLightInflated, rcMax))
	{
		m_nLastStatus = TS_UNSURE;
		return S_FALSE;
	}

	CPersistentComponentImage imgLight;
	if(pSceneImage->nImgType == HV_IMAGE_BT1120_ROTATE_Y)
	{
		if(FAILED(imgLight.Create(HV_IMAGE_YUV_422, m_rcLightInflated.Width(), m_rcLightInflated.Height())))
		{
			return E_OUTOFMEMORY;
		}
		imgLight.CropAssign(*pSceneImage, m_rcLightInflated);
	}
	else
	{
		HV_COMPONENT_IMAGE imgCrop;
		CropImage(*pSceneImage, m_rcLightInflated, &imgCrop);	
		if(FAILED(imgLight.Create(HV_IMAGE_YUV_422, imgCrop.iWidth, imgCrop.iHeight)))
		{
			return E_OUTOFMEMORY;
		}
		imgLight.Convert(imgCrop);
	}
	m_nLastStatus = GetLightStatus2(imgLight, rcLight);

	return S_OK;
}
HRESULT CTrafficLightInfo::SetTrafficLightParam(TRAFFIC_LIGHT_PARAM tlpParam)
{
	m_tlpTrafficLightParam = tlpParam;
	return S_OK;
}
HRESULT CTrafficLightInfo::SetAdjustRect(const HV_RECT& rcRect)
{
	//���̵ƶ�̬�����ķ�Χ
	int iAdjustWidth = m_rcLight.Width() > m_rcLight.Height() ? m_rcLight.Height() : m_rcLight.Width();
	m_rcLightAdjust.left = m_rcLight.left - iAdjustWidth * 2;
	m_rcLightAdjust.right = m_rcLight.right + iAdjustWidth * 2;
	m_rcLightAdjust.top = m_rcLight.top - iAdjustWidth * 2;
	m_rcLightAdjust.bottom = m_rcLight.bottom + iAdjustWidth * 2;

	m_rcLightAdjust.left = m_rcLightAdjust.left < 0?0:m_rcLightAdjust.left;
	m_rcLightAdjust.top = m_rcLightAdjust.top < 0?0:m_rcLightAdjust.top;
	
	//��ʼ�����̵Ʊ궨����
	m_rcInitLight = rcRect;
	//��ʼ��������ȷʶ���������
	m_rcLastSureLight = rcRect;
	
	m_fInit = true;
	signalway::direction dtDirection = (m_nType == 0?signalway::HORZ:signalway::VERT);
	signalway::light_group lgLightPos(m_rcInitLight, dtDirection, m_nLightCount);
	HV_SIZE hsSize;
	hsSize.cx = 0;
	hsSize.cy = 0;
	m_cTrafficLightObj.SetRecognizerParam(lgLightPos, hsSize, 1);

	return S_OK;
}
HRESULT CTrafficLightInfo::SetRect(const HV_RECT& cRect, int nLightCount, int nType)
{
	m_rcLight = cRect;
	m_nLightCount = nLightCount;
	m_nType = nType;

	//������鷶Χ
	int iInflateWidth = m_rcLight.Width() > m_rcLight.Height() ? m_rcLight.Height() : m_rcLight.Width();
	iInflateWidth = (int)(iInflateWidth * m_tlpTrafficLightParam.iAutoScanThread / 10.0);
	m_rcLightInflated = m_rcLight;
	m_rcLightInflated.InflateRect(iInflateWidth, iInflateWidth);

	m_rcLightInflated.top = m_rcLightInflated.top < 0?0:m_rcLightInflated.top;
	m_rcLightInflated.left = m_rcLightInflated.left < 0?0:m_rcLightInflated.left;

	return S_OK;
}

HRESULT CTrafficLightInfo::SetRect(int nLeft, int nTop, int nRight, int nBottom, int nLightCount, int nType)
{
	HV_RECT rcRect = { nLeft, nTop, nRight, nBottom };
	return SetRect(rcRect, nLightCount, nType);
}

#include "hv2ipl.h"

DWORD32 CTrafficLightInfo::GetLightStatus2( HV_COMPONENT_IMAGE imgLight, HiVideo::CRect rcLight )
{
	if(m_nType != 0 && m_nType != 1) return TS_UNSURE;

	//תΪYUV_422
	CPersistentComponentImage imgTrans;
	if(imgLight.nImgType != HV_IMAGE_YUV_422)
	{
		imgTrans.Create(HV_IMAGE_YUV_422, imgLight.iWidth, imgLight.iHeight);
		imgTrans.Convert(imgLight);
		imgLight = imgTrans;
	}

	//ת����RGB
	CPersistentComponentImage imgR, imgG, imgB, imgLR, imgLG;
	imgR.Create(HV_IMAGE_GRAY, imgLight.iWidth, imgLight.iHeight);
	imgG.Create(HV_IMAGE_GRAY, imgLight.iWidth, imgLight.iHeight);
	imgB.Create(HV_IMAGE_GRAY, imgLight.iWidth, imgLight.iHeight);
	imgLR.Create(HV_IMAGE_GRAY, imgLight.iWidth, imgLight.iHeight);
	imgLG.Create(HV_IMAGE_GRAY, imgLight.iWidth, imgLight.iHeight);
	IMAGE_ConvertYCbCr2BGR(&imgLight, GetHvImageData(&imgR, 0), GetHvImageData(&imgG, 0), GetHvImageData(&imgB, 0), imgR.iStrideWidth[0]);


	//��������ֵ
	RESTRICT_PBYTE8 pRLine = GetHvImageData(&imgR, 0);
	RESTRICT_PBYTE8 pGLine = GetHvImageData(&imgG, 0);
	RESTRICT_PBYTE8 pBLine = GetHvImageData(&imgB, 0);
	RESTRICT_PBYTE8 pLRLine = GetHvImageData(&imgLR, 0);
	RESTRICT_PBYTE8 pLGLine = GetHvImageData(&imgLG, 0);
	for(int i = 0; i < imgR.iHeight; i++)
	{
		for(int j = 0; j < imgR.iWidth; j++)
		{
			pLRLine[j] = (BYTE8)(GetPixelRedValue(pRLine[j], pGLine[j], pBLine[j]));
			pLGLine[j] = (BYTE8)(GetPixelGreenValue(pRLine[j], pGLine[j], pBLine[j]));
		}
		pRLine += imgR.iStrideWidth[0];
		pGLine += imgG.iStrideWidth[0];
		pBLine += imgB.iStrideWidth[0];
		pLRLine += imgLR.iStrideWidth[0];
		pLGLine += imgLG.iStrideWidth[0];
	}

	//��ֵ��
	BYTE8 bThreshold;
	IMAGE_CalcBinaryThreshold(imgLR, 1, 1, &bThreshold);
	if ((int)bThreshold > m_iMaxRedThreshold || m_iMaxRedThreshold == -1) m_iMaxRedThreshold = (int)bThreshold;
	if ((int)bThreshold < m_iMinRedThreshold || m_iMinRedThreshold == -1) m_iMinRedThreshold = (int)bThreshold;
	if (m_iMaxRedThreshold != -1 && m_iMinRedThreshold != -1 
		&& m_iMaxRedThreshold > (m_iMinRedThreshold * 2) 
		&& m_iMaxRedThreshold > 40)
	{
		if (m_iRedThreshold != m_iMaxRedThreshold)
		{
			//HV_Trace("<Red %d>", m_iMaxRedThreshold);
		}
		m_iRedThreshold = m_iMaxRedThreshold;
		m_iMaxRedThreshold = m_iMinRedThreshold = -1;
	}
	if ((int)bThreshold < m_iRedThreshold) bThreshold = (BYTE8)m_iRedThreshold;
	hvThreshold(&imgLR, &imgLR, bThreshold, 255, HV_THRESH_BINARY);

	IMAGE_CalcBinaryThreshold(imgLG, 1, 1, &bThreshold);
	if ((int)bThreshold > m_iMaxGreenThreshold || m_iMaxGreenThreshold == -1) m_iMaxGreenThreshold = (int)bThreshold;
	if ((int)bThreshold < m_iMinGreenThreshold || m_iMinGreenThreshold == -1) m_iMinGreenThreshold = (int)bThreshold;
	if (m_iMaxGreenThreshold != -1 && m_iMinGreenThreshold != -1 
		&& m_iMaxGreenThreshold > (m_iMinGreenThreshold * 2) 
		&& m_iMaxGreenThreshold > 15)
	{
		if (m_iGreenThreshold != m_iMaxGreenThreshold)
		{
			//HV_Trace("<Green %d>", m_iMaxGreenThreshold);
		}
		m_iGreenThreshold = m_iMaxGreenThreshold;
		m_iMaxGreenThreshold = m_iMinGreenThreshold = -1;
	}
	if ((int)bThreshold < m_iGreenThreshold) bThreshold = (BYTE8)m_iGreenThreshold;
	hvThreshold(&imgLG, &imgLG, bThreshold, 255, HV_THRESH_BINARY);
	hvDilateNew(&imgLG, &imgG, m_pElement);
	hvErodeNew(&imgG, &imgLG, m_pElement);
	hvDilateNew(&imgLR, &imgR, m_pElement);
	hvErodeNew(&imgR, &imgLR, m_pElement);

	//HvImageDebugShow(&imgLR, cvSize(500, 200));

	//�ҵ����ϸ߿�ĵ�
	const int iMaxBox = 20;
	int cBox = 0;
	HiVideo::CRect rgrcBox[iMaxBox];
	bool rgfBoxsIsGreen[iMaxBox] = {false};
	int nRects;
	nRects = hvFindContoursImg(
		&imgLR,
		HV_RETR_EXTERNAL,
		HV_CHAIN_APPROX_SIMPLE
		);
	for (int i = 0; i < nRects; i++)
	{
		if (cBox == iMaxBox) break;
		rgrcBox[cBox] = g_rgContourRect[i];
		rgfBoxsIsGreen[cBox] = false;
		cBox++;
	}
	nRects = hvFindContoursImg(
		&imgLG,
		HV_RETR_EXTERNAL,
		HV_CHAIN_APPROX_SIMPLE
		);
	for (int i = 0; i < nRects; i++)
	{
		if (cBox == iMaxBox) break;
		rgrcBox[cBox] = g_rgContourRect[i];
		rgfBoxsIsGreen[cBox] = true;
		cBox++;
	}

	DWORD32 dwLightStatus = TS_UNSURE;
	int cLightItems = 0;
	HiVideo::CRect rgrcLightItems[CTrafficLightInfo::MAX_POS_COUNT] = {HiVideo::CRect(0, 0, 0, 0)};
	bool rgfLightItemsIsGreen[CTrafficLightInfo::MAX_POS_COUNT] = {false};

	if (m_nType == 0)
	{
		float fltLightWidth = (float)rcLight.Width() / m_nLightCount;
		float fltLightHeight = (float)rcLight.Height();
		for(int nIndex = 0; nIndex < cBox; nIndex++)
		{
			HiVideo::CRect rcTemp(rgrcBox[nIndex]);
			int iLightItemConnected(0);
			if (rcTemp.Height() > (fltLightHeight * 0.25f) &&
				rcTemp.Height() < (fltLightHeight * 1.5f))
			{
				//�ж�����һ��ĵ�
				if (rcTemp.Width() > (fltLightWidth * 0.5f) &&
					rcTemp.Width() < (fltLightWidth * 1.5f))
				{
					iLightItemConnected = 1;
				}
				else if (rcTemp.Width() > (fltLightWidth * 1.5f) &&
					rcTemp.Width() < (fltLightWidth * 2.5f))
				{
					iLightItemConnected = 2;
				}
				else if (rcTemp.Width() > (fltLightWidth * 2.5f) &&
					rcTemp.Width() < (fltLightWidth * 3.5f))
				{
					iLightItemConnected = 3;
				}
			}
			if (iLightItemConnected > 0)
			{
				//�ж��µĵ��Ƿ���һ������
				if (cLightItems > 0 &&
					HV_ABS(rgrcLightItems[cLightItems - 1].CenterPoint().y - rcTemp.CenterPoint().y) > (fltLightHeight / 2))
				{
					if (HV_ABS(rgrcLightItems[cLightItems - 1].CenterPoint().y - (imgLight.iHeight / 2)) >
						HV_ABS(rcTemp.CenterPoint().y - (imgLight.iHeight / 2)))
					{
						cLightItems = 0;
					}
					else
					{
						iLightItemConnected = 0;
					}
				}
			}
			if (iLightItemConnected > 0 && (cLightItems + iLightItemConnected) < CTrafficLightInfo::MAX_POS_COUNT)
			{
				//���մ����ҵ�˳�����е�
				int i;
				for (i = cLightItems; i > 0; i--)
				{
					if (rgrcLightItems[i - 1].CenterPoint().x > rcTemp.CenterPoint().x)
					{
						rgrcLightItems[i - 1 + iLightItemConnected] = rgrcLightItems[i - 1];
						rgfLightItemsIsGreen[i - 1 + iLightItemConnected] = rgfLightItemsIsGreen[1 - 1];
					}
					else
					{
						break;
					}
				}
				for (int j = 0; j < iLightItemConnected; j++)
				{
					HiVideo::CRect rcLightItem;
					rcLightItem.top = rcTemp.top;
					rcLightItem.bottom = rcTemp.bottom;
					rcLightItem.left = (int)(rcTemp.left + j * fltLightWidth + 0.5);
					rcLightItem.right = (int)(rcLightItem.left + fltLightWidth + 0.5);
					rgrcLightItems[i + j] = rcLightItem;
					rgfLightItemsIsGreen[i + j] = rgfBoxsIsGreen[nIndex];
				}
				cLightItems += iLightItemConnected;
			}
		}
		if (cLightItems > 0)
		{
			//��ʼ������λ��֮ǰ��Ҫ��λ
			if (!m_fInit)
			{
				if (m_rcLastLight.Width() == 0)
				{
					m_rcLastLight.left = rgrcLightItems[0].left;
					m_rcLastLight.right = rgrcLightItems[cLightItems - 1].right;
					m_rcLastLight.top = rgrcLightItems[0].top;
					m_rcLastLight.bottom = (int)(m_rcLastLight.top + fltLightHeight + 0.5);
				}
				else
				{
					if (HV_ABS(rgrcLightItems[0].CenterPoint().y - m_rcLastLight.CenterPoint().y) > (fltLightHeight * 0.5f))
					{
						m_rcLastLight = HiVideo::CRect(0, 0, 0, 0);
					}
					else
					{
						if (rgrcLightItems[0].left < m_rcLastLight.left)
							m_rcLastLight.left = rgrcLightItems[0].left;
						if (rgrcLightItems[cLightItems - 1].right > m_rcLastLight.right)
							m_rcLastLight.right = rgrcLightItems[cLightItems - 1].right;
						m_rcLastLight.top = rgrcLightItems[0].top;
						m_rcLastLight.bottom = (int)(m_rcLastLight.top + fltLightHeight + 0.5);
					}
				}
				if (m_rcLastLight.Width() > (fltLightWidth * m_nLightCount + fltLightWidth))
					m_rcLastLight = HiVideo::CRect(0, 0, 0, 0);
				else if (m_rcLastLight.Width() > (fltLightWidth * m_nLightCount - fltLightWidth / 2))
				{
					m_fInit = true;
					if (rgrcLightItems[0].left == m_rcLastLight.left)
						m_rcLastLight.right = m_rcLastLight.left + m_rcLight.Width();
					else
						m_rcLastLight.left = m_rcLastLight.right - m_rcLight.Width();
				}
			}
			else
			{
				float fltOffsetX = 0.0f, fltOffsetY = 0.0f, fltMinOffsetX = fltLightWidth / 2, fltMinOffsetY = fltLightHeight / 2;
				float fltLightPosX = m_rcLastLight.left + fltLightWidth / 2;
				int iLightIndex = 0;
				for (int i = 0; i < m_nLightCount; i++)
				{
					dwLightStatus <<= 4;
					if (iLightIndex < cLightItems)
					{
						fltOffsetX = rgrcLightItems[iLightIndex].CenterPoint().x - fltLightPosX;
						fltOffsetY = rgrcLightItems[iLightIndex].CenterPoint().y - (m_rcLastLight.top + fltLightHeight / 2);
						if (HV_ABS(fltOffsetX) < (fltLightWidth * 0.5f) &&
							HV_ABS(fltOffsetY) < (fltLightHeight * 0.5f))
						{
							if (rgfLightItemsIsGreen[iLightIndex])
							{
								dwLightStatus |= TS_GREEN;
							}
							else
							{
								dwLightStatus |= TS_RED;
							}
							iLightIndex++;
							if ((fltOffsetX * fltOffsetX + fltOffsetY * fltOffsetY) < (fltMinOffsetX * fltMinOffsetX + fltMinOffsetY * fltMinOffsetY))
							{
								fltMinOffsetX = fltOffsetX;
								fltMinOffsetY = fltOffsetY;
							}
						}
					}
					fltLightPosX += fltLightWidth;
				}
				if (HV_ABS(fltMinOffsetX) < (fltLightWidth / 2) &&
					HV_ABS(fltMinOffsetY) < (fltLightHeight / 2))
				{
					m_rcLastLight.OffsetRect((int)(fltMinOffsetX + 0.5), (int)(fltMinOffsetY + 0.5));
				}
			}
		}
	}
	else
	{
		float fltLightWidth = (float)rcLight.Width();
		float fltLightHeight = (float)rcLight.Height() / m_nLightCount;
		for(int nIndex = 0; nIndex < cBox; nIndex++)
		{
			HiVideo::CRect rcTemp(rgrcBox[nIndex]);
			int iLightItemConnected(0);
			if (rcTemp.Width() > (fltLightWidth * 0.25f) &&
				rcTemp.Width() < (fltLightWidth * 1.5f)	)
			{
				//�ж�����һ��ĵ�
				if (rcTemp.Height() > (fltLightHeight * 0.5f) &&
					rcTemp.Height() < (fltLightHeight * 1.5f))
				{
					iLightItemConnected = 1;
				}
				else if (rcTemp.Height() > (fltLightHeight * 1.5f) &&
					rcTemp.Height() < (fltLightHeight * 2.5f))
				{
					iLightItemConnected = 2;
				}
				else if (rcTemp.Height() > (fltLightHeight * 2.5f) &&
					rcTemp.Height() < (fltLightHeight * 3.5f))
				{
					iLightItemConnected = 3;
				}
			}
			if (iLightItemConnected > 0)
			{
				//�ж��µĵ��Ƿ���һ������
				if (cLightItems > 0 &&
					HV_ABS(rgrcLightItems[cLightItems -1].CenterPoint().x - rcTemp.CenterPoint().x) > (fltLightWidth / 2))
				{
					if (HV_ABS(rgrcLightItems[cLightItems - 1].CenterPoint().x - (imgLight.iWidth / 2)) >
						HV_ABS(rcTemp.CenterPoint().x - (imgLight.iWidth / 2)))
					{
						cLightItems = 0;
					}
					else
					{
						iLightItemConnected = 0;
					}
				}
			}
			if (iLightItemConnected > 0 && (cLightItems + iLightItemConnected) < CTrafficLightInfo::MAX_POS_COUNT)
			{
				//���մ����ҵ�˳�����е�
				int i = 0;
				if (cLightItems > 0)
				{
					for (i = cLightItems; i > 0; i--)
					{
						if (rgrcLightItems[i - 1].CenterPoint().y > rcTemp.CenterPoint().y)
						{
							rgrcLightItems[i - 1 + iLightItemConnected] = rgrcLightItems[i - 1];
							rgfLightItemsIsGreen[i - 1 + iLightItemConnected] = rgfLightItemsIsGreen[i - 1];
						}
						else
						{
							break;
						}
					}
				}
				for (int j = 0; j < iLightItemConnected; j++)
				{
					HiVideo::CRect rcLightItem;
					rcLightItem.top = (int)(rcTemp.top + j * fltLightHeight + 0.5);
					rcLightItem.bottom = (int)(rcLightItem.top + fltLightHeight + 0.5);
					rcLightItem.left = rcTemp.left;
					rcLightItem.right = rcTemp.right;
					rgrcLightItems[i + j] = rcLightItem;
					rgfLightItemsIsGreen[i + j] = rgfBoxsIsGreen[nIndex];
				}
				cLightItems += iLightItemConnected;
			}
		}
		if (cLightItems > 0)
		{
			//��ʼ������λ��֮ǰ��Ҫ��λ
			if (!m_fInit)
			{
				if (m_rcLastLight.Height() == 0)
				{
					m_rcLastLight.top = rgrcLightItems[0].top;
					m_rcLastLight.bottom = rgrcLightItems[cLightItems - 1].bottom;
					m_rcLastLight.left = rgrcLightItems[0].left;
					m_rcLastLight.right = (int)(m_rcLastLight.left + fltLightWidth + 0.5);
				}
				else
				{
					if (abs(rgrcLightItems[0].CenterPoint().x - m_rcLastLight.CenterPoint().x) > (fltLightWidth * 0.5f))
					{
						m_rcLastLight = HiVideo::CRect(0, 0, 0, 0);
					}
					else
					{
						if (rgrcLightItems[0].top < m_rcLastLight.top)
							m_rcLastLight.top = rgrcLightItems[0].top;
						if (rgrcLightItems[cLightItems - 1].bottom > m_rcLastLight.bottom)
							m_rcLastLight.bottom = rgrcLightItems[cLightItems - 1].bottom;
						m_rcLastLight.left = rgrcLightItems[0].left;
						m_rcLastLight.right = (int)(m_rcLastLight.left + fltLightWidth + 0.5);
					}
				}
				if (m_rcLastLight.Height() > (fltLightHeight * m_nLightCount + fltLightHeight))
					m_rcLastLight = HiVideo::CRect(0, 0, 0, 0);
				else if (m_rcLastLight.Height() > (fltLightHeight * m_nLightCount - fltLightHeight / 2))
				{
					m_fInit = true;
					if (rgrcLightItems[0].top == m_rcLastLight.top)
						m_rcLastLight.bottom = m_rcLastLight.top + m_rcLight.Height();
					else
						m_rcLastLight.top = m_rcLastLight.bottom - m_rcLight.Height();
				}
			}
			else
			{
				float fltOffsetX = 0, fltOffsetY = 0, fltMinOffsetX = fltLightWidth / 2, fltMinOffsetY = fltLightHeight / 2;
				float fltLightPosY = m_rcLastLight.top + fltLightHeight / 2;
				int iLightIndex = 0;
				for (int i = 0; i < m_nLightCount; i++)
				{
					dwLightStatus <<= 4;
					if (iLightIndex < cLightItems)
					{
						fltOffsetX = rgrcLightItems[iLightIndex].CenterPoint().x - (m_rcLastLight.left + fltLightWidth / 2);
						fltOffsetY = rgrcLightItems[iLightIndex].CenterPoint().y - fltLightPosY;
						if (HV_ABS(fltOffsetX) < (fltLightWidth * 0.5f) &&
							HV_ABS(fltOffsetY) < (fltLightHeight * 0.5f))
						{
							if (rgfLightItemsIsGreen[iLightIndex])
							{
								dwLightStatus |= TS_GREEN;
							}
							else
							{
								dwLightStatus |= TS_RED;
							}
							iLightIndex++;
							if ((fltOffsetX * fltOffsetX + fltOffsetY * fltOffsetY) < (fltMinOffsetX * fltMinOffsetX + fltMinOffsetY * fltMinOffsetY))
							{
								fltMinOffsetX = fltOffsetX;
								fltMinOffsetY = fltOffsetY;
							}
						}
					}
					fltLightPosY += fltLightHeight;
				}
				if (HV_ABS(fltMinOffsetX) < (fltLightWidth / 2) &&
					HV_ABS(fltMinOffsetY) < (fltLightHeight / 2))
				{
					m_rcLastLight.OffsetRect((int)(fltMinOffsetX + 0.5), (int)(fltMinOffsetY + 0.5));
				}
			}
		}
	}
	return dwLightStatus;
}
//�������ӽӿ�,����̬��⵽�ĵƵ����괫��ȥ��ʾ
void CTrafficLightInfo::CreateLightGroupPos(HV_RECT rctSrc)
{
	int lightCount = m_queFindLights.size ();
	m_iRedLightCountFinded = 0;
	m_iGreenLightCountFinded = 0;
	memset(m_rcRedLightPosFinded, 0, sizeof(m_rcRedLightPosFinded));
	memset(m_rcGreenLightPosFinded, 0, sizeof(m_rcGreenLightPosFinded));
	for (int i = 0; i < lightCount; i++)
	{
		trafficlight tlLight = m_queFindLights[i];
		HV_RECT rcTmp;
		rcTmp.left = tlLight.position_.left;
		rcTmp.right = tlLight.position_.right;
		rcTmp.top = tlLight.position_.top;
		rcTmp.bottom = tlLight.position_.bottom;
		if (tlLight.status_ == GREEN)
		{
			m_rcGreenLightPosFinded[m_iGreenLightCountFinded] = rcTmp;
			m_iGreenLightCountFinded ++;
		}
		else
		{
			m_rcRedLightPosFinded[m_iRedLightCountFinded] = rcTmp;
			m_iRedLightCountFinded ++;
		}
	}
}
HRESULT CTrafficLightInfo::UpdateStatus3( HV_COMPONENT_IMAGE* pSceneImage , bool fNight)
{
	if( !pSceneImage ) return E_INVALIDARG;

	HiVideo::CRect rcLight = GetRect();

	HiVideo::CRect rcMax(0, 0, pSceneImage->iWidth, pSceneImage->iHeight);

	if(rcLight.bottom <= rcLight.top ||
		rcLight.right <= rcLight.left ||
		!rcLight.IntersectRect(&rcMax, &rcLight))
	{
		m_nLastStatus = TS_UNSURE;
		return S_FALSE;
	}

	//���Һ��̵�
	m_queFindLights.clear();

	m_cTrafficLightObj.FindTrafficLight(pSceneImage, m_queFindLights, true, m_tlpTrafficLightParam.fIsLightAdhesion == TRUE, fNight);

	//������ҵ��ĺ��̵�,���㹤����ʾ���ҵ��ĵ�
	CreateLightGroupPos(rcLight);
	//�������̵ƿ�
	AdjuestLightPos(m_rcInitLight);
	
	//ȡ������ĵƿ�
	rcLight = GetRect();
	//����й���,��Ҫ����ʶ��
	if (m_tlpTrafficLightParam.fIsLightAdhesion)
	{
		int iLightY = 0;
		int iLightX = 0;
		if (m_nType == 0)
		{
			iLightY = (int)(rcLight.Height() / 4 + 0.5);
			iLightX = (int)(rcLight.Width() / (4 * m_nLightCount) + 0.5);
		}
		else
		{
			iLightY = (int)(rcLight.Height() / (4 * m_nLightCount) + 0.5);
			iLightX = (int)(rcLight.Width() / 4 + 0.5);
		}		
		rcLight.InflateRect(iLightX, iLightY);
		
		rcLight.left = rcLight.left < 0?0:rcLight.left;
		rcLight.right = rcLight.right > pSceneImage->iWidth?pSceneImage->iWidth:rcLight.right;
		rcLight.top = rcLight.top < 0?0:rcLight.top;
		rcLight.bottom = rcLight.bottom > pSceneImage->iHeight?pSceneImage->iHeight:rcLight.bottom;
	}
	CPersistentComponentImage imgTemp;
	if(pSceneImage->nImgType == HV_IMAGE_BT1120_ROTATE_Y)
	{
		if(FAILED(imgTemp.Create(HV_IMAGE_YUV_422, rcLight.Width(), rcLight.Height())))
		{
			return E_OUTOFMEMORY;
		}
		imgTemp.CropAssign(*pSceneImage, rcLight);
	}
	else
	{
		HV_COMPONENT_IMAGE imgCropNew;
		HRESULT hCrop = CropImage(*pSceneImage, rcLight, &imgCropNew);
		if(hCrop != S_OK)
		{
			return S_FALSE;
		}
	
		if(FAILED(imgTemp.Create(HV_IMAGE_YUV_422, imgCropNew.iWidth, imgCropNew.iHeight)))
		{
			return E_OUTOFMEMORY;
		}
		imgTemp.Convert(imgCropNew);
	}
	CPersistentComponentImage imgLight;
	if(FAILED(imgLight.Create(HV_IMAGE_YUV_422, imgTemp.iWidth, imgTemp.iHeight*2)))
	{
		return E_OUTOFMEMORY;
	}
	PlateResize(imgTemp, imgLight);
	
	//���̵�ʶ��
	m_nLastStatus = GetLightStatus(imgLight, m_nLightCount, m_nType);

	sv::utTrace("<123dsp> m_nLastStatus:%08x.\n", m_nLastStatus);
	return S_OK;
} // UpdateStatus4

//�ж��ҳ����ĵƵ���Ч����
bool CTrafficLightInfo::IsValidityLight(HV_RECT rcSrcLight, HV_RECT rcNewLight, int iWitdh, int iHeight)
{
	bool fValid = false;
	if ((rcNewLight.left <= rcSrcLight.left && rcNewLight.right >  rcSrcLight.left && rcNewLight.right <= rcSrcLight.right 
		&& rcNewLight.left >= (rcSrcLight.left - iWitdh * m_fltAdjustValue))
		|| ( rcNewLight.left >= rcSrcLight.left && rcNewLight.left <  rcSrcLight.right && rcNewLight.right >= rcSrcLight.right 
		&& rcNewLight.left <= (rcSrcLight.left + iWitdh * m_fltAdjustValue)))
		{
			if ((rcNewLight.top <= rcSrcLight.top && rcNewLight.bottom >  rcSrcLight.top 
				&& rcNewLight.bottom <= rcSrcLight.bottom && rcNewLight.top >= (rcSrcLight.top - iHeight * m_fltAdjustValue))
				|| ( rcNewLight.top >= rcSrcLight.top && rcNewLight.top <  rcSrcLight.bottom 
				&& rcNewLight.bottom >= rcSrcLight.bottom && rcNewLight.top <= (rcSrcLight.top + iHeight * m_fltAdjustValue))
				)
				{
					fValid = true;
				}
		}
	return fValid;
}
bool CTrafficLightInfo::IsNeedAdjust(HV_RECT rcSrcLight, HV_RECT rcNewLight, int iWitdh, int iHeight)
{
	bool fAdjust = false;
	float fltAdjustValue = 0.2f;
	if (m_tlpTrafficLightParam.iAutoScanThread >= 8)
	{
		fltAdjustValue = 0.4f;
	}
	if (HV_ABS(rcNewLight.left - rcSrcLight.left) > iWitdh * fltAdjustValue || HV_ABS(rcNewLight.top - rcSrcLight.top) > iHeight * fltAdjustValue)
	{
		HiVideo::CRect rcTemp;
		if (m_lstAdjustQuence.GetSize() > 4)
		{
			HVPOSITION psTemp = m_lstAdjustQuence.GetHeadPosition();
			fAdjust = true;
			if (psTemp == 0) fAdjust = false;
			while(psTemp != 0)
			{
				rcTemp = m_lstAdjustQuence.GetNext(psTemp);
				if (HV_ABS(rcNewLight.left - rcTemp.left) > iWitdh * fltAdjustValue || HV_ABS(rcNewLight.top - rcTemp.top) > iHeight * fltAdjustValue)
				{
					fAdjust = false;
					break;
				}
			}
			if (!fAdjust)
			{
				m_lstAdjustQuence.RemoveHead();
			}
			else
			{
				m_lstAdjustQuence.RemoveAll();
			}
			m_lstAdjustQuence.AddTail(rcNewLight);
		}
		else
		{
			m_lstAdjustQuence.AddTail(rcNewLight);
		}
	}
	else
	{
		fAdjust = true;
		m_lstAdjustQuence.RemoveAll();
		m_lstAdjustQuence.AddTail(rcNewLight);
	}
	return fAdjust;
}
//���������λ��
void CTrafficLightInfo::AdjuestLightPos(HV_RECT rcSrc)
{
	int iLightCount = m_queFindLights.size();
	HV_RECT rcLight = GetRect();
	int iWitdh = rcLight.right - rcLight.left;
	int iHeight = rcLight.bottom - rcLight.top;
	trafficlight trafficLightAdd;
	trafficLightAdd.position_.left = 0;
	trafficLightAdd.position_.right = 0;
	trafficLightAdd.position_.top = 0;
	trafficLightAdd.position_.bottom = 0;
	int iBestDistance = 65535;
	int index = 0;
	//0:ˮƽ����ĵ���,1:��ֱ����ĵ���
	if (m_nType == 0)
	{
		for (int i = 0 ; i < iLightCount; ++i)
		{
			trafficlight light = m_queFindLights[i];
			//if (light.confidence_ < 1) continue;
			int iDistance = 655536;
			HV_RECT rcOneLight;
			int iLightIndex = 0;
			
			HV_RECT rcFind = light.position_;
			int iFindWidth = rcFind.right - rcFind.left;
			int iFindHeight = rcFind.bottom - rcFind.top;

			HV_RECT rcNew = rcLight;
			rcNew.left = rcFind.left;
			rcNew.right = rcNew.left + iWitdh / m_nLightCount;
			rcNew.top = rcFind.top;
			rcNew.bottom = rcNew.top + iHeight;
			if (iFindWidth < iWitdh / m_nLightCount * 0.5 
				|| iFindWidth > 1.2 * iWitdh / m_nLightCount 
				|| iFindHeight < iHeight * 0.5 
				|| iFindHeight > iHeight * 1.2
				)
			{
				continue;
			}
			for (int j = 0; j < m_nLightCount; j++)
			{
				rcOneLight.left = rcLight.left + j * iWitdh / m_nLightCount;
				rcOneLight.top = rcLight.top ;
				rcOneLight.right = rcOneLight.left +  iWitdh/ m_nLightCount;
				rcOneLight.bottom = rcOneLight.top + iHeight;

				int iX = (rcNew.left + rcNew.right) / 2 - (rcOneLight.left + rcOneLight.right) / 2;
				int iY = (rcNew.top + rcNew.bottom) / 2 - (rcOneLight.top + rcOneLight.bottom) / 2;
				int iTemDistance = iX * iX + iY * iY;
				if (iTemDistance < iDistance)
				{
					if ((light.status_ == GREEN && m_trafficLight[j].tlsStatus == TLS_GREEN) || (light.status_ != GREEN ))	
					{
						iDistance = iTemDistance;
						iLightIndex = j;
					}
				}
			}

			rcOneLight.left = rcLight.left + iLightIndex * iWitdh / m_nLightCount;
			rcOneLight.top = rcLight.top ;
			rcOneLight.right = rcOneLight.left +  iWitdh / m_nLightCount;
			rcOneLight.bottom = rcOneLight.top + iHeight;
			
			if (IsValidityLight(rcOneLight, rcNew, iWitdh / m_nLightCount, iHeight))
			{
				if ((light.status_ == RED && m_trafficLight[iLightIndex].tlsStatus == TLS_RED)
					|| (light.status_ == GREEN && m_trafficLight[iLightIndex].tlsStatus == TLS_GREEN)
					)
				{
					if (iBestDistance > iDistance)
					{
						iBestDistance = iDistance;
						trafficLightAdd.position_ = light.position_;
						trafficLightAdd.status_ = light.status_;
						index = iLightIndex;
					}
				}
			}
		}

		if (iBestDistance < 65535 && trafficLightAdd.position_.right - trafficLightAdd.position_.left > 0 && trafficLightAdd.position_.bottom - trafficLightAdd.position_.top > 0)
		{
			if ((trafficLightAdd.status_ == RED && m_trafficLight[index].tlsStatus == TLS_RED)
				|| (trafficLightAdd.status_ == GREEN && m_trafficLight[index].tlsStatus == TLS_GREEN)
				)
			{
				//��һ�ε���,�������һ���ƴ�С��λ��
				if (m_iFirstAdjust == 0) 
				{
					m_iFirstAdjust = 1;
					m_fltAdjustValue = (float)(0.1 * m_tlpTrafficLightParam.iAutoScanThread);
				}
				HV_RECT rcSet = rcLight;
				rcSet.left = (trafficLightAdd.position_.left + trafficLightAdd.position_.right) / 2 - iWitdh / (2 * m_nLightCount) - index * iWitdh / m_nLightCount;
				rcSet.right = rcSet.left + iWitdh;
				rcSet.top = (trafficLightAdd.position_.top + trafficLightAdd.position_.bottom) / 2 - iHeight / 2;
				rcSet.bottom = rcSet.top + iHeight;
				//�ж��Ƿ���Ҫ����
				bool fAdjust = IsNeedAdjust(rcLight, rcSet, iWitdh / m_nLightCount, iHeight);
				if (fAdjust)
				{
					SetRect(rcSet, m_nLightCount, m_nType);
				}
			}
		}
	}
	else
	{
		for (int i = 0; i < iLightCount; ++i)
		{
			trafficlight light = m_queFindLights[i];
			//if (light.confidence_ < 1) continue;
			int iDistance = 655536;
			HV_RECT rcOneLight;
			int iLightIndex = 0;
			
			HV_RECT rcFind = light.position_;
			int iFindWidth = rcFind.right - rcFind.left;
			int iFindHeight = rcFind.bottom - rcFind.top;

			HV_RECT rcNew = rcLight;
			rcNew.left = light.position_.left;
			rcNew.right = rcNew.left + iWitdh;
			rcNew.top = light.position_.top;
			rcNew.bottom = rcNew.top + iHeight / m_nLightCount;

			//�����ҳ����ĵƹ�����߹�С�ĵ�
			if (iFindHeight < iHeight / m_nLightCount * 0.5 
				|| iFindHeight > 1.2 * iHeight / m_nLightCount 
				|| iFindWidth < iWitdh * 0.5 
				|| iFindWidth > iWitdh * 1.2)
			{
				continue;
			}
			//ѡ����ѵ���λ�õĵ�
			for (int j = 0; j < m_nLightCount; j++)
			{
				rcOneLight.left = rcLight.left;
				rcOneLight.top = rcLight.top + j * iHeight / m_nLightCount;
				rcOneLight.right = rcLight.right;
				rcOneLight.bottom = rcOneLight.top + iHeight / m_nLightCount;

				int iX = (rcNew.left + rcNew.right) / 2 - (rcOneLight.left + rcOneLight.right) / 2;
				int iY = (rcNew.top + rcNew.bottom) / 2 - (rcOneLight.top + rcOneLight.bottom) / 2;
				int iTemDistance = iX * iX + iY * iY;
				if (iTemDistance < iDistance)
				{
					if ((light.status_ == GREEN && m_trafficLight[j].tlsStatus == TLS_GREEN) || (light.status_ != GREEN ))				
					{
						iDistance = iTemDistance;
						iLightIndex = j;
					}
				}
			}

			rcOneLight.left = rcLight.left;
			rcOneLight.top = rcLight.top + iLightIndex * iHeight / m_nLightCount;
			rcOneLight.right = rcLight.right;
			rcOneLight.bottom = rcOneLight.top + iHeight / m_nLightCount;
			//�ж��Ƿ���ϵ����ķ�Χ,������ʵ�ʳ����Ƚ��Ƿ����
			if (IsValidityLight(rcOneLight, rcNew, iWitdh, iHeight / m_nLightCount))
			{
				if ((light.status_== RED && m_trafficLight[iLightIndex].tlsStatus == TLS_RED)
					|| (light.status_ == GREEN && m_trafficLight[iLightIndex].tlsStatus == TLS_GREEN)
					)
				{
					if (iBestDistance > iDistance)
					{
						iBestDistance = iDistance;
						trafficLightAdd.position_ = light.position_;
						trafficLightAdd.status_ = light.status_;
						index = iLightIndex;
					}
				}
			}
		}

		if (iBestDistance < 65535 && trafficLightAdd.position_.right - trafficLightAdd.position_.left > 0 && trafficLightAdd.position_.bottom - trafficLightAdd.position_.top > 0)
		{
			if ((trafficLightAdd.status_ == RED && m_trafficLight[index].tlsStatus == TLS_RED)
				|| (trafficLightAdd.status_ == GREEN && m_trafficLight[index].tlsStatus == TLS_GREEN)
				)
			{
				////��һ�ε���,�������һ���ƴ�С��λ��
				if (m_iFirstAdjust == 0) 
				{
					m_iFirstAdjust = 1;
					m_fltAdjustValue = (float)(0.1 * m_tlpTrafficLightParam.iAutoScanThread);
				}
				HV_RECT rcSet;
				rcSet.left = (trafficLightAdd.position_.left + trafficLightAdd.position_.right) / 2 - iWitdh / 2;
				rcSet.right = rcSet.left + iWitdh;
				rcSet.top = (trafficLightAdd.position_.top + trafficLightAdd.position_.bottom) / 2 - iHeight / (2 * m_nLightCount) - index * iHeight / m_nLightCount;
				rcSet.bottom = rcSet.top + iHeight;
				//�ж��Ƿ���Ҫ����
				bool fAdjust = IsNeedAdjust(rcLight, rcSet, iWitdh, iHeight / m_nLightCount);
				if (fAdjust)
				{
					SetRect(rcSet, m_nLightCount, m_nType);
				}
			}
		}
	}
}

