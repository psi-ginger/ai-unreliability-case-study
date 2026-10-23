//---------------------------------
// 2004/8/19 김태곤
//---------------------------------
#include "Gogu.h"

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

cltEventGogu::cltEventGogu() :cltEvent()
{
	Init();

}

cltEventGogu::~cltEventGogu()
{
	
}
void cltEventGogu::Init()
{
	cltEvent::Init();

	siMapIndex			= 159;

	siTowerID			= 0;
	siTotalSolNumber	= 0;
	siMonsterLevel		= 1;

	for(SI32 i = 0; i< MAX_GOGU_SOL_KIND;i++)
	{
		siSolKind[i]	= 0;
		siSolNum[i]		= 0;
	}

}

// 이벤트 시작. 
// bwarswitch : 전쟁의 영향을 받는가.. 
// 리턴 :0 -> 이미 이벤트가 진행중이다(다른 마을 혹은 이 마을에서) 
//		-1 -> 왜구 전쟁 중에는 이벤트를 진행할 수 없다.
SI32 cltEventGogu::Start(SI32 sivillageunique, bool bwarswitch)
{
	// 이미 이벤트 중이면 시작할 수 없다. 
	if(bEventSwitch == true)return 0;

	// 왜구 전쟁을 고려해야 하는 경우에는 왜구 전쟁중에는 이벤트를 시작할 수 없다. 
	if(bwarswitch)
	{
		if(pclClient->pclWarManager->bWarSwitch == true)return -1;	
	}
	
	bEventSwitch = true;

	siVillageUnique = sivillageunique;			// 이벤트 대상이 되는 마을
	dwLeftClock		= 1200000;					// 이벤트 잔여 시간. 
	dwStartClock	= pclClient->CurrentClock;	// 이벤트 시작 시간. 

	AddSol(KIND_CHINASPEARSOL,	6);
	AddSol(KIND_CHINABOWSOL,	6);
	AddSol(KIND_CHINACANNONSOL, 3);




	// 광개토 대왕비를 만든다. 
	MakeTower();

	// DB로 보내진 메시지를 집계한다. 
	((cltServer*)pclClient)->pclStatistics->siGoguEventNumber++;

	
	return 1;
}

// 이벤트 끝.
bool cltEventGogu::End()
{
	// 이미 이벤트가 중단된 상태면 중단할 수 없다. 
	if(bEventSwitch == false)return false;
	

	// 광개토 대왕비를 제거한다. 
	// 몬스터는 없앤다. 
	if(pclClient->pclCM->IsAlive(siTowerID) == true )
	{
		if(pclClient->pclCM->CR[siTowerID]->GetKind() == KIND_GWANGGAETOMONUMENT)
		{
			pclClient->pclCM->CR[siTowerID]->DeleteInCell();
			pclClient->pclCM->DeleteChar(siTowerID);
		}
		
	}

	// 이벤트 몬스터를 모두 제거한다. 
	SI32 index = 0;
	SI32 id;
	while(id = pclClient->pclCM->GetCharOrder(index))
	{
		index++;

		if(IsNPC(id) == false)continue;
		
		cltCharServer* pclchar = (cltCharServer*)pclClient->pclCM->CR[id];
		if(pclchar->siCharMode == CHARMODE_GOGUMON)
		{
			pclchar->DeleteInCell();
			if(pclClient->pclCM->DeleteChar(id) == true)
			{
				index--;
			}
		}
	}
	
	bEventSwitch = false;

	return true;

}


