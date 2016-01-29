///////////////////////////////////////////////////////////
//  CSWImage.cpp
//  Implementation of the Class CSWImage
//  Created on:      28-����-2013 14:09:52
//  Original author: zhouy
///////////////////////////////////////////////////////////
#include "SWFC.h"
#include "SWImage.h"

REGISTER_CLASS_LISTEN(CSWImage)
CSWImage::CSWImage()
	: m_pMemoryFactory(NULL)
	, m_pMemory(NULL)
	, m_dwFrameNo(0)
	, m_dwFlag(0)
	, m_dwRefTimeMS(0)
	, m_fIsCaptureImage(FALSE)
	, m_fIsCreated(FALSE)
	, m_dwBufferSize(0)
	, m_dwRealTimeHigh(0)
	, m_dwRealTimeLow(0)
	, m_fOverlay(FALSE)
	, m_pLastImage(NULL)
	, m_iOverlaySize(0)
	, m_pbOverlayBuffer(NULL)
	, m_fBeReplaced(FALSE)
{
	CLASS_LISTEN_ADD(CSWImage)
	m_szFrameName[0] = 0;
	memset(&m_cComponentImage, 0, sizeof(SW_COMPONENT_IMAGE));
	swpa_memset(m_pbSecurityCode,0x0,sizeof(m_pbSecurityCode));
}

CSWImage::~CSWImage()
{
	CLASS_LISTEN_RELEASE(CSWImage)
	Clear();
	SAFE_RELEASE(m_pLastImage);
	if(NULL != m_pbOverlayBuffer)
	{
		delete []m_pbOverlayBuffer;
	}
}

HRESULT CSWImage::AllocOverlayBuffer(INT iSize)
{
	if(m_iOverlaySize < iSize)
	{
		if(NULL != m_pbOverlayBuffer)
		{
			delete []m_pbOverlayBuffer;
		}
		m_pbOverlayBuffer = new BYTE[iSize];
		m_iOverlaySize = iSize;
	}
}

BYTE* CSWImage::GetOverlayBuffer(INT* iSize)
{
	if(NULL != iSize)
	{
		*iSize = m_iOverlaySize;
	}
	return m_pbOverlayBuffer;
}

