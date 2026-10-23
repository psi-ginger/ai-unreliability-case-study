#ifndef __PARTYQUESTMGR_SERVER_H__
#define __PARTYQUESTMGR_SERVER_H__

#include <Directives.h>
#include "../../CommonLogic/PartyQuestMgr/PartyQuestMgr_Common.h"
#include "../../common/Char/CharServer/Char-Server.h"


class cltCommonLogic;

class CPartyQuestMgr_Server : public CPartyQuestMgr_Common
{
private:

public:
	bool	Initialize();	//	매니저 초기화
	void 	UpdatePartyQuest(SI32 siCharID, SI32 siPartyIndex, SI32 siQuestType, SI32 siCondition, SI32 siAmount);	// 파티퀘스트 정보 갱신

	bool	DoCheckPartyQuest(CPartyObj* pPartyObject);		// 파티퀘스트 시작 가능 여부 체크
	bool	CheckAlreadyPartyQuest(CPartyObj* pPartyObject);
	bool	CheckPartyLeader(CPartyObj* pPartyObject);		// 파티퀘스트 파티장 체크
	bool	CheckUserNum(CPartyObj* pPartyObject);			// 파티 인원 체크
	bool	CheckLevel(CPartyObj* pPartyObject);			// 레벨 체크
	bool	CheckPCRoom(CPartyObj* pPartyObject);			// 피시방 전용 퀘스트 체크
	
	bool	IncreasePartyQuestCondition(SI32 siPartyIndex, SI32 siQuestType, SI32 siCondition, SI32 siAmount);		// 퀘스트 Amount증가
	bool	ProcessCurrentQuestClear(SI32 siPartyIndex, SI32 siCharID);	// 현재 진행중인 퀘스트를 클리어 했는지 체크
	bool	ProcessAllQuestClear(SI32 siPartyIndex);		// 모든 퀘스트를 클리어 했는지
	void	ProcessReward(SI32 siPartyIndex, SI32 siCharID);				// 퀘스트 보상
	bool	DoGiveUpPartyQuest(SI32 siPartyIndex, SI32 siLeaveCharID, SI32 siMessage);	// 파티퀘스트 포기시 
	void	UpdatePartyQuestTime(SI32 siPartyIndex, PartyQuest_UnitData* pPartyQuestData);
	bool	CheckSameArea(CPartyObj* pPartyObj);

public:
	CPartyQuestMgr_Server();
	~CPartyQuestMgr_Server();
};



#endif	//__PARTYQUESTMGR_SERVER_H__