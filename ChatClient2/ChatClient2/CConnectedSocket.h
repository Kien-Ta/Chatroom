#pragma once

#include <afxsock.h>
#include "IMySocketListener.h"

class CConnectedSocket : public CSocket
{
public:
	CConnectedSocket();
	~CConnectedSocket();

	virtual void OnReceive(int nErrorCode);
	virtual void OnClose(int nErrorCode);
	void SetSocketListener(IMySocketListener* listener);

private:
	IMySocketListener* mSocketListener = NULL;
};