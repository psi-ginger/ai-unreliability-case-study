#include "DBGameProtocol.h"
#include "DBMsg-Structure.h"
#include "DBMsg-Stock.h"


void DBGameProtocol::GetStockStrInfo()
{
	// 받은 패킷
	sDBRequest_GetStrInfo *pRecvMsg = (sDBRequest_GetStrInfo *)m_pPacket;

	SendStockStrInfo(pRecvMsg->siVillageUnique, pRecvMsg->usCharID);

}

void DBGameProtocol::SendStockStrInfo(	SI32 siVillageUnique, UI16 usCharID )
{
	// 받은 패킷
	sDBRequest_GetStrInfo *pRecvMsg = (sDBRequest_GetStrInfo *)m_pPacket;

	// 보낼 패킷
	sDBResponse_GetStrInfo sendMsg;
	sendMsg.clStrInfo.clCommonInfo.siStrType = RANKTYPE_STOCK;
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_GETSTRINFO;
	sendMsg.packetHeader.usSize = sendMsg.GetTotalSize();
	sendMsg.usCharID = usCharID;
	

	// DB Query
	BEGIN_SP( "rsp_StockInfoGet" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siVillageUnique, sizeof(pRecvMsg->siVillageUnique) );
	END_SP()
	
	EXEC_SQL()
	
	BEGIN_FETCHDATA()
		GetFeeInfo( &sendMsg.clStrInfo.clStockStrInfo.clFee );
		m_odbc.GetData( &sendMsg.clStrInfo.clCommonInfo.clMoney.itMoney, 0);
		m_odbc.GetData( sendMsg.clStrInfo.clCommonInfo.strNotice, sizeof(sendMsg.clStrInfo.clCommonInfo.strNotice) );
	END_FETCHDATA()

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

	sendMsg.siResult = 1;
	sendMsg.siVillageUnique = pRecvMsg->siVillageUnique;

	// 세금정보를 얻는다.
	GetTaxList( &sendMsg.clStrInfo.clCommonInfo.clTaxInfo, sendMsg.siVillageUnique, sendMsg.clStrInfo.clCommonInfo.siStrType );
	
	// send
	SendPacket( (BYTE *)&sendMsg );	
}


void DBGameProtocol::InvestList()
{
	// 받은 패킷
	sDBRequest_InvestList *pRecvMsg	= (sDBRequest_InvestList *)m_pPacket;
	
	// 보낼 패킷
	sDBResponse_InvestList sendMsg;
	
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_INVESTLIST;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
//	sendMsg.usCharID = pRecvMsg->usCharID;
	
	SI32 siMaxNumber = MAX_INVEST_NUMBER;

	// DB Query
	BEGIN_SP( "rsp_StockInvestListGet" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siVillageUnique,	sizeof(pRecvMsg->siVillageUnique) );
		m_odbc.SetParam( SQL_INTEGER, &siMaxNumber,					sizeof(siMaxNumber) );
	END_SP()
	
	EXEC_SQL()
	
	SI32 siCount = 0;

	BEGIN_FETCHDATA()
		GetSimplePerson( &(sendMsg.clInvestList.clInvestInfo[siCount].clPerson) );
		m_odbc.GetData( &sendMsg.clInvestList.clInvestInfo[siCount].siStockNum,	0);
		siCount++;
	END_FETCHDATA()
	
	sendMsg.siResult = ( siCount > 0 ) ? 1: 0;
	sendMsg.clInvestList.siVillageUnique = pRecvMsg->siVillageUnique;
	
	// send
	SendPacket( (BYTE *)&sendMsg );
}



void DBGameProtocol::ManyStockPrice()
{
	// 받은 패킷
	sDBRequest_ManyStockPrice *pRecvMsg	= (sDBRequest_ManyStockPrice *)m_pPacket;
	
	// 보낼 패킷
	sDBResponse_ManyStockPrice sendMsg;
	
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_MANYSTOCKPRICE;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
//	sendMsg.usCharID = pRecvMsg->usCharID;

	// DB Query
	BEGIN_SP( "rsp_StockPriceManyGet" )
	END_SP()
	
	EXEC_SQL()
	
	SI32 siVillageUnique = 0;

	BEGIN_FETCHDATA()
		m_odbc.GetData( &siVillageUnique,								sizeof(siVillageUnique) );
		m_odbc.GetData( &sendMsg.siCurrentStockPrice[siVillageUnique],	sizeof(sendMsg.siCurrentStockPrice[siVillageUnique]) );
		m_odbc.GetData( &sendMsg.siAverageStockPrice[siVillageUnique],	sizeof(sendMsg.siAverageStockPrice[siVillageUnique]) );
	END_FETCHDATA()
	
	sendMsg.siResult = (siVillageUnique > 0) ? 1: 0;
	
	// send
	SendPacket( (BYTE *)&sendMsg );
}


// 주식주문을 취소한다.
void DBGameProtocol::CancelStockOrder()
{
	// 받은 패킷
	sDBRequest_CancelStockOrder *pRecvMsg = (sDBRequest_CancelStockOrder *)m_pPacket;
	
	// 보낼 패킷
	sDBResponse_CancelStockOrder sendMsg;
	
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_CANCELSTOCKORDER;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->usCharID;
	
	// DB Query
	BEGIN_SP( "rsp_StockOrderCancel" )
		m_odbc.SetParam( SQL_INTEGER, &(pRecvMsg->siPersonID), sizeof(pRecvMsg->siPersonID) );
	END_SP()
	
	EXEC_SQL()
	
	BEGIN_FETCHDATA()
		m_odbc.GetData(&sendMsg.siResult, 0);
	END_FETCHDATA()

	sendMsg.siPersonID = pRecvMsg->siPersonID;
	
	// send
	SendPacket( (BYTE *)&sendMsg );

	// 로그 작성
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, ""
				);
		sDBLogMsg_QueryString LogMsg("Stock Order Cancel", sendMsg.siPersonID, NULL, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}



void DBGameProtocol::PersonStockAccountGet()
{
	// 받은 패킷
	sDBRequest_PersonStockAccountGet *pRecvMsg = (sDBRequest_PersonStockAccountGet *)m_pPacket;
	
	// 보낼 패킷
	sDBResponse_PersonStockAccountGet sendMsg;
	
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_PERSONSTOCKACCOUNTGET;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->usCharID;
	
	// DB Query
	BEGIN_SP( "rsp_StockPersonAccountInfoGet" )
		m_odbc.SetParam( SQL_INTEGER, &(pRecvMsg->siPersonID), sizeof(pRecvMsg->siPersonID) );
	END_SP()

	EXEC_SQL()
	
	int i = 0;

	BEGIN_FETCHDATA()
		m_odbc.GetData( &(sendMsg.clStockAccount.clUnit[i].siVillageUnique), 0);
		m_odbc.GetData( &(sendMsg.clStockAccount.clUnit[i].siAmount), 0);
		i++;
	END_FETCHDATA()
	
	if(i>0)
		sendMsg.Result = 1;
	else
		sendMsg.Result = 0;
	
	sendMsg.siPersonID = pRecvMsg->siPersonID;
	sendMsg.clStockAccount.siPersonID = pRecvMsg->siPersonID;
	
	// send
	SendPacket( (BYTE *)&sendMsg );
}



