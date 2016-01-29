// TestNewAutoLinkDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TestNewAutoLink.h"
#include "TestNewAutoLinkDlg.h"
#include ".\testnewautolinkdlg.h"


#define TE_TIME 1
#define TI_TIME 5000

#pragma comment(lib, "HvDevice.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


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




static int HVAPI_AL_OnLine_Notice(PVOID pUserData, HVAPI_OPERATE_HANDLE handle, char*szDeviceNo, char*szAddr,int iPort,char *szOtherXmlInfo)
{
	if ( pUserData== NULL )
		return 0;

	CTestNewAutoLinkDlg *pThis = (CTestNewAutoLinkDlg*)pUserData;
	return pThis->OnLineNotice(handle, szDeviceNo, szAddr,iPort,szOtherXmlInfo);

	return 0;
}


static int HVAPI_AL_OffLine_Notice(PVOID pUserData, HVAPI_OPERATE_HANDLE handle, char* szDeviceNo)
{
	if ( pUserData== NULL )
		return 0;

	CTestNewAutoLinkDlg *pThis = (CTestNewAutoLinkDlg*)pUserData;
	return pThis->OffLineNotice(handle, szDeviceNo);

	return 0;
}


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


// CTestNewAutoLinkDlg �Ի���



CTestNewAutoLinkDlg::CTestNewAutoLinkDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTestNewAutoLinkDlg::IDD, pParent)
	, m_uiTimeM(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}


CTestNewAutoLinkDlg::~CTestNewAutoLinkDlg()
{
	

	m_mtDevcieList.Lock();

	DWORD dwCount = m_DeviceArray.GetCount();

	for( int i=0; i<dwCount; i++)
	{
		POSITION p = m_DeviceArray.FindIndex(i);
		CDevice* pTemppDevice = m_DeviceArray.GetAt(p);

		delete  pTemppDevice;
		pTemppDevice = NULL;
	}

	m_mtDevcieList.Unlock();

	HVAPI_AL_UnInitActiveModule();

}


void CTestNewAutoLinkDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_DEVICELIST, m_deviceList);
	DDX_Text(pDX, IDC_EDIT_TIME, m_uiTimeM);
	DDX_Control(pDX, IDC_DATETIMEPICKER_STARTY, m_dtStartY);
	DDX_Control(pDX, IDC_DATETIMEPICKER_ENDY, m_dtEndY);
}

BEGIN_MESSAGE_MAP(CTestNewAutoLinkDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_STARTSERVER, OnBnClickedButtonStartserver)
	ON_BN_CLICKED(IDC_BUTTON_CLOSESERVER, OnBnClickedButtonCloseserver)
	ON_NOTIFY(NM_CLICK, IDC_LIST_DEVICELIST, OnNMClickListDevicelist)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_DEVICELIST, OnNMRclickListDevicelist)
	ON_COMMAND(ID_GETXML, OnGetxml)
	ON_COMMAND(ID_SYNCTIME, OnSynctime)
	ON_COMMAND(ID_GETDEVICETIME, OnGetdevicetime)
	ON_COMMAND(ID_GETTRAFICCINFO, OnGettraficcinfo)
	ON_COMMAND(ID_CAPTUREIMAGE, OnCaptureimage)
	ON_COMMAND(ID_GETVIDEO, OnGetvideo)
	ON_COMMAND(ID_STOPVIDEO, OnStopvideo)
	ON_COMMAND(ID_GETJPEG, OnGetjpeg)
	ON_COMMAND(ID_STIPJPEG, OnStipjpeg)
	ON_COMMAND(ID_UPLOADXML, OnUploadxml)
	ON_WM_TIMER()
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CTestNewAutoLinkDlg ��Ϣ�������

