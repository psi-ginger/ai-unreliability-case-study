#include "SessionList.h"


SessionList::SessionList()
{
	InitializeCriticalSection( &m_cs );
}


SessionList::~SessionList()
{
	DeleteCriticalSection( &m_cs );
}


void SessionList::AddMasterSession( cMaster *pMaster, char *szMessage )
{
	
	for( int i = 0; i < 10; i++ ) {

		if( m_sessionInfo[ i ].pServer == pMaster->m_pServer &&
			m_sessionInfo[ i ].pMaster == NULL ) {

			m_sessionInfo[ i ].pMaster = pMaster;
			
			memcpy( m_sessionInfo[ i ].szMasterSessionMessage, szMessage, 20 );
			m_sessionInfo[ i ].szMasterSessionMessage[ 19 ] = NULL;

			break;
		}

	}

	if( i == 10 ) {

		for( i = 0; i < 10; i++ ) {
			if( m_sessionInfo[ i ].pMaster == NULL && 
				m_sessionInfo[ i ].pServer == NULL ) {

				m_sessionInfo[ i ].pMaster = pMaster;
			
				memcpy( m_sessionInfo[ i ].szMasterSessionMessage, szMessage, 20 );
				m_sessionInfo[ i ].szMasterSessionMessage[ 19 ] = NULL;

				break;
			}
		}
	}

}


void SessionList::AddServerSession( cServer *pServer, char *szMessage )
{

	for( int i = 0; i < 10; i++ ) {

		if( m_sessionInfo[ i ].pMaster == pServer->m_pMaster &&
			m_sessionInfo[ i ].pServer == NULL ) {

			m_sessionInfo[ i ].pServer = pServer;
			
			memcpy( m_sessionInfo[ i ].szServerSessionMessage, szMessage, 20 );
			m_sessionInfo[ i ].szServerSessionMessage[ 19 ] = NULL;

			break;
		}

	}

	if( i == 10 ) {

		for( i = 0; i < 10; i++ ) {
			if( m_sessionInfo[ i ].pMaster == NULL && 
				m_sessionInfo[ i ].pServer == NULL ) {

				m_sessionInfo[ i ].pServer = pServer;
			
				memcpy( m_sessionInfo[ i ].szServerSessionMessage, szMessage, 20 );
				m_sessionInfo[ i ].szServerSessionMessage[ 19 ] = NULL;

				break;
			}
		}
	}


}

void SessionList::DelMasterSession( cMaster *pMaster )
{

	for( int i = 0; i < 10; i++ ) {

		if( m_sessionInfo[ i ].pMaster == pMaster ) {
			m_sessionInfo[ i ].pMaster = NULL;

			strcpy( m_sessionInfo[ i ].szMasterSessionMessage, "-Listening         " );
		}
	}

}

void SessionList::DelServerSession( cServer *pServer )
{

	for( int i = 0; i < 10; i++ ) {

		if( m_sessionInfo[ i ].pServer == pServer ) {
			m_sessionInfo[ i ].pServer = NULL;

			strcpy( m_sessionInfo[ i ].szServerSessionMessage, "-Listening         " );
		}
	}

}