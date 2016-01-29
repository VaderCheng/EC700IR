// AutoLink_demo.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "AutoLink_demo.h"
#include "AutoLink_demoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAutoLink_demoApp

BEGIN_MESSAGE_MAP(CAutoLink_demoApp, CWinApp)
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()


// CAutoLink_demoApp ����

CAutoLink_demoApp::CAutoLink_demoApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CAutoLink_demoApp ����

CAutoLink_demoApp theApp;


HANDLE  g_hMutex = NULL;
// CAutoLink_demoApp ��ʼ��

BOOL CAutoLink_demoApp::InitInstance()
{
	CWinApp::InitInstance();

	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}
	// DEBUG�²������ػ�����
#ifndef _DEBUG
	g_hMutex = CreateMutex(NULL,TRUE, _T("AutoLink_Server.exe"));
	if (g_hMutex == NULL)
	{
		return FALSE;
	}
	if (GetLastError() == ERROR_SUCCESS)
	{
		//��ǰ���ػ�����
		
		BOOL fExit = FALSE;
		while (!fExit)
		{
			ReleaseMutex(g_hMutex);

			STARTUPINFO si;
			PROCESS_INFORMATION pi;

			ZeroMemory( &si, sizeof(si) );
			si.cb = sizeof(si);
			ZeroMemory( &pi, sizeof(pi) );

			// Start the child process. 
			TCHAR szFileName[MAX_PATH] = {0};
			GetModuleFileName(NULL, szFileName, MAX_PATH - 1);	//ȡ�ð�����������ȫ·��
			if( !CreateProcess( NULL,	// No module name (use command line). 
				szFileName,					// Command line. 
				NULL,							// Process handle not inheritable. 
				NULL,							// Thread handle not inheritable. 
				FALSE,							// Set handle inheritance to FALSE. 
				0,									// No creation flags. 
				NULL,							// Use parent's environment block. 
				NULL,							// Use parent's starting directory. 
				&si,								// Pointer to STARTUPINFO structure.
				&pi )								// Pointer to PROCESS_INFORMATION structure.
				) 
			{
				AfxMessageBox(_T("�����ӽ���ʧ�ܣ�"));
				CloseHandle(g_hMutex);
				return FALSE;
			}

			// Wait until child process exits.
			WaitForSingleObject( pi.hProcess, INFINITE );

			// Close process and thread handles. 
			CloseHandle( pi.hProcess );
			CloseHandle( pi.hThread );

			//�ж��ӽ����Ƿ������˳�
			if (WaitForSingleObject(g_hMutex, INFINITE) == WAIT_OBJECT_0)
			{
				fExit = TRUE;
			}
		}
		return TRUE;
	}
	if(GetLastError() != ERROR_ALREADY_EXISTS)
	{
		return FALSE;
	}
	//��ǰ���ӽ���
	if (WaitForSingleObject(g_hMutex, 0) != WAIT_OBJECT_0)
	{
		//��ֹ�ظ�����
		CloseHandle(g_hMutex);
		return FALSE;
	}
#endif

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));

	CAutoLink_demoDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();

	if (nResponse == IDOK)
	{
		// TODO: �ڴ˷��ô����ʱ�á�ȷ�������ر�
		//�Ի���Ĵ���
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: �ڴ˷��ô����ʱ�á�ȡ�������ر�
		//�Ի���Ĵ���
	}

	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	// ����������Ӧ�ó������Ϣ�á�
	return FALSE;
}

int CAutoLink_demoApp::ExitInstance()
{
	// TODO: Add your specialized code here and/or call the base class
	if (g_hMutex)
	{
		ReleaseMutex(g_hMutex);
		CloseHandle(g_hMutex);
	}
	return CWinApp::ExitInstance();
}
