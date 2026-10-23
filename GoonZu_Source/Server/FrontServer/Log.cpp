#include "Log.h"

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com)
//	Desc : 생 성 자
//	Date : 2002. 11. 20
//	Update : 
//--------------------------------------------------------------------------------------------
CLogPrint::CLogPrint()
{
	InitializeCriticalSection(&m_LogCrit);
	//::DeleteFile(TEXT("./Log.txt"));
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com)
//	Desc : 소 멸 자
//	Date : 2002. 11. 20
//	Update : 
//--------------------------------------------------------------------------------------------
CLogPrint::~CLogPrint()
{
	DeleteCriticalSection(&m_LogCrit);
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com)
//	Desc : 화면 출력
//	Date : 2002. 11. 20
//	Update : 2003. 06. 09 - printf 에서 OutputDebugString 으로 함수 변경 했음
//--------------------------------------------------------------------------------------------
void CLogPrint::LogPrint(TCHAR *format,...)
{
//#ifdef _DEBUG	

	EnterCriticalSection(&m_LogCrit);

	va_list vl;
	TCHAR    dbgbuf[2048];

	va_start(vl, format);
	StringCchVPrintf(dbgbuf, 2048, format, vl);
	va_end(vl);

	OutputDebugString(dbgbuf);
	//printf(dbgbuf);

	LeaveCriticalSection(&m_LogCrit);

//#endif
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com)
//	Desc : 파일 출력
//	Date : 2002. 11. 20
//	Update : 
//--------------------------------------------------------------------------------------------
void CLogPrint::FilePrint( TCHAR* filename, TCHAR *format,... )
{
	EnterCriticalSection(&m_LogCrit);

	va_list vl;    

	va_start( vl, format );
	StringCchVPrintf( dbgbuf, 2048, format, vl );
	va_end( vl );

	//OutputDebugString(dbgbuf);

	FILE *fp = NULL;

	//fp = _tfopen(TEXT("./Log.txt"), TEXT("a+"));
	fp = _tfopen( filename, TEXT("a+") );

	if ( ! fp )
	{
		//printf(TEXT("File : Log.txt Make Error\n"));
		//StringCchPrintf()

		LeaveCriticalSection(&m_LogCrit);

		return;
	}



	_tstrdate( m_Date );
	//printf( TEXT("Date : %s"), tmpbuf );

	_tstrtime( m_Time );
	//printf( TEXT("Time : %s"), tmpbuf );

	_ftprintf( fp, TEXT("%s \t %s \t %s\n"), dbgbuf, m_Date, m_Time );

	fclose(fp);

	LeaveCriticalSection(&m_LogCrit);
}

//--------------------------------------------------------------------------------------------
//	Name : 김광명
//	Desc : 파일 출력
//	Date : 2007. 03. 28
//	Update : 
//--------------------------------------------------------------------------------------------
void CLogPrint::FileConfigPrint( TCHAR* filename, TCHAR *format,... )
{
	EnterCriticalSection(&m_LogCrit);

	va_list vl;    

	va_start( vl, format );
	StringCchVPrintf( dbgbuf, 2048, format, vl );
	va_end( vl );

	SYSTEMTIME sysTime;
	GetLocalTime( &sysTime );
	char szDirectoryName[ MAX_PATH ] = {'\0'};
	// 년도
	_stprintf( szDirectoryName, "C:\\GoonZuWorld\\Goonzu_China\\Config\\%dYear", sysTime.wYear );
	CreateDirectory( szDirectoryName, NULL );

	// 년도 / 달 
	_stprintf( szDirectoryName, "C:\\GoonZuWorld\\Goonzu_China\\Config\\%dYear\\%dMonth", sysTime.wYear, sysTime.wMonth );
	CreateDirectory( szDirectoryName, NULL );

	// 년도 / 달 / 이름
	_stprintf( szDirectoryName, "C:\\GoonZuWorld\\Goonzu_China\\Config\\%dYear\\%dMonth\\%s", sysTime.wYear, sysTime.wMonth, filename );
	CreateDirectory( szDirectoryName, NULL );

	char szFileName[ MAX_PATH ] = {'\0'};
	if( 10 > sysTime.wMonth && 10 > sysTime.wDay )
		_stprintf( szFileName, "%s\\%s%d%0d%0d.txt", szDirectoryName, filename, sysTime.wYear, sysTime.wMonth, sysTime.wDay );
	else if ( 10 > sysTime.wDay )
	{
		_stprintf( szFileName, "%s\\%s%d%d%0d.txt", szDirectoryName, filename, sysTime.wYear, sysTime.wMonth, sysTime.wDay );
	}
	else
		_stprintf( szFileName, "%s\\%s%d%d%d.txt", szDirectoryName, filename, sysTime.wYear, sysTime.wMonth, sysTime.wDay );

	FILE *fp = _tfopen( szFileName, TEXT("a+") );

	if ( ! fp )
	{
		LeaveCriticalSection(&m_LogCrit);
		return;
	}

	_tstrdate( m_Date );
	//printf( TEXT("Date : %s"), tmpbuf );

	_tstrtime( m_Time );
	//printf( TEXT("Time : %s"), tmpbuf );

	_ftprintf( fp, TEXT("%s \t %s \t %s\n"), dbgbuf, m_Date, m_Time );

	fclose(fp);

	LeaveCriticalSection(&m_LogCrit);

}