/**
* @brief ����ͼ��
* 
* @param [in] cImageType : ͼ������
* @param [in] dwWidth : ͼ����
* @param [in] dwHeight : ͼ��߶�
* @param [in] pMemoryFactory : �ڴ������
* @param [in] dwMemSize : ָ��ͼ��ռ���ڴ��С��Ĭ�ϲ�ָ��
* @return
* - S_OK : �ɹ�
* - E_FAIL : ʧ��
*/
HRESULT CSWImage::Create(const SW_IMAGE_TYPE& cImageType, INT iWidth, INT iHeight, CSWMemoryFactory* pMemoryFactory, const INT iMemSize)
{
	if( NULL == pMemoryFactory
		|| (cImageType >= SW_IMAGE_TYPE_COUNT || cImageType < SW_IMAGE_YUV_422)
		)
	{
		return E_INVALIDARG;
	}

	Clear();

	m_cComponentImage.cImageType = cImageType;
	m_cComponentImage.iHeight = iHeight;
	m_cComponentImage.iWidth = iWidth;
	m_pMemoryFactory = pMemoryFactory;
	m_pMemoryFactory->AddRef();

	INT rgiCompHeight[3] = {0};
	switch (cImageType)
	{
	case SW_IMAGE_BGR:
	case SW_IMAGE_HSV:
	case SW_IMAGE_HSB:
		m_cComponentImage.rgiStrideWidth[0] = ((iWidth * 3) + 3)& (~3);
		rgiCompHeight[0] = iHeight;
		break;
	case SW_IMAGE_GRAY:
		m_cComponentImage.rgiStrideWidth[0] = (iWidth + 3)& (~3);
		rgiCompHeight[0] = iHeight;
		break;
	case SW_IMAGE_BIN:
		m_cComponentImage.rgiStrideWidth[0] = (((iWidth + 7) >> 3) + 3)& (~3);
		rgiCompHeight[0] = iHeight;
		break;
	case SW_IMAGE_YUV_411: // 411 �ǿ����һ�룬�߶���һ�룬������iStrideWidth�ϱ��ֲ�����
		m_cComponentImage.rgiStrideWidth[0] = (iWidth + 3) & (~3);
		m_cComponentImage.rgiStrideWidth[1] = m_cComponentImage.rgiStrideWidth[2] = (m_cComponentImage.rgiStrideWidth[0] >> 1);
		rgiCompHeight[0] = iHeight;
		rgiCompHeight[1] = rgiCompHeight[2] = (iHeight >> 1);
		break;
	case SW_IMAGE_JPEG:
		if (0 < iMemSize)
		{
			m_cComponentImage.rgiStrideWidth[0] = 128*1024*(iMemSize / (128*1024) + (iMemSize % (128*1024) ? 1 : 0)); //��128KBΪ��λ���������ڴ�
		}
		else
		{
     		if (iWidth * iHeight > 800 * 300)
     		{
     			m_cComponentImage.rgiStrideWidth[0] = 1 * 1024 * 1024;  // 1MB
			}
     		else
     		{
     			m_cComponentImage.rgiStrideWidth[0] = 128 * 1024;  // 128KB
     		}
		}
		rgiCompHeight[0] = 1;
		break;
	case SW_IMAGE_H264:
		if (0 < iMemSize)
		{
			m_cComponentImage.rgiStrideWidth[0] = 128*1024*(iMemSize / (128*1024) + (iMemSize % (128*1024) ? 1 : 0)); //��128KBΪ��λ���������ڴ�
		}
		else
		{
			m_cComponentImage.rgiStrideWidth[0] = 512 * 1024;  // 512KB
		}
		
		rgiCompHeight[0] = 1;
		break;
	case SW_IMAGE_RAW12:
	case SW_IMAGE_YCbYCr:
	case SW_IMAGE_CbYCrY:
		m_cComponentImage.rgiStrideWidth[0] = ((iWidth << 1) + 3)& (~3);
		rgiCompHeight[0] = iHeight;
		break;
	case SW_IMAGE_BT1120:
		m_cComponentImage.rgiStrideWidth[0] = (iWidth + 3) & (~3);
		m_cComponentImage.rgiStrideWidth[1] = m_cComponentImage.rgiStrideWidth[0];
		m_cComponentImage.rgiStrideWidth[2] = 0;
		rgiCompHeight[0] = iHeight;
		rgiCompHeight[1] = iHeight;
		rgiCompHeight[2] = 0;
		break;
	case SW_IMAGE_BT1120_UV:
		m_cComponentImage.rgiStrideWidth[0] = (iWidth + 3) & (~3);
		m_cComponentImage.rgiStrideWidth[1] = m_cComponentImage.rgiStrideWidth[0];
		m_cComponentImage.rgiStrideWidth[2] = m_cComponentImage.rgiStrideWidth[0];
		rgiCompHeight[0] = iHeight;
		rgiCompHeight[1] = iHeight;
		rgiCompHeight[2] = iHeight;
		break;
	case SW_IMAGE_BT1120_FIELD:
		m_cComponentImage.rgiStrideWidth[0] = (iWidth + 3) & (~3);
		m_cComponentImage.rgiStrideWidth[1] = m_cComponentImage.rgiStrideWidth[0];
		m_cComponentImage.rgiStrideWidth[2] = 0;
		rgiCompHeight[0] = iHeight;
		rgiCompHeight[1] = iHeight;
		rgiCompHeight[2] = 0;
		break;
	case SW_IMAGE_BT1120_ROTATE_Y:
		m_cComponentImage.rgiStrideWidth[0] = (iWidth + 3) & (~3);
		m_cComponentImage.rgiStrideWidth[1] = (iHeight + 3) & (~3);
		m_cComponentImage.rgiStrideWidth[2] = 0;
		rgiCompHeight[0] = iHeight >> 1;
		rgiCompHeight[1] = iWidth;
		rgiCompHeight[2] = 0;
		break;
	case SW_IMAGE_YUV_422:
		if (0 < iMemSize)
		{
			m_cComponentImage.rgiStrideWidth[0] = 128*1024*(iMemSize / (128*1024) + (iMemSize % (128*1024) ? 1 : 0)); //��128KBΪ��λ���������ڴ�
			rgiCompHeight[0] = 1;
			rgiCompHeight[1] = rgiCompHeight[2] = 0;
		}
		else
		{
			m_cComponentImage.rgiStrideWidth[0] = (iWidth + 3) & (~3);
			m_cComponentImage.rgiStrideWidth[1] = m_cComponentImage.rgiStrideWidth[2] = (m_cComponentImage.rgiStrideWidth[0] >> 1);
			rgiCompHeight[0] = rgiCompHeight[1] = rgiCompHeight[2] = iHeight;
		}

		break;
	default:
		Clear();
		return E_INVALIDARG;
	}

	INT iImageSize = 0;
	for(INT i = 0; i < 3; ++i)
	{
		if (m_cComponentImage.rgiStrideWidth[i] > 0 && rgiCompHeight[i] > 0)
		{
			iImageSize += m_cComponentImage.rgiStrideWidth[i] * rgiCompHeight[i];
		}
	}
	m_pMemory = m_pMemoryFactory->Alloc(iImageSize);
	m_dwBufferSize = iImageSize;
	if( NULL == m_pMemory )
	{
		Clear();
		return E_OUTOFMEMORY;
	}

	INT iOffset = 0;
	for (INT i = 0; i < 3; i++)
	{
		if (m_cComponentImage.rgiStrideWidth[i] > 0 && rgiCompHeight[i] > 0)
		{
			m_cComponentImage.rgpbData[i] = (PBYTE)m_pMemory->GetBuffer() + iOffset;
			m_cComponentImage.phys[i] = (DWORD)m_pMemory->GetBuffer(1) + iOffset;
			m_cComponentImage.len[i] = m_cComponentImage.rgiStrideWidth[i] * rgiCompHeight[i];
			iOffset += m_cComponentImage.len[i];
		}
	}

	m_cComponentImage.iSize = iImageSize;
	m_fIsCreated = TRUE;
	return S_OK;
}

