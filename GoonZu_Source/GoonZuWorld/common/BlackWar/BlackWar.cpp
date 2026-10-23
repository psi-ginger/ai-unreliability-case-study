#include "BlackWar.h"

#include "../CommonLogic/CommonLogic.h"
#include "../Server/Server.h"
#include "../Server/Minister/MinisterBase.h"
#include "../Server/Minister/Minister-Yezo.h"
#include "../Server/Minister/MinisterMgr.h"
//-----------------------------------------
// Common
//-----------------------------------------
#include "../Common/Char/KindInfo/KindInfo.h"
#include "../Common/Order/Order.h"
#include "Char/CharManager/CharManager.h"

#include "MsgType-System.h"

#include "../../Client/Music/Music.h"
#include "../../Client/Interface/SoundOnOffDlg/SoundOnOffDlg.h"

#include "../../../DBManager/GameDBManager_World/DBMsg-Item.h"
#include "../../../DBManager/GameDBManager_World/DBMsg-System.h"
#include "../../../DBManager/GameDBManager_World/DBMsg-Person.h"
#include "../../../DBManager/GameDBManager_World/DBMsg-Village.h"
#include "../../../DBManager/GameDBManager_World/DBMsg-Structure.h"
#include "../../../DBManager/GameDBManager_World/DBMsg-Cityhall.h"
#include "..\CommonLogic\Msg\MsgType-Village.h"

#include <MsgType-Person.h>

#include "../CommonLogic/MsgRval-Define.h"

extern cltCommonLogic* pclClient;

CBlackWarManager::CBlackWarManager()
{
	Init();
}


CBlackWarManager::~CBlackWarManager()
{
}

void CBlackWarManager::Init()
{
	for( SI32 i = 0; i < MAX_BLACKWAR_UNIT; ++i )
	{
		m_clBlackWarUnit[i].Init();
	}

	m_bBlackWarSwitch	= false;	
	m_siTotalUnitNumber = 0;			
	m_siCurUnitNumber	= 0;			
	m_siMadeNumber		= 0;	
	m_siAddUnitNumber	= 0;
	m_siCasualtyNumber	= 0;			
	m_dwLeftTimeClock	= 0;		
	m_dwBlackWarStartClock	= 0;	
	m_siLeftUnitNumber	= 0;				
	m_bDrawSwitch		= false;	
	m_siDrawCmdFont		= 0;	
	m_dwDrawStartClock	= 0;	
	m_bDieBossSwitch	= false;
	m_bMakeBossSwitch	= false;
	m_bMakeMiddleBossSwitch[0] = false;
	m_bMakeMiddleBossSwitch[1] = false;
	m_bMakeMiddleBossSwitch[2] = false;
	m_bMakeMiddleBossSwitch[3] = false;
	m_siRewardCharID[0] = 0;
	m_siRewardCharID[1] = 0;
	m_siRewardCharID[2] = 0;
	m_siRewardCharID[3] = 0;
	m_siRewardCharID[4] = 0;
	m_bWinSwith = false;
	m_siStartHour = 0;
	m_siKillBlackUnitNumber = 0;
	m_dwBlackWarEndClock = 0;
	m_siMVPPersonID = 0;

	srand( (unsigned)time( NULL ) );
}

// 전쟁 유닛를 추가한다. 
void CBlackWarManager::AddBlackWarUnit( SI32 Kind, SI32 UnitNumber )
{
	SI32 i;

	for(i = 0;i < MAX_BLACKWAR_UNIT; ++i)
	{
		if( Kind == m_clBlackWarUnit[i].GetKind() )
		{
			m_clBlackWarUnit[i].m_siNum  += UnitNumber;
		}
	}
}


