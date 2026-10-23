
#include "ItemInventory.h"

#include "ItemInventoryBuffer.h"

#include "..\..\CommonLogic\CommonLogic.h"

//-----------------------------
// Common
//-----------------------------
#include "PersonInfo.h"
#include "Char\CharManager\CharManager.h"


#include "..\..\Client\Client.h"

#include "UserInventoryDlg.h"
#include "HorseInventoryDlg.h"
#include "BagInventoryDlg.h"

#include "../Interface/ErrorMsgDlg/ErrorMsgDlg.h"
#include "../Interface/NotifyMsgDlg/NotifyMsgDlg.h"

#include "../Interface/MinigameDlg/MiniGameDlg.h"
#include "../MiniGame/MiniGameManager.h"

extern cltCommonLogic* pclClient;

CItemInventory::CItemInventory()
{
	m_pUserInventoryDlg = new CUserInventoryDlg();
	m_pHorseInventoryDlg = new CHorseInventoryDlg();
	m_pBagInventoryDlg = new CBagInventoryDlg();
	
	m_pMakeableItemList = NULL;
			
	m_si32ImageListIndex = 0;
	m_bIsListRotate = false;
	m_bSelectPerson = true;
	m_bSelectBag = false;

	m_hMoveToHorse = NULL;
	m_hMoveToBag = NULL;


	m_pBuffer = new CItemInventoryBuffer();
	
	m_siSelectedInventoryItemIndex = -1;
	m_siUpgradeInventoryItemIndex = -1;

	m_hTip = NULL;


	m_siMakeItemComboBoxSelectedIndex = -1;
	
	//m_siSelectedWeapon = -1;
	
	m_bHorseHave = false;
	m_bBagHave = false;

	m_pSummonInfo = new cltSummonInfo();
	memset( m_pSummonInfo, 0, sizeof( cltSummonInfo ) );
	m_siCurrentSummonIndex = -1;
	

	Initialize();
}

CItemInventory::~CItemInventory()
{
	Destroy();

	if ( m_hDlg )
	{
		DestroyWindow( m_hDlg );
	}
}

void CItemInventory::Create( HWND hWnd, HINSTANCE hInst, cltPItemInfo *pPersonItemInfo )
{
	m_pPersonItemInfo = pPersonItemInfo;

	m_hInst = hInst;

	m_hDlg = ::CreateDialog( hInst, MAKEINTRESOURCE(IDD_INVENTORY_DIALOG), hWnd, StaticItemInventoryDlgProc );

	Hide();
	
	return;
}

void CItemInventory::Initialize()
{
	m_bShowFlag = false;
	
	m_hItemImage  = NULL;
	
	return;
}

void CItemInventory::Destroy()
{
	if ( m_pSummonInfo )
	{
		delete m_pSummonInfo;
		m_pSummonInfo = NULL;
	}

	if ( m_pMakeableItemList )
	{
		delete [] m_pMakeableItemList;
		m_pMakeableItemList = NULL;
	}

	if ( m_pUserInventoryDlg )
	{
		delete m_pUserInventoryDlg;
		m_pUserInventoryDlg = NULL;
	}

	if ( m_pHorseInventoryDlg )
	{
		delete m_pHorseInventoryDlg;
		m_pHorseInventoryDlg = NULL;
	}

	if ( m_pBagInventoryDlg )
	{
		delete m_pBagInventoryDlg;
		m_pBagInventoryDlg = NULL;
	}
	
	if ( m_pBuffer )
	{
		delete m_pBuffer;
		m_pBuffer = NULL;
	}
	
	return;
}

void CItemInventory::LoadDataAndSetting( HWND hDlg )
{
	SI16 StartPos = 0;

	// 소환수 Tab인 경우.
	if ( m_siCurrentSummonIndex != -1 )
	{
		StartPos = PERSONITEM_SUMMON1EQUIP0 + MAX_SHAPE_ITEM * m_siCurrentSummonIndex;
	}

	for ( int i = 0; i < MAX_ITEM_PER_PERSON; ++i )
	{
		m_clItem[ i ] = m_pPersonItemInfo->clItem[ i ];
	}

	TOOLINFO ti;
	
	for ( i = StartPos; i < StartPos + MAX_SHAPE_ITEM; ++i )
	{
		if ( m_clItem[ i ].siUnique != 0 )
		{
			SI32 ImageListIndex = 0;

			switch( i - StartPos )
			{
			case PERSONITEM_HELMET:
				{
					SetInventoryInListView( pclClient->pclItemManager, hDlg, IDC_LIST_HELMET, NULL, &m_clItem[ i ], 0, 0 );

					char ItemExplain[ 512 ] = "";
					pclClient->pclItemManager->GetExplain( &m_clItem[ i ], ItemExplain );

					ti.cbSize=sizeof(TOOLINFO);
					ti.uFlags=TTF_SUBCLASS | TTF_IDISHWND;
					ti.hwnd=hDlg;
					ti.uId=(WPARAM)m_hHelmet;
					ti.lpszText = ItemExplain;
					SendMessage( m_hTip,TTM_ADDTOOL,0,(LPARAM)(LPTOOLINFO)&ti );
					SendMessage( m_hTip, TTM_SETMAXTIPWIDTH, 0, MAX_TOOLTIP_WIDTH );
					SendMessage( m_hTip, TTM_SETDELAYTIME, TTDT_INITIAL, 0 );
				}
				break;
			case PERSONITEM_ARMOUR:
				{
					SetInventoryInListView( pclClient->pclItemManager, hDlg, IDC_LIST_ARMOR, NULL, &m_clItem[ i ], 0, 0 );

					char ItemExplain[ 512 ] = "";
					pclClient->pclItemManager->GetExplain( &m_clItem[ i ], ItemExplain );

					ti.cbSize=sizeof(TOOLINFO);
					ti.uFlags=TTF_SUBCLASS | TTF_IDISHWND;
					ti.hwnd=hDlg;
					ti.uId=(WPARAM)m_hArmor;
					ti.lpszText = ItemExplain;
					SendMessage( m_hTip,TTM_ADDTOOL,0,(LPARAM)(LPTOOLINFO)&ti );
					SendMessage( m_hTip, TTM_SETMAXTIPWIDTH, 0, MAX_TOOLTIP_WIDTH );
					SendMessage( m_hTip, TTM_SETDELAYTIME, TTDT_INITIAL, 0 );

				}
				break;
			case PERSONITEM_WEAPON1:
				{
					SetInventoryInListView( pclClient->pclItemManager, hDlg, IDC_LIST_WEAPON1, NULL, &m_clItem[ i ], 0, 0 );

					char ItemExplain[ 512 ] = "";
					pclClient->pclItemManager->GetExplain( &m_clItem[ i ], ItemExplain );

					ti.cbSize=sizeof(TOOLINFO);
					ti.uFlags=TTF_SUBCLASS | TTF_IDISHWND;
					ti.hwnd=hDlg;
					ti.uId=(WPARAM)m_hWeapon1;
					ti.lpszText = ItemExplain;
					SendMessage( m_hTip,TTM_ADDTOOL,0,(LPARAM)(LPTOOLINFO)&ti );
					SendMessage( m_hTip, TTM_SETMAXTIPWIDTH, 0, MAX_TOOLTIP_WIDTH );
					SendMessage( m_hTip, TTM_SETDELAYTIME, TTDT_INITIAL, 0 );
				}
				break;
			case PERSONITEM_WEAPON2:
				{
					SetInventoryInListView( pclClient->pclItemManager, hDlg, IDC_LIST_WEAPON2, NULL, &m_clItem[ i ], 0, 0 );

					char ItemExplain[ 512 ] = "";
					pclClient->pclItemManager->GetExplain( &m_clItem[ i ], ItemExplain );

					ti.cbSize=sizeof(TOOLINFO);
					ti.uFlags=TTF_SUBCLASS | TTF_IDISHWND;
					ti.hwnd=hDlg;
					ti.uId=(WPARAM)m_hWeapon2;
					ti.lpszText = ItemExplain;
					SendMessage( m_hTip,TTM_ADDTOOL,0,(LPARAM)(LPTOOLINFO)&ti );
					SendMessage( m_hTip, TTM_SETMAXTIPWIDTH, 0, MAX_TOOLTIP_WIDTH );
					SendMessage( m_hTip, TTM_SETDELAYTIME, TTDT_INITIAL, 0 );
				}
				break;	
			case PERSONITEM_NECK:
				{
					SetInventoryInListView( pclClient->pclItemManager, hDlg, IDC_LIST_NECKWEAR, NULL, &m_clItem[ i ], 0, 0 );

					char ItemExplain[ 512 ] = "";
					pclClient->pclItemManager->GetExplain( &m_clItem[ i ], ItemExplain );

					ti.cbSize=sizeof(TOOLINFO);
					ti.uFlags=TTF_SUBCLASS | TTF_IDISHWND;
					ti.hwnd=hDlg;
					ti.uId=(WPARAM)m_hNeck;
					ti.lpszText = ItemExplain;
					SendMessage( m_hTip,TTM_ADDTOOL,0,(LPARAM)(LPTOOLINFO)&ti );
					SendMessage( m_hTip, TTM_SETMAXTIPWIDTH, 0, MAX_TOOLTIP_WIDTH );
					SendMessage( m_hTip, TTM_SETDELAYTIME, TTDT_INITIAL, 0 );
				}
				break;
			case PERSONITEM_RING1:
				{
					SetInventoryInListView( pclClient->pclItemManager, hDlg, IDC_LIST_RING1, NULL, &m_clItem[ i ], 0, 0 );

					char ItemExplain[ 512 ] = "";
					pclClient->pclItemManager->GetExplain( &m_clItem[ i ], ItemExplain );

					ti.cbSize=sizeof(TOOLINFO);
					ti.uFlags=TTF_SUBCLASS | TTF_IDISHWND;
					ti.hwnd=hDlg;
					ti.uId=(WPARAM)m_hRing1;
					ti.lpszText = ItemExplain;
					SendMessage( m_hTip,TTM_ADDTOOL,0,(LPARAM)(LPTOOLINFO)&ti );
					SendMessage( m_hTip, TTM_SETMAXTIPWIDTH, 0, MAX_TOOLTIP_WIDTH );
					SendMessage( m_hTip, TTM_SETDELAYTIME, TTDT_INITIAL, 0 );
				}
				break;
			case PERSONITEM_RING2:
				{
					SetInventoryInListView( pclClient->pclItemManager, hDlg, IDC_LIST_RING2, NULL, &m_clItem[ i ], 0, 0 );

					char ItemExplain[ 512 ] = "";
					pclClient->pclItemManager->GetExplain( &m_clItem[ i ], ItemExplain );

					ti.cbSize=sizeof(TOOLINFO);
					ti.uFlags=TTF_SUBCLASS | TTF_IDISHWND;
					ti.hwnd=hDlg;
					ti.uId=(WPARAM)m_hRing2;
					ti.lpszText = ItemExplain;
					SendMessage( m_hTip,TTM_ADDTOOL,0,(LPARAM)(LPTOOLINFO)&ti );
					SendMessage( m_hTip, TTM_SETMAXTIPWIDTH, 0, MAX_TOOLTIP_WIDTH );
					SendMessage( m_hTip, TTM_SETDELAYTIME, TTDT_INITIAL, 0 );
				}
				break;
			case PERSONITEM_BELT:
				{
					SetInventoryInListView( pclClient->pclItemManager, hDlg, IDC_LIST_WAISTBAND, NULL, &m_clItem[ i ], 0, 0 );

					char ItemExplain[ 512 ] = "";
					pclClient->pclItemManager->GetExplain( &m_clItem[ i ], ItemExplain );

					ti.cbSize=sizeof(TOOLINFO);
					ti.uFlags=TTF_SUBCLASS | TTF_IDISHWND;
					ti.hwnd=hDlg;
					ti.uId=(WPARAM)m_hBelt;
					ti.lpszText = ItemExplain;
					SendMessage( m_hTip,TTM_ADDTOOL,0,(LPARAM)(LPTOOLINFO)&ti );
					SendMessage( m_hTip, TTM_SETMAXTIPWIDTH, 0, MAX_TOOLTIP_WIDTH );
					SendMessage( m_hTip, TTM_SETDELAYTIME, TTDT_INITIAL, 0 );
				}
				break;
			case PERSONITEM_SHOES:
				{
					SetInventoryInListView( pclClient->pclItemManager, hDlg, IDC_LIST_SHOE, NULL, &m_clItem[ i ], 0, 0 );

					char ItemExplain[ 512 ] = "";
					pclClient->pclItemManager->GetExplain( &m_clItem[ i ], ItemExplain );

					ti.cbSize=sizeof(TOOLINFO);
					ti.uFlags=TTF_SUBCLASS | TTF_IDISHWND;
					ti.hwnd=hDlg;
					ti.uId=(WPARAM)m_hShoes;
					ti.lpszText = ItemExplain;
					SendMessage( m_hTip,TTM_ADDTOOL,0,(LPARAM)(LPTOOLINFO)&ti );
					SendMessage( m_hTip, TTM_SETMAXTIPWIDTH, 0, MAX_TOOLTIP_WIDTH );
					SendMessage( m_hTip, TTM_SETDELAYTIME, TTDT_INITIAL, 0 );
				}
				break;
			case PERSONITEM_MANTLE:
				{
					SetInventoryInListView( pclClient->pclItemManager, hDlg, IDC_LIST_MANTLE, NULL, &m_clItem[ i ], 0, 0 );

					char ItemExplain[ 512 ] = "";
					pclClient->pclItemManager->GetExplain( &m_clItem[ i ], ItemExplain );

					ti.cbSize=sizeof(TOOLINFO);
					ti.uFlags=TTF_SUBCLASS | TTF_IDISHWND;
					ti.hwnd=hDlg;
					ti.uId=(WPARAM)m_hMantle;
					ti.lpszText = ItemExplain;
					SendMessage( m_hTip,TTM_ADDTOOL,0,(LPARAM)(LPTOOLINFO)&ti );
					SendMessage( m_hTip, TTM_SETMAXTIPWIDTH, 0, MAX_TOOLTIP_WIDTH );
					SendMessage( m_hTip, TTM_SETDELAYTIME, TTDT_INITIAL, 0 );
				}
				break;
			case PERSONITEM_BAG:
				{
					SetInventoryInListView( pclClient->pclItemManager, hDlg, IDC_LIST_BAG, NULL, &m_clItem[ i ], 0, 0 );

					char ItemExplain[ 512 ] = "";
					pclClient->pclItemManager->GetExplain( &m_clItem[ i ], ItemExplain );

					ti.cbSize=sizeof(TOOLINFO);
					ti.uFlags=TTF_SUBCLASS | TTF_IDISHWND;
					ti.hwnd=hDlg;
					ti.uId=(WPARAM)m_hBag;
					ti.lpszText = ItemExplain;
					SendMessage( m_hTip,TTM_ADDTOOL,0,(LPARAM)(LPTOOLINFO)&ti );
					SendMessage( m_hTip, TTM_SETMAXTIPWIDTH, 0, MAX_TOOLTIP_WIDTH );
					SendMessage( m_hTip, TTM_SETDELAYTIME, TTDT_INITIAL, 0 );
				}
				break;
			case PERSONITEM_HAT:
				{
					SetInventoryInListView( pclClient->pclItemManager, hDlg, IDC_LIST_HAT, NULL, &m_clItem[ i ], 0, 0 );

					char ItemExplain[ 512 ] = "";
					pclClient->pclItemManager->GetExplain( &m_clItem[ i ], ItemExplain );

					ti.cbSize=sizeof(TOOLINFO);
					ti.uFlags=TTF_SUBCLASS | TTF_IDISHWND;
					ti.hwnd=hDlg;
					ti.uId=(WPARAM)m_hHat;
					ti.lpszText = ItemExplain;
					SendMessage( m_hTip,TTM_ADDTOOL,0,(LPARAM)(LPTOOLINFO)&ti );
					SendMessage( m_hTip, TTM_SETMAXTIPWIDTH, 0, MAX_TOOLTIP_WIDTH );
					SendMessage( m_hTip, TTM_SETDELAYTIME, TTDT_INITIAL, 0 );
				}
				break;
			case PERSONITEM_DRESS:
				{
					SetInventoryInListView( pclClient->pclItemManager, hDlg, IDC_LIST_DRESS, NULL, &m_clItem[ i ], 0, 0 );

					char ItemExplain[ 512 ] = "";
					pclClient->pclItemManager->GetExplain( &m_clItem[ i ], ItemExplain );

					ti.cbSize=sizeof(TOOLINFO);
					ti.uFlags=TTF_SUBCLASS | TTF_IDISHWND;
					ti.hwnd=hDlg;
					ti.uId=(WPARAM)m_hDress;
					ti.lpszText = ItemExplain;
					SendMessage( m_hTip,TTM_ADDTOOL,0,(LPARAM)(LPTOOLINFO)&ti );
					SendMessage( m_hTip, TTM_SETMAXTIPWIDTH, 0, MAX_TOOLTIP_WIDTH );
					SendMessage( m_hTip, TTM_SETDELAYTIME, TTDT_INITIAL, 0 );
				}
				break;
			}
		}
		else
		{
			switch( i - StartPos )
			{
			case PERSONITEM_HELMET:
				{
					ListView_DeleteAllItems( m_hHelmet );

					ti.cbSize=sizeof(TOOLINFO);
					ti.uFlags=TTF_SUBCLASS | TTF_IDISHWND;
					ti.hwnd=hDlg;
					ti.uId=(WPARAM)m_hHelmet;
					char* pText = GetTxtFromMgr(1144);
					
					ti.lpszText=pText;
					SendMessage( m_hTip,TTM_ADDTOOL,0,(LPARAM)(LPTOOLINFO)&ti );
					SendMessage( m_hTip, TTM_SETMAXTIPWIDTH, 0, MAX_TOOLTIP_WIDTH );
					SendMessage( m_hTip, TTM_SETDELAYTIME, TTDT_INITIAL, 0 );

				}
				break;
			case PERSONITEM_ARMOUR:
				{
					ListView_DeleteAllItems( m_hArmor );

					ti.cbSize=sizeof(TOOLINFO);
					ti.uFlags=TTF_SUBCLASS | TTF_IDISHWND;
					ti.hwnd=hDlg;
					ti.uId=(WPARAM)m_hArmor;
					char* pText = GetTxtFromMgr(1145);
					
					ti.lpszText=pText;
					SendMessage( m_hTip,TTM_ADDTOOL,0,(LPARAM)(LPTOOLINFO)&ti );
					SendMessage( m_hTip, TTM_SETMAXTIPWIDTH, 0, MAX_TOOLTIP_WIDTH );
					SendMessage( m_hTip, TTM_SETDELAYTIME, TTDT_INITIAL, 0 );
				}
				break;
			case PERSONITEM_WEAPON1:
				{
					ListView_DeleteAllItems( m_hWeapon1 );

					ti.cbSize=sizeof(TOOLINFO);
					ti.uFlags=TTF_SUBCLASS | TTF_IDISHWND;
					ti.hwnd=hDlg;
					ti.uId=(WPARAM)m_hWeapon1;
					char* pText = GetTxtFromMgr(1146);
					
					ti.lpszText=pText;
					SendMessage( m_hTip,TTM_ADDTOOL,0,(LPARAM)(LPTOOLINFO)&ti );
					SendMessage( m_hTip, TTM_SETMAXTIPWIDTH, 0, MAX_TOOLTIP_WIDTH );
					SendMessage( m_hTip, TTM_SETDELAYTIME, TTDT_INITIAL, 0 );

				}
				break;
			case PERSONITEM_WEAPON2:
				{
					ListView_DeleteAllItems( m_hWeapon2 );

					// 소환수 Tab인 경우.
					if ( m_siCurrentSummonIndex != -1 )
					{
						ti.cbSize=sizeof(TOOLINFO);
						ti.uFlags=TTF_SUBCLASS | TTF_IDISHWND;
						ti.hwnd=hDlg;
						ti.uId=(WPARAM)m_hWeapon2;
						char* pText = GetTxtFromMgr(1147);
						
						ti.lpszText=pText;
						SendMessage( m_hTip,TTM_ADDTOOL,0,(LPARAM)(LPTOOLINFO)&ti );
						SendMessage( m_hTip, TTM_SETMAXTIPWIDTH, 0, MAX_TOOLTIP_WIDTH );
						SendMessage( m_hTip, TTM_SETDELAYTIME, TTDT_INITIAL, 0 );
					}
					else
					{	
						ti.cbSize=sizeof(TOOLINFO);
						ti.uFlags=TTF_SUBCLASS | TTF_IDISHWND;
						ti.hwnd=hDlg;
						ti.uId=(WPARAM)m_hWeapon2;
						char* pText = GetTxtFromMgr(1148);
						
						ti.lpszText=pText;
						SendMessage( m_hTip,TTM_ADDTOOL,0,(LPARAM)(LPTOOLINFO)&ti );
						SendMessage( m_hTip, TTM_SETMAXTIPWIDTH, 0, MAX_TOOLTIP_WIDTH );
						SendMessage( m_hTip, TTM_SETDELAYTIME, TTDT_INITIAL, 0 );
					}
				}
				break;
			case PERSONITEM_NECK:
				{
					ListView_DeleteAllItems( m_hNeck );

					ti.cbSize=sizeof(TOOLINFO);
					ti.uFlags=TTF_SUBCLASS | TTF_IDISHWND;
					ti.hwnd=hDlg;
					ti.uId=(WPARAM)m_hNeck;
					char* pText = GetTxtFromMgr(1149);
					
					ti.lpszText=pText;
					SendMessage( m_hTip,TTM_ADDTOOL,0,(LPARAM)(LPTOOLINFO)&ti );
					SendMessage( m_hTip, TTM_SETMAXTIPWIDTH, 0, MAX_TOOLTIP_WIDTH );
					SendMessage( m_hTip, TTM_SETDELAYTIME, TTDT_INITIAL, 0 );
				}
				break;
			case PERSONITEM_RING1:
				{
					ListView_DeleteAllItems( m_hRing1 );

					ti.cbSize=sizeof(TOOLINFO);
					ti.uFlags=TTF_SUBCLASS | TTF_IDISHWND;
					ti.hwnd=hDlg;
					ti.uId=(WPARAM)m_hRing1;
					char* pText = GetTxtFromMgr(1150);
					
					ti.lpszText=pText;
					SendMessage( m_hTip,TTM_ADDTOOL,0,(LPARAM)(LPTOOLINFO)&ti );
					SendMessage( m_hTip, TTM_SETMAXTIPWIDTH, 0, MAX_TOOLTIP_WIDTH );
					SendMessage( m_hTip, TTM_SETDELAYTIME, TTDT_INITIAL, 0 );
				}
				break;
			case PERSONITEM_RING2:
				{
					ListView_DeleteAllItems( m_hRing2 );

					ti.cbSize=sizeof(TOOLINFO);
					ti.uFlags=TTF_SUBCLASS | TTF_IDISHWND;
					ti.hwnd=hDlg;
					ti.uId=(WPARAM)m_hRing2;
					char* pText = GetTxtFromMgr(1150);
					
					ti.lpszText=pText;
					SendMessage( m_hTip,TTM_ADDTOOL,0,(LPARAM)(LPTOOLINFO)&ti );
					SendMessage( m_hTip, TTM_SETMAXTIPWIDTH, 0, MAX_TOOLTIP_WIDTH );
					SendMessage( m_hTip, TTM_SETDELAYTIME, TTDT_INITIAL, 0 );
				}
				break;
			case PERSONITEM_BELT:
				{
					ListView_DeleteAllItems( m_hBelt );
					
					ti.cbSize=sizeof(TOOLINFO);
					ti.uFlags=TTF_SUBCLASS | TTF_IDISHWND;
					ti.hwnd=hDlg;
					ti.uId=(WPARAM)m_hBelt;
					char* pText = GetTxtFromMgr(1151);
					
					ti.lpszText=pText;
					SendMessage( m_hTip,TTM_ADDTOOL,0,(LPARAM)(LPTOOLINFO)&ti );
					SendMessage( m_hTip, TTM_SETMAXTIPWIDTH, 0, MAX_TOOLTIP_WIDTH );
					SendMessage( m_hTip, TTM_SETDELAYTIME, TTDT_INITIAL, 0 );
				}
				break;
			case PERSONITEM_SHOES:
				{
					ListView_DeleteAllItems( m_hShoes );

					ti.cbSize=sizeof(TOOLINFO);
					ti.uFlags=TTF_SUBCLASS | TTF_IDISHWND;
					ti.hwnd=hDlg;
					ti.uId=(WPARAM)m_hShoes;
					char* pText = GetTxtFromMgr(1152);
					
					ti.lpszText=pText;
					SendMessage( m_hTip,TTM_ADDTOOL,0,(LPARAM)(LPTOOLINFO)&ti );
					SendMessage( m_hTip, TTM_SETMAXTIPWIDTH, 0, MAX_TOOLTIP_WIDTH );
					SendMessage( m_hTip, TTM_SETDELAYTIME, TTDT_INITIAL, 0 );
				}
				break;
			case PERSONITEM_MANTLE:
				{
					ListView_DeleteAllItems( m_hMantle );

					ti.cbSize=sizeof(TOOLINFO);
					ti.uFlags=TTF_SUBCLASS | TTF_IDISHWND;
					ti.hwnd=hDlg;
					ti.uId=(WPARAM)m_hMantle;
					char* pText = GetTxtFromMgr(1153);
					
					ti.lpszText=pText;
					SendMessage( m_hTip,TTM_ADDTOOL,0,(LPARAM)(LPTOOLINFO)&ti );
					SendMessage( m_hTip, TTM_SETMAXTIPWIDTH, 0, MAX_TOOLTIP_WIDTH );
					SendMessage( m_hTip, TTM_SETDELAYTIME, TTDT_INITIAL, 0 );
				}
				break;
			case PERSONITEM_BAG:
				{
					ListView_DeleteAllItems( m_hBag );

					// 소환수 Tab인 경우.
					if ( m_siCurrentSummonIndex != -1 )
					{
						ti.cbSize=sizeof(TOOLINFO);
						ti.uFlags=TTF_SUBCLASS | TTF_IDISHWND;
						ti.hwnd=hDlg;
						ti.uId=(WPARAM)m_hBag;
						char* pText = GetTxtFromMgr(1154);
						
						ti.lpszText=pText;
						SendMessage( m_hTip,TTM_ADDTOOL,0,(LPARAM)(LPTOOLINFO)&ti );
						SendMessage( m_hTip, TTM_SETMAXTIPWIDTH, 0, MAX_TOOLTIP_WIDTH );
						SendMessage( m_hTip, TTM_SETDELAYTIME, TTDT_INITIAL, 0 );
					}
					else
					{	
						ti.cbSize=sizeof(TOOLINFO);
						ti.uFlags=TTF_SUBCLASS | TTF_IDISHWND;
						ti.hwnd=hDlg;
						ti.uId=(WPARAM)m_hBag;
						char* pText = GetTxtFromMgr(1155);
						
						ti.lpszText=pText;
						SendMessage( m_hTip,TTM_ADDTOOL,0,(LPARAM)(LPTOOLINFO)&ti );
						SendMessage( m_hTip, TTM_SETMAXTIPWIDTH, 0, MAX_TOOLTIP_WIDTH );
						SendMessage( m_hTip, TTM_SETDELAYTIME, TTDT_INITIAL, 0 );
					}
				}
				break;
			case PERSONITEM_HAT:
				{
					ListView_DeleteAllItems( m_hHat );

					ti.cbSize=sizeof(TOOLINFO);
					ti.uFlags=TTF_SUBCLASS | TTF_IDISHWND;
					ti.hwnd=hDlg;
					ti.uId=(WPARAM)m_hHat;
					char* pText = GetTxtFromMgr(1156);
					
					ti.lpszText=pText;
					SendMessage( m_hTip,TTM_ADDTOOL,0,(LPARAM)(LPTOOLINFO)&ti );
					SendMessage( m_hTip, TTM_SETMAXTIPWIDTH, 0, MAX_TOOLTIP_WIDTH );
					SendMessage( m_hTip, TTM_SETDELAYTIME, TTDT_INITIAL, 0 );
				}
				break;
			case PERSONITEM_DRESS:
				{
					ListView_DeleteAllItems( m_hDress );

					ti.cbSize=sizeof(TOOLINFO);
					ti.uFlags=TTF_SUBCLASS | TTF_IDISHWND;
					ti.hwnd=hDlg;
					ti.uId=(WPARAM)m_hDress;
					char* pText = GetTxtFromMgr(1157);
					
					ti.lpszText=pText;
					SendMessage( m_hTip,TTM_ADDTOOL,0,(LPARAM)(LPTOOLINFO)&ti );
					SendMessage( m_hTip, TTM_SETMAXTIPWIDTH, 0, MAX_TOOLTIP_WIDTH );
					SendMessage( m_hTip, TTM_SETDELAYTIME, TTDT_INITIAL, 0 );
				}
				break;
			}
		}
	}

	HWND hMakeItemComboBox = GetDlgItem( m_hDlg, IDC_COMBO_MAKEITEM_LIST );

	//-------------------------콤보 박스 데이터 완전 초기화
	SendMessage( hMakeItemComboBox, CB_RESETCONTENT, 0, 0 );

	//----------------------------------------------

	SI32 siCount = 0;

	// 주어진 재료를 근거로 생산 가능한 아이템의 리스트를 작성해서 리턴한다. 
	// 얻어온 리스트는 사용후 삭제해 주어야 한다. 
	SI32 siMakeableItemNum = 0;	
	
	if ( m_pMakeableItemList )
	{
		delete [] m_pMakeableItemList;
		m_pMakeableItemList = NULL;
	}	

	m_pMakeableItemList = NULL;

	SI32 id = 1;

	if(id)
	{		
		if( pclClient->pclItemManager->FindMakeableItem(
			&pclClient->pclCM->CR[id]->pclCI->clCharSkill,
			&pclClient->pclCM->CR[id]->pclCI->clCharItem,
			&pclClient->pclCM->CR[id]->clPB.clTotalBA, 
			MAX_INVITEM_PER_PERSON, 
			&pclClient->pclCM->CR[id]->pclCI->clCharItem.clItem[PERSONITEM_INV0], 
			&m_siMakeableItemNum, &m_pMakeableItemList,
			false) == true)
		{
			
			// 콤보 박스에 아이템의 생산가능한 명단을 표시한다. 
			for( SI32 i = 0; i < m_siMakeableItemNum; ++i )
			{
				SI32 unique		= m_pMakeableItemList[ i * 2 ];
				SI32 itemnum	= m_pMakeableItemList[ i * 2 +1];
				
				if ( unique !=  0 )
				{
					char buffer[256];
					SI32 ref = pclClient->pclItemManager->FindItemRefFromUnique( unique );
					
					if(ref > 0)
					{
						char* pText = GetTxtFromMgr(676);
						
						
						sprintf( buffer, pText, pclClient->pclItemManager->pclItemInfo[ref]->GetName(), itemnum );
						
						SendMessage( hMakeItemComboBox, CB_ADDSTRING, 0, (LPARAM)buffer );
					}
				}
			}
		}
		
		if ( m_siMakeableItemNum > 0 )
		{
			m_siMakeItemComboBoxSelectedIndex = 0;
			SendMessage( hMakeItemComboBox, CB_SETCURSEL, m_siMakeItemComboBoxSelectedIndex, 0 );

			SI32 ref = pclClient->pclItemManager->FindItemRefFromUnique( m_pMakeableItemList[ m_siMakeItemComboBoxSelectedIndex * 2 ] );
			SI32 MinProductUnit = pclClient->pclItemManager->pclItemInfo[ref]->clProductionInfo.siProductUnit;
			char buffer[256] = "";
			sprintf( buffer, "%d", MinProductUnit );
			SetWindowText( GetDlgItem( m_hDlg, IDC_EDIT_MAKEITME_NUM ), buffer );
		}
	}


	// 인벤토리 무게 적는 곳
	char strWeightBuffer[256 ] = "";
	SI32 siTotalWeight = pclClient->pclCM->CR[1]->GetTotalWeight();		// 현재 인벤토리의 총무게
	SI32 siMaxWeight = pclClient->pclCM->CR[1]->GetMaxWeight();			// 들수 있는 최대 무게

	char* pText = GetTxtFromMgr(1158);
	

	sprintf( strWeightBuffer, pText, (float)siTotalWeight/10.0, (float)siMaxWeight/10.0);
	SetWindowText( GetDlgItem( m_hDlg, IDC_STATIC_INVENTORY_WEIGHT ), strWeightBuffer );

	return;
}

