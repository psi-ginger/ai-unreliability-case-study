
#include <windows.h>
#include <stdio.h>
#include <conio.h>

#include "../../DBManager/AccountDBManager/AccountDBMsg.h"

#include "AccountDBProcess.h"
#include "ServerProcess.h"
#include "MasterClientProtocol.h"
#include "ServerInfo.h"
#include "VersionInfo.h"
#include "MasterMsg.h"
#include "LogFile.h"
#include "ToolsLog.h"
#include "AccessInfo.h"

#include "../../Server/FrontServer/Log.h"

#include <process.h>

#include "Wininet.h"

#include <comutil.h>
#include <tchar.h>
#include <atlcomcli.h>

#include "C:\Program Files\MSSOAP\Inc\mssoap30.h"
#pragma comment(lib, "C:\\Program Files\\MSSOAP\\Lib\\mssoap30.lib") 
#pragma comment(lib, "Wininet.lib")
#pragma comment(lib, "comsupp.lib")

void WriteConsole( HANDLE hConsole, int x, int y, char *str, ... );
void WriteConsoleAttr( HANDLE hConsole, int x, int y, int len, WORD attr );
	
void dispMainView();
void dispMainRefresh();

HANDLE g_hStdIn, g_hStdOut;
HANDLE g_hMainView;

MasterClientProtocol	g_mcp;
ServerProcess			g_sp;
AccountDBProcess		g_ap;
ServerInfo				g_serverinfo;
VersionInfo				g_versioninfo;
CAccessInfo				g_AccessInfo;
CToolsLogProcess		g_ToolsLog;

UserCountInfo			g_UserCountInfo;

UserCountInfo			g_GlobalUserCountInfo;

int						g_view;

int						g_maxUser;

DWORD					g_dwLastNewCharTick;
DWORD					g_dwLastRecordUserNum;
DWORD					g_dwLastServerUserNum;

// 일본 동접 보고용 함수
BOOL HTTPHangameRTA(int Count);	// For HangameJP
bool GamePotReport(int nCount);	// For Gamepot
BOOL HTTPNexonRTA(int Count);	// For NexonJP

// 일본 동접 보고용 스레드
unsigned __stdcall JapanUserCountReportFunc(void* pParameter);
HANDLE					g_hJapanUserCountReport;
BOOL					g_bJapanUserCountReportRun;
UINT					g_uiJapanUserCountReportID;

CLogPrint				g_CLogPrint;

