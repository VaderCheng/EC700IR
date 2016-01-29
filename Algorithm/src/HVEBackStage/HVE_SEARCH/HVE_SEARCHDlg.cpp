// HVE_SEARCHDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "HVE_SEARCH.h"
#include "HVE_SEARCHDlg.h"
#include ".\hve_searchdlg.h"

#include <shlwapi.h>

#pragma comment(lib, "shlwapi.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define WM_SEARCHEND WM_USER+1000

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


// CHVE_SEARCHDlg �Ի���



CHVE_SEARCHDlg::CHVE_SEARCHDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CHVE_SEARCHDlg::IDD, pParent)
	, m_bIsSearching(false)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_pConnection = NULL;
	m_strViewName = _T("HVE_SEARCH_SOFTWARE_VIEW");

}

void CHVE_SEARCHDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DATETIMEPICKERSTART, m_StartTimeCtrl);
	DDX_Control(pDX, IDC_DATETIMEPICKEREND, m_EndTimeCtrl);
	DDX_Control(pDX, IDC_COMBOLOCATION, m_ComboLocation);
	DDX_Control(pDX, IDC_COMBOCARTYPE, m_ComboCarType);
	DDX_Control(pDX, IDC_COMBOROADNAME, m_ComboRoadName);
	DDX_Control(pDX, IDC_COMBODIRECTION, m_ComboDirection);
}

BEGIN_MESSAGE_MAP(CHVE_SEARCHDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTONCONNECT, OnBnClickedButtonconnect)
	ON_BN_CLICKED(IDC_BUTTONSEARCH, OnBnClickedButtonsearch)
	ON_BN_CLICKED(IDC_BUTTONDISCONNECT, OnBnClickedButtondisconnect)
	ON_MESSAGE(WM_SEARCHEND, OnSearchEnd)
END_MESSAGE_MAP()


// CHVE_SEARCHDlg ��Ϣ�������

BOOL CHVE_SEARCHDlg::OnInitDialog()
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

	char szAppPath[MAX_PATH] = {0};
	GetModuleFileName(NULL, szAppPath, MAX_PATH);
	PathRemoveFileSpec(szAppPath);
	m_strIniFilePath.Format("%s\\Config.ini", szAppPath);

	m_strAppPath.Format("%s", szAppPath);

	DisConnectDB();

	InitConfig();
	
	m_pShowLoading = new CShowLoading(this);
	m_pShowLoading->CreateOwnerEx();

	return TRUE;  // ���������˿ؼ��Ľ��㣬���򷵻� TRUE
}

void CHVE_SEARCHDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CHVE_SEARCHDlg::OnPaint() 
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
HCURSOR CHVE_SEARCHDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CHVE_SEARCHDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	// OnOK();
}

void CHVE_SEARCHDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here

	if (m_bIsSearching)
	{
		AfxMessageBox("���ڽ��в�ѯ");
		return;
	}
	DisConnectDB();

	delete m_pShowLoading;

	if (m_dlgResult.m_hWnd)
	{
		m_dlgResult.DestroyWindow();
	}

	DeleteDirectory(m_strAppPath + "\\TmpResultTmp");

	OnCancel();
}

int CHVE_SEARCHDlg::InitConfig(void)
{

	GetPrivateProfileStringA("DBInfo", "ServerIP", "", m_strServerIP.GetBuffer(256), 256, m_strIniFilePath.GetBuffer(256));
	m_strServerIP.ReleaseBuffer();
	m_strIniFilePath.ReleaseBuffer();
	SetDlgItemText(IDC_IPADDRESSSERVER, m_strServerIP);

	GetPrivateProfileStringA("DBInfo", "DBName", "", m_strDBName.GetBuffer(256), 256, m_strIniFilePath.GetBuffer(256));
	m_strDBName.ReleaseBuffer();
	m_strIniFilePath.ReleaseBuffer();
	SetDlgItemText(IDC_EDITDBNAME, m_strDBName);

	GetPrivateProfileStringA("DBInfo", "LoginName", "", m_strLoginName.GetBuffer(256), 256, m_strIniFilePath.GetBuffer(256));
	m_strLoginName.ReleaseBuffer();
	m_strIniFilePath.ReleaseBuffer();
	SetDlgItemText(IDC_EDITNAME, m_strLoginName);

	GetPrivateProfileStringA("DBInfo", "Password", "", m_strPassword.GetBuffer(256), 256, m_strIniFilePath.GetBuffer(256));
	m_strPassword.ReleaseBuffer();
	m_strIniFilePath.ReleaseBuffer();
	SetDlgItemText(IDC_EDITPASSWORD, m_strPassword);

	return 0;
}

