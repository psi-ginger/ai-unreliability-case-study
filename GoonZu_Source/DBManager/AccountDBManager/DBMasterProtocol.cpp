#include "DBMasterProtocol.h"
#include "AccountDBMsg.h"

DBMasterProtocol::DBMasterProtocol()
{

}

DBMasterProtocol::~DBMasterProtocol()
{
	m_odbc.Disconnect();
}

void DBMasterProtocol::Init( UI16 usPort, char *szDBIP, char *szDatabase, char *szUserID, char *szUserPW )
{
	m_odbc.Connect( "SQL Server", szDBIP, szDatabase, szUserID, szUserPW );

	m_SF.CreateFactory( GetSessionManager(), 1, 1000 * 1000, 1000 * 1000 );
	
	GetNetIOCP()->CreateIOCP( GetSessionManager(), 1 );
	
	GetListen()->CreateListen( GetSessionManager(), usPort );

	SetFPS( 80 );
}

void DBMasterProtocol::Update()
{

}

void DBMasterProtocol::SessionProc( ITZSession *pSession )
{
	// 버퍼에서 패킷을 하나 꺼내온다 
	sPacketHeader *pPacket = (sPacketHeader *)( pSession->GetRecvBuffer()->GetFirstPacket() );
																							
	if( pPacket == NULL ) return;	// 패킷이 없으면 나간다

	//m_odbc.ConnectProcess();

	// 패킷 커맨드에 따른 처리
	switch( pPacket->usCmd ) {
	case DBMSG_REQUEST_ADMINLOGIN:
		{

			printf( "DBMSG_REQUEST_ADMINLOGIN\n" );

			// 받은 패킷
			sDBRequest_AdminLogin	*pRecvMsg = (sDBRequest_AdminLogin *)pPacket;

			pRecvMsg->userid[ sizeof(pRecvMsg->userid) - 1 ] = NULL;
			pRecvMsg->userpw[ sizeof(pRecvMsg->userpw) - 1 ] = NULL;
			//pRecvMsg->userip[ sizeof(pRecvMsg->userip) - 1 ] = NULL;	-- IP 제한 삭제 - 마스터 권한으로 명령어 제한 - by LEEKH 2008.01.30

			// 보낼 패킷
			sDBResponse_AdminLogin	sendMsg;

			ZeroMemory( &sendMsg, sizeof( sendMsg ) );

			sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_ADMINLOGIN;
			sendMsg.packetHeader.usSize = sizeof( sendMsg );

			sendMsg.usMasterCharID = pRecvMsg->usMasterCharID;
			memcpy( sendMsg.userid, pRecvMsg->userid, sizeof(sendMsg.userid) );
			
			BEGIN_SP( "rsp_IsManager" )
				m_odbc.SetParam( SQL_VARCHAR, pRecvMsg->userid, sizeof(pRecvMsg->userid) - 1 );
				m_odbc.SetParam( SQL_VARCHAR, pRecvMsg->userpw, sizeof(pRecvMsg->userpw) - 1 );
				//m_odbc.SetParam( SQL_VARCHAR, pRecvMsg->userip, sizeof(pRecvMsg->userip) - 1 );	-- IP 제한 삭제 - 마스터 권한으로 명령어 제한 - by LEEKH 2008.01.30
			END_SP()

			EXEC_SQL()

			int result = 0;
			BEGIN_FETCHDATA()
				m_odbc.GetData( &result, 0 );
			END_FETCHDATA()

			sendMsg.usResult = result;
			
			printf( "MasterCharID:%d, userid:%s, gameMasterLevel:%d\n", 
				pRecvMsg->usMasterCharID, 
				pRecvMsg->userid, 
				sendMsg.usResult ); 

			// send
			pSession->GetSendBuffer()->Write( (BYTE*)&sendMsg );
		}
		break;

	case DBMSG_REQUEST_NEWCHARNUM:
		{
/*
			sDBRequest_NewCharNum *pRecvMsg = (sDBRequest_NewCharNum *)pPacket;

			// 보낼 패킷
			sDBResponse_NewCharNum	sendMsg;

			ZeroMemory( &sendMsg, sizeof( sendMsg ) );

			sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_NEWCHARNUM;
			sendMsg.packetHeader.usSize = sizeof( sendMsg );

			sendMsg.siCount = pRecvMsg->siCount;

			for( int i = 0; i < pRecvMsg->siCount; ++i ) {

				BEGIN_SP( "rsp_PersonStatistics" )
					m_odbc.SetParam( SQL_VARCHAR, pRecvMsg->szServerName[ i ], sizeof(pRecvMsg->szServerName[ i ]) );
				END_SP()

				EXEC_SQL()

				BEGIN_FETCHDATA()
					m_odbc.GetData(&sendMsg.siNewCharNum[ i ], 0 );
					m_odbc.GetData(&sendMsg.siDelCharNum[ i ], 0 );
				END_FETCHDATA()
			}

			// send
			pSession->GetSendBuffer()->Write( (BYTE*)&sendMsg );
*/			

		}
		break;

	case DBMSG_REQUEST_RECORDUSERNUM:
		{
			
			printf( "DBMSG_REQUEST_RECORDUSERNUM\n" );

			char ip[ 20 ];
			ip[ 0 ] = NULL;

			// 받은 패킷
			sDBRequest_RecordUserNum *pRecvMsg = (sDBRequest_RecordUserNum *)pPacket;

			BEGIN_SP( "rsp_CurrentUsersSet" )
				m_odbc.SetParam(SQL_INTEGER, &pRecvMsg->siUserNum, sizeof(pRecvMsg->siUserNum));
			END_SP()

			EXEC_SQL()

			while( m_odbc.FetchData() > 0 ) {
				;
			}
			
		}
		break;

	case DBMSG_REQUEST_RECORDSERVERUSERNUM:
		{
			printf( "DBMSG_REQUEST_RECORDSERVERUSERNUM\n" );

			sDBRequest_RecordServerUserNum *pRecvMsg = (sDBRequest_RecordServerUserNum *)pPacket;

			BEGIN_SP( "rsp_RecordServerUserNum" )
				m_odbc.SetParam(SQL_VARCHAR, &pRecvMsg->szServerName, sizeof(pRecvMsg->szServerName));
				m_odbc.SetParam(SQL_VARCHAR, &pRecvMsg->szServerIP, sizeof(pRecvMsg->szServerIP));
				m_odbc.SetParam(SQL_INTEGER, &pRecvMsg->siUserNum, sizeof(pRecvMsg->siUserNum));
			END_SP()

			EXEC_SQL()
		}
		break;

	}

	pSession->GetRecvBuffer()->RemoveFirstPacket();
}
