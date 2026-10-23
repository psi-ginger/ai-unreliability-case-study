#include "DBGameProtocol.h"
#include "DBMsg-House.h"

void DBGameProtocol::HouseUnitHorseIn()
{
	// 받은 패킷
	sDBRequest_HouseUnitHorseIn *pRecvMsg = (sDBRequest_HouseUnitHorseIn *)m_pPacket;
	
	// 보낼 패킷
	sDBResponse_HouseUnitHorseIn sendMsg;
	
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_HOUSEUNITHORSEIN;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.siCharID = pRecvMsg->siCharID;

	// DB Query
	BEGIN_SP( "rsp_HouseUnitHorseIn" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID,		sizeof(pRecvMsg->siPersonID) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siVillageUnique,	sizeof(pRecvMsg->siVillageUnique) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siHouseUnitSlot,	sizeof(pRecvMsg->siHouseUnitSlot) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siPersonHorseIndex, sizeof(pRecvMsg->siPersonHorseIndex) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siHouseUnitStgHorseIndex, sizeof(pRecvMsg->siHouseUnitStgHorseIndex) );
	END_SP()
	
	EXEC_SQL()
	
	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0);

		m_odbc.GetData( &sendMsg.siPersonID, 0);
		m_odbc.GetData( &sendMsg.siVillageUnique, 0);
		m_odbc.GetData( &sendMsg.siHouseUnitSlot, 0);
		m_odbc.GetData( &sendMsg.siPersonHorseIndex, 0);
		GetHorseInfo( &sendMsg.clPersonHorse );
		m_odbc.GetData( &sendMsg.siHouseUnitStgHorseIndex, 0);
		GetHorseInfo( &sendMsg.clHouseUnitStgHorse );
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );

	// 로그 작성
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "VillageUnique = %d, HouseUnitIndex = %d, PersonHorseIndex = %d, StgHorseIndex = %d, HorseName = %s, HorseUnique = %d, Str = %d, Dex = %d, Mag = %d, MoveSpeed = %d"
				, sendMsg.siVillageUnique, sendMsg.siHouseUnitSlot, sendMsg.siPersonHorseIndex, sendMsg.siHouseUnitStgHorseIndex
				, sendMsg.clHouseUnitStgHorse.szName, sendMsg.clHouseUnitStgHorse.siHorseUnique, sendMsg.clHouseUnitStgHorse.siStr, sendMsg.clHouseUnitStgHorse.siDex, sendMsg.clHouseUnitStgHorse.siMag, sendMsg.clHouseUnitStgHorse.siMoveSpeed
				);
		sDBLogMsg_QueryString LogMsg("Horse in House", sendMsg.siPersonID, NULL, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

void DBGameProtocol::HouseUnitHorseOut()
{
	// 받은 패킷
	sDBRequest_HouseUnitHorseOut *pRecvMsg = (sDBRequest_HouseUnitHorseOut *)m_pPacket;
	
	// 보낼 패킷
	sDBResponse_HouseUnitHorseOut sendMsg;
	
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_HOUSEUNITHORSEOUT;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.siCharID = pRecvMsg->siCharID;

	// DB Query
	BEGIN_SP( "rsp_HouseUnitHorseOut" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID,		sizeof(pRecvMsg->siPersonID) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siVillageUnique,	sizeof(pRecvMsg->siVillageUnique) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siHouseUnitSlot,	sizeof(pRecvMsg->siHouseUnitSlot) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siHouseUnitStgHorseIndex,sizeof(pRecvMsg->siHouseUnitStgHorseIndex) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siPersonHorseIndex,sizeof(pRecvMsg->siPersonHorseIndex) );
	END_SP()
	
	EXEC_SQL()
	
	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0);

		m_odbc.GetData( &sendMsg.siPersonID, 0);
		m_odbc.GetData( &sendMsg.siVillageUnique, 0);
		m_odbc.GetData( &sendMsg.siHouseUnitSlot, 0);
		m_odbc.GetData( &sendMsg.siHouseUnitStgHorseIndex, 0);
		GetHorseInfo( &sendMsg.clHouseUnitHorse );
		m_odbc.GetData( &sendMsg.siPersonHorseIndex, 0);
		GetHorseInfo( &sendMsg.clPersonHorse );
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );

	// 로그 작성
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "VillageUnique = %d, HouseUnitIndex = %d, StgHorseIndex = %d, PersonHorseIndex = %d, HorseName = %s, HorseUnique = %d, Str = %d, Dex = %d, Mag = %d, MoveSpeed = %d"
				, sendMsg.siVillageUnique, sendMsg.siHouseUnitSlot, sendMsg.siHouseUnitStgHorseIndex, sendMsg.siPersonHorseIndex
				, sendMsg.clPersonHorse.szName, sendMsg.clPersonHorse.siHorseUnique, sendMsg.clPersonHorse.siStr, sendMsg.clPersonHorse.siDex, sendMsg.clPersonHorse.siMag, sendMsg.clPersonHorse.siMoveSpeed
				);
		sDBLogMsg_QueryString LogMsg("Horse Out House", sendMsg.siPersonID, NULL, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

// 마구간을 생성한다
void DBGameProtocol::HouseUnitStableCreate()
{
	// 받은 패킷
	sDBRequest_HouseUnitStableCreate *pRecvMsg = (sDBRequest_HouseUnitStableCreate *)m_pPacket;
	
	// 보낼 패킷
	sDBResponse_HouseUnitStableCreate sendMsg;
	
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_HOUSEUNITSTABLECREATE;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.siCharID = pRecvMsg->siCharID;

	// DB Query
	BEGIN_SP( "rsp_HouseUnitStableCreate" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siVillageUnique, sizeof(pRecvMsg->siVillageUnique) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siHouseUnitSlot, sizeof(pRecvMsg->siHouseUnitSlot) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siStableIndex, sizeof(pRecvMsg->siStableIndex) );
		m_odbc.SetParam( SQL_BIGINT, &pRecvMsg->siHouseUnitStableCreateFee, sizeof(pRecvMsg->siHouseUnitStableCreateFee) );
	END_SP()
	
	EXEC_SQL()
	
	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0);

		m_odbc.GetData( &sendMsg.siPersonID, 0);
		m_odbc.GetData( &sendMsg.siVillageUnique, 0);
		m_odbc.GetData( &sendMsg.siHouseUnitSlot, 0);
		m_odbc.GetData( &sendMsg.siStableIndex, 0);
		m_odbc.GetData( &sendMsg.siHouseUnitStableCreateFee, 0);
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );

	// 로그 작성
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "VillageUnique = %d, HouseUnitIndex = %d, HorseIndex = %d, HouseUnitStableCreateFee = %I64d"
				, sendMsg.siVillageUnique, sendMsg.siHouseUnitSlot, sendMsg.siStableIndex, sendMsg.siHouseUnitStableCreateFee
				);
		sDBLogMsg_QueryString LogMsg("Create Horse Stable", sendMsg.siPersonID, NULL, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}


