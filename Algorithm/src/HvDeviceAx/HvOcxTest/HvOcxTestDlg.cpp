// HvOcxTestDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "HvOcxTest.h"
#include "HvOcxTestDlg.h"
#include ".\hvocxtestdlg.h"

#include <Shlwapi.h>
#pragma  comment(lib, "Shlwapi.lib")

#include <Dbghelp.h>
#pragma  comment(lib, "Dbghelp.lib")



#include "AppDBGHelp.h"


//�õ�����ǰĿ¼
CString GetCurrentDir(void)
{
	CString strPath;

	TCHAR szFileName[ MAX_PATH ] = {0};
	TCHAR szIniName[MAX_PATH] = {0};
	TCHAR szBakName[MAX_PATH] = {0};
	GetModuleFileName( NULL, szFileName, MAX_PATH );	//ȡ�ð�����������ȫ·��
	PathRemoveFileSpec( szFileName );				//ȥ��������

	strPath = szFileName;
	strPath += "\\";
	return strPath;
}




#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define DEBUGOCX

// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

	// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CHvOcxTestDlg �Ի���

DWORD WINAPI GetFullNameResultThread(LPVOID lpParam);

CHvOcxTestDlg::CHvOcxTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CHvOcxTestDlg::IDD, pParent)
	, m_DevStatue(0)
	, m_SaveBigImage(FALSE)
	, m_SaveSmallImge(FALSE)
	, m_SaveBin(FALSE)
	, m_SaveBinBMP(FALSE)
	, m_Savejpeg(FALSE)
	, m_Saveh264(FALSE)
	, m_SaveTime(0)
	, m_H264FrameRateNum(25)
	, m_GetH264FramRateNum(25)
	, m_SetSaveTime(false)
	, m_Patch(_T(""))
	, m_fisconnect(false)
	, m_jpegnum(0)
	, m_connecttime(0)
	, m_Jpegnum(0)
	, m_record(false)
	, m_isSaveIllegalVideo(FALSE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CHvOcxTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IP, m_IP);
	DDX_Control(pDX, IDC_CHECK1, m_GetBigImage);
	DDX_Control(pDX, IDC_CHECK2, m_GetSmallImage);
	DDX_Control(pDX, IDC_CHECK3, m_GetBinImage);
	DDX_Control(pDX, IDC_CHECK4, m_Getjpeg);
	DDX_Control(pDX, IDC_CHECK5, m_Geth264);
	DDX_Check(pDX, IDC_CHECK6, m_SaveBigImage);
	DDX_Check(pDX, IDC_CHECK7, m_SaveSmallImge);
	DDX_Check(pDX, IDC_CHECK8, m_SaveBin);
	DDX_Check(pDX, IDC_CHECK9, m_SaveBinBMP);
	DDX_Check(pDX, IDC_CHECK10, m_Savejpeg);
	DDX_Check(pDX, IDC_CHECK11, m_Saveh264);
	DDX_Text(pDX, IDC_EDIT_PATCH2, m_SaveTime);
	DDX_Text(pDX, IDC_EDIT_PATCH3, m_H264FrameRateNum);
	DDX_Text(pDX, IDC_EDIT_CUR_FRAM, m_GetH264FramRateNum);
	DDX_Text(pDX, IDC_EDIT_NAME, m_strFullName);
	DDX_Control(pDX, IDC_CHECK_SHOW_VEDIO, m_isShowVedio);
	DDX_Control(pDX, IDC_HVDEVICEAXCTRL1, m_HAC);
	DDX_Control(pDX, IDC_CKIllegalVideo, m_RecvIllegalVideo);
	DDX_Control(pDX, IDC_CKSaveIllegalVideo, m_ButtonSaveIllegalVideo);
	DDX_Control(pDX, IDC_CKIllegalVideo2, m_ckSaveFileMode);
	DDX_Control(pDX, IDC_CHECK_ICONHIDE, m_cbHideIcon);
}

BEGIN_MESSAGE_MAP(CHvOcxTestDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_CONNECT, OnBnClickedConnect)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_PATCH, OnBnClickedPatch)
	ON_BN_CLICKED(IDC_CHECK1, OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK2, OnBnClickedCheck2)
	ON_BN_CLICKED(IDC_CHECK3, OnBnClickedCheck3)
	ON_BN_CLICKED(IDC_CHECK4, OnBnClickedCheck4)
	ON_BN_CLICKED(IDC_CHECK5, OnBnClickedCheck5)
	ON_BN_CLICKED(IDC_CHECK6, OnBnClickedCheck6)
	ON_BN_CLICKED(IDC_CHECK7, OnBnClickedCheck7)
	ON_BN_CLICKED(IDC_CHECK8, OnBnClickedCheck8)
	ON_BN_CLICKED(IDC_CHECK9, OnBnClickedCheck9)
	ON_BN_CLICKED(IDC_CHECK10, OnBnClickedCheck10)
	ON_BN_CLICKED(IDC_CHECK11, OnBnClickedCheck11)
	ON_BN_CLICKED(IDC_SET_TIME, OnBnClickedSetTime)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, OnBnClickedButton3)
	ON_BN_CLICKED(IDC_SET_TIME2, OnBnClickedSetTime2)
	ON_BN_CLICKED(IDC_BUTTON4, OnBnClickedButton4)
	ON_BN_CLICKED(IDC_CHECK_SHOW_VEDIO, OnBnClickedCheckShowVedio)
	ON_BN_CLICKED(IDC_BUTTON_INPUTLIST, OnBnClickedButtonInputlist)
	ON_BN_CLICKED(IDC_BUTTON_GETLIST, OnBnClickedButtonGetlist)
	ON_BN_CLICKED(IDC_BUTTON5, OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON7, OnBnClickedButton7)
	ON_BN_CLICKED(IDC_BUTTON8, OnBnClickedButton8)
	ON_BN_CLICKED(IDC_BUTTON9, OnBnClickedButton9)
	ON_BN_CLICKED(IDC_BUTTON10, OnBnClickedButton10)
	ON_BN_CLICKED(IDC_CKIllegalVideo, &CHvOcxTestDlg::OnBnClickedCkillegalvideo)
	ON_BN_CLICKED(IDC_CKSaveIllegalVideo, &CHvOcxTestDlg::OnBnClickedCksaveillegalvideo)
	ON_BN_CLICKED(IDC_CKIllegalVideo2, &CHvOcxTestDlg::OnBnClickedCkillegalvideo2)
	ON_BN_CLICKED(IDC_BUTTON_GETIMAGE, &CHvOcxTestDlg::OnBnClickedButtonGetimage)
	ON_BN_CLICKED(IDC_CHECK_ICONHIDE, &CHvOcxTestDlg::OnBnClickedCheckIconhide)
	ON_BN_CLICKED(IDC_BUTTON11, &CHvOcxTestDlg::OnBnClickedButton11)
	ON_BN_CLICKED(IDC_BUTTON12, &CHvOcxTestDlg::OnBnClickedButton12)
	ON_BN_CLICKED(IDC_BUTTON_GETINFO, &CHvOcxTestDlg::OnBnClickedButtonGetinfo)
	ON_BN_CLICKED(IDC_BUTTON_SETINFO, &CHvOcxTestDlg::OnBnClickedButtonSetinfo)
	ON_BN_CLICKED(IDC_BUTTON_START, &CHvOcxTestDlg::OnBnClickedButtonStart)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CHvOcxTestDlg ��Ϣ�������

