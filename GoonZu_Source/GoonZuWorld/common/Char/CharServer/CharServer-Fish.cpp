#include "..\Common\CommonHeader.h"
#include "..\..\Server\Server.h"

#include "char\CharServer\Char-Server.h"

#include "..\..\Client\Music\Music.h"

#include "..\Common\Bullet\Bullet.h"
#include "..\Common\Order\Order.h"
#include "..\CharManager\CharManager.h"
#include "Util\Util.h"
#include "Skill\Skill-Manager.h"

#include "..\..\DBManager\GameDBManager_World\DBMsg-Item.h"
#include "..\..\DBManager\GameDBManager_World\DBMsg-Person.h"
#include "..\..\DBManager\GameDBManager_World\DBMsg-Fishing.h"
#include <MsgType-Person.h>
#include <MsgType-Item.h>
#include <MsgType-System.h>
#include <Msg/MsgType-ItemMall.h>
#include <Msg/MsgType-Fishing.h>
#include <Msg/MsgType-Agriculture.h>
#include "MsgRval-Define.h"

#include "../Common/Fishing/FishingMgr.h"

extern cltCommonLogic* pclClient;

// 물고기를 잡았다.
bool cltCharServer::CatchFishAction( bool IsPerson )
{
	cltServer* pclserver = (cltServer*)pclClient;

	if ( dwPreCatchFishClock < 1 )
	{
		dwPreCatchFishClock = pclClient->CurrentClock;

		return false;
	}

	if( TABS( pclClient->CurrentClock - dwPreCatchFishClock ) >=  100000 )		
	{
		
	}
	else
	{
		return false;
	}

	cltServer *pclServer = (cltServer *)pclClient; 
	



	//if(dwPreCatchFishClock >= 5 )
	//{
		dwPreCatchFishClock = pclClient->CurrentClock;
	//}
	
	
	
	// 낚시는 야외지역에서만 가능.
	if ( pclCI->clBI.GetMapIndex() != 0 )
	{
		return false;
	}
	
	SI32 OwnerCharID				= 0;
	
	// 어획술 기술 체크
	SI32 FishSkillLevel				= 0;
	
	SI16 SpecialIncreaseSkillLevel	= 0;
	
	bool bFishNoSkillGoing			= false;

	// 스킬제한없는 낚시 퀘스트 중이라면
	SI16 siQuestType, siQuestUnique, siPara1, siPara2;
	SI16 siQuestClearCond = QUEST_CLEAR_COND_FISHNOSKILL;
	bFishNoSkillGoing = pclCI->clQuestInfo.IsQuestGoing(siQuestClearCond, &siQuestType, &siQuestUnique, &siPara1, &siPara2);

	
	// 사람 캐릭인 경우.
	if ( IsPC( GetCharUnique() ) )
	{	
		OwnerCharID = GetID();
		
		if ( siFishingBaitItemUnique < 1 )
		{
			return false;
		}

#ifdef _SAFE_MEMORY
		FishSkillLevel = pclCI->clCharSkill.GetTotalSkillLevel( SKILL_FISH, 0, 0, &pclCI->clCharItem.clItem[0], OwnerCharID );
#else
		FishSkillLevel = pclCI->clCharSkill.GetTotalSkillLevel( SKILL_FISH, 0, 0, pclCI->clCharItem.clItem, OwnerCharID );
#endif
		
		if ( (FishSkillLevel < 1) && (false == bFishNoSkillGoing) )
		{
			return false;
		}
	}
	else
	{
		// 소환수인 경우 주인이 누군지 알아야함
		if ( clIdentity.clSummonInfo.siParentCharUnique < 1 )
		{
			return false;
		}
		
		OwnerCharID = pclClient->pclCM->GetIDFromCharUnique ( clIdentity.clSummonInfo.siParentCharUnique );
		
		if ( OwnerCharID < 1 || pclClient->pclCM->IsValidID( OwnerCharID ) == FALSE )
		{
			return false;
		}

		SI32 kind = GetKind();
		if(kind > 0 && kind < MAX_KIND_NUMBER)	
		{	
			SpecialIncreaseSkillLevel = pclClient->pclKindInfoSet->GetSubSkillLevel( kind, SUMMON_JOB_FISHING );
		}
		
		FishSkillLevel = pclCI->clIP.GetLevel() / 5 + pclKI->siSubSkillLevel;
		
		if ( FishSkillLevel < 1 )
		{
			FishSkillLevel = 1;
		}
				
		cltCharServer *OwnerChar = (cltCharServer*)pclClient->pclCM->CR[OwnerCharID];
		
		siFishingBaitItemUnique = OwnerChar->siFishingBaitItemUnique;
		if ( OwnerChar->siFishingBaitItemUnique < 1 )
		{
			FishingStop();
			return false;
		}
		else if ( OwnerChar->GetCurrentOrder() != ORDER_FISH )
		{
			FishingStop();
			return false;
		}
	}

	// 낚시 기술 필요없는 퀘스트 진행 중이면서 스킬레벨이 10보다 적으면 무조건 10으로 해줌
	if ( (true == bFishNoSkillGoing) && (FishSkillLevel < 10) )
	{
		FishSkillLevel = 10;
	}
	
	// 이 지역에서 획득 가능한 아이템의 Ref 리스트에 담기  (한 지역에 여러 아이템이 중복으로 최대 30 개 정도 나온다고 보고 있음 )
	SI32 AvailableGainFishKindListRef[ 30 ];
	memset( AvailableGainFishKindListRef, 0, sizeof( AvailableGainFishKindListRef ));
	
	SI16 AvailableGainFishKindListRefCounter = 0;
	
	// 황금 낚시대를 착용하면, 스킬 제한없이 낚을 수 있다. - by LEEKH 2008-09-03
	//[진성] 이벤트 기간이 지나도 낚시대를 가지고 있으면 능력은 그대로 유지 => 2008-9-5
	if( ITEMUNIQUE(17160) == pclCI->clCharItem.clItem[ActiveWeapon].siUnique /*||
		pclClient->pclEventTimeManager->InEventTime(TEXT("Fishing_Event"), &pclserver->sTime)*/	)
	{
		// 스킬값에 10000이 들어가면 스킬은 체크하지 않는다.
		if ( ! pclClient->pclFishingMgr->GetAvailableGainFishKindListRef( siFishingBaitItemUnique, pclCI->clBI.GetX(), pclCI->clBI.GetY(),   10000, AvailableGainFishKindListRef, &AvailableGainFishKindListRefCounter ) )
		{
			return false;
		}
	}
	else
	{
		if ( ! pclClient->pclFishingMgr->GetAvailableGainFishKindListRef( siFishingBaitItemUnique, pclCI->clBI.GetX(), pclCI->clBI.GetY(), FishSkillLevel, AvailableGainFishKindListRef, &AvailableGainFishKindListRefCounter ) )
		{
			return false;
		}
	}

	if ( AvailableGainFishKindListRefCounter < 1 )
	{
		return false;
	}
	//}

	if ( pclClient->IsCountrySwitch(Switch_NewDurability) )
	{
		if ( 0 >= pclCI->clCharItem.clItem[ActiveWeapon].GetDurability() )
		{
			FishingStop();
			return false;
		}
	}
	
	SI32 RandSelectFishRef = AvailableGainFishKindListRef[ rand() % AvailableGainFishKindListRefCounter ];
	
	CFishKind *pFishKind = pclClient->pclFishingMgr->GetFishKindFromRef( RandSelectFishRef );
	
	if ( pFishKind == NULL )
	{
		return false;
	}
	
	SI16 EmptyPos = -1; 
	
	cltItem clFromItem;
	clFromItem.Init();
	clFromItem.siUnique = pFishKind->GetFishItemUnique();
	clFromItem.siItemNum = 1;

	///////////////////// 낚시를 통한 새로운 아이템 얻기///////////////////////
	bool getGodHandItem		= false;
	bool bGetGoldMineral	= false; 
	bool bGetBigItem		= false; 
	BOOL bGetValentainItem	= FALSE;

	if( pclClient->IsCountrySwitch(Switch_GodHandItem) )
	{
		SI32 fishingToolUnique = pclCI->clCharItem.clItem[ActiveWeapon].siUnique;
		// 갓핸드 낚시대
		if( ITEMUNIQUE(17200) == fishingToolUnique )
		{
			SI32 value = rand() % 1000;
			if( value < 30 )
			{
				getGodHandItem = true;
				clFromItem.siUnique = ITEMUNIQUE(8074); 
				clFromItem.siItemNum = CalcTiredPoint(1);
			}
		}
		else if( ITEMUNIQUE(17100) == fishingToolUnique ||
				 ITEMUNIQUE(17150) == fishingToolUnique )
		{
			// 자동 낚시는 확률이 더 낫다
			if( bAutoFishing )
			{
				SI32 value = rand() % 1000;
				if( value < 5 )
				{
					getGodHandItem = true;
					clFromItem.siUnique = ITEMUNIQUE(8074); 
					clFromItem.siItemNum = CalcTiredPoint(1);
				}
			}
			else
			{
				SI32 value = rand() % 1000;
				if( value < 15 )
				{
					getGodHandItem = true;
					clFromItem.siUnique = ITEMUNIQUE(8074); 
					clFromItem.siItemNum = CalcTiredPoint(1);
				}
			}
		}
		// 갓핸드 아이템을 얻지 못 하면 금광석을 주도록 한다.
		if( false == getGodHandItem )
		{
			SI32 siGold = 0;
			if(pclClient->IsCountrySwitch(Switch_BigItemModify))
			{
				//siGold = CatchFish_BigItem(FishSkillLevel);
				if(siGold > 0)
				{
					bGetBigItem = true;
					clFromItem.siUnique = siGold;
					clFromItem.siItemNum = 1;
				}
			}
			else
			{
				siGold = CatchFish_GoldMineral(FishSkillLevel);
				if(siGold > 0)
				{
					bGetGoldMineral = true;
					clFromItem.siUnique = siGold;
					clFromItem.siItemNum = 1;
				}
			}
		}
	}
	else
	{
		///////////////////// 1%의 확률로 금광석 획득/////////////////////// [영진]
		// 바다의 날엔 두배의확률로 금광석 획득
		SI32 siGold = 0;
		if(pclClient->IsCountrySwitch(Switch_BigItemModify))
		{
			//siGold = CatchFish_BigItem(FishSkillLevel);
			if(siGold > 0)
			{
				bGetBigItem = true;
				clFromItem.siUnique = siGold;
				clFromItem.siItemNum = 1;
			}
		}
		else
		{
			siGold = CatchFish_GoldMineral(FishSkillLevel);

			if(siGold > 0)
			{	
				bGetGoldMineral = true;
				clFromItem.siUnique = siGold;
				clFromItem.siItemNum = 1;
			}
		}
	}

	SI32 bigItemUnique = 0;
	SI32 siSpecial = 0;

	// 스위치가 필요 없어졌다.
	siSpecial = CatchFish_SpecialItem();

	if( siSpecial > 0 )
	{
		bGetValentainItem = TRUE;
		clFromItem.siUnique = siSpecial;
		bigItemUnique = siSpecial;
		clFromItem.siItemNum = 1;
	}

	cltItem clToItem;
	clToItem.Init();
	SI32 rtnvalue;
	
	// 아이템을 넣을 수 있는지 체크
	cltCharServer* pclchar = (cltCharServer* )pclCM->CR[OwnerCharID];
	if ( ! pclchar->CanAddInv(INVMODE_ALL, &clFromItem, &EmptyPos, &clToItem , &rtnvalue) )
	{
		((cltServer *)pclClient)->SendServerResponseMsg(0, SRVAL_ADDINV_FAIL_FULL, clFromItem.siUnique, 0, GetCharUnique());
		
		FishingStop();
		
		return false;
	}
	
	SI32 CatchPercent = ( rand() * rand() ) % 100000;		// 이번에 낚을 확률.
	SI32 RetVal = pFishKind->CatchFish( FishSkillLevel, CatchPercent, SpecialIncreaseSkillLevel );
	
	//[진성] 이벤트 기간이 지나도 낚시대를 가지고 있으면 능력은 그대로 유지 => 2008-9-5
	//if( pclClient->pclEventTimeManager->InEventTime(TEXT("Fishing_Event"), &pclserver->sTime) )
	{
		//[진성] 황금 낚시대. 100% 낚는다. => 2008-8-12
		if( ITEMUNIQUE(17160) == pclCI->clCharItem.clItem[ActiveWeapon].siUnique )
		{
			//RetVal = FISHING_SUCCESS;

			// 확금 낚시대는 실패하지 않는다. 최소한 한개는 지급한다. - by LEEKH 2008-09-03
			if(RetVal < FISHING_GET_GOLD)		RetVal = FISHING_SUCCESS;
		}
	}

	// 물품의 내구도를 감소시킨다. 
	//DecreaseSkillItemEquipDur(SKILL_FISH, false);

	// 사용했으므로 아이템 내구도 차감 - 확률 25%
	if ( pclClient->IsCountrySwitch(Switch_NewDurability) )
	{
		if( IsPC( GetCharUnique()) )
		{
			SI32 siRand = rand() % 100;
			if( siRand < 25 )
			{
				DecreaseEquipDur( ActiveWeapon,  1 );
				// 낚시스킬과 관련된 아이템의 내구도를 차감한다.
				DecreaseSkillItemEquipDur( SKILL_FISH );
			}

			if ( 0 >= pclCI->clCharItem.clItem[ActiveWeapon].GetDurability() )
			{
				UseItemReasonDurabilityZero( ActiveWeapon );
			}
		}
	}

	if ( RetVal < FISHING_SUCCESS )
	{
		cltItem clTempItem;
		clTempItem.Init();
		
		/*cltGameMsgResponse_FishingGetMarineProducts clGameMsgResponse_FishingGetMarineProducts
			(
			RetVal,
			pFishKind->GetFishItemUnique(),
			0,
			&clTempItem,
			GetKind()
			);
		cltMsg clMsg( GAMEMSG_RESPONSE_FISHINGGETMARINEPRODUCTS, sizeof( clGameMsgResponse_FishingGetMarineProducts ),(BYTE*)&clGameMsgResponse_FishingGetMarineProducts );
		pclClient->pclCM->CR[OwnerCharID]->SendNetMsg((sPacketHeader*)&clMsg );*/
		
		cltGameMsgResponse_FishingGetMarineProductsNotify clGameMsgResponse_FishingGetMarineProductsNotify
			(
			//KHY - 0512 -  피로도 추가.
			CalcTiredPoint(RetVal), 
			pFishKind->GetFishItemUnique(),
			0,
			GetCharUnique(),
			GetKind()
			);
		cltMsg clMsg1( GAMEMSG_RESPONSE_FISHINGGETMARINEPRODUCTSNOTIFY, sizeof( clGameMsgResponse_FishingGetMarineProductsNotify ),(BYTE*)&clGameMsgResponse_FishingGetMarineProductsNotify );
		SendNeighbourMsg( &clMsg1 , true);
	}
	else
	{
		// 성공했으니까 아이템 추가하고...
		// 아이템 추가할때 빈 공간 없음 실패 하겠지... 
		// DB 메시지 날려야지..
		cltItem clTempItem;
		clTempItem.siItemNum = RetVal;
		clTempItem.siUnique = pFishKind->GetFishItemUnique();
		
		// 금광석 아이템
		if(bGetGoldMineral)
		{
			clTempItem.siUnique = ITEMUNIQUE(5040);
			clTempItem.siItemNum = 1;
		}

		// 대박 아이템
		if( bGetBigItem )
		{
			clTempItem.siUnique = clFromItem.siUnique;
			clTempItem.siItemNum = clFromItem.siItemNum;
		}

		//KHY - 0115 발렌타인데이 이벤트.
		if( bGetValentainItem )
		{
			clTempItem.siUnique = bigItemUnique;
			clTempItem.siItemNum = 1;
		}

		// 바다의 날은 일본만 있다
		if( pclServer->bSpecailDay9switch == true )
		{
			clTempItem.siItemNum *= 2;
		}

		if( getGodHandItem )
		{
			clTempItem.siUnique = ITEMUNIQUE(8074); 
			clTempItem.siItemNum = CalcTiredPoint(1);
		}

		if( bGetBigItem == false && bGetGoldMineral== false && bGetValentainItem == false && getGodHandItem == false )
		{
			// 군주 데이 이벤트 아이템 획득추가
			SI32 Bonus = 1;
			if( pclserver->pclEventTimeManager->InEventTime("GoonzuDay_CollectionItem2_0", &pclserver->sTime) )
			{
				cltEventTime* pclEventTime = pclserver->pclEventTimeManager->GetEvent("GoonzuDay_CollectionItem2_0");
				if(pclEventTime)	{		Bonus = pclEventTime->m_siBonusAmount;		}
			}
			if( pclserver->pclEventTimeManager->InEventTime("GoonzuDay_CollectionItem2_3", &pclserver->sTime) )
			{
				cltEventTime* pclEventTime = pclserver->pclEventTimeManager->GetEvent("GoonzuDay_CollectionItem2_3");
				if(pclEventTime)	{		Bonus = pclEventTime->m_siBonusAmount;		}
			}

			if(pclClient->IsCountrySwitch(Switch_Server_Event))
			{
				//[진성] 서버 이벤트 하루 1등 서버이면 버프가 적용된다. => 2008-6-27
				// 버프 타입확인.
				if( pclClient->pclServerEventManager->GetBonusBufType() == CNServerEvent_Manager::SERVER_EVENT_BUF_ETC)
				{
					Bonus *= 2;
				}
			}

			clTempItem.siItemNum *= Bonus;
		}

		//[진성] 이벤트 기간이 지나도 낚시대를 가지고 있으면 능력은 그대로 유지 => 2008-9-5
		//if( pclClient->pclEventTimeManager->InEventTime(TEXT("Fishing_Event"), &pclserver->sTime) )
		{
			//[진성] 황금 낚시대. 낡은 상자 획득. => 2008-8-12
			if( ITEMUNIQUE(17160) == pclCI->clCharItem.clItem[ActiveWeapon].siUnique )
			{
				SI32 value = rand() % 100;
				bool bGiveBox = false;
				if( value < 10 )
				{
					bGiveBox = true;
					clTempItem.siUnique  = ITEMUNIQUE(13326); 
					clTempItem.siItemNum = 1;
				}
				value = rand() % 100;
				if( false == bGiveBox && value < 5 )
				{
					clTempItem.siItemNum *= 2;
				}
			}
			/* [영훈] 일반 낚시대에서는 낡은 상자 획득 못하도록 변경
			else	//[진성] 일반 낚시로도 0.5%로 낡은 상자 획득.
			{
				SI32 value = rand() % 1000;
				bool bGiveBox = false;
				if( value < 5 )
				{
					bGiveBox = true;
					clTempItem.siUnique  = ITEMUNIQUE(13326); 
					clTempItem.siItemNum = 1;
				}
			}
			*/
		}

		//KHY - 0512 -  피로도 추가.
		clTempItem.siItemNum = CalcTiredPoint(clTempItem.siItemNum);

		// 사람인 경우 기술 경험치 올려주면 됨
		if ( IsPerson )
		{	
			cltItem clToItem;
			clToItem.Init();
			SI32 rtnvalue;

			// 아이템을 넣을 수 있는지 체크
			if ( ! pclchar->CanAddInv(INVMODE_ALL, &clTempItem, &EmptyPos, &clToItem , &rtnvalue) )
			{
				((cltServer *)pclClient)->SendServerResponseMsg(0, SRVAL_ADDINV_FAIL_FULL, clTempItem.siUnique, 0, GetCharUnique());

				FishingStop();

				return false;
			}

			sDBRequest_FishingGetMarineProducts clMsg
				(
				pclClient->pclCM->CR[OwnerCharID]->pclCI->GetPersonID(),
				OwnerCharID,		
				EmptyPos,
				&clTempItem,
				clTempItem.IsCreateSID(pclServer->pclItemManager),
				SKILL_FISH,
				0,
				GetCharUnique(),
				GetKind()
				);
			pclServer->pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);

	
			cltCharServer *pclChar = (cltCharServer *)pclClient->pclCM->CR[OwnerCharID];
			if ( NULL == pclChar )
			{
				return false;
			}
			

			// 금광석을 줏어 먹은 경우에는 기술경험치 5
			// - 대박 아이템 추가 -
			if(bGetGoldMineral || bGetBigItem)
				pFishKind->m_stFishInfo.siIncFishingSkillExp = 5;

			// 바다의 날엔 어획 경험치 2배 			
			SI32 skillExp = pFishKind->m_stFishInfo.siIncFishingSkillExp;
			if( pclServer->bSpecailDay9switch == true )
			{
				skillExp *= 2;
			}

			pclChar->SetSkillUpdate( SKILL_FISH, skillExp );
			
			
			// [영훈] 퀘스트 아이템 증가
			if ( 0 < pclChar->siQuestCount )
			{
				clTempItem.siItemNum = pclChar->siQuestCount;
			}

			// 낚시퀘스트 중이라면
			IncQuestPara(QUEST_CLEAR_COND_FISH, clTempItem.siUnique, clTempItem.siItemNum);
			// 스킬 필요없는 낚시 퀘스트 중이라면
			IncQuestPara(QUEST_CLEAR_COND_FISHNOSKILL, clTempItem.siUnique, clTempItem.siItemNum);

			if ( pclserver->IsCountrySwitch(Switch_Server_Event) )
			{
				CNServerEvent_Quest* pclSE_Quest =  (CNServerEvent_Quest*)pclserver->pclServerEventManager->GetServerEvent_Quest();
				if ( pclSE_Quest )
				{
					pclSE_Quest->IncServerEventQuestCondition( SERVEREVENT_TYPE_FISH, clTempItem.siUnique, clTempItem.siItemNum );
				}
			}
		}
		else		// 소환수인 경우 ( 스킬 경험치가 아니라, 일반 경험치 올려줌 )
		{
			sDBRequest_FishingGetMarineProducts clMsg
				(
				pclClient->pclCM->CR[OwnerCharID]->pclCI->GetPersonID(),
				OwnerCharID,		
				EmptyPos,
				&clTempItem,
				clTempItem.IsCreateSID(pclServer->pclItemManager),
				SKILL_FISH,
				0,						// 소환수가 낚았으니까 기술 경험치 올려줄 필요 없음.
				GetCharUnique(),
				GetKind()
				);
			pclServer->pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);

			
			// 소환수 경험치 올려주는 부분
			// - 대박 아이템 추가 -
			if(bGetGoldMineral || bGetBigItem)
				pFishKind->m_stFishInfo.siIncFishingSkillExp = 5;

			IncreaseExp( (int)( pFishKind->m_stFishInfo.siIncFishingSkillExp * 1.5 ) );// SI32->SI64 경험치 오버플로우때문에 수정	
		}
	}
	
	return true;
	
}