void main()
{
	g_dwLastNewCharTick = GetTickCount();
	g_dwLastServerUserNum = GetTickCount();

	int i;

	g_view  = 0;

	// 파일 읽기
	g_serverinfo.ReadFromFile();
	g_serverinfo.printInfo();
	
	g_versioninfo.ReadFromFile();
	g_versioninfo.printInfo();
	
	g_AccessInfo.ReadFromFile();

	g_mcp.Init();
	g_mcp.CreateThread();
	g_mcp.GetListen()->Start();	

	g_sp.Init();
	g_ap.Init( g_serverinfo.m_accountDBManagerInfo.szPrivateIP, 9100 );
	
#ifdef _CHINA
	g_ToolsLog.Init( g_serverinfo.m_ToolsLogInfo.szPrivateIP, g_serverinfo.m_ToolsLogInfo.usToolsLogServerListenPort );
#endif
	
	// standard console handle
	g_hStdIn = GetStdHandle( STD_INPUT_HANDLE );
	g_hStdOut = GetStdHandle( STD_OUTPUT_HANDLE );

	// create mainview screen
	g_hMainView = CreateConsoleScreenBuffer( GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, 0 );

	SetConsoleActiveScreenBuffer( g_hMainView );

	dispMainView();

	logFile = NULL ;
	logFile = new CLogFile();

	logFile->Init();

	BOOL bRun = TRUE;

	DWORD	dwLastTick = GetTickCount() - 1000;
	DWORD	dwCurrentTick;

	BOOL	bInitMaxUser = FALSE;

	g_dwLastRecordUserNum = 0;

	g_hJapanUserCountReport = INVALID_HANDLE_VALUE;
	if( _tcscmp(g_serverinfo.m_masterServerInfo.szServiceArea, "JAPAN") == 0)
	{
		g_bJapanUserCountReportRun = TRUE;
		// 일본 동접보고용 스레드 생성
		g_hJapanUserCountReport = (HANDLE)_beginthreadex(NULL, 0, &JapanUserCountReportFunc, NULL, 0, &g_uiJapanUserCountReportID);
	}

	// console
	while( bRun )
	{

		Sleep( 10 );
		
		if( GetTickCount() - g_dwLastRecordUserNum >= 2 * 60 * 1000 )
		{
			g_dwLastRecordUserNum = GetTickCount();

			int totuser = 0;

			g_serverinfo.Wait();

			for( i = 0; i < g_serverinfo.m_usWorldCount; ++i ) {
				totuser += g_serverinfo.m_gameRefresh[i].usCurrentUserNum;
			}

			g_serverinfo.Release();

			sDBRequest_RecordUserNum	sendMsg;

			ZeroMemory( &sendMsg, sizeof( sendMsg ) );

			sendMsg.packetHeader.usCmd = DBMSG_REQUEST_RECORDUSERNUM;
			sendMsg.packetHeader.usSize = sizeof( sendMsg );

			sendMsg.siUserNum = totuser;
			g_ap.GetSession()->GetSendBuffer()->Write( (BYTE *)&sendMsg );

		}


		if( GetTickCount() - g_dwLastServerUserNum >= 5 * 60 * 1000 )
		{
			g_dwLastServerUserNum = GetTickCount();
			
			g_serverinfo.Wait();

			UI16		usWorldID;
			for( i = 0; i < g_serverinfo.GetServerCount(); ++i )
			{
				usWorldID = g_serverinfo.m_serverinfo[ i ].usWorldID;

				if( usWorldID == 0 ) continue;

				sDBRequest_RecordServerUserNum	sendMsg;

				ZeroMemory( &sendMsg, sizeof( sendMsg ) );

				sendMsg.packetHeader.usCmd = DBMSG_REQUEST_RECORDSERVERUSERNUM;
				sendMsg.packetHeader.usSize = sizeof( sendMsg );

				strncpy( sendMsg.szServerName,	g_serverinfo.m_gameWorldInfo[ usWorldID - 1 ].szWorldName, sizeof(sendMsg.szServerName) );
				strncpy( sendMsg.szServerIP,	g_serverinfo.m_gameWorldInfo[ usWorldID - 1 ].gameServerInfo.szPublicIP, sizeof(sendMsg.szServerIP) );

				if(g_serverinfo.m_gameRefresh[ usWorldID - 1 ].usStatus == SERVERSTATUS_NOHEARTBEAT)
					sendMsg.siUserNum = -1;
				else
					sendMsg.siUserNum = g_serverinfo.m_gameRefresh[ usWorldID - 1 ].usCurrentUserNum;

				g_ap.GetSession()->GetSendBuffer()->Write( (BYTE *)&sendMsg );
			}

			g_serverinfo.Release();
		}
		//if( GetTickCount() - g_dwLastServerUserNum >= 5 * 60 * 1000 )
		//{
		//	g_dwLastServerUserNum = GetTickCount();

		//	g_serverinfo.Wait();

		//	for( i = 0; i < g_serverinfo.m_usWorldCount; ++i )
		//	{
		//		sDBRequest_RecordServerUserNum	sendMsg;

		//		ZeroMemory( &sendMsg, sizeof( sendMsg ) );

		//		sendMsg.packetHeader.usCmd = DBMSG_REQUEST_RECORDSERVERUSERNUM;
		//		sendMsg.packetHeader.usSize = sizeof( sendMsg );

		//		strncpy( sendMsg.szServerName,	g_serverinfo.m_gameWorldInfo[i].szWorldName, sizeof(sendMsg.szServerName) );
		//		strncpy( sendMsg.szServerIP,	g_serverinfo.m_gameWorldInfo[i].gameServerInfo.szPublicIP, sizeof(sendMsg.szServerIP) );
		//		sendMsg.siUserNum = g_serverinfo.m_gameRefresh[i].usCurrentUserNum;
		//		
		//		g_ap.GetSession()->GetSendBuffer()->Write( (BYTE *)&sendMsg );
		//	}

		//	g_serverinfo.Release();

		//}

		// 전체서버 캐릭터 생성 숫자 얻어오기 
		if( GetTickCount() - g_dwLastNewCharTick >= 3 * 60 * 1000 ) {
			g_dwLastNewCharTick = GetTickCount();

			
			sDBRequest_NewCharNum sendMsg;

			sendMsg.packetHeader.usCmd = DBMSG_REQUEST_NEWCHARNUM;
			sendMsg.packetHeader.usSize = sizeof( sendMsg );

			g_serverinfo.Wait();

			sendMsg.siCount = g_serverinfo.GetWorldCount();			

			int i, j;
			for( i = 0; i < sendMsg.siCount; ++i ) {
				strcpy( sendMsg.szServerName[ i ], g_serverinfo.m_gameWorldInfo[ i ].szWorldName );
				for( j = 0; j < strlen( sendMsg.szServerName[ i ] ); ++j ) {
					if( sendMsg.szServerName[ i ][ j ] == '(' ) {
						sendMsg.szServerName[ i ][ j ] = NULL;
						break;
					}
				}
			}

			g_serverinfo.Release();

			g_ap.GetSession()->GetSendBuffer()->Write( (BYTE *)&sendMsg );

		}


		g_sp.Run();

		g_ap.Run();

#ifdef _CHINA
		g_ToolsLog.Run();
#endif

		dwCurrentTick = GetTickCount();
		if( dwCurrentTick - dwLastTick >= 1000 )
		{
			dwLastTick = dwCurrentTick;
			dispMainRefresh();
		}

		logFile->Run();


		// 최고 동접 초기화
		SYSTEMTIME systime;
		GetLocalTime( &systime );

		if( systime.wHour == 0 &&
			systime.wMinute == 0 &&
			bInitMaxUser == FALSE )
		{
			InterlockedExchange( (LPLONG)&g_maxUser, 0 );
			bInitMaxUser = TRUE;

		} else {
			bInitMaxUser = FALSE;
		}

		int totuser = 0;

		g_serverinfo.Wait();

		for( i = 0; i < g_serverinfo.m_usWorldCount; ++i ) {
			totuser += g_serverinfo.m_gameRefresh[i].usCurrentUserNum;
		}

		g_serverinfo.Release();

		if( g_maxUser < totuser ) InterlockedExchange( (LPLONG)&g_maxUser, totuser );

		if( kbhit() )
		{
			switch( getch() )
			{
			case 27:
				{
					bRun = FALSE;
					if ( logFile ) delete logFile ;
					logFile = NULL ;
				}
				break;

			case 75:				// left
				{
					if( g_view == 0 ) g_view = g_serverinfo.m_usWorldCount;
					else g_view--;

					dispMainRefresh();
				}
				break;

			case 77:				// right
				{
					if( g_view == g_serverinfo.m_usWorldCount ) g_view = 0;
					else g_view++;

					dispMainRefresh();
				}
				break;

			case '1':
				{
				}
				break;
			}
		}
	}
	if ( logFile ) delete logFile ;
	logFile = NULL ;

	g_bJapanUserCountReportRun = FALSE;
	if( g_hJapanUserCountReport != INVALID_HANDLE_VALUE )
	{		
		WaitForSingleObject(g_hJapanUserCountReport, 10 * 1000);
		TerminateThread(g_hJapanUserCountReport, 0);
	}
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
	char buf1[ 128 ]="";
	char buf2[ 128 ]="";
	
	COORD coord = { 0, 23 };

	SetConsoleCursorPosition( g_hMainView, coord );

	WriteConsole( g_hMainView, 0, 0, "--------------------------------------------------------------------------------" );
	WriteConsole( g_hMainView, 0, 1, "Master Server" );
	WriteConsole( g_hMainView, 0, 2, "--------------------------------------------------------------------------------" );

	WriteConsoleAttr( g_hMainView, 0, 0, 80, (8 << 4) | 15 );
	WriteConsoleAttr( g_hMainView, 0, 1, 80, (8 << 4) | 15 );
	WriteConsoleAttr( g_hMainView, 0, 2, 80, (8 << 4) | 15 );

	WriteConsole( g_hMainView, 0, 3, "Compiled and linked - [DATE: %s  TIME: %s ]", __DATE__, __TIME__ );	
	WriteConsole( g_hMainView, 0, 4, "GameVersion[%d]", g_versioninfo.m_uiVersionList[ g_versioninfo.m_uiVersionCount - 1 ] );

	
	for( int i = 6; i < 21; i++ ) {
		WriteConsole( g_hMainView, 39,  i, "|" );
	}

	WriteConsole( g_hMainView, 0, 5, "--------------------------------------------------------------------------------" );
	WriteConsole( g_hMainView, 0, 7, "--------------------------------------------------------------------------------" );

	WriteConsole( g_hMainView, 0, 13, "--------------------------------------------------------------------------------" );
	WriteConsole( g_hMainView, 0, 15, "--------------------------------------------------------------------------------" );

	WriteConsole( g_hMainView, 0, 21, "--------------------------------------------------------------------------------" );

	WriteConsole( g_hMainView,  0, 23, " ESC : Quit" );
	WriteConsole( g_hMainView, 50, 23, "<- Prev Page     Next Page ->" );

	dispMainRefresh();

}

