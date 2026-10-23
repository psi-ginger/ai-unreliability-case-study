#include "..\Client\Client.h"
#include "Char\CharServer\Char-Server.h"

#include "Map\Map.h"
//-----------------------------
// Common
//-----------------------------
#include "..\Common\NPCManager\NPC.h"

#include "../common/Char/MonsterGroupManager/MonsterGroupManager.h"

#include "..\..\Order\Order.h"
#include "PersonInfo.h"
#include "..\CharManager\CharManager.h"

#include "../../../Server/Server.h"
#include "../../CommonLogic/CityHall/CityHall.h"

extern cltCommonLogic* pclClient;

// 근처의 적을 찾는다. 
bool cltCharServer::SearchEnemy(SI32 range)
{
	SI32 i, j;
	SI32 id;

	for(i = -range;i <= range;i++)
		for(j = -range;j <= range;j++)
		{
			id = pclMap->GetIDMap(GetX()+j, GetY()+i);


			if(IsEnemy(id) && CanAttack(id) )
			{
				cltCharServer* pclchar = (cltCharServer*)pclCM->CR[id];

				if (clIdentity.siIdentity == IDENTITY_RACOONDODGE_EVENT ) // 러쿤 이벤트 몬스터면 무조건 공겨 
				{
					cltCharServer* pclcharTarget = (cltCharServer*)pclCM->CR[id];
					SI32 siCharInMAp = pclcharTarget->GetMapIndex() ;

					if ( siCharInMAp = MAPINDEX_FESTIVAL) 
					{
						cltMapCommon* pclmap = pclClient->pclMapManager->GetMapPointer( siCharInMAp )	;	// 맵포인터
						if(pclmap != NULL)
						{
							SI32 siColl = pclmap->CalcCollInfo( pclcharTarget->GetX(), pclcharTarget->GetY())	;
							if( siColl & BLOCK_EVENT1 || siColl & BLOCK_EVENT4 )	// 4번 이벤타 타
							{
								OrderAttack(siID, id, BY_PLAYER);
								return true;
							}
							else
							{
								return false;
							}
						}
					}

				}
				else if (clIdentity.siIdentity == IDENTITY_BATTLEROYAL_MONSTER )
				{
					cltCharServer* pclTargetChar = (cltCharServer*)pclCM->GetCharServer(id);
					if( NULL == pclTargetChar)
						continue;
					SI32 siMapIndex = pclTargetChar->GetMapIndex();

					if( siMapIndex == MAPINDEX_BATTLEROYAL_MABRIUM || siMapIndex == MAPINDEX_BATTLEROYAL_GRANDMABRIUM)
					{
						OrderAttack(siID, id, BY_PLAYER);
						return true;
					}

				}
				else
				{
					if( (pclKI != NULL && pclKI->IsAtb(ATB_WEAKENEMYNOATTACK)) || siCharMode == CHARMODE_BLACKWAR )// 약한 적은 공격하지 않는 속성이면, 
					{
						if(IsPC(pclchar->GetCharUnique()) && (pclchar->pclCI->clIP.GetLevel() < 20) )
						{
							continue;
						}	
					}

					// 낚시, 농경, 채굴중인 캐릭터는 공격하지 못 한다.
					if( ORDER_FISH == pclchar->GetCurrentOrder() ||
						ORDER_FARM == pclchar->GetCurrentOrder() ||
						ORDER_MINE == pclchar->GetCurrentOrder() )
					{
						continue;
					}

					//[추가 : 황진성 2008. 2. 18 히나마쓰리. 선공 방지.]
					if( pclchar->pclCI->clBI.siHomeVillage > 0 )
					{
						cltServer*  pclserver = (cltServer*)pclClient;
						cltStrInfo* pclinfo   = pclserver->pclCityHallManager->GetStrInfo( pclchar->pclCI->clBI.siHomeVillage );
						if ( pclinfo->clCityhallStrInfo.IHaveEventNpc(pclClient->GetUniqueFromHash(TEXT("KIND_HINADOLL"))) == TRUE )
						{	
							continue;
						}				
					}
				
					// 선공 방지가 되어 있는 캐릭터는 무시한다. 
					if(pclchar->bNoFirstAttackSwitch == true)continue;
					if(pclchar->pclCI->clBI.clBufInfo.IsHaveBuf(BUF_NOFIRSTATTACK)) continue;

					if(pclchar->pclCI->clBI.clBufInfo.IsHaveBuf(BUF_HINAMOCHI)) continue;
					
					if(pclchar->pclCI->clBI.clBufInfo.IsHaveBuf(BUF_EVENT_EGGMOONCAKE )) continue;
					
					if(pclchar->pclCI->clBI.clBufInfo.IsHaveBufExecptGetType(BUF_TRANSFORM,BUF_GET_TYPE_NIGHTMARKET)) continue;
					
					// 고양이, 강아지 방석 선공 방지 기능.
					if( (pclchar->GetCurrentOrder() == ORDER_SITDOWN )&&( pclchar->IsHaveCushionItem() > 0)  )	continue;

					//KHY - 1113 - 크리스마스 이벤트 - 선공방지.
					if( pclchar && pclchar->m_siSetItemType == XMAS_SETITEM )	continue;

					// 흑의군단 전쟁용 캐릭터인 경우 - 일반 개인상점 캐릭터도 공격하지 않는다.
					if( siCharMode == CHARMODE_BLACKWAR )
					{
						if(pclchar->bPersonalShopSwitch == true || pclchar->bPremiumPersonalShop == true)		continue;
					}

					// 흑의군단 전쟁용 캐릭터가 아닌경우 - 프리미엄 개인상점 캐릭터만 공격하지 않는다.
					if(pclchar->bPersonalShopSwitch == true && pclchar->bPremiumPersonalShop == true)			continue;

					// 적이 소환수일 경우 자신보다 약하면 공격하지 않는다.
					if( pclchar->clIdentity.siIdentity == IDENTITY_PERSON_SUMMON)
					{
						if(clPB.GetBattlePower() > pclchar->clPB.GetBattlePower())
						{
							continue;
						}
						// [기형] 소환수 선공 방지 
						if(pclchar->bNoFirstAttackSwitch == true)
						{
							continue;
						}
						// [기형] 주인이 프리미엄 선공 방지 상태일 경우 선공 불가
						cltCharServer* pclParentChar = pclCM->GetCharServer(pclchar->GetSummonParentID());
						if( NULL != pclParentChar)
						{
							if( true == pclParentChar->pclCI->clBI.clBufInfo.m_bNoFirstAttackPreminum)	continue;

							// [춘기] 주인이 방석에 앉아 있는 상태에서는 소환수한테도 선공방지가 걸린다. (2009.09.23)
							if((pclParentChar->GetCurrentOrder() == ORDER_SITDOWN )&&( pclParentChar->IsHaveCushionItem() > 0))	
							{
								continue;
							}	
						}
					}

					if ( pclClient->IsCountrySwitch(Switch_Set_Item_Add) )
					{
						//[진성] 세트 아이템 선공 방지.
						if( pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemAbilityValue( pclchar, SETITEM_ABILITY_TYPE_NO_FIRST_ATTACK, USE_SETITEM_1 ) || 
							pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemAbilityValue( pclchar, SETITEM_ABILITY_TYPE_NO_FIRST_ATTACK, USE_SETITEM_2 ) ||
							pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemAbilityValue( pclchar, SETITEM_ABILITY_TYPE_NO_FIRST_ATTACK, USE_SETITEM_3 ) ||
							pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemAbilityValue( pclchar, SETITEM_ABILITY_TYPE_NO_FIRST_ATTACK, USE_SETITEM_4 ) ||
							pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemAbilityValue( pclchar, SETITEM_ABILITY_TYPE_NO_FIRST_ATTACK, USE_SETITEM_5 ))
						{
							continue;
						}
					}
					// 말 프리미엄 파츠 풀파츠시 특수능력 삭제.
					//// 말 풀파츠 일때 선공 방지
					//cltHorse* pclHorse = pclchar->pclCI->clHorseInfo.GetCurrentShowHorse()	;
					//if ( pclHorse != NULL )
					//{
					//	if( pclHorse->GetPremiumPartsNum() == 3 )
					//	{
					//		SI32 siHorseUnique = pclHorse->siHorseUnique ;
					//		switch ( siHorseUnique )
					//		{
					//		case HORSEUNIQUE_TIGER :// 호랑이
					//		case HORSEUNIQUE_WOLF:// 늑대
					//		case HORSEUNIQUE_CAMEL:// 낙타
					//		case HORSEUNIQUE_ELEPHANT:// 코끼리
					//		case HORSEUNIQUE_MOTORCYCLE:// 오토바이
					//		case HORSEUNIQUE_FIREHATCH:// 불해치
					//			{
					//				continue	;
					//			}
					//			break;
					//		}
					//	}
					//}

					// 명령을 만든다. 
					OrderAttack(siID, id, BY_PLAYER);
					return true;
				}
			}
		}

		return false;
}

