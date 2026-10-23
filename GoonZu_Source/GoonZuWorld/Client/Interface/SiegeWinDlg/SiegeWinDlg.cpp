// SiegeWinDlg.cpp: implementation of the CSiegeWinDlg class.
//
//////////////////////////////////////////////////////////////////////
/*
#include "SiegeWinDlg.h"
#include "../../Client/client.h"

extern cltCommonLogic* pclClient;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSiegeWinDlg::CSiegeWinDlg()
{
	m_hSiegeWinDlg = NULL ;
	m_hSiegeWinStatic = NULL ;
}

CSiegeWinDlg::~CSiegeWinDlg()
{
	if ( m_hSiegeWinDlg )
	{
		DestroyWindow(m_hSiegeWinDlg);
	}
}

void CSiegeWinDlg::Create( HINSTANCE hInst, HWND hParent )
{
	m_hSiegeWinDlg = CreateDialog( hInst, MAKEINTRESOURCE( IDD_DIALOG_SIEGE_WIN ), hParent, StatiCSiegeWinProc );
	m_hSiegeWinStatic = GetDlgItem(m_hSiegeWinDlg,IDC_STATIC_SIEGE_WIN);

	Hide();
}

BOOL CALLBACK CSiegeWinDlg::StatiCSiegeWinProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	cltClient *pclclient = (cltClient*)pclClient;
	return pclclient->m_pSiegeWinDlg->SiegeWinProc( hDlg, iMsg, wParam, lParam );
}

BOOL CALLBACK CSiegeWinDlg::SiegeWinProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam )
{
	switch( iMsg )
	{
	case WM_INITDIALOG:
		{
		}
		break;
	case WM_CLOSE:
		{
			Hide();
		}
		break;
	case WM_DESTROY:
		{
			Hide();
			m_hSiegeWinDlg = NULL ;
		}
		break;
	}

	return FALSE;
}

void CSiegeWinDlg::Set(TCHAR* ptext)
{
	SetWindowText(m_hSiegeWinStatic,ptext);
	Show();
}

void CSiegeWinDlg::Show()
{
	ShowWindow( m_hSiegeWinDlg, SW_SHOW );
}

void CSiegeWinDlg::Hide()
{
	ShowWindow( m_hSiegeWinDlg, SW_HIDE );
}
*/