// JapanAuth.cpp : DLL 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"

#include <process.h>

#include "JapanAuth.h"
#include "AuthThread.h"
#include "Wininet.h"

#include <comutil.h>
#include <tchar.h>
#include <atlcomcli.h>

#include "C:\Program Files\MSSOAP\Inc\mssoap30.h"
#pragma comment(lib, "C:\\Program Files\\MSSOAP\\Lib\\mssoap30.lib") 
#pragma comment(lib, "Wininet.lib")
#pragma comment(lib, "comsupp.lib")

AuthThread*	g_pAuthThread;
int			g_nThreadCount;
JAPANAUTHCALLBACK	g_Callback;

HANDLE			g_hThread;
BOOL			g_bRun;
AuthInfoQueue	g_AuthInfoQueue;

unsigned __stdcall JapanAuthThreadFunc(void* pParameter);
unsigned __stdcall JapanAuthMainFunc(void* pParameter);

int GamePotAuth(char* pszID, char* pszPW, char* pszIP);
int HttpsPostAuth(TCHAR *pszID, TCHAR *pszPW);
int HttpsPostUsaAuth( AuthInfo* _pAuthInfo );	// [진성] USA 새로운 인증.
void USA_Xml_Decode( char* _pszResultAuthString, TCHAR* pszResult, TCHAR* pszID, TCHAR* pszIP ); //[진성] USA 인증 결과 얻기.

//===========================================================================================
//
//===========================================================================================
BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		{
			TRACEF("DLL_PROCESS_ATTACH\n");
		}
		break;
	case DLL_THREAD_ATTACH:
		{
			//TRACEF("DLL_THREAD_ATTACH\n");
		}
		break;
	case DLL_THREAD_DETACH:
		{
			//TRACEF("DLL_THREAD_DETACH\n");
		}
		break;
	case DLL_PROCESS_DETACH:
		{
			JapanAuthShutdown();
			TRACEF("DLL_PROCESS_DETACH\n");
		}
		break;
	}
	return TRUE;
}

//===========================================================================================
//
//===========================================================================================
JAPANAUTH_API int JapanAuthInit(int nThreadCount, JAPANAUTHCALLBACK CallBack)
{
	g_pAuthThread = new AuthThread[nThreadCount];
	if(g_pAuthThread == NULL)		return 0;

	g_nThreadCount	= nThreadCount;
	g_Callback		= CallBack;

	for(int i=0; i<nThreadCount; i++)
	{
		g_pAuthThread[i].m_bRun = TRUE;
		g_pAuthThread[i].m_hThread = (HANDLE)_beginthreadex(NULL, 0, &JapanAuthThreadFunc, &g_pAuthThread[i], 0, &g_pAuthThread[i].m_nThreadID);
	}

	
	UINT	uiThreadID;
	g_bRun = TRUE;
	g_hThread = (HANDLE)_beginthreadex(NULL, 0, &JapanAuthMainFunc, NULL, 0, &uiThreadID);

	return 1;
}

//===========================================================================================
//
//===========================================================================================
JAPANAUTH_API void JapanAuthShutdown()
{
	if(g_pAuthThread != NULL)
	{
		delete[] g_pAuthThread;
		g_pAuthThread = NULL;
	}

	if( g_hThread != INVALID_HANDLE_VALUE )
	{
		g_bRun = FALSE;
		WaitForSingleObject(g_hThread, 3 * 1000);
		g_hThread = INVALID_HANDLE_VALUE;
	}
}

//===========================================================================================
//
//===========================================================================================
unsigned __stdcall JapanAuthMainFunc(void* pParameter)
{
	AuthInfo	Info;
	BOOL		bRet;
	AuthThread*	pAuthThread = NULL;

	while(g_bRun)
	{
		pAuthThread = NULL;

		// 쉬고 있는 스레드를 찾는다.
		for( int i=0; i<g_nThreadCount; i++ )
		{
			if( g_pAuthThread[i].m_bUse == FALSE )
			{
				pAuthThread = &g_pAuthThread[i];
				break;
			}
		}

		if( pAuthThread != NULL )
		{
			bRet = g_AuthInfoQueue.QueueOut(&Info);

			if(bRet)
			{
				TRACEF("pAuthThread \n");

				pAuthThread->m_bUse		= TRUE;
				pAuthThread->m_AuthInfo = Info;
			}
		}
		
		Sleep(1);
	}

	TRACEF("JapanAuthMainFunc End\n");
	return 0;
}

