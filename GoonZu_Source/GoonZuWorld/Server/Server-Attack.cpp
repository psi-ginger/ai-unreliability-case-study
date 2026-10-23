//---------------------------------
// 2003/5/17 김태곤
//---------------------------------

#include "Char/CharManager/CharManager.h"
//----------------------------------------
// CommonLogic
//----------------------------------------
#include "../CommonLogic/MagicMgr/MagicMgr.h"
#include "../CommonLogic/Hunt/Hunt.h"
#include "../CommonLogic/Cityhall/Cityhall.h"

#include "../CommonLogic/MsgType-Person.h"
#include "../CommonLogic/MsgRval-Define.h"
#include "../CommonLogic/MsgType-System.h"
#include "../CommonLogic/Msg/MsgType-Summon.h"

#include "../CommonLogic/Msg/MsgType-RacoonDodgeEvent.h"
#include "../CommonLogic/MsgType.h"


#include "../CommonLogic/MagicMgr/MagicMgr.h"
#include "../CommonLogic/MagicMgr/MagicAttribute/MagicAttribute.h"

#include "../CommonLogic/BlackArmy/BlackArmy.h"

#include "../CommonLogic/Marriage/MarriageManager.h"

//----------------------------------------
// Common
//----------------------------------------
#include "../Common/Map/Map.h"
#include "../Common/Order/Order.h"
#include "../Common/Map/MapManager/MapManager.h"
#include "../Common/Skill/Skill-Manager.h"
#include "../Common/PartyMgr/PartyObj.h"
#include "../Common/PartyMgr/PartyMgr.h"
#include "../Common/NPCManager/NPC.h"
#include "../common/Item/ItemCommon/cltSetItem.h"
#include "../common/Event/MofuMofu/MofuMofu.h"

//-----------------------------------------
// Server
//-----------------------------------------
#include "Server.h"
#include "Statistics/Statistics.h"
#include "Minister/MinisterBase.h"
#include "Minister/MinisterMgr.h"
#include "Minister/Minister-Hyungzo.h"


//----------------------------------------
// DBManager
//----------------------------------------
#include "../../../DBManager/GameDBManager_World/DBMsg-Item.h"
#include "../../../DBManager/GameDBManager_World/DBMsg-Person.h"
#include "../../../DBManager/GameDBManager_World/DBMsg-PostOffice.h"

#include "../../../Server/AuthServer/AuthMsg.h"

#include "../Server/FourLeafManager/ForleafManager.h"

#include "../common/Util/Util.h"

extern cltCommonLogic* pclClient;

#define CAPTURE_DOLLNUM	1		// 포획술로 획득할 수 있는 인형 갯수
#define MAX_UNIQUENUM	20	//m_100healmanager.txt 에 들어갈 최대 유니크 갯수

// 마법에 의한 캐릭터의 조치 계산. 
void cltServer::MagicPopAction()
{
	BYTE data[256];
	SI32 i, j;
	stMagicMsgHeader* psHeader;
	SI32 hitrate = 0; 
	SI32 dodgerate = 0; 

	SI32 size;
	while(pclMagicManager->Pop(data, &size))
	{
		if(size>=256)
		{
			psHeader = (stMagicMsgHeader*)data;
			MsgBox(TEXT("dsfv8dfjd"), TEXT("fSDfcv9df:%d"), psHeader->si32MsgKind);
			continue;
		}

		psHeader = (stMagicMsgHeader*)data;
		switch(psHeader->si32MsgKind)
		{
		case MAGIC_DAMAGE:
			{
				SI32 siAttackType = 0;
				switch(psHeader->si32MagicKind)
				{
				case MAGIC_MON_FROZENSORCERER:	siAttackType = ATTACKTYPE_ICE;		break;
				default:						siAttackType = ATTACKTYPE_MAGIC;	break;
				}

				stMagicMsgDamage* pclMagic = (stMagicMsgDamage*)data;

				SI32 fromid	= pclCM->GetIDFromCharUnique(pclMagic->si32FromID);
				if(pclCM->IsAlive(fromid) == false)return ;

				SI32 toid	= pclCM->GetIDFromCharUnique(pclMagic->si32ToID);
				if(pclCM->IsAlive(toid) == false)return ;

				SI32 damage = pclMagic->si32Damage;

				// 마법에도 민첩이 영향을 받게 pvp-
				// by LEEKH 2007.10.10 - PVP 국가스위치를 ServerInfo로 변경
				//if( IsWhereServiceArea(ConstSwitch_PVP) )

				if(pclClient->IsCountrySwitch(Switch_HealthAddChange))
				{	
					hitrate   = pclCM->CR[fromid]->clPB.GetHitRate( pclCM->CR[toid], pclClient->pclCM->CR[fromid], true );	
					dodgerate = pclCM->CR[toid]->clPB.GetDodgeRate( pclCM->CR[fromid], pclClient->pclCM->CR[toid], true );	// 적의 회피율이 필요하다.

				}
				else
				{
					if(siEnablePVP == 1)
					{
						hitrate = pclCM->CR[fromid]->clPB.GetHitRate( pclCM->CR[toid], pclClient->pclCM->CR[fromid], true );	
					}
					else
						hitrate = 100;

					dodgerate = 0;
				}
				cltCharPos clpos(pclCM->CR[toid]->GetMapIndex(), pclCM->CR[toid]->GetX(), pclCM->CR[toid]->GetY());
				cltAttackTarget cltarget(toid, &clpos, hitrate , dodgerate);
				cltAttackDamageInfo cldamageinfo( siAttackType , damage, pclMagic->sMagicMsgHeader.si32MagicKind);
				HitOperation(fromid, &cldamageinfo, 1, &cltarget, pclCM->CR[toid]->pclMap);
			}
			break;
		case MAGIC_CIRCLE_DAMAGE:
			{
				SI32 siAttackType = 0;
				switch(psHeader->si32MagicKind)
				{
				case MAGIC_MON_POISONFLAME:		
				case MAGIC_MON_TOADPOISON:		siAttackType = ATTACKTYPE_POISON;			break;
				case MAGIC_MON_FIREBURN:		siAttackType = ATTACKTYPE_MAGICFIREBURN;	break;
				default:						siAttackType = ATTACKTYPE_MAGIC;			break;
				}

				stMagicMsgCircleDamage* pclMagic = (stMagicMsgCircleDamage*)data;

				SI32 fromid		= pclCM->GetIDFromCharUnique(pclMagic->si32FromID);
				SI32 toid		= pclCM->GetIDFromCharUnique(pclMagic->si32ToID);
				if(pclCM->IsAlive(fromid) == false)return ;
				SI32 x			= pclCM->CR[fromid]->GetX();
				SI32 y			= pclCM->CR[fromid]->GetY();

				cltMapCommon* pclMap = pclCM->CR[fromid]->pclMap;
				if(pclMap == NULL)return ;

				SI32 damage		= pclMagic->si32Damage;
				SI32 radius		= max(1, pclMagic->si32Radius);
				SI32 curradius	= max(1, pclMagic->si32CurrentRadius);
				SI32 id			= 0;
				SI32 length		= 0;

				// [진성] 소환수 드래곤의 스턴 마법시 타겟이 된 캐릭에게 데미지 주기..
				if( MAGIC_MON_SUMMONDRAGON_STUN == psHeader->si32MagicKind )
				{
					cltCharServer* pclChar = pclCM->GetCharServer( toid );
                    if( pclChar ) 
					{
						if(pclClient->IsCountrySwitch(Switch_HealthAddChange))
						{	
							hitrate = pclCM->CR[fromid]->clPB.GetHitRate( pclChar, pclClient->pclCM->CR[fromid], true );	
							dodgerate = pclChar->clPB.GetDodgeRate( pclCM->CR[fromid], pclChar, true );	
						}
						else
						{
							if(siEnablePVP == 1)
							{
								hitrate = pclCM->CR[fromid]->clPB.GetHitRate( pclChar, pclCM->CR[fromid], true );	
							}
							else
								hitrate = 100;

							dodgerate = 0;
						}

						cltCharPos clpos(pclChar->GetMapIndex(), pclChar->GetX(), pclChar->GetY());
						cltAttackTarget cltarget(pclChar->GetID(), &clpos, hitrate , dodgerate); 
						cltAttackDamageInfo cldamageinfo(siAttackType, damage * 2, psHeader->si32MagicKind); 

						HitOperation(fromid, &cldamageinfo, 1, &cltarget, pclChar->pclMap);
					}
				}

				for(i = -curradius; i <= curradius; i++)
				{
					for(j = -curradius; j <= curradius; j++)
					{
						if(TABS(i) != curradius && TABS(j) != curradius)continue;

						id = pclMap->GetIDMap(x+j, y+i);

						if(pclCM->IsAlive(id)			  == false)	continue;
						if(pclCM->CR[fromid]->IsEnemy(id) == false)	continue;

						// [진성] 소환수 드래곤의 스턴 마법.
						if( MAGIC_MON_SUMMONDRAGON_STUN == psHeader->si32MagicKind )
						{
							cltCharServer* pclChar = pclCM->GetCharServer(id);
							if( NULL == pclChar ) continue;
	
							pclChar->SetStun( 5 );		
						}
						
						if(pclClient->IsCountrySwitch(Switch_HealthAddChange))
						{	
							hitrate = pclCM->CR[fromid]->clPB.GetHitRate( pclCM->CR[id], pclCM->CR[fromid], true );	
							dodgerate = pclCM->CR[id]->clPB.GetDodgeRate( pclCM->CR[fromid], pclCM->CR[id], true );	
						}
						else
						{
							if(siEnablePVP == 1)
							{
								hitrate = pclCM->CR[fromid]->clPB.GetHitRate( pclCM->CR[id], pclCM->CR[fromid], true );	
							}
							else
								hitrate = 100;

							dodgerate = 0;
						}

						length = min(radius, max( TABS(i), TABS(j)) ) ;
						cltCharPos clpos(pclCM->CR[id]->GetMapIndex(), pclCM->CR[id]->GetX(), pclCM->CR[id]->GetY());
						cltAttackTarget cltarget(id, &clpos, hitrate , dodgerate);
						cltAttackDamageInfo cldamageinfo(siAttackType, damage * max(1, (radius - length)) / radius, psHeader->si32MagicKind);

						HitOperation(fromid, &cldamageinfo, 1, &cltarget, pclCM->CR[id]->pclMap);
					}
				}
			}
			break;

		case MAGIC_WHIRLPOOL_DAMAGE:
			{
				stMagicMsgWhirlPoolDamage* pclMagic = (stMagicMsgWhirlPoolDamage*)data;

				SI32 fromid	= pclCM->GetIDFromCharUnique(pclMagic->si32FromID);
				if ( fromid < 1 )
					return;
				SI32 toid	= pclCM->GetIDFromCharUnique(pclMagic->si32ToID);
				if ( toid < 1 )
					return;
				SI32 damage = pclMagic->si32Damage;
				if ( damage < 2 )
				{
					//tsoi 	printf(TEXT("ets"));
				}
				SI32 radius	= pclMagic->si32Radius;
				if(radius <=0 )
				{
					radius = 1;
				}

				if(pclCM->IsAlive(fromid) == false)return ;

				cltMapCommon* pclMap = pclCM->CR[fromid]->pclMap;
				if(pclMap == NULL)return ;

				SI32 x = pclMagic->si32XCenterPos;
				SI32 y = pclMagic->si32YCenterPos;

				//------------------------------------------------
				// 원래 타겟은 반드시 맞는다. 
				//------------------------------------------------

				if(pclClient->IsCountrySwitch(Switch_HealthAddChange))
				{	
					hitrate = pclCM->CR[fromid]->clPB.GetHitRate( pclCM->CR[toid], pclCM->CR[fromid], true );	
					dodgerate = pclCM->CR[toid]->clPB.GetDodgeRate( pclCM->CR[fromid], pclCM->CR[toid], true );	

				}
				else
				{
					switch ( siServiceArea )
					{
					case ConstServiceArea_English:
					case ConstServiceArea_NHNChina:
					case ConstServiceArea_EUROPE:
						{
							hitrate = pclCM->CR[fromid]->clPB.GetHitRate( pclCM->CR[toid], pclCM->CR[fromid], true );	
						}
						break;
					default:
						{
							hitrate = 100;
						}
						break;

					}

					dodgerate = 0;
				}
				cltCharPos clpos(pclCM->CR[toid]->GetMapIndex(), pclCM->CR[toid]->GetX(), pclCM->CR[toid]->GetY());
				cltAttackTarget cltarget(toid, &clpos, hitrate , dodgerate);
				cltAttackDamageInfo cldamageinfo(ATTACKTYPE_MAGIC, damage, psHeader->si32MagicKind);
				HitOperation(fromid, &cldamageinfo, 1, &cltarget, pclCM->CR[toid]->pclMap);

				SI32 id;
				for(i = -radius; i <= radius; i++)
				{
					for(j = -radius; j <= radius; j++)
					{
						id = pclMap->GetIDMap(x+j, y+i);

						//-----------------------------------------------
						// 원래 타겟은 이미 맞았으므로 제외한다. 
						//-----------------------------------------------
						if(id == toid)continue;
						if(pclCM->IsAlive(id) == false)continue;
						if(pclCM->CR[fromid]->IsEnemy(id)== false)continue;

						// 당분간은 한 캐릭터만 공격 영향을 주기 위해서...
						if ( id == toid && pclCM->IsAlive(id) && pclCM->CR[fromid]->IsEnemy(id) )
						{
							// 명중률 고려 pvp-
							// by LEEKH 2007.10.10 - PVP 국가스위치를 ServerInfo로 변경
							//if( IsWhereServiceArea(ConstSwitch_PVP) )
							if(pclClient->IsCountrySwitch(Switch_HealthAddChange))
							{	
								hitrate = pclCM->CR[fromid]->clPB.GetHitRate( pclCM->CR[id], pclCM->CR[fromid], true );	
								dodgerate = pclCM->CR[id]->clPB.GetDodgeRate( pclCM->CR[fromid], pclCM->CR[id], true );	
							}
							else
							{
								if(siEnablePVP == 1)
								{
									hitrate = pclCM->CR[fromid]->clPB.GetHitRate( pclCM->CR[id], pclCM->CR[fromid], true );	
								}
								else
									hitrate = 100;

								dodgerate = 0;
							}

							cltCharPos clpos(pclCM->CR[id]->GetMapIndex(), pclCM->CR[id]->GetX(), pclCM->CR[id]->GetY());
							cltAttackTarget cltarget(id, &clpos, hitrate , dodgerate);
							cltAttackDamageInfo cldamageinfo(ATTACKTYPE_MAGIC, damage, pclMagic->sMagicMsgHeader.si32MagicKind);
							HitOperation(fromid, &cldamageinfo, 1, &cltarget, pclCM->CR[id]->pclMap);

						}
					}
				}

			}
			break;

		case MAGIC_INCREASE_LIFE:
			{
				stMagicMsgDamage* pclMagic = (stMagicMsgDamage*)data;

				SI32 fromid	= pclCM->GetIDFromCharUnique(pclMagic->si32FromID);
				if ( fromid < 1 )
					return;
				SI32 toid	= pclCM->GetIDFromCharUnique(pclMagic->si32ToID);
				if ( toid < 1 )
					return;
				SI32 damage = pclMagic->si32Damage;
				if(pclCM->IsAlive(toid) && pclCM->IsAlive(fromid))
				{
					pclCM->CR[toid]->IncreaseLife(damage);

					// 이웃에게 통보 
					cltCharChangeStatusInfo	StatusInfo(CHANGETYPE_HEAL, true, pclCM->CR[toid]->GetLife(), damage);
					cltGameMsgResponse_StatusInfo clChangeInfo(pclCM->CR[toid]->GetCharUnique(), &StatusInfo);
					cltMsg clMsg(GAMEMSG_RESPONSE_CHANGESTATUSINFO, sizeof(clChangeInfo), (BYTE*)&clChangeInfo);
					((cltCharServer*)pclCM->CR[toid])->SendNeighbourMsg(&clMsg, true);

					//==========================================
					// 주인이 갖고 있는 소환수의 정보를 설정하고 통보 한다.
					//==========================================
					SI32 siParentID = pclCM->CR[toid]->GetSummonParentID();
					if(pclCM->IsValidID(siParentID))
					{
						//==========================================
						// 
						//==========================================
						cltSummon *pclSummon = pclCM->CR[siParentID]->pclCI->clSummonInfo.GetSummon(pclCM->CR[toid]->GetSummonIndex());
						if(pclSummon)
						{
							pclSummon->clIP.Set(&pclCM->CR[toid]->pclCI->clIP);
							cltGameMsgResponse_Summon clInfo(pclCM->CR[siParentID]->GetCharUnique(), pclCM->CR[toid]->GetSummonIndex(), pclSummon);
							cltMsg clMsg(GAMEMSG_RESPONSE_SUMMONINFO, sizeof(clInfo), (BYTE*)&clInfo);
							pclCM->CR[siParentID]->SendNetMsg((sPacketHeader*)&clMsg);
						}
					}

				}
			}
			break;

		default:
			MsgBox(TEXT("fdsf93jfdf"), TEXT("dv9ejd:%d"), psHeader->si32MagicKind);
		}
	}
}


// 공격의 여러 속성을 고려하여 적의 체력을 저하시킨다. 
// 이는 서버에서만 이루어지는 작업이다. 
BOOL cltServer::HitOperation(SI32 myid, cltAttackDamageInfo* pcldamageinfo, SHORT range, const cltAttackTarget* pcltarget, cltMapCommon* pclMap)
{

	SHORT length=0;
	SHORT damage=0;
	SHORT realpower = 0;
	SI32 id;
	cltAttackTargetList clList;

	SI32 attackid	= pcltarget->GetAttackID();
	if(pclCM->IsValidID(attackid) == false)		return FALSE;
	if(pclCM->IsValidID(myid) == false)			return FALSE;

	SI32 dx = pcltarget->clPos.siX;
	SI32 dy = pcltarget->clPos.siY;

	// 여기 들어온건 맞았다는 의미이다.
	// 그냥 맞으면 일단 마을로 보낸다 
	// 라쿤 피하기 이벤트 
	//Switch_10MonthEvent_RacoonDodge
	//if(pclClient->IsCountrySwitch(Switch_10MonthEvent_RacoonDodge))
	//{
	//	cltCharCommon* pclAttackChar = pclCM->GetChar( myid)	;
	//	if( pclAttackChar != NULL )
	//	{
	//		if(pclAttackChar->clIdentity.siIdentity == IDENTITY_RACOONDODGE_EVENT)
	//		{
	//			cltCharCommon* pclChar = pclCM->GetChar( attackid )	;// 맞은 사람
	//			if ( pclChar != NULL )
	//			{
	//				cltWarpInfo clWarpInfo(1, 0, true, false, 38, 33, -1, -1);
	//				clWarpInfo.uiMapIndex = PLAY_MAP_INDEX_INDODEVENT;

	//				pclChar->SetWarpInfo(&clWarpInfo, 2200);	// 해당 위치로 워프 한다 .
	//				pclChar->Warp()	;

	//				cltGameMsgResponse_30Seceond_Clear clinfo( pclChar->pclCI->GetPersonID() );
	//				cltMsg clMsg(GAMEMSG_RESPONS_RACOONDODGE_EVENT_30SECOND_CLEAR, sizeof(clinfo), (BYTE*)&clinfo);
	//				pclChar->SendNetMsg((sPacketHeader*)&clMsg);

	//				cltGameMsgResponse_Round_Clear clinfo2( pclChar->pclCI->GetPersonID() );
	//				cltMsg clMsg2(GAMEMSG_RESPONS_RACOONDODGE_EVENT_ROUNDCOUNT_CLEAR, sizeof(clinfo2), (BYTE*)&clinfo2);
	//				pclChar->SendNetMsg((sPacketHeader*)&clMsg2);
	//			}

	//		}
	//	}
	//}

	if(pclClient->GameMode == GAMEMODE_SERVER)
	{
		if(pclClient->IsCountrySwitch(Switch_SoulGuard))
		{
			if ( IsPC(attackid) )
			{
				cltCharServer* pclCharServer = pclCM->GetCharServer(attackid)	;
				if(pclCharServer != NULL )
				{
					// pvp때는 발동 하지 않는다
					if(pclCharServer->siEnemUnique > 0 )
					{
					}
					else
					{
						SI32 siValue =  pclCharServer->SoulGuardAbilityActivity( SOULGUARD_ABILITY_DEFENSE_SUMMON );
						if ( 0 < siValue )
						{
							// 소환한다 몬스터를 
							// 가고일 소환 해야지
							// 생명은 20초 IDENTITY_SOULGUARD_SUMMONPUMKIN

							///myid가 엔피씨 인지 아닌지 확인 한다.

							cltInnerStatusInfo clinnerstatus;
							clinnerstatus.clEnemyType.Set(ENEMYTYPE_ALLNPC, 0);

							cltIdentity clIdentity;
							clIdentity.MakeRaccoonDodgeEvent(IDENTITY_SOULGUARD_SUMMONPUMKIN);

							SI32 siMonsterKind	=	siValue	;
							SI32 siRocX		=	pclCharServer->GetX()	;// 나의 위치
							SI32 siRocY		=	pclCharServer->GetY()	;
							SI32 siMapIndex	=	pclCharServer->GetMapIndex()	;

							SI32 siEnergy	=	5000	;	// 이건 가고일 의 능력을 받아 와야 하는데 ..
							SI32 siLevel	=	99	;


							SI32 id = SetNPCChar( siMonsterKind,
								VILLAGE_NONE, 
								&clIdentity, 
								siLevel, 
								siEnergy, 
								siRocX, 
								siRocY,
								siMapIndex, 
								NPCITEMMODE_NONE, 
								&clinnerstatus );

							// 해당 npc의 적 셋팅 
							cltCharServer* pclCharServer_Summon = pclCM->GetCharServer(id)	;
							if( pclCharServer_Summon != NULL)// 정상적으로 소환이 되었다면 
							{
								pclCharServer_Summon->SetReservedAttackTargetID( myid)	;
								// 20초 뒤에 죽게 하려고 시간 셋팅 
								pclCharServer_Summon->dwTimerClock = CurrentClock;
							}
						}

					}
				}
			}
			
		}
	}


	// 복불복 게임
	if ( pclClient->IsCountrySwitch(Switch_BokBulBok))
	{
		cltCharServer* pclAttackChar = pclCM->GetCharServer( myid );	// 때린사람

		if ( pclAttackChar != NULL )
		{
			cltCharCommon* pclDamagedChar = pclCM->GetChar( attackid )	;// 맞은 사람

			SI32 siDamagedMapIndex = pclDamagedChar->GetMapIndex();
			SI32 siAttackerMapIndex = pclAttackChar->GetMapIndex();

			if ( siDamagedMapIndex == siAttackerMapIndex ) // 97 & 
			{
				cltMapCommon* pDamagedMap = pclMapManager->GetMapPointer( siDamagedMapIndex );
				cltMapCommon* pAttackerMap = pclMapManager->GetMapPointer( siAttackerMapIndex );

				if ( NULL != pDamagedMap && NULL != pAttackerMap )
				{
					SI32 siDamagedColl = pDamagedMap->CalcCollInfo( pclDamagedChar->GetX(), pclDamagedChar->GetY() );
					SI32 siAttackerColl = pAttackerMap->CalcCollInfo( pclAttackChar->GetX(), pclAttackChar->GetY() );

					if ( siDamagedColl == siAttackerColl )
					{
						if ( pclDamagedChar->clIdentity.siIdentity == IDENTITY_BOKBULBOK_ONEKILLMONSTER )
						{
							pclClient->m_pServer_BBBMgr->StageFail( myid );
						}

						/*else if ( pclDamagedChar->clIdentity.siIdentity == IDENTITY_BOKBULBOK_EVENTMONSTER ||
							pclDamagedChar->clIdentity.siIdentity == IDENTITY_BOKBULBOK_KEYMONSTER )
						{
						}*/
					}
					else
					{
						return FALSE;
					}
				}
				else
				{
					//return FALSE;
				}
			}
			else
			{
				//return FALSE;
			}
		}
		else
		{
			//return FALSE;
		}
	}

	// 피격되고 있음을 알린다. 
	if(pclCM->IsAlive(attackid) )
	{
		((cltCharServer*)pclCM->CR[attackid])->SetBeAttackedSwitch(this, myid);
	}

	// 명중률을 근거로 타격 여부를 결정한다.
	if(pclClient->IsCountrySwitch(Switch_HealthAddChange))
	{
		SI16 randHit = rand() % 10000;
		//SI32 lastHitVal = 0 ;

		//if(pcltarget->siDodgeRate > 0)
		//	lastHitVal = (pcltarget->siHitRate - (pcltarget->siDodgeRate/2));
		//else	
		//	lastHitVal = pcltarget->siHitRate;

		//(pcltarget->siHitRate < randHitRate)||( pcltarget->siDodgeRate >randDodgeRate)

		UI08 type = 0;
		if( true == (pclCM->CR[attackid]->pclKI != NULL && pclCM->CR[attackid]->pclKI->IsAtb( ATB_MONSTER )) )	{		type = 0;		}
		else															{		type = 1;		}

		// DODGE
		if( pcltarget->siHitRate < randHit && randHit <= pcltarget->siHitRate + (pcltarget->siDodgeRate/2) )
		{
			cltGameMsgResponse_Hit clHitInfo(pclCM->CR[attackid]->GetCharUnique(), 0, 0, 0, 0, 0, false, false, true, type );
			cltMsg clMsg(GAMEMSG_RESPONSE_HIT, sizeof(clHitInfo), (BYTE*)&clHitInfo);
			((cltCharServer*)pclCM->CR[myid])->SendNeighbourMsg(&clMsg, true);

			return FALSE;
		}
		// MISS
		else if ( pcltarget->siHitRate + (pcltarget->siDodgeRate/2) < randHit )
		{
			cltGameMsgResponse_Hit clHitInfo(pclCM->CR[attackid]->GetCharUnique(), 0, 0, 0, 0, 0, false, true, false, type );
			cltMsg clMsg(GAMEMSG_RESPONSE_HIT, sizeof(clHitInfo), (BYTE*)&clHitInfo);
			((cltCharServer*)pclCM->CR[myid])->SendNeighbourMsg(&clMsg, true);

			return FALSE;
		}

		//if( lastHitVal < randHit)
		//{
		//	cltGameMsgResponse_Hit clHitInfo(pclCM->CR[attackid]->GetCharUnique(), 0, 0, 0, 0, 0, false, false, true, type );
		//	cltMsg clMsg(GAMEMSG_RESPONSE_HIT, sizeof(clHitInfo), (BYTE*)&clHitInfo);
		//	((cltCharServer*)pclCM->CR[myid])->SendNeighbourMsg(&clMsg, true);

		//	return FALSE;
		//}
	}
	else
	{
		if(rand() % 100 > pcltarget->siHitRate)
		{
			UI08 type = 0;
			if( true == (pclCM->CR[attackid]->pclKI != NULL && pclCM->CR[attackid]->pclKI->IsAtb( ATB_MONSTER )) )
			{
				type = 0;
			}
			else
			{
				type = 1;
			}

			cltGameMsgResponse_Hit clHitInfo(pclCM->CR[attackid]->GetCharUnique(), 0, 0, 0, 0, 0, false, true, false, type );
			cltMsg clMsg(GAMEMSG_RESPONSE_HIT, sizeof(clHitInfo), (BYTE*)&clHitInfo);
			((cltCharServer*)pclCM->CR[myid])->SendNeighbourMsg(&clMsg, true);

			return FALSE;
		}
	}

	// 공격 범위 속성. 
	SI32 rangeflag = 0;
	if(pcldamageinfo->siAttackType >= 0 && pcldamageinfo->siAttackType < MAX_ATTACKTYPE_NUMBER )
		rangeflag = pclAttackTypeInfoManager->pclAttackTypeInfo[pcldamageinfo->siAttackType]->GetFlagRange();

	if( rangeflag & ATTACKTYPE_ATB_RANGE1 )
	{
		// 공격자가 무효하면 리턴. 
		if(pclCM->IsAlive(myid)==FALSE)return FALSE;
		// 목표가 무효하면 리턴
		if(pclCM->IsAlive(attackid)==FALSE)return FALSE;

		//[진성] 독이면 맵을 검사 하지 않는다.
		if(		false == pclCM->CR[attackid]->clInnerStatus.IsStatus(INNERSTATUS_POISON)
			||	false == pclCM->CR[attackid]->clInnerStatus.IsStatus(INNERSTATUS_POISON_SETITEM_EFFECT) )
		{
			// 같은 지도 상에 있어야 한다. 
			if(pclCM->CR[attackid]->GetMapIndex() != pclCM->CR[myid]->GetMapIndex())
				return FALSE;
		}

		if(pclCM->CR[myid]->IsInRange(attackid, range))
		{
			clList.Push(attackid, pcldamageinfo);
		}

	}
	else if( rangeflag & ATTACKTYPE_ATB_RANGE2 )
	{
		// 목표가 무효하면 리턴, 
		if(pclCM->IsAlive(attackid)==FALSE)return FALSE;

		//[진성] 독이면 맵을 검사 하지 않는다.
		if(		false == pclCM->CR[attackid]->clInnerStatus.IsStatus(INNERSTATUS_POISON)
			||	false == pclCM->CR[attackid]->clInnerStatus.IsStatus(INNERSTATUS_POISON_SETITEM_EFFECT)	)
		{
			// 같은 지도 상에 있어야 한다. 
			if(pclCM->CR[attackid]->GetMapIndex() != pclCM->CR[myid]->GetMapIndex())return FALSE;
		}

		clList.Push(attackid, pcldamageinfo);
	}

	else if( rangeflag & ATTACKTYPE_ATB_RANGE3 )
	{
		if(range<=0)range = 2;

		// 공격자가 무효하면 리턴. 
		if(pclCM->IsValidID(myid)==FALSE)return FALSE;


		SI32 index = 0;
		while(id = pclCM->GetCharOrder(index))
		{
			index++;

			if( pclCM->IsAlive(id) && pclCM->CR[myid]->IsEnemy(id) == true )
			{
				// 같은 지도 상에 있어야 한다. 
				if(pclCM->CR[id]->GetMapIndex() != pclCM->CR[myid]->GetMapIndex())continue;

				if(clList.IsInList(id) == FALSE)
				{
					length		=	max(TABS(pclCM->CR[id]->GetX() - dx), TABS(pclCM->CR[id]->GetY() - dy));
					if(length > range)continue;

					realpower	=	pcldamageinfo->siDamage - pcldamageinfo->siDamage*( length*100 / (range+1) )/100;

					cltAttackDamageInfo cldamage;
					cldamage.Set(pcldamageinfo);
					cldamage.siDamage = realpower;

					clList.Push(id, &cldamage);
				}
			}
		}

	} 
	else
	{
		MsgBox(TEXT("A1284"), TEXT("attacktype:%d kind:%d"), pcldamageinfo->siAttackType, pclCM->CR[myid]->GetKind());
	}


	//------------------------------------------
	// 공격해야 할 타겟을 공격한다. 
	//------------------------------------------
	SI32 hitid;
	cltAttackDamageInfo cldamageinfo;
	SI32 siCriticalRate = 0;
	while(clList.Pop(&hitid, &cldamageinfo) == TRUE)
	{
		// 필살기로 작동하면 공격력을 2배로 준다. 
		siCriticalRate = pclCM->CR[myid]->clPB.GetCriticalRate();

		//과금 총알에 의한 크리티컬 상승율[성웅]
		if (pclCM->CR[myid]->pclCI->clCharItem.clItem[PERSONITEM_WEAPON1].siUnique == ITEMUNIQUE(13601) ||
			pclCM->CR[myid]->pclCI->clCharItem.clItem[PERSONITEM_WEAPON2].siUnique == ITEMUNIQUE(13601) ||
			pclCM->CR[myid]->pclCI->clCharItem.clItem[PERSONITEM_WEAPON1].siUnique == ITEMUNIQUE(13600) || 
			pclCM->CR[myid]->pclCI->clCharItem.clItem[PERSONITEM_WEAPON2].siUnique == ITEMUNIQUE(13600))
		{
			siCriticalRate = siCriticalRate +((siCriticalRate/100)*5);
		}

		// 마법에 영향을 받는 Critical을 구한다.
		if(pclCM->CR[myid]->clInnerStatus.IsStatus(INNERSTATUS_CURSECRITICAL))
		{
			siCriticalRate -= siCriticalRate * pclCM->CR[myid]->siCurseCriticalPower / 100;
		}

		// 버프 마법에 의한 크리티컬률 상승
		if ( IsPC(myid) )
		{
			SI32 siAddCriticalRate = pclCM->CR[myid]->m_clMagicBufMgr.GetIncCriticalRate( pclCM->CR[myid] );
			if ( siAddCriticalRate )
			{
				SI32 siCalcCriticalRate = ((siCriticalRate / 100) * siAddCriticalRate);
				siCriticalRate += siCalcCriticalRate;
				pclClient->pclLog->LogPrint( TEXT("크리티컬 증가 : %d\n"), siCriticalRate );
			}
		}

		if ( pclClient->IsCountrySwitch(Switch_Set_Item_Add) )
		{
			//[진성] 세트 아이템 으로 인한 크리티컬률 변경.
			if( pclCM->CR[myid]->m_siSetItemType )
			{
				siCriticalRate += siCriticalRate *	(	pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemAbilityValue( pclCM->CR[myid], SETITEM_ABILITY_TYPE_CRITICAL, USE_SETITEM_1 )
													  + pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemAbilityValue( pclCM->CR[myid], SETITEM_ABILITY_TYPE_CRITICAL, USE_SETITEM_2 )
													  + pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemAbilityValue( pclCM->CR[myid], SETITEM_ABILITY_TYPE_CRITICAL, USE_SETITEM_3 )
													  + pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemAbilityValue( pclCM->CR[myid], SETITEM_ABILITY_TYPE_CRITICAL, USE_SETITEM_4 )
													  + pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemAbilityValue( pclCM->CR[myid], SETITEM_ABILITY_TYPE_CRITICAL, USE_SETITEM_5 )) / 100;
			}
		}

		// PCK: 수박버프추가 - 수박버프가 걸려있고, 탈것을 탄상태이면 5%의 확률로 추가타를 넣어준다. (크리티컬과 같은 방식)
		// 단, 표시하는 방법은 크리티컬 데미지 / 2 해서 각각 데미지를 표시하는 방식으로 한다.

		cltCharServer *Mypclchar = (cltCharServer*)pclCM->CR[myid];
		cltBufInfo clBuf;
		clBuf.SetBuf( &Mypclchar->pclCI->clBI.clBufInfo );

		if(pclCM->IsAlive(myid))
		{
			if(siCriticalRate > (rand() % 1000))
			{
				Hit(myid, hitid, &cldamageinfo, 1);
			}
			else
			{
				if(clBuf.IsHaveBuf(BUF_WATERMELON) && (Mypclchar->pclCI->clHorseInfo.siCurrentHorseRIDEHORSE == TRUE) && (rand() % 100 < 20))
				{
					Hit(myid, hitid, &cldamageinfo, 2);
				}
				else
				{
					Hit(myid, hitid, &cldamageinfo, 0);
				}
			}
		}

		//PCK : 수호정령 특수능력 - 몬스터에게 맞을시 일정확률로 마법을 발사한다.(09.07.07)
		if(IsCountrySwitch(Switch_SoulGuard))
		{
			if( IsPC( attackid ) )
			{	
				SI32 siMagicUnique = ((cltCharServer*)pclCM->CR[attackid])->SoulGuardAbilityActivity( SOULGUARD_ABILITY_DEFENSE_MAGIC );
				SI32 returnval = 0;

				if ( 0 < siMagicUnique )
				{
					SendServerResponseMsg(0, SRVAL_RETURN_SOULGUARD_SHOT_MAGIC,  myid, siMagicUnique, pclCM->CR[attackid]->GetCharUnique());
				}
			}
			if( IsPC( myid ) )  // myid  - 공격 , attackid - 맞는사람
			{
				// 공격 할때 일정 확률로 마법을 쏜다 펌킨
				cltCharServer* pCharServer = pclCM->GetCharServer( myid)	;
				if(pCharServer != NULL)
				{
					SI32 siMagicUnique = pCharServer->SoulGuardAbilityActivity( SOULGUARD_ABILITY_ATTACK_MAGIC )	;
					if(siMagicUnique > 0 )
					{
						SendServerResponseMsg(0, SRVAL_RETURN_SOULGUARD_SHOT_MAGIC,  attackid  , siMagicUnique, pclCM->CR[myid]->GetCharUnique() );
					}
				}
			}
			if( IsPC( myid ) )
			{
				// 공격 할때 일정 확률로 마법을 쏜다 펌킨
				cltCharServer* pCharServer = pclCM->GetCharServer( myid)	;
				if(pCharServer != NULL)
				{
					// 스턴공격을 한다
					SI32 siValue =  pCharServer->SoulGuardAbilityActivity( SOULGUARD_ABILITY_ATTACK_STUN );
					if ( 0 < siValue )
					{
						cltCharServer* pCharServer_Enemy = pclCM->GetCharServer( attackid )	;
						if(NULL != pCharServer_Enemy)
						{
							pCharServer_Enemy->SetStun(siValue );
						}
					}
				}
			}
			if( IsPC( myid ) )
			{
				// 공격 할때 일정 확률로 마법을 쏜다 얼음정령
				cltCharServer* pCharServer = pclCM->GetCharServer( myid)	;
				if(pCharServer != NULL)
				{
					// 얼린다.
					SI32 siValue =  pCharServer->SoulGuardAbilityActivity( SOULGUARD_ABILITY_ATTACK_SOULICE );
					if ( 0 < siValue )
					{
						cltCharServer* pCharServer_Enemy = pclCM->GetCharServer( attackid )	;
						if(NULL != pCharServer_Enemy)
						{
							pCharServer_Enemy->SetSoulGuardIce(siValue );	// 초를 넣는다 
						}
					}
				}
			}
		}
	}

	
	if( IsPC(myid) && 0 < pcldamageinfo->siMagicKind )
	{
		AddMagicAction( myid, attackid, pcldamageinfo->siMagicKind, cldamageinfo.siDamage );
		SubMagicAction( myid, attackid, pcldamageinfo->siMagicKind, cldamageinfo.siDamage );
	}

	return TRUE;
}

SI32 cltServer::GetEnchantDefen(SI32 AttackType, SI32 defenid)
{
	SI32 EnchantDef = 0;
	if(IsPC(defenid))
	{
		cltItem *pclItemList = pclCM->CR[defenid]->pclCI->clCharItem.GetEquipSet(EQUIPSET_PERSON);
		EnchantDef = pclCM->CR[defenid]->clPB.GetEnchantDefDamage(AttackType, pclItemList);  		
	}

	return EnchantDef;
}


// 공격속성, 공격력, 공격대상을 고려하여 실제 피해를 계산한다. 
bool cltServer::GetDamage(cltAttackDamageInfo* pcldamageinfo, SI32 myid, SI32 attackid, UI08 specialattackswitch, cltAttackDamageInfo* pclreturndamageinfo)
{
	// 공격 목표가 이미 유효하지 않으면 계산 할 수 없다.
	if( pclCM->IsAlive( attackid ) == FALSE )
		return false;

	// 중독되어 있는 상태에서의 타이머 타격일 때에는 데미지를 계산하지 않고 공격력을 그대로 데미지로 인정한다. 
	if( pcldamageinfo->siAttackType == ATTACKTYPE_POISONSTATUS )
	{
		pclreturndamageinfo->Set( pcldamageinfo );
		return true;
	}
	// 불이 붙어 있는 상태에서의 타이머 타격일 때에는 데미지를 계산하지 않고 공격력을 그대로 데미지로 인정한다. 
	if( pcldamageinfo->siAttackType == ATTACKTYPE_MAGICFIRESTATUS )
	{
		pclreturndamageinfo->Set( pcldamageinfo );
		return true;
	}
	//-----------------------------
	// 방어력에 의한 감소치 계산. 
	//-----------------------------
	SI32 validpower = 0;

	if(pclClient->IsCountrySwitch(Switch_HealthAddChange))
	{
		// 피격자의 방어력. 
		SI32 magickind = pcldamageinfo->siMagicKind;
		if(magickind < MAGIC_INDEX_START || magickind  > MAGIC_INDEX_LAST)	magickind = 0;

		SI32 defensiveRate = 0; 

		if(magickind>0)
		{
			defensiveRate= pclCM->CR[attackid]->clPB.GetDefensiveRate( pclCM->CR[myid],true);
		}
		else
		{
			defensiveRate= pclCM->CR[attackid]->clPB.GetDefensiveRate( pclCM->CR[myid]);
			// 얼음정령 물리 방어율을 높여 준다 
			cltCharServer* pclCharServer = pclCM->GetCharServer( attackid )	;
			if(pclCharServer != NULL )
			{
				SI32 siValue =  pclCharServer->SoulGuardAbilityActivity( SOULGUARD_ABILITY_ATTACK_DEFENSE );
				if ( 0 < siValue )
				{
					defensiveRate	=	defensiveRate	+ ( siValue * 100);
				}
			}
		}

		if(defensiveRate >0)
		{

			validpower = (pcldamageinfo->siDamage) - (((pcldamageinfo->siDamage) * defensiveRate)/10000);
		}
		else
		{
			validpower = pcldamageinfo->siDamage;
		}

	}
	else
	{
		// 피격자의 방어력. 
		SI32 ac = pclCM->CR[attackid]->clPB.GetTotalAC();

		// 공격력 감소분. 
		SI32 decpower;
		if((pcldamageinfo->siDamage + ac) == 0)
		{
			decpower = 0;
		}
		else
		{
			//cyj 방어력에 의한 데미지경감효과 2배로 수정(2005.12.20)
			//cyj 방어력에 의한 데미지경감효과 1.3배로 수정(2005.12.26)
			//decpower = (pcldamageinfo->siDamage * ac)/((pcldamageinfo->siDamage*2) + ac*8/10) ;
			//decpower = ( (pcldamageinfo->siDamage * ac)/((pcldamageinfo->siDamage*2) + ac*8/10) ) * 2;
			decpower = ( (pcldamageinfo->siDamage * ac)/((pcldamageinfo->siDamage*2) + ac*8/10) ) * 1.3;
		}

		validpower	= pcldamageinfo->siDamage - decpower;
	}

	if( validpower <= 0 )	validpower = 1;

	//------------------------------
	// 피격자의 데미지 감소치가 있다면, 
	//-------------------------------
	if(pclCM->CR[attackid]->clPB.siEquipAssistDamageReduceRate)
	{
		validpower = max(1, validpower - (validpower * pclCM->CR[attackid]->clPB.siEquipAssistDamageReduceRate / 1000) );
	}

	//-------------------------------------------------
	// 공격력에 따른 인챈트 계산하기    by tae-yang
	//-------------------------------------------------

	SI32 TotalEnchantPowerA		= 0;
	SI32 A_enchantTypeDef		= 0;
	SI32 A_enchantPowerDef		= 0;
	SI32 My_EnchantPowerAttack	= 0;
	SI32 My_enchantTypeAttack	= 0;

	SI32 monsterkind	  =  pclCM->CR[attackid]->GetKind();
	REAL32 monster_fire   = 0;
	REAL32 monster_water  = 0;
	REAL32 monster_wind   = 0;
	REAL32 monster_land   = 0;	
	REAL32 monster_nature = 0;

	if( monsterkind > 0)
	{
		if(pclCM->CR[attackid]->clIdentity.siIdentity == IDENTITY_GMCREATE_MONSTER)
		{
			monster_fire   = pclKindInfoSet->pclGMCommandKindInfo[monsterkind]->clCharElemental.GetFire();
			monster_water  = pclKindInfoSet->pclGMCommandKindInfo[monsterkind]->clCharElemental.GetWater();
			monster_wind   = pclKindInfoSet->pclGMCommandKindInfo[monsterkind]->clCharElemental.GetWind();
			monster_land   = pclKindInfoSet->pclGMCommandKindInfo[monsterkind]->clCharElemental.GetLand();	
			monster_nature = pclKindInfoSet->pclGMCommandKindInfo[monsterkind]->clCharElemental.GetNature();
		}
		else
		{
			monster_fire   = pclKindInfoSet->pclKI[monsterkind]->pclCharElemental.GetFire();
			monster_water  = pclKindInfoSet->pclKI[monsterkind]->pclCharElemental.GetWater();
			monster_wind   = pclKindInfoSet->pclKI[monsterkind]->pclCharElemental.GetWind();
			monster_land   = pclKindInfoSet->pclKI[monsterkind]->pclCharElemental.GetLand();	
			monster_nature = pclKindInfoSet->pclKI[monsterkind]->pclCharElemental.GetNature();
		}
	}

	SI32 bflag = false;

	//validpower = validpower - TotalEnchantPowerA;

	if(IsPC(myid) || (pclCM->CR[myid]->clIdentity.siIdentity == IDENTITY_PERSON_SUMMON))
	{
		//SI32 equiploc = PERSONITEM_SUMMON1EQUIP0;
		SI32		enchanttype		= 0;
		SI32		enchantgrade	= 0;

		cltItem		*pclItemList	= pclCM->CR[myid]->pclCI->clCharItem.GetEquipSet(EQUIPSET_PERSON);

		if(pclCM->CR[myid]->clIdentity.siIdentity == IDENTITY_PERSON_SUMMON)
		{
			bflag =true;

			/*
			//equipset = EQUIPSET_SUMMON1;   //0 일때 
			if(pclCM->CR[myid]->GetSummonIndex() == 1)
			{
				//equipset = EQUIPSET_SUMMON2;    // 1일때 
				equiploc = PERSONITEM_SUMMON2EQUIP0;
			}


			myid = pclCM->CR[myid]->clIdentity.clSummonInfo.siParentCharUnique;
			enchanttype = pclCM->CR[myid]->clPB.pclCI->clCharItem.clItem[equiploc + 2].Element.siType; 
			enchantgrade =  pclCM->CR[myid]->clPB.pclCI->clCharItem.clItem[equiploc + 2].Element.siPower;
			*/
				SI32 siSummonIndex = pclCM->CR[myid]->GetSummonIndex();

			myid = pclCM->CR[myid]->clIdentity.clSummonInfo.siParentCharUnique;
			if ( false == pclCM->IsValidID( myid ) )
			{
				return false;
			}

			if ( pclCM->CR[myid]->clPB.pclCI->clSummonInfo.GetSummonEquipItem(siSummonIndex, 2) )
			{
				cltItem clSummonWeapon;
				clSummonWeapon.Set( pclCM->CR[myid]->clPB.pclCI->clSummonInfo.GetSummonEquipItem(siSummonIndex, 2) );

				enchanttype		= clSummonWeapon.Element.siType;
				enchantgrade	= clSummonWeapon.Element.siPower;
			}
			else
			{
				enchanttype		= 0;
				enchantgrade	= 0;
			}

			pclItemList	= pclCM->CR[myid]->clPB.pclCI->clSummonInfo.GetSummonEquipItemList( siSummonIndex );
		}

		if ( NULL == pclItemList )
		{
			return false;
		}

		if(bflag == false) // 공격자가 소환수가 아닐경우
		{
			enchanttype  = pclCM->CR[myid]->clPB.pclCI->clCharItem.clItem[pclCM->CR[myid]->ActiveWeapon].Element.siType;
			enchantgrade = pclCM->CR[myid]->clPB.pclCI->clCharItem.clItem[pclCM->CR[myid]->ActiveWeapon].Element.siPower;
		}

		SI32 enchantdamage = pclItemManager->GetEnchantDamageFromGrade(enchantgrade);

		if(pclClient->IsCountrySwitch(Switch_HealthAddChange))
		{
			cltCharServer *Mypclchar = (cltCharServer*)pclCM->CR[myid];
			SI16 Myactiveweaponpos = pclCM->CR[myid]->ActiveWeapon;

			//cltCharServer *Monpclchar = (cltCharServer*)pclCM->CR[attackid];
			//SI16 Monactiveweaponpos = pclCM->CR[attackid]->ActiveWeapon;
			//cltItem *MonpclItemList = pclCM->CR[attackid]->pclCI->clCharItem.GetEquipSet(equipset);

			SI16 addDamagePer = 0;

			switch(enchanttype)				
			{
			case ENCHANT_TYPE_FIRE_A:
				{
					if(IsPC(Mypclchar->GetCharUnique()))
					{						
						addDamagePer = pclItemManager->GetEquipCrystalFireAttack(pclItemList,Myactiveweaponpos,Mypclchar); // 내가 가진 공격력을 구한다.
						enchantdamage = enchantdamage + (SI32)(enchantdamage *addDamagePer /100);
					}
					enchantdamage =enchantdamage - (SI32)(enchantdamage * monster_fire /100 );

					if ( pclClient->IsCountrySwitch(Switch_Set_Item_Add) )
					{
						if( Mypclchar->m_siSetItemType )
						{
							enchantdamage += pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemAbilityValue(Mypclchar, SETITEM_ABILITY_TYPE_FIRE_ENCHANT_ATTACK, USE_SETITEM_1);
							enchantdamage += pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemAbilityValue(Mypclchar, SETITEM_ABILITY_TYPE_FIRE_ENCHANT_ATTACK, USE_SETITEM_2);
							enchantdamage += pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemAbilityValue(Mypclchar, SETITEM_ABILITY_TYPE_FIRE_ENCHANT_ATTACK, USE_SETITEM_3);
							enchantdamage += pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemAbilityValue(Mypclchar, SETITEM_ABILITY_TYPE_FIRE_ENCHANT_ATTACK, USE_SETITEM_4);
							enchantdamage += pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemAbilityValue(Mypclchar, SETITEM_ABILITY_TYPE_FIRE_ENCHANT_ATTACK, USE_SETITEM_5);
						}
					}

					// PCK : 수호정령 - 불씨정령이 소환되어 있으면 불속성 데미지가 1.5배 증가됨 (09.07.08)
					// SOULGUARD_ABILITY_ELEMENTDAMAGE_UP와는 별개로 적용된다. (기본 능력임)
					if(pclClient->IsCountrySwitch(Switch_SoulGuard))
					{
						if(Mypclchar->IsSoulGuardStatusOut() == TRUE && Mypclchar->pclSoulGuard->GetKind() == pclClient->GetUniqueFromHash(TEXT("KIND_SOULGUARD_FIRESPIRIT")))
						{
							enchantdamage = enchantdamage * 150 / 100;
						}
					}
				}
				break;
			case ENCHANT_TYPE_WATER_A:
				{
					if(IsPC(Mypclchar->GetCharUnique()))
					{						
						addDamagePer = pclItemManager->GetEquipCrystalWaterAttack(pclItemList,Myactiveweaponpos,Mypclchar); // 내가 가진 공격력을 구한다.
						enchantdamage = enchantdamage + (SI32)(enchantdamage *addDamagePer /100);
					}
					enchantdamage =enchantdamage- (SI32)(enchantdamage *monster_water / 100);

					if ( pclClient->IsCountrySwitch(Switch_Set_Item_Add) )
					{
						if( Mypclchar->m_siSetItemType )
						{
							enchantdamage += pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemAbilityValue(Mypclchar, SETITEM_ABILITY_TYPE_WATER_ENCHANT_ATTACK, USE_SETITEM_1);
							enchantdamage += pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemAbilityValue(Mypclchar, SETITEM_ABILITY_TYPE_WATER_ENCHANT_ATTACK, USE_SETITEM_2);
							enchantdamage += pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemAbilityValue(Mypclchar, SETITEM_ABILITY_TYPE_WATER_ENCHANT_ATTACK, USE_SETITEM_3);
							enchantdamage += pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemAbilityValue(Mypclchar, SETITEM_ABILITY_TYPE_WATER_ENCHANT_ATTACK, USE_SETITEM_4);
							enchantdamage += pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemAbilityValue(Mypclchar, SETITEM_ABILITY_TYPE_WATER_ENCHANT_ATTACK, USE_SETITEM_5);
						}
					}
					// 얼음정령 - 물속성 공격력 1.5배 증가 (기본능력)
					if(pclClient->IsCountrySwitch(Switch_SoulGuard))
					{
						if(Mypclchar->IsSoulGuardStatusOut() == TRUE && Mypclchar->pclSoulGuard->GetKind() == pclClient->GetUniqueFromHash(TEXT("KIND_SOULGUARD_ICESPIRIT")))
						{
							enchantdamage = enchantdamage * 150 / 100;
						}
					}
				}
				break;
			case ENCHANT_TYPE_WIND_A:
				{
					if(IsPC(Mypclchar->GetCharUnique()))
					{						
						addDamagePer = pclItemManager->GetEquipCrystalWindAttack(pclItemList,Myactiveweaponpos,Mypclchar); // 내가 가진 공격력을 구한다.
						enchantdamage = enchantdamage + (SI32)(enchantdamage *addDamagePer /100);
					}
					enchantdamage =enchantdamage- (SI32)(enchantdamage *monster_wind / 100 );

					if ( pclClient->IsCountrySwitch(Switch_Set_Item_Add) )
					{
						if( Mypclchar->m_siSetItemType )
						{
							enchantdamage += pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemAbilityValue(Mypclchar, SETITEM_ABILITY_TYPE_WIND_ENCHANT_ATTACK, USE_SETITEM_1);
							enchantdamage += pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemAbilityValue(Mypclchar, SETITEM_ABILITY_TYPE_WIND_ENCHANT_ATTACK, USE_SETITEM_2);
							enchantdamage += pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemAbilityValue(Mypclchar, SETITEM_ABILITY_TYPE_WIND_ENCHANT_ATTACK, USE_SETITEM_3);
							enchantdamage += pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemAbilityValue(Mypclchar, SETITEM_ABILITY_TYPE_WIND_ENCHANT_ATTACK, USE_SETITEM_4);
							enchantdamage += pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemAbilityValue(Mypclchar, SETITEM_ABILITY_TYPE_WIND_ENCHANT_ATTACK, USE_SETITEM_5);
						}
					}
				}				
				break;
			case ENCHANT_TYPE_GROUND_A:
				{
					if(IsPC(Mypclchar->GetCharUnique()))
					{						
						addDamagePer = pclItemManager->GetEquipCrystalEarthAttack(pclItemList,Myactiveweaponpos,Mypclchar); // 내가 가진 공격력을 구한다.
						enchantdamage = enchantdamage + (SI32)(enchantdamage *addDamagePer /100);
					}
					enchantdamage =enchantdamage- (SI32)(enchantdamage *monster_wind / 100 );
				}
				enchantdamage =enchantdamage- (SI32)(enchantdamage *monster_land / 100);


				if ( pclClient->IsCountrySwitch(Switch_Set_Item_Add) )
				{
					if( Mypclchar->m_siSetItemType )
					{
						enchantdamage += pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemAbilityValue(Mypclchar, SETITEM_ABILITY_TYPE_LAND_ENCHANT_ATTACK, USE_SETITEM_1);
						enchantdamage += pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemAbilityValue(Mypclchar, SETITEM_ABILITY_TYPE_LAND_ENCHANT_ATTACK, USE_SETITEM_2);
						enchantdamage += pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemAbilityValue(Mypclchar, SETITEM_ABILITY_TYPE_LAND_ENCHANT_ATTACK, USE_SETITEM_3);
						enchantdamage += pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemAbilityValue(Mypclchar, SETITEM_ABILITY_TYPE_LAND_ENCHANT_ATTACK, USE_SETITEM_4);
						enchantdamage += pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemAbilityValue(Mypclchar, SETITEM_ABILITY_TYPE_LAND_ENCHANT_ATTACK, USE_SETITEM_5);
					}
				}


				break;
			default:
				break;
			}
		}
		else
		{
			switch(enchanttype)
			{
			case ENCHANT_TYPE_FIRE_A:
				enchantdamage =enchantdamage - (SI32)(enchantdamage * monster_fire /100 );
				break;
			case ENCHANT_TYPE_WATER_A:
				enchantdamage =enchantdamage- (SI32)(enchantdamage *monster_water / 100);
				break;
			case ENCHANT_TYPE_WIND_A:
				enchantdamage =enchantdamage- (SI32)(enchantdamage *monster_wind / 100 );
				break;
			case ENCHANT_TYPE_GROUND_A:
				enchantdamage =enchantdamage- (SI32)(enchantdamage *monster_land / 100);
				break;
			default:
				break;

			}
		}

		if(enchantdamage<0)
			enchantdamage = 0;

		if ( IsCountrySwitch(Switch_SoulGuard) )
		{
			SI32 siSoulGuardAddEnchantDamage = ((cltCharServer*)pclCM->CR[myid])->SoulGuardAbilityActivity( SOULGUARD_ABILITY_ELEMENTDAMAGE_UP );
			if ( 0 < siSoulGuardAddEnchantDamage )
			{
				enchantdamage += siSoulGuardAddEnchantDamage;
			}
		}

		//물리저항 계산 
		if(monster_nature)
		{
			validpower  =  validpower - (SI32)(monster_nature * 0.01 * (REAL32)validpower);
			//	validpower = (SI32)temp;
		}

		validpower =  validpower + enchantdamage;
	}

	else if(IsNPC(myid))
	{
		monsterkind =  pclCM->CR[myid]->GetKind();
		SI32 monster_fireA = 0;
		SI32 monster_waterA =0;
		SI32 monster_windA = 0;
		SI32 monster_landA = 0;	

		if(monsterkind > 0)
		{
			monster_fireA = pclKindInfoSet->pclKI[monsterkind]->pclCharElemental.GetFireA();
			monster_waterA = pclKindInfoSet->pclKI[monsterkind]->pclCharElemental.GetWaterA();
			monster_windA = pclKindInfoSet->pclKI[monsterkind]->pclCharElemental.GetWindA();
			monster_landA = pclKindInfoSet->pclKI[monsterkind]->pclCharElemental.GetLandA();	

		}

		//몬스터면은 ... 어떻게 될까? 
		validpower = validpower + monster_fireA + monster_waterA + monster_windA + monster_landA;
		if(monster_nature)
		{
			validpower= validpower + (SI32)(monster_nature * 0.01 * (REAL32)validpower);
		}

		//	REAL32 temp = 0;
		if ( pclClient->IsCountrySwitch(Switch_Set_Item_Add) )
		{
			if( pclCM->CR[attackid]->m_siSetItemType )
			{
				validpower -= validpower *  (   pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemAbilityValue(pclCM->CR[attackid], SETITEM_ABILITY_TYPE_NATURE, USE_SETITEM_1)
											  + pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemAbilityValue(pclCM->CR[attackid], SETITEM_ABILITY_TYPE_NATURE, USE_SETITEM_2)
											  + pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemAbilityValue(pclCM->CR[attackid], SETITEM_ABILITY_TYPE_NATURE, USE_SETITEM_3)
											  + pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemAbilityValue(pclCM->CR[attackid], SETITEM_ABILITY_TYPE_NATURE, USE_SETITEM_4)
											  + pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemAbilityValue(pclCM->CR[attackid], SETITEM_ABILITY_TYPE_NATURE, USE_SETITEM_5)) / 100;
			}
		}
	}

	//--------------------------------------------------
	// 방어력에 따른 인챈트 계산하기     by tae-yang
	//--------------------------------------------------

	monsterkind =  pclCM->CR[myid]->GetKind();

	SI32 monster_fireA = 0; 
	SI32 monster_waterA =0; 
	SI32 monster_windA = 0; 
	SI32 monster_landA = 0; 


	if(monsterkind > 0 )
	{
		monster_fireA  = pclKindInfoSet->pclKI[monsterkind]->pclCharElemental.GetFireA();
		monster_waterA = pclKindInfoSet->pclKI[monsterkind]->pclCharElemental.GetWaterA();
		monster_windA  = pclKindInfoSet->pclKI[monsterkind]->pclCharElemental.GetWindA();
		monster_landA  = pclKindInfoSet->pclKI[monsterkind]->pclCharElemental.GetLandA();	
	}


	SI32 A_enchantType = 0;
	SI32 enchantPercent = 0;
	SI32 enchantac = 0;

	for(SI32 i =0; i < 4; i++)
	{
		A_enchantType = i*2 + 2;		

		if(IsPC(attackid))
		{
			cltItem *pclItemList = pclCM->CR[attackid]->pclCI->clCharItem.GetEquipSet(EQUIPSET_PERSON);
			enchantPercent = pclCM->CR[attackid]->clPB.GetEnchantDefDamage(A_enchantType, pclItemList); 

			if(pclClient->IsCountrySwitch(Switch_HealthAddChange))
			{
				SI16 addPer = 0;

				cltCharServer *Mypclchar = (cltCharServer*)pclCM->CR[attackid];
				SI16 Myactiveweaponpos = pclCM->CR[attackid]->ActiveWeapon;

				switch(A_enchantType)
				{
				case ENCHANT_TYPE_FIRE_D:
					{
						enchantac = 0;

						if ( pclClient->IsCountrySwitch(Switch_Set_Item_Add) )
						{
							SI32 siValue = 0;
							if( Mypclchar->m_siSetItemType )
							{

								cltSetItemAbilityManager* pcltSetItemAbilityManager = pclClient->pclItemManager->pcltSetItemAbilityManager;
								if( pcltSetItemAbilityManager ) 
								{
									siValue += pcltSetItemAbilityManager->GetSetItemAbilityValue(Mypclchar, SETITEM_ABILITY_TYPE_FIRE_ENCHANT_DEFENSIVE, USE_SETITEM_1 );
									siValue += pcltSetItemAbilityManager->GetSetItemAbilityValue(Mypclchar, SETITEM_ABILITY_TYPE_FIRE_ENCHANT_DEFENSIVE, USE_SETITEM_2 );
									siValue += pcltSetItemAbilityManager->GetSetItemAbilityValue(Mypclchar, SETITEM_ABILITY_TYPE_FIRE_ENCHANT_DEFENSIVE, USE_SETITEM_3 );
									siValue += pcltSetItemAbilityManager->GetSetItemAbilityValue(Mypclchar, SETITEM_ABILITY_TYPE_FIRE_ENCHANT_DEFENSIVE, USE_SETITEM_4 );
									siValue += pcltSetItemAbilityManager->GetSetItemAbilityValue(Mypclchar, SETITEM_ABILITY_TYPE_FIRE_ENCHANT_DEFENSIVE, USE_SETITEM_5 );
								}
							}

							enchantac = ( enchantPercent * monster_fireA  / 100 );
							enchantac += (enchantac * (siValue / 100));
						}
						else
						{
							if( pclCM->CR[attackid]->m_siSetItemType == VAMPIRE_SETITEM )
							{

								enchantac = ( enchantPercent * monster_fireA / 100 ) + ( 40 * monster_fireA / 100 );
							}
							else
							{
								if(enchantPercent)
								{
									enchantac = enchantPercent * monster_fireA / 100; 
								}
							}
						}

						if(enchantac < 0)
							enchantac = 0;
						else
						{						
							addPer = pclItemManager->GetEquipCrystalFireDeffence(pclItemList,Myactiveweaponpos,Mypclchar); // 내가 가진 공격력을 구한다.
							enchantac = enchantac + (SI32)(enchantac * addPer /100);
						}

						validpower = validpower - enchantac;
					}
					break;

				case ENCHANT_TYPE_WATER_D:
					{
						enchantac = 0;
						if ( pclClient->IsCountrySwitch(Switch_Set_Item_Add) )
						{
							SI32 siValue = 0;
							if( Mypclchar->m_siSetItemType ) 
							{
								cltSetItemAbilityManager* pcltSetItemAbilityManager = pclClient->pclItemManager->pcltSetItemAbilityManager;
								if( pcltSetItemAbilityManager ) 
								{
									siValue += pcltSetItemAbilityManager->GetSetItemAbilityValue(Mypclchar, SETITEM_ABILITY_TYPE_WATER_ENCHANT_DEFENSIVE, USE_SETITEM_1 );
									siValue += pcltSetItemAbilityManager->GetSetItemAbilityValue(Mypclchar, SETITEM_ABILITY_TYPE_WATER_ENCHANT_DEFENSIVE, USE_SETITEM_2 );
									siValue += pcltSetItemAbilityManager->GetSetItemAbilityValue(Mypclchar, SETITEM_ABILITY_TYPE_WATER_ENCHANT_DEFENSIVE, USE_SETITEM_3 );
									siValue += pcltSetItemAbilityManager->GetSetItemAbilityValue(Mypclchar, SETITEM_ABILITY_TYPE_WATER_ENCHANT_DEFENSIVE, USE_SETITEM_4 );
									siValue += pcltSetItemAbilityManager->GetSetItemAbilityValue(Mypclchar, SETITEM_ABILITY_TYPE_WATER_ENCHANT_DEFENSIVE, USE_SETITEM_5 );
								}	
							}

							enchantac  = ( enchantPercent * monster_waterA  / 100 );
							enchantac += (enchantac * (siValue / 100));
						}
						else
						{
							if( pclCM->CR[attackid]->m_siSetItemType == VAMPIRE_SETITEM )
							{
								enchantac = ( enchantPercent * monster_waterA / 100 ) + ( 40 * monster_waterA / 100 );
							}
							else
							{
								if(enchantPercent)
								{
									enchantac = enchantPercent * monster_waterA / 100; 
								}
							}
						}

						if(enchantac < 0)
							enchantac= 0;
						else
						{						
							addPer = pclItemManager->GetEquipCrystalWaterDeffence(pclItemList,Myactiveweaponpos,Mypclchar); // 내가 가진 공격력을 구한다.
							enchantac = enchantac + (SI32)(enchantac *addPer /100);
						}

						validpower = validpower - enchantac;

					}
					break;

				case ENCHANT_TYPE_WIND_D:
					{
						enchantac = 0;
						if ( pclClient->IsCountrySwitch(Switch_Set_Item_Add) )
						{
							SI32 siValue = 0;
							if( Mypclchar->m_siSetItemType )
							{
								cltSetItemAbilityManager* pcltSetItemAbilityManager = pclClient->pclItemManager->pcltSetItemAbilityManager;
								if( pcltSetItemAbilityManager ) 
								{
									siValue += pcltSetItemAbilityManager->GetSetItemAbilityValue(Mypclchar, SETITEM_ABILITY_TYPE_WIND_ENCHANT_DEFENSIVE, USE_SETITEM_1 );
									siValue += pcltSetItemAbilityManager->GetSetItemAbilityValue(Mypclchar, SETITEM_ABILITY_TYPE_WIND_ENCHANT_DEFENSIVE, USE_SETITEM_2 );
									siValue += pcltSetItemAbilityManager->GetSetItemAbilityValue(Mypclchar, SETITEM_ABILITY_TYPE_WIND_ENCHANT_DEFENSIVE, USE_SETITEM_3 );
									siValue += pcltSetItemAbilityManager->GetSetItemAbilityValue(Mypclchar, SETITEM_ABILITY_TYPE_WIND_ENCHANT_DEFENSIVE, USE_SETITEM_4 );
									siValue += pcltSetItemAbilityManager->GetSetItemAbilityValue(Mypclchar, SETITEM_ABILITY_TYPE_WIND_ENCHANT_DEFENSIVE, USE_SETITEM_5 );
								}
							}

							enchantac  = ( enchantPercent * monster_windA  / 100 );
							enchantac += (enchantac * (siValue / 100));

						}
						else
						{

							if( pclCM->CR[attackid]->m_siSetItemType == VAMPIRE_SETITEM )
							{
								enchantac = ( enchantPercent * monster_windA / 100 ) + ( 40 * monster_windA / 100 );
							}
							else
							{
								if(enchantPercent)
								{
									enchantac = enchantPercent * monster_windA / 100; 
								}
							}
						}

						if(enchantac < 0)
							enchantac= 0;
						else
						{						
							addPer = pclItemManager->GetEquipCrystalWindDeffence(pclItemList,Myactiveweaponpos,Mypclchar); // 내가 가진 공격력을 구한다.
							enchantac = enchantac + (SI32)(enchantac *addPer /100);
						}

						validpower = validpower - enchantac;
					}
					break;

				case ENCHANT_TYPE_GROUND_D:
					{
						enchantac = 0;
						if ( pclClient->IsCountrySwitch(Switch_Set_Item_Add) )
						{
							SI32 siValue = 0;
							if( Mypclchar->m_siSetItemType )
							{
								cltSetItemAbilityManager* pcltSetItemAbilityManager = pclClient->pclItemManager->pcltSetItemAbilityManager;
								if( pcltSetItemAbilityManager ) 
								{
									siValue += pcltSetItemAbilityManager->GetSetItemAbilityValue(Mypclchar, SETITEM_ABILITY_TYPE_LAND_ENCHANT_DEFENSIVE, USE_SETITEM_1 );
									siValue += pcltSetItemAbilityManager->GetSetItemAbilityValue(Mypclchar, SETITEM_ABILITY_TYPE_LAND_ENCHANT_DEFENSIVE, USE_SETITEM_2 );
									siValue += pcltSetItemAbilityManager->GetSetItemAbilityValue(Mypclchar, SETITEM_ABILITY_TYPE_LAND_ENCHANT_DEFENSIVE, USE_SETITEM_3 );
									siValue += pcltSetItemAbilityManager->GetSetItemAbilityValue(Mypclchar, SETITEM_ABILITY_TYPE_LAND_ENCHANT_DEFENSIVE, USE_SETITEM_4 );
									siValue += pcltSetItemAbilityManager->GetSetItemAbilityValue(Mypclchar, SETITEM_ABILITY_TYPE_LAND_ENCHANT_DEFENSIVE, USE_SETITEM_5 );
								}
							}

							enchantac  = ( enchantPercent * monster_landA  / 100 );
							enchantac += (enchantac * (siValue / 100));
						}
						else
						{
							if( pclCM->CR[attackid]->m_siSetItemType == VAMPIRE_SETITEM )
							{
								enchantac = ( enchantPercent * monster_landA / 100 ) + ( 40 * monster_landA / 100 );
							}
							else
							{
								if(enchantPercent)
								{
									enchantac = enchantPercent * monster_landA / 100; 
								}
							}
						}

						if(enchantac < 0)
							enchantac= 0;
						else
						{						
							addPer = pclItemManager->GetEquipCrystalEarthDeffence(pclItemList,Myactiveweaponpos,Mypclchar); // 내가 가진 공격력을 구한다.
							enchantac = enchantac + (SI32)(enchantac *addPer /100);
						}

						validpower = validpower - enchantac;
					}
					break;

				default:
					break;

				}		
			}
			else	// if(pclClient->IsCountrySwitch(Switch_HealthAddChange))
			{
				switch(A_enchantType)
				{
				case ENCHANT_TYPE_FIRE_D:
					{
						enchantac = 0;
						if ( pclClient->IsCountrySwitch(Switch_Set_Item_Add) )
						{
							if( pclCM->CR[attackid]->m_siSetItemType ) 
							{
								SI32 siValue = 0;
								cltSetItemAbilityManager* pcltSetItemAbilityManager = pclClient->pclItemManager->pcltSetItemAbilityManager;
								if( pcltSetItemAbilityManager ) 
								{
									siValue += pcltSetItemAbilityManager->GetSetItemAbilityValue(pclCM->CR[attackid], SETITEM_ABILITY_TYPE_FIRE_ENCHANT_DEFENSIVE, USE_SETITEM_1 );
									siValue += pcltSetItemAbilityManager->GetSetItemAbilityValue(pclCM->CR[attackid], SETITEM_ABILITY_TYPE_FIRE_ENCHANT_DEFENSIVE, USE_SETITEM_2 );
									siValue += pcltSetItemAbilityManager->GetSetItemAbilityValue(pclCM->CR[attackid], SETITEM_ABILITY_TYPE_FIRE_ENCHANT_DEFENSIVE, USE_SETITEM_3 );
									siValue += pcltSetItemAbilityManager->GetSetItemAbilityValue(pclCM->CR[attackid], SETITEM_ABILITY_TYPE_FIRE_ENCHANT_DEFENSIVE, USE_SETITEM_4 );
									siValue += pcltSetItemAbilityManager->GetSetItemAbilityValue(pclCM->CR[attackid], SETITEM_ABILITY_TYPE_FIRE_ENCHANT_DEFENSIVE, USE_SETITEM_5 );
								}

								enchantac = ( enchantPercent * monster_fireA / 100 );
								enchantac += (enchantac * (siValue / 100));
							}

						}
						else
						{
							if( pclCM->CR[attackid]->m_siSetItemType == VAMPIRE_SETITEM )
							{
								enchantac = ( enchantPercent * monster_fireA / 100 ) + ( 40 * monster_fireA / 100 );
							}
							else
							{
								if(enchantPercent)
								{
									enchantac = enchantPercent * monster_fireA / 100; 
								}
							}
						}

						if(enchantac < 0)
							enchantac= 0;
						validpower = validpower - enchantac;
					}
					break;
				case ENCHANT_TYPE_WATER_D:
					{
						enchantac = 0;
						if ( pclClient->IsCountrySwitch(Switch_Set_Item_Add) )
						{
							if( pclCM->CR[attackid]->m_siSetItemType ) 
							{
								SI32 siValue = 0;
								cltSetItemAbilityManager* pcltSetItemAbilityManager = pclClient->pclItemManager->pcltSetItemAbilityManager;
								if( pcltSetItemAbilityManager ) 
								{
									siValue += pcltSetItemAbilityManager->GetSetItemAbilityValue(pclCM->CR[attackid], SETITEM_ABILITY_TYPE_WATER_ENCHANT_DEFENSIVE, USE_SETITEM_1 );
									siValue += pcltSetItemAbilityManager->GetSetItemAbilityValue(pclCM->CR[attackid], SETITEM_ABILITY_TYPE_WATER_ENCHANT_DEFENSIVE, USE_SETITEM_2 );
									siValue += pcltSetItemAbilityManager->GetSetItemAbilityValue(pclCM->CR[attackid], SETITEM_ABILITY_TYPE_WATER_ENCHANT_DEFENSIVE, USE_SETITEM_3 );
									siValue += pcltSetItemAbilityManager->GetSetItemAbilityValue(pclCM->CR[attackid], SETITEM_ABILITY_TYPE_WATER_ENCHANT_DEFENSIVE, USE_SETITEM_4 );
									siValue += pcltSetItemAbilityManager->GetSetItemAbilityValue(pclCM->CR[attackid], SETITEM_ABILITY_TYPE_WATER_ENCHANT_DEFENSIVE, USE_SETITEM_5 );
								}

								enchantac  = ( enchantPercent * monster_waterA / 100 );
								enchantac += (enchantac * (siValue / 100));
							}
						}
						else
						{
							if( pclCM->CR[attackid]->m_siSetItemType == VAMPIRE_SETITEM )
							{
								enchantac = ( enchantPercent * monster_waterA / 100 ) + ( 40 * monster_waterA / 100 );
							}
							else
							{
								if(enchantPercent)
								{
									enchantac = enchantPercent * monster_waterA / 100; 
								}
							}
						}

						if(enchantac < 0)
							enchantac= 0;
						validpower = validpower - enchantac;

					}
					break;
				case ENCHANT_TYPE_WIND_D:
					{
						enchantac = 0;
						if ( pclClient->IsCountrySwitch(Switch_Set_Item_Add) )
						{
							if( pclCM->CR[attackid]->m_siSetItemType ) 
							{
								SI32 siValue = 0;
								cltSetItemAbilityManager* pcltSetItemAbilityManager = pclClient->pclItemManager->pcltSetItemAbilityManager;
								if( pcltSetItemAbilityManager ) 
								{
									siValue += pcltSetItemAbilityManager->GetSetItemAbilityValue(pclCM->CR[attackid], SETITEM_ABILITY_TYPE_WIND_ENCHANT_DEFENSIVE, USE_SETITEM_1 );
									siValue += pcltSetItemAbilityManager->GetSetItemAbilityValue(pclCM->CR[attackid], SETITEM_ABILITY_TYPE_WIND_ENCHANT_DEFENSIVE, USE_SETITEM_2 );
									siValue += pcltSetItemAbilityManager->GetSetItemAbilityValue(pclCM->CR[attackid], SETITEM_ABILITY_TYPE_WIND_ENCHANT_DEFENSIVE, USE_SETITEM_3 );
									siValue += pcltSetItemAbilityManager->GetSetItemAbilityValue(pclCM->CR[attackid], SETITEM_ABILITY_TYPE_WIND_ENCHANT_DEFENSIVE, USE_SETITEM_4 );
									siValue += pcltSetItemAbilityManager->GetSetItemAbilityValue(pclCM->CR[attackid], SETITEM_ABILITY_TYPE_WIND_ENCHANT_DEFENSIVE, USE_SETITEM_5 );
								}

								enchantac  = ( enchantPercent * monster_windA / 100 );
								enchantac += (enchantac * (siValue / 100));
							}
						}
						else
						{
							if( pclCM->CR[attackid]->m_siSetItemType == VAMPIRE_SETITEM )
							{
								enchantac = ( enchantPercent * monster_windA / 100 ) + ( 40 * monster_windA / 100 );
							}
							else
							{
								if(enchantPercent)
								{
									enchantac = enchantPercent * monster_windA / 100; 
								}
							}
						}

						if(enchantac < 0)
							enchantac= 0;
						validpower = validpower - enchantac;
					}
					break;
				case ENCHANT_TYPE_GROUND_D:
					{
						enchantac = 0;
						if ( pclClient->IsCountrySwitch(Switch_Set_Item_Add) )
						{
							if( pclCM->CR[attackid]->m_siSetItemType ) 
							{
								SI32 siValue = 0;
								cltSetItemAbilityManager* pcltSetItemAbilityManager = pclClient->pclItemManager->pcltSetItemAbilityManager;
								if( pcltSetItemAbilityManager ) 
								{
									siValue += pcltSetItemAbilityManager->GetSetItemAbilityValue(pclCM->CR[attackid], SETITEM_ABILITY_TYPE_LAND_ENCHANT_DEFENSIVE, USE_SETITEM_1 );
									siValue += pcltSetItemAbilityManager->GetSetItemAbilityValue(pclCM->CR[attackid], SETITEM_ABILITY_TYPE_LAND_ENCHANT_DEFENSIVE, USE_SETITEM_2 );
									siValue += pcltSetItemAbilityManager->GetSetItemAbilityValue(pclCM->CR[attackid], SETITEM_ABILITY_TYPE_LAND_ENCHANT_DEFENSIVE, USE_SETITEM_3 );
									siValue += pcltSetItemAbilityManager->GetSetItemAbilityValue(pclCM->CR[attackid], SETITEM_ABILITY_TYPE_LAND_ENCHANT_DEFENSIVE, USE_SETITEM_4 );
									siValue += pcltSetItemAbilityManager->GetSetItemAbilityValue(pclCM->CR[attackid], SETITEM_ABILITY_TYPE_LAND_ENCHANT_DEFENSIVE, USE_SETITEM_5 );
								}

								enchantac  = ( enchantPercent * monster_landA / 100 );
								enchantac += (enchantac * (siValue / 100));
							}
						}
						else
						{
							if( pclCM->CR[attackid]->m_siSetItemType == VAMPIRE_SETITEM )
							{
								enchantac = ( enchantPercent * monster_landA / 100 ) + ( 40 * monster_landA / 100 );
							}
							else
							{
								if(enchantPercent)
								{
									enchantac = enchantPercent * monster_landA / 100; 
								}
							}
						}

						if(enchantac < 0)
							enchantac= 0;
						validpower = validpower - enchantac;
					}
					break;
				default:
					break;

				}	// switch(A_enchantType)		
			}
			// ac = ac + enchantac;
		}
		// [영훈] 소환수 장비 속성 방어력 추가
		else if(pclCM->CR[attackid]->clIdentity.siIdentity == IDENTITY_PERSON_SUMMON)
		{
			cltCharServer* pclSummon = pclCM->GetCharServer( attackid );
			if ( NULL == pclSummon )
			{
				return false;
			}

			SI32 siSummonIndex	= pclSummon->GetSummonIndex();
			SI32 siParentCharID	= pclSummon->GetSummonParentID();

			cltCharServer* pclParent = pclCM->GetCharServer( siParentCharID );
			if ( NULL == pclParent )
			{
				return false;
			}

			// 루프를 돌기때문에 우선 초기화
			enchantPercent	= 0;
			enchantac		= 0;

			// 소환수의 장비를 얻어온다
			cltItem* pclItemList = pclParent->clPB.pclCI->clSummonInfo.GetSummonEquipItemList( siSummonIndex );
			if ( pclItemList )
			{
				enchantPercent = pclSummon->clPB.GetEnchantDefDamage( A_enchantType, pclItemList );
			}

			// 방어구에 의한 속성방어력 %있어야 계산한다
			if ( 0 < enchantPercent )
			{
				switch ( A_enchantType )
				{
				case ENCHANT_TYPE_FIRE_D:	{	enchantac = enchantPercent * monster_fireA/100;		}	break;
				case ENCHANT_TYPE_WATER_D:	{	enchantac = enchantPercent * monster_waterA/100;	}	break;
				case ENCHANT_TYPE_WIND_D:	{	enchantac = enchantPercent * monster_windA/100;		}	break;
				case ENCHANT_TYPE_GROUND_D:	{	enchantac = enchantPercent * monster_landA/100;		}	break;
				}		
			}

			if(enchantac < 0)
			{
				enchantac = 0;
			}
			// 실질적으로 입을 데미지에서 빼준다
			validpower = validpower - enchantac;
		}
		else if(IsNPC(attackid))
		{

		}
	}


	// 최대 데미지에 한계가 있는 캐릭터라면, (왜구전쟁이벤트용 캐릭터라면,, ) 
	if((pclCM->CR[attackid]->pclKI != NULL && pclCM->CR[attackid]->pclKI->IsAtb( ATB_DAMAGELIMIT )))
	{
		validpower = min(300, validpower);
	}

	if(specialattackswitch == 1)	// 크리티컬이면
	{
		//KHY - 1220 - 변신시스템.
		cltCharServer *pclChar = (cltCharServer *)pclCM->CR[myid];

		/*		if(pclChar->clTransFormInfo.bTransFormMode == true) //변신중이고.
		{
		//해당 변신 캐릭터 싱크가 100%이면.
		if(pclChar->clTransFormInfo.GetSynchroTransForm(pclChar->clTransFormInfo.uiNowTransFormKind) == 100)
		{
		SI32 attcktype =  pclChar->GetAttackType();

		switch(attcktype)
		{
		case ATTACKTYPE_AXE :
		validpower *= 2.2; //220%
		break;
		default:
		validpower *= 2;
		break;
		}

		}		
		}
		else
		*/
		{
			validpower *= 2;
		}
	}

	//--------------------------------------
	// 전투향상술에 의한 데미지 2배(이것은 스페셜데미지에 2배가 될수도 있음, 그렇다면 총 4배!!)
	//---------------------------------------
	if ( IsPC(myid) )
	{
		SI32 siSkillUnique = 0;

		switch ( pcldamageinfo->siAttackType )
		{
		case ATTACKTYPE_SWORD:
			siSkillUnique = SKILL_SWORD_SPEAR_DOUBLEAT;
			break;
		case ATTACKTYPE_SPEAR:
			siSkillUnique = SKILL_SPEAR_DOUBLEAT;
			break;
		case ATTACKTYPE_AXE:
			siSkillUnique = SKILL_AXE_SPEAR_DOUBLEAT;
			break;
		case ATTACKTYPE_STAFF:
		case ATTACKTYPE_STAFF1:
		case ATTACKTYPE_STAFF2:
		case ATTACKTYPE_NEW_STAFF2:
			siSkillUnique = SKILL_STAFF_SPEAR_DOUBLEAT;
			break;
		case ATTACKTYPE_BOW:
		case ATTACKTYPE_BOW1:
		case ATTACKTYPE_NEW_BOW1:
			siSkillUnique = SKILL_BOW_SPEAR_DOUBLEAT;
			break;
		case ATTACKTYPE_GUN:
		case ATTACKTYPE_GUN1:
		case ATTACKTYPE_NEW_GUN1:
			siSkillUnique = SKILL_GUN_SPEAR_DOUBLEAT;
			break;
		}

		if ( (0 < siSkillUnique) && (MAX_SKILL_NUMBER > siSkillUnique) )
		{
			SI32 siSkillLevel		= pclCM->CR[myid]->pclCI->clCharSkill.GetTotalSkillLevel( siSkillUnique, 0, 0 );
			SI32 siWeaponSkillRate	= pclClient->pclSkillManager->GetWeaponSkillRate( siSkillUnique, siSkillLevel );
			if ( 0 < siWeaponSkillRate )
			{
				SI32 siRandValue = (rand() % 100) + 1;

				pclLog->LogPrint( TEXT("[WeaponSkill-DoubleAT] Rate:%d/RandValue:%d\n"), siWeaponSkillRate, siRandValue );

				if ( siWeaponSkillRate >= siRandValue )
				{
					validpower *= 2;
				}
			}
		}
	}

	//--------------------------------------
	// 떄리는 놈이 Curse Attack 마법에 걸려 있으면 공격력을 감소시킨다.
	//--------------------------------------
	if(pclCM->CR[myid]->clInnerStatus.IsStatus(INNERSTATUS_CURSEATTACK))
	{
		validpower = max(1, validpower - (validpower * pclCM->CR[myid]->siCurseAttackPower/ 100) );
	}

	//--------------------------------------
	// 맞는 놈이 Curse Defense 마법에 걸려 있으면 공격력을 증가시킨다.
	//--------------------------------------
	if(pclCM->CR[attackid]->clInnerStatus.IsStatus(INNERSTATUS_CURSEDEFENSE))
	{
		validpower = max(1, validpower + (validpower * pclCM->CR[attackid]->siCurseDefensePower/ 100) );
	}

	if ( IsPC(myid) )
	{
		//--------------------------------------
		// 마법 버프에 의한 데미지 증가
		//--------------------------------------
		SI32 siAddDamage = pclCM->CR[myid]->m_clMagicBufMgr.GetIncDamage( pclCM->CR[myid] );
		if ( 0 < siAddDamage )
		{
			validpower += siAddDamage;
		}

		//--------------------------------------
		// 수호정령 특수능력 발동에 의한 데미지 증가
		//--------------------------------------
		if ( IsCountrySwitch(Switch_SoulGuard) )
		{
			SI32 siSoulGuardAddDamage = ((cltCharServer*)pclCM->CR[myid])->SoulGuardAbilityActivity( SOULGUARD_ABILITY_DAMAGE_UP );
			if ( 0 < siSoulGuardAddDamage )
			{
				validpower += siSoulGuardAddDamage;
			}
		}
	}

	//--------------------------------------
	// 수호정령 특수능력 발동에 의한 몬스터에게 입는 데미지 감소
	//--------------------------------------
	if ( IsCountrySwitch(Switch_SoulGuard) )
	{
		if ( IsNPC(myid) && IsPC(attackid) )
		{
			SI32 siSoulGuardSubDamage = ((cltCharServer*)pclCM->CR[attackid])->SoulGuardAbilityActivity( SOULGUARD_ABILITY_DAMAGE_DOWN );
			if ( 0 < siSoulGuardSubDamage )
			{
				validpower -= siSoulGuardSubDamage;
			}
		}
	}

	//--------------------------------------
	// XBOX EVENT	Damage = 1
	//--------------------------------------
	// 맞는 놈의 속성에 Event XBox 속성이 있으면 Damage는 무조건 1
	if(pclCM->CR[attackid]->IsAtb(ATB_EVENTXBOX))
	{
		validpower = 1;
	}
	// 구리구리면 무조건 10식 닳게 하자
	else if ( pclCM->CR[attackid]->GetKind() == pclClient->GetUniqueFromHash(TEXT("KIND_JOKER")) )
	{
		validpower = 10;
	}
	else if ( pclCM->CR[attackid]->GetKind() == pclClient->GetUniqueFromHash(TEXT("KIND_SANTARACCOON_ZIQ")) )
	{
		validpower = 1;
	}
	//[추가 : 보스 와일드 보어 데미지 1씩 감소하게..., 황진성 2007.9.18]
	else if ( pclCM->CR[attackid]->GetKind() == pclClient->GetUniqueFromHash(TEXT("KIND_WILDBOARBOSS")) )
	{
		// 한국 추석 이벤트에서만.
		if(pclClient->IsCountrySwitch(Switch_BossWildBoar))
		{
			_SYSTEMTIME sTime;
			GetLocalTime(&sTime);

			// 15시와 22시.
			if((15 == sTime.wHour || 22 == sTime.wHour))	
			{
				// 22일 부터 26일까지.
				if(sTime.wDay >= 13 && sTime.wDay <= 15)
				{
					// 9월.
					if(9 == sTime.wMonth)
					{
						validpower = 1;
					}
				}
			}
		}
	}
	//[추가 : 추수 감사절 이벤트. 칠면조 데미지 1씩 감소하게..., 황진성 2007.9.18]
	else if ( pclCM->CR[attackid]->GetKind() == pclClient->GetUniqueFromHash(TEXT("KIND_TURKEY")) )
	{
		// USA, GLOBAL 에서만.
		if(pclClient->IsCountrySwitch(Switch_Thanksgiving_day))
		{
			_SYSTEMTIME sTime;
			GetLocalTime(&sTime);

			switch(pclClient->siServiceArea)
			{
			case ConstServiceArea_English:
				{
					// 2시, 8시, 14시, 20시
					if((2 == sTime.wHour || 8 == sTime.wHour || 14 == sTime.wHour || 20 == sTime.wHour))	
					{
						// 20일 부터 30일까지. 또는 1일 부터 4일까지
						if( sTime.wDay >= 20 && sTime.wDay <= 30 )
						{
							// 11월, 12월
							if( 11 == sTime.wMonth )
							{
								validpower = 1;
							}
						}
						if( sTime.wDay >= 1 && sTime.wDay <= 4 )
						{
							if( 12 == sTime.wMonth )
							{
								// 마지막날 20시는 서버점검 후 이벤트가 끝나기 때문에 생략.
								if(12 == sTime.wMonth && 4 == sTime.wDay && 20 == sTime.wHour)
									break;

								validpower = 1;
							}
						}
					}
				}
				break;
			case ConstServiceArea_USA:
				{
					// 2시, 8시, 14시, 20시
					if( 2 == sTime.wHour || 8 == sTime.wHour || 14 == sTime.wHour || 20 == sTime.wHour )
					{
						// 21일 부터 30일까지. 또는 1일 부터 5일까지
						if( sTime.wDay >= 21 && sTime.wDay <= 30 )
						{
							// 11월, 12월
							if( 11 == sTime.wMonth )
							{
								validpower = 1;
							}
						}

						if(	sTime.wDay >= 1 && sTime.wDay <= 5 )
						{
							if(	12 == sTime.wMonth )
							{
								validpower = 1;
							}
						}
					}
				}
				break;
			case ConstServiceArea_NHNChina:
				{
					// 2시, 8시, 14시, 20시
					if((2 == sTime.wHour || 8 == sTime.wHour || 14 == sTime.wHour || 20 == sTime.wHour))	
					{
						// 20일 부터 30일까지. 또는 1일 부터 4일까지
						if( sTime.wDay >= 20 && sTime.wDay <= 30 )
						{
							// 11월, 12월
							if( 11 == sTime.wMonth )
							{
								validpower = 1;
							}
						}
						if( sTime.wDay >= 1 && sTime.wDay <= 4 )
						{
							if( 12 == sTime.wMonth )
							{
								// 마지막날 20시는 서버점검 후 이벤트가 끝나기 때문에 생략.
								if(12 == sTime.wMonth && 4 == sTime.wDay && 20 == sTime.wHour)
									break;

								validpower = 1;
							}
						}
					}
				}
				break;

			case ConstServiceArea_EUROPE:
				{
					// 2시, 8시, 14시, 20시
					if((2 == sTime.wHour || 8 == sTime.wHour || 14 == sTime.wHour || 20 == sTime.wHour))	
					{
						// 20일 부터 30일까지. 또는 1일 부터 4일까지
						if( sTime.wDay >= 20 && sTime.wDay <= 30 )
						{
							// 11월, 12월
							if( 11 == sTime.wMonth )
							{
								validpower = 1;
							}
						}
						if( sTime.wDay >= 1 && sTime.wDay <= 4 )
						{
							if( 12 == sTime.wMonth )
							{
								// 마지막날 20시는 서버점검 후 이벤트가 끝나기 때문에 생략.
								if(12 == sTime.wMonth && 4 == sTime.wDay && 20 == sTime.wHour)
									break;

								validpower = 1;
							}
						}
					}
				}
				break;
			}
		}
	}
	// [설날 이벤트 : 2008.01.10 김영훈 추가 - 카이트라쿤의 데미지는 무조건 1]
	else if ( pclCM->CR[attackid]->GetKind() == pclClient->GetUniqueFromHash(TEXT("KIND_KITERACCOON")) )
	{
		if( (pclClient->pclEventTimeManager->InEventTime( TEXT("NewYear_RaccoonTime"), &sTime) == true) )
		{
			validpower = 1;
		}
	}
	// [진성] 추석 이벤트. 달, 우주토끼 데미지 감소 1씩. => 2008-8-7
	else if (    pclCM->CR[attackid]->GetKind() == pclClient->GetUniqueFromHash(TEXT("KIND_MOON"))
		|| pclCM->CR[attackid]->GetKind() == pclClient->GetUniqueFromHash(TEXT("KIND_COSMOSRABBIT2")))
	{
		validpower = 1;
	}
	// [영훈] 그린 코이노보리는 무조건 1씩
	else if ( pclCM->CR[attackid]->GetKind() == pclClient->GetUniqueFromHash(TEXT("KIND_KOINOBORYGREEN")) )
	{
		validpower = 1;
	}
	// [성웅] 운영자 몬스터는 무조건 1씩
	else if ( pclCM->CR[attackid]->GetKind() == pclClient->GetUniqueFromHash(TEXT("KIND_BERSERKER")) )
	{
		validpower = 1;
	}


	//--------------------------------------
	// 모후모후 이벤트 : 국가 이벤트
	//--------------------------------------
	// 모후모후는 향수효과가 없으면 데미지가 10%만 허용된다.
	if( IsPC( myid ) && pclCM->CR[attackid]->clIdentity.siIdentity == IDENTITY_MOFUMOFUEVENT )
	{
		// 향수버프가 없으면 데미지는 10%가 된다.
		cltBufInfo * pclMyBuf = &pclCM->CR[ myid ]->pclCI->clBI.clBufInfo;

		BOOL bMofuMofuPerfume = FALSE;

		if( pclMyBuf->IsHaveBuf( BUF_MOFUMOFU_PERFUME ) )		
			bMofuMofuPerfume = TRUE;
		if( pclCM->CR[myid]->pclCI->clCharItem.clItem[PERSONITEM_HAT].siUnique == ITEMUNIQUE( 23090 ) )
			bMofuMofuPerfume = TRUE;

		if( bMofuMofuPerfume == FALSE )
		{
			validpower = max( 1, validpower - (validpower * 90 / 100) );
		}
	}

	//--------------------------------------
	// 퀘스트용 안티포션
	//--------------------------------------
	if ( (pclCM->CR[myid]->GetKind() == pclClient->GetUniqueFromHash(TEXT("KIND_GREATWALLASURA"))) && IsPC(attackid) )
	{
		// 안티포션 버프가 걸려있다면 아수라에게 받는 데미지가 30% 깎인다
		cltBufInfo * pclAttackBuf = &pclCM->CR[ attackid ]->pclCI->clBI.clBufInfo;
		if ( (pclAttackBuf) && (pclAttackBuf->IsHaveBuf(BUF_QUEST_ANTIPOTION)) )
		{
			validpower = max( 1, validpower - (validpower * 30 / 100) );
		}
	}

	//[2007.12.13 추가] 인첸트에 의한 데미지 -(마이너스)나오는 버그 수정
	validpower = max(1,validpower);
	pclreturndamageinfo->Set(pcldamageinfo);
	pclreturndamageinfo->siDamage = validpower;
	return true;
}


// 적에게 실제로 데미지를 입힌다.
// specialattackswitch : 필살기로 작동 여부.( 0 : 일반 공격, 1 : 크리티컬, 2 : 추가타 (수박버프) )
BOOL cltServer::Hit(SI32 myid, SI32 enemyid, cltAttackDamageInfo* pcldamageinfo, UI08 specialattackswitch)
{
	//------------------------------------			
	// 공격에 따른 타격치를 구한다. 
	//------------------------------------
	cltAttackDamageInfo cldamageinfo;

	SI32 realdamage = 0;
	SI32 displaydamage = 0;
	SI32 doubledamage = 0;

	if( GetDamage( pcldamageinfo, myid, enemyid, specialattackswitch, &cldamageinfo ) == true )
	{
		//---------------------------------------
		// 피격자의 체력을 줄여준다.(데미지가 0이 나올수는 없다. 단 아래 SpecialDamage함수 내에서 특정 모드에 따라 0이 나올수는 있음) [영훈]
		//---------------------------------------
		realdamage    = max( 1, cldamageinfo.siDamage );
		displaydamage = max( 1, cldamageinfo.siDamage );

		// PCK : 추가타 데미지는 기본데미지의 80%~95% 수준으로 정한다.
		if(specialattackswitch == 2)
		{
			SI32	siRandValue = (rand() % 16) + 79;
			doubledamage = max( 1, cldamageinfo.siDamage * siRandValue / 100 );
		}			
	}

	cltCharServer* pclchartemp = (cltCharServer*)pclCM->CR[enemyid];

	//---------------------------------------
	// 특별한 데미지 값 설정
	//---------------------------------------
	SpecialDamage(myid, enemyid, &realdamage, &displaydamage);

	//[진성] 세트 효과에 따른 공격력 상승.
	if ( pclClient->IsCountrySwitch(Switch_Set_Item_Add) )
	{
		if ( pclCM->CR[myid] && IsPC( pclCM->CR[myid]->GetID() ) )
		{
			if( pclCM->CR[myid]->m_siSetItemType ) 
			{
				if( pcldamageinfo->siAttackType == ATTACKTYPE_MAGIC )
				{
					realdamage	  += realdamage    *  (	pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemAbilityValue( pclCM->CR[myid], SETITEM_ABILITY_TYPE_MAGIC_ATTACK, USE_SETITEM_1 ) 
													  + pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemAbilityValue( pclCM->CR[myid], SETITEM_ABILITY_TYPE_MAGIC_ATTACK, USE_SETITEM_2 )
													  + pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemAbilityValue( pclCM->CR[myid], SETITEM_ABILITY_TYPE_MAGIC_ATTACK, USE_SETITEM_3 )
													  + pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemAbilityValue( pclCM->CR[myid], SETITEM_ABILITY_TYPE_MAGIC_ATTACK, USE_SETITEM_4 )
													  + pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemAbilityValue( pclCM->CR[myid], SETITEM_ABILITY_TYPE_MAGIC_ATTACK, USE_SETITEM_5 )) / 100;

					displaydamage += displaydamage *  (	pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemAbilityValue( pclCM->CR[myid], SETITEM_ABILITY_TYPE_MAGIC_ATTACK, USE_SETITEM_1 )
													  + pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemAbilityValue( pclCM->CR[myid], SETITEM_ABILITY_TYPE_MAGIC_ATTACK, USE_SETITEM_2 )
													  + pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemAbilityValue( pclCM->CR[myid], SETITEM_ABILITY_TYPE_MAGIC_ATTACK, USE_SETITEM_3 )
													  + pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemAbilityValue( pclCM->CR[myid], SETITEM_ABILITY_TYPE_MAGIC_ATTACK, USE_SETITEM_4 )
													  + pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemAbilityValue( pclCM->CR[myid], SETITEM_ABILITY_TYPE_MAGIC_ATTACK, USE_SETITEM_5 )) / 100;

					doubledamage  += doubledamage  *  ( pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemAbilityValue( pclCM->CR[myid], SETITEM_ABILITY_TYPE_MAGIC_ATTACK, USE_SETITEM_1 ) 
													  + pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemAbilityValue( pclCM->CR[myid], SETITEM_ABILITY_TYPE_MAGIC_ATTACK, USE_SETITEM_2 )
													  + pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemAbilityValue( pclCM->CR[myid], SETITEM_ABILITY_TYPE_MAGIC_ATTACK, USE_SETITEM_3 )
													  + pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemAbilityValue( pclCM->CR[myid], SETITEM_ABILITY_TYPE_MAGIC_ATTACK, USE_SETITEM_4 )
													  + pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemAbilityValue( pclCM->CR[myid], SETITEM_ABILITY_TYPE_MAGIC_ATTACK, USE_SETITEM_5 )) / 100;
				}
				else
				{
					realdamage	  += realdamage    *  ( pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemAbilityValue( pclCM->CR[myid], SETITEM_ABILITY_TYPE_ATTACK, USE_SETITEM_1 ) 
													  + pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemAbilityValue( pclCM->CR[myid], SETITEM_ABILITY_TYPE_ATTACK, USE_SETITEM_2 )
													  + pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemAbilityValue( pclCM->CR[myid], SETITEM_ABILITY_TYPE_ATTACK, USE_SETITEM_3 )
													  + pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemAbilityValue( pclCM->CR[myid], SETITEM_ABILITY_TYPE_ATTACK, USE_SETITEM_4 )
													  + pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemAbilityValue( pclCM->CR[myid], SETITEM_ABILITY_TYPE_ATTACK, USE_SETITEM_5 )) / 100;

					displaydamage += displaydamage *  ( pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemAbilityValue( pclCM->CR[myid], SETITEM_ABILITY_TYPE_ATTACK, USE_SETITEM_1 )
												      + pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemAbilityValue( pclCM->CR[myid], SETITEM_ABILITY_TYPE_ATTACK, USE_SETITEM_2 )
													  + pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemAbilityValue( pclCM->CR[myid], SETITEM_ABILITY_TYPE_ATTACK, USE_SETITEM_3 )
													  + pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemAbilityValue( pclCM->CR[myid], SETITEM_ABILITY_TYPE_ATTACK, USE_SETITEM_4 )
													  + pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemAbilityValue( pclCM->CR[myid], SETITEM_ABILITY_TYPE_ATTACK, USE_SETITEM_5 )) / 100;

					doubledamage  += doubledamage  *  ( pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemAbilityValue( pclCM->CR[myid], SETITEM_ABILITY_TYPE_ATTACK, USE_SETITEM_1 )
													  + pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemAbilityValue( pclCM->CR[myid], SETITEM_ABILITY_TYPE_ATTACK, USE_SETITEM_2 )
													  + pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemAbilityValue( pclCM->CR[myid], SETITEM_ABILITY_TYPE_ATTACK, USE_SETITEM_3 )
													  + pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemAbilityValue( pclCM->CR[myid], SETITEM_ABILITY_TYPE_ATTACK, USE_SETITEM_4 )
													  + pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemAbilityValue( pclCM->CR[myid], SETITEM_ABILITY_TYPE_ATTACK, USE_SETITEM_5 )) / 100;

					// 일정 확률로 상대를 얼리기.
					if( pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemAbilityValue(pclCM->CR[myid], SETITEM_ABILITY_TYPE_ICE_MAGIC_TARGET_ATTACK, USE_SETITEM_1 ) || 
						pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemAbilityValue(pclCM->CR[myid], SETITEM_ABILITY_TYPE_ICE_MAGIC_TARGET_ATTACK, USE_SETITEM_2 )	||
						pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemAbilityValue(pclCM->CR[myid], SETITEM_ABILITY_TYPE_ICE_MAGIC_TARGET_ATTACK, USE_SETITEM_3 ) ||
						pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemAbilityValue(pclCM->CR[myid], SETITEM_ABILITY_TYPE_ICE_MAGIC_TARGET_ATTACK, USE_SETITEM_4 ) ||
						pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemAbilityValue(pclCM->CR[myid], SETITEM_ABILITY_TYPE_ICE_MAGIC_TARGET_ATTACK, USE_SETITEM_5 ))
					{
						cltCharServer* pcltempchar = (cltCharServer*)pclCM->CR[enemyid];
						if(pcltempchar)
						{
							pcltempchar->SetIce(0);
						}	
					}
				}
				
				// [지연] 일정 확률로 상대를 중독시키기.
				if( pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemAbilityValue(pclCM->CR[myid], SETITEM_ABILITY_TYPE_POISON_MAGIC_TARGET_ATTACK, USE_SETITEM_1 ) || 
					pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemAbilityValue(pclCM->CR[myid], SETITEM_ABILITY_TYPE_POISON_MAGIC_TARGET_ATTACK, USE_SETITEM_2 ) ||
					pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemAbilityValue(pclCM->CR[myid], SETITEM_ABILITY_TYPE_POISON_MAGIC_TARGET_ATTACK, USE_SETITEM_3 ) ||
					pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemAbilityValue(pclCM->CR[myid], SETITEM_ABILITY_TYPE_POISON_MAGIC_TARGET_ATTACK, USE_SETITEM_4 ) ||
					pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemAbilityValue(pclCM->CR[myid], SETITEM_ABILITY_TYPE_POISON_MAGIC_TARGET_ATTACK, USE_SETITEM_5 ))
				{
					cltCharServer* pcltempchar = (cltCharServer*)pclCM->CR[enemyid];
					if (pcltempchar)
					{
						if(		false == pcltempchar->clInnerStatus.IsStatus(INNERSTATUS_POISON) 
							&&	false == pcltempchar->clInnerStatus.IsStatus(INNERSTATUS_POISON_SETITEM_EFFECT) )
						{
							if(	DEVIL_MAN == pclCM->CR[myid]->m_siSetItemType  || DEVIL_WOMAN == pclCM->CR[myid]->m_siSetItemType  ||
								DEVIL_MAN == pclCM->CR[myid]->m_siSetItemType2 || DEVIL_WOMAN == pclCM->CR[myid]->m_siSetItemType2 ||
								DEVIL_MAN == pclCM->CR[myid]->m_siSetItemType3 || DEVIL_WOMAN == pclCM->CR[myid]->m_siSetItemType3 ||
								DEVIL_MAN == pclCM->CR[myid]->m_siSetItemType4 || DEVIL_WOMAN == pclCM->CR[myid]->m_siSetItemType4 ||
								DEVIL_MAN == pclCM->CR[myid]->m_siSetItemType5 || DEVIL_WOMAN == pclCM->CR[myid]->m_siSetItemType5	)
							{
								pcltempchar->SetPoisonSetItemEffect(myid, realdamage);
							}
							else
							{
								pcltempchar->SetPoison(myid, realdamage);
							}
						}
					}	
				}
			}
		}
	}

	// 할로윈 셋트 사용시 마법 데미지 10% 증가
	else if (  pclCM->CR[myid]->m_siSetItemType == HALLOWEEN1_SETITEM
			|| pclCM->CR[myid]->m_siSetItemType == HALLOWEEN2_SETITEM 
			|| pclCM->CR[myid]->m_siSetItemType == HALLOWEEN3_SETITEM
			|| pclCM->CR[myid]->m_siSetItemType == HALLOWEEN4_SETITEM ) 
	{
		if( pcldamageinfo->siAttackType == ATTACKTYPE_MAGIC || 
			pcldamageinfo->siAttackType == ATTACKTYPE_POISON )
		{
			realdamage		= (realdamage * 110 )/100	; // 마법 데미지 10% 증가 
			displaydamage	= (displaydamage * 110)/100	; // 마법 데미지 10% 증가 
			doubledamage	= (doubledamage * 110)/100	; // 마법 데미지 10% 증가
		}
	}
	//----------------------------------------------------------------------------------
	if(IsCountrySwitch(Switch_SoulGuard))
	{
		// 해당 데미지가 마법 데미지 일때
		if(pcldamageinfo->siMagicKind > 0 )
		{
			// 공격을 했을 경우
			cltCharServer* pclMyChar = pclCM->GetCharServer(myid);
			if ( NULL != pclMyChar )
			{
				// 수호정령 펌킨일때 데미지 증가
				if ( (pclMyChar->IsSoulGuardStatusOut()) && (pclMyChar->pclSoulGuard->GetKind()==pclClient->GetUniqueFromHash(TEXT("KIND_SOULGUARD_PUMPKIN"))) )
				{
					realdamage		= (realdamage * 110 )/100	; // 마법 데미지 10% 증가 
					displaydamage	= (displaydamage * 110)/100	; // 마법 데미지 10% 증가 
					doubledamage	= (doubledamage * 110)/100	; // 마법 데미지 10% 증가
				}
			}

			// 공격을 받앗을 경우
			cltCharServer* pclCharServer = pclCM->GetCharServer( enemyid)	;
			if ( pclCharServer != NULL )
			{
				SI32 sivalue = pclCharServer->SoulGuardAbilityActivity(SOULGUARD_ABILITY_MAGIC_DEFENSE )	; // 펌킨 마법이 
				if(sivalue > 0 )
				{
					realdamage		=	realdamage - (realdamage * sivalue / 100 )	;
					displaydamage	=	displaydamage - (displaydamage * sivalue / 100 )	;
					doubledamage	=	doubledamage - (doubledamage * sivalue / 100 )	;
				}
			}
		}
	}
	
	// [기형] 배틀로얄 모드중 파워 업 모드 일 경우
	cltCharServer* pclChar = pclCM->GetCharServer(myid);
	if( NULL != pclChar )
	{
		if(pclChar->bBattleRoyalPowerupMode == true)
		{
			realdamage = realdamage * 2;
			displaydamage = displaydamage * 2;
		}
	}

	//----------------------------------------------------------------------------------
	if(specialattackswitch == 2)
	{
		SI32 life = pclchartemp->DecreaseLife(realdamage + doubledamage);
	}
	else
	{
		SI32 life = pclchartemp->DecreaseLife(realdamage);
	}

	//------------------------------------------------
	// 타겟이 피격되면 처리해야할 개개 캐릭터의 함수. 
	//-----------------------------------------------
	pclchartemp->BeHitOperate(myid);


	if( pclCM->IsAlive( enemyid ) == false )
	{
		// 마지막으로 사냥한 시간을 업데이트 한다. 
		if(pclCM->CR[myid]->clIdentity.siIdentity == IDENTITY_PERSON_SUMMON)
		{
			SI16 parentid = pclCM->GetIDFromCharUnique(pclCM->CR[myid]->clIdentity.clSummonInfo.siParentCharUnique);
			if(parentid)
			{
				((cltCharServer*)pclCM->CR[parentid])->SetLastKillClock(CurrentClock);				// 캐릭터를 죽인 가장 최근 시간을 업데이트 한다. 
			}
		}
		else
		{
			((cltCharServer*)pclCM->CR[myid])->SetLastKillClock(CurrentClock);				// 캐릭터를 죽인 가장 최근 시간을 업데이트 한다. 
		}

		// 대박아이템 얻었는지 스위치
		bool bBigItemGet = false;

		if( IsPC(pclCM->CR[enemyid]->GetCharUnique()) )
		{
			DeadChar_PC(myid, enemyid);
		}
		else 
		{
			DeadChar_NPC(myid, enemyid);

			// 이벤트 대박 아이템
			bBigItemGet = GiveBigItem_Event(myid, enemyid);

			// 이벤트 대박 아이템을 받지 못한경우에만 대박아이템을 계산한다.
			if( bBigItemGet == false )
			{
				if(pclClient->IsCountrySwitch(Switch_BigItemModify))
				{
					bBigItemGet = GiveBigItem_New(myid, enemyid);
				}
				else
				{
					bBigItemGet = GiveBigItem(myid, enemyid);
				}
			}

			GiveSpecialItem(myid, enemyid);
		}

		//-----------------------------
		// 죽었다면 전리품을 챙긴다. 
		//-----------------------------

		// 대박아이템 나왔을때는 일반 아이템 안먹도록
		if (bBigItemGet == false)
		{
			//파티획득막음

			// 만약 소환수라면 부모의 아이디를 구해서 파티중인지 확인한다.
			SI32 parentID = pclCM->CR[myid]->GetSummonParentID();
			if( pclCM->IsValidID(parentID) == TRUE )
			{
				if( pclCM->CR[parentID]->siPartyIndex > 0 )	// 파티에 가입중이라면 파티 아이템 분배 함수 호출
				{			
					CPartyObj* pclparty = pclPartyMgr->GetPartyObj(pclCM->CR[parentID]->siPartyIndex);
					// 파티가 있고 아이템 분배 방식이 솔로 플레이 와 다른 방식이라면
					if(pclparty != NULL && pclparty->GetItemGetPattern() != ITEM_GETPATTERN_NORMAL )
					{
						PartyWarItemOperate(parentID, enemyid, pclparty);				
					}
					else
						WarItemOperate(myid, enemyid);
				}
				else
					WarItemOperate(myid, enemyid);

			}
			else // 플레이어라면 
			{
				if( pclCM->CR[myid]->siPartyIndex > 0 )	// 파티에 가입중이라면 파티 아이템 분배 함수 호출
				{			
					CPartyObj* pclparty = pclPartyMgr->GetPartyObj(pclCM->CR[myid]->siPartyIndex);
					// 파티가 있고 아이템 분배 방식이 솔로 플레이 와 다른 방식이라면
					if(pclparty != NULL && pclparty->GetItemGetPattern() != ITEM_GETPATTERN_NORMAL )
					{
						PartyWarItemOperate(myid, enemyid, pclparty);				
					}
					else
						WarItemOperate(myid, enemyid);
				}
				else
					WarItemOperate(myid, enemyid);
			}

		}

		// 구리구리에게 죽었을 때만 남긴다
		if ( IsNPC(myid) && IsPC(enemyid) )
		{
			if ( pclCM->CR[myid]->GetKind() == pclClient->GetUniqueFromHash(TEXT("KIND_JOKER") ) )
			{
				pclLog->FilePrint("Config\\DeathfromJoker.txt", "User : [%s]",
					pclCM->CR[enemyid]->pclCI->clBI.GetName());
			}
		}

		//--------------------------------------------
		// 매크로
		//-----------------------------------------
		if ( IsPC(myid) )
		{
			((cltCharServer*)pclCM->CR[myid])->AddKillMonsterNum(1);
		}
		else if ( IsNPC(myid) )
		{
			SI32 siParentID = pclCM->CR[myid]->GetSummonParentID();
			if ( pclCM->IsValidID(siParentID) )
			{
				((cltCharServer*)pclCM->CR[siParentID])->AddKillMonsterNum(1);
			}
		}

		cltCharServer* pcltempchar = (cltCharServer*)pclCM->CR[myid];

		if( IsPC(pcltempchar->GetCharUnique()) )
		{
			if ( pcltempchar->GetLife() > 0 )
			{
				SI32 siDrainRate = 0;

				if ( pclClient->IsCountrySwitch(Switch_Set_Item_Add) )
				{
					if( pcltempchar->m_siSetItemType )
					{
						cltSetItemAbilityManager* pcltSetItemAbilityManager = pclClient->pclItemManager->pcltSetItemAbilityManager;
						if( pcltSetItemAbilityManager ) 
						{
							if(pcltempchar->m_siSetItemType)
							{
								siDrainRate = pcltSetItemAbilityManager->GetSetItemAbilityValue(pcltempchar, SETITEM_ABILITY_TYPE_DRAIN, USE_SETITEM_1 );
								siDrainRate += pcltSetItemAbilityManager->GetSetItemAbilityValue(pcltempchar, SETITEM_ABILITY_TYPE_DRAIN, USE_SETITEM_2 );
								siDrainRate += pcltSetItemAbilityManager->GetSetItemAbilityValue(pcltempchar, SETITEM_ABILITY_TYPE_DRAIN, USE_SETITEM_3 );
								siDrainRate += pcltSetItemAbilityManager->GetSetItemAbilityValue(pcltempchar, SETITEM_ABILITY_TYPE_DRAIN, USE_SETITEM_4 );
								siDrainRate += pcltSetItemAbilityManager->GetSetItemAbilityValue(pcltempchar, SETITEM_ABILITY_TYPE_DRAIN, USE_SETITEM_5 );
								/*
								SI32 siValue = 0;
								siValue = pcltSetItemAbilityManager->GetSetItemAbilityValue(pcltempchar, SETITEM_ABILITY_TYPE_DRAIN, USE_SETITEM_1 );

								pcltempchar->SetDrain(pcldamageinfo->siDamage, siValue, 0);
								*/
							}
						}
					}
				}
				else
				{
					// 뱀파이어 세트 착용시 20%흡혈 사람만 적용시켜주자
					if ( pcltempchar->m_siSetItemType == VAMPIRE_SETITEM )
					{
						siDrainRate = 20;
					}
					else if ( pcltempchar->m_siSetItemType == HALLOWEEN1_SETITEM 
						||pcltempchar->m_siSetItemType == HALLOWEEN2_SETITEM
						||pcltempchar->m_siSetItemType == HALLOWEEN3_SETITEM
						||pcltempchar->m_siSetItemType == HALLOWEEN4_SETITEM)	// 호박 셋트 일경우는 10% 흡혈
					{
						siDrainRate = 10;
					}
				}

				// 전투향상술에 의한 흡혈
				if ( (PERSONITEM_WEAPON1 == pcltempchar->ActiveWeapon) || (PERSONITEM_WEAPON2 == pcltempchar->ActiveWeapon) )
				{
					SI32 siSkillUnique		= 0;
					SI32 siItemType			= pclClient->pclItemManager->GetItemType( pcltempchar->pclCI->clCharItem.clItem[pcltempchar->ActiveWeapon].siUnique );
					switch ( siItemType )
					{
					case ITEMTYPE_SWORD:	{	siSkillUnique = SKILL_SWORD_BOW_DRAIN;	}	break;
					case ITEMTYPE_SPEAR:	{	siSkillUnique = SKILL_SPEAR_BOW_DRAIN;	}	break;
					case ITEMTYPE_AXE:		{	siSkillUnique = SKILL_AXE_BOW_DRAIN;	}	break;
					case ITEMTYPE_BOW:		{	siSkillUnique = SKILL_BOW_DRAIN;		}	break;
					case ITEMTYPE_GUN:		{	siSkillUnique = SKILL_GUN_BOW_DRAIN;	}	break;
					case ITEMTYPE_STAFF:	{	siSkillUnique = SKILL_STAFF_BOW_DRAIN;	}	break;
					}

					if ( (0 < siSkillUnique) && (MAX_SKILL_NUMBER > siSkillUnique) )
					{
						SI32 siSkillLevel		= pcltempchar->pclCI->clCharSkill.GetTotalSkillLevel( siSkillUnique, 0, 0 );
						SI16 siWeaponSkillRate	= pclClient->pclSkillManager->GetWeaponSkillRate( siSkillUnique, siSkillLevel );
						if ( 0 < siWeaponSkillRate )
						{
							SI32 siRandValue = (rand() % 100) + 1;

							pclLog->LogPrint( TEXT("[WeaponSkill-Drain/Death] Rate:%d/RandValue:%d\n"), siWeaponSkillRate, siRandValue );

							if ( siWeaponSkillRate >= siRandValue )
							{
								siDrainRate = 30;
							}
						}
					}
				}

				if ( 0 < siDrainRate )
				{
					pcltempchar->SetDrain( pcldamageinfo->siDamage, siDrainRate, 0 );
				}
			} 
		}
	}
	else
	{
		// 물리 공격 
		// 무기에의한 공격시 해당 무기에 부적에의해 부착된 특수능력에 의한 공격을 수행한다.

		cltAttackDamageInfo clinfo;
		clinfo.Set(&cldamageinfo);
		clinfo.siDamage = realdamage;
		AddScrollMagicOperate(myid, enemyid, &clinfo);

		// 마법 공격 
		ChangeStatusMagicOperate(myid, enemyid, &clinfo);

	}

	//=======================================================
	// 캐릭터가 죽거나 살거나 해당 마법은 수행된다.
	//=======================================================
	cltAttackDamageInfo clinfo;
	clinfo.Set(&cldamageinfo);
	if(specialattackswitch == 2)
	{
		clinfo.siDamage = displaydamage + doubledamage;
	}
	else
	{
		clinfo.siDamage = displaydamage;
	}

	PassiveMagicOperate(myid, enemyid, &clinfo);


	//=======================================================
	// 캐릭터의 상태를 설정하고 전송한다.
	//=======================================================
	// 피격 상태로 전환한다. 
	bool			bIndure = true;
	cltCharServer*	pclchar	= NULL;

	if ( IsPC(enemyid) )
	{
		pclchar = pclCM->GetCharServer(enemyid);
	}

	// 전투향상술에 의한 피격모션 유/무
	if ( (pclchar) && ( (PERSONITEM_WEAPON1 == pclchar->ActiveWeapon) || (PERSONITEM_WEAPON2 == pclchar->ActiveWeapon) ) )
	{
		SI32 siSkillUnique	= 0;
		SI32 siItemType		= pclchar->pclCI->clCharItem.GetWeaponType( pclItemManager, pclchar->ActiveWeapon );
		switch ( siItemType )
		{
		case ITEMTYPE_SWORD:	{	siSkillUnique = SKILL_SWORD_AXE_INDURE;	}	break;
		case ITEMTYPE_SPEAR:	{	siSkillUnique = SKILL_SPEAR_AXE_INDURE;	}	break;
		case ITEMTYPE_AXE:		{	siSkillUnique = SKILL_AXE_INDURE;		}	break;
		case ITEMTYPE_BOW:		{	siSkillUnique = SKILL_BOW_AXE_INDURE;	}	break;
		case ITEMTYPE_GUN:		{	siSkillUnique = SKILL_GUN_AXE_INDURE;	}	break;
		case ITEMTYPE_STAFF:	{	siSkillUnique = SKILL_STAFF_AXE_INDURE;	}	break;
		}

		bool bHideHitMotion = false;

		if ( (0 < siSkillUnique) && (MAX_SKILL_NUMBER > siSkillUnique) )
		{
			SI32 siSkillLevel		= pclchar->pclCI->clCharSkill.GetTotalSkillLevel( siSkillUnique, 0, 0 );
			SI16 siWeaponSkillRate	= pclClient->pclSkillManager->GetWeaponSkillRate( siSkillUnique, siSkillLevel );
			if ( 0 < siWeaponSkillRate )
			{
				SI32 siRandValue = (rand() % 100) + 1;

				pclLog->LogPrint( TEXT("[WeaponSkill-INDURE] Rate:%d/RandValue:%d\n"), siWeaponSkillRate, siRandValue );

				if ( siWeaponSkillRate >= siRandValue )
				{
					bHideHitMotion = true;
				}
			}
		}

		if ( bHideHitMotion )
		{
			pclchar->SetHitSwitch( false );
			bIndure = false ;
		}
		else
		{
			pclchar->SetHitSwitch( true );
		}
	}
	else
	{
		pclCM->CR[enemyid]->SetHitSwitch(true);
	}

	SI32 siHitMode = 0 ;
	SI32 siSkillLevel = 0 ;
	if ( IsPC(myid) && IsPC(enemyid) == false )
	{
		cltCharServer* pclchar = (cltCharServer*)pclCM->CR[myid];

		switch( pclchar->GetAttackType() )
		{
		case ATTACKTYPE_STAFF:
		case ATTACKTYPE_STAFF1:
		case ATTACKTYPE_STAFF2:
		case ATTACKTYPE_NEW_STAFF2:
			{
				siHitMode = 1 ;
				siSkillLevel = pclchar->pclCI->clCharSkill.GetTotalSkillLevel(SKILL_USESTAFF1,0,0,&pclchar->pclCI->clCharItem.clItem[0]) ;

				if ( siSkillLevel > 60 ) siHitMode = 6 ;
			}
			break;
		case ATTACKTYPE_SWORD:
			{
				siHitMode = 2 ;
				siSkillLevel = pclchar->pclCI->clCharSkill.GetTotalSkillLevel(SKILL_USESWORD1,0,0,&pclchar->pclCI->clCharItem.clItem[0]) ;

				if ( siSkillLevel > 60 ) siHitMode = 7 ;
			}
			break;
		case ATTACKTYPE_SPEAR:
			{
				siHitMode = 3 ;
				siSkillLevel = pclchar->pclCI->clCharSkill.GetTotalSkillLevel(SKILL_USESPEAR1,0,0,&pclchar->pclCI->clCharItem.clItem[0]) ;

				if ( siSkillLevel > 60 ) siHitMode = 8 ;
			}
			break;
		case ATTACKTYPE_AXE:
			{
				siHitMode = 4 ;
				siSkillLevel = pclchar->pclCI->clCharSkill.GetTotalSkillLevel(SKILL_USEAXE1,0,0,&pclchar->pclCI->clCharItem.clItem[0]) ;

				if ( siSkillLevel > 60 ) siHitMode = 9 ;
			}
			break;
		case ATTACKTYPE_BOW:
		case ATTACKTYPE_BOW1:
		case ATTACKTYPE_NEW_BOW1:
			{
				siHitMode = 5 ;
				siSkillLevel = pclchar->pclCI->clCharSkill.GetTotalSkillLevel(SKILL_USEBOW1,0,0,&pclchar->pclCI->clCharItem.clItem[0]) ;

				if ( siSkillLevel > 60 ) siHitMode = 10 ;
			}
			break;
		case ATTACKTYPE_GUN:
		case ATTACKTYPE_GUN1:
		case ATTACKTYPE_NEW_GUN1:
			{
				siHitMode = 5 ;
				siSkillLevel = pclchar->pclCI->clCharSkill.GetTotalSkillLevel(SKILL_USEGUN1,0,0,&pclchar->pclCI->clCharItem.clItem[0]) ;

				if ( siSkillLevel > 60 ) siHitMode = 10 ;
			}
			break;
		default:
			{
				siHitMode = 0 ;
			}
			break;
		}

		if ( siSkillLevel < 30 ) siHitMode = 0 ;
	}

	UI08 type = 0;
	if( true == (pclCM->CR[enemyid]->pclKI != NULL && pclCM->CR[enemyid]->pclKI->IsAtb( ATB_MONSTER )) )
	{
		type = 0;
	}
	else
	{
		type = 1;
	}
	//------------------------------
	// 이웃에게 맞았음을 통보한다. 
	//------------------------------
	if(specialattackswitch == 2)
	{
		cltGameMsgResponse_Hit clHitInfo(pclCM->CR[enemyid]->GetCharUnique(), displaydamage, doubledamage,
			pclCM->CR[enemyid]->GetLife(), siHitMode, specialattackswitch, bIndure, false, false, type );
		cltMsg clMsg(GAMEMSG_RESPONSE_HIT, sizeof(clHitInfo), (BYTE*)&clHitInfo);
		((cltCharServer*)pclCM->CR[enemyid])->SendNeighbourMsg(&clMsg, true);
	}
	else
	{
		cltGameMsgResponse_Hit clHitInfo(pclCM->CR[enemyid]->GetCharUnique(), displaydamage, 0,
			pclCM->CR[enemyid]->GetLife(), siHitMode, specialattackswitch, bIndure, false, false, type );
		cltMsg clMsg(GAMEMSG_RESPONSE_HIT, sizeof(clHitInfo), (BYTE*)&clHitInfo);
		((cltCharServer*)pclCM->CR[enemyid])->SendNeighbourMsg(&clMsg, true);
	}

	//==========================================
	// 주인이 갖고 있는 소환수의 정보를 설정하고 통보 한다.
	//==========================================
	SI32 siParentID = pclCM->CR[enemyid]->GetSummonParentID();
	if(siParentID)
	{

		//==========================================
		// 
		//==========================================
		cltSummon *pclSummon = pclCM->CR[siParentID]->pclCI->clSummonInfo.GetSummon(pclCM->CR[enemyid]->GetSummonIndex());
		if(pclSummon)
		{
			pclSummon->clIP.Set(&pclCM->CR[enemyid]->pclCI->clIP);
			cltGameMsgResponse_Summon clInfo(pclCM->CR[siParentID]->GetCharUnique(), pclCM->CR[enemyid]->GetSummonIndex(), pclSummon);
			cltMsg clMsg(GAMEMSG_RESPONSE_SUMMONINFO, sizeof(clInfo), (BYTE*)&clInfo);
			pclCM->CR[siParentID]->SendNetMsg((sPacketHeader*)&clMsg);
		}
	}

	if ( IsCountrySwitch(Switch_NewDurability) )
	{
		if( pclCM->IsAlive(myid) )
		{
			cltCharServer* pclmychar	= pclCM->GetCharServer( myid );
			cltCharServer* pclenemychar = pclCM->GetCharServer( enemyid );

			bool bDecreaseDur = false;		// 내구도를 차감 시켜야 하는가?

			// 상대방 캐릭터가 몬스터인 경우에만 내구도가 차감된다.
			if( pclenemychar &&
				IsNPC(pclenemychar->GetCharUnique()) &&
				pclenemychar->clIdentity.siIdentity != IDENTITY_PERSON_SUMMON )
			{
				bDecreaseDur = true;
			}

			if( bDecreaseDur )
			{
				// 공격자의 무기 내구도를 낮춘다. 
				if( pclmychar )		pclmychar->DecreaseWeaponDur( enemyid );
			}
		}
	}

	//--------------------------------------------------
	// 수호정령에 의한 특수 능력 발동
	//--------------------------------------------------
	if ( IsCountrySwitch(Switch_SoulGuard) )
	{
		cltCharServer* pclMyChar = pclCM->GetCharServer( myid );

		// 캐릭터 일때(살아 있을때만 작동한다)
		if ( IsPC(myid) && (pclMyChar) && (pclCM->IsAlive(myid)) )
		{
			// 내 현재 HP수치가 맥스치 보다 적을때만 발동된다
			if ( pclMyChar->clPB.GetMaxLife() > pclMyChar->pclCI->clIP.GetLife() )
			{
				SI32 siAddHP = pclMyChar->SoulGuardAbilityActivity( SOULGUARD_ABILITY_RECOVER_HP );
				if ( 0 < siAddHP )
				{
					pclMyChar->IncreaseLife( siAddHP );

					cltGameMsgResponse_IncLife clInfo( pclMyChar->GetCharUnique(), pclMyChar->pclCI->clIP.GetLife(), siAddHP, pclMyChar->clPB.siReservedLife, 0, INCLIFE_REASON_SOULGUARD );
					cltMsg clMsg( GAMEMSG_RESPONSE_INCLIFE, sizeof(clInfo), (BYTE*)&clInfo );
					pclMyChar->SendNetMsg((sPacketHeader*)&clMsg);
				}
			}

			// 내 현재 MP수치가 맥스치 보다 적을때만 발동된다
			if ( pclMyChar->clPB.GetMaxMana() > pclMyChar->pclCI->clIP.GetMana() )
			{
				SI32 siAddMana = pclMyChar->SoulGuardAbilityActivity( SOULGUARD_ABILITY_RECOVER_MP );
				if ( 0 < siAddMana )
				{
					pclMyChar->IncreaseMana( siAddMana );

					cltGameMsgResponse_IncMana clInfo( pclMyChar->GetCharUnique(), pclMyChar->pclCI->clIP.GetMana(),  siAddMana, INCMANA_REASON_SOULGUARD );
					cltMsg clMsg( GAMEMSG_RESPONSE_INCMANA, sizeof(clInfo), (BYTE*)&clInfo );
					pclMyChar->SendNetMsg((sPacketHeader*)&clMsg);
				}
			}

			// 포만감이 -일때만 올려준다
			if ( 0 > pclMyChar->clPB.clHealth.pclInfo->siHungry )
			{
				SI32 siAddHungry = pclMyChar->SoulGuardAbilityActivity( SOULGUARD_ABILITY_HUNGRY_UP );
				if ( 0 < siAddHungry )
				{
					pclMyChar->clPB.clHealth.IncreaseHungry( siAddHungry );

					cltGameMsgResponse_IncHungry clInfo( 0, pclMyChar->clPB.clHealth.pclInfo->siHungry );
					cltMsg clMsg(GAMEMSG_REPONSE_INCHUNGRY, sizeof(clInfo), (BYTE*)&clInfo);
					pclMyChar->SendNetMsg((sPacketHeader*)&clMsg);
				}
			}

		}	// if ( IsPC(myid) && (pclMyChar) && (pclCM->IsAlive(myid)) )종료

		// 소환수 일때
		if ( IsNPC(myid) && (pclMyChar) && (IDENTITY_PERSON_SUMMON == pclMyChar->clIdentity.siIdentity) && (pclCM->IsAlive(myid)) )
		{
			SI32			siParentCharID	= pclMyChar->GetSummonParentID();
			cltCharServer*	pclParentChar	= pclCM->GetCharServer( siParentCharID );

			// 내 현재 HP수치가 맥스치 보다 적을때만 발동된다
			if ( (pclParentChar) && (pclMyChar->clPB.GetMaxLife() > pclMyChar->pclCI->clIP.GetLife()) )
			{
				SI32 siAddSummonHP = pclParentChar->SoulGuardAbilityActivity( SOULGUARD_ABILITY_SUMMONRECOVERY_HP );
				if ( 0 < siAddSummonHP )
				{
					pclMyChar->IncreaseLife( siAddSummonHP );

					cltGameMsgResponse_IncLife clInfo( pclMyChar->GetCharUnique(), pclMyChar->pclCI->clIP.GetLife(), siAddSummonHP, pclMyChar->clPB.siReservedLife, 0, INCLIFE_REASON_SOULGUARD );
					cltMsg clMsg( GAMEMSG_RESPONSE_INCLIFE, sizeof(clInfo), (BYTE*)&clInfo );
					pclMyChar->SendNetMsg((sPacketHeader*)&clMsg);
				}
			}

			// 내 현재 MP수치가 맥스치 보다 적을때만 발동된다
			if ( (pclParentChar) && (pclMyChar->clPB.GetMaxMana() > pclMyChar->pclCI->clIP.GetMana()) )
			{
				SI32 siAddSummonMana = pclMyChar->SoulGuardAbilityActivity( SOULGUARD_ABILITY_SOMMONRECOVERY_MP );
				if ( 0 < siAddSummonMana )
				{
					pclMyChar->IncreaseMana( siAddSummonMana );

					cltGameMsgResponse_IncMana clInfo( pclMyChar->GetCharUnique(), pclMyChar->pclCI->clIP.GetMana(),  siAddSummonMana, INCMANA_REASON_SOULGUARD );
					cltMsg clMsg( GAMEMSG_RESPONSE_INCMANA, sizeof(clInfo), (BYTE*)&clInfo );
					pclMyChar->SendNetMsg((sPacketHeader*)&clMsg);
				}
			}
		}

	}	// if ( IsCountrySwitch(Switch_SoulGuard) )

	// 획득한 경험치를 구한다. (myid가 enemyid를 공격했을 때의 경험치) 
	{

		cltAttackDamageInfo clinfo;
		clinfo.Set(&cldamageinfo);
		clinfo.siDamage = realdamage;
		SI32 exp = ExpOperate(myid, enemyid, &clinfo);
	}

	return TRUE;			
}


//===============================================
// 물리적인 타격시 부가 능력을 적용시킨다.
//===============================================
BOOL cltServer::AddScrollMagicOperate(SI32 myid, SI32 enemyid, cltAttackDamageInfo* pcldamageinfo)
{
	/*
	if(pcldamageinfo->siAttackType != ATTACKTYPE_SWORD
		&& pcldamageinfo->siAttackType != ATTACKTYPE_SPEAR
		&& pcldamageinfo->siAttackType != ATTACKTYPE_AXE
		&& pcldamageinfo->siAttackType != ATTACKTYPE_GUN
		&& pcldamageinfo->siAttackType != ATTACKTYPE_BOW
		&& pcldamageinfo->siAttackType != ATTACKTYPE_STAFF
		&& pcldamageinfo->siAttackType != ATTACKTYPE_STAFF1)	return FALSE;
	*/

		//===============================================
		// 사람과 소환수만 처리를 한다.
		//===============================================
		if(!IsPC(pclCM->CR[myid]->GetCharUnique()) && pclCM->CR[myid]->clIdentity.siIdentity != IDENTITY_PERSON_SUMMON)
			return FALSE;

	// 현재 나 자신
	cltCharServer* pclMyChar = pclCM->GetCharServer( myid );
	if ( NULL == pclMyChar )
	{
		return FALSE;
	}

	// 적
	cltCharServer* pclEnemyChar = pclCM->GetCharServer( enemyid );
	if ( NULL == pclEnemyChar )
	{
		return FALSE;
	}

	// 소환수일때 부모 캐릭터 NULL이 나올 수 있음
	cltCharServer*		pclParentChar	= NULL; 
	if ( 0 < pclMyChar->GetSummonParentID() )
	{
		pclParentChar = pclCM->GetCharServer( pclMyChar->GetSummonParentID() );
	}

	SI32				siSummonIndex	= -1;
	cltSummon*			pclSummon		= NULL;
	cltSummonScroll*	pclSummonScroll	= NULL;

	// 소환수 관련 정보는 부모캐릭터가 있어야 가능함(사용하기전 NULL 채크 반드시 할것)
	if ( pclParentChar )
	{
		siSummonIndex	= pclMyChar->GetSummonIndex();

		pclSummon		= pclParentChar->pclCI->clSummonInfo.GetSummon( siSummonIndex );
		if ( pclSummon )
		{
			pclSummonScroll = &pclSummon->clScroll;
		}
	}


	bool already = false;

	//=======================================================
	// POISON
	//=======================================================
	if(already == false)
	{
		SI32 rate = 0;
		//=======================================================
		// 소환수라면 소환수의 부적능력을 얻어온다.
		//=======================================================
		if ( pclParentChar )	
		{
			SI32 siActive = pclMyChar->ActiveWeapon;

			if ( pclSummonScroll)
			{
				rate += pclSummonScroll->GetScrollPoisonRate(pclItemManager);
			}

			rate += pclParentChar->pclCI->clSummonInfo.GetSummonEquipPoisonRate( siSummonIndex, pclItemManager, siActive );

			rate = min(300,rate) ;
		}
		//=======================================================
		if (IsPC( pclMyChar->GetCharUnique()) )
		{
			rate += pclMyChar->pclCI->clCharItem.GetEquipPoisonRate( pclItemManager, pclMyChar->ActiveWeapon );
		}

		if ( rate && (rand() % 1000  <= rate) )
		{
			pclEnemyChar->SetPoison( myid, pcldamageinfo->siDamage );
			already = true;
		}
	}

	// 지금은 전부 설정될수 있도록 한다.
	already = false;
	//=======================================================
	// ICE
	//=======================================================
	if(already == false)
	{
		SI32 rate = 0;
		//=======================================================
		// 소환수라면 소환수의 부적능력을 얻어온다.
		//=======================================================
		if ( pclParentChar )	
		{
			SI32 siActive = pclMyChar->ActiveWeapon;

			if ( pclSummonScroll)
			{
				rate += pclSummonScroll->GetScrollIceRate( pclItemManager );
			}

			rate += pclParentChar->pclCI->clSummonInfo.GetSummonEquipIceRate( siSummonIndex, pclItemManager, siActive );

			rate = min(300,rate) ;
		}
		//=======================================================
		if (IsPC( pclMyChar->GetCharUnique()) )
		{
			rate += pclMyChar->pclCI->clCharItem.GetEquipIceRate( pclItemManager, pclMyChar->ActiveWeapon );
		}

		if ( (true == pclMapManager->IsInstanceMap(pclMyChar->GetMapIndex())) || (true == (pclEnemyChar->pclKI != NULL && pclEnemyChar->pclKI->IsAtb(ATB_NOICE))) )
		{
			rate /= 20 ;
		}

		if ( rate && (rand() % 1000  <= rate) )
		{
			pclEnemyChar->SetIce( pcldamageinfo->siDamage );
			already = true;
		}
	}
	// 지금은 전부 설정될수 있도록 한다.
	already = false;

	//=======================================================
	// DRAIN
	//=======================================================
	if(already == false)
	{
		SI32 rate = 0;
		//=======================================================
		// 소환수라면 소환수의 부적능력을 얻어온다.
		//=======================================================
		if ( pclParentChar )	
		{
			SI32 siActive = PERSONITEM_SHOES;

			if ( pclSummonScroll)
			{
				rate += pclSummonScroll->GetScrollDrainRate( pclItemManager );
			}

			rate += pclParentChar->pclCI->clSummonInfo.GetSummonEquipDrainRate( siSummonIndex, pclItemManager, siActive );

			rate = min(300,rate) ;
		}
		//=======================================================
		if (IsPC( pclMyChar->GetCharUnique()) )
		{
			rate += pclMyChar->pclCI->clCharItem.GetEquipDrainRate( pclItemManager, pclMyChar->ActiveWeapon );
		}

		if(rate && (rand() % 1000  <= rate))
		{
			if ( 0 < pclMyChar->GetLife() )
			{
				if ( pclMyChar->SetDrain(pcldamageinfo->siDamage, 20, 0) && pclParentChar )
				{
					pclParentChar->SetUpdateSwitch( UPDATE_SUMMON, true, 0 );
				}

				already = true;
			}
		}
	}

	//=======================================================
	// 세트 갑옷에 의한 흡혈 20%
	//=======================================================
	already = false;
	if ( already == false )
	{
		if ( (0 < pclMyChar->GetLife()) )
		{
			SI32 siDrainRate = 0;

			if ( pclClient->IsCountrySwitch(Switch_Set_Item_Add) )
			{
				if( pclMyChar->m_siSetItemType )
				{
					cltSetItemAbilityManager* pcltSetItemAbilityManager = pclClient->pclItemManager->pcltSetItemAbilityManager;
					if( pcltSetItemAbilityManager ) 
					{
						siDrainRate = pcltSetItemAbilityManager->GetSetItemAbilityValue(pclMyChar, SETITEM_ABILITY_TYPE_DRAIN, USE_SETITEM_1 );
						siDrainRate += pcltSetItemAbilityManager->GetSetItemAbilityValue(pclMyChar, SETITEM_ABILITY_TYPE_DRAIN, USE_SETITEM_2 );
						siDrainRate += pcltSetItemAbilityManager->GetSetItemAbilityValue(pclMyChar, SETITEM_ABILITY_TYPE_DRAIN, USE_SETITEM_3 );
						siDrainRate += pcltSetItemAbilityManager->GetSetItemAbilityValue(pclMyChar, SETITEM_ABILITY_TYPE_DRAIN, USE_SETITEM_4 );
						siDrainRate += pcltSetItemAbilityManager->GetSetItemAbilityValue(pclMyChar, SETITEM_ABILITY_TYPE_DRAIN, USE_SETITEM_5 );

						already = true;
					}
				}

				//if(	(HALLOWEEN1_SETITEM == pclMyChar->m_siSetItemType)	||
				//	(HALLOWEEN2_SETITEM == pclMyChar->m_siSetItemType)	|| 
				//	(HALLOWEEN3_SETITEM == pclMyChar->m_siSetItemType)	|| 
				//	(HALLOWEEN4_SETITEM == pclMyChar->m_siSetItemType)	)
				//{
				//	siDrainRate = 10;

				//	already = true;
				//}
			}
			else
			{
				if( VAMPIRE_SETITEM == pclMyChar->m_siSetItemType )
				{
					siDrainRate = 20;

					already = true;
				}
				else if(   (HALLOWEEN1_SETITEM == pclMyChar->m_siSetItemType) 
					|| (HALLOWEEN2_SETITEM == pclMyChar->m_siSetItemType)
					|| (HALLOWEEN3_SETITEM == pclMyChar->m_siSetItemType)
					|| (HALLOWEEN4_SETITEM == pclMyChar->m_siSetItemType)	)
				{
					siDrainRate = 10;

					already = true;
				}
			}

			// 전투향상술에 의한 흡혈
			if ( (PERSONITEM_WEAPON1 == pclMyChar->ActiveWeapon) || (PERSONITEM_WEAPON2 == pclMyChar->ActiveWeapon) )
			{
				SI32 siSkillUnique		= 0;
				SI32 siItemType			= pclClient->pclItemManager->GetItemType( pclMyChar->pclCI->clCharItem.clItem[pclMyChar->ActiveWeapon].siUnique );
				switch ( siItemType )
				{
				case ITEMTYPE_SWORD:	{	siSkillUnique = SKILL_SWORD_BOW_DRAIN;	}	break;
				case ITEMTYPE_SPEAR:	{	siSkillUnique = SKILL_SPEAR_BOW_DRAIN;	}	break;
				case ITEMTYPE_AXE:		{	siSkillUnique = SKILL_AXE_BOW_DRAIN;	}	break;
				case ITEMTYPE_BOW:		{	siSkillUnique = SKILL_BOW_DRAIN;		}	break;
				case ITEMTYPE_GUN:		{	siSkillUnique = SKILL_GUN_BOW_DRAIN;	}	break;
				case ITEMTYPE_STAFF:	{	siSkillUnique = SKILL_STAFF_BOW_DRAIN;	}	break;
				}

				if ( (0 < siSkillUnique) && (MAX_SKILL_NUMBER > siSkillUnique) )
				{
					SI32 siSkillLevel		= pclMyChar->pclCI->clCharSkill.GetTotalSkillLevel( siSkillUnique, 0, 0 );
					SI16 siWeaponSkillRate	= pclClient->pclSkillManager->GetWeaponSkillRate( siSkillUnique, siSkillLevel );
					if ( 0 < siWeaponSkillRate )
					{
						SI32 siRandValue = (rand() % 100) + 1;

						pclLog->LogPrint( TEXT("[WeaponSkill-Drain] Rate:%d/RandValue:%d\n"), siWeaponSkillRate, siRandValue );

						if ( siWeaponSkillRate >= siRandValue )
						{
							siDrainRate = 30;

							already = true;
						}
					}
				}
			}

			if ( 0 < siDrainRate )
			{
				pclMyChar->SetDrain( pcldamageinfo->siDamage, siDrainRate, 0 );
			}

		}
	}

	return TRUE;
}

//===============================================
// 캐릭터의 STATUS를 변경하는 MAGIC OPERATE 
// 상태를 변경하므로 살아있을때만 적을 한다.
//===============================================
BOOL cltServer::ChangeStatusMagicOperate(SI32 myid, SI32 enemyid, cltAttackDamageInfo* pcldamageinfo)
{
	if( pclCM->IsValidID(myid) == false )		return FALSE;
	if( pclCM->IsValidID(enemyid) == false )	return FALSE;

	// 현재 Magic을 설정하고 있고 Passive magic 이라면 
	SI32 magickind = pcldamageinfo->siMagicKind;
	if(magickind < MAGIC_INDEX_START || magickind  > MAGIC_INDEX_LAST)	magickind = 0;

	// 명중률을 근거로 타격 여부를 결정한다.
	SI32 hitrate = 0;	
	// pvp-

	// by LEEKH 2007.10.10 - PVP 국가스위치를 ServerInfo로 변경
	//if( IsWhereServiceArea(ConstSwitch_PVP) )
	if(pclClient->IsCountrySwitch(Switch_HealthAddChange))
	{
		hitrate = pclCM->CR[myid]->clPB.GetHitRate( pclCM->CR[enemyid], pclCM->CR[myid], true );	

	}
	else
	{
		if(siEnablePVP == 1)
			hitrate = pclCM->CR[myid]->clPB.GetHitRate( pclCM->CR[enemyid], pclCM->CR[myid], true );	
		else
			hitrate = pclCM->CR[myid]->clPB.GetHitRate( pclCM->CR[enemyid], pclCM->CR[myid] );		
	}

	if ( pclMapManager->IsInstanceMap(pclCM->CR[myid]->GetMapIndex()) == true ||
		(pclCM->CR[enemyid]->pclKI != NULL && pclCM->CR[enemyid]->pclKI->IsAtb(ATB_NOICE)) == true
		)
		hitrate /= 20 ;

	if(pclClient->IsCountrySwitch(Switch_HealthAddChange))
	{
		if(rand() % 10000 > hitrate) return FALSE;
	}
	else
	{
		if(rand() % 100 > hitrate) return FALSE;
	}
	//=====================================================================

	//=====================================================================
	SI32 PassiveDamage = 0, PassiveMana = 0;
	if(magickind)	// 해당 Magic으로부터 Damage와 mana를 얻어온다.
	{
		CMagicAttribute *pMagicAttributePtr = pclMagicManager->GetMagicAttributePtr();

		SI32 kind = 0, min  = 0, max = 0;
		// 해당 Magic 에 적용될 기술을 얻어온다.
		if(IsNPC(pclCM->CR[myid]->GetCharUnique())||IsInstanceNPC(pclCM->CR[myid]->GetCharUnique()))	kind =pclCM->CR[myid]->GetKind();

		// [진성] 스킬북 획득 마법일 경우 스킬 정보를 얻어준다.
		SI32 SkillType = pMagicAttributePtr->GetMagicSkillType(magickind);
		if( SKILL_SIKLLBOOK == SkillType && IsPC(myid) )
		{
			cltSkillBookMagic*	pclSkillBookMagic = pclCM->CR[myid]->pclCharSkillBookInfo->GetSkillBookMagic( pclCM->CR[myid]->GetAttackType() );	
			if( NULL == pclSkillBookMagic ) return 0;

			SkillType = pclSkillBookMagic->GetSkillUnique();
		}

		SI32 SkillLevel = pclCM->CR[myid]->pclCI->clCharSkill.GetTotalSkillLevel(SkillType, kind, pclCM->CR[myid]->pclCI->clIP.GetLevel(), &pclCM->CR[myid]->pclCI->clCharItem.clItem[0]);

		// 내장기관의 상태에 따라 추가 Damage를 부과한다.
		stMagicAttributeData *pMagicAttributeData = pMagicAttributePtr->GetMagicAttribute(magickind);
		SI32 siOrganUnique = pclCM->CR[myid]->pclCI->clHealthInfo.GetOrg(pMagicAttributeData->si16MagicOrganKind);

		stMagicInfo clMagicInfo(magickind, myid, enemyid, 0);
		// 해당 기술을 이용하여  min, max, mana 를 계산한다.
		pclMagicManager->GetMinMaxDamage( &clMagicInfo, pclCM->CR[myid]->clPB.GetAddMagicDamage(),  SkillLevel, min, max, siOrganUnique );
		pclMagicManager->CalculateDamage(min, max , PassiveDamage);
		PassiveMana = pclMagicManager->GetSpendMana(magickind, SkillLevel,myid);
	}
	//=====================================================================

	// 중독 속성이 있으면 확률 
	if(pcldamageinfo->siAttackType == ATTACKTYPE_POISON)
	{
		cltCharServer* pcltempchar = (cltCharServer*)pclCM->CR[enemyid];

		DWORD dwPoisonTime = 20000;
		if( pclClient->IsCountrySwitch(Switch_SkillBook) )
		{
			if( pclCM->CR[myid]->pclCharSkillBookInfo )
			{
				SI32 siSubMagickind = 0;
				stMagicAttributeData* psSubMagicData = pclCM->CR[myid]->pclCharSkillBookInfo->FindMagicAttributeDataFromParentMagic( magickind, siSubMagickind );
				if( psSubMagicData )
				{
					dwPoisonTime += (dwPoisonTime * psSubMagicData->si32MagicMaxActionFrame) / 100;
				}
			}
		}

		pcltempchar->SetPoison( myid, pcldamageinfo->siDamage, dwPoisonTime );
	}
	// 얼음 속성이 있으면 
	if(pcldamageinfo->siAttackType == ATTACKTYPE_ICE)
	{
		SI32 siIceMagicAct = 0;
		if( pclClient->IsCountrySwitch(Switch_Set_Item_Add) )
		{
			if( pclCM->CR[enemyid]->m_siSetItemType )
			{
				siIceMagicAct += pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemAbilityValue(pclCM->CR[enemyid], SETITEM_ABILITY_TYPE_ICE_MAGIC_GET_ATTACK, USE_SETITEM_1 );
				siIceMagicAct += pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemAbilityValue(pclCM->CR[enemyid], SETITEM_ABILITY_TYPE_ICE_MAGIC_GET_ATTACK, USE_SETITEM_2 );
				siIceMagicAct += pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemAbilityValue(pclCM->CR[enemyid], SETITEM_ABILITY_TYPE_ICE_MAGIC_GET_ATTACK, USE_SETITEM_3 );
				siIceMagicAct += pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemAbilityValue(pclCM->CR[enemyid], SETITEM_ABILITY_TYPE_ICE_MAGIC_GET_ATTACK, USE_SETITEM_4 );
				siIceMagicAct += pclClient->pclItemManager->pcltSetItemAbilityManager->GetSetItemAbilityValue(pclCM->CR[enemyid], SETITEM_ABILITY_TYPE_ICE_MAGIC_GET_ATTACK, USE_SETITEM_5 );
			}
		}

		if( !siIceMagicAct )
		{
			if(pclCM->CR[enemyid]->bNoIceAttackSwitch == FALSE)
			{
				cltCharServer* pcltempchar = (cltCharServer*)pclCM->CR[enemyid];
				pcltempchar->SetIce(pcldamageinfo->siDamage);
			}
		}
	}

	// 보스의 얼음 공격 속성
	if(pcldamageinfo->siAttackType == ATTACKTYPE_FROZEN)
	{
		cltCharServer* pcltempchar = (cltCharServer*)pclCM->CR[enemyid];
		pcltempchar->SetFrozen(pcldamageinfo->siDamage);
	}
	// 방어력저하 속성이 있으면 
	if(pcldamageinfo->siAttackType == ATTACKTYPE_CURSEDEFENSE)
	{
		cltCharServer* pcltempchar = (cltCharServer*)pclCM->CR[myid];
		pcltempchar->SetCurseDefense(pcldamageinfo->siDamage, PassiveDamage, PassiveMana, enemyid);
	}
	// 이동속도 저하가 있으면 
	if(pcldamageinfo->siAttackType == ATTACKTYPE_CURSEMOVESPEED)
	{
		cltCharServer* pcltempchar = (cltCharServer*)pclCM->CR[myid];
		//[진성] 타임브레이크는 마나를 중복으로 소모하지 않게 하기위해. => 2008-6-16
		//pcltempchar->SetCurseMovespeed(pcldamageinfo->siDamage, PassiveDamage, PassiveMana, enemyid);
		pcltempchar->SetCurseMovespeed(pcldamageinfo->siDamage, PassiveDamage, 0, enemyid);
	}

	// 영항마법 속성이 있으면 
	if(pcldamageinfo->siAttackType == ATTACKTYPE_CANNON_MAGIC)
	{
		if(pclCM->CR[enemyid]->bNoIceAttackSwitch == FALSE)
		{
			cltCharServer* pcltempchar = (cltCharServer*)pclCM->CR[enemyid];
			pcltempchar->SetIce(pcldamageinfo->siDamage);
		}
	}
	// 불붙는 마법 속성이 있으면
	if(pcldamageinfo->siAttackType == ATTACKTYPE_MAGICFIREBURN)
	{
		cltCharServer* pcltempchar = (cltCharServer*)pclCM->CR[enemyid];
		pcltempchar->SetMagicFire(myid, pcldamageinfo->siDamage);
	}
	return TRUE;
}


//===============================================
// PASSIVE MAGIC을 적용시킨다. 
// PASSIVE MAGIC은 현재 Target이 죽던질 살던지 적용한다.
//===============================================
BOOL cltServer::PassiveMagicOperate(SI32 myid, SI32 enemyid, cltAttackDamageInfo* pcldamageinfo)
{
	//=====================================================================
	// PREPROCESS : Passive 을 계산하기전에 미리 계산 
	// PASSIVE를 수행할 캐릭터가 죽어있으면 돌아간다.
	if( pclCM->IsAlive(myid) == FALSE)			return FALSE;
	if( pclCM->IsValidID(enemyid) == FALSE)		return FALSE;

	// 현재 Magic을 설정하고 있고 Passive magic 이라면 
	SI32 magickind = pclCM->CR[myid]->GetCurrentMagic();
	SI32 magicactive = pclCM->CR[myid]->GetCurrentMagicCastType();

	// 현재 마법이 설정되어있지 않거나 PASSIVE마법이 아니라면
	if ( (magickind < MAGIC_INDEX_START) || (MAGIC_CASTTYPE_PASSIVE != magicactive) )	return FALSE;

	// 현재 공격한 무기타입과 PASSIVE 무기 타입이 일치하면 작동한다.
	CMagicAttribute *pMagicAttributePtr = pclMagicManager->GetMagicAttributePtr();
	if(pMagicAttributePtr->IsMatchAttackTye(myid, magickind, pcldamageinfo->siAttackType) == false) return FALSE;

	SI32 hitrate = 0;	
	// pvp
	//if( IsWhereServiceArea(ConstServiceArea_English) )
	if(siEnablePVP == 1)
	{
		hitrate = pclCM->CR[myid]->clPB.GetHitRate( pclCM->CR[enemyid], pclCM->CR[myid], true );	
	}
	else
		hitrate = pclCM->CR[myid]->clPB.GetHitRate( pclCM->CR[enemyid], pclCM->CR[myid] );	

	if(pclClient->IsCountrySwitch(Switch_HealthAddChange))
	{
		if(rand() % 10000 > hitrate) return FALSE;
	}
	else
	{
		if(rand() % 100 > hitrate) return FALSE;
	}
	//=====================================================================

	// 해당 Magic으로부터 Damage와 mana를 얻어온다.
	SI32 PassiveDamage = 0, PassiveMana = 0;
	SI32 kind = 0, min  = 0, max = 0;
	// 해당 Magic 에 적용될 기술을 얻어온다.
	if(IsNPC(pclCM->CR[myid]->GetCharUnique())||IsInstanceNPC(pclCM->CR[myid]->GetCharUnique()))	kind =pclCM->CR[myid]->GetKind();

	// [진성] 스킬북 획득 마법일 경우 스킬 정보를 얻어준다.
	SI32 SkillType = pMagicAttributePtr->GetMagicSkillType(magickind);
	if( SKILL_SIKLLBOOK == SkillType && IsPC(myid)  )
	{
		cltSkillBookMagic*	pclSkillBookMagic = pclCM->CR[myid]->pclCharSkillBookInfo->GetSkillBookMagic( pclCM->CR[myid]->GetAttackType() );	
		if( NULL == pclSkillBookMagic ) return FALSE;

		SkillType = pclSkillBookMagic->GetSkillUnique();
	}

	SI32 SkillLevel = pclCM->CR[myid]->pclCI->clCharSkill.GetTotalSkillLevel(SkillType, kind, pclCM->CR[myid]->pclCI->clIP.GetLevel(), &pclCM->CR[myid]->pclCI->clCharItem.clItem[0]);

	// 내장기관의 상태에 따라 추가 Damage를 부과한다.
	stMagicAttributeData *pMagicAttributeData = pMagicAttributePtr->GetMagicAttribute(magickind);
	SI32 siOrganUnique = pclCM->CR[myid]->pclCI->clHealthInfo.GetOrg(pMagicAttributeData->si16MagicOrganKind);

	stMagicInfo clMagicInfo(magickind, myid, enemyid, 0);
	// 해당 기술을 이용하여  min, max, mana 를 계산한다.
	pclMagicManager->GetMinMaxDamage( &clMagicInfo, pclCM->CR[myid]->clPB.GetAddMagicDamage(),  SkillLevel, min, max, siOrganUnique );
	pclMagicManager->CalculateDamage(min, max , PassiveDamage);
	PassiveMana = pclMagicManager->GetSpendMana(magickind, SkillLevel,myid);
	//=====================================================================


	BOOL bSuccess = FALSE;
	switch(pMagicAttributePtr->GetMagicGroup(magickind))
	{
		//=======================================================
		// DRAIN
		//=======================================================
	case MAGIC_GROUP_DRAIN:
		{
			// 내가 성황신이나 성황신 소환수라면 DRAIN은 발휘되지 않는다.
			if( pclCM->CR[myid]->clIdentity.siIdentity == IDENTITY_VILLAGEGOD ||
				pclCM->CR[myid]->clIdentity.siIdentity == IDENTITY_VILLAGEGOD_SUMMON)
			{
				PassiveDamage =  1;
			}
			// 소환수라면 Drain 양을 반으로 줄인다.
			else if( pclCM->CR[myid]->clIdentity.siIdentity == IDENTITY_PERSON_SUMMON)
			{
				PassiveDamage /= 2;
			}
			cltCharServer* pcltempchar = (cltCharServer*)pclCM->CR[myid];
			bSuccess = pcltempchar->SetDrain(pcldamageinfo->siDamage, PassiveDamage, PassiveMana);
		}
		break;
	case MAGIC_GROUP_CURSEATTACK:
		{
			cltCharServer* pcltempchar = (cltCharServer*)pclCM->CR[myid];
			bSuccess = pcltempchar->SetCurseAttack(pcldamageinfo->siDamage, PassiveDamage, PassiveMana, enemyid);
		}
		break;
	case MAGIC_GROUP_CURSECRITICAL:
		{
			cltCharServer* pcltempchar = (cltCharServer*)pclCM->CR[myid];
			bSuccess = pcltempchar->SetCurseCritical(pcldamageinfo->siDamage, PassiveDamage, PassiveMana, enemyid);
		}
		break;
	default: 
		break;
	}

	// 기타 조건에 의해서 마법이 구현되었을 때만 손상시킨다.
	if(bSuccess)
	{
		// 마법사용에 따른 내장기관의 손상을 발생시킨다.
		if(IsPC(pclCM->CR[myid]->GetCharUnique()))
		{
			if(pclCM->CR[myid]->pclCI->clHealthInfo.DecreaseOrg(pMagicAttributeData->si16MagicOrganKind, pMagicAttributeData->si16MagicOrganDamage, pclCM->CR[myid]->pclCI->clIP.GetLevel()) == true)
			{
				pclCM->CR[myid]->SetUpdateSwitch(UPDATE_HEALTH, true, 0);		// 클라이언트에 업데이트 시킨다. 
			}
		}
		// 어떤 마법에 몇개나 사용하는지 검사한다.
		if(IsPC(pclCM->CR[myid]->GetCharUnique()))
		{
			pclMagicManager->IncreaseMagicUse(magickind);
		}

		// 소환수라면 업데이트 시켜준다.
		SI32 siParentID = pclCM->CR[myid]->GetSummonParentID();
		if(pclCM->IsAlive(siParentID) == TRUE)
		{
			pclCM->CR[siParentID]->SetUpdateSwitch(UPDATE_SUMMON, true, 0);
		}

		// 기술경험치를 증가킨다.
		cltAttackDamageInfo clinfo;
		clinfo.Set(pcldamageinfo);
		clinfo.siMagicKind	= magickind;
		clinfo.siDamage		= PassiveDamage;
		ExpSkillOperate(myid, enemyid, &clinfo);
	}

	return bSuccess;

}
// myid가 enemyid를 공격했을 때의 경험치를 얻어온다. 	
SI32 cltServer::GetExp(SI16 myid, SI16 enemyid, SI16 damage)
{
	SI32 exp = pclCM->CR[enemyid]->GetMyExp();

	// 서비스 타임이라면...
	SI32 bonusPercent = 0;

	if(pclClient->bBonusTimeSwitch)
	{
		if( bIsGMBonusTime && siGMBonusTimePercent > 0 )
		{
			bonusPercent = siGMBonusTimePercent;
		}
		else
		{
			if( pclClient->siServiceArea == ConstServiceArea_English )
			{
				bonusPercent = 30;
			}
			else if( pclClient->siServiceArea == ConstServiceArea_Japan )
			{
				bonusPercent = 50;
			}
			//LEEKH - 0810 - 군주S 경험치 적용비율 수정.(+20%)
			else if( pclClient->siServiceArea == ConstServiceArea_Korea)
			{
				bonusPercent = 20;
			}
			// USA 경험치 적용비율 수정.(+100%) - by LEEKH 2007.12.03
			else if( pclClient->siServiceArea == ConstServiceArea_USA )
			{
				// 미국 임시 추가 - 군주 보너스 타임은 130%
				if(bBonusTimeGoonzu)
				{
					bonusPercent = 30;

					if(true == bAlldayBonus)
						bonusPercent = 100;
				}
				else
				{
					bonusPercent = 100;
				}
			}
			else if( pclClient->siServiceArea == ConstServiceArea_NHNChina )
			{
				bonusPercent = 30;
			}
			else if( pclClient->siServiceArea == ConstServiceArea_EUROPE )
			{
				bonusPercent = 30;
			}
			else
			{
				bonusPercent = 100;
			}

		}

	}

	// 군주 데이
	if( pclEventTimeManager->InEventTime("GoonzuDay_HuntExpBonus150_9", &sTime) )
	{
		cltEventTime* pclEventTime = pclEventTimeManager->GetEvent("GoonzuDay_HuntExpBonus150_9");
		if(pclEventTime)	{	bonusPercent = pclEventTime->m_siBonusAmount;	}
	}
	if( pclEventTimeManager->InEventTime("GoonzuDay_HuntExpBonus150_15", &sTime) )
	{
		cltEventTime* pclEventTime = pclEventTimeManager->GetEvent("GoonzuDay_HuntExpBonus150_15");
		if(pclEventTime)	{	bonusPercent = pclEventTime->m_siBonusAmount;	}
	}
	if( pclEventTimeManager->InEventTime("GoonzuDay_HuntExpBonus200_21", &sTime) )
	{
		cltEventTime* pclEventTime = pclEventTimeManager->GetEvent("GoonzuDay_HuntExpBonus200_21");
		if(pclEventTime)	{	bonusPercent = pclEventTime->m_siBonusAmount;	}
	}

	// 피시방 사용자라면

	//넷카페 프리미과 파나의 성장비약을 사용했을시 효과 중복
	// 2007.06.27 [성웅]
	if ( pclCM->CR[myid]->pclCI->clBI.bPCRoom )
	{		
		if( pclCM->CR[myid]->pclCI->clBI.bPremiumService )
		{
			bonusPercent = max( bonusPercent , 50 );
		}
		bonusPercent += 50;
	}

	// 수수깨끼 소환알2에서 나온 몬스터라면 경험치 100% 추가
	cltCharServer* pclenemychar = (cltCharServer*)pclClient->pclCM->CR[enemyid];
	if(pclenemychar->siCharMode == CHARMODE_SUMMON_EGG2)
	{
		bonusPercent += 100;
	}

	cltBufInfo * pclMyBuf = &pclCM->CR[myid]->pclCI->clBI.clBufInfo;
	SI32 parentID  = pclCM->CR[myid]->GetSummonParentID();
	if( IsPC(myid) )
	{
		if(    pclMyBuf->IsHaveBuf(BUF_LOVEPOWER) 
			|| pclMyBuf->IsHaveBuf(BUF_FRIENDSHIP) 
			|| pclMyBuf->IsHaveBuf(BUF_HINAMOCHI) 
			|| pclMyBuf->IsHaveBuf(BUF_PREMIUMEXPUP)
			|| pclMyBuf->IsHaveBuf(BUF_SUPEREXPUP)
			|| pclMyBuf->IsHaveBuf(BUF_REWARDBONUS) 
			|| pclMyBuf->IsHaveBuf(BUF_EXP_UP) 
			|| pclMyBuf->IsHaveBuf(BUF_SPECIALAMPUL_EXPUP300)
			|| pclMyBuf->IsHaveBuf(BUF_GIANTRACCOON_RESURRECT_EVENT_FIRST_VILLAGE)	
			|| pclMyBuf->IsHaveBuf(BUF_RESIDENTS_EXPUP_10)
			|| pclMyBuf->IsHaveBuf(BUF_VALENTINEDAY))
		{
			//내가 PC이고 열혈버프 중이면 중이라면 경험치를 100% 증가 
			if( pclMyBuf->IsHaveBuf(BUF_SUPEREXPUP) )
			{
				bonusPercent += 100 ;
			}
			//내가 PC이고 사랑 버프 중이라면 경험치를 100% 증가 
			if( pclMyBuf->IsHaveBuf(BUF_LOVEPOWER) )
			{
				bonusPercent += 100 ;
			}
			//내가 PC이고 우정 버프 중이라면 경험치를 50% 증가

			if( pclMyBuf->IsHaveBuf(BUF_FRIENDSHIP) )
			{
				// 보너스가 줄어들진 않는다.
				bonusPercent += 50 ;
			}	

			// 히나모찌 를 먹었다면 경험치 + 100%;
			if( pclMyBuf->IsHaveBuf(BUF_HINAMOCHI) )
			{
				bonusPercent += 100;
			}

			if( pclMyBuf->IsHaveBuf(BUF_PREMIUMEXPUP) )
			{
				bonusPercent += 50 ;
			}

			// 이관 보상 아이템 경험치 + 100%
			if( pclMyBuf->IsHaveBuf(BUF_REWARDBONUS) )
			{
				bonusPercent += 100 ;
			}
			// [진성] 경험치 증가 버프 추가. => 2008-6-27
			if(pclMyBuf->IsHaveBuf(BUF_EXP_UP))
			{
				bonusPercent += pclMyBuf->GetBufPercent(BUF_EXP_UP);
			}
			// [지연] 스페셜 엠플을 먹었다면 전투 경험치 + 200%
			if( pclMyBuf->IsHaveBuf(BUF_SPECIALAMPUL_EXPUP300))
			{
				bonusPercent += 200;
			}
			// [진성] 자이언트 라쿤 부활 이벤트에 구조물 최초 완성을 한 마을이면. 경험치를 50% 증가 
			if( pclMyBuf->IsHaveBuf(BUF_GIANTRACCOON_RESURRECT_EVENT_FIRST_VILLAGE))
			{
				bonusPercent += 50;
			}
			// [지연] 마을 EXP 캡슐을 먹었다면 전투 경험치 + 10%
			if( pclMyBuf->IsHaveBuf( BUF_RESIDENTS_EXPUP_10))
			{
				bonusPercent += 10;
			}

			if( pclMyBuf->IsHaveBuf( BUF_VALENTINEDAY))
			{
				bonusPercent += 20;
			}
		}
		else	// 사랑, 우정 버프가 없을때만 프리미엄 서비스(파나의 성장비약) 을 체크한다
		{
			if( pclCM->CR[myid]->pclCI->clBI.bPremiumService )
			{
				bonusPercent = max( bonusPercent , 50 );
			}
		}

		if(IsCountrySwitch(Switch_Server_Event))
		{	
			//서버 이벤트 진행 기간이면...
			if( pclEventTimeManager->InEventTime("GoonzuDay_Week", &sTime) )
			{
				//[진성] 서버 이벤트 하루 1등 서버이면 버프가 적용된다. => 2008-6-23
				// 버프 타입확인.
				if( pclClient->pclServerEventManager->GetBonusBufType() == CNServerEvent_Manager::SERVER_EVENT_BUF_ATTACK_EXP_150 || 
					pclClient->pclServerEventManager->GetBonusBufType() == CNServerEvent_Manager::SERVER_EVENT_BUF_ATTACK_EXP_200		)
				{
					// 중복불가.
					if( false == pclMyBuf->IsHaveBuf(BUF_HINAMOCHI)    &&
						false == pclMyBuf->IsHaveBuf(BUF_FRIENDSHIP)   &&
						false == pclMyBuf->IsHaveBuf(BUF_LOVEPOWER)    &&
						false == pclMyBuf->IsHaveBuf(BUF_MAKEEXPUP)	   &&
						false == pclMyBuf->IsHaveBuf(BUF_PREMIUMEXPUP) &&
						false == pclMyBuf->IsHaveBuf(BUF_SUPEREXPUP)   &&
						false == pclMyBuf->IsHaveBuf(BUF_REWARDBONUS)  &&
						false == pclMyBuf->IsHaveBuf(BUF_EXP_UP) )
					{
						switch(pclClient->pclServerEventManager->GetBonusBufType())
						{
						case CNServerEvent_Manager::SERVER_EVENT_BUF_ATTACK_EXP_150:
							{
								bonusPercent += 50 ;
							}
							break;
						case CNServerEvent_Manager::SERVER_EVENT_BUF_ATTACK_EXP_200:
							{
								bonusPercent += 100 ;
							}
							break;
						}
					}
				}
			}
			else
			{
				//서버 이벤트 보상 기간이면...
				if( pclEventTimeManager->InEventTime("GoonzuDay_Week_Reward", &sTime) )
				{
					// 중복불가.
					if( false == pclMyBuf->IsHaveBuf(BUF_HINAMOCHI)    &&
						false == pclMyBuf->IsHaveBuf(BUF_FRIENDSHIP)   &&
						false == pclMyBuf->IsHaveBuf(BUF_LOVEPOWER)    &&
						false == pclMyBuf->IsHaveBuf(BUF_MAKEEXPUP)	   &&
						false == pclMyBuf->IsHaveBuf(BUF_PREMIUMEXPUP) &&
						false == pclMyBuf->IsHaveBuf(BUF_SUPEREXPUP)   &&
						false == pclMyBuf->IsHaveBuf(BUF_REWARDBONUS)  &&
						false == pclMyBuf->IsHaveBuf(BUF_EXP_UP) )
					{
						// 최종 1위 서버에게는 경험치를 주는데 
						if(pclClient->pclServerEventManager->IsMyServerRankBest())
						{
							bonusPercent += 50;
							//bonusPercent += 100;
						}
						/*else if(pclClient->pclServerEventManager->IsMyServerRankBest2())
						{
							bonusPercent += 50;
						}*/
					}
				}
				else
				{
					// 서버 이벤트 보상으로 사용하던 맵에 기간외의 시간에 들어가 있으면 경험치 2배로주자.
					if( pclClient->pclServerEventManager->IsEventMap(pclCM->CR[myid]->GetMapIndex()) )
					{
						bonusPercent += 100;
					}
				}
			}
		}
		SI16 siMyPVPLeagueGrade = pclCM->CR[myid]->GetPVP_LeagueGrade();

		// PVP 무한대전 등급관련 보너스
		switch ( siMyPVPLeagueGrade )
		{
		case PVP_LEAGUE_GRADE_1:
			{
				if ( pclEventTimeManager->InEventTime(TEXT("PVP_League_1Grade_Bonus"), &sTime) )
				{
					cltEventTime* pcl1GradeBonue = pclEventTimeManager->GetEvent( TEXT("PVP_League_1Grade_Bonus") );
					if ( pcl1GradeBonue )
					{
						bonusPercent += pcl1GradeBonue->m_siBonusAmount;
					}
				}
				else if ( pclEventTimeManager->InEventTime(TEXT("PVP_League_1Grade_Bonus_2"), &sTime) )
				{
					cltEventTime* pcl1GradeBonue = pclEventTimeManager->GetEvent( TEXT("PVP_League_1Grade_Bonus_2") );
					if ( pcl1GradeBonue )
					{
						bonusPercent += pcl1GradeBonue->m_siBonusAmount;
					}
				}
			}
			break;

		case PVP_LEAGUE_GRADE_2:
			{
				if ( pclEventTimeManager->InEventTime(TEXT("PVP_League_2Grade_Bonus"), &sTime) )
				{
					cltEventTime* pcl2GradeBonue = pclEventTimeManager->GetEvent( TEXT("PVP_League_2Grade_Bonus") );
					if ( pcl2GradeBonue )
					{
						bonusPercent += pcl2GradeBonue->m_siBonusAmount;
					}
				}
				else if ( pclEventTimeManager->InEventTime(TEXT("PVP_League_2Grade_Bonus_2"), &sTime) )
				{
					cltEventTime* pcl2GradeBonue = pclEventTimeManager->GetEvent( TEXT("PVP_League_2Grade_Bonus_2") );
					if ( pcl2GradeBonue )
					{
						bonusPercent += pcl2GradeBonue->m_siBonusAmount;
					}
				}
			}
			break;

		case PVP_LEAGUE_GRADE_3:
			{
				if ( pclEventTimeManager->InEventTime(TEXT("PVP_League_3Grade_Bonus"), &sTime) )
				{
					cltEventTime* pcl3GradeBonue = pclEventTimeManager->GetEvent( TEXT("PVP_League_3Grade_Bonus") );
					if ( pcl3GradeBonue )
					{
						bonusPercent += pcl3GradeBonue->m_siBonusAmount;
					}
				}
				else if ( pclEventTimeManager->InEventTime(TEXT("PVP_League_3Grade_Bonus_2"), &sTime) )
				{
					cltEventTime* pcl3GradeBonue = pclEventTimeManager->GetEvent( TEXT("PVP_League_3Grade_Bonus_2") );
					if ( pcl3GradeBonue )
					{
						bonusPercent += pcl3GradeBonue->m_siBonusAmount;
					}
				}
			}
			break;
		}	// switch ( siMyPVPLeagueGrade )

		// [영훈] 결혼에 의한 경험치 상승
		if ( IsCountrySwitch(Switch_WeddingSystem) )
		{
			if ( (m_pclMarriageMgr) && (TRUE == pclCM->CR[myid]->IsMarried()) && (MARRIAGE_MATE_CONNECT_MODE_SAMEMAP == pclCM->CR[myid]->pclMarriage->GetConnectMode()) )
			{
				SI32 siMarriageValue = m_pclMarriageMgr->GetTotalExpAbilityValue( pclCM->CR[myid]->pclMarriage->GetLoveLevel() );
				if ( 0 < siMarriageValue )
				{
					bonusPercent += siMarriageValue;
				}
			}
		}
		// [성웅] 가족 관계에 의한 경험치 상승
		if ( IsCountrySwitch((Switch_We_Are_Family) ) )	// 국가코드는 아직..
		{
			cltCharServer* pclChar = pclCM->GetCharServer( myid)	;
			if(pclChar->pclFamily != NULL	)
			{
				if( pclChar->pclFamily->GetMyClass_InFamily() == MYCLASS_IN_FAMILY_PARANTS )	// 일단 자신이 부모 케릭터여야 한다.
				{
					SI32 siOnChildNum = pclChar->pclFamily->GetOnChildrenNum()	;
					if ( siOnChildNum > 0 )	// 자식케릭터가 1명 이상 접속 해야 한다.
					{
						cltClient* pclclient = (cltClient*)pclClient;
						SI32 siAddValue	=	0	;

						SYSTEMTIME stFamilyEffectTime_Now	;

						// 가족 이용권 기간이 아직 남아 있는 자식 숫자를 찾는다.
						for ( SI32 siSlot = 0 ; siSlot < MAX_CHILDREN_NUM ; siSlot++)
						{
							SI32 siCharPersonID = pclChar->pclFamily->GetChildPersonID( siSlot)	;
							if( siCharPersonID  > 0)
							{
								ZeroMemory( &stFamilyEffectTime_Now ,sizeof( SYSTEMTIME ))	;
								stFamilyEffectTime_Now	= pclChar->pclFamily->GetChild_Add_Date( siSlot ) ;						

								if ( stFamilyEffectTime_Now.wMonth > 0 && stFamilyEffectTime_Now.wMonth <= MAX_MONTH_FAMILY	&& 
									stFamilyEffectTime_Now.wDay > 0 && stFamilyEffectTime_Now.wDay <= MAX_DAY_FOR_FAMILY)
								{
									if ( sTime.wMonth > 0 && sTime.wMonth <= MAX_MONTH_FAMILY && sTime.wDay > 0 && sTime.wDay <= MAX_DAY_FOR_FAMILY  )
									{
										SI64 servertime = GetTimeValue(&sTime )	;
										SI64 limittime = GetTimeValue(&stFamilyEffectTime_Now )	;
										if ( servertime >0 && limittime > 0 )
										{
											if ( servertime < limittime )
											{
												siAddValue++	;

											}

										}

									}

								}
							}							
						}
						if ( MAX_FAMILY_ADD_EXP <= siAddValue )	{						siAddValue = MAX_FAMILY_ADD_EXP	; }	// 최대는 10%
						// 변경된 밸류 적용 
						bonusPercent += siAddValue;
						siAddValue = 0 ;	// 적용하고 또 초기화 
					}
				}
			}

		}

		// 수호정령에 의한 경험치 증가
		if ( IsCountrySwitch(Switch_SoulGuard) )
		{
			SI32 siSoulGuardValue = ((cltCharServer*)pclCM->CR[myid])->SoulGuardAbilityActivity( SOULGUARD_ABILITY_EXP_UP );
			if ( 0 < siSoulGuardValue )
			{
				bonusPercent += siSoulGuardValue;
			}
		}

		// [춘기] 크리스마스 이벤트 - 엔펠리스 앞에 크리스마스 트리가 세워지면 1단계 : 5%, 2단계 : 10%의 경험치 증가 효과를 받는다.
		if((pclClient->pclEventTimeManager->InEventTime( TEXT("ChristmasEvent"), &sTime) == true))
		{
			cltServer* pclserver = (cltServer*)pclClient;

			SI32	siWorldChristmasTree01Kind = pclClient->GetUniqueFromHash(TEXT("KIND_CHRISTMASWORLDTREE01"));
			SI32	siWorldChristmasTree02Kind = pclClient->GetUniqueFromHash(TEXT("KIND_CHRISTMASWORLDTREE02"));

			SI32 siWorldChristmasTree01ID = pclserver->siWorldTreeID[0];
			SI32 siWorldChristmasTree02ID = pclserver->siWorldTreeID[1];

			cltCharServer*	pclWorldChristmasTreeChar01 = (cltCharServer*)pclClient->pclCM->CR[siWorldChristmasTree01ID];
			cltCharServer*	pclWorldChristmasTreeChar02 = (cltCharServer*)pclClient->pclCM->CR[siWorldChristmasTree02ID];

			if(pclWorldChristmasTreeChar01)
			{
				if((pclWorldChristmasTreeChar01->GetMapIndex() == MAPINDEX_MAINFIELD) && (pclWorldChristmasTreeChar01->GetKind() == siWorldChristmasTree01Kind))
				{
					bonusPercent += 5;
				}
			}

			if(pclWorldChristmasTreeChar02)
			{
				if((pclWorldChristmasTreeChar02->GetMapIndex() == MAPINDEX_MAINFIELD) && (pclWorldChristmasTreeChar02->GetKind() == siWorldChristmasTree02Kind))
				{
					bonusPercent += 10;
				}
			}
		}
	}
	else if( pclCM->IsValidID( parentID ) )	// 소환수라면
	{
		//부모가 프리미엄 사용자라면
		if ( pclCM->CR[parentID]->pclCI->clBI.bPremiumService || pclCM->CR[parentID]->pclCI->clBI.clBufInfo.IsHaveBuf(BUF_PREMIUMEXPUP) )
		{
			bonusPercent = max( 50 , bonusPercent ) ;
		}
	}


	// 경험치 보너스가 존재한다면 추가해준다.
	if( bonusPercent > 0 )
	{
		exp += (SI32)(exp * bonusPercent / 100 );
	}

	cltCharServer* pclchar = (cltCharServer*)pclCM->CR[enemyid];	// 죽은놈. 


	//---------------------------------------------
	// 전투력 편차에 의한 경험치 보정. 
	//---------------------------------------------
	// 적의 전투력을 구한다. 
	SI32 enemybattle	= pclCM->CR[enemyid]->clPB.GetBattlePower();

	// 공격자의 전투력을 구한다. 
	SI32 attackerbattle	= pclCM->CR[myid]->clPB.GetBattlePower();

	// 사용자이고 (소환수는 이런 적용을 받지 않는다.) 
	// 내 레벨이 20 이하일때는 조정이 없다. 
	// 전투력 차이가 많이나면 경험치를 조정한다. 

	// 배고픔 패널티
	SI32 addHunger = 0;

	if(IsPC(myid) && pclCM->CR[myid]->pclCI->clIP.GetLevel() > 20 && attackerbattle >= 0)
	{
		SI32 percent = 0;
		bool bDoHunger = false;

		// 50레벨 이상부터만 배고픔 증가
		if (pclCM->CR[myid]->pclCI->clIP.GetLevel() >= 50)
			bDoHunger = true;

		if(attackerbattle>0)	percent = enemybattle * 100 / attackerbattle;
		else					percent = 0;
		// 40%
		if(percent  < 40)		
		{	
			exp = 0;	

			if (bDoHunger)
				addHunger = 5;
		}
		// 50%
		else if( percent  < 50)	
		{	
			exp = exp * 30 / 100;

			if (bDoHunger)
				addHunger = 4;
		}
		// 60%
		else if( percent  < 60)	
		{	
			exp = exp * 40 / 100;		

			if (bDoHunger)
				addHunger = 3;

		}
		// 70%
		else if( percent  < 70)	
		{	
			exp = exp * 60 / 100;		

			if (bDoHunger)
				addHunger = 2;

		}
		// 80%
		else if( percent  < 80)	
		{	
			exp = exp * 80 / 100;		

			if (bDoHunger)
				addHunger = 1;

		}

	}

	//-----------------------------------------
	// 스틸인 경우 경험치를 주지 않는다. 
	//-----------------------------------------
	if(pclCM->CR[enemyid]->clJim.siJimHostCharUnique)
	{
		SI32 jimid = pclCM->GetIDFromCharUnique( pclCM->CR[enemyid]->clJim.siJimHostCharUnique );
		if(jimid)
		{
			// 내가 찜한 몬스터가 아니고 
			// 내 주인이 찜한 몬스터도 아니고 
			// 나 소환수가 찜한 몬스터도 아니다.
			if(jimid != myid 
				&& jimid != pclCM->CR[myid]->GetSummonID()
				&& jimid != pclCM->CR[myid]->GetSummonParentID())
			{
				// 찜한 캐릭터가 파티에 속해있지 않다면 
				if(pclCM->CR[jimid]->siPartyIndex == 0 ||  (pclCM->CR[jimid]->siPartyIndex != pclCM->CR[myid]->siPartyIndex))
				{
					exp = 0;

					// 악질적인 스틸이면, 
					if(TABS(GetFrame() - pclCM->CR[enemyid]->clJim.dwNewJimFrame) > 100)
					{
						// 해당 몬스터가 사망했을 때만 악의적인 스틸로 인정된다. 
						if(pclCM->IsAlive(enemyid) == false)
						{
							cltCharServer* pclchar = (cltCharServer*)pclCM->CR[myid];

							pclchar->siStealNum++;
							SI32 stealnum		=	pclchar->GetStealNum();
							CMinisterHyungzo *pclminister = (CMinisterHyungzo *)pclMinisterMgr->GetMinister(MINISTER_HYUNGZO);

							SI32 maxstealnum	= min(10, max(3, pclminister->m_siLimitStealNum));
							if(stealnum < maxstealnum)
							{
								SendServerResponseMsg( 0, SRVAL_BADSTEALACTION,  stealnum, maxstealnum, pclCM->CR[myid]->GetCharUnique() );
							}
							else
							{
								// 경험치를 줄여준다.(level * 10)
								SI32 changedexp = pclchar->pclCI->clIP.GetLevel() * 10;
								pclchar->pclCI->clIP.DecreaseExp( changedexp );

								// 클라이언트로 정보를 보낸다.
								// 경험치 얻었음을 통보한다. 
								cltGameMsgResponse_GetExp clinfo(pclchar->GetCharUnique(),  -changedexp,  0, 0);
								cltMsg clMsg(GAMEMSG_RESPONSE_GETEXP, sizeof(clinfo), (BYTE*)&clinfo);
								pclchar->SendNetMsg((sPacketHeader*)&clMsg);

								// 안전 지대로 보낸다. 
								pclchar->WarpToSafeZone();

								SendServerResponseMsg( 0, SRVAL_BADSTEALACTIONPUNISH,  changedexp, 0, pclCM->CR[myid]->GetCharUnique() );
							}
						}
					}
				}
			}
		}
	}

	if(pclCM->IsAlive(myid) == TRUE)
	{
		// 배고픔을 증가시킨다. 
		if(rand() % 8 == 0)
		{
			((cltCharServer*)pclCM->CR[myid])->MakeHunger(addHunger);
		}

	}

	if(pclCM->IsAlive(enemyid) == TRUE)
	{
		// 적이 살아 있다면 타격을 준 것에 비례하여 경험치를 준다. 
		SI32 enemymaxdamage = pclCM->CR[enemyid]->clPB.GetMaxLife();

		if(enemymaxdamage)
		{
			exp  = (exp * damage) / enemymaxdamage / 3;
		}
		else
		{
			exp  = 0;
		}

	}

	return exp;

}			

// 호위병을 만든다. 
SI32 cltServer::MakeGuardChar(SI32 kind, SI16 homevillage, SI32 mapindex, SI32 x, SI32 y, cltOrderDataInfo* pclorder, cltEnemyTypeInfo* pclenemytype)
{

	if(kind)
	{
		cltInnerStatusInfo clinnerstatus;
		clinnerstatus.clEnemyType.Set(pclenemytype);

		SI32 guardid = SetNPCChar(kind, homevillage, NULL, 100, 0, x, y, mapindex, NPCITEMMODE_NORMAL, &clinnerstatus, pclorder );
		if(guardid)
		{
			return guardid; 
		}
	}

	return 0;

}

// myid가  enemyid를 공격했을때의 경험치 조치. 
SI32 cltServer::ExpOperate(SI32 myid, SI32 enemyid, cltAttackDamageInfo* pcldamageinfo)
{
	WRITE_MEMLOG( 130, 1, 0, 0 );

	SI32 i;

	//--------------------------------------------------------------------
	// 공격자가 사망시 경험치를 전혀 받을 수 없다. 
	//--------------------------------------------------------------------
	if( pclCM->IsAlive(myid) == false)return 0;

	cltCharServer* pclchar = (cltCharServer*)pclCM->CR[myid];
	CMagicAttribute *pMagicAttributePtr = pclMagicManager->GetMagicAttributePtr();
	if ( pMagicAttributePtr->GetMagicGroup(pcldamageinfo->siMagicKind) == MAGIC_GROUP_DRAIN )
		return 0;

	//--------------------------------------------------------------------
	// 사용자가 아니고 소환수가 아니면 경험치를 받을 수 없다. 
	//--------------------------------------------------------------------
	if(IsPC(pclchar->GetCharUnique()) == false && pclchar->clIdentity.siIdentity != IDENTITY_PERSON_SUMMON)return 0;

	//--------------------------------------------------------------------
	// Clone이 아닌 놈에게서만 경험치를 받을수 있다.
	//--------------------------------------------------------------------
	if(pclCM->CR[enemyid]->IsAtb(ATB_CLONE)&& pclCM->CR[enemyid]->clInnerStatus.IsStatus(INNERSTATUS_CLONE) == true) return 0;


	//[진성] 자이언트 라쿤 부활 축제 이벤트 관련해서 생성된 몬스터는 경험치를 주지 않는다.
	if( IDENTITY_GIANTRACCOONRESURRECTEVENT == pclCM->CR[enemyid]->clIdentity.siIdentity )
	{
		return 0;
	}

	//--------------------------------------------------------------------
	// myid가 enemyid를 공격해서 damage를 입혔을 경우의 경험치를 구한다. 
	//-------------------------------------------------------------------
	SI32 exp = GetExp(myid, enemyid, pcldamageinfo->siDamage);
	if(exp <= 0)return 0;

	//-------------------------------------------------------------------
	// 주소지 마을의 레벨에 따라 경험치가 가중된다. 
	//-------------------------------------------------------------------
	SI32 siExpBonus = 0;

	if(pclCM->CR[myid]->pclCI->clBI.siHomeVillage)
	{
		cltStrInfo* pclstr = pclCityHallManager->GetStrInfo( pclCM->CR[myid]->pclCI->clBI.siHomeVillage );
		if(pclstr)
		{
			SI32 villagelevel = min(50, pclstr->clCityhallStrInfo.siVillageLevel / 2);

			if(villagelevel)
			{
				//exp = exp * (100 + villagelevel) / 100;
				siExpBonus += villagelevel;
			}
		}
	}


	//-------------------------------------------------------------------
	// myid가 파티 중이면 파티에 기여한 바를 기록한다. 
	//-------------------------------------------------------------------
	// [지연] 소환수는 파티 정보, 퀘스트 정보가 없다. 파티 퀘스트 관련 처리는 소환한 캐릭터를 가지고 한다.
	if ( pclchar->clIdentity.siIdentity == IDENTITY_PERSON_SUMMON )
	{
		SI32 siParentID = pclchar->GetSummonParentID();
		cltCharServer* pRealCharacter = (cltCharServer*)pclCM->CR[siParentID];
		if ( NULL != pRealCharacter )
		{
			if( pRealCharacter->siPartyIndex > 0 )
			{
				pclPartyMgr->IncreaseExp(pRealCharacter->siPartyIndex, pRealCharacter->pclCI->GetPersonID(), exp);

				//-------------------------------------------------------------------
				// myid가 파티 사냥을 해보라는 퀘스트라면 퀘스트를 클리어 시킨다.
				//-------------------------------------------------------------------
				pRealCharacter->DoQuestCheckAndAction(QUEST_CLEAR_COND_PARTYHUNTING);

				SI16 siGetType		= 0;
				SI16 siGetUnique	= 0;
				SI16 siGetPara1		= 0;
				SI16 siGetPara2		= 0;

				// QUEST_CLEAR_COND_TWOWAYKILLMONBONUS컨디션의 몬스터를 잡을때 해당 파티원중 같은 컨디션을 하고 수행하고 있는 파티원이 2명이상이라면
				if ( pRealCharacter->pclCI->clQuestInfo.IsQuestGoing(QUEST_CLEAR_COND_TWOWAYKILLMONBONUS, &siGetType, &siGetUnique, &siGetPara1, &siGetPara2) )
				{
					SI16 siEnemyKind = pclCM->CR[enemyid]->GetKind();
					if ( (siGetPara1) && (siEnemyKind == siGetPara1) && (2 <= pclPartyMgr->CountQuestPalyUserNum(pRealCharacter->siPartyIndex, QUEST_CLEAR_COND_TWOWAYKILLMONBONUS)) )
					{
						siExpBonus += 20;
					}
				}
			}
		}
	}
	
	else if(pclchar->siPartyIndex > 0)
	{
		pclPartyMgr->IncreaseExp(pclchar->siPartyIndex, pclchar->pclCI->GetPersonID(), exp);

		//-------------------------------------------------------------------
		// myid가 파티 사냥을 해보라는 퀘스트라면 퀘스트를 클리어 시킨다.
		//-------------------------------------------------------------------
		pclchar->DoQuestCheckAndAction(QUEST_CLEAR_COND_PARTYHUNTING);

		SI16 siGetType		= 0;
		SI16 siGetUnique	= 0;
		SI16 siGetPara1		= 0;
		SI16 siGetPara2		= 0;

		// QUEST_CLEAR_COND_TWOWAYKILLMONBONUS컨디션의 몬스터를 잡을때 해당 파티원중 같은 컨디션을 하고 수행하고 있는 파티원이 2명이상이라면
		if ( pclchar->pclCI->clQuestInfo.IsQuestGoing(QUEST_CLEAR_COND_TWOWAYKILLMONBONUS, &siGetType, &siGetUnique, &siGetPara1, &siGetPara2) )
		{
			SI16 siEnemyKind = pclCM->CR[enemyid]->GetKind();
			if ( (siGetPara1) && (siEnemyKind == siGetPara1) && (2 <= pclPartyMgr->CountQuestPalyUserNum(pclchar->siPartyIndex, QUEST_CLEAR_COND_TWOWAYKILLMONBONUS)) )
			{
				siExpBonus += 20;
			}
		}
	}

	if ( 0 < siExpBonus )
	{
		exp += ((exp * siExpBonus) / 100);
	}


	//-----------------------------------------------------------------
	// enemyid가 사망시에는. 
	//-----------------------------------------------------------------
	if(pclCM->IsAlive(enemyid) == FALSE)
	{
		//---------------------------------------------------------------------
		// 공격자가 수렵장에 있다면 수렵장 행수에게도 경험치를 일부 준다. (3%)
		//---------------------------------------------------------------------
		SI32 huntvillageunique = pclchar->GetHuntVillageUnique();
		if(huntvillageunique)
		{
			// 수렵장에서 사용된 소환인형의통계를 낸다. 
			pclItemManager->AddUseNum( pclKindInfoSet->pclKI[ pclCM->CR[enemyid]->GetKind() ]->siDollUnique, 1);

			// 새로운 통계
			//NStatistics_Add("Item", pclKindInfoSet->pclKI[ pclCM->CR[enemyid]->GetKind() ]->siDollUnique, "UseNum", 1);


			if(rand() % 100 == 0)
			{

				cltHunt* pclhunt = (cltHunt*)pclHuntManager->pclStruct[huntvillageunique];
				if(pclhunt)
				{
					pclhunt->clStrInfo.clHuntStrInfo.siChiefExp += exp;		// 경험치를 누적시킨다. 
				}
			}
		}

		//--------------------------
		// 기술 경험치를 준다. 
		//--------------------------
		SI32 skillexp = 0;
		SI32 skillunique = pMagicAttributePtr->GetMagicSkillType(pcldamageinfo->siMagicKind);
		
		// [진성] 스킬북 획득 마법일 경우 스킬 정보를 얻어준다.
		if( SKILL_SIKLLBOOK == skillunique && IsPC(pclchar->GetID())  )
		{
			cltSkillBookMagic*	pclSkillBookMagic = pclchar->pclCharSkillBookInfo->GetSkillBookMagic( pclchar->GetAttackType() );	
			if( NULL == pclSkillBookMagic ) return 0;

			skillunique = pclSkillBookMagic->GetSkillUnique();
		}

		if(skillunique <= 0)
		{
			if((rand() % 3) == 0)
			{
				switch(pcldamageinfo->siAttackType)
				{
				case ATTACKTYPE_SWORD:		skillunique = SKILL_USESWORD1;				break;
				case ATTACKTYPE_SPEAR:		skillunique = SKILL_USESPEAR1;				break;
				case ATTACKTYPE_AXE:		skillunique = SKILL_USEAXE1;				break;
				case ATTACKTYPE_STAFF:
				case ATTACKTYPE_STAFF1:
				case ATTACKTYPE_STAFF2:			//skillunique = SKILL_USESTAFF1;				break;
				case ATTACKTYPE_NEW_STAFF2: skillunique = SKILL_USESTAFF1;				break;
				case ATTACKTYPE_BOW:
				case ATTACKTYPE_BOW1:		//skillunique = SKILL_USEBOW1;				break;
				case ATTACKTYPE_NEW_BOW1:	skillunique = SKILL_USEBOW1;				break;
				case ATTACKTYPE_GUN:
				case ATTACKTYPE_GUN1:		//skillunique = SKILL_USEGUN1;				break;
				case ATTACKTYPE_NEW_GUN1:	skillunique = SKILL_USEGUN1;				break;
				default:
					{
						// 원거리 공격 변신인경우, bullet 으로 공격하는 경우만 처리한다.
						// bullet 의 AttackType을 사용하기때문에, 다음과 같이 캐릭터의 무기타입으로 처리한다.
						// 변신 중에는 무기를 변경할 수 없기 때문에 캐릭터의 무기타입으로 처리해도 문제 없다.
						SI32 siBulletUnique = pclAttackTypeInfoManager->GetBulletUnique(pcldamageinfo->siAttackType);
						if( pclchar->clTransFormInfo.bTransFormMode &&
							siBulletUnique > 0 )
						{
							switch( pclchar->GetAttackType() )
							{
								//case ATTACKTYPE_SWORD:	skillunique = SKILL_USESWORD1;				break;
								//case ATTACKTYPE_SPEAR:	skillunique = SKILL_USESPEAR1;				break;
								//case ATTACKTYPE_AXE:	skillunique = SKILL_USEAXE1;				break;
							case ATTACKTYPE_STAFF:
							case ATTACKTYPE_STAFF1:
							case ATTACKTYPE_STAFF2:	//skillunique = SKILL_USESTAFF1;				break;
							case ATTACKTYPE_NEW_STAFF2: skillunique = SKILL_USESTAFF1;				break;
							case ATTACKTYPE_BOW:
							case ATTACKTYPE_BOW1: //skillunique = SKILL_USEBOW1;				break;
							case ATTACKTYPE_NEW_BOW1:skillunique = SKILL_USEBOW1;				break;
							case ATTACKTYPE_GUN:
							case ATTACKTYPE_GUN1: //skillunique = SKILL_USEGUN1;				break;
							case ATTACKTYPE_NEW_GUN1:skillunique = SKILL_USEGUN1;				break;
							}
						}
					}
					break;
				}

				skillexp = exp /	2;
			}
		}
		else
		{
			skillexp = exp /	10;
		}

		// 피로방지 적용
		skillexp = pclchar->CalcTiredPoint( skillexp );


		if(skillexp > 0 && skillunique > 0 )
		{
			SI32 maxskilllevel	= pclSkillManager->GetMaxLevel(skillunique);
			if(skillunique >= 0)
			{
				cltCharServer* pclchar = (cltCharServer*)pclCM->CR[myid];		
				pclchar->SetSkillUpdate(skillunique, skillexp);
			}
		}



		//-----------------------------
		// 파티원이면 경험치 노나먹기
		//-----------------------------
		//if( pclchar->siPartyIndex )
		// [지연] 킬한 캐릭이 소환수일경우
		cltCharServer* pRealCharacter = NULL;
		if ( pclchar->clIdentity.siIdentity == IDENTITY_PERSON_SUMMON )
		{
			SI32 siParentID = pclchar->GetSummonParentID();
			cltCharServer* tmpChar = (cltCharServer*)pclCM->CR[siParentID];
			if ( NULL != tmpChar )
			{
				pRealCharacter = tmpChar;
			}
		}
		else
		{
			pRealCharacter = pclchar;
		}

		if( pRealCharacter->siPartyIndex )
		{
			CPartyObj* pclparty = pclPartyMgr->GetPartyObj(pRealCharacter->siPartyIndex);
			if(pclparty)
			{
				SI32 totallevel = 0;

				SI32 partyindex = 0;
				SI32 partylist[MAX_PARTY_USER_NUM];
				for(i = 0;i < MAX_PARTY_USER_NUM;i++)
				{
					partylist[i] = 0;
				}

				// 유효한 파티원의 리스트를 구한다. 
				stPartyUser* puser = NULL;
				for(i = 0;i < MAX_PARTY_USER_NUM;i++)
				{
					puser = pclparty->GetPartyUser(i);
					if(puser && puser->bPartyStatus == true)
					{
						if(pclCM->IsValidID(puser->siCharUnique) && 
							pclCM->CR[puser->siCharUnique]->pclCI->GetPersonID() == puser->siPersonID)
						{
							// 몬스터와 같은 지도, 같은 영역안에 있어야 한다. 그리고 살아 있어야 한다.
							if( pclCM->CR[enemyid]->GetMapIndex() == pclCM->CR[puser->siCharUnique]->GetMapIndex() &&
								max(TABS(pclCM->CR[enemyid]->GetX() - pclCM->CR[puser->siCharUnique]->GetX()), TABS(pclCM->CR[enemyid]->GetY() - pclCM->CR[puser->siCharUnique]->GetY())) < 30 &&
								pclCM->IsAlive(puser->siCharUnique) )								
							{
								//파티원과 20레벨 차이나면 경험치 줄 때 뺀다.일본 제외
								if(pclClient->IsCountrySwitch(Switch_20levelparty)){
									if( pRealCharacter->pclCI->clIP.GetLevel() - 20 <= puser->siLevel &&
										pRealCharacter->pclCI->clIP.GetLevel() + 20 >= puser->siLevel )
									{
										partylist[partyindex] = puser->siCharUnique;
										partyindex++;
									}
								}
								else{//일본은 그냥 적용
									partylist[partyindex] = puser->siCharUnique;
									partyindex++;
								}
							}
						}
					}
				}	

				WRITE_MEMLOG( 130, 2, 0, 0 );

				if(partyindex <= 0 || partyindex > MAX_PARTY_USER_NUM) return 0;

				// 경험치를 증가시킨다. (추가 1인당 10%) 
				if(partyindex > 1)
				{
					exp = exp + exp * (partyindex - 1) * 10 / 100;
				}

				// 레벨 합계를 구한다. 
				for(i = 0;i < partyindex;i++)
				{
					// 레벨 합계를 구한다. 
					totallevel += pclCM->CR[ partylist[i] ]->pclCI->clIP.GetLevel();
				}

				// 경험치를 배분한다. 
				if(totallevel)
				{
					for(i = 0;i < partyindex;i++)
					{
						SI32 tempexp		= 0;
						SI32 level			= pclCM->CR[ partylist[i] ]->pclCI->clIP.GetLevel();
						SI32 siEnemyKind	= pclCM->CR[enemyid]->GetKind();

						tempexp = exp * level / totallevel;

						//KHY - 1210 - 파티 경험치 최대값 레벨 *100 수정.
						// 경험치의 최대값은 level * 100 
						tempexp = min(level * 100, tempexp);

						cltCharServer* pclchar = (cltCharServer*)pclCM->CR[ partylist[i] ];

						// 파티원 각자의 경험치는 소환수나 소유주와 나눠 갖는다.
						if(pclchar)
						{
							IncreaseBothExp(partylist[i],  tempexp);

							pclchar->IncQuestPara( QUEST_CLEAR_COND_PARTYKILLMON, siEnemyKind, 1 );

							pclchar->IncQuestPara( QUEST_CLEAR_COND_TWOWAYKILLMON, siEnemyKind, 1 );
							pclchar->IncQuestPara( QUEST_CLEAR_COND_TWOWAYKILLMONBONUS, siEnemyKind, 1 );
						}
					}
				}

				WRITE_MEMLOG( 130, 3, 0, 0 );
			}
		}
		else	// 적이 죽었으나 파티원이 아니면 
		{
			IncreaseBothExp(myid,  exp);
		}
		// 가족관계
		// 얻은 경험치의 0.3%를 현제 접속한  자녀에게 준다.
		// 주는건 마리당 500이 최대 
		if( exp )
		{
			if( IsCountrySwitch( Switch_We_Are_Family ) )
			{
				if(pclchar->pclFamily != NULL )
				{
					if( pclchar->pclFamily->IsParent( pclchar->pclCI->GetPersonID() ) == MYCLASS_IN_FAMILY_PARANTS )// 내가 부모 일때.
					{
						if(  pclchar->pclFamily->GetOnChildrenNum() > 0  ) // 접속해있는 사람이 0이상
						{
							SI32 other_Add_Exp =  exp * FAMILY_ADD_CHILDREN_EXP	; // 자식들에게 줄 exp
							if( other_Add_Exp >= 500) 
							{
								other_Add_Exp = 500 ; 
							}
							if( other_Add_Exp <= 1) 
							{
								other_Add_Exp = 1 ; 
							}
							if ( other_Add_Exp > 0 )
							{
								for ( SI32 siSlot = 0; siSlot < MAX_CHILDREN_NUM ; siSlot++)
								{
									SI32 siChildren_PersonID = pclchar->pclFamily->GetChildPersonID( siSlot)	;
									SI32 siChildren_id = pclCM->GetIDFromPersonID( siChildren_PersonID )		;
									if(pclCM->IsValidID( siChildren_id ) == false )	continue					;
									cltCharServer* pclChildrenCharServer =  pclCM->GetCharServer( siChildren_id )	;
									pclChildrenCharServer->IncreaseExp( other_Add_Exp )	;
								}
							}
						}
					}
				}
			}


		}
	}
	else		// 적이 살아 있으면 
	{
		IncreaseBothExp(myid,  exp);
	}

	return exp;

}

SI32 cltServer::ExpSkillOperate( SI32 siMyID, SI32 siEnemyID, cltAttackDamageInfo* pclDamageInfo )
{
	// 공격하는 사람이 죽어 있다면 경험치를 받을 수 없다
	if ( FALSE == pclCM->IsAlive(siMyID) )
	{
		return 0;
	}

	// 공격 받는 적이 죽었을때만 스킬 경험치를 받을 수 있다
	if ( TRUE == pclCM->IsAlive(siEnemyID) )
	{
		return 0;
	}

	if ( NULL == pclDamageInfo )
	{
		return 0;
	}

	// 흡혈일경우 스킬경험치를 주지 않는다
	CMagicAttribute* pclMagicAttributePtr = pclMagicManager->GetMagicAttributePtr();
	if ( (NULL == pclMagicAttributePtr) || (MAGIC_GROUP_DRAIN == pclMagicAttributePtr->GetMagicGroup(pclDamageInfo->siMagicKind)) )
	{
		return 0;
	}

	cltCharServer* pclMyChar = pclCM->GetCharServer( siMyID );
	if ( NULL == pclMyChar )
	{
		return 0;
	}

	// 유저가 아니거나 소환수가 아니면 경험치를 못 얻는다
	if ( (false == IsPC(pclMyChar->GetCharUnique())) && (IDENTITY_PERSON_SUMMON != pclMyChar->clIdentity.siIdentity) )
	{
		return 0;
	}

	cltCharServer* pclEnemyChar = pclCM->GetCharServer( siEnemyID );
	if ( NULL == pclEnemyChar )
	{
		return 0;
	}

	// 클론인 적에게서는 경험치를 받을 수 없다
	if ( (pclEnemyChar->IsAtb(ATB_CLONE)) && (pclEnemyChar->clInnerStatus.IsStatus(INNERSTATUS_CLONE)) )
	{
		return 0;
	}

	//[진성] 자이언트 라쿤 부활 축제 이벤트 관련해서 생성된 몬스터는 경험치를 주지 않는다.
	if( IDENTITY_GIANTRACCOONRESURRECTEVENT == pclEnemyChar->clIdentity.siIdentity )
	{
		return 0;
	}

	// 일단 스킬 경험치를 구하기전 시드값인 캐릭터 경험치를 구한다
	SI32 siGetExp = GetExp( siMyID, siEnemyID, pclDamageInfo->siDamage );
	if ( 0 >= siGetExp )
	{
		return 0;
	}

	SI32 siExpBonus = 0;
	if ( 0 < pclMyChar->pclCI->clBI.siHomeVillage )
	{
		cltStrInfo* pclGetStrInfo = pclCityHallManager->GetStrInfo( pclMyChar->pclCI->clBI.siHomeVillage );
		if ( pclGetStrInfo )
		{
			SI32 siCalcVillageLevel = min( 50, pclGetStrInfo->clCityhallStrInfo.siVillageLevel/2 );
			if ( 0 < siCalcVillageLevel )
			{
				siExpBonus += siCalcVillageLevel;
			}
		}
	}

	if ( 0 < pclMyChar->siPartyIndex )
	{
		SI16 siGetType		= 0;
		SI16 siGetUnique	= 0;
		SI16 siGetPara1		= 0;
		SI16 siGetPara2		= 0;

		// QUEST_CLEAR_COND_TWOWAYKILLMONBONUS컨디션의 몬스터를 잡을때 해당 파티원중 같은 컨디션을 하고 수행하고 있는 파티원이 2명이상이라면
		if ( pclMyChar->pclCI->clQuestInfo.IsQuestGoing(QUEST_CLEAR_COND_TWOWAYKILLMONBONUS, &siGetType, &siGetUnique, &siGetPara1, &siGetPara2) )
		{
			if ( (siGetPara1) && (pclEnemyChar->GetKind() == siGetPara1) && (2 <= pclPartyMgr->CountQuestPalyUserNum(pclMyChar->siPartyIndex, QUEST_CLEAR_COND_TWOWAYKILLMONBONUS)) )
			{
				siExpBonus += 20;
			}
		}
	}

	// 보너스 포인트를 계산한다
	if ( 0 < siExpBonus )
	{
		siGetExp += ((siGetExp * siExpBonus) / 100);
	}

	// 실제 기술 경험치를 준다
	SI32 siSkillExp		= 0;
	SI32 siSkillType	= pclMagicAttributePtr->GetMagicSkillType( pclDamageInfo->siMagicKind );
	SI32 siSkillUnique	= 0;

	// 스킬 타입이 없을때
	if ( 0 >= siSkillType )
	{
		// 3분의 1확률로
		if ( (rand() % 3) == 0 )
		{
			switch ( pclDamageInfo->siAttackType )
			{
			// 검
			case ATTACKTYPE_SWORD:		siSkillUnique = SKILL_USESWORD1;	break;
			// 창
			case ATTACKTYPE_SPEAR:		siSkillUnique = SKILL_USESPEAR1;	break;
			// 도끼
			case ATTACKTYPE_AXE:		siSkillUnique = SKILL_USEAXE1;		break;
			// 지팡이
			case ATTACKTYPE_STAFF:
			case ATTACKTYPE_STAFF1:
			case ATTACKTYPE_STAFF2:
			case ATTACKTYPE_NEW_STAFF2: siSkillUnique = SKILL_USESTAFF1;	break;
			// 활
			case ATTACKTYPE_BOW:
			case ATTACKTYPE_BOW1:
			case ATTACKTYPE_NEW_BOW1:	siSkillUnique = SKILL_USEBOW1;		break;
			// 총
			case ATTACKTYPE_GUN:
			case ATTACKTYPE_GUN1:
			case ATTACKTYPE_NEW_GUN1:	siSkillUnique = SKILL_USEGUN1;		break;
			default:
				{
					// 원거리 공격 변신인경우, bullet 으로 공격하는 경우만 처리한다.
					// bullet 의 AttackType을 사용하기때문에, 다음과 같이 캐릭터의 무기타입으로 처리한다.
					// 변신 중에는 무기를 변경할 수 없기 때문에 캐릭터의 무기타입으로 처리해도 문제 없다.
					SI32 siBulletUnique = pclAttackTypeInfoManager->GetBulletUnique(pclDamageInfo->siAttackType);
					if ( (0 < siBulletUnique) && (pclMyChar->clTransFormInfo.bTransFormMode ) )
					{
						// 원거리 무기만 해당
						switch( pclMyChar->GetAttackType() )
						{
						// 지팡이
						case ATTACKTYPE_STAFF:
						case ATTACKTYPE_STAFF1:
						case ATTACKTYPE_STAFF2:	
						case ATTACKTYPE_NEW_STAFF2:		siSkillUnique = SKILL_USESTAFF1;	break;
						// 활
						case ATTACKTYPE_BOW:
						case ATTACKTYPE_BOW1:
						case ATTACKTYPE_NEW_BOW1:		siSkillUnique = SKILL_USEBOW1;		break;
						// 총
						case ATTACKTYPE_GUN:
						case ATTACKTYPE_GUN1:
						case ATTACKTYPE_NEW_GUN1:		siSkillUnique = SKILL_USEGUN1;		break;
						}
					}
				}
				break;
			}

			siSkillExp = siGetExp / 2;
		}
	}
	else
	{
		siSkillExp = siGetExp / 10;
	}

	// 피로방지 적용
	siSkillExp = pclMyChar->CalcTiredPoint( siSkillExp );

	if ( (0 < siSkillExp) && (0 < siSkillUnique) )
	{
		pclMyChar->SetSkillUpdate( siSkillUnique, siSkillExp );
	}

	return siSkillExp;
}

#define GIVEEXP_EACHOTHER	30

void cltServer::IncreaseBothExp(SI32 siID, SI32 siExp)
{

	if(pclCM->IsValidID(siID) == FALSE) return;
	cltCharServer* pclchar = (cltCharServer*)pclCM->CR[siID];

	SI32 mapindex = pclchar->GetMapIndex();

	// PK 가능 사냥터 일경우 경험치 추가 적용.
	if( pclClient->pclMapManager->IsPkMap( mapindex ) )
	{
		siExp = siExp * siPKMapExpRate / 100;
	}

	// 피로방지 시스템 적용
	siExp = pclchar->CalcTiredPoint( siExp );

	// 길드가 소유한 던전일경우

	if( pclClient->pclMapManager->IsGuildMap( mapindex )  )
	{
		pclGuildDungeonMgr->AddExp( mapindex, siExp );
	}



	SI32 siMyExp    = siExp;
	SI32 siOtherExp = 0;

	//=========================================================================
	// 사람이 몬스터를 죽여 경험치를 얻을때 
	//=========================================================================
	// 내가 소환수를 갖고 있으면 내가 얻은 경험치를 소환수와 나누어 갖는다.
	//=========================================================================
	SI32 siSummonID = pclchar->GetSummonID();
	if(pclCM->IsAlive(siSummonID) == TRUE)
	{
		cltCharServer* pclSummonChar = (cltCharServer*)pclCM->CR[siSummonID];

		SI32 summonlevel	= pclSummonChar->pclCI->clIP.GetLevel();
		SI32 parentlevel	= pclchar->pclCI->clIP.GetLevel();

		SI32 siTotalLevel = max(1, summonlevel + parentlevel);
		if(summonlevel + parentlevel <= 0)
		{
			pclLog->FilePrint(TEXT("Config\\ErrorLog.log"), TEXT("fdieff:summonlevel:%d parentlevel:%d"), summonlevel, parentlevel);
		}


		// 부모의 소환술이 있으면 경험치를 더 얻는다. 
		SI32 skilllevel = pclchar->pclCI->clCharSkill.GetTotalSkillLevel( SKILL_SUMMON, 0, 0, &pclchar->pclCI->clCharItem.clItem[0]);

		//=========================================================================
		// 진화시키지 않을경우 소환술의 패널티 
		//=========================================================================
		SI32 siSkillModfier = 10;
		SI32 siCurrSummonKind  = pclSummonChar->GetKind();
		if(pclKindInfoSet->IsValidKind(siCurrSummonKind) == true)
		{
			if(pclSummonChar->pclKI->bSummonMall == TRUE)
			{
				siSkillModfier = 10;
			}
			else if(summonlevel	> pclSummonChar->pclKI->siEvolveLevel) // 진화시기가 지나면 
			{
				if(summonlevel		> pclSummonChar->pclKI->siEvolveLevel * 2)		siSkillModfier = 20;
				else if(summonlevel > pclSummonChar->pclKI->siEvolveLevel * 1.5)	siSkillModfier = 15;
				else																siSkillModfier = 10;
			}
		}

		//=========================================================================
		// 소환수가 얻는 경험치를 계산한다.
		//=========================================================================
		siOtherExp	= (SI32)(siExp  * GIVEEXP_EACHOTHER * 0.01f);
		pclchar->SetSkillUpdate(SKILL_SUMMON,  max( siOtherExp / siSkillModfier , 1 ) );	// 소환술 경험치를 추가한다. (최소 1을 보장)
		siOtherExp += (siOtherExp * skilllevel / 100);						// 소환술의 능력치만큼 경험치를 추가한다.
		if ( bSpecailDay7switch ) // 충무공 탄신일에는 소환영웅의 경험치가 2배
		{
			if ( pclCM->CR[siSummonID]->pclKI->bSummonMall == TRUE )
			{
				siOtherExp = siOtherExp*2 ;
			}
		}
		if(siOtherExp > 0)	pclSummonChar->IncreaseExp(siOtherExp);

		// 내가 얻을 경험치 
		siMyExp    = siExp;			// 이제는 잡은 놈은 그대로 다 준다.		

		//===========================================================
		// 5%의 확률로 나머지 소환수에게도 경험치를 똑같이 준다.
		//===========================================================
		if(siOtherExp  > 0 && rand()%20 == 0)
		{
			for(SI32 i = 0; i < MAX_SUMMON_OWN; ++i)
			{
				// 나머지 소환수에게만 경험치를 준다.
				if(i != pclSummonChar->GetSummonIndex())
				{
					cltSummon *pclSummon = pclchar->pclCI->clSummonInfo.GetSummon(i);
					if(pclSummon && pclSummon->siKind)
					{
						pclSummon->clIP.IncreaseExp(siOtherExp); 
						cltGameMsgResponse_Summon clInfo(pclchar->GetCharUnique(), i, pclSummon);
						cltMsg clMsg(GAMEMSG_RESPONSE_SUMMONINFO, sizeof(clInfo), (BYTE*)&clInfo);
						pclchar->SendNetMsg((sPacketHeader*)&clMsg);
					}
				}
			}
		}

	}

	//=========================================================================
	// 소환수가 몬스터를 죽여 경험치를 얻을때 
	//=========================================================================
	// 내가 소환수라면 내가 얻은 경험치를 나누어 갖는다.
	//=========================================================================
	SI32 siParentID = pclchar->GetSummonParentID();
	if(pclCM->IsAlive(siParentID) == TRUE)
	{
		cltCharServer* pclParentChar = (cltCharServer*)pclCM->CR[siParentID];

		SI32 summonlevel	= pclchar->pclCI->clIP.GetLevel();
		SI32 parentlevel	= pclParentChar->pclCI->clIP.GetLevel();

		SI32 siTotalLevel = max(1, summonlevel + parentlevel);
		if(summonlevel + parentlevel <= 0)
		{
			pclLog->FilePrint(TEXT("Config\\ErrorLog.log"), TEXT("f123dieff:summonlevel:%d parentlevel:%d"), summonlevel, parentlevel);
		}

		// 부모의 소환술이 있으면 경험치를 더 얻는다. 
		SI32 skilllevel = pclParentChar->pclCI->clCharSkill.GetTotalSkillLevel( SKILL_SUMMON, 0, 0, &pclParentChar->pclCI->clCharItem.clItem[0]);

		//=========================================================================
		// 내가 얻는 경험치를 계산한다.
		//=========================================================================
		siOtherExp	= (SI32)(siExp * GIVEEXP_EACHOTHER * 0.01f );
		if(siOtherExp > 0) pclParentChar->IncreaseExp(siOtherExp);

		siMyExp	 =	siExp;										// 소환수가 잡으면 소환수는 그대로 경험치를 얻는다.

		//=========================================================================
		// 진화시키지 않을경우 소환술의 패널티 
		//=========================================================================

		SI32 siSkillModfier = 10;
		SI32 siCurrSummonKind  = pclchar->GetKind();
		if(pclKindInfoSet->IsValidKind(siCurrSummonKind) == true)
		{
			//cltKindInfo* pclki = pclKindInfoSet->pclKI[siCurrSummonKind];
			cltKindInfo* pclki = NULL;
			if(pclClient->IsCountrySwitch(Switch_Summon_Separate)) 
			{
				pclki = pclClient->pclKindInfoSet->pclSummonKI[siCurrSummonKind];
			}
			else
			{
				pclki = pclClient->pclKindInfoSet->pclKI[siCurrSummonKind];
			}

			if(pclki && pclki->bSummonMall == TRUE)
			{
				siSkillModfier = 10;
			}
			else if(pclki && summonlevel > pclki->siEvolveLevel &&
				pclKindInfoSet->IsSummonFromRank(pclki->siSummonRank+1) == true ) // 진화시기가 지나면 
			{
				if(summonlevel > pclki->siEvolveLevel * 2)			siSkillModfier = 30;
				else if(summonlevel > pclki->siEvolveLevel * 1.5)	siSkillModfier = 25;
				else												siSkillModfier = 20;
			}
		}

		pclParentChar->SetSkillUpdate(SKILL_SUMMON, siMyExp / siSkillModfier);	// 부모에게 소환술 경험치를 추가한다. 
		siMyExp += (siMyExp * skilllevel / 100);					// 부모의 소환술에따라 더받는다.

		//===========================================================
		// 10%의 확률로 나머지 소환수에게도 경험치를 똑같이 준다.
		//===========================================================
		if(siMyExp > 0 && rand()%10 == 0)
		{
			for(SI32 i = 0; i < MAX_SUMMON_OWN; ++i)
			{
				// 나머지 소환수에게만 경험치를 준다.
				if(i != pclchar->GetSummonIndex())
				{
					cltSummon *pclSummon = pclParentChar->pclCI->clSummonInfo.GetSummon(i);
					if(pclSummon && pclSummon->siKind && pclSummon->siStatus != SUMMON_STATUS_DEAD )
					{
						pclSummon->clIP.IncreaseExp(siMyExp); 
						cltGameMsgResponse_Summon clInfo(pclParentChar->GetCharUnique(), i, pclSummon);
						cltMsg clMsg(GAMEMSG_RESPONSE_SUMMONINFO, sizeof(clInfo), (BYTE*)&clInfo);
						pclParentChar->SendNetMsg((sPacketHeader*)&clMsg);
					}
				}
			}
		}		
	}
	if(siMyExp > 0) pclchar->IncreaseExp(siMyExp);
}


void cltServer::PartyWarItemOperate(SI32 myid, SI32 enemyid, CPartyObj* pPartyObj)
{	//[영진]

	WRITE_MEMLOG( 130, 4, 0, 0 );

	SI16 i=0, j=0;
	SI32 itemfrompos=0;
	cltItem clfromitem;

	if(pclCM->IsValidID(myid) == false) return;
	if(pclCM->IsValidID(enemyid) == false) return;

	// 파티가 없다면 끝낸다.
	if(pPartyObj == NULL) return;

	//  파티를 한 PC만 전리품을 빼앗을 수 있다. ( 소환수였다면 주인의 아이디를 인자로 받아오기때문에.
	if(IsPC(pclCM->CR[myid]->GetCharUnique()) == false )
	{
		return ;
	}

	// NPC에서만 아이템을 빼앗는다. 
	if(IsPC(pclCM->CR[enemyid]->GetCharUnique()) )
		return ;

	// Clone이 아닌 놈에게서만 아이팀을 빼앗는다.
	if(pclCM->CR[enemyid]->IsAtb(ATB_CLONE)&& pclCM->CR[enemyid]->clInnerStatus.IsStatus(INNERSTATUS_CLONE) == true)
		return;

	// 랜덤으로 아이템 하나 빼온다
	if(pclCM->CR[enemyid]->pclCI->clCharItem.RobInv(&itemfrompos, &clfromitem) == true)
	{
		SI32 totalLuck = 0;
		SI32 totalBattlePower = 0;
		SI32 totalLevel = 0;
		SI32 partyindex = 0;
		SI32 partylist[MAX_PARTY_USER_NUM];

		for(i = 0;i < MAX_PARTY_USER_NUM;i++)
		{
			partylist[i] = 0;
		}
		//아이템을 노나먹을 유효한 파티원의 CharUnique 리스트를 구한다. 
		stPartyUser* puser = NULL;
		for(i = 0;i < MAX_PARTY_USER_NUM;i++)
		{
			puser = pPartyObj->GetPartyUser(i);
			if(puser && puser->bPartyStatus == true)
			{
				if(pclCM->IsValidID(puser->siCharUnique) && pclCM->CR[puser->siCharUnique]->pclCI->GetPersonID() == puser->siPersonID)
				{
					// 몬스터와 같은 지도, 같은 영역안에 있어야 한다. 
					if(pclCM->CR[enemyid]->GetMapIndex() == pclCM->CR[puser->siCharUnique]->GetMapIndex()
						&& max(TABS(pclCM->CR[enemyid]->GetX() - pclCM->CR[puser->siCharUnique]->GetX()), TABS(pclCM->CR[enemyid]->GetY() - pclCM->CR[puser->siCharUnique]->GetY())) < 30
						&& pclCM->IsAlive(puser->siCharUnique ))
					{
						partylist[partyindex] = puser->siCharUnique;
						partyindex++;
					}
				}
			}
		}	

		if(partyindex <= 0 || partyindex > MAX_PARTY_USER_NUM) return;


		for(i = 0;i < partyindex;i++)
		{
			// 럭 합계를 구한다. 
			totalLuck += pclCM->CR[ partylist[i] ]->pclCI->clIP.siLuk;			
			// 전투력 합계를 구한다.
			totalBattlePower += pclCM->CR[ partylist[i] ]->clPB.GetBattlePower();

			totalLevel += pclCM->CR[ partylist[i] ]->pclCI->clIP.GetLevel();
			/*
			for(j = 0 ; j < MAX_SUMMON_OWN ; j++  )
			{
				cltSummon* pclSummon = pclCM->CR[ partylist[i] ]->clSummonInfo.GetSummon(j) 
					if( pclSummon != NULL && pclSummon->siKind > 0 && pclSummon->siKind < MAX_KIND_NUMBER )
					{


					}
			}
			*/
		}		


		//해당 맵에 있는 유효한 파티원의  Luk스테이터스 평균 적용해서 운좋으면 아이템 +1 개

		SI32 lukpercent = 0;
		SI32 maxlukpercent = (SI32)( totalLuck / partyindex *  0.1 / 3);

		if( maxlukpercent > 0) 
		{
			lukpercent = rand() % maxlukpercent + 1;

			if ( lukpercent > 15 )
				lukpercent = 15;

			if ( rand() % 100 + 1 <= lukpercent )
				clfromitem.siItemNum = clfromitem.siItemNum + 1;
		}




		// 전투력 평균에 의한 아이템 갯수 페널티	

		// 적의 전투력을 구한다. 
		SI32 enemybattle	= pclCM->CR[enemyid]->clPB.GetBattlePower();
		// 파티의 평균 전투력을 구한다. 
		SI32 attackerbattle	=  (SI32)( totalBattlePower / partyindex);
		// 파티의 평균 레벨을 구한다.
		SI32 level = (SI32)(totalLevel / partyindex);



		// 전투력 차이가 많이나면 아이템갯수를 조정한다. 

		// 평균 30 이하일때는 조정이 없다. 
		if( level > 30 )
		{
			SI32 percent = 0;

			if(attackerbattle > 0)
				percent = enemybattle * 100 / attackerbattle;
			else
				percent = 0;


			if(percent  < 40)		{	clfromitem.siItemNum = clfromitem.siItemNum - 3;	}

			else if( percent  < 70)	{	clfromitem.siItemNum = clfromitem.siItemNum - 2;	}


			if (clfromitem.siItemNum <= 0)
				clfromitem.siItemNum = 1;
		}

		// 수렵장에서 사냥한 것이라면, 
		SI32 huntvillageunique	= pclCM->CR[myid]->GetHuntVillageUnique();
		GMONEY huntmoney			= 0;
		if(huntvillageunique)
		{
			if(rand() % 5 == 0)		// 20% 확률로 수렵장에 지급한다. 
			{
				cltItemPriceUnit* pclprice = pclItemManager->GetCurrentPrice(clfromitem.siUnique);
				if(pclprice)
				{
					huntmoney = pclprice->siPrice * clfromitem.siItemNum;

					cltHunt* pclhunt = (cltHunt*)pclHuntManager->pclStruct[huntvillageunique];
					if(pclhunt)
					{
						pclhunt->clStrInfo.clHuntStrInfo.siTempMoney += huntmoney;
					}
				}
			}
		}

		// 인스턴스 던전에서 사냥한다면
		GMONEY instancemoney = 0 ;
		SI32 mapindex = pclCM->CR[myid]->GetMapIndex();

		WRITE_MEMLOG( 130, 5, 0, 0 );

		// 길드 소유가 가능한 던전에서 사냥한다면
		if( pclMapManager->IsGuildMap(mapindex)  )
		{
			cltItemPriceUnit* pclprice = pclItemManager->GetCurrentPrice(clfromitem.siUnique);
			if(pclprice)
			{
				GMONEY money = pclprice->siPrice * clfromitem.siItemNum;

				if ( money )	pclGuildDungeonMgr->AddMoney(mapindex, money);
			}
		}
		//------------------------------------------
		// 수렵장, 인던에 지급된 아이템이 아니면 캐릭터에게 지급한다. 
		//------------------------------------------
		if(huntmoney == 0 && instancemoney == 0) 
		{
			//줄 캐릭터를 설정한다

			SI32 waritemid = 0;
			SI32 getitemrate = 1000 ;

			// 찜되어 있는 상태면 찜한 주체에게 전리품을 준다. 
			if(pclCM->CR[enemyid]->clJim.siJimHostCharUnique)
			{
				SI32 jimid = pclCM->GetIDFromCharUnique( pclCM->CR[enemyid]->clJim.siJimHostCharUnique );
				if(jimid)// && pclCM->IsValidID(jimid))
				{
					if(pclCM->CR[jimid]->clJim.siJimSlaveCharUnique == pclCM->CR[enemyid]->GetCharUnique())
					{
						// 부모가 있다면 (jimid 가 소환수라면) //함수 호출전에 주인의 아이디로 가져오지만 혹시 모르니까!
						SI32 parentid = pclCM->CR[jimid]->GetSummonParentID();
						if(parentid)
						{
							SI32 levelgap = pclCM->CR[jimid]->pclKI->siSummonRank - pclCM->CR[enemyid]->pclKI->siSummonRank ;
							if ( pclCM->CR[enemyid]->pclKI->siSummonRank > 0 && levelgap > 17 &&
								pclCM->CR[jimid]->pclKI->bSummonMall != TRUE )
							{
								SI32 randrate = rand() % 1000 ;
								getitemrate = getitemrate - ( ( levelgap - 17 ) * 30 ) ;
								if ( randrate >= getitemrate )
								{
									return ;
								}
							}

							// 부모에게 전리품을 준다. 
							waritemid = parentid;
						}
						// 플레이어라면
						else
						{
							// 자신이 전리품을 갖는다. 
							waritemid = jimid;
						}
					}
				}
			}
			if(waritemid >0 )
			{	// 파티원중에 한명이 찜한것인지 체크하여 찜과 무관하게 파티의 룰에 따른다.
				for(SI16 j = 0 ; j < partyindex; j++)
				{
					if( partylist[j] == waritemid )
					{
						waritemid = 0;
						break;
					}
				}
			}
			// 찜한님이 없으면 아이템 획득 룰에 따라 파티원끼리 나눈다.
			if(waritemid == 0 || pclCM->IsValidID(waritemid) == false)
			{			
				SI16 itemGetPattern = pPartyObj->GetItemGetPattern();
				if(itemGetPattern == ITEM_GETPATTERN_NORMAL)
				{
					return; // error
				}
				else if(itemGetPattern == ITEM_GETPATTERN_ROTATION)
				{
					WRITE_MEMLOG( 130, 7, 0, 0 );
					// 내부에선 waritemid 가 아이템을 먹을것이라는 가정하에 다음차례로 획득순서가 넘어가있다.
					waritemid =	pclCM->GetIDFromCharUnique(pPartyObj->GetItemGetCharUnique(partylist,partyindex));
					if(waritemid <= 0) 
						waritemid = myid;
				}
				else if(itemGetPattern == ITEM_GETPATTERN_FINISH)
				{
					WRITE_MEMLOG( 130, 8, 0, 0 );
					waritemid = myid;
				}
				else if(itemGetPattern == ITEM_GETPATTERN_RANDOM)
				{
					WRITE_MEMLOG( 130, 9, 0, 0 );
					SI32 rndPartyIndex = rand()%partyindex;	

					if(rndPartyIndex < 0 || rndPartyIndex >= partyindex)
						rndPartyIndex = 0;

					waritemid =  partylist[rndPartyIndex] ;	
				}
				else
				{
					return; // error
				}
			}			

			if(pclCM->IsValidID(waritemid) == false) return;

			// 통계를 낸다. 
			//pclItemManager->AddWarItemNum(clfromitem.siUnique, clfromitem.siItemNum);

			cltCharServer* pclchar = (cltCharServer*)pclCM->CR[waritemid];

			if(pclchar == NULL) return;

			// temp - 아이템 드랍 두배
			// clfromitem.siItemNum *= 2;

			// 피로방지 시스템 적용
			clfromitem.siItemNum = pclchar->CalcTiredPoint( clfromitem.siItemNum );

			if ( pclCM->CR[waritemid]->pclCI->clBI.clBufInfo.IsHaveBuf( BUF_FOURLEAF) )  // 네잎클로버 상태라면 - 아이템을 2배로 받는다.
			{
				clfromitem.siItemNum = clfromitem.siItemNum * 2; // 2배를 해준다.
			}

			if ( clfromitem.siItemNum > 0 )
			{

				InputItem(pclchar, &clfromitem, INPUTITEMPERSON_OPTION_WAR);

				// 통계를 낸다. 
				pclItemManager->AddWarItemNum(clfromitem.siUnique, clfromitem.siItemNum);

				// 새로운 통계 - 전리품 아이템 통계를 낸다.
				NStatistics_Add("Item", clfromitem.siUnique, "MonNum", clfromitem.siItemNum);
				NStatistics_Set("Item", clfromitem.siUnique, "Name", pclItemManager->GetName(clfromitem.siUnique));
			}

			WRITE_MEMLOG( 130, 6, 0, 0 );
		}



	}// end if( npc 한테서 아이템 랜덤으로 얻어왔다면)

	//------------------------------------------------------
	// 수렵술이 연마되어 있다면, 소환인형을 얻는다. 
	//------------------------------------------------------
	// 수렵장이 아니어야 한다. 
	if( pclMapManager->IsHuntMap(pclCM->CR[myid]->GetMapIndex()) == false )
	{
		// 내가 소환수라면
		SI32 parentid = pclCM->CR[myid]->GetSummonParentID();
		SI32 siID = myid ;
		if(pclCM->IsValidID(parentid))
		{
			siID = parentid ;
		}

		SI32 captureskilllevel	= pclCM->CR[siID]->pclCI->clCharSkill.GetTotalSkillLevel(SKILL_CAPTURE, 0, 0,
			&pclCM->CR[siID]->pclCI->clCharItem.clItem[0]);
		if(captureskilllevel)
		{
			SI32 capturerate = pclCM->CR[enemyid]->pclKI->GetCaptureRate(captureskilllevel);
			if(rand() % 10000 < capturerate)
			{
				SI32 dollitemunique = 0;
				for(i = 0;i < MAX_ITEMINFO_NUMBER;i++)
				{
					if(pclItemManager->pclItemInfo[i] == NULL)continue;

					if(pclItemManager->pclItemInfo[i]->siMakeKind == pclCM->CR[enemyid]->GetKind())
					{
						dollitemunique = pclItemManager->pclItemInfo[i]->clItem.siUnique;
					}
				}

				if(dollitemunique)
				{
					// 통계를 낸다. 
					cltCharServer* pclchar = (cltCharServer*)pclCM->CR[siID];

					// 피로방지 시스템 적용
					// test
					SI32 dollnum	= pclchar->CalcTiredPoint( CAPTURE_DOLLNUM ); 

					pclItemManager->AddWarItemNum(dollitemunique, dollnum);


					//cyj pclchar->GiveItem(dollitemunique, dollnum, INPUTITEMPERSON_OPTION_WAR/*INPUTITEMPERSON_OPTION_NONE*/,0);
					SI32 ref = pclClient->pclItemManager->FindItemRefFromUnique(dollitemunique);
					UI16 usedate = pclClient->pclItemManager->pclItemInfo[ref]->clItem.uiDateUseDay;
					pclchar->GiveItem(dollitemunique, dollnum, INPUTITEMPERSON_OPTION_WAR/*INPUTITEMPERSON_OPTION_NONE*/,usedate);

					SI32 skillunique	= SKILL_CAPTURE;
					SI32 maxskilllevel	= pclSkillManager->GetMaxLevel(skillunique);
					if(skillunique >= 0)
					{
						// 기술 경험치량을 구한다. 
						SI32 skillexp = max(1, pclCM->CR[enemyid]->pclKI->Exp / 5);

						// 피로방지 시스템 적용
						pclchar->CalcTiredPoint( skillexp );
						pclchar->SetSkillUpdate( skillunique, skillexp );
					}

				}
			}
		}
	}

	// 인스턴스 던전의 몬스터를 죽인다면
	if( pclMapManager->IsInstanceMap(pclCM->CR[myid]->GetMapIndex()) == true )
	{
		SI32 mapindex = pclCM->CR[myid]->GetMapIndex() ;
	}
}


//myid가 enemyid를 죽여서 얻게 되는 전리품 처리. 
void cltServer::WarItemOperate(SI32 myid, SI32 enemyid) 
{
	SI32 i;

	SI32 itemfrompos;
	cltItem clfromitem;

	// PC, 소환수만 전리품을 빼앗을 수 있다. 
	if(IsPC(pclCM->CR[myid]->GetCharUnique()) == false && pclCM->CR[myid]->GetSummonParentID() == 0)
	{
		return ;
	}

	// NPC에서만 아이템을 빼앗는다. 
	if(IsPC(pclCM->CR[enemyid]->GetCharUnique()) )return ;

	// Clone이 아닌 놈에게서만 아이팀을 빼앗는다.
	if(pclCM->CR[enemyid]->IsAtb(ATB_CLONE)&& pclCM->CR[enemyid]->clInnerStatus.IsStatus(INNERSTATUS_CLONE) == true) return;

	//[지연] - 2010신년이벤트 : 사냥후 1% 확률로 1개의 포춘쿠키 조각 획득
	((cltCharServer*)(pclCM->CR[myid]))->GiveFortuneCookiePiece(10, 1);
	
	
	// 사냥을 통해 퀘스트 아이템을 획득 합니다.
	((cltCharServer*)(pclCM->CR[myid]))->HuntQuestitem(myid, pclCM->CR[enemyid]->GetKind());

	if(pclCM->CR[enemyid]->pclCI->clCharItem.RobInv(&itemfrompos, &clfromitem) == true)
	{
		//=================================================================
		//cyj 드롭아이템갯수 +1 에 Luk스테이터스 적용
		//=================================================================
		//SI32 maxlukpercent = (SI32)(pclCM->CR[myid]->pclCI->clIP.GetLuk() *  0.1 / 3);
		SI32 maxlukpercent = (SI32)(pclCM->CR[myid]->clPB.clTotalBA.GetLuk() *  0.1 / 3);

		if( maxlukpercent > 0) 
		{
			SI32 lukpercent = rand() % maxlukpercent + 1;

			if (lukpercent > 15 )
				lukpercent = 15;

			if (rand() % 100 + 1 <= lukpercent )
				clfromitem.siItemNum = clfromitem.siItemNum + 1;
		}

		//--------------------------------------------------
		// 수호 정령에 의한 아이템 드랍겟수 증가
		//--------------------------------------------------
		if ( IsCountrySwitch(Switch_SoulGuard) )
		{
			SI32 siAddItemNum = ((cltCharServer*)pclCM->CR[myid])->SoulGuardAbilityActivity( SOULGUARD_ABILITY_DROPITEM );
			if ( 0 < siAddItemNum )
			{
				clfromitem.siItemNum += siAddItemNum;
			}
		}

		// 전투력에 의한 아이템 갯수 페널티

		// 적의 전투력을 구한다. 
		SI32 enemybattle	= pclCM->CR[enemyid]->clPB.GetBattlePower();

		// 공격자의 전투력을 구한다. 
		SI32 attackerbattle	= pclCM->CR[myid]->clPB.GetBattlePower();

		// 사용자이고 (소환수는 이런 적용을 받지 않는다.) 
		// 내 레벨이 30 이하일때는 조정이 없다. 
		// 전투력 차이가 많이나면 아이템갯수를 조정한다. 
		if(IsPC(myid) && pclCM->CR[myid]->pclCI->clIP.GetLevel() > 30 && attackerbattle >= 0)
		{
			SI32 percent = 0;
			if(attackerbattle>0)	
				percent = enemybattle * 100 / attackerbattle;
			else					
				percent = 0;

			// 30%
			if( percent  < 40)	
			{
				clfromitem.siItemNum = clfromitem.siItemNum - 3;	
			}
			// 60%
			else if( percent  < 70)	
			{	
				clfromitem.siItemNum = clfromitem.siItemNum - 2;	
			}
		}

		if (clfromitem.siItemNum <= 0)
			clfromitem.siItemNum = 1;
		//=================================================================

		//-----------------------------------------
		// 전리품을 줄 캐릭터를 확인한다. 
		//-----------------------------------------
		SI32 waritemid = 0;
		SI32 getitemrate = 1000 ;

		// 찜되어 있는 상태면 찜한 주체에게 전리품을 준다. 
		if(pclCM->CR[enemyid]->clJim.siJimHostCharUnique)
		{
			SI32 jimid = pclCM->GetIDFromCharUnique( pclCM->CR[enemyid]->clJim.siJimHostCharUnique );
			if(jimid)
			{
				if(pclCM->CR[jimid]->clJim.siJimSlaveCharUnique == pclCM->CR[enemyid]->GetCharUnique())
				{
					// 부모가 있다면(자신이 소환수라면) 
					SI32 parentid = pclCM->CR[jimid]->GetSummonParentID();
					if(parentid) 
					{
						cltKindInfo* pclKI	= NULL;
						if(pclClient->IsCountrySwitch(Switch_Summon_Separate)) 
						{	pclKI = pclClient->pclKindInfoSet->pclSummonKI[ pclCM->CR[jimid]->GetKind() ];		}
						else
						{	pclKI = pclClient->pclKindInfoSet->pclKI[ pclCM->CR[jimid]->GetKind() ];				}

						SI32 levelgap = pclKI->siSummonRank - pclCM->CR[enemyid]->pclKI->siSummonRank ;
						if ( pclKI->siSummonRank > 0 && levelgap > 17 && pclKI->bSummonMall != TRUE )
						{
							SI32 randrate = rand() % 1000 ;
							getitemrate = getitemrate - ( ( levelgap - 17 ) * 30 ) ;
							if ( randrate >= getitemrate )
							{
								return ;
							}
						}

						// 부모에게 전리품을 준다. 
						waritemid = parentid;
					}
					// 부모가 없는 경우라면, 
					else
					{
						// 자신이 전리품을 갖는다. 
						waritemid = jimid;
					}
				}
			}
		}

		// 아니면 공격자에게 전리품을 준다. 
		if(waritemid == 0)
		{
			//SI32  Lastattackid = pclCM->GetIDFromCharUnique (pclCM->CR[enemyid]->GetCharUnique());
			// 주인 아이디를 구한다.
			SI32 parentid = pclCM->CR[myid]->GetSummonParentID();// 주인아이디
			if(parentid) 
			{
				waritemid = parentid;
			}
			else{
				waritemid = myid;
			}

		}

		// 수렵장에서 사냥한 것이라면, 
		SI32 huntvillageunique	= pclCM->CR[myid]->GetHuntVillageUnique();
		GMONEY huntmoney			= 0;
		if(huntvillageunique)
		{
			if(rand() % 5 == 0)		// 20% 확률로 수렵장에 지급한다. 
			{
				cltItemPriceUnit* pclprice = pclItemManager->GetCurrentPrice(clfromitem.siUnique);
				if(pclprice)
				{
					huntmoney = pclprice->siPrice * clfromitem.siItemNum;

					cltHunt* pclhunt = (cltHunt*)pclHuntManager->pclStruct[huntvillageunique];
					if(pclhunt)
					{
						pclhunt->clStrInfo.clHuntStrInfo.siTempMoney += huntmoney;
					}
				}
			}
		}


		// 인스턴스 던전에서 사냥한다면
		GMONEY instancemoney = 0 ;
		SI32 mapindex = pclCM->CR[myid]->GetMapIndex();

		// 길드 소유가 가능한 던전에서 사냥한다면
		if( pclMapManager->IsGuildMap(mapindex)  )
		{
			cltItemPriceUnit* pclprice = pclItemManager->GetCurrentPrice(clfromitem.siUnique);
			if(pclprice)
			{
				GMONEY money = pclprice->siPrice * clfromitem.siItemNum;

				if ( money )	pclGuildDungeonMgr->AddMoney(mapindex, money);
			}
		}

		//------------------------------------------
		// 수렵장, 인던에 지급된 아이템이 아니면 캐릭터에게 지급한다. 
		//------------------------------------------
		if(huntmoney == 0 && instancemoney == 0) 
		{
			cltCharServer* pclchar = (cltCharServer*)pclCM->CR[waritemid];
			// 만약 네잎클로버 버프중이라면 갯수 * 2, 0.01% 확률로 * 100

			SI32 siBonus = 1;

			//-----------------------------------------------------------
			if( pclchar->pclCI->clBI.clBufInfo.IsHaveBuf(BUF_FOURLEAF) == true )
			{
				SI32 randVal = TABS(pclClient->CurrentClock)%10000;

				if( randVal < 1 )
				{						
					cltServer* pclserver = (cltServer*)pclClient;
					if( pclserver->pclFourLeafManager->IsFourleafRareItem(clfromitem.siUnique) == true ) {
						//clfromitem.siItemNum *= 2;
						siBonus = 2;
					}
					else {
						//clfromitem.siItemNum *= 100;
						siBonus = 100;
					}
				}
				else
				{
					//clfromitem.siItemNum *= 2;
					siBonus = 2;
				}	
			}

			//[진성] 서버 이벤트가 종료되 보상기간일 때 나타나는 맵에서는 무조건 드랍률이 2배이다. => 2008-7-2 
			if( IsCountrySwitch( Switch_Server_Event ) )
			{
				if(pclServerEventManager->IsEventMap(pclCM->CR[waritemid]->GetMapIndex()))
				{
					//1%확률로 백년삼이 떨어진다.
					if(1 > rand()%100)
					{
						clfromitem.Init();
						clfromitem.siUnique  = ITEMUNIQUE(2765);
						clfromitem.siItemNum = 1;

						// [로그추가 : 황진성 2008. 7. 7] 서버 이벤트
						// 백년삼.
						cltServer * pclServer = (cltServer*)pclClient;
						pclServer->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_SERVER_EVENT, LOGCOMMAND_INDEX_SERVER_EVENT_PREMIUM_GINSENG, 
							0, NULL, NULL, 0, 0, 0, &clfromitem, 0, 0, 0, 0, 0, 0, 
							0, 0, 0, 0, 0, NULL, NULL);

						pclClient->pclLog->FilePrint( "config\\ServerEvent.log" , "[GetItem]\t%s\tOneHundredGinseng", pclCM->CR[waritemid]->GetName());
					}
					// 백년삼이 아니면 2배.
					else
					{
						clfromitem.siItemNum = clfromitem.siItemNum * 2; // 2배를 해준다.
					}
				}
			}

			// [진성] 추석 이벤트. 사냥시 우주토끼2 인형 주기. => 2008-8-11
			if( pclEventTimeManager->InEventTime(TEXT("Chuseok_Monster_Appear"), &sTime) )
			{
				//[진성] 우주토끼2를 잡으면 인형을 주지 않는다. => 2008-9-3
				// PCK : 수정 - 우주토끼인형대신 칠면조인형으로 대신한다.

				if(pclCM->CR[enemyid]->GetKind() != pclClient->GetUniqueFromHash(TEXT("KIND_TURKEY")))
				{
					//	칠면조 인형 Drop확률 5% ->3%			
					if(3 > rand()%100)
					{
						clfromitem.Init();
						clfromitem.siUnique  = ITEMUNIQUE(12526);
						clfromitem.siItemNum = 1;

						pclClient->pclLog->FilePrint( "config\\ChuseokEvent.log" , "[ATTACK GiveItem] %s : KIND_TURKEY", pclCM->CR[waritemid]->GetName());
					}
				}

			}

			// [지연] 복불복 이벤트
			if ( pclClient->IsCountrySwitch(Switch_BokBulBok))	// 복불복 스위치
			{
				if ( IDENTITY_BOKBULBOK_ITEMMONSTER == pclCM->CR[enemyid]->clIdentity.siIdentity )
				{
					cltCharServer* pCharServer = pclCM->GetCharServer( enemyid );
					if ( NULL != pCharServer )
					{
						SI16 siItemUnique = 0;
						switch ( rand() % 4 )
						{
						case 0:
							siItemUnique = 3675;
							break;
						case 1:
							siItemUnique = 2006;
							break;
						case 2:
							siItemUnique = 5000;
							break;
						case 3:
							siItemUnique = 6105;
							break;
						}

						if ( 0 != siItemUnique && rand() % 10 == 0 )		// 10% 확률로 아이템 랜덤지급 -테스트용으로 하려면 나누는 수 10을 1로
						{
							clfromitem.Init();
							clfromitem.siUnique  = ITEMUNIQUE(siItemUnique);
							clfromitem.siItemNum = 1;
						}
					}
				}
			}

			// [성웅] 할로윈 이벤트 모든 몬스터를 잡았을때 촛대를 5%확률로 준다
			// PCK : 촛대 줄 확률 5% - > 3%으로 수정 (09.08.06)
			if( pclEventTimeManager->InEventTime(TEXT("HalloweenEvent"), &sTime) )
			{
				if(3 > rand()%100)
				{
					clfromitem.Init();
					clfromitem.siUnique  = ITEMUNIQUE( 13049 ) ;
					clfromitem.siItemNum = 1;

					pclClient->pclLog->FilePrint( "config\\halloweenEvent.log" , "[ATTACK GiveItem] %s :  Trick_or_Treat_Candy", pclCM->CR[waritemid]->GetName());
				}
			}

			// [춘기] 눈의 축복 버프를 받고 있으면 5%의 확률로 몬스터 사냥시 호랑가시나무 열매를 획득할 수 있다.
			if(pclchar->pclCI->clBI.clBufInfo.IsHaveBuf(BUF_CHRISTMASEVENT_SNOWBLESS) == true)
			{
				if ( (rand()%100) < 20 )
				{
					clfromitem.Init();
					clfromitem.siUnique  = ITEMUNIQUE( 13060 ) ;
					clfromitem.siItemNum = 1;

					pclClient->pclLog->FilePrint( "config\\ChristmasEvent.log" , "[ATTACK GiveItem] %s : Fruit of holly  ", pclCM->CR[waritemid]->GetName());
				}
			}

			// 이관 보상 아이템 - 아이템 2배 획득
			if( pclchar->pclCI->clBI.clBufInfo.IsHaveBuf(BUF_REWARDBONUS) == true )
			{
				siBonus = 2;
			}

			// 군주데이 - 아이템 2배 획득
			if( pclEventTimeManager->InEventTime("GoonzuDay_HuntItem2_18", &sTime) )
			{
				cltEventTime* pclEventTime =  pclEventTimeManager->GetEvent("GoonzuDay_HuntItem2_18");
				if(pclEventTime)		{		siBonus = pclEventTime->m_siBonusAmount;		}
			}

			//-------------------------------------------------------------------------------------
			//	보물찾기 이벤트 
			//-------------------------------------------------------------------------------------
			// 해당 퀘스트를 완료 했거나 퀘스트 진행 중일때 안나온다
			// 몬스터가 나보다 21Lev 이하면 안된다.
			if ( pclClient->IsCountrySwitch(Switch_TreasureEvent)  )
			{
				if( pclEventTimeManager->InEventTime(TEXT("TreasureEvent"), &sTime) )
				{
					bool bQuestStatus_Going_Clear = pclCM->CR[myid]->pclCI->clQuestInfo.IsQuestClearOrGoing(QUEST_TYPE_FINDTREASURE)	;
					if ( false ==  bQuestStatus_Going_Clear )
					{
						SI32 siMonsterKind = pclKindInfoSet->FindKindFromName( (TCHAR*)pclCM->CR[enemyid]->GetName()	)	;

						SI32 siMonsterLevel = pclCM->GetChar(enemyid )->pclCI->clIP.GetLevel()	;

						SI32 siMyLevel = pclCM->CR[myid]->pclCI->clIP.GetLevel()	;

						SI32 siLevelGap = siMonsterLevel - siMyLevel;

						if( siLevelGap > -21 )
						{
							// 5% 확률로아이템을 준다.
							if ( rand()%20 == 7)
							{
								clfromitem.Init();
								clfromitem.siUnique  = ITEMUNIQUE(13058);	// 지오반니 교서
								clfromitem.siItemNum = 1;

								pclClient->pclLog->FilePrint( "config\\TreasureEvent.log" , "[ATTACK GiveItem] %s : ", pclCM->CR[waritemid]->GetName());
							}
						}
					}
				}		 
			}		
			
			// [진성] 자이언트 라쿤 부활 이벤트에 나타나는 몬스터를 사냥하면 자이언트 라쿤 부활알을 준다.
			if( IDENTITY_GIANTRACCOONRESURRECTEVENT == pclCM->CR[enemyid]->clIdentity.siIdentity )
			{
				clfromitem.Init();
				clfromitem.siUnique  = ITEMUNIQUE(13064);
				clfromitem.siItemNum = 1;
								
				pclClient->pclLog->FilePrint( "config\\GiantRaccoonResurrectEvent.log" , "[GiveItem] %s : ", pclItemManager->GetName( clfromitem.siUnique ) );
			}

			// 아이템 보너스 지급
			clfromitem.siItemNum *= siBonus;

			// 통계를 낸다. 
			clfromitem.siItemNum = pclchar->CalcTiredPoint( clfromitem.siItemNum );

			if ( clfromitem.siItemNum > 0 )
			{
				pclItemManager->AddWarItemNum(clfromitem.siUnique, clfromitem.siItemNum);

				// 새로운 통계 - 전리품 아이템 통계를 낸다.
				NStatistics_Add("Item", clfromitem.siUnique, "MonNum", clfromitem.siItemNum);
				NStatistics_Set("Item", clfromitem.siUnique, "Name", pclItemManager->GetName(clfromitem.siUnique));

				InputItem(pclchar, &clfromitem, INPUTITEMPERSON_OPTION_WAR);

				//[추가 : 황진성 2007. 11. 13 대왕 구리댁 죽이면 얻는 아이템 공지를 위한.]
				if(pclCM->CR[enemyid]->GetKind() == pclClient->GetUniqueFromHash(TEXT("KIND_BIGRACCOON")))
				{
					TCHAR  buffer[MAX_MESSAGE_SIZE] = {'\0', };
					TCHAR* text					    = GetTxtFromMgr( 7610 );

					StringCchPrintf(buffer, sizeof(buffer), text, pclCM->CR[waritemid]->pclCI->clBI.clPerson.szName, pclItemManager->GetName(clfromitem.siUnique), clfromitem.siItemNum);

					cltGameMsgResponse_KingGuriInfo clKingGuriInfo(CKingGuriEvent::ITEM, buffer);
					cltMsg clMsg( GAMEMSG_RESPONSE_KINGGURIINFO, sizeof(clKingGuriInfo), (BYTE*)&clKingGuriInfo );

					SI32 index = 0;
					SI32 id	   = 0;
					while(id = pclClient->pclCM->GetCharOrder(index))
					{
						index++;

						// 사용자인 컴에게만 전달한다. 
						if( true == pclClient->pclCM->CR[id]->IsValidConnection() )
						{
							pclClient->pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
						}
					}
				}
				// [영훈] 2008.01.13 - 설날 이벤트 추가
				else if ( pclCM->CR[enemyid]->GetKind() == pclClient->GetUniqueFromHash( TEXT("KIND_KITERACCOON") ) )
				{
					TCHAR tszName[MAX_PLAYER_NAME] = {'\0', };
					StringCchCopy( tszName, MAX_PLAYER_NAME, pclchar->GetName() );

					cltGameMsgResponse_KiteRaccoonInfo clKiteRaccoonInfo( cltEventTimeManager::KITERACCOON_EVENT_KILL, pclCM->CR[enemyid]->GetKind(), clfromitem.siUnique, tszName );
					cltMsg clMsg( GAMEMSG_RESPONSE_KITERACCOONINFO, sizeof(clKiteRaccoonInfo), (BYTE*)&clKiteRaccoonInfo );

					pclCM->SendAllPersonMsg( (sPacketHeader*)&clMsg );
				}
			}
		}
	}
	//------------------------------------------------------
	// 수렵술이 연마되어 있다면, 소환인형을 얻는다. 
	//------------------------------------------------------
	// 수렵장이 아니어야 한다. 
	if( pclMapManager->IsHuntMap(pclCM->CR[myid]->GetMapIndex()) == false )
	{
		// 내가 소환수라면
		SI32 parentid = pclCM->CR[myid]->GetSummonParentID();
		SI32 siID = myid ;
		if(pclCM->IsValidID(parentid))
		{
			siID = parentid ;
		}

		SI32 captureskilllevel	= pclCM->CR[siID]->pclCI->clCharSkill.GetTotalSkillLevel(SKILL_CAPTURE, 0, 0,
			&pclCM->CR[siID]->pclCI->clCharItem.clItem[0]);
		if(captureskilllevel)
		{
			SI32 capturerate = pclCM->CR[enemyid]->pclKI->GetCaptureRate(captureskilllevel);
			if(rand() % 10000 < capturerate)
			{
				SI32 dollitemunique = 0;
				for(i = 0;i < MAX_ITEMINFO_NUMBER;i++)
				{
					if(pclItemManager->pclItemInfo[i] == NULL)continue;

					if(pclItemManager->pclItemInfo[i]->siMakeKind == pclCM->CR[enemyid]->GetKind())
					{
						dollitemunique = pclItemManager->pclItemInfo[i]->clItem.siUnique;
					}
				}

				if(dollitemunique)
				{
					cltCharServer* pclchar = (cltCharServer*)pclCM->CR[siID];

					// test
					SI32 dollnum	= pclchar->CalcTiredPoint( CAPTURE_DOLLNUM );

					// 통계를 낸다. 
					pclItemManager->AddWarItemNum(dollitemunique, dollnum);


					//cyj pclchar->GiveItem(dollitemunique, dollnum, INPUTITEMPERSON_OPTION_WAR/*INPUTITEMPERSON_OPTION_NONE*/,0);
					SI32 ref = pclClient->pclItemManager->FindItemRefFromUnique(dollitemunique);
					UI16 usedate = pclClient->pclItemManager->pclItemInfo[ref]->clItem.uiDateUseDay;
					pclchar->GiveItem(dollitemunique, dollnum, INPUTITEMPERSON_OPTION_WAR/*INPUTITEMPERSON_OPTION_NONE*/,usedate);

					SI32 skillunique	= SKILL_CAPTURE;
					SI32 maxskilllevel	= pclSkillManager->GetMaxLevel(skillunique);
					if(skillunique >= 0)
					{
						// 기술 경험치량을 구한다. 
						SI32 skillexp = max(1, pclCM->CR[enemyid]->pclKI->Exp / 5);

						// 피로방지 시스템 적용
						pclchar->CalcTiredPoint( skillexp );
						pclchar->SetSkillUpdate(skillunique, skillexp);
					}

				}
			}
		}
	}

	// 인스턴스 던전의 몬스터를 죽인다면
	if( pclMapManager->IsInstanceMap(pclCM->CR[myid]->GetMapIndex()) == true )
	{
		SI32 mapindex = pclCM->CR[myid]->GetMapIndex() ;
	}
}

// pvp-
SI32 cltServer::GetPVPWeaponDamage( SI32 RealDamage, SI32 AttackType )
{
	SI32 returnValue = 0;
	if( ATTACKTYPE_HAND == AttackType )
	{
		returnValue = RealDamage * siHandDamageRate / 100;
	}
	else if( ATTACKTYPE_SWORD == AttackType )
	{
		returnValue = RealDamage * siSwordDamageRate / 100;
	}
	else if( ATTACKTYPE_SPEAR == AttackType )
	{
		returnValue = RealDamage * siSpearDamageRate / 100;
	}
	else if( ATTACKTYPE_AXE == AttackType )
	{
		returnValue = RealDamage * siAxeDamageRate / 100;
	}
	else if( ATTACKTYPE_BOW == AttackType ||
		ATTACKTYPE_BOW1 == AttackType )
	{
		returnValue = RealDamage * siBowDamageRate / 100;
	}
	else if( ATTACKTYPE_GUN == AttackType ||
		ATTACKTYPE_GUN == AttackType )
	{
		returnValue = RealDamage * siGunDamageRate / 100;
	}
	else if( ATTACKTYPE_STAFF == AttackType ||
		ATTACKTYPE_STAFF1 == AttackType ||
		ATTACKTYPE_STAFF2 == AttackType )
	{
		returnValue = RealDamage * siStaffDamageRate / 100;
	}
	else
		returnValue = RealDamage;

	pclClient->pclLog->LogPrint(TEXT("AttackType : %d \n"), AttackType );

	return returnValue;
}


bool cltServer::NewYearEventDay(void)
{
	//[추가 : 황진성 2007. 12. 11 신년이벤트 행운상자 획득 가능 기간.]
	SYSTEMTIME	StartTime;	ZeroMemory(&StartTime, sizeof(StartTime));
	SYSTEMTIME	EndTime;	ZeroMemory(&EndTime,   sizeof(EndTime));
	switch(pclClient->siServiceArea)
	{
	case ConstServiceArea_English:	
		{
			StartTime.wYear	 = 2008;		EndTime.wYear 	= 2008;
			StartTime.wMonth =   11;		EndTime.wMonth	=	12;
			StartTime.wDay	 =   10;		EndTime.wDay	=   31;
			StartTime.wHour	 =    3;		EndTime.wHour	=    2;
		}
		break;
	case ConstServiceArea_Korea:	
		{
			StartTime.wYear	 = 2007;		EndTime.wYear 	= 2008;
			StartTime.wMonth =   12;		EndTime.wMonth	=	 1;
			StartTime.wDay	 =   27;		EndTime.wDay	=   10;
			StartTime.wHour	 =    0;		EndTime.wHour	=   23;
		}
		break;
	case ConstServiceArea_Japan:	
		{
			StartTime.wYear	 = 2007;		EndTime.wYear 	= 2008;
			StartTime.wMonth =   12;		EndTime.wMonth	=	 1;
			StartTime.wDay	 =   26;		EndTime.wDay	=   10;
			StartTime.wHour	 =    0;		EndTime.wHour	=   11;
		}
		break;
	case ConstServiceArea_Taiwan:	
	case ConstServiceArea_USA:	
		{
			StartTime.wYear	 = 2007;		EndTime.wYear 	= 2008;
			StartTime.wMonth =   12;		EndTime.wMonth	=	 1;
			StartTime.wDay	 =   26;		EndTime.wDay	=    9;
			StartTime.wHour	 =    0;		EndTime.wHour	=   23;
		}
		break;
	case ConstServiceArea_NHNChina:	
		{
			StartTime.wYear	 = 2007;		EndTime.wYear 	= 2008;
			StartTime.wMonth =   12;		EndTime.wMonth	=	 1;
			StartTime.wDay	 =   27;		EndTime.wDay	=    8;
			StartTime.wHour	 =    3;		EndTime.wHour	=    2;
		}
		break;
	case ConstServiceArea_EUROPE:	
		{
			StartTime.wYear	 = 2008;		EndTime.wYear 	= 2008;
			StartTime.wMonth =   11;		EndTime.wMonth	=	12;
			StartTime.wDay	 =   10;		EndTime.wDay	=   31;
			StartTime.wHour	 =    3;		EndTime.wHour	=    2;
		}
		break;
	}

	SYSTEMTIME	CurTime;
	GetSystemTime(&CurTime);
	if(true == TimeInBounds(&CurTime, &StartTime, &EndTime))
	{
		return true;
	}

	return false;	
}

void cltServer::NewYearLuckyBox(cltItem& clfromitem)
{
	if(pclClient->pclEventTimeManager->InEventTime("NewYearEvent", &sTime) )
	{
		const cltUniqueRange ItemUnique = { 13303, 1 };
		SI32 Unique = GetUniqueWithRange( &ItemUnique, sizeof(ItemUnique) / sizeof(cltUniqueRange), 300);

		if(0 != Unique)
		{
			ZeroMemory(&clfromitem, sizeof(clfromitem));

			clfromitem.siUnique  = Unique;
			clfromitem.siItemNum = 1;
		}
	}
	//if(true == NewYearEventDay())
	//{
	//	const cltUniqueRange ItemUnique = { 13303, 1 };
	//	SI32 Unique = GetUniqueWithRange( &ItemUnique, sizeof(ItemUnique) / sizeof(cltUniqueRange), 300);

	//	if(0 != Unique)
	//	{
	//		ZeroMemory(&clfromitem, sizeof(clfromitem));

	//		clfromitem.siUnique  = Unique;
	//		clfromitem.siItemNum = 1;
	//	}
	//}
}

void cltServer::SpecialDamage(SI32 myid, SI32 enemyid, SI32* psiRealDamage, SI32* psiDisplayDamage)
{
	cltCharServer* pclenemychar = (cltCharServer*)pclCM->CR[enemyid];

	//--------------------------------------
	// GOD - 실제 줄이지는 않으면서 화면 표시는 그대로 
	//--------------------------------------
	// 맞는놈이 사람이고  GOD 모드라면 Damage = 0
	if( IsPC( pclCM->CR[enemyid]->GetCharUnique() ) && pclCM->CR[enemyid]->bGodMode == true )
	{
		*psiRealDamage    = 0;
		*psiDisplayDamage = *psiDisplayDamage;
	}

	//---------------------------------------
	// CLONE - 실제로 모두 에너지를 줄이긴 하지만 표시는 0 damage를 설정 
	//---------------------------------------
	if( pclenemychar->IsAtb(ATB_CLONE) && pclenemychar->clInnerStatus.IsStatus( INNERSTATUS_CLONE ) == true )
	{
		*psiRealDamage   = pclenemychar->GetLife();
		*psiDisplayDamage= 0;	// Damage를 작게 보이게 한다.
	}
	//=================================================================

	//----------------------------------------------------------------------------
	// 공성중 유저에게 주는 대미지는 1/3 한다. [영진]
	//----------------------------------------------------------------------------
	if(IsPC(pclCM->CR[myid]->GetCharUnique()) && IsPC(pclCM->CR[enemyid]->GetCharUnique()) )
	{
		if( pclVillageManager->IsVillageAtWar( pclCM->CR[myid]->pclCI->clBI.siHomeVillage ) 
			&& pclVillageManager->IsVillageAtWar( pclCM->CR[enemyid]->pclCI->clBI.siHomeVillage	) )
		{
			*psiRealDamage /=3;
			*psiRealDamage = max(1,*psiRealDamage);
		}
	}

	// 새로운 길드전 맵 소유 길드전이 들어가면 데미지를 줄아지 않는다.
	//if (IsCountrySwitch(Switch_NewGuildHuntMapWar) )
	//{
	//	//----------------------------------------------------------------------------
	//	// 길드전중 유저에게 주는 대미지는 1/5 한다. [유상]
	//	//----------------------------------------------------------------------------
	//	if(IsPC(pclCM->CR[myid]->GetCharUnique()) && IsPC(pclCM->CR[enemyid]->GetCharUnique()) )
	//	{
	//		SI32 myMapIndex = pclCM->CR[myid]->GetMapIndex();
	//		SI32 enemyMapIndex = pclCM->CR[enemyid]->GetMapIndex();
	//		if( IsCountrySwitch(Switch_NewGuildHuntMapWar ) )	// 변경 후
	//		{
	//			if ( IsGuildWarMap_NEW(myMapIndex) && IsGuildWarMap_NEW(enemyMapIndex)  )
	//			{
	//				SI32 enemyPersonID = pclCM->CR[enemyid]->pclCI->GetPersonID();

	//				if ( pclGuildHuntMapManager->IsGod(enemyPersonID ) )
	//				{
	//					*psiRealDamage = 0;
	//					*psiDisplayDamage = 0;
	//				}
	//			}
	//			else if ( IsGuildWarMap_Use_New_GuildWar(myMapIndex) && IsGuildWarMap_Use_New_GuildWar(enemyMapIndex) )
	//			{
	//				SI32 enemyPersonID = pclCM->CR[enemyid]->pclCI->GetPersonID();
	//				if( IsCountrySwitch(Switch_NewGuildHuntMapWar ) )	// 변경 후
	//				{
	//					if ( pclGuildWarManager[GetGuildWarFieldIndexFromMapIndex_Use_New_Guild_War( enemyMapIndex )]->IsGod( enemyPersonID ) )
	//					{
	//						*psiRealDamage = 0;
	//						*psiDisplayDamage = 0;
	//					}
	//					else
	//					{
	//						*psiRealDamage /= 5;
	//						*psiRealDamage = max(1,*psiRealDamage);

	//						*psiDisplayDamage /= 5;
	//						*psiDisplayDamage = max(1,*psiDisplayDamage);
	//					}

	//				}
	//				else	//변경전
	//				{
	//					if ( pclGuildWarManager[GetGuildWarFieldIndexFromMapIndex_Not_Use_New_Guild_War( enemyMapIndex )]->IsGod( enemyPersonID ) )
	//					{
	//						*psiRealDamage = 0;
	//						*psiDisplayDamage = 0;
	//					}
	//					else
	//					{
	//						*psiRealDamage /= 5;
	//						*psiRealDamage = max(1,*psiRealDamage);

	//						*psiDisplayDamage /= 5;
	//						*psiDisplayDamage = max(1,*psiDisplayDamage);
	//					}
	//				}


	//			}
	//		}
	//		else
	//		{
	//			if ( IsGuildWarMap_Not_Use_New_GuildWar(myMapIndex) && IsGuildWarMap_Not_Use_New_GuildWar(enemyMapIndex) )
	//			{
	//				SI32 enemyPersonID = pclCM->CR[enemyid]->pclCI->GetPersonID();
	//				if( IsCountrySwitch(Switch_NewGuildHuntMapWar ) )	// 변경 후
	//				{
	//					if ( pclGuildWarManager[GetGuildWarFieldIndexFromMapIndex_Use_New_Guild_War( enemyMapIndex )]->IsGod( enemyPersonID ) )
	//					{
	//						*psiRealDamage = 0;
	//						*psiDisplayDamage = 0;
	//					}
	//					else
	//					{
	//						*psiRealDamage /= 5;
	//						*psiRealDamage = max(1,*psiRealDamage);

	//						*psiDisplayDamage /= 5;
	//						*psiDisplayDamage = max(1,*psiDisplayDamage);
	//					}

	//				}
	//				else	//변경전
	//				{
	//					if ( pclGuildWarManager[GetGuildWarFieldIndexFromMapIndex_Not_Use_New_Guild_War( enemyMapIndex )]->IsGod( enemyPersonID ) )
	//					{
	//						*psiRealDamage = 0;
	//						*psiDisplayDamage = 0;
	//					}
	//					else
	//					{
	//						*psiRealDamage /= 5;
	//						*psiRealDamage = max(1,*psiRealDamage);

	//						*psiDisplayDamage /= 5;
	//						*psiDisplayDamage = max(1,*psiDisplayDamage);
	//					}
	//				}


	//			}

	//		}
	//	}
	//}
	//else	// 길드워 적용전
	//{
	//	SI32 myMapIndex = pclCM->CR[myid]->GetMapIndex();
	//	SI32 enemyMapIndex = pclCM->CR[enemyid]->GetMapIndex();

	//	if ( IsGuildWarMap_NEW(myMapIndex) && IsGuildWarMap_NEW(enemyMapIndex)  )
	//	{
	//		SI32 enemyPersonID = pclCM->CR[enemyid]->pclCI->GetPersonID();

	//		if ( pclGuildHuntMapManager->IsGod(enemyPersonID ) )
	//		{
	//			*psiRealDamage = 0;
	//			*psiDisplayDamage = 0;
	//		}
	//	}// 적용 전이니까 얘들 필요 없어 
	//	else if(IsGuildWarMap_Not_Use_New_GuildWar(myMapIndex) && IsGuildWarMap_Not_Use_New_GuildWar(enemyMapIndex))
	//	{
	//		SI32 enemyPersonID = pclCM->CR[enemyid]->pclCI->GetPersonID();

	//		SI32 siindex = GetGuildWarFieldIndexFromMapIndex_Not_Use_New_Guild_War( enemyMapIndex )	;
	//		if ( pclGuildWarManager[siindex ]->IsGod( enemyPersonID ) )
	//		{
	//			*psiRealDamage = 0;
	//			*psiDisplayDamage = 0;
	//		}
	//	}
	//}
	if (IsCountrySwitch(Switch_NewGuildHuntMapWar) )
	{
		SI32 myMapIndex = pclCM->CR[myid]->GetMapIndex();
		SI32 enemyMapIndex = pclCM->CR[enemyid]->GetMapIndex();

		if ( IsGuildWarMap_NEW(myMapIndex) && IsGuildWarMap_NEW(enemyMapIndex)  )
		{
			SI32 enemyPersonID = pclCM->CR[enemyid]->pclCI->GetPersonID();

			if ( pclGuildHuntMapManager->IsGod(enemyPersonID ) )
			{
				*psiRealDamage = 0;
				*psiDisplayDamage = 0;
			}
		}
		else if(IsGuildWarMap_Use_New_GuildWar(myMapIndex) && IsGuildWarMap_Use_New_GuildWar(enemyMapIndex))
		{
			SI32 enemyPersonID = pclCM->CR[enemyid]->pclCI->GetPersonID();

			if ( pclGuildWarManager[GetGuildWarFieldIndexFromMapIndex_Use_New_Guild_War( enemyMapIndex )]->IsGod( enemyPersonID ) )
			{
				*psiRealDamage = 0;
				*psiDisplayDamage = 0;
			}
		}

	}
	else
	{
		//----------------------------------------------------------------------------
		// 길드전중 유저에게 주는 대미지는 1/5 한다. [유상]
		//----------------------------------------------------------------------------
		if(IsPC(pclCM->CR[myid]->GetCharUnique()) && IsPC(pclCM->CR[enemyid]->GetCharUnique()) )
		{
			SI32 myMapIndex = pclCM->CR[myid]->GetMapIndex();
			SI32 enemyMapIndex = pclCM->CR[enemyid]->GetMapIndex();

			if ( IsGuildWarMap_Not_Use_New_GuildWar(myMapIndex) && IsGuildWarMap_Not_Use_New_GuildWar(enemyMapIndex) )
			{
				SI32 enemyPersonID = pclCM->CR[enemyid]->pclCI->GetPersonID();

				if ( pclGuildWarManager[GetGuildWarFieldIndexFromMapIndex_Not_Use_New_Guild_War( enemyMapIndex )]->IsGod( enemyPersonID ) )
				{
					*psiRealDamage = 0;
					*psiDisplayDamage = 0;
				}
				else
				{
					*psiRealDamage /= 5;
					*psiRealDamage = max(1,*psiRealDamage);

					*psiDisplayDamage /= 5;
					*psiDisplayDamage = max(1,*psiDisplayDamage);
				}
			}
		}
	}
	//----------------------------------------------------------------------------
	// PVP 데미지
	//----------------------------------------------------------------------------
	if(pclClient->siEnablePVP == 1)
	{
		if ( (IsPC(pclCM->CR[myid]->GetCharUnique())) && (IsPC(pclCM->CR[enemyid]->GetCharUnique())) )
		{
			if( pclMapManager->IsPkMap( pclCM->CR[myid]->GetMapIndex() ) )
			{
				if( pclMapManager->IsPkMap( pclCM->CR[enemyid]->GetMapIndex() ) )
				{
					if( pclCM->CR[myid]->clInnerStatus.clEnemyType.IsType( ENEMYTYPE_PVP )  ||
						pclCM->CR[enemyid]->clInnerStatus.clEnemyType.IsType( ENEMYTYPE_PVP ) ) 
					{		
						// 각 무기별로 데미지 감소가 들어간다.
						*psiRealDamage = GetPVPWeaponDamage( *psiRealDamage, pclCM->CR[myid]->GetAttackType() );
						*psiRealDamage = max(1,*psiRealDamage);
						*psiDisplayDamage = *psiRealDamage;
					}
				}
			}	
		}
	}

	//----------------------------------------------------------------------------
	// 대왕구리댁 관련
	//----------------------------------------------------------------------------
	if( pclCM->CR[enemyid]->GetKind() == pclClient->GetUniqueFromHash(TEXT("KIND_RACCOONCHINA")) )
	{
		*psiRealDamage = 1 ;
		*psiDisplayDamage = 1 ;
	}

	//----------------------------------------------------------------------------
	//[추가 : 황진성 2007. 11. 13 대왕구리댁 관련 데미지]
	//----------------------------------------------------------------------------
	if( pclCM->CR[enemyid]->GetKind() == pclClient->GetUniqueFromHash(TEXT("KIND_BIGRACCOON")) )
	{
		*psiRealDamage	  = 1;
		*psiDisplayDamage = 1;
	}

	//----------------------------------------------------------------------------
	// OneKill모드라면 적을 한방에 죽인다.
	//----------------------------------------------------------------------------
	if( pclCM->CR[myid]->bOneKill == true )
	{
		*psiRealDamage = pclenemychar->GetLife();
	}

	// PCK : 공격력방어제를 먹고 있으면 뼈티라노 사냥시에 공격력 30%증가 된다.
	if ( (pclCM->CR[enemyid]->GetKind() == pclClient->GetUniqueFromHash(TEXT("KIND_BONETYRANNO"))) && IsPC(myid) )
	{
		cltBufInfo * pclAttackBuf = &pclCM->CR[ myid ]->pclCI->clBI.clBufInfo;
		if ( (pclAttackBuf) && (pclAttackBuf->IsHaveBuf(BUF_QUEST_ATTACK)) )
		{
			*psiRealDamage		= (*psiRealDamage * 130 )/100; 
			*psiDisplayDamage	= (*psiDisplayDamage * 130)/100;
		}
	}

	//[진성] 자이언트 라쿤 부활 축제 이벤트 관련해서 생성된 몬스터는 한방에 죽게 한다.
	if( IDENTITY_GIANTRACCOONRESURRECTEVENT == pclenemychar->clIdentity.siIdentity )
	{
		*psiRealDamage = pclenemychar->GetLife();
	}

	if (IsCountrySwitch(Switch_10MonthEvent_RacoonDodge) )
	{
		SI32 siMapindex = pclCM->CR[enemyid]->GetMapIndex()	;
		if ( siMapindex == PLAY_MAP_INDEX_INDODEVENT )
		{
			SI32 siMapindex = pclCM->CR[enemyid]->GetMapIndex()	;
			if ( siMapindex == PLAY_MAP_INDEX )
			{
				cltMapCommon* pclmap = pclClient->pclMapManager->GetMapPointer(siMapindex)	;

				if(pclmap != NULL)
				{
					SI32 siColl = pclmap->CalcCollInfo( pclCM->CR[enemyid]->GetX(), pclCM->CR[enemyid]->GetY())	;
					if( siColl & BLOCK_EVENT1 || siColl & BLOCK_EVENT4 )	// 4번 이벤타 타일을 밟고 있을때.
					{
						*psiRealDamage    = 10000;
						*psiDisplayDamage = 10000;
					}
				}
			}
		}
	}



}

void cltServer::DeadChar_PC(SI32 myid, SI32 enemyid)
{
	cltCharServer* pclchar		= (cltCharServer*)pclCM->CR[myid];		// 죽인놈(살인자) 
	cltCharServer* pclenemychar = (cltCharServer*)pclCM->CR[enemyid];	// 죽은놈 

	/// 죽임을 당한자에게 킬러의 이름을 기억시킨다. 
	StringCchCopy(pclenemychar->szKillMeCharName, MAX_PLAYER_NAME, pclchar->GetName());

	if(!IsPC(pclCM->CR[myid]->GetCharUnique()))
	{
		// LEEKH MODIFY 2007.10.25 - 몬스터에게서 캐릭터가 죽은 상황을 로그로 남긴다
		if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
		{
			// [로그추가 : 황진성 2007. 10. 15] // 캐릭터 사망. 
			// param1    == 신원.
			// param2	 == 이 캐릭터가 어떤 모드로 작동하는가 ?(왜구전쟁, .. ) 
			SendLog_WriteLogToDB(LOGCOMMAND_TYPE_CHARACTOR, LOGCOMMAND_INDEX_CHARDIE, 
				0, pclenemychar, NULL, 0, 0, 0, 0, 0, 0, pclchar->GetKind(), 0, 0, 0, 
				pclchar->clIdentity.siIdentity, pclchar->siCharMode, 0, 0, 0, NULL, NULL);
		}
	}

	//----------------------------------
	// 공격자가 불가사리라면, 
	//----------------------------------
	if(pclchar->clIdentity.siIdentity == IDENTITY_VILLAGEHUNT)
	{
		pclenemychar->siKilledMode = KILLEDMODE_VILLAGEHUNT;		// 사망 페널티가 적용되지 않는다. 
	}

	//----------------------------------
	// [기형] 공격자가 배틀로얄 게임중이 캐릭터 일 경우
	//----------------------------------
	if(NULL != m_pclBattleRoyalMgr)
	{
		CBattleRoyal* pclBattleRoyal = m_pclBattleRoyalMgr->GetBattleRoyal(pclenemychar);
		if( NULL != pclBattleRoyal)
		{
			if( pclenemychar->GetMapIndex() == pclBattleRoyal->GetStadiumMapIndex()
				&& pclchar->GetMapIndex() == pclBattleRoyal->GetStadiumMapIndex())
			{
				pclenemychar->siKilledMode = KILLEDMODE_BATTLEROYAL;
				if(pclchar->siCharMode == CHARMODE_BATTLEROYAL)
				{
					pclBattleRoyal->GiveItemToKiller(pclchar);
				}
			}
		}
	}
	//----------------------------------
	// 공격자가 흑의군단에서(전체전투)
	//----------------------------------
	if( pclchar->siCharMode == CHARMODE_BLACKWAR )
	{
		pclenemychar->siKilledMode = KILLEDMODE_BLACKWAR;		// 사망 페널티가 적용되지 않는다. 
		pclenemychar->siBlackWarKilledNumber++;
		//[추가 : 황진성 2008. 1. 22 => 흑의 군단 개편 후에는 싸워서 죽으면 주는 페널티를 적용하지 않는다.]
		if(pclClient->IsCountrySwitch(Switch_BlackWarReform)											&&
			( BLACKARMY_MAP_STARTINDEX > pclenemychar->GetCurrentVillageUnique()						||
			BLACKARMY_MAP_STARTINDEX+MAX_BLACKARMY_MAP_CNT < pclenemychar->GetCurrentVillageUnique())			)
		{

		}
		else
		{
			if (pclenemychar->siBlackWarEnemyKillNumber >= 2)
				pclenemychar->siBlackWarEnemyKillNumber -= 2;
			else
				pclenemychar->siBlackWarEnemyKillNumber = 0;

			//--------------------------------
			// 클라이언트로 전과를 알려준다. 
			//--------------------------------
			cltGameMsgResponse_WarKill clinfo( 0, pclenemychar->siBlackWarEnemyKillNumber, pclenemychar->siBlackWarKilledNumber, 0 );
			cltMsg clMsg(GAMEMSG_RESPONSE_WARKILL, sizeof(clinfo), (BYTE*)&clinfo);
			pclCM->CR[pclenemychar->GetID()]->SendNetMsg((sPacketHeader*)&clMsg);
		}
	}

	//----------------------------------
	// 공격자가 흑의 군단이면(이벤트전투)
	//----------------------------------
	if(pclchar->clIdentity.siIdentity == IDENTITY_BLACKARMY)
	{
		pclenemychar->siKilledMode = KILLEDMODE_BLACKARMY;		// 사망 페널티가 적용되지 않는다. 
		pclenemychar->siWarKilledNumber++;
		if (pclenemychar->siWarEnemyKillNumber >= 2)
			pclenemychar->siWarEnemyKillNumber = pclenemychar->siWarEnemyKillNumber - 2;
		else
			pclenemychar->siWarEnemyKillNumber = 0;

		//--------------------------------
		// 클라이언트로 전과를 알려준다. 
		//--------------------------------
		cltGameMsgResponse_WarKill clinfo(1, pclenemychar->siWarEnemyKillNumber, pclenemychar->siWarKilledNumber, pclenemychar->siBlackArmyWarMapIndex);
		cltMsg clMsg(GAMEMSG_RESPONSE_WARKILL, sizeof(clinfo), (BYTE*)&clinfo);
		pclCM->CR[pclenemychar->GetID()]->SendNetMsg((sPacketHeader*)&clMsg);
	}


	//----------------------------------
	// 공격자가 모후모후 이벤트 몬스터이면
	//----------------------------------
	if(pclchar->clIdentity.siIdentity == IDENTITY_MOFUMOFUEVENT)
	{
		pclenemychar->siKilledMode = KILLEDMODE_MOFUMOFUEVENT;		// 사망 페널티가 적용되지 않는다. 
	}

	//-----------------------------
	// 인스턴스 던젼에서 사망
	//----------------------------
	if( pclClient->pclMapManager->IsInstanceMap( pclenemychar->GetMapIndex() ) )
	{
		pclenemychar->siKilledMode = KILLEDMODE_INSTANCEMAP;
	}

	//-----------------------------
	// 엔펠리스에서 부활시켜야 할경우, 여기서 설정한다.
	//KHY - 1113 - 크리스마스 이벤트 - 산타 작업장에서 사망시.
	//----------------------------
	if(pclenemychar->GetMapIndex()  == MAPINDEX_SANTAFACTORY_1  ||
		pclenemychar->GetMapIndex()  == MAPINDEX_SANTAFACTORY_2  ||
		pclenemychar->GetMapIndex()  == MAPINDEX_SANTAFACTORY_3  ||
		pclenemychar->GetMapIndex()  == MAPINDEX_SANTAFACTORY_1_2||
		pclenemychar->GetMapIndex()  == MAPINDEX_SANTAFACTORY_2_2||
		pclenemychar->GetMapIndex()  == MAPINDEX_SANTAFACTORY_3_2)
	{
		pclenemychar->siKilledMode = KILLEDMODE_GOTOSAFEZONE;
	}


	//--------------------------------
	// 공성전중 사망 결정
	//--------------------------------
	// 나를 죽인놈이 마을 적대적인 관계라면, 
	if( bVillageWarSwitch && ( pclchar->clInnerStatus.clEnemyType.siEnemyType & ENEMYTYPE_VILLAGE))
	{
		pclenemychar->siKilledMode = KILLEDMODE_VILLAGEWAR;	// 공성전 중 사망. 

		SI32 levelGep = pclchar->pclCI->clIP.GetLevel() - pclenemychar->pclCI->clIP.GetLevel();


		// 적이 나보다 11레벨 이상 높다면 10 포인트 가산
		if( levelGep < -10  )
			pclchar->siVillageWarKillScore += 10;
		else if( levelGep > 10)	//적이 나보다 11레벨 이하 낮다면
			pclchar->siVillageWarKillScore += 1;
		else
			pclchar->siVillageWarKillScore += 5;

		// LEEKH MODIFY 2007.10.25 - 공성전 중 사망 로그
		if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
		{
			// [로그추가 : 황진성 2007. 10. 15] // 캐릭터 사망. 
			// param1 == 죽인놈의 마을 유니크
			SendLog_WriteLogToDB(LOGCOMMAND_TYPE_CHARACTOR, LOGCOMMAND_INDEX_CHARLDIE_VILLAGEWAR, 
				0, pclenemychar, NULL, pclchar->pclCI->GetPersonID(), 0, 0, 0, 0, 0, 0, 0, 0, 0, 
				pclchar->pclCI->clBI.siHomeVillage, 0, 0, 0, 0, NULL, NULL);
		}
	}

	//-----------------------------
	// 길드전중 사망 결정
	//----------------------------
	SI32 fieldindex = 0 ;
	if ( pclClient->IsCountrySwitch(Switch_NewGuildHuntMapWar ))	// 변경후
	{
		fieldindex = GetGuildWarFieldIndexFromMapIndex_Use_New_Guild_War( pclchar->GetMapIndex() );
	}
	else	// 변경전 
	{
		fieldindex = GetGuildWarFieldIndexFromMapIndex_Not_Use_New_Guild_War( pclchar->GetMapIndex() );
	}
	//변경후
	if ( pclClient->IsCountrySwitch(Switch_NewGuildHuntMapWar ))
	{
		if ( fieldindex >= 0 && fieldindex < GUILDWAR_FIELDNUM_OLD )
		{
			pclGuildWarManager[fieldindex]->AddKillPoint( pclchar->pclCI->GetPersonID() );
			// LEEKH MODIFY 2007.10.25 -  길드전 중 사망로그
			if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
			{
				// [로그추가 : 황진성 2007. 10. 15] // 캐릭터 사망. 
				// param1 == 죽인놈의 소속 길드 인덱스 추가
				SendLog_WriteLogToDB(LOGCOMMAND_TYPE_CHARACTOR, LOGCOMMAND_INDEX_CHARLDIE_GUILDWAR, 
					0, pclenemychar, NULL, pclchar->pclCI->GetPersonID(), 0, 0, 0, 0, 0, 0, 0, 0, 0, 
					pclchar->pclCI->clBI.clGuildRank.siGuildUnitIndex, 0, 0, 0, 0, NULL, NULL);
			}	
		}
	}
	//변경전
	else
	{
		if ( fieldindex >= 0 && fieldindex < GUILDWAR_FIELDNUM)
		{
			pclGuildWarManager[fieldindex]->AddKillPoint( pclchar->pclCI->GetPersonID() );
			// LEEKH MODIFY 2007.10.25 -  길드전 중 사망로그
			if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
			{
				// [로그추가 : 황진성 2007. 10. 15] // 캐릭터 사망. 
				// param1 == 죽인놈의 소속 길드 인덱스 추가
				SendLog_WriteLogToDB(LOGCOMMAND_TYPE_CHARACTOR, LOGCOMMAND_INDEX_CHARLDIE_GUILDWAR, 
					0, pclenemychar, NULL, pclchar->pclCI->GetPersonID(), 0, 0, 0, 0, 0, 0, 0, 0, 0, 
					pclchar->pclCI->clBI.clGuildRank.siGuildUnitIndex, 0, 0, 0, 0, NULL, NULL);
			}	
		}
	}

	// 길드맵 획득방식 변경 길드전
	SI32 siMapIndex = GetHuntmapGuildWarFieldIndexFromMapIndex( pclchar->GetMapIndex() );
	if ( siMapIndex >= 0 && siMapIndex < GUILDWAR_FIELDNUM )
	{
		pclGuildHuntMapManager->addKillPoint(myid,enemyid);
	}

	//-----------------------------
	// PVP 중 사망
	//----------------------------
	if(siEnablePVP == 1)
	{
		if( IsPC( pclchar->GetCharUnique() ) )
		{
			if( pclchar->clInnerStatus.clEnemyType.IsType( ENEMYTYPE_PVP  ) )
			{
				if( pclMapManager->IsPkMap( pclchar->GetMapIndex() ) )
				{
					if( pclenemychar->clInnerStatus.clEnemyType.IsType( ENEMYTYPE_PVP ) )
					{
						if( pclMapManager->IsPkMap( pclenemychar->GetMapIndex() ) )
						{	
							// 아이템을 뺏고 결과를 올려주자, 그리고 PVP상태를 없애주자
							if( 1 == siLoseItem )
							{
								// 아이템을 뺏고 결과를 올려주자, 그리고 PVP상태를 없애주자
								SI32 sendItemPosition = -1;
								SI32 randValue = rand() % 100;

								if ( IsCountrySwitch( Switch_PKHuntMap_Change_Info )) 
								{
									// 축복받은 부활약이 있으면 아이템은 깨지지 않는다 
									if( pclenemychar->pclCI->clCharItem.GetItemNum( ITEMUNIQUE(2731 ) ) > 0 )
									{
									}
									else
									{
										if		( 1 > randValue )	{		sendItemPosition = PERSONITEM_WEAPON1;			}
										else if	( 2 > randValue )	{		sendItemPosition = PERSONITEM_WEAPON2;			}
										else if ( 7 > randValue )	{		sendItemPosition = PERSONITEM_HELMET;			}
										else if ( 12 > randValue )	{		sendItemPosition = PERSONITEM_ARMOUR;			}
										else if ( 31 > randValue )	{		sendItemPosition = PERSONITEM_BELT;				}
										else if ( 50 > randValue )	{		sendItemPosition = PERSONITEM_SHOES;			}
										else								sendItemPosition = -1;

										if( -1 != sendItemPosition )
										{
											if( 0 >= pclenemychar->pclCI->clCharItem.clItem[sendItemPosition].siUnique )
											{
												sendItemPosition = -1;
											}
										}

										if( -1 < sendItemPosition )
										{
											cltItem clItem;
											clItem.Init();

											SI32 rtnvalue = -1;
											SI16 findIndex = -1;
											if ( true == pclchar->CanAddInv( INVMODE_ALL, &pclenemychar->pclCI->clCharItem.clItem[sendItemPosition], &findIndex, &clItem, &rtnvalue ) )
											{
												bool destoryItem = false;
												SI32 destoryValue = rand() % 100;

												if( 20 > destoryValue )
												{

													destoryItem = true;
												}

												sDBRequest_MoveItemPerson clMsg( pclchar->GetID(), pclenemychar->pclCI->GetPersonID(), 
													sendItemPosition, &pclenemychar->pclCI->clCharItem.clItem[sendItemPosition],
													false, (TCHAR*)pclenemychar->GetName(), pclchar->pclCI->GetPersonID(), findIndex, &clItem, (TCHAR*)pclchar->GetName(), 
													MOVEITEMPERSON_PVP, destoryItem );

												pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);

											}
										}

									}
								}
								else
								{
									if		( 5 > randValue )	{		sendItemPosition = PERSONITEM_WEAPON1;			}
									else if	( 10 > randValue )	{		sendItemPosition = PERSONITEM_WEAPON2;			}
									else if ( 15 > randValue )	{		sendItemPosition = PERSONITEM_HELMET;			}
									else if ( 20 > randValue )	{		sendItemPosition = PERSONITEM_ARMOUR;			}
									else if ( 45 > randValue )	{		sendItemPosition = PERSONITEM_BELT;				}
									else if ( 70 > randValue )	{		sendItemPosition = PERSONITEM_SHOES;			}
									else								sendItemPosition = -1;

									if( -1 != sendItemPosition )
									{
										if( 0 >= pclenemychar->pclCI->clCharItem.clItem[sendItemPosition].siUnique )
										{
											sendItemPosition = -1;
										}
									}

									if( -1 < sendItemPosition )
									{
										cltItem clItem;
										clItem.Init();

										SI32 rtnvalue = -1;
										SI16 findIndex = -1;
										if ( true == pclchar->CanAddInv( INVMODE_ALL, &pclenemychar->pclCI->clCharItem.clItem[sendItemPosition], &findIndex, &clItem, &rtnvalue ) )
										{
											bool destoryItem = false;
											SI32 destoryValue = rand() % 100;

											if( 20 > destoryValue )
											{

												destoryItem = true;
											}

											sDBRequest_MoveItemPerson clMsg( pclchar->GetID(), pclenemychar->pclCI->GetPersonID(), 
												sendItemPosition, &pclenemychar->pclCI->clCharItem.clItem[sendItemPosition],
												false, (TCHAR*)pclenemychar->GetName(), pclchar->pclCI->GetPersonID(), findIndex, &clItem, (TCHAR*)pclchar->GetName(), 
												MOVEITEMPERSON_PVP, destoryItem );

											pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);

										}
									}


								}
							}

							//[진성] pk결과 보고. => 2008-5-9
							sDBRequest_PVPResult clMsg( pclchar->GetID(), pclenemychar->GetID(), pclchar->pclCI->GetPersonID(), pclenemychar->pclCI->GetPersonID(), (TCHAR*)pclchar->GetName(), (TCHAR*)pclenemychar->GetName() );
							pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);

							pclenemychar->siKilledMode = KILLEDMODE_PVP;

							pclchar->clInnerStatus.clEnemyType.siEnemyType = ENEMYTYPE_ALLNPC | ENEMYTYPE_VILLAGE | ENEMYTYPE_GUILDWAR;
							pclenemychar->clInnerStatus.clEnemyType.siEnemyType = ENEMYTYPE_ALLNPC | ENEMYTYPE_VILLAGE | ENEMYTYPE_GUILDWAR;

							// LEEKH MODIFY 2007.10.25
							if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
							{
								// [로그추가 : 황진성 2007. 9. 11] // 캐릭터 사망 (pk)
								SendLog_WriteLogToDB(LOGCOMMAND_TYPE_CHARACTOR, LOGCOMMAND_INDEX_CHARLPK, 
									0, pclenemychar, NULL, pclchar->pclCI->GetPersonID(), 0, 0, 0, 0, 0, 0, 0, 0, 0, 
									0, 0, 0, 0, 0, NULL, NULL);
							}
						}
					}
				}
			}
		}
	}
	//-----------------------------
	// 1:1 Pvp중 사망
	//-----------------------------
	{
		SI32 siMyParent		=	pclchar->GetSummonParentID()		;
		SI32 siEnemyParent	=	pclenemychar->GetSummonParentID()	;

		// siMyParent 가 있으면 소환수가 죽인거다
		if ( siMyParent == 0 )		siMyParent		=	myid		;
		if ( siEnemyParent == 0 )	siEnemyParent	=	enemyid		;

		cltCharServer* pclMyChar = pclCM->GetCharServer( siMyParent )		;		if(pclMyChar == NULL)		return;
		cltCharServer* pclEnemyChar = pclCM->GetCharServer( siEnemyParent )	;		if(pclEnemyChar == NULL)	return;

		// 전투를 pvp꺼를 써서 하나더 셋팅한 리그 퍼슨아이디가 있는지 없는지를 체크 [ 성웅] 2008.11.23
		if ( pclCM->CR[siMyParent]->siEnemUnique > 0 &&  pclCM->CR[siMyParent]->GetPVP_LeagueEnemyPersonID() <= 0) 
		{
			pclenemychar->siKilledMode = KILLEDMODE_PVP;

			// 죽었을 떄 나의 EnemyID를 초기화 한다.
			// 클라이언트로 끝났음을 알리고 클라이언트의 enemyID도 초기화 한다.
			if( pclClient->IsCountrySwitch( Switch_ValenTineWar ) )
			{
				//발렌타인데이 전쟁 관련 체크, 커플이나 솔로라고 정했고, 아이템을 받지 않았을 경우에만 준다.
				SI32 siCouple = pclMyChar->pclValentineWar->GetCouple()	;// 이긴 사람의 커플 여부
				SI32 siLoser = pclEnemyChar->pclValentineWar->GetCouple()	;// 진 사람의 커플 여부
				bool bGetPrize = pclMyChar->pclValentineWar->GetPrize()	;	// 이긴사람이 아이템을 받았었는지

				if( abs(pclMyChar->pclCI->clIP.GetLevel() - pclenemychar->pclCI->clIP.GetLevel() ) < 11
					&& siCouple != 0 && siLoser != 0
					&& siCouple != siLoser 
					&& bGetPrize == false )
				{
					SI32 siRewardItemUnique	=	ITEMUNIQUE(2781) ;
					if( siRewardItemUnique ) 
					{
						cltItem clItem;
						SI16 brareswitch = 0;
						if ( pclClient->pclItemManager->MakeRandItemUnique( siRewardItemUnique,&clItem,0,0,&brareswitch,0,0,0,0,0) == true ) 
						{
							clItem.siItemNum = 1;
							SI32 rtnvalue = 0;
							if(pclMyChar->InputItem(pclClient->pclItemManager, &clItem, INPUTITEMPERSON_OPTION_VALENTINEWAR_PRIZE, &rtnvalue))
							{
								// 아이템을 주고 줬다고 셋팅 해준다.
								sDBRequest_Valantine_Get_Prize clMsg( pclMyChar->pclCI->GetPersonID(), true );
								pclGameDBProcess->SendMsg( DBSELECT_SYSTEM, (sPacketHeader *)&clMsg);
							}
							else
							{
								if(rtnvalue == CANADDINV_FAIL_FULL)
								{
									sDBRequest_PostOfficeMasterSendItem clMsg( INPUTITEMPERSON_OPTION_VALENTINEWAR_PRIZE, 0, (TCHAR*)pclMyChar->GetName(), &clItem, &pclTime->clDate );
									pclGameDBProcess->SendMsg( DBSELECT_SYSTEM, (sPacketHeader *)&clMsg);
									// 아이템을 주고 줬다고 셋팅 해준다.
									sDBRequest_Valantine_Get_Prize clMsg2( pclMyChar->pclCI->GetPersonID(), true );
									pclGameDBProcess->SendMsg( DBSELECT_SYSTEM, (sPacketHeader *)&clMsg2);
								}
							}
						}		
					}
				}
			}
			{//승리자
				// 개인간 pvp가 끝나면서 각종 중독상태 제거
				pclMyChar->DeletePoison();						// 중독 상태를 삭제한다.
				pclMyChar->DeletePoisonSetItemEffect();			// 중독 상태를 삭제한다.
				
				pclMyChar->DeleteIce();							// 얼음 상태를 삭제한다.
				pclMyChar->DeleteCurseAttack();					// 공격력이 저하 상태를 삭제한다.
				pclMyChar->DeleteCurseCritical();				// Critical 확률이 낮아지는 상태를 삭제한다.
				pclMyChar->DeleteCurseDefense();				// 방어력이 낮아지는 상태를 삭제한다.
				pclMyChar->DeleteCurseMovespeed();				// 이동속도 저하 상태를 삭제한다.
				pclMyChar->DeleteMagicFire();					// 불에 타는 상태를 삭제한다.
				pclMyChar->DeleteFrozen();						// 보스 얼음 상태를 삭제한다.
				pclMyChar->DeleteCurseEffect();
			}
			{	// 패배자
				// 개인간 pvp가 끝나면서 각종 중독상태 제거
				pclEnemyChar->DeletePoison();					// 중독 상태를 삭제한다.
				pclEnemyChar->DeletePoisonSetItemEffect();		// 중독 상태를 삭제한다.
				pclEnemyChar->DeleteIce();						// 얼음 상태를 삭제한다.
				pclEnemyChar->DeleteCurseAttack();				// 공격력이 저하 상태를 삭제한다.
				pclEnemyChar->DeleteCurseCritical();			// Critical 확률이 낮아지는 상태를 삭제한다.
				pclEnemyChar->DeleteCurseDefense();				// 방어력이 낮아지는 상태를 삭제한다.
				pclEnemyChar->DeleteCurseMovespeed();			// 이동속도 저하 상태를 삭제한다.
				pclEnemyChar->DeleteMagicFire();				// 불에 타는 상태를 삭제한다.
				pclEnemyChar->DeleteFrozen();					// 보스 얼음 상태를 삭제한다.
				pclEnemyChar->DeleteCurseEffect();
			}
			pclCM->CR[ siMyParent ]->siEnemUnique		=	0 ;
			pclCM->CR[ siEnemyParent ]->siEnemUnique	=	-100 ;

			// 나에게 끝났음을 알려준다.
			cltGameMsgResponse_PVP_Request clMe( END_PVP ,WIN_PVP, pclMyChar->GetCharUnique(), TEXT("") );  //  적을 죽였다
			cltMsg clMsg( GAMEMSG_RESPONSE_PVP_FILED, sizeof(clMe), (BYTE*)&clMe );
			pclMyChar->SendNeighbourMsg(&clMsg, true);

			pclMyChar->IncQuestPara( QUEST_CLEAR_COND_PVPWIN, 0, 1 );

			// 상대방에게 끝났음을 알려준다.
			cltGameMsgResponse_PVP_Request clEnemy( END_PVP , LOSE_PVP, pclEnemyChar->GetCharUnique(), TEXT("") ); // 적에게는 너 졌다고 알린다.
			cltMsg clMsg2( GAMEMSG_RESPONSE_PVP_FILED, sizeof(clEnemy), (BYTE*)&clEnemy );
			pclEnemyChar->SendNeighbourMsg(&clMsg2, true);

			if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
			{
				// [로그추가 : 손서웅ㅇ 2008. 10. 29] // pvp 패배
				// para1	:	승리자 personID // 죽인놈
				// para2	:	패배자 personID	// 죽은놈
				// PCK : 수정 - PersonID로 캐릭터이름을 구해서 좀더 보기 쉽게한다. (09.07.31)

				TCHAR szCharName[100];
				ZeroMemory(szCharName, sizeof(szCharName));
				StringCchPrintf(szCharName, sizeof(szCharName), TEXT("WinnerChar:%s, LoserChar:%s"), pclMyChar->GetName(), pclEnemyChar->GetName());

				SendLog_WriteLogToDB(LOGCOMMAND_TYPE_PERSONALPVP, LOGCOMMAND_INDEX_PERSONALPVP_END_DIE, 
					0,pclMyChar,NULL , 0, 0, 0, NULL, 0, 0, 0, 0, 0, 0, 
					pclMyChar->pclCI->GetPersonID(), 
					pclEnemyChar->pclCI->GetPersonID(), 0, 0, 0, szCharName, NULL);
			}
			{
				// 서버에 남긴다.
				TCHAR fileNameBuffer[256];
				//슬롯에 찍을 시간을 텍스트로 완성한다.
				TCHAR szTime[128];
				TCHAR szBuffer0[128];
				SYSTEMTIME stTimeInfo;
				GetLocalTime(&stTimeInfo);
				StringCchPrintf( szTime,  sizeof(szTime) ,TEXT("%d:%d:%d:%d:%d"),stTimeInfo.wMonth,stTimeInfo.wDay,stTimeInfo.wHour,stTimeInfo.wMinute,stTimeInfo.wSecond);
				StringCchPrintf(szBuffer0, sizeof(szBuffer0),"%s",&szTime) ;
				StringCchPrintf( fileNameBuffer , sizeof(fileNameBuffer) , TEXT("Config\\PVPLog_%04d%02d%02d.log") 
					, stTimeInfo.wYear , stTimeInfo.wMonth , stTimeInfo.wDay );
				pclClient->pclLog->FilePrint(TEXT(fileNameBuffer), 
					TEXT(" PVPEND_DIE \tWINOPERSON : \t[%s]\t \t LOSEPERSON2 : \t[%s]\t Time"),
					pclMyChar->GetName(),pclEnemyChar->GetName(),szBuffer0 );
			}
		}
	}
	//-----------------------------
	// PVP_League 도중 사망.
	//-----------------------------
	{
		SI32 siWinParent		=	pclchar->GetSummonParentID()	;
		SI32 siLoseParent		=	pclenemychar->GetSummonParentID()	;

		// siMyParent 가 있으면 소환수가 죽인거다
		if ( siWinParent == 0 )		siWinParent		=	myid		;// 승리 한사람
		if ( siLoseParent == 0 )	siLoseParent	=	enemyid		;	// 죽은사람

		cltCharServer* pclWinnerChar = pclCM->GetCharServer( siWinParent )		;		if(pclWinnerChar == NULL)		return;
		cltCharServer* pclLoseChar = pclCM->GetCharServer( siLoseParent )	;		if(pclLoseChar == NULL)	return;

		if ( pclWinnerChar->GetPVP_LeagueEnemyPersonID() > 0 && pclWinnerChar->siEnemUnique > 0 &&
			pclLoseChar->GetPVP_LeagueEnemyPersonID() > 0 && pclLoseChar->siEnemUnique > 0) // 무한대전
		{	
			pclLoseChar->siKilledMode = KILLEDMODE_PVP	;
		}
	}
}

void cltServer::DeadChar_NPC(SI32 myid, SI32 enemyid)
{
	// 전투병을 처치한 것이라면 , 
	cltCharServer* pclchar = (cltCharServer*)pclCM->CR[enemyid];	// 죽은놈. 
	cltCharServer* pclenemychar = (cltCharServer*)pclCM->CR[myid];	// 죽인놈. (살인자)  

	pclchar->m_clCharCurseEffect.Dead( enemyid );

	//////////////////////////////////////////////////////////////////////////
	// [기형] 배틀로얄 몹이 죽었을 때
	if(pclchar->clIdentity.siIdentity == IDENTITY_BATTLEROYAL_MONSTER)
	{
		CBattleRoyal* pclBattleRoyal = m_pclBattleRoyalMgr->GetBattleRoyal(pclenemychar);
		if( NULL != pclBattleRoyal)
			pclBattleRoyal->DieMob();
	}

	//////////////////////////////////////////////////////////////////////////
	// [지연] 복불복 이벤트 몹을 죽였을때
	if ( pclClient->IsCountrySwitch(Switch_BokBulBok))	// 복불복 스위치
	{
		if ( pclenemychar != NULL )
		{
			if ( pclchar->clIdentity.siIdentity == IDENTITY_BOKBULBOK_ONEKILLMONSTER )
			{
				cltCharServer* pclChar = pclCM->GetCharServer( myid );

				if ( pclChar != NULL )
				{
					pclClient->m_pServer_BBBMgr->StageFail( myid );
				}
			}
		}
	}

	if ( pclClient->IsCountrySwitch(Switch_BokBulBok))	// 복불복 스위치
	{
		if ( pclenemychar != NULL )
		{
			if ( pclchar->clIdentity.siIdentity == IDENTITY_BOKBULBOK_KEYMONSTER )
			{
				cltCharServer* pclChar = pclCM->GetCharServer( myid );

				if ( pclChar != NULL )
				{
					pclClient->m_pServer_BBBMgr->StageClear( myid );
				}
			}
		}
	}

	//////////////////////////////////////////////////////////////////////////


	//[진성] 마을 이벤트-몬스터 습격 중이면.
	if(pclMonsterAttackEvent->IsEvent())
	{
		if(pclchar->clIdentity.siIdentity == IDENTITY_MONSTER_ATTACK_EVENT )
		{
			pclMonsterAttackEvent->MonsterDie(pclchar->GetKind());
		}
	}

	//--------------------------------------------------------------------------------------------------------------------------------
	// 공성전중 수호상이 죽으면
	//--------------------------------------------------------------------------------------------------------------------------------
	if(pclchar->clIdentity.siIdentity == IDENTITY_VILLAGE_KINGMONUMENT)
	{
		// 죽인 놈이 
		if(pclenemychar->pclCI->clBI.siHomeVillage)
		{
			// 서로 적대관계라면 
			if(pclVillageManager->IsAggressiveVillage( pclchar->pclCI->clBI.siHomeVillage, pclenemychar->pclCI->clBI.siHomeVillage))
			{
				// 승리 마을 사람들에게 경험치를 지급한다. 
				// 이긴 마을이 하위 마을이라면 경험치를 많이 주고 상위 마을이라면 경험치를 적게 준다. 
				SI32 rate = 0;
				SI32 expreason	= 0;

				/* 영진 [공성전] 마을 상,하위 시스템 폐지
				if(pclVillageManager->GetVillageRelation(pclenemychar->pclCI->clBI.siHomeVillage, pclchar->pclCI->clBI.siHomeVillage) == TRUE)
				{
					rate = 100;
					expreason	= GETEXP_REASON_VILLAGEWAR2;	//하위 마을 점령
				}
				else
				{
					rate = 300;
					expreason	= GETEXP_REASON_VILLAGEWAR1;	//상위마을 점령. 
				}*/

					// 석상 뽀갠사람한테 레벨 * 100 의 경험치를 줌.

					rate = 100;
				expreason	= GETEXP_REASON_VILLAGEWAR2;	//하위 마을 점령


				SI32 index	= 0;
				SI32 id		= 0;
				while(id = pclCM->GetCharOrder(index))
				{
					index++;

					if(pclCM->IsAlive(id) && IsPC(pclCM->CR[id]->GetCharUnique())
						&& pclCM->CR[id]->pclCI->clBI.siHomeVillage == pclenemychar->pclCI->clBI.siHomeVillage)
					{
						SI32 tempexp = pclCM->CR[id]->pclCI->clIP.GetLevel() * rate;

						cltCharServer* pclchar = (cltCharServer*)pclCM->CR[id];
						pclchar->IncreaseExp(tempexp, expreason);
					}
				}

				// 승패를 결정하고 마을점수, 자산 을 순위에 비례하여 빼앗아 온다.
				// 패배한 마을의 경우 패배한 날짜를 DB에 기록하여 둔다. (일정기간 보호를 위해)

				SetWarResult(pclenemychar->pclCI->clBI.siHomeVillage, pclchar->pclCI->clBI.siHomeVillage);

				// 공성 결과를 Log로 남긴다. 
				pclLog->FilePrint(TEXT("config\\VillageWar.log"), TEXT("Win :\t[%s]\tLose :\t[%s]"),
					pclVillageManager->GetName(pclenemychar->pclCI->clBI.siHomeVillage),
					pclVillageManager->GetName(pclchar->pclCI->clBI.siHomeVillage));

				// 승패에 따라 주식을 소각한다.
				//SetVillageWarLoss(pclenemychar->pclCI->clBI.siHomeVillage, pclchar->pclCI->clBI.siHomeVillage);

			}

		}

	}

	//--------------------------------------------------------------------------------------------------------------------------------
	// 흑의군단 전체이벤트
	//--------------------------------------------------------------------------------------------------------------------------------
	if( pclchar->siCharMode == CHARMODE_BLACKWAR )
	{
		if( pclBlackWarManager->IsBlackWar() )
		{
			cltCharServer *pclHostchar = pclenemychar->GetHostChar();

			SI32 siKind = pclchar->GetKind();
			SI16 VillageScore = 0;

			// [추가 : 황진성 2008. 1. 21 => 흑의 군단을 죽인 결과를 저장.]
			if(pclClient->IsCountrySwitch(Switch_BlackWarReform)					&&
				( BLACKARMY_MAP_STARTINDEX > pclHostchar->GetCurrentVillageUnique()	||
				BLACKARMY_MAP_STARTINDEX+MAX_BLACKARMY_MAP_CNT < pclHostchar->GetCurrentVillageUnique())	)
			{
				SI08 Score = 0;
				// 죽인 넘덜에 해당하는 점수 누적.
				if (siKind == pclClient->GetUniqueFromHash(TEXT("KIND_NEWCATAR")))
					Score = 1;
				else if (siKind == pclClient->GetUniqueFromHash(TEXT("KIND_NEWRIFLE")))
					Score = 2;
				else if (siKind == pclClient->GetUniqueFromHash(TEXT("KIND_NEWDAEGUM")))
					Score = 3;
				else if (siKind == pclClient->GetUniqueFromHash(TEXT("KIND_YELLOWSHABEL")))
					Score = 8;
				else if (siKind == pclClient->GetUniqueFromHash(TEXT("KIND_GREENSHABEL")))
					Score = 8;
				else if (siKind == pclClient->GetUniqueFromHash(TEXT("KIND_REDSHABEL")))
					Score = 8;
				else if (siKind == pclClient->GetUniqueFromHash(TEXT("KIND_BLUESHABEL")))
					Score = 8;
				else if (siKind == pclClient->GetUniqueFromHash(TEXT("KIND_BLACKBOSS")))
				{
					Score = 20;
					pclBlackWarManager->SetDieBoss( true );
				}

				pclHostchar->siBlackWarEnemyKillScore += Score;
				// 마을 주민의 전쟁기록 목록에 보일때 참조되는 변수이기에 흑군 점수를 더해준다. 
				pclHostchar->siVillageWarKillScore	= pclHostchar->siBlackWarEnemyKillScore;

				//[추가 : 황진성 2008. 1. 22 => 흑의 군단 진행중 마을 점수 증가.]
				if( 0 < pclHostchar->pclCI->clBI.siHomeVillage && MAX_VILLAGE_NUMBER > pclHostchar->pclCI->clBI.siHomeVillage)
				{
					cltServer* pclserver = (cltServer*)pclClient;
					pclserver->pclVillageManager->BlackWarScoreAdd(pclHostchar->pclCI->clBI.siHomeVillage, Score);

					VillageScore = pclserver->pclVillageManager->GetBlackWarVillageScore(pclHostchar->pclCI->clBI.siHomeVillage);
				}    

				// 죽인 마리수 +1 기록.
				++pclHostchar->siBlackWarEnemyKillNumber;

				SI32 Index = 0;
				SI32 id    = 0;
				while(id = pclClient->pclCM->GetCharOrder(Index))
				{
					Index++;

					// 사용자인 컴에게만 전달한다. 
					if( true == pclClient->pclCM->CR[id]->IsValidConnection() )
					{
						if( pclClient->pclCM->CR[id]->pclCI->clBI.siHomeVillage == pclHostchar->pclCI->clBI.siHomeVillage )
						{
							//--------------------------------
							// 클라이언트로 전과를 알려준다. 
							//--------------------------------
							cltGameMsgResponse_WarKill clinfo(0, pclHostchar->siBlackWarEnemyKillNumber, pclHostchar->siBlackWarKilledNumber, 0, pclClient->pclCM->CR[id]->siBlackWarEnemyKillScore, VillageScore);
							cltMsg clMsg(GAMEMSG_RESPONSE_WARKILL, sizeof(clinfo), (BYTE*)&clinfo);
							pclClient->pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
						}
					}
				}
			}
			// 이전에 사용중인 방법.
			else
			{
				if (siKind == pclClient->GetUniqueFromHash(TEXT("KIND_NEWCATAR")))
					pclHostchar->siBlackWarEnemyKillNumber += 1;
				else if (siKind == pclClient->GetUniqueFromHash(TEXT("KIND_NEWRIFLE")))
					pclHostchar->siBlackWarEnemyKillNumber += 2;
				else if (siKind == pclClient->GetUniqueFromHash(TEXT("KIND_NEWDAEGUM")))
					pclHostchar->siBlackWarEnemyKillNumber += 3;
				else if (siKind == pclClient->GetUniqueFromHash(TEXT("KIND_YELLOWSHABEL")))
					pclHostchar->siBlackWarEnemyKillNumber += 5;
				else if (siKind == pclClient->GetUniqueFromHash(TEXT("KIND_GREENSHABEL")))
					pclHostchar->siBlackWarEnemyKillNumber += 5;
				else if (siKind == pclClient->GetUniqueFromHash(TEXT("KIND_REDSHABEL")))
					pclHostchar->siBlackWarEnemyKillNumber += 5;
				else if (siKind == pclClient->GetUniqueFromHash(TEXT("KIND_BLUESHABEL")))
					pclHostchar->siBlackWarEnemyKillNumber += 5;
				else if (siKind == pclClient->GetUniqueFromHash(TEXT("KIND_BLACKBOSS")))
				{
					pclHostchar->siBlackWarEnemyKillNumber += 20;
					pclBlackWarManager->SetDieBoss( true );
				}

				//--------------------------------
				// 클라이언트로 전과를 알려준다. 
				//--------------------------------
				cltGameMsgResponse_WarKill clinfo(0, pclHostchar->siBlackWarEnemyKillNumber, pclHostchar->siBlackWarKilledNumber, 0, pclHostchar->siBlackWarEnemyKillScore, VillageScore);
				cltMsg clMsg(GAMEMSG_RESPONSE_WARKILL, sizeof(clinfo), (BYTE*)&clinfo);
				pclCM->CR[pclHostchar->GetID()]->SendNetMsg((sPacketHeader*)&clMsg);
			}	

			pclBlackWarManager->IncreaseKillBlackUnitNumber();
		}
	}

	//--------------------------------------------------------------------------------------------------------------------------------
	// 대왕 구리댁
	//--------------------------------------------------------------------------------------------------------------------------------
	if ( pclchar->siCharMode == CHARMODE_KINGGURI )
	{
		if(pclClient->IsCountrySwitch(Switch_RaccoonEvent_Change_Goonzu))// [성웅] 라쿤이벤트 군주 이벤트로 변경 2009-10
		{
			cltCharServer *pclHostchar = pclenemychar->GetHostChar();
			pclGoonzuKingGuriEventManager->SetDieKingGuri( true, pclchar->GetID(), pclHostchar->GetID() );
		}
		else
		{
			cltCharServer *pclHostchar = pclenemychar->GetHostChar();
			pclKingGuriEventManager->SetDieKingGuri( true, pclchar->GetID(), pclHostchar->GetID() );
		}
	}

	//--------------------------------------------------------------------------------------------------------------------------------
	//[추가 : 황진성 2007. 11. 29 산타라쿤 다이.]
	//--------------------------------------------------------------------------------------------------------------------------------
	if ( pclchar->siCharMode == CHARMODE_SANTARACCOON )
	{
		cltCharServer *pclHostchar = pclenemychar->GetHostChar();
		pclSantaRaccoon->SetDieSantaRaccoon( true, pclchar->GetID(), pclHostchar->GetID() );
	}

	//--------------------------------------------------------------------------------------------------------------------------------
	// 모후모후 이벤트 몬스터가 죽었다.
	//--------------------------------------------------------------------------------------------------------------------------------
	if(	pclchar->clIdentity.siIdentity == IDENTITY_MOFUMOFUEVENT	|| 
		pclchar->GetKind() == KIND_MAMARACCOON						||
		pclchar->GetKind() == KIND_MAGICALRACCOON					||
		pclchar->GetKind() == KIND_RACCOONLORD						||
		pclchar->GetKind() == KIND_RACCOONTOWER						||
		pclchar->GetKind() == KIND_ZORRORACCOON)
	{
		pclMofuMofuEvent->KilledEnemy(	pclchar->clIdentity.clMofoMofuEvent.siField, 
			pclchar->clIdentity.clMofoMofuEvent.siIndex, 
			enemyid );
	}

	//--------------------------------------------------------------------------------------------------------------------------------
	// [진성] 추석 이벤트. 달이 죽음. => 2008-8-7
	//--------------------------------------------------------------------------------------------------------------------------------
	if (pclchar->GetKind() ==  pclClient->GetUniqueFromHash(TEXT("KIND_MOON")) )
	{
		cltServer*	pclserver = (cltServer*)pclClient;

		cltInnerStatusInfo	clinnerstatus;
		clinnerstatus.clEnemyType.Set( ENEMYTYPE_ALLPC, 0 );	// 모든 PC와 우호적, 사람과는 적대적. 

		SI32 siID				= 0;
		SI32 siCharKind			= pclClient->GetUniqueFromHash(TEXT("KIND_COSMOSRABBIT2"));
		SI16 siVillageUnique	= pclchar->GetCurrentVillageUnique();
		SI32 siLevel			= 1;

		for(SI32 Count = 0; Count < 100; ++Count)
		{
			siID = pclserver->SetNPCChar(	siCharKind, 
				siVillageUnique, 
				NULL, 
				siLevel, 
				0, 
				pclchar->GetX(), 
				pclchar->GetY(), 
				0, 
				NPCITEMMODE_NORMAL, 
				&clinnerstatus );	

			// 시간 제한이 있는 몬스터 설정			
			if( pclserver->pclCM->IsValidID( siID ) )
			{
				((cltCharServer*)pclserver->pclCM->CR[siID])->dwTimerClock	= pclserver->CurrentClock;
			}
		}
	}

	//--------------------------------------------------------------------------------------------------------------------------------
	// 인던 몬스터가 죽음
	//--------------------------------------------------------------------------------------------------------------------------------
	if(pclchar->clIdentity.siIdentity == IDENTITY_INSTANCEMAP)
	{
		SI32 siParentMapIndex = pclClient->pclMapManager->GetParentMap( pclchar->GetMapIndex() );
		cltMapCommon* pclMap = pclClient->pclMapManager->GetMapPointer( siParentMapIndex );
		if( pclMap != NULL )
		{
			SI32 siMonsterScore = pclchar->clIdentity.clInstanceMap.siScore;

			pclMap->m_siInstanceMapScore += siMonsterScore;

			// 보너스 몬스터는 킬 카운트에 포함시키지 않는다
			if ( pclchar->GetKind() != pclClient->GetUniqueFromHash("KIND_PTIMEBONUS" ) )
			{
				if(pclenemychar->siQuestCount > 0)	{	pclMap->m_siKillCount += pclenemychar->siQuestCount;		}
				else								{	pclMap->m_siKillCount++;							}
			}

			// [영훈] 20404 - 젠 몬스터 추가 : 리젠 몬스터를 죽인 순간 몬스터 리젠
			if ( pclchar->GetKind() == pclClient->GetUniqueFromHash("KIND_GMONBONUS" ) )
			{
				SI32 siMapIndex = pclchar->GetMapIndex();

				pclNPCManager->MakeGuildRegenerationNPC( siMapIndex );
			}

			// 서버이벤트 인던 점수를 누적한다
			if ( INSTANCEMAP_TYPE_SERVER == pclClient->pclMapManager->GetInstanceMapType(siParentMapIndex) )
			{
				if ( pclEventTimeManager->InEventTime(TEXT( "GoonzuDay_Week" ), &sTime) )
				{
					// 개인점수 누적
					pclenemychar->siServerEvent_IndunScore += siMonsterScore;

					if ( IsCountrySwitch(Switch_Server_Event) )
					{
						pclServerEventManager->AddInstanceMapScore( siMonsterScore );
					}
				}
				else
				{
					pclMap->m_siInstanceMapScore = 0;
				}
			}

#ifdef _DEBUG
			pclClient->pclLog->FilePrint( TEXT("config\\regenerationNPC.txt"), TEXT("%d\t%d\t1\t1\t"), pclchar->GetMapIndex(), pclchar->GetKind() );
#endif


		}
	}

	//--------------------------------------------------------------------------------------------------------------------------------
	// 흑의 군단
	//--------------------------------------------------------------------------------------------------------------------------------
	if(pclchar->clIdentity.siIdentity == IDENTITY_BLACKARMY)
	{
		if (pclBlackArmyManager->bWarSwitch)
		{
			cltCharServer *pclHostchar = pclenemychar->GetHostChar();

			SI32 warmapindex = pclchar->clIdentity.clBlackArmyInfo.siWarMapIndex;
			SI32 warunitindex = pclBlackArmyManager->FindWarUnitIndex(pclchar->GetKind());

			if (warunitindex >= 0 || warmapindex >= 0)
			{
				if (pclBlackArmyManager->clWarMap[warmapindex].clBlackArmyUnit[warunitindex].siCurNum > 0)
				{
					pclBlackArmyManager->clWarMap[warmapindex].clBlackArmyUnit[warunitindex].siCurNum--;
				}

				if (pclBlackArmyManager->clWarMap[warmapindex].siMapCurUnitNum > 0)
				{
					pclBlackArmyManager->clWarMap[warmapindex].siMapCurUnitNum--;
				}

				SI32 siKind = pclchar->GetKind();

				if (siKind == pclClient->GetUniqueFromHash(TEXT("KIND_CATAR")))
					pclHostchar->siWarEnemyKillNumber += 1;
				else if (siKind == pclClient->GetUniqueFromHash(TEXT("KIND_RIFLE")))
					pclHostchar->siWarEnemyKillNumber += 2;
				else if (siKind == pclClient->GetUniqueFromHash(TEXT("KIND_DAEGUM")))
					pclHostchar->siWarEnemyKillNumber += 3;
				else if (siKind == pclClient->GetUniqueFromHash(TEXT("KIND_BLACKBOSS")))
				{
					pclHostchar->siWarEnemyKillNumber += 20;
					pclBlackArmyManager->clWarMap[warmapindex].siWarcmd = WAR_CMD_WIN;
				}

				//--------------------------------
				// 클라이언트로 전과를 알려준다. 
				//--------------------------------
				cltGameMsgResponse_WarKill clinfo(1, pclHostchar->siWarEnemyKillNumber, pclHostchar->siWarKilledNumber, pclHostchar->siBlackArmyWarMapIndex);
				cltMsg clMsg(GAMEMSG_RESPONSE_WARKILL, sizeof(clinfo), (BYTE*)&clinfo);
				pclCM->CR[pclHostchar->GetID()]->SendNetMsg((sPacketHeader*)&clMsg);

			}
		}
	}
	//--------------------------------------------------------------------------------------------------------------------------------
	// 보물찾기 이벤트 유령NPC. 영웅유령NPC 죽음
	//--------------------------------------------------------------------------------------------------------------------------------
	if ( pclchar->siCharMode == CHARMODE_TREASUREEVENT)
	{
		SI32 ChangedNPCKind	=	0	;
		SI32 siNPCCharKind = pclchar->GetKind()	;		// 죽은 엔피씨 카인드

		if ( siNPCCharKind == pclClient->GetUniqueFromHash( TEXT("KIND_CURSEHIRO") ) )
		{
			ChangedNPCKind	=	pclClient->GetUniqueFromHash( TEXT("KIND_BLESSINGHIRO") ) 	;
		}
		else if( siNPCCharKind == pclClient->GetUniqueFromHash( TEXT("KIND_TREASURE_DAEGUM1") ) )
		{
			ChangedNPCKind	=	pclClient->GetUniqueFromHash( TEXT("KIND_BLESSINGJEAN") ) 	;

		}
		else if( siNPCCharKind == pclClient->GetUniqueFromHash( TEXT("KIND_TREASURE_CATAR1") ) )
		{
			ChangedNPCKind	=	pclClient->GetUniqueFromHash( TEXT("KIND_BLESSINGMERIEL") ) 	;

		}
		else if( siNPCCharKind == pclClient->GetUniqueFromHash( TEXT("KIND_TREASURE_RIFLE1") ) )
		{
			ChangedNPCKind	=	pclClient->GetUniqueFromHash( TEXT("KIND_BLESSINGCLARA") ) 	;

		}
		else if( siNPCCharKind == pclClient->GetUniqueFromHash( TEXT("KIND_TREASURE_DAEGUM2") ) )
		{
			ChangedNPCKind	=	pclClient->GetUniqueFromHash( TEXT("KIND_BLESSINGSPARKY") ) 	;

		}
		else if( siNPCCharKind == pclClient->GetUniqueFromHash( TEXT("KIND_TREASURE_CATAR2") ) )
		{
			ChangedNPCKind	=	pclClient->GetUniqueFromHash( TEXT("KIND_BLESSINGJEAN") ) ;
		}
		else if( siNPCCharKind == pclClient->GetUniqueFromHash( TEXT("KIND_TREASURE_RIFLE2") ) )
		{
			ChangedNPCKind	=	pclClient->GetUniqueFromHash( TEXT("KIND_BLESSINGCLARA") ) 	;

		}
		else if( siNPCCharKind == pclClient->GetUniqueFromHash( TEXT("KIND_TREASURE_REDSHABEL") ) )
		{
			ChangedNPCKind	=	pclClient->GetUniqueFromHash( TEXT("KIND_BLESSINGMERIEL") ) 	;

		}
		else if( siNPCCharKind == pclClient->GetUniqueFromHash( TEXT("KIND_TREASURE_BLUESHABEL") ) )
		{
			ChangedNPCKind	=	pclClient->GetUniqueFromHash( TEXT("KIND_BLESSINGSPARKY") ) 	;
		}
		SI32 siMonsterLevel	=	1	;
		SI32 siLocationX	=	pclchar->GetX()	;
		SI32 siLocationY	=	pclchar->GetY()	;
		SI32 siMapindex		=	pclchar->GetMapIndex()	;


		// 해당 위치에다가 비 전투적인 케릭터를 소환한다.

		SI32 id = SetNPCChar(ChangedNPCKind , VILLAGE_NONE, NULL, siMonsterLevel, 0, siLocationX, siLocationY, siMapindex, NPCITEMMODE_NONE, NULL );

		if(pclCM->IsValidID(id))
		{
			((cltCharServer*)pclCM->CR[id])->dwTimerClock = CurrentClock;
			SendServerResponseMsg(0, SRVAL_SUMMONEGG_USED, ChangedNPCKind, 0, pclchar->GetID());	// 메세지는 다른걸로 뽑아준다
		}
	}
}

bool cltServer::GiveBigItem_Event( SI32 myid, SI32 enemyid )
{
	if( pclClient->pclCM->IsValidID(myid) == false )		return false;
	if( pclClient->pclCM->IsValidID(enemyid) == false )		return false;

	bool bBigItemGet = false;

	//================================================
	//KHY - 0115 발렌타인데이 이벤트.
	//================================================
	NDate Today;
	Today.SetDate(sTime.wYear, sTime.wMonth, sTime.wDay );

	if ( Today.GetDateVary() >= clValentineEventStart.GetDateVary() &&
		Today.GetDateVary() <= clValentineEventEnd.GetDateVary() )
	{	
		// myid가 사람이고 수렵장, 초보자 사냥터가 아닐때
		if (IsPC(myid) && pclCM->CR[myid]->GetHuntVillageUnique() <= 0 && pclCM->CR[myid]->GetMapIndex() != MAPINDEX_BEGINNER1)
		{
			if (pclCM->CR[myid]->pclCI->clIP.GetLevel() <= 0) 
				return false;

			SI32 randvalue = rand() %  100;
			// 대박아이템 줌
			if (randvalue < siValentineAttack)
			{
				// 랜덤하게 대박 아이템을 준다. 
				UI32 randCardvalue = rand() %  3;

				SI32 biItmes[4] = {ITEMUNIQUE(13006),  //V카드 
					ITEMUNIQUE(13010),	//N1카드
					ITEMUNIQUE(13009)};//E1카드

				SI32 bigitemunique = biItmes[randCardvalue];

				if(bigitemunique)
				{
					// 대박 물품을 만들어낸다. 
					cltItem clItem;

					SI16 brareswitch = 0;
					if(pclClient->pclItemManager->MakeRandItemUnique( bigitemunique, &clItem, 0, 0, &brareswitch,
						0,0,0,0,0,0) == true)
					{
						clItem.siItemNum = 1;
						SI32 rtnvalue = 0;

						cltCharServer *pclchar = (cltCharServer*)pclCM->CR[myid];

						if(pclchar->InputItem(pclClient->pclItemManager, &clItem, INPUTITEMPERSON_OPTION_WAR, &rtnvalue))
						{
							// 대박아이템 나오면 로그로 남김
							//pclClient->pclLog->FilePrint(TEXT("Config\\WhiteDayEventItemDrop.Txt"), TEXT("RareItem Get User : \t[%s]\t / RareItem Unique, Num : \t[%d]\t[%d]"), pclCM->CR[myid]->GetName(), clItem.siUnique, clItem.siItemNum);
							bBigItemGet = true;
						}
						else
						{
							// 인벤에 빈자리가 없어서 추가할 수 없다. 
							if(rtnvalue == CANADDINV_FAIL_FULL)
							{
								SendServerResponseMsg( 0, SRVAL_ADDINV_FAIL_FULL,  clItem.siUnique, 0, pclchar->GetCharUnique());
							}
						}
					}
				}
			}
		}
	}

	//KHY - 0218 화이트데이 이벤트.
	if ( Today.GetDateVary() >= clWhiteEventStart.GetDateVary() &&
		Today.GetDateVary() <= clWhiteEventEnd.GetDateVary() )
	{	
		// myid가 사람이고 수렵장, 초보자 사냥터가 아닐때
		if (IsPC(myid) && pclCM->CR[myid]->GetHuntVillageUnique() <= 0 && pclCM->CR[myid]->GetMapIndex() != MAPINDEX_BEGINNER1)
		{
			if (pclCM->CR[myid]->pclCI->clIP.GetLevel() <= 0) 
				return false;

			SI32 randvalue = rand() %  100;
			//UI32 randCardvalue = rand() %  3;
			// 대박아이템 줌
			if (randvalue < siWhiteAttack) 
			{						
				SI32 bigitemunique =  ITEMUNIQUE(13016); //W 카드.

				if(bigitemunique)
				{
					// 대박 물품을 만들어낸다. 
					cltItem clItem;

					SI16 brareswitch = 0;
					if(pclClient->pclItemManager->MakeRandItemUnique( bigitemunique, &clItem, 0, 0, &brareswitch,
						0,0,0,0,0,0) == true)
					{
						clItem.siItemNum = 1;
						SI32 rtnvalue = 0;

						cltCharServer *pclchar = (cltCharServer*)pclCM->CR[myid];

						if(pclchar->InputItem(pclClient->pclItemManager, &clItem, INPUTITEMPERSON_OPTION_WAR, &rtnvalue))
						{
							// 대박아이템 나오면 로그로 남김
							//pclClient->pclLog->FilePrint(TEXT("Config\\WhiteDayEventItemDrop.Txt"), TEXT("RareItem Get User : \t[%s]\t / RareItem Unique, Num : \t[%d]\t[%d]"), pclCM->CR[myid]->GetName(), clItem.siUnique, clItem.siItemNum);
							bBigItemGet = true;
						}
						else
						{
							// 인벤에 빈자리가 없어서 추가할 수 없다. 
							if(rtnvalue == CANADDINV_FAIL_FULL)
							{
								SendServerResponseMsg( 0, SRVAL_ADDINV_FAIL_FULL,  clItem.siUnique, 0, pclchar->GetCharUnique());
							}
						}

					}

				}

			}
		}
	}
	//================================================
	// 신년이벤트 아이템 드롭 위치 수정 - by LEEKH 2007.12.21
	//================================================
	if(pclClient->pclEventTimeManager->InEventTime("NewYearEvent", &sTime) )
	{
		//[추가 : 황진성 2007. 12. 11 신년이벤트에 행운상자 지급.]
		cltItem clfromitem;
		clfromitem.Init();

		NewYearLuckyBox(clfromitem);

		if(clfromitem.siUnique > 0)
		{
			// 대박 물품을 만들어낸다. 
			cltItem clItem;

			SI16 brareswitch = 0;
			if(pclClient->pclItemManager->MakeRandItemUnique( clfromitem.siUnique, &clItem, 0, 0, &brareswitch,
				0,0,0,0,0,0) == true)
			{
				clItem.siItemNum = 1;

				SI32 rtnvalue = 0;

				cltCharServer *pclchar = (cltCharServer*)pclCM->CR[myid];

				if(pclchar->InputItem(pclClient->pclItemManager, &clItem, INPUTITEMPERSON_OPTION_WAR, &rtnvalue))
				{
					// 대박아이템 나오면 로그로 남김
					pclClient->pclLog->FilePrint(TEXT("Config\\NewYearItemInMonster.log"), TEXT("RareItem Get User : \t[%s]\t / RareItem Unique, Num : \t[%d]\t[%d]"), pclCM->CR[myid]->GetName(), clItem.siUnique, clItem.siItemNum);
					bBigItemGet = true;
				}
				else
				{
					// 인벤에 빈자리가 없어서 추가할 수 없다. 
					if(rtnvalue == CANADDINV_FAIL_FULL)
					{
						SendServerResponseMsg( 0, SRVAL_ADDINV_FAIL_FULL,  clItem.siUnique, 0, pclchar->GetCharUnique());
					}
				}
			}
		}
	}

	return bBigItemGet;
}

bool cltServer::GiveBigItem_New( SI32 myid, SI32 enemyid )
{
	if( pclClient->pclCM->IsValidID(myid) == false )		return false;
	if( pclClient->pclCM->IsValidID(enemyid) == false )		return false;

	SI32 siMyLevel		= pclClient->pclCM->CR[myid]->pclCI->clIP.GetLevel();
	SI32 siEnemyLevel	= pclClient->pclCM->CR[enemyid]->pclCI->clIP.GetLevel();

	// 레벨 25이상의 몬스터만 대박 아이템을 드랍한다.
	if( siEnemyLevel < 25 )			return false;

	// 레벨차가 31이상이면, 대박 아이템을 얻을 수 없다.
	if( TABS(siMyLevel - siEnemyLevel) >= 31 )		return false;

	bool bBigItemGet = false;

	SI32 siConstantA = 200;
	SI32 siRevisionA = 280;

	SI32 siLuk = pclCM->CR[myid]->clPB.clTotalBA.GetLuk();
	if(siLuk > 100)			siLuk = 100;		// 100이상은 없다.

	SI32 siLucky = ((float)siLuk * (float)( siConstantA - siLuk )) / ( (siRevisionA / 10.0f ) - (siLuk/6.0f) );
	SI32 siRand = rand() % 20000;

	SI32 bigitemunique = ITEMUNIQUE(13325);

	if( siRand < siLucky )
	{
		if(bigitemunique)
		{
			// 대박 물품을 만들어낸다. 
			cltItem clItem;

			SI16 brareswitch = 0;
			if(pclClient->pclItemManager->MakeRandItemUnique( bigitemunique, &clItem, 0, 0, &brareswitch,
				0,0,0,0,0,0) == true)
			{
				clItem.siItemNum = 1;
				SI32 rtnvalue = 0;

				cltCharServer *pclchar = (cltCharServer*)pclCM->CR[myid];

				if(pclchar->InputItem(pclClient->pclItemManager, &clItem, INPUTITEMPERSON_OPTION_WAR, &rtnvalue))
				{
					// 대박아이템 나오면 로그로 남김
					//pclClient->pclLog->FilePrint(TEXT("Config\\BigItemBoxItemDrop.Txt"), TEXT("RareItem Get User : \t[%s]\t / RareItem Unique, Num : \t[%d]\t[%d]"), pclCM->CR[myid]->GetName(), clItem.siUnique, clItem.siItemNum);
					bBigItemGet = true;
				}
				else
				{
					// 인벤에 빈자리가 없어서 추가할 수 없다. 
					if(rtnvalue == CANADDINV_FAIL_FULL)
					{
						SendServerResponseMsg( 0, SRVAL_ADDINV_FAIL_FULL,  clItem.siUnique, 0, pclchar->GetCharUnique());
					}
				}
			}
		}
	}

	return bBigItemGet;
}

bool cltServer::GiveBigItem( SI32 myid, SI32 enemyid )
{
	bool bBigItemGet = false;

	// GiveBigItem_Event 함수로 옮겨감.
	/*	//================================================
	//KHY - 0115 발렌타인데이 이벤트.
	//================================================
	NDate Today;
	Today.SetDate(sTime.wYear, sTime.wMonth, sTime.wDay );

	if ( Today.GetDateVary() >= clValentineEventStart.GetDateVary() &&
	Today.GetDateVary() <= clValentineEventEnd.GetDateVary() )
	{	
	// myid가 사람이고 수렵장, 초보자 사냥터가 아닐때
	if (IsPC(myid) && pclCM->CR[myid]->GetHuntVillageUnique() <= 0 && pclCM->CR[myid]->GetMapIndex() != MAPINDEX_BEGINNER1)
	{
	if (pclCM->CR[myid]->pclCI->clIP.GetLevel() <= 0) 
	return false;

	SI32 randvalue = rand() %  100;
	// 대박아이템 줌
	if (randvalue < siValentineAttack)
	{
	// 랜덤하게 대박 아이템을 준다. 
	UI32 randCardvalue = rand() %  3;

	SI32 biItmes[4] = {ITEMUNIQUE(13006),  //V카드 
	ITEMUNIQUE(13010),	//N1카드
	ITEMUNIQUE(13009)};//E1카드

	SI32 bigitemunique = biItmes[randCardvalue];

	if(bigitemunique)
	{
	// 대박 물품을 만들어낸다. 
	cltItem clItem;

	SI16 brareswitch = 0;
	if(pclClient->pclItemManager->MakeRandItemUnique( bigitemunique, &clItem, 0, 0, &brareswitch,
	0,0,0,0,0,0) == true)
	{
	clItem.siItemNum = 1;
	SI32 rtnvalue = 0;

	cltCharServer *pclchar = (cltCharServer*)pclCM->CR[myid];

	if(pclchar->InputItem(pclClient->pclItemManager, &clItem, INPUTITEMPERSON_OPTION_WAR, &rtnvalue))
	{
	// 대박아이템 나오면 로그로 남김
	//pclClient->pclLog->FilePrint(TEXT("Config\\WhiteDayEventItemDrop.Txt"), TEXT("RareItem Get User : \t[%s]\t / RareItem Unique, Num : \t[%d]\t[%d]"), pclCM->CR[myid]->GetName(), clItem.siUnique, clItem.siItemNum);
	bBigItemGet = true;
	}
	else
	{
	// 인벤에 빈자리가 없어서 추가할 수 없다. 
	if(rtnvalue == CANADDINV_FAIL_FULL)
	{
	SendServerResponseMsg( 0, SRVAL_ADDINV_FAIL_FULL,  clItem.siUnique, 0, pclchar->GetCharUnique());
	}
	}
	}
	}
	}
	}
	}

	//KHY - 0218 화이트데이 이벤트.
	if ( Today.GetDateVary() >= clWhiteEventStart.GetDateVary() &&
	Today.GetDateVary() <= clWhiteEventEnd.GetDateVary() )
	{	
	// myid가 사람이고 수렵장, 초보자 사냥터가 아닐때
	if (IsPC(myid) && pclCM->CR[myid]->GetHuntVillageUnique() <= 0 && pclCM->CR[myid]->GetMapIndex() != MAPINDEX_BEGINNER1)
	{
	if (pclCM->CR[myid]->pclCI->clIP.GetLevel() <= 0) 
	return false;

	SI32 randvalue = rand() %  100;
	UI32 randCardvalue = rand() %  3;
	// 대박아이템 줌
	if ((randvalue < siWhiteAttack) &&(randCardvalue == 1)) // 임의로 1/3의 확률을 줘서, 1/300을 만들어준다.
	{						
	SI32 bigitemunique =  ITEMUNIQUE(13016); //W 카드.

	if(bigitemunique)
	{
	// 대박 물품을 만들어낸다. 
	cltItem clItem;

	SI16 brareswitch = 0;
	if(pclClient->pclItemManager->MakeRandItemUnique( bigitemunique, &clItem, 0, 0, &brareswitch,
	0,0,0,0,0,0) == true)
	{
	clItem.siItemNum = 1;
	SI32 rtnvalue = 0;

	cltCharServer *pclchar = (cltCharServer*)pclCM->CR[myid];

	if(pclchar->InputItem(pclClient->pclItemManager, &clItem, INPUTITEMPERSON_OPTION_WAR, &rtnvalue))
	{
	// 대박아이템 나오면 로그로 남김
	//pclClient->pclLog->FilePrint(TEXT("Config\\WhiteDayEventItemDrop.Txt"), TEXT("RareItem Get User : \t[%s]\t / RareItem Unique, Num : \t[%d]\t[%d]"), pclCM->CR[myid]->GetName(), clItem.siUnique, clItem.siItemNum);
	bBigItemGet = true;
	}
	else
	{
	// 인벤에 빈자리가 없어서 추가할 수 없다. 
	if(rtnvalue == CANADDINV_FAIL_FULL)
	{
	SendServerResponseMsg( 0, SRVAL_ADDINV_FAIL_FULL,  clItem.siUnique, 0, pclchar->GetCharUnique());
	}
	}

	}

	}

	}
	}
	}
	//================================================
	// 신년이벤트 아이템 드롭 위치 수정 - by LEEKH 2007.12.21
	//================================================
	if(pclClient->pclEventTimeManager->InEventTime("NewYearEvent", &sTime) )
	{
	//[추가 : 황진성 2007. 12. 11 신년이벤트에 행운상자 지급.]
	cltItem clfromitem;
	clfromitem.Init();

	NewYearLuckyBox(clfromitem);

	if(clfromitem.siUnique > 0)
	{
	// 대박 물품을 만들어낸다. 
	cltItem clItem;

	SI16 brareswitch = 0;
	if(pclClient->pclItemManager->MakeRandItemUnique( clfromitem.siUnique, &clItem, 0, 0, &brareswitch,
	0,0,0,0,0,0) == true)
	{
	clItem.siItemNum = 1;

	SI32 rtnvalue = 0;

	cltCharServer *pclchar = (cltCharServer*)pclCM->CR[myid];

	if(pclchar->InputItem(pclClient->pclItemManager, &clItem, INPUTITEMPERSON_OPTION_WAR, &rtnvalue))
	{
	// 대박아이템 나오면 로그로 남김
	pclClient->pclLog->FilePrint(TEXT("Config\\NewYearItemInMonster.log"), TEXT("RareItem Get User : \t[%s]\t / RareItem Unique, Num : \t[%d]\t[%d]"), pclCM->CR[myid]->GetName(), clItem.siUnique, clItem.siItemNum);
	bBigItemGet = true;
	}
	else
	{
	// 인벤에 빈자리가 없어서 추가할 수 없다. 
	if(rtnvalue == CANADDINV_FAIL_FULL)
	{
	SendServerResponseMsg( 0, SRVAL_ADDINV_FAIL_FULL,  clItem.siUnique, 0, pclchar->GetCharUnique());
	}
	}
	}
	}
	}
	*/
	//================================================
	// 책 드롭
	//================================================
	// 중국 버전이고 myid가 사람인 경우만 
	// 국가 스위치로 변경
	if (pclClient->IsCountrySwitch(Switch_DropBookBox) && IsPC(myid) )
	{
		if (pclCM->CR[myid]->pclCI->clIP.GetLevel() <= 0) 
			return false;

		SI32 randvalue = rand() % 1000;
		// 대박아이템 줌
		cltServer* pclserver = (cltServer*)pclClient;
		if ( randvalue < pclserver->pclBookBox->GetDropRate() )
		{
			// 대박 물품을 만들어낸다. 
			cltItem clItem;

			clItem.siUnique = ITEMUNIQUE(7975); // 책상자 유니크
			// 피로방지 시스템 적용
			clItem.siItemNum = ((cltCharServer*)pclCM->CR[myid])->CalcTiredPoint( 1 );

			SI32 rtnvalue = 0;

			cltCharServer *pclchar = (cltCharServer*)pclCM->CR[myid];

			if( clItem.siItemNum > 0 && pclchar->InputItem(pclClient->pclItemManager, &clItem, INPUTITEMPERSON_OPTION_WAR, &rtnvalue))
			{
				// 대박아이템 나오면 로그로 남김
				//pclClient->pclLog->FilePrint(TEXT("Config\\BookDrop.Txt"), TEXT("BookBox Get User : \t[%s]\t / RareItem Unique, Num : \t[%d]\t[%d]"), pclCM->CR[myid]->GetName(), clItem.siUnique, clItem.siItemNum);
				//bBigItemGet = true;
			}
			else
			{
				// 인벤에 빈자리가 없어서 추가할 수 없다. 
				if(rtnvalue == CANADDINV_FAIL_FULL)
				{
					SendServerResponseMsg( 0, SRVAL_ADDINV_FAIL_FULL,  clItem.siUnique, 0, pclchar->GetCharUnique());
				}
			}
		}
	}

	// 보물/재료 상자 드롭.
	if ( pclClient->IsCountrySwitch(Switch_MaterialBox) && IsPC(myid) )
	{
		if (pclCM->CR[myid]->pclCI->clIP.GetLevel() <= 0) 
			return false;

		if (  (((rand() << 16) | rand()) % 1000000) == 1 )		// 1/1000000 확률
		{
			// 대박 물품을 만들어낸다. 
			cltItem clItem;

			// 중국은 보물상자만 준다.
			if ( rand() % 100 < 30 || pclClient->IsWhereServiceArea(ConstServiceArea_China) )
			{
				clItem.siUnique = ITEMUNIQUE(13314);	//보물상자
			}
			else
			{
				clItem.siUnique = ITEMUNIQUE(13315); //재료상자
			}

			// 피로방지 시스템 적용
			clItem.siItemNum = ((cltCharServer*)pclCM->CR[myid])->CalcTiredPoint( 1 );

			SI32 rtnvalue = 0;

			cltCharServer *pclchar = (cltCharServer*)pclCM->CR[myid];

			if( clItem.siItemNum > 0 && pclchar->InputItem(pclClient->pclItemManager, &clItem, INPUTITEMPERSON_OPTION_WAR, &rtnvalue))
			{
				// 대박아이템 나오면 로그로 남김
				pclClient->pclLog->FilePrint(TEXT("Config\\TreasureBoxDrop.log"), TEXT("BookBox Get User : \t[%s]\t / RareItem Unique, Num : \t[%d]\t[%d]"), pclCM->CR[myid]->GetName(), clItem.siUnique, clItem.siItemNum);
				bBigItemGet = true;
			}
			else
			{
				// 인벤에 빈자리가 없어서 추가할 수 없다. 
				if(rtnvalue == CANADDINV_FAIL_FULL)
				{
					SendServerResponseMsg( 0, SRVAL_ADDINV_FAIL_FULL,  clItem.siUnique, 0, pclchar->GetCharUnique());
				}
			}

		}
	}

	//================================================
	// 대박아이템 드롭
	//================================================
	// myid가 사람이고 수렵장, 초보자 사냥터가 아닐때, 중국 제외
	if ( bBigItemGet == false && IsWhereServiceArea( ConstServiceArea_China ) == false ) 
	{

		if (IsPC(myid) && pclCM->CR[myid]->GetHuntVillageUnique() <= 0 && pclCM->CR[myid]->GetMapIndex() != MAPINDEX_BEGINNER1)
		{
			if (pclCM->CR[myid]->pclCI->clIP.GetLevel() <= 0) 
				return false;

			bool bGiveBigItem = false;

			if( pclClient->IsCountrySwitch( Switch_ManufactureRevitalize ) )
			{
				// 중국 확률 수정.
				if( pclClient->IsWhereServiceArea( ConstServiceArea_China ) )
				{
					//SI32 siLuk = pclCM->CR[myid]->pclCI->clIP.GetLuk();
					SI32 siLuk = pclCM->CR[myid]->clPB.clTotalBA.GetLuk();
					if(siLuk == 0)			siLuk = 1;
					SI32 siRandMax = 5000000 / siLuk;
					siRandMax = min(500, siRandMax);
					SI32 siRand = ((rand() << 16) | rand()) % siRandMax;

					if(siRand == 0)			bGiveBigItem = true;
				}
				// 미국만 확률이 틀리다. - by LEEKH 2007.12.20
				else if( pclClient->IsWhereServiceArea( ConstServiceArea_USA ) )
				{
					//SI32 siLuk = pclCM->CR[myid]->pclCI->clIP.GetLuk();
					SI32 siLuk = pclCM->CR[myid]->clPB.clTotalBA.GetLuk();
					if(siLuk <= 8)			siLuk = 8;
					// 행운(MAX(8)) * 보정값(20) * 고정값(0.05)
					siLuk = (int)((float)siLuk * 20.0f * 0.05f);

					SI32 siRandMax = 25000 / siLuk;
					if(siRandMax > 25000)			siRandMax = 25000;

					SI32 siRand = rand() % siRandMax;
					if(siRand == 0)			bGiveBigItem = true;
				}
				else
				{
					//SI32 siLuk = pclCM->CR[myid]->pclCI->clIP.GetLuk();
					SI32 siLuk = pclCM->CR[myid]->clPB.clTotalBA.GetLuk();
					if(siLuk == 0)			siLuk = 1;
					SI32 siRandMax = 500000 / siLuk;
					//siRandMax = min(500, siRandMax);
					if(siRandMax < 500)			siRandMax = 500;

					SI32 siRand = rand() % siRandMax;

					if(siRand == 0)			bGiveBigItem = true;
				}
			}
			else
			{
				UI32 usertotalstatus = pclCM->CR[myid]->pclCI->clIP.GetLevel() * 6;
				UI32 luckpercent = pclCM->CR[myid]->pclCI->clIP.GetLuk() * 100 / usertotalstatus;
				UI32 hndpercent = pclCM->CR[myid]->pclCI->clIP.GetHnd() * 100 / usertotalstatus;

				UI32 luckpoint = 0;
				UI32 hndpoint = 0;
				UI32 huntpoint = 0;

				if (luckpercent >= 0)
				{
					if (luckpercent < 3)	luckpoint = 0;
					else if (luckpercent < 6)	luckpoint = 10;
					else if (luckpercent < 9)	luckpoint = 25;
					else if (luckpercent < 12)	luckpoint = 35;
					else if (luckpercent < 15)	luckpoint = 45;
					else if (luckpercent < 18)	luckpoint = 60;
					else if (luckpercent < 21)	luckpoint = 80;
					else if (luckpercent < 24)	luckpoint = 100;
					else if (luckpercent < 27)	luckpoint = 125;
					else	luckpoint = 150;
				}

				if (hndpercent >= 0)
				{
					if (hndpercent < 3)	hndpoint = 0;
					else if (hndpercent < 6)	hndpoint = 10;
					else if (hndpercent < 9)	hndpoint = 25;
					else if (hndpercent < 12)	hndpoint = 35;
					else if (hndpercent < 15)	hndpoint = 45;
					else if (hndpercent < 18)	hndpoint = 60;
					else if (hndpercent < 21)	hndpoint = 80;
					else if (hndpercent < 24)	hndpoint = 100;
					else if (hndpercent < 27)	hndpoint = 125;
					else	hndpoint = 150;
				}

				huntpoint = luckpoint * 0.7 + hndpoint * 0.3;

				UI32 battlepercent = 0;
				battlepercent = pclCM->CR[enemyid]->clPB.GetBattlePower() * 100 / usertotalstatus;

				if (battlepercent < 25)	huntpoint = 0;
				else if (battlepercent < 30)	huntpoint = huntpoint * 0.2;
				else if (battlepercent < 35)	huntpoint = huntpoint * 0.4;
				else if (battlepercent < 40)	huntpoint = huntpoint * 0.6;
				else if (battlepercent < 45)	huntpoint = huntpoint * 0.8;

				//UI32 randvalue = rand() % 1000000; //~0x7fff 까지밖에 안되요-_-!

				UI32 randvalue = ((rand() << 16) | rand()) % 1000000;

				// 대박아이템 줌
				if (randvalue < huntpoint)
					bGiveBigItem = true;
			}

			if(bGiveBigItem)
			{
				// 랜덤하게 대박 아이템을 준다. 
				SI32 bigitemunique = pclClient->pclItemManager->GetRandBigWarItem();

				if(bigitemunique)
				{
					// 대박 물품을 만들어낸다. 
					cltItem clItem;

					SI16 brareswitch = 0;
					if(pclClient->pclItemManager->MakeRandItemUnique( bigitemunique, &clItem, 0, 0, &brareswitch,
						0,0,0,0,0,0) == true)
					{
						cltCharServer *pclchar = (cltCharServer*)pclCM->CR[myid];

						// 피로방지 적용
						clItem.siItemNum = pclchar->CalcTiredPoint( 1 );

						SI32 rtnvalue = 0;


						if(clItem.siItemNum > 0 && pclchar->InputItem(pclClient->pclItemManager, &clItem, INPUTITEMPERSON_OPTION_WAR, &rtnvalue))
						{
							// 대박아이템 나오면 로그로 남김
							pclClient->pclLog->FilePrint(TEXT("Config\\RareItemInMonster.v"), TEXT("RareItem Get User : \t[%s]\t / RareItem Unique, Num : \t[%d]\t[%d]"), pclCM->CR[myid]->GetName(), clItem.siUnique, clItem.siItemNum);
							bBigItemGet = true;
						}
						else
						{
							// 인벤에 빈자리가 없어서 추가할 수 없다. 
							if(rtnvalue == CANADDINV_FAIL_FULL)
							{
								SendServerResponseMsg( 0, SRVAL_ADDINV_FAIL_FULL,  clItem.siUnique, 0, pclchar->GetCharUnique());
							}
						}

					}

				}

			}
		}
	}
	//================================================

	return bBigItemGet;
}

bool cltServer::GiveSpecialItem( SI32 myid, SI32 enemyid )
{
	// 보물/재료 상자 드롭.
	if ( pclClient->IsCountrySwitch(Switch_MaterialBox) && IsPC(myid) )
	{
		if (pclCM->CR[myid]->pclCI->clIP.GetLevel() <= 0) 
			return false;

		SI32 randvalue = rand() % 1000;
		// 대박아이템 줌
		cltServer* pclserver = (cltServer*)pclClient;
		if ( randvalue < pclserver->pclBookBox->GetDropRate() )
		{
			// 대박 물품을 만들어낸다. 
			cltItem clItem;

			clItem.siUnique = ITEMUNIQUE(7975); // 책상자 유니크
			// 피로방지 시스템 적용
			clItem.siItemNum = ((cltCharServer*)pclCM->CR[myid])->CalcTiredPoint( 1 );

			SI32 rtnvalue = 0;

			cltCharServer *pclchar = (cltCharServer*)pclCM->CR[myid];

			if( clItem.siItemNum > 0 && pclchar->InputItem(pclClient->pclItemManager, &clItem, INPUTITEMPERSON_OPTION_WAR, &rtnvalue))
			{
				// 대박아이템 나오면 로그로 남김
				//pclClient->pclLog->FilePrint(TEXT("Config\\BookDrop.log"), TEXT("BookBox Get User : \t[%s]\t / RareItem Unique, Num : \t[%d]\t[%d]"), pclCM->CR[myid]->GetName(), clItem.siUnique, clItem.siItemNum);
				//bBigItemGet = true;
			}
			else
			{
				// 인벤에 빈자리가 없어서 추가할 수 없다. 
				if(rtnvalue == CANADDINV_FAIL_FULL)
				{
					SendServerResponseMsg( 0, SRVAL_ADDINV_FAIL_FULL,  clItem.siUnique, 0, pclchar->GetCharUnique());
				}
			}
		}
	}

	//================================================
	// 책 드롭
	//================================================
	// 중국 버전이고 myid가 사람인 경우만 
	// 국가 스위치로 변경
	if (pclClient->IsCountrySwitch(Switch_DropBookBox) && IsPC(myid) )
	{
		if (pclCM->CR[myid]->pclCI->clIP.GetLevel() <= 0) 
			return false;

		SI32 randvalue = rand() % 1000;
		// 대박아이템 줌
		cltServer* pclserver = (cltServer*)pclClient;
		if ( randvalue < pclserver->pclBookBox->GetDropRate() )
		{
			// 대박 물품을 만들어낸다. 
			cltItem clItem;

			clItem.siUnique = ITEMUNIQUE(7975); // 책상자 유니크
			// 피로방지 시스템 적용
			clItem.siItemNum = ((cltCharServer*)pclCM->CR[myid])->CalcTiredPoint( 1 );

			SI32 rtnvalue = 0;

			cltCharServer *pclchar = (cltCharServer*)pclCM->CR[myid];

			if( clItem.siItemNum > 0 && pclchar->InputItem(pclClient->pclItemManager, &clItem, INPUTITEMPERSON_OPTION_WAR, &rtnvalue))
			{
				// 대박아이템 나오면 로그로 남김
				//pclClient->pclLog->FilePrint(TEXT("Config\\BookDrop.Txt"), TEXT("BookBox Get User : \t[%s]\t / RareItem Unique, Num : \t[%d]\t[%d]"), pclCM->CR[myid]->GetName(), clItem.siUnique, clItem.siItemNum);
				//bBigItemGet = true;
			}
			else
			{
				// 인벤에 빈자리가 없어서 추가할 수 없다. 
				if(rtnvalue == CANADDINV_FAIL_FULL)
				{
					SendServerResponseMsg( 0, SRVAL_ADDINV_FAIL_FULL,  clItem.siUnique, 0, pclchar->GetCharUnique());
				}
			}
		}
	}


	// 서버 이벤트 인던에서 NDE 부적 나옴
	if (  IsCountrySwitch(Switch_Server_Event) && IsPC( myid ) )
	{
		if ( pclMapManager->GetInstanceMapType(pclCM->CR[myid]->GetMapIndex( )) == INSTANCEMAP_TYPE_SERVER )
		{
			cltServer* pclserver = (cltServer*)pclClient;
			if ( NULL == pclserver )
			{
				return false;
			}

			if ( pclCM->CR[myid]->pclCI->clIP.GetLevel() <= 0 )
			{
				return false;
			}

			SI32 siRandValue = (rand() % 100) + 1;

			if ( siNDEScrollRate >= siRandValue )
			{
				// 대박 물품을 만들어낸다. 
				cltItem clItem;

				// NDE 부적
				clItem.siUnique = ITEMUNIQUE(13043); 
				// 피로방지 시스템 적용
				clItem.siItemNum = ((cltCharServer*)pclCM->CR[myid])->CalcTiredPoint( 1 );

				SI32 rtnvalue = 0;

				cltCharServer *pclchar = (cltCharServer*)pclCM->CR[myid];

				if( clItem.siItemNum > 0 && pclchar->InputItem(pclClient->pclItemManager, &clItem, INPUTITEMPERSON_OPTION_WAR, &rtnvalue))
				{
					// 대박아이템 나오면 로그로 남김
					pclClient->pclLog->FilePrint(TEXT("Config\\NDEScroll.log"), TEXT("NDEScroll Get User : \t[%s]\t / RareItem Unique, Num : \t[%d]\t[%d]"), pclCM->CR[myid]->GetName(), clItem.siUnique, clItem.siItemNum);
				}
				else
				{
					// 인벤에 빈자리가 없어서 추가할 수 없다. 
					if(rtnvalue == CANADDINV_FAIL_FULL)
					{
						SendServerResponseMsg( 0, SRVAL_ADDINV_FAIL_FULL,  clItem.siUnique, 0, pclchar->GetCharUnique());
					}
				}
			}	// if ( siNDEScrollRate >= siRandValue ) 종료
		}
	}

	// pvp-
	if ( IsCountrySwitch(Switch_BaekNyeonSam) && IsPC(myid) )
	{
		if( pclMapManager->IsPkMap(pclCM->CR[myid]->GetMapIndex()) )
		{
			if (pclCM->CR[myid]->pclCI->clIP.GetLevel() <= 0) 
				return false;

			SI32 randvalue = rand() % siBaenNyeonSamRate;
			// 대박아이템 줌
			cltServer* pclserver = (cltServer*)pclClient;
			if ( randvalue == 0 )
			{
				// 대박 물품을 만들어낸다. 
				cltItem clItem;

				// 백년삼
				clItem.siUnique = ITEMUNIQUE(2765); 
				// 피로방지 시스템 적용
				clItem.siItemNum = ((cltCharServer*)pclCM->CR[myid])->CalcTiredPoint( 1 );

				SI32 rtnvalue = 0;

				cltCharServer *pclchar = (cltCharServer*)pclCM->CR[myid];

				if( clItem.siItemNum > 0 && pclchar->InputItem(pclClient->pclItemManager, &clItem, INPUTITEMPERSON_OPTION_WAR, &rtnvalue))
				{
					// 대박아이템 나오면 로그로 남김
					pclClient->pclLog->FilePrint(TEXT("Config\\BackNeyonSam.log"), TEXT("BackNeyonSam Get User : \t[%s]\t / RareItem Unique, Num : \t[%d]\t[%d]"), pclCM->CR[myid]->GetName(), clItem.siUnique, clItem.siItemNum);
					//	bBigItemGet = true;
				}
				else
				{
					// 인벤에 빈자리가 없어서 추가할 수 없다. 
					if(rtnvalue == CANADDINV_FAIL_FULL)
					{
						SendServerResponseMsg( 0, SRVAL_ADDINV_FAIL_FULL,  clItem.siUnique, 0, pclchar->GetCharUnique());
					}
				}
			}
		}
	}

	return true;
}

