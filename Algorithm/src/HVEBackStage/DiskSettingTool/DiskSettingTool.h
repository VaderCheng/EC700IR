// DiskSettingTool.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error �ڰ������� PCH �Ĵ��ļ�֮ǰ������stdafx.h��
#endif

#include "resource.h"		// ������


// CDiskSettingToolApp:
// �йش����ʵ�֣������ DiskSettingTool.cpp
//

class CDiskSettingToolApp : public CWinApp
{
public:
	CDiskSettingToolApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CDiskSettingToolApp theApp;
