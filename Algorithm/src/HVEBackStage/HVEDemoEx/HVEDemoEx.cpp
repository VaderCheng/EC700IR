// HVEDemoEx.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "HVEDemoEx.h"
#include "HVEDemoExDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CHVEDemoExApp

BEGIN_MESSAGE_MAP(CHVEDemoExApp, CWinApp)
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()


// CHVEDemoExApp ����

CHVEDemoExApp::CHVEDemoExApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CHVEDemoExApp ����

CHVEDemoExApp theApp;


// CHVEDemoExApp ��ʼ��

BOOL CHVEDemoExApp::InitInstance()
{
	InitCommonControls();

	CWinApp::InitInstance();

	AfxEnableControlContainer();

	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));


	CHVEDemoExDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	//if (nResponse == IDOK)
	//{
		// TODO: �ڴ˷��ô����ʱ�á�ȷ�������ر�
		//�Ի���Ĵ���
	//}
	//else if (nResponse == IDCANCEL)
	//{
		// TODO: �ڴ˷��ô����ʱ�á�ȡ�������ر�
		//�Ի���Ĵ���
	//}

	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	// ����������Ӧ�ó������Ϣ�á�
	return FALSE;
}
