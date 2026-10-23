#include "../Client/AutoMacroMgr/clAutoMacroManager.h"
#include "TlHelp32.h"
//#include "Psapi.h"
//#pragma comment(lib, "psapi.lib")


CAutoMacroManager::CAutoMacroManager(void)
{
	ZeroMemory(this, sizeof(CAutoMacroManager));

	NEW_PROCESS_NAME(m_pszProcessName[0], "OK군주SPlus.exe");	
}

CAutoMacroManager::~CAutoMacroManager(void)
{
	for(SI32 i = 0; i < MAX_PROCESS_NAME_COUNT; ++i)
	{
		SAFE_DELETE_ARRAY(m_pszProcessName[i])
	}
}

void CAutoMacroManager::Init(void)
{
	
}

void CAutoMacroManager::ShutDown(void)
{
	for(SI32 i = 0; i < MAX_PROCESS_NAME_COUNT; ++i)
	{
		SAFE_DELETE_ARRAY(m_pszProcessName[i])
	}

	// 클라이언트 종료.
	exit(1);
}


bool CAutoMacroManager::IsCheckMacro(void)
{
	// 호환성을 위해 이부분 변경
	/* 
	DWORD thisProcess	= GetCurrentProcessId();
	HANDLE hSnapshot	= ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
	
	PROCESSENTRY32	pe32;
	pe32.dwSize		= sizeof(PROCESSENTRY32);
	BOOL bContiue	= Process32First(hSnapshot,&pe32);

	DWORD		arProcess[1024], cb;
	SI32		siProcessCnt;
	HANDLE		hProcess;
	HMODULE		hModule;
	TCHAR		szProcessName[256];

	// 프로세스 목록을 배열에 구하고 개수를 개산.
	::EnumProcesses(arProcess, sizeof(arProcess), &cb);
	siProcessCnt = cb/sizeof(cb);

	for(SI32 i = 0; i < MAX_PROCESS_NAME_COUNT; ++i)
	{
		//// 찾는 프로세스의 이름이 순서대로 들어간다는 조건하에 NULL 이 나오면 더이상 검사를 하지 않는다.
		if(NULL == m_pszProcessName[i])
			break;
		
		for(SI32 j = 0; j < siProcessCnt; ++j)
		{
			hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, arProcess[j]);
			if(hProcess)
			{
				if(EnumProcessModules(hProcess, &hModule, sizeof(hModule), &cb))	
				{
					GetModuleBaseName(hProcess, hModule, szProcessName, sizeof(szProcessName));
					if( (_tcscmp(szProcessName, m_pszProcessName[i]) == 0 ))
					{
						CloseHandle(hSnapshot);
						return true;
					}
				}
			} 
		}
		

		CloseHandle(hSnapshot);
	}

	return false;
	*/

	/////////////////////////////////////////////////

	HANDLE	hSnap = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0 );
	if ( INVALID_HANDLE_VALUE == hSnap )
	{
		return false;
	}

	PROCESSENTRY32	peEntry;	// 모듈정보를 받아올 구조체
	peEntry.dwSize = sizeof(PROCESSENTRY32);

	BOOL			bContinue = false;

	for ( SI32 siCount=0; siCount<MAX_PROCESS_NAME_COUNT; siCount++ )
	{
		if( NULL == m_pszProcessName[siCount] )
		{
			break;
		}

		bContinue = Process32First( hSnap, &peEntry );

		while ( bContinue )
		{
			// 인수의 모듈이름과 같은지
			if ( _tcscmp(peEntry.szExeFile, m_pszProcessName[siCount]) == 0 )
			{
				CloseHandle( hSnap );
				return true;
			}

			bContinue = Process32Next( hSnap, &peEntry );
		}
	}

	CloseHandle( hSnap );

	return false;

}


bool CAutoMacroManager::Undate(void)
{
	// 오토프로그램을 사용한다면 처리.
	if(true == IsCheckMacro())
	{
		return true;	
	}
	
	return false;
}
