//Switch_10MonthEvent_RacoonDodge
#include "RacoonDodgeEvent.h"
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
#include "../../DBManager/GameDBManager_World/DBMsg-Person.h"
#include "../../CommonLogic/MsgType-Person.h"
#include "../../CommonLogic/Msg/MsgType-RacoonDodgeEvent.h"
#include "../../CommonLogic/MsgRval-Define.h"

#include "MsgType-System.h"

ReserveData::ReserveData()
{
	init();
}
ReserveData::~ReserveData()
{

}
void ReserveData::init()
{
	ZeroMemory(this , sizeof(ReserveData))	;

}
SI32 ReserveData::GetCharPersonID()
{
	return PersonID	;
}
TCHAR* ReserveData::GetCharName()
{
	return m_szCharName	;
}
void ReserveData::SetCharPersonID(SI32 siPersonID ) // 메인 루틴이 아닌거의 구색마추기
{
	PersonID	= siPersonID ;
}
void ReserveData::SetCharName(TCHAR* charName)
{
	if ( charName ) StringCchCopy( m_szCharName, sizeof(m_szCharName), charName );
}
//=====================================================================================================
CRacoonDodgeEventMgr::CRacoonDodgeEventMgr()
{
	init()	;
}
CRacoonDodgeEventMgr::~CRacoonDodgeEventMgr()
{

}
void CRacoonDodgeEventMgr::init()
{
	for ( SI32 i = 0; i < MAX_RESERVER_NUM; i++)
	{
		pclReserveList[i].init()	;
	}

	for ( SI32 i = 0; i < EVENT_START_NUM*2; i++)
	{
		pclSelectPersonList[i].init()	;
	}
	
	for ( SI32 i = 0; i < MAX_MONSTER_NUM; i++)
	{
		pclMonsterUnique[i] = 0 ;
	}
	for ( SI32 i = 0; i < MAX_MAKE_MONSTER_NUM; i++)
	{
		MakeMonsterArray[i] = 0 ;
	}
	siWaitTime5Second	=	0;
	siMonsterKindNum	=	0;

	siGameStep			=	EVENTSTEP_WAIT;
	siGameLevel			=	EVENTLEVEL_WAIT;
	
	bLeadData			=	false	;
	siCheckFieldNum		=	0	;
	bCheck_1stMakeMonster	=	false	;

	SetRoundTime(30)	;

	dwCount_30Second	=	0	;
}

void CRacoonDodgeEventMgr::ClearData()
{
	cltServer* pclserver = (cltServer*)pclClient;

	for ( SI32 i = 0; i < EVENT_START_NUM*2; i++)
	{
		// 데이터 클리어 전에 메세지 클리어하라고 쏴준다 
		SI32 siPersonD_InClearData  = pclSelectPersonList[i].GetCharPersonID() ;
		SI32 siID = pclserver->pclCM->GetIDFromPersonID( siPersonD_InClearData)	;
		cltCharServer* pclCharServer = pclserver->pclCM->GetCharServer( siID ) ;	
		if(pclCharServer != NULL )
		{
			cltGameMsgResponse_30Seceond_Clear clinfo( siPersonD_InClearData );
			cltMsg clMsg3(GAMEMSG_RESPONS_RACOONDODGE_EVENT_30SECOND_CLEAR, sizeof(clinfo), (BYTE*)&clinfo);
			pclCharServer->SendNetMsg((sPacketHeader*)&clMsg3);

			cltGameMsgResponse_Round_Clear clinfo2( siPersonD_InClearData );
			cltMsg clMsg2(GAMEMSG_RESPONS_RACOONDODGE_EVENT_ROUNDCOUNT_CLEAR, sizeof(clinfo2), (BYTE*)&clinfo2);
			pclCharServer->SendNetMsg((sPacketHeader*)&clMsg2);
		}
		pclSelectPersonList[i].init()	;
	}
	for ( SI32 i = 0; i < MAX_MAKE_MONSTER_NUM; i++)
	{
		MakeMonsterArray[i] = 0 ;
	}
	siWaitTime5Second	=	0;
	siGameStep			=	EVENTSTEP_WAIT;
	siGameLevel			=	EVENTLEVEL_WAIT;

	siCheckFieldNum		=	0	;
	bCheck_1stMakeMonster	=	false	;
	dwCount_30Second		=	0	;
}

