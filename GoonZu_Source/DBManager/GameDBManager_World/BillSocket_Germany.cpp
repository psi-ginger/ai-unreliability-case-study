#include "BillManager_Germany.h"

extern char				g_BillingServerIP[ 256 ];
extern UI16				g_BillingServerPort;

CBillSocket_Germany::CBillSocket_Germany(void)
{
	m_hSocket	=	NULL;
	m_StartTime	=	0;

	ZeroMemory(&m_szBuf, sizeof(MAX_PACKETSIZE));
	ZeroMemory(&m_sServerAddress, sizeof(SOCKADDR_IN));
	
}

CBillSocket_Germany::~CBillSocket_Germany(void)
{

}


bool CBillSocket_Germany::ConnectToServer()
{
	m_hSocket	=	socket(PF_INET, SOCK_STREAM, 0);

	memset(&m_sServerAddress, 0, sizeof(SOCKADDR_IN));
	m_sServerAddress.sin_family		=	AF_INET;
	m_sServerAddress.sin_addr.s_addr	=	inet_addr(g_BillingServerIP);
	m_sServerAddress.sin_port			=	htons(g_BillingServerPort);		//	Port 입력	


	char buf[256];
	ZeroMemory(buf, 256);

	sprintf(buf,"Connected IP : %s\n", g_BillingServerIP);
	sprintf(buf,"Connected Port : %d\n", g_BillingServerPort);
	WriteErrorLog(__FILE__, __LINE__, buf);

	int err;
	err = connect(m_hSocket, (SOCKADDR*)&m_sServerAddress, sizeof(SOCKADDR_IN));
	if (err == SOCKET_ERROR && WSAGetLastError() != WSAEWOULDBLOCK)
	{
		char buf[256];
		ZeroMemory(buf, 256);

		sprintf(buf, "---------Connect to Server  Failed : Error code - %d---------\n", WSAGetLastError());
		WriteErrorLog(__FILE__, __LINE__, buf);
		
		return false;
	}

	return true;
}


//	PG로 잔액조회 요청을 합니다.
bool CBillSocket_Germany::SendBalancePacket(SI32 siAccountNumber, char* accountid)	
{
	BILL_PACK_BALANCE sBalancePacket;
	ZeroMemory(&sBalancePacket, sizeof(BILL_PACK_BALANCE));

	sBalancePacket.m_Header.m_ReqLen	=	sizeof(BILL_PACK_BALANCE);	
	sBalancePacket.m_Header.m_ReqType	=	REQTYPE_BILL_GET_BALANCE;	
	sBalancePacket.m_Header.m_RetCode	=	0;

	sBalancePacket.m_nUserNo			=	siAccountNumber;					//	ID Num
	strncpy(sBalancePacket.m_szUserID, accountid, strlen(accountid));			//	UserID


	sBalancePacket.m_nCashRemain		=	0;							//	남은돈
	//----------------------- SendPacket 설정완료-------------------------------------------


	send(m_hSocket, (char*)&sBalancePacket, sizeof(BILL_PACK_BALANCE), 0);

	char buf[256];
	ZeroMemory(buf, 256);

	sprintf(buf, "---------Send Balance Packet---------\n");
	WriteErrorLog(__FILE__, __LINE__, buf);
	sprintf(buf, "sBalancePacket.m_nUserNo = %d\n", sBalancePacket.m_nUserNo);
	WriteErrorLog(__FILE__, __LINE__, buf);
	sprintf(buf, "sBalancePacket.m_nUserID = %s\n", sBalancePacket.m_szUserID);
	WriteErrorLog(__FILE__, __LINE__, buf);

	return true;
}

