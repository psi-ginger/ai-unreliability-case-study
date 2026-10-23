#include "DBGameProtocol.h"
//#include "DBMsg.h"
#include "DBMsg-Friend.h"

void DBGameProtocol::FriendAllList()
{
	// 받은 패킷
	sDBRequest_FriendAllList *pRecvMsg = (sDBRequest_FriendAllList *)m_pPacket;

	// 보낼 패킷
	sDBResponse_FriendAllList sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_FRIENDALLLIST;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );

	sendMsg.siCharID	= pRecvMsg->siCharID;
	sendMsg.siLogin		= pRecvMsg->siLogin;

	BEGIN_SP( "rsp_FriendInfoGet" )
		m_odbc.SetParam( SQL_INTEGER, &(pRecvMsg->siPersonID), sizeof(pRecvMsg->siPersonID) );
	END_SP()
	
	EXEC_SQL()

	SI32 siGroupIndex = 0;
	BEGIN_FETCHDATA()
		m_odbc.GetData( &siGroupIndex );
		if(siGroupIndex >= MAX_FRIEND_GROUP )
			continue;
		m_odbc.GetData( sendMsg.clPFriendInfo.szGroupName[siGroupIndex], sizeof(sendMsg.clPFriendInfo.szGroupName[siGroupIndex]) );
	END_FETCHDATA()

	SI32 siFriendIndex = 0;
	BEGIN_NEXTRECORDSET()
		BEGIN_FETCHDATA()
			if(siFriendIndex >= MAX_FRIEND_NUMBER )
				break;
			m_odbc.GetData( &sendMsg.clPFriendInfo.siGroupID[siFriendIndex], sizeof(sendMsg.clPFriendInfo.siGroupID[siFriendIndex]) );
			GetSimplePerson( &sendMsg.clPFriendInfo.clFriend[siFriendIndex]);
			m_odbc.GetData( sendMsg.clPFriendInfo.szAccountID[siFriendIndex++], sizeof(sendMsg.clPFriendInfo.szAccountID[siFriendIndex++]) );
		END_FETCHDATA()
	END_NEXTRECORDSET()

	// send
	SendPacket( (BYTE *)&sendMsg );
}

void DBGameProtocol::FriendAddList()
{
	// 받은 패킷
	sDBRequest_FriendAddList *pRecvMsg = (sDBRequest_FriendAddList *)m_pPacket;
	
	// 보낼 패킷
	sDBResponse_FriendAddList sendMsg;
	
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_FRIENDADDLIST;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );

	sendMsg.siFromPersonID	= pRecvMsg->siFromPersonID;
	sendMsg.siToPersonID	= pRecvMsg->siToPersonID;
	sendMsg.siToCharID		= pRecvMsg->siToCharID;
	sendMsg.siFromCharID	= pRecvMsg->siFromCharID;

	SI32 siMax = MAX_FRIEND_NUMBER;
	// DB Query
	BEGIN_SP( "rsp_FriendAddList" )
		m_odbc.SetParam( SQL_TINYINT, &pRecvMsg->siFromGroupID,		sizeof(pRecvMsg->siFromGroupID) );			
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siFromPersonID,	sizeof(pRecvMsg->siFromPersonID) );
		m_odbc.SetParam( SQL_VARCHAR, pRecvMsg->strFromPersonName,  sizeof(pRecvMsg->strFromPersonName) );
		m_odbc.SetParam( SQL_TINYINT, &pRecvMsg->siToGroupID,		sizeof(pRecvMsg->siToGroupID) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siToPersonID,		sizeof(pRecvMsg->siToPersonID) );
		m_odbc.SetParam( SQL_VARCHAR, pRecvMsg->strToPersonName,	sizeof(pRecvMsg->strToPersonName) );
		m_odbc.SetParam( SQL_INTEGER, &siMax, sizeof(siMax) );
	END_SP()

	EXEC_SQL()
	

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0 );
		m_odbc.GetData( &sendMsg.siFromGroupID, 0 );
		m_odbc.GetData( &sendMsg.siFromPersonID, 0 );
		m_odbc.GetData( sendMsg.strFromPersonName, sizeof(sendMsg.strFromPersonName) );
		m_odbc.GetData( &sendMsg.siToGroupID, 0 );
		m_odbc.GetData( &sendMsg.siToPersonID, 0 );
		m_odbc.GetData( sendMsg.strToPersonName, sizeof(sendMsg.strToPersonName) );
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );
}

