//---------------------------------
// 2003/9/15 김태곤
//---------------------------------

#include "..\Common\CommonHeader.h"
#include "..\Server\Server.h"

#include "..\..\..\DBManager\GameDBManager_World\DBMsg.h"
#include "..\..\..\DBManager\GameDBManager_World\DBMsg-Person.h"
#include "..\..\..\DBManager\GameDBManager_World\DBMsg-Item.h"

//------------------------------------
// CommonLogic
//------------------------------------
#include "Msg\MsgType-Horse.h"
#include "MsgType-Person.h"


//-------------------------------------
// Common
//-------------------------------------
#include "PersonInfo.h"
#include "Char\CharManager\CharManager.h"

// 말을 타라는 요청에 대한 처리. 
void cltServer::DoMsg_GAMEMSG_REQUEST_RIDEHORSE(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgRequest_RideOrUnrideHorse* pclinfo = (cltGameMsgRequest_RideOrUnrideHorse*)pclMsg->cData;
	cltCharServer* pclchar = (cltCharServer*)pclCM->CR[id];

	SI32 personid =pclchar->pclCI->GetPersonID();
	if(personid <= 0)return ;

	cltHorse* pclhorse = pclchar->pclCI->clHorseInfo.GetCurrentShowHorse();

//	if(pclinfo->siHorseIndex < 0 || pclinfo->siHorseIndex >= 1)return ;
	//KHY - 1001 - 기승동물 5마리 보유 수정.
	if(pclinfo->siHorseIndex < 0 )return ;

	// 말이 있는지 확인한다. 
	if(pclchar->pclCI->clHorseInfo.GetCurrentShowHorse() == NULL)	return ;

	// 말이 탈 수 있는 상태인지 확인한다. (2살인지 확인한다.) 
	if(pclchar->pclCI->clHorseInfo.CanRide(pclinfo->siHorseIndex, &pclTime->clDate) == false)return ;

	// 변신중일때는 탈 수 없다.
	if(pclchar->clTransFormInfo.bTransFormMode == true) return;

	//KHY - 1111 - 오토바이 추가.		
	if(pclhorse->siHorseUnique == HORSEUNIQUE_MOTORCYCLE)
	{
		// 오토바이에 타기위해 필요한 종량제 시스템 확인
		if( pclchar->pclCI->clBI.clTimeMeterRateInfo.GetLeftTime( TIMEMETERRATE_MOTORCYCLE ) <= 0)
		{
			// 연료가 부족합니다.충전해 주십시요.
			return;
		}
		else
		{
			// 오토바이에 타면 종량제 시작.
			if( pclchar->pclCI->clBI.clTimeMeterRateInfo.UseStart( TIMEMETERRATE_MOTORCYCLE ) == false)
			{
				return;
			}
		}

		// 효과가 있다면 여기서 넣어준다.
	}	

	// DB에 말을 탔음을 알린다. 
	sDBRequest_SetRideHorse cldbmsg(id, personid, pclinfo->siHorseIndex , true);
	pclGameDBProcess->SendMsg((sPacketHeader *)&cldbmsg);

}

// 말에서 내리라는 요청에 대한 처리
void cltServer::DoMsg_GAMEMSG_REQUEST_UNRIDEHORSE(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_RideOrUnrideHorse * pclinfo = (cltGameMsgResponse_RideOrUnrideHorse *)pclMsg->cData;
	cltCharServer* pclchar = (cltCharServer*)pclCM->CR[id];

	SI32 personid = pclchar->pclCI->GetPersonID();

	cltHorse* pclhorse = pclchar->pclCI->clHorseInfo.GetCurrentShowHorse();

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

	// 변신중일때는 내릴 수 없다.
	if(pclchar->clTransFormInfo.bTransFormMode == true) return;

	// 대상자에게 말을 태운다.
	if( pclCM->CR[id]->UnrideHorse() == false)return ;
	if(personid <= 0)return ;

	//KHY - 1111 - 오토바이 추가.		
	if(pclhorse->siHorseUnique == HORSEUNIQUE_MOTORCYCLE)
	{
		cltTimeMeterRate* pclTimeMeterRate = pclchar->pclCI->clBI.clTimeMeterRateInfo.GetTimeMeterRate(TIMEMETERRATE_MOTORCYCLE);

		if(pclTimeMeterRate != NULL)
		{
			if( pclTimeMeterRate->IsUse() )
			{
				// DB에 사용시간 차감을 보냄
				sDBRequest_TimeMeterRate_UseTime clMsg(pclchar->GetCharID(), pclchar->pclCI->GetPersonID(), TIMEMETERRATE_MOTORCYCLE, pclTimeMeterRate->siUseTimeSec, true );
				SendDBMsg( DBSELECT_BASE, (sPacketHeader*)&clMsg );

				// 사용 종료
				pclchar->pclCI->clBI.clTimeMeterRateInfo.UseEnd( TIMEMETERRATE_MOTORCYCLE );
			}			
		}
	}

	// DB에 말을 탔음을 알린다. 
	sDBRequest_SetRideHorse cldbmsg(id, personid, pclinfo->siHorseIndex , false);
	pclGameDBProcess->SendMsg((sPacketHeader *)&cldbmsg);
}

