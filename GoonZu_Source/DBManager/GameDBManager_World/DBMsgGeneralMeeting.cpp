#include "DBGameProtocol.h"
#include "DBMsg-Cityhall.h"

void DBGameProtocol::GeneralMeetingSuggestChief()
{
	// 받은 패킷
	sDBRequest_GeneralMeeting_SuggestChief *pRecvMsg = (sDBRequest_GeneralMeeting_SuggestChief *)m_pPacket;

	// 보낼 패킷
	sDBResponse_GeneralMeeting_SuggestChief sendMsg;


	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_GENERALMEETING_SUGGEST_CHIEF;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->usCharID;

	SI32 siStartDate = pRecvMsg->siStartDate;
	SI32 siCloseDate = pRecvMsg->clCloseDate.GetDateVary();
	SI32 siBeforeStartDate = 0;
	cltDate clBeforeStartDate;
	SI32 siBurnAmount = 0;

	// DB Query
	BEGIN_SP( "rsp_GeneralMeetingSuggestChief" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siVillageUnique, sizeof(pRecvMsg->siVillageUnique) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID) );
		m_odbc.SetParam( SQL_VARCHAR, pRecvMsg->szCandidateName, sizeof(pRecvMsg->szCandidateName) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siStartDate, sizeof(pRecvMsg->siStartDate) );
		m_odbc.SetParam( SQL_INTEGER, &siCloseDate, sizeof(siCloseDate) );
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0 );
		m_odbc.GetData( &sendMsg.siVillageUnique, 0 );
		m_odbc.GetData( &sendMsg.siTotalStock, 0 );
		m_odbc.GetData( &sendMsg.siPersonID, 0 );
		m_odbc.GetData( &sendMsg.siStockAmount, 0 );
		m_odbc.GetData( &siStartDate, 0 ); sendMsg.clStartDate.MakeFullDateFromDateVary(siStartDate);
		m_odbc.GetData( &siCloseDate, 0 ); sendMsg.clCloseDate.MakeFullDateFromDateVary(siCloseDate);
		m_odbc.GetData( &siBeforeStartDate, 0 ); clBeforeStartDate.MakeFullDateFromDateVary(siBeforeStartDate);
		m_odbc.GetData( &sendMsg.siCandidatePersonID, 0 );
		m_odbc.GetData( sendMsg.szCandidateName, sizeof(sendMsg.szCandidateName) );
		m_odbc.GetData( &siBurnAmount, 0 );
	END_FETCHDATA()

	BEGIN_NEXTRECORDSET()
		SI32 siIndex = 0;
		BEGIN_FETCHDATA()
			m_odbc.GetData( &siIndex, 0 );
			if(siIndex >= 100) continue;
			GetGeneralMeetingVoter( &sendMsg.clVoter[siIndex] );
		END_FETCHDATA()
	END_NEXTRECORDSET()

	// send
	SendPacket( (BYTE *)&sendMsg );

	// 로그 작성
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "VillageUnique = %d, StockAmount = %d, BurnAmount = %d, StartDate = %d/%d/%d, CloseDate = %d/%d/%d, BeforeStartDate = %d/%d/%d"
			, sendMsg.siVillageUnique, sendMsg.siStockAmount, siBurnAmount, sendMsg.clStartDate.uiYear, sendMsg.clStartDate.uiMonth, sendMsg.clStartDate.uiDay, sendMsg.clCloseDate.uiYear, sendMsg.clCloseDate.uiMonth, sendMsg.clCloseDate.uiDay, clBeforeStartDate.uiYear, clBeforeStartDate.uiMonth, clBeforeStartDate.uiDay
			);
		sDBLogMsg_QueryString LogMsg("GeneralMeeting Suggest Chief", sendMsg.siPersonID, sendMsg.siCandidatePersonID, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

void DBGameProtocol::GeneralMeetingInfo()
{


	// 받은 패킷
	sDBRequest_GeneralMeetingInfo *pRecvMsg = (sDBRequest_GeneralMeetingInfo *)m_pPacket;

	// 보낼 패킷
	sDBResponse_GeneralMeetingInfo sendMsg;


	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_GENERALMEETING_INFO;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->usCharID;

	SI32 siCLoseDate = 0;
	SI32 siStartDate = 0;

	// DB Query
	BEGIN_SP( "rsp_GeneralMeetingInfoGet" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siVillageUnique, sizeof(pRecvMsg->siVillageUnique) );
	END_SP()

		EXEC_SQL()

		BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0 );
	m_odbc.GetData( &sendMsg.siVillageUnique, 0 );
	m_odbc.GetData( &sendMsg.siMatterType, 0 );
	m_odbc.GetData( &sendMsg.siPersonID, 0 );
	m_odbc.GetData( &siStartDate, 0 ); sendMsg.clStartDate.MakeFullDateFromDateVary(siStartDate);
	m_odbc.GetData( &siCLoseDate, 0 ); sendMsg.clCloseDate.MakeFullDateFromDateVary(siCLoseDate);
	m_odbc.GetData( &sendMsg.siTotalStock, 0 );
	m_odbc.GetData( &sendMsg.siCandidatePersonID, 0 );
	m_odbc.GetData( sendMsg.szCandidateName, sizeof(sendMsg.szCandidateName) );
	m_odbc.GetData( &sendMsg.siAcceptCandidate, 0 );
	END_FETCHDATA()

		BEGIN_NEXTRECORDSET()
		SI32 siIndex = 0;
	BEGIN_FETCHDATA()
		m_odbc.GetData( &siIndex, 0 );
	if(siIndex >= 100)
		continue;
	GetGeneralMeetingVoter( &sendMsg.voter[siIndex] );
	END_FETCHDATA()
		END_NEXTRECORDSET()
		// send
		SendPacket( (BYTE *)&sendMsg );
}


