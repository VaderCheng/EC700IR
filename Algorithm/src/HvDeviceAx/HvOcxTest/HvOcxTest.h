// HvOcxTest.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error �ڰ������� PCH �Ĵ��ļ�֮ǰ������stdafx.h��
#endif

#include "resource.h"		// ������


// CHvOcxTestApp:
// �йش����ʵ�֣������ HvOcxTest.cpp
//

class CHvOcxTestApp : public CWinApp
{
public:
	CHvOcxTestApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CHvOcxTestApp theApp;
