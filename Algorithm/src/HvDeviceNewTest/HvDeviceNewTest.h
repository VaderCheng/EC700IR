// HvDeviceNewTest.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error �ڰ������� PCH �Ĵ��ļ�֮ǰ������stdafx.h��
#endif

#include "resource.h"		// ������


// CHvDeviceNewTestApp:
// �йش����ʵ�֣������ HvDeviceNewTest.cpp
//

class CHvDeviceNewTestApp : public CWinApp
{
public:
	CHvDeviceNewTestApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CHvDeviceNewTestApp theApp;