BOOL CTestNewAutoLinkDlg::OnInitDialog()
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


	GetDlgItem(IDC_EDIT_PORT)->SetWindowText("8888");
	InitDevListCtrl();

	/*
	SYSTEMTIME st;
    GetSystemTime(&st);
    SystemTimeToTzSpecificLocalTime(NULL, &st, &st);  //ʱ��ת�� */

	m_dtStartY.SetFormat("yyyy-MM-dd   HH:mm"); 
	m_dtEndY.SetFormat("yyyy-MM-dd   HH:mm");  


	CTime tm = CTime::GetCurrentTime();
	
	DWORD64 dwTime = tm.GetTime();
	
	int iTemp = 60 * 60;
	dwTime-= iTemp;

	CTime tStart(dwTime);

	int i = CDevice::GetDiskFree();




	m_dtStartY.SetTime(&tStart);
	m_dtEndY.SetTime(&tm);

	SetTimer(1, 5000, NULL);

	HVAPI_AL_InitActiveModule();
	HVAPI_AL_RegDevOnLineNotice(HVAPI_AL_OnLine_Notice, this);
	HVAPI_AL_RegDevOffLineNotice(HVAPI_AL_OffLine_Notice, this);

	return TRUE;  // ���������˿ؼ��Ľ��㣬���򷵻� TRUE
}


void CTestNewAutoLinkDlg::OnSysCommand(UINT nID, LPARAM lParam)
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


// ��ʼ���б��
HRESULT CTestNewAutoLinkDlg::InitDevListCtrl()
{
	m_deviceList.DeleteAllItems();

	LVCOLUMN lv;
	lv. mask = LVCF_TEXT | LVCF_FMT | LVCF_WIDTH ;
	lv. fmt = LVCFMT_CENTER ;
	lv.cx = 120;

	lv.pszText = "�豸���";
	lv.cchTextMax = sizeof(lv.pszText);
	lv.iSubItem = 0;
	lv.iOrder = 0;
	m_deviceList.InsertColumn(0, &lv);

	lv.pszText = "����IP";
	lv.cchTextMax = sizeof(lv.pszText);
	lv.iSubItem = 1;
	lv.iOrder =1;

	m_deviceList.InsertColumn(1, &lv);

 	lv.pszText = "�˿�";
 	lv.cchTextMax = sizeof(lv.pszText);
 	lv.iSubItem = 2;
 	lv.iOrder = 2;
 
 	m_deviceList.InsertColumn(2, &lv);

	lv.pszText = "״̬";
 	lv.cchTextMax = sizeof(lv.pszText);
 	lv.iSubItem = 3;
 	lv.iOrder = 3;
 
 	m_deviceList.InsertColumn(3, &lv);

	lv.pszText = "H264״̬";
 	lv.cchTextMax = sizeof(lv.pszText);
 	lv.iSubItem = 4;
 	lv.iOrder = 4;
 
 	m_deviceList.InsertColumn(4, &lv);

	lv.pszText = "JPEG״̬";
 	lv.cchTextMax = sizeof(lv.pszText);
 	lv.iSubItem = 5;
 	lv.iOrder = 5;
 
 	m_deviceList.InsertColumn(5, &lv);



	m_deviceList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	return 0;
}


// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CTestNewAutoLinkDlg::OnPaint() 
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
HCURSOR CTestNewAutoLinkDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CTestNewAutoLinkDlg::OnBnClickedButtonStartserver()
{
	// TODO: Add your control notification handler code here
	
	CString strText="";
	GetDlgItem(IDC_EDIT_PORT)->GetWindowText(strText);

	int iPort = atoi(strText);
	
	if ( HVAPI_AL_OpenDevCtrlServer(iPort, 50) == S_OK )
	{
		AfxMessageBox("�����������ɹ�");
		this->SetWindowText("������������");
	}
	else
	{
		AfxMessageBox("����������ʧ��");
		
	}
	
}


