#include "DBGameProtocol.h"
#include "DBMsg-Structure.h"
#include "DBMsg-HorseMarket.h"
#include "DBMsg-Person.h"

void DBGameProtocol::GetHorseMarketStrInfo()
{
	// 받은 패킷
	sDBRequest_GetStrInfo *pRecvMsg = (sDBRequest_GetStrInfo *)m_pPacket;
	
	// 보낼 패킷
	sDBResponse_GetStrInfo sendMsg;
	sendMsg.clStrInfo.clCommonInfo.siStrType = RANKTYPE_HORSEMARKET;
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_GETSTRINFO;
	sendMsg.packetHeader.usSize = sendMsg.GetTotalSize();
	sendMsg.usCharID = pRecvMsg->usCharID;

	// DB Query
	BEGIN_SP( "rsp_HorseMarketInfoGet" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siVillageUnique, sizeof(pRecvMsg->siVillageUnique) );
	END_SP()
	
	EXEC_SQL()
	
	BEGIN_FETCHDATA()
		GetFeeInfo( &sendMsg.clStrInfo.clHorseMarketStrInfo.clFee );
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

void DBGameProtocol::HorseMarketGetManyOrder()
{
	// 받은 패킷
	sDBRequest_HorseMarketGetManyOrder *pRecvMsg = (sDBRequest_HorseMarketGetManyOrder *)m_pPacket;
	
	// 보낼 패킷
	sDBResponse_HorseMarketGetManyOrder sendMsg;
	
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_HORSEMARKET_GETMANYORDER;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );


	// DB Query
	BEGIN_SP( "rsp_HorseMarketOrderManyGet" )
	END_SP()
	
	EXEC_SQL()
	
	
	sendMsg.usCount 	= 0;
	sendMsg.siResult	= 1;

	BOOL bSend = FALSE;

	BEGIN_FETCHDATA()
		
		bSend = FALSE;

		GetHorseOrder( &sendMsg.clHorseOrder[ sendMsg.usCount ] );
		sendMsg.usCount++;

		// 50개 까지 받고 전송하기를 반복한다.
		if( sendMsg.usCount >= GET_MANY_HORSEMARKET_ORDER_NUM )
		{
			// send
			SendPacket( (BYTE *)&sendMsg );
			sendMsg.usCount = 0;
			
			bSend = TRUE;
		}
	
	END_FETCHDATA()

	if( bSend == FALSE ) {
		SendPacket( (BYTE *)&sendMsg );
	}
}

void DBGameProtocol::HorseMarketSetFee()
{
	// 받은 패킷
	sDBRequest_HorseMarketSetFee *pRecvMsg = (sDBRequest_HorseMarketSetFee *)m_pPacket;
	
	// 보낼 패킷
	sDBResponse_HorseMarketSetFee sendMsg;
	
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_HORSEMARKET_SETFEE;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->usCharID;

	cltDBFee clFee( &pRecvMsg->clFee );

	// DB Query
	BEGIN_SP( "rsp_HorseMarketFeeSet" )
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
}

