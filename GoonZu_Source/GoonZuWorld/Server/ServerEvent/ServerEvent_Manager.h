#ifndef _SERVER_EVENT_MANAGER_H_
#define _SERVER_EVENT_MANAGER_H_

#pragma once

#include "Directives.h"
#include "CommonLogic.h"
#include "ServerEvent_Rank_Record.h"
#include "ServerEvent-Quest.h"
#include "ServerEvent-InstanceMapInfo.h"

#define SAFE_POINTER(p)	if(NULL == p) return;

#define SERVER_EVENT_BONUS_MAP				  5
#define SERVER_EVENT_BONUS_MAP_START_INDEX	361

enum SERVER_EVENT_COMMANDTYPE
{
	SERVER_EVENT_COMMANDTYPE_NONE = 0,

	SERVER_EVENT_COMMANDTYPE_QUEST_START,
	SERVER_EVENT_COMMANDTYPE_QUEST_END,
	SERVER_EVENT_COMMANDTYPE_INDUN_START,
	SERVER_EVENT_COMMANDTYPE_INDUN_END,

	//-----------------------------------------------------------------------------------
	SERVER_EVENT_COMMANDTYPE_END
};

// 서버 이벤트 상태
enum SERVER_EVENT_STATE
{
	SERVER_EVENT_STATE_NONE = 0,

	SERVER_EVENT_STATE_READY,		// 최초 실행상태

	SERVER_EVENT_STATE_START_QUEST,	// 퀘스트 시작
	SERVER_EVENT_STATE_GOING_QUEST,	// 퀘스트 진행중
	SERVER_EVENT_STATE_END_QUEST,	// 퀘스트 종료

	SERVER_EVENT_STATE_START_INDUN,	// 인던 시작
	SERVER_EVENT_STATE_GOING_INDUN,	// 인던 진행중
	SERVER_EVENT_STATE_END_INDUN,	// 인던 종료

	SERVER_EVENT_STATE_WAIT,		// 이 상태가 퀘스트/인던 이 수행이 되지 않는다

	//-----------------------------------------------------------------------------------
	SERVER_EVENT_STATE_END
};


class CNServerEvent_Manager
{
public:
	enum
	{
		 SERVER_EVENT_BUF_NO					// 버프 없음.
		,SERVER_EVENT_BUF_ATTACK_EXP_150		// 전투 경험치 150%
		,SERVER_EVENT_BUF_MAKE_EXP_150			// 재조 경험치 150%
		,SERVER_EVENT_BUF_ATTACK_EXP_200		// 전투 경험치 200%
		,SERVER_EVENT_BUF_MAKE_EXP_200			// 재조 경험치 200%	
		,SERVER_EVENT_BUF_ETC					// 어획, 낚시, 채광
		,SERVER_EVENT_BUF_NUM					// 서버 버프의 개수.					
	};

	enum
	{
		 MSG_FLAG_DAILY
		,MSG_FLAG_TOTAL_SCORE_ADD
		,MSG_FLAG_SERVER_RANK_CLEAR
	};


public:
	CNServerEvent_Manager(void);
	~CNServerEvent_Manager();

	void			Init(void);
	void			Clear(void);
	void			Destroy(void);
			
	void			SendDBMsgServerEventEnd(void);								// 이벤트 종료시 최종기록 요청.(전체 랭킹 정보 요청)
	void			SendDBMsgServerEventDaily(void);							// 하루 점수 누적액 요청.(전체 랭킹 정보 요청)
	void			SendDBMsgServerEventRequest(void);							// 서버 이벤트에 관련된 모든 정보 요청.(전체 랭킹 정보 요청)
	void			SendDBMsgServerRankRequest(void);							// 서버 이벤트에 관련된 서버랭킹 정보 요청.
	void			SendDBMsgPersonalRankRequest(void);							// 서버 이벤트에 관련된 개인랭킹 정보 요청.
	void			SendDBMsgServerScoreAdd(void);								// 서버 이벤트에 서버점수 업데이트.
	void			SendDBMsgServerDailyScoreAdd(void);							// 서버 이벤트에 하루 서버점수 업데이트.

