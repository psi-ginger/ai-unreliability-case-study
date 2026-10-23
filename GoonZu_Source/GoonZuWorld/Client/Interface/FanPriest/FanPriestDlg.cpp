// FanPriestDlg.cpp: implementation of the CFanPriestDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "../../Client/client.h"
#include "../../Client/Music\Music.h"

#include "../../CommonLogic/MsgType.h"
#include "../../CommonLogic/CommonLogic.h"
#include "..\..\Client\Interface\ErrorMsgDlg\ErrorMsgDlg.h"
#include "..\..\Client\Interface\NotifyMsgDlg\NotifyMsgDlg.h"

#include "FanPriestDlg.h"

//-----------------------------
// Common
//-----------------------------
#include "PersonInfo.h"
#include "Char\CharManager\CharManager.h"

extern cltCommonLogic* pclClient;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFanPriestDlg::CFanPriestDlg()
{
	m_hInst = NULL ;
	m_hWnd = NULL ;

	m_hDlg = NULL ;
	m_hEdit = NULL ;

	m_bRectSwitch = FALSE ;

	m_iSelectVillageInx = 0 ;
	m_iOldVillageInx = 0 ;

	m_hBitmap = NULL ;
}

CFanPriestDlg::~CFanPriestDlg()
{
	if ( m_hDlg )
	{
		DestroyWindow(m_hDlg);
	}
}

void CFanPriestDlg::Create(HINSTANCE hInst, HWND hParent )
{
	m_hInst = hInst ;
	m_hWnd = hParent ;
}

BOOL CALLBACK CFanPriestDlg::StatiCFanPriestDlgProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	cltClient *pclclient = (cltClient*)pclClient;
	return pclclient->m_pFanPriestDlg->FanPriestDlgProc( hDlg, iMsg, wParam, lParam );
}

BOOL CALLBACK CFanPriestDlg::FanPriestDlgProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam )
{
	cltClient *pclclient = (cltClient*)pclClient;

	switch( iMsg )
	{
	case WM_INITDIALOG:
		{
			m_iOldVillageInx = pclclient->pclCM->CR[1]->GetMapIndex() ;
			SetTimer(hDlg, 1, 1000, NULL);
		}
		break;
	case WM_TIMER:
		{
			switch(wParam)
			{
			case 1:
				{
					SI32 village = pclclient->pclCM->CR[1]->GetMapIndex() ;
					if ( m_iOldVillageInx != village )
						DestroyWindow(m_hDlg);
				}
				break;
			}
		}
		break;
	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case IDC_LIST_VILLAGE:
			{
				switch(HIWORD(wParam))
				{
				case LBN_SELCHANGE:
					SI32 index = ListGetCurrentSel(hDlg, IDC_LIST_VILLAGE);
					if ( index < 0 ) break ;

					m_iSelectVillageInx = SendDlgItemMessage(hDlg, IDC_LIST_VILLAGE, LB_GETITEMDATA, index, 0);
				}
				break;
			}
			break;
		case IDOK:
			{
				if ( pclClient->pclCM->CR[1]->clInnerStatus.IsStatus(INNERSTATUS_PRISON))
				{
					char* pTitle = GetTxtFromMgr(1891);
					char* pText = GetTxtFromMgr(1892);
					pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);
					return FALSE;
				}
				// 영자나라에 있으면 불가능
				if ( pclClient->pclCM->CR[1]->GetMapIndex() == MAPINDEX_MASTERWORLD &&
					pclClient->pclCM->CR[1]->GetGameMasterLevel() < BASIC_GAMEMASTER_LEVEL )
				{
					return FALSE ;
				}

				char buffer[256] = "" ;

				if ( m_iSelectVillageInx > 0 && m_iSelectVillageInx <= CUR_VILLAGE_LIMIT )
				{
					if ( pclclient->pclCM->CR[1]->pclCI->clCharItem.GetItemNum(14000) > 0 )
					{
						char* pText = GetTxtFromMgr(1893);
						wsprintf(buffer, pText,pclclient->pclVillageManager->GetName(m_iSelectVillageInx));

						pText = GetTxtFromMgr(1894);
						if (  MessageBox(NULL, pText, buffer, MB_ICONQUESTION | MB_YESNO) == IDYES )
						{

							// 효과음을 연주한다
							pclclient->PushEffect(EFFECT_FANPRIEST_WARP, 0, 0);

							cltMsg clMsg(GAMEMSG_REQUEST_WARPVILLAGE, sizeof(m_iSelectVillageInx), (char*)&m_iSelectVillageInx);
							pclclient->SendMsgToServer((char*)&clMsg);
							DestroyWindow(m_hDlg);
						}
					}
					else
					{
						char* pTitle = GetTxtFromMgr(1891);
						char* pText = GetTxtFromMgr(1895);

						pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);
					}
				}
				else
				{
					char* pTitle = GetTxtFromMgr(1891);
					char* pText = GetTxtFromMgr(1896);

					pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText );
				}
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
			GetWindowRect(m_hDlg, &m_rtDlg);
			m_bRectSwitch = TRUE ;
			m_hEdit = NULL ;
			m_iSelectVillageInx = 0 ;
			m_iOldVillageInx = 0 ;

			if ( m_hBitmap )
			{
				DeleteObject(m_hBitmap);
				m_hBitmap = NULL ;
			}

			KillTimer(hDlg,1);

			m_hDlg = NULL ;
		}
		break;
	}

	return FALSE;
}

void CFanPriestDlg::Set()
{
	if ( !IsWindow(m_hDlg) )
	{
		m_hDlg = CreateDialog( m_hInst, MAKEINTRESOURCE( IDD_DIALOG_FANPRIEST ), m_hWnd, StatiCFanPriestDlgProc );
		m_hEdit = GetDlgItem(m_hDlg,IDC_EDIT_FANPRIEST);

		char* pText = GetTxtFromMgr(1897);

		SetWindowText( m_hEdit, pText );

		if ( m_bRectSwitch )
		{
			MoveWindow(m_hDlg, 
				m_rtDlg.left, m_rtDlg.top, 
				m_rtDlg.right - m_rtDlg.left, 
				m_rtDlg.bottom - m_rtDlg.top, FALSE);
		}

		if( m_hBitmap == NULL )
		{
			m_hBitmap = (HBITMAP)LoadImage( m_hInst, "Interface/Portrait/KIND_FANPRIEST.bmp", IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE );

			if ( m_hBitmap != NULL )
			{
				SendDlgItemMessage( m_hDlg, IDC_STATIC_PORTRAIT_FANPRIEST, STM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)m_hBitmap);
			}
		}

		ShowWindow( m_hDlg, SW_SHOW );
		ShowVillageList();
	}
	else
	{
		DestroyWindow(m_hDlg);
	}
}

void CFanPriestDlg::ShowVillageList()
{
	SI32 i;
	cltClient *pclclient = (cltClient*)pclClient;

	SI32 village = pclclient->pclCM->CR[1]->GetMapIndex() ;

	char buffer[256] = "";

	ListDelete(m_hDlg, IDC_LIST_VILLAGE);

	for(i = 0;i <= CUR_VILLAGE_LIMIT ;i++)
	{
		if ( i == village ) continue;

		wsprintf(buffer,"%s",pclclient->pclVillageManager->GetName(i));

		if ( !strcmp(buffer,"") ) continue;

		SI32 index = ListAddString(m_hDlg, IDC_LIST_VILLAGE, (LPARAM)buffer);

		ListSetItemData(m_hDlg, IDC_LIST_VILLAGE, index, i);
	}
}