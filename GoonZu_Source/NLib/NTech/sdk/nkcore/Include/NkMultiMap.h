//-----------------------------------------------------------------------------
// 2008/08/20 
// 엔도어즈 기술개발팀 
// 이제완
//-----------------------------------------------------------------------------

#ifndef _NKMULTIMAP_H
#define _NKMULTIMAP_H

#include "NkMemory.h"

template <class TKey, class TData>
class NkMultiMapNode : public NkMemory
{
public:
	NkMultiMapNode* m_pkNext;
	TKey m_key;
	TData m_data;
};

template <class TKey>
class NkMultiMapHashFunctor
{
public:
	static unsigned int KeyToHashIndex( TKey key, int iHashSize );
};

template <class TKey, class TData>
class NkMultiMapEqualsFunctor
{
public:
	static bool IsKeysEqual( TKey key1, TKey key2 );
	static bool IsDatasEqual( TKey key1, TData* pData1, TKey key2, TData* pData2 );
};


template <class TKey, class TData, class THash = NkMultiMapHashFunctor, class TEquals = NkMultiMapEqualsFunctor>
class NkMultiMap : public NkMemory
{
public:
	NkMultiMap( unsigned int uiHashSize = 37 );
	virtual ~NkMultiMap();

	void RemoveAll();

	//-------------------------------------------------------------------------
	// Data기반 접근 함수(Data 중복 불가)
	bool AddAt( TKey key, TData data );
	bool AddAt( TKey key, TData* pData );

	void SetAt( TKey key, TData data );
	void SetAt( TKey key, TData* pData );

	bool GetAt( TKey key, TData* pInData, TData* pOutData );
	TData* GetAt( TKey key, TData* pInData );

	bool RemoveAt( TKey key, TData* pData );
	//-------------------------------------------------------------------------
	
	//-------------------------------------------------------------------------
	// Key기반 접근 함수(Data 중복 허용)
	void AddData( TKey key, TData data );
	void AddData( TKey key, TData* pData );

	bool GetFirstData( TKey key, TData* pOutData );
	TData* GetFirstData( TKey key );

	bool GetNextData( TKey key, TData* pOutData );
	TData* GetNextData( TKey key );

	bool RemoveKeys( TKey key );
	//-------------------------------------------------------------------------

protected:
	virtual NkMultiMapNode<TKey,TData>* NewNode();
	virtual void DeleteNode( NkMultiMapNode<TKey,TData>* pkNode );

	virtual void SetData( NkMultiMapNode<TKey,TData>* pkNode, TKey key, TData& data );
	virtual void ClearData( NkMultiMapNode<TKey,TData>* pkNode );

	unsigned int m_uiCount;
	unsigned int m_uiHashSize;

	unsigned int m_uiMaxDepth;

	NkMultiMapNode<TKey,TData>*	 m_pkCurrentNode;
	NkMultiMapNode<TKey,TData>** m_ppkHashTable;
};

// 생성자
template <class TKey, class TData, class THash, class TEquals> inline
NkMultiMap<TKey,TData,THash,TEquals>::NkMultiMap( unsigned int uiHashSize )
{
	m_uiCount = 0;
	m_uiHashSize = uiHashSize;
	m_uiMaxDepth = 0;

	m_pkCurrentNode = 0;

	unsigned int uiSize = sizeof( NkMultiMapNode<TKey,TData>* ) * uiHashSize;
	m_ppkHashTable = (NkMultiMapNode<TKey,TData>**)malloc(uiSize);
	memset( m_ppkHashTable, 0, uiSize );
}

// 소멸자
template <class TKey, class TData, class THash, class TEquals> inline
NkMultiMap<TKey,TData,THash,TEquals>::~NkMultiMap()
{
	RemoveAll();
	free( m_ppkHashTable );
}

// RemoveAll
template <class TKey, class TData, class THash, class TEquals> inline
void NkMultiMap<TKey,TData,THash,TEquals>::RemoveAll()
{
	NkMultiMapNode<TKey,TData>* pkNode;

	for( unsigned int i = 0; i < m_uiHashSize; ++i )
	{
		pkNode = m_ppkHashTable[ i ];

		while( pkNode )
		{
			m_ppkHashTable[ i ] = pkNode->m_pkNext;
			ClearData( pkNode );
			DeleteNode( pkNode );

			pkNode = m_ppkHashTable[ i ];
		}
	}

	m_uiCount = 0;
}

