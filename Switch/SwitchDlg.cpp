
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
	m_port1macaddr.SetWindowTextW(theApp.GetPort1().GetMACAddress());
	m_port2macaddr.SetWindowTextW(theApp.GetPort2().GetMACAddress());
	
	m_port1desc.SetWindowTextW(theApp.GetPort1().GetDescription());
	m_port2desc.SetWindowTextW(theApp.GetPort2().GetDescription());
		
	m_port1mode.SetCurSel(0);
	m_port2mode.SetCurSel(0);

	m_port1accessvlan.SetCurSel(0);
	m_port2accessvlan.SetCurSel(0);

	m_port1vlansbutton.EnableWindow(FALSE);
	m_port2vlansbutton.EnableWindow(FALSE);

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

void CSwitchDlg::OnPort1ModeChange()
{
	if (m_port1mode.GetCurSel() == 0)
	{
		theApp.GetPort1().SetMode(ACCESS);
		m_port1accessvlan.EnableWindow(TRUE);
		m_port1vlansbutton.EnableWindow(FALSE);
	}
	else
	{
		theApp.GetPort1().SetMode(TRUNK);
		m_port1accessvlan.EnableWindow(FALSE);
		m_port1vlansbutton.EnableWindow(TRUE);
	}
}


void CSwitchDlg::OnPort2ModeChange()
{
	if (m_port2mode.GetCurSel() == 0)
	{
		theApp.GetPort2().SetMode(ACCESS);
		m_port2accessvlan.EnableWindow(TRUE);
		m_port2vlansbutton.EnableWindow(FALSE);
	}
	else
	{
		theApp.GetPort2().SetMode(TRUNK);
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
}


void CSwitchDlg::OnBnClickedPort2VLANs()
{
	// TODO: Add your control notification handler code here
}