void CRacoonDodgeEventMgr::Action()
{
	cltServer* pclserver = (cltServer*)pclClient;

	if ( true == pclClient->IsCountrySwitch( Switch_10MonthEvent_RacoonDodge ) )
	{
		if(pclClient->pclEventTimeManager->InEventTime("RaccoonDodge", &pclserver->sTime) == false )	return	;

		if(bLeadData	==	false	)
		{
			// 한번만 로드하면된다.
			LoadRocFile();			// 필드 좌표	
			LoadMonsterKindFile();	// 몬스터 카인드
			LoadMonsterInfoSetFile();// 어그래시브 , 무브 스피드 증가값
			bLeadData	=	true	;
		}
		switch ( siGameStep )
		{
		case EVENTSTEP_WAIT:
			{
				siCheckFieldNum = CheckPlayStageNum()	;
				if ( siCheckFieldNum < RACOONDODGEEVENT_STAGE_ONE )	return ;
				ChangeStep(EVENTSTEP_MAKE_PLAYUSERLIST)	;
			}break	;
		case EVENTSTEP_MAKE_PLAYUSERLIST:
			{
				ClearData();
				siCheckFieldNum = CheckPlayStageNum()	;
				if ( siCheckFieldNum >= RACOONDODGEEVENT_STAGE_ONE ) 
				{
					MakeSelectPersonList()	;			// 리스트를 만든다.				
					ChangeStep(EVENTSTEP_REQUEST_PLAY_EVENT_NOW)	;
				}
			}break	;
		case EVENTSTEP_REQUEST_PLAY_EVENT_NOW:
			{
				SendMessage_Request_PlayEvent()	;	// 만들어진 리스트를 통해서 참가 여부를 물어본다.
				siWaitTime5Second	=	pclserver->CurrentClock ;
				// 다음 단계로 넘기기 전에 질문 보낸 시간 저장
				ChangeStep(EVENTSTEP_WAIT_ANSWER)	;
			}break	;
		case EVENTSTEP_WAIT_ANSWER:
			{
				// 질문 보낸 시간이 10초가 넘어 가면 다음 단계로 넘긴다
				if( (pclserver->CurrentClock - siWaitTime5Second)  > 10000 )  //10초대기
				{
					ChangeStep(EVENTSTEP_GAMESTART)	;
				}				
			}break	;
		case EVENTSTEP_GAMESTART:
			{
				// 5초 카운트 시작
				Count5Second();
				//만약 을 대비해서 
				siWaitTime5Second	=	pclserver->CurrentClock ;
				ChangeStep(EVENTSTEP_WAITTIME)	;	// 이건 리스폰스 받았을떄 상태를 변경 하니까. 클라마다 약간의 차이가 있다.
			}break	;
		case EVENTSTEP_WAITTIME:
			{
				if( (pclserver->CurrentClock - siWaitTime5Second)  > 10000 )  //10초가 지났는데 게임 레벨이 대기면 처음으로 넘긴다.-다 패스 한거니까
				{
					ClearData();
				}
				// 여기에서 스타트 까지 타임을 5초간 기다린다.
			}break	;
		case EVENTSTEP_PLAY:
			{
				SI32 siTimeStart	=	0	;	// 게임이 스타트 한시간
				// 현상태에서 게임이 이루어 진다
				// 현제 진행중에 진행이 가능 한지 확인 한다.
				// 이건 상태가 웨이트 단계 이상 넘어 갔을때만 사용 한다.
				if ( GetLevel() > EVENTLEVEL_WAIT ) 
				{
					bool CanPlay = CanPlayEvent()	;
					if ( CanPlay == false )		// 단계가 넘어가는 도중 사람이 더이상 필드에 없어서 플레이가 불가능 할떄
					{
                        // 몬스터를 다 죽여 버린다
						KillEventMonster();
						ClearData();
					}
				}
				switch( siGameLevel ) 
				{
				case EVENTLEVEL_WAIT :
					{
						// 이번단계에서 할일
						// 시작 시간을 셋팅 한다.
						dwCount_30Second = pclserver->CurrentClock ;
						// 다음단계로 이동
						ChangeLevel( EVENTLEVEL_LV1 )	;
					}
					break;
				case EVENTLEVEL_LV1:
					{
						DWORD dwRoundTimeLimit = 0	;
						dwRoundTimeLimit = pclserver->CurrentClock - dwCount_30Second;
						if ( bCheck_1stMakeMonster == false )
						{
							Count30Second()	;
							RoundSet()	;

							if ( siCheckFieldNum == RACOONDODGEEVENT_STAGE_TWO  )	// 두개에다가 몬스터를 셋팅한다 
							{
								SetMonster( MAKE_MONSTER_NUM_10,RACOONDODGEEVENT_STAGE_FIELD_1ST )	;	// 처음에는 10마리를 생성
								SetMonster( MAKE_MONSTER_NUM_10,RACOONDODGEEVENT_STAGE_FILED_2ND )	;	// 처음에는 10마리를 생성
								bCheck_1stMakeMonster	=true ;
							}
							else if( siCheckFieldNum == RACOONDODGEEVENT_STAGE_ONE )
							{
								SetMonster( MAKE_MONSTER_NUM_10,RACOONDODGEEVENT_STAGE_FIELD_1ST )	;	// 처음에는 10마리를 생성
								bCheck_1stMakeMonster	=true ;
							}
						}
						///------------------------------
						// 30초가 다 됐으면 다음 단계로 이동 시킨다
						if( dwRoundTimeLimit > dwRoundTime )
						{
							SI32 siNowLevel  = GetLevel()	;
							// 다음 단계를 위해 시간을 기억 
							dwCount_3Second	=	pclserver->CurrentClock ;
							dwCount_30Second	=	pclserver->CurrentClock ;
							ChangeLevel(siNowLevel+1)	;

							Count30Second()	;
							RoundSet()	;
						}
					}break	;
				case EVENTLEVEL_LV2:
				case EVENTLEVEL_LV3:
				case EVENTLEVEL_LV4:
				case EVENTLEVEL_LV5:
				case EVENTLEVEL_LV6:
				case EVENTLEVEL_LV7:
				case EVENTLEVEL_LV8:
				case EVENTLEVEL_LV9:
				case EVENTLEVEL_LV10:
					{
						DWORD sitemp_MonsterMakeTermTime = pclserver->CurrentClock	- dwCount_3Second	;
						DWORD sitemp_NextLevelTermTime = pclserver->CurrentClock	- dwCount_30Second	;

						if ( sitemp_MonsterMakeTermTime > MAKEMOSTER_TERM) // 3초 간격으로 생성 
						{
							if ( siCheckFieldNum == RACOONDODGEEVENT_STAGE_TWO  )	// 두개에다가 몬스터를 셋팅한다 
							{
								SetMonster( MAKE_MONSTER_NUM_1,RACOONDODGEEVENT_STAGE_FIELD_1ST )	;	// 처음에는 10마리를 생성
								SetMonster( MAKE_MONSTER_NUM_1,RACOONDODGEEVENT_STAGE_FILED_2ND )	;	// 처음에는 10마리를 생성
							}
							else if( siCheckFieldNum == RACOONDODGEEVENT_STAGE_ONE )
							{
								SetMonster( MAKE_MONSTER_NUM_1,RACOONDODGEEVENT_STAGE_FIELD_1ST )	;	// 처음에는 10마리를 생성
								dwCount_3Second	=	pclserver->CurrentClock ;
							}
							if ( sitemp_NextLevelTermTime > dwRoundTime ) 
							{
								SI32 siNowLevel  = GetLevel()	;
								if ( siNowLevel == EVENTLEVEL_LV10) 
								{
									dwCount_3Second		=	0 ;
									dwCount_30Second	=	0	;
									siNowLevel	=	EVENTLEVEL_WAIT	;
									ChangeStep(EVENTSTEP_GAMEEND)	;
								}
								else
								{
									// 30초 날린다고 알린다.,
									dwCount_30Second	=	pclserver->CurrentClock ;
									ChangeLevel(siNowLevel+1)	;

									Count30Second()	;
									RoundSet()	;
								}
							}
						}
					}break	;
				}				
			}break	;
		case EVENTSTEP_GAMEEND:
			{
				// 결과에 따른 보상을 준다
				// 현제 맵안에 있는 엔피씨들을 모두 삭제 한다.
				KillEventMonster();
				RewardMoney();
				ClearData();
			}break	;
		}
	}
}
bool CRacoonDodgeEventMgr::AddReserveList(SI32 siPersonID)
{
	cltServer* pclserver = (cltServer*)pclClient;

	if(pclClient->pclEventTimeManager->InEventTime("RaccoonDodge", &pclserver->sTime) == false )	return	false ;

	bool bSuccess = false ;
	SI32 SIEmptyindex =	0	;

	SI32 siReservationCharId = pclserver->pclCM->GetIDFromPersonID( siPersonID ) ; 
	cltCharServer* pclReservationCharServer = pclserver->pclCM->GetCharServer( siReservationCharId )	;
	if(pclReservationCharServer == NULL )	return bSuccess	;
	bool bCanReserveList	=	false ;
	bCanReserveList = CanReserveList(siPersonID )	;
	if(bCanReserveList == true  )
	{
		SIEmptyindex = FindEmptyPosiotion()	;

		TCHAR *szCharname = (TCHAR*)pclReservationCharServer->pclCI->clBI.GetName()	;

		pclReserveList[SIEmptyindex].SetCharPersonID(siPersonID)	;
		pclReserveList[SIEmptyindex].SetCharName(szCharname  )	;
		bSuccess = true ;
	}
	else
	{
		// 실패
	}
	return bSuccess ;
}
void CRacoonDodgeEventMgr::SendMessage_Request_PlayEvent( )	// 참여자를 선택해서 메세지를 보낸다.
{
	// 만들어진 리스트를 토대로 
	cltServer* pclserver = (cltServer*)pclClient;

	for ( SI32 i = 0 ; i< EVENT_START_NUM*MAX_EVENTPLACE ; i++)
	{
		SI32 siPersonID =	0	;
		SI32 siID =			0	;

		siPersonID = pclSelectPersonList[ i ].GetCharPersonID() ;	// 만들어진 선발 리스트에서 personid를 꺼낸다
		if ( siPersonID <= 0 )	// 리스트에 사람이 없다 이젠 종료
		{
			return	;
		}
		siID = pclserver->pclCM->GetIDFromPersonID( siPersonID ) ;	// 꺼낸 personid를 통해서 charid를 찾는다.
		cltCharServer* pclCharServer = pclserver->pclCM->GetCharServer( siID )	;	// 해당 id의 char정보를 get한다.
		if( pclCharServer != NULL )
		{
			// 날라갈 꺼냐고 클라이언트로 패킷을 날린다 .

			cltGameMsgResponse_Request_PlayEvent clinfo( pclCharServer->pclCI->GetPersonID() );	// 일단 퍼슨아이디로 해놓는다
			cltMsg clMsg(GAMEMSG_RESPONS_RACOONDODGE_EVENT_REQUEST_PLAYEVENT, sizeof(clinfo), (BYTE*)&clinfo);
			pclCharServer->SendNetMsg((sPacketHeader*)&clMsg);
		}
		else
		{
			// 잘못된 char 정보 해당 케릭터는 접속해있지 않다. 
		}
	}
}
// 현제 돌고 있는 이벤트 상태 변경 
void CRacoonDodgeEventMgr::ChangeStep( SI32 siStep )
{
	siGameStep	=	siStep	;
}
void CRacoonDodgeEventMgr::ChangeLevel( SI32 siLevel )
{
	if ( siLevel >= EVENTLEVEL_LV10 )	siGameLevel	=	EVENTLEVEL_LV10	;
	siGameLevel	=	siLevel	;
}
//빈 인덱스를 찾는다 .
SI32 CRacoonDodgeEventMgr::FindEmptyPosiotion()
{
	SI32 siPersonID = 0  ;
	SI32 siEmptyindex = 0 ;

	for ( siEmptyindex = 0 ; siEmptyindex < MAX_RESERVER_NUM; siEmptyindex++)
	{
		siPersonID = pclReserveList[siEmptyindex].GetCharPersonID() ; 
		if ( siPersonID <= 0 )	// 빈곳을 찾았다~
		{
			return siEmptyindex ;
		}
	}
	return siEmptyindex;
}
void CRacoonDodgeEventMgr::Select_Participated_PersonID( SI32 siPersonID)	//선택한 사람에 대한 처리
{
	// 하는일 : 해당 퍼슨을 선택하고 ok 메세지가 오면 해당 peron을 찾아서 맵으로 워프 시킨뒤 리스트에서는 삭제
	
	SI32 siPersonIndex = FindPersonID_InList( siPersonID)	;	// 현제 해당 사람이 들어있는 인덱스
	// 워프 시킨다.
	// 이벤트 케릭터라고 셋팅 한다.
	SetEventChar( siPersonID)	;
	// 리스트에서 삭제 한다.

}
SI32 CRacoonDodgeEventMgr::FindPersonID_InList( SI32 siInputPersonID)		// 리스트에서 해당 사람을 찾아서 인덱스를 리턴 한다.
{
	SI32 siEmptyindex = 0	;
	SI32 siTempPersonID	=	0	;

	for ( siEmptyindex ; siEmptyindex < MAX_RESERVER_NUM; siEmptyindex++)
	{
		siTempPersonID = pclReserveList[siEmptyindex].GetCharPersonID() ; 

		if ( siTempPersonID == siInputPersonID ) // 그사람이 리스트의 어디에 있는지를 알았다.
		{
			return siEmptyindex ;
		}
	}
	return siEmptyindex ;    
}
bool CRacoonDodgeEventMgr::SetEventChar( SI32 siPersonID)	// 이벤트케릭터라고 셋팅 한다.
{
	cltServer* pclserver = (cltServer*)pclClient;
	bool bSuccess = false	;
	
	SI32 siEventCharId = pclserver->pclCM->GetIDFromPersonID( siPersonID )	;
	cltCharServer* pclEventChar = pclserver->pclCM->GetCharServer( siEventCharId )	;
	if( pclEventChar != NULL )
	{
		// 해당 이벤트 케릭터에게 넌 이벤트 케릭터라고 셋팅 한다 .

		bSuccess	=	true	;
	}
	else
	{
	}

	return bSuccess		;

}
void CRacoonDodgeEventMgr::MakeSelectPersonList(void )	// 선정된 사람을을 뽑아내서 리스트를 만든다 이 리스트는 예약리스트와는 다르다.
{
	bool bValue = false ; 
	SI32 siSelecrSlot = 0	;

	ReserveData			pclReserveList_Temp[MAX_RESERVER_NUM]	;				// 리스트 정리를 위한예약리스트
	ZeroMemory( pclReserveList_Temp,sizeof(pclReserveList_Temp))	;			// 템프 리스트 초기화
	SI32 siReserveListNum	=	CountReservePersonNum();

	// 이벤트 가능 인원이 차야지만 리스트 만들기를 한다.
	if(siReserveListNum >= EVENT_START_NUM)	// 예약자라 5명 이상이면 상위 5명을 뺸다.
	{
		SI32 siValue  = siReserveListNum / EVENT_START_NUM	;

		if ( siValue > 0 )
		{
			if(siValue >= MAX_EVENTPLACE)	siValue = MAX_EVENTPLACE	;
			for ( siSelecrSlot ; siSelecrSlot < EVENT_START_NUM * siValue ; siSelecrSlot++)
			{
				if ( siSelecrSlot >= EVENT_START_NUM*MAX_EVENTPLACE ) break	;

				pclSelectPersonList[siSelecrSlot].SetCharPersonID( pclReserveList[siSelecrSlot].GetCharPersonID() ) ;
				pclSelectPersonList[siSelecrSlot].SetCharName( pclReserveList[siSelecrSlot].GetCharName() );
				pclReserveList[siSelecrSlot].init()	;	// 뺏으니 비운다.
			}
			// 리스트를다 만들었으니 정리 한다

			for ( SI32 siIndex = 0 ; siIndex <MAX_RESERVER_NUM ; siIndex++) 
			{
				if ( siIndex >= MAX_RESERVER_NUM) break	;

				SI32 siTempIndex = 0 ;
				SI32 siPersonID = pclReserveList[siIndex].GetCharPersonID()	;	
				TCHAR* charName = pclReserveList[siIndex].GetCharName() ;
				if( siPersonID <= 0) continue	;
				if ( siPersonID > 0 ) 
				{
					pclReserveList_Temp[siTempIndex ].SetCharPersonID(siPersonID)	;
					pclReserveList_Temp[siTempIndex ].SetCharName(charName)	;
					pclReserveList[siIndex].init()	;	// 넣은다음 지운다
				}
			}
			// 다 돌렸으면 리스트를 카피 한다.
			for ( SI32 i = 0 ; i <MAX_RESERVER_NUM ; i++) 
			{
				if ( i >= MAX_RESERVER_NUM) break	;
				SI32 siPersonID = pclReserveList_Temp[i].GetCharPersonID();
				if ( siPersonID <= 0 )	break	;	// 비었으니 이제 할필요 없다
				TCHAR* pCharNeme = pclReserveList_Temp[i].GetCharName();
                pclReserveList[i].SetChar(siPersonID,pCharNeme)	;
			}
		}
	}
}

