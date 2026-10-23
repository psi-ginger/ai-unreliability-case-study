// HozoDlg.cpp: implementation of the CHozoDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "../../Client/client.h"

#include "../../Client/Music/Music.h"

#include "../../lib/WebHTML/WebHTML.h"

//-----------------------------
// Common
//-----------------------------
#include "PersonInfo.h"
#include "../../common/HintDlg/HintDlg.h"
#include "Char/CharManager/CharManager.h"

//-----------------------------
// CommonLogic
//-----------------------------
#include "../../CommonLogic/MsgType.h"
#include "../../CommonLogic/CommonLogic.h"
#include "../../CommonLogic/MsgType-System.h"
#include "MsgType-Person.h"


#include "../../Client/Interface/ErrorMsgDlg/ErrorMsgDlg.h"
#include "../../Client/Interface/NotifyMsgDlg/NotifyMsgDlg.h"

#include "../../Client/Interface/SearchDetailDlg/SearchDetailDlg.h"

#include "../../Server/Minister/Minister-Hozo.h"
#include "NInterface/NHintDlg/NHintDlg.h"

#include "HozoDlg.h"

extern cltCommonLogic* pclClient;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CHozoDlg::CHozoDlg()
{
	m_hDlg = NULL ;
	m_hGroup = NULL ;
	m_bRectSwitch = FALSE ;

	m_bSet = FALSE ;
	m_hEditBrush = NULL ;
	m_hDlgBrush  = NULL;

	strcpy(m_szHozoName,"");
	strcpy(m_szHozoProfile,"");
	strcpy(m_szHozoID,"");

	m_siPrizeMoney = 0 ;
	m_siPrizeSetTime = 0 ;
	m_siFixationMoney = 0 ;

	m_siViewDlg = 0 ;

	m_hHitButton = NULL ;
	m_hChangedNameList = NULL ;
}

CHozoDlg::~CHozoDlg()
{
}

void CHozoDlg::Create(HINSTANCE hInst, HWND hParent )
{
	m_hInst = hInst ;
	m_hWnd = hParent ;
}

BOOL CALLBACK CHozoDlg::StatiCHozoDlgProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	cltClient *pclclient = (cltClient*)pclClient;
	return pclclient->m_pHozoDlg->HozoDlgProc( hDlg, iMsg, wParam, lParam );
}

