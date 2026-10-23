#ifndef _MSGTYPEFRIEND_H_
#define _MSGTYPEFRIEND_H_

#include "..\..\Common\CommonHeader.h"

// 친구 전체 리스트
class cltGameMsgResponse_FriendAllList
{
public:
	cltFriendInfo	clFriendInfo;	

	cltGameMsgResponse_FriendAllList()
	{
		ZeroMemory(this, sizeof(cltGameMsgResponse_FriendAllList));
	}
};

// 친구 추가
class cltGameMsgRequest_FriendAdd
{
public:
	SI08  siGroupID;
	TCHAR strFriendName[ MAX_PLAYER_NAME ];

	cltGameMsgRequest_FriendAdd( SI08 GroupID, TCHAR *FriendName )
	{
		siGroupID = GroupID;
		MStrCpy( strFriendName, FriendName, MAX_PLAYER_NAME );
	}
};

class cltGameMsgResponse_FriendAdd
{
public:
	SI08 siGroupID;
	SI32 siCharID;
	SI32 siPersonID;
	TCHAR strFriendName[ MAX_PLAYER_NAME ];
	cltGameMsgResponse_FriendAdd( SI08 GroupID, SI32 CharID, SI32 PersonID, TCHAR *FriendName )
	{
		siGroupID = GroupID;
		siCharID = CharID;
		siPersonID = PersonID;
		MStrCpy( strFriendName, FriendName, MAX_PLAYER_NAME );
	}
};

// 친구 삭제
class cltGameMsgRequest_FriendDel
{
public:
	SI32 siPersonID;
	cltGameMsgRequest_FriendDel( SI32 PersonID )
	{
		siPersonID = PersonID;
	}
};

class cltGameMsgResponse_FriendDel
{
public:
	SI16 siFriendListIndex;
	SI32 siPersonID;
	cltGameMsgResponse_FriendDel( SI16 FriendListIndex, SI32 PersonID )
	{
		siFriendListIndex = FriendListIndex;
		siPersonID = PersonID;
	}
};

// 친구 목록 요청
class cltGameMsgRequest_FriendList
{
public:
	cltGameMsgRequest_FriendList()
	{
	}
};

class cltGameMsgResponse_FriendList
{
public:
#ifdef _SAFE_MEMORY
	NSafeTArray<SI08, MAX_FRIEND_NUMBER>			siGroupID;
	NSafeTArray<cltSimplePerson, MAX_FRIEND_NUMBER>	clSimplePerson;
	NSafeTArray<SI32, MAX_FRIEND_NUMBER>			siMapIndex;
	NSafeTArray<SI32, MAX_FRIEND_NUMBER>			siLevel;
	NSafeTArray<SI32, MAX_FRIEND_NUMBER>			siHomeMapIndex;
	NSafeTArray<SI32, MAX_FRIEND_NUMBER>			siCharKind;
	NSafeTArray<cltSimpleRank, MAX_FRIEND_NUMBER>	clCharRank;
	NSafeTArray<UI08, MAX_FRIEND_NUMBER>			uiGRank;
#else
	SI08			siGroupID[MAX_FRIEND_NUMBER];
	cltSimplePerson	clSimplePerson[MAX_FRIEND_NUMBER];
	SI32			siMapIndex[MAX_FRIEND_NUMBER];
	SI32			siLevel[MAX_FRIEND_NUMBER];
	SI32			siHomeMapIndex[MAX_FRIEND_NUMBER];
	SI32			siCharKind[MAX_FRIEND_NUMBER];
	cltSimpleRank	clCharRank[MAX_FRIEND_NUMBER];
	UI08			uiGRank[MAX_FRIEND_NUMBER];
#endif

	cltGameMsgResponse_FriendList( SI08* groupid, cltSimplePerson* simpleperson, SI32* mapindex, SI32* level, SI32* homemapindex, SI32* charkind, cltSimpleRank* rank, UI08* gRank )
	{
#ifdef _SAFE_MEMORY
		clSimplePerson.MemCpy(simpleperson);
		clCharRank.MemCpy(rank);
#else
		memcpy( clSimplePerson, simpleperson, MAX_FRIEND_NUMBER * sizeof(cltSimplePerson) );
		memcpy( clCharRank, rank, MAX_FRIEND_NUMBER * sizeof(cltSimpleRank) );
#endif

		for ( SI16 i = 0 ; i < MAX_FRIEND_NUMBER ; i++ )
		{
			siGroupID[i] = groupid[i];
			siMapIndex[i] = mapindex[i];
			siLevel[i] = level[i];
			siHomeMapIndex[i] = homemapindex[i];
			siCharKind[i] = charkind[i];
			uiGRank[i] = gRank[i];
		}
	}	
};

// 친구 목록에 추가
class cltGameMsgResponse_FriendAddList
{
public:
	SI32 siCharID;
	SI32 siPersonID;
	TCHAR strFriendName[ MAX_PLAYER_NAME ];
	TCHAR szAccountID[MAX_PLAYER_NAME];
	cltGameMsgResponse_FriendAddList( SI32 CharID, SI32 PersonID, TCHAR *FriendName ,TCHAR * AccountID, TCHAR * CharNote )
	{
		siCharID = CharID;
		siPersonID = PersonID;
		MStrCpy( strFriendName, FriendName, MAX_PLAYER_NAME );
		MStrCpy( szAccountID, AccountID, MAX_PLAYER_NAME );
	}
};

// 친구 목록에서 삭제
class cltGameMsgResponse_FriendDelList
{
public:
	SI32 siPersonID;
	cltGameMsgResponse_FriendDelList( SI32 PersonID )
	{
		siPersonID = PersonID;
	}
};

