// JangGilSanDlg.cpp: implementation of the CJangGilSanDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "../../Client/client.h"

#include "../../lib/WebHTML/WebHTML.h"

#include "../../CommonLogic/MsgType.h"
#include "../../CommonLogic/CommonLogic.h"
#include "..\..\Client\Interface\ErrorMsgDlg\ErrorMsgDlg.h"

extern cltCommonLogic* pclClient;

#include "JangGilSanDlg.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CJangGilSanDlg::CJangGilSanDlg()
{
	m_hInst = NULL ;
	m_hWnd = NULL ;

	m_hSBS = NULL ;

	m_hPortraitBitmapSBS = NULL ;

	SetRect(&m_rtSBS,0,0,0,0);

	m_bSBS = FALSE ;

}

CJangGilSanDlg::~CJangGilSanDlg()
{
}

void CJangGilSanDlg::Create(HINSTANCE hInst, HWND hParent )
{
	m_hInst = hInst ;
	m_hWnd = hParent ;
}

BOOL CALLBACK CJangGilSanDlg::StatiCJangGilSanSBSProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	cltClient *pclclient = (cltClient*)pclClient;
	return pclclient->m_pJangGilSanDlg->JangGilSanSBSProc( hDlg, iMsg, wParam, lParam );
}

BOOL CALLBACK CJangGilSanDlg::JangGilSanSBSProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam )
{
	cltClient *pclclient = (cltClient*)pclClient;

	switch( iMsg )
	{
	case WM_INITDIALOG:
		{
			if( m_hPortraitBitmapSBS == NULL )
			{
				m_hPortraitBitmapSBS = (HBITMAP)LoadImage( pclClient->GetInstance(), "Interface/Portrait/KIND_JANGGILSAN.bmp", IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE );

				if ( m_hPortraitBitmapSBS != NULL )
				{
					SendDlgItemMessage( hDlg, IDC_STATIC_PORTRAIT_JANGGILSAN_SBS, STM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)m_hPortraitBitmapSBS);
				}
			}

			//---------------------------------
			// HTML 을 초기화한다.
			//---------------------------------
			//==================================			
			HWND hwnd = NULL ;

			hwnd = GetDlgItem(hDlg, IDC_STATIC_JANGGILSAN_SBSHTML);
			if ( hwnd )
			{
				EmbedBrowserObject(hwnd);
			}
		}
		break;
	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case IDOK:
			{
				if( pclClient->siServiceArea == ConstServiceArea_Netmarble)
					return FALSE;

				ShellExecute( NULL, NULL, "IEXPLORE.EXE",
							  "http://goonzu.ndoors.com/center/event/20040720/jgs_sbs.html",
							  NULL, SW_SHOWNORMAL );
			}
			break;
		}
		break;
	case WM_CLOSE:
		{
			DestroyWindow(m_hSBS);
		}
		break;
	case WM_DESTROY:
		{
			if ( m_hPortraitBitmapSBS )
			{
				DeleteObject( m_hPortraitBitmapSBS );
				m_hPortraitBitmapSBS = NULL;
			}

			//---------------------------------
			// HTML 을 해제한다.
			//---------------------------------
			//==================================
			HWND hwnd = NULL ;

			hwnd = GetDlgItem( hDlg, IDC_STATIC_JANGGILSAN_SBSHTML );
			if( hwnd )
			{
				DoPageAction(hwnd, WEBPAGE_STOP);
				UnEmbedBrowserObject(hwnd);
			}

			GetWindowRect(m_hSBS, &m_rtSBS);
			m_bSBS = TRUE ;

			m_hSBS = NULL ;
		}
		break;
	}

	return FALSE;
}

void CJangGilSanDlg::ShowSBS(bool bWaitingMan)
{
	if ( !IsWindow(m_hSBS) )
	{
		m_hSBS = CreateDialog( m_hInst, MAKEINTRESOURCE( IDD_DIALOG_JANGGILSAN_SBS ), m_hWnd, StatiCJangGilSanSBSProc );

		if ( m_bSBS )
		{
			MoveWindow(m_hSBS, 
				m_rtSBS.left, m_rtSBS.top, 
				m_rtSBS.right - m_rtSBS.left, 
				m_rtSBS.bottom - m_rtSBS.top, FALSE);
		}

		char buf[256] = "" ;
		char path[512] = "" ;
		GetCurrentDirectory( 256, path );

		HWND hwnd = GetDlgItem(m_hSBS,IDC_STATIC_JANGGILSAN_SBSHTML);

		if ( bWaitingMan )
		{
			wsprintf(buf, "\\Interface\\JangGilSan\\%s.htm", "true");
			EnableWindow(GetDlgItem(m_hSBS,IDOK),TRUE);
		}
		else
		{
			wsprintf(buf, "\\Interface\\JangGilSan\\%s.htm", "false");
			EnableWindow(GetDlgItem(m_hSBS,IDOK),FALSE);
		}
		strcat(path,buf);
		DisplayHTMLPage( hwnd, path );

		ShowWindow( m_hSBS, SW_SHOW );
	}
	else
	{
		DestroyWindow(m_hSBS);
	}
}

