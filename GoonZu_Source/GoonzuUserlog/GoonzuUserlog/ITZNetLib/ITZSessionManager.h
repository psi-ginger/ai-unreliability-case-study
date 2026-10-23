#ifndef _ITZSESSIONMANAGER_H
#define _ITZSESSIONMANAGER_H

#include "ITZSession.h"
#include "ITZList.h"

class ITZSessionManager
{
public:
	ITZSessionManager();
	~ITZSessionManager();

public:

	ITZSession*			Connect( SI32 siSessionId, char *ip, UI16 usPort );

	ITZSession*			NewSocket( SOCKET socket, SOCKADDR_IN sockaddr );

	void				DelSession( ITZSession *pSession );

	ITZSession*			GetFirstSession() { return m_pSessionList->getFirst(); };
	ITZSession*			NextSession( ITZSession *pSession ) { return (ITZSession *)(pSession->GetNext()); };

	ITZSession*			GetSession( SI32 index );

	void				Update();

	int					GetActiveSessionCount() { return m_pSessionList->m_masterList->size(); };

public:
	SI32				m_siMaxSize;

	ITZList< ITZSession >*		m_pSessionList;
	ITZList< ITZObjectPtr >*	m_pSessionPtrList;
};

#endif
