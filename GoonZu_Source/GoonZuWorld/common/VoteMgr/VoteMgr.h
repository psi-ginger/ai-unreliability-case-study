#ifndef _VOTEMGR_H_
#define _VOTEMGR_H_

#include <Directives.h>
#include "CommonLogic.h"

#include "VoteBaseInfo.h"

struct stCandidature
{
public:
	SI32 siListIndex;
	TCHAR strChatName[ MAX_PLAYER_NAME ];	
	TCHAR strWebPageURL[MAX_PATH];

	stCandidature()
	{
		memset( this, 0, sizeof( stCandidature ) );
	}
};

class CVoteMgr
{
public:
	CVoteMgr();
	~CVoteMgr();

	void Initialize();
	void Destroy();

	bool ReadVoteData();

	void SetVoteStatus( bool bSetPlay );

	bool IsPlayStatus();
	
	void VoteStop();
	void VotePlay();

	void OpenTheGate( bool bOpen );
	void LeavePoolMap();

	stCandidature	*GetCandidatureList();
	TCHAR			*GetVoteExplain();

private:

	stCandidature	*m_pstCandidature;
	bool			m_bPlayStatus;
	
	TCHAR			*m_strVoteExplain;

	SI32			m_siGateID;		// [영훈] 투표소 게이트를 삭제할때 쓰인다
};


#endif