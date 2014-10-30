#pragma once
#include "afxwin.h"
#include "afxtempl.h"


// InitDlg dialog

class CInitDlg : public CDialog
{
	DECLARE_DYNAMIC(CInitDlg)

public:
	CInitDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CInitDlg();

// Dialog Data
	enum { IDD = IDD_INIT_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
private:
	CComboBox m_port1box;
	CComboBox m_port2box;
	pcap_if_t *interfaces;
	CArray<char *,char *> IfNames;
	CArray<char *,char *> IfDescs;
	char errbuf[PCAP_ERRBUF_SIZE];
};
