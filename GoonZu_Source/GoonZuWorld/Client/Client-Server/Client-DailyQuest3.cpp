#include "..\Client.h"
#include "..\Music\Music.h"
#include "..\CommonLogic\MsgType.h"

#include "..\CommonLogic\Msg\MsgType-DailyQuest3.h"
//#include "..\Interface\DailyQuest3Dlg\DailyQuest3SelectDlg.h"
//#include "..\Interface\DailyQuest3Dlg\DailyQuest3SituationDlg.h"

#include "../Interface/NotifyGetExpAndItemDlg/NotifyGetExpAndItemDlg.h"
#include "../Interface/SoundOnOffDlg/SoundOnOffDlg.h"
#include "../NInterface/NDailyQuest3/NDailyQuest3SelectDlg.h"
#include "../NInterface/NSoundOnOffDlg/NSoundOnOffDlg.h"

#include "..\Interface\ErrorMsgDlg\ErrorMsgDlg.h"
#include "..\Interface\NotifyMsgDlg\NotifyMsgDlg.h"
#include "../NInterface/NNotifyGetExpAndItemDlg/NNotifyGetExpAndItemDlg.h"
//-------------------------------------
// Common
//-------------------------------------
#include "PersonInfo.h"
#include "Char\CharManager\CharManager.h"

void cltClient::DoMsg_GAMEMSG_RESPONSE_DAILYQUEST3LIST(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_DailyQuest3List* pclInfo = (cltGameMsgResponse_DailyQuest3List*)pclMsg->cData;

	if ( pclClient->pclCM->CR[1]->GetMapIndex() == MAPINDEX_MAINFIELD )
		return;

	// 모병관 
	if( m_pDialog[ NDAILYQUEST3_DLG ] == NULL )
		CreateInterface( NDAILYQUEST3_DLG );

#ifdef _SAFE_MEMORY
	((CDailyQuest3SelectDlg*)m_pDialog[ NDAILYQUEST3_DLG ])->SetList( &pclInfo->m_DailyQuest3ObjDataList[0] );
#else
	((CDailyQuest3SelectDlg*)m_pDialog[ NDAILYQUEST3_DLG ])->SetList( pclInfo->m_DailyQuest3ObjDataList );
#endif
	
	return;
}

void cltClient::DoMsg_GAMEMSG_RESPONSE_DAILYQUEST3SELECT(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_DailyQuest3Select* pclInfo = (cltGameMsgResponse_DailyQuest3Select*)pclMsg->cData;
	
	pclCM->CR[1]->pclCI->clIP.SetMoney( pclInfo->siLeftMoney );
	pclCM->CR[1]->pclCI->clDailyQuest3Info.Set( &pclInfo->DailyQuest3Info );
	
	if( m_pDialog[ NDAILYQUEST3_DLG ] == NULL )
		CreateInterface( NDAILYQUEST3_DLG );

	((CDailyQuest3SelectDlg*)m_pDialog[ NDAILYQUEST3_DLG ])->LoadCurrentDailyQuest3SituationText();
	((CDailyQuest3SelectDlg*)m_pDialog[ NDAILYQUEST3_DLG ])->Refresh();

	if ( pclCM->CR[1]->pclCI->clDailyQuest3Info.siItemUnique < 1 )
	{
		return;
	}

	TCHAR *Itemname = (TCHAR*)pclClient->pclItemManager->GetName(pclCM->CR[1]->pclCI->clDailyQuest3Info.siItemUnique );
	if ( Itemname == NULL )
	{
		return;
	}
	
	TCHAR Time[ 64 ] = TEXT("");
	if ( pclCM->CR[1]->pclCI->clDailyQuest3Info.siLeftTime > 60 )
	{
		SI16 Minutes = pclCM->CR[1]->pclCI->clDailyQuest3Info.siLeftTime / 60;
		SI16 Seconds = pclCM->CR[1]->pclCI->clDailyQuest3Info.siLeftTime % 60;
		
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

		StringCchPrintf( Time, 64, pText, pclCM->CR[1]->pclCI->clDailyQuest3Info.siLeftTime );
	}
	
	TCHAR buffer[256];
	// 히든일경우 텍스트에서 시간을 뺸다
	if( pclInfo->DailyQuest3Info.bHiddenQuest == true  )
	{
		TCHAR* pText = GetTxtFromMgr(8453);
		StringCchPrintf(buffer, 256, pText, Itemname, pclCM->CR[1]->pclCI->clDailyQuest3Info.siMadeItemNum, pclCM->CR[1]->pclCI->clDailyQuest3Info.siItemNum);
		pclMessage->SetDailyQuestMsg(buffer);
	}
	else
	{
		TCHAR* pText = GetTxtFromMgr(8217);
		StringCchPrintf(buffer, 256, pText, Itemname, pclCM->CR[1]->pclCI->clDailyQuest3Info.siMadeItemNum, pclCM->CR[1]->pclCI->clDailyQuest3Info.siItemNum, Time );
		pclMessage->SetDailyQuestMsg(buffer);
	}

	return;
}

