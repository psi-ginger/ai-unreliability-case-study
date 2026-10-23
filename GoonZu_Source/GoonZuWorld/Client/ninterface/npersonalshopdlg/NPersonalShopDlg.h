/* ==========================================================================
클래스 :		CNPersonalShopDlg

작성일 :		05/03/31
작성자 :		정용래

변경사항 :		없음

차후작업 :		


========================================================================*/
#include "NPersonalShopBaseInfo.h"

#pragma once

#include "../../Client/client.h"
#include "../../InterfaceMgr/DialogController.h"

class CEachPersonTradeOrderInfo;
class CLibListMgr;

class CButton;
class CEdit;
class CImageStatic;
class CList;
class CListView;
class CComboBox;
class CRadioButton;
class CStatic;
class COutLine;

//class CNInventory;

class CNPersonalShopDlg : public CDialogController
{
public:
	enum{ SPECIAL_PREMIUN_TICKET_UNIQUE = 24116, };
	enum
	{
		SHOP_TYPE_NORMAL,
		SHOP_TYPE_PREMIUM,
		SHOP_TYPE_SPECIAL_PREMIUM
	};
	enum
	{
		BIG_DISPLAY_MODE_DELTA_X = 16,
		SMALL_DISPLAY_MODE_DELTA_Y = 80,        
		DLG_WIDTH = 0,	// DDF 파일에서 읽어서 처리
		DLG_HEIGHT = 0,	// DDF 파일에서 읽어서 처리
		DLG_BIG_POS_X = 1024 - DLG_WIDTH + BIG_DISPLAY_MODE_DELTA_X,
		DLG_BIG_POS_Y = 80,
		DLG_SMALL_POS_X = 800 - DLG_WIDTH + SMALL_DISPLAY_MODE_DELTA_Y,
		DLG_SMALL_POS_Y = 80,
	};

	CNPersonalShopDlg();
	~CNPersonalShopDlg();

	void Initialize();
	void Destroy();

	void Create();

	static	void CALLBACK StaticNPersonalShopDlgProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
	void	CALLBACK NPersonalShopDlgProc( UINT nEvent, int nControlID, CControllerObj* pControl );


	bool IsShow();
	void Show( SI32 siCharUnique, SI32 siPersonID, SI16 ShopMode, TCHAR *Name, TCHAR *AccountID, BOOL bShopStarted );
	void Hide();
	void Action();
	void RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen );

	// From cltItemManagerClient 에서 가져옴
	BOOL SetItemTypeInCombo( bool bshowall);
	void SetNInventoryInListView();

	void ShopInit(BOOL bSelf, SI16 shopmode);
	BOOL ShowItemListInCombo( SI32 itemtypeunique, bool btradeinterface);

	bool IsUserInventoryChanged();
	bool IsShopShowCaseChanged();
	void LoadUserInventory();
	void LoadShopShwocase();
	void Do_Button_reg_buy_sell();

	bool AddShopBuyItem();
	bool AddShopSellItem(SI32 siUserInventoryRealIndex);
	bool AddShopMakeItem();
	bool DropShopItem(SI32 siShopShowCaseSelectedIndex);
	bool SellShopItem(SI32 siUserInventoryRealIndex);
	bool BuyShopItem();
	bool MakeShopItem();
	void DisplayMessage(SI32 siSuccess, TCHAR *pBuffer);
	void RefreshSelectedItem();

	void StartPersonalShop();
	void StopPersonalShop();
	void ShowMoneyText();
	void SetMakeableItemList();

	void SetItemImageData(SI32 ItemUnique);
	void VDirectDrawToBackScreen( LPDIRECTDRAWSURFACE7 lpBackScreen, SI32 ScreenRX, SI32 ScreenRY );
	
	void NormalShopInit();
	void PremiumShopInit();
	void HidePremiumInterface();
	void NormalShopInitNew();		//[진성] 일반 선택시 새로운 초기화 함수.
	void PremiumShopInitNew();		//[진성] 프리미엄 선택시 새로운 초기화 함수.	
	
	bool AddSpecialShopSellItem(SI32 siUserInventoryRealIndex);			//[진성] 밀봉 프리미엄. 리스트 선택시 아이템 등록에 사용하는 함수.
	void ShopDataClear();												//[진성] 밀봉 프리미엄. 리스트 선택시 등록한 모든 아이템 제거.
	void StartPersonalSpecialShop();									//[진성] 밀봉 프리미엄. 상점 시작 버튼.
	void SendServermessgeSpecialShop(BYTE *pData);						//[진성] 밀봉 프리미엄. 관련 서버에 패킷 보내는 함수.
	void SetNInventoryInListViewSpecialPremium();						//[진성] 밀봉 프리미엄. 일때 내 인벤에서 밀봉 아닌 장비만 세팅.
	bool IsItemType(cltItem* _pItem);									//[진성] 밀봉 프리미엄. 아이템이 밀봉 프리미엄 티켓으로 거래가 가능한지 검사.
	void SetSpecialPremiumListview(SI32 _Index, cltItem* _ItemUnique);	//[진성] 밀봉 프리미엄. 리스트 뷰에 티켓과 거래 아이템을 담는다.
	
	void ShowPremiumMode( bool bPremium );
