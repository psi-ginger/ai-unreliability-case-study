#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <time.h>
//#include <crtdbg.h>

#include "GameDB.h"

#include "DBGameLogProtocol.h"
#include "DBGameProtocol.h"
#include "DBGameProtocol2.h"

#include "DBLogProtocol.h"
#include "DBLogProcess.h"

#include "AgentProcess.h"

#include "HanGameBillingProcess/NHanGameBillingProcess.h"

#include "..\..\GoonzuWorld\Common\Item\ItemCommon\cltItem.h"		// 아이템 헤더. 

// [종호_NHN->NDOORS] 빌링 모듈
#include <objbase.h>
#import "bxpgincli.tlb" named_guids
#import "bxPGCLI.tlb" named_guids

using namespace BXPGINCLILib;
using namespace BXPGCLILib;

IPGTXCli *g_pIPGTXCli;
IIPGCli	*g_plPGCli;

#define LOGPORT		20002

void WriteConsole( HANDLE hConsole, int x, int y, char *str, ... );
void WriteConsoleAttr( HANDLE hConsole, int x, int y, int len, WORD attr );

void dispInitView();
void dispMainView();
void dispMainRefresh();

// console screen handle
HANDLE g_hStdIn, g_hStdOut;
HANDLE g_hMainView;

AgentProcess			g_agent;

//DBGameLogProtocol		g_dglp;
DBGameProtocol			g_dgp[ DBCONNECTION_NUMBER ];
DBGameProtocol2			g_dgp2;

DBLogProtocol			g_dbLogProtocol;
DBLogProcess			g_dbLogProcess;

//NHanGameBillingProcess	g_HangameBilling;

BOOL					g_bInitComplete;

BOOL					g_bConnectRun;
HANDLE					g_hEvent;


int						g_requestCount;
int						g_lastRequestCount;

int						g_responseCount;
int						g_lastResponseCount;

BOOL					g_bRun = TRUE;
BOOL					bLogDB;

char					g_dbname[ 128 ];
char					g_dbid[ 128 ];
char					g_dbpw[ 128 ];

char					g_logdbname[ 128 ];
char					g_logdbid[ 128 ];
char					g_logdbpw[ 128 ];

// 한게임 설정 정보 - 일반 설정
//char					g_DBGW_ADDR[ 256 ];
char					g_GameServerName[ 50 ];

// 한게임 설정 정보 - 빌링 설정 (독일(EUROPE)도 동일하게 사용)
char					g_BillingServerIP[ 256 ];
UI16					g_BillingServerPort;
UI32					g_BillingTimeOut;

// 한게임 설정 정보 - 빌링 모드
SI32					g_BillingMode;

char					g_szServiceArea[12];
char					g_szServiceMode[8];

//--------------------------------------------------------------------------------------------------
// MINI DUMP - by LEEKH 2007.11.15 - 아틀란티카팀에서 가르쳐준 덤프 생성
//--------------------------------------------------------------------------------------------------
#if defined (USE_MINIDUMP)

LPCSTR MiniDumper::m_szAppName;

MiniDumper::MiniDumper()
{
	m_szAppName = "GoonZu";

	::SetUnhandledExceptionFilter( TopLevelFilter );
}

MiniDumper::~MiniDumper(void)
{
}