// 5초 후 시작 카운터 메세지 날림

void CRacoonDodgeEventMgr::Count5Second(void )	// 참여자를 선택해서 메세지를 보낸다.
{
	// 만들어진 리스트를 토대로 
	cltServer* pclserver = (cltServer*)pclClient;

	for ( SI32 i = 0 ; i< EVENT_START_NUM*MAX_EVENTPLACE ; i++)
	{
		SI32 siPersonID =	0	;
		SI32 siID =			0	;

		siPersonID = pclSelectPersonList[ i ].GetCharPersonID() ;	// 만들어진 선발 리스트에서 personid를 꺼낸다
		if ( siPersonID <= 0 )	// 리스트에 사람이 없다 이젠 종료
		{
			return	;
		}
		siID = pclserver->pclCM->GetIDFromPersonID( siPersonID ) ;	// 꺼낸 personid를 통해서 charid를 찾는다.

		cltCharServer* pclCharServer = pclserver->pclCM->GetCharServer( siID )	;	// 해당 id의 char정보를 get한다.
		if( pclCharServer != NULL )
		{
			SI32 siMapindex = pclCharServer->GetMapIndex()	;
			if( siMapindex == PLAY_MAP_INDEX_INDODEVENT )// 해당 맵인덱스에 있고 
			{
				cltMapCommon* pclmap = pclClient->pclMapManager->GetMapPointer(siMapindex)	;

				if(pclmap != NULL)
				{
					SI32 siColl = pclmap->CalcCollInfo( pclCharServer->GetX(), pclCharServer->GetY())	;
					if( siColl & BLOCK_EVENT1 || siColl & BLOCK_EVENT4 )	// 4번 이벤타 타일을 밟고 있을때.
					{
						// 5초카운트 시작 메세지 
						cltGameMsgResponse_Remain5Seceond clinfo( pclCharServer->pclCI->GetPersonID() );	// 일단 퍼슨아이디로 해놓는다
						cltMsg clMsg(GAMEMSG_RESPONS_RACOONDODGE_EVENT_WAIT5SECOND, sizeof(clinfo), (BYTE*)&clinfo);
						pclCharServer->SendNetMsg((sPacketHeader*)&clMsg);
					}
				}
			}
		}
		else
		{
			// 잘못된 char 정보 해당 케릭터는 접속해있지 않다. 
		}
	}
}
SI32 CRacoonDodgeEventMgr::GetStep(void )	// 매니저 액션의 현제 상태를 가지고 온다
{
	return siGameStep	;
}
SI32 CRacoonDodgeEventMgr::GetLevel(void )	// 게임의 진행레벨을 가지고 온다
{
	return siGameLevel	;
}
void CRacoonDodgeEventMgr::WarpGameSpace(SI32 PersonID )	// 해당 맵으로 워프 한다.
{
	// 만들어진 리스트를 토대로 
	cltServer* pclserver = (cltServer*)pclClient;

	for ( SI32 i = 0 ; i< EVENT_START_NUM*MAX_EVENTPLACE ; i++)
	{
		SI32 siPersonID =	0	;
		SI32 siID =			0	;

		siPersonID = pclSelectPersonList[ i ].GetCharPersonID() ;	// 만들어진 선발 리스트에서 personid를 꺼낸다
		if ( PersonID == siPersonID )
		{
			if ( i < EVENT_START_NUM )
			{
				cltWarpInfo clWarpInfo(1, 0, true, false, 117, 102, -1, -1);
				clWarpInfo.uiMapIndex = PLAY_MAP_INDEX_INDODEVENT;

				SI32 siCharId = pclClient->pclCM->GetIDFromPersonID( siPersonID ) ;

				cltCharServer* pclchar = pclClient->pclCM->GetCharServer(siCharId);
				if (pclchar == NULL )	return	;
				pclchar->SetWarpInfo(&clWarpInfo, 2200);	// 해당 위치로 워프 한다 .
				pclchar->Warp()	;
			}
			else
			{
				cltWarpInfo clWarpInfo(1, 0, true, false, 54, 101, -1, -1);
				clWarpInfo.uiMapIndex = PLAY_MAP_INDEX_INDODEVENT;

				SI32 siCharId = pclClient->pclCM->GetIDFromPersonID( siPersonID ) ;

				cltCharServer* pclchar = pclClient->pclCM->GetCharServer(siCharId);
				if (pclchar == NULL )	return	;
				pclchar->SetWarpInfo(&clWarpInfo, 2200);	// 해당 위치로 워프 한다 .
				pclchar->Warp()	;
			}
		}
	}
}
void CRacoonDodgeEventMgr::SendReserveList(SI32 siPersonID)
{
	cltServer* pclserver = (cltServer*)pclClient;

	SI32 sicharID = pclserver->pclCM->GetIDFromPersonID( siPersonID )	;
	cltCharServer* pclCharServer = pclserver->pclCM->GetCharServer( sicharID )	;

	if( pclCharServer == NULL  )	return	;
	// 현제 예약리스트를 클라이언트로 날려준다 
	// 날릴만큼 만든다
	ReserveData	clInfo[MAX_RESERVER_SHOW_NUM];

	for ( SI32 i = 0 ; i < MAX_RESERVER_SHOW_NUM; i++)
	{
		if( i >= MAX_RESERVER_SHOW_NUM )	break	;
		clInfo[i].SetCharName( pclReserveList[i].GetCharName() )	;
		clInfo[i].SetCharPersonID( pclReserveList[i].GetCharPersonID())	;
	}
	// 현제 예약 리스트를 날려준다
	cltGameMsgResponse_ReserveList clinfo( clInfo );
	cltMsg clMsg(GAMEMSG_RESPONS_RACOONDODGE_EVENT_RESERVATION, sizeof(clinfo), (BYTE*)&clinfo);
	pclCharServer->SendNetMsg((sPacketHeader*)&clMsg);
	
}

