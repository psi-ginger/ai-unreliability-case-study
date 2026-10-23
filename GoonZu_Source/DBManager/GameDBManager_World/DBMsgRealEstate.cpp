#include "DBGameProtocol.h"
#include "DBMsg-Structure.h"
#include "DBMsg-RealEstateMarket.h"

void DBGameProtocol::GetRealEstateMarketStrInfo()
{
	// 받은 패킷
	sDBRequest_GetStrInfo *pRecvMsg = (sDBRequest_GetStrInfo *)m_pPacket;
	
	// 보낼 패킷
	sDBResponse_GetStrInfo sendMsg;
	sendMsg.clStrInfo.clCommonInfo.siStrType = RANKTYPE_REALESTATEMARKET;
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_GETSTRINFO;
	sendMsg.packetHeader.usSize = sendMsg.GetTotalSize();
	sendMsg.usCharID = pRecvMsg->usCharID;

	// DB Query
	BEGIN_SP( "rsp_RealEstateMarketInfoGet" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siVillageUnique, sizeof(pRecvMsg->siVillageUnique) );
	END_SP()
	
	EXEC_SQL()
	
	BEGIN_FETCHDATA()
		GetFeeInfo( &sendMsg.clStrInfo.clRealEstateMarketStrInfo.clFee );
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


void DBGameProtocol::RealEstateMarketOrderSell()
{
	// 받은 패킷
	sDBRequest_RealEstateMarketOrderSell *pRecvMsg = (sDBRequest_RealEstateMarketOrderSell *)m_pPacket;
	
	// 보낼 패킷
	sDBResponse_RealEstateMarketOrderSell sendMsg;
	
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_REALESTATEMARKET_ORDERSELL;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->usCharID;

	SI32 siDateVary = pRecvMsg->clOrder.clDate.GetDateVary();

	// DB Query
	BEGIN_SP( "rsp_RealEstateMarketOrderSell" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siVillageUnique, sizeof(pRecvMsg->siVillageUnique) );
		SetRealEstateOrderInfo( &pRecvMsg->clOrder, &siDateVary );
		m_odbc.SetParam( SQL_BIGINT, &pRecvMsg->siTax, sizeof(pRecvMsg->siTax) );
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0 );
		m_odbc.GetData( &sendMsg.siVillageUnique, 0 );
		GetRealEstateOrderInfo( &sendMsg.clOrder );
		m_odbc.GetData( &sendMsg.siTax, 0 );
		m_odbc.GetData( &sendMsg.clRealEstateMarketMoney.itMoney, 0 );
		m_odbc.GetData( &sendMsg.siPersonMoney, 0 );
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );

	// 로그 작성
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "VillageUnique = %d, OrderIndex = %d, Price = %I64d, OrderVillageUnique = %d, OrderHouseUintIndex = %d, Tax = %I64d, MarketMoney = %I64d, PersonMoney = %I64d"
				, sendMsg.siVillageUnique, sendMsg.clOrder.siIndex, sendMsg.clOrder.siPrice, sendMsg.clOrder.siVillageUnique, sendMsg.clOrder.siHouseIndex
				, sendMsg.siTax, sendMsg.clRealEstateMarketMoney.itMoney, sendMsg.siPersonMoney
				);
		sDBLogMsg_QueryString LogMsg("RealEstateMarket Sell Order", sendMsg.clOrder.siPersonID, NULL, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

void DBGameProtocol::RealEstateMarketCancelOrder()
{
	// 받은 패킷
	sDBRequest_RealEstateMarketCancelOrder *pRecvMsg = (sDBRequest_RealEstateMarketCancelOrder *)m_pPacket;
	
	// 보낼 패킷
	sDBResponse_RealEstateMarketCancelOrder sendMsg;
	
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_REALESTATEMARKET_CANCELORDER;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->usCharID;

	//[추가 : 황진성 2007. 10. 31 로그에 이용. 부동산 판매등록 취소시 필요.]
	sendMsg.siHouseIndex	= pRecvMsg->siHouseIndex;
	sendMsg.siVillageUnique = pRecvMsg->siVillageUnique;

	// DB Query
	BEGIN_SP( "rsp_RealEstateMarketCancelOrder" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siVillageUnique, sizeof(pRecvMsg->siVillageUnique) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siHouseIndex, sizeof(pRecvMsg->siHouseIndex) );
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0 );
		m_odbc.GetData( &sendMsg.siPersonID, 0 );
		m_odbc.GetData( &sendMsg.siOrderIndex, 0 );
		m_odbc.GetData( &sendMsg.siVillageUnique, 0 );
		m_odbc.GetData( &sendMsg.siHouseIndex, 0 );
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );

	// 로그 작성
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "VillageUnique = %d, HouseUnitIndex = %d"
					, sendMsg.siVillageUnique, sendMsg.siHouseIndex
				);
		sDBLogMsg_QueryString LogMsg("RealEstateMarket Sell Order Cancel", sendMsg.siPersonID, NULL, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

void DBGameProtocol::RealEstateMarketMyOrder()
{
	// 받은 패킷
	sDBRequest_RealEstateMarketMyOrder *pRecvMsg = (sDBRequest_RealEstateMarketMyOrder *)m_pPacket;
	
	// 보낼 패킷
	sDBResponse_RealEstateMarketMyOrder sendMsg;
	
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_REALESTATEMARKET_MYORDER;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->usCharID;

	// DB Query
	BEGIN_SP( "rsp_RealEstateMarketMyOrder" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID) );
	END_SP()

	EXEC_SQL()

	sendMsg.usCount = 0;
	BEGIN_FETCHDATA()
		if(sendMsg.usCount >= 5 )
			continue;
		GetRealEstateOrderInfo( &sendMsg.clMyOrder[sendMsg.usCount++] );
	END_FETCHDATA()

	// 주문정보가 하나이상일때 1
	if(sendMsg.usCount > 0)
		sendMsg.siResult = 1;

	// send
	SendPacket( (BYTE *)&sendMsg );
}