BOOL CALLBACK CHozoDlg::HozoDlgProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam )
{
	cltClient *pclclient = (cltClient*)pclClient;

	switch( iMsg )
	{
	case WM_INITDIALOG:
		{
			m_hEditBrush = CreateSolidBrush( COLOR_DIALOG_YELLOW );
			m_hDlgBrush  = CreateSolidBrush( COLOR_DIALOG_LIGHTBLUE);
			if ( pclclient->pclCM->CR[1]->pclCI->clBI.uiGRank == GRANK_MINISTER2 )
				SetTimer(hDlg, 1, 1000, NULL);
			else
				SetTimer(hDlg, 2, 60000, NULL);

			HWND hwnd = NULL ;
			TC_ITEM item;
			RECT rect;

			hwnd = GetDlgItem(hDlg,IDC_TAB_HOZO);
			if(hwnd)
			{
				item.mask= TCIF_TEXT;
				item.iImage		= -1;
				char* pText = GetTxtFromMgr(2069);
				item.pszText	= pText;
				TabCtrl_InsertItem(hwnd,0,&item);

				pText = GetTxtFromMgr(2070);
				item.pszText	=	pText;
				TabCtrl_InsertItem(hwnd, 1, &item);

				pText = GetTxtFromMgr(2071);
				item.pszText	=	pText;
				TabCtrl_InsertItem(hwnd, 2, &item);

				pText = GetTxtFromMgr(2072);
				item.pszText	=	pText;
				TabCtrl_InsertItem(hwnd, 3, &item);

				pText = GetTxtFromMgr(2073);
				item.pszText	=	pText;
				TabCtrl_InsertItem(hwnd, 4, &item);

				m_hTabDlg[0] = CreateDialog(m_hInst, MAKEINTRESOURCE(IDD_DIALOG_HOZO_RESIDENT), hwnd, StatiCHozoResidentProc);
				GetClientRect(hwnd, &rect);
				TabCtrl_AdjustRect(hwnd, FALSE, &rect);
				MoveWindow(m_hTabDlg[0], rect.left, rect.top, rect.right-rect.left, rect.bottom- rect.top, TRUE);
				ShowWindow(m_hTabDlg[0], SW_HIDE) ;

				m_hTabDlg[1] = CreateDialog(m_hInst, MAKEINTRESOURCE(IDD_DIALOG_HOZO_PRIZE), hwnd, StatiCHozoPrizeProc);
				GetClientRect(hwnd, &rect);
				TabCtrl_AdjustRect(hwnd, FALSE, &rect);
				MoveWindow(m_hTabDlg[1], rect.left, rect.top, rect.right-rect.left, rect.bottom- rect.top, TRUE);
				ShowWindow(m_hTabDlg[1], SW_HIDE) ;

				m_hTabDlg[2] = CreateDialog(m_hInst, MAKEINTRESOURCE(IDD_DIALOG_HOZO_FIXATION), hwnd, StatiCHozoFixationProc);
				GetClientRect(hwnd, &rect);
				TabCtrl_AdjustRect(hwnd, FALSE, &rect);
				MoveWindow(m_hTabDlg[2], rect.left, rect.top, rect.right-rect.left, rect.bottom- rect.top, TRUE);
				ShowWindow(m_hTabDlg[2], SW_HIDE) ;

				m_hTabDlg[3] = CreateDialog(m_hInst, MAKEINTRESOURCE(IDD_DIALOG_HOZO_CHANGEDNAME), hwnd, StatiCHozoChangedNameListProc);
				GetClientRect(hwnd, &rect);
				TabCtrl_AdjustRect(hwnd, FALSE, &rect);
				MoveWindow(m_hTabDlg[3], rect.left, rect.top, rect.right-rect.left, rect.bottom- rect.top, TRUE);
				ShowWindow(m_hTabDlg[3], SW_HIDE) ;

				m_hTabDlg[4] = CreateDialog(m_hInst, MAKEINTRESOURCE(IDD_DIALOG_HOZO_NEWCHARRATE), hwnd, StatiCHozoNewCharRateProc);
				GetClientRect(hwnd, &rect);
				TabCtrl_AdjustRect(hwnd, FALSE, &rect);
				MoveWindow(m_hTabDlg[4], rect.left, rect.top, rect.right-rect.left, rect.bottom- rect.top, TRUE);
				ShowWindow(m_hTabDlg[4], SW_HIDE) ;
			}
			
			m_siViewDlg = 0 ;

			m_hHitButton = (HBITMAP)LoadImage( pclClient->GetInstance(), "interface/hitbutton/hozohint.bmp", IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE );
			if ( m_hHitButton != NULL )
			{
				SendDlgItemMessage( hDlg, IDC_BUTTON_HOZO_EXPLAIN, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)m_hHitButton);
			}
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

			hwnd = GetDlgItem(hDlg, IDC_STATIC_HOZOINTRO);
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
					GetDlgItemText(m_hDlg,IDC_EDIT_HOZO,buffer,256);
					SI32 limit = strlen(buffer);

					char* pText = GetTxtFromMgr(2074);
					wsprintf(buffer, pText,m_szHozoName,limit);
					SetWindowText(m_hGroup,buffer);
				}
				break;
			case 2:
				{
					cltMsg clMsg(GAMEMSG_REQUEST_GETHOZOINFO,0,NULL);
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
			case IDC_TAB_HOZO:
				{
					if ( lpNmhdr->code == TCN_SELCHANGE )
					{
						int index = TabCtrl_GetCurSel( GetDlgItem ( hDlg, IDC_TAB_HOZO ) );

						if ( index < 0 || index >= MAX_HOZO_TAB )
						{
							return FALSE;
						}

						ShowWindow(m_hTabDlg[m_siViewDlg],SW_HIDE);
						ShowWindow(m_hTabDlg[index],SW_SHOW);

						m_siViewDlg = index ;

						if ( m_siViewDlg == 3 )
						{
							// 리스트를 불러온다.
							cltGameMsgRequest_GetChangedName clinfo(1);
							cltMsg clMsg( GAMEMSG_REQUEST_GETCHANGEDNAME, sizeof( clinfo ), (char*)&clinfo );
							pclClient->SendMsgToServer((char*)&clMsg);
						}
					}
				}
				break;
			}
		}
		break;
	case WM_CTLCOLORSTATIC:
		{
			if ( GetDlgItem( hDlg, IDC_EDIT_HOZO ) == (HWND)lParam )
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
		case IDC_BUTTON_HOZO_EXPLAIN:
			{
				//pclclient->m_pHintDlg->Show("hozo");
				if((pclclient->m_pDialog[ NHINT_DLG ])==NULL)
				{
					pclclient->CreateInterface( NHINT_DLG );
				}
				((NHintDlgExplain*)pclclient->m_pDialog[ NHINT_DLG])->Show("hozo");
			}
			break;
		case IDOK:
			{
				char buffer[8] = "" ;

				HWND hWnd = GetDlgItem( m_hTabDlg[0], IDC_EDIT_RESIDENT );
				GetWindowText( hWnd, buffer, 8 );
				SI32 residentnum = atoi(buffer);
				if ( residentnum < 100 || residentnum > 150 )
				{
					char* pTitle = GetTxtFromMgr(2019);
					char* pText = GetTxtFromMgr(2075);
					pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);
					break;
				}

				hWnd = GetDlgItem( m_hTabDlg[0], IDC_EDIT_MAX_SHOP );
				GetWindowText( hWnd, buffer, 8 );
				SI32 maxshop = atoi(buffer);
				if ( maxshop < 0 || maxshop > 40 )
				{
					char* pTitle = GetTxtFromMgr(2019);
					char* pText = GetTxtFromMgr(2075);
					pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);
					break;
				}

				hWnd = GetDlgItem(m_hTabDlg[1],IDC_EDIT_PRIZE_MONEY);
				GetWindowText( hWnd, buffer, 8 );
				SI32 prizemoney = atoi(buffer) / UNIT_HOZO_SETMONEY ;
				if ( TABS( m_siPrizeMoney - prizemoney ) > 1 )
				{
					char moneybuf[256] = "" ;
					char errorbuf[256] = "" ;

                    char* pText = GetTxtFromMgr(611);
					g_JYLibFunction.SetNumTxt(UNIT_HOZO_SETMONEY,moneybuf, pText);
					pText = GetTxtFromMgr(2077);
					sprintf(errorbuf, pText,moneybuf);
					pText = GetTxtFromMgr(2078);
					pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pText,errorbuf);
					break;
				}
				if ( prizemoney < MIN_HOZO_PRIZEMONEY )
				{
					char moneybuf1[256] = "" ;
					char moneybuf2[256] = "" ;
					char errorbuf[256] = "" ;

					char* pText = GetTxtFromMgr(611);
					g_JYLibFunction.SetNumTxt(MIN_HOZO_PRIZEMONEY * UNIT_HOZO_SETMONEY,moneybuf1, pText);
					g_JYLibFunction.SetNumTxt(MAX_HOZO_PRIZEMONEY * UNIT_HOZO_SETMONEY,moneybuf2, pText);
					pText = GetTxtFromMgr(2079);
					sprintf(errorbuf, pText,moneybuf1,moneybuf2);
					pText = GetTxtFromMgr(2019);
					pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pText,errorbuf);
					break;
				}

				hWnd = GetDlgItem(m_hTabDlg[2],IDC_EDIT_FIXATION_MONEY);
				GetWindowText( hWnd, buffer, 8 );
				SI32 fixationmoney = atoi(buffer) / UNIT_HOZO_SETMONEY ;
				if ( TABS( m_siFixationMoney - fixationmoney ) > 1 )
				{
					char moneybuf[256] = "" ;
					char errorbuf[256] = "" ;

					char* pText = GetTxtFromMgr(611);
					g_JYLibFunction.SetNumTxt(UNIT_HOZO_SETMONEY,moneybuf, pText);
					pText = GetTxtFromMgr(2080);
					sprintf(errorbuf, pText,moneybuf);
					pText = GetTxtFromMgr(2078);
					pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pText,errorbuf);
					break;
				}
				if ( fixationmoney < MIN_HOZO_FIXATIONMONEY )
				{
					char moneybuf1[256] = "" ;
					char moneybuf2[256] = "" ;
					char errorbuf[256] = "" ;

					char* pText = GetTxtFromMgr(611);
					g_JYLibFunction.SetNumTxt(MIN_HOZO_FIXATIONMONEY * UNIT_HOZO_SETMONEY,moneybuf1,pText);
					g_JYLibFunction.SetNumTxt(MAX_HOZO_FIXATIONMONEY * UNIT_HOZO_SETMONEY,moneybuf2,pText);
					pText = GetTxtFromMgr(2081);
					sprintf(errorbuf, pText,moneybuf1,moneybuf2);
					pText = GetTxtFromMgr(2063);
					pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pText,errorbuf);
					break;
				}

				cltClient *pclclient = (cltClient*)pclClient;
				if ( pclclient->clClientDate.GetDateVary() <= m_siPrizeSetTime + 30 )
				{
					char* pTitle = GetTxtFromMgr(2063);
					char* pText = GetTxtFromMgr(2062);
					pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);
					break;
				}

				GetDlgItemText(m_hDlg,IDC_EDIT_HOZO,m_szHozoProfile,256);
				SI32 limit = strlen(m_szHozoProfile);
				if ( limit > 0 && limit < 256 )
				{
					cltGameMsgRequest_SetHozoInfo clinfo(m_szHozoProfile,residentnum,maxshop,
														 prizemoney,fixationmoney);
					cltMsg clMsg(GAMEMSG_REQUEST_SETHOZOINFO, sizeof(clinfo), (char*)&clinfo);
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
				cltGameMsgRequest_Userid clUserid( m_szHozoName );
				cltMsg clMsg( GAMEMSG_REQUEST_USERID, sizeof( clUserid ), (char*)&clUserid );
				pclClient->SendMsgToServer((char*)&clMsg);
			}
			break;
		case IDWHISPER:
			{
				if ( strlen(m_szHozoName) > 0 && m_szHozoName != NULL )
					pclclient->RequestSecretChatByCharName(m_szHozoName);
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
			hwnd = GetDlgItem( hDlg, IDC_STATIC_HOZOINTRO );
			if( hwnd )
			{
				DoPageAction(hwnd, WEBPAGE_STOP);
				UnEmbedBrowserObject(hwnd);
			}

			GetWindowRect(m_hDlg, &m_rtDlg);
			m_bRectSwitch = TRUE ;

			if ( pclclient->pclCM->CR[1]->pclCI->clBI.uiGRank == GRANK_MINISTER2 )
				KillTimer(hDlg,1);
			else
				KillTimer(hDlg,2);

			if ( m_hHitButton )
			{
				DeleteObject(m_hHitButton);
				m_hHitButton = NULL ;
			}

			m_siPrizeMoney = 0 ;
			m_siPrizeSetTime = 0 ;
			m_siFixationMoney = 0 ;

			m_siViewDlg = 0 ;

			m_bSet = FALSE ;
			m_hDlg = NULL ;
		}
		break;
	}

	return FALSE;
}

