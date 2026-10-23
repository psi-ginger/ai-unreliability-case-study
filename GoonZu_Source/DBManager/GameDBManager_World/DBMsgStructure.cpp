#include "DBGameProtocol.h"
#include "DBMsg-Structure.h"


void DBGameProtocol::SetStrInfo()
{
	// 받은 패킷
	sDBRequest_SetStrInfo *pRecvMsg = (sDBRequest_SetStrInfo *)m_pPacket;
	
	switch (pRecvMsg->siStrType)
	{
	case RANKTYPE_CITYHALL:
		break;
	case RANKTYPE_BANK:
		SetBankInfo();
		break;
	case RANKTYPE_STOCK:
		break;
	case RANKTYPE_MARKET:
		break;
	case RANKTYPE_HOUSE:
		break;
	case RANKTYPE_HUNT:
		break;
	case RANKTYPE_POSTOFFICE:
		break;
	case RANKTYPE_HORSEMARKET:
		break;
	case RANKTYPE_REALESTATEMARKET:
		break;
	case RANKTYPE_SUMMONMARKET:
		break;
	case RANKTYPE_LAND:
		break;
	case RANKTYPE_FEAST:
		break;
	case RANKTYPE_SUMMONHEROMARKET:
		break;
	}
}

void DBGameProtocol::GetStrInfo()
{
	// 받은 패킷
	sDBRequest_GetStrInfo *pRecvMsg = (sDBRequest_GetStrInfo *)m_pPacket;
	
	switch (pRecvMsg->siStrType)
	{
	case RANKTYPE_BANK:
		GetBankStrInfo();
		break;
	case RANKTYPE_STOCK:
		GetStockStrInfo();
		break;
	case RANKTYPE_MARKET:
		GetMarketStrInfo();
		break;
	case RANKTYPE_HOUSE:
		GetHouseStrInfo();
		break;
	case RANKTYPE_CITYHALL:
		GetCityhallStrInfo();
		break;
	case RANKTYPE_HUNT:
		GetHuntStrInfo();
		break;
	case RANKTYPE_POSTOFFICE:
		GetPostOfficeStrInfo();
		break;
	case RANKTYPE_HORSEMARKET:
		GetHorseMarketStrInfo();
		break;
	case RANKTYPE_REALESTATEMARKET:
		GetRealEstateMarketStrInfo();
		break;
	case RANKTYPE_SUMMONMARKET:
		GetSummonMarketStrInfo();
		break;
	case RANKTYPE_LAND:
		GetLandStrInfo();
		break;
	case RANKTYPE_FEAST:
		GetFeastStrInfo();
		break;
	case RANKTYPE_SUMMONHEROMARKET:
		GetSummonHeroMarketStrInfo();
		break;
	case RANKTYPE_MINE:
		GetMineStrInfo();
		break;
	case RANKTYPE_GUILD:
		GetGuildStrInfo();
		break;
	case RANKTYPE_NEWMARKET:
		GetNewMarketInfo();
		break;
	}
}

void DBGameProtocol::GetStructureInfo()
{
	// 받은 패킷
	sDBRequest_GetStructureInfo *pRecvMsg = (sDBRequest_GetStructureInfo *)m_pPacket;
	
	// 보낼 패킷
	sDBResponse_GetStructureInfo sendMsg;
	
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_GETSTRUCTUREINFO;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->usCharID;
	
	// DB Query
	BEGIN_SP( "rsp_StructureInfoGet" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siVillageUnique, sizeof(pRecvMsg->siVillageUnique) );
	END_SP()
	
	EXEC_SQL()
	
	SI32 siStructureIndex;

	BEGIN_FETCHDATA()
		m_odbc.GetData( &siStructureIndex,										0);
		m_odbc.GetData( &sendMsg.siVillageStructureInfo[siStructureIndex],		0);
	END_FETCHDATA()
	
	sendMsg.siResult = 1;
	sendMsg.siVillageUnique = pRecvMsg->siVillageUnique;

	// send
	SendPacket( (BYTE *)&sendMsg );
}


