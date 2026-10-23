#include "PVP_LeageMgr.h"


#include "../Common/CommonHeader.h"
#include "../CommonLogic/CommonLogic.h"
#include "../../Server/Server.h"
#include "../../NLib/NUtil.h"

#include "../Char/CharServer/Char-Server.h"
#include "../Char/CharManager/CharManager.h"
#include "../Char/CharCommon/Char-Common.h"

#include "../common/Event/event.h"

#include "../../DBManager/GameDBManager_World/DBMsg-PVP_League.h"
#include "../../DBManager/GameDBManager_World/DBMsg-Item.h"
#include "../../CommonLogic/MsgType-Person.h"
#include "../../CommonLogic/MsgRval-Define.h"
#include "MsgType-System.h"

//========================================================================================
// CPVP_Leage_List
//========================================================================================
CPVP_Leage_List::CPVP_Leage_List()
{
	Init();
}

CPVP_Leage_List::~CPVP_Leage_List()
{
}

void CPVP_Leage_List::Init()
{
	for (SI32 i = 0 ; i<MAX_RESERVE_PERSON; i++) 
	{
		m_clPVP_LeageReserveList[i].InitPVP_LeagueData();
	}
}

SI32 CPVP_Leage_List::Add( SI32 siPersonID, SI16 siGrade )		// 예약리스트에 한사람을 추가한다
{
	if ( 0 >= siPersonID )							return PVP_LEAGUE_APPLY_RESULT_NONE;

	// 이미 등록된 유저이다.
	if ( 0 <= GetPersonIndex(siPersonID) )			return PVP_LEAGUE_APPLY_RESULT_ALREADY;

	// 빈공간 찾기
	SI32 siEmptyPos = -1;
	for(SI32 i=0; i<MAX_RESERVE_PERSON; i++)
	{
		if( m_clPVP_LeageReserveList[i].GetPersonID() <= 0 )
		{
			siEmptyPos = i;
			break;
		}
	}
	// 더이상 공간이 없다.
	if ( siEmptyPos < 0 )							return PVP_LEAGUE_APPLY_RESULT_FULL;

	// 실제 리스트에 등록 시킨다
	m_clPVP_LeageReserveList[siEmptyPos].SetPVP_LeagueData( siPersonID, siGrade, PVP_LEAGUE_RESERVE_STATUS_POSSIBLE );

	// 로그 기록
	return PVP_LEAGUE_APPLY_RESULT_SUCCESS;
}

void CPVP_Leage_List::Delete( SI32 siPersonID )					// 예약리스트에서 특정 사람을 뺀다
{
	if ( 0 >= siPersonID )				return;

	SI32 siIndex = GetPersonIndex( siPersonID );
	if( 0 <= siIndex )
	{
		m_clPVP_LeageReserveList[siIndex].InitPVP_LeagueData();
	}
}

SI32 CPVP_Leage_List::GetPersonIndex( SI32 siPersonID )
{
	if ( 0 >= siPersonID )				return -1;

	for ( SI32 i=0; i< MAX_RESERVE_PERSON; ++i )
	{
		if ( true == m_clPVP_LeageReserveList[i].IsSamePersonID(siPersonID) )
		{
			return i;
		}
	}

	return -1;
}

SI32 CPVP_Leage_List::GetPersonID(SI32 siIndex)
{
	if( siIndex < 0 || siIndex >= MAX_RESERVE_PERSON )		return 0;
	return m_clPVP_LeageReserveList[siIndex].GetPersonID();
}

SI32 CPVP_Leage_List::GetCount()
{
	SI32	siCount = 0;

	for ( SI32 i=0; i< MAX_RESERVE_PERSON; ++i )
	{
		if ( false == m_clPVP_LeageReserveList[i].IsEmpty() )
		{
			siCount++;
		}
	}

	return siCount;
}

void CPVP_Leage_List::ArrangeList()
{
#ifdef _SAFE_MEMORY
	NSafeTArray<CPVP_Leage_ListData, MAX_RESERVE_PERSON>	clPVP_LeageReserveList	;	//	PVP_League	리스트
#else
	CPVP_Leage_ListData				clPVP_LeageReserveList[MAX_RESERVE_PERSON]	;	//	PVP_League	리스트
#endif

	SI32	i;
	// 전체 복사
	for(i=0; i<MAX_RESERVE_PERSON; i++)				{	clPVP_LeageReserveList[i].SetPVP_LeagueData( &m_clPVP_LeageReserveList[i] );	}
	// 전체 삭제
	for (i = 0 ; i<MAX_RESERVE_PERSON; i++)			{	m_clPVP_LeageReserveList[i].InitPVP_LeagueData();								}

	// 데이터를 다시 채움
	SI32	siIndex = 0;
	for(i=0; i<MAX_RESERVE_PERSON; i++)
	{
		if( false == clPVP_LeageReserveList[i].IsEmpty() )
		{
			if( siIndex >= MAX_RESERVE_PERSON )			continue;
			m_clPVP_LeageReserveList[siIndex++].SetPVP_LeagueData( &clPVP_LeageReserveList[i] );
		}
	}
}

void CPVP_Leage_List::DeleteNoPossiblePerson()				// 매칭을 원하지 않는 유저를 삭제한다.
{
	for(SI32 i=0; i<MAX_RESERVE_PERSON; i++)
	{
		if( m_clPVP_LeageReserveList[i].IsEmpty() )			continue;

		// 리스트를 만들 수 없는 상태라면 리스트에서 지워 버린다
		if ( PVP_LEAGUE_RESERVE_STATUS_POSSIBLE != m_clPVP_LeageReserveList[i].GetSatus() )
		{
			m_clPVP_LeageReserveList[i].InitPVP_LeagueData();
		}
	}

	ArrangeList();
}

void CPVP_Leage_List::SetStatus(SI32 siPersonID, SI32 siStatus)
{
	SI32 siIndex = GetPersonIndex( siPersonID );
	if( siIndex >= 0 )
	{
		m_clPVP_LeageReserveList[siIndex].SetSatus( siStatus );
	}
}

SI32 CPVP_Leage_List::FindMatchPerson(SI32 siPersonID)
{
	if( 0 >= siPersonID )					return 0;

	SI32 siIndex = GetPersonIndex( siPersonID );
	if( siIndex < 0 )						return 0;

	SI32 siSourceGrade		= m_clPVP_LeageReserveList[siIndex].GetGrade();
	SI32 siSourcePersonID	= m_clPVP_LeageReserveList[siIndex].GetPersonID();
	SI32 siDestPersonID		= 0;
	SI32 siGradeGab			= PVP_LEAGUE_GRADE_MAX;

	for ( SI32 i=0; i<MAX_RESERVE_PERSON; ++i )
	{
		if ( true == m_clPVP_LeageReserveList[siIndex].IsEmpty() )		continue;		// 비어있음
		if ( m_clPVP_LeageReserveList[i].GetPersonID() == siPersonID)	continue;		// 자기 자신과는 싸울 수 없음

		SI32 siTempPersonID	= m_clPVP_LeageReserveList[i].GetPersonID();
		SI32 siDestGrade	= m_clPVP_LeageReserveList[i].GetGrade();
		SI32 siTempGeb		= siGradeGab;

		// 비교하고자 하는 사람의 등급이 같다면 바로 매칭 리스트로
		if ( siSourceGrade == siDestGrade )
		{
			if ( true == CPVP_LeagueMgr::IsValidPerson(siSourcePersonID, siTempPersonID) )
			{
				siDestPersonID	= siTempPersonID;
				break;
			}
		}
		// 내 등급의 범위 안에 있는 사람인지 검사
		else if ( true == CPVP_LeagueMgr::CheckTheGrade(siSourceGrade, siDestGrade, &siTempGeb) )
		{
			if ( true == CPVP_LeagueMgr::IsValidPerson(siSourcePersonID, siTempPersonID) )
			{
				siDestPersonID	= siTempPersonID;
				siGradeGab		= siTempGeb;
			}
		}
	}

	return siDestPersonID;
}

void CPVP_Leage_List::SendToAgainReserveQuestion( void )
{
	cltServer* pclserver = (cltServer*)pclClient;
	if ( NULL == pclserver )			return;

	for ( SI32 siIndex=0; siIndex<MAX_RESERVE_PERSON; ++siIndex )
	{
		// 퍼슨 아이디가 없다면 빈 리스트
		SI32 siGetPersonID = m_clPVP_LeageReserveList[siIndex].GetPersonID();
		if ( 0 >= siGetPersonID )		continue;

		// 현재 캐릭터 아이디가 유효하지 않다면 그사람은 없는 사람이라고 봐도 무방
		SI32 siGetCharID = pclserver->pclCM->GetIDFromPersonID( siGetPersonID );
		if ( false == pclserver->pclCM->IsValidID(siGetCharID) )
		{
			m_clPVP_LeageReserveList[siIndex].InitPVP_LeagueData();
			continue;
		}

		// 메시지를 전송한 후
		pclserver->SendServerResponseMsg( 0, SRVAL_PVP_LEAGUE_AGAIN_RESERVE, 0, 0, siGetCharID );

		// 상태를 응답 대기상태로 변경한다
		m_clPVP_LeageReserveList[siIndex].SetSatus( PVP_LEAGUE_RESERVE_STATUS_READYFORRESPON );
	}
}

//========================================================================================
// CPVP_Leage_MatchingList
//========================================================================================
CPVP_Leage_MatchingList::CPVP_Leage_MatchingList()
{
	Init();
}

CPVP_Leage_MatchingList::~CPVP_Leage_MatchingList()
{
}

void CPVP_Leage_MatchingList::Init()
{
	for (SI32 i = 0 ; i<MAX_MATCHING_PERSON; i++) 
	{
		m_clPVP_LeageMatchingList[i].InitPVP_Leage_MatchingListData();
	}
}

bool CPVP_Leage_MatchingList::Add( SI32 siSourcePersonID, SI32 siDestPersonID, SI32 siTotalScore )
{
	SI32	siEmptyIndex = -1;
	for(SI32 i=0; i<MAX_MATCHING_PERSON; i++)
	{
		if( m_clPVP_LeageMatchingList[i].IsEmpty() == true )
		{
			siEmptyIndex = i;
			break;
		}
	}

	if( siEmptyIndex < 0 )			return false;

	m_clPVP_LeageMatchingList[siEmptyIndex].SetPVP_Leage_MatchingListData(siSourcePersonID,
		siDestPersonID,
		CPVP_LeagueMgr::GetTotalScore(siSourcePersonID, siDestPersonID));
	return true;
}

SI32 CPVP_Leage_MatchingList::GetIndex( SI32 siSourcePersonID, SI32 siDestPersonID )
{
	SI32	siRetIndex = -1;
	for(SI32 i=0; i<MAX_MATCHING_PERSON; i++)
	{
		if( m_clPVP_LeageMatchingList[i].IsMatch(siSourcePersonID, siDestPersonID) == true )
		{
			siRetIndex = i;
			break;
		}
	}

	return siRetIndex;
}

SI32 CPVP_Leage_MatchingList::GetBattlePersonIDHome( SI32 siIndex )
{
	if( siIndex < 0 || siIndex >= MAX_MATCHING_PERSON )		return 0;
	return m_clPVP_LeageMatchingList[siIndex].GetBattlePersonIDHome();
}

SI32 CPVP_Leage_MatchingList::GetBattlePersonIDAway( SI32 siIndex )
{
	if( siIndex < 0 || siIndex >= MAX_MATCHING_PERSON )		return 0;
	return m_clPVP_LeageMatchingList[siIndex].GetBattlePersonIDAway();
}

SI32 CPVP_Leage_MatchingList::FindPersonID( SI32 siPersonID )
{
	SI32	siRetIndex = -1;
	for(SI32 i=0; i<MAX_MATCHING_PERSON; i++)
	{
		if( m_clPVP_LeageMatchingList[i].GetBattlePersonIDHome() == siPersonID ||
			m_clPVP_LeageMatchingList[i].GetBattlePersonIDAway() == siPersonID )
		{
			siRetIndex = i;
			break;
		}
	}

	return siRetIndex;
}

SI32 CPVP_Leage_MatchingList::FindEnemyPersonID( SI32 siPersonID )
{
	for(SI32 i=0; i<MAX_MATCHING_PERSON; i++)
	{
		if( m_clPVP_LeageMatchingList[i].GetBattlePersonIDHome() == siPersonID )
			return m_clPVP_LeageMatchingList[i].GetBattlePersonIDAway();
		if( m_clPVP_LeageMatchingList[i].GetBattlePersonIDAway() == siPersonID )
			return m_clPVP_LeageMatchingList[i].GetBattlePersonIDHome();
	}

	return 0;
}

bool CPVP_Leage_MatchingList::Delete( SI32 siSourcePersonID, SI32 siDestPersonID )
{
	SI32 siIndex = GetIndex(siSourcePersonID, siDestPersonID);
	if(siIndex < 0)			return false;

	m_clPVP_LeageMatchingList[siIndex].InitPVP_Leage_MatchingListData();

	return true;
}

SI32 CPVP_Leage_MatchingList::GetCount()
{
	SI32 siRetCount = 0;
	for(SI32 i=0; i<MAX_MATCHING_PERSON; i++)
	{
		if( m_clPVP_LeageMatchingList[i].IsEmpty() == false )
		{
			siRetCount++;
			break;
		}
	}
	return siRetCount;
}

bool CPVP_Leage_MatchingList::IsFull()
{
	return GetCount() == MAX_MATCHING_PERSON;
}

int score_compare( const void *arg1, const void *arg2 )
{
	CPVP_Leage_MatchingListData*	pParam1 = (CPVP_Leage_MatchingListData*)arg1;
	CPVP_Leage_MatchingListData*	pParam2 = (CPVP_Leage_MatchingListData*)arg2;

	if( pParam1->GetScore() > pParam2->GetScore() )			return -1;
	else if( pParam1->GetScore() == pParam2->GetScore() )	return 0;
	else													return 1;
}

void CPVP_Leage_MatchingList::SortByTotalScore()
{
	qsort(&m_clPVP_LeageMatchingList[0], MAX_MATCHING_PERSON, sizeof(CPVP_Leage_MatchingListData), score_compare);
}

