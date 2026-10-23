#include "DBGameProtocol.h"
//#include "DBMsg.h"
#include "DBMsg-ForeignMerchantNPC.h"
#include "DBMsg-TradeMerchant.h"

void DBGameProtocol::SetForeignMerchantNPCInfo()
{
	// 받은 패킷
	sDBRequest_SetForeignMerchantNPCInfo *pRecvMsg	= (sDBRequest_SetForeignMerchantNPCInfo *)m_pPacket;
	
	// 보낼 패킷
	sDBResponse_SetForeignMerchantNPCInfo sendMsg;
	
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_PERSONTOTALINFOCREATE;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
//	sendMsg.usCharID = pRecvMsg->usCharID;
	
	cltGameDate clGameDate(&pRecvMsg->clDate);

	// DB Query
	BEGIN_SP( "rsp_ForeignMerchantNPCInfoSet" )
		m_odbc.SetParam( SQL_SMALLINT, &(pRecvMsg->siForeignMerchantKind), sizeof(pRecvMsg->siForeignMerchantKind) );
		m_odbc.SetParam( SQL_SMALLINT, &(pRecvMsg->siItemUnique), sizeof(pRecvMsg->siItemUnique) );
		m_odbc.SetParam( SQL_BIGINT, &(pRecvMsg->siPrice), sizeof(pRecvMsg->siPrice) );
		m_odbc.SetParam( SQL_INTEGER, &(pRecvMsg->siRestNum), sizeof(pRecvMsg->siRestNum) );
		SetGameDate( &clGameDate );
	END_SP()

	EXEC_SQL()
	
	BEGIN_FETCHDATA()
		m_odbc.GetData( &(sendMsg.siResult), 0 );
	END_FETCHDATA()
	
	// send
	SendPacket( (BYTE *)&sendMsg );
}

void DBGameProtocol::GetForeignMerchantNPCInfo()
{
	// 받은 패킷
	sDBRequest_GetForeignMerchantNPCInfo *pRecvMsg	= (sDBRequest_GetForeignMerchantNPCInfo *)m_pPacket;
	
	// 보낼 패킷
	sDBResponse_GetForeignMerchantNPCInfo sendMsg;
	
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_GETFOREIGNMERCHANTNPCINFO;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
//	sendMsg.usCharID = pRecvMsg->usCharID;
	
	// DB Query
	BEGIN_SP( "rsp_ForeignMerchantNPCInfoGet" )
		m_odbc.SetParam( SQL_SMALLINT, &(pRecvMsg->siForeignMerchantKind), sizeof(pRecvMsg->siForeignMerchantKind) );
	END_SP()

	EXEC_SQL()
	
	BEGIN_FETCHDATA()
		m_odbc.GetData( &(sendMsg.siResult), 0 );
		m_odbc.GetData( &(sendMsg.siForeignMerchantKind), 0 );
		m_odbc.GetData( &(sendMsg.siItemUnique), 0 );
		m_odbc.GetData( &(sendMsg.siPrice), 0 );
		m_odbc.GetData( &(sendMsg.siRestNum), 0 );
		GetGameDate( &sendMsg.clDate );
	END_FETCHDATA()
	
	// send
	SendPacket( (BYTE *)&sendMsg );
}