void dispServerInfo( int x, int y, int serverindex )
{
	WriteConsole( g_hMainView, x, y    , "PublicIP :                      " );
	WriteConsole( g_hMainView, x, y + 1, "PrivateIP :                     " );
	WriteConsole( g_hMainView, x, y + 2, "Agent Connection :              " );
	WriteConsole( g_hMainView, x, y + 3, "Heartbeat        :              " );

	WriteConsole( g_hMainView, x + 11, y, "%s", g_serverinfo.m_serverinfo[ serverindex ].szPublicIP );
	WriteConsole( g_hMainView, x + 12, y + 1, "%s", g_serverinfo.m_serverinfo[ serverindex ].szPrivateIP );

	if( g_sp.m_serverSession[ serverindex ]->GetState() == SESSION_STATE_ESTABLISHED ) {
		
		WriteConsole( g_hMainView, x + 19, y + 2, "Established" );
		WriteConsoleAttr( g_hMainView, x + 19, y + 2, 15, 10 );


	} else {

		WriteConsole( g_hMainView, x + 19, y + 2, "Not connected" );
		WriteConsoleAttr( g_hMainView, x + 19, y + 2, 15, 6 );
	}

	BOOL bHeartbeatOK = FALSE;
	UI16 usWorldID = g_serverinfo.m_serverinfo[ serverindex ].usWorldID;
	UI16 usServerType = g_serverinfo.m_serverinfo[ serverindex ].usServerType;

	switch( usServerType ) {
	case SERVERTYPE_FRONTSERVER:
		if( g_serverinfo.m_frontRefresh.usStatus == SERVERSTATUS_OK ) bHeartbeatOK = TRUE;
		break;

	case SERVERTYPE_AUTHSERVER:
		if( g_serverinfo.m_authRefresh.usStatus == SERVERSTATUS_OK ) bHeartbeatOK = TRUE;
		break;

	case SERVERTYPE_ACCOUNTDBMANAGER:
		if( g_serverinfo.m_accountDBManagerRefresh.usStatus == SERVERSTATUS_OK ) bHeartbeatOK = TRUE;
		break;

	case SERVERTYPE_GAMESERVER:
		if( g_serverinfo.m_gameRefresh[ usWorldID - 1 ].usStatus == SERVERSTATUS_OK ) bHeartbeatOK = TRUE;
		break;

	case SERVERTYPE_GAMEDBMANAGER:
		if( g_serverinfo.m_gameDBManagerRefresh[ usWorldID - 1 ].usStatus == SERVERSTATUS_OK ) bHeartbeatOK = TRUE;
		break;
	}

	if( bHeartbeatOK ) {
		
		WriteConsole( g_hMainView, x + 19, y + 3, "OK" );
		WriteConsoleAttr( g_hMainView, x + 19, y + 3, 15, 10 );


	} else {

		WriteConsole( g_hMainView, x + 19, y + 3, "No Heartbeat" );
		WriteConsoleAttr( g_hMainView, x + 19, y + 3, 15, 6 );
	}



}


