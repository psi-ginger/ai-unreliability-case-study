//---------------------------------
// 2004/9/15 성준엽
//---------------------------------
/* ==========================================================================
	클래스 :

	작성일 :		05/03/17
	작성자 :		정용래
	
	변경사항 :		없음

	차후작업 :		

   ========================================================================*/
#if defined(__LEON_NCHARSTATUS__) && defined(_DEBUG)

#include "..\..\Common\JWLib\IsCorrectID\IsCorrectID.h"
#include "Char\CharManager\CharManager.h"

#include "..\Client\Client.h"
#include "CharStatus.h"
#include "..\resource.h"
#include "..\Server\Rank\Rank.h"
#include "..\..\Client\Interface\ErrorMsgDlg\ErrorMsgDlg.h"
#include "..\..\Client\Interface\NotifyMsgDlg\NotifyMsgDlg.h"

#include "../../InterfaceMgr/Interface/Static.h"

#include "MsgType-Person.h"
#include "Msg/MsgType-ItemMall.h"

#include "../../Client/InterfaceMgr/Interface/Button.h"
#include "../../Client/InterfaceMgr/Interface/ImageStatic.h"
#include "../../Client/InterfaceMgr/Interface/Static.h"
#include "../../Client/InterfaceMgr/InterfaceFile.h"

//-----------------------------
// Common
//-----------------------------
#include "PersonInfo.h"

extern cltCommonLogic* pclClient;

cltNCharNameChange::cltNCharNameChange()
{
	m_DlgRectInitSwitch = FALSE;
}

cltNCharNameChange::~cltNCharNameChange()
{
	m_DlgRectInitSwitch = FALSE;
}

void cltNCharNameChange::Create()
{
	// 대화상자가 없는 상태라면 만든다. 
	if( ! IsCreate() )
	{

		//if(m_DlgRectInitSwitch == TRUE)
		//{
		//	MoveWindow(m_hDlg, 
		//		m_DlgRect.left,	 m_DlgRect.top, 
		//		m_DlgRect.right - m_DlgRect.left, 
		//		m_DlgRect.bottom - m_DlgRect.top, FALSE);
		//}
		CDialogController::Create( , CONTROLLER_KIND_POPUPDIALOG, NULL, , "", "NInterface/Interface/Interface_20X20_00_109.Spr", 9, true, 80, 100, , , true, false, true);

		CInterfaceFile file;

		file.LoadFile("NInterface/Data/.ddf");


	}

	ShowWindow(m_hDlg, SW_SHOW);
}

void cltNCharNameChange::Close()
{
	if ( m_hDlg && IsWindow(m_hDlg) )
		DestroyWindow(m_hDlg);
}

BOOL CALLBACK cltNCharNameChange::CharNameChangeStaticProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	cltClient* pclclient = (cltClient*)pclClient;
	return pclclient->pclCharStatus->pclCharNameChange->CharNameChangeProc(hDlg, iMsg, wParam, lParam);
}

BOOL CALLBACK cltNCharNameChange::CharNameChangeProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
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
					pclclient->m_pNotifyMsgDlg->SetText( pText,buffer);
					return TRUE;
				}

				GetWindowText(GetDlgItem(m_hDlg,IDC_EDIT_CHANGENAME),szName,MAX_PLAYER_NAME-1);

				if ( strlen(szName) <= 0 || strlen(szName) > MAX_PLAYER_NAME ||
					!IsCorrectID(szName) )
				{
					char* pText = GetTxtFromMgr(566);
					sprintf(buffer, pText,(MAX_PLAYER_NAME-1)/2);
					
					pText = GetTxtFromMgr(567);
					pclclient->m_pNotifyMsgDlg->SetText( pText,buffer);
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
#endif