void CTestNewAutoLinkDlg::OnBnClickedButtonCloseserver()
{
	// TODO: Add your control notification handler code here

	if ( HVAPI_AL_CloseDevCtrlServer() == S_OK )
	{
		AfxMessageBox("�������رճɹ�");
		this->SetWindowText("�������ر�");
	}
	else
	{
		AfxMessageBox("�������ر�ʧ��");
	}


	/*
	m_mtDevcieList.Lock();
	DWORD dwCount = m_DeviceArray.GetCount();


	for( int i=0; i<dwCount; i++)
	{
		POSITION p = m_DeviceArray.FindIndex(i);
		CDevice*pDevice = m_DeviceArray.GetAt(p);
		if ( pDevice != NULL )
		{
			delete pDevice;
			pDevice = NULL;
		}

		
	}

	m_DeviceArray.RemoveAll();

	m_mtDevcieList.Unlock();

	m_deviceList.DeleteAllItems();*/
	
}


//�豸����
int CTestNewAutoLinkDlg::OnLineNotice(HVAPI_OPERATE_HANDLE handle, char*szDeviceNo, char*szAddr,int iPort,char *szOtherXmlInfo)
{
	//�����Ƿ��Ѿ��б�����豸��ŵĲ��������һ��Ϊ��

	CString str;
	str.Format("0x%0x", handle);

	CString strText="";
	GetDlgItem(IDC_EDIT_MSG)->GetWindowText(strText);

	if ( strText.GetLength() > 1024 * 5)
	{
		strText = "";
		
	}

	strText = strText +  "\r\n�豸����:" +  szDeviceNo + "_" +  szAddr+ "_" + str;
	GetDlgItem(IDC_EDIT_MSG)->SetWindowText(strText);


	CDevice*pDevice = NULL;

	m_mtDevcieList.Lock();

	DWORD dwCount = m_DeviceArray.GetCount();
	for( int i=0; i<dwCount; i++)
	{
		POSITION p = m_DeviceArray.FindIndex(i);
		CDevice* pTemppDevice = m_DeviceArray.GetAt(p);

		if ( strcmp(pTemppDevice->GetDeviceNo(), szDeviceNo) == 0  )
		{
			pDevice = pTemppDevice;
			break;
		}
	}

	m_mtDevcieList.Unlock();

	if ( pDevice != NULL )  //�Ѵ���Ϊ���豸��������Ķ��� ������
	{
		pDevice->SetDeviceAddr(szAddr, iPort);
		pDevice->SetDeviceInfo(szOtherXmlInfo);

		if ( pDevice->GetDeviceHandle() != handle)
		{
			HVAPI_AL_AddRef(handle);  //��Ӧ�ô˾���� �����ϲ����Ҫ�����ʹ�ô˾��
			pDevice->SetDeviceHandle(handle);
		}

	}
	else  //����Ҳ������������Ķ�����˵����û�����߹��� ����˵��ɾ�����ˣ� ���½���
	{

		pDevice = new CDevice() ;
		HVAPI_AL_AddRef(handle);  //��Ӧ�ô˾���� �����ϲ����Ҫ�����ʹ�ô˾��
		pDevice->SetDeviceInfo(szAddr, iPort, szDeviceNo,szOtherXmlInfo, handle);
		m_DeviceArray.AddTail(pDevice);
	}

	pDevice->SetDeviceStatus(CONNECTED);

	//ˢ��״̬�б��

	m_mtListCtrl.Lock();
	int index = -1;
	int iNetDiskCount = m_deviceList.GetItemCount();
	for (int i = 0; i<iNetDiskCount; i++)
	{
		CString strDevSN = m_deviceList.GetItemText(i, 0);
		if (0 == strcmp(szDeviceNo, strDevSN))
		{
			index = i;
			m_deviceList.SetItem(index, 1, LVIF_TEXT, szAddr, 0, 0, 0 ,0 );
			CString strTemp;
			strTemp.Format("%d", iPort);
			m_deviceList.SetItemText(index, 2, strTemp);
			//
			 strTemp = "����";
			m_deviceList.SetItemText(index, 3, strTemp);
			break;
		}
	}

	if ( index == -1 )  // �Ҳ����� ˵�����µ��豸�� ���뵽�б�����
	{
		iNetDiskCount = m_deviceList.GetItemCount();
		LVITEM  lvitem;

		int iItemRow = m_deviceList.InsertItem(0, szDeviceNo);
		m_deviceList.SetItem(iItemRow, 1, LVIF_TEXT, szAddr, 0, 0, 0 ,0 );
		CString strTemp;
		strTemp.Format("%d", iPort);
		m_deviceList.SetItemText(iItemRow, 2, strTemp);
		strTemp = "����";
		m_deviceList.SetItemText(iItemRow, 3, strTemp);
	}
	m_mtListCtrl.Unlock();


	return 0;
}

