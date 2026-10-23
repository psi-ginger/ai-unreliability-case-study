//---------------------------------
// 2004/11/27 김태곤
//---------------------------------

#ifndef _MAIN_H
#define _MAIN_H


bool GetGameModeInFile(SI32* pgamemode, SI32* pres, BOOL* bFullScreen, SI32 *siContryCode, SI32* siRunMode );
bool SetGameModeInFile(SI32 gamemode, SI32 res, BOOL bFullScreen, SI32 runmode );
// 게임버젼
SI32 GetGameVersionInFile();
bool SetGameVersionInFile(SI32 siVersion);

bool GetDefaultServiceArea(SI32* servicearea);
// 서비스 지역 코드를 근거로 서비스지역 정보를 얻는다. 
SI32 GetServiceAreaFromCode(TCHAR* sztext);
void GetServerNameFromServerUnique(SI32 serverUnqiue, TCHAR* servername);

// 파일에서 FullScreen 여부를 얻어온다. 
bool GetFullScreenInFile( BOOL* bFullScreen );
bool SetFullScreenInFile( BOOL bFullScreen );

BOOL CheckDupGame1(TCHAR *name, TCHAR* title);
void CloseDupGameHandle1();

// 멀티클라이언트 실행제한 시작
#define	MAX_MULTICLIENT_NUM	2

BOOL CheckMultiClient( void );
// 멀티클라이언트 실행제한 끝
#include "dbghelp.h"

// -USE_NELO
#ifdef USE_NELO
	#ifdef _CLIENT
		#include "../HanAuth/NELO/include/NELO.h"
		#ifdef _DEBUG
			#pragma comment(lib, "HanAuth\\NELO\\lib\\NELOD.lib") 
		#else
			#pragma comment(lib, "HanAuth\\NELO\\lib\\NELO.lib") 
		#endif
	#endif
#endif
// -USE_NELO

//--------------------------------------------------------------------------------------------------
// MINI DUMP - by LEEKH 2007.11.15 - 아틀란티카팀에서 가르쳐준 덤프 생성
//--------------------------------------------------------------------------------------------------
#if defined (USE_MINIDUMP)
// based on dbghelp.h
typedef BOOL (WINAPI *MINIDUMPWRITEDUMP)( HANDLE hProcess, DWORD dwPid, HANDLE hFile, MINIDUMP_TYPE DumpType,
										 CONST PMINIDUMP_EXCEPTION_INFORMATION ExceptionParam,
										 CONST PMINIDUMP_USER_STREAM_INFORMATION UserStreamParam,
										 CONST PMINIDUMP_CALLBACK_INFORMATION CallbackParam );

class MiniDumper
{
public:
	MiniDumper();
	virtual ~MiniDumper(void);

	static void SetHwnd( HWND hwnd )
	{
		m_hWnd = hwnd;
	}

private:
	static LPCSTR m_szAppName;
	static HWND	  m_hWnd;

	static LONG WINAPI TopLevelFilter( struct _EXCEPTION_POINTERS* exceptioninfo );

};

#endif
//--------------------------------------------------------------------------------------------------


#endif
