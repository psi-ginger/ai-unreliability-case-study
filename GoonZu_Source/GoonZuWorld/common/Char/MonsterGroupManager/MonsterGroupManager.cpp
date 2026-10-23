//---------------------------------
// 2004/12/23 성준엽
//---------------------------------

#include "MonsterGroupManager.h"

#include "PersonInfo.h"

#include "Char/CharServer/Char-Server.h"
#include "Char/CharClient/Char-Client.h"
#include "Char/CharManager/CharManager.h"

#include "Order/Order.h"

#include "../Common/Bullet/Bullet.h"
#include "Util/Util.h"

#include "../../Server/Server.h"
#include "../../Server/Statistics/Statistics.h"

#include "../../Client/Client.h"

#include "../../DBManager/GameDBManager_World/DBMsg-Person.h"
#include "../../DBManager/GameDBManager_World/DBMsg-Item.h"

#include "../../../lib/HashTable/HashTable_IntInt.h"
#include "../../../lib/HashTable/HashTable_StringInt.h"

extern cltCommonLogic* pclClient;

cltMonsterGroupManager::cltMonsterGroupManager()
{
	for ( SI32 i = 0 ; i < MAX_MONSTER_GROUP_NUM ; i ++ )
	{
		pclInitInfo[i].Init();
		pclMonsterGroup[i].Init();
	}
}

cltMonsterGroupManager::~cltMonsterGroupManager()
{
	for ( SI32 i = 0 ; i < MAX_MONSTER_GROUP_NUM ; i ++ )
	{
		pclInitInfo[i].Init();
		pclMonsterGroup[i].Init();
	}
}

void cltMonsterGroupManager::Action()
{
	cltServer * pclserver = (cltServer*)pclClient;
	if ( pclserver == NULL ) return ;

	for ( SI32 i = 1 ; i < MAX_MONSTER_GROUP_NUM ; i ++ )
	{
		if ( BeingGroup(i) == false ) continue ; // 현재 인덱스의 그룹이 존재하냐?
		if ( BeingInitGroup(i) == false ) continue ; // 현재 인덱스의 그룹 만들라는 초기값이 있냐?
		if ( IsMapBossGroup(i) == true ) // 맵의 보스 그룹이냐 아니냐
		{
			MakeMapBossGroup(i);
		}
		else
		{
			MakeGroup(i);
			SetBoss(i);
		}
	}
}

void cltMonsterGroupManager::MakeMapBossGroup(SI32 index)
{
	if ( index < 1 || index >= MAX_MONSTER_GROUP_NUM )
		return ;
	if ( pclMonsterGroup[index].bSetMapBossGroup == true )
		return ;
}

// 현재 그룹을 새로 만들어야 하는지 마는지. false면 안 만든다.
bool cltMonsterGroupManager::BeingGroup(SI32 index)
{
	if ( index < 1 || index >= MAX_MONSTER_GROUP_NUM )
		return false ;

	for ( SI32 i = 0 ; i < MAX_MONSTER_NUM_PER_GROUP ; i ++ )
	{
		if ( pclMonsterGroup[index].siCharUnique[i] > 0 )
			return false ;
	}

	return true ;
}

bool cltMonsterGroupManager::IsMapBossGroup(SI32 index)
{
	if ( index < 1 || index >= MAX_MONSTER_GROUP_NUM )
		return false ;

	return pclMonsterGroup[index].bMapBoss ;
}

// 주어진 인덱스에 해당하는 그룹 초기화가 있는지 없는지. 없으면 false로 안 만든다.
bool cltMonsterGroupManager::BeingInitGroup(SI32 index)
{
	if ( index < 1 || index >= MAX_MONSTER_GROUP_NUM )
		return false ;

	if ( pclInitInfo[index].siMapIndex < 0 ) return false ;

	for ( SI32 i = 0 ; i < MAX_MONSTER_NUM_PER_GROUP ; i ++ )
	{
		if ( pclInitInfo[index].BeingKindInfo(i) )
			return true ;
	}

	return false ;
}

