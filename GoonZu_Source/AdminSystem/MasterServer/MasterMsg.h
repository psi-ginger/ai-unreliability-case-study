#ifndef _MASTERMSG_H
#define _MASTERMSG_H

#include "itzbase.h"
#include "ServerInfo.h"

#define AGENT_LISTEN_PORT		6500

struct MASTER_WORLDINFO
{

	UI16	usWorldID;						// 월드 ID
	TCHAR	szWorldName[ 20 ];				// 월드명
	TCHAR	szOpenDate[ 15 ];				// 서버 오픈 시간
	TCHAR	szShortMessage[ 64 ];			// 짧은 안내글	

	SI32	siServerID;						// 서버 ID
	TCHAR	szGameServerIP[ 16 ];			// 서버 IP
	UI16	usGameServerPort;				// 서버 PORT

	TCHAR	szGameDBManagerIP[ 16 ];		// 게임 db manager ip
	UI16	usGameDBManagerPort;			// 게임 db manager port
};


enum {
	AGENTMSG_REPORT_FILEPATH = 0,
	AGENTMSG_REPORT_SERVERINFO,

	MASTERMSG_REPORT_AGENT_HEARTBEAT,									//
	MASTERMSG_REPORT_MASTER_HEARTBEAT,									// 

	MASTERMSG_REQUEST_GMCLOGIN = 100,
	MASTERMSG_RESPONSE_GMCLOGIN,

	MASTERMSG_REQUEST_SERVERINFO1,										
	MASTERMSG_RESPONSE_SERVERINFO1,

	MASTERMSG_REQUEST_SERVERINFO2,
	MASTERMSG_RESPONSE_SERVERINFO2,										

	MASTERMSG_REPORT_SERVERREFRESHINFO = 110,						// 서버 갱신 정보 

	MASTERMSG_COMMAND_SERVER_ON,									// 서버 시작 명령

	MASTERMSG_COMMAND_GAMEDBMANAGER_ON,								// GameDBManager 시작 명령

	MASTERMSG_COMMAND_ALL_SERVER_ON,								// 전체 GameServer 시작 명령
	MASTERMSG_COMMAND_ALL_SERVER_OFF,								// 전체 GameServer중지 명령 

	MASTERMSG_COMMAND_ALL_GAMEDBMANAGER_ON,							// 전체 GameDBManager 시작 명령
	MASTERMSG_COMMAND_ALL_GAMEDBMANAGER_OFF,						// 전체 GameDBManager 중지 명령 

	MASTERMSG_COMMAND_MAKEFILE,
	MASTERMSG_COMMAND_MAKEDIRECTORY,
	MASTERMSG_COMMAND_SEND_FILEDATA,

	MASTERMSG_REQUEST_GMCDAYUSERNUM = 200,
	MASTERMSG_RESPONSE_GMCDAYUSERNUM,

	// 10000번 이후는 마스터 서버로 보내는 메시지 - 서버에이전트에서 처리하지않고 서버로 보내는 메세지들이다.
	MASTERMSG_REPORT_FRONTSERVER_HEARTBEAT = 10000,
	MASTERMSG_REPORT_AUTHSERVER_HEARTBEAT,
	MASTERMSG_REPORT_ACCOUNTDBMANAGER_HEARTBEAT,
	MASTERMSG_REPORT_GAMESERVER_HEARTBEAT,
	MASTERMSG_REPORT_GAMEDBMANAGER_HEARTBEAT,
	MASTERMSG_REPORT_GAMEDB_HEARTBEAT,

	MASTERMSG_REQUEST_FRONTSERVERINITINFO,								// 프론트 서버 초기화 정보 요청 
	MASTERMSG_REQUEST_AUTHSERVERINITINFO,								// 인증 서버 초기화 정보 요청
	MASTERMSG_REQUEST_ACCOUNTDBMANAGERINITINFO,							// Account DB Manager 초기화 정보 요청 
	MASTERMSG_REQUEST_GAMESERVERINITINFO,								// 게임 서버 초기화 정보 요청 
	MASTERMSG_REQUEST_GAMEDBMANAGERINITINFO,							// 게임 DB Manager 초기화 정보 요청

	MASTERMSG_RESPONSE_FRONTSERVERINITINFO,
	MASTERMSG_RESPONSE_AUTHSERVERINITINFO,
	MASTERMSG_RESPONSE_ACCOUNTDBMANAGERINITINFO,
	MASTERMSG_RESPONSE_GAMESERVERINITINFO,
	MASTERMSG_RESPONSE_GAMEDBMANAGERINITINFO,

	MASTERMSG_REQUEST_VERSIONINFO,
	MASTERMSG_RESPONSE_VERSIONINFO,

	MASTERMSG_REQUEST_NEWCHARNUM,
	MASTERMSG_RESPONSE_NEWCHARNUM,

	MASTERMSG_RESPONSE_GMORDER,

	GAMESERVER_SEND_TOOLSLOG_MASTER,		// 운영자 명령어 사용시 게임서버에서 마스터로 보낸다.

	MASTERMSG_REQUEST_FRONTSERVERINITINFO_STRING,							// 프론트 서버 초기화 정보 요청 < 문자열 버젼 >
	MASTERMSG_REQUEST_AUTHSERVERINITINFO_STRING,							// 인증 서버 초기화 정보 요청 < 문자열 버젼 >
	MASTERMSG_REQUEST_ACCOUNTDBMANAGERINITINFO_STRING,						// Account DB Manager 초기화 정보 요청 < 문자열 버젼 >
	MASTERMSG_REQUEST_GAMESERVERINITINFO_STRING,							// 게임 서버 초기화 정보 요청 < 문자열 버젼 >
	MASTERMSG_REQUEST_GAMEDBMANAGERINITINFO_STRING,							// 게임 DB Manager 초기화 정보 요청 < 문자열 버젼 >

	MASTERMSG_RESPONSE_FRONTSERVERINITINFO_STRING,							// < 문자열 버젼 >
	MASTERMSG_RESPONSE_AUTHSERVERINITINFO_STRING,							// < 문자열 버젼 >
	MASTERMSG_RESPONSE_ACCOUNTDBMANAGERINITINFO_STRING,						// < 문자열 버젼 >
	MASTERMSG_RESPONSE_GAMESERVERINITINFO_STRING,							// < 문자열 버젼 >
	MASTERMSG_RESPONSE_GAMEDBMANAGERINITINFO_STRING,						// < 문자열 버젼 >

	GOONZUMC_REQUEST_SERVERCOMMAND = 12000,									
	GOONZUMC_RESPONSE_SERVERCOMMAND,

	GOONZUMC_REQUEST_MAKEFILE,
	GOONZUMC_REQUEST_MAKEDIRECTORY,
	GOONZUMC_REQUEST_SEND_FILEDATA,

	GOONZUMC_REQUEST_SERVER_ON,
	GOONZUMC_REQUEST_SERVER_OFF,

	GOONZUMC_REQUEST_GAMEDBMANAGER_ON,
	GOONZUMC_REQUEST_GAMEDBMANAGER_OFF,

	GOONZUMC_REQUEST_ALL_SERVER_ON,
	GOONZUMC_REQUEST_ALL_SERVER_OFF,

	GOONZUMC_REQUEST_ALL_GAMEDBMANAGER_ON,
	GOONZUMC_REQUEST_ALL_GAMEDBMANAGER_OFF,