bool cltEventGogu::Action()
{
	// 이미 이벤트가 중단된 상태면 실행할 수 없다. 
	if(bEventSwitch == false)return false;
	
	// 일정 주기로 들어온다. 
	if(pclClient->GetFrame() % 150)return false;
	

	siTotalSolNumber	=	0;
	SI32 index = 0;
	SI32 id;
	while(id = pclClient->pclCM->GetCharOrder(index))
	{
		index++;

		if(IsNPC(id) == false)continue;
		
		cltCharServer* pclchar = (cltCharServer*)pclClient->pclCM->CR[id];
		if(pclchar->siCharMode == CHARMODE_GOGUMON)
		{
			siTotalSolNumber++;
		}
	}
	

	// 남은 병사들이 있다면 병사들을 배치한다. 
	MakeSoldier();

	
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
			pclClient->pclLog->FilePrint("Config\\Gogu.txt", pText, pvillname, siMonsterLevel);
		}

		//-------------------------------------------
		// 승리한 마을에 대박아이템을 보상한다. 
		//-------------------------------------------
		RewardWiner();
		
	}


	if(pclClient->pclCM->IsAlive(siTowerID) == false || pclClient->pclCM->CR[siTowerID]->GetKind() != KIND_GWANGGAETOMONUMENT)
	{
		bloseswitch = true;
		siMonsterLevel =  max(1, (siMonsterLevel - 5));

		char* pvillname = pclClient->pclVillageManager->GetName(siVillageUnique);
		if(pvillname)
		{
			char* pText = GetTxtFromMgr(3678);
			pclClient->pclLog->FilePrint("Config\\Gogu.txt", pText, pvillname, siMonsterLevel);
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
		cltGameMsgResponse_GoguInfo clinfo(siVillageUnique, leftsecond, mode);
		cltMsg clMsg(GAMEMSG_RESPONSE_GOGUINFO, sizeof(clinfo), (char*)&clinfo);
		pclClient->pclCM->SendAllPersonHomeMsg((char*)&clMsg, siVillageUnique);
	}


	return true;
}

// 명나라 군사들을 만든다.
void cltEventGogu::MakeSoldier()
{
	SI32 i, j, k;

	POINT point[3] = {	42, 114,
						40, 71,
						36, 36};

	POINT center = { 115, 101};
	
	// 병사수를 파악한다. 200명을 넘겨서는 안된다. 
	if(siTotalSolNumber >= 200)return ;


	for(i = 0;i < MAX_GOGU_SOL_KIND;i++)
	{
		if(siSolKind[i] == 0)continue;

		// 몬스터를 배치한다. 
		cltServer* pclserver = (cltServer*)pclClient;
		for(j = 0;j < siSolNum[i];j++)
		{
			for(k = 0;k < 3;k++)
			{
				cltInnerStatusInfo clinnerstatus;
				clinnerstatus.clEnemyType.Set(ENEMYTYPE_ALLPC, 0);
				
				SI32 id = pclserver->SetNPCChar(siSolKind[i], VILLAGE_NONE, NULL, siMonsterLevel, 0, point[k].x, point[k].y, siMapIndex, NPCITEMMODE_NORMAL, &clinnerstatus );
				if(id)
				{
					((cltCharServer*)pclClient->pclCM->CR[id])->siCharMode = CHARMODE_GOGUMON;
					OrderMove(id, center.x + 15 - rand() % 30, center.y + 15 - rand() % 30, BY_PLAYER);
				}
			}
		}

	}

}

// 광개토대왕비를 만든다. 
void cltEventGogu::MakeTower()
{
	// 캐릭터  종류를 설정한다. 
	SI32 kind = KIND_GWANGGAETOMONUMENT;

	// 캐릭터 생성 위치를 설정한다.
	SI32 x	= 115;
	SI32 y	= 101;

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


void cltEventGogu::AddSol(SI32 kind, SI32 num)
{
	SI32 i;

	for(i = 0;i < MAX_GOGU_SOL_KIND;i++)
	{
		if(siSolKind[i] == 0)
		{
			siSolKind[i]	= kind;
			siSolNum[i]		= num;
			return ;
		}
	}
}

//-------------------------------------------
// 승리한 마을에 대박아이템을 보상한다. 
//-------------------------------------------
void cltEventGogu::	RewardWiner()
{

	// 마을 주민중 1명에게 대박 상품을 준다. 
	SI16 villagerlist[1000];
	SI32 listindex = 0;
	
	cltServer* pclserver = (cltServer*)pclClient;

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
		if(pclchar->pclCI->clBI.siHomeVillage != siVillageUnique)continue;
		// 이벤트 지역에 위치하고 있어야 한다. 
		if(pclchar->GetMapIndex() != siMapIndex)continue;

		
		SI32 exp = pclchar->pclCI->clIP.GetLevel()  * (30 + siMonsterLevel / 3);
		pclchar->IncreaseExp(exp);
		
		SI32 fameexp = min(50, max(1, exp / 20));
		pclchar->IncreaseFameExp(fameexp);
		
		cltGameMsgResponse_GoguReward clinfo(exp, fameexp, 0);
		cltMsg clMsg(GAMEMSG_RESPONSE_GOGUREWARD, sizeof(clinfo), (char*)&clinfo);
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
				if(pclchar->InputItem(pclClient->pclItemManager, &clItem, INPUTITEMPERSON_OPTION_GOGU, &rtnvalue) == false)
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
