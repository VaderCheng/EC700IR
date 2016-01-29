#ifndef _EEPROM_DATA_STORE_H_
#define _EEPROM_DATA_STORE_H_

#include "Basecommon.h"

#define EEPROM_BLOCK_MAX_LEN	256

class CEepromDataStore {
	private:
		DWORD32			m_dwBlockStartPos;
		DWORD32			m_dwBakBlockStartPos;
		DWORD32			m_dwBlockUsedLen;
		DWORD32			m_dwBlockMaxLen;		
		BOOL			m_fInitialized;
		BOOL 			m_fCrcCheck;
		int				m_iStoreTableFlag;
		
		BYTE8			m_rgbBuff[EEPROM_BLOCK_MAX_LEN];
		HRESULT SyncDataStore( int iSrcTable, int iDstTable );
		HRESULT IsOneDataStoreActive( int iTableType );
		HRESULT SetDataOne( 
			int			iTableType,		//������
			PBYTE8		pData,			//����.
			DWORD32		dwLen,			//����.
			DWORD32		dwOffset		//��ʼ��Ҫ������ַ.
		);
	public:
	
		virtual ~CEepromDataStore();
		CEepromDataStore();
		
		//������ʼ������У���й���Ϣ,У����Զֻ�����UsedLen֮��,���Խ�����ʼ�����ɹ�.
		HRESULT InitDataStore
		(			
			BOOL		fUsedCRCCheck,	//�Ƿ�ʹ��CRCУ��.
			DWORD32 	dwStartPos,		//�洢��ʼ��ַ.
			DWORD32		dwBakStartPos,	//���洢��ʼ��ַ.
			DWORD32		dwUsedLen,		//ʵ�����ݳ���.
			DWORD32		dwMaxLen		//�������ݴ洢����󳤶�.
		);
		HRESULT IsDataStoreActive( void );		//�ж���ǰ������Ч��.
		//��������,��ʱ�Զ��������У�������Լ�����˫����.
		HRESULT SetData
		(
			PBYTE8		pData,			//����.
			DWORD32		dwLen,			//����.
			DWORD32		dwOffset		//��ʼ��Ҫ������ַ.
		);
		HRESULT GetData
		(
			PBYTE8		pData,			//����.
			DWORD32		dwLen,			//����.
			DWORD32		dwOffset		//��ʼ������ַ.
		);
};

#endif