BOOL CALLBACK CHozoDlg::StatiCHozoResidentProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	cltClient *pclclient = (cltClient*)pclClient;
	return pclclient->m_pHozoDlg->HozoResidentProc( hDlg, iMsg, wParam, lParam );
}

BOOL CALLBACK CHozoDlg::HozoResidentProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam )
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

BOOL CALLBACK CHozoDlg::StatiCHozoPrizeProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	cltClient *pclclient = (cltClient*)pclClient;
	return pclclient->m_pHozoDlg->HozoPrizeProc( hDlg, iMsg, wParam, lParam );
}

BOOL CALLBACK CHozoDlg::HozoPrizeProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam )
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

BOOL CALLBACK CHozoDlg::StatiCHozoFixationProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	cltClient *pclclient = (cltClient*)pclClient;
	return pclclient->m_pHozoDlg->HozoFixationProc( hDlg, iMsg, wParam, lParam );
}

BOOL CALLBACK CHozoDlg::HozoFixationProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam )
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

BOOL CALLBACK CHozoDlg::StatiCHozoChangedNameListProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	cltClient *pclclient = (cltClient*)pclClient;
	return pclclient->m_pHozoDlg->HozoChangedNameListProc( hDlg, iMsg, wParam, lParam );
}

BOOL CALLBACK CHozoDlg::HozoChangedNameListProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam )
{
	cltClient *pclclient = (cltClient*)pclClient;

	switch( iMsg )
	{
	case WM_INITDIALOG:
		{
			m_hChangedNameList = GetDlgItem( hDlg, IDC_LIST_HOZO_CHANGEDNAME );

			if(m_hChangedNameList)
			{
				ListView_SetExtendedListViewStyle( m_hChangedNameList, LVS_EX_FULLROWSELECT | LVS_EX_ONECLICKACTIVATE );
				ListView_SetHoverTime( m_hChangedNameList, 10 );

				char* pText = GetTxtFromMgr(1564);
				ListViewInsertColumn( m_hChangedNameList, 90, pText, 0, 1);
				pText = GetTxtFromMgr(2082);
				ListViewInsertColumn( m_hChangedNameList, 90, pText, 1, 1);
				pText = GetTxtFromMgr(2083);
				ListViewInsertColumn( m_hChangedNameList, 90, pText, 2, 1);

				ListView_DeleteAllItems(m_hChangedNameList);
				
			}

			m_siListPage = 1 ;
			char buffer[256] = "" ;
			char* pText = GetTxtFromMgr(2084);
			sprintf(buffer, pText,m_siListPage) ;
			SetWindowText(GetDlgItem(hDlg,IDC_STATIC_PAGE),buffer);
		}
		break;
	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case IDC_BUTTON_PAGE_LEFT:
			{
				if ( m_siListPage <= 1 ) break ;
				cltGameMsgRequest_GetChangedName clinfo(m_siListPage-1);
				cltMsg clMsg( GAMEMSG_REQUEST_GETCHANGEDNAME, sizeof( clinfo ), (char*)&clinfo );
				pclClient->SendMsgToServer((char*)&clMsg);
			}
			break;
		case IDC_BUTTON_PAGE_RIGHT:
			{
				cltGameMsgRequest_GetChangedName clinfo(m_siListPage+1);
				cltMsg clMsg( GAMEMSG_REQUEST_GETCHANGEDNAME, sizeof( clinfo ), (char*)&clinfo );
				pclClient->SendMsgToServer((char*)&clMsg);
			}
			break;
		}
	case WM_DESTROY:
		{
		}
		break;
	}

	return FALSE;
}

