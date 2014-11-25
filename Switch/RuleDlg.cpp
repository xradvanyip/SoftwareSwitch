// RuleDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Switch.h"
#include "RuleDlg.h"
#include "afxdialogex.h"
#include "MACtable.h"


// CRuleDlg dialog

IMPLEMENT_DYNAMIC(CRuleDlg, CDialog)

CRuleDlg::CRuleDlg(int *RuleIndex, CWnd* pParent /*=NULL*/)
	: CDialog(CRuleDlg::IDD, pParent)
	, index(NULL)
{
	index = RuleIndex;
}

CRuleDlg::~CRuleDlg()
{
}

void CRuleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ENABLECHECK, m_EnableCheckBox);
	DDX_Control(pDX, IDC_POLICYCOMBO, m_PolicyCombo);
	DDX_Control(pDX, IDC_PORTCOMBO, m_PortCombo);
	DDX_Control(pDX, IDC_DIRECTIONCOMBO, m_DirectionCombo);
	DDX_Control(pDX, IDC_SRCMACCOMBO, m_SrcMACcombo);
	DDX_Control(pDX, IDC_DESTMACCOMBO, m_DestMACcombo);
	DDX_Control(pDX, IDC_ETH2PROTCOMBO, m_Eth2ProtCombo);
	DDX_Control(pDX, IDC_SRCIPADDRESS, m_SrcIPaddr);
	DDX_Control(pDX, IDC_SRCIPCHECK, m_SrcIPcheckbox);
	DDX_Control(pDX, IDC_DESTIPADDRESS, m_DestIPaddr);
	DDX_Control(pDX, IDC_DESTIPCHECK, m_DestIPcheckbox);
	DDX_Control(pDX, IDC_IPPROTCOMBO, m_IPprotCombo);
	DDX_Control(pDX, IDC_LAYER7COMBO, m_Layer7Combo);
}