//	PG로 구매 요청을 합니다. 구매 요청후 소진된 금액과 PG측에서 발행한 구매코드를 발급 받습니다.
bool CBillSocket_Germany::SendBuyPacket(SI32 siAccountNumber, TCHAR* accountid, TCHAR* szProductKey, SI32 siProductPrice, TCHAR* szProductName)
{
	BILL_PACK_BUY sBuyPacket;
	ZeroMemory(&sBuyPacket, sizeof(BILL_PACK_BUY));

	sBuyPacket.m_Header.m_ReqLen	=	sizeof(BILL_PACK_BUY);	
	sBuyPacket.m_Header.m_ReqType	=	REQTYPE_BILL_PURCHASE;	
	sBuyPacket.m_Header.m_RetCode	=	0;

	sBuyPacket.m_nUserNo			=	siAccountNumber;							
	sBuyPacket.m_nCashRemain		=	0;			
	sBuyPacket.m_nItemPrice			=	siProductPrice;

	strncpy(sBuyPacket.m_szUserID, accountid,	strlen(accountid));
	strncpy(sBuyPacket.m_szItemID, szProductKey,	strlen(szProductKey));
	strncpy(sBuyPacket.m_szItemName, szProductName,	strlen(szProductName));
	//----------------------- SendPacket 설정완료-------------------------------------------


	send(m_hSocket, (char*)&sBuyPacket, sizeof(BILL_PACK_BUY), 0);

	char buf[256];
	ZeroMemory(buf, 256);

	sprintf(buf, "---------Send Buy Packet---------\n");
	WriteErrorLog(__FILE__, __LINE__, buf);
	sprintf(buf, "sBuyPacket.m_nUserNo = %d\n", sBuyPacket.m_nUserNo);
	WriteErrorLog(__FILE__, __LINE__, buf);
	sprintf(buf, "sBuyPacket.m_nUserID = %s\n", sBuyPacket.m_szUserID);
	WriteErrorLog(__FILE__, __LINE__, buf);
	sprintf(buf, "sBuyPacket.m_szItemID = %s\n", sBuyPacket.m_szItemID);
	WriteErrorLog(__FILE__, __LINE__, buf);
	sprintf(buf, "sBuyPacket.m_szItemName = %s\n", sBuyPacket.m_szItemName);
	WriteErrorLog(__FILE__, __LINE__, buf);
	sprintf(buf, "sBuyPacket.m_nItemPrice = %d\n", sBuyPacket.m_nItemPrice);
	WriteErrorLog(__FILE__, __LINE__, buf);

	return true;
}

//	PG로 선물 요청을 합니다. 선물 요청후 소진된 금액과 PG측에서 발행한 구매코드를 발급 받습니다.
bool CBillSocket_Germany::SendGiftPacket(SI32 siAccountNumber, char *accountid, char* szRecvUserName, TCHAR* szProductKey, SI32 siProductPrice, TCHAR* szProductName)
{
	BILL_PACK_GIFT sGiftPacket;
	ZeroMemory(&sGiftPacket, sizeof(BILL_PACK_GIFT));

	sGiftPacket.m_Header.m_ReqLen	=	sizeof(BILL_PACK_GIFT);	
	sGiftPacket.m_Header.m_ReqType	=	REQTYPE_BILL_GIFT;	
	sGiftPacket.m_Header.m_RetCode	=	0;

	sGiftPacket.m_nUserNo			=	siAccountNumber;											
	sGiftPacket.m_nCashRemain		=	0;			
	sGiftPacket.m_nItemPrice		=	siProductPrice;

	strncpy(sGiftPacket.m_szItemName,		szProductName,	strlen(szProductName));
	strncpy(sGiftPacket.m_szItemID,			szProductKey,	strlen(szProductKey));
	strncpy(sGiftPacket.m_szUserID,			accountid,		strlen(accountid));			
	strncpy(sGiftPacket.m_szReceiveUserID,	szRecvUserName,	strlen(szRecvUserName));	
	//----------------------- SendPacket 설정완료-------------------------------------------




	send(m_hSocket, (char*)&sGiftPacket, sizeof(BILL_PACK_GIFT), 0);	


	char buf[256];
	ZeroMemory(buf, 256);

	sprintf(buf, "---------Send Gift Packet---------\n");
	WriteErrorLog(__FILE__, __LINE__, buf);
	sprintf(buf, "sGiftPacket.m_nUserNo = %d\n", sGiftPacket.m_nUserNo);
	WriteErrorLog(__FILE__, __LINE__, buf);
	sprintf(buf, "sGiftPacket.m_nUserID = %s\n", sGiftPacket.m_szUserID);
	WriteErrorLog(__FILE__, __LINE__, buf);
	sprintf(buf, "sGiftPacket.m_szReceiveUserID = %s\n", sGiftPacket.m_szReceiveUserID);
	WriteErrorLog(__FILE__, __LINE__, buf);
	sprintf(buf, "sGiftPacket.m_szItemID = %s\n", sGiftPacket.m_szItemID);
	WriteErrorLog(__FILE__, __LINE__, buf);
	sprintf(buf, "sGiftPacket.m_szItemName = %s\n", sGiftPacket.m_szItemName);
	WriteErrorLog(__FILE__, __LINE__, buf);
	sprintf(buf, "sGiftPacket.m_nItemPrice = %d\n", sGiftPacket.m_nItemPrice);
	WriteErrorLog(__FILE__, __LINE__, buf);

	return true;
}


