#include "..\Client.h"
#include "..\Music\Music.h"
#include "..\CommonLogic\MsgType.h"

#include "..\CommonLogic\Msg\MsgType-DailyQuest.h"
#include "..\Interface\DailyQuestDlg\DailyQuestSelectDlg.h"
#include "..\Interface\DailyQuestDlg\DailyQuestSituationDlg.h"

#include "../Interface/NotifyGetExpAndItemDlg/NotifyGetExpAndItemDlg.h"
#include "../Interface/SoundOnOffDlg/SoundOnOffDlg.h"
#include "../NInterface/NDailyQuestDlg/NDailyQuestSelectDlg.h"
#include "../NInterface/NSoundOnOffDlg/NSoundOnOffDlg.h"

#include "..\Interface\ErrorMsgDlg\ErrorMsgDlg.h"
#include "..\Interface\NotifyMsgDlg\NotifyMsgDlg.h"
#include "../NInterface/NNotifyGetExpAndItemDlg/NNotifyGetExpAndItemDlg.h"
//-------------------------------------
// Common
//-------------------------------------
#include "PersonInfo.h"
#include "Char\CharManager\CharManager.h"

void cltClient::DoMsg_GAMEMSG_RESPONSE_DAILYQUESTLIST(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_DailyQuestList* pclInfo = (cltGameMsgResponse_DailyQuestList*)pclMsg->cData;

	if ( pclClient->pclCM->CR[1]->GetMapIndex() == MAPINDEX_MAINFIELD )
		return;

	// 모병관 
	if( m_pDialog[ NDAILYQUEST_DLG ] == NULL )
		CreateInterface( NDAILYQUEST_DLG );

#ifdef _SAFE_MEMORY
	((CNDailyQuestSelectDlg*)m_pDialog[ NDAILYQUEST_DLG ])->SetList( &pclInfo->m_DailyQuestObjDataList[0] );
#else
	((CNDailyQuestSelectDlg*)m_pDialog[ NDAILYQUEST_DLG ])->SetList( pclInfo->m_DailyQuestObjDataList );
#endif
	
	return;
}

void cltClient::DoMsg_GAMEMSG_RESPONSE_DAILYQUESTSELECT(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_DailyQuestSelect* pclInfo = (cltGameMsgResponse_DailyQuestSelect*)pclMsg->cData;
	
	pclCM->CR[1]->pclCI->clIP.SetMoney( pclInfo->siLeftMoney );
	pclCM->CR[1]->pclCI->clDailyQuestInfo.Set( &pclInfo->DailyQuestInfo );
	
	if( m_pDialog[ NDAILYQUEST_DLG ] == NULL )
		CreateInterface( NDAILYQUEST_DLG );

	((CNDailyQuestSelectDlg*)m_pDialog[ NDAILYQUEST_DLG ])->LoadCurrentDailyQuestSituationText();
	((CNDailyQuestSelectDlg*)m_pDialog[ NDAILYQUEST_DLG ])->Refresh();

	if ( pclCM->CR[1]->pclCI->clDailyQuestInfo.siMonsterKind < 1 )
	{
		return;
	}

	TCHAR *MonsterName = (TCHAR*)pclKindInfoSet->GetName(pclCM->CR[1]->pclCI->clDailyQuestInfo.siMonsterKind );
	if ( MonsterName == NULL )
	{
		return;
	}
	
	TCHAR Time[ 64 ] = TEXT("");
	if ( pclCM->CR[1]->pclCI->clDailyQuestInfo.siLeftTime > 60 )
	{
		SI16 Minutes = pclCM->CR[1]->pclCI->clDailyQuestInfo.siLeftTime / 60;
		SI16 Seconds = pclCM->CR[1]->pclCI->clDailyQuestInfo.siLeftTime % 60;
		
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

		StringCchPrintf( Time, 64, pText, pclCM->CR[1]->pclCI->clDailyQuestInfo.siLeftTime );
	}
	
	TCHAR buffer[256];
	TCHAR* pText = GetTxtFromMgr(1255);

	StringCchPrintf(buffer, 256, pText, MonsterName, pclCM->CR[1]->pclCI->clDailyQuestInfo.siKillMonsterNum, pclCM->CR[1]->pclCI->clDailyQuestInfo.siMonsterNum, Time );
	pclMessage->SetDailyQuestMsg(buffer);

	return;
}

