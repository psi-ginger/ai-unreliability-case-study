//=============================================================================
//
//	NHanGameBillingProcess.cpp
//
//=============================================================================
#include "NHanGameBillingProcess.h"

NHanGameBillingProcess::NHanGameBillingProcess()
{
	ZeroMemory(m_szIP, sizeof(m_szIP));

	m_usPort = 0;
	m_hSocket = INVALID_SOCKET;
}

NHanGameBillingProcess::~NHanGameBillingProcess()
{

}

void NHanGameBillingProcess::Init(char *pszIP, UI16 usPort )
{ 	
	if (_tcscmp(pszIP, "TEST") == 0)
	{
		// 테스트 모드
		m_bTest = true;

		printf("=============================\n");
		printf("Test Billing Mode \n");
		printf("=============================\n");

		return;
	}

	m_szIP[0] = '\0';
	if(pszIP)	_tcsncpy( m_szIP, pszIP, sizeof(m_szIP));
	m_usPort  = usPort;


	ZeroMemory(&m_sockaddr, sizeof(m_sockaddr));
	m_sockaddr.sin_family		= AF_INET;
	//m_sockaddr.sin_addr.s_addr	= inet_addr(m_szIP);
	m_sockaddr.sin_port			= htons(m_usPort);

	
	
	if(isalpha( m_szIP[0] )) 
	{   /* server address is a name */

		struct hostent *hp;

		hp = gethostbyname( m_szIP );

		if (hp == NULL) {
			
			WriteErrorLog(__FILE__,__LINE__, m_szIP );
			WriteErrorLog(__FILE__,__LINE__," fail : gethostbyname ");

			return;
		}

		memcpy( &m_sockaddr.sin_addr.s_addr, hp->h_addr, hp->h_length );

	} 
	else 
	{ 

		m_sockaddr.sin_addr.s_addr = inet_addr( m_szIP );
	} 




	m_hSocket = INVALID_SOCKET;
}

BOOL NHanGameBillingProcess::GetBillingBalance(TCHAR* pszAccountID, SI64* psiHanCoin, SI64* psiGiftCard, SI64* psiInnerEvent, SI64* psiOuterEvent)
{
	// 테스트 모드
	if( m_bTest )
	{
		*psiHanCoin = 100000;
		return true;
	}

	if(pszAccountID == NULL)					return FALSE;

	if(psiHanCoin == NULL)						return FALSE;
	if(psiGiftCard == NULL)						return FALSE;
	if(psiInnerEvent == NULL)					return FALSE;
	if(psiOuterEvent == NULL)					return FALSE;

	// 계정 문자열을 모두 소문자로 변환한다.
	char szLowerAccountID[MAX_PLAYER_NAME] = {0,};
		
	StringCchCopy( szLowerAccountID, MAX_PLAYER_NAME , pszAccountID );

	strlwr( szLowerAccountID );

	// 변환 끝


	// 전문 생성
	char		szSendMsg[64];
	wsprintf(szSendMsg,"%-2s%05d%-2s%-30s", "08", 32, "01", szLowerAccountID);

    // 서버 접속
	if(ConnectServer() == FALSE)
	{
		WriteErrorLog(__FILE__, __LINE__,"fail : GetBillingBalance  ConnectServer()");
		DisconnectServer();
		return FALSE;
	}

	if(WriteMessage(m_hSocket, szSendMsg, 39) == FALSE)
	{
		WriteErrorLog(__FILE__, __LINE__,"fail : GetBillingBalance  WriteMessage()");
		DisconnectServer();
		return FALSE;
	}

	char	szRecvMsg[256];
	if(ReadMessage(m_hSocket, szRecvMsg, 82) == FALSE)
	{
		WriteErrorLog(__FILE__, __LINE__,"fail : GetBillingBalance  ReadMessage()");
		DisconnectServer();
		return FALSE;
	}

	// 서버 접속 해지
	DisconnectServer();

	TCHAR	szReturn[8];
	ZeroMemory(szReturn, sizeof(szReturn));
	_tcsncpy(szReturn, szRecvMsg, 2);

	TCHAR szErrorMsg[1024];

	if(_tcscmp(szReturn, "00") == 0)
	{
		TCHAR	szBuffer[128];
		ZeroMemory(szBuffer, sizeof(szBuffer));
		_tcsncpy(szBuffer, szRecvMsg, sizeof(szBuffer));

		TCHAR* pszHanCoin;
		TCHAR* pszGiftCard;
		TCHAR* pszInnerEvent;
		TCHAR* pszOuterEvent;

		pszHanCoin = strtok(&szBuffer[2], "/");
		pszGiftCard = strtok(NULL, "/");
		pszInnerEvent = strtok(NULL, "/");
		pszOuterEvent = strtok(NULL, "/");

		if(pszHanCoin)		*psiHanCoin		= _atoi64(pszHanCoin);
		if(pszGiftCard)		*psiGiftCard	= _atoi64(pszGiftCard);
		if(pszInnerEvent)	*psiInnerEvent	= _atoi64(pszInnerEvent);
		if(pszOuterEvent)	*psiOuterEvent	= _atoi64(pszOuterEvent);

		return TRUE;

	}
	else if(_tcscmp(szReturn, "01") == 0)
	{
		wsprintf(szErrorMsg, "fail  : GetBillingBalance szReturn 01 - [%s]", szSendMsg);
		WriteErrorLog(__FILE__, __LINE__, szErrorMsg );

		return FALSE;
	}
	else if(_tcscmp(szReturn, "99") == 0)
	{
		wsprintf(szErrorMsg, "fail  : GetBillingBalance szReturn 99 - [%s]", szSendMsg);
		WriteErrorLog(__FILE__, __LINE__, szErrorMsg );

		return FALSE;
	}
	else
	{
		wsprintf(szErrorMsg, "fail  : GetBillingBalance szReturn Unknown - [%s]", szSendMsg);
		WriteErrorLog(__FILE__, __LINE__, szErrorMsg );

		return FALSE;
	}

	return FALSE;
}


