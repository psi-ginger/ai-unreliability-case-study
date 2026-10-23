//-------------------------------------------------------------------------------------------------
// NkHashMap.inl
// 엔도어즈 기술개발팀
// 이제완
//-------------------------------------------------------------------------------------------------

template <class TKey, class TData>
inline void
NkHashMap_Iterator<TKey,TData>::_Next()
{
	if( m_pkNode == 0 ) return;

	_Node* pkNode = m_pkNode;

	if( pkNode->m_pkNext )
	{
		m_pkNode = pkNode->m_pkNext;
		return;
	}

	unsigned int uiHashSize = m_pkHashTable->GetCount();
	for( unsigned int i = m_uiHashIndex + 1; i < uiHashSize; ++i )
	{
		pkNode = (*m_pkHashTable)[i];

		while( pkNode )
		{
			m_pkNode = pkNode;
			m_uiHashIndex = i;

			return;
		}
	}

	m_pkNode = 0;
}

template <class TKey, class TData, class THash, class TEquals, class TDataSetter> inline
NkHashMap<TKey,TData,THash,TEquals,TDataSetter>::NkHashMap(unsigned int uiHashSize)
{
	m_uiHashSize = uiHashSize;
	m_uiCount = 0;
	
	m_pkHashTable = NkNew NkVector<NodePtr>(uiHashSize);
}

template <class TKey, class TData, class THash, class TEquals, class TDataSetter> inline
NkHashMap<TKey,TData,THash,TEquals,TDataSetter>::~NkHashMap()
{
	RemoveAll();
	NkDelete m_pkHashTable;
}

//-------------------------------------------------------------------------------------------------
// 크기 관련 함수
//-------------------------------------------------------------------------------------------------
template <class TKey, class TData, class THash, class TEquals, class TDataSetter>
inline unsigned int
NkHashMap<TKey,TData,THash,TEquals,TDataSetter>::GetHashSize() const
{
	return m_uiHashSize;
}

template <class TKey, class TData, class THash, class TEquals, class TDataSetter>
inline unsigned int 
NkHashMap<TKey,TData,THash,TEquals,TDataSetter>::GetCount() const
{
	return m_uiCount;
}

template <class TKey, class TData, class THash, class TEquals, class TDataSetter>
inline unsigned int
NkHashMap<TKey,TData,THash,TEquals,TDataSetter>::GetAllocedBytes() const
{
	return (sizeof(NodePtr) * m_uiHashSize) + (sizeof(*NodePtr) * m_uiCount);
}

template <class TKey, class TData, class THash, class TEquals, class TDataSetter>
inline unsigned int
NkHashMap<TKey,TData,THash,TEquals,TDataSetter>::GetNodeCount(unsigned int uiHashIndex) const
{
	NkAssert(uiHashIndex < m_uiHashSize);

	NodePtr pkNode = (*m_pkHashTable)[uiHashIndex];
	unsigned int uiCount = 0;

	while( pkNode )
	{
		++uiCount;
		pkNode = pkNode->m_pkNext;
	}

	return uiCount;
}

//-------------------------------------------------------------------------------------------------
// 순차 탐색을 위한 함수
//-------------------------------------------------------------------------------------------------
template <class TKey, class TData, class THash, class TEquals, class TDataSetter>
inline bool
NkHashMap<TKey,TData,THash,TEquals,TDataSetter>::GetBegin( Iterator& kIter) const
{
	if( m_uiCount == 0 )
	{
		kIter.m_pkNode = 0;
		return false;
	}

	NodePtr pkNode;

	for( unsigned int i = 0; i < m_uiHashSize; ++i )
	{
		pkNode = (*m_pkHashTable)[i];

		while( pkNode )
		{
			kIter.m_pkNode = pkNode;
			kIter.m_pkHashTable = m_pkHashTable;
			kIter.m_uiHashIndex = i;

			return true;
		}
	}

	return false;
}

template <class TKey, class TData, class THash, class TEquals, class TDataSetter>
inline bool
NkHashMap<TKey,TData,THash,TEquals,TDataSetter>::GetNext( Iterator& kIter) const
{
	if( !kIter.IsValid() )
		return false;

	NodePtr pkNode = kIter.m_pkNode;

	if( pkNode->m_pkNext )
	{
		kIter.m_pkNode = pkNode->m_pkNext;
		return true;
	}

	for( unsigned int i = kIter.m_uiHashIndex + 1; i < m_uiHashSize; ++i )
	{
		pkNode = (*m_pkHashTable)[i];

		while( pkNode )
		{
			kIter.m_pkNode = pkNode;
			kIter.m_pkHashTable = m_pkHashTable;
			kIter.m_uiHashIndex = i;

			return true;
		}
	}

	kIter.m_pkNode = 0;
	return false;
}

