#include "DBProcess.h"
#include "ITZRecvBuffer.h"
#include "ITZSendBuffer.h"

extern GameProtocol g_GameProtocol;

SI32 DBProcess::s_uiDaysOfMonth[13];

DBProcess::DBProcess()
{
	m_pDBSession = NULL;

	for (SI32 i = 0 ; i < MAX_TRADESERVER_ORDER_LIST ; i ++ )
		clOrder[i].Init();

	m_bDBInit = false ;

	SI32 days[] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

	for( i = 0; i < 13; ++i ) 
	{
		s_uiDaysOfMonth[ i ] = days[ i ];
	}

	InitializeCriticalSection(&m_LogCritical);
}

DBProcess::~DBProcess()
{
	for (SI32 i = 0 ; i < MAX_TRADESERVER_ORDER_LIST ; i ++ )
		clOrder[i].Init();

	m_bDBInit = false ;

	DeleteCriticalSection(&m_LogCritical);
}

void DBProcess::Init(char *szIP, UI16 usPort )
{ 

	m_sf.CreateFactory( &m_sm, 1, 5000 * 1000, 5000 * 1000 );

	m_pDBSession = m_sm.GetSession( 0 );

	m_iocp.CreateIOCP( &m_sm, 1 );

	strcpy( m_szIP, szIP );
	m_usPort = usPort;
}

