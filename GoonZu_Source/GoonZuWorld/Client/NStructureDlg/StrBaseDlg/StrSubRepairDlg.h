#ifndef _STRSUBREPAIRDLG_H
#define _STRSUBREPAIRDLG_H

#include "StrSubBaseDlg.h"

class NStrSubInventory;

// 건물 수리 탭 다이얼로그 
class NStrSubRepairDlg : public NStrSubTabDlg
{
public:
	enum {
		EDIT_STRDURA,
		EDIT_NEEDITEM,
		EDIT_NEEDITEM_NUM,
		EDIT_USEITEM_NUM,
		EDIT_ITEMNAME,

		BUTTON_USEITEM,

		LISTVIEW_INVENTORY
	};

	NStrSubRepairDlg();
	virtual ~NStrSubRepairDlg();

	void InitDlg();
	
	void Action();

	void NDlgTabProc( UINT nEvent, int nControlID, CControllerObj *pControl );

public:
	NStrSubInventory*	m_pMyInventory;
};


#endif