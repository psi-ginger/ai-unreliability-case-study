//---------------------------------
// 2007/4/16 이강형
//---------------------------------
#include "MofuMofu.h"
#include "Char\CharManager\CharManager.h"
#include "../Server/Server.h"
#include "../CommonLogic/CommonLogic.h"
#include "../CommonLogic/MsgRval-Define.h"

#include "Msg\MsgType-MofuMofuEvent.h"
#include "../DBManager/GameDBManager_World/DBMsg-Item.h"
#include "../DBManager/GameDBManager_World/DBMsg-MofuMofuEvent.h"
#include "../DBManager/GameDBManager_World/DBMsg-Village.h"

#include "../common/Util/Util.h"
#include "../../NLib/NUtil.h"

extern cltCommonLogic* pclClient;

SI32 MofuMofuEvent_Kind[] = 
{
//	KIND_MAMARACCOON		// 마마구리댁
	KIND_MAGICALRACCOON	// 메지컬구리댁
	,KIND_RACCOONLORD		// 구리댁로드
	,KIND_RACCOONTOWER		// 구리댁타워
	,KIND_ZORRORACCOON		// 조로구리댁
};

SI32 cltMofuMofuField::CreateEnemy()
{
	SI32 siRet = 0;
	cltServer* pclserver = (cltServer*)pclClient;

	SI32 siKindCount = sizeof(MofuMofuEvent_Kind) / sizeof(SI32);

	// 최소 몬스터가 남은 경우 재생성한다.
	if(m_siCount > MOFUMOFUEVENT_FIELD_MIN_ENEMY_COUNT)		return 0;

	// 잡을 만큼 잡았다면 더이상 소환하지 않는다.
	if( m_siCreateCount >= cltMofuMofuEvent::m_siMaxEnemyCount / MOFUMOFUEVENT_MAXFIELD_COUNT &&
		m_siKilledCount >= cltMofuMofuEvent::m_siMaxEnemyCount / MOFUMOFUEVENT_MAXFIELD_COUNT)
		return 0;

	// 기본 생성수
	SI32 siCreate = MOFUMOFUEVENT_FIELD_SPWAN_ENEMY_COUNT;

	// 최대 생성수를 초과한 경우 생성수 변경
	if((siKindCount * siCreate) + m_siCreateCount > cltMofuMofuEvent::m_siMaxEnemyCount / MOFUMOFUEVENT_MAXFIELD_COUNT)
	{
		siCreate = (cltMofuMofuEvent::m_siMaxEnemyCount / MOFUMOFUEVENT_MAXFIELD_COUNT) - m_siCreateCount;
		siCreate /= siKindCount;
	}

	// 배열 한도를 초과한 경우 생성수 변경
	if((siKindCount * siCreate) + m_siCount > MOFUMOFUEVENT_FIELD_MAXENEMY_COUNT)
	{
		siCreate = MOFUMOFUEVENT_FIELD_MAXENEMY_COUNT - m_siCount;
		siCreate /= siKindCount;
	}
	siCreate = max(1, siCreate);
	
	SI32 level = 1;
	SI32 x = 0, y = 0;
	cltInnerStatusInfo clinnerstatus;
	clinnerstatus.clEnemyType.Set(ENEMYTYPE_ALLPC, 0);

	SI32 index = 0;
	SI32 i, kind;

	for(kind = 0; kind < siKindCount; kind++)
	{
		SI32 siKind = MofuMofuEvent_Kind[kind];

		for (i=0; i<siCreate; i++)
		{
			if(m_siCount >= MOFUMOFUEVENT_FIELD_MAXENEMY_COUNT)		break;
			if(index >= MOFUMOFUEVENT_FIELD_MAXENEMY_COUNT-1)		break;
			if(m_siCreateCount >= cltMofuMofuEvent::m_siMaxEnemyCount / MOFUMOFUEVENT_MAXFIELD_COUNT)	break;

			// 레벨 랜덤
			level = (rand() % 50) + 1;

			// 위치 랜덤
			SI32 siRand = rand() % MOFUMOFUEVENT_FIELD_MAXSPWAN_COUNT;
			x = m_siSpwanX[siRand] + (rand() % 2);
			y = m_siSpwanY[siRand] + (rand() % 2);

			SI32 id = pclserver->SetNPCChar(siKind, VILLAGE_NONE, NULL, level, 0, x, y, MAPINDEX_MOFUMOFULAND, NPCITEMMODE_NORMAL, &clinnerstatus );

			if(id > 0)
			{
				while(m_siEnemyID[index] != 0)	{	index++;	}
				m_siEnemyID[index] = id;

				// 모후모후 이벤트 캐릭터 신분 - 모후모후 몬스터
				cltCharServer* pclchar = (cltCharServer*)pclClient->pclCM->CR[id];
				pclchar->clIdentity.MakeMofuMofuEvent(IDENTITY_MOFUMOFUEVENT, m_siFieldIndex, index, 1);

				m_siCount++;
				siRet++;
				m_siCreateCount++;
			}
		}
	}

	return siRet;
}

