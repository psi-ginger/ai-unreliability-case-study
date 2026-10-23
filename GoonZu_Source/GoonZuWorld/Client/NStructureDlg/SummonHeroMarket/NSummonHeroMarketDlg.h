#ifndef _NSUMMONHEROMARKETDLG_H
#define _NSUMMONHEROMARKETDLG_H

#include "../StrBaseDlg/StrBaseDlg.h"
#include "../../../CommonLogic/SummonHeroMarket/SummonHeroMarket.h"

class NSummonHeroMarketDlg : public NStrBaseDlg
{
public:
	NSummonHeroMarketDlg();
	virtual ~NSummonHeroMarketDlg();

	void Create();

	void GreetingMsg();

	void RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen );
};

// 소환영웅 구입
class NSummonHeroMarketBuyDlg : public NStrTabDlg
{
public:
	enum {
		IMAGESTATIC_PORTRAIT ,
		BUTTON_BUY	,
		BUTTON_FIRST	,
		BUTTON_NEXT	,
		BUTTON_SELECTORDER	,
		BUTTON_REFRESH,
		LISTBOX_SUMMON	,
		LISTBOX_EXPLAIN	,
		COMBOBOX_KIND	,
	};

	NSummonHeroMarketBuyDlg();
	virtual ~NSummonHeroMarketBuyDlg();

	void InitDlg();
	void Action();

	void AddSummonKind();	// combo setting
	void ShowResponseOrder();
	void ShowResponseExplain();
	void DrawSummonPortrait();

	void NDlgTabProc( UINT nEvent, int nControlID, CControllerObj *pControl );

};



// 사용자 메뉴 == 소환영웅 팔기
class NSummonHeroMarketUserDlg : public NStrTabDlg
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
		LISTBOX_MYSUMMON,
		LISTBOX_MYORDER,
		LISTBOX_DETAILINFO,
		STATICTEXT_FEERATE,
		CHECKBOX_SELECTSELL,
	};

	NSummonHeroMarketUserDlg();
	virtual ~NSummonHeroMarketUserDlg();

	void InitDlg();
	void Action();

	void ShowMySummonList();
	void ShowMyOrderList();
	void ShowMySummonInfo();
	void ShowMyOrderInfo();

	BOOL ShowFee();
	BOOL ShowRealFee(GMONEY price);

	void NDlgTabProc( UINT nEvent, int nControlID, CControllerObj *pControl );

	void RetMsgBox( bool RetValue, SI16 MsgIndex,BYTE *pData, SI16 DataLen );

};


// 비변사 행수
class NSummonHeroMarketChiefDlg : public NStrTabDlg
{
public:
	enum {
		EDIT_FEERATE,
		BUTTON_SETFEERATE
	};
	NSummonHeroMarketChiefDlg();
	virtual ~NSummonHeroMarketChiefDlg();

	void InitDlg();

	void NDlgTabProc( UINT nEvent, int nControlID, CControllerObj *pControl );
};


#endif
