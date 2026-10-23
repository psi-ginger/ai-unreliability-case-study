
#include "Char\CharManager\CharManager.h"


//---------------------------------------
// Server
//---------------------------------------
#include "..\Server\Server.h"
#include "..\Server\Statistics\Statistics.h"

//--------------------------------------
// CommonLogic
//--------------------------------------
#include "..\CommonLogic\Msg\MsgType-DailyQuest3.h"
#include "..\CommonLogic\MsgType-Person.h"

//---------------------------------------
// DBManager
//---------------------------------------
#include "..\..\..\DBManager\GameDBManager_World\DBMsg-DailyQuest3.h"
#include "..\..\..\DBManager\GameDBManager_World\DBMsg-Person.h"
#include "..\..\..\DBManager\GameDBManager_World\DBMsg-Guild.h"
#include "..\..\..\DBManager\GameDBManager_World\DBMsg-Item.h"


void cltServer::DoMsg_DBMSG_RESPONSE_DAILYQUEST3ADD( sPacketHeader* pPacket)
{
	sDBResponse_DailyQuest3Add * pclMsg = (sDBResponse_DailyQuest3Add*)pPacket;

	SI32 id = pclMsg->siCharID;
	
	if ( pclMsg->siResult == 1 )
	{
		// LEEKH MODIFY 2007.10.25
		if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
		{
			// [로그추가 : 황진성 2007. 10. 8] // 잭(모병관) 시작.
			// param1 == 몬스터 수.
			// param2 == 상품 아이템 유니크.
			// param3 == 상품 아이템 개수.
			// param4 == 상금액
			// param5 == 히든 퀘스트 여부.
			cltCharServer* pclchar = NULL;
			if(pclCM->IsValidID(id))		pclchar = (cltCharServer*)pclCM->CR[id];

			SendLog_WriteLogToDB(LOGCOMMAND_TYPE_QUEST, LOGCOMMAND_INDEX_QUEST_DAILY_QUEST3_ADD, 
								0, pclchar, NULL, 0, pclMsg->siFee, pclMsg->siLeftMoney, NULL, 0, pclMsg->siItemUnique, 0, 0, 0, 0, 
								pclMsg->siItemNum, pclMsg->siRewardItemUnique, pclMsg->siRewardItemNum, pclMsg->siRewardExp, pclMsg->bHiddenQuest, NULL,NULL);
		}
	}

	if ( pclMsg->siResult != 1 )
	{
		return;
	}

	if ( pclMsg->siPersonID < 1 || pclMsg->siCharID < 1 )
	{
		return;
	}

	if ( pclMsg->siVillageUnique > 0 && pclMsg->clStrMoney.itMoney > 0 )
	{
		cltStrInfo *pStrInfo = GetStrInfo( pclMsg->siVillageUnique, pclMsg->siStrType );
		if( pStrInfo ) 
		{
			// [수정 : 황진성 2008. 1. 16 => 잭이 마을에 고용되서 유저가 사용할때 마을에 주던 이익금이 자본금으로 들어가는거 수정]
			//pStrInfo->clCommonInfo.clMoney.Set( &pclMsg->clStrMoney );
			pStrInfo->clCityhallStrInfo.clProfitMoney = pclMsg->clStrMoney;
		}
	}

	cltMoney clMoney;
	clMoney.Set( pclMsg->siFee );
	clCurrencyInfo.clOutputMoney_QuestFee.Increase( &clMoney );

	pclDailyQuest3Mgr->InitQuest3ListData( pclMsg->siQuestListIndex, pclMsg->siItemUnique, 
		pclMsg->siItemNum, pclMsg->siRewardItemUnique, pclMsg->siRewardItemNum, 
		pclMsg->siFee, pclMsg->siLeftTime,/* pclMsg->siGuildPoint, */pclMsg->siRewardExp, pclMsg->bHiddenQuest);//, pclMsg->bPCRoomQuest );

	// 퀘스트 대상자가 존재하는 경우 
	if ( pclCM->IsValidID(id) && pclCM->CR[id]->pclCI->GetPersonID() == pclMsg->siPersonID )
	{		
		//pclDailyQuestMgr->Set( pclMsg->siMonsterKind, pclMsg->siMonsterNum, pclMsg->siRewardItemUnique, pclMsg->siRewardItemNum, pclMsg->siRewardExp, pclMsg->siFee, pclMsg->siLeftTime, 0 );
		pclCM->CR[id]->pclCI->clDailyQuest3Info.Set( pclMsg->siItemUnique, pclMsg->siItemNum, pclMsg->siRewardItemUnique, pclMsg->siRewardItemNum,pclMsg->siRewardExp, pclMsg->siFee, pclMsg->siQuestTime, pclMsg->siLeftTime, 0, pclMsg->bMoneySelected, pclMsg->bHiddenQuest );
		pclCM->CR[id]->pclCI->clIP.SetMoney( pclMsg->siLeftMoney );
	
		cltGameMsgResponse_DailyQuest3Select clinfo( &pclCM->CR[id]->pclCI->clDailyQuest3Info, pclMsg->siLeftMoney );
		cltMsg clMsg(GAMEMSG_RESPONSE_DAILYQUEST3SELECT, sizeof(clinfo), (BYTE*)&clinfo);
		pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);	
	}

	return;
}

