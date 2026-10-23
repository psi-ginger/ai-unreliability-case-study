//-------------------------------------------------------------------------------------------------
// NkList.inl
// 엔도어즈 기술개발팀
// 이제완
//-------------------------------------------------------------------------------------------------

template <class T> inline
NkList<T>::NkList()
: m_stCount(0), m_pkFront(0), m_pkBack(0)
{
}

template <class T> inline
NkList<T>::~NkList()
{
	RemoveAll();
}

template <class T> inline
size_t NkList<T>::GetCount() const
{
	return m_stCount;
}

template <class T> inline
size_t NkList<T>::GetAllocedByteSize() const
{
	return sizeof(NkListNode<T>) * m_stCount;
}

template <class T> inline
T& NkList<T>::GetData(NkListPos kPos) const
{
	return ((NkListNode<T>*)kPos)->m_data;
}

template <class T> inline
T* NkList<T>::GetDataPtr(NkListPos kPos) const
{
	return &((NkListNode<T>*)kPos)->m_data;
}

//-------------------------------------------------------------------------------------------------
// Position
//-------------------------------------------------------------------------------------------------

template <class T> inline
NkListPos NkList<T>::GetFrontPos() const
{
	return m_pkFront;
}

template <class T> inline
NkListPos NkList<T>::GetBackPos() const
{
	return m_pkBack;
}

template <class T> inline
NkListPos NkList<T>::GetNextPos(NkListPos kPos) const
{
	return ((NkListNode<T>*)kPos)->m_pkNext;
}

template <class T> inline
NkListPos NkList<T>::GetPrevPos(NkListPos kPos) const
{
	return ((NkListNode<T>*)kPos)->m_pkPrev;
}

template <class T> inline
void NkList<T>::MoveFront(NkListPos& kPos) const
{
	kPos = (NkListPos)m_pkFront;
}

template <class T> inline
void NkList<T>::MoveBack(NkListPos& kPos) const
{
	kPos = (NkListPos)m_pkBack;
}

template <class T> inline
void NkList<T>::MoveNext(NkListPos& kPos) const
{
	kPos = ((NkListNode<T>*)kPos)->m_pkNext;
}

template <class T> inline
void NkList<T>::MovePrev(NkListPos& kPos) const
{
	kPos = ((NkListNode<T>*)kPos)->m_pkPrev;
}

//-----------------------------------------------------------------------------
// remove
//-----------------------------------------------------------------------------
template <class T>
void NkList<T>::RemoveAll()
{
	NkListNode<T>* pkNode = m_pkFront;
	NkListNode<T>* pkDelNode;

	while( pkNode )
	{
		pkDelNode = pkNode;
		pkNode = pkNode->m_pkNext;
		DeleteNode(pkDelNode);
	}

	m_stCount = 0;
	m_pkFront = 0;
	m_pkBack = 0;
}

template <class T>
void NkList<T>::Remove(NkListPos kPos)
{
	NkListNode<T>* pkNode = (NkListNode<T>*)kPos;

	if( !pkNode ) 
		return;

	if( pkNode == m_pkFront )
	{
		m_pkFront = m_pkFront->m_pkNext;

		if( m_pkFront ) 
			m_pkFront->m_pkPrev = 0;
		else 
			m_pkBack = 0;

		DeleteNode(pkNode);
	}
	else if( pkNode == m_pkBack )
	{
		m_pkBack = m_pkBack->m_pkPrev;

		if( m_pkBack ) 
			m_pkBack->m_pkNext = 0;
		else 
			m_pkFront = 0;

		DeleteNode(pkNode);
	}
	else
	{
		NkListNode<T>* pkPrev = pkNode->m_pkPrev;
		NkListNode<T>* pkNext = pkNode->m_pkNext;

		if( pkPrev ) 
			pkPrev->m_pkNext = pkNext;
		if( pkNext ) 
			pkNext->m_pkPrev = pkPrev;

		DeleteNode(pkNode);
	}
}

template <class T>
NkListNode<T>* NkList<T>::Detach(NkListPos kPos)
{
	NkListNode<T>* pkNode = (NkListNode<T>*)kPos;

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
		NkListNode<T>* pkPrev = pkNode->m_pkPrev;
		NkListNode<T>* pkNext = pkNode->m_pkNext;

		if( pkPrev ) 
			pkPrev->m_pkNext = pkNext;
		if( pkNext ) 
			pkNext->m_pkPrev = pkPrev;
	}

	return pkNode;
}


template <class T> inline
NkListNode<T>* NkList<T>::NewNode()
{
	return NkNew NkListNode< T >;
}

template <class T> inline
void NkList<T>::DeleteNode( NkListNode<T>* pkNode )
{
	NkDelete pkNode;

	m_stCount--;
}

//-----------------------------------------------------------------------------
// add
//-----------------------------------------------------------------------------
template <class T> inline
T* NkList<T>::NewFront()
{
	NkListNode<T>* pkNode = NewNode();

	PushNodeFront(pkNode);

	return &pkNode->m_data;
}