// 전쟁을 기획한다. 
bool CBlackWarManager::MakeWar( DWORD Clock, SI32 StartHour )
{
	//[추가 : 황진성 2008. 1. 22 => 흑의 군단 마을 점수 정보 초기화.]
	pclClient->pclVillageManager->BlackWarRecordInit();

	Init();

	// 전쟁에 참여할 병사수와 병종을 설정한다 
	FILE* fp = fopen("ServerData\\War\\BlackWarInfo.txt", "rt");
	if(fp == NULL)
		return false;

	SI32 num	= 0;
	SI32 rate	= 0;
	SI32 basic	= 0;
	SI32 rand	= 0;
	SI32 mode	= 0;
	TCHAR charname[128] = {'\0'};
	cltServer* pclserver = (cltServer*)pclClient;

	SI32 i;
	TCHAR buffer[1024] = {'\0'};
	for(i = 0;i<5;i++)
	{
		fgets(buffer, 1024, fp);
	}

	while( fgets(buffer, 1024, fp) != NULL)
	{
		if( m_siAddUnitNumber >= MAX_BLACKWAR_UNIT )
		{
			MsgBox("fdj82jfd", "dsif83jf");
			continue;
		}

		sscanf(buffer, "%s %d %d %d %d %d", charname, &num, &rate, &basic, &rand, &mode);

		SI32 kind = pclClient->pclKindInfoSet->FindKindFromCode(charname);
		if( kind > 0 )
		{
			m_clBlackWarUnit[m_siAddUnitNumber].m_siKind		 = kind;
			m_clBlackWarUnit[m_siAddUnitNumber].m_siNum			 = num;

			m_clBlackWarUnit[m_siAddUnitNumber].m_siRate		 = rate;
			m_clBlackWarUnit[m_siAddUnitNumber].m_siBasicLevel	 = basic;
			m_clBlackWarUnit[m_siAddUnitNumber].m_siRandLevel	 = rand;


			m_clBlackWarUnit[m_siAddUnitNumber].m_siCurNum		= 0;
			m_siTotalUnitNumber += m_clBlackWarUnit[m_siAddUnitNumber].m_siNum;
			m_siAddUnitNumber++;
		}
	}

	fclose(fp);


	// 보스 때문에 한마리 더 더해준다.
	//	m_siTotalUnitNumber++;
	m_siLeftUnitNumber		= m_siTotalUnitNumber;
	m_siMadeNumber			= 0;
	m_siCasualtyNumber		= 0;

#ifdef _DEBUG
	m_dwLeftTimeClock		= 1000 * 60 * 60;// 30분. 
#else
	m_dwLeftTimeClock		= 1000 * 60 * 60;// 30분. 
#endif
	m_dwBlackWarStartClock		= pclClient->CurrentClock;

	m_bBlackWarSwitch = true;

	//--------------------------------------------------
	// 모든 사용자의 WarKillerNum변수를 초기화한다. 
	//--------------------------------------------------
	SI32 index = 0;
	SI32 id;
	while(id = pclClient->pclCM->GetCharOrder(index))
	{
		index++;
		cltCharServer* pclchar = (cltCharServer*)pclClient->pclCM->CR[id];

		if( NULL != pclchar )
		{
			if( false == pclClient->pclCM->IsValidID(id) )		
				continue;

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

	//--------------------------------------------------
	// 모든 클라이언트들에게 전쟁이 시작되었음을 알린다. 
	//--------------------------------------------------
	SendWarMsg( BLACKWAR_CMD_BREAK, -1, 0, 0, false );

	SetStartHour( StartHour );

	return true;
}

void CBlackWarManager::SendWarMsg( SI32 Cmd, SI32 Mapindex, SI32 X, SI32 Y, bool MakeBoss )
{
	SI32 siWarSortRecordChar[MAX_WARRANK_PERSON_PER_MAP];
	TCHAR szWarSortCharName[MAX_WARRANK_PERSON_PER_MAP][MAX_PLAYER_NAME];
	SI32 siWarScore[MAX_WARRANK_PERSON_PER_MAP];

	ZeroMemory(siWarSortRecordChar, sizeof(SI32) * MAX_WARRANK_PERSON_PER_MAP);
	pclClient->pclCM->SortBlackWarRecord( siWarSortRecordChar, MAX_WARRANK_PERSON_PER_MAP );

	SI32 id;
	SI32 i;
	
	for (i = 0; i < MAX_WARRANK_PERSON_PER_MAP; i++)
	{
		id = siWarSortRecordChar[i];

		if (id > 0 && pclClient->pclCM->IsValidID(id))
		{
			const TCHAR *pName = pclClient->pclCM->CR[siWarSortRecordChar[i]]->GetName();

			if (pName != NULL && _tcscmp(pName, "") != 0)
			{
				StringCchPrintf(szWarSortCharName[i], MAX_PLAYER_NAME, TEXT("%s"), pName);
				siWarScore[i] = pclClient->pclCM->CR[siWarSortRecordChar[i]]->siBlackWarEnemyKillNumber;
				
				// [추가 : 황진성 2008. 1. 22 => 흑의 군단 제거로 얻은 점수 클라에 보내기.]
				if(pclClient->IsCountrySwitch(Switch_BlackWarReform))
				{
					siWarScore[i] = pclClient->pclCM->CR[siWarSortRecordChar[i]]->siBlackWarEnemyKillScore;
				}

				m_siRewardCharID[i] = id;
			}
			else
			{
				StringCchPrintf(szWarSortCharName[i], MAX_PLAYER_NAME, TEXT(""));
				siWarScore[i] = 0;
			}
		}
		else
		{
			StringCchPrintf(szWarSortCharName[i], MAX_PLAYER_NAME, TEXT(""));
			siWarScore[i] = 0;
		}
	}
	
	//[추가 : 황진성 2008. 1. 22 => 마을 점수관련해서 클라에게 줄 정보.]
	TCHAR szSortVillageNames[MAX_WARRANK_PERSON_PER_MAP][MAX_PLAYER_NAME];
	SI32  siWarVillageScores[MAX_WARRANK_PERSON_PER_MAP];
	ZeroMemory(szSortVillageNames, sizeof(szSortVillageNames));
	ZeroMemory(siWarVillageScores, sizeof(siWarVillageScores));

	if(pclClient->IsCountrySwitch(Switch_BlackWarReform)) 
	{
		for (i = 0; i < MAX_WARRANK_PERSON_PER_MAP; i++)
		{
			// [추가 : 황진성 2008. 1. 22 => 흑의 군단 제거로 얻은 마을점수 클라에 보내기.]	
			if(pclClient->pclVillageManager->cBlackWarVillageScore[i].GetVillageScore() > 0 )
			{
				SI16   VillageUnique = pclClient->pclVillageManager->cBlackWarVillageScore[i].GetVillageUnique();
				TCHAR* VillageName   = pclClient->pclVillageManager->GetName(VillageUnique);
				memcpy(szSortVillageNames[i], VillageName, sizeof(szSortVillageNames[i]));

				siWarVillageScores[i] = pclClient->pclVillageManager->cBlackWarVillageScore[i].GetVillageScore();
			}
			else
			{
				ZeroMemory(szSortVillageNames[i], sizeof(szSortVillageNames[i]));
				siWarVillageScores[i] = 0;
			}
		}
	}
	
	cltGameMsgResponse_BlackWar clBlackWar( Cmd, m_dwLeftTimeClock, m_siLeftUnitNumber, m_siTotalUnitNumber, m_siCasualtyNumber, Mapindex, X, Y, MakeBoss, 
											(TCHAR**)szWarSortCharName, (SI32*)siWarScore, (TCHAR**)szSortVillageNames, (SI32*)siWarVillageScores );
	cltMsg clMsg( GAMEMSG_RESPONSE_BLACKWAR, sizeof(clBlackWar), (BYTE*)&clBlackWar );

	SI32 index = 0;

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


void CBlackWarManager::WarEnd()
{
	m_bBlackWarSwitch = false;

	//----------------------------------
	// 전쟁 병사들의 전쟁 목표를 없앤다.
	//----------------------------------

	SI32 index = 0;
	SI32 id = 0;
	while( id = pclClient->pclCM->GetCharOrder(index) )
	{
		index++;

		cltCharServer* pclchar = (cltCharServer*)pclClient->pclCM->CR[id];

		if( NULL != pclchar)
		{
			if( pclClient->pclCM->IsValidID(id) == FALSE )		
				continue;

			if( pclchar->siCharMode == CHARMODE_BLACKWAR )	
			{
				// 셀 내에서의 정보를 삭제한다. 
				pclClient->pclCM->CR[id]->DeleteInCell();
				if(pclClient->pclCM->DeleteChar( id ) == TRUE)
				{
					index--;
				}
			}
		}
	}
}

void CBlackWarManager::Action()
{
	if( 0 < m_siMVPPersonID )
	{
		// 하루 지나면 MVP 마크 초기화
		if( TABS(pclClient->CurrentClock - m_dwBlackWarEndClock ) > 1000 * 60 * 60 * 24 )
		{
			SI32 id = pclClient->pclCM->GetIDFromPersonID( m_siMVPPersonID );
			// MVP마크 초기화 해준다
			if( true == pclClient->pclCM->IsValidID(id) )
			{
				pclClient->pclCM->CR[id]->pclCI->clBI.uiGMMode = GMMODE_ATB_NONE;
				pclClient->pclCM->CR[id]->SetUpdateSwitch(UPDATE_BASICINFO, true, 0 );
			}

			m_siMVPPersonID = 0;
		}
	}

	SI32 i;
	SI32 level;
	static SI32 portervary = 0;
	static SI32 PositionIndex = 0;
	cltServer* pclserver = (cltServer*)pclClient;

	// 흑의군단 전쟁
	if( false == m_bBlackWarSwitch )
		return ;

	//--------------------------------------------------
	// 시간 남은 것과 병사 남은 것을 비교하여 조치한다. 
	//--------------------------------------------------
	m_siLeftUnitNumber = max(0, m_siTotalUnitNumber - m_siMadeNumber) + m_siCurUnitNumber;
	
	if( pclClient->IsCountrySwitch( Switch_BlackWarReform ) )
	{
		//흑의 군단 남은 병사는 전체병사 - 죽은병사
		m_siLeftUnitNumber = m_siTotalUnitNumber - m_siKillBlackUnitNumber;
	}
	
	//------------------------------------
	// 군사들을 배치하여 자금성으로 보낸다.
	//------------------------------------
	bool makeBoss = false;
	if(pclClient->GetFrame() % 5 == 0)
	{ 
		//몬스터가 일정 수 이상이면 더이상 배치해서는 안된다. 속도 저하 방지. 
		//if( pclClient->pclCM->pclIDManager->GetNPCCharNumber() < (MAX_NPC_NUMBER-4000) )
		// [진성] 마을당 30~40마리씩 생성되도록 수정. npc수가 (MAX_NPC_NUMBER - 2000) 이하일때만.
		if(m_siCurUnitNumber < 1500 && pclClient->pclCM->pclIDManager->GetNPCCharNumber() < (MAX_NPC_NUMBER - 2000))
		{    
			//--------------------------------
			// 병사들을 배치한다. 
			//--------------------------------
			if(m_siMadeNumber <= m_siTotalUnitNumber) 
			{
				for(i = 0; i < m_siAddUnitNumber;i++)
				{
					if(m_clBlackWarUnit[ i ].m_siCurNum >= m_clBlackWarUnit[ i ].m_siNum)
						continue;
					if(m_clBlackWarUnit[ i ].m_siRate < rand() % 100)
						continue;

					SI32 kind = m_clBlackWarUnit[ i ].m_siKind;

					SI32 pos = 0 ;
					SI32 x, y;
					SI32 tempx, tempy;
					SI32 mapindex = 0;

					mapindex = 0;
					SI32 villageunique = rand() % ( CUR_GLOBAL_VILLAGE_LIMIT + 1 ) ; 

					// 타르비즈는 뺀다
					if( 21 ==  villageunique )
						continue;

					if(pclClient->pclVillageManager->pclVillageInfo[villageunique] == NULL)continue;

					tempx = pclClient->pclVillageManager->pclVillageInfo[villageunique]->PosX + (5 - rand() % 10);
					tempy = pclClient->pclVillageManager->pclVillageInfo[villageunique]->PosY + (5 - rand() % 10);

					// 몬스터를 배치할 적절한 곳을 찾는다. 
					if(pclClient->pclMapManager->pclMapArray[mapindex]->FindEmptyAreaForCharLocate(kind, tempx, tempy, 10, &x, &y, true) == false)
					{
						continue;
					}

					level = m_clBlackWarUnit[ i ].m_siBasicLevel + rand() % m_clBlackWarUnit[ i ].m_siRandLevel;

					cltInnerStatusInfo clinnerstatus;
					clinnerstatus.clEnemyType.Set(ENEMYTYPE_ALLPC, 0);

					SI32 id = pclserver->SetNPCChar(kind, VILLAGE_NONE, NULL, level, 0, x, y, mapindex, NPCITEMMODE_NORMAL, &clinnerstatus );
					if(id)
					{
						m_siMadeNumber ++;
						m_clBlackWarUnit[ i ].m_siCurNum++;

						cltCharServer* pclchar = (cltCharServer*)pclClient->pclCM->CR[id];
						pclchar->siCharMode = CHARMODE_BLACKWAR;

					/*	SI32 villageunique = pclClient->pclVillageManager->FindRandVillage();	// 공격할 마을  
						if(villageunique)
						{
							pclchar->siCharMode			= CHARMODE_BLACKWAR;
							pclchar->siCharModePara1	= 1;
						}
						else
						{
							pclchar->siCharMode = CHARMODE_BLACKWAR;
						}*/
					}
				}

				// 전체 병력의 1/5가 잡힐때 마다 중간 보스 리젠
				for( SI32 j = 0; j < 4; ++j )
				{
					// 한번 생성에 종류별로 10마리씩
					if( m_bMakeMiddleBossSwitch[j] )
						continue;

					if( m_siKillBlackUnitNumber >= ( ( m_siTotalUnitNumber / 5 ) * ( j + 1 ) ) )
					{
						SI32 i = 0;
						while( i < 10 )
						{
							SI32 pos = 0 ;
							SI32 x, y;
							SI32 tempx, tempy;
							SI32 mapindex = 0;

							SI32 villageunique = 0;

							villageunique = rand() % ( CUR_GLOBAL_VILLAGE_LIMIT + 1 ) ; 
							if(pclClient->pclVillageManager->pclVillageInfo[villageunique] == NULL)
								continue;

							tempx = pclClient->pclVillageManager->pclVillageInfo[villageunique]->PosX + (5 - rand() % 10);
							tempy = pclClient->pclVillageManager->pclVillageInfo[villageunique]->PosY + (5 - rand() % 10);

							SI32 Kind = pclClient->GetUniqueFromHash(TEXT("KIND_YELLOWSHABEL"));
							// 몬스터를 배치할 적절한 곳을 찾는다. 
							if(pclClient->pclMapManager->pclMapArray[mapindex]->FindEmptyAreaForCharLocate(Kind, tempx, tempy, 10, &x, &y, true) == true)
							{
								cltInnerStatusInfo clinnerstatus;
								clinnerstatus.clEnemyType.Set(ENEMYTYPE_ALLPC, 0);

								SI32 id = pclserver->SetNPCChar(Kind, VILLAGE_NONE, NULL, 60, 0, x, y, mapindex, NPCITEMMODE_NORMAL, &clinnerstatus );
								if(id)
								{
									m_siTotalUnitNumber++;
									m_siMadeNumber ++;

									cltCharServer* pclchar = (cltCharServer*)pclClient->pclCM->CR[id];
									pclchar->siCharMode = CHARMODE_BLACKWAR;
								}
							}

							tempx = pclClient->pclVillageManager->pclVillageInfo[villageunique]->PosX + (5 - rand() % 10);
							tempy = pclClient->pclVillageManager->pclVillageInfo[villageunique]->PosY + (5 - rand() % 10);

							Kind = pclClient->GetUniqueFromHash(TEXT("KIND_GREENSHABEL"));
							// 몬스터를 배치할 적절한 곳을 찾는다. 
							if(pclClient->pclMapManager->pclMapArray[mapindex]->FindEmptyAreaForCharLocate(Kind, tempx, tempy, 10, &x, &y, true) == true)
							{
								cltInnerStatusInfo clinnerstatus;
								clinnerstatus.clEnemyType.Set(ENEMYTYPE_ALLPC, 0);

								SI32 id = pclserver->SetNPCChar(Kind, VILLAGE_NONE, NULL, 70, 0, x, y, mapindex, NPCITEMMODE_NORMAL, &clinnerstatus );
								if(id)
								{
									m_siTotalUnitNumber++;
									m_siMadeNumber ++;

									cltCharServer* pclchar = (cltCharServer*)pclClient->pclCM->CR[id];
									pclchar->siCharMode = CHARMODE_BLACKWAR;
								}
							}

							tempx = pclClient->pclVillageManager->pclVillageInfo[villageunique]->PosX + (5 - rand() % 10);
							tempy = pclClient->pclVillageManager->pclVillageInfo[villageunique]->PosY + (5 - rand() % 10);

							Kind = pclClient->GetUniqueFromHash(TEXT("KIND_REDSHABEL"));
							// 몬스터를 배치할 적절한 곳을 찾는다. 
							if(pclClient->pclMapManager->pclMapArray[mapindex]->FindEmptyAreaForCharLocate(Kind, tempx, tempy, 10, &x, &y, true) == true)
							{
								cltInnerStatusInfo clinnerstatus;
								clinnerstatus.clEnemyType.Set(ENEMYTYPE_ALLPC, 0);

								SI32 id = pclserver->SetNPCChar(Kind, VILLAGE_NONE, NULL, 80, 0, x, y, mapindex, NPCITEMMODE_NORMAL, &clinnerstatus );
								if(id)
								{
									m_siTotalUnitNumber++;
									m_siMadeNumber ++;

									cltCharServer* pclchar = (cltCharServer*)pclClient->pclCM->CR[id];
									pclchar->siCharMode = CHARMODE_BLACKWAR;
								}
							}

							tempx = pclClient->pclVillageManager->pclVillageInfo[villageunique]->PosX + (5 - rand() % 10);
							tempy = pclClient->pclVillageManager->pclVillageInfo[villageunique]->PosY + (5 - rand() % 10);

							Kind = pclClient->GetUniqueFromHash(TEXT("KIND_BLUESHABEL"));
							// 몬스터를 배치할 적절한 곳을 찾는다. 
							if(pclClient->pclMapManager->pclMapArray[mapindex]->FindEmptyAreaForCharLocate(Kind, tempx, tempy, 10, &x, &y, true) == true)
							{
								cltInnerStatusInfo clinnerstatus;
								clinnerstatus.clEnemyType.Set(ENEMYTYPE_ALLPC, 0);

								SI32 id = pclserver->SetNPCChar(Kind, VILLAGE_NONE, NULL, 90, 0, x, y, mapindex, NPCITEMMODE_NORMAL, &clinnerstatus );
								if(id)
								{
									m_siTotalUnitNumber++;
									m_siMadeNumber ++;

									cltCharServer* pclchar = (cltCharServer*)pclClient->pclCM->CR[id];
									pclchar->siCharMode = CHARMODE_BLACKWAR;
								}
							}
							i++;
						}

						m_bMakeMiddleBossSwitch[j] = true;
					}
				}

				if( false == m_bMakeBossSwitch )
				{
					// 흑의군단 보스는 병력이 70%이상 나온후에 나온다.
					SI32 Value = 7;
					
					//[진성] 흑의군단 보스는 병력이 10% 남았을때 나온다. => 2008-4-15
					if( pclClient->IsCountrySwitch( Switch_BlackWarReform ) )
						Value = 9; 

					if( m_siKillBlackUnitNumber >= ( m_siTotalUnitNumber * Value / 10 ) )
					{
						while( !m_bMakeBossSwitch )
						{
							SI32 pos = 0 ;
							SI32 x, y;
							SI32 tempx, tempy;
							SI32 mapindex = 0;

							SI32 villageunique = 0;

							villageunique = rand() % ( CUR_GLOBAL_VILLAGE_LIMIT + 1 ) ; 
							if(pclClient->pclVillageManager->pclVillageInfo[villageunique] == NULL)
								continue;

							tempx = pclClient->pclVillageManager->pclVillageInfo[villageunique]->PosX + (5 - rand() % 10);
							tempy = pclClient->pclVillageManager->pclVillageInfo[villageunique]->PosY + (5 - rand() % 10);

							SI32 Kind = pclClient->GetUniqueFromHash(TEXT("KIND_BLACKBOSS"));
							// 몬스터를 배치할 적절한 곳을 찾는다. 
							if(pclClient->pclMapManager->pclMapArray[mapindex]->FindEmptyAreaForCharLocate(Kind, tempx, tempy, 10, &x, &y, true) == true)
							{
								cltInnerStatusInfo clinnerstatus;
								clinnerstatus.clEnemyType.Set(ENEMYTYPE_ALLPC, 0);

								SI32 id = pclserver->SetNPCChar(Kind, VILLAGE_NONE, NULL, 1, 0, x, y, mapindex, NPCITEMMODE_NORMAL, &clinnerstatus );
								if(id)
								{
									m_siTotalUnitNumber++;
									m_siMadeNumber ++;

									cltCharServer* pclchar = (cltCharServer*)pclClient->pclCM->CR[id];

									pclchar->siCharMode = CHARMODE_BLACKWAR;

									m_bMakeBossSwitch = true;
									makeBoss = true;

									pclserver->pclLog->FileConfigPrint("makeboss", "create");
								}
							}
						}
					}
				}
			}
		}
	}

	//--------------------------------------
	// 일정시간마다 병사들의 상황을 체크한다. 
	//---------------------------------------
	static DWORD dwLastWarCheckClock = 0;
	SI32 cmd = 0;
	if(TABS(pclClient->CurrentClock - dwLastWarCheckClock ) > 10000)
	{
		dwLastWarCheckClock = pclClient->CurrentClock;

		//---------------------------
		// 남은 시간을 확인한다. 
		//---------------------------
		DWORD lastclock = TABS(pclClient->CurrentClock - m_dwBlackWarStartClock);
		m_dwBlackWarStartClock	= pclClient->CurrentClock;
		if(m_dwLeftTimeClock > lastclock)
		{
			m_dwLeftTimeClock -= lastclock;
			cmd = BLACKWAR_CMD_GOING;
		}
		else
		{
			m_dwLeftTimeClock = 0;

			// 시간이 종료되면 전쟁을 끝낸다. (패배) 
			SendWarMsg( cmd, 0, 0, 0, false );
			BlackWarReward( false );
			cmd = BLACKWAR_CMD_LOSE;
			WarEnd();
		}

		//-------------------------
		// 남은 병사를 확인한다. 
		//-------------------------
		m_siCurUnitNumber	= 0;

		SI32 index = 0;
		SI32 id;
		SI32 mapindex = -1;
		SI32 x = 0;
		SI32 y = 0;

		//--------------------------------------
		// 적군 상황 파악 
		//--------------------------------------
		while(id = pclClient->pclCM->GetCharOrder(index))
		{
			index++;

			if( IsNPC(pclClient->pclCM->CR[id]->GetCharUnique()) == false )
				continue;
			if( pclClient->pclCM->IsAlive(id) == false )
				continue;

			cltCharServer* pclchar = (cltCharServer*)pclClient->pclCM->CR[id];

			if( pclClient->pclCM->IsAlive(id) && pclchar->siCharMode == CHARMODE_BLACKWAR )
			{
				m_siCurUnitNumber++;

				mapindex	= pclchar->GetMapIndex();
				x			= pclchar->GetX();
				y			= pclchar->GetY();
			}
		}

		if( m_siMadeNumber >= m_siTotalUnitNumber )
		{
			if( m_siCurUnitNumber <= 0 )
			{
				if(cmd != BLACKWAR_CMD_LOSE)
				{
					SendWarMsg( cmd, mapindex, x, y, false );
				
					BlackWarReward( true );
					cmd = BLACKWAR_CMD_WIN;
					WarEnd();

					m_bWinSwith = true;
				}
			}
		}

		/*	// 대장이 죽으면 전쟁에서 이긴다.
		if( true == m_bDieBossSwitch )
		{
		if(cmd != BLACKWAR_CMD_LOSE)
		{
		BlackWarReward();

		cmd = BLACKWAR_CMD_WIN;
		WarEnd();
		}
		}
		*/
		//---------------------------------
		// 전쟁 승패 여부를 기록한다. 
		//---------------------------------
		if(cmd == BLACKWAR_CMD_WIN)
		{
			pclClient->pclLog->FileConfigPrint( "BlackWarResult", "Win LeftTime:%d", m_dwLeftTimeClock );

			FILE* fp = fopen("War\\BlackWarInfo.txt", "wt");
			if(fp == NULL)
				return;

			_ftprintf( fp, "BlackWAr\n" );
			_ftprintf( fp, "2007-04-18\n" );
			_ftprintf( fp, "v0.1\n" );
			_ftprintf( fp, "kkm\n" );
			_ftprintf( fp, "Kind\tNum\tRate\tBasicLevel\tRandomLevel\n" );

			SI32 catarNumber = m_clBlackWarUnit[0].m_siNum + 500;
			if( 10000 <= catarNumber )
				catarNumber = 10000;
			_ftprintf( fp, "KIND_NEWCATAR\t%d\t50\t1\t5\n", catarNumber );

			SI32 rifleNumber = m_clBlackWarUnit[1].m_siNum + 500;
			if( 10000 <= rifleNumber )
				rifleNumber = 10000;
			_ftprintf( fp, "KIND_NEWRIFLE\t%d\t50\t1\t5\n", rifleNumber );

			SI32 daegumNumber = m_clBlackWarUnit[2].m_siNum + 100;
			if( 2000 <= daegumNumber )
				daegumNumber = 2000;
			_ftprintf( fp, "KIND_NEWDAEGUM\t%d\t30\t1\t5\n", daegumNumber );

			fclose( fp );
		}
		else if(cmd == BLACKWAR_CMD_LOSE)
		{
			pclClient->pclLog->FileConfigPrint( "BlackWarResult", "Win LeftUnit:%d", m_siLeftUnitNumber );

			FILE* fp = fopen("War\\BlackWarInfo.txt", "wt");
			if(fp == NULL)
				return;

			_ftprintf( fp, "BlackWAr\n" );
			_ftprintf( fp, "2007-04-18\n" );
			_ftprintf( fp, "v0.1\n" );
			_ftprintf( fp, "kkm\n" );
			_ftprintf( fp, "Kind\tNum\tRate\tBasicLevel\tRandomLevel\n" );

			SI32 catarNumber = m_clBlackWarUnit[0].m_siNum - 500;
			if( 0 >= catarNumber )
				catarNumber = m_clBlackWarUnit[0].m_siNum;
			_ftprintf( fp, "KIND_NEWCATAR\t%d\t50\t1\t5\n", catarNumber );

			SI32 rifleNumber = m_clBlackWarUnit[1].m_siNum - 500;
			if( 0 >= rifleNumber )
				rifleNumber = m_clBlackWarUnit[1].m_siNum;
			_ftprintf( fp, "KIND_NEWRIFLE\t%d\t50\t1\t5\n", rifleNumber );

			SI32 daegumNumber = m_clBlackWarUnit[2].m_siNum - 100;
			if( 0 >= daegumNumber )
				daegumNumber = m_clBlackWarUnit[2].m_siNum;
			_ftprintf( fp, "KIND_NEWDAEGUM\t%d\t30\t1\t5\n", daegumNumber );

			fclose( fp );
		}

		if( true == m_bDieBossSwitch )
			SendWarMsg( cmd, mapindex, x, y, false );
		else
			SendWarMsg( cmd, mapindex, x, y, m_bMakeBossSwitch );
	}
}

void CBlackWarManager::BlackWarReward( bool Flag )
{
	cltServer* pclserver = (cltServer*)pclClient;

	// [추가 : 황진성 2008. 1. 22 => 흑의 군단 개편 보상아이템 수정.]
	if( pclClient->IsCountrySwitch( Switch_BlackWarReform ) )
	{
		SI32 siNpcRate = pclserver->pclItemManager->clItemStatistic.siNPCPriceRate;

		SI32	id			 = 0;
		SI32	Index		 = 0;
		GMONEY	Money		 = 0;
		GMONEY	RewardMoney	 = 0;

		SI32 siWarSortRecordChar[MAX_WARRANK_PERSON_PER_MAP];
		ZeroMemory(siWarSortRecordChar, sizeof(SI32) * MAX_WARRANK_PERSON_PER_MAP);
		pclClient->pclCM->SortBlackWarRecord( siWarSortRecordChar, MAX_WARRANK_PERSON_PER_MAP );

		while(id = pclserver->pclCM->GetCharOrder(Index))
		{
			Index++;
			
			if(false == pclserver->pclCM->IsValidID(id))
			{
				continue;
			}

			if( 0 == pclserver->pclCM->CR[id]->siBlackWarEnemyKillScore )
				continue;

			SI32 UserRank = 0;
			for(SI32 Count = 0; Count < MAX_WARRANK_PERSON_PER_MAP; ++Count)
			{
				if(siWarSortRecordChar[Count] == id)
				{
					UserRank = Count + 1;
				}

			}
			GMONEY RewardMoney = ( pclserver->pclCM->CR[id]->siBlackWarEnemyKillScore * 250 * siNpcRate ) / 100	;
			if ( RewardMoney >= 1000000 )	{		RewardMoney = 1000000 ;			}

			// 자기 돈에 더한다.
			Money = RewardMoney + pclserver->pclCM->CR[id]->pclCI->clIP.GetMoney();
			
			// 보유 자금을 업데이트 한다. 
			pclserver->pclCM->CR[id]->pclCI->clIP.SetMoney(Money);
			sDBRequest_ChangeMoney clMsg(id, pclserver->pclCM->CR[id]->pclCI->GetPersonID(), CHANGE_MONEY_REASON_WARWIN, RewardMoney );
			pclClient->SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg);

			pclserver->pclAddMoneyStatistics->Add(ADDMONEY_STATISTIC_EVENT, RewardMoney);

			//pclserver->pclCM->CR[id]->SetUpdateSwitch(UPDATE_IP, true, 0);
	  
			if(pclserver->pclCM->CR[id]->GetSession())
			{
				cltGameMsgResponse_Black_War_Reform_Char_Result clBlackWarCharResult( pclserver->pclCM->CR[id]->siBlackWarEnemyKillScore, RewardMoney, Flag, UserRank);
				cltMsg clMsg( GAMEMSG_RESPONSE_BLACK_WAR_REFORM_CHAR_RESULT, sizeof(clBlackWarCharResult), (BYTE*)&clBlackWarCharResult );
				pclserver->pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);

				if( true == Flag )
				{
					if( id == m_siRewardCharID[0] )
					{
						// MVP마크 설정해준다.
						m_dwBlackWarEndClock = pclClient->CurrentClock;
						m_siMVPPersonID = pclClient->pclCM->CR[id]->pclCI->GetPersonID();

						pclClient->pclCM->CR[id]->pclCI->clBI.uiGMMode = GMMODE_ATB_WARMVP;
						pclClient->pclCM->CR[id]->SetUpdateSwitch(UPDATE_BASICINFO, true, 0 );

						cltCharServer* pclchar = (cltCharServer*)pclClient->pclCM->CR[id];

						// 클라이언트로 MVP 이펙트 뿌려준다.( 흑의군단 이미지를 그대로 쓴다 )
						cltGameMsgResponse_BlackArmy_MVPEffect clinfo(pclchar->GetCharUnique());
						cltMsg clMsg(GAMEMSG_RESPONSE_BLACKARMY_MVPEFFECT, sizeof(clinfo), (BYTE*)&clinfo);
						pclchar->SendNeighbourMsg(&clMsg, true);
					}
				}
			}
		}
		
		SI32	VillageScore	  = 0;
		SI32	VillageUnique     = 0;
		GMONEY	RewardRandMoney[] = { 10000000,  5000000, 3000000, 2000000, 1000000 };
		SI32	RewardRank		  = sizeof( RewardRandMoney ) / sizeof( RewardRandMoney[0] );
		
		for( int Rank = 0; Rank < RewardRank; ++Rank )
		{
			VillageUnique = pclserver->pclVillageManager->cBlackWarVillageScore[Rank].GetVillageUnique();
			VillageScore  = pclserver->pclVillageManager->cBlackWarVillageScore[Rank].GetVillageScore();
			Money		  = ( RewardRandMoney[Rank] * siNpcRate ) / 100;
			Money		  = min(RewardRandMoney[Rank]*2 , Money);

			if(VillageScore <= 0)
			{
				break;
			}

			sDBRequest_ChangeCityhallProfitMoney sendMsg( CHANGESTRMONEY_CITYHALL_PROFIT, 0, VillageUnique, Money);
			pclserver->pclGameDBProcess->SendMsg((sPacketHeader *)&sendMsg);

			cltStrInfo *pclStrInfo = pclserver->pclCityHallManager->GetStrInfo( VillageUnique );
			if( pclStrInfo != NULL )
			{
				cltMoney clMoney;
				clMoney.Set( Money );

				cltMoney clCityHallMoney;
				clCityHallMoney.Set( pclStrInfo->clCommonInfo.clMoney.itMoney + Money);

				pclStrInfo->clCityhallStrInfo.clInOutMoneyInfo.AddInOutMoneyInfo( RANKTYPE_CITYHALL,
																					INOUTCODE_BLACK_WAR, &pclserver->pclTime->clDate, &clMoney, &clCityHallMoney);

				sDBRequest_CityHallSetInOutMoney sendMsg( VillageUnique , RANKTYPE_CITYHALL, 
															INOUTCODE_BLACK_WAR, &pclserver->pclTime->clDate, &clMoney, &clCityHallMoney );

				pclserver->pclGameDBProcess->SendMsg( (sPacketHeader *)&sendMsg );
			}

			// 접속한 해당 마을 유저에게 흑의 군단 종료로 마을 이익금이 지불됐다는 것을 알림.
			Index = 0;
			while(id = pclserver->pclCM->GetCharOrder(Index))
			{
				Index++;
				
				if(false == pclserver->pclCM->IsValidID(id) )		
					continue;
								
				if( VillageUnique == pclserver->pclCM->CR[id]->pclCI->clBI.siHomeVillage )
				{
					if(pclserver->pclCM->CR[id]->GetSession())
					{
						cltGameMsgResponse_Black_War_Reform_Village_Result clBlackWarVillageResult( VillageScore, Rank+1, Money, Flag);
						cltMsg clMsg( GAMEMSG_RESPONSE_BLACK_WAR_REFORM_VILLAGE_RESULT, sizeof(clBlackWarVillageResult), (BYTE*)&clBlackWarVillageResult);
						pclserver->pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
					}
				}
			}
		}
	}
	else
	{
		SI32 loopCount = sizeof(REWARD_ITEMUNIQUE_PERCENT)/sizeof(REWARD_ITEMUNIQUE_PERCENT[0]);
		SI32 index = 0;
		SI32 id;

		while(id = pclClient->pclCM->GetCharOrder(index))
		{
			index++;

			cltCharServer* pclchar = (cltCharServer*)pclClient->pclCM->CR[id];

			if( NULL == pclchar )
				continue;
			// 사용자인 컴에게만 전달한다. 
			if( false == pclchar->IsValidConnection() )
				continue;

			SI32 randValue = rand() % 100;
			SI32 checkValue = 0;
			SI32 findIndex = -1;

			for( SI32 i = 0; i < loopCount; ++i)
			{
				checkValue += REWARD_ITEMUNIQUE_PERCENT[i];
				if( randValue <= checkValue )
				{
					findIndex = i;
					break;
				}
			}

			if( findIndex >= 0 &&  findIndex < loopCount )
			{
				SI32 itemNum = pclchar->siBlackWarEnemyKillNumber;

				if( true == Flag )
				{
					if( id == m_siRewardCharID[0] )
					{
						// MVP마크 설정해준다.
						m_dwBlackWarEndClock = pclClient->CurrentClock;
						m_siMVPPersonID = pclClient->pclCM->CR[id]->pclCI->GetPersonID();

						pclClient->pclCM->CR[id]->pclCI->clBI.uiGMMode = GMMODE_ATB_WARMVP;
						pclClient->pclCM->CR[id]->SetUpdateSwitch(UPDATE_BASICINFO, true, 0 );

						cltCharServer* pclchar = (cltCharServer*)pclClient->pclCM->CR[id];

						// 클라이언트로 MVP 이펙트 뿌려준다.( 흑의군단 이미지를 그대로 쓴다 )
						cltGameMsgResponse_BlackArmy_MVPEffect clinfo(pclchar->GetCharUnique());
						cltMsg clMsg(GAMEMSG_RESPONSE_BLACKARMY_MVPEFFECT, sizeof(clinfo), (BYTE*)&clinfo);
						pclchar->SendNeighbourMsg(&clMsg, true);

						itemNum = itemNum * 2;

					//	pclClient->pclLog->FilePrint( "Config\\BlackWarLog.txt", "NAME[%s] PersonID[%d] ItemNum[%d]", (TCHAR*)pclClient->pclCM->CR[id]->GetName(),
					//														  m_siMVPPersonID, itemNum );
					}
					else if( id == m_siRewardCharID[1] )
					{
						itemNum = itemNum + 40;
					}
					else if( id == m_siRewardCharID[2] )
					{
						itemNum = itemNum + 30;
					}
					else if( id == m_siRewardCharID[3] )
					{
						itemNum = itemNum + 20;
					}
					else if( id == m_siRewardCharID[4] )
					{
						itemNum = itemNum + 10;
					}
				}
				
				// 피로방지 시스템 적용
				itemNum = pclchar->CalcTiredPoint( itemNum );

				if( id == m_siRewardCharID[0] )
				{
					pclClient->pclLog->FilePrint( "Config\\BlackWarLog.log", "Name[%s] ItemNum[%d]", (TCHAR*)pclClient->pclCM->CR[id]->GetName(), itemNum );
				}

				if ( itemNum > 0 )
				{
					if ( pclchar->GiveItem( REWARD_ITEMUNIQUE[findIndex], itemNum, INPUTITEMPERSON_OPTION_BLACKWAR,0) == false )
					{
						pclserver->SendLog_InventoryFull(pclchar->pclCI->GetPersonID(),REWARD_ITEMUNIQUE[findIndex], pclchar->siBlackWarEnemyKillNumber, INPUTITEMPERSON_OPTION_BLACKWAR);
					}
				}

				cltGameMsgResponse_WinWarPrize clinfo(-1, 0, 0, 0, 0, REWARD_ITEMUNIQUE[findIndex], itemNum, true);
				cltMsg clMsg(GAMEMSG_RESPONSE_WINWARPRIZE, sizeof(clinfo), (BYTE*)&clinfo);
				pclchar->SendNetMsg((sPacketHeader*)&clMsg);
			}
		}
	}
	
	if ( pclserver->IsCountrySwitch(Switch_Server_Event) )
	{
		CNServerEvent_Quest* pclSE_Quest =  (CNServerEvent_Quest*)pclserver->pclServerEventManager->GetServerEvent_Quest();
		if ( pclSE_Quest )
		{
			pclSE_Quest->IncServerEventQuestCondition( SERVEREVENT_TYPE_BLACKWARCLEAR, 0, 1 );
		}
	}
}

void CBlackWarManager::SetDraw(SI32 cmd)
{
	m_bDrawSwitch = true;

	switch(cmd)
	{
	case BLACKWAR_CMD_BREAK:
		{
			m_siDrawCmdFont	= 0;
			pclClient->PushEffect(pclClient->GetUniqueFromHash(TEXT("EFFECT_WARBREAK")), 0, 0);
			((cltClient*)pclClient)->PlayBackgroundMusic(0);
		}
		break;
	case BLACKWAR_CMD_WIN:
		{
			m_siDrawCmdFont	= 1;	
			pclClient->PushEffect(pclClient->GetUniqueFromHash(TEXT("EFFECT_WARWIN")), 0, 0);		
		}
		break;
	case BLACKWAR_CMD_LOSE:	
		{
			m_siDrawCmdFont	= 2;	
			pclClient->PushEffect(pclClient->GetUniqueFromHash(TEXT("EFFECT_WARLOSE")), 0, 0);	
		}
		break;
	}

	if ( cmd == WAR_CMD_WIN || cmd == WAR_CMD_LOSE )
	{
		((cltClient*)pclClient)->PlayBackgroundMusic(pclClient->pclCM->CR[1]->GetMapIndex());
	}

	m_dwDrawStartClock = pclClient->CurrentClock;
}

// 전황을 표시한다. 
void CBlackWarManager::Draw(LPDIRECTDRAWSURFACE7 lpsurface, SI32 rx, SI32 ry)
{
	if( m_bDrawSwitch == false)
		return ;

	if( TABS(pclClient->CurrentClock - m_dwDrawStartClock) > 10000)
	{
		m_bDrawSwitch = false;
	}

	TSpr* pspr = pclClient->GetGlobalSpr(GIMG_MAGIC_WAR);
	if(pspr == NULL)return ;

	cltClient* pclclient = (cltClient*)pclClient;

	SI32 yindent = 0 ;

	if ( true == g_SoundOnOffDlg.m_bFullScreenMode )
	{
		yindent = 30 ;
	}

	if(GP.LockSurface(lpsurface)==TRUE)
	{
		GP.PutSpr( pspr, ( pclclient->siClientScreenXsize - pspr->GetXSize() )/2 + rx, 
			50  + ry + yindent , m_siDrawCmdFont);

		GP.UnlockSurface(lpsurface);
	}
}

