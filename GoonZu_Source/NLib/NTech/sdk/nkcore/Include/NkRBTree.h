//-------------------------------------------------------------------------------------------------
// NkRBTree.h
// 엔도어즈 기술개발팀
// 이제완
//-------------------------------------------------------------------------------------------------

#pragma once

#include "NkMemory.h"

typedef void* NkRBTreePos;

template <class TKey, class TData>
class NkRBTreeNode : public NkMemory
{
public:
	TData* GetDataPtr() { return &m_data; }
	TData& GetData() { return m_data; }

	bool IsRedNode() { return m_bRed; }

	NkRBTreeNode* m_pkParent;
	NkRBTreeNode* m_pkLeft;
	NkRBTreeNode* m_pkRight;
	
	TKey m_key;
	TData m_data;

	bool m_bRed;
};

template <class TKey, class TData>
class NkRBTree : public NkMemory
{
public:
	NkRBTree();
	~NkRBTree();

	// size
	inline unsigned int GetCount() const;
	inline unsigned int GetAllocedBytes() const;

	// data access
	inline T& GetData(NkRBTreePos kPos) const;
	inline T* GetDataPtr(NkRBTreePos kPos) const;

	void Insert(const TKey& key, const TData& data);

protected:
	inline void _InsertFixup(NkRBTreeNode<TKey,TData>* pkNode);
	inline void _RotateLeft(NkRBTreeNode<TKey,TData>* pkNode);
	inline void _RotateRight(NkRBTreeNode<TKey, TData>* pkNode);

	NkRBTreeNode<TKey,TData>* NewNode();
	void DeleteNode(NkRBTreeNode<TKey,TData>* pkNode);

	NkRBTreeNode<TKey,TData>* m_pkRoot;

	unsigned int m_uiCount;

}