#ifndef _CLTSTATISTICS_H_
#define _CLTSTATISTICS_H_

#include "../NLib/NLua.h"

#define STATISTICS_ROOT		"NStatistics"

class cltStatistics_Lib
{
private:
	NLua*	m_pclLua;

public:
	cltStatistics_Lib( NLua* pclLua );
	virtual ~cltStatistics_Lib();

	bool Read(const char* pszFilename);
	bool Write(const char* pszFileName, SYSTEMTIME* psTime = NULL );
	bool AppendTable_Hour(const char* pszTable, SYSTEMTIME* psTime );
	bool Clear();

	bool AddValue(const char *pszTable, const char* pszRow, const char* pszCol, INT64 siAddVal );
	bool SetValue(const char *pszTable, const char* pszRow, const char* pszCol, INT64 siValue );
	bool SetValue(const char *pszTable, const char* pszRow, const char* pszCol, const char* pszVal );

protected:
	bool AutoGetTableField( const char* pszField );
};

#endif