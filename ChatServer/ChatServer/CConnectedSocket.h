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
	bool available = true;
	void SetIPandPort(CString nIP, CString nPort);
	bool name = false;
	
	

private:
	IMySocketListener* mSocketListener = NULL;
	CString nIP, nPort;

	CString nickName;
};