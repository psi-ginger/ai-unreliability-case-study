//---------------------------------
// 2004/9/15 성준엽
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
#include "..\resource.h"
#include "..\Server\Rank\Rank.h"
#include "..\..\Client\Interface\ErrorMsgDlg\ErrorMsgDlg.h"
#include "..\..\Client\Interface\NotifyMsgDlg\NotifyMsgDlg.h"
#include "Msg\MsgType-Horse.h"

//-----------------------------
// Common
//-----------------------------
#include "PersonInfo.h"
#include "Char\CharManager\CharManager.h"

extern cltCommonLogic* pclClient;

char* HorseTypeItemUnique[] =
{
		"흑마",		"5503",		"300",
		"백마",		"5506",		"400",
		"적토마",	"5510",		"500",
		"얼룩마",	"5520",		"500",
		"루돌프",	"5530",		"500",
		"천리마",	"5540",		"500",
};

cltNHorseTypeChange::cltNHorseTypeChange()
{
	m_hDlg	= NULL;
	m_DlgRectInitSwitch = FALSE;
	
	m_hHorseBitmap = NULL;

	m_siSelectedUnique = 0 ;
	m_siOriUnique = 0 ;
	m_siMaxUnique = 0 ;
}

cltNHorseTypeChange::~cltNHorseTypeChange()
{
	m_hDlg	= NULL;
	m_DlgRectInitSwitch = FALSE;

	m_hHorseBitmap = NULL;
}