int CHVE_SEARCHDlg::SaveConfig(void)
{

	WritePrivateProfileStringA("DBInfo", "ServerIP", m_strServerIP.GetBuffer(256), m_strIniFilePath.GetBuffer(256));
	m_strServerIP.ReleaseBuffer();
	m_strIniFilePath.ReleaseBuffer();


	WritePrivateProfileStringA("DBInfo", "DBName", m_strDBName.GetBuffer(256), m_strIniFilePath.GetBuffer(256));
	m_strDBName.ReleaseBuffer();
	m_strIniFilePath.ReleaseBuffer();


	WritePrivateProfileStringA("DBInfo", "LoginName", m_strLoginName.GetBuffer(256), m_strIniFilePath.GetBuffer(256));
	m_strLoginName.ReleaseBuffer();
	m_strIniFilePath.ReleaseBuffer();


	WritePrivateProfileStringA("DBInfo", "Password", m_strPassword.GetBuffer(256), m_strIniFilePath.GetBuffer(256));
	m_strPassword.ReleaseBuffer();
	m_strIniFilePath.ReleaseBuffer();

	return 0;
}

void CHVE_SEARCHDlg::OnBnClickedButtonconnect()
{
	// TODO: Add your control notification handler code here

	GetDlgItemText(IDC_IPADDRESSSERVER, m_strServerIP);
	GetDlgItemText(IDC_EDITDBNAME, m_strDBName);
	GetDlgItemText(IDC_EDITNAME, m_strLoginName);
	GetDlgItemText(IDC_EDITPASSWORD, m_strPassword);

	if (-1 == ConnectDB())
	{
		return;
	}

	SaveConfig();
	
}

void CHVE_SEARCHDlg::OnBnClickedButtonsearch()
{
	// TODO: Add your control notification handler code here

	

// hide result dlg
	if (m_dlgResult.m_hWnd)
	{
		m_dlgResult.DestroyWindow();
	}

	// create result dlg
	if (NULL == m_dlgResult.m_hWnd)
	{
		m_dlgResult.Create(IDD_DIALOGSEACHRESULT, this);
	}
	m_dlgResult.ShowWindow(SW_HIDE);
	m_dlgResult.m_listResult.DeleteAllItems();


// get search condition
	GetDlgItemText(IDC_COMBOLOCATION, m_strSearchLocationInfo);
	GetDlgItemText(IDC_COMBOCARTYPE, m_strSearchCarTypeInfo);
	GetDlgItemText(IDC_COMBOROADNAME, m_strSearchRoadName);
	GetDlgItemText(IDC_COMBODIRECTION, m_strDirection);
	GetDlgItemText(IDC_EDITCARNUM, m_strSearchCarNum);
	m_strSearchCarNum.TrimLeft();
	m_strSearchCarNum.TrimRight();
	m_StartTimeCtrl.GetTime(m_SearchStartTime);
	m_EndTimeCtrl.GetTime(m_SearchEndTime);

	if (m_SearchEndTime < m_SearchStartTime)
	{
		AfxMessageBox("��ʼʱ�䲻�ܴ��ڽ���ʱ��");
		return;
	}

	GetDlgItem(IDC_BUTTONSEARCH)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTONDISCONNECT)->EnableWindow(FALSE);

	m_bIsSearching = true;

	m_pShowLoading->Start();
	m_pShowLoading->SetInfoWord("���ڲ�ѯ�����Ժ�");

	AfxBeginThread(SearchThread, this);

}


int CHVE_SEARCHDlg::InitSearchCondition(void)
{
	m_StartTimeCtrl.SetFormat("yyyy��MM��dd�� HH:mm");
	m_EndTimeCtrl.SetFormat("yyyy��MM��dd�� HH:mm");

	CTime cEndTime = CTime::GetCurrentTime();
	DWORD64 dw64Time = cEndTime.GetTime();
	dw64Time -= 3600;
	CTime cStartTime(dw64Time);
	m_StartTimeCtrl.SetTime(&cStartTime);
	m_EndTimeCtrl.SetTime(&cEndTime);


	m_ComboLocation.Clear();
	m_ComboLocation.ResetContent();
	m_ComboLocation.AddString(_T("���ж���"));
	m_ComboLocation.SetCurSel(0);

	m_ComboCarType.Clear();
	m_ComboCarType.ResetContent();
	m_ComboCarType.AddString(_T("ȫ������"));
	m_ComboCarType.SetCurSel(0);

	m_ComboRoadName.Clear();
	m_ComboRoadName.ResetContent();
	m_ComboRoadName.AddString(_T("����"));
	m_ComboRoadName.SetCurSel(0);

	m_ComboDirection.Clear();
	m_ComboDirection.ResetContent();
	m_ComboDirection.InsertString(-1, _T("����"));
	m_ComboDirection.InsertString(-1, _T("����"));
	m_ComboDirection.InsertString(-1, _T("����"));
	m_ComboDirection.SetCurSel(0);

	SetDlgItemText(IDC_EDITCARNUM, _T(""));

	return 0;
}

