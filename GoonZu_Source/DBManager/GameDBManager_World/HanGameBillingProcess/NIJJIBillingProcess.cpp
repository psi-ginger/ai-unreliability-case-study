//=============================================================================
//
//	NIJJIBillingProcess.cpp
//
//=============================================================================
#include "NIJJIBillingProcess.h"

NIJJIBillingProcess::NIJJIBillingProcess()
{
	m_coin = NULL;
	m_hdll = NULL;

	m_bTest = false;

}

NIJJIBillingProcess::~NIJJIBillingProcess()
{
	delete m_coin;
	FreeLibrary(m_hdll);
}

void NIJJIBillingProcess::Init( TCHAR *pszIP, UI16 usPort, UI32 uiTimeOut )
{ 	
	//---------------------------------------------------
	// 미국 서비스가 아니면 아무것도 하지 않는다.
#ifndef _USA
	return;
#endif
	//---------------------------------------------------

	if(m_hdll != NULL)			return;
	if(m_coin != NULL)			return;

	if (_tcscmp(pszIP, "TEST") == 0)
	{
		// 테스트 모드
		m_bTest = true;

		printf("=============================\n");
		printf("Test Billing Mode \n");
		printf("=============================\n");

		return;
	}

	// DLL 로딩
	m_hdll = LoadLibrary(DLL_LIBRARY);
	printf("=============================\n");
	if (m_hdll == NULL)
	{
		printf("Can't Load Library - %s\n", DLL_LIBRARY);
		return;
	}
	else
	{
		printf("Load Library Ok - %s\n", DLL_LIBRARY);
	}
	printf("=============================\n");


	// 클래스 인스턴스 생성함수 로딩
	PFunc func = (PFunc)GetProcAddress(m_hdll, "CreateNHNCoinInstance");
	printf("=============================\n");
	if (func == NULL)
	{
		printf("Can't Load Function - CreateNHNCoinInstance\n" );
		return;
	}
	else
	{
		printf("Load Function Ok - CreateNHNCoinInstance\n" );
	}
	printf("=============================\n");

	// 클래스 인스턴스 생성
	m_coin = (func)(pszIP, usPort, uiTimeOut);
	printf("=============================\n");
	if (m_coin == NULL)
	{
		printf("Fail to Create Instance - NHNCoin(IP:%s, Port:%d, TimeOut:%d) \n", pszIP, usPort, uiTimeOut );
		return;
	}
	else
	{
		printf("Create Instance Ok - NHNCoin(IP:%s, Port:%d, TimeOut:%d) \n", pszIP, usPort, uiTimeOut );
	}
	printf("=============================\n");
}

//[진성] 한게임 코인.  
bool NIJJIBillingProcess::GetBillingBalance(TCHAR* pszAccountID, SI32 siIDNo, SI32* psiToTalCoin )
{
	if(m_coin == NULL)
	{
		// 에러 로그
		TCHAR szMsg[2048];
		StringCchPrintf(szMsg, sizeof(szMsg), TEXT("m_coin == NULL") );
		WriteErrorLog(__FILE__,__LINE__, szMsg);
		return false;
	}

	char msg[1024] = {0};
	NHNCoin::RET_CODE retCode;
	int rtn = 0, totbal = 0;

	// IN
	TCHAR szIDNo[32];
	StringCchPrintf(szIDNo, sizeof(szIDNo), TEXT("%d"), siIDNo);

	// Process 로그
	TCHAR szMsg[2048];
	StringCchPrintf(szMsg, sizeof(szMsg), TEXT("m_coin->queryCoin() AccountID:%s, IDNo:%s"), pszAccountID, szIDNo );
	WriteProcessLog(__FILE__,__LINE__, szMsg);

	int siHanCoin, siInnerEvent, siOuterEvent, siGiftCard ;
	retCode = m_coin->queryCoin(pszAccountID, szIDNo, &rtn, msg, psiToTalCoin, &siHanCoin, &siInnerEvent, &siOuterEvent,  &siGiftCard);

	if(rtn == 0)
	{
		return true;
	}

	// 에러 로그
	StringCchPrintf(szMsg, sizeof(szMsg), TEXT("ID: %s, IDNo: %s, Code: %d, Msg: %s"), pszAccountID, szIDNo, rtn, msg);
	WriteErrorLog(__FILE__,__LINE__, szMsg);
}