SI32 cltMofuMofuField::CreateMamaMofuMofu()
{
	SI32	siRet = 0;

	cltServer* pclserver = (cltServer*)pclClient;

	cltInnerStatusInfo clinnerstatus;
	clinnerstatus.clEnemyType.Set(ENEMYTYPE_ALLPC, 0);

	// 위치 랜덤
	SI32 siRand = rand() % MOFUMOFUEVENT_FIELD_MAXSPWAN_COUNT;
	SI32 x = m_siSpwanX[siRand] + (rand() % 2);
	SI32 y = m_siSpwanY[siRand] + (rand() % 2);

	SI32 id = pclserver->SetNPCChar(KIND_MAMARACCOON, VILLAGE_NONE, NULL, 1, 0, x, y, MAPINDEX_MOFUMOFULAND, NPCITEMMODE_NORMAL, &clinnerstatus );
	if(id > 0)
	{
		siRet = 1;
		m_siMamaMofuMofuID = id;

		cltCharServer* pclchar = (cltCharServer*)pclClient->pclCM->CR[id];
		// 모후모후 이벤트 캐릭터 신분 - 마마모후는 특정 인덱스를 사용한다.
		pclchar->clIdentity.MakeMofuMofuEvent(IDENTITY_MOFUMOFUEVENT, m_siFieldIndex, MOFUMOFUEVENT_FIELD_MAMAMOFUMOFU_INDEX, 0);
	}
	
	return siRet;
}

// 특정 인덱스의 몬스터가 죽음
BOOL cltMofuMofuField::KilledEnemy(SI32 siIndex, SI32 siEnemyID)
{
	// 마마모후모후가 죽음
	if(siIndex == MOFUMOFUEVENT_FIELD_MAMAMOFUMOFU_INDEX)
	{
		if(siEnemyID == m_siMamaMofuMofuID)
		{
			m_siMamaMofuMofuID = 0;
		}
		return TRUE;
	}

	if(siIndex < 0 || siIndex >= MOFUMOFUEVENT_FIELD_MAXENEMY_COUNT)	return FALSE;

	if(m_siEnemyID[siIndex] == siEnemyID)
	{
		m_siEnemyID[siIndex] = 0;
		m_siCount--;
		m_siKilledCount++;

		if(CanAttackMaMaMofuMofu())
		{
			// 마마모후를 공격할 수 있다.
			pclClient->pclCM->CR[m_siMamaMofuMofuID]->clIdentity.clMofoMofuEvent.siCanAttack = 1;
			// 마마 모후모후를 공격할 수 있도록 설정한다.
			cltGameMsgResponse_MofuMofuEvent_MaMaMofuMofu clInfo(pclClient->pclCM->CR[m_siMamaMofuMofuID]->GetCharUnique(), 1);
			cltMsg clMsg(GAMEMSG_RESPONSE_MOFUMOFUEVENT_MAMAMOFUMOFU, sizeof(cltGameMsgResponse_MofuMofuEvent_MaMaMofuMofu), (BYTE*)&clInfo);
			pclClient->pclCM->SendAllPersonInMapMsg((sPacketHeader*)&clMsg, MAPINDEX_MOFUMOFULAND);
		}
		return TRUE;
	}
	return FALSE;
}

// 마마모후모후을 공격할 수 있는가?
BOOL cltMofuMofuField::CanAttackMaMaMofuMofu()
{
	if( m_siMamaMofuMofuID > 0 &&
		m_siCreateCount >= cltMofuMofuEvent::m_siMaxEnemyCount / MOFUMOFUEVENT_MAXFIELD_COUNT &&
		m_siKilledCount >= cltMofuMofuEvent::m_siMaxEnemyCount / MOFUMOFUEVENT_MAXFIELD_COUNT)
	{
		return TRUE;
	}

	return FALSE;
}

cltMofuMofuEvent::cltMofuMofuEvent()
{
	m_siMaxEnemyCount = cltMofuMofuEvent::m_siMaxEnemyCount;
	// 1 초에 한번씩 Action
	m_UpdateTimer.Init(1 * 1000);

	// 이벤트 진행 시간
	m_EventTimer.Init(MOFUMOFUEVENT_DEFAULT_TIME_MS);

	// 몬스터 생성 시간
	m_CreateEnemyTimer.Init(MOFUMOFUEVENT_MAKE_ENEMY_TIME_MS);

	// 입장 가능 시간
	m_EnableEnterTimer.Init(MOFUMOFUEVENT_ENABLE_ENTER_TIME_MS);

	Init();
}

cltMofuMofuEvent::~cltMofuMofuEvent()
{
}

// 모후모후 숫자 변경
SI16 cltMofuMofuEvent::m_siMaxEnemyCount = MOFUMOFUEVENT_MAXENEMY_COUNT;

