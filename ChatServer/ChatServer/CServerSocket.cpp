#include "pch.h"
#include "CServerSocket.h"

CServerSocket::CServerSocket() { }

CServerSocket::~CServerSocket() { }

void CServerSocket::OnAccept(int nErrorCode)
{
	int choose;
	CMutex lock;
	lock.Lock();
	for (int i = 0; i < 5; i++)
	{
		if (clients[i].available == true)
		{
			choose = i;
			break;
		}
	}

	Accept(clients[choose]);
	clients[choose].available = false;
	
	lock.Unlock();

	CString nIP;
	UINT port;
	clients[choose].GetPeerName(nIP, port);
	CString nPort;
	nPort.Format(_T("%u"), port);
	CString info = nIP + _T(" connected on port ") + nPort;
	clients[choose].SetIPandPort(nIP, nPort);
	if (mSocketListener != 0)
	{
		mSocketListener->UpdateClientList(nIP, nPort);
		mSocketListener->UpdateLog(info);
	}
}

void CServerSocket::SetAcceptListener(IMySocketListener* acceptListener)
{
	this->mSocketListener = acceptListener;
}