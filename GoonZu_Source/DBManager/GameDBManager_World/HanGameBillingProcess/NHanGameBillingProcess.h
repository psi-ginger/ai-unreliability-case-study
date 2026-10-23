//=============================================================================
//
//	NHanGameBillingProcess.h
//
//=============================================================================
#ifndef _NHANGAMEBILLINGPROCESS_H_
#define _NHANGAMEBILLINGPROCESS_H_

#include <windows.h>

#include "../../GoonZuWorld/common/CommonHeader.h"

//=============================================================================
// DEFINES
//=============================================================================
#define HANGAME_BILLING_WAITMESSAGE	7000
#define HANGAME_BILLING_TOTALWAIT	10000
#define HANGAME_BILLING_WAITCONNECT	500
#define HANGAME_BILLING_WAITSOCKET	500

//=============================================================================
//	CLASS   NHanGameBillingProcess
//! @brief  
//=============================================================================
class NHanGameBillingProcess
{
private:
	TCHAR		m_szIP[ 256 ];
	UI16		m_usPort;
	SOCKET		m_hSocket;
	SI32		m_siLastBillingError;
	sockaddr_in	m_sockaddr;

	bool		m_bTest;

public:
	NHanGameBillingProcess();
	~NHanGameBillingProcess();

	void	Init(TCHAR *pszIP, UI16 usPort);

	inline  SI32 GetLastBliingError()							{		return m_siLastBillingError;		}

	void	WriteErrorLog( TCHAR* pszSource, int nLine , TCHAR * pszMsg);

public:
	BOOL	GetBillingBalance(TCHAR* pszAccountID, SI64* psiHanCoin, SI64* psiGiftCard, SI64* psiInnerEvent, SI64* psiOuterEvent);

	BOOL	ChargeItem(TCHAR * pszAccountID , SI32 siProductKey , SI32 siPrice , SI32 siProductNum );

private:
	BOOL	ConnectServer();
	BOOL	DisconnectServer();

	SI32	WriteMessage(SOCKET hSocket, TCHAR *pData, SI32 siDataLength);
	SI32	ReadMessage(SOCKET hSocket, TCHAR *pData, SI32 siDataLength);

	
};


#endif
