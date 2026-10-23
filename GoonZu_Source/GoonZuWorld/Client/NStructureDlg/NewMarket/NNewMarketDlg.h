#ifndef _NNewMarket_H
#define _NNewMarket_H


#include "../../InterfaceMgr/DialogController.h"
#include "../StrBaseDlg/StrBaseDlg.h"

class NStrInventory;
class CComboBox;
class cltItem;

class NNewMarketDlg : public NStrBaseDlg
{

public:
	NNewMarketDlg();
	virtual ~NNewMarketDlg();

	void Create();

	void GreetingMsg();
	
	void DrawDialogLast( SI32 ScreenRX, SI32 ScreenRY  );

	void RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen );

	void VDirectDrawToBackScreen( LPDIRECTDRAWSURFACE7 lpBackScreen, SI32 ScreenRX, SI32 ScreenRY );

	void NDlgProc( UINT nEvent, int nControlID, CControllerObj* pControl );

};

// 객주 물품 팔기
class NNewMarketUserSellDlg : public NStrTabDlg
{
public:
	enum
	{
		 CREATE_BANK_MSGBOX = 1		// 은행 개설.
		,SELL_MSGBOX 				// 판매 등록.
		,RECALL_MSGBOX				// 판매 주문 아이템 회수.
	};

	enum
	{
		 LISTVIEW_MYINVENTORY
		,LISTVIEW_MARKETSTORAGE
		,EDIT_AMOUNT
		,EDIT_PRICE
		,EDIT_FEERATE
		,EDIT_FEE
		,EDIT_MONEY_UNIT
		,BUTTON_SELL
		,BUTTON_CANCEL_ORDER
		,BUTTON_SEARCHPRICE
		,EDIT_ITEMPRICE1
		,EDIT_ITEMPRICE2
		,EDIT_ITEMPRICE3
		,EDIT_ITEMPRICE4
		,EDIT_BACKGROUND
		,STATIC_TOTALNUM
		,STATIC_MONEYUNIT
	};

	NNewMarketUserSellDlg();
	virtual ~NNewMarketUserSellDlg();

	void 	InitDlg();
	void 	Action();
	void 	NDlgTabProc( UINT nEvent, int nControlID, CControllerObj *pControl );
	void 	RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen );

public:
	void	SetMyInventory( void );						// 내 인벤 정보 세팅.
	void	ShowFeeRate( void );						// 수수료 비율.
	void	ShowFee( SI32 _ItemNum, GMONEY _Price );	// 지불할 수수료.
	bool	IsSell( bool _bEditBoxClear = false );		// 아이템 판매 가능 여부.
	void	SetNewMarketAccount(cltGameMsgResponse_NewMarketAccount* pNewMarketAccount);
	void	RefreshSellInfo( void );
	void	SetStgItemPrice( void );
	void	SetScrollStgItemPrice( void );
	void	CancelOrder( void );
	void	SetStrInventory( void );
		
private:
	NStrInventory*	m_pMyInventory;
	NStrInventory*	m_pStgItem;

	float			m_fFeeRate; 
	SI32			m_siItemNum;
	GMONEY			m_Tax;
	GMONEY			m_Price; 

	cltNewMarketAccount	m_clNewMarketAccount;
	
};



// 객주 물품 사기
class NNewMarketUserBuyDlg : public NStrTabDlg
{
public:
	enum 
	{
		 STATIC_PRICECOND
		,CHECK_PRICECOND
		,COMBO_ITEMTYPE
		,COMBO_ITEM
		,BUTTON_REFRESH
		,BUTTON_BUY
		,BUTTON_ITEMMALL
		,EDIT_AMOUNT
		,EDIT_PRICECOND
		,EDIT_ITEMPRICE
		,EDIT_EXPLAIN
		,IMAGESTATIC_ITEMBACK
		,LIST_ORDERLIST
		,EDIT_EXPLAINCHAR 
		,STATIC_ITEMNAME
		,STATIC_ITEMTYPE
		,STATIC_ITEMINFO
		,STATIC_CHARINFO
	};

	NNewMarketUserBuyDlg();
	virtual ~NNewMarketUserBuyDlg();

	void InitDlg();

	void Action();

	void NDlgTabProc( UINT nEvent, int nControlID, CControllerObj *pControl );
	
public:
	

private:
	

};




// 객주 행수 메뉴
class NNewMarketChiefDlg : public NStrTabDlg
{
public:
	enum 
	{
		EDIT_FEERATE,
		BUTTON_SETFEERATE,
		BUTTON_SETFEERATE_DOWN,
		BUTTON_SETFEERATE_UP
	};

	NNewMarketChiefDlg();
	virtual ~NNewMarketChiefDlg();

	void	InitDlg();
	void	Action();

	void	NDlgTabProc( UINT nEvent, int nControlID, CControllerObj *pControl );

	float	m_fFeeRate; 
};


// 객주 물품 사기
class NNewMarketUserBuyDlgSub :public CDialogController
{
public:
	
	NNewMarketUserBuyDlgSub();
	virtual ~NNewMarketUserBuyDlgSub();

	void	Create( void );
	void	Action( void );
	void	Init( void );	

	static	void CALLBACK StaticCallBackDialogNNewMarketUserBuyDlgSub( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl );
			void CALLBACK CallBackDialogNNewMarketUseBuyDlgSub( UINT nEvent, int nControlID, CControllerObj* pControl );


public:
	BOOL 		ShowItemTypeListInCombo( void );															// 아이템 타입 리스트 만들기.
	BOOL 		ShowItemListInCombo( CComboBox *pCombo, SI32 itemtypeunique );								// 아이템 타입별 아이템 리스트 만들기.
	void 		VDirectDrawToBackScreen( LPDIRECTDRAWSURFACE7 lpBackScreen, SI32 ScreenRX, SI32 ScreenRY );// 아이템 리스트의 선택된 아이템 그리기.
	void 		InputAction( void );																			// 에디트 박스가 한글문자를 받을땐 메시지가 가질 않아서 인위로 문자 개수를 확인해 달라질때 마다 정보를 갱신하자.
	SI32 		GetTextOneSize( void );
	void 		ShowMyCharInfo( SI32 _siItemUnique );
	void 		ShowItemExpalan( SI32 _siItemUnique, SI32 _siServerUnique = 0 );
	void 		RefreshOrder( void );
	void 		SetProductListFromUnique( SI32 _siItemunique );
	void 		RequestNewMarketOrderList( SI32 itemunique );
	void		RequestNewMarketOrderListRefresh( void );
	void 		SetItemList( cltGameMsgResponse_NewMarketItemList* pNewMarketItemList );
	bool 		IsCheckItem( cltItem* _pclItem );
	void 		SetPage( SI32 MaxPage );
	void		RefreshItemExpalan( void );

public:
	InterfaceFactory	m_InterfaceFactory;

	SI32				m_siImageIndex;
	SI32				m_siImageType;

	SI16				m_siRequestItemUnique;
	SI16				m_siRequestPage;
	SI16				m_siMaxPage;

	SI16				m_siSelectOrderIndex;
	bool				m_bActionRequestItemList;
	
	NSafeTArray<cltNewTradeOrder, MAX_NEWMARKET_ITEMLIST> clNewTradeOrder;
	//cltNewTradeOrder	clNewTradeOrder[MAX_NEWMARKET_ITEMLIST]; 
};


#endif