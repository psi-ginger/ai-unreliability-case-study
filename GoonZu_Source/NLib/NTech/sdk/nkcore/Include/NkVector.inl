//-------------------------------------------------------------------------------------------------
// NkVector.inl
// 엔도어즈 기술개발팀
// 이제완
//-------------------------------------------------------------------------------------------------

template <class T> inline
NkVector<T>::NkVector(unsigned int uiInitSize, unsigned int uiMaxSize, unsigned int uiGrowSize)
{
	if( uiInitSize > 0 )
	{
		m_pBase = NkAlloc(T, uiInitSize);
		NkAssert(m_pBase != NULL);

		memset(m_pBase, 0, sizeof(T) * uiInitSize);
	}
	else
		m_pBase = 0;

	if( uiMaxSize < uiInitSize ) 
		m_uiMaxSize = uiInitSize;
	else 
		m_uiMaxSize = uiMaxSize;

	m_uiAllocedSize = uiInitSize;
	m_uiGrowSize = uiGrowSize;

	m_uiCount = uiInitSize;
}

template <class T> inline
NkVector<T>::~NkVector()
{
	if( m_pBase )
		NkFree(m_pBase);
}

//-------------------------------------------------------------------------------------------------
// 크기 관련 함수
//-------------------------------------------------------------------------------------------------
template <class T> inline
unsigned int NkVector<T>::GetCount() const
{
	return m_uiCount;
}

template <class T> inline
unsigned int NkVector<T>::GetAllocedCount() const
{
	return m_uiAllocedSize;
}

template <class T> inline
unsigned int NkVector<T>::GetAllocedBytes() const
{
	return sizeof(T) * m_uiAllocedSize;
}

//-------------------------------------------------------------------------------------------------
// 
//-------------------------------------------------------------------------------------------------
template <class T> inline
NkVectorPos<T> NkVector<T>::BeginPos()
{
	NkVectorPos<T> kPos;
	kPos.pT = GetFrontDataPtr();
	return kPos;
}

template <class T> inline
NkVectorPos<T> NkVector<T>::EndPos()
{
	NkVectorPos<T> kPos;
	kPos.pT = GetBackDataPtr() + 1;
	return kPos;
}

//-------------------------------------------------------------------------------------------------
// 상태 관련 함수
//-------------------------------------------------------------------------------------------------
template <class T> inline
bool NkVector<T>::IsEmpty()
{
	return m_uiCount == 0;
}

template <class T> inline
bool NkVector<T>::IsFull()
{
	return m_uiCount == m_uiMaxSize;
}


//-------------------------------------------------------------------------------------------------
// 대입 함수
//-------------------------------------------------------------------------------------------------
template <class T>
void NkVector<T>::Assign(const NkVector<T>& kVector)
{
	unsigned int uiNewSize = kVector.GetCount();

	if( m_uiCount > 0 )
		NkFree(m_pBase);

	if( uiNewSize > 0 )
	{
		m_pBase = NkAlloc(T, uiNewSize);
		memcpy(m_pBase, kVector.m_pBase, sizeof(T) * uiNewSize);
	}
	else
		m_pBase = 0;

	m_uiAllocedSize = uiNewSize;
	m_uiCount = uiNewSize;

	m_uiMaxSize = kVector.m_uiMaxSize;
	m_uiGrowSize = kVector.m_uiGrowSize;
}

template <class T>
void NkVector<T>::Assign(const T* pDatas, unsigned int uiCount)
{
	if( m_uiCount > 0 )
		NkFree(m_pBase);

	if( uiCount > 0 )
	{
		m_pBase = NkAlloc(T, uiCount);
		memcpy(m_pBase, pDatas, sizeof(T) * uiCount);
	}
	else
		m_pBase = 0;

	m_uiAllocedSize = uiCount;
	m_uiCount = uiCount;
}

template <class T> inline 
const NkVector<T>& NkVector<T>::operator=(const NkVector<T>& kVector)
{
	Assign(kVector);
	return *this;
}

//-------------------------------------------------------------------------------------------------
// 삽입 함수
//-------------------------------------------------------------------------------------------------
template <class T>
bool NkVector<T>::PushFront(const T& data)
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

	for( unsigned int i = m_uiCount; i > 0; --i )
	{
		m_pBase[i] = m_pBase[i - 1];
	}

	*m_pBase = data;
	++m_uiCount;

	return true;
}

template <class T>
bool NkVector<T>::PushBack(const T& data)
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

	m_pBase[ m_uiCount++ ] = data;

	return true;
}

template <class T>
bool NkVector<T>::Insert(unsigned int uiIndex, const T& data)
{
	if( uiIndex > m_uiCount )
		uiIndex = m_uiCount;

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

	for( unsigned int i = m_uiCount; i > uiIndex; --i )
	{
		m_pBase[i] = m_pBase[i - 1];
	}

	m_pBase[uiIndex] = data;
	++m_uiCount;

	return true;
}

//-------------------------------------------------------------------------------------------------
// 삭제 함수
//-------------------------------------------------------------------------------------------------
template <class T> inline
void NkVector<T>::PopFront()
{
	if( m_uiCount == 0 )
		return;

	--m_uiCount;

	if( m_uiCount > 0 )
		memmove(m_pBase, &m_pBase[1], sizeof(T) * m_uiCount);
}