// 첫번째 매칭리스트의 사람들은 뉴스광고를 한다.
void CPVP_Leage_MatchingList::NoticeSpecialNews()
{
	if ( true == m_clPVP_LeageMatchingList[0].IsEmpty() )		return;

	cltServer* pclserver = (cltServer*)pclClient;
	if ( NULL == pclserver )									return;

	SI32			siHomePersonID	= m_clPVP_LeageMatchingList[0].GetBattlePersonIDHome();
	SI32			siHomeCharID	= pclserver->pclCM->GetIDFromPersonID( siHomePersonID );
	cltCharServer*	pclHomeChar		= pclserver->pclCM->GetCharServer( siHomeCharID );
	if ( NULL == pclHomeChar )									return;

	SI32			siAwayPersonID	= m_clPVP_LeageMatchingList[0].GetBattlePersonIDAway();
	SI32			siAwayCharID	= pclserver->pclCM->GetIDFromPersonID( siAwayPersonID );
	cltCharServer*	pclAwayChar		= pclserver->pclCM->GetCharServer( siAwayCharID );
	if ( NULL == pclAwayChar )									return;

	NTCHARString64	strTitle	= GetTxtFromMgr(9581);
	NTCHARString256 strNews		= GetTxtFromMgr(9633);
	strNews.Replace( TEXT("#user1#"), pclHomeChar->GetName() );
	strNews.Replace( TEXT("#user2#"), pclAwayChar->GetName() );

	pclserver->PushSpecialNews( strNews, strTitle, NULL, CAPTIONKIND_NEWS, CAPTIONKIND_NEWSCHAT );
}

void CPVP_Leage_MatchingList::NoticeStartPVP()
{
	cltServer* pclserver = (cltServer*)pclClient;

	for ( SI32 siIndex=0; siIndex<MAX_MATCHING_PERSON; siIndex++ )
	{
		SI32			siHomePersonID	= m_clPVP_LeageMatchingList[siIndex].GetBattlePersonIDHome();
		SI32			siHomeCharID	= pclserver->pclCM->GetIDFromPersonID( siHomePersonID );
		cltCharServer*	pclHomeChar		= pclserver->pclCM->GetCharServer( siHomeCharID );
		if ( NULL == pclHomeChar )			continue;

		SI32			siAwayPersonID	= m_clPVP_LeageMatchingList[siIndex].GetBattlePersonIDAway();
		SI32			siAwayCharID	= pclserver->pclCM->GetIDFromPersonID( siAwayPersonID );
		cltCharServer*	pclAwayChar		= pclserver->pclCM->GetCharServer( siAwayCharID );
		if ( NULL == pclAwayChar )			continue;

		cltGameMsgResponse_PVP_Request clHome( PVP_LEAGUESTART ,0 , 0, TEXT("") );	
		cltMsg clMsg( GAMEMSG_RESPONSE_PVP_FILED, sizeof(clHome), (BYTE*)&clHome );
		pclHomeChar->SendNeighbourMsg(&clMsg, true);

		cltGameMsgResponse_PVP_Request clAway( PVP_LEAGUESTART ,0 , 0, TEXT("") );	
		cltMsg clMsg2( GAMEMSG_RESPONSE_PVP_FILED, sizeof(clAway), (BYTE*)&clAway );
		pclAwayChar->SendNeighbourMsg(&clMsg2, true);
	}
}

void CPVP_Leage_MatchingList::PVP_League_MatchingLog( void )
{
	cltServer* pclserver = (cltServer*)pclClient	;

	for ( SI32 siSlot = 0; siSlot < MAX_MATCHING_PERSON ; siSlot++)
	{
		// 홈과 어웨이 퍼슨아이디를 가지고 온다.
		SI32 siHomePersonId = m_clPVP_LeageMatchingList[siSlot].GetBattlePersonIDHome()	;
		SI32 siAwayPersonId = m_clPVP_LeageMatchingList[siSlot].GetBattlePersonIDAway()	;
		SI32 siHomeId	=	pclClient->pclCM->GetIDFromPersonID( siHomePersonId )	;
		SI32 siAwayId	=	pclClient->pclCM->GetIDFromPersonID( siAwayPersonId )	;
		if ( pclClient->pclCM->IsValidID( siHomeId ) == false )	return	;
		if ( pclClient->pclCM->IsValidID( siAwayId ) == false )	return	;
		cltCharServer* pclHomeChar = pclClient->pclCM->GetCharServer(siHomeId);			if(pclHomeChar == NULL)			return ;
		cltCharServer* pclAwayChar = pclClient->pclCM->GetCharServer(siAwayId);			if(pclAwayChar == NULL)			return ;

		if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
		{
			// para1 = 매칭된 케릭터 1 para2 상대 케릭터 
			pclserver->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_PERSONALPVP_LEAGUE, LOGCOMMAND_INDEX_PVP_LEAGUE_MATCHING, 
				0,NULL ,NULL,0, 0, 0, NULL, 0, 0, 0, 0, 0, 0, 
				pclHomeChar->pclCI->GetPersonID(), pclAwayChar->pclCI->GetPersonID(), 0, 0, 0, NULL, NULL);
		}

		// 서버에 남긴다.
		TCHAR fileNameBuffer[256];
		//슬롯에 찍을 시간을 텍스트로 완성한다.
		TCHAR szTime[128];
		TCHAR szBuffer0[128];
		SYSTEMTIME stTimeInfo;
		GetLocalTime(&stTimeInfo);
		StringCchPrintf( szTime, sizeof(szTime) ,TEXT("%d:%d:%d:%d:%d"),stTimeInfo.wMonth,stTimeInfo.wDay,stTimeInfo.wHour,stTimeInfo.wMinute,stTimeInfo.wSecond);
		StringCchPrintf(szBuffer0, sizeof(szBuffer0),"%s",&szTime) ;
		StringCchPrintf( fileNameBuffer , sizeof(fileNameBuffer) , TEXT("Config\\PVPLeagueLog_%04d%02d%02d.log") 
			, stTimeInfo.wYear , stTimeInfo.wMonth , stTimeInfo.wDay );

		pclClient->pclLog->FilePrint(TEXT(fileNameBuffer), 
			TEXT(" PVPLEAGUE_MATCHINGLIST \t PERSONNAME : [%s]\t PERSONNAME : [%s]\t Time"),
			pclHomeChar->GetName(),pclAwayChar->GetName(),szBuffer0 );
	}
}


//========================================================================================
// CPVP_Leage_EndGameList
//========================================================================================
CPVP_Leage_EndGameList::CPVP_Leage_EndGameList()
{
	Init();
}

CPVP_Leage_EndGameList::~CPVP_Leage_EndGameList()
{
}

void CPVP_Leage_EndGameList::Init()
{
	for (SI32 i = 0 ; i<MAX_MATCHING_PERSON; i++) 
	{
		m_clPVP_Leage_EndGameList[i].InitPVP_Leage_EndGameListData()	;
	}
}

void CPVP_Leage_EndGameList::Add( SI32 WinnerID, SI32 LoserID )
{
	SI32	siEmptyIndex = -1;

	for(SI32 i=0; i<MAX_MATCHING_PERSON; i++)
	{
		if( m_clPVP_Leage_EndGameList[i].IsEmpty() == true )
		{
			siEmptyIndex = i;
			break;
		}
	}

	if( siEmptyIndex < 0 )			return;

	m_clPVP_Leage_EndGameList[siEmptyIndex].SetBattlePersonIDWin(WinnerID);
	m_clPVP_Leage_EndGameList[siEmptyIndex].SetBattlePersonIDLose(LoserID);
}

SI32 CPVP_Leage_EndGameList::FindPersonID( SI32 siPersonID )
{
	SI32	siRetIndex = -1;
	for(SI32 i=0; i<MAX_MATCHING_PERSON; i++)
	{
		if( m_clPVP_Leage_EndGameList[i].GetBattlePersonIDWin() == siPersonID ||
			m_clPVP_Leage_EndGameList[i].GetBattlePersonIDLose() == siPersonID )
		{
			siRetIndex = i;
			break;
		}
	}

	return siRetIndex;
}

void CPVP_Leage_EndGameList::PVP_LeagueEndWarp()
{
	// 종료 되었음으로 지정된 곳으로 워프 시킨다.
	for ( SI32 siSlot = 0 ; siSlot < MAX_MATCHING_PERSON ; siSlot++ ) 
	{
		// 날려올 사람 선택 
		SI32 siHomePerson = m_clPVP_Leage_EndGameList[ siSlot].GetBattlePersonIDWin()	;
		SI32 siAwayPerson = m_clPVP_Leage_EndGameList[ siSlot].GetBattlePersonIDLose()	;
		if ( siHomePerson <=0 && siAwayPerson <= 0)		continue	;

		cltWarpInfo	clMAinMapWarpInfo (1, MAPINDEX_MAINFIELD, true, false,313 ,341 , -1,  -1)	;
		clMAinMapWarpInfo.uiMapIndex = MAPINDEX_MAINFIELD	;

		cltWarpInfo clinPVPMAPWarpInfo(1, MAPINDEX_PVPMAP, true, false, 49,55, -1,  -1)	;
		clinPVPMAPWarpInfo.uiMapIndex = MAPINDEX_PVPMAP	;

		SI32 Homeid = pclClient->pclCM->GetIDFromPersonID( siHomePerson ) ;
		cltCharServer* pclHomechar = (cltCharServer*)pclClient->pclCM->CR[ Homeid ]	;/*		if (pclHomechar == NULL )	return	;*/
		if ( pclHomechar != NULL) 
		{
			// 만약 현제 매인맵필드에 있다면.HOME
			if(	pclHomechar->GetMapIndex() == MAPINDEX_MAINFIELD	)
			{
				cltMapCommon* pclHomemap = pclClient->pclMapManager->GetMapPointer(pclHomechar->GetMapIndex())	;

				if(pclHomemap != NULL)
				{
					SI32 siColl = pclHomemap->CalcCollInfo( pclHomechar->GetX(), pclHomechar->GetY())	;
					if( siColl & BLOCK_EVENT4 )	// 4번 이벤타 타일을 밟고 있을때.
					{
						pclHomechar->SetWarpInfo(&clMAinMapWarpInfo, 3000);
						pclHomechar->Warp();
						// 만약 소환수가 있으면 그자리로 던져 준다.
						SI32 siHomesummonId = pclHomechar->GetSummonID()	;
						if ( siHomesummonId > 0 ) 
						{
							cltCharServer* pclHomeSummon = (cltCharServer*)pclClient->pclCM->CR[ siHomesummonId ]	;
							if ( pclHomeSummon != NULL ) 
							{
								pclHomeSummon->SetWarpInfo(&clMAinMapWarpInfo, 3000);
								pclHomeSummon->Warp();
							}
						}						
					}
				}
			}
			else if(pclHomechar->GetMapIndex() == MAPINDEX_PVPMAP )
			{
				pclHomechar->SetWarpInfo(&clinPVPMAPWarpInfo, 3000);
				pclHomechar->Warp();
			}
		}

		SI32 Awayid = pclClient->pclCM->GetIDFromPersonID( siAwayPerson ) ;
		cltCharServer* pclAwaychar = (cltCharServer*)pclClient->pclCM->CR[ Awayid ]	;/*		if (pclAwaychar == NULL )	return	;*/
		if ( pclAwaychar != NULL )
		{
			// 만약 매인필드에 있다면 AWAY
			if(	pclAwaychar->GetMapIndex() == MAPINDEX_MAINFIELD	)
			{
				cltMapCommon* pclAwaymap = pclClient->pclMapManager->GetMapPointer(pclAwaychar->GetMapIndex())	;

				if(pclAwaymap != NULL)
				{
					SI32 siColl = pclAwaymap->CalcCollInfo( pclAwaychar->GetX(), pclAwaychar->GetY())	;
					if( siColl & BLOCK_EVENT4 )	// 4번 이벤타 타일을 밟고 있을때.
					{
						pclAwaychar->SetWarpInfo(&clMAinMapWarpInfo, 3000);
						pclAwaychar->Warp();
					}
				}
			}
			else if(pclAwaychar->GetMapIndex() == MAPINDEX_PVPMAP )
			{
				pclAwaychar->SetWarpInfo(&clinPVPMAPWarpInfo, 3000);
				pclAwaychar->Warp();
				// 만약 소환수가 있으면 그자리로 던져 준다.
				SI32 siAwaysummonId = pclAwaychar->GetSummonID()	;
				if ( siAwaysummonId > 0 ) 
				{
					cltCharServer* pclAwaySummon = (cltCharServer*)pclClient->pclCM->CR[ siAwaysummonId ]	;
					if ( pclAwaySummon != NULL ) 
					{
						pclAwaySummon->SetWarpInfo(&clMAinMapWarpInfo, 3000);
						pclAwaySummon->Warp();
					}
				}						
			}
		}
	}
}


//========================================================================================
// CPVP_LeagueMgr
//========================================================================================
// 생성자
CPVP_LeagueMgr::CPVP_LeagueMgr()
{
	ZeroMemory( this, sizeof(CPVP_LeagueMgr) );
}
// 소멸자
CPVP_LeagueMgr::~CPVP_LeagueMgr()
{

}

void CPVP_LeagueMgr::Init( void )
{
	m_clPVP_LeageReserveList.Init();
	m_clPVP_LeageMatchingList.Init();
	m_clPVP_Leage_EndGameList.Init();
}

