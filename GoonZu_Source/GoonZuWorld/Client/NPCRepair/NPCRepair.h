//---------------------------------
// 2003/3/11 ±èÅÂ°ï
//---------------------------------

#ifndef _NPCREPAIR_H
#define _NPCREPAIR_H

#include <Windows.h>
#include <Directives.h>


#include "..\Client\Inventory\ItemInventoryBaseDefinition.h"

#define NPCREPAIR_SETTIMER_VALUE	1000

class CItemListView;

class cltNPCRepairManager{
public:
	HWND m_hDlg;

	bool bDlgRectInitSwitch;
	RECT DlgRect;

	cltNPCRepairManager();
	~cltNPCRepairManager();

	BOOL CreateDlg(HINSTANCE hinst, HWND hwnd, SI32 CharUnique );

	static BOOL CALLBACK StaticDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
	BOOL CALLBACK DlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);


	bool IsChangedInvItem();
	void LoadItemData();

	void SetItemRepairPriceExplain( SI32 ItemUnique, SI32 ItemPrice );
	void SetReairSuccessMent( char *Text );

private:
	CItemListView	*m_pItemListView;
	cltItem			*m_pclUserInvItem;

	SI16			m_siInvSelectedIndex;
	SI16			m_siPreInvSelectedIndex;

	HBITMAP			m_hPortraitBitmap;
	
	SI32			m_siCharUnique;
	
	SI32			m_siItemRepairPrice;


};


#endif
