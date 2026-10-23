// RichODBC.cpp: implementation of the RichODBC class.
//
//////////////////////////////////////////////////////////////////////

#include "DBGW.h"

// ----------------------------------------------------------------------------------------------
// HANGAME DBGW 사용시에만 컴파일이 된다.
// ----------------------------------------------------------------------------------------------
#ifdef USE_DBGW
// ----------------------------------------------------------------------------------------------

extern char g_DBGW_ADDR[];

#define _SIZE_ERROR_MESSAGE

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#define REGULAR_SERVER	FALSE // TRUE면 정식 서버에 올리는 상태 , FALSE면 테스트 서버에 올리는 상태

DBGW::DBGW()
{
	m_pszQuery		= new char[MAX_QUERY_SIZE_DBGW * 10];
	m_pszRspName	= new char[MAX_QUERY_SIZE_DBGW * 10];

	InitDBGW();
}

DBGW::~DBGW()
{
	ReleaseDBGW();

	if(m_pszQuery != NULL)		delete[] m_pszQuery;
	if(m_pszRspName != NULL)	delete[] m_pszRspName;	
}

/*////////////////////////////////////////////////////////////////////
   Function Name : Destructor
   Param : N/A
   Desc : 메모리 해제
////////////////////////////////////////////////////////////////////*/
void DBGW::ReleaseDBGW()
{
	if(m_pkTable != NULL)	{	m_pkTable->release();		m_pkTable = NULL;		}
	if(m_pkRetList != NULL)	{	m_pkRetList->release();		m_pkRetList = NULL;		}

	if(m_pkParamList)		{	m_pkParamList->release();	m_pkParamList = NULL;	}

	if(m_pkRPCClient)		{	delete m_pkRPCClient;		m_pkRPCClient = NULL;	}
	if(m_pkRPCConnector)	{	delete m_pkRPCConnector;	m_pkRPCConnector = NULL;	}
}


/*////////////////////////////////////////////////////////////////////
   Function Name : Constructor
   Param : N/A
   Desc : 멤버 변수들의 초기화
////////////////////////////////////////////////////////////////////*/
void DBGW::InitDBGW()
{
	m_pkRPCConnector	= NULL;
	m_pkRPCClient		= NULL;

	m_pkParamList		= NULL;

	m_pkResult			= NULL;
	m_pkRetList			= NULL;
	m_pkTable			= NULL;

	m_siParamNo = 0;
	
	ZeroMemory(m_pszQuery, sizeof(m_pszQuery));
	ZeroMemory(m_pszRspName, sizeof(m_pszRspName));
	ZeroMemory(m_cErrQuery, sizeof(m_cErrQuery));
}

