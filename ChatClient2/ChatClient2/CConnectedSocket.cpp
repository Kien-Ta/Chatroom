#include "pch.h"
#include "CConnectedSocket.h"

CConnectedSocket::CConnectedSocket() { }

CConnectedSocket::~CConnectedSocket() { }

void CConnectedSocket::OnReceive(int nErrorCode)
{
	CString message;

	char msg[150];
	memset(msg, 0, sizeof(msg));

	int nRead;

	nRead = Receive(msg, 150, 0);
	msg[nRead] = '\0';

	switch (nRead)
	{
	case 0:
		ShutDown(2);
		Close();
		mSocketListener->OnRecept(_T("Disconnected"));
		break;
	case SOCKET_ERROR:
		if (GetLastError() != WSAEWOULDBLOCK)
		{
			ShutDown(2);
			Close();
			mSocketListener->OnRecept(_T("Disconnected"));
		}
		break;
	default:
		if (mSocketListener != 0)
		{
			message = msg;
			mSocketListener->OnRecept(message);
		}
	}
}

void CConnectedSocket::OnClose(int nErrorCode)
{
	ShutDown(2);
	Close();
	mSocketListener->OnRecept(_T("Disconnected!"));
	mSocketListener->Disconnect();
}

void CConnectedSocket::SetSocketListener(IMySocketListener* listener)
{
	this->mSocketListener = listener;
}