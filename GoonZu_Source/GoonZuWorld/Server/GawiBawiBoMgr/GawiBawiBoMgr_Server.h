#ifndef	__GAWIBAWIBOMGR_SERVER_H__
#define __GAWIBAWIBOMGR_SERVER_H__

#pragma once

#include <Directives.h>
#include "../../Common/CommonHeader.h"
#include "../../Client/client.h"

#include <vector>

class CGawiBawiBoMgr_Server
{
public:
	enum ENUM_GAWIBAWIBO_CALCULATE_RESULT
	{
		GAWIBAWIBO_RESULT_NONE,
		GAWIBAWIBO_RESULT_ABUSE_WIN,
		GAWIBAWIBO_RESULT_WIN,
		GAWIBAWIBO_RESULT_DRAW,
		GAWIBAWIBO_RESULT_LOSE,
		GAWIBAWIBO_RESULT_END,
	};

	enum ENUM_GAWIBAWIBO_CALCULATE_RESULT_CASE
	{
		GAWIBAWIBO_RESULT_VALIDCOUNT_NONE,
		GAWIBAWIBO_RESULT_VALIDCOUNT_MAXWIN,	// 최고연승을 했다.
		GAWIBAWIBO_RESULT_VALIDCOUNT_INVALID,	// 문제있는 값
		GAWIBAWIBO_RESULT_VALIDCOUNT_VALID,		// 유효한 값
	};

private:
	SI16			m_siStatus;
	SI16			m_siResult;
	SI32			m_siWInPercentage[GAWIBAWIBO_RESULT_END];
	vector<SI32>	m_vecStraightWins;
	bool			m_bAlwaysWin;				// gm명령어 관련 멤버변수
	SI32			m_siLimitWin;				// 연승 제한
	
public:
	bool	Initialize();	//	매니저 초기화
	void	Action();		//	Action

	bool	LoadGawiBawiBoFiles();	//	가위바위보 리소스 Read

	void	SetStatus(SI16 siStatus){m_siStatus = siStatus;}
	SI16	GetStatus(){return m_siStatus;}

	SI08	GetCalculateResult();
	GMONEY	GetChargedMoney();										//	차감된 금액을 리턴
	GMONEY  GetCalculateCumulativeMoney(SI08 siStraightWins);		//	누적금액 계산
	SI08	IsValidCount(SI08 siCount);								//	 연승 카운트값이 유효한값인가

	void	ToggleAlwaysWin(){m_bAlwaysWin = !m_bAlwaysWin;}		//   항상 이기는 모드 토글
	bool	IsAlwaysWin(){return m_bAlwaysWin;}						//	 항상 이기는 모드인가?
	void	SetLimitWin(SI32 siLimitWin);
	SI32	GetLimitWin(){return m_siLimitWin;}
	

public:
	CGawiBawiBoMgr_Server();
	~CGawiBawiBoMgr_Server();
};

#endif