BOOL cltMofuMofuEvent::Init()
{
	cltServer* pclserver = (cltServer*)pclClient;
	SI32 i;

	m_UpdateTimer.SetActive(true, GetTickCount());

	ReInit();

	SYSTEMTIME CurTime;
	GetLocalTime(&CurTime);
	m_siHour = CurTime.wHour;

	SYSTEMTIME	RoomTime;
	ZeroMemory(&RoomTime, sizeof(RoomTime));
	RoomTime.wYear	= CurTime.wYear;
	RoomTime.wMonth = CurTime.wMonth;
	RoomTime.wDay	= CurTime.wDay;
	RoomTime.wHour	= CurTime.wHour;

	for(i=0; i<MOFUMOFUEVENT_ROOMNUM; i++)
	{
		m_clUnit[i].m_ReserveHour.wYear		= RoomTime.wYear;
		m_clUnit[i].m_ReserveHour.wMonth	= RoomTime.wMonth;
		m_clUnit[i].m_ReserveHour.wDay		= RoomTime.wDay;
		m_clUnit[i].m_ReserveHour.wHour		= RoomTime.wHour;
		m_clUnit[i].m_siVillageUnique = 0;

		TimeAddHour(&RoomTime, 1, &RoomTime);
	}

	for(i=0; i<MOFUMOFUEVENT_MAXFIELD_COUNT; i++)
	{
		m_clField[i].Init();
		m_clField[i].m_siFieldIndex = i;
	}

	// 몬스터 생성 위치
	/*m_clField[0].m_siSpwanX[0] = 28;	m_clField[0].m_siSpwanY[0] = 27;
	m_clField[0].m_siSpwanX[1] = 16;	m_clField[0].m_siSpwanY[1] = 32;
	m_clField[0].m_siSpwanX[2] = 25;	m_clField[0].m_siSpwanY[2] = 44;
	m_clField[0].m_siSpwanX[3] = 32;	m_clField[0].m_siSpwanY[3] = 44;
	m_clField[0].m_siSpwanX[4] = 40;	m_clField[0].m_siSpwanY[4] = 44;
	m_clField[0].m_siInPosX = 33;	m_clField[0].m_siInPosY = 34;
	m_clField[0].m_siOutPosX = 39;	m_clField[0].m_siOutPosY = 31;

	m_clField[1].m_siSpwanX[0] = 48;	m_clField[1].m_siSpwanY[0] = 24;
	m_clField[1].m_siSpwanX[1] = 56;	m_clField[1].m_siSpwanY[1] = 26;
	m_clField[1].m_siSpwanX[2] = 55;	m_clField[1].m_siSpwanY[2] = 20;
	m_clField[1].m_siSpwanX[3] = 60;	m_clField[1].m_siSpwanY[3] = 36;
	m_clField[1].m_siSpwanX[4] = 53;	m_clField[1].m_siSpwanY[4] = 25;
	m_clField[1].m_siInPosX = 45;	m_clField[1].m_siInPosY = 24;
	m_clField[1].m_siOutPosX = 38;	m_clField[1].m_siOutPosY = 23;

	m_clField[2].m_siSpwanX[0] = 61;	m_clField[2].m_siSpwanY[0] = 58;
	m_clField[2].m_siSpwanX[1] = 48;	m_clField[2].m_siSpwanY[1] = 65;
	m_clField[2].m_siSpwanX[2] = 43;	m_clField[2].m_siSpwanY[2] = 59;
	m_clField[2].m_siSpwanX[3] = 40;	m_clField[2].m_siSpwanY[3] = 73;
	m_clField[2].m_siSpwanX[4] = 62;	m_clField[2].m_siSpwanY[4] = 52;
	m_clField[2].m_siInPosX = 55;	m_clField[2].m_siInPosY = 51;
	m_clField[2].m_siOutPosX = 50;	m_clField[2].m_siOutPosY = 50;*/

	// 변경된 몬스터 생성 위치.
	m_clField[0].m_siSpwanX[0] = 23;	m_clField[0].m_siSpwanY[0] = 29;
	m_clField[0].m_siSpwanX[1] = 19;	m_clField[0].m_siSpwanY[1] = 33;
	m_clField[0].m_siSpwanX[2] = 24;	m_clField[0].m_siSpwanY[2] = 40;
	m_clField[0].m_siSpwanX[3] = 30;	m_clField[0].m_siSpwanY[3] = 37;
	m_clField[0].m_siSpwanX[4] = 36;	m_clField[0].m_siSpwanY[4] = 43;
	m_clField[0].m_siInPosX = 33;	m_clField[0].m_siInPosY = 34;
	m_clField[0].m_siOutPosX = 39;	m_clField[0].m_siOutPosY = 31;

	m_clField[1].m_siSpwanX[0] = 47;	m_clField[1].m_siSpwanY[0] = 27;
	m_clField[1].m_siSpwanX[1] = 59;	m_clField[1].m_siSpwanY[1] = 26;
	m_clField[1].m_siSpwanX[2] = 50;	m_clField[1].m_siSpwanY[2] = 21;
	m_clField[1].m_siSpwanX[3] = 63;	m_clField[1].m_siSpwanY[3] = 35;
	m_clField[1].m_siSpwanX[4] = 53;	m_clField[1].m_siSpwanY[4] = 27;
	m_clField[1].m_siInPosX = 45;	m_clField[1].m_siInPosY = 24;
	m_clField[1].m_siOutPosX = 38;	m_clField[1].m_siOutPosY = 23;

	m_clField[2].m_siSpwanX[0] = 55;	m_clField[2].m_siSpwanY[0] = 63;
	m_clField[2].m_siSpwanX[1] = 45;	m_clField[2].m_siSpwanY[1] = 62;
	m_clField[2].m_siSpwanX[2] = 40;	m_clField[2].m_siSpwanY[2] = 67;
	m_clField[2].m_siSpwanX[3] = 31;	m_clField[2].m_siSpwanY[3] = 69;
	m_clField[2].m_siSpwanX[4] = 64;	m_clField[2].m_siSpwanY[4] = 54;
	m_clField[2].m_siInPosX = 55;	m_clField[2].m_siInPosY = 51;
	m_clField[2].m_siOutPosX = 50;	m_clField[2].m_siOutPosY = 50;


	return TRUE;
}

