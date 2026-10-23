#include "DBGameProtocol.h"
#include "DBMsg-Cityhall.h"


void DBGameProtocol::Candidate()
{
	// 받은 패킷
	sDBRequest_Candidate *pRecvMsg	= (sDBRequest_Candidate *)m_pPacket;
	
	// 보낼 패킷
	sDBResponse_Candidate sendMsg;
	
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_CANDIDATE;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->usCharID;
	
	// DB Query
	SI32 siRankType = pRecvMsg->clRank.GetRankType();
	SI32 siVillageUnique = pRecvMsg->clRank.GetVillageUnique();
	
	BEGIN_SP( "rsp_CandidateSet" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID) );
		SetSimpleRank( &pRecvMsg->clRank );
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult,	0);
		m_odbc.GetData( &sendMsg.siIndex,	0);
		GetSimplePerson( &sendMsg.clPerson );
		GetSimpleRank( &sendMsg.clRank );
	END_FETCHDATA()
	
	// send
	SendPacket( (BYTE *)&sendMsg );
}

void DBGameProtocol::BeRank()
{
	// 받은 패킷
	sDBRequest_BeRank *pRecvMsg	= (sDBRequest_BeRank *)m_pPacket;
	
	// 보낼 패킷
	sDBResponse_BeRank sendMsg;
	
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_BERANK;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->usCharID;
	
	// DB Query
	SI32 siType = pRecvMsg->clRank.GetRankType();
	SI32 siVillageUnique = pRecvMsg->clRank.GetVillageUnique();

	cltSimplePerson clBeforePerson;

	BEGIN_SP( "rsp_RankSet" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID,		sizeof(pRecvMsg->siPersonID) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siCandidateIndex,	sizeof(pRecvMsg->siCandidateIndex) );
		m_odbc.SetParam( SQL_INTEGER, &siType,						sizeof(siType) );
		m_odbc.SetParam( SQL_INTEGER, &siVillageUnique,				sizeof(siVillageUnique) );
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult,	0);
		m_odbc.GetData( &siType,			0);
		m_odbc.GetData( &siVillageUnique,	0);
		GetSimplePerson( &sendMsg.clRank.clPerson );
		m_odbc.GetData( &sendMsg.siCandidateIndex,	0);
		GetSimplePerson( &sendMsg.clCandidatePerson );
		GetSimplePerson( &clBeforePerson );
	END_FETCHDATA()

	sendMsg.clRank.clRank.SetRankType(siType);
	sendMsg.clRank.clRank.SetVillageUnique(siVillageUnique);

	// send
	SendPacket( (BYTE *)&sendMsg );

	// 기존에 같은 신분을 가진 Person이 있었다면 그 정보를 보낸다.
	if ( clBeforePerson.GetPersonID() > 0 )
	{
		SendRankInfo( &clBeforePerson );
	}

	// 로그
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "VillageUnique = %d, Type = %d"
				,	sendMsg.clRank.clRank.siVillageUnique, sendMsg.clRank.clRank.siType
				);
		sDBLogMsg_QueryString LogMsg("Set Rank", sendMsg.clRank.clPerson.siPersonID, NULL, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}


void DBGameProtocol::CancelCandidate()
{
	// 받은 패킷
	sDBRequest_CancelCandidate *pRecvMsg	= (sDBRequest_CancelCandidate *)m_pPacket;
	
	// 보낼 패킷
	sDBResponse_CancelCandidate sendMsg;
	
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_CANCELCANDIDATE;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->usCharID;
	
	// DB Query
	BEGIN_SP( "rsp_CandidateCancel" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID) );
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult,	0);
		m_odbc.GetData( &sendMsg.siIndex,	0);
		GetSimplePerson( &sendMsg.clPerson );
		GetSimpleRank( &sendMsg.clRank );
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );
}


void DBGameProtocol::ManyCandidateInfo()
{
	// 받은 패킷
	sDBRequest_ManyCandidateInfo *pRecvMsg	= (sDBRequest_ManyCandidateInfo *)m_pPacket;
	
	// 보낼 패킷
	sDBResponse_ManyCandidateInfo sendMsg;
	
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_MANYCANDIDATEINFO;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
//	sendMsg.usCharID = pRecvMsg->usCharID;
	
	// DB Query
	BEGIN_SP( "rsp_CandidateManyInfoGet" )
		SetSimpleRank( &pRecvMsg->clRank );
	END_SP()

	EXEC_SQL()

	SI32 siIndex;

	BEGIN_FETCHDATA()
		siIndex		= 0;
		m_odbc.GetData( &siIndex, 0 );
		GetSimplePerson( &sendMsg.clPerson[siIndex] );
	END_FETCHDATA()

	sendMsg.siResult				= 1;
	sendMsg.clRank.siType			= pRecvMsg->clRank.siType;
	sendMsg.clRank.siVillageUnique	= pRecvMsg->clRank.siVillageUnique;

	// send
	SendPacket( (BYTE *)&sendMsg );
}


