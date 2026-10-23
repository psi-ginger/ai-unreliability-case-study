#include "../../../Common/CommonHeader.h"
#include "../../Client.h"

#include "ErrorMsgDlg.h"

#include "../../lib/WebHTML/WebHTML.h"

#include "../../NInterface/NSoundOnOffDlg/NSoundOnOffDlg.h"
#include "Interface/SoundOnOffDlg/SoundOnOffDlg.h"

#include "../../NInterface/NErrorMsgDlg/NErrorMsgDlg.h"

extern cltCommonLogic* pclClient;

CErrorMsgDlg	g_ErrorMsg;

CErrorMsgDlg::CErrorMsgDlg()
{
	m_hDlgBrush		= NULL;
	m_hDlgBrush1	= NULL;
	m_hErrorMsgDlg  = NULL;
	m_hErrorMsgDlg1 = NULL;
	
	m_bShowFlag		= false;
	m_bShowFlag1	= false;

	StringCchCopy(m_strLogMsg, sizeof(stLOGData) * MAX_STLOGDATA_PER_PACKET * MAX_RESPONSING_PERSONAL_LOG + 512 , TEXT(""));
}

CErrorMsgDlg::~CErrorMsgDlg()
{
	if(m_hErrorMsgDlg)
	{
		DestroyWindow(m_hErrorMsgDlg);
	}

	if(m_hErrorMsgDlg1)
	{
		DestroyWindow(m_hErrorMsgDlg1);
	}

	SAFE_DELETE_ARRAY(m_pstrPrintBigMsg);
}

void CErrorMsgDlg::Create( HINSTANCE hInst, HWND hParent )
{
	m_hErrorMsgDlg		= CreateDialog( hInst, MAKEINTRESOURCE( IDD_DIALOG_ERRORPRINT ), hParent, StaticErrorMsgDlgProc );
	m_hErrorMsgDlg1		= CreateDialog( hInst, MAKEINTRESOURCE( IDD_DIALOG_ERRORPRINT1 ), hParent, StaticErrorMsgDlgProc1 );
	m_pstrPrintBigMsg	= new TCHAR[MAX_BIG_ERRORMSG_NUM];
	
	Hide();
	Hide1();
}

BOOL CALLBACK CErrorMsgDlg::StaticErrorMsgDlgProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	return g_ErrorMsg.ErrorMsgDlgProc( hDlg, iMsg, wParam, lParam );
}