void DBGameProtocol::OrderStock()
{
	// 받은 패킷
	sDBRequest_OrderStock *pRecvMsg = (sDBRequest_OrderStock *)m_pPacket;
	
	// 보낼 패킷
	sDBResponse_OrderStock sendMsg;
	
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_ORDERSTOCK;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->usCharID;
	
	memcpy(&(sendMsg.clOrder), &(pRecvMsg->clOrder), sizeof(cltTradeOrder));
	
	// DB Query
	BEGIN_SP( "rsp_StockOrderSet" )
		m_odbc.SetParam( SQL_INTEGER, &(pRecvMsg->siOrderMode),				sizeof(pRecvMsg->siOrderMode) );
		m_odbc.SetParam( SQL_INTEGER, &(pRecvMsg->clOrder.siVillageUnique),	sizeof(pRecvMsg->clOrder.siVillageUnique) );
		m_odbc.SetParam( SQL_INTEGER, &(pRecvMsg->clOrder.siPersonID),		sizeof(pRecvMsg->clOrder.siPersonID) );
		m_odbc.SetParam( SQL_BIGINT, &(pRecvMsg->clOrder.siPrice),			sizeof(pRecvMsg->clOrder.siPrice) );
		m_odbc.SetParam( SQL_INTEGER, &(pRecvMsg->clOrder.siAmount),		sizeof(pRecvMsg->clOrder.siAmount) );
		m_odbc.SetParam( SQL_INTEGER, &(pRecvMsg->clOrder.siDate),			sizeof(pRecvMsg->clOrder.siDate) );
	END_SP()
	
	EXEC_SQL()
	
	BEGIN_FETCHDATA()
		m_odbc.GetData( &(sendMsg.clOrder.siIndex), 0);
	END_FETCHDATA()
	
	if(sendMsg.clOrder.siIndex == 0)
		sendMsg.siResult = 0;
	else
		sendMsg.siResult = 1;
	
	sendMsg.siOrderMode		= pRecvMsg->siOrderMode;
	
	// send
	SendPacket( (BYTE *)&sendMsg );

	// 로그 작성
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "OrderMode = %d, VillageUnique = %d, Price = %I64d, Amount = %d"
				, pRecvMsg->siOrderMode, pRecvMsg->clOrder.siVillageUnique, pRecvMsg->clOrder.siPrice, pRecvMsg->clOrder.siAmount
				);
		sDBLogMsg_QueryString LogMsg("Stock Order", pRecvMsg->clOrder.siPersonID, NULL, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

void DBGameProtocol::TradeStock()
{
	// 받은 패킷
	sDBRequest_TradeStock *pRecvMsg = (sDBRequest_TradeStock *)m_pPacket;
	
	// 보낼 패킷
	sDBResponse_TradeStock sendMsg;
	
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_TRADESTOCK;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->usCharID;
	
	// DB Query
//	SI32	siTradeAmount	= 0;
	SI32	siMaxStockKind	= MAX_STOCK_KIND_IN_ACOUNT;
	GMONEY	siBankMoney, siDstBankMoney;
	
	BEGIN_SP( "rsp_StockTrade" )
		m_odbc.SetParam( SQL_INTEGER, &(pRecvMsg->siOrderMode),					sizeof(pRecvMsg->siOrderMode) );
		m_odbc.SetParam( SQL_INTEGER, &(pRecvMsg->siVillageUnique),				sizeof(pRecvMsg->siVillageUnique) );

		m_odbc.SetParam( SQL_SMALLINT, &(pRecvMsg->clOrgOrder.siIndex),			sizeof(pRecvMsg->clOrgOrder.siIndex) );
		m_odbc.SetParam( SQL_SMALLINT, &(pRecvMsg->clOrgOrder.siType),			sizeof(pRecvMsg->clOrgOrder.siType) );
		m_odbc.SetParam( SQL_INTEGER, &(pRecvMsg->clOrgOrder.siPersonID),		sizeof(pRecvMsg->clOrgOrder.siPersonID) );
		m_odbc.SetParam( SQL_BIGINT, &(pRecvMsg->clOrgOrder.siPrice),			sizeof(pRecvMsg->clOrgOrder.siPrice) );
		m_odbc.SetParam( SQL_INTEGER, &(pRecvMsg->clOrgOrder.siAmount),			sizeof(pRecvMsg->clOrgOrder.siAmount) );
		m_odbc.SetParam( SQL_INTEGER, &(pRecvMsg->clOrgOrder.siDate),			sizeof(pRecvMsg->clOrgOrder.siDate) );
		m_odbc.SetParam( SQL_INTEGER, &(pRecvMsg->clOrgOrder.siVillageUnique),	sizeof(pRecvMsg->clOrgOrder.siVillageUnique) );

		m_odbc.SetParam( SQL_INTEGER, &(pRecvMsg->siDstOrderIndex),				sizeof(pRecvMsg->siDstOrderIndex) );
		m_odbc.SetParam( SQL_BIGINT, &(pRecvMsg->siPrice),						sizeof(pRecvMsg->siPrice) );
		m_odbc.SetParam( SQL_INTEGER, &siMaxStockKind,							sizeof(siMaxStockKind) );
	END_SP()
	
	EXEC_SQL()
	
	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0);
		m_odbc.GetData( &sendMsg.siOrderMode, 0);

		m_odbc.GetData( &sendMsg.siVillageUnique, 0);
		m_odbc.GetData( &sendMsg.siTradePrice, 0);
		m_odbc.GetData( &sendMsg.siTradeAmount, 0);

		m_odbc.GetData( &sendMsg.siLeftAmount, 0);
		m_odbc.GetData( &siBankMoney, 0);	sendMsg.siBankMoney = siBankMoney;
		m_odbc.GetData( &sendMsg.siDstLeftAmount, 0);
		m_odbc.GetData( &siDstBankMoney, 0);	sendMsg.siDstBankMoney = siDstBankMoney;

		m_odbc.GetData( &sendMsg.clOrgOrder.siIndex, 0);
		m_odbc.GetData( &sendMsg.clOrgOrder.siType, 0);
		m_odbc.GetData( &sendMsg.clOrgOrder.siPersonID, 0);
		m_odbc.GetData( &sendMsg.clOrgOrder.siPrice, 0);
		m_odbc.GetData( &sendMsg.clOrgOrder.siAmount, 0);
		m_odbc.GetData( &sendMsg.clOrgOrder.siDate, 0);
		m_odbc.GetData( &sendMsg.clOrgOrder.siVillageUnique, 0);

		m_odbc.GetData( &sendMsg.clDstOrder.siIndex, 0);
		m_odbc.GetData( &sendMsg.clDstOrder.siType, 0);
		m_odbc.GetData( &sendMsg.clDstOrder.siPersonID, 0);
		m_odbc.GetData( &sendMsg.clDstOrder.siPrice, 0);
		m_odbc.GetData( &sendMsg.clDstOrder.siAmount, 0);
		m_odbc.GetData( &sendMsg.clDstOrder.siDate, 0);
		m_odbc.GetData( &sendMsg.clDstOrder.siVillageUnique, 0);
	END_FETCHDATA()
	
	// send
	SendPacket( (BYTE *)&sendMsg );
	
	// 로그 작성
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "OrderMode = %d, TradeVillage = %d, VillageUnique = %d, TradePrice = %I64d, TradeAmount = %d, LeftAmount = %d, DstLeftAmount = %d, BankMoney = %I64d, DstBankMoney = %I64d"
				, sendMsg.siOrderMode, sendMsg.siVillageUnique, sendMsg.clDstOrder.siVillageUnique, sendMsg.siTradePrice, sendMsg.siTradeAmount
				, sendMsg.siLeftAmount, sendMsg.siDstLeftAmount, siBankMoney, siDstBankMoney
				);
		sDBLogMsg_QueryString LogMsg("Stock Trade", sendMsg.clOrgOrder.siPersonID, sendMsg.clDstOrder.siPersonID, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}


	// 주식계좌정보를 보낸다.
	if( sendMsg.siTradeAmount > 0)
	{
		// 보낼 패킷
		sDBResponse_PersonStockAccountGet sendMsg;
		
		sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_PERSONSTOCKACCOUNTGET;
		sendMsg.packetHeader.usSize = sizeof( sendMsg );
		sendMsg.usCharID = pRecvMsg->usCharID;
		
		// DB Query
		BEGIN_SP( "rsp_StockPersonAccountInfoGet" )
			m_odbc.SetParam( SQL_INTEGER, &(pRecvMsg->clOrgOrder.siPersonID), sizeof(pRecvMsg->clOrgOrder.siPersonID) );
		END_SP()

		EXEC_SQL()
		
		int i = 0;

		BEGIN_FETCHDATA()
			m_odbc.GetData( &(sendMsg.clStockAccount.clUnit[i].siVillageUnique), 0);
			m_odbc.GetData( &(sendMsg.clStockAccount.clUnit[i].siAmount), 0);
			i++;
		END_FETCHDATA()
		
		if(i>0)
			sendMsg.Result = 1;
		else
			sendMsg.Result = 0;
		
		sendMsg.siPersonID = pRecvMsg->clOrgOrder.siPersonID;
		sendMsg.clStockAccount.siPersonID = pRecvMsg->clOrgOrder.siPersonID;
		
		// send
		SendPacket( (BYTE *)&sendMsg );
	}
}


