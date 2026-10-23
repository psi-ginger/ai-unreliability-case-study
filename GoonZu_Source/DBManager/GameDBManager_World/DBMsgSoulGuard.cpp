#include "DBGameProtocol.h"
#include "DBMsg-SoulGuard.h"


void DBGameProtocol::SoulGuardCreate()
{	
	// 받은 패킷
	sDBRequest_SoulGuardCreate* pRecvMsg = (sDBRequest_SoulGuardCreate*)m_pPacket;

	// 보낼 패킷
	sDBResponse_SoulGuardCreate sendMsg;
	sendMsg.m_packetHeader.usCmd	= DBMSG_RESPONSE_SOULGUARD_CREATE;
	sendMsg.m_packetHeader.usSize	= sizeof(sendMsg);


	// DB Query
	BEGIN_SP( "rsp_SoulGuardCreate" )
		m_odbc.SetParam( SQL_INTEGER,	&pRecvMsg->m_siPersonID,		sizeof(pRecvMsg->m_siPersonID) );
		m_odbc.SetParam( SQL_VARCHAR,	pRecvMsg->m_szName,				sizeof(pRecvMsg->m_szName) );
		m_odbc.SetParam( SQL_SMALLINT,	&pRecvMsg->m_siStatus,			sizeof(pRecvMsg->m_siStatus) );
		m_odbc.SetParam( SQL_SMALLINT,	&pRecvMsg->m_siKind,			sizeof(pRecvMsg->m_siKind) );
		m_odbc.SetParam( SQL_INTEGER,	&pRecvMsg->m_siLimitDateVary,	sizeof(pRecvMsg->m_siLimitDateVary) );
		m_odbc.SetParam( SQL_SMALLINT,	&pRecvMsg->m_siFellowShip,		sizeof(pRecvMsg->m_siFellowShip) );
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.m_siResult );
		m_odbc.GetData( &sendMsg.m_siPersonID );
		m_odbc.GetData( &sendMsg.m_siSoulGuardID );
		m_odbc.GetData( &sendMsg.m_siPosType );
		m_odbc.GetData( sendMsg.m_szName, sizeof(sendMsg.m_szName) );
		m_odbc.GetData( &sendMsg.m_siStatus );
		m_odbc.GetData( &sendMsg.m_siKind );
		m_odbc.GetData( &sendMsg.m_siLimitDateVary );
		m_odbc.GetData( &sendMsg.m_siFellowShip );
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );

}

void DBGameProtocol::SoulGuardGet()
{
	// 받은 패킷
	sDBRequest_SoulGuardGet* pRecvMsg = (sDBRequest_SoulGuardGet*)m_pPacket;

	// 보낼 패킷
	sDBResponse_SoulGuardGet sendMsg;
	sendMsg.m_packetHeader.usCmd	= DBMSG_RESPONSE_SOULGUARD_GET;
	sendMsg.m_packetHeader.usSize	= sizeof(sendMsg);
	sendMsg.m_siPersonID			= pRecvMsg->m_siPersonID;

	// DB Query
	BEGIN_SP( "rsp_SoulGuardGet" )
		m_odbc.SetParam( SQL_INTEGER,	&pRecvMsg->m_siPersonID,		sizeof(pRecvMsg->m_siPersonID) );
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.m_siResult );
		m_odbc.GetData( &sendMsg.m_siPersonID );
		m_odbc.GetData( &sendMsg.m_siSoulGuardID );
		m_odbc.GetData( sendMsg.m_szName, sizeof(sendMsg.m_szName) );
		m_odbc.GetData( &sendMsg.m_siStatus );
		m_odbc.GetData( &sendMsg.m_siKind );
		m_odbc.GetData( &sendMsg.m_siLimitDateVary );
		m_odbc.GetData( &sendMsg.m_siFellowShip );
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );

}

