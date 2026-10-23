// RichODBC.cpp: implementation of the RichODBC class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RichODBC.h"


#define _SIZE_ERROR_MESSAGE

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#define REGULAR_SERVER	FALSE // TRUE면 정식 서버에 올리는 상태 , FALSE면 테스트 서버에 올리는 상태

RichODBC::RichODBC()
{
	/*
	if(CheckMDAC("2.8") == FALSE)
	{
		::exit(0);
	}
	*/
	m_pszQuery = new char[MAX_QUERY_SIZE];
	InitODBC();
}

RichODBC::~RichODBC()
{
	ReleaseODBC();
	if(m_pszQuery != NULL)
		delete[] m_pszQuery;
}

/*////////////////////////////////////////////////////////////////////
   Function Name : Destructor
   Param : N/A
   Desc : 메모리 해제
////////////////////////////////////////////////////////////////////*/
void RichODBC::ReleaseODBC()
{
	if(m_hdbc)
		SQLFreeHandle(SQL_HANDLE_DBC, m_hdbc);

	if(m_henv)
		SQLFreeHandle(SQL_HANDLE_ENV, m_henv);
}


/*////////////////////////////////////////////////////////////////////
   Function Name : Constructor
   Param : N/A
   Desc : 멤버 변수들의 초기화
////////////////////////////////////////////////////////////////////*/
void RichODBC::InitODBC()
{
	m_henv		= SQL_NULL_HENV;
	m_hdbc		= SQL_NULL_HDBC;
	m_hstmt		= SQL_NULL_HSTMT;

	m_siNTS		= SQL_NTS;
	m_siZero	= 0;

	m_siParamNo = 0;

	SetPooling(TRUE);
	
	ZeroMemory(m_pszQuery, sizeof(m_pszQuery));

	ZeroMemory(m_cErrQuery, sizeof(m_cErrQuery));
}

/*////////////////////////////////////////////////////////////////////
   Function Name : Connect
   Param1 : Driver (char*) ex) "SQL Server"
   Param2 : Server (char*) ex) "111.111.111.111"
   Param3 : Database (char*) ex) "TestDB"
   Param4 : USER ID (char*) ex) "sa"
   Param5 : Password (char*) ex) "password"
   Desc : Database에 접속
   return : void
////////////////////////////////////////////////////////////////////*/
BOOL RichODBC::Connect(char *cDriver, char *cServer, char *cDatabase, char *cUserID, char *cPasswd)
{
	ZeroMemory(m_cConnectionString, sizeof(m_cConnectionString));

	strcpy(m_cConnectionString, "DRIVER={");
	strcat(m_cConnectionString, cDriver);
	strcat(m_cConnectionString, "};SERVER=");
	strcat(m_cConnectionString, cServer);
	strcat(m_cConnectionString, ";DATABASE=");
	strcat(m_cConnectionString, cDatabase);
	strcat(m_cConnectionString, ";UID=");
	strcat(m_cConnectionString, cUserID);
	strcat(m_cConnectionString, ";PWD=");
	strcat(m_cConnectionString, cPasswd);
	strcat(m_cConnectionString, ";APP=RichODBC");

	if(ConnectProcess() == TRUE)
	{
		SetReconnect(FALSE);
		return TRUE;
	}
	else
	{
		SetReconnect(TRUE);
		return FALSE;
	}
}


