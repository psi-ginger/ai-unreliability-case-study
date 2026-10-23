//---------------------------------
//
//---------------------------------

#include "../Common/CommonHeader.h"
#include "../Server/Server.h"

#include "../../Lib/HashTable/HashTable_StringInt.h"

#include "../../Common/Map/MapManager/MapManager.h"
#include "Char/CharManager/CharManager.h"


#include "MsgRval-Define.h"
#include "MsgType-Person.h"
#include "Msg/MsgType-Summon.h"
#include "../../../DBManager/GameDBManager_World/DBMsg-Item.h"
#include "../../../DBManager/GameDBManager_World/DBMsg-Person.h"
#include "../../../DBManager/GameDBManager_World/DBMsg-Summon.h"
#include "Statistics/Statistics.h"

#include "Minister/MinisterBase.h"
#include "Minister/Minister-Izo.h"
#include "Minister/MinisterMgr.h"
#include "../common/Event/event.h"
#include "../common/PersonInfo.h"

void cltServer::DoMsg_GAMEMSG_REQUEST_SUMMONFREE(cltMsg* pclMsg, SI32 id)
{
	//---------------------------------------------
	// 
	//---------------------------------------------
	cltGameMsgRequest_SummonFree* pclinfo = (cltGameMsgRequest_SummonFree*)pclMsg->cData;
	SI32 charid = pclCM->GetIDFromCharUnique(pclinfo->siCharUnique);
	if(pclCM->IsValidID(charid)  == false)	return;

	cltCharServer* pclchar = (cltCharServer*)pclCM->CR[charid];
	if(pclchar==NULL)	return;

	cltSummon *pclSummon = pclchar->pclCI->clSummonInfo.GetSummonFromSummonID(pclinfo->siSummonID);
	if(pclSummon==NULL)	return;

	SI32 siSummonIndex = pclchar->pclCI->clSummonInfo.GetSummonIndexFromSummonID(pclinfo->siSummonID);

	//--------------------------------
	// 해제할 소환수의 값을 점검한다.
	//--------------------------------
	if(pclSummon->siKind == 0) return;
		
	//---------------------------------------------
	// 예약 되어 있는 아이템 사용이 있다면 처리한다. 
	//---------------------------------------------
	pclchar->ItemUseReserveAction();

	//---------------------------------------------
	// 예약 되어 있는 탄환아이템 사용이 있다면 처리한다. 
	//---------------------------------------------
	pclchar->BulletUseReserveAction();
	
	//---------------------------------------------
	// 장비를 장착한 소환수는 풀어줄 수 없다.
	//---------------------------------------------
	if ( pclchar->pclCI->clSummonInfo.IsHaveEquipItem(siSummonIndex) )
	{
		SendServerResponseMsg(0, SRVAL_SUMMON_FREE_FAIL_HAVEITEM, 0, 0, pclCM->CR[id]->GetCharUnique() );
		return;
	}
	
	
	//---------------------------------------------
	// pclinfo->siSummonIndex 를 free 시켜야한다.
	//---------------------------------------------
	// DB에 소환수를 설정한다.
	pclchar->SendDBSummon(SETSUMMON_REASON_FREE, siSummonIndex);
}

void cltServer::DoMsg_GAMEMSG_REQUEST_SUMMONNAMECHANGE(cltMsg* pclMsg, SI32 id)
{
	//---------------------------------------------
	// 
	//---------------------------------------------
	cltGameMsgRequest_SummonNameChange* pclinfo = (cltGameMsgRequest_SummonNameChange*)pclMsg->cData;
	SI32 charid = pclCM->GetIDFromCharUnique(pclinfo->siCharUnique);
	if(pclCM->IsValidID(charid)  == false)	return;

	cltCharServer* pclchar = (cltCharServer*)pclCM->CR[charid];
	if(pclchar==NULL)	return;

	cltSummon *pclSummon = pclchar->pclCI->clSummonInfo.GetSummonFromSummonID(pclinfo->siSummonID);
	if(pclSummon==NULL)	return;
	
	//--------------------------------
	// 소환수의 값을 점검한다.
	//--------------------------------
	if(pclSummon->siKind == 0) return;
		
	// 소환수 이름을 변경시킨다.
	MStrCpy(pclSummon->szName, pclinfo->strSummonName, MAX_SUMMON_NAME);

	SI32 siSummonIndex = pclchar->pclCI->clSummonInfo.GetSummonIndexFromSummonID(pclinfo->siSummonID);
	// DB에 소환수를 설정한다.
	pclchar->SendDBSummon(SETSUMMON_REASON_UPDATE, siSummonIndex);

	cltGameMsgResponse_SummonNameChange clInfo(pclchar->GetCharUnique(), pclSummon->siSummonID, pclSummon->szName);
	cltMsg clMsg(GAMEMSG_RESPONSE_SUMMONNAMECHANGE, sizeof(clInfo), (BYTE*)&clInfo);
	((cltCharServer*)pclCM->CR[charid])->SendNeighbourMsg(&clMsg, true );

	cltSimpleSummon *pclSimpleSummon = pclchar->pclCI->clSummonInfo.GetSimpleSummonFromSummonID(pclSummon->siSummonID);
	SI32 siSlotIndex = pclchar->pclCI->clSummonInfo.GetSummonBaseIndexFromSummonID(pclSummon->siSummonID);
	if(pclSimpleSummon)
	{
		StringCchCopy(pclSimpleSummon->szName, sizeof(pclSimpleSummon->szName), pclSummon->szName);

		cltGameMsgResponse_BaseSummon clInfo(pclchar->GetCharUnique(), siSlotIndex, pclSimpleSummon);
		cltMsg clMsg(GAMEMSG_RESPONSE_BASESUMMONINFO, sizeof(clInfo), (BYTE*)&clInfo);
		pclchar->SendNetMsg((sPacketHeader*)&clMsg);
	}
}