BOOL CALLBACK CHozoDlg::StatiCHozoNewCharRateProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	cltClient *pclclient = (cltClient*)pclClient;
	return pclclient->m_pHozoDlg->HozoNewCharRateProc( hDlg, iMsg, wParam, lParam );
}

BOOL CALLBACK CHozoDlg::HozoNewCharRateProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam )
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

void CHozoDlg::ShowChangedNameList(SI32 page , ChangedNameChar changednamechar [])
{
	if ( page <= 0 ) return ;
	if ( !IsWindow(m_hDlg) ) return ;

	m_siListPage = page ;

	char buffer[256] = "" ;
	char* pText = GetTxtFromMgr(2084);
	sprintf(buffer, pText,page) ;
	SetWindowText(GetDlgItem(m_hTabDlg[3],IDC_STATIC_PAGE),buffer);

	ListView_DeleteAllItems(m_hChangedNameList);

	SI32 listindex = 0 ;

	for ( int i = 0 ; i < MAX_HOZO_CHANGEDNAME_LIST ; i ++ )
	{
		if ( changednamechar[i].siDate <= 0 ) continue ;

		cltDate cldate;
		cldate.MakeFullDateFromDateVary(changednamechar[i].siDate);
		if(cldate.GetDateText(buffer) == false) continue;
		ListViewInsertItem(m_hChangedNameList, -1, listindex, 0, buffer, -1);
		ListViewInsertItem(m_hChangedNameList, -1, listindex, 1, changednamechar[i].szOldName, -1);
		ListViewInsertItem(m_hChangedNameList, -1, listindex, 2, changednamechar[i].szNewName, -1);

		listindex ++ ;
	}
}

