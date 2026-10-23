#include <Windows.h>
#include <stdio.h>

typedef int (*InitIoVation)( const char* pszSubScriberID, const char* pszAdminCode, const char* pszAdminPassword, const char* pszType );
typedef int (*ShutdownIoVation)( void );
typedef int (*CheckTransantion)( const char* pszURL, const char* pszID, const char* pszBlackBox, const char* pszIP );

typedef const char* (*GetLastErrorStr)( void );
typedef const char* (*GetReasonStr)( void );
typedef const char* (*GetDeviceAliasStr)( void );
typedef const char* (*GetTrackingNumberStr)( void );

#include "StmOCX.tlh"

#include <comutil.h>
#include <tchar.h>
#include <atlcomcli.h>
#include <stdio.h>

typedef HRESULT (*RegProto)();
typedef HRESULT (*UnRegProto)();

bool registerIovation()
{
	bool bRet = false;
	HRESULT hr;

	// register the ActiveX dll
	HMODULE hDLL = LoadLibrary( "StmOCX.dll" );
	if ( hDLL )
	{
		RegProto regFn = (RegProto) GetProcAddress( hDLL, "DllRegisterServer" );
		if ( regFn )
		{
			hr = regFn();
			if( SUCCEEDED(hr) )		bRet = true;
		}

		FreeLibrary( hDLL );
	}

	return bRet;
}

bool unregisterIovation()
{
	bool bRet = false;
	HRESULT hr;

	// register the ActiveX dll
	HMODULE hDLL = LoadLibrary( "StmOCX.dll" );
	if ( hDLL )
	{
		UnRegProto unregFn = (UnRegProto) GetProcAddress( hDLL, "DllUnregisterServer" );
		if ( unregFn )
		{
			hr = unregFn();
			if( SUCCEEDED(hr) )		bRet = true;
		}

		FreeLibrary( hDLL );
	}

	return bRet;
}

int main()
{
	registerIovation();

	//////////////////////////////////////////////////////////
	CoInitialize(NULL);		// 게임에 적용할땐 이 코드는 빠진다.
	//////////////////////////////////////////////////////////

	// 1. 블랙박스 얻기
	HRESULT hr;
	CComPtr<IStm>	clIStm;

	char	szBlackBox[2048] = { '\0', };

	hr = clIStm.CoCreateInstance( CLSID_Stm );
	if( SUCCEEDED(hr) )
	{
		//BSTR	bstrVersion;
		//bstrVersion = clIStm->ioVersion();

		BSTR	bstrBlackbox;
		bstrBlackbox = clIStm->ioBegin();

		int len = ::WideCharToMultiByte(CP_ACP, 0, bstrBlackbox, wcslen(bstrBlackbox), szBlackBox, 2048, NULL, NULL);
		printf("BlackBox String -----\n%s\n", szBlackBox);

		if(clIStm)		clIStm.Release();
	}

	// 2. IOVATION 인증 요청
	InitIoVation			InitFunc				= NULL;
	ShutdownIoVation		ShutdownFunc			= NULL;
	CheckTransantion		CheckTransactionFunc	= NULL;

	GetLastErrorStr			GetLastError			= NULL;
	GetReasonStr			GetReason				= NULL;
	GetDeviceAliasStr		GetDeviceAlias			= NULL;
	GetTrackingNumberStr	GetTrackingNumber		= NULL;

	int nRet = 0;
	HMODULE hIOVATIONDLL = LoadLibrary( "IOVATION.dll" );
	if( hIOVATIONDLL )
	{
		InitFunc				= (InitIoVation)GetProcAddress(			hIOVATIONDLL, "Init_IOVATION" );
		ShutdownFunc			= (ShutdownIoVation)GetProcAddress(		hIOVATIONDLL, "Shutdown_IOVATION" );
		CheckTransactionFunc	= (CheckTransantion)GetProcAddress(		hIOVATIONDLL, "CheckTransaction" );

		GetLastError			= (GetLastErrorStr)GetProcAddress(		hIOVATIONDLL, "GetLastErrorString" );
		GetReason				= (GetReasonStr)GetProcAddress(			hIOVATIONDLL, "GetReasonString" );
		GetDeviceAlias			= (GetDeviceAliasStr)GetProcAddress(	hIOVATIONDLL, "GetDeviceAliasString" );
		GetTrackingNumber		= (GetTrackingNumberStr)GetProcAddress( hIOVATIONDLL, "GetTrackingNumberString" );

		// Test 서버용
		nRet = InitFunc("220702", "OLTP", "AYHFUS1D", "default");
		//nRet = InitFunc("220702", "OLTP", "AYHFUS1D", "game_login_L");

		// Real 서버용
		//nRet = InitFunc("200700", "OLTP", "MV6LWVVR", "default");
		//nRet = InitFunc("200700", "OLTP", "MV6LWVVR", "game_login_L");

		printf( "InitFunc Ret : %d\n", nRet );
		
		// Test 서버용
		nRet = CheckTransactionFunc("https://ci-snare.iovation.com/api/CheckTransaction", "ndilu002", szBlackBox, "222.107.206.108");

		// Real 서버용
		//nRet = CheckTransactionFunc("https://soap.iovation.com/api/CheckTransaction", "ndilu002", szBlackBox, "222.107.206.108");

		printf( "CheckTransactionFunc Ret : %d\n", nRet );

		printf( "Reason : %s\n", GetReason() );
		printf( "DeviceAlias : %s\n", GetDeviceAlias() );
		printf( "TrackingNumber : %s\n", GetTrackingNumber() );

		printf( "LastError : %s\n", GetLastError() );

		nRet = ShutdownFunc();
		printf( "ShutdownFunc Ret : %d\n", nRet );
	}

	if( hIOVATIONDLL )		FreeLibrary( hIOVATIONDLL );

	//////////////////////////////////////////////////////////
	CoUninitialize();		// 게임에 적용할땐 이 코드는 빠진다.
	//////////////////////////////////////////////////////////

	unregisterIovation();

	return 0;
}