#include "RecommendPersonDlg.h"

#include "../../Client/client.h"

#include "../../CommonLogic/CommonLogic.h"
#include "../../Common/Char/CharManager/CharManager.h"

#include "../../CommonLogic/Msg/MsgType-Friend.h"
#include "../../CommonLogic/MsgType-System.h"
#include "../../CommonLogic/MsgType-Person.h"

#include "../ErrorMsgDlg/ErrorMsgDlg.h"

extern cltCommonLogic* pclClient;

CRecommendPersonDlg::CRecommendPersonDlg()
{
	m_hDlg = NULL;

	m_bShow = false;

	m_hBankGroundBitmap = NULL ;
	m_hButtonBitmap = NULL ;

	m_OldOKButtonProc = NULL ;
	m_OldCancelButtonProc = NULL ;

	m_hOKButton = NULL ;
	m_hCancelButton = NULL ;
}

CRecommendPersonDlg::~CRecommendPersonDlg()
{
	if ( m_hDlg )
	{
		DestroyWindow( m_hDlg );
	}
}

void CRecommendPersonDlg::Create( HINSTANCE hInst, HWND hWnd )
{
	m_hDlg = CreateDialog( hInst, MAKEINTRESOURCE( IDD_DIALOG_RECOMMEND_PERSON ), hWnd,  StaticRecommendDlgProc );
	Hide();

	return;
}

BOOL CALLBACK CRecommendPersonDlg::StaticRecommendDlgProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam )
{
	cltClient *pclclient = (cltClient*)pclClient;
	pclclient->m_pRecommendPersonDlg->RecommendDlgProc( hDlg, iMsg, wParam, lParam );
	return FALSE;
}

BOOL CALLBACK CRecommendPersonDlg::RecommendDlgProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam )
{
	cltClient *pclclient = ( cltClient* ) pclClient;

	switch( iMsg )
	{
	case WM_INITDIALOG:
		{
			m_hDlg = hDlg;

			RECT rt;
			GetWindowRect(hDlg,&rt);
			MoveWindow(hDlg,rt.left,rt.top,310,227+18,TRUE);

			HDC hdc= GetDC( hDlg );

			m_hBankGroundBitmap = pclClient->pclItemManager->MakeDDBFromDIB( hdc, "Interface/Login/RecommendPerson.bmp" );
			SendDlgItemMessage( hDlg, IDC_STATIC_RECOMMEND_BACKGROUND, STM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)m_hBankGroundBitmap );

			ReleaseDC(hDlg, hdc);

			m_hButtonBitmap = (HBITMAP)LoadImage( pclclient->GetInstance(), "Interface/Login/LoginButton.bmp",IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE ); 

			HWND hwnd = GetDlgItem( hDlg, IDC_EDIT_PERSONNAME ) ;
			SendMessage( hwnd , EM_LIMITTEXT, MAX_PLAYER_NAME - 1, 0 );
			MoveWindow(hwnd,112,114,185,23,TRUE);

			m_hOKButton = GetDlgItem( hDlg, IDC_BUTTON_RECOMMEND_PERSON );
			m_hCancelButton = GetDlgItem( hDlg, IDCANCEL );

			m_OldOKButtonProc = (WNDPROC)SetWindowLong( m_hOKButton, GWL_WNDPROC, (LONG)StaticOKButtonSubProc);
			m_OldCancelButtonProc = (WNDPROC)SetWindowLong( m_hCancelButton, GWL_WNDPROC, (LONG)StaticCancelButtonSubProc);

			MoveWindow(m_hOKButton,107,141,98,28,TRUE);
			MoveWindow(m_hCancelButton,205,142,98,28,TRUE);
		}
		break;
	case WM_DRAWITEM:
		{
			HDC hdcMem; 
			LPDRAWITEMSTRUCT lpdis; 

			lpdis = (LPDRAWITEMSTRUCT) lParam; 
			
			if ( lpdis->CtlType == ODT_BUTTON )
			{
				HBITMAP	hOldLoginBitmap;
				hdcMem = CreateCompatibleDC(lpdis->hDC); 
				
				hOldLoginBitmap = (HBITMAP)SelectObject(hdcMem, m_hButtonBitmap );
				
				switch( lpdis->CtlID )
				{
				case IDC_BUTTON_RECOMMEND_PERSON:
					{
						if ( lpdis->itemState & ODS_SELECTED )
						{
							BitBlt( 
								lpdis->hDC,         // destination DC 
								lpdis->rcItem.left, // x upper left 
								lpdis->rcItem.top,  // y upper left 
								98,
								28,
								hdcMem,    // source device context 
								98,
								0,      // x and y upper left 								
								SRCCOPY);  // raster operation
						}
						else
						{
							BitBlt( 
								lpdis->hDC,         // destination DC 
								lpdis->rcItem.left, // x upper left 
								lpdis->rcItem.top,  // y upper left 
								98,
								28,
								hdcMem,    // source device context 
								0,
								0,      // x and y upper left 								
								SRCCOPY);  // raster operation
						}
					}
					break;
				case IDCANCEL:
					{
						if ( lpdis->itemState & ODS_SELECTED )
						{
							BitBlt( 
								lpdis->hDC,         // destination DC 
								lpdis->rcItem.left, // x upper left 
								lpdis->rcItem.top,  // y upper left 
								98,
								28,
								hdcMem,    // source device context 
								98,
								28,      // x and y upper left 								
								SRCCOPY);  // raster operation
						}
						else
						{
							BitBlt( 
								lpdis->hDC,         // destination DC 
								lpdis->rcItem.left, // x upper left 
								lpdis->rcItem.top,  // y upper left 
								98,
								28,
								hdcMem,    // source device context 
								0,
								28,      // x and y upper left 								
								SRCCOPY);  // raster operation
						}
					}
					break;
				}
				
				SelectObject( hdcMem, hOldLoginBitmap );
				DeleteDC(hdcMem); 
				break;
			}
		}
		break;
/*	case WM_COMMAND:
		{
			switch( LOWORD( wParam ) )
			{
			case IDCANCEL:
				{
					Hide();
				}
				break;
			case IDC_BUTTON_RECOMMEND_PERSON:
				{
					char Buffer[ MAX_PLAYER_NAME ] = "";
					GetDlgItemText( hDlg, IDC_EDIT_PERSONNAME, Buffer, MAX_PLAYER_NAME - 1 );
					
					cltGameMsgRequest_RecommendPerson clRecommendPerson( Buffer );
					cltMsg clMsg( GAMEMSG_REQUEST_RECOMMENDPERSON, sizeof(clRecommendPerson), (char*)&clRecommendPerson );
					pclClient->SendMsgToServer((char*)&clMsg);
				}
				break;
			}
		}
		break;*/
	case WM_DESTROY:
		{
			SetWindowLong( m_hOKButton, GWL_WNDPROC, (LONG) m_OldOKButtonProc );
			SetWindowLong( m_hCancelButton, GWL_WNDPROC, (LONG) m_OldCancelButtonProc );

			if ( m_hBankGroundBitmap )
			{
				DeleteObject( m_hBankGroundBitmap );
				m_hBankGroundBitmap = NULL;
			}

			if ( m_hButtonBitmap )
			{
				DeleteObject(m_hButtonBitmap) ;
				m_hButtonBitmap = NULL ;
			}

			m_hDlg = NULL;
		}
		break;
	}

	return FALSE;
}

