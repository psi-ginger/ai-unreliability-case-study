// 파티매칭
#ifndef _MSGTYPEPARTYMATCHINGQUEST_H_
#define _MSGTYPEPARTYMATCHINGQUEST_H_

#include "../../Common/CommonHeader.h"

#include "../../common/PartyMgr/PartyBaseInfo.h"
#include "../../CommonLogic/PartyMatchingMgr/Defines_PartyMatchingMgr.h"
#include "../../CommonLogic/PartyMatchingMgr/Info_PartyMatching.h"



//#include "../../CommonLogic/PartyMatchingMgr/Client_PartyMatchingMgr.h"




//////////////////////////////////////////////////////////////////////////


// 선택한 파티의 파티원 정보를 달라고 서버에 요청
class cltGameRequest_GetPartyMemberInfo
{
public:
	SI32 siLeaderUnique;

public:
	cltGameRequest_GetPartyMemberInfo(SI32 sileaderunique)
	{
		ZeroMemory( this, sizeof(cltGameRequest_GetPartyMemberInfo) );
		siLeaderUnique = sileaderunique;
	}
};

// 파티원의 정보를 찾아서 요청했던 클라에 보내줌
class cltGameResponse_GetPartyMemberInfo
{
public:
	SI32 siLeaderUnique;
	NTCHARString16 aPartyMemberNameList[MAX_PARTY_USER_NUM];

public:
	cltGameResponse_GetPartyMemberInfo(SI32 sileaderunique)
	{
		ZeroMemory( this, sizeof(cltGameResponse_GetPartyMemberInfo) );

		siLeaderUnique = sileaderunique;
	}
};

// 등록했던 파티매칭을 지우라는 패킷 (대화상자의 버튼 누를때 쓸것) - 내용 아무것도 없는게 정상.
class cltGameRequest_RemovePartyMatching
{
public:
	cltGameRequest_RemovePartyMatching()
	{
	}
};

// 예외상황 알림 패킷의 메세지 인포
class cltGameResponse_PartyMatchingException
{
public:
	SI16 siExceptionFlags;

public:
	cltGameResponse_PartyMatchingException(SI16 siexceptionflags)
	{
		ZeroMemory( this, sizeof(cltGameResponse_PartyMatchingException) );

		siExceptionFlags = siexceptionflags;
	}
};

//////////////////////////////////////////////////////////////////////////




// 파티 퀘스트 수행 가능 레벨 체크 요청
class cltGameRequest_PartyMatchingLevel
{
public:
	SI32 m_siPersonID;
	SI16 m_siLevelType;

public:
	cltGameRequest_PartyMatchingLevel( SI32 PersonID, SI16 siLevelType )
	{
		m_siPersonID = PersonID;
		m_siLevelType = siLevelType;
	}
};

// 파티 퀘스트 수행 가능 레벨 체크 응답
class cltGameResponse_PartyMatchingLevel
{
public:
#ifdef _SAFE_MEMORY
	NSafeTArray<CInfo_PartyMatching, MAX_PARTYMATCHING_LIST_SEND_SIZE>		m_PartyMatchingList;
#else
	CInfo_PartyMatching														m_PartyMatchingList[MAX_PARTYMATCHING_LIST_SEND_SIZE];
#endif

	bool bNeedClearOldList;

public:
	cltGameResponse_PartyMatchingLevel()
	{
		ZeroMemory( this, sizeof(CInfo_PartyMatching) );
	}

	// 전달받은 siIndex위치(m_PartyMatchingList 배열에서의 인덱스)에 파티 매칭 정보를 넣는다. (자동화 하지 않는다. 사용자 측에서 인덱스 값을 컨트롤 하도록 책임을 준다.)
	void AddPartyMatchingInfo(SI16 siIndex, CInfo_PartyMatching& rPartyMatchingInfo)
	{
		if ( 0 > siIndex || MAX_PARTYMATCHING_LIST_SEND_SIZE <= siIndex ) return;

		memcpy( &m_PartyMatchingList[siIndex], &rPartyMatchingInfo, sizeof(rPartyMatchingInfo) );

		/*m_PartyMatchingList[siIndex].m_siLeaderID = rPartyMatchingInfo.m_siLeaderID;
		m_PartyMatchingList[siIndex].m_strLeaderName = rPartyMatchingInfo.m_strLeaderName;
		m_PartyMatchingList[siIndex].m_strPartyName = rPartyMatchingInfo.m_strPartyName;
		m_PartyMatchingList[siIndex].m_strHelloMessage = rPartyMatchingInfo.m_strHelloMessage;
		m_PartyMatchingList[siIndex].m_siParentQuest = rPartyMatchingInfo.m_siParentQuest;
		m_PartyMatchingList[siIndex].m_siChildQuest = rPartyMatchingInfo.m_siChildQuest;

		for ( SI16 i = 0; i < MAX_PARTY_USER_NUM; ++i )
		{
			m_PartyMatchingList[siIndex].m_PartyMemberIDList[i] = rPartyMatchingInfo.m_PartyMemberIDList[i];
		}*/
	}
};


// 새로운 파티매칭을 등록하겠다는 request 패킷의 메세지 인포
class cltGameRequest_PartyMatchingCreate
{
public:
	SI32 m_siCharID;
	CInfo_PartyMatching m_Info;

public:
	cltGameRequest_PartyMatchingCreate(CInfo_PartyMatching& rInfo)
	{
		ZeroMemory( this, sizeof(cltGameRequest_PartyMatchingCreate) );

		Set_PartyMatchingInfo( rInfo );
	}

	void Set_PartyMatchingInfo(CInfo_PartyMatching& rInfo)
	{
		memcpy( &m_Info, &rInfo, sizeof(rInfo) );
	}
};

// 서버에 정상적으로 파티매칭이 등록 되었다는 response 패킷의 메세지 인포
class cltGameResponse_PartyMatchingCreate
{
public:
	SI32 m_siCharID;
	bool m_bResult;

public:
	cltGameResponse_PartyMatchingCreate(bool bResult)
	{
		ZeroMemory( this, sizeof(cltGameResponse_PartyMatchingCreate) );

		m_bResult = bResult;
	}
};















class cltGameRequest_PartyCreateConditionCheck
{
public:
	SI32 m_siPersonID;

public:
	cltGameRequest_PartyCreateConditionCheck(SI32 PersonID)
	{
		m_siPersonID = PersonID;
	}
};

class cltGameResponse_PartyCreateConditionCheck
{
public:
	SI32 m_siPersonID;


public:
	cltGameResponse_PartyCreateConditionCheck(SI32 PersonID, CInfo_PartyMatching& rPartyInfo)
	{
		ZeroMemory( this, sizeof(cltGameResponse_PartyCreateConditionCheck) );

		m_siPersonID = PersonID;
	}

};



#endif