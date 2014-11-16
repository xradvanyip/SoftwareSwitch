
// Switch.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "Switch.h"
#include "InitDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSwitchApp

BEGIN_MESSAGE_MAP(CSwitchApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CSwitchApp construction

CSwitchApp::CSwitchApp()
{
	// support Restart Manager
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CSwitchApp object

CSwitchApp theApp;


// CSwitchApp initialization

BOOL CSwitchApp::InitInstance()
{
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	AfxEnableControlContainer();

	// Create the shell manager, in case the dialog contains
	// any shell tree view or shell list view controls.
	CShellManager *pShellManager = new CShellManager;

	// Activate "Windows Native" visual manager for enabling themes in MFC controls
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	MACTab = new MACtable();
	Port1 = new SwitchPort(1);
	Port2 = new SwitchPort(2);
	
	CInitDlg init_dlg;
	INT_PTR nResponse = init_dlg.DoModal();
	if (nResponse == IDOK)
	{
		CSwitchDlg switch_dlg;
		m_pMainWnd = &switch_dlg;
		switch_dlg.DoModal();
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}
	else if (nResponse == -1)
	{
		TRACE(traceAppMsg, 0, "Warning: dialog creation failed, so application is terminating unexpectedly.\n");
		TRACE(traceAppMsg, 0, "Warning: if you are using MFC controls on the dialog, you cannot #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS.\n");
	}

	// Delete the shell manager created above.
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}



SwitchPort * CSwitchApp::GetPort1(void)
{
	return Port1;
}


SwitchPort * CSwitchApp::GetPort2(void)
{
	return Port2;
}


CSwitchDlg * CSwitchApp::GetSwitchDlg(void)
{
	return (CSwitchDlg *)m_pMainWnd;
}


MACtable * CSwitchApp::GetMACtab(void)
{
	return MACTab;
}


UINT CSwitchApp::ReceiveThread(void * pParam)
{
	SwitchPort *port = (SwitchPort *) pParam;
	Frame *buffer = port->GetBuffer();
	pcap_t *handle;
	char errbuf[PCAP_ERRBUF_SIZE];
	CStringA errorstring;
	int flag = PCAP_OPENFLAG_PROMISCUOUS | PCAP_OPENFLAG_NOCAPTURE_LOCAL | PCAP_OPENFLAG_MAX_RESPONSIVENESS;
	pcap_pkthdr *header = NULL;
	const u_char *frame = NULL;
	SendThreadParam p;
	int retval;
	
	handle = pcap_open(port->GetName(),65536,flag,1000,NULL,errbuf);
	if (!handle)
	{
		errorstring.Format("Unable to open the adapter on PORT %d!\n%s",port->GetIndex(),errbuf);
		theApp.GetSwitchDlg()->MessageBox(CString(errorstring),_T("Error"),MB_ICONERROR);
		return 0;
	}
	
	p.handle = handle;
	if (port->GetIndex() == 1) p.port = theApp.GetPort2();
	else p.port = theApp.GetPort1();
	AfxBeginThread(CSwitchApp::SendThread,&p);

	while ((retval = pcap_next_ex(handle,&header,&frame)) >= 0)
	{
		if (retval == 0) continue;
		buffer->AddFrame(header->len,frame);
	}
	if (retval == -1)
	{
		errorstring.Format("Error receiving the packets on PORT %d!\n%s",port->GetIndex(),pcap_geterr(handle));
		theApp.GetSwitchDlg()->MessageBox(CString(errorstring),_T("Error"),MB_ICONERROR);
	}

	return 0;
}


UINT CSwitchApp::SendThread(void * pParam)
{
	SendThreadParam *p = (SendThreadParam *) pParam;
	SwitchPort *port = p->port;
	MACtable *table = theApp.GetMACtab();
	Frame *buffer = port->GetBuffer();
	pcap_t *handle = p->handle;
	CStringA errorstring;
	int retval = 0;
	MACaddr src, dest, local = port->GetMACAddrStruct();
	
	while (TRUE)
	{
		buffer->GetFrame();
		src = buffer->GetSrcMAC();
		// if source MAC address is the local MAC address, the frame is ignored
		if (table->CompareMAC(src,local) == 0) continue;
		dest = buffer->GetDestMAC();
		// if source and destination MAC addresses are the same, the frame is ignored
		if (table->CompareMAC(src,dest) == 0) continue;
		// if source MAC address is broadcast address, the frame is ignored
		if (table->IsBroadcast(src)) continue;
		// search in MAC table
		retval = table->Find(port->GetIndex(),src,dest);
		// if destination MAC address is the local MAC address, the frame is ignored
		if ((retval == -1) && (table->CompareMAC(dest,local) == 0)) continue;
		// if destination MAC address is on same port, the frame is not sent out
		if (retval == port->GetIndex()) continue;

		retval = pcap_sendpacket(handle,buffer->GetData(),buffer->GetLength());
		if (retval != 0) break;
	}
	errorstring.Format("Error sending the packets on PORT %d!\n%s",port->GetIndex(),pcap_geterr(handle));
	theApp.GetSwitchDlg()->MessageBox(CString(errorstring),_T("Error"),MB_ICONERROR);

	return 0;
}


void CSwitchApp::StartThreads(void)
{
	AfxBeginThread(CSwitchApp::ReceiveThread,Port1);
	AfxBeginThread(CSwitchApp::ReceiveThread,Port2);
}
