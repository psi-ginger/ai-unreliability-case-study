//-------------------------------------------------------------------------------------------------
// NkHashMap.h
// 엔도어즈 기술개발팀
// 이제완
//-------------------------------------------------------------------------------------------------

#pragma once

#include "NkAlgorithm.h"
#include "NkVector.h"

//=================================================================================================
//
// Hash Functor(해쉬값 생성 함수자)와 Equals Functor(키 값 비교 함수자)를 구현해서 사용하세요
//
// 구현방법 : 아래 소스를 복사하여 ClassName과 Type을 대체하여 사용
//
//=================================================================================================
/*

//-------------------------------------------------------------------------------------------------
// Hash Functor
//-------------------------------------------------------------------------------------------------
class ClassName
{
public:
	inline static unsigned int KeyToHashIndex(const Type& key, unsigned int uiHashSize)
	{
		return key % uiHashSize;
	}
};


//-------------------------------------------------------------------------------------------------
// Equals Functor
//-------------------------------------------------------------------------------------------------
class ClassName
{
public:
	inline static bool Equals(const Type& a, const Type& b)
	{
		return a == b;
	}
};

*/

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
	static inline void SetData(char*& destKey, int& destData, char*& srcKey, int& srcData) 
	{
		size_t size = strlen(key) + 1;
		char* pcString = NkAlloc(char, size);
        memcpy(pcString, key, size);
		
		destKey = pcString;
		destData = data;
	}

	static inline void DeleteData(char*& key, int& data) 
	{
		NkFree( key );
	};
};

*/

//-------------------------------------------------------------------------------------------------
// NkHashMapNode
//-------------------------------------------------------------------------------------------------
template <class TKey, class TData>
class NkHashMapNode : public NkMemory
{
public:
	TKey& GetKey() { return m_key; }
	TData& GetData() { return m_data; }

	NkHashMapNode* m_pkNext;
	TKey m_key;
	TData m_data;
};

//-------------------------------------------------------------------------------------------------
// NkHashMap_Iterator
//-------------------------------------------------------------------------------------------------
template <class TKey, class TData>
class NkHashMap_Iterator : public NkMemory
{
public:
	typedef NkHashMapNode<TKey,TData> _Node;
	typedef NkHashMap_Iterator<TKey,TData> _Iter;
	typedef NkVector<_Node*> _HashTable;

	NkHashMap_Iterator() : m_pkNode(0), m_pkHashTable(0), m_uiHashIndex(0) {};
	NkHashMap_Iterator(_Node* pkNode) { m_pkNode = pkNode; }

	_Node& operator*() const { return *m_pkNode; }
	_Node* operator->() const { return m_pkNode; }

	inline void _Next();
	_Iter& operator++() { _Next(); return (*this); }
	_Iter operator++(int) { _Node* pkTemp = m_pkNode; _Next(); return pkTemp; }

	bool operator==(const _Iter& kRight) const { return m_pkNode == kRight.m_pkNode; }
	bool operator!=(const _Iter& kRight) const { return m_pkNode != kRight.m_pkNode; }

	bool IsValid() { return m_pkNode != 0; }
	_Node* GetNodePtr() { return m_pkNode; }
	unsigned int GetHashIndex() { return m_uiHashIndex; }
	
	_Node* m_pkNode;
	_HashTable* m_pkHashTable;
	unsigned int m_uiHashIndex;	
};

//-------------------------------------------------------------------------------------------------
// NkHashMap
//-------------------------------------------------------------------------------------------------
template <class TKey, class TData,
          class THash = NkHashFunctor<TKey>,
		  class TEquals = NkEqualsFunctor<TKey>,
		  class TDataSetter = NkPairDataSetter<TKey, TData> >
class NkHashMap : public NkMemory
{
public:
	typedef NkHashMapNode<TKey,TData>* NodePtr;
	typedef NkHashMap_Iterator<TKey,TData> Iterator;

	NkHashMap(unsigned int uiHashSize = 37);
	~NkHashMap();

	// 크기 관련 함수
	inline unsigned int GetHashSize() const;
	inline unsigned int GetCount() const;
	inline unsigned int GetAllocedBytes() const;

	// 특정 해쉬 인덱스의 노드 개수 얻기
	inline unsigned int GetNodeCount(unsigned int uiHashIndex) const;
	
	// 위치 관련 함수
	inline bool GetBegin( Iterator& kIter) const;
	inline bool GetNext( Iterator& kIter) const;

	// 데이터 입력 함수
	inline void SetAt(const TKey& key, const TData& data);
	inline void SetAt(const TKey& key, const TData* pData);

	inline void SetData(const Iterator& kIter, const TData& data);
	inline void SetData(const Iterator& kIter, const TData* pData);

	// 특정 HashIndex에 데이터 추가하기
	// 주의하여 사용:
	//     Find 함수를 먼저 호출 하여 반환값이 false일 경우(동일한 키가 없는 경우)에만
	//     Find 함수에서 받은 Iterator를 사용하여 Insert 함수를 호출할 것

	inline void Insert( Iterator& kIter, const TKey& key, const TData& data);
	inline void Insert( Iterator& kIter, const TKey& key, const TData* pData);

	// 삭제 함수
	inline bool Remove(const TKey& key);
	inline bool Remove( Iterator& kIter);
	inline void RemoveAll();

	// 데이터 참조 함수
	inline bool GetData(const TKey& key,  TData& data) const;
	inline TData* GetDataPtr(const TKey& key) const;

	// 검색 함수
	inline bool Find(const TKey& key,  Iterator& kIter) const;

protected:
	inline NodePtr _NewNode();
	inline void _DeleteNode(NodePtr pkNode);

	unsigned int m_uiHashSize;
	unsigned int m_uiCount;
	
	NkVector<NodePtr>* m_pkHashTable;
};

#include "NkHashMap.inl"
