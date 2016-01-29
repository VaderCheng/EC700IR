#ifndef _HV_SYS_INTERFACE_
#define _HV_SYS_INTERFACE_

#include "swBaseType.h"
#include "swObjBase.h"

BOOL MyGetIpDWord(LPSTR lpszIP, DWORD32& dwIP);
BOOL MyGetIpString(DWORD32 dwIP, LPSTR lpszIP);

extern const char* PSZ_DSP_BUILD_NO;
extern const char* DSP_BUILD_DATE;
extern const char* DSP_BUILD_TIME;


namespace HvSys
{
	typedef enum _SYS_INFO_TYPE
	{
		//ϵͳ��Ϣ 0x00 ~ 0x1F
		INFO_SYS_ID = 0x00,	//ϵͳID
		INFO_SYS_NAME = 0x01,	//ϵͳ����
		INFO_SYS_VERSION = 0x02,	//�汾��
		INFO_PRODUCT_NAME = 0x03,	//��ȡ�豸���

		//ģʽ��Ϣ 0x20 ~ 0x3F
		INFO_BOOTMODE = 0x20,	//��λģʽ

		INFO_FONTMODE = 0x21,	//���úͶ�ȡϵͳ�ַ���ѡ��
		INFO_FONTMODE_LIST = 0x22,	//�ַ��������б�,��ָ�����

		INFO_WORKMODE = 0x23,	//��ǰ����ģʽ
		INFO_WORKMODE_LIST = 0x24,	//����ģʽ�б�,��ָ�����

		//ϵͳ���� 0x40 ~ 0x5F
		INFO_RESET_COUNT = 0x40, //��ȡϵͳ��λ����		
		INFO_SYS_LED = 0x41,		//���úͶ�ȡϵͳָʾ��
		INFO_SYS_TIME = 0x42,		//ϵͳʱ��
		INFO_SYS_PWD = 0x43,	//����

		//�����ļ� 0x60 ~ 0x7F
		INFO_FILELIST = 0x60,	//��ȡָ�����͵��ļ��б�У����Ϣ
		INFO_PRODUCTION_REPORT = 0x61,	//��������
		INFO_RESET_REPORT = 0x62	//��λ��¼����
	}
	SYS_INFO_TYPE;
	
	typedef struct _SYS_INFO
	{
		SYS_INFO_TYPE nType;

		union _Info
		{
			struct _SysID
			{
				DWORD32 IDLow;
				DWORD32 IDHigh;
			} 
			SysID;

			struct _SysName
			{
				char* pbNameBuf;
				UINT nLen;
			} 
			SysName;

			struct _SysVersion
			{
				DWORD32 dwVersion;
			} 
			SysVersion;

			struct _ProductName
			{
				char* pbNameBuf;
				UINT nLen;
			} 
			ProductName;

			struct _BootMode
			{
				DWORD32 dwMode;
				DWORD32 dwBootPrg;
			} 
			BootMode;

			struct _FontMode
			{
				DWORD32 dwMode;
				DWORD32 dwModeCount;
			} 
			FontMode;

			struct _FontModeList
			{
				int nMode;
				char* pbListBuf;
				UINT nLen;
			} 
			FontModeList;		

			struct _WorkMode
			{
				DWORD32 dwMode;
				DWORD32 dwModeCount;
			} 
			WorkMode;

			struct _WorkModeList
			{
				int nMode;
				char* pbListBuf;
				UINT nLen;
			} 
			WorkModeList;

			struct _ResetCount
			{
				DWORD32 dwCount;
			} 
			ResetCount;

			struct _SysLed
			{
				DWORD32 dwFlag;
			} 
			SysLed;

			struct _SysTime
			{
				DWORD32 dwTimeLow;
				DWORD32 dwTimeHigh;
			} 
			SysTime;

			struct _SysPWD
			{
				DWORD32 dwType;
				BYTE8* pbPWDBuf;
				UINT nLen;
			} 
			SysPWD;

			struct _FileList
			{
				DWORD32 dwFlag;
				DWORD32* pListBuf;
				UINT nLen;
			} 
			FileList;

			struct _ProductReport
			{
				char* pbReport;
				UINT nLen;
			} 
			ProductReport;

			struct _ResetReport
			{
				char* pbReport;
				UINT nLen;
			} 
			ResetReport;
		}Info;
	}
	SYS_INFO;
	
	typedef enum _MEDIUM_TYPE
	{
		MEDIUM_UNKNOWN = 0x00,
		MEDIUM_FLASH = 0x01,
		MEDIUM_EEPROM = 0x02,
		MEDIUM_NETVIDEO = 0x03
	}
	MEDIUM_TYPE;
	
