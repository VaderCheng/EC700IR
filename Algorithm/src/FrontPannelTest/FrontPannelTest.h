// FrontPannelTest.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error �ڰ������� PCH �Ĵ��ļ�֮ǰ������stdafx.h��
#endif

#include "resource.h"		// ������


// CFrontPannelTestApp:
// �йش����ʵ�֣������ FrontPannelTest.cpp
//

class CFrontPannelTestApp : public CWinApp
{
public:
	CFrontPannelTestApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CFrontPannelTestApp theApp;