void DBGameProtocol::TradeMerchantSet()
{
	// 받은 패킷
	sDBRequest_TradeMerchantSet *pRecvMsg	= (sDBRequest_TradeMerchantSet *)m_pPacket;
	
	// 보낼 패킷
	sDBResponse_TradeMerchantSet sendMsg;
	
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_TRADEMERCHANTSET;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
//	sendMsg.usCharID = pRecvMsg->usCharID;
	
	// DB Query
	BEGIN_SP( "rsp_TradeMerchantSet" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siTradeMerchantKind, sizeof(pRecvMsg->siTradeMerchantKind) );
		m_odbc.SetParam( SQL_BIGINT, &pRecvMsg->siBuyTotalMoney, sizeof(pRecvMsg->siBuyTotalMoney) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siBuyItemUnique, sizeof(pRecvMsg->siBuyItemUnique) );
		m_odbc.SetParam( SQL_BIGINT, &pRecvMsg->siBuyItemPrice, sizeof(pRecvMsg->siBuyItemPrice) );
	END_SP()

	EXEC_SQL()
	
	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0 );
		m_odbc.GetData( &sendMsg.siTradeMerchantKind, 0 );
		m_odbc.GetData( &sendMsg.siBuyTotalMoney, 0 );
		m_odbc.GetData( &sendMsg.siBuyItemUnique, 0 );
		m_odbc.GetData( &sendMsg.siBuyItemPrice, 0 );
	END_FETCHDATA()
	
	// send
	SendPacket( (BYTE *)&sendMsg );

	// 로그 작성
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "MerchantKind = %d, TotalMoney = %I64d, ItemPrice = %I64d"
				, sendMsg.siTradeMerchantKind, sendMsg.siBuyTotalMoney, sendMsg.siBuyItemPrice
				);
		sDBLogMsg_QueryString LogMsg("TradeMerchant Set", 0, NULL, sendMsg.siBuyItemUnique, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

void DBGameProtocol::TradeMerchantBuyItemFromPerson()
{
	// 받은 패킷
	sDBRequest_TradeMerchantBuyItemFromPerson *pRecvMsg	= (sDBRequest_TradeMerchantBuyItemFromPerson *)m_pPacket;
	
	// 보낼 패킷
	sDBResponse_TradeMerchantBuyItemFromPerson sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_TRADEMERCHANTBUYITEMFROMPERSON;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	
	sendMsg.siCharID = pRecvMsg->siCharID;
	
	// DB Query
	BEGIN_SP( "rsp_TradeMerchantBuyItemFromPerson" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siTradeMerchantKind, sizeof(pRecvMsg->siTradeMerchantKind) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siItemPos, sizeof(pRecvMsg->siItemPos) );
		SetItemParam( &pRecvMsg->clItem );
		m_odbc.SetParam( SQL_BIGINT, &pRecvMsg->siIncreaseMoney, sizeof(pRecvMsg->siIncreaseMoney) );
		m_odbc.SetParam( SQL_BIGINT, &pRecvMsg->siTradeTax, sizeof(pRecvMsg->siTradeTax) );
	END_SP()

	EXEC_SQL()
	
	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0 );
		m_odbc.GetData( &sendMsg.siPersonID, 0 );
		m_odbc.GetData( &sendMsg.siTradeMerchantKind, 0 );
		m_odbc.GetData( &sendMsg.siItemPos, 0 );
		GetItemData( &sendMsg.clItem );
		m_odbc.GetData( &sendMsg.siIncreateMoney, 0 );
		m_odbc.GetData( &sendMsg.siTradeTax, 0 );
		m_odbc.GetData( &sendMsg.siLeftMoney, 0 );
		m_odbc.GetData( &sendMsg.siLeftBuyTotalMoney, 0 );
	END_FETCHDATA()
	
	// send
	SendPacket( (BYTE *)&sendMsg );

	// 로그 작성
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "MerchantKind = %d, ItemPos = %d, IncreaseMoney = %I64d, LeftPersonMoney = %I64d, TradeTax = %I64d, LeftMerchantTotalMoney = %I64d"
				, sendMsg.siTradeMerchantKind, sendMsg.siItemPos, sendMsg.siIncreateMoney, sendMsg.siLeftMoney, sendMsg.siTradeTax, sendMsg.siLeftBuyTotalMoney
				);
		sDBLogMsg_QueryString LogMsg("TradeMerchant BuyItem", sendMsg.siPersonID, NULL, pRecvMsg->clItem.siUnique, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}


