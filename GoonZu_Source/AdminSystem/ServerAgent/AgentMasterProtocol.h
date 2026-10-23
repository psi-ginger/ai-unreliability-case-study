#ifndef _AGENTMASTERPROTOCOL_H
#define _AGENTMASTERPROTOCOL_H

#include "ITZNetworkThread.h"
#include "ServerAgent.h"

class AgentMasterProtocol : public ITZNetworkThread
{
public:
	AgentMasterProtocol();
	~AgentMasterProtocol();

public:
	void			Init();

	virtual void	Update();
	virtual void	SessionProc( ITZSession *pSession );

public:
	ITZSessionFactory< cMaster >	m_SF;

	// 실행 프로그램을 종료시킨다. KKM
	BOOL SafeTerminateProcess(HANDLE hProcess, UINT uExitCode);
};

#endif