LONG MiniDumper::TopLevelFilter( struct _EXCEPTION_POINTERS* exceptioninfo )
{
	LONG retval  = EXCEPTION_CONTINUE_SEARCH;
	HWND hParent = NULL; 

	HMODULE hDll = NULL;
	char dbghelp[_MAX_PATH];

	if( GetModuleFileName( NULL, dbghelp, _MAX_PATH ))
	{
		char* pSlash = _tcsrchr( dbghelp, '\\' );
		if( pSlash )
		{
			_tcscpy( pSlash + 1, "DBGHELP.DLL" );
			hDll = ::LoadLibrary( dbghelp );
		}
	}

	if( hDll == NULL )
		hDll = ::LoadLibrary( "DBGHELP.DLL" );

	LPCTSTR result = NULL;

	if( hDll )
	{
		MINIDUMPWRITEDUMP pDump = ( MINIDUMPWRITEDUMP )::GetProcAddress( hDll, "MiniDumpWriteDump" );

		if( pDump )
		{
			TCHAR szDumpFileName[MAX_PATH];

			if ( true )
			{
				// 저장파일명 셋팅
				SYSTEMTIME st_Time;
				GetLocalTime(&st_Time);

				StringCchPrintf(szDumpFileName, sizeof(szDumpFileName), TEXT("C:\\GoonZuDump\\DBMgr_%04d%02d%02d_%02d%02d%02d.dmp"), 
					st_Time.wYear, st_Time.wMonth, st_Time.wDay, st_Time.wHour, st_Time.wMinute, st_Time.wSecond );

				CreateDirectory("C:\\GoonZuDump",NULL);
			}

			HANDLE hFile = NULL;
			if(_tcslen(szDumpFileName) > 0)
			{
				// 덤프파일 생성
				hFile = ::CreateFile( szDumpFileName, 
					GENERIC_WRITE, 
					FILE_SHARE_WRITE, 
					NULL, 
					CREATE_ALWAYS,
					FILE_ATTRIBUTE_NORMAL, NULL );
			}

			if( hFile != INVALID_HANDLE_VALUE )
			{

				_MINIDUMP_EXCEPTION_INFORMATION ExInfo;

				ExInfo.ThreadId			 = ::GetCurrentThreadId();
				ExInfo.ExceptionPointers = exceptioninfo;
				ExInfo.ClientPointers	 = NULL;

				// write the dump
				BOOL bOK = pDump( GetCurrentProcess(), GetCurrentProcessId(), hFile, MiniDumpNormal, &ExInfo, NULL, NULL );

				::CloseHandle( hFile );
			}
		}
	}
	return retval;
}

#endif
//--------------------------------------------------------------------------------------------------

void SendLogMsg( sPacketHeader *pPacket )
{
	// New로그로 인해 기존로그는 보내지 않음
	if ( DBLOGMSG_QUERYSTRING == pPacket->usCmd )
	{
		return;
	}

	g_dbLogProcess.SendMsg( pPacket );
}


/*
void GetHanGameInfo()
{
	FILE* fp;
	TCHAR buffer[1024];
	TCHAR* pfilename = TEXT("HanGameInfo.txt");
	fp = _tfopen(pfilename, TEXT("rt"));
	if(fp == NULL)							return;

	TCHAR key[256], value[256];

	while(_fgetts(buffer, 1024, fp) != NULL)
	{
		_stscanf(buffer, TEXT("%s %s"), key, value);

		if(		strcmp(key, "ServiceType") == 0)	{												}
		else if(strcmp(key, "ServiceAge") == 0)		{												}
		else if(strcmp(key, "DBGW_Address") == 0)	{		strcpy(g_DBGW_ADDR, value);				}
		else if(strcmp(key, "GameServerName") == 0)	{		strcpy(g_GameServerName, value);		}
		else if(strcmp(key, "BillingServerIP") == 0)	{		strcpy(g_BillingServerIP, value);		}
		else if(strcmp(key, "BillingServerPort") == 0)	{		g_BillingServerPort = atoi(value);		}
		else if(strcmp(key, "BillingTimeOut") == 0)		{		g_BillingTimeOut = atoi(value);			}
		else if(strcmp(key, "BillingMode") == 0)		{		g_BillingMode = atoi(value);			}
	}

	fclose(fp);

	//------------------------------------------------------------------------
	// 기본값 설정 - 필요한 경우만 추가한다.
	//------------------------------------------------------------------------
	if(g_BillingMode == HANGAME_BILLINGMODE_NONE)		g_BillingMode = HANGAME_BILLINGMODE_SOCKET;
	//------------------------------------------------------------------------
}
*/
void InitServer( UI16 usGameServerListenPort, char *szDBIP, char *szDatabase, char *szUserID, char *szUserPW, char *szLogDBIP, char *szLogDatabase, char *szLogUserID, char *szLogUserPW, char* szLogProcudure )
{

	strcpy( g_dbname, szDatabase );
	strcpy( g_dbid, szUserID );
	strcpy( g_dbpw, szUserPW );

	int i;

//#ifdef USE_DBGW
//	printf( "- Database Connecting (DBGW:%s)\n", g_DBGW_ADDR );
//#else
	printf( "- Database Connecting (IP:%s)\n", szDBIP );
//#endif

	for( i = 0; i < DBCONNECTION_NUMBER; ++i ) {

		g_dgp[ i ].Init( usGameServerListenPort + i, szDBIP, szDatabase, szUserID, szUserPW );

		g_dgp[ i ].CreateThread();
	}

	g_dgp2.Init( usGameServerListenPort - 1, szDBIP, szDatabase, szUserID, szUserPW );

	g_dgp2.CreateThread();

	strcpy( g_logdbname, szLogDatabase );
	strcpy( g_logdbid, szLogUserID );
	strcpy( g_logdbpw, szLogUserPW );

	if( bLogDB ) {
	
		// MasterServer에서 LogDB정보를 가져온다.
		g_dbLogProtocol.Init( LOGPORT, szLogDBIP, szLogDatabase, szLogUserID, szLogUserPW, szLogProcudure );

		g_dbLogProtocol.CreateThread();
		g_dbLogProtocol.GetListen()->Start();
	}

	printf( "- GameServer Listen Port (Port:%d)\n", usGameServerListenPort );

	printf( "- Init Completed\n" );
}


