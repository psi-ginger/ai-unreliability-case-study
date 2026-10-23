//-------------------------------------------------------------------------------------------------
// NkDeque.inl
// 엔도어즈 기술개발팀
// 이제완
//-------------------------------------------------------------------------------------------------

template <class T> inline
NkDeque<T>::NkDeque(unsigned int uiInitSize, unsigned int uiMaxSize, unsigned int uiGrowSize)
{
	if( uiInitSize > 0 )
	{
		m_pBase = NkAlloc(T, uiInitSize);
		NkAssert(m_pBase != NULL);
	}
	else
		m_pBase = 0;

	if( uiMaxSize < uiInitSize ) 
		m_uiMaxSize = uiInitSize;
	else 
		m_uiMaxSize = uiMaxSize;

	m_uiAllocedSize = uiInitSize;
	m_uiGrowSize = uiGrowSize;

	m_uiCount = 0;
	m_uiFrontPos = 0;
	m_uiBackPos = 0;
}

template <class T> inline
NkDeque<T>::~NkDeque()
{
	if( m_pBase )
		NkFree( m_pBase );
}

template <class T> inline
unsigned int NkDeque<T>::GetCount() const
{
	return m_uiCount;
}

template <class T> inline
unsigned int NkDeque<T>::GetAllocedCount() const
{
	return m_uiAllocedSize;
}

template <class T> inline
unsigned int NkDeque<T>::GetAllocedBytes() const
{
	return sizeof(T) * m_uiAllocedSize;
}

template <class T> inline
bool NkDeque<T>::IsEmpty() const 
{
	return m_uiCount == 0;
}

template <class T> inline
bool NkDeque<T>::IsFull() const 
{
	return m_uiCount == m_uiMaxSize;
}

template <class T> inline
void NkDeque<T>::RemoveAll()
{
	m_uiCount = 0;
	m_uiFrontPos = 0;
	m_uiBackPos = 0;
}

template <class T>
bool NkDeque<T>::PushFront(const T& data)
{
	if( m_uiCount == m_uiAllocedSize )
	{
		if( m_uiAllocedSize == m_uiMaxSize )
			return false;

		unsigned int uiNewSize;

		if( m_uiGrowSize != GROWSIZE_DOUBLE )
			uiNewSize = m_uiAllocedSize + m_uiGrowSize;
		else if( m_uiAllocedSize == 0 )
			uiNewSize = 2;
		else
			uiNewSize = m_uiAllocedSize * 2;

		if( uiNewSize > m_uiMaxSize )
			uiNewSize = m_uiMaxSize;

		_Realloc(uiNewSize);
	}

	if( m_uiCount != 0 )
		--m_uiFrontPos;

	if( m_uiFrontPos == (unsigned int)-1 )
		m_uiFrontPos = m_uiAllocedSize - 1;

	m_pBase[m_uiFrontPos] = data;

	++m_uiCount;

	return true;
}

template <class T>
bool NkDeque<T>::PushBack(const T& data)
{
	if( m_uiCount == m_uiAllocedSize )
	{
		if( m_uiAllocedSize == m_uiMaxSize )
			return false;

		unsigned int uiNewSize;

		if( m_uiGrowSize != GROWSIZE_DOUBLE )
			uiNewSize = m_uiAllocedSize + m_uiGrowSize;
		else if( m_uiAllocedSize == 0 )
			uiNewSize = 2;
		else
			uiNewSize = m_uiAllocedSize * 2;

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

	return true;
}

template <class T> inline
void NkDeque<T>::PopFront()
{
	if( m_uiCount == 0 ) 
		return false;

	if( m_uiCount == 1)
	{
		m_uiCount = 0;
		return;
	}

	++m_uiFrontPos;

	if( m_uiFrontPos == m_uiAllocedSize ) 
		m_uiFrontPos = 0;

	--m_uiCount;
}

template <class T> inline
void NkDeque<T>::PopBack()
{
	if( m_uiCount == 0 ) 
		return false;

	if( m_uiCount == 1)
	{
		m_uiCount = 0;
		return;
	}

	if( m_uiBackPos == 0 ) 
		m_uiBackPos = m_uiAllocedSize - 1;
	else
		--m_uiBackPos;

	--m_uiCount;

	return true;
}

template <class T> inline
bool NkDeque<T>::PopFront(T& data)
{
	if( m_uiCount == 0 ) 
		return false;

	data = m_pBase[m_uiFrontPos];

	if( m_uiCount == 1 )
	{
		m_uiCount = 0;
		return true;
	}

	if( m_uiFrontPos == m_uiAllocedSize - 1) 
		m_uiFrontPos = 0;
	else
		++m_uiFrontPos;

	--m_uiCount;

	return true;
}

template <class T> inline
bool NkDeque<T>::PopBack(T& data)
{
	if( m_uiCount == 0 ) 
		return false;

	data = m_pBase[m_uiBackPos];

	if( m_uiCount == 1 )
	{
		m_uiCount = 0;
		return true;
	}

	if( m_uiBackPos == 0 ) 
		m_uiBackPos = m_uiAllocedSize - 1;
	else
		--m_uiBackPos;

	--m_uiCount;

	return true;
}

template <class T> inline
T& NkDeque<T>::GetFrontData() const
{
	NkAssert(m_uiCount != 0);

	return m_pBase[m_uiFrontPos];
}

template <class T> inline
T& NkDeque<T>::GetBackData() const
{
	NkAssert(m_uiCount != 0);

	return m_pBase[m_uiBackPos];
}


template <class T> inline
T* NkDeque<T>::GetFrontDataPtr() const
{
	if( m_uiCount == 0 )
		return 0;

	return &m_pBase[m_uiFrontPos];
}

template <class T> inline
T* NkDeque<T>::GetBackDataPtr() const
{
	if( m_uiCount == 0 )
		return 0;

	return &m_pBase[m_uiBackPos];
}

template <class T> inline
T& NkDeque<T>::GetData(unsigned int uiIndex) const
{
	if( m_uiCount == 0 )
		return 0;

	uiIndex = (m_uiFrontPos + uiIndex) % m_uiAllocedSize;

	return m_pBase[uiIndex];
}


template <class T> inline
T* NkDeque<T>::GetDataPtr(unsigned int uiIndex) const
{
	if( m_uiCount == 0 )
		return 0;

	uiIndex = (m_uiFrontPos + uiIndex) % m_uiAllocedSize;

	return &m_pBase[uiIndex];
}

template <class T> inline
T& NkDeque<T>::operator [](unsigned int uiIndex) const
{
	NkAssert(uiIndex < m_uiCount);

	uiIndex = (m_uiFrontPos + uiIndex) % m_uiAllocedSize;

	return m_pBase[uiIndex];
}


template <class T> inline
void NkDeque<T>::_Realloc(unsigned int uiNewSize)
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

