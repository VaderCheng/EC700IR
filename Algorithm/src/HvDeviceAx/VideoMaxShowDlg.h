#pragma once


// CVideoMaxShowDlg �Ի���

class CVideoMaxShowDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CVideoMaxShowDlg)

public:
	CVideoMaxShowDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CVideoMaxShowDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_MAXSHOWVIDEO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
};
