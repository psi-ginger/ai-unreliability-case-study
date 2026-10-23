#include "Koinobory.h"
#include "CommonLogic.h"
#include "Server.h"
#include "Char\CharCommon\Char-Common.h"
#include "Char\CharManager\CharManager.h"

#define MAX_MAKE_POS	6

cltKoinoboryManager::cltKoinoboryManager()
{
	if( pclClient->siServiceArea == ConstServiceArea_Japan )
	{
		clPos[0].siMapIndex = 0;
		clPos[0].siX = 175;
		clPos[0].siY = 266;

		clPos[1].siMapIndex = 0;
		clPos[1].siX = 320;
		clPos[1].siY = 212;

		clPos[2].siMapIndex = 0;
		clPos[2].siX = 480;
		clPos[2].siY = 40;

		clPos[3].siMapIndex = 0;
		clPos[3].siX = 780;
		clPos[3].siY = 337;

		clPos[4].siMapIndex = 0;
		clPos[4].siX = 573;
		clPos[4].siY = 625;

		clPos[5].siMapIndex = 0;
		clPos[5].siX = 368;
		clPos[5].siY = 672;
	}
	else
	{
		clPos[0].siMapIndex = 0;
		clPos[0].siX = 320;
		clPos[0].siY = 656;

		clPos[1].siMapIndex = 0;
		clPos[1].siX = 180;
		clPos[1].siY = 160;

		clPos[2].siMapIndex = 0;
		clPos[2].siX = 550;
		clPos[2].siY = 250;

		clPos[3].siMapIndex = 0;
		clPos[3].siX = 535;
		clPos[3].siY = 510;

		clPos[4].siMapIndex = 0;
		clPos[4].siX = 530;
		clPos[4].siY = 510;

		clPos[5].siMapIndex = 0;
		clPos[5].siX = 330;
		clPos[5].siY = 650;
	}

	bStartEvent = false;
}

void cltKoinoboryManager::Action()
{
	if (bStartEvent == false)
	{
		return;
	}

	SI32 index = 0;
	SI32 cnt = 0;
	SI32 id;

	TCHAR buf[256] = TEXT("");

	SI32 siPos[2];
	for (SI32 i = 0; i < 2; i++)
	{
		siPos[i] = -1;
	}
	
	while(id = pclClient->pclCM->GetCharOrder(index))
	{
		if (cnt >= 2)
			break;

		index++;

		if (id > 0 && pclClient->pclCM->IsValidID(id))
		{
			cltCharServer* pclchar = (cltCharServer*)pclClient->pclCM->CR[id];

			if ( pclchar->GetKind() == GetKoinoboryKind() )
			{
				siPos[cnt] = pclchar->clInnerStatus.clEnemyType.siPara1;
				cnt++;

				StringCchPrintf(buf, 256, TEXT("Current Mon x : %d, y : %d"), pclchar->GetX(), pclchar->GetY());
				pclClient->pclLog->FilePrint(TEXT("Config\\Koinobory.log"), buf);

			}

		}
	}

	StringCchPrintf(buf, 256, TEXT("Current cnt : %d"), cnt);
	pclClient->pclLog->FilePrint(TEXT("Config\\Koinobory.log"), buf);

	if (cnt < 2)
	{
		SI32 index1 = -1, index2 = -1;
		SI32 oldindex = -1;

		for (SI32 j = 0; j < 2 - cnt; j++)
		{
			index1 = siPos[j];

			bool bPass = false;
			while (!bPass)
			{
				index2 = rand() % MAX_MAKE_POS;
				if (index1 != index2 && index2 != oldindex )
					bPass = true;
			}

			cltServer *pclserver = (cltServer*)pclClient;

			cltInnerStatusInfo clinnerstatus;
			clinnerstatus.clEnemyType.Set(ENEMYTYPE_ALLPC, index2);

			oldindex = index2;

			SI32 x = 0, y = 0, mapindex = 0;
			mapindex = clPos[index2].siMapIndex;

			pclClient->pclMapManager->pclMapArray[mapindex]->FindEmptyAreaForCharLocate(GetKoinoboryKind(), clPos[index2].siX, clPos[index2].siY, 20, &x, &y, true);

			pclserver->SetNPCChar(GetKoinoboryKind(), VILLAGE_NONE, NULL, 1, 0, x, 
				y, mapindex, NPCITEMMODE_NORMAL, &clinnerstatus );
			StringCchPrintf(buf, 256, TEXT("Gen x : %d, y : %d"), x, y);
			pclClient->pclLog->FilePrint(TEXT("Config\\Koinobory.log"), buf);

		}

	}

}

void cltKoinoboryManager::StartEvent()
{
	if (bStartEvent)
	{
		EndEvent();
	}

	SI32 index1 = 0 , index2 = 0;
	bool bPass = false;

	index1 = rand() % MAX_MAKE_POS;

	while (!bPass)
	{
		index2 = rand() % MAX_MAKE_POS;

		if (index1 != index2)
			bPass = true;
	}

	cltServer *pclserver = (cltServer*)pclClient;

	cltInnerStatusInfo clinnerstatus;
	clinnerstatus.clEnemyType.Set(ENEMYTYPE_ALLPC, index1);

	SI32 x = 0, y = 0, mapindex = 0;

	mapindex = clPos[index1].siMapIndex;

	pclClient->pclMapManager->pclMapArray[mapindex]->FindEmptyAreaForCharLocate(GetKoinoboryKind(), clPos[index1].siX, clPos[index1].siY, 20, &x, &y, true);
	
	pclserver->SetNPCChar(GetKoinoboryKind(), VILLAGE_NONE, NULL, 1, 0, x, 
		y, mapindex, NPCITEMMODE_NORMAL, &clinnerstatus );

	x = 0; 
	y = 0;
	mapindex = clPos[index2].siMapIndex;

	cltInnerStatusInfo clinnerstatus2;
	clinnerstatus2.clEnemyType.Set(ENEMYTYPE_ALLPC, index2);

	pclClient->pclMapManager->pclMapArray[mapindex]->FindEmptyAreaForCharLocate(GetKoinoboryKind(), clPos[index2].siX, clPos[index2].siY, 20, &x, &y, true);

	pclserver->SetNPCChar(GetKoinoboryKind(), VILLAGE_NONE, NULL, 1, 0, x, 
		y, mapindex, NPCITEMMODE_NORMAL, &clinnerstatus2 );

	bStartEvent = true;

}

void cltKoinoboryManager::EndEvent()
{
	SI32 index = 0;
	SI32 id;
	while(id = pclClient->pclCM->GetCharOrder(index))
	{
		index++;

		if (id > 0 && pclClient->pclCM->IsValidID(id))
		{
			cltCharServer* pclchar = (cltCharServer*)pclClient->pclCM->CR[id];

			if (pclchar->GetKind() == GetKoinoboryKind() )
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

	bStartEvent = false;

}
inline SI32	cltKoinoboryManager::GetKoinoboryKind()
{
	if ( pclClient->siServerUnique == ConstServiceArea_Japan )
	{
		return pclClient->GetUniqueFromHash(TEXT("KIND_KOINOBORY"));
	}
	else
	{
		return pclClient->GetUniqueFromHash(TEXT("KIND_KOINOBORYENG"));
	}
}