#include "Char/CharManager/CharManager.h"
#include "ServerEvent_Manager.h"
#include "MsgType-System.h"
#include "../../DBManager/GameDBManager_World/DBMsg-System.h"
#include "../../DBManager/GameDBManager_World/DBMsg-item.h"

extern cltCommonLogic* pclClient;



CNServerEvent_Manager::CNServerEvent_Manager(void)
{
	m_siBonusBufType				= 0;							
	m_siMsgFlag						= 0;
	m_ServerEventBonusMap.ZeroMem();
	m_InstanceMapDailyOpenCount		= 0;				
	m_BackUpDailyServerUniqueRank.ZeroMem();
	ZeroMemory( &m_clServerEvent_Rank_Record, sizeof(m_clServerEvent_Rank_Record));
	m_siServerEventState			= SERVER_EVENT_STATE_READY;
	m_siGoonzuDayDayEventScore		= 0;


	m_siGoonzuDayDayEventScore		= 0;
}

CNServerEvent_Manager::~CNServerEvent_Manager()
{

}

void CNServerEvent_Manager::Init(void)
{
	for(SI32 Count = 0; Count < SERVER_EVENT_BONUS_MAP; ++Count)
	{
		m_ServerEventBonusMap[Count] = SERVER_EVENT_BONUS_MAP_START_INDEX + Count;
	}
}

void CNServerEvent_Manager::Clear(void)
{
	ZeroMemory(this, sizeof(*this));
}

void CNServerEvent_Manager::Destroy(void)
{

}


void CNServerEvent_Manager::InstanceMapClear(void)
{
	SendDBMsgServerDailyScoreAdd();	// 하루 누적 점수에 점수 증가.
	InstanceMapClearReward();		// 인던 종료 시 보상.
}

void CNServerEvent_Manager::InstanceMapClearReward(void)
{
	SI32 RewardItemUnique = 0;
	if( 7 == GetInstanceMapStepCount(0) &&
		6 == GetInstanceMapStepCount(1) &&
		8 == GetInstanceMapStepCount(2) &&
		9 == GetInstanceMapStepCount(3)		)		
	{
		//return _7_6_8_9;
		
		RewardItemUnique = ITEMUNIQUE(3897);
	}
	else if( 11 == GetInstanceMapStepCount(0) &&
			 12 == GetInstanceMapStepCount(1) &&
			 13 == GetInstanceMapStepCount(2) &&
			 14 == GetInstanceMapStepCount(3)		)
	{
		//return _11_12_13_14;
		RewardItemUnique = ITEMUNIQUE(2765);
	}
	else if(  7 == GetInstanceMapStepCount(0) &&
			 10 == GetInstanceMapStepCount(1) &&
			  7 == GetInstanceMapStepCount(2) &&
			 10 == GetInstanceMapStepCount(3)		)
	{
		//return _7_10_7_10;
		RewardItemUnique = ITEMUNIQUE(2740);
	}
	else if( 6 == GetInstanceMapStepCount(0) &&
			 8 == GetInstanceMapStepCount(1) &&
			 2 == GetInstanceMapStepCount(2) &&
			 9 == GetInstanceMapStepCount(3)		)
	{
		//return _6_8_2_9;
		if( pclClient->IsWhereServiceArea(ConstServiceArea_Korea) )		
			RewardItemUnique = ITEMUNIQUE(2770);
		else
			RewardItemUnique = ITEMUNIQUE(3911);
	}
	else if(  9 == GetInstanceMapStepCount(0) &&
			 10 == GetInstanceMapStepCount(1) &&
			  9 == GetInstanceMapStepCount(2) &&
			 10 == GetInstanceMapStepCount(3)		)
	{
		//return _9_10_9_10;
		RewardItemUnique = ITEMUNIQUE(7860);
	}

	if(60 <= GetInstanceMapStepCountSum()) 
	{
		//return COUNT_60;
		RewardItemUnique = ITEMUNIQUE(2756);
	}
	if(0 < RewardItemUnique)
	{		
		pclClient->pclCM->GiveItem(0, RewardItemUnique, 1, GIVEITEM_MODE_ALL, INPUTITEMPERSON_OPTION_SERVER_EVENT, 0, 0, 20);

		SI16 Data[4] = { GetInstanceMapStepCount(0),
						 GetInstanceMapStepCount(1),
						 GetInstanceMapStepCount(2),
						 GetInstanceMapStepCount(3)		};

		cltGameMsgResponse_Server_Event_InstanceMap_Bonus_Figure cltInfo(Data);
		cltMsg clMsg(GAMEMSG_RESPONSE_SERVER_EVENT_INSTANCEMAP_BONUS_FIGURE, sizeof(cltInfo), (BYTE*)&cltInfo);
		pclClient->pclCM->SendAllPersonMsg((sPacketHeader*)&clMsg);

		if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
		{
			// [로그추가 : 황진성 2008. 7. 7] 서버 이벤트
			// param1 = 인던 1번 단계 수.
			// param2 = 인던 2번 단계 수.
			// param3 = 인던 3번 단계 수.
			// param4 = 인던 4번 단계 수.
			// param5 = 아이템 유니크.
			cltServer * pclServer = (cltServer*)pclClient;
			pclServer->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_SERVER_EVENT, LOGCOMMAND_INDEX_SERVER_EVENT_DAILY_BONUS_ITEM, 
											0, NULL, NULL, 0, 0, 0, NULL, 0, 0, 0, 0, 0, 0, 
											GetInstanceMapStepCount(0), GetInstanceMapStepCount(1), 
											GetInstanceMapStepCount(2), GetInstanceMapStepCount(3),
											RewardItemUnique, NULL, NULL);
		}
	}

	pclClient->pclLog->FilePrint( "config\\ServerEvent.log" , "[IndunBonusItem]\t %d \t IndunCount:%d, %d, %d, %d",  
																RewardItemUnique, 
																GetInstanceMapStepCount(0), GetInstanceMapStepCount(1), 
																GetInstanceMapStepCount(2), GetInstanceMapStepCount(3));

	// 정보 삭제.
	m_clInstanceMapInfo.Clear();
}

