#include "DBGameProtocol.h"
//#include "DBMsg.h"
#include "DBMsg-Cityhall.h"
#include "DBMsg-system.h"
#include "DBMsg-Village.h"
#include "DBMsg-Item.h"

void DBGameProtocol::GetVillageInfo()
{
	// 받은 패킷
	sDBRequest_GetVillageInfo *pRecvMsg = (sDBRequest_GetVillageInfo *)m_pPacket;
	
	// 보낼 패킷
	sDBResponse_GetVillageInfo sendMsg;
	
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_GETVILLAGEINFO;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->usCharID;
	
	// DB Query
	BEGIN_SP( "rsp_VillageInfoGet" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siVillageUnique, sizeof(pRecvMsg->siVillageUnique) );
	END_SP()
	
	EXEC_SQL()
	
	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siVillageUnique, 0);
		m_odbc.GetData( &sendMsg.siMasterVillage, 0);
		m_odbc.GetData( &sendMsg.siParentVillage, 0);
		m_odbc.GetData( &sendMsg.siVillageMark, 0);
		m_odbc.GetData( &sendMsg.siNewYearEventScore, 0);
	END_FETCHDATA()

	BEGIN_NEXTRECORDSET()
		BEGIN_FETCHDATA()
			m_odbc.GetData( &sendMsg.siTotalVillageScore, 0);
		END_FETCHDATA()
	END_NEXTRECORDSET()
	
	BEGIN_NEXTRECORDSET()
		BEGIN_FETCHDATA()
			m_odbc.GetData( &sendMsg.m_bGeneralMeetingSuggest, 0);
		END_FETCHDATA()
	END_NEXTRECORDSET()

	// send
	SendPacket( (BYTE *)&sendMsg );
}

void DBGameProtocol::SetParentVillage()
{
	// 받은 패킷
	sDBRequest_SetParentVillage *pRecvMsg = (sDBRequest_SetParentVillage *)m_pPacket;
	
	// 보낼 패킷
	sDBResponse_SetParentVillage sendMsg;
	
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_SETPARENTVILLAGE;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->usCharID;
	
	// DB Query
	BEGIN_SP( "rsp_VillageParentSet" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siVillageUnique, sizeof(pRecvMsg->siVillageUnique) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siParentVillage, sizeof(pRecvMsg->siParentVillage) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siMasterVillage, sizeof(pRecvMsg->siMasterVillage) );
	END_SP()
	
	EXEC_SQL()
	
	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0);
		m_odbc.GetData( &sendMsg.siVillageUnique, 0);
		m_odbc.GetData( &sendMsg.siParentVillage, 0);
		m_odbc.GetData( &sendMsg.siMasterVillage, 0);
	END_FETCHDATA()
	
	// send
	SendPacket( (BYTE *)&sendMsg );

	// 로그 작성
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "VillageUnique = %d, ParentVillage = %d, MasterVillage = %d"
				,	pRecvMsg->siVillageUnique, pRecvMsg->siParentVillage, pRecvMsg->siMasterVillage
				);
		sDBLogMsg_QueryString LogMsg("End Village War", 0, 0, 0, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

// 선전포고
void DBGameProtocol::DeclareWar()
{
	// 받은 패킷
	sDBRequest_DeclareWar *pRecvMsg = (sDBRequest_DeclareWar *)m_pPacket;
	
	// 보낼 패킷
	sDBResponse_DeclareWar sendMsg;
	
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_DECLAREWAR;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->usCharID;
	
	// DB Query
	BEGIN_SP( "rsp_VillageDeclareWar" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siVillageUnique, sizeof(pRecvMsg->siVillageUnique) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siTargetVillage, sizeof(pRecvMsg->siTargetVillage) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->clDate.m_uiYear, sizeof(pRecvMsg->clDate.m_uiYear) );
		m_odbc.SetParam( SQL_TINYINT, &pRecvMsg->clDate.m_uiMonth, sizeof(pRecvMsg->clDate.m_uiMonth) );
		m_odbc.SetParam( SQL_TINYINT, &pRecvMsg->clDate.m_uiDay, sizeof(pRecvMsg->clDate.m_uiDay) );
	END_SP()
	
	EXEC_SQL()
	
	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0);
		m_odbc.GetData( &sendMsg.siVillageUnique, 0);
		m_odbc.GetData( &sendMsg.siTargetVillage, 0);
	END_FETCHDATA()

	sendMsg.clDate.Set( &pRecvMsg->clDate );
	
	// send
	SendPacket( (BYTE *)&sendMsg );

	// 로그 작성
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "VillageUnique = %d, TargetVillage = %d"
				,	pRecvMsg->siVillageUnique, pRecvMsg->siTargetVillage
				);
		sDBLogMsg_QueryString LogMsg("Declare War", 0, 0, 0, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

void DBGameProtocol::CancelDeclareWar()
{
	// 받은 패킷
	sDBRequest_CancelDeclareWar *pRecvMsg = (sDBRequest_CancelDeclareWar *)m_pPacket;
	
	// 보낼 패킷
	sDBResponse_CancelDeclareWar sendMsg;
	
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_CANCELDECLAREWAR;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->usCharID;
	
	SI32 siDate = pRecvMsg->clDate.GetDateVary();

	// DB Query
	BEGIN_SP( "rsp_VillageDeclareWarCancel" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siVillageUnique, sizeof(pRecvMsg->siVillageUnique) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siTargetVillage, sizeof(pRecvMsg->siTargetVillage) );
	END_SP()
	
	EXEC_SQL()
	
	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0);
		m_odbc.GetData( &sendMsg.siVillageUnique, 0);
		m_odbc.GetData( &sendMsg.siTargetVillage, 0);
	END_FETCHDATA()

	sendMsg.clDate.Set(&pRecvMsg->clDate);

	// send
	SendPacket( (BYTE *)&sendMsg );

	// 로그 작성
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "VillageUnique = %d, TargetVillage = %d"
				,	pRecvMsg->siVillageUnique, pRecvMsg->siTargetVillage
				);
		sDBLogMsg_QueryString LogMsg("Declare War Cancel", 0, 0, 0, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}


