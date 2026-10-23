//---------------------------------
// 2005/5/03 SJY
//---------------------------------

#include <CommonLogic.h>
#include "../../Client/Client.h"

#include "NPC.h"
#include "../../CommonLogic/CommonLogic.h"
#include "Char/CharServer/Char-Server.h"
#include "Char/CharManager/CharManager.h"
#include "../../Common/Order/Order.h"
#include "../../Common/util/util.h"

#include "../../common/PartyMgr/PartyObj.h"
#include "../../common/PartyMgr/PartyMgr.h"
#include "../../common/PartyMgr/PartyBaseInfo.h"

#include "../../Common/Char/MonsterGroupManager/MonsterGroupManager.h"

#include "../../CommonLogic/MsgType-System.h"

#include "../../CommonLogic/MsgRval-Define.h"

#include "../../../DBManager/GameDBManager_World/DBMsg-Item.h"

extern cltCommonLogic* pclClient;

bool cltNPCManagerCommon::MakeNPCInstance(SI32 parentmapindex, SI32 party, bool boss )
{
	cltServer* pclserver = (cltServer*)pclClient;
	if ( pclserver == NULL )											return false;
	if ( parentmapindex < 0 && parentmapindex >= MAX_MAP_NUMBER )		return false;

	if ( pclClient->pclMapManager->IsInstanceMap( parentmapindex ) == false)		return false;
	cltMapCommon* pclParentmap = pclserver->pclMapManager->pclMapArray[parentmapindex] ;
	if( pclParentmap == NULL )														return false;

	for(SI32 i = 0 ; i < MAX_NPC_GROUP_NUMBER;i++)
	{
		if( pclNPCGroup[i] == NULL )													continue;

		SI32 mapindex		= pclNPCGroup[i]->siMapIndex;
		SI32 npckindnumber	= pclNPCGroup[i]->NPCKindNumber;
		SI32 radius			= pclNPCGroup[i]->siRadius ;
		SI32 ix				= pclNPCGroup[i]->siX ;
		SI32 iy				= pclNPCGroup[i]->siY ;
		SI32 siGroupID		= i;

		if ( mapindex < 0 && mapindex >= MAX_MAP_NUMBER )								continue;
		cltMapCommon* pclmap = pclserver->pclMapManager->pclMapArray[mapindex] ;
		if( pclmap == NULL )															continue;
		if( pclClient->pclMapManager->GetParentMap( mapindex ) != parentmapindex )		continue;

		for ( SI32 j = 0 ; j < npckindnumber ; j ++ )
		{
			SI32 monsternum = 0 ;
			if ( pclNPCGroup[i]->clUnit[j].siKind <= 0 ) continue ;

			SI32 level		= pclNPCGroup[i]->clUnit[j].siKindLevel ;
			SI32 maxnumber	= pclNPCGroup[i]->clUnit[j].siKindMaxNum ;
			SI32 kind		= pclNPCGroup[i]->clUnit[j].siKind;

			while( monsternum < maxnumber )
			{
				// 더이상 생성할 공간이 없다면, 생성하지 않는다. - 무한루프 방지
				if( pclCM->pclIDManager->GetInstanceNPCNumber() >= MAX_INSTANCE_NPC_NUMBER )		break;

				cltInnerStatusInfo clinnerstatus;
				clinnerstatus.clEnemyType.Set(ENEMYTYPE_ALLPC, 0);

				SI32 x = ix + (radius/2) - rand() % radius ;
				SI32 y = iy + (radius/2) - rand() % radius ;
				SI32 dx = 0 , dy = 0 ;

				// 생성 허용 범위를 넓게 잡아준다.
				if ( pclmap->FindEmptyAreaForCharLocate(kind, x, y, 5, &dx, &dy, false) == false)
					continue ;

				switch ( pclClient->pclMapManager->GetInstanceMapType( parentmapindex ) )
				{
					//---------------------------------------------------------------------------------------
					//
					//	길드용 맵일 때 몬스터 처리
					//
					//---------------------------------------------------------------------------------------
					case INSTANCEMAP_TYPE_GUILD:
						{
							cltIdentity	clIdentity;

							// 인던 몬스터 점수
							SetGuildNPCPoint( kind, &clIdentity );

							SI32 id = pclserver->SetNPCChar(kind, VILLAGE_NONE, &clIdentity, level, 0, dx ,dy ,
								mapindex, NPCITEMMODE_NORMAL, &clinnerstatus );

							if ( id > 0 && id < (MAX_PERSON_NUMBER + MAX_INSTANCE_NPC_NUMBER) )
							{
								monsternum ++ ;

								// 게이트 몬스터 - 부모맵에 설정
								if( kind == pclClient->GetUniqueFromHash("KIND_KEY_RACCOON") )
									pclParentmap->m_siGateMonID = id;

								// 보스 몬스터 - 부모맵에 설정
								if( pclClient->pclKindInfoSet->pclKI[kind]->IsAtb(ATB_BOSS) )
									pclParentmap->m_siBossID = id;
#ifdef _DEBUG
								pclClient->pclLog->FilePrint( TEXT("config\\regenerationNPC.txt"), TEXT("%d\t%d\t0\t1\t"), mapindex, kind );
#endif
							}
							else
							{
#ifdef _DEBUG
								MessageBox(NULL, "MakeNPCInstance", "DEBUG", MB_OK);
#endif
								break ;
							}
						}
						break;

					//---------------------------------------------------------------------------------------
					//
					//	파티용 맵일 때 몬스터 처리
					//
					//---------------------------------------------------------------------------------------
					case INSTANCEMAP_TYPE_PARTY:
						{
							cltIdentity	clIdentity;
							SI32		id			= 0;

							// 더미 데이터
							clIdentity.MakeInstanceMap(IDENTITY_INSTANCEMAP, 1);

							id = pclserver->SetNPCChar(kind, VILLAGE_NONE, &clIdentity, level, 0, dx ,dy,
								mapindex, NPCITEMMODE_NORMAL, &clinnerstatus );

							if ( id > 0 && id < (MAX_PERSON_NUMBER + MAX_INSTANCE_NPC_NUMBER) )
							{
								monsternum ++ ;

								// 보너스 몬스터 - 부모맵에 설정
								if ( kind == pclClient->GetUniqueFromHash("KIND_PTIMEBONUS") )
								{
									for ( SI32 siIndex=0; siIndex<MAX_INSTANCE_BONUS_MONSTER_NUMBER; siIndex++ )
									{
										if ( pclParentmap->m_siBonusMonID[ siIndex ] != 0 ) continue;

										pclParentmap->m_siBonusMonID[ siIndex ] = id;
										break;
									}
								}

								// 보스 몬스터 - 부모맵에 설정
								if ( pclClient->pclKindInfoSet->pclKI[kind]->IsAtb(ATB_BOSS) )
								{
									pclParentmap->m_siBossID = id;
								}

								// 보너스 몬스터를 제외한 몬스터만 생성된 마릿수를 더한다
								if ( kind != pclClient->GetUniqueFromHash("KIND_PTIMEBONUS") )
								{
									pclParentmap->m_siMaxMonsterNum += 1;
								}
							}
						}
						break;

					//---------------------------------------------------------------------------------------
					//
					//	마을용 맵일 때 몬스터 처리
					//
					//---------------------------------------------------------------------------------------
					case INSTANCEMAP_TYPE_VILLAGE:
						{
							monsternum ++ ;
						}
						break;

					//---------------------------------------------------------------------------------------
					//
					//	마을용 맵일 때 몬스터 처리
					//
					//---------------------------------------------------------------------------------------
					case INSTANCEMAP_TYPE_SERVER:
						{
							// 0일때 즉 맨처음 셋팅한다면 현재 NPC Group ID로 설정한다
							if ( 0 >= pclParentmap->m_siNPCGroupID )
							{
								pclParentmap->m_siNPCGroupID = siGroupID;
							}
							
							// 맨 처음 설정한 NPC GroupID와 다르다면 생성하지 않는다
							if ( pclParentmap->m_siNPCGroupID != siGroupID )
							{
								// 실질적으로 생성되지는 않았지만 현재 로직에 맞추기 위함 (그냥 여기서 return true해볼까)
								monsternum ++;
								continue;
							}

							if (pclClient->GetUniqueFromHash("KIND_ROBIGATE") == kind)
							{
								pclParentmap->m_siNPCGroupID = 0;
							}

							// 게이트 몬스터는 맨처음만 생성하면 되기 때문에 2번째 부터는 생성하지 않는다
							if ( (0 < pclmap->m_siClearStep) && (pclClient->GetUniqueFromHash("KIND_ROBIGATE") == kind) )
							{
								monsternum ++;
								continue;
							}

							cltIdentity	clIdentity;
							SI32		id			= 0;
							SI32		siScore		= 0;

							// 맵 인덱스에 따라 점수가 다르다(멥 인덱스말고 처리할 방법은?)
							switch ( parentmapindex )
							{
								case MAPINDEX_SERVEREVENT_DUNGEON1:	siScore = 1;	break;
								case MAPINDEX_SERVEREVENT_DUNGEON2:	siScore = 2;	break;
								case MAPINDEX_SERVEREVENT_DUNGEON3:	siScore = 3;	break;
								case MAPINDEX_SERVEREVENT_DUNGEON4:	siScore = 4;	break;
							}

							//점수 데이터
							clIdentity.MakeInstanceMap( IDENTITY_INSTANCEMAP, siScore );

							id = pclserver->SetNPCChar(kind, VILLAGE_NONE, &clIdentity, level, 0, dx ,dy,
								mapindex, NPCITEMMODE_NORMAL, &clinnerstatus );

							if ( id > 0 && id < (MAX_PERSON_NUMBER + MAX_INSTANCE_NPC_NUMBER) )
							{
								monsternum ++;
								
								// 보스 몬스터는 점수는 (기본 점수 * 10)
								if ( pclClient->pclKindInfoSet->pclKI[kind]->IsAtb(ATB_BOSS) )
								{
									pclCM->CR[ id ]->clIdentity.MakeInstanceMap( IDENTITY_INSTANCEMAP, (siScore*10) );
								}
								
								// 게이트 몬스터가 아닐때만 생성
								if ( pclClient->GetUniqueFromHash("KIND_ROBIGATE") != kind )
								{
									pclParentmap->m_siMaxMonsterNum++;
								}
							}

						}
						break;

					default:
						{
							monsternum ++ ;
						}
						break;
				}
			}
		}
	}

	return true ;
}

