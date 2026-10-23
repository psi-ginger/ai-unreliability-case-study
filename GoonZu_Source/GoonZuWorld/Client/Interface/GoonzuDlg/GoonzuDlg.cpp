// GoonzuDlg.cpp: implementation of the CGoonzuDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "../../Client/client.h"

#include "../../lib/WebHTML/WebHTML.h"

#include "../../CommonLogic/MsgType.h"
#include "../../CommonLogic/CommonLogic.h"
#include "../../CommonLogic/MsgType-System.h"
#include "../../CommonLogic/MsgType-Person.h"
#include "../../Client/Interface/ErrorMsgDlg/ErrorMsgDlg.h"
#include "../../Client/Interface/NotifyMsgDlg/NotifyMsgDlg.h"

#include "../../Client/Music/Music.h"

#include "GoonzuDlg.h"

//-----------------------------
// Common
//-----------------------------
#include "PersonInfo.h"
#include "Char\CharManager\CharManager.h"

extern cltCommonLogic* pclClient;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGoonzuDlg::CGoonzuDlg()
{
	m_hDlg = NULL ;
	m_hGroup = NULL ;
	m_bRectSwitch = FALSE ;
	m_siResidentNum = 0 ;
	m_bSet = FALSE ;
	m_hEditBrush = NULL ;
	m_hDlgBrush  = NULL;

	strcpy(m_szGoonzuName,"");
	strcpy(m_szGoonzuProfile,"");
	strcpy(m_szGoonzuID,"");

	m_siSelectMinister = 0 ;
}

CGoonzuDlg::~CGoonzuDlg()
{
}

void CGoonzuDlg::Create(HINSTANCE hInst, HWND hParent )
{
	m_hInst = hInst ;
	m_hWnd = hParent ;
}

BOOL CALLBACK CGoonzuDlg::StatiCGoonzuDlgProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	cltClient *pclclient = (cltClient*)pclClient;
	return pclclient->m_pGoonzuDlg->GoonzuDlgProc( hDlg, iMsg, wParam, lParam );
}

