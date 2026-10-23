#ifndef _ITZList_H
#define _ITZList_H

#include "itzbase.h"
#include "ITZQueue.h"
#include "ITZQList.h"

#define LIST_LOOP( list, itor )	for( itor = (list)->getFirst(); itor != NULL; itor = (list)->next( itor ) )

// batch 방식과 즉시 방식이 있다. 구분 : batch 방식은 m_removeQueue가 Null이 아니다 

template< class Type >
class ITZList
{
public:
	ITZList();
	~ITZList();

public:
	void				CreateList( SI32 size = 0, BOOL bFactory = FALSE );
	void				CreateBatchList( SI32 size, BOOL bFactory = FALSE );

	void				clear();

	void				newElement( Type *pElem );

	Type*				getFirst() { return (Type *)( m_masterList->getFirst() ); }; 
	Type*				getLast() { return (Type *)( m_masterList->getLast() ); };

	Type*				getFreeFirst() { return (Type *)( m_freeList->getFirst() ); }; 
	Type*				getFreeLast() { return (Type *)( m_freeList->getLast() ); }; 

	Type*				next( Type *itor ) { return (Type *)( itor->GetNext() ); };
	Type*				prev( Type *itor ) { return (Type *)( itor->GetPrev() ); };

	void				batch();

	Type*				push();
	Type*				push_from( ITZList< Type > *pList );
	Type*				push_index( SI32 index );

	void				remove( Type *pElem );
	void				remove_into( Type *pElem, ITZList< Type > *pList );

	Type*				get( SI32 index );

public:
	ITZQList*			m_waitList;
	ITZQList*			m_masterList;
	ITZQList*			m_freeList;

	ITZQueue< Type* >*	m_removeQueue;

	Type*				m_pFactory;
	SI32				m_sizeFactory;
};

template< class Type >
ITZList< Type >::ITZList()
{
	m_waitList = NULL;
	m_masterList = NULL;
	m_freeList = NULL;
	m_removeQueue = NULL;

	m_pFactory = NULL;
}

template< class Type >
ITZList< Type >::~ITZList()
{
	clear();
}

template< class Type >
void ITZList< Type >::clear()
{
	delete m_waitList;
	delete m_masterList;
	delete m_freeList;
	delete m_removeQueue;
	delete [] m_pFactory;

	m_waitList = NULL;
	m_masterList = NULL;
	m_freeList = NULL;
	m_removeQueue = NULL;
	m_pFactory = NULL;

	m_sizeFactory = 0;
}

template< class Type >
void ITZList< Type >::newElement( Type *pElem )
{
	m_freeList->push( (ITZObject *)pElem );

	pElem->SetActive( FALSE );

}

template< class Type >
void ITZList< Type >::CreateList( SI32 size, BOOL bFactory )
{
	clear();

	m_masterList = new ITZQList;
	m_freeList = new ITZQList;

	if( bFactory == TRUE ) {

		m_sizeFactory = size;

		m_pFactory = new Type[ size ];

		for( int i = 0; i < size; ++i ) {

			m_pFactory[ i ].m_usIndex = i;

			newElement( &m_pFactory[ i ] );
		}

	}

}

template< class Type >
void ITZList< Type >::CreateBatchList( SI32 maxSize, BOOL bFactory )
{
	clear();

	m_waitList = new ITZQList;
	m_masterList = new ITZQList;
	m_freeList = new ITZQList;

	m_removeQueue = new ITZQueue< Type* >;

	if( bFactory == TRUE ) {

		m_sizeFactory = maxSize;

		m_pFactory = new Type[ maxSize ];

		for( int i = 0; i < maxSize; ++i ) {

			m_pFactory[ i ].m_usIndex = i;

			newElement( &m_pFactory[ i ] );
		}

	}

	m_removeQueue->CreateQueue( maxSize );
}


// m_waitList에 있는 모든 노드를 꺼내와서 m_masterList로 삽입한다
// m_removeQueue에 있는 노드들의 포인터를 얻어서 m_masterList에서 삭제한다
// 삭제한 노드들은 m_freeList에 넣는다
template< class Type >
void ITZList< Type >::batch()
{
	SI32		i, size;
	ITZObject	*pElem;

	size = m_waitList->size();

	for( i = 0; i < size; ++i ) {

		pElem = m_waitList->pop();

		if( pElem ) {

			pElem->SetActive( TRUE );

			m_masterList->push( pElem );

			pElem->Init();

		}
	}

	size = m_removeQueue->GetSize();

	for( i = 0; i < size; ++i ) {

		m_removeQueue->Dequeue( (Type **)&pElem );

		if( pElem && pElem->IsActive() == TRUE ) {
			
			pElem->Close();
			
			m_masterList->remove( pElem );
			m_freeList->push( pElem );

			// 비활성화 상태로 만든다
			pElem->SetActive( FALSE );

		}
	}

}

template< class Type >
Type* ITZList< Type >::push()
{

	ITZObject *pElem = m_freeList->pop();

	if( pElem == NULL ) return NULL;

	if( pElem->IsActive() == TRUE ) return NULL;

	if( m_removeQueue ) {
		// batch 방식이면,

		m_waitList->push( pElem );

	} else {
		// 활성화 상태로 만든다
		pElem->SetActive( TRUE );

		m_masterList->push( pElem );

		pElem->Init();

	}

	return (Type *)pElem;
}

template< class Type >
Type* ITZList< Type >::push_index( SI32 index )
{
	Type *pElem = get( index );

	if( pElem == NULL ) return NULL;

	if( pElem->IsActive() == TRUE ) return NULL;

	m_freeList->remove( pElem );

	if( m_removeQueue ) {
		// batch 방식이면,

		m_waitList->push( pElem );

	} else {
		
		// 활성화 상태로 만든다
		pElem->SetActive( TRUE );

		pElem->Init();

		m_masterList->push( pElem );

	}

	return pElem;
}

template< class Type >
Type* ITZList< Type >::push_from( ITZList< Type > *pList )
{
	ITZObject *pElem = pList->m_freeList->pop();

	if( pElem == NULL ) return NULL;

	if( pElem->IsActive() == TRUE ) return NULL;

	if( m_removeQueue ) {
		// batch 방식이면,

		m_waitList->push( pElem );

	} else {

		// 활성화 상태로 만든다
		pElem->SetActive( TRUE );

		pElem->Init();

		m_masterList->push( pElem );

	}

	return (Type *)pElem;
}

template< class Type >
void ITZList< Type >::remove( Type *pElem )
{
	if( pElem == NULL ) return;

	if( pElem->IsActive() == FALSE ) return;

	if( m_removeQueue ) {
		// batch 방식이면,
		m_removeQueue->Enqueue( pElem );

	} else {

		m_masterList->remove( pElem );
		m_freeList->push( pElem );

		pElem->Close();

		// 비활성화 상태로 만든다
		pElem->SetActive( FALSE );

	}
}


template< class Type >
void ITZList< Type >::remove_into( Type *pElem, ITZList< Type >* list )
{
	if( pElem == NULL ) return;
	
	if( pElem->IsActive() == FALSE ) return;

	if( m_removeQueue == NULL ) {

		m_masterList->remove( pElem );
		list->m_freeList->push( pElem );

		pElem->Close();

		// 비활성화 상태로 만든다
		pElem->SetActive( FALSE );

	}
}

template< class Type >
Type* ITZList< Type >::get( SI32 index )
{
	if( index < 0 || index >= m_sizeFactory ) return NULL;
	return &m_pFactory[ index ];
}

#endif
