#ifndef __BILLSOCKET_H__
#define __BILLSOCKET_H__

#define		MAX_PACKETSIZE				1024	//	패킷 최대크기	
#define		LIMIT_RECEIVETIME			10000	//	패킷받을수 있는 최대 시간 10초

#include "itzbase.h"

/********************************************************************
	created:	2009/04/30
	created:	30:4:2009   14:03
	filename: 	d:\GProject\DBManager\GameDBManager_World\BillSocket_Germany.h
	file path:	d:\GProject\DBManager\GameDBManager_World
	file base:	BillSocket_Germany
	file ext:	h
	author:		조종호
	
	purpose:	PG와 직접적으로 통신하는 클래스 입니다.
				BillSocketManager로부터 요청이 들어오면 이곳에서
				들어온 요청에 따라 PG측과 통신 후 Data를 Return 시킵니다.
*********************************************************************/

class CBillSocket_Germany
{
private: 
	SOCKET		m_hSocket;
	BYTE		m_szBuf[MAX_PACKETSIZE];		//	PG측으로부터 받은 Packet를 저장할 Buffer
	DWORD		m_StartTime;					//	Receive시작시간

	SOCKADDR_IN	m_sServerAddress;				//	PG측 IP 및 Port


public:
	CBillSocket_Germany();
	~CBillSocket_Germany();

public:
	bool						ConnectToServer();
	BYTE*						ReceivePacket();
	bool						SendBalancePacket(SI32 siAccountNumber, char* accountid);	
	bool						SendBuyPacket(SI32 siAccountNumber, TCHAR* accountid, TCHAR* szProductKey, SI32 siProductPrice, TCHAR* szProductName);
	bool						SendGiftPacket(SI32 siAccountNumber, char *accountid, char* szRecvUserName, TCHAR* szProductKey, SI32 siProductPrice, TCHAR* szProductName);
	bool						CheckOverTime_Bill();
	void						CloseBillSocket();
	void						WriteErrorLog(TCHAR* pszSource, int nLine , TCHAR * pszMsg);
};





////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	PG 측과 통신할 Packet 구조체
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma pack(1)

struct BILL_PACK_HEADER
{
	WORD	m_ReqLen;
	WORD	m_ReqType;
	WORD	m_RetCode;
};

//	잔액조회
struct BILL_PACK_BALANCE
{
	BILL_PACK_HEADER m_Header;

	int		m_nUserNo;
	char	m_szUserID[50+1];

	DWORD	m_nCashRemain;	
};


//	구매
struct BILL_PACK_BUY
{
	BILL_PACK_HEADER m_Header;

	int		m_nUserNo;
	char	m_szUserID[50+1];

	DWORD	m_nCashRemain;	
	char	m_szChargeNo[19+1];

	char	m_szItemID[30+1];
	int		m_nItemPrice;
	char	m_szItemName[50+1];
};


//	선물
struct BILL_PACK_GIFT
{
	BILL_PACK_HEADER m_Header;

	int		m_nUserNo;
	char	m_szUserID[50+1];
	char	m_szReceiveUserID[50+1];

	DWORD	m_nCashRemain;	
	char	m_szChargeNo[19+1];

	char	m_szItemID[30+1];
	int		m_nItemPrice;
	char	m_szItemName[50+1];
};

#pragma pack()

#endif	//__BILLSOCKET_H__

