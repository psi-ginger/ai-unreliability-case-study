#ifndef _LOGCLIENT_H_
#define _LOGCLIENT_H_

#include "../NetworkLib/ITZNetLib/ITZSession.h"
#include "../NetworkLib/ITZNetLib/ITZPerson.h"
#include "../RichODBC/ODBC/RichODBC.h"

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
		m_bSelledItemLog = FALSE;
		ZeroMemory(&m_SelledItemLog, sizeof(m_SelledItemLog));

		m_bTradeItemLog = FALSE;	
		ZeroMemory(&m_TradeItemLog, sizeof(m_TradeItemLog));	
	};
	~cLogClient() {};

	BOOL						m_bSelledItemLog;
	sDBRequest_SelledItemLog	m_SelledItemLog;
	BOOL						m_bTradeItemLog;
	sDBRequest_TradeItemLog		m_TradeItemLog;

public:
	virtual void Init()
	{	
		printf( "LogClient[%d - %s] - Connected\n", GetCharID(), GetSession()->GetIP() );	

		m_bSelledItemLog = FALSE;
		ZeroMemory(&m_SelledItemLog, sizeof(m_SelledItemLog));

		m_bTradeItemLog = FALSE;	
		ZeroMemory(&m_TradeItemLog, sizeof(m_TradeItemLog));	
	}

	virtual void Close()
	{	
		printf( "LogClient[%d - %s] - Disconnected\n", GetCharID(), GetSession()->GetIP() );	

		m_bSelledItemLog = FALSE;
		ZeroMemory(&m_SelledItemLog, sizeof(m_SelledItemLog));

		m_bTradeItemLog = FALSE;	
		ZeroMemory(&m_TradeItemLog, sizeof(m_TradeItemLog));	
	}

	BOOL SetSelledItemLog(sDBRequest_SelledItemLog* pPacket)
	{
		if(m_bSelledItemLog)			return FALSE;

		memcpy(&m_SelledItemLog, pPacket, sizeof(m_SelledItemLog));
		m_bSelledItemLog = TRUE;

		return TRUE;
	}

	BOOL SetTradeItemLog(sDBRequest_TradeItemLog* pPacket)
	{
		if(m_bTradeItemLog)			return FALSE;

		memcpy( &m_TradeItemLog, pPacket, sizeof(m_TradeItemLog) );
		m_bTradeItemLog = TRUE;

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