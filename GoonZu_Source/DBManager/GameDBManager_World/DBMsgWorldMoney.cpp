#include "DBGameProtocol.h"
#include "DBMsg-WorldMoney.h"

void DBGameProtocol::WorldMoneyDistribution()
{
	// 받은 패킷
	sDBRequest_Distribution_WorldMoney *pRecvMsg = (sDBRequest_Distribution_WorldMoney *)m_pPacket;

	// 보낼 패킷
	sDBResponse_Distribution_WorldMoney sendMsg;
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_DISTRIBUTION_WORLDMONEY;
	sendMsg.packetHeader.usSize = sizeof(sendMsg);
	sendMsg.usCharID = pRecvMsg->usCharId;

	// DB Query
	BEGIN_SP( "rsp_WorldMoneyDistribution" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID) );
		m_odbc.SetParam( SQL_BIGINT, &pRecvMsg->siMoney, sizeof(pRecvMsg->siMoney) );
		m_odbc.SetParam( SQL_BIGINT, &pRecvMsg->siWorldMoney, sizeof(pRecvMsg->siWorldMoney) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siReason, sizeof(pRecvMsg->siReason) );
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0 );
		m_odbc.GetData( &sendMsg.siPersonID, 0 );
		m_odbc.GetData( &sendMsg.siMoney, 0 );
		m_odbc.GetData( &sendMsg.siLeftMoney, 0 );
		m_odbc.GetData( &sendMsg.siWorldMoney, 0 );
		m_odbc.GetData( &sendMsg.siLeftWorldMoney, 0 );
		m_odbc.GetData( &sendMsg.siReason, 0 );
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );

	// 로그
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "Reason = %d, UseMoney = %I64d, BuyWorldMoney = %I64d, LeftMoney = %I64d, LeftWorldMoney = %I64d"
			,	sendMsg.siReason, sendMsg.siMoney, sendMsg.siWorldMoney, sendMsg.siLeftMoney, sendMsg.siLeftWorldMoney
			);
		sDBLogMsg_QueryString LogMsg("WorldMoney Distribution", sendMsg.siPersonID, NULL, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

// 아큐 거래소 리스트를 요청한다.
void DBGameProtocol::WorldMoneyTradeList()
{
	// 받은 패킷
	sDBRequest_WorldMoney_TradeList *pRecvMsg = (sDBRequest_WorldMoney_TradeList *)m_pPacket;

	// 보낼 패킷
	sDBResponse_WorldMoney_TradeList sendMsg;
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_WORLDMONEY_TRADELIST;
	sendMsg.packetHeader.usSize = sizeof(sendMsg);
	//	sendMsg.usCharID = pRecvMsg->usCharId;

	// DB Query
	SI32 siMaxSend = MAX_WORLDMONEY_TRADELIST_PER_SEND;
	BEGIN_SP( "rsp_WorldMoneyTradeList" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPage, sizeof(pRecvMsg->siPage));
		m_odbc.SetParam( SQL_INTEGER, &siMaxSend, sizeof(siMaxSend));
	END_SP()

	EXEC_SQL()

	SI32 siMarketIndex = 0;
	SI32 siIndex = 0;
	BEGIN_FETCHDATA()
		m_odbc.GetData( &siMarketIndex, 0 );
		siIndex = siMarketIndex - (pRecvMsg->siPage * MAX_WORLDMONEY_TRADELIST_PER_SEND);
		if(siMarketIndex >= MAX_WORLDMONEY_TRADELIST_PER_SEND || siMarketIndex < 0) continue;
		m_odbc.GetData( &sendMsg.siPersonID[siMarketIndex], 0 );
		m_odbc.GetData( &sendMsg.siWorldMoney[siMarketIndex], 0 );
		m_odbc.GetData( &sendMsg.siPrice[siMarketIndex], 0 );
		m_odbc.GetData( &sendMsg.siDate[siMarketIndex], 0 );
	END_FETCHDATA()

	sendMsg.siResult = 1;
	sendMsg.siPage = pRecvMsg->siPage;

	// send
	SendPacket( (BYTE *)&sendMsg );
}

// 아큐 팔기를 요청한다.
void DBGameProtocol::WorldMoneySellOrder()
{
	// 받은 패킷
	sDBRequest_WorldMoney_SellOrder *pRecvMsg = (sDBRequest_WorldMoney_SellOrder *)m_pPacket;

	// 보낼 패킷
	sDBResponse_WorldMoney_SellOrder sendMsg;
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_WORLDMONEY_SELLORDER;
	sendMsg.packetHeader.usSize = sizeof(sendMsg);
	sendMsg.usCharId = pRecvMsg->usCharId;

	// PCK : 로그용 패킷 추가
	sendMsg.siFee = pRecvMsg->siFee;

	// DB Query
	BEGIN_SP( "rsp_WorldMoneySellOrder" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID));
		m_odbc.SetParam( SQL_BIGINT, &pRecvMsg->siWorldMoney, sizeof(pRecvMsg->siWorldMoney));
		m_odbc.SetParam( SQL_BIGINT, &pRecvMsg->siPrice, sizeof(pRecvMsg->siPrice));
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siDate, sizeof(pRecvMsg->siDate));
		m_odbc.SetParam( SQL_BIGINT, &pRecvMsg->siFee, sizeof(pRecvMsg->siFee));
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0 );
		m_odbc.GetData( &sendMsg.siPersonID, 0 );
		m_odbc.GetData( &sendMsg.siListIndex, 0 );
		m_odbc.GetData( &sendMsg.siWorldMoney, 0 );
		m_odbc.GetData( &sendMsg.siPrice, 0 );
		m_odbc.GetData( &sendMsg.siDate, 0 );
		m_odbc.GetData( &sendMsg.siLeftMoney, 0 );
		m_odbc.GetData( &sendMsg.siLeftWorldMoney, 0 );
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );

	// 로그
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "MarketIndex = %d, WorldMoney = %I64d, Price = %I64d, Date = %d, Fee = %I64d, LeftMoney = %I64d, LeftWorldMoney = %I64d"
			, sendMsg.siListIndex, sendMsg.siWorldMoney, sendMsg.siPrice, sendMsg.siDate, pRecvMsg->siFee, sendMsg.siLeftMoney, sendMsg.siLeftWorldMoney
			);
		sDBLogMsg_QueryString LogMsg("WorldMoney SellOrder", sendMsg.siPersonID, NULL, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}
