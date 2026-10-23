#include "KoinoboryFamily.h"
#include "CommonLogic.h"
#include "Server.h"
#include "Char\CharCommon\Char-Common.h"
#include "Char\CharManager\CharManager.h"
#include "../CommonLogic/Village/Village.h"
#include "../Common/Event/Event.h"

cltKoinoboryFamilyPosition::cltKoinoboryFamilyPosition( void )
{
	ZeroMemory( this, sizeof(cltKoinoboryFamilyPosition) );
}

cltKoinoboryFamilyPosition::~cltKoinoboryFamilyPosition( void )
{

}

void cltKoinoboryFamilyPosition::Init( void )
{
	ZeroMemory( this, sizeof(cltKoinoboryFamilyPosition) );
}

void cltKoinoboryFamilyPosition::SetPosition( void )
{
	SI32			siPosIndex		= 0;
	SI32			siVillageIndex	= 1;

	cltVillageInfo*	pclVillage		= NULL;

	for ( siPosIndex=0; siPosIndex<MAX_KOINOBORYFAMILY_MAKE_POS; siPosIndex++ )
	{
		if ( MAX_VILLAGE_NUMBER <= siVillageIndex )
		{
			break;
		}

		// 마을 정보에서 유효한 객체를 갖고온다
		for ( SI32 siIndex=siVillageIndex; siIndex<MAX_VILLAGE_NUMBER; siIndex++)
		{
			pclVillage = pclClient->pclVillageManager->pclVillageInfo[siIndex];
			if ( NULL == pclVillage )
			{
				continue;
			}

			// 다음 인덱스를 참조해야 하기 때문에 +1을 한다
			siVillageIndex = siIndex+1;
			break;
		}

		// 한번더 검사해본다
		if ( NULL == pclVillage )
		{
			MsgBox(TEXT("cltKoinoboryFamilyPosition in SetPosition()"),TEXT("VillageInfoReadError"));
			break;
		}

		m_clPos[siPosIndex].SetMapIndex( MAPINDEX_MAINFIELD );
		m_clPos[siPosIndex].SetX( pclVillage->PosX );
		m_clPos[siPosIndex].SetY( pclVillage->PosY );

		// 렌덤 인덱스는 일단 순차적으로 넣어둔다
		m_siRandIndex[siPosIndex] = siPosIndex;
	}
}

void cltKoinoboryFamilyPosition::RandomMixUpIndex( void )
{
	SI32 siRandomIndex	= 0;
	SI32 siSwapIndex	= 0;

	for (SI32 siPosIndex=0; siPosIndex<MAX_KOINOBORYFAMILY_MAKE_POS; siPosIndex++ )
	{
		siRandomIndex = rand() % MAX_KOINOBORYFAMILY_MAKE_POS;

		if ( siPosIndex == siRandomIndex )
		{
			continue;
		}

		siSwapIndex						= m_siRandIndex[siPosIndex];
		m_siRandIndex[siPosIndex]		= m_siRandIndex[siRandomIndex];
		m_siRandIndex[siRandomIndex]	= siSwapIndex;
	}
}

cltCharPos* cltKoinoboryFamilyPosition::GetRandomPosition( const SI32 siIndex )
{
	// 저장은 42개를 하지만 필요한것은 앞에서부터 3개뿐이다 그래서 갖고올수있는 번호를 0부터 2까지로 한것임
	if ( (KOINOBORYFAMILY_KIND_NONE >= siIndex) || (KOINOBORYFAMILY_KIND_MAX <= siIndex) )
	{
		return NULL;
	}

	SI32 siRealIndex = m_siRandIndex[siIndex];
	if ( (0 > siRealIndex) || (MAX_KOINOBORYFAMILY_MAKE_POS <= siRealIndex) )
	{
		return NULL;
	}
	
	return &m_clPos[ siRealIndex ];
}


