#include "..\Client.h"
#include "..\Music\Music.h"
#include "..\CommonLogic\MsgType.h"

#include "..\CommonLogic\Msg\MsgType-DailyQuest2.h"
#include "..\Interface\DailyQuest2Dlg\DailyQuest2SelectDlg.h"
#include "..\Interface\DailyQuest2Dlg\DailyQuest2SituationDlg.h"

#include "../Interface/NotifyGetExpAndItemDlg/NotifyGetExpAndItemDlg.h"
#include "../Interface/SoundOnOffDlg/SoundOnOffDlg.h"
#include "../NInterface/NDailyQuest2/NDailyQuest2SelectDlg.h"
#include "../NInterface/NSoundOnOffDlg/NSoundOnOffDlg.h"

#include "..\Interface\ErrorMsgDlg\ErrorMsgDlg.h"
#include "..\Interface\NotifyMsgDlg\NotifyMsgDlg.h"
#include "../NInterface/NNotifyGetExpAndItemDlg/NNotifyGetExpAndItemDlg.h"
//-------------------------------------
// Common
//-------------------------------------
#include "PersonInfo.h"
#include "Char\CharManager\CharManager.h"

void cltClient::DoMsg_GAMEMSG_RESPONSE_DAILYQUEST2LIST(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_DailyQuest2List* pclInfo = (cltGameMsgResponse_DailyQuest2List*)pclMsg->cData;

	if ( pclClient->pclCM->CR[1]->GetMapIndex() == MAPINDEX_MAINFIELD )
		return;

	// 모병관 
	if( m_pDialog[ NDAILYQUEST2_DLG ] == NULL )
		CreateInterface( NDAILYQUEST2_DLG );

#ifdef _SAFE_MEMORY
	((CDailyQuest2SelectDlg*)m_pDialog[ NDAILYQUEST2_DLG ])->SetList( &pclInfo->m_DailyQuest2ObjDataList[0] );
#else
	((CDailyQuest2SelectDlg*)m_pDialog[ NDAILYQUEST2_DLG ])->SetList( pclInfo->m_DailyQuest2ObjDataList );
#endif
	
	return;
}

void cltClient::DoMsg_GAMEMSG_RESPONSE_DAILYQUEST2SELECT(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_DailyQuest2Select* pclInfo = (cltGameMsgResponse_DailyQuest2Select*)pclMsg->cData;
	
	pclCM->CR[1]->pclCI->clIP.SetMoney( pclInfo->siLeftMoney );
	pclCM->CR[1]->pclCI->clDailyQuest2Info.Set( &pclInfo->DailyQuest2Info );
	
	if( m_pDialog[ NDAILYQUEST2_DLG ] == NULL )
		CreateInterface( NDAILYQUEST2_DLG );

	((CDailyQuest2SelectDlg*)m_pDialog[ NDAILYQUEST2_DLG ])->LoadCurrentDailyQuest2SituationText();
	((CDailyQuest2SelectDlg*)m_pDialog[ NDAILYQUEST2_DLG ])->Refresh();

	if ( pclCM->CR[1]->pclCI->clDailyQuest2Info.siMonsterKind < 1 )
	{
		return;
	}

	TCHAR *MonsterName = (TCHAR*)pclKindInfoSet->GetName(pclCM->CR[1]->pclCI->clDailyQuest2Info.siMonsterKind );
	if ( MonsterName == NULL )
	{
		return;
	}
	
	TCHAR Time[ 64 ] = TEXT("");
	if ( pclCM->CR[1]->pclCI->clDailyQuestInfo.siLeftTime > 60 )
	{
		SI16 Minutes = pclCM->CR[1]->pclCI->clDailyQuest2Info.siLeftTime / 60;
		SI16 Seconds = pclCM->CR[1]->pclCI->clDailyQuest2Info.siLeftTime % 60;
		
		if ( Seconds == 0 )
		{
			TCHAR* pText = GetTxtFromMgr(1252);

			StringCchPrintf( Time, 64, pText, Minutes );
		}
		else
		{
			TCHAR* pText = GetTxtFromMgr(1253);

			StringCchPrintf( Time, 64, pText, Minutes, Seconds );
		}
	}
	else
	{
		TCHAR* pText = GetTxtFromMgr(1254);

		StringCchPrintf( Time, 64, pText, pclCM->CR[1]->pclCI->clDailyQuest2Info.siLeftTime );
	}
	
	TCHAR buffer[256];
	TCHAR* pText = GetTxtFromMgr(8096);

	StringCchPrintf(buffer, 256, pText, MonsterName, pclCM->CR[1]->pclCI->clDailyQuest2Info.siKillMonsterNum, pclCM->CR[1]->pclCI->clDailyQuestInfo.siMonsterNum, Time );
	pclMessage->SetDailyQuestMsg(buffer);

	return;
}