void DBGameProtocol::SetVillageSymbol()
{
	// 받은 패킷
	sDBRequest_SetVillageSymbol *pRecvMsg = (sDBRequest_SetVillageSymbol *)m_pPacket;
	
	// DB Query
	BEGIN_SP( "rsp_VillageSymbolSet" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siVillageUnique, sizeof(pRecvMsg->siVillageUnique) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siSymbolKind, sizeof(pRecvMsg->siSymbolKind) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siSymbolLife, sizeof(pRecvMsg->siSymbolLife) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siSymbolMana, sizeof(pRecvMsg->siSymbolMana) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siSymbolCapa, sizeof(pRecvMsg->siSymbolCapa) );
	END_SP()
	
	EXEC_SQL()

	// 로그 작성
	{
		char log[512] = "";
		sprintf(log, "VillageUnique = %d, Kind = %d, Life = %d, Mana = %d, Capa = %d"
				,	pRecvMsg->siVillageUnique, pRecvMsg->siSymbolKind, pRecvMsg->siSymbolLife, pRecvMsg->siSymbolMana, pRecvMsg->siSymbolCapa
				);
		sDBLogMsg_QueryString LogMsg("Symbol Set", 0, 0, 0, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

void DBGameProtocol::SetSymbolStatus()
{
	// 받은 패킷
	sDBRequest_SetSymbolStatus *pRecvMsg = (sDBRequest_SetSymbolStatus *)m_pPacket;
	
	// 보낼 패킷
	sDBResponse_SetSymbolStatus sendMsg;
	
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_SETSYMBOLSTATUS;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->usCharID;
	
	SI32 siDate = pRecvMsg->clDate.GetDateVary();

	// DB Query
	BEGIN_SP( "rsp_VillageSymbolStatusSet" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siVillageUnique, sizeof(pRecvMsg->siVillageUnique) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siStatKind, sizeof(pRecvMsg->siStatKind) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siStatValue, sizeof(pRecvMsg->siStatValue) );
		m_odbc.SetParam( SQL_BIGINT, &pRecvMsg->siCost, sizeof(pRecvMsg->siCost) );
		m_odbc.SetParam( SQL_INTEGER, &siDate, sizeof(siDate) );
	END_SP()
	
	EXEC_SQL()
	
	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0);
		m_odbc.GetData( &sendMsg.siVillageUnique, 0);
		m_odbc.GetData( &sendMsg.siStatKind, 0);
		m_odbc.GetData( &sendMsg.siStatValue, 0);
		m_odbc.GetData( &sendMsg.siCost, 0);
		m_odbc.GetData( &siDate, 0); sendMsg.clDate.MakeFullDateFromDateVary( siDate );
		m_odbc.GetData( &sendMsg.clCityhallMoney.itMoney, 0);
	END_FETCHDATA()

	sendMsg.clDate.Set( &pRecvMsg->clDate );

	// send
	SendPacket( (BYTE *)&sendMsg );
	
	// 로그 작성
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "VillageUnique = %d, StatKind = %d, StatValue = %d, Cost = %I64d, LeftCityHallMoney = %I64d"
				, sendMsg.siVillageUnique, sendMsg.siStatKind, sendMsg.siStatValue, sendMsg.siCost, sendMsg.clCityhallMoney.itMoney
				);
		sDBLogMsg_QueryString LogMsg("Symbol Status Set", 0, NULL, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

void DBGameProtocol::SetVillageCastle()
{
	// 받은 패킷
	sDBRequest_SetVillageCastle *pRecvMsg = (sDBRequest_SetVillageCastle *)m_pPacket;
	
	// DB Query
	BEGIN_SP( "rsp_VillageCastleSet" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siVillageUnique, sizeof(pRecvMsg->siVillageUnique) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siCastleKind, sizeof(pRecvMsg->siCastleKind) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siCastleLife, sizeof(pRecvMsg->siCastleLife) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siCastleMana, sizeof(pRecvMsg->siCastleMana) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siCastleCapa, sizeof(pRecvMsg->siCastleCapa) );
	END_SP()
	
	EXEC_SQL()
	// 로그 작성
	{
		char log[512] = "";
		sprintf(log, "VillageUnique = %d, Kind = %d, Life = %d, Mana = %d, Capa = %d"
				,	pRecvMsg->siVillageUnique, pRecvMsg->siCastleKind, pRecvMsg->siCastleLife, pRecvMsg->siCastleMana, pRecvMsg->siCastleCapa
				);
		sDBLogMsg_QueryString LogMsg("Castle Set", 0, 0, 0, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

void DBGameProtocol::SetCastleStatus()
{
	// 받은 패킷
	sDBRequest_SetCastleStatus *pRecvMsg = (sDBRequest_SetCastleStatus *)m_pPacket;
	
	// 보낼 패킷
	sDBResponse_SetCastleStatus sendMsg;
	
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_SETCASTLESTATUS;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->usCharID;
	
	SI32 siDate = pRecvMsg->clDate.GetDateVary();

	// DB Query
	BEGIN_SP( "rsp_VillageCastleStatusSet" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siVillageUnique, sizeof(pRecvMsg->siVillageUnique) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siStatKind, sizeof(pRecvMsg->siStatKind) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siStatValue, sizeof(pRecvMsg->siStatValue) );
		m_odbc.SetParam( SQL_BIGINT, &pRecvMsg->siCost, sizeof(pRecvMsg->siCost) );
		m_odbc.SetParam( SQL_INTEGER, &siDate, sizeof(siDate) );
	END_SP()
	
	EXEC_SQL()
	
	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0);
		m_odbc.GetData( &sendMsg.siVillageUnique, 0);
		m_odbc.GetData( &sendMsg.siStatKind, 0);
		m_odbc.GetData( &sendMsg.siStatValue, 0);
		m_odbc.GetData( &sendMsg.siCost, 0);
		m_odbc.GetData( &siDate, 0); sendMsg.clDate.MakeFullDateFromDateVary( siDate );
		m_odbc.GetData( &sendMsg.clCityhallMoney.itMoney, 0);
	END_FETCHDATA()

	sendMsg.clDate.Set( &pRecvMsg->clDate );

	// send
	SendPacket( (BYTE *)&sendMsg );

	// 로그 작성
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "VillageUnique = %d, StatKind = %d, StatValue = %d, Cost = %I64d, LeftCityHallMoney = %I64d"
				, sendMsg.siVillageUnique, sendMsg.siStatKind, sendMsg.siStatValue, sendMsg.siCost, sendMsg.clCityhallMoney.itMoney
				);
		sDBLogMsg_QueryString LogMsg("Castle Status Set", 0, NULL, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

void DBGameProtocol::SetVillageLevel()
{
	// 받은 패킷
	sDBRequest_SetVillageLevel *pRecvMsg = (sDBRequest_SetVillageLevel *)m_pPacket;
	
	// 보낼 패킷
	sDBResponse_SetVillageLevel sendMsg;
	
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_SETVILLAGELEVEL;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->usCharID;
	
	// DB Query
	BEGIN_SP( "rsp_VillageLevelSet" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siVillageUnique, sizeof(pRecvMsg->siVillageUnique) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siLevel, sizeof(pRecvMsg->siLevel) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siVisitLevel, sizeof(pRecvMsg->siVisitLevel) );
	END_SP()
	
	EXEC_SQL()
	
	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0);
		m_odbc.GetData( &sendMsg.siVillageUnique, 0);
		m_odbc.GetData( &sendMsg.siLevel, 0);
		m_odbc.GetData( &sendMsg.siVisitLevel, 0 );
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );

	// 로그 작성
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "VillageUnique = %d, Level = %d"
				, sendMsg.siVillageUnique, sendMsg.siLevel
				);
		sDBLogMsg_QueryString LogMsg("Village Level Set", 0, NULL, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

void DBGameProtocol::SetVillageMark()
{
	// 받은 패킷
	sDBRequest_SetVillageMark *pRecvMsg = (sDBRequest_SetVillageMark *)m_pPacket;
	
	// 보낼 패킷
	sDBResponse_SetVillageMark sendMsg;
	
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_SETVILLAGEMARK;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->usCharID;
	
	SI32 siDate = pRecvMsg->clDate.GetDateVary();

	// DB Query
	BEGIN_SP( "rsp_VillageMarkSet" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siVillageUnique, sizeof(pRecvMsg->siVillageUnique) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siVillageMark, sizeof(pRecvMsg->siVillageMark) );
		m_odbc.SetParam( SQL_INTEGER, &siDate, sizeof(siDate) );
	END_SP()
	
	EXEC_SQL()
	
	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0);
		m_odbc.GetData( &sendMsg.siVillageUnique, 0);
		m_odbc.GetData( &sendMsg.siVillageMark, 0);
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );

	// 로그 작성
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "VillageUnique = %d, Mark = %d"
				, sendMsg.siVillageUnique, sendMsg.siVillageMark
				);
		sDBLogMsg_QueryString LogMsg("Village Mark Set", 0, NULL, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

void DBGameProtocol::MapTypeInfo()
{
	// 받은 패킷
	sDBRequest_MapTypeInfo *pRecvMsg	= (sDBRequest_MapTypeInfo *)m_pPacket;
	
	// 보낼 패킷
	sDBResponse_MapTypeInfo sendMsg;
	
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_MAPTYPEINFO;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
//	sendMsg.usCharID = pRecvMsg->usCharID;
	
	// DB Query
	BEGIN_SP( "rsp_VillageMapTypeInfoGet" )
	END_SP()

	EXEC_SQL()
	
	SI32 siCount = 0;
	BEGIN_FETCHDATA()
		if(siCount >= MAX_MAP_NUMBER)
			continue;
		m_odbc.GetData( &siCount, 0 );
		m_odbc.GetData( &sendMsg.siMapTypeInfo[siCount], 0 );
	END_FETCHDATA()
	
	sendMsg.siResult = 1;

	// send
	SendPacket( (BYTE *)&sendMsg );
}

void DBGameProtocol::SetVillageMap()
{
	// 받은 패킷
	sDBRequest_SetVillageMap *pRecvMsg	= (sDBRequest_SetVillageMap *)m_pPacket;
	
	// 보낼 패킷
	sDBResponse_SetVillageMap sendMsg;
	
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_SETVILLAGEMAP;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->usCharID;

	// DB Query
	BEGIN_SP( "rsp_VillageMapSet" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siVillageUnique, sizeof(pRecvMsg->siVillageUnique) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siVillageMapIndex, sizeof(pRecvMsg->siVillageMapIndex) );
	END_SP()

	EXEC_SQL()
	
	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0 );
	END_FETCHDATA()

	sendMsg.siVillageUnique = pRecvMsg->siVillageUnique;
	sendMsg.siVillageMapIndex = pRecvMsg->siVillageMapIndex;
	sendMsg.siNeedMoney = pRecvMsg->siNeedMoney;
	sendMsg.siMapType = pRecvMsg->siMapType;

	// send
	SendPacket( (BYTE *)&sendMsg );

	// 로그 작성
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "VillageUnique = %d, MapIndex = %d, NeedMoney = %I64d"
				, pRecvMsg->siVillageUnique, pRecvMsg->siVillageMapIndex, pRecvMsg->siNeedMoney
				);
		sDBLogMsg_QueryString LogMsg( "Village Map Change", NULL, NULL, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

void DBGameProtocol::VillageEventSuccess()
{
	// 받은 패킷
	sDBRequest_VillageEventSuccess *pRecvMsg	= (sDBRequest_VillageEventSuccess *)m_pPacket;

	// 보낼 패킷
	//	sDBResponse_VillageEventSuccess sendMsg;

	//	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_VILLAGEEVENTSUCCESS;
	//	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	//	sendMsg.usCharID = pRecvMsg->usCharID;

	// DB Query
	BEGIN_SP( "rsp_VillageEventSuccess" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siVillageUnique, sizeof(pRecvMsg->siVillageUnique) );
	END_SP()

	EXEC_SQL()

	SI32 siResult = 0;
	SI32 siSuccessCount = 0;
	BEGIN_FETCHDATA()
		m_odbc.GetData( &siResult, 0 );
		m_odbc.GetData( &siSuccessCount, 0 );
	END_FETCHDATA()

	// send
	//	SendPacket( (char *)&sendMsg );

	// 로그 작성
	if(siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "VillageUnique = %d, SuccessCount = %d"
			, pRecvMsg->siVillageUnique, siSuccessCount
			);
		sDBLogMsg_QueryString LogMsg( "Village Event Success", NULL, NULL, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

void DBGameProtocol::VillageWarKillNumber()
{
	// 받은 패킷
	sDBRequest_VillageWarKillNumber *pRecvMsg	= (sDBRequest_VillageWarKillNumber *)m_pPacket;

	// 보낼 패킷
	//	sDBResponse_VillageWarKillNumber sendMsg;

	//	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_VILLAGEWARKILLNUMBER;
	//	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	//	sendMsg.usCharID = pRecvMsg->usCharID;

	// DB Query
	for(int siVillageUnique = 0; siVillageUnique < MAX_VILLAGE_NUMBER; siVillageUnique++)
	{
		if(pRecvMsg->siVillageUnique[siVillageUnique] == 0)
			continue;

		BEGIN_SP( "rsp_VillageWarKillNumber" )
			m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siVillageUnique[siVillageUnique], sizeof(pRecvMsg->siVillageUnique[siVillageUnique]));
			m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siVillageWarKillNumber[siVillageUnique], sizeof(pRecvMsg->siVillageWarKillNumber[siVillageUnique]));
		END_SP()

		EXEC_SQL()

		SI32 siResult			= 0;
		SI32 siWarKillNumber	= 0;
		BEGIN_FETCHDATA()
			m_odbc.GetData( &siResult, 0 );
			m_odbc.GetData( &siWarKillNumber, 0 );
		END_FETCHDATA()

		// 로그 작성
		if(siResult == 1)
		{
			char log[512] = "";
			sprintf(log, "VillageUnique = %d, WarKillNumber = %d, TotalWarKillNumber = %d"
				, pRecvMsg->siVillageUnique[siVillageUnique], pRecvMsg->siVillageWarKillNumber[siVillageUnique], siWarKillNumber
				);
			sDBLogMsg_QueryString LogMsg( "Village WarKillNumber", NULL, NULL, NULL, log);
			SendLogMsg((sPacketHeader *)&LogMsg);
		}
	}
}

void DBGameProtocol::VillageJoinPlayTime()
{
	// 받은 패킷
	sDBRequest_VillageJoinPlayTime *pRecvMsg	= (sDBRequest_VillageJoinPlayTime *)m_pPacket;

	// 보낼 패킷 (PCK : 로그용 추가)
	sDBReponse_VillageJoinPlayTime sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_VILLAGEJOINPLAYTIME;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
//	sendMsg.usCharID = pRecvMsg->usCharID;
	sendMsg.siVillageJoinPlayTime = pRecvMsg->siVillageJoinPlayTime;
	sendMsg.siVillageUnique = pRecvMsg->siVillageUnique;

	BEGIN_SP( "rsp_VillageJoinPlayTime" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID));
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siVillageJoinPlayTime, sizeof(pRecvMsg->siVillageJoinPlayTime));
	END_SP()

	EXEC_SQL()

	SI32 siResult			= 0;
	SI32 siVillageUnique	= 0;
	BEGIN_FETCHDATA()
		m_odbc.GetData( &siResult, 0 );
		m_odbc.GetData( &siVillageUnique, 0 );
	END_FETCHDATA()

	// 로그 작성
	if(siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "VillageUnique = %d, PlayTime = %d"
			, siVillageUnique, pRecvMsg->siVillageJoinPlayTime
			);
		sDBLogMsg_QueryString LogMsg( "VillageJoin Playtime", pRecvMsg->siPersonID, NULL, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

void DBGameProtocol::VillageJoinLevelUp()
{
	// 받은 패킷
	sDBRequest_VillageJoinLevelUp *pRecvMsg	= (sDBRequest_VillageJoinLevelUp *)m_pPacket;

	// 보낼 패킷
	sDBResponse_VillageJoinLevelUp sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_VILLAGEJOINLEVELUP;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
//	sendMsg.usCharID = pRecvMsg->usCharID;
	sendMsg.siLevelCount = pRecvMsg->siLevelCount;
	sendMsg.siVillageUnique = pRecvMsg->siVillageUnique;

	BEGIN_SP( "rsp_VillageJoinLevelUp" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID));
	END_SP()

	EXEC_SQL()

	SI32 siResult			= 0;
	SI32 siVillageUnique	= 0;
	SI32 siLevelUpCount		= 0;
	BEGIN_FETCHDATA()
		m_odbc.GetData( &siResult, 0 );
		m_odbc.GetData( &siLevelUpCount, 0 );
		m_odbc.GetData( &siVillageUnique, 0 );
	END_FETCHDATA()

	// 로그 작성
	if(siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "VillageUnique = %d, LevelUpCount = %d"
			, siVillageUnique, siLevelUpCount
			);
		sDBLogMsg_QueryString LogMsg( "VillageJoin Levelup", pRecvMsg->siPersonID, NULL, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

void DBGameProtocol::VillageJoinWarKillNumber()
{
	// 받은 패킷
	sDBRequest_VillageJoinWarKillNumber *pRecvMsg	= (sDBRequest_VillageJoinWarKillNumber *)m_pPacket;

	// 보낼 패킷
	//	sDBResponse_VillageWarKillNumber sendMsg;

	//	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_VILLAGEWARKILLNUMBER;
	//	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	//	sendMsg.usCharID = pRecvMsg->usCharID;

	BEGIN_SP( "rsp_VillageJoinWarKillNumber" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID));
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siVillageJoinWarKillNumber, sizeof(pRecvMsg->siVillageJoinWarKillNumber));
	END_SP()

	EXEC_SQL()

	SI32 siResult			= 0;
	SI32 siVillageUnique	= 0;
	SI32 siTotalKillNumber	= 0;
	BEGIN_FETCHDATA()
		m_odbc.GetData( &siResult, 0 );
		m_odbc.GetData( &siTotalKillNumber, 0 );
		m_odbc.GetData( &siVillageUnique, 0 );
	END_FETCHDATA()

	// 로그 작성
	if(siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "VillageUnique = %d, WarKillNumber = %d, TotalWarKillNumber = %d"
			, siVillageUnique, pRecvMsg->siVillageJoinWarKillNumber, siTotalKillNumber
			);
		sDBLogMsg_QueryString LogMsg( "VillageJoin WarKillNumber", pRecvMsg->siPersonID, NULL, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

void DBGameProtocol::VillageJoinMakeRareItem()
{
	// 받은 패킷
	sDBRequest_VillageJoinMakeRareItem *pRecvMsg	= (sDBRequest_VillageJoinMakeRareItem *)m_pPacket;

	// 보낼 패킷
	//	sDBResponse_VillageWarKillNumber sendMsg;

	//	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_VILLAGEWARKILLNUMBER;
	//	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	//	sendMsg.usCharID = pRecvMsg->usCharID;

	BEGIN_SP( "rsp_VillageJoinMakeRareItem" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID));
	END_SP()

	EXEC_SQL()

	SI32 siResult			= 0;
	SI32 siVillageUnique	= 0;
	SI32 siTotalMakeCount	= 0;
	BEGIN_FETCHDATA()
		m_odbc.GetData( &siResult, 0 );
		m_odbc.GetData( &siTotalMakeCount, 0 );
		m_odbc.GetData( &siVillageUnique, 0 );
	END_FETCHDATA()

	// 로그 작성
	if(siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "VillageUnique = %d, TotalMakeCount = %d"
			, siVillageUnique, siTotalMakeCount
			);
		sDBLogMsg_QueryString LogMsg( "VillageJoin MakeRareItem", pRecvMsg->siPersonID, NULL, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}


void DBGameProtocol::VillageResidentsVisitorsInfo()
{
	// 받은 패킷
	sDBRequest_VillageResidentsVisitorsInfo *pRecvMsg	= (sDBRequest_VillageResidentsVisitorsInfo *)m_pPacket;

	// 보낼 패킷
	sDBResponse_VillageResidentsVisitorsInfo sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_VILLAGERESIDENTSVISITORSINFO;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	//sendMsg.usCharID = pRecvMsg->usCharID;

	BEGIN_SP( "rsp_VillageResidentsVisitorsInfo" )
	END_SP()

	EXEC_SQL()

	sendMsg.siResult = 1;

	// send
	SendPacket( (BYTE *)&sendMsg );
}
/*
void DBGameProtocol::NewYearEventSuccess()
{
	// 받은 패킷
	sDBRequest_NewYearEventSuccess *pRecvMsg	= (sDBRequest_NewYearEventSuccess *)m_pPacket;

	// 보낼 패킷
	sDBResponse_NewYearEventSuccess sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_NEWYEAREVENTSUCCESS;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	//	sendMsg.usCharID = pRecvMsg->usCharID;

	BEGIN_SP( "rsp_VillageNewYearEventSuccess" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siVillageUnique, sizeof(pRecvMsg->siVillageUnique));
		m_odbc.SetParam( SQL_TINYINT, &pRecvMsg->bIsPlus, sizeof(pRecvMsg->bIsPlus));
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siWinCount, 0 );
	END_FETCHDATA()

	sendMsg.siVillageUnique = pRecvMsg->siVillageUnique ;

	// 로그 작성
	//	if(siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "VillageUnique = %d, Plus = %d, WinCount = %d"
			, pRecvMsg->siVillageUnique, pRecvMsg->bIsPlus, sendMsg.siWinCount
			);
		sDBLogMsg_QueryString LogMsg( "Village NewYearEventSuccess", NULL, NULL, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}

	SendPacket((BYTE*)&sendMsg);
}
*/

void DBGameProtocol::VillageJoinMakeItem()
{
	// 받은 패킷
	sDBRequest_VillageJoinMakeItem *pRecvMsg	= (sDBRequest_VillageJoinMakeItem *)m_pPacket;

	// 보낼 패킷
	sDBResponse_VillageJoinMakeItem sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_VILLAGEJOINMAKEITEM;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	//sendMsg.usCharID = pRecvMsg->usCharID;
	sendMsg.siItemSkillExp = pRecvMsg->siItemSkillExp;
	sendMsg.siVillageUnique = pRecvMsg->siVillageUnique;

	BEGIN_SP( "rsp_VillageJoinMakeItemSkillExp" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID));
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siItemSkillExp, sizeof(pRecvMsg->siItemSkillExp));
	END_SP()

	EXEC_SQL()

	SI32 siResult			= 0;
	SI32 siVillageUnique	= 0;
	SI32 siMakeItemSkillExp	= 0;
	BEGIN_FETCHDATA()
		m_odbc.GetData( &siResult, 0 );
		m_odbc.GetData( &siVillageUnique, 0 );
		m_odbc.GetData( &siMakeItemSkillExp, 0 );
	END_FETCHDATA()

	// 로그 작성
	if(siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "VillageUnique = %d, SkillExp = %d, AfterMakeSkillExp = %d"
			, siVillageUnique, pRecvMsg->siItemSkillExp, siMakeItemSkillExp
			);
		sDBLogMsg_QueryString LogMsg( "VillageJoin MakeItemSkillExp", pRecvMsg->siPersonID, NULL, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

void DBGameProtocol::VillageWarResult()
{
	// 받은 패킷
	sDBRequest_SetVillageWarResult *pRecvMsg	= (sDBRequest_SetVillageWarResult *)m_pPacket;

	// 보낼 패킷
	sDBResponse_SetVillageWarResult sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_SETVILLAGEWAR_RESULT;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.m_clDateLoser.Set( &pRecvMsg->m_clDateLoser );
	sendMsg.m_siwinnerGetVillageMoney = pRecvMsg->m_siwinnerGetVillageMoney;

//	sendMsg.usCharID = pRecvMsg->usCharID;

	BEGIN_SP( "rsp_VillageWarResult" )
		m_odbc.SetParam( SQL_INTEGER,  &pRecvMsg->m_siwinVillageUnique,			sizeof(pRecvMsg->m_siwinVillageUnique));
		m_odbc.SetParam( SQL_INTEGER,  &pRecvMsg->m_siloseVillageUnique,		sizeof(pRecvMsg->m_siloseVillageUnique));
		m_odbc.SetParam( SQL_INTEGER,  &pRecvMsg->m_siwinnerGetVillageWarScore, sizeof(pRecvMsg->m_siwinnerGetVillageWarScore));
		m_odbc.SetParam( SQL_BIGINT,   &pRecvMsg->m_siwinnerGetVillageMoney,	sizeof(pRecvMsg->m_siwinnerGetVillageMoney));
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->m_clDateLoser.m_uiYear,		sizeof(pRecvMsg->m_clDateLoser.m_uiYear) );
		m_odbc.SetParam( SQL_TINYINT,  &pRecvMsg->m_clDateLoser.m_uiMonth,		sizeof(pRecvMsg->m_clDateLoser.m_uiMonth) );
		m_odbc.SetParam( SQL_TINYINT,  &pRecvMsg->m_clDateLoser.m_uiDay,		sizeof(pRecvMsg->m_clDateLoser.m_uiDay) );
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult);
		m_odbc.GetData( &sendMsg.m_siwinVillageUnique );
		m_odbc.GetData( &sendMsg.m_siloseVillageUnique );
		m_odbc.GetData( &sendMsg.m_siwinnerVillageMoney );
		m_odbc.GetData( &sendMsg.m_siloserVillageMoney );		
	END_FETCHDATA()

	sendMsg.m_clDateLoser.Set( &pRecvMsg->m_clDateLoser );

	// send
	SendPacket( (BYTE *)&sendMsg );

	// 로그 작성
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "VillageUnique = %d, Win = %d, WarScore = %d, AddVillageMoney = %I64d, AfterVillageMoney = %I64d"
			, sendMsg.m_siwinVillageUnique, 1, pRecvMsg->m_siwinnerGetVillageWarScore, pRecvMsg->m_siwinnerGetVillageMoney, sendMsg.m_siwinnerVillageMoney
			);
		sDBLogMsg_QueryString LogMsg( "VillageWar Result", NULL, NULL, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "VillageUnique = %d, Win = %d, WarScore = %d, AddVillageMoney = %I64d, AfterVillageMoney = %I64d"
			, sendMsg.m_siloseVillageUnique, 0, pRecvMsg->m_siwinnerGetVillageWarScore*-1, pRecvMsg->m_siwinnerGetVillageMoney*-1, sendMsg.m_siloserVillageMoney
			);
		sDBLogMsg_QueryString LogMsg( "VillageWar Result", NULL, NULL, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

void DBGameProtocol::Event_InsertMaterial()
{

	// 받은 패킷
	sDBRequest_Event_InsertMaterial *pRecvMsg	= (sDBRequest_Event_InsertMaterial *)m_pPacket;

	// 보낼 패킷
	sDBResponse_Event_InsertMaterial sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_EVENT_INSERTMATERIAL;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->m_usCharID;

	BEGIN_SP( "rsp_XmasEventInsertMaterial" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->m_siPersonID, sizeof(pRecvMsg->m_siPersonID));
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->m_siVillageUnique, sizeof(pRecvMsg->m_siVillageUnique));
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->m_siConstructType, sizeof(pRecvMsg->m_siConstructType));
		for(SI32 i = 0; i < MAX_MATERIAL_NUMBER; i++)
		{
			m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->m_clMaterial[i].siStgPos, sizeof(pRecvMsg->m_clMaterial[i].siStgPos));
			m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->m_clMaterial[i].siItemUnique, sizeof(pRecvMsg->m_clMaterial[i].siItemUnique));
			m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->m_clMaterial[i].siUseItemNum, sizeof(pRecvMsg->m_clMaterial[i].siUseItemNum));
			m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->m_clMaterial[i].siNeedNum, sizeof(pRecvMsg->m_clMaterial[i].siNeedNum));
		}
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult);
		m_odbc.GetData( &sendMsg.siPersonID );
		m_odbc.GetData( &sendMsg.m_siVillageUnique );
		m_odbc.GetData( &sendMsg.m_siConstructType );
	END_FETCHDATA()

	SI32 siIndex = 0;
	BEGIN_NEXTRECORDSET()
		BEGIN_FETCHDATA()
			if(siIndex >= MAX_MATERIAL_NUMBER) break;
			m_odbc.GetData(&sendMsg.clMaterial[siIndex].siStgPos);
			m_odbc.GetData(&sendMsg.clMaterial[siIndex].siItemUnique);
			m_odbc.GetData(&sendMsg.clMaterial[siIndex].siUseItemNum);
			m_odbc.GetData(&sendMsg.clMaterial[siIndex].siNeedNum);
			siIndex++;
		END_FETCHDATA()
	END_NEXTRECORDSET()

	// send
	SendPacket( (BYTE *)&sendMsg );

	// 변경된 인벤토리의 정보를 보낸다.
	sDBResponse_SetItem sendItemMsg;
	sendItemMsg.packetHeader.usCmd = DBMSG_RESPONSE_SETITEM;
	sendItemMsg.packetHeader.usSize = sizeof( sendItemMsg );
	sendItemMsg.usCharID = pRecvMsg->m_usCharID;
	sendItemMsg.siResult = 1;
	sendItemMsg.siPersonID = pRecvMsg->m_siPersonID;

	BEGIN_NEXTRECORDSET()
		BEGIN_FETCHDATA()
			m_odbc.GetData(&sendItemMsg.siPos);
			GetItemData(&sendItemMsg.clItem);

			// send
			SendPacket( (BYTE *)&sendItemMsg );
		END_FETCHDATA()
	END_NEXTRECORDSET()

}