void DBGameProtocol::FriendDelList()
{
	// 받은 패킷
	sDBRequest_FriendDelList *pRecvMsg = (sDBRequest_FriendDelList *)m_pPacket;
	
	// 보낼 패킷
	sDBResponse_FriendDelList sendMsg;
	
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_FRIENDDELLIST;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.siFromCharID = pRecvMsg->siFromCharID;	

//	sendMsg.siCharID = pRecvMsg->siCharID;

	// DB Query
	BEGIN_SP( "rsp_FriendDelList" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siFromPersonID, sizeof(pRecvMsg->siFromPersonID) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siToPersonID, sizeof(pRecvMsg->siToPersonID) );
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0 );
		m_odbc.GetData( &sendMsg.siFromPersonID, 0 );
		m_odbc.GetData( &sendMsg.siToPersonID, 0 );
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );
}

void DBGameProtocol::FriendMemo()
{
	// 받은 패킷
	sDBRequest_FriendMemo *pRecvMsg = (sDBRequest_FriendMemo *)m_pPacket;

	// 보낼 패킷
	sDBResponse_FriendMemo sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_FRIENDMEMO;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.siCharID = pRecvMsg->siCharID;	

	// DB Query
	BEGIN_SP( "rsp_FriendMemo" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siMyPersonID, sizeof(pRecvMsg->siMyPersonID) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siFriendPersonID, sizeof(pRecvMsg->siFriendPersonID) );
	END_SP()

		EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0 );
		m_odbc.GetData( sendMsg.szFriendMemo, sizeof(sendMsg.szFriendMemo) );
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );
}


void DBGameProtocol::FriendMemoSet()
{
	// 받은 패킷
	sDBRequest_FriendMemoSet *pRecvMsg = (sDBRequest_FriendMemoSet *)m_pPacket;

	// 보낼 패킷
	sDBResponse_FriendMemoSet sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_FRIENDMEMOSET;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.siCharID = pRecvMsg->siCharID;	

	// DB Query
	BEGIN_SP( "rsp_FriendMemoSet" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siMyPersonID, sizeof(pRecvMsg->siMyPersonID) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siFriendPersonID, sizeof(pRecvMsg->siFriendPersonID) );
		m_odbc.SetParam( SQL_VARCHAR, pRecvMsg->szFriendMemo, sizeof(pRecvMsg->szFriendMemo) );
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0 );
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );
}


void DBGameProtocol::FriendChangeGroup()
{
	// 받은 패킷
	sDBRequest_FriendChangeGroup *pRecvMsg = (sDBRequest_FriendChangeGroup *)m_pPacket;

	// 보낼 패킷
	sDBResponse_FriendChangeGroup sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_FRIENDCHANGEGROUP;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.siFromPersonID = pRecvMsg->siFromPersonID;
	sendMsg.siCharID = pRecvMsg->siCharID;	

	// DB Query
	BEGIN_SP( "rsp_FriendChangeGroup" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siFromPersonID,	sizeof(pRecvMsg->siFromPersonID) );
		m_odbc.SetParam( SQL_TINYINT, &pRecvMsg->siGroupID,			sizeof(pRecvMsg->siGroupID) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siToPersonID,		sizeof(pRecvMsg->siToPersonID) );
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0 );
		m_odbc.GetData( &sendMsg.siGroupID, 0 );
		m_odbc.GetData( &sendMsg.siToPersonID, 0 );
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );
}

void DBGameProtocol::FriendChangeGroupName()
{
	// 받은 패킷
	sDBRequest_FriendChangeGroupName *pRecvMsg = (sDBRequest_FriendChangeGroupName *)m_pPacket;

	// 보낼 패킷
	sDBResponse_FriendChangeGroupName sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_FRIENDCHANGEGROUPNAME;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.siCharID = pRecvMsg->siCharID;	
	
	// 그룹명 복사
	for(int i=0; i<MAX_FRIEND_GROUP; i++)
	{
		MStrCpy(sendMsg.strGroupName[i], pRecvMsg->strGroupName[i], MAX_FRIEND_GROUPNAME);
	}

	// DB Query
	BEGIN_SP( "rsp_FriendChangeGroupName" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siMyPersonID, sizeof(pRecvMsg->siMyPersonID) );
		m_odbc.SetParam( SQL_VARCHAR, pRecvMsg->strGroupName[1], sizeof(pRecvMsg->strGroupName[1]) );
		m_odbc.SetParam( SQL_VARCHAR, pRecvMsg->strGroupName[2], sizeof(pRecvMsg->strGroupName[2]) );
		m_odbc.SetParam( SQL_VARCHAR, pRecvMsg->strGroupName[3], sizeof(pRecvMsg->strGroupName[3]) );
		m_odbc.SetParam( SQL_VARCHAR, pRecvMsg->strGroupName[4], sizeof(pRecvMsg->strGroupName[4]) );
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0 );
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );
}