SI32 cltCharServer::CatchFish_GoldMineral(SI32 FishSkillLevel)
{
	cltServer* pclServer = (cltServer*)pclClient;

	SI32 siGoldMineral = 0;

	// 중국일때는 0을 리턴 
	if( pclServer->siServiceArea == ConstServiceArea_NHNChina )	return siGoldMineral	;

	// 바다의 날엔 두배의확률로 금광석 획득
	if( pclServer->bSpecailDay9switch == true )
	{
		if( (pclClient->GetFrame() + rand())% 50 == 0  && FishSkillLevel >= 10)
		{
			siGoldMineral = ITEMUNIQUE(5040); 
		}
	}
	else
	{
		if( (pclClient->GetFrame() + rand())% 100 == 0  && FishSkillLevel >= 10)
		{
			siGoldMineral = ITEMUNIQUE(5040); 
		}
	}

	return siGoldMineral;
}

SI32 cltCharServer::CatchFish_SpecialItem()
{
	cltServer* pclServer = (cltServer*)pclClient;

	SI32 bigItemUnique = 0;

	NDate Today;
	Today.SetDate( pclServer->sTime.wYear, pclServer->sTime.wMonth, pclServer->sTime.wDay );

	//KHY - 0115 발렌타인데이 이벤트.
	if ( Today.GetDateVary() >= pclServer->clValentineEventStart.GetDateVary() &&
		Today.GetDateVary() <= pclServer->clValentineEventEnd.GetDateVary() )
	{	
		SI32 randValue = rand() % 100;

		if( randValue < pclServer->siValentineFish )
		{
			bigItemUnique = ITEMUNIQUE(13008);  //L 카드.
		}	
	}

	//KHY - 0218 화이트데이 이벤트.
	if ( Today.GetDateVary() >= pclServer->clWhiteEventStart.GetDateVary() &&
		Today.GetDateVary() <= pclServer->clWhiteEventEnd.GetDateVary() )
	{	
		SI32 randValue = rand() % 100;

		if( randValue < pclServer->siWhiteFish )
		{
			bigItemUnique = ITEMUNIQUE(13018);  //L 카드.
		}	
	}
	//////////////////////////////////////////////////////////////////////

	return bigItemUnique;
}

