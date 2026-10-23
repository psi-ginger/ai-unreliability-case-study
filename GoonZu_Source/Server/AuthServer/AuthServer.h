#ifndef _AUTHSERVER_H
#define _AUTHSERVER_H

#include "AuthMsg.h"
#include "ITZPerson.h"

#include "../../DBManager/AccountDBManager/AccountDBMsg.h"

#define MAX_SBSLOGIN_NUMBER				9

class AuthUserList;
class AccountDBProcess;
class AuthGameProtocol;
class AuthFrontProtocol;
class AgentProcess;
class CGash;
class CUserManager;

//extern AuthUserList			g_authUserList;
extern AccountDBProcess		g_accountDB;
extern AuthGameProtocol		g_agp;
extern AuthFrontProtocol	g_afp;
extern AgentProcess			g_agent;
extern CGash				g_gash;

extern BOOL					g_bInitReady;						// 초기화 준비가 됨 
extern BOOL					g_bInitComplete;					// 초기화 완료 됨 

extern SI32					g_siServerID;

extern CUserManager			g_usermanager;

// 한게임 전용 서비스 코드
extern SI32					g_nServiceCode;

class cFront : public ITZPerson
{
public:
	cFront() {};
	~cFront() {};

public:
	virtual void Init() { printf( "Front Server Connected\n" ); };
	virtual void Close() { printf( "Front Server Disconnected\n" ); };

};


class cGame : public ITZPerson
{
public:
	cGame() {};
	~cGame() {};

public:
	virtual void Init() { printf( "Game Server Connected\n" ); };
	virtual void Close() { printf( "Game Server Disconnected\n" ); };
};


class cUserInfo : public ITZObject
{
public:
	cUserInfo() {};
	~cUserInfo() {};

public:
	char			userid[ 20 ];
	char			userpw[ 20 ];

	SI32			idnum;
	UI16			usAuthKey;					// Auth서버에서 user리스트에서의 인덱스 
	SI16			siGameMasterLevel;			// 운영자 등급

	DWORD			dwLastTick;					// 최근 등록 시각
	
};
//--------------------------------------------------------------------------------------------------
// MINI DUMP - by LEEKH 2007.11.15 - 아틀란티카팀에서 가르쳐준 덤프 생성
//--------------------------------------------------------------------------------------------------
#if defined (USE_MINIDUMP)

#include "dbghelp.h"

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

