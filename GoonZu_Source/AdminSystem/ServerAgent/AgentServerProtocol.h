#ifndef _AGENTSERVERPROTOCOL_H
#define _AGENTSERVERPROTOCOL_H

#include "ITZNetworkThread.h"
#include "ServerAgent.h"

class AgentServerProtocol : public ITZNetworkThread
{
public:
	AgentServerProtocol();
	~AgentServerProtocol();

public:
	void			Init();

	virtual void	Update();
	virtual void	SessionProc( ITZSession *pSession );

public:
	ITZSessionFactory< cServer >	m_SF;

};

#endif