void dispMainRefresh()
{

	if( g_view == 0 ) {
		
		WriteConsole( g_hMainView,  0,  6, "FrontServer     " );
		WriteConsole( g_hMainView, 41,  6, "AuthServer      " );
		WriteConsole( g_hMainView,  0, 14, "AccountDBManager" );
		WriteConsole( g_hMainView, 41, 14, "AccountDB       " );

		dispServerInfo(  0,  8, 0 );
		dispServerInfo( 41,  8, 1 );
		dispServerInfo(  0, 16, 2 );
		dispServerInfo( 41, 16, 3 );

	} else {

		WriteConsole( g_hMainView,  0,  6, "WorldInfo       " );
		WriteConsole( g_hMainView, 41,  6, "GameServer      " );
		WriteConsole( g_hMainView,  0, 14, "GameDBManager   " );
		WriteConsole( g_hMainView, 41, 14, "GameDB          " );
  
		WriteConsole( g_hMainView,  0,  8, "WorldID :                              " );
		WriteConsole( g_hMainView,  0,  9, "WorldName :                            " );
		WriteConsole( g_hMainView,  0, 10, "                                       " );
		WriteConsole( g_hMainView,  0, 11, "OpenDate :                             " );
		WriteConsoleAttr( g_hMainView, 0, 11, 39, 7 );

		WriteConsole( g_hMainView,  10,  8, "%d", g_serverinfo.m_gameWorldInfo[ g_view - 1 ].usWorldID );
		WriteConsole( g_hMainView,  12,  9, "%s", g_serverinfo.m_gameWorldInfo[ g_view - 1 ].szWorldName );
		WriteConsole( g_hMainView,  11,  11, "%s", g_serverinfo.m_gameWorldInfo[ g_view - 1 ].szOpenDate );

		dispServerInfo( 41,  8, (g_view * 3) + 1 );
		dispServerInfo(  0, 16, (g_view * 3) + 2 );
		dispServerInfo( 41, 16, (g_view * 3) + 3 ); 
	}

	if( g_ap.GetSession()->GetState() == SESSION_STATE_ESTABLISHED ) {
		WriteConsole( g_hMainView, 0, 22, " AccountDBManager: Established  " );
	} else {
		WriteConsole( g_hMainView, 0, 22, " AccountDBManager: Not Connected" );
	}

#ifdef _CHINA
	if( g_ToolsLog.GetToolsLogSession()->GetState() == SESSION_STATE_ESTABLISHED ) {
		WriteConsole( g_hMainView, 35, 22, " ToolsLogDBManager: Established  " );
	} else {
		WriteConsole( g_hMainView, 35, 22, " ToolsLogDBManager: Not Connected" );
	}
#endif

	WriteConsole( g_hMainView, 70, 22, "[%d/%d]", g_view + 1, g_serverinfo.m_usWorldCount + 1 );
}