void cltServer::DoMsg_DBMSG_RESPONSE_DAILYQUEST3DEL( sPacketHeader* pPacket)
{

	sDBResponse_DailyQuest3Del * pclMsg = (sDBResponse_DailyQuest3Del*)pPacket;

	if ( pclMsg->siResult != 1 )
	{
		return;
	}
	
	SI32 id = pclMsg->siCharID;

	if ( pclMsg->siPersonID < 1 || pclMsg->siCharID < 1 )
	{
		return;
	}

	// 지웠음을 유저에게 알려줘야 하므로...
	if ( pclCM->IsValidID(id) && pclCM->CR[id]->pclCI->GetPersonID() == pclMsg->siPersonID )
	{
		if ( pclMsg->uiSuccess )
		{
			cltCharServer *pclChar = (cltCharServer *)pclCM->CR[id];

			if ( pclMsg->siAddMoney  )
			{
				// 돈 업데이트
				pclChar->pclCI->clIP.IncreaseMoney(pclMsg->siAddMoney);
			}
			else
			{
				if( pclMsg->siInventoryItemIndex >= 0 && pclMsg->siInventoryItemIndex < MAX_ITEM_PER_PERSON )
					pclChar->pclCI->clCharItem.clItem[pclMsg->siInventoryItemIndex].Set(&pclMsg->clInventoryItem);
			}
			//만든 아이템 종류의 스킬레벨 업.
			SI32 ref = pclItemManager->FindItemRefFromUnique(pclMsg->siItemUnique);
			SI32 skillunique = pclItemManager->pclItemInfo[ref]->clSkillInfo.siSkill;
			pclChar->SetSkillUpdate(skillunique,pclMsg->siExp);			
			//// 경험치 업데이트
			//pclChar->IncreaseExp( pclMsg->siExp );					
			//길드 포인트 상승.
			SI32 addGP = 0;
			SI32 guildUnique = pclChar->pclCI->clBI.clGuildRank.GetGuildUnitIndex();
			// 히든 퀘스트 완성,.
			if ( pclMsg->bHiddenQuest == true )
			{
				if (guildUnique > 0) 
				{
					addGP = (pclMsg->siExp*25 / 100) * 2;
					if ( addGP > 0 )
					{	//상자를 주고 길드 포인트도 업
						pclChar->GiveItem(ITEMUNIQUE(13321),1, INPUTITEMPERSON_OPTION_QUEST,0);
						sDBRequest_Guild_ChangeGuildPoint clMsg(guildUnique, pclChar->pclCI->GetPersonID(), addGP, TYPE_GUILDPOINT_DAILYQUEST2 );
						pclClient->SendDBMsg( DBSELECT_BASE, (sPacketHeader*)&clMsg );
					}
					else{
					}
				}
				else
					{
						//길드가 없으니 길드포인트는 주지 않고 상자만 준다
						pclChar->GiveItem(ITEMUNIQUE(13321),1, INPUTITEMPERSON_OPTION_QUEST,0);

					}
			}
			// 히든 퀘스트가 아니라면 길드 포인트만 준다.
			// 길드 가 없으면 그냥 패스
			else{
				if (guildUnique > 0) {
					addGP = pclMsg->siExp* 25 / 100;
					if ( addGP > 0 )
					{
						sDBRequest_Guild_ChangeGuildPoint clMsg(guildUnique, pclChar->pclCI->GetPersonID(), addGP, TYPE_GUILDPOINT_DAILYQUEST2 );
						pclClient->SendDBMsg( DBSELECT_BASE, (sPacketHeader*)&clMsg );
					}
				}				
				else
				{

				}

			}
			// 아이템 업데이트		
			cltGameMsgResponse_DailyQuest3End clinfo1( pclMsg->uiSuccess, pclChar->pclCI->clIP.GetCurExp(), // SI32->SI64 경험치 오버플로우때문에 수정
				pclMsg->siInventoryItemIndex, pclMsg->siChangedItemNum, &pclMsg->clInventoryItem, 
				pclMsg->siAddMoney, pclMsg->siExp,addGP,pclMsg->bHiddenQuest); 
			cltMsg clMsg1(GAMEMSG_RESPONSE_DAILYQUEST3END, sizeof(clinfo1), (BYTE*)&clinfo1);
			pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg1);

			pclCM->CR[id]->pclCI->clDailyQuest3Info.Init();//데일리 퀘스트 정보를 지워 버린다.

			// 모병관 퀘스트 클리어통계. 
			//pclStatisticsManager->clDailyStatistics.siDailyQuestClearNum++;
		}
		else
		{
			cltItem clItem;
			clItem.Init();
			cltGameMsgResponse_DailyQuest3End clinfo1( pclMsg->uiSuccess, pclCM->CR[id]->pclCI->clIP.GetCurExp(),// SI32->SI64 경험치 오버플로우때문에 수정
				0, 0, &clItem, 0,0,0,false );
			cltMsg clMsg1(GAMEMSG_RESPONSE_DAILYQUEST3END, sizeof(clinfo1), (BYTE*)&clinfo1);
			pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg1);
			pclCM->CR[id]->pclCI->clDailyQuest3Info.Init();//데일리 퀘스트 정보를 지워 버린다.
		}

		if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
		{
			// [로그추가 : 황진성 2007. 10. 8] // 모병관 종료.
			// param1 == 성공 여부.
			// param2 == 아이템 유니크.
			// param3 == 상으로 주는 경험치.
			cltCharServer* pclchar = NULL;
			if(pclCM->IsValidID(id))		pclchar = (cltCharServer*)pclCM->CR[id];

			if(pclchar)
			{
				// PCK - 아이템 받을때 남기는 로그 추가함.
				SendLog_WriteLogToDB(LOGCOMMAND_TYPE_PERSONITEM, LOGCOMMAND_INDEX_PERSONITEM_GET_ITEM,
									INPUTITEMPERSON_OPTION_DAILYQUEST3, pclchar, NULL, 0, 0, 0, &pclMsg->clRewardItem, 0, 0, 0, 0, 0, 
									pclMsg->siInventoryItemIndex, 0, 0, 0, 0, 0, NULL, NULL);

				SendLog_WriteLogToDB(LOGCOMMAND_TYPE_QUEST, LOGCOMMAND_INDEX_QUEST_DAILY_QUEST3_END, 
									0, pclchar, NULL, 0, pclMsg->siAddMoney, pclchar->pclCI->clIP.GetMoney(), NULL, 0, 0, 0, 0, 0, 0, 
									pclMsg->uiSuccess, pclMsg->clRewardItem.siUnique, pclMsg->siExp, 0, 0, NULL, NULL);
			}
		}

		// 퀘스트가 종료되었으므로, 초기화 해 줘야 함.
		pclCM->CR[id]->pclCI->clDailyQuest3Info.Init();
	}

	return;
}

