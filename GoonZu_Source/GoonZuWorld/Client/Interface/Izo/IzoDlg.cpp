// IzoDlg.cpp: implementation of the CIzoDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "../../Client/client.h"

#include "../../Client/Music/Music.h"

#include "../../lib/WebHTML/WebHTML.h"

#include "../../CommonLogic/MsgType.h"
#include "../../CommonLogic/CommonLogic.h"
#include "../../CommonLogic/MsgType-System.h"
#include "MsgType-Person.h"

#include "../../Client/Interface/ErrorMsgDlg/ErrorMsgDlg.h"
#include "../../Client/Interface/NotifyMsgDlg/NotifyMsgDlg.h"

//-----------------------------
// Common
//-----------------------------
#include "PersonInfo.h"
#include "Char/CharManager/CharManager.h"

extern cltCommonLogic* pclClient;

#include "IzoDlg.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CIzoDlg::CIzoDlg()
{
	m_hDlg = NULL ;
	m_hGroup = NULL ;
	m_bRectSwitch = FALSE ;

	m_siPriceTime = 0 ;
	m_siPriceValue = 0 ;

	m_siTotalMoney = 0 ;
	m_siCalcTime = 0 ;

	m_siShipValue = 0 ;
	m_siTradeTax = 0 ;

	m_bSet = FALSE ;
	m_hEditBrush = NULL ;
	m_hDlgBrush  = NULL;

	strcpy(m_szIzoName,"");
	strcpy(m_szIzoProfile,"");
	strcpy(m_szIzoID,"");

	m_siViewDlg = 0 ;
}

CIzoDlg::~CIzoDlg()
{
}

void CIzoDlg::Create(HINSTANCE hInst, HWND hParent )
{
	m_hInst = hInst ;
	m_hWnd = hParent ;
}

BOOL CALLBACK CIzoDlg::StatiCIzoDlgProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	cltClient *pclclient = (cltClient*)pclClient;
	return pclclient->m_pIzoDlg->IzoDlgProc( hDlg, iMsg, wParam, lParam );
}