void cltServer::DoMsg_GAMEMSG_REQUEST_HORSENAMECHANGE(cltMsg* pclMsg, SI32 id)
{	
	SI32 PersonID = pclCM->CR[id]->pclCI->GetPersonID();
	if( PersonID < 1 )
		return ;

	cltGameMsgRequest_HorseNameChange *pclinfo = (cltGameMsgRequest_HorseNameChange *)pclMsg->cData;

/*
	// 4월 28일 기준으로 말은 1마리 밖에 없음.
	if ( pclinfo->siHorseIndex != 0 )
	{
		return;
	}
*/
	if ( pclinfo->strHorseName == NULL || _tcslen( pclinfo->strHorseName ) < 1 )
	{
		return;
	}

	sDBRequest_HorseNameChange clmsg(id, PersonID, pclinfo->siHorseIndex, pclinfo->strHorseName );
	pclGameDBProcess->SendMsg( (sPacketHeader *)&clmsg );

	return;
}

void cltServer::DoMsg_GAMEMSG_REQUEST_HORSEINIT(cltMsg* pclMsg, SI32 id)
{
	if( pclCM->IsValidID( id ) == FALSE )
	{
		return;
	}

	cltCharServer *pclChar = (cltCharServer *)pclCM->CR[id];

	cltHorse* pclHorse = pclChar->pclCI->clHorseInfo.GetCurrentShowHorse();
	if ( pclHorse == NULL || pclHorse->siHorseUnique <= 0 )			return ;

	SI32 itemunique = 14070 ; // 말상태초기화이용권

	if ( pclChar->pclCI->clCharItem.GetItemNum(itemunique) <= 0 )
		return ;

	SI32 pos = pclChar->pclCI->clCharItem.GetItemPos(itemunique);
	if(pos < 0)return ;

	//------------------------------------
	// 말스테이터스를 초기화한다.
	//------------------------------------
	SI32 siTotalStatus = pclHorse->siMoveSpeed +
						 pclHorse->siStr +
						 pclHorse->siDex +
						 pclHorse->siMag ;

	pclHorse->siStatusBonus += ( siTotalStatus / 100 ) ;

	if ( siTotalStatus - ( siTotalStatus / 100 ) * 100 )
		pclHorse->siStatusBonus ++ ;

	SI32 maxbonus = 0 ;

	if ( pclHorse->siHorseUnique == 9 )
	{
		maxbonus = ( MAX_HORSE_SPEED+MAX_BEAST_STR+MAX_HORSE_DEX+MAX_HORSE_MAG ) / 100 ;
	}
	else
	{
		maxbonus = ( MAX_HORSE_SPEED+MAX_HORSE_STR+MAX_HORSE_DEX+MAX_HORSE_MAG ) / 100 ;
	}

	pclHorse->siStatusBonus = min(maxbonus, pclHorse->siStatusBonus);

	pclHorse->siMoveSpeed = 0 ;
	pclHorse->siStr = 0 ;
	pclHorse->siDex = 0 ;
	pclHorse->siMag = 0 ;

	// DB에 저장한다. 
	pclChar->UpdateHorseToDB(this, pclChar->pclCI->clHorseInfo.siCurrentShowHorse, pclHorse, SETHORSE_REASON_UPDATE);

	pclChar->SetUpdateSwitch(UPDATE_HORSE, true, 0);

	cltItem *pclItem = &pclChar->pclCI->clCharItem.clItem[pos];
	pclItem->siItemNum = 1 ;

	// 통계를 낸다. 
	pclItemManager->AddUseNum(itemunique, 1);

	// 새로운 통계 - 아이템 사용
	//NStatistics_Add("Item", itemunique, "UseNum", 1);

	sDBRequest_UseItem clMsg(id,pclChar->pclCI->GetPersonID(),pos,pclItem,USEITEM_REASON_TICKET);
	SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg);

	return ;
}

