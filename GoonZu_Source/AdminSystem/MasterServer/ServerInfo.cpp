#include "ServerInfo.h"

#define	SECTION_FRONTSERVER			1
#define	SECTION_AUTHSERVER			2
#define	SECTION_ACCOUNTDBMANAGER	3
#define	SECTION_ACCOUNTDB			4
#define	SECTION_WORLDINFO			5
#define	SECTION_GAMESERVER			6
#define	SECTION_GAMEDBMANAGER		7
#define	SECTION_GAMEDB				8
#define	SECTION_TOOLSLOG			9
#define	SECTION_LOGDB				10
#define	SECTION_MASTERSERVER		11
#define	SECTION_BILLINGSERVER		12

ServerInfo::ServerInfo()
{
	m_usServerCount = 0;

	m_usWorldCount = 0;
	m_usWorldID = 0;	

	m_dwFrontServerTick = 0;
	m_dwAuthServerTick = 0;
	m_dwAccountDBManagerTick = 0;
		
	for( int i = 0; i < 100; ++i ) {
		m_dwGameServerTick[ i ] = 0;
		m_siNewCharNum[ i ] = 0;
		m_siDelCharNum[ i ] = 0;
	}

	m_dwAccountDBManagerTick = 0;

	InitializeCriticalSection( &m_cs );
}

ServerInfo::~ServerInfo()
{
	DeleteCriticalSection( &m_cs );	
}

