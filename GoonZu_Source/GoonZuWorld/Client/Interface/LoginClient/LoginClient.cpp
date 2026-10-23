//#include "LoginClient.h"
//
//CLoginClient	g_LoginClient;
//
//#include "../../Client/Interface/GomZiDlg/GomZiDlg.h"
//
//#include "../../Client/client.h"
//extern cltCommonLogic* pclClient;
//
//CLoginClient::CLoginClient()
//{
//	memset( &m_stLoginData, 0, sizeof( stLoginData ) );
//	memset( m_strErrorMsg, 0, LOGIN_ERROR_MSG_LENGTH );
//
//	m_bErrorExist = false;
//	m_si32Status = LOGIN_READY;
//
//	m_bFlag = false;
//
//	m_hDlg = NULL;
//
//	//m_hAllUserUsingPossibleBitmap = NULL;
//	m_hLoginBackGroundBitmap = NULL;
//	
//	m_hLoginButtonBitmap = NULL;
//	m_hComboSite		 = NULL;
//
//	m_pRegistSiteInfoList = new stRegistSiteInfo[ MAX_REGISTSITE_NUM ];
//	memset( m_pRegistSiteInfoList, 0, sizeof( stRegistSiteInfo ) * MAX_REGISTSITE_NUM );
//
//	Initialize();
//}
//
//CLoginClient::~CLoginClient()
//{
//	Destroy();
//}
//
//void CLoginClient::Initialize()
//{
//
//	return;
//}
//
//void CLoginClient::Destroy()
//{	
//	if ( m_pRegistSiteInfoList )
//	{
//		delete [] m_pRegistSiteInfoList;
//		m_pRegistSiteInfoList = NULL;
//	}
//
//	if ( m_hDlg )
//	{
//		DestroyWindow( m_hDlg );
//	}
//
//	return;
//}
//
//void CLoginClient::Create( HINSTANCE hInst, HWND hParent )
//{
//	m_hDlg = CreateDialog( hInst, MAKEINTRESOURCE( IDD_DIALOG_LOGIN ), hParent, StaticLoginClientDlgProc );
//	ShowWindow( m_hDlg, SW_HIDE );
//	return;
//}
//
//bool CLoginClient::GetExistLoginID()
//{
//	FILE	*fp = _tfopen( TEXT("./Log.Dat"), TEXT("rt"));
//
//	if ( fp == NULL )
//	{
//		return false;
//	}
//
//	_ftscanf( fp, TEXT("%s"), m_stLoginData.m_strUserID );
//	
//	fclose( fp );
//
//	return true;
//}
//
//void CLoginClient::SetLoginData( TCHAR *strID, TCHAR *strPassword )
//{
//	SI32 Index = SendMessage( GetDlgItem( m_hDlg, IDC_COMBO_SELECT_SITE ), CB_GETCURSEL, 0, 0 );
//	
//	if ( Index == CB_ERR )
//	{
//		return;
//	}
//
//	if ( m_pRegistSiteInfoList[ Index ].m_siIndex < 1 )
//	{
//		return;
//	}
//	else
//	{
//		FILE	*fp = _tfopen( TEXT("./Log.Dat"), TEXT("wt"));
//
//		if ( fp == NULL )
//		{
//			return;
//		}
//		
//		_ftprintf( fp, TEXT("%s"), strID );
//		
//		fclose( fp );
//		
//		// SBS 선택
//		if ( m_pRegistSiteInfoList[ Index ].m_siIndex == 2 )
//		{
//			// s@ 를 붙여야 함으로.
//			if ( _tcslen( strID ) + 2 >= MAX_PLAYER_NAME )
//			{
//				return;
//			}
//
//			TCHAR TempID[ MAX_PLAYER_NAME ] = TEXT("");
//			StringCchCopy( TempID, MAX_PLAYER_NAME, TEXT("s_") );
//			StringCchCat( TempID, MAX_PLAYER_NAME, strID );
//			
//			StringCchCopy( m_stLoginData.m_strUserID, MAX_PLAYER_NAME, TempID );
//			StringCchCopy( m_stLoginData.m_strUserPassword, MAX_PLAYER_NAME, strPassword );
//		}
//		else
//		{
//			StringCchCopy( m_stLoginData.m_strUserID, MAX_PLAYER_NAME, strID );
//			StringCchCopy( m_stLoginData.m_strUserPassword, MAX_PLAYER_NAME, strPassword );
//		}		
//	}
//	
//	
//
//	
//
//	
//
//	return;
//}
//
//void CLoginClient::GetLoginData( stLoginData *pLoginData )
//{
//	memcpy( pLoginData, &m_stLoginData, sizeof( stLoginData ) );
//	return;
//}
//
//LRESULT CALLBACK CLoginClient::StaticIDInputEditSubProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
//{
//	return g_LoginClient.IDInputEditSubProc( hWnd, iMsg, wParam, lParam );
//}
//
//LRESULT CALLBACK CLoginClient::IDInputEditSubProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
//{
//	switch ( iMsg )
//	{
//	case WM_CHAR:
//		{
//			if ( m_hDlg )
//			{
//				switch( wParam )
//				{
//				case VK_RETURN:
//					{
//						TCHAR TempID[ LOGIN_ERROR_MSG_LENGTH ] =TEXT("");
//						TCHAR TempPassword[ LOGIN_ERROR_MSG_LENGTH ] =TEXT("");
//						
//						GetWindowText( m_hIDInputEdit, TempID, LOGIN_ERROR_MSG_LENGTH - 1 );
//						GetWindowText( m_hPasswordInputEdit, TempPassword, LOGIN_ERROR_MSG_LENGTH - 1 );
//						
//						if ( _tcslen( TempID ) < 1 || _tcslen( TempPassword ) < 1 )
//						{
//							SetWindowText( m_hIDInputEdit, TEXT("") );
//							SetWindowText( m_hPasswordInputEdit, TEXT("") );
//							TCHAR* pText = GetTxtFromMgr(5065);
//							Show( pText );
//							
//							m_si32Status = LOGIN_READY;
//						}
//						else
//						{
//							SetLoginData( TempID, TempPassword );
//														
//							SetWindowText( m_hIDInputEdit, TEXT("") );
//							SetWindowText( m_hPasswordInputEdit, TEXT("") );
//							
//							m_si32Status = LOGIN_OK;
//						}
//					}
//					break;
//				case VK_TAB:
//					{
//						SetFocus( m_hPasswordInputEdit );
//					}
//					break;
//				case VK_ESCAPE:
//					{
//						m_si32Status = LOGIN_CANCEL;
//						ShowWindow( m_hDlg, SW_HIDE );
//					}
//					break;
//				}
//			}
//		}
//		break;
//	}
//
//	return CallWindowProc( m_OldIDInputEditProc, hWnd, iMsg, wParam, lParam );
//}
//
//LRESULT CALLBACK CLoginClient::StaticPasswordInputEditSubProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
//{
//	return g_LoginClient.PasswordInputEditSubProc( hWnd, iMsg, wParam, lParam );
//}
//
//LRESULT CALLBACK CLoginClient::PasswordInputEditSubProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
//{
//	switch ( iMsg )
//	{
//	case WM_CHAR:
//		{
//			if ( m_hDlg )
//			{
//				switch( wParam )
//				{
//				case VK_RETURN:
//					{
//						TCHAR TempID[ LOGIN_ERROR_MSG_LENGTH ] =TEXT("");
//						TCHAR TempPassword[ LOGIN_ERROR_MSG_LENGTH ] =TEXT("");
//						
//						GetWindowText( m_hIDInputEdit, TempID, LOGIN_ERROR_MSG_LENGTH - 1 );
//						GetWindowText( m_hPasswordInputEdit, TempPassword, LOGIN_ERROR_MSG_LENGTH - 1 );
//						
//						if ( _tcslen( TempID ) < 1 || _tcslen( TempPassword ) < 1 )
//						{
//							SetWindowText( m_hIDInputEdit, TEXT("") );
//							SetWindowText( m_hPasswordInputEdit, TEXT("") );
//							TCHAR* pText = GetTxtFromMgr(5065);
//							Show( pText );
//							
//							m_si32Status = LOGIN_READY;
//						}
//						else
//						{
//							SetLoginData( TempID, TempPassword );
//														
//							SetWindowText( m_hIDInputEdit, TEXT("") );
//							SetWindowText( m_hPasswordInputEdit, TEXT("") );
//							
//							m_si32Status = LOGIN_OK;
//						}
//					}
//					break;
//				case VK_TAB:
//					{
//						SetFocus( m_hOKButton );
//					}
//					break;
//				case VK_ESCAPE:
//					{
//						m_si32Status = LOGIN_CANCEL;
//						ShowWindow( m_hDlg, SW_HIDE );
//					}
//					break;
//				}
//			}
//		}
//		break;
//	}
//
//	return CallWindowProc( m_OldPasswordInputEditProc, hWnd, iMsg, wParam, lParam );
//}
//
//LRESULT CALLBACK CLoginClient::StaticOKButtonSubProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
//{
//	return g_LoginClient.OKButtonSubProc( hWnd, iMsg, wParam, lParam );
//}
//
//LRESULT CALLBACK CLoginClient::OKButtonSubProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
//{
//	HBITMAP hOldBitmap;
//
//	TRACKMOUSEEVENT tme;
//
//	switch ( iMsg )
//	{
//	case WM_NCHITTEST:
//		{
//			int nHit = DefWindowProc( hWnd, iMsg, wParam, lParam );
//			
//			if ( nHit == HTCLIENT )
//			{
//				HDC hDC = GetDC( hWnd );
//				HDC mdc;
//				mdc = CreateCompatibleDC( hDC );
//				
//				hOldBitmap = (HBITMAP)SelectObject( mdc, m_hLoginButtonBitmap );
//				BitBlt( 
//					hDC,         // destination DC 
//					0, // x upper left 
//					0,  // y upper left 
//					98,
//					28,
//					mdc,    // source device context 
//					98,
//					0,      // x and y upper left 								
//					SRCCOPY);  // raster operation
//				
//				SelectObject( mdc, hOldBitmap );
//				DeleteDC( mdc ); 
//				ReleaseDC( hWnd, hDC );
//			}		
//		}
//		break;
//		
//		
//	case WM_MOUSELEAVE:
//		{
//			HDC hDC = GetDC( hWnd );
//			HDC mdc;
//			mdc = CreateCompatibleDC( hDC );
//
//			hOldBitmap = (HBITMAP)SelectObject( mdc, m_hLoginButtonBitmap );
//			BitBlt( 
//				hDC,         // destination DC 
//				0, // x upper left 
//				0,  // y upper left 
//				98,
//				28,
//				mdc,    // source device context 
//				0,
//				0,      // x and y upper left 								
//				SRCCOPY);  // raster operation
//
//			SelectObject( mdc, hOldBitmap );
//			DeleteDC( mdc ); 
//			ReleaseDC( hWnd, hDC );
//		}
//		break;
//		
//	case WM_MOUSEMOVE:
//		{
//			tme.cbSize = sizeof( TRACKMOUSEEVENT );
//			tme.dwFlags = TME_LEAVE;
//			tme.hwndTrack = hWnd;
//			tme.dwHoverTime = 500;
//			_TrackMouseEvent( &tme );
//			
//		}
//		break;		
//		
//	case WM_LBUTTONUP:
//		{
//			if ( LOWORD( lParam ) < 84 && HIWORD( lParam ) < 27 )
//			{
//				TCHAR TempID[ LOGIN_ERROR_MSG_LENGTH ] =TEXT("");
//				TCHAR TempPassword[ LOGIN_ERROR_MSG_LENGTH ] =TEXT("");
//				
//				GetWindowText( m_hIDInputEdit, TempID, LOGIN_ERROR_MSG_LENGTH - 1 );
//				GetWindowText( m_hPasswordInputEdit, TempPassword, LOGIN_ERROR_MSG_LENGTH - 1 );
//				
//				if ( _tcslen( TempID ) < 1 || _tcslen( TempPassword ) < 1 )
//				{
//					SetWindowText( m_hIDInputEdit, TEXT("") );
//					SetWindowText( m_hPasswordInputEdit, TEXT("") );
//					
//					TCHAR* pText = GetTxtFromMgr(5065);
//					Show(pText);
//					
//					m_si32Status = LOGIN_READY;
//				}
//				else
//				{	
//					SetLoginData( TempID, TempPassword );					
//					
//					SetWindowText( m_hIDInputEdit, TEXT("") );
//					SetWindowText( m_hPasswordInputEdit, TEXT("") );
//					
//					m_si32Status = LOGIN_OK;
//				}
//			}
//		}
//		break;
//	case WM_CHAR:
//		{
//			if ( m_hDlg )
//			{
//				switch( wParam )
//				{
//				case VK_RETURN:
//					{
//						TCHAR TempID[ LOGIN_ERROR_MSG_LENGTH ] =TEXT("");
//						TCHAR TempPassword[ LOGIN_ERROR_MSG_LENGTH ] =TEXT("");
//						
//						GetWindowText( m_hIDInputEdit, TempID, LOGIN_ERROR_MSG_LENGTH - 1 );
//						GetWindowText( m_hPasswordInputEdit, TempPassword, LOGIN_ERROR_MSG_LENGTH - 1 );
//						
//						if ( _tcslen( TempID ) < 1 || _tcslen( TempPassword ) < 1 )
//						{
//							SetWindowText( m_hIDInputEdit, TEXT("") );
//							SetWindowText( m_hPasswordInputEdit, TEXT("") );
//							
//							TCHAR* pText = GetTxtFromMgr(5065);
//							Show(pText);
//							
//							m_si32Status = LOGIN_READY;
//						}
//						else
//						{
//							SetLoginData( TempID, TempPassword );
//							
//							SetWindowText( m_hIDInputEdit, TEXT("") );
//							SetWindowText( m_hPasswordInputEdit, TEXT("") );
//							
//							m_si32Status = LOGIN_OK;
//						}
//					}
//					break;
//				case VK_TAB:
//					{
//						SetFocus( m_hIDInputEdit );
//					}
//					break;
//				case VK_ESCAPE:
//					{
//						m_si32Status = LOGIN_CANCEL;
//						ShowWindow( m_hDlg, SW_HIDE );
//					}
//					break;
//				}
//			}
//		}
//		break;
//	}
//	
//	return CallWindowProc( m_OldOKButtonProc, hWnd, iMsg, wParam, lParam );
//}
//
//LRESULT CALLBACK CLoginClient::StaticCancelButtonSubProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
//{
//	return g_LoginClient.CancelButtonSubProc( hWnd, iMsg, wParam, lParam );
//}
//
//LRESULT CALLBACK CLoginClient::CancelButtonSubProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
//{
//	HBITMAP hOldBitmap;
//
//	TRACKMOUSEEVENT tme;
//
//	switch ( iMsg )
//	{
//	case WM_NCHITTEST:
//		{
//			int nHit = DefWindowProc( hWnd, iMsg, wParam, lParam );
//			
//			if ( nHit == HTCLIENT )
//			{
//				HDC hDC = GetDC( hWnd );
//				HDC mdc;
//				mdc = CreateCompatibleDC( hDC );
//				
//				hOldBitmap = (HBITMAP)SelectObject( mdc, m_hLoginButtonBitmap );
//				BitBlt( 
//					hDC,         // destination DC 
//					0, // x upper left 
//					0,  // y upper left 
//					98,
//					28,
//					mdc,    // source device context 
//					98,
//					28,      // x and y upper left 								
//					SRCCOPY);  // raster operation
//				
//				SelectObject( mdc, hOldBitmap );
//				DeleteDC( mdc ); 
//				ReleaseDC( hWnd, hDC );
//			}		
//		}
//		break;
//		
//		
//	case WM_MOUSELEAVE:
//		{
//			HDC hDC = GetDC( hWnd );
//			HDC mdc;
//			mdc = CreateCompatibleDC( hDC );
//
//			hOldBitmap = (HBITMAP)SelectObject( mdc, m_hLoginButtonBitmap );
//			BitBlt( 
//				hDC,         // destination DC 
//				0, // x upper left 
//				0,  // y upper left 
//				98,
//				28,
//				mdc,    // source device context 
//				0,
//				28,      // x and y upper left 								
//				SRCCOPY);  // raster operation
//
//			SelectObject( mdc, hOldBitmap );
//			DeleteDC( mdc ); 
//			ReleaseDC( hWnd, hDC );
//		}
//		break;
//		
//	case WM_MOUSEMOVE:
//		{
//			tme.cbSize = sizeof( TRACKMOUSEEVENT );
//			tme.dwFlags = TME_LEAVE;
//			tme.hwndTrack = hWnd;
//			tme.dwHoverTime = 500;
//			_TrackMouseEvent( &tme );
//			
//		}
//		break;		
//		
//	case WM_LBUTTONUP:
//		{
//			if ( LOWORD( lParam ) < 84 && HIWORD( lParam ) < 27 )
//			{
//				m_si32Status = LOGIN_CANCEL;
//				ShowWindow( m_hDlg, SW_HIDE );
//			}
//		}
//		break;
//	case WM_CHAR:
//		{
//			if ( m_hDlg )
//			{
//				switch( wParam )
//				{
//				case VK_RETURN:
//					{
//						m_si32Status = LOGIN_CANCEL;
//						ShowWindow( m_hDlg, SW_HIDE );
//					}
//					break;
//				case VK_TAB:
//					{
//						SetFocus( m_hIDInputEdit );
//					}
//					break;
//				case VK_ESCAPE:
//					{
//						m_si32Status = LOGIN_CANCEL;
//						ShowWindow( m_hDlg, SW_HIDE );
//					}
//					break;
//				}
//			}
//		}
//		break;
//	}
//	
//	return CallWindowProc( m_OldCancelButtonProc, hWnd, iMsg, wParam, lParam );
//}
//
//LRESULT CALLBACK CLoginClient::StaticRegistButtonSubProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
//{
//	return g_LoginClient.RegistButtonSubProc( hWnd, iMsg, wParam, lParam );
//}
//
//LRESULT CALLBACK CLoginClient::RegistButtonSubProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
//{
//	HBITMAP hOldBitmap;
//
//	TRACKMOUSEEVENT tme;
//
//	switch ( iMsg )
//	{
//	case WM_NCHITTEST:
//		{
//			int nHit = DefWindowProc( hWnd, iMsg, wParam, lParam );
//			
//			if ( nHit == HTCLIENT )
//			{
//				HDC hDC = GetDC( hWnd );
//				HDC mdc;
//				mdc = CreateCompatibleDC( hDC );
//				
//				hOldBitmap = (HBITMAP)SelectObject( mdc, m_hLoginButtonBitmap );
//				BitBlt( 
//					hDC,         // destination DC 
//					0, // x upper left 
//					0,  // y upper left 
//					98,
//					28,
//					mdc,    // source device context 
//					98,
//					28 * 2,      // x and y upper left 								
//					SRCCOPY);  // raster operation
//				
//				SelectObject( mdc, hOldBitmap );
//				DeleteDC( mdc ); 
//				ReleaseDC( hWnd, hDC );
//			}			
//		}
//		break;
//		
//		
//	case WM_MOUSELEAVE:
//		{
//			HDC hDC = GetDC( hWnd );
//			HDC mdc;
//			mdc = CreateCompatibleDC( hDC );
//
//			hOldBitmap = (HBITMAP)SelectObject( mdc, m_hLoginButtonBitmap );
//			BitBlt( 
//				hDC,         // destination DC 
//				0, // x upper left 
//				0,  // y upper left 
//				98,
//				28,
//				mdc,    // source device context 
//				0,
//				28*2,      // x and y upper left 								
//				SRCCOPY);  // raster operation
//
//			SelectObject( mdc, hOldBitmap );
//			DeleteDC( mdc ); 
//			ReleaseDC( hWnd, hDC );
//		}
//		break;
//		
//	case WM_MOUSEMOVE:
//		{
//			tme.cbSize = sizeof( TRACKMOUSEEVENT );
//			tme.dwFlags = TME_LEAVE;
//			tme.hwndTrack = hWnd;
//			tme.dwHoverTime = 500;
//			_TrackMouseEvent( &tme );			
//		}
//		break;
//
//
//	case WM_LBUTTONUP:
//		{
//			if ( LOWORD( lParam ) < 84 && HIWORD( lParam ) < 27 )
//			{
//				m_si32Status = LOGIN_ID_REGIST;
//
//				switch( pclClient->siServiceArea )
//				{
//				case ConstServiceArea_Japan:
//					ShellExecute( NULL, NULL, TEXT("http://kunshu.jp"), NULL, NULL, SW_SHOWNORMAL );
//					break;
//				default:
//					ShellExecute( NULL, NULL, TEXT("http://global.goonzu.com"), NULL, NULL, SW_SHOWNORMAL );
//					break;
//				}
//			}
//		}
//		break;
//
//	case WM_CHAR:
//		{
//			if ( m_hDlg )
//			{
//				switch( wParam )
//				{
//				case VK_RETURN:
//					{
//						m_si32Status = LOGIN_ID_REGIST;
//						if(pclClient->siServiceArea == ConstServiceArea_English)
//						{
//							ShellExecute( NULL, NULL, TEXT("http://global.goonzu.com/center/account/agreement.asp?menu=61"), NULL, NULL, SW_SHOWNORMAL );
//
//						}
//						else
//						{							
//							ShellExecute( NULL, NULL, TEXT("http://kunshu.jp"), NULL, NULL, SW_SHOWNORMAL );
//						}
//					}
//					break;
//				case VK_TAB:
//					{
//						SetFocus( m_hIDInputEdit );
//					}
//					break;
//				case VK_ESCAPE:
//					{
//						m_si32Status = LOGIN_CANCEL;
//						ShowWindow( m_hDlg, SW_HIDE );
//					}
//					break;
//				}
//			}
//		}
//		break;
//	}
//	
//	return CallWindowProc( m_OldRegistButtonProc, hWnd, iMsg, wParam, lParam );
//}
//
//void CLoginClient::DrawTransparentBitmap(HDC hdc, HBITMAP hBitmap, short xStart,
//										 short yStart, COLORREF cTransparentColor)
//{
//	BITMAP     bm;
//	COLORREF   cColor;
//	HBITMAP    bmAndBack, bmAndObject, bmAndMem, bmSave;
//	HBITMAP    bmBackOld, bmObjectOld, bmMemOld, bmSaveOld;
//	HDC        hdcMem, hdcBack, hdcObject, hdcTemp, hdcSave;
//	POINT      ptSize;
//	
//	hdcTemp = CreateCompatibleDC(hdc);
//	SelectObject(hdcTemp, hBitmap);   // Select the bitmap
//	
//	GetObject(hBitmap, sizeof(BITMAP), (LPSTR)&bm);
//	ptSize.x = bm.bmWidth;            // Get width of bitmap
//	ptSize.y = bm.bmHeight;           // Get height of bitmap
//	DPtoLP(hdcTemp, &ptSize, 1);      // Convert from device
//	
//	// to logical points
//	
//	// Create some DCs to hold temporary data.
//	hdcBack   = CreateCompatibleDC(hdc);
//	hdcObject = CreateCompatibleDC(hdc);
//	hdcMem    = CreateCompatibleDC(hdc);
//	hdcSave   = CreateCompatibleDC(hdc);
//	
//	// Create a bitmap for each DC. DCs are required for a number of
//	// GDI functions.
//	
//	// Monochrome DC
//	bmAndBack   = CreateBitmap(ptSize.x, ptSize.y, 1, 1, NULL);
//	
//	// Monochrome DC
//	bmAndObject = CreateBitmap(ptSize.x, ptSize.y, 1, 1, NULL);
//	
//	bmAndMem    = CreateCompatibleBitmap(hdc, ptSize.x, ptSize.y);
//	bmSave      = CreateCompatibleBitmap(hdc, ptSize.x, ptSize.y);
//	
//	// Each DC must select a bitmap object to store pixel data.
//	bmBackOld   = (HBITMAP)SelectObject(hdcBack, bmAndBack);
//	bmObjectOld = (HBITMAP)SelectObject(hdcObject, bmAndObject);
//	bmMemOld    = (HBITMAP)SelectObject(hdcMem, bmAndMem);
//	bmSaveOld   = (HBITMAP)SelectObject(hdcSave, bmSave);
//	
//	// Set proper mapping mode.
//	SetMapMode(hdcTemp, GetMapMode(hdc));
//	
//	// Save the bitmap sent here, because it will be overwritten.
//	BitBlt(hdcSave, 0, 0, ptSize.x, ptSize.y, hdcTemp, 0, 0, SRCCOPY);
//	
//	// Set the background color of the source DC to the color.
//	// contained in the parts of the bitmap that should be transparent
//	cColor = SetBkColor(hdcTemp, cTransparentColor);
//	
//	// Create the object mask for the bitmap by performing a BitBlt
//	// from the source bitmap to a monochrome bitmap.
//	BitBlt(hdcObject, 0, 0, ptSize.x, ptSize.y, hdcTemp, 0, 0,
//		SRCCOPY);
//	
//	// Set the background color of the source DC back to the original
//	// color.
//	SetBkColor(hdcTemp, cColor);
//	
//	// Create the inverse of the object mask.
//	BitBlt(hdcBack, 0, 0, ptSize.x, ptSize.y, hdcObject, 0, 0,
//		NOTSRCCOPY);
//	
//	// Copy the background of the main DC to the destination.
//	BitBlt(hdcMem, 0, 0, ptSize.x, ptSize.y, hdc, xStart, yStart,
//		SRCCOPY);
//	
//	// Mask out the places where the bitmap will be placed.
//	BitBlt(hdcMem, 0, 0, ptSize.x, ptSize.y, hdcObject, 0, 0, SRCAND);
//	
//	// Mask out the transparent colored pixels on the bitmap.
//	BitBlt(hdcTemp, 0, 0, ptSize.x, ptSize.y, hdcBack, 0, 0, SRCAND);
//	
//	// XOR the bitmap with the background on the destination DC.
//	BitBlt(hdcMem, 0, 0, ptSize.x, ptSize.y, hdcTemp, 0, 0, SRCPAINT);
//	
//	// Copy the destination to the screen.
//	BitBlt(hdc, xStart, yStart, ptSize.x, ptSize.y, hdcMem, 0, 0,
//		SRCCOPY);
//	
//	// Place the original bitmap back into the bitmap sent here.
//	BitBlt(hdcTemp, 0, 0, ptSize.x, ptSize.y, hdcSave, 0, 0, SRCCOPY);
//	
//	// Delete the memory bitmaps.
//	DeleteObject(SelectObject(hdcBack, bmBackOld));
//	DeleteObject(SelectObject(hdcObject, bmObjectOld));
//	DeleteObject(SelectObject(hdcMem, bmMemOld));
//	DeleteObject(SelectObject(hdcSave, bmSaveOld));
//	
//	// Delete the memory DCs.
//	DeleteDC(hdcMem);
//	DeleteDC(hdcBack);
//	DeleteDC(hdcObject);
//	DeleteDC(hdcSave);
//	DeleteDC(hdcTemp);
//} 
//
//
//
//BOOL CALLBACK CLoginClient::StaticLoginClientDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
//{
//	return g_LoginClient.LoginClientDlgProc( hDlg, iMsg, wParam, lParam );
//}
//
//
//
//BOOL CALLBACK CLoginClient::LoginClientDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
//{
//	HDC hdcMem; 
//    LPDRAWITEMSTRUCT lpdis; 
//	cltClient *pclclient = ( cltClient* ) pclClient;
//
//	switch ( iMsg )
//	{
//	case WM_INITDIALOG:
//		{
//			HDC hdc= GetDC( hDlg );
//			
//			m_hLoginBackGroundBitmap = pclClient->pclItemManager->MakeDDBFromDIB( hdc, TEXT("Interface/Login/LoginBackGround.bmp") );
//			SendDlgItemMessage( hDlg, IDC_STATIC_LOGIN_BACKGROUND, STM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)m_hLoginBackGroundBitmap );
//			
//			ReleaseDC(hDlg, hdc);		
//
//			m_hIDInputEdit = GetDlgItem( hDlg, IDC_EDIT_LOGINID );
//			m_hPasswordInputEdit = GetDlgItem( hDlg, IDC_EDIT_LOGINPASSWORD );
//			m_hExplainEdit = GetDlgItem( hDlg, IDC_EDIT_LOGINEXPLAIN );
//
//			m_hOKButton = GetDlgItem( hDlg, IDOK );
//
//			SendMessage( m_hIDInputEdit, EM_LIMITTEXT, MAX_PLAYER_NAME - 1, 0 );
//			SendMessage( m_hPasswordInputEdit, EM_LIMITTEXT, MAX_PLAYER_NAME - 1, 0 );
//
//			m_hCancelButton = GetDlgItem( hDlg, IDCANCEL );
//			m_hRegistButton = GetDlgItem( hDlg, IDC_BUTTON_REGIST );
//
//			m_OldIDInputEditProc = (WNDPROC)SetWindowLong( m_hIDInputEdit, GWL_WNDPROC, (LONG)StaticIDInputEditSubProc);
//			m_OldPasswordInputEditProc = (WNDPROC)SetWindowLong( m_hPasswordInputEdit, GWL_WNDPROC, (LONG)StaticPasswordInputEditSubProc);
//			m_OldOKButtonProc = (WNDPROC)SetWindowLong( m_hOKButton, GWL_WNDPROC, (LONG)StaticOKButtonSubProc);
//			m_OldCancelButtonProc = (WNDPROC)SetWindowLong( m_hCancelButton, GWL_WNDPROC, (LONG)StaticCancelButtonSubProc);
//			m_OldRegistButtonProc = (WNDPROC)SetWindowLong( m_hRegistButton, GWL_WNDPROC, (LONG)StaticRegistButtonSubProc);
//
//			MoveWindow( m_hIDInputEdit, 18, 77, 120, 17, TRUE );
//			MoveWindow( m_hPasswordInputEdit, 17, 127, 120, 17, TRUE );
//			MoveWindow( m_hOKButton, 7, 157, 98, 28, TRUE );
//			MoveWindow( m_hCancelButton, 105, 157, 98, 28, TRUE );
//			MoveWindow( m_hRegistButton, 203, 157, 98, 28, TRUE );
//			MoveWindow( m_hExplainEdit, 7, 192, 294, 45, TRUE );
//
//			m_hLoginButtonBitmap = (HBITMAP)LoadImage( pclclient->GetInstance(), TEXT("Interface/Login/LoginButton.bmp"),IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE ); 
//			
//			FILE *fp = NULL ;
//			if ( pclClient->siServiceArea == ConstServiceArea_Korea )
//			{
//				fp = _tfopen( TEXT("./RegistSite/RegistSite.dat"), TEXT("rt") );
//			}
//			else if ( pclClient->siServiceArea == ConstServiceArea_China)
//			{
//				fp = _tfopen( TEXT("./RegistSite/RegistSiteC.dat"), TEXT("rt") );
//			}
//			else
//			{
//				fp = _tfopen( TEXT("./RegistSite/RegistSiteE.dat"), TEXT("rt") );
//			}
//
//			if ( fp != NULL )
//			{
//				SI32 Count = 0;
//				while(!feof( fp ))
//				{
//					_ftscanf( fp, TEXT("%s %d %d"), m_pRegistSiteInfoList[ Count ].m_strSiteName, &m_pRegistSiteInfoList[ Count ].m_siIndex, &m_pRegistSiteInfoList[ Count ].m_siDefaultSite );
//					++Count;
//				}
//
//				fclose( fp );
//			}
//			else
//			{
//				// RegistSite.dat 파일이 없는 경우는 그냥 군주가 디폴트임
//				TCHAR* pText = GetTxtFromMgr(599);
//				StringCchCopy( m_pRegistSiteInfoList[ 0 ].m_strSiteName, MAX_PLAYER_NAME, pText );
//				m_pRegistSiteInfoList[ 0 ].m_siIndex = 1;
//				m_pRegistSiteInfoList[ 0 ].m_siDefaultSite = 1;
//			}
//
//			m_siCurrentSelectedSiteIndex = 0;
//
//			m_hComboSite = GetDlgItem( hDlg, IDC_COMBO_SELECT_SITE );
//			if(m_hComboSite)
//			{
//				SI16 siDefaultSite = 0;
//
//				for ( SI16 i = 0; i < MAX_REGISTSITE_NUM; ++i )
//				{	
//					if ( m_pRegistSiteInfoList[ i ].m_siIndex > 0 )
//					{
//						SendDlgItemMessage(hDlg, IDC_COMBO_SELECT_SITE, CB_ADDSTRING, 0, (LPARAM)m_pRegistSiteInfoList[ i ].m_strSiteName );
//
//						if ( m_pRegistSiteInfoList[ i ].m_siDefaultSite > 0 )
//						{
//							siDefaultSite = i;
//						}
//					}
//				}
//
//				m_siCurrentSelectedSiteIndex = m_pRegistSiteInfoList[ siDefaultSite ].m_siIndex;
//				SendMessage( GetDlgItem( hDlg, IDC_COMBO_SELECT_SITE ), CB_SETCURSEL, siDefaultSite, 0 );
//			}
//			
//			return TRUE;
//		}
//		break;
//	case WM_CTLCOLOREDIT:
//		{
//			if ( m_hIDInputEdit == (HWND)lParam || m_hPasswordInputEdit == (HWND)lParam )
//			{
//				SetBkColor((HDC)wParam, RGB(214, 255, 255));
//				return TRUE;
//			}
//
//			if ( m_hExplainEdit == (HWND)lParam )
//			{
//				//SetBkColor((HDC)wParam, RGB( 58, 206, 222 ) );
//				return TRUE;
//			}
//		}
//		break;
//		
//	case WM_PAINT:
//		{
//			/*
//			if ( IsWindow(hDlg))
//			{
//				pclclient->m_pLoadingImg->Draw(pclclient->lpBackScreen,
//					pclclient->GetScreenRX(),pclclient->GetScreenRY());
//			}
//			*/
//		}
//		break;
//
//	case WM_MEASUREITEM:
//		{
//			// 각 항목의 높이는 20 픽셀로 설정한다.
//			LPMEASUREITEMSTRUCT lpmis=(LPMEASUREITEMSTRUCT)lParam;
//			switch(lpmis->CtlID)
//			{
//			case IDC_COMBO_SELECT_SITE:	lpmis->itemHeight = 17;	break;
//			}
//		}
//		return TRUE;
//	case WM_DRAWITEM:
//		{
//			lpdis = (LPDRAWITEMSTRUCT) lParam; 
//			
//			if(lpdis->CtlType == ODT_COMBOBOX)
//			{
//				switch( lpdis->CtlID )
//				{
//					case IDC_COMBO_SELECT_SITE:
//					{
//						HBRUSH bkBrush;
//						// 선택된 항목은 파란 배경에 흰색으로 출력한다.
//						if (lpdis->itemState & ODS_SELECTED) 
//						{
//							bkBrush=CreateSolidBrush(RGB(150,190,189));
//							SetBkColor(lpdis->hDC,   RGB(150,190,189));
//							SetTextColor(lpdis->hDC, RGB(0,0,0));
//						}
//						else 
//						{
//							bkBrush=CreateSolidBrush(RGB(215,255,254));
//							SetBkColor(lpdis->hDC,   RGB(215,255,254));
//							SetTextColor(lpdis->hDC, RGB(0,0,0));
//						}
//						FillRect(lpdis->hDC, &lpdis->rcItem, bkBrush);
//						DeleteObject(bkBrush);
//						
//						TCHAR szName[256];
//						if(SendDlgItemMessage(hDlg, IDC_COMBO_SELECT_SITE, CB_GETLBTEXT, lpdis->itemID, (LPARAM)szName)>0)
//						{
//							TextOut(lpdis->hDC,lpdis->rcItem.left+5, lpdis->rcItem.top+3, szName , _tcslen(szName));
//						}
//					
//					}
//					break;
//				}
//			}
//			else if ( lpdis->CtlType == ODT_BUTTON )
//			{
//				HBITMAP	hOldLoginBitmap;
//				hdcMem = CreateCompatibleDC(lpdis->hDC); 
//				
//				hOldLoginBitmap = (HBITMAP)SelectObject(hdcMem, m_hLoginButtonBitmap );
//				
//				switch( lpdis->CtlID )
//				{
//				case IDOK:
//					{
//						if ( lpdis->itemState & ODS_SELECTED )
//						{
//							BitBlt( 
//								lpdis->hDC,         // destination DC 
//								lpdis->rcItem.left, // x upper left 
//								lpdis->rcItem.top,  // y upper left 
//								98,
//								28,
//								hdcMem,    // source device context 
//								98,
//								0,      // x and y upper left 								
//								SRCCOPY);  // raster operation
//						}
//						else
//						{
//							BitBlt( 
//								lpdis->hDC,         // destination DC 
//								lpdis->rcItem.left, // x upper left 
//								lpdis->rcItem.top,  // y upper left 
//								98,
//								28,
//								hdcMem,    // source device context 
//								0,
//								0,      // x and y upper left 								
//								SRCCOPY);  // raster operation
//						}
//					}
//					break;
//				case IDCANCEL:
//					{
//						if ( lpdis->itemState & ODS_SELECTED )
//						{
//							BitBlt( 
//								lpdis->hDC,         // destination DC 
//								lpdis->rcItem.left, // x upper left 
//								lpdis->rcItem.top,  // y upper left 
//								98,
//								28,
//								hdcMem,    // source device context 
//								98,
//								28,      // x and y upper left 								
//								SRCCOPY);  // raster operation
//						}
//						else
//						{
//							BitBlt( 
//								lpdis->hDC,         // destination DC 
//								lpdis->rcItem.left, // x upper left 
//								lpdis->rcItem.top,  // y upper left 
//								98,
//								28,
//								hdcMem,    // source device context 
//								0,
//								28,      // x and y upper left 								
//								SRCCOPY);  // raster operation
//						}
//					}
//					break;
//				case IDC_BUTTON_REGIST:
//					{
//						if ( lpdis->itemState & ODS_SELECTED )
//						{
//							BitBlt( 
//								lpdis->hDC,         // destination DC 
//								lpdis->rcItem.left, // x upper left 
//								lpdis->rcItem.top,  // y upper left 
//								98,
//								28,
//								hdcMem,    // source device context 
//								98,
//								28 * 2,      // x and y upper left 								
//								SRCCOPY);  // raster operation
//						}
//						else
//						{
//							BitBlt( 
//								lpdis->hDC,         // destination DC 
//								lpdis->rcItem.left, // x upper left 
//								lpdis->rcItem.top,  // y upper left 
//								98,
//								28,
//								hdcMem,    // source device context 
//								0,
//								28 * 2,      // x and y upper left 								
//								SRCCOPY);  // raster operation
//						}
//					}
//					break;
//				}
//				
//				SelectObject( hdcMem, hOldLoginBitmap );
//				DeleteDC(hdcMem); 
//				return TRUE;
//				
//			}
//		}
//		break;
//		
//	case WM_COMMAND:
//		{			
//			switch( LOWORD( wParam ) )
//			{
//			case IDC_COMBO_SELECT_SITE:
//				switch( HIWORD( wParam ))
//				{
//				case CBN_SELCHANGE:
//					{
//						SI32 Index = SendMessage( GetDlgItem( hDlg, IDC_COMBO_SELECT_SITE ), CB_GETCURSEL, 0, 0 );
//						if ( Index == CB_ERR )
//						{
//							return FALSE;
//						}
//
//						m_siCurrentSelectedSiteIndex = m_pRegistSiteInfoList[ Index ].m_siIndex;
//
//						TCHAR Buffer[ 1024 ] = TEXT("");
//						/*sprintf( Buffer, TEXT("%s에서 회원가입하신 분은 %s를 선택하시면 됩니다."),
//										 m_pRegistSiteInfoList[ Index ].m_strSiteName,
//										 m_pRegistSiteInfoList[ Index ].m_strSiteName );*/
//						for ( SI16 i = 0 ; i < MAX_REGISTSITE_NUM ; i++ )
//						{
//							if ( m_pRegistSiteInfoList[i].m_siIndex <= 0 ) continue ;
//							TCHAR buf[256] = TEXT("") ;
//							TCHAR* pText = GetTxtFromMgr(3918);
//
//							//sprintf( buf, pText,
//							//				 m_pRegistSiteInfoList[i].m_strSiteName,
//							//				 m_pRegistSiteInfoList[i].m_strSiteName );
//							Nsprintf(Buffer,pText,TEXT("where"), m_pRegistSiteInfoList[i].m_strSiteName   ,TEXT("what"), m_pRegistSiteInfoList[i].m_strSiteName    ,NULL);
//							StringCchCat(Buffer, 1024, buf);
//						}
//						SetWindowText( GetDlgItem( hDlg, IDC_EDIT_LOGINEXPLAIN ), Buffer );
//
//						FILE *fp = NULL ;
//						if ( pclClient->siServiceArea == ConstServiceArea_Korea )
//						{
//							fp = _tfopen( TEXT("./RegistSite/RegistSite.dat"), TEXT("wt") );
//						}
//						else if( pclClient->siServiceArea == ConstServiceArea_China )
//						{
//                            fp = _tfopen( TEXT("./RegistSite/RegistSiteC.dat"), TEXT("wt") );
//						}
//						else
//						{
//							fp = _tfopen( TEXT("./RegistSite/RegistSiteE.dat"), TEXT("wt") );
//						}
//
//						for ( i = 0; i < MAX_REGISTSITE_NUM; ++i )
//						{
//							if ( m_pRegistSiteInfoList[i].m_siIndex <= 0 ) continue ;
//							if ( i == Index )
//							{
//								m_pRegistSiteInfoList[i].m_siDefaultSite = 1 ;
//							}
//							else
//							{
//								m_pRegistSiteInfoList[i].m_siDefaultSite = 0 ;
//							}
//							_ftprintf(fp, TEXT("%s %d %d\n"),
//									m_pRegistSiteInfoList[i].m_strSiteName,
//									m_pRegistSiteInfoList[i].m_siIndex,
//									m_pRegistSiteInfoList[i].m_siDefaultSite);
//						}
//
//						fclose( fp );
//					}
//					break;
//				}
//				break;
//			}
//			
//		}
//		
//		break;
//	case WM_CLOSE:
//		{
//			m_si32Status = LOGIN_CANCEL;
//			ShowWindow( hDlg, SW_HIDE );
//		}
//		break;
//	case WM_DESTROY:
//		{
//			SetWindowLong( m_hIDInputEdit, GWL_WNDPROC, (LONG) m_OldIDInputEditProc );
//			SetWindowLong( m_hPasswordInputEdit, GWL_WNDPROC, (LONG) m_OldPasswordInputEditProc );
//			SetWindowLong( m_hOKButton, GWL_WNDPROC, (LONG) m_OldOKButtonProc );
//			SetWindowLong( m_hCancelButton, GWL_WNDPROC, (LONG) m_OldCancelButtonProc );
//			SetWindowLong( m_hRegistButton, GWL_WNDPROC, (LONG) m_OldRegistButtonProc );
//
//			/*
//			if ( m_hAllUserUsingPossibleBitmap )
//			{
//				DeleteObject( m_hAllUserUsingPossibleBitmap );
//				m_hAllUserUsingPossibleBitmap = NULL;
//			}
//			*/
//
//			if ( m_hLoginBackGroundBitmap )
//			{
//				DeleteObject( m_hLoginBackGroundBitmap );
//				m_hLoginBackGroundBitmap = NULL;
//			}
//
//			if ( m_hLoginButtonBitmap )
//			{
//				DeleteObject( m_hLoginButtonBitmap );
//				m_hLoginButtonBitmap = NULL;
//			}
//
//			
//			m_hDlg = NULL;			
//		}
//		break;
//	}
//
//	return FALSE;
//}
//
//int	CLoginClient::GetStatus()
//{
//	int si32Status = m_si32Status;
//	if ( m_si32Status == LOGIN_OK )
//	{
//		m_si32Status = LOGIN_READY;
//	}
//
//	return si32Status;
//}
//
//void CLoginClient::SmallShow()
//{
//	ShowWindow( m_hDlg, SW_SHOWNORMAL );
//	SetForegroundWindow( m_hDlg );
//	//SetWindowPos( m_hDlg, HWND_TOP, 0, 0, 308, 142, SWP_NOMOVE | SWP_SHOWWINDOW );
//	return;
//}
//
//void CLoginClient::BigShow()
//{
//	ShowWindow( m_hDlg, SW_SHOWNORMAL );	
//	SetForegroundWindow( m_hDlg );
//	//SetWindowPos( m_hDlg, HWND_TOP, 0, 0, 308, 220, SWP_NOMOVE | SWP_SHOWWINDOW );
//	return;
//}
//
//void CLoginClient::Show( TCHAR *strErrorMsg )
//{
//	if ( m_hDlg == NULL )
//	{
//		return;
//	}
//
//	m_bFlag = true;
//
//	if ( strErrorMsg == NULL )
//	{
//		if ( m_bErrorExist )
//		{
//			BigShow();
//		}
//		else
//		{
//			SmallShow();
//		}
//	}
//	else
//	{
//		memset( m_strErrorMsg, 0, LOGIN_ERROR_MSG_LENGTH );
//		m_bErrorExist = true;
//		StringCchCopy( m_strErrorMsg, LOGIN_ERROR_MSG_LENGTH, strErrorMsg );
//		SetWindowText( m_hExplainEdit, strErrorMsg );
//		BigShow();
//	}
//
//	if ( GetExistLoginID() )
//	{
//		SetWindowText( m_hIDInputEdit, m_stLoginData.m_strUserID );
//		SetFocus( m_hPasswordInputEdit );
//	}
//
//	return;
//}
//
//void CLoginClient::Hide()
//{
//	if ( m_hDlg == NULL )
//	{
//		return;
//	}
//
//	m_bFlag = false;
//	ShowWindow( m_hDlg, SW_HIDE );
//
//	DestroyWindow( m_hDlg );	
//}
//
//bool CLoginClient::IsShow()
//{
//	if ( m_hDlg == NULL )
//	{
//		return false;
//	}
//
//	return m_bFlag;
//}
//
//void CLoginClient::ShowMiniHomePage()
//{	
//	return;
//	//TCHAR strURL[_MAX_PATH]; // = TEXT("http://goonzu.ndoors.com/");
//
//	//pclClient->GetMyCZUrl( strURL, m_stLoginData.m_strUserID );
//
//	//cltClient *pclclient = (cltClient *)pclClient;
//	////pclclient->m_pGomZiDlg->Set( strURL );
//	//
//	//
//	//TCHAR* pText = GetTxtFromMgr(2190);
//	//HWND hFindWindow = ::FindWindow(NULL, pText );
//	//
//	//if ( hFindWindow )
//	//{
//	//	COPYDATASTRUCT cds;
//	//	cds.dwData = 0;
//	//	cds.cbData = _tcslen ( strURL ) + 1;
//	//	cds.lpData = strURL;
//	//	
//	//	SendMessage( hFindWindow, WM_COPYDATA, (WPARAM) pclClient->GetHwnd(), (LPARAM) &cds );
//	//
//	//}
//	//else
//	//{
//	//	cltClient *pclclient = (cltClient*)pclClient;
//	//	if ( pclclient->szID != NULL && pclclient->szPassword != NULL && _tcslen( pclclient->szID ) > 0 && _tcslen( pclclient->szPassword ) > 0 )
//	//	{
//	//		
//	//		TCHAR Buffer[ 512 ] = TEXT("");
//	//		//sprintf( Buffer, TEXT("%s ret=%s&uid=%s&passwd=%s"), TEXT("http://sign.ndoors.com/confirm/login.dll?check"), strURL, pclclient->szID, pclclient->szPassword );
//	//		StringCchPrintf( Buffer, 512, TEXT("%s %s"), pclclient->szID, pclclient->szPassword );
//	//		ShellExecute( pclClient->GetHwnd(), NULL, TEXT("MiniHome.exe"), Buffer, NULL, SW_SHOWNOACTIVATE );
//	//		ShowWindow( pclClient->GetHwnd(), SW_SHOW );
//	//	}
//	//	else
//	//	{
//	//		ShellExecute( pclClient->GetHwnd(), NULL, TEXT("MiniHome.exe"), strURL, NULL, SW_SHOWNOACTIVATE );
//	//		ShowWindow( pclClient->GetHwnd(), SW_SHOW );
//	//	}
//	//}
//}