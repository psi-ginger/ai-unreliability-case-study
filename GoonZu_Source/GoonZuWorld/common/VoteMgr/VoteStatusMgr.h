#pragma once

#include <Directives.h>
#include "CommonLogic.h"

#include "VoteBaseInfo.h"

class NUpdateTimer;

// 아래 숫자는 총 개표 시간을 뜻하기도 한다
const SI32 Const_VoteStatusMgr_End_Count = 30;

class CVoteStatusMgr
{

public:
	CVoteStatusMgr();
	~CVoteStatusMgr();

	void	Init();
	void	Action();

public:
	void	StartVoteStatus( SI32 *psiVote );
	void	SendVoteStatus();

	void	WriteResult( SI32 *psiVote );

private:
	bool	ReadCandidateName();


public:
	// 후보자 이름
	TCHAR			m_tszCandidateName[MAX_CANDIDATURE_NUM][MAX_PLAYER_NAME];

	SI32			m_siTopVoteNum;							// 최대 득표자의 득표수	
	SI08			m_siCandidateNum;						// 후보자 수
	SI32			m_siTotalVote;							// 총 투표수

private:
#ifdef _SAFE_MEMORY
	NSafeTArray<SI32, MAX_CANDIDATURE_NUM>		m_siVote;
	NSafeTArray<SI32, MAX_CANDIDATURE_NUM>		m_siSendVote;
#else
	SI32			m_siVote[MAX_CANDIDATURE_NUM];			// 후보자별 총 득표수
	SI32			m_siSendVote[MAX_CANDIDATURE_NUM];		// 클라이언트에 보낸 후보자별 득표수
#endif
	
	SI08			m_siTimeCounter;						// 60분후 꺼지도록 하기 위해

	bool			m_bStart;								// 개표가 시작되었다

	NUpdateTimer	m_kActionTimer;							// Action함수 전용 타이머

};