template <class T> inline
T* NkList<T>::NewBack()
{
	NkListNode<T>* pkNode = NewNode();

	PushNodeBack(pkNode);

	return &pkNode->m_data;
}


template <class T> inline
void NkList<T>::PushNodeFront(NkListNode<T>* pkNode)
{
	pkNode->m_pkPrev = 0;
	pkNode->m_pkNext = m_pkFront;

	if( m_pkFront ) 
		m_pkFront->m_pkPrev = pkNode;
	else 
		m_pkBack = pkNode;

	m_pkFront = pkNode;
	m_stCount++;
}

template <class T> inline
void NkList<T>::PushNodeBack(NkListNode<T>* pkNode)
{
	pkNode->m_pkNext = 0;
	pkNode->m_pkPrev = m_pkBack;

	if( m_pkBack ) 
		m_pkBack->m_pkNext = pkNode;
	else 
		m_pkFront = pkNode;

	m_pkBack = pkNode;
	m_stCount++;
}

template <class T>
void NkList<T>::PopFront()
{
	NkListNode<T>* pkNode = m_pkFront;

	if( !pkNode ) 
		return;

	m_pkFront = m_pkFront->m_pkNext;

	if( m_pkFront ) 
		m_pkFront->m_pkPrev = 0;
	else 
		m_pkBack = 0;

	DeleteNode(pkNode);
}

template <class T>
void NkList<T>::PopBack()
{
	NkListNode<T>* pkNode = m_pkBack;

	if( !pkNode ) 
		return;

	m_pkBack = m_pkBack->m_pkPrev;

	if( m_pkBack ) 
		m_pkBack->m_pkNext = 0;
	else 
		m_pkFront = 0;

	DeleteNode(pkNode);
}

template <class T> inline
void NkList<T>::PushFront(const T& data)
{
	NkListNode<T>* pkNode = NewNode();

	pkNode->m_data = data;

	PushNodeFront(pkNode);
}

template <class T> inline
void NkList<T>::PushBack(const T& data)
{
	NkListNode<T>* pkNode = NewNode();

	pkNode->m_data = data;

	PushNodeBack(pkNode);
}

//-----------------------------------------------------------------------------
// insert
//-----------------------------------------------------------------------------
template <class T> inline
T* NkList<T>::InsertNewAfter(NkListPos kPos)
{
	NkListNode<T>* pkNode = NewNode();

	InsertNodeAfter(kPos, pkNode);

	return &pkNode->m_data;
}

template <class T> inline
T* NkList<T>::InsertNewBefore(NkListPos kPos)
{
	NkListNode<T>* pkNode = NewNode();

	InsertNodeBefore(kPos, pkNode);

	return &pkNode->m_data;
}

template <class T> inline
NkListPos NkList<T>::InsertAfter(NkListPos kPos, const T& data)
{
	NkListNode<T>* pkNode = NewNode();

	pkNode->m_data = data;

	return InsertNodeAfter(kPos, pkNode);
}

template <class T> inline
NkListPos NkList<T>::InsertBefore(NkListPos kPos, const T& data)
{
	NkListNode<T>* pkNode = NewNode();

	pkNode->m_data = data;

	return InsertNodeBefore(kPos, pkNode);
}


template <class T> inline
NkListPos NkList<T>::InsertNodeAfter(NkListPos kPos, NkListNode<T>* pkNode)
{
	NkListNode<T>* pkPrev = (NkListNode<T>*)kPos;

	pkNode->m_pkPrev = pkPrev;
	pkNode->m_pkNext = pkPrev->m_pkNext;

	if( pkPrev->m_pkNext ) 
		pkPrev->m_pkNext = pkNode;
	else 
		m_pkBack = pkNode;

	pkPrev->m_pkNext = pkNode;
	m_stCount++;

	return pkNode;
}

template <class T> inline
NkListPos NkList<T>::InsertNodeBefore(NkListPos kPos, NkListNode<T>* pkNode)
{
	NkListNode<T>* pkNext = (NkListNode<T>*)kPos;

	pkNode->m_pkPrev = pkPrev->m_pkPrev;
	pkNode->m_pkNext = pkNext;

	if( pkNext->m_pkPrev ) 
		pkNext->m_pkPrev = pkNode;
	else 
		m_pkFront = pkNode;

	pkNext->m_pkPrev = pkNode;
	m_stCount++;

	return pkNode;
}

//-----------------------------------------------------------------------------
// search
//-----------------------------------------------------------------------------
template <class T> inline
NkListPos NkList<T>::FindPos(const T& data, NkListPos kStart)
{
	if( kStart == 0 )
		kStart = m_pkFront;

	while( kStart )
	{
		if( data == GetData(kStart) ) 
			return kStart;

		MoveNext(kStart);
	}
	return 0;
}
