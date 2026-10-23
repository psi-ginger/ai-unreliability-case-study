#include "HorseInventoryDlg.h"
#include "ItemInventory.h"

#include "ItemInventoryBuffer.h"
#include "..\..\CommonLogic\CommonLogic.h"

//-----------------------------
// Common
//-----------------------------
#include "PersonInfo.h"
#include "Char\CharManager\CharManager.h"


#include "..\Client\Client.h"
extern cltCommonLogic* pclClient;

CHorseInventoryDlg::CHorseInventoryDlg()
{
	m_siSelectedIndex = -1;
	m_siUpgradeSelectedIndex = -1;
	m_bShow = false;
	m_hDlg = NULL;
}

CHorseInventoryDlg::~CHorseInventoryDlg()
{
	/*
	if( m_hDlg )
	{
		DestroyWindow( m_hDlg );
		m_hDlg = NULL;
	}
	*/
}

void CHorseInventoryDlg::Create( HWND hWnd, HINSTANCE hInst, cltPItemInfo *pPersonItemInfo, CItemInventoryBuffer *pBuffer, HWND hParent )
{
	m_pBuffer = pBuffer;
	m_pPersonItemInfo = pPersonItemInfo;
	m_hParentDlg = hParent;

	m_hDlg = CreateDialog( hInst, MAKEINTRESOURCE(IDD_DIALOG_HORSE_INVENTORY ), hWnd, StaticHorseInventoryDlgProc );	

	Hide();

	return;
}

void CHorseInventoryDlg::Initialize()
{
	return;
}

void CHorseInventoryDlg::Destroy()
{
	return;
}

BOOL CALLBACK CHorseInventoryDlg::StaticHorseInventoryDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	cltClient *pclclient = ( cltClient *)pclClient;
	return pclclient->m_pItemInventoryDlg->m_pHorseInventoryDlg->HorseInventoryDlgProc( hDlg, iMsg, wParam, lParam );
}

LVBKIMAGE lv2;		// 이 변수 계속 살아 있어야 되넹... 죽으면 리스트뷰 배경도 가라지뿌고... ㅡㅡ
BOOL CALLBACK CHorseInventoryDlg::HorseInventoryDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	switch( iMsg )
	{
	case WM_INITDIALOG:
		{
			m_hDlg = hDlg;

			m_hHorseInventory = GetDlgItem( hDlg, IDC_LIST_HORSE_INVENTORY );
			
			
			g_FontManager.SetFont( hDlg, IDC_LIST_HORSE_INVENTORY, 12 );
			ListView_SetExtendedListViewStyle( m_hHorseInventory, LVS_EX_FULLROWSELECT | LVS_EX_ONECLICKACTIVATE | LVS_EX_INFOTIP );
			
			char strInventoryImagePath[ _MAX_PATH ];
			char strDirectoryPath[ _MAX_PATH ];

			memset( strInventoryImagePath, 0, _MAX_PATH );
			strcpy ( strInventoryImagePath, "file://");			
			memset( strDirectoryPath, 0, _MAX_PATH );
			GetCurrentDirectory( _MAX_PATH, strDirectoryPath );			
			strcat( strInventoryImagePath, strDirectoryPath );
			strcat( strInventoryImagePath, "\\Interface\\Inventory\\Inventory.bmp");
			lv2.ulFlags = LVBKIF_SOURCE_URL | LVBKIF_STYLE_NORMAL;			
			lv2.pszImage = strInventoryImagePath;
			::SendMessage(m_hHorseInventory, LVM_SETBKIMAGE, 0, (LPARAM)&lv2);
			SendMessage( m_hHorseInventory, LVM_SETTEXTBKCOLOR, 0, 0xffffffff );
			
			cltClient *pclclient = (cltClient *)pclClient;
			pclclient->pclImageList->ConnectToListView( GetDlgItem( hDlg, IDC_LIST_HORSE_INVENTORY ), LVSIL_NORMAL );		
		}
		return TRUE;

	case WM_TIMER:				// 지속적으로 내 인벤토리 아이템들이 바뀌었는지 확인해야 하므로...
		{
			if ( IsInventoryChanged() )
			{
				LoadInventoryData();
			}
		}
		return TRUE;

	case WM_MOUSEMOVE:
		{
			if ( m_siSelectedIndex > -1 )
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
			if ( m_siSelectedIndex > -1 )
			{
				MouseReleaseOperation();
			}
		}
		break;

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
						

			if ( hdr->hwndFrom == GetDlgItem( hDlg, IDC_LIST_HORSE_INVENTORY ) )
			{
				switch( hdr->code )
				{
				case NM_CLICK:
					{
						if ( nia->iItem >= 0 )
						{
							m_siSelectedIndex = nia->iItem + PERSONITEM_HORSE0;
							InventorySelect( m_siSelectedIndex, hDlg );
						}
					}
					break;
				case NM_RCLICK:
					{
						if ( nia->iItem >= 0 )
						{
							m_siSelectedIndex = nia->iItem + PERSONITEM_HORSE0;

							if ( m_clItem[ m_siSelectedIndex ].CanRepair(pclClient->pclItemManager ) )
							{
								RepairItem( m_siSelectedIndex );
							}
							else
							{
								UseItem( m_siSelectedIndex );
							}
						}						
					}
					break;
				case LVN_GETINFOTIP:
					{
						if ( ngt->iItem >= 0 )
						{
							pclClient->pclItemManager->GetExplain( &m_clItem[ ngt->iItem + PERSONITEM_HORSE0 ], ngt->pszText );
						}
					}
					break;
				case NM_RELEASEDCAPTURE:
					{
						if ( m_siSelectedIndex > -1 || m_siUpgradeSelectedIndex > -1 )
						{
							ImageList_DragLeave( NULL );
							ImageList_EndDrag();
							ReleaseCapture();
						}
					}
					break;
				}				
			}
		}
		break;

	case WM_DESTROY:
		{
			KillTimer( m_hDlg, 0 );
			
			/*

			if ( m_hDlg )
			{
				DestroyWindow( m_hDlg );
				m_hDlg = NULL;
			}
			*/
		}
		break;
	}


	return FALSE;
}