void CItemInventory::LoadSummonInfo( HWND hDlg )
{	
	// 소환수 부분
	memcpy( m_pSummonInfo, &pclClient->pclCM->CR[1]->pclCI->clSummonInfo, sizeof( cltSummonInfo ) );
	
	ShowSummonTabDlg();
}



LVBKIMAGE lv;		// 이 변수 계속 살아 있어야 되넹... 죽으면 리스트뷰 배경도 가라지뿌고... ㅡㅡ

BOOL CALLBACK CItemInventory::ItemInventoryDlgProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam )
{
	INITCOMMONCONTROLSEX iccex;	
	cltClient *pclclient = (cltClient *)pclClient;
	
	switch( iMsg )
	{		
		case WM_INITDIALOG:
		{
			InitCommonControls();
			m_hDlg = hDlg;

			iccex.dwICC=ICC_WIN95_CLASSES;
			iccex.dwSize=sizeof(INITCOMMONCONTROLSEX);
			InitCommonControlsEx(&iccex);

			TOOLINFO ti;
			/*
			hTip=CreateWindowEx(WS_EX_TOPMOST,TOOLTIPS_CLASS,NULL,0,
				CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,
				hDlg,NULL,pclClient->GetInstance(),NULL);
			*/
			m_hTip = CreateWindow( TOOLTIPS_CLASS, NULL, WS_POPUP | TTS_NOPREFIX | TTS_BALLOON, 0, 0, 0, 0, NULL, NULL, pclClient->GetInstance(), NULL );
			
			m_siSelectedInventoryItemIndex = -1;

			m_hTabImageBitmap[ 0 ] = (HBITMAP)LoadImage( pclClient->GetInstance(), "Interface\\Inventory\\Tab0.bmp", IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE );
			m_hTabImageBitmap[ 1 ] = (HBITMAP)LoadImage( pclClient->GetInstance(), "Interface\\Inventory\\Tab1.bmp", IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE );

			//---------------------------------------------------
			// 텝 컨트롤의 탭 부분 기본 설정
			TC_ITEM item;
			// to use an image set this to TCIF_IMAGE  
			item.mask= TCIF_TEXT; 
			// index into tabs image list, set to -1 when not in use
			item.iImage = -1;
			item.lParam = 0;
			
			char* pText = GetTxtFromMgr(1159);
			

			item.pszText = pText;
			TabCtrl_InsertItem(GetDlgItem(hDlg, IDC_TAB_INVENTORY),0,&item);

			item.lParam = 1;

			pText = GetTxtFromMgr(372);
			

			item.pszText = pText;
			TabCtrl_InsertItem(GetDlgItem(hDlg, IDC_TAB_INVENTORY),1,&item);
			//---------------------------------------------------
	
			m_hTabCtrl = GetDlgItem(hDlg, IDC_TAB_INVENTORY );

			m_hTabCtrlHeroSelect = GetDlgItem( hDlg, IDC_TAB_SELECT_HERO );

			//---------------------------------------------------
			// 텝 컨트롤에 들어갈 차일드 다이얼로그 세팅
			m_pUserInventoryDlg->Create( GetDlgItem( hDlg, IDC_TAB_INVENTORY ), m_hInst, m_pPersonItemInfo, m_pBuffer, hDlg );
			m_pHorseInventoryDlg->Create( GetDlgItem( hDlg, IDC_TAB_INVENTORY ), m_hInst, m_pPersonItemInfo, m_pBuffer, hDlg );
			m_pBagInventoryDlg->Create( GetDlgItem( hDlg, IDC_TAB_INVENTORY ), m_hInst, m_pPersonItemInfo, m_pBuffer, hDlg );
			//---------------------------------------------------
			
			RECT rect;
			GetClientRect( GetDlgItem( hDlg, IDC_TAB_INVENTORY ), &rect);
			TabCtrl_AdjustRect( GetDlgItem( hDlg, IDC_TAB_INVENTORY ), FALSE, &rect);
			
			MoveWindow( m_pUserInventoryDlg->m_hDlg, 
				rect.left, rect.top, rect.right-rect. left,
				rect.bottom- rect.top, TRUE);

			MoveWindow( m_pHorseInventoryDlg->m_hDlg,
				rect.left, rect.top, rect.right-rect. left,
				rect.bottom- rect.top, TRUE);

			MoveWindow( m_pBagInventoryDlg->m_hDlg,
				rect.left, rect.top, rect.right-rect. left,
				rect.bottom- rect.top, TRUE);
			
			m_pUserInventoryDlg->Show();

			char strInventoryImagePath[ _MAX_PATH ];
			char strDirectoryPath[ _MAX_PATH ];
			
			//MoveWindow( GetDlgItem ( hDlg, IDC_BUTTON_TAB ), 11, 206, 54, 21, TRUE );
			MoveWindow( GetDlgItem ( hDlg, IDC_BUTTON_TAB ), 11, 190, 54, 21, TRUE );

			m_hWeapon1 = GetDlgItem( hDlg, IDC_LIST_WEAPON1 );
			g_FontManager.SetFont( hDlg, IDC_LIST_WEAPON1 );
			ListView_SetExtendedListViewStyle( m_hWeapon1, LVS_EX_FULLROWSELECT | LVS_EX_ONECLICKACTIVATE | LVS_EX_UNDERLINEHOT | LVS_EX_INFOTIP );
			memset( strInventoryImagePath, 0, _MAX_PATH );
			strcpy ( strInventoryImagePath, "file://");			
			memset( strDirectoryPath, 0, _MAX_PATH );
			GetCurrentDirectory( _MAX_PATH, strDirectoryPath );
			SI32 DirectoryPathSize = strlen ( strDirectoryPath );
			if ( strDirectoryPath[ DirectoryPathSize - 1 ] != '\\' )
			{
				strcat( strDirectoryPath, "\\" );
			}			
			strcat( strInventoryImagePath, strDirectoryPath );
			strcat( strInventoryImagePath, "Interface\\Inventory\\Weapon.bmp");
			lv.ulFlags = LVBKIF_SOURCE_URL | LVBKIF_STYLE_NORMAL;			
			lv.pszImage = strInventoryImagePath;
			::SendMessage(m_hWeapon1, LVM_SETBKIMAGE, 0, (LPARAM)&lv);
			SendMessage( m_hWeapon1, LVM_SETTEXTBKCOLOR, 0, 0xffffffff );			
			MoveWindow( m_hWeapon1, 13, 140, 48, 48, TRUE );

			ti.cbSize=sizeof(TOOLINFO);
			ti.uFlags=TTF_SUBCLASS | TTF_IDISHWND;
			ti.hwnd=hDlg;
			ti.uId=(WPARAM)m_hWeapon1;

			pText = GetTxtFromMgr(1146);
			

			ti.lpszText=pText;
			SendMessage( m_hTip,TTM_ADDTOOL,0,(LPARAM)(LPTOOLINFO)&ti );
			SendMessage( m_hTip, TTM_SETMAXTIPWIDTH, 0, MAX_TOOLTIP_WIDTH );
			SendMessage( m_hTip, TTM_SETDELAYTIME, TTDT_INITIAL, 0 );



			
			m_hWeapon2 = GetDlgItem( hDlg, IDC_LIST_WEAPON2 );
			g_FontManager.SetFont( hDlg, IDC_LIST_WEAPON2 );
			ListView_SetExtendedListViewStyle( m_hWeapon2, LVS_EX_FULLROWSELECT | LVS_EX_ONECLICKACTIVATE | LVS_EX_UNDERLINEHOT | LVS_EX_INFOTIP );
			memset( strInventoryImagePath, 0, _MAX_PATH );
			strcpy ( strInventoryImagePath, "file://");			
			memset( strDirectoryPath, 0, _MAX_PATH );
			GetCurrentDirectory( _MAX_PATH, strDirectoryPath );
			DirectoryPathSize = strlen ( strDirectoryPath );
			if ( strDirectoryPath[ DirectoryPathSize - 1 ] != '\\' )
			{
				strcat( strDirectoryPath, "\\" );
			}
			DirectoryPathSize = strlen ( strDirectoryPath );
			if ( strDirectoryPath[ DirectoryPathSize - 1 ] != '\\' )
			{
				strcat( strDirectoryPath, "\\" );
			}
			strcat( strInventoryImagePath, strDirectoryPath );
			strcat( strInventoryImagePath, "Interface\\Inventory\\Weapon.bmp");
			lv.ulFlags = LVBKIF_SOURCE_URL | LVBKIF_STYLE_NORMAL;			
			lv.pszImage = strInventoryImagePath;
			::SendMessage(m_hWeapon2, LVM_SETBKIMAGE, 0, (LPARAM)&lv);
			SendMessage( m_hWeapon2, LVM_SETTEXTBKCOLOR, 0, 0xffffffff );
			MoveWindow( m_hWeapon2, 13, 214, 48, 48, TRUE );

			ti.cbSize=sizeof(TOOLINFO);
			ti.uFlags=TTF_SUBCLASS | TTF_IDISHWND;
			ti.hwnd=hDlg;
			ti.uId=(WPARAM)m_hWeapon2;
			pText = GetTxtFromMgr(1148);
			

			ti.lpszText=pText;
			SendMessage( m_hTip,TTM_ADDTOOL,0,(LPARAM)(LPTOOLINFO)&ti );
			SendMessage( m_hTip, TTM_SETMAXTIPWIDTH, 0, MAX_TOOLTIP_WIDTH );
			SendMessage( m_hTip, TTM_SETDELAYTIME, TTDT_INITIAL, 0 );




			m_hHelmet = GetDlgItem( hDlg, IDC_LIST_HELMET );
			g_FontManager.SetFont( hDlg, IDC_LIST_HELMET );
			ListView_SetExtendedListViewStyle( m_hHelmet, LVS_EX_FULLROWSELECT | LVS_EX_ONECLICKACTIVATE | LVS_EX_UNDERLINEHOT | LVS_EX_INFOTIP );
			memset( strInventoryImagePath, 0, _MAX_PATH );
			strcpy ( strInventoryImagePath, "file://");			
			memset( strDirectoryPath, 0, _MAX_PATH );
			GetCurrentDirectory( _MAX_PATH, strDirectoryPath );
			DirectoryPathSize = strlen ( strDirectoryPath );
			if ( strDirectoryPath[ DirectoryPathSize - 1 ] != '\\' )
			{
				strcat( strDirectoryPath, "\\" );
			}
			DirectoryPathSize = strlen ( strDirectoryPath );
			if ( strDirectoryPath[ DirectoryPathSize - 1 ] != '\\' )
			{
				strcat( strDirectoryPath, "\\" );
			}
			strcat( strInventoryImagePath, strDirectoryPath );
			strcat( strInventoryImagePath, "Interface\\Inventory\\Helmet.bmp");
			lv.ulFlags = LVBKIF_SOURCE_URL | LVBKIF_STYLE_NORMAL;			
			lv.pszImage = strInventoryImagePath;
			::SendMessage(m_hHelmet, LVM_SETBKIMAGE, 0, (LPARAM)&lv);
			SendMessage( m_hHelmet, LVM_SETTEXTBKCOLOR, 0, 0xffffffff );			
			MoveWindow( m_hHelmet, 83, 26, 48, 48, TRUE );


			ti.cbSize=sizeof(TOOLINFO);
			ti.uFlags=TTF_SUBCLASS | TTF_IDISHWND;
			ti.hwnd=hDlg;
			ti.uId=(WPARAM)m_hHelmet;
			pText = GetTxtFromMgr(1144);
			

			ti.lpszText=pText;
			SendMessage( m_hTip,TTM_ADDTOOL,0,(LPARAM)(LPTOOLINFO)&ti );
			SendMessage( m_hTip, TTM_SETMAXTIPWIDTH, 0, MAX_TOOLTIP_WIDTH );
			SendMessage( m_hTip, TTM_SETDELAYTIME, TTDT_INITIAL, 0 );



			m_hHat  = GetDlgItem( hDlg, IDC_LIST_HAT );
			g_FontManager.SetFont( hDlg, IDC_LIST_HAT );
			ListView_SetExtendedListViewStyle( m_hHat, LVS_EX_FULLROWSELECT | LVS_EX_ONECLICKACTIVATE | LVS_EX_UNDERLINEHOT | LVS_EX_INFOTIP );
			memset( strInventoryImagePath, 0, _MAX_PATH );
			strcpy ( strInventoryImagePath, "file://");
			memset( strDirectoryPath, 0, _MAX_PATH );
			GetCurrentDirectory( _MAX_PATH, strDirectoryPath );
			DirectoryPathSize = strlen ( strDirectoryPath );
			if ( strDirectoryPath[ DirectoryPathSize - 1 ] != '\\' )
			{
				strcat( strDirectoryPath, "\\" );
			}
			strcat( strInventoryImagePath, strDirectoryPath );
			strcat( strInventoryImagePath, "Interface\\Inventory\\Hat.bmp" );
			lv.ulFlags = LVBKIF_SOURCE_URL | LVBKIF_STYLE_NORMAL;
			lv.pszImage = strInventoryImagePath;
			::SendMessage( m_hHat, LVM_SETBKIMAGE, 0, (LPARAM)&lv);
			SendMessage( m_hHat, LVM_SETTEXTBKCOLOR, 0, 0xffffffff );
			//MoveWindow( m_hHat, 25, 30, 48, 48, TRUE );
			MoveWindow( m_hHat, 22, 26, 48, 48, TRUE );

			ti.cbSize=sizeof(TOOLINFO);
			ti.uFlags=TTF_SUBCLASS | TTF_IDISHWND;
			ti.hwnd=hDlg;
			ti.uId=(WPARAM)m_hHat;
			pText = GetTxtFromMgr(1156);
			

			ti.lpszText=pText;
			SendMessage( m_hTip,TTM_ADDTOOL,0,(LPARAM)(LPTOOLINFO)&ti );
			SendMessage( m_hTip, TTM_SETMAXTIPWIDTH, 0, MAX_TOOLTIP_WIDTH );
			SendMessage( m_hTip, TTM_SETDELAYTIME, TTDT_INITIAL, 0 );
			



			m_hRing1 = GetDlgItem( hDlg, IDC_LIST_RING1 );
			g_FontManager.SetFont( hDlg, IDC_LIST_RING1 );
			ListView_SetExtendedListViewStyle( m_hRing1, LVS_EX_FULLROWSELECT | LVS_EX_ONECLICKACTIVATE | LVS_EX_UNDERLINEHOT | LVS_EX_INFOTIP );
			memset( strInventoryImagePath, 0, _MAX_PATH );
			strcpy ( strInventoryImagePath, "file://");			
			memset( strDirectoryPath, 0, _MAX_PATH );
			GetCurrentDirectory( _MAX_PATH, strDirectoryPath );
			DirectoryPathSize = strlen ( strDirectoryPath );
			if ( strDirectoryPath[ DirectoryPathSize - 1 ] != '\\' )
			{
				strcat( strDirectoryPath, "\\" );
			}
			strcat( strInventoryImagePath, strDirectoryPath );
			strcat( strInventoryImagePath, "Interface\\Inventory\\Ring.bmp");
			lv.ulFlags = LVBKIF_SOURCE_URL | LVBKIF_STYLE_NORMAL;			
			lv.pszImage = strInventoryImagePath;
			::SendMessage( m_hRing1, LVM_SETBKIMAGE, 0, (LPARAM)&lv);
			SendMessage( m_hRing1, LVM_SETTEXTBKCOLOR, 0, 0xffffffff );
			MoveWindow( m_hRing1, 194, 142, 48, 48, TRUE );

			ti.cbSize=sizeof(TOOLINFO);
			ti.uFlags=TTF_SUBCLASS | TTF_IDISHWND;
			ti.hwnd=hDlg;
			ti.uId=(WPARAM)m_hRing1;
			pText = GetTxtFromMgr(1150);
			

			ti.lpszText=pText;
			SendMessage( m_hTip,TTM_ADDTOOL,0,(LPARAM)(LPTOOLINFO)&ti );
			SendMessage( m_hTip, TTM_SETMAXTIPWIDTH, 0, MAX_TOOLTIP_WIDTH );
			SendMessage( m_hTip, TTM_SETDELAYTIME, TTDT_INITIAL, 0 );


			

			m_hRing2 = GetDlgItem( hDlg, IDC_LIST_RING2 );
			g_FontManager.SetFont( hDlg, IDC_LIST_RING2 );
			ListView_SetExtendedListViewStyle( m_hRing2, LVS_EX_FULLROWSELECT | LVS_EX_ONECLICKACTIVATE | LVS_EX_UNDERLINEHOT | LVS_EX_INFOTIP );
			::SendMessage( m_hRing2, LVM_SETBKIMAGE, 0, (LPARAM)&lv);
			SendMessage( m_hRing2, LVM_SETTEXTBKCOLOR, 0, 0xffffffff );
			MoveWindow( m_hRing2, 248, 142, 48, 48, TRUE );

			ti.cbSize=sizeof(TOOLINFO);
			ti.uFlags=TTF_SUBCLASS | TTF_IDISHWND;
			ti.hwnd=hDlg;
			ti.uId=(WPARAM)m_hRing2;
			pText = GetTxtFromMgr(1150);
			

			ti.lpszText=pText;
			SendMessage( m_hTip,TTM_ADDTOOL,0,(LPARAM)(LPTOOLINFO)&ti );
			SendMessage( m_hTip, TTM_SETMAXTIPWIDTH, 0, MAX_TOOLTIP_WIDTH );
			SendMessage( m_hTip, TTM_SETDELAYTIME, TTDT_INITIAL, 0 );





			m_hBelt  = GetDlgItem( hDlg, IDC_LIST_WAISTBAND );
			g_FontManager.SetFont( hDlg, IDC_LIST_WAISTBAND );
			ListView_SetExtendedListViewStyle( m_hBelt, LVS_EX_FULLROWSELECT | LVS_EX_ONECLICKACTIVATE | LVS_EX_UNDERLINEHOT | LVS_EX_INFOTIP );
			memset( strInventoryImagePath, 0, _MAX_PATH );
			strcpy ( strInventoryImagePath, "file://");			
			memset( strDirectoryPath, 0, _MAX_PATH );
			GetCurrentDirectory( _MAX_PATH, strDirectoryPath );
			DirectoryPathSize = strlen ( strDirectoryPath );
			if ( strDirectoryPath[ DirectoryPathSize - 1 ] != '\\' )
			{
				strcat( strDirectoryPath, "\\" );
			}
			strcat( strInventoryImagePath, strDirectoryPath );
			strcat( strInventoryImagePath, "Interface\\Inventory\\Belt.bmp");
			lv.ulFlags = LVBKIF_SOURCE_URL | LVBKIF_STYLE_NORMAL;			
			lv.pszImage = strInventoryImagePath;
			::SendMessage( m_hBelt, LVM_SETBKIMAGE, 0, (LPARAM)&lv);
			SendMessage( m_hBelt, LVM_SETTEXTBKCOLOR, 0, 0xffffffff );
			MoveWindow( m_hBelt, 75, 140, 48, 48, TRUE );

			ti.cbSize=sizeof(TOOLINFO);
			ti.uFlags=TTF_SUBCLASS | TTF_IDISHWND;
			ti.hwnd=hDlg;
			ti.uId=(WPARAM)m_hBelt;
			pText = GetTxtFromMgr(1151);
			

			ti.lpszText=pText;
			SendMessage( m_hTip,TTM_ADDTOOL,0,(LPARAM)(LPTOOLINFO)&ti );
			SendMessage( m_hTip, TTM_SETMAXTIPWIDTH, 0, MAX_TOOLTIP_WIDTH );
			SendMessage( m_hTip, TTM_SETDELAYTIME, TTDT_INITIAL, 0 );



			m_hNeck  = GetDlgItem( hDlg, IDC_LIST_NECKWEAR );
			g_FontManager.SetFont( hDlg, IDC_LIST_NECKWEAR );
			ListView_SetExtendedListViewStyle( m_hNeck, LVS_EX_FULLROWSELECT | LVS_EX_ONECLICKACTIVATE | LVS_EX_UNDERLINEHOT | LVS_EX_INFOTIP );
			memset( strInventoryImagePath, 0, _MAX_PATH );
			strcpy ( strInventoryImagePath, "file://");			
			memset( strDirectoryPath, 0, _MAX_PATH );
			GetCurrentDirectory( _MAX_PATH, strDirectoryPath );
			DirectoryPathSize = strlen ( strDirectoryPath );
			if ( strDirectoryPath[ DirectoryPathSize - 1 ] != '\\' )
			{
				strcat( strDirectoryPath, "\\" );
			}
			strcat( strInventoryImagePath, strDirectoryPath );
			strcat( strInventoryImagePath, "Interface\\Inventory\\NeckWear.bmp");
			lv.ulFlags = LVBKIF_SOURCE_URL | LVBKIF_STYLE_NORMAL;			
			lv.pszImage = strInventoryImagePath;
			::SendMessage( m_hNeck, LVM_SETBKIMAGE, 0, (LPARAM)&lv);
			SendMessage( m_hNeck, LVM_SETTEXTBKCOLOR, 0, 0xffffffff );
			MoveWindow( m_hNeck, 220, 85, 48, 48, TRUE );

			ti.cbSize=sizeof(TOOLINFO);
			ti.uFlags=TTF_SUBCLASS | TTF_IDISHWND;
			ti.hwnd=hDlg;
			ti.uId=(WPARAM)m_hNeck;
			pText = GetTxtFromMgr(1149);
			

			ti.lpszText=pText;
			SendMessage( m_hTip,TTM_ADDTOOL,0,(LPARAM)(LPTOOLINFO)&ti );
			SendMessage( m_hTip, TTM_SETMAXTIPWIDTH, 0, MAX_TOOLTIP_WIDTH );
			SendMessage( m_hTip, TTM_SETDELAYTIME, TTDT_INITIAL, 0 );




			m_hShoes  = GetDlgItem( hDlg, IDC_LIST_SHOE );
			g_FontManager.SetFont( hDlg, IDC_LIST_SHOE );
			ListView_SetExtendedListViewStyle( m_hShoes, LVS_EX_FULLROWSELECT | LVS_EX_ONECLICKACTIVATE | LVS_EX_UNDERLINEHOT | LVS_EX_INFOTIP );
			memset( strInventoryImagePath, 0, _MAX_PATH );
			strcpy ( strInventoryImagePath, "file://");			
			memset( strDirectoryPath, 0, _MAX_PATH );
			GetCurrentDirectory( _MAX_PATH, strDirectoryPath );
			DirectoryPathSize = strlen ( strDirectoryPath );
			if ( strDirectoryPath[ DirectoryPathSize - 1 ] != '\\' )
			{
				strcat( strDirectoryPath, "\\" );
			}
			strcat( strInventoryImagePath, strDirectoryPath );
			strcat( strInventoryImagePath, "Interface\\Inventory\\Shoes.bmp");
			lv.ulFlags = LVBKIF_SOURCE_URL | LVBKIF_STYLE_NORMAL;			
			lv.pszImage = strInventoryImagePath;
			::SendMessage( m_hShoes, LVM_SETBKIMAGE, 0, (LPARAM)&lv);
			SendMessage( m_hShoes, LVM_SETTEXTBKCOLOR, 0, 0xffffffff );
			MoveWindow( m_hShoes, 102, 214, 48, 48, TRUE );

			ti.cbSize=sizeof(TOOLINFO);
			ti.uFlags=TTF_SUBCLASS | TTF_IDISHWND;
			ti.hwnd=hDlg;
			ti.uId=(WPARAM)m_hShoes;
			pText = GetTxtFromMgr(1152);
			

			ti.lpszText=pText;
			SendMessage( m_hTip,TTM_ADDTOOL,0,(LPARAM)(LPTOOLINFO)&ti );
			SendMessage( m_hTip, TTM_SETMAXTIPWIDTH, 0, MAX_TOOLTIP_WIDTH );
			SendMessage( m_hTip, TTM_SETDELAYTIME, TTDT_INITIAL, 0 );


			

			m_hArmor  = GetDlgItem( hDlg, IDC_LIST_ARMOR );
			g_FontManager.SetFont( hDlg, IDC_LIST_ARMOR );
			ListView_SetExtendedListViewStyle( m_hArmor, LVS_EX_FULLROWSELECT | LVS_EX_ONECLICKACTIVATE | LVS_EX_UNDERLINEHOT | LVS_EX_INFOTIP );
			memset( strInventoryImagePath, 0, _MAX_PATH );
			strcpy ( strInventoryImagePath, "file://");			
			memset( strDirectoryPath, 0, _MAX_PATH );
			GetCurrentDirectory( _MAX_PATH, strDirectoryPath );
			DirectoryPathSize = strlen ( strDirectoryPath );
			if ( strDirectoryPath[ DirectoryPathSize - 1 ] != '\\' )
			{
				strcat( strDirectoryPath, "\\" );
			}
			strcat( strInventoryImagePath, strDirectoryPath );
			strcat( strInventoryImagePath, "Interface\\Inventory\\Armor.bmp");
			lv.ulFlags = LVBKIF_SOURCE_URL | LVBKIF_STYLE_NORMAL;			
			lv.pszImage = strInventoryImagePath;
			::SendMessage( m_hArmor, LVM_SETBKIMAGE, 0, (LPARAM)&lv);
			SendMessage( m_hArmor, LVM_SETTEXTBKCOLOR, 0, 0xffffffff );
			MoveWindow( m_hArmor, 74, 83, 48, 48, TRUE );

			ti.cbSize=sizeof(TOOLINFO);
			ti.uFlags=TTF_SUBCLASS | TTF_IDISHWND;
			ti.hwnd=hDlg;
			ti.uId=(WPARAM)m_hArmor;
			pText = GetTxtFromMgr(1145);
			

			ti.lpszText=pText;
			SendMessage( m_hTip,TTM_ADDTOOL,0,(LPARAM)(LPTOOLINFO)&ti );
			SendMessage( m_hTip, TTM_SETMAXTIPWIDTH, 0, MAX_TOOLTIP_WIDTH );
			SendMessage( m_hTip, TTM_SETDELAYTIME, TTDT_INITIAL, 0 );




			m_hDress = GetDlgItem( hDlg, IDC_LIST_DRESS );
			g_FontManager.SetFont( hDlg, IDC_LIST_DRESS );
			ListView_SetExtendedListViewStyle( m_hDress, LVS_EX_FULLROWSELECT | LVS_EX_ONECLICKACTIVATE | LVS_EX_UNDERLINEHOT | LVS_EX_INFOTIP );
			memset( strInventoryImagePath, 0, _MAX_PATH );
			strcpy ( strInventoryImagePath, "file://");			
			memset( strDirectoryPath, 0, _MAX_PATH );
			GetCurrentDirectory( _MAX_PATH, strDirectoryPath );
			DirectoryPathSize = strlen ( strDirectoryPath );
			if ( strDirectoryPath[ DirectoryPathSize - 1 ] != '\\' )
			{
				strcat( strDirectoryPath, "\\" );
			}
			strcat( strInventoryImagePath, strDirectoryPath );
			strcat( strInventoryImagePath, "Interface\\Inventory\\Dress.bmp");
			lv.ulFlags = LVBKIF_SOURCE_URL | LVBKIF_STYLE_NORMAL;			
			lv.pszImage = strInventoryImagePath;
			::SendMessage( m_hDress, LVM_SETBKIMAGE, 0, (LPARAM)&lv);
			SendMessage( m_hDress, LVM_SETTEXTBKCOLOR, 0, 0xffffffff );
			MoveWindow( m_hDress, 13, 83, 48, 48, TRUE );

			ti.cbSize=sizeof(TOOLINFO);
			ti.uFlags=TTF_SUBCLASS | TTF_IDISHWND;
			ti.hwnd=hDlg;
			ti.uId=(WPARAM)m_hDress;
			pText = GetTxtFromMgr(1157);
			

			ti.lpszText=pText;
			SendMessage( m_hTip,TTM_ADDTOOL,0,(LPARAM)(LPTOOLINFO)&ti );
			SendMessage( m_hTip, TTM_SETMAXTIPWIDTH, 0, MAX_TOOLTIP_WIDTH );
			SendMessage( m_hTip, TTM_SETDELAYTIME, TTDT_INITIAL, 0 );


			
			m_hMantle  = GetDlgItem( hDlg, IDC_LIST_MANTLE );
			g_FontManager.SetFont( hDlg, IDC_LIST_MANTLE );
			ListView_SetExtendedListViewStyle( m_hMantle, LVS_EX_FULLROWSELECT | LVS_EX_ONECLICKACTIVATE | LVS_EX_UNDERLINEHOT | LVS_EX_INFOTIP );
			memset( strInventoryImagePath, 0, _MAX_PATH );
			strcpy ( strInventoryImagePath, "file://");
			memset( strDirectoryPath, 0, _MAX_PATH );
			GetCurrentDirectory( _MAX_PATH, strDirectoryPath );
			DirectoryPathSize = strlen ( strDirectoryPath );
			if ( strDirectoryPath[ DirectoryPathSize - 1 ] != '\\' )
			{
				strcat( strDirectoryPath, "\\" );
			}
			strcat( strInventoryImagePath, strDirectoryPath );
			strcat( strInventoryImagePath, "Interface\\Inventory\\Mantle.bmp" );
			lv.ulFlags = LVBKIF_SOURCE_URL | LVBKIF_STYLE_NORMAL;
			lv.pszImage = strInventoryImagePath;
			::SendMessage( m_hMantle, LVM_SETBKIMAGE, 0, (LPARAM)&lv);
			SendMessage( m_hMantle, LVM_SETTEXTBKCOLOR, 0, 0xffffffff );
			MoveWindow( m_hMantle, 192, 27, 48, 48, TRUE );

			ti.cbSize=sizeof(TOOLINFO);
			ti.uFlags=TTF_SUBCLASS | TTF_IDISHWND;
			ti.hwnd=hDlg;
			ti.uId=(WPARAM)m_hMantle;
			pText = GetTxtFromMgr(1153);
			

			ti.lpszText=pText;
			SendMessage( m_hTip,TTM_ADDTOOL,0,(LPARAM)(LPTOOLINFO)&ti );
			SendMessage( m_hTip, TTM_SETMAXTIPWIDTH, 0, MAX_TOOLTIP_WIDTH );
			SendMessage( m_hTip, TTM_SETDELAYTIME, TTDT_INITIAL, 0 );



			m_hBag = GetDlgItem( hDlg, IDC_LIST_BAG );
			g_FontManager.SetFont( hDlg, IDC_LIST_BAG );
			ListView_SetExtendedListViewStyle( m_hBag, LVS_EX_FULLROWSELECT | LVS_EX_ONECLICKACTIVATE | LVS_EX_UNDERLINEHOT | LVS_EX_INFOTIP );
			memset( strInventoryImagePath, 0, _MAX_PATH );
			strcpy ( strInventoryImagePath, "file://");
			memset( strDirectoryPath, 0, _MAX_PATH );
			GetCurrentDirectory( _MAX_PATH, strDirectoryPath );
			DirectoryPathSize = strlen ( strDirectoryPath );
			if ( strDirectoryPath[ DirectoryPathSize - 1 ] != '\\' )
			{
				strcat( strDirectoryPath, "\\" );
			}
			strcat( strInventoryImagePath, strDirectoryPath );
			strcat( strInventoryImagePath, "Interface\\Inventory\\Bag.bmp" );
			lv.ulFlags = LVBKIF_SOURCE_URL | LVBKIF_STYLE_NORMAL;
			lv.pszImage = strInventoryImagePath;
			::SendMessage( m_hBag, LVM_SETBKIMAGE, 0, (LPARAM)&lv);
			SendMessage( m_hBag, LVM_SETTEXTBKCOLOR, 0, 0xffffffff );
			MoveWindow( m_hBag, 252, 27, 48, 48, TRUE );
			
			ti.cbSize=sizeof(TOOLINFO);
			ti.uFlags=TTF_SUBCLASS | TTF_IDISHWND;
			ti.hwnd=hDlg;
			ti.uId=(WPARAM)m_hBag;
			pText = GetTxtFromMgr(1155);
			

			ti.lpszText=pText;
			SendMessage( m_hTip,TTM_ADDTOOL,0,(LPARAM)(LPTOOLINFO)&ti );
			SendMessage( m_hTip, TTM_SETMAXTIPWIDTH, 0, MAX_TOOLTIP_WIDTH );
			SendMessage( m_hTip, TTM_SETDELAYTIME, TTDT_INITIAL, 0 );




			m_hMoveToHorse = GetDlgItem( hDlg, IDC_LIST_MOVEITEM_HORSE );
			ListView_SetExtendedListViewStyle( m_hMoveToHorse, LVS_EX_FULLROWSELECT | LVS_EX_ONECLICKACTIVATE | LVS_EX_UNDERLINEHOT | LVS_EX_INFOTIP );
			memset( strInventoryImagePath, 0, _MAX_PATH );
			strcpy ( strInventoryImagePath, "file://");			
			memset( strDirectoryPath, 0, _MAX_PATH );
			GetCurrentDirectory( _MAX_PATH, strDirectoryPath );
			DirectoryPathSize = strlen ( strDirectoryPath );
			if ( strDirectoryPath[ DirectoryPathSize - 1 ] != '\\' )
			{
				strcat( strDirectoryPath, "\\" );
			}
			strcat( strInventoryImagePath, strDirectoryPath );
			strcat( strInventoryImagePath, "Interface\\Inventory\\Horse.bmp");
			lv.ulFlags = LVBKIF_SOURCE_URL | LVBKIF_STYLE_NORMAL;			
			lv.pszImage = strInventoryImagePath;
			::SendMessage( m_hMoveToHorse, LVM_SETBKIMAGE, 0, (LPARAM)&lv);
			SendMessage( m_hMoveToHorse, LVM_SETTEXTBKCOLOR, 0, 0xffffffff );
			MoveWindow( m_hMoveToHorse, 205, 214, 48, 48, TRUE );

			
			m_hMoveToBag = GetDlgItem( hDlg, IDC_LIST_MOVEITEM_BAG );
			ListView_SetExtendedListViewStyle( m_hMoveToBag, LVS_EX_FULLROWSELECT | LVS_EX_ONECLICKACTIVATE | LVS_EX_UNDERLINEHOT | LVS_EX_INFOTIP );
			memset( strInventoryImagePath, 0, _MAX_PATH );
			strcpy ( strInventoryImagePath, "file://");			
			memset( strDirectoryPath, 0, _MAX_PATH );
			GetCurrentDirectory( _MAX_PATH, strDirectoryPath );
			DirectoryPathSize = strlen ( strDirectoryPath );
			if ( strDirectoryPath[ DirectoryPathSize - 1 ] != '\\' )
			{
				strcat( strDirectoryPath, "\\" );
			}
			strcat( strInventoryImagePath, strDirectoryPath );
			strcat( strInventoryImagePath, "Interface\\Inventory\\MoveBag.bmp");
			lv.ulFlags = LVBKIF_SOURCE_URL | LVBKIF_STYLE_NORMAL;			
			lv.pszImage = strInventoryImagePath;
			::SendMessage( m_hMoveToBag, LVM_SETBKIMAGE, 0, (LPARAM)&lv);
			SendMessage( m_hMoveToBag, LVM_SETTEXTBKCOLOR, 0, 0xffffffff );
			MoveWindow( m_hMoveToBag, 256, 214, 48, 48, TRUE );

			




			/*
			m_hUserInventory = GetDlgItem( hDlg, IDC_LIST_USER_INVENTORY );
			g_FontManager.SetFont( hDlg, IDC_LIST_USER_INVENTORY, 12 );			
			ListView_SetExtendedListViewStyle( m_hUserInventory, LVS_EX_FULLROWSELECT | LVS_EX_ONECLICKACTIVATE | LVS_EX_INFOTIP );
			*/

			/*			
			memset( strInventoryImagePath, 0, _MAX_PATH );
			strcpy ( strInventoryImagePath, "file://");			
			memset( strDirectoryPath, 0, _MAX_PATH );
			GetCurrentDirectory( _MAX_PATH, strDirectoryPath );			
			strcat( strInventoryImagePath, strDirectoryPath );
			strcat( strInventoryImagePath, "\\Interface\\Inventory\\Inventory.bmp");
			lv.ulFlags = LVBKIF_SOURCE_URL | LVBKIF_STYLE_NORMAL;			
			lv.pszImage = strInventoryImagePath;
			::SendMessage(m_hUserInventory, LVM_SETBKIMAGE, 0, (LPARAM)&lv);
			SendMessage( m_hUserInventory, LVM_SETTEXTBKCOLOR, 0, 0xffffffff );
			*/
			
			cltClient *pclclient = (cltClient *)pclClient;
			pclclient->pclImageList->ConnectToListView( m_hWeapon1, LVSIL_NORMAL );
			pclclient->pclImageList->ConnectToListView( m_hWeapon2, LVSIL_NORMAL );
			pclclient->pclImageList->ConnectToListView( m_hHelmet, LVSIL_NORMAL );
			pclclient->pclImageList->ConnectToListView( m_hHat, LVSIL_NORMAL );
			pclclient->pclImageList->ConnectToListView( m_hArmor, LVSIL_NORMAL );
			pclclient->pclImageList->ConnectToListView( m_hDress, LVSIL_NORMAL );
			pclclient->pclImageList->ConnectToListView( m_hBelt, LVSIL_NORMAL );
			pclclient->pclImageList->ConnectToListView( m_hRing1, LVSIL_NORMAL );
			pclclient->pclImageList->ConnectToListView( m_hRing2, LVSIL_NORMAL );
			pclclient->pclImageList->ConnectToListView( m_hNeck, LVSIL_NORMAL );
			pclclient->pclImageList->ConnectToListView( m_hShoes, LVSIL_NORMAL );
			pclclient->pclImageList->ConnectToListView( m_hMantle, LVSIL_NORMAL );
			pclclient->pclImageList->ConnectToListView( m_hBag, LVSIL_NORMAL );
			pclclient->pclImageList->ConnectToListView( m_hMoveToHorse, LVSIL_NORMAL );
			pclclient->pclImageList->ConnectToListView( m_hMoveToBag, LVSIL_NORMAL );

			return TRUE;
		}
		break;
	/*
	case WM_CTLCOLOREDIT:
		{
			if ( m_hUserInventory == (HWND)lParam )
			{
				MessageBox( hDlg, "tesT", "SEt", MB_OK|MB_TOPMOST );
				HDC hdc = (HDC)wParam;
				SetBkMode( hdc, TRANSPARENT );
			}
			else if ( ListView_GetEditControl( m_hUserInventory ) == (HWND)lParam )
			{
				MessageBox( hDlg, "tesT", "SEt", MB_OK|MB_TOPMOST );
				HDC hdc = (HDC)wParam;
				SetBkMode( hdc, TRANSPARENT );
			}

			return TRUE;
		}
		break;
	*/

	case WM_TIMER:
		{
			bool bBagHave = false;
			if ( pclClient->pclCM->CR[1]->pclCI->clCharItem.clItem[ PERSONITEM_BAG ].siUnique > 0 )
			{
				bBagHave = true;
			}

			if(  m_bHorseHave != pclClient->pclCM->CR[1]->HaveHorse() ||
				m_bBagHave != bBagHave
				)
			{
				// 탭 다이얼로그 변화 체크 ( 말이 있느냐 없느냐에 따라 틀려져야지.. )
				ShowTabDlg();
			}
			
			

			if ( IsSummonInfoChanged() )
			{
				LoadSummonInfo( hDlg );
				LoadDataAndSetting( hDlg );
			}
			else if ( IsChanged() )
			{
				LoadDataAndSetting( hDlg );
			}
		}
		break;


		/*
	case WM_PAINT:
		{
			if ( m_bSelectPerson )
			{
				g_UserInventoryDlg.Show();
			}
			else
			{
				g_HorseInventoryDlg.Show();
			}
		}
		break;
		*/
	
	/*
	case WM_NCHITTEST:
		{
			int nHit = DefWindowProc( hDlg, iMsg, wParam, lParam );

			if ( nHit == HTCLIENT )
			{
				SendMessage( hDlg, WM_NCLBUTTONDOWN, HTCAPTION,lParam ); 
			}
		}
		break;
	*/
	
		
	case WM_NOTIFY:
		{
			LPNMHDR						hdr;
			
			LPNMITEMACTIVATE			nia;
			LPNMLVGETINFOTIP			ngt;
			LPNMLISTVIEW				nlv;
			hdr = ( LPNMHDR )			lParam;
			ngt = ( LPNMLVGETINFOTIP )	lParam;
			nlv = ( LPNMLISTVIEW )		lParam;
			nia = (LPNMITEMACTIVATE)	lParam;
			
			/*
			switch ( hdr->idFrom )
			{
			case IDC_LIST_MOVEITEM_HORSE:
				{
					switch( hdr->code )
					{
					case NM_HOVER:
						
						break;
					}
				}
				break;
			}
			*/
			
			switch( hdr->code )
			{
			case LVN_KEYDOWN:
				{
					LPNMLVKEYDOWN lpkeydown = ( LPNMLVKEYDOWN) lParam;
					if ( lpkeydown->wVKey == VK_ESCAPE )
					{
						Hide();
						return TRUE;
					}
				}
				break;
			}

			SI32 siKind = 0;
			bool bCheck = false;

			switch ( hdr->idFrom )
			{
			case IDC_TAB_SELECT_HERO:
				{
					if( hdr->code == TCN_SELCHANGE )
					{
						int index = TabCtrl_GetCurSel(hdr->hwndFrom);
						
						char TabText[ 128 ] = "";
						
						TCITEM tc;
						tc.mask = TCIF_TEXT;
						tc.pszText = TabText;
						tc.cchTextMax = sizeof ( TabText ) - 1;
						
						if ( TabCtrl_GetItem( m_hTabCtrlHeroSelect, index, &tc ) == TRUE )
						{
							if ( strcmp( tc.pszText, pclClient->pclCM->CR[1]->GetName() ) == 0 )
							{
								m_siCurrentSummonIndex = -1;
								
								LoadDataAndSetting( hDlg );
								
								char strInventoryImagePath[ _MAX_PATH ];
								char strDirectoryPath[ _MAX_PATH ];
								SI32 DirectoryPathSize = 0;
								TOOLINFO ti;

								ListView_SetExtendedListViewStyle( m_hWeapon1, LVS_EX_FULLROWSELECT | LVS_EX_ONECLICKACTIVATE | LVS_EX_UNDERLINEHOT | LVS_EX_INFOTIP );
								memset( strInventoryImagePath, 0, _MAX_PATH );
								strcpy ( strInventoryImagePath, "file://");			
								memset( strDirectoryPath, 0, _MAX_PATH );
								GetCurrentDirectory( _MAX_PATH, strDirectoryPath );
								DirectoryPathSize = strlen ( strDirectoryPath );
								if ( strDirectoryPath[ DirectoryPathSize - 1 ] != '\\' )
								{
									strcat( strDirectoryPath, "\\" );
								}
								DirectoryPathSize = strlen ( strDirectoryPath );
								if ( strDirectoryPath[ DirectoryPathSize - 1 ] != '\\' )
								{
									strcat( strDirectoryPath, "\\" );
								}
								strcat( strInventoryImagePath, strDirectoryPath );
								strcat( strInventoryImagePath, "Interface\\Inventory\\SelectedWeapon.bmp");
								lv.ulFlags = LVBKIF_SOURCE_URL | LVBKIF_STYLE_NORMAL;			
								lv.pszImage = strInventoryImagePath;
								::SendMessage(m_hWeapon1, LVM_SETBKIMAGE, 0, (LPARAM)&lv);
								SendMessage( m_hWeapon1, LVM_SETTEXTBKCOLOR, 0, 0xffffffff );

								ListView_SetExtendedListViewStyle( m_hBag, LVS_EX_FULLROWSELECT | LVS_EX_ONECLICKACTIVATE | LVS_EX_UNDERLINEHOT | LVS_EX_INFOTIP );
								memset( strInventoryImagePath, 0, _MAX_PATH );
								strcpy ( strInventoryImagePath, "file://");
								memset( strDirectoryPath, 0, _MAX_PATH );
								GetCurrentDirectory( _MAX_PATH, strDirectoryPath );
								DirectoryPathSize = strlen ( strDirectoryPath );
								if ( strDirectoryPath[ DirectoryPathSize - 1 ] != '\\' )
								{
									strcat( strDirectoryPath, "\\" );
								}
								strcat( strInventoryImagePath, strDirectoryPath );
								strcat( strInventoryImagePath, "Interface\\Inventory\\Bag.bmp" );
								lv.ulFlags = LVBKIF_SOURCE_URL | LVBKIF_STYLE_NORMAL;
								lv.pszImage = strInventoryImagePath;
								::SendMessage( m_hBag, LVM_SETBKIMAGE, 0, (LPARAM)&lv);
								SendMessage( m_hBag, LVM_SETTEXTBKCOLOR, 0, 0xffffffff );								
																
								ti.cbSize=sizeof(TOOLINFO);
								ti.uFlags=TTF_SUBCLASS | TTF_IDISHWND;
								ti.hwnd=hDlg;
								ti.uId=(WPARAM)m_hBag;
								char* pText = GetTxtFromMgr(1155);
								

								ti.lpszText=pText;
								SendMessage( m_hTip,TTM_ADDTOOL,0,(LPARAM)(LPTOOLINFO)&ti );
								SendMessage( m_hTip, TTM_SETMAXTIPWIDTH, 0, MAX_TOOLTIP_WIDTH );
								SendMessage( m_hTip, TTM_SETDELAYTIME, TTDT_INITIAL, 0 );
					

								ListView_SetExtendedListViewStyle( m_hWeapon2, LVS_EX_FULLROWSELECT | LVS_EX_ONECLICKACTIVATE | LVS_EX_UNDERLINEHOT | LVS_EX_INFOTIP );
								memset( strInventoryImagePath, 0, _MAX_PATH );
								strcpy ( strInventoryImagePath, "file://");			
								memset( strDirectoryPath, 0, _MAX_PATH );
								GetCurrentDirectory( _MAX_PATH, strDirectoryPath );
								DirectoryPathSize = strlen ( strDirectoryPath );
								if ( strDirectoryPath[ DirectoryPathSize - 1 ] != '\\' )
								{
									strcat( strDirectoryPath, "\\" );
								}
								DirectoryPathSize = strlen ( strDirectoryPath );
								if ( strDirectoryPath[ DirectoryPathSize - 1 ] != '\\' )
								{
									strcat( strDirectoryPath, "\\" );
								}
								strcat( strInventoryImagePath, strDirectoryPath );
								strcat( strInventoryImagePath, "Interface\\Inventory\\Weapon.bmp");
								lv.ulFlags = LVBKIF_SOURCE_URL | LVBKIF_STYLE_NORMAL;			
								lv.pszImage = strInventoryImagePath;
								::SendMessage(m_hWeapon2, LVM_SETBKIMAGE, 0, (LPARAM)&lv);
								SendMessage( m_hWeapon2, LVM_SETTEXTBKCOLOR, 0, 0xffffffff );								
								
								ti.cbSize=sizeof(TOOLINFO);
								ti.uFlags=TTF_SUBCLASS | TTF_IDISHWND;
								ti.hwnd=hDlg;
								ti.uId=(WPARAM)m_hWeapon2;
								pText = GetTxtFromMgr(1148);
								

								ti.lpszText=pText;
								SendMessage( m_hTip,TTM_ADDTOOL,0,(LPARAM)(LPTOOLINFO)&ti );
								SendMessage( m_hTip, TTM_SETMAXTIPWIDTH, 0, MAX_TOOLTIP_WIDTH );
								SendMessage( m_hTip, TTM_SETDELAYTIME, TTDT_INITIAL, 0 );

								
							}
							else
							{
								m_siCurrentSummonIndex = index - 1;
								
								LoadDataAndSetting( hDlg );
								
								char strInventoryImagePath[ _MAX_PATH ];
								char strDirectoryPath[ _MAX_PATH ];
								SI32 DirectoryPathSize = 0;
								TOOLINFO ti;
								
								ListView_SetExtendedListViewStyle( m_hWeapon1, LVS_EX_FULLROWSELECT | LVS_EX_ONECLICKACTIVATE | LVS_EX_UNDERLINEHOT | LVS_EX_INFOTIP );
								memset( strInventoryImagePath, 0, _MAX_PATH );
								strcpy ( strInventoryImagePath, "file://");			
								memset( strDirectoryPath, 0, _MAX_PATH );
								GetCurrentDirectory( _MAX_PATH, strDirectoryPath );
								DirectoryPathSize = strlen ( strDirectoryPath );
								if ( strDirectoryPath[ DirectoryPathSize - 1 ] != '\\' )
								{
									strcat( strDirectoryPath, "\\" );
								}
								DirectoryPathSize = strlen ( strDirectoryPath );
								if ( strDirectoryPath[ DirectoryPathSize - 1 ] != '\\' )
								{
									strcat( strDirectoryPath, "\\" );
								}
								strcat( strInventoryImagePath, strDirectoryPath );
								strcat( strInventoryImagePath, "Interface\\Inventory\\SelectedWeapon.bmp");
								lv.ulFlags = LVBKIF_SOURCE_URL | LVBKIF_STYLE_NORMAL;			
								lv.pszImage = strInventoryImagePath;
								::SendMessage(m_hWeapon1, LVM_SETBKIMAGE, 0, (LPARAM)&lv);
								SendMessage( m_hWeapon1, LVM_SETTEXTBKCOLOR, 0, 0xffffffff );
								
								ListView_SetExtendedListViewStyle( m_hWeapon2, LVS_EX_FULLROWSELECT | LVS_EX_ONECLICKACTIVATE | LVS_EX_UNDERLINEHOT | LVS_EX_INFOTIP );
								memset( strInventoryImagePath, 0, _MAX_PATH );
								strcpy ( strInventoryImagePath, "file://");			
								memset( strDirectoryPath, 0, _MAX_PATH );
								GetCurrentDirectory( _MAX_PATH, strDirectoryPath );
								DirectoryPathSize = strlen ( strDirectoryPath );
								if ( strDirectoryPath[ DirectoryPathSize - 1 ] != '\\' )
								{
									strcat( strDirectoryPath, "\\" );
								}
								DirectoryPathSize = strlen ( strDirectoryPath );
								if ( strDirectoryPath[ DirectoryPathSize - 1 ] != '\\' )
								{
									strcat( strDirectoryPath, "\\" );
								}
								strcat( strInventoryImagePath, strDirectoryPath );
								strcat( strInventoryImagePath, "Interface\\Inventory\\EquipDisable.bmp");
								lv.ulFlags = LVBKIF_SOURCE_URL | LVBKIF_STYLE_NORMAL;			
								lv.pszImage = strInventoryImagePath;
								::SendMessage(m_hWeapon2, LVM_SETBKIMAGE, 0, (LPARAM)&lv);
								SendMessage( m_hWeapon2, LVM_SETTEXTBKCOLOR, 0, 0xffffffff );									
								
								ti.cbSize=sizeof(TOOLINFO);
								ti.uFlags=TTF_SUBCLASS | TTF_IDISHWND;
								ti.hwnd=hDlg;
								ti.uId=(WPARAM)m_hWeapon2;
								char* pText = GetTxtFromMgr(1147);
								

								ti.lpszText=pText;
								SendMessage( m_hTip,TTM_ADDTOOL,0,(LPARAM)(LPTOOLINFO)&ti );
								SendMessage( m_hTip, TTM_SETMAXTIPWIDTH, 0, MAX_TOOLTIP_WIDTH );
								SendMessage( m_hTip, TTM_SETDELAYTIME, TTDT_INITIAL, 0 );
								
								
								ListView_SetExtendedListViewStyle( m_hBag, LVS_EX_FULLROWSELECT | LVS_EX_ONECLICKACTIVATE | LVS_EX_UNDERLINEHOT | LVS_EX_INFOTIP );
								memset( strInventoryImagePath, 0, _MAX_PATH );
								strcpy ( strInventoryImagePath, "file://");
								memset( strDirectoryPath, 0, _MAX_PATH );
								GetCurrentDirectory( _MAX_PATH, strDirectoryPath );
								DirectoryPathSize = strlen ( strDirectoryPath );
								if ( strDirectoryPath[ DirectoryPathSize - 1 ] != '\\' )
								{
									strcat( strDirectoryPath, "\\" );
								}
								strcat( strInventoryImagePath, strDirectoryPath );
								strcat( strInventoryImagePath, "Interface\\Inventory\\EquipDisable.bmp" );
								lv.ulFlags = LVBKIF_SOURCE_URL | LVBKIF_STYLE_NORMAL;
								lv.pszImage = strInventoryImagePath;
								::SendMessage( m_hBag, LVM_SETBKIMAGE, 0, (LPARAM)&lv);
								SendMessage( m_hBag, LVM_SETTEXTBKCOLOR, 0, 0xffffffff );
								
								ti.cbSize=sizeof(TOOLINFO);
								ti.uFlags=TTF_SUBCLASS | TTF_IDISHWND;
								ti.hwnd=hDlg;
								ti.uId=(WPARAM)m_hBag;
								pText = GetTxtFromMgr(1154);
								

								ti.lpszText=pText;
								SendMessage( m_hTip,TTM_ADDTOOL,0,(LPARAM)(LPTOOLINFO)&ti );
								SendMessage( m_hTip, TTM_SETMAXTIPWIDTH, 0, MAX_TOOLTIP_WIDTH );
								SendMessage( m_hTip, TTM_SETDELAYTIME, TTDT_INITIAL, 0 );
								
								
							}
						}
					}
				}
				break;
			case IDC_TAB_INVENTORY:
				{
					if( hdr->code == TCN_SELCHANGE )
					{
						
						int index = TabCtrl_GetCurSel(hdr->hwndFrom);
						
						char TabText[ 128 ] = "";
						
						TCITEM tc;
						tc.mask = TCIF_TEXT;
						tc.pszText = TabText;
						tc.cchTextMax = sizeof ( TabText ) - 1;
						
						if ( TabCtrl_GetItem( m_hTabCtrl, index, &tc ) == TRUE )
						{
							TOOLINFO ti;
							
							char* pText = GetTxtFromMgr(1159);
							char* pText1 = GetTxtFromMgr(372);
							char* pText2 = GetTxtFromMgr(1033);
						

							if ( strcmp( tc.pszText, pText ) == 0 )
							{
								char strInventoryImagePath[ _MAX_PATH ];
								char strDirectoryPath[ _MAX_PATH ];
								ListView_SetExtendedListViewStyle( m_hMoveToHorse, LVS_EX_FULLROWSELECT | LVS_EX_ONECLICKACTIVATE | LVS_EX_UNDERLINEHOT | LVS_EX_INFOTIP );
								memset( strInventoryImagePath, 0, _MAX_PATH );
								strcpy ( strInventoryImagePath, "file://");			
								memset( strDirectoryPath, 0, _MAX_PATH );
								GetCurrentDirectory( _MAX_PATH, strDirectoryPath );
								SI32 DirectoryPathSize = strlen ( strDirectoryPath );
								if ( strDirectoryPath[ DirectoryPathSize - 1 ] != '\\' )
								{
									strcat( strDirectoryPath, "\\" );
								}
								strcat( strInventoryImagePath, strDirectoryPath );
								strcat( strInventoryImagePath, "Interface\\Inventory\\Horse.bmp");
								lv.ulFlags = LVBKIF_SOURCE_URL | LVBKIF_STYLE_NORMAL;			
								lv.pszImage = strInventoryImagePath;
								::SendMessage( m_hMoveToHorse, LVM_SETBKIMAGE, 0, (LPARAM)&lv);
								SendMessage( m_hMoveToHorse, LVM_SETTEXTBKCOLOR, 0, 0xffffffff );
								
								ti.cbSize=sizeof(TOOLINFO);
								ti.uFlags=TTF_SUBCLASS | TTF_IDISHWND;
								ti.hwnd=hDlg;
								ti.uId=(WPARAM)m_hMoveToHorse;
								pText = GetTxtFromMgr(1160);
								

								ti.lpszText=pText;
								SendMessage( m_hTip,TTM_ADDTOOL,0,(LPARAM)(LPTOOLINFO)&ti );
								SendMessage( m_hTip, TTM_SETMAXTIPWIDTH, 0, MAX_TOOLTIP_WIDTH );
								SendMessage( m_hTip, TTM_SETDELAYTIME, TTDT_INITIAL, 0 );
								
								
								
								ListView_SetExtendedListViewStyle( m_hMoveToBag, LVS_EX_FULLROWSELECT | LVS_EX_ONECLICKACTIVATE | LVS_EX_UNDERLINEHOT | LVS_EX_INFOTIP );
								memset( strInventoryImagePath, 0, _MAX_PATH );
								strcpy ( strInventoryImagePath, "file://");			
								memset( strDirectoryPath, 0, _MAX_PATH );
								GetCurrentDirectory( _MAX_PATH, strDirectoryPath );
								DirectoryPathSize = strlen ( strDirectoryPath );
								if ( strDirectoryPath[ DirectoryPathSize - 1 ] != '\\' )
								{
									strcat( strDirectoryPath, "\\" );
								}
								strcat( strInventoryImagePath, strDirectoryPath );
								strcat( strInventoryImagePath, "Interface\\Inventory\\MoveBag.bmp");
								lv.ulFlags = LVBKIF_SOURCE_URL | LVBKIF_STYLE_NORMAL;			
								lv.pszImage = strInventoryImagePath;
								::SendMessage( m_hMoveToBag, LVM_SETBKIMAGE, 0, (LPARAM)&lv);
								SendMessage( m_hMoveToBag, LVM_SETTEXTBKCOLOR, 0, 0xffffffff );
								
								ti.cbSize=sizeof(TOOLINFO);
								ti.uFlags=TTF_SUBCLASS | TTF_IDISHWND;
								ti.hwnd=hDlg;
								ti.uId=(WPARAM)m_hMoveToBag;

								pText = GetTxtFromMgr(1161);
								

								ti.lpszText=pText;
								SendMessage( m_hTip,TTM_ADDTOOL,0,(LPARAM)(LPTOOLINFO)&ti );
								SendMessage( m_hTip, TTM_SETMAXTIPWIDTH, 0, MAX_TOOLTIP_WIDTH );
								SendMessage( m_hTip, TTM_SETDELAYTIME, TTDT_INITIAL, 0 );
								
								
								m_bSelectPerson = true;
								m_bSelectBag = false;
								m_pHorseInventoryDlg->Hide();
								m_pBagInventoryDlg->Hide();
								m_pUserInventoryDlg->Show();
							}
							else if ( strcmp( tc.pszText, pText1 ) == 0 )
							{
								char strInventoryImagePath[ _MAX_PATH ];
								char strDirectoryPath[ _MAX_PATH ];
								ListView_SetExtendedListViewStyle( m_hMoveToHorse, LVS_EX_FULLROWSELECT | LVS_EX_ONECLICKACTIVATE | LVS_EX_UNDERLINEHOT | LVS_EX_INFOTIP );
								memset( strInventoryImagePath, 0, _MAX_PATH );
								strcpy ( strInventoryImagePath, "file://");			
								memset( strDirectoryPath, 0, _MAX_PATH );
								GetCurrentDirectory( _MAX_PATH, strDirectoryPath );
								SI32 DirectoryPathSize = strlen ( strDirectoryPath );
								if ( strDirectoryPath[ DirectoryPathSize - 1 ] != '\\' )
								{
									strcat( strDirectoryPath, "\\" );
								}
								strcat( strInventoryImagePath, strDirectoryPath );
								strcat( strInventoryImagePath, "Interface\\Inventory\\Hero.bmp");
								lv.ulFlags = LVBKIF_SOURCE_URL | LVBKIF_STYLE_NORMAL;			
								lv.pszImage = strInventoryImagePath;
								::SendMessage( m_hMoveToHorse, LVM_SETBKIMAGE, 0, (LPARAM)&lv);
								SendMessage( m_hMoveToHorse, LVM_SETTEXTBKCOLOR, 0, 0xffffffff );
								
								ti.cbSize=sizeof(TOOLINFO);
								ti.uFlags=TTF_SUBCLASS | TTF_IDISHWND;
								ti.hwnd=hDlg;
								ti.uId=(WPARAM)m_hMoveToHorse;
								pText1 = GetTxtFromMgr(1162);
								ti.lpszText= pText1;
								SendMessage( m_hTip,TTM_ADDTOOL,0,(LPARAM)(LPTOOLINFO)&ti );
								SendMessage( m_hTip, TTM_SETMAXTIPWIDTH, 0, MAX_TOOLTIP_WIDTH );
								SendMessage( m_hTip, TTM_SETDELAYTIME, TTDT_INITIAL, 0 );
								
								
								ListView_SetExtendedListViewStyle( m_hMoveToBag, LVS_EX_FULLROWSELECT | LVS_EX_ONECLICKACTIVATE | LVS_EX_UNDERLINEHOT | LVS_EX_INFOTIP );
								memset( strInventoryImagePath, 0, _MAX_PATH );
								strcpy ( strInventoryImagePath, "file://");			
								memset( strDirectoryPath, 0, _MAX_PATH );
								GetCurrentDirectory( _MAX_PATH, strDirectoryPath );
								DirectoryPathSize = strlen ( strDirectoryPath );
								if ( strDirectoryPath[ DirectoryPathSize - 1 ] != '\\' )
								{
									strcat( strDirectoryPath, "\\" );
								}
								strcat( strInventoryImagePath, strDirectoryPath );
								strcat( strInventoryImagePath, "Interface\\Inventory\\MoveBag.bmp");
								lv.ulFlags = LVBKIF_SOURCE_URL | LVBKIF_STYLE_NORMAL;			
								lv.pszImage = strInventoryImagePath;
								::SendMessage( m_hMoveToBag, LVM_SETBKIMAGE, 0, (LPARAM)&lv);
								SendMessage( m_hMoveToBag, LVM_SETTEXTBKCOLOR, 0, 0xffffffff );
								
								ti.cbSize=sizeof(TOOLINFO);
								ti.uFlags=TTF_SUBCLASS | TTF_IDISHWND;
								ti.hwnd=hDlg;
								ti.uId=(WPARAM)m_hMoveToBag;
								pText1 = GetTxtFromMgr(1161);
								
								ti.lpszText= pText1;
								SendMessage( m_hTip,TTM_ADDTOOL,0,(LPARAM)(LPTOOLINFO)&ti );
								SendMessage( m_hTip, TTM_SETMAXTIPWIDTH, 0, MAX_TOOLTIP_WIDTH );
								SendMessage( m_hTip, TTM_SETDELAYTIME, TTDT_INITIAL, 0 );
								
								m_bSelectPerson = false;
								m_bSelectBag = false;
								m_pUserInventoryDlg->Hide();
								m_pBagInventoryDlg->Hide();
								m_pHorseInventoryDlg->Show();
							}
							else if ( strcmp( tc.pszText, pText2 ) == 0 )
							{
								char strInventoryImagePath[ _MAX_PATH ];
								char strDirectoryPath[ _MAX_PATH ];
								ListView_SetExtendedListViewStyle( m_hMoveToHorse, LVS_EX_FULLROWSELECT | LVS_EX_ONECLICKACTIVATE | LVS_EX_UNDERLINEHOT | LVS_EX_INFOTIP );
								memset( strInventoryImagePath, 0, _MAX_PATH );
								strcpy ( strInventoryImagePath, "file://");			
								memset( strDirectoryPath, 0, _MAX_PATH );
								GetCurrentDirectory( _MAX_PATH, strDirectoryPath );
								SI32 DirectoryPathSize = strlen ( strDirectoryPath );
								if ( strDirectoryPath[ DirectoryPathSize - 1 ] != '\\' )
								{
									strcat( strDirectoryPath, "\\" );
								}
								strcat( strInventoryImagePath, strDirectoryPath );
								strcat( strInventoryImagePath, "Interface\\Inventory\\Horse.bmp");
								lv.ulFlags = LVBKIF_SOURCE_URL | LVBKIF_STYLE_NORMAL;			
								lv.pszImage = strInventoryImagePath;
								::SendMessage( m_hMoveToHorse, LVM_SETBKIMAGE, 0, (LPARAM)&lv);
								SendMessage( m_hMoveToHorse, LVM_SETTEXTBKCOLOR, 0, 0xffffffff );
								
								ti.cbSize=sizeof(TOOLINFO);
								ti.uFlags=TTF_SUBCLASS | TTF_IDISHWND;
								ti.hwnd=hDlg;
								ti.uId=(WPARAM)m_hMoveToHorse;
								pText2 = GetTxtFromMgr(1160);
								
								ti.lpszText= pText2;
								SendMessage( m_hTip,TTM_ADDTOOL,0,(LPARAM)(LPTOOLINFO)&ti );
								SendMessage( m_hTip, TTM_SETMAXTIPWIDTH, 0, MAX_TOOLTIP_WIDTH );
								SendMessage( m_hTip, TTM_SETDELAYTIME, TTDT_INITIAL, 0 );
								
								ListView_SetExtendedListViewStyle( m_hMoveToBag, LVS_EX_FULLROWSELECT | LVS_EX_ONECLICKACTIVATE | LVS_EX_UNDERLINEHOT | LVS_EX_INFOTIP );
								memset( strInventoryImagePath, 0, _MAX_PATH );
								strcpy ( strInventoryImagePath, "file://");			
								memset( strDirectoryPath, 0, _MAX_PATH );
								GetCurrentDirectory( _MAX_PATH, strDirectoryPath );
								DirectoryPathSize = strlen ( strDirectoryPath );
								if ( strDirectoryPath[ DirectoryPathSize - 1 ] != '\\' )
								{
									strcat( strDirectoryPath, "\\" );
								}
								strcat( strInventoryImagePath, strDirectoryPath );
								strcat( strInventoryImagePath, "Interface\\Inventory\\Hero.bmp");
								lv.ulFlags = LVBKIF_SOURCE_URL | LVBKIF_STYLE_NORMAL;			
								lv.pszImage = strInventoryImagePath;
								::SendMessage( m_hMoveToBag, LVM_SETBKIMAGE, 0, (LPARAM)&lv);
								SendMessage( m_hMoveToBag, LVM_SETTEXTBKCOLOR, 0, 0xffffffff );
								
								ti.cbSize=sizeof(TOOLINFO);
								ti.uFlags=TTF_SUBCLASS | TTF_IDISHWND;
								ti.hwnd=hDlg;
								ti.uId=(WPARAM)m_hMoveToBag;
								pText2 = GetTxtFromMgr(1162);
								
								ti.lpszText= pText2;
								SendMessage( m_hTip,TTM_ADDTOOL,0,(LPARAM)(LPTOOLINFO)&ti );
								SendMessage( m_hTip, TTM_SETMAXTIPWIDTH, 0, MAX_TOOLTIP_WIDTH );
								SendMessage( m_hTip, TTM_SETDELAYTIME, TTDT_INITIAL, 0 );
								
								
								m_bSelectPerson = false;
								m_bSelectBag = false;
								m_pUserInventoryDlg->Hide();
								m_pBagInventoryDlg->Show();
								m_pHorseInventoryDlg->Hide();
							}
						}
					}
				}
				return TRUE;
			case IDC_LIST_HELMET:
				{
					bCheck = true;
					siKind = PERSONITEM_HELMET;
				}
				break;
			case IDC_LIST_WEAPON1:
				{
					bCheck = true;
					siKind = PERSONITEM_WEAPON1;
				}
				break;
			case IDC_LIST_WEAPON2:
				{
					
					bCheck = true;
					siKind = PERSONITEM_WEAPON2;
				}
				break;
			case IDC_LIST_WAISTBAND:
				{
					bCheck = true;
					siKind = PERSONITEM_BELT;
				}
				break;
			case IDC_LIST_ARMOR:
				{
					bCheck = true;
					siKind = PERSONITEM_ARMOUR;
				}
				break;
			case IDC_LIST_SHOE:
				{
					bCheck = true;
					siKind = PERSONITEM_SHOES;
				}
				break;
			case IDC_LIST_NECKWEAR:
				{
					bCheck = true;
					siKind = PERSONITEM_NECK;
				}
				break;
			case IDC_LIST_RING1:
				{
					bCheck = true;
					siKind = PERSONITEM_RING1;
				}
				break;
			case IDC_LIST_RING2:
				{
					bCheck = true;
					siKind = PERSONITEM_RING2;
				}
				break;
			case IDC_LIST_MANTLE:
				{
					bCheck = true;
					siKind = PERSONITEM_MANTLE;
				}
				break;
			case IDC_LIST_BAG:
				{
					bCheck = true;
					siKind = PERSONITEM_BAG;
				}
				break;
			case IDC_LIST_HAT:
				{
					bCheck = true;
					siKind = PERSONITEM_HAT;
				}
				break;
			case IDC_LIST_DRESS:
				{
					bCheck = true;
					siKind = PERSONITEM_DRESS;
				}
				break;
			}
			
			
			if ( bCheck )
			{
				if ( m_siCurrentSummonIndex != -1 )
				{
					siKind += PERSONITEM_SUMMON1EQUIP0 + MAX_SHAPE_ITEM * m_siCurrentSummonIndex;
				}
				
				switch( hdr->code )
				{
				case NM_CLICK:
					{
						if ( nia->iItem >= 0 )
						{	
							m_siSelectedInventoryItemIndex = siKind;
							InventorySelect( siKind, hDlg );
							return TRUE;
						}
						
						if ( m_siSelectedInventoryItemIndex > -1 )
						{
							MouseReleaseOperation();
							return TRUE;
						}
					}
					break;
					
				case NM_DBLCLK:
					{
					/*
					// 아이템이 자동으로 장착되게 해야 하는디...
					// 이게 헬멧인지 갑옷인지 내가 모르니까니... 
					// 답답할 따름이오...
					if ( nia->iItem >= 0 )
					{
					SI32 siListViewIndex = 0;
					
					  for ( int i = PERSONITEM_INV0; i < MAX_ITEM_PER_PERSON; ++i )
					  {
					  if ( m_clItem[ i ].siUnique != 0 )
					  {
					  if ( nia->iItem == siListViewIndex )
					  {
					  m_siSelectedInventoryItemIndex = i;
					  MountItem( m_siSelectedInventoryItemIndex );
					  return TRUE;
					  }
					  else
					  {
					  ++siListViewIndex;
					  }
					  }
					  }
					  }
						*/						
					}
					break;
					
				case NM_RCLICK:
					{
						if ( nia->iItem >= 0 )
						{
							RepairItem( siKind );
							return TRUE;
						}
					}
					break;
				case LVN_GETINFOTIP:
					{
						/*
						if ( ngt->iItem >= 0 )
						{
							pclClient->pclItemManager->GetExplain( &m_clItem[ siKind ], ngt->pszText );
						}
						*/
					}
					break;
				}
			}
		}
		break;
		
	case WM_CLOSE:
		{
			Hide();
		}
		break;
		
	case WM_COMMAND:
		{
			switch( LOWORD(wParam) )
				//switch( wParam )
			{
			case IDOK:
			case IDCANCEL:
				{
					Hide();
				}
				return TRUE;
			case IDC_BUTTON_TAB:
				{
					ExchangeWeapon();
				}
				break;
				
				
			case IDC_BUTTON_MAKE_ITEM:
				{
					if( m_siMakeItemComboBoxSelectedIndex < 0 ) 
					{
						return FALSE;
					}
					
					if ( m_pMakeableItemList == NULL )
					{
						return FALSE;
					}

					//-------------------- 아이템 생성에 관련된 코드
					BOOL bSuccess;

					SI32 siMakeItemNum = GetDlgItemInt( hDlg, IDC_EDIT_MAKEITME_NUM, &bSuccess, FALSE );
					
				
					 //pclClient->pclItemManager->pclItemInfo[ref]->clProductionInfo.siProductUnit

					if ( bSuccess == FALSE || siMakeItemNum > m_pMakeableItemList[ m_siMakeItemComboBoxSelectedIndex * 2 + 1 ]  )
					{
						char* pTitle = GetTxtFromMgr(1163);
						char* pText = GetTxtFromMgr(1164);
						

						MessageBox( hDlg, pTitle, pText, MB_OK|MB_TOPMOST );
						break;
					}

					/*
					if ( siMakeItemNum % pclClient->pclItemManager->pclItemInfo[ref]->clProductionInfo.siProductUnit != 0 )
					{
						char Buffer[256];
						sprintf( Buffer, "이 아이템의 최소 생산 단위는 %d개 입니다.", pclClient->pclItemManager->pclItemInfo[ref]->clProductionInfo.siProductUnit );
						MessageBox( hDlg, Buffer, "수량 확인", MB_OK|MB_TOPMOST );
					}
					*/

					m_pBuffer->Push( INV_MAKE_ITEM, m_pMakeableItemList[ m_siMakeItemComboBoxSelectedIndex  * 2 ], siMakeItemNum );

					//pclclient->m_pMiniGameDlg->CreateMiniGame();
					//pclclient->m_pMiniGameDlg->Show();

				}
				break;
			case IDC_COMBO_MAKEITEM_LIST:
				{
					switch( HIWORD( wParam ) )
					{
					case CBN_SELCHANGE:
						{
							m_siMakeItemComboBoxSelectedIndex = SendMessage( GetDlgItem( hDlg, IDC_COMBO_MAKEITEM_LIST ), CB_GETCURSEL, 0, 0 );

							SI32 ref = pclClient->pclItemManager->FindItemRefFromUnique( m_pMakeableItemList[ m_siMakeItemComboBoxSelectedIndex * 2 ] );

							SI32 MinProductUnit = pclClient->pclItemManager->pclItemInfo[ref]->clProductionInfo.siProductUnit;
							char buffer[256] = "";
							sprintf( buffer, "%d", MinProductUnit );

							SetWindowText( GetDlgItem( hDlg, IDC_EDIT_MAKEITME_NUM ), buffer );
						}
						break;
					}
				}
				break;
			}
		}
		break;
	
	case WM_MOUSEMOVE:
		{
			if ( m_siSelectedInventoryItemIndex > -1 )
			{
				POINT pt;
				GetCursorPos( &pt );
				ImageList_DragMove( pt.x, pt.y );

				return TRUE;
			}
		}
		break;

	case WM_LBUTTONDOWN:
		{
			if ( m_siSelectedInventoryItemIndex > -1 )
			{
				//m_siUpgradeInventoryItemIndex = siKind;
				MouseReleaseOperation();
			}
		}
		break;

	case WM_DESTROY:
		{
			KillTimer( m_hDlg, 0 );

			if ( m_hTip )
			{
				DestroyWindow( m_hTip );
				m_hTip = NULL;
			}

			if ( m_hTabImageBitmap[ 0 ] )
			{
				DeleteObject( m_hTabImageBitmap[ 0 ] );
				m_hTabImageBitmap[ 0 ] = NULL ;
			}

			if ( m_hTabImageBitmap[ 1 ] )
			{
				DeleteObject( m_hTabImageBitmap[ 1 ] );
				m_hTabImageBitmap[ 1 ] = NULL ;
			}

			Hide();
			
						
			if ( m_hItemImage )
			{
				DeleteObject( m_hItemImage  );
				m_hItemImage = NULL;
			}

			m_hTabCtrl = NULL;

			m_hDlg = NULL;
		}
		break;

	}
	
		
	return FALSE;
}

