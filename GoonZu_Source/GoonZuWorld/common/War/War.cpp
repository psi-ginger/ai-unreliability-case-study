#include "War.h"

#include "DockDo/DockDo.h"
#include "Osaka/Osaka.h"

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
#include "../Common/Event/Gogu/Gogu.h"
#include "Char/CharManager/CharManager.h"


#include "MsgType-System.h"

#include "../../Client/Music/Music.h"
#include "../../Client/Interface/SoundOnOffDlg/SoundOnOffDlg.h"

#include "../../../DBManager/GameDBManager/DBMsg-System.h"
#include "../../../DBManager/GameDBManager/DBMsg-Person.h"

#include <MsgType-Person.h>

extern cltCommonLogic* pclClient;


cltWarManager::cltWarManager()
{
	bWarSwitch	= false;
	bWarOsakaSwitch = false;

	siTotalUnitNum= 0;		// 전체 병사수. 

	siCurUnitNum	= 0;		// 현재 병사수. 
	siPorterNum		= 0;		// 현재 나와 있는 짐꾼 왜구 수. 
	siMadeNum		= 0;
	siCasulty		= 0;
	uiLeftTime		= 0;
	dwWarStartClock	= 0;
	dwWarOsakaClock = 0;
	dwNewWarUnitClock = 0;
	siLeftMon		= 0;

	siPorterVillageUnique	= 0;

	//------------
	// 출력 
	//------------
	bDrawSwitch		= false;

}


cltWarManager::~cltWarManager()
{
}


// 전쟁 몬스터를 추가한다. 
void cltWarManager::AddWarMonNum(SI32 kind, SI32 monnum)
{
	SI32 i;

	for(i = 0;i < MAX_WAR_UNIT;i++)
	{
		if(clWarUnit[i].siKind == kind)
		{
			clWarUnit[i].siNum  += monnum;
		}
	}
}