void cltClient::DoMsg_GAMEMSG_RESPONSE_DAILYQUEST3SITUATION(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_DailyQuest3Situation* pclInfo = (cltGameMsgResponse_DailyQuest3Situation*)pclMsg->cData;

	pclCM->CR[1]->pclCI->clDailyQuest3Info.Set( &pclInfo->DailyQuest3Info );

	if( m_pDialog[ NDAILYQUEST3_DLG ] == NULL )
		CreateInterface( NDAILYQUEST3_DLG );

	((CDailyQuest3SelectDlg*)m_pDialog[ NDAILYQUEST3_DLG ])->LoadCurrentDailyQuest3SituationText();


	if ( pclCM->CR[1]->pclCI->clDailyQuest3Info.siItemUnique < 1 )
	{
		return;
	}

	TCHAR *ItemName = (TCHAR*)pclClient->pclItemManager->GetName(pclCM->CR[1]->pclCI->clDailyQuest3Info.siItemUnique );
	if ( ItemName == NULL )
	{
		return;
	}
	
	TCHAR Time[ 64 ] = TEXT("");
	if ( pclCM->CR[1]->pclCI->clDailyQuest3Info.siLeftTime > 60 )
	{
		SI16 Minutes = pclCM->CR[1]->pclCI->clDailyQuest3Info.siLeftTime / 60;
		SI16 Seconds = pclCM->CR[1]->pclCI->clDailyQuest3Info.siLeftTime % 60;
		
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

		StringCchPrintf( Time, 64, pText, pclCM->CR[1]->pclCI->clDailyQuest3Info.siLeftTime );
	}
	
	TCHAR buffer[256];
	// 히든일경우 텍스트에서 시간을 뺸다
	if( pclInfo->DailyQuest3Info.bHiddenQuest	==	true )
	{
		TCHAR* pText = GetTxtFromMgr(8453);
		StringCchPrintf(buffer, 256, pText, ItemName, pclCM->CR[1]->pclCI->clDailyQuest3Info.siMadeItemNum, pclCM->CR[1]->pclCI->clDailyQuest3Info.siItemNum);
		pclMessage->SetDailyQuestMsg(buffer);
	}
	else
	{
		TCHAR* pText = GetTxtFromMgr(8217);
		StringCchPrintf(buffer, 256, pText, ItemName, pclCM->CR[1]->pclCI->clDailyQuest3Info.siMadeItemNum, pclCM->CR[1]->pclCI->clDailyQuest3Info.siItemNum, Time );
		pclMessage->SetDailyQuestMsg(buffer);
	}

	return;
}

