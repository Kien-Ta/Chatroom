#pragma once

#include "afx.h"

class IMySocketListener
{
public:
	virtual void UpdateClientList(CString nIP, CString nPort);
	virtual void UpdateLog(CString info);
	virtual void DeleteClient(CString nIP, CString nPort);
	virtual void SendMsg(CString username, CString msg);
};
