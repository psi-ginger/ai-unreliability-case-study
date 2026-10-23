#include "DBProtocol.h"

#define DBMessageMap(AAA,BBB) m_pDBMsgPtr[AAA].Func = &DBProtocol::BBB

DBProtocol::DBProtocol()
{
	m_pDBMsgPtr = new stDBMsgPtr[ NTRADESERVERDB_MSG_END ];
	if(m_pDBMsgPtr == NULL)
		MessageBox(NULL, "함수포인터 할당 실패", NULL, MB_OK);
	else
	{
		// 알 수 없는 메시지가 왔을때 처리할 함수를 할당해 준다.
		for(int i = 0; i < NTRADESERVERDB_MSG_END; i++)
			m_pDBMsgPtr[i].Func = &DBProtocol::NullFuncPtr;


		// DB메시지맵등록
		DBMessageMap(NTRADESERVERDB_REQUEST_SET_SELLORDER,			SetSellOrder);
		DBMessageMap(NTRADESERVERDB_REQUEST_CANCEL_SELLORDER,		CancelSellOrder);
		DBMessageMap(NTRADESERVERDB_REQUEST_SET_BUYORDER,			SetBuyOrder);
		DBMessageMap(NTRADESERVERDB_REQUEST_GET_LISTINFO,			GetListInfo);
		DBMessageMap(NTRADESERVERDB_REQUEST_DEL_SELLINFO,			DelSellInfo);
		DBMessageMap(NTRADESERVERDB_REQUEST_REDUCEPRICE,			ReducePrice);
		DBMessageMap(NTRADESERVERDB_REQUEST_SET_SELLINFO,			SetSellInfo);
		DBMessageMap(NTRADESERVERDB_REQUEST_DELETE_SELLORDER,		DeleteSellOrder);
		DBMessageMap(NTRADESERVERDB_REQUEST_SET_SELLORDER_HOLDACU,	SetSellOrder_HoldAcu);
		DBMessageMap(NTRADESERVERDB_REQUEST_WITHDRAW_ACU,			WithdrawAcu);
		DBMessageMap(NTRADESERVERDB_REQUEST_WITHDRAW_ACU_SUCCESS,	WithdrawAcuSuccess);
		DBMessageMap(NTRADESERVERDB_REQUEST_SAVEDACU_LIST,			SavedAcuList);
		
	}
}

DBProtocol::~DBProtocol()
{
	m_odbc.Disconnect();
//	m_odbcSell.Disconnect();
	if(m_pDBMsgPtr != NULL)
		delete [] m_pDBMsgPtr;
}


void DBProtocol::Init( UI16 usPort, char *szDBIP, char *szDatabase, char *szUserID, char *szUserPW )
{
	m_odbc.Connect( "SQL Server", szDBIP, szDatabase, szUserID, szUserPW );
//	m_odbcSell.Connect( "SQL Server", szDBIP, szDatabase, szUserID, szUserPW );

	m_SF.CreateFactory( GetSessionManager(), 10, 5000 * 1000, 5000 * 1000 );

	GetNetIOCP()->CreateIOCP( GetSessionManager(), 1 );

	GetListen()->CreateListen( GetSessionManager(), usPort );

	SetFPS( 80 );
}

void DBProtocol::Update()
{
}

void DBProtocol::MessageProc()
{
	if( m_pPacket == NULL ) return;

	if(m_pPacket->usCmd >= 0 && m_pPacket->usCmd <= NTRADESERVERDB_MSG_END)
	{
		(this->*(this->m_pDBMsgPtr[m_pPacket->usCmd].Func))();
	}
	else
	{
		NullFuncPtr();
	}
}




void DBProtocol::SessionProc( ITZSession *pSession )
{
	m_pSession = pSession;

	SI32 count = pSession->GetRecvBuffer()->GetPacketCount();

	// 현재 버퍼에 있는 패킷 개수 만큼 메시지 처리 루틴 반복 
	for( int i = 0; i < count; ++i ) 
	{
		// 버퍼에서 패킷을 하나 꺼내온다 
		m_pPacket = (sPacketHeader *)( pSession->GetRecvBuffer()->GetFirstPacket() );

		if( m_pPacket == NULL ) return;	// 패킷이 없으면 나간다

		MessageProc();

		if(!m_odbc.GetReconnectStat())
			pSession->GetRecvBuffer()->RemoveFirstPacket();

	}

}

