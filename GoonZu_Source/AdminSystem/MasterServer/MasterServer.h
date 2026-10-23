#ifndef _MASTERSERVER_H
#define _MASTERSERVER_H

#include "ITZPerson.h"

class MasterClientProtocol;
class GMTOOLClientProtocol;
class ServerProcess;
class AccountDBProcess;
class ServerInfo;
class VersionInfo;
class CToolsLogProcess;


extern	MasterClientProtocol	g_mcp;
extern	ServerProcess			g_sp;
extern	AccountDBProcess		g_ap;
extern	ServerInfo				g_serverinfo;
extern	VersionInfo				g_versioninfo;
extern  CToolsLogProcess		g_ToolsLog;
extern	int						g_maxUser;

const SI32	ConstIDSize = 20;

class cAgent : public ITZPerson
{
public:
	cAgent() { m_bSendServerInfo = FALSE; };
	~cAgent() {};

public:
	virtual void Init() { m_bSendServerInfo = FALSE; 						  
						m_dwLastRecvTick = GetTickCount();  					  
						m_dwLastSendTick = GetTickCount() - 5000;

	};
	
	virtual void Close() { m_bSendServerInfo = FALSE; 
	};

	BOOL	m_bSendServerInfo;

	DWORD	m_dwLastRecvTick;
	DWORD	m_dwLastSendTick;
};


class cClient : public ITZPerson
{
protected:
	
public:
	cClient() {};
	~cClient() {};

public:
	virtual void Init() { InterlockedExchange( (LPLONG)&m_bLoginOK, FALSE );
						  m_dwLastSendTick = GetTickCount() - 5000; m_nMasterLevel = 0; }
	virtual void Close() { InterlockedExchange( (LPLONG)&m_bLoginOK, FALSE ); }

public:
	DWORD	m_dwLastSendTick;
	BOOL	m_bLoginOK;
	int		m_nMasterLevel;
	
	TCHAR	m_szID[ConstIDSize];			// 접속자 아이디
};

#endif