void DBGameProtocol::WorldMoneyCancelOrder()
{
	// 받은 패킷
	sDBRequest_WorldMoney_CancelOrder *pRecvMsg = (sDBRequest_WorldMoney_CancelOrder *)m_pPacket;

	// 보낼 패킷
	sDBResponse_WorldMoney_CancelOrder sendMsg;
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_WORLDMONEY_CANCELORDER;
	sendMsg.packetHeader.usSize = sizeof(sendMsg);
	sendMsg.usCharId = pRecvMsg->usCharId;

	// DB Query
	BEGIN_SP( "rsp_WorldMoneyCancelOrder" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID));
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0 );
		m_odbc.GetData( &sendMsg.siPersonID, 0 );
		m_odbc.GetData( &sendMsg.siListIndex, 0 );
		m_odbc.GetData( &sendMsg.siLeftWorldMoney, 0 );
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );

	// 로그
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "MarketIndex = %d, LeftWorldMoney = %I64d"
			, sendMsg.siListIndex, sendMsg.siLeftWorldMoney
			);
		sDBLogMsg_QueryString LogMsg("WorldMoney CancelOrder", sendMsg.siPersonID, NULL, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}
void DBGameProtocol::WorldMoneyBuyOrder()
{
	// 받은 패킷
	sDBRequest_WorldMoney_BuyOrder *pRecvMsg = (sDBRequest_WorldMoney_BuyOrder *)m_pPacket;

	// 보낼 패킷
	sDBResponse_WorldMoney_BuyOrder sendMsg;
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_WORLDMONEY_BUYORDER;
	sendMsg.packetHeader.usSize = sizeof(sendMsg);
	sendMsg.usCharId = pRecvMsg->usCharId;

	// DB Query
	BEGIN_SP( "rsp_WorldMoneyBuyOrder" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID));
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siListIndex, sizeof(pRecvMsg->siListIndex));
		m_odbc.SetParam( SQL_BIGINT, &pRecvMsg->siWorldMoney, sizeof(pRecvMsg->siWorldMoney));
		m_odbc.SetParam( SQL_BIGINT, &pRecvMsg->siPrice, sizeof(pRecvMsg->siPrice));
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0 );
		m_odbc.GetData( &sendMsg.siPersonID, 0 );
		m_odbc.GetData( &sendMsg.siListIndex, 0 );
		m_odbc.GetData( &sendMsg.siWorldMoney, 0 );
		m_odbc.GetData( &sendMsg.siLeftListWorldMoney, 0 );
		m_odbc.GetData( &sendMsg.siPrice, 0 );
		m_odbc.GetData( &sendMsg.siLeftMoney, 0 );
		m_odbc.GetData( &sendMsg.siLeftWorldMoney, 0 );
		m_odbc.GetData( &sendMsg.siSellerPersonID, 0 );
		m_odbc.GetData( &sendMsg.siSellerBankMoney, 0 );
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );

	// 로그
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "MarketIndex = %d, WorldMoney = %I64d, LeftMarketWorldMoney = %I64d, Price = %I64d, LeftMoney = %I64d, LeftWorldMoney = %I64d, SellerBankMoney = %I64d"
			, sendMsg.siListIndex, sendMsg.siWorldMoney, sendMsg.siLeftListWorldMoney, sendMsg.siPrice, sendMsg.siLeftMoney, sendMsg.siLeftWorldMoney, sendMsg.siSellerBankMoney
			);
		sDBLogMsg_QueryString LogMsg("WorldMoney Buy", sendMsg.siPersonID, sendMsg.siSellerPersonID, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

void DBGameProtocol::WorldMoneySetAveragePrice()
{
	// 받은 패킷
	sDBRequest_WorldMoney_SetAveragePrice *pRecvMsg = (sDBRequest_WorldMoney_SetAveragePrice *)m_pPacket;

	// 보낼 패킷
	sDBResponse_WorldMoney_SetAveragePrice sendMsg;
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_WORLDMONEY_SETAVERAGEPRICE;
	sendMsg.packetHeader.usSize = sizeof(sendMsg);
	//	sendMsg.usCharId = pRecvMsg->usCharId;

	// DB Query
	BEGIN_SP( "rsp_WorldMoneySetAveragePrice" )
		m_odbc.SetParam( SQL_BIGINT, &pRecvMsg->siAverageWorldMoneyPrice, sizeof(pRecvMsg->siAverageWorldMoneyPrice));
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0 );
		m_odbc.GetData( &sendMsg.siAverageWorldMoneyPrice, 0 );
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );

	// 로그
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "AverageWorldMoneyPrice = %I64d"
			, sendMsg.siAverageWorldMoneyPrice
			);
		sDBLogMsg_QueryString LogMsg("WorldMoney AveragePrice", NULL, NULL, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

void DBGameProtocol::WorldMoneyReducePrice()
{
	// 받은 패킷
	sDBRequest_WorldMoney_ReducePrice *pRecvMsg = (sDBRequest_WorldMoney_ReducePrice *)m_pPacket;

	// 보낼 패킷
	sDBResponse_WorldMoney_ReducePrice sendMsg;
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_WORLDMONEY_REDUCEPRICE;
	sendMsg.packetHeader.usSize = sizeof(sendMsg);
	//	sendMsg.usCharId = pRecvMsg->usCharId;

	// DB Query
	BEGIN_SP( "rsp_WorldMoneyReducePrice" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siIndex, sizeof(pRecvMsg->siIndex));
		m_odbc.SetParam( SQL_BIGINT, &pRecvMsg->siPrice, sizeof(pRecvMsg->siPrice));
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siDate, sizeof(pRecvMsg->siDate));
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID));
		m_odbc.SetParam( SQL_BIGINT, &pRecvMsg->siWorldMoney, sizeof(pRecvMsg->siWorldMoney));
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0 );
		m_odbc.GetData( &sendMsg.siPersonID, 0 );
		m_odbc.GetData( &sendMsg.siIndex, 0 );
		m_odbc.GetData( &sendMsg.siPrice, 0 );
		m_odbc.GetData( &sendMsg.siDate, 0 );
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );

	// 로그
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "MarketIndex = %d, Price = %I64d, BeforePrice = %I64d"
			, pRecvMsg->siIndex, pRecvMsg->siPrice, pRecvMsg->siWorldMoney
			);
		sDBLogMsg_QueryString LogMsg("WorldMoney Reduce Price", pRecvMsg->siPersonID, NULL, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

void DBGameProtocol::WorldMoneySetChar()
{
	// 받은 패킷
	sDBRequest_WorldMoney_Set_Char *pRecvMsg = (sDBRequest_WorldMoney_Set_Char *)m_pPacket;

	// 보낼 패킷
	sDBResponse_WorldMoney_Set_Char sendMsg;
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_WORLDMONEY_SET_CHAR;
	sendMsg.packetHeader.usSize = sizeof(sendMsg);
	sendMsg.siCharID = pRecvMsg->siCharID;

	// DB Query
	BEGIN_SP( "rsp_WorldMoneySetChar" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID));
		m_odbc.SetParam( SQL_BIGINT, &pRecvMsg->siWorldMoney, sizeof(pRecvMsg->siWorldMoney));
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0 );
		m_odbc.GetData( &sendMsg.siPersonID, 0 );
		m_odbc.GetData( &sendMsg.siWorldMoney, 0 );
		m_odbc.GetData( &sendMsg.siLeftWorldMoney, 0 );
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );

	// 로그
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "AddWorldMoney = %I64d, LeftWorldMoney = %I64d"
			, sendMsg.siWorldMoney, sendMsg.siLeftWorldMoney
			);
		sDBLogMsg_QueryString LogMsg("WorldMoney Add", pRecvMsg->siPersonID, NULL, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

void DBGameProtocol::WorldPassportSet()
{
	// 받은 패킷
	sDBRequest_WorldPassport_Set* pRecvMsg = (sDBRequest_WorldPassport_Set*)m_pPacket;

	// 보낼 패킷
	sDBResponse_WorldPassport_Set sendMsg;
	sendMsg.packetHeader.usCmd	= DBMSG_RESPONSE_WORLDPASSPORT_SET;
	sendMsg.packetHeader.usSize	= sizeof(sendMsg);
	sendMsg.m_siCharID			= pRecvMsg->m_siCharID;

	// DB Query
	BEGIN_SP( "rsp_PassportSet" )
		m_odbc.SetParam( SQL_INTEGER,	&pRecvMsg->m_siPersonID,		sizeof(pRecvMsg->m_siPersonID));
		m_odbc.SetParam( SQL_VARCHAR,	pRecvMsg->m_szPassportName,		sizeof(pRecvMsg->m_szPassportName));
		m_odbc.SetParam( SQL_INTEGER,	&pRecvMsg->m_siIssueDateVary,	sizeof(pRecvMsg->m_siIssueDateVary));
		m_odbc.SetParam( SQL_INTEGER,	&pRecvMsg->m_siExpireDateVary,	sizeof(pRecvMsg->m_siExpireDateVary));
		m_odbc.SetParam( SQL_BIGINT,	&pRecvMsg->m_siPaidAcu,			sizeof(pRecvMsg->m_siPaidAcu));
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.m_siResult );
		m_odbc.GetData( &sendMsg.m_siPersonID );
		m_odbc.GetData( sendMsg.m_szPassportName, sizeof(sendMsg.m_szPassportName) );
		m_odbc.GetData( &sendMsg.m_siIssueDateVary );
		m_odbc.GetData( &sendMsg.m_siExpireDateVary );
		m_odbc.GetData( &sendMsg.m_siPaidAcu );
		m_odbc.GetData( &sendMsg.m_siLeftAcu );
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );

}

