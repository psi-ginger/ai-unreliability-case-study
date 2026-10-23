#include "ServerOnStepNotifyDlg.h"
#include "../CommonLogic/TextMgr/TextMgr.h"
#include "../CommonLogic/CommonLogic.h"

#include <Graphic.h>
#include "../../Lib/NGraphics/Graphics.h"
#include <Graphic_Capsule.h>
#include "Client.h"

extern BOOL g_bFullScreen;		// 풀스크린 여부 

extern LPDIRECTDRAWSURFACE7 g_lpBackScreen;	// 게임이 그려질 화면 뒤의 서피스. 

CServerOnStepNotifyDlg g_ServerOnStepNotifyDlg;

extern cltCommonLogic* pclClient;

CServerOnStepNotifyDlg::CServerOnStepNotifyDlg()
{
	m_hDlg = NULL;
	
	m_hLoadingImage = NULL;
	m_hProgress = NULL;
	
	m_bShow = false;

	m_hEditBrush = NULL;

	m_siPercent = 0;
	m_siGameScreenMode=0;

	BackGoundXPos=0;
	BackGoundYPos=0;
	ProgressXPos=0;
	ProgressyPos=0;
}

CServerOnStepNotifyDlg::~CServerOnStepNotifyDlg()
{
	if ( m_hDlg )
	{
		DestroyWindow( m_hDlg );
	}
}

void CServerOnStepNotifyDlg::Create( HINSTANCE hInst, HWND hWnd, SI16 GameMode, SI16 GameScreenMode, SI16 siServiceArea )
{
	m_siGameMode = GameMode;

	if ( m_siGameMode != GAMEMODE_CLIENT )
	{
		return;
	}

	m_hParentWnd = hWnd;
	m_siGameScreenMode= GameScreenMode;


	if ( g_bFullScreen )
	{	
		return;
	}

	m_siGameMode = GameMode;

	m_hInst = hInst;
	
	if ( ConstServiceArea_China		== siServiceArea ||
		ConstServiceArea_Japan		== siServiceArea ||
		ConstServiceArea_Korea		== siServiceArea || 
		ConstServiceArea_Taiwan		== siServiceArea || 
		ConstServiceArea_USA		== siServiceArea ||
		ConstServiceArea_English	== siServiceArea ||
		ConstServiceArea_NHNChina	== siServiceArea ||
		ConstServiceArea_EUROPE		== siServiceArea
	)
	{
		m_hDlg = CreateDialog( hInst, MAKEINTRESOURCE( IDD_DIALOG_SERVERONSTEPNOTIFY_J ), hWnd, StaticServerOnStepNotifyDlgProc );
	}
	else
	{
		m_hDlg = CreateDialog( hInst, MAKEINTRESOURCE( IDD_DIALOG_SERVERONSTEPNOTIFY ), hWnd, StaticServerOnStepNotifyDlgProc );
	}

	COLORREF	barColor = RGB( 240, 144, 56 );
	COLORREF	backColor = RGB( 232, 232, 248 );
	SendMessage( m_hProgress, PBM_SETBARCOLOR, 0, (LPARAM)barColor);
	SendMessage( m_hProgress, PBM_SETBKCOLOR, 0, (LPARAM)backColor);

	Hide(); //로고 화면 먼저 출력을 위해서 숨겨서 생성
///	Show();
}
void CServerOnStepNotifyDlg::LoadSpr(SI32 siServiceArea)
{
	if(	ConstServiceArea_Japan		== siServiceArea ||
		ConstServiceArea_China		== siServiceArea ||
		ConstServiceArea_Korea		== siServiceArea ||
		ConstServiceArea_Taiwan		== siServiceArea ||
		ConstServiceArea_USA		== siServiceArea ||
		ConstServiceArea_English	== siServiceArea ||
		ConstServiceArea_NHNChina	== siServiceArea || 
		ConstServiceArea_EUROPE		== siServiceArea 
	)
	{
		GP.LoadSprFromFile(TEXT("./loading/interface_662X481_00_000.SPR"),&m_BKSpr );
		//GP.LoadSprFromFile(TEXT("./loading/IMAGE_638X18_00_000.SPR"), &m_ProgressSpr );
		GP.LoadSprFromFile(TEXT("./loading/IMAGE_4X18_00_000.SPR"), &m_ProgressSpr );
	}
	else
	{
		GP.LoadSprFromFile(TEXT("./loading/interface_304x174_00_000.spr"),&m_BKSpr );
		GP.LoadSprFromFile(TEXT("./loading/image_4x19_00_000.spr"), &m_ProgressSpr );
	}
}