// NewNode
template <class TKey, class TData, class THash, class TEquals> inline
NkMultiMapNode<TKey,TData>* NkMultiMap<TKey,TData,THash,TEquals>::NewNode()
{
	return NkNew NkMultiMapNode<TKey,TData>;
}

// DeleteNode
template <class TKey, class TData, class THash, class TEquals> inline
void NkMultiMap<TKey,TData,THash,TEquals>::DeleteNode( NkMultiMapNode<TKey,TData>* pkNode )
{
	NkDelete pkNode;
}

// SetData
template <class TKey, class TData, class THash, class TEquals> inline
void NkMultiMap<TKey,TData,THash,TEquals>::SetData( NkMultiMapNode<TKey,TData>* pkNode, TKey key, TData& data )
{
	pkNode->m_key = key;
	pkNode->m_data = data;
}

// ClearData
template <class TKey, class TData, class THash, class TEquals> inline
void NkMultiMap<TKey,TData,THash,TEquals>::ClearData( NkMultiMapNode<TKey,TData>* /*pkNode*/ )
{
}

// AddAt
template <class TKey, class TData, class THash, class TEquals> inline
bool NkMultiMap<TKey,TData,THash,TEquals>::AddAt( TKey key, TData data )
{
	unsigned int uiIndex = THash::KeyToHashIndex( key, m_uiHashSize );
	NkMultiMapNode<TKey,TData>* pkNode = m_ppkHashTable[ uiIndex ];

	unsigned int uiDepth = 0;

	while( pkNode )
	{
		++uiDepth;
		if( TEquals::IsDatasEqual( key, &data, pkNode->m_key, &pkNode->m_data ) )
		{
			return false;
		}

		pkNode = pkNode->m_pkNext;
	}

	pkNode = NewNode();
	SetData( pkNode, key, data );

	pkNode->m_pkNext = m_ppkHashTable[ uiIndex ];
	m_ppkHashTable[ uiIndex ] = pkNode;

	++m_uiCount;

	if( m_uiMaxDepth < uiDepth ) m_uiMaxDepth = uiDepth;

	return true;
}

// AddAt
template <class TKey, class TData, class THash, class TEquals> inline
bool NkMultiMap<TKey,TData,THash,TEquals>::AddAt( TKey key, TData* pData )
{
	unsigned int uiIndex = THash::KeyToHashIndex( key, m_uiHashSize );
	NkMultiMapNode<TKey,TData>* pkNode = m_ppkHashTable[ uiIndex ];

	unsigned int uiDepth = 0;

	while( pkNode )
	{
		++uiDepth;
		if( TEquals::IsDatasEqual( key, pData, pkNode->m_key, &pkNode->m_data ) )
		{
			return false;
		}

		pkNode = pkNode->m_pkNext;
	}

	pkNode = NewNode();
	SetData( pkNode, key, *pData );

	pkNode->m_pkNext = m_ppkHashTable[ uiIndex ];
	m_ppkHashTable[ uiIndex ] = pkNode;

	++m_uiCount;

	if( m_uiMaxDepth < uiDepth ) m_uiMaxDepth = uiDepth;

	return true;
}

// SetAt
template <class TKey, class TData, class THash, class TEquals> inline
void NkMultiMap<TKey,TData,THash,TEquals>::SetAt( TKey key, TData data )
{
	unsigned int uiIndex = THash::KeyToHashIndex( key, m_uiHashSize );
	NkMultiMapNode<TKey,TData>* pkNode = m_ppkHashTable[ uiIndex ];

	while( pkNode )
	{
		if( TEquals::IsDatasEqual( key, &data, pkNode->m_key &pkNode->m_data ) )
		{
			pkNode->m_data = data;
			return;
		}

		pkNode = pkNode->m_pkNext;
	}

	pkNode = NewNode();
	SetData( pkNode, key, data );

	pkNode->m_pkNext = m_ppkHashTable[ uiIndex ];
	m_ppkHashTable[ uiIndex ] = pkNode;
}