BOOL CALLBACK CIzoDlg::IzoDlgProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam )
{
	cltClient *pclclient = (cltClient*)pclClient;

	switch( iMsg )
	{
	case WM_INITDIALOG:
		{
			m_hEditBrush = CreateSolidBrush( COLOR_DIALOG_YELLOW );
			m_hDlgBrush  = CreateSolidBrush( COLOR_DIALOG_LIGHTBLUE);
			if ( pclclient->pclCM->CR[1]->pclCI->clBI.uiGRank == GRANK_MINISTER1 )
				SetTimer(hDlg, 1, 1000, NULL);
			else
				SetTimer(hDlg, 2, 60000, NULL);

			HWND hwnd = NULL ;
			TC_ITEM item;
			RECT rect;

			hwnd = GetDlgItem(hDlg,IDC_TAB_IZO);
			if(hwnd)
			{
				item.mask= TCIF_TEXT;
				item.iImage		= -1;
				char* pText = GetTxtFromMgr(2057);
				item.pszText	= pText;
				TabCtrl_InsertItem(hwnd,0,&item);

				pText = GetTxtFromMgr(2058);
				item.pszText	=	pText;
				TabCtrl_InsertItem(hwnd, 1, &item);

				m_hTabDlg[0] = CreateDialog(m_hInst, MAKEINTRESOURCE(IDD_DIALOG_IZO_PRICEVALUE), hwnd, StatiCIzoPriceValueProc);
				GetClientRect(hwnd, &rect);
				TabCtrl_AdjustRect(hwnd, FALSE, &rect);
				MoveWindow(m_hTabDlg[0], rect.left, rect.top, rect.right-rect.left, rect.bottom- rect.top, TRUE);
				ShowWindow(m_hTabDlg[0], SW_HIDE) ;

				m_hTabDlg[1] = CreateDialog(m_hInst, MAKEINTRESOURCE(IDD_DIALOG_IZO_SHIPVALUE), hwnd, StatiCIzoShipValueProc);
				GetClientRect(hwnd, &rect);
				TabCtrl_AdjustRect(hwnd, FALSE, &rect);
				MoveWindow(m_hTabDlg[1], rect.left, rect.top, rect.right-rect.left, rect.bottom- rect.top, TRUE);
				ShowWindow(m_hTabDlg[1], SW_HIDE) ;
			}

			m_siViewDlg = 0 ;

			//---------------------------------
			// HTML 을 초기화한다.
			//---------------------------------
			//==================================			
			hwnd = GetDlgItem(hDlg, IDC_STATIC_PROFILE);
			if ( hwnd )
			{
				EmbedBrowserObject(hwnd);
			}
			MoveWindow( GetDlgItem( hDlg, IDC_STATIC_PROFILE ), 9, 122, 73, 73, TRUE );

			hwnd = GetDlgItem(hDlg, IDC_STATIC_IZOINTRO);
			if ( hwnd )
			{
				EmbedBrowserObject(hwnd);
			}
		}
		break;
	case WM_NOTIFY:
		{
			LPNMHDR lpNmhdr =(LPNMHDR)lParam;

			switch ( lpNmhdr->idFrom )
			{
			case IDC_TAB_IZO:
				{
					if ( lpNmhdr->code == TCN_SELCHANGE )
					{
						int index = TabCtrl_GetCurSel( GetDlgItem ( hDlg, IDC_TAB_IZO ) );

						if ( index < 0 || index >= MAX_IZO_TAB )
						{
							return FALSE;
						}

						ShowWindow(m_hTabDlg[m_siViewDlg],SW_HIDE);
						ShowWindow(m_hTabDlg[index],SW_SHOW);

						m_siViewDlg = index ;
					}
				}
				break;
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
					GetDlgItemText(m_hDlg,IDC_EDIT_IZO,buffer,256);
					SI32 limit = strlen(buffer);

					char* pText = GetTxtFromMgr(2059);
					wsprintf(buffer, pText,m_szIzoName,limit);
					SetWindowText(m_hGroup,buffer);
				}
				break;
			case 2:
				{
					cltMsg clMsg(GAMEMSG_REQUEST_GETIZOINFO,0,NULL);
					pclclient->SendMsgToServer((char*)&clMsg);
				}
				break;
			}
		}
		break;
	case WM_CTLCOLORSTATIC:
		{
			if ( GetDlgItem( hDlg, IDC_EDIT_IZO ) == (HWND)lParam )
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
			SetTextColor( hdc, COLOR_DIALOG_BLACK);
			SetBkColor( hdc, COLOR_DIALOG_YELLOW );
								
			return (BOOL)m_hDlgBrush;
		}
		break;*/
	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case IDOK:
			{
				char buffer[1024] = "" ;

				HWND hWnd = GetDlgItem( m_hTabDlg[0], IDC_EDIT_PRICEVALUE );
				GetWindowText( hWnd, buffer, 8 );
				SI32 price = atoi(buffer);
				if ( price < 50 || price > 75 )
				{
					char* pTitle = GetTxtFromMgr(2019);
					char* pText = GetTxtFromMgr(2060);
					pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);
					break;
				}

				if ( TABS(m_siPriceValue - price) < 0 || TABS(m_siPriceValue - price) > 1 )
				{
					char* pTitle = GetTxtFromMgr(2019);
					char* pText = GetTxtFromMgr(2061);
					pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);
					break;
				}
				m_siPriceValue = price ;

				if ( pclclient->clClientDate.GetDateVary() <= m_siPriceTime + 30 )
				{
					char* pTitle = GetTxtFromMgr(2063);
					char* pText = GetTxtFromMgr(2062);
					pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);
					break;
				}

				hWnd = GetDlgItem( m_hTabDlg[1], IDC_EDIT_SHIPVALUE );
				GetWindowText( hWnd, buffer, 8 );
				price = atoi(buffer);
				if ( price < 50 || price > 85 )
				{
					char* pTitle = GetTxtFromMgr(2019);
					char* pText = GetTxtFromMgr(2064);

					pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);
					break;
				}

				if ( TABS(m_siShipValue - price) < 0 || TABS(m_siShipValue - price) > 1 )
				{
					char* pTitle = GetTxtFromMgr(2019);
					char* pText = GetTxtFromMgr(2065);

					pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);
					break;
				}
				m_siShipValue = price ;

				hWnd = GetDlgItem( m_hTabDlg[1], IDC_EDIT_TRADETAX );
				GetWindowText( hWnd, buffer, 8 );
				price = atoi(buffer);
				if ( price < 5 || price > 20 )
				{
					char* pTitle = GetTxtFromMgr(2019);
					char* pText = GetTxtFromMgr(2066);

					pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);
					break;
				}

				if ( TABS(m_siTradeTax - price) < 0 || TABS(m_siTradeTax - price) > 1 )
				{
					char* pTitle = GetTxtFromMgr(2019);
					char* pText = GetTxtFromMgr(2065);

					pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);
					break;
				}
				m_siTradeTax = price ;

				hWnd = GetDlgItem( m_hTabDlg[1], IDC_EDIT_TRADELIMITMONEY );
				GetWindowText( hWnd, buffer, 1024 );
				price = atoi(buffer);
				if ( price > 10000000 || price < 1000000 )
				{
					char* pTitle = GetTxtFromMgr(2019);
					char* pText = GetTxtFromMgr(3238);
					pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);
					break;
				}
				m_siTradeLimitMoney = price ;

				GetDlgItemText(m_hDlg,IDC_EDIT_IZO,m_szIzoProfile,256);
				SI32 limit = strlen(m_szIzoProfile);
				if ( limit > 0 && limit < 256 )
				{
					cltGameMsgRequest_SetIzoInfo clinfo(m_szIzoProfile,m_siPriceValue,m_siShipValue,
														m_siTradeTax,m_siTradeLimitMoney);
					cltMsg clMsg(GAMEMSG_REQUEST_SETIZOINFO, sizeof(clinfo), (char*)&clinfo);
					pclClient->SendMsgToServer((char*)&clMsg);
					m_bSet = TRUE ;
				}
				else
				{
					char* pTitle = GetTxtFromMgr(2019);
					char* pText = GetTxtFromMgr(2024);

					pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText );
				}
			}
			break;
		case IDGGOM:
			{
				cltGameMsgRequest_Userid clUserid( m_szIzoName );
				cltMsg clMsg( GAMEMSG_REQUEST_USERID, sizeof( clUserid ), (char*)&clUserid );
				pclClient->SendMsgToServer((char*)&clMsg);
			}
			break;
		case IDWHISPER:
			{
				if ( strlen(m_szIzoName) > 0 && m_szIzoName != NULL )
					pclclient->RequestSecretChatByCharName(m_szIzoName);
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
			hwnd = GetDlgItem( hDlg, IDC_STATIC_IZOINTRO );
			if( hwnd )
			{
				DoPageAction(hwnd, WEBPAGE_STOP);
				UnEmbedBrowserObject(hwnd);
			}

			GetWindowRect(m_hDlg, &m_rtDlg);
			m_bRectSwitch = TRUE ;

			if ( pclclient->pclCM->CR[1]->pclCI->clBI.uiGRank == GRANK_MINISTER1 )
				KillTimer(hDlg,1);
			else
				KillTimer(hDlg,2);

			m_siViewDlg = 0 ;

			m_bSet = FALSE ;
			m_hDlg = NULL ;
		}
		break;
	}

	return FALSE;
}

