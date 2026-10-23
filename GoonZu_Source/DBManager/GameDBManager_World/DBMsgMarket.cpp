#include "DBGameProtocol.h"
#include "DBMsg-Structure.h"
#include "DBMsg-Market.h"

void DBGameProtocol::GetMarketStrInfo()
{

	// 받은 패킷
	sDBRequest_GetStrInfo *pRecvMsg = (sDBRequest_GetStrInfo *)m_pPacket;

	SendMarketStrInfo(pRecvMsg->siVillageUnique, pRecvMsg->usCharID);
	
}

void DBGameProtocol::SendMarketStrInfo( SI32 siVillageUnique, UI16 usCharID )
{
	// 받은 패킷
	sDBRequest_GetStrInfo *pRecvMsg = (sDBRequest_GetStrInfo *)m_pPacket;

	// 보낼 패킷
	sDBResponse_GetStrInfo sendMsg;
	sendMsg.clStrInfo.clCommonInfo.siStrType = RANKTYPE_MARKET;
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_GETSTRINFO;
	sendMsg.packetHeader.usSize = sendMsg.GetTotalSize();
	sendMsg.usCharID = usCharID;
	

	// DB Query
	BEGIN_SP( "rsp_MarketInfoGet" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siVillageUnique, sizeof(pRecvMsg->siVillageUnique) );
	END_SP()
	
	EXEC_SQL()
	
	BEGIN_FETCHDATA()
		GetFeeInfo( &sendMsg.clStrInfo.clMarketStrInfo.clFee );
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

void DBGameProtocol::OrderMarket()
{
	// 받은 패킷
	sDBRequest_OrderMarket *pRecvMsg = (sDBRequest_OrderMarket *)m_pPacket;

	cltItem clItem;
	clItem.Set(&pRecvMsg->clOrder.clItemData);
	clItem.SetItemNum(pRecvMsg->clOrder.siAmount);


	// 보낼 패킷
	sDBResponse_OrderMarket sendMsg;
	
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_ORDERMARKET;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->usCharID;
	
	//[추가 : 황진성 2007. 10. 25 로그에서 이용, 파는 아이템 정보]
	sendMsg.clOrderItem.Set(&clItem);
	
	// DB Query
	BEGIN_SP( "rsp_MarketOrderSet" )
		m_odbc.SetParam( SQL_INTEGER, &(pRecvMsg->siOrderMode),			sizeof(pRecvMsg->siOrderMode) );
		m_odbc.SetParam( SQL_INTEGER, &(pRecvMsg->siVillageUnique),		sizeof(pRecvMsg->siVillageUnique) );
		m_odbc.SetParam( SQL_INTEGER, &(pRecvMsg->clOrder.siPersonID),	sizeof(pRecvMsg->clOrder.siPersonID) );
		m_odbc.SetParam( SQL_BIGINT, &(pRecvMsg->clOrder.siPrice),		sizeof(pRecvMsg->clOrder.siPrice) );
		SetItemParam( &clItem );
		m_odbc.SetParam( SQL_INTEGER, &(pRecvMsg->clOrder.siDate),		sizeof(pRecvMsg->clOrder.siDate) );
		m_odbc.SetParam( SQL_SMALLINT, &(pRecvMsg->siItemPos),			sizeof(pRecvMsg->siItemPos) );
		m_odbc.SetParam( SQL_TINYINT, &(pRecvMsg->bPileSwitch),			sizeof(pRecvMsg->bPileSwitch) );
	END_SP()
	
	EXEC_SQL()
	
	BEGIN_FETCHDATA()
		m_odbc.GetData( &(sendMsg.siResult),			0);
		m_odbc.GetData( &(sendMsg.clOrder.siIndex),		0);
		m_odbc.GetData( &(sendMsg.clOrder.siAmount),	0);
		m_odbc.GetData( &(sendMsg.clOrder.siPrice),		0);
		m_odbc.GetData( &(sendMsg.clOrder.siDate),		0);
	END_FETCHDATA()

	BEGIN_NEXTRECORDSET()
	BEGIN_FETCHDATA()
		GetItemData( &sendMsg.clFromItem);
	END_FETCHDATA()
	END_NEXTRECORDSET()

	sendMsg.siOrderMode			= pRecvMsg->siOrderMode;
	sendMsg.siItemFromPos		= pRecvMsg->siItemPos;
	sendMsg.clOrder.siType		= pRecvMsg->clOrder.siType;
	sendMsg.clOrder.siPersonID	= pRecvMsg->clOrder.siPersonID;
	sendMsg.clOrder.clItemData.Set(&pRecvMsg->clOrder.clItemData);
	sendMsg.clOrder.siVillageUnique = pRecvMsg->clOrder.siVillageUnique;
	sendMsg.siTax				= pRecvMsg->siTax;
	
	// send
	SendPacket( (BYTE *)&sendMsg );

	// 객주 계좌 정보를 보낸다.
    if( 0 < pRecvMsg->clOrder.siPersonID )
		SendMarketAccountInfo(pRecvMsg->clOrder.siPersonID, pRecvMsg->usCharID);

	// 로그 작성
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "VillageUnique = %d, FromInvPos = %d, MarketIndex = %d, Amount = %d, Price = %I64d"
				, pRecvMsg->siVillageUnique, pRecvMsg->siItemPos, sendMsg.clOrder.siIndex, sendMsg.clOrder.siAmount, sendMsg.clOrder.siPrice
				);
		sDBLogMsg_QueryString LogMsg("Set Market Order", sendMsg.clOrder.siPersonID, NULL, pRecvMsg->clOrder.clItemData.siUnique, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}


void DBGameProtocol::GetManyMarketOrder()
{
	// 받은 패킷
	sDBRequest_GetManyMarketOrder *pRecvMsg = (sDBRequest_GetManyMarketOrder *)m_pPacket;
	
	// 보낼 패킷
	sDBResponse_GetManyMarketOrder sendMsg;
	
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_GETMANYMARKETORDER;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->usCharID;
	
	// DB Query
	SI32	siHowMany	= GET_MANY_MARKET_ORDER_NUM;
	
	BEGIN_SP( "rsp_MarketOrderManyGet" )
		m_odbc.SetParam( SQL_INTEGER, &(pRecvMsg->siStartIndex), sizeof(pRecvMsg->siStartIndex) );
		m_odbc.SetParam( SQL_INTEGER, &siHowMany, sizeof(siHowMany) );
	END_SP()
	
	EXEC_SQL()
	
	cltItem clItem;
	int i = 0;

	BEGIN_FETCHDATA()
		m_odbc.GetData( &(sendMsg.clTradeOrder[i].siIndex), 0);
		m_odbc.GetData( &(sendMsg.clTradeOrder[i].siPersonID), 0);
		m_odbc.GetData( &(sendMsg.clTradeOrder[i].siPrice), 0);
		GetItemData( &clItem);
		m_odbc.GetData( &(sendMsg.clTradeOrder[i].siDate), 0);

		sendMsg.clTradeOrder[i].siType = TRADEORDER_TYPE_OBJECT;
		sendMsg.clTradeOrder[i].siAmount = clItem.GetItemNum();
		sendMsg.clTradeOrder[i].clItemData.Set(&clItem);
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


void DBGameProtocol::TradeMarket()
{
	// 받은 패킷
	sDBRequest_TradeMarket *pRecvMsg	= (sDBRequest_TradeMarket *)m_pPacket;
	
	// 보낼 패킷
	sDBResponse_TradeMarket sendMsg;
	
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_TRADEMARKET;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->usCharID; 
	
	cltItem clItem, clDstOrderItem;
	clItem.Set(&pRecvMsg->clOrgOrder.clItemData);
	clItem.SetItemNum(pRecvMsg->clOrgOrder.siAmount);

	//[추가 : 황진성 2007. 10. 25 순수하게 거래한 아이템 정보]
	sendMsg.clTradeItem.Set(&clItem);
	sendMsg.siReason = pRecvMsg->siReason;

	// DB Query
	BEGIN_SP( "rsp_MarketTrade" )
		m_odbc.SetParam( SQL_INTEGER,	&(pRecvMsg->siVillageUnique),		sizeof(pRecvMsg->siVillageUnique) );
		m_odbc.SetParam( SQL_TINYINT,	&(pRecvMsg->bPileSwitch),			sizeof(pRecvMsg->bPileSwitch) );
		m_odbc.SetParam( SQL_SMALLINT,	&(pRecvMsg->clOrgOrder.siIndex),	sizeof(pRecvMsg->clOrgOrder.siIndex) );
		m_odbc.SetParam( SQL_SMALLINT,	&(pRecvMsg->clOrgOrder.siType),		sizeof(pRecvMsg->clOrgOrder.siType) );
		m_odbc.SetParam( SQL_INTEGER,	&(pRecvMsg->clOrgOrder.siPersonID),	sizeof(pRecvMsg->clOrgOrder.siPersonID) );
		m_odbc.SetParam( SQL_BIGINT,	&(pRecvMsg->clOrgOrder.siPrice),	sizeof(pRecvMsg->clOrgOrder.siPrice) );

		SetItemParam( &clItem);

		m_odbc.SetParam( SQL_INTEGER,	&(pRecvMsg->siDstOrderIndex),		sizeof(pRecvMsg->siDstOrderIndex) );
		m_odbc.SetParam( SQL_BIGINT,	&(pRecvMsg->siPrice),				sizeof(pRecvMsg->siPrice) );
		m_odbc.SetParam( SQL_SMALLINT,	&pRecvMsg->siItemPos,				sizeof(pRecvMsg->siItemPos) );
	END_SP()
	
	EXEC_SQL()
	
	BEGIN_FETCHDATA()
		m_odbc.GetData( &(sendMsg.siResult),				0);
		m_odbc.GetData( &(sendMsg.siPersonID),				0);
		m_odbc.GetData( &(sendMsg.siMoney),					0);
		m_odbc.GetData( &(sendMsg.siVillageUnique),			0);
		m_odbc.GetData( &(sendMsg.clDstOrder.siIndex),		0);		sendMsg.clDstOrder.siType = pRecvMsg->clOrgOrder.siType;
		m_odbc.GetData( &(sendMsg.clDstOrder.siPersonID),	0);
		m_odbc.GetData( &(sendMsg.clDstOrder.siPrice),		0);

		GetItemData( &clDstOrderItem);
		m_odbc.GetData( &(sendMsg.clDstOrder.siDate),		0);

		sendMsg.clDstOrder.clItemData.Set(&clDstOrderItem);
		sendMsg.clDstOrder.siAmount = clDstOrderItem.GetItemNum();

		m_odbc.GetData( &(sendMsg.siTradePrice),			0);
		m_odbc.GetData( &(sendMsg.siItemPos),				0);

		GetItemData( &sendMsg.clItem);

		m_odbc.GetData( &sendMsg.siSellerPersonID, 0);
		m_odbc.GetData( &sendMsg.siSellerBankMoney, 0);
		m_odbc.GetData( &sendMsg.siTradeItemNum, 0);
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );

	// 객주 계좌 정보를 보낸다.
	if( 0 < sendMsg.siSellerPersonID )
		SendMarketAccountInfo( sendMsg.siSellerPersonID, 0);

	// 로그 작성
	if(sendMsg.siResult == 1 || sendMsg.siResult == -4)
	{
		char log[512] = "";
		sprintf(log, "Result = %d, ItemPos = %d, TradeAmount = %d, AfterItemNum = %d, TradePrice = %I64d, OrderIndex = %d, LeftMoney = %I64d, SellerBankMoney = %I64d"
				, sendMsg.siResult, sendMsg.siItemPos, sendMsg.siTradeItemNum, sendMsg.clItem.siItemNum, sendMsg.siTradePrice, pRecvMsg->clOrgOrder.siIndex
				, sendMsg.siMoney, sendMsg.siSellerBankMoney
				);
		sDBLogMsg_QueryString LogMsg("Trade Market", sendMsg.siPersonID, sendMsg.siSellerPersonID, sendMsg.clItem.siUnique, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}


void DBGameProtocol::MarketAccount()
{
	// 받은 패킷
	sDBRequest_MarketAccount *pRecvMsg	= (sDBRequest_MarketAccount *)m_pPacket;

	
	// 객주 계좌 정보를 보낸다.
	if( 0 < pRecvMsg->siPersonID )
		SendMarketAccountInfo(pRecvMsg->siPersonID, pRecvMsg->usCharID);
}



void DBGameProtocol::GetItemFromMarket()
{
	// 받은 패킷
	sDBRequest_GetItemFromMarket *pRecvMsg	= (sDBRequest_GetItemFromMarket *)m_pPacket;
	
	// 보낼 패킷
	sDBResponse_GetItemFromMarket sendMsg;
	
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_GETITEMFROMMARKET;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->usCharID;

	//[추가 : 황진성 2007. 10. 25 회수하는 아이템 정보]
	sendMsg.clGetItem.Set(&pRecvMsg->clFromItem);

	
	// DB Query
	BEGIN_SP( "rsp_PersonItemGetFromMarket" )
		m_odbc.SetParam( SQL_INTEGER,	&(pRecvMsg->siPersonID),	sizeof(pRecvMsg->siPersonID) );
		m_odbc.SetParam( SQL_SMALLINT,	&(pRecvMsg->siFromPos),		sizeof(pRecvMsg->siFromPos) );
		m_odbc.SetParam( SQL_TINYINT,	&(pRecvMsg->bPileSwitch),	sizeof(pRecvMsg->bPileSwitch) );

		SetItemParam( &pRecvMsg->clFromItem);

		m_odbc.SetParam( SQL_SMALLINT,	&(pRecvMsg->siToPos),		sizeof(pRecvMsg->siToPos) );
	END_SP()

	EXEC_SQL()
	
	SI32 siDateVary = 0, siAmount = 0, siMarketIndex = 0;
	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult,					0);
		m_odbc.GetData( &sendMsg.siPersonID,				0);
		m_odbc.GetData( &sendMsg.siFromPos,					0);
		m_odbc.GetData( &sendMsg.clOrder.siPrice,			0);
		GetItemData( &sendMsg.clFromItem);
		m_odbc.GetData( &siDateVary, 0 );
		m_odbc.GetData( &sendMsg.siToPos,					0);
		GetItemData( &sendMsg.clToItem );
		m_odbc.GetData( &siAmount, 0);
		m_odbc.GetData( &siMarketIndex, 0);
	END_FETCHDATA()

	cltTradeOrder cltorder(TRADEORDER_TYPE_OBJECT, sendMsg.siPersonID, sendMsg.clOrder.siPrice, sendMsg.clFromItem.GetItemNum(), siDateVary, &sendMsg.clFromItem);
	cltorder.siIndex	= siMarketIndex;

	sendMsg.clOrder.Set(&cltorder);

	// send
	SendPacket( (BYTE *)&sendMsg );

	// 로그 작성
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "MarketIndex = %d, FromPos = %d, ToPos = %d, Amount = %d, AfterInvNum = %d"
				, siMarketIndex, sendMsg.siFromPos, sendMsg.siToPos, siAmount, sendMsg.clToItem.siItemNum
				);
		sDBLogMsg_QueryString LogMsg("Get Item From Market", sendMsg.siPersonID, NULL, pRecvMsg->clFromItem.siUnique, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}



void DBGameProtocol::ManyItemPrice()
{
	// 받은 패킷
	sDBRequest_ManyItemPrice *pRecvMsg	= (sDBRequest_ManyItemPrice *)m_pPacket;
	
	// 보낼 패킷
	sDBResponse_ManyItemPrice sendMsg;
	
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_MANYITEMPRICE;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
//	sendMsg.usCharID = pRecvMsg->usCharID;
	
	// DB Query
//	SI32 siManyItemPriceNumber = MANYITEM_PRICE_NUMBER;
	
	SI32 siItemUnique[MAX_ITEM_UNIQUE];
	GMONEY siCurrentItemPrice[MAX_ITEM_UNIQUE];
	REAL64 fCurrentItemPrice[MAX_ITEM_UNIQUE];
	SI32 siItemNum[MAX_ITEM_UNIQUE];
	
	ZeroMemory(siItemUnique, sizeof(siItemUnique));
	ZeroMemory(siCurrentItemPrice, sizeof(siItemUnique));
	ZeroMemory(siItemNum, sizeof(siItemNum));
	
	BEGIN_SP( "rsp_ItemPriceManyGet" )
	END_SP()

	EXEC_SQL()
	
	SI32 siCount = 0, i = 0;

	BEGIN_FETCHDATA()
		m_odbc.GetData( &(siItemUnique[siCount]),		0);
		m_odbc.GetData( &(siCurrentItemPrice[siCount]),	0);
		m_odbc.GetData( &(fCurrentItemPrice[siCount]),	0);
		m_odbc.GetData( &(siItemNum[siCount]),	0);
		siCount++;
	END_FETCHDATA()
	
	for( i = 0; i < ( (siCount - 1) / MANYITEM_PRICE_NUMBER) + 1; i++)
	{
		sendMsg.siResult = 1;
		memcpy(sendMsg.siItemUnique, &siItemUnique[i * MANYITEM_PRICE_NUMBER], sizeof(sendMsg.siItemUnique));
		memcpy(sendMsg.siCurrentItemPrice, &siCurrentItemPrice[i * MANYITEM_PRICE_NUMBER], sizeof(sendMsg.siCurrentItemPrice));
		memcpy(sendMsg.fPrice, &fCurrentItemPrice[i * MANYITEM_PRICE_NUMBER], sizeof(sendMsg.fPrice));
		memcpy(sendMsg.siItemNum, &siItemNum[i * MANYITEM_PRICE_NUMBER], sizeof(sendMsg.siItemNum));

		// send
		SendPacket( (BYTE *)&sendMsg );
	}

	// 보낼 패킷
	sDBResponse_ManyItemPrice lastMsg;
	lastMsg.packetHeader.usCmd = DBMSG_RESPONSE_MANYITEMPRICE;
	lastMsg.packetHeader.usSize = sizeof( lastMsg );

	lastMsg.siResult = -1;

	// send
	SendPacket( (BYTE *)&lastMsg );
}


void DBGameProtocol::SetItemPrice()
{
	// 받은 패킷
	sDBRequest_SetItemPrice *pRecvMsg	= (sDBRequest_SetItemPrice *)m_pPacket;
	
	// 보낼 패킷
	sDBResponse_SetItemPrice sendMsg;
	
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_SETITEMPRICE;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
//	sendMsg.usCharID = pRecvMsg->usCharID;
	
	// DB Query
	BEGIN_SP( "rsp_ItemPriceSet" )
		m_odbc.SetParam( SQL_INTEGER, &(pRecvMsg->siItemUnique),	sizeof(pRecvMsg->siItemUnique) );
		m_odbc.SetParam( SQL_BIGINT, &(pRecvMsg->siPrice),			sizeof(pRecvMsg->siPrice) );
		m_odbc.SetParam( SQL_FLOAT, &(pRecvMsg->fPrice),			sizeof(pRecvMsg->fPrice) );
		m_odbc.SetParam( SQL_INTEGER, &(pRecvMsg->siItemNum),		sizeof(pRecvMsg->siItemNum) );
		m_odbc.SetParam( SQL_VARCHAR, (pRecvMsg->szItemName),		sizeof(pRecvMsg->szItemName) );
	END_SP()

	EXEC_SQL()
	
	BEGIN_FETCHDATA()
		m_odbc.GetData( &(sendMsg.siResult), 0 );
	END_FETCHDATA()
	
	// send
	SendPacket( (BYTE *)&sendMsg );

	// 로그 작성
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "ItemPrice = %I64d, fPrice = %f, ItemNum = %d"
				, pRecvMsg->siPrice, pRecvMsg->fPrice, pRecvMsg->siItemNum
				);
		sDBLogMsg_QueryString LogMsg("Item Price Set", 0, NULL, pRecvMsg->siItemUnique, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

void DBGameProtocol::ReduceOrderPrice()
{
	// 받은 패킷
	sDBRequest_ReduceOrderPrice *pRecvMsg	= (sDBRequest_ReduceOrderPrice *)m_pPacket;
	
	// 보낼 패킷
	sDBResponse_ReduceOrderPrice sendMsg;
	
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_REDUCEORDERPRICE;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
//	sendMsg.usCharID = pRecvMsg->usCharID;
	sendMsg.siPrice = pRecvMsg->siPrice;

	cltItem clItem;
	clItem.Set(&pRecvMsg->clOrder.clItemData);
	clItem.SetItemNum(pRecvMsg->clOrder.siAmount);

	// DB Query
	BEGIN_SP( "rsp_MarketOrderPriceReduce" )
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->clOrder.siIndex,		sizeof(pRecvMsg->clOrder.siIndex) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->clOrder.siPersonID,	sizeof(pRecvMsg->clOrder.siPersonID) );
		m_odbc.SetParam( SQL_BIGINT, &pRecvMsg->clOrder.siPrice,		sizeof(pRecvMsg->clOrder.siPrice) );
		SetItemParam( &clItem );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->clOrder.siDate,		sizeof(pRecvMsg->clOrder.siDate) );
	END_SP()

	EXEC_SQL()

	ZeroMemory( &clItem, sizeof(clItem));
	sendMsg.siResult = 0; // 혹시 모르니 미리 0으로...

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0);
		m_odbc.GetData( &sendMsg.siMarketIndex, 0);
		m_odbc.GetData( &sendMsg.clOrder.siIndex, 0 );
		m_odbc.GetData( &sendMsg.clOrder.siPersonID, 0 );
		m_odbc.GetData( &sendMsg.clOrder.siPrice, 0 );
		GetItemData( &clItem );
		m_odbc.GetData( &sendMsg.clOrder.siDate, 0 );
	END_FETCHDATA()

	sendMsg.clOrder.clItemData.Set(&clItem);
	sendMsg.clOrder.siType		= pRecvMsg->clOrder.siType;
	sendMsg.clOrder.siAmount	= clItem.GetItemNum();

	// send
	SendPacket( (BYTE *)&sendMsg );

	// 로그 작성
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "MarketIndex = %d, OrderIndex = %d, ItemPrice = %I64d, ItemNum = %d"
				, sendMsg.clOrder.siIndex, sendMsg.siMarketIndex, sendMsg.clOrder.siPrice, clItem.siItemNum
				);
		sDBLogMsg_QueryString LogMsg("Reduce Order Price", sendMsg.clOrder.siPersonID, NULL, clItem.siUnique, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

void DBGameProtocol::SetMarketFee()
{
	// 받은 패킷
	sDBRequest_SetMarketFee *pRecvMsg	= (sDBRequest_SetMarketFee *)m_pPacket;
	
	// 보낼 패킷
	sDBResponse_SetMarketFee sendMsg;
	
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_SETMARKETFEE;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->usCharID;
	
	cltDBFee clDBFee( &pRecvMsg->clFee );

	// DB Query
	BEGIN_SP( "rsp_MarketFeeSet" )
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
void DBGameProtocol::SetForeignMerchantNowMoney()
{
	// 받은 패킷
	sDBRequest_SetForeignMerchantNowMoney *pRecvMsg	= (sDBRequest_SetForeignMerchantNowMoney *)m_pPacket;

	// 보낼 패킷
	sDBResponse_SetForeignMerchantNowMoney sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_SET_FOREIGNMERCHANTNOWMONEY;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	//	sendMsg.siCharID = pRecvMsg->siCharID;

	// DB Query
	BEGIN_SP( "rsp_ForeignMerchantNpcNowMoneySet" )
		//m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->gPrice, sizeof(pRecvMsg->gPrice) );
		m_odbc.SetParam( SQL_BIGINT, &pRecvMsg->gPrice, sizeof(pRecvMsg->gPrice) );
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
	m_odbc.GetData( &sendMsg.siResult, 0 );
	END_FETCHDATA()

		// send
		SendPacket( (BYTE *)&sendMsg );
}
void DBGameProtocol::SetForeignMerchantBuyLimit()
{
	// 받은 패킷
	sDBRequest_SetForeignMerchantBuyLimit *pRecvMsg	= (sDBRequest_SetForeignMerchantBuyLimit *)m_pPacket;

	// 보낼 패킷
	sDBResponse_SetForeignMerchantBuyLimit sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_SET_FOREIGNMERCHANTBUYLIMIT;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	//	sendMsg.siCharID = pRecvMsg->siCharID;

	// DB Query
	BEGIN_SP( "rsp_ForeignMerchantNpcLimitMoneySet" )
		m_odbc.SetParam( SQL_BIGINT, &pRecvMsg->gPrice, sizeof(pRecvMsg->gPrice) );
		m_odbc.SetParam( SQL_REAL, &pRecvMsg->fUpPercent, sizeof(pRecvMsg->fUpPercent) );
		m_odbc.SetParam( SQL_REAL, &pRecvMsg->fDownPercent, sizeof(pRecvMsg->fDownPercent) );
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
	m_odbc.GetData( &sendMsg.siResult, 0 );
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );
}
void DBGameProtocol::GetForeignMerchantBuyLimit()
{
	// 받은 패킷
	sDBRequest_GetForeignMerchantBuyLimit *pRecvMsg = (sDBRequest_GetForeignMerchantBuyLimit *)m_pPacket;

	// 보낼 패킷
	sDBResponse_GetForeignMerchantBuyLimit sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_GET_FOREIGNMERCHANTBUYLIMIT;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	//	sendMsg.usCharID = pRecvMsg->usCharID;

	// DB Query
	BEGIN_SP( "rsp_ForeignMerchantNpcLimitMoneyGet" )
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
	m_odbc.GetData(&sendMsg.siResult);
	m_odbc.GetData(&sendMsg.gPrice);
	m_odbc.GetData(&sendMsg.fUpPercent);
	m_odbc.GetData(&sendMsg.fDownPercent);
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );
}

