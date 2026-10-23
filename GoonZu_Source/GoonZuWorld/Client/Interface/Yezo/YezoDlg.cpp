// YezoDlg.cpp: implementation of the CYezoDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "../../Client/client.h"

#include "../../lib/WebHTML/WebHTML.h"

#include "../../common/HintDlg/HintDlg.h"

#include "../../CommonLogic/MsgType.h"
#include "../../CommonLogic/CommonLogic.h"
#include "../../CommonLogic/MsgType-System.h"
#include "../../CommonLogic/MsgType-Person.h"
#include "../../Client/Interface/ErrorMsgDlg/ErrorMsgDlg.h"
#include "../../Client/Interface/NotifyMsgDlg/NotifyMsgDlg.h"

#include "../../Client/Interface/SearchDetailDlg/SearchDetailDlg.h"

#include "../../Client/Music/Music.h"

#include "YezoDlg.h"

//-----------------------------
// Common
//-----------------------------
#include "PersonInfo.h"
#include "Char\CharManager\CharManager.h"

extern cltCommonLogic* pclClient;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CYezoDlg::CYezoDlg()
{
	m_hDlg = NULL ;
	m_hGroup = NULL ;
	m_bRectSwitch = FALSE ;
	m_siSword = 0 ;
	m_siGun = 0 ;
	m_siCommand = 0 ;
	m_siPorter = 0 ;
	m_bSet = FALSE ;
	m_hEditBrush = NULL ;
	m_hDlgBrush  = NULL;

	strcpy(m_szYezoName,"");
	strcpy(m_szYezoProfile,"");
	strcpy(m_szYezoID,"");

	m_siViewDlg = 0 ;

	m_hHitButton = NULL ;

	memset( m_stYezoAngelList, 0, sizeof(YezoAngelList) * MAX_YEZO_ANGELLIST );

	m_siSelectListInx = -1 ;
	m_siSelectList = 0 ;
	
	m_bRectListSwitch = FALSE ;
}

CYezoDlg::~CYezoDlg()
{
}

void CYezoDlg::Create(HINSTANCE hInst, HWND hParent )
{
	m_hInst = hInst ;
	m_hWnd = hParent ;
}

BOOL CALLBACK CYezoDlg::StatiCYezoDlgProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	cltClient *pclclient = (cltClient*)pclClient;
	return pclclient->m_pYezoDlg->YezoDlgProc( hDlg, iMsg, wParam, lParam );
}