BYTE* CBillSocket_Germany::ReceivePacket()	
{
	int nLen = 0;
	int nTotalLen = 0;

	m_StartTime = GetTickCount();


	BILL_PACK_HEADER* pHeader	=	NULL;

	ZeroMemory(m_szBuf, sizeof(MAX_PACKETSIZE));

	while(1)
	{
		// Receive가 완료가 되지않았다고 무한정 이곳에서 대기할순 없으므로 10초동안 Receive 완료 못하면 실패 시킵니다.
		if(CheckOverTime_Bill() == false)
		{
			char buf[256];
			ZeroMemory(buf, 256);

			sprintf(buf, "---------Out of Time!!!!!!!!!---------\n");
			WriteErrorLog(__FILE__, __LINE__, buf);

			pHeader = (BILL_PACK_HEADER*)m_szBuf;
			pHeader->m_RetCode = -1;
			break;
		}

		nLen = recv(m_hSocket,(char*)m_szBuf + nTotalLen, MAX_PACKETSIZE-nTotalLen, 0);	

		if(nLen <= 0)
			continue;
		else
			nTotalLen += nLen;


		if(nTotalLen >= sizeof(WORD))
		{
			WORD size = (WORD)m_szBuf;

			pHeader = (BILL_PACK_HEADER*)m_szBuf;

			if(pHeader->m_ReqLen <= nTotalLen)
				break;
		}
	}



	switch(pHeader->m_ReqType)
	{
	case REQTYPE_BILL_GET_BALANCE:		//	PacketType	:	잔액조회
		{
			//	밑의 BILL_PACK_BALANCE* pPacketBalance 은 PG Server로부터 받은 데이터 체크하기 위한 부분
			BILL_PACK_BALANCE* pPacketBalance	=	(BILL_PACK_BALANCE*)m_szBuf;	

			char buf[256];
			ZeroMemory(buf, 256);

			sprintf(buf, "---------Receive Balance Packet---------\n");
			WriteErrorLog(__FILE__, __LINE__, buf);
			sprintf(buf, "pPacketBalance.m_nUserNo = %d\n", pPacketBalance->m_nUserNo);
			WriteErrorLog(__FILE__, __LINE__, buf);
			sprintf(buf, "pPacketBalance.m_nUserID = %s\n", pPacketBalance->m_szUserID);
			WriteErrorLog(__FILE__, __LINE__, buf);
			sprintf(buf, "pPacketBalance.m_nCashRemain = %d\n", pPacketBalance->m_nCashRemain);
			WriteErrorLog(__FILE__, __LINE__, buf);

			return m_szBuf;
		}
		break;

	case REQTYPE_BILL_PURCHASE:			//	PacketType	:	구매
		{
			BILL_PACK_BUY* pPacketBuy = (BILL_PACK_BUY*)m_szBuf;

			char buf[256];
			ZeroMemory(buf, 256);

			sprintf(buf, "---------Receive Buy Packet---------\n");
			WriteErrorLog(__FILE__, __LINE__, buf);
			sprintf(buf, "pPacketBuy.m_nUserNo = %d\n", pPacketBuy->m_nUserNo);
			WriteErrorLog(__FILE__, __LINE__, buf);
			sprintf(buf, "pPacketBuy.m_nUserID = %s\n", pPacketBuy->m_szUserID);
			WriteErrorLog(__FILE__, __LINE__, buf);
			sprintf(buf, "pPacketBuy.m_nCashRemain = %d\n", pPacketBuy->m_nCashRemain);
			WriteErrorLog(__FILE__, __LINE__, buf);
			sprintf(buf, "pPacketBuy.m_szItemName = %s\n", pPacketBuy->m_szItemName);
			WriteErrorLog(__FILE__, __LINE__, buf);
			sprintf(buf, "pPacketBuy.m_nItemPrice = %d\n", pPacketBuy->m_nItemPrice);
			WriteErrorLog(__FILE__, __LINE__, buf);
			sprintf(buf, "pPacketBuy.m_szChargeNo = %s\n", pPacketBuy->m_szChargeNo);
			WriteErrorLog(__FILE__, __LINE__, buf);

			return m_szBuf;
		}
		break;

	case REQTYPE_BILL_GIFT:				//	PacketType	:	선물
		{
			BILL_PACK_GIFT* pPacketGift = (BILL_PACK_GIFT*)m_szBuf;

			char buf[256];
			ZeroMemory(buf, 256);

			sprintf(buf, "---------Receive Gift Packet---------\n");
			WriteErrorLog(__FILE__, __LINE__, buf);
			sprintf(buf, "pPacketGift.m_nUserNo = %d\n", pPacketGift->m_nUserNo);
			WriteErrorLog(__FILE__, __LINE__, buf);
			sprintf(buf, "pPacketGift.m_nUserID = %s\n", pPacketGift->m_szUserID);
			WriteErrorLog(__FILE__, __LINE__, buf);
			sprintf(buf, "pPacketGift.m_szReceiveUserID = %s\n", pPacketGift->m_szReceiveUserID);
			WriteErrorLog(__FILE__, __LINE__, buf);
			sprintf(buf, "pPacketGift.m_nCashRemain = %d\n", pPacketGift->m_nCashRemain);
			WriteErrorLog(__FILE__, __LINE__, buf);
			sprintf(buf, "pPacketGift.m_szItemName = %s\n", pPacketGift->m_szItemName);
			WriteErrorLog(__FILE__, __LINE__, buf);
			sprintf(buf, "pPacketGift.m_nItemPrice = %d\n", pPacketGift->m_nItemPrice);
			WriteErrorLog(__FILE__, __LINE__, buf);
			sprintf(buf, "pPacketGift.m_szChargeNo = %s\n", pPacketGift->m_szChargeNo);
			WriteErrorLog(__FILE__, __LINE__, buf);

			return m_szBuf;
		}
		break;

	default:
		{

		}
		break;
	}
	return NULL;
}


