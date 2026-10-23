#ifndef _AGENTPROCESS_H
#define _AGENTPROCESS_H

#include "ITZSessionFactory.h"
#include "ITZSessionManager.h"
#include "ITZNetIOCP.h"
#include "ITZPerson.h"

#define AGENT_HEARTBEAT_INTERVAL		3000		// 서버에이전트로 보내는 허트비트 주기 (3초)

class cAgent : public ITZPerson
{
public:
	cAgent() {};
	~cAgent() {};

	virtual void Init() {};
	virtual void Close() {};
};


class AgentProcess
{
public:
	AgentProcess();
	~AgentProcess();

	void				Init( TCHAR *szIP, UI16 usPort );
	void				Run();

	ITZSession*			GetAgentSession() { return m_pAgentSession; };

	BOOL				IsInitReady() { return m_bInitReady; };

private:
	ITZSessionFactory< cAgent >		m_sf;							// 세션 팩토리
	ITZSessionManager				m_sm;							// 세션 매니저
	ITZNetIOCP						m_iocp;							// IOCP 

	ITZSession*						m_pAgentSession;				// 서버에이전트 세션 포인터

	TCHAR							m_szIP[ 16 ];
	UI16							m_usPort;

	DWORD							m_dwLastTick;

	BOOL							m_bSendFilePath;

	BOOL							m_bInitReady;

};

#endif

