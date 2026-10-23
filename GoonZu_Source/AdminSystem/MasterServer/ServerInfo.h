#ifndef _SERVERINFO_H
#define _SERVERINFO_H

#include "itzbase.h"

#define SERVERINFO_FRONTSERVER_INDEX			0
#define SERVERINFO_AUTHSERVER_INDEX				1
#define SERVERINFO_ACCOUNTDBMANAGER_INDEX		2
#define SERVERINFO_ACCOUNTDB_INDEX				3

#define SERVERTYPE_FRONTSERVER			1	
#define SERVERTYPE_AUTHSERVER			2
#define SERVERTYPE_ACCOUNTDBMANAGER		3
#define SERVERTYPE_ACCOUNTDB			4
#define	SERVERTYPE_GAMESERVER			5
#define SERVERTYPE_GAMEDBMANAGER		6
#define SERVERTYPE_GAMEDB				7
#define SERVERTYPE_SERVERAGENT			8
#define	SERVERTYPE_MASTERSERVER			9
#define SERVERTYPE_LOGDB				10

#define SERVERSTATUS_OK					0
#define SERVERSTATUS_NOHEARTBEAT		2
#define SERVERSTATUS_LOGDBERROR			10

#define MAX_SERVER_COUNT				150

#define GAMESERVER_FPS_DEADLINE			35
#define GAMESERVER_DBLATENCY_DEADLINE	10

//------------------------------------------------------------------------------------------------
// sServerBaseInfo
//------------------------------------------------------------------------------------------------
struct sServerBaseInfo
{
	sServerBaseInfo() { memset( this, 0, sizeof( sServerBaseInfo ) ); };

	SI32		siServerID;						// server ID
	UI16		usWorldID;						// 월드 ID(월드 구성 서버가 아니면 0)
	UI16		usServerType;					// server type
	TCHAR		szPublicIP[ 48 ];				// public IP
	TCHAR		szPrivateIP[ 48 ];				// private IP
	TCHAR		szFilePath[ 128 ];				// path and filename
	TCHAR		szMessage[ 40 ];				// message
};
//------------------------------------------------------------------------------------------------


//------------------------------------------------------------------------------------------------
// sFrontServerInfo
//------------------------------------------------------------------------------------------------
struct sFrontServerInfo : public sServerBaseInfo
{
	sFrontServerInfo()	{	memset( this, 0, sizeof(*this));		}

	UI16		usClientListenPort;				// 클라이언트 접속 대기 포트
	UI16		usGameServerListenPort;			// 게임서버 접속 대기 포트
};
//------------------------------------------------------------------------------------------------


//------------------------------------------------------------------------------------------------
// sAuthServerInfo
//------------------------------------------------------------------------------------------------
struct sAuthServerInfo : public sServerBaseInfo
{
	sAuthServerInfo()	{	memset( this, 0, sizeof(*this));		}

	UI16		usFrontServerListenPort;		// 프론트서버 접속 대기 포트 
	UI16		usGameServerListenPort;			// 게임서버 접속 대기 포트
};
//------------------------------------------------------------------------------------------------


//------------------------------------------------------------------------------------------------
// sAccountDBManagerInfo
//------------------------------------------------------------------------------------------------
struct sAccountDBManagerInfo : public sServerBaseInfo
{
	sAccountDBManagerInfo()	{	memset( this, 0, sizeof(*this));		}

	UI16		usAuthServerListenPort;			// 게임서버 접속 대기 포트
};
//------------------------------------------------------------------------------------------------


//------------------------------------------------------------------------------------------------
// sAccountDBInfo
//------------------------------------------------------------------------------------------------
struct sAccountDBInfo : public sServerBaseInfo
{
	sAccountDBInfo()	{	memset( this, 0, sizeof(*this));		}

	TCHAR		szDatabaseName[ 48 ];			// database name
	TCHAR		szUserID[ 20 ];					// user id
	TCHAR		szUserPW[ 20 ];					// user pw
};
//------------------------------------------------------------------------------------------------