void ServerInfo::ReadFromFile()
{

	m_usServerCount = 0;

	m_usWorldCount = 0;

	DWORD dwBytesRead;

	HANDLE hFile = CreateFile( "serverinfo.txt", GENERIC_READ, 0, 0, OPEN_EXISTING, 0, 0 );

	if( hFile == INVALID_HANDLE_VALUE ) return;

	BOOL b;

	char	ch;
	char	buf[ 3 ][ 128 ];
		
	int		bufindex = 0;
	int		section = 0;
	
	int		len		= 0;
	BOOL	bStr	= FALSE;

	BOOL	bRun	= TRUE;
	BOOL	bDQ		= FALSE;
	BOOL	bProcess = FALSE;

	while( bRun ) {

		b = ReadFile( hFile, &ch, 1, &dwBytesRead, NULL );

		if( dwBytesRead == 0 ) { ch = 13; bRun = FALSE; };

		if( bStr ) {

			if( bDQ == TRUE && ch == '\"' ) { bDQ = FALSE; ch = ' '; };

			if( bDQ == FALSE && ( ch == ' ' || ch == '=' || ch == 9 ) ) {

				buf[ bufindex ][ len ] = NULL;
				if( bufindex < 2 ) bufindex++;

				len = 0;
				bStr = FALSE;

			} else if( ch == 10 || ch == 13 ) {

				buf[ bufindex ][ len ] = NULL;
				if( bufindex < 2 ) bufindex++;

				bStr = FALSE;

			} else {

				buf[ bufindex ][ len ] = ch;
				len++;
			}

		} else {

			if( ch == '\"' ) { 

				bStr = TRUE;
				bDQ = TRUE;

			} else if( !(ch == ' ' || ch == '=' || ch == 9 || ch == 10 || ch == 13) ) {

				buf[ bufindex ][ len ] = ch;
				len++;

				bStr = TRUE;
				bProcess = FALSE;
			}

		}

		if( bProcess == FALSE && ( ch == 10 || ch == 13 ) ) {

			if( bufindex == 1 ) {

				if( buf[ 0 ][ 0 ] == '/' && buf[ 0 ][ 1 ] == '/' ) {

				} else if( _stricmp( buf[ 0 ], "[frontserver]" ) == 0 ) {
					
					section = SECTION_FRONTSERVER;
				
				} else if( _stricmp( buf[ 0 ], "[authserver]" ) == 0 ) {

					section = SECTION_AUTHSERVER;

				} else if( _stricmp( buf[ 0 ], "[accountdbmanager]" ) == 0 ) {

					section = SECTION_ACCOUNTDBMANAGER;

				} else if( _stricmp( buf[ 0 ], "[accountdb]" ) == 0 ) {

					section = SECTION_ACCOUNTDB;

				}else if( _stricmp( buf[ 0 ], "[toolslog]" ) == 0 ) {

					section = SECTION_TOOLSLOG;

				}else if( _stricmp( buf[ 0 ], "[worldinfo]" ) == 0 ) {

					m_usWorldID = 0;
					
					section = SECTION_WORLDINFO;

				} else if( _stricmp( buf[ 0 ], "[gameserver]" ) == 0 ) {

					section = SECTION_GAMESERVER;

				} else if( _stricmp( buf[ 0 ], "[gamedbmanager]" ) == 0 ) {

					section = SECTION_GAMEDBMANAGER;

				} else if( _stricmp( buf[ 0 ], "[gamedb]" ) == 0 ) {

					section = SECTION_GAMEDB;
				} else if( _stricmp( buf[ 0 ], "[logdb]" ) == 0 ) {

					section = SECTION_LOGDB;
				} else if( _stricmp( buf[ 0 ], "[masterserver]" ) == 0 ) {

					section = SECTION_MASTERSERVER;
				} else if( _stricmp( buf[ 0 ], "[billingserver]" ) == 0 ) {

					section = SECTION_BILLINGSERVER;
				}

			} else if( bufindex > 1 ) {

				switch( section ) {
				case SECTION_FRONTSERVER:

					_setFrontServerInfo( buf[ 0 ], buf[ 1 ] );
					break;

				case SECTION_AUTHSERVER:

					_setAuthServerInfo( buf[ 0 ], buf[ 1 ] );
					break;

				case SECTION_ACCOUNTDBMANAGER:

					_setAccountDBManagerInfo( buf[ 0 ], buf[ 1 ] );
					break;

				case SECTION_ACCOUNTDB:

					_setAccountDBInfo( buf[ 0 ], buf[ 1 ] );
					break;

				case SECTION_TOOLSLOG:
					{
#ifdef _CHINA
					_setToolsLogInfo( buf[ 0 ], buf[ 1 ] );
#endif
					}
					break;

				case SECTION_WORLDINFO:

					_setGameWorldInfo( buf[ 0 ], buf[ 1 ] );
					break;

				case SECTION_GAMESERVER:

					_setGameServerInfo( buf[ 0 ], buf[ 1 ] );
					break;

				case SECTION_GAMEDBMANAGER:

					_setGameDBManagerInfo( buf[ 0 ], buf[ 1 ] );
					break;

				case SECTION_GAMEDB:
	
					_setGameDBInfo( buf[ 0 ], buf[ 1 ] );
					break;

				case SECTION_LOGDB:

					_setLogDBInfo( buf[ 0 ], buf[ 1 ] );
					break;
					
				case SECTION_MASTERSERVER:

					_setMasterServerInfo( buf[ 0 ], buf[ 1 ] );
					break;

				case SECTION_BILLINGSERVER:

					_setBillingServerInfo( buf[ 0 ], buf[ 1 ] );
					break;
					
				};
				
			}

			bufindex = 0;
			len = 0;
			bDQ = FALSE;
			bProcess = TRUE;
		}

	}

	CloseHandle( hFile );

	_setServerInfo();
}


void ServerInfo::_setServerBaseInfo( sServerBaseInfo *pStruct, char *buf1, char *buf2 )
{

	if( _stricmp( buf1, "serverid" ) == 0 ) {

		pStruct->siServerID = atoi( buf2 );

	} else if( _stricmp( buf1, "publicip" ) == 0 ) {

		strcpy( pStruct->szPublicIP, buf2 );

	} else if( _stricmp( buf1, "privateip" ) == 0 ) {

		strcpy( pStruct->szPrivateIP, buf2 );

	} else if( _stricmp( buf1, "execfile" ) == 0 ) {

		strcpy( pStruct->szFilePath, buf2 );

	}
}