void DBGameProtocol::GetForeignMerchantNowMoney()
{
	// 받은 패킷
	sDBRequest_GetForeignMerchantNowMoney *pRecvMsg = (sDBRequest_GetForeignMerchantNowMoney *)m_pPacket;

	// 보낼 패킷
	sDBResponse_GetForeignMerchantNowMoney sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_GET_FOREIGNMERCHANTNOWMONEY;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	//	sendMsg.usCharID = pRecvMsg->usCharID;

	// DB Query
	BEGIN_SP( "rsp_ForeignMerchantNpcNowMoneyGet" )
		END_SP()

		EXEC_SQL()

		BEGIN_FETCHDATA()
		m_odbc.GetData( &(sendMsg.siResult), 0);
		m_odbc.GetData( &(sendMsg.gPrice), 0);
		END_FETCHDATA()

		// send
		SendPacket( (BYTE *)&sendMsg );
}

void DBGameProtocol::SetSystemBuyItemNowMoney()
{
	// 받은 패킷
	sDBRequest_SetSystemBuyItemNowMoney *pRecvMsg = (sDBRequest_SetSystemBuyItemNowMoney *)m_pPacket;

	// 보낼 패킷
	sDBResponse_SetSystemBuyItemNowMoney sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_SET_SYSTEMBUYITEMNOWMONEY;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	//	sendMsg.usCharID = pRecvMsg->usCharID;

	// DB Query
	BEGIN_SP( "rsp_SystemBuyItemNowMoneySet" )
		m_odbc.SetParam( SQL_BIGINT, &pRecvMsg->gPrice, sizeof(pRecvMsg->gPrice) );
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &(sendMsg.siResult), 0);
	END_FETCHDATA()

	sendMsg.gPrice = pRecvMsg->gPrice;

	// send
	SendPacket( (BYTE *)&sendMsg );
}

