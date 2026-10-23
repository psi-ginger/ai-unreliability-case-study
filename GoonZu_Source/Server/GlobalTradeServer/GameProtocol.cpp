#include "stdafx.h"
#include "GameProtocol.h"
#include "DBProcess.h"

#include "../BeTheRich/Server/NTradeServerProcess/TradeServerMsg.h"

extern DBProcess g_DBProcess;
extern unsigned short g_listenport;

GameProtocol::GameProtocol()
{
	m_state = 0;
	m_recvCount = 0;
	m_sendCount = 0;

	m_dwCheckClock = 0;
	for( SI32 i = 0; i < MAX_SERVER_SESSION_NUM; ++i )
	{
		m_dwLasteceiveClock[i] = 0;
	}

	dbgbuf[0] = '\0';
	m_Time[0] = '\0';
	m_Date[0] = '\0';

	InitializeCriticalSection( &m_cs );	

	m_dwHeartBeatClock = 0;
}

GameProtocol::~GameProtocol()
{
	DeleteCriticalSection( &m_cs );
}

void GameProtocol::Init()
{
	m_SF.CreateFactory( GetSessionManager(), MAX_SERVER_SESSION_NUM, 1 * 5000 * 1000, 1 * 5000 * 1000 );

	GetNetIOCP()->CreateIOCP( GetSessionManager(), 1 );

	GetListen()->CreateListen( GetSessionManager(), g_listenport );

	SetFPS( 200 );
}


void GameProtocol::LogOut( ITZSession *pSession )
{
}