void ServerInfo::_setFrontServerInfo( char *buf1, char *buf2 )
{
	_setServerBaseInfo( &m_frontServerInfo, buf1, buf2 );

	if( _stricmp( buf1, "clientlistenport" ) == 0 ) {

		m_frontServerInfo.usClientListenPort = atoi( buf2 );

	} else if( _stricmp( buf1, "gameserverlistenport" ) == 0 ) {

		m_frontServerInfo.usGameServerListenPort = atoi( buf2 );

	}
}


void ServerInfo::_setAuthServerInfo( char *buf1, char *buf2 )
{
	_setServerBaseInfo( &m_authServerInfo, buf1, buf2 );

	if( _stricmp( buf1, "frontserverlistenport" ) == 0 ) {

		m_authServerInfo.usFrontServerListenPort = atoi( buf2 );

	} else if( _stricmp( buf1, "gameserverlistenport" ) == 0 ) {

		m_authServerInfo.usGameServerListenPort = atoi( buf2 );

	}

}


void ServerInfo::_setAccountDBManagerInfo( char *buf1, char *buf2 )
{
	_setServerBaseInfo( &m_accountDBManagerInfo, buf1, buf2 );

	if( _stricmp( buf1, "authserverlistenport" ) == 0 ) {

		m_accountDBManagerInfo.usAuthServerListenPort = atoi( buf2 );

	}
}

void ServerInfo::_setAccountDBInfo( char *buf1, char *buf2 ) 
{
	_setServerBaseInfo( &m_accountDBInfo, buf1, buf2 );

	if( _stricmp( buf1, "database" ) == 0 ) {

		strcpy( m_accountDBInfo.szDatabaseName, buf2 );

	} else if( _stricmp( buf1, "userid" ) == 0 ) {

		strcpy( m_accountDBInfo.szUserID, buf2 );

	} else if( _stricmp( buf1, "userpw" ) == 0 ) {

		strcpy( m_accountDBInfo.szUserPW, buf2 );

	}
}

void ServerInfo::_setToolsLogInfo( char *buf1, char *buf2 ) 
{
	_setServerBaseInfo( &m_ToolsLogInfo, buf1, buf2 );

	if( _stricmp( buf1, "toolslogserverlistenport" ) == 0 ) 
	{
		m_ToolsLogInfo.usToolsLogServerListenPort = atoi( buf2 );
	} 
}


void ServerInfo::_setGameWorldInfo( char *buf1, char *buf2 )
{
	
	if( _stricmp( buf1, "worldid" ) == 0 ) {

		m_usWorldID = atoi( buf2 );

		if( m_usWorldID > 0 && m_usWorldID < 100 ) {
			m_gameWorldInfo[ m_usWorldID - 1 ].usWorldID = m_usWorldID;
			m_usWorldCount++;
		}

		return;
	}

	if( m_usWorldID < 1 || m_usWorldID > MAX_SERVER_COUNT - 1 ) return;

	if( _stricmp( buf1, "worldname" ) == 0 ) {

		strncpy( m_gameWorldInfo[ m_usWorldID - 1 ].szWorldName, buf2, 20 );

	} else if( _stricmp( buf1, "worldopendate" ) == 0 ) {

		strncpy( m_gameWorldInfo[ m_usWorldID - 1 ].szOpenDate, buf2, 15 );
	}

}