//===========================================================================================
//
//===========================================================================================
unsigned __stdcall JapanAuthThreadFunc(void* pParameter)
{
	AuthThread*	pAuthThread = (AuthThread*)pParameter;

	CoInitialize(NULL);

	while( pAuthThread->m_bRun )
	{
		if(pAuthThread->m_bUse)
		{
			switch(pAuthThread->m_AuthInfo.m_nAuthType)
			{
			case AUTH_TYPE_GAMEPOT:
				{
					int nRet = GamePotAuth(pAuthThread->m_AuthInfo.m_szID, pAuthThread->m_AuthInfo.m_szPW, pAuthThread->m_AuthInfo.m_szIP);

					if( g_Callback != NULL )
					{
						TRACEF("GamePotAuth - Ret : %d\n", nRet);
						g_Callback(pAuthThread->m_AuthInfo, nRet);
					}
				}
				break;
			case AUTH_TYPE_GAMANIA:
				{
					int nRet = HttpsPostAuth(pAuthThread->m_AuthInfo.m_szID, pAuthThread->m_AuthInfo.m_szPW);

					if( g_Callback != NULL )
					{
						TRACEF("HttpsPostAuth - Ret : %d\n", nRet);
						g_Callback(pAuthThread->m_AuthInfo, nRet);
					}
				}
				break;
			case AUTH_TYPE_USA:
				{
					int nRet = HttpsPostUsaAuth( &pAuthThread->m_AuthInfo );

					if( g_Callback != NULL )
					{
						TRACEF("HttpsPostUsaAuth - Ret : %d\n", nRet);
						g_Callback(pAuthThread->m_AuthInfo, nRet);
					}
				}
				break;
				
			}
			pAuthThread->m_bUse = FALSE;
		}
		Sleep(1);
	}

	CoUninitialize();

	TRACEF("JapanAuthThreadFunc End\n");

	return 0;
}


//===========================================================================================
//
//===========================================================================================
JAPANAUTH_API int JapanAuth_GamepotAuth(AuthInfo Info)	// For 감마니아
{
	Info.m_nAuthType = AUTH_TYPE_GAMEPOT;
	if( g_AuthInfoQueue.QueueIn(Info) == TRUE)
	{
		TRACEF("AUTH_TYPE_GAMEPOT - ID:%s, PW:%s, IP:%s\n", Info.m_szID, Info.m_szPW, Info.m_szIP);
		return 1;
	}

	return 0;
}

//===========================================================================================
//
//===========================================================================================
JAPANAUTH_API int JapanAuth_GamaniaAuth(AuthInfo Info)	// For 감마니아
{
	Info.m_nAuthType = AUTH_TYPE_GAMANIA;
	if( g_AuthInfoQueue.QueueIn(Info) == TRUE)
	{
		TRACEF("AUTH_TYPE_GAMANIA - ID:%s, PW:%s, IP:%s\n", Info.m_szID, Info.m_szPW, Info.m_szIP);
		return 1;
	}

	return 0;
}

JAPANAUTH_API int  USAAuth_NHNAuth(AuthInfo Info)	// [진성] USA 새로운 인증.
{
	Info.m_nAuthType = AUTH_TYPE_USA;
	if( g_AuthInfoQueue.QueueIn( Info ) == TRUE)
	{
		//진성 test.
		printf( "USAAuth_NHNAuth(): %s\n\n", Info.m_Authstring );

		TRACEF("AUTH_TYPE_USA - Auth String:%s \n", Info.m_Authstring);
		return 1;
	}

	return 0;
}


