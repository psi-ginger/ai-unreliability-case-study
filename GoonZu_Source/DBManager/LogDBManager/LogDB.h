#ifndef _LOGDB_H_
#define _LOGDB_H_

#include "ITZSession.h"
#include "ITZPerson.h"
#include "../../RichODBC/ODBC/RichODBC.h"

#include "LogDBMsg.h"

#define MAX_USERS		10

extern BOOL					g_bInitReady;						// 초기화 준비가 됨 
extern BOOL					g_bInitComplete;					// 초기화 완료 됨 

extern SI32					g_siServerID;

extern char						g_dbname[];
extern char						g_dbid[];
extern char						g_dbpw[];

void InitServer( UINT uiPort, char *szDBIP, char *szDatabase, char *szUserID, char *szUserPW );

class cLogClient : public ITZPerson
{
public:
	cLogClient()
	{
		m_bRequestLog = FALSE;
		ZeroMemory(&m_RequestLog, sizeof(m_RequestLog));

		m_bRequestNewLog = FALSE;	
		ZeroMemory(&m_RequestNewLog, sizeof(m_RequestNewLog));	

		m_bRequestToolsLog = FALSE;	
		ZeroMemory(&m_RequestToolsLog, sizeof(m_RequestToolsLog));	
	};
	~cLogClient() {};

	BOOL				m_bRequestLog;
	sDBRequest_Log		m_RequestLog;
	BOOL				m_bRequestNewLog;
	sDBRequest_NewLog	m_RequestNewLog;

	// 중국에서 사용한다.
	BOOL				m_bRequestToolsLog;
	sDBRequest_ToolsLog	m_RequestToolsLog;

public:
	virtual void Init()
	{	
		printf( "LogClient[%d - %s] - Connected\n", GetCharID(), GetSession()->GetIP() );	

		m_bRequestLog = FALSE;	
		ZeroMemory(&m_RequestLog, sizeof(m_RequestLog));	

		m_bRequestNewLog = FALSE;	
		ZeroMemory(&m_RequestNewLog, sizeof(m_RequestNewLog));	

		m_bRequestToolsLog = FALSE;	
		ZeroMemory(&m_RequestToolsLog, sizeof(m_RequestToolsLog));	
	}

	virtual void Close()
	{	
		printf( "LogClient[%d - %s] - Disconnected\n", GetCharID(), GetSession()->GetIP() );	

		m_bRequestLog = FALSE;	
		ZeroMemory(&m_RequestLog, sizeof(m_RequestLog));	

		m_bRequestNewLog = FALSE;	
		ZeroMemory(&m_RequestNewLog, sizeof(m_RequestNewLog));	

		m_bRequestToolsLog = FALSE;	
		ZeroMemory(&m_RequestToolsLog, sizeof(m_RequestToolsLog));	
	}

	BOOL SetRequestLog(sDBRequest_Log* pPacket)
	{
		if(m_bRequestLog)			return FALSE;

		memcpy(&m_RequestLog, pPacket, sizeof(m_RequestLog));
		m_bRequestLog = TRUE;

		return TRUE;
	}

	BOOL SetRequestNewLog(sDBRequest_NewLog* pPacket)
	{
		if(m_bRequestNewLog)			return FALSE;

		memset( &m_RequestNewLog, 0, sizeof(m_RequestNewLog) );
		// memcpy(&m_RequestNewLog, pPacket, sizeof(m_RequestNewLog));
		// 패킷에서 크기를 찾는다.
		memcpy( &m_RequestNewLog, pPacket, pPacket->packetHeader.usSize );
		m_bRequestNewLog = TRUE;

		return TRUE;
	}

	BOOL SetRequestToolsLog(sDBRequest_ToolsLog* pPacket)
	{
		if( TRUE == m_bRequestToolsLog )
			return FALSE;

		memcpy(&m_RequestToolsLog, pPacket, sizeof(m_RequestToolsLog));
		m_bRequestToolsLog = TRUE;

		return TRUE;
	}
};

class NThreadParameter
{
public:
	char				m_szSPName[ 512 ];
	RichODBC			m_odbc;

	cLogClient*			m_pClient;
	
	HANDLE				m_hThread;
	BOOL				m_bThreadRunning;

public:
	NThreadParameter()
	{
		m_pClient			= NULL;
		m_hThread			= NULL;
		m_bThreadRunning	= TRUE;
	};
	~NThreadParameter()	{};

	BOOL DBConnect(char* pDriver, char* pServer, char* pDatabase, char* pID, char *pPW)
	{
		return m_odbc.Connect( pDriver, pServer, pDatabase, pID, pPW );
	}

	BOOL DBDisconnect()
	{
		return m_odbc.Disconnect();
	}
};

void ThreadRunning(LPVOID lpParam);

#endif