BOOL CALLBACK CYezoDlg::YezoDlgProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam )
{
	cltClient *pclclient = (cltClient*)pclClient;

	switch( iMsg )
	{
	case WM_INITDIALOG:
		{
			m_hEditBrush = CreateSolidBrush( COLOR_DIALOG_YELLOW);
			m_hDlgBrush  = CreateSolidBrush( COLOR_DIALOG_LIGHTBLUE);
			if ( pclclient->pclCM->CR[1]->pclCI->clBI.uiGRank == GRANK_MINISTER3 )
				SetTimer(hDlg, 1, 1000, NULL);
			else
				SetTimer(hDlg, 2, 60000, NULL);

			HWND hwnd = NULL ;
			TC_ITEM item;
			RECT rect;

			hwnd = GetDlgItem(hDlg,IDC_TAB_YEZO);
			if(hwnd)
			{
				item.mask= TCIF_TEXT;
				item.iImage		= -1;
				char* pText = GetTxtFromMgr(2089);
				item.pszText	= pText;
				TabCtrl_InsertItem(hwnd,0,&item);

				pText = GetTxtFromMgr(2090);
				item.pszText	=	pText;
				TabCtrl_InsertItem(hwnd, 1, &item);

				pText = GetTxtFromMgr(2091);
				item.pszText	= pText;
				TabCtrl_InsertItem(hwnd, 2, &item);

				m_hTabDlg[0] = CreateDialog(m_hInst, MAKEINTRESOURCE(IDD_DIALOG_YEZO_JAPANWAR), hwnd, StatiCYezoJapanwarProc);
				GetClientRect(hwnd, &rect);
				TabCtrl_AdjustRect(hwnd, FALSE, &rect);
				MoveWindow(m_hTabDlg[0], rect.left, rect.top, rect.right-rect.left, rect.bottom- rect.top, TRUE);
				ShowWindow(m_hTabDlg[0], SW_HIDE) ;

				m_hTabDlg[1] = CreateDialog(m_hInst, MAKEINTRESOURCE(IDD_DIALOG_YEZO_ANGEL), hwnd, StatiCYezoAngelProc);
				GetClientRect(hwnd, &rect);
				TabCtrl_AdjustRect(hwnd, FALSE, &rect);
				MoveWindow(m_hTabDlg[1], rect.left, rect.top, rect.right-rect.left, rect.bottom- rect.top, TRUE);
				ShowWindow(m_hTabDlg[1], SW_HIDE) ;

				m_hTabDlg[2] = CreateDialog(m_hInst, MAKEINTRESOURCE(IDD_DIALOG_YEZO_ANGEL_APPLYER), hwnd, StatiCYezoAngelApplyerProc);
				GetClientRect(hwnd, &rect);
				TabCtrl_AdjustRect(hwnd, FALSE, &rect);
				MoveWindow(m_hTabDlg[2], rect.left, rect.top, rect.right-rect.left, rect.bottom- rect.top, TRUE);
				ShowWindow(m_hTabDlg[2], SW_HIDE) ;
			}
			
			m_siViewDlg = 0 ;

			//m_hHitButton = (HBITMAP)LoadImage( pclClient->GetInstance(), "interface/hitbutton/hozohint.bmp", IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE );
			//if ( m_hHitButton != NULL )
			//{
				//SendDlgItemMessage( hDlg, IDC_BUTTON_HOZO_EXPLAIN, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)m_hHitButton);
			//}
			//---------------------------------
			// HTML 을 초기화한다.
			//---------------------------------
			//==================================			
			hwnd = NULL ;

			hwnd = GetDlgItem(hDlg, IDC_STATIC_PROFILE);
			if ( hwnd )
			{
				EmbedBrowserObject(hwnd);
			}
			MoveWindow( GetDlgItem( hDlg, IDC_STATIC_PROFILE ), 9, 122, 73, 73, TRUE );

			hwnd = GetDlgItem(hDlg, IDC_STATIC_YEZOINTRO);
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
			case IDC_TAB_YEZO:
				{
					if ( lpNmhdr->code == TCN_SELCHANGE )
					{
						int index = TabCtrl_GetCurSel( GetDlgItem ( hDlg, IDC_TAB_YEZO ) );

						if ( index < 0 || index >= MAX_YEZO_TAB )
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

					HWND hWnd = GetDlgItem( m_hTabDlg[0], IDC_EDIT1 );
					GetWindowText( hWnd, buffer, 8 );
					m_siSword = atoi(buffer);
					hWnd = GetDlgItem( m_hTabDlg[0], IDC_EDIT2 );
					GetWindowText( hWnd, buffer, 8 );
					m_siGun = atoi(buffer);
					hWnd = GetDlgItem( m_hTabDlg[0], IDC_EDIT3 );
					GetWindowText( hWnd, buffer, 8 );
					m_siCommand = atoi(buffer);
					hWnd = GetDlgItem( m_hTabDlg[0], IDC_EDIT5 );
					GetWindowText( hWnd, buffer, 8 );
					m_siPorter = atoi(buffer);

					SI32 Healer = ( m_siSword + m_siGun + m_siCommand ) / 10 ;
					wsprintf(buffer,"%d",Healer);
					hWnd = GetDlgItem( m_hTabDlg[0], IDC_EDIT4 );
					SetWindowText(hWnd,buffer);

					GetDlgItemText(m_hDlg,IDC_EDIT_YEZO,buffer,256);
					SI32 limit = strlen(buffer);

					char* pText = GetTxtFromMgr(2092);
					wsprintf(buffer,pText,m_szYezoName,limit);
					SetWindowText(m_hGroup,buffer);
				}
				break;
			case 2:
				{
					cltMsg clMsg(GAMEMSG_REQUEST_GETYEZOINFO,0,NULL);
					pclclient->SendMsgToServer((char*)&clMsg);
				}
				break;
			}
		}
		break;
	case WM_CTLCOLORSTATIC:
		{
			if ( GetDlgItem( hDlg, IDC_EDIT_YEZO ) == (HWND)lParam )
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

				HWND hWnd = GetDlgItem( m_hTabDlg[0], IDC_EDIT1 );
				GetWindowText( hWnd, buffer, 8 );
				m_siSword = atoi(buffer);
				if ( m_siSword < 10 || m_siSword > 15000 )
				{
					char* pTitle = GetTxtFromMgr(2019);
					char* pText = GetTxtFromMgr(2093);
					pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);
					break;
				}
				hWnd = GetDlgItem( m_hTabDlg[0], IDC_EDIT2 );
				GetWindowText( hWnd, buffer, 8 );
				m_siGun = atoi(buffer);
				if ( m_siGun < 10 || m_siGun > 15000 )
				{
					char* pTitle = GetTxtFromMgr(2019);
					char* pText = GetTxtFromMgr(2094);
					pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);
					break;
				}
				hWnd = GetDlgItem( m_hTabDlg[0], IDC_EDIT3 );
				GetWindowText( hWnd, buffer, 8 );
				m_siCommand = atoi(buffer);
				if ( m_siCommand < 0 || m_siCommand > 1000 )
				{
					char* pTitle = GetTxtFromMgr(2019);
					char* pText = GetTxtFromMgr(2095);
					pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText(  pTitle, pText);
					break;
				}
				hWnd = GetDlgItem( m_hTabDlg[0], IDC_EDIT5 );
				GetWindowText( hWnd, buffer, 8 );
				m_siPorter = atoi(buffer);
				if ( m_siPorter < 0 || m_siPorter > 3000 )
				{
					char* pTitle = GetTxtFromMgr(2019);
					char* pText = GetTxtFromMgr(2096);
					pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText(  pTitle, pText);
					break;
				}

				char szVillage[MAX_PATH] = "" ;
				SI32 siVillageUnique = 0 ;
				ComboGetString(m_hTabDlg[0],IDC_COMBO_WARVILLAGE,(LPARAM)szVillage);

				if ( strcmp(szVillage,"") )
					siVillageUnique= pclclient->pclVillageManager->FindVillageUniqueFromName(szVillage);

				if ( siVillageUnique < 0 || siVillageUnique > CUR_VILLAGE_LIMIT )
				{
					char* pTitle = GetTxtFromMgr(1898);
					char* pText = GetTxtFromMgr(2097);
					pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText(  pTitle, pText);
					break;
				}

				GetDlgItemText(m_hDlg,IDC_EDIT_YEZO,m_szYezoProfile,256);
				SI32 limit = strlen(m_szYezoProfile);
				if ( limit > 0 && limit < 256 )
				{
					char* pTitle = GetTxtFromMgr(2098);
					char* pText = GetTxtFromMgr(2099);

					if ( siVillageUnique == 0 &&
						MessageBox(NULL,pTitle,
						pText, MB_ICONQUESTION | MB_YESNO) == IDNO )
							break;

					cltGameMsgRequest_SetYezoInfo clinfo(m_szYezoProfile,m_siSword,m_siGun,m_siCommand,m_siPorter,siVillageUnique);
					cltMsg clMsg(GAMEMSG_REQUEST_SETYEZOINFO, sizeof(clinfo), (char*)&clinfo);
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
				cltGameMsgRequest_Userid clUserid( m_szYezoName );
				cltMsg clMsg( GAMEMSG_REQUEST_USERID, sizeof( clUserid ), (char*)&clUserid );
				pclClient->SendMsgToServer((char*)&clMsg);
			}
			break;
		case IDWHISPER:
			{
				if ( strlen(m_szYezoName) > 0 && m_szYezoName != NULL )
					pclclient->RequestSecretChatByCharName(m_szYezoName);
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
			hwnd = GetDlgItem( hDlg, IDC_STATIC_YEZOINTRO );
			if( hwnd )
			{
				DoPageAction(hwnd, WEBPAGE_STOP);
				UnEmbedBrowserObject(hwnd);
			}

			GetWindowRect(m_hDlg, &m_rtDlg);
			m_bRectSwitch = TRUE ;

			if ( pclclient->pclCM->CR[1]->pclCI->clBI.uiGRank == GRANK_MINISTER3 )
				KillTimer(hDlg,1);
			else
				KillTimer(hDlg,2);

			if ( m_hHitButton )
			{
				DeleteObject(m_hHitButton);
				m_hHitButton = NULL ;
			}

			m_siViewDlg = 0 ;
			
			if ( m_hListDlg )
			{
				DestroyWindow(m_hListDlg);
				m_hListDlg = NULL ;
			}

			m_bSet = FALSE ;
			m_hDlg = NULL ;
		}
		break;
	}

	return FALSE;
}