BOOL CHvOcxTestDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��\������...\���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	GetDlgItem(IDC_CHECK6)->EnableWindow(false);
	GetDlgItem(IDC_CHECK7)->EnableWindow(false);
	GetDlgItem(IDC_CHECK8)->EnableWindow(false);
	GetDlgItem(IDC_CHECK9)->EnableWindow(false);
	GetDlgItem(IDC_CHECK10)->EnableWindow(false);
	GetDlgItem(IDC_CHECK11)->EnableWindow(false);
	GetDlgItem(IDC_CHECK_SHOW_VEDIO)->EnableWindow(false);
	m_SaveTime = 10;
	m_Patch = "\\\\hvserver\\HvShare\\store\\temp\\Xur";
	m_bExit = true;
	m_strFullName = "D:\\HAFULLNAME\\Plate";
	GetDlgItem(IDC_EDIT_PATCH)->SetWindowText(m_Patch);
	CreateDirectory(m_Patch.GetBuffer(), NULL);
	m_Patch.ReleaseBuffer();
	GetDlgItem(IDC_EDIT_PATCH2)->EnableWindow(false);
	GetDlgItem(IDC_SET_TIME)->EnableWindow(false);

	//��ʱע�͵�����
	//GetDlgItem(IDC_EDIT_PATCH3)->EnableWindow(false);
	//GetDlgItem(IDC_SET_TIME2)->EnableWindow(false);

	m_GetBigImage.EnableWindow(false);
	m_GetSmallImage.EnableWindow(false);
	//m_GetSmallImage.SetCheck(1);
	m_GetBinImage.EnableWindow(false);
	m_Getjpeg.EnableWindow(false);
	m_Geth264.EnableWindow(false);
	m_RecvIllegalVideo.EnableWindow(false);
	m_ButtonSaveIllegalVideo.EnableWindow(false);
	m_IP.SetWindowText("172.18.20.40");//m_IP.SetWindowText("172.18.0.2");
	UpdateData(false);
#ifdef DEBUG_HELP_FLAG   
	//������
	SetUnhandledExceptionFilter(MyUnhandledFilter);
#endif
	return TRUE;  // ���������˿ؼ��Ľ��㣬���򷵻� TRUE
}

void CHvOcxTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CHvOcxTestDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ��������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù����ʾ��
HCURSOR CHvOcxTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CHvOcxTestDlg::OnBnClickedConnect()
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_CONNECT)->EnableWindow(false);
	// ���ùرհ�ť
	CMenu *menu = GetSystemMenu(FALSE);
	if (menu != NULL)
	{
		menu->EnableMenuItem(SC_CLOSE, MF_BYCOMMAND | MF_GRAYED);
	}

	if(m_fisconnect == false)
	{
		//	m_HAC.Disconnect();
		CString IPtemp;
		m_IP.GetWindowText(IPtemp);
		m_HAC.ConnectTo(IPtemp);
		//ʹ�ܽ��ս��
		m_HAC.SetRecvPlateBinImageFlag(1);
		m_HAC.SetRecvPlateImageFlag(1);
		m_HAC.SetRecvSnapImageFlag(1);
		GetDlgItem(IDC_BUTTON1)->SetWindowText("ȡ������ص�");
		m_HAC.SetRecvRecordFlag(1);

		//��ʼ�����ս��
		m_GetBigImage.EnableWindow(true);
		m_GetBigImage.SetCheck(1);
		m_GetSmallImage.EnableWindow(true);
		m_GetSmallImage.SetCheck(1);
		m_GetBinImage.EnableWindow(true);
		m_GetBinImage.SetCheck(1);

		m_RecvIllegalVideo.EnableWindow(true);
		m_ckSaveFileMode.SetCheck(0);
		m_HAC.SetPathType(0);
		//m_ButtonSaveIllegalVideo.EnableWindow(true);
		//ʹ�ܿɱ�����
		GetDlgItem(IDC_CHECK6)->EnableWindow(true);
		GetDlgItem(IDC_CHECK7)->EnableWindow(true);
		GetDlgItem(IDC_CHECK8)->EnableWindow(true);
		GetDlgItem(IDC_CHECK9)->EnableWindow(true);
		GetDlgItem(IDC_CHECK_SHOW_VEDIO)->EnableWindow(false);
		//�ж��Ƿ���ʾ��Ƶ
		int isShow = m_HAC.GetShowH264VideoFlag();
		if(isShow == 0)
		{
			m_isShowVedio.SetCheck(0);
		}
		else
		{
			m_isShowVedio.SetCheck(1);
		}

		GetDlgItem(IDC_EDIT_PATCH3)->EnableWindow(true);
		GetDlgItem(IDC_SET_TIME2)->EnableWindow(true);

		m_Getjpeg.EnableWindow(true);
		m_Geth264.EnableWindow(true);
		::SetTimer(GetSafeHwnd(), 1, 500, NULL); 
		WrightLogEx("��ʼ������!");
	}
	else
	{
		m_connecttime = 0;
		m_HAC.Disconnect();
		//�رս���״̬
		m_HAC.SetRecvPlateBinImageFlag(0);
		m_HAC.SetRecvPlateImageFlag(0);
		m_HAC.SetRecvSnapImageFlag(0);
		m_HAC.SetRecvH264Flag(0);
		m_HAC.SetRecvJpegFlag(0);
		//ȡ������״̬
		m_GetBigImage.EnableWindow(false);
		m_GetBigImage.SetCheck(0);
		m_GetSmallImage.EnableWindow(false);
		m_GetSmallImage.SetCheck(0);
		m_GetBinImage.EnableWindow(false);
		m_GetBinImage.SetCheck(0);
		m_Getjpeg.SetCheck(0);
		m_Getjpeg.EnableWindow(false);
		m_Geth264.SetCheck(0);
		m_Geth264.EnableWindow(false);

		m_RecvIllegalVideo.EnableWindow(false);
		m_RecvIllegalVideo.SetCheck(0);
		m_ButtonSaveIllegalVideo.EnableWindow(false);
		m_ButtonSaveIllegalVideo.SetCheck(0);

		m_ckSaveFileMode.SetCheck(0);
		//�رտɱ���״̬
		GetDlgItem(IDC_CHECK6)->EnableWindow(false);
		GetDlgItem(IDC_CHECK7)->EnableWindow(false);
		GetDlgItem(IDC_CHECK8)->EnableWindow(false);
		GetDlgItem(IDC_CHECK9)->EnableWindow(false);
		GetDlgItem(IDC_CHECK10)->EnableWindow(false);
		GetDlgItem(IDC_CHECK11)->EnableWindow(false);
		GetDlgItem(IDC_CHECK_SHOW_VEDIO)->EnableWindow(false);
		//Ĭ�ϲ���ʾ��Ƶ
		m_isShowVedio.SetCheck(0);
		m_SetSaveTime = false;
		GetDlgItem(IDC_SET_TIME)->SetWindowText("����");
		GetDlgItem(IDC_EDIT_PATCH2)->EnableWindow(false);
		GetDlgItem(IDC_EDIT_PATCH3)->EnableWindow(false);
		GetDlgItem(IDC_SET_TIME)->EnableWindow(false);
		GetDlgItem(IDC_SET_TIME2)->EnableWindow(false);
		::KillTimer(GetSafeHwnd(), 1);
		GetDlgItem(IDC_CONNECT)->SetWindowText("����");
		GetDlgItem(IDC_BUTTON1)->SetWindowText("����ص�");
		SetWindowText("�Ͽ�");
		m_record = false;
		m_HAC.SetRecvRecordFlag(0);
		m_fisconnect = false;
		GetDlgItem(IDC_CONNECT)->EnableWindow(true);
	}

	if (menu != NULL)
	{
		menu->EnableMenuItem(SC_CLOSE, MF_BYCOMMAND | MF_ENABLED);
	}
}

