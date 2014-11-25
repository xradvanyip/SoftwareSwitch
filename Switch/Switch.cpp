
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
	: f_eth2(NULL)
	, f_ip(NULL)
	, f_ports(NULL)
	, SwitchStats(NULL)
	, SwitchFilter(NULL)
{
	// support Restart Manager
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CSwitchApp object

CSwitchApp theApp;

CRITICAL_SECTION CSwitchApp::m_cs_mactable;
CRITICAL_SECTION CSwitchApp::m_cs_stats;
CRITICAL_SECTION CSwitchApp::m_cs_filter;
CRITICAL_SECTION CSwitchApp::m_cs_file_eth2;
CRITICAL_SECTION CSwitchApp::m_cs_file_ip;


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

	InitializeCriticalSection(&m_cs_mactable);
	InitializeCriticalSection(&m_cs_stats);
	InitializeCriticalSection(&m_cs_filter);
	InitializeCriticalSection(&m_cs_file_eth2);
	InitializeCriticalSection(&m_cs_file_ip);
	MACTab = new MACtable();
	Port1 = new SwitchPort(1);
	Port2 = new SwitchPort(2);
	SwitchStats = new Stats();
	SwitchFilter = new Filter();
	
	f_eth2 = fopen("ethernet2_protocols.txt","r");
	f_ip = fopen("ip_protocols.txt","r");
	f_ports = fopen("ports.txt","r");

	CreateEth2ProtocolList();
	CreateIPProtocolList();
	CreateTCPAppList();
	CreateUDPAppList();
	
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
		errorstring.Format("Unable to open the adapter on PORT %d!\r\n%s",port->GetIndex(),errbuf);
		theApp.GetSwitchDlg()->MessageBox(CString(errorstring),_T("Error"),MB_ICONERROR);
		return 0;
	}
	
	p.handle = handle;
	p.out_port = port;
	if (port->GetIndex() == 1) p.in_port = theApp.GetPort2();
	else p.in_port = theApp.GetPort1();
	AfxBeginThread(CSwitchApp::SendThread,&p);

	while ((retval = pcap_next_ex(handle,&header,&frame)) >= 0)
	{
		if (retval == 0) continue;
		buffer->AddFrame(header->len,frame);
	}
	if (retval == -1)
	{
		errorstring.Format("Error receiving the packets on PORT %d!\r\n%s",port->GetIndex(),pcap_geterr(handle));
		theApp.GetSwitchDlg()->MessageBox(CString(errorstring),_T("Error"),MB_ICONERROR);
	}

	return 0;
}


UINT CSwitchApp::SendThread(void * pParam)
{
	SendThreadParam *p = (SendThreadParam *) pParam;
	SwitchPort *in_port = p->in_port;
	SwitchPort *out_port = p->out_port;
	MACtable *table = theApp.GetMACtab();
	Frame *buffer = in_port->GetBuffer();
	Stats *stats = theApp.GetStatistics();
	Filter *filter = theApp.GetFilter();
	pcap_t *handle = p->handle;
	CStringA errorstring;
	int retval = 0;
	MACaddr src, dest, local = in_port->GetMACAddrStruct();
	
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

		EnterCriticalSection(&CSwitchApp::m_cs_mactable);
		// search in MAC table
		retval = table->Find(in_port->GetIndex(),src,dest);
		LeaveCriticalSection(&CSwitchApp::m_cs_mactable);

		// if destination MAC address is the local MAC address, the frame is ignored
		if ((retval == -1) && (table->CompareMAC(dest,local) == 0)) continue;
		// if destination MAC address is on same port, the frame is not sent out
		if (retval == in_port->GetIndex()) continue;
		
		EnterCriticalSection(&CSwitchApp::m_cs_filter);
		retval = filter->Check(in_port->GetIndex(),In,buffer);
		LeaveCriticalSection(&CSwitchApp::m_cs_filter);
		if (!retval) continue;
		
		EnterCriticalSection(&CSwitchApp::m_cs_stats);
		if (theApp.stats_enabled) stats->Add(in_port->GetIndex(),In,buffer);
		LeaveCriticalSection(&CSwitchApp::m_cs_stats);

		//...

		EnterCriticalSection(&CSwitchApp::m_cs_filter);
		retval = filter->Check(out_port->GetIndex(),Out,buffer);
		LeaveCriticalSection(&CSwitchApp::m_cs_filter);
		if (!retval) continue;

		EnterCriticalSection(&CSwitchApp::m_cs_stats);
		if (theApp.stats_enabled) stats->Add(out_port->GetIndex(),Out,buffer);
		LeaveCriticalSection(&CSwitchApp::m_cs_stats);

		retval = pcap_sendpacket(handle,buffer->GetData(),buffer->GetLength());
		if (retval != 0) break;
	}
	errorstring.Format("Error sending the packets on PORT %d!\r\n%s",out_port->GetIndex(),pcap_geterr(handle));
	theApp.GetSwitchDlg()->MessageBox(CString(errorstring),_T("Error"),MB_ICONERROR);

	return 0;
}


