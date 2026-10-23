#ifndef _MSGTYPEVOTE_H_
#define _MSGTYPEVOTE_H_

#include "..\..\Common\VoteMgr\VoteMgr.h"


class cltGameMsgResponse_VoteKingAvailable
{
public:
	TCHAR			strVoteExplain[ MAX_VOTEEXPLAIN_LENGTH ];
	stCandidature	sCandidature[ MAX_CANDIDATURE_NUM ];

	cltGameMsgResponse_VoteKingAvailable( TCHAR *VoteExplain, stCandidature *pCandidature )
	{
		MStrCpy( strVoteExplain, VoteExplain, MAX_VOTEEXPLAIN_LENGTH );
		memcpy( sCandidature, pCandidature, sizeof( stCandidature ) * MAX_CANDIDATURE_NUM );
	}
};

class cltGameMsgRequest_VoteKingSelection
{
public:

	SI16 siListIndex;

	cltGameMsgRequest_VoteKingSelection( SI16 ListIndex )
	{
		siListIndex = ListIndex;
	}
};

class cltGameMsgResponse_VoteKingSelection
{
public:
	SI16 siListIndex;
	TCHAR strCharName[ MAX_PLAYER_NAME ];

	cltGameMsgResponse_VoteKingSelection( SI16 ListIndex, TCHAR *CharName )
	{
		siListIndex = ListIndex;
		MStrCpy( strCharName, CharName, MAX_PLAYER_NAME );
	}
};

class cltGameMsgResponse_VoteKingAggregate
{
public:
	TCHAR				pCharNames[MAX_CANDIDATURE_NUM][MAX_PLAYER_NAME];		// 후보자들의 이름
	SI32				siVoteCounts[MAX_CANDIDATURE_NUM];		// 후보자들의 득표수

	cltGameMsgResponse_VoteKingAggregate( TCHAR **CharNames, SI32* VoteCounts )
	{
		memcpy(siVoteCounts, VoteCounts, sizeof(SI32) * MAX_CANDIDATURE_NUM);
		memcpy(pCharNames, CharNames, MAX_PLAYER_NAME * MAX_CANDIDATURE_NUM * sizeof(TCHAR));
	}
};

// [영훈] 선거시스템 : 유세 통보용 패킷
class cltGameMsgRequest_ElectionStump
{
public:
	SI16 m_siCharUnique;	// 유세를 하고있는 캐릭터 유니크


	cltGameMsgRequest_ElectionStump( SI16 siCharUnique )
	{
		m_siCharUnique = siCharUnique;
	}
};



// [영훈] 선거시스템 : 유세 시작용 페킷
class cltGameMsgRequest_StartElectionStump
{
public:
	TCHAR		m_tszCandidate[ Const_Max_Candidate ];		// 지지 후보자 이름
	TCHAR		m_tszMaintenance[ Const_Max_Maintenance ];	// 지지이유


	cltGameMsgRequest_StartElectionStump( TCHAR* pszCandidate, TCHAR* pszMaintenance )
	{
		StringCchCopy( m_tszCandidate, Const_Max_Candidate, pszCandidate );
		StringCchCopy( m_tszMaintenance, Const_Max_Maintenance, pszMaintenance );
	}
};

// [영훈] 선거시스템 : 유세 내용 전달
class cltGameMsgResponse_StartElectionStump
{
public:
	SI32		m_siCharUnique;								// 유세자 캐릭터 Unique
	TCHAR		m_tszCandidate[ Const_Max_Candidate ];		// 지지 후보자 이름
	TCHAR		m_tszMaintenance[ Const_Max_Maintenance ];	// 지지이유


	cltGameMsgResponse_StartElectionStump( SI32 siCharUnique, TCHAR* pszCandidate, TCHAR* pszMaintenance )
	{

		m_siCharUnique = siCharUnique;

		StringCchCopy( m_tszCandidate, Const_Max_Candidate, pszCandidate );
		StringCchCopy( m_tszMaintenance, Const_Max_Maintenance, pszMaintenance );
	}
};

// [영훈] 선거시스템 : 유세 종료
class cltGameMsgResponse_EndElectionStump
{
public:
	SI32		m_siCharUnique;								// 유세자 캐릭터 Unique

	cltGameMsgResponse_EndElectionStump( SI32 siCharUnique )
	{
		m_siCharUnique = siCharUnique;
	}
};


// [영훈] 군주선거 개표 시작
class cltGameMsgResponse_StartVoteStatus
{
public:
	TCHAR	m_tszCandidateName[MAX_CANDIDATURE_NUM][MAX_PLAYER_NAME];	// 후보자 이름
	SI08	m_siCandidateNum;											// 후보자 수

	SI32	m_siTotalVote;												// 전체 투표수
	SI32	m_siTopVote;												// 최대 득표수

	cltGameMsgResponse_StartVoteStatus( TCHAR** ppszCandidateName, SI08 siCandidateNum, SI32 siTotalVote, SI32 siTopVote )
	{
		memcpy( m_tszCandidateName, ppszCandidateName, ( sizeof(TCHAR) * MAX_CANDIDATURE_NUM * MAX_PLAYER_NAME ) );

		m_siCandidateNum	= siCandidateNum;

		m_siTotalVote		= siTotalVote;
		m_siTopVote			= siTopVote;
	}
};

// [영훈] 군주선거 개표 현황
class cltGameMsgResponse_VoteStatus
{
public:
	SI32	m_siNowVote[MAX_CANDIDATURE_NUM]; // 현재 각 후보자별 개표현황

	cltGameMsgResponse_VoteStatus( SI32 *psiNowVote )
	{
		memcpy( m_siNowVote, psiNowVote, (sizeof(SI32) * MAX_CANDIDATURE_NUM) );
	}
};

#endif
