// HvOcxTestDlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "atltime.h"
#include "hvdeviceaxctrl1.h"

// CHvOcxTestDlg �Ի���
class CHvOcxTestDlg : public CDialog
{
// ����
public:
	CHvOcxTestDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_HVOCXTEST_DIALOG };

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
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedPatch();
	DECLARE_EVENTSINK_MAP()
	void OnReceivePlateHvdeviceaxctrl1();
	void OnReceiveJpegVideoHvdeviceaxctrl1();
	void OnReceiveH264VideoHvdeviceaxctrl1();
public:
	int m_Jpegnum;
	CString m_strFullName;
	bool m_bExit;
	void WrightLogEx(LPCSTR lpszLogInfo);
	bool m_record;
	CIPAddressCtrl m_IP;//ip	
private:
	CButton m_GetBigImage;
	CButton m_GetSmallImage;
	CButton m_GetBinImage;
	CButton m_Getjpeg;
	CButton m_Geth264;
	CButton m_isShowVedio;
	BOOL m_SaveBigImage; //��ͼ
	BOOL m_SaveSmallImge;//Сͼ
	BOOL m_SaveBin; //��λͼ
	BOOL m_SaveBinBMP; //��λͼתbmp
	BOOL m_Savejpeg;//jpeg��
	BOOL m_Saveh264;//264��
	BOOL m_isSaveIllegalVideo;
	bool m_fisconnect;//�Ƿ�����
	DWORD32 m_jpegnum;
	int m_connecttime;
	bool m_SetSaveTime;//����ʱ��
	CString m_Patch;
	int m_SaveTime;
	int m_H264FrameRateNum;
	int m_GetH264FramRateNum;
	int m_DevStatue;
	HANDLE m_hThread;
public:
	afx_msg void OnBnClickedConnect();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnBnClickedCheck1();
	afx_msg void OnBnClickedCheck2();
	afx_msg void OnBnClickedCheck3();
	afx_msg void OnBnClickedCheck4();
	afx_msg void OnBnClickedCheck5();
	afx_msg void OnBnClickedCheck6();
	afx_msg void OnBnClickedCheck7();
	afx_msg void OnBnClickedCheck8();
	afx_msg void OnBnClickedCheck9();
	afx_msg void OnBnClickedCheck10();
	afx_msg void OnBnClickedCheck11();
	afx_msg void OnBnClickedSetTime();
	afx_msg void OnClose();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedSetTime2();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedCheckShowVedio();
	afx_msg void OnBnClickedButtonInputlist();
	afx_msg void OnBnClickedButtonGetlist();
	CHvdeviceaxctrl1 m_HAC;
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton7();
	afx_msg void OnBnClickedButton8();
	afx_msg void OnBnClickedButton9();
	afx_msg void OnBnClickedButton10();
	afx_msg void OnBnClickedCkillegalvideo();
	CButton m_RecvIllegalVideo;
	afx_msg void OnBnClickedCksaveillegalvideo();
	CButton m_ButtonSaveIllegalVideo;
	afx_msg void OnBnClickedCkillegalvideo2();
	CButton m_ckSaveFileMode;
	afx_msg void OnBnClickedButtonGetimage();
	afx_msg void OnBnClickedCheckIconhide();
	CButton m_cbHideIcon;
	afx_msg void OnBnClickedButton11();
	afx_msg void OnBnClickedButton12();
	afx_msg void OnBnClickedButtonGetinfo();
	afx_msg void OnBnClickedButtonSetinfo();
	afx_msg void OnBnClickedButtonStart();
	//afx_msg void OnTimer(UINT_PTR nIDEvent);
};