void main()
{

#if defined (USE_MINIDUMP)
	MiniDumper		GoonZuDump;
#endif

	bLogDB = TRUE;

    // 한게임 서비스용 설정파일 읽기
	//GetHanGameInfo();

	if( bLogDB ) g_dbLogProcess.Init( "127.0.0.1", LOGPORT );		

	// init
	g_bInitComplete = FALSE;

	g_agent.Init( "127.0.0.1", 6400 );

//	BOOL bRun = TRUE;

	// standard console handle
	g_hStdIn = GetStdHandle( STD_INPUT_HANDLE );
	g_hStdOut = GetStdHandle( STD_OUTPUT_HANDLE );

	// create mainview screen
	g_hMainView = CreateConsoleScreenBuffer( GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, 0 );

	dispInitView();

	DWORD	dwScreenMode = 0;
	DWORD	dwLastScreenSwitchTick;

	DWORD	dwCurrentTick;
	DWORD	dwLastTick = GetTickCount();
	
	// console
	while( g_bRun ) {

		Sleep( 20 );

		g_dbLogProcess.Run();
 
		g_agent.Run();

		if( g_bInitComplete == FALSE ) {

			if( g_agent.IsInitReady() == TRUE ) {

//				g_dglp.GetListen()->Start();

				for( int i = 0; i < DBCONNECTION_NUMBER; ++i ) {
					g_dgp[ i ].GetListen()->Start();
				}
				
				g_dgp2.GetListen()->Start();
				
				Sleep( 700 );

				dispMainView();
				
				SetConsoleActiveScreenBuffer( g_hMainView );

				dwScreenMode = 1;

				g_bInitComplete = TRUE;

			}

		}

		if( g_bInitComplete == TRUE ) {

			dwCurrentTick = GetTickCount();
			if( dwCurrentTick - dwLastTick >= 1000 ) {
				dwLastTick = dwCurrentTick;

				dispMainRefresh();
			}

			if( dwScreenMode == 2 ) {
				if( dwCurrentTick - dwLastScreenSwitchTick >= 7000 ) {

					SetConsoleActiveScreenBuffer( g_hMainView );
					dwScreenMode = 1;
				}
			}
		}

		if( kbhit() ) {

			switch( getch() )
			{
			case 27:
					g_bRun = FALSE;
				break;

			case 's':
			case 'S':
				{
					if( dwScreenMode == 1 ) {

						printf( "- [S:Change Screen, ESC:Quit]\n" );

						SetConsoleActiveScreenBuffer( g_hStdOut );
						
						dwLastScreenSwitchTick = GetTickCount();
						dwScreenMode = 2;

					} else if( dwScreenMode == 2 ) {

						SetConsoleActiveScreenBuffer( g_hMainView );

						dwScreenMode = 1;
					}
				}
				break;

			}

		}

	}

	g_bConnectRun = FALSE;

	CloseHandle( g_hMainView );


}


