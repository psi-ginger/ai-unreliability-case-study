#include "TradeMerchantDlg.h"

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
#include "../../Common/SystemNPC/TradeMerchantNPC/TradeMerchantNPC.h"
#include "Char\CharManager\CharManager.h"

extern cltCommonLogic* pclClient;

CTradeMerchantDlg::CTradeMerchantDlg()
{
	m_hDlg = NULL;
	m_hEditBrush = NULL;
	m_bShow = false;
	m_siNPCKind = 0;
	m_hPortraitBitmap = NULL;

	m_siSelectedIndex = -1;

	m_pclItem = new cltItem[ MAX_ITEM_PER_PERSON ];

	m_pTradeMerchantNPC = new CTradeMerchantNPC();

}

CTradeMerchantDlg::~CTradeMerchantDlg()
{
	if ( m_pTradeMerchantNPC )
	{
		delete m_pTradeMerchantNPC;
		m_pTradeMerchantNPC = NULL;
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

void CTradeMerchantDlg::Init()
{

}

void CTradeMerchantDlg::Create( HINSTANCE hInst, HWND hWnd )
{
	m_hDlg = CreateDialog( hInst, MAKEINTRESOURCE( IDD_DIALOG_TRADEMERCHANT ), hWnd,  StaticTradeMerchantDlgProc );
	Hide();
	return;
}

bool CTradeMerchantDlg::IsInventoryChanged()
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

void CTradeMerchantDlg::LoadInventoryData()
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

		if ( m_pclItem[ i ].siUnique > 0 && m_pclItem[ i ].siUnique == m_pTradeMerchantNPC->m_siBuyItemUnique )
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
	
	ListView_SetIconSpacing( GetDlgItem( m_hDlg, IDC_LIST_USERINVENTORY ), LISTVIEW_ITEM_WIDTH, LISTVIEW_ITEM_HEIGHT );

	return;
}

BOOL CALLBACK CTradeMerchantDlg::StaticTradeMerchantDlgProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam )
{
	cltClient *pclclient = ( cltClient *)pclClient;
	return pclclient->m_pTradeMerchantDlg->TradeMerchantDlgProc( hDlg, iMsg, wParam, lParam );
}

BOOL CALLBACK CTradeMerchantDlg::TradeMerchantDlgProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam )
{
	switch ( iMsg )
	{
	case WM_INITDIALOG:
		{
			InitCommonControls();

			// EditBrush 
			m_hEditBrush = CreateSolidBrush( COLOR_DIALOG_WHITEBLUE);

			ListView_SetExtendedListViewStyle( GetDlgItem( hDlg, IDC_LIST_USERINVENTORY ), LVS_EX_FULLROWSELECT | LVS_EX_ONECLICKACTIVATE | LVS_EX_INFOTIP );

			m_hDlg = hDlg;

			cltClient *pclclient = (cltClient *)pclClient;
			pclclient->pclImageList->ConnectToListView( GetDlgItem( hDlg, IDC_LIST_NPCBUY_ITEM_IMAGE ), LVSIL_NORMAL );
			pclclient->pclImageList->ConnectToListView( GetDlgItem( hDlg, IDC_LIST_USERINVENTORY ), LVSIL_NORMAL );
		}
		return TRUE;

	case WM_TIMER:
		{
			if ( IsInventoryChanged() )
			{
				LoadInventoryData();
			}
		}
		break;

	case WM_CTLCOLORSTATIC:
		{
			if ( GetDlgItem( hDlg, IDC_EDIT_GETITEM_EXPLAIN ) == (HWND)lParam )
			{
				HDC hdc = (HDC)wParam;
				SetTextColor( hdc, COLOR_DIALOG_BLACK );
				SetBkColor( hdc, COLOR_DIALOG_WHITEBLUE);
				return (BOOL)m_hEditBrush;
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
								if ( m_pclItem[ i ].siUnique != 0 && m_pTradeMerchantNPC->m_siBuyItemUnique == m_pclItem[ i ].siUnique )
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
					if ( m_siSelectedIndex > 0 && m_pTradeMerchantNPC->m_siTradeMerchantNPCKind > 0 && m_pTradeMerchantNPC->m_siBuyItemUnique > 0 )
					{
						cltGameMsgRequest_TradeMerchantBuyItemFromPerson clGameMsgRequest_TradeMerchantBuyItemFromPerson( m_pTradeMerchantNPC->m_siTradeMerchantNPCKind, m_siSelectedIndex, &m_pclItem[ m_siSelectedIndex ] );
						cltMsg clMsg( GAMEMSG_REQUEST_TRADEMERCHANTBUYITEMFROMPERSON, sizeof(clGameMsgRequest_TradeMerchantBuyItemFromPerson), (char*)&clGameMsgRequest_TradeMerchantBuyItemFromPerson );
						pclClient->SendMsgToServer( (char*) &clMsg );
					}
				}
				break;
			}
		}
		break;

	case WM_DESTROY:
		{
			if ( m_hEditBrush )
			{
				DeleteObject( m_hEditBrush );
				m_hEditBrush = NULL;
			}

			if ( m_hPortraitBitmap )
			{
				DeleteObject( m_hPortraitBitmap );
			}

			m_hDlg = NULL;
		}
	}

	return FALSE;
}