void CNServerEvent_Manager::SendDBMsgServerEventEnd(void)
{
	// 서버 이벤트로 관련된 정보 요청.
	sDBRequest_Server_Event_Rank_Info clMsg(pclClient->siServerUnique, SERVER_EVENT_REASON_END);
	((cltServer *)pclClient)->pclGameDBProcess->SendMsg( (sPacketHeader *)&clMsg );
}

void CNServerEvent_Manager::SendDBMsgServerEventDaily(void)
{
	// 서버 이벤트로 관련된 정보 요청.
	sDBRequest_Server_Event_Rank_Info clMsg(pclClient->siServerUnique, SERVER_EVENT_REASON_DAILRY);
	((cltServer *)pclClient)->pclGameDBProcess->SendMsg( (sPacketHeader *)&clMsg );
}

void CNServerEvent_Manager::SendDBMsgServerEventRequest(void)
{
	// 서버 이벤트로 관련된 정보 요청.
	sDBRequest_Server_Event_Rank_Info clMsg(pclClient->siServerUnique, SERVER_EVENT_REASON_INFO_REQUEST);
	((cltServer *)pclClient)->pclGameDBProcess->SendMsg( (sPacketHeader *)&clMsg );
}

void CNServerEvent_Manager::SendDBMsgServerRankRequest(void)
{
	sDBRequest_Server_Event_Server_Info clMsg(pclClient->siServerUnique);
	((cltServer *)pclClient)->pclGameDBProcess->SendMsg( (sPacketHeader *)&clMsg );
}

void CNServerEvent_Manager::SendDBMsgPersonalRankRequest(void)
{
	sDBRequest_Server_Event_Personal_Info clMsg(pclClient->siServerUnique);
	((cltServer *)pclClient)->pclGameDBProcess->SendMsg( (sPacketHeader *)&clMsg );
}