void CHozoDlg::Set()
{
	if ( !IsWindow(m_hDlg) )
	{
		m_hDlg = CreateDialog( m_hInst, MAKEINTRESOURCE( IDD_DIALOG_HOZO ), m_hWnd, StatiCHozoDlgProc );
		SendDlgItemMessage( m_hDlg, IDC_EDIT_HOZO, EM_LIMITTEXT, 255, 0 );
		m_hGroup = GetDlgItem(m_hDlg,IDC_STATIC_HOZOPROFILE);

		if ( m_bRectSwitch )
		{
			MoveWindow(m_hDlg, 
				m_rtDlg.left, m_rtDlg.top, 
				m_rtDlg.right - m_rtDlg.left, 
				m_rtDlg.bottom - m_rtDlg.top, FALSE);
		}

		HWND hButton = NULL ;
		cltClient *pclclient = (cltClient*)pclClient;
		if ( pclclient->pclCM->CR[1]->pclCI->clBI.uiGRank != GRANK_MINISTER2 )
		{
			//hButton = GetDlgItem(m_hDlg,IDSETUP);
			//EnableWindow(hButton,FALSE);

			hButton = GetDlgItem(m_hDlg,IDC_EDIT_HOZO);
			EnableWindow(hButton,FALSE);

			hButton = GetDlgItem(m_hDlg,IDOK);
			EnableWindow(hButton,FALSE);

			hButton = GetDlgItem(m_hTabDlg[0],IDC_EDIT_RESIDENT);
			EnableWindow(hButton,FALSE);

			hButton = GetDlgItem(m_hTabDlg[0],IDC_EDIT_MAX_SHOP);
			EnableWindow(hButton,FALSE);

			hButton = GetDlgItem(m_hTabDlg[1],IDC_EDIT_PRIZE_MONEY);
			EnableWindow(hButton,FALSE);

			hButton = GetDlgItem(m_hTabDlg[2],IDC_EDIT_FIXATION_MONEY);
			EnableWindow(hButton,FALSE);
		}
		else
		{
			hButton = GetDlgItem(m_hDlg,IDGGOM);
			EnableWindow(hButton,FALSE);

			hButton = GetDlgItem(m_hDlg,IDWHISPER);
			EnableWindow(hButton,FALSE);
		}
		cltMsg clMsg(GAMEMSG_REQUEST_GETHOZOINFO,0,NULL);
		pclclient->SendMsgToServer((char*)&clMsg);
	}
	else
	{
		DestroyWindow(m_hDlg);
	}
}

