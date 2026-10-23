#include "cltStatistics.h"

char g_SaveStatistics[] = "\
function table_serialize(filename, t, name)\n\
	if f == nil then return end\n\
    if t == nil then return end\n\
\n\
	f:write(name)\n\
    f:write(\" = \")\n\
\n\
    serialize(f, t, \"\")\n\
	f:write( \"\\n\" )\n\
end\n\
\n\
function serialize(f, o, tab)\n\
    if type(o) == \"string\" then\n\
        f:write(string.format(\"%q\", o))\n\
    elseif type(o) == \"table\" then\n\
        f:write(\"\\n\")\n\
        f:write(tab .. \"{\\n\")\n\
        for k,v in pairs(o) do\n\
            f:write(tab .. \"    \" .. \"[\"); serialize(f, k, \"\"); f:write(\"] = \")\n\
            serialize(f, v, tab .. \"    \")\n\
            f:write(\",\\n\")\n\
        end\n\
        f:write(tab .. \"}\")\n\
    else\n\
       if o ~= nil then f:write(o) end\n\
    end\n\
end\n\
";

char g_StatisticsFunctions_GetValue[] = "\
function Statistics_GetValue( table, datetime, row, col )\n\
    if( type(NStatistics) ~= \"table\" ) then NStatistics = {} end;\n\
    if( type(NStatistics[table]) ~= \"table\" ) then NStatistics[table] = {} end;\n\
    if( type(NStatistics[table][datetime]) ~= \"table\" ) then NStatistics[table][datetime] = {} end;\n\
    if( type(NStatistics[table][datetime][row]) ~= \"table\" ) then NStatistics[table][datetime][row] = {} end;\n\
\n\
    return NStatistics[table][datetime][row][col];\n\
end\n\
";

char g_StatisticsFunctions_AddValue[] = "\
function Statistics_AddValue( table, datetime, row, col, addval )\n\
    preitem = Statistics_GetValue( table, datetime, row, col );\n\
    if preitem == nil then\n\
        preval = Statistics_SetValue(table, datetime, row, col, 0 );\n\
        Statistics_SetValue( table, datetime, row, col, preval + addval );\n\
    else\n\
        if type(preitem) == \"number\" then\n\
            preval = Statistics_GetValue(table, datetime, row, col );\n\
            Statistics_SetValue( table, datetime, row, col, preval + addval );\n\
        end\n\
    end\n\
\n\
    return NStatistics[table][datetime][row][col];\n\
end\n\
";

char g_StatisticsFunctions_SetValue[] = "\
function Statistics_SetValue( table, datetime, row, col, setval )\n\
    if( type(NStatistics) ~= \"table\" ) then NStatistics = {} end;\n\
    if( type(NStatistics[table]) ~= \"table\" ) then NStatistics[table] = {} end;\n\
    if( type(NStatistics[table][datetime]) ~= \"table\" ) then NStatistics[table][datetime] = {} end;\n\
    if( type(NStatistics[table][datetime][row]) ~= \"table\" ) then NStatistics[table][datetime][row] = {} end;\n\
\n\
    NStatistics[table][datetime][row][col] = setval;\n\
\n\
    return NStatistics[table][datetime][row][col];\n\
end\n\
";

char g_StatisticsFunctions_Write[] = "\
function Statistics_Write( filename, table, datetime )\n\
    f = assert(io.open(filename, \"a\"))\n\
\n\
    f:write(\"if( type(NStatistics) ~= \\\"table\\\" ) then NStatistics = {} end;\\n\");\n\
    f:write(\"if( type(NStatistics[\\\"\" .. table .. \"\\\"]) ~= \\\"table\\\" ) then NStatistics[\\\"\" .. table .. \"\\\"] = {} end;\\n\");\n\
    f:write(\"if( type(NStatistics[\\\"\" .. table .. \"\\\"][\\\"\" .. datetime .. \"\\\"]) ~= \\\"table\\\" ) then NStatistics[\\\"\" .. table .. \"\\\"][\\\"\" .. datetime .. \"\\\"] = {} end;\\n\");\n\
\n\
    table_serialize(f, NStatistics[table][datetime], \"NStatistics[\\\"\" .. table .. \"\\\"][\\\"\" .. datetime .. \"\\\"]\")\n\
\n\
    f:close();\n\
end\n\
";