SI32 cltCharServer::CatchFish_BigItem(SI32 FishSkillLevel)
{
	cltServer* pclServer = (cltServer*)pclClient;

	REAL32 fConstantLuck = 7.0f;
	REAL32 fConstantSkill = 3.0f;

	SI32 siLuck = clPB.clTotalBA.GetLuk() * ( fConstantLuck + clPB.clTotalBA.GetLuk() / 100.0f ) 
				+ FishSkillLevel * ( fConstantSkill + FishSkillLevel / 100.0f );

    SI32 siRand = rand() % 20000;

	SI32 siBigItem = 0;

	if( siRand < siLuck )
	{
		siBigItem = ITEMUNIQUE(13328);
	}

	return siBigItem;
}

// 물고기 잡기를 시작한다.
bool cltCharServer::FishingStart( SI32 FishingBaitItemUnique )
{
	cltItem clItem;
	clItem.siItemNum = 1;
	clItem.siUnique = FishingBaitItemUnique;
	
	SI32 InvPos = pclCI->clCharItem.GetItemPos( FishingBaitItemUnique );
	if ( InvPos < 0 )
	{
		return false;
	}
	
	cltItem *pclFrom = &pclCI->clCharItem.clItem[ InvPos ];
	
	while( clItemUseReserveInfo.Push( clItem.siUnique, -clItem.siItemNum, InvPos ) == false )
	{
		ItemUseReserveAction();
	}
	
	// 클라이언트로 업데이트 된 정보를 보낸다. 
	if( pclFrom->siItemNum == 1 )
	{
		ItemUseReserveAction();
	}
	// pclchar->ItemUseReserveAction(); 밑에 있어야 한다. 
	// 그렇지 않으면 인벤에서 물품의 위치를 찾을 수 없다.!
	UseItem( clItem );		// 아이템 사용
	pclFrom->Decrease(1);
	// 클라이언트로 사용후 정보를 보낸다. 
	cltGameMsgResponse_PersonItemInfo clinfo( InvPos, PERSONITEMINFO_OPTION_NONE, 0, 0, pclFrom, 0);
	cltMsg clMsg(GAMEMSG_RESPONSE_PERSONITEMINFO, sizeof(clinfo), (BYTE*)&clinfo);
	SendNetMsg((sPacketHeader*)&clMsg);

	return true;
}