void CHozoDlg::Show(CMinisterBase *pMini,SI32 residentnum,SI32 maxshop,SI32 totalcharnum,SI32 calctime,
					SI32 prizemoney,SI32 prizesettime,SI32 fixationmoney,SI32 newcharrate)
{
	cltClient *pclclient = (cltClient *)pclClient;
	if ( m_bSet )
	{
		char* pTilte = GetTxtFromMgr(2045);
		char* pText = GetTxtFromMgr(2046);
		pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTilte, pText);
		m_bSet = FALSE ;
		return ;
	}

	pclClient->PushEffect(EFFECT_HOZO,0,0);

	strcpy(m_szHozoID,pMini->m_szAccountID);
	strcpy(m_szHozoName,pMini->m_szPersonName);
	strcpy(m_szHozoProfile,pMini->m_szGreeting);

	m_siPrizeMoney = prizemoney ;
	m_siPrizeSetTime = prizesettime ;
	m_siFixationMoney = fixationmoney ;

	cltDate cldate;
	char buffer[256] = "" ;
	char numbuf[128] = "" ;

	char* pText = GetTxtFromMgr(2085);
	g_JYLibFunction.SetNumUnit(totalcharnum,numbuf,pText);
	SetDlgItemText(m_hTabDlg[0], IDC_STATIC_TOTALCHARNUM, numbuf);

	cldate.MakeFullDateFromDateVary(calctime);
	if(cldate.GetDateText(numbuf) == TRUE)
	{
		pText = GetTxtFromMgr(612);
		sprintf(buffer, pText, numbuf);
		SetDlgItemText(m_hTabDlg[0], IDC_STATIC_TOTALCHARNUM_DATE, buffer);
	}

	cldate.MakeFullDateFromDateVary(m_siPrizeSetTime);
	if(cldate.GetDateText(numbuf) == TRUE)
	{
		pText = GetTxtFromMgr(2086);
		sprintf(buffer, pText, numbuf);
		SetDlgItemText(m_hTabDlg[0], IDC_STATIC_PRIZE_SETTIME, buffer);
	}

	HWND hWnd = GetDlgItem( m_hTabDlg[0], IDC_EDIT_RESIDENT );
	wsprintf(buffer,"%d",residentnum);
	SetWindowText(hWnd,buffer);

	hWnd = GetDlgItem( m_hTabDlg[0], IDC_EDIT_MAX_SHOP );
	wsprintf(buffer,"%d",maxshop);
	SetWindowText(hWnd,buffer);

	hWnd = GetDlgItem( m_hTabDlg[1], IDC_EDIT_PRIZE_MONEY );
	wsprintf(buffer,"%d",m_siPrizeMoney * UNIT_HOZO_SETMONEY );
	SetWindowText(hWnd,buffer);

	hWnd = GetDlgItem( m_hTabDlg[2], IDC_EDIT_FIXATION_MONEY );
	wsprintf(buffer,"%d",m_siFixationMoney * UNIT_HOZO_SETMONEY );
	SetWindowText(hWnd,buffer);

	hWnd = GetDlgItem( m_hTabDlg[4], IDC_STATIC_NEWCHAR_RATE );
	wsprintf(buffer,"%d",newcharrate);
	SetWindowText(hWnd,buffer);

	pText = GetTxtFromMgr(2087);
	wsprintf(buffer, pText,m_szHozoName);
	SetWindowText( m_hDlg ,buffer);
	SetDlgItemText( m_hDlg,IDC_EDIT_HOZO, m_szHozoProfile );
	pText = GetTxtFromMgr(2048);
	strcat(buffer, pText);
	SetWindowText(m_hGroup,buffer);
	
	char ProfileURL[MAX_PATH];

/*	pclClient->GetCZProfileUrl( ProfileURL, m_szHozoID );

	hWnd = GetDlgItem( m_hDlg, IDC_STATIC_PROFILE );
	if(hWnd)	DisplayHTMLPage(hWnd, ProfileURL);*/

	// INTRO
	
	pclClient->GetCZIntroUrl( ProfileURL, m_szHozoID );
	hWnd = GetDlgItem(m_hDlg, IDC_STATIC_HOZOINTRO);
	if(hWnd)	DisplayHTMLPage(hWnd, ProfileURL);

	ShowWindow( m_hDlg, SW_SHOW );
	if ( m_siViewDlg == 0 )
		ShowWindow(m_hTabDlg[m_siViewDlg], SW_SHOW) ;
}