//---------------------------------
// 20010/11/1 ���°�
//---------------------------------

//-----------------------------
// Common
//-----------------------------
#include "CharManager.h"

#include "PersonInfo.h"

#include "Char\CharServer\Char-Server.h"
#include "Char\CharClient\Char-Client.h"
#include "..\..\Server\TraceBoot.h"

#include "../common/Char/MonsterGroupManager/MonsterGroupManager.h"

#include "..\Common\Bullet\Bullet.h"
#include "Util\Util.h"

//-------------------------------------------
// Server 
//-------------------------------------------
#include "..\..\Server\Server.h"
#include "..\..\Server\Statistics\Statistics.h"

//-------------------------------------------
// Client
//-------------------------------------------
#include "..\..\Client\Client.h"


#include "..\..\DBManager\GameDBManager_World\DBMsg-Person.h"
#include "..\..\DBManager\GameDBManager_World\DBMsg-Item.h"

#include "../../../lib/HashTable/HashTable_IntInt.h"
#include "../../../lib/HashTable/HashTable_StringInt.h"

#include "../CommonLogic/MsgRval-Define.h"

#include "../../../Apex/ApexGoonZu.h"

#include "MsgType-Person.h"


extern cltCommonLogic* pclClient;

//-----------------------------------------------
// cltCharManager
//-----------------------------------------------

cltCharManager::cltCharManager(SI32 gamemode)
{
	SI32 i;

	for(i = 0;i < MAX_USER_WEAPON_TYPE_NUMBER;i++)
	{
		siUserWeaponStatistic[i] = 0;
	}

	//----------------------------------------------
	// IDManager
	//----------------------------------------------
	SI32 maxcharnumber ;
	if(gamemode == GAMEMODE_SERVER)
	{
		maxcharnumber		= MAX_CHAR_NUMBER;
		siMaxSessionNumber	= MAX_PERSON_NUMBER;
		siMaxInstanceNPC	= MAX_INSTANCE_NPC_NUMBER;
	}
	else
	{
		maxcharnumber		= 450;
		siMaxSessionNumber	= 3;
		siMaxInstanceNPC	= 50;
	}

	pclIDManager	= new cltIDManager(maxcharnumber, siMaxSessionNumber, siMaxInstanceNPC);


	for(i = 0;i < MAX_CHAR_NUMBER;i++)
	{
		CR[i]				= NULL;
		SessionSwitch[i]	= true;
	}


	if(gamemode == GAMEMODE_SERVER)
	{
		dwParametaBoxActionIntervalClock	= 20000;
		// by LEEKH 2007.10.10 - PVP ��������ġ�� ServerInfo�� ����
		//if( pclClient->IsWhereServiceArea(ConstSwitch_PVP) )
		if(pclClient->siEnablePVP == 1)
		{
			siIncLifeUnit						= ((cltServer*)pclClient)->siRegenerateLifeValue;
		}
		else
			siIncLifeUnit						= (CHAR_ACTION_INTERVAL_FRAME * 10);

		for(i = siMaxSessionNumber;i < maxcharnumber;i++)
		{
			CR[i] = new cltCharServer();
			CR[i]->SetpclCM(this);
			SessionSwitch[i] = false;
		}
	}
	else
	{
		dwParametaBoxActionIntervalClock	= 500;
		// by LEEKH 2007.10.10 - PVP ��������ġ�� ServerInfo�� ����
		//if( pclClient->IsWhereServiceArea(ConstSwitch_PVP) )
		if(pclClient->siEnablePVP == 1)
		{
			siIncLifeUnit						= 10;
		}
		else
			siIncLifeUnit						= 10;

		for(i = siMaxSessionNumber; i < maxcharnumber;i++)
		{
			CR[i] = new cltCharClient();
			CR[i]->SetpclCM(this);
			SessionSwitch[i] = false;
		}
	}
}

cltCharManager::~cltCharManager()
{
	SI32 i;

	for(i = siMaxSessionNumber;i < MAX_CHAR_NUMBER;i++)
	{
		if(CR[i])
		{
			if(SessionSwitch[i] == false)
			{
				delete CR[i];
				CR[i] = NULL;
			}
		}
	}

	if(pclIDManager)
	{
		delete pclIDManager;
		pclIDManager = NULL;
	}

}
// ĳ���Ͱ� ��� �ִ��� ���θ� �˷��ش�. 
BOOL cltCharManager::IsAlive(SI32 id)
{
	if(pclIDManager->IsValidID(id) == FALSE)return FALSE;
	if(CR[id]->GetLife() <= 0)return FALSE;

	return TRUE;
}


//personid�� �ٰŷ� id�� ���Ѵ�. 
SI32 cltCharManager::GetIDFromPersonID(SI32 personid)
{

	SI32 id = 0;

	cltServer* pclserver = (cltServer *)pclClient;

	if(pclserver->pclPersonHashTable == NULL)return 0;

	if(pclserver->pclPersonHashTable->Find(personid, id) == true)
	{
		if(IsValidID(id) == true)
		{
			return id;
		}

	}

	return 0;

}

//�̸��� �ٰŷ� id�� ���Ѵ�. (PC�߿����� ã�´�. ) 
SI32 cltCharManager::GetIDFromName(TCHAR* pname)
{
	TCHAR buffer[ MAX_PLAYER_NAME ];

	StringCchCopy( buffer, MAX_PLAYER_NAME, pname );

	SI32 personid = 0, id = 0;

	if( ((cltServer *)pclClient)->pclHashTable->Find(buffer, personid, id, 500) == true)
	{
		return id;
	}

	return 0;

}

// CharUnique�� �ٰŷ� id�� ���Ѵ�.
SI32 cltCharManager::GetIDFromCharUnique(SI32 charunique)
{
	if(charunique <= 0)return 0;
	if(charunique >= MAX_CHAR_NUMBER)return 0;

	return cltCharCommon::CharUniqueInfo[charunique];
}

