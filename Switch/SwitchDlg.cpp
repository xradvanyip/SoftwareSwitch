
// SwitchDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Switch.h"
#include "SwitchDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSwitchDlg dialog



CSwitchDlg::CSwitchDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSwitchDlg::IDD, pParent)
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


void CSwitchDlg::OnPort1ModeChange()
{
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
}


void CSwitchDlg::OnPort2ModeChange()
{
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
}


void CSwitchDlg::OnPort1VLANChange()
{
	// TODO: Add your control notification handler code here
}


void CSwitchDlg::OnPort2VLANChange()
{
	// TODO: Add your control notification handler code here
}


void CSwitchDlg::OnBnClickedPort1VLANs()
{
	// TODO: Add your control notification handler code here
	CString str;
	str.Format(_T("%d %d %d count:%d"),m_mactable.GetColumnWidth(0),m_mactable.GetColumnWidth(1),m_mactable.GetColumnWidth(2),m_mactable.GetItemCount());
	AfxMessageBox(str);
}


void CSwitchDlg::OnBnClickedPort2VLANs()
{
	// TODO: Add your control notification handler code here
	theApp.GetMACtab()->Add(1,theApp.GetPort1()->GetMACAddrStruct());
	theApp.GetMACtab()->Add(2,theApp.GetPort2()->GetMACAddrStruct());
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
	
	EnterCriticalSection(&CSwitchApp::m_cs);
	theApp.GetMACtab()->SetTimeOut(seconds);
	LeaveCriticalSection(&CSwitchApp::m_cs);
	tmp.Format(_T("%.2d:%.2d"),seconds / 60, seconds % 60);
	m_timeout.SetWindowTextW(tmp);
	*pResult = 0;
}


void CSwitchDlg::OnTimer(UINT_PTR nIDEvent)
{
	EnterCriticalSection(&CSwitchApp::m_cs);

	theApp.GetMACtab()->Maintain();

	LeaveCriticalSection(&CSwitchApp::m_cs);

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
