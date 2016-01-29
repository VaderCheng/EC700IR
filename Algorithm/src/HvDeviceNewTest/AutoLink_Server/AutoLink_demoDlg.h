// AutoLink_demoDlg.h : ͷ�ļ�

#pragma once

#include "Camera.h"

typedef struct _DEV_HANDLE
{
	HVAPI_HANDLE_EX handle;
	char szDevSN[128];

	_DEV_HANDLE()
		: handle(NULL)
	{
		ZeroMemory(szDevSN , sizeof(szDevSN));
	}
}DEV_HANDLE;


// CAutoLink_demoDlg �Ի���
class CAutoLink_demoDlg : public CDialog
{
// ����
public:
	CAutoLink_demoDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_AUTOLINK_DEMO_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��
	HICON m_hIcon;
	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	int* m_pIsUsrDiscnn;
	int m_iLogEnable; // ��־����
	
	bool m_fExit;
	CString m_strDir;  // �����̷�Ŀ¼
	int m_iPort;  // �����˿�
	int m_iDevMaxNum; // �����豸����

	HANDLE m_hShowThread;
	HANDLE m_hDeleteThread;
	CList<CCamera*, CCamera*> m_connDevList;

	CCommon m_Common; // ����������
	
	HRESULT InitDevListCtrl();
	HRESULT CreatConnect(char* pSN);
	HRESULT DisConnectDev(char* pSN);
	int UpdateClient();
	HRESULT ShowClientInfor(char* pSN);

	void WriteFileIni();
	void ReadFileIni();

	void WriteDlgIni();
	void ReadDlgIni();
	void WriteLog(char* chText);

	// ѭ������
	int DeleteFileRun();
	int CheckDriveSpaceSize(char *szDriveName);
	int DeleteFileByName(const char* chFileName);
	int DeleteDirectory(const char* chDir);

public:
	static DWORD WINAPI UpdateClientThread(LPVOID lpParameter)
	{
		if (NULL == lpParameter)
			return 0;

		CAutoLink_demoDlg* pThis = (CAutoLink_demoDlg*)lpParameter;
		return pThis->UpdateClient();
	}

	static DWORD WINAPI DeleteThread(LPVOID lpParameter)
	{
		if (NULL == lpParameter)
			return 0;

		CAutoLink_demoDlg* pThis = (CAutoLink_demoDlg*)lpParameter;
		return pThis->DeleteFileRun();
	}

	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	CListCtrl m_devListCtrl;
	CEdit m_autoLinkPortEdit;
	CEdit m_maxListenNumEdit;
	CButton m_openAutoLinkBut;
	CButton m_closeAutoLinkBut;
	afx_msg void OnClose();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedFindDir();
	afx_msg void OnDBClick(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnRClickDevList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSetHistoryConnect();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedButtonDir();
	afx_msg void OnBnClickedButtonFilename();
};