void CPVP_LeagueMgr::AddEndGameList( SI32 WinnerID, SI32 LoserID )	// 종료한 게임들의 워프를 위해 기록 한다.
{
	// 게임이 끝난 캐릭터의 상태이상을 치료한다.
	SI32 siWinnerCharID = pclClient->pclCM->GetIDFromPersonID( WinnerID );
	SI32 siLoserCharID  = pclClient->pclCM->GetIDFromPersonID( LoserID );

	cltCharServer* pclCharWinner = pclClient->pclCM->GetCharServer( siWinnerCharID );
	if( pclCharWinner )
	{
		pclCharWinner->DeletePoison();					// 중독 상태를 삭제한다.
		pclCharWinner->DeletePoisonSetItemEffect();		// 중독 상태를 삭제한다.
		pclCharWinner->DeleteIce();						// 얼음 상태를 삭제한다.

		pclCharWinner->DeleteCurseAttack();				// 공격력이 저하 상태를 삭제한다.
		pclCharWinner->DeleteCurseCritical();			// Critical 확률이 낮아지는 상태를 삭제한다.
		pclCharWinner->DeleteCurseDefense();			// 방어력이 낮아지는 상태를 삭제한다.
		pclCharWinner->DeleteCurseMovespeed();			// 이동속도 저하 상태를 삭제한다.

		pclCharWinner->DeleteMagicFire();				// 불에 타는 상태를 삭제한다.
		pclCharWinner->DeleteFrozen();					// 보스 얼음 상태를 삭제한다.
	}

	cltCharServer* pclCharLoser = pclClient->pclCM->GetCharServer( siLoserCharID );
	if( pclCharLoser )
	{
		pclCharLoser->DeletePoison();					// 중독 상태를 삭제한다.
		pclCharLoser->DeletePoisonSetItemEffect();		// 중독 상태를 삭제한다.
		pclCharLoser->DeleteIce();						// 얼음 상태를 삭제한다.

		pclCharLoser->DeleteCurseAttack();				// 공격력이 저하 상태를 삭제한다.
		pclCharLoser->DeleteCurseCritical();			// Critical 확률이 낮아지는 상태를 삭제한다.
		pclCharLoser->DeleteCurseDefense();				// 방어력이 낮아지는 상태를 삭제한다.
		pclCharLoser->DeleteCurseMovespeed();			// 이동속도 저하 상태를 삭제한다.

		pclCharLoser->DeleteMagicFire();				// 불에 타는 상태를 삭제한다.
		pclCharLoser->DeleteFrozen();					// 보스 얼음 상태를 삭제한다.
	}

	// 종료된 게임퍼슨을 기록하고
	m_clPVP_Leage_EndGameList.Add( WinnerID, LoserID );

	// 매칭리스트에서 제거한다.
	m_clPVP_LeageMatchingList.Delete( WinnerID, LoserID );
}

SI32 CPVP_LeagueMgr::AddReserveList( SI32 siPersonID, SI16 siGrade )
{
	// 예약정보 기록 
	LogReserve( siPersonID)	;
	return m_clPVP_LeageReserveList.Add( siPersonID, siGrade );
}

void CPVP_LeagueMgr::SubReserveList( SI32 siPersonID )
{
	m_clPVP_LeageReserveList.Delete( siPersonID );
	m_clPVP_LeageReserveList.ArrangeList();
}

void CPVP_LeagueMgr::AgainReserveList( SI32 siPersonID, bool bAnswer )
{
	if( bAnswer )
	{
		m_clPVP_LeageReserveList.SetStatus( siPersonID, PVP_LEAGUE_RESERVE_STATUS_POSSIBLE);
	}
	else
	{
		m_clPVP_LeageReserveList.Delete( siPersonID );
		m_clPVP_LeageReserveList.ArrangeList();
	}
}

void CPVP_LeagueMgr::MakeMatchingList( void )
{
	// 기존에 있던 매칭리스트는 초기화
	m_clPVP_LeageMatchingList.Init();

	// 매칭을 할 수 없는 대상은 모두 지워버린다
	m_clPVP_LeageReserveList.DeleteNoPossiblePerson();

	// 리스트의 인원이 0 이하일때는 해줄필요가 없음
	if( 0 >= m_clPVP_LeageReserveList.GetCount() )			return;

	for( SI32 i=0; i<MAX_RESERVE_PERSON; i++ )
	{
		SI32 siSourcePersonID = m_clPVP_LeageReserveList.GetPersonID( i );
		if( siSourcePersonID <= 0 )										continue;

		SI32 siDestPersonID = m_clPVP_LeageReserveList.FindMatchPerson( siSourcePersonID );

		if( siDestPersonID > 0 )
		{
			SI32 siTotalScore = GetTotalScore( siSourcePersonID, siDestPersonID );
			if ( true == m_clPVP_LeageMatchingList.Add(siSourcePersonID, siDestPersonID, siTotalScore) )
			{			
				// 리스트에 들어간 사람은 신청자 목록에서 지운다
				m_clPVP_LeageReserveList.Delete( siSourcePersonID );
				m_clPVP_LeageReserveList.Delete( siDestPersonID );
			}
		}

		if( m_clPVP_LeageMatchingList.IsFull() )
		{
			break;
		}
	}

	m_clPVP_LeageMatchingList.SortByTotalScore();
}

SI32 CPVP_LeagueMgr::GetRerserveListCount()
{
	return m_clPVP_LeageReserveList.GetCount();
}

bool CPVP_LeagueMgr::CheckTheGrade( SI16 siSourceGrade, SI16 siDestGrade, SI32* psiGradeGab )
{
	SI16 siGradeTable[] = {
		PVP_LEAGUE_GRADE_NONE,
			PVP_LEAGUE_GRADE_9,
			PVP_LEAGUE_GRADE_8,
			PVP_LEAGUE_GRADE_7,
			PVP_LEAGUE_GRADE_6,
			PVP_LEAGUE_GRADE_5,
			PVP_LEAGUE_GRADE_4,
			PVP_LEAGUE_GRADE_3,
			PVP_LEAGUE_GRADE_2,
			PVP_LEAGUE_GRADE_1
	};

	SI32 siGradeTableCount = sizeof(siGradeTable)	/ sizeof(siGradeTable[0]);
	if ( 0 >= siGradeTableCount )
	{
		return false;
	}

	SI32 siSourcePosition	= -1;
	SI32 siDestPosition		= -1;

	for ( SI32 siIndex=0; siIndex<siGradeTableCount; ++siIndex )
	{
		if ( siGradeTable[siIndex] == siSourceGrade )
		{
			siSourcePosition = siIndex;
		}

		if ( siGradeTable[siIndex] == siDestGrade )
		{
			siDestPosition = siIndex;
		}

		if ( (0 <= siSourcePosition) && (0 <= siDestPosition) )
		{
			break;
		}
	}

	// 혹시 등급 포지션 값을 찾지 못했다면
	if ( (0 > siSourcePosition) || (0 > siDestPosition) )
	{
		return false;
	}

	// 등급 차이가 MAX_PVP_LEAGUE_GRADE_GAB 넘으면 안된다
	SI32 siGab = TABS(siSourcePosition - siDestPosition);
	if ( siGab  > MAX_PVP_LEAGUE_GRADE_GAB )
	{
		return false;
	}

	// 3등급 이내에서 기존에 검사했던 등급의 차이보다 적다면 
	if ( siGab < *psiGradeGab )
	{
		*psiGradeGab = siGab;
		return true;
	}

	return false;
}

bool CPVP_LeagueMgr::IsValidPerson( SI32 siSourcePersonID, SI32 siDestPersonID )
{
	cltServer* pclserver = (cltServer*)pclClient;
	if ( NULL == pclserver )		return false;
	//나와 상대방 이이디를 가지고 온다
	SI32 siDestCharID = pclserver->pclCM->GetIDFromPersonID( siDestPersonID );				if ( false == pclserver->pclCM->IsValidID(siDestCharID) )	return false;
	SI32 siSourceCharID = pclserver->pclCM->GetIDFromPersonID( siSourcePersonID );			if ( false == pclserver->pclCM->IsValidID(siSourceCharID) )	return false;

	cltCharServer* pclSourceChar = pclserver->pclCM->GetCharServer( siSourceCharID );		if ( NULL == pclSourceChar )return false;
	cltCharServer* pclDesteChar = pclserver->pclCM->GetCharServer( siDestCharID );			if ( NULL == pclDesteChar )return false;

	// 이미 최근 2경기 상대자라면	
	if ( true == pclSourceChar->IsPVP_LeagueLatestPersonID(siDestPersonID) )		return false;
	else if ( true == pclDesteChar->IsPVP_LeagueLatestPersonID(siSourceCharID) )	return false;

	return true;
}
void CPVP_LeagueMgr::NoticeSpecialNews( void )
{
	m_clPVP_LeageMatchingList.NoticeSpecialNews();
}

void CPVP_LeagueMgr::NoticeStartPVP( void )
{
	m_clPVP_LeageMatchingList.NoticeStartPVP();
}

void CPVP_LeagueMgr::NoticeStartTime( void )
{
	// 리소스 설정 시간을 갖고 온다
	cltEventTime* pclPVPLeagueMatch = pclClient->pclEventTimeManager->GetEvent("PVP_LeagueMatch");
	if ( NULL == pclPVPLeagueMatch )
	{
		return;
	}

	SI32	siStartHour		= pclPVPLeagueMatch->m_StartTime.wHour;
	SI32	siStartMinute	= pclPVPLeagueMatch->m_StartTime.wMinute+5;
	TCHAR	szStartTime[64]	= { '\0', };

	StringCchPrintf( szStartTime, sizeof(szStartTime), TEXT("%02d:%02d"), siStartHour, siStartMinute );

	// 공지로 쓰일 텍스트들을 갖고 온다
	NTCHARString64	strTitle( GetTxtFromMgr(5482) );
	NTCHARString128	strText( GetTxtFromMgr(9728) );

	strText.Replace( TEXT("#time#"), szStartTime );

	cltGameMsgResponse_Notice clNotice( strTitle, strText, true );
	cltMsg clMsg( GAMEMSG_RESPONSE_NOTICE, clNotice.siDataSize, (BYTE*)&clNotice );
	// 접속해있는 모든 사람들에게 알려준다
	pclClient->pclCM->SendAllPersonMsg((sPacketHeader*)&clMsg);	

}
void CPVP_LeagueMgr::NoticeEndTime( void )
{

	// 공지로 쓰일 텍스트들을 갖고 온다
	NTCHARString64	strTitle( GetTxtFromMgr(9581) );
	NTCHARString128	strText( GetTxtFromMgr(9781) );

	cltGameMsgResponse_Notice clNotice( strTitle, strText, true );
	cltMsg clMsg( GAMEMSG_RESPONSE_NOTICE, clNotice.siDataSize, (BYTE*)&clNotice );
	// 접속해있는 모든 사람들에게 알려준다
	pclClient->pclCM->SendAllPersonMsg((sPacketHeader*)&clMsg);	

}

void CPVP_LeagueMgr::Action()
{
	if (pclClient->GameMode != GAMEMODE_SERVER  )				return	;
	cltServer* pclserver = (cltServer*)pclClient;

	SYSTEMTIME stTime	= pclserver->sTime	;
	stTime.wHour	=	pclserver->sTime.wHour		;
	stTime.wMinute	=	pclserver->sTime.wMinute	;
	stTime.wSecond	=	pclserver->sTime.wSecond	;

	if(true ==  pclserver->pclEventTimeManager->InEventTime("PVP_LeagueReserve", &stTime) 
		|| true ==  pclserver->pclEventTimeManager->InEventTime("PVP_LeagueMatch", &stTime) ) // 이벤트 타임일때.
	{
		SI32 si_Battle_Status	= Get_PVP_League_Status()	;
		switch( si_Battle_Status ) 
		{
		case PVP_LEAGUE_PROGRESS_READY:
			{
				if( pclserver->pclEventTimeManager->IsEventStartTiem( "PVP_LeagueReserve", &stTime ) == true )	// 리그 처음 시작 했을때만 보여준다.
				{
					NoticeStartTime()	;	// 리그 시작 시간을 공지 한다.
					bEndNotice	= false	;	// 끝나는 공지 해야 하니까네 
				}				
				Set_PVP_League_Status( PVP_LEAGUE_PROGRESS_BEFORE_1_M )	;// 상태를 셋팅 한다.
			}
			break;
		case PVP_LEAGUE_PROGRESS_BEFORE_1_M://======1분전	매칭 리스트를 만들자.
			{
				if( pclserver->pclEventTimeManager->InEventTime( "PVP_LeagueMatch", &stTime ) == false )	return	;
				if(stTime.wMinute % 5 == 4 )	// 4분일때 리스트를 만든다.
				{
					m_clPVP_Leage_EndGameList.Init();		// 게임 리스트를 초기화.
					MakeMatchingList()		;				// 매칭 리스트를 만든다.

					m_clPVP_LeageReserveList.SendToAgainReserveQuestion();		// 매칭이 안된 사람들한테는 다시 리스트에 있을건지 확인 메시지 보냄					
					m_clPVP_LeageReserveList.ArrangeList();						// 신청자 리스트가 모두 변경되었으니 리스트를 앞으로					
					NoticeSpecialNews();										// 특설링에관한 자막을 날린다

					m_clPVP_LeageMatchingList.PVP_League_MatchingLog();			// 매칭 리스트를 만들고 로그를 남긴다.
					PVP_League_Before1m()	;									// 1분들 남았다는 메세지 날린다.
					Set_PVP_League_Status( PVP_LEAGUE_PROGRESS_BEFORE_10_S )	;
				}
			}
			break;
		case PVP_LEAGUE_PROGRESS_BEFORE_10_S://======10초전이다. 준비 하라는 메세지를 날린다.
			{
				if(stTime.wMinute % 5 == 4	&&	stTime.wSecond >=50 )
				{
					NoticeStartPVP()				;	// 곧 시작 된다는 메세지를 날린다. 
					Set_PVP_League_Status( PVP_LEAGUE_PROGRESS_ONTIME )	;
				}				
			}
			break;
		case PVP_LEAGUE_PROGRESS_ONTIME://======== 5분됐다. 워프 시킨다.
			{
				if(stTime.wMinute % 5 == 0 )
				{
					PVP_LeagueReady()	;
					//m_siEndGameListCount = 0	;	// 엔드 게임 리스트 초기화 

					Set_PVP_League_Status( PVP_LEAGUE_PROGRESS_AFTER_30_S )	;
				}				
			}
			break;
		case PVP_LEAGUE_PROGRESS_AFTER_30_S:// 워프한뒤 약 30초 정도 이후에 시작을 한다.
			{
				if( stTime.wMinute % 5 == 0 && stTime.wSecond >= 30 )
				{
					PVP_LeagueStart( )	;	// PVP리그를 시작 한다.
					Set_PVP_League_Status( PVP_LEAGUE_PROGRESS_END_BATTLE )	;
				}				
			}
			break;
		case PVP_LEAGUE_PROGRESS_END_BATTLE://======게임진행 단계. 3분 전투가 끝나면 종료 시킨다.
			{
				if(stTime.wMinute % 5 == 3	&&	stTime.wSecond >= 30 )
				{
					// 아직도 승부를 못낸 사람들을 비김 처리
					PVP_Battle_Step_Draw()	;
					Set_PVP_League_Status( PVP_LEAGUE_PROGRESS_END_ESCAPEGROUND )	;
				}
				else
				{
					// 게임 진행을 체크한다.
					PVP_Battle_Step_Game()	;
				}
			}
			break;
		case PVP_LEAGUE_PROGRESS_END_ESCAPEGROUND:
			{
				if(stTime.wMinute % 5 == 3	&&	stTime.wSecond >= 30 )
				{
					//끝난시간이 기록 되어 있으면 모든 매칭됐던 케릭터들을 워프 시킨다.(종료 시킨다.)
					m_clPVP_Leage_EndGameList.PVP_LeagueEndWarp();
					Set_PVP_League_Status( PVP_LEAGUE_PROGRESS_READY )	;
				}
			}
			break;
		default:
			{
			}
		}
	}	
	if( true == pclserver->pclEventTimeManager->IsEventEndTiem("PVP_LeagueMatch", &stTime) ) // 이벤트 타임이 끝나는 시간일때
	{
		if ( bEndNotice == false ) 
		{
			SI32 si_Battle_Status	= Get_PVP_League_Status()	;
			NoticeEndTime();// 끝났다고 말해준다.
			bEndNotice	= true	;
			Init();
			Set_PVP_League_Status( PVP_LEAGUE_PROGRESS_READY )	;
		}

	}
}