void DBProtocol::SendPacket( char *pPacket )
{
	m_pSession->GetSendBuffer()->Write( pPacket );
}

void DBProtocol::NullFuncPtr()
{
	char buffer[128];
	sprintf(buffer, "Not Defined Msg:%d", m_pPacket->usCmd);
	MessageBox(NULL, "DBProtocol Error", buffer, MB_OK);
}

void DBProtocol::SetSellOrder()
{
	// 받은 패킷
	NTradeServerDB_Request_Set_SellOrder *pRecvMsg	= (NTradeServerDB_Request_Set_SellOrder *)m_pPacket;

	// 보낼 패킷
	NTradeServerDB_Response_Set_SellOrder sendMsg;

	sendMsg.packetHeader.usCmd = NTRADESERVERDB_RESPONSE_SET_SELLORDER;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.uiServerIndex = pRecvMsg->uiServerIndex;
	sendMsg.siCharID = pRecvMsg->siCharID;

	sendMsg.siPos = pRecvMsg->siPos;

	BEGIN_SP("rsp_SetSellOrder")
		SetTradeServerOrder( &pRecvMsg->clOrder );
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult );
	END_FETCHDATA()

	BEGIN_NEXTRECORDSET()
		BEGIN_FETCHDATA()
			GetTradeServerOrder( &sendMsg.clOrder );
		END_FETCHDATA()
	END_NEXTRECORDSET()

	if ( sendMsg.siResult != 1 )
	{
		sendMsg.clOrder.siPersonID = pRecvMsg->clOrder.siPersonID ;
	}

	SendPacket( (char *)&sendMsg );
}

void DBProtocol::GetTradeServerOrder(cltTradeServerOrder *pclOrder)
{
	m_odbc.GetData( &pclOrder->siIndex ); // 주문의 고유값

	m_odbc.GetData( &pclOrder->szServerNation, sizeof(pclOrder->szServerNation) ); // 서버 국가 코드
	m_odbc.GetData( &pclOrder->siServerIndex ); // 주문 서버
	m_odbc.GetData( &pclOrder->siCharNation ); // 주문자 국가 번호

	m_odbc.GetData( pclOrder->szAccountID, sizeof(pclOrder->szAccountID) ); // 주문자 계정
	m_odbc.GetData( pclOrder->szCharName, sizeof(pclOrder->szCharName) ); // 주문자 이름
	m_odbc.GetData( &pclOrder->siPersonID ); // 주문자 personid
	GetTradeItem( &pclOrder->clitem );
}

void DBProtocol::SetTradeServerOrder(const cltTradeServerOrder *pclOrder)
{
	m_odbc.SetParam( SQL_INTEGER, (void*)&pclOrder->siIndex, sizeof(pclOrder->siIndex) );
	m_odbc.SetParam( SQL_VARCHAR, (void*)&pclOrder->szServerNation, sizeof(pclOrder->szServerNation) ); // 서버 국가 코드
	m_odbc.SetParam( SQL_SMALLINT, (void*)&pclOrder->siServerIndex, sizeof(pclOrder->siServerIndex) ); // 주문 서버
	m_odbc.SetParam( SQL_SMALLINT, (void*)&pclOrder->siCharNation, sizeof(pclOrder->siCharNation) ); // 주문자 국가 번호
	m_odbc.SetParam( SQL_VARCHAR, (void*)pclOrder->szAccountID, sizeof(pclOrder->szAccountID) );
	m_odbc.SetParam( SQL_VARCHAR, (void*)pclOrder->szCharName, sizeof(pclOrder->szCharName) );
	m_odbc.SetParam( SQL_INTEGER, (void*)&pclOrder->siPersonID, sizeof(pclOrder->siPersonID) );
	SetTradeItem( &pclOrder->clitem );
}