//------------------------------------------------------------------------------------------------------------------
//
//	cltKoinoboryFamilyManager!!	cltKoinoboryFamilyManager!!	cltKoinoboryFamilyManager!!	cltKoinoboryFamilyManager!!
//
//------------------------------------------------------------------------------------------------------------------
cltKoinoboryFamilyManager::cltKoinoboryFamilyManager()
{
	m_clPos.Init();
	m_clPos.SetPosition();

	ZeroMemory( m_siKonoboryIndex, sizeof(m_siKonoboryIndex) );

	m_siStartHour = -1;
}

cltKoinoboryFamilyManager::~cltKoinoboryFamilyManager()
{

}

void cltKoinoboryFamilyManager::Action()
{
	if ( GAMEMODE_SERVER != pclClient->GameMode )
	{
		return;
	}

	cltServer* pclserver = (cltServer*)pclClient;
	if ( NULL == pclserver )
	{
		return;
	}

	if ( false == pclserver->pclEventTimeManager->InEventTime(TEXT("GoldenWeekEvent"), &pclserver->sTime) )
	{
		return;
	}

	SI16 siNowHour = pclserver->sTime.wHour;

	// 생성된 시간이 같으면 이미 만들어져 있다고 해도된다
	if ( m_siStartHour == siNowHour )
	{
		// 유저들에게 위치를 알려준다
		NoticeKoinoboryFamily();
	}
	else
	{
		// 새롭게 코이노보리를 만든다
		MakeKoinoboryFamily();

		// 유저들에게 위치를 알려준다
		NoticeKoinoboryFamily();

		m_siStartHour = siNowHour;
	}
}

void cltKoinoboryFamilyManager::StartEvent()
{
}

void cltKoinoboryFamilyManager::EndEvent()
{
}

void cltKoinoboryFamilyManager::MakeKoinoboryFamily( void )
{
	// 출몰 위치를 섞어준다
	m_clPos.RandomMixUpIndex();

	// 3마리 생성
	for ( SI32 siCount=0; siCount<KOINOBORYFAMILY_KIND_MAX; siCount++ )
	{
		// 혹시 인덱스에 코이노보리가 존재한다면 그 코이노보리는 삭제한다
		if ( 0 < m_siKonoboryIndex[siCount] )
		{
			DeleteKoinobory( siCount );
		}

		CreateKoinobory( siCount );
	}
}

void cltKoinoboryFamilyManager::NoticeKoinoboryFamily( void )
{
	cltServer* pclserver = (cltServer*)pclClient;
	if ( NULL == pclserver )
	{
		return;
	}

	cltCharServer*	pclchar			= NULL;
	SI32			siCharID		= 0;

	SI32			siVillageUnique	= 0;
	TCHAR*			pszVillageName	= NULL;
	
	for ( SI32 siCount=0; siCount<KOINOBORYFAMILY_KIND_MAX; siCount++ )
	{
		siCharID = m_siKonoboryIndex[siCount];
		if ( siCharID <= 0 )
		{
			continue;
		}

		pclchar = pclserver->pclCM->GetCharServer( siCharID );
		if ( NULL == pclchar )
		{
			m_siKonoboryIndex[siCount] = 0;
			continue;
		}

		if ( pclchar->GetKind() != GetKoinoboryKind(siCount) )
		{
			m_siKonoboryIndex[siCount] = 0;
			continue;
		}

		siVillageUnique = pclserver->pclVillageManager->FindNearVillage( pclchar->GetX(), pclchar->GetY() );
		pszVillageName	= pclserver->pclVillageManager->GetName( siVillageUnique );
		if ( NULL == pszVillageName )
		{
			continue;
		}

		TCHAR*	pText			= GetTxtFromMgr( 7038 );
		TCHAR	szBuffer[256]	= TEXT("");

		StringCchPrintf( szBuffer, 256, pText, pszVillageName  );
		
		// 어디 부근에 있는지 공지로 보낸다
		pclserver->SendNoticeMsg( -1, 0, 0, szBuffer );
	}

}