//-------------------------------------------------------------------------------------------------
// 데이터 입력 함수
//-------------------------------------------------------------------------------------------------
template <class TKey, class TData, class THash, class TEquals, class TDataSetter>
inline void
NkHashMap<TKey,TData,THash,TEquals,TDataSetter>::SetAt(const TKey& key, const TData& data)
{
	unsigned int uiIndex = THash::KeyToHashIndex(key, m_uiHashSize);
	NodePtr pkNode = (*m_pkHashTable)[uiIndex];

	while( pkNode )
	{
		if( TEquals::Equals(key, pkNode->m_key) )
		{
			pkNode->m_data = data;
			return;
		}

		pkNode = pkNode->m_pkNext;
	}

	pkNode = _NewNode();

	TDataSetter::SetData(pkNode->m_key, pkNode->m_data, key, data);

	pkNode->m_pkNext = (*m_pkHashTable)[uiIndex];
	(*m_pkHashTable)[uiIndex] = pkNode;

	++m_uiCount;
}

template <class TKey, class TData, class THash, class TEquals, class TDataSetter>
inline void
NkHashMap<TKey,TData,THash,TEquals,TDataSetter>::SetAt(const TKey& key, const TData* pData)
{
	unsigned int uiIndex = THash::KeyToHashIndex(key, m_uiHashSize);
	NodePtr pkNode = (*m_pkHashTable)[uiIndex];

	while( pkNode )
	{
		if( TEquals::Equals(key, pkNode->m_key) )
		{
			pkNode->m_data = *pData;
			return;
		}

		pkNode = pkNode->m_pkNext;
	}

	pkNode = _NewNode();

	TDataSetter::SetData(pkNode->m_key, pkNode->m_data, key, *pData);

	pkNode->m_pkNext = (*m_pkHashTable)[uiIndex];
	(*m_pkHashTable)[uiIndex] = pkNode;

	++m_uiCount;
}

template <class TKey, class TData, class THash, class TEquals, class TDataSetter>
inline void
NkHashMap<TKey,TData,THash,TEquals,TDataSetter>::SetData(const Iterator& kIter, const TData& data)
{
	NkAssert(kIter.IsValid());

	NodePtr pkNode = kIter.m_pkNode;

	TDataSetter::SetData(pkNode->m_key, pkNode->m_data, pkNode->m_key, data);
}

template <class TKey, class TData, class THash, class TEquals, class TDataSetter>
inline void
NkHashMap<TKey,TData,THash,TEquals,TDataSetter>::SetData(const Iterator& kIter, const TData* pData)
{
	NkAssert(kIter.IsValid());

	NodePtr pkNode = kIter.m_pkNode;

	TDataSetter::SetData(pkNode->m_key, pkNode->m_data, pkNode->m_key, *pData);
}

template <class TKey, class TData, class THash, class TEquals, class TDataSetter>
inline void
NkHashMap<TKey,TData,THash,TEquals,TDataSetter>::Insert( Iterator& kIter, const TKey& key, const TData& data)
{
	NkAssert(!kIter.IsValid());

	NodePtr pkNode = _NewNode();

	TDataSetter::SetData(pkNode->m_key, pkNode->m_data, key, data);

	unsigned int uiIndex = kIter.GetHashIndex();

	pkNode->m_pkNext = (*m_pkHashTable)[uiIndex];
	(*m_pkHashTable)[uiIndex] = pkNode;

	++m_uiCount;

	kIter.m_pkNode = pkNode;
}
 
template <class TKey, class TData, class THash, class TEquals, class TDataSetter>
inline void
NkHashMap<TKey,TData,THash,TEquals,TDataSetter>::Insert( Iterator& kIter, const TKey& key, const TData* pData)
{
	NkAssert(!kIter.IsValid());

	NodePtr pkNode = _NewNode();

	TDataSetter::SetData(pkNode->m_key, pkNode->m_data, key, *pData);

	unsigned int uiIndex = kIter.GetHashIndex();

	pkNode->m_pkNext = (*m_pkHashTable)[uiIndex];
	(*m_pkHashTable)[uiIndex] = pkNode;

	++m_uiCount;

	kIter.m_pkNode = pkNode;
}

