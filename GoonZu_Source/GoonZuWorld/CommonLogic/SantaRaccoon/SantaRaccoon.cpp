#include "SantaRaccoon.h"
#include "CommonLogic.h"
#include "Server.h"
#include "Char\CharCommon\Char-Common.h"
#include "Char\CharManager\CharManager.h"


cltSantaRaccoonManager::cltSantaRaccoonManager()
{
	clPos[0].siMapIndex = 0;
	clPos[0].siX = 273;
	clPos[0].siY = 361;

	clPos[1].siMapIndex = 0;
	clPos[1].siX = 318;
	clPos[1].siY = 437;

	clPos[2].siMapIndex = 0;
	clPos[2].siX = 331;
	clPos[2].siY = 205;

	clPos[3].siMapIndex = 0;
	clPos[3].siX = 398;
	clPos[3].siY = 565;

	clPos[4].siMapIndex = 0;
	clPos[4].siX = 288;
	clPos[4].siY = 57;

	clPos[5].siMapIndex = 0;
	clPos[5].siX = 540;
	clPos[5].siY = 453;

	clPos[6].siMapIndex = 0;
	clPos[6].siX = 263;
	clPos[6].siY = 600;

	clPos[7].siMapIndex = 0;
	clPos[7].siX = 428;
	clPos[7].siY = 218;

	clPos[8].siMapIndex = 0;
	clPos[8].siX = 442;
	clPos[8].siY = 623;

	clPos[9].siMapIndex = 0;
	clPos[9].siX = 495;
	clPos[9].siY = 41;


	bStartEvent = false;
	m_siLastSantaCreatedTime = -1;
	m_siLastSantaDeletedTime = -1;
/*
#ifdef EVENT_CHRISTMAS

	bStartEvent = true;

#endif
*/
}

void cltSantaRaccoonManager::Action()
{
	if (bStartEvent == false)
	{
		return;
	}


	SI32 index = 0;

	SI32 id = 0;

	TCHAR buf[256] = TEXT("");

/*
	// 생성된 시간의 십분이 지났다면 모두 삭제한다.
	if(m_siLastSantaCreatedTime == ((cltServer*)pclClient)->sTime.wHour )
	{ 
		if( m_siLastSantaDeletedTime != m_siLastSantaCreatedTime
			&& ((cltServer*)pclClient)->sTime.wMinute > 10 )
		{
            // 모든 산타 삭제

			id = 0;
			index = 0;
			while(id = pclClient->pclCM->GetCharOrder(index))
			{
				index++;

				if (id > 0 && pclClient->pclCM->IsValidID(id))
				{
					cltCharServer* pclchar = (cltCharServer*)pclClient->pclCM->CR[id];

					if (pclchar->GetKind() == GetSantaRaccoonKind() )
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


			m_siLastSantaDeletedTime = m_siLastSantaCreatedTime;
		}

		return;
	}

*/


	// 한시간에 한번만 생성을 체크한다
	if(m_siLastSantaCreatedTime == ((cltServer*)pclClient)->sTime.wHour )
	{ 
		return;
	}

	SI32 siPos[2];
	for (SI32 i = 0; i < 2; i++)
	{
		siPos[i] = -1;
	}
	SI32 cnt = 0;
	index = 0;

	while(id = pclClient->pclCM->GetCharOrder(index))
	{
		if (cnt >= 2)
			break;

		index++;

		if (id > 0 && pclClient->pclCM->IsValidID(id))
		{
			cltCharServer* pclchar = (cltCharServer*)pclClient->pclCM->CR[id];

			if ( pclchar->GetKind() == GetSantaRaccoonKind() )
			{
				siPos[cnt] = pclchar->clInnerStatus.clEnemyType.siPara1;
				cnt++;

				StringCchPrintf(buf, 256, TEXT("Current Mon x : %d, y : %d"), pclchar->GetX(), pclchar->GetY());
				pclClient->pclLog->FilePrint(TEXT("Config\\SantaRaccoon.log"), buf);

			}

		}
	}

	StringCchPrintf(buf, 256, TEXT("Current cnt : %d"), cnt);
	pclClient->pclLog->FilePrint(TEXT("Config\\SantaRaccoon.log"), buf);



	// 산타 구리가 2마리미만일때 2마리를 채움
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
				index2 = rand() % MAX_SANTA_RACCOON_MAKE_POS;
				if (index1 != index2 && index2 != oldindex )
					bPass = true;
			}

			cltServer *pclserver = (cltServer*)pclClient;

			cltInnerStatusInfo clinnerstatus;
			clinnerstatus.clEnemyType.Set(ENEMYTYPE_ALLPC, index2);

			oldindex = index2;

			SI32 x = 0, y = 0, mapindex = 0;
			mapindex = clPos[index2].siMapIndex;

			pclClient->pclMapManager->pclMapArray[mapindex]->FindEmptyAreaForCharLocate(GetSantaRaccoonKind(), clPos[index2].siX, clPos[index2].siY, 20, &x, &y, true);

			pclserver->SetNPCChar(GetSantaRaccoonKind(), VILLAGE_NONE, NULL, 1, 0, x, 
				y, mapindex, NPCITEMMODE_NORMAL, &clinnerstatus );
			StringCchPrintf(buf, 256, TEXT("Gen x : %d, y : %d"), x, y);
			pclClient->pclLog->FilePrint(TEXT("Config\\SantaRaccoon.txt"), buf);

		}
	}
	
	m_siLastSantaCreatedTime = ((cltServer*)pclClient)->sTime.wHour;

	// 생성되었음을 유저들에게 공지
	TCHAR* pText = GetTxtFromMgr(6683);

	if( pText == NULL) return;

	((cltServer*)pclClient)->SendNoticeMsg(-1, 0, 0, pText);


}

