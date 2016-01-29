///////////////////////////////////////////////////////////
//  CSWImage.h
//  Implementation of the Class CSWImage
//  Created on:      2013/2/28 14:09:52
//  Original author: zhouy
///////////////////////////////////////////////////////////

#if !defined(EA_2FAEEA12_3A58_43d4_AAB6_89C6D92AA196__INCLUDED_)
#define EA_2FAEEA12_3A58_43d4_AAB6_89C6D92AA196__INCLUDED_

#include "SWObject.h"
#include "SWBaseType.h"
#include "SWImageBase.h"
#include "SWMemoryFactory.h"
#include "SWDateTime.h"
#include "SWMutex.h"

#define IMAGE_SECURITY_CODE_LEN (16)	//ͼƬ��α�볤��

typedef struct tag_ImageExtInfo
{
	INT iShutter;	// ����
	INT iGain;		// ����
	INT iRGain;		
	INT iGGain;
	INT iBGain;
	INT iAvgY;		// ƽ������
}
IMAGE_EXT_INFO;

/**
 * @brief ͼ�����ݻ���
 */

class CSWImage : public CSWObject
{
CLASSINFO(CSWImage,CSWObject)

public:
	CSWImage();
	virtual ~CSWImage();

public:
	 /**
	 * @brief ����ͼ��
	 * 
	 * @param [in] cImageType : ͼ������
	 * @param [in] dwWidth : ͼ����
	 * @param [in] dwHeight : ͼ��߶�
	 * @param [in] pMemoryFactory : �ڴ������
	 * @param [in] iMemSize : ָ��ͼƬռ���ڴ��С������ע�⣬����ҪCSWImage���Զ�����ͼƬ��߼��������ڴ��С����ָ����ֵΪ0������ֵ��ͬ��0�����ã���ֵ����0ʱ��CSWImage����ݸ�ֵ�����ڴ���䣬��ֻ�е�һ������ָ����Ч������������ָ��ΪNULL
	 * @return
	 * - S_OK : �ɹ�
	 * - E_FAIL : ʧ��
	 */
	HRESULT Create(const SW_IMAGE_TYPE& cImageType, INT iWidth, INT iHeight, CSWMemoryFactory* pMemoryFactory, const INT iMemSize = 0);
	/**
	 * @brief ͨ��cImageSrc����ʼ������������Ѵ�����ͼ���ʽ��ֱ�����pComponentImage��һ�»��ȱ������
	 * 
	 * @param [in] cImageSrc : ԭͼ��
	 * @param [in] pMemoryFactory : �ڴ������
	 * @return
	 * - S_OK : �ɹ�
	 * - E_FAIL : ʧ��
	 */
	HRESULT Assign(const SW_COMPONENT_IMAGE& cImageSrc, CSWMemoryFactory* pMemoryFactory);
	
	/**
	 * @brief ͼ���ڴ�Ĺ���
	 * @param [in] pImage ����һ��ͼ��,ʹ�ñ�����ʹ��������ͬһ���ڴ�
	 * @return �ɹ�����S_OK������ֵ��ʾ�������
	 */
	HRESULT Attach(CSWImage *pImage, PVOID pvExif = NULL, INT iSize = 0);

	/**
	 * @brief ��δ����ͼ���滻���Ѿ������ͼ��
	 * @param [in] pMemoryFactory : �ڴ������
	 * @return �ɹ�����S_OK������ֵ��ʾ�������
	 */
	HRESULT ReplaceFrameBuffer(CSWMemoryFactory* pMemoryFactory);
	
	HRESULT AllocOverlayBuffer(INT iSize);
	BYTE*   GetOverlayBuffer(INT* iSize = NULL);
public:
	/**
	 * @brief ȡ��ͼ��ṹ��
	 * 
	 * @param [out] pComponentImage : ͼ��ṹ��
	 * @return
	 * - S_OK : �ɹ�
	 * - E_FAIL : ʧ��
	 */
	HRESULT GetImage(SW_COMPONENT_IMAGE* pComponentImage);

	/**
	 * @brief ȡ��ͼ��ṹ��(����)
	 * 
	 * @param [out] pComponentImage : ͼ��ṹ��
	 * @return
	 * - S_OK : �ɹ�
	 * - E_FAIL : ʧ��
	 */
//	HRESULT GetImage(HV_COMPONENT_IMAGE* pComponentImage);

	/**
	 * @brief ����ͼ��ṹ��
	 * 
	 * @param [in] pComponentImage : ͼ��ṹ��
	 * @return
	 * - S_OK : �ɹ�
	 * - E_FAIL : ʧ��
	 */
	HRESULT SetImage(const SW_COMPONENT_IMAGE& pComponentImage, const INT& iAddrType = 0);
	
