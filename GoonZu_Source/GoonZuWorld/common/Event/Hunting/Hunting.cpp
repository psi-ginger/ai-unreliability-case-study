//---------------------------------
// 2004/8/13 김태곤
//---------------------------------
#include "Hunting.h"

#include "..\..\Char\KindInfo\KindInfo.h"

#include "..\..\CommonLogic\CommonLogic.h"
#include "..\..\CommonLogic\MsgType-System.h"

//-----------------------------
// Common
//-----------------------------
#include "PersonInfo.h"
#include "Char\CharManager\CharManager.h"

//-------------------------------------
// Server
//-------------------------------------
#include "..\..\Server\Server.h"
#include "..\..\Server\Statistics\Statistics.h"

#include "..\DBManager\GameDBManager_World\DBMsg-Village.h"
#include "..\DBManager\GameDBManager_World\DBMsg-Item.h"

#include "../common/Event/event.h"

extern cltCommonLogic* pclClient;

cltEventHunting::cltEventHunting()
{
	Init();
}

cltEventHunting::~cltEventHunting()
{
}

void cltEventHunting::Init()
{
	cltEvent::Init();

	siMonsterID		= 0;
	siMonsterLevel	= 10;
	siCost			= 0;
	//siHuntingEventcount = 0;
	m_arEventCount_EachVillage.ZeroMem();
}

// 불가사리 사냥 이벤트를 시작한다. 
bool cltEventHunting::Start(SI32 sivillageunique, SI32 cost)
{
	// 이미 이벤트 중이면 시작할 수 없다. 
	if(bEventSwitch == true)return false;

	bEventSwitch = true;

	siVillageUnique = sivillageunique;			// 이벤트 대상이 되는 마을
	dwLeftClock		= 30 * 60 * 1000;			// 이벤트 잔여 시간 - 현 30분
	dwStartClock	= pclClient->CurrentClock;	// 이벤트 시작 시간
    siCost			= cost;						// 이벤트비용	

	//HuntingEventCount_Increase();

	// 몬스터를 만든다. 
	MakeMonster();

	// DB로 보내진 메시지를 집계한다. 
	((cltServer*)pclClient)->pclStatisticsManager->clDailyStatistics.siHuntingEventNumber++;

	pclClient->pclLog->FilePrint(TEXT("Config\\Hunting.log"), TEXT("[EVENTSTART]\tVillageUnique=%d\tCost=%d"), siVillageUnique, siCost);

	return true;
	 
}



// 불가사리 사냥 이벤트를 시작한다. 
void cltEventHunting::End()
{
	// 이미 이벤트가 중단된 상태면 중단할 수 없다. 
	if(bEventSwitch == false)return ;

	bEventSwitch = false;

	pclClient->pclLog->FilePrint(TEXT("Config\\Hunting.log"), TEXT("[EVENTEND]") );
	// 몬스터는 없앤다. 
	if(pclClient->pclCM->IsAlive(siMonsterID) == TRUE )
	{
		if(pclClient->pclCM->CR[siMonsterID]->clIdentity.siIdentity == IDENTITY_VILLAGEHUNT)
		{
			pclClient->pclCM->CR[siMonsterID]->DeleteInCell();
			pclClient->pclCM->DeleteChar(siMonsterID);
		}
		
	}

    _Contributors.clear();
}