void cltClient::DoMsg_GAMEMSG_RESPONSE_DAILYQUEST2SITUATION(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_DailyQuest2Situation* pclInfo = (cltGameMsgResponse_DailyQuest2Situation*)pclMsg->cData;

	pclCM->CR[1]->pclCI->clDailyQuest2Info.Set( &pclInfo->DailyQuest2Info );

	if( m_pDialog[ NDAILYQUEST2_DLG ] == NULL )
		CreateInterface( NDAILYQUEST2_DLG );

	((CDailyQuest2SelectDlg*)m_pDialog[ NDAILYQUEST2_DLG ])->LoadCurrentDailyQuest2SituationText();


	if ( pclCM->CR[1]->pclCI->clDailyQuest2Info.siMonsterKind < 1 )
	{
		return;
	}

	TCHAR *MonsterName = (TCHAR*)pclKindInfoSet->GetName(pclCM->CR[1]->pclCI->clDailyQuest2Info.siMonsterKind );
	if ( MonsterName == NULL )
	{
		return;
	}
	
	TCHAR Time[ 64 ] = TEXT("");
	if ( pclCM->CR[1]->pclCI->clDailyQuest2Info.siLeftTime > 60 )
	{
		SI16 Minutes = pclCM->CR[1]->pclCI->clDailyQuest2Info.siLeftTime / 60;
		SI16 Seconds = pclCM->CR[1]->pclCI->clDailyQuest2Info.siLeftTime % 60;
		
		if ( Seconds == 0 )
		{
			TCHAR* pText = GetTxtFromMgr(1252);

			StringCchPrintf( Time, 64, pText, Minutes );
		}
		else
		{
			TCHAR* pText = GetTxtFromMgr(1253);

			StringCchPrintf( Time, 64, pText, Minutes, Seconds );
		}
	}
	else
	{
		TCHAR* pText = GetTxtFromMgr(1254);

		StringCchPrintf( Time, 64, pText, pclCM->CR[1]->pclCI->clDailyQuest2Info.siLeftTime );
	}
	
	TCHAR buffer[256];
	TCHAR* pText = GetTxtFromMgr(8096);

	StringCchPrintf(buffer, 256, pText, MonsterName, pclCM->CR[1]->pclCI->clDailyQuest2Info.siKillMonsterNum, pclCM->CR[1]->pclCI->clDailyQuest2Info.siMonsterNum, Time );
	pclMessage->SetDailyQuestMsg(buffer);

	return;
}

