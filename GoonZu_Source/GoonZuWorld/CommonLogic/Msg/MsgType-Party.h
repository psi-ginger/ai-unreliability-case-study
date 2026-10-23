#ifndef _MSGTYPEPARTY_H_
#define _MSGTYPEPARTY_H_

#include "..\..\Common\CommonHeader.h"
#include "..\..\Common\PartyMgr\PartyBaseInfo.h"

/*
// 파티 생성 요청
class cltGameMsgRequest_Party_Create
{
public:	
	TCHAR strPartyName[ MAX_PARTY_NAME_LENGTH ];

	cltGameMsgRequest_Party_Create( TCHAR *PartyName )
	{
		strncpy( strPartyName, PartyName, MAX_PARTY_NAME_LENGTH - 1 );
		strPartyName[ MAX_PARTY_NAME_LENGTH - 1 ] = '\0';
	};	
};

// 파티 생성 응답( 몇번에 파티가 생성되었음을 알려줌 )
class cltGameMsgResponse_Party_Create
{
public:
	SI32 siPartyIndex;
	TCHAR strPartyName[ MAX_PARTY_NAME_LENGTH ];

	cltGameMsgResponse_Party_Create( SI32 PartyIndex, TCHAR *PartyName )
	{
		siPartyIndex = PartyIndex;
		strncpy( strPartyName, PartyName, MAX_PARTY_NAME_LENGTH - 1 );
		strPartyName[ MAX_PARTY_NAME_LENGTH - 1 ] = '\0';
	}
};
*/

class cltGameMsgRequest_Party_ChangeItemGetPattern
{
public:
	SI16 siItemGetPattern;
	cltGameMsgRequest_Party_ChangeItemGetPattern( SI16 ItemGetPattern)
	{
		siItemGetPattern = ItemGetPattern;
	}
};

class cltGameMsgResponse_Party_ChangeItemGetPattern
{
public:
	SI16 siItemGetPattern;
	cltGameMsgResponse_Party_ChangeItemGetPattern( SI16 ItemGetPattern)
	{
		siItemGetPattern = ItemGetPattern;
	}
};

class cltGameMsgRequest_Party_CreateInvite
{
public:	
	SI32 siInvitedCharUnique;
	cltGameMsgRequest_Party_CreateInvite( SI32 InvitedCharUnique )
	{
		siInvitedCharUnique = InvitedCharUnique;
	}
};

class cltGameMsgResponse_Party_CreateInvite
{
public:
	TCHAR strCharName[ MAX_PLAYER_NAME ];			// 캐릭터명
	SI32 siPartyLeaderPersonID;						// 파티장이 될려는 사람의 PersonID

	cltGameMsgResponse_Party_CreateInvite( TCHAR *CharName, SI32 PartyLeaderPersonID )
	{
		MStrCpy( strCharName, CharName, MAX_PLAYER_NAME );

		siPartyLeaderPersonID = PartyLeaderPersonID;
	}
};

class cltGameMsgRequest_Party_CreateJoin
{
public:
	SI32 siLeaderPersonID;							// 파티장이 될려는 사람의 PersonID

	cltGameMsgRequest_Party_CreateJoin( SI32 LeaderPersonID )
	{
		siLeaderPersonID = LeaderPersonID;
	}
};

class cltGameMsgResponse_Party_CreateJoin
{
public:
	SI32 siPartyIndex;
	SI32 siLeaderPersonID;
	SI32 siLeaderCharUnique;
	SI32 siLeaderLevel;
	TCHAR strLeaderCharName[ MAX_PLAYER_NAME ];

	cltCharPos clLeaderCharPos;

	SI32 siMemberPersonID;
	SI32 siMemberCharUnique;
	SI32 siMemberLevel;
	TCHAR strMemberCharName[ MAX_PLAYER_NAME ];

	cltCharPos clMemberCharPos;

	TCHAR szPartyName[ MAX_PARTY_NAME_LENGTH ];

	// 파티매칭 정보
	bool bIsPartyQuestParty;
	SI32 siPartyQuest_Category;
	SI32 siPartyQuest_Mission;