void cltSantaRaccoonManager::StartEvent()
{
	if (bStartEvent)
	{
		EndEvent();
	}

	SI32 index1 = 0 , index2 = 0;
	bool bPass = false;

	index1 = rand() % MAX_SANTA_RACCOON_MAKE_POS;

	while (!bPass)
	{
		index2 = rand() % MAX_SANTA_RACCOON_MAKE_POS;

		if (index1 != index2)
			bPass = true;
	}

	cltServer *pclserver = (cltServer*)pclClient;

	cltInnerStatusInfo clinnerstatus;
	clinnerstatus.clEnemyType.Set(ENEMYTYPE_ALLPC, index1);

	SI32 x = 0, y = 0, mapindex = 0;

	mapindex = clPos[index1].siMapIndex;

	pclClient->pclMapManager->pclMapArray[mapindex]->FindEmptyAreaForCharLocate(GetSantaRaccoonKind(), clPos[index1].siX, clPos[index1].siY, 20, &x, &y, true);

	pclserver->SetNPCChar(GetSantaRaccoonKind(), VILLAGE_NONE, NULL, 1, 0, x, 
		y, mapindex, NPCITEMMODE_NORMAL, &clinnerstatus );

	x = 0; 
	y = 0;
	mapindex = clPos[index2].siMapIndex;

	cltInnerStatusInfo clinnerstatus2;
	clinnerstatus2.clEnemyType.Set(ENEMYTYPE_ALLPC, index2);

	pclClient->pclMapManager->pclMapArray[mapindex]->FindEmptyAreaForCharLocate(GetSantaRaccoonKind(), clPos[index2].siX, clPos[index2].siY, 20, &x, &y, true);

	pclserver->SetNPCChar(GetSantaRaccoonKind(), VILLAGE_NONE, NULL, 1, 0, x, 
		y, mapindex, NPCITEMMODE_NORMAL, &clinnerstatus2 );

	bStartEvent = true;

}

void cltSantaRaccoonManager::EndEvent()
{
	SI32 index = 0;
	SI32 id;
	while(id = pclClient->pclCM->GetCharOrder(index))
	{
		index++;

		if (id > 0 && pclClient->pclCM->IsValidID(id))
		{
			cltCharServer* pclchar = (cltCharServer*)pclClient->pclCM->CR[id];

			if (pclchar->GetKind() == GetSantaRaccoonKind() )
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
inline SI32	cltSantaRaccoonManager::GetSantaRaccoonKind()
{
		return pclClient->GetUniqueFromHash(TEXT("KIND_SANTARACCOON_ZIQ"));
}