void CHvOcxTestDlg::OnTimer(UINT nIDEvent)
{
	// TODO: Add your message handler code here and/or call default


	if (  nIDEvent == 5) 
	{
		static int index1 = 0;
		index1++;

		CString strDir = GetCurrentDir();
		strDir += "CaptureImage\\";

		CTime tm = CTime::GetCurrentTime();

		if ( !PathFileExists(strDir) )
		{
			MakeSureDirectoryPathExists(strDir);	
		}

		char szFileName[1024] = {0};
		sprintf(szFileName, "%s%s_%d.jpg", strDir, tm.Format("%Y%m%d%H%M%S"), index1);

			if ( m_HAC.GetCaptureImage(0, szFileName) == 0 )
			{
				char szLog[1024] = {0};
				sprintf(szLog, "��ȡͼƬ�ɹ�:%s", szFileName);
			
			}

			CDialog::OnTimer(nIDEvent);

			return ;

	}


	CString status_temp;
	//CString strDebug;
	CString tttemp;
	int i = m_HAC.GetStatus(1);
	switch(i)
	{
	case 0 : tttemp = "���:δ֪  ";WrightLogEx("���:δ֪");break;
	case 1 : tttemp = "���:����  ";break;
	case 2 : tttemp = "���:�Ͽ�  ";WrightLogEx("���:�Ͽ�");break;
	case 3 : tttemp = "���:����  ";WrightLogEx("���:����");break;
	}
	int j = m_HAC.GetStatus(2);
	switch(j)
	{
	case 0 : tttemp += "jpeg:δ֪  ";break;
	case 1 : tttemp += "jpeg:����  ";break;
	case 2 : tttemp += "jpeg:�Ͽ�  ";break;
	case 3 : tttemp += "jpeg:����  ";break;
	}
	int k = m_HAC.GetStatus(3);
	switch(k)
	{
	case 0 : tttemp += "h264:δ֪";break;
	case 1 : tttemp += "h264:����";break;
	case 2 : tttemp += "h264:�Ͽ�";break;
	case 3 : tttemp += "h264:����";break;
	}
	int ii = m_HAC.GetRecvPlateImageFlag();
	int ii1 = m_HAC.GetRecvPlateBinImageFlag();
	int ii2 = m_HAC.GetRecvSnapImageFlag();
	int jj = m_HAC.GetRecvJpegFlag();
	int kk = m_HAC.GetRecvH264Flag();
	//strDebug.Format("%d", m_connecttime);
	status_temp.Format("%s__{(%d,%d,%d),%d,%d}",tttemp,ii2,ii,ii1,jj,kk);
	SetWindowText(status_temp);
	if((m_fisconnect == false))
	{
		++m_connecttime;
		if((i == 1) && (m_connecttime < 6))
		{
			m_fisconnect = true;
			GetDlgItem(IDC_CONNECT)->SetWindowText("�Ͽ�");
			m_record = true ;
			GetDlgItem(IDC_CONNECT)->EnableWindow(true);
		}
		else if(m_connecttime >= 6)
		{
			m_connecttime = 0;
			m_HAC.Disconnect();
			//�رս���״̬
			m_HAC.SetRecvPlateBinImageFlag(0);
			m_HAC.SetRecvPlateImageFlag(0);
			m_HAC.SetRecvSnapImageFlag(0);
			m_HAC.SetRecvH264Flag(0);
			m_HAC.SetRecvJpegFlag(0);
			//ȡ������״̬
			m_GetBigImage.EnableWindow(false);
			m_GetBigImage.SetCheck(0);
			m_GetSmallImage.EnableWindow(false);
			m_GetSmallImage.SetCheck(0);
			m_GetBinImage.EnableWindow(false);
			m_GetBinImage.SetCheck(0);
			m_Getjpeg.SetCheck(0);
			m_Getjpeg.EnableWindow(false);
			m_Geth264.SetCheck(0);
			m_Geth264.EnableWindow(false);

			m_RecvIllegalVideo.EnableWindow(false);
			m_RecvIllegalVideo.SetCheck(0);
			m_ButtonSaveIllegalVideo.EnableWindow(false);
			m_ButtonSaveIllegalVideo.SetCheck(0);

			m_ckSaveFileMode.SetCheck(0);
			//�رտɱ���״̬
			GetDlgItem(IDC_CHECK6)->EnableWindow(false);
			GetDlgItem(IDC_CHECK7)->EnableWindow(false);
			GetDlgItem(IDC_CHECK8)->EnableWindow(false);
			GetDlgItem(IDC_CHECK9)->EnableWindow(false);
			GetDlgItem(IDC_CHECK10)->EnableWindow(false);
			GetDlgItem(IDC_CHECK11)->EnableWindow(false);
			GetDlgItem(IDC_BUTTON1)->SetWindowText("����ص�");

			GetDlgItem(IDC_EDIT_PATCH3)->EnableWindow(false);
			GetDlgItem(IDC_SET_TIME2)->EnableWindow(false);

			m_record = false;
			m_HAC.SetRecvRecordFlag(0);
			::KillTimer(GetSafeHwnd(), 1);
			MessageBox("�����豸ʧ��");
			GetDlgItem(IDC_CONNECT)->EnableWindow(true);

		}
	}
	CDialog::OnTimer(nIDEvent);
}

