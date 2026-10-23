//-----------------------------------------------------------------------------
// 2005 ¿Ã¡¶øœ
//-----------------------------------------------------------------------------

#ifndef _NLIST_H
#define _NLIST_H

#include "NArray.h"

template< class Type >
class NList;

template< class Type >
class NListAllocator;

//-----------------------------------------------------------------------------
// NListNode
//-----------------------------------------------------------------------------
template< class Type >
class NListNode
{
public:
	friend class NList;
	friend class NListAllocator;

	NListNode() : m_index(0), m_pList(0), m_pPrev(0), m_pNext(0) {}
	~NListNode() {}

	inline int					GetIndex() { return m_index; }
	inline NListNode< Type >*	GetList() { return m_pList; }
	inline NListNode< Type >*	GetPrev() { return m_pPrev; }
	inline NListNode< Type >*	GetNext() { return m_pNext; }

	inline Type&	GetData() { return m_data; }
	inline Type*	GetDataPtr() { return &m_data; }

	inline void		Remove() { m_pList->Remove( this ); }

private:
	int						m_index;

	NList< Type >*			m_pList;

	NListNode< Type >*		m_pPrev;
	NListNode< Type >*		m_pNext;

	Type					m_data;
};


//-----------------------------------------------------------------------------
// NListAllocator
//-----------------------------------------------------------------------------
template< class Type >
class NListAllocator
{
public:
	friend class NList;

	NListAllocator( int maxsize ) 
	{ 
		InitializeCriticalSection( &m_cs );
		CreateListAllocator( maxsize ); 
	}
	
	NListAllocator() { InitializeCriticalSection( &m_cs ); }
	~NListAllocator() { DeleteCriticalSection( &m_cs ); }

	inline void	lock() { EnterCriticalSection( &m_cs ); }
	inline void unlock() { LeaveCriticalSection( &m_cs ); }

	void	CreateListAllocator( int maxsize )
	{
		m_array.CreateArray( maxsize );

		NListNode< Type > *pNode;

		for( int i = 0; i < maxsize; ++i )
		{
			pNode = m_array.GetDataPtr( i );

			pNode->m_index = i;

			m_list.PushBack( pNode );
		}
	}

	inline int		GetMaxSize() 
	{ 
		return m_array.GetMaxSize(); 
	}

	inline Type&	GetData( int i ) 
	{ 
		return m_array.GetDataPtr( i )->GetData(); 
	}

	inline Type*	GetDataPtr( int i ) 
	{ 
		return m_array.GetDataPtr( i )->GetDataPtr(); 
	}

	inline Type*	GetSafeDataPtr( int i ) 
	{
		Node *pNode = m_array.GetSafeDataPtr( i );

		if( pNode ) return pNode->GetDataPtr();
		return NULL;
	}

	inline NListNode< Type >&	GetNode( int i )
	{
		return m_array.GetData( i );
	}

	inline NListNode< Type >*	GetNodePtr( int i ) 
	{ 
		return m_array.GetDataPtr( i ); 
	}
	
private:
	NArray< NListNode< Type > >		m_array;
	NList< Type >					m_list;

	CRITICAL_SECTION				m_cs;
	
};

//-----------------------------------------------------------------------------
// NList
//-----------------------------------------------------------------------------
template< class Type >
class NList
{
public:
	friend class NListAllocator;

	NList();
	NList( NListAllocator< Type > *pListAllocator );
	NList( int ListAllocatorSize );

	~NList();

	void		CreateList( NListAllocator< Type > *pListAllocator );
	void		CreateList( int ListAllocatorSize );					// self ListAllocator

	void		Destroy();

	inline NListAllocator< Type >* GetListAllocator() { return m_pListAllocator; }

	inline int					GetCount() { return m_siCount; }

	inline NListNode< Type >*	GetFirst() { return m_pFirst; }
	inline NListNode< Type >*	GetLast() { return m_pLast; }

	Type*						PushFrontGetData();
	Type*						PushBackGetData();

	Type*						RemoveFrontGetData();
	Type*						RemoveBackGetData();

