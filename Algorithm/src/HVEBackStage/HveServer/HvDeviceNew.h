#ifndef _HVDEVICENEW_H__
#define _HVDEVICENEW_H__

#include "HvDeviceBaseType.h"
#include "HvDeviceCommDef.h"

#ifdef HVDEVICE_EXPORTS
#define HV_API_EX extern "C" __declspec(dllexport)
#elif HVDEVICE_LIB
#define HV_API_EX
#else
#define HV_API_EX extern "C" __declspec(dllimport)
#endif

#define HVAPI_API_VERSION_EX	"2.0"

typedef PVOID HVAPI_HANDLE_EX;         /**< HvAPI������� */

/* �ص��������� */
#define CALLBACK_TYPE_RECORD_PLATE			0xFFFF0001
#define CALLBACK_TYPE_RECORD_BIGIMAGE		0xFFFF0002
#define CALLBACK_TYPE_RECORD_SMALLIMAGE		0xFFFF0003
#define CALLBACK_TYPE_RECORD_BINARYIMAGE	0xFFFF0004
#define CALLBACK_TYPE_RECORD_INFOBEGIN		0xFFFF0005
#define CALLBACK_TYPE_RECORD_INFOEND		0xFFFF0006
#define CALLBACK_TYPE_STRING				0xFFFF0007
#define CALLBACK_TYPE_JPEG_FRAME			0xFFFF0008
#define CALLBACK_TYPE_H264_VIDEO			0xFFFF0009
#define CALLBACK_TYPE_HISTORY_VIDEO			0xFFFF0010

/* ʶ������ͼ���Ͷ��� */
#define RECORD_BIGIMG_BEST_SNAPSHOT			0x0001	/**< ������ʶ��ͼ */
#define RECORD_BIGIMG_LAST_SNAPSHOT			0x0002	/**< ���ʶ��ͼ */
#define RECORD_BIGIMG_BEGIN_CAPTURE			0x0003	/**< ��ʼץ��ͼ */
#define RECORD_BIGIMG_BEST_CAPTURE			0x0004	/**< ������ץ��ͼ */
#define RECORD_BIGIMG_LAST_CAPTURE			0x0005	/**<  ���ץ��ͼ */

/* �������Ͷ��� */
#define CONN_TYPE_UNKNOWN         0xffff0000	/**< δ֪ */
#define CONN_TYPE_IMAGE           0xffff0001	/**< ͼƬ */
#define CONN_TYPE_VIDEO           0xffff0002	/**< ��Ƶ */
#define CONN_TYPE_RECORD          0xffff0003	/**< ʶ���� */
//
///* ͼƬ���� */
//#ifndef IMAGE_TYPE_UNKNOWN
//#define IMAGE_TYPE_UNKNOWN          0xffff0100	/**< δ֪ */
//#endif	
//#ifndef IMAGE_TYPE_JPEG_NORMAL
//#define IMAGE_TYPE_JPEG_NORMAL      0xffff0101	/**< Jpeg��ͨͼ */
//#endif
//#ifndef IMAGE_TYPE_JPEG_CAPTURE
//#define IMAGE_TYPE_JPEG_CAPTURE     0xffff0102	/**< Jpegץ��ͼ */
//#endif
//#ifndef IMAGE_TYPE_JPEG_LPR 
//#define IMAGE_TYPE_JPEG_LPR         0xffff0103	/**< Jpeg�������� */
//#endif
//

/**
* @brief			ʶ������ʼ�ص�����
* @param[out]		pUserData		��HVAPI_SetCallBack���õĴ���ص��������û�����ָ��
* @param[out]		dwCarID			����ID
* @return			0
*/
typedef INT (CDECL* HVAPI_CALLBACK_RECORD_INFOBEGIN)(PVOID pUserData, DWORD dwCarID);

/**
* @brief			ʶ������ʼ�ص�����
* @param[out]		pUserData		��HVAPI_SetCallBack���õĴ���ص��������û�����ָ��
* @param[out]		dwCarID			����ID
* @return			0
*/
typedef INT (CDECL* HVAPI_CALLBACK_RECORD_INFOEND)(PVOID pUserData, DWORD dwCarID);