BOOL NHanGameBillingProcess::ChargeItem(TCHAR * pszAccountID , SI32 siProductKey , SI32 siPrice , SI32 siProductNum )
{
	// 테스트 모드
	if( m_bTest )
	{
		return true;
	}

	// 파라메터 예외처리 ㄱㄱ


	// 파라메터 예외처리 끝

	// 전문 생성 ㄱㄱ
	char		szSendMsg[256];

	TCHAR		szProductKey[256] = {0,};
	TCHAR		szPrice[256] = {0,};	
	TCHAR		szProductNum[256] = {0,};
	TCHAR		szTime[256] = {0,};


	// 계정 문자열을 모두 소문자로 변환한다.
	char szLowerAccountID[MAX_PLAYER_NAME] = {0,};

	StringCchCopy( szLowerAccountID, MAX_PLAYER_NAME , pszAccountID );

	strlwr( szLowerAccountID );
	///////////////////////////////// 변환끝

	// 프로덕트키 문자열
	sprintf(szProductKey , "%d" , siProductKey);
	// 가격 문자열
	sprintf(szPrice , "%d" , siPrice * siProductNum);
	// 수량 문자열
	sprintf(szProductNum , "%d" , siProductNum);
	
	// 날짜,시각 문자열

	SYSTEMTIME CT;
	GetLocalTime(&CT);
	//                 y   m  d  h  m  s
	sprintf(szTime , "%04d%02d%02d%02d%02d%02d" , CT.wYear , CT.wMonth , CT.wDay , CT.wHour , CT.wMinute , CT.wSecond );

	wsprintf(szSendMsg,"%-2s%05d%-2s%-8s%-30s%-16s%-16s%-10s%-5s%-14s%-2s", "01", 103, "01","NULL", szLowerAccountID,"NULL", szProductKey , szPrice , szProductNum , szTime , "01" );


	// 서버 접속
	if(ConnectServer() == FALSE)
	{
		WriteErrorLog(__FILE__, __LINE__,"fail : ChargeItem  ConnectServer()");
		DisconnectServer();
		return FALSE;
	}

	if(WriteMessage(m_hSocket, szSendMsg, 110) == FALSE)
	{
		WriteErrorLog(__FILE__, __LINE__,"fail : ChargeItem  WriteMessage()");
		DisconnectServer();
		return FALSE;		
	}

	char	szRecvMsg[256];
	if(ReadMessage(m_hSocket, szRecvMsg, 82) == FALSE)
	{
		WriteErrorLog(__FILE__, __LINE__,"fail : ChargeItem  ReadMessage()");
		DisconnectServer();
		return FALSE;
	}

	// 서버 접속 해지
	DisconnectServer();


	TCHAR	szReturn[3];
	ZeroMemory(szReturn, sizeof(szReturn));
	_tcsncpy(szReturn, szRecvMsg, 2);

	TCHAR szErrorMsg[1024];

	if(_tcscmp(szReturn, "00") == 0)
	{

		return TRUE;
	}
	else if(_tcscmp(szReturn, "10") == 0)
	{
		return TRUE;
	}
	else if(_tcscmp(szReturn, "01") == 0)
	{
		wsprintf(szErrorMsg, "fail : ChargeItem szReturn 01 - [%s]", szSendMsg);
		WriteErrorLog(__FILE__, __LINE__, szErrorMsg );
		return FALSE;
	}
	else if(_tcscmp(szReturn, "02") == 0)
	{
		wsprintf(szErrorMsg, "fail : ChargeItem szReturn 02 - [%s]", szSendMsg);
		WriteErrorLog(__FILE__, __LINE__, szErrorMsg );
		return FALSE;
	}
	else if(_tcscmp(szReturn, "03") == 0)
	{
		wsprintf(szErrorMsg, "fail : ChargeItem szReturn 03 - [%s]", szSendMsg);
		WriteErrorLog(__FILE__, __LINE__, szErrorMsg );
		return FALSE;
	}
	else if(_tcscmp(szReturn, "04") == 0)
	{
		wsprintf(szErrorMsg, "fail : ChargeItem szReturn 04 - [%s]", szSendMsg);
		WriteErrorLog(__FILE__, __LINE__, szErrorMsg );
		return FALSE;
	}
	else if(_tcscmp(szReturn, "12") == 0)
	{
		wsprintf(szErrorMsg, "fail : ChargeItem szReturn 12 - [%s]", szSendMsg);
		WriteErrorLog(__FILE__, __LINE__, szErrorMsg );
		return FALSE;
	}
	else if(_tcscmp(szReturn, "99") == 0)
	{
		wsprintf(szErrorMsg, "fail : ChargeItem szReturn 99 - [%s]", szSendMsg);
		WriteErrorLog(__FILE__, __LINE__, szErrorMsg);
		return FALSE;
	}
	else
	{
		wsprintf(szErrorMsg, "fail : ChargeItem szReturn Unknown - [%s]", szSendMsg);
		WriteErrorLog(__FILE__, __LINE__, szErrorMsg );
		return FALSE;
	}

	return FALSE;

}

