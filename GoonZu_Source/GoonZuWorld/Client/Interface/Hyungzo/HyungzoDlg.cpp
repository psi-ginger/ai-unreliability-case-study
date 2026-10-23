// HyungzoDlg.cpp: implementation of the CHyungzoDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "../../Client/client.h"

#include "../../lib/WebHTML/WebHTML.h"

#include "../../common/HintDlg/HintDlg.h"

//-----------------------------
// CommonLogic
//-----------------------------
#include "MsgType.h"
#include "CommonLogic.h"
#include "MsgType-System.h"
#include "MsgType-Person.h"


//-----------------------------
// Common
//-----------------------------
#include "PersonInfo.h"
//#include "../../common/AutoHunting/AutoHunting.h"
#include "NInterface/NAutoHunting/NAutoHunting.h"
#include "Char/CharManager/CharManager.h"

//-----------------------------
// Client
//-----------------------------
#include "../../Client/Interface/ErrorMsgDlg/ErrorMsgDlg.h"
#include "../../Client/Interface/NotifyMsgDlg/NotifyMsgDlg.h"
#include "../../Client/Interface/SearchDetailDlg/SearchDetailDlg.h"
#include "NInterface/NHintDlg/NHintDlg.h"

#include "../../Client/Music/Music.h"

#include "HyungzoDlg.h"
#include "../Server/Minister/Minister-Hyungzo.h"

extern cltCommonLogic* pclClient;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CHyungzoDlg::CHyungzoDlg()
{
	m_hDlg = NULL ;
	for ( int i = 0 ; i < MAX_HYUNGZO_TAB ; i ++ )
		m_hTabDlg[i] = NULL ;

	m_hGroup = NULL ;
	m_bRectSwitch = FALSE ;

	m_hLogList = NULL ;

	m_siStealNum = 0 ;
	m_siSuspectTime = 0 ;
	m_siPrisonTime = 0 ;

	m_bSet = FALSE ;
	m_hEditBrush = NULL ;
	m_hDlgBrush  = NULL;

	strcpy(m_szHyungzoName,"");
	strcpy(m_szHyungzoProfile,"");
	strcpy(m_szHyungzoID,"");

	m_siViewDlg = 0 ;

	m_hAccusationHit = NULL ;
}

CHyungzoDlg::~CHyungzoDlg()
{
}

void CHyungzoDlg::Create(HINSTANCE hInst, HWND hParent )
{
	m_hInst = hInst ;
	m_hWnd = hParent ;
}

BOOL CALLBACK CHyungzoDlg::StatiCHyungzoDlgProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	cltClient *pclclient = (cltClient*)pclClient;
	return pclclient->m_pHyungzoDlg->HyungzoDlgProc( hDlg, iMsg, wParam, lParam );
}