void DBGameProtocol::SoulGuardFellowShipSet()
{
	// 받은 패킷
	sDBRequest_SoulGuard_FellowShipSet* pRecvMsg = (sDBRequest_SoulGuard_FellowShipSet*)m_pPacket;

	// 보낼 패킷
	sDBResponse_SoulGuard_FellowShipSet sendMsg;
	sendMsg.m_packetHeader.usCmd	= DBMSG_RESPONSE_SOULGUARD_FELLOWSHIP_SET;
	sendMsg.m_packetHeader.usSize	= sizeof(sendMsg);


	// DB Query
	BEGIN_SP( "rsp_SoulGuardFellowShipSet" )
		m_odbc.SetParam( SQL_INTEGER,	&pRecvMsg->m_siPersonID,		sizeof(pRecvMsg->m_siPersonID) );
		m_odbc.SetParam( SQL_INTEGER,	&pRecvMsg->m_siSoulGuardID,		sizeof(pRecvMsg->m_siSoulGuardID) );
		m_odbc.SetParam( SQL_SMALLINT,	&pRecvMsg->m_siFellowShip,		sizeof(pRecvMsg->m_siFellowShip) );
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.m_siResult );
		m_odbc.GetData( &sendMsg.m_siPersonID );
		m_odbc.GetData( &sendMsg.m_siSoulGuardID );
		m_odbc.GetData( &sendMsg.m_siFellowShip );
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );
}

void DBGameProtocol::SoulGuardChangeName()
{
	// 받은 패킷
	sDBRequest_SoulGuard_Change_Name* pRecvMsg = (sDBRequest_SoulGuard_Change_Name*)m_pPacket;

	// 보낼 패킷
	sDBResponse_SoulGuard_Change_Name sendMsg;
	sendMsg.m_packetHeader.usCmd	= DBMSG_RESPONSE_SOULGUARD_CHANGE_NAME;
	sendMsg.m_packetHeader.usSize	= sizeof(sendMsg);


	// DB Query
	BEGIN_SP( "rsp_SoulGuardChangeName" )
		m_odbc.SetParam( SQL_INTEGER,	&pRecvMsg->m_siPersonID,		sizeof(pRecvMsg->m_siPersonID) );
		m_odbc.SetParam( SQL_INTEGER,	&pRecvMsg->m_siSoulGuardID,		sizeof(pRecvMsg->m_siSoulGuardID) );
		m_odbc.SetParam( SQL_VARCHAR,	pRecvMsg->m_szChangeName,		sizeof(pRecvMsg->m_szChangeName) );
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.m_siResult );
		m_odbc.GetData( &sendMsg.m_siPersonID );
		m_odbc.GetData( &sendMsg.m_siSoulGuardID );
		m_odbc.GetData( sendMsg.m_szChangeName, sizeof(sendMsg.m_szChangeName) );
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );

}

void DBGameProtocol::SoulGuardDead()
{
	// 받은 패킷
	sDBRequest_SoulGuard_Dead* pRecvMsg = (sDBRequest_SoulGuard_Dead*)m_pPacket;

	// 보낼 패킷
	sDBResponse_SoulGuard_Dead sendMsg;
	sendMsg.m_packetHeader.usCmd	= DBMSG_RESPONSE_SOULGUARD_DEAD;
	sendMsg.m_packetHeader.usSize	= sizeof(sendMsg);
	sendMsg.m_siDeadMode			= pRecvMsg->m_siDeadMode;


	// DB Query
	BEGIN_SP( "rsp_SoulGuardDead" )
		m_odbc.SetParam( SQL_INTEGER,	&pRecvMsg->m_siPersonID,		sizeof(pRecvMsg->m_siPersonID) );
		m_odbc.SetParam( SQL_INTEGER,	&pRecvMsg->m_siSoulGuardID,		sizeof(pRecvMsg->m_siSoulGuardID) );
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.m_siResult );
		m_odbc.GetData( &sendMsg.m_siPersonID );
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );
}

