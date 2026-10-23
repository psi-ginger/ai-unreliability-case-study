#include "NStatistics.h"
#include "cltStatistics.h"

#include <process.h>

cltStatistics_Lib* g_pclStatistics = NULL;
bool bInit;
HANDLE g_hThread;
CRITICAL_SECTION g_cs;

class cltThreadParam
{
public:
	TCHAR		m_szFileName[MAX_PATH];
	SYSTEMTIME	m_stTime;

	cltThreadParam()
	{
		ZeroMemory(m_szFileName, sizeof(m_szFileName));
		ZeroMemory(&m_stTime, sizeof(m_stTime));
	}

	void Set(const char* pszFileName, SYSTEMTIME* psTime)
	{
		strncpy(m_szFileName, pszFileName, sizeof(m_szFileName));
		memcpy(&m_stTime, psTime, sizeof(m_stTime));
	}
};

bool NStatistics_Init( NLua* pclLua )
{
	g_pclStatistics = new cltStatistics_Lib( pclLua );
	if( g_pclStatistics == NULL )		return false;

	g_hThread = NULL;
	bInit = true;

	InitializeCriticalSection( &g_cs );

	return true;
}

bool NStatistics_Shutdown()
{
	if( g_hThread )
	{
		WaitForSingleObject(g_hThread, 1000 );
		g_hThread = NULL;
	}

	bInit = false;

	DeleteCriticalSection( &g_cs );

	return true;
}

bool NStatistics_Read( const char* pszFileName )
{
	EnterCriticalSection(&g_cs);
	{
		g_pclStatistics->Read( pszFileName );
	}
	LeaveCriticalSection(&g_cs);

	return true;
}

void write_thread_func( void* pParam )
{
	if( bInit == false )			return;

	cltThreadParam* pclThreadParam = (cltThreadParam*)pParam;
	if(pclThreadParam == NULL)			return;

	EnterCriticalSection(&g_cs);
	{
		g_pclStatistics->Write( pclThreadParam->m_szFileName, &pclThreadParam->m_stTime );
	}
	LeaveCriticalSection(&g_cs);

	delete pclThreadParam;
	g_hThread = NULL;
}

void append_thread_func( void* pParam )
{
	if( bInit == false )			return;

	cltThreadParam* pclThreadParam = (cltThreadParam*)pParam;
	if(pclThreadParam == NULL)			return;

	EnterCriticalSection(&g_cs);
	{
		g_pclStatistics->AppendTable_Hour( pclThreadParam->m_szFileName, &pclThreadParam->m_stTime );
	}
	LeaveCriticalSection(&g_cs);

	delete pclThreadParam;
	g_hThread = NULL;
}


bool NStatistics_Write( const char* pszFileName, SYSTEMTIME* psTime /* = NULL */ )
{
	if( bInit == false )			return false;
	if( g_hThread != NULL )			return false;

	cltThreadParam* pclThreadParam = new cltThreadParam;
	if( pclThreadParam == NULL )		return false;

	pclThreadParam->Set( pszFileName, psTime );

	g_hThread = (HANDLE)_beginthread(write_thread_func, 0, pclThreadParam);

//	g_pclStatistics->Write( pszFileName, psTime );

	return true;
}

bool NStatistics_AppendTable_Hour( const char* pszTable, SYSTEMTIME* psTime )
{
	if( bInit == false )			return false;
	if( g_hThread != NULL )			return false;

	cltThreadParam* pclThreadParam = new cltThreadParam;
	if( pclThreadParam == NULL )		return false;

	pclThreadParam->Set( pszTable, psTime );

	g_hThread = (HANDLE)_beginthread(append_thread_func, 0, pclThreadParam);

	return true;
}

bool NStatistics_Clear( )
{
	EnterCriticalSection(&g_cs);
	{
		g_pclStatistics->Clear( );
	}
	LeaveCriticalSection(&g_cs);

	return true;
}

bool NStatistics_Add( const char* pszTable, const char* pszRow, const char* pszCol, INT64 siAddVal )
{
	bool bRet = false;
	EnterCriticalSection(&g_cs);
	{
		bRet = g_pclStatistics->AddValue( pszTable, pszRow, pszCol, siAddVal );
	}
	LeaveCriticalSection(&g_cs);

	return bRet;
}

bool NStatistics_Add( const char* pszTable, INT64 siRow, const char* pszCol, INT64 siAddVal )
{
	TCHAR	szRow[256];
	sprintf(szRow, "%I64d", siRow);

	bool bRet = false;
	EnterCriticalSection(&g_cs);
	{
		bRet = g_pclStatistics->AddValue( pszTable, szRow, pszCol, siAddVal );
	}
	LeaveCriticalSection(&g_cs);

	return bRet;
}

bool NStatistics_Set( const char* pszTable, const char* pszRow, const char* pszCol, INT64 siVal )
{
	bool bRet = false;
	EnterCriticalSection(&g_cs);
	{
		bRet = g_pclStatistics->SetValue( pszTable, pszRow, pszCol, siVal );
	}
	LeaveCriticalSection(&g_cs);

	return bRet;
}

bool NStatistics_Set( const char* pszTable, const char* pszRow, const char* pszCol, const char* pszVal )
{
	bool bRet = false;
	EnterCriticalSection(&g_cs);
	{
		bRet = g_pclStatistics->SetValue( pszTable, pszRow, pszCol, pszVal );
	}
	LeaveCriticalSection(&g_cs);

	return bRet;
}

bool NStatistics_Set( const char* pszTable, INT64 siRow, const char* pszCol, INT64 siVal )
{
	TCHAR	szRow[256];
	sprintf(szRow, "%I64d", siRow);

	bool bRet = false;
	EnterCriticalSection(&g_cs);
	{
		bRet = g_pclStatistics->SetValue( pszTable, szRow, pszCol, siVal );
	}
	LeaveCriticalSection(&g_cs);

	return bRet;
}

bool NStatistics_Set( const char* pszTable, INT64 siRow, const char* pszCol, const char* pszVal )
{
	TCHAR	szRow[256];
	sprintf(szRow, "%I64d", siRow);

	bool bRet = false;
	EnterCriticalSection(&g_cs);
	{
		bRet = g_pclStatistics->SetValue( pszTable, szRow, pszCol, pszVal );
	}
	LeaveCriticalSection(&g_cs);

	return bRet;
}
