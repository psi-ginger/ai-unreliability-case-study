//---------------------------------
// 2005/06/15 김광명
//---------------------------------
#include "Kanghwado.h"

//--------------------------------
// Common
//--------------------------------
#include "..\..\Char\KindInfo\KindInfo.h"
#include "..\..\Order\Order.h"
#include "..\..\War\War.h"
#include "PersonInfo.h"
#include "Char\CharManager\CharManager.h"

//--------------------------------
// CommonLogic
//--------------------------------
#include "..\..\CommonLogic\CommonLogic.h"
#include "..\..\CommonLogic\MsgType-System.h"
#include "..\..\CommonLogic\MsgRval-Define.h"


//--------------------------------
// Server
//--------------------------------
#include "Server.h"
#include "Statistics\Statistics.h"

#include "..\..\DBManager\GameDBManager\DBMsg-Item.h"

extern cltCommonLogic* pclClient;

cltEventKanghwado::cltEventKanghwado() :cltEvent()
{
	Init();

}

cltEventKanghwado::~cltEventKanghwado()
{

}
void cltEventKanghwado::Init()
{
	cltEvent::Init();

	siMapIndex			= MAPINDEX_KANGHWADO;

	siTowerID			= 0;
	siTotalSolNumber	= 0;
	siTotalPatrolSolNumber = 0;
	siTotalShipNumber	= 0;
	siTotalMotherShipNumber = 0;
	siMonsterLevel		= 1;
	siMaxSolNumber      = 200;
	bWarStart			= false;

	for(SI32 i = 0; i< MAX_KANGHWADO_SOL_KIND;i++)
	{
		siSolKind[i]	= 0;
		siSolNum[i]		= 0;
	}

	for(i = 0; i< MAX_KANGHWADO_SOLSHIP_KIND;i++)
	{
		siSolShipKind[i]	= 0;
		siSolShipNum[i]		= 0;
	}
	
	for( i=0; i<MAX_WESTMOTHERSHIP; ++i)
	{
		bMakeTroopShip[i] = false;
	}

	// 서양 모선 좌표 설정
	for( i=0; i<MAX_WESTMOTHERSHIP; ++i)
	{
		MotherShipPoint[i].x = 40 + 50*i;
		MotherShipPoint[i].y = 180;
	}
	for( i=0; i<MAX_WESTMOTHERSHIP; ++i)
	{
		TroopShipArrivalPoint[i].x = MotherShipPoint[i].x;
		TroopShipArrivalPoint[i].y = 138;
	}
}

// 이벤트 시작. 
// bwarswitch : 전쟁의 영향을 받는가.. 
// 리턴 :0 -> 이미 이벤트가 진행중이다(다른 마을 혹은 이 마을에서) 
//		-1 -> 왜구 전쟁 중에는 이벤트를 진행할 수 없다.
SI32 cltEventKanghwado::Start(SI32 villageunique, bool warstart)
{
	// 이미 이벤트 중이면 시작할 수 없다. 
	if(bEventSwitch == true)
		return 0;

	bEventSwitch = true;

	siVillageUnique = villageunique;			// 이벤트 대상이 되는 마을
	bWarStart		= warstart;					// true는 오후2시 전쟁때다
#ifdef _DEBUG
	dwLeftClock		= 900000;					// 이벤트 잔여 시간. 
#else
	dwLeftClock		= 900000;	
#endif
	dwStartClock	= pclClient->CurrentClock;	// 이벤트 시작 시간. 

	AddSol(KIND_WESTSWORDSOL,	4);
	AddSol(KIND_WESTGUNSOL,	4);
	AddSol(KIND_WESTOFFICER,	2);
	AddShip(KIND_WESTBATTLEBOAT, 2) ;
//	AddSol(KIND_CHINACANNONSOL, 3);

	// 광개토 대왕비를 만든다. 
	MakeTower();
	MakeCannon();
	MakeMotherShip();

	// DB로 보내진 메시지를 집계한다. 
	((cltServer*)pclClient)->pclStatistics->siKanghwadoEventNumber++;

	return 1;
}