BOOL CALLBACK CHyungzoDlg::HyungzoDlgProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam )
{
	cltClient *pclclient = (cltClient*)pclClient;

	switch( iMsg )
	{
	case WM_INITDIALOG:
		{
			m_hEditBrush = CreateSolidBrush( COLOR_DIALOG_YELLOW );
			m_hDlgBrush  = CreateSolidBrush( COLOR_DIALOG_LIGHTBLUE);
			if ( pclclient->pclCM->CR[1]->pclCI->clBI.uiGRank == GRANK_MINISTER5 )
				SetTimer(hDlg, 1, 1000, NULL);
			else
				SetTimer(hDlg, 2, 60000, NULL);

			HWND hwnd = NULL ;
			TC_ITEM item;
			RECT rect;

			hwnd = GetDlgItem(hDlg,IDC_TAB_HYUNGZO);
			if(hwnd)
			{
				item.mask= TCIF_TEXT;
				item.iImage		= -1;
				char* pText = GetTxtFromMgr(2015);
				item.pszText	= pText;
				TabCtrl_InsertItem(hwnd,0,&item);

				pText = GetTxtFromMgr(2016);
				item.pszText	=	pText;
				TabCtrl_InsertItem(hwnd, 1, &item);

				pText = GetTxtFromMgr(2017);
				item.pszText	=	pText;
				TabCtrl_InsertItem(hwnd, 2, &item);

				pText = GetTxtFromMgr(5543);
				item.pszText	=	pText;
				TabCtrl_InsertItem(hwnd, 3, &item);

				m_hTabDlg[0] = CreateDialog(m_hInst, MAKEINTRESOURCE(IDD_DIALOG_HYUNGZO_MAIN), hwnd, StatiCHyungzoMainProc);
				GetClientRect(hwnd, &rect);
				TabCtrl_AdjustRect(hwnd, FALSE, &rect);
				MoveWindow(m_hTabDlg[0], rect.left, rect.top, rect.right-rect.left, rect.bottom- rect.top, TRUE);
				ShowWindow(m_hTabDlg[0], SW_HIDE) ;

				m_hTabDlg[1] = CreateDialog(m_hInst, MAKEINTRESOURCE(IDD_DIALOG_HYUNGZO_USER), hwnd, StatiCHyungzoUserProc);
				GetClientRect(hwnd, &rect);
				TabCtrl_AdjustRect(hwnd, FALSE, &rect);
				MoveWindow(m_hTabDlg[1], rect.left, rect.top, rect.right-rect.left, rect.bottom- rect.top, TRUE);
				ShowWindow(m_hTabDlg[1], SW_HIDE) ;

				m_hTabDlg[2] = CreateDialog(m_hInst, MAKEINTRESOURCE(IDD_DIALOG_HYUNGZO_ANNOUNCEMENT), hwnd, StatiCHyungzoAnnouncementProc);
				GetClientRect(hwnd, &rect);
				TabCtrl_AdjustRect(hwnd, FALSE, &rect);
				MoveWindow(m_hTabDlg[2], rect.left, rect.top, rect.right-rect.left, rect.bottom- rect.top, TRUE);
				ShowWindow(m_hTabDlg[2], SW_HIDE) ;

				m_hTabDlg[3] = CreateDialog(m_hInst, MAKEINTRESOURCE(IDD_DIALOG_HYUNGZO_POLICE), hwnd, StatiCHyungzoPoliceProc);
				GetClientRect(hwnd, &rect);
				TabCtrl_AdjustRect(hwnd, FALSE, &rect);
				MoveWindow(m_hTabDlg[3], rect.left, rect.top, rect.right-rect.left, rect.bottom- rect.top, TRUE);
				ShowWindow(m_hTabDlg[3], SW_HIDE) ;
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

			hwnd = GetDlgItem(hDlg, IDC_STATIC_HYUNGZOINTRO);
			if ( hwnd )
			{
				EmbedBrowserObject(hwnd);
			}
			
			// 형조판서 책상 열기 퀘스트중인가 ?
			pclClient->pclCM->CR[1]->DoQuestCheckAndAction(QUEST_CLEAR_COND_LOOKPANSEO);

		}
		break;
	case WM_TIMER:
		{
			switch(wParam)
			{
			case 1:
				{
					char buffer[256] = "" ;
					GetDlgItemText(m_hDlg,IDC_EDIT_HYUNGZO,buffer,256);
					SI32 limit = strlen(buffer);

					char* pText = GetTxtFromMgr(2018);
					wsprintf(buffer,pText,m_szHyungzoName,limit);
					SetWindowText(m_hGroup,buffer);
				}
				break;
			case 2:
				{
					cltMsg clMsg(GAMEMSG_REQUEST_GETHYUNGZOINFO,0,NULL);
					pclclient->SendMsgToServer((char*)&clMsg);
				}
				break;
			}
		}
		break;
	case WM_NOTIFY:
		{
			LPNMHDR lpNmhdr =(LPNMHDR)lParam;

			switch ( lpNmhdr->idFrom )
			{
			case IDC_TAB_HYUNGZO:
				{
					if ( lpNmhdr->code == TCN_SELCHANGE )
					{
						int index = TabCtrl_GetCurSel( GetDlgItem ( hDlg, IDC_TAB_HYUNGZO ) );

						if ( index < 0 || index > MAX_HYUNGZO_TAB )
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
	case WM_CTLCOLORSTATIC:
		{
			if ( GetDlgItem( hDlg, IDC_EDIT_HYUNGZO ) == (HWND)lParam )
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
				char buffer[8] = "" ;

				HWND hWnd = GetDlgItem( m_hTabDlg[0], IDC_EDIT_LIMIT_STEAL );
				GetWindowText( hWnd, buffer, 8 );
				m_siStealNum = atoi(buffer);
				if ( m_siStealNum < 3 || m_siStealNum > 10 )
				{
					char* pTitle = GetTxtFromMgr(2019);
					char* pText = GetTxtFromMgr(2020);
					pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);
					break;
				}

				hWnd = GetDlgItem( m_hTabDlg[0], IDC_EDIT_LIMIT_SUSPECTTIME );
				GetWindowText( hWnd, buffer, 8 );
				m_siSuspectTime = atoi(buffer);
				if ( m_siSuspectTime < 20 || m_siSuspectTime > 300 )
				{
					char* pTitle = GetTxtFromMgr(2019);
					char* pText = GetTxtFromMgr(2021);
					pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);
					break;
				}

				hWnd = GetDlgItem( m_hTabDlg[0], IDC_EDIT_LIMIT_SUSPECT_PRISONTIME );
				GetWindowText( hWnd, buffer, 8 );
				m_siPrisonTime = atoi(buffer);
				if ( m_siPrisonTime < 5 || m_siPrisonTime > 60 )
				{
					char* pTitle = GetTxtFromMgr(2019);
					char* pText = GetTxtFromMgr(2022);
					pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);
					break;
				}

				if ( m_siPrisonerPenalty < 20 || m_siPrisonerPenalty > 100 )
				{
					char* pTitle = GetTxtFromMgr(2019);
					char* pText = GetTxtFromMgr(2023);
					pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);
					break;
				}

				GetDlgItemText(m_hDlg,IDC_EDIT_HYUNGZO,m_szHyungzoProfile,256);
				SI32 limit = strlen(m_szHyungzoProfile);
				if ( limit > 0 && limit < 256 )
				{
					cltGameMsgRequest_SetHyungzoInfo clinfo(m_szHyungzoProfile,m_siStealNum,
						m_siSuspectTime,m_siPrisonTime,m_siPrisonerPenalty);
					cltMsg clMsg(GAMEMSG_REQUEST_SETHYUNGZOINFO, sizeof(clinfo), (char*)&clinfo);
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
				cltGameMsgRequest_Userid clUserid( m_szHyungzoName );
				cltMsg clMsg( GAMEMSG_REQUEST_USERID, sizeof( clUserid ), (char*)&clUserid );
				pclClient->SendMsgToServer((char*)&clMsg);
			}
			break;
		case IDWHISPER:
			{
				if ( strlen(m_szHyungzoName) > 0 && m_szHyungzoName != NULL )
					pclclient->RequestSecretChatByCharName(m_szHyungzoName);
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
			hwnd = GetDlgItem( hDlg, IDC_STATIC_HYUNGZOINTRO );
			if( hwnd )
			{
				DoPageAction(hwnd, WEBPAGE_STOP);
				UnEmbedBrowserObject(hwnd);
			}

			GetWindowRect(m_hDlg, &m_rtDlg);
			m_bRectSwitch = TRUE ;

			if ( pclclient->pclCM->CR[1]->pclCI->clBI.uiGRank == GRANK_MINISTER5 )
				KillTimer(hDlg,1);
			else
				KillTimer(hDlg,2);

			for ( int i = 0 ; i < MAX_HYUNGZO_TAB ; i ++ )
			{
				if ( m_hTabDlg[i] )
				{
					DestroyWindow(m_hTabDlg[i]);
					m_hTabDlg[i] = NULL ;
				}
			}

			m_bSet = FALSE ;

			m_hDlg = NULL ;
		}
		break;
	}

	return FALSE;
}

BOOL CALLBACK CHyungzoDlg::StatiCHyungzoMainProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	cltClient *pclclient = (cltClient*)pclClient;
	return pclclient->m_pHyungzoDlg->HyungzoMainProc( hDlg, iMsg, wParam, lParam );
}

BOOL CALLBACK CHyungzoDlg::HyungzoMainProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam )
{
	cltClient *pclclient = (cltClient*)pclClient;

	switch( iMsg )
	{
	case WM_INITDIALOG:
		{
		}
		break;
	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case IDC_RADIO_ONE:
			m_siPrisonerPenalty = 20 ;
			break;
		case IDC_RADIO_TWO:
			m_siPrisonerPenalty = 40 ;
			break;
		case IDC_RADIO_THREE:
			m_siPrisonerPenalty = 60 ;
			break;
		case IDC_RADIO_FOUR:
			m_siPrisonerPenalty = 80 ;
			break;
		case IDC_RADIO_FIVE:
			m_siPrisonerPenalty = 100 ;
			break;
		case IDC_BUTTON_SUSPECTTIME_INFO:
			{
				char buffer[512] = "" ;
				char* pText = GetTxtFromMgr(2025);
				sprintf(buffer, pText);
				char* pTitle = GetTxtFromMgr(2026);
				pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, buffer);
			}
			break;
		case IDC_BUTTON_PRISONERPENALTY_INFO:
			{
				char buffer[512] = "" ;
				char* pText = GetTxtFromMgr(2027);
				sprintf(buffer,pText);
				char* pTitle = GetTxtFromMgr(2028);
				pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle,buffer);
			}
			break;
		}
		break;
	case WM_DESTROY:
		{
			if ( m_hAccusationHit )
			{
				DeleteObject(m_hAccusationHit);
				m_hAccusationHit = NULL ;
			}
		}
		break;
	}

	return FALSE;
}