HRESULT CSWImage::CreateSWImage(
									CSWImage** ppImage
									, const SW_IMAGE_TYPE& cImageType
									, const INT& iPitch
									, const INT& iWidth
									, const INT& iHeight
									, CSWMemoryFactory* pMemoryFactory
									, const PVOID* pvAddr
									, const PVOID* pvPhys
									, const INT& iSize
									, const DWORD& dwFrameNo
									, const DWORD& dwRefTime
									, const BOOL& fIsCapture
									, LPCSTR szFrameName
									)
{
	if( NULL == pMemoryFactory
		|| (cImageType >= SW_IMAGE_TYPE_COUNT || cImageType < SW_IMAGE_YUV_422)
		)
	{
		return E_INVALIDARG;
	}
	
	CSWAutoPtr<CSWImage>pImage(new CSWImage());

	pImage->Clear();

	pImage->m_cComponentImage.cImageType = cImageType;
	pImage->m_cComponentImage.iHeight = iHeight;
	pImage->m_cComponentImage.iWidth = iWidth;
	pImage->m_pMemoryFactory = pMemoryFactory;
	pImage->m_pMemoryFactory->AddRef();

	INT rgiCompHeight[3] = {0};
	switch (cImageType)
	{
	case SW_IMAGE_BGR:
	case SW_IMAGE_HSV:
	case SW_IMAGE_HSB:
		pImage->m_cComponentImage.rgiStrideWidth[0] = ((iWidth * 3) + 3)& (~3);
		rgiCompHeight[0] = iHeight;
		break;
	case SW_IMAGE_GRAY:
		pImage->m_cComponentImage.rgiStrideWidth[0] = (iWidth + 3)& (~3);
		rgiCompHeight[0] = iHeight;
		break;
	case SW_IMAGE_BIN:
		pImage->m_cComponentImage.rgiStrideWidth[0] = (((iWidth + 7) >> 3) + 3)& (~3);
		rgiCompHeight[0] = iHeight;
		break;
	case SW_IMAGE_YUV_411: // 411 �ǿ����һ�룬�߶���һ�룬������iStrideWidth�ϱ��ֲ�����
		pImage->m_cComponentImage.rgiStrideWidth[0] = (iWidth + 3) & (~3);
		pImage->m_cComponentImage.rgiStrideWidth[1] = pImage->m_cComponentImage.rgiStrideWidth[2] = (pImage->m_cComponentImage.rgiStrideWidth[0] >> 1);
		rgiCompHeight[0] = iHeight;
		rgiCompHeight[1] = rgiCompHeight[2] = (iHeight >> 1);
		break;
	case SW_IMAGE_JPEG:
	case SW_IMAGE_H264:
		pImage->m_cComponentImage.rgiStrideWidth[0] = iSize;
		rgiCompHeight[0] = 1;
		break;
	case SW_IMAGE_RAW12:
	case SW_IMAGE_YCbYCr:
	case SW_IMAGE_CbYCrY:
		pImage->m_cComponentImage.rgiStrideWidth[0] = ((iWidth << 1) + 3)& (~3);
		rgiCompHeight[0] = iHeight;
		break;
	case SW_IMAGE_BT1120:
		pImage->m_cComponentImage.rgiStrideWidth[0] = (iWidth + 3) & (~3);
		pImage->m_cComponentImage.rgiStrideWidth[1] = pImage->m_cComponentImage.rgiStrideWidth[0];
		pImage->m_cComponentImage.rgiStrideWidth[2] = 0;
		rgiCompHeight[0] = iHeight;
		rgiCompHeight[1] = iHeight;
		rgiCompHeight[2] = 0;
		break;
	case SW_IMAGE_BT1120_UV:
		pImage->m_cComponentImage.rgiStrideWidth[0] = (iWidth + 3) & (~3);
		pImage->m_cComponentImage.rgiStrideWidth[1] = pImage->m_cComponentImage.rgiStrideWidth[0];
		pImage->m_cComponentImage.rgiStrideWidth[2] = pImage->m_cComponentImage.rgiStrideWidth[0];
		rgiCompHeight[0] = iHeight;
		rgiCompHeight[1] = iHeight;
		rgiCompHeight[2] = iHeight;
		break;
	case SW_IMAGE_BT1120_FIELD:
		pImage->m_cComponentImage.rgiStrideWidth[0] = (iWidth + 3) & (~3);
		pImage->m_cComponentImage.rgiStrideWidth[1] = pImage->m_cComponentImage.rgiStrideWidth[0];
		pImage->m_cComponentImage.rgiStrideWidth[2] = 0;
		rgiCompHeight[0] = iHeight;
		rgiCompHeight[1] = iHeight;
		rgiCompHeight[2] = 0;
		break;
	case SW_IMAGE_YUV420SP:
		pImage->m_cComponentImage.rgiStrideWidth[0] = iPitch;
		pImage->m_cComponentImage.rgiStrideWidth[1] = iPitch;
		pImage->m_cComponentImage.rgiStrideWidth[2] = 0;
		rgiCompHeight[0] = iHeight;
		rgiCompHeight[1] = iHeight/2;
		rgiCompHeight[2] = 0;
		break;
	case SW_IMAGE_BT1120_ROTATE_Y:
		pImage->m_cComponentImage.rgiStrideWidth[0] = (iWidth + 3) & (~3);
		pImage->m_cComponentImage.rgiStrideWidth[1] = (iHeight + 3) & (~3);
		pImage->m_cComponentImage.rgiStrideWidth[2] = 0;
		rgiCompHeight[0] = iHeight >> 1;
		rgiCompHeight[1] = iWidth;
		rgiCompHeight[2] = 0;
		break;
	case SW_IMAGE_YUV_422:
		pImage->m_cComponentImage.rgiStrideWidth[0] = (iWidth + 3) & (~3);
		pImage->m_cComponentImage.rgiStrideWidth[1] = pImage->m_cComponentImage.rgiStrideWidth[2] = (pImage->m_cComponentImage.rgiStrideWidth[0] >> 1);
		rgiCompHeight[0] = rgiCompHeight[1] = rgiCompHeight[2] = iHeight;
		break;
	default:
		pImage->Clear();
		return E_INVALIDARG;
	}

	INT iImageSize = iSize;
	pImage->m_pMemory = pImage->m_pMemoryFactory->Alloc(pvAddr[0], pvPhys[0], iSize);
	pImage->m_dwBufferSize = iImageSize;
	if( NULL == pImage->m_pMemory )
	{
		pImage->Clear();
		return E_OUTOFMEMORY;
	}
#if 0
	INT iOffset = 0;
	for (INT i = 0; i < 3; i++)
	{
		if (pImage->m_cComponentImage.rgiStrideWidth[i] > 0 && rgiCompHeight[i] > 0)
		{
			pImage->m_cComponentImage.rgpbData[i] = (PBYTE)pImage->m_pMemory->GetBuffer() + iOffset;
			pImage->m_cComponentImage.phys[i] = (DWORD)pImage->m_pMemory->GetBuffer(1) + iOffset;
			pImage->m_cComponentImage.len[i] = pImage->m_cComponentImage.rgiStrideWidth[i] * rgiCompHeight[i];
			iOffset += pImage->m_cComponentImage.len[i];
		}
	}
#else
	for (INT i = 0; i < 3; i++)
	{
		if (pImage->m_cComponentImage.rgiStrideWidth[i] > 0 && rgiCompHeight[i] > 0)
		{
			pImage->m_cComponentImage.rgpbData[i] = (BYTE*)pvAddr[i];
			pImage->m_cComponentImage.phys[i] = (DWORD)pvPhys[i];
			pImage->m_cComponentImage.len[i] = pImage->m_cComponentImage.rgiStrideWidth[i] * rgiCompHeight[i];
		}
	}
#endif
	pImage->m_cComponentImage.iSize = iImageSize;
	pImage->m_fIsCreated = TRUE;
	pImage->AddRef();
	*ppImage = pImage.Get();
	(*ppImage)->SetCaptureFlag(fIsCapture);
	(*ppImage)->SetFrameName(szFrameName);
	(*ppImage)->SetFrameNo(dwFrameNo);
	(*ppImage)->SetRefTime(dwRefTime);	
	return S_OK;
}									
/**
* @brief ͨ��cImageSrc����ʼ������������Ѵ�����ͼ���ʽ��ֱ�����pComponentImage��һ�»��ȱ������
* 
* @param [in] cImageSrc : ԭͼ��
* @param [in] pMemoryFactory : �ڴ������
* @return
* - S_OK : �ɹ�
* - E_FAIL : ʧ��
*/
HRESULT CSWImage::Assign(const SW_COMPONENT_IMAGE& cImageSrc, CSWMemoryFactory* pMemoryFactory)
{
	if( NULL == pMemoryFactory )
	{
		return E_POINTER;
	}

	if( m_fIsCreated 
		&& (cImageSrc.cImageType != m_cComponentImage.cImageType || cImageSrc.iWidth != m_cComponentImage.iWidth || cImageSrc.iHeight != m_cComponentImage.iHeight)
		)
	{
		Clear();
	}

	if( !m_fIsCreated )
	{
		HRESULT hr = Create(cImageSrc.cImageType, cImageSrc.iWidth, cImageSrc.iHeight, pMemoryFactory);
		if( S_OK != hr )
		{
			return hr;
		}
	}

	memcpy(m_cComponentImage.rgpbData[0], cImageSrc.rgpbData[0], m_cComponentImage.iSize);

	return S_OK; 
}