void DBGameProtocol::TradeMerchantBuyHorseFromPerson()
{
	// 받은 패킷
	sDBRequest_TradeMerchantBuyHorseFromPerson *pRecvMsg	= (sDBRequest_TradeMerchantBuyHorseFromPerson *)m_pPacket;
	
	// 보낼 패킷
	sDBResponse_TradeMerchantBuyHorseFromPerson sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_TRADEMERCHANTBUYHORSEFROMPERSON;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	
	sendMsg.siCharID = pRecvMsg->siCharID;
	
	// DB Query
	BEGIN_SP( "rsp_TradeMerchantBuyHorseFromPerson" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siTradeMerchantKind, sizeof(pRecvMsg->siTradeMerchantKind) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siPersonHorseIndex, sizeof(pRecvMsg->siPersonHorseIndex) );
		m_odbc.SetParam( SQL_BIGINT, &pRecvMsg->siIncreaseMoney, sizeof(pRecvMsg->siIncreaseMoney) );
	END_SP()

	EXEC_SQL()
	
	SI32 siHorseUnique = 0;
	char szHorseName[MAX_HORSE_NAME] = "";

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0 );
		m_odbc.GetData( &sendMsg.siPersonID, 0 );
		m_odbc.GetData( &sendMsg.siTradeMerchantKind, 0 );
		m_odbc.GetData( &sendMsg.siPersonHorseIndex, 0 );
		m_odbc.GetData( &sendMsg.siIncreaseMoney, 0 );
		m_odbc.GetData( &sendMsg.siLeftMoney, 0 );
		m_odbc.GetData( &siHorseUnique, 0 );
		m_odbc.GetData( szHorseName, sizeof(szHorseName) );
	END_FETCHDATA()
	
	// send
	SendPacket( (BYTE *)&sendMsg );

	// 로그 작성
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "MerchantKind = %d, HorseIndex = %d, HorseUnique = %d, HorseName = %s, IncreaseMoney = %I64d, LeftPersonMoney = %I64d"
				, sendMsg.siTradeMerchantKind, sendMsg.siPersonHorseIndex, siHorseUnique, szHorseName, sendMsg.siIncreaseMoney, sendMsg.siLeftMoney
				);
		sDBLogMsg_QueryString LogMsg("TradeMerchant BuyHorse", sendMsg.siPersonID, NULL, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

void DBGameProtocol::TradeMerchantBuyItem()
{
	// 받은 패킷
	sDBRequest_TradeMerchantBuyItem *pRecvMsg	= (sDBRequest_TradeMerchantBuyItem *)m_pPacket;
	
	// 보낼 패킷
	sDBResponse_TradeMerchantBuyItem sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_TRADEMERCHANTBUYITEM;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	
	sendMsg.siCharID = pRecvMsg->siCharID;
	
	// DB Query
	BEGIN_SP( "rsp_TradeMerchantBuyItem" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siTradeMerchantKind, sizeof(pRecvMsg->siTradeMerchantKind) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siItemPos, sizeof(pRecvMsg->siItemPos) );
		SetItemParam( &pRecvMsg->clItem );
		m_odbc.SetParam( SQL_BIGINT, &pRecvMsg->siIncreaseMoney, sizeof(pRecvMsg->siIncreaseMoney) );
	END_SP()

	EXEC_SQL()
	
	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0 );
		m_odbc.GetData( &sendMsg.siPersonID, 0 );
		m_odbc.GetData( &sendMsg.siTradeMerchantKind, 0 );
		m_odbc.GetData( &sendMsg.siBuyItemNum, 0 );
		m_odbc.GetData( &sendMsg.siBuyItemUnique, 0 );
		m_odbc.GetData( &sendMsg.siItemPos, 0 );
		GetItemData( &sendMsg.clLeftItem );
		m_odbc.GetData( &sendMsg.siIncreaseMoney, 0 );
		m_odbc.GetData( &sendMsg.siLeftMoney, 0 );
	END_FETCHDATA()
	
	// send
	SendPacket( (BYTE *)&sendMsg );

	// 로그 작성
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "MerchantKind = %d, ItemPos = %d, IncreaseMoney = %I64d, LeftPersonMoney = %I64d"
				, sendMsg.siTradeMerchantKind, sendMsg.siItemPos, sendMsg.siIncreaseMoney, sendMsg.siLeftMoney
				);
		sDBLogMsg_QueryString LogMsg("TradeMerchant BuyItem", sendMsg.siPersonID, NULL, pRecvMsg->clItem.siUnique, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

void DBGameProtocol::TradeMerchantBuyCashItem()
{
	// 받은 패킷
	sDBRequest_ForeignMerchantBuyCashItem *pRecvMsg	= (sDBRequest_ForeignMerchantBuyCashItem *)m_pPacket;
	
	// 보낼 패킷
	sDBResponse_ForeignMerchantBuyCashItem sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_FOREIGNMERCHANTBUYCASHITEM;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.siCharID = pRecvMsg->siCharID;
	
	// DB Query
	BEGIN_SP( "rsp_TradeMerchantBuyCashItem" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siItemPos, sizeof(pRecvMsg->siItemPos) );
		SetItemParam( &pRecvMsg->clSellItem );
	END_SP()

	EXEC_SQL()
	
	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0 );
		m_odbc.GetData( &sendMsg.siPersonID, 0 );
		m_odbc.GetData( &sendMsg.siItemPos, 0 );
		GetItemData( &sendMsg.clLeftItem );
	END_FETCHDATA()
	
	sendMsg.siForeignMerchantKind = pRecvMsg->siForeignMerchantKind;
	sendMsg.siForeignMerchantCharID = pRecvMsg->siForeignMerchantCharID;
	sendMsg.siForeignMerchantCharUnique = pRecvMsg->siForeignMerchantCharUnique;
	sendMsg.uiEventMoney = pRecvMsg->uiEventMoney;
	memcpy( sendMsg.szAccountID, pRecvMsg->szAccountID, MAX_PLAYER_NAME - 1 );
	sendMsg.szAccountID[ MAX_PLAYER_NAME - 1 ] = NULL;

	// send
	SendPacket( (BYTE *)&sendMsg );

	// 로그 작성
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "MerchantKind = %d, ItemPos = %d, ItemNum = %d, LeftItemNum = %d, EventMoney = %d"
				, pRecvMsg->siForeignMerchantKind, pRecvMsg->siItemPos, pRecvMsg->clSellItem.siItemNum, sendMsg.clLeftItem.siItemNum, sendMsg.uiEventMoney
				);
		sDBLogMsg_QueryString LogMsg("TradeMerchant BuyCashItem", sendMsg.siPersonID, NULL, pRecvMsg->clSellItem.siUnique, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

