/*
#ifndef _TRADEMERCHANTHAMELDLG_H_
#define _TRADEMERCHANTHAMELDLG_H_

#include <Directives.h>
#include <windows.h>
#include "../../Common/Ship/Ship.h"

class CTradeMerchantHamelDlg
{
public:
	HWND m_hDlg;

	CTradeMerchantHamelDlg();
	~CTradeMerchantHamelDlg();

	void Init();
	void Create( HINSTANCE hInst, HWND hWnd );

	void LoadShipList();

	static	BOOL CALLBACK StaticTradeMerchantHamelDlgProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam );
	BOOL CALLBACK TradeMerchantHamelDlgProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam );

	bool IsShow();
	void Show( SI32 NPCCharID );
	void Hide();
	
	void Set( SI32 *ShipKindList, SI32 *ShipPriceList );
	void SellToNPCSuccess( SI16 ShipKind, SI16 VillageUnique, SI16 PortUnitSlot, SI32 ShipPrice );

private:
	SI32 m_siNPCKind;
	SI32 m_siShipID;

	SI32 m_siSelectedIndex;
	
	SI32 m_siShipKindList[ MAX_SHIP_KIND_NUMBER ];
	SI32 m_siShipPriceList[ MAX_SHIP_KIND_NUMBER ];

	SI32 TotalPrice[3];
	SI32 OriginalDurability[3];
	SI32 MyDurability[3];
	SI32 MyShipPrice[3];
	SI32 MyShipNameKind[3];

	
	HBRUSH m_hEditBrush;

	bool m_bShow;

	HBITMAP m_hPortraitBitmap;
	
	
};

#endif
*/