void CPVP_LeagueMgr::End_PVP_Battle_End( SI32 siWinnerPersonID, SI32 siLoserPersonID)
{
	// End 게임 리스트에 들어있는 캐릭터는 이미 전투가 끝난 캐릭터이다.
	if( InEndGameList( siWinnerPersonID ) == true )	return	;
	if( InEndGameList( siLoserPersonID) == true )	return	;

	// DB로 결과 전송
	sendResultDB( siWinnerPersonID,siLoserPersonID)		;

	// 로그 기록
	LogWinLose( siWinnerPersonID,siLoserPersonID)		;

	// 경기가 끝났으므로 End게임 리스트에 저장한다.
	AddEndGameList(siWinnerPersonID,siLoserPersonID)	;
}

void CPVP_LeagueMgr::End_PVP_Battle_Draw( SI32 siFirstPerson, SI32 siSecondPerson )
{
	sendResultDrawDB( siFirstPerson,siSecondPerson)		;
	AddEndGameList(siFirstPerson,siSecondPerson)	;
}

// 승패가 났을 경우
void CPVP_LeagueMgr::sendResultDB( SI32 siWinnerPersonID, SI32 siLoserPersonID)
{
	cltServer* pclserver = (cltServer*)pclClient;
	if (siWinnerPersonID <= 0 && siLoserPersonID <= 0 )	return	;

	// 디비로 경기 결과를 보낸다. ---------------------------------
	sDBRequest_PVP_League_Battle_End clMsg(  siWinnerPersonID , siLoserPersonID, PVPLEAGUE_RESULT_WIN,pclserver->sTime);
	pclserver->pclGameDBProcess->SendMsg( DBSELECT_SYSTEM, (sPacketHeader *)&clMsg)	;

	sDBRequest_PVP_League_Battle_End clMsg2(  siLoserPersonID , siWinnerPersonID, PVPLEAGUE_RESULT_LOSE,pclserver->sTime);
	pclserver->pclGameDBProcess->SendMsg( DBSELECT_SYSTEM, (sPacketHeader *)&clMsg2)	;
}

// 비겼을 경우
void CPVP_LeagueMgr::sendResultDrawDB( SI32 siFirstPerson, SI32 siSecondPerson)
{
	cltServer* pclserver = (cltServer*)pclClient;
	if (siFirstPerson <= 0  && siSecondPerson <= 0 )	return	;

	// 디비로 경기 결과를 보낸다. ---------------------------------
	sDBRequest_PVP_League_Battle_End clMsg(  siSecondPerson , siFirstPerson, PVPLEAGUE_RESULT_DRAW,pclserver->sTime);
	pclserver->pclGameDBProcess->SendMsg( DBSELECT_SYSTEM, (sPacketHeader *)&clMsg);

	sDBRequest_PVP_League_Battle_End clMsg2(  siFirstPerson ,siSecondPerson , PVPLEAGUE_RESULT_DRAW,pclserver->sTime);
	pclserver->pclGameDBProcess->SendMsg( DBSELECT_SYSTEM, (sPacketHeader *)&clMsg2);
}

// 승패를 각각 클라이언트로 알려주자.
void CPVP_LeagueMgr::Send_PVP_Battle_Result( SI32 siWinPerson, SI32 siLosePerson, SI32 siFlag )
{
	SI32 siWinID = pclClient->pclCM->GetIDFromPersonID( siWinPerson );
	cltCharServer* pclWinChar = NULL;
	if( pclClient->pclCM->IsValidID( siWinID ) )	pclWinChar = pclClient->pclCM->GetCharServer( siWinID );

	SI32 siLoseID = pclClient->pclCM->GetIDFromPersonID( siLosePerson );
	cltCharServer* pclLoseChar = NULL;
	if( pclClient->pclCM->IsValidID( siLoseID ) )	pclLoseChar = pclClient->pclCM->GetCharServer( siLoseID );

	switch( siFlag )
	{
	case PVPLEAGUE_RESULT_WIN:
		{
			// 캐릭터가 정상적으로 게임에 있는 경우에만 처리한다.
			if( pclWinChar )
			{
				SetAfterPVP_League_Record_Win( siWinID ); // 승리한 사람의 기록에 대한 처리.

				pclWinChar->siEnemUnique	=	0	;	// 이겼으니까 적 케릭터 초기화.
				pclWinChar->SetPVP_LeagueEnemyPersonID(0)	;

				pclWinChar->IncQuestPara( QUEST_CLEAR_COND_PVPLEAGUEWIN, 0, 1 );

				// 승리자
				cltGameMsgResponse_PVP_Request clWinner( END_PVP , WIN_PVP, pclWinChar->GetCharUnique(), TEXT("") );	
				cltMsg clMsg( GAMEMSG_RESPONSE_PVP_FILED, sizeof(clWinner), (BYTE*)&clWinner );
				pclWinChar->SendNeighbourMsg(&clMsg, true);
			}
		}
		break;
	case PVPLEAGUE_RESULT_LOSE:
		{
			// 캐릭터가 정상적으로 게임에 있는 경우에만 처리한다.
			if( pclLoseChar )
			{
				SetAfterPVP_League_Record_Lose( siLoseID ); // 패배한 사람의 기록에 대한 처리.

				// PVP리그 위치에 있는 캐릭터만 제자리 부활을 시켜준다.
				if( Is_MatchingListPeron_TruePosition( siLosePerson ) )
				{		pclLoseChar->siEnemUnique	=	-100	;		}
				else
				{		pclLoseChar->siEnemUnique	=	0	;			}

				pclLoseChar->SetPVP_LeagueEnemyPersonID(0)	;

				// 패배자
				cltGameMsgResponse_PVP_Request clclLoser( END_PVP , LOSE_PVP, pclLoseChar->GetCharUnique(), TEXT("") );
				cltMsg clMsg2( GAMEMSG_RESPONSE_PVP_FILED, sizeof(clclLoser), (BYTE*)&clclLoser );
				pclLoseChar->SendNeighbourMsg(&clMsg2, true);
			}
		}
		break;
	case PVPLEAGUE_RESULT_DRAW:
		{
			// 캐릭터가 정상적으로 게임에 있는 경우에만 처리한다.
			if( pclWinChar )
			{
				SetAfterPVP_League_Record_Draw( siWinID )	;

				pclWinChar->siEnemUnique	=	0	;
				pclWinChar->SetPVP_LeagueEnemyPersonID(0)	;

				cltGameMsgResponse_PVP_Request clWinner( END_PVP , DRAW_PVP, pclWinChar->GetCharUnique(), TEXT("") );	
				cltMsg clMsg( GAMEMSG_RESPONSE_PVP_FILED, sizeof(clWinner), (BYTE*)&clWinner );
				pclWinChar->SendNeighbourMsg(&clMsg, true);
			}
		}
		break;
	}
}

void CPVP_LeagueMgr::SetAfterPVP_League_Record_Win( SI32 siID )
{
	if ( pclClient->pclCM->IsValidID( siID ) == false )		return	;
	cltCharCommon* pclWinCharCommon = pclClient->pclCM->CR[siID]		;

	pclWinCharCommon->SetPVP_LeagueEnemyPersonID(0)	;
	// 승리를 제외한 나머지 연속 기록들 0
	//------------------------------------------------------
	pclWinCharCommon->SetPVP_LeagueStraight_Draw( 0 )	;
	//------------------------------------------------------
	pclWinCharCommon->SetPVP_LeagueStraight_Lose( 0 )	;
	pclWinCharCommon->SetPVP_LeagueStraight_Lose_Grade( 0 )	;
	//------------------------------------------------------
	// 연승기록을 가지고 온다
	SI16 siStraight_Win	=	pclWinCharCommon->GetPVP_LeagueStraight_Win()	;
	pclWinCharCommon->SetPVP_LeagueStraight_Win( siStraight_Win + 1)	;
	SI16 siWinCount		=	pclWinCharCommon->GetPVP_LeagueStraight_Win()	;

	// 등급을 위한 상승 기록 추가
	SI32 siWinCountGradeNow = pclWinCharCommon->GetPVP_LeagueStraight_Win_Grade()	;
	pclWinCharCommon->SetPVP_LeagueStraight_Win_Grade( siWinCountGradeNow+1 )	;
	SI32 siWinCountGrade = pclWinCharCommon->GetPVP_LeagueStraight_Win_Grade()	;

	//경기 후 상품을 준다	
	AfterPVP_League_Record_Win_Prize(siID ,siWinCount)	;
	// 등급 조정
	PVP_League_LevelUp(siID, siWinCountGrade )	;
}

void CPVP_LeagueMgr::SetAfterPVP_League_Record_Draw( SI32 siID )
{
	SI32 siDrawId = siID	;
	if ( pclClient->pclCM->IsValidID( siDrawId ) == false )		return	;
	cltCharCommon* pclDrawCharCommon = pclClient->pclCM->CR[siDrawId]		;

	// PVP가 끝났으니 상대 정보를 삭제.
	pclDrawCharCommon->SetPVP_LeagueEnemyPersonID(0)	;
	// 무승부가 났으니 승리 기록들은 사라진다.
	//------------------------------------------------------
	pclDrawCharCommon->SetPVP_LeagueStraight_Win( 0 )	;
	pclDrawCharCommon->SetPVP_LeagueStraight_Lose( 0 )	;
	//------------------------------------------------------
	pclDrawCharCommon->SetPVP_LeagueStraight_Win_Grade( 0 )	;
	pclDrawCharCommon->SetPVP_LeagueStraight_Lose( 0 )	;
	//------------------------------------------------------
	// 연속 무승부 기록 1 증가 
	SI32 siDrawRecord = pclDrawCharCommon->GetPVP_LeagueStraight_Draw( )	;
	pclDrawCharCommon->SetPVP_LeagueStraight_Draw( siDrawRecord + 1)	;	
	SI32 siStright_Draw = pclDrawCharCommon->GetPVP_LeagueStraight_Draw()		;
	//경기 후 상품을 준다	
	AfterPVP_League_Record_Draw_Prize(siDrawId ,siStright_Draw)	;
}

void CPVP_LeagueMgr::SetAfterPVP_League_Record_Lose( SI32 siID )
{
	SI32 siLoserId = siID	;
	if ( pclClient->pclCM->IsValidID( siLoserId ) == false )		return	;
	SI32 siLoserChaeParentID = pclClient->pclCM->CR[ siLoserId ]->GetSummonParentID()	;
	if ( siLoserChaeParentID == 0 )	siLoserChaeParentID = siLoserId
		;
	cltCharCommon* pclLoseCharCommon = pclClient->pclCM->CR[siLoserChaeParentID]		;

	pclLoseCharCommon->SetPVP_LeagueEnemyPersonID(0)	;
	// 패배했으니 연속 기록들 모두 삭제.
	//------------------------------------------------------
	pclLoseCharCommon->SetPVP_LeagueStraight_Win( 0 )	;
	pclLoseCharCommon->SetPVP_LeagueStraight_Win_Grade( 0 )	;
	pclLoseCharCommon->SetPVP_LeagueStraight_Draw( 0 )	;
	//------------------------------------------------------

	// 연속 패배 기록에 1을 추가
	SI32 siLoseRecord = pclLoseCharCommon->GetPVP_LeagueStraight_Lose( )		;
	pclLoseCharCommon->SetPVP_LeagueStraight_Lose( siLoseRecord + 1)			;	
	SI32 siStright_Lose = pclLoseCharCommon->GetPVP_LeagueStraight_Lose()	;

	// 등급을 위한 패배 기록 추가
	SI32 siLoseCountGradeNow = pclLoseCharCommon->GetPVP_LeagueStraight_Lose_Grade()	;
	pclLoseCharCommon->SetPVP_LeagueStraight_Lose_Grade( siLoseCountGradeNow+1 )	;
	SI32 siLoseCountGrade = pclLoseCharCommon->GetPVP_LeagueStraight_Lose_Grade()	;
	//경기 후 상품을 준다	
	AfterPVP_League_Record_Lose_Prize( siLoserId , siStright_Lose )		;
	// 등급 조정
	PVP_League_LevelDown(siLoserId, siLoseCountGrade );

}