unsigned __stdcall JapanUserCountReportFunc(void* pParameter)
{
	CoInitialize(NULL);

	// 서로 같은 시간에 호출하면 간섭이 생길 우려가 있으므로 간격을 둔다.
	DWORD	dwHangameJPClock	= GetTickCount();
	DWORD	dwGamepotJPClock	= GetTickCount() - (30 * 1000);
	DWORD	dwNexonJPClock	= GetTickCount() - (60 * 1000);

	BOOL	bExecuteReport = FALSE;
	g_serverinfo.Wait();
	{
		if( _tcscmp(g_serverinfo.m_masterServerInfo.szServiceMode, "REAL") == 0 )
		{
			bExecuteReport = TRUE;
		}
	}
	g_serverinfo.Release();

	UI16	uiUserCount;
	while(g_bJapanUserCountReportRun)
	{
		DWORD dw = GetTickCount() - dwHangameJPClock;
		if( (GetTickCount() - dwHangameJPClock) >= (10 * 60 * 1000) )		// 10분
		{
			dwHangameJPClock += 10 * 60 * 1000;

			g_CLogPrint.WriteFileLog("HTTPHangameRTA.txt", __FILE__, __LINE__, "m_uiHangemeJP:%d", g_UserCountInfo.m_uiHangemeJP);

			g_serverinfo.Wait();
			{
				uiUserCount = g_UserCountInfo.m_uiHangemeJP;
			}
			g_serverinfo.Release();			
			if(bExecuteReport)		HTTPHangameRTA( uiUserCount );
		}

		dw = GetTickCount() - dwGamepotJPClock;
		if( (GetTickCount() - dwGamepotJPClock) >= (1 * 60 * 1000) )		// 1분
		{
			dwGamepotJPClock += 1 * 60 * 1000;

			g_CLogPrint.WriteFileLog("SOAPGamepotRTA.txt", __FILE__, __LINE__, "m_uiGamepotJP:%d", g_UserCountInfo.m_uiGamepotJP);

			g_serverinfo.Wait();
			{
				uiUserCount = g_UserCountInfo.m_uiGamepotJP;
			}
			g_serverinfo.Release();
			if(bExecuteReport)		GamePotReport( uiUserCount );
		}

		// 넥슨 동시접속자 ( RTA )보내기.
		dw = GetTickCount() - dwNexonJPClock;
		if( (GetTickCount() - dwNexonJPClock) >= (5 * 60 * 1000) )		// 5분
		{
			dwNexonJPClock += 5 * 60 * 1000;

			g_CLogPrint.WriteFileLog("HTTPNexonRTA.txt", __FILE__, __LINE__, "m_uiNexonJP:%d", g_UserCountInfo.m_uiNexonJP);

			g_serverinfo.Wait();
			{
				uiUserCount = g_UserCountInfo.m_uiNexonJP;
			}
			g_serverinfo.Release();			
			if(bExecuteReport)		HTTPNexonRTA( uiUserCount );
		}
		
		Sleep(100);
	}

	CoUninitialize();

	return 1;
}

