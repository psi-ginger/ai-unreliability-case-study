#ifndef _DBMSGFRIEND_H
#define _DBMSGFRIEND_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DBMsg.h"

//-----------------------------------------------------
// 전체 친구 정보를 받아온다.
//-----------------------------------------------------
class sDBRequest_FriendAllList
{
public:
	sPacketHeader		packetHeader;

	SI32				siCharID;
	SI32				siPersonID;

	SI08				siLogin;		// 1: 로그인, 0: 기타

	sDBRequest_FriendAllList( SI32 CharID, SI32 PersonID, SI08 Login )
	{
		packetHeader.usSize = sizeof( sDBRequest_FriendAllList );
		packetHeader.usCmd	= DBMSG_REQUEST_FRIENDALLLIST;

		siCharID	= CharID;
		siPersonID	= PersonID;

		siLogin		= Login;
	}
};

class sDBResponse_FriendAllList
{
public:
	sPacketHeader		packetHeader;

	SI32				siCharID;
	SI08				siLogin;

	cltPFriendInfo		clPFriendInfo;

	sDBResponse_FriendAllList()
	{
		memset( this, 0, sizeof( sDBResponse_FriendAllList ) );
	}
};

//-----------------------------------------------------
// 친구를 추가한다.
//-----------------------------------------------------
class sDBRequest_FriendAdd
{
public:
	sPacketHeader		packetHeader;

	SI32				siFromPersonID;
	SI32				siCharID;

	TCHAR				strToPersonName[ MAX_PLAYER_NAME ];

	sDBRequest_FriendAdd()
	{
		ZeroMemory(this, sizeof(sDBRequest_FriendAdd));
	}

	sDBRequest_FriendAdd( SI32 FromPersonID, SI32 CharID, TCHAR *ToPersonName )
	{
		packetHeader.usSize = sizeof( sDBRequest_FriendAdd );
		packetHeader.usCmd	= DBMSG_REQUEST_FRIENDADD;

		siFromPersonID = FromPersonID;
		siCharID = CharID;

		MStrCpy(strToPersonName, ToPersonName, MAX_PLAYER_NAME );
	}
};

//-----------------------------------------------------
// 결과를 리턴함.
//-----------------------------------------------------
class sDBResponse_FriendAdd
{
public:
	sPacketHeader		packetHeader;
	SI32				siResult;			// 결과값.

	SI32				siFromPersonID;
	SI32				siCharID;

	SI32				siToPersonID;
	TCHAR				strToPersonName[ MAX_PLAYER_NAME ];

	sDBResponse_FriendAdd()
	{
		memset( this, 0, sizeof( sDBResponse_FriendAdd ) );
	}
};

//-----------------------------------------------------
// 친구를 삭제한다.
//-----------------------------------------------------
class sDBRequest_FriendDel
{
public:
	sPacketHeader		packetHeader;

	SI32				siFromPersonID;
	SI32				siCharID;

	SI32				siToPersonID;

	sDBRequest_FriendDel()
	{
		ZeroMemory(this, sizeof(sDBRequest_FriendDel));
	}

	sDBRequest_FriendDel( SI32 FromPersonID, SI32 CharID, SI32 ToPersonID )
	{
		packetHeader.usSize = sizeof( sDBRequest_FriendDel );
		packetHeader.usCmd	= DBMSG_REQUEST_FRIENDDEL;

		siFromPersonID = FromPersonID;
		siCharID = CharID;

		siToPersonID = ToPersonID;
	}
};

//-----------------------------------------------------
// 결과를 리턴함.
//-----------------------------------------------------
class sDBResponse_FriendDel
{
public:
	sPacketHeader		packetHeader;
	SI32				siResult;			// 결과값.

	SI32				siFromPersonID;
	SI32				siCharID;

	SI32				siToPersonID;

	sDBResponse_FriendDel()
	{
		memset( this, 0, sizeof( sDBResponse_FriendDel ) );
	}
};

class sDBRequest_FriendAddList
{
public:
	sPacketHeader		packetHeader;

	SI08				siFromGroupID;
	SI32				siFromPersonID;
	SI32				siFromCharID;
	TCHAR				strFromPersonName[ MAX_PLAYER_NAME - 1 ];

	SI08				siToGroupID;
	SI32				siToPersonID;
	SI32				siToCharID;
	TCHAR				strToPersonName[MAX_PLAYER_NAME-1];

	sDBRequest_FriendAddList( SI08 FromGroupID, SI32 FromPersonID, SI32 FromCharID, TCHAR *FromPersonName, SI08 ToGroupID, SI32 ToPersonID, SI32 ToCharID, TCHAR *ToPersonName )
	{
		packetHeader.usSize = sizeof( sDBRequest_FriendAddList );
		packetHeader.usCmd	= DBMSG_REQUEST_FRIENDADDLIST;

		siFromGroupID = FromGroupID;
		siFromPersonID = FromPersonID;
		siFromCharID = FromCharID;
		MStrCpy( strFromPersonName, FromPersonName, MAX_PLAYER_NAME );

		siToGroupID = ToGroupID;
		siToPersonID = ToPersonID;
		siToCharID = ToCharID;
		MStrCpy( strToPersonName, ToPersonName, MAX_PLAYER_NAME );
	}
};

class sDBResponse_FriendAddList
{
public:
	sPacketHeader		packetHeader;

	SI32				siResult;

	SI08				siFromGroupID;
	SI32				siFromPersonID;
	SI32				siFromCharID;
	
	TCHAR				strFromPersonName[ MAX_PLAYER_NAME ];

	SI08				siToGroupID;
	SI32				siToPersonID;
	SI32				siToCharID;

	TCHAR				strToPersonName[ MAX_PLAYER_NAME ];
	
