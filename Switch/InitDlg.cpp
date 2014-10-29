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
{

}

CInitDlg::~CInitDlg()
{
}

void CInitDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CInitDlg, CDialog)
END_MESSAGE_MAP()


// InitDlg message handlers
