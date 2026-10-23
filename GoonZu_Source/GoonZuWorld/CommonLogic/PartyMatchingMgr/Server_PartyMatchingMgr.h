// 파티매칭
#ifndef _SERVER_PARTYMATCHING_MANAGER_H_
#define _SERVER_PARTYMATCHING_MANAGER_H_

#include "../../Common/CommonHeader.h"
#include "Info_PartyMatching.h"

#include "../../CommonLogic/PartyQuestMgr/PartyQuestMgr_Common.h"





////////////////////////////////////////////////////////////////////////
//서버쪽 파티 매칭 매니저 : 파티 리스트 관리
////////////////////////////////////////////////////////////////////////


class CServer_PartyMatchingMgr
{
public:
	CServer_PartyMatchingMgr();
	~CServer_PartyMatchingMgr();


	//--------- 파티정보 객체 관리 기능
protected:
	// 파티정보 객체의 리스트 (NkRbTreeMap이용)
	typedef NkRbTreeMap<SI32, CInfo_PartyMatching> PMLIST;
	PMLIST m_aPMList[MAX_PARTYQUEST_CATEGORY_NUM];

	DWORD m_dwWaitListRefreshTime;

protected:
	void _Refresh_UserList();

public:
	// 필요 인터페이스들
	void Remove_PartyMatching(SI32 siLeaderID);
	TCHAR* GetPartyName(SI32 siLeaderUnique);
	bool IsExist(SI32 siLeaderUnique);


	//--------- 서버 파티매칭 매니저의 기능
protected:
	// send Packet to CLIENT
	void _sendpacket_PartyMatchingList(SI32 siCharID, const PMLIST& rPMList);
	void _sendpacket_PartyMatchingCreate(SI32 siCharID, bool bSuccess);

public:
	// default
	void Action();

	// msg process
	void MsgProcess_Request_PartyMatchingLevel(SI32 siCharID, SI16 siLevelType);
	void MsgProcess_Request_PartyMatchingCreate(SI32 siCharID, CInfo_PartyMatching& rInfo);

	SI32 Get_Category(SI32 siLeaderUnique);
	SI32 Get_Mission(SI32 siLeaderUnique);
};

#endif				//_SERVER_PARTYMATCHING_MANAGER_H_