BOOL RichODBC::ConnectProcess()
{

	SQLTCHAR	szConnStrOut[1024+1];
	SQLSMALLINT	siStrLen;

	InitODBC();

	if( IsPooling )
	{
		m_retcode = SQLSetEnvAttr(NULL, SQL_ATTR_CONNECTION_POOLING, (PTR)SQL_CP_ONE_PER_DRIVER , SQL_IS_UINTEGER );
		if(!IsSuccess())
		{
			CheckError( SQL_HANDLE_ENV, m_henv );
			return FALSE;
		}
	}

	// ODBC 환경 설정을 위한 메모리 할당
	// Allocate the ODBC environment and save handle.
	m_retcode = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &m_henv);
	if(!IsSuccess())
	{
		CheckError( SQL_HANDLE_ENV, m_henv );
		return FALSE;
	}

	// ODBC 환경 속성 설정 (ODBC버전 3.0)
	// Notify ODBC that this is an ODBC 3.0 app.
	m_retcode = SQLSetEnvAttr(m_henv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER) SQL_OV_ODBC3, SQL_IS_INTEGER);
	if(!IsSuccess())
	{
		CheckError( SQL_HANDLE_ENV, m_henv );
		return FALSE;
	}

	// ODBC 연결을 위한 메모리 할당
	// Allocate ODBC connection handle and connect.
	m_retcode = SQLAllocHandle(SQL_HANDLE_DBC, m_henv, &m_hdbc);
	if(!IsSuccess())
	{
		CheckError( SQL_HANDLE_ENV, m_henv );
		return FALSE;
	}

	// ODBC를 이용한 데이터베이스와의 연결속성 설정
	//Make sure that autocommit is set to ON.
	m_retcode = SQLSetConnectAttr(m_hdbc,SQL_ATTR_AUTOCOMMIT,(void *)SQL_AUTOCOMMIT_ON,0);


	// Connect.
	m_retcode = SQLDriverConnect(m_hdbc, NULL, (SQLTCHAR*)m_cConnectionString, SQL_NTS, szConnStrOut, 1024, &siStrLen, SQL_DRIVER_NOPROMPT);
	if(!IsSuccess())
	{
		CheckError( SQL_HANDLE_DBC, m_hdbc );
		MessageBox(NULL, "Connect Fail", "SQLDriverConnect", MB_OK);
		return FALSE;
	}

	return TRUE;
}


BOOL RichODBC::Reconnect()
{

	SetReconnect(true);
	Disconnect();
	ReleaseODBC();
	return ConnectProcess();
}



/*////////////////////////////////////////////////////////////////////
   Function Name : Prepare
   Param1 : N/A
   Desc : 다른 쿼리를 생성하여 실행할때 호출한다.
   return : N/A
////////////////////////////////////////////////////////////////////*/
void RichODBC::Prepare()
{
	SetReconnect(false);

	// Allocate statement handle.
	if(m_hstmt!=NULL)
	{
		m_retcode = SQLFreeHandle(SQL_HANDLE_STMT,m_hstmt);
		if(!IsSuccess())
		{
			CheckError( SQL_HANDLE_ENV, m_henv );
		}
	}

	m_retcode = SQLAllocHandle(SQL_HANDLE_STMT, m_hdbc, &m_hstmt);
	if(!IsSuccess())
	{
		CheckError( SQL_HANDLE_ENV, m_henv );
	}
}


/*////////////////////////////////////////////////////////////////////
   Function Name : SetQuery
   Param1 : 쿼리 (char*)
   Desc : 실행할 쿼리를 입력받음.
   return : int(성공시 양수, 실패시 음수)
////////////////////////////////////////////////////////////////////*/
int RichODBC::SetQuery(char *pszQuery)
{
	Prepare();

/*
	if(m_pszQuery != NULL)
		delete[] m_pszQuery;

	m_pszQuery = new char[strlen(pszQuery) + 1];
*/
	if( sizeof(pszQuery) >= MAX_QUERY_SIZE )
		MessageBox(NULL, "쿼리를 담을 버퍼의 크기가 모자릅니다.", "Error", MB_OK);

	strncpy(m_pszQuery, pszQuery, MAX_QUERY_SIZE);

	PrintQuery(pszQuery);

	return 1;
}

/*////////////////////////////////////////////////////////////////////
   Function Name : Disconnect
   Param1 : N/A
   Desc : 접속을 해제하고 DB핸들을 해제 시켜준다.
   return : int(성공시 양수, 실패시 음수)
////////////////////////////////////////////////////////////////////*/
int RichODBC::Disconnect()
{

	m_retcode = SQLAllocHandle(SQL_HANDLE_STMT, m_hdbc, &m_hstmt);
	m_retcode = SQLFreeHandle(SQL_HANDLE_STMT, m_hstmt);

	m_retcode = SQLDisconnect(m_hdbc);

	if(IsSuccess())
		return 1;
	else
		return m_retcode;
}


/*////////////////////////////////////////////////////////////////////
   Function Name : GetData
   Param1 : 컬럼 번호
   Param2 : 받아올 데이터의 크기
   Param3 : 데이터 포인터 
   Desc	  : 쿼리 실행으로 얻어온 데이터를 받는다.
   return : int(성공시 양수, 실패시 음수)
////////////////////////////////////////////////////////////////////*/
int RichODBC::GetData(int iColNo, void* pvData, long lDataSize)
{
	long ntd;
	if(IsSuccess())
		m_retcode = SQLGetData(m_hstmt, iColNo, SQL_C_DEFAULT, pvData, lDataSize, &ntd);
	else
		return -1;

	return 1;
}