void DBGameProtocol::HorseMarketOrderSell()
{
	// 받은 패킷
	sDBRequest_HorseMarketOrderSell *pRecvMsg = (sDBRequest_HorseMarketOrderSell *)m_pPacket;
	
	// 보낼 패킷
	sDBResponse_HorseMarketOrderSell sendMsg;
	
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_HORSEMARKET_ORDERSELL;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->usCharID;
	
	SI32 siOrderDay = pRecvMsg->clHorseOrder.clDate.GetDateVary();
	SI32 siBirthDay = pRecvMsg->clHorseOrder.clHorse.clBirthDate.GetDateVary();
	SI32 siAddStatusDate = pRecvMsg->clHorseOrder.clHorse.clAddStatusDate.GetDateVary();

	SI32 siPremiumPartsDate_Slot1 = pRecvMsg->clHorseOrder.clHorse.clPremiumPartsDate_Slot1.GetDateVary();
	SI32 siPremiumPartsDate_Slot2 = pRecvMsg->clHorseOrder.clHorse.clPremiumPartsDate_Slot2.GetDateVary();
	SI32 siPremiumPartsDate_Slot3 = pRecvMsg->clHorseOrder.clHorse.clPremiumPartsDate_Slot3.GetDateVary();

	// DB Query
	BEGIN_SP( "rsp_HorseMarketSellOrder" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siVillageUnique, sizeof(pRecvMsg->siVillageUnique) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siHorseIndex, sizeof(pRecvMsg->siHorseIndex) );
		SetHorseOrder( &pRecvMsg->clHorseOrder, &siOrderDay, &siBirthDay, &siAddStatusDate
			,&siPremiumPartsDate_Slot1,&siPremiumPartsDate_Slot2,&siPremiumPartsDate_Slot3
			);		
		m_odbc.SetParam( SQL_BIGINT, &pRecvMsg->siTax, sizeof(pRecvMsg->siTax) );
	END_SP()
	
	EXEC_SQL()
	
	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0 );
		m_odbc.GetData( &sendMsg.siVillageUnique, 0 );
		m_odbc.GetData( &sendMsg.siHorseIndex, 0 );
		GetHorseOrder( &sendMsg.clHorseOrder );
		m_odbc.GetData( &sendMsg.siTax, 0 );
		m_odbc.GetData( &sendMsg.clHorseMarketMoney.itMoney, 0 );
		m_odbc.GetData( &sendMsg.siPersonMoney, 0 );
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );

	// 로그 작성
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "VillageUnique = %d, OrderIndex = %d, Price = %I64d, Tax = %I64d, HorseName = %s, HorseUnique = %d, Str = %d, Dex = %d, Mag = %d, MoveSpeed = %d, LeftStrMoney = %I64d, LeftPersonMoney = %I64d"
				, sendMsg.siVillageUnique, sendMsg.clHorseOrder.siIndex, sendMsg.clHorseOrder.siPrice, sendMsg.siTax
				, sendMsg.clHorseOrder.clHorse.szName, sendMsg.clHorseOrder.clHorse.siHorseUnique, sendMsg.clHorseOrder.clHorse.siStr, sendMsg.clHorseOrder.clHorse.siDex, sendMsg.clHorseOrder.clHorse.siMag, sendMsg.clHorseOrder.clHorse.siMoveSpeed
				, sendMsg.clHorseMarketMoney.itMoney, sendMsg.siPersonMoney
				);
		sDBLogMsg_QueryString LogMsg("HorseMarket Sell Order", sendMsg.clHorseOrder.siPersonID, NULL, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

void DBGameProtocol::HorseMarketMyAccount()
{
	// 받은 패킷
	sDBRequest_HorseMarketMyAccount *pRecvMsg = (sDBRequest_HorseMarketMyAccount *)m_pPacket;
	
	// 보낼 패킷
	sDBResponse_HorseMarketMyAccount sendMsg;
	
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_HORSEMARKET_MYACCOUNT;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->usCharID;

	// DB Query
	BEGIN_SP( "rsp_HorseMarketMyAccount" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID) );
	END_SP()
	
	EXEC_SQL()
	
	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0 );
		GetHorseOrder( &sendMsg.clHorseOrder );
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );
}

