#include "..\Common\CommonHeader.h"
#include "..\Client\Client.h"
#include <Mouse.h>

#include "mapedit.h"
#include "..\Client\SmallMap\SmallMap.h"

#include "..\resource.h"

extern cltCommonLogic* pclClient;

//----------------------------------------------
// 맵 유니크 삭제 툴
//----------------------------------------------
void cltMapEdit::DeleteUniqueTool()
{
	if(!IsWindow(hDeleteUniqueDlg))
	{
		ShowCursor(TRUE);
		hDeleteUniqueDlg = CreateDialog(GetInstance(), MAKEINTRESOURCE(IDD_DIALOG_MAPEDIT_DELETEUNIQUE), GetHwnd(), (DLGPROC)StaticDeleteUniqueDlgProc);

		ShowWindow(hDeleteUniqueDlg, SW_SHOW);
		ShowCursor(FALSE);

	}
}


BOOL CALLBACK cltMapEdit::StaticDeleteUniqueDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	return ((cltMapEdit*)pclClient)->DeleteUniqueDlgProc(hDlg, iMsg, wParam, lParam);
}

BOOL CALLBACK cltMapEdit::DeleteUniqueDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	TCHAR buffer[256];
	SI32 siUnique = 0;

	switch(iMsg)
	{
	case WM_INITDIALOG:
		SetGeoInfoListBox(hDlg);
		return TRUE;

	case WM_DESTROY:
		hGeoInfoDlg = NULL;
		return TRUE;



	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case IDOK:
			GetDlgItemText(hDlg, IDC_EDIT_UNIQUE, buffer, 16);
			siUnique = _tstoi(buffer);
			StringCchPrintf( buffer, 256, TEXT("unique(%d)를 삭제하시겠습니까?"), siUnique);


			if ( MessageBox(NULL, buffer, TEXT("필드오브젝트 삭제"), MB_YESNO) == IDYES )
			{
				
				pclMapManager->pclCurrentMap->DeleteFieldObjectUnique( siUnique );
			}
			
			DestroyWindow(hDeleteUniqueDlg);

			return TRUE;

		case IDCANCEL:
			DestroyWindow(hDeleteUniqueDlg);
			return TRUE;
		}
		break;
	}

	return FALSE;
}