int CHVE_SEARCHDlg::ConnectDB(void)
{
	DisConnectDB();

	CString strConnect;
	strConnect.Format("Provider=SQLOLEDB.1;Password=%s;User ID=%s;Initial Catalog=%s;Data Source=%s",
		m_strPassword, m_strLoginName, m_strDBName, m_strServerIP);

	HRESULT hr;
	
	::CoInitialize(NULL);

	try
	{
		hr = m_pConnection.CreateInstance("ADODB.Connection");
	}
	catch (_com_error e)
	{
		CString strerror;
		strerror.Format("���ݿ����Ӵ���ʧ��\n��Ϣ��\n%s\n%s", e.ErrorMessage(), (LPSTR)e.Description());
		AfxMessageBox(strerror);
		m_pConnection = NULL;
		::CoUninitialize();
		return -1;
	}

	if(SUCCEEDED(hr))
	{
		m_pConnection->ConnectionTimeout = 10;
		m_pConnection->CommandTimeout = 0;
		m_pConnection->CursorLocation = adUseServer;
		m_pConnection->IsolationLevel = adXactReadCommitted;

 		try
		{
			SetWindowText("������ѯ��� - ��������");
			hr = m_pConnection->Open((_bstr_t)(strConnect), "", "", adModeUnknown);
		}
		catch (_com_error e)
		{
			CString strerror;
			strerror.Format("���ݿ�����ʧ��\n��Ϣ��\n%s\n%s", e.ErrorMessage(), (LPSTR)e.Description());
			AfxMessageBox(strerror);

			m_pConnection->Cancel();
			m_pConnection = NULL;
			::CoUninitialize();
			SetWindowText("������ѯ��� - δ�������ݿ�");
			return -1;
		}

	}

	

	::CoUninitialize();
	
	GetDlgItem(IDC_COMBOLOCATION)->EnableWindow(TRUE);
	GetDlgItem(IDC_COMBOCARTYPE)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDITCARNUM)->EnableWindow(TRUE);
	GetDlgItem(IDC_COMBOROADNAME)->EnableWindow(TRUE);
	GetDlgItem(IDC_COMBODIRECTION)->EnableWindow(TRUE);
	GetDlgItem(IDC_DATETIMEPICKERSTART)->EnableWindow(TRUE);
	GetDlgItem(IDC_DATETIMEPICKEREND)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTONSEARCH)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTONDISCONNECT)->EnableWindow(TRUE);

	GetDlgItem(IDC_BUTTONCONNECT)->EnableWindow(FALSE);
	GetDlgItem(IDC_IPADDRESSSERVER)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDITDBNAME)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDITNAME)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDITPASSWORD)->EnableWindow(FALSE);

	SetWindowText("������ѯ��� - ������" + m_strServerIP + "-" + m_strDBName);

	if (-1 == GetLocationInfoFromDB())
	{
		return -1;
	}

	if (-1 == GetCarTypeInfoFromDB())
	{
		return -1;
	}

	if (-1 == GetRoadNameFromDB())
	{
		return -1;
	}

	return 0;
}

int CHVE_SEARCHDlg::DisConnectDB(void)
{

	if(m_pConnection != NULL)
	{
		try
		{
			m_pConnection->Close();
			m_pConnection->Cancel();
			m_pConnection = NULL;
		}
		catch (_com_error e)
		{
			CString strerror;
			strerror.Format("���ݿ�Ͽ�ʧ��\n��Ϣ��\n%s\n%s", e.ErrorMessage(), (LPSTR)e.Description());
			AfxMessageBox(strerror);
		
			m_pConnection = NULL;
		}	
		
	}

	GetDlgItem(IDC_COMBOLOCATION)->EnableWindow(FALSE);
	GetDlgItem(IDC_COMBOCARTYPE)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDITCARNUM)->EnableWindow(FALSE);
	GetDlgItem(IDC_COMBOROADNAME)->EnableWindow(FALSE);
	GetDlgItem(IDC_COMBODIRECTION)->EnableWindow(FALSE);
	GetDlgItem(IDC_DATETIMEPICKERSTART)->EnableWindow(FALSE);
	GetDlgItem(IDC_DATETIMEPICKEREND)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTONSEARCH)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTONDISCONNECT)->EnableWindow(FALSE);

	GetDlgItem(IDC_BUTTONCONNECT)->EnableWindow(TRUE);
	GetDlgItem(IDC_IPADDRESSSERVER)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDITDBNAME)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDITNAME)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDITPASSWORD)->EnableWindow(TRUE);

	InitSearchCondition();

	SetWindowText("������ѯ��� - δ�������ݿ�");

	if (m_dlgResult.m_hWnd)
	{
		m_dlgResult.DestroyWindow();
	}

	return 0;
}


