#ifndef _NLANDDLG_H
#define _NLANDDLG_H

#include "../StrBaseDlg/StrBaseDlg.h"
#include "../../CommonLogic/Land/Land.h"

class NStrInventory;

class NLandDlg : public NStrBaseDlg
{
public:
	NLandDlg();
	virtual ~NLandDlg();

	void Create();

	void GreetingMsg();
	
	void RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen );
};

// 농장 목록
class NLandListDlg : public NStrTabDlg
{
public:
	enum {
		LIST_LANDLIST,
	};

	NLandListDlg();
	virtual ~NLandListDlg();

	void InitDlg();
	
	void Action();

	void ShowLandUnitList();

	void NDlgTabProc( UINT nEvent, int nControlID, CControllerObj *pControl );

};



// 사용자 메뉴
class NLandUserDlg : public NStrTabDlg
{
public:
	enum {
		EDIT_RENT_FEE,
		EDIT_RENT_PERIOD,

		EDIT_CONTRACT,

		BUTTON_APPLY,
		BUTTON_CANCEL_APPLY
	};
	NLandUserDlg();
	virtual ~NLandUserDlg();

	void InitDlg();

	void Action();

	void ShowRentContractInUser();

	void NDlgTabProc( UINT nEvent, int nControlID, CControllerObj *pControl );

	void RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen );
};


// 농장 행수 메뉴 
class NLandChiefDlg : public NStrTabDlg
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

	NLandChiefDlg();
	virtual ~NLandChiefDlg();

	void InitDlg();

	void Action();

	void ShowRentContract();
	void ShowLandUnitListInChief();
	void ShowLandApplyerListInChief();
	void ShowSelectedLandUnitExplain( int index );

	void NDlgTabProc( UINT nEvent, int nControlID, CControllerObj *pControl );
};



#endif