void CNServerEvent_Manager::SendDBMsgServerScoreAdd(void)
{
	SI32 MyServerDailyRank = -1;
	for(SI32 Count = 0; Count < SERVER_MAX_NUM; ++Count)
	{
		if(pclClient->siServerUnique == GetBackUpDailyServerUniqueRank(Count))
		{
			MyServerDailyRank = Count;
		}
	}
				
	SI64 siServerScoreLast   = 0;	// 서버에 추가할 최종 점수.
	//SI32 ServerScoreMultiply = 1;	// 서버점수 추가시 특정 조건을 만족하면 등수에 따라 변해 곱해지는 값.
	
	// 서버가 순위에 없으면 점수는 0점.
	//if(-1 == MyServerDailyRank)
	//{
	//	siServerScoreLast = 0;
	//}
	//else
	//{
	//	ServerScoreMultiply = SERVER_MAX_NUM - MyServerDailyRank;
	//	siServerScoreLast   = 5 * ServerScoreMultiply;
	//	
	//	// 서버가 순위에 있어도 하루 누적 점수가 0점이 최종 점수도 0점 이다.
	//	if(0 >= m_clServerEvent_Rank_Record.GetServerDailyRankInfo(MyServerDailyRank)->GetScore())
	//		siServerScoreLast = 0;
	//}

	// 군주 데이 이벤트로 바뀌면서 실재 점수를 넣어주자.
	siServerScoreLast = m_clServerEvent_Rank_Record.GetServerDailyRankInfo(MyServerDailyRank)->GetScore();

	sDBRequest_Server_Event_Server_Score_Add clMsg(pclClient->siServerUnique, GetInstanceMapDailyOpenCount(), siServerScoreLast);
	((cltServer *)pclClient)->pclGameDBProcess->SendMsg( (sPacketHeader *)&clMsg );

	if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
	{
		// [로그추가 : 황진성 2007. 10. 9] // 마을 이벤트 성공.
		// param1 = 하루 개최 수.
		// param2 = 서버 총 점수에 추가 점수.
		cltServer * pclServer = (cltServer*)pclClient;
		pclServer->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_SERVER_EVENT, LOGCOMMAND_INDEX_SERVER_EVENT_TOTAL_SCORE, 
										0, NULL, NULL, 0, 0, 0, NULL, 0, 0, 0, 0, 0, 0, 
										GetInstanceMapDailyOpenCount(), siServerScoreLast, 0, 0, 0, NULL, NULL);
	}
	pclClient->pclLog->FilePrint( "config\\ServerEvent.log" , "[OndDayOpenCount]\t %d, \t [AllScore]\t %I64d",
																GetInstanceMapDailyOpenCount(), siServerScoreLast);

	// 인던 하루 개최 횟수.
	m_InstanceMapDailyOpenCount = 0;
}

void CNServerEvent_Manager::SendDBMsgServerDailyScoreAdd(void)
{
	//SI32 MyServerRank = -1;
	//for(SI32 Count = 0; Count < SERVER_MAX_NUM; ++Count)
	//{
	//	if(pclClient->siServerUnique == m_clServerEvent_Rank_Record.GetServerRankInfo(Count)->GetServerUnique())
	//	{
	//		MyServerRank = Count;
	//	}
	//}

	//if(-1 == MyServerRank)
	//	MyServerRank = 0;	// 자신의 서버에 기록이 없으면 순수하게 연산에 1을 곱하기 위해.

	SI64 siServerScoreLast   = 0;	// 서버에 추가할 최종 점수.
	//SI32 ServerScoreMultiply = 1;	// 서버점수 연산에 사용(인던 단계가 모두 같으면 순위따른 차이가 생김) 
	//if( GetInstanceMapStepCount(0) == GetInstanceMapStepCount(1) ==	
	//	GetInstanceMapStepCount(2) == GetInstanceMapStepCount(3)	)
	//{
	//	ServerScoreMultiply = (MyServerRank % SERVER_MAX_NUM) + 1;
	//}

	// 인던 개최 후 누적된 점수를 조건에 맞게 연산.
	//siServerScoreLast = GetInstanceMapScore() * ServerScoreMultiply;
	
	//sDBRequest_Server_Event_Server_Daily_Score_Add clMsg(pclClient->siServerUnique, siServerScoreLast, m_InstanceMapDailyOpenCount, SERVER_EVENT_REASON_INSTANCE_MAP_CLEAR);
	//((cltServer *)pclClient)->pclGameDBProcess->SendMsg( (sPacketHeader *)&clMsg );


	//***********************************************************************************************
	//	
	//	군주 데이 이벤트로 변경된 후
	//
	//***********************************************************************************************

	siServerScoreLast = GetInstanceMapScore();	// 더해질 점수.

	// 인던 개최 후 누적된 점수를 더하기.
	sDBRequest_Server_Event_Server_Daily_Score_Add clMsg(pclClient->siServerUnique, GetInstanceMapScore(), m_InstanceMapDailyOpenCount, SERVER_EVENT_REASON_INSTANCE_MAP_CLEAR);
	((cltServer *)pclClient)->pclGameDBProcess->SendMsg( (sPacketHeader *)&clMsg );

	// 인던 개최 후 누적된 점수를 더했으므로 0으로 초기화 하자.
	SetInstanceMapScore( 0 );

	if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
	{
		// [로그추가 : 황진성 2007. 10. 9] // 마을 이벤트 성공.
		// param1 = 인던 1번 단계 수.
		// param2 = 인던 2번 단계 수.
		// param3 = 인던 3번 단계 수.
		// param4 = 인던 4번 단계 수.
		// param5 = 하루 서버 점수에 추가 점수.
		cltServer * pclServer = (cltServer*)pclClient;
		pclServer->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_SERVER_EVENT, LOGCOMMAND_INDEX_SERVER_EVENT_DAILY_SCORE, 
										0, NULL, NULL, 0, 0, 0, NULL, 0, 0, 0, 0, 0, 0, 
										GetInstanceMapStepCount(0), GetInstanceMapStepCount(1), 
										GetInstanceMapStepCount(2), GetInstanceMapStepCount(3), 
										siServerScoreLast, NULL, NULL);
	}

	pclClient->pclLog->FilePrint( "config\\ServerEvent.log" , "[OneDayScore]\t %I64d \t IndunCount:%d, %d, %d, %d \t ServerUnique:%d",  
																siServerScoreLast, 
																GetInstanceMapStepCount(0), GetInstanceMapStepCount(1), 
																GetInstanceMapStepCount(2), GetInstanceMapStepCount(3),
																pclClient->siServerUnique );
}