/**
* @brief			���ƻص�����
* @param[out]		pUserData		��HVAPI_SetCallBack���õĴ���ص��������û�����ָ��
* @param[out]		dwCarID			����ID
* @param[out]		pcPlateNo		�����ַ���
* @param[out]		pcAppendInfo	���Ƹ�����Ϣ
* @param[out]		dw64TimeMS		ʶ��ʱ��
* @return			0
*/
typedef INT (CDECL* HVAPI_CALLBACK_RECORD_PLATE)(PVOID pUserData,
												 DWORD dwCarID,
												 LPCSTR pcPlateNo,
												 LPCSTR pcAppendInfo,
												 DWORD dwRecordType,
												 DWORD64 dw64TimeMS);

/**
* @brief			ʶ������ͼ�ص�����
* @param[out]		pUserData		��HVAPI_SetCallBack���õĴ���ص��������û�����ָ��
* @param[out]		dwCarID			����ID
* @param[out]		wImgType		��ͼ����
* @param[out]		wWidth			��ͼ��
* @param[out]		wHeight			��ͼ��
* @param[out]		pbPicData		��ͼ����
* @param[out]		dwImgDataLen	��ͼ���ݳ���
* @param[out]		dw64TimeMS		ʶ��ʱ��
* @return			0
*/
typedef INT (CDECL* HVAPI_CALLBACK_RECORD_BIGIMAGE)(PVOID pUserData,
													DWORD dwCarID, 
													WORD  wImgType,
													WORD  wWidth,
													WORD  wHeight,
													PBYTE pbPicData,
													DWORD dwImgDataLen,
													DWORD dwRecordType,
													DWORD64 dw64TimeMS);

/**
* @brief			ʶ��������Сͼ�ص�����
* @param[out]		pUserData		��HVAPI_SetCallBack���õĴ���ص��������û�����ָ��
* @param[out]		dwCarID			����ID
* @param[out]		wWidth			Сͼ��
* @param[out]		wHeight			Сͼ��
* @param[out]		pbPicData		Сͼ����
* @param[out]		dwImgDataLen	Сͼ���ݳ���
* @param[out]		dw64TimeMS		ʶ��ʱ��
* @return			0
*/
typedef INT (CDECL* HVAPI_CALLBACK_RECORD_SMALLIMAGE)(PVOID pUserData,
													  DWORD dwCarID,
													  WORD wWidth,
													  WORD wHeight,
													  PBYTE pbPicData,
													  DWORD dwImgDataLen,
													  DWORD dwRecordType,
													  DWORD64 dw64TimeMS);

/**
* @brief			ʶ�������ƶ�ֵͼ�ص�����
* @param[out]		pUserData		��HVAPI_SetCallBack���õĴ���ص��������û�����ָ��
* @param[out]		dwCarID			����ID
* @param[out]		wWidth			��ֵͼ��
* @param[out]		wHeight			��ֵͼ��
* @param[out]		pbPicData		��ֵͼ����
* @param[out]		dwImgDataLen	��ֵͼ���ݳ���
* @param[out]		dw64TimeMS		ʶ��ʱ��
* @return			0
*/
typedef INT (CDECL* HVAPI_CALLBACK_RECORD_BINARYIMAGE)(PVOID pUserData,
													   DWORD dwCarID,
													   WORD wWidth,
													   WORD wHeight,
													   PBYTE pbPicData,
													   DWORD dwImgDataLen,
													   DWORD dwRecordType,
													   DWORD64 dw64TimeMS);

/**
* @brief			ʶ������Ϣ�ص�����
* @param[out]		pUserData		��HVAPI_SetCallBack���õĴ���ص��������û�����ָ��
* @param[out]		pString			��Ϣ�ַ���
* @param[out]		dwStrLen		��Ϣ�ַ�������
* @return			0
*/
typedef INT (CDECL* HVAPI_CALLBACK_STRING)(PVOID pUserData, LPCSTR pString, DWORD dwStrLen);

