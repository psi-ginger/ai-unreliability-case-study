#include "DBGameProtocol.h"
#include "DBMsg-Structure.h"
#include "DBMsg-Feast.h"

void DBGameProtocol::GetFeastStrInfo()
{

	// 받은 패킷
	sDBRequest_GetStrInfo *pRecvMsg = (sDBRequest_GetStrInfo *)m_pPacket;
	
	// 보낼 패킷
	sDBResponse_GetStrInfo sendMsg;
	sendMsg.clStrInfo.clCommonInfo.siStrType = RANKTYPE_FEAST;
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_GETSTRINFO;
	sendMsg.packetHeader.usSize = sendMsg.GetTotalSize();
	sendMsg.usCharID = pRecvMsg->usCharID;

	// DB Query
	BEGIN_SP( "rsp_FeastInfoGet" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siVillageUnique, sizeof(pRecvMsg->siVillageUnique) );
	END_SP()
	
	EXEC_SQL()

	/* 내자시 기본정보1 */
	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0);
		m_odbc.GetData( &sendMsg.siVillageUnique,	0);
		m_odbc.GetData( &sendMsg.clStrInfo.clCommonInfo.clMoney.itMoney,	0);
		m_odbc.GetData( sendMsg.clStrInfo.clCommonInfo.strNotice, sizeof(sendMsg.clStrInfo.clCommonInfo.strNotice) );
		m_odbc.GetData( &sendMsg.clStrInfo.clFeastStrInfo.clFeastInfo.siCost,	0);
		m_odbc.GetData( &sendMsg.clStrInfo.clFeastStrInfo.clFeastResvInfo.siPersonID,	0);
		m_odbc.GetData( sendMsg.clStrInfo.clFeastStrInfo.clFeastResvInfo.szPersonName,	sizeof(sendMsg.clStrInfo.clFeastStrInfo.clFeastResvInfo.szPersonName) );
		m_odbc.GetData( &sendMsg.clStrInfo.clFeastStrInfo.clFeastResvInfo.siYear,	0);
		m_odbc.GetData( &sendMsg.clStrInfo.clFeastStrInfo.clFeastResvInfo.siMonth,	0);
		m_odbc.GetData( &sendMsg.clStrInfo.clFeastStrInfo.clFeastResvInfo.siDay,	0);
		m_odbc.GetData( &sendMsg.clStrInfo.clFeastStrInfo.clFeastResvInfo.siHour,	0);
		m_odbc.GetData( &sendMsg.clStrInfo.clFeastStrInfo.clFeastResvInfo.siMinute,	0);
		m_odbc.GetData( sendMsg.clStrInfo.clFeastStrInfo.clFeastResvInfo.szReason,	sizeof(sendMsg.clStrInfo.clFeastStrInfo.clFeastResvInfo.szReason) );
		m_odbc.GetData( &sendMsg.clStrInfo.clFeastStrInfo.clFeastInfo.siStatus,	0);
	END_FETCHDATA()

	/* 내자시 선물정보 */
	BEGIN_NEXTRECORDSET()
		SI32 siIndex = 0;
		BEGIN_FETCHDATA()
			m_odbc.GetData( &siIndex, 0);	if(siIndex >= MAX_FEAST_PRESENT_NUMBER) continue;
			m_odbc.GetData( &sendMsg.clStrInfo.clFeastStrInfo.clFeastInfo.siItemUnique[siIndex],	0);
			m_odbc.GetData( &sendMsg.clStrInfo.clFeastStrInfo.clFeastInfo.siItemNum[siIndex],	0);
			m_odbc.GetData( &sendMsg.clStrInfo.clFeastStrInfo.clFeastInfo.siItemGive[siIndex],	0);
		END_FETCHDATA()
	END_NEXTRECORDSET()

	/* 내자시 창고정보 */
	BEGIN_NEXTRECORDSET()
		SI32 siIndex = 0;
		BEGIN_FETCHDATA()
			m_odbc.GetData( &siIndex, 0);	if(siIndex >= MAX_FEAST_PRESENT_NUMBER) continue;
			m_odbc.GetData( &sendMsg.clStrInfo.clFeastStrInfo.clFeastStg[siIndex].siItemUnique,	0);
			m_odbc.GetData( &sendMsg.clStrInfo.clFeastStrInfo.clFeastStg[siIndex].siItemNum,	0);
		END_FETCHDATA()
	END_NEXTRECORDSET()

	/* 건물 기본정보 */
	BEGIN_NEXTRECORDSET()
		BEGIN_FETCHDATA()
			m_odbc.GetData( &sendMsg.clStrInfo.clCommonInfo.siStrDura, 0 );
			m_odbc.GetData( &sendMsg.clStrInfo.clCommonInfo.siNeedItemUnique, 0 );
			m_odbc.GetData( &sendMsg.clStrInfo.clCommonInfo.siNeedItemNum, 0 );
			m_odbc.GetData( &sendMsg.clStrInfo.clCommonInfo.siUseItemNum, 0 );
			m_odbc.GetData( &(sendMsg.clStrInfo.clCommonInfo.clBaseMoney.itMoney), 0);
			m_odbc.GetData( &sendMsg.clStrInfo.clCommonInfo.siRepairStrDura, 0 );
		END_FETCHDATA()
	END_NEXTRECORDSET()
	
	// 세금정보를 얻는다.
	GetTaxList( &sendMsg.clStrInfo.clCommonInfo.clTaxInfo, sendMsg.siVillageUnique, sendMsg.clStrInfo.clCommonInfo.siStrType );


	// send
	SendPacket( (BYTE *)&sendMsg );
}

