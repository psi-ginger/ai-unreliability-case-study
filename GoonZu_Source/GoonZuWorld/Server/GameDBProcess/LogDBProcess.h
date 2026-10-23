#ifndef _LOGDBPROCESS_H
#define _LOGDBPROCESS_H

#include "ITZSessionFactory.h"
#include "ITZSessionManager.h"
#include "ITZNetIOCP.h"
#include "ITZPerson.h"

class cLogDB : public ITZPerson
{
public:
	cLogDB() {};
	~cLogDB() {};
};


class LogDBProcess
{
private:

public:
	LogDBProcess();
	~LogDBProcess();

public:
	void			Init( TCHAR *ip, UI16 usPort );
	void			Run();

	bool			SendMsg(sPacketHeader *pPacket);

	ITZSession*		GetSession() { return m_pGameDBSession; };

private:
	ITZSessionFactory< cLogDB >		m_sf;							// 세션 팩토리
	ITZSessionManager				m_sm;							// 세션 매니저
	ITZNetIOCP						m_iocp;							// IOCP 

	ITZSession*						m_pGameDBSession;				// 게임 DB매니저 세션 포인터

	TCHAR							m_gameDBManagerIP[ 20 ];		// 게임 DB매니저 IP
	UI16							m_gameDBManagerPort;			// 게임 DB매니저 PORT

};

#endif