void cltNPCManagerCommon::SendInstanceMapInfo( SI32 mapindex )
{
	cltServer *pclServer = (cltServer *)pclClient;
	if ( pclServer == NULL ) return ;

	cltMapCommon* pclmap = pclServer->pclMapManager->pclMapArray[mapindex] ;
	if ( pclmap == NULL ) return ;
}

bool cltNPCManagerCommon::CheckTimeOut(SI32 mapindex)
{
	cltServer *pclServer = (cltServer *)pclClient;
	if ( pclServer == NULL ) return false ;

	cltMapCommon* pclmap = pclServer->pclMapManager->pclMapArray[mapindex] ;
	if ( pclmap == NULL ) return false ;

	if ( pclClient->GetFrame() % 100 )
		SendInstanceMapInfo(mapindex) ;

	// 성공여부를 넣는다.
	pclClient->pclLog->FilePrint(TEXT("Config\\Instance.log"), TEXT("mapindex : [%d] : Fail"), mapindex);

	DeleteNPCInstance(mapindex) ;


	return true ;
}

bool cltNPCManagerCommon::DeleteNPCInstance(SI32 parentmapindex)
{
	cltServer *pclServer = (cltServer *)pclClient;
	if ( pclServer == NULL ) return false ;

	cltMapCommon* pclmap = pclServer->pclMapManager->GetMapPointer(parentmapindex);
	if ( pclmap == NULL ) return false ;

	SI32 index = 0;
	SI32 id;
	while(id = pclServer->pclCM->GetCharOrder(index))
	{
		index++;

		if( IsInstanceNPC(id) == false ) continue;
		if ( pclClient->pclMapManager->GetParentMap(pclServer->pclCM->CR[id]->pclMap->siMapIndex) != parentmapindex ) continue ;

		((cltCharServer*)pclServer->pclCM->CR[id])->DeleteInCell();
		if ( pclServer->pclCM->DeleteChar(id) )
		{
			index--;
		}
	}

	return true ;
}