void DBGameProtocol::Event_MaterialList()
{
	// 받은 패킷
	sDBRequest_Event_MaterialList *pRecvMsg	= (sDBRequest_Event_MaterialList *)m_pPacket;

	// 보낼 패킷
	sDBResponse_Event_MaterialList sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_EVENT_MATERIALLIST;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->m_usCharID;

	SI32 siIndex = 0;

	BEGIN_SP( "rsp_XmasEventMaterialList" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->m_siPersonID, sizeof(pRecvMsg->m_siPersonID));
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->m_siVillageUnique, sizeof(pRecvMsg->m_siVillageUnique));
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->m_siConstructType, sizeof(pRecvMsg->m_siConstructType));
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult );
		m_odbc.GetData( &sendMsg.siPersonID );
		m_odbc.GetData( &sendMsg.m_siVillageUnique );
		m_odbc.GetData( &sendMsg.m_siConstructType );
	END_FETCHDATA()

	BEGIN_NEXTRECORDSET()
		BEGIN_FETCHDATA()
			m_odbc.GetData(&siIndex);
			if(siIndex >= MAX_MATERIAL_NUMBER) continue;
			m_odbc.GetData(&sendMsg.m_siItemUnique[siIndex]);
			m_odbc.GetData(&sendMsg.m_siItemNum[siIndex]);
		END_FETCHDATA()
	END_NEXTRECORDSET()

	// send
	SendPacket( (BYTE *)&sendMsg );
}