// 친구로 등록된 사람에게 내가 로그인했음을 알림( MSN 에서 볼수 있지 )
class cltGameMsgResponse_FriendLogin
{
public:
	SI08 siGroupID;
	SI32 siPersonID;
	SI32 siCharID;
	TCHAR szAccountID[MAX_PLAYER_NAME];

	cltGameMsgResponse_FriendLogin( SI08 GroupID, SI32 PersonID, SI32 CharID ,TCHAR * AccountID)
	{
		siGroupID = GroupID;
		siPersonID = PersonID;
		siCharID = CharID;
		StringCchCopy(szAccountID, MAX_PLAYER_NAME, AccountID);
	}
};

// 친구로 등록된 사람에게 내가 로그아웃했음을 알림
class cltGameMsgResponse_FriendLogOut
{
public:	
	SI32 siPersonID;	

	cltGameMsgResponse_FriendLogOut( SI32 PersonID )
	{
		siPersonID = PersonID;
	}
};

class cltGameMsgRequest_FriendAddAccept
{
public:
	SI08 siGroupID;		// 추가될 그룹의 ID
	SI32 siPersonID;
	SI32 siCharID;
	TCHAR strCharName[ MAX_PLAYER_NAME ];
	
	cltGameMsgRequest_FriendAddAccept( SI08 GroupID, SI32 PersonID, SI32 CharID, TCHAR *CharName )
	{
		siGroupID = GroupID;
		siPersonID = PersonID;
		siCharID = CharID;
		MStrCpy( strCharName, CharName, MAX_PLAYER_NAME );
	}
};

class cltGameMsgResponse_FriendAddAccept
{
public:
	SI16 siFriendAddListIndex;
	SI08 siGroupID;
	SI32 siPersonID;
	SI32 siCharID;
	TCHAR strCharName[ MAX_PLAYER_NAME ];
	TCHAR szAccountID[MAX_PLAYER_NAME];

	cltGameMsgResponse_FriendAddAccept( SI16 FriendAddListIndex, SI08 GroupID, SI32 PersonID, SI32 CharID, TCHAR *CharName,TCHAR *AccountID)
	{
		siFriendAddListIndex = FriendAddListIndex;
		siGroupID = GroupID;
		siPersonID = PersonID;
		siCharID = CharID;
		MStrCpy( strCharName, CharName, MAX_PLAYER_NAME );
		MStrCpy( szAccountID, AccountID, MAX_PLAYER_NAME );
	}
};

class cltGameMsgRequest_FriendMemo
{
public:
	SI32 siFriendPersonID;

	cltGameMsgRequest_FriendMemo( SI32 FriendPersonID )
	{
		siFriendPersonID = FriendPersonID;
	}
};

class cltGameMsgResponse_FriendMemo
{
public:
	TCHAR	szFriendMemo[ MAX_FRIEND_NOTESIZE ];

	cltGameMsgResponse_FriendMemo( TCHAR* Memo )
	{
        StringCchCopy( szFriendMemo, MAX_FRIEND_NOTESIZE, Memo );	
	}
};

class cltGameMsgRequest_FriendMemoSet
{
public:
	SI32	siFriendPersonID;
	TCHAR	szFriendMemo[ MAX_FRIEND_NOTESIZE ];

	cltGameMsgRequest_FriendMemoSet( SI32 FriendPersonID, TCHAR* Memo )
	{
		siFriendPersonID = FriendPersonID;
		StringCchCopy( szFriendMemo, MAX_FRIEND_NOTESIZE, Memo );	
	}
};

class cltGameMsgResponse_FriendMemoSet
{
public:
	bool	bSuccessSave;

	cltGameMsgResponse_FriendMemoSet( bool success )
	{
		bSuccessSave = success;
	}
};

// 친구 그룹 변경
class cltGameMsgRequest_FriendChangeGroup
{
public:
	SI08	siGroupID;
	SI32	siFriendPersonID;

	cltGameMsgRequest_FriendChangeGroup(SI08 GroupID, SI32 FriendPersonID )
	{
		siGroupID = GroupID;
		siFriendPersonID = FriendPersonID;
	}
};

class cltGameMsgResponse_FriendChangeGroup
{
public:
	SI16	siFriendListIndex;
	SI08	siGroupID;
	SI32	siFriendPersonID;

	cltGameMsgResponse_FriendChangeGroup(SI16 FriendListIndex, SI08 GroupID, SI32 FriendPersonID)
	{
		siFriendListIndex = FriendListIndex;
		siGroupID = GroupID;
		siFriendPersonID = FriendPersonID;
	}
};

// 그룹명 변경
class cltGameMsgRequest_FriendChangeGroupName
{
public:
	TCHAR	szGroupName[MAX_FRIEND_GROUP][MAX_FRIEND_GROUPNAME];
	
	cltGameMsgRequest_FriendChangeGroupName()
	{
		for(int i=0; i<MAX_FRIEND_GROUP; i++)		szGroupName[i][0] = '\0';
	}
};

class cltGameMsgResponse_FriendChangeGroupName
{
public:	
	bool	bSuccessChange;

	TCHAR	szGroupName[MAX_FRIEND_GROUP][MAX_FRIEND_GROUPNAME];

	cltGameMsgResponse_FriendChangeGroupName( bool success )
	{
		bSuccessChange = success;
		for(int i=0; i<MAX_FRIEND_GROUP; i++)		szGroupName[i][0] = '\0';
	}
};

#endif