void DBProtocol::GetTradeItem(cltTradeItem *pclItem)
{
	m_odbc.GetData( &pclItem->siUnique );
	m_odbc.GetData( &pclItem->siItemNum );
	m_odbc.GetData( &pclItem->siRank );
	m_odbc.GetData( &pclItem->siPrice );
	m_odbc.GetData( &pclItem->siType );
	m_odbc.GetData( &pclItem->siPower );
	m_odbc.GetData( &pclItem->siRareType );
	m_odbc.GetData( &pclItem->szItemData, sizeof(pclItem->szItemData) );
	m_odbc.GetData( &pclItem->clDate );
	m_odbc.GetData( &pclItem->uiSkillNumber );
	m_odbc.GetData( &pclItem->uiChangeAbilityCount );
	m_odbc.GetData( &pclItem->uiAbilityUp );
}

void DBProtocol::SetTradeItem(const cltTradeItem *pclItem)
{
	m_odbc.SetParam( SQL_SMALLINT, (void*)&pclItem->siUnique, sizeof(pclItem->siUnique) );
	m_odbc.SetParam( SQL_SMALLINT, (void*)&pclItem->siItemNum, sizeof(pclItem->siItemNum) );
	m_odbc.SetParam( SQL_SMALLINT, (void*)&pclItem->siRank, sizeof(pclItem->siRank) );
	m_odbc.SetParam( SQL_BIGINT, (void*)&pclItem->siPrice, sizeof(pclItem->siPrice) );
	m_odbc.SetParam( SQL_SMALLINT, (void*)&pclItem->siType, sizeof(pclItem->siType) );
	m_odbc.SetParam( SQL_SMALLINT, (void*)&pclItem->siPower, sizeof(pclItem->siPower) );
	m_odbc.SetParam( SQL_TINYINT, (void*)&pclItem->siRareType, sizeof(pclItem->siRareType) );
	m_odbc.SetParam( SQL_BINARY, (void*)&pclItem->szItemData, sizeof(pclItem->szItemData) );
	m_odbc.SetParam( SQL_INTEGER, (void*)&pclItem->clDate, sizeof(pclItem->clDate));
	m_odbc.SetParam( SQL_TINYINT, (void*)&pclItem->uiSkillNumber, sizeof(pclItem->uiSkillNumber));
	m_odbc.SetParam( SQL_TINYINT, (void*)&pclItem->uiChangeAbilityCount, sizeof(pclItem->uiChangeAbilityCount));
	m_odbc.SetParam( SQL_TINYINT, (void*)&pclItem->uiAbilityUp, sizeof(pclItem->uiAbilityUp));
}

void DBProtocol::CancelSellOrder()
{
	// 받은 패킷
	NTradeServerDB_Request_Cancel_SellOrder *pRecvMsg	= (NTradeServerDB_Request_Cancel_SellOrder *)m_pPacket;

	// 보낼 패킷
	NTradeServerDB_Response_Cancel_SellOrder sendMsg;

	sendMsg.packetHeader.usCmd = NTRADESERVERDB_RESPONSE_CANCEL_SELLORDER;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.uiServerIndex = pRecvMsg->uiServerIndex;
	sendMsg.siCharID = pRecvMsg->siCharID;
	sendMsg.siPos = pRecvMsg->siPos ;

	BEGIN_SP("rsp_CancelSellOrder")
		SetTradeServerOrder( &pRecvMsg->clOrder );
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult );
		GetTradeServerOrder( &sendMsg.clOrder );
	END_FETCHDATA()

	SendPacket( (char *)&sendMsg );
}

void DBProtocol::SetBuyOrder()
{
	// 받은 패킷
	NTradeServerDB_Request_Set_BuyOrder *pRecvMsg	= (NTradeServerDB_Request_Set_BuyOrder *)m_pPacket;

	// 보낼 패킷
	NTradeServerDB_Response_Set_BuyOrder sendMsg;

	sendMsg.packetHeader.usCmd = NTRADESERVERDB_RESPONSE_SET_BUYORDER;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.uiServerIndex = pRecvMsg->uiServerIndex;
	sendMsg.siCharID = pRecvMsg->siCharID;
	sendMsg.siPos = pRecvMsg->siPos ;

	memcpy(sendMsg.szServerNation,pRecvMsg->szServerNation,4);
	sendMsg.siServerIndex = pRecvMsg->siServerIndex;
	memcpy(sendMsg.szCharName,pRecvMsg->szCharName,20);

	BEGIN_SP("rsp_SetBuyOrder")
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID) );
		m_odbc.SetParam( SQL_VARCHAR, &pRecvMsg->szServerNation, sizeof(pRecvMsg->szServerNation) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siServerIndex, sizeof(pRecvMsg->siServerIndex) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siCharNation, sizeof(pRecvMsg->siCharNation) );
		m_odbc.SetParam( SQL_VARCHAR, &pRecvMsg->szAccountID, sizeof(pRecvMsg->szAccountID) );
		m_odbc.SetParam( SQL_VARCHAR, &pRecvMsg->szCharName, sizeof(pRecvMsg->szCharName) );
		SetTradeServerOrder( &pRecvMsg->clOrder );
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult );
		m_odbc.GetData( &sendMsg.siPersonID );
	END_FETCHDATA()

	BEGIN_NEXTRECORDSET()
		BEGIN_FETCHDATA()
			GetTradeServerOrder( &sendMsg.clOrder );
		END_FETCHDATA()
	END_NEXTRECORDSET()

	SendPacket( (char *)&sendMsg );
}

