#ifndef _GAMEDB_H
#define _GAMEDB_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DBLogMsg.h"
#include "ITZPerson.h"
#include "../../RichODBC/ODBC/RichODBC.h"

//#ifdef USE_DBGW
//	#include "../../RichODBC/DBGW/DBGW.h"
//#endif

extern char						g_dbname[];
extern char						g_dbid[];
extern char						g_dbpw[];

extern char						g_logdbname[];
extern char						g_logdbid[];
extern char						g_logdbpw[];

extern char						g_szServiceArea[12];
extern char						g_szServiceMode[8];

extern char						g_BillingServerIP[ 256 ];
extern UI16						g_BillingServerPort;
extern UI32						g_BillingTimeOut;

extern SI32						g_BillingMode;

enum
{
	HANGAME_BILLINGMODE_NONE,
	HANGAME_BILLINGMODE_SOCKET,
	HANGAME_BILLINGMODE_NEVERBILL,
};

#define DBCONNECTION_NUMBER		5

//#ifdef USE_DBGW
//	#define BEGIN_SP(a)				sprintf( m_szSPName, "goonzu_%s", a ); m_odbc.BeginSP( m_szSPName );
//#else
	#define BEGIN_SP(a)				sprintf( m_szSPName, "%s.dbo.%s", g_dbname, a ); m_odbc.BeginSP( m_szSPName );
//#endif

#define END_SP()				m_odbc.EndSP();
#define EXEC_SQL()				if(!m_odbc.ExecSQL()) return;
#define BEGIN_FETCHDATA()		while(m_odbc.FetchData() > 0) {
#define END_FETCHDATA()			}
#define BEGIN_NEXTRECORDSET()	if(m_odbc.NextRecordSet() == 1) {
#define END_NEXTRECORDSET()		}

// 아이템 로그 남기기용
#define LOGITEMDISPLAY(aaa) (UI08)aaa.cData8[0],(UI08)aaa.cData8[1],(UI08)aaa.cData8[2],(UI08)aaa.cData8[3],(UI08)aaa.cData8[4],(UI08)aaa.cData8[5],(UI08)aaa.cData8[6],(UI08)aaa.cData8[7],(UI08)aaa.cData8[8],(UI08)aaa.cData8[9],(UI08)aaa.cData8[10],(UI08)aaa.cData8[11]
// 기간제아이템용
#define USEDAYDISPLAY(aaa) aaa.uiDateYear, aaa.uiDateMonth, aaa.uiDateDay, aaa.uiDateUseDay
// Element
#define ELEMENTDISPLAY(aaa) aaa.Element.siType, aaa.Element.siPower

extern int						g_requestCount;
extern int						g_lastRequestCount;

extern int						g_responseCount;
extern int						g_lastResponseCount;

extern BOOL						g_bRun;


void SendLogMsg( sPacketHeader *pPacket );

void InitServer( UI16 usGameServerListenPort, char *szDBIP, char *szDatabase, char *szUserID, char *szUserPW, char *szLogDBIP, char *szLogDatabase, char *szLogUserID, char *szLogUserPW, char* szLogProcedure );

class cGame : public ITZPerson
{
public:
	cGame() {};
	~cGame() {};

public:
	virtual void Init() {
		printf( "- Game Server Connected\n" ); 
		InterlockedExchange( (LPLONG)&g_requestCount, 0 );
		InterlockedExchange( (LPLONG)&g_responseCount, 0 );

	};
	virtual void Close() { 
		printf( "- Game Server Disconnected\n" );
		InterlockedExchange( (LPLONG)&g_requestCount, 0 );
		InterlockedExchange( (LPLONG)&g_responseCount, 0 );
	};
};

class cLog : public ITZPerson
{
public:
	cLog() {};
	~cLog() {};

public:
	virtual void Init() {};
	virtual void Close() {};
};

//--------------------------------------------------------------------------------------------------
// MINI DUMP - by LEEKH 2007.11.15 - 아틀란티카팀에서 가르쳐준 덤프 생성
//--------------------------------------------------------------------------------------------------
#if defined (USE_MINIDUMP)

#include "dbghelp.h"

// based on dbghelp.h
typedef BOOL (WINAPI *MINIDUMPWRITEDUMP)( HANDLE hProcess, DWORD dwPid, HANDLE hFile, MINIDUMP_TYPE DumpType,
										 CONST PMINIDUMP_EXCEPTION_INFORMATION ExceptionParam,
										 CONST PMINIDUMP_USER_STREAM_INFORMATION UserStreamParam,
										 CONST PMINIDUMP_CALLBACK_INFORMATION CallbackParam );

class MiniDumper
{
public:
	MiniDumper();
	virtual ~MiniDumper(void);

	static void SetHwnd( HWND hwnd )
	{
		m_hWnd = hwnd;
	}

private:
	static LPCSTR m_szAppName;
	static HWND	  m_hWnd;

	static LONG WINAPI TopLevelFilter( struct _EXCEPTION_POINTERS* exceptioninfo );

};

#endif
//--------------------------------------------------------------------------------------------------


#endif

