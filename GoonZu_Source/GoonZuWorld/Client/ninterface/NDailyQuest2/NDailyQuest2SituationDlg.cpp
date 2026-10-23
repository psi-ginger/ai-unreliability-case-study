/* ==========================================================================
	클래스 :		CNDailyQuestSituationDlg

	작성일 :		05/05/10
	작성자 :		정용래
	
	변경사항 :		사용 안함

	차후작업 :		

   ========================================================================*/

#if defined(_NDAILYQUEST2SELECTDLG) && defined(_STARTING_NOT_USING)

#include "DailyQuest2SituationDlg.h"

#include "../../CommonLogic/Msg/MsgType-DailyQuest.h"

#include "../../Client/client.h"

//-----------------------------
// Common
//-----------------------------
#include "PersonInfo.h"
#include "Char\CharManager\CharManager.h"

extern cltCommonLogic* pclClient;

CNDailyQuest2SituationDlg::CNDailyQuest2SituationDlg()
{
	m_hDlg = NULL;
	m_bLoadSituationComplete = false;
}

CNDailyQuest2SituationDlg::~CNDailyQuest2SituationDlg()
{
	if ( m_hDlg )
	{
		DestroyWindow( m_hDlg );
	}
}

void CNDailyQuest2SituationDlg::Initialize()
{
	return;
}

void CNDailyQuest2SituationDlg::Create( HINSTANCE hInst, HWND hWnd )
{
	m_hDlg = CreateDialog( hInst, MAKEINTRESOURCE( IDD_DIALOG_DAILYQUESTSITUATION ), hWnd,  StatiCNDailyQuest2SituationDlgProc );
	Hide();
	return;
}

BOOL CALLBACK CNDailyQuest2SituationDlg::StatiCNDailyQuest2SituationDlgProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam )
{
	cltClient *pclclient = (cltClient *)pclClient;
	//return pclclient->m_pDailyQuestSituationDlg->DailyQuestSituationDlgProc( hDlg, iMsg, wParam, lParam );
	return FALSE;
}

BOOL CALLBACK CNDailyQuest2SituationDlg::DailyQuest2SituationDlgProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam )
{
	switch( iMsg )
	{
	case WM_INITDIALOG:
		{
			m_hDlg = hDlg;
		}
		return TRUE;

	case WM_TIMER:
		{
			if ( m_bLoadSituationComplete )
			{
				--pclClient->pclCM->CR[1]->pclCI->clDailyQuest2Info.siLeftTime;

				if ( pclClient->pclCM->CR[1]->pclCI->clDailyQuest2Info.siLeftTime <= 0 )
				{
					pclClient->pclCM->CR[1]->pclCI->clDailyQuest2Info.siLeftTime = 0;

					cltGameMsgRequest_DailyQuest2End clDailyQuest2End;
					cltMsg clMsg( GAMEMSG_REQUEST_DAILYQUEST2END, sizeof( clDailyQuest2End ), (char*)&clDailyQuest2End );
					pclClient->SendMsgToServer((char*)&clMsg);
				}
				
				SetDlgItemInt( m_hDlg, IDC_EDIT_LEFTTIME, pclClient->pclCM->CR[1]->pclCI->clDailyQuest2Info.siLeftTime, FALSE );
			}
		}
		break;

	case WM_COMMAND:
		{
			switch( LOWORD( wParam ) )
			{
			case IDOK:
			case IDCANCEL:
				Hide();
				return FALSE;
			case IDC_BUTTON_DAILYQUEST_CANCEL:
				{
					cltGameMsgRequest_DailyQuest2End clDailyQuest2End;
					cltMsg clMsg( GAMEMSG_REQUEST_DAILYQUEST2END, sizeof( clDailyQuest2End ), (char*)&clDailyQuest2End );
					pclClient->SendMsgToServer((char*)&clMsg);
				}
				break;
			}
		}
		break;

	case WM_DESTROY:
		{
			m_hDlg = NULL;
		}
		break;
	}
	return FALSE;
}

bool CNDailyQuest2SituationDlg::IsShow()
{
	return m_bShow;
}

void CNDailyQuest2SituationDlg::Show()
{
	if ( m_bShow )
	{
		Hide();
	}

	cltGameMsgRequest_DailyQuest2Situation clDailyQuest2Situation;
	cltMsg clMsg( GAMEMSG_REQUEST_DAILYQUEST2SITUATION, sizeof( clDailyQuest2Situation ), (char*)&clDailyQuest2Situation );
	pclClient->SendMsgToServer((char*)&clMsg);
	
	m_bLoadSituationComplete = false;
	
	SetTimer( m_hDlg, 0, 1000, NULL );
	
	m_bShow = true;
	ShowWindow( m_hDlg, SW_SHOW );

	return;
}

void CNDailyQuest2SituationDlg::Hide()
{
	m_bLoadSituationComplete = false;
	KillTimer( m_hDlg, 0 );
	m_bShow = false;
	ShowWindow( m_hDlg, SW_HIDE );

	return;
}

void CNDailyQuest2SituationDlg::LoadSituation()
{
	m_bLoadSituationComplete = true;

	char Buffer[ 1024 ] = "";

	cltPDailyQuest2Info *pDailyQuest2Info = &pclClient->pclCM->CR[1]->pclCI->clDailyQuest2Info;
	if ( pDailyQuest2Info == NULL )
	{
		return;
	}

	char *MonsterName = (char*)pclClient->pclKindInfoSet->GetName( pDailyQuest2Info->siMonsterKind );

	if ( MonsterName == NULL )
	{
		return;
	}

	char *ItemName = (char*)pclClient->pclItemManager->GetName( pDailyQuest2Info->siRewardItemUnique );
	if ( ItemName == NULL )
	{
		return;
	}

	char* pText = GetTxtFromMgr(1888);

	sprintf( Buffer, pText, MonsterName, pDailyQuest2Info->siMonsterNum, ItemName, pDailyQuest2Info->siRewardItemNum, pDailyQuest2Info->siRewardExp );

	SetWindowText( GetDlgItem( m_hDlg, IDC_EDIT_QUESTSITUATION), Buffer );

	sprintf( Buffer, "%d/%d", pDailyQuest2Info->siKillMonsterNum, pDailyQuest2Info->siMonsterNum );
	SetWindowText( GetDlgItem( m_hDlg, IDC_EDIT_KILLMONSTERNUM ), Buffer );
	
	SetDlgItemInt( m_hDlg, IDC_EDIT_LEFTTIME, pDailyQuest2Info->siLeftTime, FALSE );

	return;
}

#endif