BOOL CALLBACK CErrorMsgDlg::ErrorMsgDlgProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam )
{
	switch( iMsg )
	{
	case WM_INITDIALOG:
		{
			m_hErrorEdit = GetDlgItem( hDlg, IDC_EDIT_ERROR_PRINT );			
			m_hDlgBrush = CreateSolidBrush( COLOR_DIALOG_YELLOW );
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
	case WM_CTLCOLORSTATIC:
		{
			HDC hdc = (HDC)wParam;
			
			SetTextColor( hdc, COLOR_DIALOG_BLACK );
			SetBkColor( hdc, COLOR_DIALOG_YELLOW );

			if ( GetDlgItem( hDlg, IDC_EDIT_ERROR_PRINT ) == (HWND)lParam )
			{
				return (BOOL)m_hDlgBrush;
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
			m_hErrorMsgDlg = NULL;

			if ( m_hDlgBrush )
			{
				DeleteObject( m_hDlgBrush );
				m_hDlgBrush = NULL;
			}

		}
		break;
	}


	return FALSE;
}

BOOL CALLBACK CErrorMsgDlg::StaticErrorMsgDlgProc1(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	return g_ErrorMsg.ErrorMsgDlgProc1( hDlg, iMsg, wParam, lParam );
}

BOOL CALLBACK CErrorMsgDlg::ErrorMsgDlgProc1(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	
	switch( iMsg )
	{
	case WM_INITDIALOG:
		{			
			m_hErrorEdit1 = GetDlgItem( hDlg, IDC_EDIT_ERROR_PRINT );
			m_hDlgBrush1 = CreateSolidBrush( COLOR_DIALOG_YELLOW );
		}
		break;

	case WM_COMMAND:
		{
			switch( wParam )
			{
			case IDCANCEL:
				{
					Hide1();
				}
				break;
			}
		}
		break;
	case WM_CTLCOLORSTATIC:
		{
			HDC hdc = (HDC)wParam;
			SetTextColor( hdc, COLOR_DIALOG_BLACK );
			SetBkColor( hdc, COLOR_DIALOG_YELLOW);

			if ( GetDlgItem( hDlg, IDC_EDIT_ERROR_PRINT ) == (HWND)lParam )
			{
				return (BOOL)m_hDlgBrush1;
			}
		}
		break;		
	case WM_CLOSE:
		{
			Hide1();
		}
		break;

	case WM_DESTROY:
		{
			m_hErrorMsgDlg1 = NULL;

			if ( m_hDlgBrush1 )
			{
				DeleteObject( m_hDlgBrush1 );
				m_hDlgBrush1 = NULL;
			}

		}
		break;
	}


	return FALSE;
}

void CErrorMsgDlg::Set( bool Activate, TCHAR *strTitleData, TCHAR *strPrintData, ... )
{
	cltClient *pclclient = (cltClient *)pclClient;
	
	if( pclclient->bWatchModeSwitch )
	{
		return;
	}

	va_list ap;
	//memset( m_strPrintMsg, 0, 7168 );
	//memset( m_strTitleMsg, 0, 1024 );
	m_strPrintMsg[0] = NULL;
	m_strTitleMsg[0] = NULL;

	StringCchCopy( m_strTitleMsg, 1024, strTitleData );
	va_start( ap, strPrintData );
	StringCchVPrintf( m_strPrintMsg, 7168, strPrintData, ap );
	va_end( ap );

	if ( g_SoundOnOffDlg.m_bFullScreenMode == false )
	{
		SetWindowText( m_hErrorMsgDlg, strTitleData );
		SetWindowText( m_hErrorEdit, m_strPrintMsg );

		Show( Activate );
	}
	else
	{
		if( pclclient->m_pDialog[ NERRORMSGPRINT_DLG ] == NULL )
			pclclient->CreateInterface( NERRORMSGPRINT_DLG );

		((CNErrorMsgPrint*)pclclient->m_pDialog[ NERRORMSGPRINT_DLG ])->SetTitleCaption( m_strTitleMsg );
		((CNErrorMsgPrint*)pclclient->m_pDialog[ NERRORMSGPRINT_DLG ])->SetEditText( m_strPrintMsg );
	}

	return;
}

void CErrorMsgDlg::BigSet( TCHAR *strTitleData, TCHAR *strPrintData, ... )
{
	if ( NULL == m_pstrPrintBigMsg )
	{
		return;
	}

	cltClient *pclclient = (cltClient *)pclClient;

	va_list ap;

	ZeroMemory( m_pstrPrintBigMsg,	(sizeof(TCHAR) * MAX_BIG_ERRORMSG_NUM) );
	ZeroMemory( m_strTitleMsg,		sizeof(m_strTitleMsg) );

	StringCchCopy( m_strTitleMsg, 1024, strTitleData );
	va_start( ap, strPrintData );
	StringCchVPrintf( m_pstrPrintBigMsg, MAX_BIG_ERRORMSG_NUM, strPrintData, ap );
	va_end( ap );

	if ( g_SoundOnOffDlg.m_bFullScreenMode == false )
	{
		SetWindowText( m_hErrorMsgDlg1, strTitleData );
		SetWindowText( m_hErrorEdit1, m_pstrPrintBigMsg );

		Show1();
	}
	else
	{
		if( pclclient->m_pDialog[ NERRORMSGPRINT1_DLG ] == NULL )
			pclclient->CreateInterface( NERRORMSGPRINT1_DLG );

		((CNErrorMsgPrint1*)pclclient->m_pDialog[ NERRORMSGPRINT1_DLG ])->SetTitleCaption( m_strTitleMsg );
		((CNErrorMsgPrint1*)pclclient->m_pDialog[ NERRORMSGPRINT1_DLG ])->SetEditText( m_pstrPrintBigMsg );

	}

	return;
}

void CErrorMsgDlg::BigAppend( TCHAR *strPrintData )
{
	if ( NULL == m_pstrPrintBigMsg )
	{
		return;
	}

	cltClient *pclclient = (cltClient *)pclClient;

	StringCchCat( m_pstrPrintBigMsg, MAX_BIG_ERRORMSG_NUM, strPrintData );

	if ( g_SoundOnOffDlg.m_bFullScreenMode == false )
	{
		SetWindowText( m_hErrorMsgDlg1, m_strTitleMsg );
		SetWindowText( m_hErrorEdit1, m_pstrPrintBigMsg );

		Show1();
	}
	else
	{
		if( pclclient->m_pDialog[ NERRORMSGPRINT1_DLG ] == NULL )
			pclclient->CreateInterface( NERRORMSGPRINT1_DLG );

		((CNErrorMsgPrint1*)pclclient->m_pDialog[ NERRORMSGPRINT1_DLG ])->SetTitleCaption( m_strTitleMsg );
		((CNErrorMsgPrint1*)pclclient->m_pDialog[ NERRORMSGPRINT1_DLG ])->SetEditText( m_pstrPrintBigMsg );

	}

}

void CErrorMsgDlg::Show( bool Activate )
{
	m_bShowFlag = true;
	//ShowWindow( m_hErrorMsgDlg, SW_SHOW );

	if ( Activate )
	{
		ShowWindow( m_hErrorMsgDlg, SW_SHOW );
		SetForegroundWindow( m_hErrorMsgDlg );
	}
	else
	{
		ShowWindow( m_hErrorMsgDlg, SW_SHOW );
	}
}

void CErrorMsgDlg::Hide()
{
	m_bShowFlag = false;
	ShowWindow( m_hErrorMsgDlg, SW_HIDE );
}

void CErrorMsgDlg::Show1()
{
	m_bShowFlag1 = true;
	ShowWindow( m_hErrorMsgDlg1, SW_SHOW );
}

void CErrorMsgDlg::Hide1()
{
	m_bShowFlag1 = false;
	ShowWindow( m_hErrorMsgDlg1, SW_HIDE );
}

void CErrorMsgDlg::SetLogMsg(SI32 Result,TCHAR * strLogData,SI32 page)
{
	if ( Result == -1 && _tcslen(strLogData) <= 0 )
		ShowLogMsg();
	else if ( Result == -1 && page == MAX_RESPONSING_PERSONAL_LOG - 1 )
		ShowLogMsg();
	else StringCchCat(m_strLogMsg, sizeof(stLOGData) * MAX_STLOGDATA_PER_PACKET * MAX_RESPONSING_PERSONAL_LOG + 512 , strLogData);
}

void CErrorMsgDlg::ShowLogMsg()
{
	cltClient *pclclient = (cltClient *)pclClient;

	if ( g_SoundOnOffDlg.m_bFullScreenMode == false )
	{
		SetWindowText( m_hErrorEdit1, m_strLogMsg );
		SetWindowText( m_hErrorMsgDlg1, TEXT("Log") );

		Show1();
	}
	else
	{
		if( pclclient->m_pDialog[ NERRORMSGPRINT1_DLG ] == NULL )
			pclclient->CreateInterface( NERRORMSGPRINT1_DLG );

		((CNErrorMsgPrint1*)pclclient->m_pDialog[ NERRORMSGPRINT1_DLG ])->SetTitleCaption(  TEXT("Log") );
		((CNErrorMsgPrint1*)pclclient->m_pDialog[ NERRORMSGPRINT1_DLG ])->SetEditText( m_strLogMsg );

	}

	StringCchCopy(m_strLogMsg, sizeof(stLOGData) * MAX_STLOGDATA_PER_PACKET * MAX_RESPONSING_PERSONAL_LOG + 512 , TEXT(""));

	return ;
}