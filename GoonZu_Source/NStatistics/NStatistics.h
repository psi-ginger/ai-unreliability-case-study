#ifndef _NSTATISTICS_H_
#define _NSTATISTICS_H_

#include <windows.h>
#include "../NLib/NLua.h"

bool NStatistics_Init( NLua* pclLua );
bool NStatistics_Shutdown();
bool NStatistics_Clear();

bool NStatistics_Read( const char* pszFileName );
bool NStatistics_Write( const char* pszFileName, SYSTEMTIME* psTime = NULL );
bool NStatistics_AppendTable_Hour( const char* pszTable, SYSTEMTIME* psTime );

bool NStatistics_Add( const char* pszTable, const char* pszRow, const char* pszCol, INT64 siAddVal );
bool NStatistics_Add( const char* pszTable, INT64 siRow, const char* pszCol, INT64 siAddVal  );
bool NStatistics_Set( const char* pszTable, const char* pszRow, const char* pszCol, INT64 siVal );
bool NStatistics_Set( const char* pszTable, const char* pszRow, const char* pszCol, const char* pszVal );
bool NStatistics_Set( const char* pszTable, INT64 siRow, const char* pszCol, INT64 siVal );
bool NStatistics_Set( const char* pszTable, INT64 siRow, const char* pszCol, const char* pszVal );

#endif