	GOONZUMC_REQUEST_FRONTSERVERON,
	GOONZUMC_REQUEST_FRONTSERVEROFF,

	GOONZUMC_REQUEST_GMORDER,
	
	MASTERMSG_COMMAND_SERVER_OFF,									// 서버 중지 명령 
	MASTERMSG_COMMAND_GAMEDBMANAGER_OFF,							// GameDBManager 중지 명령 

	MASTERMSG_REQUEST_GAMEDBMGR_ERRORQUERY_STRING,		// GameDBManager에서 쿼리실행 오류 발생시 보고하는 패킷
	GOONZUMC_RESPONSE_GAMEDBMGR_ERRORQUERY_STRING,		// GoonzuMC로 DBManager오류를 보고 하는 패킷

	GOONZUGM_REQUEST_ACCOUNTCHARINFO = 13000,		// GMTOOL 명령
	GOONZUGM_RESPONSE_ACCOUNTCHARINFO,

	GOONZUGM_REQUEST_USERCHARINFO ,				// GMTOOL 명령
	GOONZUGM_RESPONSE_USERCHARINFO,

	MASTERMSG_REQUEST_GMORDER,				   // 게임 서버로 GM 명령어 실행 요청
	GOONZUMC_RESPONSE_GMORDER,				   // 툴로 GM 명령어 결과 응답	
	ENDMSG
};

const int MAX_WORLDINFO_NUMBER = 40;

#pragma pack(1)

struct sReportToAgent_Heartbeat
{
	sPacketHeader	packetHeader;
};

struct sReportToMaster_Heartbeat
{
	sPacketHeader	packetHeader;
};

// 에이전트로 서버 정보를 보낸다
struct sReportToAgent_ServerInfo
{
	sPacketHeader	packetHeader;

	SI32			siServerID;
	TCHAR			szFilePath[ 128 ];
	TCHAR			szMessage[ 20 ];
};


// 에이전트로 파일 경로 및 파일명을 보낸다
struct sReportToAgent_FilePath
{
	sPacketHeader	packetHeader;

	TCHAR			szFilePath[ 128 ];
	TCHAR			szMessage[ 20 ];
};


// 프론트 서버 초기화 정보
struct sRequest_FrontServerInitInfo
{
	sRequest_FrontServerInitInfo() {
		packetHeader.usCmd = MASTERMSG_REQUEST_FRONTSERVERINITINFO;
		packetHeader.usSize = sizeof( sRequest_FrontServerInitInfo );
	}	


	sPacketHeader		packetHeader;
};


#define MAX_MASTER_WORDLDINFO		100

struct sResponse_FrontServerInitInfo
{

	sResponse_FrontServerInitInfo() {
		packetHeader.usCmd = MASTERMSG_RESPONSE_FRONTSERVERINITINFO;
		packetHeader.usSize = sizeof( sResponse_FrontServerInitInfo );
	}

	sPacketHeader		packetHeader;

	SI32				siServerID;

	TCHAR				szAuthServerIP[ 16 ];				// 인증서버 IP
	UI16				usAuthServerPort;					// 인증서버 PORT

	UI16				usClientListenPort;				// 클라이언트 접속 대기 포트
	UI16				usGameServerListenPort;			// 게임서버 접속 대기 포트

	UI16				count;
	MASTER_WORLDINFO	worldinfo[ MAX_MASTER_WORDLDINFO ];
};

// 프론트 서버 초기화 정보
struct sRequest_FrontServerInitInfo_String
{
	sRequest_FrontServerInitInfo_String() {
		packetHeader.usCmd = MASTERMSG_REQUEST_FRONTSERVERINITINFO_STRING;
		packetHeader.usSize = sizeof( sRequest_FrontServerInitInfo_String );
	}	


	sPacketHeader		packetHeader;
};



struct sResponse_FrontServerInitInfo_String
{

	sResponse_FrontServerInitInfo_String() {
		packetHeader.usCmd = MASTERMSG_RESPONSE_FRONTSERVERINITINFO_STRING;
		packetHeader.usSize = sizeof( sResponse_FrontServerInitInfo_String );
	}

	sPacketHeader		packetHeader;

	SI32				siServerID;

	TCHAR				szMapString[400];

	UI16				count;
	TCHAR				szWorldMapStr[50][400];
};


// 버전 정보 요청
struct sRequest_VersionInfo
{
	sRequest_VersionInfo() {
		packetHeader.usCmd = MASTERMSG_REQUEST_VERSIONINFO;
		packetHeader.usSize = sizeof( sRequest_VersionInfo );
	}	


	sPacketHeader		packetHeader;
};


// 버전 정보 전송 
struct sResponse_VersionInfo
{ 
	sResponse_VersionInfo() {
		packetHeader.usCmd = MASTERMSG_RESPONSE_VERSIONINFO;
		packetHeader.usSize = sizeof( sResponse_VersionInfo );
	}

	sPacketHeader		packetHeader;

	UI32				count;
	UI32				versioninfo[ 2048 ];
};

// 인증 서버 초기화 정보
struct sRequest_AuthServerInitInfo
{

	sRequest_AuthServerInitInfo() {
		packetHeader.usCmd = MASTERMSG_REQUEST_AUTHSERVERINITINFO;
		packetHeader.usSize = sizeof( sRequest_AuthServerInitInfo );
	};

	sPacketHeader		packetHeader;
};

struct sResponse_AuthServerInitInfo
{
	sResponse_AuthServerInitInfo() {
		packetHeader.usCmd = MASTERMSG_RESPONSE_AUTHSERVERINITINFO;
		packetHeader.usSize = sizeof( sResponse_AuthServerInitInfo );
	};

	sPacketHeader		packetHeader;

	SI32				siServerID;

	TCHAR				szAccountDBManagerIP[ 16 ];		// account db manager ip
	UI16				usAccountDBManagerPort;			// account db manager port

	UI16				usFrontServerListenPort;		// Front Server Listen Port
	UI16				usGameServerListenPort;			// Game Server Listen Port

	TCHAR				szAccountDBIP[ 20 ];			// account db ip
};

// 인증 서버 초기화 정보 < 문자열 버젼 >
struct sRequest_AuthServerInitInfo_String
{
	sRequest_AuthServerInitInfo_String()
	{
		packetHeader.usCmd = MASTERMSG_REQUEST_AUTHSERVERINITINFO_STRING;
		packetHeader.usSize = sizeof( sRequest_AuthServerInitInfo_String );
	};

	sPacketHeader		packetHeader;
};

struct sResponse_AuthServerInitInfo_String
{
	sResponse_AuthServerInitInfo_String()
	{
		packetHeader.usCmd = MASTERMSG_RESPONSE_AUTHSERVERINITINFO_STRING;
		packetHeader.usSize = sizeof( sResponse_AuthServerInitInfo_String );

		siServerID		= 0;
	};

	sPacketHeader		packetHeader;
	SI32				siServerID;

	TCHAR				szMapString[512];
};

// AccountDBManager 초기화 정보
struct sRequest_AccountDBManagerInitInfo
{
	sRequest_AccountDBManagerInitInfo() {
		packetHeader.usCmd = MASTERMSG_REQUEST_ACCOUNTDBMANAGERINITINFO;
		packetHeader.usSize = sizeof( sRequest_AccountDBManagerInitInfo );
	};

