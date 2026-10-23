/* ==========================================================================
	클래스 :		CNDailyQuestSituationDlg

	작성일 :		05/05/10
	작성자 :		정용래
	
	변경사항 :		사용 안함

	차후작업 :		

   ========================================================================*/

#if defined(_NDAILYQUEST3SELECTDLG) && defined(_STARTING_NOT_USING)

#include "DailyQuest3SituationDlg.h"

#include "../../CommonLogic/Msg/MsgType-DailyQuest.h"

#include "../../Client/client.h"

//-----------------------------
// Common
//-----------------------------
#include "PersonInfo.h"
#include "Char\CharManager\CharManager.h"

extern cltCommonLogic* pclClient;

CNDailyQuest3SituationDlg::CNDailyQuest3SituationDlg()
{
	m_hDlg = NULL;
	m_bLoadSituationComplete = false;
}

CNDailyQuest3SituationDlg::~CNDailyQuest3SituationDlg()
{
	if ( m_hDlg )
	{
		DestroyWindow( m_hDlg );
	}
}

void CNDailyQuest3SituationDlg::Initialize()
{
	return;
}

void CNDailyQuest3SituationDlg::Create( HINSTANCE hInst, HWND hWnd )
{
	m_hDlg = CreateDialog( hInst, MAKEINTRESOURCE( IDD_DIALOG_DAILYQUESTSITUATION ), hWnd,  StatiCNDailyQuest2SituationDlgProc );
	Hide();
	return;
}

BOOL CALLBACK CNDailyQuest3SituationDlg::StatiCNDailyQuest3SituationDlgProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam )
{
	cltClient *pclclient = (cltClient *)pclClient;
	//return pclclient->m_pDailyQuestSituationDlg->DailyQuestSituationDlgProc( hDlg, iMsg, wParam, lParam );
	return FALSE;
}

BOOL CALLBACK CNDailyQuest3SituationDlg::DailyQuest3SituationDlgProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam )
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
				--pclClient->pclCM->CR[1]->pclCI->clDailyQuest3Info.siLeftTime;

				if ( pclClient->pclCM->CR[1]->pclCI->clDailyQuest3Info.siLeftTime <= 0 )
				{
					pclClient->pclCM->CR[1]->pclCI->clDailyQuest3Info.siLeftTime = 0;

					cltGameMsgRequest_DailyQuest3End clDailyQuest3End;
					cltMsg clMsg( GAMEMSG_REQUEST_DAILYQUEST3END, sizeof( clDailyQuest3End ), (char*)&clDailyQuest3End );
					pclClient->SendMsgToServer((char*)&clMsg);
				}
				
				SetDlgItemInt( m_hDlg, IDC_EDIT_LEFTTIME, pclClient->pclCM->CR[1]->pclCI->clDailyQuest3Info.siLeftTime, FALSE );
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
					cltGameMsgRequest_DailyQuest3End clDailyQuest3End;
					cltMsg clMsg( GAMEMSG_REQUEST_DAILYQUEST3END, sizeof( clDailyQuest3End ), (char*)&clDailyQuest3End );
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

bool CNDailyQuest3SituationDlg::IsShow()
{
	return m_bShow;
}

void CNDailyQuest3SituationDlg::Show()
{
	if ( m_bShow )
	{
		Hide();
	}

	cltGameMsgRequest_DailyQuest3Situation clDailyQuest3Situation;
	cltMsg clMsg( GAMEMSG_REQUEST_DAILYQUEST3SITUATION, sizeof( clDailyQuest3Situation ), (char*)&clDailyQuest3Situation );
	pclClient->SendMsgToServer((char*)&clMsg);
	
	m_bLoadSituationComplete = false;
	
	SetTimer( m_hDlg, 0, 1000, NULL );
	
	m_bShow = true;
	ShowWindow( m_hDlg, SW_SHOW );

	return;
}

void CNDailyQuest3SituationDlg::Hide()
{
	m_bLoadSituationComplete = false;
	KillTimer( m_hDlg, 0 );
	m_bShow = false;
	ShowWindow( m_hDlg, SW_HIDE );

	return;
}

void CNDailyQuest3SituationDlg::LoadSituation()
{
	m_bLoadSituationComplete = true;

	char Buffer[ 1024 ] = "";

	cltPDailyQuest3Info *pDailyQuest3Info = &pclClient->pclCM->CR[1]->pclCI->clDailyQuest3Info;
	if ( pDailyQuest3Info == NULL )
	{
		return;
	}

	char *ItemName = (char*)pclClient->pclItemManager->GetName( pDailyQuest3Info->siItemUnique );

	if ( ItemName == NULL )
	{
		return;
	}

	char *RewardItemName = (char*)pclClient->pclItemManager->GetName( pDailyQuest3Info->siRewardItemUnique );
	if ( ItemName == NULL )
	{
		return;
	}

	char* pText = GetTxtFromMgr(1888);

	sprintf( Buffer, pText, ItemName, pDailyQuest3Info->siItemNum, RewardItemName, pDailyQuest3Info->siRewardItemNum, pDailyQuest3Info->siRewardExp );

	SetWindowText( GetDlgItem( m_hDlg, IDC_EDIT_QUESTSITUATION), Buffer );

	sprintf( Buffer, "%d/%d", pDailyQuest3Info->siMakedeItemNum, pDailyQuest3Info->siItemNum );
	SetWindowText( GetDlgItem( m_hDlg, IDC_EDIT_KILLMONSTERNUM ), Buffer );
	
	SetDlgItemInt( m_hDlg, IDC_EDIT_LEFTTIME, pDailyQuest2Info->siLeftTime, FALSE );

	return;
}

#endif
