
// ChatClient2Dlg.h : header file
//

#pragma once
#include "IMySocketListener.h"
#include "CConnectedSocket.h"

// CChatClient2Dlg dialog
class CChatClient2Dlg : public CDialogEx, public IMySocketListener
{
// Construction
public:
	CChatClient2Dlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CHATCLIENT2_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CString nIP;
	CString nPort;
	CString nickName;
	CString msg;
	CListBox msgLog;
	int port = -1;

	CConnectedSocket mClientSocket;
	virtual void OnRecept(CString message);
	virtual void Disconnect();

	void LockControl(int nID);
	void UnlockControl(int nID);
	afx_msg void OnBnClickedButtonConndisconn();
	bool CheckIPandPort();
	afx_msg void OnBnClickedButtonSend();
};
