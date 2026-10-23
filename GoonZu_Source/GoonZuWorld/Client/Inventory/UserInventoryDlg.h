#ifndef _USERINVENTORYDLG_H_
#define _USERINVENTORYDLG_H_

#include "ItemInventoryBaseDefinition.h"

class cltPItemInfo;
class CItemInventoryBuffer;

class CUserInventoryDlg
{
public:
	CUserInventoryDlg();
	~CUserInventoryDlg();

	void Create( HWND hWnd, HINSTANCE hInst, cltPItemInfo *pPersonItemInfo, CItemInventoryBuffer *pBuffer, HWND hParentDlg );

	void Initialize();
	void Destroy();

	static BOOL CALLBACK StaticUserInventoryDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
	BOOL CALLBACK UserInventoryDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);

	bool	IsShow();
	void	Show();
	void	Hide();

	void	LoadInventoryData();
	bool	IsInventoryChanged();
	
	void	MouseReleaseOperation();
	
	void	MoveItem( HWND hDlg, SI32 siDestination );
	void	ChangeItem( HWND hDlg, SI32 siDestination );

	void	MoveInventoryItem( HWND hDlg );

	void	RepairItem( SI32 siSelectedInventoryItemIndex );
	void	InventorySelect( SI32 siSelectedInventoryItemIndex, HWND hDlg );

	void	UseItem( SI16 si16Pos );

	void	DropItem( SI16 si16Pos );
	void	MountItem( SI16 si16Pos );
	SI32    CheckIsInventory();
	SI32    GetInventoryType(SI32 type);
		


	HWND	GetHwnd();

	SI32	GetSelectedIndex();
	cltItem	*GetItemIndex( SI32 siIndex );
	
	HWND	m_hDlg;
private:
	
	SI32				m_siSelectedIndex;
	SI32				m_siUpgradeSelectedIndex;

	CItemInventoryBuffer *m_pBuffer;
	
	bool				m_bShow;

	HWND				m_hUserInventory;

	HWND				m_hParentDlg;

	cltPItemInfo		*m_pPersonItemInfo;
	cltItem				m_clItem[ MAX_ITEM_PER_PERSON ];
	HBITMAP				m_hItemImage;

	SI32				m_si32ImageListIndex;
	bool                m_siRingflag;
	

	bool				m_bIsListRotate;
		
	stImageList			m_stImageListMgr[ MAX_IMAGELIST_NUM ];

	HBITMAP				m_hPortraitBitmap;
	HBITMAP				m_hItemBitmap;

};

#endif