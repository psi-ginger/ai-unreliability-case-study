#include <windows.h>
#include <stdio.h>
#include <conio.h>

#include "AccountDB.h"
#include "DBAuthProtocol.h"
#include "DBMasterProtocol.h"
#include "AgentProcess.h"

#include "../../NLib/NUtil.h"

#define DBCONNECTION_NUMBER		10

void dispInitView();


DBAuthProtocol		g_dap[ DBCONNECTION_NUMBER ];
DBMasterProtocol	g_dmp;

AgentProcess			g_agent;

BOOL					g_bInitReady;						// �ʱ�ȭ �غ� �� 
BOOL					g_bInitComplete;					// �ʱ�ȭ �Ϸ� �� 

SI32					g_siServerID;


char					g_dbname[ 128 ];
char					g_dbid[ 128 ];
char					g_dbpw[ 128 ];

//char					g_DBGW_ADDR[ 256 ];

unsigned int			g_uiTimeTick;
SYSTEMTIME				g_stLastTime;

char					g_szServiceArea[12];
char					g_szServiceMode[8];

// ---------------------------------------------------------------------------
// Unhandled-exception reporter.
//
// The MSVC /GS check ("Run-Time Check Failure #2 - Stack around the variable
// 'sendMsg' was corrupted") only names the variable, not the code that smashed
// it. This prints the faulting RVA relative to the exe base so it can be looked
// up in the linker .map file, which is what actually identifies the offending
// write. Keeping this in place permanently is cheap and only runs on a fatal.
// ---------------------------------------------------------------------------
static LONG WINAPI AcctDBTopLevelFilter( EXCEPTION_POINTERS *pEP )
{
	if( pEP && pEP->ExceptionRecord )
	{
		DWORD			dwCode	= pEP->ExceptionRecord->ExceptionCode;
		PVOID			pAddr	= pEP->ExceptionRecord->ExceptionAddress;
		HMODULE			hExe	= GetModuleHandle( NULL );
		ULONGLONG		ullRva	= (ULONGLONG)( (BYTE*)pAddr - (BYTE*)hExe );

		printf( "\n[ACCTDB-FATAL] exception=0x%08lX faultAddr=%p exeBase=%p rva=0x%08llX\n",
			(DWORD)dwCode, pAddr, (void*)hExe, ullRva );
		printf( "[ACCTDB-FATAL] resolve rva 0x%08llX against AccountDBManager.map\n", ullRva );
		fflush( stdout );
	}

	return EXCEPTION_EXECUTE_HANDLER;
}



/*void GetHanGameInfo()
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
	}

	fclose(fp);
}*/

void InitServer( char *szDBIP, char *szDatabase, char *szUserID, char *szUserPW )
{

	// These four arrive straight off the network (sResponse_AccountDBManagerInitInfo
	// carries szDatabaseName[20]/szUserID[20]/szUserPW[20] and the _String variant is
	// parsed out of a map string), so bound every copy instead of using strcpy.
	StringCchCopy( g_dbname, sizeof(g_dbname), szDatabase );
	StringCchCopy( g_dbid,    sizeof(g_dbid),    szUserID );
	StringCchCopy( g_dbpw,    sizeof(g_dbpw),    szUserPW );

	int i;

//#ifdef USE_DBGW
//	printf( "- DBGW Connecting (ADDR:%s)\n", g_DBGW_ADDR);
//#else
	printf( "- Database Connecting (IP:%s)\n", szDBIP );
//#endif

	for( i = 0; i < DBCONNECTION_NUMBER; ++i ) {
		g_dap[ i ].Init( 10001 + i, szDBIP, szDatabase, szUserID, szUserPW );
		g_dap[ i ].CreateThread();
	}

	g_dmp.Init( 9100, szDBIP, szDatabase, szUserID, szUserPW );
	g_dmp.CreateThread();

	printf( "Init Completed\n" );
}


void main()
{
	int i;

	g_bInitReady = FALSE;
	g_bInitComplete = FALSE;

	SetUnhandledExceptionFilter( AcctDBTopLevelFilter );

//	GetHanGameInfo();

	dispInitView();
	
	g_agent.Init( "127.0.0.1", 6400 );

	BOOL bRun = TRUE;

	g_uiTimeTick = GetTickCount();

	// �ʱ� �ð� ����
	GetLocalTime(&g_stLastTime);

	// console
	while( bRun ) {

		Sleep( 10 );

		g_agent.Run();

		// �ʱ�ȭ�� �غ� �Ǿ�����, ���� ��Ʈ�� ���� �ʱ�ȭ�� �Ϸ��Ѵ� 
		if( g_bInitReady == TRUE && g_bInitComplete == FALSE ) {

			for( i = 0; i < DBCONNECTION_NUMBER; ++i ) {
				g_dap[ i ].GetListen()->Start();
			}
			g_dmp.GetListen()->Start();	

			g_bInitComplete = TRUE;
		}


		if( g_bInitComplete == TRUE ) {

			bool		bDayChange = false;

			bool		bExecute = false;
			if( strcmp(g_szServiceArea, "KOREA") == 0 )		bExecute = true;
				
			if( bExecute && GetTickCount() - g_uiTimeTick >= 1000 )
			{
				SYSTEMTIME	stTime;		GetLocalTime(&stTime);
				if( g_stLastTime.wDay != stTime.wDay )	{	bDayChange = true;	}

				memcpy(&g_stLastTime, &stTime, sizeof(g_stLastTime));

				// ��¥�� �ٲ������...
				if( stTime.wHour == 0 &&
					stTime.wMinute == 0 &&
					stTime.wSecond >= 0 &&
					stTime.wSecond <= 10 &&
					bDayChange)
				{
					// �Ϸ� ������ ��踦 ��û�Ѵ�.
					SYSTEMTIME sStatisticsTime;
					TimeSubDate(&stTime, 1, &sStatisticsTime);

					// 1. ��������
					g_dap[0].Statistics_UserCount(sStatisticsTime);

					// 2. �̿��� ����
					g_dap[0].Statistics_UserInfo(sStatisticsTime);

					// 3. ��ü �̿��� ������ ����

					// 4. �ְ� �̿��� ������ ����
					g_dap[0].Statistics_WeekUserLevel(sStatisticsTime);

					// 5. �ű԰��� ������ ����
					g_dap[0].Statistics_NewUserLevel(sStatisticsTime);

					// 6. ��Ƽ���̿��� ������ ����
					g_dap[0].Statistics_ActiveUserLevel(sStatisticsTime);

					// 7. ��ü �̿��� ���� �� ���� ����
					g_dap[0].Statistics_TotalUserAgeGender(sStatisticsTime);

					// 8. �ְ� �̿��� ���� �� ���� ����
					g_dap[0].Statistics_WeekUserAgeGender(sStatisticsTime);

					// 9. �ű� ���� ���� �� ���� ����
					g_dap[0].Statistics_NewUserAgeGender(sStatisticsTime);

					// 10. ��Ƽ�� �̿��� ���� �� ���� ����
					g_dap[0].Statistics_ActiveUserAgeGender(sStatisticsTime);
				}
			}
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

}

void dispInitView()
{
	printf( "-------------------------------------------------------------------------------\n" );
	printf( "AccountDBManager\n" );
	printf( "-------------------------------------------------------------------------------\n" );

	printf( "- Trying to Connect to ServerAgent..\n" );
}
