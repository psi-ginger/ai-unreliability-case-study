//---------------------------------
// 2003/11/11 김태곤
//---------------------------------
#include "..\Client.h"
#include "QuitConfirm.h"

#include "..\..\Resource.h"

extern cltCommonLogic* pclClient;
extern DWORD dwQuitTimer;

cltQuitConfirm::cltQuitConfirm()
{
	m_hDlg = NULL;
	DlgRectInitSwitch	= FALSE;
}

cltQuitConfirm::~cltQuitConfirm()
{

}

BOOL cltQuitConfirm::CreateDlg(HINSTANCE hinst, HWND hwnd)
{
	// 대화상자가 없는 상태라면 만든다. 
	if(!IsWindow(m_hDlg))
	{

		m_hDlg = CreateDialog(hinst, MAKEINTRESOURCE(IDD_DIALOG_QUITCONFIRM), hwnd, StaticQuitConfirmDlgProc);

		if(DlgRectInitSwitch == TRUE)
		{
			MoveWindow(m_hDlg, 
				DlgRect.left, DlgRect.top, 
				DlgRect.right - DlgRect.left, 
				DlgRect.bottom - DlgRect.top, FALSE);
		}

		ShowWindow(m_hDlg, SW_SHOW);

		return TRUE;
	}
	else
	{
		DestroyWindow(m_hDlg);
	}


	return 0;
}

BOOL CALLBACK cltQuitConfirm::StaticQuitConfirmDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	cltClient* pclclient = (cltClient*)pclClient;

	return pclclient->pclQuitConfirm->QuitConfirmDlgProc(hDlg, iMsg, wParam, lParam);
}


BOOL CALLBACK cltQuitConfirm::QuitConfirmDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	cltClient* pclclient = (cltClient*)pclClient;

	switch(iMsg)
	{
	case WM_INITDIALOG:
		return TRUE;

	case WM_NOTIFY:
		return TRUE;

	case WM_TIMER:
		return TRUE;

	case WM_CLOSE:
		DestroyWindow(m_hDlg);
		return TRUE;

	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case IDOK:
			DestroyWindow(m_hDlg);
			
			if(pclClient->GetFrame() <= 0)
			{
				dwQuitTimer = 1;
			}
			else
			{
				dwQuitTimer = pclClient->CurrentClock;
			}
			return TRUE;
			
		case IDCANCEL:
			DestroyWindow(m_hDlg);
			return TRUE;
		}
		return TRUE;
		
	case WM_DESTROY:

		// 윈도우의 위치를 기억한다. 
		GetWindowRect(hDlg, &DlgRect);
		DlgRectInitSwitch = TRUE;

		m_hDlg = NULL;
		return TRUE;

	}

	return FALSE;
}