void DBProcess::Run()
{
	if( m_pDBSession == NULL ) return;

	m_sm.Update();

	static bool bSendOK = false ;
	static bool bTimeSwtich = false ;

	GetLocalTime(&m_sTime);

	// 세션이 연결되지 않은 상태이면,
	if( m_pDBSession->GetState() == SESSION_STATE_NONE )
	{
		if( m_sm.Connect( 0, m_szIP, m_usPort ) == m_pDBSession )
		{
		}
		else
		{
		}
	} 
	// 세션이 성공적으로 연결된 상태이면,
	else if( m_pDBSession->GetState() == SESSION_STATE_ESTABLISHED )
	{
		int count = m_pDBSession->GetRecvBuffer()->GetPacketCount();

		if ( bSendOK == false && m_bDBInit == false )
		{
			for ( SI32 startindex = 0 ; startindex < MAX_TRADESERVER_ORDER_LIST ; startindex += MAX_TRADESERVER_GET_LIST )
			{
				NTradeServerDB_Request_Get_ListInfo sendmsg(0,startindex,true);
				SendMsg((sPacketHeader*)&sendmsg);
			}

			bSendOK = true ;
		}

		SetSellInfo(); // 팔린 정보에 대한 요청

		// 매시 정각에만 검사한다!
		if ( m_sTime.wMinute == 0 && bTimeSwtich == false )
		{
			ReduceAction(); // 가격 깎는 것에 대한 요청
			bTimeSwtich = true ;
		}
		else
		{
			bTimeSwtich = false ;
		}

		for( int i = 0; i < count; ++i )
		{
			// 버퍼에서 패킷을 하나 꺼내온다 
			sPacketHeader *pPacket = (sPacketHeader *)( m_pDBSession->GetRecvBuffer()->GetFirstPacket() );

			if( pPacket != NULL ) // 패킷이 있다면
			{
				// 패킷 커맨드에 따른 처리
				switch( pPacket->usCmd )
				{
				case NTRADESERVERDB_RESPONSE_SET_SELLORDER:
					{
						NTradeServerDB_Response_Set_SellOrder* pclinfo =(NTradeServerDB_Response_Set_SellOrder*)pPacket;

						UI16 serverid = pclinfo->uiServerIndex ;

						ITZSession* pSession = g_GameProtocol.GetSessionManager()->GetSession((SI32)serverid);
						if ( pSession == NULL || pSession->GetState() == SESSION_STATE_NONE ) // 서버와 연결되어 있지 않다면 나간다.
						{
							if ( pclinfo->siResult == 1 )
							{
								FilePrint("error\\SellOrder.txt","ServerNation[%s] ServerIndex[%d] CharName[%s] PersonID[%d] ItemUnique[%d] ItemNum[%d] ItemPrice[%I64d] ListIndex[%d]",
									pclinfo->clOrder.szServerNation,pclinfo->clOrder.siServerIndex,pclinfo->clOrder.szCharName,pclinfo->clOrder.siPersonID,
									pclinfo->clOrder.clitem.siUnique,pclinfo->clOrder.clitem.siItemNum,pclinfo->clOrder.clitem.siPrice,pclinfo->clOrder.siIndex);
							}
							break;
						}

						if ( pclinfo->siResult == 1 )
						{
							clOrder[pclinfo->clOrder.siIndex].Set(&pclinfo->clOrder);
						}

						NTradeServerResponse_Set_SellOrder sendmsg(pclinfo->siResult,pclinfo->siCharID,pclinfo->siPos,&pclinfo->clOrder);
						pSession->GetSendBuffer()->Write((char*)&sendmsg);
					}
					break;
				case NTRADESERVERDB_RESPONSE_CANCEL_SELLORDER:
					{
						NTradeServerDB_Response_Cancel_SellOrder* pclinfo =(NTradeServerDB_Response_Cancel_SellOrder*)pPacket;

						UI16 serverid = pclinfo->uiServerIndex ;

						ITZSession* pSession = g_GameProtocol.GetSessionManager()->GetSession((SI32)serverid);
						if ( pSession == NULL || pSession->GetState() == SESSION_STATE_NONE ) // 서버와 연결되어 있지 않다면 나간다.
						{
							if ( pclinfo->siResult == 1 )
							{
								FilePrint("error\\CancelSellOrder.txt","ServerNation[%s] ServerIndex[%d] CharName[%s] PersonID[%d] ItemUnique[%d] ItemNum[%d] ItemPrice[%I64d] ListIndex[%d]",
									pclinfo->clOrder.szServerNation,pclinfo->clOrder.siServerIndex,pclinfo->clOrder.szCharName,pclinfo->clOrder.siPersonID,
									pclinfo->clOrder.clitem.siUnique,pclinfo->clOrder.clitem.siItemNum,pclinfo->clOrder.clitem.siPrice,pclinfo->clOrder.siIndex);
							}
							break;
						}

						NTradeServerResponse_Cancel_SellOrder sendmsg;
						sendmsg.packetHeader.usCmd = NTRADESERVERRESPONSE_CANCEL_SELLORDER;
						sendmsg.packetHeader.usSize = sizeof( NTradeServerResponse_Cancel_SellOrder );

						sendmsg.siResult = pclinfo->siResult ;
						sendmsg.siCharID = pclinfo->siCharID ;
						sendmsg.clOrder.Set(&pclinfo->clOrder) ;
						sendmsg.siPos = pclinfo->siPos ;

						if ( pclinfo->siResult == 1 )
						{
							clOrder[pclinfo->clOrder.siIndex].Init();
						}

						pSession->GetSendBuffer()->Write((char*)&sendmsg);
					}
					break;
				case NTRADESERVERDB_RESPONSE_SET_BUYORDER:
					{
						NTradeServerDB_Response_Set_BuyOrder* pclinfo =(NTradeServerDB_Response_Set_BuyOrder*)pPacket;

						UI16 serverid = pclinfo->uiServerIndex ;

						ITZSession* pSession = g_GameProtocol.GetSessionManager()->GetSession((SI32)serverid);
						if ( pSession == NULL || pSession->GetState() == SESSION_STATE_NONE ) // 서버와 연결되어 있지 않다면 나간다.
						{
							if ( pclinfo->siResult == 1 )
							{
								FilePrint("error\\BuyOrder.txt","BuyerServerNation[%s] BuyerServerIndex[%d] BuyerCharName[%s] BuyerPersonID[%d] SellerServerNation[%s] SellerServerIndex[%d] SellerCharName[%s] SellerPersonID[%d] ItemUnique[%d] ItemNum[%d] ItemPrice[%I64d] ListIndex[%d]",
									pclinfo->szServerNation,pclinfo->siServerIndex,pclinfo->szCharName,pclinfo->siPersonID,
									pclinfo->clOrder.szServerNation,pclinfo->clOrder.siServerIndex,pclinfo->clOrder.szCharName,pclinfo->clOrder.siPersonID,
									pclinfo->clOrder.clitem.siUnique,pclinfo->clOrder.clitem.siItemNum,pclinfo->clOrder.clitem.siPrice,pclinfo->clOrder.siIndex);
							}
							break;
						}

						NTradeServerResponse_Set_BuyOrder sendmsg;
						sendmsg.packetHeader.usCmd = NTRADESERVERRESPONSE_SET_BUYORDER;
						sendmsg.packetHeader.usSize = sizeof( NTradeServerResponse_Set_BuyOrder );

						sendmsg.siResult = pclinfo->siResult ; // 1외에는 모두 실패
						sendmsg.siCharID = pclinfo->siCharID; // 캐릭터 ID
						sendmsg.siPersonID = pclinfo->siPersonID; // 캐릭터 PersonID
						sendmsg.clOrder.Set(&pclinfo->clOrder); // 주문 내용
						sendmsg.siPos = pclinfo->siPos ;

						if ( pclinfo->siResult == 1 )
						{
							clOrder[pclinfo->clOrder.siIndex].Init();
						}

						pSession->GetSendBuffer()->Write((char*)&sendmsg);
					}
					break;
				case NTRADESERVERDB_RESPONSE_SET_SELLINFO:
					{
						NTradeServerDB_Response_Set_SellInfo* pclinfo =(NTradeServerDB_Response_Set_SellInfo*)pPacket;

						ITZSession* pSession = NULL ;
						for ( SI32 i = 0 ; i < g_GameProtocol.GetSessionManager()->m_siMaxSize ; i ++ )
						{
							if ( g_GameProtocol.GetSessionManager()->GetSession(i) == NULL )
								continue ;
							if ( g_GameProtocol.GetSessionManager()->GetSession(i)->GetState() != SESSION_STATE_ESTABLISHED )
							{
								g_GameProtocol.GetSessionManager()->DelSession(g_GameProtocol.GetSessionManager()->GetSession(i));
								if ( g_GameProtocol.GetSessionManager()->GetSession(i) != NULL )
								{
								}
								continue ;
							}

							if ( !strcmp(pclinfo->szServerIP,g_GameProtocol.GetSessionManager()->GetSession(i)->GetIP()) )
							{
								pSession = g_GameProtocol.GetSessionManager()->GetSession(i) ;
								break;
							}
						}

						if ( pSession == NULL || pSession->GetState() != SESSION_STATE_ESTABLISHED ) // 서버와 연결되어 있지 않다면 나간다.
						{
							if ( pclinfo->siResult == 1 )
							{
								FilePrint("error\\SellInfo.txt","ServerNation[%s] ServerIndex[%d] CharName[%s] PersonID[%d] ItemUnique[%d] ItemNum[%d] ItemPrice[%I64d] ListIndex[%d]",
									pclinfo->clLeftOrder.szServerNation,pclinfo->clLeftOrder.siServerIndex,pclinfo->clLeftOrder.szCharName,pclinfo->clLeftOrder.siPersonID,
									pclinfo->clLeftOrder.clitem.siUnique,pclinfo->clLeftOrder.clitem.siItemNum,pclinfo->clLeftOrder.clitem.siPrice,pclinfo->clLeftOrder.siIndex);
							}
							break;
						}

						NTradeServerResponse_Set_SellInfo sendmsg;
						sendmsg.packetHeader.usCmd = NTRADESERVERRESPONSE_SET_SELLINFO;
						sendmsg.packetHeader.usSize = sizeof( NTradeServerResponse_Set_SellInfo );

						sendmsg.siResult = pclinfo->siResult ;
						sendmsg.clLeftOrder.Set(&pclinfo->clLeftOrder);

						memcpy(sendmsg.szBuyerServerNation,pclinfo->szBuyerServerNation,4);
						memcpy(sendmsg.szBuyerName,pclinfo->szBuyerName,20);
						memcpy(sendmsg.szBuyerAccountID,pclinfo->szBuyerAccountID,20);
						sendmsg.siBuyerNation = pclinfo->siBuyerNation;
						sendmsg.siBuyerServerIndex = pclinfo->siBuyerServerIndex ;

						pSession->GetSendBuffer()->Write((char*)&sendmsg);
					}
					break;
				case NTRADESERVERDB_RESPONSE_GET_LISTINFO:
					{
						NTradeServerDB_Response_Get_ListInfo* pclinfo =(NTradeServerDB_Response_Get_ListInfo*)pPacket;

						if ( pclinfo->bDBInit == false )
						{
							UI16 serverid = pclinfo->uiServerIndex ;

							ITZSession* pSession = g_GameProtocol.GetSessionManager()->GetSession((SI32)serverid);
							if ( pSession == NULL || pSession->GetState() == SESSION_STATE_NONE ) // 서버와 연결되어 있지 않다면 나간다.
								break;

							NTradeServerResponse_Get_ListInfo sendmsg;
							sendmsg.packetHeader.usCmd = NTRADESERVERRESPONSE_GET_LISTINFO;
							sendmsg.packetHeader.usSize = sizeof( NTradeServerResponse_Get_ListInfo );

							sendmsg.siResult = pclinfo->siResult; // 1외에는 모두 실패
							sendmsg.siListStartIndex = pclinfo->siListStartIndex ;

							for ( SI32 i = 0 ; i < MAX_TRADESERVER_GET_LIST ; i ++ )
							{
								sendmsg.clOrder[i].Set(&pclinfo->clOrder[i]);
							}

							pSession->GetSendBuffer()->Write((char*)&sendmsg);
						}
						else
						{
							if ( pclinfo->siResult == 1 )
							{
								for ( SI32 i = pclinfo->siListStartIndex ; i < pclinfo->siListStartIndex + MAX_TRADESERVER_GET_LIST ; i ++ )
								{
									clOrder[i].Set(&pclinfo->clOrder[i-pclinfo->siListStartIndex]);
								}

								if ( pclinfo->siListStartIndex == MAX_TRADESERVER_ORDER_LIST - MAX_TRADESERVER_GET_LIST )
								{
									m_bDBInit = true ;
								}
							}
						}
					}
					break;
				case NTRADESERVERDB_RESPONSE_REDUCEPRICE:
					{
						NTradeServerDB_Response_ReducePrice* pclinfo = (NTradeServerDB_Response_ReducePrice*)pPacket;

						if ( pclinfo->siResult == 1 )
						{
							clOrder[pclinfo->clOrder.siIndex].Set(&pclinfo->clOrder);
						}
					}
					break;

				case NTRADESERVERDB_RESPONSE_WITHDRAW_ACU:
					{
						NTradeServerDB_Response_Withdraw_Acu* pclinfo = (NTradeServerDB_Response_Withdraw_Acu*)pPacket;
						if ( NULL == pclinfo )
						{
							break;
						}

						
						UI16 uiServerID = pclinfo->m_uiServerIndex;

						ITZSession* pSession = g_GameProtocol.GetSessionManager()->GetSession((SI32)uiServerID);
						if ( pSession == NULL || pSession->GetState() == SESSION_STATE_NONE ) // 서버와 연결되어 있지 않다면 나간다.
						{
							break;
						}

						NTradeServerResponse_Withdraw_Acu sendMsg;
						sendMsg.m_packetHeader.usCmd	= NTRADESERVERRESPONSE_WITHDRAW_ACU;
						sendMsg.m_packetHeader.usSize	= sizeof(sendMsg);

						sendMsg.m_siResult			= pclinfo->m_siResult;
						sendMsg.m_siPersonID		= pclinfo->m_siPersonID;
						sendMsg.m_siCharID			= pclinfo->m_siCharID;
						sendMsg.m_siWithdrawAcu		= pclinfo->m_siWithdrawAcu;
						sendMsg.m_siLeftSavedAcu	= pclinfo->m_siLeftSavedAcu;
						sendMsg.m_siReservedNum		= pclinfo->m_siReservedNum;
	
						
						pSession->GetSendBuffer()->Write((char*)&sendMsg);
					}
					break;

				case NTRADESERVERDB_RESPONSE_SAVEDACU_LIST:
					{
						NTradeServerDB_Response_SavedAcu_List* pclinfo = (NTradeServerDB_Response_SavedAcu_List*)pPacket;
						if ( NULL == pclinfo )
						{
							break;
						}

						UI16 uiServerID = pclinfo->m_uiServerIndex;

						ITZSession* pSession = g_GameProtocol.GetSessionManager()->GetSession((SI32)uiServerID);
						if ( pSession == NULL || pSession->GetState() == SESSION_STATE_NONE ) // 서버와 연결되어 있지 않다면 나간다.
						{
							break;
						}
						
						NTradeServerResponse_Get_SavedACU_List sendMsg;
						sendMsg.m_packetHeader.usCmd	= NTRADESERVERRESPONSE_GET_SAVEDACU_LIST;
						sendMsg.m_packetHeader.usSize	= sizeof(sendMsg);

						sendMsg.m_siResult			= pclinfo->m_siResult;
						sendMsg.m_siPersonID		= pclinfo->m_siPersonID;
						sendMsg.m_siCharID			= pclinfo->m_siCharID;
						sendMsg.m_siSavedACU		= pclinfo->m_siSavedAcu;
						memcpy( sendMsg.m_clAcuList, pclinfo->m_clAcuList, sizeof(sendMsg.m_clAcuList) );

						pSession->GetSendBuffer()->Write((char*)&sendMsg);

					}

				default:
					break;
				}

				// 버퍼에서 패킷 삭제
				m_pDBSession->GetRecvBuffer()->RemoveFirstPacket();
			}
		}
	}
}

