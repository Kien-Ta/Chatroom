#pragma once

#include <afx.h>

class IMySocketListener
{
public:
	virtual void OnRecept(CString message);
	virtual void Disconnect();
};
