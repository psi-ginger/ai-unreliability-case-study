#include "NotifyGetItemFromMonsterDlg.h"
#include "../RememberWindowPos/RememberWindowPosMgr.h"

#include "../../Lib/JYLibFunction.h"

extern CJYLibFunction g_JYLibFunction;

#include "../../Client.h"

CNotifyGetItemFromMonsterDlg g_NotifyGetItemFromMonsterDlg;

extern cltCommonLogic* pclClient;


CNotifyGetItemFromMonsterDlg::CNotifyGetItemFromMonsterDlg()
{
	m_hDlg					= NULL;
	m_bShowFlag				= false;
	m_hItemImage			= NULL;	
	m_pRememberWindowPosMgr = NULL;
	m_hEditBrush			= NULL;
	Initialize();
}

CNotifyGetItemFromMonsterDlg::~CNotifyGetItemFromMonsterDlg()
{
	Destroy();

	if ( m_hDlg )
	{
		DestroyWindow( m_hDlg );
	}
}

void CNotifyGetItemFromMonsterDlg::Initialize()
{

	return;
}

void CNotifyGetItemFromMonsterDlg::Destroy()
{
	if ( m_hItemImage != NULL )
	{
		DeleteObject( m_hItemImage );
		m_hItemImage = NULL;
	}
	
	return;
}

void CNotifyGetItemFromMonsterDlg::Create( HINSTANCE hInst, HWND hWnd, CRememberWindowPosMgr *pRememberWindowPosMgr )
{	
	m_hDlg = CreateDialog( hInst, MAKEINTRESOURCE( IDD_DIALOG_GETITEM_NOTIFY ), hWnd, StaticNotifyGetItemFromMonsterDlgProc );

	/*
	if ( pRememberWindowPosMgr )
	{
		m_pRememberWindowPosMgr = pRememberWindowPosMgr;
		
		if ( m_pRememberWindowPosMgr->IsExist( NOTIFY_GETITEM ) )
		{
			RECT Rect;
			m_pRememberWindowPosMgr->GetWindowPosData( NOTIFY_GETITEM, (int*)&Rect.left, (int*)&Rect.top, (int*)&Rect.right, (int*)&Rect.bottom );
			MoveWindow( m_hDlg, Rect.left, Rect.top, Rect.right - Rect.left, Rect.bottom - Rect.top, TRUE );		
		}
		else
		{
			RECT Rect;
			GetWindowRect( m_hDlg, &Rect );
			m_pRememberWindowPosMgr->AddWindowPosData( NOTIFY_GETITEM, Rect.left, Rect.top, Rect.right, Rect.bottom );
		}
		
	}
	*/

	


	Hide();

	return;
}

BOOL CALLBACK CNotifyGetItemFromMonsterDlg::StaticNotifyGetItemFromMonsterDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	return g_NotifyGetItemFromMonsterDlg.NotifyGetItemFromMonsterDlgProc( hDlg, iMsg, wParam, lParam );
}

BOOL CALLBACK CNotifyGetItemFromMonsterDlg::NotifyGetItemFromMonsterDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	switch( iMsg )
	{
	case WM_INITDIALOG:
		{
			m_hDlg = hDlg;

			m_hGetItemExplainEdit = GetDlgItem( hDlg, IDC_EDIT_GETITEM_EXPLAIN );

			m_hImageWnd = GetDlgItem( hDlg, IDC_LIST_GETITEM_IMAGE );

			cltClient *pclclient = (cltClient *)pclClient;
			pclclient->pclImageList->ConnectToListView( m_hImageWnd, LVSIL_NORMAL );
			
			m_hEditBrush = CreateSolidBrush( COLOR_DIALOG_WHITEBLUE );
			
			//SetTimer( hDlg, 0, MAX_SET_TIMER_GET_ITEM_FROM_MONSTER_DLG_DELAY_TIME, NULL );
		}
		return TRUE;
	case WM_CTLCOLORSTATIC:
		{
			if ( GetDlgItem( hDlg, IDC_EDIT_GETITEM_EXPLAIN ) == (HWND)lParam )
			{
				HDC hdc = (HDC)wParam;
				
				SetTextColor( hdc, COLOR_DIALOG_BLACK );
				SetBkColor( hdc, COLOR_DIALOG_WHITEBLUE );

				return (BOOL)m_hEditBrush;
			}
		}
		break;		
	case WM_WINDOWPOSCHANGED:
		{
			/*
			if ( m_pRememberWindowPosMgr )
			{
				LPWINDOWPOS lpWindowPos = (LPWINDOWPOS)lParam;
				m_pRememberWindowPosMgr->WindowPosChange( NOTIFY_GETITEM, lpWindowPos->x, lpWindowPos->y, lpWindowPos->x + lpWindowPos->cx, lpWindowPos->y + lpWindowPos->cy );
			}
			*/
		}
		break;
	
	case WM_TIMER:
		{
			++m_siCount;

			if ( m_siCount * MAX_SET_TIMER_GET_ITEM_FROM_MONSTER_DLG_DELAY_TIME >= MAX_NOTIFY_GET_ITEM_FROM_MONSTER_DLG_DELAY_TIME )
			{
				Hide();
			}
		}
		break;
	case WM_CLOSE:
		{
			Hide();
		}
		break;
	
	case WM_DESTROY:
		{
			KillTimer( hDlg, 0 );

			if ( m_hEditBrush )
			{
				DeleteObject( m_hEditBrush );
				m_hEditBrush = NULL;
			}
			m_hDlg = NULL;
		}
		break;
	}
	
	return FALSE;
}