void DBGameProtocol::FeastReserve()
{
	// 받은 패킷
	sDBRequest_FeastReserve *pRecvMsg = (sDBRequest_FeastReserve *)m_pPacket;
	
	// 보낼 패킷
	sDBResponse_FeastReserve sendMsg;
	
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_FEAST_RESERVE;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->usCharID;
	
	// DB Query
	BEGIN_SP( "rsp_FeastReserve" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siVillageUnique, sizeof(pRecvMsg->siVillageUnique) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siYear, sizeof(pRecvMsg->siYear) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siMonth, sizeof(pRecvMsg->siMonth) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siDay, sizeof(pRecvMsg->siDay) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siHour, sizeof(pRecvMsg->siHour) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siMinute, sizeof(pRecvMsg->siMinute) );
		m_odbc.SetParam( SQL_VARCHAR, pRecvMsg->szReason, sizeof(pRecvMsg->szReason) );
		m_odbc.SetParam( SQL_BIGINT, &pRecvMsg->siCost, sizeof(pRecvMsg->siCost) );
	END_SP()
	
	EXEC_SQL()
	
	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0);
		m_odbc.GetData( &sendMsg.siVillageUnique, 0);
		m_odbc.GetData( &sendMsg.siPersonID, 0);
		m_odbc.GetData( sendMsg.szPersonName, sizeof(sendMsg.szPersonName));
		m_odbc.GetData( &sendMsg.siYear, 0);
		m_odbc.GetData( &sendMsg.siMonth, 0);
		m_odbc.GetData( &sendMsg.siDay, 0);
		m_odbc.GetData( &sendMsg.siHour, 0);
		m_odbc.GetData( &sendMsg.siMinute, 0);
		m_odbc.GetData( sendMsg.szReason, sizeof(sendMsg.szReason));
		m_odbc.GetData( &sendMsg.siCost, 0);
		m_odbc.GetData( &sendMsg.siPersonMoney, 0);
		m_odbc.GetData( &sendMsg.clFeastMoney.itMoney, 0);
	END_FETCHDATA()
	
	// send
	SendPacket( (BYTE *)&sendMsg );
	
	// 로그 작성
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "VillageUnique = %d, %d/%d/%d %d:%d, Cost = %I64d, PersonMoney = %I64d, StrMoney = %I64d, Reason = %s"
				, sendMsg.siVillageUnique, sendMsg.siYear, sendMsg.siMonth, sendMsg.siDay, sendMsg.siHour, sendMsg.siMinute
				, sendMsg.siCost, sendMsg.siPersonMoney, sendMsg.clFeastMoney.itMoney, sendMsg.szReason
				);
		sDBLogMsg_QueryString LogMsg("Feast Reserve", sendMsg.siPersonID, NULL, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

void DBGameProtocol::FeastAddPresentList()
{
	// 받은 패킷
	sDBRequest_FeastAddPresentList *pRecvMsg = (sDBRequest_FeastAddPresentList *)m_pPacket;
	
	// 보낼 패킷
	sDBResponse_FeastAddPresentList sendMsg;
	
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_FEAST_ADDPRESENTLIST;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->usCharID;
	
	// DB Query
	BEGIN_SP( "rsp_FeastAddPresentList" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siVillageUnique, sizeof(pRecvMsg->siVillageUnique) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siListIndex, sizeof(pRecvMsg->siListIndex) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siItemUnique, sizeof(pRecvMsg->siItemUnique) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siItemNum, sizeof(pRecvMsg->siItemNum) );
	END_SP()
	
	EXEC_SQL()
	
	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0);
		m_odbc.GetData( &sendMsg.siVillageUnique, 0);
		m_odbc.GetData( &sendMsg.siListIndex, 0);
		m_odbc.GetData( &sendMsg.siItemUnique, 0);
		m_odbc.GetData( &sendMsg.siItemNum, 0);
	END_FETCHDATA()
	
	// send
	SendPacket( (BYTE *)&sendMsg );
	
	// 로그 작성
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "VillageUnique = %d, ListIndex = %d, Amount = %d"
				, sendMsg.siVillageUnique, sendMsg.siListIndex, sendMsg.siItemNum
				);
		sDBLogMsg_QueryString LogMsg("Feast AddPresentList", NULL, NULL, sendMsg.siItemUnique, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

void DBGameProtocol::FeastDelPresentList()
{
	// 받은 패킷
	sDBRequest_FeastDelPresentList *pRecvMsg = (sDBRequest_FeastDelPresentList *)m_pPacket;
	
	// 보낼 패킷
	sDBResponse_FeastDelPresentList sendMsg;
	
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_FEAST_DELPRESENTLIST;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->usCharID;
	
	// DB Query
	BEGIN_SP( "rsp_FeastDelPresentList" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siVillageUnique, sizeof(pRecvMsg->siVillageUnique) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siListIndex, sizeof(pRecvMsg->siListIndex) );
	END_SP()
	
	EXEC_SQL()
	
	SI32 siDeletedItemUnique = 0;
	SI32 siDeletedItemNum = 0;
	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0);
		m_odbc.GetData( &sendMsg.siVillageUnique, 0);
		m_odbc.GetData( &sendMsg.siListIndex, 0);
		m_odbc.GetData( &siDeletedItemUnique, 0);
		m_odbc.GetData( &siDeletedItemNum, 0);
	END_FETCHDATA()
	
	// send
	SendPacket( (BYTE *)&sendMsg );
	
	// 로그 작성
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "VillageUnique = %d, ListIndex = %d, Amount = %d"
				, sendMsg.siVillageUnique, sendMsg.siListIndex, siDeletedItemNum
				);
		sDBLogMsg_QueryString LogMsg("Feast DelPresentList", NULL, NULL, siDeletedItemUnique, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

void DBGameProtocol::FeastStgItemIn()
{
	// 받은 패킷
	sDBRequest_FeastStgItemIn *pRecvMsg = (sDBRequest_FeastStgItemIn *)m_pPacket;
	             
	// 보낼 패킷
	sDBResponse_FeastStgItemIn sendMsg;
	
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_FEAST_STGITEMIN;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->usCharID;
	
	SI32 siMAX_ITEM_PILE_NUMBER = MAX_ITEM_PILE_NUMBER;

	// DB Query
	BEGIN_SP( "rsp_FeastStgItemIn" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siVillageUnique, sizeof(pRecvMsg->siVillageUnique) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siItemInventoryIndex, sizeof(pRecvMsg->siItemInventoryIndex) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siItemUnique, sizeof(pRecvMsg->siItemUnique) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siItemNum, sizeof(pRecvMsg->siItemNum) );
		m_odbc.SetParam( SQL_INTEGER, &siMAX_ITEM_PILE_NUMBER, sizeof(siMAX_ITEM_PILE_NUMBER) );
	END_SP()
	
	EXEC_SQL()
	
	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0);
		m_odbc.GetData( &sendMsg.siPersonID, 0);
		m_odbc.GetData( &sendMsg.siVillageUnique, 0);
		m_odbc.GetData( &sendMsg.siItemInventoryIndex, 0);
		m_odbc.GetData( &sendMsg.siItemUnique, 0);
		m_odbc.GetData( &sendMsg.siItemNum, 0);
		m_odbc.GetData( &sendMsg.siStgItemIndex, 0);
		m_odbc.GetData( &sendMsg.siStgItemUnique, 0);
		m_odbc.GetData( &sendMsg.siStgItemNum, 0);
	END_FETCHDATA()
	
	// send
	SendPacket( (BYTE *)&sendMsg );
	
	// 로그 작성
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "VillageUnique = %d, FromInvPos = %d, Amount = %d, ToStgIndex = %d, StgItemNum = %d"
				, sendMsg.siVillageUnique, pRecvMsg->siItemInventoryIndex, pRecvMsg->siItemNum, sendMsg.siStgItemIndex, sendMsg.siStgItemNum
				);
		sDBLogMsg_QueryString LogMsg("Feast StgItemIn", NULL, NULL, pRecvMsg->siItemUnique, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

void DBGameProtocol::FeastStgItemOut()
{
	// 받은 패킷
	sDBRequest_FeastStgItemOut *pRecvMsg = (sDBRequest_FeastStgItemOut *)m_pPacket;
	
	// 보낼 패킷
	sDBResponse_FeastStgItemOut sendMsg;
	
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_FEAST_STGITEMOUT;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->usCharID;
	
	// DB Query
	BEGIN_SP( "rsp_FeastStgItemOut" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siVillageUnique, sizeof(pRecvMsg->siVillageUnique) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siStgItemIndex, sizeof(pRecvMsg->siStgItemIndex) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siStgItemUnique, sizeof(pRecvMsg->siStgItemUnique) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siStgItemNum, sizeof(pRecvMsg->siStgItemNum) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siPersonInvIndex, sizeof(pRecvMsg->siPersonInvIndex) );
	END_SP()
	
	EXEC_SQL()
	
	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0);
		m_odbc.GetData( &sendMsg.siPersonID, 0);
		m_odbc.GetData( &sendMsg.siVillageUnique, 0);
		m_odbc.GetData( &sendMsg.siStgItemIndex, 0);
		m_odbc.GetData( &sendMsg.siStgItemUnique, 0);
		m_odbc.GetData( &sendMsg.siStgItemNum, 0);
		m_odbc.GetData( &sendMsg.siItemInventoryIndex, 0);
		m_odbc.GetData( &sendMsg.siItemUnique, 0);
		m_odbc.GetData( &sendMsg.siItemNum, 0);
	END_FETCHDATA()
	
	// send
	SendPacket( (BYTE *)&sendMsg );
	
	// 로그 작성
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "VillageUnique = %d, FromStgIndex = %d, Amount = %d, ToInvPos = %d, StgItemNum = %d"
				, sendMsg.siVillageUnique, sendMsg.siItemInventoryIndex, pRecvMsg->siStgItemNum, sendMsg.siItemInventoryIndex, sendMsg.siItemNum
				);
		sDBLogMsg_QueryString LogMsg("Feast StgItemOut", NULL, NULL, pRecvMsg->siStgItemUnique, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

void DBGameProtocol::FeastSetCost()
{
	// 받은 패킷
	sDBRequest_FeastSetCost *pRecvMsg = (sDBRequest_FeastSetCost *)m_pPacket;

	// 보낼 패킷
	sDBResponse_FeastSetCost sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_FEAST_SETCOST;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->usCharID;

	// DB Query
	BEGIN_SP( "rsp_FeastCostSet" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siVillageUnique, sizeof(pRecvMsg->siVillageUnique) );
		m_odbc.SetParam( SQL_BIGINT, &pRecvMsg->siCost, sizeof(pRecvMsg->siCost) );
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0);
		m_odbc.GetData( &sendMsg.siVillageUnique, 0);
		m_odbc.GetData( &sendMsg.siCost, 0);
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );

	// 로그 작성
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "VillageUnique = %d, Cost = %I64d"
			, sendMsg.siVillageUnique, sendMsg.siCost
			);
		sDBLogMsg_QueryString LogMsg("Feast Set Cost", NULL, NULL, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

void DBGameProtocol::FeastOpen()
{
	// 받은 패킷
	sDBRequest_FeastOpen *pRecvMsg = (sDBRequest_FeastOpen *)m_pPacket;

	// 보낼 패킷
	sDBResponse_FeastOpen sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_FEAST_OPEN;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
//	sendMsg.usCharID = pRecvMsg->usCharID;

	// DB Query
	BEGIN_SP( "rsp_FeastOpen" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siVillageUnique, sizeof(pRecvMsg->siVillageUnique) );
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0);
		m_odbc.GetData( &sendMsg.siVillageUnique, 0);
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
		sDBLogMsg_QueryString LogMsg("Feast Open", NULL, NULL, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

void DBGameProtocol::FeastClose()
{
	// 받은 패킷
	sDBRequest_FeastClose *pRecvMsg = (sDBRequest_FeastClose *)m_pPacket;

	// 보낼 패킷
	sDBResponse_FeastClose sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_FEAST_CLOSE;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
//	sendMsg.usCharID = pRecvMsg->usCharID;

	// DB Query
	BEGIN_SP( "rsp_FeastClose" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siVillageUnique, sizeof(pRecvMsg->siVillageUnique) );
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0);
		m_odbc.GetData( &sendMsg.siVillageUnique, 0);
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
		sDBLogMsg_QueryString LogMsg("Feast Close", NULL, NULL, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

void DBGameProtocol::FeastGiveItem()
{
	// 받은 패킷
	sDBRequest_FeastGiveItem *pRecvMsg = (sDBRequest_FeastGiveItem *)m_pPacket;

	// 보낼 패킷
	sDBResponse_FeastGiveItem sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_FEAST_GIVEITEM;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->usCharID;

	// DB Query
	BEGIN_SP( "rsp_FeastGiveItem" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siVillageUnique, sizeof(pRecvMsg->siVillageUnique) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPresentIndex, sizeof(pRecvMsg->siPresentIndex) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siItemUnique, sizeof(pRecvMsg->siItemUnique) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siItemNum, sizeof(pRecvMsg->siItemNum) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siStgIndex, sizeof(pRecvMsg->siStgIndex) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siStgItemUnique, sizeof(pRecvMsg->siStgItemUnique) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siStgItemNum, sizeof(pRecvMsg->siStgItemNum) );
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0);
		m_odbc.GetData( &sendMsg.siVillageUnique, 0);
		m_odbc.GetData( &sendMsg.siPresentIndex, 0);
		m_odbc.GetData( &sendMsg.siItemUnique, 0);
		m_odbc.GetData( &sendMsg.siItemNum, 0);
		m_odbc.GetData( &sendMsg.siStgIndex, 0);
		m_odbc.GetData( &sendMsg.siStgItemUnique, 0);
		m_odbc.GetData( &sendMsg.siStgItemNum, 0);
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );

	// 로그 작성
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "VillageUnique = %d, PresentIndex = %d, Amount = %d, StgIndex = %d, LeftStgItemNum = %d"
			, sendMsg.siVillageUnique, pRecvMsg->siPresentIndex, pRecvMsg->siItemNum, sendMsg.siStgIndex, sendMsg.siStgItemNum
			);
		sDBLogMsg_QueryString LogMsg("Feast GiveItem", NULL, NULL, pRecvMsg->siItemUnique, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

void DBGameProtocol::FeastUseEffect()
{
	// 받은 패킷
	sDBRequest_FeastUseEffect *pRecvMsg = (sDBRequest_FeastUseEffect *)m_pPacket;

	// 보낼 패킷
	sDBResponse_FeastUseEffect sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_FEAST_USEEFFECT;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->usCharID;

	// DB Query
	BEGIN_SP( "rsp_FeastUseEffect" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siVillageUnique, sizeof(pRecvMsg->siVillageUnique) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siPos, sizeof(pRecvMsg->siPos) );
		SetItemParam( &pRecvMsg->clitem );
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0 );
		m_odbc.GetData( &sendMsg.siVillageUnique, 0 );
		m_odbc.GetData( &sendMsg.siPos, 0 );
		GetItemData( &sendMsg.clitem );
	END_FETCHDATA()

	memcpy( sendMsg.siEffectNum, pRecvMsg->siEffectNum, sizeof(sendMsg.siEffectNum));

	// send
	SendPacket( (BYTE *)&sendMsg );

	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "Result = %d, Pos = %d, VIllageUnique = %d"
			, sendMsg.siResult, pRecvMsg->siPos, pRecvMsg->siVillageUnique
			);
		sDBLogMsg_QueryString LogMsg("Feast Use Effect", pRecvMsg->siPersonID, NULL, pRecvMsg->clitem.siUnique, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

void DBGameProtocol::PartyHallReservation()
{
	// 받은 패킷
	sDBRequest_PartyHall_Reservation *pRecvMsg = (sDBRequest_PartyHall_Reservation *)m_pPacket;

	// 보낼 패킷
	sDBResponse_PartyHall_Reservation sendMsg;

	sendMsg.m_packetHeader.usCmd = DBMSG_RESPONSE_PARTYHALL_RESERVATION;
	sendMsg.m_packetHeader.usSize = sizeof( sendMsg );
	sendMsg.m_usCharID  = pRecvMsg->m_usCharID;
	
	sendMsg.m_ResvMoney = pRecvMsg->m_ResvMoney;

	TIMESTAMP_STRUCT stTime;
	Convert_TIMESTAMP_from_SYSTEMTIME(&stTime, &pRecvMsg->m_SystemTime);

	// DB Query
	BEGIN_SP( "rsp_PartyHallReservation" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->m_siPersonID, sizeof(pRecvMsg->m_siPersonID) );
		m_odbc.SetParam( SQL_TIMESTAMP, &stTime, sizeof(stTime) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->m_siPartyHallIndex, sizeof(pRecvMsg->m_siPartyHallIndex) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->m_siDecorationKind, sizeof(pRecvMsg->m_siDecorationKind) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->m_siEntranceType, sizeof(pRecvMsg->m_siEntranceType) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->m_siValueType, sizeof(pRecvMsg->m_siValueType) );
		m_odbc.SetParam( SQL_BIGINT, &pRecvMsg->m_ResvMoney, sizeof(pRecvMsg->m_ResvMoney) );
		m_odbc.SetParam( SQL_VARCHAR, pRecvMsg->m_szReason, sizeof(pRecvMsg->m_szReason) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->m_siDecorationSlotNum, sizeof(pRecvMsg->m_siDecorationSlotNum) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->m_siVillageUnique, sizeof(pRecvMsg->m_siVillageUnique) );
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.m_siResult );
		m_odbc.GetData( &sendMsg.m_siPersonID );
		m_odbc.GetData( sendMsg.m_szPersonName, sizeof(sendMsg.m_szPersonName) );
		m_odbc.GetData( &stTime, sizeof(stTime) ); Convert_SYSTEMTIME_from_TIMESTAMP(&sendMsg.m_SystemTime, &stTime);
		m_odbc.GetData( &sendMsg.m_siPartyHallIndex );
		m_odbc.GetData( &sendMsg.m_siDecorationKind );
		m_odbc.GetData( &sendMsg.m_siEntranceType );
		m_odbc.GetData( &sendMsg.m_siValueType );
		m_odbc.GetData( &sendMsg.m_UserChangeMoney );
		m_odbc.GetData( sendMsg.m_szReason, sizeof(sendMsg.m_szReason) );
	END_FETCHDATA()

	sendMsg.m_siDecorationSlotNum = pRecvMsg->m_siDecorationSlotNum;
	sendMsg.m_siVillageUnique	  = pRecvMsg->m_siVillageUnique;

	// send
	SendPacket( (BYTE *)&sendMsg );
}

void DBGameProtocol::PartyHallAddDecoration()
{
	// 받은 패킷
	sDBRequest_PartyHall_Add_Decoration *pRecvMsg = (sDBRequest_PartyHall_Add_Decoration *)m_pPacket;

	// 보낼 패킷
	sDBResponse_PartyHall_Add_Decoration sendMsg;

	sendMsg.m_packetHeader.usCmd = DBMSG_RESPONSE_PARTYHALL_ADD_DECORATION;
	sendMsg.m_packetHeader.usSize = sizeof( sendMsg );
	sendMsg.m_usCharID = pRecvMsg->m_usCharID;

	SI32 siPos;

	// DB Query
	BEGIN_SP( "rsp_PartyHallDecorationAdd" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->m_siPersonID, sizeof(pRecvMsg->m_siPersonID) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->m_siVillageUnique, sizeof(pRecvMsg->m_siVillageUnique) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->m_siDecorationKind, sizeof(pRecvMsg->m_siDecorationKind) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->m_siDecorationNum, sizeof(pRecvMsg->m_siDecorationNum) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->m_siDecorationSlot, sizeof(pRecvMsg->m_siDecorationSlot) );
		for(SI32 i = 0; i < MAX_FEAST_STG_NUMBER; i++)
		{
			m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->m_DeleteItem[i].siItemUnique, sizeof(pRecvMsg->m_DeleteItem[i].siItemUnique) );
			m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->m_DeleteItem[i].siItemNum, sizeof(pRecvMsg->m_DeleteItem[i].siItemNum) );
		}
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.m_siResult );
		m_odbc.GetData( &sendMsg.m_siPersonID );
		m_odbc.GetData( &sendMsg.m_siVillageUnique );
		m_odbc.GetData( &sendMsg.m_siDecorationKind );
		m_odbc.GetData( &sendMsg.m_siDecorationNum );
		m_odbc.GetData( &sendMsg.m_siDecorationSlot );
	END_FETCHDATA()

	BEGIN_NEXTRECORDSET()
		BEGIN_FETCHDATA()
			m_odbc.GetData( &siPos );
			if(siPos >= MAX_FEAST_STG_NUMBER || siPos < 0)
			{
				continue;
			}
			m_odbc.GetData( &sendMsg.m_DeleteItem[siPos].siItemUnique );
			m_odbc.GetData( &sendMsg.m_DeleteItem[siPos].siItemNum );
		END_FETCHDATA()
	END_NEXTRECORDSET()

	// send
	SendPacket( (BYTE *)&sendMsg );
}