	sPacketHeader		packetHeader;

};

struct sResponse_AccountDBManagerInitInfo
{
	sResponse_AccountDBManagerInitInfo() {
		packetHeader.usCmd = MASTERMSG_RESPONSE_ACCOUNTDBMANAGERINITINFO;
		packetHeader.usSize = sizeof( sResponse_AccountDBManagerInitInfo );
	};


	sPacketHeader		packetHeader;

	SI32				siServerID;

	TCHAR				szAccountDBIP[ 16 ];
	TCHAR				szDatabaseName[ 20 ];			// database name
	TCHAR				szUserID[ 20 ];					// user id
	TCHAR				szUserPW[ 20 ];					// user pw

};

// AccountDBManager 초기화 정보
struct sRequest_AccountDBManagerInitInfo_String
{
	sRequest_AccountDBManagerInitInfo_String() {
		packetHeader.usCmd = MASTERMSG_REQUEST_ACCOUNTDBMANAGERINITINFO_STRING;
		packetHeader.usSize = sizeof( sRequest_AccountDBManagerInitInfo_String );
	};

	sPacketHeader		packetHeader;

};

struct sResponse_AccountDBManagerInitInfo_String
{
	sResponse_AccountDBManagerInitInfo_String() {
		packetHeader.usCmd = MASTERMSG_RESPONSE_ACCOUNTDBMANAGERINITINFO_STRING;
		packetHeader.usSize = sizeof( sResponse_AccountDBManagerInitInfo_String );
	};


	sPacketHeader		packetHeader;

	SI32				siServerID;

	TCHAR				szMapString[512];
};

// 게임서버 초기화 정보
struct sRequest_GameServerInitInfo
{

	sRequest_GameServerInitInfo() {
		packetHeader.usCmd = MASTERMSG_REQUEST_GAMESERVERINITINFO;
		packetHeader.usSize = sizeof( sRequest_GameServerInitInfo );
	};


	sPacketHeader		packetHeader;

};

struct sResponse_GameServerInitInfo
{
	sResponse_GameServerInitInfo() {
		packetHeader.usCmd = MASTERMSG_RESPONSE_GAMESERVERINITINFO;
		packetHeader.usSize = sizeof( sResponse_GameServerInitInfo );
	};

	sPacketHeader		packetHeader;

	SI32				siServerID;

	TCHAR				szFrontServerIP[ 16 ];
	UI16				usFrontServerPort;

	TCHAR				szAuthServerIP[ 16 ];
	UI16				usAuthServerPort;

	TCHAR				szGameDBManagerIP[ 16 ];
	UI16				usGameDBManagerPort;

	UI16				usClientListenPort;
	UI16				usServerUnique;

	SI32				siServerType;
};

// 게임서버 초기화 정보
struct sRequest_GameServerInitInfo_String
{

	sRequest_GameServerInitInfo_String() {
		packetHeader.usCmd = MASTERMSG_REQUEST_GAMESERVERINITINFO_STRING;
		packetHeader.usSize = sizeof( sRequest_GameServerInitInfo_String );
	};

	sPacketHeader		packetHeader;

};

struct sResponse_GameServerInitInfo_String
{
	sResponse_GameServerInitInfo_String() {
		packetHeader.usCmd = MASTERMSG_RESPONSE_GAMESERVERINITINFO_STRING;
		packetHeader.usSize = sizeof( sResponse_GameServerInitInfo_String );
	};

	sPacketHeader		packetHeader;

	SI32				siServerID;

	TCHAR				szMapString[1536];
};

// GameDBManager_World 초기화 정보
struct sRequest_GameDBManagerInitInfo
{
	sRequest_GameDBManagerInitInfo() {
		packetHeader.usCmd = MASTERMSG_REQUEST_GAMEDBMANAGERINITINFO;
		packetHeader.usSize = sizeof( sRequest_GameDBManagerInitInfo );
	};

	sPacketHeader		packetHeader;
};

struct sResponse_GameDBManagerInitInfo
{
	sResponse_GameDBManagerInitInfo() {
		packetHeader.usCmd = MASTERMSG_RESPONSE_GAMEDBMANAGERINITINFO;
		packetHeader.usSize = sizeof( sResponse_GameDBManagerInitInfo );
	};

	sPacketHeader		packetHeader;

	SI32				siServerID;

	UI16				usGameServerListenPort;			// game server listen port

	TCHAR				szGameDBIP[ 16 ];
	TCHAR				szDatabaseName[ 20 ];			// database name
	TCHAR				szUserID[ 20 ];					// user id
	TCHAR				szUserPW[ 20 ];					// user pw

	TCHAR				szLogDBIP[ 16 ];
	TCHAR				szLogDatabaseName[ 20 ];		// log database name
	TCHAR				szLogUserID[ 20 ];				// log user id
	TCHAR				szLogUserPW[ 20 ];				// log user pw
	TCHAR				szLogProcedure[ 40 ];			// log user pw
};

// GameDBManager_World 초기화 정보
struct sRequest_GameDBManagerInitInfo_String
{
	sRequest_GameDBManagerInitInfo_String() {
		packetHeader.usCmd = MASTERMSG_REQUEST_GAMEDBMANAGERINITINFO_STRING;
		packetHeader.usSize = sizeof( sRequest_GameDBManagerInitInfo_String );
	};

	sPacketHeader		packetHeader;
};

struct sResponse_GameDBManagerInitInfo_String
{
	sResponse_GameDBManagerInitInfo_String() {
		packetHeader.usCmd = MASTERMSG_RESPONSE_GAMEDBMANAGERINITINFO_STRING;
		packetHeader.usSize = sizeof( sResponse_GameDBManagerInitInfo_String );
	};

	sPacketHeader		packetHeader;

	SI32				siServerID;

	TCHAR				szMapString[1536];
};


// 허트비트
struct sHeartbeat
{
	sPacketHeader				packetHeader;
	DWORD						dwProcessId;
};



// 프론트서버 허트비트 
struct sReport_FrontServerHeartbeat : public sHeartbeat
{
	sFrontServerRefreshInfo			refreshInfo;
};


// 인증서버 허트비트
struct sReport_AuthServerHeartbeat : public sHeartbeat
{
	sAuthServerRefreshInfo			refreshInfo;
};

// AccountDBManager 허트비트 
struct sReport_AccountDBManagerHeartbeat : public sHeartbeat
{
	sAccountDBManagerRefreshInfo	refreshInfo;
};

// 게임서버 허트비트
struct sReport_GameServerHeartbeat : public sHeartbeat
{
	sGameServerRefreshInfo			refreshInfo;
};

// GameDBManager_World 허트비트 
struct sReport_GameDBManagerHeartbeat : public sHeartbeat
{
	sGameDBManagerRefreshInfo		refreshInfo;
};


// 모니터 클라이언트 로그인 
struct sRequest_GMCLogin
{
	sRequest_GMCLogin() {
		packetHeader.usCmd = MASTERMSG_REQUEST_GMCLOGIN;
		packetHeader.usSize = sizeof( sRequest_GMCLogin );
	};


	sPacketHeader		packetHeader;

	TCHAR				szID[ 20 ];
	TCHAR				szPW[ 20 ];
};


