#ifndef _AUTHGAMEPROTOCOL_H
#define _AUTHGAMEPROTOCOL_H

#include "AuthServer.h"
#include "ITZNetworkThread.h"

#include "NUpdateTimer.h"

class AuthGameProtocol : public ITZNetworkThread
{
public:
	AuthGameProtocol();
	~AuthGameProtocol();

public:
	void			Init(UI16 usGameServerListenPort);

	virtual void	Update();
	virtual void	LogOut( ITZSession *pSession );
	virtual void	SessionProc( ITZSession *pSession );

public:
	ITZSessionFactory< cGame >	m_SF;

private:
//	NUpdateTimer	m_kUpdateTimer_GamepotJP;
//	NUpdateTimer	m_kUpdateTimer_HangameJP;

};


#endif