int CHVE_SEARCHDlg::CreateDBView(void)
{
	if (NULL == m_pConnection)
	{
		AfxMessageBox("���ݿ�δ����");
		return -1;
	}

	DWORD64 dwTmpStartTime = m_SearchStartTime.GetTime();
	DWORD64 dwTmpEndTime = m_SearchEndTime.GetTime();
	CString strTableList = "";
	if(m_SearchEndTime.GetYear() > m_SearchStartTime.GetYear()
		|| m_SearchEndTime.GetMonth() > m_SearchStartTime.GetMonth()
		|| m_SearchEndTime.GetDay() > m_SearchStartTime.GetDay())
	{
		while(dwTmpStartTime <= dwTmpEndTime+86400)
		{
			CTime cTmpCurrentTime(dwTmpStartTime);
			CString strTmpCommand = cTmpCurrentTime.Format("Hve_Data_%Y%m%d");
			if(IsTableExists(m_pConnection, strTmpCommand) == TRUE)
			{
				if(strTableList == "")
				{
					m_bRoadNameIsFind = IsRoadNameExists(m_pConnection, strTmpCommand);
					if (m_bRoadNameIsFind)
					{
						
						strTableList.Format("(SELECT Plate_No, Pass_Time, Location_Id, Road_no, Car_Speed, Obj_Id, \
											Road_Name, big_image_1_path, big_image_2_path FROM %s)", strTmpCommand);
					}
					else
					{
						AfxMessageBox("���ݱ���׮�ţ����޷�����׮�Ų�ѯ");
						strTableList.Format("(SELECT Plate_No, Pass_Time, Location_Id, Road_no, Car_Speed, Obj_Id, \
											big_image_1_path, big_image_2_path FROM %s)", strTmpCommand);

					}
					
				}
				else
				{
					if (m_bRoadNameIsFind)
					{
						strTableList.AppendFormat("union (SELECT Plate_No, Pass_Time, Location_Id, Road_no, Car_Speed, Obj_Id, \
												  Road_Name, big_image_1_path, big_image_2_path FROM %s)", strTmpCommand.GetBuffer());
					}
					else
					{
						strTableList.AppendFormat("union (SELECT Plate_No, Pass_Time, Location_Id, Road_no, Car_Speed, Obj_Id, \
												  big_image_1_path, big_image_2_path FROM %s)", strTmpCommand.GetBuffer());

					}
					
				}
			}
			dwTmpStartTime += 86400;
		}
	}
	else
	{
		CString strTmpCommand = m_SearchStartTime.Format("Hve_Data_%Y%m%d");
		if(IsTableExists(m_pConnection, strTmpCommand) == TRUE)
		{
			m_bRoadNameIsFind = IsRoadNameExists(m_pConnection, strTmpCommand);
			if (m_bRoadNameIsFind)
			{

				strTableList.Format("(SELECT Plate_No, Pass_Time, Location_Id, Road_no, Car_Speed, Obj_Id, \
									Road_Name, big_image_1_path, big_image_2_path FROM %s)", strTmpCommand);
			}
			else
			{
				AfxMessageBox("���ݱ���׮�ţ����޷�����׮�Ų�ѯ");
				strTableList.Format("(SELECT Plate_No, Pass_Time, Location_Id, Road_no, Car_Speed, Obj_Id, \
									big_image_1_path, big_image_2_path FROM %s)", strTmpCommand);

			}
		}
	}

	if(strTableList == "")
	{
		return 1;
	}

	try
	{
		CString strSql1;
		_variant_t RecordsetAffected;

		int i=0;
		while(IsTableExists(m_pConnection, m_strViewName))
		{
			m_strViewName.Format("HVE_SEARCH_SOFTWARE_VIEW%d", i);
			i++;
		}

		strSql1.Format("IF EXISTS (SELECT TABLE_NAME FROM INFORMATION_SCHEMA.VIEWS WHERE TABLE_NAME = N'%s') DROP VIEW %s",
			m_strViewName, m_strViewName);
		m_pConnection->Execute((_bstr_t)strSql1, &RecordsetAffected, adCmdText);


		strSql1.Format("CREATE VIEW %s AS %s", m_strViewName, strTableList);
		m_pConnection->Execute((_bstr_t)strSql1, &RecordsetAffected, adCmdText);
	}
	catch (_com_error e)
	{
		CString strerror;
		strerror.Format("������ͼʧ��\n��Ϣ��\n%s\n%s", e.ErrorMessage(), (LPSTR)e.Description());
		AfxMessageBox(strerror);
		return -1;
	}

	return 0;
}

int CHVE_SEARCHDlg::DropDBView(void)
{
	if (NULL == m_pConnection)
	{
		AfxMessageBox("���ݿ�δ����");
		return -1;
	}

	CString strSql1;
	_variant_t RecordsetAffected;

	strSql1.Format("IF EXISTS (SELECT TABLE_NAME FROM INFORMATION_SCHEMA.VIEWS WHERE TABLE_NAME = N'%s') DROP VIEW %s",
		m_strViewName, m_strViewName);

	try
	{
		m_pConnection->Execute((_bstr_t)strSql1, &RecordsetAffected, adCmdText);
	}
	catch (_com_error e)
	{
		CString strerror;
		strerror.Format("ɾ����ͼʧ��\n��Ϣ��\n%s\n%s", e.ErrorMessage(), (LPSTR)e.Description());
		AfxMessageBox(strerror);
		return -1;
		
	}

	return 0;
}

