#include "ForeignMerchantDlg.h"

#include "..\..\..\Common\SMLib\LibList\LibListMgr.h"
#include "..\..\..\Common\SystemNPC\ForeignMerchantNPC\ForeignMerchantNPC.h"

#include "..\ErrorMsgDlg\ErrorMsgDlg.h"
#include "..\NotifyMsgDlg\NotifyMsgDlg.h"

#include "..\..\..\CommonLogic\CommonLogic.h"

//-----------------------------
// Common
//-----------------------------
#include "PersonInfo.h"
#include "Char\CharManager\CharManager.h"

#include "..\..\Client\Client.h"
extern cltCommonLogic* pclClient;

CForeignMerchantDlg::CForeignMerchantDlg()
{
	m_bShow = false;

	m_hDlg  = NULL;
	m_hSellItemListCtrl = NULL;
	m_pPersonItemInfo = NULL;
	m_pForeignMerchantMgr = NULL;
	m_siSelectedIndex = -1;
	m_hItemImage = NULL;
	m_bIsListRotate = false;	
	m_si32ImageListIndex = 0;
	m_siForeignMerchantKind = -1;
	
	m_hPortraitBitmap = NULL;
	m_hItemBitmap = NULL;


	m_pForeignMerchantNPC = NULL;
	memset( &m_ForeignMerchantNPCInfo, 0, sizeof( m_ForeignMerchantNPCInfo ) );
	
	m_siNPCCharUnique = 0;

	m_siItemType = 0;
		 
	Initialize();
}

CForeignMerchantDlg::~CForeignMerchantDlg()
{
	if ( m_hItemBitmap ) 
	{
		DeleteObject( m_hItemBitmap );
		m_hItemBitmap = NULL;
	}

	if ( m_pForeignMerchantMgr )
	{
		delete m_pForeignMerchantMgr;
		m_pForeignMerchantMgr = NULL;
	}

	if ( m_hDlg )
	{
		DestroyWindow( m_hDlg );
	}

}

void CForeignMerchantDlg::Initialize()
{
	return;
}

void CForeignMerchantDlg::Destroy()
{
	return;
}

void CForeignMerchantDlg::Create( HWND hWnd, HINSTANCE hInst )
{
	m_pPersonItemInfo = &pclClient->pclCM->CR[1]->pclCI->clCharItem;

	if ( m_pForeignMerchantMgr == NULL )
	{
		m_pForeignMerchantMgr = new CLibListMgr();
		m_pForeignMerchantMgr->CreateMaxEmptyObj( MAX_FOREIGN_MERCHANT_OBJ_NUM, MAX_FOREIGN_MERCHANT_OBJ_BUFFER_SIZE );
	}

	m_hDlg = CreateDialog( hInst, MAKEINTRESOURCE(IDD_DIALOG_FOREIGN_MERCHANT ), hWnd, StaticForeignMerchantDlgProc );	

	Hide();

	return;
}

BOOL CALLBACK CForeignMerchantDlg::StaticForeignMerchantDlgProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam )
{
	cltClient *pclclient = (cltClient *)pclClient;
	return pclclient->m_pForeignMerchantDlg->ForeignMerchantDlgProc( hDlg, iMsg, wParam, lParam );
}

