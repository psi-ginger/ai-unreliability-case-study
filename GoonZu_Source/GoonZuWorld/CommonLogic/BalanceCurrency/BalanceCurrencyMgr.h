#pragma once

//////////////////////////////////////////////////////////////////////////
//	Title		: [종호] 2000526 통화량 흡수 정책
//
//	Description : 게임내의 통화량을 조절하기 위한 manager 입니다.
//				  개인상점내의 영웅동상에서 +된 통화량을 흡수 합니다.
//////////////////////////////////////////////////////////////////////////

#include <Directives.h>
#include "../../Common/CommonHeader.h"

//	통화량 갱신 상태
enum CurrencyUpdateState	
{
	CURRENCY_UPDATE_NONE,
	CURRENCY_UPDATE_TODAY_DONE,					//	DB로부터 오늘 통화량 갱신 완료
	CURRENCY_UPDATE_ALL_DONE,					//	DB로부터 어제 통화량 갱신 완료
};


class CBalanceCurrencyMgr
{
private:
	enum NPCCoordinate
	{
		BALANCECURRENCY_X	=	37,
		BALANCECURRENCY_Y	=	36,
	};

	GMONEY	m_siCanUseMoney;						//	사용할수 있는 통화량
	GMONEY	m_siTodayCurrency;						//	현재 DB로부터 받아온 총 통화량
	GMONEY	m_siYesterdayCurrency;					//	어제 DB로부터 받아온 총 통화량

	SI32	m_siUpdateState;						//	통화량 갱신 상태

	bool	m_bNeedToCalculateCurrency;				//	통화량 계산이 필요한가.

private:
	SI32	m_siBalanceCurrencyID;					//	통화량 조절 NPC의 캐릭터ID


public:
	bool	CreateBalanceCurrencyNPC();				//	통화량 조절 NPC 생성
	void	Action();
	
	GMONEY	GetBuffCost();					//	npcrate가 적용된 비용
	void	CalculateBalanceCurrency();	
	SI32	GetBalanceCurrencyNPC_ID(){return m_siBalanceCurrencyID;}

	void	SetCanUserMoney(){m_siCanUseMoney = m_siTodayCurrency - m_siYesterdayCurrency;}	//	Server에서 CanUserMoney()사용할 경우에는 이 함수를 사용
	void	SetCanUserMoney(GMONEY siMoney){m_siCanUseMoney = siMoney;}						//	Client기준으로 Server에서 받아올경우에는 이 함수를 사용해서 바로 Money적용
	GMONEY	GetCanUseMoney(){return m_siCanUseMoney;}
    

	void	SetTodayCurrency(GMONEY siMoney){m_siTodayCurrency = siMoney;}
	GMONEY	GetTodayCurrency(){return m_siTodayCurrency;}

	void	SetYesterDayCurrency(GMONEY siMoney){m_siYesterdayCurrency = siMoney;}
	GMONEY	GetYesterDayCurrency(){return m_siTodayCurrency;}

	void	SetUpdateState(SI32 siState){m_siUpdateState = siState;}
	SI32	GetUpdateState(){return m_siUpdateState;}	


public:	
	CBalanceCurrencyMgr(void);
	~CBalanceCurrencyMgr(void);
};