//===========================================================================================
//KHY - 0617 - 일본 채널링 추가. - 감마니아용 인증 함수.
//===========================================================================================
int HttpsPostAuth(TCHAR *pszID, TCHAR *pszPW)	// For 감마니아
{
	if(pszID == NULL || pszPW == NULL)			return -1;

	int bResult = -1;

	HINTERNET	hSession = NULL;
	hSession = ::InternetOpen("AuthTest", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);

	char szPostData[1024] = { '\0', };
	BOOL bRet = FALSE;

	wsprintf(szPostData, "id=%s&pw=%s", pszID, pszPW);

	if(hSession)
	{
		HINTERNET hConnect = ::InternetConnect(hSession,
			"gash.gamania.co.jp",
			INTERNET_DEFAULT_HTTPS_PORT,
			"",
			"",
			INTERNET_SERVICE_HTTP,//urlcomponent.nScheme,
			0,
			0);

		if(hConnect)
		{
			// connected server..
			HINTERNET hObject = ::HttpOpenRequest( hConnect,
				"POST",
				"/acxauth/b2b.auth/authkunshu.ashx",
				HTTP_VERSION,
				"",
				NULL,
				INTERNET_FLAG_SECURE | INTERNET_FLAG_IGNORE_CERT_CN_INVALID | INTERNET_FLAG_IGNORE_CERT_DATE_INVALID,
				0);

			if(hObject)
			{
				// post header
				char szLen[MAX_PATH] = { '\0', };
				char aHeader[1024] = { '\0', };

				wsprintf(szLen, "%d", strlen(szPostData));
				strcat(aHeader, "Accept: text/*\r\n");
				strcat(aHeader, "User-Agent: Mozilla/4.0 (compatible; MSIE 5.0;* Windows NT)\r\n");
				strcat(aHeader, "Content-type: application/x-www-form-urlencoded\r\n");
				strcat(aHeader, "Content-length: ");
				strcat(aHeader, szLen);
				strcat(aHeader, "\r\n\n");

				bRet = ::HttpAddRequestHeaders(hObject,(LPCTSTR)aHeader,-1L,HTTP_ADDREQ_FLAG_ADD);
				bRet = ::HttpSendRequest(hObject, NULL, 0, (LPVOID)szPostData, (DWORD)strlen(szPostData));

				// open object
				char szBuffer[1024] = { '\0', };
				DWORD dwRead = 0;
				char szResult[2048] = { '\0', };

				while (::InternetReadFile(hObject, szBuffer, 1023, &dwRead) && dwRead > 0)
				{
					szBuffer[dwRead] = 0;
					strcat(szResult, szBuffer);
					Sleep(1);
				}

				if(strcmp(szBuffer, "00") == 0)		bResult = 0;
				else if(strcmp(szBuffer, "01") == 0)		bResult = 1;
				else if(strcmp(szBuffer, "10") == 0)		bResult = 10;
				else if(strcmp(szBuffer, "11") == 0)		bResult = 11;
				else if(strcmp(szBuffer, "99") == 0)		bResult = 99;				

				::InternetCloseHandle(hObject);
			}

			::InternetCloseHandle(hConnect);
		}
		::InternetCloseHandle(hSession);
	}

	return bResult;
}