BOOL CALLBACK CIzoDlg::StatiCIzoPriceValueProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	cltClient *pclclient = (cltClient*)pclClient;
	return pclclient->m_pIzoDlg->IzoPriceValueProc( hDlg, iMsg, wParam, lParam );
}

BOOL CALLBACK CIzoDlg::IzoPriceValueProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam )
{
	cltClient *pclclient = (cltClient*)pclClient;

	switch( iMsg )
	{
	case WM_INITDIALOG:
		{
		}
		break;
	case WM_NOTIFY:
		{
			LPNMHDR pnmhdr=(LPNMHDR)lParam;
			if(pnmhdr->hwndFrom == 	GetDlgItem(hDlg, IDC_SPIN_IZO) )
			{
				HWND hwnd = GetDlgItem(hDlg, IDC_SPIN_IZO);

				SI16 price = (SI16)LOWORD( SendMessage(hwnd, UDM_GETPOS, 0, 0 ) );

				char buffer[16] = "" ;
				wsprintf(buffer, "%d", price );
				SetDlgItemText(hDlg, IDC_EDIT_PRICEVALUE, buffer);
			}
		}
		break;
	case WM_DESTROY:
		{
		}
		break;
	}

	return FALSE;
}

BOOL CALLBACK CIzoDlg::StatiCIzoShipValueProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	cltClient *pclclient = (cltClient*)pclClient;
	return pclclient->m_pIzoDlg->IzoShipValueProc( hDlg, iMsg, wParam, lParam );
}