void DBGameProtocol::WorldPassportExtend()
{
	// 받은 패킷
	sDBRequest_WorldPassport_Extend* pRecvMsg = (sDBRequest_WorldPassport_Extend*)m_pPacket;

	// 보낼 패킷
	sDBResponse_WorldPassport_Extend sendMsg;
	sendMsg.packetHeader.usCmd	= DBMSG_RESPONSE_WORLDPASSPORT_EXTEND;
	sendMsg.packetHeader.usSize	= sizeof(sendMsg);
	sendMsg.m_siCharID			= pRecvMsg->m_siCharID;

	// DB Query
	BEGIN_SP( "rsp_PassportExtend" )
		m_odbc.SetParam( SQL_INTEGER,	&pRecvMsg->m_siPersonID,		sizeof(pRecvMsg->m_siPersonID));
		m_odbc.SetParam( SQL_INTEGER,	&pRecvMsg->m_siExpireDateVary,	sizeof(pRecvMsg->m_siExpireDateVary));
		m_odbc.SetParam( SQL_BIGINT,	&pRecvMsg->m_siPaidAcu,			sizeof(pRecvMsg->m_siPaidAcu));
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.m_siResult );
		m_odbc.GetData( &sendMsg.m_siPersonID );
		m_odbc.GetData( &sendMsg.m_siExpireDateVary );
		m_odbc.GetData( &sendMsg.m_siPaidAcu );
		m_odbc.GetData( &sendMsg.m_siLeftAcu );
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );

}