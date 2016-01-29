// DiskSettingToolDlg.h : ͷ�ļ�
//

#pragma once


// CDiskSettingToolDlg �Ի���
class CDiskSettingToolDlg : public CDialog
{
// ����
public:
	CDiskSettingToolDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_DISKSETTINGTOOL_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnOK();
	afx_msg void OnCancel();
	afx_msg void OnDClickList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRDClickList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBnClickedButton1(void);
	afx_msg void OnBnClickedButton2(void);
	DECLARE_MESSAGE_MAP()

private:
	afx_msg void StartChecking(void);
	void GetDiskListInfo(void);
	BOOL GetDiskInfo(CString strDiskChar, DWORD& dwDiskSize);
private:
	CString		m_strSystemDisk;
	CString		m_strAppPath;
	CButton		m_btOK;
	CButton		m_btCancel;
	CComboBox	m_cbCurrentDisk;
	CListCtrl	m_lcDiskInfo;
};
