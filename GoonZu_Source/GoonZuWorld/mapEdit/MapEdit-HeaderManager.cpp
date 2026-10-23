//---------------------------------
// 2003/8/4 김태곤
//---------------------------------

#include "..\Common\CommonHeader.h"
#include "..\Client\Client.h"


#include <Mouse.h>

#include "mapedit.h"

#include "..\resource.h"

extern cltCommonLogic* pclClient;

// 맵 헤더 수정용 대화 상화를 보여준다.
BOOL cltMapEdit::OpenMapEditHeaderManager()
{
	return DialogBox(GetInstance(), MAKEINTRESOURCE(IDD_DIALOG_MAPEDIT_MAPHEADER), GetHwnd(), 
		(DLGPROC)StaticHeaderManagerDlgProc);
}

BOOL CALLBACK cltMapEdit::StaticHeaderManagerDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	return ((cltMapEdit*)pclClient)->HeaderManagerDlgProc(hDlg, iMsg, wParam, lParam);
}

BOOL CALLBACK cltMapEdit::HeaderManagerDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{

	TCHAR buffer[256];
	
	switch(iMsg)
	{
	case WM_INITDIALOG:
		// 타일 세트의 초기값을 설정한다.
		StringCchPrintf(buffer, 256, TEXT("%d"), pclMapManager->pclCurrentMap->MH.TileSet1);
		SetDlgItemText(hDlg, IDC_EDIT_MAPEDIT_TILESET, buffer);

		StringCchPrintf(buffer, 256, TEXT("%d"), pclMapManager->pclCurrentMap->MH.TileSet2);
		SetDlgItemText(hDlg, IDC_EDIT_MAPEDIT_TILESET2, buffer);


		// 날씨 변수를 설정한다. 
		if(pclMapManager->pclCurrentMap->MH.bWeatherSwitch)
		{
			SendMessage(GetDlgItem(hDlg, IDC_CHECK_MAPEDIT_WEATHER), BM_SETCHECK, BST_CHECKED, 0);
		}
		else
		{
			SendMessage(GetDlgItem(hDlg, IDC_CHECK_MAPEDIT_WEATHER), BM_SETCHECK, BST_UNCHECKED, 0);
		}

		return TRUE;

	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case IDOK:
			{
				// 타일 세트를 얻어온다. 
				// 타일 세트의 초기값을 설정한다.
				GetDlgItemText(hDlg, IDC_EDIT_MAPEDIT_TILESET, buffer, 60);
				SI32 tileset1 = _tstoi(buffer);

				GetDlgItemText(hDlg, IDC_EDIT_MAPEDIT_TILESET2, buffer, 60);
				SI32 tileset2 = _tstoi(buffer);

				pclMapManager->pclCurrentMap->MH.TileSet1	= tileset1;
				pclMapManager->pclCurrentMap->MH.TileSet2	= tileset2;


				// 날씨 변수를 얻어온다. 
				SI32 weatherswitch = SendMessage(GetDlgItem(hDlg, IDC_CHECK_MAPEDIT_WEATHER), BM_GETCHECK, 0, 0);
				if(weatherswitch)
				{
					pclMapManager->pclCurrentMap->MH.bWeatherSwitch = true;
				}
				else
				{
					pclMapManager->pclCurrentMap->MH.bWeatherSwitch = false;
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