	cltGameMsgResponse_Party_CreateJoin( SI32 PartyIndex, SI32 LeaderPersonID, SI32 LeaderCharUnique, SI32 LeaderLevel, TCHAR *LeaderCharName,cltCharPos *LeaderCharPos , SI32 MemberPersonID, SI32 MemberCharUnique, SI32 MemberLevel, TCHAR *MemberCharName, cltCharPos* MemberCharPos,
		TCHAR* pszPartyName = NULL, bool bispartyquest = false, SI32 sicategory = -1, SI32 simission = -1)
	{
		ZeroMemory( this, sizeof(cltGameMsgResponse_Party_CreateJoin) );

		siPartyIndex = PartyIndex;
		siLeaderPersonID = LeaderPersonID;
		siLeaderCharUnique = LeaderCharUnique;
		siLeaderLevel = LeaderLevel;
		MStrCpy( strLeaderCharName, LeaderCharName, MAX_PLAYER_NAME );
		clLeaderCharPos.Set(LeaderCharPos);
		siMemberPersonID = MemberPersonID;
		siMemberCharUnique = MemberCharUnique;
		siMemberLevel = MemberLevel;
		MStrCpy( strMemberCharName, MemberCharName, MAX_PLAYER_NAME );
		clMemberCharPos.Set(MemberCharPos);

		if ( NULL != pszPartyName )
		{
			MStrCpy( szPartyName, pszPartyName, MAX_PARTY_NAME_LENGTH );
		}

		// 파티매칭 퀘스트 정보
		bIsPartyQuestParty = bispartyquest;
		siPartyQuest_Category = sicategory;
		siPartyQuest_Mission = simission;
	}
};

// 파티장이 초대 요청
class cltGameMsgRequest_Party_Invite
{
public:
	SI32 siInvitedCharUnique;		// 초대된 대상의 캐릭터 유니크값
	SI32 siPartyIndex;				// 초대할려는 파티 번호

	cltGameMsgRequest_Party_Invite( SI32 InvitedCharUnique, SI32 PartyIndex )
	{
		siInvitedCharUnique = InvitedCharUnique;
		siPartyIndex = PartyIndex;
	}
};

// 파티장이 초대한 대상에게 보냄
// 클라이언트 상에는 "파티장 모모씨가"에서 파티에 초대하셨습니다. 파티에 참여하시겠습니까? 이렇게 표시
class cltGameMsgResponse_Party_Invite
{
public:
	TCHAR strCharName[ MAX_PLAYER_NAME ];			// 캐릭터명
	SI32 siPartyLeaderPersonID;						// 파티장 PersonID
	SI32 siPartyIndex;								// 파티번호

	cltGameMsgResponse_Party_Invite( TCHAR *CharName, SI32 PartyLeaderPersonID, SI32 PartyIndex )
	{
		siPartyIndex = PartyIndex;

		siPartyLeaderPersonID = PartyLeaderPersonID;

		MStrCpy( strCharName, CharName, MAX_PLAYER_NAME );
	}
};

class cltGameMsgRequest_Party_InviteReject
{
public:
	SI32 siPartyLeaderPersonID;

	cltGameMsgRequest_Party_InviteReject( SI32 PartyLeaderPersonID )
	{
		siPartyLeaderPersonID = PartyLeaderPersonID;
	}
};

class cltGameMsgResponse_Party_InviteReject
{
public:
	TCHAR strCharName[ MAX_PLAYER_NAME ];

	cltGameMsgResponse_Party_InviteReject( TCHAR *CharName )
	{
		if ( NULL != CharName )
		{
			memcpy( strCharName, CharName, MAX_PLAYER_NAME - 1 );
			strCharName[ MAX_PLAYER_NAME - 1 ] = NULL;
		}
	}
};


class cltGameMsgRequest_Party_Join
{
public:
	SI32 siPartyIndex;
	SI32 siPartyLeaderPersonID;

	cltGameMsgRequest_Party_Join( SI32 PartyIndex, SI32 PartyLeaderPersonID )
	{
		siPartyIndex = PartyIndex;
		siPartyLeaderPersonID = PartyLeaderPersonID;
	}
};

class cltGameMsgResponse_Party_Join
{
public:
	SI32 siPartyIndex;
	SI16 siUserListIndex;
	SI16 siItemGetPattern;
	TCHAR szPartyName[MAX_PARTY_NAME_LENGTH];

	cltGameMsgResponse_Party_Join( SI32 PartyIndex, SI16 UserListIndex, SI16 ItemGetPattern, TCHAR* pszPartyName = NULL )
	{
		ZeroMemory( this, sizeof(cltGameMsgResponse_Party_Join) );

		siPartyIndex = PartyIndex;
		siUserListIndex = UserListIndex;
		siItemGetPattern = ItemGetPattern;

		if ( NULL != pszPartyName )
		{
			memcpy( szPartyName, pszPartyName, sizeof(TCHAR)*MAX_PARTY_NAME_LENGTH );
		}
	}
};

