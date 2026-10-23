//-------------------------------------------------------------------------------------------------
// NkVector.h
// 엔도어즈 기술개발팀
// 이제완
//-------------------------------------------------------------------------------------------------

#pragma once

#include "NkCore.h"
#include "NkAlgorithm.h"

//-------------------------------------------------------------------------------------------------
// NkVector Iterator
//-------------------------------------------------------------------------------------------------
template <class T>
class NkVector_Iterator
{
public: 
	typedef NkVector_Iterator<T> _Iter;

	NkVector_Iterator() : m_pData(0) {}
	NkVector_Iterator(T* pData) : m_pData(pData) {}

	T& operator*() const { return *m_pData; }
	T* operator->() const { return m_pData; }

	_Iter& operator++() { ++m_pData; return (*this); }
	_Iter operator++(int) { T* pTemp = m_pData; ++m_pData; return (_Iter)pTemp; }

	_Iter& operator--() { --m_pData; return (*this); }
	_Iter operator--(int) { T* pTemp = m_pData; --m_pData; return (_Iter)pTemp; }

	bool operator==(const _Iter& kRight) const { return m_pData == kRight.m_pData; }
	bool operator!=(const _Iter& kRight) const { return m_pData != kRight.m_pData; }

	T* m_pData;
};

template <class T>
class NkVector_ReverseIterator
{
public: 
	typedef NkVector_ReverseIterator<T> _Iter;

	NkVector_ReverseIterator() {}
	NkVector_ReverseIterator(T* pData) : m_kIter(pData) {}
	NkVector_ReverseIterator(NkVector_Iterator<T>& kIter) : m_kIter(kIter) {}

	T& operator*() const { return *m_kIter.m_pData; }
	T* operator->() const { return m_kIter.m_pData; }

	_Iter& operator++() { --m_kIter; return (*this); }
	_Iter operator++(int) { return (_Iter)m_kIter--; }

	_Iter& operator--() { ++m_kIter; return (*this); }
	_Iter operator--(int) { return (_Iter)m_kIter++; }

	bool operator==(const _Iter& kRight) const { return m_kIter == kRight.m_kIter; }
	bool operator!=(const _Iter& kRight) const { return m_kIter != kRight.m_kIter; }

	NkVector_Iterator<T> m_kIter;
};

//-------------------------------------------------------------------------------------------------
// NkVector
//-------------------------------------------------------------------------------------------------
template <class T>
class NkVector : public NkMemory
{
public:
	typedef NkVector_Iterator<T> Iterator;
	typedef NkVector_ReverseIterator<T> ReverseIterator;

	static const unsigned int GROWSIZE_DOUBLE = (unsigned int)-1;
	static const unsigned int MAXSIZE_DEFAULT = (unsigned int)-1;
	static const unsigned int INVALID_INDEX = (unsigned int)-1;

	NkVector(unsigned int uiInitSize = 0, unsigned int uiMaxSize = MAXSIZE_DEFAULT, unsigned int uiGrowSize = GROWSIZE_DOUBLE);
	~NkVector();

	// 크기 관련 함수
	inline unsigned int GetCount() const;
	inline unsigned int GetAllocedCount() const;
	inline unsigned int GetAllocedBytes() const;

	// 할당 메모리 크기 변경 함수1
	// --> 유효 크기(사용 가능 크기)는 변하지 않음
	// --> 잦은 메모리 재할당을 방지하기위해 앞으로 사용될 크기를 메모리에 미리 잡아 놓기 위해 사용
	void Reserve(unsigned int uiNewAllocSize = 0, unsigned int uiMaxSize = 0, unsigned int uiGrowSize = 0);

	// 할당 메모리 크기 변경 함수2
	// --> 유효 크기(사용 가능 크기)도 같이 변경됨
	void Resize(unsigned int uiNewSize = 0, unsigned int uiMaxSize = 0, unsigned int uiGrowSize = 0);

	// 할당 메모리 크기 변경 함수3
	// --> Resize함수를 인자 없이 실행 한 것과 동일한 기능
	inline void FitSize();

	// Iterator
	Iterator Begin();
	Iterator End();

	ReverseIterator RBegin();
	ReverseIterator REnd();

	// 상태 관련 함수
	inline bool IsEmpty();
	inline bool IsFull();

	// 대입 함수
	void Assign(const NkVector<T>& kVector);
	void Assign(const T* pDatas, unsigned int uiCount);

	inline const NkVector<T>& operator=(const NkVector<T>& kVector);

	// 삽입 함수
	bool PushFront(const T& data);
	bool PushBack(const T& data);

	bool Insert(unsigned int uiIndex, const T& data);

	// 삭제 함수
	inline void PopFront();
	inline void PopBack();

	inline bool PopFront(T& data);
	inline bool PopBack(T& data);

	inline void Remove(Iterator& kIter);
	inline void Remove(unsigned int uiIndex);
	inline void RemoveData(const T& data);
	inline void RemoveAll();

	// 데이터 참조 함수
	inline T& GetFrontData();
	inline T* GetFrontDataPtr();

	inline T& GetBackData();
	inline T* GetBackDataPtr();

	inline T& GetData(unsigned int uiIndex);
	inline T* GetDataPtr(unsigned int uiIndex);

	inline T& operator[] (unsigned int uiIndex) const;

	// 데이터 0 초기화 함수
	inline void ClearData(unsigned int uiBegin = 0, unsigned int uiEnd = INVALID_INDEX);
	

	// 두 인덱스의 데이터 값을 서로 바꾼다
	void SwapData(unsigned int uiIndex1, unsigned int uiIndex2);

	// 검색
	inline unsigned int FindIndex(const T& data, unsigned int uiBegin = 0, unsigned int uiEnd = INVALID_INDEX) const;
	
	// 정렬(오름차순)
	inline void SortAscend(unsigned int uiBegin = 0, unsigned int uiEnd = INVALID_INDEX);

	// 정렬(내림차순)
	inline void SortDescend(unsigned int uiBegin = 0, unsigned int uiEnd = INVALID_INDEX);

	// 정렬(비교함수설정)
	inline void Sort(int (*pCompareFunc)(T&, T&), unsigned int uiBegin = 0, unsigned int uiEnd = INVALID_INDEX);

	// 랜덤 섞기
	void Shuffle(unsigned int uiBegin = 0, unsigned int uiEnd = INVALID_INDEX);

	inline T* getbaseptr() { return m_pBase; }

protected:
	static int ASCEND(T& data1, T& data2);
	static int DESCEND(T& data1, T& data2);

	inline void _Realloc(unsigned int uiNewSize);

	unsigned int m_uiAllocedSize;
	unsigned int m_uiGrowSize;
	unsigned int m_uiMaxSize;

	unsigned int m_uiCount;

	T* m_pBase;
};

#include "NkVector.inl"