SI32 cltCharManager::GetIDFromCharKind( SI32 charkind )
{
	if ( pclClient->GameMode == GAMEMODE_CLIENT )
	{
		for ( SI32 id=0; id < pclIDManager->GetMaxIDNumber() ; ++id )
		{
			if ( IsValidID(id) )
			{
				if( CR[id]->GetKind() == charkind )
				{
					return id;
				}
			}
		} 
	}

	return 0;
}
BOOL cltCharManager::Action()
{
	SI32 id;

	SI32 index = 0;

	static int nLastOrderIndex = -1;
	if ( pclIDManager->GetCharOrderIndex() != nLastOrderIndex )
	{
		nLastOrderIndex = pclIDManager->GetCharOrderIndex();
		TraceBoot("CHARORDER CHANGED charOrderIndex=%d", nLastOrderIndex);
		for (SI32 i = 0; i < nLastOrderIndex && i < 20; ++i)
		{
			TraceBoot("CHARORDER   slot[%d] = id %d", i, pclIDManager->GetCharOrder(i));
		}
	}

	if ( pclClient->pclMapManager )
	{
		pclClient->pclMapManager->InitUserCount();
	}

	while(id = pclIDManager->GetCharOrder(index))
	{
		index++;

		if(pclIDManager->IsValidID(id) )
		{
			if(CR[id]->Action()	==	FALSE)
			{

				//������ �ƴ� ��쿡�� ���δ�.
				if(CR[id]->IsValidConnection() == false)
				{

					// -------------------------------------
					// �׷쿡�� �����Ѵ�. ������ ���ο� ������ �����.
					//--------------------------------------
					pclClient->pclMonsterGroupManager->DeleteMember(((cltCharServer*)CR[id])->siGroupUnique,
						((cltCharServer*)CR[id])->siGroupMember,
						((cltCharServer*)CR[id])->bGroupBossSwitch);
					// �� �������� ������ �����Ѵ�. 
					CR[id]->DeleteInCell();
					if(DeleteChar( id ) == TRUE)
					{
						index--;
					}

				}
				// ������ ��쿡�� �������ʰ� ������ ��ġ�� ���Ѵ�. (�� ������ ��쿡��.)
				else if(pclClient->GameMode == GAMEMODE_SERVER)
				{
					// [����] ��Ʋ�ξ� ���� ��� �׳� ��Ȱ 
					if(((cltCharServer*)CR[id])->siKilledMode == KILLEDMODE_BATTLEROYAL)
					{
						((cltCharServer*)CR[id])->Revive();
						continue;
					}

					// ����� �ʵ忡�� �׾�����!
					SI32 mapindex = ((cltCharServer*)CR[id])->pclMap->siMapIndex;
					if(pclClient->IsCountrySwitch( Switch_NewGuildHuntMapWar ) )	// ������
					{
						if ( IsGuildWarMap_Use_New_GuildWar(mapindex) )// ���� ����� ���̸�
						{
							SI32 fieldindex = 0 ;
							if ( pclClient->IsCountrySwitch(Switch_NewGuildHuntMapWar ) )	// ������
							{
								fieldindex = GetGuildWarFieldIndexFromMapIndex_Use_New_Guild_War( mapindex );
							}
							else	//������
							{
								fieldindex = GetGuildWarFieldIndexFromMapIndex_Not_Use_New_Guild_War( mapindex );
							}

							((cltServer*)pclClient)->pclGuildWarManager[fieldindex]->GuildWarDie(((cltCharServer*)CR[id])->pclCI->GetPersonID() );
						}
						else if (IsGuildWarMap_NEW(mapindex)) // ���ο� ����� ���̸�
						{
							if ( true == pclClient->IsCountrySwitch(Switch_NewGuildHuntMapWar) )
							{
								SI32 fieldindex = GetHuntmapGuildWarFieldIndexFromMapIndex( mapindex );
								((cltServer*)pclClient)->pclGuildHuntMapManager->GuildWarRevive( ((cltCharServer*)CR[id])->pclCI->GetPersonID() )					;

							}
						}
						else
						{
							// ��Ȱ�ϰ� �����. 
							//cyj Ŭ���̾�Ʈ���� ��Ȱ �޼��� �ڽ��� �����Ҷ� ��Ȱ�ϵ��� ����
							//CR[id]->Revive();
							// 1:1 PVP �� �׾����� -100���� .�Ѵ�.
							cltCharServer* pDieCharServer = GetCharServer(id) ;

							if ( pDieCharServer != NULL ) 
							{
								SI16 siMyenemyunique = pDieCharServer->siEnemUnique	;
								if ( siMyenemyunique == -100 ) 
								{
									// pvp �� �ٽ� ��� ������ 0���� �ʱ�ȭ
									((cltCharServer*)CR[id])->Resurrection( true)		;
									((cltCharServer*)CR[id])->siEnemUnique	=	0	;
									((cltCharServer*)CR[id])->SetPVP_LeagueEnemyPersonID( 0 );

								}
								else
								{
									// ���� ���� ����� �������� Ȯ�� 
									// ���ΰ� ���� �̺�Ʈ�� ���� �����̶�� 
									SI32 siDieMapIndex = pDieCharServer->GetMapIndex() ;
									if( siDieMapIndex == PLAY_MAP_INDEX_INDODEVENT )// �ش� ���ε����� �ְ� 
									{
										cltMapCommon* pMap = pclClient->pclMapManager->GetMapPointer( siDieMapIndex )	;
										if(pMap !=NULL )
										{
											SI32 siColl = pMap->CalcCollInfo( pDieCharServer->GetX(), pDieCharServer->GetY() );
											if( siColl & BLOCK_EVENT1 || siColl & BLOCK_EVENT4 )	// 4�� �̺�Ÿ Ÿ���� ��� ������.
											{
												// �г�Ƽ ���� �ٽ� ��Ƴ���
												((cltCharServer*)CR[id])->Resurrection( false)		;
											}
											// [����] ���Һ� �̺�Ʈ���� ��ų���� ����Ѱ�� �߰�
											else if ( siColl & BLOCK_EVENT2 || siColl & BLOCK_EVENT3 ||
												siColl & BLOCK_EVENT5 || siColl & BLOCK_EVENT6 )		// 2,3,5,6�� Ÿ���� ��� �������� ���� ó��
											{
												// �г�Ƽ ���� �ٽ� ��Ƴ���
												((cltCharServer*)CR[id])->Resurrection( true );
											}
										}
									}
									else
									{
										// ����� ���� ���Ƽ ����.
										((cltCharServer*)CR[id])->DeathPenalty();					
										((cltCharServer*)CR[id])->RequestRevive(id,NORMAL);
										((cltCharServer*)CR[id])->siEnemUnique = 0	;

									}
								}
							}
						}
					}
					else	// ������
					{
						if ( IsGuildWarMap_Not_Use_New_GuildWar(mapindex) )// ���� ����� ���̸�
						{
							SI32 fieldindex = 0 ;
							if ( pclClient->IsCountrySwitch(Switch_NewGuildHuntMapWar ) )	// ������
							{
								fieldindex = GetGuildWarFieldIndexFromMapIndex_Use_New_Guild_War( mapindex );
							}
							else	//������
							{
								fieldindex = GetGuildWarFieldIndexFromMapIndex_Not_Use_New_Guild_War( mapindex );
							}

							((cltServer*)pclClient)->pclGuildWarManager[fieldindex]->GuildWarDie(((cltCharServer*)CR[id])->pclCI->GetPersonID() );
						}
						else
						{
							// ��Ȱ�ϰ� �����. 
							//cyj Ŭ���̾�Ʈ���� ��Ȱ �޼��� �ڽ��� �����Ҷ� ��Ȱ�ϵ��� ����
							//CR[id]->Revive();
							// 1:1 PVP �� �׾����� -100���� .�Ѵ�.
							cltCharServer* pDieCharServer = GetCharServer(id) ;

							if ( pDieCharServer != NULL ) 
							{
								SI16 siMyenemyunique = pDieCharServer->siEnemUnique	;
								if ( siMyenemyunique == -100 ) 
								{
									// pvp �� �ٽ� ��� ������ 0���� �ʱ�ȭ
									((cltCharServer*)CR[id])->Resurrection( true)		;
									((cltCharServer*)CR[id])->siEnemUnique	=	0	;
									((cltCharServer*)CR[id])->SetPVP_LeagueEnemyPersonID( 0 );

								}
								else
								{
									// ���� ���� ����� �������� Ȯ�� 
									// ���ΰ� ���� �̺�Ʈ�� ���� �����̶�� 
									SI32 siDieMapIndex = pDieCharServer->GetMapIndex() ;
									if( siDieMapIndex == PLAY_MAP_INDEX_INDODEVENT )// �ش� ���ε����� �ְ� 
									{
										cltMapCommon* pMap = pclClient->pclMapManager->GetMapPointer( siDieMapIndex )	;
										if(pMap !=NULL )
										{
											SI32 siColl = pMap->CalcCollInfo( pDieCharServer->GetX(), pDieCharServer->GetY() );
											if( siColl & BLOCK_EVENT1 || siColl & BLOCK_EVENT4 )	// 4�� �̺�Ÿ Ÿ���� ��� ������.
											{
												// �г�Ƽ ���� �ٽ� ��Ƴ���
												((cltCharServer*)CR[id])->Resurrection( false)		;
											}
										}
									}
									else
									{
										// ����� ���� ���Ƽ ����.
										((cltCharServer*)CR[id])->DeathPenalty();					
										((cltCharServer*)CR[id])->RequestRevive(id,NORMAL);
										((cltCharServer*)CR[id])->siEnemUnique = 0	;

									}
								}
							}


						}

					}
				}

			}
		}
	}

	return TRUE;
}

