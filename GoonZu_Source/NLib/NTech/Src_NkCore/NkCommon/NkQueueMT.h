//-------------------------------------------------------------------------------------------------
// NkQueueMT.h
// 엔도어즈 기술개발팀
// 이제완
//-------------------------------------------------------------------------------------------------

#pragma once

#include "NkCore.h"

template <class T>
class NkQueueMT : public NkMemory
{
public:
	static const unsigned int GROWSIZE_DOUBLE = (unsigned int)-1;
	static const unsigned int MAXSIZE_DEFAULT = (unsigned int)-1;

	NkQueueMT(unsigned int uiInitSize = 1, unsigned int uiMaxSize = MAXSIZE_DEFAULT, unsigned int uiGrowSize = GROWSIZE_DOUBLE);
	~NkQueueMT();

	inline unsigned int GetCount() const;
	inline unsigned int GetAllocedCount() const;
	inline unsigned int GetAllocedBytes() const;

	inline bool IsEmpty() const;
	inline bool IsFull() const;

	void RemoveAll();

	bool Push(const T& data);
	inline bool Pop(T& data);
	inline void Pop();

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

	CRITICAL_SECTION m_cs;
};

#include "NkQueueMT.inl"