/**
* @brief			ͼƬ�ص�����
* @param[out]		pUserData			��HVAPI_SetCallBack���õĴ���ص��������û�����ָ��
* @param[out]		pbImageData			ͼƬ����
* @param[out]		dwImageDataLen		ͼƬ���ݳ���
* @param[out]		dwImageType			ͼƬ����
* @param[out]		szImageExtInfo		ͼƬ��Ϣ
* @return			0
*/
typedef INT (CDECL* HVAPI_CALLBACK_JPEG)(PVOID pUserData,
										 PBYTE pbImageData,
										 DWORD dwImageDataLen,
										 DWORD dwImageType,
										 LPCSTR szImageExtInfo);

/**
* @brief			��Ƶ�ص�����
* @param[out]		pUserData			��HVAPI_SetCallBack���õĴ���ص��������û�����ָ��
* @param[out]		pbVideoData			��Ƶ֡����
* @param[out]		dwVideoDataLen		��Ƶ֡���ݳ���
* @param[out]		dwVideoType			��Ƶ֡����
* @param[out]		szVideoExtInfo		��Ƶ֡��Ϣ
* @return			0
*/
typedef INT (CDECL* HVAPI_CALLBACK_H264)(PVOID pUserData,
										 PBYTE pbVideoData,
										 DWORD dwVideoDataLen,
										 DWORD dwVideoType,
										 LPCSTR szVideoExtInfo);

/**
* @brief			��ʷ��Ƶ�ص�����
* @param[out]		pUserData			��HVAPI_SetCallBack���õĴ���ص��������û�����ָ��
* @param[out]		pbVideoData			��Ƶ֡����
* @param[out]		dwVideoDataLen		��Ƶ֡���ݳ���
* @param[out]		dwVideoType			��Ƶ֡����
* @param[out]		szVideoExtInfo		��Ƶ֡��Ϣ
* @return			0
*/
typedef INT (CDECL* HVAPI_CALLBACK_HISTORY_VIDEO)(PVOID pUserData,
												  PBYTE pbVideoData,
												  DWORD dwVideoDataLen,
												  DWORD dwVideoType,
												  LPCSTR szVideoExtInfo);

/**
* @brief		�����豸
* @param[out]	pdwDeviceCount			�豸��
* @return		�ɹ�S_OK ʧ�� E_FAILE
*/
HV_API_EX HRESULT HVAPI_SearchDeviceCount(DWORD32* pdwDeviceCount);

/**
* @brief		��ȡ�豸IP
* @param[in]	dwIndex			����ʱ���������λ��
* @param[out]	dw64MacAddr		�豸MAC��ַ
* @param[out]	dwIP			�豸IP
* @param[out]	dwMask			����
* @param[out]	dwGateWay		����
* @return		�ɹ�S_OK ʧ�� E_FAILE
*/
HV_API_EX HRESULT HVAPI_GetDeviceAddr(DWORD32 dwIndex, DWORD64* dw64MacAddr, DWORD32* dwIP,
								   DWORD32* dwMask, DWORD32* dwGateWay);

/**
* @brief		ͨ��MAC��ַ�����豸IP
* @param[in]	dw64MacAddr		�豸MAC��ַ
* @param[in]	dwIP			�豸IP
* @param[in]	dwMask			����
* @param[in]	dwGateWay		����
* @return		�ɹ�S_OK ʧ�� E_FAILE
*/
HV_API_EX HRESULT CDECL HVAPI_SetIPByMacAddr(DWORD64 dw64MacAddr, DWORD32 dwIP, DWORD32 dwMask, DWORD32 dwGateway);

