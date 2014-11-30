
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
#include "Stats.h"
#include "Filter.h"
#include "VLANList.h"


struct SendThreadParam {
	pcap_t *handle;
	SwitchPort *in_port, *out_port;
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
	Stats *SwitchStats;
	Filter *SwitchFilter;
	VLANList *SwitchVLANList;
	FILE *f_eth2;
	FILE *f_ip;
	FILE *f_ports;
	CArray<CStringA> Eth2ProtocolList;
	CArray<CStringA> IPProtocolList;
	CArray<CStringA> TCPAppList;
	CArray<CStringA> UDPAppList;
public:
	static CRITICAL_SECTION m_cs_mactable;
	static CRITICAL_SECTION m_cs_stats;
	static CRITICAL_SECTION m_cs_filter;
	static CRITICAL_SECTION m_cs_vlan;
	static CRITICAL_SECTION m_cs_file_eth2;
	static CRITICAL_SECTION m_cs_file_ip;
	BOOL stats_enabled;
	SwitchPort * GetPort1(void);
	SwitchPort * GetPort2(void);
	CSwitchDlg * GetSwitchDlg(void);
	MACtable * GetMACtab(void);
	static UINT ReceiveThread(void * pParam);
	static UINT SendThread(void * pParam);
	void StartThreads(void);
	CString CheckTextFiles(void);
	CString GetEth2ProtocolName(WORD type);
	WORD GetEth2ProtocolNum(CStringA Name);
	void CreateEth2ProtocolList(void);
	int FindInEth2ProtocolList(WORD key);
	CArray<CStringA> & GetEth2ProtocolList(void);
	CString GetIPProtocolName(BYTE type);
	BYTE GetIPProtocolNum(CStringA Name);
	void CreateIPProtocolList(void);
	int FindInIPProtocolList(BYTE key);
	CArray<CStringA> & GetIPProtocolList(void);
	WORD GetPortNumber(CStringA AppName);
	CString GetAppName(WORD port, int isExtended = 0);
	void CreateTCPAppList(void);
	int FindInTCPAppList(WORD port);
	void CreateUDPAppList(void);
	int FindInUDPAppList(WORD port);
	CArray<CStringA> & GetTCPAppList(void);
	CArray<CStringA> & GetUDPAppList(void);
	Stats * GetStatistics(void);
	Filter * GetFilter(void);
	VLANList * GetVLANlist(void);
};

extern CSwitchApp theApp;