BOOL CALLBACK CYezoDlg::StatiCYezoJapanwarProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	cltClient *pclclient = (cltClient*)pclClient;
	return pclclient->m_pYezoDlg->YezoJapanwarProc( hDlg, iMsg, wParam, lParam );
}

BOOL CALLBACK CYezoDlg::YezoJapanwarProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam )
{
	cltClient *pclclient = (cltClient*)pclClient;

	switch( iMsg )
	{
	case WM_INITDIALOG:
		{
		}
		break;
	case WM_DESTROY:
		{
		}
		break;
	}

	return FALSE;
}

BOOL CALLBACK CYezoDlg::StatiCYezoAngelProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	cltClient *pclclient = (cltClient*)pclClient;
	return pclclient->m_pYezoDlg->YezoAngelProc( hDlg, iMsg, wParam, lParam );
}

BOOL CALLBACK CYezoDlg::YezoAngelProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam )
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
		case IDC_BUTTON_GETANGELLIST: // 목록 불러오기
			{
				cltMsg clMsg(GAMEMSG_REQUEST_GETANGELLIST,0,NULL);
				pclclient->SendMsgToServer((char*)&clMsg);
			}
			break;
		case IDC_BUTTON_SETANGEL: // 임명하기
			{
				if ( pclclient->pclCM->CR[1]->pclCI->clBI.uiGRank == GRANK_MINISTER3 )
				{
					cltMsg clMsg(GAMEMSG_REQUEST_GETANGELAPPLYERLIST,0,NULL);
					pclclient->SendMsgToServer((char*)&clMsg);
				}
				else
				{
					char* pTitle = GetTxtFromMgr(2100);
					char* pText = GetTxtFromMgr(2101);
					pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);
				}
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