void DBGameProtocol::GetSystemBuyItemNowMoney()
{
	// 받은 패킷
	sDBRequest_GetSystemBuyItemNowMoney *pRecvMsg = (sDBRequest_GetSystemBuyItemNowMoney *)m_pPacket;

	// 보낼 패킷
	sDBResponse_GetSystemBuyItemNowMoney sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_GET_SYSTEMBUYITEMNOWMONEY;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	//	sendMsg.usCharID = pRecvMsg->usCharID;

	// DB Query
	BEGIN_SP( "rsp_SystemBuyItemNowMoneyGet" )
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &(sendMsg.siResult), 0);
		m_odbc.GetData( &(sendMsg.gPrice), 0);
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );
}

void DBGameProtocol::NewOrderMarket()
{
	// 받은 패킷
	sDBRequest_NewOrderMarket *pRecvMsg = (sDBRequest_NewOrderMarket *)m_pPacket;

	cltItem clItem;
	clItem.Set(&pRecvMsg->clOrder.clItemData);
	clItem.SetItemNum(pRecvMsg->clOrder.siAmount);

	// 보낼 패킷
	sDBResponse_NewOrderMarket sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_NEWORDERMARKET;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->usCharID;
	sendMsg.clOrder = pRecvMsg->clOrder;

	// DB Query
	BEGIN_SP( "rsp_UnionMarketOrderSet" )
		m_odbc.SetParam( SQL_INTEGER, &(pRecvMsg->clOrder.siPersonID),	sizeof(pRecvMsg->clOrder.siPersonID) );
		m_odbc.SetParam( SQL_SMALLINT, &(pRecvMsg->clOrder.siServerUnique),		sizeof(pRecvMsg->clOrder.siServerUnique) );
		m_odbc.SetParam( SQL_INTEGER, &(pRecvMsg->siVillageUnique),		sizeof(pRecvMsg->siVillageUnique) );
		m_odbc.SetParam( SQL_BIGINT, &(pRecvMsg->clOrder.siPrice),		sizeof(pRecvMsg->clOrder.siPrice) );
		SetItemParam( &clItem );
		m_odbc.SetParam( SQL_INTEGER, &(pRecvMsg->clOrder.siDate),		sizeof(pRecvMsg->clOrder.siDate) );
		m_odbc.SetParam( SQL_SMALLINT, &(pRecvMsg->siItemPos),			sizeof(pRecvMsg->siItemPos) );
		m_odbc.SetParam( SQL_TINYINT, &(pRecvMsg->bPileSwitch),			sizeof(pRecvMsg->bPileSwitch) );
		m_odbc.SetParam( SQL_TINYINT, &(pRecvMsg->uiOrderMode),			sizeof(pRecvMsg->uiOrderMode) );			// 주문모드(0:사용자, 1:시스템)
	END_SP()

	EXEC_SQL()
	BEGIN_FETCHDATA()
		m_odbc.GetData( &(sendMsg.siResult), 0);
		m_odbc.GetData( &(sendMsg.clOrder.siOrderIndex), 0);
	END_FETCHDATA()

	BEGIN_NEXTRECORDSET()
	BEGIN_FETCHDATA()
		GetItemData( &sendMsg.clFromItem);
	END_FETCHDATA()
	END_NEXTRECORDSET()

	sendMsg.siItemFromPos		= pRecvMsg->siItemPos;
	sendMsg.clOrder.siPersonID	= pRecvMsg->clOrder.siPersonID;
	sendMsg.clOrder.clItemData.Set(&pRecvMsg->clOrder.clItemData);
	sendMsg.siTax				= pRecvMsg->siTax;

	// send
	SendPacket( (BYTE *)&sendMsg );

	// 객주 계좌 정보를 보낸다.
	if( 1 == sendMsg.siResult )
	{
		SendUnionMarketAccountInfo( pRecvMsg->clOrder.siPersonID, sendMsg.usCharID,pRecvMsg->clOrder.siServerUnique);
	}

	//// 로그 작성
	//if( sendMsg.siResult == 1)
	//{
	//	char log[512] = "";
	//	sprintf(log, "VillageUnique = %d, FromInvPos = %d, MarketIndex = %d, Amount = %d, Price = %I64d"
	//		, pRecvMsg->siVillageUnique, pRecvMsg->siItemPos, sendMsg.clOrder.siOrderIndex, pRecvMsg->clOrder.siAmount, pRecvMsg->clOrder.siPrice
	//		);
	//	sDBLogMsg_QueryString LogMsg("Set UnionMarket Order", sendMsg.clOrder.siPersonID, NULL, 0, pRecvMsg->clOrder.clItemData.siUnique, log);
	//	SendLogMsg((sPacketHeader *)&LogMsg);
	//}
}

