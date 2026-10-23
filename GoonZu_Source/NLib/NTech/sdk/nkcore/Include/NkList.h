//-------------------------------------------------------------------------------------------------
// NkList.h
// 엔도어즈 기술개발팀
// 이제완
//-------------------------------------------------------------------------------------------------

#pragma once

#include "NkCore.h"

typedef void* NkListPos;

template <class T>
class NkListNode : public NkMemory
{
public:
	NkListNode<T>* GetNext() { return m_pkNext; }
	NkListNode<T>* GetPrev() { return m_pkPrev; }
	T* GetDataPtr() { return &m_data; }
	T& GetData() { return m_data; }

	NkListNode<T>* m_pkNext;
	NkListNode<T>* m_pkPrev;
	T m_data;
};

template <class T>
class NkList : public NkMemory
{
public:
	NkList();
	~NkList();

	// size
	inline size_t GetCount() const;
	inline size_t GetAllocedByteSize() const;

	// data access
	inline T& GetData(NkListPos kPos) const;
	inline T* GetDataPtr(NkListPos kPos) const;

	// pos
	inline NkListPos GetFrontPos() const;
	inline NkListPos GetBackPos() const;

	inline NkListPos GetNextPos(NkListPos kPos) const;
	inline NkListPos GetPrevPos(NkListPos kPos) const;

	void MoveFront(NkListPos& kPos) const;
	void MoveBack(NkListPos& kPos) const;
	void MoveNext(NkListPos& kPos) const;
	void MovePrev(NkListPos& kPos) const;

	// node
	NkListNode<T>* GetFrontNode() const { return m_pkFront; }
	NkListNode<T>* GetBackNode() const { return m_pkBack; }

	// remove
	void RemoveAll();
	void Remove(NkListPos kPos);

	// detach
	NkListNode<T>* Detach(NkListPos kPos);

	// add
	T* NewFront();
	T* NewBack();

	void PopFront();
	void PopBack();

	void PushFront(const T& data);
	void PushBack(const T& data);

	void PushNodeFront(NkListNode<T>* pkNode);
	void PushNodeBack(NkListNode<T>* pkNode);

	// insert
	T* InsertNewAfter(NkListPos kPos);
	T* InsertNewBefore(NkListPos kPos);

	NkListPos InsertAfter(NkListPos kPos, const T& data);
	NkListPos InsertBefore(NkListPos kPos, const T& data);

	NkListPos InsertNodeAfter(NkListPos kPos, NkListNode<T>* pkNode);
	NkListPos InsertNodeBefore(NkListPos kPos, NkListNode<T>* pkNode);

	// search
	NkListPos FindPos(const T& data, NkListPos kPos = 0);

protected:
	NkListNode<T>* NewNode();
	void DeleteNode(NkListNode<T>* pkNode);

	NkListNode<T>* m_pkFront;
	NkListNode<T>* m_pkBack;

	size_t m_stCount;
};

#include "NkList.inl"