void cltNPCManagerCommon::SetInstanceChar(SI32 mapindex)
{
	cltServer *pclServer = (cltServer *)pclClient;
	if ( pclServer == NULL ) return ;

}

void cltNPCManagerCommon::SendGift(SI32 mapindex,SI32 myid,SI32 enemyid)
{
	cltServer *pclServer = (cltServer *)pclClient;
	if ( pclServer == NULL ) return ;

	// 성공여부를 넣는다.
	pclServer->pclLog->FilePrint(TEXT("Config\\Instance.log"), TEXT("mapindex : [%d] : Success"), mapindex);

	DeleteNPCInstance(mapindex);
}

// 길드인던 몬스터 점수 설정
void cltNPCManagerCommon::SetGuildNPCPoint( SI32 siKind, cltIdentity *pclIdentity )
{
	if ( NULL == pclIdentity )
	{
		return;
	}

	if ( 0 >= siKind )
	{
		return;
	}


	if ( pclClient->GetUniqueFromHash("KIND_RACCOON_AGIT") == siKind )
	{		
		pclIdentity->MakeInstanceMap(IDENTITY_INSTANCEMAP, 1);					
	}
	else if ( pclClient->GetUniqueFromHash("KIND_KINGRACCOON_AGIT") == siKind )
	{		
		pclIdentity->MakeInstanceMap(IDENTITY_INSTANCEMAP, 15);					
	}
	else if ( pclClient->GetUniqueFromHash("KIND_RACCOONLORD_AGIT") == siKind )
	{		
		pclIdentity->MakeInstanceMap(IDENTITY_INSTANCEMAP, 5);					
	}
	else if ( pclClient->GetUniqueFromHash("KIND_ZORRORACCOON_AGIT") == siKind )
	{		
		pclIdentity->MakeInstanceMap(IDENTITY_INSTANCEMAP, 10);					
	}
	else if ( pclClient->GetUniqueFromHash("KIND_SANTARACCOON_AGIT") == siKind )
	{		
		pclIdentity->MakeInstanceMap(IDENTITY_INSTANCEMAP, 20);					
	}
	else if ( pclClient->GetUniqueFromHash("KIND_MAGICALRACCOON_AGIT") == siKind )
	{		
		pclIdentity->MakeInstanceMap(IDENTITY_INSTANCEMAP, 18);					
	}
	else if ( pclClient->GetUniqueFromHash("KIND_RACCOONTOWER_AGIT") == siKind )
	{		
		pclIdentity->MakeInstanceMap(IDENTITY_INSTANCEMAP, 35);					
	}
	else if ( pclClient->GetUniqueFromHash("KIND_MAMARACCOON_AGIT") == siKind )
	{		
		pclIdentity->MakeInstanceMap(IDENTITY_INSTANCEMAP, 100);					
	}
	else if ( pclClient->GetUniqueFromHash("KIND_BIGRACCOON_AGIT") == siKind )
	{		
		pclIdentity->MakeInstanceMap(IDENTITY_INSTANCEMAP, 1000);					
	}
	else if ( pclClient->GetUniqueFromHash("KIND_KEY_RACCOON") == siKind )
	{		
		pclIdentity->MakeInstanceMap(IDENTITY_INSTANCEMAP, 15);					
	}
	else if ( pclClient->GetUniqueFromHash("KIND_GMONBONUS") == siKind )
	{		
		pclIdentity->MakeInstanceMap(IDENTITY_INSTANCEMAP, 1);					
	}

}

