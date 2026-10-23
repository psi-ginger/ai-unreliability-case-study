#ifndef _APPOINTMENTMAYORDLG_H
#define _APPOINTMENTMAYORDLG_H

#include "CommonHeader.h"

class cltAppointmentMayorDlg
{
public:
	cltAppointmentMayorDlg();
	~cltAppointmentMayorDlg();

	static BOOL CALLBACK DlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);

	void				 Create( HINSTANCE hInst, HWND hParent );

	static HWND			 m_hWnd;

};

#endif