// 여기에 들어가는 siSelectedInventoryItemIndex 의 값은 실제 아이템의 index번호이다.
// 리스트뷰의 index 는 절대 아니다.
void CItemInventory::InventorySelect( SI32 siSelectedInventoryItemIndex, HWND hDlg )
{
	cltClient *pclclient = (cltClient *)pclClient;
	// siSelectedInventoryItemIndex 는 리스트뷰의 index이지 실제 아이템의 index 는 아니다.
	// 따라서 실제 아이템의 index 를 가지는 변수도 있어야 한다.

	if ( m_clItem[ siSelectedInventoryItemIndex ].siUnique == 0 )
	{
		return;
	}

	if ( siSelectedInventoryItemIndex == PERSONITEM_BAG )
	{
		for ( SI16 t = PERSONITEM_BAG0; t < PERSONITEM_BAG0 + MAX_ITEM_PER_INVENTORY; ++t )
		{
			if ( pclClient->pclCM->CR[1]->pclCI->clCharItem.clItem[ t ].siUnique > 0 )
			{
				char* pTitle = GetTxtFromMgr(1165);
				char* pText = GetTxtFromMgr(1166);
				

                pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText );
				return;
			}
		}
	}

	/*
	if ( m_clItem[ siSelectedInventoryItemIndex ].CanRepair( pclClient->pclItemManager ) )
	{
		m_pBuffer->Push( INV_REPAIR_ITEM, siSelectedInventoryItemIndex, &m_clItem[ siSelectedInventoryItemIndex ] );
		return;
	}
	*/
	
	pclClient->pclItemManager->PlaySound( m_clItem[ m_siSelectedInventoryItemIndex ].siUnique, ITEMSOUND_SELECT );

	m_siSelectedInventoryItemIndex = siSelectedInventoryItemIndex;	
	
	
	POINT pt;
	GetCursorPos( &pt );

	ImageList_BeginDrag( pclclient->pclImageList->hImageList, m_clItem[ m_siSelectedInventoryItemIndex ].GetImage( pclClient->pclItemManager ), ITEM_IMAGE_WIDTH / 2, ITEM_IMAGE_WIDTH / 2 );
	ImageList_DragEnter( NULL, pt.x, pt.y );
	SetCapture( hDlg );
	
	return;
}