void cltNPCManagerCommon::MakeGuildRegenerationNPC( SI32 siMapIndex )
{
	cltServer *pclserver = (cltServer*)pclClient;
	if ( pclserver == NULL )
	{
		return;
	}
	
	if ( siMapIndex < 0 && siMapIndex >= MAX_MAP_NUMBER )
	{
		return;
	}

	if ( pclClient->pclMapManager->IsInstanceMap( siMapIndex ) == false )
	{
		return;
	}
	
	if ( pclClient->pclMapManager->GetInstanceMapType( siMapIndex ) != INSTANCEMAP_TYPE_GUILD )
	{
		return;
	}

	cltMapCommon* pclmap = pclserver->pclMapManager->pclMapArray[siMapIndex];
	if( pclmap == NULL )
	{
		return;
	}

	// 현재 맵에 생성되어 있는 NPC Kind와 마릿수
	SI32 siInHereMapNPCKind[NPCMC_SEARCH_COUNT]	= {0, };
	SI32 siInHereMapNPCNum[NPCMC_SEARCH_COUNT]	= {0, };

	// NPC 그룹 리스트에 있는 NPC Kind와 마릿수
	cltGetNPC clGetNPC[ NPCMC_SEARCH_COUNT ];
	
	// KIND와 마릿수 얻어오기
	GetGuildNPCInformation( siMapIndex, siInHereMapNPCKind, siInHereMapNPCNum );
	
	// 생성될 NPC정보 얻어오기
	GetGroupNPCInformation( siMapIndex, clGetNPC );

	
	for ( SI32 siGetNPCCount=0; siGetNPCCount<NPCMC_SEARCH_COUNT; siGetNPCCount++ )
	{
		SI32 siKind				= clGetNPC[ siGetNPCCount ].GetKind();

		// Kind가 0일때 더이상 생성할 NPC없음
		if ( clGetNPC[ siGetNPCCount ].GetKind() <= 0 )
		{
			break;
		}

		// 리젠, 게이트, 보스 몬스터는 리젠에서 제외
		if ( pclClient->GetUniqueFromHash("KIND_GMONBONUS") == siKind 
			|| pclClient->GetUniqueFromHash("KIND_KEY_RACCOON") == siKind
			|| pclClient->GetUniqueFromHash("KIND_BIGRACCOON_AGIT") == siKind
			)
		{
			continue;
		}

		SI32 siMaxNPCNumber		= clGetNPC[ siGetNPCCount ].GetNumber();
		SI32 siStartNPCNumber	= 0;

		SI32 siLevel			= clGetNPC[ siGetNPCCount ].GetLevel();

		SI32 siRadius			= clGetNPC[ siGetNPCCount ].GetRadius();
		SI32 siX				= clGetNPC[ siGetNPCCount ].GetX();
		SI32 siY				= clGetNPC[ siGetNPCCount ].GetY();

		// 현재 Kind의 NPC가 몇마리 살아 있는지 체크
		for ( SI32 siSearchCount=0; siSearchCount<NPCMC_SEARCH_COUNT; siSearchCount++ )
		{
			if ( siInHereMapNPCKind[ siSearchCount ] <= 0 )
			{
				break;
			}

			if ( siKind == siInHereMapNPCKind[ siSearchCount ] )
			{
				siStartNPCNumber = siInHereMapNPCNum[ siSearchCount ];
				break;
			}
		}

		// 실제로 몬스터 생성
		for ( SI32 siIndex=siStartNPCNumber; siIndex<siMaxNPCNumber; siIndex++ )
		{
			// 더이상 생성할 공간이 없다면, 생성하지 않는다. - 무한루프 방지
			if( pclCM->pclIDManager->GetInstanceNPCNumber() >= MAX_INSTANCE_NPC_NUMBER )		
			{
				return;
			}

			cltInnerStatusInfo clinnerstatus;
			clinnerstatus.clEnemyType.Set(ENEMYTYPE_ALLPC, 0);

			SI32 siPosX = siX + (siRadius/2) - rand() % siRadius;
			SI32 siPosY = siY + (siRadius/2) - rand() % siRadius;
			SI32 siDX	= 0;
			SI32 siDY	= 0;

			// 생성 허용 범위를 넓게 잡아준다.
			if ( pclmap->FindEmptyAreaForCharLocate(siKind, siPosX, siPosY, 20, &siDX, &siDY, true) == false )
			{
#ifdef _DEBUG
				pclClient->pclLog->FilePrint( TEXT("config\\regenerationNPCContinue.txt"), TEXT("%d\t%d\t0\t1\t"), siMapIndex, siKind );
#endif
				continue ;
			}

			cltIdentity	clIdentity;

			// 인던 몬스터 점수
			SetGuildNPCPoint( siKind, &clIdentity );

			SI32 siID = pclserver->SetNPCChar(siKind, VILLAGE_NONE, &clIdentity, siLevel, 0, siDX ,siDY, siMapIndex, NPCITEMMODE_NORMAL, &clinnerstatus );

			if ( siID > 0 && siID < (MAX_PERSON_NUMBER + MAX_INSTANCE_NPC_NUMBER) )
			{
#ifdef _DEBUG
				pclClient->pclLog->FilePrint( TEXT("config\\regenerationNPC.txt"), TEXT("%d\t%d\t0\t1\t"), siMapIndex, siKind );
#endif
			}
			else
			{
#ifdef _DEBUG
				MessageBox(NULL, "MakeGuildRegenerationNPC", "DEBUG", MB_OK);
#endif
				break ;
			}
		}	// for ( SI32 siIndex=siStartNPCNumber; siIndex<siMaxNPCNumber; siIndex++ )
	}
	

}