void cltClient::DoMsg_GAMEMSG_RESPONSE_DAILYQUESTSITUATION(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_DailyQuestSituation* pclInfo = (cltGameMsgResponse_DailyQuestSituation*)pclMsg->cData;

	pclCM->CR[1]->pclCI->clDailyQuestInfo.Set( &pclInfo->DailyQuestInfo );

	if( m_pDialog[ NDAILYQUEST_DLG ] == NULL )
		CreateInterface( NDAILYQUEST_DLG );

	((CNDailyQuestSelectDlg*)m_pDialog[ NDAILYQUEST_DLG ])->LoadCurrentDailyQuestSituationText();


	if ( pclCM->CR[1]->pclCI->clDailyQuestInfo.siMonsterKind < 1 )
	{
		return;
	}

	TCHAR *MonsterName = (TCHAR*)pclKindInfoSet->GetName(pclCM->CR[1]->pclCI->clDailyQuestInfo.siMonsterKind );
	if ( MonsterName == NULL )
	{
		return;
	}
	
	TCHAR Time[ 64 ] = TEXT("");
	if ( pclCM->CR[1]->pclCI->clDailyQuestInfo.siLeftTime > 60 )
	{
		SI16 Minutes = pclCM->CR[1]->pclCI->clDailyQuestInfo.siLeftTime / 60;
		SI16 Seconds = pclCM->CR[1]->pclCI->clDailyQuestInfo.siLeftTime % 60;
		
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

		StringCchPrintf( Time, 64, pText, pclCM->CR[1]->pclCI->clDailyQuestInfo.siLeftTime );
	}
	
	TCHAR buffer[256];
	TCHAR* pText = GetTxtFromMgr(1255);

	StringCchPrintf(buffer, 256, pText, MonsterName, pclCM->CR[1]->pclCI->clDailyQuestInfo.siKillMonsterNum, pclCM->CR[1]->pclCI->clDailyQuestInfo.siMonsterNum, Time );
	pclMessage->SetDailyQuestMsg(buffer);

	return;
}

void cltClient::DoMsg_GAMEMSG_RESPONSE_DAILYQUESTCHANGE(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_DailyQuestChange* pclInfo = (cltGameMsgResponse_DailyQuestChange*)pclMsg->cData;

	pclCM->CR[1]->pclCI->clDailyQuestInfo.siLeftTime = pclInfo->siLeftTime;
	pclCM->CR[1]->pclCI->clDailyQuestInfo.siKillMonsterNum = pclInfo->siKillMonsterNum;

	/*
	if ( m_pDailyQuestSelectDlg->IsShow() )
	{
		m_pDailyQuestSelectDlg->LoadCurrentDailyQuestSituationText();
	}
	*/

	if ( pclCM->CR[1]->pclCI->clDailyQuestInfo.siMonsterKind < 1 )
	{
		return;
	}

	TCHAR *MonsterName = (TCHAR*)pclKindInfoSet->GetName(pclCM->CR[1]->pclCI->clDailyQuestInfo.siMonsterKind );
	if ( MonsterName == NULL )
	{
		return;
	}
	
	TCHAR Time[ 64 ] = TEXT("");
	if ( pclCM->CR[1]->pclCI->clDailyQuestInfo.siLeftTime > 60 )
	{
		SI16 Minutes = pclCM->CR[1]->pclCI->clDailyQuestInfo.siLeftTime / 60;
		SI16 Seconds = pclCM->CR[1]->pclCI->clDailyQuestInfo.siLeftTime % 60;
		
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

		StringCchPrintf( Time, 64, pText, pclCM->CR[1]->pclCI->clDailyQuestInfo.siLeftTime );
	}
	
	TCHAR buffer[256];
	TCHAR* pText = GetTxtFromMgr(1255);

	StringCchPrintf(buffer, 256, pText, MonsterName, pclCM->CR[1]->pclCI->clDailyQuestInfo.siKillMonsterNum, pclCM->CR[1]->pclCI->clDailyQuestInfo.siMonsterNum, Time );
	pclMessage->SetDailyQuestMsg(buffer);
	
	return;
}

void cltClient::DoMsg_GAMEMSG_RESPONSE_DAILYQUESTEND(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_DailyQuestEnd* pclInfo = (cltGameMsgResponse_DailyQuestEnd*)pclMsg->cData;

	pclCM->CR[1]->pclCI->clDailyQuestInfo.Init();
	
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

		((CNNotifyGetExpAndItemDlg*)m_pDialog[ NNOTIFYGETEXPANDITEM_DLG ])->Show( &clItem, pclInfo->siChangedExp, pclInfo->siAddGP, pclInfo->siAddMoney );

		PushEffect( pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_DAILYQUEST_SUCCESS")), 0, 0 );
		

	}
	

	if( m_pDialog[ NDAILYQUEST_DLG ] != NULL && m_pDialog[ NDAILYQUEST_DLG ]->IsShow() == true )
	{
		//CreateInterface( NDAILYQUEST_DLG );

		if ( pclInfo->bSuccess )
		{
			((CNDailyQuestSelectDlg*)m_pDialog[ NDAILYQUEST_DLG ])->DailyQuestComplete( true );

		}
		else
		{
			((CNDailyQuestSelectDlg*)m_pDialog[ NDAILYQUEST_DLG ])->DailyQuestComplete( false );
		}
	}
	else if ( pclInfo->bSuccess == false )
	{
		TCHAR* pTitle = GetTxtFromMgr(1256);
		TCHAR* pText = GetTxtFromMgr(1257);

		m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText );
	}

	
	return;
}