void ServerInfo::_setGameServerInfo( char *buf1, char *buf2 )
{	
	
	if( m_usWorldID < 1 || m_usWorldID > MAX_SERVER_COUNT - 1 ) return;

	_setServerBaseInfo( &m_gameWorldInfo[ m_usWorldID - 1 ].gameServerInfo, buf1, buf2 );

	if( _stricmp( buf1, "clientlistenport" ) == 0 ) {

		m_gameWorldInfo[ m_usWorldID - 1 ].gameServerInfo.usClientListenPort = atoi( buf2 );

	}

	if( _stricmp( buf1, "serverunique" ) == 0 ) {

		m_gameWorldInfo[ m_usWorldID - 1 ].gameServerInfo.usServerUnique = atoi( buf2 );

	}

	m_gameWorldInfo[ m_usWorldID - 1 ].gameServerInfo.usWorldID = m_usWorldID;

}


void ServerInfo::_setGameDBManagerInfo( char *buf1, char *buf2 )
{

	if( m_usWorldID < 1 || m_usWorldID > MAX_SERVER_COUNT - 1 ) return;

	_setServerBaseInfo( &m_gameWorldInfo[ m_usWorldID - 1 ].gameDBManagerInfo, buf1, buf2 );

	if( _stricmp( buf1, "gameserverlistenport" ) == 0 ) {

		m_gameWorldInfo[ m_usWorldID - 1 ].gameDBManagerInfo.usGameServerListenPort = atoi( buf2 );

	}

	m_gameWorldInfo[ m_usWorldID - 1 ].gameDBManagerInfo.usWorldID = m_usWorldID;

}


void ServerInfo::_setGameDBInfo( char *buf1, char *buf2 )
{

	if( m_usWorldID < 1 || m_usWorldID > MAX_SERVER_COUNT - 1 ) return;

	_setServerBaseInfo( &m_gameWorldInfo[ m_usWorldID - 1 ].gameDBInfo, buf1, buf2 );

	if( _stricmp( buf1, "database" ) == 0 ) {

		strcpy( m_gameWorldInfo[ m_usWorldID - 1 ].gameDBInfo.szDatabaseName, buf2 );

	} else if( _stricmp( buf1, "userid" ) == 0 ) {

		strcpy( m_gameWorldInfo[ m_usWorldID - 1 ].gameDBInfo.szUserID, buf2 );

	} else if( _stricmp( buf1, "userpw" ) == 0 ) {

		strcpy( m_gameWorldInfo[ m_usWorldID - 1 ].gameDBInfo.szUserPW, buf2 );

	}

	m_gameWorldInfo[ m_usWorldID - 1 ].gameDBInfo.usWorldID = m_usWorldID;

}

void ServerInfo::_setLogDBInfo( char *buf1, char *buf2 )
{

	if( m_usWorldID < 1 || m_usWorldID > MAX_SERVER_COUNT - 1 ) return;

	_setServerBaseInfo( &m_gameWorldInfo[ m_usWorldID - 1 ].logDBInfo, buf1, buf2 );

	if( _stricmp( buf1, "database" ) == 0 ) {

		strcpy( m_gameWorldInfo[ m_usWorldID - 1 ].logDBInfo.szDatabaseName, buf2 );

	} else if( _stricmp( buf1, "userid" ) == 0 ) {

		strcpy( m_gameWorldInfo[ m_usWorldID - 1 ].logDBInfo.szUserID, buf2 );

	} else if( _stricmp( buf1, "userpw" ) == 0 ) {

		strcpy( m_gameWorldInfo[ m_usWorldID - 1 ].logDBInfo.szUserPW, buf2 );

	} else if( _stricmp( buf1, "logprocedure" ) == 0 ) {

		strcpy( m_gameWorldInfo[ m_usWorldID - 1 ].logDBInfo.szLogProcedure, buf2 );

	}

	m_gameWorldInfo[ m_usWorldID - 1 ].logDBInfo.usWorldID = m_usWorldID;

}