BOOL CALLBACK CHyungzoDlg::StatiCHyungzoUserProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	cltClient *pclclient = (cltClient*)pclClient;
	return pclclient->m_pHyungzoDlg->HyungzoUserProc( hDlg, iMsg, wParam, lParam );
}

BOOL CALLBACK CHyungzoDlg::HyungzoUserProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam )
{
	cltClient *pclclient = (cltClient*)pclClient;

	switch( iMsg )
	{
	case WM_INITDIALOG:
		{
			m_hAccusationHit = (HBITMAP)LoadImage( pclClient->GetInstance(), "statushint/accusationhint.bmp", IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE );
			if ( m_hAccusationHit != NULL )
			{
				SendDlgItemMessage( hDlg, IDC_BUTTON_ACCUSATION_EXPLAIN, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)m_hAccusationHit);
			}
		}
		break;
	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case IDC_BUTTON_PRISON:
			{
				char buffer[256] = "" ;
				HyungzoMember hyungzomember ;
				hyungzomember.siDate = 0 ;

				HWND hwnd = GetDlgItem(hDlg,IDC_EDIT_PRISONER);
				GetWindowText(hwnd,hyungzomember.szCharName,MAX_PLAYER_NAME);
				if ( strlen(hyungzomember.szCharName) <= 0 || !strcmp(hyungzomember.szCharName,"") )
				{
					char* pTitle = GetTxtFromMgr(2029);
					char* pText = GetTxtFromMgr(2030);
					pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);
					break;
				}

				hwnd = GetDlgItem(hDlg,IDC_EDIT_REASON);
				GetWindowText(hwnd,hyungzomember.szPrisonReason,MAX_PRISON_REASON_SIZE);
				if ( strlen(hyungzomember.szPrisonReason) <= 0 || !strcmp(hyungzomember.szPrisonReason,"") )
				{
					char* pTitle = GetTxtFromMgr(2031);
					char* pText = GetTxtFromMgr(2032);
					sprintf(buffer, pText,MAX_PRISON_REASON_SIZE/2);
					pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle,buffer);
					break;
				}

				char timelimit[16] = "" ;
				hwnd = GetDlgItem(hDlg,IDC_EDIT_LIMIT_PRISON);
				GetWindowText(hwnd,timelimit,16);
				hyungzomember.siPrisonSecond = atoi(timelimit);
				if ( hyungzomember.siPrisonSecond <= 0 || hyungzomember.siPrisonSecond > MAX_PRISON_SECOND )
				{
					char* pTitle = GetTxtFromMgr(2033);
					char* pText = GetTxtFromMgr(2034);
					sprintf(buffer,pText,MAX_PRISON_SECOND);
					pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle,buffer);
					break;
				}

				if ( pclclient->pclCM->CR[1]->pclCI->clBI.uiGRank != GRANK_MINISTER5 || 
					 pclclient->pclCM->CR[1]->pclCI->clCharItem.clItem[PERSONITEM_DRESS].siUnique != 9290 )
				{
					char* pTitle = GetTxtFromMgr(2035);
					char* pText = GetTxtFromMgr(2036);
					sprintf(buffer,pText);
					pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle,buffer);
					break;
				}

				cltGameMsgRequest_SetHyungzoPrison clinfo(&hyungzomember);
				cltMsg clMsg(GAMEMSG_REQUEST_SETHYUNGZOPRISON, sizeof(clinfo), (char*)&clinfo);
				pclClient->SendMsgToServer((char*)&clMsg);
			}
			break ;
		case IDC_BUTTON_ACCUSATION_EXPLAIN:
			{
				//pclclient->m_pHintDlg->Show("accusation");
				if((pclclient->m_pDialog[ NHINT_DLG ])==NULL)
				{
					pclclient->CreateInterface( NHINT_DLG );
				}
				((NHintDlgExplain*)pclclient->m_pDialog[ NHINT_DLG])->Show("accusation");
			}
			break;
		case IDC_BUTTON_REQUEST:
			{
				if ( pclclient->pclCM->CR[1]->pclCI->clIP.GetLevel() < 20 )
				{
					char* pTitle = GetTxtFromMgr(2037);
					char* pText = GetTxtFromMgr(2038);
					pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);
					break;
				}

				char name[MAX_PLAYER_NAME] = "" ;
				char buffer[256] = "" ;
				char buffer2[256] = "" ;

				GetWindowText(GetDlgItem(hDlg,IDC_EDIT_SUSPECT),name,MAX_PLAYER_NAME);
				
				if ( strlen(name) <= 0 || !strcmp(name,"") )
				{
					char* pTitle = GetTxtFromMgr(567);
					char* pText = GetTxtFromMgr(2039);
					pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);
					break;
				}

				char* pText = GetTxtFromMgr(2040);
				sprintf(buffer, pText,
					name);
				pText = GetTxtFromMgr(2041);
				char min[128]= " " ;
				sprintf(min,"%d",NOSUSPICTION_PRISON_TIME);
				Nsprintf(buffer2,pText,"user",  name  ,"min", min   ,NULL);
				//sprintf(buffer2, pText,
				//	name,NOSUSPICTION_PRISON_TIME);

				if (  MessageBox(NULL, buffer2,
					buffer, MB_ICONQUESTION | MB_YESNO) == IDNO )
					break;

				cltGameMsgRequest_AutoHunting clinfo(name);
				cltMsg clMsg(GAMEMSG_REQUEST_AUTOHUNTING, sizeof(clinfo), (char*)&clinfo);
				pclClient->SendMsgToServer((char*)&clMsg);
			}
			break;
		}
		break;
	case WM_DESTROY:
		{
		}
		break;
	}

	return FALSE;
}

