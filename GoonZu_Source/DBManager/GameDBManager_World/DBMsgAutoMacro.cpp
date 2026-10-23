#include "DBGameProtocol.h"
#include "DBMsg-AutoMacro.h"

void DBGameProtocol::AutoMacro()
{
	// 받은 패킷
	sDBRequest_AutoMacro *pRecvMsg = (sDBRequest_AutoMacro *)m_pPacket;

	// 보낼 패킷
	sDBResponse_AutoMacro sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_AUTOMACRO;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->usCharID;

	// DB Query
	BEGIN_SP( "rsp_SystemAutoMacro" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->usCommandtype, sizeof(pRecvMsg->usCommandtype));
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID));
		m_odbc.SetParam( SQL_VARCHAR, pRecvMsg->szAutoProcessName, sizeof(pRecvMsg->szAutoProcessName));
		m_odbc.SetParam( SQL_VARCHAR, pRecvMsg->szIP, sizeof(pRecvMsg->szIP));
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.usCommandtype);
		m_odbc.GetData( &sendMsg.uiAutoMacroCnt);
		m_odbc.GetData( &sendMsg.siPersonID);
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );
}