void cltClient::DoMsg_GAMEMSG_RESPONSE_DAILYQUEST2CHANGE(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_DailyQuest2Change* pclInfo = (cltGameMsgResponse_DailyQuest2Change*)pclMsg->cData;

	pclCM->CR[1]->pclCI->clDailyQuest2Info.siLeftTime = pclInfo->siLeftTime;
	pclCM->CR[1]->pclCI->clDailyQuest2Info.siKillMonsterNum = pclInfo->siKillMonsterNum;
	pclCM->CR[1]->pclCI->clDailyQuest2Info.bHiddenQuest = pclInfo->bHiddenquest;

	/*
	if ( m_pDailyQuestSelectDlg->IsShow() )
	{
		m_pDailyQuestSelectDlg->LoadCurrentDailyQuestSituationText();
	}
	*/

	if ( pclCM->CR[1]->pclCI->clDailyQuest2Info.siMonsterKind < 1 )
	{
		return;
	}

	TCHAR *MonsterName = (TCHAR*)pclKindInfoSet->GetName(pclCM->CR[1]->pclCI->clDailyQuest2Info.siMonsterKind );
	if ( MonsterName == NULL )
	{
		return;
	}
	
	TCHAR Time[ 64 ] = TEXT("");
	if ( pclCM->CR[1]->pclCI->clDailyQuest2Info.siLeftTime > 60 )
	{
		SI16 Minutes = pclCM->CR[1]->pclCI->clDailyQuest2Info.siLeftTime / 60;
		SI16 Seconds = pclCM->CR[1]->pclCI->clDailyQuest2Info.siLeftTime % 60;
		
		if ( Seconds == 0 )
		{
			TCHAR* pText = GetTxtFromMgr(1252);

			StringCchPrintf( Time, 64, pText, Minutes );
		}
		else
		{
			TCHAR* pText = GetTxtFromMgr(1253);

			StringCchPrintf( Time, 64, pText, Minutes, Seconds );
		}
	}
	else
	{
		TCHAR* pText = GetTxtFromMgr(1254);

		StringCchPrintf( Time, 64, pText, pclCM->CR[1]->pclCI->clDailyQuest2Info.siLeftTime );
	}
	
	TCHAR buffer[256];
	TCHAR* pText = GetTxtFromMgr(8267);

	StringCchPrintf(buffer, 256, pText, MonsterName, pclCM->CR[1]->pclCI->clDailyQuest2Info.siKillMonsterNum, pclCM->CR[1]->pclCI->clDailyQuest2Info.siMonsterNum, Time );
	pclMessage->SetDailyQuestMsg(buffer);
	
	return;
}

void cltClient::DoMsg_GAMEMSG_RESPONSE_DAILYQUEST2END(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_DailyQues2tEnd* pclInfo = (cltGameMsgResponse_DailyQues2tEnd*)pclMsg->cData;

	pclCM->CR[1]->pclCI->clDailyQuest2Info.Init();
	
	if ( pclInfo->bSuccess )
	{
		if ( pclInfo->siAddMoney )
		{
			// 보상 돈 업데이트
			pclCM->CR[1]->pclCI->clIP.IncreaseMoney( pclInfo->siAddMoney );
		}
		else
		{
			// 보상 아이템 업데이트 
			pclCM->CR[1]->pclCI->clCharItem.clItem[pclInfo->siInventoryItemIndex].Set(&pclInfo->clInvItem);
		}
		// 보상 경험치 업데이트
		pclCM->CR[1]->pclCI->clIP.SetCurExp( pclInfo->siCurExp );

		
		cltItem clItem;
		clItem.Set( &pclInfo->clInvItem );
		clItem.siItemNum = pclInfo->siChangedItemNum;

		if( m_pDialog[ NNOTIFYGETEXPANDITEM_DLG ] == NULL )
			CreateInterface( NNOTIFYGETEXPANDITEM_DLG );

		((CNNotifyGetExpAndItemDlg*)m_pDialog[ NNOTIFYGETEXPANDITEM_DLG ])->ShowDailyQuest2( &clItem, pclInfo->siChangedExp, pclInfo->siAddGP, pclInfo->siAddMoney );

		PushEffect( pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_DAILYQUEST_SUCCESS")), 0, 0 );
		

	}
	

	if( m_pDialog[ NDAILYQUEST2_DLG ] != NULL && m_pDialog[ NDAILYQUEST2_DLG ]->IsShow() == true )
	{
		//CreateInterface( NDAILYQUEST2_DLG );

		if ( pclInfo->bSuccess )
		{
			((CDailyQuest2SelectDlg*)m_pDialog[ NDAILYQUEST2_DLG ])->DailyQuest2Complete( true );

		}
		else
		{
			((CDailyQuest2SelectDlg*)m_pDialog[ NDAILYQUEST2_DLG ])->DailyQuest2Complete( false );
		}
	}
	else if ( pclInfo->bSuccess == false )
	{
		TCHAR* pTitle = GetTxtFromMgr(8058);
		TCHAR* pText = GetTxtFromMgr(1257);

		m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText );
	}

	
	return;
}