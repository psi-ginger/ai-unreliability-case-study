//-----------------------------------------------------------------------------
// 2005/09/13 이제완
//-----------------------------------------------------------------------------

#ifndef _NHASHTABLE_H
#define _NHASHTABLE_H

#include "NList.h"

//-----------------------------------------------------------------------------
// NHashTableString
//
//
//       NArray( 엔트리 리스트의 배열 )
// 
//         0(엔트리 리스트)
//         1       -  A  -  B  -  C(엔트리)
//         2       -  D
//         3       -  E  -  F
//         4
//         .
//         .
//   (arraySize - 1)
//
//
//       NListPool  
//         list    -  G  -  H  -  I  - ...  (엔트리를 담고 있는 리스트 풀)
//
//-----------------------------------------------------------------------------


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

	void			CreateHashTable( int arraySize, int listPoolSize, unsigned int (*pHashFunc)(KeyType) );

	bool			Insert( KeyType key, DataType data );
	bool			Remove( KeyType key );

	bool			Find( KeyType key, DataType *pData );
	DataType*		Find( KeyType key );

	inline int		GetHashTableIndex( KeyType key ) { return m_pHashFunc( key ) % m_iArraySize; }

private:

	NListPool< Entry >			m_listpool;
	NArray< NList< Entry > >	m_array;

	unsigned int				(*m_pHashFunc)(KeyType);

	int							m_iArraySize;
};

template< class KeyType, class DataType >
NHashTable< KeyType, DataType >::NHashTable()
{
	m_pHashFunc = NULL;
	m_iArraySize = 0;
}

template< class KeyType, class DataType >
NHashTable< KeyType, DataType >::~NHashTable()
{

}

template< class KeyType, class DataType >
void NHashTable< KeyType, DataType >::CreateHashTable( int arraySize, int listPoolSize, unsigned int (*pHashFunc)(KeyType) )
{
	
	m_listpool.CreateListPool( listPoolSize );
	m_array.CreateArray( arraySize );

	EntryList *pList;

	for( int i = 0; i < arraySize; ++i ) 
	{
		pList = m_array.GetElementPtr( i );

		pList->CreateList( &m_listpool );
	}

	m_iArraySize = arraySize;
	m_pHashFunc = pHashFunc;
}


template< class KeyType, class DataType >
bool NHashTable< KeyType, DataType >::Insert( KeyType key, DataType data )
{

	int index = GetHashTableIndex( key );

	EntryList* pList = m_array.GetElementPtr( index );

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

	EntryList* pList = m_array.GetElementPtr( index );

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

	EntryList* pList = m_array.GetElementPtr( index );

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

	EntryList* pList = m_array.GetElementPtr( index );

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


#endif
