#ifndef _GAMEDBPROCESS2_H
#define _GAMEDBPROCESS2_H

#include "ITZSessionFactory.h"
#include "ITZSessionManager.h"
#include "ITZNetIOCP.h"
#include "ITZPerson.h"

class cGameDB2 : public ITZPerson
{
public:
	cGameDB2() {};
	~cGameDB2() {};
};


class GameDBProcess2
{
private:

public:
	GameDBProcess2();
	~GameDBProcess2();

public:
	void			Init( TCHAR *ip, UI16 usPort );
	void			Run();

	bool			SendMsg(sPacketHeader *pPacket);

	ITZSession*		GetSession() { return m_pGameDBSession; };

private:
	ITZSessionFactory< cGameDB2 >	m_sf;							// 세션 팩토리
	ITZSessionManager				m_sm;							// 세션 매니저
	ITZNetIOCP						m_iocp;							// IOCP 

	ITZSession*						m_pGameDBSession;				// 게임 DB매니저 세션 포인터

	TCHAR							m_gameDBManagerIP[ 20 ];		// 게임 DB매니저 IP
	UI16							m_gameDBManagerPort;			// 게임 DB매니저 PORT

};

#endif