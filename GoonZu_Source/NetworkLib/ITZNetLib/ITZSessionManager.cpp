#include "ITZSessionManager.h"
#include "ITZPerson.h"


ITZSessionManager::ITZSessionManager()
{
	m_pSessionList = NULL;
	m_pSessionPtrList = NULL;
	
	m_siMaxSize = 0;
}

ITZSessionManager::~ITZSessionManager()
{
	delete m_pSessionPtrList;
	
	m_pSessionPtrList = NULL;

	delete m_pSessionList;

	m_pSessionList = NULL;

}

ITZSession* ITZSessionManager::Connect( SI32 siSessionId, char *ip, UI16 usPort, SI32* pReason )
{

	if( siSessionId < 0 || siSessionId >= m_pSessionList->m_sizeFactory ) return NULL;


	ITZSession *pSession;

	pSession = GetSession( siSessionId );

	if( pSession == NULL ) 
	{
		if( NULL != pReason )
			*pReason = 1;
		return NULL;
	}

	if( pSession->GetState() != SESSION_STATE_NONE ) 
	{
		if( NULL != pReason )
			*pReason = 2;
		return NULL;
	}

	if( pSession->IsActive() == TRUE ) 
	{
		if( NULL != pReason )
			*pReason = 3;
		return NULL;
	}

	if( pSession->InitConnect( ip, usPort ) == FALSE ) 
	{
		if( NULL != pReason )
			*pReason = 4;
		return NULL;
	}

	if( pSession->_connect() == FALSE ) 
	{
		if( NULL != pReason )
			*pReason = 5;
		return NULL;
	}

	if( m_pSessionList->push_index( siSessionId ) != pSession )
	{
		if( NULL != pReason )
			*pReason = 6;
		pSession->CloseSocket();
		return NULL;
	};


	pSession->SetSocket( pSession->m_socket, pSession->m_sockaddr );

	return pSession;
}

ITZSession* ITZSessionManager::NewSocket( SOCKET socket, SOCKADDR_IN sockaddr )
{

	if( m_pSessionList == NULL ) return NULL;

	ITZSession *pSession;

	pSession = m_pSessionList->push();

	if( pSession == NULL ) return NULL;

	pSession->SetSocket( socket, sockaddr );

	return pSession;
}



void ITZSessionManager::DelSession( ITZSession *pSession )
{
	if( pSession == NULL ) return;

	pSession->CloseSocket();

	m_pSessionList->remove( pSession );
}


void ITZSessionManager::Update()
{
	m_pSessionList->batch();
}

ITZSession* ITZSessionManager::GetSession( SI32 index )
{
	return m_pSessionList->get( index );
}