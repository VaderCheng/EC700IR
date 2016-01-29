#ifndef _INIFILE_INCLUDED__
#define _INIFILE_INCLUDED__

#include "HvStream.h"

#if ( RUN_PLATFORM == PLATFORM_WINDOWS )
#include <windows.h>
#endif

#if RUN_PLATFORM == PLATFORM_DSP_BIOS
#include "AddtionParam.h"
#endif

const INT g_uMaxSectionLen    = 48;
const INT g_uMaxSectionNumber = 80;
const INT g_uMaxKeyLen        = 48;
const INT g_uMaxValueLen      = 80;
const INT g_uMaxKeyNumber     = 800;
const INT g_uMaxLine          = 1024;

///�ؼ���
class Keys
{
public:
	//������section���(��1��ʼ,С��0��Ϊ�Ѿ���ɾ���Ĺؼ���,0�������κ�С�ڵĹؼ���)
	INT uSectionNumber;
	char* szKeyName;   
	char* szKeyValue;
	char* szComment;
public:
	Keys()
		: uSectionNumber( -1 ),
		szKeyName( NULL ),
		szKeyValue( NULL ),
		szComment( NULL )
	{}
	
	~Keys()
	{
		Delete();
	}
	void Delete()
	{
		uSectionNumber = -1;
		if( szKeyName != NULL )
		{
			delete[] szKeyName;
			szKeyName = NULL;
		}
		
		if( szKeyValue != NULL )
		{
			delete[] szKeyValue;
			szKeyValue = NULL;
		}
		
		if( szComment != NULL )
		{
			delete[] szComment;
			szComment = NULL;
		}
	}
};

//С��section
class Sections{
public:
	char* szSectionName;
public:
	Sections()
		: szSectionName( NULL )
	{}
	~Sections()
	{
		Delete();
	}
	void Delete()
	{
		if( szSectionName != NULL )
		{
			delete[] szSectionName;
			szSectionName = NULL;
		}
	}
};

class CIniFile
{
public:
	CIniFile(void);
	~CIniFile(void);

public:
	//����INI��
	HVRESULT ReadStream( 
		HiVideo::ISequentialStream* pStream,
		BOOL fClear = FALSE,
		HiVideo::ISequentialStream* pRevert = NULL,
		HiVideo::ISequentialStream* pModification = NULL
		);
#if RUN_PLATFORM == PLATFORM_DSP_BIOS
	HVRESULT SetAddtionParam( CAddtionParam* pAddtionParam );
#endif
	//ini�ļ���ȡ
	HVRESULT ReadString( 
		LPCSTR lpszSection, 
		LPCSTR lpszKey, 
		LPCSTR lpszDefault, 
		LPSTR  lpszResult, 
		INT    dwSize, 
		PINT   pcbLen = NULL
		);
	HVRESULT ReadInteger( 
		LPCSTR lpszSection, 
		LPCSTR lpszKey, 
		INT uDefault, 
		PINT puResult
		);
	HVRESULT ReadBoolean( 
		LPCSTR lpszSection, 
		LPCSTR lpszKey, 
		BOOL fDefault, 
		PBOOL pfResult
		);
	HVRESULT ReadBinary(
		LPCSTR lpszSection,
		LPCSTR lpszKey,
		PBYTE8 lpbDefault,
		PBYTE8 lpbResult,
		INT    iSize,
		PINT   pcbRead = NULL
		);
	//д��INI
	HVRESULT WriteString(
		LPCSTR lpszSection,
		LPCSTR lpszKey,
		LPCSTR lpszValue,
		LPCSTR lpszComment = NULL
		);

#if ( RUN_PLATFORM == PLATFORM_WINDOWS )
	//��ע���׷��
	HVRESULT AppendFromRegistry(
		const HKEY& hKey, 
		HiVideo::ISequentialStream* pRevert = NULL,
		HiVideo::ISequentialStream* pModification = NULL
		);
#endif
	//���ڴ�����д����
	HVRESULT WriteToStream( HiVideo::ISequentialStream* pStream );
	//���С����
	HVRESULT GetSectionNumber( PINT pcbSection );
	//�����ӦС���µĹؼ�����
	HVRESULT GetKeyNumber( LPCTSTR lpszSection, PINT pcbKey );
	//ö��С��
	HVRESULT EnumSection( const INT& uSection, LPTSTR lpszSection, PINT pcbLen );
	//ö����ӦС���µĹؼ���
	HVRESULT EnumKey( 
		const INT& uKey, LPCTSTR lpszSection, 
		LPTSTR lpszKeyName, PINT pcbNameSize,
		LPTSTR lpszValue, PINT pcbValueSize
		);
	//�õ���ӦС���¹ؼ��ֵ�ע��
	HVRESULT ReadComment( 
		LPCTSTR lpszSection, LPCTSTR lpszKey,
		LPTSTR lpszResult, const INT& cbLen
		);

	//HVRESULT GetMaxInteger( LPCSTR lpszSection, LPCSTR lpszKey, PINT piResult );
	//HVRESULT GetMinInteger( LPCSTR lpszSection, LPCSTR lpszKey, PINT piResult );
	//HVRESULT GetDefaultInteger( LPCSTR lpszSection, LPCSTR lpszKey, PINT piResult );

private:
	//��������Ϣ
	INT Superaddition( LPCTSTR lpszLine, LPTSTR lpszValue = NULL, const INT& iLen = 0 );
	//�����ĵ�ǰλ�ö�ȡһ������(������ȳ���uLen,��ȥ��Ҫ)
	BOOL ReadOneLine( HiVideo::ISequentialStream* pStream, LPTSTR lpszLine, INT uLen );
	//ͨ��һ�ַ����������ڴ������
	BOOL SetDataFromLine( 
		LPTSTR lpszLine,
		HiVideo::ISequentialStream* pResert,
		HiVideo::ISequentialStream* pModification,
		const BOOL& fInitialize
		);
	//�ж��Ƿ��ǲ��ô����������
    BOOL IsComplete( const BYTE8 bMaker );
	//�õ�С�ڵı��
	INT GetSection( LPCSTR lpszSection );
	//�ж��Ƿ��д�KEY,����ȡ��ֵ
	BOOL GetKeyValue( const INT& uPos, LPCSTR lpszKey, LPSTR lpszValue, QWORD64 dwLen );
	//ˢ���ڴ��е�����,��Ҫ��ɾ��û�йؼ��ֵ�С��
	void RebirthData();

#if ( RUN_PLATFORM == PLATFORM_WINDOWS )
	//��ע���׷�ӵ��ڴ�������
	BOOL SetDataFromRegistry( 
		const HKEY& hKey,
		HiVideo::ISequentialStream* pResert,
		HiVideo::ISequentialStream* pModification
		);
#endif

private:
	//С������
	Sections m_rgSection[ g_uMaxSectionNumber ];
	INT m_uSectionNumber;
	//�ؼ�������
	Keys m_rgKey[ g_uMaxKeyNumber ];
	INT m_uKeyNumber;
	//��ǰС���±�
	INT m_uCurSection;
    //�Ƿ���ع�INI
	BOOL m_fInitialize;

#if RUN_PLATFORM == PLATFORM_DSP_BIOS
	//����������INI��ϵͳ����
	CAddtionParam* m_pAddtionParam;
#endif
};

#endif