BOOL cltCharManager::SetInfo(cltCharCommon* pclchar)
{
	if(pclchar == NULL)return FALSE;

	// Char�� id�� ���Ѵ�.
	SI32 id			= pclchar->GetID();
	SI32 kind		= pclchar->GetKind();
	SI32 charunique = pclchar->GetCharUnique();
	bool bpcswitch	= false;
	bool binstancenpc = false ;

	if(IsPC(charunique) )
	{
		bpcswitch	= true;
	}
	else if(IsInstanceNPC(charunique))
	{
		binstancenpc = true ;
	}
	else
	{
		bpcswitch	= false;
	}

	// ĳ���͸� �߰��Ѵ�. 
	if(pclIDManager->AddChar(id, kind, bpcswitch, binstancenpc) == -1)
	{
		MsgBox(TEXT("fd9737495"), TEXT("4398754:%d %d"), bpcswitch, id);
		return FALSE;
	}

	return TRUE;
}

bool cltCharManager::IsValidID(SI32 id)
{
	return pclIDManager->IsValidID(id);
}


SI32 cltCharManager::GetCharOrder(SI32 index)
{
	return pclIDManager->GetCharOrder(index);
}

// Person�� �����Ѵ�. 
BOOL cltCharManager::DeleteChar(SI32 id)
{

	if(IsValidID(id) == FALSE)
	{
		return FALSE;
	}

	// ������ �ʿ��� ��ġ�� ���Ѵ�. 
	CR[id]->DeleteOperation();

	SI32 charunique	= CR[id]->GetCharUnique();

	cltCharCommon::CharUniqueInfo[ charunique ] = 0;

	// ������ �������� �����Ѵ�. 
	CR[id]->DeleteID();
	cltPersonInfo pinfo;
	CR[id]->pclCI->Set(&pinfo);

	// [����] ĳ���Ͱ� ���� �ǰ� �Ǹ� ��Ʋ�ξ� �Ŀ��� ���� ������ ���� �ȴ�.
	CR[id]->bBattleRoyalPowerupMode = false;

	if(pclIDManager->DeleteChar(id) == TRUE)
	{
		CR[id]->m_clCharCurseEffect.Clear();
		return TRUE;
	}

	return FALSE;
}

// Ư�� ���ε����� �ִ� ���͸� ���� �����Ѵ�. (��ȯ�� ����) 
void cltCharManager::DeleteAllNPCInMapIndex(SI32 mapindex)
{

	SI32 id;

	SI32 delnum = 0;
	SI32 index = 0;
	while(id = GetCharOrder(index))
	{
		index++;

		if (IsAlive(id) == false)										continue;
		if (IsPC(id))													continue;
		if (CR[id]->GetMapIndex() != mapindex)							continue;
		if (CR[id]->clIdentity.siIdentity == IDENTITY_PERSON_SUMMON)	continue;
		if (CR[id]->clIdentity.siIdentity == IDENTITY_SOULGUARD)		continue;

		// �� �������� ������ �����Ѵ�. 
		CR[id]->DeleteInCell();
		if(DeleteChar(id) == TRUE)
		{
			index--;
		}

		delnum++;
	}

	SI32 a = delnum;

}

// ��� ĳ���͸� �����Ѵ�. 
BOOL cltCharManager::DeleteChar()
{
	SI32 id;

	while(id = GetCharOrder(0))
	{
		// �� �������� ������ �����Ѵ�. 
		CR[id]->DeleteInCell();
		DeleteChar(id);
	}

	return TRUE;
}


// ĳ���� �̸��� �����.
void cltCharManager::MakeCharName()
{
	SI32 id;

	SI32 index = 0;
	while(id = GetCharOrder(index))
	{
		index++;

		cltCharClient* pclchar = (cltCharClient* )CR[id];

		pclchar->MakeCharName(NULL);

	}
}

// ĳ���� �̸��� ǥ���Ѵ�. 
void cltCharManager::DrawCharName()
{

	cltClient* pclclient = (cltClient*)pclClient;
	CBattleRoyalClient* pclBattleRoyalClient = pclclient->m_pclBattleRoyal;

	if ( pclclient->GameMode != GAMEMODE_CLIENT ) return ;

	SI32 undermouseid = pclclient->pclUnderMouseObject->GetUnderMouseID();

	SI32 id;

	SI32 index = 0;
	while(id = GetCharOrder(index))
	{
		index++;

		if(IsAlive(id))
		{ 
			if(CR[id]->GetDrawFrame() == pclClient->GetFrame())
			{
				// [����] ��Ʋ�ξ� ���� ��� �̸� ���� ������ �������� �ʴ´�.
				if( NULL != pclBattleRoyalClient)
				{
					if(TRUE == pclBattleRoyalClient->IsTransform((cltCharClient*)CR[id]))
					{
						continue;
					}
				}

				if(IsNPC(CR[id]->GetCharUnique())||IsInstanceNPC(CR[id]->GetCharUnique()))
				{
					if(id == undermouseid)
					{
						CR[id]->DrawName();
					}
				}
				else
				{
					CR[id]->DrawName();
				}
			}
		}
	}
}