// 이벤트 끝.
bool cltEventKanghwado::End()
{
	// 이미 이벤트가 중단된 상태면 중단할 수 없다. 
	if(bEventSwitch == false)
		return false;


	// 강화도 보호비를 제거한다. 
	// 몬스터는 없앤다. 
	if(pclClient->pclCM->IsAlive(siTowerID) == TRUE )
	{
		if(pclClient->pclCM->CR[siTowerID]->GetKind() == KIND_GANGHWAGATE)
		{
			pclClient->pclCM->CR[siTowerID]->DeleteInCell();
			pclClient->pclCM->DeleteChar(siTowerID);
		}
	}

	SI32 i;
	for ( i = 0 ; i < MAX_CANNON_NUM ; ++i )
	{

		if(pclClient->pclCM->IsAlive(KanghwaCannon[i]) == TRUE )
		{
			if(pclClient->pclCM->CR[KanghwaCannon[i]]->GetKind() == KIND_GANGHWACANNON)
			{
				pclClient->pclCM->CR[KanghwaCannon[i]]->DeleteInCell();
				pclClient->pclCM->DeleteChar(KanghwaCannon[i]);
			}

		}
	}
	// 이벤트 몬스터를 모두 제거한다. 
	SI32 index = 0;
	SI32 id;
	while(id = pclClient->pclCM->GetCharOrder(index))
	{
		index++;

		if(IsNPC(id) == false && IsShip(id) == false)continue;

		cltCharServer* pclchar = (cltCharServer*)pclClient->pclCM->CR[id];
		if(pclchar->siCharMode == CHARMODE_KANGHWADOMON_PART1)
		{
			pclchar->DeleteInCell();
			if(pclClient->pclCM->DeleteChar(id) == TRUE)
			{
				index--;
			}
		}
		else if(pclchar->siCharMode == CHARMODE_KANGHWADOMON_PART2)
		{
			pclchar->DeleteInCell();
			if(pclClient->pclCM->DeleteChar(id) == TRUE)
			{
				index--;
			}
		}
		else if(pclchar->siCharMode == CHARMODE_KANGHWADOBATTLESHIP)
		{
			pclchar->DeleteInCell();
			if(pclClient->pclCM->DeleteChar(id) == TRUE)
			{
				index--;
			}
		}
		else if(pclchar->siCharMode == CHARMODE_KANGHWADOMOTHERSHIP)
		{
			pclchar->DeleteInCell();
			if(pclClient->pclCM->DeleteChar(id) == TRUE)
			{
				index--;
			}
		}
	}

	bEventSwitch = false;

	DelSolAll();
	DelShipAll();

	for( i=0; i<MAX_WESTMOTHERSHIP; ++i)
	{
		bMakeTroopShip[i] = false;
	}

	return true;

}


