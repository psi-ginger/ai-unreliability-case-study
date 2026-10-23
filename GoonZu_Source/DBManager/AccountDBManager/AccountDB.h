#ifndef _ACCOUNTDB_H
#define _ACCOUNTDB_H

#include "ITZPerson.h"
#include "../../RichODBC/ODBC/RichODBC.h"

//#ifdef USE_DBGW
//	#include "../../RichODBC/DBGW/DBGW.h"
//#endif

//#ifdef USE_DBGW
//	#define BEGIN_SP(a)				sprintf( m_szSPName, "goonzu_%s", a ); m_odbc.BeginSP( m_szSPName );
//#else
	#define BEGIN_SP(a)				sprintf( m_szSPName, "%s.dbo.%s", g_dbname, a ); m_odbc.BeginSP( m_szSPName );
//#endif

//#define BEGIN_SP(a)				sprintf( m_szSPName, "%s.dbo.%s", g_dbname, a ); m_odbc.BeginSP( m_szSPName );
#define END_SP()				m_odbc.EndSP();
#define EXEC_SQL()				if(!m_odbc.ExecSQL()) break;
#define BEGIN_FETCHDATA()		while(m_odbc.FetchData() > 0) {
#define END_FETCHDATA()			}
#define BEGIN_NEXTRECORDSET()	if(m_odbc.NextRecordSet() == 1) {
#define END_NEXTRECORDSET()		}

class AgentProcess;

extern AgentProcess			g_agent;

extern BOOL					g_bInitReady;						// 초기화 준비가 됨 
extern BOOL					g_bInitComplete;					// 초기화 완료 됨 

extern SI32					g_siServerID;

extern char						g_dbname[];
extern char						g_dbid[];
extern char						g_dbpw[];


void InitServer( char *szDBIP, char *szDatabase, char *szUserID, char *szUserPW );

class cAuth : public ITZPerson
{
public:
	cAuth() {};
	~cAuth() {};

public:
	virtual void Init() { printf( "AuthServer Connected\n" ); };
	virtual void Close() { printf( "AuthServer Disconnected\n" ); };

};


class cMaster : public ITZPerson
{
public:
	cMaster() {};
	~cMaster() {};

public:
	virtual void Init() { printf( "MasterServer Connected\n" ); };
	virtual void Close() { printf( "MasterServer Disconnected\n" ); };

};


#endif

