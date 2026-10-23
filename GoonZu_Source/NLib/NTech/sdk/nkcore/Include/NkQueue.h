//-------------------------------------------------------------------------------------------------
// NkQueue.h
// 엔도어즈 기술개발팀
// 이제완
//-------------------------------------------------------------------------------------------------

#pragma once

#include "NkCore.h"

template <class T>
class NkQueue : public NkMemory
{
public:
	static const unsigned int GROWSIZE_DOUBLE = (unsigned int)-1;
	static const unsigned int MAXSIZE_DEFAULT = (unsigned int)-1;

	NkQueue(unsigned int uiInitSize = 0, unsigned int uiMaxSize = MAXSIZE_DEFAULT, unsigned int uiGrowSize = GROWSIZE_DOUBLE);
	~NkQueue();

	// 크기 관련 함수
	inline unsigned int GetCount() const;
	inline unsigned int GetAllocedCount() const;
	inline unsigned int GetAllocedBytes() const;

	// 상태 관련 함수
	inline bool IsEmpty() const;
	inline bool IsFull() const;

	// 삽입
	bool Push(const T& data);			// PushBack

	// 삭제
	inline void Pop();					// PopFront
	inline bool Pop(T& data);			// PopFront

	void RemoveAll();

	// 데이터 참조 함수
	inline T& GetData() const;			// GetFrontData
	inline T* GetDataPtr() const;		// GetFrontDataPtr

	inline T& GetData(unsigned int uiIndex) const;			// 인덱스 0 = Front
	inline T* GetDataPtr(unsigned int uiIndex) const;		// 인덱스 0 = Front

	inline T& operator[] (unsigned int uiIndex) const;		// 인덱스 0 = Front

	inline T* getbaseptr() { return m_pBase; }
	inline unsigned int getfrontpos() { return m_uiFrontPos; }
	inline unsigned int getbackpos() { return m_uiBackPos; }

protected:
	inline void _Realloc(unsigned int uiNewSize);

	unsigned int m_uiAllocedSize;
	unsigned int m_uiGrowSize;
	unsigned int m_uiMaxSize;

	unsigned int m_uiCount;
	unsigned int m_uiFrontPos;
	unsigned int m_uiBackPos;

	T* m_pBase;
};

#include "NkQueue.inl"
