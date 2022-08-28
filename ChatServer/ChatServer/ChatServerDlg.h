
// ChatServerDlg.h : header file
//

#pragma once

#include "CConnectedSocket.h"
#include "CServerSocket.h"
#include "IMySocketListener.h"

// CChatServerDlg dialog
class CChatServerDlg : public CDialogEx, public IMySocketListener
{
// Construction
public:
	CChatServerDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CHATSERVER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CListCtrl ClientList;
	CListCtrl LogList;
	int logcount = 0;
	afx_msg void OnBnClickedButtonStartstop();
	void LockControl(int nID);
	void UnlockControl(int nID);
	CString nIP;
	CString nPort;
	int port = -1;

	CServerSocket mServerSocket;

	bool CheckIPandPort();

	virtual void UpdateClientList(CString nIP, CString nPort);
	virtual void UpdateLog(CString info);
	virtual void DeleteClient(CString nIP, CString nPort);
	virtual void SendMsg(CString username, CString message);
};