void cltServer::DoMsg_DBMSG_RESPONSE_DAILYQUEST3CHANGE( sPacketHeader* pPacket)
{
	sDBResponse_DailyQuest3Change * pclMsg = (sDBResponse_DailyQuest3Change*)pPacket;

	SI32 id = pclMsg->siCharID;

	if ( pclMsg->siResult == 1 )
	{
		if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
		{
			// [로그추가 : 황진성 2007. 10. 8] // 잭(모병관) 퀘스트 변경.
			// param1 == 남은시간. 
			// param2 == 만든 아이템. 

			cltCharServer* pclchar = NULL;
			if(pclCM->IsValidID(id))		pclchar = (cltCharServer*)pclCM->CR[id];

			SendLog_WriteLogToDB(LOGCOMMAND_TYPE_QUEST, LOGCOMMAND_INDEX_QUEST_DAILY_QUEST3_CHANGE, 
								0, pclchar, NULL, 0, 0, 0, NULL, 0, 0, 0, 0, 0, 0, 
								pclMsg->siLeftTime, pclMsg->siMakedItem, 0, 0, 0, NULL, NULL);
		}
	}

	if ( pclMsg->siResult != 1 )
	{
		return;
	}

	if ( pclMsg->siPersonID < 1 || pclMsg->siCharID < 1 )
	{
		return;
	}

	if ( pclCM->IsValidID(id) && pclCM->CR[id]->pclCI->GetPersonID() == pclMsg->siPersonID )
	{
		pclCM->CR[id]->pclCI->clDailyQuest3Info.siMadeItemNum = pclMsg->siMakedItem;
		pclCM->CR[id]->pclCI->clDailyQuest3Info.siLeftTime =	pclMsg->siLeftTime;
		pclCM->CR[id]->pclCI->clDailyQuest3Info.siLeftTime =	pclMsg->bHiddenQuest;
		
		cltGameMsgResponse_DailyQuest3Change clinfo( pclMsg->siLeftTime, pclMsg->siMakedItem, pclMsg->bHiddenQuest );
		cltMsg clMsg(GAMEMSG_RESPONSE_DAILYQUEST3CHANGE, sizeof(clinfo), (BYTE*)&clinfo);
		pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);	
	}

	return;
}
