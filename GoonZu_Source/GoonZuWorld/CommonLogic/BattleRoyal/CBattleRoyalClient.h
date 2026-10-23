//
//
//  @ File Name : CBattleRoyalClient.h
//  @ Date : 2009-09-25
//  @ Author : 박기형
//	@ Desc : 배틀로얄 처리 클래스
//


#if !defined(_CBATTLEROYALCLIENT_H)
#define _CBATTLEROYALCLIENT_H

#include "../CommonLogic.h"

#include "./CBattleRoyalGameMode.h"
#include "./CBattleRoyalReward.h"
#include "./CBattleRoyalStadium.h"

class CBattleRoyalGameMode;
class CBattleRoyalReward;
class CBattleRoyalStadium;

class CBattleRoyalClient {

public:
	CBattleRoyalClient();
	virtual ~CBattleRoyalClient();

	BOOL	Create();		// 생성
	BOOL	NoticeResult(bool bResult, TCHAR* pszWinnerName, SI32 siMapIndex);	// 결과 알리기
	BOOL	RequestTime();	// 시간 요청
	BOOL	ShowTime(SI32 siTime);		// 시간 보여주기
	void	SetGameMode(SI32 siMode);
	void	ShowTimerForGamingState(SI32 siTime); 
	void	HideTimerForGamingState(); 
	BOOL	IsTransform(cltCharClient* pclChar);
	void	ShowWarpButton();
protected:
	cltClient* m_pclClient;

	SI32 m_siMode;

	CBattleRoyalGameMode*		m_pclGameMode;	// 게임 모드
	CBattleRoyalReward*			m_pclReward;	// 보상
	CBattleRoyalStadium*		m_pclStadium;	// 경기장
};

#endif  //_CBATTLEROYALCLIENT_H