void ServerInfo::_setMasterServerInfo( TCHAR *buf1, TCHAR *buf2 )
{
	_setServerBaseInfo( &m_masterServerInfo, buf1, buf2 );

	if( _stricmp( buf1, "clientlistenport" ) == 0 ) {

		m_masterServerInfo.usClientListenPort = atoi( buf2 );

	} else if( _stricmp( buf1, "servicemode" ) == 0 ) {

		strcpy( m_masterServerInfo.szServiceMode, buf2 );

	} else if( _stricmp( buf1, "servicearea" ) == 0 ) {

	strcpy( m_masterServerInfo.szServiceArea, buf2 );

	}
}

void ServerInfo::_setBillingServerInfo( TCHAR *buf1, TCHAR *buf2 )
{
	_setServerBaseInfo( &m_billingServerInfo, buf1, buf2 );

	if( _stricmp( buf1, "BillingTimeOut" ) == 0 ) {

		m_billingServerInfo.usBillingTimeOut = atoi( buf2 );

	} else if( _stricmp( buf1, "BillingMode" ) == 0 ) {

		strcpy( m_billingServerInfo.szBillingMode, buf2 );

	} else if( _stricmp( buf1, "BillingServerPort" ) == 0 ) {

		m_billingServerInfo.usBillingServerPort = atoi( buf2 );

	}
}

void ServerInfo::_setServerInfo()
{

	strcpy( m_frontServerInfo.szMessage, "FrontServer" );
	memcpy( &m_serverinfo[ SERVERINFO_FRONTSERVER_INDEX ], &m_frontServerInfo, sizeof( sServerBaseInfo ) );
	m_serverinfo[ SERVERINFO_FRONTSERVER_INDEX ].usServerType = SERVERTYPE_FRONTSERVER;
	
	strcpy( m_authServerInfo.szMessage, "AuthServer" );
	memcpy( &m_serverinfo[ SERVERINFO_AUTHSERVER_INDEX ], &m_authServerInfo, sizeof( sServerBaseInfo ) );
	m_serverinfo[ SERVERINFO_AUTHSERVER_INDEX ].usServerType = SERVERTYPE_AUTHSERVER;
	
	strcpy( m_accountDBManagerInfo.szMessage, "AccountDBManager" );
	memcpy( &m_serverinfo[ SERVERINFO_ACCOUNTDBMANAGER_INDEX ], &m_accountDBManagerInfo, sizeof( sServerBaseInfo ) );
	m_serverinfo[ SERVERINFO_ACCOUNTDBMANAGER_INDEX ].usServerType = SERVERTYPE_ACCOUNTDBMANAGER;

	strcpy( m_accountDBInfo.szMessage, "AccountDB" );
	memcpy( &m_serverinfo[ SERVERINFO_ACCOUNTDB_INDEX ], &m_accountDBInfo, sizeof( sServerBaseInfo ) );
	m_serverinfo[ SERVERINFO_ACCOUNTDB_INDEX ].usServerType = SERVERTYPE_ACCOUNTDB;

	char buf[ 20 ];
	
	for( int i = 0; i < m_usWorldCount; ++i ) {

		// game server
		memcpy( &m_serverinfo[ 4 + ( i * 3 ) ], &m_gameWorldInfo[ i ].gameServerInfo, sizeof( sServerBaseInfo ) );
		wsprintf( buf, "GameServer%d", i + 1 );
		strcpy( m_serverinfo[ 4 + ( i * 3 ) ].szMessage, buf );
		m_serverinfo[ 4 + ( i * 3 ) ].usServerType = SERVERTYPE_GAMESERVER;

		// game db manager
		memcpy( &m_serverinfo[ 5 + ( i * 3 ) ], &m_gameWorldInfo[ i ].gameDBManagerInfo, sizeof( sServerBaseInfo ) );
		wsprintf( buf, "GameDBManager%d", i + 1 );
		strcpy( m_serverinfo[ 5 + ( i * 3 ) ].szMessage, buf );
		m_serverinfo[ 5 + ( i * 3 ) ].usServerType = SERVERTYPE_GAMEDBMANAGER;

		// game db
		memcpy( &m_serverinfo[ 6 + ( i * 3 ) ], &m_gameWorldInfo[ i ].gameDBInfo, sizeof( sServerBaseInfo ) );
		wsprintf( buf, "GameDB%d", i + 1 );
		strcpy( m_serverinfo[ 6 + ( i * 3 ) ].szMessage, buf );
		m_serverinfo[ 6 + ( i * 3 ) ].usServerType = SERVERTYPE_GAMEDB;

		// log db
		//memcpy( &m_serverinfo[ 7 + ( i * 3 ) ], &m_gameWorldInfo[ i ].logDBInfo, sizeof( sServerBaseInfo ) );
		//wsprintf( buf, "GameDB%d", i + 1 );
		//strcpy( m_serverinfo[ 7 + ( i * 3 ) ].szMessage, buf );
		//m_serverinfo[ 7 + ( i * 3 ) ].usServerType = SERVERTYPE_LOGDB;
	}

	m_usServerCount = 4 + ( m_usWorldCount * 3 );
}


