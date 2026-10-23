#include "ForeignMerchantBuyCashItemDlg.h"

#include "..\ErrorMsgDlg\ErrorMsgDlg.h"
#include "..\NotifyMsgDlg\NotifyMsgDlg.h"
//-----------------------------
// Common
//-----------------------------
#include "PersonInfo.h"
#include "Char\CharManager\CharManager.h"

#include "..\..\..\CommonLogic\CommonLogic.h"
#include "../../Client.h"
#include "../../Common/SystemNPC/ForeignMerchantNPC/ForeignMerchantNPCBuyCashItem.h"

#include "../../../CommonLogic/Msg/MsgType-NPC.h"

extern cltCommonLogic* pclClient;

CForeignMerchantBuyCashItemDlg::CForeignMerchantBuyCashItemDlg()
{
	m_hDlg = NULL;
	m_bShow = false;	
	m_hEditBrush = NULL;
	m_hPortraitBitmap = NULL;

	m_pclItem = new cltItem[ PERSONITEM_INV0 + MAX_INVITEM_PER_PERSON ];

	m_siItemUniqueList = new SI16[ MAX_AVAILABLE_CASHITEMNUM ];
	m_uiItemPriceList = new UI16[ MAX_AVAILABLE_CASHITEMNUM ];

	memset( m_siItemUniqueList, 0, sizeof( SI16 ) * MAX_AVAILABLE_CASHITEMNUM );
	memset( m_uiItemPriceList, 0, sizeof( UI16 ) * MAX_AVAILABLE_CASHITEMNUM );

	m_siSelectedIndex = -1;

	m_siListBoxSelectedIndex = 0;
	
	m_hItemBitmap = NULL;
	
	m_siListNum = 0;
}

CForeignMerchantBuyCashItemDlg::~CForeignMerchantBuyCashItemDlg()
{	
	if ( m_hPortraitBitmap )
	{
		DeleteObject( m_hPortraitBitmap );
		m_hPortraitBitmap = NULL;
	}

	if ( m_siItemUniqueList )
	{
		delete [] m_siItemUniqueList;
		m_siItemUniqueList = NULL;
	}

	if ( m_uiItemPriceList )
	{
		delete [] m_uiItemPriceList;
		m_uiItemPriceList = NULL;
	}

	if ( m_pclItem )
	{
		delete [] m_pclItem;
		m_pclItem = NULL;
	}

	if ( m_hDlg )
	{
		DestroyWindow( m_hDlg );
	}
}

void CForeignMerchantBuyCashItemDlg::Create( HINSTANCE hInst, HWND hWnd )
{
	m_hDlg = CreateDialog( hInst, MAKEINTRESOURCE(IDD_DIALOG_FOREIGNMERCHANTBUYCASHITEM ), hWnd, StaticForeignMerchantBuyCashItemDlgProc );

	Hide();

	return;
}

bool CForeignMerchantBuyCashItemDlg::IsUserInventoryChanged()
{
	for ( int j = PERSONITEM_INV0; j < PERSONITEM_INV0 + MAX_INVITEM_PER_PERSON; ++j )
	{
		SI32 size = sizeof(cltItem);

		char* src = (char*)&m_pclItem[ j ];
		char* dst = (char*)&pclClient->pclCM->CR[1]->pclCI->clCharItem.clItem[ j ];

		for( int i = 0; i < size; ++i )
		{
			if( src[i] != dst[i] )
			{
				return true;
			}
		}
	}

	return false;
}

void CForeignMerchantBuyCashItemDlg::LoadUserInventory()
{
	m_siSelectedIndex = -1;

	for ( int i = PERSONITEM_INV0; i < PERSONITEM_INV0 + MAX_INVITEM_PER_PERSON; ++i )
	{
		m_pclItem[ i ] = pclClient->pclCM->CR[1]->pclCI->clCharItem.clItem[ i ];
	}

	SetInventoryInListView( pclClient->pclItemManager, m_hDlg, IDC_LIST_USER_INVENTORY, NULL, &pclClient->pclCM->CR[1]->pclCI->clCharItem );

	return;
}

