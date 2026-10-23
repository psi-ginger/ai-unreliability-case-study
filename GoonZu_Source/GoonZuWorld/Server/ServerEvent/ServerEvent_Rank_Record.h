#ifndef _SERVER_EVENT_RANK_RECORD_H_
#define _SERVER_EVENT_RANK_RECORD_H_

#pragma once

#include "CommonLogic.h"
#include "ServerEvent-Quest.h"

#define SERVER_MAX_NUM	 4
#define USER_MAX_NUM	 5

// 서버순위 랭킹 정보.(전체 서버)
class CNServerRankInfo
{
public:
	CNServerRankInfo()												{ ZeroMemory(this, sizeof(*this));										}
	~CNServerRankInfo()												{																		}

	void	SetServerUnique(SI16 _siServerUnique)					{ m_siServerUnique = _siServerUnique;									}
	SI16	GetServerUnique(void)									{ return m_siServerUnique;												}	
	void	SetHuntingGroundCount(SI16 _siHuntingGroundCount)		{ m_siHuntingGroundCount = _siHuntingGroundCount;						}
	SI16	GetHuntingGroundCount(void)								{ return m_siHuntingGroundCount;										}	
	void	SetScore(SI64 _siScore)									{ m_siScore = _siScore;													}
	SI64	GetScore(void)											{ return m_siScore;														}
	void	SetServerName(TCHAR* _szServerName)						{ if(NULL == _szServerName) return; StringCchCopy(m_szServerName, sizeof(m_szServerName), _szServerName);	}
	TCHAR*	GetServerName(void)										{ return m_szServerName;												}

	//DB 에서 사용.
	SI16*	GetpServerUnique(void)									{ return &m_siServerUnique;					}
	SI32	GetServerUniqueSize(void)								{ return sizeof(m_siServerUnique);			}
	SI16*	GetpHuntingGroundCount(void)							{ return &m_siHuntingGroundCount;			}
	SI32	GetHuntingGroundCountSize(void)							{ return sizeof(m_siHuntingGroundCount);	}
	SI64*	GetpScore(void)											{ return &m_siScore;						}
	SI32	GetScoreSize(void)										{ return sizeof(m_siScore);					}
	TCHAR*	GetpServerName(void)									{ return m_szServerName;					}
	SI32	GetServerNameSize(void)									{ return sizeof(m_szServerName);					}

public:
	SI16	m_siServerUnique;
	SI16	m_siHuntingGroundCount;
	SI64	m_siScore;
	TCHAR	m_szServerName[MAX_SERVER_NAME];
};

// 하루 서버순위 랭킹 정보.(전체 서버)
class CNServerDailyRankInfo
{
public:
	CNServerDailyRankInfo()											{ ZeroMemory(this, sizeof(*this));										}
	~CNServerDailyRankInfo()										{																		}

	void	SetServerUnique(SI16 _siServerUnique)					{ m_siServerUnique = _siServerUnique;									}
	SI16	GetServerUnique(void)									{ return m_siServerUnique;												}	
	void	SetHuntingGroundCount(SI16 _siHuntingGroundCount)		{ m_siHuntingGroundCount = _siHuntingGroundCount;						}
	SI16	GetHuntingGroundCount(void)								{ return m_siHuntingGroundCount;										}	
	void	SetScore(SI64 _siScore)									{ m_siScore = _siScore;													}
	SI64	GetScore(void)											{ return m_siScore;														}
	void	SetServerName(TCHAR* _szServerName)						{ if(NULL == _szServerName) return; StringCchCopy(m_szServerName, sizeof(m_szServerName), _szServerName);	}
	TCHAR*	GetServerName(void)										{ return m_szServerName;												}

