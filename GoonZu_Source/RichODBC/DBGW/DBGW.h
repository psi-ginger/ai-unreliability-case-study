// DBGW.h: interface for the RichODBC class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(DBGW_H)
#define DBGW_H

#include <windows.h>
#include <sql.h>
#include <sqlext.h>
#include <stdio.h>
#include <Sqltypes.h>

#include "../../GoonZuWorld/common/CommonHeader.h"
#include "../RPCClient/rpcclient.h"

// ----------------------------------------------------------------------------------------------
// HANGAME DBGW 사용시에만 컴파일이 된다.
// ----------------------------------------------------------------------------------------------
#ifdef USE_DBGW
// ----------------------------------------------------------------------------------------------

	#ifdef _DEBUG
		#pragma comment(lib, "NanoLibraryD_MT.lib") 
	#else
		#pragma comment(lib, "NanoLibrary_MT.lib") 
	#endif

// ----------------------------------------------------------------------------------------------
// HANGAME DBGW 사용시에만 컴파일이 된다.
// ----------------------------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------------------------

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define MAX_QUERY_SIZE_DBGW	1024

class DBGW
{
public:
	DBGW();
	~DBGW();

	void		InitDBGW();
	void		ReleaseDBGW();

	VOID		SetParam(int siParamNo, int siType, void *pData, int siDatasize, SQLSMALLINT siParamType = SQL_PARAM_INPUT);
	//int			GetData(int iColNo, int iType, void *pvData, long siDataSize);
	int			GetData(int iColNo, void *pvData, long siDataSize);
public:

private:
	BOOL		bReconnectStat;
	BOOL		bCanFetchRow;

public:
	void		inline SetReconnect(BOOL bStat) {	bReconnectStat = bStat;		}
	BOOL		inline GetReconnectStat()		{	return bReconnectStat;		}

	BOOL		Connect(char *cDriver, char *cServer, char *cDatabase, char *cUserID, char *cPasswd);
	BOOL		ConnectProcess();
	BOOL		Reconnect();
	int			Disconnect();

	VOID		Prepare();
	int			ExecSQL();

	int			FetchData();
	int			NextRecordSet();

	int			SetQuery(char *pszQuery);

	//////////////////////////////////////////////////////////
	void		BeginSP(char *pszQuery);
	void		EndSP();
	void		SetParam(int siType, void *pData, int siDatasize, SQLSMALLINT siParamType = SQL_PARAM_INPUT);

	// 데이터 타입 세분화
	//int			GetData(void *pvData, long siDataSize);

	// INT : 1 BYTE
	int			GetData(signed char* psiData, long siDataSize = 0);
	// UINT : 1 BYTE or BINARY
	int			GetData(unsigned char* puiData, long siDataSize = 0);

	// INT : 2 BYTE
	int			GetData(short* psiData, long siDataSize = 0);
	int			GetData(unsigned short* puiData, long siDataSize = 0);

	// INT : 4 BYTE
	int			GetData(int* psiData, long siDataSize = 0);
	int			GetData(unsigned int* puiData, long siDataSize = 0);

	// INT : 8 BYTE
	int			GetData(int64_t* psiData, long siDataSize = 0);
	int			GetData(uint64_t* puiData, long siDataSize = 0);

	// BOOL : 4 BYTE
	int			GetData(bool* pbData, long siDataSize = 0);

	// FLOAT : 4 BYTE
	int			GetData(float* pfData, long siDataSize = 0);
	// DOUBLE : 8 BYTE
	int			GetData(double* pdData, long siDataSize = 0);

	// STRING
	int			GetData(char* pcData, long siDataSize);

	// cltDate
	int			GetData(cltDate* pclDate, long siDataSize = 0);

	// cltMoney
	int			GetData(cltMoney* pclMoney, long siDataSize = 0);

	// TIMESTAMP_STRUCT
	int			GetData(TIMESTAMP_STRUCT* pclTimeStamp, long siDataSize = 0);

	/////////////////////////////////////////////////////////*/
	
	BOOL		CheckMDAC(char *cRequire);
	BOOL		CheckError(SQLSMALLINT hType, SQLHANDLE handle,char* hint=NULL, HWND hwnd = NULL);
	BOOL		IsSuccess(void);

	int			m_siParamNo;
	SQLINTEGER	m_siParamSize;

protected:
	mega::IRPCConnector*	m_pkRPCConnector;
	mega::IRPCClient*		m_pkRPCClient;

	mega::IList*			m_pkParamList;
	mega::IRPCResult*		m_pkResult;

	int						m_siLastTable;
	int						m_siCurRow;
	int						m_siCurCol;

	mega::IList*			m_pkRetList;
	mega::ITable*			m_pkTable;

	char		m_cConnectionString[1024];
	char		*m_pszQuery;
	char		*m_pszRspName;

private:
	long		m_lbytes;
	BOOL		IsPooling;
	
private:
	FILE		*m_Queryfp;
	char		m_cErrQuery[8192];
	void		PrintErrMsg(char *msg);
	void		PrintQuery(char *msg);
	void		PrintParam(char *msg);
	void		WriteQuery();
};

#endif // !defined(DBGW_H)