void DBGameProtocol::PayFee()
{
	// 받은 패킷
	sDBRequest_PayFee *pRecvMsg = (sDBRequest_PayFee *)m_pPacket;
	
	// 보낼 패킷
	sDBResponse_PayFee sendMsg;
	
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_PAYFEE;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->usCharID;
	
	/*======================================
	siFeeType	:	FEETYPE_STOCK_SELL	200
					FEETYPE_STOCK_BUY	201
					FEETYPE_MARKET_SELL	300
	=======================================*/

	// DB Query
	BEGIN_SP( "rsp_StructureFeePay" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siVillageUnique, sizeof(pRecvMsg->siVillageUnique) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siStrType, sizeof(pRecvMsg->siStrType) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siFeeType, sizeof(pRecvMsg->siFeeType) );
		m_odbc.SetParam( SQL_BIGINT, &pRecvMsg->clFeeMoney.itMoney, sizeof(pRecvMsg->clFeeMoney.itMoney) );
	END_SP()
	
	EXEC_SQL()
	
	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0);
		m_odbc.GetData( &sendMsg.siPersonID, 0);
		m_odbc.GetData( &sendMsg.siVillageUnique, 0);
		m_odbc.GetData( &sendMsg.siStrType, 0);
		m_odbc.GetData( &sendMsg.siFeeType, 0);
		m_odbc.GetData( &sendMsg.clFeeMoney.itMoney, 0);
		m_odbc.GetData( &sendMsg.clPersonMoney.itMoney, 0);
		m_odbc.GetData( &sendMsg.clStructureMoney.itMoney, 0);
	END_FETCHDATA()
	
	sendMsg.siResult = 1;
	sendMsg.siVillageUnique = pRecvMsg->siVillageUnique;

	// send
	SendPacket( (BYTE *)&sendMsg );

	// 로그 작성
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "VillageUnique = %d, StrType = %d, FeeType = %d, FeeMoney = %I64d, LeftBankMoney = %I64d"
				, sendMsg.siVillageUnique, sendMsg.siStrType, sendMsg.siFeeType, sendMsg.clFeeMoney.itMoney, sendMsg.clPersonMoney.itMoney
				);
		sDBLogMsg_QueryString LogMsg("Pay fee", sendMsg.siPersonID, NULL, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

// 건물 보유자금을 증가시키거나 감소시킨다
void DBGameProtocol::ChangeStrMoney()
{
	// 받은 패킷
	sDBRequest_ChangeStrMoney *pRecvMsg = (sDBRequest_ChangeStrMoney *)m_pPacket;
	
	// 보낼 패킷
	sDBResponse_ChangeStrMoney sendMsg;
	
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_CHANGESTRMONEY;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
//	sendMsg.usCharID = pRecvMsg->usCharID;
	
	// DB Query
	BEGIN_SP( "rsp_StructureMoneyChange" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siReason, sizeof(pRecvMsg->siReason) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siVillageUnique, sizeof(pRecvMsg->siVillageUnique) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siStrType, sizeof(pRecvMsg->siStrType) );
		m_odbc.SetParam( SQL_BIGINT, &pRecvMsg->siMoney, sizeof(pRecvMsg->siMoney) );
	END_SP()
	
	EXEC_SQL()
	
	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0);
		m_odbc.GetData( &sendMsg.siReason, 0);
		m_odbc.GetData( &sendMsg.siVillageUnique, 0);
		m_odbc.GetData( &sendMsg.siStrType, 0);
		m_odbc.GetData( &sendMsg.clMoney.itMoney, 0);
	END_FETCHDATA()
	
	sendMsg.siReason = pRecvMsg->siReason;
	sendMsg.lParam = pRecvMsg->lParam;
	sendMsg.siChangeMoney = pRecvMsg->siMoney;

	// send
	SendPacket( (BYTE *)&sendMsg );

	// 로그 작성
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "VillageUnique = %d, StrType = %d, Reason = %d, ChangeMoney = %I64d, LeftMoney = %I64d"
				, sendMsg.siVillageUnique, sendMsg.siStrType, sendMsg.siReason, pRecvMsg->siMoney, sendMsg.clMoney.itMoney
				);
		sDBLogMsg_QueryString LogMsg("Change Structure Money", 0, NULL, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}


