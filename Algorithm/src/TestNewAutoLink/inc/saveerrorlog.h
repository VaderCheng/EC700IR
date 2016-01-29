#ifndef _SAVE_ERROR_LOG_
#define _SAVE_ERROR_LOG_

#include <windows.h>
#include <tchar.h>
#include <shlwapi.h>
#include <time.h>
#include <stdio.h>

extern int g_iSaveErrorLog;
extern CRITICAL_SECTION g_csSaveErrorLog;

extern int g_iResetCount;

inline void SaveErrorLog(LPCTSTR lpszErrStr)
{
	OutputDebugString(lpszErrStr);
	OutputDebugString(_T("\n"));
	if (g_iSaveErrorLog != 1) return;
	//����LOG�ļ���
	const int MAXPATH = 260;
	TCHAR szFileName[ MAXPATH ] = {0};
	GetModuleFileName( NULL, szFileName, MAXPATH );	//ȡ�ð�����������ȫ·��
	PathRemoveFileSpec( szFileName );				//ȥ��������
	_tcsncat(szFileName, _T("\\Event.log"), MAXPATH - 1);

	//ȡ�õ�ǰ�ľ�ȷ�����ʱ��
	static time_t starttime = time(NULL);
	static DWORD starttick = GetTickCount(); 
	DWORD dwNowTick = GetTickCount() - starttick;
	time_t nowtime = starttime + (time_t)(dwNowTick / 1000);
	struct tm *pTM = localtime(&nowtime);
	DWORD dwMS = dwNowTick % 1000;

	EnterCriticalSection(&g_csSaveErrorLog);
	FILE* pfile = _tfopen(szFileName, _T("at"));
	//д��LOG��Ϣ
	if (pfile != NULL)
	{
		_ftprintf(pfile, _T("%04d-%02d-%02d %02d:%02d:%02d:%03d %s\n"),
			pTM->tm_year + 1900, pTM->tm_mon + 1, pTM->tm_mday,
			pTM->tm_hour, pTM->tm_min, pTM->tm_sec, dwMS,
			lpszErrStr);
		fclose(pfile);
	}
	LeaveCriticalSection(&g_csSaveErrorLog);
}

void SaveResetLog();
HRESULT GetResetLog(unsigned char* pbData, int* piLen);
void ClearResetLog();

#endif