// SetAt
template <class TKey, class TData, class THash, class TEquals> inline
void NkMultiMap<TKey,TData,THash,TEquals>::SetAt( TKey key, TData* pData )
{
	unsigned int uiIndex = THash::KeyToHashIndex( key, m_uiHashSize );
	NkMultiMapNode<TKey,TData>* pkNode = m_ppkHashTable[ uiIndex ];

	while( pkNode )
	{
		if( TEquals::IsDatasEqual( key, pData, pkNode->m_key, &pkNode->m_data ) )
		{
			pkNode->m_data = *pData;
			return;
		}

		pkNode = pkNode->m_pkNext;
	}

	pkNode = NewNode();
	SetData( pkNode, key, *pData );

	pkNode->m_pkNext = m_ppkHashTable[ uiIndex ];
	m_ppkHashTable[ uiIndex ] = pkNode;2

	++m_uiCount;
}

// GetAt
template <class TKey, class TData, class THash, class TEquals> inline
bool NkMultiMap<TKey,TData,THash,TEquals>::GetAt( TKey key, TData* pInData, TData* pOutData )
{
	unsigned int uiIndex = THash::KeyToHashIndex( key, m_uiHashSize );
	NkMultiMapNode<TKey,TData>* pkNode = m_ppkHashTable[ uiIndex ];

	while( pkNode )
	{
		if( TEquals::IsDatasEqual( key, pInData, pkNode->m_key, &pkNode->m_data ) )
		{
			*pOutData = pkNode->m_data;
			return true;
		}

		pkNode = pkNode->m_pkNext;
	}

	return false;
}

// GetAt
template <class TKey, class TData, class THash, class TEquals> inline
TData* NkMultiMap<TKey,TData,THash,TEquals>::GetAt( TKey key, TData* pInData )
{
	unsigned int uiIndex = THash::KeyToHashIndex( key, m_uiHashSize );
	NkMultiMapNode<TKey,TData>* pkNode = m_ppkHashTable[ uiIndex ];

	while( pkNode )
	{
		if( TEquals::IsDatasEqual( key, pInData, pkNode->m_key, &pkNode->m_data ) )
		{
			return &pkNode->m_data;
		}

		pkNode = pkNode->m_pkNext;
	}

	return 0;
}

// RemoveAt
template <class TKey, class TData, class THash, class TEquals> inline
bool NkMultiMap<TKey,TData,THash,TEquals>::RemoveAt( TKey key, TData* pData )
{
	unsigned int uiIndex = THash::KeyToHashIndex( key, m_uiHashSize );
	NkMultiMapNode<TKey,TData>* pkNode = m_ppkHashTable[ uiIndex ];

	if( pkNode )
	{
		if( TEquals::IsDatasEqual( key, pData, pkNode->m_key, &pkNode->m_data ) )
		{
			m_ppkHashTable[ uiIndex ] = pkNode->m_pkNext;

			ClearData( pkNode );
			DeleteNode( pkNode );

			--m_uiCount;
			return true;
		}
		else
		{
			NkMultiMapNode<TKey,TData>* pkPrev = pkNode;
			pkNode = pkNode->m_pkNext;

			while( pkNode )
			{
				if( TEquals::IsDatasEqual( key, pData, pkNode->m_key, &pkNode->m_data ) )
				{
					pkPrev->m_pkNext = pkNode->m_pkNext;

					ClearData( pkNode );
					DeleteNode( pkNode );

					--m_uiCount;
					return true;
				}

				pkPrev = pkNode;
				pkNode = pkNode->m_pkNext;
			}
		}
	}

	return false;
}

// AddData
template <class TKey, class TData, class THash, class TEquals> inline
void NkMultiMap<TKey,TData,THash,TEquals>::AddData( TKey key, TData data )
{
	unsigned int uiIndex = THash::KeyToHashIndex( key, m_uiHashSize );
	NkMultiMapNode<TKey,TData>* pkNode = m_ppkHashTable[ uiIndex ];

	pkNode = NewNode();
	SetData( pkNode, key, data );

	pkNode->m_pkNext = m_ppkHashTable[ uiIndex ];
	m_ppkHashTable[ uiIndex ] = pkNode;

	++m_uiCount;
}

