// AutoLinkChangeString.cpp : implementation file
//

#include "stdafx.h"
#include "AutoLink_demo.h"
#include "AutoLinkChangeString.h"
#include ".\autolinkchangestring.h"


// AutoLinkChangeString dialog

IMPLEMENT_DYNAMIC(AutoLinkChangeString, CDialog)
AutoLinkChangeString::AutoLinkChangeString(CWnd* pParent /*=NULL*/)
	: CDialog(AutoLinkChangeString::IDD, pParent)
{
}

AutoLinkChangeString::~AutoLinkChangeString()
{
}

void AutoLinkChangeString::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_MATCH1, m_ExampleEdit);
	DDX_Control(pDX, IDC_EDIT_MATCH2, m_InfoEdit);
	DDX_Control(pDX, IDC_EDIT_MATCH3, m_ChangeEdit);
	DDX_Control(pDX, IDC_COMBO_MATCHTIME, m_TimeDropList);
	DDX_Control(pDX, IDC_COMBO_MATCHPLATE, m_PlateDropList);
	DDX_Control(pDX, IDC_COMBO_MATCHDEV, m_DevInfoDropList);
	DDX_Control(pDX, IDC_COMBO_MATCHOTHER, m_OtherDropList);
	DDX_Control(pDX, IDC_COMBO_TYPE, m_TypeDropList);
}

BOOL AutoLinkChangeString::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_bFirstUseDropLise = true; // �Ƿ����ʹ�������б��
	
	InitChangeDlg();

	return TRUE;
}

BEGIN_MESSAGE_MAP(AutoLinkChangeString, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_MATCHOK, OnBnClickedButtonMatchok)
	ON_BN_CLICKED(IDC_BUTTON_MATCHCANCEL, OnBnClickedButtonMatchcancel)
	ON_WM_CLOSE()
	ON_CBN_SELCHANGE(IDC_COMBO_MATCHTIME, OnCbnSelchangeComboMatchtime)
	ON_CBN_SELCHANGE(IDC_COMBO_MATCHPLATE, OnCbnSelchangeComboMatchplate)
	ON_CBN_SELCHANGE(IDC_COMBO_MATCHDEV, OnCbnSelchangeComboMatchdev)
	ON_CBN_SELCHANGE(IDC_COMBO_MATCHOTHER, OnCbnSelchangeComboMatchother)
	ON_CBN_SELCHANGE(IDC_COMBO_TYPE, OnCbnSelchangeComboType)
END_MESSAGE_MAP()

// AutoLinkChangeString message handlers

void AutoLinkChangeString::InitChangeDlg()
{
	ShowText(m_chInitText);

	m_TimeDropList.ResetContent();  // �����������
	m_TimeDropList.SetCurSel(1);  // ���õ�ǰѡ��
	m_TimeDropList.AddString("");
	m_TimeDropList.AddString("��");
	m_TimeDropList.AddString("��");
	m_TimeDropList.AddString("��");
	m_TimeDropList.AddString("ʱ");
	m_TimeDropList.AddString("��");
	m_TimeDropList.AddString("��");
	m_TimeDropList.AddString("����");

	m_PlateDropList.ResetContent();  // �����������
	m_PlateDropList.SetCurSel(1);  // ���õ�ǰѡ��
	m_PlateDropList.AddString("");
	m_PlateDropList.AddString("���ƺ�");
	m_PlateDropList.AddString("������ɫ");
	m_PlateDropList.AddString("Υ����Ϣ");
	m_PlateDropList.AddString("��������");
	m_PlateDropList.AddString("����");
	m_PlateDropList.AddString("����");
	m_PlateDropList.AddString("ͼƬ��ĸID");
	m_PlateDropList.AddString("ͼƬID");
	m_PlateDropList.AddString("ͼƬ����");
	m_PlateDropList.AddString("������");

	m_DevInfoDropList.ResetContent();  // �����������
	m_DevInfoDropList.SetCurSel(1);  // ���õ�ǰѡ��
	m_DevInfoDropList.AddString("");
	m_DevInfoDropList.AddString("�豸IP");
	m_DevInfoDropList.AddString("�豸���");

	m_OtherDropList.ResetContent();  // �����������
	m_OtherDropList.SetCurSel(1);  // ���õ�ǰѡ��
	m_OtherDropList.AddString("");
	m_OtherDropList.AddString("·������");
	m_OtherDropList.AddString("·�ڷ���");

	m_TypeDropList.ResetContent();  // �����������
	m_TypeDropList.SetCurSel(1);  // ���õ�ǰѡ��
	m_TypeDropList.AddString("");
	m_TypeDropList.AddString("jpg");
	m_TypeDropList.AddString("bmp");
	m_TypeDropList.AddString("txt");
	m_TypeDropList.AddString("inf");
	m_TypeDropList.AddString("bin");
}