//--------------------------------------------------------------------------------------------
//	Name : 이강형
//	Desc : 로그 출력
//	Date : 2007. 06. 15
//	Update : 
//--------------------------------------------------------------------------------------------
void CLogPrint::WriteFileLog( TCHAR* pszFileName, TCHAR* pszSource, int nLine, TCHAR* pszLogFormat, ... )
{
	if(pszFileName == NULL)		return;
	if(pszSource == NULL)		return;
	if(pszLogFormat == NULL)	return;

	EnterCriticalSection(&m_LogCrit);

	// 날짜 문자열
	TCHAR	szDate[32];
	SYSTEMTIME systime;
	GetLocalTime(&systime);
	sprintf(szDate, "%04d-%02d-%02d", systime.wYear, systime.wMonth, systime.wDay);

	// 파일명 뒤에 날짜를 붙여서 로그를 적음
	TCHAR	szFileName[MAX_PATH * 2];
	TCHAR	szExt[MAX_PATH]; 
	memset(szFileName,	0, sizeof(szFileName));
	memset(szExt,		0, sizeof(szExt));

	StringCchCopy(szFileName, sizeof(szFileName), pszFileName);

	TCHAR*	pExt = _tcsrchr(szFileName, '.');
	if(pExt)
	{
		StringCchCopy(szExt, sizeof(szExt), pExt);		
		szFileName[pExt - szFileName] = '\0';
		StringCchCat(szFileName, sizeof(szFileName), "_");
		StringCchCat(szFileName, sizeof(szFileName), szDate);
		StringCchCat(szFileName, sizeof(szFileName), szExt);
	}
	else
	{
		StringCchCat(szFileName, sizeof(szFileName), "_");
		StringCchCat(szFileName, sizeof(szFileName), szDate);
	}

	TCHAR	szLogMsg[1024 * 5];
	szLogMsg[0] = '\0';

	TCHAR	szLine[128];
	sprintf(szLine, "%d", nLine);

	TCHAR	szTime[128];
	sprintf(szTime, "%04d-%02d-%02d %02d:%02d:%02d.%03d",	systime.wYear, systime.wMonth, systime.wDay,
		systime.wHour, systime.wMinute, systime.wSecond, systime.wMilliseconds);
	va_list args;

	va_start( args, pszLogFormat );
	_vsnprintf( szLogMsg, sizeof(szLogMsg) - 10, pszLogFormat, args );
	va_end( args );

	FILE* fp;
	fp = fopen(szFileName, "a");
	if(fp != NULL)
	{
		fprintf(fp, "Time=");	fprintf(fp, szTime);	fprintf(fp, "|");
		fprintf(fp, "Source=");	fprintf(fp, pszSource);	fprintf(fp, "|");
		fprintf(fp, "Line=");	fprintf(fp, szLine);	fprintf(fp, "|");
		fprintf(fp, "LogMsg=");	fprintf(fp, szLogMsg);	fprintf(fp, "|\n");		

		fclose(fp);
	}

	LeaveCriticalSection(&m_LogCrit);

}

//--------------------------------------------------------------------------------------------
//	Name : 이강형
//	Desc : 주어진 파일명앞에 날짜를 붙여준다.
//	Date : 2007. 08. 23
//	Update : 
//--------------------------------------------------------------------------------------------
void CLogPrint::GetDateFileName(TCHAR* pszFileName, TCHAR* pszOutFileName)
{
	if(pszFileName == NULL)			return;
	if(pszOutFileName == NULL)		return;

	SYSTEMTIME systime;
	GetLocalTime(&systime);
	
	sprintf(pszOutFileName, "%04d-%02d-%02d_%s", systime.wYear, systime.wMonth, systime.wDay, pszFileName );
}