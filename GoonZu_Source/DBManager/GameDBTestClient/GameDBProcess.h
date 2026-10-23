#ifndef _GAMEDBPROCESS_H
#define _GAMEDBPROCESS_H

#include "ITZSessionFactory.h"
#include "ITZSessionManager.h"
#include "ITZNetIOCP.h"
#include "ITZPerson.h"

class cGameDB : public ITZPerson
{
public:
	cGameDB() {};
	~cGameDB() {};

	virtual void Init() { printf( "GameDB Manager에 접속 성공\n" ); }
	virtual void Close() { printf( "GameDB Manager와 접속 끊김\n" ); }
};


class GameDBProcess
{
public:
	GameDBProcess();
	~GameDBProcess();

public:
	void			Init( char *ip, UI16 usPort );
	void			Run();

	ITZSession*		GetSession() { return m_pGameDBSession; };

private:
	ITZSessionFactory< cGameDB >	m_sf;							// 세션 팩토리
	ITZSessionManager				m_sm;							// 세션 매니저
	ITZNetIOCP						m_iocp;							// IOCP 

	ITZSession*						m_pGameDBSession;				// 게임 DB매니저 세션 포인터

	char							m_gameDBManagerIP[ 20 ];		// 게임 DB매니저 IP
	UI16							m_gameDBManagerPort;			// 게임 DB매니저 PORT

};

#endif