int CHVE_SEARCHDlg::QueryView(void)
{
	if (NULL == m_pConnection)
	{
		AfxMessageBox("���ݿ�δ����");
		return -1;
	}

	CString strStartTime = m_SearchStartTime.Format("%Y-%m-%d %H:%M:") + "00";
	CString strEndTime = m_SearchEndTime.Format("%Y-%m-%d %H:%M:") + "59";

	CString strCarNum = "";
	if(m_strSearchCarNum.GetLength() > 0)
	{
		strCarNum = " AND Plate_No LIKE '%";
		strCarNum += m_strSearchCarNum;
		strCarNum += "%' ";
	}

	CString strRoadName = "";
	if("����" != m_strSearchRoadName)
	{
		strRoadName.Format("AND Road_Name='%s' ", m_strSearchRoadName);
	}


	CString strCarType = "";
	if("ȫ������" != m_strSearchCarTypeInfo)
	{
		strCarType.Format("AND Obj_Id=%s ", CarTypeInfo2ID(m_strSearchCarTypeInfo));
	}

	CString strLocation = "";
	if("���ж���" != m_strSearchLocationInfo)
	{
		strLocation.Format("AND Location_Id=%s ", LocationInfo2ID(m_strSearchLocationInfo));
	}

	CString strDirection = "";
	if("����" == m_strDirection)
	{
		strDirection.Format("AND Road_no<0 ");
	}
	else if("����" == m_strDirection)
	{
		strDirection.Format("AND Road_no>0 ");
	}


	CString strSQL;

	if (m_bRoadNameIsFind)
	{
		strSQL.Format("SELECT Plate_No, Pass_Time, Location_Id, Road_no, Car_Speed, Obj_Id, \
					  Road_Name, big_image_1_path, big_image_2_path\
					  FROM %s WHERE Pass_Time BETWEEN '%s' AND '%s' %s%s%s%s%s ORDER BY %s.Pass_Time", 
					  m_strViewName, strStartTime, strEndTime, 
					  strCarNum, strRoadName, strCarType, strLocation, strDirection, m_strViewName);
	} 
	else
	{
		strSQL.Format("SELECT Plate_No, Pass_Time, Location_Id, Road_no, Car_Speed, Obj_Id, \
					  big_image_1_path, big_image_2_path\
					  FROM %s WHERE Pass_Time BETWEEN '%s' AND '%s' %s%s%s%s ORDER BY %s.Pass_Time", 
					  m_strViewName, strStartTime, strEndTime, 
					  strCarNum, strCarType, strLocation, strDirection, m_strViewName);
	}
	

	try
	{
		_RecordsetPtr pRecordset = NULL;
		HRESULT hr = pRecordset.CreateInstance("ADODB.Recordset");

		hr = pRecordset->Open(_variant_t(strSQL), m_pConnection.GetInterfacePtr(),
			adOpenDynamic, adLockOptimistic, adCmdText);

		_variant_t var;
		if(SUCCEEDED(hr))
		{
			if (pRecordset->adoEOF)
			{
				return 1;
			}

			int iDataCount = 0;

			while(!pRecordset->adoEOF)
			{
				CString strNum;
				strNum.Format("%d", iDataCount+1);
				m_dlgResult.m_listResult.InsertItem(iDataCount, strNum);

				int iSQLNum=0;
				for (int i=0; i<m_dlgResult.m_iColCount-1; i++)
				{
					var = pRecordset->GetCollect((long)iSQLNum);
					iSQLNum++;

					CString strVar;
					if (var.vt == VT_NULL)
					{
						strVar = "";
						continue;
					} 
					else
					{
						strVar = (LPCSTR)_bstr_t(var);
					}

					if (1 == i)
					{
						COleDateTime time = (COleDateTime)(var);
						strVar = time.Format("%Y-%m-%d %H:%M:%S");
					}

					if (2 == i)
					{
						m_dlgResult.m_listResult.SetItemText(iDataCount, i+1, LocationID2Info(strVar) );
						continue;
					}

					if (3 == i)
					{
						int iNum = StrToInt(strVar);
						CString strNum;
						strNum.Format(_T("%d"), abs(iNum));
						m_dlgResult.m_listResult.SetItemText(iDataCount, i+1, strNum);
						i++;

						if (iNum < 0)
						{
							m_dlgResult.m_listResult.SetItemText(iDataCount, i+1, _T("����"));
						}
						else if(iNum > 0)
						{
							m_dlgResult.m_listResult.SetItemText(iDataCount, i+1, _T("����"));
						}
						else
						{
							m_dlgResult.m_listResult.SetItemText(iDataCount, i+1, _T("δ֪"));
						}
						continue;

					}

					if ( 6 == i)
					{
						m_dlgResult.m_listResult.SetItemText(iDataCount, i+1, CarTypeID2Info(strVar) );

						if ( (!m_bRoadNameIsFind) && (11 == m_dlgResult.m_iColCount))
						{
							i++;
						}
						continue;
					}

					strVar.TrimRight();
					m_dlgResult.m_listResult.SetItemText(iDataCount, i+1, strVar);
				}

				iDataCount++;

				pRecordset->MoveNext();
			}

		}
	}
	catch (_com_error e)
	{
		CString strerror;
		strerror.Format("��ѯʧ��\n��Ϣ��\n%s\n%s", e.ErrorMessage(), (LPSTR)e.Description());
		AfxMessageBox(strerror);
		return -1;
		
	}



	return 0;
}