class cltGameMsgRequest_Party_Leave
{
public:
	SI32 siPartyIndex;

	cltGameMsgRequest_Party_Leave( SI32 PartyIndex )
	{
		siPartyIndex =  PartyIndex;
	}
};

class cltGameMsgRequest_Party_ForceLeave
{
public:
	SI32 siPartyIndex;
	SI32 siForceLeaveCharUnique;		// 강제퇴장할 캐릭터 유니크값

	cltGameMsgRequest_Party_ForceLeave( SI32 PartyIndex, SI32 ForceLeaveCharUnique )
	{
		siPartyIndex = PartyIndex;
		siForceLeaveCharUnique = ForceLeaveCharUnique;
	}
};

class cltGameMsgRequest_Party_AllUserList
{
public:
	SI32 siPartyIndex;

	cltGameMsgRequest_Party_AllUserList( SI32 PartyIndex )
	{
		siPartyIndex = PartyIndex;
	}
};

class cltGameMsgResponse_Party_Delete
{
public:
	SI32 siPartyIndex;

	cltGameMsgResponse_Party_Delete( SI32 PartyIndex )
	{
		siPartyIndex = PartyIndex;
	}
};

// 이 메시지는 파티원들 모두에게 보내서 파티원이 추가 됐다는 것을 알리는 역할을 함
// 파티에 가입 요청한 사람은 strCharName 이 필요 없지만, 나머지 파티원들은 필요하므로...
// 공통으로 쓰기 위해서 이렇게 했음.
class cltGameMsgResponse_Party_AddUser
{
public:	
	bool bPartyLeader;						// 파티장이냐, 파티원이냐를 알려 주기 위한것
	SI16 siUserListIndex;					// 유저 리스트에서 몇번째에 들어가는지...	
	SI32 siCharUnique;						// 이번에 파티에 참여하게 된 유저의 캐릭터 유니크 값
	SI32 siLevel;							// 레벨
	TCHAR strCharName[ MAX_PLAYER_NAME ];	
	cltCharPos clCharPos;

	cltGameMsgResponse_Party_AddUser( SI16 UserListIndex, SI32 CharUnique, bool PartyLeader, TCHAR *CharName, SI32 Level, cltCharPos* pclCharPos  )
	{
		siUserListIndex = UserListIndex;
		siCharUnique = CharUnique;
		bPartyLeader = PartyLeader;
		MStrCpy( strCharName, CharName, MAX_PLAYER_NAME );
		siLevel = Level;
		clCharPos.Set(pclCharPos);
	}
};

class cltGameMsgResponse_Party_DelUser
{
public:
	SI16 siUserListIndex;					// 유저 리스트 몇번째에 있는지.
	SI32 siCharUnique;						// 삭제된 유저의 캐릭터 유니크값

	cltGameMsgResponse_Party_DelUser( SI16 UserListIndex, SI32 CharUnique )
	{
		siUserListIndex = UserListIndex;
		siCharUnique = CharUnique;
	}
};

class cltGameMsgResponse_Party_ForceDelUser
{
public:
	SI16 siUserListIndex;					// 유저 리스트 몇번째에 있는지.
	SI32 siCharUnique;						// 삭제된 유저의 캐릭터 유니크값

	cltGameMsgResponse_Party_ForceDelUser( SI16 UserListIndex, SI32 CharUnique )
	{
		siUserListIndex = UserListIndex;
		siCharUnique = CharUnique;
	}
};

class cltGameMsgResponse_Party_ChangeLeader
{
public:
	SI16 siUserListIndex;					// 유저 리스트 몇번째에 있는지.
	SI32 siCharUnique;						// 파티장이 된 유저의 캐릭터 유니크값

	cltGameMsgResponse_Party_ChangeLeader( SI16 UserListIndex, SI32 CharUnique )
	{
		siUserListIndex = UserListIndex;
		siCharUnique = CharUnique;
	}
};

class cltGameMsgResponse_Party_NotifyExp
{
public:
	SI32 siCharUnique;
	SI16 siUserListIndex;	// 유저 리스트에서 몇번째에 들어가는지...
	SI32 siExp;

