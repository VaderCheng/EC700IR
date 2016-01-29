#include "stdafx.h"
#include "SetEnhanceFlagDlg.h"

IMPLEMENT_DYNAMIC(CSetEnahnceFlagDlg, CDialog)

CSetEnahnceFlagDlg::CSetEnahnceFlagDlg(HVAPI_HANDLE_EX* phHandle, CWnd* pParent /* = NULL */)
: CDialog(CSetEnahnceFlagDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_hDevice = phHandle;
}

CSetEnahnceFlagDlg::~CSetEnahnceFlagDlg()
{

}

void CSetEnahnceFlagDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_ComBoxStyle);
}

BEGIN_MESSAGE_MAP(CSetEnahnceFlagDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON8, OnBnClickedButton8)
END_MESSAGE_MAP()

BOOL CSetEnahnceFlagDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);

	m_ComBoxStyle.AddString("���Ӻ�");
	m_ComBoxStyle.AddString("ֻ�Ӻ�Υ�½��");
	m_ComBoxStyle.AddString("ȫ�Ӻ�");
	m_ComBoxStyle.AddString("ֻ�Ӻ�ץ��ͼ");
	m_ComBoxStyle.AddString("ֻ�Ӻ�Υ��ץ��ͼ");

	m_ComBoxStyle.SetCurSel(0);

	SetDlgItemText(IDC_EDIT1, "0");
	SetDlgItemText(IDC_EDIT9, "128");
	SetDlgItemText(IDC_EDIT10, "80");

	return TRUE;
}

void CSetEnahnceFlagDlg::OnBnClickedButton1()
{
	if(m_hDevice == NULL)
	{
		MessageBox("δ���ӵ�ʶ����������ʧ��", "ERROR", MB_OK|MB_ICONERROR);
		OnCancel();
	}
	
	char szTemp[20] = {0};
	int iBrightness = 0;
	GetDlgItemText(IDC_EDIT1, szTemp, 20);
	iBrightness = atoi(szTemp);
	int iHueThreshold = 0;
	GetDlgItemText(IDC_EDIT9, szTemp, 20);
	iHueThreshold = atoi(szTemp);
	int iCompressRate = 0;
	GetDlgItemText(IDC_EDIT10, szTemp, 20);
	iCompressRate = atoi(szTemp);
	if(HVAPI_SetEnhanceRedLightFlagEx(*m_hDevice, m_ComBoxStyle.GetCurSel(), 
		iBrightness, iHueThreshold, iCompressRate) != S_OK)
	{
		MessageBox("����ʧ��", "ERROR", MB_OK|MB_ICONERROR);
		return;
	}
	MessageBox("���óɹ�");
	OnCancel();
}

void CSetEnahnceFlagDlg::OnBnClickedButton8()
{
	OnCancel();
}