BOOL NHanGameBillingProcess::ConnectServer()
{
	if(m_hSocket != INVALID_SOCKET)												return FALSE;
	//if(_tcslen(m_szIP) <= 0)													return FALSE;
	//if(m_usPort <= 0)															return FALSE;

	/*
	sockaddr_in		server;
	ZeroMemory(&server, sizeof(server));
	server.sin_family		= AF_INET;
	server.sin_addr.s_addr	= inet_addr(m_szIP);
	server.sin_port			= htons(m_usPort);
	*/

	m_hSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_hSocket == INVALID_SOCKET)
	{
		WriteErrorLog(__FILE__,__LINE__,"fail: socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)");
		return FALSE;
	}

	UI32 ul = 1;
	if(ioctlsocket( m_hSocket, FIONBIO, (unsigned long *)&ul ) == SOCKET_ERROR)		// nonblocking mode로
	{
		WriteErrorLog(__FILE__,__LINE__,"fail: ioctlsocket( m_hSocket, FIONBIO, (unsigned long *)&ul ) == SOCKET_ERROR");

		closesocket(m_hSocket);
		m_hSocket = INVALID_SOCKET;
		return FALSE;
	}

	BOOL	bConnected = FALSE;
	UI32	uiStartConnectTime = GetTickCount();
	while(GetTickCount() - uiStartConnectTime <= HANGAME_BILLING_WAITCONNECT)
	{
		// 소켓 재 생성
		if (m_hSocket == INVALID_SOCKET)
		{
			m_hSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
			if (m_hSocket == INVALID_SOCKET)
			{
				return FALSE;
			}

			ul = 1;
			if(ioctlsocket( m_hSocket, FIONBIO, (unsigned long *)&ul ) == SOCKET_ERROR)		// nonblocking mode로
			{
				closesocket(m_hSocket);
				m_hSocket = INVALID_SOCKET;
				return FALSE;
			}
		}

		if(connect(m_hSocket, (sockaddr*)&m_sockaddr, sizeof(m_sockaddr)) != SOCKET_ERROR)
		{
			bConnected = TRUE;
			break;			// 연결되면 루프 종료
		}

		SI32 siError = WSAGetLastError();

		if (siError == WSAEISCONN)		// 이미 연결된 소켓임...
		{
			bConnected = TRUE;
			break;
		}

		if (siError == WSAEWOULDBLOCK || siError == WSAEALREADY || siError == WSAEINVAL)
		{
			Sleep(50);
			continue;
		}
		else
		{
			if (m_hSocket != INVALID_SOCKET)
			{
				closesocket(m_hSocket);
				m_hSocket = INVALID_SOCKET;
			}
			Sleep(50);
			continue;
		}
	}

	if (m_hSocket != INVALID_SOCKET)
	{
		ul = 0;
		if(ioctlsocket( m_hSocket, FIONBIO, (unsigned long *)&ul ) == SOCKET_ERROR)		// blocking mode로
		{
			closesocket(m_hSocket);
			m_hSocket = INVALID_SOCKET;
			return FALSE;
		}
	}

	if(bConnected == FALSE)
	{
		if(m_hSocket != INVALID_SOCKET)
		{
			closesocket(m_hSocket);
			m_hSocket = INVALID_SOCKET;
		}
		return FALSE;
	}

	return TRUE;
}