void CItemInventory::RepairItem( SI32 siSelectedInventoryItemIndex )
{
	if ( m_clItem[ siSelectedInventoryItemIndex ].CanRepair( pclClient->pclItemManager ) )
	{
		char* pTitle = GetTxtFromMgr(1141);
		char* pText = GetTxtFromMgr(1140);
		

		if (  MessageBox(NULL, pText, pTitle, MB_ICONQUESTION | MB_YESNO) == IDYES ) 
		{
			m_pBuffer->Push( INV_REPAIR_ITEM, siSelectedInventoryItemIndex, &m_clItem[ siSelectedInventoryItemIndex ] );
		}
		
		return;
	}
}

void CItemInventory::MoveItem( HWND hDlg, SI32 siDestination )
{
	if ( m_pPersonItemInfo->clItem[ siDestination ].siUnique != 0  )
	{
		m_siSelectedInventoryItemIndex = -1;
		return;
	}

	SI32 siMoveInfo = MAKELONG( m_siSelectedInventoryItemIndex, siDestination );

	pclClient->pclItemManager->PlaySound(m_clItem[ m_siSelectedInventoryItemIndex ].siUnique, ITEMSOUND_SELECT );
	
	m_pBuffer->Push( (SI16)INV_MOVE_ITEM, siMoveInfo, &m_clItem[ m_siSelectedInventoryItemIndex ] );

	return;
}