	NListNode< Type >*			PushFront();
	NListNode< Type >*			PushBack();

	NListNode< Type >*			RemoveFront();
	NListNode< Type >*			RemoveBack();

	NListNode< Type >*			Remove( int index );
	NListNode< Type >*			Remove( NListNode< Type > *pNode );

	NListNode< Type >*			Find( Type data );

private:
	NListNode< Type >*			PushFront( NListNode< Type > *pNode );
	NListNode< Type >*			PushBack( NListNode< Type > *pNode );

	NListNode< Type >*			PopFront();
	NListNode< Type >*			PopBack();


private:
	BOOL						m_bSelfListAllocator;
	NListAllocator< Type >*		m_pListAllocator;

	NListNode< Type >*			m_pFirst;
	NListNode< Type >*			m_pLast;

	int							m_siCount;

};


//-----------------------------------------------------------------------------
// NList
//-----------------------------------------------------------------------------
template< class Type >
NList< Type >::NList()
{
	m_pFirst = NULL;
	m_pLast = NULL;

	m_siCount = 0;

	m_bSelfListAllocator = FALSE;
	m_pListAllocator = NULL;
}

template< class Type >
NList< Type >::NList( NListAllocator< Type > *pListAllocator )
{
	m_pFirst = NULL;
	m_pLast = NULL;

	m_siCount = 0;	

	m_bSelfListAllocator = FALSE;
	m_pListAllocator = pListAllocator;
}

template< class Type >
NList< Type >::NList( int ListAllocatorSize )
{
	m_pFirst = NULL;
	m_pLast = NULL;

	m_siCount = 0;	

	m_bSelfListAllocator = TRUE;

	m_pListAllocator = new NListAllocator< Type >
	m_pListAllocator->CreateListAllocator( ListAllocatorSize );
}

template< class Type >
NList< Type >::~NList()
{
	Destroy();
}

template< class Type >
void NList< Type >::Destroy()
{
	NListNode< Type > *pNode, *pTemp;

	pNode = m_pFirst;

	while( pNode ) 
	{
		if( pNode->m_pList == this ) break;

		pTemp = pNode;
		pNode = pNode->GetNext();

		Remove( pTemp );
	}

	if( m_bSelfListAllocator && m_pListAllocator )
	{
		delete m_pListAllocator;
	}

	m_pFirst = NULL;
	m_pLast = NULL;

	m_siCount = 0;

	m_bSelfListAllocator = FALSE;
	m_pListAllocator = NULL;
}

template< class Type >
void NList< Type >::CreateList( NListAllocator< Type > *pListAllocator )
{
	Destroy();

	m_pListAllocator = pListAllocator;
}

template< class Type >
void NList< Type >::CreateList( int ListAllocatorSize )
{
	Destroy();

	m_bSelfListAllocator = TRUE;

	m_pListAllocator = new NListAllocator< Type >;
	m_pListAllocator->CreateListAllocator( ListAllocatorSize );
}

template< class Type >
inline Type* NList< Type >::PushFrontGetData()
{
	NListNode< Type > *pNode;	
	
	pNode = PushFront( m_pListAllocator->m_list.PopFront() );

	if( pNode ) return &pNode->m_data;

	return NULL;
}

template< class Type >
inline Type* NList< Type >::PushBackGetData()
{
	NListNode< Type > *pNode;	
	
	pNode = PushBack( m_pListAllocator->m_list.PopFront() );

	if( pNode ) return &pNode->m_data;

	return NULL;
}

template< class Type >
inline Type* NList< Type >::RemoveFrontGetData()
{
	NListNode< Type > *pNode;

	pNode = m_pListAllocator->m_list.PushBack( PopFront() );

	if( pNode ) return &pNode->m_data;

	return NULL;
}

template< class Type >
inline Type* NList< Type >::RemoveBackGetData()
{
	NListNode< Type > *pNode;

	pNode = m_pListAllocator->m_list.PushBack( PopBack() );

	if( pNode ) return &pNode->m_data;

	return NULL;
}