struct sResponse_GMCLogin
{
	sResponse_GMCLogin() {
		packetHeader.usCmd = MASTERMSG_RESPONSE_GMCLOGIN;
		packetHeader.usSize = sizeof( sResponse_GMCLogin );
	};

	sPacketHeader		packetHeader;

	UI16				usResult;									// 0:실패
};

// 프론트서버, 인증서버, AccountDBManager 정보 요청
struct sRequest_ServerInfo1
{

	sRequest_ServerInfo1() {
		packetHeader.usCmd = MASTERMSG_REQUEST_SERVERINFO1;
		packetHeader.usSize = sizeof( sRequest_ServerInfo1 );
	};

	sPacketHeader			packetHeader;
};

struct sResponse_ServerInfo1
{
	sResponse_ServerInfo1() {
		packetHeader.usCmd = MASTERMSG_RESPONSE_SERVERINFO1;
		packetHeader.usSize = sizeof( sResponse_ServerInfo1 );
	};

	sPacketHeader			packetHeader;

	sFrontServerInfo		frontServerInfo;
	sAuthServerInfo			authServerInfo;
	sAccountDBManagerInfo	accountDBManagerInfo;	
	sAccountDBInfo			accountDBInfo;

};

// 게임서버, GameDBManager_World 정보 요청 
struct sRequest_ServerInfo2
{

	sRequest_ServerInfo2() {
		packetHeader.usCmd = MASTERMSG_REQUEST_SERVERINFO2;
		packetHeader.usSize = sizeof( sRequest_ServerInfo2 );
	};

	sPacketHeader		packetHeader;

};


struct sResponse_ServerInfo2
{
	sResponse_ServerInfo2() {
		packetHeader.usCmd = MASTERMSG_RESPONSE_SERVERINFO2;
		packetHeader.usSize = sizeof( sResponse_ServerInfo2 );
	};

	sPacketHeader		packetHeader;

	UI16				usCount;
	sGameWorldInfo		gameWorldInfo[ MAX_WORLDINFO_NUMBER ];

	void SetSize() {
		packetHeader.usSize = sizeof( sPacketHeader ) + 4 + usCount * sizeof( sGameWorldInfo ) + 16;
	}

};


// MasterServer -> Monitor Client 
struct sReport_RefreshServerInfo
{

	sReport_RefreshServerInfo() {
		packetHeader.usCmd = MASTERMSG_REPORT_SERVERREFRESHINFO;
		packetHeader.usSize = sizeof( sReport_RefreshServerInfo );
	}

	sPacketHeader					packetHeader;

	SI32							siMaxUserNum;

	sFrontServerRefreshInfo			frontRefresh;
	sAuthServerRefreshInfo			authRefresh;
	sAccountDBManagerRefreshInfo	accountDBManagerRefresh;

	UI16							usCount;
	sGameServerRefreshInfo			gameRefresh[ MAX_WORLDINFO_NUMBER ];
	sGameDBManagerRefreshInfo		gameDBManagerRefresh[ MAX_WORLDINFO_NUMBER ];

	void SetSize() { packetHeader.usSize = sizeof( sPacketHeader ) + 
		sizeof( SI32 ) +
		sizeof( sFrontServerRefreshInfo ) +
		sizeof( sAuthServerRefreshInfo ) +
		sizeof( sAccountDBManagerRefreshInfo ) +
		(sizeof( sGameServerRefreshInfo ) + 
		sizeof( sGameDBManagerRefreshInfo ) ) * usCount + 16;
	};
};

struct sCommand_ServerOn
{
	sPacketHeader					packetHeader;

	SI32							siServerID;
	DWORD							dwProcessId;

	sCommand_ServerOn()
	{
		packetHeader.usCmd = MASTERMSG_COMMAND_SERVER_ON;
		packetHeader.usSize = sizeof( sCommand_ServerOn );
		siServerID = -1;
		dwProcessId = 0;
	}
};

struct sCommand_ServerOff
{
	sPacketHeader					packetHeader;

	SI32							siServerID;
	DWORD							dwProcessId;

	sCommand_ServerOff()
	{
		packetHeader.usCmd = MASTERMSG_COMMAND_SERVER_OFF;
		packetHeader.usSize = sizeof( sCommand_ServerOff );
		siServerID = -1;
		dwProcessId = 0;
	}
};

//***************************************************************************************************
//
// Class Name 		: sCommand_GameDBManagerStart
//
// Last Modified 	: 2007 / 03 / 26
// Created		 	: 김광명
//
// Function			: [요청] GameDBManager을 시작한다.
//
//***************************************************************************************************
struct sCommand_GameDBManagerOn
{
	sPacketHeader					packetHeader;

	SI32							siServerID;
	DWORD							dwProcessId;

	sCommand_GameDBManagerOn()
	{
		packetHeader.usCmd = MASTERMSG_COMMAND_GAMEDBMANAGER_ON;
		packetHeader.usSize = sizeof( sCommand_GameDBManagerOn );
		siServerID = -1;
		dwProcessId = 0;
	}
};

//***************************************************************************************************
//
// Class Name 		: sCommand_GameDBManagerOff
//
// Last Modified 	: 2007 / 03 / 26
// Created		 	: 김광명
//
// Function			: [요청] GameDBManager을 끈다
//
//***************************************************************************************************
struct sCommand_GameDBManagerOff
{
	sPacketHeader					packetHeader;

	SI32							siServerID;
	DWORD							dwProcessId;

	sCommand_GameDBManagerOff()
	{
		packetHeader.usCmd = MASTERMSG_COMMAND_GAMEDBMANAGER_OFF;
		packetHeader.usSize = sizeof( sCommand_GameDBManagerOff );
		siServerID = -1;
		dwProcessId = 0;
	}
};

//***************************************************************************************************
//
// Class Name 		: sCommand_All_ServerOn
//
// Last Modified 	: 2007 / 03 / 26
// Created		 	: 김광명
//
// Function			: [요청] 전체 GameServer을 시작한다.
//
//***************************************************************************************************
struct sCommand_All_ServerOn
{
	sPacketHeader					packetHeader;

	SI32							siServerID;
	DWORD							dwProcessId;

	sCommand_All_ServerOn()
	{
		packetHeader.usCmd = MASTERMSG_COMMAND_ALL_SERVER_ON;
		packetHeader.usSize = sizeof( sCommand_All_ServerOn );
		siServerID = -1;
		dwProcessId = 0;
	}
};

//***************************************************************************************************
//
// Class Name 		: sCommand_All_ServerOff
//
// Last Modified 	: 2007 / 03 / 26
// Created		 	: 김광명
//
// Function			: [요청] 전체 GameServer을 끈다.
//
//***************************************************************************************************
struct sCommand_All_ServerOff
{
	sPacketHeader					packetHeader;

	SI32							siServerID;
	DWORD							dwProcessId;

	sCommand_All_ServerOff()
	{
		packetHeader.usCmd = MASTERMSG_COMMAND_ALL_SERVER_OFF;
		packetHeader.usSize = sizeof( sCommand_All_ServerOff );
		siServerID = -1;
		dwProcessId = 0;
	}
};