void DBGameProtocol::GetManyStockOrder()
{
	// 받은 패킷
	sDBRequest_GetManyStockOrder *pRecvMsg = (sDBRequest_GetManyStockOrder *)m_pPacket;
	
	// 보낼 패킷
	sDBResponse_GetManyStockOrder sendMsg;
	
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_GETMANYSTOCKORDER;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->usCharID;
	
	// DB Query
	SI32	siHowMany	= GET_MANY_STOCK_ORDER_NUM;
	
	BEGIN_SP( "rsp_StockOrderManyGet" )
		m_odbc.SetParam( SQL_INTEGER, &(pRecvMsg->siOrderMode),		sizeof(pRecvMsg->siOrderMode) );
		m_odbc.SetParam( SQL_INTEGER, &(pRecvMsg->siStartIndex),	sizeof(pRecvMsg->siStartIndex) );
		m_odbc.SetParam( SQL_INTEGER, &siHowMany,					sizeof(siHowMany) );
	END_SP()
	
	EXEC_SQL()
	
	SI32	i = 0;
	SI16	siIndex = 0;

	BEGIN_FETCHDATA()
		m_odbc.GetData( &(siIndex), 0);	sendMsg.clTradeOrder[i].siIndex = siIndex;
		sendMsg.clTradeOrder[i].siType = TRADEORDER_TYPE_STOCK;
		m_odbc.GetData( &(sendMsg.clTradeOrder[i].siPersonID), 0);
		m_odbc.GetData( &(sendMsg.clTradeOrder[i].siPrice), 0);
		m_odbc.GetData( &(sendMsg.clTradeOrder[i].siAmount), 0);
		m_odbc.GetData( &(sendMsg.clTradeOrder[i].siVillageUnique), 0);
		m_odbc.GetData( &(sendMsg.clTradeOrder[i].siDate), 0);
		i++;
	END_FETCHDATA()
	
	if(i > 0)
		sendMsg.siResult = 1;
	else
		sendMsg.siResult = 0;
	
	sendMsg.siOrderMode		= pRecvMsg->siOrderMode;
	sendMsg.siStartIndex	= pRecvMsg->siStartIndex;
	
	// send
	SendPacket( (BYTE *)&sendMsg );
}

void DBGameProtocol::SetStockFee()
{
	// 받은 패킷
	sDBRequest_SetStockFee *pRecvMsg	= (sDBRequest_SetStockFee *)m_pPacket;
	
	// 보낼 패킷
	sDBResponse_SetStockFee sendMsg;
	
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_SETSTOCKFEE;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->usCharID;
	
	cltDBFee clDBFee( &pRecvMsg->clFee );
	// DB Query
	BEGIN_SP( "rsp_StockFeeSet" )
		m_odbc.SetParam( SQL_INTEGER, &(pRecvMsg->siPersonID),		sizeof(pRecvMsg->siPersonID) );
		m_odbc.SetParam( SQL_INTEGER, &(pRecvMsg->siVillageUnique),	sizeof(pRecvMsg->siVillageUnique) );
		SetFeeInfo( &clDBFee );
	END_SP()

	EXEC_SQL()
	
	BEGIN_FETCHDATA()
		m_odbc.GetData( &(sendMsg.siResult), 0 );
		m_odbc.GetData( &(sendMsg.siPersonID), 0 );
		m_odbc.GetData( &(sendMsg.siVillageUnique), 0 );
		GetFeeInfo( &sendMsg.clFee );
	END_FETCHDATA()
	
	// send
	SendPacket( (BYTE *)&sendMsg );
}

