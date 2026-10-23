#ifndef _STRINFODLG_H
#define _STRINFODLG_H

#include "StrBaseDlg.h"

// 건물 기본 정보 탭 다이얼로그 
class NStrInfoDlg : public NStrTabDlg
{
public:
	enum {
		EDIT_STRDURA,
		EDIT_CHIEF,
		EDIT_MONEY,

		LIST_TAXLIST

	};
	NStrInfoDlg();
	virtual ~NStrInfoDlg();

	void InitDlg();

	void Action();

	void NDlgTabProc( UINT nEvent, int nControlID, CControllerObj *pControl );

public:
	BOOL m_bShowTaxInfo;
};


#endif