//***************************************************************************************************
//
// Class Name 		: sCommand_All_GameDBManagerOn
//
// Last Modified 	: 2007 / 03 / 26
// Created		 	: 김광명
//
// Function			: [요청] 전체 GameDBManager을 시작한다.
//
//***************************************************************************************************
struct sCommand_All_GameDBManagerOn
{
	sPacketHeader					packetHeader;

	SI32							siServerID;
	DWORD							dwProcessId;

	sCommand_All_GameDBManagerOn()
	{
		packetHeader.usCmd = MASTERMSG_COMMAND_ALL_GAMEDBMANAGER_ON;
		packetHeader.usSize = sizeof( sCommand_All_GameDBManagerOn );
		siServerID = -1;
		dwProcessId = 0;
	}
};

//***************************************************************************************************
//
// Class Name 		: sCommand_All_GameDBManagerOff
//
// Last Modified 	: 2007 / 03 / 26
// Created		 	: 김광명
//
// Function			: [요청] 전체 GameDBManager을 끈다
//
//***************************************************************************************************
struct sCommand_All_GameDBManagerOff
{
	sPacketHeader					packetHeader;

	SI32							siServerID;
	DWORD							dwProcessId;

	sCommand_All_GameDBManagerOff()
	{
		packetHeader.usCmd = MASTERMSG_COMMAND_ALL_GAMEDBMANAGER_OFF;
		packetHeader.usSize = sizeof( sCommand_All_GameDBManagerOff );
		siServerID = -1;
		dwProcessId = 0;
	}
};

struct sRequest_ServerCommand
{
	sPacketHeader					packetHeader;

	SI32							siWorldID;
	TCHAR							szCommand[ 128 ];

	sRequest_ServerCommand( SI32 worldid, TCHAR *szCmd ) {

		packetHeader.usCmd = GOONZUMC_REQUEST_SERVERCOMMAND;
		packetHeader.usSize = sizeof( sRequest_ServerCommand );

		siWorldID = worldid;
		StringCchCopy( szCommand, 128, szCmd );
	}

};

struct sRequest_GMCDAYUSERNUM 
{
	sPacketHeader					packetHeader;

	SI32							siYear;
	SI32							siMonth;
	SI32							siDay;

	SI32							siLayer;

	sRequest_GMCDAYUSERNUM( SI32 year,SI32 month,SI32 day, SI32 layer ){

		packetHeader.usCmd = MASTERMSG_REQUEST_GMCDAYUSERNUM;
		packetHeader.usSize = sizeof(sRequest_GMCDAYUSERNUM);

		siYear = year;
		siMonth = month;
		siDay = day;

		siLayer = layer;
	}
};

struct sResponse_GMCDAYUSERNUM 
{
	sPacketHeader					packetHeader;

	SI32							siResult;

	SI32							siYear;
	SI32							siMonth;
	SI32							siDay;

	SI32							siLayer;

	SI32							siUserNum[ 288 ];

	sResponse_GMCDAYUSERNUM()
	{
		packetHeader.usCmd = MASTERMSG_RESPONSE_GMCDAYUSERNUM;
		packetHeader.usSize = sizeof( sResponse_GMCDAYUSERNUM );
	}
};

//***************************************************************************************************
//
// Class Name 		: sRequest_GMCServerOn
//
// Last Modified 	: 2007 / 03 / 26 
// Created		 	: 김광명
//
// Function			: [요청] 선택한 게임서버를 시작한다.
//
//***************************************************************************************************
struct sRequest_GMCServerOn
{
	sPacketHeader				packetHeader;

	SI32						siServerID;

	sRequest_GMCServerOn( SI32 serverid ) 
	{
		packetHeader.usCmd = GOONZUMC_REQUEST_SERVER_ON;
		packetHeader.usSize = sizeof( sRequest_GMCServerOn );

		siServerID = serverid;
	}

};

//***************************************************************************************************
//
// Class Name 		: sRequest_GMCServerOff
//
// Last Modified 	: 2007 / 03 / 26 
// Created		 	: 김광명
//
// Function			: [요청] 선택한 게임서버를 종료한다.
//
//***************************************************************************************************
struct sRequest_GMCServerOff
{
	sPacketHeader				packetHeader;

	SI32						siServerID;

	sRequest_GMCServerOff( SI32 serverid ) 
	{
		packetHeader.usCmd = GOONZUMC_REQUEST_SERVER_OFF;
		packetHeader.usSize = sizeof( sRequest_GMCServerOff );

		siServerID = serverid;
	}
};

//***************************************************************************************************
//
// Class Name 		: sRequest_GMCGameDBManagerOn
//
// Last Modified 	: 2007 / 03 / 26 
// Created		 	: 김광명
//
// Function			: [요청] 선택한 게임DB를 시작한다.
//
//***************************************************************************************************
struct sRequest_GMCGameDBManagerOn
{
	sPacketHeader				packetHeader;

	SI32						siServerID;

	sRequest_GMCGameDBManagerOn( SI32 serverid )
	{
		packetHeader.usCmd = GOONZUMC_REQUEST_GAMEDBMANAGER_ON;
		packetHeader.usSize = sizeof( sRequest_GMCServerOn );

		siServerID = serverid;
	}

};

//***************************************************************************************************
//
// Class Name 		: sRequest_GMCGameDBManagerOff
//
// Last Modified 	: 2007 / 03 / 26 
// Created		 	: 김광명
//
// Function			: [요청] 선택한 게임DB를 종료한다.
//
//***************************************************************************************************
struct sRequest_GMCGameDBManagerOff
{
	sPacketHeader				packetHeader;

	SI32						siServerID;

	sRequest_GMCGameDBManagerOff( SI32 serverid ) {
		packetHeader.usCmd = GOONZUMC_REQUEST_GAMEDBMANAGER_OFF;
		packetHeader.usSize = sizeof( sRequest_GMCGameDBManagerOff );

		siServerID = serverid;
	}
};

//***************************************************************************************************
//
// Class Name 		: sRequest_GMC_AllServerOn
//
// Last Modified 	: 2007 / 03 / 26 
// Created		 	: 김광명
//
// Function			: [요청] 모든 게임서버를 시작한다.
//
//***************************************************************************************************
struct sRequest_GMC_AllServerOn
{
	sPacketHeader				packetHeader;

	sRequest_GMC_AllServerOn() 
	{
		packetHeader.usCmd = GOONZUMC_REQUEST_ALL_SERVER_ON;
		packetHeader.usSize = sizeof( sRequest_GMC_AllServerOn );
	}

};

//***************************************************************************************************
//
// Class Name 		: sRequest_GMC_AllServerOff
//
// Last Modified 	: 2007 / 03 / 26 
// Created		 	: 김광명
//
// Function			: [요청] 선택한 게임서버를 종료한다.
//
//***************************************************************************************************
struct sRequest_GMC_AllServerOff
{
	sPacketHeader				packetHeader;

	sRequest_GMC_AllServerOff() 
	{
		packetHeader.usCmd = GOONZUMC_REQUEST_ALL_SERVER_OFF;
		packetHeader.usSize = sizeof( sRequest_GMC_AllServerOff );
	}
};

//***************************************************************************************************
//
// Class Name 		: sRequest_GMC_AllGameDBManagerOn
//
// Last Modified 	: 2007 / 03 / 26 
// Created		 	: 김광명
//
// Function			: [요청] 전체 게임DB를 시작한다.
//
//***************************************************************************************************
struct sRequest_GMC_AllGameDBManagerOn
{
	sPacketHeader				packetHeader;

