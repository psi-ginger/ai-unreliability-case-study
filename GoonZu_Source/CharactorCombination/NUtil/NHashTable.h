//-----------------------------------------------------------------------------
// 2005 이제완
//-----------------------------------------------------------------------------

#ifndef _NHASHTABLE_H
#define _NHASHTABLE_H

#include "NList.h"

template< class KeyType, class DataType >
class NHashEntry
{
public:
	KeyType		m_key;
	DataType	m_data;
};

template< class KeyType, class DataType >
class NHashTable
{
public:
	typedef NHashEntry< KeyType, DataType >	Entry;
	typedef NList< Entry >					EntryList;

	NHashTable();
	~NHashTable();

	void		CreateHashTable( int ArraySize, int ListAllocatorSize, unsigned int (*pHashFunc)(KeyType) );

	bool		Insert( KeyType key, DataType data );
	bool		Remove( KeyType key );

	bool		Find( KeyType key, DataType *pData );
	DataType*	Find( KeyType key );

	inline int	GetHashTableIndex( KeyType key ) { return m_pHashFunc( key ) % m_siArraySize; }

private:

	NListAllocator< Entry >		m_listallocator;
	NArray< NList< Entry > >	m_array;

	unsigned int				(*m_pHashFunc)(KeyType);

	int							m_siArraySize;
};

template< class KeyType, class DataType >
NHashTable< KeyType, DataType >::NHashTable()
{
	m_pHashFunc = NULL;
	m_siArraySize = 0;
}

template< class KeyType, class DataType >
NHashTable< KeyType, DataType >::~NHashTable()
{

}

template< class KeyType, class DataType >
void NHashTable< KeyType, DataType >::CreateHashTable( int ArraySize, int ListAllocatorSize, unsigned int (*pHashFunc)(KeyType) )
{
	
	m_listallocator.CreateListAllocator( ListAllocatorSize );
	m_array.CreateArray( ArraySize );

	EntryList *pList;

	for( int i = 0; i < ArraySize; ++i ) 
	{
		pList = m_array.GetDataPtr( i );

		pList->CreateList( &m_listallocator );
	}

	m_siArraySize = ArraySize;
	m_pHashFunc = pHashFunc;
}


template< class KeyType, class DataType >
bool NHashTable< KeyType, DataType >::Insert( KeyType key, DataType data )
{

	int index = GetHashTableIndex( key );

	EntryList* pList = m_array.GetDataPtr( index );

	NListNode< Entry > *pNode;
	Entry *pEntry;

	// 동일한 Key가 이미 있는지 체크
	pNode = pList->GetFirst();

	while( pNode )
	{
		if( pNode->GetDataPtr()->m_key == key ) 
		{
			pNode->GetDataPtr()->m_data = data;
			return true;
		}

		pNode = pNode->GetNext();
	}
			
	pNode = pList->PushBack();

	if( pNode == NULL ) return false;

	pEntry = pNode->GetDataPtr();

	pEntry->m_key = key;
	pEntry->m_data = data;

	return true;
}


template< class KeyType, class DataType >
bool NHashTable< KeyType, DataType >::Remove( KeyType key )
{
	int index = GetHashTableIndex( key );

	EntryList* pList = m_array.GetDataPtr( index );

	NListNode< Entry > *pNode;

	pNode = pList->GetFirst();

	while( pNode )
	{
		if( pNode->GetDataPtr()->m_key == key ) {

			pList->Remove( pNode );
			return true;
		}

		pNode = pNode->GetNext();
	}

	return false;
}

template< class KeyType, class DataType >
bool NHashTable< KeyType, DataType >::Find( KeyType key, DataType *pData )
{
	int index = GetHashTableIndex( key );

	EntryList* pList = m_array.GetDataPtr( index );

	NListNode< Entry > *pNode;

	pNode = pList->GetFirst();

	while( pNode )
	{
		if( pNode->GetDataPtr()->m_key == key ) {

			*pData = pNode->GetDataPtr()->m_data;

			return true;
		}

		pNode = pNode->GetNext();
	}

	return false;
}

template< class KeyType, class DataType >
DataType* NHashTable< KeyType, DataType >::Find( KeyType key )
{
	int index = GetHashTableIndex( key );

	EntryList* pList = m_array.GetDataPtr( index );

	NListNode< Entry > *pNode;

	pNode = pList->GetFirst();

	while( pNode )
	{
		if( pNode->GetDataPtr()->m_key == key ) {

			return &pNode->GetDataPtr()->m_data; 
		}

		pNode = pNode->GetNext();
	}

	return NULL;
}



