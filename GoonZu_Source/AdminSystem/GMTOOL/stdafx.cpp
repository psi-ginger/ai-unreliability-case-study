// stdafx.cpp : 표준 포함 파일을 포함하는 소스 파일입니다.
// GMTOOL.pch는 미리 컴파일된 헤더가 됩니다.
// stdafx.obj는 미리 컴파일된 형식 정보를 포함합니다.

#include "stdafx.h"

int _UTF8ToAnsi( const char* szUTF8, char* szAnsi, int iMaxBufSize )
{
	wchar_t buf[ 8000 ];

	MultiByteToWideChar( CP_UTF8, 0, szUTF8, -1, buf, 8000 );
	return WideCharToMultiByte( CP_ACP, 0, buf, -1, szAnsi, iMaxBufSize, NULL, NULL ) - 1;
}

int _AnsiToUTF8( const char* szAnsi, char* szUTF8, int iMaxBufSize )
{
	wchar_t buf[ 8000 ];

	MultiByteToWideChar( CP_ACP, 0, szAnsi, -1, buf, 8000 );
	return WideCharToMultiByte( CP_UTF8, 0, buf, -1, szUTF8, iMaxBufSize, NULL, NULL ) - 1;
}

