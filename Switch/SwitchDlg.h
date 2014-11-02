
// SwitchDlg.h : header file
//

#pragma once
#include "afxwin.h"


// CSwitchDlg dialog
class CSwitchDlg : public CDialog
{
// Construction
public:
	CSwitchDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_SWITCH_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

	virtual void OnOK(void);
private:
	CStatic m_port1desc;
	CStatic m_port1macaddr;
	CComboBox m_port1mode;
	CComboBox m_port1accessvlan;
	CButton m_port1vlansbutton;

	CStatic m_port2desc;
	CStatic m_port2macaddr;
	CComboBox m_port2mode;
	CComboBox m_port2accessvlan;
	CButton m_port2vlansbutton;
public:
	afx_msg void OnPort1ModeChange();
	afx_msg void OnPort2ModeChange();
	afx_msg void OnPort1VLANChange();
	afx_msg void OnPort2VLANChange();
	afx_msg void OnBnClickedPort1VLANs();
	afx_msg void OnBnClickedPort2VLANs();
};