BOOL CALLBACK CYezoDlg::StatiCYezoAngelApplyerProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	cltClient *pclclient = (cltClient*)pclClient;
	return pclclient->m_pYezoDlg->YezoAngelApplyerProc( hDlg, iMsg, wParam, lParam );
}

BOOL CALLBACK CYezoDlg::YezoAngelApplyerProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam )
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
		case IDC_BUTTON_SETANGELAPPLYER_REG: // 신청하기
			{
				cltGameMsgRequest_SetAngelApplyer clinfo(true);
				cltMsg clMsg( GAMEMSG_REQUEST_SETANGELAPPLYER, sizeof( clinfo ), (char*)&clinfo );
				pclClient->SendMsgToServer((char*)&clMsg);
			}
			break;
		case IDC_BUTTON_SETANGELAPPLYER_ESC: // 취소하기
			{
				cltGameMsgRequest_SetAngelApplyer clinfo(false);
				cltMsg clMsg( GAMEMSG_REQUEST_SETANGELAPPLYER, sizeof( clinfo ), (char*)&clinfo );
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

BOOL CALLBACK CYezoDlg::StatiCYezoAngelListProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	cltClient *pclclient = (cltClient*)pclClient;
	return pclclient->m_pYezoDlg->YezoAngelistProc( hDlg, iMsg, wParam, lParam );
}