//------------------------------------------------------------------------------------------------
// sToolsLogInfo
//------------------------------------------------------------------------------------------------
struct sToolsLogInfo : public sServerBaseInfo
{
	sToolsLogInfo()	{	memset( this, 0, sizeof(*this));		}

	UI16		usToolsLogServerListenPort;
};
//------------------------------------------------------------------------------------------------


//------------------------------------------------------------------------------------------------
// sGameServerInfo
//------------------------------------------------------------------------------------------------
struct sGameServerInfo : public sServerBaseInfo
{
	sGameServerInfo()	{	memset( this, 0, sizeof(*this));		}

	UI16		usClientListenPort;				// 클라이언트 접속 대기 포트
	UI16		usServerUnique;					// 게임 서버 고유 번호
};
//------------------------------------------------------------------------------------------------


//------------------------------------------------------------------------------------------------
// sGameDBManagerInfo
//------------------------------------------------------------------------------------------------
struct sGameDBManagerInfo : public sServerBaseInfo
{
	sGameDBManagerInfo()	{	memset( this, 0, sizeof(*this));		}

	UI16		usGameServerListenPort;			// 게임서버 접속 대기 포트
};
//------------------------------------------------------------------------------------------------


//------------------------------------------------------------------------------------------------
// sGameDBInfo
//------------------------------------------------------------------------------------------------
struct sGameDBInfo : public sServerBaseInfo
{
	sGameDBInfo()	{	memset( this, 0, sizeof(*this));		}

	TCHAR		szDatabaseName[ 48 ];			// database name
	TCHAR		szUserID[ 20 ];					// user id
	TCHAR		szUserPW[ 20 ];					// user pw
};
//------------------------------------------------------------------------------------------------


//------------------------------------------------------------------------------------------------
// sLogDBInfo
//------------------------------------------------------------------------------------------------
struct sLogDBInfo : public sServerBaseInfo
{
	sLogDBInfo()	{	memset( this, 0, sizeof(*this));		}

	TCHAR		szDatabaseName[ 48 ];			// database name
	TCHAR		szUserID[ 20 ];					// user id
	TCHAR		szUserPW[ 20 ];					// user pw
	TCHAR		szLogProcedure[ 40 ];			// Log Procedure
};
//------------------------------------------------------------------------------------------------


//------------------------------------------------------------------------------------------------
// sMastserServerInfo
//------------------------------------------------------------------------------------------------
struct sMastserServerInfo : public sServerBaseInfo
{
	sMastserServerInfo()	{	memset( this, 0, sizeof(*this));		}

	UI16		usClientListenPort;				// 모니터 클라이언트 접속 대기 포트
	TCHAR		szServiceMode[8];				// 서비스 동작 모드 - TEST, REAL	
	TCHAR		szServiceArea[12];				// 서비스 국가
};
//------------------------------------------------------------------------------------------------


//------------------------------------------------------------------------------------------------
// sMastserServerInfo
//------------------------------------------------------------------------------------------------
struct sBillingServerInfo : public sServerBaseInfo
{
	sBillingServerInfo()	{	memset( this, 0, sizeof(*this));		}

	UI16		usBillingTimeOut;				// 타임아웃
	UI16		usBillingServerPort;			// 빌링서버 포트번호
	TCHAR		szBillingMode[40];				// 빌링 모드
};
//------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------------
// sFrontServerRefreshInfo
//------------------------------------------------------------------------------------------------
struct sFrontServerRefreshInfo 
{
	sFrontServerRefreshInfo() {
		ZeroMemory( this, sizeof( sFrontServerRefreshInfo ) );
		usStatus = SERVERSTATUS_NOHEARTBEAT;
	}

	UI16		usStatus;
	DWORD		dwProcessId;
	SI32		siUserNum;
	SI32		siAuthConnect;
	SI32		siGameServerConnect;
	SI32		siGameVersion;
};
//------------------------------------------------------------------------------------------------