template <class T> inline
void NkVector<T>::PopBack()
{
	if( m_uiCount > 0 )
		--m_uiCount;
}

template <class T> inline
bool NkVector<T>::PopFront(T& data)
{
	if( m_uiCount == 0 ) 
		return false;

	data = *m_pBase;

	--m_uiCount;
	memmove(m_pBase, &m_pBase[1], sizeof(T) * m_uiCount);

	return true;
}

template <class T> inline
bool NkVector<T>::PopBack(T& data)
{
	if( m_uiCount == 0 ) 
		return false;

	data = m_pBase[ --m_uiCount ];

	return true;
}

template <class T> inline
void NkVector<T>::Remove(unsigned int uiIndex)
{
	if( uiIndex >= m_uiCount )
		return;

	--m_uiCount;
	memmove(&m_pBase[uiIndex], &m_pBase[uiIndex + 1], sizeof(T) * (m_uiCount - uiIndex));
}

template <class T> inline
void NkVector<T>::RemoveData(const T& data)
{
	for( unsigned int i = 0; i < m_uiCount; ++i )
	{
		if( m_pBase[i] == data )
		{
			--m_uiCount;

			if( m_uiCount > 0 )
				memmove(&m_pBase[i], &m_pBase[i + 1], sizeof(T) * (m_uiCount - i));
		}
	}
}

template <class T> inline
void NkVector<T>::RemoveAll()
{
	m_uiCount = 0;
}

//-------------------------------------------------------------------------------------------------
// 데이터 참조 함수
//-------------------------------------------------------------------------------------------------
template <class T> inline
T& NkVector<T>::GetFrontData()
{
	NkAssert(m_uiCount > 0);

	return *m_pBase;
}

template <class T> inline
T* NkVector<T>::GetFrontDataPtr()
{
	if( m_uiCount == 0 ) 
		return 0;

	return m_pBase;
}

template <class T> inline
T& NkVector<T>::GetBackData()
{
	NkAssert(m_uiCount > 0);

	return m_pBase[m_uiCount - 1];
}

template <class T> inline
T* NkVector<T>::GetBackDataPtr()
{
	if( m_uiCount == 0 ) 
		return 0;

	return &m_pBase[m_uiCount - 1];
}

template <class T> inline
T& NkVector<T>::GetData(unsigned int uiIndex)
{
	NkAssert(uiIndex < m_uiCount);

	return m_pBase[uiIndex];
}

template <class T> inline
T* NkVector<T>::GetDataPtr(unsigned int uiIndex)
{
	if( uiIndex >= m_uiCount ) 
		return 0;

	return &m_pBase[uiIndex];
}

template <class T> inline
T& NkVector<T>::operator [](unsigned int uiIndex) const
{
	NkAssert(uiIndex < m_uiCount);

	return m_pBase[uiIndex];
}

template <class T> inline
void NkVector<T>::SwapData(unsigned int uiIndex1, unsigned int uiIndex2)
{
	NkAssert(uiIndex1 < m_uiCount && uiIndex2 < m_uiCount);

	T temp = m_pBase[uiIndex1];
	m_pBase[uiIndex1] = m_pBase[uiIndex2];
	m_pBase[uiIndex2] = temp;
}

//-------------------------------------------------------------------------------------------------
// 데이터 0 초기화 함수 - uiBegin 부터 시작하여 uiEnd 보다 작은 인덱스 범위의 data를 0으로 초기화
//-------------------------------------------------------------------------------------------------
template <class T> inline
void NkVector<T>::ClearData(unsigned int uiBegin, unsigned int uiEnd)
{
	if( uiBegin >= m_uiCount )
		return;

	if( uiEnd > m_uiCount )
		uiEnd = m_uiCount;

	if( uiBegin >= uiEnd )
		return;

	unsigned int uiSize = uiEnd - uiBegin;

	memset(&m_pBase[uiBegin], 0, sizeof(T) * uiSize);
}

//-------------------------------------------------------------------------------------------------
// 검색 함수 - uiBegin 부터 시작하여 uiEnd 보다 작은 인덱스 범위에서 data를 검색한다
//-------------------------------------------------------------------------------------------------
template <class T> inline
unsigned int NkVector<T>::FindIndex(const T& data, unsigned int uiBegin, unsigned int uiEnd) const
{
	if( uiEnd > m_uiCount )
		uiEnd = m_uiCount;

	for( unsigned int i = uiBegin; i < uiEnd; ++i )
	{
		if( m_pBase[i] == data )
			return i;
	}
	return INVALID_INDEX;
}

//-------------------------------------------------------------------------------------------------
// 정렬 함수 - uiBegin 부터 시작하여 uiEnd 보다 작은 인덱스 범위에서 data를 정렬한다
//-------------------------------------------------------------------------------------------------
template <class T> inline
void NkVector<T>::SortAscend(unsigned int uiBegin, unsigned int uiEnd)
{
	if( uiBegin >= m_uiCount )
		return;

	if( uiEnd > m_uiCount )
		uiEnd = m_uiCount;

	if( uiBegin >= uiEnd )
		return;

	unsigned int uiSize = uiEnd - uiBegin;

	NkSort<T>::QuickSort(&m_pBase[uiBegin], uiSize, NkVector<T>::ASCEND);
}