bool CHorseInventoryDlg::IsShow()
{
	return m_bShow;
}

void CHorseInventoryDlg::Show()
{
	if ( m_bShow )
	{
		return;
	}

	m_bShow = true;
	SetTimer( m_hDlg, 0, INVENTORY_SETTIMER_VALUE, NULL );

	LoadInventoryData();
	
	ShowWindow( m_hDlg, SW_SHOWNORMAL );

	return;
}

void CHorseInventoryDlg::Hide()
{
	if ( ! m_bShow )
	{
		return;
	}

	m_bShow = false;
	KillTimer( m_hDlg, 0 );

	ShowWindow( m_hDlg, SW_HIDE );

	return;
}

void CHorseInventoryDlg::LoadInventoryData()
{	
	if ( m_siSelectedIndex > -1 || m_siUpgradeSelectedIndex > -1 )
	{
		ImageList_DragLeave( NULL );
		ImageList_EndDrag();
		ReleaseCapture();
	}

	m_siSelectedIndex = -1;
	m_siUpgradeSelectedIndex = -1;

	for ( SI32 i = PERSONITEM_INV0; i < MAX_ITEM_PER_PERSON; ++i )		// 장착창은 체크할 필요 없으므로, 10 부터 함.
	{
		m_clItem[ i ] = m_pPersonItemInfo->clItem[ i ];
	}

	SetInventoryInListView( pclClient->pclItemManager, m_hDlg, IDC_LIST_HORSE_INVENTORY, NULL, m_pPersonItemInfo, PERSONITEM_HORSE0 );	

	return;
}