	/**
	 * @brief ȡ��ͼ��ռ���ڴ��С
	 * 
	 * @return
	 * - ���������: �ɹ�
	 * - 0 : ʧ��
	 */
	INT GetSize();	
	/**
	 * @brief ȡ��ͼ����ֵ
	 * 
	 * @return
	 * - ���������: �ɹ�
	 * - 0 : ʧ��
	 */
	INT GetWidth();	
	/**
	 * @brief ȡ��ͼ��߶�ֵ
	 * 
	 * @return
	 * - ���������: �ɹ�
	 * - 0 : ʧ��
	 */
	INT GetHeight();	
	/**
	 * @brief ȡ��ͼ������
	 * 
	 * @return
	 * - ͼ������ö��ֵ
	 */
	SW_IMAGE_TYPE GetType();	
	/**
	 * @brief ȡ��ָ����ͼ�����ͼ��ṹ��
	 * 
	 * @param [out] pComponentImage : ͼ��ṹ��
	 * @param [in/out] cCorpRect : Ҫ��ȡ������
	 * @return
	 * - S_OK : �ɹ�
	 * - E_FAIL : ʧ��
	 */
	HRESULT GetCropImage(SW_COMPONENT_IMAGE* pComponentImage, SW_RECT& cCorpRect);
	/**
	 * @brief ȡ��ͼ�������ڴ��׵�ַ
	 * 
	 * @param [in]  : ��ַ���ͣ�0: Ĭ��������ƽ̨�йأ�1��ǿ��ȡ�����ַ��2��ǿ��ȡ�����ַ��
	 * @return
	 * �ڴ��׵�ַ��
	 */
	PBYTE GetImageBuffer(const INT& iAddrType = 0);
	/**
	 * @brief ȡ�������ڴ�Ĵ�С
	 * 
	 * @return
	 * �ڴ��С��
	 */
	DWORD GetImageBufferSize();

public:
	 /**
	 * @brief ͼ��������Խӿڡ�
	 */
	DWORD GetFrameNo()
	{
		return m_dwFrameNo;
	};
	LPSTR GetFrameName()
	{
		return m_szFrameName;
	};
	DWORD GetRefTime()
	{
		return m_dwRefTimeMS;
	};

	BOOL IsCaptureImage()
	{
		return m_fIsCaptureImage;
	};
	DWORD GetFlag()
	{
		return m_dwFlag;
	}
	BOOL IsOverlayed()
	{
		return m_fOverlay;
	}
	CSWImage* GetLastImage(VOID)
	{
		return m_pLastImage;
	}
	VOID SetFrameNo(const DWORD& dwFrameNo)
	{
		m_dwFrameNo = dwFrameNo;
	};
	VOID SetFrameName(LPCSTR lpszFrameName)
	{
		m_szFrameName[0] = 0;
		if (lpszFrameName != NULL && strlen(lpszFrameName) < MAX_FRAME_NAME_SIZE)
		{
			strcpy(m_szFrameName, lpszFrameName);
		}
	};
	VOID SetRefTime(const DWORD& dwRefTime)
	{
		m_dwRefTimeMS = dwRefTime;
		CSWDateTime::TimeConvert(m_dwRefTimeMS, &m_dwRealTimeHigh, &m_dwRealTimeLow);
	};
	HRESULT GetRealTime(DWORD* pdwTimeHigh, DWORD* pdwTimeLow)
	{
		if (NULL == pdwTimeHigh || NULL == pdwTimeLow)
		{
			return E_INVALIDARG;
		}

		*pdwTimeHigh = m_dwRealTimeHigh;
		*pdwTimeLow  = m_dwRealTimeLow;

		return S_OK;
	};
	VOID SetRealTime(const DWORD dwTimeHigh, const DWORD  dwTimeLow)
	{
		m_dwRealTimeHigh = dwTimeHigh;
		m_dwRealTimeLow = dwTimeLow;
	};
	VOID SetCaptureFlag(BOOL fIsCapture)
	{
		m_fIsCaptureImage = fIsCapture;
	};
	VOID SetFlag(DWORD dwFlag)
	{
		m_dwFlag = dwFlag;
	}
	VOID SetOverlayFlag(BOOL fOverlay)
	{
		m_fOverlay = fOverlay;
	}
	
	VOID SetLastImage(CSWImage *pImage)
	{
		m_pLastImage = pImage;
		if(m_pLastImage)
		{
			m_pLastImage->AddRef();
		}
	}

	VOID SetImageExtInfo(const IMAGE_EXT_INFO& cInfo)
	{
		swpa_memcpy(&m_cImageExtInfo, &cInfo, sizeof(m_cImageExtInfo));
	}

	HRESULT GetImageExtInfo(IMAGE_EXT_INFO* pInfo)
	{
		if( pInfo == NULL )
		{
			return E_INVALIDARG;
		}
		swpa_memcpy(pInfo, &m_cImageExtInfo, sizeof(m_cImageExtInfo));
		return S_OK;
	}
	