	sRequest_GMC_AllGameDBManagerOn()
	{
		packetHeader.usCmd = GOONZUMC_REQUEST_ALL_GAMEDBMANAGER_ON;
		packetHeader.usSize = sizeof( sRequest_GMC_AllGameDBManagerOn );
	}
};

//***************************************************************************************************
//
// Class Name 		: sRequest_GMCGameDBManagerOff
//
// Last Modified 	: 2007 / 03 / 26 
// Created		 	: 김광명
//
// Function			: [요청] 전체 게임DB를 종료한다.
//
//***************************************************************************************************
struct sRequest_GMC_AllGameDBManagerOff
{
	sPacketHeader				packetHeader;

	sRequest_GMC_AllGameDBManagerOff() 
	{
		packetHeader.usCmd = GOONZUMC_REQUEST_ALL_GAMEDBMANAGER_OFF;
		packetHeader.usSize = sizeof( sRequest_GMC_AllGameDBManagerOff );
	}
};

struct sRequest_GMCFrontServerOn
{
	sPacketHeader				packetHeader;

	sRequest_GMCFrontServerOn()
	{
		packetHeader.usCmd = GOONZUMC_REQUEST_FRONTSERVERON;
		packetHeader.usSize = sizeof( sRequest_GMCFrontServerOn );
	}
};

struct sRequest_GMCFrontServerOff
{
	sPacketHeader				packetHeader;

	sRequest_GMCFrontServerOff()
	{
		packetHeader.usCmd = GOONZUMC_REQUEST_FRONTSERVEROFF;
		packetHeader.usSize = sizeof( sRequest_GMCFrontServerOff );
	}
};


// 마스터 서버로 부터 전체 서버의 캐릭터 생성 및 삭제 숫자 얻어오기 
struct sResponse_NewCharNum
{
	sPacketHeader				packetHeader;

	SI32						siCount;

	SI32						siServerID[ 50 ];
	SI32						siNewCharNum[ 50 ];
	SI32						siDelCharNum[ 50 ];

};

//***************************************************************************************************
//
// Class Name 		: sRequest_GMOrder
//
// Last Modified 	: 2009 / 07 / 02 
// Created		 	: 박기형
//
// Function			: [요청] GM명령어를 특정 게임 서버로 요청한다.
//
//***************************************************************************************************

struct sRequest_GMOrder
{
	sPacketHeader				packetHeader;

	TCHAR						order[ 256 ];

	UI32						uiCharID;			// 요청자 아이디

	SI32						siWorldId;				// 게임 월드 아이디	

	sRequest_GMOrder( TCHAR*str, UI32 uiCharID_Para, SI32 siWorldId_Para)
	{
		packetHeader.usCmd = MASTERMSG_REQUEST_GMORDER;
		packetHeader.usSize = sizeof( sRequest_GMOrder );

		memcpy( order, str, 256 );
		order[ 255 ] = NULL;

		uiCharID = uiCharID_Para;

		siWorldId = siWorldId_Para;
	}
};
//***************************************************************************************************
//
// Class Name 		: sResponse_GMOrder
//
// Last Modified 	: 2009 / 07 / 02 
// Created		 	: 박기형
//
// Function			: [응답] 특정 게임 서버에서 GM명령어를 실행 후 그 결과를 보내준다.
//
//***************************************************************************************************

struct sResponse_GMOrder
{
	sPacketHeader				packetHeader;

	TCHAR						order[ 256 ];

	UI32						uiCharID;			// 요청자 아이디

	BOOL						isSuccess;			// 실행 성공 여부

	SI32						siWorldId;				// 게임 월드 아이디	

	sResponse_GMOrder( TCHAR*str, UI32 uiCharID_Para, BOOL isSuccess_Para , SI32 siWorldId_Para)
	{
		packetHeader.usCmd = MASTERMSG_RESPONSE_GMORDER;
		packetHeader.usSize = sizeof( sResponse_GMOrder );

		memcpy( order, str, 256 );
		order[ 255 ] = NULL;

		uiCharID = uiCharID_Para;

		isSuccess = isSuccess_Para;

		siWorldId = siWorldId_Para;
	}
};


//***************************************************************************************************
//
// Class Name 		: sRequest_GMC_GMOrder
//
// Last Modified 	: 2009 / 07 / 02 
// Created		 	: 박기형
//
// Function			: [요청] GM명령어 실행을 요청한다.
//
//***************************************************************************************************

struct sRequest_GMC_GMOrder
{
	sPacketHeader				packetHeader;

	TCHAR						order[ 256 ];

	SI32						siWorldId;		// 게임 월드 아이디	

	sRequest_GMC_GMOrder( TCHAR *str ,SI32 siWorldId_Para)
	{
		packetHeader.usCmd = GOONZUMC_REQUEST_GMORDER;
		packetHeader.usSize = sizeof( sRequest_GMC_GMOrder );

		memcpy( order, str, 256 );
		order[ 255 ] = NULL;

		siWorldId = siWorldId_Para;
	}
};

//***************************************************************************************************
//
// Class Name 		: sResponse_GMC_GMOrder
//
// Last Modified 	: 2009 / 07 / 02 
// Created		 	: 박기형
//
// Function			: [응답] GM명령어의 실행 결과를 알려준다.
//
//***************************************************************************************************

#define GMORDER_RESULT_FAIL 0
#define GMORDER_RESULT_SUCCESS 1
#define GMORDER_RESULT_DENY 2
struct sResponse_GMC_GMOrder
{
	sPacketHeader				packetHeader;

	TCHAR						order[ 256 ];

	SI32						siWorldId;				// 게임 월드 아이디	
	TCHAR						szRequestID[20];		// 요청자 아이디
	TCHAR						szRequestIP[20];		// 요청자 아이피
	TCHAR						szRequestTime[20];		// 요청 시간
	
	SI32						siResult;			    // 결과 값

	sResponse_GMC_GMOrder( TCHAR *str,SI32 siWorldId_Para,TCHAR* pszRequestID_Para,TCHAR* pszRequestIP_Para,TCHAR* pszRequestTime_Para, SI32 siResult_Para )
	{
		packetHeader.usCmd = GOONZUMC_RESPONSE_GMORDER;
		packetHeader.usSize = sizeof( sResponse_GMC_GMOrder );

		memcpy( order, str, 256 );
		order[ 255 ] = NULL;

		siWorldId = siWorldId_Para;

		memcpy( szRequestID, pszRequestID_Para, 20 );
		szRequestID[ 19 ] = NULL;

		memcpy( szRequestIP, pszRequestIP_Para, 20 );
		szRequestIP[ 19 ] = NULL;

		memcpy( szRequestTime, pszRequestTime_Para, 20 );
		szRequestTime[ 19 ] = NULL;

		siResult = siResult_Para;
	}
};



//***************************************************************************************************
//
// Class Name 		: sRequest_GMC_MakeFile
//
// Last Modified 	: 2007 / 03 / 11
// Created		 	: 김광명
//
// Function			: [요청] 서버에 패치할 파일을 각 서버컴퓨터 폴더에 만든다.
//						     모니터링 -> 마스터 서버
//
//***************************************************************************************************
struct sRequest_GMC_MakeFile
{
	sPacketHeader				packetHeader;