BOOL CALLBACK CServerOnStepNotifyDlg::StaticServerOnStepNotifyDlgProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam )
{
	return g_ServerOnStepNotifyDlg.ServerOnStepNotifyDlgProc( hDlg, iMsg, wParam, lParam );
}

BOOL CALLBACK CServerOnStepNotifyDlg::ServerOnStepNotifyDlgProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam )
{
	switch( iMsg )
	{
	case WM_INITDIALOG:
		{
			InitCommonControls();
			
			m_hDlg = hDlg;

			/*
			HWND hStatic = GetDlgItem( hDlg, IDC_STATIC_LOADINGIMAGE );
			if ( m_hLoadingImage != NULL )
			{
				if( SendMessage( hStatic, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM) m_hLoadingImage ) == NULL )
				{
					int b = 0;
				}
			}
			*/

			m_hProgress = GetDlgItem ( hDlg, IDC_PROGRESS_STEP );

			// EditBrush 
			m_hEditBrush = CreateSolidBrush( COLOR_DIALOG_WHITEBLUE );
			
			SendMessage( m_hProgress, PBM_SETRANGE, 0, MAKELPARAM( 0, 100 ) );
			SendMessage( m_hProgress, PBM_SETPOS, 0, 0 );
			
		}
		return TRUE;
	/*
	case WM_PAINT:
		{
			HDC hdc;
			PAINTSTRUCT ps;
			
			hdc = BeginPaint( hDlg, &ps);
			
			HDC mdc;
			mdc = CreateCompatibleDC( hdc );
			HBITMAP hOldBitmap = (HBITMAP)SelectObject( mdc, m_hLoadingImage );
			BitBlt( hdc, 0, 0, 344, 207, mdc, 0, 0, SRCCOPY );
			SelectObject( mdc, hOldBitmap );
			DeleteDC( mdc );

			EndPaint( hDlg, &ps);			
		}
		break;
	*/
	/*
	case WM_CTLCOLORSTATIC:
		{
			if ( GetDlgItem( hDlg, IDC_STATIC_PROGRESS_PERCENT ) == (HWND)lParam )
			{
				HDC hdc = (HDC)wParam;
				SetTextColor( hdc, COLOR_DIALOG_BLACK );
				SetBkColor( hdc, COLOR_DIALOG_WHITEBLUE);
				return (BOOL)m_hEditBrush;
			}
		}
		break;
	*/

	case WM_COMMAND:
		{
			switch( LOWORD( wParam ) )
			{
			case IDOK:
			case IDCANCEL:
				Hide();
				break;	
			}
		}
		break;

	case WM_CLOSE:
		{
			DestroyWindow( m_hDlg );
		}
		break;

	case WM_DESTROY:
		{
			if ( m_hEditBrush )
			{
				DeleteObject( m_hEditBrush );
				m_hEditBrush = NULL;
			}

			if ( m_hLoadingImage )
			{
				DeleteObject( m_hLoadingImage );
				m_hLoadingImage = NULL;
			}

			m_hDlg = NULL;
		}
		break;
	}

	return FALSE;
}
	
bool CServerOnStepNotifyDlg::IsShow()
{
	return m_bShow;
}