void DBGameProtocol::PartyHallAddPresent()
{
	// 받은 패킷
	sDBRequest_PartyHall_Add_Present *pRecvMsg = (sDBRequest_PartyHall_Add_Present *)m_pPacket;

	// 보낼 패킷
	sDBResponse_PartyHall_Add_Present sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_PARTYHALL_ADD_PRESENT;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->m_usCharID;

	sendMsg.m_siPresentItemInvenPos = pRecvMsg->m_siPresentItemInvenPos;

	TIMESTAMP_STRUCT stTime;

	Convert_TIMESTAMP_from_SYSTEMTIME(&stTime, &pRecvMsg->m_ResvTime);

	// DB Query
	BEGIN_SP( "rsp_PartyHallPresentAdd" )
		m_odbc.SetParam( SQL_TIMESTAMP, &stTime, sizeof(stTime) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->m_siPartyHallIndex, sizeof(pRecvMsg->m_siPartyHallIndex) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->m_siItemListIndex, sizeof(pRecvMsg->m_siItemListIndex) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->m_siItemUnique, sizeof(pRecvMsg->m_siItemUnique) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->m_siItemNum, sizeof(pRecvMsg->m_siItemNum) );
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult );
		m_odbc.GetData( &stTime, sizeof(stTime) ); Convert_SYSTEMTIME_from_TIMESTAMP(&sendMsg.m_SystemTime, &stTime);
		m_odbc.GetData( &sendMsg.m_siPartyHallIndex );
		m_odbc.GetData( &sendMsg.m_siItemListIndex );
		m_odbc.GetData( &sendMsg.m_siItemUnique );
		m_odbc.GetData( &sendMsg.m_siItemNum );
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );
}