//	일정 시간이 지나면 Receive를 중단 시키기 위한 시간체크 함수 입니다.
bool CBillSocket_Germany::CheckOverTime_Bill()
{
	bool bReturnValue = false;

	DWORD ElapseTime = GetTickCount() - m_StartTime;	

	if(ElapseTime > LIMIT_RECEIVETIME)
		bReturnValue = false;
	else
		bReturnValue = true;

	return bReturnValue;
}


void CBillSocket_Germany::CloseBillSocket()
{
	closesocket(m_hSocket);
}

void	CBillSocket_Germany::WriteErrorLog(TCHAR* pszSource, int nLine , TCHAR * pszMsg)
{
	if( pszMsg == NULL) return;

	SYSTEMTIME CT;
	GetLocalTime(&CT);

	TCHAR szFileName[256] = "";
	sprintf(szFileName, "LOG_Germany_Billing_%02d%02d%02d.txt", CT.wYear, CT.wMonth, CT.wDay);

	FILE * fp = NULL;

	fp = fopen (szFileName , "a+");

	if( fp == NULL ) return;

	fprintf(fp , "%2d-%2d-%2d\t%s\t%4d\t%s\n" , CT.wHour , CT.wMinute , CT.wSecond , pszMsg  , nLine, pszSource );
	fclose(fp);	
}