/**
 * @brief ͼ���ڴ�Ĺ���
 * @param [in] pImage ����һ��ͼ��,ʹ�ñ�����ʹ��������ͬһ���ڴ�
 * @return �ɹ�����S_OK������ֵ��ʾ�������
 */
HRESULT CSWImage::Attach(CSWImage *pImage, PVOID pvExif, INT iSize)
{
	if(this != pImage)
	{		
		if(NULL != pvExif && pImage->GetType() == SW_IMAGE_JPEG)
		{
			CSWMemoryFactory *pFactory = m_pMemoryFactory;
			Clear();
			Create(pImage->GetType(), pImage->GetWidth(), pImage->GetHeight(), pFactory);
			m_dwBufferSize = pImage->GetSize() + iSize;
			m_cComponentImage.iSize = pImage->GetSize() + iSize;
			swpa_memcpy(GetImageBuffer(), pImage->GetImageBuffer(), 2);
			swpa_memcpy(GetImageBuffer() + 2, pvExif, iSize);
			swpa_memcpy(GetImageBuffer() + 2 + iSize, pImage->GetImageBuffer() + 2, pImage->GetSize() - 2);
		}
		else
		{
			Clear();
			m_pMemoryFactory = pImage->m_pMemoryFactory;
			pImage->m_pMemoryFactory = NULL;
			
			m_pMemory = pImage->m_pMemory;
			pImage->m_pMemory = NULL;
    	
			m_dwBufferSize = pImage->GetImageBufferSize();
			
			swpa_strcpy(m_szFrameName, pImage->m_szFrameName);
			swpa_memcpy(&m_cComponentImage, &pImage->m_cComponentImage, sizeof(SW_COMPONENT_IMAGE));	
			m_fIsCreated = TRUE;
			pImage->Clear();
		}
	}
	return S_OK;
}

