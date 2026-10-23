#ifndef _STRSUBPAYTAXDLG_H
#define _STRSUBPAYTAXDLG_H

#include "StrSubBaseDlg.h"

// 건물 세금 납부 탭 다이얼로그 
class NStrSubPayTaxDlg : public NStrSubTabDlg
{
public:
	enum {
		EDIT_MONEY,
		BUTTON_PAYTAX
	};
	NStrSubPayTaxDlg();
	virtual ~NStrSubPayTaxDlg();

	void InitDlg();

	void NDlgTabProc( UINT nEvent, int nControlID, CControllerObj *pControl );
};


#endif