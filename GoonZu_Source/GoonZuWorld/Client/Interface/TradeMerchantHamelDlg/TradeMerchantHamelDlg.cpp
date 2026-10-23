/*
#include "TradeMerchantHamelDlg.h"

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

CTradeMerchantHamelDlg::CTradeMerchantHamelDlg()
{
	m_hDlg = NULL;
	m_hEditBrush = NULL;

	m_hPortraitBitmap = NULL;

	m_siShipID = 0;
	m_siNPCKind = 0;

	m_bShow = false;
}

CTradeMerchantHamelDlg::~CTradeMerchantHamelDlg()
{
	if ( m_hDlg )
	{
		DestroyWindow( m_hDlg );
	}
}

void CTradeMerchantHamelDlg::Init()
{
}

void CTradeMerchantHamelDlg::Create( HINSTANCE hInst, HWND hWnd )
{
	m_hDlg = CreateDialog( hInst, MAKEINTRESOURCE( IDD_DIALOG_TRADEMERCHANT_HAMEL ), hWnd,  StaticTradeMerchantHamelDlgProc );
	Hide();
	return;
}

void CTradeMerchantHamelDlg::LoadShipList()
{
	ListView_DeleteAllItems( GetDlgItem( m_hDlg, IDC_LIST_SHIP ) );
	
	m_siShipID = 0;

	LVITEM Li;
	Li.mask = LVIF_TEXT | LVIF_PARAM;
	Li.state = 0;
	Li.stateMask = 0;	
	
	SI16 Counter = 0;
	
	char buffer[ 256 ] = "";
	
	for ( SI16 i = 0; i < MAX_SHIP_PER_PERSON; ++i )
	{
		// 마을내 포구에 있는 경우.
		if ( pclClient->pclCM->CR[1]->pclCI->clShipInfo.clShip[ i ].siShipID > 0 && 
			pclClient->pclCM->CR[1]->pclCI->clShipInfo.clShip[ i ].siShipPos >= ShipPos_Port  && 
			pclClient->pclCM->CR[1]->pclCI->clShipInfo.clShip[ i ].siShipPos <= CUR_VILLAGE_LIMIT
			)
		{
			char *ShipName = pclClient->pclShipManager->GetShipKindName( pclClient->pclCM->CR[1]->pclCI->clShipInfo.clShip[ i ].siKind );
			char *VillageName = pclClient->pclVillageManager->GetName( pclClient->pclCM->CR[1]->pclCI->clShipInfo.clShip[ i ].siShipPos );
			SI16 Ref = pclClient->pclShipManager->GetRefFromShipName( ShipName );

			if ( ShipName != NULL && VillageName != NULL )
			{	
				Li.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;				
				Li.iItem	= Counter;
				Li.iSubItem	= 0;
				
				char* pText = GetTxtFromMgr(2006);
				sprintf(buffer, pText, VillageName, pclClient->pclCM->CR[1]->pclCI->clShipInfo.clShip[ i ].siPortUnitSlot + 1, ShipName );
				Li.pszText	= buffer;
				
				Li.lParam	= pclClient->pclCM->CR[1]->pclCI->clShipInfo.clShip[ i ].siShipID;
				SendMessage( GetDlgItem( m_hDlg, IDC_LIST_SHIP ), LVM_INSERTITEM, 0, (LPARAM)&Li );

               
				OriginalDurability[i] =pclClient->pclShipManager->pclShipBaseInfoList[ Ref ]->m_clShipBaseInfo.siDurability;
				MyDurability[i] = OriginalDurability[i]-(pclClient->pclCM->CR[1]->pclCI->clShipInfo.clShip[ i ].siDecreaseDurability*4);
				TotalPrice[i]= ((MyShipPrice[i]/10000)*MyDurability[i])/OriginalDurability[i];
				
				Li.mask = LVIF_TEXT;
				Li.iSubItem	= 1;
				Li.iItem	= Counter;
				sprintf(buffer, "%d", MyDurability[i]);
				Li.pszText	=buffer;
				//Li.lParam	= pclClient->pclCM->CR[1]->pclCI->clShipInfo.clShip[ i ].siShipID;
				SendMessage( GetDlgItem( m_hDlg, IDC_LIST_SHIP ), LVM_SETITEM, 0, (LPARAM)&Li );



				Li.mask = LVIF_TEXT;
				Li.iSubItem	= 2;
				Li.iItem	= Counter;
				sprintf(buffer, "%d", TotalPrice[i]);
				Li.pszText	=buffer;
				//Li.lParam	= pclClient->pclCM->CR[1]->pclCI->clShipInfo.clShip[ i ].siShipID;
				SendMessage( GetDlgItem( m_hDlg, IDC_LIST_SHIP ), LVM_SETITEM, 0, (LPARAM)&Li );

				
				

				Counter++;
				
			



					
			

			}
		}
	}

	return;
}

BOOL CALLBACK CTradeMerchantHamelDlg::StaticTradeMerchantHamelDlgProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam )
{
	cltClient *pclclient = (cltClient *)pclClient;
	return pclclient->m_pTradeMerchantHamelDlg->TradeMerchantHamelDlgProc( hDlg, iMsg, wParam, lParam );
}

BOOL CALLBACK CTradeMerchantHamelDlg::TradeMerchantHamelDlgProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam )
{
	switch( iMsg )
	{
	case WM_INITDIALOG:
		{
			for(int i=0;i<3;i++)
			{
				TotalPrice[i]=0;
				OriginalDurability[i]=0;
				MyDurability[i]=0;
				MyShipPrice[i]=0;
				MyShipNameKind[i]=0;
			}

			InitCommonControls();

			// EditBrush 
			m_hEditBrush = CreateSolidBrush( COLOR_DIALOG_WHITEBLUE);

			ListView_SetExtendedListViewStyle( GetDlgItem( hDlg, IDC_LIST_SHIP ), LVS_EX_FULLROWSELECT | LVS_EX_ONECLICKACTIVATE | LVS_EX_GRIDLINES | LVS_EX_INFOTIP );
			ListView_SetExtendedListViewStyle( GetDlgItem( hDlg, IDC_LIST_SHIPKIND ), LVS_EX_FULLROWSELECT | LVS_EX_ONECLICKACTIVATE | LVS_EX_GRIDLINES );
			
			m_hDlg = hDlg;

			LVCOLUMN Col;			
			
			Col.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
			Col.fmt = LVCFMT_CENTER;
			
			Col.cx = 150;
			char* pText = GetTxtFromMgr(2007);
			Col.pszText = pText;
			Col.iSubItem = 0;
			SendMessage( GetDlgItem( hDlg, IDC_LIST_SHIP ), LVM_INSERTCOLUMN, 0, (LPARAM)&Col );

			Col.cx = 100;
			pText = GetTxtFromMgr(5154);			
			Col.pszText = pText;
			Col.iSubItem = 1;
			SendMessage( GetDlgItem( hDlg, IDC_LIST_SHIP ), LVM_INSERTCOLUMN, 1, (LPARAM)&Col );


			Col.cx = 100;
			pText = GetTxtFromMgr(5155);			
			Col.pszText = pText;
			Col.iSubItem = 2;
			SendMessage( GetDlgItem( hDlg, IDC_LIST_SHIP ), LVM_INSERTCOLUMN, 2, (LPARAM)&Col );



			
            
			m_hPortraitBitmap = (HBITMAP)LoadImage( pclClient->GetInstance(), "Interface/Portrait/KIND_HAMEL.BMP", IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE );
			
			if ( m_hPortraitBitmap != NULL )
			{
				SendDlgItemMessage( m_hDlg, IDC_STATIC_PORTRAIT_HAMEL, STM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)m_hPortraitBitmap );
			}
			
			//---------------------------------
			// HTML 을 초기화한다.
			//---------------------------------
			//==================================			
			HWND hwnd = GetDlgItem(hDlg, IDC_STATIC_SHIPIMAGE );
			if ( hwnd )
			{
				EmbedBrowserObject(hwnd);
			}
			//==================================

			MoveWindow( GetDlgItem( hDlg, IDC_STATIC_SHIPIMAGE ), 1, 85, 120, 120, TRUE );

			Col.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
			Col.fmt = LVCFMT_CENTER;
			
			Col.cx = 120;
			pText = GetTxtFromMgr(2008);
			Col.pszText = pText;
			Col.iSubItem = 0;
			SendMessage( GetDlgItem( hDlg, IDC_LIST_SHIPKIND ), LVM_INSERTCOLUMN, 0, (LPARAM)&Col );

			Col.cx = 95;
			pText = GetTxtFromMgr(2009);
			Col.pszText = pText;
			Col.iSubItem = 1;
			SendMessage( GetDlgItem( hDlg, IDC_LIST_SHIPKIND ), LVM_INSERTCOLUMN, 1, (LPARAM)&Col );
		}
		return TRUE;

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
			
			if ( hdr->hwndFrom == GetDlgItem ( hDlg, IDC_LIST_SHIP ) )
			{
				switch( hdr->code )
				{
				case LVN_ITEMCHANGED:
					{
						if( nlv->uChanged == LVIF_STATE && nlv->uNewState == ( LVIS_SELECTED | LVIS_FOCUSED ) )
						{
							m_siShipID = nlv->lParam;
							
							SI16 ShipRef = pclClient->pclCM->CR[1]->pclCI->clShipInfo.GetShipDataIndex( m_siShipID );
							if ( ShipRef < 0 )
							{
								return FALSE;
							}							
						}
					}
					break;
				}
			}


			if ( hdr->hwndFrom == GetDlgItem ( hDlg, IDC_LIST_SHIPKIND ) )
			{
				switch( hdr->code )
				{
				case LVN_ITEMCHANGED:
					{
						if( nlv->uChanged == LVIF_STATE && nlv->uNewState == ( LVIS_SELECTED | LVIS_FOCUSED ) )
						{
							if ( m_siShipKindList[ nlv->lParam ] > 0 )
							{
								char Buffer[ 256 ];
								char strImagePath[ _MAX_PATH ] = "";
								GetCurrentDirectory( _MAX_PATH, strImagePath );
								
								int BufLen = strlen( strImagePath );
								
								SI32 ShipKind = m_siShipKindList[ nlv->lParam ];
								
								if( strImagePath [ BufLen - 1 ] == '\\' )
								{
									sprintf( Buffer, "Interface\\ShipPortrait\\%d.html", ShipKind );
								}
								else
								{				
									sprintf( Buffer, "\\Interface\\ShipPortrait\\%d.html", ShipKind );
								}

								strcat( strImagePath, Buffer );
								
								HWND hwnd = GetDlgItem(hDlg, IDC_STATIC_SHIPIMAGE );
								
								DisplayHTMLPage( hwnd, strImagePath );
							}
						}
					}
					break;
				}
			}
		}
		break;

	case WM_TIMER:
		{
			LoadShipList();
		}
		break;

	case WM_CTLCOLORSTATIC:
		{
			if ( GetDlgItem( hDlg, IDC_EDIT_EXPLAIN ) == (HWND)lParam )
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
			case IDC_BUTTON_SHIPSELL:
				{	
					LPNMHDR						hdr;			
					LPNMITEMACTIVATE			nia;
					LPNMLVGETINFOTIP			ngt;
					LPNMLISTVIEW				nlv;
					hdr = ( LPNMHDR )			lParam;
					ngt = ( LPNMLVGETINFOTIP )	lParam;
					nlv = ( LPNMLISTVIEW )		lParam;
					nia = (LPNMITEMACTIVATE)	lParam;

					if ( m_siShipID > 0 && m_siNPCKind > 0 )
					{
						cltShip *pShip = pclClient->pclCM->CR[1]->pclCI->clShipInfo.GetShipDataPtr( m_siShipID );

						if ( pShip == NULL )
						{
							return FALSE;
						}

						SI16 i = 0;
						for ( ; i < MAX_SHIP_KIND_NUMBER; ++i )
						{
							if ( pShip->siKind == m_siShipKindList[ i ] )
							{
								break;
							}
						}

						if ( i == MAX_SHIP_KIND_NUMBER )
						{
							return FALSE;
						}

						if ( m_siShipPriceList[ i ] < 1 )
						{
							return FALSE;
						}

						char buffer[256 ] = "";
						//int MyIndex=  nlv->lParam;
						int MyIndex= pclClient->pclCM->CR[1]->pclCI->clShipInfo.GetShipDataIndex( m_siShipID );
						char* pText = GetTxtFromMgr(2010);
						sprintf( buffer, pText,  TotalPrice[MyIndex]*10000 );
						//sprintf( buffer, pText, m_siShipPriceList[ i ] );
						char* pTitle = GetTxtFromMgr(2011);
						if( MessageBox(NULL, buffer, pTitle, MB_ICONQUESTION | MB_YESNO) != IDYES ) break;
						
						cltGameMsgRequest_TradeMerchantBuyShipFromPerson clGameMsgRequest_TradeMerchantBuyShipFromPerson( m_siNPCKind, m_siShipID );
						cltMsg clMsg( GAMEMSG_REQUEST_TRADEMERCHANTBUYSHIPFROMPERSON, sizeof(clGameMsgRequest_TradeMerchantBuyShipFromPerson), (char*)&clGameMsgRequest_TradeMerchantBuyShipFromPerson );
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

			//---------------------------------
			// HTML 을 해제한다.
			//---------------------------------
			//==================================
			HWND hwnd = GetDlgItem( hDlg, IDC_STATIC_SHIPIMAGE );
			if( hwnd )
			{
				UnEmbedBrowserObject(hwnd);
			}

			m_hDlg = NULL;
		}
		break;
	}
	return FALSE;
}

bool CTradeMerchantHamelDlg::IsShow()
{
	return m_bShow;
}

void CTradeMerchantHamelDlg::Show( SI32 NPCCharID )
{
	if ( m_bShow )
	{
		Hide();
		return;
	}

	m_siNPCKind = pclClient->pclCM->CR[ NPCCharID ]->GetKind();
	m_siShipID = 0;

	LoadShipList();
	char* pText = GetTxtFromMgr(2012);
	SetWindowText( GetDlgItem( m_hDlg, IDC_EDIT_EXPLAIN ), pText );

	cltGameMsgRequest_TradeMerchantShipBuyInfo clGameMsgRequest_TradeMerchantShipBuyInfo( m_siNPCKind );
	cltMsg clMsg( GAMEMSG_REQUEST_TRADEMERCHANTSHIPBUYINFO, sizeof(clGameMsgRequest_TradeMerchantShipBuyInfo), (char*)&clGameMsgRequest_TradeMerchantShipBuyInfo );
	pclClient->SendMsgToServer( (char*) &clMsg );

	m_bShow = true;
	SetTimer( m_hDlg, 0, 5000, NULL );			// 5 초당 1번씩 refresh
	ShowWindow( m_hDlg, SW_SHOW );	

	return;
}

void CTradeMerchantHamelDlg::Hide()
{
	m_bShow = false;
	KillTimer( m_hDlg, 0 );

	ShowWindow( m_hDlg, SW_HIDE );
	
	return;
}

void CTradeMerchantHamelDlg::Set( SI32 *ShipKindList, SI32 *ShipPriceList )
{
	ListView_DeleteAllItems( GetDlgItem( m_hDlg, IDC_LIST_SHIPKIND ) );

	LVITEM Li;	
	Li.state = 0;
	Li.stateMask = 0;
	
	char Buffer[ 1024 ] = "";

	for(SI16 i=0; i<MAX_SHIP_PER_PERSON;i++)
	{
		if(ShipKindList[i])
		{
			for( SI16 j=0; j < MAX_SHIP_KIND_NUMBER; ++ j )
			{
				MyShipNameKind[i]=pclClient->pclCM->CR[1]->pclCI->clShipInfo.clShip[ i ].siKind;
				if(MyShipNameKind[i]==ShipKindList[j])
				{				
					MyShipPrice[i]=ShipPriceList[j];				
					
					j=MAX_SHIP_KIND_NUMBER;
					
				}				
			}//end for
		}
	}//end for


	for( SI16 i = 0; i < MAX_SHIP_KIND_NUMBER; ++ i )
	{
		if ( ShipKindList[ i ] > 0 )
		{
			char *ShipName = pclClient->pclShipManager->GetShipKindName( ShipKindList[ i ] );
			
			if ( ShipName != NULL )
			{
				//Li.mask = LVIF_TEXT | LVIF_PARAM;

				Li.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
				Li.iItem = i;
				Li.iSubItem = 0;
				Li.lParam = i;
				Li.pszText = ShipName;
				
				SendMessage( GetDlgItem( m_hDlg, IDC_LIST_SHIPKIND ), LVM_INSERTITEM, 0, (LPARAM)&Li );
				
				char TempBuffer[256 ] = "";

				Li.mask = LVIF_TEXT;
				Li.iSubItem = 1;
				sprintf( TempBuffer, "%d", ShipPriceList[ i ]/10000 );		// 돈은 만냥 단위
				Li.pszText = TempBuffer;
				SendMessage( GetDlgItem( m_hDlg, IDC_LIST_SHIPKIND ), LVM_SETITEM, 0, (LPARAM)&Li );
				
				m_siShipKindList[ i ] = ShipKindList[ i ];
				m_siShipPriceList[ i ] = ShipPriceList[ i ];
			}
		}
	}
	
	char* pText = GetTxtFromMgr(2013);
	strcat( Buffer, pText );
	SetWindowText( GetDlgItem( m_hDlg, IDC_EDIT_EXPLAIN ), Buffer );
	LoadShipList();

	return;
}

void CTradeMerchantHamelDlg::SellToNPCSuccess( SI16 ShipKind, SI16 VillageUnique, SI16 PortUnitSlot, SI32 ShipPrice )
{
	char* pText = GetTxtFromMgr(2014);
	SetWindowText( GetDlgItem( m_hDlg, IDC_EDIT_EXPLAIN ), pText );
	
	// 정보 Refresh
	m_siShipID = 0;
	LoadShipList();

	return;
}
*/