bool cltEventKanghwado::Action()
{
	// 이미 이벤트가 중단된 상태면 실행할 수 없다. 
	if(bEventSwitch == false)return false;

	// 일정 주기로 들어온다. 
	if(pclClient->GetFrame() % 150)return false;


	siTotalSolNumber	=	0;
	siTotalPatrolSolNumber = 0;
	siTotalShipNumber	=   0;
	siTotalMotherShipNumber = 0;
	SI32 index = 0;
	SI32 id;
	while(id = pclClient->pclCM->GetCharOrder(index))
	{
		index++;

		if(IsNPC(id) == false && IsShip(id) == false )continue;

		cltCharServer* pclchar = (cltCharServer*)pclClient->pclCM->CR[id];
		if(pclchar->siCharMode == CHARMODE_KANGHWADOMON_PART1)
		{
			siTotalSolNumber++;
		}
		else if(pclchar->siCharMode == CHARMODE_KANGHWADOMON_PART2)
		{
			siTotalPatrolSolNumber++;
		}
		else if(pclchar->siCharMode == CHARMODE_KANGHWADOBATTLESHIP)
		{
			siTotalShipNumber++;
		}
		else if(pclchar->siCharMode == CHARMODE_KANGHWADOMOTHERSHIP)
		{
			siTotalMotherShipNumber++;
		}
	}

	SI32 i;
	for(i=0; i<MAX_WESTMOTHERSHIP; ++i)
	{
		if(pclClient->pclCM->IsAlive(siMotherShipID[i]))
		{
			// 수송선이 안 만들어져 있으므로 수송선 만든다
			if(bMakeTroopShip[i] == false)
			{
				// 몬스터를 배치한다. 
				cltServer* pclserver = (cltServer*)pclClient;

				cltInnerStatusInfo clinnerstatus;
				clinnerstatus.clEnemyType.Set(ENEMYTYPE_ALLNPC, 0);		// 모든 NPC와 적대적, 사람과는 우호관계. 

				SI32 id = pclserver->SetNPCChar(KIND_WESTBOAT, VILLAGE_NONE, NULL, 1, 0, MotherShipPoint[i].x, MotherShipPoint[i].y, siMapIndex, NPCITEMMODE_NORMAL, &clinnerstatus );
				
				if(id)
				{
					((cltCharServer*)pclClient->pclCM->CR[id])->siCharMode = CHARMODE_KANGHWADOMON_PART1;
					OrderMove(id, TroopShipArrivalPoint[i].x, TroopShipArrivalPoint[i].y, BY_PLAYER);
					siTroopShipID[i] = id;

					// 수송선이 만들어 졌다
					bMakeTroopShip[i] = true;
				}
			}
		}	
	}

	// 수송선이 해당 좌표에 도달하면 삭제 시킨다!!
	for(i=0; i<MAX_WESTMOTHERSHIP; ++i)
	{
		if(bMakeTroopShip[i])
		{
			if( pclClient->pclCM->CR[siTroopShipID[i]]->GetX() == TroopShipArrivalPoint[i].x &&
				pclClient->pclCM->CR[siTroopShipID[i]]->GetY() == TroopShipArrivalPoint[i].y )
			{
				cltCharServer* pclchar = (cltCharServer*)pclClient->pclCM->CR[siTroopShipID[i]];
				if(pclchar->siCharMode == CHARMODE_KANGHWADOMON_PART1)
				{
					pclchar->DeleteInCell();
					if(pclClient->pclCM->DeleteChar(siTroopShipID[i]) == TRUE)
					{
						bMakeTroopShip[i] = false;
					}
				}
			}
		}
	}

	// 남은 병사들이 있다면 병사들을 배치한다. 
	MakeSoldier();
	MakeBattleBoat();

	// 2시 전쟁일때 정찰병 생성
//	if( bWarStart == true )
	MakePatrolSoldier();

	// 승패 판정. 
	bool bwinswitch		= false;
	bool bloseswitch	= false;


	// 시간이 초과되면 승리 
	DWORD dwelapsedclock = pclClient->CurrentClock - dwStartClock;

	SI32 leftsecond;
	if(dwLeftClock > dwelapsedclock)
	{
		leftsecond = (dwLeftClock - dwelapsedclock) / 1000 ;
	}
	else
	{
		leftsecond = 0;
	}


	if(dwelapsedclock >= dwLeftClock)
	{
		bwinswitch = true;
		if(siMonsterLevel < 100)siMonsterLevel +=5;

		char* pvillname = pclClient->pclVillageManager->GetName(siVillageUnique);
		if(pvillname)
		{
			char* pText = GetTxtFromMgr(3677);
			pclClient->pclLog->FilePrint("Config\\Kanghwado.txt", pText, pvillname, siMonsterLevel);
		}

		//-------------------------------------------
		// 승리한 마을에 대박아이템을 보상한다. 
		//-------------------------------------------
		RewardWiner();

	}
	else if(siTotalMotherShipNumber == 0)	// 모선이 전부 파괴되면
	{
		bwinswitch = true;
		if(siMonsterLevel < 100)siMonsterLevel +=5;

		char* pvillname = pclClient->pclVillageManager->GetName(siVillageUnique);
		if(pvillname)
		{
			char* pText = GetTxtFromMgr(3677);
			pclClient->pclLog->FilePrint("Config\\Kanghwado.txt", pText, pvillname, siMonsterLevel);
		}

		//-------------------------------------------
		// 승리한 마을에 대박아이템을 보상한다. 
		//-------------------------------------------
		RewardWiner();
	}

	if(pclClient->pclCM->IsAlive(siTowerID) == false || pclClient->pclCM->CR[siTowerID]->GetKind() != KIND_GANGHWAGATE)
	{
		bloseswitch = true;
		siMonsterLevel =  max(1, (siMonsterLevel - 5));

		char* pvillname = pclClient->pclVillageManager->GetName(siVillageUnique);
		if(pvillname)
		{
			char* pText = GetTxtFromMgr(3678);
			pclClient->pclLog->FilePrint("Config\\Kanghwado.txt", pText, pvillname, siMonsterLevel);
		}

	}

	if(bwinswitch || bloseswitch)
	{
		End();
	}


	if(TABS(pclClient->CurrentClock - dwLastInfoClock) > 3000)
	{
		dwLastInfoClock = pclClient->CurrentClock;

		// 현재 상황을 이벤트 대상자들에게 통보한다.(잔여시간..) 
		SI32 mode = 0;
		if(bwinswitch)
		{
			mode = 1;
		}
		else if(bloseswitch)
		{
			mode = -1;
		}

		if( bWarStart == true )
		{
			cltGameMsgResponse_KanghwadoInfo clinfo(0, leftsecond, mode);
			cltMsg clMsg(GAMEMSG_RESPONSE_KANGHWADOINFO, sizeof(clinfo), (char*)&clinfo);
			pclClient->pclCM->SendAllPersonMsg((char*)&clMsg);
		}
		else
		{
			cltGameMsgResponse_KanghwadoInfo clinfo(siVillageUnique, leftsecond, mode);
			cltMsg clMsg(GAMEMSG_RESPONSE_KANGHWADOINFO, sizeof(clinfo), (char*)&clinfo);
			pclClient->pclCM->SendAllPersonHomeMsg((char*)&clMsg, siVillageUnique);
		}
	}


	return true;
}

