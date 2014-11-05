#pragma once
#include "afxcmn.h"
class CFixListCtrl :
	public CListCtrl
{
public:
	CFixListCtrl(void);
	~CFixListCtrl(void);
protected:
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
};