HRESULT CSWImage::ReplaceFrameBuffer(CSWMemoryFactory* pMemoryFactory)
{
	m_cMutexReplace.Lock();
	if (TRUE == m_fBeReplaced)
	{
		m_cMutexReplace.Unlock();
		return S_OK;
	}

	IMAGE image;
	if (SWPAR_OK == swpa_ipnc_get_vnf_frame(GetImageBuffer(), &image, m_dwRefTimeMS))
	{
		//SW_TRACE_NORMAL("get vnf frame sucess,ts:0x%x!\n",image.uTimeStamp);
		const PVOID* pvAddr = image.data.addr;
		const PVOID* pvPhys = image.data.phys;
		const INT iSize = image.data.size;
		const INT iWidth = image.width;
		const INT iHeight = image.height;
		const INT iPitch = image.pitch;

		//�ͷ�ԭ����ͼ�������ڴ�
		if( m_pMemoryFactory != NULL )
		{
			if( m_pMemory != NULL )
			{
				m_pMemoryFactory->Free(m_pMemory);
				m_pMemory = NULL;
			}
			m_pMemoryFactory = NULL;
		}

		//�����µ�ͼ�������ڴ�
		m_pMemoryFactory = pMemoryFactory;
		m_pMemoryFactory->AddRef();

		m_pMemory = m_pMemoryFactory->Alloc(pvAddr[0], pvPhys[0], iSize);

		if( NULL == m_pMemory )
		{
			Clear();
			SW_TRACE_NORMAL("Alloc frame buffer failed!\n");
			m_cMutexReplace.Unlock();
			return E_OUTOFMEMORY;
		}

		INT rgiCompHeight[3] = {0};

		//420SP
		
		m_cComponentImage.rgiStrideWidth[0] = iPitch;
		m_cComponentImage.rgiStrideWidth[1] = iPitch;
		m_cComponentImage.rgiStrideWidth[2] = 0;
		rgiCompHeight[0] = iHeight;
		rgiCompHeight[1] = iHeight/2;
		rgiCompHeight[2] = 0;

		for (INT i = 0; i < 3; i++)
		{
			if (m_cComponentImage.rgiStrideWidth[i] > 0 && rgiCompHeight[i] > 0)
			{
				m_cComponentImage.rgpbData[i] = (BYTE*)pvAddr[i];
				m_cComponentImage.phys[i] = (DWORD)pvPhys[i];
				m_cComponentImage.len[i] = m_cComponentImage.rgiStrideWidth[i] * rgiCompHeight[i];
			}
		}

		m_cComponentImage.iHeight = iHeight;
		m_cComponentImage.iWidth = iWidth;
		m_cComponentImage.iSize = iSize;

		m_fBeReplaced = TRUE;
	}
	else
	{
        SW_TRACE_NORMAL("get vnf frame failed!\n");
        m_cMutexReplace.Unlock();
		return E_FAIL;
	}
	
	m_cMutexReplace.Unlock();
	return S_OK;
}

