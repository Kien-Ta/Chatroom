
// ChatClient2.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CChatClient2App:
// See ChatClient2.cpp for the implementation of this class
//

class CChatClient2App : public CWinApp
{
public:
	CChatClient2App();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CChatClient2App theApp;
