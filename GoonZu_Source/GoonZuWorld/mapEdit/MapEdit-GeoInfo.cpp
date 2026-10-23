//---------------------------------
// 2003/8/25 김태곤
//---------------------------------

#include "..\Common\CommonHeader.h"
#include "..\Client\Client.h"
#include <Mouse.h>

#include "mapedit.h"
#include "..\Client\SmallMap\SmallMap.h"

#include "..\resource.h"

extern cltCommonLogic* pclClient;

//----------------------------------------------
// 지형 안내 툴 
//----------------------------------------------
void cltMapEdit::CreateGeoInfoTool()
{
	if(!IsWindow(hGeoInfoDlg))
	{
		ShowCursor(TRUE);
		hGeoInfoDlg = CreateDialog(GetInstance(), MAKEINTRESOURCE(IDD_DIALOG_MAPEDIT_GEOINFO), GetHwnd(), (DLGPROC)StaticGeoInfoDlgProc);
		
		ShowWindow(hGeoInfoDlg, SW_SHOW);
		ShowCursor(FALSE);
		
	}
}


BOOL CALLBACK cltMapEdit::StaticGeoInfoDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	return ((cltMapEdit*)pclClient)->GeoInfoDlgProc(hDlg, iMsg, wParam, lParam);
}

BOOL CALLBACK cltMapEdit::GeoInfoDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{

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
			// 현재 선택된 지형으로 이동한다. 
		case IDC_BUTTON_MAPEDIT_MOVETOGEOPOS:
			{
				cltMapClient* pclmap = (cltMapClient*)pclMapManager->pclCurrentMap;
				if(SelectedGeoInfoSlot > 0 )
				{
					SI32 i;
					SI32 startpos	= pclmap->VirtualMapX + (pclmap->VirtualMapY * pclmap->MH.MapXsize);
					SI32 endpos		= pclmap->MH.MapXsize * pclmap->MH.MapYsize;
					
					for(i = startpos;i < endpos;i++)
					{
						if(pclmap->pclMapGeoInfo->pclGeoMap[i] == SelectedGeoInfoSlot)
						{
							pclmap->SetVirtualPos(i%pclmap->MH.MapXsize, i/pclmap->MH.MapXsize);
							break;
						}

					}

					for(i = 0;i < startpos;i++)
					{
						if(pclmap->pclMapGeoInfo->pclGeoMap[i] == SelectedGeoInfoSlot)
						{
							pclmap->SetVirtualPos(i%pclmap->MH.MapXsize, i/pclmap->MH.MapXsize);
							break;
						}

					}

				}

			}
			return TRUE;
		case IDC_LIST_MAPEDIT_GEOINFO:
			switch(HIWORD(wParam))
			{
			case LBN_SELCHANGE:
				{
					SI32 index		= SendDlgItemMessage(hDlg, IDC_LIST_MAPEDIT_GEOINFO, LB_GETCURSEL,	0, 0);
					SI32 slot		= SendDlgItemMessage(hDlg, IDC_LIST_MAPEDIT_GEOINFO, LB_GETITEMDATA,	index, 0);
					if(slot > 0 || slot <= MAX_GEO_NUMBER)
					{
						// 현재의 모드를 지형 안내 설정 모드로 변경한다. 
						CurrentMode = MAPEDIT_MODE_GEOINFO;
						SelectedGeoInfoSlot	= slot;

						// 에디트 박스에 선택된 지형 정보 데이터를 넣는다. 
						SetDlgItemText(hDlg, IDC_EDIT_MAPEDIT_GEONAME, pclMapManager->pclCurrentMap->pclMapGeoInfo->szGetText[SelectedGeoInfoSlot]);
					}
				}

				break;
			}
			return TRUE;
			// 지형의 이름을 설정한다 
		case IDC_BUTTON_MAPEDIT_SETGEONAME:
			{
				if(SelectedGeoInfoSlot > 0)
				{
					TCHAR geoname[GEO_TEXT_LENGTH];
					
					GetDlgItemText(hDlg, IDC_EDIT_MAPEDIT_GEONAME, geoname, GEO_TEXT_LENGTH-1);
					MStrCpy( pclMapManager->pclCurrentMap->pclMapGeoInfo->szGetText[SelectedGeoInfoSlot], geoname, GEO_TEXT_LENGTH );


					// 리스트 박스를 재설정한다. 
					SetGeoInfoListBox(hDlg);
				}

			}
			return TRUE;
		case IDOK:
		    DestroyWindow(hGeoInfoDlg);
		   return TRUE;

		case IDCANCEL:
		    DestroyWindow(hGeoInfoDlg);
		   return TRUE;
		}
		break;
	}

	return FALSE;
}


// 지형 정보 리스트 박스에 값을 설정한다. 
void cltMapEdit::SetGeoInfoListBox(HWND hDlg)
{

	SI32 i;
	SI32 index = 0;
	TCHAR buffer[256];

	// 리스트 박스를 깨끗하게 지운다. 
	SendDlgItemMessage(hDlg, IDC_LIST_MAPEDIT_GEOINFO, LB_RESETCONTENT, 0, 0);

	for(i = 1;i < MAX_GEO_NUMBER;i++)
	{

		StringCchPrintf(buffer, 256, TEXT("%03d.%s"), i, pclMapManager->pclCurrentMap->pclMapGeoInfo->szGetText[i]);

		SendDlgItemMessage(hDlg, IDC_LIST_MAPEDIT_GEOINFO, LB_ADDSTRING,		0, (LPARAM)buffer);
		SendDlgItemMessage(hDlg, IDC_LIST_MAPEDIT_GEOINFO, LB_SETITEMDATA,	index, (LPARAM)i);
		index++;
	}

}