//===========================================================================================TR
//
//===========================================================================================
int GamePotAuth(char* pszID, char* pszPW, char* pszIP)
{
	if(pszID == NULL || pszPW == NULL || pszIP == NULL)		return -1;

	int				bRet = -1; // 실패 
	HRESULT			hr;
	TCHAR			szMethodName[] = "GameLogin";
	LPOLESTR		fszMemberFunc = new OLECHAR[strlen(szMethodName) + 1];

	_bstr_t			bstrWSDLFile = (_T("https://kunshu.postar.jp/api/channeling.asmx?wsdl"));

	DISPPARAMS		dispparams;
	EXCEPINFO		excepinfo;
	VARIANT			returnval;
	unsigned int		uArgErr = 0;

	CComPtr<ISoapClient>	pSoapClient;

	if(fszMemberFunc)		ZeroMemory(fszMemberFunc, sizeof(OLECHAR) * (strlen(szMethodName) + 1));
	::MultiByteToWideChar(CP_ACP, 0, szMethodName, (int)strlen(szMethodName), fszMemberFunc, (int)strlen(szMethodName));

	dispparams.rgvarg = NULL;

	hr = pSoapClient.CoCreateInstance(__uuidof(SoapClient30));
	if( hr == S_OK )
	{
		_bstr_t			empty(_T(""));
		DISPID			dispid;

		hr = pSoapClient->MSSoapInit(bstrWSDLFile, empty, empty, empty);		
		hr = pSoapClient->GetIDsOfNames(IID_NULL, &fszMemberFunc, 1, LOCALE_SYSTEM_DEFAULT, &dispid);

		BSTR		bstrParam_ID;
		BSTR		bstrParam_PW;
		BSTR		bstrParam_IP;
		BSTR		bstrParam_ERRNO;
		BSTR		bstrParam_ERRMSG;

		OLECHAR		oleBuf[1024];

		ZeroMemory(oleBuf, sizeof(OLECHAR) * 1024);
		::MultiByteToWideChar(CP_ACP, 0, pszID, (int)strlen(pszID), oleBuf, (int)strlen(pszID));
		bstrParam_ID = SysAllocString(oleBuf);

		ZeroMemory(oleBuf, sizeof(OLECHAR) * 1024);
		::MultiByteToWideChar(CP_ACP, 0, pszPW, (int)strlen(pszPW), oleBuf, (int)strlen(pszPW));
		bstrParam_PW = SysAllocString(oleBuf);

		ZeroMemory(oleBuf, sizeof(OLECHAR) * 1024);
		::MultiByteToWideChar(CP_ACP, 0, pszIP, (int)strlen(pszIP), oleBuf, (int)strlen(pszIP));
		bstrParam_IP = SysAllocString(oleBuf);

		ZeroMemory(oleBuf, sizeof(OLECHAR) * 1024);
		bstrParam_ERRNO = SysAllocStringLen(oleBuf, 1024);

		ZeroMemory(oleBuf, sizeof(OLECHAR) * 1024);
		bstrParam_ERRMSG = SysAllocStringLen(oleBuf, 1024);

		dispparams.rgvarg				= new VARIANTARG[5];
		dispparams.cArgs				= 5;
		dispparams.cNamedArgs			= 0;
		dispparams.rgdispidNamedArgs	= NULL;

		dispparams.rgvarg[0].vt			= VT_BYREF | VT_BSTR;
		dispparams.rgvarg[0].pbstrVal	= &bstrParam_ERRMSG;

		dispparams.rgvarg[1].vt			= VT_BYREF | VT_BSTR;
		dispparams.rgvarg[1].pbstrVal	= &bstrParam_ERRNO;

		dispparams.rgvarg[2].vt			= VT_BYREF | VT_BSTR;
		dispparams.rgvarg[2].pbstrVal	= &bstrParam_IP;

		dispparams.rgvarg[3].vt			= VT_BYREF | VT_BSTR;
		dispparams.rgvarg[3].pbstrVal	= &bstrParam_PW;

		dispparams.rgvarg[4].vt			= VT_BYREF | VT_BSTR;
		dispparams.rgvarg[4].pbstrVal	= &bstrParam_ID;

		VariantInit(&returnval);

		excepinfo.wCode					= 0;
		excepinfo.bstrSource			= NULL;
		excepinfo.bstrDescription		= NULL;
		excepinfo.bstrHelpFile			= NULL;

		hr = pSoapClient->Invoke(dispid, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &dispparams, &returnval, &excepinfo, &uArgErr);

		if( hr == S_OK )
		{
			if( returnval.vt == VT_BOOL )
			{
				if( returnval.boolVal == VARIANT_TRUE )
				{
					// 인증 성공
					if( wcscmp(bstrParam_ERRNO, L"0") == 0 )
					{
						bRet = 0; //성공 
					}
				}
				else
				{
					// 인증 실패
					if( wcscmp(bstrParam_ERRNO, L"1") == 0 )
					{
						bRet = 1;  					
					}
					else if( wcscmp(bstrParam_ERRNO, L"100") == 0 )
					{
						bRet = 100;  
					}
					else if( wcscmp(bstrParam_ERRNO, L"254") == 0 )
					{
						bRet = 254;  					
					}
					else if( wcscmp(bstrParam_ERRNO, L"255") == 0 )
					{
						bRet = 255;  					
					}
					else
					{
						bRet = -2;
					}

				}
			}
		}

		SysFreeString(bstrParam_ID);
		SysFreeString(bstrParam_PW);
		SysFreeString(bstrParam_IP);
		SysFreeString(bstrParam_ERRNO);
		SysFreeString(bstrParam_ERRMSG);
	}

	if(pSoapClient)				pSoapClient.Release();
	if(dispparams.rgvarg)		delete[] dispparams.rgvarg;

	return bRet;
}