template< class Type >
inline NListNode< Type >* NList< Type >::PushFront()
{
	return PushFront( m_pListAllocator->m_list.PopFront() );
}

template< class Type >
inline NListNode< Type >* NList< Type >::PushBack()
{
	 return PushBack( m_pListAllocator->m_list.PopFront() );
}

template< class Type >
inline NListNode< Type >* NList< Type >::RemoveFront()
{
	return m_pListAllocator->m_list.PushBack( PopFront() );
}

template< class Type >
inline NListNode< Type >* NList< Type >::RemoveBack()
{	
	return m_pListAllocator->m_list.PushBack( PopBack() );
}

template< class Type >
inline NListNode< Type >* NList< Type >::Remove( int index )
{

	NListNode<Type> *pNode;

	pNode = m_pListAllocator->m_array.GetDataPtr( index );

	return Remove( pNode );
}

template< class Type >
NListNode< Type >* NList< Type >::Remove( NListNode<Type> *pNode )
{

	if( pNode->m_pList != this ) return NULL;

	if( pNode == m_pFirst ) {

		m_pFirst = pNode->m_pNext;

	} else {
		
		pNode->m_pPrev->m_pNext = pNode->m_pNext;
	}

	if( pNode == m_pLast ) {

		m_pLast = pNode->m_pPrev;

	} else {

		pNode->m_pNext->m_pPrev = pNode->m_pPrev;
	}

	m_siCount--;

	m_pListAllocator->m_list.PushBack( pNode );

	return pNode;
}

template< class Type >
inline NListNode< Type >* NList< Type >::Find( Type data )
{

	NListNode< Type > *pNode = m_pFirst;

	while( pNode )
	{
		if( pNode->m_data == data ) return pNode;

		pNode = pNode->m_pNext;
	}

	return NULL;
}


template< class Type >
NListNode< Type >* NList< Type >::PushFront( NListNode< Type > *pNode )
{

	if( pNode == NULL ) return NULL;

	pNode->m_pList = this;

	if( m_siCount == 0 ) {

		pNode->m_pPrev = NULL;
		pNode->m_pNext = NULL;

		m_pFirst = m_pLast = pNode;

	} else {

		pNode->m_pPrev = NULL;
		pNode->m_pNext = m_pFirst;

		m_pFirst->m_pPrev = pNode;
 
		m_pFirst = pNode;

	}

	m_siCount++;

	return pNode;
}

template< class Type >
NListNode< Type >* NList< Type >::PushBack( NListNode< Type > *pNode )
{

	if( pNode == NULL ) return NULL;

	pNode->m_pList = this;

	if( m_siCount == 0 ) {

		pNode->m_pPrev = NULL;
		pNode->m_pNext = NULL;

		m_pFirst = m_pLast = pNode;

	} else {

		pNode->m_pNext = NULL;
		pNode->m_pPrev = m_pLast;

		m_pLast->m_pNext = pNode;

		m_pLast = pNode;

	}	

	m_siCount++;

	return pNode;
}

template< class Type >
NListNode< Type >* NList< Type >::PopFront()
{
	
	NListNode< Type > *pNode;

	if( m_siCount == 0 ) return NULL;		

	pNode = m_pFirst;

	pNode->m_pList = NULL;

	if( m_siCount == 1 ) {		
		m_pFirst = NULL;
		m_pLast = NULL;

		return pNode;
	}

	m_pFirst = pNode->m_pNext;
	m_pFirst->m_pPrev = NULL;

	m_siCount--;

	return pNode;
}

template< class Type >
NListNode< Type >* NList< Type >::PopBack()
{

	NListNode< Type > *pNode;

	if( m_siCount == 0 ) return NULL;		

	pNode = m_pLast;

	pNode->m_pList = NULL;

	if( m_siCount == 1 ) {		
		m_pFirst = NULL;
		m_pLast = NULL;

		return pNode;
	}

	m_pLast = pNode->m_pPrev;
	m_pLast->m_pNext = NULL;

	m_siCount--;
	
	return pNode;
}


#endif