// 전쟁을 기획한다. 
bool cltWarManager::MakeWar(DWORD clock)
{
	// 전쟁에 참여할 병사수와 병종을 설정한다 
	SI32 i;
	FILE* fp;
	char buffer[1024];

	fp = fopen("War\\WarInfo.txt", "rt");
	if(fp == NULL)return false;

	SI32 num;
	SI32 rate;
	SI32 basic;
	SI32 rand;
	SI32 mode;
	char charname[128];

	for(i = 0;i<5;i++)
	{
		fgets(buffer, 1024, fp);
	}

	cltServer* pclserver = (cltServer*)pclClient;
	CMinisterYezo* pclminister = (CMinisterYezo*)pclserver->pclMinisterMgr->GetMinister(MINISTER_YEZO);

	// 짐꾼 왜구용 마을을 선택한다. 
	siPorterVillageUnique = pclminister->m_siWarVillageUnique;
	if(siPorterVillageUnique < 0 || siPorterVillageUnique >= MAX_VILLAGE_NUMBER)siPorterVillageUnique = 0;
	if(pclClient->pclVillageManager->pclVillageInfo[siPorterVillageUnique] == NULL)siPorterVillageUnique = 0;

	SI32 healernum = (pclminister->m_siJapanCommanderNum + pclminister->m_siJapanGunSolNum + pclminister->m_siJapanSwordSolNum) / 10;
	SI32 ninjanum = (pclminister->m_siJapanCommanderNum + pclminister->m_siJapanGunSolNum + pclminister->m_siJapanSwordSolNum) / 20;

	siWatUnitNum	= 0;
	siTotalUnitNum	= 0;
	while( fgets(buffer, 1024, fp) != NULL)
	{
		if(siWatUnitNum >= MAX_WAR_UNIT)
		{
			MsgBox("fdj82jfd", "dsif83jf");
			continue;
		}

		sscanf(buffer, "%s %d %d %d %d %d", charname, &num, &rate, &basic, &rand, &mode);

		SI32 kind = pclClient->pclKindInfoSet->FindKindFromCode(charname);
		if(kind)
		{
			clWarUnit[siWatUnitNum].siKind		= kind;
			// mode 0은 일반적 왜구 1은 4방향에서 생성될 왜구
			switch(kind)
			{
			case KIND_JAPANCOMMANDER:
				{
					clWarUnit[siWatUnitNum].siNum = pclminister->m_siJapanCommanderNum ;
/*					clWarUnit[siWatUnitNum].siNum = pclminister->m_siJapanCommanderNum * 0.9;	
					clWarUnit[siWatUnitNum].siOtherNum = pclminister->m_siJapanCommanderNum * 0.1;

					SI32 temp = clWarUnit[siWatUnitNum].siOtherNum % 4;

					if(temp == 1)
						clWarUnit[siWatUnitNum].siOtherNum = clWarUnit[siWatUnitNum].siOtherNum + 3;
					else if(temp == 2)
						clWarUnit[siWatUnitNum].siOtherNum = clWarUnit[siWatUnitNum].siOtherNum + 2;
					else if(temp == 3)
						clWarUnit[siWatUnitNum].siOtherNum = clWarUnit[siWatUnitNum].siOtherNum + 1;*/

				}
				break;
			case KIND_JAPANGUNSOL:	
				{
					clWarUnit[siWatUnitNum].siNum = pclminister->m_siJapanGunSolNum ;
/*					clWarUnit[siWatUnitNum].siNum = pclminister->m_siJapanGunSolNum * 0.9;	
					clWarUnit[siWatUnitNum].siOtherNum = pclminister->m_siJapanGunSolNum * 0.1;

					SI32 temp = clWarUnit[siWatUnitNum].siOtherNum % 4;

					if(temp == 1)
						clWarUnit[siWatUnitNum].siOtherNum = clWarUnit[siWatUnitNum].siOtherNum + 3;
					else if(temp == 2)
						clWarUnit[siWatUnitNum].siOtherNum = clWarUnit[siWatUnitNum].siOtherNum + 2;
					else if(temp == 3)
						clWarUnit[siWatUnitNum].siOtherNum = clWarUnit[siWatUnitNum].siOtherNum + 1;*/
				}
				break;
			case KIND_JAPANSWORDSOL:
				{
					clWarUnit[siWatUnitNum].siNum = pclminister->m_siJapanSwordSolNum ;
/*					clWarUnit[siWatUnitNum].siNum = pclminister->m_siJapanSwordSolNum * 0.9;	
					clWarUnit[siWatUnitNum].siOtherNum = pclminister->m_siJapanSwordSolNum * 0.1;

					SI32 temp = clWarUnit[siWatUnitNum].siOtherNum % 4;

					if(temp == 1)
						clWarUnit[siWatUnitNum].siOtherNum = clWarUnit[siWatUnitNum].siOtherNum + 3;
					else if(temp == 2)
						clWarUnit[siWatUnitNum].siOtherNum = clWarUnit[siWatUnitNum].siOtherNum + 2;
					else if(temp == 3)
						clWarUnit[siWatUnitNum].siOtherNum = clWarUnit[siWatUnitNum].siOtherNum + 1;*/
				}
				break;
			case KIND_JAPANHEALER:	
				{
					clWarUnit[siWatUnitNum].siNum = healernum ;
/*					clWarUnit[siWatUnitNum].siNum = healernum * 0.9;	
					clWarUnit[siWatUnitNum].siOtherNum = healernum * 0.1;

					SI32 temp = clWarUnit[siWatUnitNum].siOtherNum % 4;

					if(temp == 1)
						clWarUnit[siWatUnitNum].siOtherNum = clWarUnit[siWatUnitNum].siOtherNum + 3;
					else if(temp == 2)
						clWarUnit[siWatUnitNum].siOtherNum = clWarUnit[siWatUnitNum].siOtherNum + 2;
					else if(temp == 3)
						clWarUnit[siWatUnitNum].siOtherNum = clWarUnit[siWatUnitNum].siOtherNum + 1;*/
				}
				break;
			case KIND_JAPANPORTERSOL:
				{
					clWarUnit[siWatUnitNum].siNum = pclminister->m_siJapanPorterNum;
				}
				break;
			case KIND_NINJA2:
				{
					clWarUnit[siWatUnitNum].siNum = ninjanum ;
/*					clWarUnit[siWatUnitNum].siNum = ninjanum * 0.9;	
					clWarUnit[siWatUnitNum].siOtherNum = ninjanum * 0.1;

					SI32 temp = clWarUnit[siWatUnitNum].siOtherNum % 4;

					if(temp == 1)
						clWarUnit[siWatUnitNum].siOtherNum = clWarUnit[siWatUnitNum].siOtherNum + 3;
					else if(temp == 2)
						clWarUnit[siWatUnitNum].siOtherNum = clWarUnit[siWatUnitNum].siOtherNum + 2;
					else if(temp == 3)
						clWarUnit[siWatUnitNum].siOtherNum = clWarUnit[siWatUnitNum].siOtherNum + 1;*/
				}
				break;
			case KIND_WOLFNINJA2:	
				{
					clWarUnit[siWatUnitNum].siNum = ninjanum ;
/*					clWarUnit[siWatUnitNum].siNum = ninjanum * 0.9;	
					clWarUnit[siWatUnitNum].siOtherNum = ninjanum * 0.1;

					SI32 temp = clWarUnit[siWatUnitNum].siOtherNum % 4;

					if(temp == 1)
						clWarUnit[siWatUnitNum].siOtherNum = clWarUnit[siWatUnitNum].siOtherNum + 3;
					else if(temp == 2)
						clWarUnit[siWatUnitNum].siOtherNum = clWarUnit[siWatUnitNum].siOtherNum + 2;
					else if(temp == 3)
						clWarUnit[siWatUnitNum].siOtherNum = clWarUnit[siWatUnitNum].siOtherNum + 1;*/
				}
				break;
			default:
				clWarUnit[siWatUnitNum].siNum		= num;
			}
			// 짐꾼왜구는 적절한 마을이 있어야만 나온다. 
			if(kind == KIND_JAPANPORTERSOL && siPorterVillageUnique == 0)
			{
				clWarUnit[siWatUnitNum].siNum = 0;
			}

			clWarUnit[siWatUnitNum].siRate		= rate;
			clWarUnit[siWatUnitNum].siBasicLevel= basic;
			clWarUnit[siWatUnitNum].siRandLevel	= rand;


			clWarUnit[siWatUnitNum].siCurNum		= 0;
			//clWarUnit[siWatUnitNum].siOtherCurNum	= 0;

			siTotalUnitNum += clWarUnit[siWatUnitNum].siNum;
			//siTotalUnitNum += clWarUnit[siWatUnitNum].siOtherNum;
			siWatUnitNum++;

		}
	}

	fclose(fp);


	siLeftMon			= siTotalUnitNum;
	siMadeNum			= 0;
	siCasulty			= 0;
	uiLeftTime			= 1800000 ;// 30분. 
	dwWarStartClock		= clock;
	dwNewWarUnitClock	= clock;	// 4방향에 나올 왜구시간체크

	bWarSwitch = true;
	bWarOsakaSwitch = false;
	//--------------------------------------------------
	// 모든 사용자의 WarKillerNum변수를 초기화한다. 
	//--------------------------------------------------
	SI32 index = 0;
	SI32 id;
	while(id = pclClient->pclCM->GetCharOrder(index))
	{
		index++;

		cltCharServer* pclchar = (cltCharServer*)pclClient->pclCM->CR[id];

		pclchar->siWarEnemyKillNumber	= 0;
		pclchar->siWarKilledNumber		= 0;
	}

	//-------------------------------------------------
	// 마을 주민 전공을 초기화한다. 
	//-------------------------------------------------
	pclClient->pclVillageManager->InitWarKillNumber();

	//--------------------------------------------------
	// 모든 클라이언트들에게 전쟁이 시작되었음을 알린다. 
	//--------------------------------------------------
	SendWarMsg(WAR_CMD_BREAK, -1, 0, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, siPorterVillageUnique);

	//--------------------------------------------------
	// 보스를 만들어낸다. 
	//--------------------------------------------------
	// 기존에 보스가 존재하면 처치한다.
	index = 0;
	while(id = pclClient->pclCM->GetCharOrder(index))
	{
		index++;

		if(pclClient->pclCM->IsValidID(id) == false)continue;
		if(pclClient->pclCM->CR[id]->GetKind() != KIND_TOYO1 && pclClient->pclCM->CR[id]->GetKind() != KIND_TOYO2)continue;

		// 셀 내에서의 정보를 삭제한다. 
		pclClient->pclCM->CR[id]->DeleteInCell();
		if(pclClient->pclCM->DeleteChar( id ) == TRUE)
		{
			index--;
		}
	}				


	// 3마리 만든다. 
	for(i = 0; i < 3;i++)
	{
		cltInnerStatusInfo clinnerstatus;
		clinnerstatus.clEnemyType.Set(ENEMYTYPE_ALLPC, 0);

		SI32 id = pclserver->SetNPCChar(KIND_TOYO1, VILLAGE_NONE, NULL, 1, 0, 777, 682, 0, NPCITEMMODE_NORMAL, &clinnerstatus );
		if(id)
		{
			cltCharServer* pclchar = (cltCharServer*)pclserver->pclCM->CR[id];
			pclchar->siCharMode = CHARMODE_WARASSIST;

		}
	}

	// 독도 이벤트 시작. 주말에만 열린다.
	//if ( pclserver->sTime.wDayOfWeek == 0 || pclserver->sTime.wDayOfWeek == 6 )
	{
		//pclserver->SendNoticeMsg(-1, 0, 0, "주말 전쟁때는 왜구의 독도 침공 이벤트가 시작됩니다." );
		pclserver->pclWarDockDoManager->Start();
	}

	pclserver->siEventItemDropRate /= 10 ;

	// 인스턴스 던전 이익금을 준다. 5시랑 9시 전쟁에서만
	if ( pclserver->pclVillageManager->siInstanceProfitVillageunique )
	{
		if ( pclserver->pclVillageManager->IsValidVillage(pclserver->pclVillageManager->siInstanceProfitVillageunique) )
		{
			sDBRequest_Change_InstanceProfit sendMsg( pclserver->pclVillageManager->siTempInstanceProfit,true);
			pclserver->SendDBMsg(DBSELECT_BASE,  (sPacketHeader *)&sendMsg );

			// 저장할 돈을 초기화 한다. 
			pclserver->pclVillageManager->siTempInstanceProfit = 0;
		}
	}

	// 전쟁을 발발 시킨다. 
	return true;
}

