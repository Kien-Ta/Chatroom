
// ChatClient2Dlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "ChatClient2.h"
#include "ChatClient2Dlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CChatClient2Dlg dialog



CChatClient2Dlg::CChatClient2Dlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CHATCLIENT2_DIALOG, pParent)
	, nIP(_T(""))
	, nPort(_T(""))
	, nickName(_T(""))
	, msg(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CChatClient2Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_IP, nIP);
	DDX_Text(pDX, IDC_EDIT_PORT, nPort);
	DDX_Text(pDX, IDC_EDIT_NICK, nickName);
	DDV_MaxChars(pDX, nickName, 9);
	DDX_Text(pDX, IDC_EDIT_MSG, msg);
	DDV_MaxChars(pDX, msg, 99);
	DDX_Control(pDX, IDC_LIST1, msgLog);
}

BEGIN_MESSAGE_MAP(CChatClient2Dlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_CONNDISCONN, &CChatClient2Dlg::OnBnClickedButtonConndisconn)
	ON_BN_CLICKED(IDC_BUTTON_SEND, &CChatClient2Dlg::OnBnClickedButtonSend)
END_MESSAGE_MAP()


// CChatClient2Dlg message handlers

BOOL CChatClient2Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	LockControl(IDC_EDIT_MSG);
	LockControl(IDC_BUTTON_SEND);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CChatClient2Dlg::OnPaint()
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
HCURSOR CChatClient2Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CChatClient2Dlg::LockControl(int nID)
{
	auto pLock = GetDlgItem(nID);
	pLock->EnableWindow(FALSE);
}

void CChatClient2Dlg::UnlockControl(int nID)
{
	auto pUnlock = GetDlgItem(nID);
	pUnlock->EnableWindow();
}

void CChatClient2Dlg::OnRecept(CString message)
{
	msgLog.AddString(message);
}

bool CChatClient2Dlg::CheckIPandPort()
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


void CChatClient2Dlg::OnBnClickedButtonConndisconn()
{
	CString temp;
	GetDlgItemText(IDC_BUTTON_CONNDISCONN, temp);
	if (temp == _T("Connect"))
	{
		UpdateData();
		if (nIP == _T("") || nPort == _T("") || nickName == _T(""))
		{
			MessageBox(_T("Please fill out the IP, Port and Nickname information"), _T("Error"), MB_OK);
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
		if (mClientSocket.Create() == FALSE)
		{
			MessageBox(_T("Socket create failed"), _T("Error"), MB_OK);
			return;
		}

		if (mClientSocket.Connect(nIP, port) == FALSE)
		{
			MessageBox(_T("Socket connect failed"), _T("Error"), MB_OK);
			mClientSocket.Close();
			return;
		}
		LockControl(IDC_EDIT_IP);
		LockControl(IDC_EDIT_NICK);
		LockControl(IDC_EDIT_PORT);
		UnlockControl(IDC_EDIT_MSG);
		UnlockControl(IDC_BUTTON_SEND);
		mClientSocket.SetSocketListener(this);
		msgLog.AddString(_T("Connect successful"));
		SetDlgItemText(IDC_BUTTON_CONNDISCONN, _T("Disconnect"));

		CT2A nick(nickName, CP_UTF8);

		int len = nickName.GetLength();
		
		mClientSocket.Send(nick, len, 0);
	}
	else
	{
		mClientSocket.ShutDown(2);
		mClientSocket.Close();
		msgLog.AddString(_T("Disconnected!"));
		UnlockControl(IDC_EDIT_IP);
		UnlockControl(IDC_EDIT_NICK);
		UnlockControl(IDC_EDIT_PORT);
		LockControl(IDC_EDIT_MSG);
		LockControl(IDC_BUTTON_SEND);
		msg = _T("");
		UpdateData(FALSE);
		SetDlgItemText(IDC_BUTTON_CONNDISCONN, _T("Connect"));
	}
}


void CChatClient2Dlg::OnBnClickedButtonSend()
{
	int len;

	UpdateData();
	if (msg == _T("")) return;
	CT2A message(msg, CP_UTF8);
	len = msg.GetLength();
	
	msg = _T("");
	UpdateData(FALSE);
	mClientSocket.Send(message, len, 0);
	
}

void CChatClient2Dlg::Disconnect()
{
	UnlockControl(IDC_EDIT_IP);
	UnlockControl(IDC_EDIT_NICK);
	UnlockControl(IDC_EDIT_PORT);
	LockControl(IDC_EDIT_MSG);
	LockControl(IDC_BUTTON_SEND);
	msg = _T("");
	UpdateData(FALSE);
	SetDlgItemText(IDC_BUTTON_CONNDISCONN, _T("Connect"));
}