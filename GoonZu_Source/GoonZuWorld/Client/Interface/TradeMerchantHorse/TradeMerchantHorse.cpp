#include "TradeMerchantHorse.h"

#include "../../Client/client.h"

#include "../../CommonLogic/MsgType.h"
#include "../../CommonLogic/CommonLogic.h"
#include "../../CommonLogic/MsgType-System.h"
#include "../../CommonLogic/Msg/MsgType-Village.h"
#include "../../CommonLogic/Msg/MsgType-TradeMerchant.h"
#include "..\..\Client\Interface\ErrorMsgDlg\ErrorMsgDlg.h"

//-----------------------------
// Common
//-----------------------------
#include "PersonInfo.h"
#include "Char\CharManager\CharManager.h"

extern cltCommonLogic* pclClient;

CTradeMerchantHorse::CTradeMerchantHorse()
{
	m_hDlg = NULL;
	m_bShow = false;
	m_siNPCKind = 0;
	m_hEditBrush = NULL;
	m_hPortraitBitmap = NULL;
	m_hHorsePortraitBitmap = NULL;

	m_siHorsePrice = 0;
}

CTradeMerchantHorse::~CTradeMerchantHorse()
{
	if ( m_hDlg )
	{
		DestroyWindow( m_hDlg );
	}
}

void CTradeMerchantHorse::Create( HINSTANCE hInst, HWND hWnd )
{
	m_hDlg = CreateDialog( hInst, MAKEINTRESOURCE( IDD_DIALOG_TRADEMERCHANT_HORSE ), hWnd,  StaticTradeMerchantHorseDlgProc );
	Hide();
	return;
}

void CTradeMerchantHorse::LoadHorse()
{
	//==========================================
	// 초상화 
	//==========================================
	// 초상화용 핸들을 삭제한다.
	if(m_hHorsePortraitBitmap)
	{
		DeleteObject(m_hHorsePortraitBitmap);
		m_hHorsePortraitBitmap = NULL;
	}
	
	cltHorse* pclhorse = pclClient->pclCM->CR[1]->pclCI->clHorseInfo.GetCurrentShowHorse();
	
	if( pclhorse->siHorseUnique )
	{
		char strFileName[ 128 ] = "";
		char strPathFileName[ 128 ] = "";
		switch( pclhorse->siHorseUnique )
		{
		case 1:	strcpy(strFileName, "BROWN_HORSE");	break;
		case 2:	strcpy(strFileName, "BLACK_HORSE"); break;
		case 3:	strcpy(strFileName, "WHITE_HORSE"); break;
		case 4:	strcpy(strFileName, "RED_HORSE");	break;
		case 5: strcpy(strFileName, "ZEBRA_HORSE"); break;
		case 6: strcpy(strFileName, "RUDOLF"); break;
		case 7: strcpy(strFileName,	"THOUSAND_HORSE"); break;
		case 8: strcpy(strFileName, "IRON_HORSE"); break;
		case 9: strcpy(strFileName, "TIGER"); break;
		default:strcpy(strFileName, "BROWN_HORSE");	break;
		}
		sprintf( strPathFileName, "Interface/Portrait/%s.bmp",	strFileName );
		m_hHorsePortraitBitmap = (HBITMAP)LoadImage( pclClient->GetInstance(), strPathFileName, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE );
	}
	if ( m_hHorsePortraitBitmap != NULL )
	{
		SendDlgItemMessage( m_hDlg, IDC_STATIC_HORSEIMAGE, STM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)m_hHorsePortraitBitmap );
	}

	char Buffer[ 512 ] = "";
	if ( pclClient->pclHorseManager->GetText( pclhorse, Buffer ) )
	{
		SetWindowText( GetDlgItem( m_hDlg, IDC_EDIT_HORSE_EXPLAIN ), Buffer );
	}
	
	return;
}

BOOL CALLBACK CTradeMerchantHorse::StaticTradeMerchantHorseDlgProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam )
{
	cltClient *pclclient = ( cltClient * )pclClient;
	return pclclient->m_pTradeMerchantHorse->TradeMerchantHorseDlgProc( hDlg, iMsg, wParam, lParam );
}