void DBProtocol::GetListInfo()
{
	// 받은 패킷
	NTradeServerDB_Request_Get_ListInfo *pRecvMsg	= (NTradeServerDB_Request_Get_ListInfo *)m_pPacket;

	// 보낼 패킷
	NTradeServerDB_Response_Get_ListInfo sendMsg;

	sendMsg.packetHeader.usCmd = NTRADESERVERDB_RESPONSE_GET_LISTINFO;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.uiServerIndex = pRecvMsg->uiServerIndex;
	sendMsg.siListStartIndex = pRecvMsg->siListStartIndex;
	sendMsg.bDBInit = pRecvMsg->bDBInit ;

	SI32 siMaxList = MAX_TRADESERVER_GET_LIST;

	BEGIN_SP("rsp_GetListInfo")
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siListStartIndex, sizeof(pRecvMsg->siListStartIndex) );
		m_odbc.SetParam( SQL_INTEGER, &siMaxList, sizeof(siMaxList) );
	END_SP()

	EXEC_SQL()

	SI32 siCount = 0;
	BEGIN_FETCHDATA()
		if(siCount >= MAX_TRADESERVER_GET_LIST) continue;
		GetTradeServerOrder(&sendMsg.clOrder[siCount]);
		siCount++;
	END_FETCHDATA()

	sendMsg.siResult = 1;

	SendPacket( (char *)&sendMsg );
}

void DBProtocol::SetSellInfo()
{
	// 받은 패킷
	NTradeServerDB_Request_Set_SellInfo *pRecvMsg	= (NTradeServerDB_Request_Set_SellInfo *)m_pPacket;

	// 보낼 패킷
	NTradeServerDB_Response_Set_SellInfo sendMsg;

	sendMsg.packetHeader.usCmd = NTRADESERVERDB_RESPONSE_SET_SELLINFO;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );


	BEGIN_SP("rsp_SetSellInfo")
		m_odbc.SetParam( SQL_VARCHAR, &pRecvMsg->szServerIP, sizeof(pRecvMsg->szServerIP));
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0 );
		m_odbc.GetData( &sendMsg.szServerIP, sizeof(sendMsg.szServerIP) );
	END_FETCHDATA()

	BEGIN_NEXTRECORDSET()
		BEGIN_FETCHDATA()
//			m_odbc.GetData( &sendMsg.szBuyerServer, sizeof(sendMsg.szBuyerServer) );
			m_odbc.GetData( &sendMsg.szBuyerServerNation, sizeof(sendMsg.szBuyerServerNation) ); // 서버 국가 코드
			m_odbc.GetData( &sendMsg.siBuyerServerIndex ); // 주문 서버
			m_odbc.GetData( &sendMsg.siBuyerNation ); // 주문자 국가 번호
			m_odbc.GetData( &sendMsg.szBuyerAccountID, sizeof(sendMsg.szBuyerAccountID) );
			m_odbc.GetData( &sendMsg.szBuyerName, sizeof(sendMsg.szBuyerName) );
			GetTradeServerOrder( &sendMsg.clLeftOrder );
		END_FETCHDATA()
	END_NEXTRECORDSET()

	if ( sendMsg.siResult == 1 )
	{
		SendPacket( (char *)&sendMsg );
	}
}

