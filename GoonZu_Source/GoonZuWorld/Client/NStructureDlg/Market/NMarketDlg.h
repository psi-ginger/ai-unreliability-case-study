#ifndef _NMARKETDLG_H
#define _NMARKETDLG_H


#include "../../InterfaceMgr/DialogController.h"
#include "../StrBaseDlg/StrBaseDlg.h"

class NStrInventory;
class CComboBox;
class cltItem;

class NMarketDlg : public NStrBaseDlg
{

public:
	NMarketDlg();
	virtual ~NMarketDlg();

	void Create();

	void GreetingMsg();
	
	void DrawDialogLast( SI32 ScreenRX, SI32 ScreenRY  );

	void RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen );

	void VDirectDrawToBackScreen( LPDIRECTDRAWSURFACE7 lpBackScreen, SI32 ScreenRX, SI32 ScreenRY );

	void NDlgProc( UINT nEvent, int nControlID, CControllerObj* pControl );

};

// 객주 물품 팔기
class NMarketUserSellDlg : public NStrTabDlg
{
public:
	enum {
		LISTVIEW_MYINVENTORY,
		LISTVIEW_MARKETSTORAGE,

		EDIT_AMOUNT,
		EDIT_PRICE,
		
		EDIT_FEERATE,
		EDIT_FEE,

		EDIT_MONEY_UNIT,

		BUTTON_SELL,
		BUTTON_CANCEL_ORDER
		
		,BUTTON_SEARCHPRICE

		,EDIT_ITEMPRICE1
		,EDIT_ITEMPRICE2
		,EDIT_ITEMPRICE3
		,EDIT_ITEMPRICE4
		,EDIT_BACKGROUND

		,STATIC_TOTALNUM
		,STATIC_MONEYUNIT
	};

	NMarketUserSellDlg();
	virtual ~NMarketUserSellDlg();

	void InitDlg();

	void Action();

	void ShowFeeRate();
	void ShowFee( SI32 itemnum, GMONEY price );

	void SetStgItemPrice(cltMarketAccount * pclMarketAccount);

	void SetScrollStgItemPrice( void );

	void NDlgTabProc( UINT nEvent, int nControlID, CControllerObj *pControl );

	void RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen );

public:
	NStrInventory*	m_pMyInventory;
	NStrInventory*	m_pStgItem;

	BOOL			m_bMyInven;

};



// 객주 물품 사기
class NMarketUserBuyDlg : public NStrTabDlg
{

	SI32	m_siImageIndex;
	SI32	m_siImageType;

public:
	enum {
		STATIC_PRICECOND,

		CHECK_PRICECOND,

		COMBO_ITEMTYPE,
		COMBO_ITEM,

		BUTTON_REFRESH,
		BUTTON_BUY,
		BUTTON_ITEMMALL,

		EDIT_AMOUNT,
		EDIT_PRICECOND,
		EDIT_ITEMPRICE,
		EDIT_EXPLAIN,

		IMAGESTATIC_ITEMBACK,

		LIST_ORDERLIST,

		EDIT_EXPLAINCHAR ,
		STATIC_ITEMNAME,
		STATIC_ITEMTYPE,
		STATIC_ITEMINFO,
		STATIC_CHARINFO
	};

	NMarketUserBuyDlg();
	virtual ~NMarketUserBuyDlg();

	void InitDlg();

	void Action();

	void ShowPrice();

	void RequestMarketOrderList( SI32 itemunique );

	void ShowRequestMsg();

	BOOL ShowResponseOrder();
	void RefreshOrder();
	void VDirectDrawToBackScreen( LPDIRECTDRAWSURFACE7 lpBackScreen, SI32 ScreenRX, SI32 ScreenRY );
	BOOL ShowItemListInCombo( CComboBox *pCombo, SI32 itemtypeunique, bool btradeinterface );

	void NDlgTabProc( UINT nEvent, int nControlID, CControllerObj *pControl );

	void DrawDialogLast( SI32 ScreenRX, SI32 ScreenRY );
	//BOOL GetExplain(cltItem* pclitem, TCHAR* ptext, bool numbswitch = true);

	void ShowMyCharInfo( SI32 itemunique );

public:
	DWORD dwRequestTick;
	BOOL  bRequest;
	bool  bSelectedSwitch;
	bool  bMakerSwitch;

};




// 객주 행수 메뉴
class NMarketChiefDlg : public NStrTabDlg
{
public:
	enum {
		EDIT_FEERATE,
		BUTTON_SETFEERATE
	};

	NMarketChiefDlg();
	virtual ~NMarketChiefDlg();

	void InitDlg();

	void NDlgTabProc( UINT nEvent, int nControlID, CControllerObj *pControl );
};


// 객주 물품 사기
class NMarketUserBuyDlgSub :public CDialogController
{
	SI32	m_siImageIndex;
	SI32	m_siImageType;

	bool    m_InputDelay;
public:
	
	NMarketUserBuyDlgSub();
	virtual ~NMarketUserBuyDlgSub();

	void Create();

	static void CALLBACK StaticCallBackDialogNMarketUserBuyDlgSub( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl );
	void CALLBACK CallBackDialogNMarketUseBuyDlgSub( UINT nEvent, int nControlID, CControllerObj* pControl );


	void Action();

	void ShowPrice();

	void RequestMarketOrderList( SI32 itemunique );

	void ShowRequestMsg();

	BOOL ShowResponseOrder();
	void RefreshOrder(CComboBox *pCombo);
	void VDirectDrawToBackScreen( LPDIRECTDRAWSURFACE7 lpBackScreen, SI32 ScreenRX, SI32 ScreenRY );
	BOOL ShowItemListInCombo( CComboBox *pCombo, SI32 itemtypeunique, bool btradeinterface );

	void DrawDialogLast( SI32 ScreenRX, SI32 ScreenRY );

	void ShowMyCharInfo( SI32 itemunique );

	void SetProductListFromUnique(SI32 itemunique);
	
	bool IsCheckItem(cltItem* _pclItem);
	void InputRefreshOrder(TCHAR* ItemName);
	void InputAction(void);
	SI32 GetTextOneSize(void);				// 국가별 문자 하나의 텍스트 사이즈. 예>한국은 2바이트....

	void ShowItemMaterial(CComboBox *pCombo, SI32 siItemUnique);		// 해당 아이템의 재료리스트를 보여준다.

public:
	DWORD dwRequestTick;
	BOOL  bRequest;
	bool  bSelectedSwitch;
	bool  bMakerSwitch;

	InterfaceFactory m_InterfaceFactory;

};


#endif