BOOL CALLBACK CGoonzuDlg::GoonzuDlgProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam )
{
	cltClient *pclclient = (cltClient*)pclClient;

	switch( iMsg )
	{
	case WM_INITDIALOG:
		{
			m_hEditBrush = CreateSolidBrush( COLOR_DIALOG_YELLOW );
			m_hDlgBrush  = CreateSolidBrush( COLOR_DIALOG_LIGHTBLUE);
			if ( pclclient->pclCM->CR[1]->pclCI->clBI.uiGRank == GRANK_KING )
				SetTimer(hDlg, 1, 1000, NULL);
			else
				SetTimer(hDlg, 2, 60000, NULL);

			//---------------------------------
			// HTML 을 초기화한다.
			//---------------------------------
			//==================================			
			HWND hwnd = NULL ;

			hwnd = GetDlgItem(hDlg, IDC_STATIC_PROFILE);
			if ( hwnd )
			{
				EmbedBrowserObject(hwnd);
			}
			MoveWindow( GetDlgItem( hDlg, IDC_STATIC_PROFILE ), 9, 131, 73, 73, TRUE );

			hwnd = GetDlgItem(hDlg, IDC_STATIC_GOONZUINTRO);
			if ( hwnd )
			{
				EmbedBrowserObject(hwnd);
			}
		}
		break;
	case WM_TIMER:
		{
			switch(wParam)
			{
			case 1:
				{
					char buffer[256] = "" ;
					GetDlgItemText(m_hDlg,IDC_EDIT_GOONZU,buffer,256);
					SI32 limit = strlen(buffer);

					char* pText = GetTxtFromMgr(2111);
					wsprintf(buffer, pText,m_szGoonzuName,limit);
					SetWindowText(m_hGroup,buffer);
				}
				break;
			case 2:
				{
					cltMsg clMsg(GAMEMSG_REQUEST_GETGOONZUINFO,0,NULL);
					pclclient->SendMsgToServer((char*)&clMsg);
				}
				break;
			}
		}
		break;
	case WM_CTLCOLORSTATIC:
		{
			if ( GetDlgItem( hDlg, IDC_EDIT_GOONZU ) == (HWND)lParam )
			{
				HDC hdc = (HDC)wParam;
				SetTextColor( hdc, COLOR_DIALOG_BLACK );
				SetBkColor( hdc, COLOR_DIALOG_YELLOW );
								
				return (BOOL)m_hEditBrush;
			}
		}
		break;
/*	case WM_CTLCOLORBTN:
		{
			HDC hdc = (HDC)wParam;
			SetTextColor( hdc, COLOR_DIALOG_BLACK );
			SetBkColor( hdc, COLOR_DIALOG_LIGHTBLUE );
							
			return (BOOL)m_hEditBrush;
		}
	case WM_CTLCOLORDLG:
		{
			HDC hdc = (HDC)wParam;
			SetTextColor( hdc, COLOR_DIALOG_BLACK);
			SetBkColor( hdc, COLOR_DIALOG_YELLOW);
								
			return (BOOL)m_hDlgBrush;
		}
		break;*/
	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case IDC_RADIO_IZO: m_siSelectMinister = 1 ; break;
		case IDC_RADIO_BYUNGZO: m_siSelectMinister = 2 ; break;
		case IDC_RADIO_HOZO: m_siSelectMinister = 3 ; break;
		case IDC_RADIO_HYUNGZO: m_siSelectMinister = 4 ; break;
		case IDC_RADIO_YEZO: m_siSelectMinister = 5 ; break;
		case IDC_RADIO_GONGZO: m_siSelectMinister = 6 ; break;
		case IDC_BUTTON_SETMINISTER:
			{
				HWND hwnd = GetDlgItem(hDlg,IDC_EDIT_SETMINISTER);
				char charname[MAX_PLAYER_NAME] = "" ;
				GetWindowText(hwnd,charname,MAX_PLAYER_NAME);

				if ( m_siSelectMinister < 1 || m_siSelectMinister > 6 )
					break ;

				if ( strlen(charname) <= 0 || charname[0] == NULL )
					break ;

				char *pText = GetTxtFromMgr(5517);
				char *pTitle = GetTxtFromMgr(5516);

				if (  MessageBox(NULL, pText, pTitle, MB_ICONQUESTION | MB_YESNO | MB_TOPMOST) == IDNO )
					break ;

				cltGameMsgRequest_Goonzu_SetMinister clinfo(m_siSelectMinister,charname,false);
				cltMsg clMsg(GAMEMSG_REQUEST_GOONZU_SETMINISTER,sizeof(clinfo),(char*)&clinfo);
				pclClient->SendMsgToServer((char*)&clMsg);
			}
			break;
		case IDC_BUTTON_LEAVEMINISTER:
			{
				HWND hwnd = GetDlgItem(hDlg,IDC_EDIT_SETMINISTER);
				char charname[MAX_PLAYER_NAME] = "" ;
				GetWindowText(hwnd,charname,MAX_PLAYER_NAME);

				if ( m_siSelectMinister < 1 || m_siSelectMinister > 6 )
					break ;

				if ( strlen(charname) <= 0 || charname[0] == NULL )
					break ;

				char *pText = GetTxtFromMgr(5515);
				char *pTitle = GetTxtFromMgr(5514);

				if (  MessageBox(NULL, pText, pTitle, MB_ICONQUESTION | MB_YESNO | MB_TOPMOST) == IDNO )
					break ;

				cltGameMsgRequest_Goonzu_SetMinister clinfo(m_siSelectMinister,charname,true);
				cltMsg clMsg(GAMEMSG_REQUEST_GOONZU_SETMINISTER,sizeof(clinfo),(char*)&clinfo);
				pclClient->SendMsgToServer((char*)&clMsg);
			}
			break;
		case IDOK:
			{
				GetDlgItemText(m_hDlg,IDC_EDIT_GOONZU,m_szGoonzuProfile,256);
				SI32 limit = strlen(m_szGoonzuProfile);
				if ( limit > 0 && limit < 256 )
				{
					cltGameMsgRequest_SetGoonzuInfo clinfo(m_szGoonzuProfile);
					cltMsg clMsg(GAMEMSG_REQUEST_SETGOONZUINFO, sizeof(clinfo), (char*)&clinfo);
					pclClient->SendMsgToServer((char*)&clMsg);
					m_bSet = TRUE ;
				}
				else
				{
					char* pTitle = GetTxtFromMgr(2019);
					char* pText = GetTxtFromMgr(2024);
					pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);
				}
			}
			break;
		case IDGGOM:
			{
				cltGameMsgRequest_Userid clUserid( m_szGoonzuName );
				cltMsg clMsg( GAMEMSG_REQUEST_USERID, sizeof( clUserid ), (char*)&clUserid );
				pclClient->SendMsgToServer((char*)&clMsg);
			}
			break;
		case IDWHISPER:
			{
				if ( strlen(m_szGoonzuName) > 0 && m_szGoonzuName != NULL )
					pclclient->RequestSecretChatByCharName(m_szGoonzuName);
			}
			break;
		case IDCANCEL:
			{
				DestroyWindow(m_hDlg);
			}
			break;
		}
		break;
	case WM_CLOSE:
		{
			DestroyWindow(m_hDlg);
		}
		break;
	case WM_DESTROY:
		{
			if ( m_hEditBrush )
			{
				DeleteObject( m_hEditBrush );
				m_hEditBrush = NULL;
			}
			if ( m_hDlgBrush )
			{
				DeleteObject( m_hDlgBrush );
				m_hDlgBrush = NULL;
			}

			//---------------------------------
			// HTML 을 해제한다.
			//---------------------------------
			//==================================
			HWND hwnd = NULL ;

			hwnd = GetDlgItem( hDlg, IDC_STATIC_PROFILE );
			if( hwnd )
			{
				DoPageAction(hwnd, WEBPAGE_STOP);
				UnEmbedBrowserObject(hwnd);
			}
			hwnd = GetDlgItem( hDlg, IDC_STATIC_GOONZUINTRO );
			if( hwnd )
			{
				DoPageAction(hwnd, WEBPAGE_STOP);
				UnEmbedBrowserObject(hwnd);
			}

			GetWindowRect(m_hDlg, &m_rtDlg);
			m_bRectSwitch = TRUE ;

			if ( pclclient->pclCM->CR[1]->pclCI->clBI.uiGRank == GRANK_KING )
				KillTimer(hDlg,1);
			else
				KillTimer(hDlg,2);


			m_bSet = FALSE ;
			m_hDlg = NULL ;
		}
		break;
	}

	return FALSE;
}