template <class T> inline
void NkVector<T>::SortDescend(unsigned int uiBegin, unsigned int uiEnd)
{
	if( uiBegin >= m_uiCount )
		return;

	if( uiEnd > m_uiCount )
		uiEnd = m_uiCount;

	if( uiBegin >= uiEnd )
		return;

	unsigned int uiSize = uiEnd - uiBegin;

	NkSort<T>::QuickSort(&m_pBase[uiBegin], uiSize, NkVector<T>::DESCEND);
}

template <class T> inline
void NkVector<T>::Sort(int (*pCompareFunc)(T&, T&), unsigned int uiBegin, unsigned int uiEnd)
{
	if( uiBegin >= m_uiCount )
		return;

	if( uiEnd > m_uiCount )
		uiEnd = m_uiCount;

	if( uiBegin >= uiEnd )
		return;

	unsigned int uiSize = uiEnd - uiBegin;

	NkSort<T>::QuickSort(&m_pBase[uiBegin], uiSize, pCompareFunc);
}


// 오름차순 크기 비교 함수
template <class T>
int NkVector<T>::ASCEND(T& data1, T& data2)
{
	return (int)(data1 > data2);
}

// 내림차순 크기 비교 함수
template <class T>
int NkVector<T>::DESCEND(T& data1, T& data2)
{
	return (int)(data1 < data2);
}


//-------------------------------------------------------------------------------------------------
// 뒤섞기 함수 - uiBegin 부터 시작하여 uiEnd 보다 작은 인덱스 범위의 data를 랜덤하게 섞는다
//-------------------------------------------------------------------------------------------------
template <class T>
void NkVector<T>::Shuffle(unsigned int uiBegin, unsigned int uiEnd)
{
	if( uiBegin >= m_uiCount )
		return;

	if( uiEnd > m_uiCount )
		uiEnd = m_uiCount;

	if( uiBegin >= uiEnd )
		return;

	unsigned int uiSize = uiEnd - uiBegin;

	unsigned int uiIndex1, uiIndex2;
	T temp;

	for( unsigned int i = 0; i < uiSize * 2; ++i )
	{
		uiIndex1 = rand() % uiSize;
		uiIndex2 = (rand() % uiSize) + uiIndex1;

		uiIndex1 += uiBegin;
		uiIndex2 = (uiIndex2 % uiSize) + uiBegin;

		temp = m_pBase[uiIndex1];
		m_pBase[uiIndex1] = m_pBase[uiIndex2];
		m_pBase[uiIndex2] = temp;
	}
}

//-------------------------------------------------------------------------------------------------
// 메모리 할당 크기 변경 함수 - 데이터 손실 없음, 메모리 재할당 발생
//-------------------------------------------------------------------------------------------------
template <class T>
void NkVector<T>::Reserve(unsigned int uiNewAllocSize, unsigned int uiMaxSize, unsigned int uiGrowSize)
{
	if( uiNewAllocSize < m_uiCount )
		uiNewAllocSize = m_uiCount;

	if( uiNewAllocSize == m_uiAllocedSize ||
		uiNewAllocSize == 0 )
		return;

	_Realloc(uiNewAllocSize);

	if( m_uiMaxSize != 0 )
		m_uiMaxSize = uiMaxSize;

	if( m_uiGrowSize != 0 )
		m_uiGrowSize = uiGrowSize;
}

//-------------------------------------------------------------------------------------------------
// 메모리 할당 크기 및 유효 크기 변경 함수 - 데이터 손실 없음, 메모리 재할당 발생
//-------------------------------------------------------------------------------------------------
template <class T>
void NkVector<T>::Resize(unsigned int uiNewSize, unsigned int uiMaxSize, unsigned int uiGrowSize)
{
	if( uiNewSize < m_uiCount )
		uiNewSize = m_uiCount;

	if( uiNewSize == m_uiAllocedSize ||
		uiNewSize == 0 )
		return;

	_Realloc(uiNewSize);

	if( m_uiMaxSize != 0 )
		m_uiMaxSize = uiMaxSize;

	if( m_uiGrowSize != 0 )
		m_uiGrowSize = uiGrowSize;

	m_uiCount = uiNewSize;
}

template <class T> inline
void NkVector<T>::FitSize()
{
	Resize();
}

template <class T> inline
void NkVector<T>::_Realloc(unsigned int uiNewSize)
{
	NkAssert(uiNewSize > 0);

	T* pNewVector;

	pNewVector = NkAlloc(T, uiNewSize);

	memset(pNewVector, 0, sizeof(T) * uiNewSize);

	if( m_uiCount > 0 )
	{
		memcpy(pNewVector, m_pBase, sizeof(T) * m_uiCount);
	}

	NkFree( m_pBase );

	m_pBase = pNewVector;
	m_uiAllocedSize = uiNewSize;
}
