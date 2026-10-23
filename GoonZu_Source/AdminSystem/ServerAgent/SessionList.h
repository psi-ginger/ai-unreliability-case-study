#ifndef _SESSIONLIST_H
#define _SESSIONLIST_H

#include "ServerAgent.h"

struct sSessionInfo
{
	sSessionInfo() {
	
		pMaster = NULL;
		strcpy( szMasterSessionMessage, "-Listening         "  );

		pServer = NULL;
		strcpy( szServerSessionMessage, "-Listening         " );

	};

	cMaster*	pMaster;
	char		szMasterSessionMessage[ 20 ];

	cServer*	pServer;
	char		szServerSessionMessage[ 20 ];

};

class SessionList
{
public:
	SessionList();
	~SessionList();

public:

	void				AddMasterSession( cMaster *pMaster, char *szMessage );
	void				AddServerSession( cServer *pServer, char *szMessage );
	
	void				DelMasterSession( cMaster *pMaster );
	void				DelServerSession( cServer *pServer );

	void				Wait() { EnterCriticalSection( &m_cs ); };
	void				Release() { LeaveCriticalSection( &m_cs ); };


public:
	sSessionInfo		m_sessionInfo[ 10 ];

	CRITICAL_SECTION	m_cs;
};



#endif