void cltServer::DoMsg_GAMEMSG_REQUEST_SUMMONREVIVE(cltMsg* pclMsg, SI32 id)
{
	if(pclCM->IsValidID(id)  == false)	return;

	//---------------------------------------------
	//
	//---------------------------------------------
	cltGameMsgRequest_SummonRevive* pclinfo = (cltGameMsgRequest_SummonRevive*)pclMsg->cData;
	SI32 charid = pclCM->GetIDFromCharUnique(pclinfo->siCharUnique);
	if(pclCM->IsValidID(charid)  == false)	return;

	cltCharServer* pclchar = (cltCharServer*)pclCM->CR[charid];
	if(pclchar==NULL)	return;

	cltSummon *pclSummon = pclchar->pclCI->clSummonInfo.GetSummonFromSummonID(pclinfo->siSummonID);
	if(pclSummon==NULL)	return;

	SI32 siSummonIndex = pclchar->pclCI->clSummonInfo.GetSummonIndexFromSummonID(pclinfo->siSummonID);
	
	//---------------------------------------------
	// 소환수가 죽어있지 않으면 부활시킬수 없다.
	//---------------------------------------------
	if(pclSummon->siStatus != SUMMON_STATUS_DEAD) return;

	//--------------------------------
	// 부활시킬 소환수의 Kind를 점검한다.
	//--------------------------------
	SI32 kind = pclSummon->siKind;
	if(kind <= 0 || kind >= MAX_KIND_NUMBER)	return;
	
	//cltKindInfo* pclki = pclKindInfoSet->pclKI[kind];
	cltKindInfo* pclki = NULL;
	cltGMCommandKindInfo* pclGMki = NULL;

	if(IsCountrySwitch(Switch_Summon_Separate)) 
	{
		pclki = pclKindInfoSet->pclSummonKI[kind];
		pclGMki = pclKindInfoSet->pclGMCommandKindInfo[kind];
	}
	else
	{
		pclki = pclKindInfoSet->pclKI[kind];
		pclGMki = pclKindInfoSet->pclGMCommandKindInfo[kind];
	}

	if(pclki == NULL)							return;
	if(pclki->siSummonRank <= 0)				return;
	if(pclGMki == NULL)							return;

	//--------------------------------
	// 부활시키기위한 준비물 점검
	//--------------------------------
	SI32 siSuccess	= 1;
	SI32 siDollPos	= 0;
	SI32 siDollUnique	= 0;
	SI32 siDollNum		= 0;

	if(pclinfo->siReviveType == SUMMON_REVIVE_DOLL)
	{
		siDollUnique= pclItemManager->GetItemDollUnique(kind);
		siDollNum	=  pclSummon->clIP.GetLevel();
		if(siDollUnique <= 0)		return;
		if(siDollNum	<= 0)		return;
		if(pclki->bSummonMall == TRUE)				return; // 소환수가 아이템몰 소환수 일경우에는 부활할수 없다.
	}
	else if(pclinfo->siReviveType == SUMMON_REVIVE_TICKET)
	{
		siDollUnique			= ITEMUNIQUE(24020);	// 소환수부활이용권  14060 -> 24020
		siDollNum	=  1;
		if(siDollUnique <= 0)		return;
		if(siDollNum	<= 0)		return;
	}
	else
	{
		return;
	}

	///////////////////////////////////////////////////////
	// 소환인형 확인 
	siDollPos = pclCM->CR[charid]->pclCI->clCharItem.GetItemPos(siDollUnique);
	
	if(siDollPos < PERSONITEM_INV0 || siDollPos >= MAX_ITEM_PER_PERSON)	
	{
		siDollUnique = ITEMUNIQUE(24130); // 기존의 소환부활이용권을 소지하고 있지 않다면 이벤트용으로 가지고 있는지 확인.

		siDollPos = pclCM->CR[charid]->pclCI->clCharItem.GetItemPos(siDollUnique);

		if(siDollPos < PERSONITEM_INV0 || siDollPos >= MAX_ITEM_PER_PERSON)
		{
			siSuccess = -1;
		}
	}
	
	if(siSuccess > 0 && pclCM->CR[id]->pclCI->clCharItem.clItem[siDollPos].siItemNum < siDollNum)	siSuccess = -1;
	//////////////////////////////////////////////////////////////////////////
	
	if(siSuccess > 0)
	{
		// 물품 사용 통계를 낸다.
		pclItemManager->AddUseNum(siDollUnique, siDollNum);

		//새로운 통계 - 물품 사용 통계를 낸다.
		//NStatistics_Add("Item", siDollUnique, "UseNum", siDollNum);

	    
		
		// 소환수부활이용권을 사용한 것이라면, 
		if(pclinfo->siReviveType == SUMMON_REVIVE_TICKET)
		{
			// 소환수 티켓으로 살리기 퀘스트중이라면, 
			pclCM->CR[id]->DoQuestCheckAndAction(QUEST_CLEAR_COND_REVIVESUMMON);
		}
		else if( pclinfo->siReviveType == SUMMON_REVIVE_DOLL )
		{
			// 소환수 인형으로 살리기 퀘스트중이라면, 
			pclCM->CR[id]->DoQuestCheckAndAction(QUEST_CLEAR_COND_REVIVESUMMONBYDOLL);
		}

		// 소환수를 소환한 마을의 Unique를 준다.
		SI32 villageunique  = pclCM->CR[charid]->GetCurrentVillageUnique();

		//---------------------------------------------
		// 소환수를 부활시키기 위한 기본적인 LIFE, MANA만 설정한다.
		//---------------------------------------------
		cltSummon clSummon;
		clSummon.Set(pclSummon);
		
		cltPIPInfo			clipinfo2;
		clipinfo2.Set(&pclSummon->clIP);
		
		cltItem clDollItem;
		SI16 brareswitch = 0;
		pclClient->pclItemManager->MakeRandItemUnique( siDollUnique, &clDollItem, 0, 0, &brareswitch, 0,0,0,0,0,0);
		clDollItem.SetItemNum(siDollNum);

		cltParametaBox clPB;
		clPB.Init(pclItemManager, &clipinfo2, NULL, pclki, pclGMki);
		clPB.Action(NULL, 0, PERSONITEM_WEAPON1, NULL, NULL, NULL, NULL, NULL, NULL, NULL, GAMEMODE_SERVER, true);
		
		clSummon.clIP.SetLife(clPB.GetMaxLife());	// 기본적인 것만 저장한다.( 사실 PBBOX 를 사용해서 풀로 채워야한다)
		clSummon.clIP.SetMana(clPB.GetMaxMana());

		if(pclCM->CR[charid]->GetSummonID())	clSummon.siStatus = SUMMON_STATUS_TAKEIN;
		else									clSummon.siStatus = SUMMON_STATUS_TAKEOUT;

		// DB로 요청한다. 
		sDBRequest_SummonRevive clMsg(charid, pclCM->CR[charid]->pclCI->GetPersonID(), villageunique, siDollPos, siDollUnique, siDollNum, 
									  &clSummon, &clDollItem);
		pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);
	}
	else
	{
		// 설정 정보를 설정한다.
		cltGameMsgResponse_SummonRevive clinfo(pclchar->GetCharUnique(), siSuccess, pclinfo->siSummonID);
		cltMsg clMsg(GAMEMSG_RESPONSE_SUMMONREVIVE , sizeof(clinfo), (BYTE*)&clinfo);
		pclCM->CR[charid]->SendNetMsg((sPacketHeader*)&clMsg);
	}
	
}
// Base 소환수를 slot에 넣는다.
void cltServer::DoMsg_GAMEMSG_REQUEST_SUMMONSLOTIN(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgRequest_SlotIn* pclinfo = (cltGameMsgRequest_SlotIn*)pclMsg->cData;
	// 소환수와 케릭터 정보 검증
	if(pclCM->IsValidID(id)  == false)						return;
	cltCharServer* pclchar = (cltCharServer*)pclCM->CR[id];
	if(pclchar==NULL)										return;
	cltSummonInfo *pclSummonInfo = &pclchar->pclCI->clSummonInfo ;
	if(pclSummonInfo==NULL)									return;

	// 뺄려는 소환수가 현재 나와있는지 체크한다.
	cltSummon* pclSummon = pclSummonInfo->GetSummonFromSummonID(pclinfo->siSummonID);	
	if(pclSummon)											return;

	// 일단 검증이 필요 하겠지 몇번째 해당 서먼아이드로 어떤 인덱스에 있는지 그리고 그 인덱스의 서먼아이디가 맞는지
	for ( SI08 sislotIndex = 0 ; sislotIndex < MAX_HAVE_SUMMON ; sislotIndex++) {
		// 해당 인덱스의 서먼 아이디가 같으면 헤당 Base의 인덱스와 낑구고자 하는 슬롯, SummonId 를 토대로 디비에 해당 소환수를 슬롯에 넣어 달라고 보낸다
		 if( pclinfo->siSummonID == pclSummonInfo->clSummonBase[ sislotIndex ].siSummonID )
		 {	
			 if ( sislotIndex > 2 && 
				 pclchar->pclCI->clBI.bSummonExtraSlot == false &&	

				 pclchar->pclCI->clBI.siSummonExtraSlotDate < pclTime->clDate.GetDateVary()) 
			 {
				 // 클라이언트에 사실을 알린다. 
				 ((cltServer *)pclClient)->SendServerResponseMsg(0, SRVAL_SUMMON_FAIL_OVERDATE,  0, 0, pclchar->GetCharUnique());
				 return;
			 }
			 else
			 {
				 // base의 소환수를 Slot에 넣는다.
				 sDBRequest_SummonSlotIn clMsg(id, pclCM->CR[id]->pclCI->GetPersonID(),pclinfo->siSlotIndex, pclinfo->siSummonID );
				 pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);
			 }
			
		 }
	}
}
// 슬롯의 소환수를 빼낸다.
void cltServer::DoMsg_GAMEMSG_REQUEST_SUMMONSLOTOUT(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgRequest_SlotOut* pclinfo = (cltGameMsgRequest_SlotOut*)pclMsg->cData;
	// 소환수와 케릭터 정보 검증
	cltCharServer* pclchar = (cltCharServer*)pclCM->CR[id];
	if(pclchar==NULL)										return;
	cltSummonInfo *pclSummonInfo = &pclchar->pclCI->clSummonInfo ;
	if(pclSummonInfo==NULL)									return;

	SI32 SummonId	=	pclinfo->siSummonID;
	SI32 SlotIndex	=	pclinfo->siSummonIndex;

	cltSummon* pclSummon = pclSummonInfo->GetSummon(SlotIndex);
	if(pclSummon == NULL || pclSummon->siSummonID != SummonId)		return;

	SI32 siSummonCharID = pclchar->GetSummonID();

	if( pclCM->IsValidID(siSummonCharID) )
	{
		if( pclCM->IsAlive(siSummonCharID) == FALSE )
		{
			// 죽은 소환수는 저장할 수 없다.
			SendServerResponseMsg(0, SRVAL_SUMMON_FAIL_CANNOTSLOTOUTDEAD, 0, 0, pclCM->CR[id]->GetCharUnique() );
			return;
		}
	}
	if( pclSummon->siStatus == SUMMON_STATUS_DEAD )
	{
		// 죽은 소환수는 저장할 수 없다.
		SendServerResponseMsg(0, SRVAL_SUMMON_FAIL_CANNOTSLOTOUTDEAD, 0, 0, pclCM->CR[id]->GetCharUnique() );
		return;
	}

	pclSummon->siStatus = SUMMON_STATUS_TAKEIN;
	pclchar->SendDBSummon(SETSUMMON_REASON_UPDATE, SlotIndex);

	// 해당 인덱스에 해당 서먼아이디의 소환수가 있나 확인
	if( pclSummonInfo->clSummon[ SlotIndex ].siSummonID == SummonId ){
		// 디비에 해당 소환수를 summonindex 에서 빼달라고 한다.
		sDBRequest_SummonSlotOut clMsg(id, pclCM->CR[id]->pclCI->GetPersonID(), pclinfo->siSummonID ,SlotIndex);
		pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);
	}
}

// 소환수에 무기 장착석을 박는다,.
void cltServer::DoMsg_GAMEMSG_REQUEST_SUMMONUSETON(cltMsg* pclMsg, SI32 id){
	
	cltGameMsgRequest_SummonEquipston* pclinfo = (cltGameMsgRequest_SummonEquipston*)pclMsg->cData;
	SI16	Weaponitemunique = 0;
	SI16	Armoritemunique	= 0 ;	

	// 소환수와 케릭터 정보 검증
	if(pclCM->IsValidID(id)  == false)	return;

	cltCharServer* pclchar = (cltCharServer*)pclCM->CR[id];
	if(pclchar==NULL)						return;

	cltSummon *pclSummon = pclchar->pclCI->clSummonInfo.GetSummonFromSummonID(pclinfo->siSummonID);
	if(pclSummon==NULL)						return;

	SI32 siSummonIndex = pclchar->pclCI->clSummonInfo.GetSummonIndexFromSummonID(pclinfo->siSummonID);

	// 나와있는 소환수만 장착석을 박을 수 있다.
	if(pclSummon->siStatus != SUMMON_STATUS_TAKEOUT)	return;

	if(pclinfo->siStonNum <= 0){
		// 값이 없다는 메세지 날려준다.
		SendServerResponseMsg(0, SRVAL_SUMMON_FREE_FAIL_STONNUMINCORRECT, 0, 0, pclCM->CR[id]->GetCharUnique() );
		return;
	}

	switch(pclinfo->siStonType)
	{
	case STON_WEAPONE:
		{
			if (pclSummon->clIP.GetLevel() < pclSummon->siAttackWearStoneNum + pclinfo->siStonNum)
			{
				// 최대 레벨 이상으로 충전 할수 없습니다
				SendServerResponseMsg(0, SRVAL_SUMMON_FREE_FAIL_CANNOTOVERUSESTON, 0, 0, pclCM->CR[id]->GetCharUnique() );
				return;
			}
		}
		break;
	case STON_ARMOR:
		{
			if (pclSummon->clIP.GetLevel() < pclSummon->siDefenseWearStoneNum + pclinfo->siStonNum){
				// 최대 레벨 이상으로 충전 할수 없습니다
				SendServerResponseMsg(0, SRVAL_SUMMON_FREE_FAIL_CANNOTOVERUSESTON, 0, 0, pclCM->CR[id]->GetCharUnique() );
				return;
			}
		}
		break;
	}

	SI32 siStoneItemUnique = 0;

	switch(pclinfo->siStonType)
	{
	case STON_WEAPONE:		siStoneItemUnique = ITEMUNIQUE(5414);		break;
	case STON_ARMOR:		siStoneItemUnique = ITEMUNIQUE(5415);		break;
	}

	if( siStoneItemUnique <= 0 )					return;

	for( SI16 invennum = PERSONITEM_INV0; invennum < PERSONITEM_INV0 + MAX_INVITEM_PER_PERSON; invennum++ )
	{
		cltItem* pclItem = &pclCM->CR[id]->pclCI->clCharItem.clItem[invennum];
		if(pclItem == NULL)			continue ;
		
		if ( pclItem->siUnique == siStoneItemUnique ) 
		{
			// 아이템을 찾고 갯수를 파악해서 가지고 있는 아이템이 더 많거나 같을때만 소환수에 장착할수 있도록 한다.
			if(pclItem->GetItemNum() >= pclinfo->siStonNum)
			{
				cltItem Useitem;
				Useitem.Set(&pclCM->CR[id]->pclCI->clCharItem.clItem[invennum]);
				Useitem.siItemNum = pclinfo->siStonNum;

				sDBRequest_UseItem useitem(id, pclCM->CR[id]->pclCI->GetPersonID(), invennum, &Useitem, USEITEM_REASON_ENCHANT_MATERIAL);
				pclGameDBProcess->SendMsg(DBSELECT_BASE,(sPacketHeader *)&useitem);

				// DB에 보낼 소환수를 설정 한다
				cltSummon clSummon;
				clSummon.Set(pclSummon);

				switch(pclinfo->siStonType)
				{
				case STON_WEAPONE:		clSummon.siAttackWearStoneNum += pclinfo->siStonNum;		break;
				case STON_ARMOR:		clSummon.siDefenseWearStoneNum += pclinfo->siStonNum;		break;
				}

				// 조건이 완성되었으므로 DB에 소환수를 보낸다
				pclchar->SendDBSummon(SETSUMMON_REASON_SUMMONSTON, siSummonIndex, &clSummon);			
			}
			else
			{
				// 장비 석이 충분 하지 않다.
				SendServerResponseMsg(0, SRVAL_SUMMON_FREE_FAIL_STONNUMNOTENOUGHSTON, 0, 0, pclCM->CR[id]->GetCharUnique() );
				return;
			}
		}
	}
}

