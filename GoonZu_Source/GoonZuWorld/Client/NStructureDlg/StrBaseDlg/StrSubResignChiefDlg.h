#ifndef _STRSUBRESIGNCHIEFDLG_H
#define _STRSUBRESIGNCHIEFDLG_H

#include "StrSubBaseDlg.h"

// 건물 행수 사임 탭 다이얼로그 
class NStrSubResignChiefDlg : public NStrSubTabDlg
{
public:
	enum {
		BUTTON_RESIGN
	};

	NStrSubResignChiefDlg();
	virtual ~NStrSubResignChiefDlg();

	void InitDlg();

	void NDlgTabProc( UINT nEvent, int nControlID, CControllerObj *pControl );
};


#endif