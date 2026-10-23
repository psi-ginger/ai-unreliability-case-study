#include "DBGameProtocol.h"
#include "DBMsg-WorldMoney.h"

void DBGameProtocol::TradeServerSetOrder()
{
	// 받은 패킷
	sDBRequest_TradeServer_SetOrder *pRecvMsg = (sDBRequest_TradeServer_SetOrder *)m_pPacket;

	// 보낼 패킷
	sDBResponse_TradeServer_SetOrder sendMsg;
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_TRADESERVER_SETORDER;
	sendMsg.packetHeader.usSize = sizeof(sendMsg);
	sendMsg.siCharID = pRecvMsg->siCharId;

	// DB Query
	BEGIN_SP( "rsp_TradeServerOrderSet" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siPos, sizeof(pRecvMsg->siPos) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->clOrder.clitem.siUnique, sizeof(pRecvMsg->clOrder.clitem.siUnique) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->clOrder.clitem.siItemNum, sizeof(pRecvMsg->clOrder.clitem.siItemNum) );
		m_odbc.SetParam( SQL_BIGINT, &pRecvMsg->siWorldMoneyFee, sizeof(pRecvMsg->siWorldMoneyFee) );
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0 );
		m_odbc.GetData( &sendMsg.siPersonID, 0 );
		m_odbc.GetData( &sendMsg.siPos, 0 );
		GetItemData( &sendMsg.clLeftItem );
		m_odbc.GetData( &sendMsg.siLeftWorldMoney, 0 );
	END_FETCHDATA()

	sendMsg.clOrder.Set( &pRecvMsg->clOrder );

	// send
	SendPacket( (BYTE *)&sendMsg );