// 불가사리 사냥 이벤트 진행. 
bool cltEventHunting::Action()
{
	bool bwinswitch = false;
	bool bloseswitch = false;


	// 일정 주기로 들어온다. 
	if(pclClient->GetFrame() % 111)return false;


	// 이미 이벤트가 중단된 상태면 실행할 수 없다. 
	if(bEventSwitch == false)return false;

	// 시간이 초과되면 패배 
	DWORD dwelapsedclock = pclClient->CurrentClock - dwStartClock;
	if(dwelapsedclock >= dwLeftClock)
	{
		bloseswitch = true;

		TCHAR* pvillname = pclClient->pclVillageManager->GetName(siVillageUnique);
		if(pvillname)
		{
			TCHAR* pText = GetTxtFromMgr(5086);
			pclClient->pclLog->FilePrint(TEXT("Config\\Hunting.log"), pText, pvillname, siMonsterLevel, siCost);
		}
	}

	SI32 leftsecond;
	if(dwLeftClock > dwelapsedclock)
	{
		leftsecond = (dwLeftClock - dwelapsedclock) / 1000 ;
	}
	else
	{
		leftsecond = 0;
	}

	// 사냥감을 죽였다. (승리) 
	if(pclClient->pclCM->IsAlive(siMonsterID) == false)
	{
		HuntingEventCount_Increase();

		bwinswitch = true;

		TCHAR* pvillname = pclClient->pclVillageManager->GetName(siVillageUnique);
		if(pvillname)
		{
			TCHAR* pText = GetTxtFromMgr(3640);
			pclClient->pclLog->FilePrint(TEXT("Config\\Hunting.log"), pText, pvillname, siMonsterLevel, siCost, leftsecond);
		}

		// 첫번째 수행일땐 공간상자 아이템 지급
		cltServer* pServer = (cltServer*)pclClient;
		if ( NULL == pServer )
		{
			return false;
		}

		SI32 siRewardItemUnique = 0;
		SI32 siRewardItemNum	= 0;

		// 군주데이이고, 라티우스 이벤트 수행 횟 수가 1회일때 공간상자 아이템 지급.
		if ( pServer->pclEventTimeManager->InEventTime(TEXT( "GoonzuDay_Week" ), &pServer->sTime)
			&& EVENT_GOONZUDAY_PLAY_COUNT == HuntingEvnetCount_Get() )
		{
			if(pclClient->IsWhereServiceArea(ConstServiceArea_Korea))
			{
				siRewardItemUnique	= ITEMUNIQUE(24131);		// GE 공간상자 아이템
			}
			else
			{
				siRewardItemUnique	= ITEMUNIQUE(24111);		// 공간상자 아이템
			}
			siRewardItemNum		= 1;
		}

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

			// 경험치
			SI64 exp = leftsecond * 3 * pclchar->pclCI->clIP.GetLevel() / MAX_CHARACTER_LEVEL;// SI32->SI64 경험치 오버플로우때문에 수정	
			pclchar->IncreaseExp(exp);

			if ( false == IsMosterHit( id, exp, siRewardItemUnique, siRewardItemNum ) )
			{
				// 보상 내역 클라에 통보
				cltGameMsgResponse_HuntingReward clinfo(exp, 0, 0, 0);
				cltMsg clMsg(GAMEMSG_RESPONSE_HUNTINGREWARD, sizeof(clinfo), (BYTE*)&clinfo);
				pclchar->SendNetMsg((sPacketHeader*)&clMsg);
			}
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

			//cyj 마을점수관련 : 성공시 DB로 메세지를 보낸다.			
			sDBRequest_VillageEventSuccess clMsg( siVillageUnique );
			((cltServer*)pclClient)->SendDBMsg(DBSELECT_SYSTEM, (sPacketHeader *)&clMsg);

			//if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
			//{
			//	// [로그추가 : 황진성 2007. 10. 9] // 마을 이벤트 성공.
			//	//
			//	cltServer * pclServer = (cltServer*)pclClient;
			//	pclServer->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_VILLAGE, LOGCOMMAND_INDEX_VILLAGE_EVENT_SUCCESS, 
			//									0, NULL, NULL, 0, 0, 0, NULL, 0, 0, 0, siVillageUnique, 0, 0, 
			//									0, 0, 0, 0, 0, NULL, NULL);
			//}

		}
		else if(bloseswitch)
		{
			mode = -1;
		}
		cltGameMsgResponse_HuntingInfo clinfo(siVillageUnique, leftsecond, mode);
		cltMsg clMsg(GAMEMSG_RESPONSE_HUNTINGINFO, sizeof(clinfo), (BYTE*)&clinfo);
		pclClient->pclCM->SendAllPersonHomeMsg((sPacketHeader*)&clMsg, siVillageUnique);
	}

	return true;
}

