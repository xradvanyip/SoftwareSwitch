
// SwitchDlg.h : header file
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "FixListCtrl.h"
#include "MACtable.h"
#include "Stats.h"

#define WM_INSERTTOMAC_MESSAGE WM_APP+100
#define WM_DELETEFROMMAC_MESSAGE WM_APP+101
#define WM_MODIFYMAC_MESSAGE WM_APP+102
#define WM_UPDATETIMEOUT_MESSAGE WM_APP+103
#define WM_INSERTSTAT_MESSAGE WM_APP+104
#define WM_UPDATESTAT_MESSAGE WM_APP+105

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
	
	CFixListCtrl m_mactable;
	CEdit m_timeout;
	CSpinButtonCtrl m_timeoutspin;

	CFixListCtrl m_stats;
	CButton m_statscheckbox;
	CButton m_statsresetbutton;
	
	void InitPortsInfo(void);
	void InitMACtable(void);
	void InitStatsTable(void);
public:
	afx_msg void OnPort1ModeChange();
	afx_msg void OnPort2ModeChange();
	afx_msg void OnPort1VLANChange();
	afx_msg void OnPort2VLANChange();
	afx_msg void OnBnClickedPort1VLANs();
	afx_msg void OnBnClickedPort2VLANs();
	afx_msg void OnBnClickedStatscheck();
	afx_msg void OnBnClickedStatsresetbutton();
	void InstertToMACTab(StoredMAC s);
	void DeleteFromMACTab(int index);
	void ModifyMACTab(int index, StoredMAC s);
	void UpdateTimeout(int index, UINT timeout);
	afx_msg void OnDeltaposTimeoutspin(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	void InsertStat(int index, Statistic s);
	void UpdateStat(int index, UINT count);
protected:
	afx_msg LRESULT OnInsertToMacMessage(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnDeleteFromMacMessage(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnModifyMacMessage(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUpdateTimeoutMessage(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnInsertStatMessage(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUpdateStatMessage(WPARAM wParam, LPARAM lParam);
};