void DBGameProtocol::UnionMarketAccount()
{
	// 받은 패킷
	sDBRequest_NewMarketAccount *pRecvMsg	= (sDBRequest_NewMarketAccount *)m_pPacket;


	// 객주 계좌 정보를 보낸다.
	if(pRecvMsg->siPersonID != 0)
	{
		SendUnionMarketAccountInfo(pRecvMsg->siPersonID, pRecvMsg->usCharID,pRecvMsg->siServerUnique);
	}
}

void DBGameProtocol::TradeUnionMarket()
{
	// 받은 패킷
	sDBRequest_NewTradeMarket *pRecvMsg	= (sDBRequest_NewTradeMarket *)m_pPacket;

	// 보낼 패킷
	sDBResponse_NewTradeMarket sendMsg;

	sendMsg.packetHeader.usCmd	= DBMSG_RESPONSE_NEWTRADEMARKET;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID			= pRecvMsg->usCharID;
	sendMsg.siBuyerPersonID		= pRecvMsg->siBuyerPersonID;
	sendMsg.siVillageUnique		= pRecvMsg->siServerUnique;
	sendMsg.siItemPos			= pRecvMsg->siItemPos;

	TIMESTAMP_STRUCT stTS;
	Convert_TIMESTAMP_from_SYSTEMTIME( &stTS, &pRecvMsg->stRequestTime);

	cltItem clItem;
	clItem.Set(&pRecvMsg->clOrgOrder.clItemData);
	clItem.SetItemNum(pRecvMsg->clOrgOrder.siAmount);

	//[추가 : 황진성 2007. 10. 25 순수하게 거래한 아이템 정보]
	sendMsg.clTradeItem.Set(&clItem);

	SI32 CreateSID = 0;

	// DB Query
	BEGIN_SP( "rsp_UnionMarketTrade" )
		m_odbc.SetParam( SQL_INTEGER,	&(pRecvMsg->clOrgOrder.siOrderIndex),	sizeof(pRecvMsg->clOrgOrder.siOrderIndex)	);
		m_odbc.SetParam( SQL_INTEGER,	&(pRecvMsg->siBuyerPersonID),			sizeof(pRecvMsg->siBuyerPersonID)			);
		m_odbc.SetParam( SQL_SMALLINT,	&(pRecvMsg->clOrgOrder.siServerUnique),	sizeof(pRecvMsg->clOrgOrder.siServerUnique) );
		m_odbc.SetParam( SQL_INTEGER,	&(pRecvMsg->siVillageUnique),			sizeof(pRecvMsg->siVillageUnique)			);
		m_odbc.SetParam( SQL_TINYINT,	&(pRecvMsg->bPileSwitch),				sizeof(pRecvMsg->bPileSwitch)				);
		m_odbc.SetParam( SQL_BIGINT,	&(pRecvMsg->clOrgOrder.siPrice),		sizeof(pRecvMsg->clOrgOrder.siPrice)		);
		SetItemParam( &clItem );
		m_odbc.SetParam( SQL_BIGINT,	&(pRecvMsg->siPrice),					sizeof(pRecvMsg->siPrice)					);
		m_odbc.SetParam( SQL_SMALLINT,	&pRecvMsg->siItemPos,					sizeof(pRecvMsg->siItemPos)					);
		m_odbc.SetParam( SQL_BIT,		&pRecvMsg->m_bCreateSID,				sizeof(pRecvMsg->m_bCreateSID)				);
		m_odbc.SetParam( SQL_TIMESTAMP, &stTS,									sizeof(stTS)								);
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult,		0);
		m_odbc.GetData( &sendMsg.siMoney,		0);
		m_odbc.GetData( &sendMsg.siTradePrice,	0);

		GetItemData( &sendMsg.clItem );

		m_odbc.GetData( &sendMsg.m_bSystem,				0);
		m_odbc.GetData( &sendMsg.siSellerPersonID,		0);
		m_odbc.GetData( &sendMsg.siSellerServerUnique,	0);
		m_odbc.GetData( &sendMsg.siTradeItemNum,		0);	
		m_odbc.GetData( &sendMsg.fPrice,				0);	
	END_FETCHDATA()


	BEGIN_NEXTRECORDSET()
	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siSellerBankMoney, 0);
	END_FETCHDATA()
	END_NEXTRECORDSET()

	// send
	SendPacket( (BYTE *)&sendMsg );

	// 객주 계좌 정보를 보낸다.
	if(sendMsg.siSellerPersonID != 0)
	{
		SendUnionMarketAccountInfo(sendMsg.siSellerPersonID, pRecvMsg->usCharID,sendMsg.siSellerServerUnique);
	}
}