// ���λس���ESC
BOOL CHvOcxTestDlg::PreTranslateMessage(MSG* pMsg)
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

void CHvOcxTestDlg::OnBnClickedPatch()
{
	// TODO: Add your control notification handler code here
	CString strFilePath;//���������ļ�·��
	BROWSEINFO bi;
	char Buffer[512];
	//��ʼ����ڲ���bi��ʼ
	bi.hwndOwner = NULL;
	bi.pidlRoot = NULL;
	bi.pszDisplayName = Buffer;//�˲�����ΪNULL������ʾ�Ի���
	bi.lpszTitle = "ѡ��·��";
	bi.ulFlags = BIF_RETURNONLYFSDIRS;
	bi.lpfn = NULL;
	bi.iImage = 0;
	//��ʼ����ڲ���bi����
	LPITEMIDLIST pIDList = SHBrowseForFolder(&bi);//������ʾѡ��Ի���
	if(pIDList)//ѡ��·��(��������ȷ����ť)
	{
		SHGetPathFromIDList(pIDList, Buffer);
		//ȡ���ļ���·����Buffer��
		strFilePath = Buffer;//��·��������һ��CString������

		m_Patch.Empty();
		m_Patch = strFilePath;
		if(m_Patch.Right(1) == "\\")
			m_Patch += "HACTest";
		else
			m_Patch += "\\HACTest";
		GetDlgItem(IDC_EDIT_PATCH)->SetWindowText(m_Patch);
		CreateDirectory(m_Patch.GetBuffer(), NULL);
		m_Patch.ReleaseBuffer();

	}
	else//ȡ��ѡ��
		return;
}
BEGIN_EVENTSINK_MAP(CHvOcxTestDlg, CDialog)
	ON_EVENT(CHvOcxTestDlg, IDC_HVDEVICEAXCTRL1, 1, OnReceivePlateHvdeviceaxctrl1, VTS_NONE)
	ON_EVENT(CHvOcxTestDlg, IDC_HVDEVICEAXCTRL1, 2, OnReceiveJpegVideoHvdeviceaxctrl1, VTS_NONE)
	ON_EVENT(CHvOcxTestDlg, IDC_HVDEVICEAXCTRL1, 3, OnReceiveH264VideoHvdeviceaxctrl1, VTS_NONE)
END_EVENTSINK_MAP()

void CHvOcxTestDlg::OnReceivePlateHvdeviceaxctrl1()
{
	// TODO: Add your message handler code here
	CString m_platepatch;
	CString ipstr;
	int iRet;
	m_IP.GetWindowText(ipstr);
	m_platepatch = m_Patch + "\\"+ ipstr;
	CreateDirectory(m_platepatch.GetBuffer(), NULL);
	m_platepatch.ReleaseBuffer();
	m_platepatch += "\\result\\";
	CreateDirectory(m_platepatch.GetBuffer(), NULL);
	m_platepatch.ReleaseBuffer();


	CString m_Value,m_CarId,m_Plate,m_PlateColor,m_PlateInfo,m_Time,m_temp , strIllegalInfo;
	m_CarId.Format("%d",m_HAC.GetCarID());
	m_temp = "����ID:" + m_CarId;
	GetDlgItem(IDC_STATIC1)->SetWindowText(m_temp);
	m_temp.Empty();

	m_Plate = m_HAC.GetPlate();
	m_temp = "����:" + m_Plate;
	GetDlgItem(IDC_STATIC2)->SetWindowText(m_temp);
	m_temp.Empty();

	m_PlateColor =  m_HAC.GetPlateColor();
	m_temp = "������ɫ:" + m_PlateColor;
	GetDlgItem(IDC_STATIC3)->SetWindowText(m_temp);
	m_temp.Empty();

	m_PlateInfo = m_HAC.GetPlateInfo();
	m_Value =  m_PlateInfo;
	m_Value.Replace("\n","\r\n");
	GetDlgItem(IDC_VALUE)->SetWindowText(m_Value);


	strIllegalInfo = m_HAC.GetIllegalReason();
	m_temp = "Υ��ԭ��:\n" + strIllegalInfo;
	GetDlgItem(IDC_SIllegalInfo)->SetWindowText(m_temp);
	m_temp.Empty();
	CString path;
	if(strIllegalInfo != "" && m_isSaveIllegalVideo)
	{
		if(m_ckSaveFileMode.GetCheck() == BST_CHECKED)
		{
			UpdateData(true);
			m_platepatch.Format("%s_%s.avi",m_strFullName,m_CarId);
		}
		iRet = m_HAC.SaveIllegalVideo(m_platepatch);
		if(iRet == 0)
		{
			path = m_HAC.GetFileName(3);
			m_temp = "Υ����Ƶ·��:" + path;
		}
		else if(iRet == -10)
		{
			m_temp = "��������";
		}
		GetDlgItem(IDC_STATIC_PATH)->SetWindowText(m_temp);
	}


	CTime m_PlateTime;

	if(m_SaveBinBMP == TRUE)
	{
		if(m_ckSaveFileMode.GetCheck() == BST_CHECKED)
		{
			UpdateData(true);
			m_platepatch.Format("%s_%s.bmp",m_strFullName,m_CarId);
		}
		iRet = m_HAC.SavePlateBin2BMP(m_platepatch);
		path.Empty();
		m_temp.Empty();
		if(iRet == 0)
		{
			path = m_HAC.GetFileName(0);
			m_temp = "��ֵͼ·��:" + path;
		}
		else if(iRet == -10)
		{
			m_temp = "��������";
		}


		GetDlgItem(IDC_STATIC_PATH)->SetWindowText(m_temp);

	}
	if(m_SaveBin == TRUE)
	{
		if(m_ckSaveFileMode.GetCheck() == BST_CHECKED)
		{
			UpdateData(true);
			m_platepatch.Format("%s_%s.bin",m_strFullName,m_CarId);
		}
		iRet = m_HAC.SavePlateBinImage(m_platepatch);
		path.Empty();
		m_temp.Empty();
		if(iRet == 0)
		{
			path = m_HAC.GetFileName(0);
			m_temp = "��ֵ��ͼ·��:" + path;
		}
		else if(iRet == -10)
		{
			m_temp = "��������";
		}


		GetDlgItem(IDC_STATIC_PATH)->SetWindowText(m_temp);
	}
	if(m_SaveSmallImge == TRUE)
	{
		if(m_ckSaveFileMode.GetCheck() == BST_CHECKED)
		{
			UpdateData(true);
			m_platepatch.Format("%s_%s.jpg",m_strFullName,m_CarId);
		}
		iRet = m_HAC.SavePlateImage(m_platepatch);
		path.Empty();
		m_temp.Empty();
		if(iRet == 0)
		{
			path = m_HAC.GetFileName(0);
			m_temp = "����Сͼ·��:" + path;
		}
		else if(iRet == -10)
		{
			m_temp = "��������";
		}


		GetDlgItem(IDC_STATIC_PATH)->SetWindowText(m_temp);
	}
	if(m_SaveBigImage == TRUE)
	{
		if(m_ckSaveFileMode.GetCheck() == BST_CHECKED)
		{
			UpdateData(true);
			m_platepatch.Format("%s_%s_0.jpg",m_strFullName,m_CarId);
		}
		iRet = m_HAC.SaveSnapImage(m_platepatch, 0);
		path.Empty();
		m_temp.Empty();
		if(iRet == 0)
		{
			path = m_HAC.GetFileName(0);
			if(path != "")
			{
				m_temp = "������ͼ·��:" + path;
				GetDlgItem(IDC_STATIC_PATH)->SetWindowText(m_temp);
			}
		}
		else if(iRet == -10)
		{
			m_temp = "��������";
			GetDlgItem(IDC_STATIC_PATH)->SetWindowText(m_temp);
			return;
		}

		if(m_ckSaveFileMode.GetCheck() == BST_CHECKED)
		{
			UpdateData(true);
			m_platepatch.Format("%s_%s_1.jpg",m_strFullName,m_CarId);
		}
		m_HAC.SaveSnapImage(m_platepatch, 1);
		path.Empty();
		m_temp.Empty();
		path = m_HAC.GetFileName(0);
		if(path != "")
		{
			m_temp = "������ͼ·��:" + path;
			GetDlgItem(IDC_STATIC_PATH)->SetWindowText(m_temp);
		}

		if(m_ckSaveFileMode.GetCheck() == BST_CHECKED)
		{
			UpdateData(true);
			m_platepatch.Format("%s_%s_2.jpg",m_strFullName,m_CarId);
		}
		m_HAC.SaveSnapImage(m_platepatch, 2);
		path.Empty();
		m_temp.Empty();
		path = m_HAC.GetFileName(0);
		if(path != "")
		{
			m_temp = "������ͼ·��:" + path;
			GetDlgItem(IDC_STATIC_PATH)->SetWindowText(m_temp);
		}

		if(m_ckSaveFileMode.GetCheck() == BST_CHECKED)
		{
			UpdateData(true);
			m_platepatch.Format("%s_%s_3.jpg",m_strFullName,m_CarId);
		}
		m_HAC.SaveSnapImage(m_platepatch, 3);
		path.Empty();
		m_temp.Empty();
		path = m_HAC.GetFileName(0);
		if(path != "")
		{
			m_temp = "������ͼ·��:" + path;
			GetDlgItem(IDC_STATIC_PATH)->SetWindowText(m_temp);
		}

		if(m_ckSaveFileMode.GetCheck() == BST_CHECKED)
		{
			UpdateData(true);
			m_platepatch.Format("%s_%s_4.jpg",m_strFullName,m_CarId);
		}
		m_HAC.SaveSnapImage(m_platepatch, 4);
		path.Empty();
		m_temp.Empty();
		path = m_HAC.GetFileName(0);
		if(path != "")
		{
			m_temp = "������ͼ·��:" + path;
			GetDlgItem(IDC_STATIC_PATH)->SetWindowText(m_temp);
		}
	}
}