BOOL CALLBACK CHyungzoDlg::StatiCHyungzoAnnouncementProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	cltClient *pclclient = (cltClient*)pclClient;
	return pclclient->m_pHyungzoDlg->HyungzoAnnouncementProc( hDlg, iMsg, wParam, lParam );
}

BOOL CALLBACK CHyungzoDlg::HyungzoAnnouncementProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam )
{
	cltClient *pclclient = (cltClient*)pclClient;

	switch( iMsg )
	{
	case WM_INITDIALOG:
		{
			// 수감자 리스트 초기화
			m_hLogList = GetDlgItem( hDlg, IDC_LIST_HYUNGZO_LOG );

			if(m_hLogList)
			{
				ListView_SetExtendedListViewStyle( m_hLogList, LVS_EX_FULLROWSELECT | LVS_EX_ONECLICKACTIVATE );
				ListView_SetHoverTime( m_hLogList, 10 );
				
				char* pText = GetTxtFromMgr(1564);
				ListViewInsertColumn( m_hLogList, 70, pText,	0, 1);
				pText = GetTxtFromMgr(2042);
				ListViewInsertColumn( m_hLogList, 70, pText, 1, 1);
				pText = GetTxtFromMgr(2043);
				ListViewInsertColumn( m_hLogList, 60, pText,	2, 1);
				pText = GetTxtFromMgr(2044);
				ListViewInsertColumn( m_hLogList, 80, pText,	3, 1);

				ListView_DeleteAllItems(m_hLogList);
			}
		}
		break;
	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case IDC_BOARD_HYUNGZO:
			{
				pclclient->m_pSearchDetailDlg->Show(BULLETINBOARD_HYUNGZO, "" );
			}
			break;
		}
		break;
	case WM_DESTROY:
		{
		}
		break;
	}

	return FALSE;
}

