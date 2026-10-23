#include "TradeMerchantDish.h"

#include "../../Client/client.h"

#include "../../CommonLogic/MsgType.h"
#include "../../CommonLogic/CommonLogic.h"
#include "../../CommonLogic/MsgType-System.h"
#include "../../CommonLogic/Msg/MsgType-Village.h"
#include "../../CommonLogic/Msg/MsgType-TradeMerchant.h"
#include "..\..\Client\Interface\ErrorMsgDlg\ErrorMsgDlg.h"

#include "../../../lib/WebHTML/WebHTML.h"

//-----------------------------
// Common
//-----------------------------
#include "PersonInfo.h"
#include "Char\CharManager\CharManager.h"

extern cltCommonLogic* pclClient;

CTradeMerchantDish::CTradeMerchantDish()
{
	m_hDlg = NULL;
	m_bShow = false;	
	m_hEditBrush = NULL;
	m_hPortraitBitmap = NULL;

	m_pclItem = new cltItem[ MAX_ITEM_PER_PERSON ];
	m_siSelectedIndex = -1;
	
	m_siItemPrice = 0;
}

CTradeMerchantDish::~CTradeMerchantDish()
{
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

void CTradeMerchantDish::Create( HINSTANCE hInst, HWND hWnd )
{
	m_hDlg = CreateDialog( hInst, MAKEINTRESOURCE( IDD_DIALOG_TRADEMERCHANT_DISH ), hWnd,  StaticTradeMerchantDishDlgProc );
	Hide();
	return;
}


bool CTradeMerchantDish::IsInventoryChanged()
{
	for ( int j = PERSONITEM_INV0; j < PERSONITEM_SUMMON1EQUIP0; ++j )		// 장착창은 체크할 필요 없으므로, 10 부터 함.
	{
		SI32 size = sizeof(cltItem);

		char* src = (char*)&m_pclItem[ j ];
		char* dst = (char*)&pclClient->pclCM->CR[1]->pclCI->clCharItem.clItem[ j];

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

void CTradeMerchantDish::LoadDish()
{
	ListView_DeleteAllItems( GetDlgItem( m_hDlg, IDC_LIST_USERINVENTORY ) );
	
	m_siSelectedIndex = -1;

	LVITEM Li;
	Li.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM;
	Li.state = 0;
	Li.stateMask = 0;	
	
	SI16 Counter = 0;

	char buffer[ 256 ] = "";
	char ItemName[ 128 ] = "";
	
	for ( SI32 i = PERSONITEM_INV0; i < PERSONITEM_SUMMON1EQUIP0; ++i )		// 장착창은 체크할 필요 없으므로, 10 부터 함.
	{
		m_pclItem[ i ] = pclClient->pclCM->CR[1]->pclCI->clCharItem.clItem[ i ];

		if ( m_pclItem[ i ].siUnique > 0 )
		{			
			if ( m_pclItem[ i ].clItemCommon.clCommonInfo.uiRareInfo == RARELEVEL_2 ||
				m_pclItem[ i ].clItemCommon.clCommonInfo.uiRareInfo == RARELEVEL_1
				)
			{
				if( m_pclItem[i].GetFullName( pclClient->pclItemManager, ItemName ) == true )
				{
					Li.iImage	= m_pclItem[i].GetImage( pclClient->pclItemManager );
					Li.iSubItem	= 0;
					Li.iItem	= Counter;
					
					char* pText = GetTxtFromMgr(676);
					sprintf(buffer, pText, ItemName, m_pclItem[i].GetItemNum() );
					Li.pszText	= buffer;
					Li.lParam	= i;
					Counter++;
					
					SendMessage( GetDlgItem( m_hDlg, IDC_LIST_USERINVENTORY ), LVM_INSERTITEM, 0, (LPARAM)&Li );
				}
			}

		}
	}
	
	ListView_SetIconSpacing( GetDlgItem( m_hDlg, IDC_LIST_USERINVENTORY ), LISTVIEW_ITEM_WIDTH, LISTVIEW_ITEM_HEIGHT );

	return;
}

BOOL CALLBACK CTradeMerchantDish::StaticTradeMerchantDishDlgProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam )
{
	cltClient *pclclient = ( cltClient * )pclClient;
	return pclclient->m_pTradeMerchantDish->TradeMerchantDishDlgProc( hDlg, iMsg, wParam, lParam );
}

BOOL CALLBACK CTradeMerchantDish::TradeMerchantDishDlgProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam )
{
	switch ( iMsg )
	{
	case WM_INITDIALOG:
		{
			InitCommonControls();
			
			m_hDlg = hDlg;
			
			ListView_SetExtendedListViewStyle( GetDlgItem( hDlg, IDC_LIST_USERINVENTORY ), LVS_EX_FULLROWSELECT | LVS_EX_ONECLICKACTIVATE | LVS_EX_INFOTIP );

			cltClient *pclclient = (cltClient *)pclClient;
			pclclient->pclImageList->ConnectToListView( GetDlgItem( hDlg, IDC_LIST_NPCBUY_ITEM_IMAGE ), LVSIL_NORMAL );
			pclclient->pclImageList->ConnectToListView( GetDlgItem( hDlg, IDC_LIST_USERINVENTORY ), LVSIL_NORMAL );

			// EditBrush 
			m_hEditBrush = CreateSolidBrush( COLOR_DIALOG_WHITEBLUE );

			m_hPortraitBitmap = (HBITMAP)LoadImage( pclClient->GetInstance(), "Interface/Portrait/KIND_MERCHANTGUN.BMP", IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE );
			
			if ( m_hPortraitBitmap != NULL )
			{
				SendDlgItemMessage( m_hDlg, IDC_STATIC_TRADEMERCHANT_IMAGE, STM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)m_hPortraitBitmap );
			}
		}
		return TRUE;
	case WM_TIMER:
		{
			if ( IsInventoryChanged() )
			{
				LoadDish();
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
			
			if ( hdr->hwndFrom == GetDlgItem ( hDlg, IDC_LIST_USERINVENTORY ) )
			{
				switch( hdr->code )
				{
				case LVN_ITEMCHANGED:
					{
						if( nlv->uChanged == LVIF_STATE && nlv->uNewState == ( LVIS_SELECTED | LVIS_FOCUSED ) )
						{
							m_siSelectedIndex = nlv->lParam;

							cltGameMsgRequest_TradeMerchantBuyItemInfo clGameMsgRequest_TradeMerchantBuyItemInfo( KIND_MERCHANTPOTTER, m_pclItem[ m_siSelectedIndex ].siUnique, m_siSelectedIndex );
							cltMsg clMsg( GAMEMSG_REQUEST_TRADEMERCHANTBUYITEMINFO, sizeof(clGameMsgRequest_TradeMerchantBuyItemInfo), (char*)&clGameMsgRequest_TradeMerchantBuyItemInfo );
							pclClient->SendMsgToServer( (char*) &clMsg );
						}
					}
					break;
				case LVN_GETINFOTIP:
					{
						if ( ngt->iItem >= 0 )
						{
							SI32 siListViewIndex = 0;
							
							for ( int i = PERSONITEM_INV0; i < PERSONITEM_SUMMON1EQUIP0; ++i )
							{
								if ( m_pclItem[ i ].siUnique > 0  )
								{	
									if ( m_pclItem[ i ].clItemCommon.clCommonInfo.uiRareInfo == RARELEVEL_2 ||
										m_pclItem[ i ].clItemCommon.clCommonInfo.uiRareInfo == RARELEVEL_1
										)
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
					}
					break;
				}
			}
		}
		break;
	case WM_CTLCOLORSTATIC:
		{
			if ( GetDlgItem( hDlg, IDC_EDIT_EXPLAIN ) == (HWND)lParam
				)
			{
				HDC hdc = (HDC)wParam;
				SetTextColor( hdc, COLOR_DIALOG_BLACK );
				SetBkColor( hdc, COLOR_DIALOG_WHITEBLUE);
				return (BOOL)m_hEditBrush;
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
			case IDC_BUTTON_SELLITEM:
				{
					if ( m_siItemPrice < 1 )
					{
						return FALSE;
					}

					if ( m_siSelectedIndex > 0 )
					{
						char buffer[256 ] = "";
						char* pText = GetTxtFromMgr(2118);
						sprintf( buffer, pText, m_siItemPrice );
						pText = GetTxtFromMgr(2119);
						if( MessageBox(NULL, buffer, pText, MB_ICONQUESTION | MB_YESNO) != IDYES ) break;

						cltGameMsgRequest_TradeMerchantBuyItem clGameMsgRequest_TradeMerchantBuyItem( KIND_MERCHANTPOTTER, m_pclItem[ m_siSelectedIndex ].siUnique, m_siSelectedIndex );
						cltMsg clMsg( GAMEMSG_REQUEST_TRADEMERCHANTBUYITEM, sizeof(clGameMsgRequest_TradeMerchantBuyItem), (char*)&clGameMsgRequest_TradeMerchantBuyItem );
						pclClient->SendMsgToServer( (char*) &clMsg );
					}
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

			if ( m_hEditBrush )
			{
				DeleteObject( m_hEditBrush );
				m_hEditBrush = NULL;
			}

			m_hDlg = NULL;
		}
		break;
	}
	return FALSE;
}

bool CTradeMerchantDish::IsShow()
{
	return m_bShow;
}

void CTradeMerchantDish::Show( SI32 NPCCharID )
{
	if ( m_bShow )
	{
		Hide();
		return;
	}
		
	char* pText = GetTxtFromMgr(2120);
	SetWindowText( GetDlgItem( m_hDlg, IDC_EDIT_EXPLAIN ), pText );

	LoadDish();
	m_bShow = true;
	SetTimer( m_hDlg, 0, 1000, NULL );			// 초당 1번씩 refresh
	ShowWindow( m_hDlg, SW_SHOW );

	return;
}

void CTradeMerchantDish::Hide()
{
	m_bShow = false;
	KillTimer( m_hDlg, 0 );

	ShowWindow( m_hDlg, SW_HIDE );
	
	return;
}

void CTradeMerchantDish::SetDishPrice( SI16 DishItemUnique, SI32 DishPrice )
{
	cltItem clItem;
	clItem.Init();
	clItem.siUnique = DishItemUnique;
	clItem.siItemNum = 1;
	SetInventoryInListView( pclClient->pclItemManager, m_hDlg, IDC_LIST_NPCBUY_ITEM_IMAGE, NULL, &clItem, 5, 5 );

	char *ItemName = (char*)pclClient->pclItemManager->GetName( DishItemUnique );
	char Buffer[256 ] = "";
	char* pText = GetTxtFromMgr(2121);
	sprintf( Buffer, pText, ItemName, DishPrice );
	SetWindowText( GetDlgItem( m_hDlg, IDC_EDIT_EXPLAIN ), Buffer );

	m_siItemPrice = DishPrice;

	return;
}

void CTradeMerchantDish::SellDish( SI16 DishItemUnique )
{
	char buffer[256 ] = "";
	char* pText = GetTxtFromMgr(2122);
	sprintf( buffer, pText, pclClient->pclItemManager->GetName( DishItemUnique ) );
	SetWindowText( GetDlgItem( m_hDlg, IDC_EDIT_EXPLAIN ), buffer );

	m_siItemPrice = 0;
	
	return;
}