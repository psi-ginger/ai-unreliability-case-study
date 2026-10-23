//---------------------------------
// 2004/10/30 김태곤
//---------------------------------

//--------------------------------------
// Common
//--------------------------------------
#include "PersonInfo.h"
#include "Char\CharServer\Char-Server.h"
#include "Order\Order.h"
#include "..\CharManager\CharManager.h"

#include "../../common/Event/MofuMofu/MofuMofu.h"

//--------------------------------------
// CommonLogic
//--------------------------------------
#include "MsgType-Person.h"

//--------------------------------------
// Server
//--------------------------------------
#include "Server.h"

// 적에게 공격받고 있을때 호출하는 함수 
void cltCharServer::SetBeAttackedSwitch(cltServer* pclserver, SHORT enemyid)
{
	if(enemyid<=0)return ;

    if(pclCM->IsAlive(enemyid)==FALSE)return ;

	// 피격 관련 정보를 설정한다. 
	clBeHitInfo.Set(true, enemyid);
	
	cltCharServer* pEnemyChar = pclCM->GetCharServer(enemyid) ;

	if(	pEnemyChar != NULL 
		&& pEnemyChar->clIdentity.siIdentity != IDENTITY_SOULGUARD_SUMMONPUMKIN
		&&IsNPC(GetCharUnique()) == true 
		&& (pclKI == NULL || pclKI->IsAtb((ATB_NOJJIM)) == false) 
		&& clIdentity.siIdentity == 0)
	{
		// 찜당했음을 설정한다. 
		if(clJim.SetBeJimed(pclCM->CR[enemyid]->GetCharUnique(), pclserver->GetFrame()) == true)
		{
			// 이미 다른 찜한 캐릭터가 있으면 찜을 해제한다. 
			if(pclCM->CR[enemyid]->clJim.siJimSlaveCharUnique)
			{
				// 이미 찜한 다른 캐릭터가 내가 아니어야...
				if(pclCM->CR[enemyid]->clJim.siJimSlaveCharUnique != GetCharUnique())
				{
					
					SI32 id = pclCM->GetIDFromCharUnique(pclCM->CR[enemyid]->clJim.siJimSlaveCharUnique);
					if(pclCM->IsValidID(id))
					{
						pclCM->CR[id]->clJim.Init();
						
						cltGameMsgResponse_Jim clinfo(pclCM->CR[enemyid]->clJim.siJimSlaveCharUnique, 0);
						cltMsg clMsg(GAMEMSG_RESPONSE_JIM, sizeof(clinfo), (BYTE*)&clinfo);
						((cltCharServer*)pclCM->CR[id])->SendNeighbourMsg(&clMsg, true);
						
					}
					
					pclCM->CR[enemyid]->clJim.siJimSlaveCharUnique = 0;
				}
				
			}
			
			// 찜했음을 설정한다. 
			pclCM->CR[enemyid]->clJim.SetJim(GetCharUnique());
			
			cltGameMsgResponse_Jim clinfo(GetCharUnique(), pclCM->CR[enemyid]->GetCharUnique());
			cltMsg clMsg(GAMEMSG_RESPONSE_JIM, sizeof(clinfo), (BYTE*)&clinfo);
			SendNeighbourMsg(&clMsg, true);
		}
	}

	//--------------------------------------------------------
	// 호전적인 캐릭터인 경우 주변의 아군에게 도움을 요청한다. 
	//--------------------------------------------------------
	bool bwarswitch = false;
	bool bvillagewarswitch = false;
	if( siCharMode == CHARMODE_INSTANCEMAP ||
		siCharMode == CHARMODE_BLACKWAR )
	{
		bwarswitch = true;
	}
	
	SI32 villageunique = pclCI->clBI.siHomeVillage;
	switch(clIdentity.siIdentity) 
	{
	case IDENTITY_VILLAGEGOD:			bvillagewarswitch = true;	break;
	case IDENTITY_CASTLE:				bvillagewarswitch = true;	break;
	case IDENTITY_VILLAGEGOD_SUMMON:	bvillagewarswitch = true;	break;	
	case IDENTITY_PERSON_SUMMON:		bvillagewarswitch = true;	break;
	case IDENTITY_VILLAGEHUNT:			break;
	default:							break;
	}
	
	if(bwarswitch || (pclKI != NULL && pclKI->IsAtb(ATB_COWORK)) || bvillagewarswitch)
	{

		// 상대의 레벨이 30은 넘어야 지원을 요청하게 된다. 
		if(pclCM->CR[enemyid]->pclCI->clIP.GetLevel() > 30)
		{
			SI32 id = pclCM->FindNearAlly(GetKind(), GetMapIndex(), GetX(), GetY(), bwarswitch, villageunique);
			if(id)
			{
				OrderAttack(id, enemyid, BY_PLAYER);
			}
		}
	
	}
	
}

// 피격시의 행위.  
void cltCharServer::BeAttackedAction()
{
	
	// 공격 받는 일이 중단되면, 
	if(clBeHitInfo.bBeAttackedSwitch == false)return ;

	// 다른 일로 바쁜 상태면 처리할 수 없다. 
	if(bBusySwitch == true) return ;

	// 살아 있어야만 한다. 
	if(GetLife() <= 0)return ;

	// NPC만 가능하다. 
	if(IsPC(CharUnique) )return ;
	// 소환수도 불가능. 
	if(clIdentity.siIdentity == IDENTITY_PERSON_SUMMON)return ;
	

	// 공격자가 살아 있어야 한다. 
	SI16 attackerid = clBeHitInfo.siAttackerID;
	if(pclCM->IsAlive(attackerid) == FALSE)	return ;

	// 피격 관련 정보를 초기화한다.  
	clBeHitInfo.Init();

	// 적에 대한 대처 
	FindEnemyAction(attackerid);

}


// 어뢰 아이템으로부터 어뢰 캐릭터를 생성한다.
void cltCharServer::MakeTorpedo(cltServer* pclserver, SI32 attackid, cltItem* pcltorpedo)
{
	cltOrderDataInfo Order;
	pclserver->pclOrderManager->MakeOrder_ATTACK(&Order, attackid, BY_PLAYER);
	
	SI32 mapindex	= GetMapIndex();
	SI32 six		= GetX();
	SI32 siy		= GetY(); 
	
	//SI32 torpedoid = pclserver->MakeGuardChar(pclClient->GetUniqueFromHash(TEXT("KIND_TORPEDO")), pclCI->clBI.siHomeVillage, mapindex, six, siy, &Order, &clInnerStatus.clEnemyType);
	//if(torpedoid)
	//{
	//	// 어뢰의 데미지를 추가한다. 
	//	pclCM->CR[torpedoid]->clPB.siTorpedoDamage = pcltorpedo->GetMinDamage(pclserver->pclItemManager) + rand() % max(1, pcltorpedo->GetMaxDamage(pclserver->pclItemManager) - pcltorpedo->GetMinDamage(pclserver->pclItemManager) ) ;
	//	pclCM->CR[torpedoid]->ParametaBoxAction();

	//	// 소환수의 생명시간을 위해 현재의 시간을 설정한다.
	//	((cltCharServer*)(pclCM->CR[ torpedoid ]))->dwTimerClock = pclserver->CurrentClock;
	//}
}