//siMake_Monster_Num 만큼 몬스터를 좌표에 만들어 낸다.
void CRacoonDodgeEventMgr::SetMonster( SI32 siMake_Monster_Num, SI32 siStageindex)
{
	cltServer* pclserver = (cltServer*)pclClient;
	SI32 siMonsterKind	=	0	;	// 뽑아낼 몬스터 카인드
	SI32 siRocX	=	0	;	// 기준에 되는 X좌표
	SI32 siRocY	=	0	;	// 기준에 되는 Y좌표
	SI32 siX	=	0	;	// 실제 떨어질 X좌표
	SI32 siY	=	0	;	// 실제 떨어질 Y좌표
	SI32 siMapIndex		=	PLAY_MAP_INDEX_INDODEVENT	;	// 떨어뜨릴 맵인덱스
	SI32 silevel		=	0	;	// 몬스터 레벨
	SI32 siMakeCount	=	0	;	// 만들어낸 몬스터  숫자
	SI32 siEnergy		=	10000;
	SI32 siLevel		=	10	;


	for (siMakeCount; siMakeCount <siMake_Monster_Num; siMakeCount++ )
	{

		SI32 monstermakeindex	=	0	;
		SI32 ramdomrocIndex = rand() % MAX_MONSTER_ROC	;

		if( siStageindex >= MAX_STAGE_NUM )		return	;
		siRocX = pclRocList[siStageindex].GetRocX( ramdomrocIndex)	;
		siRocY = pclRocList[siStageindex].GetRocY( ramdomrocIndex)	;

		if ( siRocX == 0 || siRocY == 0 ) 
		{
			if( siStageindex >= MAX_STAGE_NUM )		return	;
			siRocX = pclRocList[siStageindex].GetRocX( 0)	;
			siRocY = pclRocList[siStageindex].GetRocY( 0)	;
		}

		SI32 siKindSlot  = rand() % siMonsterKindNum	;
		siMonsterKind	=	pclMonsterUnique[siKindSlot];	// 배열에 잡힌 몬스터 카인드를 가지고 온다

		cltMapCommon* pclmap = pclserver->pclMapManager->GetMapPointer( siMapIndex )	;	// 맵포인터
		if(pclmap != NULL)
		{
			cltInnerStatusInfo clinnerstatus;
			clinnerstatus.clEnemyType.Set(ENEMYTYPE_ALLPC, 0);

			cltIdentity clIdentity;
			clIdentity.MakeRaccoonDodgeEvent(IDENTITY_RACOONDODGE_EVENT);

			SI32 id = pclserver->SetNPCChar( siMonsterKind, 
				VILLAGE_NONE, 
				&clIdentity, 
				siLevel, 
				siEnergy, 
				siRocX, 
				siRocY, 
				siMapIndex, 
				NPCITEMMODE_NONE, 
				&clinnerstatus );
			//RememberEventMonster(id);
			SetAggressiveNMoveSpeed(id)	;
		}
		
	}
}
void CRacoonDodgeEventMgr::LoadRocFile()
{
		NDataLoader	dataloader;
		BOOL bLoadFile=FALSE;
	
		if( pclClient->IsWhereServiceArea( ConstSwitch_TextEncryption ) )
		{
			bLoadFile = dataloader.LoadDataFromCryptFile( TEXT("Data/RacoonDodge_RocInfo.txt") );
		}
		else
		{
			bLoadFile = dataloader.LoadDataFromFile( TEXT("Data/RacoonDodge_RocInfo.txt") );
		}
		if (!bLoadFile)
		{
			MsgBox( TEXT("Not LoadFromFile RacoonDodge_RocInfo.txt"),TEXT("Error"));
			return;
		}
		// 데이터 관련 받을 변수는 3가지
		SI32 siStageIndex_Resource	=	0	;	// 리소스에서 받아 들인 스테이지 인덱스
		SI32 siMapRoc_X_Resource		=	0	;	// 리소스에서 받아 들인 스테이지 안의 몹이 소환될 좌표 
		SI32 siMapRoc_Y_Resource		=	0	;

		NDataTypeInfo pDataTypeInfo[] =
		{
			NDATA_INT32,	&siStageIndex_Resource,	4,
			NDATA_INT32,	&siMapRoc_X_Resource,	4,
			NDATA_INT32,	&siMapRoc_Y_Resource,	4,
			0,0,0
		};	
		dataloader.RegDataType( pDataTypeInfo );

		SI32 siStageIndex_In_DataArray	=	0	;	// 데이터들 안에 어디 좌표인지 구분 하기 위해 가지고 있는 스테이지 인덱스
		SI32 ArrayIndex_In_Data			=	0	;	// 좌표들이 가지고 있는데이터 array인덱스
		SI32 RocCount					=	0	;

		SI32 siStageIndex				=	0	;	// 스테이지 인덱스 
		while (!dataloader.IsEndOfData() )	// 변수명 다시 하는중 
		{
			if ( dataloader.ReadData( true ) )
			{
				// 해당 좌표의 스테이지를뽑는다. 
				if( siStageIndex >= MAX_STAGE_NUM )		return	;
				siStageIndex_In_DataArray	=	pclRocList[siStageIndex].GetStage(ArrayIndex_In_Data)	;

				// 스테이지 인덱스가 0이면 첫번째 스테이지 그리고 
				if ( siStageIndex_In_DataArray == 0 && siStageIndex_In_DataArray != siStageIndex_Resource)
				{
					// 예외 처리 
					if ( siStageIndex >= MAX_STAGE_NUM)	break	;
					if ( ArrayIndex_In_Data >= MAX_MONSTER_ROC)	break	;

					pclRocList[siStageIndex].SetRoc( siStageIndex_Resource ,siMapRoc_X_Resource,siMapRoc_Y_Resource)	;
				}
				else if ( siStageIndex_Resource == siStageIndex_In_DataArray )	// 이건 같은 스테이지 인덱스 일때 다음 데이터 인덱스에 넣는다
				{
					// 예외 처리 
					if ( siStageIndex >= MAX_STAGE_NUM)	break	;
					if ( ArrayIndex_In_Data >= MAX_MONSTER_ROC)	break	;
					// 들어가 있는 애랑 같은 인덱스다 그럼 같은 스테이지의 그 뒤로 넣는다
					pclRocList[siStageIndex].SetRoc( siStageIndex_Resource,siMapRoc_X_Resource,siMapRoc_Y_Resource)	;
				}
				else if( siStageIndex_Resource != siStageIndex_In_DataArray )	// 다음 스테이지 일때 여기에서 데이터 인덱스라든지 이런것들 초기화 해서 넣어준다
				{
					// 예외 처리 
					if ( siStageIndex >= MAX_STAGE_NUM)	break	;
					if ( ArrayIndex_In_Data >= MAX_MONSTER_ROC)	break	;
					// 들어가 있는 애랑 같은 인덱스다 그럼 같은 스테이지의 그 뒤로 넣는다
					siStageIndex++;
					pclRocList[siStageIndex].SetRoc( siStageIndex_Resource,siMapRoc_X_Resource,siMapRoc_Y_Resource)	;

				}
			}
		}	

}