bool NIJJIBillingProcess::GetBillingBalance(TCHAR* pszAccountID, SI32 siIDNo, SI32* psiHanCoin, SI32* psiGiftCard, SI32* psiInnerEvent, SI32* psiOuterEvent)
{
	// 테스트 모드
	if( m_bTest )
	{
		*psiHanCoin = 100000;
		return true;
	}

	if(m_coin == NULL)
	{
		// 에러 로그
		TCHAR szMsg[2048];
		StringCchPrintf(szMsg, sizeof(szMsg), TEXT("m_coin == NULL") );
		WriteErrorLog(__FILE__,__LINE__, szMsg);
		return false;
	}

	char msg[1024] = {0};
	NHNCoin::RET_CODE retCode;
	int rtn = 0, totbal = 0;

	// IN
	TCHAR szIDNo[32];
	StringCchPrintf(szIDNo, sizeof(szIDNo), TEXT("%d"), siIDNo);

	// Process 로그
	TCHAR szMsg[2048];
	StringCchPrintf(szMsg, sizeof(szMsg), TEXT("m_coin->queryCoin() AccountID:%s, IDNo:%s"), pszAccountID, szIDNo );
	WriteProcessLog(__FILE__,__LINE__, szMsg);

	retCode = m_coin->queryCoin(pszAccountID, szIDNo, &rtn, msg, &totbal, psiHanCoin, psiInnerEvent, psiOuterEvent, psiGiftCard);

	if(rtn == 0)
	{
		return true;
	}

	// 에러 로그
	StringCchPrintf(szMsg, sizeof(szMsg), TEXT("ID: %s, IDNo: %s, Code: %d, Msg: %s"), pszAccountID, szIDNo, rtn, msg);
	WriteErrorLog(__FILE__,__LINE__, szMsg);

	return false;
}


bool NIJJIBillingProcess::ChargeItem(TCHAR * pszAccountID , SI32 siIDNo, TCHAR* pszIP, SI32 siProductKey, TCHAR* pszProductName, SI32 siPrice , SI32 siProductNum )
{
	// 테스트 모드
	if( m_bTest )
	{
		return true;
	}

	if(m_coin == NULL)
	{
		// 에러 로그
		TCHAR szMsg[2048];
		StringCchPrintf(szMsg, sizeof(szMsg), TEXT("m_coin == NULL") );
		WriteErrorLog(__FILE__,__LINE__, szMsg);
		return false;
	}

	char sid[32] = {0};
	int rtn = 0, bal = 0;
	char msg[1024] = {0};

	// IN
	TCHAR szIDNo[32];
	StringCchPrintf(szIDNo, sizeof(szIDNo), TEXT("%d"), siIDNo);

	// IN - "0006" 으로 USA와 협의된 데이터임.
	TCHAR szProductKind[32];
	StringCchPrintf(szProductKind, sizeof(szProductKind), TEXT("0006") );

	// IN
	TCHAR szProductKey[32];
	StringCchPrintf(szProductKey, sizeof(szProductKey), TEXT("%d"), siProductKey);

	// IN
	TCHAR szRefKey[32];
	StringCchPrintf(szRefKey, sizeof(szRefKey), TEXT("0"));

	// OUT
	TCHAR szSID[20] = {0};

	// Process 로그
	TCHAR szMsg[2048];
	StringCchPrintf(szMsg, sizeof(szMsg), TEXT("m_coin->useCoin() AccountID:%s, IDNo:%s"), pszAccountID, szIDNo );
	WriteProcessLog(__FILE__,__LINE__, szMsg);

	NHNCoin::RET_CODE retCode = m_coin->useCoin(pszAccountID, szIDNo, szProductKind, szProductKey, pszProductName, siPrice, szRefKey, pszIP, szSID, &rtn, &bal, msg);

	if(rtn == 0)
	{
		return true;
	}

	// 에러 로그
	StringCchPrintf(szMsg, sizeof(szMsg), TEXT("ID: %s, IDNo: %s, Code: %d, Msg: %s"), pszAccountID, szIDNo, rtn, msg);
	WriteErrorLog(__FILE__,__LINE__, szMsg);

	return false;
}