BOOL CHVE_SEARCHDlg::IsTableExists(_ConnectionPtr& pConnect, CString strTableName)
{
	if(pConnect == NULL) 
		return FALSE;

	CString strSql;
	HRESULT hr = E_FAIL;
	strSql.Format("SELECT name FROM sysobjects WHERE (name = '%s')", strTableName.GetBuffer());

	try
	{
		_RecordsetPtr pPtr;
		hr = pPtr.CreateInstance("ADODB.Recordset");

		if(hr != S_OK) 
			return FALSE;

		hr = pPtr->Open(_variant_t(strSql), pConnect.GetInterfacePtr(), 
			adOpenDynamic, adLockOptimistic, adCmdText);

		if(hr != S_OK) 
			return FALSE;

		if(pPtr->GetadoEOF())
		{
			pPtr->Close();
			pPtr.Release();
			return FALSE;
		}
		pPtr->Close();
		pPtr.Release();

	}
	catch (_com_error e)
	{
		CString strerror;
		strerror.Format("�жϱ����ʧ��\n��Ϣ��\n%s\n%s", e.ErrorMessage(), (LPSTR)e.Description());
		AfxMessageBox(strerror);
		return FALSE;
		
	}

	
	return TRUE;
}

int CHVE_SEARCHDlg::GetLocationInfoFromDB(void)
{
	if (NULL == m_pConnection)
	{
		AfxMessageBox("���ݿ�δ����");
		return -1;
	}

	_RecordsetPtr pRecordset = NULL;

	HRESULT hr = NULL;

	try
	{
		hr = pRecordset.CreateInstance("ADODB.Recordset");
		CString strSQL = "SELECT Location_Id, Location_Name FROM Hve_Location_Info ORDER BY Location_Id";	
		hr = pRecordset->Open(_variant_t(strSQL), m_pConnection.GetInterfacePtr(), 
			adOpenDynamic, adLockOptimistic, adCmdText);
		
	}
	catch (_com_error e)
	{
		CString strerror;
		strerror.Format("��ȡ������Ϣʧ��\n��Ϣ��\n%s\n%s", e.ErrorMessage(), (LPSTR)e.Description());
		AfxMessageBox(strerror);
		DisConnectDB();
		return -1;
		
	}

	try
	{
		_variant_t var;
		BOOL fIsAddNewLoca = FALSE;
		if(SUCCEEDED(hr))
		{
			m_mapLocation.clear();

			while(!pRecordset->adoEOF)
			{
				var = pRecordset->GetCollect((long)0);
				CString strLocaID = (LPCSTR)_bstr_t(var);

				var = pRecordset->GetCollect((long)1);
				CString strLocaName = (LPCSTR)_bstr_t(var);
				strLocaName.TrimRight();

				m_ComboLocation.InsertString(-1, strLocaName);

				m_mapLocation[strLocaID] = strLocaName;

				pRecordset->MoveNext();
			}
		}
	}
	catch (_com_error e)
	{
		CString strerror;
		strerror.Format("��ȡ������Ϣʧ��\n��Ϣ��\n%s\n%s", e.ErrorMessage(), (LPSTR)e.Description());
		AfxMessageBox(strerror);
		pRecordset->Close();
		DisConnectDB();
		return -1;
	}

	if(pRecordset)
	{
		pRecordset->Close();
		pRecordset = NULL;
	}

	return 0;
}

