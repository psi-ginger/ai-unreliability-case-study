#pragma once
//#include "../SoulGuardMgr/SoulGuardMgr.h"

// 경기 결과 알려주기
class cltGameMsgResponse_BattleRoyalNoticeResult
{
public:
	// 승리자 이름
	TCHAR	m_szWinnerName[MAX_PLAYER_NAME];
	// 승리 했는지 
	bool	m_bResult;
	// 경기장 맵
	SI32	m_siMapIndex;
public:
	cltGameMsgResponse_BattleRoyalNoticeResult(const TCHAR* pszWinnerName , bool bResult, SI32 siMapIndex)
	{
		ZeroMemory(m_szWinnerName, MAX_PLAYER_NAME * sizeof(TCHAR));

		if ( NULL != pszWinnerName)
			memcpy(m_szWinnerName, pszWinnerName, MAX_PLAYER_NAME * sizeof(TCHAR));
		
		m_bResult = bResult;
		m_siMapIndex = siMapIndex;
	}
};	// GAMEMSG_RESPONSE_BATTLEROYAL_NOTICERESULT

// 경기 시작시 까지 남은 시간 요청
class cltGameMsgRequest_BattleRoyalRemainedTime
{
public:
	cltGameMsgRequest_BattleRoyalRemainedTime()
	{

	}
};	// GAMEMSG_REQUEST_BATTLEROYAL_REMAINEDTIME

// 경기 시작시 까지 남은 시간 응답
class cltGameMsgResponse_BattleRoyalRemainedTime
{
public:
	SI32	m_siTime;
public:
	cltGameMsgResponse_BattleRoyalRemainedTime(SI32	siTime)
	{
		m_siTime = siTime;
	}
};	// GAMEMSG_RESPONSE_BATTLEROYAL_REMAINEDTIME

// 경기 시작 
class cltGameMsgResponse_BattleRoyalStartGame
{
public:
	SI32	m_siTime;	// 걸리는 시간
	SI32	m_siMode;	// 모드
public:
	cltGameMsgResponse_BattleRoyalStartGame(SI32 siTime, SI32 siMode)
	{
		m_siTime = siTime;
		m_siMode = siMode;
	}
};	// GAMEMSG_RESPONSE_BATTLEROYAL_STARTGAME

// 경기 종료
class cltGameMsgResponse_BattleRoyalEndGame
{
public:
	cltGameMsgResponse_BattleRoyalEndGame()
	{
	}
};	// GAMEMSG_RESPONSE_BATTLEROYAL_ENDGAME

// 경기 시작 
class cltGameMsgResponse_BattleRoyalPowerupMode
{
public:
	SI32	m_siCharUnique;	// 파워업 한 charid
	bool	m_bValue;
public:
	cltGameMsgResponse_BattleRoyalPowerupMode(SI32 siCharUnique, bool bValue)
	{
		m_siCharUnique = siCharUnique;
		m_bValue = bValue;
	}
};	// GAMEMSG_RESPONSE_BATTLEROYAL_POWERUPMODE