public:

	COutLine			*m_pOutLine_test;

	CImageStatic		*m_pImageStatic_Item;
	CImageStatic		*m_pImageStatic_profile	;
	CEdit				*m_pEdit_explain	;
	CButton				*m_pButton_minihome	;
	CStatic				*m_pStatic_shopitem	;
	CList				*m_pList_shop_showcase	;
	//	CEdit				*m_pEdit_item_explain	;
	CStatic				*m_pStatic_user_item	;
	CListView			*m_pListView_user_inventory	;
	CButton				*m_pButton_item_cancel	;
	CStatic				*m_pStatic_shop_setting	;
	CRadioButton		*m_pRadioButtonButton_sell	;
	CRadioButton		*m_pRadioButtonButton_buy	;
	CRadioButton		*m_pRadioButtonButton_makeitem	;
	CStatic				*m_pStatic_handpoint	;
	CComboBox			*m_pComboBox_iteminfo_type	;
	CEdit				*m_pEdit_handpoint	;
	CEdit				*m_pEdit_skillpoint	;
	CStatic				*m_pStatic_skillpoint	;
	CStatic				*m_pStatic_item_name	;
	CComboBox			*m_pComboBox_iteminfo_name	;
	CStatic				*m_pStatic_price	;
	CEdit				*m_pEdit_price	;
	CStatic				*m_pStatic_count	;
	CStatic				*m_pStatic_amount	;
	CEdit				*m_pEdit_amount	;
	CStatic				*m_pStatic_won	;
	CButton				*m_pButton_reg_buy_sell	;
	CEdit				*m_pEdit_message	;
	CButton				*m_pButton_start	;
	CButton				*m_pButton_stop	;
	CButton				*m_pButton_close	;
	CComboBox			*m_pComboBox_makeitem_name	;
	CStatic				*m_pStatic_money	;
	CStatic				*m_pStatic_item_type	;
	CStatic				*m_pStatic_makeitem_name	;

	CButton				*m_pButton_item_search	;	// 개인 상점 - 물품 검색

	CRadioButton		*m_pRadioPremium;	// 프리미엄 개인상점  선택 라디오버튼
	CRadioButton		*m_pRadioNormal;	// 걍 개인상점  선택 라디오버튼

	CStatic				*m_pShopTitle_Static;
	CEdit				*m_pShopTitle_Edit;

	CComboBox*			m_pCombobox_personshop_kind;	//[진성] 밀봉 프리미엄. 상점 종류 콤보박스 추가.=> 2008-3-25
	CListView*			m_plistview_ticket;
	CListView*			m_plistview_item; 
	CStatic*			m_pimagestatic_premium_back;

		


	bool			m_bShow;
	bool			m_bTimer;

	//HWND			m_hUserInventory;
	//HWND			m_hUserShopShowcase;
	//HBRUSH			m_hEditBrush;		
	cltItem		*m_pclInventoryItem;

	BOOL			m_bSelf;
	SI32			m_siShopMode;
	BOOL		    m_bShopStarted;  
	BOOL			m_bShopMoneyCheck;	// 물건살때 체크


	cltPItemInfo	*m_pPersonItemInfo;
	cltItem			m_clUserItem[ MAX_ITEM_PER_PERSON ];

	BOOL				m_bShopDataChanged;
	TCHAR				m_szShopBanner[MAX_PERSONAL_SHOP_TEXT_SIZE];

	SI32				m_siShopTradeIndex[MAX_SHOP_ITEM_NUMBER];	

	cltShopTradeData	m_clShopData;

	SI32			m_siShopperCharUnique;
	SI32			m_siShopperPersonID;
	TCHAR			m_szAccountID[MAX_PLAYER_NAME];
	TCHAR			m_szCharName[MAX_PLAYER_NAME];

	SI32			m_siUserInventorySelectedRealItemIndex;
	SI32			m_siUserInventorySelectedIndex;
	SI32			m_siShopShowCaseSelectedRealItemIndex;
	SI32			m_siShopShowCaseSelectedIndex;

	SI32			m_siImageType;
	SI32			m_siImageIndex;

};
