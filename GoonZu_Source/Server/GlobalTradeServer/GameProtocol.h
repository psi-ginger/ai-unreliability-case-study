#ifndef _GAMEPROTOCOL_H
#define _GAMEPROTOCOL_H

#include "GlobalTradeServer.h"
#include "ITZNetworkThread.h"

#define MAX_SERVER_SESSION_NUM		50

class GameProtocol : public ITZNetworkThread
{
public:
	GameProtocol();
	~GameProtocol();

public:
	void				Init();

	virtual void		Update();
	virtual void		LogOut( ITZSession *pSession );
	virtual void		SessionProc( ITZSession *pSession );

	void				lock() { EnterCriticalSection( &m_cs ); };
	void				unlock() { LeaveCriticalSection( &m_cs ); };

	cGame*				GetGameServer( UI16 uiCharID );

	void FilePrint( char* filename, char *format,... );

	CRITICAL_SECTION	m_cs;

	int					m_state;

	int					m_recvCount;
	int					m_sendCount;

	DWORD				m_dwCheckClock;
	DWORD				m_dwLasteceiveClock[MAX_SERVER_SESSION_NUM];

	char				dbgbuf[2048];
	char				m_Time[ 128 ];
	char				m_Date[ 128 ];

private:
	DWORD				m_dwHeartBeatClock;

public:
	ITZSessionFactory< cGame >	m_SF;
};


#endif