void CSwitchApp::StartThreads(void)
{
	AfxBeginThread(CSwitchApp::ReceiveThread,Port1);
	AfxBeginThread(CSwitchApp::ReceiveThread,Port2);
}


CString CSwitchApp::CheckTextFiles(void)
{
	CString error(_T("Can't open:"));

	if ((f_eth2) && (f_ip) && (f_ports)) return _T("");
	if (!f_eth2) error.AppendFormat(_T("\r\nethernet2_protocols.txt"));
	if (!f_ip) error.AppendFormat(_T("\r\nip_protocols.txt"));
	if (!f_ports) error.AppendFormat(_T("\r\nports.txt"));

	return error;
}


CString CSwitchApp::GetEth2ProtocolName(WORD type)
{
	char tmp[100], scanstr[50];
	char namestr[50], numstr[10];
	int found = 0;
	CStringA name;

	sprintf(scanstr,"%%[^\t]s%.4X",type);
	sprintf(numstr,"%.4X",type);
	EnterCriticalSection(&CSwitchApp::m_cs_file_eth2);
	while (fgets(tmp,100,f_eth2) != NULL) if ((strstr(tmp,numstr) != NULL) && (sscanf(tmp,scanstr,namestr) > 0)) {
		found = 1;
		break;
	}
	rewind(f_eth2);
	LeaveCriticalSection(&CSwitchApp::m_cs_file_eth2);
	if (found) name.Format("%s",namestr);
	else name.Format("0x%X",type);
	
	return CString(name);
}


WORD CSwitchApp::GetEth2ProtocolNum(CStringA Name)
{
	WORD num;
	char tmp[100], scanstr[50];
	
	sprintf(scanstr,"%s\t%%hX",Name);
	EnterCriticalSection(&CSwitchApp::m_cs_file_eth2);
	while (fgets(tmp,100,f_eth2) != NULL) if ((strstr(tmp,Name) != NULL) && (sscanf(tmp,scanstr,&num) > 0)) break;
	rewind(f_eth2);
	LeaveCriticalSection(&CSwitchApp::m_cs_file_eth2);
	
	return num;
}


void CSwitchApp::CreateEth2ProtocolList(void)
{
	char tmp[100], namestr[50];

	EnterCriticalSection(&CSwitchApp::m_cs_file_eth2);
	while (fgets(tmp,100,f_eth2) != NULL) if (sscanf(tmp,"%[^\t]s",namestr) > 0) Eth2ProtocolList.Add(CStringA(namestr));
	rewind(f_eth2);
	LeaveCriticalSection(&CSwitchApp::m_cs_file_eth2);
}


int CSwitchApp::FindInEth2ProtocolList(WORD key)
{
	int i;

	for (i=0;i < Eth2ProtocolList.GetCount();i++) if (Eth2ProtocolList[i].Compare(CStringA(GetEth2ProtocolName(key))) == 0) return i;
	return -1;
}


CArray<CStringA> & CSwitchApp::GetEth2ProtocolList(void)
{
	return Eth2ProtocolList;
}


CString CSwitchApp::GetIPProtocolName(BYTE type)
{
	char tmp[100], scanstr[50];
	char namestr[50], numstr[10];
	int found = 0;
	CStringA name;

	sprintf(scanstr,"%%[^\t]s%u",type);
	sprintf(numstr,"%u",type);
	EnterCriticalSection(&CSwitchApp::m_cs_file_ip);
	while (fgets(tmp,100,f_ip) != NULL) if ((strstr(tmp,numstr) != NULL) && (sscanf(tmp,scanstr,namestr) > 0)) {
		found  = 1;
		break;
	}
	rewind(f_ip);
	LeaveCriticalSection(&CSwitchApp::m_cs_file_ip);
	if (found) name.Format("%s",namestr);
	else name.Format("%u",type);
	
	return CString(name);
}


