#include "DBLogProtocol.h"

#define DBMessageMap(AAA,BBB) m_pDBMsgPtr[AAA].Func = &DBLogProtocol::BBB

DBLogProtocol::DBLogProtocol()
{
	m_pDBMsgPtr = new stDBMsgPtr[TRADELOG_DBMSG_END];

	if(m_pDBMsgPtr == NULL)
		MessageBox(NULL, "함수포인터 할당 실패", NULL, MB_OK);
	else
	{
		// 알 수 없는 메시지가 왔을때 처리할 함수를 할당해 준다.
		for(int i = 0; i < TRADELOG_DBMSG_END; i++)
			m_pDBMsgPtr[i].Func = &DBLogProtocol::NullFuncPtr;

		DBMessageMap( TRADELOG_DBMSG_REQUEST_SELLED_LOG,	DoMsg_DBMSG_REQUEST_SELLED_LOG );
		DBMessageMap( TRADELOG_DBMSG_REQUEST_TRADE_LOG,		DoMsg_DBMSG_REQUEST_TRADE_LOG );
	}
}

DBLogProtocol::~DBLogProtocol()
{
	SI32 i;
	for(i=0; i<MAX_USERS; i++)
	{
		m_ThreadUser[i].m_bThreadRunning = FALSE;
		WaitForSingleObject(m_ThreadUser[i].m_hThread, 100);
	}

	for(i=0; i<MAX_USERS; i++)
	{
		m_ThreadUser[i].DBDisconnect();
	}
}

void DBLogProtocol::Init( UI16 usPort, char *szDBIP, char *szDatabase, char *szUserID, char *szUserPW )
{
	m_SF.CreateFactory( GetSessionManager(), MAX_USERS, 2 * 1000 * 1000, 2 * 1000 * 1000 );

	for(SI32 i=0; i<MAX_USERS; i++)
	{
		cLogClient* pClient = m_SF.GetPerson( i+1 );
		m_ThreadUser[i].m_pClient = pClient;
		if(m_ThreadUser[i].DBConnect( "SQL Server", szDBIP, szDatabase, szUserID, szUserPW ))
		{
			printf( "Database Connect Ok [%d]\n", pClient->GetCharID() );
		}
		m_ThreadUser[i].m_hThread = (HANDLE)_beginthread(ThreadRunning, 0, &m_ThreadUser[i]);
	}

	GetNetIOCP()->CreateIOCP( GetSessionManager(), 1 );

	GetListen()->CreateListen( GetSessionManager(), usPort );                                                      

	SetFPS( 20 );
}

void DBLogProtocol::Update()
{

}

void DBLogProtocol::SessionProc( ITZSession *pSession )
{
	SI32 count = pSession->GetRecvBuffer()->GetPacketCount();

	// 현재 버퍼에 있는 패킷 개수 만큼 메시지 처리 루틴 반복 
	for( int i = 0; i < count; ++i ) 
	{
		// 버퍼에서 패킷을 하나 꺼내온다 
		sPacketHeader* pPacket = (sPacketHeader *)( pSession->GetRecvBuffer()->GetFirstPacket() );

		if( pPacket == NULL ) return;	// 패킷이 없으면 나간다

		MessageProc( pPacket , pSession);

		pSession->GetRecvBuffer()->RemoveFirstPacket();

	}
}

void DBLogProtocol::MessageProc(sPacketHeader* pPacket, ITZSession* pSession)
{
	if( pPacket == NULL )	return;
	if( pSession == NULL )	return;

	if(pPacket->usCmd >= 0 && pPacket->usCmd <= TRADELOG_DBMSG_END)
	{
		(this->*(this->m_pDBMsgPtr[pPacket->usCmd].Func))(pPacket, pSession);
	}
	else
	{
		NullFuncPtr(pPacket, pSession);
	}
}

void DBLogProtocol::NullFuncPtr(sPacketHeader* pPacket, ITZSession* pSession)
{
	char buffer[128];
	sprintf(buffer, "Not Defined Msg:%d", pPacket->usCmd);
	MessageBox(NULL, "DBManager Error", buffer, MB_OK);
}

void DBLogProtocol::DoMsg_DBMSG_REQUEST_SELLED_LOG(sPacketHeader* pPacket, ITZSession* pSession)
{
	// 받은 패킷
	sDBRequest_SelledItemLog	*pRecvMsg = (sDBRequest_SelledItemLog *)pPacket;

	cLogClient* pClient = (cLogClient*)pSession->GetPerson();
	pClient->SetSelledItemLog( pRecvMsg );
}

void DBLogProtocol::DoMsg_DBMSG_REQUEST_TRADE_LOG(sPacketHeader* pPacket, ITZSession* pSession)
{
	// 받은 패킷
	sDBRequest_TradeItemLog	*pRecvMsg = (sDBRequest_TradeItemLog *)pPacket;

	cLogClient* pClient = (cLogClient*)pSession->GetPerson();
	pClient->SetTradeItemLog( pRecvMsg );
}
