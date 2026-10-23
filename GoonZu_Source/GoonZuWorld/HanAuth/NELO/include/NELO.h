#ifndef _NELO_H_20071112_
#define _NELO_H_20071112_

#ifdef NELO_EXPORTS
	#define NELO_API __declspec(dllexport)
#else
	#define NELO_API __declspec(dllimport)
#endif

#define NELO_API_CALL __stdcall

// ERROR CODE
#define NELO_NOERROR								0
#define NELO_E_ALREADY_INITIALIZED					100
#define NELO_E_NOT_INITIALIZED						101
#define NELO_E_ALREADY_STARTED						102
#define NELO_E_INVALID_VERSION_STRING				103
#define NELO_E_REPORT_SERVER_NOT_INITIALIZED		104
#define NELO_E_INVALID_FIELD						105
#define NELO_E_REPORTFAIL_BY_REPLYFAIL				106
#define NELO_E_REPORTFAIL_BY_CONNECTSERVER			107
#define NELO_E_NOT_SET_USERID						108
#define NELO_E_FAIL_TO_LOADFILE						109
#define NELO_E_FAIL_GET_PATH						110

// LOG LEVEL
typedef enum _NELO_LOG_LEVEL
{
	NELO_LL_FATAL	= 0,	
	NELO_LL_ERROR	= 3,
	NELO_LL_WARN	= 4,	
	NELO_LL_INFO	= 5,	
	NELO_LL_DEBUG	= 7

} NELO_LOG_LEVEL;

// NELO HANDLE
typedef struct _NELO_Handle
{
} *NELO_HANDLE;


// Error Code
NELO_API int NELO_API_CALL NELO_GetLastError();

// Handle Create/Destroy
NELO_API NELO_HANDLE NELO_API_CALL NELO_Create( const char *szappReportID, const char *szAppVersion, const char *szAppTitleName );
NELO_API NELO_HANDLE NELO_API_CALL NELO_Create( const wchar_t *wszappReportID, const wchar_t *wszAppVersion, const wchar_t *wszAppTitleName );
NELO_API bool NELO_API_CALL NELO_Destroy( NELO_HANDLE hHandle );

// Report Server Get/Set
NELO_API bool NELO_API_CALL NELO_SetReportServer( NELO_HANDLE hHandle, const char *szServerAddr, unsigned short usServerPort );
NELO_API bool NELO_API_CALL NELO_SetReportServer( NELO_HANDLE hHandle, const wchar_t *wszServerAddr, unsigned short usServerPort );
NELO_API bool NELO_API_CALL NELO_GetReportServer( NELO_HANDLE hHandle, unsigned short *pOutPort, char szServerAddrOutBuffer[], int nOutBufferSize );
NELO_API bool NELO_API_CALL NELO_GetReportServer( NELO_HANDLE hHandle, unsigned short *pOutPort, wchar_t wszServerAddrOutBuffer[], int nOutBufferSize );

// User ID Get/Set
NELO_API bool NELO_API_CALL NELO_SetUserID( NELO_HANDLE hHandle, const char *szUserID );
NELO_API bool NELO_API_CALL NELO_SetUserID( NELO_HANDLE hHandle, const wchar_t *wszUserID );
NELO_API bool NELO_API_CALL NELO_GetUserID( NELO_HANDLE hHandle, char *szUserIdOutBuffer, size_t nOutBufferSize );
NELO_API bool NELO_API_CALL NELO_GetUserID( NELO_HANDLE hHandle, wchar_t *wszUserIdOutBuffer, size_t nOutBufferSize );

// Report Log
NELO_API bool NELO_API_CALL NELO_SendLog_TCP( NELO_HANDLE hHandle, NELO_LOG_LEVEL logLevel, const char *szLogCode, const char *szMessage, const char *szLogLocationInfo );
NELO_API bool NELO_API_CALL NELO_SendLog_TCP( NELO_HANDLE hHandle, NELO_LOG_LEVEL logLevel, const wchar_t *wszLogCode, const wchar_t *wszMessage, const wchar_t *wszLogLocationInfo );
NELO_API bool NELO_API_CALL NELO_SendLog_UDP( NELO_HANDLE hHandle, NELO_LOG_LEVEL logLevel, const char *szLogCode, const char *szMessage, const char *szLogLocationInfo );
NELO_API bool NELO_API_CALL NELO_SendLog_UDP( NELO_HANDLE hHandle, NELO_LOG_LEVEL logLevel, const wchar_t *wszLogCode, const wchar_t *wszMessage, const wchar_t *wszLogLocationInfo );
NELO_API bool NELO_API_CALL NELO_SetTimeout( NELO_HANDLE hHandle, int nTimeout );

// Crash Handling
NELO_API bool NELO_API_CALL NELO_StartCrashHandler( NELO_HANDLE hHandle );
NELO_API bool NELO_API_CALL NELO_StopCrashHandler( NELO_HANDLE hHandle );
NELO_API bool NELO_API_CALL NELO_DeleteLocalDumpFile( NELO_HANDLE hHandle, bool bDeleteDumpFile );
NELO_API bool NELO_API_CALL NELO_SetAutoReport( NELO_HANDLE hHandle, bool bAutoReport );
NELO_API bool NELO_API_CALL NELO_SetAppRestart( NELO_HANDLE hHandle, bool bAppRestart, const char * szExecuteCmd );
NELO_API bool NELO_API_CALL NELO_SetAppRestart( NELO_HANDLE hHandle, bool bAppRestart, const wchar_t *wszExecuteCmd );

//-------------
// Support Old API Set

#if defined( _NELO_1_0_API ) 

NELO_API bool NELO_API_CALL NELO_Init(const char * szAppID, const char * szAppVer, const char * szAppShowName);
NELO_API void NELO_API_CALL NELO_Uninit();
NELO_API bool NELO_API_CALL NELO_SetUserID(const char * szUserID);
NELO_API const char *NELO_API_CALL NELO_GetUserID();
NELO_API bool NELO_API_CALL NELO_SetReportServer(const char * szServerIP, unsigned short usServerPort);
NELO_API bool NELO_API_CALL NELO_GetReportServer(unsigned short * pPort, char szServerIP[], int nMax);
NELO_API bool NELO_API_CALL NELO_SendLog_TCP(NELO_LOG_LEVEL eLevel, const char * szErrorCode, const char * szMsg, const char * szErrorLocationInfo);
NELO_API bool NELO_API_CALL NELO_SendLog_UDP(NELO_LOG_LEVEL eLevel, const char * szErrorCode, const char * szMsg, const char * szErrorLocationInfo);
NELO_API bool NELO_API_CALL NELO_SendCrashInfo(const char * szExceptionAddr, const char * szComment, const char * szExceptionInfoFilePath, const char * szDumpFilePath );
NELO_API bool NELO_API_CALL NELO_StartCrashHandler();
NELO_API bool NELO_API_CALL NELO_StopCrashHandler();
NELO_API bool NELO_API_CALL NELO_DeleteLocalDumpFile(bool bSet);
NELO_API bool NELO_API_CALL NELO_SetAutoReport(bool bSet);
NELO_API bool NELO_API_CALL NELO_SetAppRestart(bool bSet, const char * szExecuteCmd);

#endif

#endif
