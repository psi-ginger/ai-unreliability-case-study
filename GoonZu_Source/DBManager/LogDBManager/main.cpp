#include <windows.h>
#include <stdio.h>
#include <conio.h>

#include "LogDB.h"
#include "DBLogProtocol.h"

void dispInitView();

DBLogProtocol			g_dap;

BOOL					g_bInitReady;						// 초기화 준비가 됨 
BOOL					g_bInitComplete;					// 초기화 완료 됨 

SI32					g_siServerID;

char					g_dbname[ 128 ];
char					g_dbid[ 128 ];
char					g_dbpw[ 128 ];

TCHAR					szModifyDate3[64];// = "2008-07-17";		// 리얼 패치일
TCHAR					szModifyDate4[64];// = "2009-02-03";		// 리얼 패치일

void InitServer( UINT uiPort, char *szDBIP, char *szDatabase, char *szUserID, char *szUserPW )
{

	strcpy( g_dbname, szDatabase );
	strcpy( g_dbid, szUserID );
	strcpy( g_dbpw, szUserPW );

	printf( "- Database Connecting (IP:%s)\n", szDBIP );

	g_dap.Init( uiPort, szDBIP, szDatabase, szUserID, szUserPW );
	g_dap.CreateThread();

	printf( "Init Completed\n" );
}

void main()
{
	g_bInitReady = FALSE;
	g_bInitComplete = FALSE;

	dispInitView();

	BOOL bRun = TRUE;

	char	szPath[MAX_PATH];
	char	szExecuteFile[MAX_PATH];
	::GetModuleFileName(NULL, szExecuteFile, MAX_PATH);
	char*	pFind = strrchr(szExecuteFile, '\\');
	szExecuteFile[pFind - szExecuteFile] = '\0';
	strcpy(szPath, szExecuteFile);

	char	szINIFile[MAX_PATH];
	strcpy(szINIFile, szPath);
	strcat(szINIFile, "\\Setting.ini");

	DWORD	dwLen;
	char	szServerIP[40];
	char	szDatabase[40];
	char	szID[40];
	char	szPW[40];

	szServerIP[0]	= '\0';
	szDatabase[0]	= '\0';
	szID[0]			= '\0';
	szPW[0]			= '\0';

	dwLen = GetPrivateProfileString("LOGDB", "ServerIP", "", szServerIP, 40, szINIFile);
	if(dwLen <= 0)				return;
	dwLen = GetPrivateProfileString("LOGDB", "Database", "", szDatabase, 40, szINIFile);
	if(dwLen <= 0)				return;
	dwLen = GetPrivateProfileString("LOGDB", "ID", "", szID, 40, szINIFile);
	if(dwLen <= 0)				return;
	dwLen = GetPrivateProfileString("LOGDB", "PW", "", szPW, 40, szINIFile);
	if(dwLen <= 0)				return;

	UINT uiPort = GetPrivateProfileInt("LOGDBMANAGER", "Port", 0, szINIFile);
	if(uiPort <= 0)				return;
	dwLen = GetPrivateProfileString("LOGDBMANAGER", "ModifyDate_Log3", "", szModifyDate3, 64, szINIFile);
	if(dwLen <= 0)				return;
	dwLen = GetPrivateProfileString("LOGDBMANAGER", "ModifyDate_Log4", "", szModifyDate4, 64, szINIFile);
	if(dwLen <= 0)				return;

	InitServer( uiPort, szServerIP, szDatabase, szID, szPW);
	g_bInitReady = TRUE;

	// console
	while( bRun )
	{
		Sleep( 10 );

		// 초기화가 준비 되었으면, 리슨 포트를 열고 초기화를 완료한다 
		if( g_bInitReady == TRUE && g_bInitComplete == FALSE )
		{
			g_dap.GetListen()->Start();

			g_bInitComplete = TRUE;
		}


		if( g_bInitComplete == TRUE ) {

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
	printf( "LogDBManager\n" );
	printf( "-------------------------------------------------------------------------------\n" );
	printf( "\n" );
}

