#include "DBGameProtocol.h"
#include "DBMsg-MofuMofuEvent.h"

void DBGameProtocol::MofuMofuEvent_Reserve()
{
	// 받은 패킷
	sDBRequest_MofuMofuEvent_Reserve *pRecvMsg = (sDBRequest_MofuMofuEvent_Reserve *)m_pPacket;

	// 보낼 패킷
	sDBResponse_MofuMofuEvent_Reserve sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_MOFUMOFUEVENT_RESERVE;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->usCharID;
	sendMsg.EventCost = pRecvMsg->EventCost;

	TIMESTAMP_STRUCT stReserveHour;
	Convert_TIMESTAMP_from_SYSTEMTIME( &stReserveHour, &pRecvMsg->stReserveHour);

	// DB Query
	BEGIN_SP( "rsp_MofuMofuEventAddReserve" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID));
		m_odbc.SetParam( SQL_TIMESTAMP, &stReserveHour, sizeof(stReserveHour));
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siVillageUnique, sizeof(pRecvMsg->siVillageUnique));
		m_odbc.SetParam( SQL_BIGINT, &pRecvMsg->EventCost, sizeof(pRecvMsg->EventCost));
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult );
		m_odbc.GetData( &stReserveHour ); Convert_SYSTEMTIME_from_TIMESTAMP( &sendMsg.stReserveHour, &stReserveHour);
		m_odbc.GetData( &sendMsg.siVillageUnique );
		m_odbc.GetData( &sendMsg.siCityHallMoney );
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );

	// 로그 작성
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "Hour = %d, VillageUnique = %d, EventCost = %I64d, CityHallMoney = %I64d"
			, pRecvMsg->stReserveHour.wHour, pRecvMsg->siVillageUnique, pRecvMsg->EventCost, sendMsg.siCityHallMoney
			);
		sDBLogMsg_QueryString LogMsg("MofuMofuEvent Add Reserve", pRecvMsg->siPersonID, NULL, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

void DBGameProtocol::MofuMofuEvent_ReserveList()
{
	// 받은 패킷
	sDBRequest_MofuMofuEvent_ReserveList *pRecvMsg = (sDBRequest_MofuMofuEvent_ReserveList *)m_pPacket;

	// 보낼 패킷
	sDBResponse_MofuMofuEvent_ReserveList sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_MOFUMOFUEVENT_RESERVELIST;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	//sendMsg.siCharID = pRecvMsg->siCharID;

	TIMESTAMP_STRUCT stCurrent;
	TIMESTAMP_STRUCT stReserveHour;

	Convert_TIMESTAMP_from_SYSTEMTIME( &stCurrent, &pRecvMsg->stCurrent);

	// DB Query
	BEGIN_SP( "rsp_MofuMofuEventReserveList" )
		m_odbc.SetParam( SQL_TIMESTAMP, &stCurrent, sizeof(stCurrent));
	END_SP()

	EXEC_SQL()

	SI32 siRoomNum = 0;
	BEGIN_FETCHDATA()
		if(siRoomNum >= MOFUMOFUEVENT_ROOMNUM) break;
		m_odbc.GetData( &stReserveHour ); Convert_SYSTEMTIME_from_TIMESTAMP( &sendMsg.ReserveRooms[siRoomNum].m_ReserveHour, &stReserveHour);
		m_odbc.GetData( &sendMsg.ReserveRooms[siRoomNum++].m_siVillageUnique );
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );
}
