#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <time.h>

#include "AgentMasterProtocol.h"
#include "AgentServerProtocol.h"
#include "SessionList.h"

void WriteConsole( HANDLE hConsole, int x, int y, char *str, ... );
void WriteConsoleAttr( HANDLE hConsole, int x, int y, int len, WORD attr );

void dispMainView();
void dispMainRefresh();



HANDLE g_hStdIn, g_hStdOut;
HANDLE g_hMainView;


AgentMasterProtocol g_amp;
AgentServerProtocol g_asp;
SessionList			g_sessionList;


void main()
{

	// init 
	g_amp.Init();
	g_asp.Init();

	g_amp.CreateThread();
	g_asp.CreateThread();

	g_amp.GetListen()->Start();
	g_asp.GetListen()->Start();

	BOOL bRun = TRUE;

	// standard console handle
	g_hStdIn = GetStdHandle( STD_INPUT_HANDLE );
	g_hStdOut = GetStdHandle( STD_OUTPUT_HANDLE );

	// create mainview screen
	g_hMainView = CreateConsoleScreenBuffer( GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, 0 );

	SetConsoleActiveScreenBuffer( g_hMainView );

	dispMainView();

	DWORD	dwCurrentTick;
	DWORD	dwLastTick = GetTickCount();

	// console
	while( bRun ) {

		Sleep( 10 );


		dwCurrentTick = GetTickCount();

		if( dwCurrentTick - dwLastTick >= 1000 ) {
			dwLastTick = dwCurrentTick;

			dispMainRefresh();
		}
	
		if( kbhit() ) {

			switch( getch() )
			{
			case 27:
					bRun = FALSE;
				break;

			}

		}

	}

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

void dispMainView()
{
	char buf1[ 128 ];
	char buf2[ 128 ];
	
	COORD coord = { 0, 23 };

	SetConsoleCursorPosition( g_hMainView, coord );

	WriteConsole( g_hMainView, 0, 0, "--------------------------------------------------------------------------------" );
	WriteConsole( g_hMainView, 0, 1, "Server Agent" );
	WriteConsole( g_hMainView, 0, 2, "--------------------------------------------------------------------------------" );

	WriteConsoleAttr( g_hMainView, 0, 0, 80, (2 << 4) | 15 );
	WriteConsoleAttr( g_hMainView, 0, 1, 80, (2 << 4) | 15 );
	WriteConsoleAttr( g_hMainView, 0, 2, 80, (2 << 4) | 15 );

	WriteConsole( g_hMainView, 0, 3, "Compiled and linked - [DATE: %s  TIME: %s ]", __DATE__, __TIME__ );	

	_strdate( buf1 );
	_strtime( buf2 );
	
	WriteConsole( g_hMainView,  1,  6, "MasterSession" );
	WriteConsole (g_hMainView, 49,  6, "ServerSession" );
	
	for( int i = 6; i < 18; i++ ) {

		WriteConsole( g_hMainView, 40 - 8, i, "|" );
		WriteConsole( g_hMainView, 39 + 8, i, "|" );
	}

	WriteConsole( g_hMainView, 0, 5, "--------------------------------------------------------------------------------" );
	WriteConsole( g_hMainView, 0, 7, "--------------------------------------------------------------------------------" );
	WriteConsole( g_hMainView, 0, 18, "--------------------------------------------------------------------------------" );

	WriteConsole( g_hMainView, 0, 22, " ESC : Exit" );

	dispMainRefresh();
	
}

void dispMainRefresh()
{

	sSessionInfo sessionInfo[ 10 ];

	g_sessionList.Wait();
	memcpy( sessionInfo,g_sessionList.m_sessionInfo, sizeof( sSessionInfo ) * 10 );
	g_sessionList.Release();
	
	WriteConsole( g_hMainView, 14,  6, "[%2d]", g_amp.GetSessionManager()->GetActiveSessionCount() );
	WriteConsole( g_hMainView, 62,  6, "[%2d]", g_asp.GetSessionManager()->GetActiveSessionCount() );


	for( int i = 0; i < 10; i++ ) {

		WriteConsole( g_hMainView,  1, 8 + i, sessionInfo[ i ].szMasterSessionMessage );
		WriteConsole( g_hMainView, 49, 8 + i, sessionInfo[ i ].szServerSessionMessage );

		if( sessionInfo[ i ].pMaster != NULL && sessionInfo[ i ].pServer != NULL ) {

			WriteConsole( g_hMainView, 34, 8 + i, "ESTABLISHED" );
//			WriteConsoleAttr( g_hMainView, 34, 8 + i, 12, 15 );

			WriteConsoleAttr( g_hMainView, 1, 8 + i, 20, 10 );
			WriteConsoleAttr( g_hMainView, 49, 8 + i, 20, 10 );


		} else {

			WriteConsole( g_hMainView, 34, 8 + i, "           " );
//			WriteConsoleAttr( g_hMainView, 34, 8 + i, 12, 0 );

			if( sessionInfo[ i ].pMaster == NULL && sessionInfo[ i ].pServer == NULL ) {
				WriteConsoleAttr( g_hMainView, 1, 8 + i, 20, 8 );
				WriteConsoleAttr( g_hMainView, 49, 8 + i, 20, 8 );

			} else if( sessionInfo[ i ].pMaster == NULL ) {
				WriteConsoleAttr( g_hMainView, 1, 8 + i, 20, 8 );
				WriteConsoleAttr( g_hMainView, 49, 8 + i, 20, 6 );

			} else if( sessionInfo[ i ].pServer == NULL ) {
				WriteConsoleAttr( g_hMainView, 1, 8 + i, 20, 6 );
				WriteConsoleAttr( g_hMainView, 49, 8 + i, 20, 8 );
			}

		}


	}

	

}