LRESULT CALLBACK CRecommendPersonDlg::StaticOKButtonSubProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	cltClient *pclclient = (cltClient*)pclClient;
	return pclclient->m_pRecommendPersonDlg->OKButtonSubProc( hWnd, iMsg, wParam, lParam );
}

LRESULT CALLBACK CRecommendPersonDlg::OKButtonSubProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	HBITMAP hOldBitmap;

	TRACKMOUSEEVENT tme;

	cltClient *pclclient = (cltClient*)pclClient;

	switch ( iMsg )
	{
	case WM_NCHITTEST:
		{
			int nHit = DefWindowProc( hWnd, iMsg, wParam, lParam );
			
			if ( nHit == HTCLIENT )
			{
				HDC hDC = GetDC( hWnd );
				HDC mdc;
				mdc = CreateCompatibleDC( hDC );
				
				hOldBitmap = (HBITMAP)SelectObject( mdc, m_hButtonBitmap );
				BitBlt( 
					hDC,         // destination DC 
					0, // x upper left 
					0,  // y upper left 
					98,
					28,
					mdc,    // source device context 
					98,
					0,      // x and y upper left 								
					SRCCOPY);  // raster operation
				
				SelectObject( mdc, hOldBitmap );
				DeleteDC( mdc ); 
				ReleaseDC( hWnd, hDC );
			}		
		}
		break;
		
		
	case WM_MOUSELEAVE:
		{
			HDC hDC = GetDC( hWnd );
			HDC mdc;
			mdc = CreateCompatibleDC( hDC );

			hOldBitmap = (HBITMAP)SelectObject( mdc, m_hButtonBitmap );
			BitBlt( 
				hDC,         // destination DC 
				0, // x upper left 
				0,  // y upper left 
				98,
				28,
				mdc,    // source device context 
				0,
				0,      // x and y upper left 								
				SRCCOPY);  // raster operation

			SelectObject( mdc, hOldBitmap );
			DeleteDC( mdc ); 
			ReleaseDC( hWnd, hDC );
		}
		break;
	case WM_MOUSEMOVE:
		{
			tme.cbSize = sizeof( TRACKMOUSEEVENT );
			tme.dwFlags = TME_LEAVE;
			tme.hwndTrack = hWnd;
			tme.dwHoverTime = 500;
			_TrackMouseEvent( &tme );
		}
		break;		
	case WM_LBUTTONUP:
		{
			if ( LOWORD( lParam ) < 84 && HIWORD( lParam ) < 27 )
			{
				char Buffer[ MAX_PLAYER_NAME ] = "";
				GetDlgItemText( m_hDlg, IDC_EDIT_PERSONNAME, Buffer, MAX_PLAYER_NAME - 1 );

				SI32 index = pclclient->GetCurrentCharInfoList() ;

				cltGameMsgRequest_RecommendPerson clRecommendPerson( Buffer,index );
				cltMsg clMsg( GAMEMSG_REQUEST_RECOMMENDPERSON, sizeof(clRecommendPerson), (char*)&clRecommendPerson );
				pclClient->SendMsgToServer((char*)&clMsg);
			}
		}
		break;
	}

	return CallWindowProc( m_OldOKButtonProc, hWnd, iMsg, wParam, lParam );
}