void DBGameProtocol::PartyHallGiveItem()
{
	// 받은 패킷
	sDBRequest_PartyHall_GiveItem *pRecvMsg = (sDBRequest_PartyHall_GiveItem *)m_pPacket;

	// 보낼 패킷
	sDBResponse_PartyHall_GiveItem sendMsg;

	sendMsg.m_packetHeader.usCmd = DBMSG_RESPONSE_PARTYHALL_GIVEITEM;
	sendMsg.m_packetHeader.usSize = sizeof( sendMsg );
	sendMsg.m_usCharID = pRecvMsg->m_usCharID;

	TIMESTAMP_STRUCT stTime;

	Convert_TIMESTAMP_from_SYSTEMTIME(&stTime, &pRecvMsg->m_SystemTime);

	// DB Query
	BEGIN_SP( "rsp_PartyHallPresentGive" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->m_siPersonID, sizeof(pRecvMsg->m_siPersonID) );
		m_odbc.SetParam( SQL_TIMESTAMP, &stTime, sizeof(stTime) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->m_siPartyHallIndex, sizeof(pRecvMsg->m_siPartyHallIndex) );
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.m_siResult );
		m_odbc.GetData( &sendMsg.m_siPersonID );
		m_odbc.GetData( &stTime, sizeof(stTime) ); Convert_SYSTEMTIME_from_TIMESTAMP(&sendMsg.m_SystemTime, &stTime);
		m_odbc.GetData( &sendMsg.m_siPartyHallIndex );
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );
}

