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

	// 2007 / 02 / 23  무역서버때문에
	ITZSession*			Connect( SI32 siSessionId, char *ip, UI16 usPort, SI32* pReason = NULL );

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