void CGoonzuDlg::Set()
{
	if ( !IsWindow(m_hDlg) )
	{
		m_hDlg = CreateDialog( m_hInst, MAKEINTRESOURCE( IDD_DIALOG_GOONZU ), m_hWnd, StatiCGoonzuDlgProc );
		SendDlgItemMessage( m_hDlg, IDC_EDIT_GOONZU, EM_LIMITTEXT, 255, 0 );
		m_hGroup = GetDlgItem(m_hDlg,IDC_STATIC_GOONZUPROFILE);

		if ( m_bRectSwitch )
		{
			MoveWindow(m_hDlg, 
				m_rtDlg.left, m_rtDlg.top, 
				m_rtDlg.right - m_rtDlg.left, 
				m_rtDlg.bottom - m_rtDlg.top, FALSE);
		}

		HWND hButton = NULL ;
		cltClient *pclclient = (cltClient*)pclClient;
		if ( pclclient->pclCM->CR[1]->pclCI->clBI.uiGRank != GRANK_KING )
		{
			hButton = GetDlgItem(m_hDlg,IDC_EDIT_GOONZU);
			EnableWindow(hButton,FALSE);

			hButton = GetDlgItem(m_hDlg,IDOK);
			EnableWindow(hButton,FALSE);

			hButton = GetDlgItem(m_hDlg,IDC_RADIO_IZO);
			EnableWindow(hButton,FALSE);
			hButton = GetDlgItem(m_hDlg,IDC_RADIO_BYUNGZO);
			EnableWindow(hButton,FALSE);
			hButton = GetDlgItem(m_hDlg,IDC_RADIO_HOZO);
			EnableWindow(hButton,FALSE);
			hButton = GetDlgItem(m_hDlg,IDC_RADIO_HYUNGZO);
			EnableWindow(hButton,FALSE);
			hButton = GetDlgItem(m_hDlg,IDC_RADIO_YEZO);
			EnableWindow(hButton,FALSE);
			hButton = GetDlgItem(m_hDlg,IDC_RADIO_GONGZO);
			EnableWindow(hButton,FALSE);

			hButton = GetDlgItem(m_hDlg,IDC_EDIT_SETMINISTER);
			EnableWindow(hButton,FALSE);
			hButton = GetDlgItem(m_hDlg,IDC_BUTTON_SETMINISTER);
			EnableWindow(hButton,FALSE);
			hButton = GetDlgItem(m_hDlg,IDC_BUTTON_LEAVEMINISTER);
			EnableWindow(hButton,FALSE);
		}
		else
		{
			hButton = GetDlgItem(m_hDlg,IDGGOM);
			EnableWindow(hButton,FALSE);

			hButton = GetDlgItem(m_hDlg,IDWHISPER);
			EnableWindow(hButton,FALSE);
		}

		cltMsg clMsg(GAMEMSG_REQUEST_GETGOONZUINFO,0,NULL);
		pclclient->SendMsgToServer((char*)&clMsg);
	}
	else
	{
		DestroyWindow(m_hDlg);
	}
}