// AddData
template <class TKey, class TData, class THash, class TEquals> inline
void NkMultiMap<TKey,TData,THash,TEquals>::AddData( TKey key, TData* pData )
{
	unsigned int uiIndex = THash::KeyToHashIndex( key, m_uiHashSize );
	NkMultiMapNode<TKey,TData>* pkNode = m_ppkHashTable[ uiIndex ];

	pkNode = NewNode();
	SetData( pkNode, key, *pData );

	pkNode->m_pkNext = m_ppkHashTable[ uiIndex ];
	m_ppkHashTable[ uiIndex ] = pkNode;

	++m_uiCount;
}

// GetFirstData
template <class TKey, class TData, class THash, class TEquals> inline
bool NkMultiMap<TKey,TData,THash,TEquals>::GetFirstData( TKey key, TData* pOutData )
{
	unsigned int uiIndex = THash::KeyToHashIndex( key, m_uiHashSize );
	NkMultiMapNode<TKey,TData>* pkNode = m_ppkHashTable[ uiIndex ];

	while( pkNode )
	{
		if( TEquals::IsKeysEqual( key, pkNode->m_key ) )
		{	
			m_pkCurrentNode = pkNode;
			*pOutData = pkNode->m_data;			
			return true;
		}

		pkNode = pkNode->m_pkNext;
	}

	return false;
}

// GetFirstData
template <class TKey, class TData, class THash, class TEquals> inline
TData* NkMultiMap<TKey,TData,THash,TEquals>::GetFirstData( TKey key )
{
	unsigned int uiIndex = THash::KeyToHashIndex( key, m_uiHashSize );
	NkMultiMapNode<TKey,TData>* pkNode = m_ppkHashTable[ uiIndex ];

	while( pkNode )
	{
		if( TEquals::IsKeysEqual( key, pkNode->m_key ) )
		{			
			m_pkCurrentNode = pkNode;
			return &pkNode->m_data;
		}

		pkNode = pkNode->m_pkNext;
	}

	return 0;
}

// GetNextData
template <class TKey, class TData, class THash, class TEquals> inline
bool NkMultiMap<TKey,TData,THash,TEquals>::GetNextData( TKey key, TData* pOutData )
{
	if( !m_pkCurrentNode ) return false;

	NkMultiMapNode<TKey,TData>* pkNode = m_pkCurrentNode->m_pkNext;

	while( pkNode )
	{
		if( TEquals::IsKeysEqual( key, pkNode->m_key ) )
		{
			m_pkCurrentNode = pkNode;
			*pOutData = pkNode->m_data;			
			return true;
		}

		pkNode = pkNode->m_pkNext;
	}

	m_pkCurrentNode = 0;
	return false;
}

// GetNextData
template <class TKey, class TData, class THash, class TEquals> inline
TData* NkMultiMap<TKey,TData,THash,TEquals>::GetNextData( TKey key )
{
	if( !m_pkCurrentNode ) return false;

	NkMultiMapNode<TKey,TData>* pkNode = m_pkCurrentNode->m_pkNext;

	while( pkNode )
	{
		if( TEquals::IsKeysEqual( key, pkNode->m_key ) )
		{
			m_pkCurrentNode = pkNode;
			return &pkNode->m_data;
		}

		pkNode = pkNode->m_pkNext;
	}

	m_pkCurrentNode = 0;
	return 0;
}


// RemoveKeys
template <class TKey, class TData, class THash, class TEquals> inline
bool NkMultiMap<TKey,TData,THash,TEquals>::RemoveKeys( TKey key )
{
	bool bRet = false;

	unsigned int uiIndex = THash::KeyToHashIndex( key, m_iHashSize );
	NkMultiMapNode<TKey,TData>* pkNode = m_ppkHashTable[ uiIndex ];
	NkMultiMapNode<TKey,TData>* pkPrev = 0;
	NkMultiMapNode<TKey,TData>* pkDelete = 0;

	while( pkNode )
	{
		if( TEquals::IsKeysEqual( key, pkNode->m_key ) )
		{
			if( pkPrev )
			{
				pkPrev->m_pkNext = pkNode->m_pkNext;
			}
			else
			{
				m_ppkHashTable[ uiIndex ] = pkNode->m_pkNext;
			}

			pkDelete = pkNode;
			pkNode = pkNode->m_pkNext;

			ClearData( pkDelete );
			DeleteNode( pkDelete );

			--m_uiCount;

			bRet = true;

			continue;
		}

		pkPrev = pkNode;
		pkNode = pkNode->m_pkNext;
	}

	return bRet;
}


#endif