bool CTradeMerchantDlg::IsShow()
{
	return m_bShow;
}

void CTradeMerchantDlg::Show( SI32 CharID )
{
	if ( m_bShow )
	{
		Hide();
		return;
	}

	if ( m_hPortraitBitmap )
	{
		DeleteObject(m_hPortraitBitmap);
		m_hPortraitBitmap = NULL ;
	}

	if( m_hPortraitBitmap == NULL )
	{
		char buffer[256] = "" ;
		switch( pclClient->pclCM->CR[CharID]->GetKind() )
		{
		case KIND_CHINATRADER:
			{
				strcpy(buffer,"Interface/Portrait/KIND_CHINATRADER.bmp");
			}
			break;
		case KIND_RUSSIATRADER:
			{
				strcpy(buffer,"Interface/Portrait/KIND_RUSSIATRADER.bmp");
			}
			break;
		case KIND_JAPANTRADER:
			{
				strcpy(buffer,"Interface/Portrait/KIND_JAPANTRADER.bmp");
			}
			break;
		default:
			{
				strcpy(buffer,"Interface/Portrait/KIND_WAITINGMAN.bmp");
			}
			break;
		}

		m_hPortraitBitmap = (HBITMAP)LoadImage( pclClient->GetInstance(),buffer, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE );

		if ( m_hPortraitBitmap != NULL )
		{
			SendDlgItemMessage( m_hDlg, IDC_STATIC_TRADEMERCHANT_IMAGE, STM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)m_hPortraitBitmap);
		}
	}

	// 현재 무역 상인이 구입 중인 정보를 요청
	cltGameMsgRequest_TradeMerchantBuyInfo clGameMsgRequest_TradeMerchantBuyInfo( pclClient->pclCM->CR[CharID]->GetCharUnique(), pclClient->pclCM->CR[CharID]->GetKind() );
	cltMsg clMsg( GAMEMSG_REQUEST_TRADEMERCHANTBUYINFO, sizeof(clGameMsgRequest_TradeMerchantBuyInfo), (char*)&clGameMsgRequest_TradeMerchantBuyInfo );
	pclClient->SendMsgToServer( (char*) &clMsg );

	char* pText = GetTxtFromMgr(2002);
	SetWindowText( GetDlgItem( m_hDlg, IDC_EDIT_GETITEM_EXPLAIN ),
		pText );
	
	m_bShow = true;
	SetTimer( m_hDlg, 0, 1000, NULL );
	ShowWindow( m_hDlg, SW_SHOW );	

	return;
}

void CTradeMerchantDlg::Hide()
{
	m_bShow = false;
	KillTimer( m_hDlg, 0 );

	ShowWindow( m_hDlg, SW_HIDE );

	return;
}

