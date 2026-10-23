//-------------------------------------------------------------------------------------------------
// NkStack.h
// 엔도어즈 기술개발팀
// 이제완
//-------------------------------------------------------------------------------------------------

#pragma once

#include "NkCore.h"

template <class T>
class NkStack : public NkMemory
{
public:
	static const unsigned int GROWSIZE_DOUBLE = (unsigned int)-1;
	static const unsigned int MAXSIZE_DEFAULT = (unsigned int)-1;

	NkStack(unsigned int uiInitSize = 0, unsigned int uiMaxSize = MAXSIZE_DEFAULT, unsigned int uiGrowSize = GROWSIZE_DOUBLE);
	~NkStack();

	// 크기 관련 함수
	inline unsigned int GetCount() const;
	inline unsigned int GetAllocedCount() const;
	inline unsigned int GetAllocedBytes() const;

	// 상태 관련 함수
	inline bool IsEmpty();
	inline bool IsFull();

	inline void RemoveAll();

	bool Push(const T& data);
	inline bool Pop(T& data);
	inline void Pop();

	inline T& GetData();
	inline T* GetDataPtr();

	inline T& GetData(unsigned int uiIndex) const;
	inline T* GetDataPtr(unsigned int uiIndex) const;

	inline T& operator[] (unsigned int uiIndex) const;
	
	void Realloc(unsigned int uiNewSize, unsigned int uiGrowSize = GROWSIZE_DOUBLE, unsigned int uiMaxSize = MAXSIZE_DEFAULT);

	inline T* getbaseptr() { return m_pBase; }

protected:
	inline void _Realloc(unsigned int uiNewSize);

	unsigned int m_uiAllocedSize;
	unsigned int m_uiGrowSize;
	unsigned int m_uiMaxSize;

	unsigned int m_uiCount;
	
	T* m_pBase;
};

#include "NkStack.inl"