void CGoonzuDlg::Show(CMinisterBase *pMini,INT64 finances)
{
	cltClient *pclclient = (cltClient *)pclClient;
	if ( m_bSet )
	{
		char* pTitle = GetTxtFromMgr(2045);
		char* pText = GetTxtFromMgr(2046);
		pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);
		m_bSet = FALSE ;
		return ;
	}

	pclClient->PushEffect(EFFECT_MINISTER_GOONZU,0,0);

	strcpy(m_szGoonzuID,pMini->m_szAccountID);
	strcpy(m_szGoonzuName,pMini->m_szPersonName);
	strcpy(m_szGoonzuProfile,pMini->m_szGreeting);

	m_iTotalFinances = finances ;

	char buffer[256] = "";

	char* pText = GetTxtFromMgr(2112);
	wsprintf(buffer, pText,m_szGoonzuName);
	SetWindowText( m_hDlg ,buffer);
	SetDlgItemText( m_hDlg,IDC_EDIT_GOONZU, m_szGoonzuProfile );
	pText = GetTxtFromMgr(2048);
	strcat(buffer, pText);
	SetWindowText(m_hGroup,buffer);

	g_JYLibFunction.SetNumUnit(m_iTotalFinances,buffer);
	SetDlgItemText( m_hDlg,IDC_EDIT_TOTALTAX, buffer );

	HWND hWnd = NULL ;
	char ProfileURL[MAX_PATH];

	//pclClient->GetCZProfileUrl( ProfileURL, m_szGoonzuID );

	//hWnd = GetDlgItem( m_hDlg, IDC_STATIC_PROFILE );
	//if(hWnd)	DisplayHTMLPage(hWnd, ProfileURL);

	// INTRO
	pclClient->GetCZIntroUrl( ProfileURL, m_szGoonzuID );

	hWnd = GetDlgItem(m_hDlg, IDC_STATIC_GOONZUINTRO);
	if(hWnd)	DisplayHTMLPage(hWnd, ProfileURL);

	ShowWindow( m_hDlg, SW_SHOW );
}