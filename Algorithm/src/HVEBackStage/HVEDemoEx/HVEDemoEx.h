// HVEDemoEx.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error �ڰ������� PCH �Ĵ��ļ�֮ǰ������stdafx.h��
#endif

#include "resource.h"		// ������

// CHVEDemoExApp:
// �йش����ʵ�֣������ HVEDemoEx.cpp
//

class CHVEDemoExApp : public CWinApp
{
public:
	CHVEDemoExApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CHVEDemoExApp theApp;
