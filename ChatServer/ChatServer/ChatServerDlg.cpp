
// ChatServerDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "ChatServer.h"
#include "ChatServerDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CChatServerDlg dialog



CChatServerDlg::CChatServerDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CHATSERVER_DIALOG, pParent)
	, nIP(_T(""))
	, nPort(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CChatServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, ClientList);
	DDX_Control(pDX, IDC_LIST2, LogList);
	DDX_Text(pDX, IDC_EDIT_IP, nIP);
	DDX_Text(pDX, IDC_EDIT_PORT, nPort);
}

BEGIN_MESSAGE_MAP(CChatServerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_STARTSTOP, &CChatServerDlg::OnBnClickedButtonStartstop)
END_MESSAGE_MAP()


// CChatServerDlg message handlers

BOOL CChatServerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	ClientList.InsertColumn(0, _T("IP"), LVCFMT_LEFT, 250);
	ClientList.InsertColumn(1, _T("Port"), LVCFMT_LEFT, 150);

	LogList.InsertColumn(0, _T("Time"), LVCFMT_LEFT, 280);
	LogList.InsertColumn(1, _T("Detail"), LVCFMT_LEFT, 380);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CChatServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CChatServerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CChatServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CChatServerDlg::LockControl(int ID)
{
	auto pLock = GetDlgItem(ID);
	pLock->EnableWindow(FALSE);
}

void CChatServerDlg::UnlockControl(int ID)
{
	auto pUnlock = GetDlgItem(ID);
	pUnlock->EnableWindow();
}

bool CChatServerDlg::CheckIPandPort()
{
	CString strCheck(nIP);
	int nPointNum = 0;

	nPointNum = strCheck.Remove('.');

	if (nPointNum != 3)
	{
		return false;
	}
	if (strCheck.GetLength() > 12)
	{
		return false;
	}

	strCheck = nIP;
	while (strCheck.Find(_T(".")) >= 0)
	{

		int nLoc = strCheck.Find(_T("."));
		if (strCheck.Left(nLoc).GetLength() == 0)
		{
			return false;
		}
		int nVal = _ttoi(strCheck.Left(nLoc));
		if (nVal == 0 && strCheck.Left(nLoc) != _T("0"))
		{
			return false;
		}
		strCheck = strCheck.Right(strCheck.GetLength() - (nLoc + 1));

		if (nVal < 0 || nVal > 255 || strCheck.IsEmpty())
		{
			return false;
		}
	}
	if (_ttoi(strCheck) == 0 && strCheck != _T("0"))
	{
		return false;
	}
	if (_ttoi(strCheck) < 0 || _ttoi(strCheck) > 255)
	{
		return false;
	}

	port = _ttoi(nPort);
	if (port < 0 || port > 65535)
	{
		return false;
	}

	return true;
}

void CChatServerDlg::OnBnClickedButtonStartstop()
{
	CString temp;
	GetDlgItemText(IDC_BUTTON_STARTSTOP, temp);
	if (temp == _T("Start"))
	{
		UpdateData();
		if (nIP == _T("") || nPort == _T(""))
		{
			MessageBox(_T("Please fill out the IP and Port and information"), _T("Error"), MB_OK);
			return;
		}
		if (!CheckIPandPort())
		{
			MessageBox(_T("Please check IP address and port"), _T("Error"), MB_OK);
			return;
		}

		if (AfxSocketInit() == FALSE)
		{
			MessageBox(_T("Init socket failed"), _T("Error"), MB_OK);
			return;
		}
		if (mServerSocket.Create(port, SOCK_STREAM, nIP) == FALSE)
		{
			MessageBox(_T("Socket create failed"), _T("Error"), MB_OK);
			return;
		}
		if (mServerSocket.Listen() == FALSE)
		{
			MessageBox(_T("Socket listen failed"), _T("Error"), MB_OK);
			mServerSocket.Close();
			return;
		}
		mServerSocket.SetAcceptListener(this);
		for (int i = 0; i < NUM_CLIENTS; i++)
		{
			mServerSocket.clients[i].SetSocketListener(this);
		}

		CMutex lock;
		lock.Lock();
		CTime t = CTime::GetCurrentTime();
		CString t_str = t.Format(_T("%A, %B %d, %Y, %r"));
		int nItem = LogList.InsertItem(logcount, t_str);
		LogList.SetItemText(nItem, 1, _T("Server starts"));
		logcount++;
		lock.Unlock();

		SetDlgItemText(IDC_BUTTON_STARTSTOP, _T("Stop"));
		LockControl(IDC_EDIT_IP);
		LockControl(IDC_EDIT_PORT);
	}
	else
	{
		mServerSocket.ShutDown(2);
		mServerSocket.Close();
		CMutex lock;
		lock.Lock();
		for (int i = 0; i < NUM_CLIENTS; i++)
		{
			mServerSocket.clients[i].ShutDown(2);
			mServerSocket.clients[i].Close();
			mServerSocket.clients[i].available = true;
			mServerSocket.clients[i].name = false;
		}
		CTime t = CTime::GetCurrentTime();
		CString t_str = t.Format(_T("%A, %B %d, %Y, %r"));
		int nItem = LogList.InsertItem(logcount, t_str);
		LogList.SetItemText(nItem, 1, _T("Server stops. Disconnect all clients"));
		logcount++;
		ClientList.DeleteAllItems();

		lock.Unlock();

		SetDlgItemText(IDC_BUTTON_STARTSTOP, _T("Start"));
		UnlockControl(IDC_EDIT_IP);
		UnlockControl(IDC_EDIT_PORT);
	}
}

void CChatServerDlg::UpdateClientList(CString nIP, CString nPort)
{
	CMutex lock;
	lock.Lock();

	int nItem = ClientList.InsertItem(0, nIP);
	ClientList.SetItemText(nItem, 1, nPort);

	lock.Unlock();
}

void CChatServerDlg::UpdateLog(CString info)
{
	CMutex lock;
	lock.Lock();

	CTime t = CTime::GetCurrentTime();
	CString t_str = t.Format(_T("%A, %B %d, %Y, %r"));
	int nItem = LogList.InsertItem(logcount, t_str);
	LogList.SetItemText(nItem, 1, info);
	logcount++;

	lock.Unlock();
}

void CChatServerDlg::DeleteClient(CString nIP, CString nPort)
{
	CMutex lock;
	lock.Lock();

	CString port;
	int nIndex;
	LVFINDINFO info;
	info.flags = LVFI_STRING;
	info.psz = nIP;

	while ((nIndex = ClientList.FindItem(&info)) != -1)
	{
		port = ClientList.GetItemText(nIndex, 1);
		if (port == nPort)
		{
			ClientList.DeleteItem(nIndex);
		}
	}
	
	lock.Unlock();
}

void CChatServerDlg::SendMsg(CString username, CString message)
{	
	CString msg = username + _T(": ") + message;
	CT2A send_msg(msg, CP_UTF8);
	int len = msg.GetLength();

	CMutex lock;
	lock.Lock();

	for (int i = 0; i < NUM_CLIENTS; i++)
	{
		if (mServerSocket.clients[i].available == false)
		{
			mServerSocket.clients[i].Send(send_msg, len, 0);
		}
	}
}