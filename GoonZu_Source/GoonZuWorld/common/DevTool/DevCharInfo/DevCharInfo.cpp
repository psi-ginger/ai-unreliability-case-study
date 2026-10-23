//---------------------------------
// 2003/7/17 김태곤
//---------------------------------

#include <CommonLogic.h>

//-------------------------------
// Common
//-------------------------------
#include "Char\CharCommon\Char-Common.h"
#include "Char\CharManager\CharManager.h"
#include "Map\FieldObject\FieldObject.h"

#include "..\resource.h"

#include "DevCharInfo.h"

extern cltCommonLogic* pclClient;

cltDevCharInfo::cltDevCharInfo(cltCharManager* pclcm) : cltDevTool()
{
	CharUnique = 0;

	pclCM	= pclcm;

}

cltDevCharInfo::~cltDevCharInfo()
{

}

BOOL CALLBACK cltDevCharInfo::DlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	TCHAR buffer[256] = "";

	switch(iMsg)
	{
		case WM_DESTROY:
			{
				ACTION_WM_DESTROY();
			}
			break;		
		case WM_COMMAND:
			{
				switch( LOWORD( wParam ) )
				{
				case IDC_BUTTON_DEV_CHARUNIQUE:
					{
						GetDlgItemText(hDlg, IDC_EDIT_CHARUNIQUE, buffer, 100);
						SI32 charunique = _tstoi(buffer);
						if(charunique > 0 && charunique < MAX_CHAR_NUMBER)
						{
							CharUnique = charunique;
						}
					}
					break;
				case IDOK:
					{
						GetDlgItemText(hDlg, IDC_EDIT_CHARUNIQUE, buffer, 100);
						SI32 charunique = _tstoi(buffer);
						if(charunique > 0 && charunique < MAX_CHAR_NUMBER)
						{
							CharUnique = charunique;
						} 

						ShowCharInfo(hDlg);
					}
					break;
				case IDCANCEL:
					{
						DestroyWindow(m_hDlg);
					}
					break; 
				}
			}
			break;
	}
	return FALSE;
}

void cltDevCharInfo::ShowCharInfo(HWND hDlg)
{
	TCHAR buffer[1024];
	HWND hwnd = GetDlgItem(hDlg, IDC_EDIT_CHARINFO);

	if ( CharUnique < 1 || CharUnique >= MAX_CHAR_NUMBER )
	{
		return;
	}

	SI32 id = cltCharCommon::CharUniqueInfo[CharUnique];
	if(id == 0)
	{
		StringCchCopy(buffer, 1024, TEXT("NO PROPER CHAR\r\n"));
	}
	else
	{

		// 캐릭터의 정보를 텍스트로 정리해서 받는다.
		pclCM->CR[id]->GetCharInfoText(buffer);

	}
	
	
	SI32 coll = 0;
	
	if(pclCM->CR[id] && pclCM->CR[id]->pclMap)
	{
		coll = pclCM->CR[id]->pclMap->GetCollMap(pclCM->CR[id]->GetX(), pclCM->CR[id]->GetY());
	}
	
	TCHAR temp[1024];
	TCHAR* pText = GetTxtFromMgr(3095);
	StringCchPrintf(temp, 1024, pText, coll);
	StringCchCat(buffer, 1024, temp);

	pText = GetTxtFromMgr(3096);
	StringCchPrintf(temp, 1024, pText, pclClient->pclTileManager->siLoadedTileSetImageSize);
	StringCchCat(buffer, 1024, temp);
	pText = GetTxtFromMgr(3097);
	StringCchPrintf(temp, 1024, pText, pclClient->pclFieldObjectManager->siLoadedSize);
	StringCchCat(buffer, 1024, temp);
		
	SetWindowText(hwnd, buffer);
}