// 서양 군사들을 만든다.
void cltEventKanghwado::MakeSoldier()
{
	// 병사수를 파악한다. 200명을 넘겨서는 안된다. 마스터 명령으로 조절가능
	if(siTotalSolNumber >= siMaxSolNumber)return ;

	SI32 i, j, k;

	POINT point[3] = {	40, 110,
						90, 110,
						140, 110,
					};

	POINT center = { 88, 43};

	for(i = 0;i < MAX_KANGHWADO_SOL_KIND;i++)
	{
		if(siSolKind[i] == 0)continue;

		// 몬스터를 배치한다. 
		cltServer* pclserver = (cltServer*)pclClient;
		for(j = 0;j < siSolNum[i];j++)
		{
			for(k = 0;k < 3;k++)
			{
			//	if( pclClient->pclCM->IsAlive(siMotherShipID[k]) == false )
			//		continue;

				cltInnerStatusInfo clinnerstatus;
				clinnerstatus.clEnemyType.Set(ENEMYTYPE_ALLPC, 0);

				SI32 id = pclserver->SetNPCChar(siSolKind[i], VILLAGE_NONE, NULL, siMonsterLevel, 0, point[k].x, point[k].y, siMapIndex, NPCITEMMODE_NORMAL, &clinnerstatus );
				if(id)
				{
					((cltCharServer*)pclClient->pclCM->CR[id])->siCharMode = CHARMODE_KANGHWADOMON_PART1;
					OrderMove(id, center.x + 15 - rand() % 30, center.y - rand() % 5, BY_PLAYER);
				}
			}
		}

	}

}

// 양이 정찰병을 만든다
void cltEventKanghwado::MakePatrolSoldier()
{
	// 병사수를 파악한다. 200명을 넘겨서는 안된다. 
	if(siTotalPatrolSolNumber >= 100)
		return ;

	SI32 kind = KIND_WESTPATROLSOL;
	SI32 i, j, k;

	POINT center = { 88, 37};

	// 몬스터를 배치한다. 
	cltServer* pclserver = (cltServer*)pclClient;
	for(i = 0;i < 10;i++)
	{
		cltInnerStatusInfo clinnerstatus;
		clinnerstatus.clEnemyType.Set(ENEMYTYPE_ALLPC, 0);

		SI32 id = pclserver->SetNPCChar(kind, VILLAGE_NONE, NULL, siMonsterLevel, 0, 10 + rand()%160, 10 + rand()%15, siMapIndex, NPCITEMMODE_NORMAL, &clinnerstatus );
		if(id)
		{
			((cltCharServer*)pclClient->pclCM->CR[id])->siCharMode = CHARMODE_KANGHWADOMON_PART2;
		}
	}
}