void CServerOnStepNotifyDlg::Show()
{
	if ( m_siGameMode != GAMEMODE_CLIENT )
	{
		return;
	}

	m_bShow = true;
	ShowWindow( m_hDlg, SW_SHOW );

	switch ( pclClient->siServiceArea )
	{
		case ConstServiceArea_Korea:
			m_hLoadingImage = (HBITMAP)LoadImage( m_hInst, TEXT("GImg/LoadingK.BMP"), IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE );
			break;
		case ConstServiceArea_China:
			m_hLoadingImage = (HBITMAP)LoadImage( m_hInst, TEXT("GImg/LoadingC.BMP"), IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE );
			break;
		case ConstServiceArea_Japan:
			m_hLoadingImage = (HBITMAP)LoadImage( m_hInst, TEXT("GImg/LoadingJ.BMP"), IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE );
			break;
		case ConstServiceArea_English:
			m_hLoadingImage = (HBITMAP)LoadImage( m_hInst, TEXT("GImg/LoadingE.BMP"), IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE );
			break;
		case ConstServiceArea_Taiwan:
			m_hLoadingImage = (HBITMAP)LoadImage( m_hInst, TEXT("GImg/LoadingT.BMP"), IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE );
			break;
		case ConstServiceArea_USA:
			m_hLoadingImage = (HBITMAP)LoadImage( m_hInst, TEXT("GImg/LoadingU.BMP"), IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE );
			break;
		case ConstServiceArea_NHNChina:
			m_hLoadingImage = (HBITMAP)LoadImage( m_hInst, TEXT("GImg/LoadingC.BMP"), IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE );
			break;
		case ConstServiceArea_EUROPE:
			m_hLoadingImage = (HBITMAP)LoadImage( m_hInst, TEXT("GImg/LoadingG.BMP"), IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE );
			break;
		default:
			m_hLoadingImage = (HBITMAP)LoadImage( m_hInst, TEXT("GImg/Loading.BMP"), IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE );
			break;
	}

	return;
}

void CServerOnStepNotifyDlg::Hide()
{
	if ( m_siGameMode != GAMEMODE_CLIENT )
	{
		return;
	}
	
	m_bShow = false;
	ShowWindow( m_hDlg, SW_HIDE );

	return;
}

void CServerOnStepNotifyDlg::Set( SI32 Percent )
{
	if ( m_siGameMode != GAMEMODE_CLIENT )
	{
		return;
	}

	if ( Percent < 0 )
	{
		Percent = 0;
	}

	m_siPercent = Percent;

	
	
	if ( g_bFullScreen )
	{
		if ( m_siPercent > 100 )
		{
			if ( m_BKSpr.pImage )
			{
				GP.FreeSpr( m_BKSpr );
			}

			if ( m_ProgressSpr.pImage )
			{
				GP.FreeSpr( m_ProgressSpr );
			}
		}
		else
		{
			Draw();
			
		}

		return;
	}

	if ( m_hDlg == NULL || m_siGameMode != GAMEMODE_CLIENT )
	{
		return;
	}

	if ( Percent > 100 )
	{
		DestroyWindow( m_hDlg );
		return;
	}

	if ( m_hLoadingImage )
	{
		SendMessage( GetDlgItem( m_hDlg, IDC_STATIC_LOADINGIMAGE ), STM_SETIMAGE, IMAGE_BITMAP, (LPARAM) m_hLoadingImage );
	}


	SendMessage( m_hProgress, PBM_SETPOS, Percent, 0 );

/*	TCHAR Buffer[256 ] = TEXT("");
	//TCHAR* pText = GetTxtFromMgr(2068);
	if ( pclClient->siServiceArea == ConstServiceArea_Korea )
		StringCchPrintf( Buffer, 256, TEXT("로딩중 - %d%%"), Percent );
	else if( pclClient->siServiceArea == ConstServiceArea_China )
		StringCchPrintf( Buffer, 256, TEXT("Now Loading... - %d%%"), Percent );
	else
		StringCchPrintf( Buffer, 256, TEXT("Now Loading... - %d%%"), Percent );

	//sprintf( Buffer,pText, Percent );
	//SetWindowText( GetDlgItem( m_hDlg, IDC_STATIC_PROGRESS_PERCENT ), Buffer );
	SetWindowText( m_hDlg, Buffer );
*/
	return;
}

