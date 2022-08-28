#include "pch.h"
#include "CConnectedSocket.h"

CConnectedSocket::CConnectedSocket() { }

CConnectedSocket::~CConnectedSocket() { }

void CConnectedSocket::OnReceive(int nErrorCode)
{
	Sleep(1000);
	CString message;
	char msg[100];

	memset(msg, 0, sizeof(msg));

	int nRead;
	
	nRead = Receive(msg, 100, 0);
	msg[nRead] = '\0';
	CString info = nIP + _T(" (port ") + nPort + _T(") has disconnected");

	switch (nRead)
	{
	case 0:
		ShutDown(2);
		Close();
		available = true;
		name = false;
		mSocketListener->UpdateLog(info);
		mSocketListener->DeleteClient(nIP, nPort);
		break;
	case SOCKET_ERROR:
		if (GetLastError() != WSAEWOULDBLOCK)
		{
			ShutDown(2);
			Close();
			available = true;
			name = false;
			mSocketListener->UpdateLog(info);
			mSocketListener->DeleteClient(nIP, nPort);
		}
		break;
	default:
		if (mSocketListener != 0)
		{
			if (name == false)
			{
				nickName = msg;
				name = true;
				break;
			}
			else
			{
				message = msg;
				mSocketListener->SendMsg(nickName, message);
				break;
			}
		}
	}
}

void CConnectedSocket::OnClose(int nErrorCode)
{
	CString info = nIP + _T(" (port ") + nPort + _T(") has disconnected");

	ShutDown(2);
	Close();
	available = true;
	name = false;
	mSocketListener->UpdateLog(info);
	mSocketListener->DeleteClient(nIP, nPort);

}

void CConnectedSocket::SetIPandPort(CString nIP, CString nPort)
{
	this->nIP = nIP;
	this->nPort = nPort;
}

void CConnectedSocket::SetSocketListener(IMySocketListener* listener)
{
	this->mSocketListener = listener;
}