int CHVE_SEARCHDlg::GetCarTypeInfoFromDB(void)
{
	if (NULL == m_pConnection)
	{
		AfxMessageBox("���ݿ�δ����");
		return -1;
	}

	_RecordsetPtr pRecordset = NULL;

	HRESULT hr = NULL;
	
	try
	{
		hr = pRecordset.CreateInstance("ADODB.Recordset");
	}
	catch (_com_error e)
	{
		CString strerror;
		strerror.Format("��ȡ������Ϣʧ��\n��Ϣ��\n%s\n%s", e.ErrorMessage(), (LPSTR)e.Description());
		AfxMessageBox(strerror);
		DisConnectDB();
		return -1;
		
	}
	
	CString strSQL = "SELECT Obj_Id, Obj_Name FROM Hve_Obj_Type ORDER BY Obj_Id";
	_variant_t var;

	try
	{
		hr = pRecordset->Open(_variant_t(strSQL), m_pConnection.GetInterfacePtr(), 
			adOpenDynamic, adLockOptimistic, adCmdText);
	}
	catch (_com_error e)
	{
		CString strerror;
		strerror.Format("��ȡ������Ϣʧ��\n��Ϣ��\n%s\n%s", e.ErrorMessage(), (LPSTR)e.Description());
		AfxMessageBox(strerror);
		DisConnectDB();
		return -1;		
	}

	try
	{
		BOOL fIsAddNewLoca = FALSE;
		if(SUCCEEDED(hr))
		{
			m_mapCarType.clear();

			while(!pRecordset->adoEOF)
			{
				var = pRecordset->GetCollect((long)0);
				CString strCarTypeID = (LPCSTR)_bstr_t(var);

				var = pRecordset->GetCollect((long)1);
				CString strCarTypeName = (LPCSTR)_bstr_t(var);
				strCarTypeName.TrimRight();

				m_ComboCarType.InsertString(-1, strCarTypeName);

				m_mapCarType[strCarTypeID] = strCarTypeName;

				pRecordset->MoveNext();
			}
		}
	}
	catch (_com_error e)
	{
		CString strerror;
		strerror.Format("��ȡ������Ϣʧ��\n��Ϣ��\n%s\n%s", e.ErrorMessage(), (LPSTR)e.Description());
		AfxMessageBox(strerror);
		pRecordset->Close();
		DisConnectDB();
		return -1;	
		
	}


	if(pRecordset)
	{
		pRecordset->Close();
		pRecordset = NULL;
	}

	return 0;
}

void CHVE_SEARCHDlg::OnBnClickedButtondisconnect()
{
	// TODO: Add your control notification handler code here
	DisConnectDB();

}

int CHVE_SEARCHDlg::SearchCar(void)
{
	// create view
	switch(CreateDBView())
	{
	case 0:
		break;
	case 1:
		return 1;
	default:
		return -1;
	}

	// query view
	switch(QueryView())
	{
	case 0:
		break;
	case 1:
		DropDBView();
		return 1;
	default:
		DropDBView();
		return -1;
	}

	DropDBView();

	return 0;
}

UINT CHVE_SEARCHDlg::SearchThread(LPVOID pParam)
{
	CHVE_SEARCHDlg* pDlg = (CHVE_SEARCHDlg*) pParam;

	int iRet = pDlg->SearchCar();

	::PostMessage(pDlg->m_hWnd,WM_SEARCHEND, iRet, 0);
	return 0;
}

LRESULT CHVE_SEARCHDlg::OnSearchEnd(WPARAM wParam, LPARAM lParam)
{
	m_pShowLoading->Stop();

	switch (wParam)
	{
	case 0:
		m_dlgResult.CenterWindow(this);
		m_dlgResult.ShowWindow(SW_NORMAL);
		break;
	case 1:
		AfxMessageBox("δ��������ؼ�¼");
		break;
	default:
		break;
	}

	GetDlgItem(IDC_BUTTONSEARCH)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTONDISCONNECT)->EnableWindow(TRUE);

	m_bIsSearching = false;

	return 0;
}

CString CHVE_SEARCHDlg::LocationInfo2ID(CString strInfo)
{
	std::map<CString, CString>::iterator it = m_mapLocation.begin();
	
	while(it != m_mapLocation.end())
	{
		if (it->second == strInfo)
		{
			return it->first;
		}
		it++;
	}

	AfxMessageBox("�޷��ҵ���Ӧ��ֵ");

	return NULL;
}

CString CHVE_SEARCHDlg::LocationID2Info(CString strID)
{
	std::map<CString, CString>::iterator it = m_mapLocation.find(strID);

	if (it == m_mapLocation.end())
	{		
		return CString("����ID:" + strID + "�� �޷��ҵ���Ӧ�Ķ���");
	}

	CString strInfo = it->second;

	return strInfo.TrimRight();
}

CString CHVE_SEARCHDlg::CarTypeInfo2ID(CString strInfo)
{
	std::map<CString, CString>::iterator it = m_mapCarType.begin();

	while(it != m_mapCarType.end())
	{
		if (it->second == strInfo)
		{
			return it->first;
		}
		it++;
	}

	AfxMessageBox("�޷��ҵ���Ӧ��ֵ");

	return NULL;
}

CString CHVE_SEARCHDlg::CarTypeID2Info(CString strID)
{
	std::map<CString, CString>::iterator it = m_mapCarType.find(strID);

	if (it == m_mapCarType.end())
	{
		return CString("����ID:" + strID + "�� �޷��ҵ���Ӧ�ĳ���");
	}

	CString strInfo = it->second;
	return strInfo.TrimRight();
}

BOOL CHVE_SEARCHDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class

	// disable esc key
	if(pMsg->message==WM_KEYDOWN)
	{
		if(pMsg->wParam==VK_ESCAPE)
			return TRUE;
	}

	return CDialog::PreTranslateMessage(pMsg);
}