// [추가 : 황진성 2008. 2. 20 => 이전 소환수 능력치 변경적용하는 부분을 함수로 뺌.]
void cltServer::Use_Summon_Bonus(cltGameMsgRequest_SummonUseBonusPoint* pclinfo, SI32 id)
{
	// 주인과 소환수가 같이 있어야한다.
	if(pclCM->IsValidID(id)  == false)			return;

	//---------------------------------------------
	//
	//---------------------------------------------
	SI32 charid = pclCM->GetIDFromCharUnique(pclinfo->siCharUnique);
	if(pclCM->IsValidID(charid)  == false)	return;

	cltCharServer* pclchar = (cltCharServer*)pclCM->CR[charid];
	if(pclchar==NULL)	return;

	cltSummon *pclSummon = pclchar->pclCI->clSummonInfo.GetSummonFromSummonID(pclinfo->siSummonID);
	if(pclSummon==NULL)	return;
	//---------------------------------------------
	SI32 siSummonIndex = pclchar->pclCI->clSummonInfo.GetSummonIndexFromSummonID(pclinfo->siSummonID);

	SI32 updatereason = SETSUMMON_REASON_UPDATE ;

	SI32 siSummonID = pclchar->GetSummonID();
	if(pclCM->IsValidID(siSummonID )  == true)
	{
		// 소환수 자신이 갖고 있는 정보와 내가 갖고 있는 정보가 다르면 안된다.
		if(pclCM->CR[siSummonID]->GetSummonIndex() == siSummonIndex) 
		{
			if ( pclinfo->bStatusInit == false )
			{
				// 캐릭터가 살아 있어야하고 보너스가 남아 있어야 쓸 수 있다.
				if(pclCM->CR[siSummonID]->pclCI->clIP.GetBonus() >0)
				{
					switch(pclinfo->siBonusType)
					{
					case 0:
						pclCM->CR[siSummonID]->pclCI->clIP.IncreaseStr(1);
						pclCM->CR[siSummonID]->pclCI->clIP.DecreaseBonus(1);
						break;
					case 1:
						pclCM->CR[siSummonID]->pclCI->clIP.IncreaseDex(1);
						pclCM->CR[siSummonID]->pclCI->clIP.DecreaseBonus(1);
						break;
					case 2:
						pclCM->CR[siSummonID]->pclCI->clIP.IncreaseMag(1);
						pclCM->CR[siSummonID]->pclCI->clIP.DecreaseBonus(1);
						break;
					case 3:
						pclCM->CR[siSummonID]->pclCI->clIP.IncreaseVit(1);
						pclCM->CR[siSummonID]->pclCI->clIP.DecreaseBonus(1);
						break;
					default:
						return ;
					}
				}
			}
			else
			{
				cltKindInfo* pclki = NULL;
				if( pclSummon->siKind >= 0 && pclSummon->siKind < MAX_KIND_NUMBER )
				{
					//pclki = pclKindInfoSet->pclKI[pclSummon->siKind];
					if(IsCountrySwitch(Switch_Summon_Separate)) 
					{
						pclki = pclKindInfoSet->pclSummonKI[pclSummon->siKind];
					}
					else
					{
						pclki = pclKindInfoSet->pclKI[pclSummon->siKind];
					}
				}

				if(pclki == NULL)							return;

				switch(pclinfo->siBonusType)
				{
				case IDC_BUTTON_STR_INIT:	if(pclCM->CR[siSummonID]->pclCI->clIP.GetStr() <= pclki->clBA.GetStr())return ;	break;
				case IDC_BUTTON_DEX_INIT:	if(pclCM->CR[siSummonID]->pclCI->clIP.GetDex() <= pclki->clBA.GetDex())return ;	break;
				case IDC_BUTTON_INT_INIT:	if(pclCM->CR[siSummonID]->pclCI->clIP.GetMag() <= pclki->clBA.GetMag())return ;	break;
				case IDC_BUTTON_VIT_INIT:	if(pclCM->CR[siSummonID]->pclCI->clIP.GetVit() <= pclki->clBA.GetVit())return ;	break;
				default: return ;
				}

				// 상태부분초기화이용권이 존재하는지 확인한다. 
				//SI32 itempos = pclCM->CR[id]->pclCI->clCharItem.FindItemInv(14095);
				SI32 itempos = pclCM->CR[id]->pclCI->clCharItem.FindItemInv(ITEMUNIQUE(24035));
				if(itempos < 0)
				{
					return ;
				}

				// 상태부분초기화이용권을 소모한다. 
				cltItem clItem;
				clItem.Set(&pclCM->CR[id]->pclCI->clCharItem.clItem[itempos]);
				clItem.SetItemNum(1);

				sDBRequest_UseItem clMsg( id, pclCM->CR[id]->pclCI->GetPersonID(), itempos, &clItem,USEITEM_REASON_TICKET);
				pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);


				// 스텟을 빼서 보너스로 넣는다. 
				switch(pclinfo->siBonusType)
				{
				case IDC_BUTTON_STR_INIT:	
					pclCM->CR[siSummonID]->pclCI->clIP.DecreaseStr(1);
					pclCM->CR[siSummonID]->pclCI->clIP.IncreaseBonus(1);
					break;

				case IDC_BUTTON_DEX_INIT:
					pclCM->CR[siSummonID]->pclCI->clIP.DecreaseDex(1);
					pclCM->CR[siSummonID]->pclCI->clIP.IncreaseBonus(1);
					break;

				case IDC_BUTTON_INT_INIT:
					pclCM->CR[siSummonID]->pclCI->clIP.DecreaseMag(1);
					pclCM->CR[siSummonID]->pclCI->clIP.IncreaseBonus(1);
					break;

				case IDC_BUTTON_VIT_INIT:
					pclCM->CR[siSummonID]->pclCI->clIP.DecreaseVit(1);
					pclCM->CR[siSummonID]->pclCI->clIP.IncreaseBonus(1);
					break;

				default: return ;
				}

				SI32 updatereason = SETSUMMON_REASON_PARTINIT ;
			}
		}
		pclCM->CR[siSummonID]->SetUpdateSwitch(UPDATE_IP,   true, 0);
	}

	// 캐릭터의 IP를 클라이언트로 보낼 수 있게 한다. 
	pclCM->CR[id]->SetUpdateSwitch(UPDATE_SUMMON,		true, 0);

	//---------------------------------------------
	// DB에 소환수를 설정한다.
	//---------------------------------------------
	pclchar->SendDBSummon(updatereason, siSummonIndex);

	// 버그 수정 : 박기형 2009.06.19
	// 소환수의 스테이터스는 파라미터 값을 계산 해서 그에 따라 결정되는 값들을 수정한다. 
	// 서버에서 주기적으로 실행하시만
	// 그 시간 동안 스테이터스값이 변경 되지 않아 원치 않는 결과 발생
	// 예) 최대 체력치가 증가되지 않아 클라이언트의 hp증가되지 않는다.
	// 캐릭터 매니저의 소환수
	cltCharServer* pclSummonChar = (cltCharServer*) pclCM->CR[siSummonID];
	if(pclSummonChar != NULL)
		pclSummonChar->ParametaBoxAction();
}

