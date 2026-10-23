// ByungzoDlg.cpp: implementation of the CByungzoDlg class.
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

#include "ByungzoDlg.h"

//-----------------------------
// Common
//-----------------------------
#include "PersonInfo.h"
#include "Char\CharManager\CharManager.h"

extern cltCommonLogic* pclClient;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CByungzoDlg::CByungzoDlg()
{
	m_hDlg = NULL ;
	m_hGroup = NULL ;
	m_bRectSwitch = FALSE ;
	m_siWarTime = 0;
	m_bSet = FALSE ;
	m_hEditBrush = NULL;
	m_hDlgBrush  = NULL;

	strcpy(m_szByungzoName,"");
	strcpy(m_szByungzoProfile,"");
	strcpy(m_szByungzoID,"");
}

CByungzoDlg::~CByungzoDlg()
{
}

void CByungzoDlg::Create(HINSTANCE hInst, HWND hParent )
{
	m_hInst = hInst ;
	m_hWnd = hParent ;
}

BOOL CALLBACK CByungzoDlg::StatiCByungzoDlgProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	cltClient *pclclient = (cltClient*)pclClient;
	return pclclient->m_pByungzoDlg->ByungzoDlgProc( hDlg, iMsg, wParam, lParam );
}

BOOL CALLBACK CByungzoDlg::ByungzoDlgProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam )
{
	cltClient *pclclient = (cltClient*)pclClient;

	switch( iMsg )
	{
	case WM_INITDIALOG:
		{
			m_hEditBrush = CreateSolidBrush( COLOR_DIALOG_YELLOW );
			m_hDlgBrush  = CreateSolidBrush( COLOR_DIALOG_LIGHTBLUE);

			if ( pclclient->pclCM->CR[1]->pclCI->clBI.uiGRank == GRANK_MINISTER4 )
				SetTimer(hDlg, 1, 1000, NULL);
			else
				SetTimer(hDlg, 2, 600000, NULL);

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
			MoveWindow( GetDlgItem( hDlg, IDC_STATIC_PROFILE ), 9, 122, 73, 73, TRUE );

			hwnd = GetDlgItem(hDlg, IDC_STATIC_BYUNGZOINTRO);
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
					GetDlgItemText(m_hDlg,IDC_EDIT_BYUNGZO,buffer,256);
					SI32 limit = strlen(buffer);

					char* pText = GetTxtFromMgr(2108);
					wsprintf(buffer, pText,m_szByungzoName,limit);
					SetWindowText(m_hGroup,buffer);
				}
				break;
			case 2:
				{
					cltMsg clMsg(GAMEMSG_REQUEST_GETBYUNGZOINFO, 0, NULL);
					pclclient->SendMsgToServer((char*)&clMsg);
				}
				break;
			}
		}
		break;
	case WM_CTLCOLORSTATIC:
		{
			if ( GetDlgItem( hDlg, IDC_EDIT_BYUNGZO ) == (HWND)lParam )
			{
				HDC hdc = (HDC)wParam;
				SetTextColor( hdc, COLOR_DIALOG_BLACK);
				SetBkColor( hdc, COLOR_DIALOG_YELLOW );
								
				return (BOOL)m_hEditBrush;
			}
		}
		break;
