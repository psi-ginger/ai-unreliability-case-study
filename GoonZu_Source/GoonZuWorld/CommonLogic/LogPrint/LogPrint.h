#ifndef _LOGPRINT_H_
#define _LOGPRINT_H_

#include <Directives.h>
#include <stdio.h>
#include <time.h>
#include <sys/types.h>
#include <sys/timeb.h>

class CLogPrint  
{
public:
	CLogPrint();												// 생 성 자
	virtual ~CLogPrint();										// 소 멸 자

	void LogPrint( TCHAR *format, ... );							// 화면 출력
	void FilePrint( TCHAR* filename, TCHAR *format,... );			// 파일 출력

	// Config폴더에 년도, 달로 구분된 로그를 만든다. filename이 폴더 이름이 된다.
	void FileConfigPrint( TCHAR* filename, TCHAR *format,... );

private:
	CRITICAL_SECTION	m_LogCrit;								// 로그 출력시 엉키는 것을 방지 하기 위해서

	TCHAR				dbgbuf[2048];							// 로그

	TCHAR				m_Time[ 128 ];							// 시간
	TCHAR				m_Date[ 128 ];							// 시간

	
};

#endif