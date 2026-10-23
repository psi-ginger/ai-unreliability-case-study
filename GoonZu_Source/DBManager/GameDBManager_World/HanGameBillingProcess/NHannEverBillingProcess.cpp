//=============================================================================
//
//	NHanGameBillingProcess.cpp
//
//=============================================================================
#include "NHannEverBillingProcess.h"
#include "../../../NLib/NTCHARString.h"
#include <vector>

using namespace std;


NHannEverBillingProcess::NHannEverBillingProcess()
{
}

NHannEverBillingProcess::~NHannEverBillingProcess()
{
}

void NHannEverBillingProcess::Init( TCHAR *pszGameID )
{
	if( pszGameID == NULL )				return;

	//한국을 제외하고는 아무런일도 안한다.
//#ifndef _KOREA
//	return;
//#endif

	return;

	if (_tcscmp(pszGameID, "TEST") == 0)
	{
		// 테스트 모드



		m_bTest = true;
		printf("=============================\n");
		printf("Test Billing Mode \n");
		printf("=============================\n");
		return;
	}

//#if defined(_KOREA)
//
//	SI32 siRet = HannEverBillInit(pszGameID);
//
//	if(siRet == HAN_NEVERBILL_OK)
//	{
//		printf("Use nEverBill - Init Ok... \n");
//	}
//	else
//	{
//		printf("Use nEverBill - Init Fail - GameID :%s, Ret:%d... \n", pszGameID, siRet);
//
//		NTCHARString128		kError( TEXT("HannEverBillInit() Failed Ret: - #ret#, Msg - #error#") );
//		kError.Replace("#ret#", SI32ToString(siRet));
//		kError.Replace("#error#", HannEverBillErrorString(HannEverBillGetLastError()));
//		WriteErrorLog(__FILE__, __LINE__, kError);
//	}
//
//#endif
}

//잔액 가저오기
BOOL NHannEverBillingProcess::GetBillingBalance(TCHAR* pszAccountID, SI64* psiHanCoin, SI64* psiGiftCard, SI64* psiInnerEvent, SI64* psiOuterEvent)
{
	if(m_bTest == true)
	{	
		*psiHanCoin = 100000;
		return TRUE;
	}

//#if defined(_KOREA)
//
//	if(pszAccountID		== NULL)	return FALSE;
//	if(psiHanCoin		== NULL)	return FALSE;
//	if(psiGiftCard		== NULL)	return FALSE;
//	if(psiInnerEvent	== NULL)	return FALSE;
//	if(psiOuterEvent	== NULL)	return FALSE;
//
//	// 계정 문자열을 모두 소문자로 변환한다.
//	char szLowerAccountID[MAX_PLAYER_NAME] = {0,};		
//	StringCchCopy( szLowerAccountID, MAX_PLAYER_NAME , pszAccountID );
//	szLowerAccountID[sizeof(szLowerAccountID)-1] = '\0';
//	strlwr( szLowerAccountID );
//	// 변환 끝
//
//	//잔액 조회인데..
//	RESULT_QUERYCOIN rstQC;
//	CLEAR_RST_QC(&rstQC)
//	/*rstQC.Clear(); */
//	SI32 siRet = HannEverBillQueryCoin(&rstQC, szLowerAccountID);
//	if ( HAN_NEVERBILL_OK != siRet ) {
//		NTCHARString256		kError( TEXT("HannEverBillQueryCoin() Failed id - #id#, Ret - #code#, error - #error#" ) );
//		kError.Replace("#id#", szLowerAccountID);
//		kError.Replace("#code#", SI32ToString(siRet));
//		kError.Replace("#error#", HannEverBillErrorString(HannEverBillGetLastError()));
//		WriteErrorLog(__FILE__, __LINE__, kError);
//		return FALSE; 
//	} 
//	else {
//		*psiHanCoin		= rstQC.lCoin;
//		*psiGiftCard	= rstQC.lGiftCoin;
//		*psiInnerEvent	= rstQC.lInEvntCoin;
//		*psiOuterEvent	= rstQC.lExEvntCoin;
//	}
//
//#endif

	return TRUE;
}

