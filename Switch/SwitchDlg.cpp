
// SwitchDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Switch.h"
#include "SwitchDlg.h"
#include "afxdialogex.h"
#include "RuleDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSwitchDlg dialog



CSwitchDlg::CSwitchDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSwitchDlg::IDD, pParent)
	, AllowCheckBoxes(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSwitchDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PORT1DESC, m_port1desc);
	DDX_Control(pDX, IDC_PORT1MACADDR, m_port1macaddr);
	DDX_Control(pDX, IDC_PORT1MODECOMBO, m_port1mode);
	DDX_Control(pDX, IDC_PORT1VLANCOMBO, m_port1accessvlan);
	DDX_Control(pDX, IDC_PORT1VLANSBUTTON, m_port1vlansbutton);
	DDX_Control(pDX, IDC_PORT2DESC, m_port2desc);
	DDX_Control(pDX, IDC_PORT2MACADDR, m_port2macaddr);
	DDX_Control(pDX, IDC_PORT2MODECOMBO, m_port2mode);
	DDX_Control(pDX, IDC_PORT2VLANCOMBO, m_port2accessvlan);
	DDX_Control(pDX, IDC_PORT2VLANSBUTTON, m_port2vlansbutton);
	DDX_Control(pDX, IDC_LIST1, m_mactable);
	DDX_Control(pDX, IDC_TIMEOUT, m_timeout);
	DDX_Control(pDX, IDC_TIMEOUTSPIN, m_timeoutspin);
	DDX_Control(pDX, IDC_STATSLIST, m_stats);
	DDX_Control(pDX, IDC_STATSCHECK, m_statscheckbox);
	DDX_Control(pDX, IDC_RULESLIST, m_rules);
	DDX_Control(pDX, IDC_VLANLIST, m_vlanlist);
	DDX_Control(pDX, IDC_VLANIDSPIN, m_VIDspin);
}

BEGIN_MESSAGE_MAP(CSwitchDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_CBN_SELCHANGE(IDC_PORT1MODECOMBO, &CSwitchDlg::OnPort1ModeChange)
	ON_CBN_SELCHANGE(IDC_PORT2MODECOMBO, &CSwitchDlg::OnPort2ModeChange)
	ON_CBN_SELCHANGE(IDC_PORT1VLANCOMBO, &CSwitchDlg::OnPort1VLANChange)
	ON_CBN_SELCHANGE(IDC_PORT2VLANCOMBO, &CSwitchDlg::OnPort2VLANChange)
	ON_BN_CLICKED(IDC_PORT1VLANSBUTTON, &CSwitchDlg::OnBnClickedPort1VLANs)
	ON_BN_CLICKED(IDC_PORT2VLANSBUTTON, &CSwitchDlg::OnBnClickedPort2VLANs)
	ON_NOTIFY(UDN_DELTAPOS, IDC_TIMEOUTSPIN, &CSwitchDlg::OnDeltaposTimeoutspin)
	ON_WM_TIMER()
	ON_MESSAGE(WM_INSERTTOMAC_MESSAGE, &CSwitchDlg::OnInsertToMacMessage)
	ON_MESSAGE(WM_DELETEFROMMAC_MESSAGE, &CSwitchDlg::OnDeleteFromMacMessage)
	ON_MESSAGE(WM_MODIFYMAC_MESSAGE, &CSwitchDlg::OnModifyMacMessage)
	ON_MESSAGE(WM_UPDATETIMEOUT_MESSAGE, &CSwitchDlg::OnUpdateTimeoutMessage)
	ON_MESSAGE(WM_INSERTSTAT_MESSAGE, &CSwitchDlg::OnInsertStatMessage)
	ON_MESSAGE(WM_UPDATESTAT_MESSAGE, &CSwitchDlg::OnUpdateStatMessage)
	ON_BN_CLICKED(IDC_STATSCHECK, &CSwitchDlg::OnBnClickedStatscheck)
	ON_BN_CLICKED(IDC_STATSRESETBUTTON, &CSwitchDlg::OnBnClickedStatsresetbutton)
	ON_BN_CLICKED(IDC_RULEADDBUTTON, &CSwitchDlg::OnBnClickedRuleAddButton)
	ON_BN_CLICKED(IDC_RULEEDITBUTTON, &CSwitchDlg::OnBnClickedRuleEditButton)
	ON_BN_CLICKED(IDC_RULEREMOVEBUTTON, &CSwitchDlg::OnBnClickedRuleRemoveButton)
	ON_BN_CLICKED(IDC_RULEREMOVEALLBUTTON, &CSwitchDlg::OnBnClickedRuleRemoveAllButton)
	ON_MESSAGE(WM_EDITRULE_MESSAGE, &CSwitchDlg::OnEditRuleMessage)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_RULESLIST, &CSwitchDlg::OnLvnItemchangedList)
	ON_BN_CLICKED(IDC_VLANADDBUTTON, &CSwitchDlg::OnBnClickedVlanAddButton)
