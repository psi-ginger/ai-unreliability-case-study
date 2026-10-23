#ifndef _WARRANK_H
#define _WARRANK_H

#include <Directives.h>
#include "..\..\Common\CommonHeader.h"
#include "..\..\CommonLogic\BlackArmy\BlackArmy.h"


class cltWarRankPersonInfo
{
public:
	int			m_ranknum;
	int			m_personid;
	char		m_szPlayerName[ MAX_PLAYER_NAME ];
	int			m_score;

	cltWarRankPersonInfo()
	{
		ZeroMemory( this, sizeof( cltWarRankPersonInfo ) );
	}

	cltWarRankPersonInfo( int ranknum, int personid, char *name, int score )
	{
		m_ranknum = ranknum;
		m_personid = personid;
		MStrCpy( m_szPlayerName, name, MAX_PLAYER_NAME );
		m_score = score;
	}
};


class cltWarRankInfo
{
public:
	int						m_warmapindex;
	
#ifdef _SAFE_MEMORY
	NSafeTArray<cltWarRankPersonInfo, MAX_WARRANK_PERSON_PER_MAP>	clRankPerson;
#else
	cltWarRankPersonInfo	clRankPerson[ MAX_WARRANK_PERSON_PER_MAP ];
#endif


	cltWarRankInfo()
	{
		ZeroMemory( this, sizeof( cltWarRankInfo ) );
	}

	void Init();
	void SetRankPerson( int ranknum, int personid, char *name, int score );
	cltWarRankPersonInfo* GetRankPerson( int ranknum );

};


class cltWarRankManager
{
public:
#ifdef _SAFE_MEMORY
	NSafeTArray<cltWarRankInfo, MAX_BLACKARMY_MAP_CNT>	clRankInfo;
#else
	cltWarRankInfo	clRankInfo[ MAX_BLACKARMY_MAP_CNT ];
#endif

	cltWarRankManager()	{}

	void SetWarRankInfo( cltWarRankInfo *pWarRankInfo );
	cltWarRankInfo* GetWarRankInfo( int warmapindex );

	bool SetRankMVP( int warmapindex );
	bool IsRankMVPFromPersonID( SI32 personid );
};


#endif