bool GamePotReport(int nCount)
{
	bool			bRet = false;
	HRESULT			hr;
	TCHAR			szMethodName[] = "GameReport";
	LPOLESTR		fszMemberFunc = new OLECHAR[strlen(szMethodName) + 1];

	_bstr_t			bstrWSDLFile = (_T("https://kunshu.postar.jp/api/channeling.asmx?wsdl"));

	DISPPARAMS		dispparams;
	EXCEPINFO		excepinfo;
	VARIANT			returnval;
	unsigned int	uArgErr = 0;

	CComPtr<ISoapClient>	pSoapClient;

	if(fszMemberFunc)		ZeroMemory(fszMemberFunc, sizeof(OLECHAR) * (strlen(szMethodName) + 1));
	::MultiByteToWideChar(CP_ACP, 0, szMethodName, (int)strlen(szMethodName), fszMemberFunc, (int)strlen(szMethodName));

	dispparams.rgvarg = NULL;

	hr = pSoapClient.CoCreateInstance(__uuidof(SoapClient30));
	if( hr == S_OK )
	{
		_bstr_t			empty(_T(""));
		DISPID			dispid;

		hr = pSoapClient->MSSoapInit(bstrWSDLFile, empty, empty, empty);		
		hr = pSoapClient->GetIDsOfNames(IID_NULL, &fszMemberFunc, 1, LOCALE_SYSTEM_DEFAULT, &dispid);

		dispparams.rgvarg				= new VARIANTARG[2];
		dispparams.cArgs				= 2;
		dispparams.cNamedArgs			= 0;
		dispparams.rgdispidNamedArgs	= NULL;

		dispparams.rgvarg[0].vt			= VT_DATE;
		SYSTEMTIME	curTime;
		GetLocalTime(&curTime);
		SystemTimeToVariantTime(&curTime, &dispparams.rgvarg[0].date);

		dispparams.rgvarg[1].vt			= VT_INT;
		dispparams.rgvarg[1].intVal		= nCount;

		VariantInit(&returnval);

		excepinfo.wCode					= 0;
		excepinfo.bstrSource			= NULL;
		excepinfo.bstrDescription		= NULL;
		excepinfo.bstrHelpFile			= NULL;

		hr = pSoapClient->Invoke(dispid, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &dispparams, &returnval, &excepinfo, &uArgErr);

		g_CLogPrint.WriteFileLog("SOAPGamepotRTA.txt", __FILE__, __LINE__, "HRESULT:%d", hr);

		if( hr == S_OK )
		{
			if( returnval.vt == VT_BOOL )
			{
				if( returnval.boolVal == VARIANT_TRUE )
				{
					bRet = true;
				}
				else
				{
				}
			}
		}
	}

	if(pSoapClient)				pSoapClient.Release();
	if(dispparams.rgvarg)		delete[] dispparams.rgvarg;

	return bRet;
}