/**
* @brief ȡ��ͼ��ṹ��
* 
* @param [out] pComponentImage : ͼ��ṹ��
* @return
* - S_OK : �ɹ�
* - E_FAIL : ʧ��
*/
HRESULT CSWImage::GetImage(SW_COMPONENT_IMAGE* pComponentImage)
{
	if( NULL == pComponentImage )
	{
		return E_POINTER;
	}
	if( !m_fIsCreated )
	{
		return E_INVALIDARG;
	}

	memcpy(pComponentImage, &m_cComponentImage, sizeof(SW_COMPONENT_IMAGE));

	return S_OK;
}

/**
* @brief ȡ��ͼ��ṹ��(����)
* 
* @param [out] pComponentImage : ͼ��ṹ��
* @return
* - S_OK : �ɹ�
* - E_FAIL : ʧ��
*/
/*
HRESULT CSWImage::GetImage(HV_COMPONENT_IMAGE* pComponentImage)
{
	if( NULL == pComponentImage )
	{
		return E_POINTER;
	}
	if( !m_fIsCreated )
	{
		return E_INVALIDARG;
	}

	swpa_memcpy(pComponentImage->iStrideWidth, m_cComponentImage.rgiStrideWidth, sizeof(pComponentImage->iStrideWidth));
	pComponentImage->nImgType = (HV_IMAGE_TYPE)m_cComponentImage.cImageType;
	if(pComponentImage->nImgType == HV_IMAGE_JPEG)
	{
		pComponentImage->iHeight = GetWidth()|(GetHeight() << 16);
		pComponentImage->iWidth = GetSize();
	}
	else
	{
		pComponentImage->iHeight = GetHeight();
		pComponentImage->iWidth = GetWidth();
	}
	pComponentImage->iMemPos = 2;
	

	DWORD dwOffset2 = 0;
	DWORD dwOffset3 = 0;
	if( m_cComponentImage.rgpbData[1] != NULL )
	{
		dwOffset2 = (m_cComponentImage.rgpbData[1] - m_cComponentImage.rgpbData[0]);
	}
	if( m_cComponentImage.rgpbData[2] != NULL )
	{
		dwOffset3 = (m_cComponentImage.rgpbData[2] - m_cComponentImage.rgpbData[1]);
	}

	PVOID pvPhys = m_pMemory->GetBuffer(1);
	PVOID pvAddr = m_pMemory->GetBuffer(2);

	pComponentImage->rgImageData[0].addr = pvAddr;
	pComponentImage->rgImageData[0].phys = (UINT)pvPhys;
	if( m_cComponentImage.rgpbData[1] != NULL )
	{
		pComponentImage->rgImageData[1].addr = (PVOID)(dwOffset2 + (UINT)pComponentImage->rgImageData[0].addr);
		pComponentImage->rgImageData[1].phys = (UINT)(dwOffset2 + (UINT)pComponentImage->rgImageData[0].phys);
		pComponentImage->rgImageData[0].len = (UINT)pComponentImage->rgImageData[1].addr - (UINT)pComponentImage->rgImageData[0].addr;
	}
	else
	{
		pComponentImage->rgImageData[1].addr = NULL;
		pComponentImage->rgImageData[1].phys = 0;
		pComponentImage->rgImageData[0].len = GetImageBufferSize();
	}

	if( m_cComponentImage.rgpbData[2] != NULL )
	{
		pComponentImage->rgImageData[2].addr = (PVOID)(dwOffset3 + (UINT)pComponentImage->rgImageData[1].addr);
		pComponentImage->rgImageData[2].phys = (UINT)(dwOffset3 + pComponentImage->rgImageData[1].phys);
		pComponentImage->rgImageData[1].len = (UINT)pComponentImage->rgImageData[2].addr - (UINT)pComponentImage->rgImageData[1].addr;
	}
	else
	{
		pComponentImage->rgImageData[2].addr = NULL;
		pComponentImage->rgImageData[2].phys = 0;
		pComponentImage->rgImageData[1].len = GetImageBufferSize() - (UINT)pComponentImage->rgImageData[0].len;
	}
	pComponentImage->rgImageData[2].len = GetImageBufferSize() - pComponentImage->rgImageData[0].len - pComponentImage->rgImageData[1].len;

	return S_OK;
}
*/