BOOL CALLBACK CForeignMerchantDlg::ForeignMerchantDlgProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam )
{
	switch ( iMsg )
	{
	case WM_INITDIALOG:
		{			
			m_hDlg = hDlg;

			g_FontManager.SetFont( hDlg, IDC_LIST_USER_INVENTORY );

			
			cltClient *pclclient = (cltClient *)pclClient;
			pclclient->pclImageList->ConnectToListView( GetDlgItem( hDlg, IDC_LIST_USER_INVENTORY ), LVSIL_NORMAL );

			
			if( m_hPortraitBitmap == NULL )
			{
				m_hPortraitBitmap = (HBITMAP)LoadImage( pclClient->GetInstance(), "Interface/Portrait/KIND_CHINAMERCHANT.bmp", IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE );

				if ( m_hPortraitBitmap != NULL )
				{
					SendDlgItemMessage( hDlg, IDC_STATIC_FOREIGN_MERCHANT_IMAGE, STM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)m_hPortraitBitmap);
				}
			}

			char* pText = GetTxtFromMgr(2397);
			SetWindowText( hDlg, pText );

			pText = GetTxtFromMgr(1024);
			SendMessage( GetDlgItem( hDlg, IDC_COMBO_KIND_FOREIGNMERCHANT  ), CB_ADDSTRING, 0, (LPARAM)pText );
			pText = GetTxtFromMgr(1026);
			SendMessage( GetDlgItem( hDlg, IDC_COMBO_KIND_FOREIGNMERCHANT  ), CB_ADDSTRING, 0, (LPARAM)pText );
			pText = GetTxtFromMgr(2370);
			SendMessage( GetDlgItem( hDlg, IDC_COMBO_KIND_FOREIGNMERCHANT  ), CB_ADDSTRING, 0, (LPARAM)pText );
			pText = GetTxtFromMgr(2366);
			SendMessage( GetDlgItem( hDlg, IDC_COMBO_KIND_FOREIGNMERCHANT  ), CB_ADDSTRING, 0, (LPARAM)pText );			
			pText = GetTxtFromMgr(2376);
			SendMessage( GetDlgItem( hDlg, IDC_COMBO_KIND_FOREIGNMERCHANT  ), CB_ADDSTRING, 0, (LPARAM)pText );
			pText = GetTxtFromMgr(2368);
			SendMessage( GetDlgItem( hDlg, IDC_COMBO_KIND_FOREIGNMERCHANT  ), CB_ADDSTRING, 0, (LPARAM)pText );
			pText = GetTxtFromMgr(2374);
			SendMessage( GetDlgItem( hDlg, IDC_COMBO_KIND_FOREIGNMERCHANT  ), CB_ADDSTRING, 0, (LPARAM)pText );
			pText = GetTxtFromMgr(2372);
			SendMessage( GetDlgItem( hDlg, IDC_COMBO_KIND_FOREIGNMERCHANT  ), CB_ADDSTRING, 0, (LPARAM)pText );			
			pText = GetTxtFromMgr(1030);
			SendMessage( GetDlgItem( hDlg, IDC_COMBO_KIND_FOREIGNMERCHANT  ), CB_ADDSTRING, 0, (LPARAM)pText );
			pText = GetTxtFromMgr(1029);
			SendMessage( GetDlgItem( hDlg, IDC_COMBO_KIND_FOREIGNMERCHANT  ), CB_ADDSTRING, 0, (LPARAM)pText );			
			pText = GetTxtFromMgr(2398);
			SendMessage( GetDlgItem( hDlg, IDC_COMBO_KIND_FOREIGNMERCHANT  ), CB_ADDSTRING, 0, (LPARAM)pText );
			pText = GetTxtFromMgr(2399);
			SendMessage( GetDlgItem( hDlg, IDC_COMBO_KIND_FOREIGNMERCHANT  ), CB_ADDSTRING, 0, (LPARAM)pText );
			pText = GetTxtFromMgr(1028);
			SendMessage( GetDlgItem( hDlg, IDC_COMBO_KIND_FOREIGNMERCHANT  ), CB_ADDSTRING, 0, (LPARAM)pText );
			pText = GetTxtFromMgr(1027);
			SendMessage( GetDlgItem( hDlg, IDC_COMBO_KIND_FOREIGNMERCHANT  ), CB_ADDSTRING, 0, (LPARAM)pText );
			pText = GetTxtFromMgr(1025);
			SendMessage( GetDlgItem( hDlg, IDC_COMBO_KIND_FOREIGNMERCHANT  ), CB_ADDSTRING, 0, (LPARAM)pText );
			pText = GetTxtFromMgr(1031);
			SendMessage( GetDlgItem( hDlg, IDC_COMBO_KIND_FOREIGNMERCHANT  ), CB_ADDSTRING, 0, (LPARAM)pText );
			pText = GetTxtFromMgr(2470);
			SendMessage( GetDlgItem( hDlg, IDC_COMBO_KIND_FOREIGNMERCHANT  ), CB_ADDSTRING, 0, (LPARAM)pText );
			pText = GetTxtFromMgr(2471);
			SendMessage( GetDlgItem( hDlg, IDC_COMBO_KIND_FOREIGNMERCHANT  ), CB_ADDSTRING, 0, (LPARAM)pText );
			pText = GetTxtFromMgr(2378);
			SendMessage( GetDlgItem( hDlg, IDC_COMBO_KIND_FOREIGNMERCHANT  ), CB_ADDSTRING, 0, (LPARAM)pText);

			ListView_SetExtendedListViewStyle( GetDlgItem(hDlg, IDC_LIST_USER_INVENTORY ), LVS_EX_FULLROWSELECT | LVS_EX_ONECLICKACTIVATE | LVS_EX_UNDERLINEHOT | LVS_EX_INFOTIP );
			ListView_SetExtendedListViewStyle( GetDlgItem(hDlg, IDC_LIST_SELECT_BUYITEM ), LVS_EX_FULLROWSELECT | LVS_EX_ONECLICKACTIVATE );
			
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

	case WM_TIMER:				// 지속적으로 내 인벤토리 아이템들이 바뀌었는지 확인해야 하므로...
		{
			if ( IsInventoryChanged() )
			{
				LoadInventoryData();
			}

			if ( IsNPCBuyItemInfoChanged() )
			{
				LoadInventoryData();
				LoadForeignNPCData();
			}
		}
		return TRUE;

	case WM_COMMAND:
		{
			switch ( LOWORD( wParam ) )
			{
			case IDC_COMBO_KIND_FOREIGNMERCHANT:
				{
					switch( HIWORD( wParam ) )
					{
					case CBN_SELCHANGE:
						{
							SI32 SelectedIndex = SendMessage( GetDlgItem( hDlg, IDC_COMBO_KIND_FOREIGNMERCHANT  ), CB_GETCURSEL, 0, 0 );
							
							if ( SelectedIndex == CB_ERR )
							{
								return FALSE;
							}
							
							m_siForeignMerchantKind = SelectedIndex;
							
							// 현재 교역 상인이 구입중인 물품에 대한 정보를 요청함.
							cltGameMsgRequest_ForeignMerchantNPCBuyInfo clForeignMerchantNPCBuyInfo( m_siForeignMerchantKind );
							cltMsg clMsg(GAMEMSG_REQUEST_FOREIGNMERCHANTNPCBUYINFO, sizeof(cltGameMsgRequest_ForeignMerchantNPCBuyInfo), (char*)&clForeignMerchantNPCBuyInfo);
							pclClient->SendMsgToServer( (char*) &clMsg );
						}
						break;
					}
				}
				break;
			case IDCANCEL:
				{
					Hide();
				}
				break;
				
			case IDC_BUTTON_SELL_ITEM_TO_FOREIGH_MERCHANT:
				{
					UI16 uiSellItemNum = GetDlgItemInt( hDlg, IDC_EDIT_SELL_ITEM_NUM, NULL, FALSE );

					if ( uiSellItemNum < 1 )
					{
						char* pTitle = GetTxtFromMgr(1862);
						char* pText = GetTxtFromMgr(1861);

						MessageBox( hDlg, pText, pTitle, MB_OK|MB_TOPMOST );
						break;
					}

					if ( m_siSelectedIndex < 0 )
					{
						char* pTitle = GetTxtFromMgr(1866);
						char* pText = GetTxtFromMgr(1865);

						MessageBox( hDlg, pText, pTitle, MB_OK|MB_TOPMOST );
						break;
					}

					Push( uiSellItemNum );
				}
				break;
			
			/*
			case IDC_LIST_SELECT_BUYITEM:
				{
					switch( HIWORD( wParam ) )
					{
					case CBN_SELCHANGE:
						{
							m_siListBoxSelectedIndex = (SI16)SendMessage( GetDlgItem( hDlg, IDC_LIST_SELECT_BUYITEM ), LB_GETCURSEL, 0, 0 );

							if ( m_siListBoxSelectedIndex > -1 )
							{
								HDC hdc= GetDC( m_hDlg );
								if ( m_hItemBitmap )
								{
									::DeleteObject( m_hItemBitmap );
									m_hItemBitmap = NULL;
								}

								m_hItemBitmap = pclClient->pclItemManager->MakeDDBFromDIB( hdc, pclClient->pclItemManager->GetImage( m_ForeignMerchantNPCInfo.siBuyItemUniqueList[ m_siListBoxSelectedIndex ] ) );
								SendDlgItemMessage( m_hDlg, IDC_STATIC_FOREIGN_MERCHANT_BUYITEM_IMAGE, STM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)m_hItemBitmap );
								
								ReleaseDC( m_hDlg, hdc );
							}
						}
						break;
					}
				}
				break;
			*/
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
			
			switch( hdr->code )
			{
			case LVN_KEYDOWN:
				{
					LPNMLVKEYDOWN lpkeydown = ( LPNMLVKEYDOWN) lParam;
					if ( lpkeydown->wVKey == VK_ESCAPE || lpkeydown->wVKey == VK_F2 )
					{
						Hide();
						return TRUE;
					}
				}
				break;
			}

			if ( hdr->hwndFrom == GetDlgItem( hDlg, IDC_LIST_USER_INVENTORY ) )
			{
				switch( hdr->code )
				{
				case LVN_ITEMCHANGED:
					{
						if ( nia->iItem >= 0 )
						{
							m_siSelectedIndex = nlv->lParam;

							SetDlgItemInt( hDlg, IDC_EDIT_SELL_ITEM_NUM, m_clItem[ m_siSelectedIndex ].siItemNum, FALSE );
							return TRUE;
						}
					}
					break;
				case LVN_GETINFOTIP:
					{
						if ( ngt->iItem >= 0 && m_siItemType > 0 )
						{
							SI16 Count = 0;
							for ( SI16 i = 0; i < MAX_ITEM_PER_PERSON; ++i )
							{
								if ( m_clItem[ i ].siUnique > 0 )
								{
									SI32 ref = pclClient->pclItemManager->FindItemRefFromUnique( m_clItem[ i ].siUnique );

									if(ref > 0)
									{
										SI32 ItemType = pclClient->pclItemManager->pclItemInfo[ref]->siType;

										if ( ItemType == m_siItemType )
										{
											if ( ngt->iItem == Count )
											{
												pclClient->pclItemManager->GetExplain( &m_clItem[ i ], ngt->pszText );
												break;
											}
											else
											{
												++Count;
											}
										}
									}
								}
							}
						}
					}
					break;
				}
			}

			if ( hdr->hwndFrom == GetDlgItem(hDlg, IDC_LIST_SELECT_BUYITEM ) )
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

								m_hItemBitmap = pclClient->pclItemManager->MakeDDBFromDIB( hdc, pclClient->pclItemManager->GetImage( m_ForeignMerchantNPCInfo.siBuyItemUniqueList[ m_siListBoxSelectedIndex ] ) );
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


	case WM_CLOSE:
		{
			Hide();
		}
		break;

	case WM_DESTROY:
		{
			KillTimer( m_hDlg, 0 );

			if ( m_hPortraitBitmap )
			{
				DeleteObject( m_hPortraitBitmap );
				m_hPortraitBitmap = NULL;
			}

			m_hDlg = NULL;
		}
		break;
	}
	return FALSE;
}

bool CForeignMerchantDlg::IsShow()
{
	return m_bShow;
}

void CForeignMerchantDlg::Show( SI32 npccharunique )
{
	if ( m_bShow )
	{
		Hide();
		return;
	}
	
	m_siItemType = 0;
	m_siNPCCharUnique = npccharunique;

	m_bShow = true;
	SetTimer( m_hDlg, 0, FOREIGNMERCHANTDLG_SETTIMER_TIME, NULL );
	
	char* pText = GetTxtFromMgr(2472);
	SetWindowText( GetDlgItem( m_hDlg, IDC_EDIT_FOREIGN_MERCHANT_EXPLAIN1 ), pText );
	
	SendMessage( GetDlgItem( m_hDlg, IDC_COMBO_KIND_FOREIGNMERCHANT  ), CB_SETCURSEL, (WPARAM)-1, 0 );

	ListView_DeleteAllItems( GetDlgItem( m_hDlg, IDC_LIST_SELECT_BUYITEM ) );	
	
	m_siForeignMerchantKind = -1;


	LoadInventoryData();

	ShowWindow( m_hDlg, SW_SHOWNORMAL );
	SetForegroundWindow( m_hDlg );

	return;
}

void CForeignMerchantDlg::Hide()
{
	m_siNPCCharUnique = 0;

	m_bShow = false;
	KillTimer( m_hDlg, 0 );

	ShowWindow( m_hDlg, SW_HIDE );

	return;
}

void CForeignMerchantDlg::LoadInventoryData()
{
	ListView_DeleteAllItems( GetDlgItem( m_hDlg, IDC_LIST_USER_INVENTORY ) );
	/*
	if ( m_siForeignMerchantBuyItemUnique < 0 )				// 상인이 구입하지 않는 상태
	{
		return;
	}
	*/

	m_siSelectedIndex = -1;

	for ( SI32 i = PERSONITEM_INV0; i < PERSONITEM_SUMMON1EQUIP0; ++i )		// 장착창은 체크할 필요 없으므로, 10 부터 함.
	{
		m_clItem[ i ] = m_pPersonItemInfo->clItem[ i ];
	}

	//SetInventoryInListView( pclClient->pclItemManager, m_hDlg, IDC_LIST_USER_INVENTORY, NULL, m_pPersonItemInfo );
	
	m_pForeignMerchantNPC =  pclClient->pclForeignMerchantNPCMgr->GetForeignMerchantNPC( m_siForeignMerchantKind );	
	m_pForeignMerchantNPC->GetForeignMerchantNPCInfo( &m_ForeignMerchantNPCInfo );
	if ( m_ForeignMerchantNPCInfo.siBuyItemListCount > 0 )
	{
		cltItem clItem;
		clItem.Init();
		clItem.siUnique = m_ForeignMerchantNPCInfo.siBuyItemUniqueList[ 0 ];
		clItem.siItemNum = 1;
		
		SI32 ref = pclClient->pclItemManager->FindItemRefFromUnique( clItem.siUnique );
	
		if(ref > 0)
		{
			SI32 ItemType = pclClient->pclItemManager->pclItemInfo[ref]->siType;
			if ( ItemType > 0 )
			{
				SetInventoryInListView( pclClient->pclItemManager, m_hDlg, IDC_LIST_USER_INVENTORY, NULL, m_clItem, ItemType );
			}

			m_siItemType = ItemType;
		}
	}
	

	return;
}

void CForeignMerchantDlg::LoadForeignNPCData()
{	
	ListView_DeleteAllItems( GetDlgItem( m_hDlg, IDC_LIST_SELECT_BUYITEM ) );	
	

	m_pForeignMerchantNPC =  pclClient->pclForeignMerchantNPCMgr->GetForeignMerchantNPC( m_siForeignMerchantKind );	

	if ( m_pForeignMerchantNPC == NULL )			// 없는 NPC 찾고 있으므로...
	{
		char* pText = GetTxtFromMgr(2473);
		SetWindowText( GetDlgItem( m_hDlg, IDC_EDIT_FOREIGN_MERCHANT_EXPLAIN1 ), pText );
		return;
		//m_siForeignMerchantKind = -1;
		//return;
	}
	else
	{
		m_pForeignMerchantNPC->GetForeignMerchantNPCInfo( &m_ForeignMerchantNPCInfo );

		/*
		if( m_hPortraitBitmap != NULL )
		{
			if ( m_hPortraitBitmap )
			{
				DeleteObject( m_hPortraitBitmap );
				m_hPortraitBitmap = NULL;
			}
		}
		
		char *NPCName = (char*) pclClient->pclKindInfoSet->GetName( m_siForeignMerchantKind );
			
		if ( NPCName != NULL )
		{
			SetWindowText( m_hDlg, NPCName );
			
			char *Kind = pclClient->pclKindInfoSet->pclKI[ m_siForeignMerchantKind ]->szCode;
			
			if ( Kind != NULL )
			{
				char PortraitBitmapPath[128] = "Interface/Portrait/";
				strcat( PortraitBitmapPath, Kind );
				strcat( PortraitBitmapPath, ".bmp" );
				m_hPortraitBitmap = (HBITMAP)LoadImage( pclClient->GetInstance(), PortraitBitmapPath, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE );
				
				if ( m_hPortraitBitmap != NULL )
				{
					SendDlgItemMessage( m_hDlg, IDC_STATIC_FOREIGN_MERCHANT_IMAGE, STM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)m_hPortraitBitmap);
				}
			}
		}
		*/

		char* pText = GetTxtFromMgr(2474);
		SetWindowText( GetDlgItem( m_hDlg, IDC_EDIT_FOREIGN_MERCHANT_EXPLAIN1 ), pText );
		
		LVITEM Li;
		Li.mask = LVIF_TEXT;
		Li.state = 0;
		Li.stateMask = 0;	
		
		Li.iItem = 0;	
		Li.iSubItem = 0;
		
		SI16 Count = 0;

		char Buffer[ 1024 ];
		
		SI32 i = 0;
		for (; i < m_ForeignMerchantNPCInfo.siBuyItemListCount; ++i )
		{
			char *ItemName = ( char * )pclClient->pclItemManager->GetName( m_ForeignMerchantNPCInfo.siBuyItemUniqueList[ i ] );

			if ( ItemName == NULL )
			{
				continue;
			}

			Li.iItem = Count++;
			Li.iSubItem = 0;
			Li.pszText = ItemName;
			SendMessage( GetDlgItem( m_hDlg, IDC_LIST_SELECT_BUYITEM ), LVM_INSERTITEM, 0, (LPARAM)&Li );

			Li.iSubItem = 1;
			sprintf( Buffer, "%d", m_ForeignMerchantNPCInfo.siBuyItemPriceList[ i ] );
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
				m_hItemBitmap = pclClient->pclItemManager->MakeDDBFromDIB( hdc, pclClient->pclItemManager->GetImage( m_ForeignMerchantNPCInfo.siBuyItemUniqueList[ m_siListBoxSelectedIndex ] ) );
				SendDlgItemMessage( m_hDlg, IDC_STATIC_FOREIGN_MERCHANT_BUYITEM_IMAGE, STM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)m_hItemBitmap );
				ReleaseDC( m_hDlg, hdc );
			}
		}
	}
	
	m_siListBoxSelectedIndex = -1;

	return;
}