void DBProcess::SendMsg(sPacketHeader *pPacket )
{

	if( m_pDBSession == NULL ) return;
	if( m_pDBSession->GetState() != SESSION_STATE_ESTABLISHED ) return;

	m_pDBSession->GetSendBuffer()->Write( (char *)pPacket );
}

void DBProcess::ReduceAction()
{
	if( m_pDBSession == NULL ) return;
	if( m_pDBSession->GetState() != SESSION_STATE_ESTABLISHED ) return;

	if ( m_bDBInit == false ) return ;

	cltTradeDate cldate;
	cldate.Init();
	cldate.uiYear = m_sTime.wYear - 2000 ;
	cldate.uiMonth = m_sTime.wMonth ;
	cldate.uiDay = m_sTime.wDay ;
	cldate.uiHour = m_sTime.wHour ;

	for ( SI32 i = 0 ; i < MAX_TRADESERVER_ORDER_LIST ; i ++ )
	{
		if ( clOrder[i].siPersonID > 0 && clOrder[i].clitem.siUnique > 0 )
		{
			bool bOverTime = false ;

			UI16 year = clOrder[i].clitem.clDate.uiYear + 2000 ;
			SI32 datevary = GetDateVary(year,clOrder[i].clitem.clDate.uiMonth,clOrder[i].clitem.clDate.uiDay) * 24 + clOrder[i].clitem.clDate.uiHour ;

			SI32 currentdatevary = GetDateVary(m_sTime.wYear,m_sTime.wMonth,m_sTime.wDay) * 24 + m_sTime.wHour ;

			// 24시간 차이 나면 가격을 깎는다.
			if ( currentdatevary - datevary >= 24 )
			{
				// 10아큐 이하로는 안 떨군다.
				if ( clOrder[i].clitem.siPrice <= 10 )
					continue;

				SI64 price = ( clOrder[i].clitem.siPrice * 95 ) / 100 ; // 95%로 만든다.
				if ( price <= 10 )
					price = 10 ;

				NTradeServerDB_Request_ReducePrice sendmsg(i,price,&cldate,clOrder[i].szCharName,clOrder[i].szServerNation,
					clOrder[i].siServerIndex,clOrder[i].clitem.siUnique);
				SendMsg((sPacketHeader*)&sendmsg);
			}
		}
	}
}