	sDBResponse_FriendAddList()
	{
		memset( this, 0, sizeof( sDBResponse_FriendAddList ) );
	}
};

class sDBRequest_FriendDelList
{
public:
	sPacketHeader		packetHeader;

	SI32				siFromPersonID;
	SI32				siFromCharID;
	
	SI32				siToPersonID;

	
	sDBRequest_FriendDelList( SI32 FromPersonID, SI32 FromCharID, SI32 ToPersonID )
	{
		packetHeader.usSize = sizeof( sDBRequest_FriendDelList );
		packetHeader.usCmd	= DBMSG_REQUEST_FRIENDDELLIST;

		siFromPersonID = FromPersonID;
		siFromCharID = FromCharID;

		siToPersonID = ToPersonID;		
	}
};

class sDBResponse_FriendDelList
{
public:
	sPacketHeader		packetHeader;
	
	SI32				siResult;

	SI32				siFromPersonID;
	SI32				siFromCharID;

	SI32				siToPersonID;
	
	sDBResponse_FriendDelList()
	{
		memset( this, 0, sizeof( sDBResponse_FriendDelList ) );
	}
};

class sDBRequest_FriendMemo
{
public:
	sPacketHeader		packetHeader;

	SI32				siMyPersonID;
	SI32				siFriendPersonID;

	SI32				siCharID;

	sDBRequest_FriendMemo( SI32 myPersonID, SI32 friendPersonID, SI32 charID )
	{
		packetHeader.usSize = sizeof( sDBRequest_FriendMemo );
		packetHeader.usCmd	= DBMSG_REQUEST_FRIENDMEMO;

		siMyPersonID = myPersonID;
		siFriendPersonID = friendPersonID;
		siCharID = charID;
	}
};

class sDBResponse_FriendMemo
{
public:
	sPacketHeader		packetHeader;

	SI32				siResult;

	TCHAR				szFriendMemo[MAX_FRIEND_NOTESIZE];
	SI32				siCharID;

	sDBResponse_FriendMemo()
	{
		ZeroMemory( this, sizeof( sDBRequest_FriendMemo ) );
	}
};

class sDBRequest_FriendMemoSet
{
public:
	sPacketHeader		packetHeader;

	SI32				siMyPersonID;
	SI32				siFriendPersonID;

	TCHAR				szFriendMemo[MAX_FRIEND_NOTESIZE];

	SI32				siCharID;

	sDBRequest_FriendMemoSet( SI32 myPersonID, SI32 friendPersonID, TCHAR* memo, SI32 charID )
	{
		packetHeader.usSize = sizeof( sDBRequest_FriendMemoSet );
		packetHeader.usCmd	= DBMSG_REQUEST_FRIENDMEMOSET;

		siMyPersonID = myPersonID;
		siFriendPersonID = friendPersonID;

		StringCchCopy(szFriendMemo, MAX_FRIEND_NOTESIZE, memo );

		siCharID = charID;
	}
};

class sDBResponse_FriendMemoSet
{
public:
	sPacketHeader		packetHeader;

	SI32				siResult;

	SI32				siCharID;

	sDBResponse_FriendMemoSet()
	{
		ZeroMemory( this, sizeof(sDBResponse_FriendMemoSet) );
	}
};


//=============================================================================
//	CLASS   X
//! @brief  
//=============================================================================
class sDBRequest_FriendChangeGroup
{
public:
	sPacketHeader		packetHeader;

	SI32				siFromPersonID;
	SI32				siCharID;

    SI08				siGroupID;
	SI32				siToPersonID;

	sDBRequest_FriendChangeGroup( SI32 FromPersonID, SI32 CharID, SI08 GroupID, SI32 ToPersonID )
	{
		packetHeader.usSize = sizeof( sDBRequest_FriendChangeGroup );
		packetHeader.usCmd	= DBMSG_REQUEST_FRIENDCHANGEGROUP;

		siFromPersonID = FromPersonID;
		siCharID = CharID;

		siGroupID = GroupID;
		siToPersonID = ToPersonID;
	}
};

class sDBResponse_FriendChangeGroup
{
public:
	sPacketHeader		packetHeader;
	SI32				siResult;			// 결과값.

	SI32				siFromPersonID;
	SI32				siCharID;

	SI08				siGroupID;
	SI32				siToPersonID;

	sDBResponse_FriendChangeGroup()
	{
		memset( this, 0, sizeof( sDBResponse_FriendChangeGroup ) );
	}
};

//=============================================================================
//	CLASS   X
//! @brief  
//=============================================================================
class sDBRequest_FriendChangeGroupName
{
public:
	sPacketHeader		packetHeader;

	SI32				siMyPersonID;
	TCHAR				strGroupName[MAX_FRIEND_GROUP][MAX_FRIEND_GROUPNAME];

	SI32				siCharID;

	sDBRequest_FriendChangeGroupName(SI32 myPersonID, SI32 CharID)
	{
		packetHeader.usSize = sizeof( sDBRequest_FriendChangeGroupName );
		packetHeader.usCmd	= DBMSG_REQUEST_FRIENDCHANGEGROUPNAME;

		siMyPersonID = myPersonID;

		for(int i=0; i<MAX_FRIEND_GROUP; i++)		strGroupName[i][0] = '\0';

		siCharID = CharID;
	}
};

class sDBResponse_FriendChangeGroupName
{
public:
	sPacketHeader		packetHeader;

	SI32				siResult;
	TCHAR				strGroupName[MAX_FRIEND_GROUP][MAX_FRIEND_GROUPNAME];

	SI32				siCharID;

	sDBResponse_FriendChangeGroupName()
	{
		ZeroMemory( this, sizeof(sDBResponse_FriendChangeGroupName) );
	}
};

#endif