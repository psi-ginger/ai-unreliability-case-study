#ifndef _SERVERPROCESS_H
#define _SERVERPROCESS_H

#include "MasterServer.h"

#include "ITZSessionFactory.h"
#include "ITZSessionManager.h"
#include "ITZNetIOCP.h"

#include "ServerInfo.h"

class ServerProcess
{
public:
	ServerProcess();
	~ServerProcess();

public:
	void				Init();
	void				Run();

	void				SendPacketAll( char *pPacket );

	ITZSession*			GetGameServerSession(UI32 uiWorldId);
	
	ITZSessionFactory< cAgent >		m_sf;
	ITZSessionManager				m_sm;
	ITZNetIOCP						m_iocp;

	ITZSession*						m_serverSession[ MAX_SERVER_COUNT ];

	DWORD							m_dwLastChkTick;

	BOOL							m_bStartFrontServer;
	DWORD							m_dwStartFrontServerTick;

	DWORD							m_dwLastNewCharNum;
};

#endif