/*////////////////////////////////////////////////////////////////////
   Function Name : Connect
   Desc : DBGW에 접속
   return : void
////////////////////////////////////////////////////////////////////*/
// 파라미터는 버젼 호환성을 위해 한 것일 뿐임.
BOOL DBGW::Connect(char *cDriver, char *cServer, char *cDatabase, char *cUserID, char *cPasswd)
{
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


BOOL DBGW::ConnectProcess()
{
	InitDBGW();

	// Connector 객체 생성
	m_pkRPCConnector	= mega::CreateRPCConnector();

	// 클라이언트 객체 생성
	m_pkRPCClient		= mega::CreateRPCClient();

	// 연결
	if(m_pkRPCConnector->open(m_pkRPCClient, g_DBGW_ADDR, 3000, mega::IRPCConnector::TP_TCP) == false)
	{
		char szMessage[2048] = "";
		sprintf(szMessage, "Connect Fail : %s\n", m_pkRPCConnector->errorMessage());
		MessageBox(NULL, szMessage, "SQLDriverConnect", MB_OK);

		delete m_pkRPCClient;
		delete m_pkRPCConnector;

		m_pkRPCClient		= NULL;
		m_pkRPCConnector	= NULL;

		return FALSE;
	}

	return TRUE;
}


BOOL DBGW::Reconnect()
{
	SetReconnect(true);
	Disconnect();
	ReleaseDBGW();

	return ConnectProcess();
}



/*////////////////////////////////////////////////////////////////////
   Function Name : Prepare
   Param1 : N/A
   Desc : 다른 쿼리를 생성하여 실행할때 호출한다.
   return : N/A
////////////////////////////////////////////////////////////////////*/
void DBGW::Prepare()
{
	SetReconnect(false);

	m_pkResult		= NULL;

	if(m_pkTable != NULL)
	{
		m_pkTable->release();
		m_pkTable = NULL;
	}

	if(m_pkRetList != NULL)
	{
		m_pkRetList->release();
		m_pkRetList = NULL;
	}

	m_pkParamList = mega::CreateListInstance();
}


/*////////////////////////////////////////////////////////////////////
   Function Name : SetQuery
   Param1 : 쿼리 (char*)
   Desc : 실행할 쿼리를 입력받음.
   return : int(성공시 양수, 실패시 음수)
////////////////////////////////////////////////////////////////////*/
int DBGW::SetQuery(char *pszQuery)
{
	Prepare();

	if( sizeof(pszQuery) >= MAX_QUERY_SIZE_DBGW )
		MessageBox(NULL, "쿼리를 담을 버퍼의 크기가 모자릅니다.", "Error", MB_OK);

	strncpy(m_pszQuery, pszQuery, MAX_QUERY_SIZE_DBGW);

	PrintQuery(pszQuery);

	return 1;
}

/*////////////////////////////////////////////////////////////////////
   Function Name : Disconnect
   Param1 : N/A
   Desc : 접속을 해제하고 DB핸들을 해제 시켜준다.
   return : int(성공시 양수, 실패시 음수)
////////////////////////////////////////////////////////////////////*/
int DBGW::Disconnect()
{
	return 1;
}


/*////////////////////////////////////////////////////////////////////
   Function Name : GetData
   Param1 : 컬럼 번호
   Param2 : 받아올 데이터의 크기
   Param3 : 데이터 포인터 
   Desc	  : 쿼리 실행으로 얻어온 데이터를 받는다.
   return : int(성공시 양수, 실패시 음수)
////////////////////////////////////////////////////////////////////*/
int DBGW::GetData(int iColNo, void* pvData, long lDataSize)
{	
	if(pvData == NULL)				return -1;
	if(lDataSize < 0)				return -1;

	if(IsSuccess())
	{
		if(m_pkTable == NULL)			return -1;

		if((int)m_pkTable->rowSize() <= m_siCurRow || (int)m_pkTable->columnSize() <= iColNo)		return -1;

		const mega::IVariant* pVal = m_pkTable->getValue(m_siCurRow, iColNo);

		if(pVal->type() == mega::IVariant::TYPE_BIGINT)		// 사용안함
		{
		}
		if(pVal->type() == mega::IVariant::TYPE_BOOL)		// BOOL
		{
			pVal->get((bool*)pvData);
		}
		else if(pVal->type() == mega::IVariant::TYPE_CHAR)	// 사용안함
		{
		}
		else if(pVal->type() == mega::IVariant::TYPE_INT)	// INT : 1 or 2 or 4 BYTE
		{
			switch(lDataSize)
			{
			case 1:		pVal->get((int8_t*)pvData);			break;
			case 2:		pVal->get((int16_t*)pvData);		break;
			case 4:		pVal->get((int*)pvData);			break;
			}
		}
		else if(pVal->type() == mega::IVariant::TYPE_LONG)	// INT : 8 BYTE
		{
			pVal->get((int64_t*)pvData);
		}
		else if(pVal->type() == mega::IVariant::TYPE_DATE)	// 사용안함
		{
		}
		else if(pVal->type() == mega::IVariant::TYPE_STRING)// 문자열
		{
			const char* pszGet;
			size_t	nLen = pVal->size() < lDataSize ? pVal->size() : lDataSize;
			pVal->get(&pszGet, &nLen);
			strncpy((char*)pvData, pszGet, lDataSize);
		}
		else if(pVal->type() == mega::IVariant::TYPE_BINARY)// 바이너리 데이터
		{
			const void *pBinary;
			size_t	nLen = pVal->size() < lDataSize ? pVal->size() : lDataSize;
			pVal->get(&pBinary, &nLen);
			if(nLen > 0)
			{
				memcpy(pvData, pBinary, lDataSize);
			}
		}
		else if(pVal->type() == mega::IVariant::TYPE_DOUBLE)// 실수 : 4 or 8 BYTE
		{
			switch(lDataSize)
			{
			case 4:		pVal->get((float*)pvData);		break;
			//case 8:		pVal->get((double*)pvData);		break;
			//--------------------------------------------------
			// 임시로 8 BYTE 정수로 변경해서 사용
			//--------------------------------------------------
			case 8:
				{
					double dVal;
					pVal->get((double*)&dVal);

					*(int64_t*)pvData = (int64_t)dVal;
				}
				break;
			}
			//--------------------------------------------------
		}
		else if(pVal->type() == mega::IVariant::TYPE_NULL)	// 사용안함
		{

		}
	}
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
//int DBGW::GetData(int iColNo, int iType, void* pvData, long lDataSize)
//{
//	return GetData( m_siCurCol++, pvData, lDataSize );
//}

/*////////////////////////////////////////////////////////////////////
   Function Name : FetchData
   Param1 : N/A
   Desc : 다음 데이터를 Fetch시킨다.
   return : int(성공시 양수, 실패시 음수)
////////////////////////////////////////////////////////////////////*/
int DBGW::FetchData()
{	
	if(!IsSuccess())					return -1;
	if(m_pkTable == NULL)				return -1;

	if(m_siCurRow >= (int)m_pkTable->rowSize() - 1)	return -1;
	m_siCurRow++;
	m_siCurCol = 0;

	return 1;
}

/*////////////////////////////////////////////////////////////////////
   Function Name : NextRecordSet
   Param1 : N/A
   Desc : 다음 레코드셋으로 이동한다.
   return : int(성공시 양수, 더이상 데이터가 없을시 0, 실패시 음수)
////////////////////////////////////////////////////////////////////*/
int DBGW::NextRecordSet()
{
	if(!IsSuccess())			return -1;
	if(m_pkRetList == NULL)		return -1;

	m_siLastTable++;
	while(m_siLastTable < (int)m_pkRetList->size())
	{
		if(m_pkRetList->at(m_siLastTable)->type() == mega::IVariant::TYPE_TABLE)
		{
			if(m_pkTable != NULL)
			{
				m_pkTable->release();
				m_pkTable = NULL;
			}

			m_pkTable = m_pkRetList->at(m_siLastTable)->tableVal();
			m_siCurRow = -1;
			m_siCurCol = 0;
			break;
		}

		m_siLastTable++;
	}

	if(m_pkTable == NULL)		return -1;

	return 1;
}

/*////////////////////////////////////////////////////////////////////
   Function Name : ExecSQL
   Param1 : N/A
   Desc : 셋팅된 쿼리를 실행한다.
   return : int(성공시 양수, 실패시 음수)
////////////////////////////////////////////////////////////////////*/
int DBGW::ExecSQL()
{
	if(m_pkRPCClient == NULL)			return 0;

	DWORD dwTick = GetTickCount();
	const mega::IRPCResult* pkResult = m_pkRPCClient->exec(m_pszRspName, m_pkParamList, m_pkParamList->size());

	//printf("%s\n", m_pszRspName);

	m_pkParamList->release();
	m_pkParamList = NULL;

	//if(GetTickCount() - dwTick > 100)
	if(strstr(m_pszQuery, "rsp_HouseUnitManyGet") != NULL)
	{
		FILE *fp = fopen("Performence_DBGW.txt",	"a");
		if(fp)
		{	
			fprintf(fp, "%s	%d\n", m_pszRspName, GetTickCount() - dwTick);
			fclose(fp);
		}
	}

	if(pkResult == NULL)				return 0;

	m_pkResult = (mega::IRPCResult*)pkResult;
	if(!IsSuccess())
	{
		bCanFetchRow = FALSE;
		WriteQuery();
		return 0;
	}
	else
	{
		m_siLastTable = -1;
		m_pkRetList = (mega::IList*)pkResult->objects();
		NextRecordSet();

		return 1;
	}

	return 0;
}


/*////////////////////////////////////////////////////////////////////
   Function Name : IsSuccess
   Param1 : N/A
   Desc : ODBC관련 명령이 성공했는지 실패했는지 판단한다.
   return : BOOL (성공시 TRUE, 실패시 FALSE)
////////////////////////////////////////////////////////////////////*/
BOOL DBGW::IsSuccess(void)
{
	if(m_pkResult != NULL && m_pkResult->isSuccess() == true)
		return TRUE;
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
BOOL DBGW::CheckError(SQLSMALLINT hType, SQLHANDLE handle, char* hint, HWND hwnd)
{
	return TRUE;
}

BOOL DBGW::CheckMDAC(char *cRequire)
{
	return TRUE;
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
VOID DBGW::SetParam(int siParamNo, int siType, void *pData, int siDatasize, SQLSMALLINT siParamType)
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

	siParamType의 종류
		SQL_PARAM_TYPE_UNKNOWN
		SQL_PARAM_INPUT
		SQL_PARAM_INPUT_OUTPUT
		SQL_RESULT_COL
		SQL_PARAM_OUTPUT
		SQL_RETURN_VALUE
	///////////////////////////////////////////////////////////////////*/
	char buffer[64];
	ZeroMemory(buffer, sizeof(buffer));

	int		nValue = 0;
	short	sValue = 0;
	int64_t	lValue = 0;
	float	fValue = 0;
	double	dValue = 0;

	switch(siType)
	{
	case SQL_CHAR:
		m_pkParamList->add((char*)pData);
		PrintParam((char*)pData);
		break;
	case SQL_VARCHAR:
		m_pkParamList->add((char*)pData);
		PrintParam((char*)pData);
		break;
	case SQL_TINYINT:
		_itoa( *((BYTE*)(pData)), buffer, 10);
#ifdef _SIZE_ERROR_MESSAGE
		if(siDatasize != sizeof(char))
		{
			char err[2048] = "";
			wsprintf( err, "ParamNo = %d, RequireSize = SQL_TINYINT(1Byte), InputSize = %d\n%s\n", siParamNo, siDatasize, m_pszQuery);
			BOOL bServer = REGULAR_SERVER ;
			if ( !bServer )
				MessageBox(NULL, err, "데이터 타입이 틀리다", MB_OK);
		}
#endif
		nValue = *(BYTE*)pData;
		m_pkParamList->add(nValue);
		PrintParam(buffer);
		break;
	case SQL_SMALLINT:
		_itoa( *((short*)(pData)), buffer, 10);
#ifdef _SIZE_ERROR_MESSAGE
		if(siDatasize != sizeof(short int))
		{
			char err[2048] = "";
			wsprintf( err, "ParamNo = %d, RequireSize = SQL_SMALLINT(2Byte), InputSize = %d\n%s\n", siParamNo, siDatasize, m_pszQuery);
			BOOL bServer = REGULAR_SERVER ;
			if ( !bServer )
				MessageBox(NULL, err, "데이터 타입이 틀리다", MB_OK);
		}
#endif
		sValue = *(short*)pData;
		m_pkParamList->add((int)sValue);
		PrintParam(buffer);
		break;
	case SQL_INTEGER:
		_itoa( *((int*)(pData)), buffer, 10);
#ifdef _SIZE_ERROR_MESSAGE
		if(siDatasize != sizeof(int))
		{
			char err[2048] = "";
			wsprintf( err, "ParamNo = %d, RequireSize = SQL_INTEGER(4Byte), InputSize = %d\n%s\n", siParamNo, siDatasize, m_pszQuery);
			BOOL bServer = REGULAR_SERVER ;
			if ( !bServer )
				MessageBox(NULL, err, "데이터 타입이 틀리다", MB_OK);
		}
#endif
		nValue = *(int*)pData;
		m_pkParamList->add(nValue);
		PrintParam(buffer);
		break;
	case SQL_BIGINT:
		_i64toa( *((int64_t*)(pData)), buffer, 10);
#ifdef _SIZE_ERROR_MESSAGE
		if(siDatasize != sizeof(__int64))
		{
			char err[2048] = "";
			wsprintf( err, "ParamNo = %d, RequireSize = SQL_BIGINT(8Byte), InputSize = %d\n%s\n", siParamNo, siDatasize, m_pszQuery);
			BOOL bServer = REGULAR_SERVER ;
			if ( !bServer )
				MessageBox(NULL, err, "데이터 타입이 틀리다", MB_OK);
		}
#endif
		lValue = *(int64_t*)pData;
		m_pkParamList->add(lValue);
		PrintParam(buffer);
		break;
	case SQL_REAL:		// SQL에선 FLOAT 임 (4 Byte)
		wsprintf(buffer, "%.2f", *((float*)(pData)));
		buffer[sizeof(buffer)-1] = '\0';
#ifdef _SIZE_ERROR_MESSAGE
		if(siDatasize != sizeof(float))
		{
			char err[2048] = "";
			wsprintf( err, "ParamNo = %d, RequireSize = SQL_REAL(4Byte), InputSize = %d\n%s\n", siParamNo, siDatasize, m_pszQuery);
			BOOL bServer = REGULAR_SERVER ;
			if ( !bServer )
				MessageBox(NULL, err, "데이터 타입이 틀리다", MB_OK);
		}
#endif
		fValue = *(float*)pData;
		m_pkParamList->add(fValue);
		PrintParam(buffer);
		break;
	case SQL_FLOAT:		// SQL에선 DOUBLE 임 (8 Byte)
		wsprintf(buffer, "%.2f", *((double*)(pData)));
		buffer[sizeof(buffer)-1] = '\0';
#ifdef _SIZE_ERROR_MESSAGE
		if(siDatasize != sizeof(double))
		{
			char err[2048] = "";
			wsprintf( err, "ParamNo = %d, RequireSize = SQL_FLOAT(8Byte), InputSize = %d\n%s\n", siParamNo, siDatasize, m_pszQuery);
			BOOL bServer = REGULAR_SERVER ;
			if ( !bServer )
				MessageBox(NULL, err, "데이터 타입이 틀리다", MB_OK);
		}
#endif
		dValue = *(double*)pData;
		m_pkParamList->add(dValue);
		PrintParam(buffer);
		break;
	case SQL_BINARY:
		m_pkParamList->add((const void*)pData, siDatasize);
		PrintParam("binary data");
		break;
	}
}


void DBGW::PrintQuery(char *msg)
{
	ZeroMemory(m_cErrQuery, sizeof(m_cErrQuery));
	sprintf(m_cErrQuery, "%s\n", msg);
}

void DBGW::PrintErrMsg(char *msg)
{
	strcat(m_cErrQuery, "\n\t");
	strcat(m_cErrQuery, msg);
	strcat(m_cErrQuery, "\n");
}

void DBGW::PrintParam(char *msg)
{
	strcat(m_cErrQuery, "\t\t");
	strcat(m_cErrQuery, msg);
	strcat(m_cErrQuery, "\n");
}

void DBGW::WriteQuery()
{
	SYSTEMTIME CT;
	GetLocalTime(&CT);
	
	char szFileName[256] = "";
	sprintf(szFileName, "ErrorQuery_%02d%02d%02d.log", CT.wYear, CT.wMonth, CT.wDay);

	m_Queryfp = fopen(szFileName, "a+");

	//fprintf(m_Queryfp, "\n%02d/%02d/%02d %02d:%02d:%02d %04d\n%s", CT.wYear, CT.wMonth, CT.wDay, CT.wHour, CT.wMinute, CT.wSecond, CT.wMilliseconds, m_cErrQuery);
	fprintf(m_Queryfp, "\n%02d/%02d/%02d %02d:%02d:%02d %04d\n%s\n%s", CT.wYear, CT.wMonth, CT.wDay, CT.wHour, CT.wMinute, CT.wSecond, CT.wMilliseconds, m_pszRspName, m_pkResult->errorMessage());
	fclose(m_Queryfp);
}

//*/////////////////////////////////////////////////////////
void DBGW::BeginSP(char *pszQuery)
{
	Prepare();

	m_siParamNo = 0;

	strcpy(m_pszRspName, pszQuery);

	strcpy(m_pszQuery, "{call ");
	strcat(m_pszQuery, pszQuery);

	PrintQuery(pszQuery);
}

void DBGW::EndSP()
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

void DBGW::SetParam(int siType, void *pData, int siDatasize, SQLSMALLINT siParamType)
{
	if( m_siParamNo == 0 )
		strcat( m_pszQuery, " ( ?" );
	else
		strcat( m_pszQuery, ", ?" );

	SetParam( ++m_siParamNo, siType, pData, siDatasize, siParamType );
}

// INT : 1 BYTE
int DBGW::GetData(signed char* psiData, long siDataSize /* = 0 */)
{
	return GetData( m_siCurCol++, psiData, sizeof(signed char));
}

int DBGW::GetData(unsigned char* puiData, long siDataSize /* = 0 */)
{
	SI32 siDataLen = siDataSize;
	if(siDataLen == 0)	// UINT : 1 BYTE
	{
		siDataLen = sizeof(unsigned char);
	}
	return GetData( m_siCurCol++, puiData, siDataLen);
}

// INT : 2 BYTE
int DBGW::GetData(short* psiData, long siDataSize /* = 0 */)
{
	return GetData( m_siCurCol++, psiData, sizeof(short));
}

int DBGW::GetData(unsigned short* puiData, long siDataSize /* = 0 */)
{
	return GetData( m_siCurCol++, puiData, sizeof(unsigned short));
}

// INT : 4 BYTE
int DBGW::GetData(int* psiData, long siDataSize /* = 0 */)
{
	return GetData( m_siCurCol++, psiData, sizeof(int));
}

int DBGW::GetData(unsigned int* puiData, long siDataSize /* = 0*/)
{
	return GetData( m_siCurCol++, puiData, sizeof(unsigned int));
}

// INT : 8 BYTE
int DBGW::GetData(int64_t* psiData, long siDataSize /* = 0 */)
{
	return GetData( m_siCurCol++, psiData, sizeof(int64_t));
}

int DBGW::GetData(uint64_t* puiData, long siDataSize /* = 0 */)
{
	return GetData( m_siCurCol++, puiData, sizeof(uint64_t));
}

// BOOL : 4 BYTE
int DBGW::GetData(bool* pbData, long siDataSize /* = 0 */)
{
	return GetData( m_siCurCol++, pbData, sizeof(bool));
}

// FLOAT : 4 BYTE
int DBGW::GetData(float* pfData, long siDataSize /* = 0 */)
{
	return GetData( m_siCurCol++, pfData, sizeof(float));
}

// double : 8 BYTE
int DBGW::GetData(double* pdData, long siDataSize /* = 0 */)
{
	return GetData( m_siCurCol++, pdData, sizeof(double));
}

// STRING
int DBGW::GetData(char* pcData, long siDataSize)
{
	return GetData( m_siCurCol++, pcData, siDataSize);
}

//// BINARY
//int DBGW::GetData(void* pvData, long siDataSize)
//{
//	return GetData( m_siCurCol++, pvData, siDataSize);
//}

// cltDate : 4 BYTE
int DBGW::GetData(cltDate* pdData, long siDataSize /* = 0 */)
{
	return GetData( m_siCurCol++, pdData, sizeof(cltDate));
}

// cltMoney
int DBGW::GetData(cltMoney* pclMoney, long siDataSize /* = 0 */)
{
	return GetData( m_siCurCol++, pclMoney, sizeof(cltMoney));
}

// TIMESTAMP_STRUCT
int DBGW::GetData(TIMESTAMP_STRUCT* pclTimeStamp, long siDataSize /* = 0 */)
{
	return GetData( m_siCurCol++, pclTimeStamp, sizeof(TIMESTAMP_STRUCT));
}

/////////////////////////////////////////////////////////*/

// ----------------------------------------------------------------------------------------------
// HANGAME DBGW 사용시에만 컴파일이 된다.
// ----------------------------------------------------------------------------------------------
#endif //USE_DBGW
// ----------------------------------------------------------------------------------------------