//�豸����֪ͨ����
int CTestNewAutoLinkDlg::OffLineNotice(HVAPI_OPERATE_HANDLE handle, char* szDeviceNo)
{
	CString strText="";
	GetDlgItem(IDC_EDIT_MSG)->GetWindowText(strText);
	if ( strText.GetLength() > 1024 * 5)
	{
		strText = "";
		
	}

	strText = strText + "\r\n�豸����" + szDeviceNo;



	GetDlgItem(IDC_EDIT_MSG)->SetWindowText(strText);

	CDevice*pDevice = NULL;

	m_mtDevcieList.Lock();

	DWORD dwCount = m_DeviceArray.GetCount();
	for( int i=0; i<dwCount; i++)
	{
		POSITION p = m_DeviceArray.FindIndex(i);
		CDevice* pTempDevice = m_DeviceArray.GetAt(p);
		
		if ( pTempDevice->GetDeviceHandle() ==  handle )
		{
			pDevice = pTempDevice;
			//���豸�����ѶϿ����������������� ���������������Ķ�����Ϊ�պ�״̬
			//pDevice->SetDeviceHandle(NULL);
			pDevice->SetDeviceStatus(DISCONNECT);  //����״̬Ϊ�Ͽ�
			break;
		}
	}

	m_mtDevcieList.Unlock();


	if ( pDevice == NULL )  //˵���Ҳ�����˾��������Ķ��������
	{
		HVAPI_AL_ReleaseRef(handle);   //ɾ���˾��
		return E_FAIL;  
	}


	m_mtListCtrl.Lock();

	int iNetDiskCount = m_deviceList.GetItemCount();

	for (int i = 0; i<iNetDiskCount; i++)
	{
		CString strDevSN = m_deviceList.GetItemText(i, 0);

		if (0 == strcmp(szDeviceNo, strDevSN))
		{
			CString strTemp = "�Ͽ�";
			m_deviceList.SetItemText(i, 3, strTemp);
			break;
		}
	}

	m_mtListCtrl.Unlock();


	return 0;
}


void CTestNewAutoLinkDlg::OnNMClickListDevicelist(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	*pResult = 0;

	//��ȡ�������ڵ��к�   
	//�ҳ����λ��   
	DWORD dwPos = GetMessagePos();  
	CPoint point( LOWORD(dwPos), HIWORD(dwPos) );  
	m_deviceList.ScreenToClient(&point);  
	  
	//����ṹ��   
	LVHITTESTINFO lvinfo;  
	lvinfo.pt = point;  
	  
	//��ȡ�к���Ϣ   
	int nItem = m_deviceList.HitTest(&lvinfo);  
	if(nItem != -1)  
		m_itemSel = lvinfo.iItem;   //��ǰ�к�  

}