// 랜덤하게 주변으로 이동한다. 
bool cltCharServer::RandomMove()
{
	SI32 x, y;

	//return false;

	// 이동 속성이 없으면 이동하지 않는다. 
	if(IsAtb(ATB_MOVE) == false)return false;

	//if ( GetKind() == pclClient->GetUniqueFromHash(TEXT("KIND_LIGHTSOLDIER")) )
	//{
	//	int temp = 0;
	//	temp++;
	//}

	SI32 npcgroup = GetNPCGroup();
	// NPCGroup에 속한 경우, 
	if(npcgroup >= 0)
	{
		// 현재 위치가 NPCGroup범위밖이면 Center지점으로 간다. 
		if(((cltServer*)pclClient)->IsInNPCGroup(npcgroup, GetMapIndex(), GetX(), GetY()) == false)
		{
			cltNPCGroup* pclnpcgroup = pclClient->GetNPCGroupPointer(npcgroup);
			if(pclnpcgroup)
			{
				x = pclnpcgroup->siX;
				y = pclnpcgroup->siY;

				// 명령을 만든다. 
				OrderMove(siID, x, y, BY_PLAYER);
				return true;

			}

		}
		// 현재 위치가 NPCGroup범위안이면 랜덤하게 간다. 
		else
		{
			x = GetX() + 5 - (rand() %11);
			y = GetY() + 5 - (rand() %11);

			// 명령을 만든다. 
			if(pclMap->IsInMapArea(x, y) && IsColl(x, y) == false)
			{
				OrderStraightMove(siID, x, y, BY_PLAYER);
				return true;
			}
		}
	}
	// NPCGroup에 속하지 않은 경우, 
	else
	{
		x = GetX() + 10 - (rand() %21);
		y = GetY() + 10 - (rand() %21);

		// 명령을 만든다. 
		if(pclMap->IsInMapArea(x, y)  && IsColl(x, y) == false)
		{
			OrderStraightMove(siID, x, y, BY_PLAYER);
			return true;
		}
	}

	return false;

}