void CNServerEvent_Manager::ManagerExcute( SI32 siCommandType )
{
	if ( (SERVER_EVENT_COMMANDTYPE_NONE >= siCommandType) || (SERVER_EVENT_COMMANDTYPE_END <= siCommandType) )
	{
		return;
	}

	switch ( siCommandType ) 
	{
		case SERVER_EVENT_COMMANDTYPE_QUEST_START:
			{
			}
			break;

		case SERVER_EVENT_COMMANDTYPE_QUEST_END:
			{
			}
			break;

		case SERVER_EVENT_COMMANDTYPE_INDUN_START:
			{
				// 인던 시작으로 개최 횟수 증가.
				AddInstanceMapDailyOpenCount(1);

				SetServerEventState( SERVER_EVENT_STATE_START_INDUN );
			}
			break;

		case SERVER_EVENT_COMMANDTYPE_INDUN_END:
			{
				// 인던 종료.
				InstanceMapClear();

				SetServerEventState( SERVER_EVENT_STATE_END_INDUN );
			}
			break;
	}
}

bool CNServerEvent_Manager::IsMyServerRankBest(void)
{
	// 1등 서버의 유니크가 내 서버라면.
	if(pclClient->siServerUnique == GetServerUnique(0))
		return true;
	
	return false;
}

bool CNServerEvent_Manager::IsMyServerRankBest2(void)
{
	// 2등 서버의 유니크가 내 서버라면.
	if(pclClient->siServerUnique == GetServerUnique(1))
		return true;

	return false;
}

void CNServerEvent_Manager::SetBonusBufType(SI16 _siBonusBufType)
{
	if(SERVER_EVENT_BUF_NO > _siBonusBufType || SERVER_EVENT_BUF_NUM <= _siBonusBufType)
		return;
	
	m_siBonusBufType = _siBonusBufType;
}

SI16 CNServerEvent_Manager::GetBonusBufType(void)
{
	return m_siBonusBufType;
}