/**
* @brief ����ͼ��ṹ��
* 
* @param [out] pComponentImage : ͼ��ṹ��
* @return
* - S_OK : �ɹ�
* - E_FAIL : ʧ��
*/
HRESULT CSWImage::SetImage(const SW_COMPONENT_IMAGE& cComponentImage, const INT& iAddrType)
{

	if( !m_fIsCreated )
	{
		return E_INVALIDARG;
	}

	memcpy(&m_cComponentImage, &cComponentImage, sizeof(SW_COMPONENT_IMAGE));

	return S_OK;
}


/**
* @brief ȡ��ͼ��ռ���ڴ��С
* 
* @return
* - ���������: �ɹ�
* - 0 : ʧ��
*/
INT CSWImage::GetSize()
{
	if( !m_fIsCreated )
	{
		return 0;
	}

	return m_cComponentImage.iSize;
}

/**
* @brief ȡ��ͼ����ֵ
* 
* @return
* - ���������: �ɹ�
* - 0 : ʧ��
*/
INT CSWImage::GetWidth()
{
	if( !m_fIsCreated )
	{
		return 0;
	}

	return m_cComponentImage.iWidth;
}


/**
* @brief ȡ��ͼ��߶�ֵ
* 
* @return
* - ���������: �ɹ�
* - 0 : ʧ��
*/
INT CSWImage::GetHeight()
{
	if( !m_fIsCreated )
	{
		return 0;
	}

	return m_cComponentImage.iHeight;
}



/**
* @brief ȡ��ͼ������
* 
* @return
* - ͼ������ö��ֵ
* @note ����ֵΪSW_IMAGE_TYPE_COUNT��˵����ȡ����ʧ�ܡ�
* 
*/
SW_IMAGE_TYPE CSWImage::GetType()
{
	if( !m_fIsCreated )
	{
		return SW_IMAGE_TYPE_COUNT; //��־��ȡʧ��
	}

	return m_cComponentImage.cImageType;
}



/**
* @brief ȡ��ָ����ͼ�����ͼ��ṹ��
* 
* @param [out] pComponentImage : ͼ��ṹ��
* @param [in/out] cCorpRect : Ҫ��ȡ������
* @return
* - S_OK : �ɹ�
* - E_FAIL : ʧ��
*/
HRESULT CSWImage::GetCropImage(SW_COMPONENT_IMAGE* pComponentImage, SW_RECT& cCorpRect)
{
	if( NULL == pComponentImage )
	{
		return E_POINTER;
	}
	if( !m_fIsCreated || m_pMemoryFactory == NULL )
	{
		return E_INVALIDARG;
	}

	return CropImage(m_cComponentImage, cCorpRect, pComponentImage);
}

/**
* @brief ȡ��ͼ�������ڴ��׵�ַ
* 
* @param [in]  : ��ַ���ͣ�0: Ĭ��������ƽ̨�йأ�1��ǿ��ȡ�����ַ��2��ǿ��ȡ�����ַ��
* @return
* - S_OK : �ɹ�
* - E_FAIL : ʧ��
*/
PBYTE CSWImage::GetImageBuffer(const INT& iAddrType)
{
	if( !m_fIsCreated || NULL == m_pMemory )
	{
		return NULL;
	}

	return (PBYTE)m_pMemory->GetBuffer(iAddrType);
}

