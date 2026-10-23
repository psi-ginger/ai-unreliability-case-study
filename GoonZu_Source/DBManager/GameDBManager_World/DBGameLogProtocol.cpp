#include "DBGameLogProtocol.h"
#include "DBGameLogMsg.h"

#define DBMessageMap(AAA,BBB) m_pDBMsgPtr[AAA].Func = &DBGameLogProtocol::BBB

DBGameLogProtocol::DBGameLogProtocol()
{
	m_pDBMsgPtr = new stDBMsgPtr[DBLOGMSG_END];

	if(m_pDBMsgPtr == NULL)
		MessageBox(NULL, "함수포인터 할당 실패", NULL, MB_OK);
	else
	{
		// 알 수 없는 메시지가 왔을때 처리할 함수를 할당해 준다.
		for(int i = 0; i < DBLOGMSG_END; i++)
			m_pDBMsgPtr[i].Func = &DBGameLogProtocol::NullFuncPtr;
		
		DBMessageMap(DBLOGMSG_REQUEST_PERSONAL_LOG, PersonalLog);
	}
}

DBGameLogProtocol::~DBGameLogProtocol()
{
	m_odbc.Disconnect();
	if(m_pDBMsgPtr != NULL)
		delete [] m_pDBMsgPtr;
}


void DBGameLogProtocol::Init( UI16 usPort, char *szDBIP, char *szDatabase, char *szUserID, char *szUserPW )
{
	m_odbc.Connect( "SQL Server", szDBIP, szDatabase, szUserID, szUserPW );

	m_SF.CreateFactory( GetSessionManager(), 1, 5000 * 1000, 5000 * 1000 );
	
	GetNetIOCP()->CreateIOCP( GetSessionManager(), 1 );
	
	GetListen()->CreateListen( GetSessionManager(), usPort );

	SetFPS( 80 );
}

void DBGameLogProtocol::Update()
{
	
}

void DBGameLogProtocol::MessageProc()
{
	if( m_pPacket == NULL ) return;

	if(m_pPacket->usCmd >= 0 && m_pPacket->usCmd <= DBLOGMSG_END)
	{
		(this->*(this->m_pDBMsgPtr[m_pPacket->usCmd].Func))();
		//PersonalLog();
	}
	else
	{
		NullFuncPtr();
	}
}




void DBGameLogProtocol::SessionProc( ITZSession *pSession )
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

		InterlockedIncrement( (LPLONG)&g_requestCount );

		if(!m_odbc.GetReconnectStat())
			pSession->GetRecvBuffer()->RemoveFirstPacket();

	}

}

void DBGameLogProtocol::SendPacket( BYTE *pPacket )
{
	if( m_pSession->GetSendBuffer()->Write( pPacket ) == FALSE )
	{
		OutputDebugString("!!!!!! ---- Write Buffer is Full ---- !!!!!!");
	}

	InterlockedIncrement( (LPLONG)&g_responseCount );
}

void DBGameLogProtocol::NullFuncPtr()
{
	char buffer[128];
	sprintf(buffer, "Not Defined Msg:%d", m_pPacket->usCmd);
	MessageBox(NULL, "DBManager Error", buffer, MB_OK);
}

void DBGameLogProtocol::PersonalLog()
{
	// 받은 패킷
	sDBRequest_Personal_Log *pRecvMsg = (sDBRequest_Personal_Log *)m_pPacket;
	
	// 보낼 패킷
	sDBResponse_Personal_Log sendMsg;
	
	sendMsg.packetHeader.usCmd = DBLOGMSG_RESPONSE_PERSONAL_LOG;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->usCharID;

	// DB Query
	BEGIN_SP( "rsp_PersonalLog" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siLogType, sizeof(pRecvMsg->siLogType) );
		m_odbc.SetParam( SQL_VARCHAR, pRecvMsg->szServerName, sizeof(pRecvMsg->szServerName) );
		m_odbc.SetParam( SQL_VARCHAR, pRecvMsg->szPersonName, sizeof(pRecvMsg->szPersonName) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siItemUnique, sizeof(pRecvMsg->siItemUnique) );
		m_odbc.SetParam( SQL_VARCHAR, pRecvMsg->szStartDate, sizeof(pRecvMsg->szStartDate) );
		m_odbc.SetParam( SQL_VARCHAR, pRecvMsg->szEndDate, sizeof(pRecvMsg->szEndDate) );
	END_SP()

	EXEC_SQL()

	BOOL bSend = FALSE;
	SI32 siCount = 0;
	BEGIN_FETCHDATA()
		bSend = FALSE;
		m_odbc.GetData( sendMsg.stLogData[siCount].szDate, sizeof(sendMsg.stLogData[siCount].szDate) );
		m_odbc.GetData( sendMsg.stLogData[siCount].szCommand, sizeof(sendMsg.stLogData[siCount].szCommand) );
		m_odbc.GetData( sendMsg.stLogData[siCount].szPersonName, sizeof(sendMsg.stLogData[siCount].szPersonName) );
		m_odbc.GetData( sendMsg.stLogData[siCount].szDstPersonName, sizeof(sendMsg.stLogData[siCount].szDstPersonName) );
		m_odbc.GetData( &sendMsg.stLogData[siCount].siItemUnique, 0 );
		m_odbc.GetData( sendMsg.stLogData[siCount].szLogTxt, sizeof(sendMsg.stLogData[siCount].szLogTxt) );
		siCount++;
		if(MAX_STLOGDATA_PER_PACKET == siCount)
		{
			sendMsg.siResult = 1;
			SendPacket( (BYTE *)&sendMsg );
			ZeroMemory(&sendMsg, sizeof(sendMsg));
			sendMsg.packetHeader.usCmd = DBLOGMSG_RESPONSE_PERSONAL_LOG;
			sendMsg.packetHeader.usSize = sizeof( sendMsg );
			sendMsg.usCharID = pRecvMsg->usCharID;

			bSend = TRUE;
			siCount = 0;
		}
	END_FETCHDATA()

	// 더이상 보낼 packet 이 없으면 -1로 설정한다.
	sendMsg.siResult = -1;
	SendPacket( (BYTE *)&sendMsg );

}