// 도구를 사용한다.
bool cltCharServer::GetFishingToolInfo( SI16 FishKindListRef, DBToolInfo *pFishingToolList )
{
	cltServer* pclserver = (cltServer*)pclClient;

	CFishKind *pFishKind = pclClient->pclFishingMgr->GetFishKindFromRef( FishKindListRef );
	
	if ( pFishKind == NULL )
	{
		return false;
	}

	SI32 *FishingTollItemUniqueListPtr = pFishKind->GetFishingToolItemUniqueListPtr();

	SI16 ItemPos = -1;
	SI32 Durability = 0;

	SI16 FishingToolCounter = 0;

	cltItem clTempItem;

	SI32 FishClassification = pFishKind->m_stFishInfo.siFishingRodAtb;

	SI32 NotExistFishingToolItemUnique = 0;
	SI32 NotUsingDurabilityFishingToolItemUnique = 0;
	SI32 FindClassification = 0;
	
	SI32 UseAbleDay	= 0;
	BOOL bUseAble	= FALSE;

	for ( SI16 i = 0; i < MAX_FISHING_TOOL_NUMBER; ++i )
	{
		if ( FishingTollItemUniqueListPtr[ i ] > 0 )
		{
			ItemPos = -1;
			Durability = 0;
			
			//ItemPos = pclCI->clCharItem.GetItemPos( FishingTollItemUniqueListPtr[ i ] );
			if ( pclCI->clCharItem.clItem[ ActiveWeapon ].siUnique == FishingTollItemUniqueListPtr[ i ] )
			{
				ItemPos = ActiveWeapon;
				bUseAble = pclserver->GetItemUseAbleDay(&pclCI->clCharItem.clItem[ ActiveWeapon ], &UseAbleDay);

				// 내구도가 0이면 낚시 못함
				if ( pclClient->IsCountrySwitch(Switch_NewDurability) )
				{
					if ( 0 >= pclCI->clCharItem.clItem[ActiveWeapon].GetDurability() )
					{
						bUseAble = FALSE;
					}
				}
			}
			else
			{
				ItemPos = -1;
			}
			
			// PCK : 낚시대의 사용기간이 지났으면 낚시를 안한다. (09.01.13)
			if(bUseAble == FALSE) continue;
			if(UseAbleDay < 0) return false;

			if ( ItemPos < 0 )
			{
				// 현재까지 이 아이템 속성에 대해서 찾은 것이 있음.
				if ( FindClassification & FishClassification )
				{
					continue;
				}
				
				if ( NotExistFishingToolItemUnique == 0 )
				{
					NotExistFishingToolItemUnique = FishingTollItemUniqueListPtr[ i ];
				}

				continue;
			}
			
			SI32 ref = pclClient->pclItemManager->FindItemRefFromUnique( pclCI->clCharItem.clItem[ItemPos].siUnique );
			if( ref <= 0 )
				return false;
			
			if ( FishClassification & pclClient->pclItemManager->pclItemInfo[ref]->siFishClassification )
			{
			/*	Durability = pclCI->clCharItem.clItem[ ItemPos ].GetDurability(  );
				
				if ( FISHING_TOOL_DURABILITY_DECREASE >= Durability )
				{
					if ( NotUsingDurabilityFishingToolItemUnique == 0 )
					{
						NotUsingDurabilityFishingToolItemUnique = FishingTollItemUniqueListPtr[ i ];
					}

					continue;
				}*/

				clTempItem.Set( &pclCI->clCharItem.clItem[ ItemPos ] );
								
			//	clTempItem.DecreaseDurability( pclClient->pclItemManager, FISHING_TOOL_DURABILITY_DECREASE );
				
				if ( clTempItem.siItemNum == 0 )
				{
					clTempItem.siUnique = pclCI->clCharItem.clItem[ ItemPos ].siUnique;
				}
				
				pFishingToolList[ FishingToolCounter ++ ].Set( ItemPos, &clTempItem );

				// 현재까지 찾은 분류를 제거한다.
				FishClassification -= pclClient->pclItemManager->pclItemInfo[ref]->siFishClassification;

				// 현재까지 찾은 분류를 추가한다.
				FindClassification +=pclClient->pclItemManager->pclItemInfo[ref]->siFishClassification;

				NotExistFishingToolItemUnique = 0;
				NotUsingDurabilityFishingToolItemUnique = 0;
				
				if ( FishClassification == 0 )
				{
					return true;
				}
			}
			else
			{
				continue;
			}
		}
	}	

	// 어떤 어구가 없어서 낚시를 할 수 없습니다.
	if ( NotExistFishingToolItemUnique > 0 )
	{
		((cltServer *)pclClient)->SendServerResponseMsg(0, SRVAL_FISHTOOLNOTEXIST, NotExistFishingToolItemUnique, 0, GetCharUnique());
	}

/*	// 어떤 어구의 내구도가 낮아서 낚시를 할 수 없습니다.
	if ( NotUsingDurabilityFishingToolItemUnique > 0 )
	{
		((cltServer *)pclClient)->SendServerResponseMsg(0, SRVAL_FISHTOOLNOTENOUGHDURABILITY, NotUsingDurabilityFishingToolItemUnique, 0, GetCharUnique());
	}
*/
	return false;
}