BOOL cltNHorseTypeChange::CreateDlg(HINSTANCE hinst, HWND hwnd,SI32 horseunique)
{
	// 대화상자가 없는 상태라면 만든다. 
	if(!IsWindow(m_hDlg))
	{
		m_siOriUnique = horseunique ;

		m_hDlg = CreateDialog(hinst, MAKEINTRESOURCE(IDD_DIALOG_STATUS_HORSE_CHANGED), hwnd, HorseTypeChangeStaticProc);

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

void cltNHorseTypeChange::Close()
{
	if ( m_hDlg && IsWindow(m_hDlg) )
		DestroyWindow(m_hDlg);
}

BOOL CALLBACK cltNHorseTypeChange::HorseTypeChangeStaticProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	cltClient* pclclient = (cltClient*)pclClient;
	return pclclient->pclCharStatus->pclHorseTypeChange->HorseTypeChangeProc(hDlg, iMsg, wParam, lParam);
}

BOOL CALLBACK cltNHorseTypeChange::HorseTypeChangeProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	cltClient* pclclient = (cltClient*)pclClient;
	cltHorse* pclhorse	= pclclient->pclCM->CR[1]->pclCI->clHorseInfo.GetCurrentShowHorse();

	switch(iMsg)
	{
	case WM_INITDIALOG:
		{
			m_siMaxUnique = 0 ;

			bool bInit = false ;
			for( SI32 i = 0; i < MAX_HORSE_TYPE_INFO; i++ )
			{
				if ( i == m_siOriUnique ) continue ;
				cltHorseTypeInfo *pclHorseTypeInfo = pclClient->pclHorseManager->pclHorseTypeInfo[i];
				if ( pclHorseTypeInfo == NULL ) continue ;
				char* pText = GetTxtFromMgr(667);
				if ( !strcmp(pText,pclHorseTypeInfo->szSpecies) ) continue ;

				if ( !bInit )
				{
					m_siSelectedUnique = i ;
					bInit = true ;
				}

				m_siMaxUnique = i ;
			}

			SetHorsePortrait(hDlg);
		}
		return TRUE;
	case WM_DESTROY:
		{
			// 초상화용 핸들을 삭제한다.
			if(m_hHorseBitmap)
			{
				DeleteObject(m_hHorseBitmap);
				m_hHorseBitmap = NULL;
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
		case IDC_BUTTON_HORSETYPE_LEFT:
			{
				for ( SI32 i = m_siSelectedUnique - 1 ; i > 0 ; i -- )
				{
					if ( i == m_siOriUnique ) continue ;
					cltHorseTypeInfo *pclHorseTypeInfo = pclClient->pclHorseManager->pclHorseTypeInfo[i];
					if ( pclHorseTypeInfo == NULL ) continue ;
					char* pText = GetTxtFromMgr(667);
					if ( !strcmp(pText,pclHorseTypeInfo->szSpecies) ) continue ;

					m_siSelectedUnique = i ;
					SetHorsePortrait(hDlg);
					break;
				}
			}
			return TRUE ;
		case IDC_BUTTON_HORSETYPE_RIGHT:
			{
				for ( SI32 i = m_siSelectedUnique + 1 ; i < MAX_HORSE_TYPE_INFO ; i ++ )
				{
					if ( i == m_siOriUnique ) continue ;
					cltHorseTypeInfo *pclHorseTypeInfo = pclClient->pclHorseManager->pclHorseTypeInfo[i];
					if ( pclHorseTypeInfo == NULL ) continue ;
					char* pText = GetTxtFromMgr(667);
					if ( !strcmp(pText,pclHorseTypeInfo->szSpecies) ) continue ;

					m_siSelectedUnique = i ;
					SetHorsePortrait(hDlg);
					break;
				}
			}
			return TRUE ;
		case IDOK:
			{
				char horseunique = m_siSelectedUnique ;
				if ( horseunique == m_siOriUnique ) return TRUE ;
				if ( horseunique <= 1 && horseunique >= MAX_HORSE_TYPE_INFO ) return TRUE ;
				cltHorseTypeInfo *pclHorseTypeInfo = pclClient->pclHorseManager->pclHorseTypeInfo[horseunique];
				if ( pclHorseTypeInfo == NULL ) return TRUE ;

				char buffer[256] = "" ;

				SI32 itemunique = 0 ;
				SI32 horselife = 0 ;
				SI32 itemnum = 2 ;
				for ( SI32 i = 0 ; i < m_siMaxUnique * 3 ; i += 3 )
				{
					if ( !strcmp(HorseTypeItemUnique[i],pclHorseTypeInfo->szSpecies) )
					{
						itemunique = atoi(HorseTypeItemUnique[i+1]);
						horselife = atoi(HorseTypeItemUnique[i+2]);
						break ;
					}
				}

				if ( pclclient->pclCM->CR[1]->pclCI->clCharItem.GetItemNum(itemunique) < itemnum )
				{
					char* pText = GetTxtFromMgr(2834);
					sprintf(buffer, pText,
						pclClient->pclItemManager->GetName(itemunique),itemnum,
						pclClient->pclItemManager->GetName(itemunique));
					pText = GetTxtFromMgr(663);
					pclclient->m_pNotifyMsgDlg->SetText( pText,buffer);
					return TRUE;
				}

				if ( pclhorse->siLife - horselife < -4000 )
				{
					char* pText = GetTxtFromMgr(664);
					sprintf(buffer, pText,
						MAX_HORSE_LIFE+4000+4000);
					pText = GetTxtFromMgr(665);
					pclclient->m_pNotifyMsgDlg->SetText( pText,buffer);
					return TRUE;
				}

				char* pText = GetTxtFromMgr(666);
				sprintf(buffer, pText,
					pclHorseTypeInfo->szSpecies,horselife);
				pText = GetTxtFromMgr(663);
				if (  MessageBox(NULL, buffer, pText,
					MB_ICONQUESTION | MB_YESNO) == IDNO )
				{
					return TRUE;
				}

				cltMsg clMsg(GAMEMSG_REQUEST_CHANGE_HORSETYPE, 1, &horseunique);
				pclclient->SendMsgToServer((char*)&clMsg);

				DestroyWindow(m_hDlg);
				m_hDlg = NULL;
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

void cltNHorseTypeChange::SetHorsePortrait(HWND hDlg)
{
	cltHorseTypeInfo *pclHorseTypeInfo = pclClient->pclHorseManager->pclHorseTypeInfo[m_siSelectedUnique];

	if ( pclHorseTypeInfo == NULL ) return ;

	// 초상화용 핸들을 삭제한다.
	if(m_hHorseBitmap)
	{
		DeleteObject(m_hHorseBitmap);
		m_hHorseBitmap = NULL;
	}

	char strFileName[ 128 ] = "";
	char strPathFileName[ 128 ] = "";
	switch(m_siSelectedUnique)
	{
	case 1:	strcpy(strFileName, "BROWN_HORSE");	break;
	case 2:	strcpy(strFileName, "BLACK_HORSE"); break;
	case 3:	strcpy(strFileName, "WHITE_HORSE"); break;
	case 4:	strcpy(strFileName, "RED_HORSE");	break;
	case 5: strcpy(strFileName,	"ZEBRA_HORSE");	break;
	case 6: strcpy(strFileName,	"RUDOLF");	break;
	case 7: strcpy(strFileName,	"THOUSAND_HORSE"); break;
	default:strcpy(strFileName, "BROWN_HORSE");	break;
	}
	sprintf( strPathFileName, "Interface/Portrait/%s.bmp",	strFileName );
	m_hHorseBitmap = (HBITMAP)LoadImage( pclClient->GetInstance(), strPathFileName, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE );
	if ( m_hHorseBitmap != NULL )
	{
		SendDlgItemMessage( hDlg, IDC_STATIC_PORTRAIT_HORSETYPE, STM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)m_hHorseBitmap );
	}

	SetWindowText(GetDlgItem(hDlg,IDC_STATIC_HORSETYPE_NAME),pclHorseTypeInfo->szSpecies);
}

#endif