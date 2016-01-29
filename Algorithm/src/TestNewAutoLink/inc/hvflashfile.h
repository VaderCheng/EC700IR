#ifndef _HV_FLASH_FILE_H
#define _HV_FLASH_FILE_H

#ifdef __cplusplus
	extern "C" {
#endif	//#ifdef __cplusplus

#include "dspsys_def.h"

/*	Flash����˵��:
		Ϊ�˶��ϲ����ε�FLASH�Ĳ���,��FLASHʹ��Cache��ʽ��д����.
	���ⲿ�洢��SDRAM��ΪFLASH����һ���ڴ�����128K,���ϲ�Ҫ��ȡFLASHʱ,�Ƚ��ö�FLASH�����ڴ�,
	�ٽ�����õ��û���Ҫ���ڴ�����,���û���Ҫ�޸ĸö�FLASH�е��κ�һ������ʱ,ֱ���޸��ڴ��е�
	����,��FLASH���ʿ����128K����ʱ,�ײ��жϸö��ڴ��Ƿ��޸�,����޸���ֱ�ӽ��ö��ڴ��е�
	����д����FLASH�ж�Ӧ��;�û���һЩ�ǳ����е�FLASH��(���ݱȽϹؼ�),���ϲ�ִ����д����֮��
	Ӧ��������Flash_flush����,���ڴ��е���ӦFLASH���ڲ����ϸ�����FLASH.
		ʹ����CACHE��ʽ��ʵ��FLASH��д����,��Ȼ����ʵ�ֶ�FLASH�����д����,����ͬʱҲ������
	����һ������,���ܻ�������flushʱ,ϵͳ��λ�����,������Ϣ��ʧ.ʵ�ִ˲���,��Ҫ����Ӳ��FLASH
	��������ϲ�֮�����һ���ӿڲ�,����Ҫ����µ�����Flash_flush����,�������µ�ǰ����.
*/
typedef enum {
	FILE_PROGRAM_START_NUM		= 1,		//1	~	10:			�û������ļ�1~10.
	FILE_PROGRAM_STOP_NUM		= 10,
	FILE_DETMODEL_START_NUM		= 11,		//11	~	30:		��ⲿ��ģ���ļ�1~20.
	FILE_DETMODEL_STOP_NUM		= 30,
	FILE_RECOGMODEL_START_NUM	= 31,		//31	~	70:		ʶ�𲿷�ģ���ļ�1~40.
	FILE_RECOGMODEL_STOP_NUM	= 70,
	FILE_LOG_START_NUM			= 71,		//71	~	100:	LOG�����ļ�1~30.
	FILE_LOG_STOP_NUM			= 100,
	FILE_CONFIG_START_NUM		= 101,		//101	~	140:	���������ļ�1~40.	
	FILE_CONFIG_STOP_NUM		= 140,	
	FILE_DETMODEL_START_NUM_BAK	= 511,		//511	~	530:	��ⲿ��ģ�ͱ����ļ�1~20.
	FILE_DETMODEL_STOP_NUM_BAK	= 530,
	FILE_RECOGMODEL_START_NUM_BAK	= 531,	//531	~	570:	ʶ�𲿷�ģ�ͱ����ļ�1~40.
	FILE_RECOGMODEL_STOP_NUM_BAK	= 570,
	MAX_FILE_NUMBER				= 1000
} FILE_SERIAL_NUM_DEF;

#define MAX_FILE_STRING_LEN			32

/* ���ݽṹ˵��:�ýṹΪ�ļ�ϵͳ�ṹ����,���ڹ����ļ�ϵͳ. */
typedef struct _FILE_SYSTEM {
	DWORD32 		dwSize;				//�ļ�ϵͳ���С,���ֽ�Ϊ��λ.
	DWORD32 		dwVer;				//�ļ�ϵͳ�汾��,Ϊ�����Կ���.
	DWORD32 		dwMainFileTablePos;	//�ļ�����������ʼλ��.
	DWORD32 		dwBakFileTablePos;	//�ļ����ݷ�������ʼλ��.
	DWORD32 		dwFileBlockPos;		//�ļ���ŵ���ʼλ��.
	DWORD32 		dwFileTableSize;	//�ļ������Ĵ�С,���ֽ�Ϊ��λ.
	DWORD32 		dwReserved[4];		//�ļ�ϵͳ������.
	DWORD32 		dwCheckCode;		//�ļ�ϵͳ������У���.
} FILE_SYSTEM;


/* ���ݽṹ˵��:�ýṹΪ�ļ������ṹ����,���ڹ����ļ�. */
typedef struct _FILE_TAB {
	DWORD32 		dwSize;				//�ļ�������С,���ֽ�Ϊ��λ.
	DWORD32 		dwVer;				//�ļ������İ汾��.
	DWORD32 		dwActiveFile;		//��ǰ�ļ�ϵͳ���ж��ٸ���Ч�ļ�.	
	DWORD32 		dwMaxFileCount;		//��ǰ�ļ��������֧�ֵ�����ļ�����.
	DWORD32 		dwFileHeaderSize;	//�ļ�ͷ��С,���ֽ�Ϊ��λ.
	DWORD32 		dwTrailingBlockPos;	//FLASH�еĿ����������ʼλ��.
	DWORD32 		dwTrailingBlockLen;	//��ǰ��һ�µ�����FLASH�ռ��ܺ�.
	DWORD32 		dwReserved[4];		//�ļ����������.
	DWORD32 		dwCheckCode;		//�ļ����������У���.
} FILE_TAB;


typedef struct _FILE_HEADER {
	DWORD32 		dwVer;				//�ļ��汾��,�������ֵ�ǰ�������ļ��İ汾��,�ð汾�ŵ�������ʹ�ø��ļ����û�����.
	WORD16 			wFlag;				//�ļ���־,�����־Ϊ0x0101��ʾ���ļ���Ч,�����κ�ֵ���Ӹ��ļ���Ч.
	WORD16 			wSerialNumber;		//�����ļ����ͽ��ļ����.
	char 			szName[ MAX_FILE_STRING_LEN ];	//�ļ���ʶ��Ϣ,��������'\0'Ϊ������־��.
	DWORD32 		dwFileStartPos;		//�ļ���FLASH�д�ŵ���ʼλ��.
	DWORD32 		dwNowLen;			//�ļ���ǰ�ĳ���,���ļ�ָ��,����������dwMaxLength����Ϊ�Ƿ�,��ʾ���ļ������쳣.
	DWORD32 		dwMaxLength;		//�ļ�����󳤶�,���ֽ�Ϊ��λ.
	DWORD32			dwFileCrc;			//�ļ�CRCֵ
	DWORD32 		dwReserved[2];		//Ϊ�ļ�ͷ�ı����ֶ�,��ǰ���ñ����ֶ�ȫ����Ϊ0.
} FILE_HEADER;


/* ö�ٽṹ˵��:�ļ��򿪷�ʽ.	*/
typedef enum {
	FILE_READ 			= 0x01,			//����ʽ,ͬʱ����֧�ֶ������������; 
	FILE_WRITE 			= 0x02,			//д��ʽ,���ҽ���ֻͬʱ֧��һ��д��������;
	FILE_WRITE_CLEAR 	= 0x03			//д�����ʽ,��ǰ�ļ�ȫ�����;
} FILE_OPEN_MODE;


/* ö�ٽṹ˵��:�ļ�ָ���ƶ���ʽ(�����ʼ��). */
typedef enum {
	FILE_BEGIN			= 0x01,
	FILE_CURRENT		= 0x02,
	FILE_END			= 0x03	
} FILE_MOVE_METHOD;


typedef void * FILE_HANDLE;				//�ļ��������
/*-----------------------------------------------------------------------------------------------------------------------------------------------*/




/*-----------------------------------------------------------------------------------------------------------------------------------------------*/
/*
����˵��:�ú���������ʽ��FLASH���ļ�ϵͳ,�����ļ�ϵͳ���ļ������.
��������ֵ:
	����S_OK��ʾ��ʽ���ɹ�.
	����E_FAIL��ʾӲ���ײ���������쳣.
	����S_FALSE��ʾ��ʽ��ʧ��. */
extern HRESULT FILE_FormatFileSystem( void );

/*
����˵��:�ú�����������FLASH�ϵ��ļ�,���ļ����ļ��м�Ŀ�϶�ռ伯����FLASH����.
��������ֵ:
	����S_OK��ʾ�ļ�����ɹ�.
	����E_FAIL��ʾӲ���ײ���������쳣.
	����S_FALSE��ʾ�ļ�����ʧ��. */
extern HRESULT FILE_PackFileSystem( void );

/*
����˵��:�ú�������ȡ�õ�ǰ�ļ�ϵͳ����Ϣ.
��������ֵ:
	����S_OK��ʾ�ɹ�ȡ���ļ�ϵͳ��Ϣ.
	����E_POINTER��ʾpFileSystemInfoΪINVALID_POINTER.
	����E_FAIL��ʾ��ǰȡ��Ϣ����,û�п��õ��ļ�ϵͳ����.
	����S_FALSE��ʾȡ��Ϣʧ��. */
extern HRESULT FILE_GetFileSystemInfo( 
	FILE_SYSTEM 	*pFileSystemInfo	//ָ���ļ�ϵͳ��Ϣ.
);

/* ����˵��:�ú������������ļ�ϵͳ��Ϣ.
��������ֵ:
	����S_OK��ʾ�ɹ������ļ�ϵͳ��Ϣ.
	����E_POINTER��ʾpFileSystemInfoΪINVALID_POINTER.
	����E_FAIL��ʾ��ǰȡ��Ϣ����,û�п��õ��ļ�ϵͳ����.
	����S_FALSE��ʾ�����ļ�ϵͳ��Ϣʧ��. */
extern HRESULT FILE_SetFileSystemInfo( 
	const FILE_SYSTEM *pFileSystemInfo	//ָ���ļ�ϵͳ��Ϣ.
);

/*
����˵��:�ú�������ȡ���ļ��������Ϣ.
��������ֵ:
	����S_OK��ʾ�ɹ�ȡ���ļ��������Ϣ.
	����E_POINTER��ʾpFileTableInfoΪINVALID_POINTER.
	����E_FAIL��ʾ��ǰȡ��Ϣ����,û�п��õ��ļ�ϵͳ����.
	����S_FALSE��ʾ�����ļ�ϵͳ��Ϣʧ��. */
extern HRESULT FILE_GetFileTableInfo( 
	FILE_TAB		*pFileTableInfo		//ָ���ļ�ϵͳ��Ϣ.
);

/*
����˵��:�ú������������ļ��������Ϣ.
��������ֵ:
	����S_OK��ʾ�ɹ�ȡ���ļ��������Ϣ.
	����E_POINTER��ʾpFileTableInfoΪINVALID_POINTER.
	����E_FAIL��ʾ��ǰȡ��Ϣ����,û�п��õ��ļ�ϵͳ����.
	����S_FALSE��ʾ�����ļ��������Ϣʧ��. */
extern HRESULT FILE_SetFileTableInfo( 
	const FILE_TAB	*pFileTableInfo		//ָ���ļ��������Ϣ.
);

/*
����˵��:�ú��������������ļ�.
��������ֵ:
	����S_OK��ʾ�������ļ�,*pFileHandle������.
	����E_POINTER��ʾpFileHandleΪINVALID_POINTER.	
	����E_INVALIDARG��ʾ�������ز����Ƿ�.
	����E_FAIL��ʾ��ǰȡ��Ϣ����,û�п��õ��ļ�ϵͳ����.	
	����S_FALSE�������ļ�ʧ��. */
extern HRESULT FILE_Create( 
	WORD16 			wFileSerialNum, 	//��Ҫ�������ļ��ļ����.
	DWORD32 		dwFileMaxSize, 		//�´������ļ���󳤶�,���ֽ�Ϊ��λ.
	FILE_HANDLE 	*pFileHandle		//ָ���´������ļ��������.
);

/*
����˵��:�ú����������Ѵ��ڵ��ļ�.
��������ֵ:
	����S_OK��ʾ���ļ��ɹ�,*pFileHandle������.
	����E_POINTER��ʾpFileHandleΪINVALID_POINTER.
	����E_INVALIDARG��ʾ�������ز����Ƿ�,�ļ���Ų��Ϸ�.
	����E_FAIL��ʾ��ǰȡ��Ϣ����,û�п��õ��ļ�ϵͳ����.	
	����S_FALSE���ļ�ʧ��,�ļ������ڻ��������쳣. */
extern HRESULT FILE_Open( 
	WORD16 			wFileSerialNum,		//��Ҫ�򿪵��ļ��ļ����.
	FILE_OPEN_MODE 	FileOpenMode,		//�ļ���ģʽ.
	FILE_HANDLE		*pFileHandle		//ָ���´򿪵��ļ��������.
);

/*
����˵��:�ú����������ļ������ָʾ���ļ��ж�ȡһ����������.
��������ֵ:
	����S_OK��ʾ�ļ���ȡ�ɹ�.
	����E_HANDLE��ʾhFileΪINVALID_HANDLE.
	����E_POINTER��ʾpbDestΪINVALID_POINTER.
	����E_INVALIDARG��ʾ�ļ������ָ����ļ����û��ʵ������.
	����E_FAIL��ʾ��ǰȡ��Ϣ����,û�п��õ��ļ�ϵͳ����.	
	����S_FALSE��ʾ�ļ���ȡʧ��. */
extern HRESULT FILE_Read( 
	FILE_HANDLE 	hFile, 				//ָ���ļ��������.
	PBYTE8 			pbDest, 			//ָ�����������.
	DWORD32			dwBytesToRead,		//��Ҫ��ȡ���ݵ��ֽ���.
	PDWORD32 		pdwBytesRead		//����ʵ�ʶ��������ݸ���,���ΪNULL����.
);

/*
����˵��:�ú����������ļ������ָʾ���ļ�д��һ����������.
��������ֵ:
	����S_OK��ʾ�ļ�д��ɹ�.
	����E_HANDLE��ʾhFileΪINVALID_HANDLE.
	����E_POINTER��ʾpbSrcΪINVALID_POINTER.
	����E_INVALIDARG��ʾ�ļ������ָ����ļ����û��ʵ������.
	����E_FAIL��ʾ��ǰȡ��Ϣ����,û�п��õ��ļ�ϵͳ����.
	����S_FALSE��ʾ�ļ�����ʧ��. */
extern HRESULT FILE_Write( 
	FILE_HANDLE 	hFile, 				//ָ���ļ��������.
	const PBYTE8 	pbSrc, 				//ָ����Ҫд�������.
	DWORD32			dwBytesToWrite,		//��Ҫд�����ݵ��ֽ���.
	PDWORD32 		pdwBytesWritten		//����ʵ��д������ݸ���,���ΪNULL����.
);

/*
����˵��:�ú��������ر��Ѵ򿪵��ļ�.
��������ֵ:
	����S_OK��ʾ�ļ��رճɹ�.
	����E_HANDLE��ʾhFileΪINVALID_HANDLE.
	����E_INVALIDARG��ʾ�ļ������ָ����ļ����û��ʵ������.
	����E_FAIL��ʾ��ǰȡ��Ϣ����,û�п��õ��ļ�ϵͳ����.
	����S_FALSE��ʾ�ļ��ر�ʧ�ܻ��������쳣. */
extern HRESULT FILE_Close( FILE_HANDLE hFile );

/*
����˵��:�ú�������ɾ���ļ�.
��������ֵ:
	����S_OK��ʾ�ļ��رճɹ�.	
	����E_INVALIDARG��ʾ��������ȷ,�ļ�������.
	����E_FAIL��ʾ��ǰȡ��Ϣ����,û�п��õ��ļ�ϵͳ����.
	����S_FALSE��ʾɾ���ļ�ʧ��. */
extern HRESULT FILE_Delete( WORD16 wFileSerialNum );

/*
����˵��:�ú��������ƶ��ļ���ǰָ��.
��������ֵ:
	����S_OK��ʾ�ļ�ָ���ƶ��ɹ�.
	����E_HANDLE��ʾhFileΪINVALID_HANDLE.
	����E_FAIL��ʾ��ǰȡ��Ϣ����,û�п��õ��ļ�ϵͳ����.
	����E_INVALIDARG��ʾ��������ȷ.
	����S_FALSE��ʾ�ļ�ָ���ƶ�ʧ��. */
extern HRESULT FILE_Seek(
	FILE_HANDLE 		hFile,				//ָ���ļ��������.
	int 				iSeekLocation,		//��ǰ�ļ�ָ��λ��.
	FILE_MOVE_METHOD	MoveMethod			//�ƶ���ʽ			
);

/*
����˵��:�ú�������ȡ���ļ��й���Ϣ.
��������ֵ:
	����S_OK��ʾ�ļ���Ϣȡ�óɹ�.
	����E_HANDLE��ʾhFileΪINVALID_HANDLE.
	����E_POINTER��ʾpdwFileVer,��pszInfoString,��pdwMaxLenΪINVALID_POINTER.
	����E_FAIL��ʾ��ǰȡ��Ϣ����,û�п��õ��ļ�ϵͳ����.
	����S_FALSE��ʾ�ļ���Ϣ��ȡʧ��. */
extern HRESULT FILE_GetFileInfo( 
	FILE_HANDLE 	hFile, 				//ָ���ļ��������.
	PDWORD32 		pdwFileVer, 		//ָ���ļ��汾��.
	PCHAR 			pszInfoString, 		//ָ���ļ������ַ�����Ϣ.
	PDWORD32 		pdwMaxLen			//ָ���ļ���󳤶�.
);

/*
����˵��:�ú������������ļ���Ϣ.
��������ֵ:
	����S_OK��ʾ�ļ���Ϣ���óɹ�.
	����E_HANDLE��ʾhFileΪINVALID_HANDLE.
	����E_POINTER��ʾpszInfoStringΪINVALID_POINTER.
	����E_FAIL��ʾ��ǰȡ��Ϣ����,û�п��õ��ļ�ϵͳ����.
	����S_FALSE��ʾ�ļ���Ϣ����ʧ��. */
extern HRESULT FILE_SetFileInfo( 
	FILE_HANDLE 	hFile, 				//ָ���ļ��������.
	DWORD32 		dwFileVer, 			//�ļ��汾��.
	const PCHAR 	pszInfoString		//ָ���ļ�������Ϣ.
);

/*
����˵��:�ú���������ȡ�ļ�CRCУ��ֵ.
��������ֵ:
	����S_OK��ʾ��ȡ�ļ�CRCУ��ֵ�ɹ�.
	����E_HANDLE��ʾhFileΪINVALID_HANDLE.
	����E_FAIL��ʾ��ȡ�ļ�CRCУ��ֵ����,û�п��õ��ļ�ϵͳ����.
	����S_FALSE��ʾ��ȡ�ļ�CRCУ��ֵʧ��. */
extern HRESULT FILE_GetFileCRC( 
	FILE_HANDLE 	hFile, 				//ָ���ļ��������.
	DWORD32* 		pdwFileCrc			//�ļ�CRCֵ
);

/*
����˵��:�ú���������ȡ�ļ�CRCУ��ֵ.
��������ֵ:
	����S_OK��ʾ��ȡ�ļ�CRCУ��ֵ�ɹ�.
	����E_HANDLE��ʾ�ļ���ʧ��.
	����E_FAIL��ʾ��ȡ�ļ�CRCУ��ֵ����,û�п��õ��ļ�ϵͳ����.
	����S_FALSE��ʾ��ȡ�ļ�CRCУ��ֵʧ��.*/ 
extern HRESULT FILE_GetFileCRCFromID( 
	DWORD32 	dwFileID, 			//ָ���ļ����ļ����.
	DWORD32* 	pdwFileCrc			//�ļ�CRCֵ
);

/*
����˵��:�ú������������ļ�CRCУ��ֵ.
��������ֵ:
	����S_OK��ʾ�����ļ�CRCУ��ֵ�ɹ�.
	����E_HANDLE��ʾhFileΪINVALID_HANDLE.
	����E_FAIL��ʾ�����ļ�CRCУ��ֵ����,û�п��õ��ļ�ϵͳ����.
	����S_FALSE��ʾ�����ļ�CRCУ��ֵʧ��. */
extern HRESULT FILE_SetFileCRC( 
	FILE_HANDLE 	hFile, 				//ָ���ļ��������.
	DWORD32 		dwFileCrc			//�ļ�CRCֵ
);

/*
����˵��:�ú���������ȡFLASH��ǰʹ��������λΪ�ֽ�.
��������ֵ:
	����S_OK��ʾ��ȡFLASH��ǰʹ�����ɹ�.
	����E_HANDLE��ʾpdwFileSizeΪINVALID_HANDLE.
	����E_FAIL��ʾ��ȡFLASH��ǰʹ��������,û�п��õ��ļ�ϵͳ����. */
extern HRESULT FILE_GetUsedSize(DWORD32 *pdwFileSize);

/*-----------------------------------------------------------------------------------------------------------------------------------------------*/

#ifdef __cplusplus
	}
#endif	//#ifdef __cplusplus

#endif	//#ifndef _HV_FLASH_FILE_H



