#include "..\Common\CommonHeader.h"
#include "..\..\Server\Server.h"

#include "char\CharServer\Char-Server.h"

#include "..\..\Client\Music\Music.h"

#include "..\Common\Bullet\Bullet.h"
#include "..\Common\Order\Order.h"

#include "../CommonLogic/Mine/Mine.h"

#include "Util\Util.h"
#include "Skill\Skill-Manager.h"
#include "..\CharManager\CharManager.h"


#include "..\..\DBManager\GameDBManager_World\DBMsg-Item.h"
#include "..\..\DBManager\GameDBManager_World\DBMsg-Person.h"
#include "..\..\DBManager\GameDBManager_World\DBMsg-Agriculture.h"
#include <MsgType-Person.h>
#include <MsgType-Item.h>
#include <MsgType-System.h>
#include <Msg/MsgType-ItemMall.h>
#include <Msg/MsgType-Mining.h>
#include <Msg/MsgType-Agriculture.h>
#include "MsgRval-Define.h"

#include "../Common/Agriculture/Mining/MiningMgr.h"

extern cltCommonLogic* pclClient;


bool cltCharServer::MiningAction( bool IsPerson )
{
	if ( dwPreGetMiningItemClock < 1 )
	{
		dwPreGetMiningItemClock = pclClient->CurrentClock;
		return false;
	}

	cltServer *pclServer = (cltServer *)pclClient;

	// 채광은 야외지역에서만 가능.
	if ( pclCI->clBI.GetMapIndex() != 0 )						return false;

	SI32 OwnerCharID = 0;	
	cltCharServer *OwnerChar = NULL;

	SI32 MineSkillLevel = 0;		// 채광술 기술 체크
	SI16 SpecialIncreaseSkillLevel = 0;
	UI32 uiLeaseExpire = 0;
	// 사람 캐릭인 경우.
	if ( IsPC( GetCharUnique() ) )
	{	
		OwnerCharID = GetID();

		if(pclCM->IsValidID(OwnerCharID) == false)				return false;
		if ( siMiningItemUnique < 1 )							return false;

		OwnerChar = this;

		MineSkillLevel = pclCI->clCharSkill.GetTotalSkillLevel( SKILL_MINE, 0, 0, &pclCI->clCharItem.clItem[0] );
		if ( MineSkillLevel < 10 )
		{
			// 스킬제한없는 채광 퀘스트 중이라면 가능.
			SI16 questtype, questunique, para1, para2;
			SI16 questclearcond = QUEST_CLEAR_COND_MINENOSKILL;
			if(pclCI->clQuestInfo.IsQuestGoing(questclearcond, &questtype, &questunique, &para1, &para2) == true )
			{
				MineSkillLevel = 10;
			}
		}

		// 채광술 익혀야 함.
		if ( MineSkillLevel < 1 )								return false;

		if( pclCI->clHealthInfo.uiOrg1Disease != 0 || pclCI->clHealthInfo.uiOrg2Disease != 0 ||
			pclCI->clHealthInfo.uiOrg3Disease != 0 || pclCI->clHealthInfo.uiOrg4Disease != 0 )
		{
			((cltServer *)pclClient)->SendServerResponseMsg(0, SRVAL_MINE_FAIL, CANMINE_ERORR_HAVEDISEASE ,0 , GetCharUnique() );

			MiningStop();
			return false;
		}

		if( m_pclAgriculture )
		{
			uiLeaseExpire = m_pclAgriculture->GetExpireTime( m_pclAgriculture->GetCurrentVillageUnique() );
		}

		if( m_clCharUpdateTimer[CHAR_UPDATETIMER_30SECOND].IsTimed(GetTickCount()) )		// 30초 리프레시
		{
			SI32 leftTime = 0;
			if ( uiLeaseExpire > 0 )
			{
				leftTime = uiLeaseExpire - pclClient->GetMinuteFromsTime(((cltServer*)pclClient)->sTime);
				
				cltGameMsgResponse_AgricultureLeaseLeftTime clInfo( RANKTYPE_MINE, leftTime );
				cltMsg clMsg(GAMEMSG_RESPONSE_AGRICULTURELEASELEFTTIME, sizeof(clInfo), (BYTE*)&clInfo);
				((cltCharServer*)pclCM->CR[OwnerCharID])->SendNetMsg((sPacketHeader*)&clMsg);
			}
		}
	}
	else
	{
		OwnerCharID = pclClient->pclCM->GetIDFromCharUnique ( clIdentity.clSummonInfo.siParentCharUnique );
		if ( pclClient->pclCM->IsValidID( OwnerCharID ) == FALSE )						return false;

		OwnerChar = (cltCharServer*)pclClient->pclCM->CR[OwnerCharID];

		SpecialIncreaseSkillLevel = pclClient->pclKindInfoSet->GetSubSkillLevel( GetKind(), SUMMON_JOB_MINING );
		MineSkillLevel = max(1, SpecialIncreaseSkillLevel + pclCI->clIP.GetLevel() / 5);

		if ( OwnerChar->siMiningItemUnique < 1 )
		{
			MiningStop();
			return false;
		}
		else if ( OwnerChar->GetCurrentOrder() != ORDER_MINE )
		{
			MiningStop();
			return false;
		}

		siMiningItemUnique = OwnerChar->siMiningItemUnique;

		if( OwnerChar->m_pclAgriculture )
		{
			uiLeaseExpire = OwnerChar->m_pclAgriculture->GetExpireTime( OwnerChar->m_pclAgriculture->GetCurrentVillageUnique() );
		}
	}

	// 더이상 채광을 할 수 있는지를 체크한다.
	if( OwnerChar->m_pclAgriculture == NULL )			return false;

	bool bLease = false;
	BOOL bMiner = false;

	if( OwnerChar->m_pclAgriculture->GetCurrentVillageUnique() > 0 )
	{
		// 1. 임시 임대를 했는지?
		bLease = OwnerChar->m_pclAgriculture->IsLeaseAgriculture( OwnerChar->m_pclAgriculture->GetCurrentVillageUnique() );
		// 2. 건물에서 광산 임대를 했는지?
		bMiner = pclClient->pclMineManager->IsMiner( OwnerChar->m_pclAgriculture->GetCurrentVillageUnique(), OwnerChar->pclCI->GetPersonID() );
	}

	// 둘다 아니라면, 채광 스톱!!
	if( bLease == false && bMiner == FALSE )
	{
		MiningStop();
		return false;
	}

	// 임시 임대만 있는것이 아니라면, 제한시간은 없다.
	if( !(bLease == true && bMiner == FALSE) )			uiLeaseExpire = 0;

	SI32 delaytime = 60000;
	delaytime -= ((pclClient->pclItemManager->clItemStatistic.siItemPriceIndex / 100000 + 1 ) / 2 * 2) * 1000;
	delaytime = max( 40000, delaytime );

	if ( TABS( pclClient->CurrentClock - dwPreGetMiningItemClock ) >= delaytime )
	{
		dwPreGetMiningItemClock = pclClient->CurrentClock;

		// 내구도를 감소 시킨다
		if ( pclClient->IsCountrySwitch(Switch_NewDurability) )
		{
			if ( IsPC( GetCharUnique() ) )
			{
				// 물품의 내구도를 감소시킨다.
				SI32 siRand = rand() % 100;
				if( siRand < 10 )
				{
					DecreaseEquipDur( ActiveWeapon,  1 );
					DecreaseSkillItemEquipDur( SKILL_MINE );
				}

				if ( 0 >= pclCI->clCharItem.clItem[ActiveWeapon].GetDurability() )
				{
					UseItemReasonDurabilityZero( ActiveWeapon );
				}
			}
		}

		SI32 rate = GetMineRate( MineSkillLevel );
		SI32 sirand = rand() % 100 + 1;

		// 채광 실패!!
		if ( rate <= sirand )
		{
			SI32 leftTime = 0;
			if( uiLeaseExpire > 0 )		leftTime = uiLeaseExpire - pclClient->GetMinuteFromsTime(((cltServer*)pclClient)->sTime);

			cltGameMsgResponse_MiningGetProductsNotify clGameMsgResponse_MiningGetProductsNotify( OwnerCharID, siMiningItemUnique );
			cltMsg clMsg( GAMEMSG_RESPONSE_MININGGETPRODUCTSNOTIFY, sizeof(clGameMsgResponse_MiningGetProductsNotify), (BYTE*)&clGameMsgResponse_MiningGetProductsNotify );
			((cltCharServer*)pclCM->CR[OwnerCharID])->SendNeighbourMsg( &clMsg , true);

			cltGameMsgResponse_AgricultureStart clGameMsgResponse_AgricultureStart( siMiningItemUnique, rate, RANKTYPE_MINE, delaytime, leftTime );
			cltMsg clMsg1( GAMEMSG_RESPONSE_AGRICULTURESTART, sizeof(clGameMsgResponse_AgricultureStart), (BYTE*)&clGameMsgResponse_AgricultureStart );
			pclCM->CR[OwnerCharID]->SendNetMsg((sPacketHeader*)&clMsg1);

			return false;
		}
	}
	else
	{
		return false;
	}

	SI16 EmptyPos = -1; 

	SI16 siNearVillageUnique = 0;
	// 주어진 좌표로부터 가까운 마을을 찾는다. 
	SI32 XPos = pclCM->CR[OwnerCharID]->GetX();
	SI32 YPos = pclCM->CR[OwnerCharID]->GetY();
	siNearVillageUnique = pclServer->pclVillageManager->FindNearVillage( XPos, YPos );

	// 채광 할때 랜덤으로 광물을 정한다.
	cltItem clFromItem;
	clFromItem.Init();
	if( pclClient->IsCountrySwitch( Switch_FishFarmMineChange ) )
	{
		siMiningItemUnique = pclServer->pclMiningMgr->GetMineItemUniqueRandom( siNearVillageUnique );
	}
	else
	{
	}
	
	clFromItem.siUnique = siMiningItemUnique;

	DWORD dwStartAgricultureClock = 0;
	if( OwnerChar->m_pclAgriculture )	dwStartAgricultureClock = OwnerChar->m_pclAgriculture->GetStartAgricultureClock();
	if( dwStartAgricultureClock <= 0 )				return false;
	
	DWORD dwPastHour = TABS( pclClient->CurrentClock - dwStartAgricultureClock ) / 3600000;
	
	switch ( dwPastHour )
	{
	case 0:
		clFromItem.siItemNum = rand()%3 + 1;
		break;
	case 1:
		clFromItem.siItemNum = rand()%2 + 1;
		break;
	case 2:
		clFromItem.siItemNum = rand()%2;
		break;
	default:
		return false;
	}

	BOOL bEventItemFlag = FALSE;
	SI32 bigItemUnique = 0;

	// 스위치 삭제.
	bigItemUnique = MiningBigItem( );

	if( bigItemUnique > 0 )
	{
		bEventItemFlag = TRUE;

		clFromItem.siItemNum = 1;
		clFromItem.siUnique = bigItemUnique;
	}
	else
	{
		// 군주 데이 이벤트 아이템 획득추가
		SI32 Bonus = 1;
		if( pclServer->pclEventTimeManager->InEventTime("GoonzuDay_CollectionItem2_0", &pclServer->sTime) )
		{
			cltEventTime* pclEventTime = pclServer->pclEventTimeManager->GetEvent("GoonzuDay_CollectionItem2_0");
			if(pclEventTime)	{		Bonus = pclEventTime->m_siBonusAmount;		}
		}
		if( pclServer->pclEventTimeManager->InEventTime("GoonzuDay_CollectionItem2_3", &pclServer->sTime) )
		{
            cltEventTime* pclEventTime = pclServer->pclEventTimeManager->GetEvent("GoonzuDay_CollectionItem2_3");
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

		clFromItem.siItemNum *= Bonus;
	}

	cltItem clToItem;
	clToItem.Init();
	SI32 rtnvalue;

	// 아이템을 넣을 수 있는지 체크
	cltCharServer* pclchar = (cltCharServer* )pclCM->CR[OwnerCharID];
	if ( ! pclchar->CanAddInv(INVMODE_ALL, &clFromItem, &EmptyPos, &clToItem , &rtnvalue) )
	{
		((cltServer *)pclClient)->SendServerResponseMsg(0, SRVAL_ADDINV_FAIL_FULL, clFromItem.siUnique, 0, pclClient->pclCM->CR[OwnerCharID]->GetCharUnique());

		cltCharServer *pclOwner = (cltCharServer *)pclClient->pclCM->CR[OwnerCharID];
		pclOwner->MiningStop();

		return false;
	}

	// 성공했으니까 아이템 추가하고...
	// 아이템 추가할때 빈 공간 없음 실패 하겠지... 
	// DB 메시지 날려야지..
	cltItem clTempItem;
	clTempItem.siItemNum = clFromItem.siItemNum;
	clTempItem.siUnique = siMiningItemUnique;
	
	if( bEventItemFlag )
	{			
		clTempItem.siUnique = bigItemUnique;
		clTempItem.siItemNum = 1;
	}

	//KHY - 0512 -  피로도 추가.
	clTempItem.siItemNum = CalcTiredPoint(clTempItem.siItemNum);

	// 사람인 경우 기술 경험치 올려주면 됨
	if ( IsPerson )
	{	
		sDBRequest_AgricultureGetProducts clMsg
			( pclClient->pclCM->CR[OwnerCharID]->pclCI->GetPersonID(),
			OwnerCharID,
			EmptyPos,
			&clTempItem, clTempItem.IsCreateSID(pclServer->pclItemManager),
			GetCharUnique(),
			GetKind(),
			AGRICULTURE_TYPE_MINE
			);
		pclServer->pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);

		//[지연] - 2010신년이벤트 : 채광후 3% 확률로 1개의 포춘쿠키 조각 획득
		GiveFortuneCookiePiece( 30, 1 );

		cltCharServer *pclChar = (cltCharServer *)pclClient->pclCM->CR[OwnerCharID];
		pclChar->SetSkillUpdate( SKILL_MINE, pclClient->pclItemManager->GetMineSkillExp( siMiningItemUnique ) );

		if ( 0 < pclChar->siQuestCount )		// QuestCount에 의한 Para값 증가
		{
			clTempItem.siItemNum = pclChar->siQuestCount;
		}

		//채광 퀘스트 중이라면
		IncQuestPara(QUEST_CLEAR_COND_MINE, clTempItem.siUnique, clTempItem.siItemNum);
		IncQuestPara(QUEST_CLEAR_COND_MINENOSKILL, clTempItem.siUnique, clTempItem.siItemNum );

		if ( pclServer->IsCountrySwitch(Switch_Server_Event) )
		{
			CNServerEvent_Quest* pclSE_Quest =  (CNServerEvent_Quest*)pclServer->pclServerEventManager->GetServerEvent_Quest();
			if ( pclSE_Quest )
			{
				pclSE_Quest->IncServerEventQuestCondition( SERVEREVENT_TYPE_MINE, clTempItem.siUnique, clTempItem.siItemNum );
			}
		}

		if (pclChar->pclCI->clBI.clBufInfo.IsHaveBuf(BUF_FARMINGNMININGFOREVER))
		{
			//소환 농경용 약을 먹었으면 그냥 패스 배가 안고프고 내장기관이 - 되지 않는다,
		}
		else
		{
			//cyj 채광시 호흡기관 2회(MINING_DECREASEORG_CNT)당 5(MINING_DECREASEORG_AMT)씩 손상
			siMiningActionCount++;
			if (siMiningActionCount >= MINING_DECREASEORG_CNT )
			{
				((cltCharServer *)pclClient->pclCM->CR[OwnerCharID])->MakeHunger();

				if(pclCI->clHealthInfo.DecreaseOrg(HEALTH_ORGAN_TYPE_ORG3, MINING_DECREASEORG_AMT, pclCI->clIP.GetLevel(), true) == true)
				{
					SetUpdateSwitch(UPDATE_HEALTH, true, 0);		// 클라이언트에 업데이트 시킨다. 
				}

				siMiningActionCount = 0;
			}
		}
	}
	else		// 소환수인 경우 ( 스킬 경험치가 아니라, 일반 경험치 올려줌 )
	{
		sDBRequest_AgricultureGetProducts clMsg
			(
			pclClient->pclCM->CR[OwnerCharID]->pclCI->GetPersonID(),
			OwnerCharID,		
			EmptyPos,
			&clTempItem, clTempItem.IsCreateSID(pclServer->pclItemManager),
			GetCharUnique(),
			GetKind(),
			AGRICULTURE_TYPE_MINE
			);
		pclServer->pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);

	
		// 소환수 경험치 올려주는 부분
		IncreaseExp( 2 );
	}

	return true;
}

