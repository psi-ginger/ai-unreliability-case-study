#include "NPCTradeDlg.h"
#include "NPCTradeBuffer.h"
#include "../../FontManager/FontManager.h"
#include "../../Client/Music/Music.h"
#include "../../Client/Client.h"

#include "Char\CharManager\CharManager.h"


#include "../../../CommonLogic/Msg/MsgType-NPC.h"
extern cltCommonLogic* pclClient;

CNPCTradeDlg::CNPCTradeDlg()
{
	m_bShowFlag = false;
	
	m_hDlg = NULL;

	m_hPortraitBitmap = NULL;

	m_siUserInventorySelectedIndex = -1;
	m_siNPCInventorySelectedIndex = -1;

	m_siSellItemNum = 0;
	m_siBuyItemNum = 0;
	
	m_siNPCCharUnique = 0;
	m_siNPCKind = 0;

	m_pNPCTradeBuffer = new CNPCTradeBuffer();
}

CNPCTradeDlg::~CNPCTradeDlg()
{
	if ( m_pNPCTradeBuffer )
	{
		delete m_pNPCTradeBuffer;
		m_pNPCTradeBuffer = NULL;
	}

	if ( m_hDlg )
	{
		DestroyWindow( m_hDlg );
	}
}

void CNPCTradeDlg::Create( HINSTANCE hInst, HWND hWnd, cltPItemInfo *pPersonItemInfo, cltNPCItemInfo *pNPCItemInfo )
{
	m_pPersonItemInfo = pPersonItemInfo;
	m_pNPCItemInfo = pNPCItemInfo;

	m_hDlg = ::CreateDialog( hInst, MAKEINTRESOURCE( IDD_DIALOG_NPCTRADE1 ), hWnd, StaticNPCTradeDlgProc );


	/*
	ListViewInsertColumn( m_hUserInventory, 48, "Item1", 0 );
	ListViewInsertColumn( m_hUserInventory, 48, "Item2", 1 );
	ListViewInsertColumn( m_hUserInventory, 48, "Item3", 2 );
	ListViewInsertColumn( m_hUserInventory, 48, "Item4", 3 );
	ListViewInsertColumn( m_hUserInventory, 48, "Item5", 4 );
	ListViewInsertColumn( m_hUserInventory, 48, "Item6", 5 );
	ListViewInsertColumn( m_hUserInventory, 48, "Item7", 6 );
	*/
	
	ShowWindow( m_hDlg, SW_HIDE );
	m_bShowFlag = false;
	LoadBuyCashItemInfo();
}

BOOL CALLBACK CNPCTradeDlg::StaticNPCTradeDlgProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam )
{
	cltClient *pclclient = (cltClient *)pclClient;
	return pclclient->m_pNPCTradeDlg->NPCTradeDlgProc( hDlg, iMsg, wParam, lParam );
}