BOOL CALLBACK CHyungzoDlg::StatiCHyungzoPoliceProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	cltClient *pclclient = (cltClient*)pclClient;
	return pclclient->m_pHyungzoDlg->HyungzoPoliceProc( hDlg, iMsg, wParam, lParam );
}

BOOL CALLBACK CHyungzoDlg::HyungzoPoliceProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam )
{
	cltClient *pclclient = (cltClient*)pclClient;

	switch( iMsg )
	{
	case WM_INITDIALOG:
		{
			// 수감자 리스트 초기화
			m_hPoliceList = GetDlgItem( hDlg, IDC_LIST_POLICELIST );

			if(m_hPoliceList)
			{
				ListView_SetExtendedListViewStyle( m_hLogList, LVS_EX_FULLROWSELECT | LVS_EX_ONECLICKACTIVATE );
				ListView_SetHoverTime( m_hLogList, 10 );

				char* pText = GetTxtFromMgr(1556);
				ListViewInsertColumn( m_hPoliceList, 70, pText,	0, 1);
				pText = GetTxtFromMgr(1506);
				ListViewInsertColumn( m_hPoliceList, 30, pText, 1, 1);
				
				ListView_DeleteAllItems(m_hLogList);
			}
		}
		break;
	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		// 포교 설정
		case IDC_BUTTON_POLICESET:
			{
				
			}
			break;
		// 포교 해임
		case IDC_BUTTON_POLICEFIRE:
			{

			}
			break;
		// 현행범 목록 보기
		case IDC_BUTTON_CRIMINALLIST:
			{

			}
			break;
		// 현행범 신고
		case IDC_BUTTON_CRIMINALREPORT:
			{

			}
			break;
		}
		break;
	case WM_DESTROY:
		{
		}
		break;
	}

	return FALSE;
}