void DBGameProtocol::SetStructureMessage()
{
	// 받은 패킷
	sDBRequest_SetStructureMessage *pRecvMsg = (sDBRequest_SetStructureMessage *)m_pPacket;

	pRecvMsg->szMessage[ 254 ] = NULL;

	// DB Query
	BEGIN_SP( "rsp_StructureMessageSet" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siVillageUnique, sizeof(pRecvMsg->siVillageUnique) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siStrType, sizeof(pRecvMsg->siStrType) );
		m_odbc.SetParam( SQL_VARCHAR, pRecvMsg->szMessage, sizeof(pRecvMsg->szMessage) );
	END_SP()
	
	EXEC_SQL()
	
}

//마을 건물을 건설한다
void DBGameProtocol::BuildStructure()
{
	// 받은 패킷
	sDBRequest_BuildStructure *pRecvMsg = (sDBRequest_BuildStructure *)m_pPacket;
	
	// 보낼 패킷
	sDBResponse_BuildStructure sendMsg;
	
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_BUILDSTRUCTURE;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->usCharID;
	
	sendMsg.siType = pRecvMsg->siType;
	sendMsg.siNeedVillageScore = pRecvMsg->siNeedVillageScore;


	// DB Query
	BEGIN_SP( "rsp_StructureBuild" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siVillageUnique, sizeof(pRecvMsg->siVillageUnique) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siSlot, sizeof(pRecvMsg->siSlot) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siStructure, sizeof(pRecvMsg->siStructure) );
		m_odbc.SetParam( SQL_BIGINT, &pRecvMsg->siCost, sizeof(pRecvMsg->siCost) );
	END_SP()
	
	EXEC_SQL()
	
	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0);
		m_odbc.GetData( &sendMsg.siVillageUnique, 0);
		m_odbc.GetData( &sendMsg.siSlot, 0);
		m_odbc.GetData( &sendMsg.siStructure, 0);
		m_odbc.GetData( &sendMsg.siCost, 0);
		m_odbc.GetData( &sendMsg.clCityhallMoney.itMoney, 0);
	END_FETCHDATA()
	
	// send
	SendPacket( (BYTE *)&sendMsg );

	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "VillageUnique = %d, Slot = %d, Structure = %d, Cost = %I64d, LeftMoney = %I64d"
				, sendMsg.siVillageUnique, sendMsg.siSlot, sendMsg.siStructure, pRecvMsg->siCost, sendMsg.clCityhallMoney.itMoney
				);
		sDBLogMsg_QueryString LogMsg("Build Structure", 0, NULL, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

void DBGameProtocol::DestroyStructure()
{
	// 받은 패킷
	sDBRequest_DestroyStructure *pRecvMsg = (sDBRequest_DestroyStructure *)m_pPacket;
	
	// 보낼 패킷
	sDBResponse_DestroyStructure sendMsg;
	
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_DESTROYSTRUCTURE;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->usCharID;
	
	// DB Query
	BEGIN_SP( "rsp_StructureDestroy" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siVillageUnique, sizeof(pRecvMsg->siVillageUnique) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siSlot, sizeof(pRecvMsg->siSlot) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siStructure, sizeof(pRecvMsg->siStructure) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siRankType, sizeof(pRecvMsg->siRankType) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siLastDestroyDateVary, sizeof(pRecvMsg->siLastDestroyDateVary) );
	END_SP()
	
	EXEC_SQL()
	
	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0);
		m_odbc.GetData( &sendMsg.siVillageUnique, 0);
		m_odbc.GetData( &sendMsg.siSlot, 0);
		m_odbc.GetData( &sendMsg.siStructure, 0);
		m_odbc.GetData( &sendMsg.siChiefPersonID, 0);
		m_odbc.GetData( &sendMsg.siRankType, 0);
		m_odbc.GetData( &sendMsg.siLastDestroyDateVary, 0);
	END_FETCHDATA()
	
	// send
	SendPacket( (BYTE *)&sendMsg );

	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "VillageUnique = %d, Slot = %d, Structure = %d, ChiefPersonID = %d"
				, sendMsg.siVillageUnique, sendMsg.siSlot, sendMsg.siStructure, sendMsg.siChiefPersonID
				);
		sDBLogMsg_QueryString LogMsg("Destroy Structure", 0, NULL, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

void DBGameProtocol::ChiefInfo()
{
	// 받은 패킷
	sDBRequest_ChiefInfo *pRecvMsg = (sDBRequest_ChiefInfo *)m_pPacket;
	
	// 보낼 패킷
	sDBResponse_ChiefInfo sendMsg;
	
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_CHIEFINFO;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->usCharID;
	
	// DB Query
	BEGIN_SP( "rsp_StructureChiefInfo" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID) );
		SetSimpleRank( &pRecvMsg->SimpleRank );
	END_SP()
	
	EXEC_SQL()
	
	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0);
		m_odbc.GetData( &sendMsg.siPersonID, 0);
		GetSimplePerson( &sendMsg.Rank.clPerson );
		GetSimpleRank( &sendMsg.Rank.clRank );
		m_odbc.GetData( sendMsg.AccountID, sizeof(sendMsg.AccountID) );
	END_FETCHDATA()
	
	sendMsg.siResult = 1;
	sendMsg.siPersonID = pRecvMsg->siPersonID;
	sendMsg.Rank.clRank.Set(&pRecvMsg->SimpleRank);

	// send
	SendPacket( (BYTE *)&sendMsg );
}