BOOL CALLBACK CNPCTradeDlg::NPCTradeDlgProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam )
{
	cltClient *pclclient = (cltClient*)pclClient;
	switch( iMsg )
	{
	case WM_INITDIALOG:
		{
			m_hDlg = hDlg;
			

			//m_hFont = CreateFont( 11, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, 0, "굴림" );

			m_hUserInventory = GetDlgItem( hDlg, IDC_LIST_USER_INVENTORY );
			m_hNPCInventory = GetDlgItem( hDlg,  IDC_LIST_NPC_INVENTORY );
			m_hNPCTradeInfoViewEdit = GetDlgItem( hDlg, IDC_EDIT_TRADE_INFO );

			g_FontManager.SetFont( hDlg, IDC_LIST_USER_INVENTORY );
			g_FontManager.SetFont( hDlg, IDC_LIST_NPC_INVENTORY );
			//g_FontManager.SetFont( hDlg, IDC_EDIT_TRADE_INFO );

			

			/*
			LVBKIMAGE	UserInventoryBackGroudImage;
			UserInventoryBackGroudImage.ulFlags = LVBKIF_SOURCE_URL |	LVBKIF_STYLE_TILE;
			UserInventoryBackGroudImage.pszIma = 
			*/
			

			//ListView_SetBkImage( m_hUserInventory, 
						
			ListView_SetExtendedListViewStyle( m_hNPCInventory, LVS_EX_FULLROWSELECT | LVS_EX_ONECLICKACTIVATE | LVS_EX_UNDERLINEHOT | LVS_EX_INFOTIP );
			ListView_SetExtendedListViewStyle( m_hUserInventory, LVS_EX_FULLROWSELECT | LVS_EX_ONECLICKACTIVATE | LVS_EX_UNDERLINEHOT | LVS_EX_INFOTIP );
			
			//SetWindowText( m_hNPCTradeInfoViewEdit, "궁궐에서만 구할 수 있는 품질 좋은 물건들이 많아요. 정말로~" );
			
			cltClient *pclclient = (cltClient *)pclClient;
			pclclient->pclImageList->ConnectToListView( m_hUserInventory, LVSIL_NORMAL );	
			pclclient->pclImageList->ConnectToListView( m_hNPCInventory, LVSIL_NORMAL );
			
			LoadUserInventory();
			
			m_hPortraitBitmap = (HBITMAP)LoadImage( pclClient->GetInstance(), "Interface/Portrait/Portrait_NPC1.bmp", IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE );
			
			if ( m_hPortraitBitmap != NULL )
			{
				SendDlgItemMessage( hDlg, IDC_STATIC_NPC1, STM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)m_hPortraitBitmap);				
			}

			return TRUE;
		}
		break;
	case WM_TIMER:
		{			
			if ( IsUserInventoryChanged() )
			{
				LoadUserInventory();
			}
			
			if ( IsNPCInventoryChanged() )
			{
				LoadNPCInventory();
			}
		}
		break;
	case WM_NOTIFY:
		{
			LPNMHDR					hdr;
			
			LPNMITEMACTIVATE		nia;
			LPNMLVGETINFOTIP		ngt;

			LPNMLISTVIEW			nlv;
			
			hdr = ( LPNMHDR )		lParam;
			ngt = ( LPNMLVGETINFOTIP ) lParam;
			nlv = ( LPNMLISTVIEW )	lParam;
			nia = (LPNMITEMACTIVATE)lParam;
			
			if ( hdr->hwndFrom == m_hUserInventory )
			{
				switch( hdr->code )
				{
				case LVN_ITEMCHANGED:
					{
						SI32 m_siInvSelectedIndex = 0;
						char szPrice[128]=" ";

						if ( nia->iItem >= 0 )
						{
							m_siInvSelectedIndex = nlv->lParam;
						}
						SI32 ItemType = pclClient->pclItemManager->GetItemType(m_clUserItem[ m_siInvSelectedIndex].siUnique);
						if(IsCheckCashItem(m_clUserItem[ m_siInvSelectedIndex].siUnique)==true)
						{							
							char* pText = GetTxtFromMgr(5588);
							sprintf(szPrice,pText);
							SetWindowText( m_hNPCTradeInfoViewEdit, szPrice );
							m_siUserInventorySelectedIndex=0;
							
						}
						
						else if ( nia->iItem >= 0 )
						{
							m_siUserInventorySelectedIndex = nlv->lParam;
							SetDlgItemInt( hDlg, IDC_EDIT_ITEM_NUM, m_clUserItem[ m_siUserInventorySelectedIndex ].GetItemNum(), FALSE );
							char szPrice[128] = "";


							char* pText = GetTxtFromMgr(1169);
							
							sprintf( szPrice, pText, pclClient->pclItemManager->pclItemPrice->GetPriceSellToNPC(pclClient->pclItemManager,  m_clUserItem[ m_siUserInventorySelectedIndex ].siUnique ) );
							SetWindowText( m_hNPCTradeInfoViewEdit, szPrice );
							break;
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
								if ( m_clUserItem[ i ].siUnique != 0 )
								{
									if ( ngt->iItem == siListViewIndex )
									{
										pclClient->pclItemManager->GetExplain( &m_clUserItem[ i ], ngt->pszText );
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
			else if ( hdr->hwndFrom == m_hNPCInventory )
			{
				switch( hdr->code )
				{
				case LVN_ITEMCHANGED:
					{
						 if ( nia->iItem >= 0 )
						{
							m_siNPCInventorySelectedIndex = nlv->lParam;
							char szPrice[128] = "";

							SetDlgItemInt( hDlg, IDC_EDIT_ITEM_NUM, m_clNPCItem[ m_siNPCInventorySelectedIndex ].GetItemNum(), FALSE );
							SI32 si32Price = m_siNPCItemPrice[ m_siNPCInventorySelectedIndex ];
							
							
							char buffer[256] ="";
							g_JYLibFunction.SetNumUnit( si32Price, buffer, "냥" );
							
							char* pText = GetTxtFromMgr(1170);
							

							sprintf( szPrice, pText, buffer );
							
							SetWindowText( m_hNPCTradeInfoViewEdit, szPrice );
							break;
						}
					}
					break;
				case LVN_GETINFOTIP:
					{
						if ( ngt->iItem >= 0 )
						{
							SI32 siListViewIndex = 0;

							for ( int i = 0; i < MAX_NPC_ITEM_NUMBER; ++i )
							{
								if ( m_clNPCItem[ i ].siUnique != 0 )
								{
									if ( ngt->iItem == siListViewIndex )
									{
										pclClient->pclItemManager->GetExplain( &m_clNPCItem[ i ], ngt->pszText, false );
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
		}
		break;
	case WM_COMMAND:
		{
			switch ( LOWORD(wParam) )
			{
			case IDC_BUTTON_SELLITEM:
				{
					BOOL bSuccess;

					m_siSellItemNum = GetDlgItemInt( hDlg, IDC_EDIT_ITEM_NUM, &bSuccess, FALSE );
					
					if ( bSuccess == FALSE )
					{
						break;
					}

					if ( m_siSellItemNum < 1 )
					{
						break;
					}

					SellItemToNPC();
					
				}
				break;
			case IDC_BUTTON_BUYITEM:
				{
					BOOL bSuccess;
					m_siBuyItemNum = GetDlgItemInt( hDlg, IDC_EDIT_ITEM_NUM, &bSuccess, FALSE );
									
					if ( bSuccess == FALSE )
					{
						break;
					}

					if ( m_siBuyItemNum < 1 )
					{
						break;
					}

					BuyItemFromNPC();
					
				}
				break;
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
			KillTimer( hDlg, 0);

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

bool CNPCTradeDlg::IsUserInventoryChanged()
{
	for ( int j = PERSONITEM_INV0; j < PERSONITEM_SUMMON1EQUIP0; ++j )
	{
		SI32 size = sizeof(cltItem);

		char* src = (char*)&m_clUserItem[ j ];
		char* dst = (char*)&m_pPersonItemInfo->clItem[ j ];

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

bool CNPCTradeDlg::IsNPCInventoryChanged()
{
	cltNPCMerchantInfo *pNPCMerchantInfo = NULL;
	for ( SI16 k = 0; k < MAX_NPC_MERCHANT_NUMBER; ++k )
	{
		if ( m_pNPCItemInfo->clNPCMerchantInfo[ k ].siNPCKind == m_siNPCKind )
		{
			pNPCMerchantInfo = &m_pNPCItemInfo->clNPCMerchantInfo[ k ];
			break;
		}
	}

	if ( pNPCMerchantInfo == NULL )
	{
		return false;
	}

	for ( int j = 0; j < MAX_NPC_ITEM_NUMBER; ++j )
	{
		SI32 size = sizeof(cltItem);

		char* src = (char*)&m_clNPCItem[ j ];
		char* dst = (char*)&pNPCMerchantInfo->clItemInfo[ j ].clItem;

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

void CNPCTradeDlg::LoadUserInventory()
{	
	if ( m_hDlg == NULL )
	{
		return;
	}

	m_siUserInventorySelectedIndex = -1;

	

	for ( int i = PERSONITEM_INV0; i < PERSONITEM_SUMMON1EQUIP0; ++i )
	{
		m_clUserItem[ i ] = m_pPersonItemInfo->clItem[ i ];
		m_siUserItemPrice[ i ] = pclClient->pclItemManager->pclItemPrice->GetPriceSellToNPC(pclClient->pclItemManager,  m_clUserItem[ i ].siUnique );
	}
	
	SetInventoryInListView( pclClient->pclItemManager, m_hDlg, IDC_LIST_USER_INVENTORY, NULL, m_pPersonItemInfo );
	
	return;
}

void CNPCTradeDlg::LoadNPCInventory()
{
	if ( m_hDlg == NULL )
	{
		return;
	}

	m_siNPCInventorySelectedIndex = -1;

	cltNPCMerchantInfo *pNPCMerchantInfo = NULL;
	for ( SI16 k = 0; k < MAX_NPC_MERCHANT_NUMBER; ++k )
	{
		if ( m_pNPCItemInfo->clNPCMerchantInfo[ k ].siNPCKind == m_siNPCKind )
		{
			pNPCMerchantInfo = &m_pNPCItemInfo->clNPCMerchantInfo[ k ];
			break;
		}
	}

	if ( pNPCMerchantInfo == NULL )
	{
		return;
	}
	
	for ( int i = 0; i < MAX_NPC_ITEM_NUMBER; ++i )
	{		
		m_clNPCItem[ i ] = pNPCMerchantInfo->clItemInfo[i].clItem;
		m_siNPCItemPrice[ i ] = pNPCMerchantInfo->clItemInfo[i].siPrice;
	}
	
	cltClient *pclclient = (cltClient *)pclClient;
	SetInventoryInListView( pclClient->pclItemManager, m_hDlg, IDC_LIST_NPC_INVENTORY, pclclient->pclImageList, m_clNPCItem, MAX_NPC_ITEM_NUMBER, false );
	
	return;
}

// 유저가 NPC 에게 아이템을 판매
void CNPCTradeDlg::SellItemToNPC()
{
	if ( m_siUserInventorySelectedIndex < 0 )
	{
		return;
	}

	m_clUserItem[ m_siUserInventorySelectedIndex ].SetItemNum(m_siSellItemNum);
	m_pNPCTradeBuffer->Push( NPCTRADE_SELLITEM_TO_NPC, m_siUserInventorySelectedIndex, m_siNPCCharUnique, m_clUserItem[ m_siUserInventorySelectedIndex ] );		//여기서 i는 유저의 아이템 위치임

	return;
}

// NPC 로 부터 유저가 아이템 구입
void CNPCTradeDlg::BuyItemFromNPC()
{
	if ( m_siNPCInventorySelectedIndex < 0 )
	{
		return;
	}

	if ( m_clNPCItem[ m_siNPCInventorySelectedIndex ].CanPile( pclClient->pclItemManager ) )
	{
		m_clNPCItem[ m_siNPCInventorySelectedIndex ].SetItemNum(m_siBuyItemNum);					
	}
	else
	{
		m_clNPCItem[ m_siNPCInventorySelectedIndex ].SetItemNum( 1 );
	}
	if ( m_siNPCCharUnique > 0 )
	{
		m_pNPCTradeBuffer->Push( NPCTRADE_BUYITEM_FROM_NPC, m_siNPCInventorySelectedIndex, m_siNPCCharUnique, m_clNPCItem[ m_siNPCInventorySelectedIndex ] );		//여기서 i는 NPC의 아이템 위치임 
	}

	return;
}

void CNPCTradeDlg::Show(SI32 charunique)
{
	m_siNPCCharUnique = charunique;

	SI32 CharID = pclClient->pclCM->GetIDFromCharUnique( charunique );
	m_siNPCKind = pclClient->pclCM->CR[ CharID ]->GetKind();

	if ( m_hPortraitBitmap )
	{
		DeleteObject( m_hPortraitBitmap );
		m_hPortraitBitmap = NULL;
	}

	switch ( m_siNPCKind )
	{
	case KIND_WAITINGWOMAN:			// 무수리
		{
			m_hPortraitBitmap = (HBITMAP)LoadImage( pclClient->GetInstance(), "Interface/Portrait/Portrait_NPC1.bmp", IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE );
			
			if ( m_hPortraitBitmap != NULL )
			{
				SendDlgItemMessage( m_hDlg, IDC_STATIC_NPC1, STM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)m_hPortraitBitmap);				
			}
			
			char* pText = GetTxtFromMgr(1171);
			
			SetWindowText( m_hDlg, pText );

			
			pText = GetTxtFromMgr(1172);
			
			
			SetWindowText( m_hNPCTradeInfoViewEdit, pText );
		}
		break;
	case KIND_INVESTMENT:
		{
			m_hPortraitBitmap = (HBITMAP)LoadImage( pclClient->GetInstance(), "Interface/Portrait/KIND_INVESTMENT.bmp", IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE );
			
			if ( m_hPortraitBitmap != NULL )
			{
				SendDlgItemMessage( m_hDlg, IDC_STATIC_NPC1, STM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)m_hPortraitBitmap);				
			}
			
			char* pText = GetTxtFromMgr(1173);
			
			SetWindowText( m_hDlg, pText );

			pText = GetTxtFromMgr(1174);
			
			SetWindowText( m_hNPCTradeInfoViewEdit, pText );
		}
		break;
	case KIND_RUSSIATRADER:
		{
			m_hPortraitBitmap = (HBITMAP)LoadImage( pclClient->GetInstance(), "Interface/Portrait/KIND_RUSSIATRADER.bmp", IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE );
			
			if ( m_hPortraitBitmap != NULL )
			{
				SendDlgItemMessage( m_hDlg, IDC_STATIC_NPC1, STM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)m_hPortraitBitmap);
			}
			
			char* pText = GetTxtFromMgr(1175);
			
			SetWindowText( m_hDlg, pText);

			pText = GetTxtFromMgr(1176);
			
			SetWindowText( m_hNPCTradeInfoViewEdit, pText );
		}
		break;
	default:
		{

		}
		break;
	}

	cltGameMsgRequest_NPCSellItemInfo clGameMsgRequest_NPCSellItemInfo( true, m_siNPCKind );
	cltMsg clMsg( GAMEMSG_REQUEST_NPCSELLITEMINFO, sizeof(clGameMsgRequest_NPCSellItemInfo), (char*)&clGameMsgRequest_NPCSellItemInfo );
	pclClient->SendMsgToServer((char*)&clMsg);

	SetTimer( m_hDlg, 0, NPCTRADEINVENTORY_SETTIMER_DEALY_TIME, NULL );	
	

	
	m_bShowFlag = true;
	ShowWindow( m_hDlg, SW_SHOW );
}

void CNPCTradeDlg::Hide()
{
	KillTimer( m_hDlg, 0 );

	m_siNPCCharUnique = 0;
	m_siNPCKind = 0;
	m_bShowFlag = false;

	ShowWindow( m_hDlg, SW_HIDE );
}

bool CNPCTradeDlg::IsShow()
{
	return m_bShowFlag;
}

bool CNPCTradeDlg::Pop( stNPCTradeData * pNPCTradeData )
{
	return m_pNPCTradeBuffer->Pop( pNPCTradeData );
}

void CNPCTradeDlg::SetText( char *strText )
{
	SetWindowText( m_hNPCTradeInfoViewEdit, strText  );
}

void CNPCTradeDlg::SetText( SI32 siTextIndex )
{
	switch ( m_siNPCKind )
	{
	case KIND_WAITINGWOMAN:			// 무수리
		{
			switch( siTextIndex )
			{
			case SYSTEMNPC_SUCCESS_BUY:
				{
					cltClient *pclclient = (cltClient*)pclClient;
					pclclient->pclMusic->PushEffect( EFFECT_SUCCESS, 0, 0 );


					char* pText = GetTxtFromMgr(1177);
					

					SetWindowText( m_hNPCTradeInfoViewEdit, pText );
				}
				break;
			case SYSTEMNPC_FAIL_BUY_REASPON_MONEY_INSUFFICIENTY:
				{
					cltClient *pclclient = (cltClient*)pclClient;
					pclclient->pclMusic->PushEffect( EFFECT_FAILURE, 0, 0 );

					char* pText = GetTxtFromMgr(1178);
					

					SetWindowText( m_hNPCTradeInfoViewEdit, pText );
				}
				break;
			case SYSTEMNPC_SUCCESS_SELL:
				{
					cltClient *pclclient = (cltClient*)pclClient;
					pclclient->pclMusic->PushEffect( EFFECT_SUCCESS, 0, 0 );

					char* pText = GetTxtFromMgr(1179);
					

					SetWindowText( m_hNPCTradeInfoViewEdit, pText );
				}
				break;
			}
		}
		break;
	case KIND_INVESTMENT:			// 봉이 김선달
		{
			switch( siTextIndex )
			{
			case SYSTEMNPC_SUCCESS_BUY:
				{
					cltClient *pclclient = (cltClient*)pclClient;
					pclclient->pclMusic->PushEffect( EFFECT_SUCCESS, 0, 0 );

					char* pText = GetTxtFromMgr(1180);
					

					SetWindowText( m_hNPCTradeInfoViewEdit, pText );
				}
				break;
			case SYSTEMNPC_FAIL_BUY_REASPON_MONEY_INSUFFICIENTY:
				{
					cltClient *pclclient = (cltClient*)pclClient;
					pclclient->pclMusic->PushEffect( EFFECT_FAILURE, 0, 0 );

					char* pText = GetTxtFromMgr(1181);
					

					SetWindowText( m_hNPCTradeInfoViewEdit, pText );
				}
				break;
			case SYSTEMNPC_SUCCESS_SELL:
				{
					cltClient *pclclient = (cltClient*)pclClient;

					char* pText = GetTxtFromMgr(1182);
					

					pclclient->pclMusic->PushEffect( EFFECT_SUCCESS, 0, 0 );
					SetWindowText( m_hNPCTradeInfoViewEdit, pText );
				}
				break;
			}
		}
		break;
	case KIND_RUSSIATRADER:			// 러시아 교역상인 ( 야린 스키 )
		{
			switch( siTextIndex )
			{
			case SYSTEMNPC_SUCCESS_BUY:
				{
					cltClient *pclclient = (cltClient*)pclClient;
					pclclient->pclMusic->PushEffect( EFFECT_SUCCESS, 0, 0 );

					char* pText = GetTxtFromMgr(1183);
					

					SetWindowText( m_hNPCTradeInfoViewEdit, pText );
				}
				break;
			case SYSTEMNPC_FAIL_BUY_REASPON_MONEY_INSUFFICIENTY:
				{
					cltClient *pclclient = (cltClient*)pclClient;
					pclclient->pclMusic->PushEffect( EFFECT_FAILURE, 0, 0 );

					char* pText = GetTxtFromMgr(1184);
					

					SetWindowText( m_hNPCTradeInfoViewEdit, pText );
				}
				break;
			case SYSTEMNPC_SUCCESS_SELL:
				{
					cltClient *pclclient = (cltClient*)pclClient;
					pclclient->pclMusic->PushEffect( EFFECT_SUCCESS, 0, 0 );

					char* pText = GetTxtFromMgr(1185);
					

					SetWindowText( m_hNPCTradeInfoViewEdit, pText );
				}
				break;
			}
		}
		break;
	default:
		{
		}
		break;
	}

	return;
}




bool CNPCTradeDlg::LoadBuyCashItemInfo()
{
	memset( m_siItemUniqueList, 0, sizeof( SI16 ) * MAX_AVAILABLE_CASHITEMNUM );
	memset( m_uiItemPriceList, 0, sizeof( UI16 ) * MAX_AVAILABLE_CASHITEMNUM );
	memset( m_strItemTypeList, 0, MAX_ITEMTYPE_NUMBER * 64 );

	FILE *fp = NULL;

	if( pclClient->siServiceArea == ConstServiceArea_Korea || pclClient->siServiceArea == ConstServiceArea_Netmarble )
		fp = fopen( "Item/Data/CashItemBuyInfo.txt", "rt" );
	else if( pclClient->siServiceArea == ConstServiceArea_English )
		fp = fopen( "Item/Data/CashItemBuyInfoE.txt", "rt" );
	else if( pclClient->siServiceArea == ConstServiceArea_China )
		fp = fopen( "Item/Data/CashItemBuyInfoC.txt", "rt" );
	else if( pclClient->siServiceArea == ConstServiceArea_Taiwan )
		fp = fopen( "Item/Data/CashItemBuyInfoT.txt", "rt" );
	else if( pclClient->siServiceArea == ConstServiceArea_USA )
		fp = fopen( "Item/Data/CashItemBuyInfoU.txt", "rt" );


	if ( fp == NULL )
	{
		return false;
	}

	char buffer[1024];

	for( SI16 i = 0;i < 6; ++i )
		fgets( buffer, 1024, fp );


	SI16 Counter = 0;
	SI16 ItemTypeCounter = 0;

	SI32 ItemUnique = 0;
	char ItemName[ 64 ] = "";
	char ItemType[ 64 ] = "";
	SI32 ItemPrice = 0;


	while( fgets(buffer, 1024, fp) != NULL)
	{
		if( pclClient->siServiceArea != ConstServiceArea_Korea && pclClient->siServiceArea != ConstServiceArea_Netmarble)
		{
			ParseText( buffer );

			ItemUnique = atoi( tokens[0] );
			MStrCpy( ItemName, tokens[1], 64 );
			MStrCpy( ItemType, tokens[2], 64 );
			ItemPrice = atoi( tokens[3] );
		}
		else
		{
			sscanf( buffer, "%d %s %s %d", &ItemUnique, ItemName, ItemType, &ItemPrice );
		}

		//	sscanf( buffer, "%d %s %s %d", &ItemUnique, ItemName, ItemType, &ItemPrice );

		m_siItemUniqueList[ Counter ] = ItemUnique;
		m_uiItemPriceList[ Counter ] = ItemPrice;

		if ( ! IsSameItemTypeExist( ItemType ) )
		{
			strcpy ( m_strItemTypeList[ ItemTypeCounter++ ], ItemType );
		}

		if ( ++Counter >= MAX_AVAILABLE_CASHITEMNUM )
		{
			break;
		}
	}

	fclose( fp );

	return true;
}


bool CNPCTradeDlg::IsSameItemTypeExist( char *ItemType )
{
	for ( SI16 i = 0; i < MAX_ITEMTYPE_NUMBER; ++i )
	{
		if ( strlen( m_strItemTypeList[ i ] ) > 0 )
		{
			if ( strcmp( m_strItemTypeList[ i ], ItemType ) == 0 || strcmp( m_strItemTypeList[ i ], "생생탕" ) == 0)
			{
				return true;
			}
		}
		else
		{
			break;
		}
	}

	return false;
}

bool CNPCTradeDlg::IsCheckCashItem(SI32 Uinque)
{
	for (SI32 i = 0; i <= MAX_AVAILABLE_CASHITEMNUM; i++)
	{
		if( m_siItemUniqueList[i]  ==Uinque)
			return true;
	}
	return false;
}