BOOL CALLBACK CYezoDlg::YezoAngelistProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam )
{
	cltClient *pclclient = (cltClient*)pclClient;

	switch( iMsg )
	{
	case WM_INITDIALOG:
		{
			m_hAngelList = GetDlgItem( hDlg, IDC_LIST_ANGEL_USER );

			if(m_hAngelList)
			{
				ListView_SetExtendedListViewStyle( m_hAngelList, LVS_EX_FULLROWSELECT | LVS_EX_ONECLICKACTIVATE );
				ListView_SetHoverTime( m_hAngelList, 10 );

				char* pText = GetTxtFromMgr(1556);
				ListViewInsertColumn( m_hAngelList, 130, pText, 0, 1);
				pText = GetTxtFromMgr(1506);
				ListViewInsertColumn( m_hAngelList, 60, pText, 1, 1);
				pText = GetTxtFromMgr(2102);
				ListViewInsertColumn( m_hAngelList, 60, pText, 2, 1);

				ListView_DeleteAllItems(m_hAngelList);
			}
		}
		break;
	case WM_CLOSE:
		{
			DestroyWindow(m_hListDlg);
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

			if ( hdr->hwndFrom == m_hAngelList )
			{
				switch( hdr->code )
				{
				case LVN_ITEMCHANGED:
					{
						if ( nia->iItem >= 0 && nia->iItem < MAX_YEZO_ANGELLIST )
						{
							if ( nlv->uChanged == LVIF_STATE && nlv->uNewState == ( LVIS_SELECTED | LVIS_FOCUSED ))
							{
								if ( m_stYezoAngelList[m_siListIndex[nlv->iItem]].siLevel <= 0 ) break;
								if ( m_stYezoAngelList[m_siListIndex[nlv->iItem]].siLevel > MAX_LEVEL ) break;
								if ( m_stYezoAngelList[m_siListIndex[nlv->iItem]].siPersonID <= 0 ) break;

								m_siSelectListInx = m_siListIndex[nlv->iItem] ;
							}
						}
					}
					break;
				}
			}
		}
		break;
	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case IDC_BUTTON_ANGEL_APPOINTMENT: // 임명하기
			{
				if ( m_siSelectList != 1 ) break ;
				if ( m_siSelectListInx < 0 || m_siSelectListInx > MAX_YEZO_ANGELLIST ) break;
				cltGameMsgRequest_SetAngel clinfo(m_siSelectListInx,true);
				cltMsg clMsg( GAMEMSG_REQUEST_SETANGEL, sizeof( clinfo ), (char*)&clinfo );
				pclClient->SendMsgToServer((char*)&clMsg);
			}
			break;
		case IDC_BUTTON_ANGEL_DELETE: // 삭제하기
			{
				switch(m_siSelectList)
				{
				case 1:
					{
						if ( m_siSelectListInx < 0 || m_siSelectListInx > MAX_YEZO_ANGELLIST ) break;
						cltGameMsgRequest_SetAngelApplyer clinfo(false,m_siSelectListInx);
						cltMsg clMsg( GAMEMSG_REQUEST_SETANGELAPPLYER, sizeof( clinfo ), (char*)&clinfo );
						pclClient->SendMsgToServer((char*)&clMsg);
					}
					break;
				case 2:
					{
						if ( pclclient->pclCM->CR[1]->pclCI->clBI.uiGRank == GRANK_MINISTER3 )
						{
							if ( m_siSelectListInx < 0 || m_siSelectListInx > MAX_YEZO_ANGELLIST ) break;
							cltGameMsgRequest_SetAngel clinfo(m_siSelectListInx,false);
							cltMsg clMsg( GAMEMSG_REQUEST_SETANGEL, sizeof( clinfo ), (char*)&clinfo );
							pclClient->SendMsgToServer((char*)&clMsg);
						}
						else if ( pclclient->pclCM->CR[1]->pclCI->clBI.uiAngelSwitch )
						{
							cltGameMsgRequest_SetAngel clinfo(-1,false);
							cltMsg clMsg( GAMEMSG_REQUEST_SETANGEL, sizeof( clinfo ), (char*)&clinfo );
							pclClient->SendMsgToServer((char*)&clMsg);
						}
					}
					break;
				case 3:
				default:
					break;
				}
			}
			break;
		case IDC_BUTTON_GGOM: // 꼼지가기
			{
				if ( m_siSelectListInx < 0 || m_siSelectListInx > MAX_YEZO_ANGELLIST ) break;
				cltGameMsgRequest_Userid clUserid( m_stYezoAngelList[m_siSelectListInx].szName );
				cltMsg clMsg( GAMEMSG_REQUEST_USERID, sizeof( clUserid ), (char*)&clUserid );
				pclClient->SendMsgToServer((char*)&clMsg);
			}
			break;
		}
		break;
	case WM_DESTROY:
		{
			m_siSelectListInx = -1 ;
			m_siSelectList = 0 ;

			m_bRectListSwitch = TRUE ;
			GetWindowRect(m_hListDlg, &m_rtListDlg);
		}
		break;
	}

	return FALSE;
}