//	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "Result = %d, InvPos = %d, Amount = %d, LeftAmount = %d, Fee = %I64d, LeftAcu = %I64d"
			,sendMsg.siResult, pRecvMsg->siPos, pRecvMsg->clOrder.clitem.siItemNum, sendMsg.clLeftItem.siItemNum, pRecvMsg->siWorldMoneyFee, sendMsg.siLeftWorldMoney	
			);
		sDBLogMsg_QueryString LogMsg("TradeServer Sell Order", pRecvMsg->siPersonID, NULL, pRecvMsg->clOrder.clitem.siUnique, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

void DBGameProtocol::TradeServerCancelOrder()
{
	// 받은 패킷
	sDBRequest_TradeServer_CancelOrder *pRecvMsg = (sDBRequest_TradeServer_CancelOrder *)m_pPacket;

	// 보낼 패킷
	sDBResponse_TradeServer_CancelOrder sendMsg;
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_TRADESERVER_CANCELORDER;
	sendMsg.packetHeader.usSize = sizeof(sendMsg);
	sendMsg.siCharID = pRecvMsg->siCharId;

	// DB Query
	BEGIN_SP( "rsp_TradeServerOrderCancel" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siPos, sizeof(pRecvMsg->siPos) );
		SetItemParam( &pRecvMsg->clitem );
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0 );
		m_odbc.GetData( &sendMsg.siPersonID, 0 );
		m_odbc.GetData( &sendMsg.siPos, 0 );
		GetItemData( &sendMsg.clLeftItem );
	END_FETCHDATA()

	sendMsg.clOrder.Set( &pRecvMsg->clOrder );

	// send
	SendPacket( (BYTE *)&sendMsg );

//	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "Result = %d, InvPos = %d, Amount = %d, LeftAmount = %d"
			, sendMsg.siResult, pRecvMsg->siPos, pRecvMsg->clitem.siItemNum, sendMsg.clLeftItem.siItemNum
			);
		sDBLogMsg_QueryString LogMsg("TradeServer Cancel Sell", pRecvMsg->siPersonID, NULL, pRecvMsg->clOrder.clitem.siUnique, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

void DBGameProtocol::TradeServerBuyItem()
{
	// 받은 패킷
	sDBRequest_TradeServer_BuyItem *pRecvMsg = (sDBRequest_TradeServer_BuyItem *)m_pPacket;

	// 보낼 패킷
	sDBResponse_TradeServer_BuyItem sendMsg;
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_TRADESERVER_BUYITEM;
	sendMsg.packetHeader.usSize = sizeof(sendMsg);
	sendMsg.siCharID = pRecvMsg->siCharId;

	// DB Query
	BEGIN_SP( "rsp_TradeServerBuyItem" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siPos, sizeof(pRecvMsg->siPos) );
		SetItemParam( &pRecvMsg->clitem );
		m_odbc.SetParam( SQL_BIGINT, &pRecvMsg->clOrder.clitem.siPrice, sizeof(pRecvMsg->clOrder.clitem.siPrice) );
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0 );
		m_odbc.GetData( &sendMsg.siPersonID, 0 );
		m_odbc.GetData( &sendMsg.siPos, 0 );
		GetItemData( &sendMsg.clLeftItem );
		m_odbc.GetData( &sendMsg.siLeftWorldMoney, 0 );
	END_FETCHDATA()

	sendMsg.clOrder.Set( &pRecvMsg->clOrder );

	// send
	SendPacket( (BYTE *)&sendMsg );

//	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "Result = %d, InvPos = %d, Amount = %d, LeftAmount = %d, Price = %I64d, LeftAcu = %I64d"
			, sendMsg.siResult, pRecvMsg->siPos, pRecvMsg->clitem.siItemNum, sendMsg.clLeftItem.siItemNum, pRecvMsg->clOrder.clitem.siPrice, sendMsg.siLeftWorldMoney
			);
		sDBLogMsg_QueryString LogMsg("TradeServer Buy", pRecvMsg->siPersonID, NULL, sendMsg.clLeftItem.siUnique, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

void DBGameProtocol::TradeServerSellInfo()
{
	// 받은 패킷
	sDBRequest_TradeServer_Set_SellInfo *pRecvMsg = (sDBRequest_TradeServer_Set_SellInfo *)m_pPacket;

	// 보낼 패킷
	sDBResponse_TradeServer_Set_SellInfo sendMsg;
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_TRADESERVER_SET_SELLINFO;
	sendMsg.packetHeader.usSize = sizeof(sendMsg);
	//	sendMsg.siCharID = pRecvMsg->siCharId;

	//[추가 : 황진성 2007. 11. 2 주문정보 담기]
	sendMsg.clOrder.Set(&pRecvMsg->clOrder);

	// DB Query
	BEGIN_SP( "rsp_TradeServerSellInfo" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->clOrder.siPersonID, sizeof(pRecvMsg->clOrder.siPersonID) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->clOrder.clitem.siItemNum, sizeof(pRecvMsg->clOrder.clitem.siItemNum) );
		m_odbc.SetParam( SQL_BIGINT, &pRecvMsg->clOrder.clitem.siPrice, sizeof(pRecvMsg->clOrder.clitem.siPrice) );
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0 );
		m_odbc.GetData( &sendMsg.clOrder.siPersonID, 0 );
		m_odbc.GetData( &sendMsg.siLeftWorldMoney, 0 );
	END_FETCHDATA()

	sendMsg.clOrder.Set( &pRecvMsg->clOrder );

	MStrCpy(sendMsg.szBuyerServer,pRecvMsg->szBuyerServer,20);
	MStrCpy(sendMsg.szBuyerName,pRecvMsg->szBuyerName,20);
	MStrCpy(sendMsg.szBuyerAccountID,pRecvMsg->szBuyerAccountID,20);

	// send
	SendPacket( (BYTE *)&sendMsg );

//	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "Result = %d, Amount = %d, Price = %I64d, LeftAcu = %I64d"
			,sendMsg.siResult, pRecvMsg->clOrder.clitem.siItemNum, pRecvMsg->clOrder.clitem.siPrice, sendMsg.siLeftWorldMoney
			);
		sDBLogMsg_QueryString LogMsg("TradeServer Selled", pRecvMsg->clOrder.siPersonID, NULL, pRecvMsg->clOrder.clitem.siUnique, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

void DBGameProtocol::TradeServerGetWithdrawAcu()
{
	// 받은 패킷
	sDBRequest_TradeServer_Get_Withdraw_Acu *pRecvMsg = (sDBRequest_TradeServer_Get_Withdraw_Acu *)m_pPacket;

	// 보낼 패킷
	sDBResponse_TradeServer_Get_Withdraw_Acu sendMsg;
	sendMsg.m_packetHeader.usCmd	= DBMSG_RESPONSE_TRADESERVER_GET_WITHDRAW_ACU;
	sendMsg.m_packetHeader.usSize	= sizeof(sendMsg);
	sendMsg.m_siCharID				= pRecvMsg->m_siCharID;
	sendMsg.m_siLeftSavedAcu		= pRecvMsg->m_siLeftSavedAcu;
	sendMsg.m_siReservedNum			= pRecvMsg->m_siReservedNum;

	// DB Query
	BEGIN_SP( "rsp_WorldMoneyAdd" )
		m_odbc.SetParam( SQL_INTEGER,	&pRecvMsg->m_siPersonID,	sizeof(pRecvMsg->m_siPersonID) );
		m_odbc.SetParam( SQL_BIGINT,	&pRecvMsg->m_siWithdrawAcu,	sizeof(pRecvMsg->m_siWithdrawAcu) );
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.m_siResult );
		m_odbc.GetData( &sendMsg.m_siPersonID );
		m_odbc.GetData( &sendMsg.m_siWithdrawAcu );
		m_odbc.GetData( &sendMsg.m_siLeftAcu );
	END_FETCHDATA()

	// 메시지 전송
	SendPacket( (BYTE *)&sendMsg );
}