// ����ӿ�::ʹ��ǰ���û�������
void AutoLinkChangeString::SetInitText(char* chText)
{
	sprintf(m_chInitText, "%s", chText);
}

// ����ӿ�::ʹ�ú󷵻ؽ��
void AutoLinkChangeString::GetReturnText(char* chText)
{
	sprintf(chText, "%s", m_strReturnText);
}


void AutoLinkChangeString::OnBnClickedButtonMatchok()
{
	m_ChangeEdit.GetWindowText(m_strReturnText);
	OnOK();
}

void AutoLinkChangeString::OnBnClickedButtonMatchcancel()
{
	sprintf(m_strReturnText.GetBuffer(1024), "%s", m_chInitText);
	m_strReturnText.ReleaseBuffer();
	OnCancel();
}

void AutoLinkChangeString::OnClose()
{
	sprintf(m_strReturnText.GetBuffer(1024), "%s", m_chInitText);
	m_strReturnText.ReleaseBuffer();

	CDialog::OnClose();
}

// ���λس���ESC
BOOL AutoLinkChangeString::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN
			|| pMsg->wParam == VK_ESCAPE)
		{
			return TRUE;
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}

void AutoLinkChangeString::ShowText(CString strText)
{
	CString strTemp;
	m_ChangeEdit.GetWindowText(strTemp);

	strTemp = strTemp + strText;
	m_ChangeEdit.SetWindowText(strTemp);

	CString strExampleText = m_Common.GetStringExample(strTemp); // ʾ��
	CString strInfoText = m_Common.GetStringInfo(strTemp); // ˵��

	m_ExampleEdit.SetWindowText(strExampleText);
	m_InfoEdit.SetWindowText(strInfoText);
}

void AutoLinkChangeString::OnCbnSelchangeComboMatchtime()
{
	CString strListTemp;
	int iIndex = m_TimeDropList.GetCurSel();
	m_TimeDropList.GetLBText(iIndex, strListTemp);

	// ��һ�δ�
	if (m_bFirstUseDropLise && !strListTemp.IsEmpty())
	{
		m_ChangeEdit.SetWindowText("");
		m_bFirstUseDropLise = false;
	}

	if ("��" == strListTemp)
	{
		ShowText("$(Year)");
	}
	else if ("��" == strListTemp)
	{
		ShowText("$(Month)");
	}
	else if ("��" == strListTemp)
	{
		ShowText("$(Day)");
	}
	else if ("ʱ" == strListTemp)
	{
		ShowText("$(Hour)");
	}
	else if ("��" == strListTemp)
	{
		ShowText("$(Minute)");
	}
	else if ("��" == strListTemp)
	{
		ShowText("$(Second)");
	}
	else if ("����" == strListTemp)
	{
		ShowText("$(MilliSecond)");
	}
	else
	{
		ShowText("");
	}
}

