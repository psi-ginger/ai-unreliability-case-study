#ifndef __PARTYQUESTMGR_CLIENT_H__
#define __PARTYQUESTMGR_CLIENT_H__

#include <Directives.h>
#include "../../CommonLogic/PartyQuestMgr/PartyQuestMgr_Common.h"
#include "../../common/Char/CharServer/Char-Server.h"


class cltCommonLogic;
struct stClientPartyUser;

class CPartyQuestMgr_Client : public CPartyQuestMgr_Common
{
public:
	PartyQuest_UnitData m_PartyQuestData;
	TCHAR				m_szQuestInfoForDlg[1024];

public:
	bool	Initialize();		//	매니저 초기화

	bool	DoCheckPartyQuest(SI32 siPartyUserNum, stClientPartyUser ClientPartyUser[]);	// 파티퀘스트 시작 가능 여부 체크
	bool	CheckAlreadyPartyQuest();														// 이미 퀘스트 중인지 체크
	bool	CheckPartyLeader();																// 파티퀘스트 파티장 체크
	bool	CheckUserNum(SI32 siPartyUserNum);												// 파티 인원 체크
	bool	CheckLevel(stClientPartyUser ClientPartyUser[]);								// 파티원의 레벨 체크

	void	StartPartyQuestData(PartyQuest_UnitData* PartyQuestData);
	void	UpdatePartyQuestData(PartyQuest_UnitData* PartyQuestData, DWORD dwQuestTime, bool bClear = false);	// 파티퀘스트 데이터 설정
	void	UpdatePartyInfoMsg(PartyQuest_UnitData* PartyQuestData, DWORD dwQuestTime, bool bClear);			// 파티퀘스트 갱신 메세지
	void	DoGiveUpPartyQuest();

public:
	CPartyQuestMgr_Client();
	~CPartyQuestMgr_Client();
};



#endif	//__PARTYQUESTMGR_CLIENT_H__