// 서양 군사들을 만든다.
void cltEventKanghwado::MakeBattleBoat()
{
	// 병선수 60으로 하자
	if(siTotalShipNumber >= 60)return ;

	SI32 i, j, k;

	for(i = 0;i < MAX_KANGHWADO_SOLSHIP_KIND;i++)
	{
		if(siSolShipKind[i] == 0)continue;

		// 몬스터를 배치한다. 
		cltServer* pclserver = (cltServer*)pclClient;
		for(j = 0;j < siSolShipNum[i];j++)
		{
			for(k = 0;k < 3;k++)
			{
				//	if( pclClient->pclCM->IsAlive(siMotherShipID[k]) == false )
				//		continue;

				cltInnerStatusInfo clinnerstatus;
				clinnerstatus.clEnemyType.Set(ENEMYTYPE_ALLPC, 0);

				SI32 id = 0;
				if( k == 0)	// 모선 앞에서 나오게 하자!!
					id = pclserver->SetNPCChar(siSolShipKind[i], VILLAGE_NONE, NULL, siMonsterLevel, 0, MotherShipPoint[k].x, MotherShipPoint[k].y - 5 - rand()%10, siMapIndex, NPCITEMMODE_NORMAL, &clinnerstatus );
				else if( k == 1) // 모선 왼쪽에서 나오게 하자!!
					id = pclserver->SetNPCChar(siSolShipKind[i], VILLAGE_NONE, NULL, siMonsterLevel, 0, MotherShipPoint[k].x - 5 - rand()%10, MotherShipPoint[k].y - 5 + rand()%5, siMapIndex, NPCITEMMODE_NORMAL, &clinnerstatus );
				else if( k == 2) // 모선 오른쪽에서 나오게 하자!!
					id = pclserver->SetNPCChar(siSolShipKind[i], VILLAGE_NONE, NULL, siMonsterLevel, 0, MotherShipPoint[k].x + 5 + rand()%10, MotherShipPoint[k].y - 5 + rand()%5, siMapIndex, NPCITEMMODE_NORMAL, &clinnerstatus );
				
				if(id)
				{
					((cltCharServer*)pclClient->pclCM->CR[id])->siCharMode = CHARMODE_KANGHWADOBATTLESHIP;
				}
				
			}
		}

	}

}

// 서양 배를 만든다.
void cltEventKanghwado::MakeMotherShip()
{
	SI32 i;
	SI32 kind = KIND_WESTMOTHERSHIP;

	// 배를 배치한다. 
	cltServer* pclserver = (cltServer*)pclClient;
	
	for(i = 0;i < MAX_WESTMOTHERSHIP;++i)
	{
		cltInnerStatusInfo clinnerstatus;
		clinnerstatus.clEnemyType.Set(ENEMYTYPE_ALLPC, 0);

		SI32 id = pclserver->SetNPCChar(kind, VILLAGE_NONE, NULL, siMonsterLevel, 0, MotherShipPoint[i].x, MotherShipPoint[i].y, siMapIndex, NPCITEMMODE_NORMAL, &clinnerstatus );
		if(id)
		{
			((cltCharServer*)pclClient->pclCM->CR[id])->siCharMode = CHARMODE_KANGHWADOMOTHERSHIP;
			siMotherShipID[i] = id;
		}
	}
}

// 강화도 성문을 만든다. 
void cltEventKanghwado::MakeTower()
{
	// 캐릭터  종류를 설정한다. 
	SI32 kind = KIND_GANGHWAGATE;

	// 캐릭터 생성 위치를 설정한다.
	SI32 x	= 88;
	SI32 y	= 37;

	// 몬스터를 배치한다. 
	cltServer* pclserver = (cltServer*)pclClient;

	cltInnerStatusInfo clinnerstatus;
	clinnerstatus.clEnemyType.Set(ENEMYTYPE_ALLNPC, 0);		// 모든 NPC와 적대적, 사람과는 우호관계. 

	SI32 id = pclserver->SetNPCChar(kind, VILLAGE_NONE, NULL, 1, 0, x, y, siMapIndex, NPCITEMMODE_NORMAL, &clinnerstatus );
	if(id)
	{
		siTowerID	= id;	// 몬스터의 아이디를 확보한다.
	}
}

