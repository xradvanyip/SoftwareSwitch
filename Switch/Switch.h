
// Switch.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols
#include "SwitchDlg.h"
#include "SwitchPort.h"
#include "MACtable.h"
#include <pcap.h>
#include "Frame.h"


struct SendThreadParam {
	pcap_t *handle;
	SwitchPort *port;
};


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
	SwitchPort *Port1;
	SwitchPort *Port2;
	MACtable *MACTab;
public:
	static CRITICAL_SECTION m_cs;
	SwitchPort * GetPort1(void);
	SwitchPort * GetPort2(void);
	CSwitchDlg * GetSwitchDlg(void);
	MACtable * GetMACtab(void);
	static UINT ReceiveThread(void * pParam);
	static UINT SendThread(void * pParam);
	void StartThreads(void);
};

extern CSwitchApp theApp;