bool cltNPCManagerCommon::GetGuildNPCInformation( IN SI32 siMapIndex, OUT SI32 *psiKind, OUT SI32 *psiNumber )
{
	if ( siMapIndex < 0 && siMapIndex >= MAX_MAP_NUMBER )
	{
		return false;
	}

	if ( pclClient->pclMapManager->IsInstanceMap( siMapIndex ) == false )
	{
		return false;
	}

	if ( NULL == psiKind )
	{
		return false;
	}

	if ( NULL == psiNumber )
	{
		return false;
	}
	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 현재 siMapIndex에 있는 몬스터들의 KIND와 마리수를 얻어온다
	SI32 siSearchIndex					= 0;
	SI32 siSearchID						= 0;

	SI32 siSearchCount					= 0;


	while( siSearchID = pclClient->pclCM->GetCharOrder(siSearchIndex) )
	{
		siSearchIndex++;

		// 유효한 아이디만 뽑아온다
		if ( IsInstanceNPC(siSearchID) == false )
		{
			continue;
		}

		// 죽은상태라면 뺀다
		if ( pclClient->pclCM->IsAlive( siSearchID ) == false )
		{
			continue;
		}

		cltCharServer* pclchar = (cltCharServer* )pclClient->pclCM->CR[siSearchID];
		if ( pclchar == NULL )
		{
			continue;
		}

		// 해당맵의 케릭터인지
		if( pclchar->GetMapIndex() != siMapIndex )
		{
			continue;
		}

		SI32 siGetKind = pclchar->GetKind();

		for ( siSearchCount=0; siSearchCount<NPCMC_SEARCH_COUNT; siSearchCount++  )
		{
			if ( 0 < psiKind[siSearchCount] )
			{
				if ( siGetKind == psiKind[siSearchCount] )
				{
					psiNumber[ siSearchCount ]++;
					break;
				}

				continue;
			}
			else
			{
				psiKind[ siSearchCount ] = siGetKind;
				psiNumber[ siSearchCount ] = 1;
				break;
			}
		}

	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////

	return true;
}


bool cltNPCManagerCommon::GetGroupNPCInformation( IN SI32 siMapIndex, OUT cltGetNPC *pclGetNPC )
{
	if ( siMapIndex < 0 && siMapIndex >= MAX_MAP_NUMBER )
	{
		return false;
	}

	if ( pclClient->pclMapManager->IsInstanceMap( siMapIndex ) == false )
	{
		return false;
	}

	if ( NULL == pclGetNPC )
	{
		return false;
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	for ( SI32 siIndex=0; siIndex<MAX_NPC_GROUP_NUMBER; siIndex++ )
	{
		if( NULL == pclNPCGroup[siIndex] )
		{
			continue;
		}

		SI32 siNPCMapIndex		= pclNPCGroup[siIndex]->siMapIndex;
		SI32 siNPCKindNumber	= pclNPCGroup[siIndex]->NPCKindNumber;
		SI32 siRadius			= pclNPCGroup[siIndex]->siRadius ;
		SI32 siX				= pclNPCGroup[siIndex]->siX ;
		SI32 siY				= pclNPCGroup[siIndex]->siY ;


		if ( siNPCMapIndex < 0 && siNPCMapIndex >= MAX_MAP_NUMBER )
		{
			continue;
		}
		// 이 몬스터가 속해있는 멥인덱스와 현재멥의 클론 멥 인덱스와 같아야 한다
		if ( siNPCMapIndex != siMapIndex )
		{
			continue;
		}


		for ( SI32 siCount=0; siCount<siNPCKindNumber; siCount++ )
		{
			if ( pclNPCGroup[siIndex]->clUnit[siCount].siKind <= 0 )
			{
				continue;
			}

			SI32 siLevel		= pclNPCGroup[siIndex]->clUnit[siCount].siKindLevel;
			SI32 siMaxNum		= pclNPCGroup[siIndex]->clUnit[siCount].siKindMaxNum;
			SI32 siKind			= pclNPCGroup[siIndex]->clUnit[siCount].siKind;


			for ( SI32 siSearchCount=0; siSearchCount<NPCMC_SEARCH_COUNT; siSearchCount++  )
			{
				if ( 0 <  pclGetNPC[ siSearchCount ].GetKind() )
				{
					if ( siKind == pclGetNPC[ siSearchCount ].GetKind() )
					{
						pclGetNPC[ siSearchCount ].SetNumber( pclGetNPC[ siSearchCount ].GetNumber() + siMaxNum );
						break;
					}

					continue;
				}
				else
				{
					pclGetNPC[ siSearchCount ].SetAll( siRadius, siX, siY, siLevel, siKind, siMaxNum );
					break;
				}
			}

		}	// for ( SI32 siCount=0; siCount<siNPCKindNumber; siCount++ ) 종료
	}	// for ( SI32 siIndex=0; siIndex<MAX_NPC_GROUP_NUMBER; siIndex++ ) 종료

	return true;
}
