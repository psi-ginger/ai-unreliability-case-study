//=============================================================================
//
//	NIJJIBillingProcess.h
//
//=============================================================================
#ifndef _NIJJIBILLINGPROCESS_H_
#define _NIJJIBILLINGPROCESS_H_

#include <windows.h>

#include "NHNCoinDll.h"

#define DLL_LIBRARY "NHNCoinDll.dll"

#include "../../GoonZuWorld/common/CommonHeader.h"

// 클래스 인스턴스 생성 함수
typedef NHNCoin* (*PFunc)(const char *, unsigned short, unsigned int);

//=============================================================================
//	CLASS   NIJJIBillingProcess
//! @brief  
//=============================================================================
class NIJJIBillingProcess
{
private:
	NHNCoin *m_coin;
	HINSTANCE m_hdll;

	bool	m_bTest;

public:
	NIJJIBillingProcess();
	~NIJJIBillingProcess();

	void	Init( TCHAR *pszIP, UI16 usPort, UI32 uiTimeOut );

	void	WriteProcessLog( TCHAR* pszSource, int nLine , TCHAR * pszMsg);
	void	WriteErrorLog( TCHAR* pszSource, int nLine , TCHAR * pszMsg);

public:
	bool	GetBillingBalance(TCHAR* pszAccountID, SI32 siIDNo, SI32* psiHanCoin, SI32* psiGiftCard, SI32* psiInnerEvent, SI32* psiOuterEvent);
	bool	GetBillingBalance(TCHAR* pszAccountID, SI32 siIDNo, SI32* psiToTalCoin );	//[진성] 한게임 코인.

	bool	ChargeItem(TCHAR * pszAccountID , SI32 siIDNo, TCHAR* pszIP, SI32 siProductKey, TCHAR* pszProductName, SI32 siPrice , SI32 siProductNum );
	BOOL	GiftItem(TCHAR * pszGiftSendID, TCHAR * pszGiftRecvID , SI32 siIDNo, TCHAR* pszIP, SI32 siProductKey, TCHAR* pszProductName, SI32 siPrice , SI32 siProductNum );
};


#endif
