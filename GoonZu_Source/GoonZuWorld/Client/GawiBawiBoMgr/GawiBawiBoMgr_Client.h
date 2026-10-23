#pragma once

#include <Directives.h>
#include "../../Common/CommonHeader.h"
#include "../client.h"
#include <vector>

class CGawiBawiBoMgr_Client
{
private:
	enum ENUM_GAWIBAWIBO_CALCULATE_RESULT
	{
		GAWIBAWIBO_RESULT_NONE,
		GAWIBAWIBO_RESULT_ABUSE_WIN,
		GAWIBAWIBO_RESULT_WIN,
		GAWIBAWIBO_RESULT_DRAW,
		GAWIBAWIBO_RESULT_LOSE,
		GAWIBAWIBO_RESULT_END,
	};

	enum ENUM_GAWIBAWIBO_MAX_TIME
	{
		GAWIBAWIBO_NEXTSTEP_TIME	= 1000,
		GAWIBAWIBO_MIN_TIME			= 5000,	
		GAWIBAWIBO_MAX_TIME			= 30000,	
	};

	SI16	m_siStatus;
	SI08	m_siResult;
	GMONEY	m_siCumulativeMoney;	//	누적금액

	SI08	m_siGawiBawiBoCount;	//	연승 카운트

	DWORD	m_Timer;

	SI32	m_siWInPercentage[GAWIBAWIBO_RESULT_END];
	vector<SI32> m_vecStraightWins;
	bool	m_bIsMaxWins;

public:
	enum GAWIBAWIBO_STATUS
	{
		GAWIBAWIBO_NONE,						//	none
		GAWIBAWIBO_INIT,						//	초기화
		GAWIBAWIBO_WAIT_PLAYER,					//	플레이어 기다리는중
		GAWIBAWIBO_DO_CHECK_MONEY,				//	플레이 가능 체크 루틴으로으로
		GAWIBAWIBO_NOW_CHECKING,				//	플레이 가능한지 체크중
		GAWIBAWIBO_DO_CHARGEDMONEY,				//	금액 차감로직으로
		GAWIBAWIBO_NOW_CHARGINGMONEY,			//	금액 차감중
		GAWIBAWIBO_WAIT_SELECT,					//	플레이어 선택 기다리는 모드로
		GAWIBAWIBO_NOW_WAIT_SELECT,				//	플레이어 선택 기다리는중
		GAWIBAWIBO_SELECT,						//	패 선택함
		GAWIBAWIBO_NOW_CALCULATE_SELECT,		//	승패 결과 연산중
		GAWIBAWIBO_CALCULATE_SELECT_COMPLETE,	//	결과 연산 완료
		GAWIBAWIBO_SHOW_CALCULATE_SELECT,		//	선택한 결과 보여주는 중
		GAWIBAWIBO_RESULT,						//	Calculate and Result
		GAWIBAWIBO_SHOW_CEREMONY,				//	이기고 진 세레모니 보여주기
		GAWIBAWIBO_RESULT_PROCESS,				//	결과에 따라 진행
		GAWIBAWIBO_NOW_RESULT_PROCESS,			//	결과에 따른 진행 중
		GAWIBAWIBO_END,
	};

public:
	bool		Initialize();	//	매니저 초기화
	bool		StartGame();	//	가위바위보 매니저 작동
	void		Action();		//	Action
	bool		LoadGawiBawiBoFiles();

	void		SetStatus(SI16 siStatus);
	SI16		GetStatus(){return m_siStatus;}
	void		EndGame(){m_siStatus = GAWIBAWIBO_WAIT_PLAYER; m_siGawiBawiBoCount = 0; m_siCumulativeMoney = 0; m_bIsMaxWins = false;}
	void		ReGame(){m_siStatus = GAWIBAWIBO_DO_CHECK_MONEY;}
	void		GiveUp();

	void		DrawGame(){m_siStatus = GAWIBAWIBO_WAIT_SELECT;}
	void		Lose();

	void		SetResult(SI08 siResult);	// 가위바위보 승패 결과
	SI08		GetResult(){return m_siResult;}

	inline void	NextSteps(){m_siStatus++;}	//	다음스텝 진행

	void		InitTimer();
	void		SetTimer(clock_t lnTime);
	bool		IsTimePassed();
	clock_t		GetTimePassed();
	GMONEY		GetCumulativeMoney(){return m_siCumulativeMoney;}	//	현재 누적금액 Get
	GMONEY		GetCalculateCumulativeMoney(SI08 siCount);			//	누적금액 계산
	void		SetCalculateCumulativeMoney(GMONEY siMoney){m_siCumulativeMoney = siMoney;}

	SI08		GetCount();
	void		SetCount(SI08 siCount){m_siGawiBawiBoCount = siCount;}
	
	void		SetMaximumWins(bool bMaximumWin){m_bIsMaxWins = bMaximumWin;}		// 최고연승시
	bool		GetMaximumWins(){return m_bIsMaxWins;}		// 최고연승시

public:
	CGawiBawiBoMgr_Client();
	~CGawiBawiBoMgr_Client();
};