void DBGameProtocol::SoulGuardHouse_Get()
{
	// 받은 패킷
	sDBRequest_SoulGuardHouse_Get* pRecvMsg = (sDBRequest_SoulGuardHouse_Get*)m_pPacket;

	// 보낼 패킷
	sDBResponse_SoulGuardHouse_Get sendMsg;
	sendMsg.m_packetHeader.usCmd	= DBMSG_RESPONSE_SOULGUARD_HOUSE_GET;
	sendMsg.m_packetHeader.usSize	= sizeof(sendMsg);
	sendMsg.m_siPersonID			= pRecvMsg->m_siPersonID;

	// DB Query
	BEGIN_SP( "rsp_SoulGuard_HouseGet" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->m_siPersonID, sizeof(pRecvMsg->m_siPersonID) );
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.m_siResult );
		m_odbc.GetData( &sendMsg.m_siPersonID );
		m_odbc.GetData( &sendMsg.m_siHouseNum );
	END_FETCHDATA()

	SI32 siIndex=0;

	BEGIN_NEXTRECORDSET()
		BEGIN_FETCHDATA()
			if ( MAX_SOULGUARD_HOUSE_NUM <= siIndex ) break;

			m_odbc.GetData( &sendMsg.m_siSoulGuardID[siIndex] );
			m_odbc.GetData( &sendMsg.m_sisoulGuardKind[siIndex] );
			siIndex++;
		END_FETCHDATA()
	END_NEXTRECORDSET()
	
	// send
	SendPacket( (BYTE *)&sendMsg );
}

void DBGameProtocol::SoulGuardHouse_Change()
{
	// 받은 패킷
	sDBRequest_SoulGuardHouse_Change* pRecvMsg = (sDBRequest_SoulGuardHouse_Change*)m_pPacket;

	// 보낼 패킷
	sDBResponse_SoulGuardHouse_Change sendMsg;
	sendMsg.m_packetHeader.usCmd	= DBMSG_RESPONSE_SOULGUARD_HOUSE_CHANGE;
	sendMsg.m_packetHeader.usSize	= sizeof(sendMsg);
	

	// DB Query
	BEGIN_SP( "rsp_SoulGuard_HouseChange" )
		m_odbc.SetParam( SQL_INTEGER,	&pRecvMsg->m_siPersonID,			sizeof(pRecvMsg->m_siPersonID) );
		m_odbc.SetParam( SQL_INTEGER,	&pRecvMsg->m_siCharSoulGuardID,		sizeof(pRecvMsg->m_siCharSoulGuardID) );
		m_odbc.SetParam( SQL_INTEGER,	&pRecvMsg->m_siHouseSoulGuardID,	sizeof(pRecvMsg->m_siHouseSoulGuardID) );
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.m_siResult );
		m_odbc.GetData( &sendMsg.m_siPersonID );
		m_odbc.GetData( &sendMsg.m_siSoulGuardID );
		m_odbc.GetData( sendMsg.m_szName, sizeof(sendMsg.m_szName) );
		m_odbc.GetData( &sendMsg.m_siStatus );
		m_odbc.GetData( &sendMsg.m_siKind );
		m_odbc.GetData( &sendMsg.m_siLimitDateVary );
		m_odbc.GetData( &sendMsg.m_siFellowShip );
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );
}

void DBGameProtocol::SoulGuardHouse_Create()
{
	// 받은 패킷
	sDBRequest_SoulGuardHouse_Create* pRecvMsg = (sDBRequest_SoulGuardHouse_Create*)m_pPacket;

	// 보낼 패킷
	sDBResponse_SoulGuardHouse_Create sendMsg;
	sendMsg.m_packetHeader.usCmd	= DBMSG_RESPONSE_SOULGUARD_HOUSE_CREATE;
	sendMsg.m_packetHeader.usSize	= sizeof(sendMsg);

	// DB Query
	BEGIN_SP( "rsp_SoulGuard_HouseCreate" )
		m_odbc.SetParam( SQL_INTEGER,	&pRecvMsg->m_siPersonID,	sizeof(pRecvMsg->m_siPersonID) );
		m_odbc.SetParam( SQL_INTEGER,	&pRecvMsg->m_siHouseNum,	sizeof(pRecvMsg->m_siHouseNum) );
		m_odbc.SetParam( SQL_SMALLINT,	&pRecvMsg->m_siItemPos,		sizeof(pRecvMsg->m_siItemPos) );
		SetItemParam( &pRecvMsg->m_clItem);
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.m_siResult);
		m_odbc.GetData( &sendMsg.m_siPersonID);
		m_odbc.GetData( &sendMsg.m_siHouseNum);
	END_FETCHDATA()

	BEGIN_NEXTRECORDSET()
		BEGIN_FETCHDATA()
			m_odbc.GetData( &sendMsg.m_siLeftItemPos );
			GetItemData( &sendMsg.m_clLeftItem );
		END_FETCHDATA()
	END_NEXTRECORDSET()

	// send
	SendPacket( (BYTE *)&sendMsg );
}