void DBGameProtocol::HorseMarketOutHorse()
{
	// 받은 패킷
	sDBRequest_HorseMarketOutHorse *pRecvMsg = (sDBRequest_HorseMarketOutHorse *)m_pPacket;
	
	// 보낼 패킷
	sDBResponse_HorseMarketOutHorse sendMsg;
	
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_HORSEMARKET_OUTHORSE;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->usCharID;

	// DB Query
	BEGIN_SP( "rsp_HorseMarketHorseOut" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siHorseIndex, sizeof(pRecvMsg->siHorseIndex) );
	END_SP()
	
	EXEC_SQL()
	
	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0 );
		m_odbc.GetData( &sendMsg.siPersonID, 0 );
		m_odbc.GetData( &sendMsg.siHorseIndex, 0 );
		m_odbc.GetData( &sendMsg.siOrderIndex, 0 );
		GetHorseInfo( &sendMsg.clHorse );
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );

	// 로그 작성
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "OrderIndex = %d, HorseName = %s, HorseUnique = %d, Str = %d, Dex = %d, Mag = %d, MoveSpeed = %d"
				, sendMsg.siOrderIndex, sendMsg.clHorse.szName, sendMsg.clHorse.siHorseUnique, sendMsg.clHorse.siStr, sendMsg.clHorse.siDex, sendMsg.clHorse.siMag, sendMsg.clHorse.siMoveSpeed
				);
		sDBLogMsg_QueryString LogMsg("HorseMarket Out Horse", sendMsg.siPersonID, NULL, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

void DBGameProtocol::HorseMarketBuyHorse()
{
	// 받은 패킷
	sDBRequest_HorseMarketBuyHorse *pRecvMsg = (sDBRequest_HorseMarketBuyHorse *)m_pPacket;
	
	// 보낼 패킷
	sDBResponse_HorseMarketBuyHorse sendMsg;
	
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_HORSEMARKET_BUYHORSE;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->usCharID;

	// DB Query
	BEGIN_SP( "rsp_HorseMarketHorseBuy" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siHorseIndex, sizeof(pRecvMsg->siHorseIndex) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siVillageUnique, sizeof(pRecvMsg->siVillageUnique) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siOrderIndex, sizeof(pRecvMsg->siOrderIndex) );
		m_odbc.SetParam( SQL_BIGINT, &pRecvMsg->siPrice, sizeof(pRecvMsg->siPrice) );
	END_SP()
	
	EXEC_SQL()
	
	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0 );
		m_odbc.GetData( &sendMsg.siPersonID, 0 );
		m_odbc.GetData( &sendMsg.siMoney, 0 );
		m_odbc.GetData( &sendMsg.siVillageUnique, 0 );
		m_odbc.GetData( &sendMsg.siOrderIndex, 0 );
		m_odbc.GetData( &sendMsg.siTradePrice, 0 );
	END_FETCHDATA()

	BEGIN_NEXTRECORDSET()
		BEGIN_FETCHDATA()
			m_odbc.GetData( &sendMsg.siSellerPersonID, 0 );
			m_odbc.GetData( &sendMsg.clSellerBankMoney.itMoney, 0 );
		END_FETCHDATA()
	END_NEXTRECORDSET()

	BEGIN_NEXTRECORDSET()
		BEGIN_FETCHDATA()
			m_odbc.GetData( &sendMsg.siHorseIndex, 0 );
			GetHorseInfo( &sendMsg.clHorse );
		END_FETCHDATA()
	END_NEXTRECORDSET()

	// send
	SendPacket( (BYTE *)&sendMsg );

	// 로그 작성
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "OrderIndex = %d, Price = %I64d, LeftBuyerMoney = %I64d, LeftSellerBankMoney = %I64d, HorseName = %s, HorseUnique = %d, Str = %d, Dex = %d, Mag = %d, MoveSpeed = %d"
				, sendMsg.siOrderIndex, sendMsg.siTradePrice, sendMsg.siMoney, sendMsg.clSellerBankMoney.itMoney
				, sendMsg.clHorse.szName, sendMsg.clHorse.siHorseUnique, sendMsg.clHorse.siStr, sendMsg.clHorse.siDex, sendMsg.clHorse.siMag, sendMsg.clHorse.siMoveSpeed
				);
		sDBLogMsg_QueryString LogMsg("HorseMarket Buy Horse", sendMsg.siPersonID, sendMsg.siSellerPersonID, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

void DBGameProtocol::HorseMarketReduceOrderPrice()
{
	// 받은 패킷
	sDBRequest_ReduceHorseOrderPrice *pRecvMsg = (sDBRequest_ReduceHorseOrderPrice *)m_pPacket;
	
	// 보낼 패킷
	sDBResponse_ReduceHorseOrderPrice sendMsg;
	
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_REDUCEHORSEORDERPRICE;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
//	sendMsg.usCharID = pRecvMsg->usCharID;
	
	sendMsg.arrayindex = pRecvMsg->arrayindex;

	SI32 siOrderDay = pRecvMsg->clOrder.clDate.GetDateVary();
	SI32 siBirthDay = pRecvMsg->clOrder.clHorse.clBirthDate.GetDateVary();
	SI32 siAddStatusDate = pRecvMsg->clOrder.clHorse.clAddStatusDate.GetDateVary();

	SI32 siPramiumPartsDate_Slot1 = pRecvMsg->clOrder.clHorse.clPremiumPartsDate_Slot1.GetDateVary();
	SI32 siPramiumPartsDate_Slot2 = pRecvMsg->clOrder.clHorse.clPremiumPartsDate_Slot2.GetDateVary();
	SI32 siPramiumPartsDate_Slot3 = pRecvMsg->clOrder.clHorse.clPremiumPartsDate_Slot3.GetDateVary();

	// DB Query
	BEGIN_SP( "rsp_HorseMarketOrderPriceReduce" )
		SetHorseOrder( &pRecvMsg->clOrder, &siOrderDay, &siBirthDay, &siAddStatusDate
		, &siPramiumPartsDate_Slot1, &siPramiumPartsDate_Slot2, &siPramiumPartsDate_Slot3);
	END_SP()
	
	EXEC_SQL()
	
	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0 );
		GetHorseOrder( &sendMsg.clOrder );
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );

	// 로그 작성
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "OrderIndex = %d, Price = %I64d, HorseName = %s, HorseUnique = %d, Str = %d, Dex = %d, Mag = %d, MoveSpeed = %d"
				, sendMsg.clOrder.siIndex, sendMsg.clOrder.siPrice
				, sendMsg.clOrder.clHorse.szName, sendMsg.clOrder.clHorse.siHorseUnique, sendMsg.clOrder.clHorse.siStr, sendMsg.clOrder.clHorse.siDex, sendMsg.clOrder.clHorse.siMag, sendMsg.clOrder.clHorse.siMoveSpeed
				);
		sDBLogMsg_QueryString LogMsg("Reduce HorseOrder Price", sendMsg.clOrder.siPersonID, NULL, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

void DBGameProtocol::SetHorsePrice()
{
	// 받은 패킷
	sDBRequest_SetCurrentHorsePrice *pRecvMsg = (sDBRequest_SetCurrentHorsePrice *)m_pPacket;
	
	// 보낼 패킷
	sDBResponse_SetCurrentHorsePrice sendMsg;
	
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_SETCURRENTHORSEPRICE;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
//	sendMsg.usCharID = pRecvMsg->usCharID;
	
	// DB Query
	BEGIN_SP( "rsp_HorsePriceSet" )
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siHorseUnique, sizeof(pRecvMsg->siHorseUnique) );
		m_odbc.SetParam( SQL_BIGINT, &pRecvMsg->siHorseCurrentPrice, sizeof(pRecvMsg->siHorseCurrentPrice) );
	END_SP()
	
	EXEC_SQL()
	
	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0 );
		m_odbc.GetData( &sendMsg.siHorseUnique, 0 );
		m_odbc.GetData( &sendMsg.siHorseCurrentPrice, 0 );
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );

	// 로그 작성
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "HorseUnique = %d, Price = %I64d"
				, sendMsg.siHorseUnique, sendMsg.siHorseCurrentPrice
				);
		sDBLogMsg_QueryString LogMsg("Horse Price Set", NULL, NULL, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

void DBGameProtocol::GetHorsePrice()
{
	// 받은 패킷
	sDBRequest_GetCurrentHorsePrice *pRecvMsg = (sDBRequest_GetCurrentHorsePrice *)m_pPacket;
	
	// 보낼 패킷
	sDBResponse_GetCurrentHorsePrice sendMsg;
	
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_GETCURRENTHORSEPRICE;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
//	sendMsg.usCharID = pRecvMsg->usCharID;
	
	// DB Query
	BEGIN_SP( "rsp_HorsePriceGet" )
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siHorseUnique, sizeof(pRecvMsg->siHorseUnique) );
	END_SP()
	
	EXEC_SQL()
	
	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0 );
		m_odbc.GetData( &sendMsg.siHorseUnique, 0 );
		m_odbc.GetData( &sendMsg.siHorseCurrentPrice, 0 );
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );
}