/*////////////////////////////////////////////////////////////////////
   Function Name : GetData
   Param1 : 컬럼 번호
   Param2 : 데이터 타입 (SQL_INTEGER, SQL_BINARY, SQL_CHAR)
   Param3 : 받아올 데이터의 크기
   Param4 : 데이터 포인터 
   Desc : 쿼리 실행으로 얻어온 데이터를 받는다.
   return : int(성공시 양수, 실패시 음수)
////////////////////////////////////////////////////////////////////*/
int RichODBC::GetData(int iColNo, int iType, void* pvData, long lDataSize)
{
	long ntd;
	if(IsSuccess())
		m_retcode = SQLGetData(m_hstmt, iColNo, iType, pvData, lDataSize, &ntd);
	else
		return -1;

	return 1;
}

/*////////////////////////////////////////////////////////////////////
   Function Name : FetchData
   Param1 : N/A
   Desc : 다음 데이터를 Fetch시킨다.
   return : int(성공시 양수, 실패시 음수)
////////////////////////////////////////////////////////////////////*/
int RichODBC::FetchData()
{	
	m_siParamNo = 1;

	m_retcode = SQLFetch(m_hstmt);
	if(IsSuccess())
	{
		return 1;
	}
	else
	{
		if ( m_retcode != SQL_NO_DATA )
			CheckError(SQL_HANDLE_STMT, m_hstmt);
		return -1;
	}
}

/*////////////////////////////////////////////////////////////////////
   Function Name : NextRecordSet
   Param1 : N/A
   Desc : 다음 레코드셋으로 이동한다.
   return : int(성공시 양수, 더이상 데이터가 없을시 0, 실패시 음수)
////////////////////////////////////////////////////////////////////*/
int RichODBC::NextRecordSet()
{
	m_retcode = SQLMoreResults(m_hstmt);
	if(IsSuccess())
		return 1;
	else
	{
		if(m_retcode == SQL_NO_DATA)
			return 0;
		CheckError(SQL_HANDLE_STMT, m_hstmt);
		return -1;
	}
}

/*////////////////////////////////////////////////////////////////////
   Function Name : ExecSQL
   Param1 : N/A
   Desc : 셋팅된 쿼리를 실행한다.
   return : int(성공시 양수, 실패시 음수)
////////////////////////////////////////////////////////////////////*/
int RichODBC::ExecSQL()
{
	m_retcode = SQLExecDirect(m_hstmt,(SQLCHAR*)(LPSTR)(LPCSTR)m_pszQuery, SQL_NTS);

	if(!IsSuccess())
	{
		bCanFetchRow = FALSE;
		CheckError(SQL_HANDLE_STMT, m_hstmt, m_pszQuery);
		WriteQuery();
		return 0;
	}
	else
	{
		return 1;
	}
}


/*////////////////////////////////////////////////////////////////////
   Function Name : IsSuccess
   Param1 : N/A
   Desc : ODBC관련 명령이 성공했는지 실패했는지 판단한다.
   return : BOOL (성공시 TRUE, 실패시 FALSE)
////////////////////////////////////////////////////////////////////*/
BOOL RichODBC::IsSuccess(void)
{
	if(m_retcode == SQL_SUCCESS)
		return TRUE;
	else if(m_retcode == SQL_SUCCESS_WITH_INFO)
	{
		CheckError(SQL_HANDLE_DBC, m_hdbc);
		return TRUE;
	}
	else
		return FALSE;
}