BOOL HTTPHangameRTA(int Count)	// For HangameJP
{
	BOOL bResult = FALSE;

	HINTERNET	hSession = NULL;
	hSession = ::InternetOpen("HTTPHangameRTA", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);

	char szPostData[128] = { '\0', };
	char szObject[1024] = { '\0', };
	BOOL bRet = FALSE;

	//sprintf(szObject, "publish/common/rta.asp?gameid=pubkunshu&cnt=%d", Count);
	sprintf(szObject, "rta.nhn?cnt=%d", Count);
	
	if(hSession)
	{
		HINTERNET hConnect = ::InternetConnect(hSession,
			//"ssl.hangame.co.jp",
			"kunshu.hangame.co.jp",
			INTERNET_DEFAULT_HTTP_PORT,
			"",
			"",
			INTERNET_SERVICE_HTTP,
			0,
			0);

		if(hConnect)
		{
			// connected server..
			HINTERNET hObject = ::HttpOpenRequest( hConnect,
				"GET",
				szObject,
				HTTP_VERSION,
				"",
				NULL,
				NULL,
				0);

			if(hObject)
			{
				// post header
				char szLen[MAX_PATH] = { '\0', };
				char aHeader[1024] = { '\0', };

				sprintf(szLen, "%d", strlen(szPostData));
				strcat(aHeader, "Accept: text/*\r\n");
				strcat(aHeader, "User-Agent: Mozilla/4.0 (compatible; MSIE 5.0;* Windows NT)\r\n");
				strcat(aHeader, "Content-type: application/x-www-form-urlencoded\r\n");
				strcat(aHeader, "Content-length: ");
				strcat(aHeader, szLen);
				strcat(aHeader, "\r\n\n");

				bRet = ::HttpAddRequestHeaders(hObject,(LPCTSTR)aHeader,-1L,HTTP_ADDREQ_FLAG_ADD);
				bRet = ::HttpSendRequest(hObject, NULL, 0, (LPVOID)NULL, (DWORD)0);

				// open object
				char szBuffer[1024] = { '\0', };
				DWORD dwRead = 0;
				DWORD dwTotalRead = 0;
				char szResult[2048] = { '\0', };

				g_CLogPrint.WriteFileLog("HTTPHangameRTA.txt", __FILE__, __LINE__, "Result:%d", bRet);

				while (::InternetReadFile(hObject, szBuffer, 1023, &dwRead) && dwRead > 0)
				{
					szBuffer[dwRead] = 0;
					dwTotalRead += dwRead;
					if( strlen(szResult) + dwRead >= sizeof(szResult) )
						break;
					strcat(szResult, szBuffer);
					Sleep(1);
				}

				::InternetCloseHandle(hObject);
			}

			::InternetCloseHandle(hConnect);
		}
		::InternetCloseHandle(hSession);
	}

	return bResult;
}