/**
* @brief		������������Ϣ���ȡ�豸������Ϣ
* @param[in]	dwIndex			����ʱ���������λ��
* @param[out]	lpExtInfo		�豸������Ϣ����
* @param[in]	iBufLen			�豸������Ϣ���泤��
* @return		�ɹ�S_OK ʧ�� E_FAILE
*/
HV_API_EX HRESULT HVAPI_GetDeviceInfoEx(int iIndex, LPSTR lpExtInfo, int iBufLen);


/**
* @brief		��ȡ�豸����
* @param[in]	pcIP		�豸IP
* @param[out]	iDeviceType	�豸����
* @return		�ɹ�S_OK ʧ�� E_FAILE
*/
HV_API_EX HRESULT CDECL HVAPI_GetDevTypeEx(PSTR pcIP, int* iDeviceType);

/**
* @brief			���豸���
* @param[in]		szIp			�豸��IP��ַ
* @param[in]		szApiVer		��Ӧ�豸��API�汾��ע��ΪNULL��Ĭ��HVAPI_API_VERSION
* @return			�ɹ����豸�����ʧ�ܣ�NULL
*/
HV_API_EX HVAPI_HANDLE_EX CDECL HVAPI_OpenEx(LPCSTR szIp, LPCSTR szApiVer);

/**
* @brief			�ر��豸���
* @param[in]		hHandle			��Ӧ�豸����Ч���
* @return			�ɹ���S_OK��ʧ�ܣ�E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPI_CloseEx(HVAPI_HANDLE_EX hHandle);

/**
* @brief			��ȡ�����豸ʹ�õ�XMLЭ��汾
* @param[in]		hHandle			��Ӧ�豸����Ч���
* @param[out]		pfIsNewProtol	������ʹ�õ�XMLЭ��汾��falseΪ1.0�汾��trueΪ2.0�汾
* @return			�ɹ���S_OK��ʧ�ܣ�E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPI_GetXmlVersionEx(HVAPI_HANDLE_EX hHandle, bool* pfIsNewProtol);

/**
* @brief			��ȡ����״̬
* @param[in]		hHandle			��Ӧ�豸����Ч���
* @param[in]		nStreamType		���������ͣ������������ͣ�
* @param[out]		pdwConnStatus	�����ӵĵ�ǰ״̬
* @return			�ɹ���S_OK��ʧ�ܣ�E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPI_GetConnStatusEx(HVAPI_HANDLE_EX hHandle, INT nStreamType, DWORD* pdwConnStatus);

/**
* @brief			��ȡ��������
* @param[in]		hHandle				��Ӧ�豸����Ч���
* @param[in]		nStreamType			���������ͣ������������ͣ�
* @param[out]		pdwReConnectTimes	�����ӵĵ�ǰ��������
* @param[in]		fIsReset			��ȡ����Ƿ񽫸����ӵ�������������
* @return			�ɹ���S_OK��ʧ�ܣ�E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPI_GetReConnectTimesEx(HVAPI_HANDLE_EX hHandle, INT nStreamType, DWORD* pdwReConnectTimes, BOOL fIsReset);


/**
* @brief			ִ������
* @param[in]		hHandle			��Ӧ�豸����Ч���
* @param[in]		szCmd			�������豸����ĳ��������ַ���
* @param[in]		szRetBuf		���ص�ִ�н��(XML��ʽ)
* @param[in]		nBufLen			szRetBuf�������ĳ���
* @param[out]		pnRetLen		ʵ�ʷ��ص�ִ�н�����ȡ�ע��ΪNULL����Ըò�����
* @return			�ɹ���S_OK��ʧ�ܣ�E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPI_ExecCmdEx(HVAPI_HANDLE_EX hHandle,
								   LPCSTR szCmd,
								   LPSTR szRetBuf,
								   INT nBufLen,
								   INT* pnRetLen);

/**
* @brief			��ȡ�豸����
* @param[in]		hHandle			��Ӧ�豸����Ч���
* @param[in]		szParamDoc		��ȡ���Ĳ�����XML��ʽ��
* @param[in]		nBufLen			szParam�������ĳ���
* @param[out]		pnRetLen		ʵ�ʷ��صĲ������ȡ�ע��ΪNULL����Ըò�����
* @return			�ɹ���S_OK��ʧ�ܣ�E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPI_GetParamEx(HVAPI_HANDLE_EX hHandle,
									LPSTR szParamDoc,
									INT nBufLen,
									INT* pnRetLen);

/**
* @brief			�����豸����
* @param[in]		hHandle			��Ӧ�豸����Ч���
* @param[in]		szParamDoc		Ҫ���浽�豸�Ĳ�����XML��ʽ��
* @return			�ɹ���S_OK��ʧ�ܣ�E_FAIL
* @warning			���Ĺ��Ĳ���Ҫ���豸�´�������Ż���Ч
*/
HV_API_EX HRESULT CDECL HVAPI_SetParamEx(HVAPI_HANDLE_EX hHandle, LPCSTR szParamDoc);