END_MESSAGE_MAP()


// CSwitchDlg message handlers

BOOL CSwitchDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	InitPortsInfo();
	InitMACtable();
	InitStatsTable();
	InitFilterTable();
	InitVLANlist();
	SetTimer(1,5000,NULL);
	theApp.StartThreads();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CSwitchDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CSwitchDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CSwitchDlg::OnOK(void)
{
}


void CSwitchDlg::InitPortsInfo(void)
{
	m_port1macaddr.SetWindowTextW(theApp.GetPort1()->GetMACAddress());
	m_port2macaddr.SetWindowTextW(theApp.GetPort2()->GetMACAddress());
	
	m_port1desc.SetWindowTextW(theApp.GetPort1()->GetDescription());
	m_port2desc.SetWindowTextW(theApp.GetPort2()->GetDescription());
		
	m_port1mode.SetCurSel(0);
	m_port2mode.SetCurSel(0);

	m_port1accessvlan.SetCurSel(0);
	m_port2accessvlan.SetCurSel(0);

	m_port1vlansbutton.EnableWindow(FALSE);
	m_port2vlansbutton.EnableWindow(FALSE);
}


void CSwitchDlg::InitMACtable(void)
{
	CString tmp;
	
	m_mactable.SetExtendedStyle(m_mactable.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_mactable.InsertColumn(0,_T("Port"),LVCFMT_CENTER,32);
	m_mactable.InsertColumn(1,_T("MAC Address"),LVCFMT_CENTER,124);
	m_mactable.InsertColumn(2,_T("Timeout"),LVCFMT_CENTER,50);

	m_timeoutspin.SetRange(2,1199);
	m_timeoutspin.SetPos(theApp.GetMACtab()->GetTimeOut() / 5);
	tmp.Format(_T("%.2d:%.2d"),theApp.GetMACtab()->GetTimeOut() / 60, theApp.GetMACtab()->GetTimeOut() % 60);
	m_timeout.SetWindowTextW(tmp);
}


void CSwitchDlg::InitStatsTable(void)
{
	m_stats.SetExtendedStyle(m_stats.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_stats.InsertColumn(0,_T("Port"),LVCFMT_CENTER,32);
	m_stats.InsertColumn(1,_T("In/Out"),LVCFMT_CENTER,44);
	m_stats.InsertColumn(2,_T("Frame type"),LVCFMT_CENTER,67);
	m_stats.InsertColumn(3,_T("Protocol in Eth II"),LVCFMT_CENTER,92);
	m_stats.InsertColumn(4,_T("Protocol in IP"),LVCFMT_CENTER,86);
	m_stats.InsertColumn(5,_T("Count"),LVCFMT_CENTER,55);

	m_statscheckbox.SetCheck(BST_CHECKED);
	theApp.stats_enabled = 1;
}


void CSwitchDlg::InitFilterTable(void)
{
	m_rules.SetExtendedStyle(m_stats.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_CHECKBOXES);
	m_rules.InsertColumn(0,_T(""),LVCFMT_CENTER);
	m_rules.InsertColumn(1,_T("Policy"),LVCFMT_CENTER);
	m_rules.InsertColumn(2,_T("Port"),LVCFMT_CENTER);
	m_rules.InsertColumn(3,_T("In/Out"),LVCFMT_CENTER);
	m_rules.InsertColumn(4,_T("Source MAC"),LVCFMT_CENTER);
	m_rules.InsertColumn(5,_T("Destination MAC"),LVCFMT_CENTER);
	m_rules.InsertColumn(6,_T("Protocol in Eth II"),LVCFMT_CENTER);
	m_rules.InsertColumn(7,_T("Source IP"),LVCFMT_CENTER);
	m_rules.InsertColumn(8,_T("Destination IP"),LVCFMT_CENTER);
	m_rules.InsertColumn(9,_T("Protocol in IP"),LVCFMT_CENTER);
	m_rules.InsertColumn(10,_T("Layer 7"),LVCFMT_CENTER);
	
	AutoResizeColumns(&m_rules);
}


void CSwitchDlg::InitVLANlist(void)
{
	m_vlanlist.SetExtendedStyle(m_vlanlist.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_CHECKBOXES);
	m_vlanlist.InsertColumn(0,_T(""),LVCFMT_CENTER,25);
	m_vlanlist.InsertColumn(1,_T("VID"),LVCFMT_CENTER,36);
	m_vlanlist.InsertColumn(2,_T("T1"),LVCFMT_CENTER,25);
	m_vlanlist.InsertColumn(3,_T("T2"),LVCFMT_CENTER,25);
		
	m_VIDspin.SetBuddy(GetDlgItem(IDC_VLANID));
	m_VIDspin.SetRange32(2,4095);
	m_VIDspin.SetPos(2);

	theApp.GetVLANlist()->Add(1,1,1);
	m_vlanlist.InsertItem(0,_T(""));
	m_vlanlist.SetItemText(0,1,_T("1"));
	m_vlanlist.SetItemText(0,2,_T("*"));
	m_vlanlist.SetItemText(0,3,_T("*"));

	ListView_SetCheckState(m_vlanlist,0,TRUE);
}


void CSwitchDlg::OnPort1ModeChange()
{
	EnterCriticalSection(&CSwitchApp::m_cs_vlan);
	if (m_port1mode.GetCurSel() == 0)
	{
		theApp.GetPort1()->SetMode(ACCESS);
		m_port1accessvlan.EnableWindow(TRUE);
		m_port1vlansbutton.EnableWindow(FALSE);
	}
	else
	{
		theApp.GetPort1()->SetMode(TRUNK);
		m_port1accessvlan.EnableWindow(FALSE);
		m_port1vlansbutton.EnableWindow(TRUE);
	}
	LeaveCriticalSection(&CSwitchApp::m_cs_vlan);
}


void CSwitchDlg::OnPort2ModeChange()
{
	EnterCriticalSection(&CSwitchApp::m_cs_vlan);
	if (m_port2mode.GetCurSel() == 0)
	{
		theApp.GetPort2()->SetMode(ACCESS);
		m_port2accessvlan.EnableWindow(TRUE);
		m_port2vlansbutton.EnableWindow(FALSE);
	}
	else
	{
		theApp.GetPort2()->SetMode(TRUNK);
		m_port2accessvlan.EnableWindow(FALSE);
		m_port2vlansbutton.EnableWindow(TRUE);
	}
	LeaveCriticalSection(&CSwitchApp::m_cs_vlan);
}


void CSwitchDlg::OnPort1VLANChange()
{
	EnterCriticalSection(&CSwitchApp::m_cs_vlan);
	theApp.GetPort1()->SetVID(theApp.GetVLANlist()->GetVID(m_port1accessvlan.GetCurSel()));
	LeaveCriticalSection(&CSwitchApp::m_cs_vlan);
}


void CSwitchDlg::OnPort2VLANChange()
{
	EnterCriticalSection(&CSwitchApp::m_cs_vlan);
	theApp.GetPort2()->SetVID(theApp.GetVLANlist()->GetVID(m_port2accessvlan.GetCurSel()));
	LeaveCriticalSection(&CSwitchApp::m_cs_vlan);
}


void CSwitchDlg::OnBnClickedPort1VLANs()
{
	int i;
	BOOL IsChecked;
	
	EnterCriticalSection(&CSwitchApp::m_cs_vlan);
	for (i=0;i < m_vlanlist.GetItemCount();i++)
	{
		IsChecked = ListView_GetCheckState(m_vlanlist,i);
		theApp.GetVLANlist()->UpdateAllowedVLANs(i,1,IsChecked);
		if (IsChecked) m_vlanlist.SetItemText(i,2,_T("*"));
		else m_vlanlist.SetItemText(i,2,_T(""));
		ListView_SetCheckState(m_vlanlist,i,TRUE);

	}
	LeaveCriticalSection(&CSwitchApp::m_cs_vlan);
}


void CSwitchDlg::OnBnClickedPort2VLANs()
{
	int i;
	BOOL IsChecked;
	
	EnterCriticalSection(&CSwitchApp::m_cs_vlan);
	for (i=0;i < m_vlanlist.GetItemCount();i++)
	{
		IsChecked = ListView_GetCheckState(m_vlanlist,i);
		theApp.GetVLANlist()->UpdateAllowedVLANs(i,2,IsChecked);
		if (IsChecked) m_vlanlist.SetItemText(i,3,_T("*"));
		else m_vlanlist.SetItemText(i,3,_T(""));
		ListView_SetCheckState(m_vlanlist,i,TRUE);

	}
	LeaveCriticalSection(&CSwitchApp::m_cs_vlan);
}


void CSwitchDlg::InstertToMACTab(StoredMAC s)
{
	StoredMAC *sptr = (StoredMAC *) malloc(sizeof(StoredMAC));
	*sptr = s;
	SendMessage(WM_INSERTTOMAC_MESSAGE,0,(LPARAM)sptr);
}


void CSwitchDlg::DeleteFromMACTab(int index)
{
	int *indexptr = (int *) malloc(sizeof(int));
	*indexptr = index;
	SendMessage(WM_DELETEFROMMAC_MESSAGE,0,(LPARAM)indexptr);
}


void CSwitchDlg::ModifyMACTab(int index, StoredMAC s)
{
	int *indexptr = (int *) malloc(sizeof(int));
	StoredMAC *sptr = (StoredMAC *) malloc(sizeof(StoredMAC));
	*indexptr = index;
	*sptr = s;
	SendMessage(WM_MODIFYMAC_MESSAGE,(WPARAM)indexptr,(LPARAM)sptr);
}


void CSwitchDlg::UpdateTimeout(int index, UINT timeout)
{
	int *indexptr = (int *) malloc(sizeof(int));
	UINT *timeoutptr = (UINT *) malloc(sizeof(UINT));
	*indexptr = index;
	*timeoutptr = timeout;
	SendMessage(WM_UPDATETIMEOUT_MESSAGE,(WPARAM)indexptr,(LPARAM)timeoutptr);
}


void CSwitchDlg::OnDeltaposTimeoutspin(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	CString tmp;
	UINT seconds = (UINT)pNMUpDown->iPos * 5;
	
	EnterCriticalSection(&CSwitchApp::m_cs_mactable);
	theApp.GetMACtab()->SetTimeOut(seconds);
	LeaveCriticalSection(&CSwitchApp::m_cs_mactable);
	tmp.Format(_T("%.2d:%.2d"),seconds / 60, seconds % 60);
	m_timeout.SetWindowTextW(tmp);
	*pResult = 0;
}


void CSwitchDlg::OnTimer(UINT_PTR nIDEvent)
{
	EnterCriticalSection(&CSwitchApp::m_cs_mactable);

	theApp.GetMACtab()->Maintain();

	LeaveCriticalSection(&CSwitchApp::m_cs_mactable);

	CDialog::OnTimer(nIDEvent);
}


afx_msg LRESULT CSwitchDlg::OnInsertToMacMessage(WPARAM wParam, LPARAM lParam)
{
	StoredMAC *s = (StoredMAC *)lParam;
	int index = m_mactable.GetItemCount();
	CString tmp;
	
	tmp.Format(_T("%d"),s->port);
	m_mactable.InsertItem(index,tmp);
	tmp.Format(_T("%.2X:%.2X:%.2X:%.2X:%.2X:%.2X"),s->address.b[0],s->address.b[1],s->address.b[2],s->address.b[3],s->address.b[4],s->address.b[5]);
	m_mactable.SetItemText(index,1,tmp);
	tmp.Format(_T("%.2d:%.2d"),s->SecondsLeft / 60, s->SecondsLeft % 60);
	m_mactable.SetItemText(index,2,tmp);
	free(s);
	
	return 0;
}


afx_msg LRESULT CSwitchDlg::OnDeleteFromMacMessage(WPARAM wParam, LPARAM lParam)
{
	int *index = (int *)lParam;
	m_mactable.DeleteItem(*index);
	free(index);
	
	return 0;
}


afx_msg LRESULT CSwitchDlg::OnModifyMacMessage(WPARAM wParam, LPARAM lParam)
{
	int *index = (int *)wParam;
	StoredMAC *s = (StoredMAC *)lParam;
	CString tmp;
	
	m_mactable.DeleteItem(*index);
	tmp.Format(_T("%d"),s->port);
	m_mactable.InsertItem(*index,tmp);
	tmp.Format(_T("%.2X:%.2X:%.2X:%.2X:%.2X:%.2X"),s->address.b[0],s->address.b[1],s->address.b[2],s->address.b[3],s->address.b[4],s->address.b[5]);
	m_mactable.SetItemText(*index,1,tmp);
	tmp.Format(_T("%.2d:%.2d"),s->SecondsLeft / 60, s->SecondsLeft % 60);
	m_mactable.SetItemText(*index,2,tmp);
	free(index);
	free(s);
	
	return 0;
}


afx_msg LRESULT CSwitchDlg::OnUpdateTimeoutMessage(WPARAM wParam, LPARAM lParam)
{
	int *index = (int *)wParam;
	UINT *timeout = (UINT *)lParam;
	CString tmp;

	tmp.Format(_T("%.2d:%.2d"),*timeout / 60, *timeout % 60);
	m_mactable.SetItemText(*index,2,tmp);
	free(index);
	free(timeout);
	
	return 0;
}


afx_msg LRESULT CSwitchDlg::OnInsertStatMessage(WPARAM wParam, LPARAM lParam)
{
	int *index = (int *)wParam;
	Statistic *s = (Statistic *)lParam;
	CString tmp;
	
	tmp.Format(_T("%d"),s->port);
	m_stats.InsertItem(*index,tmp);

	if (s->d == In) m_stats.SetItemText(*index,1,_T("in"));
	else m_stats.SetItemText(*index,1,_T("out"));

	if (s->HasFrameType == 2) switch (s->FrameType)
	{
	case ETH2:
		m_stats.SetItemText(*index,2,_T("Eth II"));
		break;

	case RAW:
		m_stats.SetItemText(*index,2,_T("RAW"));
		break;

	case SNAP:
		m_stats.SetItemText(*index,2,_T("SNAP"));
		break;

	case LLC:
		m_stats.SetItemText(*index,2,_T("LLC"));
		break;
	}
	else m_stats.SetItemText(*index,2,_T("any"));

	if (s->HasLay3Type == 2) m_stats.SetItemText(*index,3,theApp.GetEth2ProtocolName(s->Lay3Type));
	else if (s->HasLay3Type == 1) m_stats.SetItemText(*index,3,_T("any"));
	else m_stats.SetItemText(*index,3,_T("-"));

	if (s->HasLay4Type == 2) m_stats.SetItemText(*index,4,theApp.GetIPProtocolName(s->Lay4Type));
	else if (s->HasLay4Type == 1) m_stats.SetItemText(*index,4,_T("any"));
	else m_stats.SetItemText(*index,4,_T("-"));

	tmp.Format(_T("%u"),s->count);
	m_stats.SetItemText(*index,5,tmp);

	free(index);
	free(s);
	
	return 0;
}


afx_msg LRESULT CSwitchDlg::OnUpdateStatMessage(WPARAM wParam, LPARAM lParam)
{
	int *index = (int *)wParam;
	UINT *count = (UINT *)lParam;
	CString tmp;

	tmp.Format(_T("%u"),*count);
	m_stats.SetItemText(*index,5,tmp);
	free(index);
	free(count);
	
	return 0;
}


void CSwitchDlg::InsertStat(int index, Statistic& s)
{
	int *indexptr = (int *) malloc(sizeof(int));
	Statistic *sptr = (Statistic *) malloc(sizeof(Statistic));
	*indexptr = index;
	*sptr = s;
	SendMessage(WM_INSERTSTAT_MESSAGE,(WPARAM)indexptr,(LPARAM)sptr);
}


void CSwitchDlg::UpdateStat(int index, UINT count)
{
	int *indexptr = (int *) malloc(sizeof(int));
	UINT *countptr = (UINT *) malloc(sizeof(UINT));
	*indexptr = index;
	*countptr = count;
	SendMessage(WM_UPDATESTAT_MESSAGE,(WPARAM)indexptr,(LPARAM)countptr);
}


void CSwitchDlg::OnBnClickedStatscheck()
{
	EnterCriticalSection(&CSwitchApp::m_cs_stats);
	if (m_statscheckbox.GetCheck()) theApp.stats_enabled = 1;
	else theApp.stats_enabled = 0;
	LeaveCriticalSection(&CSwitchApp::m_cs_stats);
}


void CSwitchDlg::OnBnClickedStatsresetbutton()
{
	EnterCriticalSection(&CSwitchApp::m_cs_stats);
	theApp.GetStatistics()->Reset();
	m_stats.DeleteAllItems();
	LeaveCriticalSection(&CSwitchApp::m_cs_stats);
}


void CSwitchDlg::AutoResizeColumns(CListCtrl *control)
{
	int i, ColumnWidth, HeaderWidth;
	int columns = control->GetHeaderCtrl()->GetItemCount();

	control->SetRedraw(FALSE);
	for (i=0;i < columns;i++)
	{
		control->SetColumnWidth(i,LVSCW_AUTOSIZE);
		ColumnWidth = control->GetColumnWidth(i);
		control->SetColumnWidth(i,LVSCW_AUTOSIZE_USEHEADER);
		HeaderWidth = control->GetColumnWidth(i);
		control->SetColumnWidth(i,max(ColumnWidth,HeaderWidth));
	}
	control->SetRedraw(TRUE);
}


void CSwitchDlg::OnBnClickedRuleAddButton()
{
	AfxBeginThread(CSwitchDlg::RuleEditThread,NULL);
}


void CSwitchDlg::OnBnClickedRuleEditButton()
{
	int *index = (int *) malloc(sizeof(int));

	*index = m_rules.GetSelectionMark();
	AfxBeginThread(CSwitchDlg::RuleEditThread,index);
}


void CSwitchDlg::OnBnClickedRuleRemoveButton()
{
	int index = m_rules.GetSelectionMark();

	if (index == -1) AfxMessageBox(_T("No rule was selected!"));
	else
	{
		EnterCriticalSection(&CSwitchApp::m_cs_filter);
		AllowCheckBoxes = 0;
		theApp.GetFilter()->Remove(index);
		m_rules.DeleteItem(index);
		AutoResizeColumns(&m_rules);
		AllowCheckBoxes = 1;
		LeaveCriticalSection(&CSwitchApp::m_cs_filter);
	}
}


void CSwitchDlg::OnBnClickedRuleRemoveAllButton()
{
	EnterCriticalSection(&CSwitchApp::m_cs_filter);
	AllowCheckBoxes = 0;
	theApp.GetFilter()->RemoveAll();
	m_rules.DeleteAllItems();
	AutoResizeColumns(&m_rules);
	LeaveCriticalSection(&CSwitchApp::m_cs_filter);
}


UINT CSwitchDlg::RuleEditThread(void * pParam)
{
	CRuleDlg rule_dlg((int *)pParam);
	
	if ((pParam) && (*((int *)pParam) == -1)) AfxMessageBox(_T("No rule was selected!"));
	else rule_dlg.DoModal();
	
	return 0;
}


afx_msg LRESULT CSwitchDlg::OnEditRuleMessage(WPARAM wParam, LPARAM lParam)
{
	int index;
	int *indexptr = (int *)wParam;
	Rule *r = (Rule *)lParam;
	CString tmp;
	
	if (wParam) {
		index = *((int *)wParam);
		m_rules.DeleteItem(index);
		free((int *)wParam);
	}
	else index = m_rules.GetItemCount();

	AllowCheckBoxes = 0;
	m_rules.InsertItem(index,_T(""));
	
	if (r->p == ALLOW) m_rules.SetItemText(index,1,_T("permit"));
	else m_rules.SetItemText(index,1,_T("deny"));
	
	if (r->HasPort == 2) tmp.Format(_T("%d"),r->SWport);
	else tmp.SetString(_T("any"));
	m_rules.SetItemText(index,2,tmp);

	if (r->HasDirection == 2)
	{
		if (r->d == In) m_rules.SetItemText(index,3,_T("in"));
		else m_rules.SetItemText(index,3,_T("out"));
	}
	else m_rules.SetItemText(index,3,_T("any"));

	if (r->HasSrcMAC == 2) tmp.Format(_T("%.2X:%.2X:%.2X:%.2X:%.2X:%.2X"),r->SrcMAC.b[0],r->SrcMAC.b[1],r->SrcMAC.b[2],r->SrcMAC.b[3],r->SrcMAC.b[4],r->SrcMAC.b[5]);
	else tmp.SetString(_T("any"));
	m_rules.SetItemText(index,4,tmp);

	if (r->HasDestMAC == 2) tmp.Format(_T("%.2X:%.2X:%.2X:%.2X:%.2X:%.2X"),r->DestMAC.b[0],r->DestMAC.b[1],r->DestMAC.b[2],r->DestMAC.b[3],r->DestMAC.b[4],r->DestMAC.b[5]);
	else tmp.SetString(_T("any"));
	m_rules.SetItemText(index,5,tmp);

	if (r->HasLay3Type == 2) m_rules.SetItemText(index,6,theApp.GetEth2ProtocolName(r->Lay3Type));
	else if (r->HasLay3Type == 1) m_rules.SetItemText(index,6,_T("any"));
	else m_rules.SetItemText(index,6,_T("-"));

	if (r->HasSrcIP == 2) tmp.Format(_T("%u.%u.%u.%u"),r->SrcIP.b[0],r->SrcIP.b[1],r->SrcIP.b[2],r->SrcIP.b[3]);
	else if (r->HasSrcIP == 1) tmp.SetString(_T("any"));
	else tmp.SetString(_T("-"));
	m_rules.SetItemText(index,7,tmp);

	if (r->HasDestIP == 2) tmp.Format(_T("%u.%u.%u.%u"),r->DestIP.b[0],r->DestIP.b[1],r->DestIP.b[2],r->DestIP.b[3]);
	else if (r->HasDestIP == 1) tmp.SetString(_T("any"));
	else tmp.SetString(_T("-"));
	m_rules.SetItemText(index,8,tmp);

	if (r->HasLay4Type == 2) m_rules.SetItemText(index,9,theApp.GetIPProtocolName(r->Lay4Type));
	else if (r->HasLay4Type == 1) m_rules.SetItemText(index,9,_T("any"));
	else m_rules.SetItemText(index,9,_T("-"));

	if (r->HasAppPort == 2) m_rules.SetItemText(index,10,theApp.GetAppName(r->AppPort,1));
	else if (r->HasAppPort == 1) m_rules.SetItemText(index,10,_T("any"));
	else m_rules.SetItemText(index,10,_T("-"));
	
	AutoResizeColumns(&m_rules);

	if (r->enabled) { ListView_SetCheckState(m_rules,index,TRUE); }
	else { ListView_SetCheckState(m_rules,index,FALSE); }
	
	AllowCheckBoxes = 1;
	free(r);
	
	return 0;
}


void CSwitchDlg::EditRule(int *index, Rule& r)
{
	Rule *rptr = (Rule *) malloc(sizeof(Rule));
	*rptr = r;

	EnterCriticalSection(&CSwitchApp::m_cs_filter);
	if (!index) theApp.GetFilter()->Add(r);
	else theApp.GetFilter()->Modify(*index,r);
	LeaveCriticalSection(&CSwitchApp::m_cs_filter);

	SendMessage(WM_EDITRULE_MESSAGE,(WPARAM)index,(LPARAM)rptr);
}


void CSwitchDlg::OnLvnItemchangedList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	if ((AllowCheckBoxes) && (pNMLV->uChanged & LVIF_STATE))
	{
		switch (pNMLV->uNewState & LVIS_STATEIMAGEMASK)
		{
		case INDEXTOSTATEIMAGEMASK(BST_CHECKED + 1):
			EnterCriticalSection(&CSwitchApp::m_cs_filter);
			theApp.GetFilter()->SetEnabled(pNMLV->iItem,1);
			LeaveCriticalSection(&CSwitchApp::m_cs_filter);
			break;
		case INDEXTOSTATEIMAGEMASK(BST_UNCHECKED + 1):
			EnterCriticalSection(&CSwitchApp::m_cs_filter);
			theApp.GetFilter()->SetEnabled(pNMLV->iItem,0);
			LeaveCriticalSection(&CSwitchApp::m_cs_filter);
			break;
		}
	}
	*pResult = 0;
}


void CSwitchDlg::OnBnClickedVlanAddButton()
{
	int retval;
	int newVID = (int) GetDlgItemInt(IDC_VLANID,NULL,0);
	int index = m_vlanlist.GetItemCount();
	CString tmp;

	EnterCriticalSection(&CSwitchApp::m_cs_vlan);
	retval = theApp.GetVLANlist()->Add(newVID,1,1);
	LeaveCriticalSection(&CSwitchApp::m_cs_vlan);

	if (retval) AfxMessageBox(_T("The selected VLAN ID is already exists!"));
	else
	{
		m_vlanlist.InsertItem(index,_T(""));

		tmp.Format(_T("%d"),newVID);
		m_vlanlist.SetItemText(index,1,tmp);
		m_vlanlist.SetItemText(index,2,_T("*"));
		m_vlanlist.SetItemText(index,3,_T("*"));

		tmp.Format(_T("VLAN %d"),newVID);
		m_port1accessvlan.AddString(tmp);
		m_port2accessvlan.AddString(tmp);

		ListView_SetCheckState(m_vlanlist,index,TRUE);
	}
}
