
// Switch.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols
#include "SwitchPort.h"
#include <pcap.h>


// CSwitchApp:
// See Switch.cpp for the implementation of this class
//

class CSwitchApp : public CWinApp
{
public:
	CSwitchApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
private:
	SwitchPort Port1;
	SwitchPort Port2;
public:
	SwitchPort & GetPort1(void);
	SwitchPort & GetPort2(void);
};

extern CSwitchApp theApp;