void CHvOcxTestDlg::OnReceiveJpegVideoHvdeviceaxctrl1()
{
	// TODO: Add your message handler code here
	CString m_jpegpatch;
	CString ipstr;
	CString temp;
	m_IP.GetWindowText(ipstr);
	m_jpegpatch = m_Patch + "\\"+ ipstr;
	CreateDirectory(m_jpegpatch.GetBuffer(), NULL);
	m_jpegpatch.ReleaseBuffer();
	m_jpegpatch += "\\JPEG\\";
	CreateDirectory(m_jpegpatch.GetBuffer(), NULL);
	m_jpegpatch.ReleaseBuffer();
	if(m_Savejpeg == TRUE)
	{
		//++m_jpegnum;
		//CString tempnum;
		//tempnum.Format("%d",m_jpegnum);
		int iRet = m_HAC.SaveJpegVideo(m_jpegpatch);
		if(iRet == 0)
		{
			CString path;
			path = m_HAC.GetFileName(2);
			temp = "JPEG����·��:" + path;

		}
		else if(iRet == -10)
		{
			temp = "��������";
		}

		GetDlgItem(IDC_STATIC_PATH)->SetWindowText(temp);
	}
}

void CHvOcxTestDlg::OnReceiveH264VideoHvdeviceaxctrl1()
{
	// TODO: Add your message handler code here
	CString m_h264patch;
	CString ipstr;
	CString temp;
	m_IP.GetWindowText(ipstr);
	m_h264patch = m_Patch + "\\"+ ipstr;
	CreateDirectory(m_h264patch.GetBuffer(), NULL);
	m_h264patch.ReleaseBuffer();
	m_h264patch +=  "\\H264";
	CreateDirectory(m_h264patch.GetBuffer(), NULL);
	m_h264patch.ReleaseBuffer();
	if(m_Saveh264 == TRUE)
	{
		long iRet = m_HAC.SaveH264Video(m_h264patch);
		if(iRet == 0)
		{
			CString path;
			path = m_HAC.GetFileName(1);
			temp = "H264����·��:" + path;

		}
		else if(iRet == -10)
		{
			temp = "��������";
		}


		GetDlgItem(IDC_STATIC_PATH)->SetWindowText(temp);

	}
	else
	{
		m_HAC.SaveH264Video("");
	}
}

void CHvOcxTestDlg::OnBnClickedCheck1()
{
	// TODO: Add your control notification handler code here

	if(m_GetBigImage.GetCheck() == BST_UNCHECKED)
	{
		GetDlgItem(IDC_CHECK6)->EnableWindow(false);
		m_SaveBigImage = false;
		UpdateData(false);
		m_HAC.SetRecvSnapImageFlag(0);
		return ;
	}
	else
	{
		GetDlgItem(IDC_CHECK6)->EnableWindow(true);
		m_HAC.SetRecvSnapImageFlag(1);
		return ;
	}

}