BOOL cltMofuMofuEvent::ReInit()
{
	m_siKingMofuMofuID		= 0;

	m_CreateEnemyTimer.SetActive(false, GetTickCount());
	m_EventTimer.SetActive(false, GetTickCount());
	m_EnableEnterTimer.SetActive(false, GetTickCount());

	SI32 i;
	for(i=0; i<MOFUMOFUEVENT_MAXFIELD_COUNT; i++)	m_clField[i].ReInit();

	m_siLastSendMsgMinute	= 0;
	m_siMofuMofuKilledCount	= 0;
	m_siKingMofuMofuKilledCount= 0;
	siVillageUnique			= 0;

	m_bEnterField			= FALSE;
	m_bCreateFirstEnemy		= FALSE;

	return TRUE;
}

BOOL cltMofuMofuEvent::Start(SI16 siRoomIndex, SI32 VillageUnique)
{
	if(siRoomIndex < 0)					return FALSE;
	if(VillageUnique <= 0)				return FALSE;

	if(siRoomIndex == 1000)
	{
		// 운영자가 실행한 이벤트
		VillageUnique = VillageUnique;
	}
	else
	{
		// 정상적인 이벤트
		cltMofuMofuEventReserveUnit* pclRoom = GetRoom(siRoomIndex);
		if(pclRoom == NULL)					return FALSE;

		VillageUnique = pclRoom->m_siVillageUnique;
		if(VillageUnique == 0)			return FALSE;
	}

	m_EventTimer.SetActive(true, GetTickCount());
	m_CreateEnemyTimer.SetActive(true, GetTickCount());
	m_EnableEnterTimer.SetActive(true, GetTickCount());

	dwLeftClock		= MOFUMOFUEVENT_DEFAULT_TIME_MS;	// 이벤트 잔여 시간 - 현 50분
	dwStartClock	= pclClient->CurrentClock;			// 이벤트 시작 시간

	siVillageUnique			= VillageUnique;

	// 모든 유저에게 이벤트 시작을 알린다.
	cltGameMsgResponse_MofuMofuEvent_Start clInfo(siVillageUnique);
	cltMsg clMsg(GAMEMSG_RESPONSE_MOFUMOFUEVENT_START, sizeof(cltGameMsgResponse_MofuMofuEvent_Start), (BYTE*)&clInfo);

	pclClient->pclCM->SendAllPersonMsg((sPacketHeader*)&clMsg);

	if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
	{
		// [로그추가 : 황진성 2007. 10. 17] // 모후모후 이벤트 시작.
		// param1 == 이벤트 잔여 시간, 
		// param2 == 이벤트 시작 시간.
		cltServer * pclserver = (cltServer*)pclClient;
		pclserver->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_MOFUMOFU_EVENT, LOGCOMMAND_INDEX_MOFUMOFU_EVENT_START, 
										0, NULL, NULL, 0, 0, 0, NULL, 0, 0, 0, siVillageUnique, 0, 0, 
										dwLeftClock, dwStartClock, 0, 0, 0, NULL, NULL);
	}



	// 운영자 명령어로 실행하였음.
	if(siRoomIndex == 1000)
	{
		for(SI32 i=0; i<MOFUMOFUEVENT_MAXFIELD_COUNT; i++)
		{
			SI32 siCount = m_clField[i].CreateEnemy();
			siCount = m_clField[i].CreateMamaMofuMofu();
		}
		// 킹 마마모후모후 설정
		SI32 siRand = rand() % MOFUMOFUEVENT_MAXFIELD_COUNT;
		m_siKingMofuMofuID = m_clField[siRand].m_siMamaMofuMofuID;

		m_CreateEnemyTimer.SetActive(false, GetTickCount());
		m_EnableEnterTimer.SetActive(false, GetTickCount());
		m_bEnterField		= TRUE;
		m_bCreateFirstEnemy = TRUE;
	}

	// DB LOG 작성
	cltServer* pclServer = (cltServer*)pclClient;
	pclServer->SendLog_MofuMofuEvent_Start(siVillageUnique);
	
	pclClient->pclLog->FilePrint(TEXT("Config\\MofuMofu.log"), TEXT("[EVENTSTART]\tVillageUnique=%d"), siVillageUnique );

	return TRUE;
}