void CItemInventory::UseItem( SI16 si16Pos )
{
	if ( m_clItem[ si16Pos ].siUnique == 0 )
	{
		return;
	}


	//DialogBox( m_hInst, MAKEINTRESOURCE( IDD_DIALOG_MSG_INPUT ), m_hDlg, StaticNotifyMsgInputDlgProc );


	m_pBuffer->Push( INV_USE_ITEM, si16Pos, &m_clItem[ si16Pos ] );

	m_siSelectedInventoryItemIndex = -1;
	
	return;
}

void CItemInventory::DropItem( SI16 si16Pos )
{
	if ( m_clItem[ si16Pos ].siUnique == 0 )
	{
		return;
	}

	m_pBuffer->Push( INV_DROP_ITEM, si16Pos, &m_clItem[ si16Pos ] );

	m_siSelectedInventoryItemIndex = -1;
}

void CItemInventory::MountItem( SI16 si16Pos )
{
	if ( m_clItem[ si16Pos ].siUnique == 0 )
	{
		return;
	}

	m_pBuffer->Push( INV_MOUNT_ITEM, si16Pos, &m_clItem[ si16Pos ] );

	m_siSelectedInventoryItemIndex = -1;

	return;
}

// WEAPON1 과 WEAPON2의 위치를 교환한다.
void CItemInventory::ExchangeWeapon()
{
	if ( m_clItem[ PERSONITEM_WEAPON1 ].siUnique == 0 && 
		 m_clItem[ PERSONITEM_WEAPON2 ].siUnique == 0 )
	{
		return;
	}

	cltMsg clMsg(GAMEMSG_REQUEST_EXCHANGEWEAPON, 0, NULL);
	pclClient->SendMsgToServer((char*)&clMsg);

	m_siSelectedInventoryItemIndex = -1;

	return;
}