LRESULT CALLBACK CRecommendPersonDlg::StaticCancelButtonSubProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	cltClient *pclclient = (cltClient*)pclClient;
	return pclclient->m_pRecommendPersonDlg->CancelButtonSubProc( hWnd, iMsg, wParam, lParam );
}

LRESULT CALLBACK CRecommendPersonDlg::CancelButtonSubProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	HBITMAP hOldBitmap;

	TRACKMOUSEEVENT tme;

	switch ( iMsg )
	{
	case WM_NCHITTEST:
		{
			int nHit = DefWindowProc( hWnd, iMsg, wParam, lParam );
			
			if ( nHit == HTCLIENT )
			{
				HDC hDC = GetDC( hWnd );
				HDC mdc;
				mdc = CreateCompatibleDC( hDC );
				
				hOldBitmap = (HBITMAP)SelectObject( mdc, m_hButtonBitmap );
				BitBlt( 
					hDC,         // destination DC 
					0, // x upper left 
					0,  // y upper left 
					98,
					28,
					mdc,    // source device context 
					98,
					28,      // x and y upper left 								
					SRCCOPY);  // raster operation
				
				SelectObject( mdc, hOldBitmap );
				DeleteDC( mdc ); 
				ReleaseDC( hWnd, hDC );
			}		
		}
		break;
		
		
	case WM_MOUSELEAVE:
		{
			HDC hDC = GetDC( hWnd );
			HDC mdc;
			mdc = CreateCompatibleDC( hDC );

			hOldBitmap = (HBITMAP)SelectObject( mdc, m_hButtonBitmap );
			BitBlt( 
				hDC,         // destination DC 
				0, // x upper left 
				0,  // y upper left 
				98,
				28,
				mdc,    // source device context 
				0,
				28,      // x and y upper left 								
				SRCCOPY);  // raster operation

			SelectObject( mdc, hOldBitmap );
			DeleteDC( mdc ); 
			ReleaseDC( hWnd, hDC );
		}
		break;
		
	case WM_MOUSEMOVE:
		{
			tme.cbSize = sizeof( TRACKMOUSEEVENT );
			tme.dwFlags = TME_LEAVE;
			tme.hwndTrack = hWnd;
			tme.dwHoverTime = 500;
			_TrackMouseEvent( &tme );
			
		}
		break;		
	case WM_LBUTTONUP:
		{
			if ( LOWORD( lParam ) < 84 && HIWORD( lParam ) < 27 )
			{
				Hide();
			}
		}
		break;
	}

	return CallWindowProc( m_OldCancelButtonProc, hWnd, iMsg, wParam, lParam );
}

bool CRecommendPersonDlg::IsShow()
{
	return m_bShow;
}

void CRecommendPersonDlg::Show()
{
	if ( m_bShow )
	{
		Hide();
		return;
	}

	m_bShow = true;

	SetFocus(GetDlgItem(m_hDlg,IDC_EDIT_PERSONNAME));

	ShowWindow( m_hDlg, SW_SHOW );

	return;
}

void CRecommendPersonDlg::Hide()
{
	m_bShow = false;
	ShowWindow( m_hDlg, SW_HIDE );

	return;
}