SI32 cltCharServer::MiningBigItem()
{
	cltServer* pclServer = (cltServer*)pclClient;

	SI32 bigItemUnique = 0; // N2카드

	NDate Today;
	Today.SetDate( pclServer->sTime.wYear, pclServer->sTime.wMonth, pclServer->sTime.wDay );

	if ( Today.GetDateVary() >= pclServer->clValentineEventStart.GetDateVary() &&
		Today.GetDateVary() <= pclServer->clValentineEventEnd.GetDateVary() )
	{
		SI32 randValue = rand() % 100;

		if( randValue < pclServer->siValentineMine)
		{
			// 랜덤하게 대박 아이템을 준다.
			bigItemUnique = ITEMUNIQUE(13013);
		}
	}

	//KHY - 0218 화이트데이 이벤트.
	if ( Today.GetDateVary() >= pclServer->clWhiteEventStart.GetDateVary() &&
		Today.GetDateVary() <= pclServer->clWhiteEventEnd.GetDateVary() )
	{
		SI32 randValue = rand() % 100;

		if( randValue < pclServer->siWhiteMine)
		{
			// 랜덤하게 대박 아이템을 준다. 
			bigItemUnique = ITEMUNIQUE(13019); // T카드
		}
	}

	return bigItemUnique;
}

