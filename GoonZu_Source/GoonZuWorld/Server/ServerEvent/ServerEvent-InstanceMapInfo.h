#ifndef _SERVER_EVENT_INSTANCEMAPINFO_H_
#define _SERVER_EVENT_INSTANCEMAPINFO_H_

#pragma once

#include "../CommonLogic/CommonLogic.h"

//#define SERVER_EVENT_INSTANCE_MAP_START_INDEX 357
#define SERVER_EVENT_INSTANCE_MAP			  4	

class CNInstanceMapInfo
{
public:
	CNInstanceMapInfo();	
	~CNInstanceMapInfo();

	void		Clear(void);
	void		DeleteGate(void);
	void		Action(void);
	
	// 인던 개최 수.
	void		SetScore(SI64 _siScore);
	SI64 		GetScore(void);
	void		AddScore(SI64 _siAddScore);
		
	// 인던안의 사냥터의 클리어 단계.
	void		SetMapClear(SI16 _Index, bool _bMapClear);
	bool 		GetMapClear(SI16 _Index);
    bool		IsAllMapClear(void);
	void		SetMapStepCount(SI16 _Index, SI16 _siMapStepCount);
	SI16 		GetMapStepCount(SI16 _Index);
	void		AddMapStepCount(SI16 _Index, SI16 _siOpenCount);
	SI16 		GetMapStepCountSum(void);
	
	// 인던시간
	void		SetStartTime( DWORD _dwStartTime );
	DWORD		GetStartTime( void );
	
	// 게이트 관련
	void		OpenTheGate( void );
	void		CloseTheGate( void );
	void		LeaveLobbyInUser( void );
	
private:

#ifdef _SAFE_MEMORY
	NSafeTArray<bool, SERVER_EVENT_INSTANCE_MAP>		m_bMapClear;
	NSafeTArray<SI16, SERVER_EVENT_INSTANCE_MAP>		m_siMapStepCount;
#else
	bool			m_bMapClear[SERVER_EVENT_INSTANCE_MAP];									// 각 인던들의 종료.
	SI16			m_siMapStepCount[SERVER_EVENT_INSTANCE_MAP];							// 인던 사냥터 진행 숫자.(단계)
#endif
	SI64			m_siScore;																// 개최 후 사냥한 점수.(하루 누적 점수에 적용)

	DWORD			m_dwStartTime;															// 인던 시작시간
	SI32			m_siGateID;																// 인던 로비용 게이트 ID

};

#endif