// 콘솔 출력 함수
void WriteConsole( HANDLE hConsole, int x, int y, char *str, ... )
{
	DWORD dwWritten;

	COORD coord;

	coord.X = x;
	coord.Y = y;

	char buf[ 128 ];

	wvsprintf( buf, str, (LPSTR)(&str + 1) );

	WriteConsoleOutputCharacter( hConsole, buf, strlen( buf ), coord, &dwWritten );
}

void WriteConsoleAttr( HANDLE hConsole, int x, int y, int len, WORD attr )
{
	DWORD dwWritten;

	COORD coord;

	coord.X = x;
	coord.Y = y;	

	WORD att[ 128 ];

	for( int i = 0; i < len; ++i ) att[ i ] = attr;

	WriteConsoleOutputAttribute( hConsole, att, len, coord, &dwWritten );
}


void dispInitView()
{
	printf( "-------------------------------------------------------------------------------\n" );
	printf( "Game DB Manager\n" );
	printf( "-------------------------------------------------------------------------------\n" );

	printf( "- Trying to Connect to Server Agent\n" );
}

void dispMainView()
{
	char buf1[ 128 ];
	char buf2[ 128 ];
	
	COORD coord = { 0, 23 };

	SetConsoleCursorPosition( g_hMainView, coord );

	WriteConsole( g_hMainView, 0, 0, "--------------------------------------------------------------------------------" );
	WriteConsole( g_hMainView, 0, 1, "Game DB Manager" );
	WriteConsole( g_hMainView, 0, 2, "--------------------------------------------------------------------------------" );

	WriteConsoleAttr( g_hMainView, 0, 0, 80, BACKGROUND_RED | BACKGROUND_GREEN | 15 );
	WriteConsoleAttr( g_hMainView, 0, 1, 80, BACKGROUND_RED | BACKGROUND_GREEN | 15 );
	WriteConsoleAttr( g_hMainView, 0, 2, 80, BACKGROUND_RED | BACKGROUND_GREEN | 15 );


	WriteConsole( g_hMainView, 0, 4, " * Compiled and linked - [DATE: %s  TIME: %s ]", __DATE__, __TIME__ );	

	_strdate( buf1 );
	_strtime( buf2 );
	
	WriteConsole( g_hMainView, 0, 6, " * Server Started - %s %s", buf1, buf2 );
	WriteConsole( g_hMainView, 0, 8, " * Game Server Connection - " );
	WriteConsole( g_hMainView, 0, 10, " * Transfered message per second -           " );
	WriteConsole( g_hMainView, 0, 12, " * Transfered message since server started -                " );

	WriteConsole( g_hMainView, 0, 21, " S : Change Screen" );
	WriteConsole( g_hMainView, 0, 22, " ESC : Quit" );

//	dispMainRefresh();
	
}

void dispMainRefresh()
{
	
	char buf1[ 128 ];
	BOOL bEstablished = TRUE;

	for( int i = 0; i < DBCONNECTION_NUMBER; ++i ) {

		if( g_dgp[ i ].GetSessionManager()->GetSession(0) == NULL ) bEstablished = FALSE;

		if( g_dgp[ i ].GetSessionManager()->GetSession(0)->GetState() != SESSION_STATE_ESTABLISHED ) bEstablished = FALSE;
	}

	if( bEstablished ) {

		strcpy( buf1, "ESTABLISHED              " );

		WriteConsoleAttr( g_hMainView, 28, 8, strlen( buf1 ), 10 );

	} else {

		strcpy( buf1, "LISTENING (not connected)" );

		WriteConsoleAttr( g_hMainView, 28, 8, strlen( buf1 ), 6 );
	}

	WriteConsole( g_hMainView, 28, 8, buf1 );
	WriteConsole( g_hMainView, 35, 10, "               " );
	WriteConsole( g_hMainView, 45, 12, "               " );
	
	WriteConsole( g_hMainView, 35, 10, "Request[%6d], Response[%6d]", 
		g_requestCount - g_lastRequestCount, g_responseCount - g_lastResponseCount );

	WriteConsole( g_hMainView, 45, 12, "Request[%6d], Response[%6d]", g_requestCount, g_responseCount );

	g_lastRequestCount = g_requestCount;
	g_lastResponseCount = g_responseCount;
}

