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

	virtual void Init() { printf( "connected\n" ); };
	virtual void Close() { printf( "disconnected\n" ); };
};


class AgentProcess
{
public:
	AgentProcess();
	~AgentProcess();

	void				Init( char *szIP, UI16 usPort );
	void				Run();

	ITZSession*			GetAgentSession() { return m_pAgentSession; };

public:
	ITZSessionFactory< cAgent >		m_sf;							// 세션 팩토리
	ITZSessionManager				m_sm;							// 세션 매니저
	ITZNetIOCP						m_iocp;							// IOCP 

	ITZSession*						m_pAgentSession;				// 서버에이전트 세션 포인터

	char							m_szIP[ 16 ];
	UI16							m_usPort;

	DWORD							m_dwLastTick;

	BOOL							m_bSendFilePath;

};

#endif

