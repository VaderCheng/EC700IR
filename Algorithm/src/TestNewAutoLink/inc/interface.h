#ifndef __INTERFACE_H__
#define __INTERFACE_H__

#include "swImage.h"
#include "swBaseType.h"

#ifdef __cplusplus
extern "C" {
#endif		// #ifdef __cplusplus

// ����ĳ������������к������ܵķ���ֵ.
#define INVALD_POINTER				-1
#define BUSY_FIELD					-2
#define INVALID_DEVICE				-3
#define INVALID_FORMAT				-4
#define DEVICE_FAIL					-5

#define LOG_CAT_LIST(MACRO_NAME)		\
	MACRO_NAME(COMM)					\
	MACRO_NAME(DETECT)					\
	MACRO_NAME(SEGMENT)					\
	MACRO_NAME(RECOGNIZE)				\
	MACRO_NAME(TRACK)					\
	MACRO_NAME(OTHER)					\
	MACRO_NAME(ERROR)					\
	MACRO_NAME(TIME)					\
	MACRO_NAME(VOTE)					\
	MACRO_NAME(DISPLAY)					\
	MACRO_NAME(EXP_DATA)				\
	MACRO_NAME(VIDEO_DET)				\
	MACRO_NAME(DETECT_DETAIL)			\

#define DEFINE_DEBUG_DEVICE_ENUM(a)		\
	DEBUG_STR_##a##_FILE,				\

enum DebugDevices
{
	LOG_CAT_LIST(DEFINE_DEBUG_DEVICE_ENUM)
	DEBUG_STR_DEVICE_COUNT
};

extern int g_rgfDebugDeviceEnabled[ DEBUG_STR_DEVICE_COUNT ];
extern LPCSTR g_rgkszDebugDeviceName[ DEBUG_STR_DEVICE_COUNT ];
extern char g_rgkszDebugLogPath[ DEBUG_STR_DEVICE_COUNT ][ 260 ];
extern BOOL g_fSaveLogFile;

__inline LPCSTR GetDebugDeviceName( int iDeviceID )
{
	return g_rgkszDebugDeviceName[iDeviceID];
}

__inline void EnableDebugDevice( int iDeviceID, int fEnable ) 
{
	g_rgfDebugDeviceEnabled[ iDeviceID ] = fEnable;
}

__inline int IsDebugDeviceEnabled( int iDeviceID ) 
{
	return g_rgfDebugDeviceEnabled[ iDeviceID ];
}

__inline LPCSTR GetDebugLogPath(int iDeviceID)
{
	return g_rgkszDebugLogPath[iDeviceID];
}

__inline void SetSaveLogFile(BOOL fSave)
{
	g_fSaveLogFile=fSave;
}

extern int UartWriteData( BYTE8 *pDataSrc, int iDataLenb, int iType );
extern int UartReadData( BYTE8 *pDataDest, int iReadLen, int iType );
extern int UartWriteString( LPSTR pszString, int iType );

// ����ĳ��������� OutputString ��, ���� iDeviceID ���ܵ�ȡֵ.
#define NULL_DEVICE					255	//a

// �궨��: int OutputString( char *pString, int iDeviceID )
// ����: �ڲ�ͬ���豸�����"����ַ���"
// ����: pString	ָ����Ҫ������ַ���. ����ò���Ϊ NULL, �������� INVALD_POINTER
//		 iDeviceID	������Ҫ��ʲô�豸�����"����ַ���". ����ܵ�ȡֵ������ĳ�������. �����ȡֵ��������ĳ����е�
//					�κ�һ��, �򱾺������� INVALID_DEVICE.
//					iDeviceID == NULL_DEVICE ������һ���պ�, û�е����κκ���, �������κ����п���. ��ʱ pImageInfo ��
//					pImage ����ȡ�κ�ֵ, ���ᵼ�´���.
//					iDeviceID == RESULT_STR_FILE ʱ pString ����д�����ļ���. ����ļ��Ĵ��λ����ʵ�־���.
//					iDeviceID == DISPLAY_STR_COUNT ʱ pString ����ʾ�ڽ����"��������"�ı�����.
//					iDeviceID == DISPLAY_STR_RESULT ʱ pString ����ʾ�ڽ����"ʶ����"�ı�����.
//					iDeviceID == DEBUG_STR_DISPLAY1 ʱ pString ����ʾ�ڽ����"������Ϣ"�ı�����.
//					iDeviceID == DEBUG_STR_DISPLAY2 ʱ pString ����ʾ�ڽ����"������Ϣ2"�ı�����. Ŀǰû��ʵ��"������
//					Ϣ2"�ı���
//					iDeviceID == DEBUG_STR_COMM_FILE ʱ pString ����д��"ͨ�õ�����Ϣ"�ļ���. "ͨ�õ�����Ϣ"�ļ��Ĵ��
//					λ����ʵ�־���.
//					iDeviceID == DEBUG_STR_DETECT_FILE ʱ pString ����д��"��������Ϣ"�ļ���. "��������Ϣ"�ļ��Ĵ��
//					λ����ʵ�־���.
//					iDeviceID == DEBUG_STR_SEGMENT_FILE ʱ pString ����д��"�ָ������Ϣ"�ļ���. "�ָ������Ϣ"�ļ��Ĵ��
//					λ����ʵ�־���.
//					iDeviceID == DEBUG_STR_RECOGNIZ_FILE ʱ pString ����д��"ʶ�������Ϣ"�ļ���. "ʶ�������Ϣ"�ļ��Ĵ��
//					λ����ʵ�־���.
//					iDeviceID == DEBUG_STR_OTHER_FILE ʱ pString ����д��"����������Ϣ"�ļ���. "����������Ϣ"�ļ��Ĵ��
//					λ����ʵ�־���.
//					֮���Խ�������Ϣ�������ϸ�Ļ���, ����Ϊ��ͬ�׶ε���������ڲ�ͬ���߳�������, ��ϸ�Ļ��ֿ��Ա������
//					��Ϣ�ļ����������߳��ƽ�˳����������������
// ����: �ɹ�ʱ���� 0.
//		 ��� pString Ϊ NULL, �������� INVALD_POINTER
//		 ��� iDeviceID ��ȡֵ�Ƿ�, �򷵻� INVALID_DEVICE.
//		 ����޷���ָ���豸�����, �򷵻� DEVICE_FAIL.
// ע��: ����Ŀǰ��ʵ������Ԥ����׶��ж� iDeviceID �Ƿ�Ϊ���豸, ��˵��ñ���ʱ iDeviceID ������һ������, ���򽫵���
//		 �������.
int DoOutputString( char *pString, int iDeviceID );

BOOL OpenLogFile(int iDeviceID,char* lpszFileName,char* lpszMode);

BOOL CloseLogFile(int iDeviceID);

BOOL IsLogFileOpened(int iDeviceID);

#define OutputString( pString, iDeviceID )			\
	if ( iDeviceID != NULL_DEVICE ) {				\
			DoOutputString( pString, iDeviceID );	\
	}

// ����ĳ��������� OutputImage ��, ���� iDeviceID ���ܵ�ȡֵ.
#define RESULT_IMG_FILE				1
#define DISPLAY_IMG_COMM_DEV		2
#define DISPLAY_IMG_DETECT_DEV		3
#define DISPLAY_IMG_SEGMENT_DEV		4
#define DISPLAY_IMG_RECOGNIZ_DEV	5
#define DEBUG_IMG_FILE				NULL_DEVICE

// �궨��: int OutputImage( char *pImageInfo, HV_IMAGE *pImage, int iDeviceID )
// ����: �ڲ�ͬ���豸������Ƿ����洢��ʽ��"���ͼ��"
// ����: pImageInfo	ָ����Ҫ�����"���ͼ��"���й���Ϣ. �� iDeviceID ȡĳЩ�ض�ֵʱ������������, ��ʱ��ȡֵ����Ϊ NULL,
//					����������ָ���ض���ʽ���ַ���, ��� iDeviceID ���й�˵��.
//		 pImage		ָ����Ҫ����ķǷ���"���ͼ��". ����ò���Ϊ NULL, ���������� INVALD_POINTER.
//		 iDeviceID	������Ҫ��ʲô�豸�����"���ͼ��". ����ܵ�ȡֵ������ĳ�������. �����ȡֵ��������ĳ����е�
//					�κ�һ��, �򱾺귵�� INVALID_DEVICE.
//					iDeviceID == NULL_DEVICE ������һ���պ�, û�е����κκ���, �������κ����п���. ��ʱ pImageInfo ��
//					pImage ����ȡ�κ�ֵ, ���ᵼ�´���.
//					iDeviceID == RESULT_IMG_FILE ʱ pImageInfo ��Ӧָ��һ���Ϸ����ļ���(����Ŀ¼��), ���������� pImage
//					ָ����ͼ�󱣴��ڸ��ļ���.
//					iDeviceID == DEBUG_IMG_FILE ʱ pImageInfo ��Ӧָ��һ���Ϸ����ļ���(����Ŀ¼��), ���������� pImage
//					ָ����ͼ�󱣴��ڸ��ļ���.
//					iDeviceID == DISPLAY_* ʱ pImageInfo ����Ϊ NULL, �������Ϊ NULL, ���������Ѹ��ַ�����ʾ��ͼ������
//					���ı�����. ���������û��Ϊ��ͼ���������Ӧ���ı���, �� pImageInfo ������.
// ����: �ɹ�ʱ���� 0.
//		 ��� pImageInfo Ϊ NULL, �������� INVALD_POINTER
//		 ��� pImage Ϊ NULL, �������� INVALD_POINTER
//		 ��� iDeviceID ��ȡֵ�Ƿ�, �򷵻� INVALID_DEVICE.
//		 ����޷���ָ���豸�����, �򷵻� DEVICE_FAIL.
// ע��: ����Ŀǰ��ʵ������Ԥ����׶��ж� iDeviceID �Ƿ�Ϊ���豸, ��˵��ñ���ʱ iDeviceID ������һ������, ���򽫵���
//		 �������.
int DoOutputImage( char *pImageInfo, HV_COMPONENT_IMAGE *pImage, int iDeviceID );

#define OutputImage( pImageInfo, pImage, iDeviceID )		\
	if ( iDeviceID != NULL_DEVICE ) {						\
			DoOutputImage( pImageInfo, pImage, iDeviceID );	\
	}

// �궨��: int OutputComponentImage( char *pImageInfo, HV_COMPONENT_IMAGE *pImage, int iDeviceID )
// ����: �ڲ�ͬ���豸�����������ʽ��"���ͼ��"
// ����: pImageInfo	ָ����Ҫ�����"���ͼ��"���й���Ϣ. �� iDeviceID ȡĳЩ�ض�ֵʱ������������, ��ʱ��ȡֵ����Ϊ NULL,
//					����������ָ���ض���ʽ���ַ���, ��� iDeviceID ���й�˵��.
//		 pImage		ָ����Ҫ����ķ�����ʽ"���ͼ��". ����ò���Ϊ NULL, ���������� INVALD_POINTER.
//		 iDeviceID	������Ҫ��ʲô�豸�����"���ͼ��". ����ܵ�ȡֵ������ĳ�������. �����ȡֵ��������ĳ����е�
//					�κ�һ��, �򱾺귵�� INVALID_DEVICE.
//					iDeviceID ��ȡֵ�������� OutputImage ����ȫһ��.
// ����: �ɹ�ʱ���� 0.
//		 ��� pImageInfo Ϊ NULL, �������� INVALD_POINTER
//		 ��� pImage Ϊ NULL, �������� INVALD_POINTER
//		 ��� iDeviceID ��ȡֵ�Ƿ�, �򷵻� INVALID_DEVICE.
//		 ����޷���ָ���豸�����, �򷵻� DEVICE_FAIL.
// ע��: ����Ŀǰ��ʵ������Ԥ����׶��ж� iDeviceID �Ƿ�Ϊ���豸, ��˵��ñ���ʱ iDeviceID ������һ������, ���򽫵���
//		 �������.
int DoOutputComponentImage( char *pImageInfo, HV_COMPONENT_IMAGE *pImage, int iDeviceID );

#define OutputComponentImage( pImageInfo, pImage, iDeviceID )			\
	if ( iDeviceID != NULL_DEVICE ) {									\
			DoOutputComponentImage( pImageInfo, pImage, iDeviceID );	\
	}

// �궨��: int GetSystemTick( void )
// ����: ȡ��ϵͳ Tick
// ����: ��
// ����: �ɹ�ʱ����ϵͳ Tick. Ŀǰ����������.
// ע��: ϵͳ Tick ��ϵͳʵ���й�, �����ʱ�䳤�Ȳ�ȷ��. Ŀǰ�� DSP_BIOS �� WINDOWS ����ȡֵ���� 1 ms.
DWORD32 GetSystemTick( void );
void BreakPoint();
int Random();

#ifdef __cplusplus
}
#endif		// #ifdef __cplusplus


#endif		// #ifndef __INTERFACE_H__
