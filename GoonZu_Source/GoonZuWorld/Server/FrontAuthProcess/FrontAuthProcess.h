#ifndef _FRONTAUTHPROCESS_H
#define _FRONTAUTHPROCESS_H

#include "ITZSessionFactory.h"
#include "ITZSessionManager.h"
#include "ITZNetIOCP.h"
#include "ITZPerson.h"

#include "../../Server/FrontServer/FrontMsg.h"


#define FRONT_HEARTBEAT_INTERVAL	3000							// 프론트 서버로 보내는 허트비트 주기 (3초)
#define AUTH_HEARTBEAT_INTERVAL		5000							// 인증 서버로 보내는 허트비트 주기 (5초)

class cFrontAuth : public ITZPerson
{
public:
	cFrontAuth() {};
	~cFrontAuth() {};
};


class FrontAuthProcess
{
public:
	FrontAuthProcess();
	~FrontAuthProcess();

	void				Init( TCHAR *frontIP, UI16 frontPort, TCHAR *authIP, UI16 authPort );
	void				Run();

	ITZSession*			GetFrontSession() { return m_pFrontSession; };
	ITZSession*			GetAuthSession() { return m_pAuthSession; };

private:
	ITZSessionFactory< cFrontAuth >	m_sf;							// 세션 팩토리
	ITZSessionManager				m_sm;							// 세션 매니저
	ITZNetIOCP						m_iocp;							// IOCP 

	ITZSession*						m_pFrontSession;				// 프론트 서버 세션 포인터
	ITZSession*						m_pAuthSession;					// 인증 서버 세션 포인터

	TCHAR							m_frontIP[ 20 ];				// 프론트 서버 IP
	UI16							m_frontPort;					// 프론트 서버 PORT

	TCHAR							m_authIP[ 20 ];					// 인증 서버 IP
	UI16							m_authPort;						// 인증 서버 PORT

	DWORD							m_frontLastTick;
	DWORD							m_authLastTick;

public:
	GAMESERVER_REFRESHINFO			m_refreshInfo;

public:
	bool SendMsg(sPacketHeader* pPacket);
};



#endif