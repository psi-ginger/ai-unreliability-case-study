//////////////////////////////////////////////////////////////////////////
//	created:	2009/04/30
//	created:	30:4:2009   13:51
//	filename: 	d:\GProject\DBManager\GameDBManager_World\BillManager_Germany.h
//	file path:	d:\GProject\DBManager\GameDBManager_World
//	file base:	BillManager_Germany
//	file ext:	h
//	author:		조종호
//	
//	purpose:	독일쪽 PG와 Bill 관련 Manager
//				게임서버로부터 PG 관련 요청이 올시에 
//				BillSocket을 통해 PG에 요청합니다.
//////////////////////////////////////////////////////////////////////////

#ifndef	__BILLMANAGER_H__
#define __BILLMANAGER_H__

#include "BillSocket_Germany.h"

//	CMD
enum REQTYPE_BILL
{
	REQTYPE_BILL_GET_BALANCE = 	10,			//	잔액조회
	REQTYPE_BILL_PURCHASE	 =	20,			//	소진
	REQTYPE_BILL_GIFT		 = 	30,			//	선물
	REQTYPE_BILL_CALCEL		 =	40,			//	취소
};

class CBillManager_Germany
{;
private:
	CBillSocket_Germany	m_cBillSocket;		//	이 소켓을 통해서 PG와 통신 합니다.


public:
	CBillManager_Germany();
	~CBillManager_Germany();

public:
	BYTE*	ProcessBalance(SI32 siAccountNumber, char* accountid);		
	BYTE*	ProcessBuyItem(SI32 siAccountNumber, TCHAR* accountid, TCHAR* szProductKey, SI32 siProductPrice, TCHAR* szProductName);	
	BYTE*	ProcessGiftItem(SI32 siAccountNumber, TCHAR* accountid, TCHAR* Recvaccountid, TCHAR* siProductKey, SI32 siProductPrice, TCHAR* szProductName);
	void	CloseBillSocket();
	void	WriteErrorLog(TCHAR* pszSource, int nLine , TCHAR * pszMsg);

};

#endif	//	__BILLMANAGER_H__