SI32 cltCharServer::MiningBigItem_New( SI32 MineSkillLevel )
{
	cltServer* pclServer = (cltServer*)pclClient;

	REAL32 fConstantLuck = 7.0f;
	REAL32 fConstantSkill = 3.0f;

	SI32 siLuck = clPB.clTotalBA.GetLuk() * ( fConstantLuck + clPB.clTotalBA.GetLuk() / 100.0f ) 
				+ MineSkillLevel * ( fConstantSkill + MineSkillLevel / 100.0f );

	SI32 siRand = rand() % 20000;

	SI32 siBigItem = 0;

	if( siRand < siLuck )
	{
		siBigItem = ITEMUNIQUE(13327);
	}

	return siBigItem;
}

void cltCharServer::MiningStop()
{
	if ( bAutoMining )
	{
		// 농경 중단
		siMiningItemUnique = 0;
		OrderForceStop( GetID(), BY_PLAYER );

		bAutoMining = false;

		cltGameMsgResponse_AgricultureAutoGain clGameMsgResponse_AgricultureAutoGain( AGRICULTURE_TYPE_MINE, false );
		cltMsg clMsg( GAMEMSG_RESPONSE_AGRICULTUREAUTOGAIN, sizeof( clGameMsgResponse_AgricultureAutoGain ), (BYTE*)&clGameMsgResponse_AgricultureAutoGain );
		SendNetMsg( (sPacketHeader*)&clMsg );
	}
	else
	{
		siMiningItemUnique = 0;
		OrderForceStop( GetID(), BY_PLAYER );
	}

	return;
}

bool cltCharServer::IsLeaseAgriculture( SI32 villageUnique )
{
	return false;
}
