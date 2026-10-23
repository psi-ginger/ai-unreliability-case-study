#include <stdio.h>
#include <conio.h>
#include <time.h>

#include "AuthServer.h"
#include "AuthFrontProtocol.h"
#include "AuthGameProtocol.h"
#include "AuthUserList.h"
#include "AccountDBProcess.h"
#include "AgentProcess.h"
#include "GASH.h"
#include "UserManager.h"

#include "ITZList.h"



void WriteConsole( HANDLE hConsole, int x, int y, char *str, ... );
void WriteConsoleAttr( HANDLE hConsole, int x, int y, int len, WORD attr );

void dispInitView();
void dispMainView();
void dispMainRefresh();

HANDLE g_hStdIn, g_hStdOut;
HANDLE g_hMainView;


AccountDBProcess	g_accountDB;
AuthGameProtocol	g_agp;
AuthFrontProtocol	g_afp;
AgentProcess		g_agent;
CGash				g_gash;
CUserManager		g_usermanager;
GameActivService::CGameActivService g_GameActivServiceT;

BOOL				g_bInitReady;							// 초기화 준비가 됨 
BOOL				g_bInitComplete;						// 초기화 완료 됨 

SI32				g_siServerID;



void main()
{
	// webservice-
	CoInitialize(NULL);

	dispInitView();

	g_bInitReady = FALSE;
	g_bInitComplete = FALSE;

	// 초기화
	g_agp.Init(); 
	g_afp.Init();

	g_agent.Init( "127.0.0.1", 6400 );

	g_agp.CreateThread();
	g_afp.CreateThread();

	// GASH

	g_gash.Init( 1024, "192.168.6.13" );
	//g_gash.Init( 1024, "127.0.0.1" );
	g_gash.CreateThread();

	// standard console handle
	g_hStdIn = GetStdHandle( STD_INPUT_HANDLE );
	g_hStdOut = GetStdHandle( STD_OUTPUT_HANDLE );

	// create mainview screen
	g_hMainView = CreateConsoleScreenBuffer( GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, 0 );

//	SetConsoleActiveScreenBuffer( g_hMainView );

	dispMainView();


	BOOL bRun = TRUE;

	// console
	while( bRun ) {

		Sleep( 10 );

		g_agent.Run();

		// 초기화가 준비 되었으면, 리슨 포트를 열고 초기화를 완료한다 
		if( g_bInitReady == TRUE && g_bInitComplete == FALSE ) {

			g_agp.GetListen()->Start();
			g_afp.GetListen()->Start();

			g_bInitComplete = TRUE;

			printf( "Init Completed!\n" );
		}


		if( g_bInitComplete == TRUE ) {

			g_accountDB.Run();
		}

		if( kbhit() ) {

			switch( getch() )
			{
			case '1':
				g_gash.AddRequestInfo( REQUESTCODE_AUTH, 1, 1, "test", "test", "111.111.111.111", 0, 0 );
				break;
			
			case '2':
				g_gash.AddRequestInfo( REQUESTCODE_AUTH, 1, 1, "gamagpkstst", "gamepottest", "111.111.111.111", 0, 0 );
				break;

			case 27:
					bRun = FALSE;
				break;
			}
		}

	}

	CloseHandle( g_hMainView );

	// webservice-
	CoUninitialize();
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
	printf( "AuthServer\n" );
	printf( "-------------------------------------------------------------------------------\n" );

	printf( "- Trying to Connect to ServerAgent..\n" );
}

void dispMainView()
{
	
	COORD coord = { 0, 23 };

	SetConsoleCursorPosition( g_hMainView, coord );

	WriteConsole( g_hMainView, 0, 0, "--------------------------------------------------------------------------------" );
	WriteConsole( g_hMainView, 0, 1, "Auth Server" );
	WriteConsole( g_hMainView, 0, 2, "--------------------------------------------------------------------------------" );

	WriteConsoleAttr( g_hMainView, 0, 0, 80, (8 << 4) | 15 );
	WriteConsoleAttr( g_hMainView, 0, 1, 80, (8 << 4) | 15 );
	WriteConsoleAttr( g_hMainView, 0, 2, 80, (8 << 4) | 15 );

	WriteConsole( g_hMainView, 0, 3, "Compiled and linked - [DATE: %s  TIME: %s ]", __DATE__, __TIME__ );	

	WriteConsole( g_hMainView, 0, 22, " ESC : Quit" );

	dispMainRefresh();
	
}

void dispMainRefresh()
{

}