// index : 1 -> 수호천사 임명(예조판서), 2 -> 수호천사 목록(민간인?), 3 -> 수호천사 신청자 목록
bool CYezoDlg::SetListDlg(SI32 index)
{
	cltClient *pclclient = (cltClient*)pclClient;

	if ( IsWindow(m_hListDlg) )
		DestroyWindow(m_hListDlg);

	m_hListDlg = CreateDialog( m_hInst, MAKEINTRESOURCE( IDD_DIALOG_YEZO_APPOINTMENT ), m_hWnd, StatiCYezoAngelListProc );

	if ( m_bRectListSwitch )
	{
		MoveWindow(m_hListDlg, 
			m_rtListDlg.left, m_rtListDlg.top, 
			m_rtListDlg.right - m_rtListDlg.left, 
			m_rtListDlg.bottom - m_rtListDlg.top, FALSE);
	}

	m_siSelectList = index ;

	switch(index)
	{
	case 1:
		{
			if ( pclclient->pclCM->CR[1]->pclCI->clBI.uiGRank != GRANK_MINISTER3 )
				return false ;
			
			char* pText = GetTxtFromMgr(2103);
			SetWindowText(m_hListDlg, pText);
		}
		break;
	case 2:
		{
			HWND hwnd = NULL ;

			if ( pclclient->pclCM->CR[1]->pclCI->clBI.uiGRank != GRANK_MINISTER3 )
			{
				hwnd = GetDlgItem(m_hListDlg,IDC_BUTTON_ANGEL_APPOINTMENT);
				EnableWindow(hwnd,FALSE) ;

				hwnd = GetDlgItem(m_hListDlg,IDC_BUTTON_ANGEL_DELETE);
				char* pText = GetTxtFromMgr(2104);
				if ( pclclient->pclCM->CR[1]->pclCI->clBI.uiAngelSwitch )
					SetWindowText(hwnd, pText);
				else
					EnableWindow(hwnd,FALSE) ;
			}
			else
			{
				hwnd = GetDlgItem(m_hListDlg,IDC_BUTTON_ANGEL_APPOINTMENT);
				EnableWindow(hwnd , FALSE );
			}
			char* pText = GetTxtFromMgr(2105);
			SetWindowText(m_hListDlg,pText);
		}
		break;
	default:
		return false ;
		break;
	}

	ShowWindow(m_hListDlg,SW_SHOW);

	return true ;
}

