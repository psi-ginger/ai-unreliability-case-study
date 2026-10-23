//-------------------------------------------------------------------------------------------------
// NkList.inl
// 엔도어즈 기술개발팀
// 이제완
//-------------------------------------------------------------------------------------------------

template <class T>
inline
NkList<T>::NkList()
: m_uiCount(0), m_pkFront(0), m_pkBack(0)
{
}

template <class T>
inline
NkList<T>::~NkList()
{
	RemoveAll();
}

//-------------------------------------------------------------------------------------------------
// 크기 관련 함수
//-------------------------------------------------------------------------------------------------
template <class T>
inline unsigned int
NkList<T>::GetCount() const
{
	return m_uiCount;
}

template <class T>
inline unsigned int
NkList<T>::GetAllocedByteSize() const
{
	return sizeof(NkListNode<T>) * m_uiCount;
}

template <class T>
inline typename NkList<T>::Iterator
NkList<T>::Begin() const
{
	return m_pkFront;
}

template <class T>
inline typename NkList<T>::Iterator
NkList<T>::End() const
{
	return 0;
}

//-------------------------------------------------------------------------------------------------
// 크기 관련 함수
//-------------------------------------------------------------------------------------------------
template <class T>
inline T&
NkList<T>::GetData(NkListPos kPos) const
{
	return ((NodePtr)kPos)->m_data;
}

template <class T>
inline T*
NkList<T>::GetDataPtr(NkListPos kPos) const
{
	return &((NodePtr)kPos)->m_data;
}

//-------------------------------------------------------------------------------------------------
// Position
//-------------------------------------------------------------------------------------------------

template <class T>
inline NkListPos
NkList<T>::GetFrontPos() const
{
	return m_pkFront;
}

template <class T>
inline NkListPos
NkList<T>::GetBackPos() const
{
	return m_pkBack;
}

template <class T>
inline NkListPos
NkList<T>::GetNextPos(NkListPos kPos) const
{
	return ((NodePtr)kPos)->m_pkNext;
}

template <class T>
inline NkListPos
NkList<T>::GetPrevPos(NkListPos kPos) const
{
	return ((NodePtr)kPos)->m_pkPrev;
}

template <class T>
inline void
NkList<T>::MoveFront(NkListPos& kPos) const
{
	kPos = m_pkFront;
}

template <class T>
inline void
NkList<T>::MoveBack(NkListPos& kPos) const
{
	kPos = m_pkBack;
}

template <class T>
inline void
NkList<T>::MoveNext(NkListPos& kPos) const
{
	kPos = ((NodePtr)kPos)->m_pkNext;
}

template <class T>
inline void
NkList<T>::MovePrev(NkListPos& kPos) const
{
	kPos = ((NodePtr)kPos)->m_pkPrev;
}

//-------------------------------------------------------------------------------------------------
// 삽입 함수
//-------------------------------------------------------------------------------------------------
template <class T>
inline T*
NkList<T>::NewFront()
{
	NodePtr pkNode = _NewNode();

	PushNodeFront(pkNode);

	return &pkNode->m_data;
}

template <class T>
inline T*
NkList<T>::NewBack()
{
	NodePtr pkNode = _NewNode();

	PushNodeBack(pkNode);

	return &pkNode->m_data;
}

template <class T>
inline void
NkList<T>::PushFront(const T& data)
{
	NodePtr pkNode = _NewNode();

	pkNode->m_data = data;

	PushNodeFront(pkNode);
}

template <class T>
inline void
NkList<T>::PushBack(const T& data)
{
	NodePtr pkNode = _NewNode();

	pkNode->m_data = data;

	PushNodeBack(pkNode);
}

template <class T>
inline void
NkList<T>::PushNodeFront(NodePtr pkNode)
{
	pkNode->m_pkPrev = 0;
	pkNode->m_pkNext = m_pkFront;

	if( m_pkFront ) 
		m_pkFront->m_pkPrev = pkNode;
	else 
		m_pkBack = pkNode;

	m_pkFront = pkNode;
	++m_uiCount;
}

template <class T>
inline void
NkList<T>::PushNodeBack(NodePtr pkNode)
{
	pkNode->m_pkNext = 0;
	pkNode->m_pkPrev = m_pkBack;

	if( m_pkBack ) 
		m_pkBack->m_pkNext = pkNode;
	else 
		m_pkFront = pkNode;

	m_pkBack = pkNode;
	++m_uiCount;
}

//-----------------------------------------------------------------------------
// insert
//-----------------------------------------------------------------------------
template <class T>
inline T*
NkList<T>::NewAfter(Iterator kIter)
{
	NodePtr pkNode = _NewNode();

	InsertNodeAfter(kIter, pkNode);

	return &pkNode->m_data;
}

template <class T>
inline T*
NkList<T>::NewBefore(Iterator kIter)
{
	NodePtr pkNode = _NewNode();

	InsertNodeBefore(kIter, pkNode);

	return &pkNode->m_data;
}

template <class T>
inline typename NkList<T>::Iterator
NkList<T>::InsertAfter(Iterator kIter, const T& data)
{
	NodePtr pkNode = _NewNode();

	pkNode->m_data = data;

	return InsertNodeAfter(kIter, pkNode);
}

template <class T>
inline typename NkList<T>::Iterator
NkList<T>::InsertBefore(Iterator kIter, const T& data)
{
	NodePtr pkNode = _NewNode();

	pkNode->m_data = data;

	return InsertNodeBefore(kIter, pkNode);
}


