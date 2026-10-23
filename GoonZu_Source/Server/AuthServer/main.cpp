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
#include "../FrontServer/Log.h"

#include "cltIOVATION.h"

#include "ITZList.h"

#ifdef USE_HANAUTH
	// - NHN ASM -
	#include "./HanAuth/HanAuthForSvr.h"
	// - NHN ASM -

	// 한게임 전용 서비스 코드
#if defined (_KOREA)
	#define HANAUTH_SERVICECODE_FOR_ALPHA	SERVICE_KOR | SERVICE_ALPHA
	#define HANAUTH_SERVICECODE_FOR_REAL	SERVICE_KOR | SERVICE_REAL
#elif defined (_USA)
	#define HANAUTH_SERVICECODE_FOR_ALPHA	SERVICE_USA | SERVICE_ALPHA
	#define HANAUTH_SERVICECODE_FOR_REAL	SERVICE_USA | SERVICE_REAL
#endif

#endif //USE_HANAUTH

//KHY - 0204 -  중국 PurpleAuth 인증 모듈
#ifdef USE_PORTEAUTH
	#include "./Porte/PorteAuthForSvr.h"	
	using namespace Porte::PorteAuthSystem;

//KHY - 0218 - 중국 NHN 피로도 모듈 적용 - 인증모듈이 적용되어야만 한다.
#ifdef USE_HANANTITOXIC
	#include "./HanAntiToxic/HanAntiToxic.h"	

	TCHAR g_HanSID[MAX_SID_BUF];
	BOOL g_Tired; 
#endif
#endif // USE_PORTEAUTH


void WriteConsole( HANDLE hConsole, int x, int y, char *str, ... );
void WriteConsoleAttr( HANDLE hConsole, int x, int y, int len, WORD attr );

void dispInitView();
void dispMainView();
void dispMainRefresh();

HANDLE g_hStdIn, g_hStdOut;
HANDLE g_hMainView;

#ifdef _IOVATION
	cltIOVATION		g_clioVation;
#endif

AccountDBProcess	g_accountDB;
AuthGameProtocol	g_agp;
AuthFrontProtocol	g_afp;
AgentProcess		g_agent;
CGash				g_gash;
CUserManager		g_usermanager;

BOOL				g_bInitReady;							// 초기화 준비가 됨 
BOOL				g_bInitComplete;						// 초기화 완료 됨 

SI32				g_siServerID;

SI32				g_nServiceCode;

CLogPrint			g_CLogPrint;

BOOL				g_bRun;

char				g_szServiceArea[12];
char				g_szServiceMode[8];

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

				StringCchPrintf(szDumpFileName, sizeof(szDumpFileName), TEXT("C:\\GoonZuDump\\AuthServer_%04d%02d%02d_%02d%02d%02d.dmp"), 
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

		if(		strcmp(key, "ServiceType") == 0)
		{
#ifdef USE_HANAUTH
			if (	strcmp(value, "ALPHA") == 0)		{	g_nServiceCode = HANAUTH_SERVICECODE_FOR_ALPHA;		}
			else if(strcmp(value, "REAL") == 0)			{	g_nServiceCode = HANAUTH_SERVICECODE_FOR_REAL;		}
#endif // USE_HANAUTH
		}
		else if(strcmp(key, "ServiceAge") == 0)		{												}
	}

	fclose(fp);
}

