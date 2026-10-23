//---------------------------------
// 2004/10/20 성준엽
//---------------------------------

#include "..\Client\Client.h"
#include "CharStatus.h"
#include "..\resource.h"
#include "..\Server\Rank\Rank.h"
#include "..\..\Client\Interface\ErrorMsgDlg\ErrorMsgDlg.h"
#include "../../Client/Interface/NotifyMsgDlg/NotifyMsgDlg.h"
#include "Msg\MsgType-Horse.h"

//-----------------------------
// Common
//-----------------------------
#include "PersonInfo.h"
#include "Char\CharManager\CharManager.h"

extern cltCommonLogic* pclClient;

cltCharKindChange::cltCharKindChange()
{
	m_hDlg	= NULL;
	m_DlgRectInitSwitch = FALSE;

	m_hBitmap = NULL ;
}

cltCharKindChange::~cltCharKindChange()
{
	m_hDlg	= NULL;
	m_DlgRectInitSwitch = FALSE;

	m_hBitmap = NULL ;
}

BOOL cltCharKindChange::CreateDlg(HINSTANCE hinst, HWND hwnd)
{
	// 대화상자가 없는 상태라면 만든다. 
	if(!IsWindow(m_hDlg))
	{
		m_hDlg = CreateDialog(hinst, MAKEINTRESOURCE(IDD_DIALOG_STATUS_CHARKIND_CHANGED), hwnd, CharKindChangeStaticProc);

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

void cltCharKindChange::Close()
{
	if ( m_hDlg && IsWindow(m_hDlg) )
		DestroyWindow(m_hDlg);
}

BOOL CALLBACK cltCharKindChange::CharKindChangeStaticProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	cltClient* pclclient = (cltClient*)pclClient;
	return pclclient->pclCharStatus->pclCharKindChange->CharKindChangeProc(hDlg, iMsg, wParam, lParam);
}

BOOL CALLBACK cltCharKindChange::CharKindChangeProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	cltClient* pclclient = (cltClient*)pclClient;

	switch(iMsg)
	{
	case WM_INITDIALOG:
		{
			pclclient->pclCM->CR[1]->pclCI->clBI.GetKind();

			for ( int i = 0 ; i < KIND_SYSTEM ; i ++ )
			{
				if ( i == pclclient->pclCM->CR[1]->pclCI->clBI.GetKind() ) continue;
				cltKindInfo *pclKindInfo = pclClient->pclKindInfoSet->pclKI[i];
				if ( pclKindInfo == NULL ) continue ;

				m_siSelectedUnique = i ;
				break ;
			}

			SetPortrait(hDlg);
		}
		return TRUE;
	case WM_DESTROY:
		{
			if(m_hBitmap)
			{
				DeleteObject(m_hBitmap);
				m_hBitmap = NULL;
			}

			// 윈도우의 위치를 기억한다. 
			GetWindowRect(m_hDlg, &m_DlgRect);
			m_DlgRectInitSwitch = TRUE;

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
		case IDC_BUTTON_CHARKIND_LEFT:
			{
				for ( SI32 i = m_siSelectedUnique - 1 ; i > 0 ; i -- )
				{
					if ( i == pclclient->pclCM->CR[1]->pclCI->clBI.GetKind() ) continue ;
					cltKindInfo *pclKindInfo = pclClient->pclKindInfoSet->pclKI[i];
					if ( pclKindInfo == NULL ) continue ;
					if ( pclKindInfo->IsAtb(ATB_HERO) == false ) continue ;

					m_siSelectedUnique = i ;
					SetPortrait(hDlg);
					break;
				}
			}
			return TRUE ;
		case IDC_BUTTON_CHARKIND_RIGHT:
			{
				for ( SI32 i = m_siSelectedUnique + 1 ; i < KIND_SYSTEM ; i ++ )
				{
					if ( i == pclclient->pclCM->CR[1]->pclCI->clBI.GetKind() ) continue ;
					cltKindInfo *pclKindInfo = pclClient->pclKindInfoSet->pclKI[i];
					if ( pclKindInfo == NULL ) continue ;
					if ( pclKindInfo->IsAtb(ATB_HERO) == false ) continue ;

					m_siSelectedUnique = i ;
					SetPortrait(hDlg);
					break;
				}
			}
			return TRUE ;
		case IDOK:
			{
				cltClient *pclclient = (cltClient *)pclClient;
				char buffer[256] = "" ;

				SI32 mykind = pclclient->pclCM->CR[1]->pclCI->clBI.GetKind() ;

				if ( mykind <= 0 || mykind >= KIND_SYSTEM )
					return TRUE ;
				if ( m_siSelectedUnique == mykind )
					return TRUE ;

				SI32 itemunique = 14120 ; // 캐릭터변경이용권
				if ( pclclient->pclCM->CR[1]->pclCI->clCharItem.GetItemNum(itemunique) <= 0 )
				{
					char* pText = GetTxtFromMgr(555);
					sprintf(buffer, pText);
					
					pText = GetTxtFromMgr(556);
					pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pText,buffer);
					return TRUE;
				}

				if ( pclclient->pclCM->CR[1]->pclCI->clCharItem.clItem[PERSONITEM_DRESS].siUnique != 0 )
				{
					char* pText = GetTxtFromMgr(557);
					sprintf(buffer, pText);
					
					pText = GetTxtFromMgr(558);
					pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pText,buffer);
					return TRUE;
				}

				if ( pclclient->pclCM->CR[1]->pclCI->clCharItem.clItem[PERSONITEM_HAT].siUnique != 0 )
				{
					char* pText = GetTxtFromMgr(559);
					sprintf(buffer,pText);
					
					pText = GetTxtFromMgr(560);
					pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pText,buffer);
					return TRUE;
				}

				char* pText = GetTxtFromMgr(561);

				strcpy(buffer, pText);
				pText = GetTxtFromMgr(562);
				if (  MessageBox(NULL, buffer, pText,
					MB_ICONQUESTION | MB_YESNO) == IDNO )
				{
					return TRUE;
				}

				char senddata = m_siSelectedUnique ;
				cltMsg clMsg(GAMEMSG_REQUEST_CHANGE_CHARKIND,1,&senddata);
				pclclient->SendMsgToServer((char*)&clMsg);
				
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

void cltCharKindChange::SetPortrait(HWND hDlg)
{
	cltKindInfo *pclKindInfo = pclClient->pclKindInfoSet->pclKI[m_siSelectedUnique];

	if ( pclKindInfo == NULL ) return ;
	if ( pclKindInfo->IsAtb(ATB_HERO) == false ) return ;

	// 초상화용 핸들을 삭제한다.
	if(m_hBitmap)
	{
		DeleteObject(m_hBitmap);
		m_hBitmap = NULL;
	}

	char strFileName[ 128 ] = "";
	char strPathFileName[ 128 ] = "";
	switch(m_siSelectedUnique)
	{
	case 1:	strcpy(strFileName, "KIND_HERO1");	break;
	case 2:	strcpy(strFileName, "KIND_HEROINE1"); break;
	case 3: strcpy(strFileName, "KIND_HERO2");	break;

	default: return ;
	}
	sprintf( strPathFileName, "Interface/Portrait/%s.bmp",	strFileName );
	m_hBitmap = (HBITMAP)LoadImage( pclClient->GetInstance(), strPathFileName, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE );
	if ( m_hBitmap != NULL )
	{
		SendDlgItemMessage( hDlg, IDC_STATIC_PORTRAIT_CHARKIND, STM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)m_hBitmap );
	}

	SetWindowText(GetDlgItem(hDlg,IDC_STATIC_CHARKIND_NAME),pclKindInfo->GetName());
}
