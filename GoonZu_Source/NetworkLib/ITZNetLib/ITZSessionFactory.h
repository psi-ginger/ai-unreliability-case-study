#ifndef _ITZSESSIONFACTORY_H
#define _ITZSESSIONFACTORY_H

#include "ITZList.h"
#include "ITZSessionManager.h"

template< class Type >
class ITZSessionFactory
{
friend class ITZSessionManager;
public:
	ITZSessionFactory();
	~ITZSessionFactory();

	void CreateFactory(  ITZSessionManager *pSessionManager, int maxsize, SI32 siRecvBufSize, SI32 siSendBufSize, SI32 headerType = 0, bool bEncrypt = false )
	{

		delete m_pPersonList;
		delete m_pSessionList;
		delete m_pSessionPtrList;

		m_pPersonList = new ITZList< Type >;
		m_pSessionList = new ITZList< ITZSession >;
		m_pSessionPtrList = new ITZList< ITZObjectPtr >;

		m_pPersonList->CreateList( maxsize, TRUE );
		m_pSessionList->CreateBatchList( maxsize, TRUE );
		m_pSessionPtrList->CreateList( maxsize, TRUE );

		Type			*pPerson;
		ITZSession		*pSession;
		ITZObjectPtr	*pSessionPtr;

		pSession = m_pSessionList->getFreeFirst();

		for( int i = 0; i < maxsize; ++i ) {

			pPerson = m_pPersonList->get( i );

			pPerson->m_uiCharID = i + 1;

			pSessionPtr = m_pSessionPtrList->push();

			pSession->SetBuffer( siRecvBufSize, siSendBufSize, headerType, bEncrypt );
			pSession->SetPerson( pPerson );

			pPerson->SetSession( pSession );

			pSessionPtr->SetObject( (ITZObject *)pSession );

			pSession = m_pSessionList->next( pSession );
		}
			
		pSessionManager->m_pSessionList = m_pSessionList;
		pSessionManager->m_pSessionPtrList = m_pSessionPtrList;
		pSessionManager->m_siMaxSize = maxsize;
		
		m_siMaxSize = maxsize;

	};

	Type* GetPerson( int uiCharID ) 
	{ 
		if( uiCharID < 1 || uiCharID > m_siMaxSize ) return NULL;
		return (Type *)&m_pPersonList->m_pFactory[ uiCharID - 1 ]; 
	};


public:
	ITZList< ITZSession >*		m_pSessionList;
	ITZList< Type >*			m_pPersonList;
	
	ITZList< ITZObjectPtr >*	m_pSessionPtrList;

	SI32						m_siMaxSize;
};

template< class Type >
ITZSessionFactory< Type >::ITZSessionFactory()
{
	m_pSessionList = NULL;
	m_pPersonList = NULL;
	m_pSessionPtrList = NULL;
}

template< class Type >
ITZSessionFactory< Type >::~ITZSessionFactory()
{
	delete m_pPersonList;
	m_pPersonList = NULL;
	// sessionList와 sessionPtrList는 SessionManager에서 해제한다
}

#endif