// ĳ���� ��ǳ���� ǥ���Ѵ�. 
void cltCharManager::DrawChatBalloon()
{
	SI32 id;

	SI32 index = 0;
	while(id = GetCharOrder(index))
	{
		index++;

		if(CR[id]->GetDrawFrame() ==pclClient->GetFrame())
		{
			CR[id]->DrawChatBalloon();
		}

		//cyj �׾������� ��ǳ�� �������� ����
		/*
		if(IsAlive(id))
		{
			if(CR[id]->GetDrawFrame() ==pclClient->GetFrame())
			{
				CR[id]->DrawChatBalloon();
			}
		}
		*/
	}
}


// ��� Person���� �޽����� ������. 
void cltCharManager::SendAllPersonMsg(sPacketHeader* pmsg)
{
	SI32 id;

	SI32 index = 0;
	while(id = GetCharOrder(index))
	{
		index++;

		cltCharServer* pclchar = (cltCharServer*)CR[id];
		if(pclchar->GetSession())
		{
			pclchar->SendNetMsg(pmsg);
		}
	}
}

// Ư�� ������ ������ ����ڿ��� �޽����� ������. 
void cltCharManager::SendNewsMsg(sPacketHeader* pmsg, SI32 newskind)
{
	SI32 id;

	SI32 index = 0;
	while(id = GetCharOrder(index))
	{
		index++;

		cltCharServer* pclchar = (cltCharServer*)CR[id];
		if(pclchar->GetSession())
		{
			if( pclchar->siCaptionKind == newskind )
			{
				pclchar->SendNetMsg(pmsg);
			}
		}
	}
}

// Ư�� ������ �ִ� ����ڵ鿡�� �޽����� ������. 
void cltCharManager::SendAllPersonInVillageMsg(sPacketHeader* pmsg, SI16 villageunique)
{
	SI32 id;

	SI32 index = 0;
	while(id = GetCharOrder(index))
	{
		index++;

		cltCharServer* pclchar = (cltCharServer*)CR[id];
		if(pclchar->GetSession())
		{
			if(pclchar->GetCurrentVillageUnique() == villageunique)
			{
				pclchar->SendNetMsg(pmsg);
			}

		}
	}
}

// Ư�� ������  �ִ� ����ڵ鿡�� �޽����� ������. 
void cltCharManager::SendAllPersonInMapMsg(sPacketHeader* pmsg, SI16 mapindex)
{
	SI32 id;

	SI32 index = 0;
	while(id = GetCharOrder(index))
	{
		index++;

		cltCharServer* pclchar = (cltCharServer*)CR[id];
		if(pclchar->GetSession())
		{
			if(pclchar->GetMapIndex() == mapindex)
			{
				pclchar->SendNetMsg(pmsg);
			}

		}
	}
}

// Ư�� ������ �ֹο��Ը� �޽����� ������. 
void cltCharManager::SendAllPersonHomeMsg(sPacketHeader* pmsg, SI16 villageunique)
{
	SI32 id;

	SI32 index = 0;
	while(id = GetCharOrder(index))
	{
		index++;

		cltCharServer* pclchar = (cltCharServer*)CR[id];
		if(pclchar->GetSession())
		{
			if(pclchar->pclCI->clBI.siHomeVillage == villageunique)
			{
				pclchar->SendNetMsg(pmsg);
			}

		}
	}
}

//_LEON_GUILD_CHAT
//#if defined(_DEBUG)
// Ư�� ����� ������Ը� �޽����� ������. 
//KHY - 0131 - ��� ��� �޽��� ���� - �̸����� �˻� - ����ũ��ȣ �˻�����!
void cltCharManager::SendAllGuildMemberMsg(sPacketHeader* pmsg,  SI32 GuildUnitIndex ,TCHAR * GuildName )
{
	SI32 id;

	SI32 index = 0;

	if(GuildName != NULL)
	{
		while(id = GetCharOrder(index))
		{
			index++;

			cltCharServer* pclchar = (cltCharServer*)CR[id];
			if(pclchar->GetSession())
			{
				// ��� ����ũ�� �̸��� �Բ� üũ�Ѵ�.
				if( pclchar->pclCI->clBI.clGuildRank.IsGuildMember() &&
					pclchar->pclCI->clBI.clGuildRank.siGuildUnitIndex == GuildUnitIndex && 					
					strcmp(pclchar->pclCI->clBI.clGuildRank.szGuildName, GuildName) == 0 )
				{
					pclchar->SendNetMsg(pmsg);
				}
			}
		}
	}
}
//#endif

// ��Ƽ���鿡�� �޽����� ������. 
void cltCharManager::SendAllPartyMemberMsg(sPacketHeader* pmsg, SI32 siPartyIndex)
{
	SI32 id;

	SI32 index = 0;
	while(id = GetCharOrder(index))
	{
		index++;

		cltCharServer* pclchar = (cltCharServer*)CR[id];
		if(pclchar->GetSession())
		{
			if(pclchar->siPartyIndex == siPartyIndex)
				pclchar->SendNetMsg(pmsg);
		}
	}
}

// ���ε鿡�Ը� �޽����� ������. 
void cltCharManager::SendAllAdultUserMsg(sPacketHeader* pmsg)
{
	SI32 id;

	SI32 index = 0;
	while(id = GetCharOrder(index))
	{
		index++;

		cltCharServer* pclchar = (cltCharServer*)CR[id];
		if(pclchar->GetSession())
		{
			if(pclchar->pclCI->clBI.m_uiRealAge >= 18)
			{
				pclchar->SendNetMsg(pmsg);
			}

		}
	}
}


// ��� ����ڸ� �� ��������. (DB�� ���嵵 �Ѵ�. ) 
void cltCharManager::CloseAllSocket()
{
	SI32 id;

	SI32 index = 0;
	while(id = GetCharOrder(index))
	{
		index++;

		cltCharServer* pclchar = (cltCharServer*)CR[id];
		if(pclchar->GetSession())
		{
			// apex-
#ifdef _CHINA
			NoticeApexProxy_UserLogout(id, (char*)pclchar->pclCI->clBI.szAccountID );
			pclchar->bApexLogin = false;
#endif


#ifdef USE_GAME_GUARD_SERVER
			pclchar->CCSAuthClose();
#endif
			// -apex
			pclchar->GetSession()->CloseSocket();
		}
	}
}

