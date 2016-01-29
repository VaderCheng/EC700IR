#ifndef _HV_ALLINK_DEVICE_NEW_H_
#define _HV_ALLINK_DEVICE_NEW_H_

#include "HvDeviceBaseType.h"
#include "HvDeviceCommDef.h"

#ifdef HVDEVICE_EXPORTS
#define HV_API_AL extern "C" __declspec(dllexport)
#elif HVDEVICE_LIB
#define HV_API_EX
#else
#define HV_API_AL extern "C" __declspec(dllimport)
#endif


//��·����
#define AL_LINK_TYPE_JPEG_VIDEO  0xffff0001   /*JPEG ��Ƶ����·*/
#define AL_LINK_TYPE_H264_VIDEO   0xffff0002  /*H264 ��Ƶ����·*/

//��·״̬
#define AL_LINK_STATE_UNKNOWN         0xffff0400	/**< δ֪ */
#define AL_LINK_STATE_NORMAL  0xffff0401  /*����*/
#define AL_LINK_STATE_DISCONN 0xffff0402  /*�Ͽ�*/
#define AL_LINK_STATE_RECONN  0xffff0403  /*����*/


typedef PVOID HVAPI_OPERATE_HANDLE;

//�ص������Ķ���

/**
* @brief               ��Ƶ�ص�����
* @param[out]          pUserData          ��HVAPI_StartRecvH264Video���õĴ���ص��������û�����ָ��
* @param[out]          dwVedioFlag        ��Ƶ��� ����Ч���ݱ�ǡ���Ч���ݱ�ǡ���ʷ�������
* @param[out]          dwVideoType        ��Ƶ���� ����ʷ��Ƶ��ʵʱ��Ƶ
* @param[out]          dwWidth            ���
* @param[out]          dwHeight			  �߶�
* @param[out]          dw64TimeMS         ��Ƶʱ��
* @param[out]          pbVideoData        ��Ƶ֡����
* @param[out]          dwVideoDataLen     ��Ƶ֡����
* @param[out]          szVideoExtInfo     ��Ƶ������Ϣ
* @return              �ɹ�S_OK ʧ�� E_FAILE
*/
typedef INT (CDECL*  HVAPI_AL_CALLBACK_H264)(
												PVOID pUserData,  
												DWORD dwVedioFlag,
												DWORD dwVideoType, 
												DWORD dwWidth,
												DWORD dwHeight,
												DWORD64 dw64TimeMS,
												PBYTE pbVideoData, 
												DWORD dwVideoDataLen,
												LPCSTR szVideoExtInfo
												);

/**
* @brief               jpeg���ص�����
* @param[out]          pUserData          ��HVAPI_StartRecvH264Video���õĴ���ص��������û�����ָ��
* @param[out]          dwImageFlag        ͼƬ��� ����ЧͼƬ���ݡ���ЧͼƬ����
* @param[out]          dwImageType        ͼƬ���� ��������������������
* @param[out]          dwWidth            ���
* @param[out]          dwHeight			  �߶�
* @param[out]          dw64TimeMS         JPEGʱ��
* @param[out]          pbImageData        ͼƬ֡����
* @param[out]          dwImageDataLen     ͼƬ֡����
* @param[out]          szImageExtInfo     JPEG������Ϣ
* @return              �ɹ�S_OK ʧ�� E_FAILE
*/

typedef INT (CDECL*  HVAPI_AL_CALLBACK_JPEG)(
											PVOID pUserData,  
											DWORD dwImageFlag,
											DWORD dwImageType, 
											DWORD dwWidth,
											DWORD dwHeight,
											DWORD64 dw64TimeMS,
											PBYTE pbImageData, 
											DWORD dwImageDataLen,
											LPCSTR szImageExtInfo
											);






HV_API_AL HRESULT CDECL HVAPI_AL_InitActiveModule();
HV_API_AL HRESULT CDECL HVAPI_AL_UnInitActiveModule();

typedef INT (CDECL* HVAPI_AL_ONLINE_NOTICE)(PVOID pUserData, HVAPI_OPERATE_HANDLE handle, char*szDeviceNo, char*szAddr,int iPort,char *szOtherXmlInfo);
typedef INT (CDECL* HVAPI_AL_OFFLINE_NOTICE)(PVOID pUserData, HVAPI_OPERATE_HANDLE handle, char* szDeviceNo);

HV_API_AL HRESULT HVAPI_AL_DisConnect(HVAPI_OPERATE_HANDLE handle);

HV_API_AL HRESULT HVAPI_AL_AddRef(HVAPI_OPERATE_HANDLE handle);
HV_API_AL HRESULT HVAPI_AL_ReleaseRef(HVAPI_OPERATE_HANDLE handle);


HV_API_AL HRESULT HVAPI_AL_RegDevOnLineNotice(HVAPI_AL_ONLINE_NOTICE onLineNotice, void* pUserData);
HV_API_AL HRESULT HVAPI_AL_RegDevOffLineNotice(HVAPI_AL_OFFLINE_NOTICE offLineNotice, void* pUserData);




