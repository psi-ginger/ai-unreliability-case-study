#ifndef _STRRESIGNCHIEFDLG_H
#define _STRRESIGNCHIEFDLG_H

#include "StrBaseDlg.h"

// 건물 행수 사임 탭 다이얼로그 
class NStrResignChiefDlg : public NStrTabDlg
{
public:
	enum {
		BUTTON_RESIGN
	};

	NStrResignChiefDlg();
	virtual ~NStrResignChiefDlg();

	void InitDlg();

	void NDlgTabProc( UINT nEvent, int nControlID, CControllerObj *pControl );
};


#endif