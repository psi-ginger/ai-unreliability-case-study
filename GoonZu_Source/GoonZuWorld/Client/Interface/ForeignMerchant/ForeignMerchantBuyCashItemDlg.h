#ifndef _FOREIGHMERCHANTBUYCASHITEMDLG_H_
#define _FOREIGHMERCHANTBUYCASHITEMDLG_H_

#include <Directives.h>
#include <windows.h>

class cltItem;
struct stItemInfoList;

class CForeignMerchantBuyCashItemDlg
{
public:
	HWND m_hDlg;

	CForeignMerchantBuyCashItemDlg();
	~CForeignMerchantBuyCashItemDlg();

	void Create( HINSTANCE hInst, HWND hWnd );

	bool IsUserInventoryChanged();
	void LoadUserInventory();
	
	static	BOOL CALLBACK StaticForeignMerchantBuyCashItemDlgProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam );
	BOOL CALLBACK ForeignMerchantBuyCashItemDlgProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam );

	bool IsShow();
	void Show();
	void Hide();

	void Set( stItemInfoList *ItemInfoList, SI16 ListNum );
	
private:

	HBRUSH m_hEditBrush;

	bool m_bShow;

	HBITMAP m_hPortraitBitmap;

	SI32 m_siSelectedIndex;
	SI32 m_siListBoxSelectedIndex;
	
	cltItem *m_pclItem;

	SI32 m_siItemPrice;

	SI16 *m_siItemUniqueList;
	UI16 *m_uiItemPriceList;
	SI16 m_siListNum;

	HBITMAP m_hItemBitmap;	
};

#endif