void CTradeMerchantDlg::Set( CTradeMerchantNPC *pTradeMerchantNPC )
{
	m_pTradeMerchantNPC->Set( pTradeMerchantNPC );

	LoadTradeMerchantNPCBuyData();
	LoadInventoryData();

	return;
}

void CTradeMerchantDlg::LoadTradeMerchantNPCBuyData()
{
	if ( m_pTradeMerchantNPC->m_siBuyItemUnique > 0 )
	{
		char *ItemName = (char*)pclClient->pclItemManager->GetName( m_pTradeMerchantNPC->m_siBuyItemUnique );

		if ( ItemName != NULL && strlen ( ItemName ) > 0 )
		{
			if ( m_pTradeMerchantNPC->m_siBuyItemPrice > 0 )
			{
				SI32 tradetax = m_pTradeMerchantNPC->m_siBuyItemPrice/100*m_pTradeMerchantNPC->m_siTradeTax ;
				char Buffer[ 256 ] = "";
				char* pText = GetTxtFromMgr(2003);
				char tax[128]= " " ;
				char taxmoney[128]= " " ;
				char money[128]= " " ;
				char num1[128]= " " ;
				char num2[128]= " " ;
				
				sprintf(tax,"%d",m_pTradeMerchantNPC->m_siTradeTax);
				sprintf(taxmoney,"%d",tradetax);
				sprintf(money,"%d",m_pTradeMerchantNPC->m_siBuyItemPrice-tradetax);
				sprintf(num1,"%d",m_pTradeMerchantNPC->m_siBuyTotalMoney/m_pTradeMerchantNPC->m_siBuyItemPrice);
				sprintf(num2,"%d",MAX_TRADEMERCHATNPC_TOTAL_BUYMONEY/m_pTradeMerchantNPC->m_siBuyItemPrice);
				Nsprintf(Buffer,pText,"item", ItemName   ,"tax",  tax ,"taxmoney", taxmoney   ,"money",  money  ,"num1", num1,"num2",num2,NULL);
			//	sprintf( Buffer, pText,
			//		ItemName,m_pTradeMerchantNPC->m_siTradeTax,tradetax,
			//		m_pTradeMerchantNPC->m_siBuyItemPrice-tradetax, 
			//		m_pTradeMerchantNPC->m_siBuyTotalMoney/m_pTradeMerchantNPC->m_siBuyItemPrice,
			//		MAX_TRADEMERCHATNPC_TOTAL_BUYMONEY/m_pTradeMerchantNPC->m_siBuyItemPrice);
				SetWindowText ( GetDlgItem ( m_hDlg, IDC_EDIT_GETITEM_EXPLAIN ), Buffer );
								
				cltItem clItem;
				clItem.Init();
				clItem.siUnique = m_pTradeMerchantNPC->m_siBuyItemUnique;
				clItem.siItemNum = 1;
				
				SetInventoryInListView( pclClient->pclItemManager, m_hDlg, IDC_LIST_NPCBUY_ITEM_IMAGE, NULL, &clItem, 5, 5 );
			}
			else
			{
				ListView_DeleteAllItems( GetDlgItem( m_hDlg, IDC_LIST_NPCBUY_ITEM_IMAGE ) );
				char* pText = GetTxtFromMgr(2004);
				SetWindowText( GetDlgItem( m_hDlg, IDC_EDIT_GETITEM_EXPLAIN ), pText );
				return;
			}
		}
	}
	else
	{
		ListView_DeleteAllItems( GetDlgItem( m_hDlg, IDC_LIST_NPCBUY_ITEM_IMAGE ) );
		char* pText = GetTxtFromMgr(2004);
		SetWindowText ( GetDlgItem ( m_hDlg, IDC_EDIT_GETITEM_EXPLAIN ), pText );
	}

	return;
}

void CTradeMerchantDlg::SellToNPCSuccess()
{
	char* pText = GetTxtFromMgr(2005);
	SetWindowText ( GetDlgItem ( m_hDlg, IDC_EDIT_GETITEM_EXPLAIN ), pText );
	return;
}
