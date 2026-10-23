// 파티매칭
#ifndef _INFO_PARTYMATCHING_H_
#define _INFO_PARTYMATCHING_H_

#include "../../Common/CommonHeader.h"
#include "../../common/PartyMgr/PartyBaseInfo.h"





// 각종 파티 정보들과 그 정보를 관리할 인터페이스 추가




class CInfo_PartyMatching
{
public:
	CInfo_PartyMatching();
	~CInfo_PartyMatching();

public:
	// party member info
	SI32				m_siLeaderID;
	NTCHARString32		m_strLeaderName;

	NTCHARString64		m_strPartyName;
	NTCHARString128		m_strHelloMessage;

#if _SAFE_MEMORY
	NSafeTArray<SI32, MAX_PARTY_USER_NUM> m_PartyMemberIDList;
#else
	SI32				m_PartyMemberIDList[MAX_PARTY_USER_NUM];
#endif

	// quest info
	SI32				m_siParentQuest;
	SI32				m_siChildQuest;

	// interfaces
public:
	bool CheckCondition();

	// 기존 파티 매니저 이용할경우, 이부분 대신 파티 유저 리스트 넘버가 필요. 일단 주석
	void SetLeaderID(SI32 siLeaderID);
	void SetLeaderName(TCHAR* pszLeaderName);
	void SetPartyName(TCHAR* pszPartyName);
	void SetHelloMessage(TCHAR* pszHelloMessage);

	SI32 GetLeaderID();
	TCHAR* GetLeaderName();
	TCHAR* GetPartyName();
	TCHAR* GetHelloMessage();

	void AddMemberID(SI32 siMemberID);
	void RemoveMemberID(SI32 siMemberID);

	void SetParentQuest(SI32 siParentQuest);
	void SetChildQuest(SI32 siChildQuest);

	SI32 GetParentQuest();
	SI32 GetChildQuest();
};

#endif				//_INFO_PARTYMATCHING_H_