void CRacoonDodgeEventMgr::LoadMonsterKindFile()
{
	NDataLoader	dataloader;
	BOOL bLoadFile=FALSE;

	if( pclClient->IsWhereServiceArea( ConstSwitch_TextEncryption ) )
	{
		bLoadFile = dataloader.LoadDataFromCryptFile( TEXT("Data/RacoonDodge_MonsterKindInfo.txt") );
	}
	else
	{
		bLoadFile = dataloader.LoadDataFromFile( TEXT("Data/RacoonDodge_MonsterKindInfo.txt") );
	}
	if (!bLoadFile)
	{
		MsgBox( TEXT("Not LoadFromFile RacoonDodge_MonsterKindInfo.txt"),TEXT("Error"));
		return;
	}
	// 데이터 관련 받을 변수는 3가지
	TCHAR szMonsterKind[64]			=	TEXT("");

	NDataTypeInfo pDataTypeInfo[] =
	{
		NDATA_MBSTRING,	&szMonsterKind,	64,
			0,0,0
	};	
	dataloader.RegDataType( pDataTypeInfo );
	SI32 siArrayindex = 0 ;

	while (!dataloader.IsEndOfData() )	// 변수명 다시 하는중 
	{
		if ( dataloader.ReadData( true ) )
		{
			if ( siArrayindex >= MAX_MONSTER_NUM )	break	;
			SI32 siMonsterUnique =	pclClient->GetUniqueFromHash(szMonsterKind);
			if(siMonsterUnique <= 0 )	continue	;
			pclMonsterUnique[siArrayindex]	=	siMonsterUnique	;
			siArrayindex++;
			siMonsterKindNum++;
		}
	}	

}
void CRacoonDodgeEventMgr::LoadMonsterInfoSetFile()
{
	NDataLoader	dataloader;
	BOOL bLoadFile=FALSE;

	if( pclClient->IsWhereServiceArea( ConstSwitch_TextEncryption ) )
	{
		bLoadFile = dataloader.LoadDataFromCryptFile( TEXT("Data/RacoonDodge_MonsterInfoSetInfoo.txt") );
	}
	else
	{
		bLoadFile = dataloader.LoadDataFromFile( TEXT("Data/RacoonDodge_MonsterInfoSetInfoo.txt") );
	}
	if (!bLoadFile)
	{
		MsgBox( TEXT("Not LoadFromFile RacoonDodge_MonsterInfoSetInfo.txt"),TEXT("Error"));
		return;
	}
	// 데이터 관련 받을 변수는 3가지
	
	SI32 siGameLevel	=	0	;			// 게임 라운드 레벨

	SI32 siAggressiveGap	=	0	;			// 몬스터 어그래시브 갭
	SI32 siAggresiveAddPoint	=	0	;		// 몬스터 어그래시브 증가값

	SI32 siMoveSpeedGap	=	0	;			// 몬스터 무브 스피드 갭
	SI32 siMoveSpeedAddPoint	=	0	;	// 몬스터 무브 스피드 증가값
	

	NDataTypeInfo pDataTypeInfo[] =
	{
		NDATA_INT32,	&siGameLevel,	4,

			NDATA_INT32,	&siAggressiveGap,	4,
			NDATA_INT32,	&siAggresiveAddPoint,	4,

			NDATA_INT32,	&siMoveSpeedGap,	4,
			NDATA_INT32,	&siMoveSpeedAddPoint,	4,
			0,0,0
	};	
	dataloader.RegDataType( pDataTypeInfo );
	SI32 siArrayindex = 0 ;

	while (!dataloader.IsEndOfData() )	// 변수명 다시 하는중 
	{
		if ( dataloader.ReadData( true ) )
		{
			if (siArrayindex >= MAX_MONSTER_NUM)	break	;
			pclMonsterInfo_Level[siArrayindex].SetData( siGameLevel , siAggressiveGap,siAggresiveAddPoint,siMoveSpeedGap,siMoveSpeedAddPoint	);
			siArrayindex++;
		}
	}	

}
SI32 CRacoonDodgeEventMgr::CheckPlayStageNum()
{
	SI32 siReserveListNum	=	CountReservePersonNum();
	if(siReserveListNum > 0 )
	{
		if( siReserveListNum >= 10)	// 예약된 총수가 10명을 넘어 서면 
		{
			return RACOONDODGEEVENT_STAGE_TWO	;
		}
		else if (siReserveListNum >= 5 ) 
		{
			return RACOONDODGEEVENT_STAGE_ONE	;
		}
		else
		{
			return RACOONDODGEEVENT_STAGE_NONE	;
		}
	}
	return RACOONDODGEEVENT_STAGE_NONE	;
}
bool CRacoonDodgeEventMgr::CanReserveList(SI32 siPersonID)
{
	cltServer* pclserver = (cltServer*)pclClient;


	bool bResult = true	;
	SI32 siReservationCharId = pclserver->pclCM->GetIDFromPersonID( siPersonID ) ; 
	cltCharServer* pclReservationCharServer = pclserver->pclCM->GetCharServer( siReservationCharId )	;
	if(pclReservationCharServer == NULL )	return false ;

	// 이미 리스트에 있는지 확인
	// 꽉 찼다.
	SI32 siReserveListNum	=	CountReservePersonNum()	;

	if(siReserveListNum >= MAX_RESERVER_NUM )
	{
		pclserver->SendServerResponseMsg( 0, SRVAL_RACOONDODGEEVENT_NOTEMPTYLISTPLACE , 0, 0, pclReservationCharServer->GetCharUnique() );	// 빈자리가 없다
		bResult=  false	;
	}

	for ( SI32 i = 0 ; i < MAX_RESERVER_NUM; i++ )
	{
		if(pclReserveList[i].GetCharPersonID() == siPersonID)
		{
			pclserver->SendServerResponseMsg( 0, SRVAL_RACOONDODGEEVENT_RESERVEPERSON, 0, 0, pclReservationCharServer->GetCharUnique() );// 이미 신청 하였다.
			bResult=  false	;
			break	;
		}
	}
	// 이미 경기장에 있는지확인

	SI32 siMapindex = pclReservationCharServer->GetMapIndex() ;
	if( siMapindex == PLAY_MAP_INDEX_INDODEVENT )// 해당 맵인덱스에 있고 
	{
		cltMapCommon* pclmap = pclClient->pclMapManager->GetMapPointer(siMapindex)	;

		if(pclmap != NULL)
		{
			SI32 siColl = pclmap->CalcCollInfo( pclReservationCharServer->GetX(), pclReservationCharServer->GetY())	;
			if( siColl & BLOCK_EVENT1 || siColl & BLOCK_EVENT4 )	// 4번 이벤타 타
			{
				// 이미 경기장에 있다
				pclserver->SendServerResponseMsg( 0, SRVAL_RACOONDODGEEVENT_CANNOTRESERVE_INPLAY, 0, 0, pclReservationCharServer->GetCharUnique() );// 이미 신청 하였다.
				bResult=  false	;
			}
		}

	}

	return bResult ;
}
void CRacoonDodgeEventMgr::SetAggressiveNMoveSpeed( SI32 siCharID )
{
	cltServer* pclserver = (cltServer*)pclClient;

	cltCharServer* pclCharerver = pclserver->pclCM->GetCharServer(siCharID)	;
	if( pclCharerver != NULL  )
	{
		// 게임레벨에 따른 어그래시브와 무브 스피드 조절 
		SI32 siGameLevel = GetLevel()	;

		SI32 Aggressive = 0	;
		SI32 Movespeed = 0	;


		SI32 siAgg_AddValue	=	0	;
		SI32 siAgg_Value	=	0	;

		SI32 siMoveSpeed_Value	=	0;
		SI32 siMoveSpeed_AddValue	=	0;

		for ( SI32 siSLot = 0 ; siSLot< MAX_PLAY_LEVEL; siSLot++)
		{
			if ( pclMonsterInfo_Level[siSLot].GetSiGameLevel() == siGameLevel ) 
			{
				siAgg_Value = pclMonsterInfo_Level[siSLot].GetAGV_Value() ;
				siAgg_AddValue = pclMonsterInfo_Level[siSLot].GetAGV_AddValue() ;
				siMoveSpeed_AddValue = pclMonsterInfo_Level[siSLot].GetMoveSpeed_Value() ;
				siMoveSpeed_Value = pclMonsterInfo_Level[siSLot].GetMoveSpeed_AddValue() ;
			}
		}
		if( siAgg_Value > 0 &&  siAgg_AddValue > 0 &&  siMoveSpeed_Value > 0 &&  siMoveSpeed_AddValue > 0 )
		{
			Aggressive = rand() % siAgg_Value	+	siAgg_AddValue	;
			Movespeed = rand() % siMoveSpeed_Value	+	siMoveSpeed_AddValue	;
			if ( Aggressive >= MAX_AGGRESSIVE ){				Aggressive =  MAX_AGGRESSIVE ;			}
			if ( Movespeed >= MAX_MOVESPEED ){				Movespeed =  MAX_MOVESPEED ;			}
			if ( Aggressive > 0 && Movespeed > 0 )
			{
				if(pclCharerver->clIdentity.siIdentity == IDENTITY_RACOONDODGE_EVENT )
				{
					pclCharerver->pclKI->siAggressive	= Aggressive	;
					pclCharerver->pclKI->siMoveSpeed	= Movespeed		;
				}
			}		
		}

		pclCharerver->dwTimerClock = pclserver->CurrentClock;
	}
}