void CServerOnStepNotifyDlg::Draw()
{
	if( m_BKSpr.pImage == NULL ) return;

	if ( m_siGameMode != GAMEMODE_CLIENT )
	{
		return;
	}
	
	SI32 XDrawlocation=0;

	BackGoundXPos= (1024 - m_BKSpr.GetXSize() ) / 2 + 90;
	BackGoundYPos= (768 - m_BKSpr.GetYSize() ) / 2 + 40;

	if(	pclClient->siServiceArea == ConstServiceArea_Japan		|| 
		pclClient->siServiceArea == ConstServiceArea_China		||
		pclClient->siServiceArea == ConstServiceArea_Korea		||
		pclClient->siServiceArea == ConstServiceArea_Taiwan		||
		pclClient->siServiceArea == ConstServiceArea_USA		||
		pclClient->siServiceArea == ConstServiceArea_English	||
		pclClient->siServiceArea == ConstServiceArea_NHNChina	||
		pclClient->siServiceArea == ConstServiceArea_EUROPE
	)
	{
		ProgressXPos= BackGoundXPos+13;
		ProgressyPos= BackGoundYPos+447;
	}
	else
	{
		ProgressXPos= BackGoundXPos + 2;
		ProgressyPos= BackGoundYPos + 153;
	}

	int prowidth = m_BKSpr.GetXSize();
	int curwidth;

	int maxwidth;

	if(	pclClient->siServiceArea == ConstServiceArea_Japan		|| 
		pclClient->siServiceArea == ConstServiceArea_China		||
		pclClient->siServiceArea == ConstServiceArea_Korea		||
		pclClient->siServiceArea == ConstServiceArea_Taiwan		||
		pclClient->siServiceArea == ConstServiceArea_USA		||
		pclClient->siServiceArea == ConstServiceArea_English	||
		pclClient->siServiceArea == ConstServiceArea_NHNChina	||
		pclClient->siServiceArea == ConstServiceArea_EUROPE
	)
	{
		maxwidth = 636;
	}
	else
	{
		maxwidth = 300;
	}


	LPDIRECTDRAWSURFACE7 lpBackBuffer = g_graphics.GetBackBuffer();
	if( GP.LockSurface( lpBackBuffer ) == TRUE )
	{
		if ( m_siPercent <= 100 )
		{
			GP.PutSpr(&m_BKSpr,BackGoundXPos, BackGoundYPos, 0 );

			XDrawlocation = 0;

			curwidth = (int)(( m_siPercent / 100. ) * maxwidth);

			//while( XDrawlocation < curwidth - m_ProgressSpr.GetXSize() )
			//{
			//	GP.PutSpr(&m_ProgressSpr,ProgressXPos + XDrawlocation , ProgressyPos, 0 );
			//	XDrawlocation += 1;
			//}

			while( XDrawlocation < curwidth )
			{
				GP.PutSpr(&m_ProgressSpr, ProgressXPos + XDrawlocation , ProgressyPos, 0 );
				XDrawlocation += 4;
			}

			//GP.PutSpr(&m_ProgressSpr,ProgressXPos + curwidth - m_ProgressSpr.GetXSize(), ProgressyPos , 0 );

		}

		GP.UnlockSurface( lpBackBuffer );

		((cltClient *)pclClient)->UpdateScreen( m_hParentWnd, lpBackBuffer, true );
	}

	return;
}