void GameProtocol::SessionProc( ITZSession *pSession )
{
	if( pSession == NULL )				return;
	if( m_dwHeartBeatClock == 0 )		m_dwHeartBeatClock = GetTickCount();

	// 버젼 200(2.00) 부터 게임서버에 HeartBeat 신호를 전송한다.
	cGame* pGameServer = m_SF.GetPerson( pSession->GetPerson()->GetCharID() );
	if( pGameServer && pGameServer->m_siVersion >= 200 )
	{
		pGameServer->Request_HeartBeat();
	}

	int count = pSession->GetRecvBuffer()->GetPacketCount();

	for( int i = 0; i < count; ++i ) {

		// 버퍼에서 패킷을 하나 꺼내온다 
		sPacketHeader *pPacket = (sPacketHeader *)( pSession->GetRecvBuffer()->GetFirstPacket() );

		if( pPacket == NULL ) return;		// 패킷이 없으면 나간다

		// 패킷 커맨드에 따른 처리
		switch( pPacket->usCmd ) {
			case NTRADESERVERREQUEST_SET_SELLORDER: // 주문을 올린다.
				{
					NTradeServerRequest_Set_SellOrder * pclinfo = (NTradeServerRequest_Set_SellOrder*)pPacket;

					UI16 serverid = pSession->m_usIndex ;

					m_dwLasteceiveClock[ serverid ] = timeGetTime();

					_SYSTEMTIME stime;
					GetLocalTime(&stime);

					cltTradeServerOrder clOrder;
					clOrder.Set(&pclinfo->clOrder);
					clOrder.clitem.clDate.uiYear = stime.wYear - 2000 ;
					clOrder.clitem.clDate.uiMonth = stime.wMonth ;
					clOrder.clitem.clDate.uiDay = stime.wDay ;
					clOrder.clitem.clDate.uiHour = stime.wHour ;

					NTradeServerDB_Request_Set_SellOrder sendmsg(serverid,pclinfo->siCharID,pclinfo->siPos,
																 &clOrder);
					g_DBProcess.SendMsg((sPacketHeader*)&sendmsg);
				}
				break;
			case NTRADESERVERREQUEST_CANCEL_SELLORDER: // 주문을 내린다.
				{
					NTradeServerRequest_Cancel_SellOrder* pclinfo = (NTradeServerRequest_Cancel_SellOrder*)pPacket;

					UI16 serverid = pSession->m_usIndex ;

					m_dwLasteceiveClock[ serverid ] = timeGetTime();

					NTradeServerDB_Request_Cancel_SellOrder sendmsg(serverid,pclinfo->siCharID,&pclinfo->clOrder,pclinfo->siPos);
					g_DBProcess.SendMsg((sPacketHeader*)&sendmsg);

				}
				break;
			case NTRADESERVERREQUEST_SET_BUYORDER: // 주문을 산다.
				{
					NTradeServerRequest_Set_BuyOrder* pclinfo = (NTradeServerRequest_Set_BuyOrder*)pPacket;

					UI16 serverid = pSession->m_usIndex;

					m_dwLasteceiveClock[ serverid ] = timeGetTime();

					NTradeServerDB_Request_Set_BuyOrder sendmsg(serverid,pclinfo->siCharID,pclinfo->siPersonID,
																pclinfo->szCharName,pclinfo->szAccountID,
																pclinfo->szServerNation,pclinfo->siServerIndex,pclinfo->siCharNation,
																&pclinfo->clOrder,pclinfo->siPos);
					g_DBProcess.SendMsg((sPacketHeader*)&sendmsg);
				}
				break;
			case NTRADESERVERREQUEST_DEL_SELLINFO:
				{
					NTradeServerRequest_Del_SellInfo* pclinfo = (NTradeServerRequest_Del_SellInfo*)pPacket;

					NTradeServerDB_Request_Del_SellInfo sendmsg(&pclinfo->clOrder);
					g_DBProcess.SendMsg((sPacketHeader*)&sendmsg);
				}
				break;
			case NTRADESERVERREQUEST_GET_LISTINFO: // 리스트를 요청한다.
				{
					NTradeServerRequest_Get_ListInfo* pclinfo = (NTradeServerRequest_Get_ListInfo*)pPacket;

					UI16 serverid = pSession->m_usIndex;

					m_dwLasteceiveClock[ serverid ] = timeGetTime();

					for ( SI32 index = 0 ; index < MAX_TRADESERVER_ORDER_LIST ; index += MAX_TRADESERVER_GET_LIST )
					{
						NTradeServerDB_Request_Get_ListInfo sendmsg(serverid,index,false);
						g_DBProcess.SendMsg((sPacketHeader*)&sendmsg);
					}
				}
				break;
				/*	case NTRADESERVERREQUEST_DELETE_SELLORDER:
				{
					NTradeServerRequest_Delete_SellOrder* pclinfo = (NTradeServerRequest_Delete_SellOrder*)pPacket;

					UI16 serverid = pSession->m_usIndex ;

					NTradeServerDB_Request_Delete_SellOrder sendmsg(pclinfo->siPersonID, serverid, pclinfo->szServerNation);
					g_DBProcess.SendMsg((sPacketHeader*)&sendmsg);
				}
				break;*/
			case NTRADESERVERREQUEST_SET_SELLORDER_HOLDACU:
				{
					NTradeServerRequest_Set_SellOrder_HoldAcu* pclinfo = (NTradeServerRequest_Set_SellOrder_HoldAcu*)pPacket;
					if ( NULL == pclinfo )		
					{
						break;
					}

					UI16 uiServerIndex = pSession->m_usIndex;

					m_dwLasteceiveClock[ uiServerIndex ] = timeGetTime();

					_SYSTEMTIME stime;
					GetLocalTime(&stime);

					cltTradeServerOrder	clOrder;
					clOrder.Set(&pclinfo->m_clOrder);
					clOrder.clitem.clDate.uiYear	= stime.wYear - 2000 ;
					clOrder.clitem.clDate.uiMonth	= stime.wMonth ;
					clOrder.clitem.clDate.uiDay		= stime.wDay ;
					clOrder.clitem.clDate.uiHour	= stime.wHour ;

					NTradeServerDB_Request_Set_SellOrder_HoldAcu sendmsg( uiServerIndex, pclinfo->m_siCharID, pclinfo->m_siPos, &clOrder, pclinfo->m_bHoldAcu );
					g_DBProcess.SendMsg((sPacketHeader*)&sendmsg);
				}
				break;

			case NTRADESERVERREQUEST_WITHDRAW_ACU:
				{
					NTradeServerRequest_Withdraw_Acu* pclinfo = (NTradeServerRequest_Withdraw_Acu*)pPacket;
					if ( NULL == pclinfo )
					{
						break;
					}

					UI16 uiServerIndex = pSession->m_usIndex;

					m_dwLasteceiveClock[ uiServerIndex ] = timeGetTime();

					SI32 siCharID		= pclinfo->m_siCharID;
					SI32 siPersonID		= pclinfo->m_siPersonID;
					SI16 siServerIndex	= pclinfo->m_siServerIndex;
					SI64 siWithdrawAcu	= pclinfo->m_siWithdrawAcu;

					NTradeServerDB_Request_Withdraw_Acu sendmsg( uiServerIndex, siCharID, siPersonID, pclinfo->m_szServerNation, siServerIndex, siWithdrawAcu );
					g_DBProcess.SendMsg((sPacketHeader*)&sendmsg);
				}
				break;

			case NTRADESERVERREQUEST_WITHDRAW_ACU_SUCCESS:
				{
					NTradeServerRequest_Withdraw_Acu_Success* pclinfo = (NTradeServerRequest_Withdraw_Acu_Success*)pPacket;
					if ( NULL == pclinfo )
					{
						break;
					}

					NTradeServerDB_Request_Withdraw_Acu_Success sendmsg( pclinfo->m_siReservedNum );
					g_DBProcess.SendMsg((sPacketHeader*)&sendmsg);
				}
				break;

			case NTRADESERVERREQUEST_GET_SAVEDACU_LIST:
				{
					NTradeServerRequest_Get_SavedACU_List* pclinfo = (NTradeServerRequest_Get_SavedACU_List*)pPacket;
					if ( NULL == pclinfo )
					{
						break;
					}

					UI16 uiServerIndex = pSession->m_usIndex;

					m_dwLasteceiveClock[ uiServerIndex ] = timeGetTime();

					SI32 siCharID		= pclinfo->m_siCharID;
					SI32 siPersonID		= pclinfo->m_siPersonID;
					SI16 siServerIndex	= pclinfo->m_siServerIndex;

					NTradeServerDB_Request_SavedAcu_List sendmsg( uiServerIndex, siCharID, siPersonID, pclinfo->m_szServerNation, siServerIndex );
					g_DBProcess.SendMsg((sPacketHeader*)&sendmsg);



				}
				break;
			case NTRADESERVERREQUEST_HEARTBEAT: // HeartBeat 신호에 대한 결과를 받음
				{
					NTradeServerRequest_HeartBeat* pclinfo = (NTradeServerRequest_HeartBeat*)pPacket;
					if ( NULL == pclinfo )		{		break;			}

					cGame* pGameServer = m_SF.GetPerson( pSession->GetPerson()->GetCharID() );
					if( pGameServer )	{	pGameServer->Received_HeartBeat();		}
				}
				break;

			case NTRADESERVERREQUEST_SET_SERVERVERSION:	// 서버 버젼 설정
				{
					NTradeServerRequest_Set_ServerVersion* pclinfo = (NTradeServerRequest_Set_ServerVersion*)pPacket;
					if ( NULL == pclinfo )		{		break;			}

					cGame* pGameServer = m_SF.GetPerson( pSession->GetPerson()->GetCharID() );
					if( pGameServer )
					{
                        pGameServer->m_siVersion = pclinfo->m_siVersion;

						NTradeServerResponse_Set_ServerVersion clMsg_Version( pGameServer->m_siVersion );
						pSession->GetSendBuffer()->Write( (char*)&clMsg_Version );
					}
				}
				break;

			case NTRADESERVERREQUEST_SET_SELLINFO:
				{
					NTradeServerRequest_Set_SellInfo* pclinfo = (NTradeServerRequest_Set_SellInfo*)pPacket;

					// DB로 SellInfo를 요청한다.
					NTradeServerDB_Request_Set_SellInfo sendmsg( pSession->GetIP() );
					g_DBProcess.SendMsg((sPacketHeader*)&sendmsg);
				}
				break;

			default:
				break;
		}

		pSession->GetRecvBuffer()->RemoveFirstPacket();
	}
}

void GameProtocol::Update()
{
}

void GameProtocol::FilePrint( char* filename, char *format,... )
{
	EnterCriticalSection(&m_cs);

	va_list vl;    

	va_start( vl, format );
	wvsprintf( dbgbuf, format, vl );
	va_end( vl );

	FILE *fp = NULL;

	fp = fopen( filename, "a+" );

	if ( ! fp )
	{
		printf("File : Log.txt Make Error\n");

		LeaveCriticalSection(&m_cs);

		return;
	}

	_strdate( m_Date );

	_strtime( m_Time );

	fprintf( fp, "%s\t%s\t%s\n", dbgbuf, m_Date, m_Time );

	fclose(fp);

	LeaveCriticalSection(&m_cs);
}

cGame* GameProtocol::GetGameServer( UI16 uiCharID )
{
	return m_SF.GetPerson( uiCharID );
}