void CNServerEvent_Manager::MyServerDailyRankBestBuf(void)
{
	// 적용 버프타입 결정.
	m_siBonusBufType = ( rand() % SERVER_EVENT_BUF_ETC ) + 1;
		
	sDBRequest_Server_Event_Server_Buf_Kind clMsg(pclClient->siServerUnique, m_siBonusBufType);
	((cltServer *)pclClient)->pclGameDBProcess->SendMsg( (sPacketHeader *)&clMsg );
}
void CNServerEvent_Manager::MyServerDailyRankBestBufNo(void)
{
	m_siBonusBufType = SERVER_EVENT_BUF_NO;

	sDBRequest_Server_Event_Server_Buf_Kind clMsg(pclClient->siServerUnique, m_siBonusBufType);
	((cltServer *)pclClient)->pclGameDBProcess->SendMsg( (sPacketHeader *)&clMsg );
}

SI32 CNServerEvent_Manager::GetBonusMapIndex(SI32 _siLevel)
{
	if(_siLevel < 20)
	{
		return SERVER_EVENT_BONUS_MAP_START_INDEX;
	}	
	else if(_siLevel < 40)
	{
		return SERVER_EVENT_BONUS_MAP_START_INDEX + 1;
	}
	else if(_siLevel < 60)
	{
		return SERVER_EVENT_BONUS_MAP_START_INDEX + 2;
	}
	else if(_siLevel < 80)
	{
		return SERVER_EVENT_BONUS_MAP_START_INDEX + 3;
	}
	else if(_siLevel <= 100)
	{
		return SERVER_EVENT_BONUS_MAP_START_INDEX + 4;
	}

	// 아무것도 아니면 무조건 레벨이 제일 높은 인던 입장.(올수가 있나? 레벨 풀리면...ㅋㅋ)
	return SERVER_EVENT_BONUS_MAP_START_INDEX + 4;
}

bool CNServerEvent_Manager::IsEventMap(SI32 _siMapIndex)
{
	if(SERVER_EVENT_BONUS_MAP_START_INDEX <= _siMapIndex &&
	   SERVER_EVENT_BONUS_MAP_START_INDEX + SERVER_EVENT_BONUS_MAP > _siMapIndex)
	   return true;

	return false;
}


CNServerEvent_Rank_Record* CNServerEvent_Manager::GetServerEventRankRecord(void)
{
	return &m_clServerEvent_Rank_Record;
}

CNServerEvent_Quest* CNServerEvent_Manager::GetServerEvent_Quest(void)
{
	return &m_clServerEvent_Quest;
}

CNInstanceMapInfo* CNServerEvent_Manager::GetInstanceMapInfo(void)
{
	return &m_clInstanceMapInfo;
}

void CNServerEvent_Manager::SetServerEventRankRecord(CNServerEvent_Rank_Record* _pRankInfo)
{
	SAFE_POINTER(_pRankInfo)

	memcpy(&m_clServerEvent_Rank_Record, _pRankInfo, sizeof(m_clServerEvent_Rank_Record));
}

void CNServerEvent_Manager::SetServerRank(CNServerRankInfo* _pRankInfo)
{
	SAFE_POINTER(_pRankInfo)

	memcpy(m_clServerEvent_Rank_Record.GetServerRankInfo(0), _pRankInfo, sizeof(CNServerRankInfo) * SERVER_MAX_NUM);
}

void CNServerEvent_Manager::SetServerDailyRank(CNServerDailyRankInfo* _pRankInfo)
{
	SAFE_POINTER(_pRankInfo)

	memcpy(m_clServerEvent_Rank_Record.GetServerDailyRankInfo(0), _pRankInfo, sizeof(CNServerDailyRankInfo) * SERVER_MAX_NUM);
}

void CNServerEvent_Manager::SetPersonalRank(CNPersonalRankInfo* _pRankInfo)
{
	SAFE_POINTER(_pRankInfo)

	memcpy(m_clServerEvent_Rank_Record.GetPersonalRankInfo(0), _pRankInfo, sizeof(CNPersonalRankInfo) * USER_MAX_NUM);
}

void CNServerEvent_Manager::SetServerUnique(SI32 _siServerRank, SI16 _siServerUnique) 
{
	m_clServerEvent_Rank_Record.GetServerRankInfo(_siServerRank)->SetScore(_siServerUnique);
}
SI16 CNServerEvent_Manager::GetServerUnique(SI32 _siServerRank)
{
	return m_clServerEvent_Rank_Record.GetServerRankInfo(_siServerRank)->GetServerUnique();
}