cltStatistics_Lib::cltStatistics_Lib( NLua* pclLua )
{
	// 기존 파일 읽기
	SYSTEMTIME stTime;		GetLocalTime(&stTime);

	m_pclLua = pclLua;
	if( m_pclLua == NULL )		{	m_pclLua = new NLua();		}

	// 최상위 테이블 변수 선언
	m_pclLua->GetGlobal( STATISTICS_ROOT );
	if( m_pclLua->IsNil() )
	{
		m_pclLua->Pop( 1 );
		char szString[64] = { '\0', };
		strcat(szString, STATISTICS_ROOT );
		strcat(szString, " = {}" );

		m_pclLua->DoString( szString );
	}

	char* pszError = NULL;
	if (m_pclLua->DoString( g_SaveStatistics ) == false)				{ pszError = (char*)m_pclLua->ToString(); }	
	if (m_pclLua->DoString( g_StatisticsFunctions_GetValue ) == false)	{ pszError = (char*)m_pclLua->ToString(); }	
	if (m_pclLua->DoString( g_StatisticsFunctions_SetValue ) == false)	{ pszError = (char*)m_pclLua->ToString(); }	
	if (m_pclLua->DoString( g_StatisticsFunctions_AddValue ) == false)	{ pszError = (char*)m_pclLua->ToString(); }	
	if (m_pclLua->DoString( g_StatisticsFunctions_Write ) == false)		{ pszError = (char*)m_pclLua->ToString(); }	
}

cltStatistics_Lib::~cltStatistics_Lib()
{
}

bool cltStatistics_Lib::Read(const char* pszFilename)
{
	char szString[1024];

	sprintf( szString, "dofile(\"%s\")", pszFilename );
	m_pclLua->DoString( szString );

	return true;
}

bool cltStatistics_Lib::Write(const char* pszFileName, SYSTEMTIME* psTime /* = NULL */)
{
	char szString[1024];

	sprintf( szString, "io.output(\"%s\")", pszFileName );
	m_pclLua->DoString( szString );

	char* pszError;

	DWORD dwClock = ::GetTickCount();

	char szTableName[1024];
	if( psTime != NULL )
	{
		sprintf( szString, "io.write( \"if( type(%s) ~= \\\"table\\\" ) then %s = {} end\" )", STATISTICS_ROOT, STATISTICS_ROOT );
		if( m_pclLua->DoString( szString ) == false )		{	pszError = (char*)m_pclLua->ToString();	}

		if( m_pclLua->DoString( "io.write( \"\\n\" )") == false )		{	pszError = (char*)m_pclLua->ToString();	}

		sprintf( szTableName, "%s[\"%04d-%02d-%02d\"]", STATISTICS_ROOT, psTime->wYear, psTime->wMonth, psTime->wDay );
		sprintf( szString, "table_serialize(%s, \"%s[\\\"%04d-%02d-%02d\\\"]\")", 
							szTableName, STATISTICS_ROOT, psTime->wYear, psTime->wMonth, psTime->wDay );
		if( m_pclLua->DoString( szString ) == false )		{	pszError = (char*)m_pclLua->ToString();	}
	}
	else
	{
		sprintf( szTableName, "%s", STATISTICS_ROOT );
		sprintf( szString, "table_serialize(%s, \"%s\")", szTableName, szTableName );
		m_pclLua->DoString( szString );
	}

	sprintf( szString, "io.write( \"-- time:%dms\" )", GetTickCount() - dwClock );
	if( m_pclLua->DoString( szString ) == false )		{	pszError = (char*)m_pclLua->ToString();	}


	m_pclLua->DoString( "io.output():close()" );
	m_pclLua->DoString( "io.output(io.stdout)" );

	return true;
}