// [추가 : 황진성 2008. 2. 20 => 새로운 소환수 능력치 변경적용하는 부분을 함수로 뺌.]
void cltServer::New_Use_Summon_Bonus(cltGameMsgRequest_SummonUseBonus* pclinfo, SI32 id)
{
	// 주인과 소환수가 같이 있어야한다.
	if(pclCM->IsValidID(id)  == false)
		return;

	// 메시지가 자주 오지 못하도록 막는다
	if ( pclCM->CR[id]->CheckLastMsgClock(1000) == true )
	{
		return ;
	}

	//---------------------------------------------
	//
	//---------------------------------------------
	SI32 charid = pclCM->GetIDFromCharUnique(pclinfo->m_siCharUnique);
	if(pclCM->IsValidID(charid)  == false)
		return;

	cltCharServer* pclchar = (cltCharServer*)pclCM->CR[charid];
	if(pclchar==NULL)
		return;

	cltSummon *pclSummon = pclchar->pclCI->clSummonInfo.GetSummonFromSummonID(pclinfo->m_siSummonID);
	if(pclSummon==NULL)
		return;
	//---------------------------------------------

	SI32 siSummonIndex = pclchar->pclCI->clSummonInfo.GetSummonIndexFromSummonID(pclinfo->m_siSummonID);

	SI32 updatereason = SETSUMMON_REASON_UPDATE ;

	SI32 siSummonID = pclchar->GetSummonID();
	if(pclCM->IsValidID(siSummonID )  == true)
	{
		// 소환수 자신이 갖고 있는 정보와 내가 갖고 있는 정보가 다르면 안된다.
		if(pclCM->CR[siSummonID]->GetSummonIndex() == siSummonIndex) 
		{
			// 캐릭터가 살아 있어야하고 보너스가 남아 있어야 쓸 수 있다.
			SI32 siSummonBonus = pclCM->CR[siSummonID]->pclCI->clIP.GetBonus();
			if ( (siSummonBonus > 0) && (pclinfo->m_siDecreaseBonus > 0) && (siSummonBonus >= pclinfo->m_siDecreaseBonus) )
			{
				pclCM->CR[siSummonID]->pclCI->clIP.IncreaseStr( pclinfo->m_siStr );
				pclCM->CR[siSummonID]->pclCI->clIP.IncreaseDex( pclinfo->m_siAgi );
				pclCM->CR[siSummonID]->pclCI->clIP.IncreaseMag( pclinfo->m_siWis );
				pclCM->CR[siSummonID]->pclCI->clIP.IncreaseVit( pclinfo->m_siVit );
				pclCM->CR[siSummonID]->pclCI->clIP.IncreaseWis( pclinfo->m_siWisdom );
				pclCM->CR[siSummonID]->pclCI->clIP.IncreaseHnd( pclinfo->m_sihand);

				pclCM->CR[siSummonID]->pclCI->clIP.DecreaseBonus( pclinfo->m_siDecreaseBonus );
			}
		} 
		pclCM->CR[siSummonID]->SetUpdateSwitch(UPDATE_IP, true, 0);
	}

	// 캐릭터의 IP를 클라이언트로 보낼 수 있게 한다. 
	pclCM->CR[id]->SetUpdateSwitch(UPDATE_SUMMON, true, 0);

	//---------------------------------------------
	// DB에 소환수를 설정한다.
	//---------------------------------------------
	pclchar->SendDBSummon(updatereason, siSummonIndex);

	// 버그 수정 : 박기형 2009.06.19
	// 소환수의 스테이터스는 파라미터 값을 계산 해서 그에 따라 결정되는 값들을 수정한다. 
	// 서버에서 주기적으로 실행하시만
	// 그 시간 동안 스테이터스값이 변경 되지 않아 원치 않는 결과 발생
	// 예) 최대 체력치가 증가되지 않아 클라이언트의 hp증가되지 않는다.
	cltCharServer* pclSummonChar = (cltCharServer*)pclCM->CR[siSummonID];
	if(pclSummonChar != NULL)
		pclSummonChar->ParametaBoxAction();
}

void cltServer::DoMsg_GAMEMSG_REQUEST_USESUMMONBONUS(cltMsg* pclMsg, SI32 id)
{
    if( pclClient->IsCountrySwitch( Switch_SummonTramsform ) )
	{
		cltCharServer* pclChar = pclCM->GetCharServer(id);
		if( NULL == pclChar ) return; 

		// 내가 사용하는 소환수가 변신중이면 적용이 불가능하게 한다.
		if( pclChar->pclCI->clSummonInfo.IsSummonTranform( pclChar->siSummonIndex ) )
		{
			SendServerResponseMsg( 0, SRVAL_RETURN_SUMMON_TRANSFORM_USE_BONUS, 0, 0, id );
			return;
		}
	}
	

	// [추가 : 황진성 2008. 2. 20 => 소환수 능력치 변경 수정용.]
	if( true == pclClient->IsCountrySwitch( Switch_Summon_Ability_Reform ) )
	{	
		cltGameMsgRequest_SummonUseBonus* pclinfo = (cltGameMsgRequest_SummonUseBonus*)pclMsg->cData;
		New_Use_Summon_Bonus( pclinfo, id );
	}
	else
	{
		cltGameMsgRequest_SummonUseBonusPoint* pclinfo = (cltGameMsgRequest_SummonUseBonusPoint*)pclMsg->cData;
		Use_Summon_Bonus( pclinfo, id );
	}
}


void cltServer::DoMsg_GAMEMSG_REQUEST_TAKEINOUT(cltMsg* pclMsg, SI32 id)
{
	// 주인과 소환수가 같이 있어야한다.
	if(pclCM->IsValidID(id)  == false)	return;

	//---------------------------------------------
	//
	//---------------------------------------------
	cltGameMsgRequest_SummonTakeInOut* pclinfo = (cltGameMsgRequest_SummonTakeInOut*)pclMsg->cData;
	SI32 charid = pclCM->GetIDFromCharUnique(pclinfo->siCharUnique);
	if(pclCM->IsValidID(charid)  == false)	return;
	if(pclCM->CR[id]->GetCharUnique() != pclCM->CR[charid]->GetCharUnique())		return;

	cltCharServer* pclchar = (cltCharServer*)pclCM->CR[charid];
	if(pclchar==NULL)	return;

	cltSummon *pclSummon = pclchar->pclCI->clSummonInfo.GetSummonFromSummonID(pclinfo->siSummonID);
	if(pclSummon==NULL)	return;

	SI32 siSummonIndex = pclchar->pclCI->clSummonInfo.GetSummonIndexFromSummonID(pclinfo->siSummonID);

	//---------------------------------------------
	// [기형] 배틀로얄 경기장이라면 
	//---------------------------------------------
	if(NULL != m_pclBattleRoyalMgr)
	{
		CBattleRoyal* pclBattleRoyal = m_pclBattleRoyalMgr->GetBattleRoyal(pclchar);
		if( NULL != pclBattleRoyal)
		{
			if( TRUE == pclBattleRoyal->IsExistStadium(pclchar) )
				return;
		}
	}

	//---------------------------------------------
	// 예약 되어 있는 아이템 사용이 있다면 처리한다. 
	//---------------------------------------------
	pclchar->ItemUseReserveAction();

	//---------------------------------------------
	// 예약 되어 있는 탄환아이템 사용이 있다면 처리한다. 
	//---------------------------------------------
	pclchar->BulletUseReserveAction();

	switch(pclinfo->siStatus)
	{
	case SUMMON_STATUS_TAKEOUT:	
		{
			/*if( pclClient->IsCountrySwitch(Switch_SummonTramsform) )
			{
				//if( false == pclchar->pclCI->clSummonInfo.IsSummonTranformTickCount() ) return;
			}*/

			//---------------------------------------------
			// 소환수가 TAKE IN 상태이어야한다.
			//---------------------------------------------
			if(pclSummon->siStatus != SUMMON_STATUS_TAKEIN) return;

			//---------------------------------------------
			// DB에 소환수를 설정한다.
			//---------------------------------------------
			cltSummon clSummon;
			clSummon.Set(pclSummon);
			clSummon.siStatus = SUMMON_STATUS_TAKEOUT;
			pclchar->SendDBSummon(SETSUMMON_REASON_TAKEOUT, siSummonIndex, &clSummon);
		}
		break;
	case SUMMON_STATUS_TAKEIN:	
		{
			/*if( pclClient->IsCountrySwitch(Switch_SummonTramsform) )
			{
				//if( false == pclchar->pclCI->clSummonInfo.IsSummonTranformTickCount() ) return;
			}*/
			//---------------------------------------------
			// 소환수가 나와서 살아있지 않으면 안된다.
			//---------------------------------------------
			SI32 siSummonID = pclchar->GetSummonID();

			// 그런 ID가 없다면 
			if( pclCM->IsValidID(siSummonID) == FALSE )	
			{
				if( pclSummon->siStatus == SUMMON_STATUS_TAKEOUT )
				{
					if( pclClient->IsCountrySwitch(Switch_SummonTramsform) )
					{
						// 소환수가 변신되있는 상태면 변신을 해제한다.
						if( pclchar->pclCI->clSummonInfo.IsSummonTranform( pclchar->GetSummonIndex() ) )
						{
							pclchar->TransformSummon( pclCM, GetSummonTransformManager() );
							siSummonID = pclchar->GetSummonID();
						}
					}

					if(pclSummon->clIP.GetLife()>0)	pclSummon->siStatus = SUMMON_STATUS_TAKEIN;
					else							pclSummon->siStatus = SUMMON_STATUS_DEAD;

					//==========================================
					// 주인이 갖고 있는 소환수의 정보를 설정한다.
					//==========================================
					cltGameMsgResponse_Summon clInfo1(pclchar->GetCharUnique(), siSummonIndex, pclSummon);
					cltMsg clMsg1(GAMEMSG_RESPONSE_SUMMONINFO, sizeof(clInfo1), (BYTE*)&clInfo1);
					pclchar->SendNetMsg((sPacketHeader*)&clMsg1);

					// 주인한테도 정보를 갱신시킨다.
					pclchar->siChildCharUnique = 0;
					cltGameMsgResponse_ChangeSummonStatus clInfo2(id, pclchar->siChildCharUnique, siSummonIndex);
					cltMsg clMsg2(GAMEMSG_RESPONSE_CHANGESUMMONSTATUS, sizeof(clInfo2), (BYTE*)&clInfo2);
					pclchar->SendNetMsg((sPacketHeader*)&clMsg2);
					return;
				}
			}

			// 죽어있다면 TAKEIN이 안된다.
			if( pclCM->IsAlive(siSummonID) == FALSE )			return;

			//---------------------------------------------
			// 소환수가 TAKE OUT 상태이어야한다.
			//---------------------------------------------
			if( pclSummon->siStatus != SUMMON_STATUS_TAKEOUT )	return;

			//---------------------------------------------
			// DB에 소환수를 설정한다.
			//---------------------------------------------
			pclchar->SendDBSummon( SETSUMMON_REASON_UPDATE, siSummonIndex );

			cltSummon clSummon;
			clSummon.Set(pclSummon);
			clSummon.siStatus = SUMMON_STATUS_TAKEIN;
			pclchar->SendDBSummon( SETSUMMON_REASON_TAKEIN, siSummonIndex, &clSummon );
		}
		break;
	}
}