void DBGameProtocol::SetStrRepairItem()
{
	// 받은 패킷
	sDBRequest_SetStrRepairItem *pRecvMsg = (sDBRequest_SetStrRepairItem *)m_pPacket;
	
	// 보낼 패킷
	sDBResponse_SetStrRepairItem sendMsg;
	
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_SETSTRREPAIRITEM;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->usCharID;
	
	// DB Query
	BEGIN_SP( "rsp_StructureSetStrRepairItem" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siVillageUnique, sizeof(pRecvMsg->siVillageUnique) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siRankType, sizeof(pRecvMsg->siRankType) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siItemUnique, sizeof(pRecvMsg->siItemUnique) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siItemNum, sizeof(pRecvMsg->siItemNum) );
	END_SP()
	
	EXEC_SQL()
	
	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siVillageUnique, 0 );
		m_odbc.GetData( &sendMsg.siRankType, 0 );
		m_odbc.GetData( &sendMsg.siItemUnique, 0 );
		m_odbc.GetData( &sendMsg.siItemNum, 0 );
	END_FETCHDATA()
	
	// send
	SendPacket( (BYTE *)&sendMsg );

	if(sendMsg.siVillageUnique > 0)
	{
		char log[512] = "";
		sprintf(log, "VillageUnique = %d, RankType = %d, ItemNum = %d"
				, sendMsg.siVillageUnique, sendMsg.siRankType, sendMsg.siItemNum
				);
		sDBLogMsg_QueryString LogMsg("Structure Set RepairItem", 0, NULL, sendMsg.siItemUnique, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

void DBGameProtocol::SetStrRepairAmount()
{
	// 받은 패킷
	sDBRequest_SetStrRepairAmount *pRecvMsg = (sDBRequest_SetStrRepairAmount *)m_pPacket;

	// 보낼 패킷
	sDBResponse_SetStrRepairAmount sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_SETSTRREPAIRAMOUNT;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->usCharID;

	// DB Query
	BEGIN_SP( "rsp_StructureSetStrRepairAmount" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siVillageUnique, sizeof(pRecvMsg->siVillageUnique) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siRankType, sizeof(pRecvMsg->siRankType) );
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siVillageUnique, 0 );
		m_odbc.GetData( &sendMsg.siRankType, 0 );
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );

	if(sendMsg.siVillageUnique > 0)
	{
		char log[512] = "";
		sprintf(log, "VillageUnique = %d, RankType = %d"
			, sendMsg.siVillageUnique, sendMsg.siRankType
			);
		sDBLogMsg_QueryString LogMsg("Structure Set RepairAmount", 0, NULL, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}


void DBGameProtocol::SetStrDur()
{
	// 받은 패킷
	sDBRequest_SetStrDura *pRecvMsg = (sDBRequest_SetStrDura *)m_pPacket;
	
	// 보낼 패킷
	sDBResponse_SetStrDura sendMsg;
	
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_SETSTRDURA;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->usCharID;
	
	// DB Query
	BEGIN_SP( "rsp_StructureSetStrDur" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siVillageUnique, sizeof(pRecvMsg->siVillageUnique) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siRankType, sizeof(pRecvMsg->siRankType) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siStrDura, sizeof(pRecvMsg->siStrDura) );
	END_SP()
	
	EXEC_SQL()
	
	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siVillageUnique, 0 );
		m_odbc.GetData( &sendMsg.siRankType, 0 );
		m_odbc.GetData( &sendMsg.siStrDura, 0 );
	END_FETCHDATA()
	
	// send
	SendPacket( (BYTE *)&sendMsg );

	if(sendMsg.siVillageUnique > 0)
	{
		char log[512] = "";
		sprintf(log, "VillageUnique = %d, RankType = %d, StrDur = %d"
				, sendMsg.siVillageUnique, sendMsg.siRankType, sendMsg.siStrDura
				);
		sDBLogMsg_QueryString LogMsg("Structure Set StrDur", 0, NULL, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

void DBGameProtocol::ChangeStrDur()
{
	// 받은 패킷
	sDBRequest_ChangeStrDura *pRecvMsg = (sDBRequest_ChangeStrDura *)m_pPacket;
	
	// 보낼 패킷
	sDBResponse_ChangeStrDura sendMsg;
	
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_CHANGESTRDURA;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->usCharID;
	
	// DB Query
	BEGIN_SP( "rsp_StructureChangeStrDur" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siVillageUnique, sizeof(pRecvMsg->siVillageUnique) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siRankType, sizeof(pRecvMsg->siRankType) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siChangeDura, sizeof(pRecvMsg->siChangeDura) );
	END_SP()
	
	EXEC_SQL()
	
	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siVillageUnique, 0 );
		m_odbc.GetData( &sendMsg.siRankType, 0 );
		m_odbc.GetData( &sendMsg.siStrDura, 0 );
	END_FETCHDATA()
	
	// send
	SendPacket( (BYTE *)&sendMsg );

	if(sendMsg.siVillageUnique > 0)
	{
		char log[512] = "";
		sprintf(log, "VillageUnique = %d, RankType = %d, ChangeDur = %d, StrDur = %d"
				, sendMsg.siVillageUnique, sendMsg.siRankType, pRecvMsg->siChangeDura, sendMsg.siStrDura
				);
		sDBLogMsg_QueryString LogMsg("Structure Change StrDur", 0, NULL, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

void DBGameProtocol::RepairStructure()
{
	// 받은 패킷
	sDBRequest_RepairStructure *pRecvMsg = (sDBRequest_RepairStructure *)m_pPacket;
	
	// 보낼 패킷
	sDBResponse_RepairStructure sendMsg;
	
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_REPAIRSTRUCTURE;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->usCharID;
	
	// DB Query
	BEGIN_SP( "rsp_StructureRepair" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siVillageUnique, sizeof(pRecvMsg->siVillageUnique) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siRankType, sizeof(pRecvMsg->siRankType) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siStrRepairAmount, sizeof(pRecvMsg->siStrRepairAmount) );
		m_odbc.SetParam( SQL_BIGINT, &pRecvMsg->siRepairMoney, sizeof(pRecvMsg->siRepairMoney) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siItemUnique, sizeof(pRecvMsg->siItemUnique) );
	END_SP()
	
	EXEC_SQL()
	
	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0 );
		m_odbc.GetData( &sendMsg.siVillageUnique, 0 );
		m_odbc.GetData( &sendMsg.siRankType, 0 );
		m_odbc.GetData( &sendMsg.siPersonID, 0 );
		m_odbc.GetData( &sendMsg.siStrRepairAmount, 0 );
		m_odbc.GetData( &sendMsg.siUpdateStrRepairAmount, 0 );
		m_odbc.GetData( &sendMsg.siLeftBaseMoney, 0 );
		m_odbc.GetData( &sendMsg.siItemUnique, 0 );
	END_FETCHDATA()
	
	BEGIN_NEXTRECORDSET()
		BEGIN_FETCHDATA()
			m_odbc.GetData( &sendMsg.siItemPos, 0 );
			GetItemData( &sendMsg.clItem );
		END_FETCHDATA()
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );

	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "VillageUnique = %d, RankType = %d, StrRepairAmount = %d, AfterRepairStrDur = %d, RepairMoney = %I64d, LeftBaseMoney = %I64d"
			, sendMsg.siVillageUnique, sendMsg.siRankType, sendMsg.siStrRepairAmount, sendMsg.siUpdateStrRepairAmount, pRecvMsg->siRepairMoney, sendMsg.siLeftBaseMoney
			);
		sDBLogMsg_QueryString LogMsg("Structure Repair", 0, NULL, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

void DBGameProtocol::ChangeStrBaseMoney()
{
	// 받은 패킷
	sDBRequest_ChangeStrBaseMoney *pRecvMsg = (sDBRequest_ChangeStrBaseMoney *)m_pPacket;

	// 보낼 패킷
	sDBResponse_ChangeStrBaseMoney sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_CHANGESTRBASEMONEY;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.siCharID = pRecvMsg->siCharID;

	// DB Query
	BEGIN_SP( "rsp_StructureBaseMoneyChange" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siVillageUnique, sizeof(pRecvMsg->siVillageUnique) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siStrType, sizeof(pRecvMsg->siStrType) );
		m_odbc.SetParam( SQL_BIGINT, &pRecvMsg->siMoney, sizeof(pRecvMsg->siMoney) );
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0 );
		m_odbc.GetData( &sendMsg.siVillageUnique, 0 );
		m_odbc.GetData( &sendMsg.siStrType, 0 );
		m_odbc.GetData( &sendMsg.clBaseMoney.itMoney, 0 );
		m_odbc.GetData( &sendMsg.siChangeMoney, 0 );
		m_odbc.GetData( &sendMsg.clCityhallMoney.itMoney, 0 );
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );

	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "VillageUnique = %d, StrType = %d, ChangeMoney = %I64d, LeftStrMoney = %I64d, LeftCityhallMoney = %I64d"
			, sendMsg.siVillageUnique, sendMsg.siStrType, sendMsg.siChangeMoney, sendMsg.clBaseMoney.itMoney, sendMsg.clCityhallMoney.itMoney
			);
		sDBLogMsg_QueryString LogMsg("Structure BaseMoney Change", 0, NULL, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}
void DBGameProtocol::ContributionItemListInput()
{
	// 받은 패킷
	sDBRequest_Contribution_Item_List_Input *pRecvMsg = (sDBRequest_Contribution_Item_List_Input *)m_pPacket;

	sDBResponse_Contribution_Item_List_Input sendMsg;
	sendMsg.m_packetHeader.usCmd	= DBMSG_RESPONSE_CONTRIBUTION_ITEM_LIST_INPUT;
	sendMsg.m_packetHeader.usSize	= sizeof(sendMsg);
	sendMsg.m_usCharID				= pRecvMsg->m_usCharID;
	sendMsg.m_siPersonID			= pRecvMsg->m_siPersonID;
	memcpy( sendMsg.m_siInputItemNum,		pRecvMsg->m_siInputItemNum,		sizeof(sendMsg.m_siInputItemNum) );
	memcpy( sendMsg.m_siInputItemIvenPos,	pRecvMsg->m_siInputItemIvenPos, sizeof(sendMsg.m_siInputItemIvenPos) );

	SI32 siCount = 0;
	SI32 siIndex[CONTRIBUTION_ITEM_NUM];
	BEGIN_SP( "rsp_ContributionItemListInput" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->m_siVillageUnique, sizeof(pRecvMsg->m_siVillageUnique) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->m_siNPCKind,		sizeof(pRecvMsg->m_siNPCKind)		);

		for( siCount = 0; siCount < CONTRIBUTION_ITEM_NUM; ++siCount )
		{
			siIndex[siCount] = siCount;
			m_odbc.SetParam( SQL_INTEGER,  &siIndex[siCount],						sizeof(siIndex[siCount])					);
			m_odbc.SetParam( SQL_INTEGER,  &pRecvMsg->m_siInputItemUnique[siCount],	sizeof(pRecvMsg->m_siInputItemUnique[siCount])	);
			m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->m_siInputItemNum[siCount],	sizeof(pRecvMsg->m_siInputItemNum[siCount])		);
		}
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.m_siResult );
	END_FETCHDATA()

	siCount = 0;
	BEGIN_NEXTRECORDSET()
		BEGIN_FETCHDATA()
			m_odbc.GetData( &sendMsg.m_siVillageUnique );
			m_odbc.GetData( &sendMsg.m_siNPCKind );

			if( CONTRIBUTION_ITEM_NUM <= siCount ) break;
			m_odbc.GetData( &siIndex[siCount] );
			m_odbc.GetData( &sendMsg.m_siItemUnique[siIndex[siCount]] );
			m_odbc.GetData( &sendMsg.m_siItemNum[siIndex[siCount]] );
			siCount++;
		END_FETCHDATA()
	END_NEXTRECORDSET()
	
	SendPacket( (BYTE *)&sendMsg );
}

void DBGameProtocol::ContributionItemListDelete()
{
	// 받은 패킷
	sDBRequest_Contribution_Item_List_Delete*	pRecvMsg = (sDBRequest_Contribution_Item_List_Delete*)m_pPacket;

	sDBRsponse_Contribution_Item_List_Delete sendMsg;
	sendMsg.m_packetHeader.usCmd	= DBMSG_RESPONSE_CONTRIBUTION_ITEM_LIST_DELETE;
	sendMsg.m_packetHeader.usSize	= sizeof(sendMsg);

	BEGIN_SP( "rsp_ContributionItemListDelete" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->m_siVillageUnique, sizeof(pRecvMsg->m_siVillageUnique) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->m_siNPCKind,		sizeof(pRecvMsg->m_siNPCKind)		);
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.m_siResult );
		m_odbc.GetData( &sendMsg.m_siVillageUnique );
		m_odbc.GetData( &sendMsg.m_siNPCKind );
	END_FETCHDATA()

	SendPacket( (BYTE *)&sendMsg );
}