void DBProtocol::DelSellInfo()
{
	// 받은 패킷
	NTradeServerDB_Request_Del_SellInfo *pRecvMsg	= (NTradeServerDB_Request_Del_SellInfo *)m_pPacket;

	BEGIN_SP("rsp_DelSellInfo")
		SetTradeServerOrder( &pRecvMsg->clOrder );
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
	END_FETCHDATA()
}

void DBProtocol::ReducePrice()
{
	// 받은 패킷
	NTradeServerDB_Request_ReducePrice *pRecvMsg	= (NTradeServerDB_Request_ReducePrice *)m_pPacket;

	// 보낼 패킷
	NTradeServerDB_Response_ReducePrice sendMsg;

	sendMsg.packetHeader.usCmd = NTRADESERVERDB_RESPONSE_REDUCEPRICE;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );

	BEGIN_SP("rsp_ReducePrice")
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siIndex, sizeof(pRecvMsg->siIndex) );
		m_odbc.SetParam( SQL_VARCHAR, &pRecvMsg->szSellerServerNation, sizeof(pRecvMsg->szSellerServerNation) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siSellerServerIndex, sizeof(pRecvMsg->siSellerServerIndex) );
		m_odbc.SetParam( SQL_VARCHAR, &pRecvMsg->szSellerName, sizeof(pRecvMsg->szSellerName) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siItemUnique, sizeof(pRecvMsg->siItemUnique) );
		m_odbc.SetParam( SQL_BIGINT, &pRecvMsg->siPrice, sizeof(pRecvMsg->siPrice) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->clDate, sizeof(pRecvMsg->clDate) );
	END_SP()

	EXEC_SQL()


	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult );
	END_FETCHDATA()

	BEGIN_NEXTRECORDSET()
		BEGIN_FETCHDATA()
			GetTradeServerOrder( &sendMsg.clOrder );
		END_FETCHDATA()
	END_NEXTRECORDSET()

	SendPacket( (char *)&sendMsg );

}

void DBProtocol::DeleteSellOrder()
{
	// 받은 패킷
	NTradeServerDB_Request_Delete_SellOrder *pRecvMsg	= (NTradeServerDB_Request_Delete_SellOrder *)m_pPacket;

	BEGIN_SP("rsp_DeleteSellOrder")
		m_odbc.SetParam( SQL_VARCHAR, &pRecvMsg->szServerNation, sizeof(pRecvMsg->szServerNation) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siServerIndex, sizeof(pRecvMsg->siServerIndex) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID) );
	END_SP()

	EXEC_SQL()
}

void DBProtocol::SetSellOrder_HoldAcu()
{
	// 받은 패킷
	NTradeServerDB_Request_Set_SellOrder_HoldAcu* pRecvMsg	= (NTradeServerDB_Request_Set_SellOrder_HoldAcu *)m_pPacket;

	// 보낼 패킷
	NTradeServerDB_Response_Set_SellOrder sendMsg;

	sendMsg.packetHeader.usCmd	= NTRADESERVERDB_RESPONSE_SET_SELLORDER;
	sendMsg.packetHeader.usSize	= sizeof( sendMsg );
	sendMsg.uiServerIndex		= pRecvMsg->m_uiServerIndex;
	sendMsg.siCharID			= pRecvMsg->m_siCharID;
	sendMsg.siPos				= pRecvMsg->m_siPos;

	BEGIN_SP("rsp_SetSellOrder_Acu")
		SetTradeServerOrder( &pRecvMsg->m_clOrder );
		m_odbc.SetParam( SQL_BIT, &pRecvMsg->m_bHodlAcu, sizeof(pRecvMsg->m_bHodlAcu) );
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult );
	END_FETCHDATA()

	BEGIN_NEXTRECORDSET()
		BEGIN_FETCHDATA()
			GetTradeServerOrder( &sendMsg.clOrder );
		END_FETCHDATA()
	END_NEXTRECORDSET()

	if ( sendMsg.siResult != 1 )
	{
		sendMsg.clOrder.siPersonID = pRecvMsg->m_clOrder.siPersonID ;
	}

	SendPacket( (char *)&sendMsg );
}