BOOL CALLBACK CForeignMerchantBuyCashItemDlg::StaticForeignMerchantBuyCashItemDlgProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam )
{
	cltClient *pclclient = ( cltClient * )pclClient;
	return pclclient->m_pForeignMerchantBuyCashItemDlg->ForeignMerchantBuyCashItemDlgProc( hDlg, iMsg, wParam, lParam );
}


BOOL CALLBACK CForeignMerchantBuyCashItemDlg::ForeignMerchantBuyCashItemDlgProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam )
{
	cltClient *pclclient = (cltClient *)pclClient;
	switch( iMsg )
	{
	case WM_INITDIALOG:
		{
			cltClient *pclclient = (cltClient *)pclClient;
			pclclient->pclImageList->ConnectToListView( GetDlgItem( hDlg, IDC_LIST_USER_INVENTORY ), LVSIL_NORMAL );

			char ItemTypeCodeList[ MAX_ITEMTYPE_NUMBER ][ 64 ];
			pclClient->pclForeignMerchantNPCBuyCashItem->GetItemTypeCodeList( (char*)&ItemTypeCodeList[0] );

			char* pText = GetTxtFromMgr(1860);
			SetWindowText( GetDlgItem( hDlg, IDC_EDIT_FOREIGN_MERCHANT_EXPLAIN ), pText);
				
			if( m_hPortraitBitmap == NULL )
			{
				m_hPortraitBitmap = (HBITMAP)LoadImage( pclClient->GetInstance(), "Interface/Portrait/KIND_CHINAMERCHANT.bmp", IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE );

				if ( m_hPortraitBitmap != NULL )
				{
					SendDlgItemMessage( hDlg, IDC_STATIC_FOREIGN_MERCHANT_IMAGE, STM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)m_hPortraitBitmap);
				}
			}

			for ( SI16 i = 0; i < MAX_ITEMTYPE_NUMBER; ++i )
			{
				if(  ItemTypeCodeList[ i ] && strlen( ItemTypeCodeList[ i ] ) > 0 )
				{
					SI32 ItemTypeUnique = pclClient->pclItemManager->FindItemTypeUniqueFromTypeCode( ItemTypeCodeList[ i ] );
					if ( ItemTypeUnique > 0 )
					{
						SendMessage( GetDlgItem( hDlg, IDC_COMBO_KIND_FOREIGNMERCHANT ), CB_ADDSTRING, 0, (LPARAM)pclClient->pclItemManager->pclItemTypeInfo[ ItemTypeUnique ]->szName );
					}
				}
			}

			LVCOLUMN Col;			
			
			Col.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
			Col.fmt = LVCFMT_CENTER;
			
			Col.cx = 100;
			pText = GetTxtFromMgr(1227);
			Col.pszText = pText;
			Col.iSubItem = 0;
			SendMessage( GetDlgItem(hDlg, IDC_LIST_SELECT_BUYITEM ), LVM_INSERTCOLUMN, 0, (LPARAM)&Col );
			
			Col.cx = 100;
			pText = GetTxtFromMgr(1678);
			Col.pszText = pText;
			Col.iSubItem = 1;
			SendMessage( GetDlgItem(hDlg, IDC_LIST_SELECT_BUYITEM ), LVM_INSERTCOLUMN, 1, (LPARAM)&Col );
		}
		return TRUE;
	case WM_TIMER:
		{
			if ( IsUserInventoryChanged() )
			{
				LoadUserInventory();
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
			
			if ( hdr->hwndFrom == GetDlgItem( hDlg, IDC_LIST_USER_INVENTORY ) )
			{
				switch( hdr->code )
				{
				case LVN_ITEMCHANGED:
					{
						if ( nia->iItem >= 0 )
						{
							m_siSelectedIndex = nlv->lParam;
						}
					}
					break;
				case LVN_GETINFOTIP:
					{
						if ( ngt->iItem >= 0 )
						{
							SI32 siListViewIndex = 0;
							
							for ( int i = PERSONITEM_INV0; i < PERSONITEM_INV0 + MAX_INVITEM_PER_PERSON; ++i )
							{
								if ( m_pclItem[ i ].siUnique > 0 )
								{
									if ( ngt->iItem == siListViewIndex )
									{
										pclClient->pclItemManager->GetExplain( &m_pclItem[ i ], ngt->pszText );
										break;
									}
									else
									{
										++siListViewIndex;
									}
								}
							}
						}
					}
					break;
				}				
			}
			else if ( hdr->hwndFrom == GetDlgItem(hDlg, IDC_LIST_SELECT_BUYITEM ) )
			{
				switch( hdr->code )
				{
				case LVN_ITEMCHANGED:
					{
						if ( nia->iItem >= 0 )
						{
							m_siListBoxSelectedIndex = nia->iItem;

							if ( m_siListBoxSelectedIndex > -1 )
							{
								HDC hdc= GetDC( m_hDlg );
								if ( m_hItemBitmap )
								{
									::DeleteObject( m_hItemBitmap );
									m_hItemBitmap = NULL;
								}

								m_hItemBitmap = pclClient->pclItemManager->MakeDDBFromDIB( hdc, pclClient->pclItemManager->GetImage( m_siItemUniqueList[ m_siListBoxSelectedIndex ] ) );
								SendDlgItemMessage( m_hDlg, IDC_STATIC_FOREIGN_MERCHANT_BUYITEM_IMAGE, STM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)m_hItemBitmap );
								
								ReleaseDC( m_hDlg, hdc );
							}
						}
					}
					break;
				}
			}
		}
		break;

	case WM_COMMAND:
		{
			switch( LOWORD( wParam ) )
			{
			case IDOK:
			case IDCANCEL:
				Hide();
				break;
			case IDC_COMBO_KIND_FOREIGNMERCHANT:
				{
					switch( HIWORD( wParam ) )
					{
					case CBN_SELCHANGE:
						{
							char ItemTypeName[ 64 ] = "";
							GetWindowText( GetDlgItem( hDlg, IDC_COMBO_KIND_FOREIGNMERCHANT ), ItemTypeName, 63 );
							
							SI32 ItemType = pclClient->pclItemManager->FindItemTypeUniqueFromTypeName( ItemTypeName );
							
							if ( ItemType > 0 )
							{
								// 현재 교역 상인이 구입중인 물품에 대한 정보를 요청함.
								cltGameMsgRequest_ForeignMerchantBuyCashItemInfo cltameMsgRequest_ForeignMerchantBuyCashItemInfo( ItemType );
								cltMsg clMsg(GAMEMSG_REQUEST_FOREIGNMERCHANTBUYCASHITEMINFO, sizeof(cltGameMsgRequest_ForeignMerchantBuyCashItemInfo), (char*)&cltameMsgRequest_ForeignMerchantBuyCashItemInfo);
								pclClient->SendMsgToServer( (char*) &clMsg );
							}
						}
						break;
					}
				}
				break;
			case IDC_BUTTON_SELL_ITEM_TO_FOREIGN_MERCHANT:
				{
					UI16 uiSellItemNum = GetDlgItemInt( hDlg, IDC_EDIT_SELL_ITEM_NUM, NULL, FALSE );

					if ( uiSellItemNum < 1 )
					{
						char* pText = GetTxtFromMgr(1861);
						char* pTitle = GetTxtFromMgr(1862);
						MessageBox( hDlg, pText, pTitle, MB_OK|MB_TOPMOST );
						break;
					}

					if ( uiSellItemNum > 10 )
					{
						char* pText = GetTxtFromMgr(1863);
						char* pTitle = GetTxtFromMgr(1864);
						pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pText, pTitle );
						return FALSE;
					}

					if ( m_siSelectedIndex < 0 )
					{
						char* pText = GetTxtFromMgr(1865);
						char* pTitle = GetTxtFromMgr(1866);
						MessageBox( hDlg, pText, pTitle, MB_OK|MB_TOPMOST );
						break;
					}

					bool Find = false;

					for ( SI16 i = 0; i < m_siListNum; ++i )
					{
						if ( m_pclItem[ m_siSelectedIndex ].siUnique == m_siItemUniqueList[ i ] )
						{
							Find = true;
							break;
						}
					}

					if ( ! Find )
					{
						char* pText = GetTxtFromMgr(1867);
						char* pTitle = GetTxtFromMgr(1868);
						MessageBox( m_hDlg, pText, pTitle, MB_OK|MB_TOPMOST );
						return FALSE;
					}

					cltItem clItem;
					clItem.Set( &m_pclItem[ m_siSelectedIndex ] );
					clItem.siItemNum = uiSellItemNum;

					cltGameMsgRequest_ForeignMerchantBuyCashItem clGameMsgRequest_ForeignMerchantBuyCashItem( m_siSelectedIndex, &clItem );
					cltMsg clMsg( GAMEMSG_REQUEST_FOREIGNMERCHANTBUYCASHITEM, sizeof(clGameMsgRequest_ForeignMerchantBuyCashItem), (char*)&clGameMsgRequest_ForeignMerchantBuyCashItem );
					pclClient->SendMsgToServer((char*)&clMsg);
				}
				break;
			}
		}
		break;
	case WM_DESTROY:
		{
			if ( m_hPortraitBitmap )
			{
				DeleteObject( m_hPortraitBitmap );
				m_hPortraitBitmap = NULL;
			}

			if ( m_hDlg )
			{
				m_hDlg = NULL;
			}
		}
		break;
	}
	return FALSE;
}

