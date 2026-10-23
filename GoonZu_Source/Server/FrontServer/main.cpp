#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <time.h>

#include "FrontServer.h"
#include "FrontClientProtocol.h"
#include "FrontGameProtocol.h"
#include "AuthProcess.h"
#include "AgentProcess.h"

#include "Log.h"

#ifdef USE_PROMOTOR_FRONT_SERVER
	#include "PromoterManager/clPromoterManager.h"
#endif

void WriteConsole( HANDLE hConsole, int x, int y, char *str, ... );
void WriteConsoleAttr( HANDLE hConsole, int x, int y, int len, WORD attr );

void dispMainView();
void dispMainRefresh();

HANDLE g_hStdIn, g_hStdOut;
HANDLE g_hMainView;

CRITICAL_SECTION g_cs;											// 월드 정보 동기화를 위한 크리티컬섹션

char g_strNotice[] = "test.....";

cltWorldList			g_worldlist;

FrontClientProtocol		g_fcp;
FrontGameProtocol		g_fgp;
AuthProcess				g_auth;
AgentProcess			g_agent;

BOOL					g_bInitReady;							// 초기화 준비가 됨 
BOOL					g_bInitComplete;						// 초기화 완료 됨 

SI32					g_siServerID;

UI32					g_versionCount;
UI32					g_versionList[ 4096 ];

sPatchInfo				g_patchInfo;
SI32					g_patchselect;

DWORD					g_dwHeartbeatTick;

DWORD					g_dwClientProtocolTick;
DWORD					g_dwGameProtocolTick;

// 일본 넷마블 서비스 종료
//ISimpleCrypt*			g_pCrypt;

CLogPrint				g_CLogPrint;

char					g_szServiceArea[12];
char					g_szServiceMode[8];

#ifdef USE_PROMOTOR_FRONT_SERVER
	clPromoterManager		g_clPromoterManager;
#endif

