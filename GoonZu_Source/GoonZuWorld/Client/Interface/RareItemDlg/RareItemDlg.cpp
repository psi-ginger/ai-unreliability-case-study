/*
#include "RareItemDlg.h"

#include "../../CommonLogic/MsgType-System.h"
#include "../../StatusBar/ChatStatusBar/ChatStatusBarBaseInfo.h"
#include "../../../lib/WebHTML/WebHTML.h"

#include "../../common/HintDlg/HintDlg.h"
#include "Char\CharManager\CharManager.h"

extern cltCommonLogic* pclClient;

//CRareItemMsgDlg		g_RareItemMsg;

CRareItemMsgDlg::CRareItemMsgDlg()
{
	m_hRareItemMsgDlg	= NULL;
	m_hEditBrush		= NULL;

	strcpy( m_strPlayerName,"");

	m_hPortraitBitmap = NULL ;

	m_siHintDlg = 0 ;
	m_hHitButton = NULL ;
}

CRareItemMsgDlg::~CRareItemMsgDlg()
{
	if ( m_hRareItemMsgDlg )
	{
		DestroyWindow(m_hRareItemMsgDlg);
	}
}

void CRareItemMsgDlg::Create( HINSTANCE hInst, HWND hParent )
{
	m_hWnd = hParent ;
	m_hInst = hInst ;
}

void CRareItemMsgDlg::CreateDlg()
{
	if ( !m_hRareItemMsgDlg )
		m_hRareItemMsgDlg = CreateDialog( m_hInst, MAKEINTRESOURCE( IDD_DIALOG_RAREITEM ), m_hWnd, StaticRareItemMsgDlgProc );

	Hide();
}

BOOL CALLBACK CRareItemMsgDlg::StaticRareItemMsgDlgProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	cltClient *pclclient = (cltClient*)pclClient;

	return pclclient->m_pRareItemMsgDlg->RareItemMsgDlgProc( hDlg, iMsg, wParam, lParam );
}

BOOL CALLBACK CRareItemMsgDlg::RareItemMsgDlgProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam )
{
	HWND hwnd = NULL;
	cltClient *pclclient = (cltClient*)pclClient;

	switch( iMsg )
	{
	case WM_INITDIALOG:
		{
			m_hEditBrush	= CreateSolidBrush(COLOR_DIALOG_YELLOW);
			m_hRareItemEdit = GetDlgItem( hDlg, IDC_EDIT_RAREITEM );		
			
			m_hImageWnd = GetDlgItem( hDlg, IDC_LIST_RAREITME_IMAGE );

			cltClient *pclclient = (cltClient *)pclClient;
			pclclient->pclImageList->ConnectToListView( m_hImageWnd, LVSIL_NORMAL );

			//---------------------------------
			// HTML 을 초기화한다.
			//---------------------------------
			//==================================		
			hwnd = GetDlgItem(hDlg, IDC_STATIC_PROFILE);
			if ( hwnd )
			{
				EmbedBrowserObject(hwnd);
			}

			MoveWindow( GetDlgItem( hDlg, IDC_STATIC_PROFILE ), 4, 4, 73, 73, TRUE );

		}
		break;
	case WM_CTLCOLORSTATIC:
		{
			if ( GetDlgItem( hDlg, IDC_EDIT_RAREITEM ) == (HWND)lParam )
			{
				HDC hdc = (HDC)wParam;
				SetTextColor( hdc, COLOR_DIALOG_BLACK );
				SetBkColor( hdc, COLOR_DIALOG_YELLOW );
								
				return (BOOL)m_hEditBrush;
			}
		}
		break;

	case WM_SIZE:
		{
			//============================
			// HTML Resize 를 한다.
			//============================
			//==================================
			HWND hwnd = GetDlgItem(hDlg, IDC_STATIC_PROFILE);
			if( hwnd )
			{
				ResizeBrowser(hwnd, LOWORD(lParam), HIWORD(lParam));
			}
			return FALSE;
		}
		break;
	case WM_COMMAND:
		{
			switch( wParam )
			{
			case IDC_BUTTON_HINTDLG:
				{
					char buffer[256] = "" ;
					switch(m_siHintDlg)
					{
					case HITBUTTON_RECOMMEND:
						{
							strcpy(buffer,"recommend");
						}
						break;
					case HITBUTTON_SUMMON:
						{
							strcpy(buffer,"summonbreed");
						}
						break;
					case HITBUTTON_RAREITEM:
						{
							strcpy(buffer,"goodproduct");
						}
					}

					if ( m_siHintDlg > 0 )
						pclclient->m_pHintDlg->Show(buffer);
				}
				break;
			case IDC_BUTTON_WHISPERCHAT:
				{
					cltClient *pclclient = (cltClient*)pclClient;

					if ( strlen (m_strPlayerName) > 0 && m_strPlayerName != NULL )
					{
						char *Name = (char*)pclClient->pclCM->CR[1]->GetName();

						// 자기 자신에게는 귓속말을 보내지 못한다.
						if ( Name != NULL && !strcmp( m_strPlayerName, Name ) )
						{
							break;
						}

						pclclient->RequestSecretChatByCharName(m_strPlayerName);
					}
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
			if ( m_hEditBrush )
			{
				DeleteObject( m_hEditBrush );
				m_hEditBrush = NULL;
			}
			if ( m_hPortraitBitmap )
			{
				DeleteObject(m_hPortraitBitmap);
				m_hPortraitBitmap = NULL ;
			}

			//---------------------------------
			// HTML 을 해제한다.
			//---------------------------------
			//==================================

			hwnd = GetDlgItem( hDlg, IDC_STATIC_PROFILE );
			if( hwnd )
			{
				UnEmbedBrowserObject(hwnd);
			}

			m_hRareItemMsgDlg = NULL;
		}
		break;
	}


	return FALSE;
}

void CRareItemMsgDlg::Set( cltItem *pclItem, char *PlayerName, char *strTitleData, char *strPrintData,
						   SI32 siHorse,SI32 siSummon,SI32 siHintButton )
{
	cltClient *pclclient = (cltClient *)pclClient;
	
	if( pclclient->bWatchModeSwitch )
	{
		return;
	}

	if ( !IsWindow(m_hRareItemMsgDlg) || m_hRareItemMsgDlg == NULL )
	{
		m_hRareItemMsgDlg = NULL ;
		m_hRareItemMsgDlg = CreateDialog( m_hInst, MAKEINTRESOURCE( IDD_DIALOG_RAREITEM ), m_hWnd, StaticRareItemMsgDlgProc );
	}

//	va_list ap;
	memset( m_strPrintMsg, 0, 2048 );
	memset( m_strTitleMsg, 0, 64 );

	strcpy( m_strTitleMsg, strTitleData );
	strcpy( m_strPrintMsg, strPrintData ) ;

//	va_start( ap, strPrintData );
//	_vsnprintf( m_strPrintMsg, 256, strPrintData, ap );
//	va_end( ap );

	SetWindowText( m_hRareItemMsgDlg, strTitleData );
	SetWindowText( m_hRareItemEdit, m_strPrintMsg );

	if ( PlayerName != NULL && strlen( PlayerName ) > 0 )
	{
		strcpy( m_strPlayerName, PlayerName );
		strcat( m_strPlayerName, "" );
	}
	else
	{
		memset ( m_strPlayerName, 0, MAX_PLAYER_NAME );
	}

	char *Name = (char*)pclClient->pclCM->CR[1]->GetName();
	if ( Name )
	{
		if ( !strcmp( m_strPlayerName, Name ) )
		{
			EnableWindow(GetDlgItem(m_hRareItemMsgDlg,IDC_BUTTON_WHISPERCHAT),FALSE);
		}
		else
		{
			EnableWindow(GetDlgItem(m_hRareItemMsgDlg,IDC_BUTTON_WHISPERCHAT),TRUE);
		}
	}

	ListviewDelete(m_hRareItemMsgDlg,IDC_LIST_RAREITME_IMAGE);

	if ( pclItem != NULL && siHorse == 0 && siSummon == 0 )
	{
		SetInventoryInListView( pclClient->pclItemManager, m_hRareItemMsgDlg, IDC_LIST_RAREITME_IMAGE, NULL, pclItem, 5, 5 );
		MoveWindow(GetDlgItem(m_hRareItemMsgDlg,IDC_STATIC_HORSE_PORTRAIT),0,0,0,0,TRUE);
		MoveWindow(GetDlgItem(m_hRareItemMsgDlg,IDC_LIST_RAREITME_IMAGE),6,107,68,66,TRUE);
	}
	else if ( siHorse != 0 && siSummon == 0 && pclItem == NULL )
	{
		char strFileName[ 128 ] = "";
		char strPathFileName[ 128 ] = "";
		switch(siHorse)
		{
		case 1:	strcpy(strFileName, "BROWN_HORSE");	break;
		case 2:	strcpy(strFileName, "BLACK_HORSE"); break;
		case 3:	strcpy(strFileName, "WHITE_HORSE"); break;
		case 4:	strcpy(strFileName, "RED_HORSE");	break;
		case 5:	strcpy(strFileName, "ZEBRA_HORSE");	break;
		case 6: strcpy(strFileName, "RUDOLF");	break;
		case 7: strcpy(strFileName,	"THOUSAND_HORSE"); break;
		default:strcpy(strFileName, "BROWN_HORSE");	break;
		}

		sprintf( strPathFileName, "Interface/Portrait/%s.bmp",	strFileName );
		m_hPortraitBitmap = (HBITMAP)LoadImage( pclClient->GetInstance(), strPathFileName, IMAGE_BITMAP, 80, 80, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE );

		if ( m_hPortraitBitmap != NULL )
		{
			SendDlgItemMessage( m_hRareItemMsgDlg, IDC_STATIC_HORSE_PORTRAIT, STM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)m_hPortraitBitmap );
		}
		MoveWindow(GetDlgItem(m_hRareItemMsgDlg,IDC_LIST_RAREITME_IMAGE),0,0,0,0,TRUE);
		MoveWindow(GetDlgItem(m_hRareItemMsgDlg,IDC_STATIC_HORSE_PORTRAIT),0,94,80,80,TRUE);
	}
	else if ( siSummon > 0 && siSummon < MAX_KINDINFO_NUMBER && siHorse == 0 && pclItem == NULL )
	{
		char strFileName[ 128 ] = "";

		sprintf(strFileName, "Interface/Portrait/%s.bmp", pclClient->pclKindInfoSet->pclKI[siSummon]->szCode);
		m_hPortraitBitmap = (HBITMAP)LoadImage( pclClient->GetInstance(), strFileName, IMAGE_BITMAP, 80, 80, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE );

		if ( m_hPortraitBitmap != NULL )
		{
			SendDlgItemMessage( m_hRareItemMsgDlg, IDC_STATIC_HORSE_PORTRAIT, STM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)m_hPortraitBitmap );
		}
		MoveWindow(GetDlgItem(m_hRareItemMsgDlg,IDC_LIST_RAREITME_IMAGE),0,0,0,0,TRUE);
		MoveWindow(GetDlgItem(m_hRareItemMsgDlg,IDC_STATIC_HORSE_PORTRAIT),0,94,80,80,TRUE);
	}
	else return ;

	m_siHintDlg = siHintButton ;
	if ( m_hHitButton )
	{
		DeleteObject(m_hHitButton);
		m_hHitButton = NULL ;
	}
	switch(m_siHintDlg)
	{
	case HITBUTTON_RECOMMEND:
		{
			m_hHitButton = (HBITMAP)LoadImage( pclClient->GetInstance(), "interface/HitButton/recommend.bmp", IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE );
			if ( m_hHitButton != NULL )
			{
				SendDlgItemMessage( m_hRareItemMsgDlg, IDC_BUTTON_HINTDLG, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)m_hHitButton);
			}

			EnableWindow(GetDlgItem(m_hRareItemMsgDlg,IDC_BUTTON_HINTDLG),TRUE);
			ShowWindow(GetDlgItem(m_hRareItemMsgDlg,IDC_BUTTON_HINTDLG),SW_SHOW);
		}
		break;
	case HITBUTTON_SUMMON:
		{
			m_hHitButton = (HBITMAP)LoadImage( pclClient->GetInstance(), "interface/HitButton/summonshint.bmp", IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE );
			if ( m_hHitButton != NULL )
			{
				SendDlgItemMessage( m_hRareItemMsgDlg, IDC_BUTTON_HINTDLG, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)m_hHitButton);
			}

			EnableWindow(GetDlgItem(m_hRareItemMsgDlg,IDC_BUTTON_HINTDLG),TRUE);
			ShowWindow(GetDlgItem(m_hRareItemMsgDlg,IDC_BUTTON_HINTDLG),SW_SHOW);
		}
		break;
	case HITBUTTON_RAREITEM:
		{
			m_hHitButton = (HBITMAP)LoadImage( pclClient->GetInstance(), "interface/HitButton/rareitem.bmp", IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE );
			if ( m_hHitButton != NULL )
			{
				SendDlgItemMessage( m_hRareItemMsgDlg, IDC_BUTTON_HINTDLG, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)m_hHitButton);
			}

			EnableWindow(GetDlgItem(m_hRareItemMsgDlg,IDC_BUTTON_HINTDLG),TRUE);
			ShowWindow(GetDlgItem(m_hRareItemMsgDlg,IDC_BUTTON_HINTDLG),SW_SHOW);
		}
		break;
	default:
		{
			EnableWindow(GetDlgItem(m_hRareItemMsgDlg,IDC_BUTTON_HINTDLG),FALSE);
			ShowWindow(GetDlgItem(m_hRareItemMsgDlg,IDC_BUTTON_HINTDLG),SW_HIDE);
		}
		break;
	}

	Show();

	return;
}

void CRareItemMsgDlg::Show()
{
	ShowWindow( m_hRareItemMsgDlg, SW_SHOW );

	HWND hWnd = NULL ;

	hWnd = GetDlgItem(m_hRareItemMsgDlg, IDC_STATIC_PROFILENAME);
	if(hWnd)	SetWindowText(hWnd, m_strPlayerName);

	hWnd = GetDlgItem(m_hRareItemMsgDlg,IDC_STATIC_PROFILE);
	if(hWnd)
	{
		char path[512] = "" ;
		GetCurrentDirectory( 256, path );
		int BufLen = strlen( path );
		if( path [ BufLen - 1 ] == '\\' )
		{
			strcat(path,"Interface\\EventNotice\\rareitem.html");
		}
		else
		{				
			strcat(path,"\\Interface\\EventNotice\\rareitem.html");
		}
		DisplayHTMLPage( hWnd, path );
	}
}

void CRareItemMsgDlg::Hide()
{
	ShowWindow( m_hRareItemMsgDlg, SW_HIDE );
}
*/