// person�̸��� ������ PersonID�� ã�´�. 
SI32 cltCharManager::GetPersonIDFromName(TCHAR* pname)
{
	SI32 id;

	SI32 index = 0;
	while(id = GetCharOrder(index))
	{
		index++;

		cltCharServer* pclchar = (cltCharServer*)CR[id];
		if(pclchar->GetSession())
		{
			if(_tcscmp(pname, pclchar->GetName()) == 0)
			{
				return pclchar->pclCI->GetPersonID();
			}
		}
	}

	return 0;
}

// PersonID�� �ٰŷ� �ش� ĳ���͸� ������ �ƿ� ��Ų��. 
bool cltCharManager::OutByPersonID(SI32 personid)
{
	SI32 id;

	SI32 index = 0;
	while(id = GetCharOrder(index))
	{
		index++;

		cltCharServer* pclchar = (cltCharServer*)CR[id];
		if(pclchar->GetSession())
		{

			if(pclchar->pclCI->GetPersonID() == personid)
			{
				// apex-
#ifdef _CHINA
				NoticeApexProxy_UserLogout(id, (char*)pclchar->pclCI->clBI.szAccountID );
				pclchar->bApexLogin = false;
#endif


#ifdef USE_GAME_GUARD_SERVER
				pclchar->CCSAuthClose();
#endif
				// -apex
				pclchar->GetSession()->CloseSocket();
				return true;
			}
		}
	}

	return false;

}

// ��� ���� ������ �����Ѵ�. 
bool cltCharManager::DeleteAllWarKill()
{
	SI32 id;

	SI32 index = 0;
	while(id = GetCharOrder(index))
	{
		index++;

		cltCharServer* pclchar = (cltCharServer*)CR[id];
		if(pclchar->GetSession())
		{
			pclchar->siWarEnemyKillNumber	= 0;
			pclchar->siWarKilledNumber		= 0;
			pclchar->siBlackArmyWarMapIndex	= -1;

			if (pclchar->pclCI->clBI.uiGMMode == GMMODE_ATB_WARMVP)
			{
				pclchar->pclCI->clBI.uiGMMode = GMMODE_ATB_NONE;
				pclchar->SetUpdateSwitch(UPDATE_BASICINFO, true, 0 );
			}
		}
	}

	return true;

}

bool cltCharManager::DeleteBlackWarKill()
{
	SI32 id;

	SI32 index = 0;
	while(id = GetCharOrder(index))
	{
		index++;

		cltCharServer* pclchar = (cltCharServer*)CR[id];
		if(pclchar->GetSession())
		{
			pclchar->siBlackWarEnemyKillNumber		 = 0;
			pclchar->siBlackWarKilledNumber			 = 0;
			pclchar->siBlackWarEnemyKillScore		 = 0;
			pclchar->siBlackWarEnemyKillVillageScore = 0;

			if (pclchar->pclCI->clBI.uiGMMode == GMMODE_ATB_WARMVP)
			{
				pclchar->pclCI->clBI.uiGMMode = GMMODE_ATB_NONE;
				pclchar->SetUpdateSwitch(UPDATE_BASICINFO, true, 0 );
			}
		}
	}

	return true;
}

// �ֺ��� �Ʊ����� ������ ��û�Ѵ�. 
SI32 cltCharManager::FindNearAlly(SI32 kind, SI32 mapindex, SI32 x, SI32 y, bool bwarswitch, SI16 villageunique)
{
	SI32 id;

	SI32 index = 0;
	while(id = GetCharOrder(index))
	{
		index++;

		cltCharServer* pclchar = (cltCharServer*)CR[id];
		if(pclchar->GetSession() == NULL)
		{
			if(pclchar->GetCurrentOrder() != ORDER_MOVE && pclchar->GetCurrentOrder() != ORDER_WAIT)continue;

			// ���� ĳ�����̸�, 
			if(bwarswitch)
			{
				if( pclchar->siCharMode != CHARMODE_INSTANCEMAP	)
					continue;
			}
			// ���� ������ ���� ĳ���Ϳ��Ը� ��û 
			else if(villageunique)
			{
				if(pclchar->pclCI->clBI.siHomeVillage != villageunique)continue;
			}
			// ���� ������ ĳ���Ϳ��Ը� ��û. 
			else 
			{
				if(pclchar->GetKind() != kind)continue;
			}

			if(pclchar->GetMapIndex() != mapindex)continue;

			if(max(TABS(pclchar->GetX() - x), TABS(pclchar->GetY() - y)) > 6)continue;

			return id;
		}
	}

	return 0;
}

// �ֺ����� �������� ���� �Ʊ��� ã�´�. 
SI32 cltCharManager::FindDamagedFriend(SI32 myid, SI32 mapindex, SI32 x, SI32 y, bool bwarswitch)
{

	cltCharServer* pclmychar = (cltCharServer*)CR[myid];
	if(pclmychar->clIdentity.siIdentity == IDENTITY_PERSON_SUMMON)
	{
		// ��ȯ���� �ڽŰ� �����ָ� ġ���Ҽ� �ֵ��� �˻��Ѵ�.
		SI32 id, checkid[2];
		checkid[0] = myid;
		checkid[1] = pclmychar->GetSummonParentID();

		for(SI32 i = 0; i < 2; ++i)
		{
			id = checkid[i];
			cltCharServer* pclchar = (cltCharServer*)CR[id];
			if(IsAlive(id) == false)				continue;
			if(pclchar->GetMapIndex() != mapindex)	continue;

			// ü���� �ջ��� �Ծ�� �Ѵ�. 
			if(pclchar->clPB.GetMaxLife() == pclchar->pclCI->clIP.GetLife())continue;
			if(max(TABS(pclchar->GetX() - x), TABS(pclchar->GetY() - y)) > 10)continue;

			return id;
		}
		return 0;
	}
	else
	{
		SI32 id;
		SI32 index = 0;
		while(id = GetCharOrder(index))
		{
			index++;

			cltCharServer* pclchar = (cltCharServer*)CR[id];
			if(pclchar->GetSession() == NULL)
			{
				if(IsAlive(id) == false)continue;
				if(pclchar->GetMapIndex() != mapindex)continue;
				if(pclchar->clIdentity.siIdentity)		continue;	// Ư�� ĳ���ʹ� ã�� �ʴ´�. 

				// ü���� �ջ��� �Ծ�� �Ѵ�. 
				if(pclchar->clPB.GetMaxLife() == pclchar->pclCI->clIP.GetLife())continue;

				if(max(TABS(pclchar->GetX() - x), TABS(pclchar->GetY() - y)) > 10)continue;

				return id;
			}
		}
	}
	return 0;
}