void cltServer::DoMsg_GAMEMSG_REQUEST_SUMMONATTACKCHANGE(cltMsg* pclMsg, SI32 id)
{
	//---------------------------------------------
	// 
	//---------------------------------------------
	cltGameMsgRequest_SummonAttackChange* pclinfo = (cltGameMsgRequest_SummonAttackChange*)pclMsg->cData;
	SI32 charid = pclCM->GetIDFromCharUnique(pclinfo->siCharUnique);
	if(pclCM->IsValidID(charid)  == false)	return;

	cltCharServer* pclchar = (cltCharServer*)pclCM->CR[charid];
	if(pclchar==NULL)	return;

	cltSummon *pclSummon = pclchar->pclCI->clSummonInfo.GetSummonFromSummonID(pclinfo->siSummonID);
	if(pclSummon==NULL)	return;
	//---------------------------------------------

	SI32 siSummonIndex = pclchar->pclCI->clSummonInfo.GetSummonIndexFromSummonID(pclinfo->siSummonID);

	// 공격속성을 검사한다.
	if(	pclinfo->siAttackType !=  SUMMON_ATTACKTYPE_ACTIVE &&
		pclinfo->siAttackType !=  SUMMON_ATTACKTYPE_AVOID) return;

	// 소환수 공격속성을 설정한다.
	if(pclSummon->siAttackType != pclinfo->siAttackType)
	{
		pclSummon->siAttackType = (SI08)pclinfo->siAttackType;
		
		// DB에 소환수를 설정한다.
		pclchar->SendDBSummon(SETSUMMON_REASON_UPDATE, siSummonIndex);

		// 퀘스트 업데이트
		pclchar->DoQuestCheckAndAction(QUEST_CLEAR_COND_SUMMONCHANGESTATUS);
		
		cltGameMsgResponse_SummonAttackChange clInfo(pclchar->GetCharUnique(), 1, pclinfo->siSummonID, pclSummon->siAttackType);
		cltMsg clMsg(GAMEMSG_RESPONSE_SUMMONATTACKCHANGE, sizeof(clInfo), (BYTE*)&clInfo);
		pclCM->CR[charid]->SendNetMsg((sPacketHeader*)&clMsg);
	}

}


void cltServer::DoMsg_GAMEMSG_REQUEST_SUMMONEVOLVE(cltMsg* pclMsg, SI32 id)
{
	//---------------------------------------------
	//
	//---------------------------------------------
	cltGameMsgRequest_SummonEvolve* pclinfo = (cltGameMsgRequest_SummonEvolve*)pclMsg->cData;
	SI32 charid = pclCM->GetIDFromCharUnique(pclinfo->siCharUnique);
	if(pclCM->IsValidID(charid)  == false)	return;

	cltCharServer* pclchar = (cltCharServer*)pclCM->CR[charid];
	if(pclchar==NULL)	return;

	cltSummon *pclSummon = pclchar->pclCI->clSummonInfo.GetSummonFromSummonID(pclinfo->siSummonID);
	if(pclSummon==NULL)	return;

	SI32 siSummonIndex = pclchar->pclCI->clSummonInfo.GetSummonIndexFromSummonID(pclinfo->siSummonID);

	//---------------------------------------------
	// 아이템을 장착하고 있으면 안된다
	//---------------------------------------------
	if ( TRUE == pclchar->pclCI->clSummonInfo.IsHaveEquipItem(siSummonIndex) )
	{
		return;
	}

	//---------------------------------------------
	// 현재 진화중인데 또 같은 메시지가 오면 돌아간다.
	//---------------------------------------------
	if(pclchar->bIsEvolving == true) return ;

	//---------------------------------------------
	// 새로 변경할 SUMMON
	//---------------------------------------------
	SI32 siNewKind= pclinfo->siKind;

	//---------------------------------------------
	// 소환수가 나와있지 않으면 변경시킬수 없다.
	//---------------------------------------------
	SI32 siSummonID = pclchar->GetSummonID();
	if(pclCM->IsValidID(siSummonID)  == FALSE)	return;

	//---------------------------------------------
	// 죽어있으면 돌아간다.
	//---------------------------------------------
	if(pclSummon->clIP.GetLife()<=0)	return;

	//--------------------------------
	// 진화시킬 소환수의 Kind를 점검한다.
	//--------------------------------
	SI32 kind = pclSummon->siKind;
	if(kind <= 0 || kind >= MAX_KIND_NUMBER)	return;

	//cltKindInfo* pclki = pclKindInfoSet->pclKI[kind];
	cltKindInfo* pclki = NULL;
	if(IsCountrySwitch(Switch_Summon_Separate)) 
	{
		pclki = pclKindInfoSet->pclSummonKI[kind];
	}
	else
	{
		pclki = pclKindInfoSet->pclKI[kind];
	}

	if(pclki == NULL)							return;
	if(pclki->siSummonRank <= 0)				return;

	//---------------------------------------------
	// 진화 LEVEL보다 작으면 돌아간다. 진화일 때
	//---------------------------------------------
	if ( pclinfo->siMode == 0 && pclSummon->clIP.GetLevel() < pclki->siEvolveLevel )
		return;

	//--------------------------------
	// 진화후의 소환수 Kind를 점검한다.
	//--------------------------------
	if(siNewKind <= 0 || siNewKind >= MAX_KIND_NUMBER)	return;
	//cltKindInfo* pclnewki = pclKindInfoSet->pclKI[siNewKind];
	cltKindInfo* pclnewki = NULL;
	if(IsCountrySwitch(Switch_Summon_Separate)) 
	{
		pclnewki = pclKindInfoSet->pclSummonKI[siNewKind];
	}
	else
	{
		pclnewki = pclKindInfoSet->pclKI[siNewKind];
	}

	if(pclnewki == NULL)						return;
	if(pclnewki->siSummonRank <= 0)				return;

	//--------------------------------
	// 진화시키기위한 준비물 점검
	//--------------------------------
	SI32 siSuccess		= 1;
	SI32 siTicketPos	= 0;
	SI32 siTicketUnique = ITEMUNIQUE(5370);	// 축복의돌
	SI32 siTicketNum	= 10;
	bool bItemUse = true ;
	bool bQuestEvolve = false ;
	//--------------------------------
	// 진화후의 소환수는 진화전과 1단계만 차이가 난다. 
	//--------------------------------
	switch(pclinfo->siMode)
	{
	case SUMMONEVOLVE_EVOLVE: // 진화
		{
			if(pclnewki->siSummonRank != pclki->siSummonRank + 1)	return;
			if(pclnewki->siSummonRank >  LIMIT_SUMMON_RANK_EVOLVE)	return;
			if(pclnewki->bSummonMall  == TRUE                   )   return; // 아이템 몰 상품으로는 진화 안됨
			
			siTicketUnique = GetSummonTicketUnique(pclki->siSummonRank);
			siTicketNum	= GetSummonTicketNum(pclki->siSummonRank);		


			SI16 questtype, questunique, para1, para2;
			SI16 questclearcond =  QUEST_CLEAR_COND_SUMMONEVOLUTIONRANK;
			if(pclchar->pclCI->clQuestInfo.IsQuestGoing(questclearcond, &questtype, &questunique, &para1, &para2) == true)
			{
				if ( !pclchar->pclCI->clBI.bQuestEvolve )
				{
					siTicketNum = 0 ;
					bQuestEvolve = true ;
				}
			}

			questclearcond =  QUEST_CLEAR_COND_SUMMONEVOLUTION;
			if(pclchar->pclCI->clQuestInfo.IsQuestGoing(questclearcond, &questtype, &questunique, &para1, &para2) == true)
			{
				if ( !pclchar->pclCI->clBI.bQuestEvolve )
				{
					siTicketNum = 0 ;
					bQuestEvolve = true ;
				}
			}
		}
		break;
	case SUMMONEVOLVE_DEVOLUTION: // 퇴화
		{
			if(pclnewki->siSummonRank != pclki->siSummonRank - 1)	return;
			if(pclnewki->bSummonMall  == TRUE                   )   return; // 아이템 몰 상품으로는 진화 안됨
		}
		break;
	case SUMMONEVOLVE_TRANSFORM: // 변환
		{
			if(pclnewki->siSummonRank != pclki->siSummonRank)	return;
			if(pclnewki->bSummonMall  == TRUE                   )   return; // 아이템 몰 상품으로는 진화 안됨
		}
		break;
	default:
		return ;
	}

	if ( siTicketNum )
	{
		///////////////////////////////////////////////////////
		// 이용권 확인 
		siTicketPos = pclCM->CR[charid]->pclCI->clCharItem.GetItemPos(siTicketUnique);
		if( siTicketPos < PERSONITEM_INV0 || siTicketPos >= MAX_ITEM_PER_PERSON )							siSuccess = -1;
		if(siSuccess > 0 && pclCM->CR[id]->pclCI->clCharItem.clItem[siTicketPos].siItemNum < siTicketNum)	siSuccess = -1;
		//////////////////////////////////////////////////////////////////////////
	}
	else
	{
		bItemUse = false ;
	}
	
	//KHY - 0909 - 선무당의 선물 - 소환수의 진화시 축돌이 들경우 일정,확률로 라쿤 가죽 지갑을 준다.
	if(pclClient->IsCountrySwitch(Switch_MudangPresent))
	{
		SI32 siMudangPresent = ITEMUNIQUE(13333);	// 라쿤 가죽지갑.
		
		if((siSuccess > 0)&&(siTicketUnique == ITEMUNIQUE(5370))&&(siTicketNum > 0)) // 성공이면서 축돌을 사용해야만 계산한다.
		{	
			// - 겟수 결정.
			SI32 numPurse = 0; // 주게되는 최종 지갑 겟수.
			SI32 selectPursePer = ((siTicketNum * pclnewki->siSummonRank * 2) / 10);
			SI32 randval = (rand()%100)+1;
			
			if(selectPursePer > 100)
			{
				numPurse = selectPursePer/100;  // 100%당 1개씩 무조건 준다.
				selectPursePer = selectPursePer%100; //나머지로 확률 돌리자.
			}

			if(randval <= selectPursePer)
				numPurse += 1;
			/////////////////////////////////////////////////////////////////////
			// - 아이템 지급. - 라쿤 가죽지갑			
			if(numPurse > 0)
			{
				pclchar->GiveItem( siMudangPresent, numPurse, INPUTITEMPERSON_OPTION_MUDANGPRESENT, 0 );
			}

			// 설정 정보를 알린다.
			SendServerResponseMsg(0, SRVAL_SUMMON_ADD_MUDANGPRESENT, siMudangPresent, numPurse, pclchar->GetCharUnique() );	
		}
	}

	if(siSuccess > 0)
	{
		// 물품 사용 통계를 낸다.
		pclItemManager->AddUseNum(siTicketUnique, siTicketNum);

		//새로운 통계 - 물품 사용 통계를 낸다.
		//NStatistics_Add("Item", siTicketUnique, "UseNum", siTicketNum);

		// 소환수를 소환한 마을의 Unique를 준다.
		SI32 villageunique  = pclCM->CR[charid]->GetCurrentVillageUnique();

		//====================================================
		// 새로 진화할 소환수를 설정한다.
		//====================================================
		cltPIPInfoCommon	clipinfo;
		cltPIPInfo			clipinfo2;
		
		//cltKindInfo* pclki = pclKindInfoSet->pclKI[siNewKind];
		cltKindInfo* pclki = NULL;
		cltGMCommandKindInfo* pclGMki = NULL;

		if(IsCountrySwitch(Switch_Summon_Separate)) 
		{
			pclki = pclKindInfoSet->pclSummonKI[siNewKind];
			pclGMki = pclKindInfoSet->pclGMCommandKindInfo[siNewKind];
		}
		else
		{
			pclki = pclKindInfoSet->pclKI[siNewKind];
			pclGMki = pclKindInfoSet->pclGMCommandKindInfo[siNewKind];
		}
		if(NULL == pclki) 
			return;
		if(NULL == pclGMki)
			return;

		clipinfo.SetBasicAbility(&pclki->clBA);	

		clipinfo2.Set(&clipinfo);
		cltParametaBox clPB;
		clPB.Init(pclItemManager, &clipinfo2, NULL, pclki, pclGMki);
		clPB.Action(NULL, 0, PERSONITEM_WEAPON1, NULL, NULL, NULL, NULL, NULL, NULL, NULL, GAMEMODE_SERVER, true);

		clipinfo.SetLife(clPB.GetMaxLife());	// 기본적인 것만 저장한다.( 사실 PBBOX 를 사용해서 풀로 채워야한다)
		clipinfo.SetMana(clPB.GetMaxMana());

		//cyj 소환수의 Luk은 스태미너이므로 전에 값을 넣어줌
#ifdef _SAFE_MEMORY
		SI32 summonskill = pclClient->pclCM->CR[id]->pclCI->clCharSkill.GetTotalSkillLevel( SKILL_SUMMON, 0, 0, &pclClient->pclCM->CR[id]->pclCI->clCharItem.clItem[0], id );
#else
		SI32 summonskill = pclClient->pclCM->CR[id]->pclCI->clCharSkill.GetTotalSkillLevel( SKILL_SUMMON, 0, 0, pclClient->pclCM->CR[id]->pclCI->clCharItem.clItem, id );
#endif

		clipinfo.SetLuk(pclClient->GetMaxStaminaValue(summonskill, siNewKind) );
		
		// 예전의 경험치와 레벨을 상속받는다.
		clipinfo.siExp	 = pclSummon->clIP.siExp;
		clipinfo.siLevel = pclSummon->clIP.siLevel;
		clipinfo.siBonus = clipinfo.siLevel * 5;
		
		
		cltSummonScroll clscroll(pclki->siSummonScroll[0], 0, pclki->siSummonScroll[1], 0);	

		cltSummonItemInfo clItemInfo;
		clItemInfo.Init();
		// DB에 소환수를 설정한다.
		cltSummon clSummon(	pclSummon->siSummonID,	
							siNewKind,					
							pclSummon->szName, 
							SUMMON_STATUS_TAKEIN,	
							SUMMON_ATTACKTYPE_AVOID,	
							pclSummon->siIdentity, 
							0,	
							pclSummon->siAttackWearStoneNum,	
							pclSummon->siDefenseWearStoneNum,
							&clipinfo,				
							&clscroll,
							pclSummon->m_siTranformStamina );

		//==================================
		// 진화이용권을 2장쓰는것을막는다.
		pclchar->bIsEvolving = true;
		//==================================

		cltItem clTicketItem;
		SI16 brareswitch = 0;
		pclClient->pclItemManager->MakeRandItemUnique( siTicketUnique, &clTicketItem, 0, 0, &brareswitch, 0,0,0,0,0,0);
		clTicketItem.SetItemNum(siTicketNum);

		// DB로 요청한다. 
		sDBRequest_SummonEvolve clMsg(charid, pclCM->CR[charid]->pclCI->GetPersonID(), villageunique,siTicketPos,
									  siTicketUnique, siTicketNum, &clSummon,bQuestEvolve,bItemUse,pclinfo->siMode, 
									  &clTicketItem, kind);
		pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);
	}
	else
	{
		// 설정 정보를 설정한다.
		cltGameMsgResponse_SummonEvolve clinfo(pclchar->GetCharUnique(), siSuccess, pclinfo->siSummonID, siNewKind,pclinfo->siMode);
		cltMsg clMsg(GAMEMSG_RESPONSE_SUMMONEVOLVE, sizeof(clinfo), (BYTE*)&clinfo);
		pclCM->CR[charid]->SendNetMsg((sPacketHeader*)&clMsg);
	}
}