void cltServer::DoMsg_GAMEMSG_REQUEST_USEHORSE_BONUS(cltMsg* pclMsg, SI32 id)
{
	if( pclCM->IsValidID( id ) == FALSE )
	{
		return;
	}

	cltCharServer *pclChar = (cltCharServer *)pclCM->CR[id];

	cltHorse* pclHorse = pclChar->pclCI->clHorseInfo.GetCurrentShowHorse();
	if ( pclHorse == NULL || pclHorse->siHorseUnique <= 0 )			return ;

	// 보너스가 남아 있어야 쓸 수 있다.
	if( pclHorse->siStatusBonus <= 0)								return;

	// 캐릭터의 IP를 클라이언트로 보낼 수 있게 한다. 
	pclChar->SetUpdateSwitch(UPDATE_IP, true, 0);

	SI32 type = *pclMsg->cData;
	switch(type)
	{
	case 0:
		{
			if ( pclHorse->siMoveSpeed >= pclHorseManager->GetFullSpeed(pclHorse->siHorseUnique) )
				return ;

			pclHorse->siMoveSpeed += 100 ;
			pclHorse->siMoveSpeed = min(pclHorseManager->GetFullSpeed(pclHorse->siHorseUnique), pclHorse->siMoveSpeed);

			pclHorse->siStatusBonus -- ;
		}
		break;
	case 1:
		{
			if ( pclHorse->siStr >= pclHorseManager->GetFullStr(pclHorse->siHorseUnique) )
				return ;

			pclHorse->siStr += 100 ;
			pclHorse->siStr = min(pclHorseManager->GetFullStr(pclHorse->siHorseUnique), pclHorse->siStr);

			pclHorse->siStatusBonus -- ;
		}
		break;
	case 2:
		{
			if ( pclHorse->siDex >= pclHorseManager->GetFullDex(pclHorse->siHorseUnique) )
				return ;

			pclHorse->siDex += 100 ;
			pclHorse->siDex = min(pclHorseManager->GetFullDex(pclHorse->siHorseUnique), pclHorse->siDex);

			pclHorse->siStatusBonus -- ;
		}
		break;
	case 3:
		{
			if ( pclHorse->siMag >= pclHorseManager->GetFullMag(pclHorse->siHorseUnique) )
				return ;

			pclHorse->siMag += 100 ;
			pclHorse->siMag = min(pclHorseManager->GetFullMag(pclHorse->siHorseUnique), pclHorse->siMag);

			pclHorse->siStatusBonus -- ;
		}
		break;
	default:
		return ;
	}

	//변경된 정보를 DB에 저장한다. 
	pclChar->UpdateHorseToDB(this, pclChar->pclCI->clHorseInfo.siCurrentShowHorse, pclHorse, SETHORSE_REASON_UPDATE);

	pclChar->SetUpdateSwitch(UPDATE_HORSE, true, 0);

	return ;
}

