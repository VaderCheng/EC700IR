// VideoMaxShowDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "HvDeviceAx.h"
#include "VideoMaxShowDlg.h"
#include "afxdialogex.h"
#include "HvDeviceAxCtrl.h"


// CVideoMaxShowDlg �Ի���

IMPLEMENT_DYNAMIC(CVideoMaxShowDlg, CDialogEx)

CVideoMaxShowDlg::CVideoMaxShowDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CVideoMaxShowDlg::IDD, pParent)
{

}

CVideoMaxShowDlg::~CVideoMaxShowDlg()
{
}

void CVideoMaxShowDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CVideoMaxShowDlg, CDialogEx)
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()


// CVideoMaxShowDlg ��Ϣ�������


void CVideoMaxShowDlg::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	//�򸸴��ڷ���˫������
	//::PostMessage(::GetParent(m_hWnd),WM_LBUTTONDBLCLK, 0, 0);
	//::SetForegroundWindow(::GetParent(m_hWnd));
	((CHvDeviceAxCtrl*)GetParent())->ShowVideoNomal(); //OnLButtonDblClk(nFlags, point);
	CDialogEx::OnLButtonDblClk(nFlags, point);
	this->OnOK();
	::PostMessage(::GetParent(m_hWnd),WM_LBUTTONDBLCLK, 0, 0);
}