// 보스를 따라가게 한다
bool cltCharServer::FollowGroupBoss(SI32 bossid)
{
	if ( !bossid ) return false ;
	if ( pclCM->IsAlive(bossid) == false ) return false ; // 보스가 죽었으면 따라가진 않는다.

	cltCharServer* pclchar = (cltCharServer*)pclCM->CR[bossid];
	if ( pclchar == NULL ) return false ;

	SI32 length = max( TABS(pclchar->GetX() - GetX()) , TABS(pclchar->GetY() - GetY()) );
	SI32 x = pclchar->GetX() + 1 - (rand() % 3);
	SI32 y = pclchar->GetY() + 1 - (rand() % 3);

	// 지나치게 많이 떨어져 있으면 워프 시킨다. 
	if(length >= 8)
	{
		SI32 sx,sy ;
		if(pclchar->pclMap->FindEmptyAreaForCharLocate(GetKind(), x, y, 5, &sx, &sy, true) == true)
		{
			cltWarpInfo clinfo(true, pclchar->GetMapIndex(), false, false, sx, sy, -1, -1);
			clCharWarpInfo.Set(&clinfo);
			Warp();
		}
	}
	else if(length >= 4)
	{
		// 자신에게 이동 명령을 내린다. 
		OrderMove(siID, x, y, BY_PLAYER);

		// 자신을 포함한 이웃에게 이동을 지시한다. 
		cltOrderDataInfo clOrderDataInfo;
		if( pclClient->pclOrderManager->MakeOrder_MOVE(&clOrderDataInfo, x, y, BY_PLAYER) == true)
		{
			cltOrderData clOrderData;
			clOrderData.Make(GetCharUnique(), &clOrderDataInfo);

			cltMsg clMsg(GAMEMSG_RESPONSE_USERORDER, sizeof(clOrderData), (BYTE*)&clOrderData);
			SendNeighbourMsg(&clMsg, true);
			return true ;
		}
	}


	return false;
}

