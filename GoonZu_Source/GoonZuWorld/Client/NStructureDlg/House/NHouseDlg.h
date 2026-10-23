#ifndef _NHOUSEDLG_H
#define _NHOUSEDLG_H

#include "../StrBaseDlg/StrBaseDlg.h"
#include "../../CommonLogic/House/House.h"

class NStrInventory;

class NHouseDlg : public NStrBaseDlg
{
public:
	NHouseDlg();
	virtual ~NHouseDlg();

	void Create();

	void RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen );

	void GreetingMsg();

};

// 시전 목록
class NHouseListDlg : public NStrTabDlg
{
public:
	enum {
		LIST_HOUSELIST,
		BUTTON_ENTER,
		BUTTON_CANCELLATION_HOUSE
	};

	NHouseListDlg();
	virtual ~NHouseListDlg();

	void InitDlg();
	
	void Action();
	
	
	void ShowHouseUnitList();

	void NDlgTabProc( UINT nEvent, int nControlID, CControllerObj *pControl );

};

// 새로운 시전 목록 - 내창고 목록 추가 (PCK :08.07.10)
class NHouseNewListDlg : public NStrTabDlg
{
public:
	enum {
		LIST_HOUSELIST,
		BUTTON_ENTER,
		BUTTON_CANCELLATION_HOUSE,
		LIST_MYHOUSELIST,
		BUTTON_CHANGEHOUSENAME,
		BUTTON_EXTENSION
	};

	NHouseNewListDlg();
	virtual ~NHouseNewListDlg();

	void InitDlg();

	void Action();


	void ShowHouseUnitList();

	void NDlgTabProc( UINT nEvent, int nControlID, CControllerObj *pControl );

};

// 사용자 메뉴
class NHouseUserDlg : public NStrTabDlg
{
public:
	enum {
		EDIT_RENT_FEE,
		EDIT_RENT_PERIOD,

		EDIT_CONTRACT,

		BUTTON_APPLY,
		BUTTON_CANCEL_APPLY		
	};
	NHouseUserDlg();
	virtual ~NHouseUserDlg();

	void InitDlg();

	void Action();

	void ShowRentContractInUser();

	void NDlgTabProc( UINT nEvent, int nControlID, CControllerObj *pControl );

	virtual void Show();
};


// 시전 행수 메뉴 
class NHouseChiefDlg : public NStrTabDlg
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

	NHouseChiefDlg();
	virtual ~NHouseChiefDlg();

	void InitDlg();

	void Action();

	void ShowRentContract();
	void ShowHouseUnitListInChief();
	void ShowHouseApplyerListInChief();
	void ShowSelectedHouseUnitExplain( int index );

	void NDlgTabProc( UINT nEvent, int nControlID, CControllerObj *pControl );
};



#endif