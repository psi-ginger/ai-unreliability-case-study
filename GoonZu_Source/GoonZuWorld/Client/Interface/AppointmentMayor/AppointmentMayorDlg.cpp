#include "AppointmentMayorDlg.h"
#include "../resource.h"

HWND	 cltAppointmentMayorDlg::m_hWnd;

cltAppointmentMayorDlg::cltAppointmentMayorDlg()
{
	m_hWnd = NULL;
}

cltAppointmentMayorDlg::~cltAppointmentMayorDlg()
{

}

void cltAppointmentMayorDlg::Create( HINSTANCE hInst, HWND hParent )
{

	if( !IsWindow( m_hWnd ) )
	{
		m_hWnd = CreateDialog( hInst, MAKEINTRESOURCE( IDD_DIALOG_APPOINTMENTMAYOR ), hParent, DlgProc );
	
		ShowWindow( m_hWnd, SW_SHOW );
	}
}


BOOL cltAppointmentMayorDlg::DlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{

	switch(iMsg)
	{
	case WM_INITDIALOG:
		{
		}			
		return TRUE;
		
	case WM_TIMER:
		{
		}
		return TRUE;

	case WM_CHAR:
		return TRUE;

	case WM_DESTROY:
		return TRUE;

	case WM_NOTIFY:
		return TRUE;

	case WM_COMMAND:
		switch(wParam)
		{
		case IDOK:
			DestroyWindow(m_hWnd);
			return TRUE;

		case IDCANCEL:
			DestroyWindow(m_hWnd);
			return TRUE;
			
		}
		break;
	}

	return FALSE;
}