//[진성] 선물주기. => 2008-7-23
BOOL NIJJIBillingProcess::GiftItem(TCHAR * pszGiftSendID, TCHAR * pszGiftRecvID , SI32 siIDNo, TCHAR* pszIP, SI32 siProductKey, TCHAR* pszProductName, SI32 siPrice , SI32 siProductNum )
{
	// 테스트 모드
	if( m_bTest )
	{
		return true;
	}

	if(m_coin == NULL)
	{
		// 에러 로그
		TCHAR szMsg[2048];
		StringCchPrintf(szMsg, sizeof(szMsg), TEXT("m_coin == NULL") );
		WriteErrorLog(__FILE__,__LINE__, szMsg);
		return false;
	}

	char sid[32] = {0};
	int rtn = 0, bal = 0;
	char msg[1024] = {0};

	// IN
	TCHAR szIDNo[32];
	StringCchPrintf(szIDNo, sizeof(szIDNo), TEXT("%d"), siIDNo);

	// IN - "0006" 으로 USA와 협의된 데이터임.
	TCHAR szProductKind[32];
	StringCchPrintf(szProductKind, sizeof(szProductKind), TEXT("0006") );

	// IN
	TCHAR szProductKey[32];
	StringCchPrintf(szProductKey, sizeof(szProductKey), TEXT("%d"), siProductKey);

	// IN
	TCHAR szRefKey[32];
	StringCchPrintf(szRefKey, sizeof(szRefKey), TEXT("0"));

	// OUT
	TCHAR szSID[20] = {0};

	// Process 로그
	TCHAR szMsg[2048];
	StringCchPrintf(szMsg, sizeof(szMsg), TEXT("m_coin->useCoin() AccountID:%s, IDNo:%s"), pszGiftSendID, szIDNo );
	WriteProcessLog(__FILE__,__LINE__, szMsg);

	NHNCoin::RET_CODE retCode = m_coin->useCoinGift(pszGiftSendID, szIDNo, szProductKind, szProductKey, pszProductName, siPrice, szRefKey, pszIP, szSID, &rtn, &bal, msg);

	//[진성] ijji의 선물 가능 조건을 만족하지 않음-선물회수초과 또는 최근구매기록부족 => 2008-7-23
	if(rtn == 0)
	{
		return TRUE;
	}

	// 에러 로그
	StringCchPrintf(szMsg, sizeof(szMsg), TEXT("ID: %s, IDNo: %s, Code: %d, Msg: %s"), pszGiftSendID, szIDNo, rtn, msg);
	WriteErrorLog(__FILE__,__LINE__, szMsg);

	return rtn;
}

void NIJJIBillingProcess::WriteProcessLog(TCHAR* pszSource, int nLine , TCHAR * pszMsg)
{
	if( pszMsg == NULL) return;

	SYSTEMTIME CT;
	GetLocalTime(&CT);

	TCHAR szFileName[256] = "";
	sprintf(szFileName, "ProcessLog_%02d%02d%02d.txt", CT.wYear, CT.wMonth, CT.wDay);

	FILE * fp = NULL;

	fp = fopen (szFileName , "a+");

	if( fp == NULL ) return;

	fprintf(fp , "%2d-%2d-%2d\t%s\t%4d\t%s\n" , CT.wHour , CT.wMinute , CT.wSecond , pszMsg  , nLine, pszSource );

	fclose(fp);

}
void NIJJIBillingProcess::WriteErrorLog(TCHAR* pszSource, int nLine , TCHAR * pszMsg)
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