BOOL NHanGameBillingProcess::DisconnectServer()
{
	if(m_hSocket == INVALID_SOCKET) return FALSE;

	closesocket(m_hSocket);
	m_hSocket = INVALID_SOCKET;

	return TRUE;

}

SI32 NHanGameBillingProcess::WriteMessage(SOCKET hSocket, TCHAR *pData, SI32 siDataLength)
{
	SI32	siSendedLength = 0, siSend = 0;
	if(m_hSocket == INVALID_SOCKET)				return siSendedLength;
	if(pData == NULL)							return siSendedLength;
	if(siDataLength == 0)						return siSendedLength;

	SI32	siTimeOverMilisecond = HANGAME_BILLING_WAITSOCKET;
	if (setsockopt(hSocket, SOL_SOCKET, SO_SNDTIMEO, (const char*)&siTimeOverMilisecond, sizeof(siTimeOverMilisecond)) == SOCKET_ERROR)
		return siSendedLength;

	UI32 uiStartTick = GetTickCount();
	siSendedLength = 0;
	siSend = 0;
	while (siSendedLength < siDataLength)
	{
		siSend = send(hSocket, &pData[siSendedLength], siDataLength - siSendedLength, 0);
		if (siSend <= 0)
		{
			if(siSend == SOCKET_ERROR)
			{
				SI32 siError = WSAGetLastError();

				if(siError != WSAETIMEDOUT)		// (500ms) 타임 아웃이면 일단 계속 실행한다. 전체 타임 아웃을 밑에서 처리한다.
				{

					WriteErrorLog(__FILE__,__LINE__," fail : WriteMessage SOCKET_ERROR");
					break;
				}
			}
			else
			{
				break;
			}
		}
		else
			siSendedLength += siSend;

		// TIMEOUT
		if (abs((SI32)(GetTickCount() - uiStartTick)) > HANGAME_BILLING_WAITMESSAGE)
		{
			break;
		}
	}
	return siSendedLength;
}

SI32 NHanGameBillingProcess::ReadMessage(SOCKET hSocket, TCHAR *pData, SI32 siDataLength)
{
	SI32	siReceivedLength = 0, siRecv = 0;
	if(m_hSocket == INVALID_SOCKET)				return siReceivedLength;
	if(pData == NULL)							return siReceivedLength;
	if(siDataLength == 0)						return siReceivedLength;

	SI32	siTimeOverMilisecond = HANGAME_BILLING_WAITSOCKET;
	if (setsockopt(hSocket, SOL_SOCKET, SO_RCVTIMEO, (const char*)&siTimeOverMilisecond, sizeof(siTimeOverMilisecond)) == SOCKET_ERROR)
		return siReceivedLength;

	UI32 uiStartTick = GetTickCount();

	siReceivedLength = 0;
	siRecv = 0;
	while (siReceivedLength < siDataLength)
	{
		siRecv = recv(hSocket, &pData[siReceivedLength], siDataLength - siReceivedLength, 0);
		if (siRecv <= 0)
		{
			if(siRecv == SOCKET_ERROR)
			{
				SI32 siError = WSAGetLastError();
				if(siError != WSAETIMEDOUT)		// (500ms) 타임 아웃이면 일단 계속 실행한다. 전체 타임 아웃을 밑에서 처리한다.
				{
					WriteErrorLog(__FILE__,__LINE__," fail : ReadMessage SOCKET_ERROR");
					break;
				}
			}
			else
			{
				break;
			}
		}
		else
			siReceivedLength += siRecv;

		// TIMEOUT
		if (abs((SI32)(GetTickCount() - uiStartTick)) > HANGAME_BILLING_WAITMESSAGE)
		{
			WriteErrorLog(__FILE__,__LINE__," fail : ReadMessage TIMEOUT");
			break;
		}
	}
	return siReceivedLength;
}

void NHanGameBillingProcess::WriteErrorLog(TCHAR* pszSource, int nLine , TCHAR * pszMsg)
{
	if( pszMsg == NULL) return;

	SYSTEMTIME CT;
	GetLocalTime(&CT);

	TCHAR szFileName[256] = "";
	sprintf(szFileName, "ErrorLog_%02d%02d%02d.txt", CT.wYear, CT.wMonth, CT.wDay);

	FILE * fp = NULL;

	fp = fopen (szFileName , "a+");

	if( fp == NULL ) return;

	fprintf(fp , "%2d-%2d-%2d\t%s\t%4d\t%s\n" , CT.wHour , CT.wMinute , CT.wSecond , pszMsg  , nLine, pszSource );

	fclose(fp);

}