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

ITZSession* ITZSessionManager::Connect( SI32 siSessionId, char *ip, UI16 usPort )
{

	if( siSessionId < 0 || siSessionId >= m_pSessionList->m_sizeFactory ) return NULL;


	ITZSession *pSession;
	
	pSession = GetSession( siSessionId );

	if( pSession == NULL ) return NULL;

	if( pSession->GetState() != SESSION_STATE_NONE ) return NULL;

	if( pSession->IsActive() == TRUE ) return NULL;

	if( pSession->InitConnect( ip, usPort ) == FALSE ) return NULL;

	if( pSession->_connect() == FALSE ) return NULL;

	if( m_pSessionList->push_index( siSessionId ) != pSession ) {

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