//---------------------------------
// 2004/9/15 성준엽
//---------------------------------

#include "..\..\Common\JWLib\IsCorrectID\IsCorrectID.h"
#include "Char\CharManager\CharManager.h"

#include "..\Client\Client.h"
#include "CharStatus.h"
#include "..\resource.h"
#include "..\Server\Rank\Rank.h"
#include "..\..\Client\Interface\ErrorMsgDlg\ErrorMsgDlg.h"
#include "..\..\Client\Interface\NotifyMsgDlg\NotifyMsgDlg.h"

#include "MsgType-Person.h"
#include "Msg/MsgType-ItemMall.h"

//-----------------------------
// Common
//-----------------------------
#include "PersonInfo.h"

extern cltCommonLogic* pclClient;

cltCharNameChange::cltCharNameChange()
{
	m_hDlg	= NULL;
	m_DlgRectInitSwitch = FALSE;

	m_hStaticBrush = NULL ;
	m_hDlgBrush  = NULL ;
}

cltCharNameChange::~cltCharNameChange()
{
	m_hDlg	= NULL;
	m_DlgRectInitSwitch = FALSE;
}

BOOL cltCharNameChange::CreateDlg(HINSTANCE hinst, HWND hwnd)
{
	// 대화상자가 없는 상태라면 만든다. 
	if(!IsWindow(m_hDlg))
	{
		m_hDlg = CreateDialog(hinst, MAKEINTRESOURCE(IDD_DIALOG_STATUS_CHANGENAME), hwnd, CharNameChangeStaticProc);

		if(m_DlgRectInitSwitch == TRUE)
		{
			MoveWindow(m_hDlg, 
				m_DlgRect.left,	 m_DlgRect.top, 
				m_DlgRect.right - m_DlgRect.left, 
				m_DlgRect.bottom - m_DlgRect.top, FALSE);
		}

		ShowWindow(m_hDlg, SW_SHOW);

		return TRUE;
	}

	return FALSE ;
}

void cltCharNameChange::Close()
{
	if ( m_hDlg && IsWindow(m_hDlg) )
		DestroyWindow(m_hDlg);
}

BOOL CALLBACK cltCharNameChange::CharNameChangeStaticProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	cltClient* pclclient = (cltClient*)pclClient;
	return pclclient->pclCharStatus->pclCharNameChange->CharNameChangeProc(hDlg, iMsg, wParam, lParam);
}

BOOL CALLBACK cltCharNameChange::CharNameChangeProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	cltClient* pclclient = (cltClient*)pclClient;

	switch(iMsg)
	{
	case WM_INITDIALOG:
		{
			char buffer[1024] = "" ;
			char* pText = GetTxtFromMgr(563);
			sprintf(buffer, pText,pclclient->pclCM->CR[1]->GetName());

			HWND hwnd = GetDlgItem(hDlg,IDC_STATIC_CHANGENAME_EXPLAIN);
			SetWindowText(hwnd,buffer);

			m_hStaticBrush = CreateSolidBrush( COLOR_DIALOG_YELLOW );
			m_hDlgBrush  = CreateSolidBrush( COLOR_DIALOG_GRAY);
		}
		return TRUE;
	case WM_CTLCOLORSTATIC:
		{
			if ( GetDlgItem( hDlg, IDC_STATIC_CHANGENAME_EXPLAIN ) == (HWND)lParam )
			{
				HDC hdc = (HDC)wParam;
				SetTextColor( hdc, COLOR_DIALOG_BLACK );
				SetBkColor( hdc, COLOR_DIALOG_YELLOW );
								
				return (BOOL)m_hStaticBrush;
			}
		}
		break;
	case WM_DESTROY:
		{
			// 윈도우의 위치를 기억한다. 
			GetWindowRect(m_hDlg, &m_DlgRect);
			m_DlgRectInitSwitch = TRUE;

			if ( m_hStaticBrush )
			{
				DeleteObject( m_hStaticBrush );
				m_hStaticBrush = NULL;
			}
			if ( m_hDlgBrush )
			{
				DeleteObject( m_hDlgBrush );
				m_hDlgBrush = NULL;
			}

			m_hDlg = NULL;
		}
		return TRUE;
	case WM_CLOSE:
		{
			DestroyWindow(m_hDlg);
		}
		return TRUE;
	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case IDOK:
			{
				cltClient *pclclient = (cltClient *)pclClient;
				char buffer[256] = "" ;
				char szName[MAX_PLAYER_NAME] = "" ;

				SI32 itemunique = 14110 ;  // 이름변경이용권
				if ( pclclient->pclCM->CR[1]->pclCI->clCharItem.GetItemNum(itemunique) <= 0 )
				{
					char* pText = GetTxtFromMgr(564);
					sprintf(buffer, pText);
					
					pText = GetTxtFromMgr(565);
					pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pText,buffer);
					return TRUE;
				}

				GetWindowText(GetDlgItem(m_hDlg,IDC_EDIT_CHANGENAME),szName,MAX_PLAYER_NAME-1);

				if ( strlen(szName) <= 0 || strlen(szName) > MAX_PLAYER_NAME ||
					!IsCorrectID(szName) )
				{
					char* pText = GetTxtFromMgr(566);
					sprintf(buffer, pText,(MAX_PLAYER_NAME-1)/2);
					
					pText = GetTxtFromMgr(567);
					pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pText,buffer);
					return TRUE;
				}

				char* pText = GetTxtFromMgr(568);
				strcpy(buffer, pText);

				pText = GetTxtFromMgr(565);
				if (  MessageBox(NULL, buffer,pText,
					MB_ICONQUESTION | MB_YESNO) == IDNO )
				{
					return TRUE;
				}

				cltGameMsgRequest_ChangeCharName clinfo((char*)szName);
				cltMsg clMsg(GAMEMSG_REQUEST_CHANGE_CHARNAME, sizeof(clinfo), (char*)&clinfo);
				pclClient->SendMsgToServer((char*)&clMsg);

				DestroyWindow(m_hDlg);
			}
			return TRUE;
		case IDCANCEL:
			{
				DestroyWindow(m_hDlg);
			}
			return TRUE;
		}
		return TRUE;
	}

	return FALSE;
}