	void			ManagerExcute( SI32 siCommandType );

	void			InstanceMapClear(void);										// 인던 종료 시 호출. 
	void			InstanceMapClearReward(void);								// 사냥터(던젼) 이벤트 종료 후 클리어 횟수 조합에 따른 보상. 
	bool			IsMyServerRankBest(void);									// 내 서버가 랭킹 1위인가.
	bool			IsMyServerRankBest2(void);									// 내 서버가 랭킹 2위인가.
	void			SetBonusBufType(SI16 _siBonusBufType);						// 1위를 하고 보너스 버프를 얻은 타입 세팅. 
	SI16			GetBonusBufType(void);										// 1위를 하고 보너스 버프를 얻은 타입 얻기. 
	void			MyServerDailyRankBestBuf(void);								// 서버가 1위일 때 호출.
	void			MyServerDailyRankBestBufNo(void);							// 서버가 1위가 아닐 때 호출.
	SI32			GetBonusMapIndex(SI32 _Level);								// 유저의 레벨에 따라 보너스 맵의 인덱스를 준다.
	bool			IsEventMap(SI32 _siMapIndex);								// 해당 맵인덱스가 서버 이벤트 후 1등 서버에 보너스 게이지로 이동하는 맵인지 확인.

public:
	// 서버 랭킹 관련.
	void			SetServerUnique(SI32 _siServerIndex, SI16 _siServerUnique);
	SI16			GetServerUnique(SI32 _siServerIndex);
	void			SetServerScore(SI32 _siServerIndex, SI64 _siServerScore);
	SI64			GetServerScore(SI32 _siServerIndex);
	void			SetServerHuntingGroundCount(SI32 _siPersonalRank, SI16 _siServerHuntingGroundCount);
	SI16			GetServerHuntingGroundCount(SI32 _siPersonalRank);
	void			SetServerName(SI32 _siServerRank, TCHAR* _siServerName);
	TCHAR*			GetServerName(SI32 _siServerRank);

	// 하루 서버 랭킹 관련.
	void			SetServerUniqueDaily(SI32 _siServerIndex, SI16 _siServerUnique);
	SI16			GetServerUniqueDaily(SI32 _siServerIndex);
	void			SetServerDailyScore(SI32 _siServerIndex, SI64 _siServerScore);
	SI64			GetServerDailyScore(SI32 _siServerIndex);
	void			SetServerDailyHuntingGroundCount(SI32 _siPersonalRank, SI16 _siServerHuntingGroundCount);
	SI16			GetServerDailyHuntingGroundCount(SI32 _siPersonalRank);
	void			SetServerDailyName(SI32 _siServerRank, TCHAR* _siServerName);
	TCHAR*			GetServerDailyName(SI32 _siServerRank);
	

	// 개인 랭킹 관련.
	void			SetPersonalScore(SI32 _siServerRank, SI64 _siPersonalScore);
	SI64			GetPersonalScore(SI32 _siServerRank);
	void			SetPersonalLevel(SI32 _siPersonalRank, SI16 _siPersonalLevel);
	SI16			GetPersonalLevel(SI32 _siPersonalRank);
	void			SetPersonalName(SI32 _siPersonalRank, TCHAR* _siPersonalName);
	TCHAR*			GetPersonalName(SI32 _siPersonalRank);

	// 인던 정보 관련.
	void			SetInstanceMapScore(SI64 _siScore);
	SI64			GetInstanceMapScore(void);
	void			AddInstanceMapScore(SI64 _siAddScore);
	void			SetInstanceMapStepCount(SI16 _Index, SI16 _siMapStepCount);
	SI16 			GetInstanceMapStepCount(SI16 _Index);
	void			AddInstanceMapStepCount(SI16 _Index, SI16 _siMapStepCount);
	SI16 			GetInstanceMapStepCountSum(void);