BOOL cltCharServer::OperateWaitServer()
{
	SI32 aniindex = 0;

	int i = 0, j = 0, k = 0;
	SHORT id = 0;

	//-------------------------------------------------
	// 사망에 따른 페널티를 주었다면 그 기록을 없앤다. 
	//-------------------------------------------------
	if(bDeathPenaltySwitch	== true)
	{
		bDeathPenaltySwitch = false;
	}

	if(IsPC(CharUnique))return false;

	// 정지시 자폭 기능이 있는 몬스터라면, 
	if((pclKI != NULL && pclKI->IsAtb(ATB_WAITSELFBOMB)))
	{
		SelfBombAction();
		return true;
	}

	//===================================
	// NPC
	//===================================
	SI32 data = (rand() + siID) % (150 / CHAR_ACTION_INTERVAL_FRAME) ;

	if(data == 0)
	{   
		// 호전적인 캐릭터인 경우 적을 찾는다.
		SI32 range = 0;
		if(pclKI->siAggressive)	range = pclKI->siAggressive;
		switch(clIdentity.siIdentity) 
		{
		case IDENTITY_CASTLE:				range = 4;		break;
		case IDENTITY_VILLAGEGOD:			range = 4;		break;
		case IDENTITY_VILLAGEGOD_SUMMON:	range = 12;		break;	
		case IDENTITY_PERSON_SUMMON:		return true;	break;// 소환수는 idle일때 쓸데 없는 짓을 하지 않는다.
		case IDENTITY_VILLAGEHUNT:							break;
		case IDENTITY_BATTLEROYAL_MONSTER:	range = 20;		break;
			//case IDENTITY_SOULGUARD:			return true;	break;// 수호 정령일때는 아무짓 하지 못하도록 한다
		default:											break;
		}

		if(range)
		{
			if(SearchEnemy(pclKI->siAggressive) == true)		return true;
			if(clIdentity.siIdentity ==IDENTITY_PERSON_SUMMON)	return true;
		}

		// 공격해야 할 마을이 있는 경우, 
		if(siCharMode == CHARMODE_BLACKWAR)
		{
			// 마을의 위치를 찾는다.
			if( siCharModePara1 )
			{
				//SI32 x = pclClient->pclVillageManager->pclVillageInfo[siCharModePara1]->PosX;
				//SI32 y = pclClient->pclVillageManager->pclVillageInfo[siCharModePara1]->PosY;


			/*	// 내 위치가 마을의 주변이라면 마을 안으로 들어간다. 
				if(TABS(GetX() - x) <= 5 && TABS(GetY() - y) <= 5)
				{
				SI32 maptypeunique = pclClient->pclMapManager->pclMapArray[ siCharModePara1 ]->siMapUnique;
				SI32 centerx = pclClient->pclMapManager->pclMapTypeManager->pclMapTypeInfo[maptypeunique]->siCenterX;
				SI32 centery = pclClient->pclMapManager->pclMapTypeManager->pclMapTypeInfo[maptypeunique]->siCenterY;

				cltWarpInfo clinfo(true, siCharModePara1, false, false, centerx, centery, -1, -1);
				clCharWarpInfo.Set(&clinfo);
				Warp();
				} */
			}
			// 야외지역에 있을 때만. 
			else if(GetMapIndex() == 0)
			{
				// 마을 주변이 아니라면 이동시킨다. 자금성으로 이동시킨다.
				//OrderMove(siID, 288, 354, BY_PLAYER);

				// 랜덤하게 주변을 이동한다. - 마을 주변을 배회하는 것으로 일단 사양 변경 - by LEEKH 2008-10-20
				if(RandomMove() == true)return true;

			/*	// 랜덤하게 마을을 새로 찾는다.
				if(rand() % 50 == 0)
				{
					siCharModePara1 = pclClient->pclVillageManager->FindRandVillage();

					if(siCharModePara1)
					{
						// 마을의 위치를 찾는다.
						SI32 x = pclClient->pclVillageManager->pclVillageInfo[siCharModePara1]->PosX;
						SI32 y = pclClient->pclVillageManager->pclVillageInfo[siCharModePara1]->PosY;

						// 마을 주변이 아니라면 이동시킨다. 
						OrderMove(siID, x, y, BY_PLAYER);
					}
				}
				else
				{
					// 랜덤하게 주변을 이동한다. 
					if(RandomMove() == true)return true;
				}*/
			}
			else
			{
				// 랜덤하게 주변을 이동한다. 
				if(RandomMove() == true)return true;
			}
		}

		// 제자리에서 대기하는 설정이 아니라면, 
		if(bStaySwitch == false)
		{
			if(siGroupUnique)
			{
				// 그룹의 보스이면, 
				if(bGroupBossSwitch == TRUE)
				{
					// 랜덤하게 주변을 이동한다. 
					if(RandomMove() == true)return true;
				}
				else
				{
					// 보스를 따른다.
					SI32 bossid = pclClient->pclMonsterGroupManager->GetBossID(siGroupUnique);
					if ( bossid )
					{
						if ( FollowGroupBoss(bossid) == true ) return true ;
					}
				}
			}
			else
			{
				// 랜덤하게 주변을 이동한다. 
				if(RandomMove() == true)return true;
			}
		}
	}
	// 자동 치료. 
	else if(data == 1)
	{
		HealAI();
	}
	// 낚시 
	else if(data >= 2 && data < 10)
	{
		FishAI();
	}
	// 농경 
	else if(data >= 10 && data < 20)
	{
		FarmAI();
		MineAI();
	}
	else
	{
		// 제자리에서 대기하는 설정이 아니라면, 
		if(bStaySwitch == false)
		{
			if(siGroupUnique)
			{
				// 그룹의 보스이면, 
				if(bGroupBossSwitch == TRUE)
				{
					// 전투중인 조직원이 있는지 확인하여  조치한다. 
					SI32 targetID = 0 ;
					if ( pclClient->pclMonsterGroupManager->FindBattleMember(&targetID,siGroupUnique,bGroupBossSwitch) == true )
						pclClient->pclMonsterGroupManager->MemberAttack(targetID,siGroupUnique);
				}
				else
				{
					// 보스를 따른다.
					SI32 bossid = pclClient->pclMonsterGroupManager->GetBossID(siGroupUnique);
					if ( bossid )
					{
						if ( FollowGroupBoss(bossid) == true ) return true ;
					}
				}
			}
		}

	}



	return TRUE;
}

