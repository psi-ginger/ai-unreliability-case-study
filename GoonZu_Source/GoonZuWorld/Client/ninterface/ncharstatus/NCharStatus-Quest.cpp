//---------------------------------
// 2004/10/6 김태곤
//---------------------------------
/* ==========================================================================
	클래스 :

	작성일 :		05/03/14
	작성자 :		정용래
	
	변경사항 :		없음

	차후작업 :		

   ========================================================================*/
#if defined(__LEON_NCHARSTATUS__) && defined(_DEBUG)

#include "..\Client\Client.h"
#include "CharStatus.h"

//-----------------------------
// Common
//-----------------------------
#include "PersonInfo.h"
#include "Char\CharManager\CharManager.h"


//----------------------------
// CommonLogic
//---------------------------
#include "Msg\MsgType-Quest.h"

extern cltCommonLogic* pclClient;

BOOL CALLBACK cltNCharStatus::CharStatusQuestStaticProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	cltClient* pclclient = (cltClient*)pclClient;

	return pclclient->pclCharStatus->CharStatusQuestProc(hDlg, iMsg, wParam, lParam);
}


BOOL CALLBACK cltNCharStatus::CharStatusQuestProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	SI32 id = 1;
	char buffer[256]="";
	
	switch(iMsg)
	{
	case WM_INITDIALOG:
		// 타이머를 설정한다. 
		SetTimer(hDlg, 1, 2000, NULL);

		pclClient->pclQuestManager->ShowCurrentQuestInfo(1, hDlg, IDC_EDIT_STATUS_QUEST, IDC_EDIT_STATUS_SPECIALQUEST);
		pclClient->pclQuestManager->ShowQuestClearInfo(1, hDlg, IDC_LIST_QUEST_CLEAR, IDC_STATIC_QUESTSUM);

		return TRUE;

	case WM_CTLCOLORDLG:
		{
			HDC hdc = (HDC)wParam;
			SetTextColor( hdc, COLOR_DIALOG_BLACK);
			SetBkColor( hdc, COLOR_DIALOG_STATIC_BLUE );
								
			return (BOOL)m_hDlgBrush;
		}
		break;
		
	case WM_CTLCOLORSTATIC:
		{
			if ( GetDlgItem( hDlg, IDC_EDIT_STATUS_QUEST ) == (HWND)lParam)
			{
				HDC hdc = (HDC)wParam;
				SetBkColor( hdc, COLOR_DIALOG_STATIC_BLUE);				
				SetTextColor( hdc, COLOR_DIALOG_BLACK );

				return (BOOL)m_hStaticBrush;
			}
			else
			{
				HDC hdc = (HDC)wParam;
				SetBkMode(hdc,TRANSPARENT);
				SetBkColor( hdc, COLOR_DIALOG_STATIC_BLUE);
				SetTextColor( hdc, COLOR_DIALOG_BLACK );
				return (BOOL)m_hDlgBrush;
			}
			
		}
		break;
	case WM_TIMER:
		{
			pclClient->pclQuestManager->ShowCurrentQuestInfo(1, hDlg, IDC_EDIT_STATUS_QUEST, IDC_EDIT_STATUS_SPECIALQUEST);

			// 흥부박 퀘스트의 단어를 표시한다. 
			HWND hwnd;
			for(SI32 i = 0;i < pclClient->pclQuestManager->siWordAssemblyNumber;i++)
			{
				if(i == 0)
				{
					hwnd = GetDlgItem( hDlg, IDC_STATIC_SPQUEST_WORD1);
				}
				else if(i == 1)
				{
					hwnd = GetDlgItem( hDlg, IDC_STATIC_SPQUEST_WORD2);
				}
				else if(i == 2)
				{
					hwnd = GetDlgItem( hDlg, IDC_STATIC_SPQUEST_WORD3);
				}
				else if(i == 3)
				{
					hwnd = GetDlgItem( hDlg, IDC_STATIC_SPQUEST_WORD4);
				}
				else if(i == 4)
				{
					hwnd = GetDlgItem( hDlg, IDC_STATIC_SPQUEST_WORD5);
				}
				else if(i == 5)
				{
					hwnd = GetDlgItem( hDlg, IDC_STATIC_SPQUEST_WORD6);
				}
				else if(i == 6)
				{
					hwnd = GetDlgItem( hDlg, IDC_STATIC_SPQUEST_WORD7);
				}
				else if(i == 7)
				{
					hwnd = GetDlgItem( hDlg, IDC_STATIC_SPQUEST_WORD8);
				}
				else continue;

				if(pclClient->pclCM->CR[id]->pclCI->clQuestInfo.bWordAssemblySwitch[i] == true)
				{
					SetWindowText(hwnd,  pclClient->pclQuestManager->szWordAssemblyString[i]);
				}
				else
				{
					SetWindowText(hwnd,  "?");
				}
			}

			hwnd = GetDlgItem(hDlg,IDC_STATIC_SPECIALQUEST_WORDCLEAR);
			char buffer[256] = "";
			if ( hwnd )
			{
				char* pText = GetTxtFromMgr(2833);
				sprintf(buffer, pText
					,pclClient->pclCM->CR[1]->pclCI->clQuestInfo.siClearWordSpecialQuest);
				SetWindowText(hwnd,buffer);
			}
		}
		return TRUE;

	case WM_CHAR:
		return TRUE;

	case WM_DESTROY:
		// 타이머를 해제한다. 
		KillTimer(hDlg, 1);
		return TRUE;

	case WM_NOTIFY:
		return TRUE;

	case WM_COMMAND:
		switch(wParam)
		{
		case IDC_BUTTON_CANCELSPECIALQUEST:
			if(pclClient->pclCM->CR[id]->pclCI->clQuestInfo.clUnit[QUEST_TYPE_SPECIAL].siCurQuestUnique)
			{
				// 클라이언트에서 정보를 삭제한다. 

				cltGameMsgRequest_CancelQuest clInfo(QUEST_TYPE_SPECIAL,  1);
				cltMsg clMsg(GAMEMSG_REQUEST_CANCELQUEST, sizeof(clInfo), (char*)&clInfo);
				pclClient->pclCM->CR[id]->SendNetMsg((char*)&clMsg);
			}
			return TRUE;
	
		case IDOK:
			return TRUE;

		case IDCANCEL:
			return TRUE;
			
		}
		return TRUE;
	}
	return FALSE;

}

#endif