void CHyungzoDlg::Set()
{
	if ( !IsWindow(m_hDlg) )
	{
		m_hDlg = CreateDialog( m_hInst, MAKEINTRESOURCE( IDD_DIALOG_HYUNGZO ), m_hWnd, StatiCHyungzoDlgProc );
		SendDlgItemMessage( m_hDlg, IDC_EDIT_HYUNGZO, EM_LIMITTEXT, 255, 0 );
		m_hGroup = GetDlgItem(m_hDlg,IDC_STATIC_HYUNGZOPROFILE);

		if ( m_bRectSwitch )
		{
			MoveWindow(m_hDlg, 
				m_rtDlg.left, m_rtDlg.top, 
				m_rtDlg.right - m_rtDlg.left, 
				m_rtDlg.bottom - m_rtDlg.top, FALSE);
		}

		HWND hButton = NULL ;
		cltClient *pclclient = (cltClient*)pclClient;
		if ( pclclient->pclCM->CR[1]->pclCI->clBI.uiGRank != GRANK_MINISTER5 )
		{
			//hButton = GetDlgItem(m_hDlg,IDSETUP);
			//EnableWindow(hButton,FALSE);

			hButton = GetDlgItem(m_hDlg,IDC_EDIT_HYUNGZO);
			EnableWindow(hButton,FALSE);

			// Main - 형조판서 권한창 m_hTabDlg[0]
			hButton = GetDlgItem(m_hDlg,IDOK);
			EnableWindow(hButton,FALSE);

			hButton = GetDlgItem(m_hTabDlg[0],IDC_EDIT_LIMIT_STEAL);
			EnableWindow(hButton,FALSE);

			hButton = GetDlgItem(m_hTabDlg[0],IDC_EDIT_LIMIT_SUSPECTTIME);
			EnableWindow(hButton,FALSE);

			hButton = GetDlgItem(m_hTabDlg[0],IDC_EDIT_LIMIT_SUSPECT_PRISONTIME);
			EnableWindow(hButton,FALSE);

			hButton = GetDlgItem(m_hTabDlg[0],IDC_RADIO_ONE);
			EnableWindow(hButton,FALSE);
			hButton = GetDlgItem(m_hTabDlg[0],IDC_RADIO_TWO);
			EnableWindow(hButton,FALSE);
			hButton = GetDlgItem(m_hTabDlg[0],IDC_RADIO_THREE);
			EnableWindow(hButton,FALSE);
			hButton = GetDlgItem(m_hTabDlg[0],IDC_RADIO_FOUR);
			EnableWindow(hButton,FALSE);
			hButton = GetDlgItem(m_hTabDlg[0],IDC_RADIO_FIVE);
			EnableWindow(hButton,FALSE);

			hButton = GetDlgItem(m_hTabDlg[1],IDC_EDIT_REASON);
			EnableWindow(hButton,FALSE);

			hButton = GetDlgItem(m_hTabDlg[1],IDC_EDIT_PRISONER);
			EnableWindow(hButton,FALSE);

			hButton = GetDlgItem(m_hTabDlg[1],IDC_EDIT_LIMIT_PRISON);
			EnableWindow(hButton,FALSE);

			hButton = GetDlgItem(m_hTabDlg[1],IDC_BUTTON_PRISON);
			EnableWindow(hButton,FALSE);

			// 포교
			hButton = GetDlgItem(m_hTabDlg[2],IDC_EDIT_POLICENAME);
			EnableWindow(hButton,FALSE);
			hButton = GetDlgItem(m_hTabDlg[2],IDC_EDIT_CRIMINALNAME);
			EnableWindow(hButton,FALSE);
			hButton = GetDlgItem(m_hTabDlg[2],IDC_EDIT_CRIMINALREASON);
			EnableWindow(hButton,FALSE);

			hButton = GetDlgItem(m_hTabDlg[2],IDC_BUTTON_POLICESET);
			EnableWindow(hButton,FALSE);
			hButton = GetDlgItem(m_hTabDlg[2],IDC_BUTTON_POLICEFIRE);
			EnableWindow(hButton,FALSE);
			hButton = GetDlgItem(m_hTabDlg[2],IDC_BUTTON_CRIMINALLIST);
			EnableWindow(hButton,FALSE);
			hButton = GetDlgItem(m_hTabDlg[2],IDC_BUTTON_CRIMINALREPORT);
			EnableWindow(hButton,FALSE);
		}
		else
		{
			hButton = GetDlgItem(m_hDlg,IDGGOM);
			EnableWindow(hButton,FALSE);

			hButton = GetDlgItem(m_hDlg,IDWHISPER);
			EnableWindow(hButton,FALSE);
		}

		cltMsg clMsg(GAMEMSG_REQUEST_GETHYUNGZOINFO,0,NULL);
		pclclient->SendMsgToServer((char*)&clMsg);
	}
	else
	{
		DestroyWindow(m_hDlg);
	}
}

