#ifndef _PERSONALSHOPDLG_H_
#define _PERSONALSHOPDLG_H_

#include "PersonalShopBaseInfo.h"

class CEachPersonTradeOrderInfo;
class CLibListMgr;


class CPersonalShopDlg
{
public:
	CPersonalShopDlg();
	~CPersonalShopDlg();

	void Initialize();
	void Destroy();

	void Create( HINSTANCE hInst, HWND hWnd );

	static	BOOL CALLBACK StaticPersonalShopDlgProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam );
	BOOL	CALLBACK PersonalShopDlgProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam );

	
	bool IsShow();
	void Show( SI32 siCharUnique, SI32 siPersonID, SI16 ShopMode, char *Name, char *AccountID, BOOL bShopStarted);
	void Hide();
	
	void ShopInit(BOOL bSelf, SI16 shopmode);
	
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
	void DisplayMessage(SI32 siSuccess, char *pBuffer);

	void StartPersonalShop();
	void StopPersonalShop();
	void ShowMoneyText();
	void SetMakeableItemList(SI32 DlgItem);
		
public:
	bool			m_bShow;
	bool			m_bTimer;


	HWND			m_hDlg;
	HWND			m_hUserInventory;
	HWND			m_hUserShopShowcase;
	HBRUSH			m_hEditBrush;		
	
	BOOL			m_bSelf;
	SI32			m_siShopMode;
	BOOL		    m_bShopStarted;  
	BOOL			m_bShopMoneyCheck;


	cltPItemInfo	*m_pPersonItemInfo;
	cltItem			m_clUserItem[ MAX_ITEM_PER_PERSON ];

	BOOL				m_bShopDataChanged;
	char				m_szShopBanner[MAX_PERSONAL_SHOP_TEXT_SIZE];
	
	cltShopTradeData	m_clShopData;

	SI32			m_siShopperCharUnique;
	SI32			m_siShopperPersonID;
	char			m_szAccountID[MAX_PLAYER_NAME];
	char			m_szCharName[MAX_PLAYER_NAME];
	
	SI32			m_siUserInventorySelectedRealItemIndex;
	SI32			m_siUserInventorySelectedIndex;
	SI32			m_siShopShowCaseSelectedRealItemIndex;
	SI32			m_siShopShowCaseSelectedIndex;

};

#endif