void DBGameProtocol::SetStockBidVillage()
{
	// 받은 패킷
	sDBRequest_SetStockBidVillage *pRecvMsg	= (sDBRequest_SetStockBidVillage *)m_pPacket;
	
	// 보낼 패킷
	sDBResponse_SetStockBidVillage sendMsg;
	
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_SETSTOCKBIDVILLAGE;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->usCharID;
	
	cltGameDate clDate( &pRecvMsg->clDate );

	// DB Query
	BEGIN_SP( "rsp_StockBidVillage" )
		m_odbc.SetParam( SQL_INTEGER, &(pRecvMsg->siVillageUnique),	sizeof(pRecvMsg->siVillageUnique) );
		m_odbc.SetParam( SQL_INTEGER, &(pRecvMsg->siStockAmount),	sizeof(pRecvMsg->siStockAmount) );
		SetGameDate( &clDate );
		m_odbc.SetParam( SQL_BIGINT, &(pRecvMsg->siBeginningPrice),	sizeof(pRecvMsg->siBeginningPrice) );
	END_SP()

	EXEC_SQL()
	
	BEGIN_FETCHDATA()
		m_odbc.GetData( &(sendMsg.siResult), 0 );
		m_odbc.GetData( &(sendMsg.siVillageUnique), 0 );
		m_odbc.GetData( &(sendMsg.siStockAmount), 0 );
		GetGameDate( &sendMsg.clDate );
		m_odbc.GetData( &(sendMsg.siBeginningPrice), 0 );
	END_FETCHDATA()
	
	// send
	SendPacket( (BYTE *)&sendMsg );
}


void DBGameProtocol::GetStockBidVillageList()
{
	// 보낼 패킷
	sDBResponse_GetStockBidVillageList sendMsg;
	
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_GETSTOCKBIDVILLAGELIST;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
//	sendMsg.usCharID = pRecvMsg->usCharID;

	// DB Query
	BEGIN_SP( "rsp_StockBidVillageListGet" )
	END_SP()

	EXEC_SQL()

	sendMsg.usCount = 0;

	BEGIN_FETCHDATA()
		GetStockBidVillageInfo(&sendMsg.clStockBidVillageInfo[sendMsg.usCount]);
//		m_odbc.GetData( &(sendMsg.clStockBidVillageInfo[sendMsg.usCount].siVillageUnique), 0 );
//		m_odbc.GetData( &(sendMsg.clStockBidVillageInfo[sendMsg.usCount].siStockAmount), 0 );
//		GetGameDate( &sendMsg.clStockBidVillageInfo[sendMsg.usCount].clBiddingDate );
//		m_odbc.GetData( &(sendMsg.clStockBidVillageInfo[sendMsg.usCount].siBeginningPrice), 0 );
		sendMsg.usCount++;
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );
}


void DBGameProtocol::GetStockBidderList()
{
	// 받은 패킷
	sDBRequest_GetStockBidderList *pRecvMsg	= (sDBRequest_GetStockBidderList *)m_pPacket;

	// 보낼 패킷
	sDBResponse_GetStockBidderList sendMsg;
	
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_GETSTOCKBIDDERLIST;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->usCharID;
	
	SendStockBidderList( pRecvMsg->siVillageUnique );
}

void DBGameProtocol::SendStockBidderList(SI32 siVillageUnique)
{
	// 받은 패킷
	sDBRequest_GetStockBidderList *pRecvMsg	= (sDBRequest_GetStockBidderList *)m_pPacket;

	// 보낼 패킷
	sDBResponse_GetStockBidderList sendMsg;
	
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_GETSTOCKBIDDERLIST;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->usCharID;
	
	// DB Query
	BEGIN_SP( "rsp_StockBidderListGet" )
		m_odbc.SetParam( SQL_INTEGER, &siVillageUnique, sizeof(siVillageUnique) );
	END_SP()

	EXEC_SQL()

	sendMsg.usCount = 0;
	BEGIN_FETCHDATA()
		m_odbc.GetData( &(sendMsg.siVillageUnique), 0 );
		GetStockBidder( &sendMsg.clStockBidder[sendMsg.usCount] );
		sendMsg.usCount++;
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );
}


void DBGameProtocol::CompleteStockBidVillage()
{
	// 받은 패킷
	sDBRequest_CompleteStockBidVillage *pRecvMsg	= (sDBRequest_CompleteStockBidVillage *)m_pPacket;

	// 보낼 패킷
	sDBResponse_CompleteStockBidVillage sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_COMPLETESTOCKBIDVILLAGE;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->usCharID;

	SI32 siTotalBidAmount = 0;
	SI32 siLeftBidAmount = 0;
	GMONEY siBidPrice = 0;
	SI32 siBidDate = 0;

	// DB Query
	BEGIN_SP( "rsp_StockBidVillageComplete" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siVillageUnique, sizeof(pRecvMsg->siVillageUnique) );
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0 );
		m_odbc.GetData( &sendMsg.siVillageUnique, 0 );
		m_odbc.GetData( &siTotalBidAmount, 0 );
		m_odbc.GetData( &siLeftBidAmount, 0 );
		m_odbc.GetData( &siBidPrice, 0 );
		m_odbc.GetData( &siBidDate, 0 );
	END_FETCHDATA()

	// 로그 작성
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "VillageUnique = %d, TotalBidAmount = %d, LeftBidAmount = %d, Price = %I64d, Date = %d"
			, sendMsg.siVillageUnique, siTotalBidAmount, siLeftBidAmount, siBidPrice, siBidDate
			);
		sDBLogMsg_QueryString LogMsg("Stock Bid Complete Village", NULL, NULL, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}

	// send
	SendPacket( (BYTE *)&sendMsg );
}

void DBGameProtocol::SetBeginningPrice()
{
	// 받은 패킷
	sDBRequest_SetBeginningPrice *pRecvMsg	= (sDBRequest_SetBeginningPrice *)m_pPacket;

	// DB Query
	BEGIN_SP( "rsp_BeginningPrice" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siVillageUnique, sizeof(pRecvMsg->siVillageUnique) );
		m_odbc.SetParam( SQL_BIGINT, &pRecvMsg->siBeginningPrice, sizeof(pRecvMsg->siBeginningPrice) );
	END_SP()

	EXEC_SQL()
}