void CItemInventory::MouseReleaseOperation()
{
	if ( m_siSelectedInventoryItemIndex > -1 )
	{
		if ( m_siUpgradeInventoryItemIndex != -1 )
		{
			MessageBox( NULL, "set", "ESt", MB_OK|MB_TOPMOST );
		}

		MoveInventoryItem( m_hDlg );

		ImageList_DragLeave( NULL );
		ImageList_EndDrag();
		ReleaseCapture();

		m_siSelectedInventoryItemIndex = -1;
		m_siUpgradeInventoryItemIndex = -1;
	}

	return;
}

void CItemInventory::MoveInventoryItem( HWND hDlg )
{
	cltClient *pclclient = (cltClient *)pclClient;
	POINT pt;
	GetCursorPos( &pt );

	HWND hFindWnd = WindowFromPoint( pt );

	SI16 StartPos = 0;
	if ( m_siCurrentSummonIndex != -1 )
	{
		StartPos = PERSONITEM_SUMMON1EQUIP0 + MAX_SHAPE_ITEM * m_siCurrentSummonIndex;
	}

	if ( hFindWnd == GetDlgItem( hDlg, IDC_LIST_HELMET ) )
	{
		if ( m_clItem[ PERSONITEM_HELMET + StartPos ].siUnique != 0 && m_siSelectedInventoryItemIndex != PERSONITEM_HELMET + StartPos )
		{
			m_siUpgradeInventoryItemIndex = PERSONITEM_HELMET + StartPos;
			m_pBuffer->Push( INV_ADD_ITEM, m_siSelectedInventoryItemIndex, m_siUpgradeInventoryItemIndex, m_clItem[ m_siSelectedInventoryItemIndex ].siUnique , &m_clItem[ m_siUpgradeInventoryItemIndex ] );
		}
		else
		{
			MoveItem( hDlg, PERSONITEM_HELMET + StartPos );
		}
	}
	else if ( hFindWnd == GetDlgItem( hDlg, IDC_LIST_ARMOR ) )
	{
		if ( m_clItem[ PERSONITEM_ARMOUR + StartPos ].siUnique != 0 && m_siSelectedInventoryItemIndex != PERSONITEM_ARMOUR + StartPos )
		{
			m_siUpgradeInventoryItemIndex = PERSONITEM_ARMOUR + StartPos;
			m_pBuffer->Push( INV_ADD_ITEM, m_siSelectedInventoryItemIndex, m_siUpgradeInventoryItemIndex, m_clItem[ m_siSelectedInventoryItemIndex ].siUnique , &m_clItem[ m_siUpgradeInventoryItemIndex ] );
		}
		else
		{
			MoveItem( hDlg, PERSONITEM_ARMOUR + StartPos );
		}
	}
	else if ( hFindWnd == GetDlgItem( hDlg, IDC_LIST_WEAPON1 ) )
	{
		if ( m_clItem[ PERSONITEM_WEAPON1 + StartPos ].siUnique != 0 && m_siSelectedInventoryItemIndex != PERSONITEM_WEAPON1 + StartPos )
		{
			m_siUpgradeInventoryItemIndex = PERSONITEM_WEAPON1 + StartPos;
			m_pBuffer->Push( INV_ADD_ITEM, m_siSelectedInventoryItemIndex, m_siUpgradeInventoryItemIndex, m_clItem[ m_siSelectedInventoryItemIndex ].siUnique , &m_clItem[ m_siUpgradeInventoryItemIndex ] );
		}
		else
		{
			MoveItem( hDlg, PERSONITEM_WEAPON1 + StartPos );
		}
	}
	else if ( hFindWnd == GetDlgItem( hDlg, IDC_LIST_WEAPON2 ) )
	{
		// JAYCEE WEAPON2
		if ( m_clItem[ PERSONITEM_WEAPON2 + StartPos ].siUnique != 0 && m_siSelectedInventoryItemIndex != PERSONITEM_WEAPON2 + StartPos )
		{
			m_siUpgradeInventoryItemIndex = PERSONITEM_WEAPON2 + StartPos;
			m_pBuffer->Push( INV_ADD_ITEM, m_siSelectedInventoryItemIndex, m_siUpgradeInventoryItemIndex, m_clItem[ m_siSelectedInventoryItemIndex ].siUnique , &m_clItem[ m_siUpgradeInventoryItemIndex ] );
		}
		else
		{
			MoveItem( hDlg, PERSONITEM_WEAPON2 + StartPos );
		}
		
	}
	else if ( hFindWnd == GetDlgItem( hDlg, IDC_LIST_NECKWEAR ) )
	{
		if ( m_clItem[ PERSONITEM_NECK + StartPos ].siUnique != 0 && m_siSelectedInventoryItemIndex != PERSONITEM_NECK + StartPos )
		{
			m_siUpgradeInventoryItemIndex = PERSONITEM_NECK + StartPos;
			m_pBuffer->Push( INV_ADD_ITEM, m_siSelectedInventoryItemIndex, m_siUpgradeInventoryItemIndex, m_clItem[ m_siSelectedInventoryItemIndex ].siUnique , &m_clItem[ m_siUpgradeInventoryItemIndex ] );
		}
		else
		{
			MoveItem( hDlg, PERSONITEM_NECK + StartPos );
		}
	}
	else if ( hFindWnd == GetDlgItem( hDlg, IDC_LIST_RING1 ) )
	{
		if ( m_clItem[ PERSONITEM_RING1 + StartPos ].siUnique != 0 && m_siSelectedInventoryItemIndex != PERSONITEM_RING1 + StartPos )
		{
			m_siUpgradeInventoryItemIndex = PERSONITEM_RING1 + StartPos;
			m_pBuffer->Push( INV_ADD_ITEM, m_siSelectedInventoryItemIndex, m_siUpgradeInventoryItemIndex, m_clItem[ m_siSelectedInventoryItemIndex ].siUnique , &m_clItem[ m_siUpgradeInventoryItemIndex ] );
		}
		else
		{
			MoveItem( hDlg, PERSONITEM_RING1 + StartPos );
		}
	}
	else if ( hFindWnd == GetDlgItem( hDlg, IDC_LIST_RING2 ) )
	{
		if ( m_clItem[ PERSONITEM_RING2 + StartPos ].siUnique != 0 && m_siSelectedInventoryItemIndex != PERSONITEM_RING2 + StartPos )
		{
			m_siUpgradeInventoryItemIndex = PERSONITEM_RING2 + StartPos;
			m_pBuffer->Push( INV_ADD_ITEM, m_siSelectedInventoryItemIndex, m_siUpgradeInventoryItemIndex, m_clItem[ m_siSelectedInventoryItemIndex ].siUnique , &m_clItem[ m_siUpgradeInventoryItemIndex ] );
		}
		else
		{
			MoveItem( hDlg, PERSONITEM_RING2 + StartPos );
		}
	}
	else if ( hFindWnd == GetDlgItem( hDlg, IDC_LIST_WAISTBAND ) )
	{
		if ( m_clItem[ PERSONITEM_BELT + StartPos ].siUnique != 0 && m_siSelectedInventoryItemIndex != PERSONITEM_BELT + StartPos )
		{
			m_siUpgradeInventoryItemIndex = PERSONITEM_BELT + StartPos;
			m_pBuffer->Push( INV_ADD_ITEM, m_siSelectedInventoryItemIndex, m_siUpgradeInventoryItemIndex, m_clItem[ m_siSelectedInventoryItemIndex ].siUnique , &m_clItem[ m_siUpgradeInventoryItemIndex ] );
		}
		else
		{
			MoveItem( hDlg, PERSONITEM_BELT + StartPos );
		}
	}
	else if ( hFindWnd == GetDlgItem( hDlg, IDC_LIST_SHOE ) )
	{
		if ( m_clItem[ PERSONITEM_SHOES + StartPos ].siUnique != 0 && m_siSelectedInventoryItemIndex != PERSONITEM_SHOES + StartPos )
		{
			m_siUpgradeInventoryItemIndex = PERSONITEM_SHOES + StartPos;
			m_pBuffer->Push( INV_ADD_ITEM, m_siSelectedInventoryItemIndex, m_siUpgradeInventoryItemIndex, m_clItem[ m_siSelectedInventoryItemIndex ].siUnique , &m_clItem[ m_siUpgradeInventoryItemIndex ] );
		}
		else
		{
			MoveItem( hDlg, PERSONITEM_SHOES + StartPos );
		}
	}	
	else if ( hFindWnd == GetDlgItem( hDlg, IDC_LIST_MANTLE ) )
	{
		if ( m_clItem[ PERSONITEM_MANTLE + StartPos ].siUnique != 0 && m_siSelectedInventoryItemIndex != PERSONITEM_MANTLE + StartPos )
		{
			m_siUpgradeInventoryItemIndex = PERSONITEM_MANTLE + StartPos;
			m_pBuffer->Push( INV_ADD_ITEM, m_siSelectedInventoryItemIndex, m_siUpgradeInventoryItemIndex, m_clItem[ m_siSelectedInventoryItemIndex ].siUnique , &m_clItem[ m_siUpgradeInventoryItemIndex ] );
		}
		else
		{
			MoveItem( hDlg, PERSONITEM_MANTLE + StartPos );
		}
	}
	else if ( hFindWnd == GetDlgItem( hDlg, IDC_LIST_BAG ) )
	{
		if ( m_clItem[ PERSONITEM_BAG + StartPos ].siUnique != 0 && m_siSelectedInventoryItemIndex != PERSONITEM_BAG + StartPos )
		{
			m_siUpgradeInventoryItemIndex = PERSONITEM_BAG + StartPos;
			m_pBuffer->Push( INV_ADD_ITEM, m_siSelectedInventoryItemIndex, m_siUpgradeInventoryItemIndex, m_clItem[ m_siSelectedInventoryItemIndex ].siUnique , &m_clItem[ m_siUpgradeInventoryItemIndex ] );
		}
		else
		{
			MoveItem( hDlg, PERSONITEM_BAG + StartPos );
		}
	}
	else if ( hFindWnd == GetDlgItem( hDlg, IDC_LIST_HAT ) )
	{
		if ( m_clItem[ PERSONITEM_HAT + StartPos ].siUnique != 0 && m_siSelectedInventoryItemIndex != PERSONITEM_HAT + StartPos )
		{
			m_siUpgradeInventoryItemIndex = PERSONITEM_HAT + StartPos;
			m_pBuffer->Push( INV_ADD_ITEM, m_siSelectedInventoryItemIndex, m_siUpgradeInventoryItemIndex, m_clItem[ m_siSelectedInventoryItemIndex ].siUnique , &m_clItem[ m_siUpgradeInventoryItemIndex ] );
		}
		else
		{
			MoveItem( hDlg, PERSONITEM_HAT + StartPos );
		}
	}
	else if ( hFindWnd == GetDlgItem( hDlg, IDC_LIST_DRESS ) )
	{
		if ( m_clItem[ PERSONITEM_DRESS + StartPos ].siUnique != 0 && m_siSelectedInventoryItemIndex != PERSONITEM_DRESS + StartPos )
		{
			m_siUpgradeInventoryItemIndex = PERSONITEM_DRESS + StartPos;
			m_pBuffer->Push( INV_ADD_ITEM, m_siSelectedInventoryItemIndex, m_siUpgradeInventoryItemIndex, m_clItem[ m_siSelectedInventoryItemIndex ].siUnique , &m_clItem[ m_siUpgradeInventoryItemIndex ] );
		}
		else
		{
			MoveItem( hDlg, PERSONITEM_DRESS + StartPos );
		}
	}
	else if ( hFindWnd == GetDlgItem( m_pUserInventoryDlg->GetHwnd(), IDC_LIST_USER_INVENTORY ) )
	{
		for ( int i = PERSONITEM_INV0; i < PERSONITEM_INV0 + MAX_ITEM_PER_INVENTORY; ++i )
		{
			if ( m_pPersonItemInfo->clItem[i].siUnique == 0 )
			{
				MoveItem( hDlg, i );
				break;
			}
		}
	}
	else if ( hFindWnd == GetDlgItem( m_pHorseInventoryDlg->GetHwnd(), IDC_LIST_HORSE_INVENTORY ) )
	{		
		for ( int i = PERSONITEM_HORSE0; i < PERSONITEM_HORSE0 + MAX_ITEM_PER_INVENTORY; ++i )
		{
			if ( m_pPersonItemInfo->clItem[i].siUnique == 0 )
			{				
				MoveItem( hDlg, i );
				break;
			}
		}
	}
	else if ( hFindWnd == GetDlgItem( m_pBagInventoryDlg->GetHwnd(), IDC_LIST_BAG_INVENTORY ) )
	{
		if ( m_siSelectedInventoryItemIndex == PERSONITEM_BAG )
		{
			char* pTitle = GetTxtFromMgr(1167);
			char* pText = GetTxtFromMgr(1168);
			

			pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText );
			m_siSelectedInventoryItemIndex = -1;
			return;
		}

		for ( int i = PERSONITEM_BAG0; i < PERSONITEM_BAG0 + MAX_ITEM_PER_INVENTORY; ++i )
		{
			if ( m_pPersonItemInfo->clItem[i].siUnique == 0 )
			{
				MoveItem( hDlg, i );
				break;
			}
		}
	}
	else if ( hFindWnd == pclClient->GetHwnd() )
	{
		if ( m_siSelectedInventoryItemIndex >= PERSONITEM_INV0 )		// 리스트뷰에 있는 것만 바닥에 버릴 수 있으므로...
		{
			DropItem( m_siSelectedInventoryItemIndex );
			return;			
		}
	}
	
	return;
}


