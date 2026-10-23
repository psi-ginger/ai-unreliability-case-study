#ifndef _TRADEMERCHANTDISH_H_
#define _TRADEMERCHANTDISH_H_

#include <Directives.h>
#include <windows.h>

class cltItem;

class CTradeMerchantDish
{
public:

	HWND m_hDlg;

	CTradeMerchantDish();
	~CTradeMerchantDish();

	void Create( HINSTANCE hInst, HWND hWnd );

	bool IsInventoryChanged();
	void LoadDish();

	static	BOOL CALLBACK StaticTradeMerchantDishDlgProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam );
	BOOL CALLBACK TradeMerchantDishDlgProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam );

	bool IsShow();
	void Show( SI32 NPCCharID );
	void Hide();
	
	void SetDishPrice( SI16 DishItemUnique, SI32 DishPrice );
	void SellDish( SI16 DishItemUnique );
	

private:

	HBRUSH m_hEditBrush;

	bool m_bShow;

	HBITMAP m_hPortraitBitmap;

	SI32 m_siSelectedIndex;
	cltItem *m_pclItem;

	SI32 m_siItemPrice;

	
	
};

#endif