bool cltCharServer::IsAvailableFishing( SI32 FishingBaitItemUnique, SI16 FishKindListRef )
{
	cltServer* pclserver = (cltServer*)pclClient;

	if ( FishKindListRef < 0 )
	{
		return false;
	}

	// 미끼로 사용되는 아이템이 남아 있는지 체크
	SI32 BaitItemPos = pclCI->clCharItem.GetItemPos( FishingBaitItemUnique );

	if ( BaitItemPos < 0 )
	{
		return false;
	}

	CFishKind *pFishKind = pclClient->pclFishingMgr->GetFishKindFromRef( FishKindListRef );
	SI32 *FishingTollItemUniqueListPtr = pFishKind->GetFishingToolItemUniqueListPtr();

	SI16 ItemPos = -1;
	SI32 Durability = 0;

	SI32 FishClassification = pFishKind->m_stFishInfo.siFishingRodAtb;
	
	SI32 NotExistFishingToolItemUnique = 0;
	SI32 NotUsingDurabilityFishingToolItemUnique = 0;
	SI32 FindClassification = 0;
	
	SI32 UseAbleDay	= 0;
	BOOL bUseAble	= FALSE;

	for ( SI16 i = 0; i < MAX_FISHING_TOOL_NUMBER; ++i )
	{
		if ( FishingTollItemUniqueListPtr[ i ] > 0 )
		{
			ItemPos = -1;
			Durability = 0;
			
			//ItemPos = pclCI->clCharItem.GetItemPos( FishingTollItemUniqueListPtr[ i ] );			
			if ( pclCI->clCharItem.clItem[ ActiveWeapon ].siUnique == FishingTollItemUniqueListPtr[ i ] )
			{
				ItemPos = ActiveWeapon;
				bUseAble = pclserver->GetItemUseAbleDay(&pclCI->clCharItem.clItem[ ActiveWeapon ], &UseAbleDay);
			}
			else
			{
				ItemPos = -1;
			}
			
			// PCK : 낚시대의 사용기간이 지났으면 낚시를 안한다. (09.01.13)
			if(bUseAble == FALSE) continue;
			if(UseAbleDay < 0) return false;

			if ( ItemPos < 0 )
			{
				// 현재까지 이 아이템 속성에 대해서 찾은 것이 있음.
				if ( FindClassification & FishClassification )
				{
					continue;
				}
				
				if ( NotExistFishingToolItemUnique == 0 )
				{
					NotExistFishingToolItemUnique = FishingTollItemUniqueListPtr[ i ];
				}
				
				continue;
			}

			SI32 ref = pclClient->pclItemManager->FindItemRefFromUnique( pclCI->clCharItem.clItem[ItemPos].siUnique );
			if( ref <= 0 )
				return false;

			if ( FishClassification & pclClient->pclItemManager->pclItemInfo[ref]->siFishClassification )
			{
			/*	Durability = pclCI->clCharItem.clItem[ ItemPos ].GetDurability(  );
				
				if ( FISHING_TOOL_DURABILITY_DECREASE >= Durability )
				{
					if ( NotUsingDurabilityFishingToolItemUnique == 0 )
					{
						NotUsingDurabilityFishingToolItemUnique = FishingTollItemUniqueListPtr[ i ];
					}

					continue;
				}*/
				
				// 현재까지 찾은 분류를 제거한다.
				FishClassification -= pclClient->pclItemManager->pclItemInfo[ref]->siFishClassification;

				// 현재까지 찾은 분류를 추가한다.
				FindClassification +=pclClient->pclItemManager->pclItemInfo[ref]->siFishClassification;
				
				NotExistFishingToolItemUnique = 0;
				NotUsingDurabilityFishingToolItemUnique = 0;
				
				if ( FishClassification == 0 )
				{
					return true;
				}
			}
			else
			{
				continue;
			}
		}
	}

	// 어떤 어구가 없어서 낚시를 할 수 없습니다.
	if ( NotExistFishingToolItemUnique > 0 )
	{
		((cltServer *)pclClient)->SendServerResponseMsg(0, SRVAL_FISHTOOLNOTEXIST, NotExistFishingToolItemUnique, 0, GetCharUnique());
	}

/*	// 어떤 어구의 내구도가 낮아서 낚시를 할 수 없습니다.
	if ( NotUsingDurabilityFishingToolItemUnique > 0 )
	{
		((cltServer *)pclClient)->SendServerResponseMsg(0, SRVAL_FISHTOOLNOTENOUGHDURABILITY, NotUsingDurabilityFishingToolItemUnique, 0, GetCharUnique());
	}
*/	
	return false;
}

void cltCharServer::FishingStop()
{
	if ( bAutoFishing )
	{
		// 낚시 중단
		siFishingBaitItemUnique = 0;
		OrderForceStop( GetID(), BY_PLAYER );
		
		bAutoFishing = false;
		
		cltGameMsgResponse_AgricultureAutoGain clGameMsgResponse_AgricultureAutoGain( AGRICULTURE_TYPE_FISH, false );
		cltMsg clMsg( GAMEMSG_RESPONSE_AGRICULTUREAUTOGAIN, sizeof( clGameMsgResponse_AgricultureAutoGain ), (BYTE*)&clGameMsgResponse_AgricultureAutoGain );
		SendNetMsg((sPacketHeader*)&clMsg );
	}
	else
	{
		siFishingBaitItemUnique = 0;
		OrderForceStop( GetID(), BY_PLAYER );
	}

	return;
}