void cltWarManager::SendWarMsg(SI32 cmd, SI32 mapindex1, SI32 x1, SI32 y1, SI32 mapindex2, SI32 x2, SI32 y2,SI16 vill1, SI16 kill1, SI16 vill2, SI16 kill2, SI16 vill3, SI16 kill3, SI16 portervillageunique)
{

	cltGameMsgResponse_War clwar(cmd, uiLeftTime, siLeftMon, siTotalUnitNum, siCasulty, mapindex1, x1, y1, mapindex2, x2, y2, vill1, kill1, vill2, kill2, vill3, kill3, portervillageunique);
	cltMsg clMsg(GAMEMSG_RESPONSE_WAR, sizeof(clwar), (char*)&clwar);

	SI32 index = 0;
	SI32 id;
	while(id = pclClient->pclCM->GetCharOrder(index))
	{
		index++;

		// 사용자인 컴에게만 전달한다. 
		if(pclClient->pclCM->CR[id]->IsValidConnection() == true)
		{
			pclClient->pclCM->CR[id]->SendNetMsg((char*)&clMsg);
		}
	}

}


void cltWarManager::WarEnd()
{
	bWarSwitch = false;

	SI32 i;
	SI32 statistic[10];

	for(i = 0;i < 10;i++)
	{
		statistic[i] = 0;
	}

	//----------------------------------
	// 전쟁 병사들의 전쟁 목표를 없앤다.
	//----------------------------------

	SI32 index = 0;
	SI32 id;
	while(id = pclClient->pclCM->GetCharOrder(index))
	{
		index++;

		cltCharServer* pclchar = (cltCharServer*)pclClient->pclCM->CR[id];

		// NPC이면 전쟁 목표를 없앤다. 
		if(IsNPC(pclchar->GetCharUnique()))
		{
			if(pclchar->siCharMode == CHARMODE_WAR)
			{
				pclchar->siCharMode = 0;

				OrderStop(id, BY_PLAYER);

			}
		}
		// 사용자이면 통계를 낸다. 
		else
		{
			SI32 index = min(9, (pclchar->siWarEnemyKillNumber / 5));

			statistic[index] ++; 
		}
	}

	pclClient->siEventItemDropRate *= 10 ;
}