void ServerInfo::printInfo()
{

	printf( "\n[FrontServerInfo]\n" );

	printServerBaseInfo( &m_frontServerInfo );
	printf( "ClientListenPort: %d\n", m_frontServerInfo.usClientListenPort );
	printf( "GameServerListenPort: %d\n", m_frontServerInfo.usGameServerListenPort );
	printf( "ExecFile: %s\n", m_frontServerInfo.szFilePath );

	printf( "\n[AuthServerInfo]\n" );
	printServerBaseInfo( &m_authServerInfo );
	printf( "FrontServerListenPort: %d\n", m_authServerInfo.usFrontServerListenPort );
	printf( "GameServerListenPort: %d\n", m_authServerInfo.usGameServerListenPort );
	printf( "ExecFile: %s\n", m_authServerInfo.szFilePath );
	
	for( int i = 0; i < m_usWorldCount; ++i ) {
		printf( "\n[GameServerInfo]\n" );

		printServerBaseInfo( &m_gameWorldInfo[ i ].gameServerInfo );

		printf( "\n[GameDBManagerInfo]\n" );

		printServerBaseInfo( &m_gameWorldInfo[ i ].gameDBManagerInfo );

		printf( "\n[GameDBInfo]\n" );

		printServerBaseInfo( &m_gameWorldInfo[ i ].gameDBInfo );
	}
}

void ServerInfo::printServerBaseInfo( sServerBaseInfo *pStruct )
{
	printf( "ServerID : %d\n", pStruct->siServerID );
	printf( "PublicIP : %s\n", pStruct->szPublicIP );
	printf( "PrivateIP: %s\n", pStruct->szPrivateIP );
}

void ServerInfo::GetServerStatusString( int status, char *buf )
{
	switch( status )
	{
	case SERVERSTATUS_OK:
		strcpy( buf, "OK" );
		break;

	case SERVERSTATUS_NOHEARTBEAT:
		strcpy( buf, "No Heartbeat" );
		break;

	case SERVERSTATUS_LOGDBERROR:
		strcpy( buf, "LogDBError" );
		break;
	}
}
sGameWorldInfo*	ServerInfo::GetGameWorldInfo(UI32 uiWorldId)
{
	if (uiWorldId > m_usWorldCount)
		return NULL;

	return &m_gameWorldInfo[uiWorldId-1];
}

