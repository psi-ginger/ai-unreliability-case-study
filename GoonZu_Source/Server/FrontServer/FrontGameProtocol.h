
#include "ITZNetworkThread.h"

#include "FrontServer.h"

#define MAX_GAMESERVER_CONNECTION		20

class FrontGameProtocol : public ITZNetworkThread
{
public:
	FrontGameProtocol();
	~FrontGameProtocol();

public:
	void			Init(UI16 siListenPort);

	virtual void	Update();
	virtual void	LogOut( ITZSession *pSession );
	virtual void	SessionProc( ITZSession *pSession );

	void			RequestGameServerInfo( ITZSession *pSession );


public:
	ITZSessionFactory< cGame >	m_SF;

};

