//---------------------------------
// 2003/7/17 김태곤
//---------------------------------

#include <CommonLogic.h>
#include "Char\CharCommon\Char-Common.h"
#include "..\resource.h"

#include "DevCharInfo.h"

extern cltCommonLogic* pclClient;

cltDevCharInfo::cltDevCharInfo(cltCharManager* pclcm)
{

	m_hDlg	= NULL;
	
		TabDlg	= NULL;

	DlgRectInitSwitch = FALSE;

	CharUnique = 0;

	pclCM	= pclcm;

}

cltDevCharInfo::~cltDevCharInfo()
{

}

BOOL CALLBACK cltDevCharInfo::DevCharInfoDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	char buffer[128];

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
		ShowCharInfo(hDlg);
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
		case IDC_BUTTON_DEV_CHARUNIQUE:
			GetDlgItemText(hDlg, IDC_EDIT_CHARUNIQUE, buffer, 100);
			CharUnique = atoi(buffer);

			break;

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

void cltDevCharInfo::ShowCharInfo(HWND hDlg)
{
	char buffer[1024];
	HWND hwnd = GetDlgItem(hDlg, IDC_EDIT_CHARINFO);

	SI32 id = cltCharCommon::CharUniqueInfo[CharUnique];
	if(id == 0)
	{
		strcpy(buffer, "NO PROPER CHAR\r\n");
	}
	else
	{

		// 캐릭터의 정보를 텍스트로 정리해서 받는다.
		pclCM->CR[id]->GetCharInfoText(buffer);

	}
	
	char temp[1024];
	sprintf(temp, "\r\n로딩타일바이트:[%d]\r\n", pclClient->pclTileSet->siLoadedTileSetImageSize);
	strcat(buffer, temp);
	sprintf(temp, "로딩FieldObj:[%d]\r\n", pclClient->pclFieldObjectSet->siLoadedSize);
	strcat(buffer, temp);
	

	SetWindowText(hwnd, buffer);
}