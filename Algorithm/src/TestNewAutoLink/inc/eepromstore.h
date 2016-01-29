#ifndef _EEPROMSTORE_H
#define _EEPROMSTORE_H

#ifdef __cplusplus
	extern "C" {
#endif	// #ifdef __cplusplus

/*
ÿ����EEPROM�д洢�����ݿ����ݽṹ����:
struct typedef _DATA_STORE_STRUCT {
	BYTE8			rgbActiveFlag[2];
	DWORD32			dwDataLen;
	BYTE8			rgbData[];
	DWORD32			dwCrcValue;
} DATA_STORE_STRUCT;
*/

// �ø������ָʾ�������ⲿ����ʧ�Դ洢���е������ļ��������.
typedef enum {
	BOOT_FILE_NUMBER		= 0,		// Bootloader��������ļ�������.
	COUNT_FILE_NUMBER		= 1,		// ϵͳ��������ֵ�ļ�������,�縴λ����ֵ.
	REPORT_FILE_NUMBER		= 2,		// �������������ļ�������.
	CONFIG_FILE_NUMBER		= 3,		// ϵͳ�������������ļ�������.
	MAX_EEPROM_FILE_NUM
} STORE_FILE_INDEX;

// ���ϵͳ��ǰ�������ļ�����������10000ʱ��ʾΪ����EEPROM�ļ�.
#define EEPROM_FILE_NUMBER_BEGIN		10000
#define EEPROM_FILE_NUMBER_END			( EEPROM_FILE_NUMBER_BEGIN + MAX_EEPROM_FILE_NUM )

/*
����˵��:�ú��������ӷ���ʧ�Դ洢����һ�ζ�����Ӧ�ļ�������Ӧ���ļ���������.
��������ֵ:
	����S_OK��ʾ��ȡ���ݳɹ�.
	����E_POINTER��ʾpbDestΪNULL.
	����E_INVALIDARG��ʾFileNumΪ�Ƿ�.
	����E_FAIL��ʾ��ǰ���ļ�����У�鲻ͨ��.
	����S_FALSE��ʾ��ǰ�ļ���������Ч. */
extern HRESULT GetFileStoreData(
	STORE_FILE_INDEX	FileNum,		// ��Ҫȡ�õ������ļ�������.
	PBYTE8				pbDest,			// ָ��Ŀ������.
	DWORD32				dwBytesToRead,	// �������������ݸ���.
	PDWORD32			pdwBytesRead	// ΪNULLʱ��ʾ������ʵ�ʶ��������ݸ���.
);

/*
����˵��:�ú������������ʧ�Դ洢����һ��д����Ҫд������,Ĭ��Ϊ����Щֱ�Ӵ����Ϊһ���ļ�.
��������ֵ:
	����S_OK��ʾд�����ɹ�.
	����E_POINTER��ʾpbSrcΪNULL.
    ����E_INVALIDARG��ʾFileNumΪ�Ƿ�.
	����E_FAIL��ʾд����ʧ��. */
extern HRESULT SaveFileStoreData(
	STORE_FILE_INDEX	FileNum,		// ��Ҫ���������ļ�������.
	const PBYTE8		pbSrc,			// ָ��Դ����.
	DWORD32				dwBytesToWrite,	// ����д������ݸ���.
	PDWORD32			pdwBytesWritten	// ΪNULLʱ��ʾ������ʵ��д������ݸ���.
);

#ifdef __cplusplus
	}
#endif /* #ifdef __cplusplus */

#endif