//===========================================================================================
//[진성] USA 인증.
//===========================================================================================
int HttpsPostUsaAuth( AuthInfo* _pAuthInfo )	
{
	if( _pAuthInfo == NULL )	return -1;

	int bResult = -1;

	HINTERNET	hSession = NULL;
	hSession = ::InternetOpen("AuthTest", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);

	char szPostData[4096] = { '\0', };
	BOOL bRet = FALSE;

	memcpy(szPostData, _pAuthInfo->m_Authstring, sizeof(szPostData) );

	if(hSession)
	{
		HINTERNET hConnect = ::InternetConnect(	hSession,
												"channel.ijji.com", // 리얼: "channel.ijji.com", 테섭:"alpha-channel.ijji.com",
												INTERNET_DEFAULT_HTTPS_PORT,
												"",
												"",
												INTERNET_SERVICE_HTTP,//urlcomponent.nScheme,
												0,
												0);

		if(hConnect)
		{
			// connected server..
			HINTERNET hObject = ::HttpOpenRequest(	hConnect,
													"POST",
													"/api/gametoken/validate.nhn?",
													HTTP_VERSION,
													"",
													NULL,
													INTERNET_FLAG_SECURE | INTERNET_FLAG_IGNORE_CERT_CN_INVALID | INTERNET_FLAG_IGNORE_CERT_DATE_INVALID,
													0);

			if(hObject)
			{
				// post header
				char szLen[MAX_PATH] = { '\0', };
				char aHeader[1024]   = { '\0', };

				//맨뒤에 따음표(") 삭제.
				szPostData[strlen(szPostData)-1] = '\0';

				char szToken[5000] = { '\0', };
				strcat(szToken, "gametoken=");
				strcat(szToken, &szPostData[1]);
				strcat(szToken, "&gameid=uc_luminary");

				//인증 토큰 관련 출력.
				printf( "★ %s\n\n", szToken );

				wsprintf(szLen, "%d", strlen(szToken));
				strcat(aHeader, "Accept: text/*\r\n");
				strcat(aHeader, "User-Agent: Mozilla/4.0 (compatible; MSIE 5.0;* Windows NT)\r\n");
				strcat(aHeader, "Content-type: application/x-www-form-urlencoded\r\n");
				strcat(aHeader, "Content-length: ");
				strcat(aHeader, szLen);
				strcat(aHeader, "\r\n\n");

				bRet = ::HttpAddRequestHeaders( hObject, (LPCTSTR)aHeader, -1L, HTTP_ADDREQ_FLAG_ADD );
				bRet = ::HttpSendRequest( hObject, NULL, 0, (LPVOID)szToken, (DWORD)strlen(szToken) );

				// open object
				char szBuffer[1024] = { '\0', };
				DWORD dwRead = 0;
				
				while (::InternetReadFile(hObject, szBuffer, 1023, &dwRead) && dwRead > 0)
				{
					szBuffer[dwRead] = 0;
					Sleep(1);
				}

				//진성 test
				printf( "★ %s\n\n", szBuffer );
				
				TCHAR szResult[32] = {'\0', };
				USA_Xml_Decode( szBuffer, szResult, _pAuthInfo->m_szID, _pAuthInfo->m_szIP );

				//진성 test
				printf( "★ ID: %s\n\n", _pAuthInfo->m_szID );

				if(strcmp(szResult, "0")   == 0)		 
				{
					printf( "@@ szResult: 0\n\n" );
					bResult = 0;
				}
				else if(strcmp(szResult, "100") == 0)	 
				{
					printf( "@@ szResult: 100\n\n" );
					bResult = 100;
				}
				else if(strcmp(szResult, "101") == 0)	 
				{
					printf( "@@ szResult: 101\n\n" );
					bResult = 101;
				}
				else if(strcmp(szResult, "102") == 0)	 
				{
					printf( "@@ szResult: 102\n\n" );
					bResult = 102;
				}
				else if(strcmp(szResult, "120") == 0)	
				{
					printf( "@@ szResult: 120\n\n" );
					bResult = 120;			
				}

				::InternetCloseHandle(hObject);
			}

			::InternetCloseHandle(hConnect);
		}
		::InternetCloseHandle(hSession);
	}

	return bResult;
}