void CHvOcxTestDlg::OnBnClickedCheck2()
{
	// TODO: Add your control notification handler code here
	m_GetSmallImage.EnableWindow(false);
	if(m_GetSmallImage.GetCheck() == BST_UNCHECKED)
	{
		GetDlgItem(IDC_CHECK7)->EnableWindow(false);
		m_SaveSmallImge = false;
		UpdateData(false);
		m_HAC.SetRecvPlateImageFlag(0);
		m_GetSmallImage.EnableWindow(true);
		return ;
	}
	else
	{
		GetDlgItem(IDC_CHECK7)->EnableWindow(true);
		m_HAC.SetRecvPlateImageFlag(1);
		m_GetSmallImage.EnableWindow(true);
		return ;
	}
}

void CHvOcxTestDlg::OnBnClickedCheck3()
{
	// TODO: Add your control notification handler code here
	if(m_GetBinImage.GetCheck() == BST_UNCHECKED)
	{
		GetDlgItem(IDC_CHECK8)->EnableWindow(false);
		GetDlgItem(IDC_CHECK9)->EnableWindow(false);
		m_SaveBin = false;
		m_SaveBinBMP = false;
		UpdateData(false);
		m_HAC.SetRecvPlateBinImageFlag(0);
		return ;
	}
	else
	{
		GetDlgItem(IDC_CHECK8)->EnableWindow(true);
		GetDlgItem(IDC_CHECK9)->EnableWindow(true);
		m_HAC.SetRecvPlateBinImageFlag(1);
		return ;
	}
}

void CHvOcxTestDlg::OnBnClickedCheck4()
{
	// TODO: Add your control notification handler code here
	if(m_Getjpeg.GetCheck() == BST_UNCHECKED)
	{
		m_Geth264.EnableWindow(true);
		GetDlgItem(IDC_CHECK10)->EnableWindow(false);
		m_Savejpeg = false;
		UpdateData(false);
		m_HAC.SetRecvJpegFlag(0);
		return ;
	}
	else
	{
		m_Geth264.EnableWindow(false);
		GetDlgItem(IDC_CHECK10)->EnableWindow(true);
		m_jpegnum = 0;
		m_HAC.SetRecvJpegFlag(1);
		return ;
	}
}

void CHvOcxTestDlg::OnBnClickedCheck5()
{
	// TODO: Add your control notification handler code here
	if(m_Geth264.GetCheck() == BST_UNCHECKED)
	{
		m_Getjpeg.EnableWindow(true);
		GetDlgItem(IDC_CHECK11)->EnableWindow(false);
		m_Saveh264 = false;
		UpdateData(false);
		m_HAC.SetRecvH264Flag(0);
		GetDlgItem(IDC_EDIT_PATCH2)->EnableWindow(false);
		GetDlgItem(IDC_SET_TIME)->EnableWindow(false);
		GetDlgItem(IDC_EDIT_PATCH3)->EnableWindow(true);
		GetDlgItem(IDC_SET_TIME2)->EnableWindow(true);
		GetDlgItem(IDC_CHECK_SHOW_VEDIO)->EnableWindow(false);
		return ;
	}
	else
	{
		m_Getjpeg.EnableWindow(false);
		GetDlgItem(IDC_CHECK11)->EnableWindow(true);
		m_HAC.SetRecvH264Flag(1);
		GetDlgItem(IDC_EDIT_PATCH2)->EnableWindow(true);
		GetDlgItem(IDC_SET_TIME)->EnableWindow(true);
		GetDlgItem(IDC_EDIT_PATCH3)->EnableWindow(false);
		GetDlgItem(IDC_SET_TIME2)->EnableWindow(false);
		GetDlgItem(IDC_CHECK_SHOW_VEDIO)->EnableWindow(true);
		return ;
	}
}

void CHvOcxTestDlg::OnBnClickedCheck6()
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
}

void CHvOcxTestDlg::OnBnClickedCheck7()
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
}

void CHvOcxTestDlg::OnBnClickedCheck8()
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
}

void CHvOcxTestDlg::OnBnClickedCheck9()
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
}

void CHvOcxTestDlg::OnBnClickedCheck10()
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
}

void CHvOcxTestDlg::OnBnClickedCheck11()
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
}

void CHvOcxTestDlg::OnBnClickedSetTime()
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
	if(m_SetSaveTime == false)
	{
		if(m_SaveTime < 1 || m_SaveTime > 60)
		{
			m_SaveTime = 10;
			MessageBox("������ֵ��СΪ1�����Ϊ60");
			UpdateData(false);
			return ;
		}
		m_SetSaveTime = true;
		m_HAC.SetSaveH264VideoTime(m_SaveTime);
		GetDlgItem(IDC_SET_TIME)->SetWindowText("ȡ������");
	}
	else
	{
		m_SetSaveTime = false;
		GetDlgItem(IDC_SET_TIME)->SetWindowText("����");
		m_SaveTime = 10;
		MessageBox("����������Ҫ����ʱ��Ĭ��Ϊ10����");
		m_HAC.SetSaveH264VideoTime(m_SaveTime);
		UpdateData(false);

	}
}

void CHvOcxTestDlg::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	if (IDYES == MessageBox("�Ƿ��˳�����", "�˳���ʾ", MB_YESNO))
	{
		// ���ùرհ�ť�������ظ��ر�
		CMenu *menu = GetSystemMenu(FALSE);
		if (menu != NULL)
		{
			menu->EnableMenuItem(SC_CLOSE, MF_BYCOMMAND | MF_GRAYED);
		}
		GetDlgItem(IDC_CONNECT)->EnableWindow(FALSE);
		m_HAC.Disconnect();
		CDialog::OnClose();
	}

}

void CHvOcxTestDlg::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	if(m_record == true)
	{
		m_record = false;
		GetDlgItem(IDC_BUTTON1)->SetWindowText("����ص�");
		m_HAC.SetRecvRecordFlag(0);
	}
	else
	{
		m_record = true;
		GetDlgItem(IDC_BUTTON1)->SetWindowText("ȡ������ص�");
		m_HAC.SetRecvRecordFlag(1);
	}
}
void CHvOcxTestDlg::WrightLogEx(LPCSTR lpszLogInfo)
{
	CTime cTime = CTime::GetCurrentTime();
	char szTime[256] = {0};
	FILE* fp = fopen("D:\\OcxLog.txt", "a+");
	if(fp)
	{
		sprintf(szTime, "%s  ", cTime.Format("%Y-%m-%d %H:%M:%S"));
		fwrite(szTime, strlen(szTime), 1, fp);
		fwrite(lpszLogInfo, strlen(lpszLogInfo), 1, fp);
		fwrite("\n", 1, 1, fp);
		fclose(fp);
	}
}
void CHvOcxTestDlg::OnBnClickedButton2()
{
	// TODO: Add your control notification handler code here
	CString strBuf = "ForceSend";
	if (0 == m_HAC.SoftTriggerCaptureAndForceGetResult())
	{
		char szDebugInfo[256];
		sprintf( szDebugInfo , "HVAPI_ExecCmdEx :%s Success" ,strBuf  );
		AfxMessageBox(szDebugInfo); 

	}
	else
	{
		char szDebugInfo[256];
		sprintf( szDebugInfo , "HVAPI_ExecCmdEx :%s fail" ,strBuf  );
		AfxMessageBox(szDebugInfo);
	}
}

