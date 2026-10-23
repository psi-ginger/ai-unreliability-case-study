
#include <windows.h>
#include <stdio.h>
#include <conio.h>

#include "NBillServer.h"
#include "NBillProtocol.h"


NBillProtocol g_bp;

void main()
{


	//-------------------------------------------

	g_bp.Init();
	g_bp.CreateThread();
	g_bp.GetListen()->Start();	

	BOOL bRun = TRUE;

	printf( "-----------------------------------\n" );
	printf( "NBillServer Started\n" );
	printf( "-----------------------------------\n" );


	// console
	while( bRun ) {

		Sleep( 10 );

		if( kbhit() ) {

			switch( getch() )
			{
			case 27:
				bRun = FALSE;
				break;

			}

		}

	}


}