void CYezoDlg::ShowListDlg(YezoAngelList yezoangellist [])
{
	ListView_DeleteAllItems(m_hAngelList);

	SI32 listindex = 0 ;

	memcpy(m_stYezoAngelList,yezoangellist,sizeof(YezoAngelList) * MAX_YEZO_ANGELLIST);

	for ( int i = 0 ; i < MAX_YEZO_ANGELLIST ; i ++ )
	{
		m_siListIndex[i] = -1 ;
		if ( yezoangellist[i].siLevel <= 0 ) continue ;
		if ( yezoangellist[i].siPersonID <= 0 ) continue ;
		if ( !strcmp(yezoangellist[i].szAccountID,"") ) continue ;
		if ( !strcmp(yezoangellist[i].szName,"") ) continue ;

		char buffer[256] = "" ;

		ListViewInsertItem(m_hAngelList, -1, listindex, 0, yezoangellist[i].szName, -1);

		sprintf(buffer,"%d",yezoangellist[i].siLevel ) ;
		ListViewInsertItem(m_hAngelList, -1, listindex, 1, buffer, -1);

		sprintf(buffer,"%d",yezoangellist[i].siFameLevel ) ;
		ListViewInsertItem(m_hAngelList, -1, listindex, 2, buffer, -1);

		m_siListIndex[listindex] = i ;
		listindex ++ ;
	}
}

void CYezoDlg::Set()
{
	if ( !IsWindow(m_hDlg) )
	{
		m_hDlg = CreateDialog( m_hInst, MAKEINTRESOURCE( IDD_DIALOG_YEZO ), m_hWnd, StatiCYezoDlgProc );
		SendDlgItemMessage( m_hDlg, IDC_EDIT_YEZO, EM_LIMITTEXT, 255, 0 );
		m_hGroup = GetDlgItem(m_hDlg,IDC_STATIC_YEZOPROFILE);

		if ( m_bRectSwitch )
		{
			MoveWindow(m_hDlg, 
				m_rtDlg.left, m_rtDlg.top, 
				m_rtDlg.right - m_rtDlg.left, 
				m_rtDlg.bottom - m_rtDlg.top, FALSE);
		}

		HWND hButton = NULL ;
		cltClient *pclclient = (cltClient*)pclClient;
		if ( pclclient->pclCM->CR[1]->pclCI->clBI.uiGRank != GRANK_MINISTER3 )
		{
			//hButton = GetDlgItem(m_hDlg,IDSETUP);
			//EnableWindow(hButton,FALSE);

			hButton = GetDlgItem(m_hDlg,IDC_EDIT_YEZO);
			EnableWindow(hButton,FALSE);

			hButton = GetDlgItem(m_hDlg,IDOK);
			EnableWindow(hButton,FALSE);

			hButton = GetDlgItem(m_hTabDlg[0],IDC_EDIT1);
			EnableWindow(hButton,FALSE);
			hButton = GetDlgItem(m_hTabDlg[0],IDC_EDIT2);
			EnableWindow(hButton,FALSE);
			hButton = GetDlgItem(m_hTabDlg[0],IDC_EDIT3);
			EnableWindow(hButton,FALSE);
			hButton = GetDlgItem(m_hTabDlg[0],IDC_EDIT5);
			EnableWindow(hButton,FALSE);
			hButton = GetDlgItem(m_hTabDlg[0],IDC_COMBO_WARVILLAGE);
			EnableWindow(hButton,FALSE);
		}
		else
		{
			hButton = GetDlgItem(m_hDlg,IDGGOM);
			EnableWindow(hButton,FALSE);

			hButton = GetDlgItem(m_hDlg,IDWHISPER);
			EnableWindow(hButton,FALSE);
		}

		cltMsg clMsg(GAMEMSG_REQUEST_GETYEZOINFO,0,NULL);
		pclclient->SendMsgToServer((char*)&clMsg);
	}
	else
	{
		DestroyWindow(m_hDlg);
	}
}