void CHyungzoDlg::Show(CMinisterBase *pMini,SI16 stealnum,SI16 suspecttime,SI16 prisontime,
					   SI16 prisonerpenalty, HyungzoMember *pHyungzo)
{
	cltClient *pclclient = (cltClient*)pclClient;

	if ( m_bSet )
	{
		m_bSet = FALSE ;

		char* pTitle = GetTxtFromMgr(2045);
		char* pText = GetTxtFromMgr(2046);
		
		pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);
		return ;
	}

	pclClient->PushEffect(EFFECT_HYUNGZO,0,0);

	strcpy(m_szHyungzoID,pMini->m_szAccountID);
	strcpy(m_szHyungzoName,pMini->m_szPersonName);
	strcpy(m_szHyungzoProfile,pMini->m_szGreeting);

	m_siStealNum = stealnum ;

	char buffer[256] = "" ;

	HWND hWnd = GetDlgItem( m_hTabDlg[0], IDC_EDIT_LIMIT_STEAL );
	wsprintf(buffer,"%d",m_siStealNum);
	SetWindowText(hWnd,buffer);

	m_siSuspectTime = suspecttime ;

	hWnd = GetDlgItem( m_hTabDlg[0], IDC_EDIT_LIMIT_SUSPECTTIME );
	wsprintf(buffer,"%d",m_siSuspectTime);
	SetWindowText(hWnd,buffer);

	m_siPrisonTime = prisontime ;

	hWnd = GetDlgItem( m_hTabDlg[0], IDC_EDIT_LIMIT_SUSPECT_PRISONTIME );
	wsprintf(buffer,"%d",m_siPrisonTime);
	SetWindowText(hWnd,buffer);

	m_siPrisonerPenalty = prisonerpenalty ;

	switch(m_siPrisonerPenalty)
	{
	case 20 : CheckRadioButton(m_hTabDlg[0],IDC_RADIO_ONE,IDC_RADIO_FIVE,IDC_RADIO_ONE); break;
	case 40 : CheckRadioButton(m_hTabDlg[0],IDC_RADIO_ONE,IDC_RADIO_FIVE,IDC_RADIO_TWO); break;
	case 60 : CheckRadioButton(m_hTabDlg[0],IDC_RADIO_ONE,IDC_RADIO_FIVE,IDC_RADIO_THREE); break;
	case 80 : CheckRadioButton(m_hTabDlg[0],IDC_RADIO_ONE,IDC_RADIO_FIVE,IDC_RADIO_FOUR); break;
	case 100 : CheckRadioButton(m_hTabDlg[0],IDC_RADIO_ONE,IDC_RADIO_FIVE,IDC_RADIO_FIVE); break;
	}

	char* pText = GetTxtFromMgr(2047);
	wsprintf(buffer, pText,m_szHyungzoName);
	SetWindowText( m_hDlg ,buffer);
	SetDlgItemText( m_hDlg,IDC_EDIT_HYUNGZO, m_szHyungzoProfile );
	pText = GetTxtFromMgr(2048);
	strcat(buffer, pText);
	SetWindowText(m_hGroup,buffer);

	char ProfileURL[MAX_PATH];