	cltGameMsgResponse_Party_NotifyExp( SI32 CharUnique, SI16 UserListIndex, SI32 Exp )
	{
		siCharUnique = CharUnique;
		siUserListIndex = UserListIndex;
		siExp = Exp;
	}
};

class cltGameMsgResponse_Party_NotifyItemGet
{
public:
	SI32 siCharUnique;
	cltItem clItem;

	cltGameMsgResponse_Party_NotifyItemGet( SI32 CharUnique, cltItem* pclItem)
	{
		siCharUnique = CharUnique;
		clItem.Set(pclItem);		
	}
};

class cltGameMsgResponse_Party_PartyUserPos
{
public:
	SI32 siCharUnique;
	cltCharPos clCharPos;

	cltGameMsgResponse_Party_PartyUserPos(SI32 CharUnique, cltCharPos* pclCharPos)
	{
		siCharUnique = CharUnique;
		clCharPos.Set(pclCharPos);
	}
};

class cltGameMsgResponse_Party_NotifyLevelUp
{
public:
	SI32 siCharUnique;
	SI32 siLevel;

	cltGameMsgResponse_Party_NotifyLevelUp(SI32 CharUnique, SI32 Level)
	{
		siCharUnique = CharUnique;
		siLevel = Level;
	}
};


//////////////////////////////////////////////////////////////////////////
// 요청의 방향이 [파티원 -> 파장] 인 처리를 위한 메세지 인포들
//////////////////////////////////////////////////////////////////////////

//--------------------------------------------------
class cltGameMsgRequest_Party_WantJoin
{
public:
	SI32 siLeaderUnique;
	SI32 siMemberUnique;
	TCHAR szMemberName[ MAX_PLAYER_NAME ];

	cltGameMsgRequest_Party_WantJoin(SI32 leaderunique, SI32 memberunique, TCHAR* pszmembername)
	{
		ZeroMemory( this, sizeof(cltGameMsgRequest_Party_WantJoin) );

		siLeaderUnique = leaderunique;
		siMemberUnique = memberunique;
		if ( NULL != pszmembername )
		{
			memcpy( szMemberName, pszmembername, sizeof(TCHAR)*MAX_PLAYER_NAME );
		}
	}
};

class cltGameMsgResponse_Party_WantJoin
{
public:
	SI32 siLeaderUnique;
	SI32 siMemberUnique;
	TCHAR szMemberName[ MAX_PLAYER_NAME ];
	SI32 siPartyIndex;

	cltGameMsgResponse_Party_WantJoin(SI32 leaderunique, SI32 memberunique, TCHAR* pszmembername, SI32 partyindex)
	{
		ZeroMemory( this, sizeof(cltGameMsgResponse_Party_WantJoin) );

		siLeaderUnique = leaderunique;
		siMemberUnique = memberunique;
		if ( NULL != pszmembername )
		{
			memcpy( szMemberName, pszmembername, sizeof(TCHAR)*MAX_PLAYER_NAME );
		}
		siPartyIndex = partyindex;
	}
};

//---------------------------------------------------
class cltGameMsgRequest_Party_WantJoinResult
{
public:
	bool bResult;
	SI32 siLeaderUnique;
	SI32 siMemberUnique;
	SI32 siPartyIndex;

	cltGameMsgRequest_Party_WantJoinResult(bool result, SI32 leaderunique, SI32 memberunique, SI32 partyindex)
	{
		ZeroMemory( this, sizeof(cltGameMsgRequest_Party_WantJoinResult) );

		bResult = result;
		siLeaderUnique = leaderunique;
		siMemberUnique = memberunique;
		siPartyIndex = partyindex;
	}
};

class cltGameMsgResponse_Party_WantJoinResult
{
public:
	bool bResult;
	SI32 siLeaderUnique;
	SI32 siMemberUnique;
	SI32 siPartyIndex;
	TCHAR szPartyName[ MAX_PARTY_NAME_LENGTH ];

	cltGameMsgResponse_Party_WantJoinResult(bool result, SI32 leaderunique, SI32 memberunique, SI32 partyindex, TCHAR* pszpartyname)
	{
		ZeroMemory( this, sizeof(cltGameMsgResponse_Party_WantJoinResult) );

		bResult = result;
		siLeaderUnique = leaderunique;
		siMemberUnique = memberunique;
		siPartyIndex = partyindex;
		if ( NULL != pszpartyname )
		{
			memcpy( szPartyName, pszpartyname, sizeof(TCHAR)*MAX_PARTY_NAME_LENGTH );
		}
	}
};

#endif