	TCHAR						m_szFileName[ MAX_PATH ];

	sRequest_GMC_MakeFile( TCHAR *str )
	{
		packetHeader.usCmd = GOONZUMC_REQUEST_MAKEFILE;
		packetHeader.usSize = sizeof( sRequest_GMC_MakeFile );

		_tcsncpy( m_szFileName, str, MAX_PATH );
	}

	TCHAR* GetFileName()
	{
		return m_szFileName;
	}
};

//***************************************************************************************************
//
// Class Name 		: sResponse_GMC_MakeFile
//
// Last Modified 	: 2007 / 03 / 11
// Created		 	: 김광명
//
// Function			: [응답] 서버에 패치할 파일을 각 서버컴퓨터 폴더에 만든다.
//                           마스터 서버 -> 서버 에이전트
//
//***************************************************************************************************
struct sResponse_GMC_MakeFile
{
	sPacketHeader				packetHeader;

	TCHAR						m_szFileName[ MAX_PATH ];

	sResponse_GMC_MakeFile( TCHAR *str )
	{
		packetHeader.usCmd = MASTERMSG_COMMAND_MAKEFILE;
		packetHeader.usSize = sizeof( sResponse_GMC_MakeFile );

		_tcsncpy( m_szFileName, str, MAX_PATH );
	}

	TCHAR* GetFileName()
	{
		return m_szFileName;
	}
};

//***************************************************************************************************
//
// Class Name 		: sRequest_GMC_MakeDirectory
//
// Last Modified 	: 2007 / 03 / 11
// Created		 	: 김광명
//
// Function			: [요청] 서버에 복사될 파일의 폴더를 만들어준다.
//                           모니터링 -> 마스터 서버
//
//***************************************************************************************************
struct sRequest_GMC_MakeDirectory
{
	sPacketHeader				packetHeader;

	TCHAR						m_szDirectoryName[ MAX_PATH ];

	sRequest_GMC_MakeDirectory( TCHAR *str )
	{
		packetHeader.usCmd = GOONZUMC_REQUEST_MAKEDIRECTORY;
		packetHeader.usSize = sizeof( sRequest_GMC_MakeDirectory );

		_tcsncpy( m_szDirectoryName, str, MAX_PATH );
	}

	TCHAR* GetDirectoryName()
	{
		return m_szDirectoryName;
	}
};

//***************************************************************************************************
//
// Class Name 		: sResponse_GMC_MakeDirectory
//
// Last Modified 	: 2007 / 03 / 11
// Created		 	: 김광명
//
// Function			: [응답] 서버에 복사될 파일의 폴더를 만들어준다.
//                           마스터 서버 -> 서버 에이전트
//
//***************************************************************************************************
struct sResponse_GMC_MakeDirectory
{
	sPacketHeader				packetHeader;

	TCHAR						m_szDirectoryName[ MAX_PATH ];

	sResponse_GMC_MakeDirectory( TCHAR *str )
	{
		packetHeader.usCmd = MASTERMSG_COMMAND_MAKEDIRECTORY;
		packetHeader.usSize = sizeof( sResponse_GMC_MakeDirectory );

		_tcsncpy( m_szDirectoryName, str, MAX_PATH );
	}

	TCHAR* GetDirectoryName()
	{
		return m_szDirectoryName;
	}
};

const int MAX_SEND_BYPE = 1000;
//***************************************************************************************************
//
// Class Name 		: sRequest_GMC_SendFileData
//
// Last Modified 	: 2007 / 03 / 11
// Created		 	: 김광명
//
// Function			: [요청] 서버에 복사될 파일의 실제 정보를 보낸다.
//
//***************************************************************************************************
class sRequest_GMC_SendFileData
{
public:
	sPacketHeader		packetHeader;

	SI32				m_siSendCount;
	SI32				m_siReadData;
	TCHAR				m_szFileName[ MAX_PATH ];
	TCHAR				m_cData[MAX_SEND_BYPE];

	sRequest_GMC_SendFileData( SI32 SendCount, SI32 ReadData, TCHAR* pFileName, TCHAR* pData ) 
	{
		packetHeader.usSize = sizeof( sRequest_GMC_SendFileData );
		packetHeader.usCmd = GOONZUMC_REQUEST_SEND_FILEDATA;

		m_siSendCount = SendCount;
		m_siReadData = ReadData;
		m_szFileName[0] = '\0';
		_tcsncpy( m_szFileName, pFileName, MAX_PATH );
		m_cData[0] = '\0';
		memcpy( m_cData, pData, MAX_SEND_BYPE);
	}
};

//***************************************************************************************************
//
// Class Name 		: sResponse_GMC_SendFileData
//
// Last Modified 	: 2007 / 03 / 11
// Created		 	: 김광명
//
// Function			: [요청] 서버에 복사될 파일의 실제 정보를 보낸다.
//
//***************************************************************************************************
class sResponse_GMC_SendFileData
{
public:
	sPacketHeader		packetHeader;

	SI32				m_siSendCount;
	SI32				m_siReadData;
	TCHAR				m_szFileName[ MAX_PATH ];
	TCHAR				m_cData[MAX_SEND_BYPE];

	sResponse_GMC_SendFileData( SI32 SendCount, SI32 ReadData, TCHAR* pFileName, TCHAR* pData ) 
	{
		packetHeader.usSize = sizeof( sResponse_GMC_SendFileData );
		packetHeader.usCmd = MASTERMSG_COMMAND_SEND_FILEDATA;

		m_siSendCount = SendCount;
		m_siReadData = ReadData;
		m_szFileName[0] = '\0';
		_tcsncpy( m_szFileName, pFileName, MAX_PATH );
		m_cData[0] = '\0';
		memcpy( m_cData, pData, MAX_SEND_BYPE);
	}
};

//***************************************************************************************************
//
// Class Name 		: sRequest_GMC_SendFileData
//
// Last Modified 	: 2007 / 03 / 11
// Created		 	: 김광명
//
// Function			: [요청] 서버에 복사될 파일의 실제 정보를 보낸다.
//
//***************************************************************************************************
struct sRequest_Send_ToolsLog_MasterServer
{
	sPacketHeader		packetHeader;

	TCHAR szServerName[10];
	TCHAR szCommand[50];		// 실행한 명령
	TCHAR szAccountID[20];	// 계정
	TCHAR szCharName[20];	// 캐릭터 이름
	TCHAR szLog[256];		// 세부 설명

	sRequest_Send_ToolsLog_MasterServer( TCHAR* ServerName, TCHAR* Command, TCHAR* AccountID, TCHAR* CharName, TCHAR* Log )
	{
		packetHeader.usSize = sizeof( sRequest_Send_ToolsLog_MasterServer );
		packetHeader.usCmd	= GAMESERVER_SEND_TOOLSLOG_MASTER;

		memcpy( szServerName, ServerName, 10);
		szServerName[9] = NULL;
		memcpy( szCommand,	  Command, 50);
		szCommand[49] = NULL;
		memcpy( szAccountID, AccountID, 20);
		szAccountID[19] = NULL;
		memcpy( szCharName,  CharName, 20);
		szCharName[19] = NULL;
		memcpy( szLog,		  Log, 256);
		szLog[255] = NULL;
	}
};