// 인공지능 낚시. 
void cltCharServer::FishAI()
{
	if(pclKI == NULL || pclKI->IsAtb(ATB_FISH) == false)return ;

	// 소환수가 아니면 실패. 
	if(clIdentity.siIdentity != IDENTITY_PERSON_SUMMON)return ;


	// 주인이 낚시 중인지 확인한다. 
	SI16 parentid = clIdentity.clSummonInfo.siParentCharUnique;
	if(pclCM->IsValidID(parentid) == false)return ;

	if(pclCM->CR[parentid]->GetCurrentOrder() != ORDER_FISH)return ;


	// 소환수의 위치가 낚시 가능 위치인지 확인한다. 
	SI32 dir = 0;
	if(IsFishArea(GetX(), GetY(), &dir) == true)
	{
		// 서버를 낚시하게 만든다. 
		OrderFish(GetID(), dir, BY_PLAYER);						
	}
	// 낚시 불가능 위치라면 위치를 찾는다. 
	else
	{
		// 주인 주변의 적당한 낚시 위치를 찾는다.
		SI32 x, y;
		if(pclCM->CR[parentid]->FindFishArea(&x, &y) == true)
		{
			OrderMove(GetID(), x, y, BY_PLAYER);
		}
	}
}

// 인공지능 농경
void cltCharServer::FarmAI()
{
	if(pclKI == NULL || pclKI->IsAtb(ATB_FARM) == false)return ;

	// 소환수가 아니면 실패. 
	if(clIdentity.siIdentity != IDENTITY_PERSON_SUMMON)return ;

	// 자신이 농경중이 아니어야 한다. 
	if(GetCurrentOrder() == ORDER_FARM)return ;

	// 주인이 농경 중인지 확인한다. 
	SI16 parentid = clIdentity.clSummonInfo.siParentCharUnique;
	if(pclCM->IsValidID(parentid) == false)return ;

	if(pclCM->CR[parentid]->GetCurrentOrder() != ORDER_FARM)return ;


	// 소환수를 농사짓게 한다. 
	OrderFarm(GetID(), pclCM->CR[parentid]->GetX(), pclCM->CR[parentid]->GetY(), BY_PLAYER);						
}

// 인공지능 채광
void cltCharServer::MineAI()
{
	if(pclKI == NULL || pclKI->IsAtb(ATB_MINE) == false)return ;

	// 소환수가 아니면 실패. 
	if(clIdentity.siIdentity != IDENTITY_PERSON_SUMMON)return ;

	// 자신이 채광중이 아니어야 한다. 
	if(GetCurrentOrder() == ORDER_MINE)return ;

	// 주인이 채광 중인지 확인한다. 
	SI16 parentid = clIdentity.clSummonInfo.siParentCharUnique;
	if(pclCM->IsValidID(parentid) == false)return ;

	if(pclCM->CR[parentid]->GetCurrentOrder() != ORDER_MINE)return ;


	// 소환수를 채광하게 한다. 
	OrderMine(GetID(), pclCM->CR[parentid]->GetX(), pclCM->CR[parentid]->GetY(), BY_PLAYER);

	return;
}