template <class T>
inline typename NkList<T>::Iterator
NkList<T>::InsertNodeAfter(Iterator kIter, NodePtr pkNode)
{
	NodePtr pkPrev = kIter.m_pkNode;

	pkNode->m_pkPrev = pkPrev;
	pkNode->m_pkNext = pkPrev->m_pkNext;

	if( pkPrev->m_pkNext ) 
		pkPrev->m_pkNext = pkNode;
	else 
		m_pkBack = pkNode;

	pkPrev->m_pkNext = pkNode;
	++m_uiCount;

	return pkNode;
}

template <class T>
inline typename NkList<T>::Iterator
NkList<T>::InsertNodeBefore(Iterator kIter, NodePtr pkNode)
{
	NodePtr pkNext = kIter.m_pkNode;

	pkNode->m_pkPrev = pkPrev->m_pkPrev;
	pkNode->m_pkNext = pkNext;

	if( pkNext->m_pkPrev ) 
		pkNext->m_pkPrev = pkNode;
	else 
		m_pkFront = pkNode;

	pkNext->m_pkPrev = pkNode;
	++m_uiCount;

	return pkNode;
}

//-----------------------------------------------------------------------------
// 삭제 함수
//-----------------------------------------------------------------------------
template <class T>
void
NkList<T>::PopFront()
{
	NodePtr pkNode = m_pkFront;

	if( !pkNode ) 
		return;

	m_pkFront = m_pkFront->m_pkNext;

	if( m_pkFront ) 
		m_pkFront->m_pkPrev = 0;
	else 
		m_pkBack = 0;

	_DeleteNode(pkNode);
	--m_uiCount;
}

template <class T>
void
NkList<T>::PopBack()
{
	NodePtr pkNode = m_pkBack;

	if( !pkNode ) 
		return;

	m_pkBack = m_pkBack->m_pkPrev;

	if( m_pkBack ) 
		m_pkBack->m_pkNext = 0;
	else 
		m_pkFront = 0;

	_DeleteNode(pkNode);
	--m_uiCount;
}

template <class T>
void
NkList<T>::RemoveAll()
{
	NodePtr pkNode = m_pkFront;
	NodePtr pkDelNode;

	while( pkNode )
	{
		pkDelNode = pkNode;
		pkNode = pkNode->m_pkNext;
		_DeleteNode(pkDelNode);
	}

	m_uiCount = 0;
	m_pkFront = 0;
	m_pkBack = 0;
}

template <class T>
typename NkList<T>::Iterator
NkList<T>::Remove(Iterator kIter)
{
	NodePtr pkNode = kIter.m_pkNode;

	if( !pkNode ) 
		return 0;

	if( pkNode == m_pkFront )
	{
		m_pkFront = m_pkFront->m_pkNext;

		if( m_pkFront ) 
			m_pkFront->m_pkPrev = 0;
		else 
			m_pkBack = 0;

		_DeleteNode(pkNode);
		--m_uiCount;

		return m_pkFront;
	}
	else if( pkNode == m_pkBack )
	{
		m_pkBack = m_pkBack->m_pkPrev;

		if( m_pkBack ) 
			m_pkBack->m_pkNext = 0;
		else 
			m_pkFront = 0;

		_DeleteNode(pkNode);
		--m_uiCount;

		return 0;
	}
	else
	{
		NodePtr pkPrev = pkNode->m_pkPrev;
		NodePtr pkNext = pkNode->m_pkNext;

		if( pkPrev ) 
			pkPrev->m_pkNext = pkNext;
		if( pkNext ) 
			pkNext->m_pkPrev = pkPrev;

		_DeleteNode(pkNode);
		--m_uiCount;

		return pkNext;
	}
}

template <class T>
typename NkList<T>::NodePtr
NkList<T>::Detach(Iterator kIter)
{
	NodePtr pkNode = kIter.m_pkNode;

	if( !pkNode ) 
		return 0;

	if( pkNode == m_pkFront )
	{
		m_pkFront = m_pkFront->m_pkNext;

		if( m_pkFront ) 
			m_pkFront->m_pkPrev = 0;
		else 
			m_pkBack = 0;
	}
	else if( pkNode == m_pkBack )
	{
		m_pkBack = m_pkBack->m_pkPrev;

		if( m_pkBack ) 
			m_pkBack->m_pkNext = 0;
		else 
			m_pkFront = 0;
	}
	else
	{
		NodePtr pkPrev = pkNode->m_pkPrev;
		NodePtr pkNext = pkNode->m_pkNext;

		if( pkPrev ) 
			pkPrev->m_pkNext = pkNext;
		if( pkNext ) 
			pkNext->m_pkPrev = pkPrev;
	}

	return pkNode;
}

//-----------------------------------------------------------------------------
// 검색 함수
//-----------------------------------------------------------------------------
template <class T>
inline typename NkList<T>::Iterator
NkList<T>::Find(const T& data)
{
	Iterator kStart = Begin();
	Iterator kEnd = End();

	for( ; kStart != kEnd; ++kStart )
	{
		if( data == kStart->GetData() ) 
			return kStart;		
	}
	return 0;
}


template <class T>
inline typename NkList<T>::NodePtr
NkList<T>::_NewNode()
{
	return NkNew NkListNode<T>;
}

template <class T>
inline void
NkList<T>::_DeleteNode(NodePtr pkNode)
{
	NkDelete pkNode;
}