bool CHorseInventoryDlg::IsInventoryChanged()
{
	for ( int j = PERSONITEM_INV0; j < MAX_ITEM_PER_PERSON; ++j )
	{
		SI32 size = sizeof(cltItem);

		char* src = (char*)&m_clItem[ j ];
		char* dst = (char*)&m_pPersonItemInfo->clItem[ j];

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

void CHorseInventoryDlg::MouseReleaseOperation()
{
	if ( m_siSelectedIndex > -1 )
	{
		MoveInventoryItem( m_hDlg );

		ImageList_DragLeave( NULL );
		ImageList_EndDrag();
		ReleaseCapture();

		m_siSelectedIndex = -1;
		m_siUpgradeSelectedIndex = -1;
	}

	return;
}

void CHorseInventoryDlg::MoveItem( HWND hDlg, SI32 siDestination )
{
	/*
	if ( m_pPersonItemInfo->clItem[ siDestination ].siUnique != 0 )
	{
		m_siSelectedIndex = -1;
		return;
	}
	*/


	SI32 siMoveInfo = MAKELONG( m_siSelectedIndex, siDestination );

	if ( m_clItem[ siDestination ].siUnique != 0 )
	{
		if ( m_clItem[ siDestination ].CanPile( pclClient->pclItemManager ) == false )
		{
			return;
		}
	}

	pclClient->pclItemManager->PlaySound(m_clItem[ m_siSelectedIndex ].siUnique, ITEMSOUND_SELECT );
	
	m_pBuffer->Push( (SI16)INV_MOVE_ITEM, siMoveInfo, &m_clItem[ m_siSelectedIndex ] );

	return;
}

/*
void CHorseInventoryDlg::MoveInventoryItem( HWND hDlg )
{
	POINT pt;
	GetCursorPos( &pt );

	HWND hFindWnd = WindowFromPoint( pt );

	SI16 StartPos = 0;
	cltClient *pclclient = (cltClient *)pclClient;
	if ( pclclient->m_pItemInventoryDlg->GetCurrentSummonIndex() != -1 )
	{
		StartPos = PERSONITEM_SUMMON1EQUIP0 + MAX_SHAPE_ITEM * pclclient->m_pItemInventoryDlg->GetCurrentSummonIndex();
	}
	
	if ( hFindWnd == GetDlgItem( m_hParentDlg, IDC_LIST_HELMET ) )
	{
		if ( m_pPersonItemInfo->clItem[ PERSONITEM_HELMET + StartPos ].siUnique != 0 && m_siSelectedIndex != PERSONITEM_HELMET + StartPos )
		{
			m_siUpgradeSelectedIndex = PERSONITEM_HELMET + StartPos;
			//m_pBuffer->Push( INV_ADD_ITEM, m_siSelectedIndex, m_siUpgradeSelectedIndex, m_clItem[ m_siSelectedIndex ].siUnique , &m_clItem[ m_siUpgradeSelectedIndex ] );
			m_pBuffer->Push( INV_ADD_ITEM, m_siSelectedIndex, m_siUpgradeSelectedIndex, m_pPersonItemInfo->clItem[ m_siSelectedIndex ].siUnique , &m_pPersonItemInfo->clItem[ m_siUpgradeSelectedIndex ] );
		}
		else
		{
			MoveItem( m_hParentDlg, PERSONITEM_HELMET + StartPos );
		}
	}
	else if ( hFindWnd == GetDlgItem( m_hParentDlg, IDC_LIST_ARMOR ) )
	{
		if ( m_pPersonItemInfo->clItem[ PERSONITEM_ARMOUR + StartPos ].siUnique != 0 && m_siSelectedIndex != PERSONITEM_ARMOUR + StartPos )
		{
			m_siUpgradeSelectedIndex = PERSONITEM_ARMOUR + StartPos;
			m_pBuffer->Push( INV_ADD_ITEM, m_siSelectedIndex, m_siUpgradeSelectedIndex, m_pPersonItemInfo->clItem[ m_siSelectedIndex ].siUnique , &m_pPersonItemInfo->clItem[ m_siUpgradeSelectedIndex ] );
		}
		else
		{
			MoveItem( m_hParentDlg, PERSONITEM_ARMOUR + StartPos );
		}
	}
	else if ( hFindWnd == GetDlgItem( m_hParentDlg, IDC_LIST_WEAPON1 ) )
	{
		if ( m_pPersonItemInfo->clItem[ PERSONITEM_WEAPON1 + StartPos ].siUnique != 0 && m_siSelectedIndex != PERSONITEM_WEAPON1 + StartPos )
		{
			m_siUpgradeSelectedIndex = PERSONITEM_WEAPON1 + StartPos;
			m_pBuffer->Push( INV_ADD_ITEM, m_siSelectedIndex, m_siUpgradeSelectedIndex, m_pPersonItemInfo->clItem[ m_siSelectedIndex ].siUnique , &m_pPersonItemInfo->clItem[ m_siUpgradeSelectedIndex ] );
		}
		else
		{
			MoveItem( m_hParentDlg, PERSONITEM_WEAPON1 + StartPos );
		}
	}
	else if ( hFindWnd == GetDlgItem( m_hParentDlg, IDC_LIST_WEAPON2 ) )
	{
		// JAYCEE WEAPON2
		if ( m_pPersonItemInfo->clItem[ PERSONITEM_WEAPON2 + StartPos ].siUnique != 0 && m_siSelectedIndex != PERSONITEM_WEAPON2 + StartPos )
		{
			m_siUpgradeSelectedIndex = PERSONITEM_WEAPON2 + StartPos;
			m_pBuffer->Push( INV_ADD_ITEM, m_siSelectedIndex, m_siUpgradeSelectedIndex, m_pPersonItemInfo->clItem[ m_siSelectedIndex ].siUnique , &m_pPersonItemInfo->clItem[ m_siUpgradeSelectedIndex ] );
		}
		else
		{
			MoveItem( m_hParentDlg, PERSONITEM_WEAPON2 + StartPos );
		}
	}	
	else if ( hFindWnd == GetDlgItem( m_hParentDlg, IDC_LIST_NECKWEAR ) )
	{
		if ( m_pPersonItemInfo->clItem[ PERSONITEM_NECK + StartPos ].siUnique != 0 && m_siSelectedIndex != PERSONITEM_NECK + StartPos )
		{
			m_siUpgradeSelectedIndex = PERSONITEM_NECK + StartPos;
			m_pBuffer->Push( INV_ADD_ITEM, m_siSelectedIndex, m_siUpgradeSelectedIndex, m_pPersonItemInfo->clItem[ m_siSelectedIndex ].siUnique , &m_pPersonItemInfo->clItem[ m_siUpgradeSelectedIndex ] );
		}
		else
		{
			MoveItem( m_hParentDlg, PERSONITEM_NECK + StartPos );
		}
	}
	else if ( hFindWnd == GetDlgItem( m_hParentDlg, IDC_LIST_RING1 ) )
	{
		if ( m_pPersonItemInfo->clItem[ PERSONITEM_RING1 + StartPos ].siUnique != 0 && m_siSelectedIndex != PERSONITEM_RING1 + StartPos )
		{
			m_siUpgradeSelectedIndex = PERSONITEM_RING1 + StartPos;
			m_pBuffer->Push( INV_ADD_ITEM, m_siSelectedIndex, m_siUpgradeSelectedIndex, m_pPersonItemInfo->clItem[ m_siSelectedIndex ].siUnique , &m_pPersonItemInfo->clItem[ m_siUpgradeSelectedIndex ] );
		}
		else
		{
			MoveItem( m_hParentDlg, PERSONITEM_RING1 + StartPos );
		}
	}
	else if ( hFindWnd == GetDlgItem( m_hParentDlg, IDC_LIST_RING2 ) )
	{
		if ( m_pPersonItemInfo->clItem[ PERSONITEM_RING2 + StartPos ].siUnique != 0 && m_siSelectedIndex != PERSONITEM_RING2 + StartPos )
		{
			m_siUpgradeSelectedIndex = PERSONITEM_RING2 + StartPos;
			m_pBuffer->Push( INV_ADD_ITEM, m_siSelectedIndex, m_siUpgradeSelectedIndex, m_pPersonItemInfo->clItem[ m_siSelectedIndex ].siUnique , &m_pPersonItemInfo->clItem[ m_siUpgradeSelectedIndex ] );
		}
		else
		{
			MoveItem( m_hParentDlg, PERSONITEM_RING2 + StartPos );
		}
	}
	else if ( hFindWnd == GetDlgItem( m_hParentDlg, IDC_LIST_WAISTBAND ) )
	{
		if ( m_pPersonItemInfo->clItem[ PERSONITEM_BELT + StartPos ].siUnique != 0 && m_siSelectedIndex != PERSONITEM_BELT + StartPos )
		{
			m_siUpgradeSelectedIndex = PERSONITEM_BELT + StartPos;
			m_pBuffer->Push( INV_ADD_ITEM, m_siSelectedIndex, m_siUpgradeSelectedIndex, m_pPersonItemInfo->clItem[ m_siSelectedIndex ].siUnique , &m_pPersonItemInfo->clItem[ m_siUpgradeSelectedIndex ] );
		}
		else
		{
			MoveItem( m_hParentDlg, PERSONITEM_BELT + StartPos );
		}
	}
	else if ( hFindWnd == GetDlgItem( m_hParentDlg, IDC_LIST_SHOE ) )
	{
		if ( m_pPersonItemInfo->clItem[ PERSONITEM_SHOES + StartPos ].siUnique != 0 && m_siSelectedIndex != PERSONITEM_SHOES + StartPos )
		{
			m_siUpgradeSelectedIndex = PERSONITEM_SHOES + StartPos;
			m_pBuffer->Push( INV_ADD_ITEM, m_siSelectedIndex, m_siUpgradeSelectedIndex, m_pPersonItemInfo->clItem[ m_siSelectedIndex ].siUnique , &m_pPersonItemInfo->clItem[ m_siUpgradeSelectedIndex ] );
		}
		else
		{
			MoveItem( m_hParentDlg, PERSONITEM_SHOES + StartPos );
		}
	}
	else if ( hFindWnd == GetDlgItem( m_hParentDlg, IDC_LIST_MANTLE ) )
	{
		if ( m_pPersonItemInfo->clItem[ PERSONITEM_MANTLE + StartPos ].siUnique != 0 && m_siSelectedIndex != PERSONITEM_MANTLE + StartPos )
		{
			m_siUpgradeSelectedIndex = PERSONITEM_MANTLE + StartPos;
			m_pBuffer->Push( INV_ADD_ITEM, m_siSelectedIndex, m_siUpgradeSelectedIndex, m_pPersonItemInfo->clItem[ m_siSelectedIndex ].siUnique , &m_pPersonItemInfo->clItem[ m_siUpgradeSelectedIndex ] );
		}
		else
		{
			MoveItem( m_hParentDlg, PERSONITEM_MANTLE + StartPos );
		}
	}
	else if ( hFindWnd == GetDlgItem( m_hParentDlg, IDC_LIST_BAG ) )
	{
		if ( m_pPersonItemInfo->clItem[ PERSONITEM_BAG + StartPos ].siUnique != 0 && m_siSelectedIndex != PERSONITEM_BAG + StartPos )
		{
			m_siUpgradeSelectedIndex = PERSONITEM_BAG + StartPos;
			m_pBuffer->Push( INV_ADD_ITEM, m_siSelectedIndex, m_siUpgradeSelectedIndex, m_pPersonItemInfo->clItem[ m_siSelectedIndex ].siUnique , &m_pPersonItemInfo->clItem[ m_siUpgradeSelectedIndex ] );
		}
		else
		{
			MoveItem( m_hParentDlg, PERSONITEM_BAG + StartPos );
		}
	}
	else if ( hFindWnd == GetDlgItem( m_hParentDlg, IDC_LIST_HAT ) )
	{
		if ( m_pPersonItemInfo->clItem[ PERSONITEM_HAT + StartPos ].siUnique != 0 && m_siSelectedIndex != PERSONITEM_HAT + StartPos )
		{
			m_siUpgradeSelectedIndex = PERSONITEM_HAT + StartPos;
			m_pBuffer->Push( INV_ADD_ITEM, m_siSelectedIndex, m_siUpgradeSelectedIndex, m_pPersonItemInfo->clItem[ m_siSelectedIndex ].siUnique , &m_pPersonItemInfo->clItem[ m_siUpgradeSelectedIndex ] );
		}
		else
		{
			MoveItem( m_hParentDlg, PERSONITEM_HAT + StartPos );
		}
	}
	else if ( hFindWnd == GetDlgItem( m_hParentDlg, IDC_LIST_DRESS ) )
	{
		if ( m_pPersonItemInfo->clItem[ PERSONITEM_DRESS + StartPos ].siUnique != 0 && m_siSelectedIndex != PERSONITEM_DRESS + StartPos )
		{
			m_siUpgradeSelectedIndex = PERSONITEM_DRESS + StartPos;
			m_pBuffer->Push( INV_ADD_ITEM, m_siSelectedIndex, m_siUpgradeSelectedIndex, m_pPersonItemInfo->clItem[ m_siSelectedIndex ].siUnique , &m_pPersonItemInfo->clItem[ m_siUpgradeSelectedIndex ] );
		}
		else
		{
			MoveItem( m_hParentDlg, PERSONITEM_DRESS + StartPos );
		}
	}
	else if ( hFindWnd == GetDlgItem( m_hParentDlg, IDC_LIST_MOVEITEM_HORSE ) )
	{
		if ( pclClient->pclCM->CR[1]->HaveHorse() )
		{
			SI16 pos;
			cltItem cltoitem;
			SI32 rtnvalue;

			if(pclClient->pclCM->CR[1]->pclCI->clCharItem.CanAddInv(pclClient->pclItemManager, INVMODE_PERSON, 
				&m_clItem[ m_siSelectedIndex ],
				NULL, 
				&pos, 
				&cltoitem,
				&rtnvalue) == true)
			{
				MoveItem( m_hDlg, pos );
			}			
		}
	}
	else if ( hFindWnd == GetDlgItem( m_hParentDlg, IDC_LIST_MOVEITEM_BAG ) )
	{
		if ( pclClient->pclCM->CR[1]->pclCI->clCharItem.clItem[ PERSONITEM_BAG ].siUnique > 0  )
		{
			SI16 pos;
			cltItem cltoitem;
			SI32 rtnvalue;

			if(pclClient->pclCM->CR[1]->pclCI->clCharItem.CanAddInv(pclClient->pclItemManager, INVMODE_BAG, 
				&m_clItem[ m_siSelectedIndex ],
				NULL, 
				&pos, 
				&cltoitem,
				&rtnvalue) == true)
			{
				MoveItem( m_hDlg, pos );
			}
		}
	}
	else if ( hFindWnd == GetDlgItem( m_hDlg, IDC_LIST_HORSE_INVENTORY ) )
	{
		// 리스트뷰에서 리스트뷰로 옮길때는 성능 업그레이드할때만...
		if ( m_siSelectedIndex >= PERSONITEM_HORSE0 )
		{
			ScreenToClient( GetDlgItem( hDlg, IDC_LIST_HORSE_INVENTORY ), &pt );

			LVHITTESTINFO HitTestInfo;
			HitTestInfo.pt = pt;
			HitTestInfo.flags = LVHT_ONITEMICON | LVHT_NOWHERE;
			int temp =  ListView_HitTest( GetDlgItem( hDlg, IDC_LIST_HORSE_INVENTORY ), &HitTestInfo );

			if ( temp != -1 )
			{
				if ( temp + PERSONITEM_HORSE0 != m_siSelectedIndex )
				{
					m_siUpgradeSelectedIndex = temp + PERSONITEM_HORSE0;

					if ( m_clItem[ m_siSelectedIndex ].GetType( pclClient->pclItemManager ) == ITEMTYPE_SCROLL &&  m_pPersonItemInfo->clItem[ m_siUpgradeSelectedIndex ].siUnique != 0 )
					{
						m_pBuffer->Push( INV_ADD_ITEM, m_siSelectedIndex, m_siUpgradeSelectedIndex, m_pPersonItemInfo->clItem[ m_siSelectedIndex ].siUnique , &m_pPersonItemInfo->clItem[ m_siUpgradeSelectedIndex ] );
					}
					else
					{
						MoveItem( hDlg, m_siUpgradeSelectedIndex );
					}
				}
			}

			return;
		}
	}
	else if ( hFindWnd == pclClient->GetHwnd() )
	{
		if ( m_siSelectedIndex >= PERSONITEM_HORSE0 )		// 리스트뷰에 있는 것만 바닥에 버릴 수 있으므로...
		{
			DropItem( m_siSelectedIndex );
			return;			
		}
	}
	
	return;
}


*/

void CHorseInventoryDlg::MoveInventoryItem( HWND hDlg )
{
	POINT pt;
	GetCursorPos( &pt );

	HWND hFindWnd = WindowFromPoint( pt );

	SI16 StartPos = 0;
	cltClient *pclclient = (cltClient *)pclClient;
	if ( pclclient->m_pItemInventoryDlg->GetCurrentSummonIndex() != -1 )
	{
		StartPos = PERSONITEM_SUMMON1EQUIP0 + MAX_SHAPE_ITEM * pclclient->m_pItemInventoryDlg->GetCurrentSummonIndex();
	}


	if ( hFindWnd == GetDlgItem( m_hParentDlg, IDC_LIST_MOVEITEM_HORSE ) )
	{
		if ( pclClient->pclCM->CR[1]->HaveHorse() )
		{
			SI16 pos;
			cltItem cltoitem;
			SI32 rtnvalue;

			if(pclClient->pclCM->CR[1]->pclCI->clCharItem.CanAddInv(pclClient->pclItemManager, INVMODE_PERSON, 
				&m_clItem[ m_siSelectedIndex ],
				NULL, 
				&pos, 
				&cltoitem,
				&rtnvalue) == true)
			{
				MoveItem( m_hDlg, pos );
			}			
		}
	}
	else if ( hFindWnd == GetDlgItem( m_hParentDlg, IDC_LIST_MOVEITEM_BAG ) )
	{
		if ( pclClient->pclCM->CR[1]->pclCI->clCharItem.clItem[ PERSONITEM_BAG ].siUnique > 0  )
		{
			SI16 pos;
			cltItem cltoitem;
			SI32 rtnvalue;

			if(pclClient->pclCM->CR[1]->pclCI->clCharItem.CanAddInv(pclClient->pclItemManager, INVMODE_BAG, 
				&m_clItem[ m_siSelectedIndex ],
				NULL, 
				&pos, 
				&cltoitem,
				&rtnvalue) == true)
			{
				MoveItem( m_hDlg, pos );
			}
		}
	}
	else if ( hFindWnd == GetDlgItem( m_hDlg, IDC_LIST_HORSE_INVENTORY ) )
	{
		// 리스트뷰에서 리스트뷰로 옮길때는 성능 업그레이드할때만...
		if ( m_siSelectedIndex >= PERSONITEM_HORSE0 )
		{
			ScreenToClient( GetDlgItem( hDlg, IDC_LIST_HORSE_INVENTORY ), &pt );

			LVHITTESTINFO HitTestInfo;
			HitTestInfo.pt = pt;
			HitTestInfo.flags = LVHT_ONITEMICON | LVHT_NOWHERE;
			int temp =  ListView_HitTest( GetDlgItem( hDlg, IDC_LIST_HORSE_INVENTORY ), &HitTestInfo );

			if ( temp != -1 )
			{
				if ( temp + PERSONITEM_HORSE0 != m_siSelectedIndex )
				{
					m_siUpgradeSelectedIndex = temp + PERSONITEM_HORSE0;

					if ( m_clItem[ m_siSelectedIndex ].GetType( pclClient->pclItemManager ) == ITEMTYPE_SCROLL &&  m_pPersonItemInfo->clItem[ m_siUpgradeSelectedIndex ].siUnique != 0 )
					{
						m_pBuffer->Push( INV_ADD_ITEM, m_siSelectedIndex, m_siUpgradeSelectedIndex, m_pPersonItemInfo->clItem[ m_siSelectedIndex ].siUnique , &m_pPersonItemInfo->clItem[ m_siUpgradeSelectedIndex ] );
					}
					else
					{
						MoveItem( hDlg, m_siUpgradeSelectedIndex );
					}
				}
			}

			return;
		}
	}
	else if ( hFindWnd == pclClient->GetHwnd() )
	{
		if ( m_siSelectedIndex >= PERSONITEM_INV0 )		// 리스트뷰에 있는 것만 바닥에 버릴 수 있으므로...
		{
			DropItem( m_siSelectedIndex );
			return;
		}
	}

	//여기서 부터 작업 ..ㅠㅠ
	SI32 siCheckInven = CheckIsInventory();	

	if(siCheckInven==PERSONITEM_NONE)
	{
		return;
	}

	SI32 siItemUinque =m_pPersonItemInfo->clItem[ m_siSelectedIndex ].siUnique;
	SI32 ItemType= pclclient->pclItemManager->GetItemType(siItemUinque);
	SI32 siItemType = GetInventoryType(ItemType);
	
	if(siCheckInven==PERSONITEM_WEAPON2 || siCheckInven==PERSONITEM_WEAPON1)
	{
		if(ItemType >=ITEMTYPE_SWORD && ItemType <=ITEMTYPE_GUN)
		{
			siItemType = siCheckInven;
		}
	}	

	if(siCheckInven==PERSONITEM_RING1 || siCheckInven==PERSONITEM_RING2)
	{
		if(ItemType ==ITEMTYPE_RING)
		{
			siItemType = siCheckInven;
		}
	}	

	if(ItemType == ITEMTYPE_SCROLL)
	{
		siItemType = siCheckInven;
	}


	//SI32 siItemType = m_pPersonItemInfo->clItem[ CheckIsInventory() + StartPos ].GetType(pclItemManager);


	if(siItemType >= PERSONITEM_HELMET && siItemType<=PERSONITEM_DRESS)
	{
		if ( m_pPersonItemInfo->clItem[ siItemType + StartPos ].siUnique != 0 && m_siSelectedIndex != siItemType + StartPos && ItemType == ITEMTYPE_SCROLL)
		{
			m_siUpgradeSelectedIndex = siItemType + StartPos;
			m_pBuffer->Push( INV_ADD_ITEM, m_siSelectedIndex, m_siUpgradeSelectedIndex, m_pPersonItemInfo->clItem[ m_siSelectedIndex ].siUnique , &m_pPersonItemInfo->clItem[ m_siUpgradeSelectedIndex ] );
		}		
		else if ( m_pPersonItemInfo->clItem[ siItemType + StartPos ].siUnique != 0 && m_siSelectedIndex != siItemType + StartPos) 
		{
			ChangeItem( m_hParentDlg, siItemType + StartPos );
		}		
		else
		{			
			MoveItem( m_hParentDlg, siItemType + StartPos );
		}
	}
	


	return;
}
void CHorseInventoryDlg::RepairItem( SI32 siSelectedInventoryItemIndex )
{
	if ( m_clItem[ m_siSelectedIndex ].CanRepair( pclClient->pclItemManager ) )
	{
		char* pTitle = GetTxtFromMgr(1141);
		char* pText = GetTxtFromMgr(1140);
		
		if (  MessageBox(NULL,  pText,pTitle, MB_ICONQUESTION | MB_YESNO) == IDYES ) 
		{
			m_pBuffer->Push( INV_REPAIR_ITEM, m_siSelectedIndex, &m_clItem[ m_siSelectedIndex ] );
		}
		return;
	}

	return;
}

void CHorseInventoryDlg::InventorySelect( SI32 siSelectedInventoryItemIndex, HWND hDlg )
{
	// siSelectedInventoryItemIndex 는 리스트뷰의 index이지 실제 아이템의 index 는 아니다.
	// 따라서 실제 아이템의 index 를 가지는 변수도 있어야 한다.

	if ( m_clItem[ siSelectedInventoryItemIndex ].siUnique == 0 )
	{
		return;
	}
	
	pclClient->pclItemManager->PlaySound( m_clItem[ m_siSelectedIndex ].siUnique, ITEMSOUND_SELECT );

	m_siSelectedIndex = siSelectedInventoryItemIndex;	
	
	
	POINT pt;
	GetCursorPos( &pt );
	cltClient *pclclient = (cltClient *)pclClient;
	ImageList_BeginDrag( pclclient->pclImageList->hImageList, m_clItem[ m_siSelectedIndex ].GetImage( pclClient->pclItemManager ), ITEM_IMAGE_WIDTH / 2, ITEM_IMAGE_WIDTH / 2 );
	ImageList_DragEnter( NULL, pt.x, pt.y );
	SetCapture( hDlg );

	return;
}

void CHorseInventoryDlg::UseItem( SI16 si16Pos )
{
	if ( m_clItem[ si16Pos ].siUnique == 0 )
	{
		return;
	}

	if ( m_clItem[ si16Pos ].GetType( pclClient->pclItemManager ) == ITEMTYPE_BOOK )
	{
		char* pTitle = GetTxtFromMgr(1142);
		char* pText = GetTxtFromMgr(1143);
		
		if (  MessageBox(NULL, pTitle, pText, MB_ICONQUESTION | MB_YESNO) == IDYES )
		{
			m_pBuffer->Push( INV_USE_ITEM, si16Pos, &m_clItem[ si16Pos ] );
		}
	}
	else
	{
		m_pBuffer->Push( INV_USE_ITEM, si16Pos, &m_clItem[ si16Pos ] );
	}


	m_siSelectedIndex = -1;
	
	return;
}

void CHorseInventoryDlg::DropItem( SI16 si16Pos )
{
	char* pTitle = GetTxtFromMgr(5261);
	char* pText = GetTxtFromMgr(5262);

	if (  MessageBox(NULL,  pText,pTitle, MB_ICONQUESTION | MB_YESNO) == IDNO ) 
	{
		return;
	}

	if ( m_clItem[ si16Pos ].siUnique == 0 )
	{
		return;
	}

	m_pBuffer->Push( INV_DROP_ITEM, si16Pos, &m_clItem[ si16Pos ] );

	m_siSelectedIndex = -1;
}

void CHorseInventoryDlg::MountItem( SI16 si16Pos )
{
	if ( m_clItem[ si16Pos ].siUnique == 0 )
	{
		return;
	}

	m_pBuffer->Push( INV_MOUNT_ITEM, si16Pos, &m_clItem[ si16Pos ] );

	m_siSelectedIndex = -1;
}

HWND CHorseInventoryDlg::GetHwnd()
{
	return m_hDlg;
}

SI32 CHorseInventoryDlg::GetSelectedIndex()
{
	return m_siSelectedIndex;
}

cltItem	*CHorseInventoryDlg::GetItemIndex( SI32 siIndex )
{
	return &m_clItem[ siIndex ];
}

SI32 CHorseInventoryDlg::GetInventoryType(SI32 siItemType)
{
	SI32 StartPos =0;
	cltClient *pclclient = (cltClient *)pclClient;
	if ( pclclient->m_pItemInventoryDlg->GetCurrentSummonIndex() != -1 )
	{
		StartPos = PERSONITEM_SUMMON1EQUIP0 + MAX_SHAPE_ITEM * pclclient->m_pItemInventoryDlg->GetCurrentSummonIndex();
	}
	SI32 siItemUinque =m_pPersonItemInfo->clItem[ m_siSelectedIndex ].siUnique;

	if (siItemType == ITEMTYPE_HELMET )
		return PERSONITEM_HELMET;

	else if  (siItemType == ITEMTYPE_ARMOUR  )
		return PERSONITEM_ARMOUR;

	else if(( siItemType == ITEMTYPE_CANNON || siItemType == ITEMTYPE_GUN || siItemType == ITEMTYPE_BOW ||siItemType == ITEMTYPE_AXE ||siItemType == ITEMTYPE_STAFF ||siItemType == ITEMTYPE_SPEAR 
		||siItemType == ITEMTYPE_SWORD || siItemType == ITEMTYPE_FISHINGROD || siItemUinque == 10100 ||siItemUinque == 10090) && ( m_pPersonItemInfo->clItem[ PERSONITEM_WEAPON1 + StartPos ].siUnique != 0 && m_siSelectedIndex != PERSONITEM_WEAPON1 + StartPos ))
		return PERSONITEM_WEAPON2;

	else if ( siItemType == ITEMTYPE_CANNON || siItemType == ITEMTYPE_GUN || siItemType == ITEMTYPE_BOW ||siItemType == ITEMTYPE_AXE ||siItemType == ITEMTYPE_STAFF ||siItemType == ITEMTYPE_SPEAR 
		||siItemType == ITEMTYPE_SWORD || siItemType == ITEMTYPE_FISHINGROD || siItemUinque == 10100 ||siItemUinque == 10090)
		return PERSONITEM_WEAPON1;

	else if ( siItemType == ITEMTYPE_NECK )
		return PERSONITEM_NECK;

	else if (( siItemType == ITEMTYPE_RING )
		&&( m_pPersonItemInfo->clItem[ PERSONITEM_RING2 + StartPos ].siUnique != 0 && m_siSelectedIndex != PERSONITEM_RING2 + StartPos ))
		return PERSONITEM_RING1;

	else if ( siItemType == ITEMTYPE_RING )
		return PERSONITEM_RING2;

	else if ( siItemType == ITEMTYPE_BELT )
		return PERSONITEM_BELT;

	else if ( siItemType == ITEMTYPE_SHOES )
		return PERSONITEM_SHOES;

	else if ( siItemType ==ITEMTYPE_MANTLE )
		return PERSONITEM_MANTLE;

	else if ( siItemType == ITEMTYPE_BAG )
		return PERSONITEM_BAG;

	else if ( siItemType == ITEMTYPE_HAT )
		return PERSONITEM_HAT;

	else if ( siItemType == ITEMTYPE_DRESS )
		return PERSONITEM_DRESS;
	else
	{
		return PERSONITEM_NONE;
	}


}
SI32 CHorseInventoryDlg::CheckIsInventory()
{
	POINT pt;
	GetCursorPos( &pt );

	HWND hFindWnd = WindowFromPoint( pt );
	if ( hFindWnd == GetDlgItem( m_hParentDlg, IDC_LIST_HELMET ) )
		return PERSONITEM_HELMET;
	else if ( hFindWnd == GetDlgItem( m_hParentDlg, IDC_LIST_ARMOR ) )
		return PERSONITEM_ARMOUR;
	else if ( hFindWnd == GetDlgItem( m_hParentDlg, IDC_LIST_WEAPON1 ) )
		return PERSONITEM_WEAPON1;
	else if ( hFindWnd == GetDlgItem( m_hParentDlg, IDC_LIST_WEAPON2 ) )
		return PERSONITEM_WEAPON2;
	else if ( hFindWnd == GetDlgItem( m_hParentDlg, IDC_LIST_NECKWEAR ) )
		return PERSONITEM_NECK;
	else if ( hFindWnd == GetDlgItem( m_hParentDlg, IDC_LIST_RING1 ) )
		return PERSONITEM_RING1;
	else if ( hFindWnd == GetDlgItem( m_hParentDlg, IDC_LIST_RING2 ) )
		return PERSONITEM_RING2;
	else if ( hFindWnd == GetDlgItem( m_hParentDlg, IDC_LIST_WAISTBAND ) )
		return PERSONITEM_BELT;
	else if ( hFindWnd == GetDlgItem( m_hParentDlg, IDC_LIST_SHOE ) )
		return PERSONITEM_SHOES;
	else if ( hFindWnd == GetDlgItem( m_hParentDlg, IDC_LIST_MANTLE ) )
		return PERSONITEM_MANTLE;
	else if ( hFindWnd == GetDlgItem( m_hParentDlg, IDC_LIST_BAG ) )
		return PERSONITEM_BAG;
	else if ( hFindWnd == GetDlgItem( m_hParentDlg, IDC_LIST_HAT ) )
		return PERSONITEM_HAT;
	else if ( hFindWnd == GetDlgItem( m_hParentDlg, IDC_LIST_DRESS ) )
		return PERSONITEM_DRESS;
	else
	{
		return PERSONITEM_NONE;
	}
}

void CHorseInventoryDlg::ChangeItem( HWND hDlg, SI32 siDestination )
{
	SI32 siMoveInfo = MAKELONG( m_siSelectedIndex, siDestination );

	if ( m_clItem[ siDestination ].siUnique != 0 )
	{
		if ( m_clItem[ siDestination ].CanPile( pclClient->pclItemManager ) == false )
		{
			return;
		}
	}


	pclClient->pclItemManager->PlaySound(m_clItem[ m_siSelectedIndex ].siUnique, ITEMSOUND_SELECT );	
	m_pBuffer->Push( (SI16)INV_CHANGE_ITEM, siMoveInfo, &m_clItem[ m_siSelectedIndex ] );

	return;


}