void CRacoonDodgeEventMgr::Count30Second(void )	// 참여자를 선택해서 메세지를 보낸다.
{
	// 만들어진 리스트를 토대로 
	cltServer* pclserver = (cltServer*)pclClient;

	for ( SI32 i = 0 ; i< EVENT_START_NUM*MAX_EVENTPLACE ; i++)
	{
		SI32 siPersonID =	0	;
		SI32 siID =			0	;

		siPersonID = pclSelectPersonList[ i ].GetCharPersonID() ;	// 만들어진 선발 리스트에서 personid를 꺼낸다
		if ( siPersonID <= 0 )	//리스트에 사람없다  패스
		{
			continue	;
		}
		siID = pclserver->pclCM->GetIDFromPersonID( siPersonID ) ;	// 꺼낸 personid를 통해서 charid를 찾는다.

		cltCharServer* pclCharServer = pclserver->pclCM->GetCharServer( siID )	;	// 해당 id의 char정보를 get한다.
		if( pclCharServer != NULL )
		{
			SI32 siMapindex = pclCharServer->GetMapIndex()	;
			if( siMapindex == PLAY_MAP_INDEX_INDODEVENT )// 해당 맵인덱스에 있고 
			{
				cltMapCommon* pclmap = pclClient->pclMapManager->GetMapPointer(siMapindex)	;

				if(pclmap != NULL)
				{
					SI32 siColl = pclmap->CalcCollInfo( pclCharServer->GetX(), pclCharServer->GetY())	;
					if( siColl & BLOCK_EVENT1 || siColl & BLOCK_EVENT4 )	// 4번 이벤타 타일을 밟고 있을때.
					{
						cltGameMsgResponse_30Seceond clinfo( pclCharServer->pclCI->GetPersonID(),dwRoundTime );
						cltMsg clMsg(GAMEMSG_RESPONS_RACOONDODGE_EVENT_30SECOND, sizeof(clinfo), (BYTE*)&clinfo);
						pclCharServer->SendNetMsg((sPacketHeader*)&clMsg);
					}
				}
			}
		}
		else
		{
			// 잘못된 char 정보 해당 케릭터는 접속해있지 않다. 
		}
	}
}
// 더이상 사람이업을대 
bool CRacoonDodgeEventMgr::CanPlayEvent(void )	// 참여자를 선택해서 메세지를 보낸다.
{
	// 만들어진 리스트를 토대로 
	cltServer* pclserver = (cltServer*)pclClient;

	SI32 siPlayGameNumNow	=	0	;	// 몬스터 어그래시브 추가 증감값

	for ( SI32 i = 0 ; i< EVENT_START_NUM*MAX_EVENTPLACE ; i++)
	{
		SI32 siPersonID =	0	;
		SI32 siID =			0	;

		siPersonID = pclSelectPersonList[ i ].GetCharPersonID() ;	// 만들어진 선발 리스트에서 personid를 꺼낸다
		siID = pclserver->pclCM->GetIDFromPersonID( siPersonID ) ;	// 꺼낸 personid를 통해서 charid를 찾는다.

		cltCharServer* pclCharServer = pclserver->pclCM->GetCharServer( siID )	;	// 해당 id의 char정보를 get한다.
		if( pclCharServer != NULL )
		{
			SI32 siMapindex = pclCharServer->GetMapIndex()	;
			if( siMapindex == PLAY_MAP_INDEX_INDODEVENT )// 해당 맵인덱스에 있고 
			{
				cltMapCommon* pclmap = pclClient->pclMapManager->GetMapPointer(siMapindex)	;

				if(pclmap != NULL)
				{
					SI32 siColl = pclmap->CalcCollInfo( pclCharServer->GetX(), pclCharServer->GetY())	;
					if( siColl & BLOCK_EVENT1 || siColl & BLOCK_EVENT4 )	// 4번 이벤타 타일을 밟고 있을때.
					{
						if( pclserver->pclCM->IsAlive(siID) == TRUE )
						{
							// 해당 케릭터가 플레이중이면 카운트를 올린다 .
							siPlayGameNumNow++;
						}
					}
					else
					{
						// 리워드 주고는 화면에 카운트를 지운다 
						cltGameMsgResponse_30Seceond_Clear clinfo( siPersonID );
						cltMsg clMsg3(GAMEMSG_RESPONS_RACOONDODGE_EVENT_30SECOND_CLEAR, sizeof(clinfo), (BYTE*)&clinfo);
						pclCharServer->SendNetMsg((sPacketHeader*)&clMsg3);

						cltGameMsgResponse_Round_Clear clinfo2( siPersonID );
						cltMsg clMsg2(GAMEMSG_RESPONS_RACOONDODGE_EVENT_ROUNDCOUNT_CLEAR, sizeof(clinfo2), (BYTE*)&clinfo2);
						pclCharServer->SendNetMsg((sPacketHeader*)&clMsg2);

						// 주고 리스트에서 지워 버려 
						pclSelectPersonList[ i ].init()	;
					}
				}
			}
		}
		else
		{
			// 잘못된 char 정보 해당 케릭터는 접속해있지 않다. 
		}
	}
	if ( siPlayGameNumNow <= 0 ) 
	{
		// 종료 시킨다

		return false	;
	}
	else
	{
		return true	;
	}
}
void CRacoonDodgeEventMgr::KillEventMonster(void )	// 이벤트 몬스터를 다 죽인다.
{
	cltServer* pclserver = (cltServer*)pclClient;

	for ( SI32 siIndex = 0; siIndex <MAX_MAKE_MONSTER_NUM; siIndex++ ) 
	{
		SI32 siMonsterID = MakeMonsterArray[siIndex]	;
		// 아이디를 뺐으면 0을 채워 리스트애서 지운다 .
		MakeMonsterArray[siIndex] = 0 ;
		cltCharServer* pclMonster = pclClient->pclCM->GetCharServer( siMonsterID );
		if ( pclMonster != NULL ) 
		{
			if(IDENTITY_RACOONDODGE_EVENT == pclMonster->clIdentity.siIdentity)
			{
				SI32 siMapindex = pclMonster->GetMapIndex()	;

				if( siMapindex == PLAY_MAP_INDEX_INDODEVENT )// 해당 맵인덱스에 있고 
				{
					cltMapCommon* pclmap = pclClient->pclMapManager->GetMapPointer(siMapindex)	;

					if(pclmap != NULL)
					{
						SI32 siColl = pclmap->CalcCollInfo( pclMonster->GetX(), pclMonster->GetY())	;
						if( siColl & BLOCK_EVENT1 || siColl & BLOCK_EVENT4 )	// 4번 이벤타 타일을 밟고 있을때.
						{
							pclMonster->SetLife(0)	;

						}
					}
				}
			
			}			
		}
	}
}
//void CRacoonDodgeEventMgr::RememberEventMonster(SI32 siID)
//{
//	for ( SI32 monstermakeindex = 0 ; monstermakeindex< MAX_MAKE_MONSTER_NUM; monstermakeindex++) 
//	{
//		if ( MakeMonsterArray[monstermakeindex] == 0 ) 
//		{
//			MakeMonsterArray[monstermakeindex] = siID	;
//			return ;
//		}
//	}
//}