/*////////////////////////////////////////////////////////////////////
   Function Name : CheckError
   Param1 : 핸들의 종류 (SQL_HANDLE_STMT, SQL_HANDLE_DBC, SQL_HANDLE_ENV등등)
   Param2 : 핸들
   Param3 : 에러메시지에 추가로 표시될 힌트...
   Desc : 에러 발생시 발생된 에러를 표시한다.
   return : N/A
////////////////////////////////////////////////////////////////////*/
BOOL RichODBC::CheckError(SQLSMALLINT hType, SQLHANDLE handle, char* hint, HWND hwnd)
{
	SQLCHAR       SqlState[6], Msg[SQL_MAX_MESSAGE_LENGTH];
	char caption[256]="", err[2048]="";
	SQLINTEGER    NativeError =0;
	SQLSMALLINT   i = 1, MsgLen;
	SQLRETURN	 thisRet ;
	BOOL	ret = TRUE ;

	ZeroMemory ( SqlState, sizeof(SqlState ) );
	ZeroMemory ( Msg, sizeof(Msg ) );
	
	if( hint ){
		strncpy(caption, hint, sizeof(caption) );
		strcat(caption, "\n" );
	}
    while ((thisRet = SQLGetDiagRec(hType, handle, i, SqlState, &NativeError, Msg, sizeof(Msg), &MsgLen) != SQL_NO_DATA))
	{
		if( thisRet == SQL_INVALID_HANDLE )
		{
			strncpy(err, "Invalid Handle\n", sizeof(err));
			break;
		}
		else if ( thisRet == SQL_ERROR )
		{
			strncpy(err, "RecNumber was negative or 0 \n", sizeof(err));
			strcat(err, "BufferLength was less than zero \n");
			break;
		} 

		if( hType == SQL_HANDLE_DBC && handle == m_hdbc && m_retcode == SQL_SUCCESS_WITH_INFO
			&& !strcmp( (char*)SqlState , "01000" ) )
		{
			ret = FALSE ;
		}
		else
		{
			ret = TRUE ;
			sprintf(err, "%s %s\n", SqlState, Msg);
			strcpy(err, (char*)SqlState);
			strcat(err, " ");
			strcat(err, (char*)Msg);
		}
		i++;
		PrintErrMsg(err);
    }

	if( !strcmp( (char*)SqlState, "08S01" ) )
	{
		Reconnect();
	}
	else if (		strcmp( (char*)SqlState, "01000" )
				&&	strcmp( (char*)SqlState, "24000" )
				&&	strcmp( (char*)SqlState, "23000" )
				&&	strcmp( (char*)SqlState, "42000" )
			)
	{
		MessageBox(hwnd, m_cErrQuery, hint, MB_OK);
	}
	return ret;
}