// 서버 이상 작동 로그 남기기
void	ServerInfo::printErrorServer()
{
	//const ConstTmpBufSize =  128;
	//
	////시간 구하기
	//SYSTEMTIME systime;
	//GetLocalTime(&systime);

	//TCHAR szDirPath[] = TEXT(".\\ServerErrorLog");

	//// 디렉토리 존재 여부
	//int Code = GetFileAttributes(szDirPath); 
	//if( !((Code != -1) && (Code & FILE_ATTRIBUTE_DIRECTORY )))
	//{
	//	CreateDirectory(TEXT("ServerErrorLog"), NULL);
	//}

	//// 임시 파일 이름
	//TCHAR	szFileName[ConstTmpBufSize];
	//ZeroMemory(szFileName, sizeof(TCHAR) * ConstTmpBufSize);

	//// 프론트 서버 접속 상태
	//if(m_frontRefresh.usStatus == SERVERSTATUS_NOHEARTBEAT)
	//{
	//	StringCchPrintf(szFileName,ConstTmpBufSize,TEXT("%s\\FrontServer-%d-%d-%d.txt"),
	//		szDirPath,systime.wYear,systime.wMonth,systime.wDay);
	//	g_CLogPrint.FilePrint(szFileName,"[%s]",  TEXT("FRONTSERVER_NOHEARTBEAT"));
	//}

	//ZeroMemory(szFileName, sizeof(TCHAR) * ConstTmpBufSize);
	//// 인증 서버 접속 상태
	//if(m_authRefresh.usStatus == SERVERSTATUS_NOHEARTBEAT)
	//{
	//	StringCchPrintf(szFileName,ConstTmpBufSize,TEXT("%s\\AuthServer-%d-%d-%d.txt"),
	//		szDirPath,systime.wYear,systime.wMonth,systime.wDay);
	//	g_CLogPrint.FilePrint(szFileName,"[%s]",  TEXT("AUTHSERVER_NOHEARTBEAT"));
	//}

	//ZeroMemory(szFileName, sizeof(TCHAR) * ConstTmpBufSize);
	//// 계정 DB 서버 접속 상태
	//if(m_accountDBManagerRefresh.usStatus == SERVERSTATUS_NOHEARTBEAT)
	//{
	//	StringCchPrintf(szFileName,ConstTmpBufSize,TEXT("%s\\AccountDBManager-%d-%d-%d.txt"),
	//		szDirPath,systime.wYear,systime.wMonth,systime.wDay);
	//	g_CLogPrint.FilePrint(szFileName,"[%s]",  TEXT("ACCOUNTDBMANAGER_NOHEARTBEAT"));
	//}
	//
	//for( int i = 0; i < m_usWorldCount; ++i ) {
	//	
	//	ZeroMemory(szFileName, sizeof(TCHAR) * ConstTmpBufSize);
	//	
	//	StringCchPrintf(szFileName,ConstTmpBufSize,TEXT("%s\\%s-%d-%d-%d.txt"),
	//		szDirPath,m_gameWorldInfo[ i ].szWorldName,
	//		systime.wYear,systime.wMonth,systime.wDay);

	//	// 게임 서버 접속 상태
	//	if(m_gameRefresh[ i ].usStatus == SERVERSTATUS_NOHEARTBEAT)
	//	{
	//		g_CLogPrint.FilePrint(szFileName,"[%s]",  TEXT("GAMESERVER_NOHEARTBEAT"));
	//	}
	//	
	//	// 게임 DB 서버 접속 상태
	//	if(m_gameDBManagerRefresh[ i ].usStatus == SERVERSTATUS_NOHEARTBEAT)
	//	{
	//		g_CLogPrint.FilePrint(szFileName,"[%s]",  TEXT("GAMEDBMANAGER_NOHEARTBEAT"));
	//	}

	//	// 게임 서버 FPS 상태
	//	if(m_gameRefresh[ i ].usFPS < GAMESERVER_FPS_DEADLINE)
	//	{
	//		g_CLogPrint.FilePrint(szFileName,"[%s]",  TEXT("GAMESERVER_FPS"));
	//	}

	//	// 게임 서버 DBLATENCY 상태
	//	if(m_gameRefresh[ i ].usDBLatency > GAMESERVER_DBLATENCY_DEADLINE)
	//	{
	//		g_CLogPrint.FilePrint(szFileName,"[%s]",  TEXT("GAMESERVER_DBLATENCY"));
	//	}

	//}
}