bool CForeignMerchantBuyCashItemDlg::IsShow()
{
	return m_bShow;
}

void CForeignMerchantBuyCashItemDlg::Show()
{
	if( m_bShow )
	{
		Hide();
		return;
	}
	
	m_siListBoxSelectedIndex = -1;

	LoadUserInventory();
	SetTimer( m_hDlg, 0, 1000, NULL );

	m_bShow = true;
	ShowWindow( m_hDlg, SW_SHOW );

	return;
}

void CForeignMerchantBuyCashItemDlg::Hide()
{
	m_bShow = false;
	KillTimer( m_hDlg, 0 );

	ShowWindow( m_hDlg, SW_HIDE );

	return;
}

void CForeignMerchantBuyCashItemDlg::Set( stItemInfoList *ItemInfoList, SI16 ListNum )
{
	ListView_DeleteAllItems( GetDlgItem( m_hDlg, IDC_LIST_SELECT_BUYITEM ) );
	
	SI32 i = 0;
	for ( i = 0; i < ListNum; ++i )
	{
		m_siItemUniqueList[ i ] = ItemInfoList[ i ].siBuyItemUniqueList;
		m_uiItemPriceList[ i ] = ItemInfoList[ i ].uiBuyItemPriceList;
	}

	m_siListNum = ListNum;
	
	LVITEM Li;
	Li.mask = LVIF_TEXT;
	Li.state = 0;
	Li.stateMask = 0;	
	
	Li.iItem = 0;	
	Li.iSubItem = 0;
	
	SI16 Count = 0;
	
	char Buffer[ 1024 ];
	
	for ( i = 0; i < ListNum; ++i )
	{
		char *ItemName = ( char * )pclClient->pclItemManager->GetName( m_siItemUniqueList[ i ] );
		
		if ( ItemName == NULL )
		{
			continue;
		}
		
		Li.iItem = Count++;
		Li.iSubItem = 0;
		Li.pszText = ItemName;
		SendMessage( GetDlgItem( m_hDlg, IDC_LIST_SELECT_BUYITEM ), LVM_INSERTITEM, 0, (LPARAM)&Li );
		
		Li.iSubItem = 1;
		sprintf( Buffer, "%d", m_uiItemPriceList[ i ] );
		Li.pszText = Buffer;			
		SendMessage( GetDlgItem( m_hDlg, IDC_LIST_SELECT_BUYITEM ), LVM_SETITEM, 0, (LPARAM)&Li );
		
		if ( i == 0 )
		{
			HDC hdc= GetDC( m_hDlg );
			if ( m_hItemBitmap )
			{
				DeleteObject( m_hItemBitmap );
				m_hItemBitmap = NULL;
			}
			
			m_siListBoxSelectedIndex = 0;
			m_hItemBitmap = pclClient->pclItemManager->MakeDDBFromDIB( hdc, pclClient->pclItemManager->GetImage( m_siItemUniqueList[ i ] ) );
			SendDlgItemMessage( m_hDlg, IDC_STATIC_FOREIGN_MERCHANT_BUYITEM_IMAGE, STM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)m_hItemBitmap );
			ReleaseDC( m_hDlg, hdc );
		}
	}
	

	return;
}