void CPVP_LeagueMgr::AfterPVP_League_Record_Win_Prize( SI32 siID, SI32 siStrightWinCount) // 퍼슨아이디 아닙니다 유니크 받습니다.
{
	cltServer* pclserver = (cltServer*)pclClient	;

	SI32 siId = siID	;
	if ( pclClient->pclCM->IsValidID( siID ) == false )		return	;
	cltCharServer* pclChar = pclClient->pclCM->GetCharServer(siId);			if(pclChar == NULL)			return ;

	//연속기록에 대한 보상
	SI32 strightdraw100 = pclClient->GetGlobalValue("GV_straight_win100")	;
	SI32 strightdraw1000 = pclClient->GetGlobalValue("GV_straight_win1000")	;

	SI32 strightdraw100Unique = pclClient->GetGlobalValue("GV_straight_win100Unique")	;
	SI32 strightdraw1000Unique = pclClient->GetGlobalValue("GV_straight_win1000Unique")	;

	SI32 siRewardItemUnique	=	pclClient->GetGlobalValue("GV_PVP_League_Win_Unique")	;
	PVP_League_GiveItem(siId, siRewardItemUnique , false );

	if ( siStrightWinCount % strightdraw1000	==	0	) 
	{
		// 1000연승 메세지를 서버로 날린다.
		// 아이템을 준다
		LogGoal( pclChar->pclCI->GetPersonID(),PVP_LEAGUE_END_RESULT_1000WIN  )	;
		PVP_League_GiveItem(siId, strightdraw1000Unique,true );
		PVP_Battle_HaveItemMessage( siId, strightdraw1000Unique );
	}

	else if ( siStrightWinCount 	%	strightdraw100  == 0	) 
	{
		// 100연승 메세지를 서버로 날린다.
		// 아이템을 준다.
		LogGoal( pclChar->pclCI->GetPersonID(),PVP_LEAGUE_END_RESULT_100WIN )	;
		PVP_League_GiveItem(siId, strightdraw100Unique, true );
		PVP_Battle_HaveItemMessage( siId, strightdraw100Unique);
	}

}

void CPVP_LeagueMgr::AfterPVP_League_Record_Draw_Prize( SI32 siID , SI32 siStrightWinCount)
{
	SI32 siId = siID	;
	if ( pclClient->pclCM->IsValidID( siId ) == false )		return	;
	cltCharServer* pclChar = pclClient->pclCM->GetCharServer(siId);			if(pclChar == NULL)			return ;

	//연속기록에 대한 보상
	SI32 strightdraw100 = pclClient->GetGlobalValue("GV_straight_Draw100")		;
	SI32 strightdraw1000 = pclClient->GetGlobalValue("GV_straight_draw1000")	;

	SI32 strightdraw100Unique = pclClient->GetGlobalValue("GV_straight_Draw100Unique")		;
	SI32 strightdraw1000Unique = pclClient->GetGlobalValue("GV_straight_draw1000Unique")	;

	SI32 siRewardItemUnique	=	pclClient->GetGlobalValue("GV_PVP_League_Draw_Unique")	;

	PVP_League_GiveItem(siId,siRewardItemUnique, true)	;
	if ( siStrightWinCount % strightdraw1000 == 0 ) 
	{
		// 1000경기 연속 무승부임을 알린다.
		// 상품을 준다
		LogGoal( pclChar->pclCI->GetPersonID(),PVP_LEAGUE_END_RESULT_1000DRAW  )	;
		PVP_League_GiveItem(siId, strightdraw1000Unique,true )	;
		PVP_Battle_HaveItemMessage( siId, strightdraw1000Unique );
	}	
	else if ( siStrightWinCount % strightdraw100 == 0 ) 
	{
		// 100경기 연속 무승부임을 알린다.
		// 상품을 준다
		LogGoal( pclChar->pclCI->GetPersonID(),PVP_LEAGUE_END_RESULT_100DRAW  )	;
		PVP_League_GiveItem(siId, strightdraw100Unique, true )	;
		PVP_Battle_HaveItemMessage( siId, strightdraw100Unique );
	}

}

void CPVP_LeagueMgr::AfterPVP_League_Record_Lose_Prize( SI32 siID , SI32 siStrightLoseCount)
{
	cltServer* pclserver = (cltServer*)pclClient	;

	SI32 siId = siID;
	if ( pclClient->pclCM->IsValidID( siId ) == false )		return	;
	SI32 siParentID = pclClient->pclCM->CR[ siId ]->GetSummonParentID()		;
	if ( siParentID == 0 )	siParentID = siId		;
	cltCharServer* pclChar = pclClient->pclCM->GetCharServer(siParentID);			if(pclChar == NULL)			return ;

	SI32 siRewardItemUnique	=	pclClient->GetGlobalValue("GV_PVP_League_Draw_Unique")	;

	// 패배 보상을 한다.
	PVP_League_GiveItem(siParentID, siRewardItemUnique , false )	;
}

//--등급
void CPVP_LeagueMgr::PVP_League_LevelUp( SI32 siID ,SI32 siStrightWinCountGrade )
{
	cltServer* pclserver = (cltServer*)pclClient	;

	SI32 siId = siID	;
	if ( pclClient->pclCM->IsValidID( siID ) == false )		return	;
	cltCharServer* pclChar = pclClient->pclCM->GetCharServer(siId);			if(pclChar == NULL)			return ;
	SI32 siGradeNow	=	pclChar->GetPVP_LeagueGrade()	;
	SI32 ChangedGrade = ChangeGrade(siGradeNow,siStrightWinCountGrade,PVP_LEAGUE_END_GRADEUP )	;

	if(pclChar->GetPVP_LeagueGrade() != ChangedGrade )
	{
		pclChar->SetPVP_LeagueStraight_Win_Grade( 0 )	;
		LogGradeUpDown(pclChar->pclCI->GetPersonID() ,siGradeNow, ChangedGrade,LOGCOMMAND_INDEX_PVP_LEAGUE_GRADE_UP );

		sDBRequest_PVP_League_Battle_ChangeGrade clMsg(  pclChar->pclCI->GetPersonID() ,  ChangedGrade ,pclserver->sTime );
		pclserver->pclGameDBProcess->SendMsg( DBSELECT_SYSTEM, (sPacketHeader *)&clMsg);
	}
	//// 5등급 이상 3연승 했을 경우에 케릭터의 등급을 올려준다.
	//if( siGradeNow > PVP_LEAGUE_GRADE_6  && siStrightWinCountGrade != 0 && siStrightWinCountGrade % 3 == 0 )
	//{
	//	SI32 ChangedGrade = siGradeNow + MAX_PVP_LEAGUE_GRADE_UP_DOWN_POINT	;
	//	if ( ChangedGrade > PVP_LEAGUE_GRADE_1 ) 
	//	{
	//		ChangedGrade = PVP_LEAGUE_GRADE_1	;
	//	}
	//	pclChar->SetPVP_LeagueStraight_Win_Grade( 0 )	;
	//	LogGradeUpDown(pclChar->pclCI->GetPersonID() ,siGradeNow, ChangedGrade,LOGCOMMAND_INDEX_PVP_LEAGUE_GRADE_UP );
	//	sDBRequest_PVP_League_Battle_ChangeGrade clMsg(  pclChar->pclCI->GetPersonID() ,  ChangedGrade );
	//	pclserver->pclGameDBProcess->SendMsg( DBSELECT_SYSTEM, (sPacketHeader *)&clMsg);
	//}
	//// 5등급 미만 2연승 했을 경우 케릭터의 등급을 올려준다
	//else if(siGradeNow <= PVP_LEAGUE_GRADE_6 && siStrightWinCountGrade != 0  && siStrightWinCountGrade % 2 == 0 )
	//{
	//	SI32 ChangedGrade = siGradeNow + MAX_PVP_LEAGUE_GRADE_UP_DOWN_POINT	;
	//	if ( ChangedGrade >  PVP_LEAGUE_GRADE_1 ) 
	//	{
	//		ChangedGrade = PVP_LEAGUE_GRADE_1	;
	//	}
	//	pclChar->SetPVP_LeagueStraight_Win_Grade( 0 )	;
	//	LogGradeUpDown(pclChar->pclCI->GetPersonID() ,siGradeNow, ChangedGrade,LOGCOMMAND_INDEX_PVP_LEAGUE_GRADE_UP );
	//	sDBRequest_PVP_League_Battle_ChangeGrade clMsg(  pclChar->pclCI->GetPersonID() ,  ChangedGrade );
	//	pclserver->pclGameDBProcess->SendMsg( DBSELECT_SYSTEM, (sPacketHeader *)&clMsg);		
	//}
}

void CPVP_LeagueMgr::PVP_League_LevelDown( SI32 siID , SI32 siStrightLoseCountGrade )
{
	cltServer* pclserver = (cltServer*)pclClient	;

	SI32 siId = siID	;
	if ( pclClient->pclCM->IsValidID( siID ) == false )		return	;
	cltCharServer* pclChar = pclClient->pclCM->GetCharServer(siId);			if(pclChar == NULL)			return ;
	SI32 siGradeNow	=	pclChar->GetPVP_LeagueGrade()	;
	SI32 ChangedGrade = ChangeGrade(siGradeNow,siStrightLoseCountGrade,PVP_LEAGUE_END_GRADEDOWN )	;

	if(pclChar->GetPVP_LeagueGrade() != ChangedGrade )
	{
		pclChar->SetPVP_LeagueStraight_Lose_Grade( 0 )	;
		LogGradeUpDown(pclChar->pclCI->GetPersonID() ,siGradeNow, ChangedGrade,LOGCOMMAND_INDEX_PVP_LEAGUE_GRADE_DOWN );

		sDBRequest_PVP_League_Battle_ChangeGrade clMsg(  pclChar->pclCI->GetPersonID() ,  ChangedGrade,pclserver->sTime );
		pclserver->pclGameDBProcess->SendMsg( DBSELECT_SYSTEM, (sPacketHeader *)&clMsg);
	}

	//// 5등급 이상 3연패 했을 경우에 케릭터의 등급을 내린다
	//if( pclChar->GetPVP_LeagueGrade() > PVP_LEAGUE_GRADE_6  && siStrightLoseCountGrade != 0 && siStrightLoseCountGrade % 3 == 0 )
	//{
	//	SI32 siMyGrade = pclChar->GetPVP_LeagueGrade()	;
	//	SI32 ChangeGrade = siMyGrade - MAX_PVP_LEAGUE_GRADE_UP_DOWN_POINT	;
	//	if ( ChangeGrade <= PVP_LEAGUE_GRADE_9 ) 
	//	{
	//		ChangeGrade = PVP_LEAGUE_GRADE_9	;
	//	}
	//	pclChar->SetPVP_LeagueStraight_Lose_Grade( 0 )	;
	//	LogGradeUpDown(pclChar->pclCI->GetPersonID() ,siMyGrade, ChangeGrade,LOGCOMMAND_INDEX_PVP_LEAGUE_GRADE_DOWN );
	//	sDBRequest_PVP_League_Battle_ChangeGrade clMsg(  pclChar->pclCI->GetPersonID() ,  ChangeGrade );
	//	pclserver->pclGameDBProcess->SendMsg( DBSELECT_SYSTEM, (sPacketHeader *)&clMsg);		
	//}
	//// 5등급 미만 2연패 했을 경우 케릭터의 등급을 내린다.
	//else if(pclChar->GetPVP_LeagueGrade() <= PVP_LEAGUE_GRADE_6  &&	siStrightLoseCountGrade != 0 && siStrightLoseCountGrade % 2 == 0 ) 
	//{
	//	SI32 siMyGrade = pclChar->GetPVP_LeagueGrade()	;
	//	SI32 ChangeGrade = siMyGrade - MAX_PVP_LEAGUE_GRADE_UP_DOWN_POINT	;
	//	if ( ChangeGrade <= PVP_LEAGUE_GRADE_9 ) 
	//	{
	//		ChangeGrade = PVP_LEAGUE_GRADE_9	;
	//	}
	//	pclChar->SetPVP_LeagueStraight_Lose_Grade( 0 )	;
	//	LogGradeUpDown(pclChar->pclCI->GetPersonID() ,siMyGrade, ChangeGrade,LOGCOMMAND_INDEX_PVP_LEAGUE_GRADE_DOWN );
	//	sDBRequest_PVP_League_Battle_ChangeGrade clMsg(  pclChar->pclCI->GetPersonID() ,  ChangeGrade );
	//	pclserver->pclGameDBProcess->SendMsg( DBSELECT_SYSTEM, (sPacketHeader *)&clMsg);		

	//}
}

void CPVP_LeagueMgr::PVP_LeagueReady()
{
	//여기서는 워프만 해준다.
	for ( SI32 siSlot = 0 ; siSlot < MAX_MATCHING_PERSON ; siSlot++ ) 
	{
		// 날려올 사람 선택 
		SI32 siHomePerson = m_clPVP_LeageMatchingList.GetBattlePersonIDHome(siSlot)	;
		SI32 siAwayPerson = m_clPVP_LeageMatchingList.GetBattlePersonIDAway(siSlot)	;
		if ( siHomePerson <=0 && siAwayPerson <= 0)		continue	;

		if ( 0 == siSlot) 
		{
			PVP_League_Warp_Stage( siHomePerson, siAwayPerson , MAPINDEX_MAINFIELD, siSlot)	;
		}
		else
		{
			PVP_League_Warp_Stage( siHomePerson, siAwayPerson , MAPINDEX_PVPMAP, siSlot)	;
		}
	}
}

void CPVP_LeagueMgr::PVP_League_Warp_Stage( SI32 siHomePersonID,SI32 siAwayPersonID, SI32 siMapIndex, SI32 siSlot )
{
	cltWarpInfo clHomeWarpInfo(1, siMapIndex, true, false, pclmapinfo[siSlot].m_HomeX, pclmapinfo[siSlot].m_HomeY, -1,  -1)	;

	SI32 Homeid = pclClient->pclCM->GetIDFromPersonID( siHomePersonID ) ;
	cltCharServer* pclHomechar = NULL;
	// 살아 있을때만 소환 한다,.
	if( pclClient->pclCM->IsAlive(Homeid) )	pclHomechar = (cltCharServer*)pclClient->pclCM->GetCharServer( Homeid );
	if (pclHomechar )
	{
		pclHomechar->PushPVP_LeagueLatestPersonID( siAwayPersonID )	;
		pclHomechar->SetWarpInfo(&clHomeWarpInfo, 3000);
		pclHomechar->Warp();		
	}

	cltWarpInfo clAwayWarpInfo(1, siMapIndex, true, false, pclmapinfo[siSlot].m_AwayX, pclmapinfo[siSlot].m_AwayY, -1,  -1)	;
	clAwayWarpInfo.uiMapIndex = siMapIndex	;
	SI32 Awayid = pclClient->pclCM->GetIDFromPersonID( siAwayPersonID ) ;
	cltCharServer* pclAwaychar = NULL;
	// 살아 있을때만 소환 한다,.
	if( pclClient->pclCM->IsAlive(Awayid) )	pclAwaychar = (cltCharServer*)pclClient->pclCM->GetCharServer( Awayid );
	if (pclAwaychar )
	{
		pclAwaychar->PushPVP_LeagueLatestPersonID( siHomePersonID )	;
		pclAwaychar->SetWarpInfo(&clAwayWarpInfo, 3000);
		pclAwaychar->Warp();
		
	}
}

