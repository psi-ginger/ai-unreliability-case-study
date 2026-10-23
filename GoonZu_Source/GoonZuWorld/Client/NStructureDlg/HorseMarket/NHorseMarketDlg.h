#ifndef _NHORSEMARKETDLG_H
#define _NHORSEMARKETDLG_H

#include "../StrBaseDlg/StrBaseDlg.h"
#include "../../../CommonLogic/HorseMarket/HorseMarket.h"

class NHorseMarketDlg : public NStrBaseDlg
{
public:
	NHorseMarketDlg();
	virtual ~NHorseMarketDlg();

	void Create();

	void GreetingMsg();

	void RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen );
};

// 말 구입
class NHorseMarketBuyDlg : public NStrTabDlg
{
public:
	enum {
		BUTTON_BUY	,
		BUTTON_REFRESH,

		LISTBOX_HORSE,
		EDITBOX_EXPLAIN,

		COMBOBOX_KIND,
		COMBOBOX_PRICE,
	};

	NHorseMarketBuyDlg();
	virtual ~NHorseMarketBuyDlg();

	void InitDlg();
	void Action();

	void ShowResponseOrder();
	void ShowResponseExplain();

	void NDlgTabProc( UINT nEvent, int nControlID, CControllerObj *pControl );

};



// 말 팔기
class NHorseMarketUserDlg : public NStrTabDlg
{
public:
	enum {
		BUTTON_SELL,
		BUTTON_CANCELORDER,

		EDITBOX_REALFEE,
		EDITBOX_SELLPRICE,
		EDITBOX_SELLPRICECHAR,
		EDITBOX_EXPLAIN,
		
		EDITBOX_MYHORSE,
		EDITBOX_MYORDER,
		
		STATICTEXT_FEERATE,
	};

	NHorseMarketUserDlg();
	virtual ~NHorseMarketUserDlg();

	void InitDlg();
	void Action();

	void ShowMyHorseList();
	void ShowMyOrderList();

	void ShowMyHorseInfo();
	void ShowMyOrderInfo();

	BOOL ShowFee();
	BOOL ShowRealFee(GMONEY price);

	void NDlgTabProc( UINT nEvent, int nControlID, CControllerObj *pControl );

	void RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen );

};


// 사복시 행수 
class NHorseMarketChiefDlg : public NStrTabDlg
{
public:
	enum {
		EDIT_FEERATE,
		BUTTON_SETFEERATE
	};
	NHorseMarketChiefDlg();
	virtual ~NHorseMarketChiefDlg();

	void InitDlg();

	void NDlgTabProc( UINT nEvent, int nControlID, CControllerObj *pControl );
};


#endif