void DBProcess::SetSellInfo()
{
	ITZSession* pSession = NULL ;
	for ( SI32 i = 0 ; i < g_GameProtocol.GetSessionManager()->m_siMaxSize ; i ++ )
	{
		cGame* pGameServer = g_GameProtocol.GetGameServer( i );
		if( pGameServer == NULL )				continue;

		// 버젼 200(2.00) 부터는 게임서버에서 요청하는 경우에만 Set_SellInfo정보를 처리한다.
		if( pGameServer->m_siVersion < 200 )
		{
			pSession = pGameServer->GetSession() ;
			if ( pSession && pSession->GetState() == SESSION_STATE_ESTABLISHED )
			{
				NTradeServerDB_Request_Set_SellInfo sendmsg( pSession->GetIP() );
				SendMsg((sPacketHeader*)&sendmsg);
			}
		}

/*		if ( g_GameProtocol.GetSessionManager()->GetSession(i) == NULL )
			continue ;
		if ( g_GameProtocol.GetSessionManager()->GetSession(i)->GetState() == SESSION_STATE_ESTABLISHED )
		{
			pSession = g_GameProtocol.GetSessionManager()->GetSession(i) ;

			NTradeServerDB_Request_Set_SellInfo sendmsg(pSession->GetIP());
			SendMsg((sPacketHeader*)&sendmsg);
		}*/
	}
}

