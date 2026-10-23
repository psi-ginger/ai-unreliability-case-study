#include "DBGameProtocol2.h"
#include "DBMsg.h"
#include "DBMsg-Web.h"

#define DBMessageMap(AAA,BBB) m_pDBMsgPtr[AAA].Func = &DBGameProtocol2::BBB

DBGameProtocol2::DBGameProtocol2()
{
	m_pDBMsgPtr = new stDBMsgPtr[DBMSG_END];
	if(m_pDBMsgPtr == NULL)
		MessageBox(NULL, "함수포인터 할당 실패", NULL, MB_OK);
	else
	{
		// 알 수 없는 메시지가 왔을때 처리할 함수를 할당해 준다.
		for(int i = 0; i < DBMSG_END; i++)
			m_pDBMsgPtr[i].Func = &DBGameProtocol2::NullFuncPtr;

		DBMessageMap(DBMSG_REQUEST_NEWS, News);
	}
}

DBGameProtocol2::~DBGameProtocol2()
{
	m_odbc.Disconnect();
	if(m_pDBMsgPtr != NULL)
		delete [] m_pDBMsgPtr;
}


void DBGameProtocol2::Init( UI16 usPort, char *szDBIP, char *szDatabase, char *szUserID, char *szUserPW )
{
	m_odbc.Connect( "SQL Server", szDBIP, szDatabase, szUserID, szUserPW );

	m_SF.CreateFactory( GetSessionManager(), 1, 5000 * 1000, 5000 * 1000 );
	
	GetNetIOCP()->CreateIOCP( GetSessionManager(), 1 );
	
	GetListen()->CreateListen( GetSessionManager(), usPort );

	SetFPS( 80 );
}

void DBGameProtocol2::Update()
{
	
}

void DBGameProtocol2::MessageProc()
{
	if( m_pPacket == NULL ) return;

	if(m_pPacket->usCmd >= 0 && m_pPacket->usCmd < DBMSG_END)
	{
		(this->*(this->m_pDBMsgPtr[m_pPacket->usCmd].Func))();
	}
	else
	{
		NullFuncPtr();
	}
}




void DBGameProtocol2::SessionProc( ITZSession *pSession )
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

void DBGameProtocol2::SendPacket( BYTE *pPacket )
{
	m_pSession->GetSendBuffer()->Write( pPacket );

	InterlockedIncrement( (LPLONG)&g_responseCount );
}

void DBGameProtocol2::NullFuncPtr()
{
	char buffer[128];
	sprintf(buffer, "Not Defined Msg:%d", m_pPacket->usCmd);
	MessageBox(NULL, "DBManager Error", buffer, MB_OK);
}