// 공모주식을 구입한다.
void DBGameProtocol::StockDistributionOrder()
{
	// 받은 패킷
	sDBRequest_StockDistributionOrder *pRecvMsg	= (sDBRequest_StockDistributionOrder *)m_pPacket;

	// 보낼 패킷
	sDBResponse_StockDistributionOrder sendMsg;
	
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_STOCKDISTRIBUTIONORDER;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.siCharID = pRecvMsg->siCharID;
	
	GMONEY itBeforeMoney = 0;
	UI08 uiWarType = pRecvMsg->bVillageWarStockDistribution;

	// DB Query
	BEGIN_SP( "rsp_StockDistributionOrder" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siVillageUnique, sizeof(pRecvMsg->siVillageUnique) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siStockDistributionNum, sizeof(pRecvMsg->siStockDistributionNum) );
		m_odbc.SetParam( SQL_BIGINT, &pRecvMsg->siStockPrice, sizeof(pRecvMsg->siStockPrice) );
		m_odbc.SetParam( SQL_TINYINT, &uiWarType, sizeof(uiWarType) );
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0 );
		m_odbc.GetData( &sendMsg.siVillageUnique, 0 );
		m_odbc.GetData( &sendMsg.siPersonID, 0 );
		m_odbc.GetData( &sendMsg.siStockDistributionNum, 0 );
		m_odbc.GetData( &sendMsg.siStockPrice, 0 );
		m_odbc.GetData( &sendMsg.clPersonMoney.itMoney, 0 );
		m_odbc.GetData( &itBeforeMoney, 0 );
	END_FETCHDATA()

	sendMsg.bVillageWarStockDistribution = pRecvMsg->bVillageWarStockDistribution;

	// send
	SendPacket( (BYTE *)&sendMsg );

	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "WarType = %d, VillageUnique = %d, StockNum = %d, Price = %I64d, Money = %I64d, BeforeMoney = %I64d"
				, pRecvMsg->bVillageWarStockDistribution, sendMsg.siVillageUnique, sendMsg.siStockDistributionNum
				, sendMsg.siStockPrice, sendMsg.clPersonMoney.itMoney, itBeforeMoney
				);
		sDBLogMsg_QueryString LogMsg("Stock Distribution", sendMsg.siPersonID, NULL, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);

	}
}

void DBGameProtocol::MaxStockDistributionNum()
{
	// 받은 패킷
	sDBRequest_MaxStockDistributionNum *pRecvMsg	= (sDBRequest_MaxStockDistributionNum *)m_pPacket;

	// 보낼 패킷
	sDBResponse_MaxStockDistributionNum sendMsg;
	
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_MAXSTOCKDISTRIBUTIONNUM;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
//	sendMsg.siCharID = pRecvMsg->siCharID;
	
	UI08 uiWarType = pRecvMsg->bVillageWarStockDistribution;

	// DB Query
	BEGIN_SP( "rsp_StockMaxDistributionNumSet" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siVillageUnique, sizeof(pRecvMsg->siVillageUnique) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siMaxStockDistributionNum, sizeof(pRecvMsg->siMaxStockDistributionNum) );
		m_odbc.SetParam( SQL_TINYINT, &uiWarType, sizeof(uiWarType) );
		m_odbc.SetParam( SQL_TINYINT, &pRecvMsg->uiRememberVillageUnique, sizeof(pRecvMsg->uiRememberVillageUnique) );
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0 );
		m_odbc.GetData( &sendMsg.siVillageUnique, 0 );
		m_odbc.GetData( &sendMsg.siMaxStockDistributionNum, 0 );
	END_FETCHDATA()

	sendMsg.uiRememberVillageUnique = pRecvMsg->uiRememberVillageUnique;
	sendMsg.siStockDistributionNum	= pRecvMsg->siStockDistributionNum;
	sendMsg.siStockPrice			= pRecvMsg->siStockPrice;
	sendMsg.bVillageWarStockDistribution = pRecvMsg->bVillageWarStockDistribution;
	
	sendMsg.siVillageWarWinVillageUnique = pRecvMsg->siVillageWarWinVillageUnique;


	// send
	SendPacket( (BYTE *)&sendMsg );

	// 로그 작성
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "WarType = %d, VillageUnique = %d, MaxStockNum = %d, StockDistributionNum = %d, StockPrice = %I64d"
				, pRecvMsg->bVillageWarStockDistribution, sendMsg.siVillageUnique, sendMsg.siMaxStockDistributionNum
				, sendMsg.siStockDistributionNum, sendMsg.siStockPrice
				);
		sDBLogMsg_QueryString LogMsg("Stock Max Distribution Set", NULL, NULL, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);

	}
}

void DBGameProtocol::SetVillageWarLoss()
{
	// 받은 패킷
	sDBRequest_SetVillageWarLoss *pRecvMsg	= (sDBRequest_SetVillageWarLoss *)m_pPacket;

	// 보낼 패킷
	sDBResponse_SetVillageWarLoss sendMsg;
	
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_SETVILLAGEWARLOSS;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
//	sendMsg.siCharID = pRecvMsg->siCharID;
	
	// DB Query
	BEGIN_SP( "rsp_StockVillageWarLoss" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siVillageUnique, sizeof(pRecvMsg->siVillageUnique) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siWinVillage, sizeof(pRecvMsg->siWinVillage) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siLossStockAmount, sizeof(pRecvMsg->siLossStockAmount) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siLossPersent, sizeof(pRecvMsg->siLossPersent) );
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0 );
		m_odbc.GetData( &sendMsg.siVillageUnique, 0 );
		m_odbc.GetData( &sendMsg.siWinVillage, 0 );
		m_odbc.GetData( &sendMsg.siTotalVillageStockAmount, 0 );
		m_odbc.GetData( &sendMsg.siTotalLossStockAmount, 0 );
	END_FETCHDATA()

	sendMsg.usCount = 0;
	BEGIN_NEXTRECORDSET()
		BEGIN_FETCHDATA()
			if(sendMsg.usCount >= 30) break;
			m_odbc.GetData( &sendMsg.siStockHolderPersonID[sendMsg.usCount], 0 );
			m_odbc.GetData( &sendMsg.siStockAmount[sendMsg.usCount], 0 );
			sendMsg.usCount++;
		END_FETCHDATA()
	END_NEXTRECORDSET()

	sendMsg.siLossPersent = pRecvMsg->siLossPersent;

	// send
	SendPacket( (BYTE *)&sendMsg );

	// 로그 작성
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "VillageUnique = %d, WinVillage = %d, TotalVillageStockAmount = %d, TotalLossStockAmount = %d,	LossPersonNum = %d"
				, sendMsg.siVillageUnique, sendMsg.siWinVillage, sendMsg.siTotalVillageStockAmount
				, sendMsg.siTotalLossStockAmount, sendMsg.usCount
				);
		sDBLogMsg_QueryString LogMsg("Stock Village War Loss", NULL, NULL, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);

	}
}