void cltServer::DoMsg_GAMEMSG_REQUEST_CHANGE_HORSETYPE(cltMsg* pclMsg,SI32 id)
{
	if( pclCM->IsValidID( id ) == FALSE )
	{
		return;
	}

	cltCharServer *pclChar = (cltCharServer *)pclCM->CR[id];

	cltHorse* pclHorse = pclChar->pclCI->clHorseInfo.GetCurrentShowHorse();
	if( pclHorse == NULL || pclHorse->siHorseUnique <= 0 )					return;

	//if ( pclChar->pclCI->clHorseInfo.clHorse.siLife < -4000 ) return ;

	SI32 horseunique = *pclMsg->cData;
	if( horseunique < 0 || horseunique >= MAX_HORSE_TYPE_INFO )		return;
	cltHorseTypeInfo *pclHorseTypeInfo = pclHorseManager->pclHorseTypeInfo[horseunique];
	if ( pclHorseTypeInfo == NULL ) return ;

	SI32 itemunique = 0 ;
	SI16 horselife = 0 ;
	switch(horseunique)
	{
	case 2: // 흑마
		{
			itemunique = 5503 ;
			horselife = 300 ;
		}
		break;
	case 3: // 백마
		{
			itemunique = 5506 ;
			horselife = 400 ;
		}
		break;
	case 4: // 적토마
		{
			itemunique = 5510 ;
			horselife = 500 ;
		}
		break;
	case 5: // 얼룩마
		{
			itemunique = 5520 ;
			horselife = 500 ;
		}
		break;
	case 6: // 루돌프
		{
			itemunique = 5530 ;
			horselife = 500 ;
		}
		break;
	case 7: // 천리마
		{
			itemunique = 5540 ;
			horselife = 500 ;
		}
		break;
	case 8: // 철기마
		{
			itemunique = 5550 ;
			horselife = 500 ;
		}
		break;
	case 9: // 호랑이
		{
			itemunique = 5560 ;
			horselife = 500 ;
		}
		break;
	default:
		return ;
	}

	if ( pclChar->pclCI->clCharItem.GetItemNum(itemunique) < 2 )
		return ;

	SI32 pos = pclChar->pclCI->clCharItem.GetItemPos(itemunique);
	if(pos < 0)return ;
	
	//------------------------------------
	// 말의 품종을 변경한다
	//------------------------------------
	pclHorse->siHorseUnique = horseunique ;
	//-------------------------------------------
	// 수명을 늘려준다.
	//-----------------------------------------
	pclHorse->siLife -= horselife ;

	// DB에 저장한다. 
	pclChar->UpdateHorseToDB(this, pclChar->pclCI->clHorseInfo.siCurrentShowHorse, pclHorse, SETHORSE_REASON_CHANGETYPE);

	pclChar->SetUpdateSwitch(UPDATE_HORSE, true, 0);

	cltItem *pclItem = &pclChar->pclCI->clCharItem.clItem[pos];
	pclItem->siItemNum = 2 ;

	// 통계를 낸다. 
	pclItemManager->AddUseNum(itemunique, 2);

	// 새로운 통계 - 아이템 사용
	//NStatistics_Add("Item", itemunique, "UseNum", 2);

	sDBRequest_UseItem clMsg(id,pclChar->pclCI->GetPersonID(),pos,pclItem,USEITEM_REASON_HORSECHANGE);
	SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg);

	return ;
}

void cltServer::DoMsg_GAMEMSG_REQUEST_HORSE_ADDSTATUS(cltMsg* pclMsg,SI32 id)
{
	if( pclCM->IsValidID( id ) == FALSE )
	{
		return;
	}

	cltCharServer *pclChar = (cltCharServer *)pclCM->CR[id];

	cltHorse* pclHorse = pclChar->pclCI->clHorseInfo.GetCurrentShowHorse();
	if( pclHorse == NULL || pclHorse->siHorseUnique <= 0 )					return;

	SI32 itemunique = ITEMUNIQUE(24050) ; // 말능력향상이용권

	if ( pclChar->pclCI->clCharItem.GetItemNum(itemunique) <= 0 )
		return ;

	SI32 pos = pclChar->pclCI->clCharItem.GetItemPos(itemunique);
	if(pos < 0)return ;

	//------------------------------------
	// 향상을 시켜준다.
	//------------------------------------
//	cltDate cldate ;
	if ( pclHorse->siStatusTicketNum == 0 )
	{
		pclHorse->clAddStatusDate.Set(&pclTime->clDate);
	}
	pclHorse->siStatusTicketNum += 1 ;

	// DB에 저장한다. 
	pclChar->SetUpdateSwitch(UPDATE_HORSE, true, 0);
	pclChar->UpdateHorseToDB(this, pclChar->pclCI->clHorseInfo.siCurrentShowHorse, pclHorse, SETHORSE_REASON_TICKET);

	cltItem *pclItem = &pclChar->pclCI->clCharItem.clItem[pos];
	pclItem->siItemNum = 1 ;

	// 통계를 낸다. 
	pclItemManager->AddUseNum(itemunique, 1);

	// 새로운 통계 - 아이템 사용
	//NStatistics_Add("Item", itemunique, "UseNum", 1);

	sDBRequest_UseItem clMsg(id,pclChar->pclCI->GetPersonID(),pos,pclItem,USEITEM_REASON_TICKET);
	SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg);

	return ;
}