//***************************************************************************************************
//
// Class Name 		: sRequest_GMC_UserCharInfo
//
// Last Modified 	: 2007 / 07 / 03
// Created		 	: 이강형
//
// Function			: [요청] 계정의 모든 캐릭터 이름을 요청한다.
//
//***************************************************************************************************
class sRequest_GMC_AccountCharInfo
{
public:
	sPacketHeader		m_packetHeader;
	SI32				m_siCharID;			// 캐릭터번호 - MasterServer에서 설정한다.
	TCHAR				m_szAccountID[20];	// 계정ID

	sRequest_GMC_AccountCharInfo(SI32 siServerID, TCHAR* pszAccountID)
	{
		m_packetHeader.usSize	= sizeof( sRequest_GMC_AccountCharInfo );
		m_packetHeader.usCmd	= GOONZUGM_REQUEST_ACCOUNTCHARINFO;
		m_siCharID				= 0;

		m_szAccountID[0] = '\0';
		if(pszAccountID)		_tcsncpy(m_szAccountID, pszAccountID, sizeof(m_szAccountID));
	}
};

class sResponse_GMC_AccountCharInfo
{
public:
	sPacketHeader		m_packetHeader;
	SI32				m_siCharID;			// 캐릭터번호 - MasterServer에서 설정한다.

	TCHAR				m_szAccountID[20];	// 계정ID
	TCHAR				m_szServerName[32];	// 서버명

	TCHAR				m_szCharName1[20];	// 캐릭터1
	TCHAR				m_szCharName2[20];	// 캐릭터2
	TCHAR				m_szCharName3[20];	// 캐릭터3

	sResponse_GMC_AccountCharInfo()
	{
		m_packetHeader.usSize = sizeof( sResponse_GMC_AccountCharInfo );
		m_packetHeader.usCmd	= GOONZUGM_RESPONSE_ACCOUNTCHARINFO;

		m_szCharName1[0] = '\0';
		m_szCharName2[0] = '\0';
		m_szCharName3[0] = '\0';
	}
};


//***************************************************************************************************
//
// Class Name 		: sRequest_GMC_UserCharInfo
//
// Last Modified 	: 2007 / 07 / 03
// Created		 	: 이강형
//
// Function			: [요청] 캐릭터정보를 요청한다.
//
//***************************************************************************************************
class sRequest_GMC_UserCharInfo
{
public:
	sPacketHeader		m_packetHeader;
	SI32				m_siCharID;			// 캐릭터번호 - MasterServer에서 설정한다.

	SI32				m_siServerID;
	TCHAR				m_szCharName[20];	// 캐릭터 이름

	sRequest_GMC_UserCharInfo(SI32 siServerID, TCHAR* pszCharName)
	{
		m_packetHeader.usSize	= sizeof( sRequest_GMC_UserCharInfo );
		m_packetHeader.usCmd	= GOONZUGM_REQUEST_USERCHARINFO;
		m_siCharID				= 0;

		m_siServerID = siServerID;

		m_szCharName[0] = '\0';
		if(pszCharName)		_tcsncpy(m_szCharName, pszCharName, sizeof(m_szCharName));
	}
};

class sResponse_GMC_UserCharInfo
{
public:
	sPacketHeader		m_packetHeader;
	SI32				m_siCharID;			// 캐릭터번호 - MasterServer에서 설정한다.

	TCHAR				m_szAccountID[30];
	TCHAR				m_szPassword[30];
	bool				m_bConnectSwitch;		// 접속 여부. 

	SI64 				m_clTotalMoney;		// 소지금
	SI64 				m_clBankMoney;		// 전장금액

	SI64				m_siWorldTradeMoney;

	//cltSummonInfo		m_clSummonInfo;		// 소환수 정보
	SI32				m_siVillageUnique;	// 가입 마을
	UI08				m_uiWarnMode;		// 경고(블록 상태)

	UI08				m_uiBlockMode;
	TCHAR				m_szBlockReason[200] ;

	SI32				m_siPersonID;
	SI16				m_siLevel;
	SI64				m_siMoney;

	SI16				m_siMapIndex;
	SI16				m_siX;
	SI16				m_siY;
	SI08				m_siGoonzuFriends;

	//TCHAR				m_szOtherPersonName1[ MAX_PLAYER_NAME ];
	//TCHAR				m_szOtherPersonName2[ MAX_PLAYER_NAME ];
	TCHAR				m_szBlockStartDate[24];
	TCHAR				m_szBlockEndDate[24];

	TCHAR				m_szRecommender[20];		// 추천인
	TCHAR				m_szFatherName[20];			// 스승
	SI16				m_siStudentLevel;			// 제자가 되었을 때의 레벨

	TCHAR				m_szMakeDate[24];			// 캐릭터 생성일

	//cltItem clItem[MAX_ITEM_PER_PERSON];
	SI16	m_siItemUnique[110];
	SI16	m_siItemCount[110];


	sResponse_GMC_UserCharInfo()
	{
		ZeroMemory(this, sizeof(sResponse_GMC_UserCharInfo));

		m_packetHeader.usSize = sizeof( sResponse_GMC_UserCharInfo );
		m_packetHeader.usCmd	= GOONZUGM_RESPONSE_USERCHARINFO;
	}
};

//***************************************************************************************************
//
// Class Name 		: sRequest_GameDBMgr_ErroyQuery_String
//
// Last Modified 	: 2009 / 03 / 30
// Created		 	: 이강형
//
// Function			: [GAMEDBMGR->MASTERSERVER] 에러쿼리가 발생했을때 전송한다.
//
//***************************************************************************************************
class sRequest_GameDBMgr_ErroyQuery_String
{
public:
	sPacketHeader		m_packetHeader;

	TCHAR				m_szMapString[1024];

	sRequest_GameDBMgr_ErroyQuery_String( TCHAR* pszMapStr )
	{
		ZeroMemory(this, sizeof(sRequest_GameDBMgr_ErroyQuery_String));

		m_packetHeader.usSize = sizeof( sRequest_GameDBMgr_ErroyQuery_String );
		m_packetHeader.usCmd	= MASTERMSG_REQUEST_GAMEDBMGR_ERRORQUERY_STRING;

		StringCchCopy( m_szMapString, sizeof(m_szMapString), pszMapStr );
	}
};

//***************************************************************************************************
//
// Class Name 		: sResponse_GameDBMgr_ErrorQuery_String
//
// Last Modified 	: 2009 / 03 / 30
// Created		 	: 이강형
//
// Function			: [MASTERSERVER->GOONZUMC] 에러쿼리 정보를 전송한다.
//
//***************************************************************************************************
class sResponse_GameDBMgr_ErrorQuery_String
{
public:
	sPacketHeader		m_packetHeader;

	TCHAR				m_szMapString[1024];

	sResponse_GameDBMgr_ErrorQuery_String( TCHAR* pszMapStr )
	{
		ZeroMemory(this, sizeof(sResponse_GameDBMgr_ErrorQuery_String));

		m_packetHeader.usSize = sizeof( sResponse_GameDBMgr_ErrorQuery_String );
		m_packetHeader.usCmd	= GOONZUMC_RESPONSE_GAMEDBMGR_ERRORQUERY_STRING;

		StringCchCopy( m_szMapString, sizeof(m_szMapString), pszMapStr );
	}
};

#pragma pack()

#endif