void DBGameProtocol::ReturnStockBidVillage()
{
	// 받은 패킷
	sDBRequest_ReturnStockBidVillage *pRecvMsg	= (sDBRequest_ReturnStockBidVillage *)m_pPacket;

	// 보낼 패킷
	sDBResponse_ReturnStockBidVillage sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_RETURNSTOCKBIDVILLAGE;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	//	sendMsg.siCharID = pRecvMsg->siCharID;

	// DB Query
	// DB Query
	BEGIN_SP( "rsp_StockBidVillageReturn" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siVillageUnique, sizeof(pRecvMsg->siVillageUnique) );
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0 );
		m_odbc.GetData( &sendMsg.siVillageUnique, 0 );
	END_FETCHDATA()

	BEGIN_NEXTRECORDSET()
		SI32 siBidderIndex = 0;
		SI64 itBeforeBankMoney = 0;
		BEGIN_FETCHDATA()
			itBeforeBankMoney = 0;
			m_odbc.GetData( &siBidderIndex, 0 );
			if(MAX_STOCKBIDDER_INVILLAGE <= siBidderIndex)
				continue;
			m_odbc.GetData( &sendMsg.clResultStockBidder[siBidderIndex].siPersonID, 0 );
			m_odbc.GetData( &sendMsg.clResultStockBidder[siBidderIndex].bSuccessfulBid, 0 );
			m_odbc.GetData( &sendMsg.clResultStockBidder[siBidderIndex].bExceedLimit, 0 );
			m_odbc.GetData( &sendMsg.clResultStockBidder[siBidderIndex].clPersonBankMoney.itMoney, 0 );
			m_odbc.GetData( &sendMsg.clResultStockBidder[siBidderIndex].clBidMoney.itMoney, 0 );
			m_odbc.GetData( &itBeforeBankMoney, 0 );
			// 로그 작성
			if(sendMsg.siResult == 1)
			{
				char log[512] = "";
				sprintf(log, "VillageUnique = %d, PersonID = %d, SuccessBid = %d, ExceedLimit = %d, BeforeBankMoney = %I64d, AfterBankMoney = %I64d, BidMoney = %I64d"
					, sendMsg.siVillageUnique, sendMsg.clResultStockBidder[siBidderIndex].siPersonID
					, sendMsg.clResultStockBidder[siBidderIndex].bSuccessfulBid, sendMsg.clResultStockBidder[siBidderIndex].bExceedLimit
					, itBeforeBankMoney, sendMsg.clResultStockBidder[siBidderIndex].clPersonBankMoney.itMoney
					, sendMsg.clResultStockBidder[siBidderIndex].clBidMoney.itMoney
					);
				sDBLogMsg_QueryString LogMsg("Stock Bid Return", sendMsg.clResultStockBidder[siBidderIndex].siPersonID, NULL, NULL, log);
				SendLogMsg((sPacketHeader *)&LogMsg);

			}
		END_FETCHDATA()
	END_NEXTRECORDSET()

	// send
	SendPacket( (BYTE *)&sendMsg );
}


//증자 주식에 입찰 신청을 한다
void DBGameProtocol::BidStock()
{
	// 받은 패킷
	sDBRequest_BidStock *pRecvMsg	= (sDBRequest_BidStock *)m_pPacket;

	// 보낼 패킷
	sDBResponse_BidStock sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_BIDSTOCK;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->usCharID;
	
	// PCK : 로그용 패킷
	sendMsg.siVillageUnique = pRecvMsg->siVillageUnique;

	SI32 siDateVary = pRecvMsg->clDate.GetDateVary();

	BEGIN_SP( "rsp_StockBidStart" )
		m_odbc.SetParam(SQL_INTEGER, &pRecvMsg->siVillageUnique, sizeof(pRecvMsg->siVillageUnique));
		m_odbc.SetParam(SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID));
		m_odbc.SetParam(SQL_BIGINT, &pRecvMsg->siStockMoney, sizeof(pRecvMsg->siStockMoney));
		m_odbc.SetParam(SQL_INTEGER, &siDateVary, sizeof(siDateVary));
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0 );
		GetStockBidVillageInfo(&sendMsg.clStockBidVillageInfo);
//		m_odbc.GetData( &sendMsg.clStockBidVillageInfo.siVillageUnique_, 0 );
//		m_odbc.GetData( &sendMsg.clStockBidVillageInfo.siTotalStockAmount_, 0 );
//		m_odbc.GetData( &sendMsg.clStockBidVillageInfo.siTottalIncreaseStockAmount_, 0 );
//		m_odbc.GetData( &sendMsg.clStockBidVillageInfo.siLeftIncreaseStockAmount_, 0 );
//		m_odbc.GetData( &sendMsg.clStockBidVillageInfo.siCurrentStockPrice_, 0 );
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );

	// 로그 작성
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "VillageUnique = %d, TotalStock = %d, BidAmount = %d, DateVary = %d"
			, sendMsg.clStockBidVillageInfo.siVillageUnique_, sendMsg.clStockBidVillageInfo.siTottalIncreaseStockAmount_, sendMsg.clStockBidVillageInfo.siTottalIncreaseStockAmount_, siDateVary
			);
		sDBLogMsg_QueryString LogMsg("Stock Bid", pRecvMsg->siPersonID, NULL, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);

	}
}

void DBGameProtocol::BuyBidStock()
{
	// 받은 패킷
	sDBRequest_BuyBidStock *pRecvMsg	= (sDBRequest_BuyBidStock *)m_pPacket;

	// 보낼 패킷
	sDBResponse_BuyBidStock sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_BUYBIDSTOCK;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->usCharID;

	SI32 siOwnerAmount = 0;

	BEGIN_SP( "rsp_StockBidBuy" )
		m_odbc.SetParam(SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID));
		m_odbc.SetParam(SQL_INTEGER, &pRecvMsg->siVillageUnique, sizeof(pRecvMsg->siVillageUnique));
		m_odbc.SetParam(SQL_INTEGER, &pRecvMsg->siAmount, sizeof(pRecvMsg->siAmount));
		m_odbc.SetParam(SQL_BIGINT, &pRecvMsg->siPrice, sizeof(pRecvMsg->siPrice));
		m_odbc.SetParam(SQL_BIGINT, &pRecvMsg->siTotalPrice, sizeof(pRecvMsg->siTotalPrice));
		m_odbc.SetParam(SQL_TINYINT, &pRecvMsg->bBuySwitch, sizeof(pRecvMsg->bBuySwitch));
		m_odbc.SetParam(SQL_BIGINT, &pRecvMsg->siFee, sizeof(pRecvMsg->siFee));
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0 );
		m_odbc.GetData( &sendMsg.siAmount, 0 );
		m_odbc.GetData( &sendMsg.siTotalPrice, 0 );
		GetStockBidVillageInfo( &sendMsg.clStockBidVillageInfo );
		m_odbc.GetData( &siOwnerAmount, 0 );
		m_odbc.GetData( &sendMsg.clCityhallMoney.itMoney, 0 );
		m_odbc.GetData( &sendMsg.siBuyStockLeftMoney, 0 );
	END_FETCHDATA()

	sendMsg.siFee = pRecvMsg->siFee;
	sendMsg.siPersonID = pRecvMsg->siPersonID;
	sendMsg.siBuyMode = pRecvMsg->siBuyMode;

	// send
	SendPacket( (BYTE *)&sendMsg );

	// 로그 작성
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "VillageUnique = %d, Amount = %d, Price = %I64d, TotalPrice = %I64d, LeftBidAmount = %d, LeftOwnerAmount = %d, Fee = %I64d, CityhallProfitMoney = %I64d, LeftPersonMoney = %I64d"
			, pRecvMsg->siVillageUnique, pRecvMsg->siAmount, pRecvMsg->siPrice, pRecvMsg->siTotalPrice, sendMsg.clStockBidVillageInfo.siLeftIncreaseStockAmount_, siOwnerAmount
			, pRecvMsg->siFee, sendMsg.clCityhallMoney.itMoney, sendMsg.siBuyStockLeftMoney
			);
		sDBLogMsg_QueryString LogMsg("Stock Bid Buy", pRecvMsg->siPersonID, NULL, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);

	}
}