BYTE CSwitchApp::GetIPProtocolNum(CStringA Name)
{
	WORD num;
	char tmp[100], scanstr[50];
	
	sprintf(scanstr,"%s\t%%hu",Name);
	EnterCriticalSection(&CSwitchApp::m_cs_file_ip);
	while (fgets(tmp,100,f_ip) != NULL) if ((strstr(tmp,Name) != NULL) && (sscanf(tmp,scanstr,&num) > 0)) break;
	rewind(f_ip);
	LeaveCriticalSection(&CSwitchApp::m_cs_file_ip);
	
	return num;
}


void CSwitchApp::CreateIPProtocolList(void)
{
	char tmp[100], namestr[50];

	EnterCriticalSection(&CSwitchApp::m_cs_file_ip);
	while (fgets(tmp,100,f_ip) != NULL) if (sscanf(tmp,"%[^\t]s",namestr) > 0) IPProtocolList.Add(CStringA(namestr));
	rewind(f_ip);
	LeaveCriticalSection(&CSwitchApp::m_cs_file_ip);
}


int CSwitchApp::FindInIPProtocolList(BYTE key)
{
	int i;

	for (i=0;i < IPProtocolList.GetCount();i++) if (IPProtocolList[i].Compare(CStringA(GetIPProtocolName(key))) == 0) return i;
	return -1;
}


CArray<CStringA> & CSwitchApp::GetIPProtocolList(void)
{
	return IPProtocolList;
}


WORD CSwitchApp::GetPortNumber(CStringA AppName)
{
	WORD num = 0;
	char tmp[100], scanstr[50];
	
	sprintf(scanstr,"%s\t%%*3c\t%%hu",AppName);
	while (fgets(tmp,100,f_ports) != NULL) if ((strstr(tmp,AppName) != NULL) && (sscanf(tmp,scanstr,&num) > 0)) break;
	rewind(f_ports);
	
	return num;
}


CString CSwitchApp::GetAppName(WORD port, int isExtended)
{
	char tmp[100], scanstr[50];
	char namestr[50], numstr[10];
	int found = 0;
	CStringA name;

	sprintf(scanstr,"%%[^\t]s%%*3c\t%u",port);
	sprintf(numstr,"%u",port);
	while (fgets(tmp,100,f_ports) != NULL) if ((strstr(tmp,numstr) != NULL) && (sscanf(tmp,scanstr,namestr) > 0)) {
		found = 1;
		break;
	}
	rewind(f_ports);
	if ((isExtended) && (found)) name.Format("%s (%u)",namestr,port);
	else if (found) name.Format("%s",namestr);
	else name.Format("%u",port);
	
	return CString(name);
}


void CSwitchApp::CreateTCPAppList(void)
{
	char tmp[100], namestr[50];

	while (fgets(tmp,100,f_ports) != NULL) if ((strstr(tmp,"TCP") != NULL) && (sscanf(tmp,"%[^\t]s",namestr) > 0)) TCPAppList.Add(CStringA(namestr));
	rewind(f_ports);
}


int CSwitchApp::FindInTCPAppList(WORD port)
{
	int i;

	for (i=0;i < TCPAppList.GetCount();i++) if (TCPAppList[i].Compare(CStringA(GetAppName(port))) == 0) return i;
	return -1;
}


void CSwitchApp::CreateUDPAppList(void)
{
	char tmp[100], namestr[50];

	while (fgets(tmp,100,f_ports) != NULL) if ((strstr(tmp,"UDP") != NULL) && (sscanf(tmp,"%[^\t]s",namestr) > 0)) UDPAppList.Add(CStringA(namestr));
	rewind(f_ports);
}


int CSwitchApp::FindInUDPAppList(WORD port)
{
	int i;

	for (i=0;i < UDPAppList.GetCount();i++) if (UDPAppList[i].Compare(CStringA(GetAppName(port))) == 0) return i;
	return -1;
}


CArray<CStringA> & CSwitchApp::GetTCPAppList(void)
{
	return TCPAppList;
}


CArray<CStringA> & CSwitchApp::GetUDPAppList(void)
{
	return UDPAppList;
}


Stats * CSwitchApp::GetStatistics(void)
{
	return SwitchStats;
}


Filter * CSwitchApp::GetFilter(void)
{
	return SwitchFilter;
}
