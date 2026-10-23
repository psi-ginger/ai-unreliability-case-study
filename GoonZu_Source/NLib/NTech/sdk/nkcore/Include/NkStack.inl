//-------------------------------------------------------------------------------------------------
// NkStack.inl
// 엔도어즈 기술개발팀
// 이제완
//-------------------------------------------------------------------------------------------------

template <class T> inline
NkStack<T>::NkStack(unsigned int uiInitSize, unsigned int uiMaxSize, unsigned int uiGrowSize)
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
}

template <class T> inline
NkStack<T>::~NkStack()
{
	if( m_pBase )
		NkFree( m_pBase );
}

template <class T> inline
unsigned int NkStack<T>::GetCount() const
{
	return m_uiCount;
}

template <class T> inline
unsigned int NkStack<T>::GetAllocedCount() const
{
	return m_uiAllocedSize;
}

template <class T> inline
unsigned int NkStack<T>::GetAllocedBytes() const
{
	return sizeof(T) * m_uiAllocedSize;
}

template <class T> inline
bool NkStack<T>::IsEmpty()
{
	return m_uiCount == 0;
}

template <class T> inline
bool NkStack<T>::IsFull()
{
	return m_uiCount == m_uiMaxSize;
}

template <class T> inline
void NkStack<T>::RemoveAll()
{
	m_uiCount = 0;
}

template <class T>
bool NkStack<T>::Push(const T& data)
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

	m_pBase[m_uiCount++] = data;

	return true;
}

template <class T> inline
bool NkStack<T>::Pop(T& data)
{
	if( m_uiCount == 0 ) 
		return false;

	data = m_pBase[--m_uiCount];

	return true;
}

template <class T> inline
void NkStack<T>::Pop()
{
	if( m_uiCount == 0 ) 
		return;

	--m_uiCount;

	return true;
}

template <class T> inline
T& NkStack<T>::GetData()
{
	if( m_uiCount == 0 ) 
		return 0;

	return m_pBase[m_uiCount - 1];
}

template <class T> inline
T* NkStack<T>::GetDataPtr()
{
	if( m_uiCount == 0 ) 
		return 0;

	return &m_pBase[m_uiCount - 1];
}

template <class T> inline
T& NkStack<T>::GetData(unsigned int uiIndex) const
{
	if( m_uiCount == 0 )
		return 0;

	return m_pBase[uiIndex];
}


template <class T> inline
T* NkStack<T>::GetDataPtr(unsigned int uiIndex) const
{
	if( m_uiCount == 0 )
		return 0;

	return &m_pBase[uiIndex];
}

template <class T> inline
T& NkStack<T>::operator [](unsigned int uiIndex) const
{
	NkAssert(uiIndex < m_uiCount);

	return m_pBase[uiIndex];
}

template <class T>
void NkStack<T>::Realloc(unsigned int uiNewSize, unsigned int uiGrowSize, unsigned int uiMaxSize)
{
	if( uiNewSize < m_uiCount )
		uiNewSize = m_uiCount;

	if( uiNewSize == m_uiAllocedSize )
		return;

	if( uiNewSize == 0 )
		uiNewSize = 1;

	_Realloc(uiNewSize);

	m_uiGrowSize = uiGrowSize;
	m_uiMaxSize = uiMaxSize;
}

template <class T> inline
void NkStack<T>::_Realloc(unsigned int uiNewSize)
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