	//�����α��
	VOID SetSecurityCode(PBYTE pCode,INT iDataLen)
	{
		if (iDataLen > 0)
			swpa_memcpy(m_pbSecurityCode,pCode,swpa_min(iDataLen,IMAGE_SECURITY_CODE_LEN));
	}
	//��ȡ��α��
	VOID GetSecurityCode(PBYTE pCode,INT iBufLen)
	{
		if (pCode != NULL && iBufLen > 0)
		{
			swpa_memcpy(pCode,m_pbSecurityCode,swpa_min(iBufLen,IMAGE_SECURITY_CODE_LEN));
		}
	}
	
public:
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
	static HRESULT CropImage(
		const SW_COMPONENT_IMAGE &cImageSrc,
		SW_RECT& rcCrop,
		SW_COMPONENT_IMAGE *pImageDest
		);
		
	
  /**
	 * @brief ����ͼ�񣬲������Ҫ����Դ
	 *
	 * @param [out] ppImage : ͼ��ָ��
	 * @param [in] cImageType : ͼ������
	 * @param [in] dwWidth : ͼ����
	 * @param [in] dwHeight : ͼ��߶�
	 * @param [in] pMemoryFactory : �ڴ������
	 * @param [in] dwFrameNo : ͼƬ֡���
	 * @param [in] dwRefTime : ͼƬTickʱ��
	 * @param [in] fIsCapture : ץ��ͼ��־
	 * @param [in] szFrameName : ͼƬ֡��
	 * @param [in] iMemSize : ָ��ͼƬռ���ڴ��С������ע�⣬����ҪCSWImage���Զ�����ͼƬ��߼��������ڴ��С����ָ����ֵΪ0������ֵ��ͬ��0�����ã���ֵ����0ʱ��CSWImage����ݸ�ֵ�����ڴ���䣬��ֻ�е�һ������ָ����Ч������������ָ��ΪNULL
	 * @return
	 * - S_OK : �ɹ�
	 * - E_FAIL : ʧ��
	 */
	static HRESULT CreateSWImage(
									CSWImage** ppImage
									, const SW_IMAGE_TYPE& cImageType
									, const INT& iWidth
									, const INT& iHeight
									, CSWMemoryFactory* pMemoryFactory
									,	const DWORD& dwFrameNo = 0
									, const DWORD& dwRefTime = 0
									, const BOOL& fIsCapture = false
									, LPCSTR szFrameName  = NULL
									, const INT iMemSize = 0
									);

	static HRESULT CreateSWImage(
									CSWImage** ppImage
									, const SW_IMAGE_TYPE& cImageType
									, const INT& iPitch
									, const INT& iWidth
									, const INT& iHeight
									, CSWMemoryFactory* pMemoryFactory
									, const PVOID* pvAddr
									, const PVOID* pvPhys
									, const INT& iSize
									, const DWORD& dwFrameNo = 0
									, const DWORD& dwRefTime = 0
									, const BOOL& fIsCapture = FALSE
									, LPCSTR szFrameName  = NULL
									);

	 /**
	 * @brief �������
	 *
	 * @return
	 * - S_OK : ת���ɹ�
	 * - E_FAIL : ת��ʧ��
	 */
	HRESULT Clear();

private:
	SW_COMPONENT_IMAGE m_cComponentImage;
	CSWMemory* m_pMemory;
	CSWMemoryFactory* m_pMemoryFactory;
	BOOL m_fIsCreated;

	static const INT MAX_FRAME_NAME_SIZE = 256;
	DWORD m_dwFrameNo;							// ͼ��֡���
	DWORD m_dwRefTimeMS;				        // ͼ��ȡ��ʱ��ϵͳʱ��.����.
	CHAR m_szFrameName[MAX_FRAME_NAME_SIZE];	// ͼ��֡�ļ�����
	BOOL m_fIsCaptureImage;						// �Ƿ���ץ��֡��
	DWORD m_dwFlag;                   //�û��Զ����־λ
	BOOL m_fOverlay;                  //�Ƿ��Ѿ��ַ�����
	CSWImage* m_pLastImage;

	DWORD m_dwBufferSize;
	IMAGE_EXT_INFO m_cImageExtInfo;
	DWORD m_dwRealTimeHigh;
	DWORD m_dwRealTimeLow;
	
	INT   m_iOverlaySize;
	BYTE* m_pbOverlayBuffer;
	BYTE m_pbSecurityCode[IMAGE_SECURITY_CODE_LEN];

	//�����滻
	BOOL m_fBeReplaced;	//�Ƿ��ѱ��滻
	CSWMutex m_cMutexReplace;
};

#endif // !defined(EA_2FAEEA12_3A58_43d4_AAB6_89C6D92AA196__INCLUDED_)