bool CForeignMerchantDlg::IsInventoryChanged()
{
	for ( int j = PERSONITEM_INV0; j < PERSONITEM_SUMMON1EQUIP0; ++j )		// 장착창은 체크할 필요 없으므로, 10 부터 함.
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

bool CForeignMerchantDlg::IsNPCBuyItemInfoChanged()
{
	m_pForeignMerchantNPC =  pclClient->pclForeignMerchantNPCMgr->GetForeignMerchantNPC( m_siForeignMerchantKind );

	if ( m_pForeignMerchantNPC == NULL )
	{
		return false;
	}
	
	stForeignMerchantNPCInfo *pForeignMerchantNPCInfo = m_pForeignMerchantNPC->GetForeignMerchantNPCInfo();

	if ( pForeignMerchantNPCInfo == NULL )
	{
		return false;
	}

	SI32 size = sizeof(stForeignMerchantNPCInfo);
	char* src = (char*)&m_ForeignMerchantNPCInfo;
	char* dst = (char*)pForeignMerchantNPCInfo;

	for(int i = 0;i < size; i++)
	{
		if(src[i] != dst[i])
		{
			return true;
		}
	}

	return false;
}

void CForeignMerchantDlg::Push( UI16 uiSellItemNum )
{
	/*
	if ( m_pForeignMerchantNPCInfo == NULL )
	{
		return;
	}
	*/

	if ( m_siNPCCharUnique < 1 )
	{
		return;
	}

	if ( uiSellItemNum > 1000 )
	{
		cltClient *pclclient = (cltClient *)pclClient;
		char* pTitle = GetTxtFromMgr(1863);
		char* pText = GetTxtFromMgr(2475);
		pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText );
		return;
	}

	bool bFindExactItem = false;
	for ( SI32 i = 0; i < m_ForeignMerchantNPCInfo.siBuyItemListCount; ++i )
	{
		if ( m_ForeignMerchantNPCInfo.siBuyItemUniqueList[ i ] == m_clItem[ m_siSelectedIndex ].siUnique )
		{
			bFindExactItem = true;
			break;
		}
	}

	if ( ! bFindExactItem )
	{
		char* pTitle = GetTxtFromMgr(1868);
		char* pText = GetTxtFromMgr(1867);
		MessageBox( m_hDlg, pText, pTitle, MB_OK|MB_TOPMOST );
		return;
	}
	
	stSellToForeignMerchant sSellToForeignMerchant;
	sSellToForeignMerchant.siForeignMerchantKind = m_siForeignMerchantKind;
	sSellToForeignMerchant.uiSelectedIndex = (UI16) m_siSelectedIndex;
	sSellToForeignMerchant.uiSellItemNum = uiSellItemNum;
	sSellToForeignMerchant.siSellItemUnique = m_clItem[ m_siSelectedIndex ].siUnique;
	sSellToForeignMerchant.siNPCCharUnique = m_siNPCCharUnique;
	
	CObj *pTemp = m_pForeignMerchantMgr->PopFrontToEmptyListAndPushBackToExistList();

	if ( pTemp )
	{
		pTemp->SetData( ( char * )&sSellToForeignMerchant, sSellToForeignMerchant.sHeader.uiSize );
	}

	return;
}

bool CForeignMerchantDlg::Pop( stSellToForeignMerchant *pSellToForeignMerchant )
{
	CObj *pTemp = m_pForeignMerchantMgr->PopFrontToExistListAndPushBackToEmptyList();

	if ( pTemp )
	{
		UI32 uiDataSize;
		pTemp->GetData( (char*)pSellToForeignMerchant, &uiDataSize );
		return true;
	}

	return false;
}