void cltWarManager::Action()
{
	SI32 i;
	SI32 level;
	static SI32 portervary = 0;
	static SI32 PositionIndex = 0;
	cltServer* pclserver = (cltServer*)pclClient;

	if( pclClient->siServiceArea == ConstServiceArea_Korea )
	{
		if( bWarOsakaSwitch == true)
		{
			if(TABS(pclClient->CurrentClock - dwWarOsakaClock) > 20000)
			{
				pclserver->pclWarOsakaManager->Start();
				bWarOsakaSwitch = false;
			}
		}

		if( pclserver->pclWarOsakaManager->bWarOsaka == true )
		{
			pclserver->pclWarOsakaManager->Action();
		}
	}
	// 왜구 전쟁

	if(bWarSwitch == false)return ;

	//--------------------------------------------------
	// 시간 남은 것과 병사 남은 것을 비교하여 조치한다. 
	//--------------------------------------------------
	siLeftMon = max(0, siTotalUnitNum - siMadeNum) + siCurUnitNum;

	/*	if(uiLeftTime < 1700000 && uiLeftTime > 700000 )
	{
	if(siLeftMon < (SI32)uiLeftTime / 9000)
	{
	SI32 kind = 0;
	switch(rand() % 3)
	{
	case 0:	kind = KIND_JAPANSWORDSOL;	break;
	case 1:	kind = KIND_JAPANGUNSOL;	break;
	default:kind = KIND_JAPANHEALER;	break;
	}

	//			AddWarMonNum(kind, max(1, uiLeftTime / 60000));

	}
	}
	*/

	//------------------------------------
	// 군사들을 배치하여 한양으로 보낸다.
	//------------------------------------
	//if(pclClient->GetFrame() % 2 == 0)
	{
		portervary ++;

		//몬스터가 일정 수 이상이면 더이상 배치해서는 안된다. 속도 저하 방지. 
		if(pclClient->pclCM->pclIDManager->GetNPCCharNumber() < (MAX_NPC_NUMBER-3000))
		{
			//--------------------------------
			// 병사들을 배치한다. 
			//--------------------------------
			if(siMadeNum <= siTotalUnitNum)
			{
				for(i = 0; i < siWatUnitNum;i++)
				{
					if(clWarUnit[ i ].siCurNum >= clWarUnit[ i ].siNum)continue;
					if(clWarUnit[ i ].siRate < rand() % 100)continue;

					SI32 kind = clWarUnit[ i ].siKind;


					SI32 pos = 0 ;
					SI32 x, y;
					SI32 tempx, tempy;
					SI32 mapindex = 0;

					// 짐꾼왜구는 마을에서 나오게 한다. 
					if(kind == KIND_JAPANPORTERSOL)
					{
						// 짐꾼 왜구는 일반 몬스터의 절반 확률로 등장한다. 
						if(portervary % 3)
						{
							continue;
						}

						// 짐꾼왜구는 특정 마을에 몰려 나오기 때문에 일정수 이상이 출현하면 더이상 나오지 않아야 한다. 
						if(siPorterNum > 100)
						{
							continue;
						}

						mapindex			= siPorterVillageUnique;
						SI32 maptypeunique	= pclClient->pclMapManager->pclMapArray[mapindex]->siMapUnique;

						tempx =	rand() %  pclClient->pclMapManager->pclMapArray[mapindex]->MH.MapXsize;
						tempy = rand() %  pclClient->pclMapManager->pclMapArray[mapindex]->MH.MapYsize;

					}
					// 일반 왜구는 메인 필드에서 나오게 한다. 
					else
					{
						/*	POINT	NewWarUnitPosition[4] = { 522, 544, 
						325, 641,
						139, 234,
						424, 209		
						};*/

						mapindex = 0;
						SI32 villageunique = rand() % ( CUR_VILLAGE_LIMIT + 1 ) ; 
						if(pclClient->pclVillageManager->pclVillageInfo[villageunique] == NULL)continue;

						tempx = pclClient->pclVillageManager->pclVillageInfo[villageunique]->PosX + (5 - rand() % 10);
						tempy = pclClient->pclVillageManager->pclVillageInfo[villageunique]->PosY + (5 - rand() % 10);
					}

					// 몬스터를 배치할 적절한 곳을 찾는다. 
					if(pclClient->pclMapManager->pclMapArray[mapindex]->FindEmptyAreaForCharLocate(kind, tempx, tempy, 10, &x, &y, true) == false)
					{
						continue;
					}

					level = clWarUnit[ i ].siBasicLevel + rand() % clWarUnit[ i ].siRandLevel;

					cltInnerStatusInfo clinnerstatus;
					clinnerstatus.clEnemyType.Set(ENEMYTYPE_ALLPC, 0);

					SI32 id = pclserver->SetNPCChar(kind, VILLAGE_NONE, NULL, level, 0, x, y, mapindex, NPCITEMMODE_NORMAL, &clinnerstatus );
					if(id)
					{
						siMadeNum ++;
						clWarUnit[ i ].siCurNum++;

						cltCharServer* pclchar = (cltCharServer*)pclClient->pclCM->CR[id];

						SI32 villageunique = pclClient->pclVillageManager->FindRandVillage();	// 공격할 마을  
						if(villageunique)
						{
							pclchar->siCharMode			= CHARMODE_WAR;
							pclchar->siCharModePara1	= villageunique;
						}
						else
						{
							pclchar->siCharMode = 0;
						}
						//pclchar->dwTimerClock = 1000;
					}
				}
/*				static SI32 k=0; // 생성될 위치 인덱스
				// 3분마다 생성

				if(TABS(pclClient->CurrentClock - dwNewWarUnitClock) > 180000)
				{
					for(i = 0; i < siWatUnitNum;i++)
					{
						if(clWarUnit[ i ].siOtherCurNum >= clWarUnit[ i ].siOtherNum)
							continue;
						k = 0;

						for(SI32 j = 0; j < clWarUnit[ i ].siOtherNum/4 ; ++j)
						{
							SI32 kind = clWarUnit[ i ].siKind;

							SI32 pos = 0 ;
							SI32 x, y;
							SI32 tempx, tempy;
							SI32 mapindex = 0;

							// 일반 왜구는 메인 필드에서 나오게 한다. 
							POINT	NewWarUnitPosition[4] = { 522, 544, 
								325, 641,
								139, 234,
								424, 209		
							};

							mapindex = 0;
							SI32 villageunique = rand() % ( CUR_VILLAGE_LIMIT + 1 ) ; 
							if(pclClient->pclVillageManager->pclVillageInfo[villageunique] == NULL)continue;

							tempx = NewWarUnitPosition[k].x + (5 - rand() % 10);
							tempy = NewWarUnitPosition[k].y + (5 - rand() % 10);
							k++;
							if(k >= 4)
								k=0;

							// 몬스터를 배치할 적절한 곳을 찾는다. 
							if(pclClient->pclMapManager->pclMapArray[mapindex]->FindEmptyAreaForCharLocate(kind, tempx, tempy, 10, &x, &y, true) == false)
							{
								continue;
							}

							level = clWarUnit[ i ].siBasicLevel + rand() % clWarUnit[ i ].siRandLevel;

							cltInnerStatusInfo clinnerstatus;
							clinnerstatus.clEnemyType.Set(ENEMYTYPE_ALLPC, 0);

							SI32 id = pclserver->SetNPCChar(kind, VILLAGE_NONE, NULL, level, 0, x, y, mapindex, NPCITEMMODE_NORMAL, &clinnerstatus );
							if(id)
							{
								siMadeNum ++;
								clWarUnit[ i ].siOtherCurNum++;

								cltCharServer* pclchar = (cltCharServer*)pclClient->pclCM->CR[id];

								SI32 villageunique = pclClient->pclVillageManager->FindRandVillage();	// 공격할 마을  
								if(villageunique)
								{
									pclchar->siCharMode			= CHARMODE_WAR;
									pclchar->siCharModePara1	= villageunique;
								}
								else
								{
									pclchar->siCharMode = 0;
								}
								//pclchar->dwTimerClock = 1000;
							}
						}
					}
					// 다시 시간을 초기화해서 4분후를 기다린다
					dwNewWarUnitClock = pclClient->CurrentClock;
				}*/
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
		DWORD lastclock = TABS(pclClient->CurrentClock - dwWarStartClock);
		dwWarStartClock	= pclClient->CurrentClock;
		if(uiLeftTime > lastclock)
		{
			uiLeftTime -= lastclock;
			cmd = WAR_CMD_GOING;
		}
		else
		{
			uiLeftTime = 0;

			// 시간이 종료되면 전쟁을 끝낸다. (패배) 
			WarEnd();
			cmd = WAR_CMD_LOSE;
		}

		//-------------------------
		// 남은 병사를 확인한다. 
		//-------------------------
		siCurUnitNum	= 0;
		siPorterNum		= 0;
		SI32 index = 0;
		SI32 id;
		SI32 mapindex1 = -1;
		SI32 x1 = 0;
		SI32 y1 = 0;
		SI32 mapindex2 = -1;
		SI32 x2 = 0;
		SI32 y2 = 0;


		//--------------------------------
		// 마을 전공을 소트한다. 
		//--------------------------------
		SI16 siVillageUnique[MAX_VILLAGE_NUMBER];
		SI16 siVillageWarKillNumber[MAX_VILLAGE_NUMBER];

		for(i = 0;i < MAX_VILLAGE_NUMBER;i++)
		{
			siVillageUnique[i]			= 0;
			siVillageWarKillNumber[i]	= 0;
		}

		pclClient->pclVillageManager->WarKillNumberSort(siVillageUnique,  siVillageWarKillNumber);


		//--------------------------------------
		// 적군 상황 파악 
		//--------------------------------------
		while(id = pclClient->pclCM->GetCharOrder(index))
		{
			index++;

			if(IsNPC(pclClient->pclCM->CR[id]->GetCharUnique()) == false )continue;
			if(pclClient->pclCM->IsAlive(id) == false)continue;

			cltCharServer* pclchar = (cltCharServer*)pclClient->pclCM->CR[id];

			if(pclClient->pclCM->IsAlive(id) && pclchar->siCharMode == CHARMODE_WAR)
			{
				siCurUnitNum++;

				// 적군의 위치 확인. (초보자용 몬스터인 짐꾼 정보 파악 ) 
				if(pclchar->GetKind() == KIND_JAPANPORTERSOL)
				{
					siPorterNum++;
					mapindex2	= pclchar->GetMapIndex();
					x2			= pclchar->GetX();
					y2			= pclchar->GetY();
				}
				else
				{
					mapindex1	= pclchar->GetMapIndex();
					x1			= pclchar->GetX();
					y1			= pclchar->GetY();
				}
			}
		}

		// 모든 병사가 배치되었는데 현재 남은 병사가 없다면 승리 
		if(siMadeNum >= siTotalUnitNum)
		{
			if(siCurUnitNum <= 0)
			{

				//---------------------------------------------
				// 모든 사용자들에게 전리품으로 돈을 지급한다. 
				//---------------------------------------------
				if(cmd != WAR_CMD_LOSE)
				{
					//----------------------------------
					// 전공이 높은 상위 20명을 소트한다. 
					//----------------------------------
					SI32 siWarSortRecordChar[20];
					ZeroMemory(siWarSortRecordChar, sizeof(SI32) * 20);
					pclClient->pclCM->SortWarRecord(siWarSortRecordChar, 20);


					// 서버의 물가 수준 확보. 
					SI32 npcpricerate = max(100, pclClient->pclItemManager->clItemStatistic.siNPCPriceRate);

					// 남은 시간(초)에 해당하는 자금 지원. 
					SI32 money = (uiLeftTime / 1000 ) * 2;
					SI32 lastmoney;

					SI32 index = 0;
					SI32 id;
					while(id = pclClient->pclCM->GetCharOrder(index))
					{
						index++;

						// 사용자인 컴에게만 전달한다. 
						if(pclClient->pclCM->CR[id]->IsValidConnection() == false)continue;

						//-------------------------------------
						// 전쟁의 전공에 의하여 돈을 더 준다. 
						//-------------------------------------
						cltCharServer* pclchar = (cltCharServer*)pclClient->pclCM->CR[id];
						lastmoney = money + min(30000, (pclchar->siWarEnemyKillNumber * 300)) + min(1000, (pclchar->siWarKilledNumber * 50));
						lastmoney = lastmoney  * npcpricerate / 100;	// 각 서버별 물가에 의해서 보정해 준다. 

						// 개인 전공에 의한 명성 경험치 지급. 
						SI32 personalfameexp	= min(5, pclchar->siWarEnemyKillNumber / 100);
						if(personalfameexp > 1)
						{
							pclClient->pclLog->FilePrint("Config\\WarFameExpGetList.txt", "%s-warrecord:%d fameexp:%d", pclchar->GetName(), pclchar->siWarEnemyKillNumber, personalfameexp);
						}


						// 상위 마을과 비교한다. 
						SI32 grade		= -1;		// -1이 기본값. 
						SI32 fameexp	= 0;

						for(i = 0; i < WAR_REWARD_VILLAGE_NUMBER;i++)
						{
							if(siVillageUnique[i] && siVillageUnique[i] == pclchar->pclCI->clBI.siHomeVillage)
							{
								grade = i;
								fameexp	= max(0, (3 - i)) * max(1, ( pclchar->pclCI->clIP.GetLevel() / 50));

								lastmoney = lastmoney  + lastmoney * (WAR_REWARD_VILLAGE_NUMBER - i) * 15 / 100;
							}
						}

						//-----------------------------------
						// 돈의 액수에 따라 경험치도 준다.
						//-----------------------------------
						SI32 exp = min(10000, lastmoney / 5);

						// 현충일, 광복절, 국군의 날, 3.1절에는 포상금과 경험치가 2배. 
						if( pclserver->bSpecailDay1Switch || pclserver->bSpecailDay2Switch ||
							pclserver->bSpecailDay3Switch || pclserver->bSpecailDay6Switch ||
							pclserver->bSpecailDay7switch )
						{
							lastmoney *= 2 ;
							exp *= 2 ;
						}

						if ( pclchar->pclCI->clIP.GetLevel() <= INIT_LEVEL )
							exp = 0 ;

						sDBRequest_ChangeMoney clMsg(id, pclClient->pclCM->CR[id]->pclCI->GetPersonID(), CHANGE_MONEY_REASON_WARWIN, lastmoney);
						pclClient->SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg);

						if(exp)
						{
							pclchar->IncreaseExp( exp );
						}
						SI32 totalfameexp = fameexp + personalfameexp;
						if(totalfameexp)
						{
							pclchar->IncreaseFameExp( totalfameexp);
						}

						// 클라이언트에게 돈과 경험치를 받았음을 통보한다. 
						{
							cltGameMsgResponse_WinWarPrize clinfo(grade, lastmoney, exp, fameexp, personalfameexp);
							cltMsg clMsg(GAMEMSG_RESPONSE_WINWARPRIZE, sizeof(clinfo), (char*)&clinfo);
							pclchar->SendNetMsg((char*)&clMsg);
						}



						// 통화량 집계한다. 
						cltServer* pclserver = (cltServer*)pclClient;
						cltMoney clmoney;
						clmoney.Set(lastmoney);
						pclserver->clCurrencyInfo.clInputMoney_War.Increase(&clmoney);

					}
					// 승리 마을은 다음 고구려 유적 방어 이벤트를 시작한다. 
					//	pclClient->pclEventGogu->End();	// 기존의 이벤트 마을은 삭제한다.
					//	pclClient->pclEventGogu->Start(siVillageUnique[0], false);

				}

				if(cmd != WAR_CMD_LOSE)
				{
					cmd = WAR_CMD_WIN;
					WarEnd();

					// 왜탐 승리후 오사카 전투를 시작한다 시작 시간 딜레이를 준다
					if( pclClient->siServiceArea == ConstServiceArea_Korea )
					{
						pclClient->pclVillageManager->WarOsakaWarpSwitch();
					
						bWarOsakaSwitch = true;
						dwWarOsakaClock = pclClient->CurrentClock;
					}
				}
			}
		}

		//---------------------------------
		// 전쟁 승패 여부를 기록한다. 
		//---------------------------------
		if(cmd == WAR_CMD_WIN)
		{
			pclClient->pclLog->FilePrint("Config\\WarResult.txt", "Win LeftTime:%d", uiLeftTime);
		}
		else if(cmd == WAR_CMD_LOSE)
		{
			pclClient->pclLog->FilePrint("Config\\WarResult.txt", "Lose LeftMon:%d", siLeftMon);
		}

		SendWarMsg(cmd, mapindex1, x1, y1, mapindex2, x2, y2,
			siVillageUnique[0], siVillageWarKillNumber[0],
			siVillageUnique[1], siVillageWarKillNumber[1],
			siVillageUnique[2], siVillageWarKillNumber[2], siPorterVillageUnique);

	}

	// 독도 이벤트를 돌린다. 
	//if ( pclserver->sTime.wDayOfWeek == 0 || pclserver->sTime.wDayOfWeek == 6 )
	{
		pclserver->pclWarDockDoManager->Action(cmd) ;
	}
}


void cltWarManager::SetDraw(SI32 cmd)
{
	bDrawSwitch = true;

	switch(cmd)
	{
	case WAR_CMD_BREAK:	siDrawCmdFont	= 0;	pclClient->PushEffect(EFFECT_WARBREAK, 0, 0);	break;
	case WAR_CMD_WIN:	siDrawCmdFont	= 1;	pclClient->PushEffect(EFFECT_WARWIN, 0, 0);		break;
	case WAR_CMD_LOSE:	siDrawCmdFont	= 2;	pclClient->PushEffect(EFFECT_WARLOSE, 0, 0);	break;
	case WAR_OSAKA_BREAK:	siDrawCmdFont	= 3;	pclClient->PushEffect(EFFECT_WARBREAK, 0, 0);	break;
	case WAR_OSAKA_WIN:	siDrawCmdFont	= 4;	pclClient->PushEffect(EFFECT_WARWIN, 0, 0);		break;
	case WAR_OSAKA_LOSE:	siDrawCmdFont	= 5;	pclClient->PushEffect(EFFECT_WARLOSE, 0, 0);	break;
	}

	dwDrawStartClock = pclClient->CurrentClock;
}

// 전황을 표시한다. 
void cltWarManager::Draw(LPDIRECTDRAWSURFACE7 lpsurface, SI32 rx, SI32 ry)
{

	if(bDrawSwitch == false)return ;

	if( TABS(pclClient->CurrentClock - dwDrawStartClock) > 10000)
	{
		bDrawSwitch = false;
	}

	TSpr* pspr = pclClient->GetGlobalSpr(GIMG_MAGIC_WAR);

	if(pspr == NULL)return ;

	cltClient* pclclient = (cltClient*)pclClient;

	SI32 yindent = 0 ;

	if ( g_SoundOnOffDlg.m_bFullScreenMode )
	{
		yindent = 30 ;
	}

	if(GP.LockSurface(lpsurface)==TRUE)
	{
		GP.PutSpr(pspr, (pclclient->siClientScreenXsize - pspr->GetXSize())/2 + rx, 
			50  + ry + yindent , siDrawCmdFont);

		GP.UnlockSurface(lpsurface);
	}

}