void cltEventKanghwado::MakeCannon()
{
	SI32 kind = KIND_GANGHWACANNON;

	// 몬스터를 배치한다. 
	cltServer* pclserver = (cltServer*)pclClient;

	SI32 CannonX1[3]=
	{
		33, 36, 39
	};

	SI32 CannonX2[4]=
	{
		85, 88, 91, 94
	};

	SI32 CannonX3[3]=
	{
		122, 125, 128
	};
	
	SI32 i;
	for(i=0;i<3;++i)
	{
		cltInnerStatusInfo clinnerstatus;	
		clinnerstatus.clEnemyType.Set(ENEMYTYPE_ALLNPC, 0);		// 모든 PC와 우호적, 사람과는 적대적. 

		SI32 id = pclserver->SetNPCChar(kind, VILLAGE_NONE, NULL, 1, 0, CannonX1[i] , 45, siMapIndex, NPCITEMMODE_NORMAL, &clinnerstatus );
		if(id)
		{
			KanghwaCannon[i]	= id;	// 몬스터의 아이디를 확보한다.
		}
	}

	for(i=0;i<4;++i)
	{
		cltInnerStatusInfo clinnerstatus;	
		clinnerstatus.clEnemyType.Set(ENEMYTYPE_ALLNPC, 0);		// 모든 PC와 우호적, 사람과는 적대적. 

		SI32 id = pclserver->SetNPCChar(kind, VILLAGE_NONE, NULL, 1, 0, CannonX2[i] , 55, siMapIndex, NPCITEMMODE_NORMAL, &clinnerstatus );
		if(id)
		{
			KanghwaCannon[i + 3]	= id;	// 몬스터의 아이디를 확보한다.
		}
	}

	for(i=0;i<3;++i)
	{
		cltInnerStatusInfo clinnerstatus;	
		clinnerstatus.clEnemyType.Set(ENEMYTYPE_ALLNPC, 0);		// 모든 PC와 우호적, 사람과는 적대적. 

		SI32 id = pclserver->SetNPCChar(kind, VILLAGE_NONE, NULL, 1, 0, CannonX3[i] , 45, siMapIndex, NPCITEMMODE_NORMAL, &clinnerstatus );
		if(id)
		{
			KanghwaCannon[i + 7]	= id;	// 몬스터의 아이디를 확보한다.
		}
	}
}


void cltEventKanghwado::AddSol(SI32 kind, SI32 num)
{
	SI32 i;

	for(i = 0;i < MAX_KANGHWADO_SOL_KIND;i++)
	{
		if(siSolKind[i] == 0)
		{
			siSolKind[i]	= kind;
			siSolNum[i]		= num;
			return ;
		}
	}
}

void cltEventKanghwado::AddShip(SI32 kind, SI32 num)
{
	SI32 i;

	for(i = 0;i < MAX_KANGHWADO_SOLSHIP_KIND;i++)
	{
		if(siSolShipKind[i] == 0)
		{
			siSolShipKind[i]	= kind;
			siSolShipNum[i]		= num;
			return ;
		}
	}
}

