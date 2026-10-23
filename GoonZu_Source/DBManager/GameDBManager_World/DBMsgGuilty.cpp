#include "DBGameProtocol.h"
#include "DBMsg-System.h"
#include "DBMsg-Person.h"

void DBGameProtocol::PrisonUser()
{
	// 받은 패킷
	sDBRequest_PrisonUser *pRecvMsg = (sDBRequest_PrisonUser *)m_pPacket;
	
	// 보낼 패킷
	sDBResponse_PrisonUser sendMsg;
	
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_PRISONUSER;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->usCharID;

	// DB Query
	BEGIN_SP( "rsp_GuiltyPrisonUser" )
		m_odbc.SetParam( SQL_VARCHAR, pRecvMsg->szPersonName, sizeof(pRecvMsg->szPersonName) );
		SetGuiltyInfo( &pRecvMsg->pclGuiltyInfo );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siDate, sizeof(pRecvMsg->siDate) );
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0 );
		m_odbc.GetData( &sendMsg.siPersonID, 0 );
		m_odbc.GetData( sendMsg.szPersonName, sizeof(sendMsg.szPersonName) );
		GetGuiltyInfo( &sendMsg.clGuiltyInfo );
	END_FETCHDATA()


	// send
	SendPacket( (BYTE *)&sendMsg );

	// 로그 작성
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "TotalSecond = %d, LeftSecond = %d, Reason = %s"
				, sendMsg.clGuiltyInfo.siTotalPrisonSecond, sendMsg.clGuiltyInfo.siLeftPrisonSecond, sendMsg.clGuiltyInfo.szPrisonReason
				);
		sDBLogMsg_QueryString LogMsg("Guilty PrisonUser", sendMsg.siPersonID, pRecvMsg->siRequestID, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

void DBGameProtocol::GuiltyInfoRecord()
{
	// 받은 패킷
	sDBRequest_GuiltyInfoRecord *pRecvMsg = (sDBRequest_GuiltyInfoRecord *)m_pPacket;
	
	// 보낼 패킷
	sDBResponse_GuiltyInfoRecord sendMsg;
	
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_GUILTYINFORECORD;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.siCharID = pRecvMsg->siCharID;
	sendMsg.cIGuiltyInfo.Set(&pRecvMsg->clGuiltyInfo);

	// DB Query
	BEGIN_SP( "rsp_GuiltyInfoRecord" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID) );
		SetGuiltyInfo( &pRecvMsg->clGuiltyInfo);
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0 );
		m_odbc.GetData( &sendMsg.siPersonID, 0 );
	END_FETCHDATA()


	// send
	SendPacket( (BYTE *)&sendMsg );

	// 로그 작성
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "TotalSecond = %d, LeftSecond = %d, Reason = %s"
				, pRecvMsg->clGuiltyInfo.siTotalPrisonSecond, pRecvMsg->clGuiltyInfo.siLeftPrisonSecond, pRecvMsg->clGuiltyInfo.szPrisonReason
				);
		sDBLogMsg_QueryString LogMsg("Guilty Info Record", sendMsg.siPersonID, NULL, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