BEGIN_MESSAGE_MAP(CRuleDlg, CDialog)
	ON_CBN_SELCHANGE(IDC_ETH2PROTCOMBO, &CRuleDlg::OnCbnSelchangeEth2protcombo)
	ON_CBN_SELCHANGE(IDC_IPPROTCOMBO, &CRuleDlg::OnCbnSelchangeIpprotcombo)
	ON_BN_CLICKED(IDC_SRCIPCHECK, &CRuleDlg::OnBnClickedSrcipcheck)
	ON_BN_CLICKED(IDC_DESTIPCHECK, &CRuleDlg::OnBnClickedDestipcheck)
	ON_BN_CLICKED(IDOK, &CRuleDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CRuleDlg message handlers


BOOL CRuleDlg::OnInitDialog()
{
	Rule r;
	int result;
	CString tmp;
	CDialog::OnInitDialog();

	CopyMACtable();
	FillInEth2Protocols();
	FillInIPProtocols();
	
	if (!index)
	{
		m_EnableCheckBox.SetCheck(BST_CHECKED);
		m_PolicyCombo.SetCurSel(0);
		m_PortCombo.SetCurSel(0);
		m_DirectionCombo.SetCurSel(0);
		m_SrcMACcombo.SetCurSel(0);
		m_DestMACcombo.SetCurSel(0);
		m_Eth2ProtCombo.SetCurSel(0);
		m_SrcIPcheckbox.SetCheck(BST_CHECKED);
		m_DestIPcheckbox.SetCheck(BST_CHECKED);
		OnCbnSelchangeEth2protcombo();
	}
	else
	{
		r = theApp.GetFilter()->Get(*index);

		if (r.enabled) m_EnableCheckBox.SetCheck(BST_CHECKED);
		else m_EnableCheckBox.SetCheck(BST_UNCHECKED);

		if (r.p == ALLOW) m_PolicyCombo.SetCurSel(0);
		else m_PolicyCombo.SetCurSel(1);

		if (r.HasPort == 1) m_PortCombo.SetCurSel(0);
		else m_PortCombo.SetCurSel(r.SWport);

		if (r.HasDirection == 1) m_DirectionCombo.SetCurSel(0);
		else if (r.d == In) m_DirectionCombo.SetCurSel(1);
		else m_DirectionCombo.SetCurSel(2);

		if (r.HasSrcMAC == 1) m_SrcMACcombo.SetCurSel(0);
		else
		{
			tmp.Format(_T("%.2X:%.2X:%.2X:%.2X:%.2X:%.2X"),r.SrcMAC.b[0],r.SrcMAC.b[1],r.SrcMAC.b[2],r.SrcMAC.b[3],r.SrcMAC.b[4],r.SrcMAC.b[5]);
			m_SrcMACcombo.SetWindowTextW(tmp);
		}

		if (r.HasDestMAC == 1) m_DestMACcombo.SetCurSel(0);
		else
		{
			tmp.Format(_T("%.2X:%.2X:%.2X:%.2X:%.2X:%.2X"),r.DestMAC.b[0],r.DestMAC.b[1],r.DestMAC.b[2],r.DestMAC.b[3],r.DestMAC.b[4],r.DestMAC.b[5]);
			m_DestMACcombo.SetWindowTextW(tmp);
		}

		m_Eth2ProtCombo.SetCurSel(0);
		if (r.HasLay3Type == 2)
		{
			result = theApp.FindInEth2ProtocolList(r.Lay3Type);
			if (result != -1) m_Eth2ProtCombo.SetCurSel(result + 1);
			else AfxMessageBox(_T("Error while setting up the Eth2 protocol field!"));
		}

		m_SrcIPcheckbox.SetCheck(BST_CHECKED);
		switch (r.HasSrcIP)
		{
		case 0:
			m_SrcIPcheckbox.EnableWindow(FALSE);
			m_SrcIPaddr.EnableWindow(FALSE);
			break;

		case 1:
			m_SrcIPaddr.EnableWindow(FALSE);
			break;

		case 2:
			m_SrcIPcheckbox.SetCheck(BST_UNCHECKED);
			m_SrcIPaddr.SetAddress(r.SrcIP.b[0],r.SrcIP.b[1],r.SrcIP.b[2],r.SrcIP.b[3]);
			break;
		}

		m_DestIPcheckbox.SetCheck(BST_CHECKED);
		switch (r.HasDestIP)
		{
		case 0:
			m_DestIPcheckbox.EnableWindow(FALSE);
			m_DestIPaddr.EnableWindow(FALSE);
			break;

		case 1:
			m_DestIPaddr.EnableWindow(FALSE);
			break;

		case 2:
			m_DestIPcheckbox.SetCheck(BST_UNCHECKED);
			m_DestIPaddr.SetAddress(r.DestIP.b[0],r.DestIP.b[1],r.DestIP.b[2],r.DestIP.b[3]);
			break;
		}

		m_IPprotCombo.SetCurSel(0);
		m_Layer7Combo.SetCurSel(0);
		switch (r.HasLay4Type)
		{
		case 0:
			m_IPprotCombo.EnableWindow(FALSE);
			break;

		case 1:
			m_IPprotCombo.EnableWindow(TRUE);
			break;

		case 2:
			result = theApp.FindInIPProtocolList(r.Lay4Type);
			if (result != -1) m_IPprotCombo.SetCurSel(result + 1);
			else AfxMessageBox(_T("Error while setting up the IP protocol field!"));
			m_IPprotCombo.EnableWindow(TRUE);
			if (r.Lay4Type == 6) FillInTCPapps();
			if (r.Lay4Type == 17) FillInUDPapps();
			break;
		}

		switch (r.HasAppPort)
		{
		case 0:
			m_Layer7Combo.EnableWindow(FALSE);
			break;

		case 1:
			m_Layer7Combo.EnableWindow(TRUE);
			break;

		case 2:
			result = -1;
			if (r.Lay4Type == 6) result = theApp.FindInTCPAppList(r.AppPort);
			if (r.Lay4Type == 17) result = theApp.FindInUDPAppList(r.AppPort);
			if (result != -1) m_Layer7Combo.SetCurSel(result + 1);
			else {
				tmp.Format(_T("%u"),r.AppPort);
				m_Layer7Combo.SetWindowTextW(tmp);
			}
			m_Layer7Combo.EnableWindow(TRUE);
			break;
		}
	}
	
	return TRUE;
}


void CRuleDlg::CopyMACtable(void)
{
	int i;
	CString tmp;

	EnterCriticalSection(&CSwitchApp::m_cs_mactable);
	CArray<StoredMAC> &table = theApp.GetMACtab()->GetTable();
	for (i=0;i < table.GetCount();i++)
	{
		tmp.Format(_T("%.2X:%.2X:%.2X:%.2X:%.2X:%.2X"),table[i].address.b[0],table[i].address.b[1],table[i].address.b[2],table[i].address.b[3],table[i].address.b[4],table[i].address.b[5]);
		m_SrcMACcombo.AddString(tmp);
		m_DestMACcombo.AddString(tmp);
	}
	LeaveCriticalSection(&CSwitchApp::m_cs_mactable);
}


void CRuleDlg::FillInEth2Protocols(void)
{
	int i;
	CArray<CStringA> &names = theApp.GetEth2ProtocolList();

	for (i=0;i < names.GetCount();i++) m_Eth2ProtCombo.AddString(CString(names[i]));
}


void CRuleDlg::FillInIPProtocols(void)
{
	int i;
	CArray<CStringA> &names = theApp.GetIPProtocolList();

	for (i=0;i < names.GetCount();i++) m_IPprotCombo.AddString(CString(names[i]));
}


void CRuleDlg::FillInTCPapps(void)
{
	int i;
	CArray<CStringA> &names = theApp.GetTCPAppList();

	m_Layer7Combo.ResetContent();
	m_Layer7Combo.AddString(_T("any"));
	for (i=0;i < names.GetCount();i++) m_Layer7Combo.AddString(CString(names[i]));
	m_Layer7Combo.SetCurSel(0);
}


void CRuleDlg::FillInUDPapps(void)
{
	int i;
	CArray<CStringA> &names = theApp.GetUDPAppList();

	m_Layer7Combo.ResetContent();
	m_Layer7Combo.AddString(_T("any"));
	for (i=0;i < names.GetCount();i++) m_Layer7Combo.AddString(CString(names[i]));
	m_Layer7Combo.SetCurSel(0);
}


void CRuleDlg::OnCbnSelchangeEth2protcombo()
{
	int index = m_Eth2ProtCombo.GetCurSel();

	if ((index != 0) && (!theApp.GetEth2ProtocolList().GetAt(index - 1).Compare("IP")))
	{
		m_SrcIPcheckbox.EnableWindow(TRUE);
		OnBnClickedSrcipcheck();
		m_DestIPcheckbox.EnableWindow(TRUE);
		OnBnClickedDestipcheck();
		m_IPprotCombo.EnableWindow(TRUE);
		OnCbnSelchangeIpprotcombo();
	}
	else
	{
		m_SrcIPcheckbox.EnableWindow(FALSE);
		m_SrcIPaddr.EnableWindow(FALSE);
		m_DestIPcheckbox.EnableWindow(FALSE);
		m_DestIPaddr.EnableWindow(FALSE);
		m_IPprotCombo.SetCurSel(0);
		m_IPprotCombo.EnableWindow(FALSE);
		m_Layer7Combo.SetCurSel(0);
		m_Layer7Combo.EnableWindow(FALSE);
	}
}


void CRuleDlg::OnCbnSelchangeIpprotcombo()
{
	int index = m_IPprotCombo.GetCurSel();

	if ((index != 0) && (!theApp.GetIPProtocolList().GetAt(index - 1).Compare("TCP")))
	{
		m_Layer7Combo.EnableWindow(TRUE);
		FillInTCPapps();
	}
	else if ((index != 0) && (!theApp.GetIPProtocolList().GetAt(index - 1).Compare("UDP")))
	{
		m_Layer7Combo.EnableWindow(TRUE);
		FillInUDPapps();
	}
	else
	{
		m_Layer7Combo.SetCurSel(0);
		m_Layer7Combo.EnableWindow(FALSE);
	}
}


void CRuleDlg::OnBnClickedSrcipcheck()
{
	if (m_SrcIPcheckbox.GetCheck()) m_SrcIPaddr.EnableWindow(FALSE);
	else m_SrcIPaddr.EnableWindow(TRUE);
}


void CRuleDlg::OnBnClickedDestipcheck()
{
	if (m_DestIPcheckbox.GetCheck()) m_DestIPaddr.EnableWindow(FALSE);
	else m_DestIPaddr.EnableWindow(TRUE);
}


void CRuleDlg::OnBnClickedOk()
{
	Rule newRule;
	CString tmp;
	WORD macaddr[6];
	int i, index1, index2;

	if (m_EnableCheckBox.GetCheck()) newRule.enabled = 1;
	else newRule.enabled = 0;

	if (m_PolicyCombo.GetCurSel() == 0) newRule.p = ALLOW;
	else newRule.p = DENY;

	if (m_PortCombo.GetCurSel() == 0) newRule.HasPort = 1;
	else {
		newRule.HasPort = 2;
		newRule.SWport = m_PortCombo.GetCurSel();
	}

	switch (m_DirectionCombo.GetCurSel())
	{
	case 0:
		newRule.HasDirection = 1;
		break;

	case 1:
		newRule.HasDirection = 2;
		newRule.d = In;
		break;

	case 2:
		newRule.HasDirection = 2;
		newRule.d = Out;
		break;
	}

	if (m_SrcMACcombo.GetCurSel() == 0) newRule.HasSrcMAC = 1;
	else
	{
		m_SrcMACcombo.GetWindowTextW(tmp);
		if (swscanf(tmp,_T("%hX:%hX:%hX:%hX:%hX:%hX"),&macaddr[0],&macaddr[1],&macaddr[2],&macaddr[3],&macaddr[4],&macaddr[5]) != 6)
		{
			AfxMessageBox(_T("Source MAC address has incorrect format!"));
			return;
		}
		else {
			for (i=0;i < 6;i++) newRule.SrcMAC.b[i] = (BYTE)macaddr[i];
			newRule.HasSrcMAC = 2;
		}
	}

	if (m_DestMACcombo.GetCurSel() == 0) newRule.HasDestMAC = 1;
	else
	{
		m_DestMACcombo.GetWindowTextW(tmp);
		if (swscanf(tmp,_T("%hX:%hX:%hX:%hX:%hX:%hX"),&macaddr[0],&macaddr[1],&macaddr[2],&macaddr[3],&macaddr[4],&macaddr[5]) != 6)
		{
			AfxMessageBox(_T("Destination MAC address has incorrect format!"));
			return;
		}
		else {
			for (i=0;i < 6;i++) newRule.DestMAC.b[i] = (BYTE)macaddr[i];
			newRule.HasDestMAC = 2;
		}
	}

	if ((index1 = m_Eth2ProtCombo.GetCurSel()) != 0)
	{
		newRule.Lay3Type = theApp.GetEth2ProtocolNum(theApp.GetEth2ProtocolList().GetAt(index1 - 1));
		newRule.HasLay3Type = 2;

		if (newRule.Lay3Type == 0x0800)
		{
			if (m_SrcIPcheckbox.GetCheck()) newRule.HasSrcIP = 1;
			else
			{
				m_SrcIPaddr.GetAddress(newRule.SrcIP.b[0],newRule.SrcIP.b[1],newRule.SrcIP.b[2],newRule.SrcIP.b[3]);
				newRule.HasSrcIP = 2;
			}

			if (m_DestIPcheckbox.GetCheck()) newRule.HasDestIP = 1;
			else
			{
				m_DestIPaddr.GetAddress(newRule.DestIP.b[0],newRule.DestIP.b[1],newRule.DestIP.b[2],newRule.DestIP.b[3]);
				newRule.HasDestIP = 2;
			}

			if ((index2 = m_IPprotCombo.GetCurSel()) != 0)
			{
				newRule.Lay4Type = theApp.GetIPProtocolNum(theApp.GetIPProtocolList().GetAt(index2 - 1));
				newRule.HasLay4Type = 2;

				if ((newRule.Lay4Type == 6) || (newRule.Lay4Type == 17))
				{
					if (m_Layer7Combo.GetCurSel() == 0) newRule.HasAppPort = 1;
					else if (m_Layer7Combo.GetCurSel() > 0)
					{
						if (newRule.Lay4Type == 6) newRule.AppPort = theApp.GetPortNumber(theApp.GetTCPAppList().GetAt(m_Layer7Combo.GetCurSel() - 1));
						else newRule.AppPort = theApp.GetPortNumber(theApp.GetUDPAppList().GetAt(m_Layer7Combo.GetCurSel() - 1));
						newRule.HasAppPort = 2;
					}
					else
					{
						m_Layer7Combo.GetWindowTextW(tmp);
						swscanf(tmp,_T("%hu"),&newRule.AppPort);
						newRule.HasAppPort = 2;
					}
				}
				else newRule.HasAppPort = 0;

			}
			else
			{
				newRule.HasLay4Type = 1;
				newRule.HasAppPort = 0;
			}
		}
		else
		{
			newRule.HasSrcIP = 0;
			newRule.HasDestIP = 0;
			newRule.HasLay4Type = 0;
			newRule.HasAppPort = 0;
		}
	}
	else
	{
		newRule.HasLay3Type = 1;
		newRule.HasSrcIP = 0;
		newRule.HasDestIP = 0;
		newRule.HasLay4Type = 0;
		newRule.HasAppPort = 0;
	}

	theApp.GetSwitchDlg()->EditRule(index,newRule);

	CDialog::OnOK();
}


int CRuleDlg::IsNumber(CString str)
{
	//if (str.SpanIncluding(_T("0123456789")) == str)
	return 0;
}