// ������� ���忡 ���� ��踦 ����. 
void cltCharManager::AddUserWeapon(cltItem* pclitem)
{
	SI32 type = pclitem->GetType(pclClient->pclItemManager);

	switch(type)
	{
	case ITEMTYPE_SWORD:	siUserWeaponStatistic[1]++;	break;
	case ITEMTYPE_SPEAR:	siUserWeaponStatistic[2]++; break;
	case ITEMTYPE_STAFF:	siUserWeaponStatistic[3]++; break;
	case ITEMTYPE_AXE:		siUserWeaponStatistic[4]++; break;
	case ITEMTYPE_BOW:		siUserWeaponStatistic[5]++; break;
	case ITEMTYPE_GUN:		siUserWeaponStatistic[6]++; break;
	default:				siUserWeaponStatistic[0]++; break;
	}
}

// ������� ���� ���ο� ������� ����ġ�� �ش�.
void cltCharManager::IncExp(SI32 personid, SI32 exp, SI32 reason)
{
	// �������̸�, 
	SI32 fatherid = GetIDFromPersonID( personid );
	if(fatherid)
	{
		cltCharServer* pclchar = (cltCharServer*)CR[fatherid];
		pclchar->IncreaseExp(exp);// SI32->SI64 ����ġ �����÷ο춧���� ����	
	}
	// ���������� �ʴٸ�, 
	else
	{
		// DB�� ����ġ �λ��� ��û�Ѵ�. 
		sDBRequest_ChangeExp clMsg(personid, reason, exp);
		pclClient->SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg);
	}
}

// ������� ���� ���ο� ������� ���� ����ġ�� �ش�.
void cltCharManager::IncFameExp(SI32 personid, SI32 exp, SI32 serverreason, SI32 dbreason)
{
	// �������̸�, 
	SI32 fatherid = GetIDFromPersonID( personid );
	if(fatherid)
	{
		cltCharServer* pclchar = (cltCharServer*)CR[fatherid];

		pclchar->IncreaseFameExp(exp, serverreason);
	}
	// ���������� �ʴٸ�, 
	else
	{
		// DB�� ����ġ �λ��� ��û�Ѵ�. 
		sDBRequest_ChangeFameExp clMsg(personid, dbreason, exp);
		pclClient->SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg);
	}
}



// Ư�� ������ ���� ��ġ�ϰ� �ִ� �ش� ���� �ֹε��� ����Ʈ. 
void cltCharManager::FindResidentListInVillage(SI32 villageunique)
{
	if(pclClient->GameMode != GAMEMODE_SERVER)return ;

	cltServer* pclserver = (cltServer*)pclClient;

	pclserver->siParaCharListIndex = 0;

	if(villageunique <= 0)return ;

	SI32 id;

	SI32 index = 0;
	while(id = GetCharOrder(index))
	{
		index++;

		if(IsPC(id) == false)continue;
		if(IsAlive(id) == false)continue;
		if(CR[id]->pclCI->clBI.siHomeVillage != villageunique)continue;
		if(CR[id]->GetMapIndex() != villageunique)continue;


		pclserver->siParaCharList[pclserver->siParaCharListIndex] = id;
		pclserver->siParaCharListIndex++;
	}
}


// Ư�� �����ȿ� �ִ� ������� ���� �Ա��� ������Ų��.
void cltCharManager::WarpToVillageGate(SI32 villageunique)
{
	if(villageunique <= 0 || villageunique >= MAX_VILLAGE_NUMBER)	return ;

	SI32 id;

	// ���� �Ա��� ã�´�. 
	SI32 centerx = pclClient->pclVillageManager->pclVillageInfo[villageunique]->PosX;
	SI32 centery = pclClient->pclVillageManager->pclVillageInfo[villageunique]->PosY;

	cltWarpInfo clWarpInfo(1, 	0, 	false, false, centerx, centery, -1, -1);

	SI32 index = 0;
	while(id = GetCharOrder(index))
	{
		index++;

		if(IsPC(id) == false)continue;
		if(CR[id]->GetMapIndex() != villageunique)continue;


		CR[id]->SetWarpInfo(&clWarpInfo, 60);

	}
}

// ��� ������� ������ �缳���Ѵ�. 
void cltCharManager::InitBlock()
{
	SI32 id;

	SI32 index = 0;
	while(id = GetCharOrder(index))
	{
		index++;

		CR[id]->SetBlock();
	}
}



// ĳ���� ���� ��� 
void cltCharManager::GetStatistic(cltStatistics* pclstatistic)
{
	SI32 i = 0 ;
	SI32 id = 0 ;

	if ( pclstatistic == NULL ) return ;

	// ��� ��ġ �ʱ�ȭ. 
	pclstatistic->siCurSBSUserNum	= 0;
	pclstatistic->siFishUserNumber	= 0;
	pclstatistic->siFarmUserNumber	= 0;
	pclstatistic->siMineUserNumber	= 0;
	pclstatistic->siSummonNumber	= 0;	// ��ȯ�� ���� 

	for(i = 0;i < MAX_QUEST_TYPE;i++)
	{
		pclstatistic->siQuestClear[i][0] = 0;
		pclstatistic->siQuestClear[i][1] = 0;
	}


	SI32 index = 0;
	while(id = GetCharOrder(index))
	{
		index++;

		if(IsPC(id))
		{
			// SBS ���� ���. 
			if(CR[id]->IsSBS())
			{
				pclstatistic->siCurSBSUserNum ++;
			}

			// ����Ʈ ���� ���. 
			if(CR[id]->pclCI->clIP.GetLevel() >= 50)
			{
				for(i = 0;i < MAX_QUEST_TYPE;i++)
				{
					// ������ ����Ʈ�� ã�� �����ٸ� Ŭ������ ���̴�. 
					if(CR[id]->GetMinBit(i) < 0)
					{
						pclstatistic->siQuestClear[i][0] ++;
					}
					else
					{
						pclstatistic->siQuestClear[i][1] ++;
					}
				}
			}

			if(CR[id]->GetCurrentOrder()  == ORDER_FISH)
			{
				pclstatistic->siFishUserNumber++;
			}
			if(CR[id]->GetCurrentOrder()  == ORDER_FARM)
			{
				pclstatistic->siFarmUserNumber++;
			}
			if(CR[id]->GetCurrentOrder()  == ORDER_MINE)
			{
				pclstatistic->siMineUserNumber++;
			}

		}
		else if(IsNPC(id))
		{
			// ��ȯ�� ���� �ľ�. 
			if(CR[id]->clIdentity.siIdentity == IDENTITY_PERSON_SUMMON)
			{
				pclstatistic->siSummonNumber++;
			}
		}
	}
}