void CTestNewAutoLinkDlg::OnNMRclickListDevicelist(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here

	NM_LISTVIEW *pNMListView = (NM_LISTVIEW*)pNMHDR;

	if (-1 != pNMListView->iItem)
	{
		CString strDevSN = m_deviceList.GetItemText(pNMListView->iItem , 0);

		int iSubMenuNum = 0;
		CMenu menu, *pSubMenu;
		menu.LoadMenu(IDR_MENU1);
		pSubMenu = menu.GetSubMenu(iSubMenuNum);

		POINT oPoint;
		GetCursorPos(&oPoint);
		pSubMenu->TrackPopupMenu(TPM_LEFTALIGN, oPoint.x, oPoint.y, this);
	}


	*pResult = 0;
}

CDevice* CTestNewAutoLinkDlg::GetSelDevice()
{
	CDevice*pDevice = NULL;
	if ( m_itemSel != -1)
	{
		CString strDevSN = m_deviceList.GetItemText( m_itemSel , 0);

		DWORD dwCount = m_DeviceArray.GetCount();
		
		for( int i=0; i<dwCount; i++)
		{
			POSITION p = m_DeviceArray.FindIndex(i);
			CDevice *pTemppDevice = m_DeviceArray.GetAt(p);
			
			if ( strcmp(pTemppDevice->GetDeviceNo(), strDevSN) == 0  )
			{
				pDevice  = pTemppDevice;
				break;
			}
		}//for

	}

	return pDevice;
}

void CTestNewAutoLinkDlg::OnGetxml()
{
	// TODO: Add your command handler code here

	CDevice*pDevice = GetSelDevice();
	if ( pDevice != NULL )
	{
		if ( pDevice->GetDeviceXml() )
		{
			AfxMessageBox("���سɹ�");
		}
		else
		{
			AfxMessageBox("����ʧ��");
		}
	}

}

void CTestNewAutoLinkDlg::OnSynctime()
{
	// TODO: Add your command handler code here

	CDevice*pDevice = GetSelDevice();
	if ( pDevice != NULL )
	{
		if ( pDevice->SyncTime() )
		{
			AfxMessageBox("ͬ��ʱ��ɹ�");
		}
		else
		{
			AfxMessageBox("ͬ��ʱ��ʧ��");
		}
	}

}


void CTestNewAutoLinkDlg::OnGetdevicetime()
{
	// TODO: Add your command handler code here

	CDevice*pDevice = GetSelDevice();
	if ( pDevice != NULL )
	{
		CString strTime = pDevice->GetDeviceTime();
		AfxMessageBox(strTime);
	}
}

void CTestNewAutoLinkDlg::OnGettraficcinfo()
{
	// TODO: Add your command handler code here

	CTime timeStart;
	m_dtStartY. GetTime(timeStart);

	CTime timeEnd;
	m_dtEndY.GetTime(timeEnd);

	if ( timeStart >=timeEnd )
	{
		AfxMessageBox("���õ�ʱ�䲻����");
		return ;
	}

	CDevice*pDevice = GetSelDevice();

	//m_dtStartY.
	
	
	

	if ( pDevice != NULL )
	{
		DWORD64 dwBeginTime = ((DWORD64)timeStart.GetTime()) * 1000;
		DWORD64 dwEndTime = ((DWORD64)timeEnd.GetTime()) * 1000;
		
		CTime tm = CTime::GetCurrentTime();
		

		//dwEndTime = ((DWORD64)tm.GetTime()) * 1000;
		//һ��Сʱ�ĺ���
		//dwBeginTime = dwEndTime - 60 * 60 * 1000 * 24  * 6;
		int iRetLen = 1024 * 1024;
		char *szRetInfo = new char[iRetLen];

		memset(szRetInfo, 0, iRetLen);

		
		if ( pDevice->GetHumanTrafficInfo(dwBeginTime, dwEndTime, szRetInfo,  &iRetLen) ) 
		{
			CString strFileName;
			CTime tm = CTime::GetCurrentTime();
			strFileName.Format("%s_info.txt", tm.Format("%Y%m%d%H%M%S"));

			CString strDir = GetCurrentDir()+"TrifficInfo\\";

			CString  *strTemp = new CString(szRetInfo); 

			strTemp->Replace(";", ";\r\n");

			if ( !PathFileExists(strDir) )
			{
				MakeSureDirectoryPathExists(strDir);
				//����в����ڣ��򱣴浽��Ӧ��Ŀ¼��
			}

			strFileName =  strDir + strFileName;
			FILE *myFile = fopen(strFileName, "w");
			
			if ( myFile )
			{
				int iLen = strTemp->GetLength();

				fwrite(strTemp->GetBuffer(), 1, iLen, myFile);
				fclose(myFile);
				myFile = NULL;
				strTemp->ReleaseBuffer();
			}

			delete strTemp;

			AfxMessageBox("��ȡ������ϢOK"+strFileName);
		}
		else
		{
			AfxMessageBox("��ȡ������Ϣʧ��");
		}

		delete[] szRetInfo;
		szRetInfo = NULL;

		//CString strTime = pDevice->GetDeviceTime(, );
		//AfxMessageBox(strTime);
	}

}