void DBGameProtocol::UnionMarketItemList()
{
	// 받은 페킷
	sDBRequest_NewMarketItemList *pRecvMsg	= (sDBRequest_NewMarketItemList *)m_pPacket;

	// 보낼 패킷
	sDBResponse_NewMarketItemList sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_NEWMARKETITEMLIST;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->usCharID;
	sendMsg.uiType = pRecvMsg->uiType;
	sendMsg.siItemUnique = pRecvMsg->siItemUnique;

	// DB Query
	BEGIN_SP( "rsp_UnionMarketItemListGet" )
		m_odbc.SetParam( SQL_INTEGER,	&(pRecvMsg->siItemUnique),	sizeof(pRecvMsg->siItemUnique) );
		m_odbc.SetParam( SQL_SMALLINT,	&(pRecvMsg->siRequestPage),		sizeof(pRecvMsg->siRequestPage) );
		m_odbc.SetParam( SQL_TINYINT,	&(pRecvMsg->uiType),		sizeof(pRecvMsg->uiType) );
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &(sendMsg.siResult), 0);
		m_odbc.GetData( &(sendMsg.siMaxPage), 0);
		m_odbc.GetData( &(sendMsg.siCurrentPrice), 0);
		m_odbc.GetData( &(sendMsg.fCurrentPrice), 0);
	END_FETCHDATA()

	SI16 count = 0;
	BEGIN_NEXTRECORDSET()
	BEGIN_FETCHDATA()
		if(count > MAX_NEWMARKET_ITEMLIST)
			break;

		GetItemData( &sendMsg.clNewTradeOrder[count].clItemData);

		m_odbc.GetData( &sendMsg.clNewTradeOrder[count].siServerUnique , 0);
		m_odbc.GetData( &sendMsg.clNewTradeOrder[count].siOrderIndex , 0);
		m_odbc.GetData( &sendMsg.clNewTradeOrder[count].siPersonID , 0);
		m_odbc.GetData( &sendMsg.clNewTradeOrder[count].siDate , 0);
		m_odbc.GetData( &sendMsg.clNewTradeOrder[count].siPrice , 0);
		m_odbc.GetData( &sendMsg.clNewTradeOrder[count].siAmount , 0);
		count++;

	END_FETCHDATA()
	END_NEXTRECORDSET()


	// send
	SendPacket( (BYTE *)&sendMsg );
}