SI32 DBProcess::GetDateVary(UI16 uiYear,UI08 uiMonth,UI08 uiDay)
{
	UI16 year = uiYear - 1;
	int gday = year * 365;

	s_uiDaysOfMonth[ 2 ] += ((uiYear % 4) == 0 ) - ((uiYear % 100) == 0 ) + ((uiYear % 400) == 0 );

	gday += ( year / 4 ) - ( year / 100 ) + ( year / 400 );

	for( int i = 1; i < uiMonth; ++i ) 
	{
		gday += s_uiDaysOfMonth[ i ];
	}

	gday += uiDay;

	s_uiDaysOfMonth[ 2 ] = 28;

	return gday;
}

void DBProcess::FilePrint( char* filename, char *format,... )
{
	EnterCriticalSection(&m_LogCritical);

	va_list vl;    

	va_start( vl, format );
	wvsprintf( dbgbuf, format, vl );
	va_end( vl );

	FILE *fp = NULL;

	fp = fopen( filename, "a+" );

	if ( ! fp )
	{
		printf("File : Log.txt Make Error\n");

		LeaveCriticalSection(&m_LogCritical);

		return;
	}

	_strdate( m_Date );

	_strtime( m_Time );

	fprintf( fp, "%s\t%s\t%s\n", dbgbuf, m_Date, m_Time );

	fclose(fp);

	LeaveCriticalSection(&m_LogCritical);

}
