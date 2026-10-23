#ifndef _AUTHFRONTPROTOCOL_H
#define _AUTHFRONTPROTOCOL_H

#include "AuthServer.h"
#include "ITZNetworkThread.h"

class AuthFrontProtocol : public ITZNetworkThread
{
public:
	AuthFrontProtocol();
	~AuthFrontProtocol();

public:
	void			Init();

	virtual void	Update();
	virtual void	LogOut( ITZSession *pSession );
	virtual void	SessionProc( ITZSession *pSession );

public:
	ITZSessionFactory< cFront >	m_SF;
};


#endif
