#ifndef _TRADEMERCHANTHORSE_H_
#define _TRADEMERCHANTHORSE_H_

#include <Directives.h>
#include <windows.h>

class CTradeMerchantHorse
{
public:

	HWND m_hDlg;

	CTradeMerchantHorse();
	~CTradeMerchantHorse();

	void Create( HINSTANCE hInst, HWND hWnd );

	void LoadHorse();

	static	BOOL CALLBACK StaticTradeMerchantHorseDlgProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam );
	BOOL CALLBACK TradeMerchantHorseDlgProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam );

	bool IsShow();
	void Show( SI32 NPCCharID );
	void Hide();

	void SetHorsePrice( SI16 HorseUnique, SI32 HorsePrice );
	void SellToNPCSuccess();

private:
	SI32 m_siNPCKind;

	HBRUSH m_hEditBrush;

	bool m_bShow;

	HBITMAP m_hPortraitBitmap;
	
	HBITMAP m_hHorsePortraitBitmap;
	
	SI32 m_siHorsePrice;
	
	
};

#endif