void cltMofuMofuEvent::End()
{
	cltServer* pclserver = (cltServer*)pclClient;

	SI32 siSuccess = 0;
	SI32 siAddPoint = 100;

	// 모든 모후모후를 잡음 && 마마모후모후 죽음 
	if( m_siMofuMofuKilledCount >= cltMofuMofuEvent::m_siMaxEnemyCount &&
		m_siKingMofuMofuKilledCount >= MOFUMOFUEVENT_MAXKINGENEMY_COUNT )
	{
		pclClient->pclLog->FilePrint(TEXT("Config\\MofuMofu.log"), TEXT("[SUCCESS]") );
		// 마마모후모후를 죽였으므로 보상 지급
		// 1. 국가 점수 제공 - DB에서 처리
		siSuccess	= 1;
		siAddPoint	= 200;

		// 2. 로그인하고 있는 모든 국민에게 천토궁 열쇠 지급
		SI32 index = 0;
		SI32 id;
		while(id = pclClient->pclCM->GetCharOrder(index))
		{
			index++;

			cltCharServer* pclchar  = (cltCharServer*)pclClient->pclCM->CR[id];
			// 사용자인 컴에게만 전달한다. 
			if(pclchar->IsValidConnection() == false)						continue;
			// 해당 마을 주민에게만 준다.
			if(pclchar->pclCI->clBI.siHomeVillage != siVillageUnique)	continue;

			// 천토궁 열쇠 지급
			pclchar->GiveItem(ITEMUNIQUE(7917), 1, INPUTITEMPERSON_OPTION_MOFUMOFUEVENT, 0);
		}

		// 3. 모후모후 이벤트 던젼에 있는 모든 주민에게 보상 지급
		SI32	siVillagerInMap[MAX_RESIDENTS_PER_VILLAGE];
		SI32	siVillagerInMapCount = 0;
		memset(siVillagerInMap, 0, sizeof(siVillagerInMap));

		index = 0;
		while(id = pclClient->pclCM->GetCharOrder(index))
		{
			index++;

			cltCharServer* pclchar  = (cltCharServer*)pclClient->pclCM->CR[id];
			// 사용자인 컴에게만 전달한다. 
			if(pclchar->IsValidConnection() == false)						continue;
			// 해당 마을 주민
			if(pclchar->pclCI->clBI.siHomeVillage != siVillageUnique)	continue;
			// 모후모후 이벤트 맵 안에 있는 캐릭터만
			if(pclchar->GetMapIndex() != MAPINDEX_MOFUMOFULAND)				continue;

			if(siVillagerInMapCount >= MAX_RESIDENTS_PER_VILLAGE)			continue;

			siVillagerInMap[siVillagerInMapCount] = id;
			siVillagerInMapCount++;
		}

		if(siVillagerInMapCount > 0)
		{
			SI32 siRewardGoldCharCount = min(10, siVillagerInMapCount);
			if(siVillagerInMapCount > 10)
			{
				// 섞는다.
				for(int j=0; j<siVillagerInMapCount/2; j++)
				{
					SI32 siIndex1 = rand() % siVillagerInMapCount;
					SI32 siFar = rand() % siVillagerInMapCount / 2;
					SI32 siIndex2 = (siIndex1 + siFar) % siVillagerInMapCount;

					SI32 siTemp;
					siTemp = siVillagerInMap[siIndex1];
					siVillagerInMap[siIndex1] = siVillagerInMap[siIndex2];
					siVillagerInMap[siIndex2] = siTemp;
				}
			}

			// 3-1. 10명까지 랜덤으로 금괴 10개 지급 - 위에서 섞었으므로 처음부터 차례대로 지급해도 랜덤 효과가 나타난다.
			for(SI32 i=0; i<siRewardGoldCharCount; i++)
			{
				SI32 siRewardCharID = siVillagerInMap[i];
				cltCharServer* pclchar  = (cltCharServer*)pclClient->pclCM->CR[siRewardCharID];
				if(pclchar != NULL)
				{
					pclchar->GiveItem(ITEMUNIQUE(5045), 10, INPUTITEMPERSON_OPTION_MOFUMOFUEVENT, 0);
				}
			}

			// 3-2. 1명 랜덤으로 모후모후 7일짜리 모자 지급
			SI32 siRand = rand() % siVillagerInMapCount;
			SI32 siRewardCharID = siVillagerInMap[siRand];
			cltCharServer* pclchar  = (cltCharServer*)pclClient->pclCM->CR[siRewardCharID];
			if(pclchar != NULL)
			{
				pclchar->GiveItem(ITEMUNIQUE(23090), 1, INPUTITEMPERSON_OPTION_MOFUMOFUEVENT, 7);

				pclClient->pclLog->FilePrint(TEXT("Config\\MofuMofu.log"), TEXT("[GETITEM]\tPersonID=%d"), pclchar->pclCI->GetPersonID() );
			}
		}

	}
	else
	{
		pclClient->pclLog->FilePrint(TEXT("Config\\MofuMofu.log"), TEXT("[Fail]\tMonsterCount=%d"), m_siMofuMofuKilledCount );
	}

	// DB에 이벤트 종료를 알린다. - 동시에 필요한 작업을 수행한다.
	/*sDBRequest_MofuMofuEvent_End clDBMsg;
	clDBMsg.siSuccess		= siSuccess;
	clDBMsg.siVillageUnique = siVillageUnique;
	pclserver->SendDBMsg(DBSELECT_SYSTEM, (sPacketHeader *)&clDBMsg);*/
	if(siSuccess == 1)
	{
		sDBRequest_VillageEventSuccess clMsg(siVillageUnique);
		pclserver->SendDBMsg(DBSELECT_SYSTEM, (sPacketHeader *)&clMsg);

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

	// 모후모후 이벤트 맵의 모든 NPC를 지운다.
	pclserver->pclCM->DeleteAllNPCInMapIndex(MAPINDEX_MOFUMOFULAND);

	// 모든 클라이언트에게 이벤트 종료를 알린다.
	cltGameMsgResponse_MofuMofuEvent_End clEndMsg(siVillageUnique);
	cltMsg clMsg(GAMEMSG_RESPONSE_MOFUMOFUEVENT_END, sizeof(cltGameMsgResponse_MofuMofuEvent_End), (BYTE*)&clEndMsg);
	pclClient->pclCM->SendAllPersonMsg((sPacketHeader*)&clMsg);

	pclClient->pclLog->FilePrint(TEXT("Config\\MofuMofu.log"), TEXT("[EVENTEND]") );
	ReInit();
}

SI32 cltMofuMofuEvent::GetCurrentEventVillage()
{
	return siVillageUnique;
}

SI32 cltMofuMofuEvent::GetRoomIndexFromServerTime()
{
	cltServer* pclServer = (cltServer*)pclClient;
	if(pclServer == NULL)									return -1;

	return pclServer->sTime.wHour;
}

BOOL cltMofuMofuEvent::Action()
{
//---------------------------------
// 서버만 컴파일 됨
//---------------------------------
#ifdef _SERVER
//---------------------------------
	// 일정시간이 되어야 수행한다.
	if(m_UpdateTimer.IsTimed(GetTickCount()) == FALSE)		return TRUE;

	cltServer* pclserver = (cltServer*)pclClient;

	SI16 day = (SI16)pclserver->sTime.wDay;
	SI16 hour = (SI16)pclserver->sTime.wHour;
	SI16 minute = (SI16)pclserver->sTime.wMinute;

	if( minute >= 55 && m_siHour == hour)
	{
		SYSTEMTIME	RoomTime;
		ZeroMemory(&RoomTime, sizeof(RoomTime));
		RoomTime.wYear	= pclserver->sTime.wYear;
		RoomTime.wMonth = pclserver->sTime.wMonth;
		RoomTime.wDay	= pclserver->sTime.wDay;
		RoomTime.wHour	= pclserver->sTime.wHour;

		TimeAddHour(&RoomTime, 1, &RoomTime);

		for(SI32 i=0; i<MOFUMOFUEVENT_ROOMNUM; i++)
		{
			m_clUnit[i].m_ReserveHour.wYear		= RoomTime.wYear;
			m_clUnit[i].m_ReserveHour.wMonth	= RoomTime.wMonth;
			m_clUnit[i].m_ReserveHour.wDay		= RoomTime.wDay;
			m_clUnit[i].m_ReserveHour.wHour		= RoomTime.wHour;
			m_clUnit[i].m_siVillageUnique = 0;

			TimeAddHour(&RoomTime, 1, &RoomTime);
		}

		// 다음 시간 기억
		m_siHour = hour + 1;
		m_siHour %= 24;

		SYSTEMTIME	stNextTime;
		TimeAddHour(&pclserver->sTime, 1, &stNextTime);
		// 다음 시간을 전송
		stNextTime.wMinute = 0;
		stNextTime.wSecond = 0;
		stNextTime.wMilliseconds = 0;

		sDBRequest_MofuMofuEvent_ReserveList clMsg( stNextTime );
		pclserver->SendDBMsg(DBSELECT_SYSTEM, (sPacketHeader *)&clMsg);
	}

	cltServer* pclServer = (cltServer*)pclClient;
	if(pclServer == NULL)									return TRUE;

	// 모후모후 졸병들을 다 잡음 & 킹(보스)를 다 잡음
	if( m_siMofuMofuKilledCount >= cltMofuMofuEvent::m_siMaxEnemyCount &&
		m_siKingMofuMofuKilledCount >= MOFUMOFUEVENT_MAXKINGENEMY_COUNT )
	{
		End();
	}

	SI32 siLeftMin = 60 - pclServer->sTime.wMinute;
	if(siLeftMin > 0 && siLeftMin <= MOFUMOFUEVENT_SENDMSG_MIN)
	{
		// %d분 남았습니다. 모후모후 놀이공원 앞으로 모이세요. [자동이용료 1000Q]
		if(m_siLastSendMsgMinute != siLeftMin)
		{
            m_siLastSendMsgMinute = siLeftMin;

			// 클라이언트로 메시지 전송
			SYSTEMTIME stNextHour;
			TimeAddHour(&pclServer->sTime, 1, &stNextHour);
			SI32 siRoomIndex = GetRoomIndex(stNextHour.wYear, stNextHour.wMonth, stNextHour.wDay, stNextHour.wHour);

			cltGate	clGate;
			pclServer->pclMapManager->pclGateManager->FindGate(MAPINDEX_MOFUMOFULAND, MAPINDEX_MAINFIELD, &clGate);

			cltMofuMofuEventReserveUnit* pclRoom = GetRoom(siRoomIndex);
			if(pclRoom != NULL && pclRoom->m_siVillageUnique > 0)
			{
				cltServerResponseValue clreturn(0, SRVAL_MOFUMOFUEVENT_START_LEFTTIME, pclRoom->m_siVillageUnique, siLeftMin);
				cltMsg clMsg(GAMEMSG_RESPONSE_SERVERRTN, sizeof(cltServerResponseValue), (BYTE*)&clreturn);

				SI32 index = 0;
				SI32 id;
				while(id = pclClient->pclCM->GetCharOrder(index))
				{
					index++;

					cltCharServer* pclchar  = (cltCharServer*)pclClient->pclCM->CR[id];
					// 사용자인 컴에게만 전달한다. 
					if(pclchar->IsValidConnection() == false)								continue;
					// 해당 마을 주민에게만 준다.
					if( pclchar->pclCI->clBI.siHomeVillage != pclRoom->m_siVillageUnique )	continue;

					// 플레이어가 모후모후 이벤트 지역에 있다면 메시지를 전송하지 않는다.
					if(pclchar->GetMapIndex() == MAPINDEX_MOFUMOFULAND)						continue;

					// 플레이어가 감옥에 있다면 메시지를 전송하지 않는다.					
					if( pclchar->clInnerStatus.IsStatus(INNERSTATUS_PRISON) == true  )		continue;

					// [영훈] 개인상점 개설시 메시지를 전송하지 않는다
					if ( (true == pclchar->bPersonalShopSwitch) || (true == pclchar->bPremiumPersonalShop) )
					{
						continue;
					}

					// 플레이어가 게이트 주변에 있다면 메시지를 전송하지 않는다.
					SI32 siRange = 10;
					SI32 siPosX, siPosY;
					siPosX = pclchar->GetX();
					siPosY = pclchar->GetY();
					if(abs(siPosX - clGate.DstX) <= siRange && abs(siPosY - clGate.DstY) <= siRange)	continue;

					// 자동 이동 메시지 전송
					pclchar->SendNetMsg((sPacketHeader*)&clMsg);
				}
			}
		}
	}

	// 매시 정각에 이벤트가 시작된다.
	if(pclServer->sTime.wMinute == 0 && pclServer->sTime.wSecond == 0)
	{
		SI32 siRoomIndex = GetRoomIndex(pclServer->sTime.wYear, pclServer->sTime.wMonth, pclServer->sTime.wDay, pclServer->sTime.wHour);
		cltMofuMofuEventReserveUnit* pclUnit = GetRoom(siRoomIndex);

		if( pclUnit != NULL &&
			pclUnit->m_siVillageUnique > 0 )
		{
			// 현재 이벤트하는 마을이 없는 경우 이벤트를 시작한다.
			if( siVillageUnique <= 0 )
			{
				Start(siRoomIndex, pclUnit->m_siVillageUnique);
			}
		}
	}

	if(siVillageUnique > 0)
	{
		// 몬스터 소환 - 4분이 지나면 한번 실행
		if( m_CreateEnemyTimer.IsTimed(GetTickCount(), false) )
		{
			for(SI32 i=0; i<MOFUMOFUEVENT_MAXFIELD_COUNT; i++)
			{
				SI32 siCount = m_clField[i].CreateEnemy();
				siCount = m_clField[i].CreateMamaMofuMofu();
			}
			// 킹 마마모후모후 설정
			SI32 siRand = rand() % MOFUMOFUEVENT_MAXFIELD_COUNT;
			m_siKingMofuMofuID = m_clField[siRand].m_siMamaMofuMofuID;
			m_bCreateFirstEnemy = TRUE;
		}

		// 입장 가능 시간 - 5분이 지나면 입장 가능
		if( m_EnableEnterTimer.IsTimed(GetTickCount(), false) )
		{
			m_bEnterField = TRUE;
		}

		if(m_bCreateFirstEnemy)
		{
			// 몬스터 재 소환
			for(SI32 i=0; i<MOFUMOFUEVENT_MAXFIELD_COUNT; i++)
			{
				m_clField[i].CreateEnemy();
			}
		}

		// 현재 상황 전송 - 이벤트를 진행하는 마을 주민에게만 전송한다.
		SI32 siLeftSecond = 0;

		DWORD dwelapsedclock = pclClient->CurrentClock - dwStartClock;
		if(dwLeftClock > dwelapsedclock)
		{
			siLeftSecond = (dwLeftClock - dwelapsedclock) / 1000 ;
		}
		else
		{
			siLeftSecond = 0;
		}

		cltGameMsgResponse_MofuMofuEvent_Info clInfoMsg(siLeftSecond, m_siKingMofuMofuKilledCount, MOFUMOFUEVENT_MAXKINGENEMY_COUNT, m_siMofuMofuKilledCount, cltMofuMofuEvent::m_siMaxEnemyCount);
		cltMsg clMsg(GAMEMSG_RESPONSE_MOFUMOFUEVENT_INFO, sizeof(cltGameMsgResponse_MofuMofuEvent_Info), (BYTE*)&clInfoMsg);
		pclClient->pclCM->SendAllPersonHomeMsg((sPacketHeader*)&clMsg, siVillageUnique);

		// 이벤트 종료
		if(m_EventTimer.IsTimed(GetTickCount(), false) == TRUE)
		{
			End();
		}
	}
	
//---------------------------------
// 서버만 컴파일 됨
//---------------------------------
#endif //_SERVER
//---------------------------------
	return TRUE;
}

BOOL cltMofuMofuEvent::ReserveRoom(SI16 siYear, SI16 siMonth, SI16 siDay, SI16 siHour, SI32 VillageUnique)
{
	if(siYear < 1000)													return FALSE;
	if(siMonth < 0 || siMonth > 12)										return FALSE;
	if(siDay < 0   || siDay > 31)										return FALSE;
	if(siHour < 0  || siHour > 23)										return FALSE;
	if(VillageUnique <= 0)												return FALSE;

	if(CanReserveRoom(siYear, siMonth, siDay, siHour, VillageUnique) == FALSE)			return FALSE;

	SetRoom(siYear, siMonth, siDay, siHour, VillageUnique);

	return TRUE;
}

BOOL cltMofuMofuEvent::SetRoom(SI16 siYear, SI16 siMonth, SI16 siDay, SI16 siHour, SI32 VillageUnique)
{
	SI32 siRoomIndex= GetRoomIndex(siYear, siMonth, siDay, siHour);
	cltMofuMofuEventReserveUnit* pclUnit = GetRoom(siRoomIndex);

	if(pclUnit == NULL)			return FALSE;

	pclUnit->m_siVillageUnique = VillageUnique;

	return TRUE;
}

BOOL cltMofuMofuEvent::CanReserveRoom(SI16 siYear, SI16 siMonth, SI16 siDay, SI16 siHour, SI32 VillageUnique)
{
	cltServer* pclserver = (cltServer*)pclClient;

	SYSTEMTIME	stWant;
	ZeroMemory(&stWant, sizeof(stWant));

	stWant.wYear = siYear;
	stWant.wMonth = siMonth;
	stWant.wDay = siDay;
	stWant.wHour = siHour;

	// 현재 시간보다 뒤에 시간이어야 한다.
	if( TimeCompare(&pclserver->sTime, &stWant) > 0 )		return FALSE;

	SI16	i;
	for( i = 0; i < MOFUMOFUEVENT_ROOMNUM ; i++ )
	{
		// 해당 시간의 공간에 예약 여부 확인
		 if( m_clUnit[i].m_ReserveHour.wYear == siYear &&
			 m_clUnit[i].m_ReserveHour.wMonth == siMonth &&
			 m_clUnit[i].m_ReserveHour.wDay == siDay &&
			 m_clUnit[i].m_ReserveHour.wHour == siHour )
		 {
			 if( m_clUnit[i].m_siVillageUnique > 0)							return FALSE;
		 }
	}

	for( i = 0; i < MOFUMOFUEVENT_ROOMNUM ; i++ )
	{
		// 예약내역이 있으면 예약 불가
		if( m_clUnit[i].m_siVillageUnique == VillageUnique)
			return FALSE;
	}

	return TRUE;
}

SI32 cltMofuMofuEvent::GetRoomIndex(SI16 siYear, SI16 siMonth, SI16 siDay, SI16 siHour)
{
	for( SI16 i = 0; i < MOFUMOFUEVENT_ROOMNUM ; i++ )
	{
		if( m_clUnit[i].m_ReserveHour.wYear == siYear &&
			m_clUnit[i].m_ReserveHour.wMonth == siMonth &&
			m_clUnit[i].m_ReserveHour.wDay == siDay &&
			m_clUnit[i].m_ReserveHour.wHour == siHour )
		{
			return i;
		}
	}

	return -1;
}

cltMofuMofuEventReserveUnit* cltMofuMofuEvent::GetRoom(SI32 siRoomIndex)
{
	if( siRoomIndex < 0 || siRoomIndex >= MOFUMOFUEVENT_ROOMNUM )		return NULL;
	return & m_clUnit[siRoomIndex];
}

BOOL cltMofuMofuEvent::KilledEnemy(SI16 siFieldIndex, SI16 siIndex, SI32 siEnemeyID)
{
	if(siFieldIndex < 0 || siFieldIndex >= MOFUMOFUEVENT_MAXFIELD_COUNT)	return FALSE;

	if(m_clField[siFieldIndex].KilledEnemy(siIndex, siEnemeyID))
	{
		if(siIndex != MOFUMOFUEVENT_FIELD_MAMAMOFUMOFU_INDEX)
		{
			m_siMofuMofuKilledCount++;
		}
		else
		{
			if(siEnemeyID == m_siKingMofuMofuID)
			{
				m_siKingMofuMofuKilledCount++;
				if(m_siKingMofuMofuKilledCount >= MOFUMOFUEVENT_MAXKINGENEMY_COUNT)		m_siKingMofuMofuKilledCount = MOFUMOFUEVENT_MAXKINGENEMY_COUNT;
			}
		}

		return TRUE;
	}

	return FALSE;
}