	typedef enum SYSIO_CMD
	{
		//�ļ�ϵͳ��� 0x00~0x1f
		CMD_FILESYS_FORMAT = 0x00,
		CMD_FILESYS_PACK = 0x01,
		CMD_SET_FILEINFO = 0x02,
		CMD_GET_FILEINFO = 0x03,
		CMD_GET_FILESYSINFO = 0x04,
		CMD_CLEAR_TEST_INFO = 0x05, 
		CMD_CHECK_DISK = 0x06, 
		CMD_GET_CHECK_DISK = 0x07, 
		
		//�����������
		CMD_PUT_FILE = 0x20,
		CMD_GET_FILE = 0x21,
		CMD_PUT_DAT = 0x22,
		CMD_GET_DAT = 0x23,
		CMD_GET_BUF =0x24
	}
	SYSIO_CMD;
	
	typedef struct _SYSIO_INFO
	{
		SYSIO_CMD nCmdID;
		MEDIUM_TYPE nMediumType;
		
		union _Info
		{	
			struct _FILE_SYS_INFO
			{
				DWORD32 dwFileSysInfo;
			}
			FileSysInfo;

			struct _FILE_INFO
			{
				DWORD32 dwFlag;
				DWORD32 dwFileID;
				char* szFileInfo;
				DWORD32 dwFileVer;
				DWORD32 dwMaxLen;
			}
			FileInfo;
			
			struct _PUT_FILE_INFO
			{
				DWORD32 dwFileID;
				PVOID pFileDat;
				UINT nFileLen;
				DWORD32 dwFileVer;
				char* szFileInfo;
				DWORD32 dwFlag;
			}
			PutFileInfo;
			
			struct _GET_FILE_INFO
			{
				MEDIUM_TYPE nType;
				DWORD32 dwFileID;
				DWORD32 dwVer;
				PVOID pFileDatBuf;
				UINT nLen;
				DWORD32 dwFlag;
			}
			GetFileInfo;
			
			struct _PUT_DAT_INFO
			{
				DWORD32 dwAddr;
				PVOID pDatBuf;
				UINT nLen;
				DWORD32 dwFlag;
			}
			PutDatInfo;
			
			struct _GET_DAT_INFO
			{
				DWORD32 dwAddr;
				PVOID pDatBuf;
				UINT nLen;
				DWORD32 dwFlag;
			}
			GetDatInfo;	
		}Info;
	}
	SYSIO_INFO;

	typedef enum _PARAM_APPEND
	{
		ADD_OVERWRITE = 0x00,	//����ԭ����
		ADD_NEWONLY = 0x01,	 //ֻ׷������
		REPLACE_ALL = 0x02	//�滻ȫ��
	}
	PARAM_APPEND;
	
	typedef enum _PARAM_RESET
	{
		RESET_ALL = 0x00,

		KEEP_ADDR = 0x01,
		KEEP_SYS = 0x02,
		KEEP_HVDSP = 0x04,
		KEEP_TRACKER = 0x08,
		CLEAR_CFG_ALL = 0xFF
	}
	PARAM_RESET;

	typedef enum _PWD_TYPE
	{
		PWT_SETTING = 0x00,
		PWT_DEBUG = 0x01,
		PWT_MAINTAIN = 0x02
	}
	PWD_TYPE;

	interface ISysControl
	{	
		//ϵͳ��Ϣ
		STDMETHOD(GetSysInfo)(
			SYS_INFO* pInfo
			) = 0;
			
		STDMETHOD(SetSysInfo)(
			SYS_INFO* pInfo
			) = 0;
			
		//��ַ��Ϣ
		STDMETHOD(GetTcpipAddr)( 
			BYTE8 *pMac,
			DWORD32 *pdwIP, 
			DWORD32 *pdwMask, 
			DWORD32 *pdwGateway 
			) = 0;
			
		STDMETHOD(SetTcpipAddr)( 
			const BYTE8 *pMac,
			DWORD32 dwIP, 
			DWORD32 dwMask, 
			DWORD32 dwGateway 
			) = 0;

		STDMETHOD(GetStdPtlParam)(
			BYTE8 *pucgMode, 
			WORD16 *pwgLocalPort, 
			WORD16 *pwRemotePort, 
			DWORD32 *pdwRemoteAddr 
			) = 0;

		STDMETHOD(SetStdPtlParam)(
			BYTE8 ucMode, 
			WORD16 wLocalPort, 
			WORD16 wRemotePort, 
			DWORD32 dwRemoteAddr
			) = 0;
			
		STDMETHOD(GetMacAddr)(
			BYTE8* rgMac	//����Ϊ6�ֽڵĵ�ַ����
			) = 0;
			
		//��λ
		STDMETHOD(ResetHv)(
			DWORD32 dwFlag = 0
			) = 0;

		//�ļ�ϵͳ����
		STDMETHOD(SendIOCommand)(
			SYSIO_INFO* pInfo
			) = 0;
			
		//��������
		STDMETHOD(GetParamDat)(
			const char* szSection,
			const char* szKey,
			BYTE8* pbParamBuf,
			UINT* pBufLen
			) = 0;
			
		STDMETHOD(SetParamDat)(
			const char* szSection,
			const char* szKey,
			BYTE8* pbParamBuf,
			UINT nBufLen,
			DWORD32 dwMode
			) = 0;
			
		STDMETHOD(ResetParam)(
			DWORD32 dwMode
			) = 0;