void DBGameProtocol::MyStockInfo()
{
	// 받은 패킷
	sDBRequest_MyStockInfo *pRecvMsg	= (sDBRequest_MyStockInfo *)m_pPacket;

	// 보낼 패킷
	sDBResponse_MyStockInfo sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_MYSTOCKINFO;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->usCharID;


	BEGIN_SP( "rsp_StockPersonAccountInfoGet" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID) );
	END_SP()

	EXEC_SQL()

	int siIndex = 0;

	BEGIN_FETCHDATA()
		m_odbc.GetData( &(sendMsg.clInfo.clUnit[siIndex].siVillageUnique), 0);
		m_odbc.GetData( &(sendMsg.clInfo.clUnit[siIndex].siAmount), 0);
		m_odbc.GetData( &(sendMsg.clInfo.clUnit[siIndex].bBuySwitch), 0);
		m_odbc.GetData( &(sendMsg.clInfo.clUnit[siIndex].siLimitBuyBidStockAmount), 0);
		siIndex++;
	END_FETCHDATA()

	sendMsg.siResult = 1;
	sendMsg.siPersonID = pRecvMsg->siPersonID;

	// send
	SendPacket( (BYTE *)&sendMsg );
}

void DBGameProtocol::GetStockOfferingInfo()
{
	// 받은 패킷
	sDBRequest_GetStockOfferingInfo *pRecvMsg	= (sDBRequest_GetStockOfferingInfo *)m_pPacket;

	// 보낼 패킷
	sDBResponse_GetStockOfferingInfo sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_GETSTOCKOFFERINGINFO;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
//	sendMsg.usCharID = pRecvMsg->usCharID;


	BEGIN_SP( "rsp_StockOfferingInfoGet" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siVillageUnique, sizeof(pRecvMsg->siVillageUnique) );
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siVillageUnique );
		m_odbc.GetData( &sendMsg.siStatus );
		m_odbc.GetData( &sendMsg.siTotalOffering );
		m_odbc.GetData( &sendMsg.siStockAmount );
		m_odbc.GetData( &sendMsg.siPrice );
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );
}

void DBGameProtocol::SetStockOffering()
{
	// 받은 패킷
	sDBRequest_SetStockOffering *pRecvMsg	= (sDBRequest_SetStockOffering *)m_pPacket;

	// 보낼 패킷
	sDBResponse_SetStockOffering sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_SETSTOCKOFFERING;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	//sendMsg.usCharID = pRecvMsg->usCharID;


	BEGIN_SP( "rsp_StockOfferingInfoSet" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siVillageUnique, sizeof(pRecvMsg->siVillageUnique) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siTotalOffering, sizeof(pRecvMsg->siTotalOffering) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siStockAmount, sizeof(pRecvMsg->siStockAmount) );
		m_odbc.SetParam( SQL_BIGINT, &pRecvMsg->siPrice, sizeof(pRecvMsg->siPrice) );
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siVillageUnique );
		m_odbc.GetData( &sendMsg.siTotalOffering );
		m_odbc.GetData( &sendMsg.siStockAmount );
		m_odbc.GetData( &sendMsg.siPrice );
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );

	// 로그 작성
	{
		char log[512] = "";
		sprintf(log, "VillageUnique = %d, TotalOffering = %d, StockAmount = %d, Price = %I64d"
			, pRecvMsg->siVillageUnique, sendMsg.siTotalOffering, sendMsg.siStockAmount, sendMsg.siPrice
			);
		sDBLogMsg_QueryString LogMsg("Stock Offering Set", NULL, NULL, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);

	}
}

void DBGameProtocol::EndStockOffering()
{
	// 받은 패킷
	sDBRequest_EndStockOffering *pRecvMsg	= (sDBRequest_EndStockOffering *)m_pPacket;

	// 보낼 패킷
	sDBResponse_EndStockOffering sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_ENDSTOCKOFFERING;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	//sendMsg.usCharID = pRecvMsg->usCharID;


	BEGIN_SP( "rsp_StockOfferingEnd" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siVillageUnique, sizeof(pRecvMsg->siVillageUnique) );
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siVillageUnique );
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );

	// 로그 작성
	{
		char log[512] = "";
		sprintf(log, "VillageUnique = %d"
			, pRecvMsg->siVillageUnique
			);
		sDBLogMsg_QueryString LogMsg("Stock Offering End", NULL, NULL, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);

	}
}

void DBGameProtocol::StockOfferingOrder()
{
	// 받은 패킷
	sDBRequest_StockOfferingOrder *pRecvMsg	= (sDBRequest_StockOfferingOrder *)m_pPacket;

	// 보낼 패킷
	sDBResponse_StockOfferingOrder sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_STOCKOFFERINGORDER;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->usCharID;


	BEGIN_SP( "rsp_StockOfferingOrder" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siVillageUnique, sizeof(pRecvMsg->siVillageUnique) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siStockAmount, sizeof(pRecvMsg->siStockAmount) );
		m_odbc.SetParam( SQL_BIGINT, &pRecvMsg->siMoney, sizeof(pRecvMsg->siMoney) );
		m_odbc.SetParam( SQL_VARCHAR, pRecvMsg->szIP, sizeof(pRecvMsg->szIP) );
		m_odbc.SetParam( SQL_VARCHAR, pRecvMsg->szPrivateIP, sizeof(pRecvMsg->szPrivateIP) );
		m_odbc.SetParam( SQL_VARCHAR, pRecvMsg->szMacAddress, sizeof(pRecvMsg->szMacAddress) );
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult );
		m_odbc.GetData( &sendMsg.siVillageUnique );
		m_odbc.GetData( &sendMsg.siPersonID );
		m_odbc.GetData( &sendMsg.siStockAmount );
		m_odbc.GetData( &sendMsg.siMoney );
		m_odbc.GetData( &sendMsg.siPersonMoney );
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );

	// 로그 작성
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "VillageUnique = %d, StockAmount = %d, Money = %I64d, AfterPersonMoney = %I64d"
			, pRecvMsg->siVillageUnique, sendMsg.siStockAmount, sendMsg.siMoney, sendMsg.siPersonMoney
			);
		sDBLogMsg_QueryString LogMsg("Stock Offering Order", pRecvMsg->siPersonID, NULL, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);

	}
}

