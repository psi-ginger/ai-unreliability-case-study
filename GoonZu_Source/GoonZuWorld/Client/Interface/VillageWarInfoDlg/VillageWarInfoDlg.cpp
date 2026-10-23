// VillageWarInfoDlg.cpp: implementation of the CVillageWarInfoDlg class.
//
//////////////////////////////////////////////////////////////////////
/*
#include "../../Client/client.h"

#include "../../CommonLogic/MsgType.h"
#include "../../CommonLogic/CommonLogic.h"
#include "../../CommonLogic/MsgType-System.h"

#include "..\..\FontManager\FontManager.h"

#include "../../Music/Music.h"

#include "../../../lib/WebHTML/WebHTML.h"

#include "VillageWarInfoDlg.h"

extern cltCommonLogic* pclClient;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CVillageWarInfoDlg::CVillageWarInfoDlg()
{
	m_hInst = NULL ;
	m_hWnd = NULL ;

	m_bRectSwitch = FALSE ;

	m_hDlg = NULL ;

	m_hEditBrush = NULL ;
}

CVillageWarInfoDlg::~CVillageWarInfoDlg()
{
}

void CVillageWarInfoDlg::Create(HINSTANCE hInst, HWND hParent )
{
	m_hInst = hInst ;
	m_hWnd = hParent ;
}

BOOL CALLBACK CVillageWarInfoDlg::StatiCVillageWarInfoDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	cltClient *pclclient = (cltClient*)pclClient;
	return pclclient->m_pVillageWarInfoDlg->VillageWarInfoDlgProc( hDlg, iMsg, wParam, lParam );
}

BOOL CALLBACK CVillageWarInfoDlg::VillageWarInfoDlgProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam )
{
	cltClient *pclclient = (cltClient*)pclClient;

	switch( iMsg )
	{
	case WM_INITDIALOG:
		{
			g_FontManager.SetFont( hDlg, IDC_EDIT_VILLAGEWARINFO, 13 );
			
			m_hEditBrush = CreateSolidBrush(COLOR_DIALOG_YELLOW);

			//---------------------------------
			// HTML 을 초기화한다.
			//---------------------------------
			//==================================
			HWND hwnd = GetDlgItem(hDlg, IDC_STATIC_VILLAGEWARINFO_HTML);
			if ( hwnd )
			{
				EmbedBrowserObject(hwnd);
			}
			MoveWindow( hwnd, 0, 0, 100, 100, TRUE );

			MoveWindow( GetDlgItem( hDlg, IDC_EDIT_VILLAGEWARINFO ), 101, 0, 250, 100, TRUE );
		}
		break;
	case WM_SIZE:
		{
			//============================
			// HTML Resize 를 한다.
			//============================			
			HWND hwnd = GetDlgItem(hDlg, IDC_STATIC_EVENTNOTICE_HTML);
			if( hwnd )
			{
				ResizeBrowser(hwnd, LOWORD(lParam), HIWORD(lParam));
			}

			return FALSE;
		}
		break;
	case WM_CTLCOLORSTATIC:
		{
			if ( GetDlgItem( hDlg, IDC_EDIT_VILLAGEWARINFO ) == (HWND)lParam )
			{
				HDC hdc = (HDC)wParam;
				SetTextColor( hdc, COLOR_DIALOG_BLACK );
				SetBkColor( hdc, COLOR_DIALOG_YELLOW );
				return (BOOL)m_hEditBrush;
			}
		}
		break;
	case WM_CLOSE:
		{
			DestroyWindow(m_hDlg);
		}
		break;
	case WM_DESTROY:
		{
			m_bRectSwitch = TRUE ;

			HWND hwnd = GetDlgItem( hDlg, IDC_STATIC_VILLAGEWARINFO_HTML );
			if( hwnd )
			{
				UnEmbedBrowserObject(hwnd);
			}

			if ( m_hEditBrush )
			{
				DeleteObject( m_hEditBrush );
				m_hEditBrush = NULL;
			}

			m_hDlg = NULL ;
		}
		break;
	}

	return FALSE;
}

void CVillageWarInfoDlg::Set(SI32 winvillage,SI32 losevillage)
{
	if ( winvillage < 1 || winvillage > CUR_VILLAGE_LIMIT) return ;
	if ( losevillage < 1 || losevillage > CUR_VILLAGE_LIMIT ) return ;

	cltClient *pclclient = (cltClient*)pclClient;

	if ( !IsWindow(m_hDlg) )
	{
		m_hDlg = CreateDialog( m_hInst, MAKEINTRESOURCE( IDD_DIALOG_VILLAGEWARINFO ), m_hWnd, StatiCVillageWarInfoDlgProc );

		if ( m_bRectSwitch )
		{
			MoveWindow(m_hDlg, 
				m_rtDlg.left, m_rtDlg.top, 
				m_rtDlg.right - m_rtDlg.left, 
				m_rtDlg.bottom - m_rtDlg.top, FALSE);
		}

		ShowWindow( m_hDlg, SW_SHOW );
	}

	char buf[256] = "" ;
	char path[512] = "" ;

	GetCurrentDirectory( 256, path );
	strcpy(buf, "\\Interface\\VillageWarInfoDlg\\VillageWarInfo.html");
	strcat(path,buf);	

	HWND hStatic = GetDlgItem(m_hDlg,IDC_STATIC_VILLAGEWARINFO_HTML);	
	DisplayHTMLPage( hStatic, path );

	sprintf(buf,"[%s] 마을이 [%s] 마을을 함락시켰습니다!",pclclient->pclVillageManager->GetName(winvillage),pclclient->pclVillageManager->GetName(losevillage));
	hStatic = GetDlgItem(m_hDlg,IDC_EDIT_VILLAGEWARINFO);
	SetWindowText( hStatic, buf );
}

*/