// 서버점수 처리.
void CNServerEvent_Manager::SetServerScore(SI32 _siServerRank, SI64 _siServerScore)
{
	m_clServerEvent_Rank_Record.GetServerRankInfo(_siServerRank)->SetScore(_siServerScore);
}	
SI64 CNServerEvent_Manager::GetServerScore(SI32 _siServerRank)
{
	return m_clServerEvent_Rank_Record.GetServerRankInfo(_siServerRank)->GetScore();
}

// 사냥터 개최 숫자 처리.
void CNServerEvent_Manager::SetServerHuntingGroundCount(SI32 _siServerRank, SI16 _siServerHuntingGroundCount)
{
	m_clServerEvent_Rank_Record.GetServerRankInfo(_siServerRank)->SetHuntingGroundCount(_siServerHuntingGroundCount);
}
SI16 CNServerEvent_Manager::GetServerHuntingGroundCount(SI32 _siServerRank)
{
	return m_clServerEvent_Rank_Record.GetServerRankInfo(_siServerRank)->GetHuntingGroundCount();
}

// 서버이름 처리.
void CNServerEvent_Manager::SetServerName(SI32 _siServerRank, TCHAR* _siServerName)
{
	m_clServerEvent_Rank_Record.GetServerRankInfo(_siServerRank)->SetServerName(_siServerName);
}
TCHAR* CNServerEvent_Manager::GetServerName(SI32 _siServerRank)
{
	return m_clServerEvent_Rank_Record.GetServerRankInfo(_siServerRank)->GetServerName();
}

void CNServerEvent_Manager::SetServerUniqueDaily(SI32 _siServerRank, SI16 _siServerUnique)
{
	m_clServerEvent_Rank_Record.GetServerDailyRankInfo(_siServerRank)->SetScore(_siServerUnique);
}
SI16 CNServerEvent_Manager::GetServerUniqueDaily(SI32 _siServerRank)
{
	return m_clServerEvent_Rank_Record.GetServerDailyRankInfo(_siServerRank)->GetServerUnique();
}

// 서버점수 처리.
void CNServerEvent_Manager::SetServerDailyScore(SI32 _siServerRank, SI64 _siServerScore)
{
	m_clServerEvent_Rank_Record.GetServerDailyRankInfo(_siServerRank)->SetScore(_siServerScore);
}
SI64 CNServerEvent_Manager::GetServerDailyScore(SI32 _siServerRank)
{
	return m_clServerEvent_Rank_Record.GetServerDailyRankInfo(_siServerRank)->GetScore();
}

// 사냥터 개최 숫자 처리.
void CNServerEvent_Manager::SetServerDailyHuntingGroundCount(SI32 _siServerRank, SI16 _siServerHuntingGroundCount)
{
	m_clServerEvent_Rank_Record.GetServerDailyRankInfo(_siServerRank)->SetHuntingGroundCount(_siServerHuntingGroundCount);
}
SI16 CNServerEvent_Manager::GetServerDailyHuntingGroundCount(SI32 _siServerRank)
{
	return m_clServerEvent_Rank_Record.GetServerDailyRankInfo(_siServerRank)->GetHuntingGroundCount();
}

// 서버이름 처리.
void CNServerEvent_Manager::SetServerDailyName(SI32 _siServerRank, TCHAR* _siServerName)
{
	m_clServerEvent_Rank_Record.GetServerDailyRankInfo(_siServerRank)->SetServerName(_siServerName);
}
TCHAR* CNServerEvent_Manager::GetServerDailyName(SI32 _siServerRank)
{
	return m_clServerEvent_Rank_Record.GetServerDailyRankInfo(_siServerRank)->GetServerName();
}



// 개인점수 처리. 
void CNServerEvent_Manager::SetPersonalScore(SI32 _siPersonalRank, SI64 _siPersonalScore)
{
	return m_clServerEvent_Rank_Record.GetPersonalRankInfo(_siPersonalRank)->SetScore(_siPersonalScore);
}
SI64 CNServerEvent_Manager::GetPersonalScore(SI32 _siPersonalRank)
{
	return m_clServerEvent_Rank_Record.GetPersonalRankInfo(_siPersonalRank)->GetScore();
}