void CYezoDlg::Show(CMinisterBase *pMini,SI32 sword,SI32 gun,SI32 command,SI32 porter,SI32 villageunique)
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

	pclClient->PushEffect(EFFECT_YEZO,0,0);

	strcpy(m_szYezoID,pMini->m_szAccountID);
	strcpy(m_szYezoName,pMini->m_szPersonName);
	strcpy(m_szYezoProfile,pMini->m_szGreeting);

	m_siSword = sword;
	m_siGun = gun;
	m_siCommand = command;

	char buffer[256] = "";
	SI32 healer = 0 ;
	HWND hWnd = GetDlgItem( m_hTabDlg[0], IDC_EDIT1 );
	wsprintf(buffer,"%d",m_siSword);
	SetWindowText(hWnd,buffer);
	hWnd = GetDlgItem( m_hTabDlg[0], IDC_EDIT2 );
	wsprintf(buffer,"%d",m_siGun);
	SetWindowText(hWnd,buffer);
	hWnd = GetDlgItem( m_hTabDlg[0], IDC_EDIT3 );
	wsprintf(buffer,"%d",m_siCommand);
	SetWindowText(hWnd,buffer);
	hWnd = GetDlgItem( m_hTabDlg[0], IDC_EDIT4 );
	healer = ( m_siSword + m_siGun + m_siCommand ) / 10 ;
	wsprintf(buffer,"%d",healer);
	SetWindowText(hWnd,buffer);
	hWnd = GetDlgItem( m_hTabDlg[0], IDC_EDIT5 );
	wsprintf(buffer,"%d",porter);
	SetWindowText(hWnd,buffer);

	char* pText = GetTxtFromMgr(2106);
    wsprintf(buffer, pText,m_szYezoName);
	SetWindowText( m_hDlg ,buffer);
	SetDlgItemText( m_hDlg,IDC_EDIT_YEZO, m_szYezoProfile );
	pText = GetTxtFromMgr(2048);
	strcat(buffer, pText);
	SetWindowText(m_hGroup,buffer);

	char ProfileURL[MAX_PATH];

/*	pclClient->GetCZProfileUrl( ProfileURL, m_szYezoID );

	hWnd = GetDlgItem( m_hDlg, IDC_STATIC_PROFILE );
	if(hWnd)	DisplayHTMLPage(hWnd, ProfileURL);*/

	// INTRO
	
	pclClient->GetCZIntroUrl( ProfileURL, m_szYezoID );
	
	hWnd = GetDlgItem(m_hDlg, IDC_STATIC_YEZOINTRO);
	if(hWnd)	DisplayHTMLPage(hWnd, ProfileURL);

	SI32 siVillageUnique = villageunique ;

	if ( siVillageUnique <= 0 || siVillageUnique > CUR_VILLAGE_LIMIT )
		siVillageUnique = 0 ;

	SetVillageCombo();

	SendMessage( GetDlgItem( m_hTabDlg[0], IDC_COMBO_WARVILLAGE ), CB_SELECTSTRING, 0, (LPARAM)pclClient->pclVillageManager->GetName(siVillageUnique) );

	ShowWindow( m_hDlg, SW_SHOW );
	if ( m_siViewDlg == 0 )
		ShowWindow(m_hTabDlg[m_siViewDlg], SW_SHOW) ;
}

void CYezoDlg::SetVillageCombo()
{
	cltClient *pclclient = (cltClient*)pclClient;

	char buffer[256] = "";

	ComboDelete(m_hTabDlg[0],IDC_COMBO_WARVILLAGE);

	for ( SI32 i = 0 ; i <= CUR_VILLAGE_LIMIT ; i++ )
	{
		wsprintf(buffer,"%s",pclclient->pclVillageManager->GetName(i));

		if ( !strcmp(buffer,"") ) continue;

		ComboAddString(m_hTabDlg[0],IDC_COMBO_WARVILLAGE,(LPARAM)buffer);
	}
}