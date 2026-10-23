#include "ITZQList.h"
#include "ITZObject.h"


ITZQList::ITZQList()
{
	
	m_siNumberOfElements = 0;

	m_pFirst = NULL;
	m_pLast = NULL;

	InitializeCriticalSection( &m_cs );
}

ITZQList::~ITZQList()
{
	DeleteCriticalSection( &m_cs );
}

void ITZQList::clear()
{

	EnterCriticalSection( &m_cs );

	m_siNumberOfElements = 0;

	m_pFirst = NULL;
	m_pLast = NULL;

	LeaveCriticalSection( &m_cs );
}


void ITZQList::push( ITZObject *pElem )
{

	EnterCriticalSection( &m_cs );

	if( m_siNumberOfElements == 0 ) {

		pElem->m_pPrev = NULL;
		pElem->m_pNext = NULL;

		m_pFirst = pElem;
		m_pLast = pElem;

	} else {

		pElem->m_pNext = NULL;
		pElem->m_pPrev = m_pLast;

		m_pLast->m_pNext = pElem;

		m_pLast = pElem;
	}	

	// 노드 개수 증가
	InterlockedIncrement( (LPLONG)&m_siNumberOfElements );

	LeaveCriticalSection( &m_cs );

}


ITZObject* ITZQList::pop()
{
	EnterCriticalSection( &m_cs );

	ITZObject* pElem;

	if( m_siNumberOfElements == 0 ) {
		LeaveCriticalSection( &m_cs );
		return NULL;
	}

	if( m_siNumberOfElements == 1 ) {


		if( m_pFirst == m_pLast ) {

			pElem = m_pFirst;

			m_pFirst = m_pLast = NULL;

			// 노드 개수 감소
			InterlockedDecrement( (LPLONG)&m_siNumberOfElements );

			LeaveCriticalSection( &m_cs );

			return pElem;

		}

		LeaveCriticalSection( &m_cs );

		return NULL;
		
	}

	pElem = m_pFirst;
		
	m_pFirst = m_pFirst->m_pNext;
	m_pFirst->m_pPrev = NULL;

	// 노드 개수 감소
	InterlockedDecrement( (LPLONG)&m_siNumberOfElements );

	LeaveCriticalSection( &m_cs );

	return pElem;

}

void ITZQList::remove( ITZObject *pElem )
{

	EnterCriticalSection( &m_cs );

	if( pElem == m_pFirst ) {
		
		m_pFirst = pElem->m_pNext;

	} else {

		pElem->m_pPrev->m_pNext = pElem->m_pNext;
	}

	if( pElem == m_pLast ) {

		m_pLast = pElem->m_pPrev;

	} else {

		pElem->m_pNext->m_pPrev = pElem->m_pPrev;

	}
	// 노드 개수 감소
	InterlockedDecrement( (LPLONG)&m_siNumberOfElements );

	LeaveCriticalSection( &m_cs );
}