		STDMETHOD(SetParam)(
			const char* szParamCmd, 
			BYTE8* pRetBuf, 
			UINT* pRetLen
			) = 0;

		// ��ȡIni
		STDMETHOD(GetIniFile)(
			const char* szBuffer,
			UINT* pnBufLen
			) = 0;

		// ����Ini
		STDMETHOD(SetIniFile)(
			const char* szBuffer,
			UINT nBufLen
			) = 0;

		// д��������
		STDMETHOD(ComWriteData)(	
			UINT nComPort,	// ��ʾ��ǰ�����Ĵ��ڶ˿ں�.
			const PBYTE8 pbData,	// ָ����Ҫд�������
			DWORD32 dwBytesToWrite,	// ����д��������ֽڸ���.
			PDWORD32 pdwBytesWrite	// ʵ��д��������ֽڸ���,ΪNULL����.
			) = 0;

		// ����������
		STDMETHOD(ComReadData)(
			UINT nComPort,	// ��ʾ��ǰ�����Ĵ��ڶ˿ں�.
			PBYTE8 pbData,	// ָ�����������
			DWORD32 dwBytesToRead,	// ����������ַ���Ŀ.
			PDWORD32 pdwBytesRead	// ʵ�ʶ������ַ�����,���ΪNULL����.
			) = 0;

		// ��ȡ����IP��Ϣ
		STDMETHOD(GetConnectedIP)(
			DWORD32* pdwCount, 
			DWORD32* pdwIP, 
			DWORD32* pdwType
			) = 0;

		// ����
		STDMETHOD(ForceSend)(DWORD32 dwVideoID) = 0;

		// FLASH��CRCУ��
		STDMETHOD(FlashCrcCheck)(
			DWORD32 dwStartAddr,
			DWORD32 dwTryTimes,
			DWORD32* pdwTryCount,
			DWORD32* pdwCrcValue
		) = 0;

		// ͣ����ģʽ���5V��������
		STDMETHOD(Send5VTrigger)(DWORD32 dwVideoID) = 0;

		STDMETHOD(AT88_InitConfig)(
			unsigned char* pbSecureCode,
			unsigned char* pbNc,
			unsigned char* pbReadOnlyData
			) = 0;

		STDMETHOD(At88_WriteFuse)(
			unsigned char* pbSecureCode
			) = 0;
		
		// ����JPGE��ѹ
		STDMETHOD(TestJpeg)(
			DWORD32* pdwJpegLen,
			unsigned char* pdwJpegBuf
			) = 0;

		//���У��
		STDMETHOD(CheckIdentifyPassword)(
			char* pszPassword,
			BOOL &fSuccessed
			) = 0;

		// �Ϻ���������Э��
		STDMETHOD(GetJiaojiRecoDevCount)(
			DWORD32* pdwRecDevCount,
			DWORD32* pdwRecDevAddr
			) = 0;

		STDMETHOD(GetJiaojiRecoDevState)(
			DWORD32 dwRecDevAddr,
			DWORD32* pdwRecDevState
			) = 0;

		STDMETHOD(GetJiaojiResultData)(
			DWORD32 dwTimeLow,
			DWORD32 dwTimeHigh,
			DWORD32 dwIndex,
			char* pszPlateInfo,
			DWORD32* pdwSnapImageSize,
			PBYTE8 pbSnapImageData,
			DWORD32* pdwCaptureImageSize,
			PBYTE8 pbCaptureImageData
			) = 0;

		STDMETHOD(GetDetectInfoData)(
			DWORD32& dwTimeLow,
			DWORD32& dwTimeHigh,
			char* pszDetectInfo
			) = 0;

		STDMETHOD(GetFlashUsedSize)(
			DWORD32 *pdwSize
			) = 0;
	};

	#define NOT_SEND_RESULT 0x1000

	typedef struct _HV_CMD_INFO
	{
		DWORD32 dwFlag;		//��һ���ֽ���Ϊ���ͱ�ʶ
		DWORD32 dwCmdID;
		DWORD32 dwArgLen;
	}
	HV_CMD_INFO;

	typedef struct _HV_CMD_RESPOND
	{
		DWORD32 dwFlag;
		DWORD32 dwCmdID;
		int nResult;
		DWORD32 dwArgLen;
	}
	HV_CMD_RESPOND;

	interface ICmdDataLink
	{
		STDMETHOD(RecvCmdHeader)(
			HV_CMD_INFO *pInfo
			) = 0;
		STDMETHOD(SendRespond)(
			HV_CMD_RESPOND* pInfo
			) = 0;
		STDMETHOD(ReceiveData)(
			PVOID pbBuf,
			UINT nLen,
			UINT* pReceivedLen
			) = 0;
		STDMETHOD(SendData)(
			PVOID pbBuf,
			UINT nLen,
			UINT* pSendLen
			) = 0;
	};

	interface ICmdProcess
	{
		STDMETHOD(Process)(
			HV_CMD_INFO* pCmdInfo,
			ICmdDataLink* pCmdDataLink
			) = 0;
	};
}

#endif