/**
* @brief			�ϴ����ذ���ư������ļ���һ���
* @param[in]		hHandle			��Ӧ�豸����Ч���
* @param[in]		szParamDoc		Ҫ���浽�豸�Ĳ�����XML��ʽ��
* @return			�ɹ���S_OK��ʧ�ܣ�E_FAIL
* @warning			���Ĺ��Ĳ���Ҫ���豸�´�������Ż���Ч
*/
HV_API_EX HRESULT CDECL HVAPI_SendControllPannelUpdateFileEx(HVAPI_HANDLE_EX hHandle,
														PBYTE pUpdateFileBuffer,
														DWORD dwFileSize);

/**
* @brief			�������������ջص�����
* @param[in]		hHandle			��Ӧ�豸����Ч���
* @param[in]		pFunc			�ص�����ָ�롣ע��ΪNULLʱ��ʾ�ر�nStreamType���͵���������
* @param[in]		pUserData		����ص��������û�����ָ��
* @param[in]		iVideoID		��Ƶͨ����Ŀǰֻʹ��0
* @param[in]		nStreamType		�ص�����������
* @param[in]		szConnCmd		��������������
* @return			�ɹ���S_OK��ʧ�ܣ�E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPI_SetCallBackEx(HVAPI_HANDLE_EX hHandle, PVOID pFunc, PVOID pUserData,
									   INT iVideoID, INT iCallBackType, LPCSTR szConnCmd);

/**
* @brief			�����ͼ��ƼӺ�����
* @param[in]		hHandle			��Ӧ�豸����Ч���
* @param[in]		dwEnhanceStyle	�Ӻ촦��ģʽ 0���Ӻ� 1ֻ�Ӻ�Υ�½�� 2ȫ�Ӻ� 3ֻ�Ӻ�ץ��ͼ
* @param[in]		iBrightness		�Ӻ��ͼƬ������ǿ����
* @param[in]		iHubThreshold	���ɫ�ȷ�ֵ
* @param[in]		iCompressRate	�Ӻ촦���ͼƬѹ��Ʒ��
* @return			�ɹ���S_OK��ʧ�ܣ�E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPI_SetEnhanceRedLightFlagEx(HVAPI_HANDLE_EX hHandle, DWORD dwEnhanceStyle, INT iBrightness, INT iHubThreshold, INT iCompressRate);

/**
* @brief			��ʷ���JPEGͼƬ֡��ƼӺ�����
* @param[in]		hHandle			��Ӧ�豸����Ч���
* @param[in]		dwEnhanceStyle	�Ӻ촦��ģʽ 0���Ӻ� 1�Ӻ�
* @param[in]		iBrightness		�Ӻ��ͼƬ������ǿ����
* @param[in]		iHubThreshold	���ɫ�ȷ�ֵ
* @param[in]		iCompressRate	�Ӻ촦���ͼƬѹ��Ʒ��
* @return			�ɹ���S_OK��ʧ�ܣ�E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPI_SetHistoryVideoEnhanceRedLightFlagEx(HVAPI_HANDLE_EX hHandle, DWORD dwEnhanceStyle, INT iBrightness, INT iHubThreshold, INT iCompressRate);

/**
* @brief			Ԥ���ӿڣ������豸��չ�¹��ܺ��ȡ��ص���չ��Ϣ
* @param[in]		hHandle			��Ӧ�豸����Ч���
* @param[in]		dwType			��Ϣ����
* @param[in]		iBrightness		������Ϣ�Ļ����ַ
* @param[in/out]	iBrightness		������Ϣ�Ļ����ַ���Ⱥ�ʵ�ʷ�����Ϣ����
* @return			�ɹ���S_OK��ʧ�ܣ�E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPI_GetExtensionInfoEx(HVAPI_HANDLE_EX hHnadle, DWORD dwType, LPVOID pRetData, INT* iBufLen);

/**
* @brief			�ӽ��������Ϣ�л�ȡĳ����Ϣ
* @param[in]		szAppened			���������Ϣ
* @param[in]		szInfoName			��ȡ����
* @param[out]		szRetInfo			����ִ�
* @param[in]		iRetInfoBufLen		����ִ����滺������
* @return			�ɹ���S_OK��ʧ�ܣ�E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPIUTILS_GetRecordInfoFromAppenedStringEx(LPCSTR szAppened, LPCSTR szInfoName, LPSTR szRetInfo, INT iRetInfoBufLen);

/**
* @brief			�ӽ��������Ϣ����ȡȫ����Ϣÿ��Ŀ��Ϣʹ�ûس�������
* @param[in]		pszXmlPlateInfo			���������Ϣ
* @param[out]		pszPlateInfoBuf			����ִ�������
* @param[in]		iPlateInfoBufLen		����ִ�����������
* @return			�ɹ���S_OK��ʧ�ܣ�E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPIUTILS_ParsePlateXmlStringEx(LPCSTR pszXmlPlateInfo, LPSTR pszPlateInfoBuf, INT iPlateInfoBufLen);

/**
* @brief			�������Сͼ����ת����BMP��ʽ
* @param[in]		pbSmallImageData		����Сͼ����
* @param[in]		nSmallImageWidth		����Сͼ��
* @param[in]		nSmallImageHeight		����Сͼ��
* @param[out]		pbBitmapData			ת�����BMP����
* @param[out]		pnBitmapDataLen			ת�����BMP���ݳ���
* @return			�ɹ���S_OK��ʧ�ܣ�E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPIUTILS_SmallImageToBitmapEx(PBYTE pbSmallImageData, INT nSmallImageWidth, INT nSmallImageHeight, PBYTE pbBitmapData, INT* pnBitmapDataLen);

/**
* @brief			��ʶ������ֵͼת��ΪBMPλͼ
* @param[in]		pbBinImageData		ʶ������ֵͼ
* @param[in]		pbBitmapData		BMPλͼ���ݴ�Ż�����
* @param[in,out]	pnBitmapDataLen		BMPλͼ���ݴ�Ż��������ȡ��������غ���Ϊ��BMPλͼ����ʵ�ʳ���
* @return			�ɹ���S_OK��ʧ�ܣ�E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPIUTILS_BinImageToBitmapEx(PBYTE pbBinImageData,
												 PBYTE pbBitmapData,
												 INT* pnBitmapDataLen);

/**
* @brief		������ִ�н��XML�ַ����л�ȡ��Ӧ��Ϣ
* @param[in]	fIsNewXmlProtocol	XMLЭ������
* @param[in]	pszRetXmlStr		ִ������ʱ���ص�ִ�н��XML�ַ���
* @param[in]	pszCmdName			��������
* @param[in]	pszInfoName			��ȡ��Ϣ����
* @param[out]	pszInfoValue		��ȡ��Ϣ����ִ�
* @return			�ɹ���S_OK��ʧ�ܣ�E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPIUTILS_GetExeCmdRetInfoEx(BOOL fIsNewXmlProtocol, LPCSTR pszRetXmlStr, LPCSTR pszCmdName, 
													  LPCSTR pszInfoName, LPSTR pszInfoValue);

#endif
