// HVE_SEARCH.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error �ڰ������� PCH �Ĵ��ļ�֮ǰ������stdafx.h��
#endif

#include "resource.h"		// ������


// CHVE_SEARCHApp:
// �йش����ʵ�֣������ HVE_SEARCH.cpp
//

class CHVE_SEARCHApp : public CWinApp
{
public:
	CHVE_SEARCHApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CHVE_SEARCHApp theApp;