BOOL CALLBACK CItemInventory::StaticItemInventoryDlgProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam )
{
	cltClient *pclclient = ( cltClient *)pclClient;
	return pclclient->m_pItemInventoryDlg->ItemInventoryDlgProc( hDlg, iMsg, wParam, lParam );
}

BOOL CALLBACK CItemInventory::StaticNotifyMsgInputDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	cltClient *pclclient = ( cltClient *)pclClient;
	return pclclient->m_pItemInventoryDlg->NotifyMsgInputDlgProc( hDlg, iMsg, wParam, lParam );
}

BOOL CALLBACK CItemInventory::NotifyMsgInputDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	switch ( iMsg )
	{
	case WM_INITDIALOG:
		{
			SetWindowText( GetDlgItem( hDlg, IDD_DIALOG_MSG_INPUT ),  "" );
		}
		break;

	case WM_COMMAND:
		{
			char Msg[ 256 ] = "";
			switch ( LOWORD( wParam ) )
			{
			case IDOK:
			case IDCANCEL:
				GetDlgItemText( hDlg, IDD_DIALOG_MSG_INPUT, Msg, 256);
				EndDialog( hDlg, 0 );
				break;
			}
		}
		break;		
	}

	return FALSE;
}

bool CItemInventory::IsChanged()
{
	for ( int j = 0; j < MAX_ITEM_PER_PERSON; ++j )
	{
		SI32 size = sizeof(cltItem);

		char* src = (char*)&m_clItem[ j ];
		char* dst = (char*)&m_pPersonItemInfo->clItem[ j ];

		for(int i = 0;i < size; i++)
		{
			if(src[i] != dst[i])
			{
				return true;
			}
		}
	}

	return false;
}

bool CItemInventory::IsSummonInfoChanged()
{
	/*
	
	SI32 size = sizeof(cltSummonInfo);
	
	char* src = (char*)m_pSummonInfo;
	char* dst = (char*)&pclClient->pclCM->CR[1]->pclCI->clSummonInfo;
	
	for(int i = 0;i < size; i++)
	{
		if(src[i] != dst[i])
		{
			return true;
		}
	}
	*/

	for ( SI16 i = 0; i < MAX_SUMMON_OWN; ++i )
	{
		if ( m_pSummonInfo->clSummon[ i ].siKind != pclClient->pclCM->CR[1]->pclCI->clSummonInfo.clSummon[ i ].siKind )
		{
			return true;
		}
	}

	return false;
}