void CNotifyGetItemFromMonsterDlg::Show( cltItem *pclItem, SI64 pricevalue ,SI32 siCenterPosX, SI32 siCenterPosY )
{
	if ( pclItem == NULL )
	{
		return;
	}
	
	cltClient *pclclient = (cltClient *)pclClient;
	if ( pclclient->bWatchModeSwitch )
	{
		return;
	}

	KillTimer( m_hDlg, 0 );

	if ( m_siCount > 0 && m_bShowFlag )
	{
		SetInventoryInListView( pclClient->pclItemManager, m_hDlg, IDC_LIST_GETITEM_IMAGE, NULL, pclItem, 5, 5 );
		
		char szString[ 512 ] = "";

		char itemname[128] = "" ;
		char moneybuf[256] = "" ;
		if(pclItem->GetFullName(pclClient->pclItemManager , itemname) == true)
		{
			bool bPirce = DATA_TO_BOOL(pricevalue) ;
			if ( bPirce )
			{
				char* pText = GetTxtFromMgr(611);
				g_JYLibFunction.SetNumUnit(pricevalue,moneybuf,pText);
				pText = GetTxtFromMgr(2393);
				sprintf( szString, pText, itemname, pclItem->GetItemNum() ,moneybuf);
			}
			else
			{
				char* pText = GetTxtFromMgr(2394);
				sprintf( szString, pText, itemname, pclItem->GetItemNum() );
			}
			
			SetWindowText( m_hGetItemExplainEdit, szString );
		}

		Update();


		SetTimer( m_hDlg, 0, MAX_SET_TIMER_GET_ITEM_FROM_MONSTER_DLG_DELAY_TIME, NULL );

		m_bShowFlag = true;
		
		SendMessage( m_hDlg, WM_MOVE, siCenterPosX - 150, siCenterPosY - 40 );
		
		ShowWindow( m_hDlg, SW_SHOWNOACTIVATE );
	}
	else
	{
		m_siCount = 0;

		SetInventoryInListView( pclClient->pclItemManager, m_hDlg, IDC_LIST_GETITEM_IMAGE, NULL, pclItem, 5, 5 );

		char szString[ 512 ] = "";

		char itemname[128] = "" ;
		char moneybuf[256] = "" ;
		if(pclItem->GetFullName(pclClient->pclItemManager , itemname) == true)
		{
			bool bPirce = DATA_TO_BOOL(pricevalue) ;
			if ( bPirce )
			{
				char* pText = GetTxtFromMgr(611);
				g_JYLibFunction.SetNumUnit(pricevalue,moneybuf,pText);
				pText = GetTxtFromMgr(2393);
				sprintf( szString, pText, itemname, pclItem->GetItemNum() ,moneybuf);
			}
			else
			{
				char* pText = GetTxtFromMgr(2394);
				sprintf( szString, pText, itemname, pclItem->GetItemNum() );
			}
			
			SetWindowText( m_hGetItemExplainEdit, szString );
		}

		SetTimer( m_hDlg, 0, MAX_SET_TIMER_GET_ITEM_FROM_MONSTER_DLG_DELAY_TIME, NULL );

		m_bShowFlag = true;

		SendMessage( m_hDlg, WM_MOVE, siCenterPosX - 150, siCenterPosY - 40 );

		ShowWindow( m_hDlg, SW_SHOWNOACTIVATE );
		
	}

	return;
}

void CNotifyGetItemFromMonsterDlg::Hide()
{
	m_siCount = 0;
	m_bShowFlag = false;

	KillTimer( m_hDlg, 0 );

	if ( m_hItemImage != NULL )
	{
		DeleteObject( m_hItemImage );
		m_hItemImage = NULL;
	}

	ShowWindow( m_hDlg, SW_HIDE );

	return;
}

void CNotifyGetItemFromMonsterDlg::Update()
{
	m_siCount = 0;
	
	return;
}
