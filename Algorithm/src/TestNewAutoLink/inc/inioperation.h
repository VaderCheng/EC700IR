#ifndef _INI_OPERATION_H__
#define _INI_OPERATION_H__

#include "swBaseType.h"
#include "swWinError.h"

#ifdef __cplusplus
	extern "C" {
#endif	/* #ifdef __cplusplus */

typedef void * INIFILE_HANDLE;

/*
����˵��:�ú�������Ini�ļ�.
��������ֵ:
	����S_OK��ʾ��ʼ��Ini�ļ��ɹ�.
	����E_POINTER��ʾpFileHandleΪNULL.
	����E_NOTIMPL��ʾϵͳ��ǰ��֧��Ini�ļ�ϵͳ.
	����E_FAIL��ʾ��ʼ��Ini�ļ�ʧ��. */
extern HRESULT InitIniFile( 
	LPCSTR					pcszFileName,	//ָ���ļ����ַ���.
	INIFILE_HANDLE			*pFileHandle	//ָ��Ini�ļ�����.
);

/* 
����˵��:�ú���������Ini�ļ������Ը��ļ��������Ĵ洢��д��.
��������ֵ:
	����S_OK��ʾ�����ɹ�.
	����E_HANDLE��ʾhFileΪNULL.
	����E_FAIL��ʾ����ʧ��. */
extern HRESULT FlushIniFile(
	INIFILE_HANDLE			hFile
);

/*
����˵��:�ú���������Ini�ļ��ж�ȡĳ������ĳ�����ַ���.
��������ֵ:
	����S_OK��ʾ��ȡ���ݳɹ�.
	����E_HANDLE��ʾhFileΪNULL.
	����E_POINTER��ʾpcszSubSet����pcszItem,����pszStrΪNULL.
	����E_FAIL��ʾ��ǰ��Ҫ��ȡ�������.
	����S_FALSE��ʾ��ȡ�õ��ַ������ȴ�������������󳤶�. */
extern HRESULT GetIniFileStr( 
	INIFILE_HANDLE			hFile,			//ָ��Ini�ļ����.
	LPCSTR					pcszSubSet,		//ָ�������ַ���.
	LPCSTR					pcszItem,		//ָ�������ַ���.
	LPSTR					pszRetStr,		//ָ���ȡ���ַ�������.
	DWORD32					dwSize			//Ŀ�껺�����Ĵ�С
);

/*
����˵��:�ú���������Ini�ļ���ĳ��ĳ��д��һ�ַ���.
��������ֵ:
	����S_OK��ʾд�����ݳɹ�.
	����E_HANDLE��ʾhFileΪNULL.
	����E_POINTER��ʾpcszSubSet����pcszItem,����pcszStrΪNULL.
	����E_FAIL��ʾ��ǰ��Ҫ��ȡ�������. */
extern HRESULT WriteIniFileStr(
	INIFILE_HANDLE			hFile,			//ָ��Ini�ļ����.
	LPCSTR					pcszSubSet,		//ָ�������ַ���,����������򴴽�.
	LPCSTR					pcszItem,		//ָ�������ַ���,����������򴴽�.
	LPCSTR					pcszStr			//ָ����Ҫд����ַ���.
);

/*
����˵��:�ú���������Ini�ļ���ɾ��ĳ���е�ĳ��.
��������ֵ:
	����S_OK��ʾд�����ݳɹ�.
	����E_HANDLE��ʾhFileΪNULL.
	����E_POINTER��ʾpcszSubSetΪNULL.
	����E_FAIL��ʾ��ǰ��Ҫɾ���������. */
extern HRESULT DeleteIniFileItem( 
	INIFILE_HANDLE			hFile,			//ָ��Ini�ļ����.
	LPCSTR					pcszSubSet,		//ָ�������ַ���.
	LPCSTR					pcszItem		//ָ�������ַ���,���Ϊ����ɾ��������.
);

/*
����˵��:�ú���������Ini�ļ�ȫ�����.
��������ֵ:
	����S_OK��ʾ��ճɹ�.
	����E_HANDLE��ʾhFileΪNULL.
	����E_FAIL��ʾ����ļ�ʧ��. */
extern HRESULT ClearIniFile(
	INIFILE_HANDLE			hFile			//ָ��Ini�ļ����.
);

//��Ini�ļ��ж�ȡ���Ͳ���
extern HRESULT GetIniFileInteger(
	INIFILE_HANDLE			hFile,			//ָ��Ini�ļ����.
	LPCSTR					pcszSubSet,		//ָ�������ַ���.
	LPCSTR					pcszItem,		//ָ�������ַ���.
	int						*piValue,		//ָ���ȡ����������
	int						iDefault		//����������ʱ���ص�ȱʡֵ
);

//��Ini�ļ��ж�ȡ�����Ͳ���
extern HRESULT GetIniFileFloat(
	INIFILE_HANDLE			hFile,			//ָ��Ini�ļ����.
	LPCSTR					pcszSubSet,		//ָ�������ַ���.
	LPCSTR					pcszItem,		//ָ�������ַ���.
	float					*pfltValue,		//ָ���ȡ�ĸ���������
	float					fltDefault		//����������ʱ���ص�ȱʡֵ
);

//��Ini�ļ���д��һ���Ͳ���.
extern HRESULT SetIniFileInteger(
	INIFILE_HANDLE			hFile,			//ָ��Ini�ļ����.
	LPCSTR					pcszSubSet,		//ָ�������ַ���.
	LPCSTR					pcszItem,		//ָ�������ַ���.
	int						iValue			//д�����ֵ.
);

//��Ini�ļ���д��һ������.
extern HRESULT SetIniFileFloat(
	INIFILE_HANDLE			hFile,			//ָ��Ini�ļ����.
	LPCSTR					pcszSubSet,		//ָ�������ַ���.
	LPCSTR					pcszItem,		//ָ�������ַ���.
	float					fValue			//д�����ֵ.
);

/*
����˵��:�ú���������Ini�ļ��ļ��ر�.
��������ֵ:
	����S_OK��ʾ�رճɹ�.
	����E_HANDLE��ʾhFileΪNULL.
	����E_FAIL��ʾ�ر��ļ�ʧ��. */
extern HRESULT CloseIniFile(
	INIFILE_HANDLE			hFile			//ָ��Ini�ļ����.
);

#ifdef __cplusplus
	}
#endif /* #ifdef __cplusplus */

#endif //_INI_OPERATION_H__
