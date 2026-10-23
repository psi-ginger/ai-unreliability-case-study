#ifndef _NSHIPMARKETDLG_H
#define _NSHIPMARKETDLG_H

#include "../StrBaseDlg/StrBaseDlg.h"
#include "../../../CommonLogic/ShipMarket/ShipMarket.h"

class NShipMarketDlg : public NStrBaseDlg
{
public:
	NShipMarketDlg();
	virtual ~NShipMarketDlg();

	void Create();

	void GreetingMsg();

	void RetMsgBox( bool RetValue, SI16 MsgIndex, char *pData, SI16 DataLen );

};

// 선박 구입
class NShipMarketBuyDlg : public NStrTabDlg
{
public:
	enum {
		BUTTON_BUY,
		BUTTON_REFRESH,
		BUTTON_SELECTORDER	,
		LISTBOX_SHIP,
		LISTBOX_EXPLAIN,
	};

	NShipMarketBuyDlg();
	virtual ~NShipMarketBuyDlg();

	void InitDlg();
	void Action();

	void ShowResponseOrder();
	void ShowResponseExplain();

	void NDlgTabProc( UINT nEvent, int nControlID, CControllerObj *pControl );

};

// 선박 팔기
class NShipMarketUserDlg : public NStrTabDlg
{
public:
	enum {
		BUTTON_SELL,
		BUTTON_CANCELORDER,
		//BUTTON_SELECTSELL,
		EDITBOX_REALFEE,
		EDITBOX_SELLPRICE,
		EDITBOX_SELLPRICECHAR,
		EDITBOX_PLAYERNAME,
		LISTBOX_MYSHIP,
		LISTBOX_MYORDER,
		LISTBOX_DETAILINFO,
		STATICTEXT_FEERATE,
		CHECKBOX_SELECTSELL,
	};

	NShipMarketUserDlg();
	virtual ~NShipMarketUserDlg();

	void InitDlg();
	void Action();

	void ShowMyShipList();
	void ShowMyOrderList();
	void ShowMyShipInfo();
	void ShowMyOrderInfo();

	BOOL ShowFee();
	BOOL ShowRealFee(SI32 price);

	void NDlgTabProc( UINT nEvent, int nControlID, CControllerObj *pControl );

	void RetMsgBox( bool RetValue, SI16 MsgIndex, char *pData, SI16 DataLen );

};


// 전함사 행수
class NShipMarketChiefDlg : public NStrTabDlg
{
public:
	enum {
		EDIT_FEERATE,
		BUTTON_SETFEERATE
	};
	NShipMarketChiefDlg();
	virtual ~NShipMarketChiefDlg();

	void InitDlg();

	void NDlgTabProc( UINT nEvent, int nControlID, CControllerObj *pControl );
};


#endif
