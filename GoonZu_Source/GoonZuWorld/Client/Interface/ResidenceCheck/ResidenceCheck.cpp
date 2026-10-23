// ResidenceCheck.cpp: implementation of the CResidenceCheck class.
//
//////////////////////////////////////////////////////////////////////

#include "ResidenceCheck.h"
#include "../../Client/client.h"

//------------------------------
// CommonLogic
//------------------------------
#include "MsgType-Person.h"


extern cltCommonLogic* pclClient;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CResidenceCheck::CResidenceCheck()
{
	m_hResidenceCheck = NULL ;
	m_hResidenceEdit = NULL ;

	m_hEditBrush = NULL ;
	m_hDlgBrush = NULL ;

	Init();
}

CResidenceCheck::~CResidenceCheck()
{
	Init();

	if ( m_hResidenceCheck )
	{
		DestroyWindow(m_hResidenceCheck);
	}
}

void CResidenceCheck::Create( HINSTANCE hInst, HWND hParent )
{
	m_hResidenceCheck = CreateDialog( hInst, MAKEINTRESOURCE( IDD_DIALOG_RESIDENCE ), hParent, StatiCResidenceCheckProc );
	m_hResidenceEdit = GetDlgItem(m_hResidenceCheck,IDC_EDIT_RESIDENCE);
	SendMessage( m_hResidenceEdit, EM_LIMITTEXT, (WPARAM)7, 0 );
	m_OldEditProc = (WNDPROC)SetWindowLong( m_hResidenceEdit, GWL_WNDPROC, (LONG)StaticEditSubProc);

	Hide();
}

BOOL CALLBACK CResidenceCheck::StatiCResidenceCheckProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	cltClient *pclclient = (cltClient*)pclClient;
	return pclclient->m_pResidenceCheck->ResidenceCheckProc( hDlg, iMsg, wParam, lParam );
}

BOOL CALLBACK CResidenceCheck::ResidenceCheckProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam )
{
	switch( iMsg )
	{
	case WM_INITDIALOG:
		{
			Init();

			m_hEditBrush = CreateSolidBrush( COLOR_DIALOG_LIGHTBLUE );
			m_hDlgBrush  = CreateSolidBrush( COLOR_DIALOG_LIGHTBLUE );
		}
		break;
	case WM_CTLCOLORSTATIC:
		{
/*			if ( GetDlgItem( hDlg, IDC_STATIC_RESIDENCE_CHARNAME ) == (HWND)lParam )
			{
				HDC hdc = (HDC)wParam;
				SetTextColor( hdc, COLOR_DIALOG_BLACK );
				SetBkColor( hdc, COLOR_DIALOG_LIGHTBLUE );
								
				return (BOOL)m_hEditBrush;
			}
			if ( GetDlgItem( hDlg,IDC_STATIC_RESIDENCE) == (HWND)lParam )
			{
				HDC hdc = (HDC)wParam;
				SetTextColor( hdc, COLOR_DIALOG_BLACK );
				SetBkColor( hdc, COLOR_DIALOG_LIGHTBLUE );
								
				return (BOOL)m_hEditBrush;
			}
			if ( GetDlgItem( hDlg,IDC_STATIC_RESIDENCE2) == (HWND)lParam )
			{
				HDC hdc = (HDC)wParam;
				SetTextColor( hdc, COLOR_DIALOG_BLACK );
				SetBkColor( hdc, COLOR_DIALOG_LIGHTBLUE );
								
				return (BOOL)m_hEditBrush;
			}
			if ( GetDlgItem( hDlg,IDC_STATIC_RESIDENCE3) == (HWND)lParam )
			{
				HDC hdc = (HDC)wParam;
				SetTextColor( hdc, COLOR_DIALOG_BLACK );
				SetBkColor( hdc, COLOR_DIALOG_LIGHTBLUE );
								
				return (BOOL)m_hEditBrush;
			}*/
			HDC hdc = (HDC)wParam;
			SetTextColor( hdc, COLOR_DIALOG_BLACK );
			SetBkColor( hdc, COLOR_DIALOG_LIGHTBLUE );
							
			return (BOOL)m_hEditBrush;
		}
		break;
	case WM_CTLCOLORDLG:
		{
			HDC hdc = (HDC)wParam;
			SetTextColor( hdc, COLOR_DIALOG_BLACK );
			SetBkColor( hdc, COLOR_DIALOG_LIGHTBLUE );
								
			return (BOOL)m_hDlgBrush;
		}
		break;
	case WM_COMMAND:
		{
			switch( wParam )
			{
			case ID_RESIDENCE_OK:
				{
					TCHAR buffer[8];
					GetWindowText(m_hResidenceEdit,buffer,12);
					StringCchCopy( m_szResidenceNum, 12, buffer );
					m_bCheck = TRUE ;
					SetWindowText( m_hResidenceEdit, TEXT("") );
					Hide();

					if ( GetCheck() )
					{
						// JAYCEE DEL CHAR 
						cltGameMsgRequest_DelUserChar clinfo(m_siCurrentCharInfoList, GetResidenceNum());
						cltMsg clMsg(GAMEMSG_REQUEST_DELUSERCHAR, sizeof(clinfo), (BYTE*)&clinfo);
						pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
					}
				}
				break;
			case ID_RESIDENCE_CANCEL:
				{
					SetWindowText( m_hResidenceEdit, TEXT("") );
					Init();
					Hide();
				}
				break;
			}
		}
		break;

	case WM_CLOSE:
		{
			Init();
			Hide();
		}
		break;

	case WM_DESTROY:
		{
			if ( m_hEditBrush )
			{
				DeleteObject( m_hEditBrush );
				m_hEditBrush = NULL;
			}
			if ( m_hDlgBrush )
			{
				DeleteObject( m_hDlgBrush );
				m_hDlgBrush = NULL;
			}

			Init();

			m_hResidenceEdit = NULL ;
			m_hResidenceCheck = NULL;
		}
		break;
	}

	return FALSE;
}

