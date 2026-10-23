#ifndef _ITEMINVENTORY_H_
#define _ITEMINVENTORY_H_

#include "ItemInventoryBaseDefinition.h"

class	CItemInventoryBuffer;
	
class	cltPItemInfo;

class CUserInventoryDlg;
class CHorseInventoryDlg;
class CBagInventoryDlg;

class cltSummonInfo;

class CItemInventory  
{
public:
	CItemInventory();
	~CItemInventory();

	void	Create( HWND hWnd, HINSTANCE hInst, cltPItemInfo *pPersonItemInfo );

	void	Initialize();
	void	Destroy();	

	static BOOL CALLBACK StaticItemInventoryDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
	BOOL CALLBACK ItemInventoryDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);


	static	BOOL CALLBACK StaticNotifyMsgInputDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
	BOOL	CALLBACK NotifyMsgInputDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);

	
	void	LoadDataAndSetting( HWND hDlg );
	void	LoadSummonInfo( HWND hDlg );

	
	void	RepairItem( SI32 siSelectedInventoryItemIndex );
	void	InventorySelect( SI32 siSelectedInventoryItemIndex, HWND hDlg );

	void	UseItem( SI16 si16Pos );

	void	DropItem( SI16 si16Pos );
	void	MountItem( SI16 si16Pos );
	
	void	ExchangeWeapon();



	void	MouseReleaseOperation();
	void	MoveItem( HWND hDlg, SI32 siDestination );
	void	MoveInventoryItem( HWND hDlg );
	
	bool	IsChanged();

	bool	IsSummonInfoChanged();

	void	Show();
	void	Hide();

	bool	IsShow();

	void	SetRepairMode( bool bRepairMode );


	bool	IsSameBeforeOpenedToolTipIndex( SI32 siBeforeOpenedToolTipIndex );
	
	bool	Pop( stMoveData * pMoveData );

	bool	Pop( char* pMoveData );

	void	DrawWeaponBackGroundImage( SI16 SelectedWeapon );

	void	ShowTabDlg();
	void	ShowSummonTabDlg();
	
	void	InsertTabDialog( HWND hTab, SI32 Index, char *TabDlgName );
	void	AdjustTabRect();
	
	HWND					m_hDlg;
	
	CUserInventoryDlg		*m_pUserInventoryDlg;
	CHorseInventoryDlg		*m_pHorseInventoryDlg;
	CBagInventoryDlg		*m_pBagInventoryDlg;

	SI16	GetCurrentSummonIndex()
	{
		return m_siCurrentSummonIndex;
	}

private:
	
	cltPItemInfo			*m_pPersonItemInfo;
	cltItem					m_clItem[ MAX_ITEM_PER_PERSON ];
	
	CItemInventoryBuffer	*m_pBuffer;

	HWND					m_hTabCtrl;
	
	HWND					m_hTabCtrlHeroSelect;
	
	HWND					m_hTip;
	
	HWND					m_hWeapon1;
	HWND					m_hWeapon2;
	HWND					m_hHelmet;
	HWND					m_hHat;
	HWND					m_hRing1;
	HWND					m_hRing2;
	HWND					m_hBelt;
	HWND					m_hNeck;
	HWND					m_hShoes;
	HWND					m_hArmor;
	HWND					m_hDress;
	HWND					m_hMantle;
	HWND					m_hBag;
	HWND					m_hMoveToHorse;
	HWND					m_hMoveToBag;

	HBITMAP					m_hItemImage;
	
	HBITMAP					m_hEmptyBitmap;	
	
		
	bool					m_bShowFlag;

	

	SI32					m_siSelectedInventoryItemIndex;
	SI32					m_siUpgradeInventoryItemIndex;		// 무기 제련
	SI32					m_siRealItemIndex;

	
	HINSTANCE				m_hInst;

	//HWND					m_hUserInventory;

	SI32					*m_pMakeableItemList;
	SI32					m_siMakeableItemNum;
	
	SI32					m_siMakeItemComboBoxSelectedIndex;
	//cltItem					clItem[20];
	
	SI32					m_si32ImageListIndex;
	bool					m_bIsListRotate;

	bool					m_bSelectPerson;
	bool					m_bSelectBag;
	
	
	HBITMAP					m_hTabImageBitmap[2];
	

	bool					m_bHorseHave;
	bool					m_bBagHave;

	cltSummonInfo			*m_pSummonInfo;
	SI16					m_siCurrentSummonIndex;
	
	//SI16					m_siSelectedWeapon;
};






#endif

