#include "DBGameProtocol.h"
#include "DBMsg-Wedding.h"

void DBGameProtocol::WeddingReserveListGet()
{
	// 받은 패킷
	sDBRequest_WeddingReserveListGet	*pRecvMsg = (sDBRequest_WeddingReserveListGet*)m_pPacket;

	// 보낼 패킷
	sDBResponse_WeddingReserveListGet	sendMsg;

	sendMsg.m_packetHeader.usCmd	= DBMSG_RESPONSE_WEDDING_RESERVE_LIST_GET;
	sendMsg.m_packetHeader.usSize	= sizeof( sendMsg );

	TIMESTAMP_STRUCT stCurrentTime;
	Convert_TIMESTAMP_from_SYSTEMTIME( &stCurrentTime, &pRecvMsg->m_stCurrentTime );

	// DB Query
	BEGIN_SP( "rsp_WeddingListGet" )
		m_odbc.SetParam( SQL_TIMESTAMP, &stCurrentTime, sizeof(stCurrentTime) );
	END_SP()

	EXEC_SQL()

	TIMESTAMP_STRUCT	stReserveTime;
	SI32				siReserveCount = 0;

	BEGIN_FETCHDATA()
		// 24개만 받으면 된다
		if ( WEDDING_RESERVE_LIST_NUM <= siReserveCount) break;
		m_odbc.GetData( &sendMsg.m_clReserveList[siReserveCount].m_siMate1_PersonID );
		m_odbc.GetData( sendMsg.m_clReserveList[siReserveCount].m_szMate1_Name, sizeof(sendMsg.m_clReserveList[siReserveCount].m_szMate1_Name) );
		m_odbc.GetData( &sendMsg.m_clReserveList[siReserveCount].m_siMate2_PersonID );
		m_odbc.GetData( sendMsg.m_clReserveList[siReserveCount].m_szMate2_Name, sizeof(sendMsg.m_clReserveList[siReserveCount].m_szMate2_Name) );
		m_odbc.GetData( &sendMsg.m_clReserveList[siReserveCount].m_siReserveNum );
		m_odbc.GetData( &stReserveTime );
		// 시간 데이터는 변환시키고
		Convert_SYSTEMTIME_from_TIMESTAMP( &sendMsg.m_clReserveList[siReserveCount].m_stReserveTime, &stReserveTime);
		// 카운트 증가
		siReserveCount++;
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );

}

void DBGameProtocol::WeddingReserve()
{
	// 받은 패킷
	sDBRequest_WeddingReserve	*pRecvMsg = (sDBRequest_WeddingReserve*)m_pPacket;

	// 보낼 패킷
	sDBResponse_WeddingReserve	sendMsg;

	sendMsg.m_packetHeader.usCmd	= DBMSG_RESPONSE_WEDDING_RESERVE;
	sendMsg.m_packetHeader.usSize	= sizeof( sendMsg );

	TIMESTAMP_STRUCT stReserveTime;
	Convert_TIMESTAMP_from_SYSTEMTIME( &stReserveTime, &pRecvMsg->m_stReserveTime );

	// DB Query
	BEGIN_SP( "rsp_WeddingReserve" )
		m_odbc.SetParam( SQL_INTEGER,	&pRecvMsg->m_siPersonID,	sizeof(pRecvMsg->m_siPersonID) );
		m_odbc.SetParam( SQL_TIMESTAMP,	&stReserveTime,				sizeof(stReserveTime) );
		m_odbc.SetParam( SQL_BIGINT,	&pRecvMsg->m_gmCost,		sizeof(pRecvMsg->m_gmCost) );
	END_SP()

	EXEC_SQL()

	TIMESTAMP_STRUCT	stSendReserveTime;

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.m_siResult );
		m_odbc.GetData( &sendMsg.m_siPersonID );
		m_odbc.GetData( sendMsg.m_szPersonName, sizeof(sendMsg.m_szPersonName) );
		m_odbc.GetData( &sendMsg.m_siMatePersonID );
		m_odbc.GetData( sendMsg.m_szMateName, sizeof(sendMsg.m_szMateName) );
		m_odbc.GetData( &sendMsg.m_siReserveNum );
		m_odbc.GetData( &stSendReserveTime ); Convert_SYSTEMTIME_from_TIMESTAMP( &sendMsg.m_stReserveTime, &stSendReserveTime );
		m_odbc.GetData( &sendMsg.m_gmLeftBankMoney );
	END_FETCHDATA()

		sendMsg.m_gmReserveMoney = pRecvMsg->m_gmCost	;

	// send
	SendPacket( (BYTE *)&sendMsg );

}

void DBGameProtocol::WeddingReserveCancel()
{
	// 받은 패킷
	sDBRequest_WeddingReserveCancel		*pRecvMsg = (sDBRequest_WeddingReserveCancel*)m_pPacket;
	
	// 보낼 패킷	
	sDBResponse_WeddingReserveCancel	sendMsg;

	sendMsg.m_packetHeader.usCmd	= DBMSG_RESPONSE_WEDDING_RESERVE_CANCEL;
	sendMsg.m_packetHeader.usSize	= sizeof( sendMsg );

	// DB Query
	BEGIN_SP( "rsp_WeddingReserveCancel" )
		m_odbc.SetParam( SQL_INTEGER,	&pRecvMsg->m_siPersonID,	sizeof(pRecvMsg->m_siPersonID) );
		m_odbc.SetParam( SQL_INTEGER,	&pRecvMsg->m_siReserveNum,	sizeof(pRecvMsg->m_siReserveNum) );
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.m_siResult );
		m_odbc.GetData( &sendMsg.m_siPersonID );
		m_odbc.GetData( &sendMsg.m_siReserveNum );
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );
}