void USA_Xml_Decode(char* _pszResultAuthString, TCHAR* pszResult, TCHAR* pszID, TCHAR* pszIP  )
{
	char	buffer[128] = "";

	char*	pToken1		= NULL;
	char*	pToken2		= NULL;
	int		CopySize	= 0;

	pszResult[0] = '\0';
	pszID[0]	 = '\0';
	pszIP[0]	 = '\0';


	// <responseCode>
	pToken1 = _tcschr( _pszResultAuthString,	'>' );		// <> 치워버려~
	pToken1 = _tcschr( pToken1+1,		'>'	);		// <> 치워버려~
	pToken1 = _tcschr( pToken1+1,	'>' );	pToken1++;	// '>' 다음을 가리키게.
	pToken2 = _tcschr( pToken1,		'<' );
	CopySize = labs(pToken2 - pToken1);
	_tcsncpy( pszResult, pToken1,  CopySize );
	pszResult[CopySize] = NULL;
	if( strcmp(pszResult, "0") == 0 )
	{
		// <responseMessage>
		pToken1 = _tcschr( pToken1,		'>' );				// <> 치워버려~
		pToken1 = _tcschr( pToken1+1,	'>' );	pToken1++;	// '>' 다음을 가리키게.
		pToken2 = _tcschr( pToken1,		'<' );
		strncpy( buffer, pToken1,  labs(pToken2 - pToken1) );

		// <userId>
		pToken1 = _tcschr( pToken1,		'>' );				// <> 치워버려~
		pToken1 = _tcschr( pToken1+1,	'>' );	pToken1++;	// '>' 다음을 가리키게.
		pToken2 = _tcschr( pToken1,		'<' );
		CopySize = labs(pToken2 - pToken1);
		_tcsncpy( pszID, pToken1,  CopySize );
		pszID[CopySize] = NULL;

		// <userUid>
		pToken1 = _tcschr( pToken1,		'>' );				// <> 치워버려~
		pToken1 = _tcschr( pToken1+1,	'>' );	pToken1++;	// '>' 다음을 가리키게.
		pToken2 = _tcschr( pToken1,		'<' );
		_tcsncpy( buffer, pToken1,  labs(pToken2 - pToken1) );

		// <ip>
		pToken1 = _tcschr( pToken1,		'>' );				// <> 치워버려~
		pToken1 = _tcschr( pToken1+1,	'>' );	pToken1++;	// '>' 다음을 가리키게.
		pToken2 = _tcschr( pToken1,		'<' );
		CopySize = labs(pToken2 - pToken1);
		_tcsncpy( pszIP, pToken1,  CopySize );
		pszIP[CopySize] = NULL;

		// <expires>
		pToken1 = _tcschr( pToken1,		'>' );				// <> 치워버려~
		pToken1 = _tcschr( pToken1+1,	'>' );	pToken1++;	// '>' 다음을 가리키게.
		pToken2 = _tcschr( pToken1,		'<' );
		_tcsncpy( buffer, pToken1,  labs(pToken2 - pToken1) );
	}
}

void TRACEF(TCHAR* pszOut, ...)
{
	TCHAR	szBuf[2048];

	va_list	ar;
	va_start(ar, pszOut);
	vsprintf(szBuf, pszOut, ar);
	va_end(ar);

	OutputDebugString(szBuf);
}
