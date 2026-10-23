#ifndef _STRPAYTAXDLG_H
#define _STRPAYTAXDLG_H

#include "StrBaseDlg.h"

// 건물 세금 납부 탭 다이얼로그 
class NStrPayTaxDlg : public NStrTabDlg
{
public:
	enum {
		EDIT_MONEY,
		BUTTON_PAYTAX
	};
	NStrPayTaxDlg();
	virtual ~NStrPayTaxDlg();

	void InitDlg();

	void NDlgTabProc( UINT nEvent, int nControlID, CControllerObj *pControl );
};


#endif