void main()
{
#if defined (USE_MINIDUMP)
	MiniDumper		GoonZuDump;
#endif

	// webservice-
	CoInitialize(NULL);

	// 한게임 서비스 파일 읽기
	GetHanGameInfo();

	dispInitView();

	g_bInitReady = FALSE;
	g_bInitComplete = FALSE;

#ifdef USE_HANAUTH

	// Han Auth 초기화
	#if defined(_KOREA)
		HanAuthInit( "K_GOONZU", g_nServiceCode, 10 );
	#elif defined(_USA)
		HanAuthInit( "U_GOONZU", g_nServiceCode, 10 );
	#endif

#endif //USE_HANAUTH

//KHY - 0204 -  중국 PurpleAuth 인증 모듈 		//서버 - 초기화
#ifdef USE_PORTEAUTH
		SI32	siPorteAuthRet = 0;

		//서버 - 초기화 - 인증처리 쓰레드 갯수  -10 임시.
		//siPorteAuthRet = PubAuthInit("C_GZ", SERVICE_CHN|SERVICE_ALPHA|SERVICE_IDC01 , 10);	<- 변경전
		// 초기화 함수 변경 2009.09.08
		TCHAR* szCustomConfigFileName = NULL	;
		siPorteAuthRet = PubAuthInit("C_GZ", SERVICE_CHN|SERVICE_ALPHA|SERVICE_IDC01 , 10,  szCustomConfigFileName );
		if(siPorteAuthRet != 0)		
		{	
			g_CLogPrint.WriteFileLog("AuthserverProcessLog_PorteAuth.txt", __FILE__, __LINE__, "Ret=%d", siPorteAuthRet);	
		}

//KHY - 0218 - 중국 NHN 피로도 모듈 적용
#ifdef USE_HANANTITOXIC
		SI32	siHanAntiToxicRet = 0;

		siHanAntiToxicRet = HanAntiToxicInit("C_GZ");
		if(siHanAntiToxicRet != 0)		
		{	
			g_CLogPrint.WriteFileLog("AuthserverProcessLog_HanAntiToxicRet.txt", __FILE__, __LINE__, "Ret=%d", siHanAntiToxicRet);	
		}

#endif //USE_HANANTITOXIC
#endif // USE_PORTEAUTH

	// 초기화
	//g_agp.Init(); 
	//g_afp.Init();

	g_agent.Init( "127.0.0.1", 6400 );

	//g_agp.CreateThread();
	//g_afp.CreateThread();

	// GASH
	// 2008년 7월 1일 Ndoors Entertainment 서비스로 이관하면서 인증방식이 변경되어 삭제됨 - by LEEKH 2008.06.28

	//g_gash.Init( 1024, "192.168.6.13" );
	//g_gash.Init( 1024, "127.0.0.1" );
	//g_gash.CreateThread();
	
	// standard console handle
	g_hStdIn = GetStdHandle( STD_INPUT_HANDLE );
	g_hStdOut = GetStdHandle( STD_OUTPUT_HANDLE );

	// create mainview screen
	g_hMainView = CreateConsoleScreenBuffer( GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, 0 );

//	SetConsoleActiveScreenBuffer( g_hMainView );

	dispMainView();


//	BOOL bRun = TRUE;

	g_bRun = TRUE;

	// console
	while( g_bRun ) {

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
				// 2008년 7월 1일 Ndoors Entertainment 서비스로 이관하면서 인증방식이 변경되어 삭제됨 - by LEEKH 2008.06.28
				//g_gash.AddRequestInfo( REQUESTCODE_AUTH, 1, 1, "test", "test", "111.111.111.111", 0, 0 );
				break;
			
			case '2':
				// 2008년 7월 1일 Ndoors Entertainment 서비스로 이관하면서 인증방식이 변경되어 삭제됨 - by LEEKH 2008.06.28
				//g_gash.AddRequestInfo( REQUESTCODE_AUTH, 1, 1, "gamagpkstst", "gamepottest", "111.111.111.111", 0, 0 );
				break;

			case 27:
				printf("- ESC key pressed...\n");
				g_bRun = FALSE;
				break;
			}
		}

	}
//KHY - 0204 -  중국 PurpleAuth 인증 모듈
#ifdef USE_PORTEAUTH
	PubDestroy();
#endif // USE_PORTEAUTH
//KHY - 0218 - 중국 NHN 피로도 모듈 적용
#ifdef USE_HANANTITOXIC
	HanAntiToxicTerm();
#endif //USE_HANANTITOXIC
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