/**
* @brief ȡ�������ڴ�Ĵ�С
* 
* @return
* �ڴ��С��
*/
DWORD CSWImage::GetImageBufferSize()
{
	return m_dwBufferSize;
}
/**
* @brief ��ȡͼ��,ͼ�����ݲ������ڴ濽����ֻ���޸�ָ��ָ�����ݡ�
*
* @param [in] cImageSrc : ԭͼ��ṹ��
* @param [in] rcCrop : Ҫ��ȡ������
* @param [out] pImageDest : ��ȡ��ͼ��
* @return
* - S_OK : �ɹ�
* - E_FAIL : ʧ��
*/
HRESULT CSWImage::CropImage(
	const SW_COMPONENT_IMAGE &cImageSrc,
	SW_RECT& rcCrop,
	SW_COMPONENT_IMAGE *pImageDest
	)
{
	if( NULL != pImageDest )
	{
		return E_POINTER;
	}
	if (rcCrop.left < 0 || rcCrop.top < 0 ||
		rcCrop.right < rcCrop.left || rcCrop.bottom < rcCrop.top ||
		rcCrop.right > cImageSrc.iWidth ||
		rcCrop.bottom > cImageSrc.iHeight)
	{
		return E_INVALIDARG;
	}
	rcCrop.left &= ~1;

	INT rgiOffset[3] = {0};
	switch (cImageSrc.cImageType)
	{
	case SW_IMAGE_YUV_422:
		rgiOffset[0] = cImageSrc.rgiStrideWidth[0] * rcCrop.top + rcCrop.left;
		rgiOffset[1] = rgiOffset[2] = (rgiOffset[0] >> 1);
		break;
	case SW_IMAGE_YCbYCr:
	case SW_IMAGE_CbYCrY:
		rgiOffset[0] = cImageSrc.rgiStrideWidth[0] * rcCrop.top + (rcCrop.left << 1);
		rgiOffset[1] = rgiOffset[2] = 0;
		break;
	case SW_IMAGE_BT1120:
	case SW_IMAGE_BT1120_FIELD:
		rgiOffset[0] = cImageSrc.rgiStrideWidth[0] * rcCrop.top + rcCrop.left;
		rgiOffset[1] = cImageSrc.rgiStrideWidth[1] * rcCrop.top + rcCrop.left;
		rgiOffset[1] &= ~1;
		rgiOffset[2] = 0;
		break;
		// SW_IMAGE_BT1120_ROTATE_Y��ʽֻ��Y����
	case SW_IMAGE_BT1120_ROTATE_Y:
		rgiOffset[0] = cImageSrc.rgiStrideWidth[0] * rcCrop.top + rcCrop.left;
		rgiOffset[1] = 0;
		rgiOffset[2] = 0;
		break;
	default:
		return E_INVALIDARG;
	}

	memcpy(pImageDest, &cImageSrc, sizeof(SW_COMPONENT_IMAGE));
	for(INT i = 0; i < 3; ++i)
	{
		if( cImageSrc.rgpbData[i] != NULL )
		{
			pImageDest->rgpbData[i] = cImageSrc.rgpbData[i] + rgiOffset[i];
		}
	}
	pImageDest->iWidth = rcCrop.right - rcCrop.left;
	pImageDest->iHeight = rcCrop.bottom - rcCrop.top;

	return S_OK;
}

HRESULT CSWImage::CreateSWImage(CSWImage** ppImage, const SW_IMAGE_TYPE& cImageType, const INT& iWidth, const INT& iHeight, CSWMemoryFactory* pMemoryFactory,
	const DWORD& dwFrameNo /*= 0*/, const DWORD& dwRefTime /*= 0*/, const BOOL& fIsCapture /*= FALSE*/, LPCSTR szFrameName /*= NULL*/, const INT iMemSize/* = 0 */)
{
	if( NULL == ppImage )
	{
		return E_POINTER;
	}

	(*ppImage) = new CSWImage();
	if( NULL == (*ppImage) )
	{
		return E_OUTOFMEMORY;
	}

	HRESULT hr = (*ppImage)->Create(cImageType, iWidth, iHeight, pMemoryFactory, iMemSize);
	if( FAILED(hr) )
	{
		delete (*ppImage);
		(*ppImage) = NULL;
		return hr;
	}

	(*ppImage)->SetCaptureFlag(fIsCapture);
	(*ppImage)->SetFrameName(szFrameName);
	(*ppImage)->SetFrameNo(dwFrameNo);
	(*ppImage)->SetRefTime(dwRefTime);

	return S_OK;
}

/**
* @brief �������
*
* @return
* - S_OK : �ɹ�
* - E_FAIL : ʧ��
*/
HRESULT CSWImage::Clear()
{
	if( m_pMemoryFactory != NULL )
	{
		if( m_pMemory != NULL )
		{
			m_pMemoryFactory->Free(m_pMemory);
			m_pMemory = NULL;
		}
		m_pMemoryFactory = NULL;
	}
	m_fIsCreated = FALSE;
	m_szFrameName[0] = 0;
	memset(&m_cComponentImage, 0, sizeof(SW_COMPONENT_IMAGE));

	return S_OK;
}