// 그룹을 만든다. - Server
void cltMonsterGroupManager::MakeGroup(SI32 index)
{
	cltServer * pclserver = (cltServer*)pclClient;
	if ( pclserver == NULL ) return ;

	if ( index < 1 || index >= MAX_MONSTER_GROUP_NUM )
		return ;

	SI32 mapindex = pclInitInfo[index].siMapIndex ;
	if ( mapindex < 0 ) return ;
	
	SI32 radius = pclInitInfo[index].siRadius ;
	if ( radius < 0 ) return ;

	SI32 maxvalue = 0 ;
	for ( SI32 i = 0 ; i < MAX_MONSTER_KIND_PER_GROUP ; i ++ )
	{
		if ( pclInitInfo[index].pclKindInfoPerGroup[i].siKind > 0 )
		{
			maxvalue += pclInitInfo[index].pclKindInfoPerGroup[i].siValue ;
		}
	}

	if ( maxvalue > 0 )
	{
		for ( i = 0 ; i < MAX_MONSTER_NUM_PER_GROUP ; i ++ )
		{
			SI32 randvalue = rand() % maxvalue ;
			SI32 charkind = 0 ;
			SI32 level = 0 ;
			SI32 tempvalue = 0 ;
			for ( SI32 j = 0 ; j < MAX_MONSTER_KIND_PER_GROUP ; j ++ )
			{
				if ( pclInitInfo[index].pclKindInfoPerGroup[j].siKind > 0 &&
					 randvalue < pclInitInfo[index].pclKindInfoPerGroup[j].siValue )
				{
					if ( tempvalue == 0 ||
						( tempvalue != 0 && tempvalue > pclInitInfo[index].pclKindInfoPerGroup[j].siValue ) )
					{
						charkind = pclInitInfo[index].pclKindInfoPerGroup[j].siKind ;
						tempvalue = pclInitInfo[index].pclKindInfoPerGroup[j].siValue ;
						level = pclInitInfo[index].pclKindInfoPerGroup[j].siLevel ;
					}
				}
			}

			if ( charkind <= 0 || level <= 0 || level > MAX_CHARACTER_LEVEL ) continue ;

			SI32 x = pclInitInfo[index].siCenterX + rand() % radius ;
			SI32 y = pclInitInfo[index].siCenterY + rand() % radius ;
			SI32 dx = 0 ;
			SI32 dy = 0 ;

			if(pclserver->pclMapManager->pclMapArray[mapindex]->FindEmptyAreaForCharLocate(charkind, x, y, 10, &dx, &dy, true) == false)continue;

			cltInnerStatusInfo clinnerstatus;
			clinnerstatus.clEnemyType.Set(ENEMYTYPE_ALLPC, 0);

			SI32 charid = pclserver->SetNPCChar(charkind, VILLAGE_NONE, NULL, level, 0, dx, dy, mapindex, NPCITEMMODE_NORMAL, &clinnerstatus ) ;

			if( charid > 0)
			{
				pclMonsterGroup[index].siCharUnique[i] = charid ;
				cltCharServer* pclchar = (cltCharServer*)pclserver->pclCM->CR[charid] ;
				pclchar->siGroupUnique = index ;
				pclchar->siGroupMember = i ;
			}
		}
	}
}

// 보스를 설정한다.
void cltMonsterGroupManager::SetBoss(SI32 index)
{
	cltServer * pclserver = (cltServer*)pclClient;
	if ( pclserver == NULL ) return ;

	if ( index < 1 || index >= MAX_MONSTER_GROUP_NUM )
		return ;

	if ( pclMonsterGroup[index].siBossUnique > 0 ) return ;

	SI32 randdate = rand() ;
	SI32 siList[MAX_MONSTER_NUM_PER_GROUP];
	SI32 maxlist = 0 ;

	for ( SI32 i = 0 ; i < MAX_MONSTER_NUM_PER_GROUP ; i ++ )
	{
		if ( pclMonsterGroup[index].siCharUnique[i] > 0 )
		{
			siList[maxlist] = pclMonsterGroup[index].siCharUnique[i];
			maxlist ++ ;
		}
	}
	if ( maxlist <= 0 ) return ;

	pclMonsterGroup[index].siBossUnique = siList[randdate % maxlist ] ;
	if ( pclMonsterGroup[index].siBossUnique )
		((cltCharServer*)pclserver->pclCM->CR[pclMonsterGroup[index].siBossUnique])->bGroupBossSwitch = TRUE ;
}

// 죽은 놈을 없앤다. 보스가 죽으면 새로 뽑는다.
void cltMonsterGroupManager::DeleteMember(SI32 index,SI32 unique,BOOL bBoss )
{
	cltServer * pclserver = (cltServer*)pclClient;
	if ( pclserver == NULL ) return ;

	if ( index < 1 || index >= MAX_MONSTER_GROUP_NUM )
		return ;
	if ( unique < 0 || unique >= MAX_MONSTER_NUM_PER_GROUP )
		return ;

	cltCharServer* pclchar = ((cltCharServer*)pclClient->pclCM->CR[pclMonsterGroup[index].siCharUnique[unique]]);
	if ( pclchar == NULL ) return ;

	if ( bBoss ) // 보스인가?
	{
		pclMonsterGroup[index].siCharUnique[unique] = 0 ;
		pclMonsterGroup[index].siBossUnique = 0 ;
		SetBoss(index);
	}
	else
	{
		pclMonsterGroup[index].siCharUnique[unique] = 0 ;
	}

	pclchar->siGroupUnique = 0 ;
	pclchar->siGroupMember = 0 ;
	pclchar->bGroupBossSwitch = FALSE ;
}

SI32 cltMonsterGroupManager::GetBossID(SI32 index)
{
	if ( index < 1 || index >= MAX_MONSTER_GROUP_NUM )
		return 0 ;

	return pclMonsterGroup[index].siBossUnique ;
}