void CPVP_LeagueMgr::PVP_LeagueStart()
{	
	cltServer* pclserver = (cltServer*)pclClient;

	for ( SI32 siSlot = 0 ; siSlot < MAX_MATCHING_PERSON ; siSlot++ ) 
	{
		// 날려올 사람 선택 
		SI32 siHomePerson = m_clPVP_LeageMatchingList.GetBattlePersonIDHome(siSlot)	;
		SI32 siAwayPerson = m_clPVP_LeageMatchingList.GetBattlePersonIDAway(siSlot)	;
		if ( siHomePerson <= 0 && siAwayPerson <= 0)						continue	;

		SI32 siHomeId		=	pclserver->pclCM->GetIDFromPersonID(siHomePerson)	;
		SI32 siAwayId		=	pclserver->pclCM->GetIDFromPersonID( siAwayPerson)	;

		cltCharServer* pclHomeChar = NULL;
		cltCharServer* pclAwayChar = NULL;

		if( pclClient->pclCM->IsAlive( siHomeId) )		pclHomeChar = pclClient->pclCM->GetCharServer(siHomeId);	
		if( pclClient->pclCM->IsAlive( siAwayId) )		pclAwayChar = pclClient->pclCM->GetCharServer(siAwayId);	

		if( pclHomeChar )
		{
			pclHomeChar->siPVPWaitRequestId		=	siHomeId			;// PVP 요청한 사람 ID
			pclHomeChar->siPVPWaitAcceptId		=	siAwayId			;// PVP 요청을 받아서 ok 한 사람 ID

			pclHomeChar->SetPVP_LeagueEnemyPersonID( siAwayPerson )	;

			cltGameMsgResponse_PVP_Request clAcceptToRequest( READY_PVP,0,0,TEXT(""));	// 각가 기다리라는 메세지를 날려준다 서버 시간을 날린다.
			cltMsg clMsg( GAMEMSG_RESPONSE_PVP_FILED, sizeof(clAcceptToRequest), (BYTE*)&clAcceptToRequest );
			pclHomeChar->SendNetMsg((sPacketHeader*)&clMsg);
		}

		if( pclAwayChar )
		{
			pclAwayChar->siPVPWaitRequestId		=	siHomeId			;// PVP 요청한 사람 ID
			pclAwayChar->siPVPWaitAcceptId		=	siAwayId			;// PVP 요청을 받아서 ok 한 사람 ID

			pclAwayChar->SetPVP_LeagueEnemyPersonID( siHomePerson )	;

			//각기 클라이언트로 알려준다.
			cltGameMsgResponse_PVP_Request clclAcceptToAccepeter( READY_PVP ,0,0,TEXT(""));// 기다리라는 메세지를 날려준다 서버시간을 날린다
			cltMsg clMsg2( GAMEMSG_RESPONSE_PVP_FILED, sizeof(clclAcceptToAccepeter), (BYTE*)&clclAcceptToAccepeter );
			pclAwayChar->SendNetMsg((sPacketHeader*)&clMsg2);
		}
	}
}

void CPVP_LeagueMgr::PVP_LeagueMapInfo()
{
	NDataLoader	dataloader;
	BOOL bLoadFile=FALSE;

	//KHY - 0810 - 텍스트 리소스 암호화.
	if( pclClient->IsWhereServiceArea( ConstSwitch_TextEncryption ) )
	{
		bLoadFile = dataloader.LoadDataFromCryptFile( TEXT("Data/PVPMapInfo.txt") );
	}
	else
	{
		bLoadFile = dataloader.LoadDataFromFile( TEXT("Data/PVPMapInfo.txt") );
	}

	if (!bLoadFile)
	{
		MsgBox( TEXT("Not LoadFromFile PVPMapInfo"),TEXT("Error"));
		return;
	}

	SI32 pslot,pmapindex, phomeX,phomeY,pawayX,pawayY;

	NDataTypeInfo pDataTypeInfo[] =
	{
		NDATA_INT32,	&pslot,	4,
			NDATA_INT32,	&pmapindex,	4,
			NDATA_INT32,	&phomeX,	4,
			NDATA_INT32,	&phomeY,	4,
			NDATA_INT32,	&pawayX,	4,
			NDATA_INT32,	&pawayY,	4,
			0,0,0
	};

	dataloader.RegDataType( pDataTypeInfo );


	SI16 index = 0 ;

	while( !dataloader.IsEndOfData() ) 
	{
		if (dataloader.ReadData( true ))	
		{
			pclmapinfo[index].m_slot	=	pslot	;
			pclmapinfo[index].m_index	=	pmapindex	;
			pclmapinfo[index].m_HomeX	=	phomeX	;
			pclmapinfo[index].m_HomeY	=	phomeY	;
			pclmapinfo[index].m_AwayX	=	pawayX	;
			pclmapinfo[index].m_AwayY	=	pawayY	;
			index++;
		}
	}
}

void CPVP_LeagueMgr::PVP_Battle_Step_Draw( void )
{
	for (SI32 siSLot = 0 ; siSLot < MAX_MATCHING_PERSON; siSLot++) 
	{
		SI32 siHome = m_clPVP_LeageMatchingList.GetBattlePersonIDHome(siSLot)	;
		SI32 siAway = m_clPVP_LeageMatchingList.GetBattlePersonIDAway(siSLot)	;
		if ( siHome == 0 && siAway == 0 )	continue	;

		End_PVP_Battle_Draw(siHome,siAway);
		LogDraw( siHome,siAway )	;
	}	
}

void CPVP_LeagueMgr::PVP_Battle_Step_Game( void )
{
	// 게임 진행 - 승, 패, 비김을 처리한다.
	for (SI32 siSLot = 0 ; siSLot < MAX_MATCHING_PERSON; siSLot++) 
	{
		SI32 siHome = m_clPVP_LeageMatchingList.GetBattlePersonIDHome(siSLot)	;
		SI32 siAway = m_clPVP_LeageMatchingList.GetBattlePersonIDAway(siSLot)	;
		if ( siHome == 0 && siAway == 0 )	continue	;

		SI32 siHomeCharID = pclClient->pclCM->GetIDFromPersonID( siHome );
		SI32 siAwayCharID = pclClient->pclCM->GetIDFromPersonID( siAway );

		bool IsHomeLose = false;		// 전투에서 졌는지를 찾는다.
		bool IsAwayLose = false;		// 전투에서 졌는지를 찾는다.

		cltCharServer* pclCharHome = pclClient->pclCM->GetCharServer( siHomeCharID );
		cltCharServer* pclCharAway = pclClient->pclCM->GetCharServer( siAwayCharID );

		if( pclClient->pclCM->IsAlive( siHomeCharID ) == false ||		// 살아있지 않거나, 캐릭터가 없는경우는 진거다.
			pclCharHome->pclCI->GetPersonID() != siHome )				// 캐릭터가 바뀐경우는 진거다.
		{	IsHomeLose = true;	}

		if( pclClient->pclCM->IsAlive( siAwayCharID ) == false ||		// 살아있지 않거나, 캐릭터가 없는경우는 진거다.
			pclCharAway->pclCI->GetPersonID() != siAway )				// 캐릭터가 바뀐경우는 진거다.
		{	IsAwayLose = true;	}

		if( Is_MatchingListPeron_TruePosition( siHome ) == false )		// 리그맵에 있지 않으면 진거다.
		{	IsHomeLose = true;	}

		if( Is_MatchingListPeron_TruePosition( siAway ) == false )		// 리그맵에 있지 않으면 진거다.
		{	IsAwayLose = true;	}

		if( IsHomeLose == true && IsAwayLose == true )
		{
			// 둘다 졌으니 비김.
			End_PVP_Battle_Draw( siHome, siAway );
		}
		else if ( IsHomeLose == true && IsAwayLose == false)
		{
			// Away 이김
			End_PVP_Battle_End( siAway, siHome );
		}
		else if( IsHomeLose == false && IsAwayLose == true )
		{
			// Home 이김
			End_PVP_Battle_End( siHome, siAway );
		}
	}	
}

void CPVP_LeagueMgr::PVP_Battle_HaveItemMessage( SI32 siID, SI32 siItemUnique )	// 아이템을 받았다고 명품공지창을 이용해서 알린다.
{
	cltServer* pclserver = (cltServer*)pclClient	;

	SI32 siId = siID ;//pclClient->pclCM->GetIDFromPersonID(siPersonID ); // 아이디를 가지곻 온다.
	if ( pclClient->pclCM->IsValidID( siId ) == false )		return	;
	SI32 siParentID = pclClient->pclCM->CR[ siId ]->GetSummonParentID()		;
	if ( siParentID == 0 )	siParentID = siId		;
	cltCharServer* pclChar = pclClient->pclCM->GetCharServer(siParentID);			if(pclChar == NULL)			return ;
	SI16 brareswitch = 0 ;
	cltItem clitemGoal;	
	pclserver->pclItemManager->MakeRandItemUnique(siItemUnique, &clitemGoal, 0, 0,&brareswitch , 0, 0, 0, 0, 0, 0);
	// 명품 알림 공지를 사용하여 아이템 획득을 알린다.
	cltGameMsgResponse_RareItemMade	clinfo((TCHAR*)(pclChar->GetName()), pclChar->pclCI->GetPersonID(), 
		&clitemGoal,
		RARAITEM_NOTICE_SWITCH_PVPLEAGUEPRIZE,
		0);
	cltMsg clMsg(GAMEMSG_RESPONSE_RAREITEMMADE, sizeof(clinfo), (BYTE*)&clinfo);
	pclClient->pclCM->SendAllPersonMsg((sPacketHeader*)&clMsg);
}

void CPVP_LeagueMgr::PVP_League_Before1m( void )
{
	for ( SI32 siIndex=0; siIndex<MAX_MATCHING_PERSON; ++siIndex ) 
	{
		//각 슬롯의 매칭 리스트에 있는 사람을 찾아서
		SI32 siHomePersonID	=	m_clPVP_LeageMatchingList.GetBattlePersonIDHome(siIndex)	;
		SI32 siAwayPersonID	=	m_clPVP_LeageMatchingList.GetBattlePersonIDAway(siIndex)	;
		if ( siHomePersonID <= 0 && siAwayPersonID <= 0 )	continue	;	// 0이면 다음 슬롯

		SI32 siHomeID = pclClient->pclCM->GetIDFromPersonID( siHomePersonID)	;
		SI32 siAwayID = pclClient->pclCM->GetIDFromPersonID(siAwayPersonID)	;
		cltCharServer* pclHomeChar = pclClient->pclCM->GetCharServer(siHomeID);
		cltCharServer* pclAwayChar = pclClient->pclCM->GetCharServer(siAwayID);

		if( pclHomeChar )
		{
			cltGameMsgResponse_PVP_Request cltoHome( PVP_LEAGUEBEFORE1M ,0,0,TEXT(""))	;
			cltMsg clMsg( GAMEMSG_RESPONSE_PVP_FILED, sizeof(cltoHome), (BYTE*)&cltoHome )	;
			pclHomeChar->SendNetMsg((sPacketHeader*)&clMsg)	;
		}

		if( pclAwayChar )
		{
			cltGameMsgResponse_PVP_Request clToAway( PVP_LEAGUEBEFORE1M ,0,0,TEXT(""))	;
			cltMsg clMsg2( GAMEMSG_RESPONSE_PVP_FILED, sizeof(clToAway), (BYTE*)&clToAway )	;
			pclAwayChar->SendNetMsg((sPacketHeader*)&clMsg2)	;
		}
	}
}

// 일반 보상 아이템
void CPVP_LeagueMgr::PVP_League_GiveItem( SI32 siID, SI32 siItemUnique, bool bSpecial )
{
	cltServer* pclserver = (cltServer*)pclClient	;

	SI32 siId = siID	;
	if ( pclClient->pclCM->IsValidID( siID ) == false )		return	;
	cltCharServer* pclChar = pclClient->pclCM->GetCharServer(siId);			if(pclChar == NULL)			return ;

	SI32 siSpecial = 0	;
	if(bSpecial == true)	{siSpecial = INPUTITEMPERSON_OPTION_PVPLEAGUEPRIZE_SPECIAL	;	}
	else {siSpecial	=	INPUTITEMPERSON_OPTION_PVPLEAGUEPRIZE	;	}

	SI32 siRewardItemUnique	=	siItemUnique ;
	if( siRewardItemUnique ) 
	{
		cltItem clItem;
		SI16 brareswitch = 0;
		if ( pclClient->pclItemManager->MakeRandItemUnique( siRewardItemUnique,&clItem,0,0,&brareswitch,0,0,0,0,0) == true ) 
		{
			clItem.siItemNum = 1;
			SI32 rtnvalue = 0;
			if(pclChar->InputItem(pclClient->pclItemManager, &clItem, siSpecial, &rtnvalue))
			{
				if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
				{
					// [로그] 아이템을 준다
					pclserver->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_PERSONALPVP_LEAGUE, LOGCOMMAND_INDEX_PVP_LEAGUE_GIFT, 
						0, pclChar,NULL ,0, 0, 0, &clItem, 0, 0, 0, 0, 0, 0, 
						0, 0, 0, 0, 0, NULL, NULL);
				}

				// 서버에 남긴다.
				TCHAR fileNameBuffer[256];
				//슬롯에 찍을 시간을 텍스트로 완성한다.
				TCHAR szTime[128];
				TCHAR szBuffer0[128];
				SYSTEMTIME stTimeInfo;
				GetLocalTime(&stTimeInfo);
				StringCchPrintf( szTime, sizeof(szTime) ,TEXT("%d:%d:%d:%d:%d"),stTimeInfo.wMonth,stTimeInfo.wDay,stTimeInfo.wHour,stTimeInfo.wMinute,stTimeInfo.wSecond);
				StringCchPrintf(szBuffer0, sizeof(szBuffer0),"%s",&szTime) ;
				StringCchPrintf( fileNameBuffer , sizeof(fileNameBuffer) , TEXT("Config\\PVPLeagueLog_%04d%02d%02d.log") 
					, stTimeInfo.wYear , stTimeInfo.wMonth , stTimeInfo.wDay );

				pclClient->pclLog->FilePrint(TEXT(fileNameBuffer), 
					TEXT(" PVPLEAGUE_GIVEITEM \t PERSONNAME : [%s]\t \tITEM : [%d]\t NUM : [%d] Time"),
					pclChar->GetName(),clItem.siUnique,clItem.siItemNum,szBuffer0 );
			}
			else
			{
				if(rtnvalue == CANADDINV_FAIL_FULL)
				{
					pclserver->SendServerResponseMsg( 0, SRVAL_ADDINV_FAIL_FULL,  clItem.siUnique, 0, pclChar->GetCharUnique());
				}
			}
		}		
	}
}