void CTestNewAutoLinkDlg::OnCaptureimage()
{
	// TODO: Add your command handler code here

	UpdateData(TRUE);

	CDevice*pDevice = GetSelDevice();
	if ( pDevice != NULL )
	{
		int iBuffLen = 1024 * 1024;
		int iImageLen = 0;
		BYTE *pTempBuff = new BYTE[iBuffLen];
		memset(pTempBuff, 0, iBuffLen);

		DWORD64 dwTime = 0; 
		DWORD dwWidth = 0;
		DWORD dwHeigh = 0;

		if ( pDevice->GetCaptureImage(m_uiTimeM, pTempBuff, iBuffLen, &iImageLen, &dwTime,&dwWidth, &dwHeigh ))
		{
			CString strFileName;
			CTime tm = CTime::GetCurrentTime();
			strFileName.Format("%s.jpeg", tm.Format("%Y%m%d%H%M%S"));

			CString strDir = GetCurrentDir()+"Capture\\";

			if ( !PathFileExists(strDir) )
			{
				MakeSureDirectoryPathExists(strDir);
				//����в����ڣ��򱣴浽��Ӧ��Ŀ¼��
			}

			strFileName =  strDir + strFileName;

			FILE *myFile =fopen(strFileName, "wb");
			if (myFile)
			{
				fwrite(pTempBuff, 1, iImageLen, myFile);
				fclose(myFile);
			}

			CTime tImgaeTime(dwTime/1000);
			CString strTemp;
			strTemp.Format("ImgeTime:%s, Width:%d, Heigh:%d", tImgaeTime.Format("%Y%m%d%H%M%S"),dwWidth, dwHeigh);

			AfxMessageBox(strTemp);
		}
		else
		{
			AfxMessageBox("��ȡͼƬʧ��");
		}

		delete[] pTempBuff;
		pTempBuff = NULL;
	}

}

void CTestNewAutoLinkDlg::OnGetvideo()
{
	// TODO: Add your command handler code here

	CDevice*pDevice = GetSelDevice();
	if ( pDevice != NULL )
	{
		

		if ( pDevice->StartVideo())
		{
			AfxMessageBox("���������ѷ���");
		}
		else
		{
			AfxMessageBox("���������ʧ��");
		}
	}


}

void CTestNewAutoLinkDlg::OnStopvideo()
{
	// TODO: Add your command handler code here

	CDevice*pDevice = GetSelDevice();
	if ( pDevice != NULL )
	{
		

		if ( pDevice->StopVideo())
		{
			AfxMessageBox("ֹͣ��Ƶ�����ѷ���");
		}
		else
		{
			AfxMessageBox("ֹͣ��Ƶ�����ʧ��");
		}
	}

}