//-------------------------------------------
// 승리한 마을에 대박아이템을 보상한다. 
//-------------------------------------------
void cltEventKanghwado::	RewardWiner()
{
	cltServer* pclserver = (cltServer*)pclClient;

	// 마을 주민중 1명에게 대박 상품을 준다. 
	SI16 villagerlist[1000];
	SI32 listindex = 0;

	

	// 남은 시간을 고려하여 경험치를 준다. 
	SI32 index = 0;
	SI32 id;
	while(id = pclClient->pclCM->GetCharOrder(index))
	{
		index++;

		cltCharServer* pclchar  = (cltCharServer*)pclClient->pclCM->CR[id];
		// 사용자인 컴에게만 전달한다. 
		if(pclchar->IsValidConnection() == false)continue;
		// 해당 마을 주민에게만 준다.
		if(bWarStart == false)
			if(pclchar->pclCI->clBI.siHomeVillage != siVillageUnique)continue;
		// 이벤트 지역에 위치하고 있어야 한다. 
		if(pclchar->GetMapIndex() != siMapIndex)continue;


		SI32 exp = pclchar->pclCI->clIP.GetLevel()  * (30 + siMonsterLevel / 3);
		pclchar->IncreaseExp(exp);

		SI32 fameexp = min(50, max(1, exp / 20));
		pclchar->IncreaseFameExp(fameexp);

		cltGameMsgResponse_KanghwadoReward clinfo(exp, fameexp, 0);
		cltMsg clMsg(GAMEMSG_RESPONSE_KANGHWADOREWARD, sizeof(clinfo), (char*)&clinfo);
		pclchar->SendNetMsg((char*)&clMsg);

		// 마을 주민 리스트를 작성한다. (대박 물품을 주기 위해서 )
		villagerlist[listindex] = id;

		if(listindex < (1000-1))
		{
			listindex++;
		}
	}

	if(listindex)
	{
		id = villagerlist[rand() % listindex];
		cltCharServer* pclchar = (cltCharServer*)pclClient->pclCM->CR[id];

		cltItem clItem;
		SI32 itemunique = pclClient->pclItemManager->GetRandBigWarItem();
		//SI32 ref = pclClient->pclItemManager->FindItemRefFromUnique(itemunique);
		if(itemunique > 0)
		{
			//clItem.Set( &pclClient->pclItemManager->pclItemInfo[ref]->clItem);
			bool RareSwitch = false;
			if ( pclClient->pclItemManager->MakeRandItemUnique( itemunique, &clItem, 0, &RareSwitch,
				0,0,0,0,0,14 ) == true )
			{
				SI32 rtnvalue = 0;
				if(pclchar->InputItem(pclClient->pclItemManager, &clItem, INPUTITEMPERSON_OPTION_KANGHWADO, &rtnvalue) == false)
				{
					// 인벤에 빈자리가 없어서 추가할 수 없다. 
					if(rtnvalue == CANADDINV_FAIL_FULL)
					{
						((cltServer*)pclClient)->SendServerResponseMsg( 0, SRVAL_ADDINV_FAIL_FULL,  clItem.siUnique, 0, pclchar->GetCharUnique());
					}
				}
			}
		}
	}


	// 바닥에 금괴를 지급한다. 
	cltItem clItem;
	bool brareswitch ;

	if(pclserver->pclItemManager->MakeRandItemUnique( 5045, &clItem, 0, &brareswitch) )
	{
		clItem.SetItemNum(1) ;

		SI32 index = 0;
		SI32 totalindex = 0;
		while(index < 100 && totalindex < 1000)
		{
			SI32 x = rand() % pclserver->pclMapManager->pclMapArray[siMapIndex]->GetXsize();
			SI32 y = rand() % pclserver->pclMapManager->pclMapArray[siMapIndex]->GetYsize();

			SI32 targetx, targety;
			if(pclserver->pclMapManager->FindProperAreaForDrop(siMapIndex, x, y, &targetx, &targety) == true)
			{
				index++;
				pclserver->DropItem(siMapIndex, targetx, targety, &clItem, 0);
			}
			totalindex++;
		}
	}
}

void cltEventKanghwado::DelSolAll()
{
	for ( SI32 i = 0 ; i < MAX_KANGHWADO_SOL_KIND ; i ++ )
	{
		if ( siSolKind[i] != 0 )
		{
			siSolKind[i] = 0 ;
			siSolNum[i] = 0 ;
		}
	}
}

void cltEventKanghwado::DelShipAll()
{
	for ( SI32 i = 0 ; i < MAX_KANGHWADO_SOLSHIP_KIND ; i ++ )
	{
		if ( siSolShipKind[i] != 0 )
		{
			siSolShipKind[i] = 0 ;
			siSolShipNum[i] = 0 ;
		}
	}
}