void DBGameProtocol::GetItemFromUnionMarket()
{
	// 받은 패킷
	sDBRequest_GetItemFromNewMarket *pRecvMsg	= (sDBRequest_GetItemFromNewMarket *)m_pPacket;

	// 보낼 패킷
	sDBResponse_GetItemFromNewMarket sendMsg;

	sendMsg.packetHeader.usCmd	= DBMSG_RESPONSE_GETITEMFROMNEWMARKET;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID			= pRecvMsg->usCharID;

	//[추가 : 황진성 2007. 10. 25 회수하는 아이템 정보]
	sendMsg.clGetItem.Set(&pRecvMsg->clFromItem);

	// DB Query
	BEGIN_SP( "rsp_UnionMarketItemGet" )
		m_odbc.SetParam( SQL_INTEGER,	&(pRecvMsg->siPersonID),			sizeof(pRecvMsg->siPersonID)			);
		m_odbc.SetParam( SQL_INTEGER,	&(pRecvMsg->siServerUnique),		sizeof(pRecvMsg->siServerUnique)		);
		m_odbc.SetParam( SQL_INTEGER,	&(pRecvMsg->siOrderIndex),			sizeof(pRecvMsg->siOrderIndex)			);
		m_odbc.SetParam( SQL_TINYINT,	&(pRecvMsg->bPileSwitch),			sizeof(pRecvMsg->bPileSwitch)			);
		m_odbc.SetParam( SQL_SMALLINT,	&(pRecvMsg->siToPos),				sizeof(pRecvMsg->siToPos)				);
		m_odbc.SetParam( SQL_SMALLINT,	&(pRecvMsg->clFromItem.siUnique),	sizeof(pRecvMsg->clFromItem.siUnique)	);
		m_odbc.SetParam( SQL_SMALLINT,	&(pRecvMsg->clFromItem.siItemNum),	sizeof(pRecvMsg->clFromItem.siItemNum)	);
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult,		0 );
		m_odbc.GetData( &sendMsg.siPersonID,	0 );
		m_odbc.GetData( &sendMsg.siToPos,		0 );
		GetItemData( &sendMsg.clToItem );
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );
}