void CHvOcxTestDlg::OnBnClickedButton3()
{
	// TODO: Add your control notification handler code here
	CString strBuf = "SendTriggerOut";
	if (0 == m_HAC.SendTriggerOut())
	{
		char szDebugInfo[256];
		sprintf( szDebugInfo , "HVAPI_ExecCmdEx :%s Success" ,strBuf  );
		AfxMessageBox(szDebugInfo); 

	}
	else
	{
		char szDebugInfo[256];
		sprintf( szDebugInfo , "HVAPI_ExecCmdEx :%s fail" ,strBuf  );
		AfxMessageBox(szDebugInfo); 

	}
}

void CHvOcxTestDlg::OnBnClickedSetTime2()
{
	// TODO: Add your control notification handler code here
	UpdateData(true);

#ifdef DEBUGOCX
	CString strDebug;
	strDebug.Format("m_H264FrameRateNum = %d  m_Geth264=%d  ", m_H264FrameRateNum, m_Geth264);
	OutputDebugStringA( strDebug);
#endif

	if ( 0 == m_HAC.SetH264FrameRateNum( m_H264FrameRateNum )  )
	{
		char szDebugInfo[256];
		sprintf( szDebugInfo , "����֡��Ϊ :%d Success" ,m_H264FrameRateNum  );
		AfxMessageBox(szDebugInfo); 

		////#ifdef DEBUGOCX

		////	strDebug.Format("GetH264FrameRateNum = %d ",  m_HAC.GetH264FrameRateNum());
		////	AfxMessageBox( strDebug);
		////#endif


	}
	else
	{
		m_H264FrameRateNum=25;
		char szDebugInfo[256];
		sprintf( szDebugInfo , "����֡��ʧ��, ��رս���h264flag �� ֡��ֵ����Ϊ5~60" ,m_H264FrameRateNum  );
		AfxMessageBox(szDebugInfo); 
		UpdateData(false);
	}	

}

void CHvOcxTestDlg::OnBnClickedButton4()
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
	m_GetH264FramRateNum = m_HAC.GetH264FrameRateNum();
	UpdateData(false);
}

void CHvOcxTestDlg::OnBnClickedCheckShowVedio()
{
	// TODO: Add your control notification handler code here
	if(m_isShowVedio.GetCheck() == BST_UNCHECKED)
	{
		m_HAC.SetShowH264VideoFlag(0);
	}
	else
	{
		m_HAC.SetShowH264VideoFlag(1);
	}
}

void CHvOcxTestDlg::OnBnClickedButtonInputlist()
{
	// TODO: Add your control notification handler code here
	const int MAX_BUFF_LIST = 1024 * 1024;
	char *szWhileNameList = new char[MAX_BUFF_LIST];
	char *szBlackNameList = new char[MAX_BUFF_LIST];

	memset(szWhileNameList, 0, MAX_BUFF_LIST);
	memset(szBlackNameList, 0, MAX_BUFF_LIST);

	char szTemp[256] = { 0 };
	CString strTemp;

	FILE *myfile = NULL;
	myfile = fopen("WhileNameList.txt", "r");
	if (myfile != NULL )
	{
		char *pTemp = szWhileNameList;
		memset(szTemp, 0, 256);
		while (fgets(szTemp, 256, myfile) != NULL )
		{
			strTemp = szTemp;
			strTemp.Trim("\n");
			strTemp+="$";
			if ( strTemp.GetLength()<8)
				continue;

			int iLen = strTemp.GetLength();
			sprintf(pTemp, "%s", strTemp);
			pTemp+=iLen;
		}
		fclose(myfile);	
		myfile = NULL;
	}

	myfile =fopen("BlackNameList.txt", "r");
	if (myfile != NULL )
	{
		char *pTemp = szBlackNameList;
		memset(szTemp, 0, 256);
		while (fgets(szTemp, 256, myfile) != NULL )
		{
			strTemp = szTemp;
			strTemp.Trim("\n");
			strTemp+="$";
			if ( strTemp.GetLength()<8)
				continue;

			int iLen = strTemp.GetLength();
			sprintf(pTemp, "%s", strTemp);
			pTemp+=iLen;
		}
		fclose(myfile);	
		myfile = NULL;
	}

	int iWhileNameListLen = 0;
	int iBlackNameListLen = 0;
	iWhileNameListLen = (int)strlen(szWhileNameList);
	iBlackNameListLen = (int)strlen(szBlackNameList);

	if(S_OK == m_HAC.InportNameListEx(szWhileNameList, iWhileNameListLen, szBlackNameList, iBlackNameListLen))
	{
		AfxMessageBox("��������ɹ���", MB_OK);
	}
	else
	{
		AfxMessageBox("��������ʧ�ܣ�", MB_OK);
	}

	delete[] szWhileNameList;
	delete[] szBlackNameList;

	return ;
}

void CHvOcxTestDlg::OnBnClickedButtonGetlist()
{
	// TODO: Add your control notification handler code here
	CString chNameList = m_HAC.GetNameListEx();

	FILE *myFile = NULL;
	myFile = fopen("AllNameList.txt", "w");	
	if ( myFile != NULL )
	{
		if (!chNameList.IsEmpty())
		{
			fprintf(myFile, "%s\n", chNameList);
			fclose(myFile);
			myFile = NULL;
		}
	}

	AfxMessageBox("���غ��ˣ�", MB_OK);
}

void CHvOcxTestDlg::OnBnClickedButton5()
{
	// TODO: Add your control notification handler code here
	if (S_OK == m_HAC.TriggerSignal(0))
	{
		AfxMessageBox("��������ɹ���", MB_OK);
	}
	else
	{
		AfxMessageBox("��������ʧ�ܣ�", MB_OK);
	}
}

void CHvOcxTestDlg::OnBnClickedButton7()
{
	// TODO: Add your control notification handler code here
	if (S_OK == m_HAC.TriggerSignal(1))
	{
		AfxMessageBox("��������ɹ���", MB_OK);
	}
	else
	{
		AfxMessageBox("��������ʧ�ܣ�", MB_OK);
	}
}