//한게임 머니사용
BOOL NHannEverBillingProcess::ChargeItem(TCHAR * pszAccountID , SI32 siProductKey , SI32 siPrice , SI32 siProductNum )
{
	if(m_bTest == true)
		return TRUE;

//#if defined(_KOREA)
//
//	// 전문 생성
//	TCHAR		szProductKey[256] = {0,};
//
//	// 계정 문자열을 모두 소문자로 변환한다.
//	char szLowerAccountID[MAX_PLAYER_NAME] = {0,};
//	StringCchCopy( szLowerAccountID, MAX_PLAYER_NAME , pszAccountID );
//	szLowerAccountID[sizeof(szLowerAccountID)-1] = '\0';
//	strlwr( szLowerAccountID );
//	///////////////////////////////// 변환끝
//
//	// 프로덕트키 문자열
//	sprintf(szProductKey , "%d" , siProductKey);
//	//상품구매갯수가 한개 일때는 기존의 방식대로 구매    
//	if (siProductNum == 1) {        
//		REQUEST_USECOIN reqUC;
//		CLEAR_REQ_UC(&reqUC);
//		/*reqUC.Clear(); */
//		RESULT_USECOIN rstUC;
//		CLEAR_RST_UC(&rstUC)
//		/*rstUC.Clear(); */
//		SI32 Payment = siPrice; 
//
//		reqUC.nPayment = Payment;
//		strncpy(reqUC.szUserKey,	szLowerAccountID,	nEB_USERKEY_LEN); 
//		strncpy(reqUC.szCPID,		"GZS",				nEB_CPID_LEN); 
//		strncpy(reqUC.szItemID,		szProductKey,		nEB_ITEMID_LEN); 
//
//		if ( HAN_NEVERBILL_OK != HannEverBillUseCoin(&rstUC, &reqUC) ) {
//			NTCHARString128		kError( TEXT("HannEverBillUseCoin() Failed Code: - #code#, Msg - #error#") );
//			kError.Replace("#code#", SI32ToString(rstUC.nRtnCode));
//			kError.Replace("#error#", HannEverBillErrorString(HannEverBillGetLastError()));
//			WriteErrorLog(__FILE__, __LINE__, kError);
//			return FALSE;
//		}
//	}
//	else if (siProductNum > 1) {
//		REQUEST_USECOIN reqUC; 
//		CLEAR_REQ_UC(&reqUC); 
//		RESULT_MULTI_USECOIN rstMUC;
//		CLEAR_RST_MUC(&rstMUC); 
//		char szRtnMsg[512]; 
//		
//		rstMUC.pRtnMsgBuff = szRtnMsg; 
//		rstMUC.nRtnMsgBuffLen = sizeof(szRtnMsg);
//
//		reqUC.nPayment = siPrice; 
//		strncpy(reqUC.szUserKey, szLowerAccountID,	nEB_USERKEY_LEN); 
//		strncpy(reqUC.szCPID,	"GZS",				nEB_CPID_LEN ); 
//		strncpy(reqUC.szItemID,	szProductKey,		nEB_ITEMID_LEN); 
//		
//		REQUEST_USECOIN		ReqUseCoin[HANNEVERBILL_MAX_MULTICOUNT];
//
//		for(int i=0; i<siProductNum; i++)
//		{
//			CLEAR_REQ_UC(&ReqUseCoin[i]); 
//			ReqUseCoin[i].nPayment = siPrice; 
//			strncpy(ReqUseCoin[i].szUserKey, szLowerAccountID,	nEB_USERKEY_LEN); 
//			strncpy(ReqUseCoin[i].szCPID,	"GZS",				nEB_CPID_LEN ); 
//			strncpy(ReqUseCoin[i].szItemID,	szProductKey,		nEB_ITEMID_LEN); 
//		}
//
//		// commit
//		//오류 기록
//		 SI32 retvalue = HannEverBillMultiUseCoin(&rstMUC, ReqUseCoin, siProductNum);
//		if (HAN_NEVERBILL_OK != retvalue   ) {
//			NTCHARString128		kError( TEXT("HannEverBillMultiUseCoin() Failed ret: - #ret#,Msg - #error#") );
//			kError.Replace("#ret#", SI32ToString(retvalue));
//			kError.Replace("#error#", HannEverBillErrorString(HannEverBillGetLastError()));
//			WriteErrorLog(__FILE__, __LINE__, kError);
//			return FALSE;
//		} 
//		if ( 0 != rstMUC.nRtnCode ) {
//			NTCHARString128		kError( TEXT("HannEverBillMultiUseCoin() Failed Code: - #code#,Msg - #error#") );
//			kError.Replace("#code#", SI32ToString(rstMUC.nRtnCode));		
//			kError.Replace("#error#", HannEverBillErrorString(HannEverBillGetLastError()));
//			WriteErrorLog(__FILE__, __LINE__, kError);
//			return FALSE;
//		}
//	}
//
//	//SI32 Payment = siPrice * siProductNum; 
//
//	//reqUC.nPayment = Payment;
//	//strncpy(reqUC.szUserKey,	szLowerAccountID,	nEB_USERKEY_LEN); 
//	//strncpy(reqUC.szCPID,		"GZS",				nEB_CPID_LEN); 
//	//strncpy(reqUC.szItemID,		szProductKey,		nEB_ITEMID_LEN); 
//
//	//
//	//if ( HAN_NEVERBILL_OK != HannEverBillUseCoin(&rstUC, &reqUC) ) {
//	//	NTCHARString128		kError( TEXT("HannEverBillUseCoin() Failed Code: - #code#, Msg - #error#") );
//	//	kError.Replace("#code#", SI32ToString(rstUC.nRtnCode));
//	//	kError.Replace("#error#", HannEverBillErrorString(HannEverBillGetLastError()));
//	//	WriteErrorLog(__FILE__, __LINE__, kError);
//	//	return FALSE;
//#endif

	return TRUE;
}

void NHannEverBillingProcess::WriteErrorLog(TCHAR* pszSource, int nLine , TCHAR * pszMsg)
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