void DBGameProtocol::ContributionMakeNPC()
{
	// 받은 패킷
	sDBRequest_Contribution_Make_NPC*	pRecvMsg = (sDBRequest_Contribution_Make_NPC*)m_pPacket;

	sDBResponse_Contribution_Make_NPC sendMsg;
	sendMsg.m_packetHeader.usCmd	= DBMSG_RESPONSE_CONTRIBUTION_MAKE_NPC;
	sendMsg.m_packetHeader.usSize	= sizeof(sendMsg);
	sendMsg.m_usCharID				= pRecvMsg->m_usCharID;
	sendMsg.m_siPersonID			= pRecvMsg->m_siPersonID;

	SI32 siCount = 0;
	SI32 siIndex[CONTRIBUTION_ITEM_NUM];
	BEGIN_SP( "rsp_ContributionMakeNPCCheck" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->m_siVillageUnique, sizeof(pRecvMsg->m_siVillageUnique) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->m_siNPCKind,		sizeof(pRecvMsg->m_siNPCKind)		);

		for( siCount = 0; siCount < CONTRIBUTION_ITEM_NUM; ++siCount )
		{
			siIndex[siCount] = siCount;
			m_odbc.SetParam( SQL_INTEGER,  &siIndex[siCount],					sizeof(siIndex[siCount]) );
			m_odbc.SetParam( SQL_INTEGER,  &pRecvMsg->m_siItemUnique[siCount],	sizeof(pRecvMsg->m_siItemUnique[siCount])	);
			m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->m_siItemNum[siCount],		sizeof(pRecvMsg->m_siItemNum[siCount])		);
		}
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.m_siResult );
		m_odbc.GetData( &sendMsg.m_siVillageUnique );
		m_odbc.GetData( &sendMsg.m_siNPCKind );
	END_FETCHDATA()
	
	SendPacket( (BYTE *)&sendMsg );
}

