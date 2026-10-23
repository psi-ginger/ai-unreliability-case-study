#ifndef _ITZCAQUEUE_H
#define _ITZCAQUEUE_H

template< class Type >
class ITZCAQueue
{
public:
	ITZCAQueue();
	~ITZCAQueue();

public:
	void				CreateQueue( SI32 siMaxSize );
	void				ClearQueue();
	UI16				Enqueue( const Type &rElem );
	UI16				Dequeue( Type *pElem );
	SI32				GetSize() { return m_siNumberOfElements; };

private:
	Type	 			*m_pQueue;

	SI32				m_siMaxSize;
	SI32				m_siBeginPos;
	SI32				m_siEndPos;
	
	SI32				m_siNumberOfElements;

	CRITICAL_SECTION	m_cs;

};


template< class Type >
ITZCAQueue< Type >::ITZCAQueue()
{

	m_pQueue = NULL;

	m_siMaxSize = 0;
	m_siNumberOfElements = 0;

	InitializeCriticalSection( &m_cs );

}


template< class Type >
ITZCAQueue< Type >::~ITZCAQueue()
{

	delete [] m_pQueue;

	DeleteCriticalSection( &m_cs );	
}

template< class Type >
void ITZCAQueue< Type >::CreateQueue( SI32 siMaxSize )
{
	EnterCriticalSection( &m_cs );	

	if( m_pQueue ) delete [] m_pQueue;

	m_pQueue = new Type[ siMaxSize ];

	m_siMaxSize = siMaxSize;
	
	m_siBeginPos = 0;
	m_siEndPos = 0;
	m_siNumberOfElements = 0;
	
	LeaveCriticalSection( &m_cs );	

}

template< class Type >
void ITZCAQueue< Type >::ClearQueue()
{
	EnterCriticalSection( &m_cs );	

	m_siBeginPos = 0;
	m_siEndPos = 0;	
	m_siNumberOfElements = 0;

	LeaveCriticalSection( &m_cs );

}


template< class Type >
UI16 ITZCAQueue< Type >::Enqueue( const Type &rElem )
{
	EnterCriticalSection( &m_cs );

	UI16 ret = m_siEndPos;

	// ²Ë Ã¡À¸¸é,
	if( m_siMaxSize == m_siNumberOfElements ) {

		m_pQueue[ m_siEndPos ] = rElem;

		if( m_siBeginPos == m_siMaxSize - 1 ) {

			m_siBeginPos = m_siEndPos = 0;

		} else {
			++m_siBeginPos;
			++m_siEndPos;
		}

		LeaveCriticalSection( &m_cs );	

		return ret;
	}

	m_pQueue[ m_siEndPos ] = rElem;

	++m_siEndPos;

	if( m_siEndPos == m_siMaxSize ) m_siEndPos = 0;

	++m_siNumberOfElements;
	
	LeaveCriticalSection( &m_cs );

	return ret;
}

template< class Type >
BOOL ITZCAQueue< Type >::Dequeue( Type *pElem )
{
	EnterCriticalSection( &m_cs );

	if( m_siNumberOfElements == 0 ) {

		LeaveCriticalSection( &m_cs );

		return FALSE;
	}

	*pElem = m_pQueue[ m_siBeginPos ];

	++m_siBeginPos;

	if( m_siBeginPos == m_siMaxSize ) m_siBeginPos = 0;

	--m_siNumberOfElements;

	LeaveCriticalSection( &m_cs );

	return TRUE;
}

#endif
