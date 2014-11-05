#include "stdafx.h"
#include "FixListCtrl.h"


CFixListCtrl::CFixListCtrl(void)
{
}


CFixListCtrl::~CFixListCtrl(void)
{
}


BOOL CFixListCtrl::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	switch (((NMHDR*)lParam)->code)
	{
		case HDN_BEGINTRACKW:
		case HDN_BEGINTRACKA:
			*pResult = TRUE;
			return TRUE;
	}
	
	return CListCtrl::OnNotify(wParam, lParam, pResult);
}
