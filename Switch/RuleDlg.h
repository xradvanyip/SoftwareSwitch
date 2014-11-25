#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CRuleDlg dialog

class CRuleDlg : public CDialog
{
	DECLARE_DYNAMIC(CRuleDlg)

public:
	CRuleDlg(int *RuleIndex, CWnd* pParent = NULL);   // standard constructor
	virtual ~CRuleDlg();

// Dialog Data
	enum { IDD = IDD_RULE_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
private:
	int *index;
	CButton m_EnableCheckBox;
	CComboBox m_PolicyCombo;
	CComboBox m_PortCombo;
	CComboBox m_DirectionCombo;
	CComboBox m_SrcMACcombo;
	CComboBox m_DestMACcombo;
	CComboBox m_Eth2ProtCombo;
	CIPAddressCtrl m_SrcIPaddr;
	CButton m_SrcIPcheckbox;
	CIPAddressCtrl m_DestIPaddr;
	CButton m_DestIPcheckbox;
	CComboBox m_IPprotCombo;
	CComboBox m_Layer7Combo;
	void CopyMACtable(void);
	void FillInEth2Protocols(void);
	void FillInIPProtocols(void);
	void FillInTCPapps(void);
	void FillInUDPapps(void);
public:
	afx_msg void OnCbnSelchangeEth2protcombo();
	afx_msg void OnCbnSelchangeIpprotcombo();
	afx_msg void OnBnClickedSrcipcheck();
	afx_msg void OnBnClickedDestipcheck();
	afx_msg void OnBnClickedOk();
private:
	int IsNumber(CString str);
};
