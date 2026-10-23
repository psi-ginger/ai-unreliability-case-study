#include "DailyQuestSituationDlg.h"

#include "../../CommonLogic/Msg/MsgType-DailyQuest.h"

#include "../../Client/client.h"

//-----------------------------
// Common
//-----------------------------
#include "PersonInfo.h"
#include "Char\CharManager\CharManager.h"

extern cltCommonLogic* pclClient;

CDailyQuestSituationDlg::CDailyQuestSituationDlg()
{
	m_hDlg = NULL;
	m_bLoadSituationComplete = false;
}

CDailyQuestSituationDlg::~CDailyQuestSituationDlg()
{
	if ( m_hDlg )
	{
		DestroyWindow( m_hDlg );
	}
}

void CDailyQuestSituationDlg::Initialize()
{
	return;
}

void CDailyQuestSituationDlg::Create( HINSTANCE hInst, HWND hWnd )
{
	m_hDlg = CreateDialog( hInst, MAKEINTRESOURCE( IDD_DIALOG_DAILYQUESTSITUATION ), hWnd,  StaticDailyQuestSituationDlgProc );
	Hide();
	return;
}

BOOL CALLBACK CDailyQuestSituationDlg::StaticDailyQuestSituationDlgProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam )
{
	cltClient *pclclient = (cltClient *)pclClient;
	//return pclclient->m_pDailyQuestSituationDlg->DailyQuestSituationDlgProc( hDlg, iMsg, wParam, lParam );
	return FALSE;
}

BOOL CALLBACK CDailyQuestSituationDlg::DailyQuestSituationDlgProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam )
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
				--pclClient->pclCM->CR[1]->pclCI->clDailyQuestInfo.siLeftTime;

				if ( pclClient->pclCM->CR[1]->pclCI->clDailyQuestInfo.siLeftTime <= 0 )
				{
					pclClient->pclCM->CR[1]->pclCI->clDailyQuestInfo.siLeftTime = 0;

					cltGameMsgRequest_DailyQuestEnd clDailyQuestEnd;
					cltMsg clMsg( GAMEMSG_REQUEST_DAILYQUESTEND, sizeof( clDailyQuestEnd ), (char*)&clDailyQuestEnd );
					pclClient->SendMsgToServer((char*)&clMsg);
				}
				
				SetDlgItemInt( m_hDlg, IDC_EDIT_LEFTTIME, pclClient->pclCM->CR[1]->pclCI->clDailyQuestInfo.siLeftTime, FALSE );
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
					cltGameMsgRequest_DailyQuestEnd clDailyQuestEnd;
					cltMsg clMsg( GAMEMSG_REQUEST_DAILYQUESTEND, sizeof( clDailyQuestEnd ), (char*)&clDailyQuestEnd );
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

bool CDailyQuestSituationDlg::IsShow()
{
	return m_bShow;
}

void CDailyQuestSituationDlg::Show()
{
	if ( m_bShow )
	{
		Hide();
	}

	cltGameMsgRequest_DailyQuestSituation clDailyQuestSituation;
	cltMsg clMsg( GAMEMSG_REQUEST_DAILYQUESTSITUATION, sizeof( clDailyQuestSituation ), (char*)&clDailyQuestSituation );
	pclClient->SendMsgToServer((char*)&clMsg);
	
	m_bLoadSituationComplete = false;
	
	SetTimer( m_hDlg, 0, 1000, NULL );
	
	m_bShow = true;
	ShowWindow( m_hDlg, SW_SHOW );

	return;
}

void CDailyQuestSituationDlg::Hide()
{
	m_bLoadSituationComplete = false;
	KillTimer( m_hDlg, 0 );
	m_bShow = false;
	ShowWindow( m_hDlg, SW_HIDE );

	return;
}

void CDailyQuestSituationDlg::LoadSituation()
{
	m_bLoadSituationComplete = true;

	char Buffer[ 1024 ] = "";

	cltPDailyQuestInfo *pDailyQuestInfo = &pclClient->pclCM->CR[1]->pclCI->clDailyQuestInfo;
	if ( pDailyQuestInfo == NULL )
	{
		return;
	}

	char *MonsterName = (char*)pclClient->pclKindInfoSet->GetName( pDailyQuestInfo->siMonsterKind );

	if ( MonsterName == NULL )
	{
		return;
	}

	char *ItemName = (char*)pclClient->pclItemManager->GetName( pDailyQuestInfo->siRewardItemUnique );
	if ( ItemName == NULL )
	{
		return;
	}

	char* pText = GetTxtFromMgr(1888);
	
	char num[128]= " " ;
	char item[128]= " " ;
	char itemnum[128]= " " ;
	char exp[128]= " " ;
	
	sprintf(num,"%d",pDailyQuestInfo->siMonsterNum);	
	sprintf(itemnum,"%d",pDailyQuestInfo->siRewardItemNum);
	sprintf(exp,"%d",pDailyQuestInfo->siRewardExp );

	Nsprintf(Buffer,pText,"monster",  MonsterName  ,"num", num  ,"item", ItemName   ,"itemnum",  itemnum  ,"exp", exp   ,NULL);
	//sprintf( Buffer, pText, MonsterName, pDailyQuestInfo->siMonsterNum, ItemName, pDailyQuestInfo->siRewardItemNum, pDailyQuestInfo->siRewardExp );

	SetWindowText( GetDlgItem( m_hDlg, IDC_EDIT_QUESTSITUATION), Buffer );

	sprintf( Buffer, "%d/%d", pDailyQuestInfo->siKillMonsterNum, pDailyQuestInfo->siMonsterNum );
	SetWindowText( GetDlgItem( m_hDlg, IDC_EDIT_KILLMONSTERNUM ), Buffer );
	
	SetDlgItemInt( m_hDlg, IDC_EDIT_LEFTTIME, pDailyQuestInfo->siLeftTime, FALSE );

	return;
}