LRESULT CALLBACK CResidenceCheck::StaticEditSubProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	cltClient *pclclient = (cltClient*)pclClient;
	return pclclient->m_pResidenceCheck->EditSubProc( hWnd, msg, wParam, lParam );
}

LRESULT CALLBACK CResidenceCheck::EditSubProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg) 
	{
	case WM_KEYUP:
		if( wParam == VK_RETURN )
		{
			if ( hWnd )
			{
				TCHAR buffer[8];
				GetWindowText( m_hResidenceEdit,buffer,12 );
				StringCchCopy( m_szResidenceNum, 12, buffer );
				m_bCheck = TRUE ;
				SetWindowText( m_hResidenceEdit, TEXT("") );
				Hide();
			}
		}
		else if (wParam == VK_ESCAPE )
		{
			SetWindowText( m_hResidenceEdit, TEXT("") );
			Init();
			Hide();
		}
		break;
	default :
		break;
    }

    return CallWindowProc( m_OldEditProc, hWnd, msg, wParam, lParam );
}

void CResidenceCheck::set(HWND hDlg,TCHAR * szName,TCHAR * szLevel,SI32 currentchar)
{
	Init();
	Show();
	SetFocus(m_hResidenceEdit);

	RECT rtLogin;
	GetWindowRect(hDlg,&rtLogin);

	MoveWindow( m_hResidenceCheck,rtLogin.left,rtLogin.top,
				rtLogin.right-rtLogin.left,
				rtLogin.bottom-rtLogin.top,TRUE);

	TCHAR buffer[256] = TEXT("") ;

	TCHAR* pText = GetTxtFromMgr(1889);
	//sprintf(buffer,pText,szName,szLevel);

	Nsprintf(buffer,pText,TEXT("char"),  szName  ,TEXT("level"), szLevel   ,NULL);
	SetWindowText(GetDlgItem(m_hResidenceCheck,IDC_STATIC_RESIDENCE_CHARNAME),buffer);

	m_siCurrentCharInfoList = currentchar ;
}

void CResidenceCheck::Show()
{
	ShowWindow( m_hResidenceCheck, SW_SHOW );
}

void CResidenceCheck::Hide()
{
	if ( m_hResidenceCheck )
		ShowWindow( m_hResidenceCheck, SW_HIDE );
}

BOOL CResidenceCheck::GetCheck()
{
	if ( m_bCheck )
	{
		SetCheck();
		return TRUE ;
	}
	else return FALSE ;
}

void CResidenceCheck::Init()
{
	m_bCheck = FALSE ;
	ZeroMemory( m_szResidenceNum, 12 );
	//m_siResidenceNum = 0 ;
	m_siCurrentCharInfoList = 0 ;
}