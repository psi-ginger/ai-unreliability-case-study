#ifndef _DBPROCESS_H
#define _DBPROCESS_H

#include "stdafx.h"
#include "GameProtocol.h"

#include "ITZSessionFactory.h"
#include "ITZSessionManager.h"
#include "ITZNetIOCP.h"
#include "ITZPerson.h"

#include "itzbase.h"

#include "../BeTheRich/Server/NTradeServerProcess/TradeServerMsg.h"

class cDB_client : public ITZPerson
{
public:
	cDB_client() {};
	~cDB_client() {};

public:
	virtual void Init() {};
	virtual void Close() {};
};

class DBProcess
{
public:
	DBProcess();
	~DBProcess();

	void				Init( char *szIP, UI16 usPort );
	void				Run();

	void				ReduceAction();
	void				SetSellInfo();

	SI32				GetDateVary(UI16 year,UI08 month,UI08 day);

	ITZSession*			GetLogSession() { return m_pDBSession; };

	void				SendMsg( sPacketHeader *pPacket );

	cltTradeServerOrder	clOrder[MAX_TRADESERVER_ORDER_LIST];

	static SI32			s_uiDaysOfMonth[ 13 ];

	void FilePrint( char* filename, char *format,... );

private:
	ITZSessionFactory< cDB_client >		m_sf;									// 세션 팩토리
	ITZSessionManager					m_sm;									// 세션 매니저
	ITZNetIOCP							m_iocp;									// IOCP 

	ITZSession*							m_pDBSession;							// 서버에이전트 세션 포인터

	char								m_szIP[ 16 ];
	UI16								m_usPort;

	bool								m_bDBInit; // DB에서 데이타를 다 받았다
	_SYSTEMTIME							m_sTime; // 시간 변수

	CRITICAL_SECTION					m_LogCritical;
	char								dbgbuf[2048];
	char								m_Time[ 128 ];
	char								m_Date[ 128 ];

};

#endif 
