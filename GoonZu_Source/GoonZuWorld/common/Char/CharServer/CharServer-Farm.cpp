#include "..\Common\CommonHeader.h"
#include "..\..\Server\Server.h"
#include "char\CharServer\Char-Server.h"

#include "..\..\Client\Music\Music.h"


#include "..\Common\Bullet\Bullet.h"
#include "..\Common\Order\Order.h"
#include "../CommonLogic/Land/Land.h"


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
#include <Msg/MsgType-Farming.h>
#include <Msg/MsgType-Agriculture.h>
#include "MsgRval-Define.h"

#include "../Common/Farming/FarmingMgr.h"

extern cltCommonLogic* pclClient;


#define FARMING_BIGWARITEM_COUNT		10000

// 수확
bool cltCharServer::FarmingAction( bool IsPerson )
{	
	cltServer *pclServer = (cltServer *)pclClient;
	if ( dwPreGetFarmingItemClock < 1 )
	{
		dwPreGetFarmingItemClock = pclClient->CurrentClock;
		return false;
	}

	// 농경은 야외지역에서만 가능.
	if ( pclCI->clBI.GetMapIndex() != 0 )	{			return false;		}

	SI32 OwnerCharID = 0;
	cltCharServer *OwnerChar;
	SI32 FarmSkillLevel = 0;
	SI16 SpecialIncreaseSkillLevel = 0;
	UI32 uiLeaseExpire = 0;
	
	// 사람 캐릭인 경우.
	if ( IsPC( GetCharUnique() ) )
	{	
		OwnerCharID = GetID();
		if( pclCM->IsValidID(OwnerCharID) == false )	return false;
		if( siFarmingItemUnique < 1 )					return false;
		if( m_pclAgriculture == NULL )					return false;

		OwnerChar = this;
		
		FarmSkillLevel = pclCI->clCharSkill.GetTotalSkillLevel( SKILL_FARM, 0, 0, &pclCI->clCharItem.clItem[0] );
		if ( FarmSkillLevel < 10 )
		{
			// 스킬제한없는 농경 퀘스트 중이라면 가능.
			SI16 siQuestType, siQuestUnique, siPara1, siPara2;
			SI16 siQuestClearCond = QUEST_CLEAR_COND_FARMNOSKILL;
			if( pclCI->clQuestInfo.IsQuestGoing(siQuestClearCond, &siQuestType, &siQuestUnique, &siPara1, &siPara2) == true )
			{
				// 농경술 레벨을 10으로 설정해준다.
				FarmSkillLevel = 10;
			}
		}

		// 농경술 익혀야 함.
		if ( FarmSkillLevel < 1 )			{			return false;		}

		// 건강 능력치 이상으로 농경을 할 수 없다.
		if( pclCI->clHealthInfo.uiOrg1Disease != 0 || pclCI->clHealthInfo.uiOrg2Disease != 0 ||
			pclCI->clHealthInfo.uiOrg3Disease != 0 || pclCI->clHealthInfo.uiOrg4Disease != 0 )
		{
			((cltServer *)pclClient)->SendServerResponseMsg(0, SRVAL_FARM_FAIL, CANFARM_ERORR_HAVEDISEASE ,0 , GetCharUnique() );

			OrderForceStop( GetID(), BY_PLAYER );
			return false;
		}

		// 30초마다 한번씩 남은 시간을 전송한다.
		SI32 leftTime = 0;
		if( OwnerChar->m_pclAgriculture )	uiLeaseExpire = m_pclAgriculture->GetExpireTime( m_pclAgriculture->GetCurrentVillageUnique() );

		if( m_clCharUpdateTimer[CHAR_UPDATETIMER_30SECOND].IsTimed(GetTickCount()) )		// 30초 리프레시
		{
			if ( uiLeaseExpire > 0 )
			{
				leftTime = uiLeaseExpire - pclClient->GetMinuteFromsTime(((cltServer*)pclClient)->sTime);

				cltGameMsgResponse_AgricultureLeaseLeftTime clInfo( RANKTYPE_LAND, leftTime );
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
		if( OwnerChar->GetCurrentOrder() != ORDER_FARM )
		{	
			OrderForceStop( GetID(), BY_PLAYER );
			return false;
		}

		SpecialIncreaseSkillLevel = pclClient->pclKindInfoSet->GetSubSkillLevel( GetKind(), SUMMON_JOB_FARMING );
		FarmSkillLevel = max(1, SpecialIncreaseSkillLevel + pclCI->clIP.GetLevel() / 5);		
		siFarmingItemUnique = OwnerChar->siFarmingItemUnique;

		if( OwnerChar->m_pclAgriculture )	uiLeaseExpire = OwnerChar->m_pclAgriculture->GetExpireTime( m_pclAgriculture->GetCurrentVillageUnique() );
	}

	// 더이상 채광을 할 수 있는지를 체크한다.
	if( OwnerChar->m_pclAgriculture == NULL )			return false;

	bool bLease = false;
	BOOL bFarmer = false;

	if( OwnerChar->m_pclAgriculture->GetCurrentVillageUnique() > 0 )
	{
		// 1. 임시 임대를 했는지?
		bLease = OwnerChar->m_pclAgriculture->IsLeaseAgriculture( OwnerChar->m_pclAgriculture->GetCurrentVillageUnique() );
		// 2. 건물에서 농장 임대를 했는지?
		bFarmer = pclClient->pclLandManager->IsFarmer( OwnerChar->m_pclAgriculture->GetCurrentVillageUnique(), OwnerChar->pclCI->GetPersonID() );
	}

	// 둘다 아니라면, 채광 스톱!!
	if( bLease == false && bFarmer == FALSE )
	{
		OwnerChar->FarmingStop();
		return false;
	}

	// 임시 임대만 있는것이 아니라면, 제한시간은 없다.
	if( !(bLease == true && bFarmer == FALSE) )			uiLeaseExpire = 0;

	SI32 delaytime = 40000;
	delaytime -= ((pclClient->pclItemManager->clItemStatistic.siItemPriceIndex / 100000 + 1 ) / 2 * 2) * 1000;
	delaytime = max( 20000, delaytime );

	if( TABS( pclClient->CurrentClock - dwPreGetFarmingItemClock ) >= delaytime )		
	{
		dwPreGetFarmingItemClock = pclClient->CurrentClock;

		// 내구도를 소모 시킨다
		if ( pclClient->IsCountrySwitch(Switch_NewDurability) )
		{
			if ( IsPC( GetCharUnique() ) )
			{
				// 물품의 내구도를 감소시킨다.
				SI32 siRand = rand() % 100;
				if( siRand < 10 )
				{
					DecreaseEquipDur( ActiveWeapon,  1 );
					DecreaseSkillItemEquipDur( SKILL_FARM );
				}

				if ( 0 >= pclCI->clCharItem.clItem[ActiveWeapon].GetDurability() )
				{
					UseItemReasonDurabilityZero( ActiveWeapon );
				}
			}
		}

		// 농경 성공/실패 여부를 판단
		SI32 rate = GetFarmRate( FarmSkillLevel );
		SI32 sirand = rand() % 100 + 1;

		if ( rate <= sirand )
		{
			SI32 leftTime = 0;
			if( uiLeaseExpire > 0 )		leftTime = uiLeaseExpire - pclClient->GetMinuteFromsTime(((cltServer*)pclClient)->sTime);
			
			cltGameMsgResponse_FarmingGetProductsNotify clGameMsgResponse_FarmingGetProductsNotify( OwnerCharID, siFarmingItemUnique );
			cltMsg clMsg( GAMEMSG_RESPONSE_FARMINGGETPRODUCTSNOTIFY, sizeof(clGameMsgResponse_FarmingGetProductsNotify), (BYTE*)&clGameMsgResponse_FarmingGetProductsNotify );
			((cltCharServer*)pclCM->CR[OwnerCharID])->SendNeighbourMsg( &clMsg , true);

			cltGameMsgResponse_AgricultureStart clGameMsgResponse_AgricultureStart( siFarmingItemUnique, rate, RANKTYPE_LAND, delaytime, leftTime );
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

	cltItem clFromItem;
	clFromItem.Init();
	clFromItem.siUnique = siFarmingItemUnique;

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
	SI32 siBigItem = 0;

	// 스위치가 필요 없어졌다.
	siBigItem = FarmingBigItem();
	
	if(siBigItem > 0)
	{
		bEventItemFlag = TRUE;

		clFromItem.siItemNum = 1;
		clFromItem.siUnique = siBigItem;
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
		pclOwner->FarmingStop();

		return false;
	}
	
	// 성공했으니까 아이템 추가하고...
	// 아이템 추가할때 빈 공간 없음 실패 하겠지... 
	// DB 메시지 날려야지..
	cltItem clTempItem;
	clTempItem.siItemNum = clFromItem.siItemNum;
	clTempItem.siUnique = siFarmingItemUnique;

	//KHY - 0115 발렌타인데이 이벤트.
	if( bEventItemFlag )
	{
		clTempItem.siUnique = clFromItem.siUnique;
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
			AGRICULTURE_TYPE_FARM
			);
		pclServer->pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);

		//[지연] - 2010신년이벤트 : 농경후 3% 확률로 1개의 포춘쿠키 조각 획득
		GiveFortuneCookiePiece( 30, 1 );

			
		cltCharServer *pclChar = (cltCharServer *)pclClient->pclCM->CR[OwnerCharID];
		pclChar->SetSkillUpdate( SKILL_FARM, pclClient->pclItemManager->GetFarmSkillExp( siFarmingItemUnique ) );
		
		if ( 0 < pclChar->siQuestCount )
		{
			clTempItem.siItemNum = pclChar->siQuestCount;
		}

		// 농경 퀘스트 중이라면
		IncQuestPara(QUEST_CLEAR_COND_FARM, clTempItem.siUnique, clTempItem.siItemNum);
		// 스킬 필요없는 농경 퀘스즈 중이라면
		IncQuestPara(QUEST_CLEAR_COND_FARMNOSKILL, clTempItem.siUnique, clTempItem.siItemNum);
		
		if ( pclServer->IsCountrySwitch(Switch_Server_Event) )
		{
			CNServerEvent_Quest* pclSE_Quest =  (CNServerEvent_Quest*)pclServer->pclServerEventManager->GetServerEvent_Quest();
			if ( pclSE_Quest )
			{
				pclSE_Quest->IncServerEventQuestCondition( SERVEREVENT_TYPE_FARM, clTempItem.siUnique, clTempItem.siItemNum );
			}
		}

		if (pclChar->pclCI->clBI.clBufInfo.IsHaveBuf(BUF_FARMINGNMININGFOREVER))
		{
			//소환 농경용 약을 먹었으면 그냥 패스 배가 안고프고 내장기관이 - 되지 않는다,
		}
		else{
			//cyj 농경시 신경기관 4회(FARMING_DECREASEORG_CNT)당 5(FARMING_DECREASEORG_AMT)씩 손상
			siFarmingActionCount++;
			if (siFarmingActionCount >= FARMING_DECREASEORG_CNT )
			{
				((cltCharServer *)pclClient->pclCM->CR[OwnerCharID])->MakeHunger();

				if(pclCI->clHealthInfo.DecreaseOrg(HEALTH_ORGAN_TYPE_ORG4, FARMING_DECREASEORG_AMT, pclCI->clIP.GetLevel(), true) == true)
				{
					SetUpdateSwitch(UPDATE_HEALTH, true, 0);		// 클라이언트에 업데이트 시킨다. 
				}
				siFarmingActionCount = 0;
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
			AGRICULTURE_TYPE_FARM
			);
		pclServer->pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);

			
		// 소환수 경험치 올려주는 부분
		IncreaseExp( 2 );
	}

	return true;
}

SI32 cltCharServer::FarmingBigItem()
{
	cltServer* pclServer = (cltServer*)pclClient;

	SI32 bigItemUnique = 0;

	NDate Today;
	Today.SetDate( pclServer->sTime.wYear, pclServer->sTime.wMonth, pclServer->sTime.wDay );

	if ( Today.GetDateVary() >= pclServer->clValentineEventStart.GetDateVary() &&
		Today.GetDateVary() <= pclServer->clValentineEventEnd.GetDateVary() )
	{		
		SI32 randValue = rand() % 100;

		if( randValue < pclServer->siValentineFarm) //농경 
		{
			// 랜덤하게 대박 아이템을 준다. 
			bigItemUnique =  ITEMUNIQUE(13014); // E2카드 
		}	
	}

	//KHY - 0218 화이트데이 이벤트.
	if ( Today.GetDateVary() >= pclServer->clWhiteEventStart.GetDateVary() &&
		Today.GetDateVary() <= pclServer->clWhiteEventEnd.GetDateVary() )
	{		
		SI32 randValue = rand() % 100;

		if( randValue < pclServer->siWhiteFarm) //농경 
		{
			// 랜덤하게 대박 아이템을 준다. 
			bigItemUnique =  ITEMUNIQUE(13019); // T카드 
		}	
	}

	return bigItemUnique;
}

SI32 cltCharServer::FarmingBigItem_New(SI32 FarmSkillLevel)
{
	cltServer* pclServer = (cltServer*)pclClient;

	REAL32 fConstantLuck = 7.0f;
	REAL32 fConstantSkill = 3.0f;

	SI32 siLuck = clPB.clTotalBA.GetLuk() * ( fConstantLuck + clPB.clTotalBA.GetLuk() / 100.0f ) 
				+ FarmSkillLevel * ( fConstantSkill + FarmSkillLevel / 100.0f );

    SI32 siRand = rand() % 20000;

	SI32 siBigItem = 0;

	if( siRand < siLuck )
	{
		siBigItem = ITEMUNIQUE(13326);
	}

	return siBigItem;
}

// 농경 스톱
void cltCharServer::FarmingStop()
{
	if ( bAutoFarming )
	{
		// 농경 중단
		siFarmingItemUnique = 0;
		OrderForceStop( GetID(), BY_PLAYER );
		
		bAutoFarming = false;
		
		cltGameMsgResponse_AgricultureAutoGain clGameMsgResponse_AgricultureAutoGain( AGRICULTURE_TYPE_FARM, false );
		cltMsg clMsg( GAMEMSG_RESPONSE_AGRICULTUREAUTOGAIN, sizeof( clGameMsgResponse_AgricultureAutoGain ),(BYTE*)&clGameMsgResponse_AgricultureAutoGain );
		SendNetMsg((sPacketHeader*)&clMsg );
	}
	else
	{
		siFarmingItemUnique = 0;
		OrderForceStop( GetID(), BY_PLAYER );
	}

	return;
}

SI32 cltAgriculture::FindVillageUnique( SI32 siVillageUnique )
{
	SI32 siSlot = -1;
	if( siVillageUnique <= 0 )				return siSlot;			// 안해도 되지만 속도 향상을 위해 코딩.

	for( SI32 i=0; i<MAX_AGRICULTURE_COUNT; i++ )
	{
		if( m_siVillageUnique[i] == siVillageUnique )	{		siSlot = i;		break;		}
	}

    return siSlot;
}

void cltAgriculture::AddAgriculture( SI32 siVillageUnique, UI32 uiExpireTime )
{
	SI32 siSlot = -1;
	SI32 i;

	for( i=0; i<MAX_AGRICULTURE_COUNT; i++ )
	{
		if( m_siVillageUnique[i] == siVillageUnique )
		{
			siSlot = i;
			break;
		}
	}

	// 기존에 등록되었던 마을이 없다면, 새로 빈곳을 찾는다.
	if( siSlot < 0 )
	{		
		for( i=0; i<MAX_AGRICULTURE_COUNT; i++ )
		{
			if( m_siVillageUnique[i] == 0 )
			{
				siSlot = i;
				break;
			}
		}
	}
	
	if( siSlot >= 0 )
	{
		m_siVillageUnique[siSlot] = siVillageUnique;
		m_uiExpireTime[siSlot] = uiExpireTime;
	}
}

void cltAgriculture::DeleteAgriculture( SI32 siVillageUnique )
{
	SI32 siSlot = FindVillageUnique( siVillageUnique );

	if( siSlot >= 0 )
	{
		m_siVillageUnique[siSlot] = 0;
		m_uiExpireTime[siSlot] = 0;
	}
}

UI32 cltAgriculture::GetExpireTime( SI32 siVillageUnique )
{
	SI32 siSlot = FindVillageUnique( siVillageUnique );

	if( siSlot >= 0 )
	{
		return m_uiExpireTime[siSlot];
	}

	return 0;
}

bool cltAgriculture::IsLeaseAgriculture( SI32 siVillageUnique )
{
	cltServer* pclserver = (cltServer*)pclClient;
	UI32 uiExpireTime = GetExpireTime( siVillageUnique );

	if( uiExpireTime > 0 )
	{
		if ( uiExpireTime > pclClient->GetMinuteFromsTime( pclserver->sTime )	)
		{
			return true;
		}
	}

	return false;
}

bool cltAgriculture::StartAgriculture( SI32 siVillageUnique, DWORD dwStartClock )
{
	m_siCurrentVillageUnique = siVillageUnique;
	m_dwStartAgricultureClock = dwStartClock;

	return true;
}

bool cltAgriculture::EndAgriculture( SI32 siVillageUnique )
{
	m_siCurrentVillageUnique = 0;
	m_dwStartAgricultureClock = 0;

	return true;
}
