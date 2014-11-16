// InitDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Switch.h"
#include "InitDlg.h"
#include "afxdialogex.h"


// InitDlg dialog

IMPLEMENT_DYNAMIC(CInitDlg, CDialog)

CInitDlg::CInitDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CInitDlg::IDD, pParent)
	, interfaces(NULL)
{

}

CInitDlg::~CInitDlg()
{
}

void CInitDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PORT1COMBO, m_port1box);
	DDX_Control(pDX, IDC_PORT2COMBO, m_port2box);
}


BEGIN_MESSAGE_MAP(CInitDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CInitDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// InitDlg message handlers


void CInitDlg::OnBnClickedOk()
{
	char newname[PCAP_BUF_SIZE];
	CStringA port1name, port1desc, port2name, port2desc;

	if (m_port1box.GetCurSel() == m_port2box.GetCurSel())
	{
		MessageBox(_T("You cannon select the same interface for each ports!"),_T("Warning"),MB_ICONWARNING);
		return;
	}
	pcap_createsrcstr(newname,PCAP_SRC_IFLOCAL,NULL,NULL,IfNames[m_port1box.GetCurSel()],errbuf);
	theApp.GetPort1()->SetName(newname);
	theApp.GetPort1()->SetDescription(IfDescs[m_port1box.GetCurSel()]);
	pcap_createsrcstr(newname,PCAP_SRC_IFLOCAL,NULL,NULL,IfNames[m_port2box.GetCurSel()],errbuf);
	theApp.GetPort2()->SetName(newname);
	theApp.GetPort2()->SetDescription(IfDescs[m_port2box.GetCurSel()]);
	IfNames.RemoveAll();
	IfDescs.RemoveAll();
	pcap_freealldevs(interfaces);
	
	CDialog::OnOK();
}


BOOL CInitDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	if (pcap_findalldevs(&interfaces,errbuf) == -1)
	{
		MessageBox(CString("Error during searching for interfaces: ") + CString(errbuf),_T("Error"),MB_ICONERROR);
		OnCancel();
	}

	while (interfaces)
	{
		IfNames.Add(interfaces->name);
		if (interfaces->description)
		{
			IfDescs.Add(interfaces->description);
			m_port1box.AddString(CString(interfaces->description));
			m_port2box.AddString(CString(interfaces->description));
		}
		else
		{
			IfDescs.Add(interfaces->name);
			m_port1box.AddString(CString(interfaces->name));
			m_port2box.AddString(CString(interfaces->name));
		}
		interfaces = interfaces->next;
	}
	if (IfNames.GetCount() < 2)
	{
		MessageBox(_T("Application requires at least 2 network interfaces!"),_T("Error"),MB_ICONERROR);
		OnCancel();
	}
	m_port1box.SetCurSel(0);
	m_port2box.SetCurSel(1);
	return TRUE;
}