void cltClient::DoMsg_GAMEMSG_RESPONSE_DAILYQUEST3CHANGE(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_DailyQuest3Change* pclInfo = (cltGameMsgResponse_DailyQuest3Change*)pclMsg->cData;

	pclCM->CR[1]->pclCI->clDailyQuest3Info.siLeftTime = pclInfo->siLeftTime;
	pclCM->CR[1]->pclCI->clDailyQuest3Info.siMadeItemNum = pclInfo->siMakedItem;
	pclCM->CR[1]->pclCI->clDailyQuest3Info.bHiddenQuest = pclInfo->bHiddenquest;

	/*
	if ( m_pDailyQuestSelectDlg->IsShow() )
	{
		m_pDailyQuestSelectDlg->LoadCurrentDailyQuestSituationText();
	}
	*/

	if ( pclCM->CR[1]->pclCI->clDailyQuest3Info.siItemUnique < 1 )
	{
		return;
	}

	TCHAR *Itemname = (TCHAR*)pclClient->pclItemManager->GetName(pclCM->CR[1]->pclCI->clDailyQuest3Info.siItemUnique );
	if ( Itemname == NULL )
	{
		return;
	}
	
	TCHAR Time[ 64 ] = TEXT("");
	if ( pclCM->CR[1]->pclCI->clDailyQuest3Info.siLeftTime > 60 )
	{
		SI16 Minutes = pclCM->CR[1]->pclCI->clDailyQuest3Info.siLeftTime / 60;
		SI16 Seconds = pclCM->CR[1]->pclCI->clDailyQuest3Info.siLeftTime % 60;
		
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

		StringCchPrintf( Time, 64, pText, pclCM->CR[1]->pclCI->clDailyQuest3Info.siLeftTime );
	}
	
	TCHAR buffer[256];
	// 해당 퀘스트가 히든 퀘스트면 시간을 보여줄 필요가 없다.
	if(pclInfo->bHiddenquest == true )
	{
		TCHAR* pText = GetTxtFromMgr(8453);
		StringCchPrintf(buffer, 256, pText, Itemname, pclCM->CR[1]->pclCI->clDailyQuest3Info.siMadeItemNum, pclCM->CR[1]->pclCI->clDailyQuest3Info.siItemNum);
		pclMessage->SetDailyQuestMsg(buffer);
	}
	// 일반 퀘스트
	else
	{
		TCHAR* pText = GetTxtFromMgr(8268);
		StringCchPrintf(buffer, 256, pText, Itemname, pclCM->CR[1]->pclCI->clDailyQuest3Info.siMadeItemNum, pclCM->CR[1]->pclCI->clDailyQuest3Info.siItemNum, Time );
		pclMessage->SetDailyQuestMsg(buffer);
	}	
	return;
}

void cltClient::DoMsg_GAMEMSG_RESPONSE_DAILYQUEST3END(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_DailyQuest3End* pclInfo = (cltGameMsgResponse_DailyQuest3End*)pclMsg->cData;

	pclCM->CR[1]->pclCI->clDailyQuest3Info.Init();
	
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

		((CNNotifyGetExpAndItemDlg*)m_pDialog[ NNOTIFYGETEXPANDITEM_DLG ])->ShowDailyQuest3( &clItem, pclInfo->siChangedExp, pclInfo->siAddGP, pclInfo->siAddMoney );

		PushEffect( pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_DAILYQUEST_SUCCESS")), 0, 0 );
		

	}
	

	if( m_pDialog[ NDAILYQUEST3_DLG ] != NULL && m_pDialog[ NDAILYQUEST3_DLG ]->IsShow() == true )
	{
		//CreateInterface( NDAILYQUEST2_DLG );

		if ( pclInfo->bSuccess )
		{
			((CDailyQuest3SelectDlg*)m_pDialog[ NDAILYQUEST3_DLG ])->DailyQuest3Complete( true );

		}
		else
		{
			((CDailyQuest3SelectDlg*)m_pDialog[ NDAILYQUEST3_DLG ])->DailyQuest3Complete( false );
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