void CPVP_LeagueMgr::LogWinLose( SI32 siWinnerPersonId , SI32 siLoserPersonId )
{
	cltServer* pclserver = (cltServer*)pclClient	;

	// 홈과 어웨이 퍼슨아이디를 가지고 온다.
	SI32 siWinnerId	=	pclClient->pclCM->GetIDFromPersonID( siWinnerPersonId )	;
	SI32 siLoserId	=	pclClient->pclCM->GetIDFromPersonID( siLoserPersonId )	;
	if ( pclClient->pclCM->IsValidID( siWinnerId ) == false )	return	;
	if ( pclClient->pclCM->IsValidID( siLoserId ) == false )	return	;
	cltCharServer* pclWinnerChar = pclClient->pclCM->GetCharServer(siWinnerId)	;			if(pclWinnerChar == NULL)			return	;
	cltCharServer* pclLoserChar = pclClient->pclCM->GetCharServer(siLoserId)	;			if(pclLoserChar == NULL)			return	;

	SI16 siWinnerActiveWeapone = pclWinnerChar->ActiveWeapon	;
	SI16 siLoserActiveWeapone = pclLoserChar->ActiveWeapon	;
	SI32 siWinnerWeaponeITemType = pclWinnerChar->pclCI->clCharItem.clItem[siWinnerActiveWeapone].GetType( pclClient->pclItemManager)	;
	SI32 siLoserWeaponeITemType = pclLoserChar->pclCI->clCharItem.clItem[siLoserActiveWeapone].GetType( pclClient->pclItemManager)	;

	if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
	{
		// para1 = 이긴케릭터 케릭터 ,para2 : 이긴사람 아이템 타입  para3 진 케릭터  para3 : 진사람 아이템 타입
		// PCK : 수정 - PersonID로 캐릭터이름을 구해서 좀더 보기 쉽게한다. (09.07.31)

		TCHAR szCharName[100];
		ZeroMemory(szCharName, sizeof(szCharName));
		StringCchPrintf(szCharName, sizeof(szCharName), TEXT("WinnerChar:%s, LoserChar:%s"), pclWinnerChar->GetName(), pclLoserChar->GetName());

		pclserver->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_PERSONALPVP_LEAGUE, LOGCOMMAND_INDEX_PVP_LEAGUE_RESULT, 
			0,NULL ,NULL,0, 0, 0, NULL, 0, 0, 0, 0, 0, 0,
			pclWinnerChar->pclCI->GetPersonID(),siWinnerWeaponeITemType, pclLoserChar->pclCI->GetPersonID(), siLoserWeaponeITemType, 0, szCharName, NULL);
	}
	// 서버에 남긴다.
	TCHAR fileNameBuffer[256];
	//슬롯에 찍을 시간을 텍스트로 완성한다.
	TCHAR szTime[128];
	TCHAR szBuffer0[128];
	SYSTEMTIME stTimeInfo;
	GetLocalTime(&stTimeInfo);
	StringCchPrintf( szTime, sizeof(szTime) ,TEXT("%d:%d:%d:%d:%d"),stTimeInfo.wMonth,stTimeInfo.wDay,stTimeInfo.wHour,stTimeInfo.wMinute,stTimeInfo.wSecond);
	StringCchPrintf(szBuffer0, sizeof(szBuffer0),"%s",&szTime) ;
	StringCchPrintf( fileNameBuffer , sizeof(fileNameBuffer) , TEXT("Config\\PVPLeagueLog_%04d%02d%02d.log") 
		, stTimeInfo.wYear , stTimeInfo.wMonth , stTimeInfo.wDay );

	pclClient->pclLog->FilePrint(TEXT(fileNameBuffer), 
		TEXT(" PVPLEAGUE_RESULT \t Winner : [%s]\tLoser : [%s]\t \t WinnerWeapone vs LoserWeapone : [%d] vs [%d] \tTime"),
		pclWinnerChar->GetName(),pclLoserChar->GetName(),siWinnerWeaponeITemType,siLoserWeaponeITemType,szBuffer0 );
}

void CPVP_LeagueMgr::LogDraw( SI32 siDrawPersonId1 , SI32 siDrawPersonId )
{
	cltServer* pclserver = (cltServer*)pclClient	;

	// 홈과 어웨이 퍼슨아이디를 가지고 온다.
	SI32 siDrawId1	=	pclClient->pclCM->GetIDFromPersonID( siDrawPersonId1 )	;
	SI32 siDrawId2	=	pclClient->pclCM->GetIDFromPersonID( siDrawPersonId )	;
	if ( pclClient->pclCM->IsValidID( siDrawId1 ) == false )	return	;
	if ( pclClient->pclCM->IsValidID( siDrawId2 ) == false )	return	;
	cltCharServer* pclDrawChar1 = pclClient->pclCM->GetCharServer(siDrawId1)	;			if(pclDrawChar1 == NULL)			return	;
	cltCharServer* pclDrawChar2 = pclClient->pclCM->GetCharServer(siDrawId2)	;			if(pclDrawChar2 == NULL)			return	;

	SI16 siDraw1ActiveWeapone = pclDrawChar1->ActiveWeapon	;
	SI16 siDraw2ActiveWeapone = pclDrawChar2->ActiveWeapon	;

	SI32 siDraw1WeaponeITemType = pclDrawChar1->pclCI->clCharItem.clItem[siDraw1ActiveWeapone].GetType( pclClient->pclItemManager)	;
	SI32 siDraw2WeaponeITemType = pclDrawChar2->pclCI->clCharItem.clItem[siDraw2ActiveWeapone].GetType( pclClient->pclItemManager)	;

	if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
	{
		// para1 = 비긴 케릭터 1 para2 비긴 케릭터 
		pclserver->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_PERSONALPVP_LEAGUE, LOGCOMMAND_INDEX_PVP_LEAGUE_RESULT_DRAW, 
			0,NULL ,NULL,0, 0, 0, NULL, 0, 0, 0, 0, 0, 0, 
			pclDrawChar1->pclCI->GetPersonID(), pclDrawChar2->pclCI->GetPersonID(), 0, 0, 0, NULL, NULL);
	}

	// 서버에 남긴다.
	TCHAR fileNameBuffer[256];
	//슬롯에 찍을 시간을 텍스트로 완성한다.
	TCHAR szTime[128];
	TCHAR szBuffer0[128];
	SYSTEMTIME stTimeInfo;
	GetLocalTime(&stTimeInfo);
	StringCchPrintf( szTime, sizeof(szTime) ,TEXT("%d:%d:%d:%d:%d"),stTimeInfo.wMonth,stTimeInfo.wDay,stTimeInfo.wHour,stTimeInfo.wMinute,stTimeInfo.wSecond);
	StringCchPrintf(szBuffer0, sizeof(szBuffer0),"%s",&szTime) ;
	StringCchPrintf( fileNameBuffer , sizeof(fileNameBuffer) , TEXT("Config\\PVPLeagueLog_%04d%02d%02d.log") 
		, stTimeInfo.wYear , stTimeInfo.wMonth , stTimeInfo.wDay );

	pclClient->pclLog->FilePrint(TEXT(fileNameBuffer), 
		TEXT(" PVPLEAGUE_DRAW \t Person1 : [%s]\t Person2 : [%s]\t Weapone VS Weapone : [%d] vs [%d][]Time"),
		pclDrawChar1->GetName(),pclDrawChar2->GetName(),siDraw1WeaponeITemType,siDraw2WeaponeITemType,szBuffer0 );
}

void CPVP_LeagueMgr::LogGoal( SI32 siPersonID , SI32 siGoalFlag )
{
	cltServer* pclserver = (cltServer*)pclClient	;

	SI32 siID	=	pclClient->pclCM->GetIDFromPersonID( siPersonID )	;
	if ( pclClient->pclCM->IsValidID( siID ) == false )	return	;
	cltCharServer* pclChar = pclClient->pclCM->GetCharServer(siID)	;			if(pclChar == NULL)			return	;

	if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
	{
		// prar1은 목표 달성 내용
		pclserver->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_PERSONALPVP_LEAGUE, LOGCOMMAND_INDEX_PVP_LEAGUE_GOAL, 
			0,pclChar ,NULL,0, 0, 0, NULL, 0, 0, 0, 0, 0, 0, 
			siGoalFlag, 0, 0, 0, 0, NULL, NULL);
	}

	// 서버에 남긴다.
	TCHAR fileNameBuffer[256];
	//슬롯에 찍을 시간을 텍스트로 완성한다.
	TCHAR szTime[128];
	TCHAR szBuffer0[128];
	SYSTEMTIME stTimeInfo;
	GetLocalTime(&stTimeInfo);
	StringCchPrintf( szTime, sizeof(szTime) ,TEXT("%d:%d:%d:%d:%d"),stTimeInfo.wMonth,stTimeInfo.wDay,stTimeInfo.wHour,stTimeInfo.wMinute,stTimeInfo.wSecond);
	StringCchPrintf(szBuffer0, sizeof(szBuffer0),"%s",&szTime) ;
	StringCchPrintf( fileNameBuffer , sizeof(fileNameBuffer) , TEXT("Config\\PVPLeagueLog_%04d%02d%02d.log") 
		, stTimeInfo.wYear , stTimeInfo.wMonth , stTimeInfo.wDay );

	if ( siGoalFlag == PVP_LEAGUE_END_RESULT_100WIN) 
	{
		pclClient->pclLog->FilePrint(TEXT(fileNameBuffer), 
			TEXT(" PVPLEAGUE_Goal \t NAME : [%s]\t GoalFlag : [%d]\t Time"),
			pclChar->GetName(),PVP_LEAGUE_END_RESULT_100WIN,szBuffer0 );
	}
	else if ( siGoalFlag == PVP_LEAGUE_END_RESULT_1000WIN) 
	{
		pclClient->pclLog->FilePrint(TEXT(fileNameBuffer), 
			TEXT(" PVPLEAGUE_Goal \t NAME : [%s]\t GoalFlag : [%d]\t Time"),
			pclChar->GetName(),PVP_LEAGUE_END_RESULT_1000WIN,szBuffer0 );
	}
	else if ( siGoalFlag == PVP_LEAGUE_END_RESULT_100DRAW) 
	{
		pclClient->pclLog->FilePrint(TEXT(fileNameBuffer), 
			TEXT(" PVPLEAGUE_Goal \t NAME : [%s]\t GoalFlag : [%d]\t Time"),
			pclChar->GetName(),PVP_LEAGUE_END_RESULT_100DRAW,szBuffer0 );
	}
	else if ( siGoalFlag == PVP_LEAGUE_END_RESULT_1000DRAW) 
	{
		pclClient->pclLog->FilePrint(TEXT(fileNameBuffer), 
			TEXT(" PVPLEAGUE_Goal \t NAME : [%s]\t GoalFlag : [%d]\t Time"),
			pclChar->GetName(),PVP_LEAGUE_END_RESULT_1000DRAW,szBuffer0 );
	}
}

void CPVP_LeagueMgr::LogReserve( SI32 siPersonID )
{
	cltServer* pclserver = (cltServer*)pclClient	;

	SI32 siId = pclClient->pclCM->GetIDFromPersonID( siPersonID )	;
	if ( pclClient->pclCM->IsValidID( siId ) == false )		return	;
	SI32 siParentID = pclClient->pclCM->CR[ siId ]->GetSummonParentID()		;
	if ( siParentID == 0 )	siParentID = siId		;
	cltCharServer* pclChar = pclClient->pclCM->GetCharServer(siParentID);			if(pclChar == NULL)			return ;

	if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
	{
		// [로그추가 : 손서웅ㅇ 2008. 10. 29] // pvp 타겟에 요청
		// 타켓 personID 
		pclserver->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_PERSONALPVP_LEAGUE, LOGCOMMAND_INDEX_PVP_LEAGUE_RESERVE, 
			0, pclChar,NULL ,0, 0, 0, NULL, 0, 0, 0, 0, 0, 0, 
			0, 0, 0, 0, 0, NULL, NULL);
	}

	// 서버에 남긴다.
	TCHAR fileNameBuffer[256];
	//슬롯에 찍을 시간을 텍스트로 완성한다.
	TCHAR szTime[128];
	TCHAR szBuffer0[128];
	SYSTEMTIME stTimeInfo;
	GetLocalTime(&stTimeInfo);
	StringCchPrintf( szTime, sizeof(szTime) ,TEXT("%d:%d:%d:%d:%d"),stTimeInfo.wMonth,stTimeInfo.wDay,stTimeInfo.wHour,stTimeInfo.wMinute,stTimeInfo.wSecond);
	StringCchPrintf(szBuffer0, sizeof(szBuffer0),"%s",&szTime) ;
	StringCchPrintf( fileNameBuffer , sizeof(fileNameBuffer) , TEXT("Config\\PVPLeagueLog_%04d%02d%02d.log") 
		, stTimeInfo.wYear , stTimeInfo.wMonth , stTimeInfo.wDay );

	pclClient->pclLog->FilePrint(TEXT(fileNameBuffer), 
		TEXT(" PVPLEAGUE_RESERVE \t PERSONNAME : [%s]\t Time"),
		pclChar->GetName(),szBuffer0 );
}