void AutoLinkChangeString::OnCbnSelchangeComboMatchplate()
{
	CString strListTemp;
	int iIndex = m_PlateDropList.GetCurSel();
	m_PlateDropList.GetLBText(iIndex, strListTemp);
	
	// ��һ�δ�
	if (m_bFirstUseDropLise && !strListTemp.IsEmpty())
	{
		m_ChangeEdit.SetWindowText("");
		m_bFirstUseDropLise = false;
	}

	if ("���ƺ�" == strListTemp)
	{
		ShowText("$(PlateNo)");
	}
	else if ("������ɫ" == strListTemp)
	{
		ShowText("$(PlateColor)");
	}
	else if ("Υ����Ϣ" == strListTemp)
	{
		ShowText("$(IllegalInfo)");
	}
	else if ("��������" == strListTemp)
	{
		ShowText("$(CarType)");
	}
	else if ("����" == strListTemp)
	{
		ShowText("$(Speed)");
	}
	else if ("����" == strListTemp)
	{
		ShowText("$(LimitSpeed)");
	}
	else if ("ͼƬ��ĸID" == strListTemp)
	{
		ShowText("$(BigImageID_Letter)");
	}
	else if ("ͼƬID" == strListTemp)
	{
		ShowText("$(BigImageID)");
	}
	else if ("ͼƬ����" == strListTemp)
	{
		ShowText("$(BigImageCount)");
	}
	else if ("������" == strListTemp)
	{
		ShowText("$(RoadNum)");
	}
	else
	{
		ShowText("");
	}
}

void AutoLinkChangeString::OnCbnSelchangeComboMatchdev()
{
	CString strListTemp;
	int iIndex = m_DevInfoDropList.GetCurSel();
	m_DevInfoDropList.GetLBText(iIndex, strListTemp);
	
	// ��һ�δ�
	if (m_bFirstUseDropLise && !strListTemp.IsEmpty())
	{
		m_ChangeEdit.SetWindowText("");
		m_bFirstUseDropLise = false;
	}

	if ("�豸IP" == strListTemp)
	{
		ShowText("$(DeviceIP)");
	}
	else if ("�豸���" == strListTemp)
	{
		ShowText("$(DeviceNo)");
	}
	else
	{
		ShowText("");
	}
}

void AutoLinkChangeString::OnCbnSelchangeComboMatchother()
{
	CString strListTemp;
	int iIndex = m_OtherDropList.GetCurSel();
	m_OtherDropList.GetLBText(iIndex, strListTemp);

	// ��һ�δ�
	if (m_bFirstUseDropLise && !strListTemp.IsEmpty())
	{
		m_ChangeEdit.SetWindowText("");
		m_bFirstUseDropLise = false;
	}

	if ("·������" == strListTemp)
	{
		ShowText("$(StreetName)");
	}
	else if ("·�ڷ���" == strListTemp)
	{
		ShowText("$(StreetDirection)");
	}
	else
	{
		ShowText("");
	}
}

void AutoLinkChangeString::OnCbnSelchangeComboType()
{
	// TODO: Add your control notification handler code here
	CString strListTemp;
	int iIndex = m_TypeDropList.GetCurSel();
	m_TypeDropList.GetLBText(iIndex, strListTemp);

	// ��һ�δ�
	if (m_bFirstUseDropLise && !strListTemp.IsEmpty())
	{
		m_ChangeEdit.SetWindowText("");
		m_bFirstUseDropLise = false;
	}

	if ("jpg" == strListTemp)
	{
		ShowText(".jpg");
	}
	else if ("bmp" == strListTemp)
	{
		ShowText(".bmp");
	}
	else if ("bin" == strListTemp)
	{
		ShowText(".bin");
	}
	else if ("inf" == strListTemp)
	{
		ShowText(".inf");
	}
	else if ("txt" == strListTemp)
	{
		ShowText(".txt");
	}
	else
	{
		ShowText("");
	}
}
