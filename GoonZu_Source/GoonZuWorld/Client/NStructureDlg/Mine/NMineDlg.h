#ifndef _NMINEDLG_H
#define _NMINEDLG_H

#include "../StrBaseDlg/StrBaseDlg.h"
#include "../../CommonLogic/Mine/Mine.h"

class NStrInventory;

class NMineDlg : public NStrBaseDlg
{
public:
	NMineDlg();
	virtual ~NMineDlg();

	void Create();

	void GreetingMsg();
	
	void RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen );
};

// 광산 목록
class NMineListDlg : public NStrTabDlg
{
public:
	enum {
		LIST_MINELIST,
	};

	NMineListDlg();
	virtual ~NMineListDlg();

	void InitDlg();
	
	void Action();

	void ShowMineUnitList();

	void NDlgTabProc( UINT nEvent, int nControlID, CControllerObj *pControl );

};



// 사용자 메뉴
class NMineUserDlg : public NStrTabDlg
{
public:
	enum {
		EDIT_RENT_FEE,
		EDIT_RENT_PERIOD,

		EDIT_CONTRACT,

		BUTTON_APPLY,
		BUTTON_CANCEL_APPLY
	};
	NMineUserDlg();
	virtual ~NMineUserDlg();

	void InitDlg();

	void Action();

	void ShowRentContractInUser();

	void NDlgTabProc( UINT nEvent, int nControlID, CControllerObj *pControl );

	void RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen );
};


// 광산 행수 메뉴 
class NMineChiefDlg : public NStrTabDlg
{
public:
	enum {
		EDIT_RENT_FEE,
		EDIT_RENT_PERIOD,

		EDIT_EXPLAIN,

		LIST_RENTLIST,
		LIST_APPLYLIST,

		BUTTON_SETUP,

		BUTTON_REFRESH_RENTLIST,
		BUTTON_REFRESH_APPLYLIST,

		BUTTON_RENT,
		BUTTON_CLEAR_LIST
	};

	NMineChiefDlg();
	virtual ~NMineChiefDlg();

	void InitDlg();

	void Action();

	void ShowRentContract();
	void ShowMineUnitListInChief();
	void ShowMineApplyerListInChief();
	void ShowSelectedMineUnitExplain( int index );

	void NDlgTabProc( UINT nEvent, int nControlID, CControllerObj *pControl );
};



#endif