/*	case WM_CTLCOLORDLG:
		{
			HDC hdc = (HDC)wParam;
			return (BOOL)m_hDlgBrush;
		}
		break;*/
	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case IDC_RADIO_TWENTY:
			m_siWarTime = 20 ;
			break;
		case IDC_RADIO_TWENTYFIVE:
			m_siWarTime = 25 ;
			break;
		case IDC_RADIO_THIRTY:
			m_siWarTime = 30 ;
			break;
		case IDOK:
			{
				/*cltGameMsgRequest_SetVillageWarTime clinfo(m_siWarTime);
				cltMsg clMsg(GAMEMSG_REQUEST_SETVILLAGEWARTIME, sizeof(clinfo), (char*)&clinfo);
				pclclient->SendMsgToServer((char*)&clMsg);*/
				GetDlgItemText(m_hDlg,IDC_EDIT_BYUNGZO,m_szByungzoProfile,256);
				SI32 limit = strlen(m_szByungzoProfile);
				if ( limit > 0 && limit < 256 )
				{
					cltGameMsgRequest_SetByungzoInfo clinfo(m_szByungzoProfile,m_siWarTime);
					cltMsg clMsg(GAMEMSG_REQUEST_SETBYUNGZOINFO, sizeof(clinfo), (char*)&clinfo);
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
				cltGameMsgRequest_Userid clUserid( m_szByungzoName );
				cltMsg clMsg( GAMEMSG_REQUEST_USERID, sizeof( clUserid ), (char*)&clUserid );
				pclClient->SendMsgToServer((char*)&clMsg);
			}
			break;
		case IDWHISPER:
			{
				if ( strlen(m_szByungzoName) > 0 && m_szByungzoName != NULL )
					pclclient->RequestSecretChatByCharName(m_szByungzoName);
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
			hwnd = GetDlgItem( hDlg, IDC_STATIC_BYUNGZOINTRO );
			if( hwnd )
			{
				DoPageAction(hwnd, WEBPAGE_STOP);
				UnEmbedBrowserObject(hwnd);
			}

			GetWindowRect(m_hDlg, &m_rtDlg);
			m_bRectSwitch = TRUE ;
			m_siWarTime = 0 ;

			if ( pclclient->pclCM->CR[1]->pclCI->clBI.uiGRank == GRANK_MINISTER4 )
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

void CByungzoDlg::Set()
{
	if ( !IsWindow(m_hDlg) )
	{
		m_hDlg = CreateDialog( m_hInst, MAKEINTRESOURCE( IDD_DIALOG_BYUNGZO ), m_hWnd, StatiCByungzoDlgProc );
		SendDlgItemMessage( m_hDlg, IDC_EDIT_BYUNGZO, EM_LIMITTEXT, 255, 0 );
		m_hGroup = GetDlgItem(m_hDlg,IDC_STATIC_BYUNGZOPROFILE);

		if ( m_bRectSwitch )
		{
			MoveWindow(m_hDlg, 
				m_rtDlg.left, m_rtDlg.top, 
				m_rtDlg.right - m_rtDlg.left, 
				m_rtDlg.bottom - m_rtDlg.top, FALSE);
		}

		HWND hButton = NULL ;
		cltClient *pclclient = (cltClient*)pclClient;
		if ( pclclient->pclCM->CR[1]->pclCI->clBI.uiGRank != GRANK_MINISTER4 )
		{
			hButton = GetDlgItem(m_hDlg,IDOK);
			EnableWindow(hButton,FALSE);

			hButton = GetDlgItem(m_hDlg,IDC_RADIO_TWENTY);
			EnableWindow(hButton,FALSE);
			hButton = GetDlgItem(m_hDlg,IDC_RADIO_TWENTYFIVE);
			EnableWindow(hButton,FALSE);
			hButton = GetDlgItem(m_hDlg,IDC_RADIO_THIRTY);
			EnableWindow(hButton,FALSE);

			hButton = GetDlgItem(m_hDlg,IDC_EDIT_BYUNGZO);
			EnableWindow(hButton,FALSE);

			//hButton = GetDlgItem(m_hDlg,IDSETUP);
			//EnableWindow(hButton,FALSE);
		}
		else
		{
			hButton = GetDlgItem(m_hDlg,IDGGOM);
			EnableWindow(hButton,FALSE);

			hButton = GetDlgItem(m_hDlg,IDWHISPER);
			EnableWindow(hButton,FALSE);
		}

		//cltMsg clMsg(GAMEMSG_REQUEST_GETVILLAGEWARTIME, 0, NULL);
		//pclclient->SendMsgToServer((char*)&clMsg);
		cltMsg clMsg(GAMEMSG_REQUEST_GETBYUNGZOINFO,0,NULL);
		pclclient->SendMsgToServer((char*)&clMsg);
	}
	else
	{
		DestroyWindow(m_hDlg);
	}
}

void CByungzoDlg::Show(CMinisterBase *pMini,SI32 time )
{
	cltClient *pclclient = (cltClient *)pclClient;
	if ( m_bSet )
	{
		char* pTitle = GetTxtFromMgr(2045);
		char* pText = GetTxtFromMgr(2046);
		pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
		m_bSet = FALSE ;
		return ;
	}

	pclClient->PushEffect(EFFECT_BYUNGZO,0,0);

	strcpy(m_szByungzoID,pMini->m_szAccountID);
	strcpy(m_szByungzoName,pMini->m_szPersonName);
	strcpy(m_szByungzoProfile,pMini->m_szGreeting);

	switch(time)
	{
	case 20 : CheckRadioButton(m_hDlg,IDC_RADIO_TWENTY,IDC_RADIO_THIRTY,IDC_RADIO_TWENTY); break;
	case 25 : CheckRadioButton(m_hDlg,IDC_RADIO_TWENTY,IDC_RADIO_THIRTY,IDC_RADIO_TWENTYFIVE); break;
	case 30 : CheckRadioButton(m_hDlg,IDC_RADIO_TWENTY,IDC_RADIO_THIRTY,IDC_RADIO_THIRTY); break;
	}

	m_siWarTime = time ;

	char buffer[256] = "" ;
	char* pText = GetTxtFromMgr(2109);
	wsprintf(buffer, pText,m_szByungzoName);
	SetWindowText( m_hDlg ,buffer);
	SetDlgItemText( m_hDlg,IDC_EDIT_BYUNGZO, m_szByungzoProfile );
	pText = GetTxtFromMgr(2048);
	strcat(buffer, pText);
	SetWindowText(m_hGroup,buffer);

	HWND hWnd = NULL ;
	char ProfileURL[MAX_PATH];

/*	pclClient->GetCZProfileUrl( ProfileURL, m_szByungzoID );

	hWnd = GetDlgItem( m_hDlg, IDC_STATIC_PROFILE );
	if(hWnd)	DisplayHTMLPage(hWnd, ProfileURL);*/

	// INTRO
	
	pclClient->GetCZIntroUrl( ProfileURL, m_szByungzoID );
	hWnd = GetDlgItem(m_hDlg, IDC_STATIC_BYUNGZOINTRO);
	if(hWnd)	DisplayHTMLPage(hWnd, ProfileURL);

	ShowWindow( m_hDlg, SW_SHOW );
}