#ifndef _STRREPAIRDLG_H
#define _STRREPAIRDLG_H

#include "StrBaseDlg.h"

class NStrInventory;

// 건물 수리 탭 다이얼로그 
class NStrRepairDlg : public NStrTabDlg
{
public:
	enum
	{
		EDIT_STRDURA,
		EDIT_BASEMONEY,
		EDIT_NEEDREPAIR,	// 수리해야 할 양
		EDIT_REPAIR,		// 행수가 수리할 양
			
		BUTTON_REPAIR,

		STATIC_GOLD_REPAIR,
		STATIC_SET_REPAIR,
		EDITBOX_GOLD_REPAIR,
		EDITBOX_SET_REPAIR,
		BUTTON_SET_REPAIR,
		
		STATIC_NONAME1,  // 설명,
		STATIC_NONAME2,  // 설명,
		STATIC_NONAME3,  // 설명,
		STATIC_NONAME10, // 설명,
		STATIC_NONAME11, // 설명,
		STATIC_NONAME12, // 설명,

		OUTLINE_NONAME2,
		OUTLINE_NONAME3,
	};

	NStrRepairDlg();
	virtual ~NStrRepairDlg();

	void InitDlg();

	void Action();

	void NDlgTabProc( UINT nEvent, int nControlID, CControllerObj *pControl );

	GMONEY m_siNeedRepairMoney;
	SI32		m_siNeedRepairItem;

};
/*
class NStrRepairDlg : public NStrTabDlg
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

	NStrRepairDlg();
	virtual ~NStrRepairDlg();

	void InitDlg();
	
	void Action();

	void NDlgTabProc( UINT nEvent, int nControlID, CControllerObj *pControl );

public:
	NStrInventory*	m_pMyInventory;
};
*/


#endif