// 전투중인 대원을 찾고 전투타겟을 돌려준다.
bool cltMonsterGroupManager::FindBattleMember(SI32 * targetID,SI32 index,BOOL bBoss)
{
	cltServer * pclserver = (cltServer*)pclClient;
	if ( pclserver == NULL ) return false ;

	if ( !bBoss ) return false ;

	if ( index < 1 || index >= MAX_MONSTER_GROUP_NUM )
		return false ;

	for ( SI32 i = 0 ; i < MAX_MONSTER_NUM_PER_GROUP ; i ++ )
	{
		SI32 charid = pclMonsterGroup[index].siCharUnique[i] ;
		if ( charid )
		{
			// 일단 전투중인 놈을 찾는다.
			if ( pclserver->pclCM->CR[charid]->GetCurrentOrder() == ORDER_ATTACK )
			{
				SI32 targetid = pclserver->pclCM->CR[charid]->clAttackTarget.GetAttackID() ;
				if ( targetid )
				{
					*targetID = targetid ;
					return true ;
				}
			}
		}
	}

	return false ;
}

void cltMonsterGroupManager::MemberAttack(SI32 targetID,SI32 index)
{
	cltServer * pclserver = (cltServer*)pclClient;
	if ( pclserver == NULL ) return ;

	if ( index < 1 || index >= MAX_MONSTER_GROUP_NUM )
		return ;

	if ( IsPC(targetID) && pclserver->pclCM->IsAlive(targetID) )
	{
		for ( SI32 i = 0 ; i < MAX_MONSTER_NUM_PER_GROUP ; i ++ )
		{
			SI32 charid = pclMonsterGroup[index].siCharUnique[i] ;
			cltCharServer* pclchar = (cltCharServer*)pclserver->pclCM->CR[charid];
			if ( charid && pclchar != NULL )
			{
				// 전투중이지 않은 대원을 찾는다.
				if ( pclchar->GetCurrentOrder() != ORDER_ATTACK )
				{
					if( pclserver->pclCM->IsAlive(charid) == TRUE &&
						pclchar->clAttackTarget.GetAttackID() != targetID &&
						pclchar->CanAttack(targetID) == TRUE )
					{
						// 서버에서 공격하도록 한다. 
						OrderAttack(charid, targetID, BY_PLAYER);

						cltOrderDataInfo clOrderDataInfo;
						if( pclserver->pclOrderManager->MakeOrder_ATTACK(&clOrderDataInfo, targetID, BY_PLAYER) == true)
						{
							cltOrderData clOrderData;
							clOrderData.Make(charid, &clOrderDataInfo);
							
							cltMsg clMsg(GAMEMSG_RESPONSE_USERORDER, sizeof(clOrderData), (BYTE*)&clOrderData);
							pclchar->SendNeighbourMsg(&clMsg, true);
						}
					}
				}
			}
		}
	}
}

bool cltMonsterGroupManager::SetInitFromFile(SI32 index,SI32 mapindex,SI32 centerx,SI32 centery,SI32 radius,
											 SI32* kind,SI32* level,SI32* value )
{
	if ( index < 0 || index > MAX_MONSTER_GROUP_NUM )
		return false ;

	if ( pclInitInfo[index].siMapIndex >= 0 )
		return false ;

	pclInitInfo[index].siMapIndex = mapindex ;
	pclInitInfo[index].siCenterX = centerx ;
	pclInitInfo[index].siCenterY = centery ;
	pclInitInfo[index].siRadius = radius ;

	for ( SI32 i = 0 ; i < MAX_MONSTER_KIND_PER_GROUP ; i ++ )
	{
		pclInitInfo[index].pclKindInfoPerGroup[i].siKind = kind[i] ;
		pclInitInfo[index].pclKindInfoPerGroup[i].siLevel = level[i] ;
		pclInitInfo[index].pclKindInfoPerGroup[i].siValue = value[i] ;
	}

	return true ;
}

//================================================
// cltKindInfoPerGroup
//================================================
void cltKindInfoPerGroup::Init()
{
	siKind = 0 ;
	siValue = 0 ;
}

//================================================
// cltInitInfoPerGroup
//================================================
void cltInitInfoPerGroup::Init()
{
	for ( int i = 0 ;  i < MAX_MONSTER_KIND_PER_GROUP ; i ++ )
	{
		pclKindInfoPerGroup[i].Init();
	}

	siMapIndex = -1 ;
	siCenterX = -1 ;
	siCenterY = -1 ;
	siRadius = -1 ;
	bMapBoss = false ;
}

bool cltInitInfoPerGroup::BeingKindInfo(SI32 index)
{
	if ( index < 0 || index >= MAX_MONSTER_KIND_PER_GROUP )
		return false ;

	if ( pclKindInfoPerGroup[index].siKind > 0 &&
		pclKindInfoPerGroup[index].siValue > 0 )
		return true ;

	return false ;
}

//================================================
// cltMonsterGroupInfo
//================================================
void cltMonsterGroupInfo::Init()
{
	for ( int i = 0 ; i < MAX_MONSTER_NUM_PER_GROUP ; i ++ )
	{
		siCharUnique[i] = 0 ;
	}

	siBossUnique = 0 ;
	bMapBoss = false ;
	bSetMapBossGroup = false ;
}
