#ifndef _ITZQUEUE_H
#define _ITZQUEUE_H

template< class Type >
class ITZQueue
{
public:
	ITZQueue();
	~ITZQueue();

public:
	void				CreateQueue( SI32 siMaxSize );
	void				ClearQueue();
	BOOL				Enqueue( const Type &rElem );
	BOOL				Dequeue( Type *pElem );
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
ITZQueue< Type >::ITZQueue()
{

	m_pQueue = NULL;

	m_siMaxSize = 0;
	m_siNumberOfElements = 0;

	InitializeCriticalSection( &m_cs );

}


template< class Type >
ITZQueue< Type >::~ITZQueue()
{

	delete [] m_pQueue;
	m_pQueue = NULL;

	DeleteCriticalSection( &m_cs );	
}

template< class Type >
void ITZQueue< Type >::CreateQueue( SI32 siMaxSize )
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
void ITZQueue< Type >::ClearQueue()
{
	EnterCriticalSection( &m_cs );	

	m_siBeginPos = 0;
	m_siEndPos = 0;	
	m_siNumberOfElements = 0;

	LeaveCriticalSection( &m_cs );

}


template< class Type >
BOOL ITZQueue< Type >::Enqueue( const Type &rElem )
{
	EnterCriticalSection( &m_cs );

	if( m_siMaxSize == m_siNumberOfElements ) {

		LeaveCriticalSection( &m_cs );	

		return FALSE;
	}

	m_pQueue[ m_siEndPos ] = rElem;

	++m_siEndPos;

	if( m_siEndPos == m_siMaxSize ) m_siEndPos = 0;

	++m_siNumberOfElements;
	
	LeaveCriticalSection( &m_cs );

	return TRUE;

}

template< class Type >
BOOL ITZQueue< Type >::Dequeue( Type *pElem )
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

