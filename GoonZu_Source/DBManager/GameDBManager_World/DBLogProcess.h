#ifndef _DBLOGPROCESS_H
#define _DBLOGPROCESS_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ITZSessionFactory.h"
#include "ITZSessionManager.h"
#include "ITZNetIOCP.h"
#include "ITZPerson.h"

#include "itzbase.h"
#include "GameDB.h"


class DBLogProcess
{
public:
	DBLogProcess();
	~DBLogProcess();

	void				Init( char *szIP, UI16 usPort );
	void				Run();

	ITZSession*			GetLogSession() { return m_pLogSession; };

	void				SendMsg( sPacketHeader *pPacket );

private:
	ITZSessionFactory< cLog >		m_sf;									// 세션 팩토리
	ITZSessionManager				m_sm;									// 세션 매니저
	ITZNetIOCP						m_iocp;									// IOCP 

	ITZSession*						m_pLogSession;							// 서버에이전트 세션 포인터

	char							m_szIP[ 16 ];
	UI16							m_usPort;
	
};

#endif 
