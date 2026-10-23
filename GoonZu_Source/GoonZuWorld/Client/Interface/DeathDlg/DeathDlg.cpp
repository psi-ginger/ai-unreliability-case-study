/*
#include "DeathDlg.h"

#include "..\..\Client\Client.h"

//-----------------------------
// Common
//-----------------------------
#include "PersonInfo.h"
#include "Char\CharManager\CharManager.h"

extern cltCommonLogic* pclClient;

CDeathDlg		g_DeathDlg;

#define DEATHDLG_TIME		200

CDeathDlg::CDeathDlg()
{
	m_hDeathDlg = NULL ;
	m_siBitmapAnimationIndex = 0 ;
	m_siSex = 0 ;
}

CDeathDlg::~CDeathDlg()
{
	if( m_hDeathDlg )
	{
		DestroyWindow( m_hDeathDlg );
	}
}

void CDeathDlg::Create( HINSTANCE hInst, HWND hParent )
{
	m_hDeathDlg = CreateDialog( hInst, MAKEINTRESOURCE( IDD_DEATH ), hParent, StatiCDeathDlgProc );
	m_hDeathEdit = GetDlgItem( m_hDeathDlg, IDC_EDIT_DEATH );
	
	Hide();
}

BOOL CALLBACK CDeathDlg::StatiCDeathDlgProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	return g_DeathDlg.DeathDlgProc( hDlg, iMsg, wParam, lParam );
}

BOOL CALLBACK CDeathDlg::DeathDlgProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam )
{
	switch( iMsg )
	{
	case WM_INITDIALOG:
		{
			m_hHeroBitmap[0] = (HBITMAP)LoadImage( pclClient->GetInstance(), "Interface\\DeathHero\\DeathHero0.BMP", IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE );
			m_hHeroBitmap[1] = (HBITMAP)LoadImage( pclClient->GetInstance(), "Interface\\DeathHero\\DeathHero1.BMP", IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE );
			m_hHeroBitmap[2] = (HBITMAP)LoadImage( pclClient->GetInstance(), "Interface\\DeathHero\\DeathHero2.BMP", IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE );
			m_hHeroBitmap[3] = (HBITMAP)LoadImage( pclClient->GetInstance(), "Interface\\DeathHero\\DeathHero3.BMP", IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE );

			m_hHeroineBitmap[0] = (HBITMAP)LoadImage( pclClient->GetInstance(), "Interface\\DeathHeroine\\DeathHeroine0.BMP", IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE );
			m_hHeroineBitmap[1] = (HBITMAP)LoadImage( pclClient->GetInstance(), "Interface\\DeathHeroine\\DeathHeroine1.BMP", IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE );
			m_hHeroineBitmap[2] = (HBITMAP)LoadImage( pclClient->GetInstance(), "Interface\\DeathHeroine\\DeathHeroine2.BMP", IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE );
			m_hHeroineBitmap[3] = (HBITMAP)LoadImage( pclClient->GetInstance(), "Interface\\DeathHeroine\\DeathHeroine3.BMP", IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE );
		}
		break;

	case WM_TIMER:
		{
			if ( pclClient->pclCM->CR[1]->pclCI->clBI.GetKind() == 1 )
			{
				SendDlgItemMessage( hDlg, IDC_STATIC_DEATH_INFO, STM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)m_hHeroBitmap[m_siBitmapAnimationIndex++] );

				if ( m_siBitmapAnimationIndex >= 4 )
					m_siBitmapAnimationIndex = 0;
			}
			else
			{
				SendDlgItemMessage( hDlg, IDC_STATIC_DEATH_INFO, STM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)m_hHeroineBitmap[m_siBitmapAnimationIndex++] );

				if ( m_siBitmapAnimationIndex >= 4 )
					m_siBitmapAnimationIndex = 0;
			}
		}
		break;
	case WM_COMMAND:
		{
			switch( wParam )
			{
			case IDCANCEL:
				{
					Hide();
				}
				break;
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
			KillTimer(m_hDeathDlg,0);
			for ( int i = 0; i < 4; ++i )
			{
				if ( m_hHeroBitmap[i] )
				{
					DeleteObject( m_hHeroBitmap[i] );
					m_hHeroBitmap[i] = NULL;
				}

				if ( m_hHeroineBitmap[i] )
				{
					DeleteObject( m_hHeroineBitmap[i] );
					m_hHeroineBitmap[i] = NULL;
				}
			}

			m_hDeathDlg = NULL;
		}
		break;
	}

	return FALSE;
}

void CDeathDlg::set(SI32 money , SI32 exp, SI16 killmode, char* pname )
{
	char strbuf[256] = "" ;

	sprintf(strbuf,"[%s]의 공격으로 사망했습니다.\r\n돈 [%d]냥과 경험치 [%d]를 잃었습니다.",pname, money,exp);
	SetWindowText( m_hDeathEdit, strbuf );

	Show();

	return;
}

void CDeathDlg::Show()
{
	m_siBitmapAnimationIndex = 0;
	SetTimer( m_hDeathDlg, 0, DEATHDLG_TIME, NULL );	
	ShowWindow( m_hDeathDlg, SW_SHOW );
}

void CDeathDlg::Hide()
{
	m_siBitmapAnimationIndex = 0;
	KillTimer(m_hDeathDlg,0);
	ShowWindow( m_hDeathDlg, SW_HIDE );
}
*/