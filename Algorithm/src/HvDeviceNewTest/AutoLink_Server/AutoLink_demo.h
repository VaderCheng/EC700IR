// AutoLink_demo.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error �ڰ������� PCH �Ĵ��ļ�֮ǰ������stdafx.h��
#endif

#include "resource.h"		// ������


// CAutoLink_demoApp:
// �йش����ʵ�֣������ AutoLink_demo.cpp
//

class CAutoLink_demoApp : public CWinApp
{
public:
	CAutoLink_demoApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();
};

extern CAutoLink_demoApp theApp;