void CTestNewAutoLinkDlg::OnGetjpeg()
{
	// TODO: Add your command handler code here
	CDevice*pDevice = GetSelDevice();
	if ( pDevice != NULL )
	{
		

		if ( pDevice->StartJpegVideo())
		{
			AfxMessageBox("����JPEG�����ѷ���");
		}
		else
		{
			AfxMessageBox("����JPEG�����ʧ��");
		}
	}

}

void CTestNewAutoLinkDlg::OnStipjpeg()
{
	// TODO: Add your command handler code here
	CDevice*pDevice = GetSelDevice();
	if ( pDevice != NULL )
	{
		

		if ( pDevice->StopJpegVideo())
		{
			AfxMessageBox("ֹͣJPEG��Ƶ�����ѷ���");
		}
		else
		{
			AfxMessageBox("ֹͣJPEG��Ƶ�����ʧ��");
		}
	}

}


//�ϴ������ļ�
void CTestNewAutoLinkDlg::OnUploadxml()
{
	// TODO: Add your command handler code here
	CString FilePathName;
    CFileDialog dlg(TRUE, //TRUEΪOPEN�Ի���FALSEΪSAVE AS�Ի���
        NULL, 
        NULL,
        OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
        (LPCTSTR)_TEXT("Xml Files (*.xml)|*.xml|All Files (*.*)|*.*||"),
        NULL);

    if(dlg.DoModal()==IDOK)
    {
        FilePathName=dlg.GetPathName(); //�ļ�����������FilePathName��
    }
    else
    {
         return;
    }



	CDevice*pDevice = GetSelDevice();
	if ( pDevice != NULL )
	{
		

		if ( pDevice->UpLoadDeviceXml(FilePathName))
		{
			AfxMessageBox("�ϴ��ļ��ɹ�, �豸����");
			pDevice->DeviceReset();
		}
		else
		{
			AfxMessageBox("�ϴ��ļ�ʧ��");
		}
	}

}

void CTestNewAutoLinkDlg::OnTimer(UINT nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	if ( nIDEvent == 1 )  //����H264 ״̬
	{
		//m_mtDevcieList.Lock();

		DWORD dwCount = m_DeviceArray.GetCount();
		for( int i=0; i<dwCount; i++)
		{
			POSITION p = m_DeviceArray.FindIndex(i);
			CDevice* pTemppDevice = m_DeviceArray.GetAt(p);

			char *szDeviceNo = pTemppDevice->GetDeviceNo();

			////
			//m_mtListCtrl.Lock();
			int iNetDiskCount = m_deviceList.GetItemCount();
			for (int j = 0; i<iNetDiskCount; j++)
			{
				CString strDevSN = m_deviceList.GetItemText(j, 0);

				if (0 == strcmp(szDeviceNo, strDevSN))
				{
					DWORD dwStatus  = pTemppDevice->GetDeviceH264Status();
					if ( dwStatus == AL_LINK_STATE_NORMAL)
						m_deviceList.SetItemText(j, 4, "����");
					else if (dwStatus == AL_LINK_STATE_RECONN )
						m_deviceList.SetItemText(j, 4, "����");
					else
						m_deviceList.SetItemText(j, 4, "�Ͽ�");

					dwStatus = pTemppDevice->GetDeviceJpegStatus();
					if ( dwStatus == AL_LINK_STATE_NORMAL)
						m_deviceList.SetItemText(j, 5, "����");
					else if (dwStatus == AL_LINK_STATE_RECONN )
						m_deviceList.SetItemText(j, 5, "����");
					else
						m_deviceList.SetItemText(j, 5, "�Ͽ�");

					
					break;
				}
			}
		//	m_mtListCtrl.Unlock();
			/////	
		}

	//m_mtDevcieList.Unlock();

	}

	CDialog::OnTimer(nIDEvent);
}

void CTestNewAutoLinkDlg::OnClose()
{
	// TODO: Add your message handler code here and/or call default

	KillTimer(1);
	

	CDialog::OnClose();
}