	//DB 에서 사용.
	SI16*	GetpServerUnique(void)									{ return &m_siServerUnique;					}
	SI32	GetServerUniqueSize(void)								{ return sizeof(m_siServerUnique);			}
	SI16*	GetpHuntingGroundCount(void)							{ return &m_siHuntingGroundCount;			}
	SI32	GetHuntingGroundCountSize(void)							{ return sizeof(m_siHuntingGroundCount);	}
	SI64*	GetpScore(void)											{ return &m_siScore;						}
	SI32	GetScoreSize(void)										{ return sizeof(m_siScore);					}
	TCHAR*	GetpServerName(void)									{ return m_szServerName;					}
	SI32	GetServerNameSize(void)									{ return sizeof(m_szServerName);			}

public:
	SI16	m_siServerUnique;
	SI16	m_siHuntingGroundCount;
	SI64	m_siScore;
	TCHAR	m_szServerName[MAX_SERVER_NAME];
};

// 개인순위 랭킹 정보.(한 서버)
class CNPersonalRankInfo
{
public:
	CNPersonalRankInfo()											{ ZeroMemory(this, sizeof(*this));												}
	~CNPersonalRankInfo()											{																				}

	void	SetLevel(SI16 _siLevel)									{ m_siLevel = _siLevel;															}
	SI16	GetLevel(void)											{ return m_siLevel;																}
	void	SetScore(SI64 _siScore)									{ m_siScore = _siScore;															}
	SI64	GetScore(void)											{ return m_siScore;																}
	void	SetPersonalName(TCHAR* _siPersonalName)					{ if(NULL == _siPersonalName) return; StringCchCopy(m_siPersonalName, sizeof(m_siPersonalName), _siPersonalName);	}
	TCHAR*	GetPersonalName(void)									{ return m_siPersonalName;														}

	//DB 에서 사용.
	SI16*	GetpLevel(void)											{ return &m_siLevel;					}
	SI32	GetLevelSize(void)										{ return sizeof(m_siLevel);				}
	SI64*	GetpScore(void)											{ return &m_siScore;					}
	SI32	GetScoreSize(void)										{ return sizeof(m_siScore);				}
	TCHAR*	GetpPersonalName(void)									{ return m_siPersonalName;				}
	SI32	GetPersonalNameSize(void)								{ return sizeof(m_siPersonalName);		}


public:
	SI16	m_siLevel;
	SI64	m_siScore;
	TCHAR	m_siPersonalName[MAX_PLAYER_NAME];
};

class CNServerEvent_Rank_Record
{
public:
	CNServerEvent_Rank_Record();
	~CNServerEvent_Rank_Record();

	void						Clear(void);
	void						Destroy(void);
	
	CNServerRankInfo*			GetServerRankInfo(SI32 _Index);			
	CNPersonalRankInfo*			GetPersonalRankInfo(SI32 _Index);
	CNServerDailyRankInfo*		GetServerDailyRankInfo(SI32 _Index);

	void						SetCNServerEvent_Quest(void* _ServerEvent_Quest);
	CNServerEvent_Quest*		GetCNServerEvent_Quest(void);

private:
#ifdef _SAFE_MEMORY
	NSafeTArray<CNServerRankInfo,		SERVER_MAX_NUM>			m_clServerRankInfo;
	NSafeTArray<CNServerDailyRankInfo,	SERVER_MAX_NUM>			m_clServerDailyRankInfo;
	NSafeTArray<CNPersonalRankInfo,		USER_MAX_NUM>			m_clPersonalRankInfo;
#else
	CNServerRankInfo			m_clServerRankInfo[SERVER_MAX_NUM];			// 이벤트 종료시까지 누적되는 전체 서버점수.(포인트 점수)
	CNServerDailyRankInfo		m_clServerDailyRankInfo[SERVER_MAX_NUM];	// 하루 누적되는 전체 서버점수.(유저의 총점)
	CNPersonalRankInfo			m_clPersonalRankInfo[USER_MAX_NUM];			// 개인누적 점수.(각 서버의 1~5위)
#endif

	void*						m_pServerEvent_Quest;
};

#endif