//보상을 주자 
void CRacoonDodgeEventMgr::RewardMoney(void)
{

	// 만들어진 리스트를 토대로 
	cltServer* pclserver = (cltServer*)pclClient;

	// 보상금을 셋팅 한다 .
	GMONEY gCost	=	(pclClient->pclItemManager->clItemStatistic.siNPCPriceRate*1000)/100	;	// npcrate에 따른 코스트
	GMONEY gReward = gCost * REWARDPOINT	;


	for ( SI32 i = 0 ; i< EVENT_START_NUM*MAX_EVENTPLACE ; i++)
	{
		SI32 siPersonID =	0	;
		SI32 siID =			0	;

		siPersonID = pclSelectPersonList[ i ].GetCharPersonID() ;	// 만들어진 선발 리스트에서 personid를 꺼낸다
		siID = pclserver->pclCM->GetIDFromPersonID( siPersonID ) ;	// 꺼낸 personid를 통해서 charid를 찾는다.

		cltCharServer* pclCharServer = pclserver->pclCM->GetCharServer( siID )	;	// 해당 id의 char정보를 get한다.
		if( pclCharServer != NULL )
		{
			SI32 siMapindex = pclCharServer->GetMapIndex()	;
			if( siMapindex == PLAY_MAP_INDEX_INDODEVENT )// 해당 맵인덱스에 있고 
			{
				cltMapCommon* pclmap = pclClient->pclMapManager->GetMapPointer(siMapindex)	;

				if(pclmap != NULL)
				{
					SI32 siColl = pclmap->CalcCollInfo( pclCharServer->GetX(), pclCharServer->GetY())	;
					if( siColl & BLOCK_EVENT1 || siColl & BLOCK_EVENT4 )	// 4번 이벤타 타일을 밟고 있을때.
					{
						if( pclserver->pclCM->IsAlive(siID) == false ) 
						{
							continue	;
						}
						else
						{
							sDBRequest_ChangeMoney clMsg(pclCharServer->GetID(), pclCharServer->pclCI->GetPersonID(), CHANGE_MONEY_REASON_RACOONDODGE_REWARD, gReward);
							pclClient->SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg);
							pclserver->SendServerResponseCharMsg2( 0, SRVAL_RETURN_RACOONDODGEEVENT_CLEAR, SI64ToString(gReward), NULL, NULL, pclCharServer->GetCharUnique() );		

							cltWarpInfo clWarpInfo(1, 0, true, false, 91, 135, -1, -1);
							clWarpInfo.uiMapIndex = PLAY_MAP_INDEX_INDODEVENT;
							pclCharServer->SetWarpInfo(&clWarpInfo, 2200);	// 해당 위치로 워프 한다 .
							pclCharServer->Warp()	;

							if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog))     
							{
								// [로그추가 : 손서웅 2009. 8. 24] // 라쿤이벤트 참가비 지불 
								// 타켓 personID 
								pclserver->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_RACCOONDODGE_EVENT, LOGCOMMAND_INDEX_PVP_RACCOONDODGE_EVENT_REWARD, 
									0, pclCharServer,NULL ,0, 0, 0, NULL, 0, 0, 0, 0, 0, 0, 
									gReward, 0, 0, 0, 0, NULL, NULL);	// para1 : 상금
							}
						}
					}
				}
			}
		}
	}
}