void main()
{

	bool bCrypt = false;
	HRESULT hr;

	CoInitialize(NULL);

// 컴파일 옵션에서 중국은 뺐음 - KKM
// 일본 넷마블 서비스 종료
/*#ifdef _JAPAN

	bCrypt = true;

	CLSID clsid;
	hr = ::CLSIDFromProgID( OLESTR("CryptCom.SimpleCrypt.1"), &clsid );

	if( FAILED(hr) ) {
		printf( "Please Install Netmarble_CryptCOM.\n" );
		bCrypt = false;
	}

	if( bCrypt ) {

		hr = CoCreateInstance(
			clsid, 
			NULL, 
			CLSCTX_INPROC_SERVER,
			IID_SIMPLECRYPT,
			(void**)&g_pCrypt );

		if( FAILED( hr ) ) {
			MessageBox( NULL, "Please Install Netmarble_CryptCOM.\r\n\r\n", 0, 0 );
			g_pCrypt = NULL;
			CoUninitialize();
			return;
		}
	}

#endif*/

	// patchinfo
	FILE *pf = fopen( "patchinfo.txt", "r" );

	if( pf ) {


		fscanf( pf, "%s", g_patchInfo.addr[0] );
		fscanf( pf, "%s", g_patchInfo.dir[0] );
		fscanf( pf, "%s", g_patchInfo.id[0] );
		fscanf( pf, "%s", g_patchInfo.pw[0] );

		fscanf( pf, "%s", g_patchInfo.addr[1] );
		fscanf( pf, "%s", g_patchInfo.dir[1] );
		fscanf( pf, "%s", g_patchInfo.id[1] );
		fscanf( pf, "%s", g_patchInfo.pw[1] );

		fscanf( pf, "%d", &g_patchInfo.versionChangeUp );

		fclose( pf );

		g_CLogPrint.WriteFileLog("ProcessLog.txt", __FILE__, __LINE__, "PatchAddr1=%s;PatchDir1=%s;PatchID1=%d;PatchPW1=%d;PatchAddr2=%s;PatchDir2=%s;PatchID2=%d;PatchPW2=%d",
			g_patchInfo.addr[0], g_patchInfo.dir[0], g_patchInfo.id[0], g_patchInfo.pw[0],
			g_patchInfo.addr[1], g_patchInfo.dir[1], g_patchInfo.id[1], g_patchInfo.pw[1]
			);
	}
	else
	{
		g_CLogPrint.WriteFileLog("ErrorLog.txt", __FILE__, __LINE__, "Can not find \"patchinfo.txt\"");
	}

	g_patchselect = 0;

	InitializeCriticalSection( &g_cs );


	g_bInitReady = FALSE;
	g_bInitComplete = FALSE;

	// 초기화
	//g_fcp.Init();
	//g_fgp.Init();

	g_agent.Init( "127.0.0.1", 6400 );

	//g_fcp.CreateThread();
	//g_fgp.CreateThread();

	// standard console handle
	g_hStdIn = GetStdHandle( STD_INPUT_HANDLE );
	g_hStdOut = GetStdHandle( STD_OUTPUT_HANDLE );

	// create mainview screen
	g_hMainView = CreateConsoleScreenBuffer( GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, 0 );

	SetConsoleActiveScreenBuffer( g_hMainView );

	dispMainView();

	BOOL bRun = TRUE;

	DWORD dwCurrentTick;
	DWORD dwLastTick = GetTickCount();

	g_CLogPrint.WriteFileLog( "ProcessLog.txt", __FILE__, __LINE__, "Enter Main Loop" );

	// console
	while( bRun ) 
	{
		Sleep( 10 );

		dwCurrentTick = GetTickCount();
		if( dwCurrentTick - dwLastTick >= 1000 ) 
		{
			dwLastTick = dwCurrentTick;

			dispMainRefresh();
		}

		InterlockedExchange( (LPLONG)&g_dwHeartbeatTick, dwCurrentTick );

		g_agent.Run();

		// 초기화가 준비 되었으면, 리슨 포트를 열고 초기화를 완료한다 
		if( g_bInitReady == TRUE && g_bInitComplete == FALSE ) {

			g_fcp.GetListen()->Start();
			g_fgp.GetListen()->Start();

			g_bInitComplete = TRUE;

			g_CLogPrint.WriteFileLog( "ProcessLog.txt", __FILE__, __LINE__, "Init Completed!" );

			printf( "Init Completed!\n" );
		}

		if( g_bInitComplete == TRUE ) 
		{
			g_auth.Run();
#ifdef USE_PROMOTOR_FRONT_SERVER
			g_clPromoterManager.Action();
#endif
		}

		if( kbhit() ) 
		{
			switch( getch() )
			{
			case 27:
				bRun = FALSE;
				break;
			}
		}
	}

	CloseHandle( g_hMainView );

	DeleteCriticalSection( &g_cs );

	CoUninitialize();

	g_CLogPrint.WriteFileLog( "ProcessLog.txt", __FILE__, __LINE__, "End Program" );
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

void dispMainView()
{

	char buf1[ 32 ], buf2[ 32 ];

	COORD coord = { 0, 23 };

	SetConsoleCursorPosition( g_hMainView, coord );

	WriteConsole( g_hMainView, 0, 0, "--------------------------------------------------------------------------------" );
	WriteConsole( g_hMainView, 0, 1, "Front Server" );
	WriteConsole( g_hMainView, 0, 2, "--------------------------------------------------------------------------------" );

	WriteConsoleAttr( g_hMainView, 0, 0, 80, (1 << 4) | 15 );
	WriteConsoleAttr( g_hMainView, 0, 1, 80, (1 << 4) | 15 );
	WriteConsoleAttr( g_hMainView, 0, 2, 80, (1 << 4) | 15 );

	WriteConsole( g_hMainView, 0, 4, " * Compiled and linked - [DATE: %s  TIME: %s ]", __DATE__, __TIME__ );	

	_strdate( buf1 );
	_strtime( buf2 );

	WriteConsole( g_hMainView, 0, 6, " * Server Started - %s %s", buf1, buf2 );
	WriteConsole( g_hMainView, 0, 8, " * Initialized By Master Server - " );
	WriteConsole( g_hMainView, 0, 10, " * Server Agent Connection - " );
	WriteConsole( g_hMainView, 0, 12, " * Auth Server Connection - " );
	WriteConsole( g_hMainView, 0, 14, " * Number of Connected Game Server - " );
	WriteConsole( g_hMainView, 0, 16, " * Number of Connected User - ");

	WriteConsole( g_hMainView, 50, 8, " * Game Version -          " );


	WriteConsole( g_hMainView, 0, 22, " ESC : Exit" );

	dispMainRefresh();

}

void dispMainRefresh()
{

	char buf1[ 64 ];

	//	if( g_dgp.GetSessionManager()->GetSession(0) == NULL ) return;

	// Initialize By Master Server
	if( g_bInitComplete == TRUE ) {
		strcpy( buf1, "OK     " );

		WriteConsoleAttr( g_hMainView, 34, 8, strlen( buf1 ), 10 );
	} else {

		strcpy( buf1, "NO     " );

		WriteConsoleAttr( g_hMainView, 34, 8, strlen( buf1 ), 6 );
	}
	WriteConsole( g_hMainView, 34, 8, buf1 );

	// Server Agent Connection
	if( g_agent.GetAgentSession()->GetState() == SESSION_STATE_ESTABLISHED ) {

		strcpy( buf1, "Established              " );

		WriteConsoleAttr( g_hMainView, 29, 10, strlen( buf1 ), 10 );

	} else {

		strcpy( buf1, "Not Connected            " );

		WriteConsoleAttr( g_hMainView, 29, 10, strlen( buf1 ), 6 );
	}
	WriteConsole( g_hMainView, 29, 10, buf1 );

	// Auth Server Connection
	if( g_auth.m_pAuthSession != NULL && g_auth.m_pAuthSession->GetState() == SESSION_STATE_ESTABLISHED ) {
		strcpy( buf1, "Established              " );

		WriteConsoleAttr( g_hMainView, 28, 12, strlen( buf1 ), 10 );

	} else {

		strcpy( buf1, "Not Connected            " );

		WriteConsoleAttr( g_hMainView, 28, 12, strlen( buf1 ), 6 );
	}
	WriteConsole( g_hMainView, 28, 12, buf1 );

	// Connected Game Server
	if( g_bInitComplete == TRUE )
		sprintf( buf1, "%d", g_fgp.GetSessionManager()->GetActiveSessionCount() );
	else
		sprintf( buf1, "0" );
	WriteConsole( g_hMainView, 37, 14, buf1 );

	// Connected User 
	if( g_bInitComplete == TRUE )
		sprintf( buf1, "[%4d]", g_fcp.GetSessionManager()->GetActiveSessionCount() );
	else
		sprintf( buf1, "0" );

	WriteConsole( g_hMainView, 30, 16, buf1 );

	// Game Version
	if( g_versionCount > 0 )
		sprintf( buf1, "%d", g_versionList[ g_versionCount - 1 ] );
	else
		strcpy( buf1, "0" );

	WriteConsole( g_hMainView, 68, 8, buf1 );


	/*
	WriteConsole( g_hMainView, 35, 10, "               " );
	WriteConsole( g_hMainView, 45, 12, "               " );

	WriteConsole( g_hMainView, 35, 10, "%d", g_transfered2 - g_lastTransfered );
	WriteConsole( g_hMainView, 45, 12, "%d(%d)", g_transfered, g_transfered2 );

	*/
}
