#ifndef _TRADEMERCHANTDLG_H_
#define _TRADEMERCHANTDLG_H_


#include <Directives.h>
#include <windows.h>

class CTradeMerchantNPC;
class cltItem;

class CTradeMerchantDlg
{
public:
	HWND m_hDlg;

	CTradeMerchantNPC *m_pTradeMerchantNPC;

	CTradeMerchantDlg();
	~CTradeMerchantDlg();

	void Init();

	void Create( HINSTANCE hInst, HWND hWnd );

	bool IsInventoryChanged();
	void LoadInventoryData();

	static	BOOL CALLBACK StaticTradeMerchantDlgProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam );
	BOOL CALLBACK TradeMerchantDlgProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam );

	bool IsShow();
	void Show( SI32 NPCCharID );
	void Hide();

	void Set( CTradeMerchantNPC *pTradeMerchantNPC );
	void LoadTradeMerchantNPCBuyData();
	
	void SellToNPCSuccess();

private:
	bool m_bShow;	
	SI32 m_siNPCKind;	

	HBITMAP m_hPortraitBitmap;
	cltItem	*m_pclItem;
	
	SI32 m_siSelectedIndex;
	
	HBRUSH m_hEditBrush;

};


#endif