//-----------------------------------------------------------------------------
// NHashTableString
//-----------------------------------------------------------------------------
template< class DataType >
class NHashTableString
{
public:
	typedef NHashEntry< char*, DataType >	Entry;
	typedef NList< Entry >					EntryList;

	NHashTableString();
	~NHashTableString();

	void			CreateHashTableString( int ArraySize, int ListAllocatorSize, int MaxStrUnitSize );

	bool			Insert( char *key, DataType data );
	bool			Remove( char *key );

	bool			Find( char *key, DataType *pData );
	DataType*		Find( char *key );

	inline int		GetHashTableIndex( char *key ) { return HashFunc( key ) % m_siArraySize; }

public:
	unsigned int	HashFunc( char *key );

	NListAllocator< Entry >			m_listallocator;
	NArray< NList< Entry > >	m_array;
	
	NStringArray				m_stringArray;

	int							m_siArraySize;

};

template< class DataType >
NHashTableString< DataType >::NHashTableString()
{
	m_siArraySize = 0;
}

template< class DataType >
NHashTableString< DataType >::~NHashTableString()
{
}

template< class DataType >
void NHashTableString< DataType >::CreateHashTableString( int ArraySize, int ListAllocatorSize, int MaxStrUnitSize )
{

	m_listallocator.CreateListAllocator( ListAllocatorSize );
	m_array.CreateArray( ArraySize );

	m_stringArray.CreateStringArray( ListAllocatorSize, MaxStrUnitSize );

	// ListAllocator의 Entry의 char*와 stringArray의 포인터를 연결한다 
	int i;
	Entry *pEntry;

	for( i = 0; i < ListAllocatorSize; ++i )
	{
		pEntry = m_listallocator.GetDataPtr( i );

		pEntry->m_key = m_stringArray.GetData( i );
	}

	// 각 entry의 list 생성 
	EntryList *pList;

	for( i = 0; i < ArraySize; ++i ) 
	{
		pList = m_array.GetDataPtr( i );

		pList->CreateList( &m_listallocator );
	}

	m_siArraySize = ArraySize;
}

template< class DataType >
unsigned int NHashTableString< DataType >::HashFunc( char *key )
{

	unsigned int hash, i;
	
	hash = 0;
	i = 1;

	while( *key )
	{
		hash += ( i * (unsigned char)*key );
		++i;
		++key;
	}

	return hash;
}

template< class DataType >
bool NHashTableString< DataType >::Insert( char *key, DataType data )
{

	int index = GetHashTableIndex( key );

	EntryList* pList = m_array.GetDataPtr( index );

	NListNode< Entry > *pNode;
	Entry *pEntry;

	// 동일한 Key가 이미 있는지 체크
	pNode = pList->GetFirst();

	while( pNode )
	{
		if( strcmp( pNode->GetDataPtr()->m_key, key ) == 0 ) 
		{
			pNode->GetDataPtr()->m_data = data;
			return true;
		}

		pNode = pNode->GetNext();
	}
			
	pNode = pList->PushBack();

	if( pNode == NULL ) return false;

	pEntry = pNode->GetDataPtr();

	strcpy( pEntry->m_key, key );
	pEntry->m_data = data;

	return true;
}


template< class DataType >
bool NHashTableString< DataType >::Remove( char *key )
{

	int index = GetHashTableIndex( key );

	EntryList* pList = m_array.GetDataPtr( index );

	NListNode< Entry > *pNode;

	pNode = pList->GetFirst();

	while( pNode )
	{
		if( strcmp( pNode->GetDataPtr()->m_key, key ) == 0 ) {

			pList->Remove( pNode );
			return true;
		}

		pNode = pNode->GetNext();
	}

	return false;
}

template< class DataType >
bool NHashTableString< DataType >::Find( char *key, DataType *pDdata )
{
	int index = GetHashTableIndex( key );

	EntryList* pList = m_array.GetDataPtr( index );

	NListNode< Entry > *pNode;

	pNode = pList->GetFirst();

	while( pNode )
	{
		if( strcmp( pNode->GetDataPtr()->m_key, key ) == 0 ) {

			*pData = pNode->GetDataPtr()->m_data; 

			return true;
		}

		pNode = pNode->GetNext();
	}

	return false;
}

template< class DataType >
DataType* NHashTableString< DataType >::Find( char *key )
{
	int index = GetHashTableIndex( key );

	EntryList* pList = m_array.GetDataPtr( index );

	NListNode< Entry > *pNode;

	pNode = pList->GetFirst();

	while( pNode )
	{
		if( strcmp( pNode->GetDataPtr()->m_key, key ) == 0 ) {

			return &pNode->GetDataPtr()->m_data; 
		}

		pNode = pNode->GetNext();
	}

	return NULL;
}


#endif