void DBGameProtocol::GetStockOfferingOrderList()
{
	// 받은 패킷
	sDBRequest_GetStockOfferingOrderList *pRecvMsg	= (sDBRequest_GetStockOfferingOrderList *)m_pPacket;

	// 보낼 패킷
	sDBResponse_GetStockOfferingOrderList sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_GETSTOCKOFFERINGORDERLIST;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	//sendMsg.usCharID = pRecvMsg->usCharID;


	BEGIN_SP( "rsp_StockOfferingOrderList" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siVillageUnique, sizeof(pRecvMsg->siVillageUnique) );
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siVillageUnique );
		m_odbc.GetData( &sendMsg.siPersonID );
		m_odbc.GetData( &sendMsg.siStockAmount );
		m_odbc.GetData( &sendMsg.siMoney );
		m_odbc.GetData( sendMsg.szIP, sizeof(sendMsg.szIP) );
		m_odbc.GetData( sendMsg.szPrivateIP, sizeof(sendMsg.szPrivateIP) );
		m_odbc.GetData( sendMsg.szMacAddress, sizeof(sendMsg.szMacAddress) );
		// send
		SendPacket( (BYTE *)&sendMsg );
	END_FETCHDATA()

	sendMsg.siVillageUnique = -1;
	SendPacket( (BYTE *)&sendMsg );

}

void DBGameProtocol::ExecuteStockOfferingOrder()
{
	// 받은 패킷
	sDBRequest_ExecuteStockOfferingOrder *pRecvMsg	= (sDBRequest_ExecuteStockOfferingOrder *)m_pPacket;

	// 보낼 패킷
	sDBResponse_ExecuteStockOfferingOrder sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_EXECUTESTOCKOFFERINGORDER;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->usCharID;

	MStrCpy(sendMsg.szIP,			pRecvMsg->szIP,			sizeof(sendMsg.szIP));
	MStrCpy(sendMsg.szPrivateIP,	pRecvMsg->szPrivateIP,	sizeof(sendMsg.szPrivateIP));
	MStrCpy(sendMsg.szMacAddress,	pRecvMsg->szMacAddress, sizeof(sendMsg.szMacAddress));

	BEGIN_SP( "rsp_StockOfferingOrderExecute" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siType, sizeof(pRecvMsg->siType) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siVillageUnique, sizeof(pRecvMsg->siVillageUnique) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siStockAmount, sizeof(pRecvMsg->siStockAmount) );
		m_odbc.SetParam( SQL_BIGINT, &pRecvMsg->siMoney, sizeof(pRecvMsg->siMoney) );
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult );
		m_odbc.GetData( &sendMsg.siType );
		m_odbc.GetData( &sendMsg.siVillageUnique );
		m_odbc.GetData( &sendMsg.siPersonID );
		m_odbc.GetData( &sendMsg.siStockAmount );
		m_odbc.GetData( &sendMsg.siMoney );
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );

	// 로그 작성
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "VillageUnique = %d, Type = %d, StockAmount = %d, Money = %I64d"
			, pRecvMsg->siVillageUnique, sendMsg.siType, sendMsg.siStockAmount, sendMsg.siMoney
			);
		sDBLogMsg_QueryString LogMsg("Stock Offering Order Execute", pRecvMsg->siPersonID, NULL, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);

	}
}

void DBGameProtocol::ResetStockOfferingOrderList()
{
	// 받은 패킷
	sDBRequest_ResetStockOfferingOrderList *pRecvMsg	= (sDBRequest_ResetStockOfferingOrderList *)m_pPacket;

	// 보낼 패킷
	sDBResponse_ResetStockOfferingOrderList sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_RESETSTOCKOFFERINGORDERLIST;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	//sendMsg.usCharID = pRecvMsg->usCharID;


	BEGIN_SP( "rsp_StockOfferingOrderReset" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siVillageUnique, sizeof(pRecvMsg->siVillageUnique) );
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult );
		m_odbc.GetData( &sendMsg.siVillageUnique );
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );
	// 로그 작성
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "VillageUnique = %d"
			, pRecvMsg->siVillageUnique
			);
		sDBLogMsg_QueryString LogMsg("Stock Offering Order Reset", NULL, NULL, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

void DBGameProtocol::ExchangeFromStock()
{
	// 받은 패킷
	sDBRequest_ExchangeFromStock  *pRecvMsg	= (sDBRequest_ExchangeFromStock  *)m_pPacket;

	// 보낼 패킷
	sDBResponse_ExchangeFromStock sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_EXCHANGEFROMSTOCK;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.siCharID = pRecvMsg->siCharID;


	BEGIN_SP( "rsp_StockExchangeToBank" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siStockUnique, sizeof(pRecvMsg->siStockUnique) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siSellAmount, sizeof(pRecvMsg->siSellAmount) );
		m_odbc.SetParam( SQL_VARCHAR, pRecvMsg->szSecondPassWord, sizeof(pRecvMsg->szSecondPassWord) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siBankType, sizeof(pRecvMsg->siBankType) );
		m_odbc.SetParam( SQL_VARCHAR, pRecvMsg->szBankAccount, sizeof(pRecvMsg->szBankAccount) );
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult );
		m_odbc.GetData( &sendMsg.siStockUnique );
		m_odbc.GetData( &sendMsg.siSellAmount );
		m_odbc.GetData( &sendMsg.siLeftStockAmount );
		m_odbc.GetData( &sendMsg.siTotalStockAmount );
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );
}

void DBGameProtocol::GiveStock()
{
	// 받은 패킷
	sDBRequest_GiveStock  *pRecvMsg	= (sDBRequest_GiveStock  *)m_pPacket;

	// 보낼 패킷
	sDBResponse_GiveStock sendMsg;

	sendMsg.packetHeader.usCmd	= DBMSG_RESPONSE_GIVESTOCK;
	sendMsg.packetHeader.usSize	= sizeof( sendMsg );
	sendMsg.siCharID			= pRecvMsg->siCharID;
	sendMsg.siPersonID			= pRecvMsg->siPersonID;

	SI32 siMaxStockKind = MAX_STOCK_KIND_IN_ACOUNT;

	BEGIN_SP( "rsp_StockGive" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID,		sizeof(pRecvMsg->siPersonID) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siVillageUnique,	sizeof(pRecvMsg->siVillageUnique) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siStockAmount,		sizeof(pRecvMsg->siStockAmount) );
		m_odbc.SetParam( SQL_INTEGER, &siMaxStockKind,				sizeof(siMaxStockKind) );
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult );
		m_odbc.GetData( &sendMsg.siVillageUnique );
		m_odbc.GetData( &sendMsg.siStockAmount );
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );

	// Write Log
	if ( sendMsg.siResult == 1 )
	{
		char log[512] = "";
		sprintf( log, "VillageUnique = %d, Amount = %d", pRecvMsg->siVillageUnique, pRecvMsg->siStockAmount );
		sDBLogMsg_QueryString LogMsg("Give Stock", pRecvMsg->siPersonID, NULL, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}
