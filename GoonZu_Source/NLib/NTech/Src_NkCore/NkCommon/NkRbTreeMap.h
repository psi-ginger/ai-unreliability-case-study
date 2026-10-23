//-------------------------------------------------------------------------------------------------
// NkRbTreeMapMap.h
// 엔도어즈 기술개발팀
// 이제완
//-------------------------------------------------------------------------------------------------

#pragma once

#include "NkMemory.h"
#include "NkVector.h"


//=================================================================================================
//
// Key, Data의 추가/삭제시 별도의 메모리 할당이나 해제가 필요할 경우에는,
// PairDataSetter(데이터 생성, 소멸 함수자)를 구현해서 사용하세요
//
// 구현 방법 : 아래 소스를 복사하여 ClassName과 KeyType, DataType을 대체하여 사용
//
//=================================================================================================
/*

class ClassName
{
public:
	static inline void SetData(KeyType& destKey, DataType& destData, const KeyType& srcKey, const DataType& srcData) 
	{
		destKey = srcKey;
		destData = srcData;
	}

	static inline void DeleteData(KeyType& key, DataType& data)
	{
	}
};

*/

//-------------------------------------------------------------------------------------------------
// PairDataSetter 구현 예
//-------------------------------------------------------------------------------------------------
/*

class MyDataSetter
{
public:
	static inline void SetData(char*& destKey, int& destData, char* srcKey, int srcData) 
	{
		int size = strlen(key) + 1;
		char* pcString = NkAlloc(char, size);
		memcpy(pcString, key, size)

		destKey = pcString;
		destData = data;
	}

	static inline void DeleteData(char*& key, int& data) 
	{
		NkFree( key );
	};
};

*/

typedef void* NkRbTreeMapPos;

template <class TKey, class TData>
class NkRbTreeMapNode : public NkMemory
{
public:
	NkRbTreeMapNode() {}
	~NkRbTreeMapNode() {}
	TData* GetDataPtr() { return &m_data; }
	TData& GetData() { return m_data; }

	bool IsRedNode() { return m_bRed; }

	NkRbTreeMapNode* m_pkParent;
	NkRbTreeMapNode* m_pkLeft;
	NkRbTreeMapNode* m_pkRight;

	TKey m_key;
	TData m_data;

	bool m_bRed;
};

class NkRbTreeMapPosVector : public  NkVector<NkRbTreeMapPos>
{
public:
	NkRbTreeMapPosVector(){};
};


template <class TKey, class TData, 
		  class TComp = NkCompareFunctor<TKey>,
		  class TDataSetter = NkPairDataSetter<TKey,TData> >
class NkRbTreeMap : public NkMemory
{
public:
	NkRbTreeMap();
	~NkRbTreeMap();

	// 크기 관련 함수
	inline unsigned int GetCount() const;
	inline unsigned int GetAllocedBytes() const;
	inline unsigned int GetMaxDepth() const;

	// 위치 관련 함수
	inline bool InvalidPos(NkRbTreeMapPos kPos) const;

	inline NkRbTreeMapPos GetRootPos() const;
	inline NkRbTreeMapPos GetParentPos(NkRbTreeMapPos kPos) const;
	inline NkRbTreeMapPos GetLeftChildPos(NkRbTreeMapPos kPos) const;
	inline NkRbTreeMapPos GetRightChildPos(NkRbTreeMapPos kPos) const;

	NkRbTreeMapPos GetBeginPos() const;
	NkRbTreeMapPos GetEndPos() const;
	NkRbTreeMapPos GetNextPos(NkRbTreeMapPos kPos) const;
	NkRbTreeMapPos GetPrevPos(NkRbTreeMapPos kPos) const;
	
	NkRbTreeMapPosVector* BuildMapPosVector();

	// 데이터 참조 함수
	inline bool IsRedNode(NkRbTreeMapPos kPos) const;

	inline TKey& GetKey(NkRbTreeMapPos kPos) const;
	inline TKey* GetKeyPtr(NkRbTreeMapPos kPos) const;

	inline TData& GetData(NkRbTreeMapPos kPos) const;
	inline TData* GetDataPtr(NkRbTreeMapPos kPos) const;

	// 삽입 (이미 동일한 Key가 존재하는 경우 false반환)
	bool Insert(const TKey& key, const TData& data);

	// 삭제
	bool Remove(const TKey& key);
	void RemoveAll();

	// 검색
	NkRbTreeMapPos FindPos(const TKey& key) const;
	bool FindData(const TKey& key, TData& OUT data) const;
	TData* FindDataPtr(const TKey& key) const;

protected:
	inline NkRbTreeMapNode<TKey,TData>* _FindMin(NkRbTreeMapNode<TKey,TData>* pkNode) const;
	inline NkRbTreeMapNode<TKey,TData>* _FindMax(NkRbTreeMapNode<TKey,TData>* pkNode) const;
	inline NkRbTreeMapNode<TKey,TData>* _FindPrev(NkRbTreeMapNode<TKey,TData>* pkNode) const;
	inline NkRbTreeMapNode<TKey,TData>* _FindNext(NkRbTreeMapNode<TKey,TData>* pkNode) const;
	inline NkRbTreeMapNode<TKey,TData>* _Find(const TKey& key) const;

	inline void _InsertFixup(NkRbTreeMapNode<TKey,TData>* pkNode);
	inline void _RemoveFixup(NkRbTreeMapNode<TKey,TData>* pkNode);
	
	inline void _RotateLeft(NkRbTreeMapNode<TKey,TData>* pkNode);
	inline void _RotateRight(NkRbTreeMapNode<TKey,TData>* pkNode);

	void _RemoveRecursive(NkRbTreeMapNode<TKey,TData>* pkNode);

	NkRbTreeMapNode<TKey,TData>* _NewNode();
	void _DeleteNode(NkRbTreeMapNode<TKey,TData>* pkNode);

	NkRbTreeMapNode<TKey,TData>* NIL;
	NkRbTreeMapNode<TKey,TData>* m_pkRoot;

	unsigned int m_uiCount;

	NkRbTreeMapPosVector m_kPosVector;
};

#include "NkRbTreeMap.inl"