void CRacoonDodgeEventMgr::SetRoundTime(SI32 siSecond)
{
	dwRoundTime = siSecond *1000	;
}

void CRacoonDodgeEventMgr::RoundSet(void )	// 참여자를 선택해서 메세지를 보낸다.
{
	// 만들어진 리스트를 토대로 
	cltServer* pclserver = (cltServer*)pclClient;

	for ( SI32 i = 0 ; i< EVENT_START_NUM*MAX_EVENTPLACE ; i++)
	{
		SI32 siPersonID =	0	;
		SI32 siID =			0	;

		siPersonID = pclSelectPersonList[ i ].GetCharPersonID() ;	// 만들어진 선발 리스트에서 personid를 꺼낸다
		if ( siPersonID <= 0 )	// 리스트에 사람이 없다 다음 사람
		{
			continue	;
		}
		siID = pclserver->pclCM->GetIDFromPersonID( siPersonID ) ;	// 꺼낸 personid를 통해서 charid를 찾는다.

		cltCharServer* pclCharServer = pclserver->pclCM->GetCharServer( siID )	;	// 해당 id의 char정보를 get한다.
		if( pclCharServer != NULL )
		{
			SI32 siMapindex = pclCharServer->GetMapIndex()	;
			if( siMapindex == PLAY_MAP_INDEX_INDODEVENT )// 해당 맵인덱스에 있고 
			{
				cltMapCommon* pclmap = pclClient->pclMapManager->GetMapPointer(siMapindex)	;

				if(pclmap != NULL)
				{
					SI32 siColl = pclmap->CalcCollInfo( pclCharServer->GetX(), pclCharServer->GetY())	;
					if( siColl & BLOCK_EVENT1 || siColl & BLOCK_EVENT4 )	// 4번 이벤타 타일을 밟고 있을때.
					{

						SI32 siGameRound = GetLevel()	;
						switch ( siGameRound )
						{
						case EVENTLEVEL_LV1:
						case EVENTLEVEL_LV2:
						case EVENTLEVEL_LV3:
						case EVENTLEVEL_LV4:
						case EVENTLEVEL_LV5:
						case EVENTLEVEL_LV6:
						case EVENTLEVEL_LV7:
						case EVENTLEVEL_LV8:
						case EVENTLEVEL_LV9:
						case EVENTLEVEL_LV10:
							{
								cltGameMsgResponse_RoundCount clinfo( pclCharServer->pclCI->GetPersonID(),siGameRound-1 );
								cltMsg clMsg(GAMEMSG_RESPONS_RACOONDODGE_EVENT_ROUNDCOUNT, sizeof(clinfo), (BYTE*)&clinfo);
								pclCharServer->SendNetMsg((sPacketHeader*)&clMsg);
							}break;
						default:
							{
								cltGameMsgResponse_RoundCount clinfo( pclCharServer->pclCI->GetPersonID(),-1 );
								cltMsg clMsg(GAMEMSG_RESPONS_RACOONDODGE_EVENT_ROUNDCOUNT, sizeof(clinfo), (BYTE*)&clinfo);
								pclCharServer->SendNetMsg((sPacketHeader*)&clMsg);
							}
							break;
						}
					}
				}
			}
		}
		else
		{
			// 잘못된 char 정보 해당 케릭터는 접속해있지 않다. 
		}
	}
}

SI32 CRacoonDodgeEventMgr::CountReservePersonNum(void )	// 참여자를 선택해서 메세지를 보낸다.
{
	SI32 siReserveListNum=	0	;
	for ( SI32 siSlotNum = 0 ; siSlotNum < MAX_RESERVER_NUM; siSlotNum++ ) 
	{
		SI32 siReservePersonId = pclReserveList[siSlotNum].GetCharPersonID() ;
		if ( siReservePersonId > 0  ) 
		{
			siReserveListNum++;
		}
	}
	return siReserveListNum	;
}