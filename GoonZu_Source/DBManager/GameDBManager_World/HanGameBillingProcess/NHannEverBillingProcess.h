//=============================================================================
//
//NHannEverBillingProcess.h
//
//=============================================================================
#ifndef _NHANNEVERBILLINGPROCESS_H_
#define _NHANNEVERBILLINGPROCESS_H_

#include <windows.h>
#include <stdio.h>

#include "../../GoonZuWorld/common/CommonHeader.h"

//#if defined(_KOREA)
//	#include "./include/HannEverBillForSvr.h"
//#endif

//=============================================================================
// DEFINES
//=============================================================================
#define HANNEVERBILL_MAX_MULTICOUNT		100

//=============================================================================
//	CLASS   NHannEverBillingProcess
//! @brief  
//=============================================================================
class NHannEverBillingProcess
{
private:
	bool		m_bTest;//테스트

public:
	NHannEverBillingProcess();
	~NHannEverBillingProcess();

	void	Init(TCHAR *pszGameID);

	//사용 할 필요 없음
	/*inline  SI32 GetLastBliingError()							{		return m_siLastBillingError;		}*/

	void	WriteErrorLog( TCHAR* pszSource, int nLine , TCHAR * pszMsg);//그대로 쓴다.

public:
	BOOL	GetBillingBalance(TCHAR* pszAccountID, SI64* psiHanCoin, SI64* psiGiftCard, SI64* psiInnerEvent, SI64* psiOuterEvent);

	BOOL	ChargeItem(TCHAR * pszAccountID , SI32 siProductKey , SI32 siPrice , SI32 siProductNum );

//한게임은 모듈 사용으로 더이상 이부분은 사용하지 않음.
private:
};


#endif