void CHvOcxTestDlg::OnBnClickedButton8()
{
	// TODO: Add your control notification handler code here
	if (S_OK == m_HAC.TriggerAlarmSignal())
	{
		AfxMessageBox("��������ɹ���", MB_OK);
	}
	else
	{
		AfxMessageBox("��������ʧ�ܣ�", MB_OK);
	}
}

void CHvOcxTestDlg::OnBnClickedButton9()
{
	// TODO: Add your control notification handler code here
	m_HAC.SetPathType(1);
	//m_bExit = false;
	//m_hThread = CreateThread(NULL, 0, GetFullNameResultThread, this, 0, NULL);
	GetDlgItem(IDC_BUTTON9)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON10)->EnableWindow(TRUE);
}

void CHvOcxTestDlg::OnBnClickedButton10()
{
	// TODO: Add your control notification handler code here
	m_HAC.SetPathType(0);
	//m_bExit = true;
	GetDlgItem(IDC_BUTTON9)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON10)->EnableWindow(FALSE);
}

static int iCount = 0; // ȫ�ļ����������
DWORD WINAPI GetFullNameResultThread(LPVOID lpParam)
{
	CHvOcxTestDlg* pThis = (CHvOcxTestDlg*)lpParam;
	CString strPath;
	while(!pThis->m_bExit)
	{
		pThis->UpdateData(true);
		//����Ŀ¼
		strPath = pThis->m_strFullName;
		strPath.Delete(strPath.ReverseFind('\\'), 100);
		if ( !PathFileExists(strPath) )
		{
			//MakeSureDirectoryPathExists(strPath.GetBuffer(strPath.GetLength()));
			CreateDirectory(strPath.GetBuffer(strPath.GetLength()), NULL);
		}
		strPath.ReleaseBuffer();

		CString strFullName = "D:\\HAFULLNAME\\plate";
		strFullName.Format("%s-%d", pThis->m_strFullName, iCount);
		pThis->m_HAC.SavePlateBin2BMP(strFullName + ".bmp");
		pThis->m_HAC.SavePlateBinImage(strFullName + ".bin");
		pThis->m_HAC.SavePlateImage(strFullName + ".jpeg");
		pThis->m_HAC.SaveSnapImage(strFullName + "-0.jpeg", 0);
		pThis->m_HAC.SaveSnapImage(strFullName + "-1.jpeg", 1);
		pThis->m_HAC.SaveSnapImage(strFullName + "-2.jpeg", 2);
		pThis->m_HAC.SaveSnapImage(strFullName + "-3.jpeg", 3);
		pThis->m_HAC.SaveSnapImage(strFullName + "-4.jpeg", 4);

		iCount++;
		Sleep(1000 * 2);
	}
	return 0;
}

void CHvOcxTestDlg::OnBnClickedCkillegalvideo()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(m_RecvIllegalVideo.GetCheck() == BST_UNCHECKED)
	{
		GetDlgItem(IDC_CKSaveIllegalVideo)->EnableWindow(false);
		UpdateData(false);
		m_HAC.SetRecvIllegalVideoFlag(0);
		return ;
	}
	else
	{
		GetDlgItem(IDC_CKSaveIllegalVideo)->EnableWindow(true);
		UpdateData(false);
		m_HAC.SetRecvIllegalVideoFlag(1);
		return ;
	}
}


void CHvOcxTestDlg::OnBnClickedCksaveillegalvideo()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(true);
	if(m_ButtonSaveIllegalVideo.GetCheck() == BST_UNCHECKED)
	{
		m_isSaveIllegalVideo = false;
	}
	else
	{
		m_isSaveIllegalVideo = true;
	}
}


void CHvOcxTestDlg::OnBnClickedCkillegalvideo2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(true);
	if(m_ckSaveFileMode.GetCheck() == BST_UNCHECKED)
	{
		m_HAC.SetPathType(0);
	}
	else
	{
		m_HAC.SetPathType(1);
	}
}


//��ȡһ��ͼƬ

void CHvOcxTestDlg::OnBnClickedButtonGetimage()
{
	// TODO: 
	static int index = 0;
	index++;

	CString strDir = GetCurrentDir();
	strDir += "CaptureImage\\";

	CTime tm = CTime::GetCurrentTime();

	if ( !PathFileExists(strDir) )
	{
		MakeSureDirectoryPathExists(strDir);	
	}

	char szFileName[1024] = {0};
	sprintf(szFileName, "%s%s_%d.jpg", strDir, tm.Format("%Y%m%d%H%M%S"), index);

	if ( m_HAC.GetCaptureImage(0, szFileName) == 0 )
	{
		char szLog[1024] = {0};
		sprintf(szLog, "��ȡͼƬ�ɹ�:%s", szFileName);
		//AfxMessageBox(szLog, MB_OK);
	}
	/*
	else
	{
		AfxMessageBox("��ȡͼƬʧ�ܣ�", MB_OK);
	}*/

}


void CHvOcxTestDlg::OnBnClickedCheckIconhide()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (m_cbHideIcon.GetCheck() == BST_UNCHECKED )
	{
		m_HAC.SetHideIcon(0);
	}
	else
	{
		m_HAC.SetHideIcon(1);
	}
}

void CHvOcxTestDlg::OnBnClickedButton11()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString szSN = m_HAC.GetDevSN();
	GetDlgItem(IDC_SN_TXT)->SetWindowText("SN: "+szSN);
}


void CHvOcxTestDlg::OnBnClickedButton12()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString szMac = m_HAC.GetDevMac();
	GetDlgItem(IDC_MAC_TXT)->SetWindowText("MAC: "+szMac);
}


void CHvOcxTestDlg::OnBnClickedButtonGetinfo()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//IDC_EDIT_INFO
	GetDlgItem(IDC_EDIT_INFO)->SetWindowText("");
	CString strInfo = m_HAC.GetCusTomInfo();
	GetDlgItem(IDC_EDIT_INFO)->SetWindowText(strInfo);

	return ;

}


void CHvOcxTestDlg::OnBnClickedButtonSetinfo()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString strInfo;
	GetDlgItem(IDC_EDIT_INFO)->GetWindowText(strInfo);

	if ( m_HAC.SetCusTomInfo(strInfo) == 0 )
	{
		AfxMessageBox("���óɹ�");
	}
	else
	{
		AfxMessageBox("����ʧ��");
	}


}


void CHvOcxTestDlg::OnBnClickedButtonStart()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	CString strInfo;
	GetDlgItem(IDC_BUTTON_START)->GetWindowText(strInfo);

	if ( strInfo == "��ʼ" )
	{
		GetDlgItem(IDC_EDIT_INFO)->SetWindowText("����");

		SetTimer(5, 1000, NULL);
	}
	else
	{
		GetDlgItem(IDC_EDIT_INFO)->SetWindowText("��ʼ");
		KillTimer(5);
	}


	


}