void cltServer::DoMsg_GAMEMSG_REQUEST_SUMMONSCROLL(cltMsg* pclMsg, SI32 id)
{
	//---------------------------------------------
	//
	//---------------------------------------------
	cltGameMsgRequest_SummonScroll* pclinfo = (cltGameMsgRequest_SummonScroll*)pclMsg->cData;
	SI32 charid = pclCM->GetIDFromCharUnique(pclinfo->siCharUnique);
	if(pclCM->IsValidID(charid)  == false)	return;

	cltCharServer* pclchar = (cltCharServer*)pclCM->CR[charid];
	if(pclchar==NULL)	return;

	cltSummon *pclSummon = pclchar->pclCI->clSummonInfo.GetSummonFromSummonID(pclinfo->siSummonID);
	if(pclSummon==NULL)	return;
	//---------------------------------------------
	
	//---------------------------------------------
	// 새로 강화할 Scroll
	//---------------------------------------------
	SI32 siScrollUnique		= pclinfo->siScrollUnique;
	// 아이템이 부적인지 확인한다. 
	SI64 atb =	pclItemManager->GetItemAtb(siScrollUnique);
	if((atb & ITEMATB_SCROLL) ==0)		return;

	// 부적의 내용을 확인한다. 
	cltItemInfo* pclscrollinfo = pclItemManager->FindItemInfo(siScrollUnique);
	if(pclscrollinfo == NULL)			return;

	// 부적의 기능을 확인한다. 
	SI32 function	= pclscrollinfo->siScrollFunction;
	if(function <= 0)					return;

	// 부적의 증가 능력을 구한다.
	SI32 siAddScrollPercent = pclscrollinfo->siScrollAbility;
	if(siAddScrollPercent <= 0)			return;

	//---------------------------------------------
	// 소환수가 나와있지 않으면 변경시킬수 없다.
	//---------------------------------------------
	SI32 siSummonID = pclchar->GetSummonID();
	if(pclCM->IsValidID(siSummonID)  == FALSE)	return;

	//---------------------------------------------
	// 죽어있으면 돌아간다.
	//---------------------------------------------
	if(pclSummon->clIP.GetLife()<=0)	return;


	//---------------------------------------------
	// 강화제한보다 크면 돌아간다.
	//---------------------------------------------
	if(pclSummon->clScroll.FindUnique(siScrollUnique) == FALSE)	return;
	SI32 CurrPercent = pclSummon->clScroll.GetPercent(siScrollUnique);
	SI32 siMaxSrollPercent = min(LIMIT_SUMMON_ADD_SCROLL_PERCENT, pclSummon->clIP.GetLevel()*10/2 );
	if(CurrPercent >= siMaxSrollPercent)	return;
	if(CurrPercent + siAddScrollPercent >= siMaxSrollPercent)
	{
		siAddScrollPercent = siMaxSrollPercent - CurrPercent;
	}

	
	//--------------------------------
	// 강화시키기위한 준비물 점검
	//--------------------------------
	SI32 siSuccess		= 1;
	SI32 siScrollPos	= 0;
	SI32 siScrollNum	= 1;
	if(siScrollUnique <= 0 || siScrollNum <= 0)		return;

	///////////////////////////////////////////////////////
	// 부적 확인 
	siScrollPos = pclCM->CR[charid]->pclCI->clCharItem.GetItemPos(siScrollUnique);
	if( siScrollPos < PERSONITEM_INV0 || siScrollPos >= MAX_ITEM_PER_PERSON )							siSuccess = -1;
	if(siSuccess > 0 && pclCM->CR[id]->pclCI->clCharItem.clItem[siScrollPos].siItemNum < siScrollNum)	siSuccess = -1;
	//////////////////////////////////////////////////////////////////////////
	
	if(siSuccess > 0)
	{
		// 물품 사용 통계를 낸다.
		pclItemManager->AddUseNum(siScrollUnique, siScrollNum);

		//새로운 통계 - 물품 사용 통계를 낸다.
		//NStatistics_Add("Item", siScrollUnique, "UseNum", siScrollNum);

		pclStatisticsManager->clDailyStatistics.siScrollSummonNumber += siScrollNum;	// 통계처리를 한다.
 
		// 소환수를 소환한 마을의 Unique를 준다.
		SI32 villageunique  = pclCM->CR[charid]->GetCurrentVillageUnique();

		//====================================================
		// 새로 강화할 소환수를 설정한다.
		//====================================================
		//---------------------------------------------
		// 소환수를 강화시키기위한 부적을 적용한다.
		//---------------------------------------------
		cltSummon clSummon;
		clSummon.Set(pclSummon);
		clSummon.clScroll.AddPercent(siScrollUnique, siAddScrollPercent);

		// DB로 요청한다. 
		sDBRequest_SummonScroll clMsg(charid, pclCM->CR[charid]->pclCI->GetPersonID(), villageunique, siScrollPos, siScrollUnique, siScrollNum, &clSummon);
		pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);
		
	}
	else
	{
		// 설정 정보를 설정한다.
		cltGameMsgResponse_SummonScroll clinfo(pclchar->GetCharUnique(), siSuccess, pclinfo->siSummonID);
		cltMsg clMsg(GAMEMSG_RESPONSE_SUMMONSCROLL, sizeof(clinfo), (BYTE*)&clinfo);
		pclCM->CR[charid]->SendNetMsg((sPacketHeader*)&clMsg);
	}
}

