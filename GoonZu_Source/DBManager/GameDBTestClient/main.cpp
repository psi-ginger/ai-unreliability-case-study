#include <windows.h>
#include <stdio.h>
#include <conio.h>

#include "ITZSendBuffer.h"
#include "GameDBProcess.h"
#include "../GameDBManager/DBMsg.h"


// 윈속 초기화
void WinSockInit()
{
	WORD	wVersionRequested;
	WSADATA	wsaData;

	int err;

	wVersionRequested = MAKEWORD( 2, 2 );

	err = WSAStartup( wVersionRequested, &wsaData );

	if( err != 0 ) {
		return;
	}

	if( LOBYTE( wsaData.wVersion ) != 2 ||
		HIBYTE( wsaData.wVersion ) != 2 ) {

		WSACleanup();
		return;
	}
}



void main()
{

	WinSockInit();

	printf( "-----------------------------------------------------\n" );
	printf( "Game DB Test Client\n" );
	printf( "-----------------------------------------------------\n" );

	GameDBProcess	gd;

	gd.Init( "127.0.0.1", 9001 );

	BOOL bRun = TRUE;

	// console
	while( bRun ) {

		gd.Run();

		Sleep( 10 );

		if( kbhit() ) {

			switch( getch() )
			{
			case 27:
				{
					bRun = FALSE;
				}
				break;

			case '1':
				{
					sDBRequest_PersonListInfoGet	msg( 0, 1 );

					gd.GetSession()->GetSendBuffer()->Write( (char *)&msg );
				}
				break;

			}

		}

	}

	WSACleanup();
}

