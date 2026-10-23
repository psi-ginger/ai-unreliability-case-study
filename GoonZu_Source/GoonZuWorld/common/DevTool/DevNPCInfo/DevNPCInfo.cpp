//---------------------------------
// 2003/11/25 김태곤
//---------------------------------

#include <CommonLogic.h>
#include "Char\CharCommon\Char-Common.h"
#include "..\resource.h"

#include "DevNPCInfo.h"

extern cltCommonLogic* pclClient;

cltDevNPCInfo::cltDevNPCInfo(cltCharManager* pclcm) : cltDevTool()
{

}

cltDevNPCInfo::~cltDevNPCInfo()
{

}

BOOL CALLBACK cltDevNPCInfo::DlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	TCHAR buffer[256] = TEXT("");

	switch(iMsg)
	{
	case WM_INITDIALOG:
		{
			// 타이머를 설정한다. 
			SetTimer(hDlg, 1, 1000, NULL);
			
		}
		return TRUE;

	case WM_CHAR:
		return TRUE;

	case WM_TIMER:
		ShowInfo(hDlg);
		return TRUE;

	case WM_DESTROY:
		// 타이머를 해제한다. 
		KillTimer(hDlg, 1);

		// 윈도우의 위치를 기억한다. 
		GetWindowRect(m_hDlg, &DlgRect);

		DlgRectInitSwitch = TRUE;

		m_hDlg = NULL;
		return TRUE;

	case WM_COMMAND:
		switch(wParam)
		{
		case IDOK:
			DestroyWindow(m_hDlg);
			return TRUE;

		case IDCANCEL:
			DestroyWindow(m_hDlg);
			return TRUE;
			
		}
		return TRUE;
	}
	return FALSE;
}

void cltDevNPCInfo::ShowInfo(HWND hDlg)
{
/*	TCHAR buffer[1024];
	HWND hwnd = GetDlgItem(hDlg, IDC_EDIT_CHARINFO);

	SI32 id = cltCharCommon::CharUniqueInfo[CharUnique];
	if(id == 0)
	{
		_tcscpy(buffer, TEXT("NO PROPER CHAR\r\n"));
	}
	else
	{

		// 캐릭터의 정보를 텍스트로 정리해서 받는다.
		pclCM->CR[id]->GetCharInfoText(buffer);

	}
	
	TCHAR temp[1024];
	sprintf(temp, TEXT("\r\n로딩타일바이트:[%d]\r\n"), pclClient->pclTileSet->siLoadedTileSetImageSize);
	strcat(buffer, temp);
	sprintf(temp, TEXT("로딩FieldObj:[%d]\r\n"), pclClient->pclFieldObjectSet->siLoadedSize);
	strcat(buffer, temp);
	

	SetWindowText(hwnd, buffer);
	*/
}