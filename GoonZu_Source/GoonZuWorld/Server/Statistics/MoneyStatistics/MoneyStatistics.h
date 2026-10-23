//---------------------------------
// 2008/05/21 이강형
//---------------------------------
#ifndef _MONEYSTATISTICS_H
#define _MONEYSTATISTICS_H

#include "../../../Lib/Directives.h"
#include "../../NLib/NTCHARString.h"
#include "../../NLib/NUpdateTimer.h"
#include "../../../Lib/Directives.h"

class cltMoneyStatistics
{
protected:
	NTCHARString<32>*	m_pColumnNames;
	SI64*				m_Row;	
	NTCHARString<32>	m_kFileName;
	SI32				m_siColumnCount;

	NUpdateTimer		m_kUpdateTimer_Action;

public:
	cltMoneyStatistics()
	{
		m_pColumnNames	= NULL;
		m_Row			= NULL;
		m_siColumnCount = 0;
	}

	~cltMoneyStatistics()
	{
		if(m_pColumnNames)		delete [] m_pColumnNames;
		if(m_Row)				delete [] m_Row;
	}

public:
	virtual void Add(SI32 siColumn, SI64 siValue) = 0;

	void Action();

protected:
	virtual void Init(SI32 siColumnCount) = 0;

	bool ReadStatistics();
	bool WriteStatistics();

	bool CreateFile();
	bool ChangeExistFileName();

	int CheckColumn();
};


//------------------------------------------------------------------------------------------------------
// ADDMONEYSTATISTIC
//------------------------------------------------------------------------------------------------------
enum ADDMONEYSTATISTIC_COLUMN
{
	ADDMONEY_STATISTIC_DATE,
	//==========================================

	ADDMONEY_STATISTIC_DAILYQUEST,
	ADDMONEY_STATISTIC_LEVEL20,
	ADDMONEY_STATISTIC_ANKALANSTOCK30,
	ADDMONEY_STATISTIC_SKILLMONEY,
	ADDMONEY_STATISTIC_EVENT,
	ADDMONEY_STATISTIC_FATHER,	
	ADDMONEY_STATISTIC_SPECIALQUEST,
	ADDMONEY_STATISTIC_QUEST,
	ADDMONEY_STATISTIC_SELLNPC,
	ADDMONEY_STATISTIC_SYSTEMREWARD,
	ADDMONEY_STATISTIC_MONTHMONEY,
	ADDMONEY_STATISTIC_NPCBUYITEM,

	ADDMONEY_STATISTIC_DIVIDEND,
	ADDMONEY_STATISTIC_GUILDDUNGEON,

	//==========================================
	ADDMONEY_STATISTIC_TOTAL
};

class cltAddMoneyStatistics : public cltMoneyStatistics
{
protected:
	void Init(SI32 siColumnCount);

public:
	cltAddMoneyStatistics()
	{
		Init(ADDMONEY_STATISTIC_TOTAL);
	}

	~cltAddMoneyStatistics()
	{
	}

	void Add(SI32 siColumn, SI64 siValue)
	{
		if( siValue < 0 )		return;
		if( siColumn <= ADDMONEY_STATISTIC_DATE || siColumn >= ADDMONEY_STATISTIC_TOTAL )	return;

		m_Row[siColumn] += siValue;
	}

protected:
};

//------------------------------------------------------------------------------------------------------
// DELMONEYSTATISTIC
//------------------------------------------------------------------------------------------------------
enum DELMONEYSTATISTIC_COLUMN
{
	DELMONEY_STATISTIC_DATE,
	//==========================================

	DELMONEY_SERVICECHARGE,			// 수수료 - 건물로 들어가는 돈
	DELMONEY_TAX,					// 세금 - 사라지는 돈
	DELMONEY_DONATE,				// 기부금
	DELMONEY_SAVEHOUSE,				// 클라라 물품 수수료
	DELMONEY_DEATHPANELTY,			// 사망 패널티
	DELMONEY_MARKETSERVICECHARGE,	// 마켓 등록 수수료
	DELMONEY_WARP,					// 워프 비용
	DELMONEY_ENTEREVENT,			// 이벤트 참여금
	DELMONEY_BUYNPC,				// NPC 물건 구입
	DELMONEY_POSTOFFICE,			// 우체국 이용료
	DELMONEY_STOCK,					// 주식구입비
	DELMONEY_GUILDCREATE,			// 길드 개설비 (계좌)
	DELMONEY_BANK_FEE,				// 사용료 (계좌)
	DELMONEY_BANK_SERVICECHARGE,	// 수수료 (계좌)
	DELMONEY_BUFF_ROULETTE,			// [진성] 버프룰렛을 이용한 비용.
	DELMONEY_MARKETCONDITIONS,		// [희영] 시세확인.
	DELMONEY_CHAR_ROULETTE,			// [강형] 변신룰렛을 이용한 비용.
	DELMONEY_NEWYEAREVENT_ROULETTE,	// [춘기] 신년이벤트 운세룰렛을 이용한 비용.
	DELMONEY_BALANCECURRENCY,		// [종호] 통화량 흡수 정책

	//==========================================
	DELMONEY_STATISTIC_TOTAL
};

class cltDelMoneyStatistics : public cltMoneyStatistics
{
protected:
	void Init(SI32 siColumnCount);

public:
	cltDelMoneyStatistics()
	{
		Init(DELMONEY_STATISTIC_TOTAL);
	}

	~cltDelMoneyStatistics()
	{
	}

	void Add(SI32 siColumn, SI64 siValue)
	{
		//if( siValue < 0 )		return;
		if( siColumn <= DELMONEY_STATISTIC_DATE || siColumn >= DELMONEY_STATISTIC_TOTAL )	return;

		m_Row[siColumn] += TABS(siValue);
	}

	void Del(SI32 siColumn, SI64 siValue)
	{
		if( siValue < 0 )		return;
		if( siColumn <= DELMONEY_STATISTIC_DATE || siColumn >= DELMONEY_STATISTIC_TOTAL )	return;

		m_Row[siColumn] -= siValue;
	}

protected:
};

#endif
