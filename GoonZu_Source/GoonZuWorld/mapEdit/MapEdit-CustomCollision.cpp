//---------------------------------
// 2004/5/17이제형
//---------------------------------

#include "..\Common\CommonHeader.h"
#include "..\Client\Client.h"


#include <Mouse.h>

#include "mapedit.h"

#include "..\resource.h"

extern cltCommonLogic* pclClient;

// 맵 헤더 수정용 대화 상화를 보여준다.
BOOL cltMapEdit::OpenCustomCollision()
{
	return DialogBox(GetInstance(), MAKEINTRESOURCE(IDD_MAPEDIT_CUSTOMCOLLISION), GetHwnd(), 
		(DLGPROC)StaticCustomCollisionDlgProc);
}

BOOL CALLBACK cltMapEdit::StaticCustomCollisionDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	return ((cltMapEdit*)pclClient)->CustomCollisionDlgProc(hDlg, iMsg, wParam, lParam);
}

BOOL CALLBACK cltMapEdit::CustomCollisionDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{

	TCHAR buffer[256] = TEXT("") ;
	
	switch(iMsg)
	{
	case WM_INITDIALOG:
		CheckRadioButton(hDlg, IDC_RADIO_ALLBLOCK, IDC_RADIO_CUSTOM7, IDC_RADIO_ALLBLOCK);
		return TRUE;

	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case IDOK:
			{
				// 선택된 CUSTOM COLLISION TYPE을 설정한다.
				((cltMapEdit *)pclClient)->CurrentMode				= MAPEDIT_MODE_CUSTOMCOLL;
				if(SendDlgItemMessage(hDlg, IDC_RADIO_ALLBLOCK, BM_GETCHECK, 0, 0) == BST_CHECKED)
				{
					((cltMapEdit *)pclClient)->CurrentCustomCollision   = BLOCK_CUSTOM;	
				}
				else if(SendDlgItemMessage(hDlg, IDC_RADIO_CUSTOM1, BM_GETCHECK, 0, 0) == BST_CHECKED)
				{
					((cltMapEdit *)pclClient)->CurrentCustomCollision   = BLOCK_EVENT1;
				}
				else if(SendDlgItemMessage(hDlg, IDC_RADIO_CUSTOM2, BM_GETCHECK, 0, 0) == BST_CHECKED)
				{
					((cltMapEdit *)pclClient)->CurrentCustomCollision   = BLOCK_EVENT2;
				}
				else if(SendDlgItemMessage(hDlg, IDC_RADIO_CUSTOM3, BM_GETCHECK, 0, 0) == BST_CHECKED)
				{
					((cltMapEdit *)pclClient)->CurrentCustomCollision   = BLOCK_EVENT3;
				}
				else if(SendDlgItemMessage(hDlg, IDC_RADIO_CUSTOM4, BM_GETCHECK, 0, 0) == BST_CHECKED)
				{
					((cltMapEdit *)pclClient)->CurrentCustomCollision   = BLOCK_EVENT4;
				}
				else if(SendDlgItemMessage(hDlg, IDC_RADIO_CUSTOM5, BM_GETCHECK, 0, 0) == BST_CHECKED)
				{
					((cltMapEdit *)pclClient)->CurrentCustomCollision   = BLOCK_EVENT5;
				}
				else if(SendDlgItemMessage(hDlg, IDC_RADIO_CUSTOM6, BM_GETCHECK, 0, 0) == BST_CHECKED)
				{
					((cltMapEdit *)pclClient)->CurrentCustomCollision   = BLOCK_EVENT6;
				}
				else if(SendDlgItemMessage(hDlg, IDC_RADIO_CUSTOM7, BM_GETCHECK, 0, 0) == BST_CHECKED)
				{
					((cltMapEdit *)pclClient)->CurrentCustomCollision   = BLOCK_EVENT7;
				}
				else
				{
					((cltMapEdit *)pclClient)->CurrentCustomCollision   = BLOCK_CUSTOM;	
				}
				
				EndDialog(hDlg, 1);
			}
			return TRUE;

		case IDCANCEL:

			// 게임을 종료한다. 
			EndDialog(hDlg, 0);
			return TRUE;
		}
		break;
	}

	return FALSE;
}