void DBGameProtocol::GeneralMeetingVote()
{
	// 받은 패킷
	sDBRequest_GeneralMeeting_Vote *pRecvMsg = (sDBRequest_GeneralMeeting_Vote *)m_pPacket;
	
	// 보낼 패킷
	sDBResponse_GeneralMeeting_Vote sendMsg;


	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_GENERALMEETING_VOTE;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->usCharID;

	SI32 siRank = 0;
	SI32 siAmount = 0;

	// DB Query
	BEGIN_SP( "rsp_GeneralMeetingVote" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siVillageUnique, sizeof(pRecvMsg->siVillageUnique) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siVoterPersonID, sizeof(pRecvMsg->siVoterPersonID) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siVoteType, sizeof(pRecvMsg->siVoteType) );
	END_SP()
	
	EXEC_SQL()
	
	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0 );
		m_odbc.GetData( &sendMsg.siVillageUnique, 0 );
		m_odbc.GetData( &sendMsg.siVoterPersonID, 0 );
		m_odbc.GetData( sendMsg.szVoterName, sizeof(sendMsg.szVoterName) );
		m_odbc.GetData( &sendMsg.siVoteType, 0 );
		m_odbc.GetData( &siRank, 0 );
		m_odbc.GetData( &siAmount, 0 );
	END_FETCHDATA()
	
	// send
	SendPacket( (BYTE *)&sendMsg );

	// 로그 작성
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "VillageUnique = %d, VoteType = %d, Amount = %d, Rank = %d"
			, sendMsg.siVillageUnique, sendMsg.siVoteType, siAmount, siRank
			);
		sDBLogMsg_QueryString LogMsg("GeneralMeeting Vote", sendMsg.siVoterPersonID, NULL, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

void DBGameProtocol::GeneralMeetingAcceptCandidate()
{
	// 받은 패킷
	sDBRequest_GeneralMeeting_AcceptCandidate *pRecvMsg = (sDBRequest_GeneralMeeting_AcceptCandidate *)m_pPacket;
	
	// 보낼 패킷
	sDBResponse_GeneralMeeting_AcceptCandidate sendMsg;


	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_GENERALMEETING_ACCEPTCANDIDATE;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->usCharID;

	SI32 siCloseDate = pRecvMsg->clCloseDate.GetDateVary();
	sendMsg.clCloseDate.Set(&pRecvMsg->clCloseDate);

	// DB Query
	BEGIN_SP( "rsp_GeneralMeetingAcceptCandidate" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siVillageUnique, sizeof(pRecvMsg->siVillageUnique) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID) );
		m_odbc.SetParam( SQL_INTEGER, &siCloseDate, sizeof(siCloseDate) );
	END_SP()
	
	EXEC_SQL()
	
	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0 );
		m_odbc.GetData( &sendMsg.siVillageUnique, 0 );
		m_odbc.GetData( &sendMsg.siPersonID, 0 );
	END_FETCHDATA()


	// send
	SendPacket( (BYTE *)&sendMsg );

	// 로그 작성
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "VillageUnique = %d"
			, sendMsg.siVillageUnique
			);
		sDBLogMsg_QueryString LogMsg("GeneralMeeting Accept Candidate", sendMsg.siPersonID, NULL, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

void DBGameProtocol::GeneralMeetingCloseChief()
{
	// 받은 패킷
	sDBRequest_GeneralMeeting_CloseChief *pRecvMsg = (sDBRequest_GeneralMeeting_CloseChief *)m_pPacket;
	
	// 보낼 패킷
	sDBResponse_GeneralMeeting_CloseChief sendMsg;


	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_GENERALMEETING_CLOSE_CHIEF;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->usCharID;

	sendMsg.clDate.Set( &pRecvMsg->clDate );

	// DB Query
	BEGIN_SP( "rsp_GeneralMeetingCloseChief" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siVillageUnique, sizeof(pRecvMsg->siVillageUnique) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siVoteResult, sizeof(pRecvMsg->siVoteResult) );
	END_SP()
	
	EXEC_SQL()
	
	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0 );
		m_odbc.GetData( &sendMsg.siVillageUnique, 0 );
		m_odbc.GetData( &sendMsg.siVoteResult, 0 );
		m_odbc.GetData( &sendMsg.siChiefPersonID, 0 );
		m_odbc.GetData( sendMsg.szChiefName, sizeof(sendMsg.szChiefName) );
	END_FETCHDATA()
	
	// send
	SendPacket( (BYTE *)&sendMsg );

	// 로그 작성
	if(sendMsg.siResult == 1 || sendMsg.siResult == 2)
	{
		char log[512] = "";
		sprintf(log, "VillageUnique = %d, VoteResult = %d, Result = %d"
			, sendMsg.siVillageUnique, sendMsg.siVoteResult, sendMsg.siResult
			);
		sDBLogMsg_QueryString LogMsg("GeneralMeeting Close Chief", sendMsg.siChiefPersonID, NULL, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

void DBGameProtocol::GeneralMeetingSetVoterState()
{
	// 받은 패킷
	sDBRequest_GeneralMeeting_SetVoterState *pRecvMsg = (sDBRequest_GeneralMeeting_SetVoterState *)m_pPacket;

	// 보낼 패킷
	sDBResponse_GeneralMeeting_SetVoterState sendMsg;


	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_GENERALMEETING_SETVOTERSTATE;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->usCharID;

	// DB Query
	BEGIN_SP( "rsp_GeneralMeetingVoterStateSet" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siVillageUnique, sizeof(pRecvMsg->siVillageUnique) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siParticipate, sizeof(pRecvMsg->siParticipate) );
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0 );
		m_odbc.GetData( &sendMsg.siVillageUnique, 0 );
		m_odbc.GetData( &sendMsg.siPersonID, 0 );
		m_odbc.GetData( &sendMsg.siParticipate, 0 );
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );

	// 로그 작성
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "VillageUnique = %d, Participate = %d"
			, sendMsg.siVillageUnique, sendMsg.siParticipate
			);
		sDBLogMsg_QueryString LogMsg("GeneralMeeting VoterState Set", sendMsg.siPersonID, NULL, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

void DBGameProtocol::GeneralMeetingSuggestOnOff()
{
	// 받은 패킷
	sDBRequest_GeneralMeeting_Suggest_On_Off *pRecvMsg = (sDBRequest_GeneralMeeting_Suggest_On_Off *)m_pPacket;

	// 보낼 패킷
	sDBResponse_GeneralMeeting_Suggest_On_Off sendMsg;


	sendMsg.m_packetHeader.usCmd = DBMSG_RESPONSE_GENERALMEETING_SUGGEST_ON_OFF;
	sendMsg.m_packetHeader.usSize = sizeof( sendMsg );
	//sendMsg.usCharID = pRecvMsg->usCharID;

	// DB Query
	BEGIN_SP( "rsp_GeneralMeetingSuggestOnOff" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->m_siVillageUnique, sizeof(pRecvMsg->m_siVillageUnique) );
		m_odbc.SetParam( SQL_BIT, &pRecvMsg->m_bGeneralMeeting_Suggest_On_Off, sizeof(pRecvMsg->m_bGeneralMeeting_Suggest_On_Off) );
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.m_siResult, 0 );
		m_odbc.GetData( &sendMsg.m_siVillageUnique, 0 );
		m_odbc.GetData( &sendMsg.m_bGeneralMeeting_Suggest_On_Off, 0 );
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );
}