void DBGameProtocol::UnionMarketResult()
{
	// 받은 패킷
	sDBRequest_NewMarketResult *pRecvMsg	= (sDBRequest_NewMarketResult *)m_pPacket;

	// 보낼 패킷
	sDBResponse_NewMarketResult sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_NEWMARKETRESULT;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );


	BEGIN_SP( "rsp_UnionMarketLogGet" )
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siServerUnique, sizeof(pRecvMsg->siServerUnique) );
	END_SP()
		
	EXEC_SQL()
	
	SI16 count = 0;
	BEGIN_FETCHDATA()
		if( count >= MAX_NEWMARKET_RESULTLIST )
			break;
		m_odbc.GetData( &sendMsg.siResult, 0);
		m_odbc.GetData( &sendMsg.stSellerInfo[count].siSellerPersonID,	0);
		m_odbc.GetData( &sendMsg.stSellerInfo[count].siSellItemUnique,	0);
		m_odbc.GetData( &sendMsg.stSellerInfo[count].siSellPrice,		0);
		m_odbc.GetData( &sendMsg.stSellerInfo[count].siSellItemNum,		0);
		m_odbc.GetData( &sendMsg.stSellerInfo[count].siVillageUnique,	0);
		count++;
	END_FETCHDATA()


	SendPacket( (BYTE *)&sendMsg );
}


void DBGameProtocol::UnionMarketInputBankMoney()
{
	// 받은 패킷
	sDBRequest_NewMarket_IncreaseBankMoney *pRecvMsg	= (sDBRequest_NewMarket_IncreaseBankMoney *)m_pPacket;

	// 보낼 패킷
	sDBResponse_NewMarket_IncreaseBankMoney sendMsg;

	sendMsg.packetHeader.usCmd	= DBMSG_RESPONSE_NEWMARKET_INCREASEBANKMONEY;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.siPersonID			= pRecvMsg->siPersonID;

	BEGIN_SP( "rsp_UnionMarketInputBankMoney" )
		m_odbc.SetParam( SQL_INTEGER,	&(pRecvMsg->siPersonID), sizeof(pRecvMsg->siPersonID) );
		m_odbc.SetParam( SQL_BIGINT,	&(pRecvMsg->siIncreaseMoney), sizeof(pRecvMsg->siIncreaseMoney) );
	END_SP()

	EXEC_SQL()
	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0);
		m_odbc.GetData( &sendMsg.clTotalMoney , 0);
		m_odbc.GetData( &sendMsg.clTotalInputMoney, 0);
		m_odbc.GetData( &sendMsg.clCurrentLeftMoney, 0);
		m_odbc.GetData( &sendMsg.siVillageUnique, 0 );
	END_FETCHDATA()

	SendPacket( (BYTE *)&sendMsg );
}


void DBGameProtocol::SetUinonMarketFee()
{
	// 받은 패킷
	sDBRequest_SetNewMarketFee *pRecvMsg	= (sDBRequest_SetNewMarketFee *)m_pPacket;

	// 보낼 패킷
	sDBResponse_SetNewMarketFee sendMsg;

	sendMsg.packetHeader.usCmd	= DBMSG_RESPONSE_SETNEWMARKETFEE;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.siCharID			= pRecvMsg->siCharID;

	// DB Query
	BEGIN_SP( "rsp_UnionMarketFeeSet" )
		m_odbc.SetParam( SQL_INTEGER, &(pRecvMsg->siPersonID),		sizeof(pRecvMsg->siPersonID) );
		m_odbc.SetParam( SQL_INTEGER, &(pRecvMsg->siVillageUnique),	sizeof(pRecvMsg->siVillageUnique) );
		SetFeeInfo( (const class cltDBFee *)&pRecvMsg->clFee );
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &(sendMsg.siResult),		0 );
		m_odbc.GetData( &(sendMsg.siPersonID),		0 );
		m_odbc.GetData( &(sendMsg.siVillageUnique), 0 );
		GetFeeInfoNew( &sendMsg.clFee );
	END_FETCHDATA()

	/*sendMsg.siResult		= 1;
	sendMsg.siPersonID		= pRecvMsg->siPersonID;
	sendMsg.siVillageUnique = pRecvMsg->siVillageUnique;
	sendMsg.clFee			= pRecvMsg->clFee;*/
	

	// send
	SendPacket( (BYTE *)&sendMsg );

}


