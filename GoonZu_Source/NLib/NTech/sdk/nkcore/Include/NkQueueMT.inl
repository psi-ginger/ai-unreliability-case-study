//-------------------------------------------------------------------------------------------------
// NkQueueMT.inl
// 엔도어즈 기술개발팀
// 이제완
//-------------------------------------------------------------------------------------------------

template <class T> inline
NkQueueMT<T>::NkQueueMT(unsigned int uiInitSize, unsigned int uiMaxSize, unsigned int uiGrowSize)
{
	InitializeCriticalSection(&m_cs);

	m_pBase = NkAlloc(T, uiInitSize);
	NkAssert(m_pBase != NULL);

	if( uiMaxSize < uiInitSize ) 
		m_uiMaxSize = uiInitSize;
	else 
		m_uiMaxSize = uiMaxSize;

	m_uiAllocedSize = uiInitSize;
	m_uiGrowSize = uiGrowSize;

	m_uiFrontPos = 0;
	m_uiBackPos = 0;
	m_uiCount = 0;
}

template <class T> inline
NkQueueMT<T>::~NkQueueMT()
{
	NkFree(m_pBase);

	DeleteCriticalSection(&m_cs);
}

template <class T> inline
unsigned int NkQueueMT<T>::GetCount() const
{
	return m_uiCount;
}

template <class T> inline
unsigned int NkQueueMT<T>::GetAllocedCount() const
{
	return m_uiAllocedSize;
}

template <class T> inline
unsigned int NkQueueMT<T>::GetAllocedBytes() const
{
	return sizeof(T) * m_uiAllocedSize;
}

template <class T> inline
void NkQueueMT<T>::RemoveAll()
{
	EnterCriticalSection(&m_cs);

	m_uiCount = 0;
	m_uiFrontPos = 0;
	m_uiBackPos = 0;

	LeaveCriticalSection(&m_cs);
}

template <class T> inline
bool NkQueueMT<T>::IsEmpty() const 
{
	return m_uiCount == 0;
}

template <class T> inline
bool NkQueueMT<T>::IsFull() const 
{
	return m_uiCount == m_uiMaxSize;
}

template <class T>
bool NkQueueMT<T>::Push(const T& data)
{
	EnterCriticalSection(&m_cs);

	if( m_uiCount == m_uiAllocedSize )
	{
		if( m_uiAllocedSize == m_uiMaxSize )
		{
			LeaveCriticalSection(&m_cs);
			return false;
		}

		unsigned int uiNewSize;

		if( m_uiGrowSize == GROWSIZE_DOUBLE )
			uiNewSize = m_uiAllocedSize * 2;
		else
			uiNewSize = m_uiAllocedSize + m_uiGrowSize;

		if( uiNewSize > m_uiMaxSize )
			uiNewSize = m_uiMaxSize;

		_Realloc(uiNewSize);
	}

	if( m_uiCount != 0 )
		++m_uiBackPos;

	if( m_uiBackPos == m_uiAllocedSize )
		m_uiBackPos = 0;

	m_pBase[m_uiBackPos] = data;

	++m_uiCount;

	LeaveCriticalSection(&m_cs);

	return true;
}


template <class T> inline
bool NkQueueMT<T>::Pop(T& data)
{
	EnterCriticalSection(&m_cs);

	if( m_uiCount == 0 )
	{
		LeaveCriticalSection(&m_cs);
		return false;
	}

	data = m_pBase[m_uiFrontPos];

	if( m_uiCount == 1 )
	{
		m_uiCount = 0;

		LeaveCriticalSection(&m_cs);
		return true;
	}

	++m_uiFrontPos;

	if( m_uiFrontPos == m_uiAllocedSize ) 
		m_uiFrontPos = 0;

	--m_uiCount;

	LeaveCriticalSection(&m_cs);
	return true;
}

template <class T> inline
void NkQueueMT<T>::Pop()
{
	EnterCriticalSection(&m_cs);

	if( m_uiCount == 0 ) 
	{
		LeaveCriticalSection(&m_cs);
		return false;
	}

	if( m_uiCount == 1)
	{
		m_uiCount = 0;

		LeaveCriticalSection(&m_cs);
		return;
	}

	++m_uiFrontPos;

	if( m_uiFrontPos == m_uiAllocedSize ) 
		m_uiFrontPos = 0;

	--m_uiCount;

	LeaveCriticalSection(&m_cs);
}


template <class T> inline
void NkQueueMT<T>::_Realloc(unsigned int uiNewSize)
{
	NkAssert(uiNewSize > m_uiAllocedSize);

	T* pNewQueue;

	pNewQueue = NkAlloc(T, uiNewSize);

	if( m_uiCount != 0 )
	{	
		if( m_uiBackPos < m_uiFrontPos )
		{
			memcpy(pNewQueue, m_pBase, sizeof(T) * (m_uiBackPos + 1));

			unsigned int uiSize = m_uiAllocedSize - m_uiFrontPos;
			memcpy(pNewQueue + uiNewSize - uiSize, m_pBase + m_uiFrontPos, sizeof(T) * uiSize);

			m_uiFrontPos = uiNewSize - uiSize;
		}
		else
		{
			memcpy(pNewQueue, m_pBase + m_uiFrontPos, sizeof(T) * m_uiCount);
			m_uiFrontPos = 0;
			m_uiBackPos = m_uiCount - 1;
		}

		m_uiAllocedSize = uiNewSize;
	}
	else
	{
		m_uiFrontPos = 0;
		m_uiBackPos = 0;
	}

	NkFree( m_pBase );
	m_pBase = pNewQueue;
}