// 개인레벨 처리.
void CNServerEvent_Manager::SetPersonalLevel(SI32 _siPersonalRank, SI16 _siPersonalLevel)
{
	return m_clServerEvent_Rank_Record.GetPersonalRankInfo(_siPersonalRank)->SetLevel(_siPersonalLevel);
}
SI16 CNServerEvent_Manager::GetPersonalLevel(SI32 _siPersonalRank)
{
	return m_clServerEvent_Rank_Record.GetPersonalRankInfo(_siPersonalRank)->GetLevel();
}

// 개인 이름.
void CNServerEvent_Manager::SetPersonalName(SI32 _siPersonalRank, TCHAR* _siPersonalName)
{
	m_clServerEvent_Rank_Record.GetPersonalRankInfo(_siPersonalRank)->SetPersonalName(_siPersonalName);
}
TCHAR* CNServerEvent_Manager::GetPersonalName(SI32 _siPersonalRank)
{
	return m_clServerEvent_Rank_Record.GetPersonalRankInfo(_siPersonalRank)->GetPersonalName();
}


void CNServerEvent_Manager::SetInstanceMapScore(SI64 _siScore)
{
	m_clInstanceMapInfo.SetScore(_siScore);
}

SI64 CNServerEvent_Manager::GetInstanceMapScore(void)
{
	return m_clInstanceMapInfo.GetScore();
}
void CNServerEvent_Manager::AddInstanceMapScore(SI64 _siAddScore)
{
	m_clInstanceMapInfo.AddScore(_siAddScore);
}

void CNServerEvent_Manager::SetInstanceMapStepCount(SI16 _Index, SI16 _siMapStepCount)
{
	m_clInstanceMapInfo.SetMapStepCount(_Index, _siMapStepCount);
}

SI16 CNServerEvent_Manager::GetInstanceMapStepCount(SI16 _Index)
{
	return m_clInstanceMapInfo.GetMapStepCount(_Index);
}

void CNServerEvent_Manager::AddInstanceMapStepCount(SI16 _Index, SI16 _siOpenCount)
{
	m_clInstanceMapInfo.AddMapStepCount(_Index, _siOpenCount);
}

SI16 CNServerEvent_Manager::GetInstanceMapStepCountSum(void)
{
	return m_clInstanceMapInfo.GetMapStepCountSum();
}

void CNServerEvent_Manager::SetInstanceMapDailyOpenCount(SI32 _siOpenCount)
{
	m_InstanceMapDailyOpenCount = _siOpenCount;
}
SI32 CNServerEvent_Manager::GetInstanceMapDailyOpenCount(void)
{
	return m_InstanceMapDailyOpenCount;
}
void CNServerEvent_Manager::AddInstanceMapDailyOpenCount(SI32 _siOpenCount)
{ 
	m_InstanceMapDailyOpenCount += _siOpenCount;
}

void CNServerEvent_Manager::SetInstanceMapClear(SI16 _Index, bool _bMapClear)
{
	m_clInstanceMapInfo.SetMapClear(_Index, _bMapClear);
}
bool CNServerEvent_Manager::GetInstanceMapClear(SI16 _Index)
{
	return m_clInstanceMapInfo.GetMapClear(_Index);
}
bool CNServerEvent_Manager::IsAllInstanceMapClear(void)
{
	return m_clInstanceMapInfo.IsAllMapClear();
}

void CNServerEvent_Manager::BackUpDailyServerUniqueRankClear(void)
{
#ifdef _SAFE_MEMORY
	m_BackUpDailyServerUniqueRank.ZeroMem();
#else
	ZeroMemory(m_BackUpDailyServerUniqueRank, sizeof(m_BackUpDailyServerUniqueRank));
#endif
}
void CNServerEvent_Manager::SetBackUpDailyServerUniqueRank(void)
{
	for(SI32 Count = 0; Count < SERVER_MAX_NUM; ++Count)
	{
		m_BackUpDailyServerUniqueRank[Count] = m_clServerEvent_Rank_Record.GetServerDailyRankInfo(Count)->GetServerUnique();
	}
}
SI16 CNServerEvent_Manager::GetBackUpDailyServerUniqueRank(SI16 _Index)
{
	return m_BackUpDailyServerUniqueRank[_Index];
}