void DBGameProtocol::DelCandidate()
{
	// 받은 패킷
	sDBRequest_DelCandidate *pRecvMsg	= (sDBRequest_DelCandidate *)m_pPacket;
	
	// 보낼 패킷
	sDBResponse_DelCandidate sendMsg;
	
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_DELCANDIDATE;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->usCharID;
	
	// DB Query
	BEGIN_SP( "rsp_CandidateDelete" )
		m_odbc.SetParam( SQL_TINYINT, &pRecvMsg->clRank.siType,				sizeof(pRecvMsg->clRank.siType) );
		m_odbc.SetParam( SQL_TINYINT, &pRecvMsg->clRank.siVillageUnique,	sizeof(pRecvMsg->clRank.siVillageUnique) );
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult,	0);
	END_FETCHDATA()

	sendMsg.clRank.siType			= pRecvMsg->clRank.siType;
	sendMsg.clRank.siVillageUnique	= pRecvMsg->clRank.siVillageUnique;

	// send
	SendPacket( (BYTE *)&sendMsg );
}


void DBGameProtocol::Resign()
{
	// 받은 패킷
	sDBRequest_Resign *pRecvMsg	= (sDBRequest_Resign *)m_pPacket;
	
	// 보낼 패킷
	sDBResponse_Resign sendMsg;
	
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_RESIGN;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->usCharID;
	
	// DB Query
	BEGIN_SP( "rsp_RankResign" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID,				sizeof(pRecvMsg->siPersonID) );
		m_odbc.SetParam( SQL_TINYINT, &pRecvMsg->clRank.siType,				sizeof(pRecvMsg->clRank.siType) );
		m_odbc.SetParam( SQL_TINYINT, &pRecvMsg->clRank.siVillageUnique,	sizeof(pRecvMsg->clRank.siVillageUnique) );
	END_SP()

	EXEC_SQL()

	SI32 siDate = 0;
	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult,	0);
		GetSimplePerson( &sendMsg.clPerson );
		m_odbc.GetData( &siDate, 0 ); sendMsg.clDate.MakeFullDateFromDateVary(siDate);
		m_odbc.GetData( &sendMsg.siVillageUnique, 0 );
	END_FETCHDATA()

	sendMsg.clRank.siType			= pRecvMsg->clRank.siType;
	sendMsg.clRank.siVillageUnique	= pRecvMsg->clRank.siVillageUnique;

	// send
	SendPacket( (BYTE *)&sendMsg );

	// 로그
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "VillageUnique = %d, Type = %d"
				,	pRecvMsg->clRank.siVillageUnique, pRecvMsg->clRank.siType
				);
		sDBLogMsg_QueryString LogMsg("Resign Rank", pRecvMsg->siPersonID, NULL, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}


void DBGameProtocol::ManyRank()
{
	// 받은 패킷
	sDBRequest_ManyRank *pRecvMsg	= (sDBRequest_ManyRank *)m_pPacket;
	
	// 보낼 패킷
	sDBResponse_ManyRank sendMsg;
	
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_MANYRANK;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
//	sendMsg.usCharID = pRecvMsg->usCharID;

	// DB Query
	SI32 siCount		= 0;

	BEGIN_SP( "rsp_RankManyGet" )
	END_SP()

	EXEC_SQL()
	
	BEGIN_FETCHDATA()
		sendMsg.clRank[siCount].Init();
		m_odbc.GetData( &sendMsg.clRank[siCount].clRank.siType, 0 );
		m_odbc.GetData( &sendMsg.clRank[siCount].clRank.siVillageUnique, 0 );
		GetSimplePerson( &sendMsg.clRank[siCount++].clPerson );
		// send
		if( siCount >= GET_MANY_RANK_NUM )
		{
			siCount = 0;
			sendMsg.siResult = 1;
			SendPacket( (BYTE *)&sendMsg );
		}
	END_FETCHDATA()

	if(siCount != 0)
	{
		sendMsg.siResult = 1;
		SendPacket( (BYTE *)&sendMsg );
	}
}