bool cltKoinoboryFamilyManager::CreateKoinobory( const SI32 siIndex )
{
	if ( (KOINOBORYFAMILY_KIND_NONE >= siIndex) || (KOINOBORYFAMILY_KIND_MAX <= siIndex) )
	{
		return false;
	}

	cltServer* pclserver = (cltServer*)pclClient;
	if ( NULL == pclserver )
	{
		return false;
	}

	cltCharPos* pclPos = m_clPos.GetRandomPosition( siIndex );
	if ( NULL == pclPos )
	{
		return false;
	}

	SI32 siKind		= GetKoinoboryKind( siIndex );
	SI32 siMapIndex = pclPos->GetMapIndex();
	SI32 siX		= pclPos->GetX();
	SI32 siY		= pclPos->GetY();

	SI32 siGetX		= 0;
	SI32 siGetY		= 0;

	cltInnerStatusInfo clinnerstatus;
	clinnerstatus.clEnemyType.Set(ENEMYTYPE_ALLPC, 0);

	cltMapCommon* pclMapCommon = pclClient->pclMapManager->pclMapArray[siMapIndex];
	if ( NULL == pclMapCommon )
	{
		return false;
	}

	// 혹시 모르니 몇번 돌려본다
	for ( SI32 siCount=0; siCount<20; siCount++ )
	{
		if ( false == pclMapCommon->FindEmptyAreaForCharLocate(siKind, siX, siY, 20, &siGetX, &siGetY, true) )
		{
			continue;
		}

		SI32 siID = pclserver->SetNPCChar(siKind, VILLAGE_NONE, NULL, 1, 0, siGetX, siGetY, siMapIndex, NPCITEMMODE_NORMAL, &clinnerstatus );
		if ( pclserver->pclCM->IsValidID(siID) )
		{
			cltCharServer* pclchar = pclserver->pclCM->GetCharServer( siID );
			if ( NULL == pclchar )
			{
				continue;
			}

			pclchar->dwTimerClock		= pclserver->CurrentClock;
			m_siKonoboryIndex[siIndex]	= siID;

			return true;
		}
	}

	return false;
}

bool cltKoinoboryFamilyManager::DeleteKoinobory( const SI32 siIndex )
{
	if ( (KOINOBORYFAMILY_KIND_NONE >= siIndex) || (KOINOBORYFAMILY_KIND_MAX <= siIndex) )
	{
		return false;
	}

	SI32 siCharIndex = m_siKonoboryIndex[siIndex];
	if ( false == pclClient->pclCM->IsValidID(siCharIndex) )
	{
		m_siKonoboryIndex[siIndex] = 0;
		return true;
	}

	cltCharServer* pclchar = pclClient->pclCM->GetCharServer(siCharIndex);
	if ( NULL == pclchar )
	{
		m_siKonoboryIndex[siIndex] = 0;
		return true;
	}

	if ( pclchar->GetKind() != GetKoinoboryKind(siIndex) )
	{
		m_siKonoboryIndex[siIndex] = 0;
		return true;
	}

	pclchar->DeleteInCell();
	if ( TRUE == pclClient->pclCM->DeleteChar(siCharIndex) )
	{
		m_siKonoboryIndex[siIndex] = 0;
		return true;
	}

	return false;

}

SI32 cltKoinoboryFamilyManager::GetKoinoboryKind( const SI32 siIndex )
{
	switch ( siIndex )
	{
	case KOINOBORYFAMILY_KIND_YELLOW:
		{
			return pclClient->GetUniqueFromHash( TEXT("KIND_KOINOBORYYELLOW") );
		}
		break;

	case KOINOBORYFAMILY_KIND_RED:
		{
			return pclClient->GetUniqueFromHash( TEXT("KIND_KOINOBORYRED") );
		}
		break;

	case KOINOBORYFAMILY_KIND_BLUE:
		{
			return pclClient->GetUniqueFromHash( TEXT("KIND_KOINOBORYBLUE")  );
		}
		break;

	}

	return KOINOBORYFAMILY_KIND_NONE;
}