//------------------------------------------------------------------------------------------------
// sAuthServerRefreshInfo
//------------------------------------------------------------------------------------------------
struct sAuthServerRefreshInfo
{
	sAuthServerRefreshInfo() {
		memset( this, 0, sizeof(*this));
		usStatus = SERVERSTATUS_NOHEARTBEAT;
	}

	UI16		usStatus;
};
//------------------------------------------------------------------------------------------------


//------------------------------------------------------------------------------------------------
// sAccountDBManagerRefreshInfo
//------------------------------------------------------------------------------------------------
struct sAccountDBManagerRefreshInfo
{
	sAccountDBManagerRefreshInfo() {
		memset( this, 0, sizeof(*this));
		usStatus = SERVERSTATUS_NOHEARTBEAT;
	}

	UI16		usStatus;
};
//------------------------------------------------------------------------------------------------


//------------------------------------------------------------------------------------------------
// sGameServerRefreshInfo
//------------------------------------------------------------------------------------------------
struct sGameServerRefreshInfo
{
	sGameServerRefreshInfo() {
		memset( this, 0, sizeof(*this));
		usStatus = SERVERSTATUS_NOHEARTBEAT;
	}

	UI16		usStatus;
	UI16		usCurrentUserNum;

	//KHY - 0708 - 일본 모니터링 클라이언트 수정.
	UI16		usCurrentHanGameJP;
	UI16		usCurrentGamaniaJP;
	UI16		usCurrentGamePotJP;
	UI16		usCurrentNDEJP;
	UI16		usCurrentNEXONJP;
	
	UI16		usMaxUserNum;
	UI16		usCurrentNPCNum;
	UI16		usDBLatency;
	UI16		usFPS;

	UI16		usFrontServerConnection;
	UI16		usAuthServerConnection;
	UI16		usDBManagerConnection;

	UI16		uiTradeServerSession;
	UI32		uiFrame;

	UI32		uiNewCharNum;
	UI32		uiDelCharNum;

	// [진성] USA 이관 작업.
	UI16		usCurrentGLOBAL;
	UI16		usCurrentNHN;
};
//------------------------------------------------------------------------------------------------


//------------------------------------------------------------------------------------------------
// sGameDBManagerRefreshInfo
//------------------------------------------------------------------------------------------------
struct sGameDBManagerRefreshInfo
{
	sGameDBManagerRefreshInfo() {
		memset( this, 0, sizeof(*this));
		usStatus = SERVERSTATUS_NOHEARTBEAT;
	}

	UI16		usStatus;

};
//------------------------------------------------------------------------------------------------


//------------------------------------------------------------------------------------------------
// sGameWorldInfo
//------------------------------------------------------------------------------------------------
struct sGameWorldInfo
{
	sGameWorldInfo()	{		memset( this, 0, sizeof(*this));		}

	UI16				usWorldID;				// 월드 ID
	TCHAR				szWorldName[ 20 ];		// 월드명
	TCHAR				szOpenDate[ 15 ];		// 서버 오픈 시간
	TCHAR				szShortMessage[ 64 ];	// 짧은 안내글

	sGameServerInfo		gameServerInfo;			// 게임서버 정보
	sGameDBManagerInfo	gameDBManagerInfo;		// 게임DB Manager 정보
	sGameDBInfo			gameDBInfo;				// 게임DB 정보
	sLogDBInfo			logDBInfo;				// 로그DB 정보
};
//------------------------------------------------------------------------------------------------


//------------------------------------------------------------------------------------------------
// ServerInfo
//------------------------------------------------------------------------------------------------
class ServerInfo
{
public:
	ServerInfo();
	~ServerInfo();

public:
	void					Wait() { EnterCriticalSection( &m_cs ); };
	void					Release() { LeaveCriticalSection( &m_cs);  };

	void					ReadFromFile();

	int						GetServerCount() { return m_usServerCount; };
	int						GetWorldCount() { return m_usWorldCount; };

