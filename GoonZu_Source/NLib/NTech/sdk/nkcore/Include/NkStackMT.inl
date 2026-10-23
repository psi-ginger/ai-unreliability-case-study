//-------------------------------------------------------------------------------------------------
// NkStackMT.inl
// 엔도어즈 기술개발팀
// 이제완
//-------------------------------------------------------------------------------------------------

template <class T> inline
NkStackMT<T>::NkStackMT(unsigned int uiInitSize, unsigned int uiMaxSize, unsigned int uiGrowSize)
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

	m_uiCount = 0;
}

template <class T> inline
NkStackMT<T>::~NkStackMT()
{
	NkFree(m_pBase);

	DeleteCriticalSection(&m_cs);
}

template <class T> inline
unsigned int NkStackMT<T>::GetCount() const
{
	return m_uiCount;
}

template <class T> inline
unsigned int NkStackMT<T>::GetAllocedCount() const
{
	return m_uiAllocedSize;
}

template <class T> inline
unsigned int NkStackMT<T>::GetAllocedBytes() const
{
	return sizeof(T) * m_uiAllocedSize;
}

template <class T> inline
bool NkStackMT<T>::IsEmpty()
{
	return m_uiCount == 0;
}

template <class T> inline
bool NkStackMT<T>::IsFull()
{
	return m_uiCount == m_uiMaxSize;
}

template <class T> inline
void NkStackMT<T>::RemoveAll()
{
	EnterCriticalSection(&m_cs);
	m_uiCount = 0;
	LeaveCriticalSection(&m_cs);
}

template <class T>
bool NkStackMT<T>::Push(const T& data)
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

	m_pBase[ m_uiCount++ ] = data;

	LeaveCriticalSection(&m_cs);

	return true;
}

template <class T> inline
bool NkStackMT<T>::Pop(T& data)
{
	EnterCriticalSection(&m_cs);

	if( m_uiCount == 0 ) 
	{
		LeaveCriticalSection(&m_cs);
		return false;
	}

	data = m_pBase[ --m_uiCount ];

	LeaveCriticalSection(&m_cs);

	return true;
}

template <class T> inline
void NkStackMT<T>::Pop()
{
	EnterCriticalSection(&m_cs);

	if( m_uiCount == 0 ) 
	{
		LeaveCriticalSection(&m_cs);
		return;
	}

	--m_uiCount;

	LeaveCriticalSection(&m_cs);

	return true;
}

template <class T>
void NkStackMT<T>::Realloc(unsigned int uiNewSize, unsigned int uiGrowSize, unsigned int uiMaxSize)
{
	EnterCriticalSection(&m_cs);

	if( uiNewSize < m_uiCount )
		uiNewSize = m_uiCount;

	if( uiNewSize == m_uiAllocedSize )
		return;

	if( uiNewSize == 0 )
		uiNewSize = 1;

	_Realloc(uiNewSize);

	m_uiGrowSize = uiGrowSize;
	m_uiMaxSize = uiMaxSize;

	LeaveCriticalSection(&m_cs);
}

template <class T> inline
void NkStackMT<T>::_Realloc(unsigned int uiNewSize)
{
	NkAssert(uiNewSize > m_uiAllocedSize);

	T* pNewQueue;

	pNewQueue = NkAlloc(T, uiNewSize);

	if( m_uiCount != 0 )
		memcpy(pNewQueue, m_pBase, sizeof(T) * m_uiCount);

	NkFree( m_pBase );
	m_pBase = pNewQueue;
	m_uiAllocedSize = uiNewSize;
}