void DBGameProtocol::ManyNewItemPrice()
{
	// 받은 패킷
	sDBRequest_ManyItemNewPrice *pRecvMsg	= (sDBRequest_ManyItemNewPrice *)m_pPacket;

	// 보낼 패킷
	sDBResponse_ManyItemNewPrice sendMsg;

	sendMsg.packetHeader.usCmd  = DBMSG_RESPONSE_MANYITEMNEWPRICE;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );

	SI16 siItemUnique[MAX_ITEM_UNIQUE];
	SI64 siCurrentItemPrice[MAX_ITEM_UNIQUE];
	float fCurrentItemPrice[MAX_ITEM_UNIQUE];
	SI32 siItemNum[MAX_ITEM_UNIQUE];

	ZeroMemory(siItemUnique, sizeof(siItemUnique));
	ZeroMemory(siCurrentItemPrice, sizeof(siItemUnique));
	ZeroMemory(siItemNum, sizeof(siItemNum));

	TIMESTAMP_STRUCT stTS;
	Convert_TIMESTAMP_from_SYSTEMTIME( &stTS, &pRecvMsg->stRequestTime);

	BEGIN_SP( "rsp_UnionMarketItemPriceGet" )
		m_odbc.SetParam( SQL_TIMESTAMP, &stTS, sizeof(stTS) );
	END_SP()

	EXEC_SQL()

	SI32 siCount = 0, i = 0;
	BEGIN_FETCHDATA()
		m_odbc.GetData( &(siItemUnique[siCount]),		0);
		m_odbc.GetData( &(siCurrentItemPrice[siCount]),	0);
		m_odbc.GetData( &(fCurrentItemPrice[siCount]),	0);
		m_odbc.GetData( &(siItemNum[siCount]),	0);
		siCount++;
	END_FETCHDATA()

	for( i = 0; i < ( (siCount - 1) / MANYITEM_PRICE_NUMBER) + 1; i++)
	{
		sendMsg.siResult = 1;
		memcpy(sendMsg.siItemUnique, &siItemUnique[i * MANYITEM_PRICE_NUMBER], sizeof(sendMsg.siItemUnique));
		memcpy(sendMsg.siCurrentItemPrice, &siCurrentItemPrice[i * MANYITEM_PRICE_NUMBER], sizeof(sendMsg.siCurrentItemPrice));
		memcpy(sendMsg.fPrice, &fCurrentItemPrice[i * MANYITEM_PRICE_NUMBER], sizeof(sendMsg.fPrice));
		memcpy(sendMsg.siItemNum, &siItemNum[i * MANYITEM_PRICE_NUMBER], sizeof(sendMsg.siItemNum));

		// send
		SendPacket( (BYTE *)&sendMsg );
	}


	sDBResponse_ManyItemNewPrice_Completed sendMsgCompledted;
	sendMsgCompledted.packetHeader.usCmd = DBMSG_RESPONSE_MANYITEMNEWPRICE_COMPLETED;
	sendMsgCompledted.packetHeader.usSize = sizeof( sendMsgCompledted );

	sendMsgCompledted.siResult	= 1;

	// send
	SendPacket( (BYTE *)&sendMsgCompledted );
}



void DBGameProtocol::SetNewItemPrice()
{
	// 받은 패킷
	sDBRequest_SetItemNewPrice *pRecvMsg	= (sDBRequest_SetItemNewPrice *)m_pPacket;

	// 보낼 패킷
	sDBResponse_SetItemNewPrice sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_SETITEMNEWPRICE;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );

	// DB Query
	BEGIN_SP( "rsp_UnionMarketItemPriceSet" )
		m_odbc.SetParam( SQL_SMALLINT,	&(pRecvMsg->siItemUnique),	sizeof(pRecvMsg->siItemUnique)	);
		m_odbc.SetParam( SQL_BIGINT,	&(pRecvMsg->siPrice),		sizeof(pRecvMsg->siPrice)		);
		m_odbc.SetParam( SQL_REAL,		&(pRecvMsg->fPrice),		sizeof(pRecvMsg->fPrice)		);
		m_odbc.SetParam( SQL_INTEGER,	&(pRecvMsg->siItemNum),		sizeof(pRecvMsg->siItemNum)		);
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &(sendMsg.siResult), 0 );
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );
}


void DBGameProtocol::NewMarketReducePriceNotice()
{
	// 받은 패킷
	sDBRequest_NewMarketReducePriceNotice *pRecvMsg	= (sDBRequest_NewMarketReducePriceNotice *)m_pPacket;

	// 보낼 패킷
	sDBResponse_NewMarketReducePriceNotice sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_NEWMARKETREDUCEPRICENOTICE;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );

	TIMESTAMP_STRUCT stTS;
	Convert_TIMESTAMP_from_SYSTEMTIME( &stTS, &pRecvMsg->stRequestTime);

	// DB Query
	BEGIN_SP( "rsp_UnionMarketReducePriceNotice" )
		m_odbc.SetParam( SQL_SMALLINT, &(pRecvMsg->siServerUnique),	sizeof(pRecvMsg->siServerUnique) );
		m_odbc.SetParam( SQL_TIMESTAMP, &stTS, sizeof(stTS) );
	END_SP()

	EXEC_SQL()

	SI32 siCount = 0, i = 0;
	BEGIN_FETCHDATA()
		if( siCount >= MAX_REDUCEPRICE_USERLIST )
		{
			// send
			SendPacket( (BYTE *)&sendMsg );
			siCount = 0;
		}

		m_odbc.GetData( &( sendMsg.stData[ siCount ].siPersonID ),		0 );
		m_odbc.GetData( &( sendMsg.stData[ siCount ].siItemUnique ),	0 );
		m_odbc.GetData( &( sendMsg.stData[ siCount ].siItemQty ),		0 );
		m_odbc.GetData( &( sendMsg.stData[ siCount ].clPrice ),			0 );		

		siCount++;

	END_FETCHDATA()

	sendMsg.siResult = 1;

	if(siCount != 0)
		SendPacket( (BYTE *)&sendMsg );	
}


void DBGameProtocol::GetNewMarketInfo()
{

	// 받은 패킷
	sDBRequest_GetStrInfo *pRecvMsg = (sDBRequest_GetStrInfo *)m_pPacket;

	SendNewMarketInfo(pRecvMsg->siVillageUnique, pRecvMsg->usCharID);

}

void DBGameProtocol::SendNewMarketInfo( SI32 siVillageUnique, UI16 usCharID )
{
	// 받은 패킷
	sDBRequest_GetStrInfo *pRecvMsg = (sDBRequest_GetStrInfo *)m_pPacket;

	// 보낼 패킷
	sDBResponse_GetStrInfo sendMsg;
	sendMsg.clStrInfo.clCommonInfo.siStrType = RANKTYPE_NEWMARKET;
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_GETSTRINFO;
	sendMsg.packetHeader.usSize = sendMsg.GetTotalSize();
	sendMsg.usCharID = usCharID;


	// DB Query
	BEGIN_SP( "rsp_UnionMarketInfoGet" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siVillageUnique, sizeof(pRecvMsg->siVillageUnique) );
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		GetFeeInfoNew( &sendMsg.clStrInfo.clNewMarketStrInfo.clFee );
		m_odbc.GetData( &sendMsg.clStrInfo.clCommonInfo.clMoney.itMoney, 0);
		m_odbc.GetData( &sendMsg.clStrInfo.clCommonInfo.strNotice, sizeof(sendMsg.clStrInfo.clCommonInfo.strNotice) );
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