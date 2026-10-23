#include <Windows.h>
#include "cltIOVATION.h"
#include "./iovation/IOVATION.h"

cltIOVATION::cltIOVATION()
{
	HMODULE m_hIOVATIONDLL = LoadLibrary( "IOVATION.dll" );
	if( m_hIOVATIONDLL )
	{
		m_InitFunc				= (InitIoVation)GetProcAddress(			m_hIOVATIONDLL, "Init_IOVATION" );
		m_ShutdownFunc			= (ShutdownIoVation)GetProcAddress(		m_hIOVATIONDLL, "Shutdown_IOVATION" );
		m_CheckTransactionFunc	= (CheckTrans)GetProcAddress(			m_hIOVATIONDLL, "CheckTransaction" );

		m_GetLastError			= (GetLastErrorStr)GetProcAddress(		m_hIOVATIONDLL, "GetLastErrorString" );
		m_GetReason				= (GetReasonStr)GetProcAddress(			m_hIOVATIONDLL, "GetReasonString" );
		m_GetDeviceAlias		= (GetDeviceAliasStr)GetProcAddress(	m_hIOVATIONDLL, "GetDeviceAliasString" );
		m_GetTrackingNumber		= (GetTrackingNumberStr)GetProcAddress( m_hIOVATIONDLL, "GetTrackingNumberString" );

		if( m_InitFunc )
		{
			FILE* fp = fopen("IOVATION_SETTING.txt", "r");
			if( fp )
			{
				char szLine[1024] = {'\0',};
				char szKey[1024] = {'\0',};		NTCHARString1024 kKey;
				char szValue[1024] = {'\0',};
				while( fgets(szLine, sizeof(szLine), fp) != NULL )
				{
					sscanf(szLine, "%s %s", szKey, szValue);		kKey = szKey;
					if		( kKey == "SUBSCRIBERID" )		{		m_kSubScriberID = szValue;		}
					else if	( kKey == "ADMINCODE" )			{		m_kAdminCode = szValue;			}
					else if	( kKey == "ADMINPASSWORD" )		{		m_kAdminPassword = szValue;		}
					else if	( kKey == "URL" )				{		m_kURL = szValue;				}
					else if	( kKey == "TYPE" )				{		m_kType = szValue;				}
				}

				fclose(fp);
			}
			else
			{
				MessageBox(NULL, " IOVATION_SETTING.txt file Open Error", "ERROR", MB_OK);
			}

			m_InitFunc( m_kSubScriberID, m_kAdminCode, m_kAdminPassword, m_kType );
		}
		else
		{
			MessageBox(NULL, " Init_IOVATION Function Pointer Error", "ERROR", MB_OK);
		}
	}
	else
	{
		MessageBox(NULL, " LoadLibrary(IOVATION.dll) Error", "ERROR", MB_OK);
	}
}

cltIOVATION::~cltIOVATION()
{
	if( m_ShutdownFunc )	m_ShutdownFunc();

	if( m_hIOVATIONDLL )	FreeLibrary(m_hIOVATIONDLL);
}

bool cltIOVATION::CheckTransantion(const char* pszID, const char* pszBlackBox, const char* pszIP )
{
	int nRet = 0;
	if( m_CheckTransactionFunc == NULL )	return false;
	if( pszID == NULL )						return false;
	if( pszBlackBox == NULL )				return false;
	if( pszIP == NULL )						return false;

	// IOVATION은 소문자로만 요청한다.
	char  szLowercaseID[64];
	strncpy(szLowercaseID, pszID, sizeof(szLowercaseID));
	strlwr(szLowercaseID);

	nRet = m_CheckTransactionFunc(m_kURL, szLowercaseID, pszBlackBox, pszIP);

	SYSTEMTIME stTime;		GetLocalTime(&stTime);
	TCHAR szFileName[MAX_PATH];		StringCchPrintf(szFileName, sizeof(szFileName), "IOVATION_%04d%02d%02d.log", stTime.wYear, stTime.wMonth, stTime.wDay);
	FILE* fp = fopen(szFileName, "a");

	switch( nRet )
	{
	case IOVATION_RESULT_ALLOW:
		{
			// 아무처리 않함.
		}
		break;
	case IOVATION_RESULT_ERROR:
		{
			// 로그 기록
			if( fp )
			{
				if(	m_GetLastError )	fprintf(fp, "Result:IOVATION_RESULT_ERROR\tID:%s\tBlackBox:%s\tError:%s\n", szLowercaseID, pszBlackBox, m_GetLastError());
				else					fprintf(fp, "Result:IOVATION_RESULT_ERROR\tID:%s\tBlackBox:%s\t\tError:%s\n", szLowercaseID, pszBlackBox, "");
			}

			//IOVATION_RESULT_ALLOW로 강제 변경한다.
			nRet = IOVATION_RESULT_ALLOW;
		}
		break;
	case IOVATION_RESULT_DENY:
		{
			// 로그 기록
			if( fp )
			{
				fprintf( fp, "Result:IOVATION_RESULT_DENY\tID:%s\tReason:%s\tError:%s\n", szLowercaseID, m_GetReason() );
			}
		}
		break;
	case IOVATION_RESULT_REVIEW:
		{
			// 로그 기록
			if( fp )
			{
				fprintf(fp, "Result:IOVATION_RESULT_REVIEW\tID:%s\tError:%s\n", szLowercaseID);
			}
		}
		break;
	}

	if( fp )			fclose( fp );

	return ( nRet == IOVATION_RESULT_ALLOW );
}