//-------------------------------------------------------------------------------------------------
// 삭제 함수
//-------------------------------------------------------------------------------------------------
template <class TKey, class TData, class THash, class TEquals, class TDataSetter>
inline bool
NkHashMap<TKey,TData,THash,TEquals,TDataSetter>::Remove(const TKey& key)
{
	unsigned int uiIndex = THash::KeyToHashIndex(key, m_uiHashSize);
	NodePtr pkNode = (*m_pkHashTable)[uiIndex];

	if( pkNode )
	{
		if( TEquals::Equals(key, pkNode->m_key) )
		{
			(*m_pkHashTable)[uiIndex] = pkNode->m_pkNext;

			_DeleteNode(pkNode);
			--m_uiCount;

			return true;
		}
		else
		{
			NodePtr pkPrev = pkNode;
			pkNode = pkNode->m_pkNext;

			while( pkNode )
			{
				if( TEquals::Equals(key, pkNode->m_key) )
				{
					pkPrev->m_pkNext = pkNode->m_pkNext;

					_DeleteNode(pkNode);
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

template <class TKey, class TData, class THash, class TEquals, class TDataSetter>
inline bool
NkHashMap<TKey,TData,THash,TEquals,TDataSetter>::Remove( Iterator& kIter)
{
	NkAssert(kIter.IsValid());

	unsigned int uiIndex = kIter.GetHashIndex();
	NodePtr pkNode = kIter.m_pkNode;

	(*m_pkHashTable)[uiIndex] = pkNode->m_pkNext;

	_DeleteNode(pkNode);
	--m_uiCount;

	return true;
}

template <class TKey, class TData, class THash, class TEquals, class TDataSetter>
inline void
NkHashMap<TKey,TData,THash,TEquals,TDataSetter>::RemoveAll()
{
	NodePtr pkNode;

	for( unsigned int i = 0; i < m_uiHashSize; ++i )
	{
		pkNode = (*m_pkHashTable)[i];

		while( pkNode )
		{
			(*m_pkHashTable)[i] = pkNode->m_pkNext;

			_DeleteNode(pkNode);

			pkNode = (*m_pkHashTable)[i];
		}
	}

	m_uiCount = 0;
}

//-------------------------------------------------------------------------------------------------
// 데이터 참조 함수
//-------------------------------------------------------------------------------------------------
template <class TKey, class TData, class THash, class TEquals, class TDataSetter>
inline bool
NkHashMap<TKey,TData,THash,TEquals,TDataSetter>::GetData(const TKey& key,  TData& data) const
{
	unsigned int uiIndex = THash::KeyToHashIndex(key, m_uiHashSize);
	NodePtr pkNode = (*m_pkHashTable)[uiIndex];

	while( pkNode )
	{
		if( TEquals::Equals(key, pkNode->m_key) )
		{
			data = pkNode->m_data;
			return true;
		}

		pkNode = pkNode->m_pkNext;
	}

	return false;
}

template <class TKey, class TData, class THash, class TEquals, class TDataSetter>
inline TData*
NkHashMap<TKey,TData,THash,TEquals,TDataSetter>::GetDataPtr(const TKey& key) const
{
	unsigned int uiIndex = THash::KeyToHashIndex(key, m_uiHashSize);
	NodePtr pkNode = (*m_pkHashTable)[uiIndex];

	while( pkNode )
	{
		if( TEquals::Equals(key, pkNode->m_key) )
		{
			return &pkNode->m_data;
		}

		pkNode = pkNode->m_pkNext;
	}

	return 0;
}

//-------------------------------------------------------------------------------------------------
// 검색 함수
//-------------------------------------------------------------------------------------------------
template <class TKey, class TData, class THash, class TEquals, class TDataSetter>
inline bool
NkHashMap<TKey,TData,THash,TEquals,TDataSetter>::Find(const TKey& key,  Iterator& kIter) const
{
	unsigned int uiIndex = THash::KeyToHashIndex(key, m_uiHashSize);
	
	kIter.m_uiHashIndex = uiIndex;

	NodePtr pkNode = (*m_pkHashTable)[uiIndex];

	while( pkNode )
	{
		if( TEquals::Equals(key, pkNode->m_key) )
		{
			kIter.m_pkNode = pkNode;
			return true;
		}

		pkNode = pkNode->m_pkNext;
	}

	kIter.m_pkNode = 0;

	return false;
}

// NewNode
template <class TKey, class TData, class THash, class TEquals, class TDataSetter>
inline typename NkHashMap<TKey,TData,THash,TEquals,TDataSetter>::NodePtr
NkHashMap<TKey,TData,THash,TEquals,TDataSetter>::_NewNode()
{
	return NkNew NkHashMapNode<TKey,TData>;
}

// DeleteNode
template <class TKey, class TData, class THash, class TEquals, class TDataSetter>
inline void
NkHashMap<TKey,TData,THash,TEquals,TDataSetter>::_DeleteNode(NodePtr pkNode)
{
	TDataSetter::DeleteData(pkNode->m_key, pkNode->m_data);
	NkDelete pkNode;
}



