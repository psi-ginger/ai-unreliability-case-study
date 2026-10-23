#include "ToolBarBaseInfoDlg.h"

#include "..\..\Interface\ErrorMsgDlg\ErrorMsgDlg.h"

#include "../../Client/client.h"


//-----------------------------
// Common
//-----------------------------
#include "PersonInfo.h"
#include "../../Common/Father/Father.h"
#include "Char\CharManager\CharManager.h"

#include "../../Lib/JYLibFunction.h"
extern CJYLibFunction g_JYLibFunction;
extern cltCommonLogic* pclClient;

CToolBarBaseInfoDlg g_ToolBarBaseInfoDlg;

CToolBarBaseInfoDlg::CToolBarBaseInfoDlg()
{
	m_hDlg = NULL;
}

CToolBarBaseInfoDlg::~CToolBarBaseInfoDlg()
{
	if ( m_hDlg )
	{
		KillTimer( m_hDlg, 0 );
		DestroyWindow( m_hDlg );
	}
}

void CToolBarBaseInfoDlg::Initialize()
{
	return;
}

void CToolBarBaseInfoDlg::Destroy()
{
	return;
}

void CToolBarBaseInfoDlg::Create( HINSTANCE hInst, HWND hWnd )
{
	m_hDlg = ::CreateDialog( hInst, MAKEINTRESOURCE(IDD_DIALOG_TOOLBAR_BASEINFO), hWnd, StaticToolBaseInfoDlgProc );

	MoveWindow( m_hDlg, 575, 1, 232, 24, TRUE );

	ShowWindow( m_hDlg, SW_SHOW );
	return;
}

BOOL CALLBACK CToolBarBaseInfoDlg::StaticToolBaseInfoDlgProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam )
{
	return g_ToolBarBaseInfoDlg.ToolBaseInfoDlgProc( hDlg, iMsg, wParam, lParam );
}

BOOL CALLBACK CToolBarBaseInfoDlg::ToolBaseInfoDlgProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam )
{	
	static HBITMAP hBackGroundBitmap = NULL;

	switch ( iMsg )
	{
	case WM_INITDIALOG:
		{
			m_hDlg = hDlg;
			
			hBackGroundBitmap = (HBITMAP)LoadImage( pclClient->GetInstance(), TEXT("Interface/ToolBarBaseInfoDlg/ToolBarBaseInfo.bmp"), IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE );
			SendDlgItemMessage( hDlg, IDC_STATIC_TOOLBAR_BASEINFO, STM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)hBackGroundBitmap  );
			MoveWindow( GetDlgItem( hDlg, IDC_STATIC_TOOLBAR_BASEINFO ), 0, 1, 232, 24, TRUE );

			
			MoveWindow( GetDlgItem( hDlg, IDC_STATIC_DATE ), 26, 1, 108, 22, TRUE );
			//MoveWindow( GetDlgItem( hDlg, IDC_STATIC_SOCIALPOSITION ), 199, 1, 78, 22, TRUE );
			MoveWindow( GetDlgItem( hDlg, IDC_STATIC_MONEY ), 160, 1, 72, 22, TRUE );

			SetTimer( m_hDlg, 0, 1000, NULL );
		}
		return TRUE;

	case WM_TIMER:
		{
			// 주기적( 1 초당 한번 정도 )으로 업데이트 시키면 되므로...
			if ( pclClient && pclClient->pclCM && pclClient->GameMode == GAMEMODE_CLIENT &&
				 pclClient->pclCM->CR[1] )
			{
				// 현재 가진 돈
				GMONEY Money = pclClient->pclCM->CR[1]->pclCI->clIP.GetMoney();
				TCHAR Buffer[256 ] = TEXT("");
				//sprintf( Buffer, TEXT("%d"), Money );
				g_JYLibFunction.SetNumUnit( Money, Buffer,256, TEXT("") );

				SetWindowText( GetDlgItem( m_hDlg, IDC_STATIC_MONEY ), Buffer );

				/*
				// 현재 신분
				TCHAR SocialPosition[128]=TEXT("");
				cltSimpleRank* pclrank = &pclClient->pclCM->CR[1]->pclCI->clCharRank;
				GetRankName(pclrank, SocialPosition);
				SetWindowText( GetDlgItem( hDlg, IDC_STATIC_SOCIALPOSITION ), SocialPosition );
				*/
			}
		}
		break;
	case WM_COMMAND:
		{	
		}
		break;
		
	case WM_DESTROY:
		{
			// 타이머를 해제한다. 
			KillTimer( m_hDlg, 0 );
			
			if ( hBackGroundBitmap )
			{
				DeleteObject( hBackGroundBitmap );
				hBackGroundBitmap = NULL;
			}
			
			m_hDlg = NULL;
		}
		break;
	}

	return FALSE;
}

void CToolBarBaseInfoDlg::SetDate( TCHAR *strDate )
{
	/*
	cltClient *pclclient = (cltClient *)pclClient;
	if ( pclclient->clClientDate.GetDateText( Buffer ) )
	{
	}
	*/
	
	SetWindowText( GetDlgItem( m_hDlg, IDC_STATIC_DATE ), strDate );
	return;
}