BOOL HTTPNexonRTA(int Count)	// For HangameJP
{
	BOOL bResult = FALSE;

	HINTERNET	hSession = NULL;
	hSession = ::InternetOpen("HTTPNexonRTA", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);

	char szPostData[1024] = { '\0', };
	char szTimeDate[128]  = { '\0', };

	int bRet1 = -1;
	int bRet2 = -1;

	SYSTEMTIME	curTime;
	GetLocalTime(&curTime);
	//sprintf(szTimeDate, "request_time=\"%04d-%02d-%02d %02d:%02d:%02d\"",curTime.wYear,curTime.wMonth, curTime.wDay, curTime.wHour, curTime.wMinute, curTime.wSecond);
	sprintf(szTimeDate, "request_time=%04d-%02d-%02d+%02d:%02d:%02d",curTime.wYear,curTime.wMonth, curTime.wDay, curTime.wHour, curTime.wMinute, curTime.wSecond);

	sprintf(szPostData, "game_name=kunshu&%s&cnt=%d",szTimeDate, Count);

	if(hSession)
	{
		HINTERNET hConnect = ::InternetConnect(hSession,
			"webservice.nexon.co.jp",
			INTERNET_DEFAULT_HTTP_PORT,
			"",
			"",
			INTERNET_SERVICE_HTTP,
			0,
			0);

		if(hConnect)
		{
			// connected server..
			HINTERNET hObject = ::HttpOpenRequest( hConnect,
				"POST",
				"/channeling/cnt/getChannelingCnt.asp",
				HTTP_VERSION,
				"",
				NULL,
				NULL,
				0);

			if(hObject)
			{
				// post header
				char szLen[MAX_PATH] = { '\0', };
				char aHeader[1024] = { '\0', };

				sprintf(szLen, "%d", strlen(szPostData));
				strcat(aHeader, "Accept: text/*\r\n");
				strcat(aHeader, "User-Agent: Mozilla/4.0 (compatible; MSIE 5.0;* Windows NT)\r\n");
				strcat(aHeader, "Content-type: application/x-www-form-urlencoded\r\n");
				strcat(aHeader, "Content-length: ");
				strcat(aHeader, szLen);
				strcat(aHeader, "\r\n\n");

				bRet1 = ::HttpAddRequestHeaders(hObject,(LPCTSTR)aHeader,-1L,HTTP_ADDREQ_FLAG_ADD);
				bRet2 = ::HttpSendRequest(hObject, NULL, 0, (LPVOID)szPostData, (DWORD)strlen(szPostData));

				DWORD dwErr = GetLastError();

				// open object
				char szBuffer[1024] = { '\0', };
				DWORD dwRead = 0;
				char szResult[2048] = { '\0', };

				g_CLogPrint.WriteFileLog("HTTPNexonRTA.txt", __FILE__, __LINE__, "Result= %d : %d", bRet1, bRet2);

				while (::InternetReadFile(hObject, szBuffer, 1023, &dwRead) && dwRead > 0)
				{
					szBuffer[dwRead] = 0;
					strcat(szResult, szBuffer);
					Sleep(1);
				}

				::InternetCloseHandle(hObject);
			}

			::InternetCloseHandle(hConnect);
		}
		::InternetCloseHandle(hSession);
	}

	return bResult;
}

