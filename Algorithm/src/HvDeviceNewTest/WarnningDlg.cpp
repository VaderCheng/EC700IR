#include "stdafx.h"
#include "WarnningDlg.h"
#include ".\warnningdlg.h"

IMPLEMENT_DYNAMIC(CWarnningDlg, CDialog)

CWarnningDlg::CWarnningDlg(DWORD* pdwProceMode, int iCRCValue, CWnd* pParnt /* = NULL */)
: CDialog(CWarnningDlg::IDD, pParnt)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pdwProcMode = pdwProceMode;
	m_iCRCValue = iCRCValue;
}

CWarnningDlg::~CWarnningDlg()
{

}

void CWarnningDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CWarnningDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON8, OnBnClickedButton8)
END_MESSAGE_MAP()

BOOL CWarnningDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);

	char szInfo[512] = {0};
	if(m_iCRCValue != -1)
	sprintf(szInfo, "ԭʼ�����ļ�CRCУ��ֵ�Ѷ�ʧ���޷�����������Ч����֤�����ư���У��ֵΪ��%d,��ȷ���л�����������ģʽ�����½�������",
		m_iCRCValue);
	else
	sprintf(szInfo, "ԭʼ�����ļ�CRCУ��ֵ�Ѷ�ʧ���һ�ȡ���ư���У��ֵҲʧ�ܣ��޷�ȷ�������ɹ����������½�������");
	SetDlgItemText(IDC_STATIC, szInfo);

	return TRUE;
}

void CWarnningDlg::OnCancel()
{
	
}



void CWarnningDlg::OnBnClickedButton1()
{
	*m_pdwProcMode = 1;
	CDialog::OnCancel();
}

void CWarnningDlg::OnBnClickedButton8()
{
	*m_pdwProcMode = 2;
	CDialog::OnCancel();
}
