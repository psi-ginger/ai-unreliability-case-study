#ifndef _ACCOUNTDBPROCESS_H
#define _ACCOUNTDBPROCESS_H

#include "ITZSessionFactory.h"
#include "ITZSessionManager.h"
#include "ITZNetIOCP.h"
#include "ITZPerson.h"

#include "MasterServer.h"

class cAccountDB : public ITZPerson
{
public:
	cAccountDB() {};
	~cAccountDB() {};
};

class AccountDBProcess
{
public:
	AccountDBProcess();
	~AccountDBProcess();

public:
	void			Init( char *ip, UI16 usPort );
	void			Run();

	ITZSession*		GetSession() { return m_pAccountDBSession; };

private:
	ITZSessionFactory< cAccountDB >	m_sf;							// 세션 팩토리
	ITZSessionManager				m_sm;							// 세션 매니저
	ITZNetIOCP						m_iocp;							// IOCP 

	ITZSession*						m_pAccountDBSession;			// 게임 DB매니저 세션 포인터

	char							m_accountDBManagerIP[ 20 ];		// 게임 DB매니저 IP
	UI16							m_accountDBManagerPort;			// 게임 DB매니저 PORT

};

#endif