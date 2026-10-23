#ifndef _AUTHGAMEPROTOCOL_H
#define _AUTHGAMEPROTOCOL_H

#include "AuthServer.h"
#include "ITZNetworkThread.h"

class AuthGameProtocol : public ITZNetworkThread
{
public:
	AuthGameProtocol();
	~AuthGameProtocol();

public:
	void			Init();

	virtual void	Update();
	virtual void	LogOut( ITZSession *pSession );
	virtual void	SessionProc( ITZSession *pSession );

public:
	ITZSessionFactory< cGame >	m_SF;

};


#endif