	void			SetInstanceMapDailyOpenCount(SI32 _siOpenCount);
	SI32 			GetInstanceMapDailyOpenCount(void);
	void			AddInstanceMapDailyOpenCount(SI32 _siOpenCount);

	void			SetInstanceMapClear(SI16 _Index, bool _bMapClear);
	bool 			GetInstanceMapClear(SI16 _Index);
	bool			IsAllInstanceMapClear(void);

	void			BackUpDailyServerUniqueRankClear(void);
	void			SetBackUpDailyServerUniqueRank(void);
	SI16 			GetBackUpDailyServerUniqueRank(SI16 _Index);

	// 서버 이벤트 상태 설정
	void			SetServerEventState( SI32 _siServerEventState );
	void			ServerEventStateReady( void );
	SI32			GetServerEventState( void );

	void			ServerEventStateProcess( void );


	// 랭킹 정보 세팅.
	CNServerEvent_Rank_Record*	GetServerEventRankRecord(void);											// 서버, 개인 전체 랭킹정보 얻기.
	CNServerEvent_Quest*		GetServerEvent_Quest(void);												// 서버 이벤트 퀘스트 얻기.
	CNInstanceMapInfo*			GetInstanceMapInfo(void);												// 서버 이벤트 인던 관련 정보 얻기.

	void						SetServerEventRankRecord(CNServerEvent_Rank_Record* _pRankInfo);		// 서버, 개인 전체 랭킹정보 세팅.
	void						SetServerRank(CNServerRankInfo* _pRankInfo);							// 서버 랭킹정보 세팅.
	void						SetServerDailyRank(CNServerDailyRankInfo* _pRankInfo);					// 하루 서버 랭킹정보 세팅.
	void						SetPersonalRank(CNPersonalRankInfo* _pRankInfo);						// 개인 랭킹정보 세팅.
	
private:
	SI16						m_siBonusBufType;														// 보너스 버프 타입.
	SI32						m_siMsgFlag;															// 메시지 호출 관련 플레그.
#ifdef _SAFE_MEMORY
	NSafeTArray<SI32, SERVER_EVENT_BONUS_MAP>			m_ServerEventBonusMap;
#else
	SI32						m_ServerEventBonusMap[SERVER_EVENT_BONUS_MAP];							// 서버 이벤트 보너스 맵 인덱스.
#endif
	SI32						m_InstanceMapDailyOpenCount;											// 인트턴트 맵의 하루 개최 횟수.

	SI32						m_siServerEventState;													// 현재 서버 이벤트 상태
#ifdef _SAFE_MEMORY
	NSafeTArray<SI16, SERVER_MAX_NUM>					m_BackUpDailyServerUniqueRank;
#else
	SI16						m_BackUpDailyServerUniqueRank[SERVER_MAX_NUM];							// 하루 점수로 서버 랭킹을 임시 저장.      
#endif

private:
	CNServerEvent_Rank_Record		m_clServerEvent_Rank_Record;										// 서버이벤트 랭킹정보 기록.(배열 순서가 순위)
	CNServerEvent_Quest				m_clServerEvent_Quest;												// 서버이벤트 퀘스트.
	CNInstanceMapInfo				m_clInstanceMapInfo;												// 서버이벤트 인던관련 정보.


//******************************************************************************************
//
// 변경일자:  2009.12.18 ~ 
//
// * 서버 이벤트가 군주 데이 이벤트로 변경되어 사용되는 변수, 함수. 
//
// * 서버 이벤트는 사라짐.
//
//******************************************************************************************
public:	
	void				ActionGoonzuDay(void);				// 서버 이벤트를 군주데이 이벤트로 적용시킴. 
	void				ActionGoonzuDayReward(void);		// 보상기간 동안 정보만 요청함.
	bool				IsGoonzuDayEventSuccess( void );	// 군주 데이 이벤트 때 얻은 점수가 습득해야되는 최대 점수 이상이 되면 성공.
	void				DeleteGate( void );

private:	
	SI64				m_siGoonzuDayDayEventScore;
};

#endif