void DBGameProtocol::HouseUnitHorseInfoSet()
{
	// 받은 패킷
	sDBRequest_HouseUnitHorseInfoSet *pRecvMsg = (sDBRequest_HouseUnitHorseInfoSet *)m_pPacket;
	
	// 보낼 패킷
	sDBResponse_HouseUnitHorseInfoSet sendMsg;
	
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_HOUSEUNITHORSEINFOSET;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.siCharID = pRecvMsg->siCharID;

	SI32 siBirthDay = pRecvMsg->clHouseUnitHorseInfo.clBirthDate.GetDateVary();
	SI32 siAddStatusDate = pRecvMsg->clHouseUnitHorseInfo.clAddStatusDate.GetDateVary();

	SI32 siPramiumPartsDate_Slot1 = pRecvMsg->clHouseUnitHorseInfo.clPremiumPartsDate_Slot1.GetDateVary();
	SI32 siPramiumPartsDate_Slot2 = pRecvMsg->clHouseUnitHorseInfo.clPremiumPartsDate_Slot2.GetDateVary();
	SI32 siPramiumPartsDate_Slot3 = pRecvMsg->clHouseUnitHorseInfo.clPremiumPartsDate_Slot3.GetDateVary();

	// DB Query
	BEGIN_SP( "rsp_HouseUnitHorseInfoSet" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siVillageUnique, sizeof(pRecvMsg->siVillageUnique) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siHouseUnitSlot, sizeof(pRecvMsg->siHouseUnitSlot) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siHouseUnitHorseIndex, sizeof(pRecvMsg->siHouseUnitHorseIndex) );
		SetHorseInfo( &pRecvMsg->clHouseUnitHorseInfo, &siBirthDay, &siAddStatusDate
			,&siPramiumPartsDate_Slot1,&siPramiumPartsDate_Slot2,&siPramiumPartsDate_Slot3 );
	END_SP()
	
	EXEC_SQL()
	
	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0);

		m_odbc.GetData( &sendMsg.siPersonID, 0);
		m_odbc.GetData( &sendMsg.siVillageUnique, 0);
		m_odbc.GetData( &sendMsg.siHouseUnitSlot, 0);
		m_odbc.GetData( &sendMsg.siHouseUnitHorseIndex, 0);
		GetHorseInfo( &sendMsg.clHouseUnitHorseInfo );
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );
	
	// 로그 작성
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "VillageUnique = %d, HouseUnitIndex = %d, StgHorseIndex = %d, HorseName = %s, HorseUnique = %d, Str = %d, Dex = %d, Mag = %d, MoveSpeed = %d, AddStatusDate = %d, StatusTIcketNum = %d"
				, sendMsg.siVillageUnique, sendMsg.siHouseUnitSlot, sendMsg.siHouseUnitHorseIndex
				, sendMsg.clHouseUnitHorseInfo.szName, sendMsg.clHouseUnitHorseInfo.siHorseUnique, sendMsg.clHouseUnitHorseInfo.siStr, sendMsg.clHouseUnitHorseInfo.siDex, sendMsg.clHouseUnitHorseInfo.siMag, sendMsg.clHouseUnitHorseInfo.siMoveSpeed
				, sendMsg.clHouseUnitHorseInfo.clAddStatusDate.GetDateVary(), sendMsg.clHouseUnitHorseInfo.siStatusTicketNum
				);
		sDBLogMsg_QueryString LogMsg("Set house horse info", sendMsg.siPersonID, NULL, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

void DBGameProtocol::HouseUnitHorseEatFood()
{
	// 받은 패킷
	sDBRequest_HouseUnitHorseEatFood *pRecvMsg = (sDBRequest_HouseUnitHorseEatFood *)m_pPacket;
	
	// 보낼 패킷
	sDBResponse_HouseUnitHorseEatFood sendMsg;
	
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_HOUSEUNITHORSEEATFOOD;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.siCharID = pRecvMsg->siCharID;
	
	// DB Query
	BEGIN_SP( "rsp_HouseUnitHorseEatFood" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siVillageUnique, sizeof(pRecvMsg->siVillageUnique) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siHouseUnitSlot, sizeof(pRecvMsg->siHouseUnitSlot) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siHouseUnitItemIndex, sizeof(pRecvMsg->siHouseUnitItemIndex) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siHouseUnitHorseEatItemUnique, sizeof(pRecvMsg->siHouseUnitHorseEatItemUnique) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siHouseUnitHorseEatItemNum, sizeof(pRecvMsg->siHouseUnitHorseEatItemNum) );
	END_SP()
	
	EXEC_SQL()
	
	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0 );
		m_odbc.GetData( &sendMsg.siPersonID, 0 );
		m_odbc.GetData( &sendMsg.siVillageUnique, 0 );
		m_odbc.GetData( &sendMsg.siHouseUnitSlot, 0 );
		m_odbc.GetData( &sendMsg.siHouseUnitHorseEatItemUnique, 0 );
		m_odbc.GetData( &sendMsg.siHouseUnitHorseEatItemNum, 0 );
		m_odbc.GetData( &sendMsg.siHouseUnitItemIndex, 0 );
		GetItemData( &sendMsg.clHouseUnitItem );
	END_FETCHDATA()
	
	// send
	SendPacket( (BYTE *)&sendMsg );
}