void DBProtocol::WithdrawAcu()
{
	// 받은 패킷
	NTradeServerDB_Request_Withdraw_Acu* pRecvMsg = (NTradeServerDB_Request_Withdraw_Acu*)m_pPacket;

	// 보낼 패킷
	NTradeServerDB_Response_Withdraw_Acu sendMsg;
	sendMsg.m_packetHeader.usCmd	= NTRADESERVERDB_RESPONSE_WITHDRAW_ACU;
	sendMsg.m_packetHeader.usSize	= sizeof(sendMsg);
	sendMsg.m_uiServerIndex			= pRecvMsg->m_uiServerIndex;
	sendMsg.m_siCharID				= pRecvMsg->m_siCharID;

	BEGIN_SP("rsp_WidhrawAcu")
		m_odbc.SetParam( SQL_VARCHAR,	pRecvMsg->m_szServerNation,	sizeof(pRecvMsg->m_szServerNation) );
		m_odbc.SetParam( SQL_SMALLINT,	&pRecvMsg->m_siServerIndex,	sizeof(pRecvMsg->m_siServerIndex) );
		m_odbc.SetParam( SQL_INTEGER,	&pRecvMsg->m_siPersonID,	sizeof(pRecvMsg->m_siPersonID) );
		m_odbc.SetParam( SQL_BIGINT,	&pRecvMsg->m_siWithdrawAcu,	sizeof(pRecvMsg->m_siWithdrawAcu) );
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.m_siResult );
		m_odbc.GetData( &sendMsg.m_siPersonID );
		m_odbc.GetData( &sendMsg.m_siWithdrawAcu );
		m_odbc.GetData( &sendMsg.m_siLeftSavedAcu );
		m_odbc.GetData( &sendMsg.m_siReservedNum );
	END_FETCHDATA()


	SendPacket( (char *)&sendMsg );

}

void DBProtocol::WithdrawAcuSuccess()
{
	// 받은 패킷
	NTradeServerDB_Request_Withdraw_Acu_Success* pRecvMsg = (NTradeServerDB_Request_Withdraw_Acu_Success*)m_pPacket;

	BEGIN_SP("rsp_WithrawAcuSuccess")
		m_odbc.SetParam( SQL_INTEGER,	&pRecvMsg->m_siReservedNum,	sizeof(pRecvMsg->m_siReservedNum) );
	END_SP()

	EXEC_SQL()
}

void DBProtocol::SavedAcuList()
{
	// 받은 패킷
	NTradeServerDB_Request_SavedAcu_List* pRecvMsg = (NTradeServerDB_Request_SavedAcu_List*)m_pPacket;

	// 보낼 패킷
	NTradeServerDB_Response_SavedAcu_List sendMsg;
	sendMsg.m_packetHeader.usCmd	= NTRADESERVERDB_RESPONSE_SAVEDACU_LIST;
	sendMsg.m_packetHeader.usSize	= sizeof(sendMsg);
	sendMsg.m_uiServerIndex			= pRecvMsg->m_uiServerIndex;
	sendMsg.m_siCharID				= pRecvMsg->m_siCharID;

	BEGIN_SP("rsp_SavedAcuList")
		m_odbc.SetParam( SQL_VARCHAR,	pRecvMsg->m_szServerNation,	sizeof(pRecvMsg->m_szServerNation) );
		m_odbc.SetParam( SQL_SMALLINT,	&pRecvMsg->m_siServerIndex,	sizeof(pRecvMsg->m_siServerIndex) );
		m_odbc.SetParam( SQL_INTEGER,	&pRecvMsg->m_siPersonID,	sizeof(pRecvMsg->m_siPersonID) );
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.m_siResult );
		m_odbc.GetData( &sendMsg.m_siPersonID );
		m_odbc.GetData( &sendMsg.m_siSavedAcu );
	END_FETCHDATA()
	
	//  리스트 받는다
	SI32 siListIndex = 0;
	BEGIN_NEXTRECORDSET()
		BEGIN_FETCHDATA()
			if(siListIndex >= MAX_TRADESERVER_ACU_LIST) break;
			m_odbc.GetData(&sendMsg.m_clAcuList[siListIndex].m_siItemUnique);
			m_odbc.GetData(&sendMsg.m_clAcuList[siListIndex].m_siPriceACU);
			m_odbc.GetData(&sendMsg.m_clAcuList[siListIndex++].m_clDate);
		END_FETCHDATA()
	END_NEXTRECORDSET()


	SendPacket( (char *)&sendMsg );
}