void DBGameProtocol::Event_MaterialComplete()
{
	// 받은 패킷
	sDBRequest_Event_MaterialComplete *pRecvMsg	= (sDBRequest_Event_MaterialComplete *)m_pPacket;

	// 보낼 패킷
	sDBResponse_Event_MaterialComplete sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_EVENT_MATERIALCOMPLETE;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->m_usCharID;

	BEGIN_SP( "rsp_XmasEventComplete" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->m_siPersonID, sizeof(pRecvMsg->m_siPersonID));
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->m_siVillageUnique, sizeof(pRecvMsg->m_siVillageUnique));
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->m_siConstructType, sizeof(pRecvMsg->m_siConstructType));
		for(SI32 i = 0; i < MAX_MATERIAL_NUMBER; i++)
		{
			m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->m_siItemUnique[i], sizeof(pRecvMsg->m_siItemUnique[i]));
			m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->m_siItemNum[i], sizeof(pRecvMsg->m_siItemNum[i]));
		}
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->m_siOutputPos, sizeof(pRecvMsg->m_siOutputPos));
		SetItemParam(&pRecvMsg->m_clItem);
		m_odbc.SetParam( SQL_TINYINT, &pRecvMsg->m_bCanPileSwitch, sizeof(pRecvMsg->m_bCanPileSwitch));
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult );
		m_odbc.GetData( &sendMsg.siPersonID );
		m_odbc.GetData( &sendMsg.m_siVillageUnique );
		m_odbc.GetData( &sendMsg.m_siConstructType );
		m_odbc.GetData( &sendMsg.siAddedNum );
		m_odbc.GetData( &sendMsg.siOutputPos );
		GetItemData( &sendMsg.clItem );
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );

	// 로그 작성
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "VillageUnique = %d, ConstructType = %d, ItemNum = %d, InvPos = %d"
			, sendMsg.m_siVillageUnique, sendMsg.m_siConstructType, sendMsg.siAddedNum, sendMsg.siOutputPos
			);
		sDBLogMsg_QueryString LogMsg( "Xmas Tree Complete", sendMsg.siPersonID, sendMsg.clItem.siUnique, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

void DBGameProtocol::Add_Village_Score(void)
{
	// 받은 패킷
	sDBRequest_Add_Village_Score* pRecvMsg	= (sDBRequest_Add_Village_Score*)m_pPacket;

	// 보낼 패킷
	sDBResponse_Add_Village_Score sendMsg;

	sendMsg.packetHeader.usCmd	= DBMSG_RESPONSE_ADD_VILLAGE_SCORE;
	sendMsg.packetHeader.usSize	= sizeof( sendMsg );

	sendMsg.m_siAddVillageScore = pRecvMsg->m_siAddVillageScore;
	
	BEGIN_SP( "rsp_VillageScoreAdd" )
	m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->m_siVillageUnique, sizeof(pRecvMsg->m_siVillageUnique));
	m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->m_siAddVillageScore, sizeof(pRecvMsg->m_siAddVillageScore));
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
	m_odbc.GetData( &sendMsg.siResult );
	m_odbc.GetData( &sendMsg.m_siVillageUnique );
	m_odbc.GetData( &sendMsg.m_siVillageScore );
	END_FETCHDATA()
	SendPacket( (BYTE *)&sendMsg );

}
