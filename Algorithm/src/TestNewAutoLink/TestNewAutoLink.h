// TestNewAutoLink.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error �ڰ������� PCH �Ĵ��ļ�֮ǰ������stdafx.h��
#endif

#include "resource.h"		// ������


// CTestNewAutoLinkApp:
// �йش����ʵ�֣������ TestNewAutoLink.cpp
//

class CTestNewAutoLinkApp : public CWinApp
{
public:
	CTestNewAutoLinkApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CTestNewAutoLinkApp theApp;