BOOL CALLBACK CTradeMerchantHorse::TradeMerchantHorseDlgProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam )
{
	switch ( iMsg )
	{
	case WM_INITDIALOG:
		{
			m_hDlg = hDlg;

			// EditBrush 
			m_hEditBrush = CreateSolidBrush( COLOR_DIALOG_WHITEBLUE );
			
			m_hPortraitBitmap = (HBITMAP)LoadImage( pclClient->GetInstance(), "Interface/Portrait/KIND_MERCHANTGUN.BMP", IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE );
			
			if ( m_hPortraitBitmap != NULL )
			{
				SendDlgItemMessage( m_hDlg, IDC_STATIC_TRADEMERCHANT_IMAGE, STM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)m_hPortraitBitmap );
			}			
		}
		return TRUE;
	case WM_CTLCOLORSTATIC:
		{
			if ( GetDlgItem( hDlg, IDC_EDIT_EXPLAIN ) == (HWND)lParam ||
				GetDlgItem( hDlg, IDC_EDIT_HORSE_EXPLAIN ) == (HWND)lParam
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
			case IDC_BUTTON_SELLHORSE:
				{
					if ( m_siHorsePrice < 1 )
					{
						return FALSE;
					}

					cltHorse* pclhorse = pclClient->pclCM->CR[1]->pclCI->clHorseInfo.GetCurrentShowHorse();

					if ( pclhorse->siHorseUnique < 1 )
					{
						return FALSE;
					}
					
					if ( pclClient->pclCM->CR[1]->pclCI->clHorseInfo.siCurrentHorseRIDEHORSE == true )
					{
						char* pText = GetTxtFromMgr(2088);
						SetWindowText( GetDlgItem( hDlg, IDC_EDIT_EXPLAIN ), pText );
						return FALSE;
					}

					if ( pclClient->pclCM->CR[1]->pclCI->clCharItem.IsHorseHaveItem() )
					{
						char* pText = GetTxtFromMgr(2107);
						SetWindowText( GetDlgItem( hDlg, IDC_EDIT_EXPLAIN ), pText );
						return FALSE;
					}
					
					char buffer[256 ] = "";
					char* pText = GetTxtFromMgr(2110);
					sprintf( buffer, pText, m_siHorsePrice );
					pText = GetTxtFromMgr(2113);
					if( MessageBox(NULL, buffer, pText, MB_ICONQUESTION | MB_YESNO) != IDYES ) break;

					cltGameMsgRequest_TradeMerchantBuyHorseFromPerson clGameMsgRequest_TradeMerchantBuyHorseFromPerson( KIND_MERCHANTHORSE );
					cltMsg clMsg( GAMEMSG_REQUEST_TRADEMERCHANTBUYHORSEFROMPERSON, sizeof(clGameMsgRequest_TradeMerchantBuyHorseFromPerson), (char*)&clGameMsgRequest_TradeMerchantBuyHorseFromPerson );
					pclClient->SendMsgToServer( (char*) &clMsg );
				}
				break;
			}
		}
		break;
	case WM_DESTROY:
		{
			if ( m_hHorsePortraitBitmap )
			{
				DeleteObject( m_hHorsePortraitBitmap );
				m_hHorsePortraitBitmap = NULL;
			}

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

bool CTradeMerchantHorse::IsShow()
{
	return m_bShow;
}

void CTradeMerchantHorse::Show( SI32 NPCCharID )
{
	if ( m_bShow )
	{
		Hide();
		return;
	}

	m_siNPCKind = pclClient->pclCM->CR[ NPCCharID ]->GetKind();

	m_bShow = true;	
	ShowWindow( m_hDlg, SW_SHOW );	

	if ( ! pclClient->pclCM->CR[1]->HaveHorse() )
	{
		char* pText =GetTxtFromMgr(2114);
		SetWindowText( GetDlgItem( m_hDlg, IDC_EDIT_EXPLAIN ), pText );
		return;
	}

	cltClient *pclclient = (cltClient *)pclClient;
	SI32 datevary = pclclient->clClientDate.GetDateVary();

	char* pText =GetTxtFromMgr(2115);
	SetWindowText( GetDlgItem( m_hDlg, IDC_EDIT_EXPLAIN ), pText );

	cltGameMsgRequest_TradeMerchantHorseBuyInfo clGameMsgRequest_TradeMerchantHorseBuyInfo( KIND_MERCHANTHORSE );
	cltMsg clMsg( GAMEMSG_REQUEST_TRADEMERCHANTHORSEBUYINFO, sizeof(clGameMsgRequest_TradeMerchantHorseBuyInfo), (char*)&clGameMsgRequest_TradeMerchantHorseBuyInfo );
	pclClient->SendMsgToServer( (char*) &clMsg );

	LoadHorse();	
	
	return;
}

void CTradeMerchantHorse::Hide()
{
	m_bShow = false;
	
	ShowWindow( m_hDlg, SW_HIDE );
	
	return;
}

void CTradeMerchantHorse::SetHorsePrice( SI16 HorseUnique, SI32 HorsePrice )
{
	char *HorseKindName = pclClient->pclHorseManager->GetHorseName( HorseUnique );
	char Buffer[256 ] = "";
	char* pText =GetTxtFromMgr(2116);
	sprintf( Buffer, pText, HorseKindName, HorsePrice );

	m_siHorsePrice = HorsePrice;
	SetWindowText( GetDlgItem( m_hDlg, IDC_EDIT_EXPLAIN ), Buffer );
}

void CTradeMerchantHorse::SellToNPCSuccess()
{
	char* pText =GetTxtFromMgr(2117);
	SetWindowText( GetDlgItem( m_hDlg, IDC_EDIT_EXPLAIN ), pText);
	
	// 말이 팔렸음으로, 말 정보 초기화
	m_siHorsePrice = 0;

	SetWindowText( GetDlgItem( m_hDlg, IDC_EDIT_HORSE_EXPLAIN ), "" );

	if(m_hHorsePortraitBitmap)
	{
		DeleteObject(m_hHorsePortraitBitmap);
		m_hHorsePortraitBitmap = NULL;
	}


	return;
}