void CNServerEvent_Manager::SetServerEventState( SI32 _siServerEventState )
{
	if ( (_siServerEventState <= SERVER_EVENT_STATE_NONE) || (_siServerEventState >= SERVER_EVENT_STATE_END) )
	{
		return;
	}

	m_siServerEventState = _siServerEventState;
}
void CNServerEvent_Manager::DeleteGate( void )
{
	// 생성되있는 게이트를 삭제.
	m_clInstanceMapInfo.DeleteGate();
}

void CNServerEvent_Manager::ServerEventStateReady( void )
{
	m_siServerEventState = SERVER_EVENT_STATE_READY;

	m_clServerEvent_Quest.Init();

	// 생성되있는 게이트를 삭제.
	m_clInstanceMapInfo.DeleteGate();
	m_clInstanceMapInfo.Clear();
}

SI32 CNServerEvent_Manager::GetServerEventState( void )
{
	return m_siServerEventState;
}

void CNServerEvent_Manager::ServerEventStateProcess( void )
{
	// 대기상태는 처리하기 않고 그냥 리턴
	if ( SERVER_EVENT_STATE_WAIT  ==  m_siServerEventState )
	{
		return;
	}

	switch ( m_siServerEventState )
	{
		case SERVER_EVENT_STATE_READY:
			{
				m_clServerEvent_Quest.SendMessageToDB( SERVEREVENT_QUEST_PACKETTYPE_GET_QUESTINFO );
				SetServerEventState( SERVER_EVENT_STATE_WAIT );
			}
			break;

		case SERVER_EVENT_STATE_START_QUEST:
			{
				cltServer* pclserver = (cltServer*)pclClient;
				if ( NULL == pclserver )
				{
					return;
				}

				if ( pclserver->pclEventTimeManager->InEventTime(TEXT( "GoonzuDay_Week" ), &pclserver->sTime) )
				{
					m_clServerEvent_Quest.StartServerEventQuest();
				}
				else
				{
					SetServerEventState( SERVER_EVENT_STATE_WAIT );
				}
			}
			break;

		case SERVER_EVENT_STATE_GOING_QUEST:
			{
				m_clServerEvent_Quest.Action();
			}
			break;

		case SERVER_EVENT_STATE_END_QUEST:
			{
			}
			break;
			
		case SERVER_EVENT_STATE_START_INDUN:
			{
				m_clInstanceMapInfo.OpenTheGate();
			}
			break;

		case SERVER_EVENT_STATE_GOING_INDUN:
			{
				m_clInstanceMapInfo.Action();
			}
			break;

		case SERVER_EVENT_STATE_END_INDUN:
			{
				m_clInstanceMapInfo.CloseTheGate();
				m_clInstanceMapInfo.LeaveLobbyInUser();
			}
			break;

	}

}

//******************************************************************************************
//
// 서버 이벤트가 군주 데이 이벤트로 변경되어 사용되는 함수. 
// * 서버 이벤트는 사라짐.
//
//******************************************************************************************
bool CNServerEvent_Manager::IsGoonzuDayEventSuccess( void )
{

	return true;
}


void CNServerEvent_Manager::ActionGoonzuDayReward(void)
{
	// 서버에서만 실행
	if ( GAMEMODE_SERVER != pclClient->GameMode )	return;

	// [영훈] 서버 이벤트관련 로직이 완벽하게 종료하기 위해 이곳에도 넣는다
	ServerEventStateProcess();

	// 5분 마다 DB에 서버 이벤트 정보 요청.
	static DWORD StartTickCount = GetTickCount();
	if(20000 < GetTickCount() - StartTickCount)
	{
		StartTickCount = GetTickCount();

		SendDBMsgServerEventRequest();
	}
}

void CNServerEvent_Manager::ActionGoonzuDay(void)
{
	// 서버에서만 실행
	if ( GAMEMODE_SERVER != pclClient->GameMode )	return;

	ServerEventStateProcess();

	cltServer* pclServer = (cltServer*)pclClient;

	// 5분 마다 DB에 서버 이벤트 정보 요청.
	static DWORD StartTickCount = GetTickCount();
	if(20000 < GetTickCount() - StartTickCount)
	{
		StartTickCount = GetTickCount();

		SendDBMsgServerEventRequest();

		SendDBMsgServerDailyScoreAdd();	// 하루 누적 점수에 점수 증가.
	}
}