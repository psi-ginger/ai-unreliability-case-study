#ifndef _MINISTER_GOONZU_H_
#define _MINISTER_GOONZU_H_

#include "MinisterBase.h"
#include "MinisterMgr.h"

class CMinisterGoonzu : public CMinisterBase
{
public:
	CMinisterGoonzu();
	~CMinisterGoonzu();

	GMONEY	m_iTempFinances; // 임시 재정
	GMONEY	m_iTotalFinances; // 총 재정

#ifdef _SAFE_MEMORY
	NSafeTArray<SI32, 6>		m_siSetMinisterTime;
#else
	SI32	m_siSetMinisterTime[6] ;
#endif

	// kkm 군주의 파산마을변수는 쓰지 않음 2007 / 08 / 15
	// 호조가 설정한 후에 없애주자
	SI16	m_siLiquidationVillageNumber;

	SI16	m_siBonusTime;	// 군주 권한 보너스 타임

	SI16	m_siMakeBonusTime;

	SI16	m_siLastChangedBonusTimeYear;	// 군주 권한 보너스 타임이 변환된 년
	SI16	m_siLastChangedBonusTimeMonth;	// 군주 권한 보너스 타임이 변환된 월
	SI16	m_siLastChangedBonusTimeDay;	// 군주 권한 보너스 타임이 변환된 일


	void AddTempFinances(GMONEY money)
	{
		if ( money < 0 ) return ;
		m_iTempFinances += money ;
	}
	void AddTotalFinances(GMONEY money)
	{
		if ( money < 0 ) return ;
		m_iTotalFinances += money ;
	}

	void RemoveFinances(GMONEY money);

	INT64 GetTempFinances(){return m_iTempFinances;}
	INT64 GetTotalFinances(){return m_iTotalFinances;}

	SI16 GetLiquidationVillageNumber() { return m_siLiquidationVillageNumber; }

	void 	InitializeDB();
	void 	DoMsg_DBMsgResponse_GetMinisterInfo(sPacketHeader* pPacket);

	void 	DoMsg_GameMsgRequest_GetMinisterInfo(SI32 siID);

	void 	DoMsg_GameMsgRequest_SetMinisterInfo(cltMsg* pclMsg, SI32 siID);

	void 	DoMsg_DBMsgResponse_SetMinisterInfo(sPacketHeader* pPacket);

	// server.cpp의 serveraction에서 돌 것
	void	DoMsgDBMsgRequest_SetGoonzuFinances(clock_t currentclock) ;
	void	ActionGameServerInfo(clock_t currentclock);

};

#endif