void cltCharManager::GiveItemForVillageMember(cltItem clitem , SI32 villageunique,SI32 inputmode /* = INPUTITEMPERSON_OPTION_FEAST  */)
{
	if(villageunique <= 0)return ;

	SI32 i = 0 ;
	SI32 id = 0 ;
	SI32 index = 0;
#ifdef _SAFE_MEMORY
	NSafeTArray<SI32, MAX_USER_GIVEITEM_FROM_VILLAGE>	list;
	NSafeTArray<SI32, MAX_USER_GIVEITEM_FROM_VILLAGE>	numlist;
#else
	SI32 list[MAX_USER_GIVEITEM_FROM_VILLAGE] = {0,};
	SI32 numlist[MAX_USER_GIVEITEM_FROM_VILLAGE] = {0,};
#endif
	SI32 listindex = 0 ;
	while(id = GetCharOrder(index))
	{
		index++;

		if(IsPC(id) == false)continue;
		if(CR[id]->GetMapIndex() != villageunique)continue;
		if ( listindex >= MAX_USER_GIVEITEM_FROM_VILLAGE ) break;

		list[listindex] = id ;
		listindex ++ ;
	}

	SI16 maxitemnum = clitem.siItemNum ;

	while ( maxitemnum > 0 )
	{
		if ( maxitemnum > listindex )
		{
			for ( i = 0 ; i< listindex ; i ++ )
			{
				id = list[i] ;
				if(IsPC(id) == false)continue;
				if(CR[id]->GetMapIndex() != villageunique)continue;

				numlist[i] ++ ;
				maxitemnum -- ;
			}
		}
		else
		{
			for ( i = 0 ; i < maxitemnum ; i ++ )
			{
				id = list[rand() % listindex] ;
				if(IsPC(id) == false)continue;
				if(CR[id]->GetMapIndex() != villageunique)continue;

				numlist[i] ++ ;
				maxitemnum -- ;
			}
		}
	}

	for ( i = 0 ; i < listindex ; i ++ )
	{
		id = list[i] ;
		if(IsPC(id) == false)continue;
		if(CR[id]->GetMapIndex() != villageunique)continue;
		if(numlist[i] <= 0 )continue;

		// ĳ���Ϳ��� �������� �ְ� �ʿ��� ��ġ�� ���Ѵ�. 
		cltCharServer* pclchar = (cltCharServer* )CR[id];

		//cyj pclchar->GiveItem(clitem.siUnique, numlist[i], inputmode,0);
		pclchar->GiveItem(clitem.siUnique, numlist[i], inputmode,clitem.uiDateUseDay);
	}
}

// ����ڿ��� �������� �ش�. 
void cltCharManager::GiveItem(SI16 charunique, 
							  SI16 itemunique, SI16 itemnum, SI16 itemgivemode, SI32 inputmode, SI32 usedate, 
							  SI32 senderID, SI32 level, SI32 VillageUnique, SI32 EnchantType, SI32 EnchantLevel, SI32 SlotNum )
{

	if ( itemunique < 1 || itemnum < 1 )
		return;

	SI32 id;

	switch( itemgivemode )
	{
	case GIVEITEM_MODE_ADULT:
		{
			SI32 index = 0;
			while(id = GetCharOrder(index))
			{
				index++;

				if(IsPC(id) == false)continue;
				if(CR[id]->pclCI->clIP.GetLevel() < FATHER_LEVEL)continue;

				// ĳ���Ϳ��� �������� �ְ� �ʿ��� ��ġ�� ���Ѵ�. 
				cltCharServer* pclchar = (cltCharServer* )CR[id];
				pclchar->GiveItem(itemunique, itemnum, inputmode,usedate);

			}
		}
		break;
	case GIVEITEM_MODE_ALL:
		{
			SI32 index = 0;
			while(id = GetCharOrder(index))
			{
				index++;

				if(IsPC(id) == false)continue;

				// ĳ���Ϳ��� �������� �ְ� �ʿ��� ��ġ�� ���Ѵ�. 
				cltCharServer* pclchar = (cltCharServer* )CR[id];
				pclchar->GiveItem(itemunique, itemnum, inputmode,usedate);

			}
		}
		break;
	case GIVEITEM_MODE_PERSON:
		{
			// ĳ���Ϳ��� �������� �ְ� �ʿ��� ��ġ�� ���Ѵ�. 
			cltCharServer* pclchar = (cltCharServer* )CR[ charunique ];
			SI32 rtnVal = pclchar->GiveItem(itemunique, itemnum, inputmode,usedate);
			//SI32 rtnVal = pclchar->GiveItem(itemunique, itemnum, INPUTITEMPERSON_OPTION_MASTER,usedate);
			if( rtnVal == GIVEITEM_RESULT_NOINVEN )
			{
				if ( senderID )
				{
					((cltServer*)pclClient)->SendServerResponseMsg(0, SRVAL_FAIL_GIVEITEM_NOINVEN,  0, 0, senderID);
				}
			}
		}
		break;
	case GIVEITEM_MODE_MINLEVEL:
		{
			SI32 index = 0;
			while(id = GetCharOrder(index))
			{
				index++;

				if(IsPC(id) == false)continue;
				if(CR[id]->pclCI->clIP.GetLevel() < level)continue;

				// ĳ���Ϳ��� �������� �ְ� �ʿ��� ��ġ�� ���Ѵ�. 
				cltCharServer* pclchar = (cltCharServer* )CR[id];
				pclchar->GiveItem(itemunique, itemnum, inputmode,usedate);

			}
		}
		break;
	case GIVEITEM_MODE_VILLAGER:
		{
			SI32 id;
			for( id = 1; id < MAX_PERSON_NUMBER; id++ )
			{
				cltCharServer* pclchar = (cltCharServer* )CR[id];
				if(pclchar == NULL)							continue;
				if(pclchar->IsValidConnection() == false)	continue;

				if(VillageUnique == 0)
				{
					// �Ҽ� ������ ������ �ش�.

					if( CR[id]->pclCI->clBI.siHomeVillage > 0 )
						pclchar->GiveItem(itemunique, itemnum, inputmode, usedate);
				}
				else
				{
					// �Ҽ� ���� �ֹ����׸� �ش�.
					if( CR[id]->pclCI->clBI.siHomeVillage == VillageUnique )
						pclchar->GiveItem(itemunique, itemnum, inputmode, usedate);
				}
			}
		}
		break;
	case GIVEITEM_MODE_SEQGMCOMMAND:
		{
			// ĳ���Ϳ��� �������� �ְ� �ʿ��� ��ġ�� ���Ѵ�. 
			cltCharServer* pclchar = (cltCharServer* )CR[ charunique ];
			SI32 rtnVal = pclchar->GiveItem(itemunique, itemnum, INPUTITEMPERSON_OPTION_SEQGMCOMMAND,usedate, 0, EnchantType, EnchantLevel, SlotNum);

			if( rtnVal == GIVEITEM_RESULT_NOINVEN )
			{
				if ( senderID )
				{
					((cltServer*)pclClient)->SendServerResponseMsg(0, SRVAL_FAIL_GIVEITEM_NOINVEN,  0, 0, senderID);
				}
			}
			else if(rtnVal == GIVEITEM_RESULT_SUCCESS)
			{
				if ( senderID )
				{
					((cltServer*)pclClient)->SendServerResponseMsg(0, SRVAL_RETURN_EQUIPMENTCREATE_DLG_SUCCESS,  0, 0, senderID);
				}
			}
		}
		break;

	default:
		break;
	}

	//else if( itemgivemode == GIVEITEM_MODE_PCROOM ) 
	//{
	//	SI32 index = 0;
	//	while(id = GetCharOrder(index))
	//	{
	//		index++;

	//		if(IsPC(id) == false)continue;
	//	
	//		cltCharServer* pclchar = (cltCharServer* )CR[id];

	//		if(pclchar->pclCI->clBI.IsPCRoom() == false )continue;

	//		// ĳ���Ϳ��� �������� �ְ� �ʿ��� ��ġ�� ���Ѵ�. 
	//		pclchar->GiveItem(itemunique, itemnum, inputmode,usedate);
	//	}
	//}
}