int CHVE_SEARCHDlg::GetRoadNameFromDB(void)
{
	if (NULL == m_pConnection)
	{
		AfxMessageBox("���ݿ�δ����");
		return -1;
	}

	_RecordsetPtr pRecordset = NULL;

	HRESULT hr = NULL;

	try
	{
		hr = pRecordset.CreateInstance("ADODB.Recordset");
	}
	catch (_com_error e)
	{
		CString strerror;
		strerror.Format("��¼������ʧ��\n��Ϣ��\n%s\n%s", e.ErrorMessage(), (LPSTR)e.Description());
		AfxMessageBox(strerror);
		DisConnectDB();
		return -1;

	}

	CString strSQL = "SELECT Road_Name FROM Hve_Road_Info GROUP BY Road_Name";
	_variant_t var;

	try
	{
		hr = pRecordset->Open(_variant_t(strSQL), m_pConnection.GetInterfacePtr(), 
			adOpenDynamic, adLockOptimistic, adCmdText);
	}
	catch (_com_error e)
	{
		AfxMessageBox("׮����Ϣ��ȡʧ�ܣ����޷���׮�Ž��в�ѯ");
		GetDlgItem(IDC_COMBOROADNAME)->EnableWindow(FALSE);
		m_dlgResult.m_bRoadNameIsExist = FALSE;
		return 1;		
	}

	m_dlgResult.m_bRoadNameIsExist = TRUE;

	try
	{
		BOOL fIsAddNewLoca = FALSE;
		if(SUCCEEDED(hr))
		{

			while(!pRecordset->adoEOF)
			{
				
				var = pRecordset->GetCollect((long)0);

				CString strRoadName;

				if (var.vt == VT_NULL)
				{
					strRoadName = "";
				}
				else
				{
					strRoadName = (LPCSTR)_bstr_t(var);
				}
				
				strRoadName.TrimRight();

				m_ComboRoadName.InsertString(-1, strRoadName);

				pRecordset->MoveNext();
			}
		}
	}
	catch (_com_error e)
	{
		CString strerror;
		strerror.Format("��ȡ׮��ʧ��\n��Ϣ��\n%s\n%s", e.ErrorMessage(), (LPSTR)e.Description());
		AfxMessageBox(strerror);
		pRecordset->Close();
		DisConnectDB();
		return -1;	

	}


	if(pRecordset)
	{
		pRecordset->Close();
		pRecordset = NULL;
	}

	return 0;
}

BOOL CHVE_SEARCHDlg::DeleteDirectory(CString strDirPath)
{
	CFileFind fileFind; 

	BOOL bFind = fileFind.FindFile(strDirPath + _T("\\*.*"));

	while(bFind)
	{
		bFind = fileFind.FindNextFile();

		if(fileFind.IsDots())
		{
			continue;
		}

		CString strFilePath = fileFind.GetFilePath();

		if (fileFind.IsDirectory())
		{
			if (FALSE == DeleteDirectory(strFilePath))
			{
				TRACE("DeleteDirectory %s error:%d\n", strFilePath, GetLastError());
				return FALSE;
			}
		} 
		else
		{
			if (FALSE == DeleteFile(strFilePath))
			{
				TRACE("DeleteFile %s error:%d\n", strFilePath, GetLastError());
				return FALSE;
			}
		}

	}

	fileFind.Close();

	if (FALSE == RemoveDirectory(strDirPath))
	{
		TRACE("RemoveDirectory %s error:%d\n", strDirPath, GetLastError());
	}

	return TRUE;
}

BOOL CHVE_SEARCHDlg::IsRoadNameExists(_ConnectionPtr& pConnect, CString strTableName)
{
	if(pConnect == NULL) 
		return FALSE;

	CString strSql;
	HRESULT hr = E_FAIL;
	strSql.Format("SELECT Name FROM SysColumns Where id=Object_Id(\'%s\') and Name='Road_Name'", strTableName.GetBuffer());

	try
	{
		_RecordsetPtr pPtr;
		hr = pPtr.CreateInstance("ADODB.Recordset");

		if(hr != S_OK) 
			return FALSE;

		hr = pPtr->Open(_variant_t(strSql), pConnect.GetInterfacePtr(), 
			adOpenDynamic, adLockOptimistic, adCmdText);

		if(hr != S_OK) 
			return FALSE;

		if(pPtr->GetadoEOF())
		{
			pPtr->Close();
			pPtr.Release();
			return FALSE;
		}
		pPtr->Close();
		pPtr.Release();

	}
	catch (_com_error e)
	{
		CString strerror;
		strerror.Format("�ж�׮�Ŵ���ʧ��\n��Ϣ��\n%s\n%s", e.ErrorMessage(), (LPSTR)e.Description());
		AfxMessageBox(strerror);
		return FALSE;

	}


	return TRUE;
}