HV_API_AL HRESULT HVAPI_AL_OpenDevCtrlServer(int iPort, int iMaxCount);

HV_API_AL HRESULT HVAPI_AL_CloseDevCtrlServer();


HV_API_AL HRESULT HVAPI_AL_GetDevConfigInfo(HVAPI_OPERATE_HANDLE handle, LPSTR szParamDoc, INT nBufLen, INT* pnRetLen);

HV_API_AL HRESULT HVAPI_AL_UploadDevConfigInfo(HVAPI_OPERATE_HANDLE handle, LPCSTR szParamDoc);

HV_API_AL HRESULT CDECL HVAPI_AL_ExecCmdOnDev(HVAPI_OPERATE_HANDLE handle,LPCSTR szCmd,LPSTR szRetBuf,INT nBufLen,INT* pnRetLen);

/**
* @brief              ȡ���˿���������(���ֻ��ȡһ��������)
* @param[in]          hHandle          �豸���
* @param[in]          dw64StartTime    ��Ӧ��ȡ��������Ϣ����ʼʱ��,Ϊ��1970-01-01 00:00:00��ʼ��Ŀǰ�������ĺ�����
* @param[in]          dw64EndTime      ��Ӧ��ȡ��������Ϣ�Ľ���ʱ��,Ϊ��1970-01-01 00:00:00��ʼ��Ŀǰ�������ĺ�����
* @param[out]         szRetInfo		   ��Ӧʹ����������ڴ����򣬺������óɹ����������Ϣ�����ڴ�
* @param[in,out]      iLen		       szRetInfo���������ȣ��������غ�Ϊ�ַ���ʵ�ʳ���
* @return             �ɹ���S_OK��ʧ�ܣ�E_FAIL
*/
HV_API_AL HRESULT CDECL HVAPI_AL_GetPCSHumanTrafficInfo(HVAPI_OPERATE_HANDLE hHandle, DWORD64 dw64StartTime, DWORD64 dw64EndTime, CHAR* szRetInfo, INT* iLen);

HV_API_AL HRESULT CDECL HVAPI_AL_GetCaptureImage(HVAPI_OPERATE_HANDLE hHandle,int nTimeM, BYTE *pImageBuffLen, int iBuffLen, int *iImageLen, DWORD64 *dwTime,  DWORD *dwImageWidth, DWORD *dwImageHeigh);


/**
* @brief              ����H264����
* @param[in]          hHandle				�豸���
* @param[in]          pFunc				    �ص�����ָ��
* @param[in]          pUserData			    �û�����
* @param[in]          iVideoID				��Ƶͨ����Ŀǰֻʹ��0
* @param[in]          dw64BeginTimeMS       ��ʼʱ��
* @param[in]          dw64EndTimeMS         ����ʱ�� 
* @param[in]          dwRecvFlag            ���ձ�־ :ʵʱ��Ƶ����ʷ��Ƶ
* @return             �ɹ���S_OK��ʧ�ܣ�E_FAIL
*/
HV_API_AL HRESULT CDECL HVAPI_AL_StartRecvH264Video(
	HVAPI_OPERATE_HANDLE hHandle,
	PVOID pFunc,
	PVOID pUserData,
	INT iVideoID ,       
	DWORD64 dw64BeginTimeMS,
	DWORD64 dw64EndTimeMS,
	DWORD dwRecvFlag
);
/**
* @brief              ֹͣH264����
* @param[in]          hHandle          �豸���
* @return             �ɹ���S_OK��ʧ�ܣ�E_FAIL
*/
HV_API_AL HRESULT CDECL  HVAPI_AL_StopRecvH264Video(HVAPI_OPERATE_HANDLE hHandle);



/**
* @brief              ����MJPEG����
* @param[in]          hHandle				 �豸���
* @param[in]          pFunc					 �ص�����ָ��
* @param[in]          pUserData				 �û�����
* @param[in]          iVideoID				 ��Ƶͨ����Ŀǰֻʹ��0
* @param[in]          dwRecvFlag			 ���ձ�־��������������������
* @return             �ɹ���S_OK��ʧ�ܣ�E_FAIL
*/

HV_API_AL HRESULT CDECL HVAPI_AL_StartRecvMJPEG(
					 HVAPI_OPERATE_HANDLE hHandle,
					 PVOID pFunc,
					 PVOID pUserData,
					 INT iVideoID ,
					 DWORD dwRecvFlag
					 );
/**
* @brief              ֹͣMJPEG����
* @param[in]          hHandle          �豸���
* @return             �ɹ���S_OK��ʧ�ܣ�E_FAIL
*/
HV_API_AL HRESULT CDECL  HVAPI_AL_StopRecvMJPEG(HVAPI_OPERATE_HANDLE hHandle);


//��ȡ��·״̬
HV_API_AL HRESULT CDECL HVAPI_AL_GetDevLinkStatus(HVAPI_OPERATE_HANDLE handle, DWORD dwLinkType, DWORD *dwStatus);


#endif