// ���� ������ �����Ѵ�. 
bool cltCharManager::SortBlackWarRecord(SI32* pchar, SI32 num)
{
	SI32 mindata	= 0;
	SI32* pdata = NULL;


	pdata  = new SI32 [num];
	if(pdata == NULL)
		return false;

	ZeroMemory(pdata, sizeof(SI32) * num);

	SI32 id			= 0;
	SI32 index		= 0;
	while(id = GetCharOrder(index))
	{
		index++;

		if( IsPC(id) == false )
			continue;

		cltCharServer* pclchar = (cltCharServer* )CR[id];
		if ( pclchar == NULL ) 
			continue;

		//[�߰� : Ȳ���� 2008. 1. 23 => ���� ���� ���� ĳ���� ���� ��Ʈ�� ������ ������.]
		if(pclClient->IsCountrySwitch(Switch_BlackWarReform)									&&
			(true == pclClient->pclBlackWarManager->IsBlackWar())								/*&&
																								(BLACKARMY_MAP_STARTINDEX > pclchar->GetCurrentVillageUnique()						||
																								BLACKARMY_MAP_STARTINDEX+MAX_BLACKARMY_MAP_CNT < pclchar->GetCurrentVillageUnique())*/		)
		{
			SortAdd(pchar, pdata, &mindata, id, pclchar->siBlackWarEnemyKillScore, num);
		}
		else
		{
			SortAdd(pchar, pdata, &mindata, id, pclchar->siBlackWarEnemyKillNumber, num);
		}
	}


	if(pdata)
	{
		delete[] pdata;
		pdata  = NULL;
	}

	return true;

}

// ���� ������ �����Ѵ�. 
bool cltCharManager::SortWarRecord(SI32* pchar, SI32 num, SI32 warmapindex)
{
	SI32 mindata	= 0;
	SI32* pdata = NULL;


	pdata  = new SI32 [num];
	if(pdata == NULL)return false;

	ZeroMemory(pdata, sizeof(SI32) * num);

	SI32 id			= 0;
	SI32 index		= 0;
	while(id = GetCharOrder(index))
	{
		index++;

		if(IsPC(id) == false)continue;
		// ĳ���Ϳ��� �������� �ְ� �ʿ��� ��ġ�� ���Ѵ�. 
		cltCharServer* pclchar = (cltCharServer* )CR[id];
		if ( pclchar == NULL ) continue ;

		if ( pclchar->siBlackArmyWarMapIndex != warmapindex) continue;

		SortAdd(pchar, pdata, &mindata, id, pclchar->siWarEnemyKillNumber, num);
	}


	//// ���Ͽ� ����Ѵ�. 
	//FILE *fp = _tfopen( TEXT("Config/WarRecord.txt"), TEXT("wt") );

	//if (fp)
	//{
	//	SI32 i;
	//	TCHAR buffer[256];

	//	for(i = 0; i < num;i++)
	//	{
	//		id = pchar[i];
	//		if(IsPC(id) == false)continue;
	//		if(IsValidID(id) == false)continue;

	//		cltCharServer* pclchar = (cltCharServer* )CR[id];

	//		const TCHAR* pname = CR[id]->GetName();

	//		StringCchPrintf(buffer, 256, TEXT("%d.[%s] - %d\r\n"), 
	//			i + 1, pname, pclchar->siWarEnemyKillNumber);

	//		_ftprintf( fp, buffer);
	//	}

	//	fclose( fp );
	//}
	//


	if(pdata)
	{
		delete[] pdata;
		pdata  = NULL;
	}

	return true;

}

// ���� �Ƿ� ������ �ϳ� ���Ѵ�. 
bool cltCharManager::GetPersonalShopMakeInfo(TCHAR* pname, SI16 txtSize, SI16* pitemunique, SI32* pitemnum, GMONEY* pfee, SI32* pskill, SI32* phand)
{
	static SI32 index = 0;
	SI32 i;

	for(i = index;i < MAX_CHAR_NUMBER;i++)
	{
		if(IsValidID(i) == false)continue;
		if(IsPC(i) == false)continue;

		cltCharServer* pclchar = (cltCharServer* )CR[i];
		if ( pclchar == NULL ) continue ;


		// ���� �Ƿ� ���� �ƴϸ� .. 
		if(pclchar->clShopData.m_siShopMode != PSHOP_MODE_MAKEITEM)continue;


		StringCchCopy(pname, txtSize, pclchar->GetName());

		if(pclchar->clShopData.m_clItem[0].siUnique)
		{
			*pitemunique	= pclchar->clShopData.m_clItem[0].siUnique;
			*pitemnum		= pclchar->clShopData.m_clItem[0].siItemNum;
			if(*pitemnum <= 0)continue;		// ������ ��ȿ�ϸ� .. 

			*pfee			= pclchar->clShopData.m_siPrice[0];
			*pskill			= pclchar->clShopData.m_siSkillLevel;
			*phand			= pclchar->clShopData.m_siHandPoint;

			index			= i;			
			return true;
		}

	}

	index	= 0;
	return false;
}

cltCharClient* cltCharManager::GetCharClient(SI32 id)
{
	if( pclClient->GameMode != GAMEMODE_CLIENT )		return NULL;

	if( IsValidID(id) == false )
	{
		//#ifdef _DEBUG
		//		_ASSERT( IsValidID(id) );
		//#endif
		return NULL;
	}

	return (cltCharClient*)CR[id];
}

cltCharServer* cltCharManager::GetCharServer(SI32 id)
{
	if( pclClient->GameMode != GAMEMODE_SERVER )		return NULL;

	if( IsValidID(id) == false )
	{
		//#ifdef _DEBUG
		//		_ASSERT( IsValidID(id) );
		//#endif
		return NULL;
	}

	return (cltCharServer*)CR[id];
}