void CPVP_LeagueMgr::LogGradeUpDown( SI32 siPersonID,SI32 siBeforeGrade, SI32 siAfterGrade, SI32 UpDownFlag )
{
	cltServer* pclserver = (cltServer*)pclClient	;

	SI32 siId = pclClient->pclCM->GetIDFromPersonID( siPersonID )	;
	if ( pclClient->pclCM->IsValidID( siId ) == false )		return	;
	SI32 siParentID = pclClient->pclCM->CR[ siId ]->GetSummonParentID()		;
	if ( siParentID == 0 )	siParentID = siId		;
	cltCharServer* pclChar = pclClient->pclCM->GetCharServer(siParentID);			if(pclChar == NULL)			return ;
	if ( LOGCOMMAND_INDEX_PVP_LEAGUE_GRADE_UP == UpDownFlag ) 
	{
		if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
		{
			// para1 : 이전 등급 para2 : 이후등급
			pclserver->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_PERSONALPVP_LEAGUE, LOGCOMMAND_INDEX_PVP_LEAGUE_GRADE_UP, 
				0, pclChar,NULL ,0, 0, 0, NULL, 0, 0, 0, 0, 0, 0, 
				siBeforeGrade, siAfterGrade, 0, 0, 0, NULL, NULL);
		}
		// 서버에 남긴다.
		TCHAR fileNameBuffer[256];
		//슬롯에 찍을 시간을 텍스트로 완성한다.
		TCHAR szTime[128];
		TCHAR szBuffer0[128];
		SYSTEMTIME stTimeInfo;
		GetLocalTime(&stTimeInfo);
		StringCchPrintf( szTime, sizeof(szTime) ,TEXT("%d:%d:%d:%d:%d"),stTimeInfo.wMonth,stTimeInfo.wDay,stTimeInfo.wHour,stTimeInfo.wMinute,stTimeInfo.wSecond);
		StringCchPrintf(szBuffer0, sizeof(szBuffer0),"%s",&szTime) ;
		StringCchPrintf( fileNameBuffer , sizeof(fileNameBuffer) , TEXT("Config\\PVPLeagueLog_%04d%02d%02d.log") 
			, stTimeInfo.wYear , stTimeInfo.wMonth , stTimeInfo.wDay );

		pclChar = pclClient->pclCM->GetCharServer(siParentID);			if(pclChar == NULL)			return ;
		pclClient->pclLog->FilePrint(TEXT(fileNameBuffer), 
			TEXT(" PVPLEAGUE_GRADEUP \t PERSONNAME:[%s] \t BEFOREGRADE:[%d] \t AFTERGRADE:[%d] \t Time"),
			pclChar->GetName(),siBeforeGrade,siAfterGrade,szBuffer0 );
	}
	else if ( LOGCOMMAND_INDEX_PVP_LEAGUE_GRADE_DOWN == UpDownFlag) 
	{
		if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
		{
			// para1 : 이전 등급 para2 : 이후등급
			pclserver->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_PERSONALPVP_LEAGUE, LOGCOMMAND_INDEX_PVP_LEAGUE_GRADE_DOWN, 
				0, pclChar,NULL ,0, 0, 0, NULL, 0, 0, 0, 0, 0, 0, 
				siBeforeGrade, siAfterGrade, 0, 0, 0, NULL, NULL);
		}

		// 서버에 남긴다.
		TCHAR fileNameBuffer[256];
		//슬롯에 찍을 시간을 텍스트로 완성한다.
		TCHAR szTime[128];
		TCHAR szBuffer0[128];
		SYSTEMTIME stTimeInfo;
		GetLocalTime(&stTimeInfo);
		StringCchPrintf( szTime, sizeof(szTime) ,TEXT("%d:%d:%d:%d:%d"),stTimeInfo.wMonth,stTimeInfo.wDay,stTimeInfo.wHour,stTimeInfo.wMinute,stTimeInfo.wSecond);
		StringCchPrintf(szBuffer0, sizeof(szBuffer0),"%s",&szTime) ;
		StringCchPrintf( fileNameBuffer , sizeof(fileNameBuffer) , TEXT("Config\\PVPLeagueLog_%04d%02d%02d.log") 
			, stTimeInfo.wYear , stTimeInfo.wMonth , stTimeInfo.wDay );

		pclChar = pclClient->pclCM->GetCharServer(siParentID);			if(pclChar == NULL)			return ;
		pclClient->pclLog->FilePrint(TEXT(fileNameBuffer),
			TEXT(" PVPLEAGUE_GRADEDOWN \t PERSONNAME:[%s] \t BEFOREGRADE:[%d] \t AFTERGRADE:[%d] \t Time"),
			pclChar->GetName(),siBeforeGrade,siAfterGrade,szBuffer0 );
	}	
}

bool CPVP_LeagueMgr::InMatchingList( SI32 siPersonID )	// 내가 매칭 리스트에 있는지 확인 한다.
{
	return (m_clPVP_LeageMatchingList.FindPersonID( siPersonID ) > 0 );
}

SI32 CPVP_LeagueMgr::GetEnemyPersonID( SI32 siPersonID )	// 나의 적이 있는지 확인 한다
{
	return m_clPVP_LeageMatchingList.FindEnemyPersonID( siPersonID );
}

bool CPVP_LeagueMgr::InEndGameList( SI32 siPersonID )	// 내가 EndGame 리스트에 있는지 확인 한다.
{
	return ( m_clPVP_Leage_EndGameList.FindPersonID( siPersonID ) > 0);
}

// 해당 퍼슨아이디의 사람이 매칭리스트에 있고 정상위치에 있는지 시작전에 확인 하기 위해 
bool CPVP_LeagueMgr::Is_MatchingListPeron_TruePosition( SI32 siPersonID )
{
	cltServer* pclserver = (cltServer*)pclClient	;

	SI32 siId		=	pclserver->pclCM->GetIDFromPersonID(siPersonID)	;
	if( pclserver->pclCM->IsValidID( siId ) == false )		return	false	; // 요청한 사람이 정상적인 ID인가 확인
	cltCharServer* pclChar = pclserver->pclCM->GetCharServer(siId);			if(pclChar == NULL)		return	false ;

	// 시작 할때 정확한 맵위에 있는지 확인 한다.
	SI32 siMapIndex = pclChar->GetMapIndex()	;
	cltMapCommon* pclmap = pclClient->pclMapManager->GetMapPointer( siMapIndex )	;
	if ( pclmap != NULL	 )
	{
		SI32 siColl = pclmap->GetCollMap(	pclChar->GetX() , pclChar->GetY()	)	;

		if( MAPINDEX_MAINFIELD == siMapIndex ) 
		{
			if ( siColl & BLOCK_EVENT4 ) //PVP리그 맵이고 경기장 안으로 존재 하면 
			{
				return true ;
			}
		}

		else if ( MAPINDEX_PVPMAP == siMapIndex ) 
		{
			if ( siColl & BLOCK_EVENT4 ) //PVP리그 맵이고 경기장 안으로 존재 하면 
			{
				return true ;
			}
		}
	}

	return false	;
}

SI32 CPVP_LeagueMgr::GetTotalScore( SI32 siSourcePersonID, SI32 siDestPersonID )
{
	cltServer* pclserver = (cltServer*)pclClient;
	if ( NULL == pclserver )
	{
		return 0;
	}

	SI32			siSourceCharID	= pclserver->pclCM->GetIDFromPersonID( siSourcePersonID );
	cltCharServer*	pclSourceChar	= pclserver->pclCM->GetCharServer( siSourceCharID );
	if ( NULL == pclSourceChar )
	{
		return 0;
	}

	SI32			siDestCharID	= pclserver->pclCM->GetIDFromPersonID( siDestPersonID );
	cltCharServer*	pclDestChar		= pclserver->pclCM->GetCharServer( siDestCharID );
	if ( NULL == pclDestChar )
	{
		return 0;
	}

	// 특설링 경기자를 뽑기위해 각 조건들에 점수를 부여한다
	SI32 siGradeScore	= (pclSourceChar->GetPVP_LeagueGrade() + pclDestChar->GetPVP_LeagueGrade()) * 10000000;
	SI32 siLevelScore	= (pclSourceChar->pclCI->clIP.GetLevel() + pclDestChar->pclCI->clIP.GetLevel()) * 10000;
	SI32 siWinScore		= (pclSourceChar->GetPVP_LeagueStraight_Win() + pclDestChar->GetPVP_LeagueStraight_Win());

	SI32 siTotalScore	= siGradeScore + siLevelScore + siWinScore;

	return siTotalScore;
}


//PVP리그에서 그래이드 정보를 데이터파일로 부터 읽어 온다.
void CPVP_LeagueMgr::PVP_LeagueGradeInfo()
{
	NDataLoader	dataloader;
	BOOL bLoadFile=FALSE;

	//KHY - 0810 - 텍스트 리소스 암호화.
	if( pclClient->IsWhereServiceArea( ConstSwitch_TextEncryption ) )
	{
		bLoadFile = dataloader.LoadDataFromCryptFile( TEXT("Data/PVPGradeInfo.txt") );
	}
	else
	{
		bLoadFile = dataloader.LoadDataFromFile( TEXT("Data/PVPGradeInfo.txt") );
	}

	if (!bLoadFile)
	{
		MsgBox( TEXT("Not LoadFromFile PVP_League_Grade"),TEXT("Error"));
		return;
	}

	SI32 pGrade, pGradeValue,pUpWin, pDownLose;

	// pGradePostion은 그래이드 1등급 2등급 이런거 pGradeUnique는 PVP_LEAGUE_GRADE_MAX같은거, pGradeValue는 서버에서 쓰이는 값
	NDataTypeInfo pDataTypeInfo[] =
	{
		NDATA_INT32,	&pGrade,	4,
			NDATA_INT32,	&pGradeValue,	4,
			NDATA_INT32,	&pUpWin,	4,
			NDATA_INT32,	&pDownLose,	4,
			0,0,0
	};	
	dataloader.RegDataType( pDataTypeInfo );

	SI16 index = 0 ;

	while( !dataloader.IsEndOfData() ) 
	{
		if (dataloader.ReadData( true ))	
		{
			pclGradeInfo[index].m_Grade			=	pGrade;
			pclGradeInfo[index].m_GradeValue	=	pGradeValue;
			pclGradeInfo[index].m_UpWin			=	pUpWin;		// 다음 등급에 올라갈수 있는 연속 승수
			pclGradeInfo[index].m_DownLose		=	pDownLose;	// 하위 등급으로 떨어질수 있는 연속 패수
			m_siLastGradeIndex = index	;						// 마지막 그래이드의 위치를 알기 위해서
			index++;
		}
	}
}
// 현제 등급을 넣으면 다음 등급을 리턴한다.
SI32 CPVP_LeagueMgr::ChangeGrade( SI32 GradeNow, SI32 WinLoseCount, SI32 siUpDown )
{
	// GradeNow는 GetPVP_LeagueGrade()해서 나온 값으로서 리소스의 value값과 같다.그래서 value 값을 토대로 찾는다.그러면 현제 위치가
	// grade로 알수 있다.
	SI32 siGradeNow				=	0	;	// 이건 현제 이 놈의 등급
	SI32 siChangedGradevalue	=	GradeNow	;	// 기본 적으로는 원래 등급
	SI32 siUpWins	=	0	;
	SI32 siDownLose	=	0	;
	for ( SI32 siIndex = 0; siIndex <MAX_GRADECOUNT ; siIndex++) 
	{
		if( pclGradeInfo[ siIndex ].m_GradeValue == GradeNow )
		{
			siGradeNow = pclGradeInfo[ siIndex ].m_Grade	;	// 이게 이사람의 현제 등급이다. 1등급이면 1이렇게 나온다.
			siUpWins = pclGradeInfo[ siIndex ].m_UpWin	;		// 등급 상승에 필요한 연속 승수
			siDownLose = pclGradeInfo[ siIndex ].m_DownLose	;	// 등급 하락에 필요한 연속 패수
			break;
		}
	}
	switch( siUpDown )  
	{
	case PVP_LEAGUE_END_GRADEUP :
		{
			if(siUpWins == WinLoseCount)
			{
				SI32 siChangedGrade = siGradeNow - 1	;
				if( siChangedGrade > 0 )
				{
					siChangedGradevalue	=	pclGradeInfo[ siChangedGrade ].m_GradeValue	;// 한등급 올린다. 
				}
				else	// 0과 같거나 같으므로 더이상 등급은 오를수 없다 그러니까 기존 등급을 넣어 준다.
				{
					siChangedGradevalue	=	pclGradeInfo[ siGradeNow ].m_GradeValue	;
				}				
			}
		}
		break;
	case PVP_LEAGUE_END_GRADEDOWN :
		{
			if(siDownLose == WinLoseCount)
			{
				SI32 siChangedGradeindex = siGradeNow + 1	;
				if( siChangedGradeindex < m_siLastGradeIndex && siChangedGradeindex != 0  )
				{
					siChangedGradevalue	=	pclGradeInfo[ siChangedGradeindex ].m_GradeValue	;	// 한등급 내린다.
				}
				else	// 0과 같거나 같으므로 더이상 등급은 오를수 없다 그러니까 기존 등급을 넣어 준다.
				{
					siChangedGradevalue	=	pclGradeInfo[ m_siLastGradeIndex ].m_GradeValue	;
				}
			}
		}
		break;	
	}
	return siChangedGradevalue	;
}

SI32 CPVP_LeagueMgr::GetMyGradeFromGradeValue( SI32 GradeValue)
{
	SI32 siGradeNow	=	0;
	for ( SI32 siGrade = 0 ; siGrade <MAX_GRADECOUNT ; siGrade++) 
	{
		if( pclGradeInfo[ siGrade ].m_GradeValue == GradeValue )
		{
			siGradeNow = pclGradeInfo[ siGrade ].m_Grade	;	// 이게 이사람의 현제 등급이다. 1등급이면 1이렇게 나온다.
		}
	}
	return siGradeNow	;
}