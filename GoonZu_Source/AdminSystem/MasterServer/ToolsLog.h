#ifndef _TOOLSLOG_H
#define _TOOLSLOG_H

#include "ITZSessionFactory.h"
#include "ITZSessionManager.h"
#include "ITZNetIOCP.h"
#include "ITZPerson.h"

#include "itzbase.h"

class CToolsLog : public ITZPerson
{
public:
	CToolsLog() {};
	~CToolsLog() {};

public:
	virtual void Init() {};
	virtual void Close() {};
};

class CToolsLogProcess
{
public:
	CToolsLogProcess();
	~CToolsLogProcess();

	void				Init( char *szIP, UI16 usPort );
	void				Run();

	ITZSession*			GetToolsLogSession() { return m_pToolsLogSession; };

	void				SendMsg( sPacketHeader *pPacket );

private:
	ITZSessionFactory< CToolsLog >	m_sf;									// 세션 팩토리
	ITZSessionManager				m_sm;									// 세션 매니저
	ITZNetIOCP						m_iocp;									// IOCP 

	ITZSession*						m_pToolsLogSession;						// 서버에이전트 세션 포인터

	char							m_szIP[ 16 ];
	UI16							m_usPort;

};

#endif 
