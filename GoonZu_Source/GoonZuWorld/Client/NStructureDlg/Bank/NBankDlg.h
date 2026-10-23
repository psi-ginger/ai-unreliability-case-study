#ifndef _NBANKDLG_H
#define _NBANKDLG_H

#include "../StrBaseDlg/StrBaseDlg.h"
#include "../../CommonLogic/Bank/Bank.h"
#include "../../CommonLogic/Bank/Bank-Manager.h"

// 전장 기본 다이얼로그 
class NBankDlg : public NStrBaseDlg
{
public:
	NBankDlg();
	virtual ~NBankDlg();

	void Create();

	void GreetingMsg();

	void RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen );
	
};

// 전장 입,출금 탭 다이얼로그
class NBankUserDlg : public NStrTabDlg
{
public:
	enum {
		EDIT_CREATE_VILLAGE,
		EDIT_ACCOUNTNUM,
		EDIT_BANK_MONEY,

		EDIT_CURRENT_MONEY,
		
		EDIT_MONEY,
		EDIT_MONEY_UNIT,

		EDIT_FEE,
		EDIT_FEERATE,
		
		BUTTON_INPUT_ALL,
		BUTTON_INPUT,
		BUTTON_OUTPUT,
		BUTTON_OUTPUT_ALL,
		BUTTON_CREATE_BANKBOOK,
		BUTTON_DELETE_BANKBOOK
	};


	NBankUserDlg();
	virtual ~NBankUserDlg();

	void InitDlg();
	void Action();

	void ShowAccountInfo();

	void NDlgTabProc( UINT nEvent, int nControlID, CControllerObj *pControl );

	void RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen );

};

// 전장 정보 탭 다이얼로그 
class NBankInfoDlg : public NStrTabDlg
{
public:
	enum {
		EDIT_STRDURA,
		EDIT_CHIEF,
		EDIT_MONEY,
		EDIT_TOTAL_INPUT_MONEY,
		EDIT_PROFIT,
		LIST_TAXLIST,
	};

	NBankInfoDlg();
	virtual ~NBankInfoDlg();

	void InitDlg();
	void Action();

	void ShowBankInfo();

	void NDlgTabProc( UINT nEvent, int nControlID, CControllerObj *pControl );

public:
	BOOL m_bShowTaxInfo_local;
};

// 전장 행수 탭 다이얼로그
class NBankChiefDlg : public NStrTabDlg
{
public:
	enum {
		EDIT_FEERATE,
		BUTTON_SETFEERATE
	};
	NBankChiefDlg();
	virtual ~NBankChiefDlg();

	void InitDlg();

	void NDlgTabProc( UINT nEvent, int nControlID, CControllerObj *pControl );
};

#endif