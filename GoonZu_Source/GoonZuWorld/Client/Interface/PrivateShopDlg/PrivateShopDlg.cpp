#include "PrivateShopDlg.h"

CPrivateShopDlg g_PrivateShopDlg;

CPrivateShopDlg::CPrivateShopDlg()
{
	m_bShow = false;
	m_hDlg = NULL;
}

CPrivateShopDlg::~CPrivateShopDlg()
{
	if ( m_hDlg )
	{
		DestroyWindow( m_hDlg );
	}
}

void CPrivateShopDlg::Initialize()
{
	return;
}

void CPrivateShopDlg::Destroy()
{
	return;
}

void CPrivateShopDlg::Create( HINSTANCE hInst, HWND hWnd )
{
	m_hDlg = ::CreateDialog( hInst, MAKEINTRESOURCE(IDD_DIALOG_PRIVATE_SHOP_SELL), hWnd, StaticPrivateShopDlgProc );
	Hide();

	return;
}

BOOL CALLBACK CPrivateShopDlg::StaticPrivateShopDlgProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam )
{
	return g_PrivateShopDlg.PrivateShopDlgProc( hDlg, iMsg, wParam, lParam );
}

BOOL CALLBACK CPrivateShopDlg::PrivateShopDlgProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam )
{
	switch( iMsg )
	{
	case WM_INITDIALOG:
		{
			m_hDlg = hDlg;
		}
		break;

	case WM_DESTROY:
		{
			m_hDlg = NULL;
		}
		break;
	}

	return FALSE;
}

bool CPrivateShopDlg::IsShow()
{
	return m_bShow;
}

void CPrivateShopDlg::Show()
{
	m_bShow = true;

	ShowWindow( m_hDlg, SW_SHOW );


	return;
}

void CPrivateShopDlg::Hide()
{
	m_bShow = false;

	ShowWindow( m_hDlg, SW_HIDE );

	return;
}