void cltServer::DoMsg_GAMEMSG_REQUEST_SUMMON_STATUSINIT(cltMsg* pclMsg,SI32 id)
{
	if(pclCM->IsValidID(id)  == false)	return;

	//---------------------------------------------
	//
	//---------------------------------------------
	cltGameMsgRequest_Summon_StatusInit* pclinfo = (cltGameMsgRequest_Summon_StatusInit*)pclMsg->cData;
	SI32 charid = pclCM->GetIDFromCharUnique(pclinfo->siCharUnique);
	if(pclCM->IsValidID(charid)  == false)	return;

	cltCharServer* pclchar = (cltCharServer*)pclCM->CR[charid];
	if(pclchar==NULL)	return;

	cltSummon *pclSummon = pclchar->pclCI->clSummonInfo.GetSummonFromSummonID(pclinfo->siSummonID);
	if(pclSummon==NULL)	return;

	SI32 siSummonIndex = pclchar->pclCI->clSummonInfo.GetSummonIndexFromSummonID(pclinfo->siSummonID);

	//---------------------------------------------
	// 소환수가 죽어 있으면 안된다.
	//---------------------------------------------
	if(pclSummon->siStatus == SUMMON_STATUS_DEAD) return;

	//---------------------------------------------
	// 소환수가 장비를 착용하고 있으면 안된다
	//---------------------------------------------
	if ( TRUE == pclchar->pclCI->clSummonInfo.IsHaveEquipItem(siSummonIndex) )
	{
		SendServerResponseMsg(0, SRVAL_SUMMON_STATUSINIT_FAIL_HAVEITEM, 0, 0, pclchar->GetCharUnique() );
		return;
	}

	//--------------------------------
	// 초기화시키기 위한 준비
	//--------------------------------
	//cyj 상태초기화이용권 유니크변경 14090->24030
	//SI32 itemunique = 14090 ;  // 상태초기화이용권
	
	//[진성] 캐릭터 상태 초기화 일정기간동안 마음것 사용. 
	cltItem clitem;
	SI32	pos = 0;
	if(pclClient->pclEventTimeManager->InEventTime("char_ability_init", &sTime) )
	{
		clitem.Init();	
		pos = 0;
	}   
	else
	{
		SI32 itemunique			= ITEMUNIQUE(24030) ;  // 상태초기화이용권
		SI32 siEventItemUnique	= ITEMUNIQUE(24125) ;  // 상태초기화이용권(이벤트)

		if (	pclchar->pclCI->clCharItem.GetItemNum(itemunique) <= 0 &&
				pclchar->pclCI->clCharItem.GetItemNum(siEventItemUnique) <= 0)
			return ;

		pos = pclchar->pclCI->clCharItem.GetItemPos(siEventItemUnique);
		if(pos < 0)
		{
			pos = pclchar->pclCI->clCharItem.GetItemPos(itemunique);
			if(pos < 0)
				return;
		}
		
		clitem.Init();	
		clitem.Set( &pclchar->pclCI->clCharItem.clItem[pos]);
		clitem.SetItemNum(1);
	}


	//====================================================
	// 초기화할 소환수를 설정한다.
	//====================================================
	cltPIPInfoCommon	clipinfo;
	cltPIPInfo			clipinfo2;
	//--------------------------------
	// 소환수의 kind를 점검한다.
	//--------------------------------
	SI32 kind = pclSummon->siKind;
	if(kind <= 0 || kind >= MAX_KIND_NUMBER)	return;
	
	cltKindInfo* pclki = NULL;
	cltGMCommandKindInfo*	pclGMki = NULL;

	if(pclClient->IsCountrySwitch(Switch_Summon_Separate)) 
	{
		pclki = pclKindInfoSet->pclSummonKI[kind];
		pclGMki = pclKindInfoSet->pclGMCommandKindInfo[kind];
	}
	else
	{
		pclki = pclKindInfoSet->pclKI[kind];
		pclGMki = pclKindInfoSet->pclGMCommandKindInfo[kind];
	}
	
	if(pclki == NULL)							return;
	if(pclki->siSummonRank <= 0)				return;
	if(pclGMki == NULL)							return;

	clipinfo.SetBasicAbility(&pclki->clBA);	

	clipinfo2.Set(&clipinfo);
	cltParametaBox clPB;
	clPB.Init(pclItemManager, &clipinfo2, NULL, pclki, pclGMki);
	clPB.Action(NULL, 0, PERSONITEM_WEAPON1, NULL, NULL, NULL, NULL, NULL, NULL, NULL, GAMEMODE_SERVER, true);

	clipinfo.SetLife(clPB.GetMaxLife());	// 기본적인 것만 저장한다.( 사실 PBBOX 를 사용해서 풀로 채워야한다)
	clipinfo.SetMana(clPB.GetMaxMana());

	//cyj 소환수의 Luk은 스태미너이므로 전에 값을 넣어줌
#ifdef _SAFE_MEMORY
	SI32 summonskill = pclClient->pclCM->CR[id]->pclCI->clCharSkill.GetTotalSkillLevel( SKILL_SUMMON, 0, 0, &pclClient->pclCM->CR[id]->pclCI->clCharItem.clItem[0], id );
#else
	SI32 summonskill = pclClient->pclCM->CR[id]->pclCI->clCharSkill.GetTotalSkillLevel( SKILL_SUMMON, 0, 0, pclClient->pclCM->CR[id]->pclCI->clCharItem.clItem, id );
#endif

	clipinfo.SetLuk(pclClient->GetMaxStaminaValue(summonskill, kind));

	// 예전의 경험치와 레벨을 상속받는다.
	clipinfo.siExp	 = pclSummon->clIP.siExp;
	clipinfo.siLevel = pclSummon->clIP.siLevel;
	clipinfo.siBonus = clipinfo.siLevel * 5;

	cltSummonScroll clscroll;
	clscroll.Set(&pclSummon->clScroll);

	cltSummonItemInfo clItemInfo;
	clItemInfo.Init();

	cltSummon clSummon(	pclSummon->siSummonID,
						kind,
						pclSummon->szName,
						SUMMON_STATUS_TAKEIN,
						SUMMON_ATTACKTYPE_AVOID,	
						pclSummon->siIdentity, 
						0, 
						pclSummon->siAttackWearStoneNum, 
						pclSummon->siDefenseWearStoneNum, 
						&clipinfo,
						&clscroll,
						pclSummon->m_siTranformStamina	);

	sDBRequest_Summon_StatusInit clMsg(charid, pclCM->CR[charid]->pclCI->GetPersonID(), pos,&clitem , &clSummon);
	pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);
}

void cltServer::DoMsg_GAMEMSG_REQUEST_SUMMON_SETAUTOHEAL(cltMsg* pclMsg,SI32 id)
{
	if(pclCM->IsValidID(id)  == false)	return;

	cltGameMsgRequest_Summon_SetAutoHeal* pclinfo = (cltGameMsgRequest_Summon_SetAutoHeal*)pclMsg->cData;
	SI32 charid = pclCM->GetIDFromCharUnique(pclinfo->siCharUnique);
	if(pclCM->IsValidID(charid)  == false)	return;

	cltCharServer* pclchar = (cltCharServer*)pclCM->CR[charid];
	if(pclchar==NULL)	return;

	cltSummonInfo *pclSummonInfo = &pclchar->pclCI->clSummonInfo ;
	if(pclSummonInfo==NULL)	return;

	if ( pclinfo->siSelectAutoHeal < 0 || pclinfo->siSelectAutoHeal > 2 ) return ;

	sDBRequest_Summon_SetAutoHeal clMsg(charid, pclCM->CR[charid]->pclCI->GetPersonID(),pclinfo->siSelectAutoHeal,pclinfo->bAutoHeal);
	pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);
}

void cltServer::DoMsg_GAMEMSG_REQUEST_TRADEMERCHANTBUYSUMMONINFO(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgRequest_TradeMerchantBuySummonInfo * pclinfo = (cltGameMsgRequest_TradeMerchantBuySummonInfo *)pclMsg->cData;

	if ( pclCM->IsValidID( id ) == FALSE )
	{
		return;
	}

	SI32 PersonID = pclCM->CR[id]->pclCI->GetPersonID();
	if( PersonID < 1 )
	{
		return;
	}

	// 소환수 소유 확인
	if ( ! pclCM->CR[id]->pclCI->clSummonInfo.HaveSummon() )
	{
		return;
	}

	SI32 siMySummonIndex = pclCM->CR[id]->pclCI->clSummonInfo.GetSummonIndexFromSummonID(pclinfo->siSummonID);

	// 소환수가 소환되어 있는지 확인
	SI32 siSummonID = pclCM->CR[id]->GetSummonID() ;
	if ( pclCM->IsValidID(siSummonID) == false ) return ;
	SI32 siSummonIndex = pclCM->CR[siSummonID]->GetSummonIndex() ;
	if ( siMySummonIndex != siSummonIndex ) return ; // 팔려는 놈이 맞는가?
	cltSummon *pclSummon = pclCM->CR[id]->pclCI->clSummonInfo.GetSummon(siSummonIndex);
	if ( pclSummon == NULL ) return ; // 소환수 정보가 있어야 한다.
	if ( pclSummon->siStatus != SUMMON_STATUS_TAKEOUT ) return ; // 나와있어야 한다
	if ( pclSummon->clIP.GetLife() <= 0 ) return ; // 죽었으면 안된다.
	if ( pclSummon->siKind <= 0 || pclSummon->siKind >= MAX_KINDINFO_NUMBER) return;
	if ( pclKindInfoSet->pclKI[pclSummon->siKind]->bSummonMall == TRUE ) return ; // 소환영웅은 안 산다.

	SI32 siSummonUnitRank = pclKindInfoSet->pclKI[pclSummon->siKind]->siSummonRank;

	SI32 siUseTicketNumber = 0;
	if(siSummonUnitRank < 9)	// 진화이용권 1장쓰는 소환수들
	{
		siUseTicketNumber = siSummonUnitRank - 1;
	}
	else if(siSummonUnitRank < 17)	// 진화이용권 2장쓰는 소환수들
	{
		siUseTicketNumber = (2 * (siSummonUnitRank - 8)) + 7;
	}
	else if(siSummonUnitRank < 25)	// 진화이용권 3장쓰는 소환수들
	{
		siUseTicketNumber = (3 * (siSummonUnitRank - 16)) + 23;
	}
	else if(siSummonUnitRank < 40)	// 진화이용권 4장쓰는 소환수들
	{
		siUseTicketNumber = (4 * (siSummonUnitRank - 24)) + 47;
	}
	else if(siSummonUnitRank >= 40)	// 진화이용권 5장쓰는 소환수들
	{
		siUseTicketNumber = (5 * (siSummonUnitRank - 39)) + 107;
	}
	else return ;

	CMinisterIzo* pclMinister = (CMinisterIzo*)pclMinisterMgr->GetMinister( MINISTER_IZO );
	if ( pclMinister == NULL ) return;

	SI64 siTicketPrice =  pclItemManager->clItemStatistic.siNPCPriceRate * 10000;
	SI64 siSelectSummonPrice = siUseTicketNumber * siTicketPrice * pclMinister->m_siPriceValue / 100 ;

	cltGameMsgResponse_TradeMerchantBuySummonInfo clGameMsgResponse_TradeMerchantBuySummonInfo( pclinfo->siSummonID, siSelectSummonPrice );
	cltMsg clMsg( GAMEMSG_RESPONSE_TRADEMERCHANTBUYSUMMONINFO,  sizeof( clGameMsgResponse_TradeMerchantBuySummonInfo ), (BYTE*)&clGameMsgResponse_TradeMerchantBuySummonInfo );
	pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);

}