BOOL CALLBACK CIzoDlg::IzoShipValueProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam )
{
	cltClient *pclclient = (cltClient*)pclClient;

	switch( iMsg )
	{
	case WM_INITDIALOG:
		{
		}
		break;
	case WM_NOTIFY:
		{
			LPNMHDR pnmhdr=(LPNMHDR)lParam;
			if(pnmhdr->hwndFrom == 	GetDlgItem(hDlg, IDC_SPIN_IZO_SHIP) )
			{
				HWND hwnd = GetDlgItem(hDlg, IDC_SPIN_IZO_SHIP);

				SI16 price = (SI16)LOWORD( SendMessage(hwnd, UDM_GETPOS, 0, 0 ) );

				char buffer[16] = "" ;
				wsprintf(buffer, "%d", price );
				SetDlgItemText(hDlg, IDC_EDIT_SHIPVALUE, buffer);
			}
		}
		break;
	case WM_DESTROY:
		{
		}
		break;
	}

	return FALSE;
}

void CIzoDlg::Set()
{
	if ( !IsWindow(m_hDlg) )
	{
		m_hDlg = CreateDialog( m_hInst, MAKEINTRESOURCE( IDD_DIALOG_IZO ), m_hWnd, StatiCIzoDlgProc );
		SendDlgItemMessage( m_hDlg, IDC_EDIT_IZO, EM_LIMITTEXT, 255, 0 );
		m_hGroup = GetDlgItem(m_hDlg,IDC_STATIC_IZOPROFILE);

		if ( m_bRectSwitch )
		{
			MoveWindow(m_hDlg, 
				m_rtDlg.left, m_rtDlg.top, 
				m_rtDlg.right - m_rtDlg.left, 
				m_rtDlg.bottom - m_rtDlg.top, FALSE);
		}

		HWND hButton = NULL ;
		cltClient *pclclient = (cltClient*)pclClient;
		if ( pclclient->pclCM->CR[1]->pclCI->clBI.uiGRank != GRANK_MINISTER1 )
		{
			//hButton = GetDlgItem(m_hDlg,IDSETUP);
			//EnableWindow(hButton,FALSE);

			hButton = GetDlgItem(m_hDlg,IDC_EDIT_IZO);
			EnableWindow(hButton,FALSE);

			hButton = GetDlgItem(m_hDlg,IDOK);
			EnableWindow(hButton,FALSE);
			
			hButton = GetDlgItem(m_hTabDlg[0],IDC_SPIN_IZO);
			EnableWindow(hButton,FALSE);

			hButton = GetDlgItem(m_hTabDlg[1],IDC_SPIN_IZO_SHIP);
			EnableWindow(hButton,FALSE);

			hButton = GetDlgItem(m_hTabDlg[1],IDC_SPIN_IZO_TRADE);
			EnableWindow(hButton,FALSE);

			hButton = GetDlgItem(m_hTabDlg[1],IDC_EDIT_TRADELIMITMONEY);
			EnableWindow(hButton,FALSE);
		}
		else
		{
			hButton = GetDlgItem(m_hDlg,IDGGOM);
			EnableWindow(hButton,FALSE);

			hButton = GetDlgItem(m_hDlg,IDWHISPER);
			EnableWindow(hButton,FALSE);
		}

		cltMsg clMsg(GAMEMSG_REQUEST_GETIZOINFO,0,NULL);
		pclclient->SendMsgToServer((char*)&clMsg);
	}
	else
	{
		DestroyWindow(m_hDlg);
	}
}