void DBGameProtocol::PartyHallReserveList()
{
	// 받은 패킷
	sDBRequest_PartyHall_ReserveList *pRecvMsg = (sDBRequest_PartyHall_ReserveList *)m_pPacket;

	// 보낼 패킷
	sDBResponse_PartyHall_ReserveList sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_PARTYHALL_RESERVELIST;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	//sendMsg.usCharID = pRecvMsg->usCharID;

	SI16 siPartyHallIndex = 0;
	SI16 siPresentListIndex = 0;

	TIMESTAMP_STRUCT stTime;

	Convert_TIMESTAMP_from_SYSTEMTIME(&stTime, &pRecvMsg->stCurrent);

	// DB Query
	BEGIN_SP( "rsp_PartyHallReserveList" )
		m_odbc.SetParam( SQL_TIMESTAMP, &stTime, sizeof(stTime) );
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData(&siPartyHallIndex);
		m_odbc.GetData(&stTime);
        if(siPartyHallIndex < 0 || siPartyHallIndex >= MAX_PARTYHALL_MAP || stTime.hour < 0 || stTime.hour >= MAX_PARTYHALL_RESERVATION_INFO_LIST)
		{
			continue;
		}
		Convert_SYSTEMTIME_from_TIMESTAMP(&sendMsg.clPartyHallResvInfo[siPartyHallIndex][stTime.hour].m_SystemTime, &stTime);
		m_odbc.GetData(&sendMsg.clPartyHallResvInfo[siPartyHallIndex][stTime.hour].m_siPersonID);
		m_odbc.GetData(sendMsg.clPartyHallResvInfo[siPartyHallIndex][stTime.hour].m_szPersonName, sizeof(sendMsg.clPartyHallResvInfo[siPartyHallIndex][stTime.hour].m_szPersonName));
		m_odbc.GetData(&sendMsg.clPartyHallResvInfo[siPartyHallIndex][stTime.hour].m_siDecorationKind);
		m_odbc.GetData(&sendMsg.clPartyHallResvInfo[siPartyHallIndex][stTime.hour].m_siEntranceType);
		m_odbc.GetData(&sendMsg.clPartyHallResvInfo[siPartyHallIndex][stTime.hour].m_siValueType);
		m_odbc.GetData(sendMsg.clPartyHallResvInfo[siPartyHallIndex][stTime.hour].m_szReason, sizeof(sendMsg.clPartyHallResvInfo[siPartyHallIndex][stTime.hour].m_szReason));
		m_odbc.GetData(&siPresentListIndex);
		if(siPresentListIndex < 0 || siPresentListIndex >= RESERVATIO_PRESENT_ITEM)
		{
			continue;
		}
		m_odbc.GetData(&sendMsg.clPartyHallResvInfo[siPartyHallIndex][stTime.hour].m_PresentItem[siPresentListIndex].m_siItemUnique);
		m_odbc.GetData(&sendMsg.clPartyHallResvInfo[siPartyHallIndex][stTime.hour].m_PresentItem[siPresentListIndex].m_siItemNum);
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );
}

void DBGameProtocol::PartyHallDecorationList()
{
	// 받은 패킷
	sDBRequest_PartyHall_Decoration_List *pRecvMsg = (sDBRequest_PartyHall_Decoration_List*)m_pPacket;

	// 보낼 패킷
	sDBResponse_PartyHall_Decoration_List sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_PARTYHALL_DECORATION_LIST;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );

	SI32 siVillageUnique;
	SI16 siDecorationSlot;

	// DB Query
	BEGIN_SP( "rsp_PartyHallDecorationList" )
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		siVillageUnique = siDecorationSlot = 0;
		m_odbc.GetData(&siVillageUnique);
		m_odbc.GetData(&siDecorationSlot);
		if(siVillageUnique >= MAX_VILLAGE_NUMBER || siDecorationSlot >= MAX_PARTYHALL_DECORATION_ITEM)
		{
			continue;
		}
		m_odbc.GetData(&sendMsg.clDecoration[siVillageUnique][siDecorationSlot].m_DacorationKind);
		m_odbc.GetData(&sendMsg.clDecoration[siVillageUnique][siDecorationSlot].m_DacorationNum);
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );
}