SI32 cltKoinoboryFamilyManager::GetKoinoboryYellowKind()
{
	return pclClient->GetUniqueFromHash( TEXT("KIND_KOINOBORYYELLOW") );
}

SI32 cltKoinoboryFamilyManager::GetKoinoboryRedKind()
{
	return pclClient->GetUniqueFromHash( TEXT("KIND_KOINOBORYRED") );
}

SI32 cltKoinoboryFamilyManager::GetKoinoboryBlueKind()
{
	return pclClient->GetUniqueFromHash( TEXT("KIND_KOINOBORYBLUE")  );
}


/*
void cltKoinoboryFamilyManager::Action()	// 1분에 한번씩 호출해 줘야함
{
	//	if (bStartEvent == false)
	//	{
	//		return;
	//	}

	if( m_siLastCreatedTime < 0 )
	{
		m_siLastCreatedTime = ((cltServer*)pclClient)->sTime.wHour;
		return;
	}

	// 이벤트 시간 체크로 변경
	SYSTEMTIME	stCur;
	GetLocalTime(&stCur);
	if( pclClient->pclEventTimeManager->InEventTime("GoldenWeekEvent", &stCur) == false )
		return;

	SI32 index = 0;
	SI32 id = 0;
	TCHAR buf[256] = TEXT("");

	bool bAliveYellowKoinobory = false;
	bool bAliveRedKoinobory = false;
	bool bAliveBlueKoinobory = false;

	// 1분에 한번씩 공지를 통해 이벤트가 진행중임을 알린다
	// 한시간에 한번만 생성을 체크한다
	if(m_siLastCreatedTime == ((cltServer*)pclClient)->sTime.wHour )
	{ 
		// 코이노보리 가족이 어딘가의 마을 근처에서 떠돌아 다니고있다는 메시지만 보여주고 끝냄.
		// 코이노보리 가족이 전부다 죽었으면 공지 메시지 없이 그냥 끝냄
		index = 0;

		while(id = pclClient->pclCM->GetCharOrder(index))
		{
			if ( bAliveYellowKoinobory && bAliveRedKoinobory && bAliveBlueKoinobory )
				break;

			index++;

			if (id > 0 && pclClient->pclCM->IsValidID(id))
			{
				cltCharServer* pclchar = (cltCharServer*)pclClient->pclCM->CR[id];

				if ( pclchar->GetKind() == GetKoinoboryYellowKind() )
				{	
					bAliveYellowKoinobory = true;		

					// 돌아다니고 있음을 유저들에게 공지
					TCHAR * pText = GetTxtFromMgr(7038);
					const TCHAR * pMapName = pclClient->pclVillageManager->GetName(pclClient->pclVillageManager->FindNearVillage(pclchar->GetX(), pclchar->GetY()));
					TCHAR szBuffer[256];
					szBuffer[0] = 0;
					if( pText != NULL && pMapName != NULL )
					{
						StringCchPrintf( szBuffer , 256 , pText , pMapName );
						((cltServer*)pclClient)->SendNoticeMsg(-1, 0, 0, szBuffer);
					}
				}
				else if ( pclchar->GetKind() == GetKoinoboryBlueKind() )
				{	
					bAliveBlueKoinobory = true;

					// 돌아다니고 있음을 유저들에게 공지
					TCHAR * pText = GetTxtFromMgr(7038);
					const TCHAR * pMapName = pclClient->pclVillageManager->GetName(pclClient->pclVillageManager->FindNearVillage(pclchar->GetX(), pclchar->GetY()));
					TCHAR szBuffer[256];
					szBuffer[0] = 0;
					if( pText != NULL && pMapName != NULL )
					{
						StringCchPrintf( szBuffer , 256 , pText , pMapName );
						((cltServer*)pclClient)->SendNoticeMsg(-1, 0, 0, szBuffer);
					}
				}
				else if ( pclchar->GetKind() == GetKoinoboryRedKind() )
				{	
					bAliveRedKoinobory = true;

					// 돌아다니고 있음을 유저들에게 공지
					TCHAR * pText = GetTxtFromMgr(7038);
					const TCHAR * pMapName = pclClient->pclVillageManager->GetName(pclClient->pclVillageManager->FindNearVillage(pclchar->GetX(), pclchar->GetY()));
					TCHAR szBuffer[256];
					szBuffer[0] = 0;
					if( pText != NULL && pMapName != NULL )
					{
						StringCchPrintf( szBuffer , 256 , pText , pMapName );
						((cltServer*)pclClient)->SendNoticeMsg(-1, 0, 0, szBuffer);
					}
				}
			}
		}

		return;
	}
	else
	{
		// 코이노보리 가족이 죽어서 없어졌다면 새로 생성한다.
		index = 0;

		while(id = pclClient->pclCM->GetCharOrder(index))
		{
			if ( bAliveYellowKoinobory && bAliveRedKoinobory && bAliveBlueKoinobory )
				break;

			index++;

			if (id > 0 && pclClient->pclCM->IsValidID(id))
			{
				cltCharServer* pclchar = (cltCharServer*)pclClient->pclCM->CR[id];

				if ( pclchar->GetKind() == GetKoinoboryYellowKind() )
				{	
					bAliveYellowKoinobory = true;

					StringCchPrintf(buf, 256, TEXT("Current Yellow Mon x : %d, y : %d"), pclchar->GetX(), pclchar->GetY());
					pclClient->pclLog->FilePrint(TEXT("Config\\KoinoboryFamily.log"), buf);

				}
				else if ( pclchar->GetKind() == GetKoinoboryBlueKind() )
				{	
					bAliveBlueKoinobory = true;

					StringCchPrintf(buf, 256, TEXT("Current Blue Mon x : %d, y : %d"), pclchar->GetX(), pclchar->GetY());
					pclClient->pclLog->FilePrint(TEXT("Config\\KoinoboryFamily.log"), buf);
				}
				else if ( pclchar->GetKind() == GetKoinoboryRedKind() )
				{	
					bAliveRedKoinobory = true;

					StringCchPrintf(buf, 256, TEXT("Current Red Mon x : %d, y : %d"), pclchar->GetX(), pclchar->GetY());
					pclClient->pclLog->FilePrint(TEXT("Config\\KoinoboryFamily.log"), buf);
				}

			}
		}


		SI32 index1 = -1, index2 = -1 , index3 = -1;


		// 코이노보리들중 존재하지 않는 녀석은 새로 만들어준다
		if ( bAliveYellowKoinobory != true )
		{

			index1 = rand() % MAX_KOINOBORYFAMILY_MAKE_POS;

			cltServer *pclserver = (cltServer*)pclClient;

			cltInnerStatusInfo clinnerstatus;
			clinnerstatus.clEnemyType.Set(ENEMYTYPE_ALLPC, index1);

			SI32 x = 0, y = 0, mapindex = 0;
			mapindex = clPos[index1].siMapIndex;

			pclClient->pclMapManager->pclMapArray[mapindex]->FindEmptyAreaForCharLocate(GetKoinoboryYellowKind(), clPos[index1].siX, clPos[index1].siY, 20, &x, &y, true);

			pclserver->SetNPCChar(GetKoinoboryYellowKind(), VILLAGE_NONE, NULL, 1, 0, x, 
				y, mapindex, NPCITEMMODE_NORMAL, &clinnerstatus );
			StringCchPrintf(buf, 256, TEXT("Gen Yellow x : %d, y : %d"), x, y);
			pclClient->pclLog->FilePrint(TEXT("Config\\KoinoboryFamily.log"), buf);

			// 생성되었음을 유저들에게 공지
			TCHAR * pText = GetTxtFromMgr(7037);
			const TCHAR * pMapName = pclClient->pclVillageManager->GetName(pclClient->pclVillageManager->FindNearVillage(x, y));
			TCHAR szBuffer[256];
			szBuffer[0] = 0;
			if( pText != NULL && pMapName != NULL )
			{
				StringCchPrintf( szBuffer , 256 , pText ,pMapName );
				((cltServer*)pclClient)->SendNoticeMsg(-1, 0, 0, szBuffer);
			}
		}

		// 코이노보리들중 존재하지 않는 녀석은 새로 만들어준다
		if ( bAliveRedKoinobory != true )
		{
			bool bPass = false;
			SI32 cnt = 0;

			while( bPass != true )
			{
				index2 = rand() % MAX_KOINOBORYFAMILY_MAKE_POS;

				if( index2 != index1 )
					bPass = true;

				if( cnt++ > 1000000)
					return;

			}

			cltServer *pclserver = (cltServer*)pclClient;

			cltInnerStatusInfo clinnerstatus;
			clinnerstatus.clEnemyType.Set(ENEMYTYPE_ALLPC, index2);

			SI32 x = 0, y = 0, mapindex = 0;
			mapindex = clPos[index2].siMapIndex;

			pclClient->pclMapManager->pclMapArray[mapindex]->FindEmptyAreaForCharLocate(GetKoinoboryRedKind(), clPos[index2].siX, clPos[index2].siY, 20, &x, &y, true);

			pclserver->SetNPCChar(GetKoinoboryRedKind(), VILLAGE_NONE, NULL, 1, 0, x, 
				y, mapindex, NPCITEMMODE_NORMAL, &clinnerstatus );
			StringCchPrintf(buf, 256, TEXT("Gen Red x : %d, y : %d"), x, y);
			pclClient->pclLog->FilePrint(TEXT("Config\\KoinoboryFamily.log"), buf);

			// 생성되었음을 유저들에게 공지
			TCHAR * pText = GetTxtFromMgr(7037);
			const TCHAR * pMapName = pclClient->pclVillageManager->GetName(pclClient->pclVillageManager->FindNearVillage(x, y));
			TCHAR szBuffer[256];
			szBuffer[0] = 0;
			if( pText != NULL && pMapName != NULL )
			{
				StringCchPrintf( szBuffer , 256 , pText  , pMapName );
				((cltServer*)pclClient)->SendNoticeMsg(-1, 0, 0, szBuffer);
			}
		}

		// 코이노보리들중 존재하지 않는 녀석은 새로 만들어준다
		if ( bAliveBlueKoinobory != true )
		{
			bool bPass = false;     
			SI32 cnt = 0;

			while( bPass != true )
			{
				index3 = rand() % MAX_KOINOBORYFAMILY_MAKE_POS;

				if( index3 != index1 && index3 != index2 )
					bPass = true;

				if( cnt++ > 1000000)
					return;

			}

			cltServer *pclserver = (cltServer*)pclClient;

			cltInnerStatusInfo clinnerstatus;
			clinnerstatus.clEnemyType.Set(ENEMYTYPE_ALLPC, index3);

			SI32 x = 0, y = 0, mapindex = 0;
			mapindex = clPos[index3].siMapIndex;

			pclClient->pclMapManager->pclMapArray[mapindex]->FindEmptyAreaForCharLocate(GetKoinoboryBlueKind(), clPos[index3].siX, clPos[index3].siY, 20, &x, &y, true);

			pclserver->SetNPCChar(GetKoinoboryBlueKind(), VILLAGE_NONE, NULL, 1, 0, x, 
				y, mapindex, NPCITEMMODE_NORMAL, &clinnerstatus );
			StringCchPrintf(buf, 256, TEXT("Gen Blue x : %d, y : %d"), x, y);
			pclClient->pclLog->FilePrint(TEXT("Config\\KoinoboryFamily.log"), buf);

			// 생성되었음을 유저들에게 공지
			TCHAR * pText = GetTxtFromMgr(7037);
			const TCHAR * pMapName = pclClient->pclVillageManager->GetName(pclClient->pclVillageManager->FindNearVillage(x, y));
			TCHAR szBuffer[256];
			szBuffer[0] = 0;
			if( pText != NULL && pMapName != NULL )
			{
				StringCchPrintf( szBuffer , 256 , pText , pMapName );
				((cltServer*)pclClient)->SendNoticeMsg(-1, 0, 0, szBuffer);
			}
		}

		m_siLastCreatedTime = ((cltServer*)pclClient)->sTime.wHour;
	}
}

void cltKoinoboryFamilyManager::StartEvent()
{
	//	if (bStartEvent)
	//	{
	//		EndEvent();
	//	}

	SI32 index1 = 0 , index2 = 0, index3 = 0;
	bool bPass = false;
	SI32 cnt = 0;

	index1 = rand() % MAX_KOINOBORYFAMILY_MAKE_POS;

	while (!bPass)
	{
		index2 = rand() % MAX_KOINOBORYFAMILY_MAKE_POS;

		if (index1 != index2)
			bPass = true;

		if(cnt++ > 1000000) return;		

	}

	bPass = false;
	cnt = 0;

	while (!bPass)
	{
		index3 = rand() % MAX_KOINOBORYFAMILY_MAKE_POS;

		if (index1 != index3 && index2 != index3)
			bPass = true;

		if(cnt++ > 1000000) return;	
	}

	cltServer *pclserver = (cltServer*)pclClient;

	cltInnerStatusInfo clinnerstatus;
	clinnerstatus.clEnemyType.Set(ENEMYTYPE_ALLPC, index1);

	SI32 x = 0, y = 0, mapindex = 0;

	mapindex = clPos[index1].siMapIndex;

	pclClient->pclMapManager->pclMapArray[mapindex]->FindEmptyAreaForCharLocate(GetKoinoboryYellowKind(), clPos[index1].siX, clPos[index1].siY, 20, &x, &y, true);

	pclserver->SetNPCChar(GetKoinoboryYellowKind(), VILLAGE_NONE, NULL, 1, 0, x, 
		y, mapindex, NPCITEMMODE_NORMAL, &clinnerstatus );

	x = 0; 
	y = 0;
	mapindex = clPos[index2].siMapIndex;

	cltInnerStatusInfo clinnerstatus2;
	clinnerstatus2.clEnemyType.Set(ENEMYTYPE_ALLPC, index2);

	pclClient->pclMapManager->pclMapArray[mapindex]->FindEmptyAreaForCharLocate(GetKoinoboryRedKind(), clPos[index2].siX, clPos[index2].siY, 20, &x, &y, true);

	pclserver->SetNPCChar(GetKoinoboryRedKind(), VILLAGE_NONE, NULL, 1, 0, x, 
		y, mapindex, NPCITEMMODE_NORMAL, &clinnerstatus2 );

	x = 0; 
	y = 0;
	mapindex = clPos[index3].siMapIndex;

	cltInnerStatusInfo clinnerstatus3;
	clinnerstatus3.clEnemyType.Set(ENEMYTYPE_ALLPC, index3);

	pclClient->pclMapManager->pclMapArray[mapindex]->FindEmptyAreaForCharLocate(GetKoinoboryBlueKind(), clPos[index3].siX, clPos[index3].siY, 20, &x, &y, true);

	pclserver->SetNPCChar(GetKoinoboryBlueKind(), VILLAGE_NONE, NULL, 1, 0, x, 
		y, mapindex, NPCITEMMODE_NORMAL, &clinnerstatus3 );

	//	bStartEvent = true;

}

void cltKoinoboryFamilyManager::EndEvent()
{
	SI32 index = 0;
	SI32 id;
	while(id = pclClient->pclCM->GetCharOrder(index))
	{
		index++;

		if (id > 0 && pclClient->pclCM->IsValidID(id))
		{
			cltCharServer* pclchar = (cltCharServer*)pclClient->pclCM->CR[id];

			if (pclchar->GetKind() == GetKoinoboryBlueKind() 
				|| pclchar->GetKind() == GetKoinoboryRedKind()
				|| pclchar->GetKind() == GetKoinoboryYellowKind())
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

	//	bStartEvent = false;

}
*/