	void					SetServerInfo() { _setServerInfo(); };
	
	void					printErrorServer();
	void					printInfo();
	void					printServerBaseInfo( sServerBaseInfo *pStruct );

	void					GetServerStatusString( int status, TCHAR *buf );

	sGameWorldInfo*			GetGameWorldInfo(UI32 uiWorldId);
private:
	void					_setServerBaseInfo( sServerBaseInfo *pStruct, TCHAR *buf1, TCHAR *buf2 );

	void					_setFrontServerInfo( TCHAR *buf1, TCHAR *buf2 );
	void					_setAuthServerInfo( TCHAR *buf1, TCHAR *buf2 );

	void					_setAccountDBManagerInfo( TCHAR *buf1, TCHAR *buf2 );
	void					_setAccountDBInfo( TCHAR *buf1, TCHAR *buf2 );

	void					_setToolsLogInfo( TCHAR *buf1, TCHAR *buf2 );

	void					_setGameWorldInfo( TCHAR *buf1, TCHAR *buf2 );
	
	void					_setGameServerInfo( TCHAR *buf1, TCHAR *buf2 );
	void					_setGameDBManagerInfo( TCHAR *buf1, TCHAR *buf2 );
	void					_setGameDBInfo( TCHAR *buf1, TCHAR *buf2 );
	void					_setLogDBInfo( TCHAR *buf1, TCHAR *buf2 );

	void					_setMasterServerInfo( TCHAR *buf1, TCHAR *buf2 );

	void					_setBillingServerInfo( TCHAR *buf1, TCHAR *buf2 );

	void					_setServerInfo();

public:
	sFrontServerInfo				m_frontServerInfo;
	sAuthServerInfo					m_authServerInfo;
	sAccountDBManagerInfo			m_accountDBManagerInfo;
	sAccountDBInfo					m_accountDBInfo;
	sToolsLogInfo					m_ToolsLogInfo;
	sGameWorldInfo					m_gameWorldInfo[ MAX_SERVER_COUNT ];

	sFrontServerRefreshInfo			m_frontRefresh;
	sAuthServerRefreshInfo			m_authRefresh;
	sAccountDBManagerRefreshInfo	m_accountDBManagerRefresh;
	sGameServerRefreshInfo			m_gameRefresh[ MAX_SERVER_COUNT ];
	sGameDBManagerRefreshInfo		m_gameDBManagerRefresh[ MAX_SERVER_COUNT ];

	sMastserServerInfo				m_masterServerInfo;

	sBillingServerInfo				m_billingServerInfo;

	DWORD							m_dwFrontServerTick;
	DWORD							m_dwAuthServerTick;
	DWORD							m_dwAccountDBManagerTick;
	DWORD							m_dwGameServerTick[ MAX_SERVER_COUNT ];
	DWORD							m_dwGameDBManagerTick[ MAX_SERVER_COUNT ];

	UI16							m_usWorldID;
	UI16							m_usWorldCount;

	UI16							m_usServerCount;

	sServerBaseInfo					m_serverinfo[ MAX_SERVER_COUNT ];

	SI32							m_siNewCharNum[ MAX_SERVER_COUNT ];
	SI32							m_siDelCharNum[ MAX_SERVER_COUNT ];

	CRITICAL_SECTION				m_cs;

};
//------------------------------------------------------------------------------------------------


//------------------------------------------------------------------------------------------------
// UserCountInfo
//------------------------------------------------------------------------------------------------
struct UserCountInfo
{
	UserCountInfo()		{	memset(this, 0, sizeof(*this));		}

	UI16							m_uiGamepotJP;
	UI16							m_uiHangemeJP;
	UI16							m_uiNexonJP;

};

struct GlobalUserCountInfo
{
	GlobalUserCountInfo()		{	memset(this, 0, sizeof(*this));		}

	UI16							m_uiGlobal;
	UI16							m_uiNHN;
};
//------------------------------------------------------------------------------------------------

#endif