void cltServer::DoMsg_GAMEMSG_REQUEST_SUMMONTRADE(cltMsg* pclMsg, SI32 id)
{
	if ( pclCM->IsValidID( id ) == FALSE )
	{
		return;
	}

	SI32 PersonID = pclCM->CR[id]->pclCI->GetPersonID();
	if( PersonID < 1 )
	{
		return;
	}

	cltGameMsgRequest_TradeMerchantBuySummon * pclinfo = (cltGameMsgRequest_TradeMerchantBuySummon*)pclMsg->cData;

	
	// 소환수 소유 확인
	if ( ! pclCM->CR[id]->pclCI->clSummonInfo.HaveSummon() )
	{
		return;
	}

	cltCharServer* pclchar = (cltCharServer* )pclCM->CR[id];
	// 소환수 판매중이라면 리턴
	if(pclchar->bIsTradeing == true)
		return;

	SI32 siMySummonIndex = pclCM->CR[id]->pclCI->clSummonInfo.GetSummonIndexFromSummonID(pclinfo->siSummonID);

	// 소환수가 소환되어 있는지 확인
	SI32 siSummonID = pclCM->CR[id]->GetSummonID() ;
	if ( pclCM->IsValidID(siSummonID) == false ) return ;
	SI32 siSummonIndex = pclCM->CR[siSummonID]->GetSummonIndex() ;
	if ( siMySummonIndex != siSummonIndex ) return ; // 팔려는 놈이 맞는가?
	cltSummon *pclSummon = pclCM->CR[id]->pclCI->clSummonInfo.GetSummon(siSummonIndex);
	if ( pclSummon == NULL ) return ; // 소환수 정보가 있어야 한다.
	if ( pclSummon->siStatus != SUMMON_STATUS_TAKEOUT ) return ; // 나와있어야 한다
	if ( pclSummon->clIP.GetLife() <= 0 ) return ; // 죽었으면 안된다.
	if ( pclSummon->siKind <= 0 || pclSummon->siKind >= MAX_KINDINFO_NUMBER) return;
	if ( pclKindInfoSet->pclKI[pclSummon->siKind]->bSummonMall == TRUE ) return ; // 소환영웅은 안 산다.

	SI32 siSummonUnitRank = pclKindInfoSet->pclKI[pclSummon->siKind]->siSummonRank;

	SI32 siUseTicketNumber = 0;
	if(siSummonUnitRank < 9)	// 진화이용권 1장쓰는 소환수들
	{
		siUseTicketNumber = siSummonUnitRank - 1;
	}
	else if(siSummonUnitRank < 17)	// 진화이용권 2장쓰는 소환수들
	{
		siUseTicketNumber = (2 * (siSummonUnitRank - 8)) + 7;
	}
	else if(siSummonUnitRank < 25)	// 진화이용권 3장쓰는 소환수들
	{
		siUseTicketNumber = (3 * (siSummonUnitRank - 16)) + 23;
	}
	else if(siSummonUnitRank < 40)	// 진화이용권 4장쓰는 소환수들
	{
		siUseTicketNumber = (4 * (siSummonUnitRank - 24)) + 47;
	}
	else if(siSummonUnitRank >= 40)	// 진화이용권 5장쓰는 소환수들
	{
		siUseTicketNumber = (5 * (siSummonUnitRank - 39)) + 107;
	}
	else return ;

	CMinisterIzo* pclMinister = (CMinisterIzo*)pclMinisterMgr->GetMinister( MINISTER_IZO );
	if ( pclMinister == NULL ) return;

	SI64 siTicketPrice =  pclItemManager->clItemStatistic.siNPCPriceRate * 10000;
	SI64 siSelectSummonPrice = siUseTicketNumber * siTicketPrice * pclMinister->m_siPriceValue / 100 ;

	cltSummon clSummon;
	clSummon.Set(pclSummon);

	pclchar->bIsTradeing = true;

	sDBRequest_SummonTrade clMsg(id,PersonID,&clSummon,siSelectSummonPrice) ;
	pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);
}

void cltServer::DoMsg_GAMEMSG_REQUEST_SELECT_SUMMONCHAR(cltMsg* pclMsg,SI32 id)
{
	if ( pclCM->IsAlive( id ) == FALSE )
	{
		return;
	}

	SI32 mapindex = pclCM->CR[id]->GetMapIndex() ;

	if ( pclMapManager->IsInstanceMap(mapindex) == false )
	{
		return ;
	}

	cltGameMsgRequest_Select_SummonChar* pclinfo = (cltGameMsgRequest_Select_SummonChar*)pclMsg->cData;

	pclCM->CR[id]->siSelectedCharIndex = pclinfo->siSelectIndex ;

	SI32 summonid = pclCM->CR[id]->GetSummonID() ;

	if ( pclCM->IsAlive(summonid) == false && pclCM->CR[id]->siSelectedCharIndex != -1 )
	{
		pclCM->CR[id]->siSelectedCharIndex = -1 ;
	}
	else if ( pclCM->IsAlive(summonid) && pclCM->CR[summonid]->GetSummonIndex() != pclCM->CR[id]->siSelectedCharIndex )
	{
		pclCM->CR[id]->siSelectedCharIndex = -1 ;
	}

	cltGameMsgResponse_Select_SummonChar clinfo(pclCM->CR[id]->siSelectedCharIndex);
	cltMsg clMsg( GAMEMSG_RESPONSE_SELECT_SUMMONCHAR,  sizeof( clinfo ), (BYTE*)&clinfo );
	pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
}

void cltServer::DoMsg_GAMEMSG_REQUEST_SUMMON_SPTCHARGE(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgRequest_Summon_ChargeStamina* pclinfo = (cltGameMsgRequest_Summon_ChargeStamina*)pclMsg->cData;

	if(pclCM->IsValidID(id)  == false)	return;
	cltCharServer* pclchar = (cltCharServer*)pclCM->CR[id];
	if(pclchar==NULL)							return;

	cltSummon *pclSummon = pclchar->pclCI->clSummonInfo.GetSummonFromSummonID(pclinfo->siSummonID);
	if(pclSummon==NULL)							return;

	SI32 siSummonIndex = pclchar->pclCI->clSummonInfo.GetSummonIndexFromSummonID(pclinfo->siSummonID);

	//--------------------------------
	// 소환수의 kind를 점검한다.
	//--------------------------------
	SI32 kind = pclSummon->siKind;
	if( pclKindInfoSet->IsValidKind(kind)  == FALSE)	return;

	// 소환수가 죽어 있으면 리턴
	if(pclSummon->siStatus == SUMMON_STATUS_DEAD) return;
	
	// 소환수의 스태미너가 0이 아니면 리턴
	if(pclSummon->clIP.GetLuk() > 0)			return;

	// 충전아이템이 인벤에 없으면 리턴
	SI32 itempos = pclCM->CR[id]->pclCI->clCharItem.FindItemInv(ITEMUNIQUE(13625));
	if(itempos < 0)	return;

	cltSummon clSummon;
	clSummon.Set(pclSummon);

	SI32 summonskill = pclClient->pclCM->CR[id]->pclCI->clCharSkill.GetTotalSkillLevel( SKILL_SUMMON, 0, 0, &pclClient->pclCM->CR[id]->pclCI->clCharItem.clItem[0], id );
	clSummon.clIP.SetLuk( pclClient->GetMaxStaminaValue(summonskill, kind));

	// 충전 아이템을 사용한다.
	cltItem	clItem;
	clItem.Set(&pclCM->CR[id]->pclCI->clCharItem.clItem[itempos]);
	clItem.SetItemNum(1);

	sDBRequest_UseItem clMsg(id, pclCM->CR[id]->pclCI->GetPersonID(), itempos, &clItem, USEITEM_REASON_NONE);
	pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);

	pclchar->SendDBSummon(SETSUMMON_REASON_CHARGESTAMINA, siSummonIndex, &clSummon);
}


void cltServer::DoMsg_GAMEMSG_REQUEST_SUMMON_TRANSFORM(cltMsg* pclMsg, SI32 id)
{
	cltCharServer* pclchar = pclCM->GetCharServer( id );
	if( NULL == pclchar ) return;

	//cltGameMsgRequest_Summon_Transform* pclinfo = (cltGameMsgRequest_Summon_Transform*)pclMsg->cData;

	pclchar->TransformSummon( pclCM, m_pclSummonTransformManager );
}