void CIzoDlg::Show(CMinisterBase *pMini,SI32 pricetime,SI32 priceinx,SI32 pricevalue,
				   SI64 money,SI32 calctime,SI32 shipvalue,SI32 tradetax,SI32 tradelimitmoney)
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

	pclClient->PushEffect(EFFECT_IZO,0,0);

	cltDate cldate;
	char buffer[256] = "" ;
	char numbuf[128] = "" ;

	strcpy(m_szIzoID,pMini->m_szAccountID);
	strcpy(m_szIzoName,pMini->m_szPersonName);
	strcpy(m_szIzoProfile,pMini->m_szGreeting);

	m_siPriceValue = pricevalue ;
	m_siPriceTime = pricetime;
	m_siTotalMoney = money ;
	m_siCalcTime = calctime ;
	m_siShipValue = shipvalue;
	m_siTradeTax = tradetax;
	m_siTradeLimitMoney = tradelimitmoney ;

	// 날짜를 구한다.
	cldate.MakeFullDateFromDateVary(pricetime);
	if(cldate.GetDateText(buffer) == FALSE)
	{
		return ;
	}
	SetDlgItemText(m_hDlg,IDC_STATIC_IZODAY,buffer);

	// 통화량 업데이트 날짜. 
	cldate.MakeFullDateFromDateVary(m_siCalcTime);
	if(cldate.GetDateText(numbuf) == TRUE)
	{
		char* pText = GetTxtFromMgr(612);
		sprintf(buffer, pText, numbuf);
		SetDlgItemText(m_hTabDlg[0], IDC_STATIC_TOTALMONEY_DATE, buffer);
	}

	HWND hspin = GetDlgItem(m_hTabDlg[0], IDC_SPIN_IZO);
	HWND hedit = GetDlgItem(m_hTabDlg[0], IDC_EDIT_PRICEVALUE);

	SendMessage(hspin, UDM_SETBUDDY, (WPARAM)hedit, 0);
	if ( m_siPriceValue <= 50 )
		SendMessage(hspin, UDM_SETRANGE, 0, MAKELPARAM(m_siPriceValue+1, 50));
	else if ( m_siPriceValue >= 75 )
		SendMessage(hspin, UDM_SETRANGE, 0, MAKELPARAM(75, m_siPriceValue-1));
	else SendMessage(hspin, UDM_SETRANGE, 0, MAKELPARAM(m_siPriceValue+1, m_siPriceValue-1));

	hspin = GetDlgItem(m_hTabDlg[1], IDC_SPIN_IZO_SHIP);
	hedit = GetDlgItem(m_hTabDlg[1], IDC_EDIT_SHIPVALUE);

	SendMessage(hspin, UDM_SETBUDDY, (WPARAM)hedit, 0);
	if ( m_siShipValue <= 50 )
		SendMessage(hspin, UDM_SETRANGE, 0, MAKELPARAM(m_siShipValue+1, 50));
	else if ( m_siShipValue >= 85 )
		SendMessage(hspin, UDM_SETRANGE, 0, MAKELPARAM(85, m_siShipValue-1));
	else SendMessage(hspin, UDM_SETRANGE, 0, MAKELPARAM(m_siShipValue+1, m_siShipValue-1));

	hspin = GetDlgItem(m_hTabDlg[1], IDC_SPIN_IZO_TRADE);
	hedit = GetDlgItem(m_hTabDlg[1], IDC_EDIT_TRADETAX);

	SendMessage(hspin, UDM_SETBUDDY, (WPARAM)hedit, 0);
	if ( m_siTradeTax <= 5 )
		SendMessage(hspin, UDM_SETRANGE, 0, MAKELPARAM(m_siTradeTax+1, 5));
	else if ( m_siTradeTax >= 20 )
		SendMessage(hspin, UDM_SETRANGE, 0, MAKELPARAM(20, m_siTradeTax-1));
	else SendMessage(hspin, UDM_SETRANGE, 0, MAKELPARAM(m_siTradeTax+1, m_siTradeTax-1));

	char* pText = GetTxtFromMgr(2067);
	wsprintf(buffer, pText,m_szIzoName);
	SetWindowText( m_hDlg ,buffer);
	SetDlgItemText( m_hDlg,IDC_EDIT_IZO, m_szIzoProfile );
	pText = GetTxtFromMgr(2048);
	strcat(buffer, pText);
	SetWindowText(m_hGroup,buffer);

	wsprintf(buffer,"%d",priceinx);
	SetDlgItemText( m_hTabDlg[0],IDC_EDIT_PRICEINX, buffer );

	wsprintf(buffer, "%d", m_siPriceValue );
	SetDlgItemText(m_hTabDlg[0], IDC_EDIT_PRICEVALUE, buffer);

	wsprintf(buffer, "%d", m_siShipValue );
	SetDlgItemText(m_hTabDlg[1], IDC_EDIT_SHIPVALUE, buffer);

	wsprintf(buffer, "%d", m_siTradeTax );
	SetDlgItemText(m_hTabDlg[1], IDC_EDIT_TRADETAX, buffer);

	wsprintf(buffer,"%d",m_siTradeLimitMoney);
	SetDlgItemText(m_hTabDlg[1], IDC_EDIT_TRADELIMITMONEY, buffer);

	pText = GetTxtFromMgr(611);
	g_JYLibFunction.SetNumUnit(m_siTotalMoney,numbuf, pText);
	SetDlgItemText(m_hTabDlg[0], IDC_STATIC_TOTALMONEY, numbuf);

	SI32 alchemyrate = ( 75 - m_siPriceValue ) * (-0.5 ) + 13 ;
	wsprintf(buffer, "%d %%", alchemyrate );
	SetDlgItemText(m_hTabDlg[0], IDC_STATIC_ALCHEMY_RATE, buffer);

	HWND hWnd = NULL ;
	char ProfileURL[MAX_PATH];

/*	pclClient->GetCZProfileUrl( ProfileURL, m_szIzoID );
	
	hWnd = GetDlgItem( m_hDlg, IDC_STATIC_PROFILE );
	if(hWnd)	DisplayHTMLPage(hWnd, ProfileURL);*/

	// INTRO
	pclClient->GetCZIntroUrl( ProfileURL, m_szIzoID );
	
	hWnd = GetDlgItem(m_hDlg, IDC_STATIC_IZOINTRO);
	if(hWnd)	DisplayHTMLPage(hWnd, ProfileURL);

	ShowWindow( m_hDlg, SW_SHOW );
	if ( m_siViewDlg == 0 )
		ShowWindow(m_hTabDlg[m_siViewDlg], SW_SHOW) ;
}