#pragma once


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

	DECLARE_MESSAGE_MAP()
};
