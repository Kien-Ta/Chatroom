#pragma once

#include "CConnectedSocket.h"
#include "IMySocketListener.h"
#include <afxsock.h>


#define NUM_CLIENTS 5

class CServerSocket : public CSocket
{
public:
	CServerSocket();
	~CServerSocket();

	virtual void OnAccept(int nErrorCode);
	void SetAcceptListener(IMySocketListener* acceptListener);

	CConnectedSocket clients[NUM_CLIENTS];

private:
	IMySocketListener* mSocketListener = NULL;
};