void DBGameProtocol::RealEstateMarketGetManyOrder()
{
	// 받은 패킷
	sDBRequest_RealEstateMarketGetManyOrder *pRecvMsg = (sDBRequest_RealEstateMarketGetManyOrder *)m_pPacket;
	
	// 보낼 패킷
	sDBResponse_RealEstateMarketGetManyOrder sendMsg;
	
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_REALESTATEMARKET_GETMANYORDER;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->usCharID;

	// DB Query
	BEGIN_SP( "rsp_RealEstateMarketGetManyOrder" )
	END_SP()

	EXEC_SQL()

	BOOL bSend = FALSE;

	sendMsg.siResult = 1;
	sendMsg.usCount = 0;
	

	BEGIN_FETCHDATA()
		bSend = FALSE;
		GetRealEstateOrderInfo( &sendMsg.clOrder[ sendMsg.usCount++ ] );

		if( sendMsg.usCount >= 50)
		{
			SendPacket( (BYTE *)&sendMsg );
			sendMsg.usCount = 0;
			bSend = TRUE;
		}
	END_FETCHDATA()

	if(bSend == FALSE)
	{
		SendPacket( (BYTE *)&sendMsg );
	}
}

void DBGameProtocol::RealEstateMarketSetFee()
{
	// 받은 패킷
	sDBRequest_RealEstateMarketSetFee *pRecvMsg = (sDBRequest_RealEstateMarketSetFee *)m_pPacket;
	
	// 보낼 패킷
	sDBResponse_RealEstateMarketSetFee sendMsg;
	
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_REALESTATEMARKET_SETFEE;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->usCharID;

	cltDBFee clFee( &pRecvMsg->clFee );

	// DB Query
	BEGIN_SP( "rsp_RealEstateMarketSetFee" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siVillageUnique, sizeof(pRecvMsg->siVillageUnique) );
		SetFeeInfo( &clFee );
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0 );
		m_odbc.GetData( &sendMsg.siPersonID, 0 );
		m_odbc.GetData( &sendMsg.siVillageUnique, 0 );
		GetFeeInfo( &sendMsg.clFee );
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );

	// 로그 작성
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "VillageUnique = %d, FeeRate = %d, MinFee = %I64d, MaxFee = %I64d"
				, sendMsg.siVillageUnique, sendMsg.clFee.GetFeeRate(), sendMsg.clFee.GetMinFee(), sendMsg.clFee.GetMaxFee()
				);
		sDBLogMsg_QueryString LogMsg("RealEstateMarket Set Fee", sendMsg.siPersonID, NULL, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

void DBGameProtocol::RealEstateMarketReduceOrderPrice()
{
	// 받은 패킷
	sDBRequest_ReduceRealEstateOrderPrice *pRecvMsg = (sDBRequest_ReduceRealEstateOrderPrice *)m_pPacket;
	
	// 보낼 패킷
	sDBResponse_ReduceRealEstateOrderPrice sendMsg;
	
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_REDUCEREALESTATEORDERPRICE;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
//	sendMsg.usCharID = pRecvMsg->usCharID;

	SI32 siDateVary = pRecvMsg->clOrder.clDate.GetDateVary();

	// DB Query
	BEGIN_SP( "rsp_RealEstateMarketReduceOrderPrice" )
		SetRealEstateOrderInfo( &pRecvMsg->clOrder, &siDateVary );
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0 );
		GetRealEstateOrderInfo( &sendMsg.clOrder );
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );

	// 로그 작성
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "OrderIndex = %d, Price = %I64d, OrderVillageUnique = %d, OrderHouseUnitIndex = %d"
				, pRecvMsg->clOrder.siIndex, pRecvMsg->clOrder.siPrice, pRecvMsg->clOrder.siVillageUnique, pRecvMsg->clOrder.siHouseIndex
				);
		sDBLogMsg_QueryString LogMsg("RealEstateMarket Price Reduce", pRecvMsg->clOrder.siPersonID, NULL, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

void DBGameProtocol::RealEstateMarketBuy()
{
	// 받은 패킷
	sDBRequest_RealEstateMarketBuy *pRecvMsg = (sDBRequest_RealEstateMarketBuy *)m_pPacket;
	
	// 보낼 패킷
	sDBResponse_RealEstateMarketBuy sendMsg;
	
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_REALESTATEMARKET_BUY;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->usCharID;

	// DB Query
	BEGIN_SP( "rsp_RealEstateMarketBuy" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siOrderIndex, sizeof(pRecvMsg->siOrderIndex) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siHouseVillage, sizeof(pRecvMsg->siHouseVillage) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siHouseIndex, sizeof(pRecvMsg->siHouseIndex) );
		m_odbc.SetParam( SQL_BIGINT, &pRecvMsg->siPrice, sizeof(pRecvMsg->siPrice) );
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0 );
		m_odbc.GetData( &sendMsg.siPersonID, 0 );
		m_odbc.GetData( sendMsg.szPersonName, sizeof(sendMsg.szPersonName) );
		m_odbc.GetData( &sendMsg.siMoney, 0 );
		m_odbc.GetData( &sendMsg.siOrderIndex, 0 );
		m_odbc.GetData( &sendMsg.siHouseVillage, 0 );
		m_odbc.GetData( &sendMsg.siHouseIndex, 0 );
		m_odbc.GetData( &sendMsg.siTradePrice, 0 );
		m_odbc.GetData( &sendMsg.siSellerPersonID, 0 );
		m_odbc.GetData( &sendMsg.clSellerBankMoney.itMoney, 0);
		m_odbc.GetData( &sendMsg.siVillageUnique, 0);
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );

	// 로그 작성
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "OrderIndex = %d, HouseUnitIndex = %d, Price = %I64d, AfterBuyerMoney = %I64d, AfterSellerBankMoney = %I64d"
				, pRecvMsg->siOrderIndex, pRecvMsg->siHouseVillage, pRecvMsg->siHouseIndex, sendMsg.siTradePrice, sendMsg.siMoney, sendMsg.clSellerBankMoney.itMoney
				);
		sDBLogMsg_QueryString LogMsg("RealEstateMarket Buy", sendMsg.siPersonID, sendMsg.siSellerPersonID, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

void DBGameProtocol::RealEstateMarketDelOrder()
{
	// 받은 패킷
	sDBRequest_RealEstateMarketDelOrder *pRecvMsg = (sDBRequest_RealEstateMarketDelOrder *)m_pPacket;
	
	// 보낼 패킷
	sDBResponse_RealEstateMarketDelOrder sendMsg;
	
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_REALESTATEMARKET_DELORDER;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
//	sendMsg.usCharID = pRecvMsg->usCharID;

	// DB Query
	BEGIN_SP( "rsp_RealEstateMarketDelOrder" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siOrderIndex, sizeof(pRecvMsg->siOrderIndex) );
	END_SP()

	EXEC_SQL()

	SI32 siVillageUnique = 0;
	SI32 siHouseIndex = 0;

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0 );
		m_odbc.GetData( &sendMsg.siPersonID, 0 );
		m_odbc.GetData( &sendMsg.siOrderIndex, 0 );
		m_odbc.GetData( &siVillageUnique, 0 );
		m_odbc.GetData( &siHouseIndex, 0 );
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );

	// 로그 작성
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "VillageUnique = %d, HouseUnitIndex = %d"
					, siVillageUnique, siHouseIndex
				);
		sDBLogMsg_QueryString LogMsg("RealEstateMarket Sell Order Delete", sendMsg.siPersonID, NULL, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}