void CItemInventory::Show()
{
	ShowTabDlg();

	m_bShowFlag = true;
	
	LoadSummonInfo( m_hDlg );
	
	// 소환수 Tab인 경우.
	if ( m_siCurrentSummonIndex != -1 )
	{	
		DrawWeaponBackGroundImage( PERSONITEM_WEAPON1 );
	}
	else
	{
		DrawWeaponBackGroundImage( pclClient->pclCM->CR[1]->ActiveWeapon );
	}
	
	LoadDataAndSetting( m_hDlg );

	/*
	if ( IsSummonInfoChanged() )
	{
		LoadSummonInfo( m_hDlg );
	}
	else if ( IsChanged() )
	{
		LoadDataAndSetting( m_hDlg );
	}
	*/

	
	
	
	/*
	if ( m_bSelectPerson )
	{
		m_pUserInventoryDlg->Show();
	}
	else
	{
		m_pHorseInventoryDlg->Show();
	}
	*/



	/*
	m_siSelectedWeapon = pclClient->pclCM->CR[1]->ActiveWeapon;
	DrawWeaponBackGroundImage();
	*/


	ShowWindow ( m_hDlg, SW_SHOWNORMAL );

	SetTimer( m_hDlg, 0, INVENTORY_SETTIMER_VALUE, NULL );

	return;
}

void CItemInventory::Hide()
{
	m_bShowFlag = false;
	KillTimer( m_hDlg, 0 );

	m_pHorseInventoryDlg->Hide();
	m_pUserInventoryDlg->Hide();
	m_pBagInventoryDlg->Hide();
	ShowWindow ( m_hDlg, SW_HIDE );	

	return;
}

bool CItemInventory::IsShow()
{
	return m_bShowFlag;
}

bool CItemInventory::Pop( stMoveData * pMoveData )
{
	return m_pBuffer->Pop( pMoveData );	
}

bool CItemInventory::Pop( char* pMoveData )
{
	return m_pBuffer->Pop( pMoveData );
}

void CItemInventory::DrawWeaponBackGroundImage( SI16 SelectedWeapon )
{	
	char strSelectedWeaponImagePath[ _MAX_PATH ];
	char strWeaponImagePath[ _MAX_PATH ];
	char strDirectoryPath[ _MAX_PATH ];
	memset( strSelectedWeaponImagePath, 0, _MAX_PATH );
	memset( strWeaponImagePath, 0, _MAX_PATH );
	strcpy ( strWeaponImagePath, "file://");
	memset( strDirectoryPath, 0, _MAX_PATH );
	GetCurrentDirectory( _MAX_PATH, strDirectoryPath );
	SI32 DirectoryPathSize = strlen ( strDirectoryPath );
	if ( strDirectoryPath[ DirectoryPathSize - 1 ] != '\\' )
	{
		strcat( strDirectoryPath, "\\" );
	}
	strcat( strWeaponImagePath, strDirectoryPath );
	strcat( strSelectedWeaponImagePath, strDirectoryPath );	

	strcat( strWeaponImagePath, "Interface\\Inventory\\Weapon.bmp");
	strcat( strSelectedWeaponImagePath, "Interface\\Inventory\\SelectedWeapon.bmp");

	lv.ulFlags = LVBKIF_SOURCE_URL | LVBKIF_STYLE_NORMAL;			

	HWND TabImageBtn = GetDlgItem( m_hDlg, IDC_BUTTON_TAB );

	switch( SelectedWeapon )
	{
	case PERSONITEM_WEAPON1:
		lv.pszImage = strSelectedWeaponImagePath;
		SendMessage( m_hWeapon1, LVM_SETBKIMAGE, 0, (LPARAM)&lv);
		SendMessage( m_hWeapon1, LVM_SETTEXTBKCOLOR, 0, 0xffffffff );
		
		lv.pszImage = strWeaponImagePath;
		SendMessage( m_hWeapon2, LVM_SETBKIMAGE, 0, (LPARAM)&lv);
		SendMessage( m_hWeapon2, LVM_SETTEXTBKCOLOR, 0, 0xffffffff );

		/*
		lv.pszImage = strTab0ImagePath;
		SendMessage( TabImageCtrl, LVM_SETBKIMAGE, 0, (LPARAM)&lv);
		SendMessage( TabImageCtrl, LVM_SETTEXTBKCOLOR, 0, 0xffffffff );
		*/
		SendMessage( TabImageBtn, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)m_hTabImageBitmap[0] );

		break;
	case PERSONITEM_WEAPON2:
		lv.pszImage = strWeaponImagePath;
		SendMessage( m_hWeapon1, LVM_SETBKIMAGE, 0, (LPARAM)&lv);
		SendMessage( m_hWeapon1, LVM_SETTEXTBKCOLOR, 0, 0xffffffff );
		
		lv.pszImage = strSelectedWeaponImagePath;
		SendMessage( m_hWeapon2, LVM_SETBKIMAGE, 0, (LPARAM)&lv);
		SendMessage( m_hWeapon2, LVM_SETTEXTBKCOLOR, 0, 0xffffffff );

		/*
		lv.pszImage = strTab1ImagePath;
		SendMessage( TabImageCtrl, LVM_SETBKIMAGE, 0, (LPARAM)&lv);
		SendMessage( TabImageCtrl, LVM_SETTEXTBKCOLOR, 0, 0xffffffff );
		*/

		SendMessage( TabImageBtn, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)m_hTabImageBitmap[1] );

		break;
	default:
		lv.pszImage = strWeaponImagePath;
		SendMessage( m_hWeapon1, LVM_SETBKIMAGE, 0, (LPARAM)&lv);
		SendMessage( m_hWeapon1, LVM_SETTEXTBKCOLOR, 0, 0xffffffff );

		SendMessage( m_hWeapon2, LVM_SETBKIMAGE, 0, (LPARAM)&lv);
		SendMessage( m_hWeapon2, LVM_SETTEXTBKCOLOR, 0, 0xffffffff );

		/*
		lv.pszImage = strTab0ImagePath;
		SendMessage( TabImageCtrl, LVM_SETBKIMAGE, 0, (LPARAM)&lv);
		SendMessage( TabImageCtrl, LVM_SETTEXTBKCOLOR, 0, 0xffffffff );
		*/
		
		SendMessage( TabImageBtn, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)m_hTabImageBitmap[0] );
		
		break;
	}
}

void CItemInventory::ShowTabDlg()
{
	m_bHorseHave = pclClient->pclCM->CR[1]->HaveHorse();
	
	if ( pclClient->pclCM->CR[1]->pclCI->clCharItem.clItem[ PERSONITEM_BAG ].siUnique > 0 )
	{
		m_bBagHave = true;
	}
	else
	{
		m_bBagHave = false;
	}
		
	TabCtrl_DeleteAllItems( m_hTabCtrl );

	ShowWindow( GetDlgItem( m_hDlg, IDC_LIST_MOVEITEM_BAG ), SW_HIDE );
	ShowWindow( GetDlgItem( m_hDlg, IDC_LIST_MOVEITEM_HORSE ), SW_HIDE );
	
	char* pText = GetTxtFromMgr(1159);
	

	InsertTabDialog( m_hTabCtrl, 0, pText );

	if ( pclClient->pclCM->CR[1]->HaveHorse() )
	{

		ShowWindow( GetDlgItem( m_hDlg, IDC_LIST_MOVEITEM_HORSE ), SW_SHOW );
		pText = GetTxtFromMgr(372);
		
		InsertTabDialog( m_hTabCtrl, 1, pText );
	}
	
	if ( m_bBagHave )
	{
		ShowWindow( GetDlgItem( m_hDlg, IDC_LIST_MOVEITEM_BAG ), SW_SHOW );
		pText = GetTxtFromMgr(1033);
		
		InsertTabDialog( m_hTabCtrl, 2, pText );
	}	
	
	AdjustTabRect();
}

void CItemInventory::ShowSummonTabDlg()
{
	TabCtrl_DeleteAllItems( m_hTabCtrlHeroSelect );

	m_siCurrentSummonIndex = -1;
	InsertTabDialog( m_hTabCtrlHeroSelect, 0, (char*)(pclClient->pclCM->CR[1]->GetName() ) );

	SI16 Count = 0;

	for ( SI16 i = 0; i < MAX_SUMMON_OWN; ++i )
	{
		if ( m_pSummonInfo->clSummon[ i ].siKind > 0 )
		{
			if ( m_pSummonInfo->clSummon[ i ].szName != NULL )
			{
				InsertTabDialog( m_hTabCtrlHeroSelect, ++Count, m_pSummonInfo->clSummon[ i ].szName );
			}
		}
		else
		{
			char* pText = GetTxtFromMgr(848);
			

			InsertTabDialog( m_hTabCtrlHeroSelect, ++Count, pText );
		}
	}

	char strInventoryImagePath[ _MAX_PATH ];
	char strDirectoryPath[ _MAX_PATH ];
	SI32 DirectoryPathSize = 0;
	TOOLINFO ti;
	
	ListView_SetExtendedListViewStyle( m_hBag, LVS_EX_FULLROWSELECT | LVS_EX_ONECLICKACTIVATE | LVS_EX_UNDERLINEHOT | LVS_EX_INFOTIP );
	memset( strInventoryImagePath, 0, _MAX_PATH );
	strcpy ( strInventoryImagePath, "file://");
	memset( strDirectoryPath, 0, _MAX_PATH );
	GetCurrentDirectory( _MAX_PATH, strDirectoryPath );
	DirectoryPathSize = strlen ( strDirectoryPath );
	if ( strDirectoryPath[ DirectoryPathSize - 1 ] != '\\' )
	{
		strcat( strDirectoryPath, "\\" );
	}
	strcat( strInventoryImagePath, strDirectoryPath );
	strcat( strInventoryImagePath, "Interface\\Inventory\\Bag.bmp" );
	lv.ulFlags = LVBKIF_SOURCE_URL | LVBKIF_STYLE_NORMAL;
	lv.pszImage = strInventoryImagePath;
	::SendMessage( m_hBag, LVM_SETBKIMAGE, 0, (LPARAM)&lv);
	SendMessage( m_hBag, LVM_SETTEXTBKCOLOR, 0, 0xffffffff );
		
	ti.cbSize=sizeof(TOOLINFO);
	ti.uFlags=TTF_SUBCLASS | TTF_IDISHWND;
	ti.hwnd=m_hDlg;
	ti.uId=(WPARAM)m_hBag;
	char* pText = GetTxtFromMgr(1155);
	

	ti.lpszText=pText;
	SendMessage( m_hTip,TTM_ADDTOOL,0,(LPARAM)(LPTOOLINFO)&ti );
	SendMessage( m_hTip, TTM_SETMAXTIPWIDTH, 0, MAX_TOOLTIP_WIDTH );
	SendMessage( m_hTip, TTM_SETDELAYTIME, TTDT_INITIAL, 0 );
	
	
	ListView_SetExtendedListViewStyle( m_hWeapon2, LVS_EX_FULLROWSELECT | LVS_EX_ONECLICKACTIVATE | LVS_EX_UNDERLINEHOT | LVS_EX_INFOTIP );
	memset( strInventoryImagePath, 0, _MAX_PATH );
	strcpy ( strInventoryImagePath, "file://");			
	memset( strDirectoryPath, 0, _MAX_PATH );
	GetCurrentDirectory( _MAX_PATH, strDirectoryPath );
	DirectoryPathSize = strlen ( strDirectoryPath );
	if ( strDirectoryPath[ DirectoryPathSize - 1 ] != '\\' )
	{
		strcat( strDirectoryPath, "\\" );
	}
	DirectoryPathSize = strlen ( strDirectoryPath );
	if ( strDirectoryPath[ DirectoryPathSize - 1 ] != '\\' )
	{
		strcat( strDirectoryPath, "\\" );
	}
	strcat( strInventoryImagePath, strDirectoryPath );
	strcat( strInventoryImagePath, "Interface\\Inventory\\Weapon.bmp");
	lv.ulFlags = LVBKIF_SOURCE_URL | LVBKIF_STYLE_NORMAL;			
	lv.pszImage = strInventoryImagePath;
	::SendMessage(m_hWeapon2, LVM_SETBKIMAGE, 0, (LPARAM)&lv);
	SendMessage( m_hWeapon2, LVM_SETTEXTBKCOLOR, 0, 0xffffffff );
		
	ti.cbSize=sizeof(TOOLINFO);
	ti.uFlags=TTF_SUBCLASS | TTF_IDISHWND;
	ti.hwnd=m_hDlg;
	ti.uId=(WPARAM)m_hWeapon2;
	pText = GetTxtFromMgr(1148);
	

	ti.lpszText=pText;
	SendMessage( m_hTip,TTM_ADDTOOL,0,(LPARAM)(LPTOOLINFO)&ti );
	SendMessage( m_hTip, TTM_SETMAXTIPWIDTH, 0, MAX_TOOLTIP_WIDTH );
	SendMessage( m_hTip, TTM_SETDELAYTIME, TTDT_INITIAL, 0 );
	
	
	// 가방은 사람일때만 보여야 됨.
	// m_hBag
	// ShowWindow( m_hBag, SW_SHOW );
}

void CItemInventory::InsertTabDialog( HWND hTab, SI32 Index, char *TabDlgName )
{
	TC_ITEM item;
	item.mask= TCIF_TEXT;
	item.iImage = -1;

	item.lParam = Index;
	item.pszText = TabDlgName;

	TabCtrl_InsertItem( hTab, Index, &item);
	
	return;
}

void CItemInventory::AdjustTabRect()
{
	TOOLINFO ti;
	
	RECT rect;
	GetClientRect( m_hTabCtrl, &rect);
	TabCtrl_AdjustRect( m_hTabCtrl, FALSE, &rect);
	
	MoveWindow( m_pUserInventoryDlg->m_hDlg, 
		rect.left, rect.top, rect.right-rect. left,
		rect.bottom- rect.top, TRUE);
	
	MoveWindow( m_pHorseInventoryDlg->m_hDlg,
		rect.left, rect.top, rect.right-rect. left,
		rect.bottom- rect.top, TRUE);

	MoveWindow( m_pBagInventoryDlg->m_hDlg,
		rect.left, rect.top, rect.right-rect. left,
		rect.bottom- rect.top, TRUE);
	
	char strInventoryImagePath[ _MAX_PATH ];
	char strDirectoryPath[ _MAX_PATH ];
	ListView_SetExtendedListViewStyle( m_hMoveToHorse, LVS_EX_FULLROWSELECT | LVS_EX_ONECLICKACTIVATE | LVS_EX_UNDERLINEHOT | LVS_EX_INFOTIP );
	memset( strInventoryImagePath, 0, _MAX_PATH );
	strcpy ( strInventoryImagePath, "file://");			
	memset( strDirectoryPath, 0, _MAX_PATH );
	GetCurrentDirectory( _MAX_PATH, strDirectoryPath );
	SI32 DirectoryPathSize = strlen ( strDirectoryPath );
	if ( strDirectoryPath[ DirectoryPathSize - 1 ] != '\\' )
	{
		strcat( strDirectoryPath, "\\" );
	}
	strcat( strInventoryImagePath, strDirectoryPath );
	strcat( strInventoryImagePath, "Interface\\Inventory\\Horse.bmp");
	lv.ulFlags = LVBKIF_SOURCE_URL | LVBKIF_STYLE_NORMAL;			
	lv.pszImage = strInventoryImagePath;
	::SendMessage( m_hMoveToHorse, LVM_SETBKIMAGE, 0, (LPARAM)&lv);
	SendMessage( m_hMoveToHorse, LVM_SETTEXTBKCOLOR, 0, 0xffffffff );

	ti.cbSize=sizeof(TOOLINFO);
	ti.uFlags=TTF_SUBCLASS | TTF_IDISHWND;
	ti.hwnd=m_hDlg;
	ti.uId=(WPARAM)m_hMoveToHorse;
	char* pText = GetTxtFromMgr(1160);
	

	ti.lpszText=pText;
	SendMessage( m_hTip,TTM_ADDTOOL,0,(LPARAM)(LPTOOLINFO)&ti );
	SendMessage( m_hTip, TTM_SETMAXTIPWIDTH, 0, MAX_TOOLTIP_WIDTH );
	SendMessage( m_hTip, TTM_SETDELAYTIME, TTDT_INITIAL, 0 );
	

	
	ListView_SetExtendedListViewStyle( m_hMoveToBag, LVS_EX_FULLROWSELECT | LVS_EX_ONECLICKACTIVATE | LVS_EX_UNDERLINEHOT | LVS_EX_INFOTIP );
	memset( strInventoryImagePath, 0, _MAX_PATH );
	strcpy ( strInventoryImagePath, "file://");			
	memset( strDirectoryPath, 0, _MAX_PATH );
	GetCurrentDirectory( _MAX_PATH, strDirectoryPath );
	DirectoryPathSize = strlen ( strDirectoryPath );
	if ( strDirectoryPath[ DirectoryPathSize - 1 ] != '\\' )
	{
		strcat( strDirectoryPath, "\\" );
	}
	strcat( strInventoryImagePath, strDirectoryPath );
	strcat( strInventoryImagePath, "Interface\\Inventory\\MoveBag.bmp");
	lv.ulFlags = LVBKIF_SOURCE_URL | LVBKIF_STYLE_NORMAL;			
	lv.pszImage = strInventoryImagePath;
	::SendMessage( m_hMoveToBag, LVM_SETBKIMAGE, 0, (LPARAM)&lv);
	SendMessage( m_hMoveToBag, LVM_SETTEXTBKCOLOR, 0, 0xffffffff );

	ti.cbSize=sizeof(TOOLINFO);
	ti.uFlags=TTF_SUBCLASS | TTF_IDISHWND;
	ti.hwnd=m_hDlg;
	ti.uId=(WPARAM)m_hMoveToBag;
	pText = GetTxtFromMgr(1161);
	

	ti.lpszText=pText;
	SendMessage( m_hTip,TTM_ADDTOOL,0,(LPARAM)(LPTOOLINFO)&ti );
	SendMessage( m_hTip, TTM_SETMAXTIPWIDTH, 0, MAX_TOOLTIP_WIDTH );
	SendMessage( m_hTip, TTM_SETDELAYTIME, TTDT_INITIAL, 0 );
	
	m_bSelectPerson = true;
	m_bSelectBag = false;

	m_pUserInventoryDlg->Show();
	m_pHorseInventoryDlg->Hide();
	m_pBagInventoryDlg->Hide();

	//ShowWindow( m_pUserInventoryDlg->m_hDlg, SW_SHOW);
	//ShowWindow( m_pHorseInventoryDlg->m_hDlg, SW_HIDE);
}