bool cltStatistics_Lib::AppendTable_Hour(const char* pszTable, SYSTEMTIME* psTime )
{
	char* pszError;
	char szString[1024];

	char szDateTime[64];
	sprintf( szDateTime, "%04d-%02d-%02d_%02d:00", psTime->wYear, psTime->wMonth, psTime->wDay, psTime->wHour );

	char szFilename[64];
	sprintf( szFilename, "./Statistics/%s_%04d%02d%02d.log", pszTable, psTime->wYear, psTime->wMonth, psTime->wDay );

	sprintf(szString, "Statistics_Write(\"%s\", \"%s\", \"%s\")", szFilename, pszTable, szDateTime );

	if( m_pclLua->DoString( szString ) == false )		{	pszError = (char*)m_pclLua->ToString();	}

	return true;
}

bool cltStatistics_Lib::Clear()
{
	char szString[1024];

	sprintf( szString, "%s = {}", STATISTICS_ROOT );
	m_pclLua->DoString( szString );

	return true;
}

bool cltStatistics_Lib::AddValue(const char *pszTable, const char* pszRow, const char* pszCol, INT64 siAddVal )
{
	char* pszError;

	char szDateTime[64];
	SYSTEMTIME stTime;		GetLocalTime(&stTime);
	sprintf( szDateTime, "%04d-%02d-%02d_%02d:00", stTime.wYear, stTime.wMonth, stTime.wDay, stTime.wHour );

	char szString[1024];
	sprintf(szString,"Statistics_AddValue(\"%s\", \"%s\", \"%s\", \"%s\", %d)", pszTable, szDateTime, pszRow, pszCol, siAddVal );
	if( m_pclLua->DoString( szString ) == false )		{	pszError = (char*)m_pclLua->ToString();	}

	return true;
}

bool cltStatistics_Lib::SetValue(const char *pszTable, const char* pszRow, const char* pszCol, INT64 siVal )
{
	char* pszError;

	char szDateTime[64];
	SYSTEMTIME stTime;		GetLocalTime(&stTime);

	sprintf( szDateTime, "%04d-%02d-%02d_%02d:00", stTime.wYear, stTime.wMonth, stTime.wDay, stTime.wHour );

	char szString[1024];
	sprintf(szString,"Statistics_SetValue(\"%s\", \"%s\", \"%s\", \"%s\", %d)", pszTable, szDateTime, pszRow, pszCol, siVal );
	if( m_pclLua->DoString( szString ) == false )		{	pszError = (char*)m_pclLua->ToString();	}

	return true;
}

bool cltStatistics_Lib::SetValue(const char *pszTable, const char* pszRow, const char* pszCol, const char* pszVal )
{
	char* pszError;

	char szDateTime[64];
	SYSTEMTIME stTime;		GetLocalTime(&stTime);

	sprintf( szDateTime, "%04d-%02d-%02d_%02d:00", stTime.wYear, stTime.wMonth, stTime.wDay, stTime.wHour );

	char szString[1024];
	sprintf(szString,"Statistics_SetValue(\"%s\", \"%s\", \"%s\", \"%s\", \"%s\")", pszTable, szDateTime, pszRow, pszCol, pszVal );
	if( m_pclLua->DoString( szString ) == false )		{	pszError = (char*)m_pclLua->ToString();	}

	return true;
}

bool cltStatistics_Lib::AutoGetTableField( const char* pszField )
{
	if( m_pclLua->IsTable() == false )		return false;

	m_pclLua->GetTableField( pszField );
	if( m_pclLua->IsNil() == true )
	{
		// 생성
		m_pclLua->Pop( 1 );
		m_pclLua->PushString( pszField );
		m_pclLua->NewTable();
		m_pclLua->SetTable( -3 );

		m_pclLua->GetTableField( pszField );
	}

	return false;
}