BOOL RichODBC::CheckMDAC(char *cRequire)
{
	HKEY hKey;
	char cData[256]= "";
	char msg[1024];
	LONG result = 0;
	BOOL ret = FALSE;
	DWORD size;

	result = RegOpenKeyEx(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\DataAccess", 0, KEY_READ, &hKey);
	if(result != ERROR_SUCCESS)
	{
		printf("Registry Open Failed : %d\n", result);
	}

	result = RegQueryValueEx(hKey, "FullInstallVer", 0, NULL, (BYTE*)cData, &size);
	if(result != ERROR_SUCCESS)
	{
		printf("Registry Query Failed : %d\n", result);
	}
	else
	{
		if(strcmp(cData, cRequire) >= 0)
			ret = TRUE;
		else
		{
			sprintf(msg, "요구버전 : %s\n현재버전 : %s", cRequire, cData);
			MessageBox(NULL, msg, "MDAC Version", MB_OK);
		}
	}

	RegCloseKey(hKey);
	return ret;
}

/*////////////////////////////////////////////////////////////////////
   Function Name : GetResult
   Param1 : 결과를 담을 데이터
   Param2 : 데이터의 최대크기.
   Desc : 쿼리를 실행한 결과값을 얻는다. ( 쿼리의 결과가 1로우/1컬럼일 경우만.)
   return : N/A
////////////////////////////////////////////////////////////////////*/
VOID RichODBC::GetResult(VOID *pvResult, long siDatasize)
{
//	SQLINTEGER siRowcount=0;
//	SQLSetStmtAttr(m_hstmt, SQL_ATTR_ROWS_FETCHED_PTR, &siRowcount, 0);

	while ( FetchData() > 0 )
	{
//		m_siRowcount = siRowcount;
		GetData(1, SQL_C_DEFAULT, pvResult, siDatasize);
	}
}



/*////////////////////////////////////////////////////////////////////
   Function Name : GetRowcount
   Param1 : N/A
   Desc : 리턴된 결과의 최대 줄 수를 얻는다.
   return : int
////////////////////////////////////////////////////////////////////*/
int RichODBC::GetRowcount()
{
	return m_siRowcount;
}



/*////////////////////////////////////////////////////////////////////
   Function Name : SetParam
   Param1 : 파라메타 번호
   Param2 : 데이타 타입
   Param3 : 데이타
   Param4 : 데이타 길이
   Desc : 쿼리를 호출할때 사용할 파라메타를 설정한다.
   return : N/A
////////////////////////////////////////////////////////////////////*/
VOID RichODBC::SetParam(int siParamNo, int siType, void *pData, int siDatasize)
{
	/*///////////////////////////////////////////////////////////////////
	siType의 종류
		SQL_CHAR		: char * (문자열)
		SQL_VARCHAR		: char * (문자열)
		SQL_TINYINT		: unsigned char (1byte)
		SQL_SMALLINT	: small int (2byte)
		SQL_INT			: long int	(4byte)
		SQL_BIGINT		: _int64	(8byte)
		SQL_REAL		: float		(4byte)
		SQL_BINARY		: unsigned char * (이진문자열, 최대길이 8000바이트)
		SQL_TYPE_TIMESTAMP : TIMESTAMP_STRUCT	(16byte)
	///////////////////////////////////////////////////////////////////*/
	char buffer[20];
	ZeroMemory(buffer, sizeof(buffer));

	switch(siType)
	{
	case SQL_CHAR:
		m_retcode = SQLBindParameter(m_hstmt, siParamNo, SQL_PARAM_INPUT, SQL_C_TCHAR, SQL_CHAR, siDatasize, 0, pData, 0, &m_siNTS);
		PrintParam((char*)pData);
		break;
	case SQL_VARCHAR:
		m_retcode = SQLBindParameter(m_hstmt, siParamNo, SQL_PARAM_INPUT, SQL_C_TCHAR, SQL_VARCHAR, siDatasize, 0, pData, 0, &m_siNTS);
		PrintParam((char*)pData);
		break;
	case SQL_TINYINT:
#ifdef _SIZE_ERROR_MESSAGE
		if(siDatasize != sizeof(char))
		{
			char err[255] = "";
			wsprintf( err, "RequireSize = SQL_TINYINT(1Byte), InputSize = %d\n", siDatasize);
			BOOL bServer = REGULAR_SERVER ;
			if ( !bServer )
				MessageBox(NULL, err, "데이터 타입이 틀리다", MB_OK);
		}
#endif
		m_retcode = SQLBindParameter(m_hstmt, siParamNo, SQL_PARAM_INPUT, SQL_C_TINYINT, SQL_TINYINT, 0, 0, pData, 0, &m_siZero);
		_itoa( *((char*)(pData)), buffer, 10);
		PrintParam(buffer);
		break;
	case SQL_SMALLINT:
#ifdef _SIZE_ERROR_MESSAGE
		if(siDatasize != sizeof(short int))
		{
			char err[255] = "";
			wsprintf( err, "RequireSize = SQL_SMALLINT(2Byte), InputSize = %d\n", siDatasize);
			BOOL bServer = REGULAR_SERVER ;
			if ( !bServer )
				MessageBox(NULL, err, "데이터 타입이 틀리다", MB_OK);
		}
#endif
		m_retcode = SQLBindParameter(m_hstmt, siParamNo, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, pData, 0, &m_siZero);
		_itoa( *((short*)(pData)), buffer, 10);
		PrintParam(buffer);
		break;
	case SQL_INTEGER:
#ifdef _SIZE_ERROR_MESSAGE
		if(siDatasize != sizeof(int))
		{
			char err[255] = "";
			wsprintf( err, "RequireSize = SQL_INTEGER(4Byte), InputSize = %d\n", siDatasize);
			BOOL bServer = REGULAR_SERVER ;
			if ( !bServer )
				MessageBox(NULL, err, "데이터 타입이 틀리다", MB_OK);
		}
#endif
		m_retcode = SQLBindParameter(m_hstmt, siParamNo, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, pData, 0, &m_siZero);
		_itoa( *((int*)(pData)), buffer, 10);
		PrintParam(buffer);
		break;
	case SQL_BIGINT:
#ifdef _SIZE_ERROR_MESSAGE
		if(siDatasize != sizeof(__int64))
		{
			char err[255] = "";
			wsprintf( err, "RequireSize = SQL_BIGINT(8Byte), InputSize = %d\n", siDatasize);
			BOOL bServer = REGULAR_SERVER ;
			if ( !bServer )
				MessageBox(NULL, err, "데이터 타입이 틀리다", MB_OK);
		}
#endif
		m_retcode = SQLBindParameter(m_hstmt, siParamNo, SQL_PARAM_INPUT, SQL_C_SBIGINT, SQL_BIGINT, 0, 0, pData, 0, &m_siZero);
		_itoa( *((int*)(pData)), buffer, 10);
		PrintParam(buffer);
		break;
	case SQL_REAL:
#ifdef _SIZE_ERROR_MESSAGE
		if(siDatasize != sizeof(float))
		{
			char err[255] = "";
			wsprintf( err, "RequireSize = SQL_REAL(4Byte), InputSize = %d\n", siDatasize);
			BOOL bServer = REGULAR_SERVER ;
			if ( !bServer )
				MessageBox(NULL, err, "데이터 타입이 틀리다", MB_OK);
		}
#endif
		m_retcode = SQLBindParameter(m_hstmt, siParamNo, SQL_PARAM_INPUT, SQL_C_FLOAT, SQL_REAL, 0, 0, pData, 0, &m_siZero);
		_itoa( *((int*)(pData)), buffer, 10);
		PrintParam(buffer);
		break;
	case SQL_BINARY:
		m_siParamSize = siDatasize; // StrLen_or_IndPtr인자는 값을 Execute시에 참조하므로 지역변수로 값을 대입하면 않된다.
		m_retcode = SQLBindParameter(m_hstmt, siParamNo, SQL_PARAM_INPUT, SQL_C_BINARY, SQL_BINARY, siDatasize, 0, pData, siDatasize, &m_siParamSize);
		PrintParam("binary data");
		break;
	}

	if(!IsSuccess())
	{
		bCanFetchRow = FALSE;
		CheckError(SQL_HANDLE_STMT, m_hstmt, m_pszQuery);
		WriteQuery();
	}
}


void RichODBC::PrintQuery(char *msg)
{
	ZeroMemory(m_cErrQuery, sizeof(m_cErrQuery));
	sprintf(m_cErrQuery, "%s\n", msg);
}

void RichODBC::PrintErrMsg(char *msg)
{
	strcat(m_cErrQuery, "\n\t");
	strcat(m_cErrQuery, msg);
	strcat(m_cErrQuery, "\n");
}

void RichODBC::PrintParam(char *msg)
{
	strcat(m_cErrQuery, "\t\t");
	strcat(m_cErrQuery, msg);
	strcat(m_cErrQuery, "\n");
}

void RichODBC::WriteQuery()
{
	SYSTEMTIME CT;
	GetLocalTime(&CT);
	
	char szFileName[256] = "";
	sprintf(szFileName, "ErrorQuery_%02d%02d%02d.txt", CT.wYear, CT.wMonth, CT.wDay);

	m_Queryfp = fopen(szFileName, "a+");

	fprintf(m_Queryfp, "\n%02d/%02d/%02d %02d:%02d:%02d %04d\n%s", CT.wYear, CT.wMonth, CT.wDay, CT.wHour, CT.wMinute, CT.wSecond, CT.wMilliseconds, m_cErrQuery);
	fclose(m_Queryfp);
}

//*/////////////////////////////////////////////////////////
void RichODBC::BeginSP(char *pszQuery)
{
	Prepare();

	m_siParamNo = 0;

	strcpy(m_pszQuery, "{call ");
	strcat(m_pszQuery, pszQuery);

	PrintQuery(pszQuery);
}

void RichODBC::EndSP()
{
	if( m_siParamNo == 0 )
	{
		strcat( m_pszQuery, "}" );
	}
	else
	{
		strcat( m_pszQuery, " )}" );
	}
}

void RichODBC::SetParam(int siType, void *pData, int siDatasize)
{
	if( m_siParamNo == 0 )
		strcat( m_pszQuery, " ( ?" );
	else
		strcat( m_pszQuery, ", ?" );

	SetParam( ++m_siParamNo, siType, pData, siDatasize );
}


int RichODBC::GetData(void *pvData, long siDataSize)
{
	return GetData( m_siParamNo++, pvData, siDataSize);
}
/////////////////////////////////////////////////////////*/