// 사냥감을 만든다. 
void cltEventHunting::MakeMonster()
{
	SI32 i;
	// 몬스터 종류를 설정한다. 
	SI32 kind = pclClient->GetUniqueFromHash( TEXT("KIND_CHIMERA"));

	// 해당 마을을 리스트에 추가한다.
	SI16 villagelist[MAX_VILLAGE_NUMBER];
	SI32 index = 0;
	for(i = 0;i < MAX_VILLAGE_NUMBER;i++)
	{
		villagelist[i] = 0;
	}

	for(i = 0;i < MAX_VILLAGE_NUMBER;i++)
	{
		if(pclClient->pclVillageManager->pclVillageInfo[i])
		{
			villagelist[index] = i;
			index++;
		}
	}

	SI32 villageunique = 0;
	if(index)
	{
		villageunique = villagelist[rand() % index];
	}
	else
	{
		MsgBox(TEXT("fdsfiff"), TEXT("f3fdf"));
		return ;
	}


	// 찾은 마을 주변에 몬스터를 만든다.
	SI32 tempx, tempy;
	SI32 x, y;
	do
	{
		tempx = pclClient->pclVillageManager->pclVillageInfo[villageunique]->PosX + (5 - rand() % 10);
		tempy = pclClient->pclVillageManager->pclVillageInfo[villageunique]->PosY + (5 - rand() % 10);
		
	}
	while(pclClient->pclMapManager->pclMapArray[0]->FindEmptyAreaForCharLocate(kind, tempx, tempy, 10, &x, &y, true) == false);

	// 몬스터를 배치한다. 
	cltIdentity clIdentity;
	clIdentity.MakeHunt(IDENTITY_VILLAGEHUNT, siVillageUnique);

	cltServer* pclserver = (cltServer*)pclClient;

	cltInnerStatusInfo clinnerstatus;
	clinnerstatus.clEnemyType.Set(ENEMYTYPE_ONEVILLAGE, siVillageUnique);

	if(	pclClient->IsCountrySwitch( Switch_Event_Hunt_Reform ) )
	{
		//[진성]  레벨을 80~120 에서 5단위로 랜덤하게 => 2008-5-30
		siMonsterLevel = ((rand() % 9) + 16) * 5;
	}
	else
	{
		// 레벨을 1~100 에서 10단위로 랜덤하게
		siMonsterLevel = ((rand() % 10) + 1) * 10;
	}

	SI32 id = pclserver->SetNPCChar(kind, VILLAGE_NONE, &clIdentity, siMonsterLevel, 0, x, y, 0, NPCITEMMODE_NORMAL, &clinnerstatus );

	if(id)
	{
		siMonsterID	= id;	// 몬스터의 아이디를 확보한다.
	}

	pclClient->pclLog->FilePrint(TEXT("Config\\Hunting.log"), TEXT("[MAKEMONSTER]\tVillageUnique=%d\tX=%d\tY=%d\tLevel=%d"), siVillageUnique, x, y, siMonsterLevel );
}

// 한대라도 친사람은 다~ 추가
void cltEventHunting::AddContributor(SI32 AttackerID)
{
	_Contributors.insert(AttackerID);
}

void cltEventHunting::HuntingEventCount_Increase()
{
	//++siHuntingEventcount;
	if ( MAX_VILLAGE_NUMBER <= siVillageUnique || 0 > siVillageUnique ) return;
	m_arEventCount_EachVillage[siVillageUnique] = m_arEventCount_EachVillage[siVillageUnique] + 1;
}

void cltEventHunting::HuntingEventCount_Reset()
{
	//siHuntingEventcount = 0;
	m_arEventCount_EachVillage.ZeroMem();
}

SI16 cltEventHunting::HuntingEvnetCount_Get()
{
	//return siHuntingEventcount;
	if ( MAX_VILLAGE_NUMBER <= siVillageUnique || 0 > siVillageUnique ) return 0;
	else return m_arEventCount_EachVillage[siVillageUnique];
}

bool cltEventHunting::IsMosterHit(SI32 siCharID, SI64 siExp, SI32 siRewardItemUnique, SI32 siRewardItemNum)
{
	std::set<SI32>::const_iterator Iter = _Contributors.begin();

	while(Iter != _Contributors.end())
	{
		if ( siCharID == (*Iter) )
		{
			if(pclClient->pclCM->IsValidID(*Iter) && pclClient->pclCM->CR[*Iter]->pclCI->clBI.siHomeVillage == siVillageUnique)
			{
				((cltCharServer *)pclClient->pclCM->CR[(*Iter)])->GiveItem(24000, 1, INPUTITEMPERSON_OPTION_CHIMERA_REWARD, 0);

				// 상자
				if ( 0 < siRewardItemUnique )
				{
					((cltCharServer *)pclClient->pclCM->CR[(*Iter)])->GiveItem(siRewardItemUnique, siRewardItemNum, INPUTITEMPERSON_OPTION_CHIMERA_REWARD, 0);

					// 보상 내역 클라에 통보
					cltGameMsgResponse_HuntingReward clinfo(siExp, 0, siRewardItemUnique, siRewardItemNum);
					cltMsg clMsg(GAMEMSG_RESPONSE_HUNTINGREWARD, sizeof(clinfo), (BYTE*)&clinfo);
					((cltCharServer *)pclClient->pclCM->CR[(*Iter)])->SendNetMsg((sPacketHeader*)&clMsg);

					return true;
				}
			}
		}

		++Iter;
	}

	return false;
}