/*	pclClient->GetCZProfileUrl( ProfileURL, m_szHyungzoID );

	hWnd = GetDlgItem( m_hDlg, IDC_STATIC_PROFILE );
	if(hWnd)	DisplayHTMLPage(hWnd, ProfileURL);*/

	// INTRO
	pclClient->GetCZIntroUrl( ProfileURL, m_szHyungzoID );

	hWnd = GetDlgItem(m_hDlg, IDC_STATIC_HYUNGZOINTRO);
	if(hWnd)	DisplayHTMLPage(hWnd, ProfileURL);

	ListView_DeleteAllItems(m_hLogList);

	SI32 listindex = 0 ;

	for ( int i = 0 ; i < MAX_PRISON_LIST_NUM ; i ++ )
	{
		if ( pHyungzo[i].siDate <= 0 ) continue ;
		if ( pHyungzo[i].siPrisonSecond <= 0 ) continue ;
		char buffer[256] = "" ;
		cltDate cldate;
		cldate.MakeFullDateFromDateVary(pHyungzo[i].siDate);
		if(cldate.GetDateText(buffer) == false) continue;
		ListViewInsertItem(m_hLogList, -1, listindex, 0, buffer, -1);
		ListViewInsertItem(m_hLogList, -1, listindex, 1, pHyungzo[i].szCharName, -1);

		SI32 minute = pHyungzo[i].siPrisonSecond / 60 ;
		char* pText = GetTxtFromMgr(1252);
		sprintf(buffer, pText,minute);
		ListViewInsertItem(m_hLogList, -1, listindex, 2, buffer, -1);
		ListViewInsertItem(m_hLogList, -1, listindex, 3, pHyungzo[i].szPrisonReason, -1);

		listindex ++ ;
	}

	ShowWindow( m_hDlg, SW_SHOW ) ;
	if ( m_siViewDlg == 0 )
		ShowWindow(m_hTabDlg[m_siViewDlg], SW_SHOW) ;
}

void CHyungzoDlg::ShowListDlg(HyungzoPoliceList* stHyungzoPoliceList)
{
}

void CHyungzoDlg::ShowListDlg(HyungzoCriminalList* stHyungzoCriminalList)
{
}